/*
 *  Copyright The Mbed TLS Contributors
 *  Copyright 2022-2025, Texas Instruments Incorporated
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

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#ifdef TFM_BUILD
    #include "config_tfm.h"
    #include "tfm_mbedcrypto_include.h" /* Must be included before psa/crypto.h to prefix the API names */
#else
    #include <third_party/psa_crypto/crypto_config.h>
#endif

#include <third_party/psa_crypto/include/psa/crypto.h>
#include <third_party/psa_crypto/crypto_helper.h>

#include <ti/drivers/AESCBC.h>
#include <ti/drivers/AESCCM.h>
#include <ti/drivers/AESCMAC.h>
#include <ti/drivers/AESCTR.h>
#include <ti/drivers/AESECB.h>
#include <ti/drivers/AESGCM.h>
#include <ti/drivers/ECDH.h>
#include <ti/drivers/ECDSA.h>
/* EdDSA driver is not currently supported on CC35XX */
#if (DeviceFamily_PARENT != DeviceFamily_PARENT_CC35XX)
    #include <ti/drivers/EDDSA.h>
#endif
#include <ti/drivers/RNG.h>
#include <ti/drivers/SHA2.h>
#include <ti/drivers/TRNG.h>

#include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA_init.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKeyPlaintext.h>
#include <ti/drivers/cryptoutils/ecc/ECCParams.h>

#include <ti/devices/DeviceFamily.h>

#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
     (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
    #include <ti/drivers/aescbc/AESCBCCC26XX.h>
    #include <ti/drivers/aesccm/AESCCMCC26X4.h>
    #include <ti/drivers/aescmac/AESCMACCC26XX.h>
    #include <ti/drivers/aesctr/AESCTRCC26XX.h>
    #include <ti/drivers/aesecb/AESECBCC26XX.h>
    #include <ti/drivers/aesgcm/AESGCMCC26X4.h>
    #include <ti/drivers/ecdh/ECDHCC26X2.h>
    #include <ti/drivers/ecdsa/ECDSACC26X2.h>
    #include <ti/drivers/eddsa/EDDSACC26X2.h>
    #include <ti/drivers/sha2/SHA2CC26X2.h>
    #include <ti/drivers/trng/TRNGCC26XX.h>
#elif ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    #include <ti/drivers/aescbc/AESCBCXXF3.h>
    #include <ti/drivers/aesccm/AESCCMXXF3.h>
    #include <ti/drivers/aescmac/AESCMACXXF3.h>
    #include <ti/drivers/aesctr/AESCTRXXF3.h>
    #include <ti/drivers/aesecb/AESECBXXF3.h>
    #include <ti/drivers/aesgcm/AESGCMXXF3HSM.h>
    #include <ti/drivers/ecdh/ECDHXXF3HSM.h>
    #include <ti/drivers/ecdsa/ECDSAXXF3HSM.h>
    #if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
        #include <ti/drivers/eddsa/EDDSAXXF3HSM.h>
    #endif
    #include <ti/drivers/rng/RNGXXF3HSM.h>
    #include <ti/drivers/sha2/SHA2XXF3HSM.h>

    #include <ti/drivers/cryptoutils/utils/CryptoUtils.h>
    #include <ti/drivers/cryptoutils/hsm/HSMXXF3.h>
    #include <ti/drivers/dpl/SemaphoreP.h>

    #define KeyStore_PSA_initKey KeyStore_PSA_initKeyHSM
#else
    #error "Device family not supported"
#endif

#if (TFM_ENABLED == 0) || defined(TFM_BUILD)
    /* For KeyStore_PSA_init(), KeyStore_acquireLock(), and KeyStore_releaseLock() in secure only build or TFM build */
    #include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA_helpers.h>
#endif

#if ((TFM_ENABLED == 1) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX))
    #include <ti/drivers/crypto/PSACrypto_s.h>
    #include <ti/drivers/cryptoutils/aes/AESCommonXXF3.h>
#endif /* ((TFM_ENABLED == 1) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)) */

/* The PSA Crypto Wrapper is on the NS side for Thor TFM-enabled builds so requires special handling */
#if ((TFM_ENABLED == 1) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
    /* Array for storing the mbedTLS algorithm value of a key before translating
     * to the mbed TLS value in psa_import_key. Used to recover the original
     * algorithm value in psa_get_key_attributes. For now, the application can
     * create only up to ten keys at once on tfm_enabled configurations.
     */
    #define TFM_ENABLED_KEY_STORAGE_LIMIT 10

typedef struct
{
    psa_key_id_t keyID;
    psa_algorithm_t alg;
} psa_key_data_to_reverse_mapping;

static psa_key_data_to_reverse_mapping keyAlgorithms[TFM_ENABLED_KEY_STORAGE_LIMIT];
static uint8_t keyAlgorithmsLength = 0;

#endif /* ((TFM_ENABLED == 1) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)) */

#ifndef PSA_ECC_MAX_CURVE_BITS
    /* SECP_R1_521 is the largest supported curve */
    #define PSA_ECC_MAX_CURVE_BITS 521
#endif

#ifndef PSA_OUTPUT_BUF_SIZE
    #define PSA_OUTPUT_BUF_SIZE PSA_KEY_EXPORT_ECC_PUBLIC_KEY_MAX_SIZE(PSA_ECC_MAX_CURVE_BITS)
#endif

/* Memory buffer for largest supported generated key, hash, shared secret, or
 * MAC to avoid malloc. Word-aligned to support HSM output buffer address
 * requirement.
 */
static uint8_t localBuf[PSA_OUTPUT_BUF_SIZE] __attribute__((aligned(4)));

#ifdef ENABLE_TI_CRYPTO_RNG
    #if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
/* Buffer used to byte-swap ECC curve order for key generation */
static uint8_t curveOrder[PSA_BITS_TO_BYTES(PSA_ECC_MAX_CURVE_BITS)] __attribute__((aligned(4)));
    #endif
#endif /* ENABLE_TI_CRYPTO_RNG */

/* The default value of psa_crypto_ns_caller must be false to support S-only builds.
 * For TFM-enabled builds, tfm_crypto_api_dispatcher() will set this flag for each PSA call.
 */
bool psa_crypto_ns_caller = false;

/**
 * Returns 1 if the algorithm is SHA2,
 * Otherwise, returns 0.
 */
#define PSA_ALG_IS_SHA2(alg)                                  \
    ((alg == PSA_ALG_SHA_224 || alg == PSA_ALG_SHA_256)   ? 1 \
     : (alg == PSA_ALG_SHA_384 || alg == PSA_ALG_SHA_512) ? 1 \
                                                          : 0)

#define PSA_CRYPTO_TI_DRIVER 1

typedef enum
{
    AES_ENCRYPT = 0,
    AES_DECRYPT
} aes_operation_t;

typedef enum
{
    PSA_POLLING_MODE = 0,
    PSA_BLOCKING_MODE,
    PSA_CALLBACK_MODE
} psa_return_behavior_t;

/******************************************************************************/
/* Global data, support functions and library management */
/******************************************************************************/

/* These globals are required to support operations with callback return
 * behavior since locally defined operation structs may get removed from the
 * stack before the SL driver has finished using them. A union was not used
 * for the operational structs to avoid violating MISRA rule 19.2.
 */
#ifdef ENABLE_TI_CRYPTO_AESCBC
static AESCBC_Operation aescbcOp;
static AESCBC_SegmentedOperation aescbcSegmentedOp;
#endif

#ifdef ENABLE_TI_CRYPTO_AESCCM
static AESCCM_OneStepOperation aesccmOneStepOp;
static AESCCM_SegmentedAADOperation aesccmSegmentedAadOp;
static AESCCM_SegmentedDataOperation aesccmSegmentedDataOp;
static AESCCM_SegmentedFinalizeOperation aesccmSegmentedFinalizeOp;
#endif

#ifdef ENABLE_TI_CRYPTO_AESCMAC
static AESCMAC_Operation aescmacOp;
#endif

#ifdef ENABLE_TI_CRYPTO_AESCTR
static AESCTR_Operation aesctrOp;
static AESCTR_SegmentedOperation aesctrSegmentedOp;
#endif

#ifdef ENABLE_TI_CRYPTO_AESECB
static AESECB_Operation aesecbOp;
#endif

#ifdef ENABLE_TI_CRYPTO_AESGCM
static AESGCM_OneStepOperation aesgcmOneStepOp;
static AESGCM_SegmentedAADOperation aesgcmSegmentedAadOp;
static AESGCM_SegmentedDataOperation aesgcmSegmentedDataOp;
static AESGCM_SegmentedFinalizeOperation aesgcmSegmentedFinalizeOp;
#endif

#ifdef ENABLE_TI_CRYPTO_ECDH
static ECDH_OperationComputeSharedSecret computeSharedSecretOp;
static CryptoKey sharedSecret;
#endif

#if defined(ENABLE_TI_CRYPTO_ECDH) || defined(ENABLE_TI_CRYPTO_ECDSA) || defined(ENABLE_TI_CRYPTO_EDDSA)
static CryptoKey privateKey;
static CryptoKey publicKey;
#endif

#ifdef ENABLE_TI_CRYPTO_ECDSA
static ECDSA_OperationSign ecdsaSignOp;
static ECDSA_OperationVerify ecdsaVerifyOp;
#endif

#ifdef ENABLE_TI_CRYPTO_EDDSA
static EDDSA_OperationSign eddsaSignOp;
static EDDSA_OperationVerify eddsaVerifyOp;
#endif

/* Device-specific crypto object mapping */
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
     (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))

    #define AESCBC_Object AESCBCCC26XX_Object

    #if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)
        #define AESCCM_Object AESCCMCC26X4_Object
        #define AESGCM_Object AESGCMCC26X4_Object
    #else
        #define AESCCM_Object AESCCMCC26XX_Object
        #define AESGCM_Object AESGCMCC26XX_Object
    #endif

    #define AESCMAC_Object AESCMACCC26XX_Object
    #define AESCTR_Object  AESCTRCC26XX_Object
    #define AESECB_Object  AESECBCC26XX_Object
    #define ECDH_Object    ECDHCC26X2_Object
    #define ECDSA_Object   ECDSACC26X2_Object
    #define EDDSA_Object   EDDSACC26X2_Object
    #define SHA2_Object    SHA2CC26X2_Object
    #define TRNG_Object    TRNGCC26XX_Object

#elif ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))

    #define AESCBC_Object  AESCBCXXF3_Object
    #define AESCCM_Object  AESCCMXXF3_Object
    #define AESCMAC_Object AESCMACXXF3_Object
    #define AESCTR_Object  AESCTRXXF3_Object
    #define AESECB_Object  AESECBXXF3_Object
    #define AESGCM_Object  AESGCMXXF3HSM_Object
    #define ECDH_Object    ECDHXXF3HSM_Object
    #define ECDSA_Object   ECDSAXXF3HSM_Object
    #if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
        #define EDDSA_Object EDDSAXXF3HSM_Object
    #endif
    #define RNG_Object  RNGXXF3HSM_Object
    #define SHA2_Object SHA2XXF3HSM_Object

#else
    #error "Device family not supported"
#endif

/* SimpleLink crypto driver config structs and their associated device-specific
 * object and HW attributes structs.
 */
#ifdef ENABLE_TI_CRYPTO_AESCBC
static AESCBC_Handle aescbcHandle = NULL;
static AESCBC_Config aescbcConfig;
static AESCBC_Object aescbcObject;
#endif
#ifdef ENABLE_TI_CRYPTO_AESCCM
static AESCCM_Handle aesccmHandle = NULL;
static AESCCM_Config aesccmConfig;
static AESCCM_Object aesccmObject;
#endif
#ifdef ENABLE_TI_CRYPTO_AESCMAC
static AESCMAC_Handle aescmacHandle   = NULL;
static AESCMAC_Handle aescbcmacHandle = NULL;
static AESCMAC_Config aescmacConfig;
static AESCMAC_Config aescbcmacConfig;
static AESCMAC_Object aescmacObject;
static AESCMAC_Object aescbcmacObject;
#endif
#ifdef ENABLE_TI_CRYPTO_AESCTR
static AESCTR_Handle aesctrHandle = NULL;
static AESCTR_Config aesctrConfig;
static AESCTR_Object aesctrObject;
#endif
#ifdef ENABLE_TI_CRYPTO_AESECB
static AESECB_Handle aesecbHandle = NULL;
static AESECB_Config aesecbConfig;
static AESECB_Object aesecbObject;
#endif
#ifdef ENABLE_TI_CRYPTO_AESGCM
static AESGCM_Handle aesgcmHandle = NULL;
static AESGCM_Config aesgcmConfig;
static AESGCM_Object aesgcmObject;
#endif
#ifdef ENABLE_TI_CRYPTO_ECDH
static ECDH_Handle ecdhHandle = NULL;
static ECDH_Config ecdhConfig;
static ECDH_Object ecdhObject;
#endif
#ifdef ENABLE_TI_CRYPTO_ECDSA
static ECDSA_Handle ecdsaHandle = NULL;
static ECDSA_Config ecdsaConfig;
static ECDSA_Object ecdsaObject;
#endif
#ifdef ENABLE_TI_CRYPTO_EDDSA
static EDDSA_Handle eddsaHandle = NULL;
static EDDSA_Config eddsaConfig;
static EDDSA_Object eddsaObject;
#endif
#if defined(ENABLE_TI_CRYPTO_RNG)
static RNG_Handle rngHandle = NULL;
static RNG_Config rngConfig;
static RNG_Object rngObject;
#endif
#ifdef ENABLE_TI_CRYPTO_SHA2
static SHA2_Handle sha2Handle = NULL;
static SHA2_Config sha2Config;
static SHA2_Object sha2Object;
#endif
#if defined(ENABLE_TI_CRYPTO_TRNG)
static TRNG_Handle trngHandle = NULL;
static TRNG_Config trngConfig;
static TRNG_Object trngObject;
#endif

#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
     (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))

    #ifdef ENABLE_TI_CRYPTO_AESCBC
static const AESCBCCC26XX_HWAttrs aescbcHWAttrs = {.intPriority = (~0)};
    #endif

    #if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)
        #ifdef ENABLE_TI_CRYPTO_AESCCM
static const AESCCMCC26X4_HWAttrs aesccmHWAttrs = {.intPriority = (~0)};
        #endif
        #ifdef ENABLE_TI_CRYPTO_AESGCM
static const AESGCMCC26X4_HWAttrs aesgcmHWAttrs = {.intPriority = (~0)};
        #endif
    #else /* DeviceFamily_PARENT_CC13X2_CC26X2 */
        #ifdef ENABLE_TI_CRYPTO_AESCCM
static const AESCCMCC26XX_HWAttrs aesccmHWAttrs = {.intPriority = (~0)};
        #endif
        #ifdef ENABLE_TI_CRYPTO_AESGCM
static const AESGCMCC26XX_HWAttrs aesgcmHWAttrs = {.intPriority = (~0)};
        #endif
    #endif

    #ifdef ENABLE_TI_CRYPTO_AESCMAC
static const AESCMACCC26XX_HWAttrs aescmacHWAttrs   = {.intPriority = (~0)};
static const AESCMACCC26XX_HWAttrs aescbcmacHWAttrs = {.intPriority = (~0)};
    #endif

    #ifdef ENABLE_TI_CRYPTO_AESCTR
static const AESCTRCC26XX_HWAttrs aesctrHWAttrs = {.intPriority = (~0)};
    #endif

    #ifdef ENABLE_TI_CRYPTO_AESECB
static const AESECBCC26XX_HWAttrs aesecbHWAttrs = {.intPriority = (~0)};
    #endif

    #ifdef ENABLE_TI_CRYPTO_ECDH
static ECDHCC26X2_HWAttrs ecdhHWAttrs = {.intPriority = (~0)};
    #endif

    #ifdef ENABLE_TI_CRYPTO_ECDSA
static ECDSACC26X2_HWAttrs ecdsaHWAttrs = {.intPriority = (~0), .trngIntPriority = (~0)};
    #endif

    #ifdef ENABLE_TI_CRYPTO_EDDSA
static EDDSACC26X2_HWAttrs eddsaHWAttrs = {.intPriority = (~0), .sha2IntPriority = (~0)};
    #endif

    #ifdef ENABLE_TI_CRYPTO_SHA2
static SHA2CC26X2_HWAttrs sha2HWAttrs = {.intPriority = (~0)};
    #endif

    #ifdef ENABLE_TI_CRYPTO_TRNG
static TRNGCC26XX_HWAttrs trngHWAttrs = {.intPriority = (~0), .swiPriority = 0, .samplesPerCycle = 240000};
    #endif

#elif ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))

    #ifdef ENABLE_TI_CRYPTO_AESCBC
static const AESCBCXXF3_HWAttrs aescbcHWAttrs     = {.intPriority = (~0)};
    #endif

    #ifdef ENABLE_TI_CRYPTO_AESCCM
static const AESCCMXXF3_HWAttrs aesccmHWAttrs     = {.intPriority = (~0)};
    #endif

    #ifdef ENABLE_TI_CRYPTO_AESCMAC
static const AESCMACXXF3_HWAttrs aescmacHWAttrs   = {.intPriority = (~0)};
static const AESCMACXXF3_HWAttrs aescbcmacHWAttrs = {.intPriority = (~0)};
    #endif

    #ifdef ENABLE_TI_CRYPTO_AESCTR
static const AESCTRXXF3_HWAttrs aesctrHWAttrs     = {.intPriority = (~0)};
    #endif

    #ifdef ENABLE_TI_CRYPTO_AESECB
static const AESECBXXF3_HWAttrs aesecbHWAttrs     = {.intPriority = (~0)};
    #endif

    #ifdef ENABLE_TI_CRYPTO_AESGCM
static const AESGCMXXF3HSM_HWAttrs aesgcmHWAttrs  = {.intPriority = (~0)};
    #endif

    #ifdef ENABLE_TI_CRYPTO_ECDH
static ECDHXXF3HSM_HWAttrs ecdhHWAttrs            = {.reserved1 = 0};
    #endif

    #ifdef ENABLE_TI_CRYPTO_ECDSA
static ECDSAXXF3HSM_HWAttrs ecdsaHWAttrs          = {.dummy = 0};
    #endif

    #ifdef ENABLE_TI_CRYPTO_EDDSA
        #if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
static EDDSAXXF3HSM_HWAttrs eddsaHWAttrs          = {.reserved1 = 0};
        #endif
    #endif

    #ifdef ENABLE_TI_CRYPTO_RNG
static RNGXXF3HSM_HWAttrs rngHWAttrs              = {.reserved1 = 0};
    #endif

    #ifdef ENABLE_TI_CRYPTO_SHA2
static SHA2XXF3HSM_HWAttrs sha2HWAttrs            = {.reserved1 = 0};
    #endif

#else
    #error "Device family not supported"
#endif

#ifdef TFM_BUILD
/* When PSA Crypto is built into the TFM, the SL crypto drivers are used in callback mode */

/* Crypto Driver Callback Functions */
extern void PSACrypto_s_aescbcHwiCallback(AESCBC_Handle handle,
                                          int_fast16_t returnValue,
                                          AESCBC_OperationUnion *operation,
                                          AESCBC_OperationType operationType);
extern void PSACrypto_s_aesccmHwiCallback(AESCCM_Handle handle,
                                          int_fast16_t returnValue,
                                          AESCCM_OperationUnion *operation,
                                          AESCCM_OperationType operationType);
extern void PSACrypto_s_aescmacHwiCallback(AESCMAC_Handle handle,
                                           int_fast16_t returnValue,
                                           AESCMAC_Operation *operation,
                                           AESCMAC_OperationType operationType);
extern void PSACrypto_s_aescbcmacHwiCallback(AESCMAC_Handle handle,
                                             int_fast16_t returnValue,
                                             AESCMAC_Operation *operation,
                                             AESCMAC_OperationType operationType);
extern void PSACrypto_s_aesctrHwiCallback(AESCTR_Handle handle,
                                          int_fast16_t returnValue,
                                          AESCTR_OperationUnion *operation,
                                          AESCTR_OperationType operationType);
extern void PSACrypto_s_aesecbHwiCallback(AESECB_Handle handle,
                                          int_fast16_t returnValue,
                                          AESECB_Operation *operation,
                                          AESECB_OperationType operationType);
extern void PSACrypto_s_aesgcmHwiCallback(AESGCM_Handle handle,
                                          int_fast16_t returnValue,
                                          AESGCM_OperationUnion *operation,
                                          AESGCM_OperationType operationType);
extern void PSACrypto_s_ecdhHwiCallback(ECDH_Handle handle,
                                        int_fast16_t returnStatus,
                                        ECDH_Operation operation,
                                        ECDH_OperationType operationType);
extern void PSACrypto_s_ecdsaHwiCallback(ECDSA_Handle handle,
                                         int_fast16_t returnStatus,
                                         ECDSA_Operation operation,
                                         ECDSA_OperationType operationType);
extern void PSACrypto_s_eddsaHwiCallback(EDDSA_Handle handle,
                                         int_fast16_t returnStatus,
                                         EDDSA_Operation operation,
                                         EDDSA_OperationType operationType);
extern void PSACrypto_s_sha2HwiCallback(SHA2_Handle handle, int_fast16_t returnStatus);

extern PSACrypto_s_SecureCallback *PSACrypto_s_secureCB;

#endif /* TFM_ENABLED */

#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))

/* Externs for KeyMgmt functions */
extern psa_status_t KeyMgmt_psa_copy_key(mbedtls_svc_key_id_t source_key,
                                         const psa_key_attributes_t *attributes,
                                         mbedtls_svc_key_id_t *target_key);
extern psa_status_t KeyMgmt_psa_crypto_init(void);
extern psa_status_t KeyMgmt_psa_destroy_key(mbedtls_svc_key_id_t key);
extern psa_status_t KeyMgmt_psa_export_key(mbedtls_svc_key_id_t key,
                                           uint8_t *data,
                                           size_t data_size,
                                           size_t *data_length);
extern psa_status_t KeyMgmt_psa_export_public_key(mbedtls_svc_key_id_t key,
                                                  uint8_t *data,
                                                  size_t data_size,
                                                  size_t *data_length);
extern psa_status_t KeyMgmt_psa_get_key_attributes(mbedtls_svc_key_id_t key, psa_key_attributes_t *attributes);
extern psa_status_t KeyMgmt_psa_import_key(const psa_key_attributes_t *attributes,
                                           const uint8_t *data,
                                           size_t data_length,
                                           mbedtls_svc_key_id_t *key);
extern psa_status_t KeyMgmt_psa_purge_key(mbedtls_svc_key_id_t key);
extern void KeyMgmt_psa_reset_key_attributes(psa_key_attributes_t *attributes);

extern psa_status_t KeyMgmt_psa_key_derivation_setup(psa_key_derivation_operation_t *operation, psa_algorithm_t alg);
extern psa_status_t KeyMgmt_psa_key_derivation_get_capacity(const psa_key_derivation_operation_t *operation,
                                                            size_t *capacity);
extern psa_status_t KeyMgmt_psa_key_derivation_set_capacity(psa_key_derivation_operation_t *operation, size_t capacity);
extern psa_status_t KeyMgmt_psa_key_derivation_input_bytes(psa_key_derivation_operation_t *operation,
                                                           psa_key_derivation_step_t step,
                                                           const uint8_t *data,
                                                           size_t data_length);
extern psa_status_t KeyMgmt_psa_key_derivation_input_integer(psa_key_derivation_operation_t *operation,
                                                             psa_key_derivation_step_t step,
                                                             uint64_t value);
extern psa_status_t KeyMgmt_psa_key_derivation_input_key(psa_key_derivation_operation_t *operation,
                                                         psa_key_derivation_step_t step,
                                                         mbedtls_svc_key_id_t key);
extern psa_status_t KeyMgmt_psa_key_derivation_output_key(const psa_key_attributes_t *attributes,
                                                          psa_key_derivation_operation_t *operation,
                                                          mbedtls_svc_key_id_t *key);
extern psa_status_t KeyMgmt_psa_key_derivation_verify_bytes(psa_key_derivation_operation_t *operation,
                                                            const uint8_t *expected_output,
                                                            size_t output_length);
extern psa_status_t KeyMgmt_psa_key_derivation_verify_key(psa_key_derivation_operation_t *operation,
                                                          mbedtls_svc_key_id_t expected);
extern psa_status_t KeyMgmt_psa_key_derivation_abort(psa_key_derivation_operation_t *operation);

extern psa_status_t tfm_its_init(void);
#endif /* ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)) \
        */

/* Forward declarations */
static psa_status_t psa_generate_random_internal(uint8_t *output, size_t output_size);

/* Functions to use fixed IV value for testing/debug purposes */
#if defined(PSA_DEBUG)
static uint8_t *iv;
static uint8_t iv_set = 0U;
/*
 *  ======== psa_debug_get_iv ========
 */
psa_status_t psa_debug_get_iv(uint8_t **new_iv)
{
    if (iv_set == 0U)
    {
        return PSA_ERROR_BAD_STATE;
    }

    iv_set  = 0U;
    *new_iv = iv;
    return PSA_SUCCESS;
}

/*
 *  ======== psa_debug_set_iv ========
 */
void psa_debug_set_iv(uint8_t *new_iv)
{
    iv_set = 1U;
    iv     = new_iv;
}
#endif

#if (defined(ENABLE_TI_CRYPTO_AESCBC) || defined(ENABLE_TI_CRYPTO_AESCCM) || defined(ENABLE_TI_CRYPTO_AESCMAC) || \
     defined(ENABLE_TI_CRYPTO_AESCTR) || defined(ENABLE_TI_CRYPTO_AESECB) || defined(ENABLE_TI_CRYPTO_AESGCM))

    #if defined(TFM_BUILD) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== psa_set_aes_return_behavior ========
 */
static void psa_set_aes_return_behavior(AESCommonXXF3_Object *object, psa_return_behavior_t returnBehavior)
{
    if (returnBehavior == PSA_POLLING_MODE)
    {
        object->returnBehavior = AES_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        object->returnBehavior = AES_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        object->returnBehavior = AES_RETURN_BEHAVIOR_CALLBACK;
    }
}
    #endif /* TFM_BUILD && DeviceFamily_PARENT_CC27XX */

#endif /* (defined(ENABLE_TI_CRYPTO_AESCBC) || defined(ENABLE_TI_CRYPTO_AESCCM) || \
          defined(ENABLE_TI_CRYPTO_AESCMAC) || defined(ENABLE_TI_CRYPTO_AESCTR) || \
          defined(ENABLE_TI_CRYPTO_AESECB) || defined(ENABLE_TI_CRYPTO_AESGCM)) */

#ifdef ENABLE_TI_CRYPTO_AESCBC
/*
 *  ======== psa_aescbc_construct ========
 */
static void psa_aescbc_construct(psa_return_behavior_t returnBehavior)
{
    AESCBC_Params aescbcParams;
    AESCBC_Params_init(&aescbcParams);

    if (returnBehavior == PSA_POLLING_MODE)
    {
        aescbcParams.returnBehavior = AESCBC_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        aescbcParams.returnBehavior = AESCBC_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        aescbcParams.returnBehavior = AESCBC_RETURN_BEHAVIOR_CALLBACK;
    #ifdef TFM_BUILD
        aescbcParams.callbackFxn = PSACrypto_s_aescbcHwiCallback;
    #endif
    }

    memset(&aescbcObject, 0x0, sizeof(aescbcObject));
    aescbcConfig.object  = &aescbcObject;
    aescbcConfig.hwAttrs = &aescbcHWAttrs;

    aescbcHandle = AESCBC_construct(&aescbcConfig, &aescbcParams);
}

    #if defined(TFM_BUILD) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== psa_aescbc_set_return_behavior ========
 */
static void psa_aescbc_set_return_behavior(psa_return_behavior_t returnBehavior)
{
    AESCBC_Object *object = (AESCBC_Object *)aescbcHandle->object;

    psa_set_aes_return_behavior(&object->common, returnBehavior);
}
    #endif
#endif

#ifdef ENABLE_TI_CRYPTO_AESCCM
/*
 *  ======== psa_aesccm_construct ========
 */
static void psa_aesccm_construct(psa_return_behavior_t returnBehavior)
{
    AESCCM_Params aesccmParams;
    AESCCM_Params_init(&aesccmParams);

    if (returnBehavior == PSA_POLLING_MODE)
    {
        aesccmParams.returnBehavior = AESCCM_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        aesccmParams.returnBehavior = AESCCM_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        aesccmParams.returnBehavior = AESCCM_RETURN_BEHAVIOR_CALLBACK;
    #ifdef TFM_BUILD
        aesccmParams.callbackFxn = PSACrypto_s_aesccmHwiCallback;
    #endif
    }

    memset(&aesccmObject, 0x0, sizeof(aesccmObject));
    aesccmConfig.object  = &aesccmObject;
    aesccmConfig.hwAttrs = &aesccmHWAttrs;

    aesccmHandle = AESCCM_construct(&aesccmConfig, &aesccmParams);
}
    #if defined(TFM_BUILD) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== psa_aesccm_set_return_behavior ========
 */
static void psa_aesccm_set_return_behavior(psa_return_behavior_t returnBehavior)
{
    AESCCM_Object *object = (AESCCM_Object *)aesccmHandle->object;

    psa_set_aes_return_behavior(&object->common, returnBehavior);
}
    #endif
#endif

#ifdef ENABLE_TI_CRYPTO_AESCMAC
static void psa_aescmac_construct(psa_return_behavior_t returnBehavior)
{
    AESCMAC_Params aescmacParams;
    AESCMAC_Params_init(&aescmacParams);

    if (returnBehavior == PSA_POLLING_MODE)
    {
        aescmacParams.returnBehavior = AESCMAC_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        aescmacParams.returnBehavior = AESCMAC_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        aescmacParams.returnBehavior = AESCMAC_RETURN_BEHAVIOR_CALLBACK;
    #ifdef TFM_BUILD
        aescmacParams.callbackFxn = PSACrypto_s_aescmacHwiCallback;
    #endif
    }

    memset(&aescmacObject, 0x0, sizeof(aescmacObject));
    aescmacConfig.object  = &aescmacObject;
    aescmacConfig.hwAttrs = &aescmacHWAttrs;

    aescmacHandle = AESCMAC_construct(&aescmacConfig, &aescmacParams);
}

    #if defined(TFM_BUILD) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== psa_aescmac_set_return_behavior ========
 */
static void psa_aescmac_set_return_behavior(AESCMAC_Handle handle, psa_return_behavior_t returnBehavior)
{
    AESCMAC_Object *object = (AESCMAC_Object *)handle->object;

    psa_set_aes_return_behavior(&object->common, returnBehavior);
}

    #endif

/*
 *  ======== psa_aescbcmac_construct ========
 */
