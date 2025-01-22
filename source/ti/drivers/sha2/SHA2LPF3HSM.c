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

#include <string.h>
#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/power/PowerCC27XX.h>
#include <ti/drivers/cryptoutils/sharedresources/CryptoResourceLPF3.h>

#if (ENABLE_KEY_STORAGE == 1)
    #include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA.h>
    #include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA_helpers.h>
#endif

#include <third_party/hsmddk/include/Integration/Adapter_DriverInit/incl/api_driver_init.h>
#include <third_party/hsmddk/include/Integration/Adapter_VEX/incl/adapter_vex.h>
#include <third_party/hsmddk/include/Integration/HSMSAL/HSMSAL.h>
#include <third_party/hsmddk/include/Integration/Adapter_Generic/incl/adapter_interrupts.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_asset_policy.h>

#include <ti/drivers/cryptoutils/hsm/HSMLPF3.h>
#include <ti/drivers/sha2/SHA2LPF3HSM.h>
#include <ti/devices/DeviceFamily.h>

#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_hsmcrypto.h)

/* Max key length supported by HSM for HMAC- needed for KeyStore material retrieval */
#define SHA2LPF3HSM_MAX_HMAC_KEY_LENGTH_BYTES 128

/* Forward declarations */
static inline uint32_t SHA2LPF3HSM_largestBlockSizeMultiple(uint32_t length, uint32_t blockSize);

static void SHA2LPF3HSM_setIntermediateDigestLength(SHA2_HashType hashType, uint32_t *digestLength);

static void SHA2LPF3HSM_setAlgorithmAndDigestLength(SHA2LPF3HSM_Object *object);

static int_fast16_t SHA2LPF3HSM_computeIntermediateHash(SHA2_Handle handle);

static int_fast16_t SHA2LPF3HSM_addData(SHA2_Handle handle, const void *data, size_t length);

static int_fast16_t SHA2LPF3HSM_finalize(SHA2_Handle handle, void *digest);

static int_fast16_t SHA2LPF3HSM_hashData(SHA2_Handle handle, const void *data, size_t length, void *digest);

static void SHA2LPF3HSM_hashPostProcess(uintptr_t driverHandle);

static void SHA2LPF3HSM_finalizePostProcess(uintptr_t driverHandle);

static void SHA2LPF3HSM_addDataPostProcess(uintptr_t driverHandle);

static void SHA2LPF3HSM_intermediateHashPostProcess(uintptr_t driverHandle);

static int_fast16_t SHA2LPF3HSM_processOneStepAndFinalizeOperation(SHA2_Handle handle);

static int_fast16_t SHA2LPF3HSM_createAndLoadKeyAssetID(SHA2_Handle handle);
static int_fast16_t SHA2LPF3HSM_createKeyAsset(SHA2_Handle handle);
static int_fast16_t SHA2LPF3HSM_LoadKeyAsset(SHA2_Handle handle, uint8_t *key);
static int_fast16_t SHA2LPF3HSM_CreateTempAssetID(SHA2_Handle handle);
static int_fast16_t SHA2LPF3HSM_freeAllAssets(SHA2_Handle handle);
static int_fast16_t SHA2LPF3HSM_freeAssetID(SHA2_Handle handle, uint32_t AssetID);

/* This table converts from SHA2_HashType values to the corresponding block size. */
static const uint8_t blockSizeTable[] = {SHA2_BLOCK_SIZE_BYTES_224,
                                         SHA2_BLOCK_SIZE_BYTES_256,
                                         SHA2_BLOCK_SIZE_BYTES_384,
                                         SHA2_BLOCK_SIZE_BYTES_512};

static uint8_t *SHA2_data;

/* Tracks dataBytes that are used in intermediate hashes requiring two HSM operations */
static uint32_t SHA2_dataBytesRemaining;

static bool isInitialized = false;

/* Allows post-processing function to know transactionLength used in SHA2LPF3HSM_addData() */
static uint32_t addDataTransactionLength;

/*
 *  ======== SHA2LPF3HSM_largestBlockSizeMultiple ========
 */
static inline uint32_t SHA2LPF3HSM_largestBlockSizeMultiple(uint32_t length, uint32_t blockSize)
{
    return (length / blockSize) * blockSize;
}

static void SHA2LPF3HSM_setIntermediateDigestLength(SHA2_HashType hashType, uint32_t *digestLength)
{
    switch (hashType)
    {
        case SHA2_HASH_TYPE_224:
            /* Intermediate states for SHA-224 are 256 bits */
            *digestLength = SHA2_DIGEST_LENGTH_BYTES_256;
            break;
        case SHA2_HASH_TYPE_256:
            *digestLength = SHA2_DIGEST_LENGTH_BYTES_256;
            break;
        case SHA2_HASH_TYPE_384:
            /* Intermediate states for SHA-384 are 512 bits */
            *digestLength = SHA2_DIGEST_LENGTH_BYTES_512;
            break;
        case SHA2_HASH_TYPE_512:
            *digestLength = SHA2_DIGEST_LENGTH_BYTES_512;
            break;
        default:
            /* Do nothing. If this function is called, the
             * object's hash type is already confirmed to be
             * valid
             */
            break;
    }
}

/*
 *  ======== SHA2LPF3HSM_setAlgorithmAndDigestLength ========
 */
static void SHA2LPF3HSM_setAlgorithmAndDigestLength(SHA2LPF3HSM_Object *object)
{
    switch (object->hashType)
    {
        case SHA2_HASH_TYPE_224:
            object->algorithm    = VEXTOKEN_ALGO_HASH_SHA224;
            object->digestLength = SHA2_DIGEST_LENGTH_BYTES_224;
            break;
        case SHA2_HASH_TYPE_256:
            object->algorithm    = VEXTOKEN_ALGO_HASH_SHA256;
            object->digestLength = SHA2_DIGEST_LENGTH_BYTES_256;
            break;
        case SHA2_HASH_TYPE_384:
            object->algorithm    = VEXTOKEN_ALGO_HASH_SHA384;
            object->digestLength = SHA2_DIGEST_LENGTH_BYTES_384;
            break;
        case SHA2_HASH_TYPE_512:
            object->algorithm    = VEXTOKEN_ALGO_HASH_SHA512;
            object->digestLength = SHA2_DIGEST_LENGTH_BYTES_512;
            break;
        default:
            /* Do nothing. Valid hash_type will be checked before this function
             * is called.
             */
            break;
    }
}

