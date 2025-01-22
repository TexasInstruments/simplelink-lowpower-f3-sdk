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

#include <ti/drivers/aescmac/AESCMACLPF3.h>

#include <ti/drivers/AESCMAC.h>
#include <ti/drivers/AESCommon.h>
#include <ti/drivers/aesecb/AESECBLPF3.h>
#include <ti/drivers/cryptoutils/aes/AESCommonLPF3.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKey.h>
#include <ti/drivers/cryptoutils/sharedresources/CryptoResourceLPF3.h>
#include <ti/drivers/cryptoutils/utils/CryptoUtils.h>
#include <ti/drivers/dma/UDMALPF3.h>

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    #include <ti/drivers/cryptoutils/hsm/HSMLPF3.h>
    #include <ti/drivers/cryptoutils/hsm/HSMLPF3Utility.h>
    #include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_asset_policy.h>
    #include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_result.h>
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

/**
 * The threshold at which blocking and callback mode transfers will utilize DMA.
 * For data lengths below this threshold, polling CPU R/W will be used instead
 * of DMA. With task switching and interrupt overhead, it is inefficient to
 * utilize DMA for shorter length operations.
 * The threshold value must be a multiple of AES_BLOCK_SIZE.
 */
#define AESCMACLPF3_DMA_SIZE_THRESHOLD (1U * AES_BLOCK_SIZE)

#define AESCMAC_CONST_RB  ((uint8_t)0x87U)
#define AESCMAC_MSB_CHECK ((uint8_t)0x80U)
#define AESCMAC_PADDING   ((uint8_t)0x80U)

typedef enum
{
    AESCMAC_SUBKEY1,
    AESCMAC_SUBKEY2
} AESCMAC_SUBKEY_NUM;

/* Forward declarations */
static void AESCMACLPF3_deriveSubKey(uint8_t *buffer);
static void AESCMACLPF3_generateSubKey(AESCMAC_SUBKEY_NUM subKeyNum, uint32_t subKey[AES_BLOCK_SIZE_WORDS]);
static void AESCMACLPF3_getResult(AESCMACLPF3_Object *object);
static inline void AESCMACLPF3_prepareFinalInputBlock(AESCMACLPF3_Object *object, size_t *transactionLength);
static int_fast16_t AESCMACLPF3_setupSegmentedOperation(AESCMACLPF3_Object *object, const CryptoKey *key);
static int_fast16_t AESCMACLPF3_oneStepOperation(AESCMAC_Handle handle,
                                                 AESCMAC_Operation *operation,
                                                 const CryptoKey *key,
                                                 AESCMAC_OperationType operationType);
static int_fast16_t AESCMACLPF3_startOperation(AESCMAC_Handle handle);
static inline int_fast16_t AESCMACLPF3_waitForResult(AESCMAC_Handle handle);
static inline void AESCMACLPF3_xorBlock(uint32_t *block1_dst, const uint32_t *block2);

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/* OneStep-related internal APIs*/
static int_fast16_t AESCMACLPF3HSM_oneStepOperation(AESCMAC_Handle handle,
                                                    AESCMAC_Operation *operation,
                                                    const CryptoKey *key,
                                                    AESCMAC_OperationType operationType);
static void AESCMACLPF3HSM_postProcessingCommon(AESCMAC_Handle handle, int_fast16_t status);
static int_fast16_t AESCMACLPF3HSM_performHSMOperation(AESCMAC_Handle handle);
static int_fast16_t AESCMACLPF3HSM_processFinalBlock(AESCMAC_Handle handle);

/* Segmented addData-related internal APIs*/
static int_fast16_t AESCMACLPF3HSM_addData(AESCMAC_Handle handle, AESCMAC_Operation *operation);

/* Segmented finalize-related internal APIs*/
static int_fast16_t AESCMACLPF3HSM_finalize(AESCMAC_Handle handle, AESCMAC_Operation *operation);

/* Asset management-related internal APIs*/
static int_fast16_t AESCMACLPF3HSM_createAndLoadKeyAssetID(AESCMAC_Handle handle);
static int_fast16_t AESCMACLPF3HSM_createKeyAsset(AESCMAC_Handle handle);
static int_fast16_t AESCMACLPF3HSM_loadKeyAsset(AESCMAC_Handle handle, uint8_t *key);
static int_fast16_t AESCMACLPF3HSM_CreateTempAssetID(AESCMAC_Handle handle);
static int_fast16_t AESCMACLPF3HSM_freeAllAssets(AESCMAC_Handle handle);
static int_fast16_t AESCMACLPF3HSM_freeAssetID(AESCMAC_Handle handle, uint32_t AssetID);
#endif

/*
 *  ======== AESCMACLPF3_getObject ========
 */
static inline AESCMACLPF3_Object *AESCMACLPF3_getObject(AESCMAC_Handle handle)
{
    AESCMACLPF3_Object *object = (AESCMACLPF3_Object *)handle->object;
    DebugP_assert(object);

    return object;
}

/*
 *  ======== AESCMACLPF3_hwiFxn ========
 */
static void AESCMACLPF3_hwiFxn(uintptr_t arg0)
{
    AESCMAC_Handle handle      = (AESCMAC_Handle)arg0;
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);

    /*
     * Only the input channel A interrupt is enabled.
     */
    uint32_t intStatus = AESGetMaskedInterruptStatus();

    /* Disable DMA, clear interupts, and release power constraint */
    AESCommonLPF3_cleanupHwi(&object->common);

    if ((intStatus & (uint32_t)AES_MIS_CHADONE_M) != (uint32_t)0U)
    {
        UDMALPF3_clearInterrupt(AESCommonLPF3_DMA_CHA_BITMASK);

        AESCMACLPF3_getResult(object);

        /* Clear operation in-progress if one-step or finalize operation */
        if (object->operationType != AESCMAC_OP_TYPE_SEGMENTED_SIGN &&
            object->operationType != AESCMAC_OP_TYPE_SEGMENTED_VERIFY)
        {
            AESCommonLPF3_clearOperationInProgress(&object->common);
        }

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
            object->callbackFxn((AESCMAC_Handle)handle,
                                object->common.returnStatus,
                                object->operation,
                                object->operationType);
        }
    }
}

/*
 *  ======== AESCMAC_init ========
 */
void AESCMAC_init(void)
{
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    HSMLPF3_constructRTOSObjects();
#endif
    AESCommonLPF3_init();
}

/*
 *  ======== AESCMAC_construct ========
 */
AESCMAC_Handle AESCMAC_construct(AESCMAC_Config *config, const AESCMAC_Params *params)
{
    DebugP_assert(config);

    int_fast16_t status;
    AESCMAC_Handle handle      = config;
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);

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
    object->keyAssetID                   = 0U;
    object->tempAssetID                  = 0U;
#endif

    /* If params are NULL, use defaults */
    if (params == NULL)
    {
        params = &AESCMAC_defaultParams;
    }

    DebugP_assert((params->returnBehavior != AES_RETURN_BEHAVIOR_CALLBACK) || (params->callbackFxn != NULL));

    object->operationalMode = params->operationalMode;
    object->callbackFxn     = params->callbackFxn;
    object->threadSafe      = true;

    status = AESCommonLPF3_construct(&object->common, (AES_ReturnBehavior)params->returnBehavior, params->timeout);

    if (status != AES_STATUS_SUCCESS)
    {
        handle = NULL;
    }

    return handle;
}

/*
 *  ======== AESCMACLPF3_readTag ========
 */
void AESCMACLPF3_readTag(uint32_t tagOut[AES_TAG_LENGTH_BYTES / 4U])
{
    /* Wait until the operation is done */
    while (AESGetStatus() != (uint32_t)AES_STA_STATE_IDLE) {}

    AESReadTag32(&tagOut[0]);
}

/*
 *  ======== AESCMACLPF3_getResult ========
 */
