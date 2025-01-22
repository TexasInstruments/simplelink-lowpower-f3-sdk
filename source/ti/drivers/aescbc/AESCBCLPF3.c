/*
 * Copyright (c) 2021-2024, Texas Instruments Incorporated
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
#include <stdint.h>
#include <string.h>

#include <ti/drivers/aescbc/AESCBCLPF3.h>
#include <ti/drivers/AESCBC.h>
#include <ti/drivers/AESCommon.h>
#include <ti/drivers/aesecb/AESECBLPF3.h>
#include <ti/drivers/cryptoutils/aes/AESCommonLPF3.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKey.h>
#include <ti/drivers/cryptoutils/sharedresources/CryptoResourceLPF3.h>
#include <ti/drivers/cryptoutils/utils/CryptoUtils.h>
#include <ti/drivers/dma/UDMALPF3.h>

#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC23XX) && (ENABLE_KEY_STORAGE == 1))
    #error "Key storage is not supported for CC23XX"
#endif

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    #include <ti/drivers/cryptoutils/hsm/HSMLPF3.h>
    #include <third_party/hsmddk/include/Integration/Adapter_DriverInit/incl/api_driver_init.h>
    #include <third_party/hsmddk/include/Integration/Adapter_VEX/incl/adapter_vex.h>
    #include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_exchangetoken.h>
    #include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_asset.h>
    #include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_result.h>
    #include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_asset.h>
    #if (ENABLE_KEY_STORAGE == 1)
        #include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA.h>
        #include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA_helpers.h>
    #endif
#endif

#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/aes.h)
#include DeviceFamily_constructPath(inc/hw_aes.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)

/*
 * Default AES CBC auto config:
 *  CBC SRC as TXTXBUF
 *  Trigger points for auto CBC as RDTXT3 and WRBUF3S
 *   (the first encryption starts by writing BUF3, the successive ones by reading TXT3)
 *  DONEACT as GATE_TRGAES_ON_CHA_DEL (to avoid spurious last ECB using DMA)
 *  BUSHALT enabled
 */
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC23X0) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    #define AESCBCLPF3_DEFAULT_AUTOCFG                                                 \
        ((uint32_t)AES_AUTOCFG_AESSRC_TXTXBUF | (uint32_t)AES_AUTOCFG_TRGAES_WRBUF3S | \
         (uint32_t)AES_AUTOCFG_TRGAES_RDTXT3 | (uint32_t)AES_AUTOCFG_BUSHALT_EN)
#else
    #error "Unsupported DeviceFamily_Parent for AESCBCLPF3!"
#endif

/*
 * AES CBC auto config for a single block encryption:
 *  CBC SRC as TXTXBUF
 *  Trigger points for auto CBC as WRBUF3S (encryption starts by writing BUF3)
 *  BUSHALT enabled
 */
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC23X0) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    #define AESCBCLPF3_SINGLE_BLOCK_AUTOCFG \
        ((uint32_t)AES_AUTOCFG_AESSRC_TXTXBUF | (uint32_t)AES_AUTOCFG_TRGAES_WRBUF3S | (uint32_t)AES_AUTOCFG_BUSHALT_EN)
#else
    #error "Unsupported DeviceFamily_Parent for AESCBCLPF3!"
#endif

/* Forward declarations */
static int_fast16_t AESCBCLPF3_checkOperation(AESCBCLPF3_Object *object,
                                              uint8_t *input,
                                              uint8_t *output,
                                              size_t inputLength);
static void AESCBCLPF3_finishOperation(AESCBCLPF3_Object *object);
static inline int_fast16_t AESCBCLPF3_oneStepOperation(AESCBC_Handle handle,
                                                       AESCBC_Operation *operation,
                                                       AESCBC_OperationType operationType);
static int_fast16_t AESCBCLPF3_startOperation(AESCBC_Handle handle,
                                              const uint8_t *input,
                                              uint8_t *output,
                                              size_t inputLength);
static inline int_fast16_t AESCBCLPF3_waitForResult(AESCBC_Handle handle);

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
static int_fast16_t AESCBCLPF3HSM_oneStepOperation(AESCBC_Handle handle,
                                                   AESCBC_OneStepOperation *operation,
                                                   AESCBC_OperationType operationType);
static int_fast16_t AESCBCLPF3HSM_processData(AESCBC_Handle handle);
int_fast16_t AESCBCLPF3HSM_addData(AESCBC_Handle handle, AESCBC_SegmentedOperation *operation);
int_fast16_t AESCBCLPF3HSM_finalize(AESCBC_Handle handle, AESCBC_SegmentedOperation *operation);
#endif

/*
 *  ======== AESCBCLPF3_getObject ========
 */
static inline AESCBCLPF3_Object *AESCBCLPF3_getObject(AESCBC_Handle handle)
{
    AESCBCLPF3_Object *object = (AESCBCLPF3_Object *)handle->object;
    DebugP_assert(object);

    return object;
}

/*
 *  ======== AESCBCLPF3_hwiFxn ========
 */
