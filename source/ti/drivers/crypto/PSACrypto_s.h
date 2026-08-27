/*
 * Copyright (c) 2025, Texas Instruments Incorporated - http://www.ti.com
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

#ifndef ti_drivers_crypto_PSACrypto_s__include
#define ti_drivers_crypto_PSACrypto_s__include

#include <ti/drivers/tfm/SecureCallback.h>

#ifdef TFM_BUILD
    #include "tfm_mbedcrypto_include.h"                 /* Must be included before psa/crypto.h */
    #include <third_party/mbedtls/include/psa/crypto.h> /* For PSA_KEY_EXPORT_ECC_PUBLIC_KEY_MAX_SIZE() */
#else
    /* This handles the case where PSACrypto_s.h is included by PSACrypto_ns.c */
    #include <third_party/tfm/interface/include/psa/crypto.h>
#endif
#include <third_party/tfm/interface/include/psa/error.h>   /* For psa_status_t */
#include <third_party/tfm/interface/include/psa/service.h> /* For psa_msg_t */

#ifdef __cplusplus
extern "C" {
#endif

#define PSA_CRYPTO_REGISTER_SECURE_CALLBACK_TYPE 0x00005AF0 /* this value must fit in int16_t */

#define PSA_ECC_MAX_CURVE_BITS 521

#define PSA_OUTPUT_BUF_SIZE PSA_KEY_EXPORT_ECC_PUBLIC_KEY_MAX_SIZE(PSA_ECC_MAX_CURVE_BITS)

/* ========= Secure Callback struct =========
 * Non-secure client must register their callback during init.
 */
typedef struct
{
    SecureCallback_Object object;

    size_t length; /* The significance of the 'length' depends on the PSA function */
    size_t keyBits;
    volatile psa_status_t status;

    /* TF-M operation handle represents an 1-based index to a secure operation struct array */
    volatile uint32_t *tfmCryptoOperationHandle;

    /* The contents of this buffer depends on the PSA function. Word-aligned for
     * performance and in case the driver requires a word-aligned buffer.
     */
    uint8_t buffer[PSA_OUTPUT_BUF_SIZE] __attribute__((aligned(4)));
} PSACrypto_s_SecureCallback;

/*!
 *  @brief  Handles PSA messages for PSA Crypto
 *
 *  @note   This function should be called by secure partition thread only.
 *
 *  @param [in]  msg     pointer to PSA message
 *
 *  @retval PSA_SUCCESS if successful.
 *  @retval PSA_ERROR_PROGRAMMER_ERROR if any args are invalid.
 *  @retval PSA_ERROR_NOT_SUPPORTED if the crypto function is not supported/enabled.
 */
psa_status_t PSACrypto_s_handlePsaMsg(const psa_msg_t *msg);

/*!
 *  @brief  Initializes the PSA Crypto secure service
 *
 *  @note   This function should be called by secure partition thread only.
 *
 *  @retval PSA_SUCCESS if successful.
 *  @retval PSA_ERROR_GENERIC_ERROR if provisioning NV entropy seed fails (only if CRYPTO_NV_SEED set).
 *  @retval PSA_ERROR_HARDWARE_FAILURE if crypto accelerator initialization fails.
 */
psa_status_t PSACrypto_s_init(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_crypto_PSACrypto_s__include */
