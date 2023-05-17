/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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

#include <ti/drivers/aesecb/AESECBLPF3.h>
#include <ti/drivers/aesctr/AESCTRLPF3.h>
#include <ti/drivers/aesgcm/AESGCMLPF3.h>
#include <ti/drivers/AESCommon.h>
#include <ti/drivers/cryptoutils/aes/AESCommonLPF3.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKey.h>
#include <ti/drivers/cryptoutils/sharedresources/CryptoResourceLPF3.h>
#include <ti/drivers/cryptoutils/utils/CryptoUtils.h>

#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/aes.h)
#include DeviceFamily_constructPath(inc/hw_aes.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)

/*
 * Default AES CTR auto config used by GCM:
 *  ECB SRC as BUF
 *  Trigger points for auto ECB as RDTX3 and WRBUF3S
 *   (the first encryption starts by writing BUF3, the successive ones by reading TXT3)
 *  Counter size as 128-bits
 *  Counter endianness as Big Endian
 *  BUSHALT enabled
 */
#define AESGCMLPF3_DEFAULT_AUTOCFG                                                                                   \
    ((uint32_t)AES_AUTOCFG_ECBSRC_BUF | (uint32_t)AES_AUTOCFG_TRGECB_WRBUF3S | (uint32_t)AES_AUTOCFG_TRGECB_RDTXT3 | \
     (uint32_t)AES_AUTOCFG_CTRSIZE_CTR128 | (uint32_t)AES_AUTOCFG_CTRENDN_BIGENDIAN |                                \
     (uint32_t)AES_AUTOCFG_BUSHALT_EN)

/* Special element, R, for Galoi Field Multiplication R = 0xE1 */
#define GALOIS_FIELD_MULT_R 0xe1000000U

/* Mask used to extract the upper or lower 4-bits of a byte */
#define BYTE_MASK 0xf

/* Mask used to extract the upper or lower byte of a word */
#define WORD_MASK 0xff

/*
 * Shoup's method for multiplication uses precomputed table with
 *      last4[x] = x * P^128
 * where x and last4[x] are seen as elements of GF(2^128)
 */
/* clang-format off */
static const uint64_t last4[16] = {0x0000, 0x1c20, 0x3840, 0x2460, 0x7080, 0x6ca0, 0x48c0, 0x54e0,
                                   0xe100, 0xfd20, 0xd940, 0xc560, 0x9180, 0x8da0, 0xa9c0, 0xb5e0};
/* clang-format on */

/* Precomputed Hash Table Low */
uint64_t hashTableLow[AESGCMLPF3_HASH_PRECOMPUTE_TABLE_SIZE];
/* Precomputed Hash Table High */
uint64_t hashTableHigh[AESGCMLPF3_HASH_PRECOMPUTE_TABLE_SIZE];

/* Forward declarations */
static void AESGCMLPF3_initCounter(AESGCMLPF3_Object *object, const uint8_t initialCounter[AES_BLOCK_SIZE]);
static int_fast16_t AESGCMLPF3_oneStepOperation(AESGCM_Handle handle,
                                                AESGCM_OneStepOperation *operation,
                                                AESGCM_OperationType operationType);
static int_fast16_t AESGCMLPF3_startOperation(AESGCM_Handle handle, bool isOneStepOrFinalOperation);
static int_fast16_t AESGCMLPF3_waitForResult(AESGCM_Handle handle);
static int_fast16_t AESGCMLPF3_encryptOneAlignedAESBlockECB(AESCommonLPF3_Object *object,
                                                            const uint32_t *input,
                                                            uint32_t *output);
static void AESGCMLPF3_ghash(uint8_t *y, uint8_t *x, int32_t len);
static void AESGCMLPF3_xorByte(uint8_t *vector1, const uint8_t *vector2, size_t len);
static void AESGCMLPF3_xorWord(uint32_t *vector1, const uint32_t *vector2);
static void AESGCMLPF3_computeTag(AESGCM_Handle handle, bool isEncrypt);
static void AESGCMLPF3_lenToBits(uint32_t byteLen, uint8_t *bitLen);
static void AESGCMLPF3_precomputeGhashTable(AESGCM_Handle handle);
static void AESGCMLPF3_galoisMult(uint8_t x[16]);

