/*
 * Copyright (c) 2022-2025, Texas Instruments Incorporated
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

/* Number of microseconds between each FreeRTOS OS tick */
#define FREERTOS_TICKPERIOD_US (1000000 / configTICK_RATE_HZ)

/* Max number of ClockP ticks into the future supported by this ClockP
 * implementation.
 * Under the hood, ClockP uses the SysTimer whose events trigger immediately if
 * the compare value is less than 2^22 systimer ticks in the past
 * (4.194sec at 1us resolution). Therefore, the max number of SysTimer ticks you
 * can schedule into the future is 2^32 - 2^22 - 1 ticks (~= 4290 sec at 1us
 * resolution).
 */
#define MAX_SYSTIMER_DELTA (0xFFBFFFFFU)

/* Clock frequency divider used as a conversion factor when working at a lower
 * clock rate on FPGA.
 */
#define CLOCK_FREQUENCY_DIVIDER (96000000U / configCPU_CLOCK_HZ)

#define SYSTIMER_CHANNEL_COUNT (5U)

/* This global is used to pass the expected ticks that the OS will sleep from
 * vPortSuppressTicksAndSleep() to the Power policy.
 */
static volatile uint32_t PowerCC27XX_idleTimeOS = 0U;

/* Shift values to convert between the different resolutions of the SysTimer
 * channels. Channel 0 can technically support either 1us or 250ns, but it is
 * used by ClockP with a resolution of 1us.
 */
const uint8_t sysTimerResolutionShift[SYSTIMER_CHANNEL_COUNT] = {
    0, /* 1us */
    0, /* 1us */
    2, /* 250ns -> 1us */
    2, /* 250ns -> 1us */
    2  /* 250ns -> 1us */
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
    uint32_t osDelta;
    uint32_t sysTimerDelta;
    uint32_t sysTimerIMASK;
    uint32_t sysTimerLoopDelta;
    uint32_t sysTimerCurrTime;
    uint8_t sysTimerIndex;
    uint32_t sysTickDelta;
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
        sysTimerCurrTime = HWREG(SYSTIM_BASE + SYSTIM_O_TIME1U);

        /* We only want to check the SysTimer channels if at least one of them
         * is active. It may be that no one is using ClockP or RCL in this
         * application or they have not been initialised yet.
         */
        if (sysTimerIMASK != 0)
        {
            /* Set initial SysTimer delta to max possible value. It needs to be
             * this large since we will shrink it down to the soonest timeout with
             * Math_MIN() comparisons.
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
                    /* Store current channel timeout in native channel
                     * resolution. Read CHnCCSR to avoid clearing any pending
                     * events as side effect of reading CHnCC.
                     */
                    sysTimerLoopDelta = HWREG(SYSTIM_BASE + SYSTIM_O_CH0CCSR + (sysTimerIndex * sizeof(uint32_t)));

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

                    /* If sysTimerDelta is larger than MAX_SYSTIMER_DELTA, the
                     * compare event happened in the past and we need to abort
                     * entering standby to handle the timeout instead of waiting
                     * a really long time.
                     */
                    if (sysTimerLoopDelta > MAX_SYSTIMER_DELTA)
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
            sysTimerDelta = MAX_SYSTIMER_DELTA;
        }

        /* Check soonestDelta wake time pending for FreeRTOS in 1us resolution */
        osDelta = PowerCC27XX_idleTimeOS * FREERTOS_TICKPERIOD_US;

        /* Get soonestDelta wake time and corresponding ClockP timeout */
        soonestDelta = Math_MIN(sysTimerDelta, osDelta);

        /* Check soonestDelta time vs STANDBY latency */
        if (soonestDelta > PowerCC27XX_TOTALTIMESTANDBY)
        {
            Log_printf(LogModule_Power,
                       Log_INFO,
                       "PowerCC27XX_standbyPolicy: Entering standby. Soonest timeout = 0x%x",
                       (soonestDelta + sysTimerCurrTime));

            /* Disable scheduling */
            PowerCC27XX_schedulerDisable();

            /* Since the CPU and thus the SysTick does not have retention and
             * is instead restored programmatically in ROM, the SysTick counter
             * will be corrupted in standby.
             * Since we cannot actually directly write to the counter value
             * to restore it but can set it to 0, we can instead set the
             * reload value to the current value, force a reload of the counter,
             * and then change the reload value back to the FreeRTOS tick
             * period.
             */
            sysTickDelta = SysTickGetValue();

            /* Save SysTimer tick count before sleep */
            ticksBefore = sysTimerCurrTime;

            /* Flush any remaining log messages in the ITM */
            ITM_flush();

            PowerLPF3_sleep(soonestDelta + sysTimerCurrTime);

            /* Restore ITM settings */
            ITM_restore();

            /* Restore SysTick if it was disabled during PowerLPF3_sleep() */
            if ((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) == 0)
            {

                /* Restore SysTick counter value by setting the period to the
                 * stored value from before entering standby.
                 * Do not use SysTickSetPeriod() since that does not allow 0 as
                 * an input, which we might get as a return value from
                 * SysTickGetValue().
                 */
                HWREG(SYSTICK_BASE + SYSTICK_O_RVR) = sysTickDelta;

                /* Get SysTimer tick count after sleep */
                ticksAfter = HWREG(SYSTIM_BASE + SYSTIM_O_TIME1U);

                /* Calculate elapsed FreeRTOS tick periods in STANDBY */
                sleptTicks = (ticksAfter - ticksBefore) * CLOCK_FREQUENCY_DIVIDER;

                sleptTicks = sleptTicks / FREERTOS_TICKPERIOD_US;

                /* Update FreeRTOS tick count for time spent in STANDBY */
                vTaskStepTick(sleptTicks);

                /* Reset SysTick counter to 0 so it reloads with the remaining
                 * timeout set above.
                 */
                HWREG(SYSTICK_BASE + SYSTICK_O_CVR) = 0;

                /* Restart FreeRTOS ticks */
                SysTickEnable();

                /* Set the SysTick period back to the FreeRTOS tick period for
                 * the next and all subsequent timeouts.
                 * Interrupts must still be disabled in between re-enabling the
                 * SysTick to force a reload of the counter and changing the period
                 * back to the FreeRTOS tick period.
                 * Otherwise, a small leftover counter value could cause the reload
                 * value to be low enough to infinitely trigger the ISR since we
                 * do not reach the line below before timing out again.
                 * If the timeout was small, it will time out and pend the
                 * interrupt but not vector to it. Until after the new reload value
                 * is applied.
                 * We will only vector to the ISR once regardless of whether the
                 * counter times out more than once.
                 */
                SysTickSetPeriod(configCPU_CLOCK_HZ / configTICK_RATE_HZ);
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
