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

#include <ti/drivers/aesgcm/AESGCMLPF3.h>
#include <ti/drivers/aesecb/AESECBLPF3.h>
#include <ti/drivers/aesctr/AESCTRLPF3.h>
#include <ti/drivers/AESCommon.h>
#include <ti/drivers/cryptoutils/aes/AESCommonLPF3.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKey.h>
#include <ti/drivers/cryptoutils/sharedresources/CryptoResourceLPF3.h>
#include <ti/drivers/cryptoutils/utils/CryptoUtils.h>

#include <ti/drivers/dpl/DebugP.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/aes.h)
#include DeviceFamily_constructPath(inc/hw_aes.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)

/* Macro to byte swap a word */
#if (defined(__IAR_SYSTEMS_ICC__) || defined(__TI_COMPILER_VERSION__))
    #include <arm_acle.h>
    #define BSWAP32 __rev
#elif defined(__GNUC__)
    #define BSWAP32 __builtin_bswap32
#else
    #error Unsupported compiler
#endif

/* Default AES CTR auto config used by GCM:
 *  ECB SRC as BUF
 *  Trigger points for auto ECB as RDTX3 and WRBUF3S
 *   (the first encryption starts by writing BUF3, the successive ones by reading TXT3)
 *  Counter size as 128-bits
 *  Counter endianness as Big Endian
 *  BUSHALT enabled
 */
#define AESGCMLPF3_DEFAULT_AUTOCFG                                                                                   \
    ((uint32_t)AES_AUTOCFG_AESSRC_BUF | (uint32_t)AES_AUTOCFG_TRGAES_WRBUF3S | (uint32_t)AES_AUTOCFG_TRGAES_RDTXT3 | \
     (uint32_t)AES_AUTOCFG_CTRSIZE_CTR128 | (uint32_t)AES_AUTOCFG_CTRENDN_BIGENDIAN |                                \
     (uint32_t)AES_AUTOCFG_BUSHALT_EN)

/* Special element, R, for Galoi Field Multiplication R = 0xE1 */
#define GALOIS_FIELD_MULT_R 0xe1000000U

/* Mask used to extract the upper or lower 4-bits of a byte */
#define BYTE_MASK 0xFU

/* Mask used to extract the upper or lower byte of a word */
#define WORD_MASK 0xFFU

/* Number of 16-byte entries in the pre-computed table for each key in GHASH operation */
#define AESGCMLPF3_HASH_PRECOMPUTE_TABLE_SIZE 16U

