/*
 * Copyright (c) 2022-2026, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== PowerCC27XX_freertos.c ========
 */

#include <stdbool.h>

/* Driver header files */
#include <ti/drivers/ITM.h>
#include <ti/drivers/Power.h>

/* Utilities header files */
#include <ti/drivers/utils/Math.h>

/* FreeRTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

/* Log header file */
#include <ti/log/Log.h>

/* Driverlib header files */
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_systim.h)
#include DeviceFamily_constructPath(cmsis/core/cmsis_compiler.h)
#include DeviceFamily_constructPath(driverlib/systick.h)
#include DeviceFamily_constructPath(driverlib/systimer.h)

/* Number of microseconds between each FreeRTOS OS tick */
#define FREERTOS_TICKPERIOD_US (1000000 / configTICK_RATE_HZ)

/* Number of CPU clock cycles per FreeRTOS tick. In other words, the FreeRTOS
 * tick period measured in CPU clock cycles.
 */
#define CPU_CLOCKS_PER_FREERTOS_TICK (configCPU_CLOCK_HZ / configTICK_RATE_HZ)

/* Number of CPU clock cycles per microsecond */
#define CPU_CLOCKS_PER_US (configCPU_CLOCK_HZ / 1000000U)

/* This global is used to pass the expected ticks that the OS will sleep from
 * vPortSuppressTicksAndSleep() to the Power policy.
 */
static volatile uint32_t PowerCC27XX_idleTimeOS = 0U;

/* Shift values to convert between the different resolutions of the SysTimer
 * channels. Channel 0 and 5 can technically support either 1 us or 250 ns.
 * Channel 0 is used by ClockP with a resolution of 1 us and channel 5 is also
 * assumed to have a resolution of 1 us.
 */
const uint8_t sysTimerResolutionShift[SYSTIMER_CHANNEL_COUNT] = {
    0, /* 1 us */
    0, /* 1 us */
    2, /* 250 ns -> 1 us */
    2, /* 250 ns -> 1 us */
    2, /* 250 ns -> 1 us */
    0, /* 1 us */
};

/*
 *  ======== PowerCC27XX_standbyPolicy ========
 */
