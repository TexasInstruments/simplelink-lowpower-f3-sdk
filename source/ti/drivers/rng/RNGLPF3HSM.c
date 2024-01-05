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

/* !!!!!!!!!!!!! WARNING !!!!!!!!!!!!!
 * RNG driver is non-functional on CC27CC devices, as it uses a constant seed.
 */

#include <ti/drivers/RNG.h>
#include <ti/drivers/rng/RNGLPF3HSM.h>
#include <ti/drivers/AESCTRDRBG.h>
#include <ti/drivers/aesctrdrbg/AESCTRDRBGXX.h>
#include <ti/drivers/cryptoutils/utils/CryptoUtils.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(driverlib/hapi.h)

extern const RNGLPF3HSM_HWAttrs RNGLPF3HSM_hwAttrs;
extern uint8_t RNG_instancePool[];

extern const RNG_ReturnBehavior RNGLPF3HSM_returnBehavior;

typedef struct RNGLPF3HSM_OperationParameters_ RNGLPF3HSM_OperationParameters;

typedef bool (*RNGLPF3HSM_validator)(RNGLPF3HSM_OperationParameters *opParams);

/*
 * These values are used with the validator function prototype to provide potentially relevant parameters
 * for validation after a candidate number has been generated.
 */
struct RNGLPF3HSM_OperationParameters_
{
    uint8_t *output;
    size_t outputBitLength;
    CryptoUtils_Endianess endianess;
    const uint8_t *lowerLimit;
    const uint8_t *upperLimit;
};

typedef struct
{
    /* No data in the structure should be read or written without first taking this semaphore. */
    SemaphoreP_Struct accessSemaphore;
    size_t poolLevel;
    AESCTRDRBG_Config drbgConfig;
    AESCTRDRBGXX_Object drbgObject;
    AESCTRDRBGXX_HWAttrs drbgHWattrs;
} RNGLPF3HSM_Instance;

RNGLPF3HSM_Instance RNGLPF3HSM_instanceData;

static bool RNGLPF3HSM_isInitialized = false;

/*** Prototypes ***/
static int_fast16_t RNGLPF3HSM_translateDRBGStatus(int_fast16_t drbgStatus);
static int_fast16_t RNGLPF3HSM_generateEntropy(AESCTRDRBG_Handle drbgHandle, uint8_t *byteDest, size_t byteSize);
static int_fast16_t RNGLPF3HSM_fillPoolIfLessThan(size_t bytes);
static int_fast16_t RNGLPF3HSM_getEntropyFromPool(void *dest, size_t byteSize, size_t *bytesRemaining);
static bool RNGLPF3HSM_checkRange(RNGLPF3HSM_OperationParameters *opParams);
static int_fast16_t RNGLPF3HSM_getValidatedNumber(RNG_Handle handle,
                                                  void *randomNumber,
                                                  size_t randomNumberBitLength,
                                                  CryptoUtils_Endianess endianess,
                                                  RNGLPF3HSM_validator validator,
                                                  const void *lowerLimit,
                                                  const void *upperLimit);
static int_fast16_t RNGLPF3HSM_createDRBGInstance(void);

static int_fast16_t RNGLPF3HSM_translateDRBGStatus(int_fast16_t drbgStatus)
{
    int_fast16_t returnValue;

    switch (drbgStatus)
    {
        case AESCTRDRBG_STATUS_SUCCESS:
            returnValue = RNG_STATUS_SUCCESS;
            break;
        case AESCTRDRBG_STATUS_ERROR:
            returnValue = RNG_STATUS_ERROR;
            break;
        case AESCTRDRBG_STATUS_RESOURCE_UNAVAILABLE:
            returnValue = RNG_STATUS_RESOURCE_UNAVAILABLE;
            break;
        case AESCTRDRBG_STATUS_RESEED_REQUIRED:
            /* Map to RNG_ENTROPY_EXHAUSTED, reboot required for recovery. */
            returnValue = RNG_ENTROPY_EXHAUSTED;
            break;
        case AESCTRDRBG_STATUS_UNINSTANTIATED:
            /* Map to RNG_ENTROPY_EXHAUSTED, reboot required for recovery. */
            returnValue = RNG_ENTROPY_EXHAUSTED;
            break;
        default:
            returnValue = RNG_STATUS_ERROR;
            break;
    }

    return returnValue;
}

