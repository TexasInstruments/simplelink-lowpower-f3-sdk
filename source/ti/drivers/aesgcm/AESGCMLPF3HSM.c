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
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_asset_policy.h>

#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/power/PowerCC27XX.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/aes.h)
#include DeviceFamily_constructPath(inc/hw_aes.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)

#define KEY_TEMP_ASSET_SIZE 48U

#define AES_NON_BLOCK_MULTIPLE_MASK 0x0FU

#define AESGCMLPF3_DMA_SIZE_THRESHOLD (1U * AES_BLOCK_SIZE)

/* Forward declarations */
static int_fast16_t AESGCMLPF3HSM_oneStepOperation(AESGCM_Handle handle,
                                                   AESGCM_OneStepOperation *operation,
                                                   AESGCM_OperationType operationType);
static int_fast16_t AESGCMLPF3HSM_processOneStepOperation(AESGCM_Handle handle);

static int_fast16_t AESGCMLPF3HSM_createTempAssetID(AESGCM_Handle handle, AESGCM_Mode direction);
static int_fast16_t AESGCMLPF3HSM_freeTempAssetID(AESGCM_Handle handle);

static int_fast16_t AESGCMLPF3HSM_processSegmentedData(AESGCM_Handle handle,
                                                       AESGCM_OperationType operationType,
                                                       AESGCM_OperationUnion *operation,
                                                       const uint8_t *input,
                                                       uint8_t *output,
                                                       size_t inputLength);

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
        /* Upon HSM Boot failure, the AES-CCM Driver stores the failure status in the object
         * This is done so that users of the AES-CCM Driver do not get a NULL handle and still can use
         * the driver in LAES mode.
         */
        object->hsmStatus = HSMLPF3_STATUS_ERROR;
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
    object->aadLength       = object->totalAADLength;
    object->totalAADLength  = operation->aadLength;
    object->ivLength        = operation->ivLength;

    object->totalDataLengthRemaining = object->totalDataLength;
    object->totalAADLengthRemaining  = object->totalAADLength;

    object->tempAssetID = 0U;

    /* Clear the counter value */
    (void)memset(object->counter, 0U, sizeof(object->counter));

    return AESGCMLPF3HSM_processOneStepOperation(handle);
}

/*
 *  ======== AESGCMLPF3HSM_OneStepPostProcessing ========
 */
static inline void AESGCMLPF3HSM_OneStepPostProcessing(uintptr_t arg0)
{
    AESGCM_Handle handle                      = (AESGCM_Handle)arg0;
    AESGCMLPF3HSM_Object *object              = AESGCMLPF3HSM_getObject(handle);
    AESGCM_OneStepOperation *oneStepOperation = (AESGCM_OneStepOperation *)object->operation;
    int_fast16_t status                       = AESGCM_STATUS_ERROR;
    int32_t physicalResult                    = HSMLPF3_getResultCode();
    int32_t tokenResult                       = physicalResult & HSMLPF3_RETVAL_MASK;

    /* The HSM IP will throw an error when operation->macLength is zero despite it producing a correct
     * ciphertext/plaintext for both encrypt/decrypt operations and will compute a mac anyways.
     */
    if ((tokenResult == EIP130TOKEN_RESULT_INVALID_LENGTH) && (oneStepOperation->macLength == 0U))
    {
        tokenResult = EIP130TOKEN_RESULT_SUCCESS;
    }

    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        if (object->operationType == AESGCM_OP_TYPE_ONESTEP_ENCRYPT)
        {
            HSMLPF3_getAESEncryptTag((void *)&oneStepOperation->mac[0]);
        }

        status = AESGCM_STATUS_SUCCESS;
    }
    else if ((object->operationType == AESGCM_OP_TYPE_ONESTEP_DECRYPT) ||
             (tokenResult == EIP130TOKEN_RESULT_VERIFY_ERROR))
    {
        status = AESGCM_STATUS_MAC_INVALID;
    }

    object->common.returnStatus = status;

    HSMLPF3_releaseLock();

    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

    if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK)
    {
        object->callbackFxn(handle, object->common.returnStatus, object->operation, object->operationType);
    }
}

