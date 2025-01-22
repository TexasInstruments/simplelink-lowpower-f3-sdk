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

/******************************************************************************
 * PSA Crypto helper functions
 ******************************************************************************/

#include <third_party/psa_crypto/include/psa/crypto.h>
#include <third_party/psa_crypto/crypto_config.h>
#include <third_party/psa_crypto/crypto_helper.h>

#include <ti/drivers/AESCommon.h>
#include <ti/drivers/ECDH.h>
#include <ti/drivers/ECDSA.h>
// TODO: Remove conditional compile once EDDSA driver is completed for CC27xx
#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
     (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
    /* EDDSA is not yet supported for CC27xx */
    #include <ti/drivers/EDDSA.h>
#endif
#include <ti/drivers/RNG.h>
#include <ti/drivers/SHA2.h>
#include <ti/drivers/TRNG.h>

#include <ti/devices/DeviceFamily.h>

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    #include <ti/drivers/ecdh/ECDHLPF3HSM.h>
    #include <ti/drivers/ecdsa/ECDSALPF3HSM.h>
    #include <ti/drivers/rng/RNGLPF3HSM.h>
    #include <ti/drivers/trng/TRNGLPF3HSM.h>
#endif

#ifdef TFM_BUILD
    #include <third_party/tfm/secure_fw/partitions/crypto/tfm_crypto_api.h> /* For tfm_crypto_get_caller_id */
#endif

/*
 *  ======== map_SHA2_status ========
 */
psa_status_t map_SHA2_status(int_fast16_t status)
{
    psa_status_t psaStatus;

    switch (status)
    {
        case SHA2_STATUS_SUCCESS:
            psaStatus = PSA_SUCCESS;
            break;

        case SHA2_STATUS_ERROR:
            psaStatus = PSA_ERROR_GENERIC_ERROR;
            break;

        case SHA2_STATUS_RESOURCE_UNAVAILABLE:
            psaStatus = PSA_ERROR_HARDWARE_FAILURE;
            break;

        case SHA2_STATUS_CANCELED:
            psaStatus = PSA_ERROR_BAD_STATE;
            break;

        case SHA2_STATUS_KEYSTORE_ERROR:
            psaStatus = PSA_ERROR_INVALID_HANDLE;
            break;

        case SHA2_STATUS_UNSUPPORTED:
            psaStatus = PSA_ERROR_NOT_SUPPORTED;
            break;

        default:
            psaStatus = PSA_ERROR_GENERIC_ERROR;
            break;
    }

    return psaStatus;
}

/*
 *  ======== map_ECDSA_status ========
 */
psa_status_t map_ECDSA_status(int_fast16_t status, bool isVerifyOp)
{
    psa_status_t psaStatus;

    switch (status)
    {
        case ECDSA_STATUS_SUCCESS:
            psaStatus = PSA_SUCCESS;
            break;

        case ECDSA_STATUS_ERROR:
            if (isVerifyOp)
            {
                psaStatus = PSA_ERROR_INVALID_SIGNATURE;
            }
            else
            {
                psaStatus = PSA_ERROR_GENERIC_ERROR;
            }
            break;

        case ECDSA_STATUS_RESOURCE_UNAVAILABLE:
            psaStatus = PSA_ERROR_HARDWARE_FAILURE;
            break;

        case ECDSA_STATUS_R_LARGER_THAN_ORDER:
        case ECDSA_STATUS_S_LARGER_THAN_ORDER:
            psaStatus = PSA_ERROR_INVALID_SIGNATURE;
            break;

        case ECDSA_STATUS_CANCELED:
            psaStatus = PSA_ERROR_BAD_STATE;
            break;

        case ECDSA_STATUS_INVALID_KEY_SIZE:
        case ECDSA_STATUS_POINT_AT_INFINITY:
        case ECDSA_STATUS_PUBLIC_KEY_NOT_ON_CURVE:
        case ECDSA_STATUS_PUBLIC_KEY_LARGER_THAN_PRIME:
        case ECDSA_STATUS_KEYSTORE_ERROR:
            psaStatus = PSA_ERROR_INVALID_ARGUMENT;
            break;

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)

        case ECDSALPF3HSM_STATUS_NO_VALID_CURVE_TYPE_PROVIDED:
        case ECDSALPF3HSM_STATUS_INVALID_KEY_ENCODING:
            psaStatus = PSA_ERROR_INVALID_ARGUMENT;
            break;

        case ECDSALPF3HSM_STATUS_HARDWARE_ERROR:
            psaStatus = PSA_ERROR_HARDWARE_FAILURE;
            break;

#endif /* DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX */

        default:
            psaStatus = PSA_ERROR_GENERIC_ERROR;
            break;
    }

    return psaStatus;
}

