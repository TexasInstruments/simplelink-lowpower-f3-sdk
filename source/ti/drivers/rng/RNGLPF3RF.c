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
/*
 *  ======== RNGLPF3RF.c ========
 */

#include <ti/drivers/RNG.h>
#include <ti/drivers/rng/RNGLPF3RF.h>
#include <ti/drivers/AESCMAC.h>
#include <ti/drivers/AESCTRDRBG.h>
#include <ti/drivers/aescmac/AESCMACLPF3.h>
#include <ti/drivers/aesctrdrbg/AESCTRDRBGXX.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKeyPlaintext.h>
#include <ti/drivers/cryptoutils/sharedresources/CryptoResourceLPF3.h>
#include <ti/drivers/cryptoutils/utils/CryptoUtils.h>

#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/HwiP.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_memmap.h)

extern const RNGLPF3RF_HWAttrs RNGLPF3RF_hwAttrs;
extern uint8_t RNG_instancePool[];

extern const RNG_ReturnBehavior RNGLPF3RF_returnBehavior;

extern const bool RNGLPF3RF_rctEnabled;
extern const bool RNGLPF3RF_aptEnabled;
extern const int RNGLPF3RF_rctThreshold;
extern const int RNGLPF3RF_aptThreshold;
extern const int RNGLPF3RF_aptBimodalThreshold;

/* CBC MAC key words used to compute RNG seed */
extern uint32_t RNGLPF3RF_noiseConditioningKeyWord0;
extern uint32_t RNGLPF3RF_noiseConditioningKeyWord1;
extern uint32_t RNGLPF3RF_noiseConditioningKeyWord2;
extern uint32_t RNGLPF3RF_noiseConditioningKeyWord3;

typedef struct RNGLPF3RF_OperationParameters_ RNGLPF3RF_OperationParameters;

typedef bool (*RNGLPF3RF_validator)(RNGLPF3RF_OperationParameters *opParams);

/* Mask used to extract the upper or lower byte of a word */
#define BYTE_MASK 0xff

/*
 * Window size for Adaptive Proportion Test
 */
#define RNG_APT_WINDOW_SIZE 512

#define RNG_MAX_RESEED_INTERVAL UINT32_MAX

/*
 * These values are used with the validator function prototype to provide potentially relevant parameters
 * for validation after a candidate number has been generated.
 */
struct RNGLPF3RF_OperationParameters_
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
} RNGLPF3RF_Instance;

RNGLPF3RF_Instance RNGLPF3RF_instanceData;

static bool RNGLPF3RF_isInitialized = false;
static bool RNGLPF3RF_isSeeded      = false;

/*** Prototypes ***/
static int_fast16_t RNGLPF3RF_translateDRBGStatus(int_fast16_t drbgStatus);
static int_fast16_t RNGLPF3RF_generateEntropy(AESCTRDRBG_Handle drbgHandle, uint8_t *byteDest, size_t byteSize);
static int_fast16_t RNGLPF3RF_fillPoolIfLessThan(size_t bytes);
static int_fast16_t RNGLPF3RF_getEntropyFromPool(void *dest, size_t byteSize, size_t *bytesRemaining);
static bool RNGLPF3RF_checkRange(RNGLPF3RF_OperationParameters *opParams);
static int_fast16_t RNGLPF3RF_getValidatedNumber(RNG_Handle handle,
                                                 void *randomNumber,
                                                 size_t randomNumberBitLength,
                                                 CryptoUtils_Endianess endianess,
                                                 RNGLPF3RF_validator validator,
                                                 const void *lowerLimit,
                                                 const void *upperLimit);
static int_fast16_t RNGLPF3RF_createDRBGInstance(void);
static int_fast16_t RNGLPF3RF_conditionNoise(uint32_t *noiseInput, uint32_t *seed);
static void RNGLPF3RF_addCodesToAptDensities(uint16_t codes, volatile uint16_t *densities);
static int_fast16_t RNGLPF3RF_entropyHealthTests(uint32_t *noiseData);