/*
 *  ======== AESGCMLPF3_getObject ========
 */
static inline AESGCMLPF3_Object *AESGCMLPF3_getObject(AESGCM_Handle handle)
{
    AESGCMLPF3_Object *object = (AESGCMLPF3_Object *)handle->object;
    DebugP_assert(object);

    return object;
}

/*
 *  ======== AESGCM_init ========
 */
void AESGCM_init(void)
{
    AESCommonLPF3_init();
}

/*
 *  ======== AESGCM_construct ========
 */
AESGCM_Handle AESGCM_construct(AESGCM_Config *config, const AESGCM_Params *params)
{
    DebugP_assert(config);

    int_fast16_t status;
    AESGCM_Handle handle      = config;
    AESGCMLPF3_Object *object = AESGCMLPF3_getObject(handle);

    /* If params are NULL, use defaults */
    if (params == NULL)
    {
        params = &AESGCM_defaultParams;
    }

    DebugP_assert((params->returnBehavior != AESGCM_RETURN_BEHAVIOR_CALLBACK) || (params->callbackFxn != NULL));

    object->threadSafe = true;

    status = AESCommonLPF3_construct(&object->common, (AES_ReturnBehavior)params->returnBehavior, params->timeout);

    if (status != AES_STATUS_SUCCESS)
    {
        handle = NULL;
    }

    return handle;
}

/*
 *  ======== AESGCM_close ========
 */
void AESGCM_close(AESGCM_Handle handle)
{
    DebugP_assert(handle);

    AESGCMLPF3_Object *object = AESGCMLPF3_getObject(handle);

    AESCommonLPF3_close(&object->common);
}

/*
 *  ======== AESGCM_oneStepEncrypt ========
 */
int_fast16_t AESGCM_oneStepEncrypt(AESGCM_Handle handle, AESGCM_OneStepOperation *operationStruct)
{
    return AESGCMLPF3_oneStepOperation(handle, operationStruct, AESGCM_OPERATION_TYPE_ENCRYPT);
}

/*
 *  ======== AESGCM_oneStepDecrypt ========
 */
int_fast16_t AESGCM_oneStepDecrypt(AESGCM_Handle handle, AESGCM_OneStepOperation *operationStruct)
{
    return AESGCMLPF3_oneStepOperation(handle, operationStruct, AESGCM_OPERATION_TYPE_DECRYPT);
}

/*
 *  ======== AESGCMLPF3_oneStepOperation ========
 */