static void AESCBCLPF3_hwiFxn(uintptr_t arg0)
{
    AESCBC_Handle handle      = (AESCBC_Handle)arg0;
    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);

    /*
     * Only the output channel B interrupt is enabled.
     */
    uint32_t intStatus = AESGetMaskedInterruptStatus();

    /* Disable DMA, clear interupts, and release power constraint */
    AESCommonLPF3_cleanupHwi(&object->common);

    if ((intStatus & (uint32_t)AES_MIS_CHBDONE_M) != (uint32_t)0U)
    {
        UDMALPF3_clearInterrupt(AESCommonLPF3_DMA_CHB_BITMASK);

        AESCBCLPF3_finishOperation(object);

        /* Cleanup and release crypto resource lock */
        AESCommonLPF3_cleanup(&object->common);

        if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_BLOCKING)
        {
            /* Unblock the pending task to signal that the operation is complete */
            SemaphoreP_post(&CryptoResourceLPF3_operationSemaphore);
        }
        else
        {
            /* Call the callback function provided by the application */
            object->callbackFxn((AESCBC_Handle)handle,
                                object->common.returnStatus,
                                object->operation,
                                object->operationType);
        }
    }
}

/*
 *  ======== AESCBC_init ========
 */
void AESCBC_init(void)
{
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    HSMLPF3_constructRTOSObjects();
#endif
    AESCommonLPF3_init();
}

/*
 *  ======== AESCBC_construct ========
 */
AESCBC_Handle AESCBC_construct(AESCBC_Config *config, const AESCBC_Params *params)
{
    DebugP_assert(config);

    int_fast16_t status;
    AESCBC_Handle handle      = config;
    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
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

    object->segmentedOperationInProgress = false;
#endif

    /* If params are NULL, use defaults */
    if (params == NULL)
    {
        params = &AESCBC_defaultParams;
    }

    DebugP_assert((params->returnBehavior != AESCBC_RETURN_BEHAVIOR_CALLBACK) || (params->callbackFxn != NULL));

    object->callbackFxn = params->callbackFxn;
    object->threadSafe  = true;

    status = AESCommonLPF3_construct(&object->common, (AES_ReturnBehavior)params->returnBehavior, params->timeout);

    if (status != AES_STATUS_SUCCESS)
    {
        handle = NULL;
    }

    return handle;
}

/*
 *  ======== AESCBC_close ========
 */
void AESCBC_close(AESCBC_Handle handle)
{
    DebugP_assert(handle);

    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);

    AESCommonLPF3_close(&object->common);
}

/*
 *  ======== AESCBC_setIV ========
 */
int_fast16_t AESCBC_setIV(AESCBC_Handle handle, const uint8_t *iv, size_t ivLength)
{
    DebugP_assert(handle);
    DebugP_assert(iv);

    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);

    /* Don't continue the operation if there was an error or cancellation */
    if (object->common.returnStatus != AESCBC_STATUS_SUCCESS)
    {
        return object->common.returnStatus;
    }

    /* This function is reserved for segmented operations only */
    DebugP_assert((object->operationType == AESCBC_OP_TYPE_DECRYPT_SEGMENTED) ||
                  (object->operationType == AESCBC_OP_TYPE_ENCRYPT_SEGMENTED));

    /* The length of the new IV should be 16 or AES_BLOCK_SIZE bytes */
    if (ivLength != AES_BLOCK_SIZE)
    {
        return AESCBC_STATUS_ERROR;
    }

    /*
     * Copy the user-provided IV to the object structure since the IV in
     * operation struct is reserved for one-shot operations only
     */
    (void)memcpy((void *)&object->iv[0], (void *)iv, sizeof(object->iv));

    return AESCBC_STATUS_SUCCESS;
}

/*
 *  ======== AESCBC_oneStepEncrypt ========
 */
int_fast16_t AESCBC_oneStepEncrypt(AESCBC_Handle handle, AESCBC_Operation *operation)
{
    int_fast16_t status = AESCBC_STATUS_SUCCESS;

    if ((operation->key->encoding == CryptoKey_PLAINTEXT) || (operation->key->encoding == CryptoKey_KEYSTORE))
    {
        status = AESCBCLPF3_oneStepOperation(handle, operation, AESCBC_OPERATION_TYPE_ENCRYPT);
    }
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    else if ((operation->key->encoding == CryptoKey_PLAINTEXT_HSM) ||
             (operation->key->encoding == CryptoKey_KEYSTORE_HSM))
    {
        status = AESCBCLPF3HSM_oneStepOperation(handle, operation, AESCBC_OPERATION_TYPE_ENCRYPT);
    }
#endif
    else
    {
        status = AESCBC_STATUS_ERROR;
    }
    return status;
}

/*
 *  ======== AESCBC_oneStepDecrypt ========
 */