/*
 *  ======== RNGLPF3RF_translateDRBGStatus ========
 */
static int_fast16_t RNGLPF3RF_translateDRBGStatus(int_fast16_t drbgStatus)
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

/*
 *  ======== RNGLPF3RF_generateEntropy ========
 */
int_fast16_t RNGLPF3RF_generateEntropy(AESCTRDRBG_Handle drbgHandle, uint8_t *byteDest, size_t byteSize)
{
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;
    int_fast16_t drbgResult;

    if (!RNGLPF3RF_isInitialized)
    {
        returnValue = RNG_STATUS_NOT_INITIALIZED;
    }

    if (returnValue != RNG_STATUS_NOT_INITIALIZED)
    {
        drbgResult = AESCTRDRBG_getRandomBytes(drbgHandle, byteDest, byteSize);

        returnValue = RNGLPF3RF_translateDRBGStatus(drbgResult);
    }

    return returnValue;
}

/*
 *  ======== RNGLPF3RF_fillPoolIfLessThan ========
 */
static int_fast16_t RNGLPF3RF_fillPoolIfLessThan(size_t bytes)
{
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;
    size_t bytesNeeded;

    if (RNGLPF3RF_instanceData.poolLevel < bytes)
    {
        /*
         * Adjust poolLevel to ensure word alignment as underlying AES
         * driver may only support output to word aligned addresses.
         */
        RNGLPF3RF_instanceData.poolLevel = (RNGLPF3RF_instanceData.poolLevel >> 2u) << 2u;

        bytesNeeded = RNG_poolByteSize - RNGLPF3RF_instanceData.poolLevel;

        returnValue = RNGLPF3RF_generateEntropy((AESCTRDRBG_Handle)&RNGLPF3RF_instanceData.drbgConfig,
                                                &RNG_instancePool[RNGLPF3RF_instanceData.poolLevel],
                                                bytesNeeded);

        if (returnValue == RNG_STATUS_SUCCESS)
        {
            RNGLPF3RF_instanceData.poolLevel = RNG_poolByteSize;
        }
    }

    return returnValue;
}

/*
 * Precondition: RNGLPF3RF_instanceData.accessSemaphore has been taken.
 *
 * Updates bytesRemaining to fulfill the total request (rounded up from number of bits remaining.)
 * These will have to be generated since these additional bytes could not be copied from the pool.
 *
 * Postcondition: If dest is not word aligned, then bytesRemaining  will either be 0 or dest[byteSize-bytesRemaining]
 *                will be word aligned.
 */
/*
 *  ======== RNGLPF3RF_getEntropyFromPool ========
 */
static int_fast16_t RNGLPF3RF_getEntropyFromPool(void *dest, size_t byteSize, size_t *bytesRemaining)
{

    uint8_t *byteDest        = (uint8_t *)dest;
    size_t bytesToCopy       = byteSize;
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;

    if (RNGLPF3RF_instanceData.poolLevel < byteSize && ((uintptr_t)dest & 0x3u) != 0u &&
        RNGLPF3RF_instanceData.poolLevel < (4 - ((uintptr_t)dest & 0x3u)))
    {
        /* Fill pool so there will be enough entropy to get to an aligned address within dest[]. */
        returnValue = RNGLPF3RF_fillPoolIfLessThan(RNG_poolByteSize);
    }

    if (RNGLPF3RF_instanceData.poolLevel < byteSize)
    {
        /*
         * Cap number of bytes taken from pool to ensure next byte of entropy to generate into dest
         * is at a word-aligned address.
         */
        bytesToCopy = (4 - ((uintptr_t)dest & 0x3u));
        bytesToCopy = bytesToCopy + (((RNGLPF3RF_instanceData.poolLevel - bytesToCopy) >> 2u) << 2u);
    }

    /* Get entropy from pool */
    if ((bytesToCopy > 0u) && (RNGLPF3RF_instanceData.poolLevel > 0u))
    {
        (void)memcpy(byteDest, &RNG_instancePool[RNGLPF3RF_instanceData.poolLevel - bytesToCopy], bytesToCopy);
        CryptoUtils_memset(&RNG_instancePool[RNGLPF3RF_instanceData.poolLevel - bytesToCopy],
                           RNG_poolByteSize,
                           0,
                           bytesToCopy);
        RNGLPF3RF_instanceData.poolLevel -= bytesToCopy;
    }

    *bytesRemaining = byteSize - bytesToCopy;

    return returnValue;
}