static int_fast16_t AESGCMLPF3_oneStepOperation(AESGCM_Handle handle,
                                                AESGCM_OneStepOperation *operation,
                                                AESGCM_OperationType operationType)
{
    DebugP_assert(handle);
    DebugP_assert(operation);
    DebugP_assert(operation->key);
    /* No need to assert operationType since we control it within the driver */

    AESGCMLPF3_Object *object = AESGCMLPF3_getObject(handle);
    int_fast16_t status;
    bool tagValid;

    /* Only polling mode supported for GCM on LPF3 devices */
    if (object->common.returnBehavior != AES_RETURN_BEHAVIOR_POLLING)
    {
        return AESGCM_STATUS_FEATURE_NOT_SUPPORTED;
    }

#if (AESCommonLPF3_UNALIGNED_IO_SUPPORT_ENABLE == 0)
    /* Check word-alignment of input & output pointers */
    if (!IS_WORD_ALIGNED(operation->input) || !IS_WORD_ALIGNED(operation->output))
    {
        return AESGCM_STATUS_UNALIGNED_IO_NOT_SUPPORTED;
    }
#endif

    /* Verify input + AAD length is non-zero */
    if ((operation->inputLength + operation->aadLength) == 0U)
    {
        return AESGCM_STATUS_ERROR;
    }

    /* The only currently supported IV length is 12 bytes */
    if (operation->ivLength != AESGCMLPF3_IV_LENGTH)
    {
        return AESGCM_STATUS_ERROR;
    }

    /*
     * Check if there is no operation already in progress for this driver
     * instance, and then mark the current operation to be in progress.
     */
    status = AESCommonLPF3_setOperationInProgress(&object->common);

    if (status != AESGCM_STATUS_SUCCESS)
    {
        return status;
    }

    if (object->threadSafe)
    {
        if (!CryptoResourceLPF3_acquireLock(object->common.semaphoreTimeout))
        {
            AESCommonLPF3_clearOperationInProgress(&object->common);
            return AESGCM_STATUS_RESOURCE_UNAVAILABLE;
        }

        object->common.cryptoResourceLocked = true;
    }

    object->operation           = (AESGCM_OperationUnion *)operation;
    object->operationType       = operationType;
    /* We will only change the returnStatus if there is an error or cancellation */
    object->common.returnStatus = AESGCM_STATUS_SUCCESS;

    /* Make internal copy of operational params */
    object->common.key           = *(operation->key);
    object->input                = operation->input;
    object->inputLength          = operation->inputLength;
    object->inputLengthRemaining = operation->inputLength;
    object->output               = operation->output;
    object->mac                  = operation->mac;
    object->aad                  = operation->aad;

    /* Compute Hash Key H by initializing counter to zeros */
    AESGCMLPF3_initCounter(object, NULL);
    status = AESGCMLPF3_encryptOneAlignedAESBlockECB(&object->common,
                                                     (const uint32_t *)&object->counter,
                                                     (uint32_t *)&object->hashKey);

    AESGCMLPF3_precomputeGhashTable(handle);

    if (status != AESGCM_STATUS_SUCCESS)
    {
        return status;
    }

    /* Construct the word aligned counter block Y0: IV || 1 */
    AESGCMLPF3_initCounter(object, operation->iv);
    object->counter[3] |= 1 << 24;

    /* Compute tag OPT: ENC(key, Y0) */
    status = AESGCMLPF3_encryptOneAlignedAESBlockECB(&object->common,
                                                     (const uint32_t *)&object->counter,
                                                     (uint32_t *)&object->tagOTP);

    if (status != AESGCM_STATUS_SUCCESS)
    {
        return status;
    }

    /* Compute Y1: Y0 + 1 used for GCTR operation*/
    object->counter[3] = object->counter[3] << 1;

    /* Clear intermediate tag */
    memset(object->intermediateTag, 0, sizeof(object->intermediateTag));

    if (operationType == AESGCM_OPERATION_TYPE_ENCRYPT)
    {
        /* Compute Ciphertext */
        status = AESGCMLPF3_startOperation(handle, true);

        /* Compute Authentication tag T */
        AESGCMLPF3_computeTag(handle, true);

        /* Copy computed tag to operations struct */
        memcpy(operation->mac, object->intermediateTag, object->operation->oneStepOperation.macLength);
    }
    else
    {
        /* Compute Authentication tag T' */
        AESGCMLPF3_computeTag(handle, false);

        /* Verify if computed T' matches T in operations struct */
        tagValid = CryptoUtils_buffersMatch(object->intermediateTag,
                                            object->mac,
                                            (size_t)object->operation->oneStepOperation.macLength);

        if (!tagValid)
        {
            return AESGCM_STATUS_MAC_INVALID;
        }
        /* Compute Plaintext if tags match*/
        status = AESGCMLPF3_startOperation(handle, true);
    }

    return status;
}

/*
 *  ======== AESGCMLPF3_computeTag ========
 */
