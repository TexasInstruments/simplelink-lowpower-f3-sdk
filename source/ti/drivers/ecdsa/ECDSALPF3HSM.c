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

#include <stdint.h>
#include <string.h>

#include <ti/drivers/ECDSA.h>
#include <ti/drivers/ecdsa/ECDSALPF3HSM.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKeyPlaintext.h>
#include <ti/drivers/cryptoutils/utils/CryptoUtils.h>

/* HSM related includes */
#include <ti/drivers/cryptoutils/hsm/HSMLPF3.h>
#include <ti/drivers/cryptoutils/hsm/HSMLPF3Utility.h>
#include <third_party/hsmddk/include/Integration/HSMSAL/HSMSAL.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_asset_policy.h>
#include <third_party/hsmddk/include/Kit/EIP130/DomainHelper/incl/eip130_domain_ecc_curves.h>

#if (ENABLE_KEY_STORAGE == 1)
    #include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA.h>
    #include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA_helpers.h>
    #include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_asset.h>

    /* Note that this size is in terms of the HSM sub-vector format, which is how KeyStore will
     * return the key material to the driver. The 521-bit curve length must be converted to a word boundary,
     * multiplied by number of bytes per word, and added by 4 bytes for the additional header word per component.
     * Since there are two components in the public key, multiply the whole prior value by 2.
     */
    #define ECDSALPF3HSM_MAX_PUBLIC_KEY_SIZE \
        (HSM_ASYM_ECC_PUB_KEY_VCOUNT * (HSM_WORD_LENGTH * ((ECDSA_CURVE_LENGTH_521 + 31) / 32) + HSM_ASYM_DATA_VHEADER))
#endif

#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/HwiP.h>

/* Helper functions */
static int_fast16_t ECDSALPF3HSM_initializeHSMOperations(ECDSA_Handle handle);

static int_fast16_t ECDSALPF3HSM_createAndLoadAllAssets(ECDSA_Handle handle);
static int_fast16_t ECDSALPF3HSM_createAndLoadKeyAssetID(ECDSA_Handle handle);
static int_fast16_t ECDSALPF3HSM_createKeyAsset(ECDSA_Handle handle);
static int_fast16_t ECDSALPF3HSM_LoadKeyAsset(ECDSA_Handle handle, uint8_t *key, bool isFromKeyStore);

static int_fast16_t ECDSALPF3HSM_createAndLoadECurveAssetID(ECDSA_Handle handle);
static int_fast16_t ECDSALPF3HSM_createECurveAsset(ECDSA_Handle handle);
static int_fast16_t ECDSALPF3HSM_LoadECurve(ECDSA_Handle handle);

static int_fast16_t ECDSALPF3HSM_freeAssetID(ECDSA_Handle handle, uint32_t AssetID);
static int_fast16_t ECDSALPF3HSM_freeAllAssets(ECDSA_Handle handle);

static int_fast16_t ECDSALPF3HSM_setECCurveParameters(ECDSALPF3HSM_Object *object);

/*
 *  ======== ECDSA_init ========
 */
void ECDSA_init(void)
{
    HSMLPF3_constructRTOSObjects();
}

/*
 *  ======== ECDSA_close ========
 */
void ECDSA_close(ECDSA_Handle handle)
{
    DebugP_assert(handle);

    ECDSALPF3HSM_Object *object = handle->object;

    /* Mark the module as available */
    object->isOpen = false;
}

/*
 *  ======== ECDSA_construct ========
 */
ECDSA_Handle ECDSA_construct(ECDSA_Config *config, const ECDSA_Params *params)
{
    DebugP_assert(config);

    ECDSA_Handle handle         = NULL;
    ECDSALPF3HSM_Object *object = config->object;
    uintptr_t key;

    /* Initialize and boot HSM and related FW architectures */
    if (HSMLPF3_init() != HSMLPF3_STATUS_SUCCESS)
    {
        /* Upon HSM Boot failure, this driver stores the failure status in the object */
        object->hsmStatus = HSMLPF3_STATUS_ERROR;

        return NULL;
    }
    else
    {
        object->hsmStatus = HSMLPF3_STATUS_SUCCESS;
    }

    /* If params are NULL, use defaults */
    if (params == NULL)
    {
        params = &ECDSA_defaultParams;
    }

    key = HwiP_disable();

    if (object->isOpen)
    {
        HwiP_restore(key);
    }
    else
    {
        object->isOpen = true;

        HwiP_restore(key);

        object->returnBehavior = params->returnBehavior;
        object->accessTimeout  = params->timeout;
        object->callbackFxn    = params->callbackFxn;
        object->keyAssetID     = 0U;
        object->paramAssetID   = 0U;

        handle = (ECDSA_Handle)config;
    }

    return handle;
}