/*
 *  ======== RNGLPF3RF_checkRange ========
 */
static bool RNGLPF3RF_checkRange(RNGLPF3RF_OperationParameters *opParams)
{
    return CryptoUtils_isNumberInRange(opParams->output,
                                       opParams->outputBitLength,
                                       opParams->endianess,
                                       opParams->lowerLimit,
                                       opParams->upperLimit);
}

/*
 *  ======== RNGLPF3RF_getValidatedNumber ========
 */
static int_fast16_t RNGLPF3RF_getValidatedNumber(RNG_Handle handle,
                                                 void *randomNumber,
                                                 size_t randomNumberBitLength,
                                                 CryptoUtils_Endianess endianess,
                                                 RNGLPF3RF_validator validator,
                                                 const void *lowerLimit,
                                                 const void *upperLimit)
{
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;
    RNGLPF3RF_Object *object;
    size_t bytesToGenerate = 0;
    size_t byteSize;
    uint8_t *byteDestination;
    uint8_t bitMask;
    bool isValid = false;
    RNGLPF3RF_OperationParameters opParams;

    if ((handle == NULL) || (randomNumber == NULL) || (randomNumberBitLength >= RNG_MAX_BIT_LENGTH))
    {
        returnValue = RNG_STATUS_INVALID_INPUTS;
    }

    if (returnValue == RNG_STATUS_SUCCESS)
    {
        object = (RNGLPF3RF_Object *)handle->object;

        if (SemaphoreP_pend(&RNGLPF3RF_instanceData.accessSemaphore, object->timeout) != SemaphoreP_OK)
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

        returnValue = RNGLPF3RF_getEntropyFromPool(byteDestination, byteSize, &bytesToGenerate);

        if (returnValue == RNG_STATUS_SUCCESS && bytesToGenerate > 0u)
        {
            returnValue = RNGLPF3RF_generateEntropy((AESCTRDRBG_Handle)&RNGLPF3RF_instanceData.drbgConfig,
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

    SemaphoreP_post(&RNGLPF3RF_instanceData.accessSemaphore);

    return returnValue;
}

/*
 *  ======== RNGLPF3RF_createDRBGInstance ========
 */
static int_fast16_t RNGLPF3RF_createDRBGInstance(void)
{
    int_fast16_t returnValue = RNG_STATUS_ERROR;
    AESCTRDRBG_Handle drbgHandle;
    AESCTRDRBG_Params drbgParams;

    AESCTRDRBG_init();

    /* Copy RNG interrupt priority (set in syscfg) to AES CTR interrupt priority */
    RNGLPF3RF_instanceData.drbgHWattrs.aesctrHWAttrs.intPriority = RNGLPF3RF_hwAttrs.intPriority;

    RNGLPF3RF_instanceData.drbgConfig.object  = &RNGLPF3RF_instanceData.drbgObject;
    RNGLPF3RF_instanceData.drbgConfig.hwAttrs = &RNGLPF3RF_instanceData.drbgHWattrs;
    RNGLPF3RF_instanceData.drbgObject.isOpen  = false;

    AESCTRDRBG_Params_init(&drbgParams);
    /* Ensure seed length will be 32 bytes long. (Seed length = key length + AES block length.) */
    drbgParams.keyLength      = AESCTRDRBG_AES_KEY_LENGTH_128;
    drbgParams.returnBehavior = (AESCTRDRBG_ReturnBehavior)RNGLPF3RF_returnBehavior;
    drbgParams.reseedInterval = RNG_MAX_RESEED_INTERVAL;

    drbgHandle = AESCTRDRBG_construct((AESCTRDRBG_Handle)&RNGLPF3RF_instanceData.drbgConfig, &drbgParams);

    if (drbgHandle != NULL)
    {
        returnValue = RNG_STATUS_SUCCESS;
    }

    return returnValue;
}

/* Store a 32-bit unsigned integer as bytes in big-endian order in memory. */
/*
 *  ======== RNGLPF3RF_storeUint32InArray ========
 */
static void RNGLPF3RF_storeUint32InArray(uint8_t *dst, uint32_t x)
{
    uint8_t *buf;

    buf    = dst;
    buf[0] = (uint8_t)(x >> 24U) & BYTE_MASK;
    buf[1] = (uint8_t)(x >> 16U) & BYTE_MASK;
    buf[2] = (uint8_t)(x >> 8U) & BYTE_MASK;
    buf[3] = (uint8_t)x;
}

/* Condition the noise input using CBC MAC to generate the seed */
/*
 *  ======== RNGLPF3RF_conditionNoise ========
 */
static int_fast16_t RNGLPF3RF_conditionNoise(uint32_t *noiseInput, uint32_t *seed)
{
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;
    uint8_t *rawData;
    size_t rawDataLength;

    /* CBC MAC variables */
    uint8_t keyMaterial[AES_BLOCK_SIZE];
    CryptoKey cryptoKey;
    int_fast16_t cbcMacResult;
    AESCMAC_Operation cbcmacOperation;
    AESCMAC_Handle dynHandle;
    AESCMAC_Config dynConfig;
    AESCMACLPF3_Object dynObject;
    const AESCMACLPF3_HWAttrs dynHWAttrs = {.intPriority = (~0)};
    AESCMAC_Params params;

    RNGLPF3RF_storeUint32InArray(keyMaterial, RNGLPF3RF_noiseConditioningKeyWord0);
    RNGLPF3RF_storeUint32InArray(keyMaterial + 4, RNGLPF3RF_noiseConditioningKeyWord1);
    RNGLPF3RF_storeUint32InArray(keyMaterial + 8, RNGLPF3RF_noiseConditioningKeyWord2);
    RNGLPF3RF_storeUint32InArray(keyMaterial + 12, RNGLPF3RF_noiseConditioningKeyWord3);

    /* Check if the Noise input pointer was initialized */
    if (noiseInput == NULL)
    {
        return RNG_STATUS_NOISE_INPUT_INVALID;
    }
    /* Check if noise input was populated by user before calling RNG_init() */
    bool noiseFilled = CryptoUtils_isBufferAllZeros(noiseInput, AES_BLOCK_SIZE);
    if (noiseFilled)
    {
        return RNG_STATUS_NOISE_INPUT_INVALID;
    }

    if (RNGLPF3RF_rctEnabled || RNGLPF3RF_aptEnabled)
    {
        /* Health check before conditioning */
        returnValue = RNGLPF3RF_entropyHealthTests(noiseInput);
        if (returnValue != RNG_STATUS_SUCCESS)
        {
            return returnValue;
        }
    }

    rawData       = (uint8_t *)noiseInput;
    rawDataLength = RNGLPF3RF_noiseInputWordLen * 4;

    memset(&dynObject, 0x00, sizeof(dynObject));

    dynConfig.object  = &dynObject;
    dynConfig.hwAttrs = &dynHWAttrs;

    AESCMAC_init();
    AESCMAC_Params_init(&params);

    params.operationalMode = AESCMAC_OPMODE_CBCMAC;
    params.returnBehavior  = AESCMAC_RETURN_BEHAVIOR_POLLING;

    dynHandle = AESCMAC_construct(&dynConfig, &params);

    if (dynHandle == NULL)
    {
        return RNG_STATUS_ERROR;
    }

    /* Setup Entropy Conditioning Key */
    CryptoKeyPlaintext_initKey(&cryptoKey, keyMaterial, sizeof(keyMaterial));

    AESCMAC_Operation_init(&cbcmacOperation);

    /* Generate 16B of seed using half the entropy*/
    cbcmacOperation.input       = rawData;
    cbcmacOperation.inputLength = rawDataLength / 2;
    cbcmacOperation.macLength   = 16;
    cbcmacOperation.mac         = (uint8_t *)seed;
    cbcMacResult                = AESCMAC_oneStepSign(dynHandle, &cbcmacOperation, &cryptoKey);

    if (cbcMacResult != AESCMAC_STATUS_SUCCESS)
    {
        returnValue = RNG_STATUS_ERROR;
    }

    if (cbcMacResult == AESCMAC_STATUS_SUCCESS)
    {
        /* Generate another 16B of seed using the rest of the entropy */
        cbcmacOperation.input = rawData + rawDataLength / 2;
        cbcmacOperation.mac   = (uint8_t *)seed + 16;
        cbcMacResult          = AESCMAC_oneStepSign(dynHandle, &cbcmacOperation, &cryptoKey);

        if (cbcMacResult != AESCMAC_STATUS_SUCCESS)
        {
            returnValue = RNG_STATUS_ERROR;
        }
    }

    /* Clear noise input after conditioning */
    memset(noiseInput, 0, RNGLPF3RF_noiseInputWordLen * sizeof(uint32_t));

    /* Destroy dynamic instance */
    AESCMAC_close(dynHandle);

    return returnValue;
}

/*
 *  ======== RNG_Params_init ========
 */
void RNG_Params_init(RNG_Params *params)
{
    *params                = RNG_defaultParams;
    params->returnBehavior = RNGLPF3RF_returnBehavior;
}

/*
 *  ======== RNGLPF3RF_conditionNoiseToGenerateSeed ========
 */
int_fast16_t RNGLPF3RF_conditionNoiseToGenerateSeed(uint32_t *noisePtr)
{
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;
    int_fast16_t drbgResult;
    uint32_t seed[256 / 32]; /* Seed is SHA256 Digest: 256 bits */
    if (RNGLPF3RF_isSeeded == false)
    {
        RNGLPF3RF_instanceData.poolLevel = 0;

        if (SemaphoreP_constructBinary(&RNGLPF3RF_instanceData.accessSemaphore, 1) == NULL)
        {
            returnValue = RNG_STATUS_ERROR;
        }

        if (returnValue == RNG_STATUS_SUCCESS)
        {
            returnValue = RNGLPF3RF_conditionNoise(noisePtr, seed);
        }

        if (returnValue == RNG_STATUS_SUCCESS)
        {

            returnValue = RNGLPF3RF_createDRBGInstance();

            if (returnValue == RNG_STATUS_SUCCESS)
            {
                drbgResult  = AESCTRDRBG_reseed((AESCTRDRBG_Handle)&RNGLPF3RF_instanceData.drbgConfig, seed, NULL, 0);
                returnValue = RNGLPF3RF_translateDRBGStatus(drbgResult);
                CryptoUtils_memset(seed, sizeof(seed), 0, sizeof(seed));
            }

            if (returnValue == RNG_STATUS_SUCCESS)
            {
                RNGLPF3RF_isSeeded = true;
            }
        }
    }

    return returnValue;
}

/*
 *  ======== RNG_init ========
 */
int_fast16_t RNG_init(void)
{
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;

    if (RNGLPF3RF_isInitialized == false)
    {
        /* If the RNG driver was seeded, initialization was successful */
        if (RNGLPF3RF_isSeeded == true)
        {
            RNGLPF3RF_isInitialized = true;
        }
    }

    return returnValue;
}

/*
 *  ======== RNG_construct ========
 */
RNG_Handle RNG_construct(const RNG_Config *config, const RNG_Params *params)
{
    RNG_Handle handle;
    RNGLPF3RF_Object *object;
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
            if (params->returnBehavior != RNGLPF3RF_returnBehavior)
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

/*
 *  ======== RNG_close ========
 */
void RNG_close(RNG_Handle handle)
{
    RNGLPF3RF_Object *object;

    if (handle != NULL)
    {
        object         = (RNGLPF3RF_Object *)handle->object;
        object->isOpen = false;
    }
}

/*
 *  ======== RNG_getRandomBits ========
 */
int_fast16_t RNG_getRandomBits(RNG_Handle handle, void *randomBits, size_t randomBitsLength)
{

    return RNGLPF3RF_getValidatedNumber(handle,
                                        randomBits,
                                        randomBitsLength,
                                        CryptoUtils_ENDIANESS_LITTLE,
                                        NULL,
                                        NULL,
                                        NULL);
}

/*
 *  ======== RNG_getLERandomNumberInRange ========
 */
int_fast16_t RNG_getLERandomNumberInRange(RNG_Handle handle,
                                          const void *lowerLimit,
                                          const void *upperLimit,
                                          void *randomNumber,
                                          size_t randomNumberBitLength)
{

    return RNGLPF3RF_getValidatedNumber(handle,
                                        randomNumber,
                                        randomNumberBitLength,
                                        CryptoUtils_ENDIANESS_LITTLE,
                                        &RNGLPF3RF_checkRange,
                                        lowerLimit,
                                        upperLimit);
}

/*
 *  ======== RNG_getBERandomNumberInRange ========
 */
int_fast16_t RNG_getBERandomNumberInRange(RNG_Handle handle,
                                          const void *lowerLimit,
                                          const void *upperLimit,
                                          void *randomNumber,
                                          size_t randomNumberBitLength)
{

    return RNGLPF3RF_getValidatedNumber(handle,
                                        randomNumber,
                                        randomNumberBitLength,
                                        CryptoUtils_ENDIANESS_BIG,
                                        &RNGLPF3RF_checkRange,
                                        lowerLimit,
                                        upperLimit);
}

/*
 *  ======== RNG_generateKey ========
 */
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

        returnValue = RNGLPF3RF_getValidatedNumber(handle,
                                                   randomBits,
                                                   randomBitsLength,
                                                   CryptoUtils_ENDIANESS_LITTLE,
                                                   NULL,
                                                   NULL,
                                                   NULL);
    }

    return returnValue;
}

/*
 *  ======== RNG_generateLEKeyInRange ========
 */
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

        returnValue = RNGLPF3RF_getValidatedNumber(handle,
                                                   randomBits,
                                                   randomNumberBitLength,
                                                   CryptoUtils_ENDIANESS_LITTLE,
                                                   &RNGLPF3RF_checkRange,
                                                   lowerLimit,
                                                   upperLimit);
    }

    return returnValue;
}