static void AESCMACLPF3_getResult(AESCMACLPF3_Object *object)
{
    AESCMAC_Operation *operation = object->operation;
    uint8_t opcode               = (object->operationType & AESCMAC_OP_CODE_MASK);

    if (object->common.returnStatus == AESCMAC_STATUS_SUCCESS)
    {
        if ((object->common.key.encoding == CryptoKey_PLAINTEXT) || (object->common.key.encoding == CryptoKey_KEYSTORE))
        {
            /* If One-step or Finalize operation, process the final input block */
            if (opcode != AESCMAC_OP_CODE_SEGMENTED)
            {
                AESWriteBUF32(&object->finalInputBlock[0]);
            }

            AESCMACLPF3_readTag((uint32_t *)&object->intermediateTag[0]);
        }
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
        else if ((object->common.key.encoding == CryptoKey_PLAINTEXT_HSM) ||
                 (object->common.key.encoding == CryptoKey_KEYSTORE_HSM))
        {
            /* Do nothing as HSM does not rely on AES engine */
        }
#endif

        /* If One-step or Finalize operation, verify or copy the MAC */
        if (opcode != AESCMAC_OP_CODE_SEGMENTED)
        {
            if (object->operationType & AESCMAC_OP_FLAG_SIGN)
            {
                (void)memcpy((void *)operation->mac, (void *)&object->intermediateTag[0], operation->macLength);
            }
            else
            {
                /* Constant time comparison of output tag versus provided MAC */
                if (!CryptoUtils_buffersMatch(&object->intermediateTag[0], operation->mac, operation->macLength))
                {
                    object->common.returnStatus = AESCMAC_STATUS_MAC_INVALID;
                }
            }

            /* Zeroize the calculated MAC so that we are not keeping it in RAM unnecessarily */
            CryptoUtils_memset((void *)&object->intermediateTag[0],
                               sizeof(object->intermediateTag),
                               (uint8_t)0U,
                               sizeof(object->intermediateTag));
        }
    }
}

/*
 *  ======== AESCMACLPF3_startOperation ========
 */
static int_fast16_t AESCMACLPF3_startOperation(AESCMAC_Handle handle)
{
    AESCMACLPF3_Object *object   = AESCMACLPF3_getObject(handle);
    AESCMAC_Operation *operation = object->operation;
#if (ENABLE_KEY_STORAGE == 1)
    int_fast16_t keyStoreStatus;
    uint8_t KeyStore_keyingMaterial[AESCommonLPF3_256_KEY_LENGTH_BYTES];
    KeyStore_PSA_KeyUsage usage;
    KeyStore_PSA_Algorithm alg;
#endif

    /* Input pointer cannot be NULL if input length is non-zero */
    DebugP_assert((operation->inputLength == 0U) || operation->input);

    uint8_t opcode = (object->operationType & AESCMAC_OP_CODE_MASK);

    /*
     * MAC pointer cannot be NULL if performing a one-step operation or
     * finalizing a segmented operation
     */
    DebugP_assert((opcode == AESCMAC_OP_CODE_SEGMENTED) || (operation->mac));

    AESCMAC_OperationType operationType;
    int_fast16_t status      = AESCMAC_STATUS_ERROR;
    size_t transactionLength = operation->inputLength;

    /* Load key */
    if (object->common.key.encoding == CryptoKey_PLAINTEXT)
    {
        AESCommonLPF3_loadKey(&object->common.key);
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (object->common.key.encoding == CryptoKey_KEYSTORE)
    {
        usage = (object->operationType & AESCMAC_OP_FLAG_SIGN) ? KEYSTORE_PSA_KEY_USAGE_SIGN_MESSAGE
                                                               : KEYSTORE_PSA_KEY_USAGE_VERIFY_MESSAGE;
        alg   = (object->operationalMode == AESCMAC_OPMODE_CMAC) ? KEYSTORE_PSA_ALG_CMAC : KEYSTORE_PSA_ALG_CBC_MAC;

        keyStoreStatus = KeyStore_PSA_retrieveFromKeyStore(&object->common.key,
                                                           &KeyStore_keyingMaterial[0],
                                                           sizeof(KeyStore_keyingMaterial),
                                                           &object->keyAssetID,
                                                           alg,
                                                           usage);

        if (keyStoreStatus == KEYSTORE_PSA_STATUS_SUCCESS)
        {
            AESWriteKEY(KeyStore_keyingMaterial);
        }
        else if (keyStoreStatus == KEYSTORE_PSA_STATUS_INVALID_KEY_ID)
        {
            return AESCMAC_STATUS_KEYSTORE_INVALID_ID;
        }
        else
        {
            return AESCMAC_STATUS_KEYSTORE_GENERIC_ERROR;
        }
    }
#endif
    else
    {
        /* This condition should not occur - a valid encoding is checked before this point */
        return status;
    }

    /*
     * If One-step or Finalization operation, prepare the final
     * input block and adjust the transaction length accordingly.
     */
    if (opcode != AESCMAC_OP_CODE_SEGMENTED)
    {
        AESCMACLPF3_prepareFinalInputBlock(object, &transactionLength);
    }

    AESSetAUTOCFG(AESCMACLPF3_DEFAULT_AUTOCFG);

    /*
     * Set IV to intermediate tag (initialized to zero at the start
     * of a new operation).
     */
    AESWriteIV32((uint32_t *)&object->intermediateTag[0]);

    if (transactionLength == 0U)
    {
        /*
         * If transaction length is zero, only the final input block
         * of data remains to be processed for CMAC. Call AESCMAC_getResult()
         * to process the last block, obtain the result, and store status of
         * the operation in object->returnStatus.
         */
        AESCMACLPF3_getResult(object);

        /*
         * Save the object's data to provide to callback in case it
         * is overwritten during by the start of a new operation
         * after the operationInProgress flag is cleared or access
         * semaphore is posted.
         */
        status        = object->common.returnStatus;
        operationType = object->operationType;

        AESCommonLPF3_clearOperationInProgress(&object->common);

        /*
         * Handle clean up of the operation and post access semaphore to allow
         * callback to chain operations.
         */
        AESCommonLPF3_cleanup(&(object->common));

        if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK)
        {
            /* Call the callback function provided by the application */
            object->callbackFxn(handle, status, operation, operationType);

            /* Always return success in callback mode */
            status = AESCMAC_STATUS_SUCCESS;
        }
    }
    else
    {
        if ((object->common.returnBehavior == AES_RETURN_BEHAVIOR_POLLING) ||
            (transactionLength < AESCMACLPF3_DMA_SIZE_THRESHOLD))
        {
            /*
             * Process all data. Transaction length is updated in
             * AESCMACLPF3_prepareFinalInputBlock() to be the truncated
             * message length (which excludes the final block).
             */
            AESCMACLPF3_processBlocks(operation->input, transactionLength);
        }
        else
        {
            /*
             * We need to set the HWI function and priority since the same physical
             * interrupt is shared by multiple drivers and they all need to coexist.
             * Whenever a driver starts an operation, it registers its HWI callback
             * with the OS.
             */
            AESCMACLPF3_HWAttrs const *hwAttrs = handle->hwAttrs;
            AESCommonLPF3_setupHwi(AESCMACLPF3_hwiFxn, (uintptr_t)handle, hwAttrs->intPriority);

            /* Clear BUSHALT when using DMA */
            AESClearAUTOCFGBusHalt();

            /* Setup DMA configuration and set power constraint */
            AESCommonLPF3_setupDMA(&object->common, AESCMACLPF3_DMA_CONFIG);

            AESCommonLPF3_configInputDMA(operation->input, transactionLength);

            /* Enable interrupt upon input DMA done */
            AESSetIMASK((uint32_t)AES_IMASK_CHADONE_M);

            /* Manually trigger the DMA to start the ECB operation */
            AESSetTrigger((uint32_t)AES_TRG_DMACHA);
        }

        status = AESCMACLPF3_waitForResult(handle);
    }

    return status;
}

/*
 *  ======== AESCMACLPF3_processBlocks ========
 */
void AESCMACLPF3_processBlocks(const uint8_t *input, size_t transactionLength)
{
    size_t bytesProcessed                = 0;
    size_t transactionLengthDoubleBlocks = transactionLength & AES_DOUBLE_BLOCK_SIZE_MULTIPLE_MASK;
    size_t transactionLengthSingleBlocks = transactionLength & (size_t)~AES_DOUBLE_BLOCK_SIZE_MULTIPLE_MASK;

    /*
     * The code below has additional optimizations for word-aligned input data
     * to support lower latency AES-CCM operations for BLE stack.
     */
    if (IS_WORD_ALIGNED(input))
    {
        AESProcessAlignedBlocksCMAC((const uint32_t *)input, (uint32_t)AES_GET_NUM_BLOCKS(transactionLength));
    }
    else
    {
        while (bytesProcessed < transactionLengthDoubleBlocks)
        {
            AESWriteBUF(&input[bytesProcessed]);
            bytesProcessed += AES_BLOCK_SIZE;

            AESWriteBUF(&input[bytesProcessed]);
            bytesProcessed += AES_BLOCK_SIZE;
        }

        /* Process any remaining single block */
        if (transactionLengthSingleBlocks != 0U)
        {
            AESWriteBUF(&input[bytesProcessed]);
        }
    }
}

