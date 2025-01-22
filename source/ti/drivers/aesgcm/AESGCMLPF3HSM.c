/*
 * Copyright (c) 2023-2024, Texas Instruments Incorporated
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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <ti/drivers/aesgcm/AESGCMLPF3HSM.h>
#include <ti/drivers/aesecb/AESECBLPF3.h>
#include <ti/drivers/aesctr/AESCTRLPF3.h>
#include <ti/drivers/AESCommon.h>
#include <ti/drivers/cryptoutils/aes/AESCommonLPF3.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKey.h>
#include <ti/drivers/cryptoutils/sharedresources/CryptoResourceLPF3.h>
#include <ti/drivers/cryptoutils/utils/CryptoUtils.h>

#include <ti/drivers/cryptoutils/hsm/HSMLPF3.h>
#include <ti/drivers/cryptoutils/hsm/HSMLPF3Utility.h>

#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/power/PowerCC27XX.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/aes.h)
#include DeviceFamily_constructPath(inc/hw_aes.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)

#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC23XX) && (ENABLE_KEY_STORAGE == 1))
    #error "Key storage is not supported for CC23XX"
#endif

#if (ENABLE_KEY_STORAGE == 1)
    #include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA.h>
    #include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA_helpers.h>
#endif

/* Forward declarations */
#if (ENABLE_KEY_STORAGE == 1)
static int_fast16_t AESGCMLPF3HSM_getKeyMaterial(AESGCMLPF3HSM_Object *object);
#endif
static void AESGCMLPF3HSM_setupObjectMetaData(AESGCMLPF3HSM_Object *object);
static int_fast16_t AESGCMLPF3HSM_oneStepOperation(AESGCM_Handle handle,
                                                   AESGCM_OneStepOperation *operation,
                                                   AESGCM_OperationType operationType);
static int_fast16_t AESGCMLPF3HSM_performHSMOperation(AESGCM_Handle handle);
static void AESGCMLPF3HSM_postProcessingCommon(AESGCM_Handle handle, int_fast16_t status, int8_t tokenResult);
static int_fast16_t AESGCMLPF3HSM_setupSegmentedOperation(AESGCM_Handle handle,
                                                          AESGCM_OperationType operationType,
                                                          const CryptoKey *key,
                                                          size_t totalAADLength,
                                                          size_t totalDataLength);
static int_fast16_t AESGCMLPF3HSM_addData(AESGCM_Handle handle,
                                          AESGCM_OperationType operationType,
                                          AESGCM_OperationUnion *operation);
static int_fast16_t AESGCMLPF3HSM_performFinalizeChecks(const AESGCMLPF3HSM_Object *object,
                                                        const AESGCM_SegmentedFinalizeOperation *operation);
static int_fast16_t AESGCMLPF3HSM_finalizeCommon(AESGCM_Handle handle,
                                                 AESGCM_OperationType operationType,
                                                 AESGCM_SegmentedFinalizeOperation *operation);
static int_fast16_t AESGCMLPF3HSM_createTempAssetID(AESGCM_Handle handle);
static int_fast16_t AESGCMLPF3HSM_freeAllAssets(AESGCM_Handle handle);
static int_fast16_t AESGCMLPF3HSM_freeTempAssetID(AESGCM_Handle handle);

/*
 *  ======== AESGCMLPF3HSM_getObject ========
 */
static inline AESGCMLPF3HSM_Object *AESGCMLPF3HSM_getObject(AESGCM_Handle handle)
{
    AESGCMLPF3HSM_Object *object = (AESGCMLPF3HSM_Object *)handle->object;
    DebugP_assert(object);

    return object;
}

/*
 *  ======== AESGCM_init ========
 */
void AESGCM_init(void)
{
    HSMLPF3_constructRTOSObjects();
}

/*
 *  ======== AESGCM_construct ========
 */
AESGCM_Handle AESGCM_construct(AESGCM_Config *config, const AESGCM_Params *params)
{
    DebugP_assert(config);

    AESGCM_Handle handle         = config;
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);

    /* Initialize and boot HSM */
    if (HSMLPF3_init() != HSMLPF3_STATUS_SUCCESS)
    {
        /* Upon HSM Boot failure, the AES-GCM Driver stores the failure status in the object. */
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
        params = &AESGCM_defaultParams;
    }

    DebugP_assert((params->returnBehavior != AESGCM_RETURN_BEHAVIOR_CALLBACK) || (params->callbackFxn != NULL));

    object->callbackFxn = params->callbackFxn;

    object->common.returnBehavior = (AES_ReturnBehavior)params->returnBehavior;

    if (params->returnBehavior == AESGCM_RETURN_BEHAVIOR_BLOCKING)
    {
        object->common.semaphoreTimeout = params->timeout;
    }
    else
    {
        object->common.semaphoreTimeout = SemaphoreP_NO_WAIT;
    }

    object->segmentedOperationInProgress = false;

    return handle;
}

/*
 *  ======== AESGCM_close ========
 */
void AESGCM_close(AESGCM_Handle handle)
{
    DebugP_assert(handle);
}

/*
 *  ======== AESGCM_oneStepEncrypt ========
 */
int_fast16_t AESGCM_oneStepEncrypt(AESGCM_Handle handle, AESGCM_OneStepOperation *operationStruct)
{
    return AESGCMLPF3HSM_oneStepOperation(handle, operationStruct, AESGCM_OPERATION_TYPE_ENCRYPT);
}

/*
 *  ======== AESGCM_oneStepDecrypt ========
 */
int_fast16_t AESGCM_oneStepDecrypt(AESGCM_Handle handle, AESGCM_OneStepOperation *operationStruct)
{
    return AESGCMLPF3HSM_oneStepOperation(handle, operationStruct, AESGCM_OPERATION_TYPE_DECRYPT);
}

/*
 *  ======== AESGCMLPF3HSM_oneStepOperation ========
 */
