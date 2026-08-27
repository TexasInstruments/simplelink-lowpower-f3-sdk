/*
 *  Copyright The Mbed TLS Contributors
 *  Copyright (c) 2025-2026 Texas Instruments Incorporated
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  Modified by Texas Instruments to support SimpleLink device crypto hardware
 *  drivers.
 */

#ifndef TI_PSA_CRYPTO_AEAD_H
#define TI_PSA_CRYPTO_AEAD_H

#include <stddef.h>
#include <stdint.h>

#if ((TFM_ENABLED == 1) && !defined(TFM_BUILD))
    #include <third_party/tfm/interface/include/psa/crypto.h>
#else
    #include <third_party/mbedtls/include/psa/crypto.h>
#endif
#include <ti/drivers/cryptoutils/cryptokey/CryptoKey.h>
#include <ti/devices/DeviceFamily.h>
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX) || \
     (DeviceFamily_PARENT == DeviceFamily_PARENT_CC23X1))
    #include <ti/drivers/aesccm/AESCCMXXF3.h>
    #include <ti/drivers/aesgcm/AESGCMXXF3HSM.h>
#elif ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
       (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
    #include <ti/drivers/aesccm/AESCCMCC26XX.h>
    #include <ti/drivers/aesgcm/AESGCMCC26XX.h>
#endif

struct ti_psa_aead_operation_s
{
    union
    {
        struct
        {
            AESCCM_Config aesccmConfig;
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX) || \
     (DeviceFamily_PARENT == DeviceFamily_PARENT_CC23X1))
            AESCCMXXF3_Object aesccmObject;
#elif ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
       (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
            AESCCMCC26XX_Object aesccmObject;
#endif
        } aesccm;
        struct
        {
            AESGCM_Config aesgcmConfig;
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX) || \
     (DeviceFamily_PARENT == DeviceFamily_PARENT_CC23X1))
            AESGCMXXF3HSM_Object aesgcmObject;
#elif ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
       (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
            AESGCMCC26XX_Object aesgcmObject;
#endif
        } aesgcm;
    } driver;
    /* Used to mark the operation struct as ready.*/
    unsigned int id;
    psa_algorithm_t alg;
    unsigned int iv_set:1;
    unsigned int in_error_state:1;
    size_t adLength;
    size_t plaintextLength;
    size_t runningADLength;
    size_t runningPlaintextLength;
    size_t tagSize;

    /* Buffer for data that has not been processed yet. Word-aligned for max
     * performance and in case any drivers require aligned input buffer.
     * Double-buffer is used when PSA crypto is built into the TFM to prevent
     * corruption before the data can be consumed since the underlying crypto
     * drivers are used in callback mode. */
#ifdef TFM_BUILD
    uint8_t unprocessedData[PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE * 2] __attribute__((aligned(4)));
#else
    uint8_t unprocessedData[PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE] __attribute__((aligned(4)));
#endif /* TFM_BUILD */

    /* Pointer to the current unprocessed data */
    uint8_t *curr_unprocessed_data;

    size_t unprocessed_len;

    CryptoKey cryptoKey;
    bool done_updating_ad;
    bool length_set;
};

typedef struct ti_psa_aead_operation_s ti_psa_aead_operation_t;

#define TI_PSA_AEAD_OPERATION_INIT \
    (ti_psa_aead_operation_t)      \
    {                              \
        0                          \
    }
static inline struct ti_psa_aead_operation_s ti_psa_aead_operation_init(void)
{
    const struct ti_psa_aead_operation_s v = TI_PSA_AEAD_OPERATION_INIT;
    return (v);
}

/******************************************************************************/
/* Multi-Step AEAD */
/******************************************************************************/
psa_status_t ti_psa_aead_abort(psa_aead_operation_t *psa_operation);

psa_status_t ti_psa_aead_encrypt_setup(psa_aead_operation_t *psa_operation,
                                       mbedtls_svc_key_id_t key,
                                       psa_algorithm_t alg);

psa_status_t ti_psa_aead_decrypt_setup(psa_aead_operation_t *psa_operation,
                                       mbedtls_svc_key_id_t key,
                                       psa_algorithm_t alg);

psa_status_t ti_psa_aead_finish(psa_aead_operation_t *psa_operation,
                                uint8_t *ciphertext,
                                size_t ciphertext_size,
                                size_t *ciphertext_length,
                                uint8_t *tag,
                                size_t tag_size,
                                size_t *tag_length);

psa_status_t ti_psa_aead_update(psa_aead_operation_t *psa_operation,
                                const uint8_t *input,
                                size_t input_length,
                                uint8_t *output,
                                size_t output_size,
                                size_t *output_length);

psa_status_t ti_psa_aead_update_ad(psa_aead_operation_t *psa_operation, const uint8_t *input, size_t input_length);

/******************************************************************************/
/* One-Step AEAD */
/******************************************************************************/
psa_status_t ti_psa_aead_encrypt(mbedtls_svc_key_id_t key,
                                 psa_algorithm_t alg,
                                 const uint8_t *nonce,
                                 size_t nonce_length,
                                 const uint8_t *additional_data,
                                 size_t additional_data_length,
                                 const uint8_t *plaintext,
                                 size_t plaintext_length,
                                 uint8_t *ciphertext,
                                 size_t ciphertext_size,
                                 size_t *ciphertext_length);

psa_status_t ti_psa_aead_decrypt(mbedtls_svc_key_id_t key,
                                 psa_algorithm_t alg,
                                 const uint8_t *nonce,
                                 size_t nonce_length,
                                 const uint8_t *additional_data,
                                 size_t additional_data_length,
                                 const uint8_t *ciphertext,
                                 size_t ciphertext_length,
                                 uint8_t *plaintext,
                                 size_t plaintext_size,
                                 size_t *plaintext_length);

psa_status_t ti_psa_aead_generate_nonce(psa_aead_operation_t *psa_operation,
                                        uint8_t *nonce,
                                        size_t nonce_size,
                                        size_t *nonce_length);

psa_status_t ti_psa_aead_set_lengths(psa_aead_operation_t *psa_operation, size_t ad_length, size_t plaintext_length);

psa_status_t ti_psa_aead_set_nonce(psa_aead_operation_t *psa_operation, const uint8_t *nonce, size_t nonce_length);

psa_status_t ti_psa_aead_verify(psa_aead_operation_t *psa_operation,
                                uint8_t *plaintext,
                                size_t plaintext_size,
                                size_t *plaintext_length,
                                const uint8_t *tag,
                                size_t tag_length);

#endif /* TI_PSA_CRYPTO_AEAD_H */