int_fast16_t RNGLPF3HSM_generateEntropy(AESCTRDRBG_Handle drbgHandle, uint8_t *byteDest, size_t byteSize)
{
    int_fast16_t returnValue;
    int_fast16_t drbgResult;

    drbgResult = AESCTRDRBG_getRandomBytes(drbgHandle, byteDest, byteSize);

    returnValue = RNGLPF3HSM_translateDRBGStatus(drbgResult);

    return returnValue;
}

static int_fast16_t RNGLPF3HSM_fillPoolIfLessThan(size_t bytes)
{
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;
    size_t bytesNeeded;

    if (RNGLPF3HSM_instanceData.poolLevel < bytes)
    {
        /*
         * Adjust poolLevel to ensure word alignment as underlying AES
         * driver may only support output to word aligned addresses.
         */
        RNGLPF3HSM_instanceData.poolLevel = (RNGLPF3HSM_instanceData.poolLevel >> 2u) << 2u;

        bytesNeeded = RNG_poolByteSize - RNGLPF3HSM_instanceData.poolLevel;

        returnValue = RNGLPF3HSM_generateEntropy((AESCTRDRBG_Handle)&RNGLPF3HSM_instanceData.drbgConfig,
                                                 &RNG_instancePool[RNGLPF3HSM_instanceData.poolLevel],
                                                 bytesNeeded);

        if (returnValue == RNG_STATUS_SUCCESS)
        {
            RNGLPF3HSM_instanceData.poolLevel = RNG_poolByteSize;
        }
    }

    return returnValue;
}

/*
 * Precondition: RNGLPF3HSM_instanceData.accessSemaphore has been taken.
 *
 * Updates bytesRemaining to fulfill the total request (rounded up from number of bits remaining.)
 * These will have to be generated since these additional bytes could not be copied from the pool.
 *
 * Postcondition: If dest is not word aligned, then bytesRemaining  will either be 0 or dest[byteSize-bytesRemaining]
 *                will be word aligned.
 */
static int_fast16_t RNGLPF3HSM_getEntropyFromPool(void *dest, size_t byteSize, size_t *bytesRemaining)
{

    uint8_t *byteDest        = (uint8_t *)dest;
    size_t bytesToCopy       = byteSize;
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;

    if (RNGLPF3HSM_instanceData.poolLevel < byteSize && ((uintptr_t)dest & 0x3u) != 0u &&
        RNGLPF3HSM_instanceData.poolLevel < (4 - ((uintptr_t)dest & 0x3u)))
    {
        /* Fill pool so there will be enough entropy to get to an aligned address within dest[]. */
        returnValue = RNGLPF3HSM_fillPoolIfLessThan(RNG_poolByteSize);
    }

    if (RNGLPF3HSM_instanceData.poolLevel < byteSize)
    {
        /*
         * Cap number of bytes taken from pool to ensure next byte of entropy to generate into dest
         * is at a word-aligned address.
         */
        bytesToCopy = (4 - ((uintptr_t)dest & 0x3u));
        bytesToCopy = bytesToCopy + (((RNGLPF3HSM_instanceData.poolLevel - bytesToCopy) >> 2u) << 2u);
    }

    /* Get entropy from pool */
    if ((bytesToCopy > 0u) && (RNGLPF3HSM_instanceData.poolLevel > 0u))
    {
        (void)memcpy(byteDest, &RNG_instancePool[RNGLPF3HSM_instanceData.poolLevel - bytesToCopy], bytesToCopy);
        CryptoUtils_memset(&RNG_instancePool[RNGLPF3HSM_instanceData.poolLevel - bytesToCopy],
                           RNG_poolByteSize,
                           0,
                           bytesToCopy);
        RNGLPF3HSM_instanceData.poolLevel -= bytesToCopy;
    }

    *bytesRemaining = byteSize - bytesToCopy;

    return returnValue;
}

static bool RNGLPF3HSM_checkRange(RNGLPF3HSM_OperationParameters *opParams)
{
    return CryptoUtils_isNumberInRange(opParams->output,
                                       opParams->outputBitLength,
                                       opParams->endianess,
                                       opParams->lowerLimit,
                                       opParams->upperLimit);
}