static int_fast16_t AESGCMLPF3HSM_oneStepOperation(AESGCM_Handle handle,
                                                   AESGCM_OneStepOperation *operation,
                                                   AESGCM_OperationType operationType)
{
    DebugP_assert(handle);
    DebugP_assert(operation);
    DebugP_assert(operation->key);
    /* No need to assert operationType since we control it within the driver */

    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status          = AESGCM_STATUS_SUCCESS;

    /* Verify input + AAD length is non-zero */
    if ((operation->inputLength + operation->aadLength) == 0U)
    {
        return AESGCM_STATUS_ERROR;
    }

    if (object->hsmStatus != HSMLPF3_STATUS_SUCCESS)
    {
        return AESGCM_STATUS_ERROR;
    }

    /* A segmented operation may have been started but not finalized yet */
    if (object->segmentedOperationInProgress)
    {
        return AESGCM_STATUS_ERROR;
    }

    object->operation           = (AESGCM_OperationUnion *)operation;
    object->operationType       = operationType;
    /* We will only change the returnStatus if there is an error or cancellation */
    object->common.returnStatus = AESGCM_STATUS_SUCCESS;

    /* Make internal copy of operational params */
    object->common.key = *(operation->key);
    object->input      = operation->input;
    object->output     = operation->output;
    object->mac        = operation->mac;
    object->aad        = operation->aad;
    object->iv         = operation->iv;

    object->inputLength     = operation->inputLength;
    object->totalDataLength = operation->inputLength;
    object->macLength       = operation->macLength;
    object->aadLength       = operation->aadLength;
    object->totalAADLength  = operation->aadLength;
    object->ivLength        = operation->ivLength;

    object->totalDataLengthRemaining = object->totalDataLength;
    object->totalAADLengthRemaining  = object->totalAADLength;

    object->tempAssetID = 0U;

    if ((operationType == AESGCM_OP_TYPE_ONESTEP_ENCRYPT) && (operation->macLength == 0U))
    {
        /* We are not supplied the macLength at this point of the segmented operation.
         * Therefore, the driver gives it a dummy length which is the max length until
         * the user supplies the desired macLength in _setLengths() API.
         */
        object->macLength = HSM_MAC_MAX_LENGTH;
    }

    if (!HSMLPF3_acquireLock(object->common.semaphoreTimeout, (uintptr_t)handle))
    {
        return AESGCM_STATUS_RESOURCE_UNAVAILABLE;
    }

    /* If input length is larger than 1 block and it is not a multiple of block-size,
     * then a one step operation becomes a segmented operation and requires a state
     * asset to store the intermediate state of the MAC within the HSM.
     */
    if ((operation->inputLength > AES_BLOCK_SIZE) &&
        (!HSM_IS_SIZE_MULTIPLE_OF_AES_BLOCK_SIZE((operation->inputLength))))
    {
        status = AESGCMLPF3HSM_createTempAssetID(handle);
    }

#if (ENABLE_KEY_STORAGE == 1)
    status = AESGCMLPF3HSM_getKeyMaterial(object);
#endif

    /* Clear the counter value */
    (void)memset(object->counter, 0U, sizeof(object->counter));

    if (status != AESGCM_STATUS_SUCCESS)
    {
        /* In the case of failure to initialize the operation, we need to free all assets allocated.
         * Capturing the return status of this operation is pointless since we are returning an
         * error code anyways.
         */
        (void)AESGCMLPF3HSM_freeTempAssetID(handle);

        HSMLPF3_releaseLock();
    }
    else
    {
        status = AESGCMLPF3HSM_performHSMOperation(handle);
    }

    return status;
}

/*
 *  ======== AESGCMLPF3HSM_postProcessingCommon ========
 */
static void AESGCMLPF3HSM_postProcessingCommon(AESGCM_Handle handle, int_fast16_t status, int8_t tokenResult)
{
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);

    if (status == AESGCM_STATUS_SUCCESS)
    {
        if (object->operationType == AESGCM_OP_TYPE_ONESTEP_ENCRYPT)
        {
            AESGCM_OneStepOperation *operation = (AESGCM_OneStepOperation *)object->operation;

            HSMLPF3_getAESEncryptTag(&operation->mac[0], operation->macLength);
        }
        else if ((object->operationType == AESGCM_OP_TYPE_FINALIZE_ENCRYPT) &&
                 ((object->inputLength != 0U) || (object->aadLength != 0U)))
        {
            AESGCM_SegmentedFinalizeOperation *operation = (AESGCM_SegmentedFinalizeOperation *)object->operation;

            HSMLPF3_getAESEncryptTag(&operation->mac[0], operation->macLength);
        }
        else if (object->operationType == AESGCM_OP_TYPE_DATA_ENCRYPT)
        {
            HSMLPF3_getAESEncryptTag((void *)&object->intermediateTag[0], object->macLength);
        }
        else if ((tokenResult == EIP130TOKEN_RESULT_VERIFY_ERROR) &&
                 ((object->operationType == AESGCM_OP_TYPE_FINALIZE_DECRYPT) ||
                  (object->operationType == AESGCM_OP_TYPE_ONESTEP_DECRYPT) ||
                  (object->operationType == AESGCM_OP_TYPE_DATA_DECRYPT)))
        {
            status = AESGCM_STATUS_MAC_INVALID;
        }
    }

    if ((object->operationType == AESGCM_OP_TYPE_FINALIZE_ENCRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_FINALIZE_DECRYPT))
    {
        object->segmentedOperationInProgress = false;
    }

    /* Free all assets. This includes key-related asset and state-related asset (temporary asset). */
    if (AESGCMLPF3HSM_freeAllAssets(handle) != AESGCM_STATUS_SUCCESS)
    {
        status = AESGCM_STATUS_ERROR;
    }

    object->common.returnStatus = status;

    HSMLPF3_releaseLock();

    if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK)
    {
        object->callbackFxn(handle, object->common.returnStatus, object->operation, object->operationType);
    }
}

/*
 *  ======== AESGCMLPF3HSM_processFinalBlockPostProcessing ========
 */
inline static void AESGCMLPF3HSM_processFinalBlockPostProcessing(uintptr_t arg0)
{
    AESGCM_Handle handle         = (AESGCM_Handle)arg0;
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status          = AESGCM_STATUS_ERROR;
    int32_t physicalResult       = HSMLPF3_getResultCode();
    int8_t tokenResult           = physicalResult & HSMLPF3_RETVAL_MASK;

    /* The HSM IP will throw an error when operation->macLength is zero despite it producing a correct
     * ciphertext/plaintext for both encrypt/decrypt operations and will compute a mac anyways.
     */
    if ((tokenResult == EIP130TOKEN_RESULT_SUCCESS) || ((tokenResult == EIP130TOKEN_RESULT_VERIFY_ERROR) &&
                                                        ((object->operationType == AESGCM_OP_TYPE_ONESTEP_DECRYPT) ||
                                                         (object->operationType == AESGCM_OP_TYPE_DATA_DECRYPT) ||
                                                         (object->operationType == AESGCM_OP_TYPE_FINALIZE_DECRYPT))))
    {
        object->totalDataLengthRemaining -= object->inputLength;

        status = AESGCM_STATUS_SUCCESS;
    }

    if (status == AESGCM_STATUS_SUCCESS)
    {
        uint8_t *output = NULL;

        if ((object->operationType == AESGCM_OP_TYPE_ONESTEP_ENCRYPT) ||
            (object->operationType == AESGCM_OP_TYPE_ONESTEP_DECRYPT))
        {
            AESGCM_OneStepOperation *operation = (AESGCM_OneStepOperation *)object->operation;
            output                             = operation->output + (object->totalDataLength - object->inputLength);
        }
        else if ((object->operationType == AESGCM_OP_TYPE_DATA_ENCRYPT) ||
                 (object->operationType == AESGCM_OP_TYPE_DATA_DECRYPT))
        {
            AESGCM_SegmentedDataOperation *operation = (AESGCM_SegmentedDataOperation *)object->operation;
            output = operation->output + (operation->inputLength - object->inputLength);
        }
        else
        {
            AESGCM_SegmentedFinalizeOperation *operation = (AESGCM_SegmentedFinalizeOperation *)object->operation;
            output = operation->output + (operation->inputLength - object->inputLength);
        }

        (void)memcpy(output, object->outputFinalBlock, object->inputLength);
    }

    AESGCMLPF3HSM_postProcessingCommon(handle, status, tokenResult);
}