static int_fast16_t AESGCMLPF3HSM_processOneStepOperation(AESGCM_Handle handle)
{
    int_fast16_t status          = AESGCM_STATUS_ERROR;
    int_fast16_t hsmRetval       = HSMLPF3_STATUS_ERROR;
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);

    if (!HSMLPF3_acquireLock(SemaphoreP_NO_WAIT, (uintptr_t)handle))
    {
        return AESGCM_STATUS_RESOURCE_UNAVAILABLE;
    }

    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    HSMLPF3_constructAESGCMOneStepPhysicalToken(object);

    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->common.returnBehavior,
                                    AESGCMLPF3HSM_OneStepPostProcessing,
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
        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

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

    /* Cancellation is only supported in callback mode */
    if (object->common.returnBehavior != AES_RETURN_BEHAVIOR_CALLBACK)
    {
        return AESGCM_STATUS_ERROR;
    }

    uintptr_t interruptKey = HwiP_disable();

    /* Return success if there is no active operation to cancel.
     * Do not execute the callback as it would have been executed already
     * when the operation completed.
     */
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    if (((object->common.key.encoding & CRYPTOKEY_HSM) == 0) && (!object->common.operationInProgress))
#else
    if (!object->common.operationInProgress)
#endif
    {
        HwiP_restore(interruptKey);
    }
    else
    {
        HwiP_restore(interruptKey);

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
        /* Since the HSM cannot cancel an in-progress token, we must wait for the result to allow for
         * subsequent token submissions to succeed.
         */
        (void)HSMLPF3_cancelOperation();

        object->segmentedOperationInProgress = false;

        int_fast16_t status = AESGCMLPF3HSM_freeTempAssetID(handle);
        if (status != AESGCM_STATUS_SUCCESS)
        {
            return AESGCM_STATUS_ERROR;
        }
#endif

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

    return AESGCM_STATUS_SUCCESS;
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

    HSMLPF3_releaseLock();

    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*
 *  ======== AESGCMLPF3HSM_createTempAssetID ========
 */
static int_fast16_t AESGCMLPF3HSM_createTempAssetID(AESGCM_Handle handle, AESGCM_Mode direction)
{
    int_fast16_t status          = AESGCM_STATUS_ERROR;
    int_fast16_t hsmRetval       = HSMLPF3_STATUS_ERROR;
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    uint64_t assetPolicy         = 0U;

    if (!HSMLPF3_acquireLock(SemaphoreP_NO_WAIT, (uintptr_t)handle))
    {
        return AESGCM_STATUS_RESOURCE_UNAVAILABLE;
    }

    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    assetPolicy = EIP130_ASSET_POLICY_SYM_TEMP | EIP130_ASSET_POLICY_SCUICIPHERAUTH | EIP130_ASSET_POLICY_SCACAES |
                  EIP130_ASSET_POLICY_SCMCAGCM;
    assetPolicy |= direction == AESGCM_MODE_DECRYPT ? EIP130_ASSET_POLICY_SCDIRDECVRFY
                                                    : EIP130_ASSET_POLICY_SCDIRENCGEN;

    HSMLPF3_constructCreateAssetToken(assetPolicy, KEY_TEMP_ASSET_SIZE);

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
        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== AESGCMLPF3HSM_setupSegmentedOperation ========
 */
static int_fast16_t AESGCMLPF3HSM_setupSegmentedOperation(AESGCMLPF3HSM_Object *object,
                                                          const CryptoKey *key,
                                                          size_t totalAADLength,
                                                          size_t totalDataLength)
{
    DebugP_assert(key);

    /* If the HSM IP or HSMSAL failed to boot then we cannot perform any HSM-related operation */
    if (object->hsmStatus != HSMLPF3_STATUS_SUCCESS || key->encoding != CryptoKey_PLAINTEXT_HSM)
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

    /* Initialize MAC pointer to NULL to avoid premature processing of the
     * MAC in the ISR.
     */
    object->mac = NULL;

    /* Initialize operation pointer to NULL in case AESGCM_cancelOperation
     * is called after AESGCM_setupXXXX and callback should be skipped.
     */
    object->operation = NULL;

    object->segmentedOperationInProgress = true;

    return AESGCM_STATUS_SUCCESS;
}

/*
 *  ======== AESGCM_setupEncrypt ========
 */
int_fast16_t AESGCM_setupEncrypt(AESGCM_Handle handle,
                                 const CryptoKey *key,
                                 size_t totalAADLength,
                                 size_t totalPlaintextLength)
{
    DebugP_assert(handle);
    int_fast16_t status          = AESGCM_STATUS_ERROR;
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);

    status = AESGCMLPF3HSM_setupSegmentedOperation(object, key, totalAADLength, totalPlaintextLength);
    if (status == AES_STATUS_SUCCESS)
    {
        object->operationType = AESGCM_OPERATION_TYPE_ENCRYPT;
        status                = AESGCMLPF3HSM_createTempAssetID(handle, AESGCM_MODE_ENCRYPT);
    }

    return status;
}

/*
 *  ======== AESGCM_setupDecrypt ========
 */
int_fast16_t AESGCM_setupDecrypt(AESGCM_Handle handle,
                                 const CryptoKey *key,
                                 size_t totalAADLength,
                                 size_t totalPlaintextLength)
{
    DebugP_assert(handle);
    int_fast16_t status          = AESGCM_STATUS_ERROR;
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);

    status = AESGCMLPF3HSM_setupSegmentedOperation(object, key, totalAADLength, totalPlaintextLength);
    if (status == AES_STATUS_SUCCESS)
    {
        object->operationType = AESGCM_OPERATION_TYPE_DECRYPT;
        status                = AESGCMLPF3HSM_createTempAssetID(handle, AESGCM_MODE_DECRYPT);
    }

    return status;
}