static void AESGCMLPF3_computeTag(AESGCM_Handle handle, bool isEncrypt)
{
    AESGCMLPF3_Object *object = AESGCMLPF3_getObject(handle);

    /* Compute GF128-multiplication over AAD */
    if (object->operation->oneStepOperation.aadLength)
    {
        AESGCMLPF3_ghash(object->intermediateTag, object->aad, object->operation->oneStepOperation.aadLength);
    }

    /* Compute GF128-multiplication over ciphertext: input for dec and output for enc */
    if (object->operation->oneStepOperation.inputLength)
    {
        if (isEncrypt)
        {
            AESGCMLPF3_ghash(object->intermediateTag, object->output, object->operation->oneStepOperation.inputLength);
        }
        else
        {
            AESGCMLPF3_ghash(object->intermediateTag,
                             (uint8_t *)object->input,
                             object->operation->oneStepOperation.inputLength);
        }
    }

    /* Compute GF128-multiplication over concatenated bit length of input and AAD */
    uint8_t concatMessageLen[AES_BLOCK_SIZE];

    AESGCMLPF3_lenToBits(object->operation->oneStepOperation.aadLength, concatMessageLen);
    AESGCMLPF3_lenToBits(object->operation->oneStepOperation.inputLength, concatMessageLen + (AES_BLOCK_SIZE / 2));

    AESGCMLPF3_ghash(object->intermediateTag, concatMessageLen, AES_BLOCK_SIZE);

    /* Post processing: XOR above GF128-mult output with Tag OTP: E(K, Y0) */
    /* Check word-alignment of XOR input pointers */
    if (!IS_WORD_ALIGNED(object->intermediateTag) || !IS_WORD_ALIGNED(object->tagOTP))
    {
        AESGCMLPF3_xorByte((uint8_t *)&object->intermediateTag, (uint8_t *)&object->tagOTP, AES_BLOCK_SIZE);
    }
    else
    {
        AESGCMLPF3_xorWord((uint32_t *)&object->intermediateTag, (uint32_t *)&object->tagOTP);
    }
}

/*
 *  ======== AESGCMLPF3_lenToBits ========
 */
static void AESGCMLPF3_lenToBits(uint32_t byteLen, uint8_t *bitLen)
{
    int8_t i;

    /* Length is converted from bytes to bits */
    byteLen *= 8;
    for (i = 3; i >= 0; i--)
    {
        bitLen[i]     = 0;
        bitLen[i + 4] = byteLen & 0xFF;
        byteLen >>= 8;
    }
}

/*
 *  ======== AESGCMLPF3_encryptOneAlignedAESBlockECB ========
 */
static int_fast16_t AESGCMLPF3_encryptOneAlignedAESBlockECB(AESCommonLPF3_Object *object,
                                                            const uint32_t *input,
                                                            uint32_t *output)
{
    int_fast16_t status;

    /* Set up the key and AES engine to begin an operation */
    AESCommonLPF3_setupOperation(&object->key, AESEBCLPF3_SINGLE_BLOCK_AUTOCFG);

    /* Process the single block with CPU R/W */
    AESProcessAlignedBlocksECB(input, output, (uint32_t)AES_GET_NUM_BLOCKS(AES_BLOCK_SIZE));

    /*
     * Save the returnStatus prior clearing operationInProgress or
     * releasing the access semaphore in case it's overwritten.
     */
    status = object->returnStatus;
    AESCommonLPF3_clearOperationInProgress(object);
    AESCommonLPF3_cleanup(object);

    return status;
}

/*
 *  ======== AESGCMLPF3_initCounter ========
 */
static void AESGCMLPF3_initCounter(AESGCMLPF3_Object *object, const uint8_t initialCounter[AES_BLOCK_SIZE])
{
    if (initialCounter != NULL)
    {
        /* Reset object's counter to zero */
        memset(object->counter, 0, sizeof(object->counter));

        /* Intiialize with the provided IV */
        memcpy(object->counter, initialCounter, object->operation->oneStepOperation.ivLength);
    }
    else
    {
        memset(object->counter, 0, sizeof(object->counter));
    }
}

/*
 *  ======== AESGCMLPF3_startOperation ========
 */