static int_fast16_t RNGLPF3HSM_getValidatedNumber(RNG_Handle handle,
                                                  void *randomNumber,
                                                  size_t randomNumberBitLength,
                                                  CryptoUtils_Endianess endianess,
                                                  RNGLPF3HSM_validator validator,
                                                  const void *lowerLimit,
                                                  const void *upperLimit)
{
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;
    RNGLPF3HSM_Object *object;
    size_t bytesToGenerate = 0;
    size_t byteSize;
    uint8_t *byteDestination;
    uint8_t bitMask;
    bool isValid = false;
    RNGLPF3HSM_OperationParameters opParams;

    if ((handle == NULL) || (randomNumber == NULL) || (randomNumberBitLength >= RNG_MAX_BIT_LENGTH))
    {
        returnValue = RNG_STATUS_INVALID_INPUTS;
    }

    if (returnValue == RNG_STATUS_SUCCESS)
    {
        object = (RNGLPF3HSM_Object *)handle->object;

        if (SemaphoreP_pend(&RNGLPF3HSM_instanceData.accessSemaphore, object->timeout) != SemaphoreP_OK)
        {
            returnValue = RNG_STATUS_RESOURCE_UNAVAILABLE;
        }
    }

    if (returnValue == RNG_STATUS_SUCCESS)
    {
        /*
         * Convert bit length to byte size by rounding up the number of bytes.
         * Mask the extra bits from rounding up written in the destination buffer.
         */
        byteSize        = (randomNumberBitLength + 7u) >> 3u;
        byteDestination = (uint8_t *)randomNumber;
        bitMask         = (2u << (((randomNumberBitLength + 7u) % 8u))) - 1u;
    }

    while ((returnValue == RNG_STATUS_SUCCESS) && !isValid)
    {

        returnValue = RNGLPF3HSM_getEntropyFromPool(byteDestination, byteSize, &bytesToGenerate);

        if (returnValue == RNG_STATUS_SUCCESS && bytesToGenerate > 0u)
        {
            returnValue = RNGLPF3HSM_generateEntropy((AESCTRDRBG_Handle)&RNGLPF3HSM_instanceData.drbgConfig,
                                                     &byteDestination[byteSize - bytesToGenerate],
                                                     bytesToGenerate);
        }

        /* Mask off extra bits in MSB */
        if (endianess == CryptoUtils_ENDIANESS_BIG)
        {
            byteDestination[0] &= bitMask;
        }
        else
        {
            byteDestination[byteSize - 1u] &= bitMask;
        }

        if (validator != NULL)
        {
            opParams.outputBitLength = randomNumberBitLength;
            opParams.output          = randomNumber;
            opParams.endianess       = endianess;
            opParams.lowerLimit      = lowerLimit;
            opParams.upperLimit      = upperLimit;

            isValid = validator(&opParams);
        }
        else
        {
            isValid = true;
        }
    }

    SemaphoreP_post(&RNGLPF3HSM_instanceData.accessSemaphore);

    return returnValue;
}

static int_fast16_t RNGLPF3HSM_createDRBGInstance(void)
{
    int_fast16_t returnValue = RNG_STATUS_ERROR;
    AESCTRDRBG_Handle drbgHandle;
    AESCTRDRBG_Params drbgParams;

    AESCTRDRBG_init();

    /* Copy RNG interrupt priority (set in syscfg) to AES CTR interrupt priority */
    RNGLPF3HSM_instanceData.drbgHWattrs.aesctrHWAttrs.intPriority = RNGLPF3HSM_hwAttrs.intPriority;

    RNGLPF3HSM_instanceData.drbgConfig.object  = &RNGLPF3HSM_instanceData.drbgObject;
    RNGLPF3HSM_instanceData.drbgConfig.hwAttrs = &RNGLPF3HSM_instanceData.drbgHWattrs;
    RNGLPF3HSM_instanceData.drbgObject.isOpen  = false;

    AESCTRDRBG_Params_init(&drbgParams);
    /* Ensure seed length will be 32 bytes long. (Seed length = key length + AES block length.) */
    drbgParams.keyLength      = AESCTRDRBG_AES_KEY_LENGTH_128;
    drbgParams.returnBehavior = (AESCTRDRBG_ReturnBehavior)RNGLPF3HSM_returnBehavior;

    drbgHandle = AESCTRDRBG_construct((AESCTRDRBG_Handle)&RNGLPF3HSM_instanceData.drbgConfig, &drbgParams);

    if (drbgHandle != NULL)
    {
        returnValue = RNG_STATUS_SUCCESS;
    }

    return returnValue;
}