/*
 *  ======== AESGCMLPF3HSM_processFinalBlock ========
 */
static int_fast16_t AESGCMLPF3HSM_processFinalBlock(AESGCM_Handle handle)
{
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status          = AESGCM_STATUS_ERROR;
    int_fast16_t hsmRetval       = HSMLPF3_STATUS_ERROR;
    size_t remainderLength       = 0U;
    bool saveIV                  = false;
    bool loadIV                  = true;

    if ((object->operationType == AESGCM_OP_TYPE_ONESTEP_ENCRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_ONESTEP_DECRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_FINALIZE_ENCRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_FINALIZE_DECRYPT))
    {
        remainderLength = object->totalDataLengthRemaining;
    }
    else
    {
        AESGCM_SegmentedDataOperation *operation = (AESGCM_SegmentedDataOperation *)object->operation;
        remainderLength                          = operation->inputLength - object->inputLength;
    }

    (void)memset(object->inputFinalBlock, 0, AES_BLOCK_SIZE);
    (void)memcpy(object->inputFinalBlock, object->input + object->inputLength, remainderLength);

    (void)memset(object->outputFinalBlock, 0, AES_BLOCK_SIZE);

    object->input       = object->inputFinalBlock;
    object->output      = object->outputFinalBlock;
    object->inputLength = remainderLength;
    object->aadLength   = 0U;

    HSMLPF3_constructGCMToken(object, saveIV, loadIV); /* only loadIV is true */

    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->common.returnBehavior,
                                    AESGCMLPF3HSM_processFinalBlockPostProcessing,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->common.returnStatus;
        }
    }

    return status;
}

/*
 *  ======== AESGCMLPF3HSM_postProcessingFxn ========
 */
static inline void AESGCMLPF3HSM_postProcessingFxn(uintptr_t arg0)
{
    AESGCM_Handle handle         = (AESGCM_Handle)arg0;
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status          = AESGCM_STATUS_ERROR;
    int8_t tokenResult           = HSMLPF3_getResultCode() & HSMLPF3_RETVAL_MASK;
    bool finalizeOp              = true;

    /* The HSM IP will throw an error when operation->macLength is zero despite it producing a correct
     * ciphertext/plaintext for both encrypt/decrypt operations and will compute a mac anyways.
     */
    if ((tokenResult == EIP130TOKEN_RESULT_SUCCESS) || ((tokenResult == EIP130TOKEN_RESULT_VERIFY_ERROR) &&
                                                        ((object->operationType == AESGCM_OP_TYPE_ONESTEP_DECRYPT) ||
                                                         (object->operationType == AESGCM_OP_TYPE_DATA_DECRYPT) ||
                                                         (object->operationType == AESGCM_OP_TYPE_FINALIZE_DECRYPT))))
    {
        object->totalAADLengthRemaining -= object->aadLength;
        object->totalDataLengthRemaining -= object->inputLength;

        status = AESGCM_STATUS_SUCCESS;
    }

    if (status == AESGCM_STATUS_SUCCESS)
    {
        if ((object->operationType == AESGCM_OP_TYPE_ONESTEP_ENCRYPT) ||
            (object->operationType == AESGCM_OP_TYPE_ONESTEP_DECRYPT))
        {
            AESGCM_OneStepOperation *operation = (AESGCM_OneStepOperation *)object->operation;

            if (object->totalDataLengthRemaining > 0U)
            {
                status = AESGCMLPF3HSM_processFinalBlock(handle);

                if (status == AESGCM_STATUS_SUCCESS)
                {
                    finalizeOp = false;
                }
            }
            else if (operation->inputLength < AES_BLOCK_SIZE)
            {
                (void)memcpy(operation->output, object->output, operation->inputLength);
            }
        }
        else if ((object->operationType == AESGCM_OP_TYPE_FINALIZE_ENCRYPT) ||
                 (object->operationType == AESGCM_OP_TYPE_FINALIZE_DECRYPT))
        {
            AESGCM_SegmentedFinalizeOperation *operation = (AESGCM_SegmentedFinalizeOperation *)object->operation;

            if (object->totalDataLengthRemaining > 0U)
            {
                status = AESGCMLPF3HSM_processFinalBlock(handle);

                if (status == AESGCM_STATUS_SUCCESS)
                {
                    finalizeOp = false;
                }
            }
            else if (operation->inputLength < AES_BLOCK_SIZE)
            {
                (void)memcpy(operation->output, object->output, operation->inputLength);
            }
        }
        else if ((object->operationType == AESGCM_OP_TYPE_DATA_ENCRYPT) ||
                 (object->operationType == AESGCM_OP_TYPE_DATA_DECRYPT))
        {
            AESGCM_SegmentedDataOperation *operation = (AESGCM_SegmentedDataOperation *)object->operation;

            if ((!HSM_IS_SIZE_MULTIPLE_OF_AES_BLOCK_SIZE(operation->inputLength)) &&
                (operation->inputLength > AES_BLOCK_SIZE))
            {
                status = AESGCMLPF3HSM_processFinalBlock(handle);

                if (status == AESGCM_STATUS_SUCCESS)
                {
                    finalizeOp = false;
                }
            }
            else if (operation->inputLength < AES_BLOCK_SIZE)
            {
                (void)memcpy(operation->output, object->output, operation->inputLength);
            }
        }
    }

    if (finalizeOp)
    {
        AESGCMLPF3HSM_postProcessingCommon(handle, status, tokenResult);
    }
}

#if (ENABLE_KEY_STORAGE == 1)
/*
 *  ======== AESGCMLPF3HSM_getKeyMaterial ========
 */