/*
 *  ======== SHA2LPF3HSM_computeIntermediateHash ========
 */
static int_fast16_t SHA2LPF3HSM_computeIntermediateHash(SHA2_Handle handle)
{
    SHA2LPF3HSM_Object *object = handle->object;
    uint32_t blockSize         = blockSizeTable[object->hashType];
    uint32_t transactionLength = SHA2LPF3HSM_largestBlockSizeMultiple(SHA2_dataBytesRemaining, blockSize);
    int_fast16_t status        = SHA2_STATUS_ERROR;
    int_fast16_t hsmRetval     = HSMLPF3_STATUS_ERROR;

    /* Make sure to always leave some in buffer, so there's no empty-finalize case */
    if ((SHA2_dataBytesRemaining % blockSize) == 0)
    {
        transactionLength -= blockSize;
    }

    SHA2LPF3HSM_setAlgorithmAndDigestLength(object);

    /* Overwrite the intermediate digest lengths for the special cases of SHA-224 and SHA-384 */
    if (object->hashType == SHA2_HASH_TYPE_224)
    {
        object->digestLength = SHA2_DIGEST_LENGTH_BYTES_256;
    }
    else if (object->hashType == SHA2_HASH_TYPE_384)
    {
        object->digestLength = SHA2_DIGEST_LENGTH_BYTES_512;
    }

    object->input       = SHA2_data;
    object->inputLength = transactionLength;
    object->mode        = VEXTOKEN_MODE_HASH_MAC_CONT2CONT;

    /* Populates the HSMLPF3 commandToken as a hash token for a SHA2 operation. */
    HSMLPF3_constructSHA2PhysicalToken(object);

    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    SHA2LPF3HSM_intermediateHashPostProcess,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* operationInProgress is still true from the SHA2_addData() call */

        /* Handles post command token submission mechanism.
         * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers post-processing
         * fxn) and returns immediately when in callback mode.
         */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
        else
        {
            /* If there's an error waiting for the result, abort any post-processing and be sure to
             * release locks here. This can only happen in polling mode, so do not call the user's
             * callback.
             */
            object->operationInProgress = false;

            HSMLPF3_releaseLock();
        }
    }
    else
    {
        object->bytesInBuffer   = 0;
        object->bytesProcessed  = 0;
        SHA2_data               = NULL;
        SHA2_dataBytesRemaining = 0;

        /* The post-process function will not execute, so this execution path must
         * release the lock and power constraint itself
         */
        HSMLPF3_releaseLock();

        object->operationInProgress = false;

        /* If there is an HSM token error, clear internal buffers as the
         * segmented hash is now corrupted. Set operationInProgress to false
         * because we are no longer waiting for an HSM result. In this case,
         * we will call the application's callback with error status, because the
         * initial SHA2_addData() call was successful
         */
        if (object->returnBehavior == SHA2_RETURN_BEHAVIOR_CALLBACK)
        {
            object->callbackFxn(handle, SHA2_STATUS_ERROR);
        }
    }

    return status;
}

/*
 *  ======== SHA2_init ========
 */
void SHA2_init(void)
{
    HSMLPF3_constructRTOSObjects();

    isInitialized = true;
}

/*
 *  ======== SHA2_construct ========
 */
SHA2_Handle SHA2_construct(SHA2_Config *config, const SHA2_Params *params)
{
    SHA2_Handle handle;
    SHA2LPF3HSM_Object *object;
    uintptr_t key;

    handle = (SHA2_Config *)config;
    object = handle->object;

    key = HwiP_disable();

    if (object->isOpen || !isInitialized)
    {
        HwiP_restore(key);
        return NULL;
    }

    object->isOpen              = true;
    object->operationInProgress = false;

    HwiP_restore(key);

    if (params == NULL)
    {
        params = &SHA2_defaultParams;
    }

    /* Because there is no alternative hardware accelerator for SHA2 on CC27XX,
     * a NULL handle is returned when the HSM cannot boot successfully
     */
    if (HSMLPF3_init() != HSMLPF3_STATUS_SUCCESS)
    {
        return NULL;
    }

    object->bytesInBuffer  = 0;
    object->bytesProcessed = 0;
    object->returnBehavior = params->returnBehavior;
    object->callbackFxn    = params->callbackFxn;
    object->hashType       = params->hashType;
    object->keyAssetID     = 0U;
    object->tempAssetID    = 0U;

    if (params->returnBehavior == SHA2_RETURN_BEHAVIOR_BLOCKING)
    {
        object->accessTimeout = params->timeout;
    }
    else
    {
        object->accessTimeout = SemaphoreP_NO_WAIT;
    }

    return handle;
}

/*
 *  ======== SHA2_close ========
 */
void SHA2_close(SHA2_Handle handle)
{
    SHA2LPF3HSM_Object *object = handle->object;

    /* This is true only if in callback mode and waiting on a result */
    if (object->operationInProgress)
    {
        SHA2_cancelOperation(handle);
    }

    if (!HSMLPF3_acquireLock(object->accessTimeout, (uintptr_t)handle))
    {
        return;
    }

    /* SHA2_close() is a void function and cannot return the status of attempting to clean up.
     * The best the driver can do is attempt to remove all assets it created, if they weren't
     * already released like they should have been.
     */
    (void)SHA2LPF3HSM_freeAllAssets(handle);

    HSMLPF3_releaseLock();

    object->isOpen = false;
}

/*
 *  ======== SHA2_addData ========
 */
int_fast16_t SHA2_addData(SHA2_Handle handle, const void *data, size_t length)
{
    return SHA2LPF3HSM_addData(handle, data, length);
}

/*
 *  ======== SHA2LPF3HSM_addData ========
 */