/*
 *  ======== ECDSALPF3HSM_setECCurveParameters ========
 */
static int_fast16_t ECDSALPF3HSM_setECCurveParameters(ECDSALPF3HSM_Object *object)
{
    int_fast16_t status = ECDSA_STATUS_ERROR;
    Eip130Domain_ECCurveFamily_t curveFamily;
    object->domainId = ECDSA_DOMAIN_ID_SEC;

    /* Initialize critical ECDSA driver metadata:
     * - curve length: (224, 256, 384, 512, or 521 bits)
     * - digest length: (224, 256, 384, or 512 bits)
     * - curve family and domain ID: (NIST or BrainPool)
     */
    switch (object->curveType)
    {
        case ECDSA_TYPE_SEC_P_224_R1:
            object->curveLength  = ECDSA_CURVE_LENGTH_224;
            object->digestLength = ECDSA_DIGEST_LENGTH_224;
            curveFamily          = EIP130DOMAIN_ECC_FAMILY_NIST_P;
            break;
        case ECDSA_TYPE_BRP_P_256_R1:
        case ECDSA_TYPE_SEC_P_256_R1:
            object->curveLength  = ECDSA_CURVE_LENGTH_256;
            object->digestLength = ECDSA_DIGEST_LENGTH_256;
            if (object->curveType == ECDSA_TYPE_SEC_P_256_R1)
            {
                curveFamily = EIP130DOMAIN_ECC_FAMILY_NIST_P;
            }
            else
            {
                curveFamily      = EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1;
                object->domainId = ECDSA_DOMAIN_ID_BRP;
            }
            break;
        case ECDSA_TYPE_BRP_P_384_R1:
        case ECDSA_TYPE_SEC_P_384_R1:
            object->curveLength  = ECDSA_CURVE_LENGTH_384;
            object->digestLength = ECDSA_DIGEST_LENGTH_384;
            if (object->curveType == ECDSA_TYPE_SEC_P_384_R1)
            {
                curveFamily = EIP130DOMAIN_ECC_FAMILY_NIST_P;
            }
            else
            {
                curveFamily      = EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1;
                object->domainId = ECDSA_DOMAIN_ID_BRP;
            }
            break;
        case ECDSA_TYPE_BRP_P_512_R1:
            object->curveLength  = ECDSA_CURVE_LENGTH_512;
            object->digestLength = ECDSA_DIGEST_LENGTH_512;
            curveFamily          = EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1;
            object->domainId     = ECDSA_DOMAIN_ID_BRP;
            break;
        case ECDSA_TYPE_SEC_P_521_R1:
            object->curveLength  = ECDSA_CURVE_LENGTH_521;
            object->digestLength = ECDSA_DIGEST_LENGTH_512;
            curveFamily          = EIP130DOMAIN_ECC_FAMILY_NIST_P;
            break;
        default:
            /* Invalid CurveType. set curveFamily to NONE and return an error.
             * A valid curveType and one that is supported by the ECDSA driver should be
             * provided in order to perform a digital signature operation.
             */
            curveFamily = EIP130DOMAIN_ECC_FAMILY_NONE;
            break;
    }

    /* Retrieve and store the location of the appropriate curve parameters for later use.
     * These curve parameters will be loaded into the HSM IP as an asset holding the values
     * that will be later used to produce or verify a digital signature.
     */
    if (curveFamily != EIP130DOMAIN_ECC_FAMILY_NONE)
    {
        if (!Eip130Domain_ECC_GetCurve(curveFamily, object->curveLength, &object->curveParam, &object->curveParamSize))
        {
            status = ECDSA_STATUS_ERROR;
        }
        else
        {
            status = ECDSA_STATUS_SUCCESS;
        }
    }
    else
    {
        status = ECDSALPF3HSM_STATUS_NO_VALID_CURVE_TYPE_PROVIDED;
    }

    return status;
}

/*
 *  ======== ECDSALPF3HSM_initializeHSMOperations ========
 */