/*
 *  ======== RNG_generateBEKeyInRange ========
 */
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

        returnValue = RNGLPF3RF_getValidatedNumber(handle,
                                                   randomBits,
                                                   randomNumberBitLength,
                                                   CryptoUtils_ENDIANESS_BIG,
                                                   &RNGLPF3RF_checkRange,
                                                   lowerLimit,
                                                   upperLimit);
    }

    return returnValue;
}

/*
 *  ======== RNG_fillPoolIfLessThan ========
 */
int_fast16_t RNG_fillPoolIfLessThan(size_t bytes)
{
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;

    if (SemaphoreP_pend(&RNGLPF3RF_instanceData.accessSemaphore, SemaphoreP_WAIT_FOREVER) != SemaphoreP_OK)
    {
        returnValue = RNG_STATUS_RESOURCE_UNAVAILABLE;
    }
    else
    {
        returnValue = RNGLPF3RF_fillPoolIfLessThan(bytes);

        SemaphoreP_post(&RNGLPF3RF_instanceData.accessSemaphore);
    }

    return returnValue;
}

/*
 *  ======== RNG_cancelOperation ========
 */
int_fast16_t RNG_cancelOperation(RNG_Handle handle)
{
    /* Cancel not supported in this implementation since AESCTRDRBG driver does not support cancellation. */
    return RNG_STATUS_ERROR;
}

