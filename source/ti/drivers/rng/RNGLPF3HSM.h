/*
 * Copyright (c) 2023-2024, Texas Instruments Incorporated
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
/** ============================================================================
 *  @file       RNGLPF3HSM.h
 *
 *  @brief      RNG driver implementation for the LPF3 family
 *
 *  This file should only be included in the board file to fill the RNG_config
 *  structure.
 *
 *  This is a temporary RNG implementation that uses a constant seed to derive
 *  random numbers. This implementation will be updated to HSM's TRNG to
 *  seed the RNG driver.
 *
 *  @note This implementation does not support the RNG_RETURN_BEHAVIOR_CALLBACK
 *        return mode.
 *
 *  @note This implementation treats RNG_RETURN_BEHAVIOR_POLLING as
 *        RNG_RETURN_BEHAVIOR_BLOCKING.
 */

#ifndef ti_drivers_rng_RNGLPF3HSM__include
#define ti_drivers_rng_RNGLPF3HSM__include

#include <stdint.h>
#include <stdbool.h>

#include <ti/drivers/RNG.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \cond Internal APIs */

/*!
 * @brief   Invalid Input Key encoding
 *
 *  User needs to either use CryptoKey_BLANK_PLAINTEXT_HSM or CryptoKey_BLANK_PLAINTEXT
 */
#define RNG_STATUS_INVALID_KEY_ENCODING ((int_fast16_t)(RNG_STATUS_RESERVED - 0))

/*!
 *  @brief RNGLPF3HSM Object
 *
 *  \note The application must not access any member variables of this structure!
 */
typedef struct
{
    bool isOpen;
    uint32_t timeout;
} RNGLPF3HSM_Object;

typedef struct
{
    uint8_t intPriority;
} RNGLPF3HSM_HWAttrs;

/*! \endcond */

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_rng_RNGLPF3HSM__include */