int_fast16_t AESCBC_oneStepDecrypt(AESCBC_Handle handle, AESCBC_Operation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);
    int_fast16_t status = AESCBC_STATUS_SUCCESS;

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    if ((operation->key->encoding == CryptoKey_PLAINTEXT_HSM) || (operation->key->encoding == CryptoKey_KEYSTORE_HSM))
    {
        status = AESCBCLPF3HSM_oneStepOperation(handle, operation, AESCBC_OPERATION_TYPE_DECRYPT);
    }
    else
#endif
    {
        status = AESCBC_STATUS_FEATURE_NOT_SUPPORTED;
    }
    return status;
}

/*
 *  ======== AESCBCLPF3_checkOperation ========
 */
static int_fast16_t AESCBCLPF3_checkOperation(AESCBCLPF3_Object *object,
                                              uint8_t *input,
                                              uint8_t *output,
                                              size_t inputLength)
{
#if (AESCommonLPF3_UNALIGNED_IO_SUPPORT_ENABLE == 0)
    /* Check word-alignment of input & output pointers */
    if (!IS_WORD_ALIGNED(input) || !IS_WORD_ALIGNED(output))
    {
        return AESCBC_STATUS_UNALIGNED_IO_NOT_SUPPORTED;
    }
#endif

    /* Verify input length is a non-zero multiple of the AES block size */
    if ((inputLength == 0UL) || (AES_NON_BLOCK_SIZE_MULTIPLE_LENGTH(inputLength) > 0UL))
    {
        return AESCBC_STATUS_ERROR;
    }

    /* Check DMA xfer limit for blocking and callback modes */
    if ((object->common.returnBehavior != AES_RETURN_BEHAVIOR_POLLING) &&
        !AESCommonLPF3_isDMALengthValid(input, output, inputLength))
    {
        return AESCBC_STATUS_ERROR;
    }

    return AESCBC_STATUS_SUCCESS;
}

/*
 *  ======== AESCBCLPF3_oneStepOperation ========
 */
static inline int_fast16_t AESCBCLPF3_oneStepOperation(AESCBC_Handle handle,
                                                       AESCBC_OneStepOperation *operation,
                                                       AESCBC_OperationType operationType)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);
    int_fast16_t status;

    status = AESCBCLPF3_checkOperation(object, operation->input, operation->output, operation->inputLength);

    if (status != AESCBC_STATUS_SUCCESS)
    {
        return status;
    }

    /*
     * Check that there are no operations already in progress.
     * If not, mark the current operation to be in progress
     */
    status = AESCommonLPF3_setOperationInProgress(&object->common);

    if (status != AESCBC_STATUS_SUCCESS)
    {
        return status;
    }

    if (object->threadSafe)
    {
        if (!CryptoResourceLPF3_acquireLock(object->common.semaphoreTimeout))
        {
            AESCommonLPF3_clearOperationInProgress(&object->common);
            return AESCBC_STATUS_RESOURCE_UNAVAILABLE;
        }

        object->common.cryptoResourceLocked = true;
    }

    object->operation     = (AESCBC_OperationUnion *)operation;
    object->operationType = operationType;
    (void)memcpy((void *)&object->iv[0], (void *)operation->iv, sizeof(object->iv));

    object->common.key          = *(operation->key);
    /* We will only change the returnStatus if there is an error or cancellation */
    object->common.returnStatus = AESCBC_STATUS_SUCCESS;

    status = AESCBCLPF3_startOperation(handle, operation->input, operation->output, operation->inputLength);

    return status;
}

/*
 *  ======== AESCCBLPF3_startOperation ========
 */
static int_fast16_t AESCBCLPF3_startOperation(AESCBC_Handle handle,
                                              const uint8_t *input,
                                              uint8_t *output,
                                              size_t inputLength)
{
    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);
    int_fast16_t status;
#if (ENABLE_KEY_STORAGE == 1)
    int_fast16_t keyStoreStatus;
    uint8_t KeyStore_keyingMaterial[AESCommonLPF3_256_KEY_LENGTH_BYTES];
    KeyStore_PSA_KeyUsage usage;
