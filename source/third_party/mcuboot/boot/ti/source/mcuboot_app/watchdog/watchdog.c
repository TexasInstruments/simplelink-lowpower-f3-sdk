/*
 * Copyright (c) 2026, Texas Instruments Incorporated
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

#include "mcuboot_config.h"
#include "watchdog.h"

#ifdef MCUBOOT_SWAP_USING_MOVE

#include <stdbool.h>
#include <ti/devices/DeviceFamily.h>

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC23X0) || \
    (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)

#include DeviceFamily_constructPath(driverlib/ckmd.h)
#include "mcuboot_config/mcuboot_logging.h"

#ifndef MCUBOOT_HW_WATCHDOG_TIMEOUT_MS
  #error "MCUBOOT_HW_WATCHDOG_TIMEOUT_MS must be defined in mcuboot_config.h when MCUBOOT_SWAP_USING_MOVE is used on this device"
#endif

#define SCLK_LF_FREQ_HZ  32768U  /* LFCLK frequency in Hz  */
#define MS_PER_SEC       1000U   /* milliseconds per second */

/* WDT_MS_TO_TICKS: convert a millisecond timeout to hardware counter ticks. */
#define WDT_MS_TO_TICKS(ms) \
    ((uint32_t)(((ms) / MS_PER_SEC) * SCLK_LF_FREQ_HZ + \
                (((ms) % MS_PER_SEC) * SCLK_LF_FREQ_HZ) / MS_PER_SEC))

/* Module state */
static bool     wdt_active = false;
static uint32_t wdt_ticks  = 0U;

void mcuboot_watchdog_init(void)
{
    wdt_ticks = WDT_MS_TO_TICKS(MCUBOOT_HW_WATCHDOG_TIMEOUT_MS);
    CKMDUnlockWatchdog();
    CKMDSetWatchdogCounter(wdt_ticks);
    CKMDLockWatchdog();

    wdt_active = true;
    MCUBOOT_LOG_INF("HW watchdog started (%u ms)", (unsigned int)MCUBOOT_HW_WATCHDOG_TIMEOUT_MS);
}

void mcuboot_watchdog_feed(void)
{
    if (wdt_active)
    {
        CKMDUnlockWatchdog();
        CKMDSetWatchdogCounter(wdt_ticks);
        CKMDLockWatchdog();

    }
}

#else /* no-op stubs for unsupported device families */

void mcuboot_watchdog_init(void)    { }
void mcuboot_watchdog_feed(void)    { }

#endif /* DeviceFamily check */

#endif /* MCUBOOT_SWAP_USING_MOVE */