static int_fast16_t SHA2LPF3HSM_addData(SHA2_Handle handle, const void *data, size_t length)
{
    SHA2LPF3HSM_Object *object = handle->object;
    uint32_t blockSize         = blockSizeTable[object->hashType];
    int_fast16_t status        = SHA2_STATUS_ERROR;
    int_fast16_t hsmRetval     = HSMLPF3_STATUS_ERROR;
    uint8_t *bufferTail;
    uint32_t transactionLength;
    uint32_t remainingDataLength;
    const uint8_t *transactionStartAddress;
    const uint8_t *remainingData;
    uint32_t bytesToCopyToBuffer;
    bool tokenSubmitted;
    uintptr_t key;

    /* Try and obtain access to the crypto module */
    if (!HSMLPF3_acquireLock(object->accessTimeout, (uintptr_t)handle))
    {
        return SHA2_STATUS_RESOURCE_UNAVAILABLE;
    }

    if (length == 0)
    {
        /* No operation, as no new data has been provided. Any cleanup or intermediate hashes
         * necessary would be completed by a prior call to SHA2LPF3HSM_addData. Set
         * tokenSubmitted to false so that we still trigger a callback.
         */
        tokenSubmitted = false;
    }
    else if ((object->bytesInBuffer + length) == blockSize)
    {
        /* We must make sure to always leave some data in buffer, so that there's
         * no empty-finalize case.
         */
        bufferTail = &object->buffer[object->bytesInBuffer];
        memcpy(bufferTail, data, length);
        object->bytesInBuffer += length;
        SHA2_dataBytesRemaining = 0;
        tokenSubmitted          = false;
    }
    else if ((object->bytesInBuffer + length) > blockSize)
    {
        /* We have accumulated enough data to start a transaction. Now the question
         * remains whether we have to merge bytes from the data stream into the
         * buffer first. If so, we do that now, then start a transaction.
         * If the buffer is empty, we can start a transaction on the data stream.
         * Once the transaction is finished, we will decide how to follow up,
         * i.e. copy remaining data into the buffer.
         */

        transactionLength = SHA2LPF3HSM_largestBlockSizeMultiple((object->bytesInBuffer + length), blockSize);
        if (transactionLength > DMA_MAX_TXN_LENGTH)
        {
            HSMLPF3_releaseLock();

            return SHA2_STATUS_DMA_ERROR;
        }

        if (object->bytesInBuffer > 0)
        {
            /* Copy to buffer so it has exactly the block size in it */
            bufferTail          = &object->buffer[object->bytesInBuffer];
            bytesToCopyToBuffer = blockSize - object->bytesInBuffer;
            memcpy(bufferTail, data, bytesToCopyToBuffer);

            /* The data in buffer right now is going to get consumed */
            object->bytesInBuffer = 0;

            /* Overwrite transactionLength to just one block's worth of data. Another token will be submitted
             * in SHA2LPF3HSM_computeIntermediateHash() if necessary. Data is consumed directly from the buffer
             * for the first token.
             */
            transactionLength       = blockSize;
            transactionStartAddress = object->buffer;

            /* This points to the data passed in to SHA2_addData() that isn't being consumed by the first
             * upcoming hash token. It may be used in an intermediate hash, and the remainder data will be
             * buffered.
             */
            SHA2_data               = (uint8_t *)data + bytesToCopyToBuffer;
            SHA2_dataBytesRemaining = length - bytesToCopyToBuffer;
        }
        else
        { /* The SHA2_addData() call solely provided more than a blockSize of data. Buffer any remainder */
            transactionStartAddress = data;

            /* Make sure to always leave some in buffer, so there's no empty-finalize case */
            if ((object->bytesInBuffer + length) % blockSize == 0)
            {
                transactionLength -= blockSize;
            }

            remainingData = (uint8_t *)data + transactionLength;

            remainingDataLength = length - transactionLength;

            /* Buffer any remaining data */
            if (remainingDataLength > 0)
            {
                memcpy(object->buffer, remainingData, remainingDataLength);
                object->bytesInBuffer = remainingDataLength;
            }
        }

        SHA2LPF3HSM_setAlgorithmAndDigestLength(object);

        /* Overwrite the intermediate digest lengths for the special cases of SHA-224 and SHA-384 */
        if (object->hashType == SHA2_HASH_TYPE_224)
        {
            object->digestLength = SHA2_DIGEST_LENGTH_BYTES_256;
        }
        else if (object->hashType == SHA2_HASH_TYPE_384)
        {
            object->digestLength = SHA2_DIGEST_LENGTH_BYTES_512;
        }

        object->input       = (uint8_t *)transactionStartAddress;
        object->inputLength = transactionLength;

        /* Finally we need to decide whether this is the first hash
         * operation or a follow-up from a previous one.
         */
        if (object->bytesProcessed > 0)
        {
            object->mode = VEXTOKEN_MODE_HASH_MAC_CONT2CONT;
        }
        else
        {
            object->mode = VEXTOKEN_MODE_HASH_MAC_INIT2CONT;
        }

        /* Populates the HSMLPF3 commandToken as a hash token for a SHA2 operation. */
        HSMLPF3_constructSHA2PhysicalToken(object);

        /* The postProcessFxn needs access to the transaction length
         * that was determined above
         */
        addDataTransactionLength = transactionLength;

        /* Starting the operation and setting object->operationInProgress must be atomic */
        key = HwiP_disable();

        hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                        SHA2LPF3HSM_addDataPostProcess,
                                        (uintptr_t)handle);

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            object->operationInProgress = true;

            HwiP_restore(key);

            tokenSubmitted = true;

            /* The return status is overwritten if this token submission
             * yields an error, or if SHA2LPF3HSM_computeIntermediateHash() gets
             * called and returns an error
             */

            /* Handles post command token submission mechanism.
             * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers
             * post-processing fxn) and returns immediately when in callback mode.
             */
            hsmRetval = HSMLPF3_waitForResult();

            if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
            {
                status = object->returnStatus;
            }
            else
            {
                /* If there's an error waiting for the result, abort any post-processing and be sure to
                 * release locks here. This can only happen in polling mode, so do not call the user's
                 * callback.
                 */

                object->operationInProgress = false;

                HSMLPF3_releaseLock();
            }
        }
        else
        {
            HwiP_restore(key);

            /* If there is an HSM token error, clear internal buffers as the
             * segmented hash is now corrupted
             */
            SHA2_reset(handle);

            tokenSubmitted = false;

            status = SHA2_STATUS_ERROR;
        }
    }
    else
    {
        /* No action required by the HSM. */
        bufferTail = &object->buffer[object->bytesInBuffer];
        memcpy(bufferTail, data, length);
        object->bytesInBuffer += length;
        SHA2_dataBytesRemaining = 0;
        status                  = SHA2_STATUS_SUCCESS;
        tokenSubmitted          = false;
    }

    if (!tokenSubmitted)
    {
        object->returnStatus = SHA2_STATUS_SUCCESS;

        status = SHA2_STATUS_SUCCESS;

        HSMLPF3_releaseLock();

        /* Since there's no HSM operation ongoing, we can call the application's
         * callback function now. Make sure not to call it if there was a
         * token submission error.
         */
        if (status == SHA2_STATUS_SUCCESS && object->returnBehavior == SHA2_RETURN_BEHAVIOR_CALLBACK)
        {
            object->callbackFxn(handle, status);
        }
    }

    return status;
}