static int_fast16_t AESGCMLPF3HSM_getKeyMaterial(AESGCMLPF3HSM_Object *object)
{
    int_fast16_t status = AESGCM_STATUS_SUCCESS;

    if (object->common.key.encoding == CryptoKey_KEYSTORE_HSM)
    {
        KeyStore_PSA_KeyFileId keyID;
        KeyStore_PSA_KeyAttributes attributes = KEYSTORE_PSA_KEY_ATTRIBUTES_INIT;
        KeyStore_PSA_KeyUsage usage           = KEYSTORE_PSA_KEY_USAGE_ENCRYPT;
        KeyStore_PSA_KeyLifetime lifetime;
        int_fast16_t keyStoreStatus;

        if ((object->operationType == AESGCM_OP_TYPE_ONESTEP_DECRYPT) ||
            (object->operationType == AESGCM_OP_TYPE_AAD_DECRYPT) ||
            (object->operationType == AESGCM_OP_TYPE_DATA_DECRYPT) ||
            (object->operationType == AESGCM_OP_TYPE_DATA_DECRYPT))
        {
            usage = KEYSTORE_PSA_KEY_USAGE_DECRYPT;
        }

        GET_KEY_ID(keyID, object->common.key.u.keyStore.keyID);

        keyStoreStatus = KeyStore_PSA_getKeyAttributes(keyID, &attributes);

        if (keyStoreStatus == KEYSTORE_PSA_STATUS_SUCCESS)
        {
            keyStoreStatus = KeyStore_PSA_retrieveFromKeyStore(&object->common.key,
                                                               &object->KeyStore_keyingMaterial[0],
                                                               AESCommonLPF3_256_KEY_LENGTH_BYTES,
                                                               &object->keyAssetID,
                                                               KEYSTORE_PSA_ALG_CCM,
                                                               usage);

            if (keyStoreStatus == KEYSTORE_PSA_STATUS_SUCCESS)
            {
                lifetime = KeyStore_PSA_getKeyLifetime(&attributes);

                object->keyLocation = KEYSTORE_PSA_KEY_LIFETIME_GET_LOCATION(lifetime);
            }
        }

        if (keyStoreStatus == KEYSTORE_PSA_STATUS_INVALID_KEY_ID)
        {
            status = AESGCM_STATUS_KEYSTORE_INVALID_ID;
        }
        else if (keyStoreStatus != KEYSTORE_PSA_STATUS_SUCCESS)
        {
            status = AESGCM_STATUS_KEYSTORE_GENERIC_ERROR;
        }
    }

    return status;
}
#endif

/*
 *  ======== AESGCMLPF3HSM_setupObjectMetaData ========
 */
static void AESGCMLPF3HSM_setupObjectMetaData(AESGCMLPF3HSM_Object *object)
{
    bool saveIV = false;
    bool loadIV = false;

    if ((object->operationType == AESGCM_OP_TYPE_ONESTEP_ENCRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_ONESTEP_DECRYPT))
    {
        AESGCM_OneStepOperation *operation = (AESGCM_OneStepOperation *)object->operation;

        if (operation->inputLength < AES_BLOCK_SIZE)
        {
            (void)memset(object->inputFinalBlock, 0, AES_BLOCK_SIZE);
            (void)memcpy(object->inputFinalBlock, operation->input, operation->inputLength);

            (void)memset(object->outputFinalBlock, 0, AES_BLOCK_SIZE);

            object->input       = object->inputFinalBlock;
            object->output      = object->outputFinalBlock;
            object->inputLength = operation->inputLength;
        }
        else if (HSM_IS_SIZE_MULTIPLE_OF_AES_BLOCK_SIZE(operation->inputLength))
        {
            /* Get block-size aligned input length */
            object->input       = operation->input;
            object->output      = operation->output;
            object->inputLength = operation->inputLength;
        }
        else
        {
            /* Get block-size aligned input length */
            object->input       = operation->input;
            object->output      = operation->output;
            object->inputLength = operation->inputLength & AES_BLOCK_SIZE_MULTIPLE_MASK;

            saveIV = true;
        }
    }
    else if ((object->operationType == AESGCM_OP_TYPE_AAD_ENCRYPT) ||
             (object->operationType == AESGCM_OP_TYPE_AAD_DECRYPT))
    {
        AESGCM_SegmentedAADOperation *operation = (AESGCM_SegmentedAADOperation *)object->operation;

        object->input       = NULL;
        object->output      = NULL;
        object->inputLength = 0U;

        if (HSM_IS_SIZE_MULTIPLE_OF_AES_BLOCK_SIZE(operation->aadLength))
        {
            /* Get block-size aligned input length */
            object->aad       = operation->aad;
            object->aadLength = operation->aadLength;

            if (operation->aadLength == object->totalAADLengthRemaining)
            {
                object->aadLength -= AES_BLOCK_SIZE;
                object->bufferedAADLength = AES_BLOCK_SIZE;

                (void)memset(object->aadFinalBlock, 0, AES_BLOCK_SIZE);
                (void)memcpy(&object->aadFinalBlock[0], operation->aad + object->aadLength, object->bufferedAADLength);
            }
        }
        else
        {
            /* Get block-size aligned input length */
            object->aad               = operation->aad;
            object->aadLength         = (operation->aadLength & AES_BLOCK_SIZE_MULTIPLE_MASK);
            object->bufferedAADLength = (operation->aadLength & AES_NON_BLOCK_SIZE_MULTIPLE_MASK);

            (void)memset(object->aadFinalBlock, 0, AES_BLOCK_SIZE);
            (void)memcpy(&object->aadFinalBlock[0], operation->aad + object->aadLength, object->bufferedAADLength);
        }

        if (object->tempAssetID != 0U)
        {
            if (object->totalAADLengthRemaining != object->totalAADLength)
            {
                loadIV = true;
            }

            saveIV = true;
        }
    }
    else if ((object->operationType == AESGCM_OP_TYPE_DATA_ENCRYPT) ||
             (object->operationType == AESGCM_OP_TYPE_DATA_DECRYPT))
    {
        AESGCM_SegmentedDataOperation *operation = (AESGCM_SegmentedDataOperation *)object->operation;

        if (object->totalAADLengthRemaining > 0U)
        {
            object->aad       = &object->aadFinalBlock[0];
            object->aadLength = object->bufferedAADLength;
        }
        else
        {
            object->aad       = NULL;
            object->aadLength = 0;
        }

        if (operation->inputLength == 0U)
        {
            /* Get block-size aligned input length */
            object->input       = NULL;
            object->output      = NULL;
            object->inputLength = 0U;
        }
        else if (operation->inputLength < AES_BLOCK_SIZE)
        {
            (void)memset(object->inputFinalBlock, 0, AES_BLOCK_SIZE);
            (void)memcpy(object->inputFinalBlock, operation->input, operation->inputLength);

            (void)memset(object->outputFinalBlock, 0, AES_BLOCK_SIZE);

            object->input       = object->inputFinalBlock;
            object->output      = object->outputFinalBlock;
            object->inputLength = operation->inputLength;
        }
        else if (HSM_IS_SIZE_MULTIPLE_OF_AES_BLOCK_SIZE(operation->inputLength))
        {
            /* Get block-size aligned input length */
            object->input       = operation->input;
            object->output      = operation->output;
            object->inputLength = operation->inputLength;
        }
        else
        {
            /* Get block-size aligned input length */
            object->input       = operation->input;
            object->output      = operation->output;
            object->inputLength = operation->inputLength & AES_BLOCK_SIZE_MULTIPLE_MASK;
        }

        if (object->tempAssetID != 0U)
        {
            if ((object->totalAADLength != object->totalAADLengthRemaining) ||
                (object->totalDataLength != object->totalDataLengthRemaining))
            {
                loadIV = true;
            }

            if (object->totalDataLengthRemaining != object->inputLength)
            {
                saveIV = true;
            }
        }
    }
    else
    {
        AESGCM_SegmentedFinalizeOperation *operation = (AESGCM_SegmentedFinalizeOperation *)object->operation;

        if (object->totalAADLengthRemaining > 0U)
        {
            object->aad       = &object->aadFinalBlock[0];
            object->aadLength = object->bufferedAADLength;
        }
        else
        {
            object->aad       = NULL;
            object->aadLength = 0;
        }

        if (operation->inputLength == 0U)
        {
            /* Get block-size aligned input length */
            object->input       = NULL;
            object->output      = NULL;
            object->inputLength = 0U;
        }
        else if (operation->inputLength < AES_BLOCK_SIZE)
        {
            (void)memset(object->inputFinalBlock, 0, AES_BLOCK_SIZE);
            (void)memcpy(object->inputFinalBlock, operation->input, operation->inputLength);

            (void)memset(object->outputFinalBlock, 0, AES_BLOCK_SIZE);

            object->input       = object->inputFinalBlock;
            object->output      = object->outputFinalBlock;
            object->inputLength = operation->inputLength;
        }
        else if (HSM_IS_SIZE_MULTIPLE_OF_AES_BLOCK_SIZE(operation->inputLength))
        {
            /* Get block-size aligned input length */
            object->input       = operation->input;
            object->output      = operation->output;
            object->inputLength = operation->inputLength;
        }
        else
        {
            /* Get block-size aligned input length */
            object->input       = operation->input;
            object->output      = operation->output;
            object->inputLength = operation->inputLength & AES_BLOCK_SIZE_MULTIPLE_MASK;
        }

        if (object->tempAssetID != 0U)
        {
            if ((object->totalAADLength != object->totalAADLengthRemaining) ||
                (object->totalDataLength != object->totalDataLengthRemaining))
            {
                loadIV = true;
            }

            if (object->totalDataLengthRemaining != object->inputLength)
            {
                saveIV = true;
            }
        }
    }

    HSMLPF3_constructGCMToken(object, saveIV, loadIV);
}