/*
 *  ======== addCodesToAPTDensities ========
 */
static void RNGLPF3RF_addCodesToAptDensities(uint16_t codes, volatile uint16_t *densities)
{
    /* Counts a set of three codes in the densities array. */
    for (uint_fast8_t i = 0U; i < 3U; i++)
    {
        uint8_t code = codes & 0x0F;
        densities[code]++;
        codes >>= 5;
    }
}

/*!
 * @brief Runs Repetitive Count Tests (RCT).
 *
 * Each word is assumed to contain 3 IA Codes (IAC) and 3 QA Codes (QAC).
 * Each group of codes (IAC and QAC) are treated independently for the purpose of this test.
 *
 * For efficiency, each group of 3 codes is compared to the last group of
 * three codes (the 3 new IACs are compared to the last 3 IACs in one
 * comparison operation, same for the QACs).
 *
 * @param lastWord          is the previous word from noise data
 * @param newWord           is the current word from noise data.
 * @param countRepeatedIac  the current count of repeated IAC codes.
 * @param countRepeatedQac  the current count of repeated QAC codes.
 *
 * @retval #RNG_STATUS_RCT_FAIL                  RCT Failed
 * @retval #RNG_STATUS_SUCCESS                   RCT Passed
 *
 */
/*
 *  ======== executeRCT ========
 */
