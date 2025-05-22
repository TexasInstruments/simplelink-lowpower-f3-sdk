/*
 * Copyright (c) 2024-2025, Texas Instruments Incorporated
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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/devices/DeviceFamily.h>

#include <ti/drivers/Power.h>

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    #include <ti/drivers/power/PowerCC27XX.h>
#elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
    #include <ti/drivers/power/PowerWFF3.h>
#endif

#include <ti/drivers/TRNG.h>
#include <ti/drivers/trng/TRNGLPF3HSM.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKey.h>

#include <ti/drivers/cryptoutils/hsm/HSMLPF3.h>
#include <ti/drivers/cryptoutils/hsm/HSMLPF3Utility.h>

#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(driverlib/cpu.h)
#include DeviceFamily_constructPath(driverlib/interrupt.h)

static uint8_t TRNGLPF3HSM_randomDataRawPool[HSM_RAW_RNG_BLOCK_SIZE] = {0};

/*  TRNGLPF3HSM_getRandomRaw()
 *  The underlying function that executes the operation
 *  of getting random entropy data.
 *
 *  IMPORTANT:
 *  If random data is requested to be output to a CryptoKey, the caller
 *  must ensure both the entropyKey and entropyBuffer are non-null values.
 *  If random data is requested to be output to an array/buffer, the caller
 *  must set the entropyKey field to null and ensure the entropyBuffer field
 *  is not null.
 */
static int_fast16_t TRNGLPF3HSM_getRandomRaw(TRNG_Handle handle);

static bool isInitialized = false;

/* !!!!!!!!!!!!! WARNING !!!!!!!!!!!!!
 * The TRNG driver cannot accept a non 32-bit aligned input size
 */

/*
 *  ======== TRNG_init ========
 */
void TRNG_init(void)
{
    if (isInitialized == false)
    {
        HSMLPF3_constructRTOSObjects();

        isInitialized = true;
    }
}

/*
 *  ======== TRNGLPF3HSM_construct ========
 */
TRNG_Handle TRNG_construct(TRNG_Config *config, const TRNG_Params *params)
{
    TRNG_Handle handle;
    TRNGLPF3HSM_Object *object;
    uintptr_t key;

    handle = config;
    object = handle->object;

    key = HwiP_disable();

    /* Initialize and boot HSM and related FW architectures */
    if (HSMLPF3_init() != HSMLPF3_STATUS_SUCCESS)
    {
        /* Upon HSM Boot failure, this driver stores the failure status in the object */
        object->hsmStatus = HSMLPF3_STATUS_ERROR;

        HwiP_restore(key);
        return NULL;
    }
    else
    {
        object->hsmStatus = HSMLPF3_STATUS_SUCCESS;
    }

    if (object->isOpen || !isInitialized)
    {
        HwiP_restore(key);
        return NULL;
    }

    object->isOpen = true;

    HwiP_restore(key);

    /* If params are NULL, use defaults */
    if (params == NULL)
    {
        params = &TRNG_defaultParams;
    }

    /* For callback mode, check if at least one of the callback function pointers is set. */
    if ((params->returnBehavior == TRNG_RETURN_BEHAVIOR_CALLBACK) && (params->cryptoKeyCallbackFxn == NULL) &&
        (params->randomBytesCallbackFxn == NULL))
    {
        return NULL;
    }

    object->returnBehavior         = params->returnBehavior;
    object->semaphoreTimeout       = object->returnBehavior == TRNG_RETURN_BEHAVIOR_BLOCKING ? params->timeout
                                                                                             : SemaphoreP_NO_WAIT;
    object->cryptoKeyCallbackFxn   = params->cryptoKeyCallbackFxn;
    object->randomBytesCallbackFxn = params->randomBytesCallbackFxn;
    object->handle                 = handle;

    return handle;
}

/*
 *  ======== TRNG_close ========
 */
void TRNG_close(TRNG_Handle handle)
{
    TRNGLPF3HSM_Object *object;

    /* Get the pointer to the object and hwAttrs */
    object = handle->object;

    /* Mark the module as available */
    object->isOpen = false;
}

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== TRNGLPF3HSM_reseedHSMPostProcessing ========
 */