#endif

    if (object->common.key.encoding == CryptoKey_PLAINTEXT)
    {
        /* Set up the key and AES engine to begin an operation */
        if (inputLength == AES_BLOCK_SIZE)
        {
            AESCommonLPF3_setupOperation(&object->common.key, AESCBCLPF3_SINGLE_BLOCK_AUTOCFG);
        }
        else
        {
            AESCommonLPF3_setupOperation(&object->common.key, AESCBCLPF3_DEFAULT_AUTOCFG);
        }
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (object->common.key.encoding == CryptoKey_KEYSTORE)
    {
        if ((object->operationType == AESCBC_OP_TYPE_ONESTEP_ENCRYPT) ||
            (object->operationType == AESCBC_OP_TYPE_ENCRYPT_SEGMENTED) ||
            (object->operationType == AESCBC_OP_TYPE_FINALIZE_ENCRYPT_SEGMENTED))
        {
            usage = KEYSTORE_PSA_KEY_USAGE_ENCRYPT;
        }
        else
        {
            usage = KEYSTORE_PSA_KEY_USAGE_DECRYPT;
        }

        keyStoreStatus = KeyStore_PSA_retrieveFromKeyStore(&object->common.key,
                                                           &KeyStore_keyingMaterial[0],
                                                           sizeof(KeyStore_keyingMaterial),
                                                           &object->keyAssetID,
                                                           KEYSTORE_PSA_ALG_CBC_NO_PADDING,
                                                           usage);
        if (keyStoreStatus == KEYSTORE_PSA_STATUS_SUCCESS)
        {
            AESWriteKEY(KeyStore_keyingMaterial);

            if (inputLength == AES_BLOCK_SIZE)
            {
                /* Set AUTOCFG */
                AESSetAUTOCFG(AESCBCLPF3_SINGLE_BLOCK_AUTOCFG);
            }
            else
            {
                /* Set AUTOCFG */
                AESSetAUTOCFG(AESCBCLPF3_DEFAULT_AUTOCFG);
            }
        }
        else if (keyStoreStatus == KEYSTORE_PSA_STATUS_INVALID_KEY_ID)
        {
            return AESCBC_STATUS_KEYSTORE_INVALID_ID;
        }
        else
        {
            return AESCBC_STATUS_KEYSTORE_GENERIC_ERROR;
        }
    }
#endif
    else
    {
        return AESCBC_STATUS_FEATURE_NOT_SUPPORTED;
    }

    AESWriteIV((uint8_t *)&object->iv[0]);

    if ((object->common.returnBehavior != AES_RETURN_BEHAVIOR_POLLING) &&
        (inputLength >= AESECBLPF3_DMA_SIZE_THRESHOLD))
    {
        /*
         * We need to set the HWI function and priority since the same physical
         * interrupt is shared by multiple drivers and they all need to coexist.
         * Whenever a driver starts an operation, it registers its HWI callback
         * with the OS.
         */
        AESCBCLPF3_HWAttrs const *hwAttrs = handle->hwAttrs;
        AESCommonLPF3_setupHwi(AESCBCLPF3_hwiFxn, (uintptr_t)handle, hwAttrs->intPriority);
    }

    /* ECB function is used for processing the data */
    AESECBLPF3_processData(&object->common, input, output, inputLength);

    status = AESCBCLPF3_waitForResult(handle);

    return status;
}

/*
 *  ======== AESCCBLPF3_finishOperation ========
 */
static void AESCBCLPF3_finishOperation(AESCBCLPF3_Object *object)
{
    if ((object->operationType == AESCBC_OP_TYPE_ONESTEP_ENCRYPT) ||
        (object->operationType == AESCBC_OP_TYPE_FINALIZE_ENCRYPT_SEGMENTED))
    {
        /*
         * For one-shot and finalize operations, clear the IV buffer
         * to prevent data leakage
         */
        CryptoUtils_memset((void *)&object->iv[0], sizeof(object->iv), (uint8_t)0U, sizeof(object->iv));

        AESCommonLPF3_clearOperationInProgress(&object->common);
    }
    else
    {
        /*
         * Store the new iv into object for the next block should we want to
         * continue the chain of blocks in a later segmented operation.
         */
        AESReadIV32((uint32_t *)&object->iv[0]);
    }
}

/*
 *  ======== AESCBCLPF3_waitForResult ========
 */
static inline int_fast16_t AESCBCLPF3_waitForResult(AESCBC_Handle handle)
{
    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);
    int_fast16_t status;
    size_t inputLength;

    if (object->operationType == AESCBC_OP_TYPE_ONESTEP_ENCRYPT)
    {
        inputLength = object->operation->oneStepOperation.inputLength;
    }
    else
    {
        inputLength = object->operation->segmentedOperation.inputLength;
    }

    if ((object->common.returnBehavior == AES_RETURN_BEHAVIOR_POLLING) || (inputLength < AESECBLPF3_DMA_SIZE_THRESHOLD))
    {
        AESCBCLPF3_finishOperation(object);

        /*
         * Save the returnStatus prior clearing operationInProgress or
         * releasing the access semaphore in case it's overwritten.
         */
        status = object->common.returnStatus;

        AESCommonLPF3_cleanup(&object->common);

        if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK)
        {
            object->callbackFxn(handle, status, object->operation, object->operationType);

            /* Always return success in callback mode */
            status = AESCBC_STATUS_SUCCESS;
        }
    }
    else if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_BLOCKING)
    {
        /* Ignore return value since timeout is infinite */
        (void)SemaphoreP_pend((SemaphoreP_Handle)&CryptoResourceLPF3_operationSemaphore,
                              (uint32_t)SemaphoreP_WAIT_FOREVER);

        status = object->common.returnStatus;
    }
    else /* AES_RETURN_BEHAVIOR_CALLBACK */
    {
        /* Success is always returned in callback mode */
        status = AESCBC_STATUS_SUCCESS;
    }

    return status;
}

/*
 *  ======== AESCBC_setupEncrypt ========
 */