/* Shoup's method for multiplication uses precomputed table with
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
static int_fast16_t AESGCMLPF3_oneStepOperation(AESGCM_Handle handle,
                                                AESGCM_OneStepOperation *operation,
                                                AESGCM_OperationType operationType);
static int_fast16_t AESGCMLPF3_startOperation(AESGCM_Handle handle, bool isOneStepOrFinalOperation);
static int_fast16_t AESGCMLPF3_waitForResult(AESGCM_Handle handle);
static void AESGCMLPF3_encryptOneAlignedAESBlockECB(AESCommonLPF3_Object *object,
                                                    const uint32_t *input,
                                                    uint32_t *output);
static void AESGCMLPF3_computeGHASH(void *outputBlock, const void *input, int32_t len);
static void AESGCMLPF3_xorBytes(uint8_t *vector1, const uint8_t *vector2, size_t len);
static void AESGCMLPF3_xorBlock(uint32_t *vector1, const uint32_t *vector2);
static void AESGCMLPF3_computeTag(AESGCM_Handle handle, size_t aadLength, size_t inputLength, bool isEncrypt);
static void AESGCMLPF3_precomputeGHASHTable(uint32_t hashKey[AES_BLOCK_SIZE_WORDS]);
static void AESGCMLPF3_galoisMult(uint8_t x[16]);
static inline void AESGCMLPF3_incrementBEWord(uint32_t *word);

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

    /* Callback return behavior is not supported */
    if (params->returnBehavior == AESGCM_RETURN_BEHAVIOR_CALLBACK)
    {
        return NULL;
    }

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
    bool tagValid;
    int_fast16_t status;
    uint32_t ivBits;

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

    /* Check if there is no operation already in progress for this driver
     * instance, and then mark the current operation to be in progress.
     */
    status = AESCommonLPF3_setOperationInProgress(&object->common);

    if (status != AESGCM_STATUS_SUCCESS)
    {
        return status;
    }

    if (!CryptoResourceLPF3_acquireLock(object->common.semaphoreTimeout))
    {
        AESCommonLPF3_clearOperationInProgress(&object->common);
        return AESGCM_STATUS_RESOURCE_UNAVAILABLE;
    }

    object->common.cryptoResourceLocked = true;

    object->operation           = (AESGCM_OperationUnion *)operation;
    object->operationType       = operationType;
    /* We will only change the returnStatus if there is an error or cancellation */
    object->common.returnStatus = AESGCM_STATUS_SUCCESS;

    /* Make internal copy of operational params */
    object->common.key  = *(operation->key);
    object->input       = operation->input;
    object->inputLength = operation->inputLength;
    object->output      = operation->output;
    object->mac         = operation->mac;
    object->aad         = operation->aad;

    /* Clear the counter value */
    (void)memset(object->counter, 0, sizeof(object->counter));

    /* Compute Hash Key H by encrypting a block of zeros */
    AESGCMLPF3_encryptOneAlignedAESBlockECB(&object->common, object->counter, object->hashKey);

    AESGCMLPF3_precomputeGHASHTable(object->hashKey);

    /* Compute the initial counter value */
    if (operation->ivLength == 12U)
    {
        /* Construct the initial counter block Y0: IV || 1 */
        (void)memcpy(object->counter, operation->iv, operation->ivLength);
        object->counter[AES_BLOCK_SIZE_WORDS - 1U] = ((uint32_t)1U << 24U);
    }
    else
    {
        /* Construct the initial counter block Y0: GHASH(IV || 0s+64 || [len(IV)]64)
         * where the IV is padded with the minimum number of '0' bits, possibly
         * none, so that the length of the resulting string is a multiple of 128
         * bits (the block size). This string in turn is appended with 64
         * additional '0' bits, followed by the 64-bit representation of the
         * length of the IV in bits. Finally, the GHASH function is applied to
         * the resulting string to form the initial counter block.
         */

        /* Compute GHASH(IV) */
        AESGCMLPF3_computeGHASH(object->counter, operation->iv, operation->ivLength);

        /* Temporarily use intermediate tag block to store (0s+64 || [len(IV)]64)
         * where [len(IV)]64 is the 64-bit length of IV in bits formatted in
         * big-endian.
         */
        (void)memset(object->intermediateTag, 0, sizeof(object->intermediateTag));
        ivBits                                             = operation->ivLength * 8U;
        object->intermediateTag[AES_BLOCK_SIZE_WORDS - 1U] = BSWAP32(ivBits);

        /* Compute GHASH(0s+64 || [len(IV)]64) with the previously computed
         * GHASH(IV) which yields Y0: GHASH(IV || 0s+64 || [len(IV)]64)
         */
        AESGCMLPF3_computeGHASH(object->counter, object->intermediateTag, sizeof(object->intermediateTag));
    }

    /* Compute tag OPT: ENC(key, Y0) */
    AESGCMLPF3_encryptOneAlignedAESBlockECB(&object->common, object->counter, object->tagOTP);

    /* Compute Y1: Y0 + 1 used for GCTR operation*/
    AESGCMLPF3_incrementBEWord(&object->counter[AES_BLOCK_SIZE_WORDS - 1U]);

    /* Clear intermediate tag */
    (void)memset(object->intermediateTag, 0, sizeof(object->intermediateTag));

    if (operationType == AESGCM_OPERATION_TYPE_ENCRYPT)
    {
        /* Compute Ciphertext */
        status = AESGCMLPF3_startOperation(handle, true);

        /* Compute Authentication tag T */
        AESGCMLPF3_computeTag(handle, operation->aadLength, operation->inputLength, true);

        /* Copy computed tag to operations struct */
        (void)memcpy(operation->mac, object->intermediateTag, object->operation->oneStepOperation.macLength);
    }
    else
    {
        /* Compute Authentication tag T' */
        AESGCMLPF3_computeTag(handle, operation->aadLength, operation->inputLength, false);

        /* Verify if computed T' matches T in operations struct */
        tagValid = CryptoUtils_buffersMatch(object->intermediateTag,
                                            object->mac,
                                            (size_t)object->operation->oneStepOperation.macLength);

        if (tagValid)
        {
            /* Compute Plaintext if tags match */
            status = AESGCMLPF3_startOperation(handle, true);
        }
        else
        {
            CryptoResourceLPF3_releaseLock();
            AESCommonLPF3_clearOperationInProgress(&object->common);
            status = AESGCM_STATUS_MAC_INVALID;
        }
    }

    return status;
}