static inline void TRNGLPF3HSM_reseedHSMPostProcessing(uintptr_t arg0)
{
    TRNG_Handle handle         = (TRNG_Handle)arg0;
    TRNGLPF3HSM_Object *object = handle->object;
    int_fast16_t status        = TRNG_STATUS_ERROR;
    int32_t tokenResult        = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as many other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = TRNG_STATUS_SUCCESS;
    }

    object->returnStatus = status;

    HSMLPF3_releaseLock();

    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*
 *  ======== TRNGLPF3HSM_reseedHSMAsyncPostProcessing ========
 */
static inline void TRNGLPF3HSM_reseedHSMAsyncPostProcessing(uintptr_t arg0)
{
    HSMLPF3_releaseLock();

    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*
 *  ======== TRNGLPF3HSM_reseedHSM ========
 */
int_fast16_t TRNGLPF3HSM_reseedHSMAsync()
{
    int_fast16_t status    = TRNG_STATUS_ERROR;
    int_fast16_t hsmRetval = HSMLPF3_STATUS_ERROR;

    /* Use a dummy variable as a handle. */
    if (!HSMLPF3_acquireLock(SemaphoreP_NO_WAIT, (uintptr_t)(&isInitialized)))
    {
        return TRNG_STATUS_RESOURCE_UNAVAILABLE;
    }

    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    /* Populates the HSMLPF3 commandToken as an RNG configure operation */
    HSMLPF3_constructRNGReseedDRBGPhysicalToken();

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_CALLBACK,
                                    TRNGLPF3HSM_reseedHSMAsyncPostProcessing,
                                    (uintptr_t)(&isInitialized));

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        status = TRNG_STATUS_SUCCESS;
    }
    else
    {
        HSMLPF3_releaseLock();

        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
    }

    return status;
}

/*
 *  ======== TRNGLPF3HSM_reseedHSM ========
 */
int_fast16_t TRNGLPF3HSM_reseedHSM(TRNG_Handle handle)
{
    TRNGLPF3HSM_Object *object = handle->object;
    int_fast16_t status        = TRNG_STATUS_ERROR;
    int_fast16_t hsmRetval     = HSMLPF3_STATUS_ERROR;

    /* Callback is not allowed. */
    if (object->returnBehavior == TRNG_RETURN_BEHAVIOR_CALLBACK)
    {
        return TRNG_STATUS_INVALID_INPUTS;
    }

    if (!HSMLPF3_acquireLock(object->semaphoreTimeout, (uintptr_t)handle))
    {
        return TRNG_STATUS_RESOURCE_UNAVAILABLE;
    }

    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    /* Populates the HSMLPF3 commandToken as an RNG configure operation */
    HSMLPF3_constructRNGReseedDRBGPhysicalToken();

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    TRNGLPF3HSM_reseedHSMPostProcessing,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Handles post command token submission mechanism.
         * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers post-processing
         * fxn) and returns immediately when in callback mode.
         */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    if (hsmRetval != HSMLPF3_STATUS_SUCCESS)
    {
        HSMLPF3_releaseLock();

        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
    }

    return status;
}

/*
 *  ======== TRNGLPF3HSM_switchNrbgPostProcessing ========
 */
static inline void TRNGLPF3HSM_switchNrbgPostProcessing(uintptr_t arg0)
{
    TRNG_Handle handle         = (TRNG_Handle)arg0;
    TRNGLPF3HSM_Object *object = handle->object;
    int_fast16_t status        = TRNG_STATUS_ERROR;
    int32_t tokenResult        = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as many other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        HSMLPF3_updateInternalNRBGMode();

        status = TRNG_STATUS_SUCCESS;
    }

    object->returnStatus = status;

    HSMLPF3_releaseLock();

    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*
 *  ======== TRNGLPF3HSM_switchNrbgMode ========
 */