#ifdef ENABLE_TI_CRYPTO_EDDSA
/*
 *  ======== map_EDDSA_status ========
 */
psa_status_t map_EDDSA_status(int_fast16_t status, bool isVerifyOp)
{
    psa_status_t psaStatus;

    switch (status)
    {
        case EDDSA_STATUS_SUCCESS:
            psaStatus = PSA_SUCCESS;
            break;

        case EDDSA_STATUS_ERROR:
            if (isVerifyOp)
            {
                psaStatus = PSA_ERROR_INVALID_SIGNATURE;
            }
            else
            {
                psaStatus = PSA_ERROR_GENERIC_ERROR;
            }
            break;

        case EDDSA_STATUS_INVALID_R_SIZE:
        case EDDSA_STATUS_INVALID_S_SIZE:
        case EDDSA_STATUS_S_LARGER_THAN_ORDER:
            psaStatus = PSA_ERROR_INVALID_SIGNATURE;
            break;

        case EDDSA_STATUS_HASH_UNAVAILABLE:
        case EDDSA_STATUS_PKA_UNAVAILABLE:
        case EDDSA_STATUS_SHA2_HASH_FAILURE:
            psaStatus = PSA_ERROR_HARDWARE_FAILURE;
            break;

        case EDDSA_STATUS_PUBLIC_KEY_NOT_ON_CURVE:
        case EDDSA_STATUS_INVALID_PRIVATE_KEY_SIZE:
        case EDDSA_STATUS_INVALID_PUBLIC_KEY_SIZE:
        case EDDSA_STATUS_KEY_ID_PARAM_MISMATCH:
        case EDDSA_STATUS_KEYSTORE_ERROR:
        case EDDSA_STATUS_KEYSTORE_GENERIC_FAILURE:
        case EDDSA_STATUS_KEYSTORE_INVALID_ID:
            psaStatus = PSA_ERROR_INVALID_ARGUMENT;
            break;

        case EDDSA_STATUS_CANCELED:
            psaStatus = PSA_ERROR_BAD_STATE;
            break;

        default:
            psaStatus = PSA_ERROR_GENERIC_ERROR;
            break;
    }

    return psaStatus;
}
#endif

/*
 *  ======== map_ECDH_status ========
 */
