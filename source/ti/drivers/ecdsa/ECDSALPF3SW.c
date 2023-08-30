/*
 * Copyright (c) 2021-2023, Texas Instruments Incorporated
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

#include <ti/drivers/ECDSA.h>
#include <ti/drivers/ecdsa/ECDSALPF3SW.h>
#include <ti/drivers/RNG.h>
#include <ti/drivers/rng/RNGLPF3RF.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKeyPlaintext.h>
#include <ti/drivers/cryptoutils/ecc/ECCInitLPF3SW.h>
#include <ti/drivers/cryptoutils/ecc/ECCParams.h>
#include <ti/drivers/cryptoutils/utils/CryptoUtils.h>

#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/HwiP.h>

#include <third_party/ecc/include/ECCSW.h>

/* Defines */

/*
 *  ======== ECDSA_init ========
 */
void ECDSA_init(void)
{
    /* Do nothing */
}

/*
 *  ======== ECDSA_close ========
 */
void ECDSA_close(ECDSA_Handle handle)
{
    DebugP_assert(handle);

    ECDSALPF3SW_Object *object = handle->object;

    /* Mark the module as available */
    object->isOpen = false;
}

/*
 *  ======== ECDSA_construct ========
 */
ECDSA_Handle ECDSA_construct(ECDSA_Config *config, const ECDSA_Params *params)
{
    DebugP_assert(config);

    ECDSA_Handle handle        = NULL;
    ECDSALPF3SW_Object *object = config->object;
    uintptr_t key;

    /* If params are NULL, use defaults */
    if (params == NULL)
    {
        params = &ECDSA_defaultParams;
    }

    /*
     * Since LPF3SW ECC is a pure SW implementation, callback return behavior
     * is not supported.
     */
    if (params->returnBehavior != ECDSA_RETURN_BEHAVIOR_CALLBACK)
    {
        key = HwiP_disable();

        if (object->isOpen)
        {
            HwiP_restore(key);
        }
        else
        {
            object->isOpen = true;

            HwiP_restore(key);

            object->returnBehavior = params->returnBehavior;

            handle = (ECDSA_Handle)config;
        }
    }

    return handle;
}

/*
 *  ======== ECDSA_sign ========
 */
