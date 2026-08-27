/*
 * Copyright (c) 2025-2026, Texas Instruments Incorporated
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
 *  ======== PowerCC283X_freertos.c ========
 */

#include <stdbool.h>

/* Driver header files */
#include <ti/drivers/Power.h>

#include "OSTickLPF4.h"

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
#include DeviceFamily_constructPath(cmsis/core/cmsis_compiler.h)
#include DeviceFamily_constructPath(driverlib/systimer.h)

/* Number of microseconds between each FreeRTOS OS tick */
#define FREERTOS_TICKPERIOD_US (1000000 / configTICK_RATE_HZ)

#if ((FREERTOS_TICKPERIOD_US * configTICK_RATE_HZ) != 1000000)
    #error configTICK_RATE_HZ must divide 1000000 evenly
#endif

/* This global is used to pass the expected ticks that the OS will sleep from
 * vPortSuppressTicksAndSleep() to the Power policy.
 */
static volatile uint32_t PowerCC283X_idleTimeOS = 0U;

/*
 *  ======== PowerCC283X_standbyPolicy ========
 */
void PowerCC283X_standbyPolicy(void)
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
    uint8_t sysTimerResolutionShift;
    uint32_t sysTimerCurrTime;
    uint8_t sysTimerIndex;
    uint32_t nextFreeRtosTick;
    uintptr_t key;
    bool standbyAllowed;
    bool idleAllowed;

    uint32_t sysTimerChannelCount = SYSTIMER_CHANNEL_COUNT;

    key = __get_PRIMASK();
    __set_PRIMASK(1);

    /* Final check with FreeRTOS to make sure it is still OK to go to sleep. */
    eSleep = eTaskConfirmSleepModeStatus();
    if (eSleep == eAbortSleep)
    {
        __set_PRIMASK(key);
        return;
    }

    /* Check state of constraints */
    constraints    = Power_getConstraintMask();
    standbyAllowed = (constraints & (1U << PowerLPF4_DISALLOW_STANDBY)) == 0U;
    idleAllowed    = (constraints & (1U << PowerLPF4_DISALLOW_IDLE)) == 0U;

    Log_printf(LogModule_Power,
               Log_VERBOSE,
               "PowerCC283X_standbyPolicy: Standby constraint count = %d. Idle constraint count = %d",
               Power_getConstraintCount(PowerLPF4_DISALLOW_STANDBY),
               Power_getConstraintCount(PowerLPF4_DISALLOW_IDLE));

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
            for (sysTimerIndex = 0; sysTimerIndex < sysTimerChannelCount; sysTimerIndex++)
            {
                if (sysTimerIndex == OSTickLPF4_TICK_SYSTIM_CHANNEL)
                {
                    /* This SysTimer channel is used for OS ticks and should not
                     * be considered.
                     */
                    continue;
                }

                if (sysTimerIMASK & (1 << sysTimerIndex))
                {
                    /* Store current channel timeout in native channel
                     * resolution.
                     */
                    sysTimerLoopDelta = SysTimerGetCaptureCompareValue(sysTimerIndex);

                    /* Get the resolution of the channel and determine the shift
                     * necessary to convert to/from 1us resolution.
                     */
                    switch (SysTimerGetChannelResolution(sysTimerIndex))
                    {
                        case SYSTIMER_CONFIG_RESOLUTION_250NS:
                            sysTimerResolutionShift = 2;
                            break;
                        case SYSTIMER_CONFIG_RESOLUTION_1US: /* Intentional fall-through to the default case. */
                        default:
                            sysTimerResolutionShift = 0;
                            break;
                    }

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
                    sysTimerLoopDelta -= sysTimerCurrTime << sysTimerResolutionShift;

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
                    sysTimerLoopDelta = sysTimerLoopDelta >> sysTimerResolutionShift;

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

        /* Check soonestDelta wake time pending for FreeRTOS in 1us resolution.
         * Cap the value at SYSTIMER_MAX_DELTA.
         */
        osDelta = (uint64_t)PowerCC283X_idleTimeOS * FREERTOS_TICKPERIOD_US;
        if (osDelta > SYSTIMER_MAX_DELTA)
        {
            osDelta = SYSTIMER_MAX_DELTA;
        }

        /* Get soonestDelta wake time and corresponding ClockP timeout.
         * Casting osDelta to uint32_t is safe since the value is capped
         * at SYSTIMER_MAX_DELTA which fits in uint32_t.
         */
        soonestDelta = Math_MIN(sysTimerDelta, (uint32_t)osDelta);

        /* Check soonestDelta time vs STANDBY latency */
        if (soonestDelta > PowerCC283X_TOTALTIMESTANDBY)
        {
            Log_printf(LogModule_Power,
                       Log_INFO,
                       "PowerCC283X_standbyPolicy: Entering standby. Soonest timeout = 0x%x",
                       (soonestDelta + sysTimerCurrTime));

            /* Disable scheduling */
            PowerCC283X_schedulerDisable();

            /* Save SysTimer tick count before sleep */
            ticksBefore = sysTimerCurrTime;

            /* Save the next FreeRTOS Tick time before sleep. */
            nextFreeRtosTick = SysTimerGetCaptureCompareValue(OSTickLPF4_TICK_SYSTIM_CHANNEL);

            PowerLPF4_sleep(soonestDelta + sysTimerCurrTime);

            /* Get SysTimer tick count after sleep */
            ticksAfter = SysTimerGetTime1Us();

            /* Compute the number of FreeRTOS ticks that have been suppressed
             * during standby.
             */
            sleptTicks = (ticksAfter - nextFreeRtosTick) / FREERTOS_TICKPERIOD_US;

            /* Determine if we woke up before the next FreeRTOS tick. */
            if ((nextFreeRtosTick - ticksBefore) < SYSTIMER_MAX_DELTA)
            {
                if ((nextFreeRtosTick - ticksBefore) > (ticksAfter - ticksBefore))
                {
                    /* We woke up before the next FreeRTOS tick, so no ticks
                     * have been suppressed.
                     */
                    sleptTicks = 0;
                }
            }

            /* Increment the next FreeRTOS tick time.
             * This multiplication can't overflow because sleptTicks is the
             * result of dividing a 32 bit integer by FREERTOS_TICKPERIOD_US
             * above or 0.
             */
            nextFreeRtosTick += sleptTicks * FREERTOS_TICKPERIOD_US;

            /* Update FreeRTOS tick count for time spent in STANDBY.
             * Note: FreeRTOS expects that the value passed to vTaskStepTick()
             * is at most PowerCC283X_idleTimeOS. This is true since the wakeup
             * time is selected such that we wakeup before the scheduled tick.
             * For the same reason, it is safe to cast to TickType_t here since
             * sleptTicks is at most PowerCC283X_idleTimeOS, which is at most
             * the maximum value of TickType_t.
             */
            vTaskStepTick((TickType_t)sleptTicks);

            /* Set the new FreeRTOS tick compare value. */
            SysTimerSetCompareValue(OSTickLPF4_TICK_SYSTIM_CHANNEL, nextFreeRtosTick);

            /* Re-enable scheduling. Also re-enables interrupts.
             * This must happen only after restoring the SysTimer because
             * otherwise a Clock might be started in an ISR and then the Power
             * policy clobbers the hardware state.
             */
            PowerCC283X_schedulerRestore();

            Log_printf(LogModule_Power,
                       Log_INFO,
                       "PowerCC283X_standbyPolicy: Exiting standby. Time after = 0x%x",
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
                   "PowerCC283X_standbyPolicy: Only WFI allowed");

            PowerCC283X_doWFI();
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

        PowerCC283X_doWFI();
    }

    __set_PRIMASK(key);
#endif
}

/*
 *  ======== PowerCC283X_schedulerDisable ========
 */
void PowerCC283X_schedulerDisable()
{
    vTaskSuspendAll();
}

/*
 *  ======== PowerCC283X_schedulerRestore ========
 */
void PowerCC283X_schedulerRestore()
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
    PowerCC283X_idleTimeOS = xExpectedIdleTime;

    /*
     * call Power-driver-specified idle function, to conditionally invoke the
     * Power policy
     */
    Power_idleFunc();
}