/*
 *  ======== AESGCMLPF3HSM_performHSMOperation ========
 */
static int_fast16_t AESGCMLPF3HSM_performHSMOperation(AESGCM_Handle handle)
{
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status          = AESGCM_STATUS_ERROR;
    int_fast16_t hsmRetval       = HSMLPF3_STATUS_ERROR;

    AESGCMLPF3HSM_setupObjectMetaData(object);

    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->common.returnBehavior,
                                    AESGCMLPF3HSM_postProcessingFxn,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->common.returnStatus;
        }
    }

    if (hsmRetval != HSMLPF3_STATUS_SUCCESS)
    {
        /* In the case of failure to initialize the operation, we need to free all assets allocated.
         * Capturing the return status of this operation is pointless since we are returning an
         * error code anyways.
         */
        (void)AESGCMLPF3HSM_freeTempAssetID(handle);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== AESGCM_cancelOperation ========
 */
int_fast16_t AESGCM_cancelOperation(AESGCM_Handle handle)
{
    DebugP_assert(handle);

    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status          = AESGCM_STATUS_ERROR;

    /* Cancellation is only supported in callback mode */
    if (object->common.returnBehavior != AES_RETURN_BEHAVIOR_CALLBACK)
    {
        return status;
    }

    uintptr_t interruptKey = HwiP_disable();

    /* Return success if there is no active operation to cancel.
     * Do not execute the callback as it would have been executed already
     * when the operation completed.
     */
    if (((object->common.key.encoding & CRYPTOKEY_HSM) == 0) && (!object->common.operationInProgress))
    {
        HwiP_restore(interruptKey);
    }
    else
    {
        HwiP_restore(interruptKey);

        /* Since the HSM cannot cancel an in-progress token, we must wait for the result to allow for
         * subsequent token submissions to succeed.
         */
        (void)HSMLPF3_cancelOperation();

        object->segmentedOperationInProgress = false;

        /* Free all assets. This includes key-related asset and state-related asset (temporary asset). */
        status = AESGCMLPF3HSM_freeAllAssets(handle);

        object->aad       = NULL;
        object->input     = NULL;
        object->output    = NULL;
        object->iv        = NULL;
        object->mac       = NULL;
        object->operation = NULL;

        object->inputLength              = 0U;
        object->totalDataLengthRemaining = 0U;
        object->totalAADLength           = 0U;
        object->totalDataLength          = 0U;

        object->macLength = 0U;
        object->ivLength  = 0U;

        /* Operation pointer could be NULL if a segmented operation was setup
         * but neither AESGCM_addData or AESGCM_finalize was called.
         */
        if ((object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK) && (object->operation != NULL))
        {
            /* Call the callback function provided by the application */
            object->callbackFxn(handle, AESGCM_STATUS_CANCELED, object->operation, object->operationType);
        }
    }

    return status;
}

/*
 *  ======== AESGCMLPF3HSM_setupSegmentedOperation ========
 */
static int_fast16_t AESGCMLPF3HSM_setupSegmentedOperation(AESGCM_Handle handle,
                                                          AESGCM_OperationType operationType,
                                                          const CryptoKey *key,
                                                          size_t totalAADLength,
                                                          size_t totalDataLength)
{
    DebugP_assert(key);

    int_fast16_t status          = AESGCM_STATUS_SUCCESS;
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);

    /* If the HSM IP or HSMSAL failed to boot, then we cannot perform any HSM-related operation */
    if (object->hsmStatus != HSMLPF3_STATUS_SUCCESS)
    {
        return AESGCM_STATUS_ERROR;
    }

    /* A segmented operation may have been started but not finalized yet */
    if (object->segmentedOperationInProgress)
    {
        return AESGCM_STATUS_ERROR;
    }

    /* Make internal copy of crypto key */
    object->common.key = *key;

    /* returnStatus is only changed in the case of an error or cancellation */
    object->common.returnStatus = AES_STATUS_SUCCESS;

    /* If the user doesn't provide the total lengths in the setupXXXX()
     * calls, they must provide the lengths in setLengths().
     */
    object->totalAADLength  = totalAADLength;
    object->totalDataLength = totalDataLength;

    object->totalDataLengthRemaining = totalDataLength;
    object->totalAADLengthRemaining  = totalAADLength;
    object->inputLength              = 0U;
    object->aadLength                = 0U;

    object->operationType = operationType;

    /* We are not supplied the macLength at this point of the segmented operation.
     * Therefore, the driver gives it a dummy length which is the max length until
     * the user supplies the desired macLength in _setLengths() API.
     */
    object->macLength = HSM_MAC_MAX_LENGTH;

    (void)memset(object->KeyStore_keyingMaterial, 0, AESCommonLPF3_256_KEY_LENGTH_BYTES);
    (void)memset(object->inputFinalBlock, 0, AES_BLOCK_SIZE);
    (void)memset(object->outputFinalBlock, 0, AES_BLOCK_SIZE);
    (void)memset(object->aadFinalBlock, 0, AES_BLOCK_SIZE);

    /* Initialize MAC pointer to NULL to avoid premature processing of the
     * MAC in the ISR.
     */
    object->mac = NULL;

    /* Initialize operation pointer to NULL in case AESGCM_cancelOperation
     * is called after AESGCM_setupXXXX and callback should be skipped.
     */
    object->operation = NULL;

    object->segmentedOperationInProgress = true;

#if (ENABLE_KEY_STORAGE == 1)
    status = AESGCMLPF3HSM_getKeyMaterial(object);
#endif

    if (((totalAADLength > AES_BLOCK_SIZE) || (totalDataLength > AES_BLOCK_SIZE)) && (object->tempAssetID == 0U))
    {
        if (!HSMLPF3_acquireLock(object->common.semaphoreTimeout, (uintptr_t)handle))
        {
            return AESGCM_STATUS_RESOURCE_UNAVAILABLE;
        }

        status = AESGCMLPF3HSM_createTempAssetID(handle);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== AESGCM_setupEncrypt ========
 */
int_fast16_t AESGCM_setupEncrypt(AESGCM_Handle handle,
                                 const CryptoKey *key,
                                 size_t totalAADLength,
                                 size_t totalPlaintextLength)
{
    return AESGCMLPF3HSM_setupSegmentedOperation(handle,
                                                 AESGCM_OPERATION_TYPE_ENCRYPT,
                                                 key,
                                                 totalAADLength,
                                                 totalPlaintextLength);
}

/*
 *  ======== AESGCM_setupDecrypt ========
 */
int_fast16_t AESGCM_setupDecrypt(AESGCM_Handle handle,
                                 const CryptoKey *key,
                                 size_t totalAADLength,
                                 size_t totalPlaintextLength)
{
    return AESGCMLPF3HSM_setupSegmentedOperation(handle,
                                                 AESGCM_OPERATION_TYPE_DECRYPT,
                                                 key,
                                                 totalAADLength,
                                                 totalPlaintextLength);
}

/*
 *  ======== AESGCM_setLengths ========
 */
int_fast16_t AESGCM_setLengths(AESGCM_Handle handle, size_t aadLength, size_t plaintextLength)
{
    DebugP_assert(handle);

    int_fast16_t status          = AESGCM_STATUS_SUCCESS;
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);

    /* This shouldn't be called after addXXX() or finalizeXXX() */
    DebugP_assert(object->operationType == AESGCM_OPERATION_TYPE_DECRYPT ||
                  object->operationType == AESGCM_OPERATION_TYPE_ENCRYPT);

    /* Don't continue the segmented operation if there
     * was an error or a cancellation
     */
    if (object->common.returnStatus != AESGCM_STATUS_SUCCESS)
    {
        return object->common.returnStatus;
    }

    /* The combined length of AAD and payload data must be non-zero. */
    if ((aadLength == 0U) && (plaintextLength == 0U))
    {
        return AESGCM_STATUS_ERROR;
    }

    object->totalAADLength           = aadLength;
    object->totalDataLength          = plaintextLength;
    object->totalAADLengthRemaining  = aadLength;
    object->totalDataLengthRemaining = plaintextLength;

    if (((aadLength > AES_BLOCK_SIZE) || (plaintextLength > AES_BLOCK_SIZE)) && (object->tempAssetID == 0U))
    {
        if (!HSMLPF3_acquireLock(object->common.semaphoreTimeout, (uintptr_t)handle))
        {
            return AESGCM_STATUS_RESOURCE_UNAVAILABLE;
        }

        status = AESGCMLPF3HSM_createTempAssetID(handle);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== AESGCMLPF3HSM_setMac ========
 */
int_fast16_t AESGCMLPF3HSM_setMac(AESGCM_Handle handle, const uint8_t *mac, size_t macLength)
{
    DebugP_assert(handle);
    DebugP_assert(nonce);

    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);

    /* This function cannot be called after addXXX() or finalizeXXX() */
    DebugP_assert((object->operationType == AESGCM_OPERATION_TYPE_DECRYPT) ||
                  (object->operationType == AESGCM_OPERATION_TYPE_ENCRYPT));

    /* Don't continue the segmented operation if there
     * was an error during setup.
     */
    if (object->common.returnStatus != AESGCM_STATUS_SUCCESS)
    {
        return object->common.returnStatus;
    }

    object->mac       = (uint8_t *)mac;
    object->macLength = (uint8_t)macLength;

    if ((object->operationType == AESGCM_OPERATION_TYPE_ENCRYPT) && (macLength == 0U))
    {
        /* We are not supplied the macLength at this point of the segmented operation.
         * Therefore, the driver gives it a dummy length which is the max length until
         * the user supplies the desired macLength in _setLengths() API.
         */
        object->macLength = HSM_MAC_MAX_LENGTH;
    }

    return AESGCM_STATUS_SUCCESS;
}

/*
 *  ======== AESGCM_setIV ========
 */
int_fast16_t AESGCM_setIV(AESGCM_Handle handle, const uint8_t *iv, size_t ivLength)
{
    DebugP_assert(handle);
    DebugP_assert(nonce);

    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);

    /* This function cannot be called after addXXX() or finalizeXXX() */
    DebugP_assert((object->operationType == AESGCM_OPERATION_TYPE_DECRYPT) ||
                  (object->operationType == AESGCM_OPERATION_TYPE_ENCRYPT));

    /* Don't continue the segmented operation if there
     * was an error during setup.
     */
    if (object->common.returnStatus != AESGCM_STATUS_SUCCESS)
    {
        return object->common.returnStatus;
    }

    object->iv       = iv;
    object->ivLength = (uint8_t)ivLength;

    return AESGCM_STATUS_SUCCESS;
}

/*
 *  ======== AESGCM_generateIV ========
 */
int_fast16_t AESGCM_generateIV(AESGCM_Handle handle, uint8_t *iv, size_t ivSize, size_t *ivLength)
{
    /* Segmented operations are not currently supported by this implementation */
    return AESGCM_STATUS_FEATURE_NOT_SUPPORTED;
}

/*
 *  ======== AESGCM_addAAD ========
 */
int_fast16_t AESGCM_addAAD(AESGCM_Handle handle, AESGCM_SegmentedAADOperation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESGCMLPF3HSM_Object *object       = AESGCMLPF3HSM_getObject(handle);
    object->operation                  = (AESGCM_OperationUnion *)operation;
    AESGCM_OperationType operationType = AESGCM_OP_TYPE_AAD_ENCRYPT;

    /* If the HSM IP and/or HSMSAL failed to boot then we cannot perform any HSM-related operation */
    if ((object->totalAADLength == 0U) || (object->hsmStatus != HSMLPF3_STATUS_SUCCESS))
    {
        return AESGCM_STATUS_ERROR;
    }
    else if (object->common.returnStatus != AESGCM_STATUS_SUCCESS)
    {
        /* Don't continue the segmented operation if there
         * was an error or a cancellation.
         */
        return object->common.returnStatus;
    }

    /* This operation can be called after setup or after addAAD again. */
    DebugP_assert((object->operationType == AESGCM_OPERATION_TYPE_DECRYPT) ||
                  (object->operationType == AESGCM_OPERATION_TYPE_ENCRYPT) ||
                  (object->operationType == AESGCM_OP_TYPE_AAD_DECRYPT) ||
                  (object->operationType == AESGCM_OP_TYPE_AAD_ENCRYPT));

    if ((object->operationType == AESGCM_OPERATION_TYPE_DECRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_AAD_DECRYPT))
    {
        operationType = AESGCM_OP_TYPE_AAD_DECRYPT;
    }

    object->operationType = operationType;
    object->operation     = (AESGCM_OperationUnion *)operation;

    if ((operation->aadLength <= AES_BLOCK_SIZE) && (operation->aadLength == object->totalAADLengthRemaining))
    {
        (void)memcpy(&object->aadFinalBlock[0], operation->aad, operation->aadLength);
        object->bufferedAADLength = operation->aadLength;

        object->common.returnStatus = AESGCM_STATUS_SUCCESS;

        if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK)
        {
            object->callbackFxn(handle, object->common.returnStatus, object->operation, object->operationType);
        }

        return AESGCM_STATUS_SUCCESS;
    }

    if (!HSMLPF3_acquireLock(object->common.semaphoreTimeout, (uintptr_t)handle))
    {
        return AESGCM_STATUS_RESOURCE_UNAVAILABLE;
    }

    return AESGCMLPF3HSM_performHSMOperation(handle);
}