int_fast16_t TRNGLPF3HSM_switchNrbgMode(TRNG_Handle handle, TRNGLFP3HSM_NRBGMode nrbgMode)
{
    TRNGLPF3HSM_Object *object       = handle->object;
    int_fast16_t status              = TRNG_STATUS_ERROR;
    int_fast16_t hsmRetval           = HSMLPF3_STATUS_ERROR;
    HSMLPF3_NRBGMode currentNRBGMode = HSMLPF3_getCurrentNRBGMode();

    if (currentNRBGMode == (HSMLPF3_NRBGMode)nrbgMode)
    {
        /* The engine is already set to the desired NRBG type */
        return TRNG_STATUS_ALREADY_IN_SPECIFIED_NRBG_TYPE;
    }

    /* Callback is not allowed. */
    if (object->returnBehavior == TRNG_RETURN_BEHAVIOR_CALLBACK)
    {
        return TRNG_STATUS_INVALID_INPUTS;
    }

    if (!HSMLPF3_acquireLock(object->semaphoreTimeout, (uintptr_t)handle))
    {
        return TRNG_STATUS_RESOURCE_UNAVAILABLE;
    }

    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    /* Populates the HSMLPF3 commandToken as an RNG configure operation */
    HSMLPF3_constructRNGSwitchNRBGWithDefaultsPhysicalToken((HSMLPF3_NRBGMode)nrbgMode);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    TRNGLPF3HSM_switchNrbgPostProcessing,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Handles post command token submission mechanism.
         * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers post-processing
         * fxn) and returns immediately when in callback mode.
         */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    if (hsmRetval != HSMLPF3_STATUS_SUCCESS)
    {
        HSMLPF3_releaseLock();

        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
    }

    return status;
}
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

/*
 *  ======== TRNG_getRandomRawPostProcessing ========
 */
static inline void TRNG_getRandomRawPostProcessing(uintptr_t arg0)
{
    TRNG_Handle handle         = (TRNG_Handle)arg0;
    TRNGLPF3HSM_Object *object = handle->object;
    int_fast16_t status        = TRNG_STATUS_ERROR;
    int32_t tokenResult        = HSMLPF3_getResultCode();

    /* TokenResult carries information regarding the operation result status as well as many other information such as
     * whether the operation is FIPS approved or not. The code below applies an HSMLPF3_RETVAL_MASK to extract only
     * relevant information to an operation's result status.
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
    {
        status = TRNG_STATUS_SUCCESS;

        if (object->entropyRequested >= HSM_RAW_RNG_BLOCK_SIZE)
        {
            /* If raw entropy requested is a block-size multiple, nothing needs to be done but cleanup. */
            object->entropyGenerated = object->entropyRequested;
        }
        else
        {
            /* If raw entropy requested is less than a block size, copy entropy from refilled pool into destination
             * address.
             */
            memcpy(object->entropyBuffer, TRNGLPF3HSM_randomDataRawPool, object->entropyRequested);
            memset((void *)&TRNGLPF3HSM_randomDataRawPool[0], 0, object->entropyRequested);

            object->entropyGenerated = object->entropyRequested;
        }

        if (object->entropyKey != NULL)
        {
            if (object->entropyKey->encoding == CryptoKey_BLANK_PLAINTEXT)
            {
                object->entropyKey->encoding = CryptoKey_PLAINTEXT;
            }
            else
            {
                object->entropyKey->encoding = CryptoKey_PLAINTEXT_HSM;
            }
        }
    }

    if (object->returnStatus != TRNG_STATUS_CANCELED)
    {
        object->returnStatus = status;

        HSMLPF3_releaseLock();

        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

        if (object->returnBehavior == TRNG_RETURN_BEHAVIOR_CALLBACK)
        {
            if (object->entropyKey != NULL)
            {
                object->cryptoKeyCallbackFxn((TRNG_Handle)arg0, status, object->entropyKey);
            }
            else
            {
                object->randomBytesCallbackFxn((TRNG_Handle)arg0,
                                               status,
                                               object->entropyBuffer,
                                               object->entropyRequested);
            }
        }
    }
}

/*
 *  ======== TRNG_getRandomRaw ========
 */