/*
 *  ======== AESGCM_setLengths ========
 */
int_fast16_t AESGCM_setLengths(AESGCM_Handle handle, size_t aadLength, size_t plaintextLength)
{
    DebugP_assert(handle);

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
    object->totalDataLengthRemaining = plaintextLength;

    return AESGCM_STATUS_SUCCESS;
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
 *  ======== AESGCMLPF3HSM_SegmentedPostProcessing ========
 */
static inline void AESGCMLPF3HSM_SegmentedPostProcessing(uintptr_t arg0)
{
    AESGCM_Handle handle         = (AESGCM_Handle)arg0;
    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status          = AESGCM_STATUS_ERROR;
    int32_t physicalResult       = HSMLPF3_getResultCode();
    int32_t tokenResult          = physicalResult & HSMLPF3_RETVAL_MASK;

    /* The HSM IP will throw an error when operation->macLength is zero despite it producing a correct
     * ciphertext/plaintext for both encrypt/decrypt operations and will compute a mac anyways.
     */
    if ((tokenResult == EIP130TOKEN_RESULT_INVALID_LENGTH) && (object->macLength == 0U))
    {
        tokenResult = EIP130TOKEN_RESULT_SUCCESS;
    }

    if (object->operationType == AESGCM_OP_TYPE_AAD_ENCRYPT || object->operationType == AESGCM_OP_TYPE_AAD_DECRYPT)
    {
        if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
        {
            object->totalAADLengthRemaining -= object->aadLength;
            object->aadLength = 0U;

            status = AESGCM_STATUS_SUCCESS;
        }
    }
    else
    {
        object->totalAADLengthRemaining -= object->aadLength;
        object->totalDataLengthRemaining -= object->inputLength;

        object->aadLength   = 0U;
        object->inputLength = 0U;

        if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
        {
            status = AESGCM_STATUS_SUCCESS;

            if ((object->operationType == AESGCM_OP_TYPE_DATA_ENCRYPT) ||
                (object->operationType == AESGCM_OP_TYPE_FINALIZE_ENCRYPT))
            {
                HSMLPF3_getAESEncryptTag((uint8_t *)&object->intermediateTag[0]);
            }
        }
        else if ((object->operationType == AESGCM_OP_TYPE_FINALIZE_DECRYPT) &&
                 (tokenResult == EIP130TOKEN_RESULT_VERIFY_ERROR))
        {
            status = AESGCM_STATUS_MAC_INVALID;
        }

        if ((object->operationType == AESGCM_OP_TYPE_FINALIZE_ENCRYPT) && object->totalDataLengthRemaining == 0U)
        {
            (void)memcpy((void *)&object->mac[0], (void *)&object->intermediateTag[0], object->macLength);
        }
    }

    HSMLPF3_releaseLock();

    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

    if ((object->operationType == AESGCM_OP_TYPE_FINALIZE_ENCRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_FINALIZE_DECRYPT))
    {
        status = AESGCMLPF3HSM_freeTempAssetID(handle);

        object->segmentedOperationInProgress = false;
    }

    object->common.returnStatus = status;

    if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK)
    {
        object->callbackFxn(handle, status, object->operation, object->operationType);
    }
}