/*
 *  ======== AESGCM_addData ========
 */
int_fast16_t AESGCM_addData(AESGCM_Handle handle, AESGCM_SegmentedDataOperation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);

    /* This operation can be called after setupXXXX, addAAD, or addData */
    DebugP_assert((object->operationType == AESGCM_OP_TYPE_AAD_ENCRYPT) ||
                  (object->operationType == AESGCM_OP_TYPE_DATA_ENCRYPT));

    if (object->common.returnStatus != AESGCM_STATUS_SUCCESS)
    {
        return object->common.returnStatus;
    }

    /* Check word-alignment of input & output pointers */
    if (!IS_WORD_ALIGNED(operation->output))
    {
        return AESGCM_STATUS_UNALIGNED_IO_NOT_SUPPORTED;
    }

    /* The input length must be a non-zero multiple of an AES block size
     * unless you are dealing with the last chunk of payload data
     */
    if ((operation->inputLength == 0U) || ((!HSM_IS_SIZE_MULTIPLE_OF_WORD(operation->inputLength)) &&
                                           (operation->inputLength != object->totalDataLengthRemaining)))
    {
        return AESGCM_STATUS_ERROR;
    }

    /* The total input length must not exceed the lengths specified in
     * AESGCM_setLengths() or setupXXXX().
     */
    if (operation->inputLength > object->totalDataLengthRemaining)
    {
        return AESGCM_STATUS_ERROR;
    }

    AESGCM_OperationType operationType = AESGCM_OP_TYPE_DATA_ENCRYPT;

    if ((object->operationType == AESGCM_OPERATION_TYPE_DECRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_AAD_DECRYPT) || (object->operationType == AESGCM_OP_TYPE_DATA_DECRYPT))
    {
        operationType = AESGCM_OP_TYPE_DATA_DECRYPT;
    }

    return AESGCMLPF3HSM_addData(handle, operationType, (AESGCM_OperationUnion *)operation);
}