static int_fast16_t ECDSALPF3HSM_initializeHSMOperations(ECDSA_Handle handle)
{
    ECDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = ECDSA_STATUS_ERROR;

    /* Check if HSMLPF3 initialization (boot up sequence or SW architecture) has failed.
     * If so, return the appropriate code.
     */
    if (object->hsmStatus == HSMLPF3_STATUS_ERROR)
    {
        return ECDSALPF3HSM_STATUS_HARDWARE_ERROR;
    }

    /* Initializes critical ECDSA driver metadata and retrieves and stores ECC curve parameters in the object */
    status = ECDSALPF3HSM_setECCurveParameters(object);

    if (status != ECDSA_STATUS_SUCCESS)
    {
        return status;
    }

    if (!HSMLPF3_acquireLock(object->accessTimeout, (uintptr_t)handle))
    {
        return ECDSA_STATUS_RESOURCE_UNAVAILABLE;
    }

    /* Create two assets and load data into them:
     * - First asset is the private key.
     * - Second asset is the ECC curve parameters.
     */
    status = ECDSALPF3HSM_createAndLoadAllAssets(handle);

    return status;
}

/*
 *  ======== ECDSALPF3HSM_ecdsaPostProcessing ========
 */
static inline void ECDSALPF3HSM_ecdsaPostProcessing(uintptr_t arg0)
{
    ECDSA_Handle handle         = (ECDSA_Handle)arg0;
    ECDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = ECDSA_STATUS_ERROR;
    int_fast16_t assetStatus    = ECDSA_STATUS_ERROR;
    int32_t tokenResult         = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = ECDSA_STATUS_SUCCESS;

        if (object->operationType == ECDSA_OPERATION_TYPE_SIGN)
        {
            /*
             * Convert the signature from HSM IP format to OS format by extracting
             * the R and S components of the signature, reversing the format and tossing out the header
             */
            ECDSA_OperationSign *signOperation = (ECDSA_OperationSign *)object->operation;
            HSMLPF3_asymDsaSignatureFromHW(&object->signature[0],
                                           object->curveLength,
                                           signOperation->r,
                                           signOperation->s);
        }
    }

    /* Free two assets (private key and ECC curve assets) */
    assetStatus = ECDSALPF3HSM_freeAllAssets(handle);

    if (assetStatus != ECDSA_STATUS_SUCCESS)
    {
        status = assetStatus;
    }

    object->returnStatus = status;

    HSMLPF3_releaseLock();

    if (object->returnBehavior == ECDSA_RETURN_BEHAVIOR_CALLBACK)
    {
        object->callbackFxn((ECDSA_Handle)arg0, status, *object->operation, object->operationType);
    }
}

/*
 *  ======== ECDSA_sign ========
 */