/*
 *  ======== AESCMACLPF3_xorBlock ========
 *  XOR's two 16-byte blocks, storing the result in block1_dst.
 */
static inline void AESCMACLPF3_xorBlock(uint32_t *block1_dst, const uint32_t *block2)
{
    uint_fast8_t i;

    for (i = (uint_fast8_t)0U; i < (uint_fast8_t)AES_BLOCK_SIZE_WORDS; i++)
    {
        block1_dst[i] = block1_dst[i] ^ block2[i];
    }
}

/*
 *  ======== AESCMACLPF3_prepareFinalInputBlock ========
 */
static inline void AESCMACLPF3_prepareFinalInputBlock(AESCMACLPF3_Object *object, size_t *transactionLength)
{
    AESCMAC_Operation *operation = object->operation;
    size_t finalInputLength      = 0U;
    size_t truncatedInputLength  = 0U;
    uint32_t subKey[AES_BLOCK_SIZE_WORDS];

    /* Copy last partial or full block of input into local buffer */
    CryptoUtils_memset((void *)&object->finalInputBlock[0],
                       sizeof(object->finalInputBlock),
                       (uint8_t)0U,
                       sizeof(object->finalInputBlock));

    if (operation->inputLength != 0U)
    {
        finalInputLength = AES_NON_BLOCK_SIZE_MULTIPLE_LENGTH(operation->inputLength);

        if (finalInputLength == 0U)
        {
            finalInputLength = AES_BLOCK_SIZE;
        }

        truncatedInputLength = operation->inputLength - finalInputLength;

        (void)memcpy(object->finalInputBlock, &operation->input[truncatedInputLength], finalInputLength);
    }

    /* CMAC requires the final block to be XOR'd with a subkey */
    if (object->operationalMode == AESCMAC_OPMODE_CMAC)
    {
        /* Check if input message length is a positive block multiple */
        if ((operation->inputLength != 0U) && (finalInputLength == AES_BLOCK_SIZE))
        {
            /* Generate subkey1 */
            AESCMACLPF3_generateSubKey(AESCMAC_SUBKEY1, subKey);
        }
        else
        {
            /* Generate subkey2 */
            AESCMACLPF3_generateSubKey(AESCMAC_SUBKEY2, subKey);

            /* Set padding byte if partial block */
            ((uint8_t *)&object->finalInputBlock[0])[finalInputLength] = AESCMAC_PADDING;
        }

        /* XOR final block with subkey */
        AESCMACLPF3_xorBlock(object->finalInputBlock, subKey);
    }

    *transactionLength = truncatedInputLength;
}

/*
 *  ======== AESCMACLPF3_oneStepOperation ========
 */
static int_fast16_t AESCMACLPF3_oneStepOperation(AESCMAC_Handle handle,
                                                 AESCMAC_Operation *operation,
                                                 const CryptoKey *key,
                                                 AESCMAC_OperationType operationType)
{
    DebugP_assert(handle);
    DebugP_assert(operation);
    DebugP_assert(key);
    /* No need to assert operationType since we control it within the driver */

    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);

    /* CBC-MAC is not permitted for zero length messages */
    if ((object->operationalMode == AESCMAC_OPMODE_CBCMAC) && (operation->inputLength == 0U))
    {
        return AESCMAC_STATUS_ERROR;
    }

#if (AESCommonLPF3_UNALIGNED_IO_SUPPORT_ENABLE == 0)
    /* Check word-alignment of input pointer */
    if (!IS_WORD_ALIGNED(operation->input))
    {
        return AESCMAC_STATUS_UNALIGNED_IO_NOT_SUPPORTED;
    }
#endif

    if (operation->macLength > sizeof(object->intermediateTag))
    {
        return AESCMAC_STATUS_ERROR;
    }

    /*
     * Check that there is no operation already in progress for this driver
     * instance.
     */
    int_fast16_t status = AESCommonLPF3_setOperationInProgress(&object->common);

    if (status != AESCMAC_STATUS_SUCCESS)
    {
        return status;
    }

    if (object->threadSafe)
    {
        if (!CryptoResourceLPF3_acquireLock(object->common.semaphoreTimeout))
        {
            AESCommonLPF3_clearOperationInProgress(&object->common);
            return AESCMAC_STATUS_RESOURCE_UNAVAILABLE;
        }

        object->common.cryptoResourceLocked = true;
    }

    object->operation           = operation;
    object->operationType       = operationType;
    /* We will only change the returnStatus if there is an error or cancellation */
    object->common.returnStatus = AESCMAC_STATUS_SUCCESS;
    /* Make internal copy of crypto key */
    object->common.key          = *key;

    /* Zero the intermediate tag because it will be used as the IV */
    CryptoUtils_memset((void *)&object->intermediateTag[0],
                       sizeof(object->intermediateTag),
                       (uint8_t)0U,
                       sizeof(object->intermediateTag));

    if (object->common.key.encoding == CryptoKey_PLAINTEXT || object->common.key.encoding == CryptoKey_KEYSTORE)
    {
        status = AESCMACLPF3_startOperation(handle);
    }
    else
    {
        status = AESCMAC_STATUS_ERROR;
    }

    if ((status != AESCMAC_STATUS_SUCCESS) && (object->common.cryptoResourceLocked))
    {
        CryptoResourceLPF3_releaseLock();
        object->common.cryptoResourceLocked = false;
        AESCommonLPF3_clearOperationInProgress(&object->common);
    }

    return status;
}

/*
 *  ======== AESCMACLPF3_waitForResult ========
 */
static inline int_fast16_t AESCMACLPF3_waitForResult(AESCMAC_Handle handle)
{
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    int_fast16_t status        = AESCMAC_STATUS_ERROR;
    uint8_t opcode             = (object->operationType & AESCMAC_OP_CODE_MASK);

    if ((object->common.returnBehavior == AES_RETURN_BEHAVIOR_POLLING) ||
        (object->operation->inputLength < AESCMACLPF3_DMA_SIZE_THRESHOLD))
    {
        AESCMACLPF3_getResult(object);

        /*
         * Save the object's returnStatus before clearing operationInProgress or
         * posting the access semaphore in case it is overwritten.
         */
        status = object->common.returnStatus;

        /* Mark operation as no longer in progress if one-step or final operation */
        if (opcode != AESCMAC_OP_CODE_SEGMENTED)
        {
            AESCommonLPF3_clearOperationInProgress(&object->common);
        }

        AESCommonLPF3_cleanup(&(object->common));

        if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK)
        {
            object->callbackFxn(handle, status, object->operation, object->operationType);

            /* Always return success in callback mode */
            status = AESCMAC_STATUS_SUCCESS;
        }
    }
    else if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_BLOCKING)
    {
        (void)SemaphoreP_pend((SemaphoreP_Handle)&CryptoResourceLPF3_operationSemaphore,
                              (uint32_t)SemaphoreP_WAIT_FOREVER);

        status = object->common.returnStatus;
    }
    else /* AES_RETURN_BEHAVIOR_CALLBACK */
    {
        /* AESCMAC_STATUS_SUCCESS is always returned in callback mode */
        status = AESCMAC_STATUS_SUCCESS;
    }

    return status;
}

/*
 *  ======== AESCMAC_oneStepSign ========
 */
int_fast16_t AESCMAC_oneStepSign(AESCMAC_Handle handle, AESCMAC_Operation *operation, CryptoKey *key)
{
    int_fast16_t status;

    if ((key->encoding == CryptoKey_PLAINTEXT) || (key->encoding == CryptoKey_KEYSTORE))
    {
        status = AESCMACLPF3_oneStepOperation(handle, operation, key, AESCMAC_OP_TYPE_SIGN);
    }
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    else if ((key->encoding == CryptoKey_PLAINTEXT_HSM) || (key->encoding == CryptoKey_KEYSTORE_HSM))
    {
        status = AESCMACLPF3HSM_oneStepOperation(handle, operation, key, AESCMAC_OP_TYPE_SIGN);
    }
#endif
    else
    {
        status = AESCMAC_STATUS_ERROR;
    }
    return status;
}