/*
 *  ======== AESGCMLPF3HSM_addData ========
 */
static int_fast16_t AESGCMLPF3HSM_addData(AESGCM_Handle handle,
                                          AESGCM_OperationType operationType,
                                          AESGCM_OperationUnion *operation)
{
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);

    /* If the HSM IP and/or HSMSAL failed to boot then we cannot perform any HSM-related operation */
    if (object->hsmStatus != HSMLPF3_STATUS_SUCCESS)
    {
        return AESGCM_STATUS_ERROR;
    }

    object->operationType = operationType;
    object->operation     = operation;

    if (!HSMLPF3_acquireLock(object->common.semaphoreTimeout, (uintptr_t)handle))
    {
        return AESGCM_STATUS_RESOURCE_UNAVAILABLE;
    }

    return AESGCMLPF3HSM_performHSMOperation(handle);
}

/*
 *  ======== AESGCMLPF3HSM_performFinalizeChecks ========
 */
static int_fast16_t AESGCMLPF3HSM_performFinalizeChecks(const AESGCMLPF3HSM_Object *object,
                                                        const AESGCM_SegmentedFinalizeOperation *operation)
{
    /* This operation can be called after setupXXXX, addAAD, or addData */
    DebugP_assert((object->operationType == AESGCM_OP_TYPE_AAD_ENCRYPT) ||
                  (object->operationType == AESGCM_OP_TYPE_DATA_ENCRYPT));

    /* Don't continue the segmented operation if there
     * was an error or a cancellation.
     */
    if (object->common.returnStatus != AESGCM_STATUS_SUCCESS)
    {
        return object->common.returnStatus;
    }

    /* If the HSM IP and/or HSMSAL failed to boot then we cannot perform any HSM-related operation */
    if (object->hsmStatus != HSMLPF3_STATUS_SUCCESS)
    {
        return AESGCM_STATUS_ERROR;
    }

    /* Additional payload data can be passed in finalize */
    if (operation->inputLength != object->totalDataLengthRemaining)
    {
        return AESGCM_STATUS_ERROR;
    }

    return AESGCM_STATUS_SUCCESS;
}

/*
 *  ======== AESGCMLPF3HSM_finalizeCommon ========
 */