int_fast16_t AESCBC_setupEncrypt(AESCBC_Handle handle, const CryptoKey *key)
{
    DebugP_assert(handle);
    int_fast16_t status       = AESCBC_STATUS_ERROR;
    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);

    /*
     * Key material pointer and length are not checked until adding or
     * finalizing data.
     */
    if ((key->encoding == CryptoKey_PLAINTEXT) || (key->encoding == CryptoKey_KEYSTORE))
    {
        /* When using the AES driver with the LAES engine */
        status = AESCommonLPF3_setupSegmentedOperation(&object->common, key);
    }
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    else if ((key->encoding == CryptoKey_PLAINTEXT_HSM) || (key->encoding == CryptoKey_KEYSTORE_HSM))
    {
        if (object->hsmStatus != HSMLPF3_STATUS_SUCCESS)
        {
            return AESCBC_STATUS_ERROR;
        }

        /* A segmented operation may have been started but not finalized yet */
        if (object->segmentedOperationInProgress)
        {
            return AESCBC_STATUS_ERROR;
        }

        /* Make internal copy of crypto key */
        object->common.key = *key;

        /* returnStatus is only changed in the case of an error or cancellation */
        object->common.returnStatus = AES_STATUS_SUCCESS;

        status = AESCBC_STATUS_SUCCESS;

        object->segmentedOperationInProgress = true;
    }
#endif
    else
    {
        /* User Key encoding is not valid or not supported */
    }

    if (status == AES_STATUS_SUCCESS)
    {
        /* Initialize operation pointer to NULL in case AESCBC_cancelOperation
         * is called after AESCBC_setupXXXX and callback should be skipped.
         */
        object->operation = NULL;

        object->operationType = AESCBC_OP_TYPE_ENCRYPT_SEGMENTED;
    }

    return status;
}

/*
 *  ======== AESCBC_setupDecrypt ========
 */
int_fast16_t AESCBC_setupDecrypt(AESCBC_Handle handle, const CryptoKey *key)
{
    DebugP_assert(handle);
    DebugP_assert(key);

    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);
    int_fast16_t status       = AESCBC_STATUS_FEATURE_NOT_SUPPORTED;

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    if ((key->encoding == CryptoKey_PLAINTEXT_HSM) || (key->encoding == CryptoKey_KEYSTORE_HSM))
    {
        /* If the HSM IP and/or HSMSAL failed to boot then we cannot perform any HSM-related operation */
        if (object->hsmStatus != HSMLPF3_STATUS_SUCCESS)
        {
            return AESCBC_STATUS_ERROR;
        }

        /* A segmented operation may have been started but not finalized yet */
        if (object->segmentedOperationInProgress)
        {
            return AESCBC_STATUS_ERROR;
        }

        /* Make internal copy of crypto key */
        object->common.key = *key;

        /* returnStatus is only changed in the case of an error or cancellation */
        object->common.returnStatus = AES_STATUS_SUCCESS;

        /* Initialize operation pointer to NULL in case AESCBC_cancelOperation
         * is called after AESCBC_setupXXXX and callback should be skipped.
         */
        object->operation = NULL;

        object->segmentedOperationInProgress = true;

        status = AESCBC_STATUS_SUCCESS;

        object->operationType = AESCBC_OP_TYPE_DECRYPT_SEGMENTED;
    }
    else
#endif
    {
        /* empty */
    }

    /* Save the error status in case addData or finalize is called afterward */
    object->common.returnStatus = status;

    return status;
}

/*
 *  ======== AESCBC_addData ========
 */
int_fast16_t AESCBC_addData(AESCBC_Handle handle, AESCBC_SegmentedOperation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);
    int_fast16_t status;

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    if ((object->common.key.encoding == CryptoKey_PLAINTEXT_HSM) ||
        (object->common.key.encoding == CryptoKey_KEYSTORE_HSM))
    {
        return AESCBCLPF3HSM_addData(handle, operation);
    }
#endif

    /*
     * Assert the segmented operation was setup.
     * LAES128 only supports CBC encryption.
     */
    DebugP_assert(object->operationType == AESCBC_OP_TYPE_ENCRYPT_SEGMENTED);

    /* Check for previous failure or cancellation of segmented operation */
    if (object->common.returnStatus != AESCBC_STATUS_SUCCESS)
    {
        /*
         * Return the status of the previous call.
         * The callback function will not be executed.
         */
        return object->common.returnStatus;
    }

    status = AESCBCLPF3_checkOperation(object, operation->input, operation->output, operation->inputLength);

    if (status != AESCBC_STATUS_SUCCESS)
    {
        return status;
    }

    if (object->threadSafe)
    {
        if (!CryptoResourceLPF3_acquireLock(object->common.semaphoreTimeout))
        {
            return AESCBC_STATUS_RESOURCE_UNAVAILABLE;
        }

        object->common.cryptoResourceLocked = true;
    }

    object->operation = (AESCBC_OperationUnion *)operation;

    status = AESCBCLPF3_startOperation(handle, operation->input, operation->output, operation->inputLength);

    return status;
}

/*
 *  ======== AESCBC_finalize ========
 */
