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

#include <ti/drivers/RNG.h>
#include <ti/drivers/rng/RNGLPF3HSM.h>
#include <ti/drivers/TRNG.h>
#include <ti/drivers/trng/TRNGLPF3HSM.h>
#include <ti/drivers/RNG.h>
#include <ti/drivers/cryptoutils/utils/CryptoUtils.h>
#include <ti/drivers/cryptoutils/hsm/HSMLPF3.h>
#include <ti/drivers/cryptoutils/hsm/HSMLPF3Utility.h>
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

static bool RNGLPF3HSM_isInitialized = false;

/*** Prototypes ***/
static int_fast16_t RNGLPF3HSM_generateEntropyInternal(RNG_Handle handle,
                                                       uintptr_t outputBuffer,
                                                       size_t bytesToGenerate);
static int_fast16_t RNGLPF3HSM_generateEntropy(RNG_Handle handle, void *randomBytes, size_t randomBytesSize);
static bool RNGLPF3HSM_checkRange(RNGLPF3HSM_OperationParameters *opParams);
static int_fast16_t RNGLPF3HSM_getValidatedNumber(RNG_Handle handle,
                                                  void *randomNumber,
                                                  size_t randomNumberBitLength,
                                                  CryptoUtils_Endianess endianess,
                                                  RNGLPF3HSM_validator validator,
                                                  const void *lowerLimit,
                                                  const void *upperLimit);

/*
 *  ======== RNG_Params_init ========
 */
void RNG_Params_init(RNG_Params *params)
{
    *params                = RNG_defaultParams;
    params->returnBehavior = RNGLPF3HSM_returnBehavior;
}

/*
 *  ======== RNG_init ========
 */
int_fast16_t RNG_init(void)
{
    if (RNGLPF3HSM_isInitialized == false)
    {
        HSMLPF3_constructRTOSObjects();

        RNGLPF3HSM_isInitialized = true;
    }

    return RNG_STATUS_SUCCESS;
}

/*
 *  ======== RNG_construct ========
 */
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

        /* Initialize and boot HSM and related FW architectures */
        if (HSMLPF3_init() != HSMLPF3_STATUS_SUCCESS)
        {
            return NULL;
        }

        /* If params are NULL, use defaults */
        if (params == NULL)
        {
            object->timeout = RNG_defaultParams.timeout;
        }
        else
        {
            object->timeout = params->timeout;
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

/*
 *  ======== RNGLPF3HSM_generateEntropyInternal ========
 */
static int_fast16_t RNGLPF3HSM_generateEntropyInternal(RNG_Handle handle,
                                                       uintptr_t outputBuffer,
                                                       size_t bytesToGenerate)
{
    int_fast16_t status    = RNG_STATUS_ERROR;
    int_fast16_t hsmRetval = HSMLPF3_STATUS_ERROR;
    int32_t tokenResult    = 0U;

    /* Populates the HSMLPF3 commandToken as a RNG get DRBG random number operation */
    HSMLPF3_constructRNGGetRandomNumberPhysicalToken(outputBuffer, bytesToGenerate);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)RNGLPF3HSM_returnBehavior, NULL, (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Handles post command token submission mechanism.
         * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers post-processing
         * fxn) and returns immediately when in callback mode.
         */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            tokenResult = HSMLPF3_getResultCode();

            if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
            {
                status = RNG_STATUS_SUCCESS;
            }
        }
    }

    return status;
}

/*
 *  ======== RNGLPF3HSM_generateEntropy ========
 */
static int_fast16_t RNGLPF3HSM_generateEntropy(RNG_Handle handle, void *randomBytes, size_t randomBytesSize)
{
    RNGLPF3HSM_Object *object                   = handle->object;
    int_fast16_t status                         = RNG_STATUS_ERROR;
    uintptr_t outputBuffer                      = 0U;
    uint8_t finalBlock[HSM_DRBG_RNG_BLOCK_SIZE] = {0};
    size_t bytesToGenerate                      = 0U;

    if (randomBytesSize > HSM_RAW_RNG_MAX_LENGTH)
    {
        /* Return error. */
        return RNG_STATUS_INVALID_INPUTS;
    }

    if (!HSMLPF3_acquireLock(object->timeout, (uintptr_t)handle))
    {
        return RNG_STATUS_RESOURCE_UNAVAILABLE;
    }

    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    if (randomBytesSize < HSM_DRBG_RNG_BLOCK_SIZE)
    {
        outputBuffer    = (uintptr_t)finalBlock;
        bytesToGenerate = HSM_DRBG_RNG_BLOCK_SIZE;
    }
    else if (HSM_IS_SIZE_MULTIPLE_OF_WORD(randomBytesSize))
    {
        outputBuffer    = (uintptr_t)randomBytes;
        bytesToGenerate = randomBytesSize;
    }
    else
    {
        outputBuffer    = (uintptr_t)randomBytes;
        bytesToGenerate = randomBytesSize & ~(HSM_DRBG_RNG_BLOCK_SIZE - 1);
    }

    status = RNGLPF3HSM_generateEntropyInternal(handle, outputBuffer, bytesToGenerate);

    if (status == RNG_STATUS_SUCCESS)
    {
        if (randomBytesSize < HSM_DRBG_RNG_BLOCK_SIZE)
        {
            (void)memcpy(randomBytes, &finalBlock[0], randomBytesSize);
        }
        else if (!HSM_IS_SIZE_MULTIPLE_OF_WORD(randomBytesSize))
        {
            uint8_t offset  = bytesToGenerate;
            bytesToGenerate = randomBytesSize - bytesToGenerate;

            status = RNGLPF3HSM_generateEntropyInternal(handle, (uintptr_t)finalBlock, bytesToGenerate);

            if (status == RNG_STATUS_SUCCESS)
            {
                (void)memcpy((uint8_t *)randomBytes + offset, finalBlock, bytesToGenerate);
            }
        }
    }

    HSMLPF3_releaseLock();

    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

    return status;
}