psa_status_t map_ECDH_status(int_fast16_t status)
{
    psa_status_t psaStatus;

    switch (status)
    {
        case ECDH_STATUS_SUCCESS:
            psaStatus = PSA_SUCCESS;
            break;

        case ECDH_STATUS_ERROR:
            psaStatus = PSA_ERROR_GENERIC_ERROR;
            break;

        case ECDH_STATUS_RESOURCE_UNAVAILABLE:
            psaStatus = PSA_ERROR_HARDWARE_FAILURE;
            break;

        case ECDH_STATUS_INVALID_KEY_SIZE:
        case ECDH_STATUS_KEYSTORE_ERROR:
            psaStatus = PSA_ERROR_INVALID_ARGUMENT;
            break;

        case ECDH_STATUS_CANCELED:
            psaStatus = PSA_ERROR_BAD_STATE;
            break;

        case ECDH_STATUS_POINT_AT_INFINITY:
        case ECDH_STATUS_PRIVATE_KEY_LARGER_EQUAL_ORDER:
        case ECDH_STATUS_PRIVATE_KEY_ZERO:
        case ECDH_STATUS_PUBLIC_KEY_LARGER_THAN_PRIME:
        case ECDH_STATUS_PUBLIC_KEY_NOT_ON_CURVE:
            psaStatus = PSA_ERROR_INVALID_HANDLE;
            break;

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)

        case ECDHLPF3HSM_STATUS_NO_VALID_CURVE_TYPE_PROVIDED:
        case ECDHLPF3HSM_STATUS_INVALID_PRIVATE_KEY_ENCODING:
        case ECDHLPF3HSM_STATUS_INVALID_PUBLIC_KEY_ENCODING:
        case ECDHLPF3HSM_STATUS_INVALID_SHARED_SECRET_KEY_ENCODING:
        case ECDHLPF3HSM_STATUS_INVALID_PRIVATE_KEY_SIZE:
        case ECDHLPF3HSM_STATUS_INVALID_PUBLIC_KEY_SIZE:
        case ECDHLPF3HSM_STATUS_INVALID_SHARED_SECRET_KEY_SIZE:
        case ECDHLPF3HSM_STATUS_INVALID_ECC_KEYS:
            psaStatus = PSA_ERROR_INVALID_ARGUMENT;
            break;

        case ECDHLPF3HSM_STATUS_HARDWARE_ERROR:
            psaStatus = PSA_ERROR_HARDWARE_FAILURE;
            break;

#endif /* DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX */

        default:
            psaStatus = PSA_ERROR_GENERIC_ERROR;
            break;
    }
    return psaStatus;
}

#ifdef ENABLE_TI_CRYPTO_RNG
/*
 *  ======== map_RNG_status ========
 */
psa_status_t map_RNG_status(int_fast16_t status)
{
    psa_status_t psaStatus;

    switch (status)
    {
        case RNG_STATUS_SUCCESS:
            psaStatus = PSA_SUCCESS;
            break;

        case RNG_STATUS_ERROR:
            psaStatus = PSA_ERROR_GENERIC_ERROR;
            break;

        case RNG_STATUS_RESOURCE_UNAVAILABLE:
            psaStatus = PSA_ERROR_HARDWARE_FAILURE;
            break;

        case RNG_STATUS_INVALID_INPUTS:
            psaStatus = PSA_ERROR_INVALID_ARGUMENT;
            break;

        case RNG_STATUS_CANCELED:
            psaStatus = PSA_ERROR_BAD_STATE;
            break;

    #if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)

        case RNG_STATUS_INVALID_KEY_ENCODING:
            psaStatus = PSA_ERROR_INVALID_ARGUMENT;
            break;

    #endif /* DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX */

        default:
            psaStatus = PSA_ERROR_GENERIC_ERROR;
            break;
    }

    return psaStatus;
}
#endif

#ifdef ENABLE_TI_CRYPTO_TRNG
/*
 *  ======== map_TRNG_status ========
 */
psa_status_t map_TRNG_status(int_fast16_t status)
{
    psa_status_t psaStatus;

    switch (status)
    {
        case TRNG_STATUS_SUCCESS:
            psaStatus = PSA_SUCCESS;
            break;

        case TRNG_STATUS_ERROR:
            psaStatus = PSA_ERROR_GENERIC_ERROR;
            break;

        case TRNG_STATUS_RESOURCE_UNAVAILABLE:
            psaStatus = PSA_ERROR_HARDWARE_FAILURE;
            break;

        case TRNG_STATUS_INVALID_INPUTS:
            psaStatus = PSA_ERROR_INVALID_ARGUMENT;
            break;

        case TRNG_STATUS_CANCELED:
            psaStatus = PSA_ERROR_BAD_STATE;
            break;

        case TRNG_STATUS_KEYSTORE_ERROR:
            psaStatus = PSA_ERROR_INVALID_HANDLE;
            break;

    #if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)

        case TRNG_STATUS_INVALID_INPUT_SIZE:
        case TRNG_STATUS_INVALID_KEY_ENCODING:
            psaStatus = PSA_ERROR_INVALID_ARGUMENT;
            break;

        case TRNG_STATUS_ALREADY_IN_SPECIFIED_NRBG_TYPE:
            /* This is not an error so return success */
            psaStatus = PSA_SUCCESS;
            break;

    #endif /* DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX */

        default:
            psaStatus = PSA_ERROR_GENERIC_ERROR;
            break;
    }

    return psaStatus;
}
#endif