/*
 *  ======== AESGCM_addAAD ========
 */
int_fast16_t AESGCM_addAAD(AESGCM_Handle handle, AESGCM_SegmentedAADOperation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status          = AESGCM_STATUS_SUCCESS;
    int_fast16_t hsmRetval       = HSMLPF3_STATUS_ERROR;

    object->operation = (AESGCM_OperationUnion *)operation;

    if (object->totalAADLengthRemaining == object->totalAADLength)
    {
        object->aad = operation->aad;
    }

    /* Don't continue the segmented operation if there
     * was an error or a cancellation.
     */
    if (object->common.returnStatus != AESGCM_STATUS_SUCCESS)
    {
        return object->common.returnStatus;
    }

    /* This operation can be called after setup or after addAAD again. */
    DebugP_assert((object->operationType == AESGCM_OPERATION_TYPE_DECRYPT) ||
                  (object->operationType == AESGCM_OPERATION_TYPE_ENCRYPT) ||
                  (object->operationType == AESGCM_OP_TYPE_AAD_DECRYPT) ||
                  (object->operationType == AESGCM_OP_TYPE_AAD_ENCRYPT));

    size_t aadBytesProcessed = object->totalAADLength - object->totalAADLengthRemaining;
    size_t calcAADLen        = aadBytesProcessed + operation->aadLength;

    /* The total AAD input length must not exceed the total length specified
     * in AESGCM_setLengths() or the setupXXXX() call.
     */
    if (calcAADLen > object->totalAADLength)
    {
        return AESGCM_STATUS_ERROR;
    }

    AESGCM_OperationType operationType = AESGCM_OP_TYPE_AAD_ENCRYPT;

    if ((object->operationType == AESGCM_OPERATION_TYPE_DECRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_AAD_DECRYPT))
    {
        operationType = AESGCM_OP_TYPE_AAD_DECRYPT;
    }

    object->operationType = operationType;

    uint8_t aadBytesToProcess = operation->aadLength;
    uint8_t inputRemainder    = aadBytesToProcess & AES_NON_BLOCK_MULTIPLE_MASK;

    if (inputRemainder > 0)
    {
        aadBytesToProcess -= inputRemainder;
    }

    /*
     * The HSM HW requires that we leave at least one block size of AAD left over
     * to be processed with the first batch of input data.
     * So whatevever is left is being processed with the first call to _addData().
     */
    if (aadBytesProcessed + aadBytesToProcess == object->totalAADLength)
    {
        aadBytesToProcess -= AES_BLOCK_SIZE;
    }

    object->aadLength   = aadBytesToProcess;
    object->inputLength = 0U;

    if (aadBytesToProcess > 0)
    {
        if (!HSMLPF3_acquireLock(SemaphoreP_NO_WAIT, (uintptr_t)handle))
        {
            return AESGCM_STATUS_RESOURCE_UNAVAILABLE;
        }

        Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

        HSMLPF3_constructAESGCMSegmentedAADPhysicalToken(object);

        hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->common.returnBehavior,
                                        AESGCMLPF3HSM_SegmentedPostProcessing,
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
            Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

            HSMLPF3_releaseLock();
        }
    }
    else
    {
        if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK)
        {
            object->callbackFxn(handle, status, (AESGCM_OperationUnion *)operation, operationType);

            /* Always return success in callback mode */
            status = AESGCM_STATUS_SUCCESS;
        }
    }

    object->common.returnStatus = status;

    return status;
}