/*
 *  ======== AESGCMLPF3_incrementBEWord ========
 *  Increments a big-endian word and discards the carry bit if any.
 */
static inline void AESGCMLPF3_incrementBEWord(uint32_t *word)
{
    uint32_t tempWord = *word;

    tempWord = BSWAP32(tempWord);
    tempWord++;
    *word = BSWAP32(tempWord);
}

/*
 *  ======== AESGCMLPF3_computeTag ========
 */
static void AESGCMLPF3_computeTag(AESGCM_Handle handle, size_t aadLength, size_t inputLength, bool isEncrypt)
{
    AESGCMLPF3_Object *object = AESGCMLPF3_getObject(handle);
    uint32_t concatBitLengths[AES_BLOCK_SIZE_WORDS];

    /* Compute GF128-multiplication over AAD */
    if (aadLength > 0U)
    {
        AESGCMLPF3_computeGHASH(object->intermediateTag, object->aad, aadLength);
    }

    /* Compute GF128-multiplication over ciphertext: input for dec and output for enc */
    if (inputLength > 0U)
    {
        if (isEncrypt)
        {
            AESGCMLPF3_computeGHASH(object->intermediateTag, object->output, inputLength);
        }
        else
        {
            AESGCMLPF3_computeGHASH(object->intermediateTag, object->input, inputLength);
        }
    }

    /* Compute GF128-multiplication over concatenated bit lengths of ADD and
     * input. This code is optimized based on the fact that the AAD and input
     * lengths will never be greater than (2^32 / 8) bytes.
     */
    concatBitLengths[0] = 0U;
    concatBitLengths[1] = BSWAP32(aadLength * 8U);
    concatBitLengths[2] = 0U;
    concatBitLengths[3] = BSWAP32(inputLength * 8U);

    AESGCMLPF3_computeGHASH(object->intermediateTag, concatBitLengths, sizeof(concatBitLengths));

    /* Post processing: XOR above GF128-mult output with Tag OTP: E(K, Y0) */
    AESGCMLPF3_xorBlock(object->intermediateTag, object->tagOTP);
}

/*
 *  ======== AESGCMLPF3_encryptOneAlignedAESBlockECB ========
 *  Encrypts a single word-aligned block in polling mode.
 */
static void AESGCMLPF3_encryptOneAlignedAESBlockECB(AESCommonLPF3_Object *object,
                                                    const uint32_t *input,
                                                    uint32_t *output)
{
    /* Set up the key and AES engine to begin an operation */
    AESCommonLPF3_setupOperation(&object->key, AESECBLPF3_SINGLE_BLOCK_AUTOCFG);

    /* Process the single block with CPU R/W */
    AESProcessAlignedBlocksECB(input, output, (uint32_t)AES_GET_NUM_BLOCKS(AES_BLOCK_SIZE));
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
    AESCTRLPF3_writeCounter(object->counter);

    /* Process all blocks with CPU R/W */
    AESCTRLPF3_processData(object->input, object->output, object->inputLength, isOneStepOrFinalOperation);

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
    AESCTRLPF3_readCounter(object->counter);

    /* Save the object's returnStatus before clearing operationInProgress or
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
    /* Cancellation returns an error because this driver does not support
     * callback return behavior.
     */
    return AESGCM_STATUS_ERROR;
}

/*
 *  ======== AESGCMLPF3_xorBytes ========
 *  XOR a vector into another vector
 */
