/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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
 *  ======== PowerCC283X_nortos.c ========
 */

#include <stdbool.h>

/* Driver header files */
#include <ti/drivers/Power.h>

/* Utilities header files */
#include <ti/drivers/utils/Math.h>

/* DPL header files */
#include <ti/drivers/dpl/HwiP.h>

/* Driverlib header files */
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(cmsis/device.h)
#include DeviceFamily_constructPath(driverlib/systimer.h)
#include DeviceFamily_constructPath(driverlib/systick.h)

/*
 *  ======== PowerCC283X_standbyPolicy ========
 */
void PowerCC283X_standbyPolicy(void)
{
    uint32_t constraints;
    uint32_t sysTimerDelta;
    uint32_t sysTimerIMASK;
    uint32_t sysTimerLoopDelta;
    uint8_t sysTimerResolutionShift;
    uint32_t sysTimerCurrTime;
    uint8_t sysTimerIndex;
    uintptr_t key;
    bool standbyAllowed;
    bool idleAllowed;
    bool sysTickEnabled;

    uint32_t sysTimerChannelCount = SYSTIMER_CHANNEL_COUNT;

    key = HwiP_disable();

    /* Check state of constraints */
    constraints    = Power_getConstraintMask();
    standbyAllowed = (constraints & (1U << PowerLPF4_DISALLOW_STANDBY)) == 0U;
    idleAllowed    = (constraints & (1U << PowerLPF4_DISALLOW_IDLE)) == 0U;

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

        /* Check sysTimerDelta time vs STANDBY latency */
        if (sysTimerDelta > PowerCC283X_TOTALTIMESTANDBY)
        {
            /* Store SysTick enabled state */
            sysTickEnabled = ((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) != 0);

            /* Go to standby mode */
            PowerLPF4_sleep(sysTimerDelta + sysTimerCurrTime);

            /* If PowerLPF4_sleep() disabled SysTick, it must be re-enabled if
             * it was enabled before calling PowerLPF4_sleep().
             */
            if (((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) == 0) && sysTickEnabled)
            {
                SysTickEnable();
            }
        }
        else if (idleAllowed)
        {
            /* If we would be allowed to enter standby but there is not enough
             * time for it to make sense from an overhead perspective, enter
             * idle instead.
             */

            PowerCC283X_doWFI();
        }
    }
    else if (idleAllowed)
    {
        /* We are not allowed to enter standby.
         * Enter idle instead if it is allowed.
         */

        PowerCC283X_doWFI();
    }

    HwiP_restore(key);
}