int_fast16_t ECDSA_sign(ECDSA_Handle handle, ECDSA_OperationSign *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    ECDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = ECDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;
    object->key                 = (CryptoKey *)operation->myPrivateKey;
    object->curveType           = operation->curveType;
    object->operationType       = ECDSA_OPERATION_TYPE_SIGN;
    object->operation           = (ECDSA_Operation *)operation;
    object->input               = (uint8_t *)operation->hash;
    object->keyAssetID          = 0;
    object->paramAssetID        = 0;

    /* Perform the following operations:
     * - Check the HSM HW status
     * - Setup up Driver metadata and EC curve parameters
     * - Create assets for the operation's key and EC parameters
     * - Attempt to acquire access semaphore for HSM operations.
     */
    status = ECDSALPF3HSM_initializeHSMOperations(handle);

    if (status != ECDSA_STATUS_SUCCESS)
    {
        /* In the case of failure to initialize the operation, we need to free all assets allocated
         * Capturing the return status of this operation is pointless since we are retuning an
         * error code anyways.
         */
        (void)ECDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();

        return status;
    }

    /* Populates the HSMLPF3 commandToken as a PK token for an ECDSA sign operation */
    HSMLPF3_constructECDSASignPhysicalToken(object);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    ECDSALPF3HSM_ecdsaPostProcessing,
                                    (uintptr_t)handle);
    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Handles post command token submission mechanism.
         * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers post-processing
         * fxn) and returns immediately when in callback mode.
         */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    if (hsmRetval != HSMLPF3_STATUS_SUCCESS)
    {
        status = ECDSA_STATUS_ERROR;

        /* In the case of failure to initialize the operation, we need to free all assets allocated
         * Capturing the return status of this operation is pointless since we are retuning an
         * error code anyways.
         */
        (void)ECDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== ECDSA_verify ========
 */
int_fast16_t ECDSA_verify(ECDSA_Handle handle, ECDSA_OperationVerify *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    ECDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = ECDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;
    object->key                 = (CryptoKey *)operation->theirPublicKey;
    object->curveType           = operation->curveType;
    object->operationType       = ECDSA_OPERATION_TYPE_VERIFY;
    object->operation           = (ECDSA_Operation *)operation;
    object->input               = (uint8_t *)operation->hash;
    object->keyAssetID          = 0;
    object->paramAssetID        = 0;

    /* Perform the following operations:
     * - Check the HSM HW status
     * - Setup up Driver metadata and EC curve parameters
     * - Create assets for the operation's key and EC parameters
     * - Attempt to acquire access semaphore for HSM operations.
     */
    status = ECDSALPF3HSM_initializeHSMOperations(handle);

    if (status != ECDSA_STATUS_SUCCESS)
    {
        /* In the case of failure to initialize the operation, we need to free all assets allocated
         * Capturing the return status of this operation is pointless since we are retuning an
         * error code anyways.
         */
        (void)ECDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();

        return status;
    }

    /* Signature passed by the user must be converted to a format that the HSM IP requires it to be.
     * A 32-bit word header is attached at the beginning of each sub-vector component includes:
     * - The length of the sub-vector component
     * - Total number of sub-vector components
     * - Domain ID of the sub-vector component
     * - Index of each sub-vector component
     * The data in the body of the sub-vector component should be revered into little endian format.
     */
    memset(&object->signature[0], 0, sizeof(object->signature));
    HSMLPF3_asymDsaSignatureToHW(operation->r, operation->s, object->curveLength, &object->signature[0]);

    /* Populates the HSMLPF3 commandToken as a PK token for an ECDSA verify operation */
    HSMLPF3_constructECDSASignPhysicalToken(object);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    ECDSALPF3HSM_ecdsaPostProcessing,
                                    (uintptr_t)handle);
    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Handles post command token submission mechanism.
         * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers post-processing
         * fxn) and returns immediately when in callback mode.
         */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    if (hsmRetval != HSMLPF3_STATUS_SUCCESS)
    {
        status = ECDSA_STATUS_ERROR;

        /* In the case of failure to initialize the operation, we need to free all assets allocated
         * Capturing the return status of this operation is pointless since we are retuning an
         * error code anyways.
         */
        (void)ECDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== ECDSALPF3HSM_createAndLoadAllAssets ========
 */
static int_fast16_t ECDSALPF3HSM_createAndLoadAllAssets(ECDSA_Handle handle)
{
    int_fast16_t status = ECDSA_STATUS_ERROR;

    /* Perform two HSM operations:
     * - Create an asset that holds the operation's asymmetric key
     *   (private key for a sign operation or public key for a verify operation)
     * - Load the operation's asymmetric key onto HSM RAM
     */
    status = ECDSALPF3HSM_createAndLoadKeyAssetID(handle);
    if (status == ECDSA_STATUS_SUCCESS)
    {
        /* Perform two HSM operations:
         * - Create an asset that holds the operation's ECC curve parameters
         * - Load the appropriate ECC curve parameters onto HSM RAM
         */
        status = ECDSALPF3HSM_createAndLoadECurveAssetID(handle);
    }

    return status;
}

/*
 *  ======== ECDSALPF3HSM_createAndLoadKeyAssetID ========
 */
static int_fast16_t ECDSALPF3HSM_createAndLoadKeyAssetID(ECDSA_Handle handle)
{
    ECDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = ECDSA_STATUS_ERROR;
    uint32_t keyLength          = 0U;
    bool isKeyStoreKeyMaterial  = false;
    uint8_t *keyMaterial;
#if (ENABLE_KEY_STORAGE == 1)
    uint8_t KeyStore_keyingMaterial[ECDSALPF3HSM_MAX_PUBLIC_KEY_SIZE];
    /* Although the key is used by ECDSA to sign or verify a HASH, we must provide the MESSAGE usage
     * to support psa_sign_message() and psa_verify_message(). When a key imported with KEYSTORE_PSA_KEY_USAGE_SIGN_HASH
     * or KEYSTORE_PSA_KEY_USAGE_VERIFY_HASH is retrieved, its allowed usage flags are extended to include the message
     * variants. Therefore, we can always ask for the message usage from the driver.
     */
    KeyStore_PSA_KeyUsage usage = (object->operationType == ECDSA_OPERATION_TYPE_SIGN)
                                      ? KEYSTORE_PSA_KEY_USAGE_SIGN_MESSAGE
                                      : KEYSTORE_PSA_KEY_USAGE_VERIFY_MESSAGE;
#endif

    /* While ECDSA only supports HSM operations on CC27XX, we will accept both
     * key encodings for plaintext only.
     */
    if ((object->key->encoding == CryptoKey_PLAINTEXT_HSM) || (object->key->encoding == CryptoKey_PLAINTEXT))
    {
        keyLength   = object->key->u.plaintext.keyLength;
        keyMaterial = object->key->u.plaintext.keyMaterial;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (object->key->encoding == CryptoKey_KEYSTORE_HSM)
    {
        keyLength   = object->key->u.keyStore.keyLength;
        keyMaterial = &KeyStore_keyingMaterial[0];

        status = KeyStore_PSA_retrieveFromKeyStore(object->key,
                                                   &KeyStore_keyingMaterial[0],
                                                   sizeof(KeyStore_keyingMaterial),
                                                   &object->keyAssetID,
                                                   KEYSTORE_PSA_ALG_ECDSA,
                                                   usage);

        if (status != KEYSTORE_PSA_STATUS_SUCCESS)
        {
            return status;
        }
        else if (object->keyAssetID != 0)
        {
            /* In this case, we already retrieved an asset from KeyStore,
             * so we don't need the driver to create and load an asset itself.
             * We must mark this before validating key sizes to ensure we cleanup
             * properly in the case that key size validation fails.
             */
            object->driverCreatedKeyAsset = false;
        }
        else
        {
            /* This variable is used to indicate that KeyStore has returned specifically plaintext
             * key material. It is not considered true in the above condition where KeyStore
             * retrieved an asset ID instead of plaintext key material. We must mark when plaintext
             * is retrieved from KeyStore, because KeyStore has already placed it into HSM
             * sub-vector format.
             */
            isKeyStoreKeyMaterial = true;
        }
    }
#endif
    else
    {
        return ECDSALPF3HSM_STATUS_INVALID_KEY_ENCODING;
    }

    /* Validate key sizes to make sure octet string format is used */
    if ((object->operationType == ECDSA_OPERATION_TYPE_SIGN) && (keyLength != BITS_TO_BYTES(object->curveLength)))
    {
        return ECDSA_STATUS_INVALID_KEY_SIZE;
    }
    else if ((object->operationType == ECDSA_OPERATION_TYPE_VERIFY) &&
             (keyLength != ((2 * BITS_TO_BYTES(object->curveLength)) + OCTET_STRING_OFFSET)))
    {
        /* The CryptoKey keyLength must always correspond to the public key's length. This is the case even
         * when a key_pair key ID is used to perform a verification operation.
         */
        return ECDSA_STATUS_INVALID_KEY_SIZE;
    }

#if (ENABLE_KEY_STORAGE == 1)
    if ((object->key->encoding == CryptoKey_KEYSTORE_HSM) && (object->keyAssetID != 0))
    {
        /* If we reached this point, then KeyStore_PSA_retrieveFromKeyStore() must have returned success */
        return ECDSA_STATUS_SUCCESS;
    }
#endif

    /* The keyMaterial may only be checked after verifying that we have not
     * received an asset directly from KeyStore. We will also skip this check if we received
     * plaintext from KeyStore, since it will be in HSM sub-vector format, not octet-string.
     */
    if ((!isKeyStoreKeyMaterial) && (object->operationType == ECDSA_OPERATION_TYPE_VERIFY) && (keyMaterial[0] != 0x04))
    {
        return ECDSA_STATUS_INVALID_KEY_SIZE;
    }

    /* Constructs the policy for the asset and submits an asset create token to the HSM IP */
    status = ECDSALPF3HSM_createKeyAsset(handle);
    if (status == ECDSA_STATUS_SUCCESS)
    {
        /* Now that the driver has successfully created an asset, object->keyAssetID is now non-zero.
         * If any failure condition happens after this moment, the cleanup will expect
         * object->driverCreatedKeyAsset to be accurate, since the keyAssetID will reflect that there
         * is an asset to free, and the cleanup will need to know how to do that.
         */
        object->driverCreatedKeyAsset = true;

        /* Constructs an asset load token and submits it to the HSM IP. Note, when the driver
         * retrieves plaintext key material from keystore, it is already in the HSM's sub-vector
         * format. We must mark this so that the driver doesn't try to reformat the already-formatted
         * key.
         */
        status = ECDSALPF3HSM_LoadKeyAsset(handle, keyMaterial, isKeyStoreKeyMaterial);
    }
    else
    {
        /* If the driver fails to create an asset, keyAssetID is already 0
         * from the top of ECDSA_sign() and ECDSA_verify(), so the cleanup already
         * knows there is nothing to do here.
         */
    }

    return status;
}

/*
 *  ======== ECDSALPF3HSM_CreateKeyAssetPostProcessing ========
 */
static inline void ECDSALPF3HSM_CreateKeyAssetPostProcessing(uintptr_t arg0)
{
    ECDSA_Handle handle         = (ECDSA_Handle)arg0;
    ECDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = ECDSA_STATUS_ERROR;
    int32_t tokenResult         = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as many other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->keyAssetID = HSMLPF3_getResultAssetID();
        status             = ECDSA_STATUS_SUCCESS;
    }

    object->hsmStatus = status;
}

/*
 *  ======== ECDSALPF3HSM_createKeyAsset ========
 */
static int_fast16_t ECDSALPF3HSM_createKeyAsset(ECDSA_Handle handle)
{
    int_fast16_t status         = ECDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;
    ECDSALPF3HSM_Object *object = handle->object;
    uint64_t assetPolicy        = 0x0;
    uint32_t assetSize          = HSM_ASYM_DATA_SIZE_VWB(object->curveLength);

    if (object->operationType == ECDSA_OPERATION_TYPE_VERIFY)
    {
        assetSize *= HSM_ASYM_ECC_PUB_KEY_VCOUNT;
    }

    /* Operation (Lower 16-bits + general Operation) + Direction. No Mode */
    assetPolicy = EIP130_ASSET_POLICY_ASYM_SIGNVERIFY | EIP130_ASSET_POLICY_ACA_ECDSA;

    /* In the case of a sign operation, the key must be labeled "private data" since it is the private key.
     * This is not the case when doing a verify operation since the key loaded is the public key.
     */
    if (object->operationType == ECDSA_OPERATION_TYPE_SIGN)
    {
        assetPolicy |= EIP130_ASSET_POLICY_PRIVATEDATA;
    }

    /* the chosen curve type determines the asset policy's SHA algorithm of choice */
    switch (object->curveType)
    {
        case ECDSA_TYPE_SEC_P_224_R1:
            assetPolicy |= EIP130_ASSET_POLICY_ACH_SHA224;
            break;
        case ECDSA_TYPE_BRP_P_256_R1:
        case ECDSA_TYPE_SEC_P_256_R1:
            assetPolicy |= EIP130_ASSET_POLICY_ACH_SHA256;
            break;
        case ECDSA_TYPE_BRP_P_384_R1:
        case ECDSA_TYPE_SEC_P_384_R1:
            assetPolicy |= EIP130_ASSET_POLICY_ACH_SHA384;
            break;
        case ECDSA_TYPE_BRP_P_512_R1:
        case ECDSA_TYPE_SEC_P_521_R1:
            assetPolicy |= EIP130_ASSET_POLICY_ACH_SHA512;
            break;
        default:
            /* Do nothing. Curve type is invalid */
            break;
    }

    /* Populates the HSMLPF3 commandToken as a create asset token */
    HSMLPF3_constructCreateAssetToken(assetPolicy, assetSize);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    ECDSALPF3HSM_CreateKeyAssetPostProcessing,
                                    (uintptr_t)handle);
    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Handles post command token submission mechanism.
         * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers post-processing
         * fxn) and returns immediately when in callback mode.
         */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->hsmStatus;
        }
    }

    return status;
}