static int_fast16_t AESGCMLPF3_startOperation(AESGCM_Handle handle, bool isOneStepOrFinalOperation)
{
    AESGCMLPF3_Object *object = AESGCMLPF3_getObject(handle);
    int_fast16_t status       = AESGCM_STATUS_SUCCESS;

    /* Set up the key and AES engine to begin an operation */
    AESCommonLPF3_setupOperation(&object->common.key, AESGCMLPF3_DEFAULT_AUTOCFG);

    /* Process all data as a polling mode operation */
    /* Write the counter value to the AES engine to trigger first encryption */
    AESCTRLPF3_writeCounter((uint32_t *)&object->counter[0]);

    /* Process all blocks with CPU R/W */
    AESCTRLPF3_processData(object->input, object->output, object->inputLength, isOneStepOrFinalOperation);

    object->inputLengthRemaining = 0U;

    status = AESGCMLPF3_waitForResult(handle);

    return status;
}

/*
 *  ======== AESGCMLPF3_waitForResult ========
 */
static int_fast16_t AESGCMLPF3_waitForResult(AESGCM_Handle handle)
{
    AESGCMLPF3_Object *object = AESGCMLPF3_getObject(handle);
    int_fast16_t status       = AESGCM_STATUS_ERROR;

    /* Save the last counter value from the AES engine */
    AESCTRLPF3_readCounter((uint32_t *)&object->counter[0]);

    /*
     * Save the object's returnStatus before clearing operationInProgress or
     * posting the access semaphore in case it is overwritten.
     */
    status = object->common.returnStatus;

    /* One-step or finalization operation is complete */
    AESCommonLPF3_clearOperationInProgress(&object->common);

    AESCommonLPF3_cleanup(&object->common);

    return status;
}

/*
 *  ======== AESGCM_cancelOperation ========
 */
int_fast16_t AESGCM_cancelOperation(AESGCM_Handle handle)
{
    DebugP_assert(handle);

    AESGCMLPF3_Object *object = AESGCMLPF3_getObject(handle);

    uintptr_t interruptKey = HwiP_disable();

    /*
     * Return success if there is no active operation to cancel.
     * Do not execute the callback as it would have been executed already
     * when the operation completed.
     */
    if (!object->common.operationInProgress)
    {
        HwiP_restore(interruptKey);
        return AESGCM_STATUS_SUCCESS;
    }

    HwiP_restore(interruptKey);

    /*
     * Cancel DMA for input and output channels, clear operation in-progress,
     * and releases crypto resource if necessary.
     */
    AESCommonLPF3_cancelOperation(&object->common, true);

    return AESGCM_STATUS_SUCCESS;
}

/* XOR a vector into another vector. */
/*
 *  ======== AESGCMLPF3_xorByte ========
 */
static void AESGCMLPF3_xorByte(uint8_t *vector1, const uint8_t *vector2, size_t len)
{
    uint_fast8_t i;

    for (i = 0; i < len; i++)
    {
        vector1[i] ^= vector2[i];
    }
}

/* XOR a word aligned vector into another word aligned vector. */
/*
 *  ======== AESGCMLPF3_xorWord ========
 */
static void AESGCMLPF3_xorWord(uint32_t *vector1, const uint32_t *vector2)
{
    uint32_t i;

    size_t lenWords = AES_BLOCK_SIZE / 4;

    for (i = 0; i < lenWords; i++)
    {
        vector1[i] ^= vector2[i];
    }
}

/* GHASH Function */
/*
 *  ======== AESGCMLPF3_ghash ========
 */
static void AESGCMLPF3_ghash(uint8_t *y, uint8_t *x, int32_t len)
{
    for (; len > 0; len -= AES_BLOCK_SIZE)
    {
        if (len >= AES_BLOCK_SIZE)
        {
            /* Check word-alignment of XOR input pointers */
            if (!IS_WORD_ALIGNED(y) || !IS_WORD_ALIGNED(x))
            {
                AESGCMLPF3_xorByte(y, x, len);
            }
            else
            {
                AESGCMLPF3_xorWord((uint32_t *)y, (uint32_t *)x);
            }
        }
        else
        {
            AESGCMLPF3_xorByte(y, x, len);
        }

        AESGCMLPF3_galoisMult(y);

        x += AES_BLOCK_SIZE;
    }
}