int_fast16_t AESCBC_finalize(AESCBC_Handle handle, AESCBC_SegmentedOperation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);
    int_fast16_t status;

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    if ((object->common.key.encoding == CryptoKey_PLAINTEXT_HSM) ||
        (object->common.key.encoding == CryptoKey_KEYSTORE_HSM))
    {
        return AESCBCLPF3HSM_finalize(handle, operation);
    }
#endif

    /*
     * Assert the segmented operation was setup.
     * LAES128 only supports CBC encryption.
     */
    DebugP_assert(object->operationType == AESCBC_OP_TYPE_ENCRYPT_SEGMENTED);

    /* Check for previous failure or cancellation of segmented operation */
    if (object->common.returnStatus != AESCBC_STATUS_SUCCESS)
    {
        /*
         * Return the status of the previous call.
         * The callback function will not be executed.
         */
        return object->common.returnStatus;
    }

    AESCBC_OperationType operationType = AESCBC_OP_TYPE_FINALIZE_ENCRYPT_SEGMENTED;

    if (operation->inputLength > 0U)
    {
#if (AESCommonLPF3_UNALIGNED_IO_SUPPORT_ENABLE == 0)
        /* Check word-alignment of input & output pointers */
        if (!IS_WORD_ALIGNED(operation->input) || !IS_WORD_ALIGNED(operation->output))
        {
            return AESCBC_STATUS_UNALIGNED_IO_NOT_SUPPORTED;
        }
#endif

        /* Verify input length is a multiple of the AES block size */
        if (AES_NON_BLOCK_SIZE_MULTIPLE_LENGTH(operation->inputLength) > 0U)
        {
            return AESCBC_STATUS_ERROR;
        }

        /* Check DMA xfer limit for blocking and callback modes */
        if ((object->common.returnBehavior != AES_RETURN_BEHAVIOR_POLLING) &&
            !AESCommonLPF3_isDMALengthValid(operation->input, operation->output, operation->inputLength))
        {
            return AESCBC_STATUS_ERROR;
        }

        /* Try and obtain access to the crypto module */
        if (object->threadSafe)
        {
            if (!CryptoResourceLPF3_acquireLock(object->common.semaphoreTimeout))
            {
                return AESCBC_STATUS_RESOURCE_UNAVAILABLE;
            }

            object->common.cryptoResourceLocked = true;
        }

        object->operationType = operationType;
        object->operation     = (AESCBC_OperationUnion *)operation;

        status = AESCBCLPF3_startOperation(handle, operation->input, operation->output, operation->inputLength);
    }
    else /* Operation was finalized without additional data to process */
    {
        status = object->common.returnStatus;

        AESCommonLPF3_clearOperationInProgress(&object->common);

        if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK)
        {
            object->callbackFxn(handle, status, (AESCBC_OperationUnion *)operation, operationType);

            /* Always return success in callback mode */
            status = AESCBC_STATUS_SUCCESS;
        }
    }

    return status;
}

/*
 *  ======== AESCBC_cancelOperation ========
 */
int_fast16_t AESCBC_cancelOperation(AESCBC_Handle handle)
{
    DebugP_assert(handle);

    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);

    /* Cancellation is only supported in callback mode */
    if (object->common.returnBehavior != AES_RETURN_BEHAVIOR_CALLBACK)
    {
        return AESCBC_STATUS_ERROR;
    }

    uintptr_t interruptKey = HwiP_disable();

    /*
     * Return success if there is no active operation to cancel.
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
        return AESCBC_STATUS_SUCCESS;
    }

    HwiP_restore(interruptKey);

    /*
     * Cancel DMA for input and output channels, clear operation in-progress,
     * and releases crypto resource if necessary.
     */
    AESCommonLPF3_cancelOperation(&object->common, true);

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    if ((object->common.key.encoding & CRYPTOKEY_HSM))
    {
        /* Since the HSM cannot cancel an in-progress token, we must wait for the result to allow for
         * subsequent token submissions to succeed.
         */
        (void)HSMLPF3_cancelOperation();

        object->segmentedOperationInProgress = false;
    }
#endif

    object->input  = NULL;
    object->output = NULL;

    /*
     * Operation pointer could be NULL if a segmented operation was setup
     * but neither AESCBC_addData or AESCBC_finalize was called.
     */
    if ((object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK) && (object->operation != NULL))
    {
        /* Call the callback function provided by the application */
        object->callbackFxn(handle, AESCBC_STATUS_CANCELED, object->operation, object->operationType);
    }

    return AESCBC_STATUS_SUCCESS;
}

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)

/*
 *  ======== AESCBCLPF3HSM_oneStepOperation ========
 */