static inline int_fast16_t RNGLPF3RF_executeRct(uint32_t lastWord,
                                                uint32_t newWord,
                                                size_t *countRepeatedIac,
                                                size_t *countRepeatedQac)
{
    if (((newWord ^ lastWord) & 0x0000FFFF) != 0U)
    {
        *countRepeatedQac = 0U;
    }
    else
    {
        (*countRepeatedQac)++;

        if (*countRepeatedQac >= RNGLPF3RF_rctThreshold)
        {
            return RNG_STATUS_RCT_FAIL;
        }
    }

    if (((newWord ^ lastWord) & 0xFFFF0000) != 0U)
    {
        *countRepeatedIac = 0U;
    }
    else
    {
        (*countRepeatedIac)++;

        if (*countRepeatedIac >= RNGLPF3RF_rctThreshold)
        {
            return RNG_STATUS_RCT_FAIL;
        }
    }
    return RNG_STATUS_SUCCESS;
}

/*!
 * @brief Runs Adaptive Proportion Tests (APT).
 *
 * These tests are a modified version of the test described in NIST SP 800-90B.
 * This implementation is more conservative than the implementation described in
 * the NIST document. The NIST specification only considers a single code value
 * within a window of 512 values. This test considers all code values
 * within that window (or slightly more since codes are loaded in
 * 6 code sequences). Thus, this implementation is more likely
 * to detect a failure of the entropy source than the NIST specified
 * test.
 *
 * In addition, this test also considers a bimodal threshold to
 * detect instances where the entropy source is cycling between
 * two distinct code values such that the two values are much
 * more common in the sequence that expected.
 *
 * @param densities is a collection of 4-bit code counts over a 512 (+ 5) code window.
 *
 * @retval #RNG_STATUS_APT_FAIL                  APT Failed
 * @retval #RNG_STATUS_APT_BIMODAL_FAIL          APT Bimodal Failed
 * @retval #RNG_STATUS_SUCCESS                   Both APT and APT Bimodal passed
 */