/*
 * Return the unsigned 32 bits integer corresponding to four bytes in
 * big-endian order (MSB first).
 */
/*
 *  ======== AESGCMLPF3_getUint32BE ========
 */
static uint32_t AESGCMLPF3_getUint32BE(uint8_t *src)
{
    uint8_t *buf;
    uint32_t result;

    buf    = src;
    result = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];

    return result;
}

/* Store a 32 bits unsigned integer in big-endian order in memory. */
/*
 *  ======== AESGCMLPF3_storeUint32InArray ========
 */
static void AESGCMLPF3_storeUint32InArray(uint8_t *dst, uint32_t x)
{
    uint8_t *buf;

    buf    = dst;
    buf[0] = (uint8_t)(x >> 24) & WORD_MASK;
    buf[1] = (uint8_t)(x >> 16) & WORD_MASK;
    buf[2] = (uint8_t)(x >> 8) & WORD_MASK;
    buf[3] = (uint8_t)x;
}

/* Pre-compute Shoup's 4-bit tables */
/*
 *  ======== AESGCMLPF3_precomputeGhashTable ========
 */
static void AESGCMLPF3_precomputeGhashTable(AESGCM_Handle handle)
{
    AESGCMLPF3_Object *object = AESGCMLPF3_getObject(handle);

    int8_t i;
    uint_fast8_t j;
    uint64_t upperWord, lowerWord;
    uint64_t vLow, vHigh;
    uint32_t tmp;

    /* Pack hashKey as two 64-bit big-endian variables in vHigh and vLow */
    upperWord = AESGCMLPF3_getUint32BE((uint8_t *)object->hashKey);
    lowerWord = AESGCMLPF3_getUint32BE((uint8_t *)object->hashKey + 4);
    vHigh     = (uint64_t)upperWord << 32 | lowerWord;

    upperWord = AESGCMLPF3_getUint32BE((uint8_t *)object->hashKey + 8);
    lowerWord = AESGCMLPF3_getUint32BE((uint8_t *)object->hashKey + 12);
    vLow      = (uint64_t)upperWord << 32 | lowerWord;

    /* 8th table entry is the hash key */
    hashTableLow[8]  = vLow;
    hashTableHigh[8] = vHigh;

    /* 1st table entry is all zeros */
    hashTableLow[0]  = 0;
    hashTableHigh[0] = 0;

    /* Compute table indices that are powers of two as product of H * special element */
    for (i = 4U; i > 0; i >>= 1U)
    {
        tmp   = (vLow & 1) * GALOIS_FIELD_MULT_R;
        vLow  = (vHigh << 63) | (vLow >> 1);
        vHigh = (vHigh >> 1) ^ ((uint64_t)tmp << 32);

        hashTableLow[i]  = vLow;
        hashTableHigh[i] = vHigh;
    }

    /* Compute the other table indices as sum of previous table entries */
    for (i = 2U; i <= 8U; i *= 2U)
    {
        for (j = 1U; j < i; j++)
        {
            hashTableHigh[i + j] = hashTableHigh[i] ^ hashTableHigh[j];
            hashTableLow[i + j]  = hashTableLow[i] ^ hashTableLow[j];
        }
    }
}

/* x = x.H, computed using the precomputed tables in object */
/*
 *  ======== AESGCMLPF3_galoisMult ========
 */
