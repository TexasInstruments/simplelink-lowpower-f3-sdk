/*
 * Copyright (c) 2021-2023, Texas Instruments Incorporated
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
 *  ======== PowerCC23X0_freertos.c ========
 */

#include <stdbool.h>

/* Driver header files */
#include <ti/drivers/Power.h>

/* Utilities header files */
#include <ti/drivers/utils/Math.h>

/* DPL header files */
#include <ti/drivers/dpl/ClockP.h>
#include "ClockPLPF3.h"
#include <ti/drivers/dpl/HwiP.h>

/* FreeRTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

/* Driverlib header files */
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/systick.h)
#include DeviceFamily_constructPath(driverlib/cpu.h)
#include DeviceFamily_constructPath(driverlib/lrfd.h)
#include DeviceFamily_constructPath(cmsis/core/cmsis_compiler.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_systim.h)
#include DeviceFamily_constructPath(inc/hw_rtc.h)
#include DeviceFamily_constructPath(inc/hw_evtsvt.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_ckmd.h)

/* Function prototypes */
extern int_fast16_t PowerCC23X0_notify(uint_fast16_t eventType);

/* Number of micro seconds between each FreeRTOS OS tick */
#define FREERTOS_TICKPERIOD_US (1000000 / configTICK_RATE_HZ)

/* Max number of ClockP ticks into the future supported by this ClockP
 * implementation.
 * Under the hood, ClockP uses the SysTimer whose events trigger immediately if
 * the compare value is less than 2^22 systimer ticks in the past
 * (4.194sec at 1us resolution). Therefore, the max number of SysTimer ticks you
 * can schedule into the future is 2^32 - 2^22 - 1 ticks (~= 4290 sec at 1us
 * resolution).
 */
#define MAX_SYSTIMER_DELTA 0xFFBFFFFFU

/* Clock frequency divider used as a conversion factor when working at a lower
 * clock rate on FPGA.
 */
#define CLOCK_FREQUENCY_DIVIDER (48000000U / configCPU_CLOCK_HZ)

/* The RTC has a time base of 8us and the SysTimer uses 1us or 250ns. The
 * conversion factor assumes that the SysTimer values have been converted to 1us
 * already.
 */
#define RTC_TO_SYSTIMER_TICKS 8U

#define SYSTIMER_CHANNEL_COUNT 5U

/* This global is used to pass the expected ticks that the OS will sleep from
 * vPortSuppressTicksAndSleep() to the Power policy.
 */
static volatile uint32_t PowerCC23X0_idleTimeOS = 0;

/* Global to stash the SysTimer timeouts while we enter standby */
uint32_t sysTimerTimeouts[SYSTIMER_CHANNEL_COUNT];

/* Shift values to convert between the different resolutions of the SysTimer
 * channels. Channel 0 can technically support either 1us or 250ns. Until the
 * channel is actively used, we will hard-code it to 1us resolution to improve
 * runtime.
 */
const uint8_t sysTimerResolutionShift[SYSTIMER_CHANNEL_COUNT] = {
    0, /* 1us */
    0, /* 1us */
    2, /* 250ns -> 1us */
    2, /* 250ns -> 1us */
    2  /* 250ns -> 1us */
};

/*
 *  ======== PowerCC23X0_standbyPolicy ========
 */