static void psa_aescbcmac_construct(psa_return_behavior_t returnBehavior)
{
    AESCMAC_Params aescbcmacParams;
    AESCMAC_Params_init(&aescbcmacParams);

    aescbcmacParams.operationalMode = AESCMAC_OPMODE_CBCMAC;

    if (returnBehavior == PSA_POLLING_MODE)
    {
        aescbcmacParams.returnBehavior = AESCMAC_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        aescbcmacParams.returnBehavior = AESCMAC_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        aescbcmacParams.returnBehavior = AESCMAC_RETURN_BEHAVIOR_CALLBACK;
    #ifdef TFM_BUILD
        aescbcmacParams.callbackFxn = PSACrypto_s_aescbcmacHwiCallback;
    #endif
    }

    memset(&aescbcmacObject, 0x0, sizeof(aescbcmacObject));
    aescbcmacConfig.object  = &aescbcmacObject;
    aescbcmacConfig.hwAttrs = &aescbcmacHWAttrs;

    aescbcmacHandle = AESCMAC_construct(&aescbcmacConfig, &aescbcmacParams);
}

static AESCMAC_Handle psa_aescmac_get_handle(psa_algorithm_t alg)
{
    AESCMAC_Handle handle;

    if (alg == PSA_ALG_CMAC)
    {
        handle = aescmacHandle;
    }
    else
    {
        handle = aescbcmacHandle;
    }

    return handle;
}
#endif

#ifdef ENABLE_TI_CRYPTO_AESCTR
/*
 *  ======== psa_aesctr_construct ========
 */
static void psa_aesctr_construct(psa_return_behavior_t returnBehavior)
{
    AESCTR_Params aesctrParams;
    AESCTR_Params_init(&aesctrParams);

    if (returnBehavior == PSA_POLLING_MODE)
    {
        aesctrParams.returnBehavior = AESCTR_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        aesctrParams.returnBehavior = AESCTR_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        aesctrParams.returnBehavior = AESCTR_RETURN_BEHAVIOR_CALLBACK;
    #ifdef TFM_BUILD
        aesctrParams.callbackFxn = PSACrypto_s_aesctrHwiCallback;
    #endif
    }

    memset(&aesctrObject, 0x0, sizeof(aesctrObject));
    aesctrConfig.object  = &aesctrObject;
    aesctrConfig.hwAttrs = &aesctrHWAttrs;

    aesctrHandle = AESCTR_construct(&aesctrConfig, &aesctrParams);
}

    #if defined(TFM_BUILD) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== psa_aesctr_set_return_behavior ========
 */
static void psa_aesctr_set_return_behavior(psa_return_behavior_t returnBehavior)
{
    AESCTR_Object *object = (AESCTR_Object *)aesctrHandle->object;

    psa_set_aes_return_behavior(&object->common, returnBehavior);
}
    #endif
#endif

#ifdef ENABLE_TI_CRYPTO_AESECB
/*
 *  ======== psa_aesecb_construct ========
 */
static void psa_aesecb_construct(psa_return_behavior_t returnBehavior)
{
    AESECB_Params aesecbParams;
    AESECB_Params_init(&aesecbParams);

    if (returnBehavior == PSA_POLLING_MODE)
    {
        aesecbParams.returnBehavior = AESECB_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        aesecbParams.returnBehavior = AESECB_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        aesecbParams.returnBehavior = AESECB_RETURN_BEHAVIOR_CALLBACK;
    #ifdef TFM_BUILD
        aesecbParams.callbackFxn = PSACrypto_s_aesecbHwiCallback;
    #endif
    }

    memset(&aesecbObject, 0x0, sizeof(aesecbObject));
    aesecbConfig.object  = &aesecbObject;
    aesecbConfig.hwAttrs = &aesecbHWAttrs;

    aesecbHandle = AESECB_construct(&aesecbConfig, &aesecbParams);
}

    #if defined(TFM_BUILD) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== psa_aesecb_set_return_behavior ========
 */
static void psa_aesecb_set_return_behavior(psa_return_behavior_t returnBehavior)
{
    AESECB_Object *object = (AESECB_Object *)aesecbHandle->object;

    psa_set_aes_return_behavior(&object->common, returnBehavior);
}
    #endif
#endif

#ifdef ENABLE_TI_CRYPTO_AESGCM
/*
 *  ======== psa_aesgcm_construct ========
 */
static void psa_aesgcm_construct(psa_return_behavior_t returnBehavior)
{
    AESGCM_Params aesgcmParams;
    AESGCM_Params_init(&aesgcmParams);

    if (returnBehavior == PSA_POLLING_MODE)
    {
        aesgcmParams.returnBehavior = AESGCM_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        aesgcmParams.returnBehavior = AESGCM_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        aesgcmParams.returnBehavior = AESGCM_RETURN_BEHAVIOR_CALLBACK;
    #ifdef TFM_BUILD
        aesgcmParams.callbackFxn = PSACrypto_s_aesgcmHwiCallback;
    #endif
    }

    memset(&aesgcmObject, 0x0, sizeof(aesgcmObject));
    aesgcmConfig.object  = &aesgcmObject;
    aesgcmConfig.hwAttrs = &aesgcmHWAttrs;

    aesgcmHandle = AESGCM_construct(&aesgcmConfig, &aesgcmParams);
}

    #if defined(TFM_BUILD) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== psa_aesgcm_set_return_behavior ========
 */
static void psa_aesgcm_set_return_behavior(psa_return_behavior_t returnBehavior)
{
    AESGCM_Object *object = (AESGCM_Object *)aesgcmHandle->object;

    psa_set_aes_return_behavior(&object->common, returnBehavior);
}
    #endif
#endif

#ifdef ENABLE_TI_CRYPTO_ECDH
/*
 *  ======== psa_ecdh_construct ========
 */
static void psa_ecdh_construct(psa_return_behavior_t returnBehavior)
{
    ECDH_Params ecdhParams;
    ECDH_Params_init(&ecdhParams);

    if (returnBehavior == PSA_POLLING_MODE)
    {
        ecdhParams.returnBehavior = ECDH_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        ecdhParams.returnBehavior = ECDH_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        ecdhParams.returnBehavior = ECDH_RETURN_BEHAVIOR_CALLBACK;
    #ifdef TFM_BUILD
        ecdhParams.callbackFxn = PSACrypto_s_ecdhHwiCallback;
    #endif
    }

    memset(&ecdhObject, 0x0, sizeof(ecdhObject));
    ecdhConfig.object  = &ecdhObject;
    ecdhConfig.hwAttrs = &ecdhHWAttrs;

    ecdhHandle = ECDH_construct(&ecdhConfig, &ecdhParams);
}

    #if defined(TFM_BUILD) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== psa_ecdh_set_return_behavior ========
 */
static void psa_ecdh_set_return_behavior(psa_return_behavior_t returnBehavior)
{
    ECDH_Object *object = (ECDH_Object *)ecdhHandle->object;

    if (returnBehavior == PSA_POLLING_MODE)
    {
        object->returnBehavior = ECDH_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        object->returnBehavior = ECDH_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        object->returnBehavior = ECDH_RETURN_BEHAVIOR_CALLBACK;
    }
}
    #endif
#endif

#ifdef ENABLE_TI_CRYPTO_ECDSA
/*
 *  ======== psa_ecdsa_construct ========
 */
static void psa_ecdsa_construct(psa_return_behavior_t returnBehavior)
{
    ECDSA_Params ecdsaParams;
    ECDSA_Params_init(&ecdsaParams);

    if (returnBehavior == PSA_POLLING_MODE)
    {
        ecdsaParams.returnBehavior = ECDSA_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        ecdsaParams.returnBehavior = ECDSA_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        ecdsaParams.returnBehavior = ECDSA_RETURN_BEHAVIOR_CALLBACK;
    #ifdef TFM_BUILD
        ecdsaParams.callbackFxn = PSACrypto_s_ecdsaHwiCallback;
    #endif
    }

    memset(&ecdsaObject, 0x0, sizeof(ecdsaObject));
    ecdsaConfig.object  = &ecdsaObject;
    ecdsaConfig.hwAttrs = &ecdsaHWAttrs;

    ecdsaHandle = ECDSA_construct(&ecdsaConfig, &ecdsaParams);
}

    #if defined(TFM_BUILD) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== psa_ecdsa_set_return_behavior ========
 */
static void psa_ecdsa_set_return_behavior(psa_return_behavior_t returnBehavior)
{
    ECDSA_Object *object = (ECDSA_Object *)ecdsaHandle->object;

    if (returnBehavior == PSA_POLLING_MODE)
    {
        object->returnBehavior = ECDSA_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        object->returnBehavior = ECDSA_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        object->returnBehavior = ECDSA_RETURN_BEHAVIOR_CALLBACK;
    }
}
    #endif
#endif

#ifdef ENABLE_TI_CRYPTO_EDDSA
/*
 *  ======== psa_eddsa_construct ========
 */
static void psa_eddsa_construct(psa_return_behavior_t returnBehavior)
{
    EDDSA_Params eddsaParams;
    EDDSA_Params_init(&eddsaParams);

    if (returnBehavior == PSA_POLLING_MODE)
    {
        eddsaParams.returnBehavior = EDDSA_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        eddsaParams.returnBehavior = EDDSA_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        eddsaParams.returnBehavior = EDDSA_RETURN_BEHAVIOR_CALLBACK;
    #ifdef TFM_BUILD
        eddsaParams.callbackFxn = PSACrypto_s_eddsaHwiCallback;
    #endif
    }

    memset(&eddsaObject, 0x0, sizeof(eddsaObject));
    eddsaConfig.object  = &eddsaObject;
    eddsaConfig.hwAttrs = &eddsaHWAttrs;

    eddsaHandle = EDDSA_construct(&eddsaConfig, &eddsaParams);
}

    #if defined(TFM_BUILD) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== psa_eddsa_set_return_behavior ========
 */
static void psa_eddsa_set_return_behavior(psa_return_behavior_t returnBehavior)
{
    EDDSA_Object *object = (EDDSA_Object *)eddsaHandle->object;

    if (returnBehavior == PSA_POLLING_MODE)
    {
        object->returnBehavior = EDDSA_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        object->returnBehavior = EDDSA_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        object->returnBehavior = EDDSA_RETURN_BEHAVIOR_CALLBACK;
    }
}
    #endif
#endif

#ifdef ENABLE_TI_CRYPTO_SHA2
/*
 *  ======== psa_sha2_construct ========
 */
static void psa_sha2_construct(psa_return_behavior_t returnBehavior)
{
    SHA2_Params sha2Params;

    if (sha2Handle != NULL)
    {
        SHA2_close(sha2Handle);
        sha2Handle = NULL;
    }

    SHA2_Params_init(&sha2Params);

    if (returnBehavior == PSA_POLLING_MODE)
    {
        sha2Params.returnBehavior = SHA2_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        sha2Params.returnBehavior = SHA2_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        sha2Params.returnBehavior = SHA2_RETURN_BEHAVIOR_CALLBACK;
    #ifdef TFM_BUILD
        sha2Params.callbackFxn = PSACrypto_s_sha2HwiCallback;
    #endif
    }

    memset(&sha2Object, 0x0, sizeof(sha2Object));
    sha2Config.object  = &sha2Object;
    sha2Config.hwAttrs = &sha2HWAttrs;

    sha2Handle = SHA2_construct(&sha2Config, &sha2Params);
}

    #if defined(TFM_BUILD) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== psa_sha2_set_return_behavior ========
 */
static void psa_sha2_set_return_behavior(psa_return_behavior_t returnBehavior)
{
    SHA2_Object *object = (SHA2_Object *)sha2Handle->object;

    if (returnBehavior == PSA_POLLING_MODE)
    {
        object->returnBehavior = SHA2_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        object->returnBehavior = SHA2_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
        object->returnBehavior = SHA2_RETURN_BEHAVIOR_CALLBACK;
    }
}
    #endif

#endif

#ifdef ENABLE_TI_CRYPTO_RNG
/*
 *  ======== psa_rng_construct ========
 */
static void psa_rng_construct(psa_return_behavior_t returnBehavior)
{
    RNG_Params rngParams;
    RNG_Params_init(&rngParams);

    /* Note: For LPF3 family devices, the RNG driver uses a global return
     * behavior set by SysConfig and the return behavior in the RNG params is ignored.
     */
    if (returnBehavior == PSA_POLLING_MODE)
    {
        rngParams.returnBehavior = RNG_RETURN_BEHAVIOR_POLLING;
    }
    else if (returnBehavior == PSA_BLOCKING_MODE)
    {
        rngParams.returnBehavior = RNG_RETURN_BEHAVIOR_BLOCKING;
    }
    else
    {
    #if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
        /* RNG_construct() will return NULL if RNG_RETURN_BEHAVIOR_CALLBACK is
         * specified so set to polling to match SysConfig default.
         */
        rngParams.returnBehavior = RNG_RETURN_BEHAVIOR_POLLING;
    #else
        rngParams.returnBehavior = RNG_RETURN_BEHAVIOR_CALLBACK;
        #error "Callback function must be implemented"
    #endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */
    }

    memset(&rngObject, 0x0, sizeof(rngObject));
    rngConfig.object  = &rngObject;
    rngConfig.hwAttrs = &rngHWAttrs;

    rngHandle = RNG_construct(&rngConfig, &rngParams);
}

#endif

#ifdef TFM_BUILD

/*
 *  ======== psa_setup_return_behavior ========
 */
