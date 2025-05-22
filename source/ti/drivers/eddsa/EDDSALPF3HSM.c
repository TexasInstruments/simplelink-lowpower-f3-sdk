/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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
#include <stdbool.h>
#include <string.h>

/* Top-level and device-specific includes */
#include <ti/drivers/EDDSA.h>
#include <ti/drivers/eddsa/EDDSALPF3HSM.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKey.h>

/* HSM-related includes */
#include <ti/drivers/cryptoutils/hsm/HSMLPF3.h>
#include <ti/drivers/cryptoutils/hsm/HSMLPF3Utility.h>

/* RTOS-related includes */
#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC27XX.h>

/* KeyStore-related includes */
#if (ENABLE_KEY_STORAGE == 1)
    #include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA.h>
    #include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA_helpers.h>
    #include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_asset.h>
#endif

/* Helper functions */
static int_fast16_t EDDSALPF3HSM_initializeHSMOperations(EDDSA_Handle handle);
static int_fast16_t EDDSALPF3HSM_validateKeys(EDDSALPF3HSM_Object *object);

static int_fast16_t EDDSALPF3HSM_createAndLoadAllAssets(EDDSA_Handle handle);

static int_fast16_t EDDSALPF3HSM_createAndLoadPrivateKeyAssetID(EDDSA_Handle handle);
static int_fast16_t EDDSALPF3HSM_createPrivateKeyAsset(EDDSA_Handle handle);
static int_fast16_t EDDSALPF3HSM_loadPrivateKeyAsset(EDDSA_Handle handle, uint8_t *key, bool isFromKeyStore);

static int_fast16_t EDDSALPF3HSM_createAndLoadPublicKeyAssetID(EDDSA_Handle handle);
static int_fast16_t EDDSALPF3HSM_createPublicKeyAsset(EDDSA_Handle handle);
static int_fast16_t EDDSALPF3HSM_loadPublicKeyAsset(EDDSA_Handle handle, uint8_t *key, bool isFromKeyStore);

static int_fast16_t EDDSALPF3HSM_createTempAssetID(EDDSA_Handle handle);

static int_fast16_t EDDSALPF3HSM_createAndLoadECurveAssetID(EDDSA_Handle handle);
static int_fast16_t EDDSALPF3HSM_createECurveAsset(EDDSA_Handle handle);
static int_fast16_t EDDSALPF3HSM_loadECurve(EDDSA_Handle handle);

static int_fast16_t EDDSALPF3HSM_computeIntermediateHash(EDDSA_Handle handle, HSMLPF3_CallbackFxn callbackFxn);

static int_fast16_t EDDSALPF3HSM_signInitial(EDDSA_Handle handle);
static int_fast16_t EDDSALPF3HSM_signUpdate(EDDSA_Handle handle);
static int_fast16_t EDDSALPF3HSM_signFinalize(EDDSA_Handle handle);

static int_fast16_t EDDSALPF3HSM_verifyInitial(EDDSA_Handle handle);
static int_fast16_t EDDSALPF3HSM_verifyFinalize(EDDSA_Handle handle);

static int_fast16_t EDDSALPF3HSM_freeAssetID(EDDSA_Handle handle, uint32_t assetID);
static int_fast16_t EDDSALPF3HSM_freeAllAssets(EDDSA_Handle handle);

static int_fast16_t EDDSALPF3HSM_setECCurveParameters(EDDSALPF3HSM_Object *object);

/* Defines */
#define EDDSA_DMA_MAX_LENGTH            0x001FFFFFU
#define EDDSA_CURVE_LENGTH_25519        255
#define EDDSA_DIGEST_LENGTH_256         256
#define EDDSA_DOMAIN_ID_TWISTED_EDWARDS 1
#define EDDSA_CURVE_FAMILY_TWISTED_ED   EIP130DOMAIN_ECC_FAMILY_TWISTED_EDWARDS
#define EDDSA_SIGN_INITIAL_MAX_LENGTH   96U
#define EDDSA_SIGN_UPDATE_MAX_LENGTH    4095U
#define EDDSA_SIGN_FINALIZE_MAX_LENGTH  64U
#define EDDSA_VERIFY_INITIAL_MAX_LENGTH 64U

/* KeyStore-related defines */
#if (ENABLE_KEY_STORAGE == 1)
    /* Note that this size is in terms of the HSM sub-vector format, which is how KeyStore will
     * return the key material to the driver. The 521-bit curve length must be converted to a word boundary,
     * multiplied by number of bytes per word, and added by 4 bytes for the additional header word per component.
     */
    #define EDDSALPF3HSM_PRIVATE_KEY_SIZE \
        ((HSM_WORD_LENGTH * ((EDDSA_CURVE_LENGTH_25519 + 31) / 32)) + HSM_ASYM_DATA_VHEADER)
#endif

/* Static globals */
static bool EDDSALPF3HSM_isInitialized = false;

/*
 *  ======== EDDSA_init ========
 */
void EDDSA_init(void)
{
    HSMLPF3_constructRTOSObjects();

    EDDSALPF3HSM_isInitialized = true;
}

/*
 *  ======== EDDSA_close ========
 */
void EDDSA_close(EDDSA_Handle handle)
{
    EDDSALPF3HSM_Object *object;

    DebugP_assert(handle);

    /* Get the pointer to the object */
    object = handle->object;

    /* Mark the module as available */
    object->isOpen = false;
}

/*
 *  ======== EDDSA_construct ========
 */
EDDSA_Handle EDDSA_construct(EDDSA_Config *config, const EDDSA_Params *params)
{
    EDDSA_Handle handle;
    EDDSALPF3HSM_Object *object;
    uint_fast8_t key;

    handle = (EDDSA_Handle)config;
    object = handle->object;

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
        params = (EDDSA_Params *)&EDDSA_defaultParams;
    }

    key = HwiP_disable();

    if (!EDDSALPF3HSM_isInitialized || object->isOpen)
    {
        HwiP_restore(key);
        return (NULL);
    }

    object->isOpen = true;

    HwiP_restore(key);

    DebugP_assert((params->returnBehavior == EDDSA_RETURN_BEHAVIOR_CALLBACK) ? params->callbackFxn : true);

    object->returnBehavior = params->returnBehavior;

    if (object->returnBehavior == EDDSA_RETURN_BEHAVIOR_CALLBACK)
    {
        object->callbackFxn = params->callbackFxn;
    }

    object->semaphoreTimeout = params->timeout;

    object->privateKeyAssetID = 0;
    object->publicKeyAssetID  = 0;
    object->paramAssetID      = 0;
    object->tempAssetID       = 0;

    return (handle);
}

/*
 *  ======== EDDSALPF3HSM_setECCurveParameters ========
 */
static int_fast16_t EDDSALPF3HSM_setECCurveParameters(EDDSALPF3HSM_Object *object)
{
    int_fast16_t status = EDDSA_STATUS_ERROR;

    /* Retrieve and store the location of the appropriate curve parameters for later use.
     * These curve parameters will be loaded into the HSM IP as an asset holding the values
     * that will be later used to produce or verify a digital signature.
     */
    if (Eip130Domain_ECC_GetCurve(EDDSA_CURVE_FAMILY_TWISTED_ED,
                                  EDDSA_CURVE_LENGTH_25519,
                                  &object->curveParam,
                                  &object->curveParamSize))
    {
        status = EDDSA_STATUS_SUCCESS;
    }

    return status;
}

/*
 *  ======== EDDSALPF3HSM_validateKeys ========
 */