/*
 *  ======== AESCMAC_oneStepVerify ========
 */
int_fast16_t AESCMAC_oneStepVerify(AESCMAC_Handle handle, AESCMAC_Operation *operation, CryptoKey *key)
{
    int_fast16_t status;

    if ((key->encoding == CryptoKey_PLAINTEXT) || (key->encoding == CryptoKey_KEYSTORE))
    {
        status = AESCMACLPF3_oneStepOperation(handle, operation, key, AESCMAC_OP_TYPE_VERIFY);
    }
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    else if ((key->encoding == CryptoKey_PLAINTEXT_HSM) || (key->encoding == CryptoKey_KEYSTORE_HSM))
    {
        status = AESCMACLPF3HSM_oneStepOperation(handle, operation, key, AESCMAC_OP_TYPE_VERIFY);
    }
#endif
    else
    {
        status = AESCMAC_STATUS_ERROR;
    }
    return status;
}

/*
 *  ======== AESCMACLPF3_setupSegmentedOperation ========
 */
static int_fast16_t AESCMACLPF3_setupSegmentedOperation(AESCMACLPF3_Object *object, const CryptoKey *key)
{
    DebugP_assert(key);

    int_fast16_t status = AESCMAC_STATUS_SUCCESS;

    /*
     * Key material pointer and length are not asserted until adding or
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
        /* A segmented operation may have been started but not finalized yet */
        if (object->segmentedOperationInProgress)
        {
            return AESCMAC_STATUS_ERROR;
        }

        /* Make internal copy of crypto key */
        object->common.key = *key;

        /* returnStatus is only changed in the case of an error or cancellation */
        object->common.returnStatus = AES_STATUS_SUCCESS;

        object->segmentedOperationInProgress = true;

        object->keyAssetID  = 0U;
        object->tempAssetID = 0U;
    }
#endif
    else
    {
        status = AESCMAC_STATUS_ERROR;
    }

    if (status == AESCMAC_STATUS_SUCCESS)
    {
        /*
         * Initialize operation pointer to NULL in case AESCBC_cancelOperation
         * is called after AESCMAC_setupXXXX and callback should be skipped.
         */
        object->operation = NULL;

        /* Zero the intermediate tag because it will be used as the IV */
        CryptoUtils_memset((void *)&object->intermediateTag[0],
                           sizeof(object->intermediateTag),
                           (uint8_t)0U,
                           sizeof(object->intermediateTag));
    }

    return status;
}

/*
 *  ======== AESCMAC_setupSign ========
 */
int_fast16_t AESCMAC_setupSign(AESCMAC_Handle handle, const CryptoKey *key)
{
    DebugP_assert(handle);

    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);

    int_fast16_t status = AESCMACLPF3_setupSegmentedOperation(object, key);

    if (status == AESCMAC_STATUS_SUCCESS)
    {
        object->operationType = AESCMAC_OP_TYPE_SEGMENTED_SIGN;
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
        if ((key->encoding == CryptoKey_PLAINTEXT_HSM) || (key->encoding == CryptoKey_KEYSTORE_HSM))
        {
            /* If the HSM IP and/or HSMSAL failed to boot then we cannot perform any HSM-related operation */
            if (object->hsmStatus == AESCMAC_STATUS_ERROR)
            {
                return AESCMAC_STATUS_ERROR;
            }

            if (!HSMLPF3_acquireLock(object->common.semaphoreTimeout, (uintptr_t)handle))
            {
                return AESCMAC_STATUS_RESOURCE_UNAVAILABLE;
            }

            status = AESCMACLPF3HSM_createAndLoadKeyAssetID(handle);

            if (status != AESCMAC_STATUS_SUCCESS)
            {
                (void)AESCMACLPF3HSM_freeAllAssets(handle);
            }

            HSMLPF3_releaseLock();
        }
#endif
    }

    return status;
}

/*
 *  ======== AESCMAC_setupVerify ========
 */
int_fast16_t AESCMAC_setupVerify(AESCMAC_Handle handle, const CryptoKey *key)
{
    DebugP_assert(handle);

    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);

    int_fast16_t status = AESCMACLPF3_setupSegmentedOperation(object, key);

    if (status == AESCMAC_STATUS_SUCCESS)
    {
        object->operationType = AESCMAC_OP_TYPE_SEGMENTED_VERIFY;
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
        if ((key->encoding == CryptoKey_PLAINTEXT_HSM) || (key->encoding == CryptoKey_KEYSTORE_HSM))
        {
            /* If the HSM IP and/or HSMSAL failed to boot then we cannot perform any HSM-related operation */
            if (object->hsmStatus == AESCMAC_STATUS_ERROR)
            {
                return AESCMAC_STATUS_ERROR;
            }

            if (!HSMLPF3_acquireLock(object->common.semaphoreTimeout, (uintptr_t)handle))
            {
                return AESCMAC_STATUS_RESOURCE_UNAVAILABLE;
            }

            status = AESCMACLPF3HSM_createAndLoadKeyAssetID(handle);

            if (status != AESCMAC_STATUS_SUCCESS)
            {
                (void)AESCMACLPF3HSM_freeAllAssets(handle);
            }

            HSMLPF3_releaseLock();
        }
#endif
    }

    return status;
}

/*
 *  ======== AESCMAC_addData ========
 */
int_fast16_t AESCMAC_addData(AESCMAC_Handle handle, AESCMAC_Operation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    int_fast16_t status        = AESCMAC_STATUS_ERROR;

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    if ((object->common.key.encoding == CryptoKey_PLAINTEXT_HSM) ||
        (object->common.key.encoding == CryptoKey_KEYSTORE_HSM))
    {
        return AESCMACLPF3HSM_addData(handle, operation);
    }
#endif

    /* Assert the segmented operation was setup */
    DebugP_assert((object->operationType == AESCMAC_OP_TYPE_SEGMENTED_SIGN) ||
                  (object->operationType == AESCMAC_OP_TYPE_SEGMENTED_VERIFY));

    /* Check for previous failure or cancellation of segmented operation */
    if (object->common.returnStatus != AESCMAC_STATUS_SUCCESS)
    {
        /* Return the status of the previous call.
         * The callback function will not be executed.
         */
        return object->common.returnStatus;
    }

#if (AESCommonLPF3_UNALIGNED_IO_SUPPORT_ENABLE == 0)
    /* Check word-alignment of input pointer */
    if (!IS_WORD_ALIGNED(operation->input))
    {
        return AESCMAC_STATUS_UNALIGNED_IO_NOT_SUPPORTED;
    }
#endif

    /* Verify the input length is non-zero and a multiple of the block size */
    if ((operation->inputLength == 0U) || (AES_NON_BLOCK_SIZE_MULTIPLE_LENGTH(operation->inputLength) > 0U))
    {
        return AESCMAC_STATUS_ERROR;
    }

    if (object->threadSafe)
    {
        if (!CryptoResourceLPF3_acquireLock(object->common.semaphoreTimeout))
        {
            return AESCMAC_STATUS_RESOURCE_UNAVAILABLE;
        }

        object->common.cryptoResourceLocked = true;
    }

    object->operation = operation;
    if (object->common.key.encoding == CryptoKey_PLAINTEXT || object->common.key.encoding == CryptoKey_KEYSTORE)
    {
        status = AESCMACLPF3_startOperation(handle);
    }
    else
    {
        status = AESCMAC_STATUS_ERROR;
    }

    if ((status != AESCMAC_STATUS_SUCCESS) && (object->common.cryptoResourceLocked))
    {
        CryptoResourceLPF3_releaseLock();
        object->common.cryptoResourceLocked = false;
    }

    return status;
}

/* Perform ECB encryption on a block of zeros */
static inline void AESCMACLPF3_encryptZeroBlockECB(uint32_t output[AES_BLOCK_SIZE_WORDS])
{
    const uint32_t zeroBlock[AES_BLOCK_SIZE_WORDS] = {0};

    AESSetAUTOCFG(AESECBLPF3_SINGLE_BLOCK_AUTOCFG);

    /* Write block of zeros to input */
    AESWriteBUF32(&zeroBlock[0]);

    /* Wait until the operation is done */
    while (AESGetStatus() != (uint32_t)AES_STA_STATE_IDLE) {}

    /* Read output */
    AESReadTXT32(&output[0]);
}

