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

#ifndef TI_MCUBOOT_WATCHDOG_H
#define TI_MCUBOOT_WATCHDOG_H

#include <stdint.h>
#include "mcuboot_config.h"

#ifdef MCUBOOT_SWAP_USING_MOVE

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize and start the hardware watchdog.
 *
 * Once started, the watchdog cannot be disabled.
 * Timeout period is configured via @c MCUBOOT_HW_WATCHDOG_TIMEOUT_MS.
 */
void mcuboot_watchdog_init(void);

/**
 * @brief Feed (kick) the watchdog timer.
 *
 * Resets the watchdog countdown. Must be called periodically within the
 * configured timeout period to prevent a watchdog reset. No-op if the
 * watchdog has not been started.
 */
void mcuboot_watchdog_feed(void);

#ifdef __cplusplus
}
#endif

#else /* !MCUBOOT_SWAP_USING_MOVE */

#define mcuboot_watchdog_init() do { } while (0)
#define mcuboot_watchdog_feed() do { } while (0)

#endif /* MCUBOOT_SWAP_USING_MOVE */

#endif /* TI_MCUBOOT_WATCHDOG_H */