static int_fast16_t EDDSALPF3HSM_validateKeys(EDDSALPF3HSM_Object *object)
{
    uint32_t privateKeyLength = 0;
    uint32_t publicKeyLength  = 0;

    if ((object->operationType == EDDSA_OPERATION_TYPE_GENERATE_PUBLIC_KEY) ||
        (object->operationType == EDDSA_OPERATION_TYPE_SIGN))
    {
        if ((object->privateKey->encoding != CryptoKey_PLAINTEXT) &&
            (object->privateKey->encoding != CryptoKey_PLAINTEXT_HSM))
        {
#if (ENABLE_KEY_STORAGE == 1)
            if (object->privateKey->encoding != CryptoKey_KEYSTORE_HSM)
            {
                return EDDSALPF3HSM_STATUS_INVALID_KEY_ENCODING;
            }
#else
            return EDDSALPF3HSM_STATUS_INVALID_KEY_ENCODING;
#endif
        }

        /* One of the following two conditions is guaranteed due to the encoding validation above */
        if ((object->privateKey->encoding & CRYPTOKEY_KEYSTORE) == 0)
        {
            privateKeyLength = object->privateKey->u.plaintext.keyLength;
        }
#if (ENABLE_KEY_STORAGE == 1)
        else
        {
            privateKeyLength = object->privateKey->u.keyStore.keyLength;
        }
#endif

        /* Check private key length. It should match the curve length after conversion. */
        if (privateKeyLength != BITS_TO_BYTES(EDDSA_CURVE_LENGTH_25519))
        {
            return EDDSA_STATUS_INVALID_PRIVATE_KEY_SIZE;
        }
    }

    if (object->operationType == EDDSA_OPERATION_TYPE_GENERATE_PUBLIC_KEY)
    {
        if ((object->publicKey->encoding != CryptoKey_BLANK_PLAINTEXT) &&
            (object->publicKey->encoding != CryptoKey_BLANK_PLAINTEXT_HSM))
        {
            return EDDSALPF3HSM_STATUS_INVALID_KEY_ENCODING;
        }
    }

    if ((object->operationType == EDDSA_OPERATION_TYPE_SIGN) || (object->operationType == EDDSA_OPERATION_TYPE_VERIFY))
    {
        if ((object->publicKey->encoding != CryptoKey_PLAINTEXT) &&
            (object->publicKey->encoding != CryptoKey_PLAINTEXT_HSM))
        {
#if (ENABLE_KEY_STORAGE == 1)
            if (object->publicKey->encoding != CryptoKey_KEYSTORE_HSM)
            {
                return EDDSALPF3HSM_STATUS_INVALID_KEY_ENCODING;
            }
#else
            return EDDSALPF3HSM_STATUS_INVALID_KEY_ENCODING;
#endif
        }
    }

    /* One of the following two conditions is guaranteed due to the encoding validation above */
    if ((object->publicKey->encoding & CRYPTOKEY_KEYSTORE) == 0)
    {
        publicKeyLength = object->publicKey->u.plaintext.keyLength;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else
    {
        publicKeyLength = object->publicKey->u.keyStore.keyLength;
    }
#endif

    /* Check public key length. It should match the curve length after conversion. */
    if (publicKeyLength != BITS_TO_BYTES(EDDSA_CURVE_LENGTH_25519))
    {
        return EDDSA_STATUS_INVALID_PUBLIC_KEY_SIZE;
    }

    return EDDSA_STATUS_SUCCESS;
}

/*
 *  ======== EDDSALPF3HSM_initializeHSMOperations ========
 */
static int_fast16_t EDDSALPF3HSM_initializeHSMOperations(EDDSA_Handle handle)
{
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;

    /* Check if HSMLPF3 initialization (boot up sequence or FW architecture) has failed.
     * If so, return the appropriate code.
     */
    if (object->hsmStatus == HSMLPF3_STATUS_ERROR)
    {
        return EDDSALPF3HSM_STATUS_HARDWARE_ERROR;
    }

    /* Validate the following for private and public CryptoKeys:
     * - Encoding type.
     * - Key length.
     */
    status = EDDSALPF3HSM_validateKeys(object);

    if (status != EDDSA_STATUS_SUCCESS)
    {
        return status;
    }

    /* Initializes critical EDDSA driver metadata, retrieves and stores ECC curve parameters in the object */
    status = EDDSALPF3HSM_setECCurveParameters(object);

    if (status != EDDSA_STATUS_SUCCESS)
    {
        return status;
    }

    /* Acquire sole access to the HSM IP. */
    if (!HSMLPF3_acquireLock(object->semaphoreTimeout, (uintptr_t)handle))
    {
        return EDDSALPF3HSM_STATUS_RESOURCE_UNAVAILABLE;
    }

    /* Create four assets and load data into them:
     * - First asset is the private key.
     * - Second asset is the public key.
     * - Third asset is the ECC curve parameters.
     * - Fourth asset is a temporary asset used for intermediate hash values.
     */
    status = EDDSALPF3HSM_createAndLoadAllAssets(handle);

    if (status != EDDSA_STATUS_SUCCESS)
    {
        /* Free four assets:
         * - Private key.
         * - Public key.
         * - ECC curve parameters.
         * - Digest asset.
         *
         * Ignore the return value here since the driver status code is error.
         */
        (void)EDDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== EDDSALPF3HSM_eddsaPostProcessing ========
 */
static inline void EDDSALPF3HSM_eddsaPostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle         = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int8_t tokenResult          = HSMLPF3_getResultCode();
    EDDSA_OperationGeneratePublicKey *generatePublicKey;
    EDDSA_OperationSign *signOperation;

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = EDDSA_STATUS_SUCCESS;

        object->stateAssetID = 0U;

        if (object->operationType == EDDSA_OPERATION_TYPE_GENERATE_PUBLIC_KEY)
        {
            generatePublicKey = (EDDSA_OperationGeneratePublicKey *)object->operation;

            HSMLPF3_asymDHPubKeyFromHW(object->signature,
                                       EDDSA_CURVE_LENGTH_25519,
                                       1,
                                       HSMLPF3_LITTLE_ENDIAN_KEY,
                                       generatePublicKey->myPublicKey->u.plaintext.keyMaterial);

            generatePublicKey->myPublicKey->encoding = (generatePublicKey->myPublicKey->encoding ==
                                                        CryptoKey_BLANK_PLAINTEXT)
                                                           ? CryptoKey_PLAINTEXT
                                                           : CryptoKey_PLAINTEXT_HSM;
        }
        else if (object->operationType == EDDSA_OPERATION_TYPE_SIGN)
        {
            signOperation = (EDDSA_OperationSign *)object->operation;

            /* Convert the HSM-formatted signature to userspace buffer. */
            HSMLPF3_asymDsaSignatureFromHW(object->signature,
                                           EDDSA_CURVE_LENGTH_25519,
                                           HSMLPF3_LITTLE_ENDIAN_KEY,
                                           signOperation->R,
                                           signOperation->S);
        }
    }

    /* Free four assets:
     * - Private key.
     * - Public key.
     * - ECC curve parameters.
     * - Digest asset.
     */
    if (EDDSALPF3HSM_freeAllAssets(handle) != EDDSA_STATUS_SUCCESS)
    {
        status = EDDSA_STATUS_ERROR;
    }

    object->returnStatus = status;

    HSMLPF3_releaseLock();

    if (object->returnBehavior == EDDSA_RETURN_BEHAVIOR_CALLBACK)
    {
        object->callbackFxn((EDDSA_Handle)arg0, status, *object->operation, object->operationType);
    }
}

/*
 *  ======== EDDSA_generatePublicKey ========
 */
int_fast16_t EDDSA_generatePublicKey(EDDSA_Handle handle, EDDSA_OperationGeneratePublicKey *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;
    object->privateKey          = (CryptoKey *)operation->myPrivateKey;
    object->publicKey           = (CryptoKey *)operation->myPublicKey;
    object->operationType       = EDDSA_OPERATION_TYPE_GENERATE_PUBLIC_KEY;
    object->operation           = (EDDSA_Operation *)operation;

    /* Initialize assets as unallocated so that a cleanup operation
     * doesn't attempt to free an asset that doesn't exist yet.
     */
    object->privateKeyAssetID = 0U;
    object->publicKeyAssetID  = 0U;
    object->paramAssetID      = 0U;
    object->tempAssetID       = 0U;

    /* Perform the following operations:
     * - Check the HSM HW status
     * - Setup Driver metadata and EC curve parameters
     * - Create assets for the operation's key and EC parameters
     * - Attempt to acquire access semaphore for HSM operations.
     */
    status = EDDSALPF3HSM_initializeHSMOperations(handle);

    if (status != EDDSA_STATUS_SUCCESS)
    {
        return status;
    }

    /* Populates the HSMLPF3 commandToken as a PK token for an EDDSA generate public key operation */
    HSMLPF3_constructEDDSAGenPubKeyPhysicalToken(object);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    EDDSALPF3HSM_eddsaPostProcessing,
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

    if (hsmRetval == HSMLPF3_STATUS_ERROR)
    {
        /* Free four assets:
         * - Private key.
         * - Public key.
         * - ECC curve parameters.
         * - Digest asset.
         *
         * Ignore the return value here since the driver status code is error.
         */
        (void)EDDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== EDDSA_sign ========
 */
int_fast16_t EDDSA_sign(EDDSA_Handle handle, EDDSA_OperationSign *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    object->privateKey          = (CryptoKey *)operation->myPrivateKey;
    object->publicKey           = (CryptoKey *)operation->myPublicKey;
    object->operationType       = EDDSA_OPERATION_TYPE_SIGN;
    object->operation           = (EDDSA_Operation *)operation;

    /* Initialize assets as unallocated so that a cleanup operation
     * doesn't attempt to free an asset that doesn't exist yet.
     */
    object->privateKeyAssetID = 0U;
    object->publicKeyAssetID  = 0U;
    object->paramAssetID      = 0U;
    object->tempAssetID       = 0U;

    /* HSM limitation: The message length must be greater than 0. */
    if (operation->preHashedMessageLength == 0U)
    {
        return EDDSALPF3HSM_STATUS_INVALID_MESSAGE_LENGTH;
    }

    /* Perform the following operations:
     * - Check the HSM HW status
     * - Setup Driver metadata and EC curve parameters
     * - Create assets for the operation's key and EC parameters
     * - Attempt to acquire access semaphore for HSM operations.
     */
    status = EDDSALPF3HSM_initializeHSMOperations(handle);

    if (status != EDDSA_STATUS_SUCCESS)
    {
        return status;
    }

    return EDDSALPF3HSM_signInitial(handle);
}

/*
 *  ======== EDDSALPF3HSM_computeIntermediateHashUpdatePostProcessing ========
 */
inline static void EDDSALPF3HSM_computeIntermediateHashUpdatePostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle         = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int8_t tokenResult          = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->inputLengthRemaining -= object->inputLength;

        /* If remaining input length is larger than 4095, then hash the excess outside of the
         * EdDSA HSM operation sequence.
         */
        if (object->inputLengthRemaining > EDDSA_SIGN_UPDATE_MAX_LENGTH)
        {
            status = EDDSALPF3HSM_computeIntermediateHash(handle,
                                                          EDDSALPF3HSM_computeIntermediateHashUpdatePostProcessing);
        }
        else
        {
            status = EDDSALPF3HSM_signUpdate(handle);
        }
    }
    else
    {
        status = EDDSA_STATUS_HASH_UNAVAILABLE;
    }

    if (status != EDDSA_STATUS_SUCCESS)
    {
        /* Free four assets:
         * - Private key.
         * - Public key.
         * - ECC curve parameters.
         * - Digest asset.
         *
         * Ignore the return value here since the driver status code is error.
         */
        (void)EDDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    object->returnStatus = status;
}

/*
 *  ======== EDDSALPF3HSM_computeIntermediateHashFinalPostProcessing ========
 */
inline static void EDDSALPF3HSM_computeIntermediateHashFinalPostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle         = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int8_t tokenResult          = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->inputLengthRemaining -= object->inputLength;

        /* If remaining input length is larger than 4095, then hash the excess outside of the
         * EdDSA HSM operation sequence.
         */
        if (object->inputLengthRemaining > EDDSA_SIGN_UPDATE_MAX_LENGTH)
        {
            status = EDDSALPF3HSM_computeIntermediateHash(handle,
                                                          EDDSALPF3HSM_computeIntermediateHashFinalPostProcessing);
        }
        else
        {
            status = EDDSALPF3HSM_signFinalize(handle);
        }
    }
    else
    {
        status = EDDSA_STATUS_HASH_UNAVAILABLE;
    }

    if (status != EDDSA_STATUS_SUCCESS)
    {
        /* Free four assets:
         * - Private key.
         * - Public key.
         * - ECC curve parameters.
         * - Digest asset.
         *
         * Ignore the return value here since the driver status code is error.
         */
        (void)EDDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    object->returnStatus = status;
}

/*
 *  ======== EDDSALPF3HSM_signInitialPostProcessing ========
 */
inline static void EDDSALPF3HSM_signInitialPostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle            = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object    = handle->object;
    EDDSA_OperationSign *operation = (EDDSA_OperationSign *)object->operation;
    int_fast16_t status            = EDDSA_STATUS_ERROR;
    int8_t tokenResult             = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->stateAssetID = HSMLPF3_getResultAssetID();

        object->input = (uint8_t *)&operation->preHashedMessage[object->inputLength];
        object->inputLengthRemaining -= object->inputLength;

        /* The EdDSA sign sequence of operations:
         * - Sign initial phase which can only process up to 64 bytes. (If total length is less than 64 bytes, then pass
         * the entire message.)
         * - If message length is greater than 64 bytes (Sign initial max input length) + 4095 (Sign update max input
         * length), then compute intermediate hash outside of the EdDSA HSM operation sequence.
         * - Sign update phase which can process up to 4095 bytes.
         * - If message length is greater than 4095 bytes, then compute intermediate hash outside of the EdDSA HSM
         * operation sequence.
         * - Sign finalize phase which can process up to 4095 bytes.
         */
        if (operation->preHashedMessageLength > (EDDSA_SIGN_INITIAL_MAX_LENGTH + EDDSA_SIGN_UPDATE_MAX_LENGTH))
        {
            status = EDDSALPF3HSM_computeIntermediateHash(handle,
                                                          EDDSALPF3HSM_computeIntermediateHashUpdatePostProcessing);
        }
        else
        {
            status = EDDSALPF3HSM_signUpdate(handle);
        }
    }
    else if (tokenResult == EIP130TOKEN_RESULT_INVALID_ASSET)
    {
        status = EDDSA_STATUS_PUBLIC_KEY_NOT_ON_CURVE;
    }

    if (status != EDDSA_STATUS_SUCCESS)
    {
        /* Free four assets:
         * - Private key.
         * - Public key.
         * - ECC curve parameters.
         * - Digest asset.
         *
         * Ignore the return value here since the driver status code is error.
         */
        (void)EDDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    object->returnStatus = status;
}