/*
 *  ======== AESCMACLPF3_generateSubKey ========
 *  Generate AES CMAC subkey based on
 *  https://tools.ietf.org/html/rfc4493#section-2.3
 */
static void AESCMACLPF3_generateSubKey(AESCMAC_SUBKEY_NUM subKeyNum, uint32_t subKey[AES_BLOCK_SIZE_WORDS])
{
    /* Perform ECB encryption on the block of zeros using CPU R/W */
    AESCMACLPF3_encryptZeroBlockECB(&subKey[0]);

    /*
     * At this point, subKey buffer only has the ciphertext
     * generated by encrypting a block of 0's.
     * Derive SubKey1.
     */
    AESCMACLPF3_deriveSubKey((uint8_t *)&subKey[0]);

    if (subKeyNum == AESCMAC_SUBKEY2)
    {
        /* At this point, subKey buffer contains SubKey1. Derive SubKey2. */
        AESCMACLPF3_deriveSubKey((uint8_t *)&subKey[0]);
    }
}

/*
 *  ======== AESCMACLPF3_deriveSubKey ========
 */
static void AESCMACLPF3_deriveSubKey(uint8_t *buffer)
{
    uint_fast8_t i;
    uint8_t xorMask = (uint8_t)0U;

    if ((buffer[0] & AESCMAC_MSB_CHECK) != (uint8_t)0U)
    {
        xorMask = AESCMAC_CONST_RB;
    }

    for (i = (uint_fast8_t)0U; i < (uint_fast8_t)AES_BLOCK_SIZE; i += (uint_fast8_t)1U)
    {
        if (i != (uint_fast8_t)0U)
        {
            buffer[i - (uint_fast8_t)1U] += (buffer[i] >> 7U);
        }

        buffer[i] = buffer[i] << 1U;
    }

    buffer[AES_BLOCK_SIZE - 1U] ^= xorMask;
}

/*
 *  ======== AESCMAC_finalize ========
 */
int_fast16_t AESCMAC_finalize(AESCMAC_Handle handle, AESCMAC_Operation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    int_fast16_t status        = AESCMAC_STATUS_ERROR;

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    if ((object->common.key.encoding == CryptoKey_PLAINTEXT_HSM) ||
        (object->common.key.encoding == CryptoKey_KEYSTORE_HSM))
    {
        return AESCMACLPF3HSM_finalize(handle, operation);
    }
#endif

    /* Assert the segmented operation was setup */
    DebugP_assert((object->operationType == AESCMAC_OP_TYPE_SEGMENTED_SIGN) ||
                  (object->operationType == AESCMAC_OP_TYPE_SEGMENTED_VERIFY));

    /* Check for previous failure or cancellation of segmented operation */
    if (object->common.returnStatus != AESCMAC_STATUS_SUCCESS)
    {
        /*
         * Return the failure status of previous call.
         * The callback will not be called.
         */
        return object->common.returnStatus;
    }

#if (AESCommonLPF3_UNALIGNED_IO_SUPPORT_ENABLE == 0)
    /* Check word-alignment of input pointer */
    if (!IS_WORD_ALIGNED(operation->input))
    {
        return AESCMAC_STATUS_UNALIGNED_IO_NOT_SUPPORTED;
    }
#endif

    if (operation->macLength > sizeof(object->intermediateTag))
    {
        return AESCMAC_STATUS_ERROR;
    }

    if (operation->inputLength == 0U)
    {
        /*
         * Finalizing an operation without providing data to process is not
         * supported. Return an error.
         */
        return AESCMAC_STATUS_ERROR;
    }

    /* Try and obtain access to the crypto module */
    if (object->threadSafe)
    {
        if (!CryptoResourceLPF3_acquireLock(object->common.semaphoreTimeout))
        {
            return AESCMAC_STATUS_RESOURCE_UNAVAILABLE;
        }

        object->common.cryptoResourceLocked = true;
    }

    if (object->operationType == AESCMAC_OP_TYPE_SEGMENTED_SIGN)
    {
        object->operationType = AESCMAC_OP_TYPE_FINALIZE_SIGN;
    }
    else
    {
        object->operationType = AESCMAC_OP_TYPE_FINALIZE_VERIFY;
    }

    object->operation = operation;
    if (object->common.key.encoding == CryptoKey_PLAINTEXT || object->common.key.encoding == CryptoKey_KEYSTORE)
    {
        status = AESCMACLPF3_startOperation(handle);
    }
    else
    {
        status = AESCMAC_STATUS_ERROR;
    }

    if ((status != AESCMAC_STATUS_SUCCESS) && (object->common.cryptoResourceLocked))
    {
        CryptoResourceLPF3_releaseLock();
        object->common.cryptoResourceLocked = false;
    }

    return status;
}

/*
 *  ======== AESCMAC_close ========
 */
void AESCMAC_close(AESCMAC_Handle handle)
{
    DebugP_assert(handle);

    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    if (object->common.key.encoding & CRYPTOKEY_HSM)
    {
        /* Attempt to free all assets the driver created. If there is a failure,
         * we cannot return so in this void API. This is a double-check cleanup
         * anyways, since the driver should clean up these assets at the end of
         * each operation.
         */
        (void)AESCMACLPF3HSM_freeAllAssets(handle);
    }
#endif

    AESCommonLPF3_close(&object->common);
}

/*
 *  ======== AESCMAC_cancelOperation ========
 */
int_fast16_t AESCMAC_cancelOperation(AESCMAC_Handle handle)
{
    DebugP_assert(handle);

    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);

    uintptr_t interruptKey = HwiP_disable();

    /* Return success if there is no active operation to cancel.
     * Do not execute the callback as it would have been executed already
     * when the operation completed.
     */

    /* If the operation is in _HSM mode, then the driver does not rely on 'object->common' metadata
     * and instead relies on HSM metadata. The check below insures that.
     */
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    if (((object->common.key.encoding & CRYPTOKEY_HSM) == 0U) && (!object->common.operationInProgress))
#else
    if (!object->common.operationInProgress)
#endif
    {
        HwiP_restore(interruptKey);
        return AESCMAC_STATUS_SUCCESS;
    }

    HwiP_restore(interruptKey);

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    /* Since the HSM cannot cancel an in-progress token, we must wait for the result to allow for
     * subsequent token submissions to succeed.
     */
    (void)HSMLPF3_cancelOperation();

    object->segmentedOperationInProgress = false;

    if (object->common.key.encoding & CRYPTOKEY_HSM)
    {
        /* Attempt to free all the assets the driver created. Whether or not this is successful,
         * we should continue on to attempt cancellation of the operation.
         */
        (void)AESCMACLPF3HSM_freeAllAssets(handle);
    }
#endif

    /* Cancel DMA for input channel A only, clear operation in-progress,
     * and release crypto resource if locked.
     */
    AESCommonLPF3_cancelOperation(&object->common, false);

    /* Operation pointer could be NULL if a segmented operation was setup
     * but neither AESCMAC_addData or AESCMAC_finalize was called.
     */
    if ((object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK) && (object->operation != NULL))
    {
        /* Call the callback function provided by the application */
        object->callbackFxn(handle, AESCMAC_STATUS_CANCELED, object->operation, object->operationType);
    }

    return AESCMAC_STATUS_SUCCESS;
}

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)

/*
 *  ======== AESCMACLPF3HSM_oneStepOperation ========
 */
