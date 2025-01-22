/*
 *  Copyright 2022-2024, Texas Instruments Incorporated
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
 */

#ifndef CRYPTO_HELPER_H
#define CRYPTO_HELPER_H

#include <stdbool.h>
#include <stdint.h>

#include <third_party/psa_crypto/include/psa/crypto.h>

#include <ti/drivers/cryptoutils/ecc/ECCParams.h>

#ifdef DeviceFamily_CC27XX
    #include <ti/drivers/ECDH.h>
    #include <ti/drivers/ECDSA.h>
#endif

/* Error mapping functions. These functions take as input SimpleLink driver
 * error codes and maps them to nearest PSA error code.
 */
psa_status_t map_AES_status(int_fast16_t status);
psa_status_t map_ECDH_status(int_fast16_t status);
psa_status_t map_ECDSA_status(int_fast16_t status, bool isVerifyOp);
psa_status_t map_EDDSA_status(int_fast16_t status, bool isVerifyOp);
psa_status_t map_RNG_status(int_fast16_t status);
psa_status_t map_SHA2_status(int_fast16_t status);
psa_status_t map_TRNG_status(int_fast16_t status);

/* Convert to and from the keyID format expected by the SimpleLink KeyStore API */
psa_key_id_t toKeyID(mbedtls_svc_key_id_t keystoreKeyID);
mbedtls_svc_key_id_t toKeyStoreKeyID(psa_key_id_t keyID);

#ifdef DeviceFamily_CC27XX
/* Maps PSA ECC key types to their corresponding ECDSA Curve Type for HSM.
 * Assumes that the keyType is some form of ECC key type. Returns 0 if could not
 * find a mapping.
 */
ECDSA_CurveType map_keyTypeToECDSACurveTypeHSM(psa_key_type_t keyType, size_t keyBits);

/* Maps PSA ECC key types to their corresponding ECDH Curve Type for HSM.
 * Assumes that the keyType is some form of ECC key type. Returns 0 if could not
 * find a mapping.
 */
ECDH_CurveType map_keyTypeToECDHCurveTypeHSM(psa_key_type_t keyType, size_t keyBits);
#endif

/* Maps PSA ECC key types to their corresponding ECCParams_CurveParams struct
 * pointer. Assumes that the keyType is some form of ECC key type. Returns NULL
 * if could not find a mapping.
 */
const ECCParams_CurveParams *map_keyTypeToECCParams(psa_key_type_t keyType, size_t keyBits);

#endif /* CRYPTO_HELPER_H */