static int_fast16_t TRNGLPF3HSM_getRandomRaw(TRNG_Handle handle)
{
    TRNGLPF3HSM_Object *object = handle->object;
    int_fast16_t status        = TRNG_STATUS_ERROR;
    int_fast16_t hsmRetval     = HSMLPF3_STATUS_ERROR;
    size_t rawDataBlocksNum    = object->entropyRequested / HSM_RAW_RNG_BLOCK_SIZE;
    uintptr_t outputBuffer     = (uintptr_t)object->entropyBuffer;

    if (!HSMLPF3_acquireLock(object->semaphoreTimeout, (uintptr_t)handle))
    {
        return TRNG_STATUS_RESOURCE_UNAVAILABLE;
    }

    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    object->rawEntropyBlocksRequested = rawDataBlocksNum == 0U ? 1U : rawDataBlocksNum;
    outputBuffer = rawDataBlocksNum == 0U ? (uintptr_t)&TRNGLPF3HSM_randomDataRawPool[0] : outputBuffer;

    /* Populates the HSMLPF3 commandToken as a TRNG get raw random number operation */
    HSMLPF3_constructRNGGetRawRandomNumberPhysicalToken(outputBuffer, object->rawEntropyBlocksRequested);

    /* Submit token to the HSM IP engine */
    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    TRNG_getRandomRawPostProcessing,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        /* Handles post command token submission mechanism.
         * Waits for a result token from the HSM IP in polling and blocking modes (and calls the drivers post-processing
         * fxn) and returns immediately when in callback mode.
         */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            status = object->returnStatus;
        }
    }

    if (hsmRetval != HSMLPF3_STATUS_SUCCESS)
    {
        HSMLPF3_releaseLock();

        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
    }

    return status;
}

/*
 *  ======== TRNG_generateEntropy ========
 */
int_fast16_t TRNG_generateEntropy(TRNG_Handle handle, CryptoKey *entropy)
{
    TRNGLPF3HSM_Object *object = handle->object;

    if ((entropy == NULL) || (entropy->u.plaintext.keyMaterial == NULL))
    {
        return TRNG_STATUS_INVALID_INPUTS;
    }
    else if ((entropy->encoding != CryptoKey_BLANK_PLAINTEXT) && (entropy->encoding != CryptoKey_BLANK_PLAINTEXT_HSM))
    {
        return TRNG_STATUS_INVALID_KEY_ENCODING;
    }
    else if ((entropy->u.plaintext.keyLength == 0) ||
             ((entropy->u.plaintext.keyLength > HSM_RAW_RNG_BLOCK_SIZE) &&
              (!HSM_IS_SIZE_MULTIPLE_OF_RAW_BLOCK(entropy->u.plaintext.keyLength))) ||
             (entropy->u.plaintext.keyLength > HSM_RAW_RNG_MAX_LENGTH))
    {
        /* Given that the HSM IP returns n number of 256-bytes blocks for raw entropy, Two limitation apply:
         * 1. keyLength cannot be 0.
         * 2. keyLength can be any number from 1 to 256 Bytes but has to be a multiple of 256 beyond that.
         *      Ex. keyLength can be: 1, 2, 5, 33, 65, 128, 256, 512, 768, 1024....
         *      But cannot be for example: 257, 258, or 1025.
         */
        return TRNG_STATUS_INVALID_INPUT_SIZE;
    }

    object->returnStatus = TRNG_STATUS_SUCCESS;

    object->entropyGenerated = 0U;
    object->entropyKey       = entropy;
    object->entropyBuffer    = entropy->u.plaintext.keyMaterial;
    object->entropyRequested = entropy->u.plaintext.keyLength;

    return TRNGLPF3HSM_getRandomRaw(handle);
}

/*
 *  ======== TRNG_generateKey ========
 */