static int_fast16_t AESCBCLPF3HSM_oneStepOperation(AESCBC_Handle handle,
                                                   AESCBC_OneStepOperation *operation,
                                                   AESCBC_OperationType operationType)
{
    DebugP_assert(handle);
    DebugP_assert(operation);
    DebugP_assert(operation->key);
    /* No need to assert operationType since we control it within the driver */

    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);

    /* If the HSM IP and/or HSMSAL failed to boot then we cannot perform any HSM-related operation */
    if (object->hsmStatus != HSMLPF3_STATUS_SUCCESS)
    {
        return AESCBC_STATUS_ERROR;
    }

    /* Verify input length is non-zero */
    if (operation->inputLength == 0U)
    {
        return AESCBC_STATUS_ERROR;
    }

    /* A segmented operation may have been started but not finalized yet */
    if (object->segmentedOperationInProgress)
    {
        return AESCBC_STATUS_ERROR;
    }

    object->operation           = (AESCBC_OperationUnion *)operation;
    object->operationType       = operationType;
    /* We will only change the returnStatus if there is an error or cancellation */
    object->common.returnStatus = AESCBC_STATUS_SUCCESS;

    /* Make internal copy of operational params */
    object->common.key  = *(operation->key);
    object->input       = operation->input;
    object->output      = operation->output;
    object->inputLength = operation->inputLength;

    /*
     * Copy the user-provided IV to the object structure since the IV in
     * operation struct is reserved for one-shot operations only
     */
    (void)memcpy((void *)&object->iv[0], (void *)operation->iv, sizeof(object->iv));

    return AESCBCLPF3HSM_processData(handle);
}

/*
 *  ======== AESCBCLPF3HSM_OneStepPostProcessing ========
 */
static inline void AESCBCLPF3HSM_OneStepPostProcessing(uintptr_t arg0)
{
    AESCBC_Handle handle      = (AESCBC_Handle)arg0;
    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);
    int_fast16_t status       = AESCBC_STATUS_ERROR;
    int32_t physicalResult    = HSMLPF3_getResultCode();
    int32_t tokenResult       = physicalResult & HSMLPF3_RETVAL_MASK;

    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = AESCBC_STATUS_SUCCESS;

        HSMLPF3_getAESIV((void *)&object->iv[0]);
    }

    object->common.returnStatus = status;

    HSMLPF3_releaseLock();

    if ((object->operationType == AESCBC_OPERATION_TYPE_ENCRYPT) ||
        (object->operationType == AESCBC_OPERATION_TYPE_DECRYPT) ||
        (object->operationType == AESCBC_OP_TYPE_FINALIZE_ENCRYPT_SEGMENTED) ||
        (object->operationType == AESCBC_OP_TYPE_FINALIZE_DECRYPT_SEGMENTED))
    {
        object->segmentedOperationInProgress = false;
    }

    if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK)
    {
        object->callbackFxn(handle, object->common.returnStatus, object->operation, object->operationType);
    }
}

/*
 *  ======== AESCBCLPF3HSM_processData ========
 */
static int_fast16_t AESCBCLPF3HSM_processData(AESCBC_Handle handle)
{
    int_fast16_t status       = AESCBC_STATUS_SUCCESS;
    int_fast16_t hsmRetval    = HSMLPF3_STATUS_ERROR;
    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);
    uint8_t *keyMaterial      = NULL;
    #if (ENABLE_KEY_STORAGE == 1)
    KeyStore_PSA_KeyFileId keyID;
    KeyStore_PSA_KeyAttributes attributes = KEYSTORE_PSA_KEY_ATTRIBUTES_INIT;
    KeyStore_PSA_KeyLifetime lifetime;
    int_fast16_t keyStoreStatus;
    uint8_t KeyStore_keyingMaterial[AESCommonLPF3_256_KEY_LENGTH_BYTES];
    KeyStore_PSA_KeyUsage usage;
    #endif

    if (!HSMLPF3_acquireLock(object->common.semaphoreTimeout, (uintptr_t)handle))
    {
        return AESCBC_STATUS_RESOURCE_UNAVAILABLE;
    }

    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        keyMaterial = object->common.key.u.plaintext.keyMaterial;
    }
    #if (ENABLE_KEY_STORAGE == 1)
    else if (object->common.key.encoding == CryptoKey_KEYSTORE_HSM)
    {
        if ((object->operationType == AESCBC_OP_TYPE_ONESTEP_ENCRYPT) ||
            (object->operationType == AESCBC_OP_TYPE_ENCRYPT_SEGMENTED) ||
            (object->operationType == AESCBC_OP_TYPE_FINALIZE_ENCRYPT_SEGMENTED))
        {
            usage = KEYSTORE_PSA_KEY_USAGE_ENCRYPT;
        }
        else
        {
            usage = KEYSTORE_PSA_KEY_USAGE_DECRYPT;
        }

        GET_KEY_ID(keyID, object->common.key.u.keyStore.keyID);

        keyStoreStatus = KeyStore_PSA_getKeyAttributes(keyID, &attributes);

        if (keyStoreStatus == KEYSTORE_PSA_STATUS_SUCCESS)
        {
            keyStoreStatus = KeyStore_PSA_retrieveFromKeyStore(&object->common.key,
                                                               &KeyStore_keyingMaterial[0],
                                                               sizeof(KeyStore_keyingMaterial),
                                                               &object->keyAssetID,
                                                               KEYSTORE_PSA_ALG_CBC_NO_PADDING,
                                                               usage);

            if (keyStoreStatus == KEYSTORE_PSA_STATUS_SUCCESS)
            {
                lifetime = KeyStore_PSA_getKeyLifetime(&attributes);

                object->keyLocation = KEYSTORE_PSA_KEY_LIFETIME_GET_LOCATION(lifetime);

                keyMaterial = &KeyStore_keyingMaterial[0];
            }
        }

        if (keyStoreStatus == KEYSTORE_PSA_STATUS_INVALID_KEY_ID)
        {
            status = AESCBC_STATUS_KEYSTORE_INVALID_ID;
        }
        else if (keyStoreStatus != KEYSTORE_PSA_STATUS_SUCCESS)
        {
            status = AESCBC_STATUS_KEYSTORE_GENERIC_ERROR;
        }

        if (status != AESCBC_STATUS_SUCCESS)
        {
            HSMLPF3_releaseLock();

            return status;
        }
    }
    #endif
    else
    {
        HSMLPF3_releaseLock();

        return AESCBC_STATUS_FEATURE_NOT_SUPPORTED;
    }

    /* keyMaterial will point to either KeyStore_keyingMaterial, or plaintext key material from
     * the application's CryptoKey object. It is possible that keyMaterial points to
     * KeyStore_keyingMaterial as an empty array - in this case, the object's keyLocation
     * will reflect that, so HSMLPF3 will retrieve the loaded HSM Asset ID instead.
     */
    HSMLPF3_constructAESCBCOneStepPhysicalToken(object, keyMaterial);

    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->common.returnBehavior,
                                    AESCBCLPF3HSM_OneStepPostProcessing,
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
 *  ======== AESCBCLPF3HSM_addData ========
 */