/*
 *  ======== RNGLPF3HSM_checkRange ========
 */
static bool RNGLPF3HSM_checkRange(RNGLPF3HSM_OperationParameters *opParams)
{
    return CryptoUtils_isNumberInRange(opParams->output,
                                       opParams->outputBitLength,
                                       opParams->endianess,
                                       opParams->lowerLimit,
                                       opParams->upperLimit);
}

/*
 *  ======== RNGLPF3HSM_getValidatedNumber ========
 */
static int_fast16_t RNGLPF3HSM_getValidatedNumber(RNG_Handle handle,
                                                  void *randomNumber,
                                                  size_t randomNumberBitLength,
                                                  CryptoUtils_Endianess endianess,
                                                  RNGLPF3HSM_validator validator,
                                                  const void *lowerLimit,
                                                  const void *upperLimit)
{
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;
    size_t byteSize;
    uint8_t *byteDestination;
    uint8_t bitMask;
    bool isValid = false;
    RNGLPF3HSM_OperationParameters opParams;

    if ((handle == NULL) || (randomNumber == NULL) || (randomNumberBitLength >= RNG_MAX_BIT_LENGTH))
    {
        returnValue = RNG_STATUS_INVALID_INPUTS;
    }
    else if (randomNumberBitLength == 0U)
    {
        return returnValue;
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

    while ((returnValue == RNG_STATUS_SUCCESS) && (!isValid))
    {
        returnValue = RNGLPF3HSM_generateEntropy(handle, byteDestination, byteSize);

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

    return returnValue;
}

/*
 *  ======== RNG_getRandomBits ========
 */
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

/*
 *  ======== RNG_getLERandomNumberInRange ========
 */
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

/*
 *  ======== RNG_getBERandomNumberInRange ========
 */
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

/*
 *  ======== RNG_generateKey ========
 */
int_fast16_t RNG_generateKey(RNG_Handle handle, CryptoKey *key)
{
    int_fast16_t returnValue = RNG_STATUS_SUCCESS;
    uint8_t *randomBits;
    size_t randomBitsLength;

    if ((key == NULL) || (key->u.plaintext.keyLength > (RNG_MAX_BIT_LENGTH >> 3u)))
    {
        return RNG_STATUS_INVALID_INPUTS;
    }
    else if ((key->encoding != CryptoKey_BLANK_PLAINTEXT) && (key->encoding != CryptoKey_BLANK_PLAINTEXT_HSM))
    {
        returnValue = RNG_STATUS_INVALID_KEY_ENCODING;
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

    if (key == NULL)
    {
        return RNG_STATUS_INVALID_INPUTS;
    }
    else if ((key->encoding != CryptoKey_BLANK_PLAINTEXT) && (key->encoding != CryptoKey_BLANK_PLAINTEXT_HSM))
    {
        returnValue = RNG_STATUS_INVALID_KEY_ENCODING;
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
    if (key == NULL)
    {
        return RNG_STATUS_INVALID_INPUTS;
    }
    else if ((key->encoding != CryptoKey_BLANK_PLAINTEXT) && (key->encoding != CryptoKey_BLANK_PLAINTEXT_HSM))
    {
        returnValue = RNG_STATUS_INVALID_KEY_ENCODING;
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

/*
 *  ======== RNG_fillPoolIfLessThan ========
 */
int_fast16_t RNG_fillPoolIfLessThan(size_t bytes)
{
    return RNG_STATUS_ERROR;
}

/*
 *  ======== RNG_cancelOperation ========
 */
int_fast16_t RNG_cancelOperation(RNG_Handle handle)
{
    return RNG_STATUS_ERROR;
}