/*
 *  ======== map_AES_status ========
 */
psa_status_t map_AES_status(int_fast16_t status)
{
    psa_status_t psaStatus;

    switch (status)
    {
        case AES_STATUS_SUCCESS:
            psaStatus = PSA_SUCCESS;
            break;

        case AES_STATUS_ERROR:
            psaStatus = PSA_ERROR_GENERIC_ERROR;
            break;

        case AES_STATUS_RESOURCE_UNAVAILABLE:
            psaStatus = PSA_ERROR_HARDWARE_FAILURE;
            break;

        case AES_STATUS_CANCELED:
            psaStatus = PSA_ERROR_BAD_STATE;
            break;

        case AES_STATUS_FEATURE_NOT_SUPPORTED:
            psaStatus = PSA_ERROR_NOT_SUPPORTED;
            break;

        case AES_STATUS_KEYSTORE_GENERIC_ERROR:
        case AES_STATUS_KEYSTORE_INVALID_ID:
            psaStatus = PSA_ERROR_INVALID_HANDLE;
            break;

        case AES_STATUS_MAC_INVALID:
            psaStatus = PSA_ERROR_INVALID_SIGNATURE;
            break;

        default:
            psaStatus = PSA_ERROR_GENERIC_ERROR;
            break;
    }

    return psaStatus;
}

#ifdef DeviceFamily_CC27XX
/*
 *  ======== map_keyTypeToECDSACurveTypeHSM ========
 */
