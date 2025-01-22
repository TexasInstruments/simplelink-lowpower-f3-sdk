/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
/*!****************************************************************************
 *  @file       TRNGLPF3HSM.h
 *
 *  @brief      TRNG driver implementation for the CC27XX family
 *
 *  This file should only be included in the board file to fill the TRNG_config
 *  struct.
 *
 *  # Hardware
 *
 *  The CC27XX family has a dedicated hardware engine, the Hardware Security Module (HSM)
 *  that contains TRNG capabilities based on sampling multiple free running oscillators
 *  (FROs).
 *
 *  # Behaviour
 *
 *  The driver submits a get random number token to the HSM. The token will indicate the
 *  destination buffer and the request size.
 *
 *  The TRNG driver can be used as a True Random Number Generator (TRNG) or as a Cryptographically
 *  Secure Pseudo Random Number Generator (CRNG). CRNG is a deterministic (hence the pseudo)
 *  computational method to turn an input (FRO samples) into a sequence of bits that are
 *  indistinguishable from ideal randomness, with cryptographic certainty, for a computationally
 *  bounded adversary not knowing the input.
 *
 *  By default, the HSM HW is configured and initialized to be in CRNG mode. To switch to TRNG,
 *  the user must call TRNGLPF3HSM_switchNrbgMode() API and specify TRNG_MODE_TRNG as the
 *  NRBG Mode.
 *
 *  # Samples Per Cycle Vs. Mix Cycles
 *
 *  # Mix Cycles
 *
 *  When the HSM EIP-130 HW in CRNG mode, the Mix Cycles value represents the number of clock
 *  cycles before the output is sampled. The MixCycles value must be in the range [0...15].
 *  However, in the current implementation the value 0 is interpreted as not configured and
 *  therefore will be set to 15 internally, which is also the default value.
 *
 *  # Samples Per Cycle
 *
 *  When the HSM EIP-130 HW is in TRNG mode, the Samples Per Cycle value represents the number of
 *  (XOR-ed) FRO samples XOR-ed together to generate a single 'noise' bit. This value must be
 *  set such that the total amount of entropy in 8 'noise' bits equals at least 1 bit. The value
 *  to program here depends upon the jitter behavior of the FROs.
 *
 *  !!!!!!!!!!!!! WARNING !!!!!!!!!!!!!
 *  The HSM HW engine is configured and initialized with the Samples Per Cycle value set to 19390
 *  and Mix Cycles value set to 15. This value is not available for the customer to configure.
 */

#ifndef ti_drivers_TRNG_TRNGLPF3HSM__include
#define ti_drivers_TRNG_TRNGLPF3HSM__include

#include <stdint.h>
#include <stdbool.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/TRNG.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKey.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *
 *  !!!!!!!!!!!!! WARNING !!!!!!!!!!!!!
 *  ## TRNG Driver Limitation for CC27XX devices #
 *
 *  For CC27XX devices, the size of random data requested must be a 32-bit multiple. The
 *  appropriate error code, TRNG_STATUS_INVALID_INPUT_SIZE, will be returned to the user in
 *  the case this rule is not adhered to and the input size is not a multiple of 4 bytes, (32-bits).
 *
 */

/*!
 * @brief   TRNG driver already in specified NRBG type
 *
 * TRNGLPF3HSM_switchNrbg() API will return TRNG_STATUS_ALREADY_IN_SPECIFIED_NRBG_TYPE if the user specifies an NRBG
 * mode and the HSM is in that mode already
 */
#define TRNG_STATUS_ALREADY_IN_SPECIFIED_NRBG_TYPE ((int_fast16_t)(TRNG_STATUS_RESERVED - 0))

/*!
 * @brief   Input size is not a multiple of a 32-bit block
 *
 * This is an known HSM limitation. Please provide a 32-bit (4 Bytes) aligned input size.
 */
#define TRNG_STATUS_INVALID_INPUT_SIZE ((int_fast16_t)(TRNG_STATUS_RESERVED - 1))

/*!
 * @brief   Invalid Input Key encoding
 *
 *  User needs to either use CryptoKey_BLANK_PLAINTEXT_HSM or CryptoKey_BLANK_PLAINTEXT
 */
#define TRNG_STATUS_INVALID_KEY_ENCODING ((int_fast16_t)(TRNG_STATUS_RESERVED - 2))

/*!
 *  @brief  Enum for the NRBG engine type
 */