int_fast16_t TRNG_generateKey(TRNG_Handle handle, CryptoKey *entropy)
{
    TRNGLPF3HSM_Object *object = handle->object;

    if ((entropy == NULL) || (entropy->u.plaintext.keyMaterial == NULL))
    {
        return TRNG_STATUS_INVALID_INPUTS;
    }
    else if ((entropy->encoding != CryptoKey_BLANK_PLAINTEXT) && (entropy->encoding != CryptoKey_BLANK_PLAINTEXT_HSM))
    {
        return TRNG_STATUS_INVALID_KEY_ENCODING;
    }
    else if ((entropy->u.plaintext.keyLength == 0) ||
             ((entropy->u.plaintext.keyLength < HSM_RAW_RNG_BLOCK_SIZE) &&
              (!HSM_IS_SIZE_MULTIPLE_OF_WORD(entropy->u.plaintext.keyLength))) ||
             ((entropy->u.plaintext.keyLength > HSM_RAW_RNG_BLOCK_SIZE) &&
              (!HSM_IS_SIZE_MULTIPLE_OF_RAW_BLOCK(entropy->u.plaintext.keyLength))) ||
             (entropy->u.plaintext.keyLength > HSM_RAW_RNG_MAX_LENGTH))
    {
        /* Given that the HSM IP returns n number of 256-bytes blocks for raw entropy, Two limitation apply:
         * 1. keyLength cannot be 0.
         * 2. keyLength from 1 to 256 Bytes, keyLength has to be 32-bit multiple.
         * 3. keyLength > 256 Bytes, keyLength has to be 256-byte multiple.
         *      Ex. keyLength can be: 1, 2, 5, 33, 65, 128, 256, 512, 768, 1024....
         *      But cannot be for example: 257, 258, or 1025.
         */
        return TRNG_STATUS_INVALID_INPUT_SIZE;
    }

    object->returnStatus = TRNG_STATUS_SUCCESS;

    object->entropyGenerated = 0;
    object->entropyKey       = entropy;
    object->entropyBuffer    = entropy->u.plaintext.keyMaterial;
    object->entropyRequested = entropy->u.plaintext.keyLength;

    return TRNGLPF3HSM_getRandomRaw(handle);
}

/*
 *  ======== TRNG_getRandomBytes ========
 */
int_fast16_t TRNG_getRandomBytes(TRNG_Handle handle, void *randomBytes, size_t randomBytesSize)
{
    TRNGLPF3HSM_Object *object = handle->object;

    if (randomBytes == NULL)
    {
        return TRNG_STATUS_INVALID_INPUTS;
    }
    else if ((randomBytesSize == 0) ||
             ((randomBytesSize > HSM_RAW_RNG_BLOCK_SIZE) && (!HSM_IS_SIZE_MULTIPLE_OF_RAW_BLOCK(randomBytesSize))) ||
             (randomBytesSize > HSM_RAW_RNG_MAX_LENGTH))
    {
        /* Given that the HSM IP returns n number of 256-bytes blocks for raw entropy, Two limitation apply:
         * 1. keyLength cannot be 0.
         * 2. keyLength can be any number from 1 to 256 Bytes but has to be a multiple of 256 beyond that.
         *      Ex. keyLength can be: 1, 2, 5, 33, 65, 128, 256, 512, 768, 1024....
         *      But cannot be for example: 257, 258, or 1025.
         */
        return TRNG_STATUS_INVALID_INPUT_SIZE;
    }

    object->returnStatus = TRNG_STATUS_SUCCESS;

    object->entropyGenerated = 0;
    object->entropyKey       = NULL;
    object->entropyBuffer    = randomBytes;
    object->entropyRequested = randomBytesSize;

    return TRNGLPF3HSM_getRandomRaw(handle);
}

/*
 *  ======== TRNG_cancelOperation ========
 */
int_fast16_t TRNG_cancelOperation(TRNG_Handle handle)
{
    TRNGLPF3HSM_Object *object = handle->object;
    uintptr_t key;

    key = HwiP_disable();

    object->returnStatus = TRNG_STATUS_CANCELED;

    /* Since the HSM cannot cancel an in-progress token, we must wait for the result to allow for
     * subsequent token submissions to succeed.
     */
    (void)HSMLPF3_cancelOperation();

    if (object->entropyKey != NULL)
    {
        (void)memset(object->entropyKey->u.plaintext.keyMaterial, 0, object->entropyRequested);
    }
    else
    {
        (void)memset(object->entropyBuffer, 0, object->entropyRequested);
    }

    HwiP_restore(key);

    if (object->returnBehavior == TRNG_RETURN_BEHAVIOR_CALLBACK)
    {
        if (object->entropyKey != NULL)
        {
            object->cryptoKeyCallbackFxn(handle, TRNG_STATUS_CANCELED, object->entropyKey);
        }
        else
        {
            object->randomBytesCallbackFxn(handle,
                                           TRNG_STATUS_CANCELED,
                                           object->entropyBuffer,
                                           object->entropyRequested);
        }
    }

    return TRNG_STATUS_SUCCESS;
}