int_fast16_t ECDSA_sign(ECDSA_Handle handle, ECDSA_OperationSign *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    CryptoKey pmsnKey;
    ECDSALPF3SW_Object *object = handle->object;
    int_fast16_t returnStatus  = ECDSA_STATUS_ERROR;
    int_fast16_t rngStatus;
    RNG_Config rngConfig;
    RNG_Handle rngHandle;
    RNG_Params rngParams;
    RNGLPF3RF_Object rngObject = {0};
    uint8_t eccStatus;
    size_t curveLength     = operation->curve->length;
    size_t curveWordLength = curveLength / sizeof(uint32_t);

    /*
     * Allocate local copies of the private and public keys because the ECC
     * library implementation requires the word length to be prepended to every
     * array input.
     */
    ECC_Param privateKeyUnion;
    ECC_Param pmsnUnion;
    ECC_Param hashUnion;
    ECC_Param rUnion;
    ECC_Param sUnion;

    /* Create a blank CryptoKey to store the per-message secret number (PMSN) */
    CryptoKeyPlaintext_initBlankKey(&pmsnKey, &pmsnUnion.byte[ECC_LENGTH_PREFIX_BYTES], curveLength);

    /*
     * Note: For CC23X0, RNG must be initialized by application in a task context with interrupts enabled
     * using the following steps, before using ECDSA_sign() and prior to the use of the Radio.
     * 1. Read radio noise using RCL_AdcNoise_get_samples_blocking(). This RCL function must
     *    be called from a task context with interrupts enabled and therefore cannot be called
     *    by startup code. This must be executed prior to the use of the radio.
     * 2. Condition the noise to seed the RNG using RNGLPF3RF_conditionNoiseToGenerateSeed().
     * 3. Initialize the RNG from the application with RNG_init()
     * RNG_init() need not be called again here or by any other code.
     */

    rngConfig.object  = &rngObject;
    rngConfig.hwAttrs = NULL;

    /*
     * Initialize the RNG params. The RNG driver return behavior will be
     * automatically set to the correct value which may or may not match
     * ECDSA driver's return behavior.
     */
    RNG_Params_init(&rngParams);

    if (curveLength == ECCParams_NISTP256_LENGTH)
    {
        /* Initialize object with NIST-P256 curve */
        ECCInitLPF3SW_NISTP256(&object->eccState, ECDSALPF3SW_ECC_WINDOW_SIZE, object->eccWorkZone);
    }
    else if (curveLength == ECCParams_NISTP224_LENGTH)
    {
        /* Initialize object with NIST-P224 curve */
        ECCInitLPF3SW_NISTP224(&object->eccState, ECDSALPF3SW_ECC_WINDOW_SIZE, object->eccWorkZone);
    }
    else
    {
        return ECDSA_STATUS_ERROR;
    }

    /* Prepend the word length for ECC Library */
    privateKeyUnion.word[0] = curveWordLength;
    pmsnUnion.word[0]       = curveWordLength;
    hashUnion.word[0]       = curveWordLength;
    rUnion.word[0]          = curveWordLength;
    sUnion.word[0]          = curveWordLength;

    rngHandle = RNG_construct(&rngConfig, &rngParams);

    if (rngHandle != NULL)
    {
        /* Try generating a new PMSN until it's within in range [1, n-1] */
        do
        {
            /*
             * Get a random PMSN. curveLength is in bytes
             * so multiply by 8 for the number of random bits.
             */
            rngStatus = RNG_getRandomBits(rngHandle, pmsnKey.u.plaintext.keyMaterial, curveLength * 8);

            if (rngStatus != RNG_STATUS_SUCCESS)
            {
                RNG_close(rngHandle);
                return ECDSA_STATUS_ERROR;
            }

            /* Check if PMSN in [1, n-1] */
            eccStatus = ECCSW_validatePrivateKeyWeierstrass(&object->eccState, pmsnUnion.word);
        } while (eccStatus != STATUS_PRIVATE_VALID);

        RNG_close(rngHandle);

        /*
         * Since we are receiving the private and public keys in octet string
         * format, we need to convert them to little-endian for use with the
         * ECC library functions.
         */
        CryptoUtils_reverseCopyPad(operation->hash, &hashUnion.word[1], curveLength);

        CryptoUtils_reverseCopyPad(operation->myPrivateKey->u.plaintext.keyMaterial,
                                   &privateKeyUnion.word[1],
                                   curveLength);

        eccStatus = ECCSW_ECDSASign(&object->eccState,
                                    privateKeyUnion.word,
                                    hashUnion.word,
                                    pmsnUnion.word,
                                    rUnion.word,
                                    sUnion.word);

        /*
         * Check the ECC SW library return code for success and set the driver
         * status accordingly. ECCSW_ECDSASign has numerous error return codes
         * which do not directly map to driver return codes thus no translation
         * is done here.
         */
        if (eccStatus == STATUS_ECDSA_SIGN_OK)
        {
            /*
             * Now that we have created r and s, we need to copy them back in
             * reverse byte order since the ECC library implementation generates
             * little-endian values.
             */
            CryptoUtils_reverseCopy(&rUnion.word[1], operation->r, curveLength);

            CryptoUtils_reverseCopy(&sUnion.word[1], operation->s, curveLength);

            returnStatus = ECDSA_STATUS_SUCCESS;
        }
    }

    return returnStatus;
}

/*
 *  ======== ECDSA_verify ========
 */