/*
 *  ======== ECDSALPF3HSM_LoadKeyAssetPostProcessing ========
 */
static inline void ECDSALPF3HSM_LoadKeyAssetPostProcessing(uintptr_t arg0)
{
    ECDSA_Handle handle         = (ECDSA_Handle)arg0;
    ECDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = ECDSA_STATUS_ERROR;
    int32_t tokenResult         = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as many other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = ECDSA_STATUS_SUCCESS;
    }

    object->hsmStatus = status;
}

static int_fast16_t ECDSALPF3HSM_LoadKeyAsset(ECDSA_Handle handle, uint8_t *key, bool isFromKeyStore)
{
    int_fast16_t status         = ECDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;
    ECDSALPF3HSM_Object *object = handle->object;
    uint32_t componentLength    = 0;

    if (object->operationType == ECDSA_OPERATION_TYPE_SIGN)
    {
        /* For sign operations, private keys consist of only one component */
        componentLength = HSM_ASYM_DATA_SIZE_VWB(object->curveLength);
    }
    else
    {
        /* For verify operations, public keys consist of two components (x and y) */
        componentLength = HSM_ASYM_ECC_PUB_KEY_VCOUNT * HSM_ASYM_DATA_SIZE_VWB(object->curveLength);
    }

    if (!isFromKeyStore)
    {
        /* Initialize a buffer that will hold the single- or multi-component vector for the operation's key */
        uint8_t componentVector[ECDSALPF3HSM_COMPONENT_VECTOR_LENGTH];
        memset(&componentVector[0], 0, componentLength);

        if (object->operationType == ECDSA_OPERATION_TYPE_SIGN)
        {
            /* In the case of a sign operation, there is one sub-vector component that needs to
             *     modified to comply with HSM IP format requirements
             * Modifications include:
             * - Properly populating the first 32-bit word as the vector header that describes the key
             * - Convert the key's format from OS to little endian.
             */
            HSMLPF3_asymDsaPriKeyToHW(key, object->curveLength, object->domainId, &componentVector[0]);
        }
        else
        {
            /* In the case of a verify operation, there are two sub-vector components that need to
             *     modified to comply with HSM IP format requirements
             * Modifications include:
             * - Properly populating the first 32-bit word as the vector header that describes the key
             * - Convert the key's format from OS to little endian.
             */
            HSMLPF3_asymDsaPubKeyToHW(key + 1, object->curveLength, object->domainId, &componentVector[0]);
        }

        /* Populates the HSMLPF3 commandToken as a load asset token */
        HSMLPF3_constructLoadPlaintextAssetToken(&componentVector[0], componentLength, object->keyAssetID);
    }
    else
    {
        /* Populates the HSMLPF3 commandToken as a load asset token. The key is already formatted for the HSM
         * if we retrieved it from KeyStore.
         */
        HSMLPF3_constructLoadPlaintextAssetToken(key, componentLength, object->keyAssetID);
    }

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    ECDSALPF3HSM_LoadKeyAssetPostProcessing,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Handles post command token submission mechanism.
         * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers post-processing
         * fxn) and returns immediately when in callback mode.
         */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->hsmStatus;
        }
    }

    return status;
}

