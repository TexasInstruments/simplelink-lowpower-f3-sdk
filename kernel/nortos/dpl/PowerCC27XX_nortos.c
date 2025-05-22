/*
 * Copyright (c) 2023-2025, Texas Instruments Incorporated
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
 *  ======== PowerCC27XX_nortos.c ========
 */

#include <stdbool.h>

/* Driver header files */
#include <ti/drivers/ITM.h>
#include <ti/drivers/Power.h>

/* Utilities header files */
#include <ti/drivers/utils/Math.h>

/* DPL header files */
#include <ti/drivers/dpl/HwiP.h>

/* Driverlib header files */
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_systim.h)
#include DeviceFamily_constructPath(cmsis/core/cmsis_compiler.h)
#include DeviceFamily_constructPath(driverlib/systick.h)

/* Max number of ClockP ticks into the future supported by this ClockP
 * implementation.
 * Under the hood, ClockP uses the SysTimer whose events trigger immediately if
 * the compare value is less than 2^22 systimer ticks in the past
 * (4.194sec at 1us resolution). Therefore, the max number of SysTimer ticks you
 * can schedule into the future is 2^32 - 2^22 - 1 ticks (~= 4290 sec at 1us
 * resolution).
 */
#define MAX_SYSTIMER_DELTA (0xFFBFFFFFU)

#define SYSTIMER_CHANNEL_COUNT (5U)

/* Shift values to convert between the different resolutions of the SysTimer
 * channels. Channel 0 can technically support either 1us or 250ns. Until the
 * channel is actively used, we will hard-code it to 1us resolution to improve
 * runtime.
 */
static const uint8_t sysTimerResolutionShift[SYSTIMER_CHANNEL_COUNT] = {
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
    uint32_t constraints;
    uint32_t sysTimerDelta;
    uint32_t sysTimerIMASK;
    uint32_t sysTimerLoopDelta;
    uint32_t sysTimerCurrTime;
    uint8_t sysTimerIndex;
    uintptr_t key;
    bool standbyAllowed;
    bool idleAllowed;
    bool sysTickEnabled;

    key = HwiP_disable();

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
    }

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

        /* Check sysTimerDelta time vs STANDBY latency */
        if (sysTimerDelta > PowerCC27XX_TOTALTIMESTANDBY)
        {
            /* Store SysTick enabled state */
            sysTickEnabled = ((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) != 0);

            /* Flush any remaining log messages in the ITM */
            ITM_flush();

            /* Go to standby mode */
            PowerLPF3_sleep(sysTimerDelta + sysTimerCurrTime);

            /* Restore ITM settings */
            ITM_restore();

            /* If PowerLPF3_sleep() disabled SysTick, it must be re-enabled if
             * it was enabled before calling PowerLPF3_sleep().
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

            PowerCC27XX_doWFI();
        }
    }
    else if (idleAllowed)
    {
        /* We are not allowed to enter standby.
         * Enter idle instead if it is allowed.
         */

        PowerCC27XX_doWFI();
    }

    HwiP_restore(key);
}