int_fast16_t ECDSA_verify(ECDSA_Handle handle, ECDSA_OperationVerify *operation)
{
    DebugP_assert(handle);
    DebugP_assert(operation);

    ECDSALPF3SW_Object *object = handle->object;
    int_fast16_t returnStatus  = ECDSA_STATUS_ERROR;
    uint8_t eccStatus;
    size_t curveLength     = operation->curve->length;
    size_t curveWordLength = curveLength / sizeof(uint32_t);

    /* Validate key sizes to make sure octet string format is used */
    if ((operation->theirPublicKey->u.plaintext.keyLength != (2 * operation->curve->length + OCTET_STRING_OFFSET)) ||
        (operation->theirPublicKey->u.plaintext.keyMaterial[0] != 0x04))
    {
        return ECDSA_STATUS_INVALID_KEY_SIZE;
    }

    /*
     * Allocate local copies of the private and public keys because the ECC
     * library implementation requires the word length to be prepended to every
     * array input.
     */
    ECC_Param publicKeyUnionX;
    ECC_Param publicKeyUnionY;
    ECC_Param hashUnion;
    ECC_Param rUnion;
    ECC_Param sUnion;

    if (curveLength == ECCParams_NISTP256_LENGTH)
    {
        /* Initialize object with NIST-P256 curve */
        ECCInitLPF3SW_NISTP256(&object->eccState, ECDSALPF3SW_ECC_WINDOW_SIZE, object->eccWorkZone);
    }
    else if (curveLength == ECCParams_NISTP224_LENGTH)
    {
        /* Initialize object with NIST-P224 curve */
        ECCInitLPF3SW_NISTP224(&object->eccState, ECDSALPF3SW_ECC_WINDOW_SIZE, object->eccWorkZone);
    }
    else
    {
        return ECDSA_STATUS_ERROR;
    }

    /* Prepend the word length for ECC Library */
    publicKeyUnionX.word[0] = curveWordLength;
    publicKeyUnionY.word[0] = curveWordLength;
    hashUnion.word[0]       = curveWordLength;
    rUnion.word[0]          = curveWordLength;
    sUnion.word[0]          = curveWordLength;

    /*
     * Since we are receiving the private and public keys in octet string
     * format, we need to convert them to little-endian for use with the
     * ECC library functions.
     */
    CryptoUtils_reverseCopyPad(operation->hash, &hashUnion.word[1], curveLength);

    CryptoUtils_reverseCopyPad(operation->r, &rUnion.word[1], curveLength);

    CryptoUtils_reverseCopyPad(operation->s, &sUnion.word[1], curveLength);

    CryptoUtils_reverseCopyPad(operation->theirPublicKey->u.plaintext.keyMaterial + OCTET_STRING_OFFSET,
                               &publicKeyUnionX.word[1],
                               curveLength);

    CryptoUtils_reverseCopyPad(operation->theirPublicKey->u.plaintext.keyMaterial + curveLength + OCTET_STRING_OFFSET,
                               &publicKeyUnionY.word[1],
                               curveLength);

    /* Verify r in range [1, n-1] where n is the order of the curve */
    eccStatus = ECCSW_validatePrivateKeyWeierstrass(&object->eccState, rUnion.word);

    if (eccStatus == STATUS_PRIVATE_VALID)
    {
        /* Verify s in range [1, n-1] where n is the order of the curve */
        eccStatus = ECCSW_validatePrivateKeyWeierstrass(&object->eccState, sUnion.word);

        if (eccStatus != STATUS_PRIVATE_VALID)
        {
            returnStatus = ECDSA_STATUS_S_LARGER_THAN_ORDER;
        }
    }
    else
    {
        returnStatus = ECDSA_STATUS_R_LARGER_THAN_ORDER;
    }

    if (eccStatus == STATUS_PRIVATE_VALID)
    {
        eccStatus = ECCSW_validatePublicKeyWeierstrass(&object->eccState, publicKeyUnionX.word, publicKeyUnionY.word);
    }

    if (eccStatus == STATUS_ECC_POINT_ON_CURVE)
    {
        /* Call ECC library to perform verification */
        eccStatus = ECCSW_ECDSAVerify(&object->eccState,
                                      publicKeyUnionX.word,
                                      publicKeyUnionY.word,
                                      hashUnion.word,
                                      rUnion.word,
                                      sUnion.word);

        /* Check the ECC library return code and set the driver status accordingly */
        if (eccStatus == STATUS_ECDSA_VALID_SIGNATURE)
        {
            returnStatus = ECDSA_STATUS_SUCCESS;
        }
    }

    return returnStatus;
}