static int_fast16_t ECDSALPF3HSM_createAndLoadECurveAssetID(ECDSA_Handle handle)
{
    int_fast16_t status = ECDSA_STATUS_ERROR;

    status = ECDSALPF3HSM_createECurveAsset(handle);
    if (status == ECDSA_STATUS_SUCCESS)
    {
        status = ECDSALPF3HSM_LoadECurve(handle);
    }

    return status;
}

/*
 *  ======== ECDSALPF3HSM_createECurveAssetPostProcessing ========
 */
static inline void ECDSALPF3HSM_createECurveAssetPostProcessing(uintptr_t arg0)
{
    ECDSA_Handle handle         = (ECDSA_Handle)arg0;
    ECDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = ECDSA_STATUS_ERROR;
    int32_t tokenResult         = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as many other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->paramAssetID = HSMLPF3_getResultAssetID();
        status               = ECDSA_STATUS_SUCCESS;
    }

    object->hsmStatus = status;
}

static int_fast16_t ECDSALPF3HSM_createECurveAsset(ECDSA_Handle handle)
{
    int_fast16_t status         = ECDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;
    ECDSALPF3HSM_Object *object = handle->object;
    uint64_t assetPolicy        = EIP130_ASSET_POLICY_ASYM_KEYPARAMS;

    /* Populates the HSMLPF3 commandToken as a create asset token */
    HSMLPF3_constructCreateAssetToken(assetPolicy, object->curveParamSize);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    ECDSALPF3HSM_createECurveAssetPostProcessing,
                                    (uintptr_t)handle);
    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Handles post command token submission mechanism.
         * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers post-processing
         * fxn) and returns immediately when in callback mode.
         */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->hsmStatus;
        }
    }

    return status;
}