/*
 *  ======== EDDSALPF3HSM_signInitial ========
 */
static int_fast16_t EDDSALPF3HSM_signInitial(EDDSA_Handle handle)
{
    /* Implementation for the initial phase of the signing process
     * This function should prepare the initial state for the signing operation
     * and return the appropriate status.
     */
    EDDSALPF3HSM_Object *object    = handle->object;
    EDDSA_OperationSign *operation = (EDDSA_OperationSign *)object->operation;
    int_fast16_t status            = EDDSA_STATUS_ERROR;
    int_fast16_t hsmRetval         = HSMLPF3_STATUS_ERROR;

    object->input                = (uint8_t *)operation->preHashedMessage;
    object->inputLength          = operation->preHashedMessageLength;
    object->inputLengthRemaining = operation->preHashedMessageLength;

    if (operation->preHashedMessageLength > EDDSA_SIGN_INITIAL_MAX_LENGTH)
    {
        object->inputLength = EDDSA_SIGN_INITIAL_MAX_LENGTH;
    }

    /* Populates the HSMLPF3 commandToken as a sign initial token */
    HSMLPF3_constructEDDSASignInitialPhysicalToken(object);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    EDDSALPF3HSM_signInitialPostProcessing,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Wait for a result token from the HSM IP */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    if (hsmRetval == HSMLPF3_STATUS_ERROR)
    {
        /* Free four assets:
         * - Private key.
         * - Public key.
         * - ECC curve parameters.
         * - Digest asset.
         *
         * Ignore the return value here since the driver status code is error.
         */
        (void)EDDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== EDDSALPF3HSM_computeIntermediateHash ========
 */
static int_fast16_t EDDSALPF3HSM_computeIntermediateHash(EDDSA_Handle handle, HSMLPF3_CallbackFxn callbackFxn)
{
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_HASH_UNAVAILABLE;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;

    /* Leave two blocks for finalize operation to take care of it. */
    object->inputLength = object->inputLengthRemaining - (2U * 128U);

    if (object->inputLength > EDDSA_DMA_MAX_LENGTH)
    {
        /* Leave four blocks for finalize operation to take care of it. */
        object->inputLength = EDDSA_DMA_MAX_LENGTH - (4U * 128);
    }

    /* Make sure the length passed to the SHA2 hasing operation is block-size aligned. */
    object->inputLength = (size_t)(object->inputLength & (~(128U - 1U)));

    /* Populates the HSMLPF3 commandToken as a hash update token */
    HSMLPF3_constructEDDSAIntermediateHashPhysicalToken(object->input, object->inputLength, object->tempAssetID);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior, callbackFxn, (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Wait for a result token from the HSM IP */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    if (hsmRetval == HSMLPF3_STATUS_ERROR)
    {
        /* Free four assets:
         * - Private key.
         * - Public key.
         * - ECC curve parameters.
         * - Digest asset.
         *
         * Ignore the return value here since the driver status code is error.
         */
        (void)EDDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== EDDSALPF3HSM_signUpdatePostProcessing ========
 */
inline static void EDDSALPF3HSM_signUpdatePostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle            = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object    = handle->object;
    EDDSA_OperationSign *operation = (EDDSA_OperationSign *)object->operation;
    int_fast16_t status            = EDDSA_STATUS_ERROR;
    int8_t tokenResult             = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->input                = (uint8_t *)operation->preHashedMessage;
        object->inputLength          = operation->preHashedMessageLength;
        object->inputLengthRemaining = operation->preHashedMessageLength;

        if (operation->preHashedMessageLength > EDDSA_SIGN_FINALIZE_MAX_LENGTH)
        {
            object->input = (uint8_t *)&operation->preHashedMessage[EDDSA_SIGN_FINALIZE_MAX_LENGTH];
            object->inputLengthRemaining -= EDDSA_SIGN_FINALIZE_MAX_LENGTH;
        }

        /* If remaining input length is larger than 4095, then hash the excess outside of the
         * EdDSA HSM operation sequence. */
        if (object->inputLengthRemaining > EDDSA_SIGN_UPDATE_MAX_LENGTH)
        {
            status = EDDSALPF3HSM_computeIntermediateHash(handle,
                                                          EDDSALPF3HSM_computeIntermediateHashFinalPostProcessing);
        }
        else
        {
            status = EDDSALPF3HSM_signFinalize(handle);
        }
    }

    if (status != EDDSA_STATUS_SUCCESS)
    {
        /* Free four assets:
         * - Private key.
         * - Public key.
         * - ECC curve parameters.
         * - Digest asset.
         *
         * Ignore the return value here since the driver status code is error.
         */
        (void)EDDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    object->returnStatus = status;
}

/*
 *  ======== EDDSALPF3HSM_signUpdate ========
 */
static int_fast16_t EDDSALPF3HSM_signUpdate(EDDSA_Handle handle)
{
    /* Implementation for the update phase of the signing process
     * This function should handle the intermediate steps of the signing operation
     * and return the appropriate status.
     */
    EDDSALPF3HSM_Object *object    = handle->object;
    EDDSA_OperationSign *operation = (EDDSA_OperationSign *)object->operation;
    int_fast16_t status            = EDDSA_STATUS_ERROR;
    int_fast16_t hsmRetval         = HSMLPF3_STATUS_ERROR;

    if (object->inputLengthRemaining == 0U)
    {
        object->input       = (uint8_t *)operation->preHashedMessage;
        object->inputLength = operation->preHashedMessageLength;
    }
    else
    {
        object->input = (uint8_t *)&operation
                            ->preHashedMessage[operation->preHashedMessageLength - object->inputLengthRemaining];
        object->inputLength = object->inputLengthRemaining;
    }

    /* Populates the HSMLPF3 commandToken as a sign update token */
    HSMLPF3_constructEDDSASignUpdatePhysicalToken(object);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    EDDSALPF3HSM_signUpdatePostProcessing,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Wait for a result token from the HSM IP */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    if (hsmRetval == HSMLPF3_STATUS_ERROR)
    {
        /* Free four assets:
         * - Private key.
         * - Public key.
         * - ECC curve parameters.
         * - Digest asset.
         *
         * Ignore the return value here since the driver status code is error.
         */
        (void)EDDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== EDDSALPF3HSM_signFinalize ========
 */
static int_fast16_t EDDSALPF3HSM_signFinalize(EDDSA_Handle handle)
{
    EDDSALPF3HSM_Object *object    = handle->object;
    EDDSA_OperationSign *operation = (EDDSA_OperationSign *)object->operation;
    int_fast16_t status            = EDDSA_STATUS_ERROR;
    int_fast16_t hsmRetval         = HSMLPF3_STATUS_ERROR;

    if (object->inputLengthRemaining != operation->preHashedMessageLength)
    {
        object->input = (uint8_t *)&operation
                            ->preHashedMessage[operation->preHashedMessageLength - object->inputLengthRemaining];
        object->inputLength = object->inputLengthRemaining;
    }

    /* Populates the HSMLPF3 commandToken as a sign finalize token */
    HSMLPF3_constructEDDSASignFinalizePhysicalToken(object);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    EDDSALPF3HSM_eddsaPostProcessing,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Wait for a result token from the HSM IP */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    if (hsmRetval == HSMLPF3_STATUS_ERROR)
    {
        /* Free four assets:
         * - Private key.
         * - Public key.
         * - ECC curve parameters.
         * - Digest asset.
         *
         * Ignore the return value here since the driver status code is error.
         */
        (void)EDDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== EDDSA_verify ========
 */
int_fast16_t EDDSA_verify(EDDSA_Handle handle, EDDSA_OperationVerify *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    object->privateKey          = NULL;
    object->publicKey           = (CryptoKey *)operation->theirPublicKey;
    object->operationType       = EDDSA_OPERATION_TYPE_VERIFY;
    object->operation           = (EDDSA_Operation *)operation;

    /* Initialize assets as unallocated so that a cleanup operation
     * doesn't attempt to free an asset that doesn't exist yet.
     */
    object->privateKeyAssetID = 0U;
    object->publicKeyAssetID  = 0U;
    object->paramAssetID      = 0U;
    object->tempAssetID       = 0U;

    /* HSM limitation: The message length must be greater than 0. */
    if (operation->preHashedMessageLength == 0U)
    {
        return EDDSALPF3HSM_STATUS_INVALID_MESSAGE_LENGTH;
    }

    /* Perform the following operations:
     * - Check the HSM HW status
     * - Setup Driver metadata and EC curve parameters
     * - Create assets for the operation's key and EC parameters
     * - Attempt to acquire access semaphore for HSM operations.
     */
    status = EDDSALPF3HSM_initializeHSMOperations(handle);

    if (status != EDDSA_STATUS_SUCCESS)
    {
        return status;
    }

    return EDDSALPF3HSM_verifyInitial(handle);
}

/*
 *  ======== EDDSALPF3HSM_computeIntermediateHashVerifyFinalPostProcessing ========
 */
inline static void EDDSALPF3HSM_computeIntermediateHashVerifyFinalPostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle         = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int8_t tokenResult          = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->inputLengthRemaining -= object->inputLength;

        if (object->inputLengthRemaining > EDDSA_SIGN_UPDATE_MAX_LENGTH)
        {
            status =
                EDDSALPF3HSM_computeIntermediateHash(handle,
                                                     EDDSALPF3HSM_computeIntermediateHashVerifyFinalPostProcessing);
        }
        else
        {
            status = EDDSALPF3HSM_verifyFinalize(handle);
        }
    }
    else
    {
        status = EDDSA_STATUS_HASH_UNAVAILABLE;
    }

    if (status != EDDSA_STATUS_SUCCESS)
    {
        /* Free four assets:
         * - Private key.
         * - Public key.
         * - ECC curve parameters.
         * - Digest asset.
         *
         * Ignore the return value here since the driver status code is error.
         */
        (void)EDDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    object->returnStatus = status;
}

/*
 *  ======== EDDSALPF3HSM_verifyInitialPostProcessing ========
 */
inline static void EDDSALPF3HSM_verifyInitialPostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle              = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object      = handle->object;
    EDDSA_OperationVerify *operation = (EDDSA_OperationVerify *)object->operation;
    int_fast16_t status              = EDDSA_STATUS_ERROR;
    int8_t tokenResult               = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->stateAssetID = HSMLPF3_getResultAssetID();

        object->input = (uint8_t *)&operation->preHashedMessage[object->inputLength];
        object->inputLengthRemaining -= object->inputLength;

        /* The EdDSA verify sequence of operations:
         * - Verify initial phase which can only process up to 64 bytes. (If total length is less than 64 bytes, then
         * pass the entire message.)
         * - If message length is greater than 64 bytes (Sign initial max input length) + 4095 (Sign update max input
         * length), then compute intermediate hash outside of the EdDSA HSM operation sequence.
         * - Verify finalize phase which can process up to 4095 bytes.
         */
        if (operation->preHashedMessageLength > (EDDSA_VERIFY_INITIAL_MAX_LENGTH + EDDSA_SIGN_UPDATE_MAX_LENGTH))
        {
            status =
                EDDSALPF3HSM_computeIntermediateHash(handle,
                                                     EDDSALPF3HSM_computeIntermediateHashVerifyFinalPostProcessing);
        }
        else
        {
            status = EDDSALPF3HSM_verifyFinalize(handle);
        }
    }
    else if (tokenResult == EIP130TOKEN_RESULT_INVALID_ASSET)
    {
        status = EDDSA_STATUS_PUBLIC_KEY_NOT_ON_CURVE;
    }

    if (status != EDDSA_STATUS_SUCCESS)
    {
        /* Free four assets:
         * - Private key.
         * - Public key.
         * - ECC curve parameters.
         * - Digest asset.
         *
         * Ignore the return value here since the driver status code is error.
         */
        (void)EDDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    object->returnStatus = status;
}

/*
 *  ======== EDDSALPF3HSM_verifyInitial ========
 */
static int_fast16_t EDDSALPF3HSM_verifyInitial(EDDSA_Handle handle)
{
    EDDSALPF3HSM_Object *object      = handle->object;
    EDDSA_OperationVerify *operation = (EDDSA_OperationVerify *)object->operation;
    int_fast16_t status              = EDDSA_STATUS_ERROR;
    int_fast16_t hsmRetval           = HSMLPF3_STATUS_ERROR;

    object->input                = (uint8_t *)operation->preHashedMessage;
    object->inputLength          = operation->preHashedMessageLength;
    object->inputLengthRemaining = operation->preHashedMessageLength;

    if (operation->preHashedMessageLength > EDDSA_VERIFY_INITIAL_MAX_LENGTH)
    {
        object->inputLength = EDDSA_VERIFY_INITIAL_MAX_LENGTH;
    }

    /* Signature passed by the user must be converted to a format that the HSM IP requires it to be.
     * A 32-bit word header is attached at the beginning of each sub-vector component includes:
     * - The length of the sub-vector component
     * - Total number of sub-vector components
     * - Domain ID of the sub-vector component
     * - Index of each sub-vector component
     * The data in the body of the sub-vector component should be in little endian format.
     */
    memset(&object->signature[0], 0, sizeof(object->signature));
    HSMLPF3_asymDsaSignatureToHW(operation->R,
                                 operation->S,
                                 HSMLPF3_LITTLE_ENDIAN_KEY,
                                 EDDSA_CURVE_LENGTH_25519,
                                 &object->signature[0]);

    /* Populates the HSMLPF3 commandToken as a verify initial token */
    HSMLPF3_constructEDDSAVerifyInitialPhysicalToken(object);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    EDDSALPF3HSM_verifyInitialPostProcessing,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Wait for a result token from the HSM IP */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    if (hsmRetval == HSMLPF3_STATUS_ERROR)
    {
        /* Free four assets:
         * - Private key.
         * - Public key.
         * - ECC curve parameters.
         * - Digest asset.
         *
         * Ignore the return value here since the driver status code is error.
         */
        (void)EDDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== EDDSALPF3HSM_verifyFinalize ========
 */
static int_fast16_t EDDSALPF3HSM_verifyFinalize(EDDSA_Handle handle)
{
    EDDSALPF3HSM_Object *object      = handle->object;
    EDDSA_OperationVerify *operation = (EDDSA_OperationVerify *)object->operation;
    int_fast16_t status              = EDDSA_STATUS_ERROR;
    int_fast16_t hsmRetval           = HSMLPF3_STATUS_ERROR;

    if (object->inputLengthRemaining == 0U)
    {
        object->input       = (uint8_t *)operation->preHashedMessage;
        object->inputLength = operation->preHashedMessageLength;
    }
    else
    {
        object->input = (uint8_t *)&operation
                            ->preHashedMessage[operation->preHashedMessageLength - object->inputLengthRemaining];
        object->inputLength = object->inputLengthRemaining;
    }

    /* Populates the HSMLPF3 commandToken as a verify finalize token */
    HSMLPF3_constructEDDSAVerifyFinalizePhysicalToken(object);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    EDDSALPF3HSM_eddsaPostProcessing,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Wait for a result token from the HSM IP */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    if (hsmRetval == HSMLPF3_STATUS_ERROR)
    {
        /* Free four assets:
         * - Private key.
         * - Public key.
         * - ECC curve parameters.
         * - Digest asset.
         *
         * Ignore the return value here since the driver status code is error.
         */
        (void)EDDSALPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }
    return status;
}

/*
 *  ======== EDDSA_cancelOperation ========
 */
int_fast16_t EDDSA_cancelOperation(EDDSA_Handle handle)
{
    /* Since the FSM requires multiple token submissions to the HSM hardware happening in a HWI context,
     * cancelling an operation is not possible.
     */
    return EDDSA_STATUS_FEATURE_NOT_SUPPORTED;
}

/*
 *  ======== EDDSALPF3HSM_createAndLoadAllAssets ========
 */
static int_fast16_t EDDSALPF3HSM_createAndLoadAllAssets(EDDSA_Handle handle)
{
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_SUCCESS;

    if ((object->operationType == EDDSA_OPERATION_TYPE_GENERATE_PUBLIC_KEY) ||
        (object->operationType == EDDSA_OPERATION_TYPE_SIGN))
    {
        /* Perform two HSM operations:
         * - Create An asset that holds the operation's asymmetric private key,
         *      if the operation is generate public key or sign.
         * - Load the operation's private key onto HSM RAM
         */
        status = EDDSALPF3HSM_createAndLoadPrivateKeyAssetID(handle);

        if (status != EDDSA_STATUS_SUCCESS)
        {
            return status;
        }
    }

    /* Perform two HSM operations:
     * - Create An asset that holds the operation's asymmetric public key
     * - Load the operation's public key onto HSM RAM, ony in the case of sign and verify operation.
     */
    status = EDDSALPF3HSM_createAndLoadPublicKeyAssetID(handle);

    if (status != EDDSA_STATUS_SUCCESS)
    {
        return status;
    }

    if ((object->operationType == EDDSA_OPERATION_TYPE_SIGN) || (object->operationType == EDDSA_OPERATION_TYPE_VERIFY))
    {
        /* Peform a single HSM operation:
         * - Create an asset that holds the operation's intermediate digest.
         */
        status = EDDSALPF3HSM_createTempAssetID(handle);

        if (status != EDDSA_STATUS_SUCCESS)
        {
            return status;
        }
    }

    /* Perform two HSM operations:
     * - Create an asset that holds the operation's ECC curve parameters
     * - Load the appropriate ECC curve parameters onto HSM RAM
     */
    status = EDDSALPF3HSM_createAndLoadECurveAssetID(handle);

    if (status != EDDSA_STATUS_SUCCESS)
    {
        return status;
    }

    return status;
}

/*
 *  ================ APIs to Handle Private Key Asset ================
 */

/*
 *  ======== EDDSALPF3HSM_createAndLoadPrivateKeyAssetID ========
 */
static int_fast16_t EDDSALPF3HSM_createAndLoadPrivateKeyAssetID(EDDSA_Handle handle)
{
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    EDDSALPF3HSM_Object *object = handle->object;
    bool isKeyStoreKeyMaterial  = false;
    uint8_t *keyMaterial;

#if (ENABLE_KEY_STORAGE == 1)
    uint8_t KeyStore_keyingMaterial[EDDSALPF3HSM_PRIVATE_KEY_SIZE];
    /* This is the intended usage of the key. Providing the intended usage allows KeyStore
     * to verify that the key being retrieved allows this usage. However, for CC27XX,
     * this check is performed in the PSA Crypto Wrapper as opposed to within KeyStore.
     */
    KeyStore_PSA_KeyUsage usage = KEYSTORE_PSA_KEY_USAGE_DERIVE;

    if (object->privateKey->encoding == CryptoKey_KEYSTORE_HSM)
    {
        keyMaterial = &KeyStore_keyingMaterial[0];

        status = KeyStore_PSA_retrieveFromKeyStore(object->privateKey,
                                                   &KeyStore_keyingMaterial[0],
                                                   sizeof(KeyStore_keyingMaterial),
                                                   &object->privateKeyAssetID,
                                                   KEYSTORE_PSA_ALG_PURE_EDDSA,
                                                   usage);

        if (status != KEYSTORE_PSA_STATUS_SUCCESS)
        {
            return status;
        }
        else if (object->privateKeyAssetID != 0)
        {
            /* In this case, we already retrieved an asset from KeyStore,
             * so we don't need the driver to create and load an asset itself.
             */
            object->driverCreatedPrivateKeyAsset = false;

            return status;
        }
        else
        {
            isKeyStoreKeyMaterial = true;
        }
    }
    else
#endif
    {
        keyMaterial = object->privateKey->u.plaintext.keyMaterial;
    }

    /* Constructs the policy for the asset and submits an asset create token to the HSM IP */
    status = EDDSALPF3HSM_createPrivateKeyAsset(handle);
    if (status == EDDSA_STATUS_SUCCESS)
    {
        /* Now that the driver has successfully created an asset, object->privateKeyAssetID is now non-zero.
         * If any failure condition happens after this moment, the cleanup will expect
         * object->driverCreatedPrivateKeyAsset to be accurate, since the privateKeyAssetID will reflect that there
         * is an asset to free, and the cleanup will need to know how to do that.
         */
        object->driverCreatedPrivateKeyAsset = true;

        /* Constructs an asset load token and submits it to the HSM IP */
        status = EDDSALPF3HSM_loadPrivateKeyAsset(handle, keyMaterial, isKeyStoreKeyMaterial);
    }

    return status;
}

/*
 *  ======== EDDSALPF3HSM_createPrivateKeyAssetPostProcessing ========
 */
static inline void EDDSALPF3HSM_createPrivateKeyAssetPostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle         = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int8_t tokenResult          = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->privateKeyAssetID = HSMLPF3_getResultAssetID();
        status                    = EDDSA_STATUS_SUCCESS;
    }

    object->hsmStatus = status;
}

/*
 *  ======== EDDSALPF3HSM_createKeyAsset ========
 */
static int_fast16_t EDDSALPF3HSM_createPrivateKeyAsset(EDDSA_Handle handle)
{
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;
    EDDSALPF3HSM_Object *object = handle->object;
    uint64_t assetPolicy        = 0U;
    uint32_t assetSize          = HSM_ASYM_DATA_SIZE_VWB(EDDSA_CURVE_LENGTH_25519);

    /* Operation (Lower 16-bits + general Operation) + Direction. No Mode */
    assetPolicy = HSM_ASSET_POLICY_ASYM_SIGNVRFY | HSM_ASSET_POLICY_NONMODIFIABLE | HSM_ASSET_POLICY_ASYM_ED25519 |
                  HSM_ASSET_POLICY_ASYM_SHA2_512 | HSM_ASSET_POLICY_PRIVATEDATA;

    /* Populates the HSMLPF3 commandToken as a create asset token */
    HSMLPF3_constructCreateAssetToken(assetPolicy, assetSize);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    EDDSALPF3HSM_createPrivateKeyAssetPostProcessing,
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
 *  ======== EDDSALPF3HSM_loadKeyAssetPostProcessing ========
 */
static inline void EDDSALPF3HSM_loadPrivateKeyAssetPostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle         = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int8_t tokenResult          = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = EDDSA_STATUS_SUCCESS;
    }

    object->hsmStatus = status;
}

/*
 *  ======== EDDSALPF3HSM_loadPrivateKeyAsset ========
 */
static int_fast16_t EDDSALPF3HSM_loadPrivateKeyAsset(EDDSA_Handle handle, uint8_t *key, bool isFromKeyStore)
{
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;
    EDDSALPF3HSM_Object *object = handle->object;
    uint8_t *key_p              = object->privateKey->u.plaintext.keyMaterial;
    uint32_t componentLength    = HSM_ASYM_DATA_SIZE_VWB(EDDSA_CURVE_LENGTH_25519);

    if (!isFromKeyStore)
    {
        /* Initialize the object->output buffer that will hold the single or
         * multi-component vector for the operation's key.
         */
        memset(&object->signature[0], 0, componentLength);

        /* In the case of a sign operation, there is one sub-vector component that needs to
         * modified to comply with HSM IP format requirements.
         * Modifications include:
         * - Properly populating the first 32-bit word as the vector header that describes the key
         * - Copy the key data to the vector in little-endian format.
         */
        HSMLPF3_asymDHPriKeyToHW(key_p,
                                 EDDSA_CURVE_LENGTH_25519,
                                 EDDSA_DOMAIN_ID_TWISTED_EDWARDS,
                                 HSMLPF3_LITTLE_ENDIAN_KEY,
                                 &object->signature[0]);

        /* Populates the HSMLPF3 commandToken as a load asset token */
        (void)HSMLPF3_constructLoadPlaintextAssetToken(&object->signature[0],
                                                       componentLength,
                                                       object->privateKeyAssetID);
    }
#if (ENABLE_KEY_STORAGE == 1)
    else
    {
        /* Populates the HSMLPF3 commandToken as a load asset token */
        (void)HSMLPF3_constructLoadPlaintextAssetToken(key, componentLength, object->privateKeyAssetID);
    }
#endif

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    EDDSALPF3HSM_loadPrivateKeyAssetPostProcessing,
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

    if (hsmRetval == HSMLPF3_STATUS_ERROR)
    {
        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ================ APIs to Handle Public Key Asset ================
 */

/*
 *  ======== EDDSALPF3HSM_createAndLoadPublicKeyAssetID ========
 */
static int_fast16_t EDDSALPF3HSM_createAndLoadPublicKeyAssetID(EDDSA_Handle handle)
{
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    EDDSALPF3HSM_Object *object = handle->object;
    bool isKeyStoreKeyMaterial  = false;
    uint8_t *keyMaterial;

#if (ENABLE_KEY_STORAGE == 1)
    uint8_t KeyStore_keyingMaterial[EDDSALPF3HSM_PRIVATE_KEY_SIZE];
    /* This is the intended usage of the key. Providing the intended usage allows KeyStore
     * to verify that the key being retrieved allows this usage. However, for CC27XX,
     * this check is performed in the PSA Crypto Wrapper as opposed to within KeyStore.
     */
    KeyStore_PSA_KeyUsage usage = KEYSTORE_PSA_KEY_USAGE_DERIVE;

    if (object->operationType == EDDSA_OPERATION_TYPE_SIGN)
    {
        usage = KEYSTORE_PSA_KEY_USAGE_SIGN_MESSAGE;
    }
    else if (object->operationType == EDDSA_OPERATION_TYPE_VERIFY)
    {
        usage = KEYSTORE_PSA_KEY_USAGE_VERIFY_MESSAGE;
    }

    if ((object->publicKey->encoding == CryptoKey_KEYSTORE_HSM) &&
        ((object->operationType == EDDSA_OPERATION_TYPE_SIGN) ||
         (object->operationType == EDDSA_OPERATION_TYPE_VERIFY)))
    {
        keyMaterial = &KeyStore_keyingMaterial[0];

        status = KeyStore_PSA_retrieveFromKeyStore(object->publicKey,
                                                   &KeyStore_keyingMaterial[0],
                                                   sizeof(KeyStore_keyingMaterial),
                                                   &object->publicKeyAssetID,
                                                   KEYSTORE_PSA_ALG_PURE_EDDSA,
                                                   usage);

        if (status != KEYSTORE_PSA_STATUS_SUCCESS)
        {
            return status;
        }
        else if (object->publicKeyAssetID != 0)
        {
            /* In this case, we already retrieved an asset from KeyStore,
             * so we don't need the driver to create and load an asset itself.
             */
            object->driverCreatedPublicKeyAsset = false;

            return status;
        }
        else
        {
            isKeyStoreKeyMaterial = true;
        }
    }
    else
#endif
    {
        keyMaterial = object->publicKey->u.plaintext.keyMaterial;
    }

    /* Constructs the policy for the asset and submits an asset create token to the HSM IP */
    status = EDDSALPF3HSM_createPublicKeyAsset(handle);
    if (status == EDDSA_STATUS_SUCCESS)
    {
        /* Now that the driver has successfully created an asset, object->publicKeyAssetID is now non-zero.
         * If any failure condition happens after this moment, the cleanup will expect
         * object->driverCreatedKeyAsset to be accurate, since the publicKeyAssetID will reflect that there
         * is an asset to free, and the cleanup will need to know how to do that.
         */
        object->driverCreatedPublicKeyAsset = true;

        if ((object->operationType == EDDSA_OPERATION_TYPE_SIGN) ||
            (object->operationType == EDDSA_OPERATION_TYPE_VERIFY))
        {
            /* Constructs an asset load token and submits it to the HSM IP */
            status = EDDSALPF3HSM_loadPublicKeyAsset(handle, keyMaterial, isKeyStoreKeyMaterial);
        }
    }

    return status;
}

/*
 *  ======== EDDSALPF3HSM_createPublicKeyAssetPostProcessing ========
 */
static inline void EDDSALPF3HSM_createPublicKeyAssetPostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle         = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int8_t tokenResult          = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->publicKeyAssetID = HSMLPF3_getResultAssetID();
        status                   = EDDSA_STATUS_SUCCESS;
    }

    object->hsmStatus = status;
}

/*
 *  ======== EDDSALPF3HSM_createPublicKeyAsset ========
 */
static int_fast16_t EDDSALPF3HSM_createPublicKeyAsset(EDDSA_Handle handle)
{
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;
    EDDSALPF3HSM_Object *object = handle->object;
    uint64_t assetPolicy        = 0U;
    uint32_t assetSize          = HSM_ASYM_DATA_SIZE_VWB(EDDSA_CURVE_LENGTH_25519);

    /* Operation (Lower 16-bits + general Operation) + Direction. No Mode */
    assetPolicy = HSM_ASSET_POLICY_ASYM_SIGNVRFY | HSM_ASSET_POLICY_NONMODIFIABLE | HSM_ASSET_POLICY_ASYM_ED25519 |
                  HSM_ASSET_POLICY_ASYM_SHA2_512;

    /* Populates the HSMLPF3 commandToken as a create asset token */
    HSMLPF3_constructCreateAssetToken(assetPolicy, assetSize);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    EDDSALPF3HSM_createPublicKeyAssetPostProcessing,
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
 *  ======== EDDSALPF3HSM_loadPublicKeyAssetPostProcessing ========
 */
static inline void EDDSALPF3HSM_loadPublicKeyAssetPostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle         = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int8_t tokenResult          = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = EDDSA_STATUS_SUCCESS;
    }

    object->hsmStatus = status;
}

/*
 *  ======== EDDSALPF3HSM_loadPublicKeyAsset ========
 */
static int_fast16_t EDDSALPF3HSM_loadPublicKeyAsset(EDDSA_Handle handle, uint8_t *key, bool isFromKeyStore)
{
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;
    EDDSALPF3HSM_Object *object = handle->object;
    uint32_t componentLength    = HSM_ASYM_DATA_SIZE_VWB(EDDSA_CURVE_LENGTH_25519);

    if (!isFromKeyStore)
    {
        /* Initialize the object->output buffer that will hold the single or
         * multi-component vector for the operation's key.
         */
        memset(&object->signature[0], 0, componentLength);

        /* In the case of a sign operation, there is one sub-vector component that needs to be
         * modified to comply with HSM IP format requirements.
         * Modifications include:
         * - Properly populating the first 32-bit word as the vector header that describes the key
         * - Copy the key data to the vector in little-endian format.
         */
        HSMLPF3_asymDHPubKeyToHW(object->publicKey->u.plaintext.keyMaterial,
                                 EDDSA_CURVE_LENGTH_25519,
                                 HSM_ASYM_CURVE25519_PUB_KEY_VCOUNT,
                                 EDDSA_DOMAIN_ID_TWISTED_EDWARDS,
                                 HSMLPF3_LITTLE_ENDIAN_KEY,
                                 &object->signature[0]);

        /* Populates the HSMLPF3 commandToken as a load asset token */
        (void)HSMLPF3_constructLoadPlaintextAssetToken(&object->signature[0],
                                                       componentLength,
                                                       object->publicKeyAssetID);
    }
#if (ENABLE_KEY_STORAGE == 1)
    else
    {
        /* Populates the HSMLPF3 commandToken as a load asset token */
        (void)HSMLPF3_constructLoadPlaintextAssetToken(key, componentLength, object->publicKeyAssetID);
    }
#endif

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    EDDSALPF3HSM_loadPublicKeyAssetPostProcessing,
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
 *  ================ APIs to Handle Temp Digest AssetID ================
 */

/*
 *  ======== EDDSALPF3HSM_createTempAssetIDPostProcessing ========
 */
static inline void EDDSALPF3HSM_createTempAssetIDPostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle         = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int8_t tokenResult          = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = EDDSA_STATUS_SUCCESS;

        object->tempAssetID = HSMLPF3_getResultAssetID();
    }

    object->hsmStatus = status;
}

/*
 *  ======== EDDSALPF3HSM_createAndLoadECurveAssetID ========
 */
static int_fast16_t EDDSALPF3HSM_createTempAssetID(EDDSA_Handle handle)
{
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;
    EDDSALPF3HSM_Object *object = handle->object;
    uint64_t assetPolicy        = 0U;
    uint32_t assetSize          = SHA2_DIGEST_LENGTH_BYTES_512;

    assetPolicy = HSM_ASSET_POLICY_SYM_HASH | HSM_ASSET_POLICY_TEMPORARY | HSM_ASSET_POLICY_SYM_AIH_SHA2_512 |
                  HSM_ASSET_POLICY_DIR_ENC_GEN;

    /* Populates the HSMLPF3 commandToken as a create asset token */
    HSMLPF3_constructCreateAssetToken(assetPolicy, assetSize);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    EDDSALPF3HSM_createTempAssetIDPostProcessing,
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
 *  ================ APIs to Handle EC Curve Asset ================
 */

/*
 *  ======== EDDSALPF3HSM_createAndLoadECurveAssetID ========
 */
static int_fast16_t EDDSALPF3HSM_createAndLoadECurveAssetID(EDDSA_Handle handle)
{
    int_fast16_t status = EDDSA_STATUS_ERROR;

    status = EDDSALPF3HSM_createECurveAsset(handle);
    if (status == EDDSA_STATUS_SUCCESS)
    {
        status = EDDSALPF3HSM_loadECurve(handle);
    }

    return status;
}

/*
 *  ======== EDDSALPF3HSM_createECurveAssetPostProcessing ========
 */
static inline void EDDSALPF3HSM_createECurveAssetPostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle         = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int8_t tokenResult          = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->paramAssetID = HSMLPF3_getResultAssetID();
        status               = EDDSA_STATUS_SUCCESS;
    }

    object->hsmStatus = status;
}

/*
 *  ======== EDDSALPF3HSM_createECurveAsset ========
 */
static int_fast16_t EDDSALPF3HSM_createECurveAsset(EDDSA_Handle handle)
{
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;
    EDDSALPF3HSM_Object *object = handle->object;
    uint64_t assetPolicy        = HSM_ASSET_POLICY_ASYM_KEYPARAMS | HSM_ASSET_POLICY_NONMODIFIABLE;

    /* Populates the HSMLPF3 commandToken as a create asset token */
    HSMLPF3_constructCreateAssetToken(assetPolicy, object->curveParamSize);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    EDDSALPF3HSM_createECurveAssetPostProcessing,
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
 *  ======== EDDSALPF3HSM_loadECurvePostProcessing ========
 */
static inline void EDDSALPF3HSM_loadECurvePostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle         = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int8_t tokenResult          = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = EDDSA_STATUS_SUCCESS;
    }

    object->hsmStatus = status;
}

/*
 *  ======== EDDSALPF3HSM_loadECurve ========
 */
static int_fast16_t EDDSALPF3HSM_loadECurve(EDDSA_Handle handle)
{
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;
    EDDSALPF3HSM_Object *object = handle->object;

    /* Populates the HSMLPF3 commandToken as a load asset token */
    (void)HSMLPF3_constructLoadPlaintextAssetToken(object->curveParam, object->curveParamSize, object->paramAssetID);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    EDDSALPF3HSM_loadECurvePostProcessing,
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
 *  ================ APIs to All Free Assets ================
 */

/*
 *  ======== EDDSALPF3HSM_freeAllAssets ========
 */
static int_fast16_t EDDSALPF3HSM_freeAllAssets(EDDSA_Handle handle)
{
    int_fast16_t status         = EDDSA_STATUS_SUCCESS;
    EDDSALPF3HSM_Object *object = handle->object;

    if (object->privateKeyAssetID != 0U)
    {
        /* If the object has a stored privateKeyAssetID, then driverCreatedKeyAsset MUST
         * be set. It can only be false if KeyStore is enabled. If it is false,
         * it means that we retrieved an asset directly from KeyStore, so the
         * driver should not free the asset. Instead, the driver should direct
         * KeyStore to free the asset (which will perform the necessary persistence
         * check and only free the asset if it should be freed).
         */
        if (object->driverCreatedPrivateKeyAsset)
        {
            status = EDDSALPF3HSM_freeAssetID(handle, object->privateKeyAssetID);
        }
#if (ENABLE_KEY_STORAGE == 1)
        else
        {
            KeyStore_PSA_KeyFileId keyID;

            GET_KEY_ID(keyID, object->privateKey->u.keyStore.keyID);

            status = KeyStore_PSA_assetPostProcessing(keyID);
        }
#endif

        /* This condition captures success from both the EDDSALPF3HSM and
         * KeyStore_PSA APIs.
         */
        if (status == EDDSA_STATUS_SUCCESS)
        {
            object->privateKeyAssetID = 0;
        }
    }

    if (object->publicKeyAssetID != 0U)
    {
        /* If the object has a stored privateKeyAssetID, then driverCreatedKeyAsset MUST
         * be set. It can only be false if KeyStore is enabled. If it is false,
         * it means that we retrieved an asset directly from KeyStore, so the
         * driver should not free the asset. Instead, the driver should direct
         * KeyStore to free the asset (which will perform the necessary persistence
         * check and only free the asset if it should be freed).
         */
        if (object->driverCreatedPublicKeyAsset)
        {
            status = EDDSALPF3HSM_freeAssetID(handle, object->publicKeyAssetID);
        }
#if (ENABLE_KEY_STORAGE == 1)
        else
        {
            KeyStore_PSA_KeyFileId keyID;

            GET_KEY_ID(keyID, object->publicKey->u.keyStore.keyID);

            status = KeyStore_PSA_assetPostProcessing(keyID);
        }
#endif

        /* This condition captures success from both the EDDSALPF3HSM and
         * KeyStore_PSA APIs.
         */
        if (status == EDDSA_STATUS_SUCCESS)
        {
            object->publicKeyAssetID = 0;
        }
    }

    if (object->tempAssetID != 0U)
    {
        if (EDDSALPF3HSM_freeAssetID(handle, object->tempAssetID) != EDDSA_STATUS_SUCCESS)
        {
            status = EDDSA_STATUS_ERROR;
        }
        else
        {
            object->tempAssetID = 0;
        }
    }

    if (object->paramAssetID != 0U)
    {
        if (EDDSALPF3HSM_freeAssetID(handle, object->paramAssetID) != EDDSA_STATUS_SUCCESS)
        {
            status = EDDSA_STATUS_ERROR;
        }
        else
        {
            object->paramAssetID = 0;
        }
    }

    if (object->stateAssetID != 0U)
    {
        if (EDDSALPF3HSM_freeAssetID(handle, object->stateAssetID) != EDDSA_STATUS_SUCCESS)
        {
            status = EDDSA_STATUS_ERROR;
        }
        else
        {
            object->stateAssetID = 0;
        }
    }

    return status;
}

/*
 *  ======== EDDSALPF3HSM_freeAssetPostProcessing ========
 */
static inline void EDDSALPF3HSM_freeAssetPostProcessing(uintptr_t arg0)
{
    EDDSA_Handle handle         = (EDDSA_Handle)arg0;
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int8_t tokenResult          = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = EDDSA_STATUS_SUCCESS;
    }

    object->hsmStatus = status;
}

/*
 *  ======== EDDSALPF3HSM_freeAssetID ========
 */
static int_fast16_t EDDSALPF3HSM_freeAssetID(EDDSA_Handle handle, uint32_t assetID)
{
    EDDSALPF3HSM_Object *object = handle->object;
    int_fast16_t status         = EDDSA_STATUS_ERROR;
    int_fast16_t hsmRetval      = HSMLPF3_STATUS_ERROR;

    /* Populates the HSMLPF3 commandToken as a delete asset token */
    (void)HSMLPF3_constructDeleteAssetToken(assetID);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    EDDSALPF3HSM_freeAssetPostProcessing,
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