static void AESGCMLPF3_xorBytes(uint8_t *vector1, const uint8_t *vector2, size_t len)
{
    size_t i;

    for (i = 0U; i < len; i++)
    {
        vector1[i] ^= vector2[i];
    }
}

/*
 *  ======== AESGCMLPF3_xorBlock ========
 *  XOR a word-aligned block into another word-aligned block
 */
static void AESGCMLPF3_xorBlock(uint32_t *vector1, const uint32_t *vector2)
{
    uint_fast8_t i;

    for (i = 0U; i < AES_BLOCK_SIZE_WORDS; i++)
    {
        vector1[i] ^= vector2[i];
    }
}

/*
 *  ======== AESGCMLPF3_computeGHASH ========
 *  GHASH Function
 *  @pre AESGCMLPF3_precomputeGHASHTable
 */
static void AESGCMLPF3_computeGHASH(void *outputBlock, const void *input, int32_t len)
{
    for (; len > 0; len -= AES_BLOCK_SIZE)
    {
        if (len >= AES_BLOCK_SIZE)
        {
            /* Check word-alignment of XOR input pointers */
            if (!IS_WORD_ALIGNED(outputBlock) || !IS_WORD_ALIGNED(input))
            {
                AESGCMLPF3_xorBytes(outputBlock, input, AES_BLOCK_SIZE);
            }
            else
            {
                AESGCMLPF3_xorBlock((uint32_t *)outputBlock, (uint32_t *)input);
            }
        }
        else
        {
            AESGCMLPF3_xorBytes(outputBlock, input, len);
        }

        AESGCMLPF3_galoisMult(outputBlock);

        input = (const uint8_t *)input + AES_BLOCK_SIZE;
    }
}

/*
 *  ======== AESGCMLPF3_storeUint32InArray ========
 *  Store a 32 bits unsigned integer in big-endian order in memory
 */
static void AESGCMLPF3_storeUint32InArray(uint8_t *dst, uint32_t x)
{
    uint8_t *buf;

    buf    = dst;
    buf[0] = (uint8_t)((x >> 24) & WORD_MASK);
    buf[1] = (uint8_t)((x >> 16) & WORD_MASK);
    buf[2] = (uint8_t)((x >> 8) & WORD_MASK);
    buf[3] = (uint8_t)x;
}

/*
 *  ======== AESGCMLPF3_precomputeGHASHTable ========
 *  Pre-compute Shoup's 4-bit tables
 */
static void AESGCMLPF3_precomputeGHASHTable(uint32_t hashKey[AES_BLOCK_SIZE_WORDS])
{
    uint_fast8_t i;
    uint_fast8_t j;
    uint64_t upperWord, lowerWord;
    uint64_t vLow, vHigh;
    uint32_t tmp;

    /* Pack hashKey as two 64-bit big-endian variables in vHigh and vLow */
    upperWord = BSWAP32(hashKey[0]);
    lowerWord = BSWAP32(hashKey[1]);
    vHigh     = ((uint64_t)upperWord << 32) | lowerWord;

    upperWord = BSWAP32(hashKey[2]);
    lowerWord = BSWAP32(hashKey[3]);
    vLow      = ((uint64_t)upperWord << 32) | lowerWord;

    /* 8th table entry is the hash key */
    hashTableLow[8]  = vLow;
    hashTableHigh[8] = vHigh;

    /* 1st table entry is all zeros */
    hashTableLow[0]  = 0U;
    hashTableHigh[0] = 0U;

    /* Compute table indices that are powers of two as product of H * special element */
    for (i = 4U; i > 0U; i >>= 1U)
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

/*
 *  ======== AESGCMLPF3_galoisMult ========
 *  x = x.H, computed using the precomputed tables in object
 *  @pre AESGCMLPF3_precomputeGHASHTable
 */
static void AESGCMLPF3_galoisMult(uint8_t *x)
{
    int_fast8_t i;
    uint_fast8_t lower4Bits, upper4Bits, rem;
    uint64_t zHigh, zLow;

    lower4Bits = x[15] & BYTE_MASK;

    zHigh = hashTableHigh[lower4Bits];
    zLow  = hashTableLow[lower4Bits];

    for (i = 15; i >= 0; i--)
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