void PowerCC23X0_standbyPolicy(void)
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

    key = HwiP_disable();

    /* Final check with FreeRTOS to make sure still OK to go to sleep... */
    eSleep = eTaskConfirmSleepModeStatus();
    if (eSleep == eAbortSleep)
    {
        HwiP_restore(key);
        return;
    }

    /* Check state of constraints */
    constraints    = Power_getConstraintMask();
    standbyAllowed = (constraints & (1 << PowerLPF3_DISALLOW_STANDBY)) == 0;
    idleAllowed    = (constraints & (1 << PowerLPF3_DISALLOW_IDLE)) == 0;

    /* If we are using LFOSC, we need to wait for the LFINC filter to settle
     * before entering standby. We also cannot enter idle instead of standby
     * because otherwise we could end up waiting for the next standby wakeup
     * signal from the RTC or another wakeup source while we are still in idle.
     * That could be a very long time.
     * But if standby is currently disallowed from the constraints, that means
     * we do want to enter idle since something set that constraint and will
     * lift it again.
     */
    if ((HWREG(CKMD_BASE + CKMD_O_LFCLKSEL) & CKMD_LFCLKSEL_MAIN_M) == CKMD_LFCLKSEL_MAIN_LFOSC)
    {
        if (((HWREG(CKMD_BASE + CKMD_O_LFCLKSTAT) & CKMD_LFCLKSTAT_FLTSETTLED_M) == false) && (standbyAllowed == true))
        {
            standbyAllowed = false;
            idleAllowed    = false;
        }
    }

    /* Do quick check to see if only WFI allowed; if yes, do it now. */
    if (standbyAllowed)
    {
        /* If we are allowed to enter standby, check whether the next timeout is
         * far enough away for it to make sense.
         */

        /* Save SysTimer IMASK to restore afterwards */
        sysTimerIMASK = HWREG(SYSTIM_BASE + SYSTIM_O_IMASK);

        /* We only want to check the SysTimer channels if at least one of them
         * is active. It may be that no one is using ClockP or RCL in this
         * application or they have not been initialised yet.
         */
        if (sysTimerIMASK != 0)
        {
            /* Set initial SysTimer delta to max possible value. It needs to be
             * this large since we will shrink it down to the soonest timeout with
             * Math_MIN() comparisons
             */
            sysTimerDelta = 0xFFFFFFFF;

            /* Get current time in 1us resolution */
            sysTimerCurrTime = HWREG(SYSTIM_BASE + SYSTIM_O_TIME1U);

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
                    /* Stash SysTimer channel compare value */
                    sysTimerTimeouts[sysTimerIndex] = HWREG(SYSTIM_BASE + SYSTIM_O_CH0CC + sysTimerIndex * sizeof(uint32_t));

                    /* Store current channel timeout in native channel resolution */
                    sysTimerLoopDelta = sysTimerTimeouts[sysTimerIndex];

                    /* Convert current time from 1us to native resolution and subtract from timeout to get delta in
                     * in native channel resolution.
                     * We compute the delta in the native resolution to correctly handle wrapping and underflow at the
                     * 32-bit boundary.
                     * To simplify code paths and SRAM, we shift up the 1us resolution time stamp instead of reading out
                     * and keeping track of the 250ns time stamp and associating that with channels 2 to 4. The loss
                     * of resolution for wakeup is not material as we wake up sufficiently early to handle timing jitter
                     * in the wakeup duration.
                     */
                    sysTimerLoopDelta -= sysTimerCurrTime << sysTimerResolutionShift[sysTimerIndex];

                    /* If sysTimerDelta is larger than MAX_SYSTIMER_DELTA, the compare
                     * event happened in the past and we need to abort entering standby to
                     * handle the timeout instead of waiting a really long time.
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
        osDelta = PowerCC23X0_idleTimeOS * FREERTOS_TICKPERIOD_US;

        /* Get soonestDelta wake time and corresponding ClockP timeout */
        soonestDelta = Math_MIN(sysTimerDelta, osDelta);

        /* Check soonestDelta time vs STANDBY latency */
        if (soonestDelta > PowerCC23X0_TOTALTIMESTANDBY)
        {
            /* Disable scheduling */
            PowerCC23X0_schedulerDisable();

            /* Stop FreeRTOS ticks */
            SysTickDisable();

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

            /* Do not use SysTickSetPeriod() since that does not allow 0 as an
             * input, which we might get as a return value from
             * SysTickGetValue().
             */
            HWREG(SYSTICK_BASE + SYSTICK_O_RVR) = sysTickDelta;

            /* Switch EVTSVT_O_CPUIRQ16SEL in eventfabric to RTC
             * Since the CC23X0 only has limited interrupt lines, we need
             * to switch the interrupt line from SysTimer to RTC in the
             * event fabric.
             * The triggered interrupt will wake up the device with
             * interrupts disabled. We can consume that interrupt event
             * without vectoring to the ISR and then change the event fabric
             * signal back to the SysTimer.
             * Thus, there is no need to swap out the actual interrupt
             * function of the clockHwi.
             */
            HWREG(EVTSVT_BASE + EVTSVT_O_CPUIRQ16SEL) = EVTSVT_CPUIRQ16SEL_PUBID_AON_RTC_COMB;

            /* Clear interrupt in case it triggered since we disabled interrupts */
            HwiP_clearInterrupt(INT_CPUIRQ16);

            /* Ensure the device wakes up early enough to reinitialise the
             * HW and take care of housekeeping.
             */
            soonestDelta -= PowerCC23X0_WAKEDELAYSTANDBY;

            /* Divide delta until next timeout by two to sync with real time
             * since the FPGA is running at half clock speed
             */
            soonestDelta /= CLOCK_FREQUENCY_DIVIDER;
            soonestDelta /= RTC_TO_SYSTIMER_TICKS;

            /* Save RTC tick count before sleep */
            ticksBefore = HWREG(RTC_BASE + RTC_O_TIME8U);

            /* RTC channel 0 compare is automatically armed upon writing the
             * compare value. It will automatically be disarmed when it
             * triggers.
             * The SysTimer has a time base of 1us while the RTC uses 8us.
             * Divide by 8 to convert from SysTimer to RTC time base
             */
            HWREG(RTC_BASE + RTC_O_CH0CC8U) = ticksBefore + soonestDelta;

            /* Go to standby mode */
            Power_sleep(PowerLPF3_STANDBY);

            /* Clear the wakeup event */
            HWREG(RTC_BASE + RTC_O_ICLR) = RTC_ICLR_EV0_CLR;
            HwiP_clearInterrupt(INT_CPUIRQ16);

            /* Switch EVTSVT_O_CPUIRQ16SEL in eventfabric back to SysTimer */
            HWREG(EVTSVT_BASE + EVTSVT_O_CPUIRQ16SEL) = EVTSVT_CPUIRQ16SEL_PUBID_SYSTIM0;

            /* If waking up from an async event (not RTC), the SysTimer may not
             * have synchronised with the RTC by now and will read out 0. If we
             * wait for the register to take on a non-zero value, we know the
             * SysTimer time and any generated events from code below are valid.
             *
             * 0 is a legal value so we will need to wait 1us for that 1 in 2^32
             * -1 case that we woke up just after rollover.
             */
            while (HWREG(SYSTIM_BASE + SYSTIM_O_TIME1U) == 0) {}

            /* Restore SysTimer timeouts since standby wiped them */
            for (sysTimerIndex = 0; sysTimerIndex < SYSTIMER_CHANNEL_COUNT; sysTimerIndex++)
            {
                if (sysTimerIMASK & (1 << sysTimerIndex))
                {
                    HWREG(SYSTIM_BASE + SYSTIM_O_CH0CC + sysTimerIndex * sizeof(uint32_t)) = sysTimerTimeouts[sysTimerIndex];
                }
            }

            /* Restore SysTimer IMASK */
            HWREG(SYSTIM_BASE + SYSTIM_O_IMASK) = sysTimerIMASK;

            /* Re-configure LRFD clocks */
            LRFDApplyClockDependencies();

            /* Signal clients registered for standby wakeup notification;
             * this should be used to initialize any timing critical or IO
             * dependent hardware.
             * The callback needs to go out after the SysTimer is restored
             * such that notifications can invoke RCL and ClockP APIs if needed.
             */
            PowerCC23X0_notify(PowerLPF3_AWAKE_STANDBY);

            /* Get RTC tick count after sleep */
            ticksAfter = HWREG(RTC_BASE + RTC_O_TIME8U);

            /* Calculate elapsed FreeRTOS tick periods in STANDBY */
            sleptTicks = (ticksAfter - ticksBefore) * CLOCK_FREQUENCY_DIVIDER * RTC_TO_SYSTIMER_TICKS;

    #if (FREERTOS_TICKPERIOD_US == 1000)
            /* Use a dedicated divide by 1000 function to run in 16 cycles
             * instead of 95. This shaves about 1.6us off in real-time.
             */
            sleptTicks = Math_divideBy1000(sleptTicks);
    #else
            /* If we are not using the default tick rate, do a slow
             * divide
             */
            sleptTicks = sleptTicks / FREERTOS_TICKPERIOD_US;
    #endif

            /* Update FreeRTOS tick count for time spent in STANDBY */
            vTaskStepTick(sleptTicks);

            /* Reset SysTick counter to 0 so it reloads with the remaining
             * timeout set above.
             */
            HWREG(SYSTICK_BASE + SYSTICK_O_CVR) = 0;

            /* Restart FreeRTOS ticks */
            SysTickEnable();

            /* Set the SysTick period to the previous value for next timeout.
             * Interrupts must still be disabled in between re-enabling the
             * SysTick to force a reload of the counter and changing the period
             * back to the FreeRTOS tick period.
             * Otherwise, a small leftover counter value could cause the reload
             * value to be low enough to inifinitely trigger the ISR since we
             * do not reach the line below before timing out again.
             * If the timeout was small, it will time out and pend the
             * interrupt but not vector to it. Until after the new reload value
             * is applied.
             * We will only vector to the ISR once regardless of whether the
             * counter times out more than once.
             */
            SysTickSetPeriod(configCPU_CLOCK_HZ / configTICK_RATE_HZ);

            /* Re-enable scheduling. Also re-enables interrupts.
             * This must happen only after restoring the SysTimer because
             * otherwise a Clock might be started in an ISR and then the Power
             * policy clobbers the hardware state.
             * This must also be called only after clearing the RTC event.
             * Otherwise, when interrupts are enabled in this fxn, the CPU
             * will immediately vector to CPUIRQ16, which still has the ClockP
             * ISR registered.
             */
            PowerCC23X0_schedulerRestore();
        }
        else if (idleAllowed)
        {
            /* If we would be allowed to enter standby but there is not enough
             * time for it to make sense from an overhead perspective, enter idle
             * instead.
             */
            __WFI();
        }
    }
    else if (idleAllowed)
    {
        /* We are not allowed to enter standby.
         * Enter idle instead if it is allowed.
         */
        __WFI();
    }

    HwiP_restore(key);
#endif
}

/*
 *  ======== PowerCC23X0_schedulerDisable ========
 */
void PowerCC23X0_schedulerDisable()
{
    vTaskSuspendAll();
}

/*
 *  ======== PowerCC23X0_schedulerRestore ========
 */
void PowerCC23X0_schedulerRestore()
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
    PowerCC23X0_idleTimeOS = xExpectedIdleTime;

    /*
     * call Power-driver-specified idle function, to conditionally invoke the
     * Power policy
     */
    Power_idleFunc();
}
