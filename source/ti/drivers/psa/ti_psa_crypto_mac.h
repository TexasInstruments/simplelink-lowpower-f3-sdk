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

#ifndef TI_PSA_CRYPTO_MAC_H
#define TI_PSA_CRYPTO_MAC_H

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
    #include <ti/drivers/sha2/SHA2XXF3HSM.h>
    #include <ti/drivers/aescmac/AESCMACXXF3.h>
#elif ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
       (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
    #include <ti/drivers/sha2/SHA2CC26X2.h>
    #include <ti/drivers/aescmac/AESCMACCC26XX.h>
#endif

struct ti_psa_mac_operation_s
{
    union
    {
        struct
        {
            SHA2_Config sha2Config;
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX) || \
     (DeviceFamily_PARENT == DeviceFamily_PARENT_CC23X1))
            SHA2XXF3HSM_Object sha2Object;
#elif ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
       (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
            SHA2CC26X2_Object sha2Object;
#endif
        } sha2;
        struct
        {
            AESCMAC_Config aescmacConfig;
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX) || \
     (DeviceFamily_PARENT == DeviceFamily_PARENT_CC23X1))
            AESCMACXXF3_Object aescmacObject;
#elif ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
       (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
            AESCMACCC26XX_Object aescmacObject;
#endif
        } aescmac;
        struct
        {
            AESCMAC_Config aescbcmacConfig;
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX) || \
     (DeviceFamily_PARENT == DeviceFamily_PARENT_CC23X1))
            AESCMACXXF3_Object aescbcmacObject;
#elif ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
       (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
            AESCMACCC26XX_Object aescbcmacObject;
#endif
        } aescbcmac;
    } driver;
    /** Unique ID indicating which driver got assigned to do the
     * operation. Since driver contexts are driver-specific, swapping
     * drivers halfway through the operation is not supported.
     * ID values are auto-generated in psa_driver_wrappers.h
     * ID value zero means the context is not valid or not assigned to
     * any driver (i.e. none of the driver contexts are active). */
    unsigned int id;
    size_t mac_size;
    psa_algorithm_t alg;
    size_t unprocessed_len;

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

    CryptoKey cryptoKey;
    bool is_sign;
};

typedef struct ti_psa_mac_operation_s ti_psa_mac_operation_t;

#define TI_PSA_MAC_OPERATION_INIT \
    (ti_psa_mac_operation_t)      \
    {                             \
        0                         \
    }

static inline struct ti_psa_mac_operation_s ti_psa_mac_operation_init(void)
{
    const struct ti_psa_mac_operation_s v = TI_PSA_MAC_OPERATION_INIT;
    return (v);
}

/******************************************************************************/
/* Multi-Step Message Authentication Code (MAC) */
/******************************************************************************/
psa_status_t ti_psa_mac_abort(psa_mac_operation_t *psa_operation);

psa_status_t ti_psa_mac_sign_finish(psa_mac_operation_t *psa_operation,
                                    uint8_t *mac,
                                    size_t mac_size,
                                    size_t *mac_length);

psa_status_t ti_psa_mac_sign_setup(psa_mac_operation_t *psa_operation, mbedtls_svc_key_id_t key, psa_algorithm_t alg);

psa_status_t ti_psa_mac_update(psa_mac_operation_t *psa_operation, const uint8_t *input, size_t input_length);

psa_status_t ti_psa_mac_verify_finish(psa_mac_operation_t *psa_operation, const uint8_t *mac, size_t mac_length);

psa_status_t ti_psa_mac_verify_setup(psa_mac_operation_t *psa_operation, mbedtls_svc_key_id_t key, psa_algorithm_t alg);

/******************************************************************************/
/* One-Step Message Authentication Code (MAC) */
/******************************************************************************/
psa_status_t ti_psa_mac_compute(mbedtls_svc_key_id_t key,
                                psa_algorithm_t alg,
                                const uint8_t *input,
                                size_t input_length,
                                uint8_t *mac,
                                size_t mac_size,
                                size_t *mac_length);

psa_status_t ti_psa_mac_verify(mbedtls_svc_key_id_t key,
                               psa_algorithm_t alg,
                               const uint8_t *input,
                               size_t input_length,
                               const uint8_t *mac,
                               size_t mac_length);

#endif /* TI_PSA_CRYPTO_MAC_H */