/*
 *  ======== ECDSALPF3HSM_LoadECurvePostProcessing ========
 */
static inline void ECDSALPF3HSM_LoadECurvePostProcessing(uintptr_t arg0)
{
    ECDSA_Handle handle         = (ECDSA_Handle)arg0;
    ECDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = ECDSA_STATUS_ERROR;
    int32_t tokenResult         = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as many other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = ECDSA_STATUS_SUCCESS;
    }

    object->hsmStatus = status;
}

/*
 *  ======== ECDSALPF3HSM_LoadECurve ========
 */
static int_fast16_t ECDSALPF3HSM_LoadECurve(ECDSA_Handle handle)
{
    int_fast16_t status         = ECDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;
    ECDSALPF3HSM_Object *object = handle->object;

    /* Populates the HSMLPF3 commandToken as a load asset token */
    (void)HSMLPF3_constructLoadPlaintextAssetToken(object->curveParam, object->curveParamSize, object->paramAssetID);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    ECDSALPF3HSM_LoadECurvePostProcessing,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Handles post command token submission mechanism.
         * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers post-processing
         * fxn) and returns immediately when in callback mode.
         */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->hsmStatus;
        }
    }

    return status;
}

/*
 *  ======== ECDSALPF3HSM_freeAllAssets ========
 */