void PowerCC27XX_standbyPolicy(void)
{
#if (configUSE_TICKLESS_IDLE != 0)
    eSleepModeStatus eSleep;
    uint32_t constraints;
    uint32_t ticksBefore;
    uint32_t ticksAfter;
    uint32_t sleptTicks;
    uint32_t soonestDelta;
    uint64_t osDelta;
    uint32_t sysTimerDelta;
    uint32_t sysTimerIMASK;
    uint32_t sysTimerLoopDelta;
    uint32_t sysTimerCurrTime;
    uint8_t sysTimerIndex;
    int32_t sysTickValue;      /* The SysTick counter is only 24 bits wide, so
                                * it will fit in signed 32 bits.
                                */
    uint32_t missedTicks = 0U; /* Number of missed SysTick interrupts from the
                                * time interrupts are disabled until
                                * sysTickValue is set.
                                */
    uint32_t standbyDuration;
    uintptr_t key;
    bool standbyAllowed;
    bool idleAllowed;

    key = __get_PRIMASK();
    __set_PRIMASK(1);

    /* Final check with FreeRTOS to make sure it is still OK to go to sleep... */
    eSleep = eTaskConfirmSleepModeStatus();
    if (eSleep == eAbortSleep)
    {
        __set_PRIMASK(key);
        return;
    }

    /* Check state of constraints */
    constraints    = Power_getConstraintMask();
    standbyAllowed = (constraints & (1U << PowerLPF3_DISALLOW_STANDBY)) == 0U;
    idleAllowed    = (constraints & (1U << PowerLPF3_DISALLOW_IDLE)) == 0U;

    if (standbyAllowed && (PowerLPF3_isLfincFilterAllowingStandby() == false))
    {
        /* We cannot enter standby until LFINC filter has settled, we also
         * cannot enter idle instead of standby because otherwise we could end
         * up waiting for the next standby wakeup signal from SysTimer or
         * another wakeup source while we are still in idle. That could be a
         * very long time. But if standby is currently disallowed from the
         * constraints, that means we do want to enter idle since something set
         * that constraint and will lift it again.
         */
        standbyAllowed = false;
        idleAllowed    = false;

        Log_printf(LogModule_Power,
                       Log_INFO,
                       "PowerCC27XX_standbyPolicy: LFINC filter is not guaranteed to have settled yet. Standby and Idle are not allowed yet.");
    }

    Log_printf(LogModule_Power,
               Log_VERBOSE,
               "PowerCC27XX_standbyPolicy: Standby constraint count = %d. Idle constraint count = %d",
               Power_getConstraintCount(PowerLPF3_DISALLOW_STANDBY),
               Power_getConstraintCount(PowerLPF3_DISALLOW_IDLE));

    /* Do quick check to see if only WFI allowed; if yes, do it now. */
    if (standbyAllowed)
    {
        /* If we are allowed to enter standby, check whether the next timeout is
         * far enough away for it to make sense.
         */

        /* Get SysTimer IMASK state */
        sysTimerIMASK = HWREG(SYSTIM_BASE + SYSTIM_O_IMASK);

        /* Get current time in 1us resolution */
        sysTimerCurrTime = SysTimerGetTime1Us();

        /* Store current SysTick value, it will be used to restore SysTick
         * after waking up. This should be done as close as possible to
         * getting the SysTimer time above.
         */
        sysTickValue = SysTickGetValue();

        if ((SCB->ICSR & SCB_ICSR_PENDSTSET_Msk) != 0U)
        {
            /* The SysTick interrupt is pending. This has happened after
             * interrupts were disabled earlier in this function. It might
             * also have happened after reading the current SysTick value
             * above. So read the SysTick value again, but also the SysTimer
             * value to ensure they are read in close succession.
             * This pending interrupt means we have missed a tick, keep track
             * of that missed tick.
             * Note, do not clear the pending SysTick interrupt here, that is
             * done later after sleeping if the SysTick gets disabled.
             */
            missedTicks++;

            /* Read SysTimer and SysTick again. */
            sysTimerCurrTime = SysTimerGetTime1Us();
            sysTickValue     = SysTickGetValue();
        }

        if (sysTickValue == 0)
        {
            /* If SysTick is currently at 0, the next interrupt is actually a
             * full FreeRTOS tick period in the future.
             */
            sysTickValue = CPU_CLOCKS_PER_FREERTOS_TICK;
        }

        /* We only want to check the SysTimer channels if at least one of them
         * is active. It may be that no one is using ClockP or RCL in this
         * application or they have not been initialised yet.
         */
        if (sysTimerIMASK != 0)
        {
            /* Set initial SysTimer delta to max possible value. It needs to be
             * this large since we will shrink it down to the soonest timeout
             * with Math_MIN() comparisons.
             */
            sysTimerDelta = 0xFFFFFFFF;

            /* Loop over all SysTimer channels and compute the soonest timeout.
             * Since the channels have different time bases (1us vs 250ns),
             * we need to shift all of that to a 1us time base to compare them.
             * If no channel is active, we will use the max timeout value
             * supported by the SysTimer.
             */
            for (sysTimerIndex = 0; sysTimerIndex < SYSTIMER_CHANNEL_COUNT; sysTimerIndex++)
            {
                if (sysTimerIMASK & (1 << sysTimerIndex))
                {
                    /* Get current channel timeout in native channel
                     * resolution.
                     */
                    sysTimerLoopDelta = SysTimerGetCaptureCompareValue(sysTimerIndex);

                    /* Convert current time from 1us to native resolution and
                     * subtract from timeout to get delta in in native channel
                     * resolution.
                     * We compute the delta in the native resolution
                     * to correctly handle wrapping and underflow at the 32-bit
                     * boundary.
                     * To simplify code paths and SRAM, we shift up the 1us
                     * resolution time stamp instead of reading out and keeping
                     * track of the 250ns time stamp and associating that with
                     * 250ns channels. The loss of resolution for wakeup is not
                     * material as we wake up sufficiently early to handle
                     * timing jitter in the wakeup duration.
                     */
                    sysTimerLoopDelta -= sysTimerCurrTime << sysTimerResolutionShift[sysTimerIndex];

                    /* If sysTimerDelta is larger than SYSTIMER_MAX_DELTA, the
                     * compare event happened in the past and we need to abort
                     * entering standby to handle the timeout instead of waiting
                     * a really long time.
                     */
                    if (sysTimerLoopDelta > SYSTIMER_MAX_DELTA)
                    {
                        sysTimerLoopDelta = 0;
                    }

                    /* Convert delta to 1us resolution */
                    sysTimerLoopDelta = sysTimerLoopDelta >> sysTimerResolutionShift[sysTimerIndex];

                    /* Update the smallest SysTimer delta */
                    sysTimerDelta = Math_MIN(sysTimerDelta, sysTimerLoopDelta);
                }
            }
        }
        else
        {
            /* None of the SysTimer channels are active. Use the maximum
             * SysTimer delta instead. That lets us sleep for at least this
             * long if the OS timeout is even longer.
             */
            sysTimerDelta = SYSTIMER_MAX_DELTA;
        }

        if (missedTicks >= PowerCC27XX_idleTimeOS)
        {
            /* If we have already missed the expected idle time, set the OS
             * delta to 0 to avoid underflow below.
             */
            osDelta = 0;
        }
        else
        {
            /* Check soonestDelta wake time pending for FreeRTOS in 1us
             * resolution. The next tick is not necessarily a full tick period
             * in the future. To account for this, the time that has already
             * passed since the last tick is subtracted.
             * Also cap value at SYSTIMER_MAX_DELTA to avoid overflow, since
             * that is the maximum value that can be used for the standby
             * duration below.
             * Note, to make sure the multiplication doesn't result in an
             * overflow, osDelta is declared as a 64-bit value since
             * PowerCC27XX_idleTimeOS can be up to the maximum value of
             * TickType_t, which can be up to an unsigned 32-bit integer, and
             * that multiplied by FREERTOS_TICKPERIOD_US can overflow 32 bits.
             */
            osDelta = (uint64_t)(PowerCC27XX_idleTimeOS - missedTicks) * FREERTOS_TICKPERIOD_US -
                      ((CPU_CLOCKS_PER_FREERTOS_TICK - sysTickValue) / CPU_CLOCKS_PER_US);
            if (osDelta > SYSTIMER_MAX_DELTA)
            {
                osDelta = SYSTIMER_MAX_DELTA;
            }
        }

        /* Get soonestDelta wake time and corresponding ClockP timeout.
         * Casting osDelta to uint32_t is safe since the value is capped
         * at SYSTIMER_MAX_DELTA which fits in uint32_t.
         */
        soonestDelta = Math_MIN(sysTimerDelta, (uint32_t)osDelta);

        /* Check soonestDelta time vs STANDBY latency */
        if (soonestDelta > PowerCC27XX_TOTALTIMESTANDBY)
        {
            Log_printf(LogModule_Power,
                       Log_INFO,
                       "PowerCC27XX_standbyPolicy: Entering standby. Soonest timeout = 0x%x",
                       (soonestDelta + sysTimerCurrTime));

            /* Disable scheduling */
            PowerCC27XX_schedulerDisable();

            /* Save SysTimer tick count before sleep */
            ticksBefore = sysTimerCurrTime;

            /* Flush any remaining log messages in the ITM */
            ITM_flush();

            /* Go to standby mode.
             * Note, overflow in the addition is possible, but since we
             * guarantee that soonestDelta is at most SYSTIMER_MAX_DELTA,
             * this is safe.
             */
            PowerLPF3_sleep(soonestDelta + sysTimerCurrTime);

            /* Restore ITM settings */
            ITM_restore();

            /* Restore SysTick if it was disabled during PowerLPF3_sleep() */
            if ((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) == 0)
            {
                /* Clear any pending SysTick interrupts, that might have been
                 * triggered before SysTick was disabled in PowerLPF3_sleep().
                 */
                SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk;

                /* Reset SysTick counter to 0 so it reloads with the remaining
                 * timeout set later below. This can be done without knowing the
                 * standby duration, so it is done before reading the SysTimer
                 * time, to reduce the time between reading the SysTimer to
                 * enabling SysTick.
                 */
                HWREG(SYSTICK_BASE + SYSTICK_O_CVR) = 0;

                /* Get SysTimer tick count after sleep. This should be as close
                 * to the SysTickEnable() call as possible.
                 */
                ticksAfter = SysTimerGetTime1Us();

                /* Calculate elapsed time in standby */
                standbyDuration = (ticksAfter - ticksBefore);

                /* Compute elapsed FreeRTOS tick periods in standby */
                sleptTicks = standbyDuration / FREERTOS_TICKPERIOD_US;

                /* Some precision is lost in the division when calculating
                 * sleptTicks above. Use the remainder to compute new SysTick
                 * counter value. A negative value will be handled in if
                 * statement below.
                 */
                sysTickValue -= (standbyDuration % FREERTOS_TICKPERIOD_US) * CPU_CLOCKS_PER_US;

                if (sysTickValue <= 0)
                {
                    /* The SysTick counter value is computed to be negative or
                     * zero, meaning the "next" tick would be in the past (or
                     * now), which is not possible. Instead the counter value is
                     * increased by one FreeRTOS tick period to make it
                     * positive. To account for this change, sleptTicks is
                     * incremented by one.
                     */
                    sysTickValue += CPU_CLOCKS_PER_FREERTOS_TICK;
                    sleptTicks++;
                }

                /* Set initial period since we cannot set the counter value
                 * directly.
                 */
                SysTickSetPeriod(sysTickValue);

                /* Restart FreeRTOS ticks */
                SysTickEnable();

                /* Set the SysTick period to the previous value for next
                 * timeout. Interrupts must still be disabled in between
                 * re-enabling the SysTick to force a reload of the counter and
                 * changing the period back to the FreeRTOS tick period.
                 * Otherwise, a small leftover counter value could cause the
                 * reload value to be low enough to infinitely trigger the ISR
                 * since we do not reach the line below before timing out again.
                 * If the timeout was small, it will time out and pend the
                 * interrupt but not vector to it. Until after the new reload
                 * value is applied. We will only vector to the ISR once
                 * regardless of whether the counter times out more than once.
                 */
                SysTickSetPeriod(CPU_CLOCKS_PER_FREERTOS_TICK);

                /* Account for missed ticks at the start of this function. */
                sleptTicks += missedTicks;

                /* Update FreeRTOS tick count for time spent in STANDBY.
                 * This is done after enabling SysTick to reduce time between
                 * reading SysTimer and enabling SysTick.
                 * Note: FreeRTOS expects that the value passed to
                 * vTaskStepTick() is at most PowerCC27XX_idleTimeOS. This is
                 * true since the wakeup time is selected such that we wakeup
                 * before the scheduled tick. For the same reason, it is safe to
                 * cast to TickType_t here since sleptTicks is at most
                 * PowerCC27XX_idleTimeOS, which is at most the maximum value of
                 * TickType_t.
                 */
                vTaskStepTick((TickType_t)sleptTicks);
            }

            /* Re-enable scheduling. Also re-enables interrupts.
             * This must happen only after restoring the SysTimer because
             * otherwise a Clock might be started in an ISR and then the Power
             * policy clobbers the hardware state.
             * This must also be called only after clearing the RTC event.
             * Otherwise, when interrupts are enabled in this fxn, the CPU
             * will immediately vector to CPUIRQ16, which still has the ClockP
             * ISR registered.
             */
            PowerCC27XX_schedulerRestore();

            Log_printf(LogModule_Power,
                       Log_INFO,
                       "PowerCC27XX_standbyPolicy: Exiting standby. Time after = 0x%x",
                       ticksAfter);
        }
        else if (idleAllowed)
        {
            /* If we would be allowed to enter standby but there is not enough
             * time for it to make sense from an overhead perspective, enter
             * idle instead.
             */
            Log_printf(LogModule_Power,
                   Log_INFO,
                   "PowerCC27XX_standbyPolicy: Only WFI allowed");

            PowerCC27XX_doWFI();
        }
    }
    else if (idleAllowed)
    {
        /* We are not allowed to enter standby.
         * Enter idle instead if it is allowed.
         */
        Log_printf(LogModule_Power,
                   Log_INFO,
                   "PowerCC27XX_standbyPolicy: Only WFI allowed");

        PowerCC27XX_doWFI();
    }

    __set_PRIMASK(key);
#endif
}

/*
 *  ======== PowerCC27XX_schedulerDisable ========
 */
void PowerCC27XX_schedulerDisable()
{
    vTaskSuspendAll();
}

/*
 *  ======== PowerCC27XX_schedulerRestore ========
 */
void PowerCC27XX_schedulerRestore()
{
    xTaskResumeAll();
}

/*
 *  ======== vPortSuppressTicksAndSleep ========
 *  FreeRTOS hook function invoked when Idle when configUSE_TICKLESS_IDLE
 */
void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    /* Stash FreeRTOS' expected idle time */
    PowerCC27XX_idleTimeOS = xExpectedIdleTime;

    /*
     * call Power-driver-specified idle function, to conditionally invoke the
     * Power policy
     */
    Power_idleFunc();
}