static int_fast16_t AESCMACLPF3HSM_oneStepOperation(AESCMAC_Handle handle,
                                                    AESCMAC_Operation *operation,
                                                    const CryptoKey *key,
                                                    AESCMAC_OperationType operationType)
{
    DebugP_assert(handle);
    DebugP_assert(operation);
    DebugP_assert(key);
    /* No need to assert operationType since we control it within the driver */
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    int_fast16_t status        = AESCMAC_STATUS_SUCCESS;

    /* If the HSM IP and/or HSMSAL failed to boot then we cannot perform any HSM-related operation */
    if (object->hsmStatus == HSMLPF3_STATUS_ERROR)
    {
        return HSMLPF3_STATUS_ERROR;
    }

    /* CBC-MAC is not permitted for zero length messages */
    if ((object->operationalMode == AESCMAC_OPMODE_CBCMAC) && (operation->inputLength == 0U))
    {
        return AESCMAC_STATUS_ERROR;
    }

    if ((operation->macLength > sizeof(object->intermediateTag)) || (operation->macLength == 0U))
    {
        return AESCMAC_STATUS_ERROR;
    }

    /* A segmented operation may have been started but not finalized yet */
    if (object->segmentedOperationInProgress)
    {
        return AESCMAC_STATUS_ERROR;
    }

    object->operation           = operation;
    object->operationType       = operationType;
    /* We will only change the returnStatus if there is an error or cancellation */
    object->common.returnStatus = AESCMAC_STATUS_SUCCESS;
    /* Make internal copy of crypto key */
    object->common.key          = *key;
    object->keyAssetID          = 0U;
    object->tempAssetID         = 0U;

    /* Zero the intermediate tag because it will be used as the IV */
    CryptoUtils_memset((void *)&object->intermediateTag[0],
                       sizeof(object->intermediateTag),
                       (uint8_t)0U,
                       sizeof(object->intermediateTag));

    if (!HSMLPF3_acquireLock(object->common.semaphoreTimeout, (uintptr_t)handle))
    {
        return AESCMAC_STATUS_RESOURCE_UNAVAILABLE;
    }

    status = AESCMACLPF3HSM_createAndLoadKeyAssetID(handle);

    /* If input length is larger than 1 block and it is not a multiple of block-size,
     * then a one step operation becomes a segmented operation and requires a state
     * asset to store the intermediate state of the MAC within the HSM.
     */
    if ((operation->inputLength > AES_BLOCK_SIZE) && (!HSM_IS_SIZE_MULTIPLE_OF_AES_BLOCK_SIZE(operation->inputLength)))
    {
        status = AESCMACLPF3HSM_CreateTempAssetID(handle);
    }

    if (status != AESCMAC_STATUS_SUCCESS)
    {
        /* In the case of failure to initialize the operation, we need to free all assets allocated.
         * Capturing the return status of this operation is pointless since we are returning an
         * error code anyways.
         */
        (void)AESCMACLPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }
    else
    {
        status = AESCMACLPF3HSM_performHSMOperation(handle);
    }

    return status;
}

/*
 *  ======== AESCMACLPF3HSM_postProcessingCommon ========
 */
static void AESCMACLPF3HSM_postProcessingCommon(AESCMAC_Handle handle, int_fast16_t status)
{
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    int_fast16_t finalStatus   = status;

    if (status == AESCMAC_STATUS_SUCCESS)
    {
        /* For verification operations, the token will return success even if verification fails.
         * That is because the driver is itself doing the comparison to the expected MAC.
         * AESCMACLPF3_getResult will set object->common.returnStatus if the MACs do not match.
         */
        HSMLPF3_getAESCMACSignMac((void *)&object->intermediateTag[0], HSM_MAC_MAX_LENGTH);

        AESCMACLPF3_getResult(object);

        if (object->common.returnStatus == AESCMAC_STATUS_MAC_INVALID)
        {
            finalStatus = AESCMAC_STATUS_MAC_INVALID;
        }
        else
        {
            finalStatus = AESCMAC_STATUS_SUCCESS;
        }
    }

    /* In the case the HSM returns a failure code, we need to free all assets allocated. */
    if (AESCMACLPF3HSM_freeAllAssets(handle) != AESCMAC_STATUS_SUCCESS)
    {
        finalStatus = AESCMAC_STATUS_ERROR;
    }

    object->segmentedOperationInProgress = false;
    object->common.returnStatus          = finalStatus;

    HSMLPF3_releaseLock();

    if (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK)
    {
        object->callbackFxn(handle, object->common.returnStatus, object->operation, object->operationType);
    }
}

/*
 *  ======== AESCMACLPF3HSM_processFinalBlockPostProcessing ========
 */
inline static void AESCMACLPF3HSM_processFinalBlockPostProcessing(uintptr_t arg0)
{
    AESCMAC_Handle handle  = (AESCMAC_Handle)arg0;
    int_fast16_t status    = AESCMAC_STATUS_ERROR;
    int32_t physicalResult = HSMLPF3_getResultCode();
    int8_t tokenResult     = physicalResult & HSMLPF3_RETVAL_MASK;

    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = AESCMAC_STATUS_SUCCESS;
    }

    AESCMACLPF3HSM_postProcessingCommon(handle, status);
}

/*
 *  ======== AESCMACLPF3HSM_processFinalBlock ========
 */
static int_fast16_t AESCMACLPF3HSM_processFinalBlock(AESCMAC_Handle handle)
{
    AESCMACLPF3_Object *object   = AESCMACLPF3_getObject(handle);
    AESCMAC_Operation *operation = object->operation;
    int_fast16_t status          = AESCMAC_STATUS_ERROR;
    int_fast16_t hsmRetval       = HSMLPF3_STATUS_ERROR;
    size_t remainderLength       = operation->inputLength - object->inputLength;

    (void)memset(object->finalInputBlock, 0, AES_BLOCK_SIZE);
    (void)memcpy(object->finalInputBlock, operation->input + object->inputLength, remainderLength);

    object->input       = (uint8_t *)object->finalInputBlock;
    object->inputLength = remainderLength;

    HSMLPF3_constructCMACToken(object, false, true);

    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->common.returnBehavior,
                                    AESCMACLPF3HSM_processFinalBlockPostProcessing,
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
 *  ======== AESCMACLPF3HSM_performHSMOperationPostProcessing ========
 */
inline static void AESCMACLPF3HSM_performHSMOperationPostProcessing(uintptr_t arg0)
{
    AESCMAC_Handle handle        = (AESCMAC_Handle)arg0;
    AESCMACLPF3_Object *object   = AESCMACLPF3_getObject(handle);
    AESCMAC_Operation *operation = object->operation;
    int_fast16_t status          = AESCMAC_STATUS_ERROR;
    int32_t physicalResult       = HSMLPF3_getResultCode();
    int8_t tokenResult           = physicalResult & HSMLPF3_RETVAL_MASK;
    bool finalizeOp              = true;

    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        /* Check to see if one-step or finalize operations still have a remaining chunk to process. */
        if ((operation->inputLength > AES_BLOCK_SIZE) &&
            (!HSM_IS_SIZE_MULTIPLE_OF_AES_BLOCK_SIZE(operation->inputLength)))
        {
            status = AESCMACLPF3HSM_processFinalBlock(handle);

            if (status == AESCMAC_STATUS_SUCCESS)
            {
                finalizeOp = false;
            }
        }
        else
        {
            status = AESCMAC_STATUS_SUCCESS;
        }
    }

    if (finalizeOp)
    {
        AESCMACLPF3HSM_postProcessingCommon(handle, status);
    }
}

/*
 *  ======== AESCMACLPF3HSM_performHSMOperation ========
 */