/*
 *  ======== AESGCM_addData ========
 */
int_fast16_t AESGCM_addData(AESGCM_Handle handle, AESGCM_SegmentedDataOperation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status          = AESGCM_STATUS_ERROR;

    /* This operation can be called after setupXXXX, addAAD, or addData */
    DebugP_assert((object->operationType == AESGCM_OP_TYPE_AAD_ENCRYPT) ||
                  (object->operationType == AESGCM_OP_TYPE_DATA_ENCRYPT));

    if (object->common.returnStatus != AESGCM_STATUS_SUCCESS)
    {
        return object->common.returnStatus;
    }

#if (DeviceFamily_PARENT != DeviceFamily_PARENT_CC27XX) && (AESCommonLPF3_UNALIGNED_IO_SUPPORT_ENABLE == 0)
    /* Check word-alignment of input & output pointers */
    if (!IS_WORD_ALIGNED(operation->input) || !IS_WORD_ALIGNED(operation->output))
    {
        return AESGCM_STATUS_UNALIGNED_IO_NOT_SUPPORTED;
    }
#endif

    /* The input length must be a non-zero multiple of an AES block size
     * unless you are dealing with the last chunk of payload data
     */
    if ((operation->inputLength == 0U) || ((AES_NON_BLOCK_SIZE_MULTIPLE_LENGTH(operation->inputLength) > 0U) &&
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

    if (object->totalDataLengthRemaining == object->totalDataLength)
    {
        object->input  = operation->input;
        object->output = operation->output;
    }

    AESGCM_OperationType operationType = AESGCM_OP_TYPE_DATA_ENCRYPT;

    if ((object->operationType == AESGCM_OPERATION_TYPE_DECRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_AAD_DECRYPT) || (object->operationType == AESGCM_OP_TYPE_DATA_DECRYPT))
    {
        operationType = AESGCM_OP_TYPE_DATA_DECRYPT;
    }

    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        status = AESGCMLPF3HSM_processSegmentedData(handle,
                                                    operationType,
                                                    (AESGCM_OperationUnion *)operation,
                                                    operation->input,
                                                    operation->output,
                                                    operation->inputLength);
        return status;
    }
    else
    {
        status = AESGCM_STATUS_ERROR;
    }

    if ((object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK) &&
        (operation->inputLength < AESGCMLPF3_DMA_SIZE_THRESHOLD))
    {
        object->callbackFxn(handle, status, (AESGCM_OperationUnion *)operation, operationType);

        /* Always return success in callback mode */
        status = AESGCM_STATUS_SUCCESS;
    }

    return status;
}

/*
 *  ======== AESGCMLPF3HSM_processSegmentedData ========
 */
int_fast16_t AESGCMLPF3HSM_processSegmentedData(AESGCM_Handle handle,
                                                AESGCM_OperationType operationType,
                                                AESGCM_OperationUnion *operation,
                                                const uint8_t *input,
                                                uint8_t *output,
                                                size_t inputLength)
{
    int_fast16_t status    = AESGCM_STATUS_ERROR;
    int_fast16_t hsmRetval = HSMLPF3_STATUS_ERROR;

    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);

    if (!HSMLPF3_acquireLock(SemaphoreP_NO_WAIT, (uintptr_t)handle))
    {
        return AESGCM_STATUS_RESOURCE_UNAVAILABLE;
    }

    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    object->operationType = operationType;
    object->operation     = operation;

    object->aadLength   = object->totalAADLengthRemaining;
    object->inputLength = inputLength;

    HSMLPF3_constructAESGCMSegmentedDataPhysicalToken(object);

    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->common.returnBehavior,
                                    AESGCMLPF3HSM_SegmentedPostProcessing,
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
        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

        HSMLPF3_releaseLock();
    }

    object->common.returnStatus = status;

    return status;
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
 *  ======== AESGCM_finalizeEncrypt ========
 */