typedef enum
{
    TRNG_MODE_CRNG = 1,
    TRNG_MODE_TRNG = 2,
} TRNGLFP3HSM_NRBGMode;

/*!
 *  @brief      TRNGLPF3HSM Hardware Attributes
 */
typedef struct
{
    /*!
     * @brief Crypto Peripheral's interrupt priority.
     *
     * The CC27xx uses four of the priority bits,
     * meaning ~0 has the same effect as INT_PRI_LEVEL15.
     *
     * (15 << 4) will apply the lowest priority.
     *
     * (1 << 5) will apply the highest priority.
     *
     * Setting the priority to 0 is not supported by this driver.
     *
     * HWI's with priority 0 ignore the HWI dispatcher to support zero-latency
     * interrupts, thus invalidating the critical sections in this driver.
     */
    uint8_t intPriority;
} TRNGLPF3HSM_HWAttrs;

/*! \cond Internal APIs */

/*!
 *  The application must not access any member variables of this structure!
 */
typedef struct
{
    TRNG_Handle handle;
    TRNG_CryptoKeyCallbackFxn cryptoKeyCallbackFxn;
    TRNG_RandomBytesCallbackFxn randomBytesCallbackFxn;
    CryptoKey *entropyKey;
    uint8_t *entropyBuffer;
    uint32_t semaphoreTimeout;
    size_t entropyGenerated;
    size_t entropyRequested;
    size_t rawEntropyBlocksRequested;
    int_fast16_t returnStatus;
    int_fast16_t hsmStatus;
    TRNG_ReturnBehavior returnBehavior;
    bool isOpen;
} TRNGLPF3HSM_Object;

/*! \endcond */

/*!
 *  @brief   Reseed the HSM IP DRBG engine
 *
 *  @attention This function makes a direct call to the HSM engine to force a reseed to the HSM IP.
 *
 *  Although the HSM IP auto-reseeds internally after a pre-defined level set in the OTP (256 * 64KB) entropy
 *  have been fetched, this API is provided to the user to call whenever they want to force a reseed.
 *
 *  This API can only be called in polling and blocking modes only.
 *
 *  @retval #TRNG_STATUS_SUCCESS              Reseeding was successful.
 *  @retval #TRNG_STATUS_ERROR                Reseeding was not successful.
 *  @retval #TRNG_STATUS_RESOURCE_UNAVAILABLE The required hardware was unavailable.
 */
extern int_fast16_t TRNGLPF3HSM_reseedHSM(TRNG_Handle handle);

/*!
 *  @brief   Reseed the HSM IP DRBG engine
 *
 *  @attention This function makes a direct call to the HSM engine to force a reseed to the HSM IP.
 *
 *  Although the HSM IP auto-reseeds internally after a pre-defined level set in the OTP (256 * 64KB) entropy
 *  have been fetched, this API is provided to the user to call whenever they want to force a reseed.
 *
 *  This operation is done asynchronously which means the call returns as soon as the request is deposited to the HSM
 *  engine and does not notify the user when the operation is complete.
 *
 *  Whilst the operation is running, the underlying HSM access semaphore is taken and no other operations or threads
 *  can leverage the HSM IP. The #XXXXX_STATUS_RESOURCE_UNAVAILABLE error code will be returned.
 *
 *  @retval #TRNG_STATUS_SUCCESS              Reseeding was successful.
 *  @retval #TRNG_STATUS_ERROR                Reseeding was not successful.
 *  @retval #TRNG_STATUS_RESOURCE_UNAVAILABLE The required hardware was unavailable.
 */
extern int_fast16_t TRNGLPF3HSM_reseedHSMAsync();

/*!
 *  @brief  Switches the TRNG driver NRBG Mode between CRNG-based or TRNG-based
 *
 *  This operation will always run in polling or blocking modes only.
 *
 *  @pre    TRNG_open() has to be called first successfully
 *
 *  @param  handle      A TRNGLPF3HSM handle returned from TRNGLPF3HSM_open
 *  @param  nrbgMode    Either #TRNG_MODE_CRNG or #TRNG_MODE_TRNG
 *
 *  @sa     TRNG_open()
 */
extern int_fast16_t TRNGLPF3HSM_switchNrbgMode(TRNG_Handle handle, TRNGLFP3HSM_NRBGMode nrbgMode);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_TRNG_TRNGLPF3HSM__include */