ECDSA_CurveType map_keyTypeToECDSACurveTypeHSM(psa_key_type_t keyType, size_t keyBits)
{
    psa_ecc_family_t curveFamily = PSA_KEY_TYPE_ECC_GET_FAMILY(keyType);
    ECDSA_CurveType curveType    = (ECDSA_CurveType)0; /* 0 is an invalid curveType */

    switch (curveFamily)
    {
        case PSA_ECC_FAMILY_BRAINPOOL_P_R1:
            switch (keyBits)
            {
                case 256:
                    curveType = ECDSA_TYPE_BRP_P_256_R1;
                    break;
                case 384:
                    curveType = ECDSA_TYPE_BRP_P_384_R1;
                    break;
                case 512:
                    curveType = ECDSA_TYPE_BRP_P_512_R1;
                    break;
                default:
                    break;
            }
            break;

        case PSA_ECC_FAMILY_SECP_R1:
            switch (keyBits)
            {
                case 192:
                    // TODO: Uncomment once this curve type is supported by HSM drivers (TIDRIVERS-6205)
                    // curveType = ECDSA_TYPE_SEC_P_192_R1;
                    break;
                case 224:
                    curveType = ECDSA_TYPE_SEC_P_224_R1;
                    break;
                case 256:
                    curveType = ECDSA_TYPE_SEC_P_256_R1;
                    break;
                case 384:
                    curveType = ECDSA_TYPE_SEC_P_384_R1;
                    break;
                case 521:
                    curveType = ECDSA_TYPE_SEC_P_521_R1;
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }

    return curveType;
}

/*
 *  ======== map_keyTypeToECDHCurveTypeHSM ========
 */
ECDH_CurveType map_keyTypeToECDHCurveTypeHSM(psa_key_type_t keyType, size_t keyBits)
{
    psa_ecc_family_t curveFamily = PSA_KEY_TYPE_ECC_GET_FAMILY(keyType);
    ECDH_CurveType curveType     = (ECDH_CurveType)0; /* 0 is an invalid curveType */

    switch (curveFamily)
    {
        case PSA_ECC_FAMILY_BRAINPOOL_P_R1:
            switch (keyBits)
            {
                case 256:
                    curveType = ECDH_TYPE_BRP_P_256_R1;
                    break;
                case 384:
                    curveType = ECDH_TYPE_BRP_P_384_R1;
                    break;
                case 512:
                    curveType = ECDH_TYPE_BRP_P_512_R1;
                    break;
                default:
                    break;
            }
            break;

        case PSA_ECC_FAMILY_SECP_R1:
            switch (keyBits)
            {
                case 192:
                    // TODO: Uncomment once this curve type is supported by HSM drivers (TIDRIVERS-6737)
                    // curveType = ECDH_TYPE_SEC_P_192_R1;
                    break;
                case 224:
                    curveType = ECDH_TYPE_SEC_P_224_R1;
                    break;
                case 256:
                    curveType = ECDH_TYPE_SEC_P_256_R1;
                    break;
                case 384:
                    curveType = ECDH_TYPE_SEC_P_384_R1;
                    break;
                case 521:
                    curveType = ECDH_TYPE_SEC_P_521_R1;
                    break;
                default:
                    break;
            }
            break;

        case PSA_ECC_FAMILY_MONTGOMERY:
            switch (keyBits)
            {
                case 255:
                    curveType = ECDH_TYPE_CURVE_25519;
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }

    return curveType;
}

#endif /* DeviceFamily_CC27XX */

/*
 *  ======== map_keyTypeToECCParams ========
 */
const ECCParams_CurveParams *map_keyTypeToECCParams(psa_key_type_t keyType, size_t keyBits)
{
    psa_ecc_family_t curveFamily             = PSA_KEY_TYPE_ECC_GET_FAMILY(keyType);
    const ECCParams_CurveParams *curveParams = NULL;

    switch (curveFamily)
    {
        case PSA_ECC_FAMILY_BRAINPOOL_P_R1:
            switch (keyBits)
            {
                case 256:
                    curveParams = &ECCParams_BrainpoolP256R1;
                    break;
                case 384:
                    curveParams = &ECCParams_BrainpoolP384R1;
                    break;
                case 512:
                    curveParams = &ECCParams_BrainpoolP512R1;
                    break;
                default:
                    break;
            }
            break;

        case PSA_ECC_FAMILY_SECP_R1:
            switch (keyBits)
            {
                case 224:
                    curveParams = &ECCParams_NISTP224;
                    break;
                case 256:
                    curveParams = &ECCParams_NISTP256;
                    break;
                case 384:
                    curveParams = &ECCParams_NISTP384;
                    break;
                case 521:
                    curveParams = &ECCParams_NISTP521;
                    break;
                default:
                    break;
            }
            break;

        case PSA_ECC_FAMILY_MONTGOMERY:
            if (keyBits == 255)
            {
                curveParams = &ECCParams_Curve25519;
            }
            break;

#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
     (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
        case PSA_ECC_FAMILY_TWISTED_EDWARDS:
            if (keyBits == 255)
            {
                curveParams = &ECCParams_Ed25519;
            }
            break;
#endif

        default:
            break;
    }

    return curveParams;
}

/*
 *  ======== toKeyStoreKeyID ========
 */
mbedtls_svc_key_id_t toKeyStoreKeyID(psa_key_id_t keyID)
{
#if ((TFM_ENABLED == 0) || defined(TFM_BUILD))
    mbedtls_svc_key_id_t keystoreKeyID;

    keystoreKeyID.key_id = keyID;

    #ifdef TFM_BUILD
    /* tfm_crypto_get_caller_id always returns PSA_SUCCESS */
    (void)tfm_crypto_get_caller_id(keystoreKeyID.owner);
    #else
    keystoreKeyID.owner = PSA_CRYPTO_KEY_ID_DEFAULT_OWNER;
    #endif /* TFM_BUILD */

    return keystoreKeyID;
#else
    return keyID;
#endif
}

/*
 *  ======== toKeyID ========
 */
psa_key_id_t toKeyID(mbedtls_svc_key_id_t keystoreKeyID)
{
#if ((TFM_ENABLED == 0) || defined(TFM_BUILD))
    return keystoreKeyID.key_id;
#else
    return keystoreKeyID;
#endif
}