static int_fast16_t ECDSALPF3HSM_freeAllAssets(ECDSA_Handle handle)
{
    int_fast16_t status         = ECDSA_STATUS_SUCCESS;
    ECDSALPF3HSM_Object *object = handle->object;

    if (object->keyAssetID != 0)
    {
        /* If the object has a stored keyAssetID, then driverCreatedKeyAsset MUST
         * be set. It can only be false if KeyStore is enabled. If it is false,
         * it means that we retrieved an asset directly from KeyStore, so the
         * driver should not free the asset. Instead, the driver should direct
         * KeyStore to free the asset (which will perform the necessary persistence
         * check and only free the asset if it should be freed).
         */
        if (object->driverCreatedKeyAsset == true)
        {
            status = ECDSALPF3HSM_freeAssetID(handle, object->keyAssetID);
        }
#if (ENABLE_KEY_STORAGE == 1)
        else
        {
            KeyStore_PSA_KeyFileId keyID;

            GET_KEY_ID(keyID, object->key->u.keyStore.keyID);

            status = KeyStore_PSA_assetPostProcessing(keyID);
        }

        if (status == ECDSA_STATUS_SUCCESS)
        {
            object->keyAssetID = 0;
        }
#endif
    }

    if ((object->paramAssetID != 0) && (ECDSALPF3HSM_freeAssetID(handle, object->paramAssetID) != ECDSA_STATUS_SUCCESS))
    {
        status = ECDSA_STATUS_ERROR;
    }

    return status;
}

/*
 *  ======== ECDSALPF3HSM_freeAssetPostProcessing ========
 */
static inline void ECDSALPF3HSM_freeAssetPostProcessing(uintptr_t arg0)
{
    ECDSA_Handle handle         = (ECDSA_Handle)arg0;
    ECDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = ECDSA_STATUS_ERROR;
    int32_t tokenResult         = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as many other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = ECDSA_STATUS_SUCCESS;
    }

    object->hsmStatus = status;
}

/*
 *  ======== ECDSALPF3HSM_freeAssetID ========
 */
static int_fast16_t ECDSALPF3HSM_freeAssetID(ECDSA_Handle handle, uint32_t assetID)
{
    int_fast16_t status         = ECDSA_STATUS_ERROR;
    ECDSALPF3HSM_Object *object = handle->object;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;

    /* Populates the HSMLPF3 commandToken as a delete asset token */
    (void)HSMLPF3_constructDeleteAssetToken(assetID);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    ECDSALPF3HSM_freeAssetPostProcessing,
                                    (uintptr_t)handle);
    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Handles post command token submission mechanism.
         * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers post-processing
         * fxn) and returns immediately when in callback mode.
         */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->hsmStatus;
        }
    }

    return status;
}