int_fast16_t AESCBCLPF3HSM_addData(AESCBC_Handle handle, AESCBC_SegmentedOperation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);

    /* If the HSM IP and/or HSMSAL failed to boot then we cannot perform any HSM-related operation */
    if (object->hsmStatus != HSMLPF3_STATUS_SUCCESS)
    {
        return AESCBC_STATUS_ERROR;
    }

    /*
     * Assert the segmented operation was setup.
     * LAES128 only supports CBC encryption.
     */
    DebugP_assert(object->operationType == AESCBC_OP_TYPE_ENCRYPT_SEGMENTED);

    /* Check for previous failure or cancellation of segmented operation */
    if (object->common.returnStatus != AESCBC_STATUS_SUCCESS)
    {
        /*
         * Return the status of the previous call.
         * The callback function will not be executed.
         */
        return object->common.returnStatus;
    }

    object->operation = (AESCBC_OperationUnion *)operation;

    /* Make internal copy of operational params */
    object->input       = operation->input;
    object->output      = operation->output;
    object->inputLength = operation->inputLength;

    return AESCBCLPF3HSM_processData(handle);
}

/*
 *  ======== AESCBCLPF3HSM_finalize ========
 */
int_fast16_t AESCBCLPF3HSM_finalize(AESCBC_Handle handle, AESCBC_SegmentedOperation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESCBCLPF3_Object *object = AESCBCLPF3_getObject(handle);
    int_fast16_t status;
    AESCBC_OperationType operationType;

    /* Assert the segmented operation was setup */
    DebugP_assert((object->operationType == AESCBC_OPERATION_TYPE_ENCRYPT_SEGMENTED) ||
                  (object->operationType == AESCBC_OPERATION_TYPE_DECRYPT_SEGMENTED));

    /* Check for previous failure or cancellation of segmented operation */
    if (object->common.returnStatus != AESCBC_STATUS_SUCCESS)
    {
        /*
         * Return the status of the previous call.
         * The callback function will not be executed.
         */
        return object->common.returnStatus;
    }

    if (object->operationType == AESCBC_OP_TYPE_ENCRYPT_SEGMENTED)
    {
        operationType = AESCBC_OP_TYPE_FINALIZE_ENCRYPT_SEGMENTED;
    }
    else
    {
        operationType = AESCBC_OP_TYPE_FINALIZE_DECRYPT_SEGMENTED;
    }

    if (operation->inputLength > 0U)
    {
        object->operationType = operationType;
        object->operation     = (AESCBC_OperationUnion *)operation;

        /* Make internal copy of operational params */
        object->input       = operation->input;
        object->output      = operation->output;
        object->inputLength = operation->inputLength;

        status = AESCBCLPF3HSM_processData(handle);
    }
    else /* Operation was finalized without additional data to process */
    {
        /* Save the object's returnStatus in case it is
         * overwritten during setup of a new segmented operation
         * after the operationInProgress flag is cleared.
         */
        status = object->common.returnStatus;

        object->segmentedOperationInProgress = false;

        if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK)
        {
            object->callbackFxn(handle, status, (AESCBC_OperationUnion *)operation, operationType);

            /* Always return success in callback mode */
            status = AESCBC_STATUS_SUCCESS;
        }
    }

    return status;
}

#endif