/*
 *  ======== executeAPT ========
 */
static inline int_fast16_t RNGLPF3RF_executeApt(volatile uint16_t *densities)
{
    uint8_t populationsAboveBimodalLimit = 0U;

    for (uint_fast8_t j = 0U; j < 16U; j++)
    {
        if (densities[j] > RNGLPF3RF_aptThreshold)
        {
            return RNG_STATUS_APT_FAIL;
        }

        if (densities[j] > RNGLPF3RF_aptBimodalThreshold)
        {
            if (populationsAboveBimodalLimit > 0U)
            {
                return RNG_STATUS_APT_BIMODAL_FAIL;
            }
            else
            {
                populationsAboveBimodalLimit++;
            }
        }
        densities[j] = 0;
    }

    return RNG_STATUS_SUCCESS;
}

/*!
 * @brief Performs Health Checks on the noise buffer from RCL before conditioning.
 *
 * This function performs 2 Health Checks - Repetitive Count Test (RCT) and Adaptive Proportion Test (APT).
 * These tests are a modified version of the tests described in NIST SP 800-90B. RCT is performed if
 * RNGLPF3RF_rctEnabled is true and apt is performed if RNGLPF3RF_aptEnabled is true
 *
 * The noiseData should not be used if this function returns an error code.
 *
 * @param  noiseData A pointer to the buffer containing noise input from RCL
 *                      A word of input noise data follows the following format
 *                      Bit 31: 0
 *                      Bit 30..26: 5-bit I Arithmetic Code Reading X+2
 *                      Bit 25..21: 5-bit I Arithmetic Code Reading X+1
 *                      Bit 20..16: 5-bit I Arithmetic Code Reading X
 *                      Bit 15: 0
 *                      Bit 14..10: 5-bit Q Arithmetic Code X+2
 *                      Bit  9..5 :  5-bit Q Arithmetic Code X+1
 *                      Bit  4..0 :  5-bit Q Arithmetic Code X
 *
 * @retval #RNG_STATUS_RCT_FAIL                  RCT Failed, making the noise input invalid
 * @retval #RNG_STATUS_APT_FAIL                  APT Failed, making the noise input invalid
 * @retval #RNG_STATUS_APT_BIMODAL_FAIL          APT Bimodal Failed, making the noise input invalid
 * @retval #RNG_STATUS_SUCCESS                   All Health Checks passed, making the noise input valid
 */