static int_fast16_t AESCMACLPF3HSM_performHSMOperation(AESCMAC_Handle handle)
{
    AESCMACLPF3_Object *object   = AESCMACLPF3_getObject(handle);
    AESCMAC_Operation *operation = object->operation;
    int_fast16_t status          = AESCMAC_STATUS_ERROR;
    int_fast16_t hsmRetval       = HSMLPF3_STATUS_ERROR;
    bool isNew                   = true;
    bool isFinal                 = true;

    /* If operation is a segmented operation (this include finalize operations too) then,
     *  determine the values of isNew and isFinal based on whether the tempAsset has been created or not.
     */
    if ((object->operationType != AESCMAC_OP_TYPE_SIGN) && (object->operationType != AESCMAC_OP_TYPE_VERIFY))
    {
        if (object->tempAssetID == 0U)
        {
            status = AESCMACLPF3HSM_CreateTempAssetID(handle);
        }
        else
        {
            isNew = false;
        }

        /* If the operation is segmented, the user has to call #AESCMAC_finalize and therefore, the driver does not
         * finalize the HSM operations yet. */
        if ((object->operationType == AESCMAC_OP_TYPE_SEGMENTED_SIGN) ||
            (object->operationType == AESCMAC_OP_TYPE_SEGMENTED_VERIFY))
        {
            isFinal = false;
        }
    }

    /* The AESCMAC-HSM operations can be summed up in 3 scenarios:
     *  1. Operation input length (total length) is zero or less than AES_BLOCK_SIZE in which case the driver copies
     *  the user data into an internal buffer for the HSM to process in ONE token.
     *      - This applies to AESCMAC_oneStepSign, AESCMAC_oneStepVerify, and AESCMAC_finalize operations.
     *  2. Operation input length (total length) is a block-size-multiple in which case the driver uses the user's
     *  buffer for HSM to process in ONE token.
     *      - This applies to AESCMAC_oneStepSign, AESCMAC_oneStepVerify, AESCMAC_finalize operations.
     *      - This is also a rule for AESCMAC_addData operations.
     *  3. Operation input length (total length) is a larger than AES_BLOCK_SIZE and non-block-size-multiple in
     *  which case the driver truncates the last non-block-size-aligned input and for the HSM to process in the FIRST
     *  token. A SECOND token is expected to contain the remainder data copied into an internal buffer for the HSM to
     *  process it.
     *      - This applies to AESCMAC_oneStepSign, AESCMAC_oneStepVerify, AESCMAC_finalize operations.
     */
    if (operation->inputLength < AES_BLOCK_SIZE)
    {
        (void)memset(object->finalInputBlock, 0, AES_BLOCK_SIZE);
        (void)memcpy(object->finalInputBlock, operation->input, operation->inputLength);

        object->input       = (uint8_t *)object->finalInputBlock;
        object->inputLength = operation->inputLength;
    }
    else if (HSM_IS_SIZE_MULTIPLE_OF_AES_BLOCK_SIZE(operation->inputLength))
    {
        /* Get block-size aligned input length */
        object->input       = operation->input;
        object->inputLength = operation->inputLength;
    }
    else
    {
        /* Get block-size aligned input length */
        object->input       = operation->input;
        object->inputLength = operation->inputLength & AES_BLOCK_SIZE_MULTIPLE_MASK;

        isFinal = false;
    }

    HSMLPF3_constructCMACToken(object, isNew, isFinal);

    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->common.returnBehavior,
                                    AESCMACLPF3HSM_performHSMOperationPostProcessing,
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
        /* In the case of failure to initialize the operation, we need to free all assets allocated
         * Capturing the return status of this operation is pointless since we are returning an
         * error code anyways.
         */
        (void)AESCMACLPF3HSM_freeAllAssets(handle);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== AESCMACLPF3HSM_createAndLoadKeyAssetID ========
 */
static int_fast16_t AESCMACLPF3HSM_createAndLoadKeyAssetID(AESCMAC_Handle handle)
{
    int_fast16_t status        = AESCMAC_STATUS_ERROR;
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    uint8_t *keyMaterial       = NULL;
    #if (ENABLE_KEY_STORAGE == 1)
    uint8_t KeyStore_keyingMaterial[AESCommonLPF3_256_KEY_LENGTH_BYTES];
    KeyStore_PSA_KeyUsage usage = (object->operationType & AESCMAC_OP_FLAG_SIGN)
                                      ? KEYSTORE_PSA_KEY_USAGE_SIGN_MESSAGE
                                      : KEYSTORE_PSA_KEY_USAGE_VERIFY_MESSAGE;
    KeyStore_PSA_Algorithm alg  = (object->operationalMode == AESCMAC_OPMODE_CMAC) ? KEYSTORE_PSA_ALG_CMAC
                                                                                   : KEYSTORE_PSA_ALG_CBC_MAC;
    #endif

    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        keyMaterial = object->common.key.u.plaintext.keyMaterial;
    }
    #if (ENABLE_KEY_STORAGE == 1)
    else if (object->common.key.encoding == CryptoKey_KEYSTORE_HSM)
    {
        keyMaterial = &KeyStore_keyingMaterial[0];

        status = KeyStore_PSA_retrieveFromKeyStore(&object->common.key,
                                                   &KeyStore_keyingMaterial[0],
                                                   sizeof(KeyStore_keyingMaterial),
                                                   &object->keyAssetID,
                                                   alg,
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
            /* Key material has been retrieved in plaintext */
        }
    }
    #endif
    else
    {
        /* A valid encoding is checked before this point, so this condition should not happen */
        status = AESCMAC_STATUS_ERROR;
    }

    /* If we haven't already retrieved an asset directly from KeyStore, then the driver
     * will have to create and load an asset itself.
     */
    if (object->keyAssetID == 0)
    {
        status = AESCMACLPF3HSM_createKeyAsset(handle);
        if (status == AESCMAC_STATUS_SUCCESS)
        {
            /* Now that the driver has successfully created an asset, object->keyAssetID is now non-zero.
             * If any failure condition happens after this moment, the cleanup will expect
             * object->driverCreatedKeyAsset to be accurate, since the keyAssetID will reflect that there
             * is an asset to free, and the cleanup will need to know how to do that.
             */
            object->driverCreatedKeyAsset = true;

            status = AESCMACLPF3HSM_loadKeyAsset(handle, keyMaterial);
        }
        else
        {
            /* object->keyAssetID is still 0, so cleanup knows there's no asset to free */
        }
    }

    return status;
}

/*
 *  ======== AESCMACLPF3HSM_CreateKeyAssetPostProcessing ========
 */
static inline void AESCMACLPF3HSM_CreateKeyAssetPostProcessing(uintptr_t arg0)
{
    AESCMAC_Handle handle      = (AESCMAC_Handle)arg0;
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    int_fast16_t status        = AESCMAC_STATUS_ERROR;
    int32_t physicalResult     = HSMLPF3_getResultCode();
    int8_t tokenResult         = physicalResult & HSMLPF3_RETVAL_MASK;

    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->keyAssetID = HSMLPF3_getResultAssetID();
        status             = AESCMAC_STATUS_SUCCESS;
    }

    object->common.returnStatus = status;
}

/*
 *  ======== AESCMACLPF3HSM_createKeyAsset ========
 */
static int_fast16_t AESCMACLPF3HSM_createKeyAsset(AESCMAC_Handle handle)
{
    int_fast16_t status        = AESCMAC_STATUS_ERROR;
    int_fast16_t hsmRetval     = HSMLPF3_STATUS_ERROR;
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    uint64_t assetPolicy       = 0U;
    uint32_t keyLength         = 0U;

    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        keyLength = object->common.key.u.plaintext.keyLength;
    }
    #if (ENABLE_KEY_STORAGE == 1)
    else if (object->common.key.encoding == CryptoKey_KEYSTORE_HSM)
    {
        keyLength = object->common.key.u.keyStore.keyLength;
    }
    #endif

    /* Operation (Lower 16-bits + general Operation) + Direction + Mode */
    assetPolicy = EIP130_ASSET_POLICY_SYM_BASE | EIP130_ASSET_POLICY_SCUIMACCIPHER | EIP130_ASSET_POLICY_SCACAES |
                  EIP130_ASSET_POLICY_SCDIRENCGEN;

    if (object->operationalMode == AESCMAC_OPMODE_CMAC)
    {
        assetPolicy |= EIP130_ASSET_POLICY_SCMCMCMAC;
    }
    else
    {
        assetPolicy |= EIP130_ASSET_POLICY_SCMCMCBCMAC;
    }

    HSMLPF3_constructCreateAssetToken(assetPolicy, keyLength);

    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    AESCMACLPF3HSM_CreateKeyAssetPostProcessing,
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
 *  ======== AESCMACLPF3HSM_loadKeyAssetPostProcessing ========
 */
static inline void AESCMACLPF3HSM_loadKeyAssetPostProcessing(uintptr_t arg0)
{
    AESCMAC_Handle handle      = (AESCMAC_Handle)arg0;
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    int_fast16_t status        = AESCMAC_STATUS_ERROR;
    int32_t physicalResult     = HSMLPF3_getResultCode();
    int8_t tokenResult         = physicalResult & HSMLPF3_RETVAL_MASK;

    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = AESCMAC_STATUS_SUCCESS;
    }

    object->hsmStatus = status;
}

/*
 *  ======== AESCMACLPF3HSM_loadKeyAsset ========
 */
static int_fast16_t AESCMACLPF3HSM_loadKeyAsset(AESCMAC_Handle handle, uint8_t *key)
{
    int_fast16_t status        = AESCMAC_STATUS_ERROR;
    int_fast16_t hsmRetval     = HSMLPF3_STATUS_ERROR;
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    uint32_t keyLength         = 0U;

    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        keyLength = object->common.key.u.plaintext.keyLength;
    }
    #if (ENABLE_KEY_STORAGE == 1)
    else if (object->common.key.encoding == CryptoKey_KEYSTORE_HSM)
    {
        keyLength = object->common.key.u.keyStore.keyLength;
    }
    #endif

    HSMLPF3_constructLoadPlaintextAssetToken(key, keyLength, object->keyAssetID);

    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    AESCMACLPF3HSM_loadKeyAssetPostProcessing,
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
 *  ======== AESCMACLPF3HSM_createTempAssetPostProcessing ========
 */