void RNG_Params_init(RNG_Params *params)
{
    *params                = RNG_defaultParams;
    params->returnBehavior = RNGLPF3HSM_returnBehavior;
}

int_fast16_t RNG_init(void)
{
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;
    int_fast16_t drbgResult;
    /* Use zeros as dummy seed value */
    uint32_t seed[8] = {0};

    if (RNGLPF3HSM_isInitialized == false)
    {
        if (returnValue == RNG_STATUS_SUCCESS)
        {
            RNGLPF3HSM_instanceData.poolLevel = 0;

            if (SemaphoreP_constructBinary(&RNGLPF3HSM_instanceData.accessSemaphore, 1) == NULL)
            {
                returnValue = RNG_STATUS_ERROR;
            }
        }

        if (returnValue == RNG_STATUS_SUCCESS)
        {

            returnValue = RNGLPF3HSM_createDRBGInstance();

            if (returnValue == RNG_STATUS_SUCCESS)
            {
                drbgResult  = AESCTRDRBG_reseed((AESCTRDRBG_Handle)&RNGLPF3HSM_instanceData.drbgConfig, seed, NULL, 0);
                returnValue = RNGLPF3HSM_translateDRBGStatus(drbgResult);
                CryptoUtils_memset(seed, sizeof(seed), 0, sizeof(seed));
            }
        }

        if (returnValue == RNG_STATUS_SUCCESS)
        {
            RNGLPF3HSM_isInitialized = true;
        }
    }

    return returnValue;
}

RNG_Handle RNG_construct(const RNG_Config *config, const RNG_Params *params)
{
    RNG_Handle handle;
    RNGLPF3HSM_Object *object;
    uintptr_t key;

    handle = (RNG_Handle)config;
    object = handle->object;

    key = HwiP_disable();

    if (object->isOpen)
    {
        HwiP_restore(key);
        handle = NULL;
    }

    if (handle != NULL)
    {
        object->isOpen = true;

        HwiP_restore(key);

        /* If params are NULL, use defaults */
        if (params == NULL)
        {
            object->timeout = RNG_defaultParams.timeout;
        }
        else
        {
            /*
             * Return behavior is set statically for all instances and on open the requesting setting must
             * match the static setting.
             */
            if (params->returnBehavior != RNGLPF3HSM_returnBehavior)
            {
                handle = NULL;
            }

            /*
             * Callback return behavior is not supported.
             */
            if (params->returnBehavior == RNG_RETURN_BEHAVIOR_CALLBACK)
            {
                handle = NULL;
            }
        }
    }

    return handle;
}

void RNG_close(RNG_Handle handle)
{
    RNGLPF3HSM_Object *object;

    if (handle != NULL)
    {
        object         = (RNGLPF3HSM_Object *)handle->object;
        object->isOpen = false;
    }
}

int_fast16_t RNG_getRandomBits(RNG_Handle handle, void *randomBits, size_t randomBitsLength)
{

    return RNGLPF3HSM_getValidatedNumber(handle,
                                         randomBits,
                                         randomBitsLength,
                                         CryptoUtils_ENDIANESS_LITTLE,
                                         NULL,
                                         NULL,
                                         NULL);
}

int_fast16_t RNG_getLERandomNumberInRange(RNG_Handle handle,
                                          const void *lowerLimit,
                                          const void *upperLimit,
                                          void *randomNumber,
                                          size_t randomNumberBitLength)
{

    return RNGLPF3HSM_getValidatedNumber(handle,
                                         randomNumber,
                                         randomNumberBitLength,
                                         CryptoUtils_ENDIANESS_LITTLE,
                                         &RNGLPF3HSM_checkRange,
                                         lowerLimit,
                                         upperLimit);
}