static int_fast16_t RNGLPF3RF_entropyHealthTests(uint32_t *noiseData)
{
    volatile uint16_t densities[32];
    uint32_t newWord;
    uint16_t iaCodes;
    uint16_t qaCodes;
    int_fast16_t returnValue   = RNG_STATUS_SUCCESS;
    size_t numCodesInAptWindow = 0U;
    size_t countRepeatedIac    = 0U;
    size_t countRepeatedQac    = 0U;
    uint32_t lastWord          = 0xFFFFFFFF;

    memset((void *)densities, 0x0, 32 * sizeof(uint16_t));

    for (size_t i = 0U; i < RNGLPF3RF_noiseInputWordLen; i++)
    {
        newWord = noiseData[i];

        if (RNGLPF3RF_rctEnabled)
        {
            returnValue = RNGLPF3RF_executeRct(lastWord, newWord, &countRepeatedIac, &countRepeatedQac);
            if (returnValue != RNG_STATUS_SUCCESS)
            {
                return returnValue;
            }
            lastWord = newWord;
        }

        if (RNGLPF3RF_aptEnabled)
        {
            qaCodes = newWord & 0xFFFF;
            RNGLPF3RF_addCodesToAptDensities(qaCodes, densities);
            iaCodes = newWord >> 16;
            RNGLPF3RF_addCodesToAptDensities(iaCodes, densities);

            numCodesInAptWindow += 6U;

            if (numCodesInAptWindow >= RNG_APT_WINDOW_SIZE)
            {
                returnValue = RNGLPF3RF_executeApt(densities);
                if (returnValue != RNG_STATUS_SUCCESS)
                {
                    return returnValue;
                }
                numCodesInAptWindow = 0U;
            }
        }
    }
    return returnValue;
}