static inline void AESCMACLPF3HSM_createTempAssetPostProcessing(uintptr_t arg0)
{
    AESCMAC_Handle handle      = (AESCMAC_Handle)arg0;
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    int_fast16_t status        = AESCMAC_STATUS_ERROR;
    int32_t physicalResult     = HSMLPF3_getResultCode();
    int8_t tokenResult         = physicalResult & HSMLPF3_RETVAL_MASK;

    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->tempAssetID = HSMLPF3_getResultAssetID();
        status              = AESCMAC_STATUS_SUCCESS;
    }

    object->common.returnStatus = status;
}

/*
 *  ======== AESCMACLPF3HSM_CreateTempAssetID ========
 */
static int_fast16_t AESCMACLPF3HSM_CreateTempAssetID(AESCMAC_Handle handle)
{
    int_fast16_t status        = AESCMAC_STATUS_ERROR;
    int_fast16_t hsmRetval     = HSMLPF3_STATUS_ERROR;
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    uint64_t assetPolicy       = 0U;

    /* Operation (Lower 16-bits + general Operation) + Mode. */
    assetPolicy = EIP130_ASSET_POLICY_SYM_TEMP | EIP130_ASSET_POLICY_SCUIMACCIPHER | EIP130_ASSET_POLICY_SCACAES |
                  EIP130_ASSET_POLICY_SCDIRENCGEN;

    if (object->operationalMode == AESCMAC_OPMODE_CMAC)
    {
        assetPolicy |= EIP130_ASSET_POLICY_SCMCMCMAC;
    }
    else
    {
        assetPolicy |= EIP130_ASSET_POLICY_SCMCMCBCMAC;
    }

    HSMLPF3_constructCreateAssetToken(assetPolicy, HSM_MAC_MAX_LENGTH);

    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    AESCMACLPF3HSM_createTempAssetPostProcessing,
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
 *  ======== AESCMACLPF3HSM_freeAllAssets ========
 */
static int_fast16_t AESCMACLPF3HSM_freeAllAssets(AESCMAC_Handle handle)
{
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    int_fast16_t status        = AESCCM_STATUS_SUCCESS;

    object->common.cryptoResourceLocked = true;

    if ((object->operationType == AESCMAC_OP_TYPE_SEGMENTED_SIGN) ||
        (object->operationType == AESCMAC_OP_TYPE_SEGMENTED_VERIFY))
    {
        return AESCCM_STATUS_SUCCESS;
    }

    if (object->keyAssetID != 0U)
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
            status = AESCMACLPF3HSM_freeAssetID(handle, object->keyAssetID);
        }
    #if (ENABLE_KEY_STORAGE == 1)
        else
        {
            KeyStore_PSA_KeyFileId keyID;

            GET_KEY_ID(keyID, object->common.key.u.keyStore.keyID);

            status = KeyStore_PSA_assetPostProcessing(keyID);
        }

        if (status == AESCMAC_STATUS_SUCCESS)
        {
            object->keyAssetID = 0;
        }
    #endif
    }

    if (object->tempAssetID != 0)
    {
        status = AESCMACLPF3HSM_freeAssetID(handle, object->tempAssetID);
        if (status == AESCMAC_STATUS_SUCCESS)
        {
            object->tempAssetID = 0U;
        }
    }

    object->common.cryptoResourceLocked = false;

    return status;
}

/*
 *  ======== AESCMACLPF3HSM_FreeAssetPostProcessing ========
 */
static inline void AESCMACLPF3HSM_FreeAssetPostProcessing(uintptr_t arg0)
{
    AESCMAC_Handle handle      = (AESCMAC_Handle)arg0;
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    int_fast16_t status        = AESCMAC_STATUS_ERROR;
    int32_t physicalResult     = HSMLPF3_getResultCode();
    int8_t tokenResult         = physicalResult & HSMLPF3_RETVAL_MASK;

    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = AESCMAC_STATUS_SUCCESS;
    }

    object->common.returnStatus = status;

    if ((status == AESCMAC_STATUS_ERROR) && (object->common.returnBehavior == AES_RETURN_BEHAVIOR_CALLBACK))
    {
        object->callbackFxn(handle, object->common.returnStatus, object->operation, object->operationType);
    }
}

/*
 *  ======== AESCMACLPF3HSM_freeAssetID ========
 */
static int_fast16_t AESCMACLPF3HSM_freeAssetID(AESCMAC_Handle handle, uint32_t AssetID)
{
    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    int_fast16_t status        = AESCMAC_STATUS_SUCCESS;
    int_fast16_t hsmRetval     = HSMLPF3_STATUS_ERROR;

    HSMLPF3_constructDeleteAssetToken(AssetID);

    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                    AESCMACLPF3HSM_FreeAssetPostProcessing,
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
 *  ======== AESCMACLPF3HSM_addData ========
 */
static int_fast16_t AESCMACLPF3HSM_addData(AESCMAC_Handle handle, AESCMAC_Operation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    int_fast16_t status        = AESCMAC_STATUS_ERROR;

    /* If the HSM IP and/or HSMSAL failed to boot then we cannot perform any HSM-related operation */
    if (object->hsmStatus == AESCMAC_STATUS_ERROR)
    {
        return AESCMAC_STATUS_ERROR;
    }

    /* Assert the segmented operation was setup */
    DebugP_assert((object->operationType == AESCMAC_OP_TYPE_SEGMENTED_SIGN) ||
                  (object->operationType == AESCMAC_OP_TYPE_SEGMENTED_VERIFY));

    /* Check for previous failure or cancellation of segmented operation */
    if (object->common.returnStatus != AESCMAC_STATUS_SUCCESS)
    {
        /* Return the status of the previous call.
         * The callback function will not be executed.
         */
        return object->common.returnStatus;
    }

    if ((operation->inputLength == 0U) || (!HSM_IS_SIZE_MULTIPLE_OF_AES_BLOCK_SIZE(operation->inputLength)))
    {
        return AESCMAC_STATUS_ERROR;
    }

    object->operation = operation;

    if (!HSMLPF3_acquireLock(object->common.semaphoreTimeout, (uintptr_t)handle))
    {
        return AESCMAC_STATUS_RESOURCE_UNAVAILABLE;
    }

    status = AESCMACLPF3HSM_performHSMOperation(handle);

    return status;
}

/*
 *  ======== AESCMACLPF3HSM_finalize ========
 */
static int_fast16_t AESCMACLPF3HSM_finalize(AESCMAC_Handle handle, AESCMAC_Operation *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    AESCMACLPF3_Object *object = AESCMACLPF3_getObject(handle);
    int_fast16_t status        = AESCMAC_STATUS_ERROR;

    if (object->hsmStatus == HSMLPF3_STATUS_ERROR)
    {
        return HSMLPF3_STATUS_ERROR;
    }

    /* Assert the segmented operation was setup */
    DebugP_assert((object->operationType == AESCMAC_OP_TYPE_SEGMENTED_SIGN) ||
                  (object->operationType == AESCMAC_OP_TYPE_SEGMENTED_VERIFY));

    /* Check for previous failure or cancellation of segmented operation */
    if (object->common.returnStatus != AESCMAC_STATUS_SUCCESS)
    {
        /* Return the status of the previous call.
         * The callback function will not be executed.
         */
        return object->common.returnStatus;
    }

    if (operation->inputLength == 0U)
    {
        /*
         * Finalizing an operation without providing data to process is not
         * supported. Return an error.
         */
        return AESCMAC_STATUS_ERROR;
    }

    object->operation = operation;

    if (object->operationType & AESCMAC_OP_FLAG_SIGN)
    {
        object->operationType = AESCMAC_OP_TYPE_FINALIZE_SIGN;
    }
    else
    {
        object->operationType = AESCMAC_OP_TYPE_FINALIZE_VERIFY;
    }

    if (!HSMLPF3_acquireLock(object->common.semaphoreTimeout, (uintptr_t)handle))
    {
        return AESCMAC_STATUS_RESOURCE_UNAVAILABLE;
    }

    status = AESCMACLPF3HSM_performHSMOperation(handle);

    return status;
}

#endif