int_fast16_t AESGCM_finalizeEncrypt(AESGCM_Handle handle, AESGCM_SegmentedFinalizeOperation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status;

    status = AESGCMLPF3HSM_performFinalizeChecks(object, operation);

    if (status != AESGCM_STATUS_SUCCESS)
    {
        return status;
    }

    /*
     * When calling _setupEncrypt(), we are not provided with input and output pointers.
     * These two pointers get set in _addAAD() and _addData() APIs.
     * This case accounts when we call this API immediately after _setupEncrypt().
     */
    if (object->totalDataLengthRemaining == object->totalDataLength)
    {
        object->input  = operation->input;
        object->output = operation->output;
    }

    object->mac       = operation->mac;
    object->macLength = operation->macLength;

    if ((operation->inputLength > 0U) || (object->totalAADLengthRemaining > 0))
    {
        status = AESGCMLPF3HSM_processSegmentedData(handle,
                                                    AESGCM_OP_TYPE_FINALIZE_ENCRYPT,
                                                    (AESGCM_OperationUnion *)operation,
                                                    operation->input,
                                                    operation->output,
                                                    operation->inputLength);
    }
    else
    {
        (void)memcpy(operation->mac, (uint32_t *)&object->intermediateTag[0], operation->macLength);

        status = AESGCMLPF3HSM_freeTempAssetID(handle);

        object->segmentedOperationInProgress = false;

        if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK)
        {
            object->callbackFxn(handle, status, (AESGCM_OperationUnion *)operation, AESGCM_OP_TYPE_FINALIZE_ENCRYPT);
        }
    }
    return status;
}

/*
 *  ======== AESGCM_finalizeDecrypt ========
 */
int_fast16_t AESGCM_finalizeDecrypt(AESGCM_Handle handle, AESGCM_SegmentedFinalizeOperation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESGCMLPF3HSM_Object *object = AESGCMLPF3HSM_getObject(handle);
    int_fast16_t status;

    status = AESGCMLPF3HSM_performFinalizeChecks(object, operation);

    if (status != AESGCM_STATUS_SUCCESS)
    {
        return status;
    }

    /*
     * When calling _setupDecrypt(), we are not provided with input and output pointers.
     * These two pointers get set in _addAAD() and _addData() APIs.
     * This case accounts when we call this API immediately after _setupDecrypt().
     */
    if (object->totalDataLengthRemaining == object->totalDataLength)
    {
        object->input  = operation->input;
        object->output = operation->output;
    }

    object->mac       = operation->mac;
    object->macLength = operation->macLength;

    if ((operation->inputLength > 0U) || (object->totalAADLengthRemaining > 0))
    {
        status = AESGCMLPF3HSM_processSegmentedData(handle,
                                                    AESGCM_OP_TYPE_FINALIZE_DECRYPT,
                                                    (AESGCM_OperationUnion *)operation,
                                                    operation->input,
                                                    operation->output,
                                                    operation->inputLength);
    }
    else
    {
        status = AESGCMLPF3HSM_freeTempAssetID(handle);

        object->segmentedOperationInProgress = false;

        if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK)
        {
            object->callbackFxn(handle, status, (AESGCM_OperationUnion *)operation, AESGCM_OP_TYPE_FINALIZE_DECRYPT);
        }
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

    if ((HSMLPF3_ReturnBehavior)object->common.returnBehavior == HSMLPF3_RETURN_BEHAVIOR_POLLING)
    {
        HSMLPF3_releaseLock();

        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
    }
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
        if ((HSMLPF3_ReturnBehavior)object->common.returnBehavior == HSMLPF3_RETURN_BEHAVIOR_POLLING)
        {
            if (!HSMLPF3_acquireLock(SemaphoreP_NO_WAIT, (uintptr_t)handle))
            {
                return AESGCM_STATUS_RESOURCE_UNAVAILABLE;
            }

            Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);
        }

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

        if (((HSMLPF3_ReturnBehavior)object->common.returnBehavior == HSMLPF3_RETURN_BEHAVIOR_POLLING) &&
            (hsmRetval != HSMLPF3_STATUS_SUCCESS))
        {
            Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

            HSMLPF3_releaseLock();
        }
    }
    return status;
}