void psa_setup_return_behavior(bool ns_caller)
{
    psa_crypto_ns_caller = ns_caller;

    if (ns_caller)
    {
    #ifdef ENABLE_TI_CRYPTO_AESCBC
        psa_aescbc_set_return_behavior(PSA_CALLBACK_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_AESCMAC
        psa_aescmac_set_return_behavior(aescmacHandle, PSA_CALLBACK_MODE);
        psa_aescmac_set_return_behavior(aescbcmacHandle, PSA_CALLBACK_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_AESCCM
        psa_aesccm_set_return_behavior(PSA_CALLBACK_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_AESCTR
        psa_aesctr_set_return_behavior(PSA_CALLBACK_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_AESECB
        psa_aesecb_set_return_behavior(PSA_CALLBACK_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_AESGCM
        psa_aesgcm_set_return_behavior(PSA_CALLBACK_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_ECDH
        psa_ecdh_set_return_behavior(PSA_CALLBACK_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_ECDSA
        psa_ecdsa_set_return_behavior(PSA_CALLBACK_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_EDDSA
        psa_eddsa_set_return_behavior(PSA_CALLBACK_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_SHA2
        psa_sha2_set_return_behavior(PSA_CALLBACK_MODE);
    #endif
        /* Note: For LPF3 devices, the return behavior for RNG is fixed in
         * SysConfig and not configurable.
         */
    #ifdef ENABLE_TI_CRYPTO_TRNG
        #error "psa_setup_return_behavior does not support TRNG"
    #endif
    }
    else
    {
        /* For secure callers, all operations must be completed in polling mode */
    #ifdef ENABLE_TI_CRYPTO_AESCBC
        psa_aescbc_set_return_behavior(PSA_POLLING_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_AESCMAC
        psa_aescmac_set_return_behavior(aescmacHandle, PSA_POLLING_MODE);
        psa_aescmac_set_return_behavior(aescbcmacHandle, PSA_POLLING_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_AESCCM
        psa_aesccm_set_return_behavior(PSA_POLLING_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_AESCTR
        psa_aesctr_set_return_behavior(PSA_POLLING_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_AESECB
        psa_aesecb_set_return_behavior(PSA_POLLING_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_AESGCM
        psa_aesgcm_set_return_behavior(PSA_POLLING_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_ECDH
        psa_ecdh_set_return_behavior(PSA_POLLING_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_ECDSA
        psa_ecdsa_set_return_behavior(PSA_POLLING_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_EDDSA
        psa_eddsa_set_return_behavior(PSA_POLLING_MODE);
    #endif
    #ifdef ENABLE_TI_CRYPTO_SHA2
        psa_sha2_set_return_behavior(PSA_POLLING_MODE);
    #endif
        /* Note: For LPF3 devices, the return behavior for RNG is fixed in
         * SysConfig and not configurable.
         */
    }
}

#endif /* TFM_BUILD */

#ifdef ENABLE_TI_CRYPTO_AESCCM
/*
 *  ======== psa_validate_ccm_tag_length ========
 */
static psa_status_t psa_validate_ccm_tag_length(size_t tagLength)
{
    psa_status_t status = PSA_ERROR_INVALID_ARGUMENT;

    if ((tagLength == 4) || (tagLength == 6) || (tagLength == 8) || (tagLength == 10) || (tagLength == 12) ||
        (tagLength == 14) || (tagLength == 16))
    {
        status = PSA_SUCCESS;
    }

    return status;
}
#endif

#ifdef ENABLE_TI_CRYPTO_AESGCM
/*
 *  ======== psa_validate_gcm_tag_length ========
 */
static psa_status_t psa_validate_gcm_tag_length(size_t tagLength)
{
    psa_status_t status = PSA_ERROR_INVALID_ARGUMENT;

    if ((tagLength == 4) || (tagLength == 8) || ((tagLength >= 12) && (tagLength <= 16)))
    {
        status = PSA_SUCCESS;
    }

    return status;
}
#endif

/*
 *  ======== psa_check_aes_handle ========
 */
static psa_status_t psa_check_aes_handle(psa_algorithm_t alg)
{
    psa_status_t status = PSA_ERROR_BAD_STATE;

    switch (alg)
    {
#ifdef ENABLE_TI_CRYPTO_AESCBC
        case PSA_ALG_CBC_NO_PADDING:
            if (aescbcHandle != NULL)
            {
                status = PSA_SUCCESS;
            }
            break;
#endif

#ifdef ENABLE_TI_CRYPTO_AESCCM
        case PSA_ALG_CCM:
            if (aesccmHandle != NULL)
            {
                status = PSA_SUCCESS;
            }
            break;
#endif

#ifdef ENABLE_TI_CRYPTO_AESCMAC
        case PSA_ALG_CMAC:
            if (aescmacHandle != NULL)
            {
                status = PSA_SUCCESS;
            }
            break;

        case PSA_ALG_CBC_MAC:
            if (aescbcmacHandle != NULL)
            {
                status = PSA_SUCCESS;
            }
            break;
#endif

#ifdef ENABLE_TI_CRYPTO_AESCTR
        case PSA_ALG_CTR:
            if (aesctrHandle != NULL)
            {
                status = PSA_SUCCESS;
            }
            break;
#endif

#ifdef ENABLE_TI_CRYPTO_AESGCM
        case PSA_ALG_GCM:
            if (aesgcmHandle != NULL)
            {
                status = PSA_SUCCESS;
            }
            break;
#endif

#ifdef ENABLE_TI_CRYPTO_AESECB
        case PSA_ALG_ECB_NO_PADDING:
            if (aesecbHandle != NULL)
            {
                status = PSA_SUCCESS;
            }
            break;
#endif

        default:
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
    }

    return status;
}

#ifdef ENABLE_TI_CRYPTO_SHA2
/*
 *  ======== psa_set_sha2_hash_type ========
 */
static psa_status_t psa_set_sha2_hash_type(psa_algorithm_t algorithm)
{
    int_fast16_t ret;
    psa_status_t status;

    if (sha2Handle == NULL)
    {
        return PSA_ERROR_BAD_STATE;
    }

    switch (algorithm)
    {
        case PSA_ALG_SHA_224:
            ret = SHA2_setHashType(sha2Handle, SHA2_HASH_TYPE_224);
            break;

        case PSA_ALG_SHA_256:
            ret = SHA2_setHashType(sha2Handle, SHA2_HASH_TYPE_256);
            break;

        case PSA_ALG_SHA_384:
            ret = SHA2_setHashType(sha2Handle, SHA2_HASH_TYPE_384);
            break;

        case PSA_ALG_SHA_512:
            ret = SHA2_setHashType(sha2Handle, SHA2_HASH_TYPE_512);
            break;

        default:
            ret = SHA2_STATUS_UNSUPPORTED;
            break;
    }

    status = map_SHA2_status(ret);

    return status;
}
#endif

/******************************************************************************/
/* Key management */
/******************************************************************************/

/** Check whether a given key type is valid for use with a given MAC algorithm
 *
 * Upon successful return of this function, the behavior of #PSA_MAC_LENGTH
 * when called with the validated \p algorithm and \p key_type is well-defined.
 *
 * \param[in] algorithm     The specific MAC algorithm (can be wildcard).
 * \param[in] key_type      The key type of the key to be used with the
 *                          \p algorithm.
 *
 * \retval #PSA_SUCCESS
 *         The \p key_type is valid for use with the \p algorithm
 * \retval #PSA_ERROR_INVALID_ARGUMENT
 *         The \p key_type is not valid for use with the \p algorithm
 */
static psa_status_t psa_mac_key_can_do(psa_algorithm_t algorithm, psa_key_type_t key_type)
{
    psa_status_t status = PSA_ERROR_INVALID_ARGUMENT;

    if (PSA_ALG_IS_HMAC(algorithm))
    {
        if (key_type == PSA_KEY_TYPE_HMAC)
        {
            return PSA_SUCCESS;
        }
    }
    else if (PSA_ALG_IS_MAC(algorithm))
    {
        /* Our device only supports AES-MAC */
        if (key_type == PSA_KEY_TYPE_AES)
        {
            status = PSA_SUCCESS;
        }
    }

    return status;
}

/*
 *  ======== psa_key_algorithm_permits ========
 */
static int psa_key_algorithm_permits(psa_key_type_t key_type, psa_algorithm_t policy_alg, psa_algorithm_t requested_alg)
{
    /* If it isn't explicitly permitted, it's forbidden */
    int permits = 0;

    /* Common case: the policy only allows requested_alg */
    if (requested_alg == policy_alg)
    {
        permits = 1;
    }
    /* If policy_alg is a hash-and-sign with a wildcard for the hash, and
     * requested_alg is the same hash-and-sign family with any hash, then
     * requested_alg is compliant with policy_alg.
     */
    else if (PSA_ALG_IS_HASH_AND_SIGN(requested_alg) && PSA_ALG_SIGN_GET_HASH(policy_alg) == PSA_ALG_ANY_HASH)
    {
        permits = ((policy_alg & ~PSA_ALG_HASH_MASK) == (requested_alg & ~PSA_ALG_HASH_MASK));
    }
    /* If policy_alg is a wildcard AEAD algorithm of the same base as the
     * requested algorithm, check the requested tag length to be equal-length or
     * longer than the wildcard-specified length.
     */
    else if (PSA_ALG_IS_AEAD(policy_alg) && PSA_ALG_IS_AEAD(requested_alg) &&
             (PSA_ALG_AEAD_WITH_SHORTENED_TAG(policy_alg, 0) == PSA_ALG_AEAD_WITH_SHORTENED_TAG(requested_alg, 0)) &&
             ((policy_alg & PSA_ALG_AEAD_AT_LEAST_THIS_LENGTH_FLAG) != 0))
    {
        permits = (PSA_ALG_AEAD_GET_TAG_LENGTH(policy_alg) <= PSA_ALG_AEAD_GET_TAG_LENGTH(requested_alg));
    }
    /* If policy_alg is a MAC algorithm of the same base as the requested
     * algorithm, check whether their MAC lengths are compatible.
     */
    else if (PSA_ALG_IS_MAC(policy_alg) && PSA_ALG_IS_MAC(requested_alg) &&
             (PSA_ALG_FULL_LENGTH_MAC(policy_alg) == PSA_ALG_FULL_LENGTH_MAC(requested_alg)))
    {
        /* Validate the combination of key type and algorithm. Since the policy
         * and requested algorithms are the same, we only need this once.
         */
        if (PSA_SUCCESS != psa_mac_key_can_do(policy_alg, key_type))
        {
            permits = 1;
        }
        else
        {
            /* Get both the requested output length for the algorithm which is
             * to be verified, and the default output length for the base
             * algorithm. Note that none of the currently supported algorithms
             * have an output length dependent on actual key size, so setting it
             * to a bogus value of 0 is currently OK.
             */
            size_t requested_output_length = PSA_MAC_LENGTH(key_type, 0, requested_alg);
            size_t default_output_length   = PSA_MAC_LENGTH(key_type, 0, PSA_ALG_FULL_LENGTH_MAC(requested_alg));

            /* If the policy is default-length, only allow an algorithm with
             * a declared exact-length matching the default.
             */
            if (PSA_MAC_TRUNCATED_LENGTH(policy_alg) == 0)
            {
                permits = (requested_output_length == default_output_length);
            }
            /* If the requested algorithm is default-length, allow it if the
             * policy length exactly matches the default length.
             */
            else if (PSA_MAC_TRUNCATED_LENGTH(requested_alg) == 0 &&
                     PSA_MAC_TRUNCATED_LENGTH(policy_alg) == default_output_length)
            {
                permits = (1);
            }
            /* If policy_alg is an at-least-this-length wildcard MAC algorithm,
             * check for the requested MAC length to be equal to or longer than
             * the minimum allowed length.
             */
            else if ((policy_alg & PSA_ALG_MAC_AT_LEAST_THIS_LENGTH_FLAG) != 0)
            {
                permits = (PSA_MAC_TRUNCATED_LENGTH(policy_alg) <= requested_output_length);
            }
        }
    }
    /* If policy_alg is a generic key agreement operation, then using it for
     * a key derivation with that key agreement should also be allowed. This
     * behaviour is expected to be defined in a future specification version.
     */
    else if (PSA_ALG_IS_RAW_KEY_AGREEMENT(policy_alg) && PSA_ALG_IS_KEY_AGREEMENT(requested_alg))
    {
        permits = (PSA_ALG_KEY_AGREEMENT_GET_BASE(requested_alg) == policy_alg);
    }

    return permits;
}

/** Test whether a policy permits an algorithm.
 *
 * The caller must test usage flags separately.
 *
 * \note This function requires providing the key type for which the policy is
 *       being validated, since some algorithm policy definitions (e.g. MAC)
 *       have different properties depending on what kind of cipher it is
 *       combined with.
 *
 * \retval PSA_SUCCESS                  When \p alg is a specific algorithm
 *                                      allowed by the \p policy.
 * \retval PSA_ERROR_INVALID_ARGUMENT   When \p alg is not a specific algorithm
 * \retval PSA_ERROR_NOT_PERMITTED      When \p alg is a specific algorithm, but
 *                                      the \p policy does not allow it.
 */
static psa_status_t psa_key_policy_permits(const psa_key_policy_t *policy, psa_key_type_t key_type, psa_algorithm_t alg)
{
    psa_status_t status;

    /* '0' is not a valid algorithm */
    if (alg == 0)
    {
        status = PSA_ERROR_INVALID_ARGUMENT;
    }
    /* A requested algorithm cannot be a wildcard */
    else if (PSA_ALG_IS_WILDCARD(alg))
    {
        status = PSA_ERROR_INVALID_ARGUMENT;
    }
    else if (psa_key_algorithm_permits(key_type, policy->alg, alg) ||
             psa_key_algorithm_permits(key_type, policy->alg2, alg))
    {
        status = PSA_SUCCESS;
    }
    else
    {
        status = PSA_ERROR_NOT_PERMITTED;
    }

    return status;
}

/** Check whether a set of key attributes allows the given usage policy and algorithm
 *
 * The key must have allow all the usage flags set in \p usage. If \p alg is
 * nonzero, the key must allow operations with this algorithm. If \p alg is
 * zero, the algorithm is not checked.
 */
static psa_status_t psa_key_attributes_usage_check(psa_key_attributes_t *attributes,
                                                   psa_key_usage_t usage,
                                                   psa_algorithm_t alg)
{
    psa_status_t status       = PSA_SUCCESS;
    psa_key_type_t key_type   = psa_get_key_type(attributes);
    psa_key_usage_t key_usage = psa_get_key_usage_flags(attributes);

#if (TFM_ENABLED == 0) || defined(TFM_BUILD)
    psa_key_policy_t key_policy = attributes->core.policy;
#else
    psa_key_policy_t key_policy = {.usage = key_usage, .alg = attributes->alg};
#endif

    /* Enforce that usage policy for the key contains all the flags required by
     * the usage parameter. There is one exception: public keys can always be
     * exported, so we treat public key objects as if they had the export flag.
     */
    if (PSA_KEY_TYPE_IS_PUBLIC_KEY(key_type))
    {
        usage &= ~PSA_KEY_USAGE_EXPORT;
    }

    if ((key_usage & usage) != usage)
    {
        status = PSA_ERROR_NOT_PERMITTED;
    }
    /* Enforce that the usage policy permits the requested algorithm */
    else if (alg != 0)
    {
        status = psa_key_policy_permits(&key_policy, key_type, alg);
    }

    return status;
}

#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))

/*
 *  ======== psa_destroy_key ========
 */
psa_status_t psa_destroy_key(psa_key_id_t key)
{
    psa_status_t status = PSA_ERROR_GENERIC_ERROR;

    if (HSMXXF3_acquireLock(SemaphoreP_WAIT_FOREVER, (uintptr_t)NULL))
    {
        if (KeyStore_acquireLock())
        {
            status = KeyMgmt_psa_destroy_key(toKeyStoreKeyID(key));
            KeyStore_releaseLock();
        }

        HSMXXF3_releaseLock();
    }

    return status;
}

/*
 *  ======== psa_purge_key ========
 */
psa_status_t psa_purge_key(psa_key_id_t key)
{
    psa_status_t status = PSA_ERROR_GENERIC_ERROR;

    /* No HSM lock is required as no tokens will be submitted */

    if (KeyStore_acquireLock())
    {
        status = KeyMgmt_psa_purge_key(toKeyStoreKeyID(key));
        KeyStore_releaseLock();
    }

    return status;
}

/*
 *  ======== psa_get_key_attributes ========
 *  Retrieve full key attributes. If built into the TFM, the crypto secure
 *  service is responsible for translating the full key attributes to client key
 *  attributes.
 */
psa_status_t psa_get_key_attributes(psa_key_id_t key, psa_key_attributes_t *attributes)
{
    psa_algorithm_t originalAlgorithm;
    psa_status_t status = PSA_ERROR_GENERIC_ERROR;

    /* No HSM lock is required as no tokens will be submitted */

    if (KeyStore_acquireLock())
    {
        status = KeyMgmt_psa_get_key_attributes(toKeyStoreKeyID(key), attributes);

        KeyStore_releaseLock();
    }

    if (status == PSA_SUCCESS)
    {
        /* Retrieve the algorithm set by the application, before the algorithm was
         * mapped to the value needed by the drivers.
         */
        originalAlgorithm = attributes->core.policy.alg2;

        psa_set_key_algorithm(attributes, originalAlgorithm);
    }

    return status;
}

/*
 *  ======== psa_reset_key_attributes ========
 *  Reset full key attributes. If built into the TFM, the crypto secure
 *  service is responsible for:
 *  1) Translating client key attributes to the full key attributes before
 *     calling this function
 *  2) Translating the full key attributes to client key attributes after calling
 *     this function.
 */
void psa_reset_key_attributes(psa_key_attributes_t *attributes)
{
    /* Acquiring the HSM and keystore lock is not required as this function does
     * not access the key store.
     */
    KeyMgmt_psa_reset_key_attributes(attributes);
}

/*
 *  ======== psa_export_key ========
 */
psa_status_t psa_export_key(psa_key_id_t key, uint8_t *data, size_t data_size, size_t *data_length)
{
    psa_status_t status = PSA_ERROR_GENERIC_ERROR;

    /* No HSM lock is required as no tokens will be submitted */

    if (KeyStore_acquireLock())
    {
        status = KeyMgmt_psa_export_key(toKeyStoreKeyID(key), data, data_size, data_length);

        KeyStore_releaseLock();
    }

    return status;
}

/*
 *  ======== psa_export_public_key ========
 */
psa_status_t psa_export_public_key(psa_key_id_t key, uint8_t *data, size_t data_size, size_t *data_length)
{
    psa_status_t status = PSA_ERROR_GENERIC_ERROR;

    if (!HSMXXF3_acquireLock(SemaphoreP_WAIT_FOREVER, (uintptr_t)NULL))
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (KeyStore_acquireLock())
    {
        status = KeyMgmt_psa_export_public_key(toKeyStoreKeyID(key), data, data_size, data_length);
        KeyStore_releaseLock();
    }

    HSMXXF3_releaseLock();

    return status;
}

/*
 *  ======== psa_copy_attr_and_modify_alg ========
 */
static psa_status_t psa_copy_attr_and_modify_alg(psa_key_attributes_t *dest,
                                                 const psa_key_attributes_t *src,
                                                 size_t len)
{
    psa_algorithm_t originalAlg = psa_get_key_algorithm(src);

    (void)memcpy(dest, src, len);

    if (PSA_ALG_IS_AEAD(originalAlg))
    {
        /* Distinct algorithms for AEAD with non-standard tag sizes are not
         * supported as the SL Crypto driver for CCM and GCM retrieve keystore
         * keys using the standard PSA_ALG_CCM or PSA_ALG_GCM algorithm.
         */
        psa_algorithm_t aeadAlg = PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(originalAlg);

        if ((aeadAlg == PSA_ALG_CCM) || (aeadAlg == PSA_ALG_GCM))
        {
            psa_set_key_algorithm(dest, aeadAlg);
        }
        else
        {
            return PSA_ERROR_NOT_SUPPORTED;
        }
    }
    else if (PSA_ALG_IS_BLOCK_CIPHER_MAC(originalAlg))
    {
        /* Distinct algorithms for MAC with non-standard tag sizes are not
         * supported as the SL Crypto driver for CMAC/CBC-MAC retrieve keystore
         * keys using the standard PSA_ALG_CMAC or PSA_ALG_CBC_MAC algorithm.
         */
        psa_algorithm_t macAlg = PSA_ALG_FULL_LENGTH_MAC(originalAlg);

        if ((macAlg == PSA_ALG_CMAC) || (macAlg == PSA_ALG_CBC_MAC))
        {
            psa_set_key_algorithm(dest, macAlg);
        }
        else
        {
            return PSA_ERROR_NOT_SUPPORTED;
        }
    }

    /* Store the original algorithm value so that it can be restored by
     * psa_get_key_attributes.
     */
    dest->core.policy.alg2 = originalAlg;

    return PSA_SUCCESS;
}

/*
 *  ======== psa_import_key ========
 *  This function requires full key attributes. If built into the TFM, the
 *  crypto secure service is responsible for translating client key attributes
 *  to the full key attributes before calling this function.
 */
psa_status_t psa_import_key(const psa_key_attributes_t *attributes,
                            const uint8_t *data,
                            size_t data_length,
                            psa_key_id_t *key)
{
    mbedtls_svc_key_id_t keyStoreKeyID = toKeyStoreKeyID(*key);
    psa_key_attributes_t attributesCopy;
    psa_status_t status;

    status = psa_copy_attr_and_modify_alg(&attributesCopy, attributes, sizeof(attributesCopy));

    if (status == PSA_SUCCESS)
    {
        if (HSMXXF3_acquireLock(SemaphoreP_WAIT_FOREVER, (uintptr_t)NULL))
        {
            if (KeyStore_acquireLock())
            {
                status = KeyMgmt_psa_import_key(&attributesCopy, data, data_length, &keyStoreKeyID);
                *key   = toKeyID(keyStoreKeyID);

                KeyStore_releaseLock();
            }
            else
            {
                status = PSA_ERROR_GENERIC_ERROR;
            }

            HSMXXF3_releaseLock();
        }
        else
        {
            status = PSA_ERROR_GENERIC_ERROR;
        }
    }

    if (status != PSA_SUCCESS)
    {
        *key = PSA_KEY_ID_NULL;
    }

    return status;
}

/*
 *  ======== psa_copy_key ========
 *  This function requires full key attributes. If built into the TFM, the
 *  crypto secure service is responsible for translating client key attributes
 *  to the full key attributes before calling this function.
 */
psa_status_t psa_copy_key(psa_key_id_t source_key, const psa_key_attributes_t *attributes, psa_key_id_t *target_key)
{
    mbedtls_svc_key_id_t targetKeyStoreKeyID = toKeyStoreKeyID(*target_key);
    psa_key_attributes_t attributesCopy;
    psa_status_t status;

    status = psa_copy_attr_and_modify_alg(&attributesCopy, attributes, sizeof(attributesCopy));

    if (status == PSA_SUCCESS)
    {
        if (HSMXXF3_acquireLock(SemaphoreP_WAIT_FOREVER, (uintptr_t)NULL))
        {
            if (KeyStore_acquireLock())
            {
                status      = KeyMgmt_psa_copy_key(toKeyStoreKeyID(source_key), &attributesCopy, &targetKeyStoreKeyID);
                /* Set the caller's target key ID */
                *target_key = toKeyID(targetKeyStoreKeyID);

                KeyStore_releaseLock();
            }
            else
            {
                status = PSA_ERROR_GENERIC_ERROR;
            }

            HSMXXF3_releaseLock();
        }
        else
        {
            status = PSA_ERROR_GENERIC_ERROR;
        }
    }

    if (status != PSA_SUCCESS)
    {
        *target_key = PSA_KEY_ID_NULL;
    }

    return status;
}

/*
 *  ======== psa_can_do_hash ========
 *  Helper function implemented by mbedTLS that must be implemented in our PSA Crypto library
 */
int psa_can_do_hash(psa_algorithm_t hash_alg)
{
    (void)hash_alg;
    return 1;
}

/*
 *  ======== psa_can_do_cipher ========
 *  Helper function implemented by mbedTLS that must be implemented in our PSA Crypto library
 */
int psa_can_do_cipher(psa_key_type_t key_type, psa_algorithm_t cipher_alg)
{
    (void)key_type;
    (void)cipher_alg;
    return 1;
}

#elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)

/*
 *  ======== psa_destroy_key ========
 */
psa_status_t psa_destroy_key(psa_key_id_t key)
{
    #if (TFM_ENABLED == 1)
    /* Get index of element to remove */
    int8_t toRemove = -1;
    for (uint8_t i = 0; i < keyAlgorithmsLength; i++)
    {
        if (keyAlgorithms[i].keyID == key)
        {
            toRemove = i;
            break;
        }
    }

    if (toRemove >= 0)
    {
        /* Remove element by shifting everything to the left */
        for (uint8_t i = toRemove; i < keyAlgorithmsLength - 1; i++)
        {
            keyAlgorithms[i] = keyAlgorithms[i + 1];
        }

        keyAlgorithmsLength--;
    }
    #endif

    return KeyStore_PSA_destroyKey(toKeyStoreKeyID(key));
}

/*
 *  ======== psa_purge_key ========
 */
psa_status_t psa_purge_key(psa_key_id_t key)
{

    psa_status_t status             = PSA_SUCCESS;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_get_key_attributes(key, &attributes);
    psa_key_lifetime_t lifetime = psa_get_key_lifetime(&attributes);

    /* Nothing to be done for persistent keys */
    if (lifetime != PSA_KEY_PERSISTENCE_VOLATILE)
    {
        status = KeyStore_PSA_purgeKey(toKeyStoreKeyID(key));
    }

    psa_reset_key_attributes(&attributes);

    return status;
}

/*
 *  ======== psa_get_key_attributes ========
 *  Retrieve all the publicly-accessible attributes of a key.
 */
psa_status_t psa_get_key_attributes(psa_key_id_t key, psa_key_attributes_t *attributes)
{
    *attributes         = PSA_KEY_ATTRIBUTES_INIT;
    psa_status_t status = KeyStore_PSA_getKeyAttributes(toKeyStoreKeyID(key), attributes);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Retrieve the algorithm set by the application, before the algorithm was
     * mapped to the value needed by the drivers.
     */
    psa_algorithm_t originalAlgorithm = 0;
    #if (TFM_ENABLED == 0)
    originalAlgorithm                 = attributes->core.policy.alg2;
    #else
    for (uint8_t i = 0; i < keyAlgorithmsLength; i++)
    {
        if (keyAlgorithms[i].keyID == key)
        {
            originalAlgorithm = keyAlgorithms[i].alg;
            break;
        }
    }

    if (originalAlgorithm == 0)
    {
        return PSA_ERROR_INVALID_HANDLE;
    }
    #endif

    psa_set_key_algorithm(attributes, originalAlgorithm);
    return PSA_SUCCESS;
}

/*
 *  ======== psa_reset_key_attributes ========
 */
void psa_reset_key_attributes(psa_key_attributes_t *attributes)
{
    #if (TFM_ENABLED == 1)
    (void)memset(attributes, 0, sizeof(*attributes));
    #else
    KeyStore_PSA_resetKeyAttributes(attributes);
    #endif
}

/*
 *  ======== psa_export_key ========
 */
psa_status_t psa_export_key(psa_key_id_t key, uint8_t *data, size_t data_size, size_t *data_length)
{
    return KeyStore_PSA_exportKey(toKeyStoreKeyID(key), data, data_size, data_length);
}

/*
 *  ======== psa_export_public_key ========
 */
psa_status_t psa_export_public_key(psa_key_id_t key, uint8_t *data, size_t data_size, size_t *data_length)
{
    ECDH_OperationGeneratePublicKey generatePublicKeyOp;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_status_t status;

    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    psa_key_type_t keyType = psa_get_key_type(&attributes);

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    /* KeyStore_PSA does not support exporting the public part of a key pair, so
     * in that case generate the public part using ECDH_generatePublicKey.
     */
    if (PSA_KEY_TYPE_IS_ECC_KEY_PAIR(keyType))
    {
    #ifdef ENABLE_TI_CRYPTO_ECDH
        int_fast16_t ret;
        size_t keyBits  = psa_get_key_bits(&attributes);
        size_t keyBytes = PSA_BITS_TO_BYTES(keyBits);
        size_t publicKeyBytes;
        ECDH_KeyMaterialEndianness endianess;

        if (ecdhHandle == NULL)
        {
            return PSA_ERROR_BAD_STATE;
        }

        /* For most families key size is twice the private key size + 1 */
        psa_ecc_family_t keyFamily = PSA_KEY_TYPE_ECC_GET_FAMILY(keyType);
        if (keyFamily == PSA_ECC_FAMILY_MONTGOMERY)
        {
            publicKeyBytes = keyBytes;
            endianess      = ECDH_LITTLE_ENDIAN_KEY;
        }
        else
        {
            publicKeyBytes = keyBytes * 2 + 1;
            endianess      = ECDH_BIG_ENDIAN_KEY;
        }

        if (publicKeyBytes > data_size)
        {
            return PSA_ERROR_BUFFER_TOO_SMALL;
        }

        /* Init cryptoKeys for use in the GeneratePublicKey operation */
        CryptoKeyPlaintext_initBlankKey(&publicKey, data, publicKeyBytes);
        KeyStore_PSA_initKey(&privateKey, toKeyStoreKeyID(key), keyBytes, NULL);

        /* Init the operation */
        ECDH_OperationGeneratePublicKey_init(&generatePublicKeyOp);
        generatePublicKeyOp.myPrivateKey = &privateKey;
        generatePublicKeyOp.myPublicKey  = &publicKey;
        /* Get the correct parameters, some curves are not supported */
        generatePublicKeyOp.curve        = map_keyTypeToECCParams(keyType, keyBits);

        if (generatePublicKeyOp.curve == NULL)
        {
            return PSA_ERROR_NOT_SUPPORTED;
        }

        generatePublicKeyOp.keyMaterialEndianness = endianess;

        ret    = ECDH_generatePublicKey(ecdhHandle, &generatePublicKeyOp);
        status = map_ECDH_status(ret);

        if (status != PSA_SUCCESS)
        {
            *data_length = 0;
        }
        else
        {
            *data_length = publicKeyBytes;
        }

    #endif /* ENABLE_TI_CRYPTO_ECDH */
    }
    else
    {
        /* KeyStore_PSA_exportPublicKey does not return until the export is complete */
        status = KeyStore_PSA_exportPublicKey(toKeyStoreKeyID(key), data, data_size, data_length);
    }

    return status;
}

/*
 *  ======== psa_import_key ========
 */
psa_status_t psa_import_key(const psa_key_attributes_t *attributes,
                            const uint8_t *data,
                            size_t data_length,
                            psa_key_id_t *key)
{
    psa_status_t status;
    psa_key_attributes_t attributesCopy;
    psa_key_type_t psaKeyType;
    psa_algorithm_t originalAlgorithm;
    mbedtls_svc_key_id_t keyStoreKeyID;

    attributesCopy    = *attributes;
    psaKeyType        = psa_get_key_type(attributes);
    originalAlgorithm = psa_get_key_algorithm(attributes);
    keyStoreKeyID     = toKeyStoreKeyID(*key);

    /* SL KeyStore driver does not support distinct algorithms for AEAD with
     * different tag sizes The SL Crypto driver for CCM and GCM can only accept
     * the standard PSA_ALG_CCM or PSA_ALG_GCM algorithm.
     */
    if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(originalAlgorithm) == PSA_ALG_CCM)
    {
        psa_set_key_algorithm(&attributesCopy, PSA_ALG_CCM);
    }
    if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(originalAlgorithm) == PSA_ALG_GCM)
    {
        psa_set_key_algorithm(&attributesCopy, PSA_ALG_GCM);
    }
    psa_set_key_type(&attributesCopy, psaKeyType);

    #if (TFM_ENABLED == 0)
    /* Store the original algorithm value so that it can be restored by
     * psa_get_key_attributes.
     */
    attributesCopy.core.policy.alg2 = originalAlgorithm;
    #endif

    status = KeyStore_PSA_importKey(&attributesCopy, (uint8_t *)data, data_length, &keyStoreKeyID);
    *key   = toKeyID(keyStoreKeyID);

    if (status != KEYSTORE_PSA_STATUS_SUCCESS)
    {
        KeyStore_PSA_destroyKey(keyStoreKeyID);
        return status;
    }

    #if (TFM_ENABLED == 1)
    /* With TFM_ENABLED=1, key attributes struct only has one alg slot,
     * so store original algorithms in an array of length
     * TFM_ENABLED_KEY_STORAGE_LIMIT (TFM_ENABLED_KEY_STORAGE_LIMIT is the
     * max number of keys that can be stored at any one time)
     */
    if (keyAlgorithmsLength < TFM_ENABLED_KEY_STORAGE_LIMIT)
    {
        keyAlgorithms[keyAlgorithmsLength].keyID = *key;
        keyAlgorithms[keyAlgorithmsLength].alg   = originalAlgorithm;
        keyAlgorithmsLength++;
    }
    else
    {
        KeyStore_PSA_destroyKey(keyStoreKeyID);
        status = PSA_ERROR_NOT_SUPPORTED;
    }
    #endif

    return status;
}

/*
 *  ======== psa_copy_key ========
 */
psa_status_t psa_copy_key(psa_key_id_t source_key, const psa_key_attributes_t *attributes, psa_key_id_t *target_key)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

#else

    #error "Key Management functions not supported for this device"

#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC235X) \
        */

/******************************************************************************/
/* Message digests */
/******************************************************************************/

/* Hash suspend/resume cannot be supported if TFM is enabled and PSA Crypto
 * Wrapper is not built into TFM since the internal driver structs are not
 * accessible to save/restore the hash state.
 *
 * The CC27xx SHA2 driver may buffer an entire block of data, thus cannot
 * support hash suspend/resume which limits the amount of unprocessed input
 * to (block_size - 1) bytes.
 */
#if ((TFM_ENABLED == 1) && !defined(TFM_BUILD))

/*
 *  ======== psa_hash_resume ========
 */
psa_status_t psa_hash_resume(psa_hash_operation_t *operation, const uint8_t *hash_state, size_t hash_state_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

/*
 *  ======== psa_hash_suspend ========
 */
psa_status_t psa_hash_suspend(psa_hash_operation_t *operation,
                              uint8_t *hash_state,
                              size_t hash_state_size,
                              size_t *hash_state_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

#else

/*
 * addIntToCharArrayBigEndian, charArrayBigEndianToInt
 * Helper functions for psa_hash_suspend and psa_hash_resume
 */

    #ifdef ENABLE_TI_CRYPTO_SHA2
/*
 *  ======== addIntToCharArrayBigEndian ========
 *  Adds a 'numBytes' byte value to a uint8_t array in big endian format.
 *  The first (numBytes - 4) of the array are zeroed out and the 'value' is
 *  written to the last 4-bytes of the array. 'numBytes' must be >= 4.
 */
static void addIntToCharArrayBigEndian(uint32_t value, uint8_t numBytes, uint8_t *array)
{
    uint_fast8_t arrayIndex;
    uint8_t byte;
    uint8_t rightShiftAmount = 24U;

    /* Zero the leading bytes since only the last 4-bytes holds the 'value' */
    (void)memset(array, 0U, (numBytes - 4U));

    /* Write the 32-bit 'value' in big endian format */
    for (arrayIndex = (numBytes - 4U); arrayIndex < numBytes; arrayIndex++)
    {
        byte              = (value >> rightShiftAmount) & 0xFF;
        array[arrayIndex] = byte;
        rightShiftAmount -= 8U;
    }
}

/*
 *  ======== charArrayBigEndianToInt ========
 *  Extracts a uint32_t value from a uint8_t array in big endian format.
 */
static uint32_t charArrayBigEndianToInt(const uint8_t *array)
{
    uint_fast8_t i;
    uint32_t value          = 0U;
    uint8_t leftShiftAmount = 24U;

    /* Extract the 32-bit 'value' from big endian byte array */
    for (i = 0U; i < sizeof(uint32_t); i++)
    {
        value |= array[i] << leftShiftAmount;
        leftShiftAmount -= 8U;
    }

    return value;
}

    #endif /* ENABLE_TI_CRYPTO_SHA2 */

/*
 *  ======== psa_hash_resume ========
 */
psa_status_t psa_hash_resume(psa_hash_operation_t *operation, const uint8_t *hash_state, size_t hash_state_length)
{
    #ifndef ENABLE_TI_CRYPTO_SHA2
    return PSA_ERROR_NOT_SUPPORTED;
    #else

        #if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
            (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)
    SHA2CC26X2_Object *object = (SHA2CC26X2_Object *)sha2Handle->object;
        #elif ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || \
               (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    SHA2XXF3HSM_Object *object = (SHA2XXF3HSM_Object *)sha2Handle->object;
        #else
            #error "Device family not supported"
        #endif
    psa_algorithm_t alg;
    psa_status_t status;
    uint_fast8_t i;
    uint32_t inputLength;
    uint32_t leadingZero;
    uint8_t blockSize;
    uint8_t digestBufferLength;
    uint8_t inputLengthNumBytes;

    /* hash_state = alg || input_length || hash_state || unprocessed_input */

    alg = charArrayBigEndianToInt(hash_state);
    hash_state += 4;

    blockSize           = PSA_HASH_BLOCK_LENGTH(alg);
    digestBufferLength  = PSA_HASH_SUSPEND_HASH_STATE_FIELD_LENGTH(alg);
    inputLengthNumBytes = PSA_HASH_SUSPEND_INPUT_LENGTH_FIELD_LENGTH(alg);

    /* Operation must be in an inactive state */
    if ((operation->id != 0) || (operation->alg != 0))
    {
        return PSA_ERROR_BAD_STATE;
    }

    /* SL drivers only support SHA2 */
    if (!PSA_ALG_IS_SHA2(alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Check that the input length does not require more than 4 bytes to store.
     * 'inputLengthNumBytes' is either 8 or 16 but SHA2 driver stores input length
     * as a 4-byte integer. Integer lengths that require more than 4 bytes are
     * not supported.
     */
    for (i = 0U; i < (inputLengthNumBytes - 4U); i += 4U)
    {
        leadingZero = charArrayBigEndianToInt(hash_state);
        hash_state += 4;

        if (leadingZero != 0U)
        {
            return PSA_ERROR_NOT_SUPPORTED;
        }
    }

    inputLength = charArrayBigEndianToInt(hash_state);
    hash_state += 4;

    /* Restore the operation struct contents */
    status = psa_hash_setup(operation, alg);

    if (status == PSA_SUCCESS)
    {
        object->bytesProcessed = (inputLength / blockSize) * blockSize;

        /* Copy hash state to SHA2 driver object */
        (void)memcpy(object->digest, hash_state, digestBufferLength);
        hash_state += digestBufferLength;

        /* Copy any unprocessed data to SHA2 driver object */
        (void)memcpy(object->buffer, hash_state, blockSize);

        /* Set object->bytesInBuffer to any remaining partial blocks of the
         * inputLength (total number of bytes input to the hash).
         */
        object->bytesInBuffer = inputLength % blockSize;
    }

    return status;
    #endif /* ENABLE_TI_CRYPTO_SHA2 */
}

/*
 *  ======== psa_hash_suspend ========
 */
psa_status_t psa_hash_suspend(psa_hash_operation_t *operation,
                              uint8_t *hash_state,
                              size_t hash_state_size,
                              size_t *hash_state_length)
{
    #ifndef ENABLE_TI_CRYPTO_SHA2
    return PSA_ERROR_NOT_SUPPORTED;
    #else

        #if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
            (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)
    SHA2CC26X2_Object *object   = (SHA2CC26X2_Object *)sha2Handle->object;
        #elif ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || \
               (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    SHA2XXF3HSM_Object *object = (SHA2XXF3HSM_Object *)sha2Handle->object;
        #else
            #error "Device family not supported"
        #endif
    psa_algorithm_t alg         = operation->alg;
    uint8_t inputLengthNumBytes = PSA_HASH_SUSPEND_INPUT_LENGTH_FIELD_LENGTH(alg);
    uint8_t hashStateNumBytes   = PSA_HASH_SUSPEND_HASH_STATE_FIELD_LENGTH(alg);

    /* Operation must be in an active state */
    if ((operation->id == 0U) || (operation->alg == 0))
    {
        return PSA_ERROR_BAD_STATE;
    }

        /* The CC27xx SHA2 driver may buffer an entire block of data, thus cannot
         * support hash suspend/resume which limits the amount of unprocessed input
         * to (block_size - 1) bytes.
         */
        #if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    if (object->bytesInBuffer == PSA_HASH_BLOCK_LENGTH(alg))
    {
        if (sha2Handle == NULL)
        {
            return PSA_ERROR_BAD_STATE;
        }

        /* Finalize to cleanup the operation */
        (void)SHA2_finalize(sha2Handle, NULL);
        return PSA_ERROR_NOT_SUPPORTED;
    }
        #endif

    /* Buffer must be large enough to fit all the data */
    *hash_state_length = 4 + inputLengthNumBytes + hashStateNumBytes + object->bytesInBuffer;
    if (hash_state_size < *hash_state_length)
    {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    /* SHA3 does not support neither suspend nor resume */
    if ((alg == PSA_ALG_SHA3_224) || (alg == PSA_ALG_SHA3_256) || (alg == PSA_ALG_SHA3_384) ||
        (alg == PSA_ALG_SHA3_512))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Per the PSA spec, hash suspend state has the following format:
     * hash_suspend_state = algorithm || input_length || hash_state || unprocessed_input
     */

    /* Add algorithm identifier (32 bit integer value) in big endian format */
    addIntToCharArrayBigEndian(operation->alg, 4, hash_state);
    hash_state += 4;

    uint32_t inputLength = object->bytesProcessed + object->bytesInBuffer;

    /* Add input length in big endian format. For all hash algorithms other than
     * MD2, this is the total number of bytes of input to the hash computation
     * including the unprocessed bytes.
     */
    addIntToCharArrayBigEndian(inputLength, inputLengthNumBytes, hash_state);
    hash_state += inputLengthNumBytes;

    /* Add the current hash state (i.e. hash digest):
     * - For SHA-224 and SHA-256: 8x 32-bit integers, in big-endian encoding.
     * - For SHA-512, SHA-384 and SHA-512/256: 8x 64-bit integers, in big-endian encoding.
     */
    (void)memcpy(hash_state, object->digest, hashStateNumBytes);
    hash_state += hashStateNumBytes;

    /* Add the unprocessed input data. Must be between 0 and (block_size - 1) bytes */
    (void)memcpy(hash_state, object->buffer, object->bytesInBuffer);

    /* Operation should return to an inactive state */
    return psa_hash_abort(operation);
    #endif /* ENABLE_TI_CRYPTO_SHA2 */
}

#endif /* ((TFM_ENABLED == 1) && !defined(TFM_BUILD)) */

/*
 *  ======== psa_hash_abort ========
 */
psa_status_t psa_hash_abort(psa_hash_operation_t *operation)
{
#ifndef ENABLE_TI_CRYPTO_SHA2
    return PSA_ERROR_NOT_SUPPORTED;
#else
    /* Aborting a non-active operation is allowed */
    if (operation->alg == 0)
    {
        return PSA_SUCCESS;
    }

    if (sha2Handle == NULL)
    {
        return PSA_ERROR_BAD_STATE;
    }

    /* Call reset to clear the internal buffers for SHA2 and any intermediate
     * digests.
     */
    SHA2_reset(sha2Handle);
    *operation = PSA_HASH_OPERATION_INIT;

    return PSA_SUCCESS;
#endif /* ENABLE_TI_CRYPTO_SHA2 */
}

/*
 *  ======== psa_hash_setup ========
 */
psa_status_t psa_hash_setup(psa_hash_operation_t *operation, psa_algorithm_t alg)
{
#ifndef ENABLE_TI_CRYPTO_SHA2
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

    /* Except for first call to this fuction, this function cannot be called
     * without calling abort psa_hash_abort operation, which resets the internal
     * buffers for SHA2.
     */

    /* Return this error to prevent out of order execution */
    if (operation->id != 0)
    {
        return PSA_ERROR_BAD_STATE;
    }

    /* There should be no alg in this object yet */
    if (operation->alg != 0)
    {
        return PSA_ERROR_BAD_STATE;
    }

    /* This call will handle checking sha2Handle and supported alg */
    status = psa_set_sha2_hash_type(alg);

    if (status == PSA_SUCCESS)
    {
        operation->alg = alg;
        operation->id  = PSA_CRYPTO_TI_DRIVER;
    }

    return status;
#endif /* ENABLE_TI_CRYPTO_SHA2 */
}

/*
 *  ======== psa_hash_update ========
 */
psa_status_t psa_hash_update(psa_hash_operation_t *operation, const uint8_t *input, size_t input_length)
{
#ifndef ENABLE_TI_CRYPTO_SHA2
    return PSA_ERROR_NOT_SUPPORTED;
#else
    int_fast16_t ret;
    psa_status_t status;

    /* Return this error to prevent out of order execution */
    if (operation->id == 0U)
    {
        return PSA_ERROR_BAD_STATE;
    }

    /* Don't require hash implementations to behave correctly on a zero-length
     * input which may have an invalid pointer.
     */
    if (input_length == 0)
    {
    #ifdef TFM_BUILD
        return PSA_OPERATION_COMPLETE;
    #else
        return PSA_SUCCESS;
    #endif
    }

    /* Note: Replacing this with PSA_ALG_IS_HASH(operation->alg) is not
     * sufficient since we only support SHA2.
     */
    if (!PSA_ALG_IS_SHA2(operation->alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    if (sha2Handle == NULL)
    {
        return PSA_ERROR_BAD_STATE;
    }

    ret    = SHA2_addData(sha2Handle, input, input_length);
    status = map_SHA2_status(ret);

    return status;
#endif /* ENABLE_TI_CRYPTO_SHA2 */
}

/*
 *  ======== psa_hash_finish ========
 */
psa_status_t psa_hash_finish(psa_hash_operation_t *operation, uint8_t *hash, size_t hash_size, size_t *hash_length)
{
#ifndef ENABLE_TI_CRYPTO_SHA2
    return PSA_ERROR_NOT_SUPPORTED;
#else
    int_fast16_t ret;
    psa_status_t status       = PSA_SUCCESS;
    size_t actual_hash_length = PSA_HASH_LENGTH(operation->alg);
    /* Fill the output buffer with something that isn't a valid hash (barring an
     * attack on the hash and deliberately-crafted input), in case the caller
     * doesn't check the return status properly.
     */
    *hash_length              = actual_hash_length;

    /* If hash_size is 0 then hash may be NULL and then the call to memset would
     * have undefined behavior.
     */
    if (hash_size != 0)
    {
        (void)memset(hash, '!', hash_size);
    }

    /* If the output length is smaller than the hash, then report user mistake
     * for this alg that does not support a shortened hash.
     */
    if (hash_size < actual_hash_length)
    {
        status = PSA_ERROR_BUFFER_TOO_SMALL;
    }
    else if (operation->id == 0U)
    {
        status = PSA_ERROR_BAD_STATE;
    }
    /* Note: Replacing this with PSA_ALG_IS_HASH(operation->alg) is not
     * sufficient since we only support SHA2)
     */
    else if (PSA_ALG_IS_SHA2(operation->alg))
    {
        if (sha2Handle == NULL)
        {
            status = PSA_ERROR_BAD_STATE;
        }
        else
        {
            ret    = SHA2_finalize(sha2Handle, hash);
            status = map_SHA2_status(ret);
        }
    }
    else
    {
        status = PSA_ERROR_NOT_SUPPORTED;
    }

    #ifdef TFM_BUILD
        /* Do nothing. NS client will handle calling psa_hash_abort() */
    #else
    /* Ignore this return value and return the previously set status */
    (void)psa_hash_abort(operation);
    #endif

    return status;
#endif /* ENABLE_TI_CRYPTO_SHA2 */
}

/*
 *  ======== psa_hash_verify ========
 */
psa_status_t psa_hash_verify(psa_hash_operation_t *operation, const uint8_t *hash, size_t hash_length)
{
    uint8_t *actual_hash = &localBuf[0];
    size_t actual_hash_length;
    psa_status_t status;

    if (hash_length > sizeof(localBuf))
    {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    /* Zero out max TI supported hash size */
    (void)memset(actual_hash, 0, SHA2_DIGEST_LENGTH_BYTES_512);

#ifdef TFM_BUILD
    if (psa_crypto_ns_caller)
    {
        /* Compute the hash */
        status = psa_hash_finish(operation,
                                 &PSACrypto_s_secureCB->buffer[0],
                                 SHA2_DIGEST_LENGTH_BYTES_512,
                                 &actual_hash_length);
    }
    else
#endif
    {
        /* Compute the hash */
        status = psa_hash_finish(operation, actual_hash, SHA2_DIGEST_LENGTH_BYTES_512, &actual_hash_length);
    }

    if (status != PSA_SUCCESS)
    {
        if (status == PSA_ERROR_BUFFER_TOO_SMALL)
        {
            status = PSA_ERROR_INVALID_SIGNATURE;
        }
    }
    else
    {
#ifdef TFM_BUILD
        PSACrypto_s_secureCB->length = actual_hash_length;
#else
        /* For verification the hash_length provided must be the same as the
         * actual_hash_length.
         */
        if (actual_hash_length != hash_length)
        {
            status = PSA_ERROR_INVALID_SIGNATURE;
        }
        else
        {
            status = memcmp(hash, actual_hash, actual_hash_length);

            if (status != 0)
            {
                status = PSA_ERROR_INVALID_SIGNATURE;
            }
            else
            {
                status = PSA_SUCCESS;
            }
        }
#endif
    }

    /* Flush the internal hash buffers */
    if (status != PSA_SUCCESS)
    {
        /* Ignore this return value and return the previously set status */
        (void)psa_hash_abort(operation);
    }

    return status;
}

/*
 *  ======== psa_hash_compute ========
 */
psa_status_t psa_hash_compute(psa_algorithm_t alg,
                              const uint8_t *input,
                              size_t input_length,
                              uint8_t *hash,
                              size_t hash_size,
                              size_t *hash_length)
{
#ifndef ENABLE_TI_CRYPTO_SHA2
    return PSA_ERROR_NOT_SUPPORTED;
#else
    int_fast16_t ret;
    psa_status_t status;
    size_t actual_hash_length = PSA_HASH_LENGTH(alg);

    /* Check if operation is a hash */
    if (!PSA_ALG_IS_HASH(alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Fill the output buffer with something that isn't a valid hash (barring an
     * attack on the hash and deliberately-crafted input), in case the caller
     * doesn't check the return status properly.
     */
    *hash_length = actual_hash_length;

    /* If hash_size is 0 then hash may be NULL and then the call to memset would
     * have undefined behavior.
     */
    if (hash_size != 0)
    {
        (void)memset(hash, '!', hash_size);
    }

    /* If the output length is smaller than the hash, then report user mistake
     * for this alg that does not support a shortened hash.
     */
    if (hash_size < actual_hash_length)
    {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    /* This call will handle checking sha2Handle and supported alg */
    status = psa_set_sha2_hash_type(alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    ret    = SHA2_hashData(sha2Handle, input, input_length, hash);
    status = map_SHA2_status(ret);

    return status;
#endif
}

/*
 *  ======== psa_hash_compute_internal ========
 */
static psa_status_t psa_hash_compute_internal(psa_algorithm_t alg,
                                              const uint8_t *input,
                                              size_t input_length,
                                              uint8_t *hash,
                                              size_t hash_size,
                                              size_t *hash_length)
{
    psa_status_t status;

#if defined(ENABLE_TI_CRYPTO_SHA2) && defined(TFM_BUILD)
    if (psa_crypto_ns_caller)
    {
        /* Set SHA2 to use polling mode since the caller PSA function requires additional SL API calls */
        psa_sha2_set_return_behavior(PSA_POLLING_MODE);
    }

#endif

    status = psa_hash_compute(alg, input, input_length, hash, hash_size, hash_length);

#if defined(ENABLE_TI_CRYPTO_SHA2) && defined(TFM_BUILD)
    if (psa_crypto_ns_caller)
    {
        /* Restore SHA2 callback return behavior */
        psa_sha2_set_return_behavior(PSA_CALLBACK_MODE);
    }
#endif

    return status;
}

/*
 *  ======== psa_hash_compare ========
 */
psa_status_t psa_hash_compare(psa_algorithm_t alg,
                              const uint8_t *input,
                              size_t input_length,
                              const uint8_t *hash,
                              size_t hash_length)
{
    int rc;
    psa_status_t status;
    size_t actual_hash_length;
    uint8_t actual_hash[PSA_HASH_LENGTH(PSA_ALG_SHA_512)];

    if (!PSA_ALG_IS_HASH(alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

#ifdef TFM_BUILD
    if (psa_crypto_ns_caller)
    {
        /* Compute the hash and write to secure CB buffer. To allow the
         * computation to be completed in callback mode, the hash comparision
         * will be completed within the NS client.
         */
        status = psa_hash_compute(alg,
                                  input,
                                  input_length,
                                  &PSACrypto_s_secureCB->buffer[0],
                                  hash_length,
                                  &PSACrypto_s_secureCB->length);
    }
    else
#endif
    {
        /* Compute the hash */
        status = psa_hash_compute_internal(alg, input, input_length, actual_hash, hash_length, &actual_hash_length);

        if (status != PSA_SUCCESS)
        {
            /* In case the length was not correct for the output, we report a
             * invalid signature. Check crypto_values.h under the
             * PSA_ERROR_INVALID_ARGUMENT macro for further explanation (Only in
             * compare operation).
             */
            if (status == PSA_ERROR_BUFFER_TOO_SMALL)
            {
                status = PSA_ERROR_INVALID_ARGUMENT;
            }
        }
        else
        {
            /* Compare the actual hash with the provided hash */
            rc = memcmp(actual_hash, hash, hash_length);

            if (rc != 0)
            {
                status = PSA_ERROR_INVALID_SIGNATURE;
            }
        }
    }

    return status;
}

/*
 *  ======== psa_hash_clone ========
 */
psa_status_t psa_hash_clone(const psa_hash_operation_t *source_operation, psa_hash_operation_t *target_operation)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

/******************************************************************************/
/* Message Authentication Code (MAC) */
/******************************************************************************/

/*
 *  ======== psa_mac_finalize_alg_and_key_validation ========
 */
static psa_status_t psa_mac_finalize_alg_and_key_validation(psa_algorithm_t alg,
                                                            const psa_key_attributes_t *attributes,
                                                            size_t *mac_size)
{
    psa_status_t status     = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_type_t key_type = psa_get_key_type(attributes);
    size_t key_bits         = psa_get_key_bits(attributes);

    if (!PSA_ALG_IS_MAC(alg))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Validate the combination of key type and algorithm */
    status = psa_mac_key_can_do(alg, key_type);

    if (status == PSA_SUCCESS)
    {
        /* Get the output length for the algorithm and key combination */
        *mac_size = PSA_MAC_LENGTH(key_type, key_bits, alg);

        if (*mac_size < 4)
        {
            /* A very short MAC is too short for security since it can be
             * brute-forced. Ancient protocols with 32-bit MACs do exist, so we
             * make this our minimum, even though 32 bits is still too small for
             * security.
             */
            status = PSA_ERROR_NOT_SUPPORTED;
        }
        else if (*mac_size > PSA_MAC_LENGTH(key_type, key_bits, PSA_ALG_FULL_LENGTH_MAC(alg)))
        {
            /* It's impossible to "truncate" to a larger length than the full
             * length of the algorithm.
             */
            status = PSA_ERROR_INVALID_ARGUMENT;
        }
        else
        {
            status = PSA_SUCCESS;
        }
    }

    return status;
}

/*
 *  ======== psa_mac_setup ========
 */
static psa_status_t psa_mac_setup(psa_mac_operation_t *operation, psa_key_id_t key, psa_algorithm_t alg, bool is_sign)
{
#if defined(ENABLE_TI_CRYPTO_SHA2) || defined(ENABLE_TI_CRYPTO_AESCMAC)
    int_fast16_t ret;
#endif
    psa_status_t status             = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;

    if (operation->id != 0)
    {
        return PSA_ERROR_BAD_STATE;
    }

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    status = psa_mac_finalize_alg_and_key_validation(alg, &attributes, &operation->mac_size);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&attributes,
                                            (is_sign) ? PSA_KEY_USAGE_SIGN_MESSAGE : PSA_KEY_USAGE_VERIFY_MESSAGE,
                                            alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    size_t keyBits = psa_get_key_bits(&attributes);

    KeyStore_PSA_initKey(&operation->cryptoKey, toKeyStoreKeyID(key), PSA_BITS_TO_BYTES(keyBits), NULL);

    operation->is_sign = is_sign;

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    psa_algorithm_t macAlg = PSA_ALG_FULL_LENGTH_MAC(alg);

    if ((macAlg == PSA_ALG_CMAC) || (macAlg == PSA_ALG_CBC_MAC))
    {
#ifdef ENABLE_TI_CRYPTO_AESCMAC

        AESCMAC_Handle handle = psa_aescmac_get_handle(macAlg);

        if (handle == NULL)
        {
            return PSA_ERROR_BAD_STATE;
        }

        if (is_sign)
        {
            ret = AESCMAC_setupSign(handle, &operation->cryptoKey);
        }
        else
        {
            ret = AESCMAC_setupVerify(handle, &operation->cryptoKey);
        }

        status = map_AES_status(ret);
#endif /* ENABLE_TI_CRYPTO_AESCMAC */
    }
    else if (PSA_ALG_IS_HMAC(alg))
    {
#ifdef ENABLE_TI_CRYPTO_SHA2
        psa_algorithm_t hashAlg = PSA_ALG_HMAC_GET_HASH(alg);

        if (PSA_ALG_IS_SHA2(hashAlg))
        {
            /* This call will handle checking sha2Handle and supported alg */
            status = psa_set_sha2_hash_type(hashAlg);

            if (status == PSA_SUCCESS)
            {
    #ifdef TFM_BUILD
                if (psa_crypto_ns_caller)
                {
                    psa_sha2_set_return_behavior(PSA_POLLING_MODE);
                }
    #endif
                ret    = SHA2_setupHmac(sha2Handle, &operation->cryptoKey);
                status = map_SHA2_status(ret);

    #ifdef TFM_BUILD
                if (psa_crypto_ns_caller)
                {
                    /* Restore SHA2 callback return behavior */
                    psa_sha2_set_return_behavior(PSA_CALLBACK_MODE);
                }
    #endif
            }
        }
#endif
    }
    else
    {
        status = PSA_ERROR_INVALID_ARGUMENT;
    }

    if (status == PSA_SUCCESS)
    {
        /* Store the algorithm as the full length version to make algorithm
         * checking easier. No information is lost since the mac length is
         * stored in operation->mac_length.
         */
        operation->alg                   = PSA_ALG_FULL_LENGTH_MAC(alg);
        operation->id                    = PSA_CRYPTO_TI_DRIVER;
        operation->unprocessed_len       = 0;
        operation->curr_unprocessed_data = &operation->unprocessedData[0];
    }

    return status;
}

/*
 *  ======== psa_mac_sign_setup ========
 */
psa_status_t psa_mac_sign_setup(psa_mac_operation_t *operation, psa_key_id_t key, psa_algorithm_t alg)
{
    return psa_mac_setup(operation, key, alg, true);
}

/*
 *  ======== psa_mac_verify_setup ========
 */
psa_status_t psa_mac_verify_setup(psa_mac_operation_t *operation, psa_key_id_t key, psa_algorithm_t alg)
{
    return psa_mac_setup(operation, key, alg, false);
}

#ifdef ENABLE_TI_CRYPTO_AESCMAC

/*
 *  ======== psa_mac_cancel_internal ========
 */
static void psa_mac_cancel_internal(psa_algorithm_t alg)
{
    AESCMAC_Handle handle = psa_aescmac_get_handle(alg);

    #ifdef TFM_BUILD
    if (psa_crypto_ns_caller)
    {
        psa_aescmac_set_return_behavior(handle, PSA_POLLING_MODE);
    }
    #endif

    /* Always returns success */
    (void)AESCMAC_cancelOperation(handle);

    #ifdef TFM_BUILD
    if (psa_crypto_ns_caller)
    {
        psa_aescmac_set_return_behavior(handle, PSA_CALLBACK_MODE);
    }
    #endif
}

/*
 *  ======== psa_mac_update_internal ========
 */
static psa_status_t psa_mac_update_internal(psa_algorithm_t alg,
                                            const uint8_t *input,
                                            size_t input_length,
                                            bool isFinalOp)
{
    #ifndef TFM_BUILD
    (void)isFinalOp;
    #endif
    int_fast16_t ret;
    psa_status_t status;

    AESCMAC_Handle handle = psa_aescmac_get_handle(alg);

    if (handle == NULL)
    {
        return PSA_ERROR_BAD_STATE;
    }

    AESCMAC_Operation_init(&aescmacOp);
    aescmacOp.input       = (uint8_t *)input;
    aescmacOp.inputLength = input_length;

    #ifdef TFM_BUILD
    if (!isFinalOp && psa_crypto_ns_caller)
    {
        psa_aescmac_set_return_behavior(handle, PSA_POLLING_MODE);
    }
    #endif

    ret    = AESCMAC_addData(handle, &aescmacOp);
    status = map_AES_status(ret);

    #ifdef TFM_BUILD
    if (!isFinalOp && psa_crypto_ns_caller)
    {
        psa_aescmac_set_return_behavior(handle, PSA_CALLBACK_MODE);
    }
    #endif

    return status;
}
#endif

/*
 *  ======== psa_mac_abort ========
 */
psa_status_t psa_mac_abort(psa_mac_operation_t *operation)
{
    psa_status_t status;

    /* Aborting a non-active operation is allowed */
    if (operation->id == 0U)
    {
        return PSA_SUCCESS;
    }

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    if ((operation->alg == PSA_ALG_CMAC) || (operation->alg == PSA_ALG_CBC_MAC))
    {
#ifdef ENABLE_TI_CRYPTO_AESCMAC
        psa_mac_cancel_internal(operation->alg);
        status = PSA_SUCCESS;
#endif
    }
    else if (PSA_ALG_IS_HMAC(operation->alg))
    {
#ifdef ENABLE_TI_CRYPTO_SHA2
        /* Clear internal SHA2 buffers */
        SHA2_reset(sha2Handle);
        status = PSA_SUCCESS;
#endif
    }
    else
    {
        status = PSA_ERROR_INVALID_ARGUMENT;
    }

    *operation = PSA_MAC_OPERATION_INIT;

    return status;
}

/*
 *  ======== psa_mac_update ========
 */
psa_status_t psa_mac_update(psa_mac_operation_t *operation, const uint8_t *input, size_t input_length)
{
    int_fast16_t ret;
    psa_status_t status = PSA_SUCCESS;

    if (operation->id == 0U)
    {
        return PSA_ERROR_BAD_STATE;
    }

    /* Don't require hash implementations to behave correctly on a zero-length
     * input, which may have an invalid pointer.
     */
    if (input_length == 0)
    {
#ifdef TFM_BUILD
        return PSA_OPERATION_COMPLETE;
#else
        return PSA_SUCCESS;
#endif
    }

    if (operation->alg == 0)
    {
        return PSA_ERROR_BAD_STATE;
    }

    if (!PSA_ALG_IS_MAC(operation->alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    if ((operation->alg == PSA_ALG_CMAC) || (operation->alg == PSA_ALG_CBC_MAC))
    {
#ifdef ENABLE_TI_CRYPTO_AESCMAC
        /* Input needs to be buffered (and the final block of data needs to be
         * saved for use in psa_mac_sign_finish). the code must buffer all input
         * but and must leave the unprocessedData buffer unempty. In other
         * words, AESCMAC_addData must not be called on any data that could
         * potentially be the last data passed in using this function.
         */
        size_t totalUnprocessedLen = operation->unprocessed_len + input_length;

        /* Not enough total data to make up one block */
        if (totalUnprocessedLen < PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE)
        {
            /* Buffer the data and wait for more data to arrive */
            (void)memcpy(operation->curr_unprocessed_data + operation->unprocessed_len, input, input_length);
            operation->unprocessed_len = totalUnprocessedLen;

    #ifdef TFM_BUILD
            return PSA_OPERATION_COMPLETE;
    #else
            return PSA_SUCCESS;
    #endif
        }

        /* Enough data to make up at least a single block. Append enough input data
         * to the existing unprocessed data to build a full block.
         */
        size_t amountToCopy = PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE - operation->unprocessed_len;
        (void)memcpy(operation->curr_unprocessed_data + operation->unprocessed_len, input, amountToCopy);
        input += amountToCopy;
        operation->unprocessed_len += amountToCopy;

        /* Do not add the data if it could be the last block of data */
        totalUnprocessedLen -= PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE;

        if (totalUnprocessedLen == 0)
        {
    #ifdef TFM_BUILD
            return PSA_OPERATION_COMPLETE;
    #else
            return PSA_SUCCESS;
    #endif
        }

        /* Get the largest multiple of the block size that is less than the
         * number of bytes of unprocessed data.
         */
        size_t amountToAdd = (totalUnprocessedLen / PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE) * PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE;

        bool isFinalOp = true;

        if (amountToAdd > 0)
        {
            isFinalOp = false;
        }

        status = psa_mac_update_internal(operation->alg,
                                         operation->curr_unprocessed_data,
                                         PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE,
                                         isFinalOp);

        if (status != PSA_SUCCESS)
        {
            return status;
        }

        operation->unprocessed_len = 0;

    #ifdef TFM_BUILD
        /* Update the current pointer to unprocessed data to avoid potentially
         * modifying the input buffer before the crypto engine has processed it
         * when the SL driver is operating with callback return behavior.
         */
        if (operation->curr_unprocessed_data == &operation->unprocessedData[0])
        {
            operation->curr_unprocessed_data = &operation->unprocessedData[PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE];
        }
        else
        {
            operation->curr_unprocessed_data = &operation->unprocessedData[0];
        }
    #endif

        /* To ensure that not all of the data is added */
        if (amountToAdd == totalUnprocessedLen)
        {
            amountToAdd -= PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE;
        }

        if (amountToAdd > 0)
        {
            isFinalOp = true;

            /* Add any left over data to the unprocessed data buffer. This must
             * be done prior to psa_mac_update_internal() in case callback
             * return behavior is used.
             */
            (void)memcpy(operation->curr_unprocessed_data, (input + amountToAdd), (totalUnprocessedLen - amountToAdd));
            operation->unprocessed_len = totalUnprocessedLen - amountToAdd;

            status = psa_mac_update_internal(operation->alg, input, amountToAdd, isFinalOp);
        }
        else
        {
            /* Add any left over data to the unprocessed data buffer */
            (void)memcpy(operation->curr_unprocessed_data, input, totalUnprocessedLen);
            operation->unprocessed_len = totalUnprocessedLen;

    #ifdef TFM_BUILD
            if (!isFinalOp)
            {
                status = PSA_OPERATION_COMPLETE;
            }
    #endif
        }

#endif /* ENABLE_TI_CRYPTO_AESCMAC */
    }
    else if (PSA_ALG_IS_HMAC(operation->alg))
    {
#ifdef ENABLE_TI_CRYPTO_SHA2
        if (sha2Handle == NULL)
        {
            return PSA_ERROR_BAD_STATE;
        }

        ret    = SHA2_addData(sha2Handle, input, input_length);
        status = map_SHA2_status(ret);
#endif
    }
    else
    {
        (void)ret;
        status = PSA_ERROR_INVALID_ARGUMENT;
    }

    return status;
}

/*
 *  ======== psa_mac_sign_finish ========
 */
psa_status_t psa_mac_sign_finish(psa_mac_operation_t *operation, uint8_t *mac, size_t mac_size, size_t *mac_length)
{
    int_fast16_t ret;
    psa_status_t status;

    if (operation->id == 0U)
    {
        status = PSA_ERROR_BAD_STATE;
        goto exit;
    }

    if (!operation->is_sign)
    {
        status = PSA_ERROR_BAD_STATE;
        goto exit;
    }

    /* Sanity check. This will guarantee that mac_size != 0 (and so mac != NULL)
     * once all the error checks are done.
     */
    if (operation->mac_size == 0U)
    {
        status = PSA_ERROR_BAD_STATE;
        goto exit;
    }

    if (mac_size < operation->mac_size)
    {
        status = PSA_ERROR_BUFFER_TOO_SMALL;
        goto exit;
    }

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    /* Use the default computed mac size or the mac_size passed in */
    if ((operation->alg == PSA_ALG_CMAC) || (operation->alg == PSA_ALG_CBC_MAC))
    {
#ifdef ENABLE_TI_CRYPTO_AESCMAC
        AESCMAC_Handle handle = psa_aescmac_get_handle(operation->alg);

        if (handle == NULL)
        {
            return PSA_ERROR_BAD_STATE;
        }

        AESCMAC_Operation_init(&aescmacOp);
        aescmacOp.input       = operation->curr_unprocessed_data;
        aescmacOp.inputLength = operation->unprocessed_len;
        aescmacOp.mac         = mac;
        aescmacOp.macLength   = operation->mac_size;

        /* AESCMAC_finalize does not accept input lengths of zero, so if there
         * is no input, call oneStepSign, otherwise call finalize.
         */
        if (operation->unprocessed_len == 0)
        {
            psa_mac_cancel_internal(operation->alg);

            ret = AESCMAC_oneStepSign(handle, &aescmacOp, &operation->cryptoKey);
        }
        else
        {
            ret = AESCMAC_finalize(handle, &aescmacOp);
        }

        status = map_AES_status(ret);
#endif
    }
    else if (PSA_ALG_IS_HMAC(operation->alg))
    {
#ifdef ENABLE_TI_CRYPTO_SHA2
        ret    = SHA2_finalizeHmac(sha2Handle, mac);
        status = map_SHA2_status(ret);
#endif
    }
    else
    {
        (void)ret;
        status = PSA_ERROR_INVALID_ARGUMENT;
    }

exit:
    if (status != PSA_SUCCESS)
    {
        operation->mac_size = 0;
    }

#ifdef TFM_BUILD
    PSACrypto_s_secureCB->length = operation->mac_size;

    /* NS client will handle calling psa_mac_abort() after verifying the final
     * result and invalidating excess room in output buffer.
     */
#else
    *mac_length = operation->mac_size;

    /* In case of success, set the potential excess room in the output buffer to
     * an invalid value, to avoid potentially leaking a longer MAC. In case of
     * error, set the output length and content to a safe default, such that in
     * case the caller misses an error check, the output would be an
     * unachievable MAC.
     */
    if (mac_size > operation->mac_size)
    {
        (void)memset(&mac[operation->mac_size], '!', mac_size - operation->mac_size);
    }

    (void)psa_mac_abort(operation);
#endif

    return status;
}

/*
 *  ======== psa_mac_verify_finish ========
 */
psa_status_t psa_mac_verify_finish(psa_mac_operation_t *operation, const uint8_t *mac, size_t mac_length)
{
    int_fast16_t ret;
    psa_status_t status;

    if (operation->id == 0U)
    {
        status = PSA_ERROR_BAD_STATE;
        goto exit;
    }

    if (operation->is_sign)
    {
        status = PSA_ERROR_BAD_STATE;
        goto exit;
    }

    /* Sanity check. This will guarantee that mac_size != 0 (and so mac != NULL)
     * once all the error checks are done.
     */
    if (operation->mac_size == 0)
    {
        status = PSA_ERROR_BAD_STATE;
        goto exit;
    }

    if (mac_length != operation->mac_size)
    {
        status = PSA_ERROR_INVALID_SIGNATURE;
        goto exit;
    }

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    /* Use the default computed mac size or the mac_size passed in */
    if ((operation->alg == PSA_ALG_CMAC) || (operation->alg == PSA_ALG_CBC_MAC))
    {
#ifdef ENABLE_TI_CRYPTO_AESCMAC
        AESCMAC_Handle handle = psa_aescmac_get_handle(operation->alg);

        if (handle == NULL)
        {
            status = PSA_ERROR_BAD_STATE;
            goto exit;
        }

        AESCMAC_Operation_init(&aescmacOp);
        aescmacOp.input       = operation->curr_unprocessed_data;
        aescmacOp.inputLength = operation->unprocessed_len;
        aescmacOp.mac         = (uint8_t *)mac;
        aescmacOp.macLength   = operation->mac_size;

        /* AESCMAC_finalize does not accept input lengths of zero, so if there
         * is no input, call oneStepVerify, otherwise call finalize.
         */
        if (operation->unprocessed_len == 0)
        {
            psa_mac_cancel_internal(operation->alg);

            ret = AESCMAC_oneStepVerify(handle, &aescmacOp, &operation->cryptoKey);
        }
        else
        {
            ret = AESCMAC_finalize(handle, &aescmacOp);
        }

        status = map_AES_status(ret);
#endif
    }
    else if (PSA_ALG_IS_HMAC(operation->alg))
    {
#ifdef ENABLE_TI_CRYPTO_SHA2
        int rc;

        if (sha2Handle == NULL)
        {
            status = PSA_ERROR_BAD_STATE;
            goto exit;
        }

    #ifdef TFM_BUILD
        if (psa_crypto_ns_caller)
        {
            ret    = SHA2_finalizeHmac(sha2Handle, &PSACrypto_s_secureCB->buffer[0]);
            status = map_SHA2_status(ret);
        }
        else
    #endif
        {
            uint8_t actualMAC[PSA_HMAC_MAX_HASH_BLOCK_SIZE] = {0};

            ret    = SHA2_finalizeHmac(sha2Handle, actualMAC);
            status = map_SHA2_status(ret);

            if (status == PSA_SUCCESS)
            {
                rc = memcmp(actualMAC, mac, operation->mac_size);

                if (rc != 0)
                {
                    status = PSA_ERROR_INVALID_SIGNATURE;
                }
            }
        }
#endif
    }
    else
    {
        (void)ret;
        status = PSA_ERROR_INVALID_ARGUMENT;
    }

exit:
#ifdef TFM_BUILD
    /* Do nothing. NS client will handle calling psa_mac_abort() after verifying
     * the MAC.
     */
#else
    (void)psa_mac_abort(operation);
#endif

    return status;
}

/*
 *  ======== psa_mac_compute ========
 */
psa_status_t psa_mac_compute(psa_key_id_t key,
                             psa_algorithm_t alg,
                             const uint8_t *input,
                             size_t input_length,
                             uint8_t *mac,
                             size_t mac_size,
                             size_t *mac_length)
{
    int_fast16_t ret;
    psa_status_t status             = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    size_t operation_mac_size       = 0;
    psa_algorithm_t algBase;
    size_t keyBits;
    CryptoKey cryptoKey;

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    keyBits = psa_get_key_bits(&attributes);

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Compute operation mac size */
    status = psa_mac_finalize_alg_and_key_validation(alg, &attributes, &operation_mac_size);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    if (mac_size < operation_mac_size)
    {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    KeyStore_PSA_initKey(&cryptoKey, toKeyStoreKeyID(key), PSA_BITS_TO_BYTES(keyBits), NULL);

    algBase = PSA_ALG_FULL_LENGTH_MAC(alg);

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    if ((algBase == PSA_ALG_CMAC) || (algBase == PSA_ALG_CBC_MAC))
    {
#ifdef ENABLE_TI_CRYPTO_AESCMAC
        AESCMAC_Handle handle = psa_aescmac_get_handle(algBase);

        if (handle == NULL)
        {
            status = PSA_ERROR_BAD_STATE;
        }
        else
        {
            AESCMAC_Operation_init(&aescmacOp);
            aescmacOp.input       = (uint8_t *)input;
            aescmacOp.inputLength = input_length;
            aescmacOp.mac         = mac;
            aescmacOp.macLength   = operation_mac_size;

            ret    = AESCMAC_oneStepSign(handle, &aescmacOp, &cryptoKey);
            status = map_AES_status(ret);
        }
#endif
    }
    else if (PSA_ALG_IS_HMAC(alg))
    {
#ifdef ENABLE_TI_CRYPTO_SHA2
        psa_algorithm_t hashAlg = PSA_ALG_HMAC_GET_HASH(alg);

        /* This call will handle checking sha2Handle and supported alg */
        status = psa_set_sha2_hash_type(hashAlg);

        if (status == PSA_SUCCESS)
        {
            ret    = SHA2_hmac(sha2Handle, &cryptoKey, input, input_length, mac);
            status = map_SHA2_status(ret);
        }
#endif
    }
    else
    {
        (void)ret;
        /* Do nothing */
    }

    if (status == PSA_SUCCESS)
    {
        *mac_length = operation_mac_size;
    }

    return status;
}

/*
 *  ======== psa_mac_verify ========
 */
psa_status_t psa_mac_verify(psa_key_id_t key,
                            psa_algorithm_t alg,
                            const uint8_t *input,
                            size_t input_length,
                            const uint8_t *mac,
                            size_t mac_length)
{
    int_fast16_t ret;
    psa_status_t status             = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    size_t operation_mac_size       = 0;
    size_t keyBits;
    psa_algorithm_t algBase;
    CryptoKey cryptoKey;

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    keyBits = psa_get_key_bits(&attributes);

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&attributes, PSA_KEY_USAGE_VERIFY_MESSAGE, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Compute operation mac size */
    status = psa_mac_finalize_alg_and_key_validation(alg, &attributes, &operation_mac_size);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    if (mac_length < operation_mac_size)
    {
        return PSA_ERROR_INVALID_SIGNATURE;
    }

    KeyStore_PSA_initKey(&cryptoKey, toKeyStoreKeyID(key), PSA_BITS_TO_BYTES(keyBits), NULL);

    algBase = PSA_ALG_FULL_LENGTH_MAC(alg);

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    if ((algBase == PSA_ALG_CMAC) || (algBase == PSA_ALG_CBC_MAC))
    {
#ifdef ENABLE_TI_CRYPTO_AESCMAC
        AESCMAC_Handle handle = psa_aescmac_get_handle(algBase);

        if (handle == NULL)
        {
            return PSA_ERROR_BAD_STATE;
        }

        AESCMAC_Operation_init(&aescmacOp);
        aescmacOp.input       = (uint8_t *)input;
        aescmacOp.inputLength = input_length;
        aescmacOp.mac         = (uint8_t *)mac;
        aescmacOp.macLength   = mac_length;

        ret    = AESCMAC_oneStepVerify(handle, &aescmacOp, &cryptoKey);
        status = map_AES_status(ret);
#endif /* ENABLE_TI_CRYPTO_AESCMAC */
    }
    else if (PSA_ALG_IS_HMAC(alg))
    {
#ifdef ENABLE_TI_CRYPTO_SHA2
        psa_algorithm_t hashAlg = PSA_ALG_HMAC_GET_HASH(alg);

        /* This call will handle checking sha2Handle and supported alg */
        status = psa_set_sha2_hash_type(hashAlg);
        if (status != PSA_SUCCESS)
        {
            return status;
        }

    #ifdef TFM_BUILD
        if (psa_crypto_ns_caller)
        {
            ret    = SHA2_hmac(sha2Handle, &cryptoKey, input, input_length, &PSACrypto_s_secureCB->buffer[0]);
            status = map_SHA2_status(ret);
        }
        else
    #endif
        {
            ret    = SHA2_hmac(sha2Handle, &cryptoKey, input, input_length, &localBuf[0]);
            status = map_SHA2_status(ret);

            if (status == PSA_SUCCESS)
            {
                int rc;

                /* Add a verification step here */
                rc = memcmp(localBuf, mac, mac_length);
                if (rc != 0)
                {
                    /* No match */
                    status = PSA_ERROR_INVALID_SIGNATURE;
                }
            }
        }
#endif /* ENABLE_TI_CRYPTO_SHA2 */
    }
    else
    {
        (void)ret;
        /* Do nothing */
    }

    return status;
}

/******************************************************************************/
/* Asymmetric cryptography */
/******************************************************************************/

/*
 *  ======== psa_sign_verify_check_alg ========
 */
static psa_status_t psa_sign_verify_check_alg(int input_is_message, psa_algorithm_t alg)
{
    if (input_is_message)
    {
        if (!PSA_ALG_IS_SIGN_MESSAGE(alg))
        {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        if (PSA_ALG_IS_HASH_AND_SIGN(alg))
        {
            if (!PSA_ALG_IS_HASH(PSA_ALG_SIGN_GET_HASH(alg)))
            {
                return PSA_ERROR_INVALID_ARGUMENT;
            }
        }
    }
    else
    {
        if (!PSA_ALG_IS_HASH_AND_SIGN(alg))
        {
            return PSA_ERROR_INVALID_ARGUMENT;
        }
    }

    return PSA_SUCCESS;
}

#ifdef ENABLE_TI_CRYPTO_ECDSA
/*
 *  ======== psa_get_hash_length ========
 */
static psa_status_t psa_get_hash_length(psa_algorithm_t hash_alg, size_t *hash_size)
{
    psa_status_t status = PSA_SUCCESS;

    switch (hash_alg)
    {
        case PSA_ALG_SHA_224:
            *hash_size = SHA2_DIGEST_LENGTH_BYTES_224;
            break;

        case PSA_ALG_SHA_256:
            *hash_size = SHA2_DIGEST_LENGTH_BYTES_256;
            break;

        case PSA_ALG_SHA_384:
            *hash_size = SHA2_DIGEST_LENGTH_BYTES_384;
            break;

        case PSA_ALG_SHA_512:
            *hash_size = SHA2_DIGEST_LENGTH_BYTES_512;
            break;

        default:
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
    }

    return status;
}
#endif

/*
 *  ======== psa_sign_message ========
 */
psa_status_t psa_sign_message(psa_key_id_t key,
                              psa_algorithm_t alg,
                              const uint8_t *input,
                              size_t input_length,
                              uint8_t *signature,
                              size_t signature_size,
                              size_t *signature_length)
{
    int_fast16_t ret;
    psa_status_t status             = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_type_t keyType;
    size_t keyBits;
    size_t keyBytes;

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    keyType = psa_get_key_type(&attributes);
    keyBits = psa_get_key_bits(&attributes);

    *signature_length = PSA_SIGN_OUTPUT_SIZE(keyType, keyBits, alg);

    /* 1 to represent message instead of hash */
    status = psa_sign_verify_check_alg(1, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Immediately reject a zero-length signature buffer. This guarantees that
     * signature must be a valid pointer. (On the other hand, the input buffer
     * can in principle be empty since it doesn't actually have to be a hash.)
     */
    if (signature_size == 0)
    {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    if (!PSA_KEY_TYPE_IS_KEY_PAIR(keyType))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    keyBytes = PSA_BITS_TO_BYTES(keyBits);

#if defined(ENABLE_TI_CRYPTO_EDDSA) || defined(ENABLE_TI_CRYPTO_ECDSA)
    KeyStore_PSA_initKey(&privateKey, toKeyStoreKeyID(key), keyBytes, NULL);
#endif

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    if (alg == PSA_ALG_PURE_EDDSA)
    {
#ifdef ENABLE_TI_CRYPTO_EDDSA
        /* The twisted Edwards curves Ed25519 */
        if (eddsaHandle == NULL)
        {
            return PSA_ERROR_BAD_STATE;
        }

        uint8_t *myPublicKeyMaterial;

        /* First generate public key */
        if (sizeof(localBuf) >= ECCParams_CURVE25519_LENGTH)
        {
            myPublicKeyMaterial = &localBuf[0];
        }
        else
        {
            return PSA_ERROR_INSUFFICIENT_MEMORY;
        }

        CryptoKeyPlaintext_initBlankKey(&publicKey, myPublicKeyMaterial, ECCParams_CURVE25519_LENGTH);
        EDDSA_OperationGeneratePublicKey operation;
        EDDSA_OperationGeneratePublicKey_init(&operation);
        operation.myPrivateKey = &privateKey;
        operation.myPublicKey  = &publicKey;
    #if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
         (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
        /* Only non-HSM drivers utilize input curve params for EdDSA */
        operation.curve = &ECCParams_Ed25519;
    #endif

    #ifdef TFM_BUILD
        if (psa_crypto_ns_caller)
        {
            /* Set EDDSA driver to use polling mode since an additional SL API call is needed */
            psa_eddsa_set_return_behavior(PSA_POLLING_MODE);
        }
    #endif

        ret = EDDSA_generatePublicKey(eddsaHandle, &operation);

    #ifdef TFM_BUILD
        if (psa_crypto_ns_caller)
        {
            /* Set EDDSA driver to use callback mode for the final SL API call */
            psa_eddsa_set_return_behavior(PSA_CALLBACK_MODE);
        }
    #endif

        status = map_EDDSA_status(ret, false);

        if (status != PSA_SUCCESS)
        {
            return status;
        }

        EDDSA_OperationSign_init(&eddsaSignOp);
    #if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
         (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
        /* Only non-HSM drivers utilize input curve params for EdDSA */
        eddsaSignOp.curve = &ECCParams_Ed25519;
    #endif
        eddsaSignOp.myPrivateKey           = &privateKey;
        eddsaSignOp.myPublicKey            = &publicKey;
        eddsaSignOp.preHashedMessage       = input;
        eddsaSignOp.preHashedMessageLength = input_length;
        eddsaSignOp.R                      = signature;
        eddsaSignOp.S                      = signature + (*signature_length / 2);

        ret    = EDDSA_sign(eddsaHandle, &eddsaSignOp);
        status = map_EDDSA_status(ret, false);

        if (status != PSA_SUCCESS)
        {
            return status;
        }
#endif /* ENABLE_TI_CRYPTO_EDDSA */
    }
    else if (PSA_ALG_IS_RANDOMIZED_ECDSA(alg) && PSA_KEY_TYPE_IS_ECC(keyType))
    {
#ifdef ENABLE_TI_CRYPTO_ECDSA
        psa_algorithm_t hash_alg = PSA_ALG_SIGN_GET_HASH(alg);
        size_t hash_length;
        size_t hash_size;
        size_t curveBytes;
        uint8_t *hash = NULL;

        if (ecdsaHandle == NULL)
        {
            return PSA_ERROR_BAD_STATE;
        }

        ECDSA_OperationSign_init(&ecdsaSignOp);

    #if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
        ecdsaSignOp.curveType = map_keyTypeToECDSACurveTypeHSM(keyType, keyBits);

        if (ecdsaSignOp.curveType == (ECDSA_CurveType)0)
        {
            return PSA_ERROR_NOT_SUPPORTED;
        }
    #elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)
        ecdsaSignOp.curve = map_keyTypeToECCParams(keyType, keyBits);

        if (ecdsaSignOp.curve == NULL)
        {
            return PSA_ERROR_NOT_SUPPORTED;
        }
    #endif

        status = psa_get_hash_length(hash_alg, &hash_size);

        if (status != PSA_SUCCESS)
        {
            return status;
        }

        curveBytes = PSA_BITS_TO_BYTES(psa_get_key_bits(&attributes));

        /* Per FIPS 186-4 recommendation, the hash length must provide at least as much
         * security strength as the curve length. This is satisfied when the hash length
         * at least matches the curve length, as well as when SHA-512 is used with a 521-bit
         * curve.
         */
        if ((hash_size < curveBytes) && ((hash_size != 64) && (curveBytes != 66)))
        {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        if (hash_size <= sizeof(localBuf))
        {
            hash = &localBuf[0];
        }
        else
        {
            return PSA_ERROR_INSUFFICIENT_MEMORY;
        }

        /* Compute the hash */
        status = psa_hash_compute_internal(hash_alg, input, input_length, hash, hash_size, &hash_length);

        if (status != PSA_SUCCESS)
        {
            return status;
        }

        ecdsaSignOp.myPrivateKey = &privateKey;
        ecdsaSignOp.hash         = hash;
        ecdsaSignOp.r            = signature;
        ecdsaSignOp.s            = signature + (*signature_length / 2);

        /* Generates the signature of the message hash */
        ret    = ECDSA_sign(ecdsaHandle, &ecdsaSignOp);
        status = map_ECDSA_status(ret, false);
#endif /* ENABLE_TI_CRYPTO_ECDSA */
    }
    else
    {
        (void)ret;
        /* Do nothing */
    }
    /* Supported Curve Types are listed in the ECDSACC26X2.h. Montgomery and
     * Edwards curve types are not currently supported.
     */

    return status;
}

/*
 *  ======== psa_verify_message ========
 */
psa_status_t psa_verify_message(psa_key_id_t key,
                                psa_algorithm_t alg,
                                const uint8_t *input,
                                size_t input_length,
                                const uint8_t *signature,
                                size_t signature_length)
{
    int_fast16_t ret;
    psa_status_t status             = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_type_t keyType;
    psa_key_bits_t keyBits;
    psa_ecc_family_t curve;
    size_t publicKeyBytes;

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&attributes, PSA_KEY_USAGE_VERIFY_MESSAGE, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    keyType = psa_get_key_type(&attributes);
    keyBits = psa_get_key_bits(&attributes);

    /* 1 to indicate message instead of hash */
    status = psa_sign_verify_check_alg(1, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Immediately reject a zero-length signature buffer. This guarantees that
     * signature must be a valid pointer. (On the other hand, the input buffer
     * can in principle be empty since it doesn't actually have to be a hash.)
     */
    if (signature_length == 0)
    {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    curve = PSA_KEY_TYPE_ECC_GET_FAMILY(keyType);

    /* Note: The public key size for Montgomery and Twisted Edwards curves is
     * the same as the key_bytes, while the public key size for Weierstrass and
     * Brainpool curves use uncompressed format (2 x key_bytes + 1). The
     * following publicKeyBytes assignment cannot be replaced with
     * PSA_EXPORT_KEY_OUTPUT_SIZE(keyType, keyBits) because it returns the
     * uncompressed format size for Twisted Edwards curves which is incorrect.
     */
    publicKeyBytes = ((curve == PSA_ECC_FAMILY_MONTGOMERY) || (curve == PSA_ECC_FAMILY_TWISTED_EDWARDS))
                         ? PSA_BITS_TO_BYTES(keyBits)
                         : PSA_KEY_EXPORT_ECC_PUBLIC_KEY_MAX_SIZE(keyBits);

#if defined(ENABLE_TI_CRYPTO_EDDSA) || defined(ENABLE_TI_CRYPTO_ECDSA)
    KeyStore_PSA_initKey(&publicKey, toKeyStoreKeyID(key), publicKeyBytes, NULL);
#endif

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    if (alg == PSA_ALG_PURE_EDDSA)
    {
#ifdef ENABLE_TI_CRYPTO_EDDSA
        if (eddsaHandle == NULL)
        {
            return PSA_ERROR_BAD_STATE;
        }

        EDDSA_OperationVerify_init(&eddsaVerifyOp);
    #if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
         (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
        /* Only non-HSM drivers utilize input curve params for EdDSA */
        eddsaVerifyOp.curve = &ECCParams_Ed25519;
    #endif
        eddsaVerifyOp.theirPublicKey         = &publicKey;
        eddsaVerifyOp.preHashedMessage       = input;
        eddsaVerifyOp.preHashedMessageLength = input_length;
        eddsaVerifyOp.R                      = signature;
        eddsaVerifyOp.S                      = signature + (signature_length / 2);

        ret    = EDDSA_verify(eddsaHandle, &eddsaVerifyOp);
        status = map_EDDSA_status(ret, true);
#endif /* ENABLE_TI_CRYPTO_EDDSA */
    }
    else if (PSA_ALG_IS_RANDOMIZED_ECDSA(alg) && PSA_KEY_TYPE_IS_ECC(keyType))
    {
#ifdef ENABLE_TI_CRYPTO_ECDSA
        psa_algorithm_t hash_alg = PSA_ALG_SIGN_GET_HASH(alg);
        size_t hash_length;
        uint8_t *hash = NULL;
        size_t hash_size;
        size_t curveBytes;

        if (ecdsaHandle == NULL)
        {
            return PSA_ERROR_BAD_STATE;
        }

        ECDSA_OperationVerify_init(&ecdsaVerifyOp);

    #if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
        ecdsaVerifyOp.curveType = map_keyTypeToECDSACurveTypeHSM(keyType, keyBits);

        if (ecdsaVerifyOp.curveType == (ECDSA_CurveType)0)
        {
            return PSA_ERROR_NOT_SUPPORTED;
        }
    #elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)
        ecdsaVerifyOp.curve = map_keyTypeToECCParams(keyType, keyBits);

        if (ecdsaVerifyOp.curve == NULL)
        {
            return PSA_ERROR_NOT_SUPPORTED;
        }
    #endif

        status = psa_get_hash_length(hash_alg, &hash_size);

        if (status != PSA_SUCCESS)
        {
            return status;
        }

        curveBytes = PSA_BITS_TO_BYTES(psa_get_key_bits(&attributes));

        /* Per FIPS 186-4 recommendation, the hash length must provide at least as much
         * security strength as the curve length. This is satisfied when the hash length
         * at least matches the curve length, as well as when SHA-512 is used with a 521-bit
         * curve.
         */
        if ((hash_size < curveBytes) && ((hash_size != 64) && (curveBytes != 66)))
        {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        if (hash_size <= sizeof(localBuf))
        {
            hash = &localBuf[0];
        }
        else
        {
            return PSA_ERROR_INSUFFICIENT_MEMORY;
        }

        /* Compute the hash */
        status = psa_hash_compute_internal(hash_alg, input, input_length, hash, hash_size, &hash_length);

        if (status != PSA_SUCCESS)
        {
            return status;
        }

        ecdsaVerifyOp.theirPublicKey = &publicKey;
        ecdsaVerifyOp.hash           = hash;
        ecdsaVerifyOp.r              = signature;
        ecdsaVerifyOp.s              = signature + (signature_length / 2);

        /* Generates the signature */
        ret    = ECDSA_verify(ecdsaHandle, &ecdsaVerifyOp);
        status = map_ECDSA_status(ret, true);
#endif /* ENABLE_TI_CRYPTO_ECDSA */
    }
    else
    {
        (void)ret;
        /* Do nothing */
    }

    return status;
}

/*
 *  ======== psa_sign_hash ========
 */
psa_status_t psa_sign_hash(psa_key_id_t key,
                           psa_algorithm_t alg,
                           const uint8_t *hash,
                           size_t hash_length,
                           uint8_t *signature,
                           size_t signature_size,
                           size_t *signature_length)
{
#ifndef ENABLE_TI_CRYPTO_ECDSA
    return PSA_ERROR_NOT_SUPPORTED;
#else
    int_fast16_t ret;
    psa_status_t status             = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    CryptoKey myPrivateKey;
    size_t curveBits;
    size_t keyBits;
    psa_key_type_t privateKeyType;
    size_t outputSize;
    size_t curveBytes;

    *signature_length = 0;

    if (ecdsaHandle == NULL)
    {
        return PSA_ERROR_BAD_STATE;
    }

    /* 0 to rep hash */
    status = psa_sign_verify_check_alg(0, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Immediately reject a zero-length signature buffer. This guarantees that
     * signature must be a valid pointer. (On the other hand, the input buffer
     * can in principle be empty since it doesn't actually have to be a hash.)
     */
    if (signature_size == 0)
    {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&attributes, PSA_KEY_USAGE_SIGN_HASH, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Use key type to map to curve type */
    privateKeyType = psa_get_key_type(&attributes);
    keyBits        = psa_get_key_bits(&attributes);

    /* SL Crypto drivers only support ECC */
    if (!PSA_KEY_TYPE_IS_ECC_KEY_PAIR(privateKeyType))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Only support ECDSA for psa_sign_hash */
    if (!PSA_ALG_IS_ECDSA(alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* No support for deterministic ECDSA */
    if (PSA_ALG_IS_DETERMINISTIC_ECDSA(alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    curveBits  = psa_get_key_bits(&attributes);
    curveBytes = PSA_BITS_TO_BYTES(curveBits);
    outputSize = PSA_SIGN_OUTPUT_SIZE(privateKeyType, curveBits, alg);
    if (signature_size < outputSize)
    {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    if (hash_length != PSA_HASH_LENGTH(PSA_ALG_GET_HASH(alg)))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Per FIPS 186-4 recommendation, the hash length must provide at least as much
     * security strength as the curve length. This is satisfied when the hash length
     * at least matches the curve length, as well as when SHA-512 is used with a 521-bit
     * curve.
     */
    if ((hash_length < curveBytes) && ((hash_length != 64) && (curveBytes != 66)))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    KeyStore_PSA_initKey(&myPrivateKey, toKeyStoreKeyID(key), PSA_BITS_TO_BYTES(keyBits), NULL);

    ECDSA_OperationSign_init(&ecdsaSignOp);

    #if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    ecdsaSignOp.curveType = map_keyTypeToECDSACurveTypeHSM(privateKeyType, keyBits);

    if (ecdsaSignOp.curveType == (ECDSA_CurveType)0)
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }
    #elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)
    ecdsaSignOp.curve = map_keyTypeToECCParams(privateKeyType, keyBits);

    if (ecdsaSignOp.curve == NULL)
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }
    #endif

    ecdsaSignOp.myPrivateKey = &myPrivateKey;
    ecdsaSignOp.hash         = hash;
    ecdsaSignOp.r            = signature;
    ecdsaSignOp.s            = signature + curveBytes;

    /* Generates the signature */
    ret    = ECDSA_sign(ecdsaHandle, &ecdsaSignOp);
    status = map_ECDSA_status(ret, false);

    if (status == PSA_SUCCESS)
    {
        *signature_length = outputSize;
    }

    return status;
#endif /* ENABLE_TI_CRYPTO_ECDSA */
}

/*
 *  ======== psa_verify_hash ========
 */
psa_status_t psa_verify_hash(psa_key_id_t key,
                             psa_algorithm_t alg,
                             const uint8_t *hash,
                             size_t hash_length,
                             const uint8_t *signature,
                             size_t signature_length)
{
#ifndef ENABLE_TI_CRYPTO_ECDSA
    return PSA_ERROR_NOT_SUPPORTED;
#else
    int_fast16_t ret;
    psa_status_t status             = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    CryptoKey theirPublicKey;
    size_t publicKeyBytes;
    size_t expectedOutputSize;
    size_t keyBits;
    size_t curveBytes;
    psa_key_type_t keyType;

    if (ecdsaHandle == NULL)
    {
        return PSA_ERROR_BAD_STATE;
    }

    /* 0 to rep hash */
    status = psa_sign_verify_check_alg(0, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&attributes, PSA_KEY_USAGE_VERIFY_HASH, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Use key type to map to curve type */
    keyType = psa_get_key_type(&attributes);
    keyBits = psa_get_key_bits(&attributes);

    /* Only accept PSA_KEY_TYPE_ECC_KEY_PAIR keys here */
    if (!PSA_KEY_TYPE_ECC_PUBLIC_KEY(keyType))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Only support ECDSA */
    if (!PSA_ALG_IS_ECDSA(alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* No support for deterministic ecdsa */
    if (PSA_ALG_IS_DETERMINISTIC_ECDSA(alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    expectedOutputSize = PSA_SIGN_OUTPUT_SIZE(keyType, keyBits, alg);

    if (signature_length != expectedOutputSize)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    curveBytes = PSA_BITS_TO_BYTES(psa_get_key_bits(&attributes));

    /* Per FIPS 186-4 recommendation, the hash length must provide at least as much
     * security strength as the curve length. This is satisfied when the hash length
     * at least matches the curve length, as well as when SHA-512 is used with a 521-bit
     * curve.
     */
    if ((hash_length < curveBytes) && ((hash_length != 64) && (curveBytes != 66)))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    publicKeyBytes = PSA_EXPORT_KEY_OUTPUT_SIZE(keyType, keyBits);

    KeyStore_PSA_initKey(&theirPublicKey, toKeyStoreKeyID(key), publicKeyBytes, NULL);

    ECDSA_OperationVerify_init(&ecdsaVerifyOp);

    #if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    ecdsaVerifyOp.curveType = map_keyTypeToECDSACurveTypeHSM(keyType, keyBits);

    if (ecdsaVerifyOp.curveType == (ECDSA_CurveType)0)
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }
    #elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)
    ecdsaVerifyOp.curve = map_keyTypeToECCParams(keyType, keyBits);

    if (ecdsaVerifyOp.curve == NULL)
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }
    #endif

    ecdsaVerifyOp.theirPublicKey = &theirPublicKey;
    ecdsaVerifyOp.hash           = hash;
    ecdsaVerifyOp.r              = signature;
    ecdsaVerifyOp.s              = signature + (signature_length / 2);

    /* Generates the signature */
    ret    = ECDSA_verify(ecdsaHandle, &ecdsaVerifyOp);
    status = map_ECDSA_status(ret, true);

    return status;
#endif /* ENABLE_TI_CRYPTO_ECDSA */
}

/*
 *  ======== psa_asymmetric_encrypt ========
 */
psa_status_t psa_asymmetric_encrypt(psa_key_id_t key,
                                    psa_algorithm_t alg,
                                    const uint8_t *input,
                                    size_t input_length,
                                    const uint8_t *salt,
                                    size_t salt_length,
                                    uint8_t *output,
                                    size_t output_size,
                                    size_t *output_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

/*
 *  ======== psa_asymmetric_decrypt ========
 */
psa_status_t psa_asymmetric_decrypt(psa_key_id_t key,
                                    psa_algorithm_t alg,
                                    const uint8_t *input,
                                    size_t input_length,
                                    const uint8_t *salt,
                                    size_t salt_length,
                                    uint8_t *output,
                                    size_t output_size,
                                    size_t *output_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

/******************************************************************************/
/* Symmetric cryptography */
/******************************************************************************/

/*
 *  ======== psa_cipher_error ========
 *  Puts the operation in an error state, resets the key attributes if necessary,
 *  and spits back the error code to return to the application
 */
static psa_status_t psa_cipher_error(psa_cipher_operation_t *operation,
                                     psa_status_t error,
                                     psa_key_attributes_t *attributes)
{
    operation->in_error_state = 1U;

    if (attributes != NULL)
    {
        psa_reset_key_attributes(attributes);
    }

    return error;
}

/*
 *  ======== psa_cipher_encrypt ========
 */
psa_status_t psa_cipher_encrypt(psa_key_id_t key,
                                psa_algorithm_t alg,
                                const uint8_t *input,
                                size_t input_length,
                                uint8_t *output,
                                size_t output_size,
                                size_t *output_length)
{
    CryptoKey cryptoKey;
    int_fast16_t ret;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_type_t symKeyType;
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    size_t blockLength;
    size_t keyBits;
    size_t outputSize;
#if defined(ENABLE_TI_CRYPTO_AESCTR) || defined(ENABLE_TI_CRYPTO_AESCBC)
    size_t ivSize;
    #if defined(PSA_DEBUG)
    uint8_t tempArray[16] = {0};
    uint8_t *iv_array     = tempArray;
    #else
    uint8_t iv_array[16] = {0};
    #endif
#endif

    *output_length = 0;

    /* The requested algorithm must be one that can be processed by cipher */
    if (!PSA_ALG_IS_CIPHER(alg))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&attributes, PSA_KEY_USAGE_ENCRYPT, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    symKeyType  = psa_get_key_type(&attributes);
    blockLength = PSA_BLOCK_CIPHER_BLOCK_LENGTH(symKeyType);
    if (input_length < blockLength)
    {
        /* If no padding is required, then input must be at least blockLength */
        if (alg == PSA_ALG_ECB_NO_PADDING || alg == PSA_ALG_CBC_NO_PADDING)
        {
            return PSA_ERROR_INVALID_ARGUMENT;
        }
    }

#if defined(ENABLE_TI_CRYPTO_AESCTR) || defined(ENABLE_TI_CRYPTO_AESCBC)
    /* IV length */
    ivSize = PSA_CIPHER_IV_LENGTH(symKeyType, alg);
#endif

    /* This macro accounts for the IV as well  */
    outputSize = PSA_CIPHER_ENCRYPT_OUTPUT_SIZE(symKeyType, alg, input_length);
    /* Make sure output_size is big enough to take in the IV (if there is one) and ciphertext */
    if (output_size < outputSize)
    {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    keyBits = psa_get_key_bits(&attributes);
    KeyStore_PSA_initKey(&cryptoKey, toKeyStoreKeyID(key), PSA_BITS_TO_BYTES(keyBits), NULL);

    status = psa_check_aes_handle(alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    if (alg == PSA_ALG_ECB_NO_PADDING)
    {
#ifdef ENABLE_TI_CRYPTO_AESECB
        AESECB_Operation_init(&aesecbOp);
        aesecbOp.key         = &cryptoKey;
        aesecbOp.input       = (uint8_t *)input;
        aesecbOp.inputLength = input_length;
        aesecbOp.output      = output;

        ret    = AESECB_oneStepEncrypt(aesecbHandle, &aesecbOp);
        status = map_AES_status(ret);
#endif
    }
    else if (alg == PSA_ALG_CTR)
    {
#ifdef ENABLE_TI_CRYPTO_AESCTR
        AESCTR_Operation_init(&aesctrOp);
        aesctrOp.key         = &cryptoKey;
        aesctrOp.input       = input;
        aesctrOp.inputLength = input_length;
        /* Per PSA spec, psa_cipher_encrypt IV is followed by ciphertext */
        aesctrOp.output      = output + ivSize;

    #if defined(PSA_DEBUG)
        status = psa_debug_get_iv(&iv_array);
        if (status != PSA_SUCCESS)
        {
            status = psa_generate_random_internal(iv_array, ivSize);
            if (status != PSA_SUCCESS)
            {
                return status;
            }
        }
    #else
        status = psa_generate_random_internal(iv_array, ivSize);
        if (status != PSA_SUCCESS)
        {
            return status;
        }
    #endif

        aesctrOp.initialCounter = iv_array;

        ret    = AESCTR_oneStepEncrypt(aesctrHandle, &aesctrOp);
        status = map_AES_status(ret);

        /* Write the IV to output buffer */
        (void)memcpy(output, iv_array, ivSize);
#endif /* ENABLE_TI_CRYPTO_AESCTR */
    }
    else if (alg == PSA_ALG_CBC_NO_PADDING)
    {
#ifdef ENABLE_TI_CRYPTO_AESCBC
        AESCBC_Operation_init(&aescbcOp);
        aescbcOp.key         = &cryptoKey;
        aescbcOp.input       = (uint8_t *)input;
        aescbcOp.inputLength = input_length;
        /* Per PSA spec, psa_cipher_encrypt IV is followed by ciphertext */
        aescbcOp.output      = output + ivSize;

    #if defined(PSA_DEBUG)
        status = psa_debug_get_iv(&iv_array);
        if (status != PSA_SUCCESS)
        {
            status = psa_generate_random_internal(iv_array, ivSize);
            if (status != PSA_SUCCESS)
            {
                return status;
            }
        }
    #else
        status = psa_generate_random_internal(iv_array, ivSize);
        if (status != PSA_SUCCESS)
        {
            return status;
        }
    #endif

        aescbcOp.iv = iv_array;

        ret    = AESCBC_oneStepEncrypt(aescbcHandle, &aescbcOp);
        status = map_AES_status(ret);

        /* Write the IV to output buffer */
        (void)memcpy(output, iv_array, ivSize);
#endif /* ENABLE_TI_CRYPTO_AESCBC */
    }
    else
    {
        (void)ret;
        status = PSA_ERROR_NOT_SUPPORTED;
    }

    if (status == PSA_SUCCESS)
    {
        *output_length = outputSize;
    }

    return status;
}

/*
 *  ======== psa_cipher_decrypt ========
 */
psa_status_t psa_cipher_decrypt(psa_key_id_t key,
                                psa_algorithm_t alg,
                                const uint8_t *input,
                                size_t input_length,
                                uint8_t *output,
                                size_t output_size,
                                size_t *output_length)
{
    CryptoKey cryptoKey;
    int_fast16_t ret;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_type_t symKeyType;
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    size_t blockLength;
    size_t ivSize;
    size_t keyBits;
    size_t payloadSize;

    *output_length = 0;

    /* The requested algorithm must be one that can be processed by cipher */
    if (!PSA_ALG_IS_CIPHER(alg))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&attributes, PSA_KEY_USAGE_DECRYPT, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    symKeyType  = psa_get_key_type(&attributes);
    ivSize      = PSA_CIPHER_IV_LENGTH(symKeyType, alg);
    payloadSize = PSA_CIPHER_DECRYPT_OUTPUT_SIZE(symKeyType, alg, input_length - ivSize);

    /* This is needed to check that there is an IV the input since if there is
     * none, the payloadSize will be a large unreasonable number due to previous
     * operation.
     */
    if (payloadSize > PSA_CIPHER_DECRYPT_OUTPUT_MAX_SIZE(input_length))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (output_size < payloadSize)
    {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    blockLength = PSA_BLOCK_CIPHER_BLOCK_LENGTH(symKeyType);
    /* If payload size is smaller than the actual block length, then there is a
     * problem if there is no padding.
     */
    if (payloadSize < blockLength)
    {
        /* If no padding is required, then input must be at least blockLength */
        if (alg == PSA_ALG_ECB_NO_PADDING || alg == PSA_ALG_CBC_NO_PADDING)
        {
            return PSA_ERROR_INVALID_ARGUMENT;
        }
    }

    keyBits = psa_get_key_bits(&attributes);
    KeyStore_PSA_initKey(&cryptoKey, toKeyStoreKeyID(key), PSA_BITS_TO_BYTES(keyBits), NULL);

    status = psa_check_aes_handle(alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    if (alg == PSA_ALG_ECB_NO_PADDING)
    {
#ifdef ENABLE_TI_CRYPTO_AESECB
        AESECB_Operation_init(&aesecbOp);
        aesecbOp.key         = &cryptoKey;
        aesecbOp.input       = (uint8_t *)(input + ivSize);
        aesecbOp.inputLength = input_length - ivSize;
        aesecbOp.output      = output;

        ret    = AESECB_oneStepDecrypt(aesecbHandle, &aesecbOp);
        status = map_AES_status(ret);
#endif
    }
    else if (alg == PSA_ALG_CTR)
    {
#ifdef ENABLE_TI_CRYPTO_AESCTR
        AESCTR_Operation_init(&aesctrOp);
        aesctrOp.key            = &cryptoKey;
        aesctrOp.input          = (uint8_t *)(input + ivSize);
        aesctrOp.inputLength    = input_length - ivSize;
        aesctrOp.output         = output;
        aesctrOp.initialCounter = input;

        ret    = AESCTR_oneStepDecrypt(aesctrHandle, &aesctrOp);
        status = map_AES_status(ret);
#endif
    }
    else if (alg == PSA_ALG_CBC_NO_PADDING)
    {
#ifdef ENABLE_TI_CRYPTO_AESCBC
        AESCBC_Operation_init(&aescbcOp);
        aescbcOp.key         = &cryptoKey;
        aescbcOp.input       = (uint8_t *)(input + ivSize);
        aescbcOp.inputLength = input_length - ivSize;
        aescbcOp.output      = output;
        aescbcOp.iv          = (uint8_t *)input;

        ret    = AESCBC_oneStepDecrypt(aescbcHandle, &aescbcOp);
        status = map_AES_status(ret);
#endif
    }
    else
    {
        (void)ret;
        status = PSA_ERROR_NOT_SUPPORTED;
    }

    if (status == PSA_SUCCESS)
    {
        *output_length = payloadSize;
    }

    return status;
}

/*
 *  ======== psa_cipher_setup ========
 */
static psa_status_t psa_cipher_setup(psa_cipher_operation_t *operation,
                                     psa_key_id_t key,
                                     psa_algorithm_t alg,
                                     aes_operation_t cipher_operation)
{
    int_fast16_t ret;
    psa_status_t status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_usage_t usage           = (cipher_operation == AES_ENCRYPT ? PSA_KEY_USAGE_ENCRYPT : PSA_KEY_USAGE_DECRYPT);

    status = psa_check_aes_handle(alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* A context must be freshly initialized before it can be set up */
    if ((operation->id != 0U) || (operation->in_error_state == 1U))
    {
        return psa_cipher_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    /* The requested algorithm must be one that can be processed by cipher */
    if (!PSA_ALG_IS_CIPHER(alg))
    {
        return psa_cipher_error(operation, PSA_ERROR_NOT_SUPPORTED, NULL);
    }

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return psa_cipher_error(operation, status, &attributes);
    }

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&attributes, usage, alg);
    if (status != PSA_SUCCESS)
    {
        return psa_cipher_error(operation, status, &attributes);
    }

    /* Added to validate the key type since PSA_KEY_TYPE_RAW_DATA is not allowed
     * and anything other than PSA_KEY_TYPE_AES is invalid.
     */
    psa_key_type_t keyType = psa_get_key_type(&attributes);
    if (keyType != PSA_KEY_TYPE_AES)
    {
        return psa_cipher_error(operation, PSA_ERROR_INVALID_ARGUMENT, &attributes);
    }

    /* Initialize the operation struct members, except for id. The id member is
     * used to indicate to psa_cipher_abort that there are resources to free, so
     * we only set it (in the driver wrapper) after resources have been
     * allocated/initialized.
     */
    operation->iv_set = 0U;
    if (alg == PSA_ALG_ECB_NO_PADDING)
    {
        operation->iv_required = 0U;
    }
    else
    {
        operation->iv_required = 1U;
    }

    operation->default_iv_length = PSA_CIPHER_IV_LENGTH(keyType, alg);

    operation->unprocessed_len       = 0;
    operation->curr_unprocessed_data = &operation->unprocessed_data[0];

    size_t keyBits = psa_get_key_bits(&attributes);
    KeyStore_PSA_initKey(&operation->cryptoKey, toKeyStoreKeyID(key), PSA_BITS_TO_BYTES(keyBits), NULL);

    /* Try doing the operation through a driver before using software fallback */
    if (cipher_operation == AES_ENCRYPT)
    {
        switch (alg)
        {
#ifdef ENABLE_TI_CRYPTO_AESECB
            case PSA_ALG_ECB_NO_PADDING:
                ret    = AESECB_setupEncrypt(aesecbHandle, &operation->cryptoKey);
                status = map_AES_status(ret);
                break;
#endif

#ifdef ENABLE_TI_CRYPTO_AESCBC
            case PSA_ALG_CBC_NO_PADDING:
                ret    = AESCBC_setupEncrypt(aescbcHandle, &operation->cryptoKey);
                status = map_AES_status(ret);
                break;
#endif

#ifdef ENABLE_TI_CRYPTO_AESCTR
            case PSA_ALG_CTR:
                ret    = AESCTR_setupEncrypt(aesctrHandle, &operation->cryptoKey, NULL);
                status = map_AES_status(ret);
                break;
#endif

            default:
                status = PSA_ERROR_NOT_SUPPORTED;
                break;
        }
    }
    else
    {
        switch (alg)
        {
#ifdef ENABLE_TI_CRYPTO_AESECB
            case PSA_ALG_ECB_NO_PADDING:
                ret    = AESECB_setupDecrypt(aesecbHandle, &operation->cryptoKey);
                status = map_AES_status(ret);
                break;
#endif

#ifdef ENABLE_TI_CRYPTO_AESCBC
            case PSA_ALG_CBC_NO_PADDING:
                ret    = AESCBC_setupDecrypt(aescbcHandle, &operation->cryptoKey);
                status = map_AES_status(ret);
                break;
#endif

#ifdef ENABLE_TI_CRYPTO_AESCTR
            case PSA_ALG_CTR:
                ret    = AESCTR_setupDecrypt(aesctrHandle, &operation->cryptoKey, NULL);
                status = map_AES_status(ret);
                break;
#endif

            default:
                (void)ret;
                status = PSA_ERROR_NOT_SUPPORTED;
                break;
        }
    }

    if (status != PSA_SUCCESS)
    {
        return psa_cipher_error(operation, status, &attributes);
    }

    operation->alg = alg;
    operation->id  = PSA_CRYPTO_TI_DRIVER;

    return status;
}

/*
 *  ======== psa_cipher_encrypt_setup ========
 */
psa_status_t psa_cipher_encrypt_setup(psa_cipher_operation_t *operation, psa_key_id_t key, psa_algorithm_t alg)
{
    operation->is_encrypt = 1U;
    return psa_cipher_setup(operation, key, alg, AES_ENCRYPT);
}

/*
 *  ======== psa_cipher_decrypt_setup ========
 */
psa_status_t psa_cipher_decrypt_setup(psa_cipher_operation_t *operation, psa_key_id_t key, psa_algorithm_t alg)
{
    operation->is_encrypt = 0U;
    return psa_cipher_setup(operation, key, alg, AES_DECRYPT);
}

/*
 *  ======== psa_cipher_generate_iv ========
 */
psa_status_t psa_cipher_generate_iv(psa_cipher_operation_t *operation, uint8_t *iv, size_t iv_size, size_t *iv_length)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

    *iv_length = 0;

    if ((operation->id == 0U) || (operation->in_error_state == 1))
    {
        return psa_cipher_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    if ((operation->iv_set == 1U) || (operation->iv_required == 0U))
    {
        return psa_cipher_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    if (iv_size < operation->default_iv_length)
    {
        return psa_cipher_error(operation, PSA_ERROR_BUFFER_TOO_SMALL, NULL);
    }

    status = psa_generate_random_internal(iv, operation->default_iv_length);
    if (status != PSA_SUCCESS)
    {
        return psa_cipher_error(operation, status, NULL);
    }

    status = psa_cipher_set_iv(operation, iv, operation->default_iv_length);
    if (status != PSA_SUCCESS)
    {
        return psa_cipher_error(operation, status, NULL);
    }

    operation->iv_set = 1U;
    *iv_length        = operation->default_iv_length;

    return PSA_SUCCESS;
}

/*
 *  ======== psa_cipher_set_iv ========
 */
psa_status_t psa_cipher_set_iv(psa_cipher_operation_t *operation, const uint8_t *iv, size_t iv_length)
{
    int_fast16_t ret;
    psa_status_t status;

    status = psa_check_aes_handle(operation->alg);
    if (status != PSA_SUCCESS)
    {
        return psa_cipher_error(operation, status, NULL);
    }

    if ((operation->id == 0U) || (operation->in_error_state == 1U))
    {
        return psa_cipher_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    if ((operation->iv_set == 1U) || (operation->iv_required == 0U))
    {
        return psa_cipher_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    if (iv_length > PSA_CIPHER_IV_MAX_SIZE)
    {
        return psa_cipher_error(operation, PSA_ERROR_INVALID_ARGUMENT, NULL);
    }

    if (iv_length < operation->default_iv_length)
    {
        return psa_cipher_error(operation, PSA_ERROR_INVALID_ARGUMENT, NULL);
    }

    switch (operation->alg)
    {
#ifdef ENABLE_TI_CRYPTO_AESCBC
        case PSA_ALG_CBC_NO_PADDING:
            ret    = AESCBC_setIV(aescbcHandle, iv, iv_length);
            status = map_AES_status(ret);
            break;
#endif

#ifdef ENABLE_TI_CRYPTO_AESCTR
        case PSA_ALG_CTR:
    #ifdef TFM_BUILD
            if (psa_crypto_ns_caller)
            {
                psa_aesctr_set_return_behavior(PSA_POLLING_MODE);
            }
    #endif
            ret    = AESCTR_cancelOperation(aesctrHandle);
            status = map_AES_status(ret);

    #ifdef TFM_BUILD
            if (psa_crypto_ns_caller)
            {
                psa_aesctr_set_return_behavior(PSA_CALLBACK_MODE);
            }
    #endif

            if (status == PSA_SUCCESS)
            {
                /* AESCTR driver does not have a setIV function, but the initial
                 * counter can be set with setupEncrypt.
                 */
                if (operation->is_encrypt == 1U)
                {
                    ret = AESCTR_setupEncrypt(aesctrHandle, &operation->cryptoKey, iv);
                }
                else
                {
                    ret = AESCTR_setupDecrypt(aesctrHandle, &operation->cryptoKey, iv);
                }

                status = map_AES_status(ret);
            }
            break;
#endif

        default:
            (void)ret;
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
    }

    if (status != PSA_SUCCESS)
    {
        return psa_cipher_error(operation, status, NULL);
    }

    operation->iv_set = 1U;
    return status;
}

/*
 * Helper functions for psa_cipher_update.
 */

/*
 *  ======== psa_aesAddBlock ========
 */
static psa_status_t psa_aesAddBlock(psa_algorithm_t alg,
                                    uint8_t numBlocks,
                                    const uint8_t *input,
                                    uint8_t *output,
                                    CryptoKey *cryptoKey,
                                    bool isFinalOp)
{
    int_fast16_t ret;
    psa_status_t status = PSA_SUCCESS;

    switch (alg)
    {
#ifdef ENABLE_TI_CRYPTO_AESECB
        case PSA_ALG_ECB_NO_PADDING:
            {
    #ifdef TFM_BUILD
                if (!isFinalOp && psa_crypto_ns_caller)
                {
                    psa_aesecb_set_return_behavior(PSA_POLLING_MODE);
                }
    #endif

                AESECB_Operation_init(&aesecbOp);
                aesecbOp.input       = (uint8_t *)input;
                aesecbOp.key         = cryptoKey;
                aesecbOp.inputLength = PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE * numBlocks;
                aesecbOp.output      = output;
                ret                  = AESECB_addData(aesecbHandle, &aesecbOp);
                status               = map_AES_status(ret);

    #ifdef TFM_BUILD
                if (!isFinalOp && psa_crypto_ns_caller)
                {
                    psa_aesecb_set_return_behavior(PSA_CALLBACK_MODE);
                }
    #endif
                break;
            }
#endif

#ifdef ENABLE_TI_CRYPTO_AESCTR
        case PSA_ALG_CTR:
            {
    #ifdef TFM_BUILD
                if (!isFinalOp && psa_crypto_ns_caller)
                {
                    psa_aesctr_set_return_behavior(PSA_POLLING_MODE);
                }
    #endif

                AESCTR_SegmentedOperation_init(&aesctrSegmentedOp);
                aesctrSegmentedOp.input       = (uint8_t *)input;
                aesctrSegmentedOp.inputLength = PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE * numBlocks;
                aesctrSegmentedOp.output      = output;
                ret                           = AESCTR_addData(aesctrHandle, &aesctrSegmentedOp);
                status                        = map_AES_status(ret);

    #ifdef TFM_BUILD
                if (!isFinalOp && psa_crypto_ns_caller)
                {
                    psa_aesctr_set_return_behavior(PSA_CALLBACK_MODE);
                }
    #endif
                break;
            }
#endif

#ifdef ENABLE_TI_CRYPTO_AESCBC
        case PSA_ALG_CBC_NO_PADDING:
            {
    #ifdef TFM_BUILD
                if (!isFinalOp && psa_crypto_ns_caller)
                {
                    psa_aescbc_set_return_behavior(PSA_POLLING_MODE);
                }
    #endif

                AESCBC_SegmentedOperation_init(&aescbcSegmentedOp);
                aescbcSegmentedOp.input       = (uint8_t *)input;
                aescbcSegmentedOp.inputLength = PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE * numBlocks;
                aescbcSegmentedOp.output      = output;
                ret                           = AESCBC_addData(aescbcHandle, &aescbcSegmentedOp);
                status                        = map_AES_status(ret);

    #ifdef TFM_BUILD
                if (!isFinalOp && psa_crypto_ns_caller)
                {
                    psa_aescbc_set_return_behavior(PSA_CALLBACK_MODE);
                }
    #endif
                break;
            }
#endif

        default:
            (void)ret;
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
    }

    return status;
}

/*
 *  ======== psa_aesAddDataNoPadding ========
 */
static psa_status_t psa_aesAddDataNoPadding(psa_cipher_operation_t *operation,
                                            const uint8_t *input,
                                            size_t input_length,
                                            uint8_t *output,
                                            size_t output_size)
{
    psa_status_t status = PSA_SUCCESS;
    bool isFinalOp      = true;

    /* First check if there is enough input data to process */
    if (input_length + operation->unprocessed_len < PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE)
    {
        /* Buffer for next time */
        (void)memcpy((operation->curr_unprocessed_data + operation->unprocessed_len), input, input_length);
        operation->unprocessed_len += input_length;
#ifdef TFM_BUILD
        return PSA_OPERATION_COMPLETE;
#else
        return PSA_SUCCESS;
#endif
    }

    /* If there is data to process, then first process buffered data */
    if (operation->unprocessed_len != 0)
    {
        /* Figure out how much more is needed to get to a block size */
        size_t copy_len = PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE - operation->unprocessed_len;
        (void)memcpy((operation->curr_unprocessed_data + operation->unprocessed_len), input, copy_len);

        if ((input_length - copy_len) >= PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE)
        {
            isFinalOp = false;
        }

        status = psa_aesAddBlock(operation->alg,
                                 1,
                                 operation->curr_unprocessed_data,
                                 output,
                                 &operation->cryptoKey,
                                 isFinalOp);

        /* Update outputs */
        output += PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE;
        /* Update inputs */
        input += copy_len;
        input_length -= copy_len;
        /* Reset to show buffer is empty */
        operation->unprocessed_len = 0;
    }

    /* Process all the remaining blocks of data */
    if ((status == PSA_SUCCESS) && (input_length >= PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE))
    {
        size_t remainingFullBlocks = (input_length / PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE);
        size_t dataToAdd           = remainingFullBlocks * PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE;

        isFinalOp = true;

        status = psa_aesAddBlock(operation->alg, remainingFullBlocks, input, output, &operation->cryptoKey, isFinalOp);
        /* Update inputs */
        input += dataToAdd;
        input_length -= dataToAdd;
    }

    /* Check if there is any data left to be buffered (data left should always
     * be < PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE).
     */
    if ((status == PSA_SUCCESS) && (input_length != 0))
    {
#ifdef TFM_BUILD
        /* Update the current pointer to unprocessed data to avoid potentially
         * modifying the input buffer before the crypto engine has processed it
         * when the SL driver is operating with callback return behavior.
         */
        if (operation->curr_unprocessed_data == &operation->unprocessed_data[0])
        {
            operation->curr_unprocessed_data = &operation->unprocessed_data[PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE];
        }
        else
        {
            operation->curr_unprocessed_data = &operation->unprocessed_data[0];
        }
#endif

        (void)memcpy(operation->curr_unprocessed_data, input, input_length);
        operation->unprocessed_len = input_length;
    }

    return status;
}

/*
 *  ======== psa_cipher_update ========
 */
psa_status_t psa_cipher_update(psa_cipher_operation_t *operation,
                               const uint8_t *input,
                               size_t input_length,
                               uint8_t *output,
                               size_t output_size,
                               size_t *output_length)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    size_t totalUnprocessedLen;

    *output_length = 0;

    if ((operation->id == 0U) || (operation->in_error_state == 1U))
    {
        return psa_cipher_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    if ((operation->iv_required == 1U) && (operation->iv_set == 0U))
    {
        return psa_cipher_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    if (operation->alg == 0)
    {
        return psa_cipher_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    /* The requested algorithm must be one that can be processed by cipher */
    if (!PSA_ALG_IS_CIPHER(operation->alg))
    {
        return psa_cipher_error(operation, PSA_ERROR_INVALID_ARGUMENT, NULL);
    }

    if (input_length == 0)
    {
#ifdef TFM_BUILD
        return PSA_OPERATION_COMPLETE;
#else
        return PSA_SUCCESS;
#endif
    }

    totalUnprocessedLen = operation->unprocessed_len + input_length;

    /* SL CBC, ECB, and CTR drivers only support adding full blocks of data */
    *output_length = (totalUnprocessedLen / PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE) * PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE;

    if (output_size < *output_length)
    {
        return psa_cipher_error(operation, PSA_ERROR_BUFFER_TOO_SMALL, NULL);
    }

    switch (operation->alg)
    {
        /* Note no padding means if the input is not BLOCK LENGTH, then the
         * input data needs to be buffered and processed when the data buffered
         * size = BLOCK LENGTH.
         */
        case PSA_ALG_ECB_NO_PADDING:
        case PSA_ALG_CTR:
        case PSA_ALG_CBC_NO_PADDING:
            status = psa_aesAddDataNoPadding(operation, input, input_length, output, output_size);
            break;

        default:
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
    }

    if ((status != PSA_SUCCESS) && (status != PSA_OPERATION_COMPLETE))
    {
        return psa_cipher_error(operation, status, NULL);
    }

    return status;
}

/*
 *  ======== psa_cipher_finish ========
 */
psa_status_t psa_cipher_finish(psa_cipher_operation_t *operation,
                               uint8_t *output,
                               size_t output_size,
                               size_t *output_length)
{
    int_fast16_t ret;
    psa_status_t status = PSA_ERROR_GENERIC_ERROR;

    status = psa_check_aes_handle(operation->alg);
    if (status != PSA_SUCCESS)
    {
        return psa_cipher_error(operation, status, NULL);
    }

    if ((operation->id == 0U) || (operation->in_error_state == 1U))
    {
        return psa_cipher_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    if ((operation->iv_required == 1U) && (operation->iv_set == 0U))
    {
        return psa_cipher_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    /* The requested algorithm must be one that can be processed by cipher */
    if (!PSA_ALG_IS_CIPHER(operation->alg))
    {
        return psa_cipher_error(operation, PSA_ERROR_INVALID_ARGUMENT, NULL);
    }

    /* True for ciphers supported/tested */
    *output_length = PSA_CIPHER_FINISH_OUTPUT_SIZE(PSA_KEY_TYPE_AES, operation->alg);
    if (output_size < *output_length)
    {
        return psa_cipher_error(operation, PSA_ERROR_BUFFER_TOO_SMALL, NULL);
    }

    if (operation->unprocessed_len != 0)
    {
        if ((operation->alg == PSA_ALG_ECB_NO_PADDING) || (operation->alg == PSA_ALG_CBC_NO_PADDING))
        {
            return psa_cipher_error(operation, PSA_ERROR_BAD_STATE, NULL);
        }
        else if (output_size < operation->unprocessed_len)
        {
            return psa_cipher_error(operation, PSA_ERROR_BUFFER_TOO_SMALL, NULL);
        }
    }

    switch (operation->alg)
    {
#ifdef ENABLE_TI_CRYPTO_AESECB
        case PSA_ALG_ECB_NO_PADDING:
            {
                AESECB_Operation_init(&aesecbOp);
                aesecbOp.output      = output;
                aesecbOp.key         = &operation->cryptoKey;
                aesecbOp.inputLength = 0;

                ret    = AESECB_finalize(aesecbHandle, &aesecbOp);
                status = map_AES_status(ret);
                break;
            }
#endif

#ifdef ENABLE_TI_CRYPTO_AESCTR
        case PSA_ALG_CTR:
            {
                /* CTR input length does not need to be a multiple of the block
                 * size, so there may be more input data left to process
                 */
                AESCTR_SegmentedOperation_init(&aesctrSegmentedOp);
                aesctrSegmentedOp.output      = output;
                aesctrSegmentedOp.input       = operation->curr_unprocessed_data;
                aesctrSegmentedOp.inputLength = operation->unprocessed_len;

                ret            = AESCTR_finalize(aesctrHandle, &aesctrSegmentedOp);
                status         = map_AES_status(ret);
                *output_length = operation->unprocessed_len;
                break;
            }
#endif

#ifdef ENABLE_TI_CRYPTO_AESCBC
        case PSA_ALG_CBC_NO_PADDING:
            {
                AESCBC_SegmentedOperation_init(&aescbcSegmentedOp);
                aescbcSegmentedOp.inputLength = 0;
                aescbcSegmentedOp.output      = output;

                ret    = AESCBC_finalize(aescbcHandle, &aescbcSegmentedOp);
                status = map_AES_status(ret);
                break;
            }
#endif
        default:
            (void)ret;
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
    }

#if (TFM_ENABLED == 1) && (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)
    /* On tfm_enabled, there is an issue with AESCBC_finalize returning an error code
     * even when the operation completed successfully. In those cases, abort the operation
     * and return psa_success.
     */
    if ((operation->alg == PSA_ALG_CBC_NO_PADDING) && (status == PSA_ERROR_GENERIC_ERROR))
    {
        psa_cipher_abort(operation);
        status = PSA_SUCCESS;
    }
#endif

    if (status != PSA_SUCCESS)
    {
        *output_length = 0;
        return psa_cipher_error(operation, status, NULL);
    }

    return PSA_SUCCESS;
}

/*
 *  ======== psa_cipher_abort ========
 */
psa_status_t psa_cipher_abort(psa_cipher_operation_t *operation)
{
    psa_status_t status = PSA_SUCCESS;

    if (operation->id == 0U)
    {
        /* The object has (apparently) been initialized but it is not (yet)
         * in use. It's ok to call abort on such an object, and there's
         * nothing to do.
         */
        return PSA_SUCCESS;
    }

    status = psa_check_aes_handle(operation->alg);
    if (status != PSA_SUCCESS)
    {
        return psa_cipher_error(operation, status, NULL);
    }

    switch (operation->alg)
    {
#ifdef ENABLE_TI_CRYPTO_AESECB
        case PSA_ALG_ECB_NO_PADDING:
            /* Cancel operation is not supported in polling mode for AESECB,
             * workaround is to close and reopen the driver instance.
             */
            AESECB_close(aesecbHandle);

    #ifdef TFM_BUILD
            psa_aesecb_construct(PSA_CALLBACK_MODE);
    #else
            psa_aesecb_construct(PSA_BLOCKING_MODE);
    #endif
            break;
#endif

#ifdef ENABLE_TI_CRYPTO_AESCTR
        case PSA_ALG_CTR:
            {
                int_fast16_t ret;

    #ifdef TFM_BUILD
                if (psa_crypto_ns_caller)
                {
                    psa_aesctr_set_return_behavior(PSA_POLLING_MODE);
                }
    #endif
                ret    = AESCTR_cancelOperation(aesctrHandle);
                status = map_AES_status(ret);

    #ifdef TFM_BUILD
                if (psa_crypto_ns_caller)
                {
                    psa_aesctr_set_return_behavior(PSA_CALLBACK_MODE);
                }
    #endif
                break;
            }
#endif

#ifdef ENABLE_TI_CRYPTO_AESCBC
        case PSA_ALG_CBC_NO_PADDING:
            /* Cancel operation is not supported in polling mode for AESCBC,
             * workaround is to close and reopen the driver instance.
             */
            AESCBC_close(aescbcHandle);

    #ifdef TFM_BUILD
            psa_aescbc_construct(PSA_CALLBACK_MODE);
    #else
            psa_aescbc_construct(PSA_BLOCKING_MODE);
    #endif

            break;
#endif

        default:
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
    }

    *operation = PSA_CIPHER_OPERATION_INIT;

    if (status != PSA_SUCCESS)
    {
        return psa_cipher_error(operation, status, NULL);
    }

    return status;
}

/******************************************************************************/
/* Authenticated Encryption with Associated Data (AEAD) */
/******************************************************************************/

/*
 *  ======== psa_aead_error ========
 *  Puts the operation into an error state, resets key attributes if necessary,
 *  and spits back out the error code to be returned to the application.
 */
static psa_status_t psa_aead_error(psa_aead_operation_t *operation,
                                   psa_status_t error,
                                   psa_key_attributes_t *attributes)
{
    operation->in_error_state = 1U;

    if (attributes != NULL)
    {
        psa_reset_key_attributes(attributes);
    }

    return error;
}

/*
 *  ======== psa_aead_set_lengths_internal ========
 */
static psa_status_t psa_aead_set_lengths_internal(psa_aead_operation_t *operation,
                                                  size_t ad_length,
                                                  size_t plaintext_length)
{
    int_fast16_t ret;
    psa_status_t status;

    switch (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg))
    {
#ifdef ENABLE_TI_CRYPTO_AESCCM
        case PSA_ALG_CCM:
            ret    = AESCCM_setLengths(aesccmHandle, ad_length, plaintext_length, operation->tagSize);
            status = map_AES_status(ret);
            break;
#endif

#ifdef ENABLE_TI_CRYPTO_AESGCM
        case PSA_ALG_GCM:
            ret    = AESGCM_setLengths(aesgcmHandle, ad_length, plaintext_length);
            status = map_AES_status(ret);
            break;
#endif

        default:
            (void)ret;
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
    }

    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, NULL);
    }

    return PSA_SUCCESS;
}

/*
 *  ======== psa_aead_set_lengths ========
 */
psa_status_t psa_aead_set_lengths(psa_aead_operation_t *operation, size_t ad_length, size_t plaintext_length)
{
    if ((operation->id == 0U) || (operation->iv_set == 1U) || (operation->in_error_state == 1U))
    {
        return psa_aead_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    operation->length_set      = true;
    operation->adLength        = ad_length;
    operation->plaintextLength = plaintext_length;

    return psa_aead_set_lengths_internal(operation, ad_length, plaintext_length);
}

/******************************************************************************/
/* Multi-step AEAD operations */
/******************************************************************************/

/*
 *  ======== psa_aead_encrypt_setup ========
 */
psa_status_t psa_aead_encrypt_setup(psa_aead_operation_t *operation, psa_key_id_t key, psa_algorithm_t alg)
{
    int_fast16_t ret;
    psa_status_t status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;

    status = psa_check_aes_handle(PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg));
    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, NULL);
    }

    if ((operation->id != 0) || (operation->in_error_state == 1U))
    {
        return psa_aead_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    /* Check if the algorithm is part of the AEAD (auth encryptions with associated data) */
    if (!(PSA_ALG_IS_AEAD(alg)))
    {
        return psa_aead_error(operation, PSA_ERROR_INVALID_ARGUMENT, NULL);
    }

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, &attributes);
    }

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&attributes, PSA_KEY_USAGE_ENCRYPT, alg);
    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, &attributes);
    }

    /* Call the hardware related operations */
    size_t keyBits = psa_get_key_bits(&attributes);
    size_t keyType = psa_get_key_type(&attributes);
    KeyStore_PSA_initKey(&operation->cryptoKey, toKeyStoreKeyID(key), PSA_BITS_TO_BYTES(keyBits), NULL);

    size_t tagLength = PSA_AEAD_TAG_LENGTH(keyType, keyBits, alg);

    /* True for supported AEAD */
    if (PSA_BLOCK_CIPHER_BLOCK_LENGTH(keyType) != PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE)
    {
        return psa_aead_error(operation, PSA_ERROR_INVALID_ARGUMENT, &attributes);
    }

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg) == PSA_ALG_CCM)
    {
#ifdef ENABLE_TI_CRYPTO_AESCCM
        /* Check for correct tag size. Test assumes tagLength=0 is invalid */
        status = psa_validate_ccm_tag_length(tagLength);

        if (status == PSA_SUCCESS)
        {
            ret    = AESCCM_setupEncrypt(aesccmHandle, &operation->cryptoKey, 0, 0, tagLength);
            status = map_AES_status(ret);
        }
#endif
    }
    else if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg) == PSA_ALG_GCM)
    {
#ifdef ENABLE_TI_CRYPTO_AESGCM
        /* Check for correct tag size. Test assumes tagLength=0 is invalid */
        status = psa_validate_gcm_tag_length(tagLength);

        if (status == PSA_SUCCESS)
        {
            ret    = AESGCM_setupEncrypt(aesgcmHandle, &operation->cryptoKey, 0, tagLength);
            status = map_AES_status(ret);
        }
#endif
    }
    else
    {
        (void)ret;
        /* Do nothing */
    }

    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, &attributes);
    }

    operation->alg                   = alg;
    operation->tagSize               = tagLength;
    operation->id                    = PSA_CRYPTO_TI_DRIVER;
    operation->curr_unprocessed_data = &operation->unprocessedData[0];

    return PSA_SUCCESS;
}

/*
 *  ======== psa_aead_decrypt_setup ========
 */
psa_status_t psa_aead_decrypt_setup(psa_aead_operation_t *operation, psa_key_id_t key, psa_algorithm_t alg)
{
    int_fast16_t ret;
    psa_status_t status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;

    status = psa_check_aes_handle(PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg));
    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, NULL);
    }

    if ((operation->id != 0) || (operation->in_error_state == 1U))
    {
        return psa_aead_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    /* Check if the algorithm is part of the AEAD (auth encryptions with associated data) */
    if (!(PSA_ALG_IS_AEAD(alg)))
    {
        return psa_aead_error(operation, PSA_ERROR_INVALID_ARGUMENT, NULL);
    }

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, &attributes);
    }

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&attributes, PSA_KEY_USAGE_DECRYPT, alg);
    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, &attributes);
    }

    /* Call the hardware related operations */
    size_t keyBits = psa_get_key_bits(&attributes);
    size_t keyType = psa_get_key_type(&attributes);
    KeyStore_PSA_initKey(&operation->cryptoKey, toKeyStoreKeyID(key), PSA_BITS_TO_BYTES(keyBits), NULL);

    size_t tagLength = PSA_AEAD_TAG_LENGTH(keyType, keyBits, alg);

    /* True for supported AEAD */
    if (PSA_BLOCK_CIPHER_BLOCK_LENGTH(keyType) != PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE)
    {
        return psa_aead_error(operation, PSA_ERROR_INVALID_ARGUMENT, &attributes);
    }

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg) == PSA_ALG_CCM)
    {
#ifdef ENABLE_TI_CRYPTO_AESCCM
        /* Check for correct tag size. Test assumes tagLength=0 is invalid */
        status = psa_validate_ccm_tag_length(tagLength);

        if (status == PSA_SUCCESS)
        {
            ret    = AESCCM_setupDecrypt(aesccmHandle, &operation->cryptoKey, 0, 0, tagLength);
            status = map_AES_status(ret);
        }
#endif
    }
    else if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg) == PSA_ALG_GCM)
    {
#ifdef ENABLE_TI_CRYPTO_AESGCM
        /* Check for correct tag size. Test assumes tagLength=0 is invalid */
        status = psa_validate_gcm_tag_length(tagLength);

        if (status == PSA_SUCCESS)
        {
            ret    = AESGCM_setupDecrypt(aesgcmHandle, &operation->cryptoKey, 0, 0);
            status = map_AES_status(ret);
        }
#endif
    }
    else
    {
        (void)ret;
        /* Do nothing */
    }

    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, &attributes);
    }

    operation->alg                   = alg;
    operation->tagSize               = tagLength;
    operation->id                    = PSA_CRYPTO_TI_DRIVER;
    operation->curr_unprocessed_data = &operation->unprocessedData[0];

    return PSA_SUCCESS;
}

/*
 *  ======== psa_aead_update_ad_internal ========
 *  Uses the appropriate driver depending on the alg to update the additional
 *  data. Unless it is the final call to this function for the operation,
 *  input_length must be a multiple of the block size.
 */
static psa_status_t psa_aead_update_ad_internal(psa_algorithm_t alg,
                                                const uint8_t *input,
                                                size_t input_length,
                                                bool isFinalOp)
{
    int_fast16_t ret;
    psa_status_t status;

    if (input_length == 0)
    {
        return PSA_SUCCESS;
    }

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg) == PSA_ALG_CCM)
    {
#ifdef ENABLE_TI_CRYPTO_AESCCM

    #ifdef TFM_BUILD
        if (!isFinalOp && psa_crypto_ns_caller)
        {
            psa_aesccm_set_return_behavior(PSA_POLLING_MODE);
        }
    #endif

        AESCCM_SegmentedAADOperation_init(&aesccmSegmentedAadOp);
        aesccmSegmentedAadOp.aad       = (uint8_t *)input;
        aesccmSegmentedAadOp.aadLength = input_length;

        ret    = AESCCM_addAAD(aesccmHandle, &aesccmSegmentedAadOp);
        status = map_AES_status(ret);

    #ifdef TFM_BUILD
        if (!isFinalOp && psa_crypto_ns_caller)
        {
            psa_aesccm_set_return_behavior(PSA_CALLBACK_MODE);
        }
    #endif
#endif
    }
    else if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg) == PSA_ALG_GCM)
    {
#ifdef ENABLE_TI_CRYPTO_AESGCM

    #ifdef TFM_BUILD
        if (!isFinalOp && psa_crypto_ns_caller)
        {
            psa_aesgcm_set_return_behavior(PSA_POLLING_MODE);
        }
    #endif

        AESGCM_SegmentedAADOperation_init(&aesgcmSegmentedAadOp);
        aesgcmSegmentedAadOp.aad       = (uint8_t *)input;
        aesgcmSegmentedAadOp.aadLength = input_length;

        ret    = AESGCM_addAAD(aesgcmHandle, &aesgcmSegmentedAadOp);
        status = map_AES_status(ret);

    #ifdef TFM_BUILD
        if (!isFinalOp && psa_crypto_ns_caller)
        {
            psa_aesgcm_set_return_behavior(PSA_CALLBACK_MODE);
        }
    #endif
#endif
    }
    else
    {
        (void)ret;
        /* Do nothing */
    }

    return status;
}

/*
 *  ======== psa_aead_update_internal ========
 *  Uses the appropriate driver depending on the alg to pass additional data to
 *  the operation. Unless it is the final call to this function for the
 *  operation, input_length must be a multiple of the block size.
 */
static psa_status_t psa_aead_update_internal(psa_algorithm_t alg,
                                             const uint8_t *input,
                                             size_t input_length,
                                             uint8_t *output,
                                             bool isFinalOp)
{
    int_fast16_t ret;
    psa_status_t status;

    if (input_length == 0)
    {
        return PSA_SUCCESS;
    }

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg) == PSA_ALG_CCM)
    {
#ifdef ENABLE_TI_CRYPTO_AESCCM

    #ifdef TFM_BUILD
        if (!isFinalOp && psa_crypto_ns_caller)
        {
            psa_aesccm_set_return_behavior(PSA_POLLING_MODE);
        }
    #endif

        AESCCM_SegmentedDataOperation_init(&aesccmSegmentedDataOp);

        aesccmSegmentedDataOp.input       = (uint8_t *)input;
        aesccmSegmentedDataOp.output      = output;
        aesccmSegmentedDataOp.inputLength = input_length;

        ret    = AESCCM_addData(aesccmHandle, &aesccmSegmentedDataOp);
        status = map_AES_status(ret);

    #ifdef TFM_BUILD
        if (!isFinalOp && psa_crypto_ns_caller)
        {
            psa_aesccm_set_return_behavior(PSA_CALLBACK_MODE);
        }
    #endif
#endif
    }
    else if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg) == PSA_ALG_GCM)
    {
#ifdef ENABLE_TI_CRYPTO_AESGCM

    #ifdef TFM_BUILD
        if (!isFinalOp && psa_crypto_ns_caller)
        {
            psa_aesgcm_set_return_behavior(PSA_POLLING_MODE);
        }
    #endif

        AESGCM_SegmentedDataOperation_init(&aesgcmSegmentedDataOp);
        aesgcmSegmentedDataOp.input       = (uint8_t *)input;
        aesgcmSegmentedDataOp.output      = output;
        aesgcmSegmentedDataOp.inputLength = input_length;

        ret    = AESGCM_addData(aesgcmHandle, &aesgcmSegmentedDataOp);
        status = map_AES_status(ret);

    #ifdef TFM_BUILD
        if (!isFinalOp && psa_crypto_ns_caller)
        {
            psa_aesgcm_set_return_behavior(PSA_CALLBACK_MODE);
        }
    #endif
#endif
    }
    else
    {
        (void)ret;
        /* Do nothing */
    }

    return status;
}

/*
 *  ======== psa_aead_update ========
 */
psa_status_t psa_aead_update(psa_aead_operation_t *operation,
                             const uint8_t *input,
                             size_t input_length,
                             uint8_t *output,
                             size_t output_size,
                             size_t *output_length)
{
    bool isFinalOp      = false;
    psa_status_t status = PSA_SUCCESS;
    size_t totalUnprocessedLen;

    *output_length = 0;

    /* Per PSA spec, calling psa_aead_set_lengths() is optional for GCM.
     * However, our SL GCM driver requires the lengths to be set prior to
     * processing any data so an error must be returned.
     */
    if (!operation->length_set)
    {
        return psa_aead_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    if ((operation->id == 0U) || (operation->in_error_state == 1U) || (operation->iv_set == 0U))
    {
        return psa_aead_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    status = psa_check_aes_handle(PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg));
    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, NULL);
    }

    operation->runningPlaintextLength += input_length;

    if (operation->length_set && ((operation->runningADLength < operation->adLength) ||
                                  (operation->runningPlaintextLength > operation->plaintextLength)))
    {
        return psa_aead_error(operation, PSA_ERROR_INVALID_ARGUMENT, NULL);
    }

    if (!operation->done_updating_ad)
    {
        /* Add any remaining additional data */
        status = psa_aead_update_ad_internal(operation->alg,
                                             operation->curr_unprocessed_data,
                                             operation->unprocessed_len,
                                             isFinalOp);
        if (status != PSA_SUCCESS)
        {
            return psa_aead_error(operation, status, NULL);
        }

        operation->done_updating_ad = true;
        operation->unprocessed_len  = 0;
    }

    totalUnprocessedLen = operation->unprocessed_len + input_length;

    /* SL CCM and GCM drivers only support adding full blocks of data */
    *output_length = (totalUnprocessedLen / PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE) * PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE;

    if (output_size < *output_length)
    {
        return psa_aead_error(operation, PSA_ERROR_BUFFER_TOO_SMALL, NULL);
    }

    /* Not enough total data to make up one block */
    if (totalUnprocessedLen < PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE)
    {
        /* Buffer the data */
        (void)memcpy(operation->curr_unprocessed_data + operation->unprocessed_len, input, input_length);
        operation->unprocessed_len = totalUnprocessedLen;

#ifdef TFM_BUILD
        return PSA_OPERATION_COMPLETE;
#else
        return PSA_SUCCESS;
#endif
    }

    if (operation->unprocessed_len > 0)
    {
        /* Enough data to make up at least a single block. Append enough input data
         * to the existing unprocessed data to build a full block.
         */
        size_t amountToCopy = PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE - operation->unprocessed_len;
        (void)memcpy(operation->curr_unprocessed_data + operation->unprocessed_len, input, amountToCopy);
        input += amountToCopy;

        /* If there will be less than a full block remaining, then this is final
         * operation to be issued to the SL driver for this call to psa_aead_update().
         */
        if ((totalUnprocessedLen - PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE) < PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE)
        {
            isFinalOp = true;
        }

        status = psa_aead_update_internal(operation->alg,
                                          operation->curr_unprocessed_data,
                                          PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE,
                                          output,
                                          isFinalOp);

        if (status != PSA_SUCCESS)
        {
            return psa_aead_error(operation, status, NULL);
        }

        output += PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE;
        totalUnprocessedLen -= PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE;
        operation->unprocessed_len = 0;

#ifdef TFM_BUILD
        /* Update the current pointer to unprocessed data to avoid potentially
         * modifying the input buffer before the crypto engine has processed it
         * when the SL driver is operating with callback return behavior.
         */
        if (operation->curr_unprocessed_data == &operation->unprocessedData[0])
        {
            operation->curr_unprocessed_data = &operation->unprocessedData[PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE];
        }
        else
        {
            operation->curr_unprocessed_data = &operation->unprocessedData[0];
        }
#endif
    }

    /* Get the largest block size multiple of the remaining data to process */
    size_t amountToAdd = (totalUnprocessedLen / PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE) * PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE;

    if (amountToAdd > 0)
    {
        isFinalOp = true;

        status = psa_aead_update_internal(operation->alg, input, amountToAdd, output, isFinalOp);
        if (status != PSA_SUCCESS)
        {
            return psa_aead_error(operation, status, NULL);
        }

        totalUnprocessedLen -= amountToAdd;
        input += amountToAdd;
        output += amountToAdd;
    }

    /* Add any left over data to the unprocessed data buffer */
    if (totalUnprocessedLen > 0)
    {
        (void)memcpy(operation->curr_unprocessed_data, input, totalUnprocessedLen);
        operation->unprocessed_len = totalUnprocessedLen;
    }

    return PSA_SUCCESS;
}

/*
 *  ======== psa_aead_update_ad ========
 */
psa_status_t psa_aead_update_ad(psa_aead_operation_t *operation, const uint8_t *input, size_t input_length)
{
    bool isFinalOp             = false;
    psa_status_t status        = PSA_SUCCESS;
    size_t totalUnprocessedLen = operation->unprocessed_len + input_length;

    /* Per PSA spec, calling psa_aead_set_lengths() is optional for GCM.
     * However, our SL GCM driver requires the lengths to be set prior to
     * processing any data so an error must be returned.
     */
    if (!operation->length_set)
    {
        return psa_aead_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    status = psa_check_aes_handle(PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg));
    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, NULL);
    }

    if ((operation->id == 0U) || (operation->in_error_state == 1U) || (operation->iv_set == 0U) ||
        operation->done_updating_ad)
    {
        return psa_aead_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    operation->runningADLength += input_length;

    if (operation->length_set && (operation->runningADLength > operation->adLength))
    {
        return psa_aead_error(operation, PSA_ERROR_INVALID_ARGUMENT, NULL);
    }

    /* Not enough total data to make up one block */
    if (totalUnprocessedLen < PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE)
    {
        /* Buffer the data and wait for more data to arrive */
        (void)memcpy(operation->curr_unprocessed_data + operation->unprocessed_len, input, input_length);
        operation->unprocessed_len = totalUnprocessedLen;
#ifdef TFM_BUILD
        return PSA_OPERATION_COMPLETE;
#else
        return PSA_SUCCESS;
#endif
    }

    if (operation->unprocessed_len > 0)
    {
        /* Create a full block of unprocessed data to add */
        size_t amountToCopy = PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE - operation->unprocessed_len;
        (void)memcpy(operation->curr_unprocessed_data + operation->unprocessed_len, input, amountToCopy);
        input += amountToCopy;

        if ((totalUnprocessedLen - PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE) == 0)
        {
            isFinalOp = true;
        }

        status = psa_aead_update_ad_internal(operation->alg,
                                             operation->curr_unprocessed_data,
                                             PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE,
                                             isFinalOp);
        if (status != PSA_SUCCESS)
        {
            return psa_aead_error(operation, status, NULL);
        }

        totalUnprocessedLen -= PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE;
        operation->unprocessed_len = 0;

#ifdef TFM_BUILD
        /* Update the current pointer to unprocessed data to avoid potentially
         * modifying the input buffer before the crypto engine has processed it
         * when the SL driver is operating with callback return behavior.
         */
        if (operation->curr_unprocessed_data == &operation->unprocessedData[0])
        {
            operation->curr_unprocessed_data = &operation->unprocessedData[PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE];
        }
        else
        {
            operation->curr_unprocessed_data = &operation->unprocessedData[0];
        }
#endif
    }

    /* Get the largest multiple of the block size that is less than the number
     * of bytes of unprocessed data.
     */
    size_t amountToAdd = ((int)(totalUnprocessedLen / PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE)) *
                         PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE;

    if (amountToAdd > 0)
    {
        isFinalOp = true;

        status = psa_aead_update_ad_internal(operation->alg, input, amountToAdd, isFinalOp);
        if (status != PSA_SUCCESS)
        {
            return psa_aead_error(operation, status, NULL);
        }
        totalUnprocessedLen -= amountToAdd;
        input += amountToAdd;
    }

    /* Add any left over data to the unprocessed data buffer */
    if (totalUnprocessedLen > 0)
    {
        (void)memcpy(operation->curr_unprocessed_data, input, totalUnprocessedLen);
        operation->unprocessed_len = totalUnprocessedLen;
    }

    return PSA_SUCCESS;
}

/*
 *  ======== psa_aead_set_nonce ========
 */
psa_status_t psa_aead_set_nonce(psa_aead_operation_t *operation, const uint8_t *nonce, size_t nonce_length)
{
    int_fast16_t ret;
    psa_status_t status;

    status = psa_check_aes_handle(PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg));
    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, NULL);
    }

    if ((operation->id == 0U) || (operation->iv_set == 1U) || (operation->in_error_state == 1U))
    {
        return psa_aead_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg) == PSA_ALG_CCM)
    {
#ifdef ENABLE_TI_CRYPTO_AESCCM
        /* CCM requires that the length be set before setting the nonce */
        if (!operation->length_set)
        {
            status = PSA_ERROR_BAD_STATE;
        }
        else
        {
            /* Verify proper nonce length */
            if ((nonce_length < 7) || (nonce_length > 13))
            {
                status = PSA_ERROR_INVALID_ARGUMENT;
            }
            else
            {
                ret    = AESCCM_setNonce(aesccmHandle, nonce, nonce_length);
                status = map_AES_status(ret);
            }
        }
#endif
    }
    else if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg) == PSA_ALG_GCM)
    {
#ifdef ENABLE_TI_CRYPTO_AESGCM
        /* Verify proper nonce length */
        if ((nonce_length == 0U) || (nonce_length > PSA_AEAD_NONCE_MAX_SIZE))
        {
            status = PSA_ERROR_INVALID_ARGUMENT;
        }
        else
        {
            /* Currently the AESGCM driver only supports nonce lengths of 12 */
            ret    = AESGCM_setIV(aesgcmHandle, nonce, nonce_length);
            status = map_AES_status(ret);
        }
#endif
    }
    else
    {
        (void)ret;
        /* Do nothing */
    }

    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, NULL);
    }

    operation->iv_set = 1U;

    return PSA_SUCCESS;
}

/*
 *  ======== psa_aead_generate_nonce ========
 */
psa_status_t psa_aead_generate_nonce(psa_aead_operation_t *operation,
                                     uint8_t *nonce,
                                     size_t nonce_size,
                                     size_t *nonce_length)
{
    psa_status_t status;
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    uint8_t buffer[16]; /* word multiple buffer as required by HSM */
#endif

    if ((operation->id == 0U) || (operation->iv_set == 1U) || (operation->in_error_state == 1U))
    {
        return psa_aead_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    if ((PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg) == PSA_ALG_CCM) && !operation->length_set)
    {
        return psa_aead_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    /* For CCM, this size may not be correct according to the PSA
     * specification. The PSA Crypto 1.0.1 specification states:
     *
     * CCM encodes the plaintext length pLen in L octets, with L the smallest
     * integer >= 2 where pLen < 2^(8L). The nonce length is then 15 - L bytes.
     *
     * However this restriction that L has to be the smallest integer is not
     * applied in practice, and it is not implementable here since the
     * plaintext length may or may not be known at this time.
     */
    *nonce_length = PSA_AEAD_NONCE_LENGTH(PSA_KEY_TYPE_AES, operation->alg);

    if (*nonce_length > nonce_size)
    {
        return psa_aead_error(operation, PSA_ERROR_BUFFER_TOO_SMALL, NULL);
    }

#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    status = psa_generate_random_internal(buffer, sizeof(buffer));
#else
    status = psa_generate_random_internal(nonce, *nonce_length);
#endif

    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, NULL);
    }

#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    (void)memcpy(nonce, buffer, *nonce_length);
#endif

    return psa_aead_set_nonce(operation, nonce, *nonce_length);
}

/*
 *  ======== psa_aead_finish ========
 */
psa_status_t psa_aead_finish(psa_aead_operation_t *operation,
                             uint8_t *ciphertext,
                             size_t ciphertext_size,
                             size_t *ciphertext_length,
                             uint8_t *tag,
                             size_t tag_size,
                             size_t *tag_length)
{
    int_fast16_t ret;
    psa_status_t status = PSA_SUCCESS;

    *ciphertext_length = 0;

    status = psa_check_aes_handle(PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg));
    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, NULL);
    }

    if ((operation->id == 0U) || (operation->iv_set == 0U) || (operation->in_error_state == 1U))
    {
        return psa_aead_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    if (ciphertext_size < operation->unprocessed_len)
    {
        return psa_aead_error(operation, PSA_ERROR_BUFFER_TOO_SMALL, NULL);
    }

    if (tag_size < operation->tagSize)
    {
        return psa_aead_error(operation, PSA_ERROR_BUFFER_TOO_SMALL, NULL);
    }

    if (operation->length_set && ((operation->runningADLength < operation->adLength) ||
                                  (operation->runningPlaintextLength < operation->plaintextLength)))
    {
        return psa_aead_error(operation, PSA_ERROR_INVALID_ARGUMENT, NULL);
    }

    /* To handle the case when update is never called because there is zero
     * plaintext, but there is still additional data that has not been
     * processed.
     */
    if (!operation->done_updating_ad)
    {
        status = psa_aead_update_ad_internal(operation->alg,
                                             operation->curr_unprocessed_data,
                                             operation->unprocessed_len,
                                             false);
        if (status != PSA_SUCCESS)
        {
            return psa_aead_error(operation, status, NULL);
        }

        operation->done_updating_ad = true;
        operation->unprocessed_len  = 0;
    }

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg) == PSA_ALG_CCM)
    {
#ifdef ENABLE_TI_CRYPTO_AESCCM
        AESCCM_SegmentedFinalizeOperation_init(&aesccmSegmentedFinalizeOp);
        aesccmSegmentedFinalizeOp.input       = operation->curr_unprocessed_data;
        aesccmSegmentedFinalizeOp.output      = ciphertext;
        aesccmSegmentedFinalizeOp.inputLength = operation->unprocessed_len;
        aesccmSegmentedFinalizeOp.mac         = tag;
        aesccmSegmentedFinalizeOp.macLength   = operation->tagSize;

        ret                        = AESCCM_finalizeEncrypt(aesccmHandle, &aesccmSegmentedFinalizeOp);
        *tag_length                = aesccmSegmentedFinalizeOp.macLength;
        *ciphertext_length         = aesccmSegmentedFinalizeOp.inputLength;
        operation->unprocessed_len = 0;

        status = map_AES_status(ret);
#endif
    }
    else if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg) == PSA_ALG_GCM)
    {
#ifdef ENABLE_TI_CRYPTO_AESGCM
        AESGCM_SegmentedFinalizeOperation_init(&aesgcmSegmentedFinalizeOp);
        aesgcmSegmentedFinalizeOp.input       = operation->curr_unprocessed_data;
        aesgcmSegmentedFinalizeOp.output      = ciphertext;
        aesgcmSegmentedFinalizeOp.inputLength = operation->unprocessed_len;
        aesgcmSegmentedFinalizeOp.mac         = tag;
        aesgcmSegmentedFinalizeOp.macLength   = operation->tagSize;

        ret                        = AESGCM_finalizeEncrypt(aesgcmHandle, &aesgcmSegmentedFinalizeOp);
        *tag_length                = aesgcmSegmentedFinalizeOp.macLength;
        *ciphertext_length         = aesgcmSegmentedFinalizeOp.inputLength;
        operation->unprocessed_len = 0;

        status = map_AES_status(ret);
#endif
    }
    else
    {
        (void)ret;
        /* Do nothing */
    }

    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, NULL);
    }

    return PSA_SUCCESS;
}

/*
 *  ======== psa_aead_verify ========
 */
psa_status_t psa_aead_verify(psa_aead_operation_t *operation,
                             uint8_t *plaintext,
                             size_t plaintext_size,
                             size_t *plaintext_length,
                             const uint8_t *tag,
                             size_t tag_length)

{
    int_fast16_t ret;
    psa_status_t status = PSA_SUCCESS;

    *plaintext_length = 0;

    status = psa_check_aes_handle(PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg));
    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, NULL);
    }

    if ((operation->id == 0U) || (operation->iv_set == 0U) || (operation->in_error_state == 1U))
    {
        return psa_aead_error(operation, PSA_ERROR_BAD_STATE, NULL);
    }

    if (plaintext_size < operation->unprocessed_len)
    {
        return psa_aead_error(operation, PSA_ERROR_BUFFER_TOO_SMALL, NULL);
    }

    if (operation->length_set && ((operation->runningADLength < operation->adLength) ||
                                  (operation->runningPlaintextLength < operation->plaintextLength)))
    {
        return psa_aead_error(operation, PSA_ERROR_INVALID_ARGUMENT, NULL);
    }

    if ((PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg) == PSA_ALG_CCM) && (operation->tagSize != tag_length))
    {
        return psa_aead_error(operation, PSA_ERROR_INVALID_ARGUMENT, NULL);
    }

    /* To handle the case when update is never called because there is zero
     * ciphertext, but there is still additional data that has not been
     * processed.
     */
    if (!operation->done_updating_ad)
    {
        status = psa_aead_update_ad_internal(operation->alg,
                                             operation->curr_unprocessed_data,
                                             operation->unprocessed_len,
                                             false);
        if (status != PSA_SUCCESS)
        {
            return psa_aead_error(operation, status, NULL);
        }

        operation->done_updating_ad = true;
        operation->unprocessed_len  = 0;
    }

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg) == PSA_ALG_CCM)
    {
#ifdef ENABLE_TI_CRYPTO_AESCCM
        AESCCM_SegmentedFinalizeOperation_init(&aesccmSegmentedFinalizeOp);
        aesccmSegmentedFinalizeOp.input       = operation->curr_unprocessed_data;
        aesccmSegmentedFinalizeOp.output      = plaintext;
        aesccmSegmentedFinalizeOp.inputLength = operation->unprocessed_len;
        aesccmSegmentedFinalizeOp.mac         = (uint8_t *)tag;
        aesccmSegmentedFinalizeOp.macLength   = operation->tagSize;

        ret               = AESCCM_finalizeDecrypt(aesccmHandle, &aesccmSegmentedFinalizeOp);
        *plaintext_length = aesccmSegmentedFinalizeOp.inputLength;

        status = map_AES_status(ret);
#endif
    }
    else if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg) == PSA_ALG_GCM)
    {
#ifdef ENABLE_TI_CRYPTO_AESGCM
        AESGCM_SegmentedFinalizeOperation_init(&aesgcmSegmentedFinalizeOp);
        aesgcmSegmentedFinalizeOp.input       = operation->curr_unprocessed_data;
        aesgcmSegmentedFinalizeOp.output      = plaintext;
        aesgcmSegmentedFinalizeOp.inputLength = operation->unprocessed_len;
        aesgcmSegmentedFinalizeOp.mac         = (uint8_t *)tag;
        aesgcmSegmentedFinalizeOp.macLength   = tag_length;

        ret               = AESGCM_finalizeDecrypt(aesgcmHandle, &aesgcmSegmentedFinalizeOp);
        *plaintext_length = aesgcmSegmentedFinalizeOp.inputLength;

        status = map_AES_status(ret);
#endif
    }
    else
    {
        (void)ret;
        /* Do nothing */
    }

    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, NULL);
    }

    return PSA_SUCCESS;
}

/*
 *  ======== psa_aead_abort ========
 */
psa_status_t psa_aead_abort(psa_aead_operation_t *operation)
{
    psa_status_t status;

    if (operation->id == 0U)
    {
        /* The object has (apparently) been initialized but it is not (yet) in
         * use. It's ok to call abort on such an object, and there's nothing to
         * do.
         */
        return PSA_SUCCESS;
    }

    status = psa_check_aes_handle(PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg));
    if (status != PSA_SUCCESS)
    {
        return psa_aead_error(operation, status, NULL);
    }

    switch (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(operation->alg))
    {
#ifdef ENABLE_TI_CRYPTO_AESCCM
        case PSA_ALG_CCM:
            /* Close and reopen driver since cancellation is not supported for polling mode */
            AESCCM_close(aesccmHandle);

    #ifdef TFM_BUILD
            psa_aesccm_construct(PSA_CALLBACK_MODE);
    #else
            psa_aesccm_construct(PSA_BLOCKING_MODE);
    #endif
            break;
#endif

#ifdef ENABLE_TI_CRYPTO_AESGCM
        case PSA_ALG_GCM:
            /* Close and reopen driver since cancellation is not supported for polling mode */
            AESGCM_close(aesgcmHandle);

    #ifdef TFM_BUILD
            psa_aesgcm_construct(PSA_CALLBACK_MODE);
    #else
            psa_aesgcm_construct(PSA_BLOCKING_MODE);
    #endif
            break;
#endif

        default:
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
    }

    *operation = PSA_AEAD_OPERATION_INIT;

    return status;
}

/******************************************************************************/
/* One-step AEAD operations */
/******************************************************************************/

/*
 *  ======== psa_aead_encrypt ========
 */
psa_status_t psa_aead_encrypt(psa_key_id_t key,
                              psa_algorithm_t alg,
                              const uint8_t *nonce,
                              size_t nonce_length,
                              const uint8_t *additional_data,
                              size_t additional_data_length,
                              const uint8_t *plaintext,
                              size_t plaintext_length,
                              uint8_t *ciphertext,
                              size_t ciphertext_size,
                              size_t *ciphertext_length)
{
    CryptoKey cryptoKey;
    int_fast16_t ret;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_type_t keyType;
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    size_t keyBits;
    size_t tagLength;
    uint8_t *tag;

    *ciphertext_length = 0;

    status = psa_check_aes_handle(PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg));
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    if (!PSA_ALG_IS_AEAD(alg) || PSA_ALG_IS_WILDCARD(alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&attributes, PSA_KEY_USAGE_ENCRYPT, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    keyType   = psa_get_key_type(&attributes);
    keyBits   = psa_get_key_bits(&attributes);
    tagLength = PSA_AEAD_TAG_LENGTH(keyType, keyBits, alg);
    if (tagLength > PSA_AEAD_TAG_MAX_SIZE)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (ciphertext_size < (plaintext_length + tagLength))
    {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    tag = ciphertext + plaintext_length;

    /* True for supported AEAD */
    if (PSA_BLOCK_CIPHER_BLOCK_LENGTH(keyType) != PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (((tag == NULL) && (tagLength != 0)) || ((additional_data == NULL) && (additional_data_length != 0)) ||
        ((plaintext == NULL) && (plaintext_length != 0)) || ((ciphertext == NULL) && (ciphertext_size != 0)) ||
        (nonce == NULL))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    KeyStore_PSA_initKey(&cryptoKey, toKeyStoreKeyID(key), PSA_BITS_TO_BYTES(keyBits), NULL);

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    switch (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg))
    {
#ifdef ENABLE_TI_CRYPTO_AESCCM
        case PSA_ALG_CCM:
            {
                /* Check for correct tag size. Test assumes tagLength=0 is invalid */
                status = psa_validate_ccm_tag_length(tagLength);
                if (status != PSA_SUCCESS)
                {
                    return status;
                }

                AESCCM_OneStepOperation_init(&aesccmOneStepOp);
                aesccmOneStepOp.key         = &cryptoKey;
                aesccmOneStepOp.aad         = (uint8_t *)additional_data;
                aesccmOneStepOp.aadLength   = additional_data_length;
                aesccmOneStepOp.input       = (uint8_t *)plaintext;
                aesccmOneStepOp.output      = ciphertext;
                aesccmOneStepOp.inputLength = plaintext_length;
                aesccmOneStepOp.nonce       = (uint8_t *)nonce;
                aesccmOneStepOp.nonceLength = nonce_length;
                aesccmOneStepOp.mac         = tag;
                aesccmOneStepOp.macLength   = tagLength;

                ret    = AESCCM_oneStepEncrypt(aesccmHandle, &aesccmOneStepOp);
                status = map_AES_status(ret);
                break;
            }
#endif

#ifdef ENABLE_TI_CRYPTO_AESGCM
        case PSA_ALG_GCM:
            {
                /* Check for correct tag size. Test assumes tagLength=0 is invalid */
                status = psa_validate_gcm_tag_length(tagLength);
                if (status != PSA_SUCCESS)
                {
                    return status;
                }

                AESGCM_OneStepOperation_init(&aesgcmOneStepOp);
                aesgcmOneStepOp.key                   = &cryptoKey;
                aesgcmOneStepOp.aad                   = (uint8_t *)additional_data;
                aesgcmOneStepOp.aadLength             = additional_data_length;
                aesgcmOneStepOp.input                 = (uint8_t *)plaintext;
                aesgcmOneStepOp.output                = ciphertext;
                aesgcmOneStepOp.inputLength           = plaintext_length;
                aesgcmOneStepOp.iv                    = (uint8_t *)nonce;
                aesgcmOneStepOp.ivLength              = nonce_length;
                aesgcmOneStepOp.ivInternallyGenerated = 0;
                aesgcmOneStepOp.mac                   = tag;
                aesgcmOneStepOp.macLength             = tagLength;

                ret    = AESGCM_oneStepEncrypt(aesgcmHandle, &aesgcmOneStepOp);
                status = map_AES_status(ret);
                break;
            }
#endif

        default:
            (void)ret;
            /* Do nothing */
            break;
    }

    if (status == PSA_SUCCESS)
    {
        *ciphertext_length = PSA_AEAD_ENCRYPT_OUTPUT_SIZE(keyType, alg, plaintext_length);
    }

    return status;
}

/*
 *  ======== psa_aead_decrypt ========
 */
psa_status_t psa_aead_decrypt(psa_key_id_t key,
                              psa_algorithm_t alg,
                              const uint8_t *nonce,
                              size_t nonce_length,
                              const uint8_t *additional_data,
                              size_t additional_data_length,
                              const uint8_t *ciphertext,
                              size_t ciphertext_length,
                              uint8_t *plaintext,
                              size_t plaintext_size,
                              size_t *plaintext_length)
{
    CryptoKey cryptoKey;
    int_fast16_t ret;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_type_t keyType;
    psa_status_t status;
    size_t keyBits;
    size_t payload_length;
    size_t tagLength;
    uint8_t *tag;

    *plaintext_length = 0;

    status = psa_check_aes_handle(PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg));
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    if (!PSA_ALG_IS_AEAD(alg) || PSA_ALG_IS_WILDCARD(alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(key, &attributes);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&attributes, PSA_KEY_USAGE_DECRYPT, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    keyType        = psa_get_key_type(&attributes);
    keyBits        = psa_get_key_bits(&attributes);
    tagLength      = PSA_AEAD_TAG_LENGTH(keyType, keyBits, alg);
    payload_length = ciphertext_length - tagLength;
    if ((tagLength > PSA_AEAD_TAG_MAX_SIZE) || (tagLength > ciphertext_length))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (payload_length > plaintext_size)
    {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    tag = (uint8_t *)(ciphertext + payload_length);

    /* True for supported AEAD */
    if (PSA_BLOCK_CIPHER_BLOCK_LENGTH(keyType) != PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (((tag == NULL) && (tagLength != 0)) || ((additional_data == NULL) && (additional_data_length != 0)) ||
        ((plaintext == NULL) && (plaintext_length != 0)) || ((ciphertext == NULL) && (ciphertext_length != 0)) ||
        (nonce == NULL))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    KeyStore_PSA_initKey(&cryptoKey, toKeyStoreKeyID(key), PSA_BITS_TO_BYTES(keyBits), NULL);

    /* Init status in case the crypto driver is not supported */
    status = PSA_ERROR_NOT_SUPPORTED;

    switch (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg))
    {
#ifdef ENABLE_TI_CRYPTO_AESCCM
        case PSA_ALG_CCM:
            {
                /* Check for correct tag size. Test assumes tagLength=0 is invalid */
                status = psa_validate_ccm_tag_length(tagLength);
                if (status != PSA_SUCCESS)
                {
                    return status;
                }

                AESCCM_OneStepOperation_init(&aesccmOneStepOp);
                aesccmOneStepOp.key         = &cryptoKey;
                aesccmOneStepOp.aad         = (uint8_t *)additional_data;
                aesccmOneStepOp.aadLength   = additional_data_length;
                aesccmOneStepOp.input       = (uint8_t *)ciphertext;
                aesccmOneStepOp.output      = (uint8_t *)plaintext;
                aesccmOneStepOp.inputLength = payload_length;
                aesccmOneStepOp.nonce       = (uint8_t *)nonce;
                aesccmOneStepOp.nonceLength = nonce_length;
                aesccmOneStepOp.mac         = tag;
                aesccmOneStepOp.macLength   = tagLength;

                ret    = AESCCM_oneStepDecrypt(aesccmHandle, &aesccmOneStepOp);
                status = map_AES_status(ret);
                break;
            }
#endif

#ifdef ENABLE_TI_CRYPTO_AESGCM
        case PSA_ALG_GCM:
            {
                /* Check for correct tag size. Test assumes tagLength=0 is invalid */
                status = psa_validate_gcm_tag_length(tagLength);
                if (status != PSA_SUCCESS)
                {
                    return status;
                }

                AESGCM_OneStepOperation_init(&aesgcmOneStepOp);
                aesgcmOneStepOp.key                   = &cryptoKey;
                aesgcmOneStepOp.aad                   = (uint8_t *)additional_data;
                aesgcmOneStepOp.aadLength             = additional_data_length;
                aesgcmOneStepOp.input                 = (uint8_t *)ciphertext;
                aesgcmOneStepOp.output                = plaintext;
                aesgcmOneStepOp.inputLength           = payload_length;
                aesgcmOneStepOp.iv                    = (uint8_t *)nonce;
                aesgcmOneStepOp.ivLength              = nonce_length;
                aesgcmOneStepOp.ivInternallyGenerated = 0;
                aesgcmOneStepOp.mac                   = tag;
                aesgcmOneStepOp.macLength             = tagLength;

                ret    = AESGCM_oneStepDecrypt(aesgcmHandle, &aesgcmOneStepOp);
                status = map_AES_status(ret);
                break;
            }
#endif

        default:
            (void)ret;
            /* Do nothing */
            break;
    }

    if (status == PSA_SUCCESS)
    {
        *plaintext_length = PSA_AEAD_DECRYPT_OUTPUT_SIZE(keyType, alg, ciphertext_length);
    }

    return status;
}

/******************************************************************************/
/* Key derivation */
/******************************************************************************/

/*
 *  ======== psa_key_derivation_abort ========
 */
psa_status_t psa_key_derivation_abort(psa_key_derivation_operation_t *operation)
{
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    psa_status_t status;

    if (KeyStore_acquireLock())
    {
        status = KeyMgmt_psa_key_derivation_abort(operation);

        KeyStore_releaseLock();

        return status;
    }
    else
    {
        return PSA_ERROR_GENERIC_ERROR;
    }
#else
    return PSA_ERROR_NOT_SUPPORTED;
#endif
}

/*
 *  ======== psa_key_derivation_get_capacity ========
 */
psa_status_t psa_key_derivation_get_capacity(const psa_key_derivation_operation_t *operation, size_t *capacity)
{
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    psa_status_t status;

    if (KeyStore_acquireLock())
    {
        status = KeyMgmt_psa_key_derivation_get_capacity(operation, capacity);

        KeyStore_releaseLock();

        return status;
    }
    else
    {
        return PSA_ERROR_GENERIC_ERROR;
    }
#else
    return PSA_ERROR_NOT_SUPPORTED;
#endif
}

/*
 *  ======== psa_key_derivation_set_capacity ========
 */
psa_status_t psa_key_derivation_set_capacity(psa_key_derivation_operation_t *operation, size_t capacity)
{
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    psa_status_t status;

    if (KeyStore_acquireLock())
    {
        status = KeyMgmt_psa_key_derivation_set_capacity(operation, capacity);

        KeyStore_releaseLock();

        return status;
    }
    else
    {
        return PSA_ERROR_GENERIC_ERROR;
    }
#else
    return PSA_ERROR_NOT_SUPPORTED;
#endif
}

/*
 *  ======== psa_key_derivation_output_bytes ========
 */
psa_status_t psa_key_derivation_output_bytes(psa_key_derivation_operation_t *operation,
                                             uint8_t *output,
                                             size_t output_length)
{
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    return PSA_ERROR_NOT_SUPPORTED;
#else
    return PSA_ERROR_NOT_SUPPORTED;
#endif
}

/*
 *  ======== psa_key_derivation_output_key ========
 */
psa_status_t psa_key_derivation_output_key(const psa_key_attributes_t *attributes,
                                           psa_key_derivation_operation_t *operation,
                                           psa_key_id_t *key)
{
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    psa_status_t status;
    mbedtls_svc_key_id_t outputKeyID;
    psa_key_attributes_t attributesCopy;

    status = psa_copy_attr_and_modify_alg(&attributesCopy, attributes, sizeof(attributesCopy));

    if (status == PSA_SUCCESS)
    {
        if (HSMXXF3_acquireLock(SemaphoreP_WAIT_FOREVER, (uintptr_t)NULL))
        {
            if (KeyStore_acquireLock())
            {
                status = KeyMgmt_psa_key_derivation_output_key(&attributesCopy, operation, &outputKeyID);

                if (status == PSA_SUCCESS)
                {
                    *key = toKeyID(outputKeyID);
                }
                else
                {
                    *key = PSA_KEY_ID_NULL;
                }

                KeyStore_releaseLock();
            }
            else
            {
                status = PSA_ERROR_GENERIC_ERROR;
            }

            HSMXXF3_releaseLock();
        }
        else
        {
            status = PSA_ERROR_GENERIC_ERROR;
        }
    }

    return status;

#else
    return PSA_ERROR_NOT_SUPPORTED;
#endif
}

/*
 *  ======== psa_key_derivation_setup ========
 */
psa_status_t psa_key_derivation_setup(psa_key_derivation_operation_t *operation, psa_algorithm_t alg)
{
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    psa_status_t status;

    if (KeyStore_acquireLock())
    {
        status = KeyMgmt_psa_key_derivation_setup(operation, alg);

        KeyStore_releaseLock();

        return status;
    }
    else
    {
        return PSA_ERROR_GENERIC_ERROR;
    }
#else
    return PSA_ERROR_NOT_SUPPORTED;
#endif
}

/*
 *  ======== psa_key_derivation_input_bytes ========
 */
psa_status_t psa_key_derivation_input_bytes(psa_key_derivation_operation_t *operation,
                                            psa_key_derivation_step_t step,
                                            const uint8_t *data,
                                            size_t data_length)
{
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    psa_status_t status;

    if (KeyStore_acquireLock())
    {
        status = KeyMgmt_psa_key_derivation_input_bytes(operation, step, data, data_length);

        KeyStore_releaseLock();

        return status;
    }
    else
    {
        return PSA_ERROR_GENERIC_ERROR;
    }
#else
    return PSA_ERROR_NOT_SUPPORTED;
#endif
}

/*
 *  ======== psa_key_derivation_input_integer ========
 */
psa_status_t psa_key_derivation_input_integer(psa_key_derivation_operation_t *operation,
                                              psa_key_derivation_step_t step,
                                              uint64_t value)
{
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    psa_status_t status;

    if (KeyStore_acquireLock())
    {
        status = KeyMgmt_psa_key_derivation_input_integer(operation, step, value);

        KeyStore_releaseLock();

        return status;
    }
    else
    {
        return PSA_ERROR_GENERIC_ERROR;
    }
#else
    return PSA_ERROR_NOT_SUPPORTED;
#endif
}

/*
 *  ======== psa_key_derivation_input_key ========
 */
psa_status_t psa_key_derivation_input_key(psa_key_derivation_operation_t *operation,
                                          psa_key_derivation_step_t step,
                                          psa_key_id_t key)
{
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    psa_status_t status;

    if (KeyStore_acquireLock())
    {
        status = KeyMgmt_psa_key_derivation_input_key(operation, step, toKeyStoreKeyID(key));

        KeyStore_releaseLock();

        return status;
    }
    else
    {
        return PSA_ERROR_GENERIC_ERROR;
    }
#else
    return PSA_ERROR_NOT_SUPPORTED;
#endif
}

/*
 *  ======== psa_key_derivation_verify_bytes ========
 */
psa_status_t psa_key_derivation_verify_bytes(psa_key_derivation_operation_t *operation,
                                             const uint8_t *expected_output,
                                             size_t output_length)
{
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    return PSA_ERROR_NOT_SUPPORTED;
#else
    return PSA_ERROR_NOT_SUPPORTED;
#endif
}

/*
 *  ======== psa_key_derivation_verify_key ========
 */
psa_status_t psa_key_derivation_verify_key(psa_key_derivation_operation_t *operation, psa_key_id_t expected)
{
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    return PSA_ERROR_NOT_SUPPORTED;
#else
    return PSA_ERROR_NOT_SUPPORTED;
#endif
}

/******************************************************************************/
/* Key Agreement */
/******************************************************************************/

/*
 *  ======== psa_raw_key_agreement ========
 */
psa_status_t psa_raw_key_agreement(psa_algorithm_t alg,
                                   psa_key_id_t private_key,
                                   const uint8_t *peer_key,
                                   size_t peer_key_length,
                                   uint8_t *output,
                                   size_t output_size,
                                   size_t *output_length)
{
#ifndef ENABLE_TI_CRYPTO_ECDH
    return PSA_ERROR_NOT_SUPPORTED;
#else
    int_fast16_t ret;
    psa_ecc_family_t curveFamily;
    psa_key_attributes_t privateKeyAttr = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_type_t privateKeyType;
    psa_status_t status;
    size_t curveBits;
    size_t curveBytes;
    size_t keyBits;
    size_t publicKeyBytes;

    if (ecdhHandle == NULL)
    {
        return PSA_ERROR_BAD_STATE;
    }

    /* For now, ECDH is the only supported key agreement algorithm */
    if (alg != PSA_ALG_ECDH)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Fetch key attributes using key ID */
    status = psa_get_key_attributes(private_key, &privateKeyAttr);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    /* Check key policies (permitted usage and allowed algorithms) */
    status = psa_key_attributes_usage_check(&privateKeyAttr, PSA_KEY_USAGE_DERIVE, alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    privateKeyType = psa_get_key_type(&privateKeyAttr);
    curveFamily    = PSA_KEY_TYPE_ECC_GET_FAMILY(privateKeyType);
    keyBits        = psa_get_key_bits(&privateKeyAttr);
    curveBits      = psa_get_key_bits(&privateKeyAttr);
    curveBytes     = PSA_BITS_TO_BYTES(curveBits);

    /* The only valid key type is PSA_KEY_TYPE_IS_ECC_KEY_PAIR */
    if (!PSA_KEY_TYPE_IS_ECC_KEY_PAIR(privateKeyType))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ECDH_OperationComputeSharedSecret_init(&computeSharedSecretOp);

    #if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    computeSharedSecretOp.curveType = map_keyTypeToECDHCurveTypeHSM(privateKeyType, keyBits);

    if (computeSharedSecretOp.curveType == 0)
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }
    #elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)
    computeSharedSecretOp.curve = map_keyTypeToECCParams(privateKeyType, keyBits);

    if (computeSharedSecretOp.curve == NULL)
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }
    #else
        #error "Device not supported"
    #endif

    if (curveFamily == PSA_ECC_FAMILY_MONTGOMERY)
    {
        publicKeyBytes                              = curveBytes;
        computeSharedSecretOp.keyMaterialEndianness = ECDH_LITTLE_ENDIAN_KEY;
    }
    else
    {
        publicKeyBytes = PSA_KEY_EXPORT_ECC_PUBLIC_KEY_MAX_SIZE(curveBits);

        /* Check that the first byte of the peer key is 0x04 to indicate
         * uncompressed point format.
         */
        if (peer_key[0] != 0x04)
        {
            /* Key format required for platform implementation. For more info,
             * see TI Drivers API documentation.
             */
            return PSA_ERROR_INVALID_ARGUMENT;
        }
    }

    /* Check if peer key has the correct length to verify that it is on the same
     * curve. This check needs to change in the future to a better check that
     * verifies the public key is on the correct (chosen) curve.
     */
    if (peer_key_length != publicKeyBytes)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (output_size < curveBytes)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* The device supports Montgomery and Short Weierstrass */
    KeyStore_PSA_initKey(&privateKey, toKeyStoreKeyID(private_key), PSA_BITS_TO_BYTES(keyBits), NULL);
    CryptoKeyPlaintext_initKey(&publicKey, (uint8_t *)peer_key, peer_key_length);

    #ifdef TFM_BUILD
    if (psa_crypto_ns_caller)
    {
        PSACrypto_s_secureCB->length  = curveBytes;
        PSACrypto_s_secureCB->keyBits = keyBits;

        /* Store the shared secret material in the secure callback buffer. The
         * PSA Crypto NS client is responsible for copying the 'x' component of
         * the shared secret to the user provided buffer.
         */
        CryptoKeyPlaintext_initBlankKey(&sharedSecret, &PSACrypto_s_secureCB->buffer[0], publicKeyBytes);
    }
    else
    #endif
    {
        CryptoKeyPlaintext_initBlankKey(&sharedSecret, &localBuf[0], publicKeyBytes);
    }

    computeSharedSecretOp.myPrivateKey   = &privateKey;
    computeSharedSecretOp.theirPublicKey = &publicKey;
    computeSharedSecretOp.sharedSecret   = &sharedSecret;

    ret    = ECDH_computeSharedSecret(ecdhHandle, &computeSharedSecretOp);
    status = map_ECDH_status(ret);

    if (!psa_crypto_ns_caller)
    {
        if (status == PSA_SUCCESS)
        {
            if (keyBits == 255)
            {
                /* For Curve25519, only the 'x' component is returned by the ECDH
                 * driver thus it can be copied directly as the shared secret.
                 */
                (void)memcpy(output, &localBuf[0], curveBytes);
            }
            else
            {
                /* The ECDH driver returns the shared secret in uncompressed point
                 * format (0x04 || x || y). Copy only the 'x' component in raw
                 * encoding format as the shared secret.
                 */
                (void)memcpy(output, &localBuf[1], curveBytes);
            }

            *output_length = curveBytes;
        }
    }

    return status;
#endif /* ENABLE_TI_CRYPTO_ECDH */
}

/*
 *  ======== psa_key_derivation_key_agreement ========
 */
psa_status_t psa_key_derivation_key_agreement(psa_key_derivation_operation_t *operation,
                                              psa_key_derivation_step_t step,
                                              psa_key_id_t private_key,
                                              const uint8_t *peer_key,
                                              size_t peer_key_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

/******************************************************************************/
/* Random generation */
/******************************************************************************/

/*
 *  ======== psa_generate_random ========
 */
psa_status_t psa_generate_random(uint8_t *output, size_t output_size)
{
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    #ifndef ENABLE_TI_CRYPTO_RNG
    return PSA_ERROR_NOT_SUPPORTED;
    #else
    int_fast16_t ret;
    psa_status_t status;

    if (rngHandle == NULL)
    {
        return PSA_ERROR_BAD_STATE;
    }

    ret    = RNG_getRandomBits(rngHandle, output, PSA_BYTES_TO_BITS(output_size));
    status = map_RNG_status(ret);

    return status;
    #endif /* ENABLE_TI_CRYPTO_RNG */
#else
    #ifndef ENABLE_TI_CRYPTO_TRNG
    return PSA_ERROR_NOT_SUPPORTED;
    #else
    int_fast16_t ret;
    psa_status_t status;

    if (trngHandle == NULL)
    {
        return PSA_ERROR_BAD_STATE;
    }

    ret    = TRNG_getRandomBytes(trngHandle, output, output_size);
    status = map_TRNG_status(ret);

    return status;
    #endif /* ENABLE_TI_CRYPTO_TRNG */
#endif /* ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)) \
        */
}

/*
 *  ======== psa_generate_random_internal ========
 */
static psa_status_t psa_generate_random_internal(uint8_t *output, size_t output_size)
{
    psa_status_t status;

#if defined(TFM_BUILD) && defined(ENABLE_TI_CRYPTO_RNG)
    /* Set RNG to use polling mode since this PSA function requires additional SL API calls */
    psa_rng_construct(PSA_POLLING_MODE);
#endif

    status = psa_generate_random(output, output_size);

#if defined(TFM_BUILD) && defined(ENABLE_TI_CRYPTO_RNG)
    /* Restore RNG return behavior */
    psa_rng_construct(PSA_CALLBACK_MODE);
#endif

    return status;
}

#ifdef ENABLE_TI_CRYPTO_RNG

    #if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
/*
 *  ======== psa_get_curve_order ========
 *  Returns a pointer to the curve order in little-endian.
 */
static psa_status_t psa_get_curve_order(const uint8_t **order, psa_key_type_t key_type, size_t key_bits)
{
    psa_status_t status                      = PSA_SUCCESS;
    const ECCParams_CurveParams *curveParams = map_keyTypeToECCParams(key_type, key_bits);

    if (curveParams != NULL)
    {
        *order = curveParams->order;
    }
    else
    {
        status = PSA_ERROR_NOT_SUPPORTED;
    }

    return status;
}
    #endif /* ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == \
              DeviceFamily_PARENT_CC35XX)) */

#endif /* ENABLE_TI_CRYPTO_RNG */

/*
 *  ======== psa_generate_key ========
 *  This function requires full key attributes. If built into the TFM, the
 *  crypto secure service is responsible for translating client key attributes
 *  to the full key attributes before calling this function.
 */
psa_status_t psa_generate_key(const psa_key_attributes_t *attributes, psa_key_id_t *key)
{
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    #ifndef ENABLE_TI_CRYPTO_RNG
    return PSA_ERROR_NOT_SUPPORTED;
    #else
    const uint8_t *upperLimit;
    int_fast16_t ret;
    psa_key_type_t keyType = psa_get_key_type(attributes);
    psa_status_t status;
    size_t keyBits       = psa_get_key_bits(attributes);
    size_t keyBytes      = PSA_BITS_TO_BYTES(keyBits);
    uint8_t *keyMaterial = NULL;

    if (PSA_KEY_TYPE_IS_PUBLIC_KEY(keyType) || (keyBits == 0))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (keyBytes <= sizeof(localBuf))
    {
        keyMaterial = &localBuf[0];
    }
    else
    {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    if (rngHandle == NULL)
    {
        return PSA_ERROR_BAD_STATE;
    }

    if (PSA_KEY_TYPE_IS_UNSTRUCTURED(keyType))
    {
        /* Symmetric key or raw non-key data can use any random number */
        status = psa_generate_random_internal(keyMaterial, keyBytes);
    }
    else if (PSA_KEY_TYPE_IS_ECC_KEY_PAIR(keyType) || PSA_KEY_TYPE_IS_DH_KEY_PAIR(keyType))
    {
        /* Set upper limit to point to curve order (little-endian) */
        status = psa_get_curve_order(&upperLimit, keyType, keyBits);
        if (status != PSA_SUCCESS)
        {
            return status;
        }

        /* A private key 'd' must be with the range: 1 <= d < n, where 'n' is
         * the order of the chosen curve. The lower limit (inclusive) is 1 and
         * upper limit (exclusive) is 'n' when provided to
         * RNG_getxxRandomNumberInRange().
         */
        if (PSA_ECC_FAMILY_IS_WEIERSTRASS(PSA_KEY_TYPE_ECC_GET_FAMILY(keyType)))
        {
            /* Private keys are big-endian for Weierstrass (Brainpool and
             * SECP/NIST) curves.
             */

            /* Convert curve order to big-endian format for upper limit */
            CryptoUtils_reverseCopy(upperLimit, &curveOrder[0], keyBytes);
            upperLimit = &curveOrder[0];

            ret = RNG_getBERandomNumberInRange(rngHandle, CryptoUtils_limitOne, upperLimit, keyMaterial, keyBits);
        }
        else
        {
            /* Private keys are little-endian for Montgomery (Curve25519) and
             * Edwards (Ed25519) curves.
             */
            ret = RNG_getLERandomNumberInRange(rngHandle, CryptoUtils_limitOne, upperLimit, keyMaterial, keyBits);
        }

        status = map_RNG_status(ret);
    }
    else
    {
        /* Other key types such as RSA and DES keys are not supported */
        return PSA_ERROR_NOT_SUPPORTED;
    }

    if (status == PSA_SUCCESS)
    {
        status = psa_import_key(attributes, keyMaterial, keyBytes, key);
    }

    return status;
    #endif /* ENABLE_TI_CRYPTO_RNG */
#else
    #ifndef ENABLE_TI_CRYPTO_TRNG
    return PSA_ERROR_NOT_SUPPORTED;
    #else
    int_fast16_t ret;
    CryptoKey cryptoKey;
    uint8_t *keyMaterial   = NULL;
    psa_key_type_t keyType = psa_get_key_type(attributes);
    psa_status_t status;
    size_t keyBytes = PSA_BITS_TO_BYTES(psa_get_key_bits(attributes));

    if (trngHandle == NULL)
    {
        return PSA_ERROR_BAD_STATE;
    }

    if (PSA_KEY_TYPE_IS_PUBLIC_KEY(keyType) || (keyBytes == 0))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (keyBytes <= sizeof(localBuf))
    {
        keyMaterial = &localBuf[0];
    }
    else
    {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    CryptoKeyPlaintext_initBlankKey(&cryptoKey, keyMaterial, keyBytes);

    ret    = TRNG_generateKey(trngHandle, &cryptoKey);
    status = map_TRNG_status(ret);

    if (status == PSA_SUCCESS)
    {
        status = psa_import_key(attributes, keyMaterial, keyBytes, key);
    }

    return status;
    #endif /* ENABLE_TI_CRYPTO_TRNG */
#endif     /* ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == \
            * DeviceFamily_PARENT_CC35XX))                                                    \
            */
}

/******************************************************************************/
/* Initialization */
/******************************************************************************/

/*
 *  ======== psa_crypto_init ========
 */
psa_status_t psa_crypto_init(void)
{
    psa_status_t status = PSA_SUCCESS;

#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
    if (HSMXXF3_init() != HSMXXF3_STATUS_SUCCESS)
    {
        return PSA_ERROR_HARDWARE_FAILURE;
    }
#endif

/* Driver init */
#ifdef ENABLE_TI_CRYPTO_AESCBC
    if (aescbcHandle == NULL)
    {
        AESCBC_init();

    #ifdef TFM_BUILD
        psa_aescbc_construct(PSA_CALLBACK_MODE);
    #else
        psa_aescbc_construct(PSA_BLOCKING_MODE);
    #endif
    }
#endif

#ifdef ENABLE_TI_CRYPTO_AESCCM
    if (aesccmHandle == NULL)
    {
        AESCCM_init();

    #ifdef TFM_BUILD
        psa_aesccm_construct(PSA_CALLBACK_MODE);
    #else
        psa_aesccm_construct(PSA_BLOCKING_MODE);
    #endif
    }
#endif

#ifdef ENABLE_TI_CRYPTO_AESCMAC
    /* It is safe to unconditionally initialize AES CMAC driver */
    AESCMAC_init();

    if (aescmacHandle == NULL)
    {
    #ifdef TFM_BUILD
        psa_aescmac_construct(PSA_CALLBACK_MODE);
    #else
        psa_aescmac_construct(PSA_BLOCKING_MODE);
    #endif
    }

    if (aescbcmacHandle == NULL)
    {
    #ifdef TFM_BUILD
        psa_aescbcmac_construct(PSA_CALLBACK_MODE);
    #else
        psa_aescbcmac_construct(PSA_BLOCKING_MODE);
    #endif
    }
#endif

#ifdef ENABLE_TI_CRYPTO_AESCTR
    if (aesctrHandle == NULL)
    {
        AESCTR_init();

    #ifdef TFM_BUILD
        psa_aesctr_construct(PSA_CALLBACK_MODE);
    #else
        psa_aesctr_construct(PSA_BLOCKING_MODE);
    #endif
    }
#endif

#ifdef ENABLE_TI_CRYPTO_AESECB
    if (aesecbHandle == NULL)
    {
        AESECB_init();

    #ifdef TFM_BUILD
        psa_aesecb_construct(PSA_CALLBACK_MODE);
    #else
        psa_aesecb_construct(PSA_BLOCKING_MODE);
    #endif
    }
#endif

#ifdef ENABLE_TI_CRYPTO_AESGCM
    if (aesgcmHandle == NULL)
    {
        AESGCM_init();

    #ifdef TFM_BUILD
        psa_aesgcm_construct(PSA_CALLBACK_MODE);
    #else
        psa_aesgcm_construct(PSA_BLOCKING_MODE);
    #endif
    }
#endif

#ifdef ENABLE_TI_CRYPTO_ECDH
    if (ecdhHandle == NULL)
    {
        ECDH_init();

    #ifdef TFM_BUILD
        psa_ecdh_construct(PSA_CALLBACK_MODE);
    #else
        psa_ecdh_construct(PSA_BLOCKING_MODE);
    #endif
    }
#endif

#ifdef ENABLE_TI_CRYPTO_ECDSA
    if (ecdsaHandle == NULL)
    {
        ECDSA_init();

    #ifdef TFM_BUILD
        psa_ecdsa_construct(PSA_CALLBACK_MODE);
    #else
        psa_ecdsa_construct(PSA_BLOCKING_MODE);
    #endif
    }
#endif

#ifdef ENABLE_TI_CRYPTO_EDDSA
    if (eddsaHandle == NULL)
    {
        EDDSA_init();

    #ifdef TFM_BUILD
        psa_eddsa_construct(PSA_CALLBACK_MODE);
    #else
        psa_eddsa_construct(PSA_BLOCKING_MODE);
    #endif
    }
#endif

#ifdef ENABLE_TI_CRYPTO_SHA2
    if (sha2Handle == NULL)
    {
        SHA2_init();

    #ifdef TFM_BUILD
        psa_sha2_construct(PSA_CALLBACK_MODE);
    #else
        psa_sha2_construct(PSA_BLOCKING_MODE);
    #endif
    }
#endif

#if defined(ENABLE_TI_CRYPTO_RNG)
    if (rngHandle == NULL)
    {
        RNG_init();

    #ifdef TFM_BUILD
        psa_rng_construct(PSA_CALLBACK_MODE);
    #else
        psa_rng_construct(PSA_BLOCKING_MODE);
    #endif
    }
#endif

#if defined(ENABLE_TI_CRYPTO_TRNG)
    if (trngHandle == NULL)
    {
        TRNG_init();

        TRNG_Params trngParams;
        TRNG_Params_init(&trngParams);
        trngParams.returnBehavior = TRNG_RETURN_BEHAVIOR_BLOCKING;

        memset(&trngObject, 0x0, sizeof(trngObject));
        trngConfig.object  = &trngObject;
        trngConfig.hwAttrs = &trngHWAttrs;

        trngHandle = TRNG_construct(&trngConfig, &trngParams);
    }
#endif

#if (TFM_ENABLED == 0)
    /* Initialize Key Store which will handle initialization of ITS */
    status = KeyStore_PSA_init();
#endif

    return status;
}