int_fast16_t RNG_getBERandomNumberInRange(RNG_Handle handle,
                                          const void *lowerLimit,
                                          const void *upperLimit,
                                          void *randomNumber,
                                          size_t randomNumberBitLength)
{

    return RNGLPF3HSM_getValidatedNumber(handle,
                                         randomNumber,
                                         randomNumberBitLength,
                                         CryptoUtils_ENDIANESS_BIG,
                                         &RNGLPF3HSM_checkRange,
                                         lowerLimit,
                                         upperLimit);
}

int_fast16_t RNG_generateKey(RNG_Handle handle, CryptoKey *key)
{
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;
    uint8_t *randomBits;
    size_t randomBitsLength;

    if (key->encoding != CryptoKey_BLANK_PLAINTEXT)
    {
        returnValue = RNG_STATUS_INVALID_INPUTS;
    }

    if (key->u.plaintext.keyLength > (RNG_MAX_BIT_LENGTH >> 3u))
    {
        returnValue = RNG_STATUS_INVALID_INPUTS;
    }

    if (returnValue == RNG_STATUS_SUCCESS)
    {
        randomBits       = key->u.plaintext.keyMaterial;
        randomBitsLength = key->u.plaintext.keyLength << 3u; /* Bytes to bits */

        returnValue = RNGLPF3HSM_getValidatedNumber(handle,
                                                    randomBits,
                                                    randomBitsLength,
                                                    CryptoUtils_ENDIANESS_LITTLE,
                                                    NULL,
                                                    NULL,
                                                    NULL);
    }

    return returnValue;
}

int_fast16_t RNG_generateLEKeyInRange(RNG_Handle handle,
                                      const void *lowerLimit,
                                      const void *upperLimit,
                                      CryptoKey *key,
                                      size_t randomNumberBitLength)
{
    int_fast16_t returnValue;
    uint8_t *randomBits;

    if (key->encoding != CryptoKey_BLANK_PLAINTEXT)
    {
        returnValue = RNG_STATUS_INVALID_INPUTS;
    }
    else
    {

        randomBits = key->u.plaintext.keyMaterial;

        returnValue = RNGLPF3HSM_getValidatedNumber(handle,
                                                    randomBits,
                                                    randomNumberBitLength,
                                                    CryptoUtils_ENDIANESS_LITTLE,
                                                    &RNGLPF3HSM_checkRange,
                                                    lowerLimit,
                                                    upperLimit);
    }

    return returnValue;
}

int_fast16_t RNG_generateBEKeyInRange(RNG_Handle handle,
                                      const void *lowerLimit,
                                      const void *upperLimit,
                                      CryptoKey *key,
                                      size_t randomNumberBitLength)
{
    int_fast16_t returnValue;
    uint8_t *randomBits;

    if (key->encoding != CryptoKey_BLANK_PLAINTEXT)
    {
        returnValue = RNG_STATUS_INVALID_INPUTS;
    }
    else
    {
        randomBits = key->u.plaintext.keyMaterial;

        returnValue = RNGLPF3HSM_getValidatedNumber(handle,
                                                    randomBits,
                                                    randomNumberBitLength,
                                                    CryptoUtils_ENDIANESS_BIG,
                                                    &RNGLPF3HSM_checkRange,
                                                    lowerLimit,
                                                    upperLimit);
    }

    return returnValue;
}

int_fast16_t RNG_fillPoolIfLessThan(size_t bytes)
{
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;

    if (SemaphoreP_pend(&RNGLPF3HSM_instanceData.accessSemaphore, SemaphoreP_WAIT_FOREVER) != SemaphoreP_OK)
    {
        returnValue = RNG_STATUS_RESOURCE_UNAVAILABLE;
    }
    else
    {
        returnValue = RNGLPF3HSM_fillPoolIfLessThan(bytes);

        SemaphoreP_post(&RNGLPF3HSM_instanceData.accessSemaphore);
    }

    return returnValue;
}

int_fast16_t RNG_cancelOperation(RNG_Handle handle)
{
    /* Cancel not supported in this implementation since AESCTRDRBG driver does not support cancellation. */
    return RNG_STATUS_ERROR;
}