static int_fast16_t AESGCMLPF3HSM_finalizeCommon(AESGCM_Handle handle,
                                                 AESGCM_OperationType operationType,
                                                 AESGCM_SegmentedFinalizeOperation *operation)
{
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status          = AESGCM_STATUS_ERROR;

    status = AESGCMLPF3HSM_performFinalizeChecks(object, operation);

    if (status != AESGCM_STATUS_SUCCESS)
    {
        return status;
    }

    if (object->totalDataLengthRemaining == object->totalDataLength)
    {
        object->input  = operation->input;
        object->output = operation->output;
    }

    object->mac       = operation->mac;
    object->macLength = operation->macLength;

    if ((operation->inputLength > 0U) || (object->totalAADLengthRemaining > 0U))
    {
        status = AESGCMLPF3HSM_addData(handle, operationType, (AESGCM_OperationUnion *)operation);
    }
    else
    {
        object->operationType = operationType;

        if (operationType == AESGCM_OP_TYPE_FINALIZE_ENCRYPT)
        {
            (void)memcpy(operation->mac, (void *)&object->intermediateTag[0], operation->macLength);
        }

        if (!HSMLPF3_acquireLock(object->common.semaphoreTimeout, (uintptr_t)handle))
        {
            return AESGCM_STATUS_RESOURCE_UNAVAILABLE;
        }

        object->aadLength   = 0U;
        object->inputLength = 0U;

        AESGCMLPF3HSM_postProcessingCommon(handle, status, (int8_t)AESGCM_STATUS_SUCCESS);
    }

    return status;
}

/*
 *  ======== AESGCM_finalizeEncrypt ========
 */
int_fast16_t AESGCM_finalizeEncrypt(AESGCM_Handle handle, AESGCM_SegmentedFinalizeOperation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    return AESGCMLPF3HSM_finalizeCommon(handle, AESGCM_OP_TYPE_FINALIZE_ENCRYPT, operation);
}

/*
 *  ======== AESGCM_finalizeDecrypt ========
 */
int_fast16_t AESGCM_finalizeDecrypt(AESGCM_Handle handle, AESGCM_SegmentedFinalizeOperation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    return AESGCMLPF3HSM_finalizeCommon(handle, AESGCM_OP_TYPE_FINALIZE_DECRYPT, operation);
}

/*
 *  ======== AESGCMLPF3HSM_CreateAssetPostProcessing ========
 */
static inline void AESGCMLPF3HSM_CreateAssetPostProcessing(uintptr_t arg0)
{
    AESGCM_Handle handle         = (AESGCM_Handle)arg0;
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status          = AESGCM_STATUS_ERROR;
    int32_t tokenResult          = HSMLPF3_getResultCode();

    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->tempAssetID = HSMLPF3_getResultAssetID();
        status              = AESGCM_STATUS_SUCCESS;
    }

    object->common.returnStatus = status;
}

/*
 *  ======== AESGCMLPF3HSM_createTempAssetID ========
 */
static int_fast16_t AESGCMLPF3HSM_createTempAssetID(AESGCM_Handle handle)
{
    int_fast16_t status          = AESGCM_STATUS_ERROR;
    int_fast16_t hsmRetval       = HSMLPF3_STATUS_ERROR;
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    uint64_t assetPolicy = HSM_ASSET_POLICY_SYM_AES_AUTH | HSM_ASSET_POLICY_TEMPORARY | HSM_ASSET_POLICY_SYM_MODE_GCM;

    if (object->operationType == AESGCM_OPERATION_TYPE_ENCRYPT)
    {
        assetPolicy |= HSM_ASSET_POLICY_DIR_ENC_GEN;
    }
    else
    {
        assetPolicy |= HSM_ASSET_POLICY_DIR_DEC_VRFY;
    }

    HSMLPF3_constructCreateAssetToken(assetPolicy, HSM_AEAD_TEMP_ASSET_SIZE);

    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    AESGCMLPF3HSM_CreateAssetPostProcessing,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->common.returnStatus;
        }
    }

    if (hsmRetval != HSMLPF3_STATUS_SUCCESS)
    {
        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== AESGCMLPF3HSM_freeAllAssets ========
 */
static int_fast16_t AESGCMLPF3HSM_freeAllAssets(AESGCM_Handle handle)
{
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status          = AESGCM_STATUS_SUCCESS;
#if (ENABLE_KEY_STORAGE == 1)
    KeyStore_PSA_KeyFileId keyID;
#endif

    if ((object->operationType == AESGCM_OP_TYPE_AAD_ENCRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_AAD_DECRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_DATA_ENCRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_DATA_DECRYPT))
    {
        /* Operation is not done yet and therefore, do not free any asset. */
        return AESGCM_STATUS_SUCCESS;
    }

#if (ENABLE_KEY_STORAGE == 1)
    if ((object->common.key.encoding == CryptoKey_KEYSTORE_HSM) &&
        (object->keyLocation == KEYSTORE_PSA_KEY_LOCATION_HSM_ASSET_STORE))
    {
        GET_KEY_ID(keyID, object->common.key.u.keyStore.keyID);

        /* For keys with a persistence that does not designate them to remain in asset store,
         * the following function will remove them. Otherwise, the key will remain in asset store
         * for future usage.
         */
        if (status == AESGCM_STATUS_SUCCESS)
        {
            status = KeyStore_PSA_assetPostProcessing(keyID);

            if (status != KEYSTORE_PSA_STATUS_SUCCESS)
            {
                status = AESGCM_STATUS_ERROR;
            }
            else
            {
                status = AESGCM_STATUS_SUCCESS;
            }
        }
        else
        {
            (void)KeyStore_PSA_assetPostProcessing(keyID);
        }
    }
#endif

    if (status != AESGCM_STATUS_SUCCESS)
    {
        (void)AESGCMLPF3HSM_freeTempAssetID(handle);
    }
    else
    {
        status = AESGCMLPF3HSM_freeTempAssetID(handle);
    }

    return status;
}

/*
 *  ======== AESGCMLPF3HSM_FreeTempAssetIDPostProcessing ========
 */
static inline void AESGCMLPF3HSM_FreeTempAssetIDPostProcessing(uintptr_t arg0)
{
    AESGCM_Handle handle         = (AESGCM_Handle)arg0;
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status          = AESGCM_STATUS_ERROR;
    int32_t tokenResult          = HSMLPF3_getResultCode();

    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->tempAssetID = 0U;
        status              = AESGCM_STATUS_SUCCESS;
    }

    object->common.returnStatus = status;
}

/*
 *  ======== AESGCMLPF3HSM_freeTempAssetID ========
 */
static int_fast16_t AESGCMLPF3HSM_freeTempAssetID(AESGCM_Handle handle)
{
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status          = AESGCM_STATUS_SUCCESS;
    int_fast16_t hsmRetval       = HSMLPF3_STATUS_ERROR;

    if (object->tempAssetID != 0)
    {
        HSMLPF3_constructDeleteAssetToken(object->tempAssetID);

        hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                        AESGCMLPF3HSM_FreeTempAssetIDPostProcessing,
                                        (uintptr_t)handle);

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            hsmRetval = HSMLPF3_waitForResult();

            if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
            {
                status = object->common.returnStatus;
            }
        }
    }
    return status;
}