/*
 *  ======== SHA2_finalize ========
 */
int_fast16_t SHA2_finalize(SHA2_Handle handle, void *digest)
{
    return SHA2LPF3HSM_finalize(handle, digest);
}

/*
 *  ======== SHA2LPF3HSM_finalize ========
 */
static int_fast16_t SHA2LPF3HSM_finalize(SHA2_Handle handle, void *digest)
{
    SHA2LPF3HSM_Object *object = handle->object;
    int_fast16_t status        = SHA2_STATUS_ERROR;
    int_fast16_t hsmRetval     = HSMLPF3_STATUS_ERROR;
    uintptr_t key;

    SHA2LPF3HSM_setAlgorithmAndDigestLength(object);

    /* Overwrite the intermediate digest lengths for the special cases of SHA-224 and SHA-384 */
    if (object->hashType == SHA2_HASH_TYPE_224)
    {
        object->digestLength = SHA2_DIGEST_LENGTH_BYTES_256;
    }
    else if (object->hashType == SHA2_HASH_TYPE_384)
    {
        object->digestLength = SHA2_DIGEST_LENGTH_BYTES_512;
    }

    object->input       = object->buffer;
    object->output      = digest;
    object->inputLength = object->bytesInBuffer;

    if (object->bytesProcessed == 0)
    {
        /* Since no hash operation has been performed yet and no intermediate
         * digest is available, we have to perform a full hash operation on the data in buffer.
         */
        object->mode            = (uint32_t)VEXTOKEN_MODE_HASH_MAC_INIT2FINAL;
        object->totalDataLength = (uint32_t)object->bytesInBuffer;
    }
    else if (object->bytesInBuffer > 0)
    {
        /* We've already performed a hash, so there's an intermediate digest, but there's also data in the buffer */
        object->mode            = (uint32_t)VEXTOKEN_MODE_HASH_MAC_CONT2FINAL;
        object->totalDataLength = ((uint32_t)object->bytesInBuffer) + object->bytesProcessed;
    }
    else
    {
        return SHA2_STATUS_ERROR;
    }

    /* Try and obtain access to the crypto module */
    if (!HSMLPF3_acquireLock(object->accessTimeout, (uintptr_t)handle))
    {
        return SHA2_STATUS_RESOURCE_UNAVAILABLE;
    }

    /* Populates the HSMLPF3 commandToken as a hash token for a SHA2 operation. */
    HSMLPF3_constructSHA2PhysicalToken(object);

    /* Revert the digest lengths for the special cases of SHA-224 and SHA-384.
     * We must do so after constructing the token, which involves copying over the
     * intermediate digest of modified length. However, reverting must come before
     * submitting the token, since the post-processing function will retrieve the
     * final digest of length matching the hashType.
     */
    if (object->hashType == SHA2_HASH_TYPE_224)
    {
        object->digestLength = SHA2_DIGEST_LENGTH_BYTES_224;
    }
    else if (object->hashType == SHA2_HASH_TYPE_384)
    {
        object->digestLength = SHA2_DIGEST_LENGTH_BYTES_384;
    }

    key = HwiP_disable();

    /* Exchange token to hash the remainder data */
    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    SHA2LPF3HSM_finalizePostProcess,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        object->operationInProgress = true;

        HwiP_restore(key);

        /* Handles post command token submission mechanism.
         * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers post-processing
         * fxn) and returns immediately when in callback mode.
         */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
        else
        {
            /* If there's an error waiting for the result, abort any post-processing and be sure to
             * release locks here. This can only happen in polling mode, so do not call the user's
             * callback.
             */
            HSMLPF3_releaseLock();
        }
    }
    else
    {
        HwiP_restore(key);

        status = SHA2_STATUS_ERROR;

        /* The application's callback is not called in this error case */

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== SHA2_hashData ========
 */
int_fast16_t SHA2_hashData(SHA2_Handle handle, const void *data, size_t dataLength, void *digest)
{
    return SHA2LPF3HSM_hashData(handle, data, dataLength, digest);
}

/*
 *  ======== SHA2LPF3HSM_hashData ========
 */
static int_fast16_t SHA2LPF3HSM_hashData(SHA2_Handle handle, const void *data, size_t dataLength, void *digest)
{
    SHA2LPF3HSM_Object *object = handle->object;
    int_fast16_t status        = SHA2_STATUS_ERROR;
    int_fast16_t hsmRetval     = HSMLPF3_STATUS_ERROR;
    uintptr_t key;

    if (dataLength > DMA_MAX_TXN_LENGTH)
    {
        return SHA2_STATUS_DMA_ERROR;
    }

    /* Try and obtain access to the crypto module */
    if (!HSMLPF3_acquireLock(object->accessTimeout, (uintptr_t)handle))
    {
        return SHA2_STATUS_RESOURCE_UNAVAILABLE;
    }

    SHA2LPF3HSM_setAlgorithmAndDigestLength(object);

    /* Calls to SHA2_hashData() clear intermediate data
     * from a previous partial operation started with
     * SHA2_addData()
     */
    object->bytesInBuffer   = 0;
    object->bytesProcessed  = 0;
    SHA2_data               = NULL;
    SHA2_dataBytesRemaining = 0;
    object->input           = (uint8_t *)data;
    object->output          = digest;
    object->inputLength     = dataLength;
    object->totalDataLength = object->inputLength;
    object->mode            = (uint32_t)VEXTOKEN_MODE_HASH_MAC_INIT2FINAL;
    object->key             = NULL;

    /* Populates the HSMLPF3 commandToken as a hash token for a SHA2 operation. */
    HSMLPF3_constructSHA2PhysicalToken(object);

    key = HwiP_disable();

    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    SHA2LPF3HSM_hashPostProcess,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        object->operationInProgress = true;

        HwiP_restore(key);

        /* Handles post command token submission mechanism.
         * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers post-processing
         * fxn) and returns immediately when in callback mode.
         */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
        else
        {
            HSMLPF3_releaseLock();
        }
    }
    else
    {
        HwiP_restore(key);

        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== SHA2_reset ========
 */
void SHA2_reset(SHA2_Handle handle)
{
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;

    /* This is only true when the handle is in callback mode, and the
     * result has not yet been received
     */
    if (object->operationInProgress == true)
    {
        SHA2_cancelOperation(handle);
    }

    /* Clear internal buffers relevant to a segmented hash from prior
     * SHA2_addData() calls. SHA2_hashData() also clears intermediate
     * data.
     */
    object->bytesInBuffer   = 0;
    object->bytesProcessed  = 0;
    SHA2_data               = NULL;
    SHA2_dataBytesRemaining = 0;
}

/*
 *  ======== SHA2_cancelOperation ========
 */
int_fast16_t SHA2_cancelOperation(SHA2_Handle handle)
{
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;

    object->bytesInBuffer   = 0;
    object->bytesProcessed  = 0;
    SHA2_data               = NULL;
    SHA2_dataBytesRemaining = 0;

    if (!object->operationInProgress)
    {
        /* Do nothing */
    }
    else
    {
        object->operationInProgress = false;

        /* Since the HSM cannot cancel an in-progress token, we must wait for the result to allow for
         * subsequent token submissions to succeed.
         */
        (void)HSMLPF3_cancelOperation();

        if (object->returnBehavior == SHA2_RETURN_BEHAVIOR_CALLBACK)
        {
            object->callbackFxn(handle, SHA2_STATUS_CANCELED);
        }
    }

    return SHA2_STATUS_SUCCESS;
}

/*
 *  ======== SHA2_setHashType ========
 */
int_fast16_t SHA2_setHashType(SHA2_Handle handle, SHA2_HashType type)
{
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    int_fast16_t status        = SHA2_STATUS_SUCCESS;

    if ((object->operationInProgress) || (object->bytesProcessed > 0))
    {
        status = SHA2_STATUS_ERROR;
    }
    else
    {
        object->hashType = type;
    }

    return status;
}

/*
 *  ======== SHA2_setupHmac ========
 */
int_fast16_t SHA2_setupHmac(SHA2_Handle handle, const CryptoKey *key)
{
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    int_fast16_t status        = SHA2_STATUS_ERROR;

    SHA2LPF3HSM_setAlgorithmAndDigestLength(object);

    if (object->hashType == SHA2_HASH_TYPE_224)
    {
        object->digestLength = SHA2_DIGEST_LENGTH_BYTES_256;
    }
    else if (object->hashType == SHA2_HASH_TYPE_384)
    {
        object->digestLength = SHA2_DIGEST_LENGTH_BYTES_512;
    }

    object->mode        = (uint32_t)VEXTOKEN_MODE_HASH_MAC_INIT2CONT;
    object->key         = (CryptoKey *)key;
    object->keyAssetID  = 0U;
    object->tempAssetID = 0U;

    if ((object->key->encoding == CryptoKey_PLAINTEXT_HSM) || (object->key->encoding == CryptoKey_KEYSTORE_HSM))
    {
        if (!HSMLPF3_acquireLock(object->accessTimeout, (uintptr_t)handle))
        {
            return SHA2_STATUS_RESOURCE_UNAVAILABLE;
        }

        status = SHA2LPF3HSM_createAndLoadKeyAssetID(handle);

        if (status == SHA2_STATUS_SUCCESS)
        {
            status = SHA2LPF3HSM_CreateTempAssetID(handle);
        }

        /* If an error occurred at any point AFTER successfully creating the key asset, then there
         * is an allocated asset that the driver must clean up.
         */
        if (status != SHA2_STATUS_SUCCESS)
        {
            (void)SHA2LPF3HSM_freeAllAssets(handle);
        }

        HSMLPF3_releaseLock();
    }

    if ((status == SHA2_STATUS_SUCCESS) && (object->returnBehavior == SHA2_RETURN_BEHAVIOR_CALLBACK))
    {
        object->callbackFxn(handle, object->returnStatus);
    }

    return status;
}

/*
 *  ======== SHA2_finalizeHmac ========
 */
int_fast16_t SHA2_finalizeHmac(SHA2_Handle handle, void *hmac)
{
    return SHA2LPF3HSM_finalize(handle, hmac);
}

/*
 *  ======== SHA2_hmac ========
 */
int_fast16_t SHA2_hmac(SHA2_Handle handle, const CryptoKey *key, const void *data, size_t size, void *hmac)
{
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    int_fast16_t status        = SHA2_STATUS_ERROR;

    SHA2LPF3HSM_setAlgorithmAndDigestLength(object);

    object->bytesInBuffer   = 0;
    object->bytesProcessed  = 0;
    SHA2_data               = NULL;
    SHA2_dataBytesRemaining = 0;
    object->input           = (uint8_t *)data;
    object->output          = hmac;
    object->inputLength     = size;
    object->totalDataLength = object->inputLength;
    object->mode            = (uint32_t)VEXTOKEN_MODE_HASH_MAC_INIT2FINAL;
    object->key             = (CryptoKey *)key;
    object->keyAssetID      = 0U;
    object->tempAssetID     = 0U;

    if ((object->key->encoding == CryptoKey_PLAINTEXT_HSM) || (object->key->encoding == CryptoKey_KEYSTORE_HSM))
    {
        if (!HSMLPF3_acquireLock(object->accessTimeout, (uintptr_t)handle))
        {
            return SHA2_STATUS_RESOURCE_UNAVAILABLE;
        }

        status = SHA2LPF3HSM_createAndLoadKeyAssetID(handle);

        if (status != SHA2_STATUS_SUCCESS)
        {
            /* If an error occurred at any point AFTER successfully creating the key asset, then there
             * is an allocated asset that the driver must clean up.
             */
            (void)SHA2LPF3HSM_freeAllAssets(handle);

            HSMLPF3_releaseLock();
        }
    }

    if (status == SHA2_STATUS_SUCCESS)
    {
        status = SHA2LPF3HSM_processOneStepAndFinalizeOperation(handle);
    }

    return status;
}

/*
 *  ======== SHA2LPF3HSM_oneStepAndFinalizePostProcessing ========
 */
static inline void SHA2LPF3HSM_oneStepAndFinalizePostProcessing(uintptr_t arg0)
{
    SHA2_Handle handle         = (SHA2_Handle)arg0;
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    int32_t physicalResult     = HSMLPF3_getResultCode();
    int_fast16_t status        = SHA2_STATUS_ERROR;

    if ((physicalResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = SHA2_STATUS_SUCCESS;

        HSMLPF3_getResultDigest(object->output, object->digestLength);
    }
    else
    {
        status = SHA2_STATUS_ERROR;
    }

    if (SHA2LPF3HSM_freeAllAssets(handle) != SHA2_STATUS_SUCCESS)
    {
        object->returnStatus = SHA2_STATUS_ERROR;
    }

    object->returnStatus = status;

    HSMLPF3_releaseLock();

    if (object->returnBehavior == SHA2_RETURN_BEHAVIOR_CALLBACK)
    {
        object->callbackFxn(handle, object->returnStatus);
    }
}

/*
 *  ======== SHA2LPF3HSM_processOneStepAndFinalizeOperation ========
 */
static int_fast16_t SHA2LPF3HSM_processOneStepAndFinalizeOperation(SHA2_Handle handle)
{
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    int_fast16_t status        = SHA2_STATUS_ERROR;

    (void)HSMLPF3_constructSHA2PhysicalToken(object);

    int_fast16_t hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                                 SHA2LPF3HSM_oneStepAndFinalizePostProcessing,
                                                 (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    if (hsmRetval != HSMLPF3_STATUS_SUCCESS)
    {
        HSMLPF3_releaseLock();
    }

    return status;
}

/*
 *  ======== SHA2LPF3HSM_hashPostProcess ========
 */
static void SHA2LPF3HSM_hashPostProcess(uintptr_t driverHandle)
{
    SHA2_Handle handle         = (SHA2_Handle)driverHandle;
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    int32_t physicalResult     = HSMLPF3_getResultCode();
    int_fast16_t status        = SHA2_STATUS_ERROR;

    /* tokenResult carries information regarding the operation result status as well as many other information such as
     * wether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((physicalResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = SHA2_STATUS_SUCCESS;

        HSMLPF3_getResultDigest(object->output, object->digestLength);
    }

    object->returnStatus = status;

    object->operationInProgress = false;

    HSMLPF3_releaseLock();

    if (object->returnBehavior == SHA2_RETURN_BEHAVIOR_CALLBACK)
    {
        object->callbackFxn(handle, object->returnStatus);
    }
}

/*
 *  ======== SHA2LPF3HSM_finalizePostProcess ========
 */
static void SHA2LPF3HSM_finalizePostProcess(uintptr_t driverHandle)
{
    SHA2_Handle handle         = (SHA2_Handle)driverHandle;
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    int32_t physicalResult     = HSMLPF3_getResultCode();

    /* tokenResult carries information regarding the operation result status as well as many other information such as
     * wether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((physicalResult & HSMLPF3_RETVAL_MASK) > 0)
    {
        object->returnStatus = SHA2_STATUS_ERROR;
    }
    else
    {
        object->returnStatus = SHA2_STATUS_SUCCESS;
        HSMLPF3_getResultDigest(object->output, object->digestLength);
    }

    /* The multi-step operation is now complete, so reset any
     * values tracking data from the operation.
     */
    object->bytesProcessed  = 0;
    object->bytesInBuffer   = 0;
    SHA2_data               = NULL;
    SHA2_dataBytesRemaining = 0;

    if (object->key)
    {
        if (SHA2LPF3HSM_freeAllAssets(handle) != SHA2_STATUS_SUCCESS)
        {
            object->returnStatus = SHA2_STATUS_ERROR;
        }
    }

    HSMLPF3_releaseLock();

    object->operationInProgress = false;

    if (object->returnBehavior == SHA2_RETURN_BEHAVIOR_CALLBACK)
    {
        object->callbackFxn(handle, object->returnStatus);
    }
}

/*
 *  ======== SHA2LPF3HSM_addDataPostProcess ========
 */
static void SHA2LPF3HSM_addDataPostProcess(uintptr_t driverHandle)
{
    SHA2_Handle handle         = (SHA2_Handle)driverHandle;
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    uint32_t blockSize         = blockSizeTable[object->hashType];
    uint32_t digestLength      = 0;
    int32_t physicalResult     = HSMLPF3_getResultCode();

    /* tokenResult carries information regarding the operation result status as well as many other information such as
     * wether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((physicalResult & HSMLPF3_RETVAL_MASK) > 0)
    {
        object->returnStatus = SHA2_STATUS_ERROR;

        SHA2_reset(handle);

        HSMLPF3_releaseLock();

        object->operationInProgress = false;

        if (object->returnBehavior == SHA2_RETURN_BEHAVIOR_CALLBACK)
        {
            object->callbackFxn(handle, object->returnStatus);
        }
    }
    else
    {
        object->returnStatus = SHA2_STATUS_SUCCESS;

        SHA2LPF3HSM_setIntermediateDigestLength(object->hashType, &digestLength);

        /* Copy intermediate state to object */
        HSMLPF3_getResultDigest(object->digest, digestLength);

        object->bytesProcessed += addDataTransactionLength;

        if (SHA2_dataBytesRemaining > blockSize)
        {
            /* The post-processing function for SHA2LPF3HSM_computeIntermediateHash will be responsible
             * for calling the application's callback function. If there is a token submission error on
             * the intermediate hash, then the user's callback will instead be called with status
             * SHA2_STATUS_ERROR.
             */
            object->returnStatus = SHA2LPF3HSM_computeIntermediateHash(handle);
        }
        else
        {
            if (SHA2_dataBytesRemaining > 0)
            {
                memcpy(object->buffer, SHA2_data, SHA2_dataBytesRemaining);
                object->bytesInBuffer = SHA2_dataBytesRemaining;
            }

            object->returnStatus = SHA2_STATUS_SUCCESS;

            HSMLPF3_releaseLock();

            object->operationInProgress = false;

            if (object->returnBehavior == SHA2_RETURN_BEHAVIOR_CALLBACK)
            {
                object->callbackFxn(handle, SHA2_STATUS_SUCCESS);
            }
        }
    }
}

/*
 *  ======== SHA2LPF3HSM_intermediateHashPostProcess ========
 */
static void SHA2LPF3HSM_intermediateHashPostProcess(uintptr_t driverHandle)
{
    SHA2_Handle handle         = (SHA2_Handle)driverHandle;
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    uint32_t blockSize         = blockSizeTable[object->hashType];
    uint32_t transactionLength = SHA2LPF3HSM_largestBlockSizeMultiple(SHA2_dataBytesRemaining, blockSize);
    uint32_t digestLength      = 0;
    int32_t physicalResult     = HSMLPF3_getResultCode();
    int_fast16_t status        = SHA2_STATUS_ERROR;

    /* tokenResult carries information regarding the operation result status as well as many other information such as
     * wether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((physicalResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        if ((SHA2_dataBytesRemaining % blockSize) == 0)
        {
            transactionLength -= blockSize;
        }

        SHA2LPF3HSM_setIntermediateDigestLength(object->hashType, &digestLength);

        /* Copy intermediate state to object */
        HSMLPF3_getResultDigest(object->digest, digestLength);

        object->bytesProcessed += transactionLength;

        /* Buffer any remaining data */
        if (SHA2_dataBytesRemaining - transactionLength > 0)
        {
            memcpy(object->buffer, SHA2_data + transactionLength, (SHA2_dataBytesRemaining - transactionLength));
            object->bytesInBuffer += (SHA2_dataBytesRemaining - transactionLength);
        }

        /* No more intermediate hash needed. */
        SHA2_dataBytesRemaining = 0;

        status = SHA2_STATUS_SUCCESS;
    }

    object->returnStatus = status;

    if (status == SHA2_STATUS_ERROR)
    {
        SHA2_reset(handle);
    }

    HSMLPF3_releaseLock();

    object->operationInProgress = false;

    if (object->returnBehavior == SHA2_RETURN_BEHAVIOR_CALLBACK)
    {
        object->callbackFxn(handle, object->returnStatus);
    }
}

/******************************************************************************/

/*
 *  ======== SHA2LPF3HSM_CreateKeyAssetPostProcessing ========
 */
static inline void SHA2LPF3HSM_CreateKeyAssetPostProcessing(uintptr_t arg0)
{
    SHA2_Handle handle         = (SHA2_Handle)arg0;
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    int_fast16_t status        = SHA2_STATUS_ERROR;
    int32_t tokenResult        = HSMLPF3_getResultCode();

    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->keyAssetID = HSMLPF3_getResultAssetID();
        status             = SHA2_STATUS_SUCCESS;
    }

    object->returnStatus = status;
}

/*
 *  ======== SHA2LPF3HSM_createKeyAsset ========
 */
static int_fast16_t SHA2LPF3HSM_createKeyAsset(SHA2_Handle handle)
{
    int_fast16_t status        = SHA2_STATUS_ERROR;
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    uint64_t assetPolicy       = 0x0;
    uint32_t keyLength         = 0U;

    if (object->key->encoding == CryptoKey_PLAINTEXT_HSM)
    {
        keyLength = object->key->u.plaintext.keyLength;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (object->key->encoding == CryptoKey_KEYSTORE_HSM)
    {
        keyLength = object->key->u.keyStore.keyLength;
    }
#endif

    /* Operation (Lower 16-bits + general Operation) + Direction. No Mode */
    assetPolicy = EIP130_ASSET_POLICY_SYM_MACHASH | EIP130_ASSET_POLICY_SCDIRENCGEN;

    switch (object->hashType)
    {
        case SHA2_HASH_TYPE_224:
            assetPolicy |= EIP130_ASSET_POLICY_SCAHSHA224;
            break;
        case SHA2_HASH_TYPE_256:
            assetPolicy |= EIP130_ASSET_POLICY_SCAHSHA256;
            break;
        case SHA2_HASH_TYPE_384:
            assetPolicy |= EIP130_ASSET_POLICY_SCAHSHA384;
            break;
        case SHA2_HASH_TYPE_512:
            assetPolicy |= EIP130_ASSET_POLICY_SCAHSHA512;
            break;
        default:
            /* Do nothing. Valid hash_type will be checked before this function
             * is called.
             */
            break;
    }

    HSMLPF3_constructCreateAssetToken(assetPolicy, keyLength);

    int_fast16_t hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                                 SHA2LPF3HSM_CreateKeyAssetPostProcessing,
                                                 (uintptr_t)handle);
    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    return status;
}

/*
 *  ======== SHA2LPF3HSM_LoadKeyAssetPostProcessing ========
 */
static inline void SHA2LPF3HSM_LoadKeyAssetPostProcessing(uintptr_t arg0)
{
    SHA2_Handle handle         = (SHA2_Handle)arg0;
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    int_fast16_t status        = SHA2_STATUS_ERROR;
    int32_t tokenResult        = HSMLPF3_getResultCode();

    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = SHA2_STATUS_SUCCESS;
    }

    object->returnStatus = status;
}

/*
 *  ======== SHA2LPF3HSM_LoadKeyAsset ========
 */
static int_fast16_t SHA2LPF3HSM_LoadKeyAsset(SHA2_Handle handle, uint8_t *key)
{
    int_fast16_t status        = SHA2_STATUS_ERROR;
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    uint32_t keyLength         = 0U;

    if (object->key->encoding == CryptoKey_PLAINTEXT_HSM)
    {
        keyLength = object->key->u.plaintext.keyLength;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (object->key->encoding == CryptoKey_KEYSTORE_HSM)
    {
        keyLength = object->key->u.keyStore.keyLength;
    }
#endif

    /* Constructing an HSM token is a void operation that cannot fail */
    (void)HSMLPF3_constructLoadPlaintextAssetToken(key, keyLength, object->keyAssetID);

    int_fast16_t hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                                 SHA2LPF3HSM_LoadKeyAssetPostProcessing,
                                                 (uintptr_t)handle);
    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    return status;
}

/*
 *  ======== SHA2LPF3HSM_createAndLoadKeyAssetID ========
 */
static int_fast16_t SHA2LPF3HSM_createAndLoadKeyAssetID(SHA2_Handle handle)
{
    int_fast16_t status        = SHA2_STATUS_ERROR;
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    uint8_t *keyMaterial       = NULL;
#if (ENABLE_KEY_STORAGE == 1)
    uint8_t KeyStore_keyingMaterial[SHA2LPF3HSM_MAX_HMAC_KEY_LENGTH_BYTES];
    KeyStore_PSA_KeyUsage usage = KEYSTORE_PSA_KEY_USAGE_SIGN_MESSAGE;
    KeyStore_PSA_Algorithm alg;

    switch (object->hashType)
    {
        case SHA2_HASH_TYPE_224:
            alg = KEYSTORE_PSA_ALG_HMAC(KEYSTORE_PSA_ALG_SHA_224);
            break;
        case SHA2_HASH_TYPE_256:
            alg = KEYSTORE_PSA_ALG_HMAC(KEYSTORE_PSA_ALG_SHA_256);
            break;
        case SHA2_HASH_TYPE_384:
            alg = KEYSTORE_PSA_ALG_HMAC(KEYSTORE_PSA_ALG_SHA_384);
            break;
        case SHA2_HASH_TYPE_512:
            alg = KEYSTORE_PSA_ALG_HMAC(KEYSTORE_PSA_ALG_SHA_512);
            break;
        default:
            return SHA2_STATUS_ERROR;
            break;
    }
#endif

    if (object->key->encoding == CryptoKey_PLAINTEXT_HSM)
    {
        keyMaterial = object->key->u.plaintext.keyMaterial;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (object->key->encoding == CryptoKey_KEYSTORE_HSM)
    {
        keyMaterial = &KeyStore_keyingMaterial[0];

        status = KeyStore_PSA_retrieveFromKeyStore(object->key,
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
        /* Invalid key encoding */
        return SHA2_STATUS_ERROR;
    }

    /* If we haven't already retrieved an asset directly from KeyStore, then the driver
     * will have to create and load an asset itself.
     */
    if (object->keyAssetID == 0)
    {
        status = SHA2LPF3HSM_createKeyAsset(handle);

        if (status == SHA2_STATUS_SUCCESS)
        {
            /* Now that the driver has successfully created an asset, object->keyAssetID is now non-zero.
             * If any failure condition happens after this moment, the cleanup will expect
             * object->driverCreatedKeyAsset to be accurate, since the keyAssetID will reflect that there
             * is an asset to free, and the cleanup will need to know how to do that.
             */
            object->driverCreatedKeyAsset = true;

            status = SHA2LPF3HSM_LoadKeyAsset(handle, keyMaterial);
        }
        else
        {
            /* object->keyAssetID is still 0, so cleanup knows there's no asset to free */
        }
    }

    return status;
}

/*
 *  ======== SHA2LPF3HSM_CreateTempAssetPostProcessing ========
 */
static inline void SHA2LPF3HSM_CreateTempAssetPostProcessing(uintptr_t arg0)
{
    SHA2_Handle handle         = (SHA2_Handle)arg0;
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    int_fast16_t status        = SHA2_STATUS_ERROR;
    int32_t tokenResult        = HSMLPF3_getResultCode();

    if (tokenResult == EIP130TOKEN_RESULT_SUCCESS)
    {
        object->tempAssetID = HSMLPF3_getResultAssetID();
        status              = SHA2_STATUS_SUCCESS;
    }

    object->returnStatus = status;
}

/*
 *  ======== SHA2LPF3HSM_CreateTempAssetID ========
 */
static int_fast16_t SHA2LPF3HSM_CreateTempAssetID(SHA2_Handle handle)
{
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    int_fast16_t status        = SHA2_STATUS_ERROR;
    uint64_t assetPolicy       = 0x0;

    /* Operation (Lower 16-bits + general Operation) + Direction. No Mode */
    assetPolicy = EIP130_ASSET_POLICY_SYM_TEMP | EIP130_ASSET_POLICY_SCUIMACHASH | EIP130_ASSET_POLICY_SCDIRENCGEN;

    switch (object->hashType)
    {
        case SHA2_HASH_TYPE_224:
            assetPolicy |= EIP130_ASSET_POLICY_SCAHSHA224;
            break;
        case SHA2_HASH_TYPE_256:
            assetPolicy |= EIP130_ASSET_POLICY_SCAHSHA256;
            break;
        case SHA2_HASH_TYPE_384:
            assetPolicy |= EIP130_ASSET_POLICY_SCAHSHA384;
            break;
        case SHA2_HASH_TYPE_512:
            assetPolicy |= EIP130_ASSET_POLICY_SCAHSHA512;
            break;
        default:
            /* Do nothing. Valid hash_type will be checked before this function
             * is called.
             */
            break;
    }

    HSMLPF3_constructCreateAssetToken(assetPolicy, object->digestLength);

    int_fast16_t hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                                 SHA2LPF3HSM_CreateTempAssetPostProcessing,
                                                 (uintptr_t)handle);
    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    return status;
}

/*
 *  ======== SHA2LPF3HSM_FreeAssetPostProcessing ========
 */
static inline void SHA2LPF3HSM_FreeAssetPostProcessing(uintptr_t arg0)
{
    SHA2_Handle handle         = (SHA2_Handle)arg0;
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    int_fast16_t status        = SHA2_STATUS_ERROR;
    int32_t physicalResult     = HSMLPF3_getResultCode();

    if ((physicalResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = SHA2_STATUS_SUCCESS;
    }

    object->returnStatus = status;

    if ((status == SHA2_STATUS_ERROR) && (object->returnBehavior == SHA2_RETURN_BEHAVIOR_CALLBACK))
    {
        object->callbackFxn(handle, object->returnStatus);
    }
}

/*
 *  ======== SHA2LPF3HSM_freeAssetID ========
 */
static int_fast16_t SHA2LPF3HSM_freeAssetID(SHA2_Handle handle, uint32_t AssetID)
{
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    int_fast16_t status        = SHA2_STATUS_SUCCESS;

    (void)HSMLPF3_constructDeleteAssetToken(AssetID);

    int_fast16_t hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING,
                                                 SHA2LPF3HSM_FreeAssetPostProcessing,
                                                 (uintptr_t)handle);
    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    return status;
}

/*
 *  ======== SHA2LPF3HSM_freeAllAssets ========
 */
static int_fast16_t SHA2LPF3HSM_freeAllAssets(SHA2_Handle handle)
{
    SHA2LPF3HSM_Object *object = (SHA2LPF3HSM_Object *)handle->object;
    int_fast16_t status        = SHA2_STATUS_ERROR;

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
            status = SHA2LPF3HSM_freeAssetID(handle, object->keyAssetID);
            if (status == SHA2_STATUS_SUCCESS)
            {
                object->keyAssetID = 0;
            }
        }
#if (ENABLE_KEY_STORAGE == 1)
        else
        {
            KeyStore_PSA_KeyFileId keyID;

            GET_KEY_ID(keyID, object->key->u.keyStore.keyID);

            status = KeyStore_PSA_assetPostProcessing(keyID);
            if (status == KEYSTORE_PSA_STATUS_SUCCESS)
            {
                object->keyAssetID = 0;
            }
        }
#endif
    }

    if (object->tempAssetID != 0)
    {
        status = SHA2LPF3HSM_freeAssetID(handle, object->tempAssetID);
        if (status == SHA2_STATUS_SUCCESS)
        {
            object->tempAssetID = 0;
        }
    }

    return status;
}