static void AESGCMLPF3_galoisMult(uint8_t *x)
{
    int8_t i;
    uint_fast8_t lower4Bits, upper4Bits, rem;
    uint64_t zHigh, zLow;

    lower4Bits = x[15] & BYTE_MASK;

    zHigh = hashTableHigh[lower4Bits];
    zLow  = hashTableLow[lower4Bits];

    for (i = 15U; i >= 0; i--)
    {
        lower4Bits = x[i] & BYTE_MASK;
        upper4Bits = (x[i] >> 4) & BYTE_MASK;

        if (i != 15)
        {
            rem   = (uint8_t)zLow & BYTE_MASK;
            zLow  = (zHigh << 60) | (zLow >> 4);
            zHigh = (zHigh >> 4);
            zHigh ^= (uint64_t)last4[rem] << 48;
            zHigh ^= hashTableHigh[lower4Bits];
            zLow ^= hashTableLow[lower4Bits];
        }

        rem   = (uint8_t)zLow & BYTE_MASK;
        zLow  = (zHigh << 60) | (zLow >> 4);
        zHigh = (zHigh >> 4);
        zHigh ^= (uint64_t)last4[rem] << 48;
        zHigh ^= hashTableHigh[upper4Bits];
        zLow ^= hashTableLow[upper4Bits];
    }

    /* Copy multiplication result to x in array format */
    AESGCMLPF3_storeUint32InArray(x, zHigh >> 32);
    AESGCMLPF3_storeUint32InArray(x + 4, zHigh);
    AESGCMLPF3_storeUint32InArray(x + 8, zLow >> 32);
    AESGCMLPF3_storeUint32InArray(x + 12, zLow);
}

/*
 *  ======== AESGCM_setupEncrypt ========
 */
int_fast16_t AESGCM_setupEncrypt(AESGCM_Handle handle,
                                 const CryptoKey *key,
                                 size_t totalAADLength,
                                 size_t totalPlaintextLength)
{
    /* Segmented operations are not currently supported by this implementation */
    return AESGCM_STATUS_FEATURE_NOT_SUPPORTED;
}

/*
 *  ======== AESGCM_setupDecrypt ========
 */
int_fast16_t AESGCM_setupDecrypt(AESGCM_Handle handle,
                                 const CryptoKey *key,
                                 size_t totalAADLength,
                                 size_t totalPlaintextLength)
{
    /* Segmented operations are not currently supported by this implementation */
    return AESGCM_STATUS_FEATURE_NOT_SUPPORTED;
}

/*
 *  ======== AESGCM_setLengths ========
 */
int_fast16_t AESGCM_setLengths(AESGCM_Handle handle, size_t aadLength, size_t plaintextLength)
{
    /* Segmented operations are not currently supported by this implementation */
    return AESGCM_STATUS_FEATURE_NOT_SUPPORTED;
}

/*
 *  ======== AESGCM_setIV ========
 */
int_fast16_t AESGCM_setIV(AESGCM_Handle handle, const uint8_t *iv, size_t ivLength)
{
    /* Segmented operations are not currently supported by this implementation */
    return AESGCM_STATUS_FEATURE_NOT_SUPPORTED;
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
    /* Segmented operations are not currently supported by this implementation */
    return AESGCM_STATUS_FEATURE_NOT_SUPPORTED;
}

/*
 *  ======== AESGCM_addData ========
 */
int_fast16_t AESGCM_addData(AESGCM_Handle handle, AESGCM_SegmentedDataOperation *operation)
{
    /* Segmented operations are not currently supported by this implementation */
    return AESGCM_STATUS_FEATURE_NOT_SUPPORTED;
}

/*
 *  ======== AESGCM_finalizeEncrypt ========
 */
int_fast16_t AESGCM_finalizeEncrypt(AESGCM_Handle handle, AESGCM_SegmentedFinalizeOperation *operation)
{
    /* Segmented operations are not currently supported by this implementation */
    return AESGCM_STATUS_FEATURE_NOT_SUPPORTED;
}

/*
 *  ======== AESGCM_finalizeDecrypt ========
 */
int_fast16_t AESGCM_finalizeDecrypt(AESGCM_Handle handle, AESGCM_SegmentedFinalizeOperation *operation)
{
    /* Segmented operations are not currently supported by this implementation */
    return AESGCM_STATUS_FEATURE_NOT_SUPPORTED;
}
