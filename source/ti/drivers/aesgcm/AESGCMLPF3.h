/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 *  @file       AESGCMLPF3.h
 *
 *  @brief      AESGCM driver implementation for the Low Power F3 family
 *
 *  This file should only be included in the board file to fill the AESGCM_config
 *  struct.
 *
 *  # Hardware Accelerator #
 *  The Low Power F3 family of devices has a dedicated AES hardware that can
 *  perform AES encryption operations with 128-bit keys. Only one operation can
 *  be carried out on the accelerator at a time. Mutual exclusion is implemented
 *  at the driver level and coordinated between all drivers relying on the
 *  accelerator. It is transparent to the application and only noted to ensure
 *  sensible access timeouts are set. The GHASH computation for GCM has
 *  optimizations using Shoup's 4-bit tables that are precomputed for each hash
 *  key H.
 *
 *  # Implementation Limitations
 *  - Only plaintext CryptoKeys are supported by this implementation.
 *  - Callback return behavior is not supported by this implementation.
 *  - Blocking return behavior operates the same as polling return behavior and
 *    does not block the calling thread.
 *
 *  # Runtime Parameter Validation #
 *  The driver implementation does not perform runtime checks for most input
 *  parameters. Only values that are likely to have a stochastic element to them
 *  are checked (such as whether a driver is already open). Higher input
 *  parameter validation coverage is achieved by turning on assertions when
 *  compiling the driver.
 */

#ifndef ti_drivers_aesgcm_AESGCMLPF3__include
#define ti_drivers_aesgcm_AESGCMLPF3__include

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ti/drivers/AESGCM.h>
#include <ti/drivers/cryptoutils/aes/AESCommonLPF3.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/aes.h)

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief      AESCCMLPF3 Hardware Attributes
 *
 *  AESCCMLPF3 hardware attributes should be included in the board file
 *  and pointed to by the AESCCM_config struct.
 */
typedef AESCommonLPF3_HWAttrs AESGCMLPF3_HWAttrs;

/*!
 *  @brief      AESGCMLPF3 Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct
{
    /* Common member first to allow struct to be cast to the common type */
    AESCommonLPF3_Object common;
    uint32_t hashKey[AES_BLOCK_SIZE_WORDS];
    uint32_t tagOTP[AES_BLOCK_SIZE_WORDS];
    uint32_t intermediateTag[AES_BLOCK_SIZE_WORDS];
    size_t inputLength;
    uint32_t counter[AES_BLOCK_SIZE_WORDS];
    const uint8_t *input;
    uint8_t *output;
    uint8_t *mac;
    uint8_t *aad;

    AESGCM_OperationUnion *operation;
    AESGCM_OperationType operationType;
} AESGCMLPF3_Object;

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_aesgcm_AESGCMLPF3__include */
