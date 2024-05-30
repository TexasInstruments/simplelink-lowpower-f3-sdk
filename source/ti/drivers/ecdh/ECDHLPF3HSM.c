/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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

#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/ECDH.h>
#include <ti/drivers/ecdh/ECDHLPF3HSM.h>
#include <ti/drivers/cryptoutils/ecc/ECCParams.h>
#include <ti/drivers/cryptoutils/utils/CryptoUtils.h>
#include <ti/drivers/cryptoutils/hsm/HSMLPF3.h>
#include <ti/drivers/Power.h>

#include <third_party/hsmddk/include/Integration/Adapter_DriverInit/incl/api_driver_init.h>
#include <third_party/hsmddk/include/Integration/Adapter_VEX/incl/adapter_vex.h>
#include <third_party/hsmddk/include/Integration/HSMSAL/HSMSAL.h>
#include <third_party/hsmddk/include/Integration/Adapter_Generic/incl/adapter_interrupts.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_hsmcrypto.h)

/* Defines the size of the largest array necessary to hold both components
 * of a base point (for use in ecc multiplication), including the necessary
 * padded words. NISTP521 requires 17 words per component of a point, plus the HSM
 * requires 3 words of padding between the two components.
 */
#define ECDHLPF3HSM_ECC_POINT_LENGTH_BYTES 148

#define ECDHLPF3HSM_MAX_PRIVATE_KEY_SIZE 66
/* Defines the amount of padding that is necessary between the two components of
 * the base point in a ECC multiplication, so that the HSM will load it correctly
 * via DMA
 */
#define PADDING_BYTES_EVEN               8
#define PADDING_BYTES_ODD                12

/* Result token value for HSM ECC multiplication operations that yield the point
 * at infinity
 */
#define HSM_POINT_AT_INFINITY 0x01

/* Static globals */
static bool isInitialized = false;

static uint8_t basePoint[ECDHLPF3HSM_ECC_POINT_LENGTH_BYTES];
static uint8_t resultPoint[ECDHLPF3HSM_ECC_POINT_LENGTH_BYTES];

/* Forward declarations */
static void ECDHLPF3HSM_formatCurve25519PrivateKey(uint8_t *keyMaterial);
static void ECDHLPF3HSM_getUncompressedHSMPoint(ECDH_Handle handle);
static void ECDHLPF3HSM_errorCleanup(ECDH_Handle handle);
static void ECDHLPF3HSM_formatKeyResult(ECDH_Handle handle, CryptoKey *key);
static uint8_t ECDHLPF3HSM_getPaddingLength(size_t paramLength);
static inline uint32_t ECDHLPF3HSM_getBasePointLength(ECDH_Handle handle);
static int_fast16_t ECDHLPF3HSM_loadPKA(ECDH_Handle handle, const uint8_t *buffer, uint8_t index, uint8_t length);
static int_fast16_t ECDHLPF3HSM_acquireAndLoadPKA(ECDH_Handle handle);
static void ECDHLPF3HSM_eccMultPostProcess(uintptr_t driverHandle);
static int_fast16_t ECDHLPF3HSM_submitECCMultiplicationToken(ECDH_Handle handle);
static int_fast16_t ECDHLPF3HSM_releasePKA(ECDH_Handle handle);

/*
 *  ======== ECDHLPF3HSM_formatCurve25519PrivateKey ========
 */
static void ECDHLPF3HSM_formatCurve25519PrivateKey(uint8_t *keyMaterial)
{
    /* As per RFC 7748, the private key of Curve25519 is pruned so that
     * the three LSB's are cleared, bit 255 is cleared, and bit 254 is
     * set.
     */
    /* Clear bit 0, 1, and 2 */
    keyMaterial[0] &= 0xF8;
    /* Clear bit 255 */
    keyMaterial[31] &= 0x7F;
    /* Set bit 254 */
    keyMaterial[31] |= 0x40;
}

/*
 *  ======== ECDHLPF3HSM_getUncompressedHSMPoint ========
 */
static void ECDHLPF3HSM_getUncompressedHSMPoint(ECDH_Handle handle)
{
    ECDHLPF3HSM_Object *object = handle->object;

    const ECCParams_CurveParams *curve;
    size_t paramLength;
    uint8_t numWords;
    size_t paddingLength;
    uint8_t generatorYOffset;
    uint8_t *theirPublicKey;

    if (object->operationType == ECDH_OPERATION_TYPE_GENERATE_PUBLIC_KEY)
    {
        curve       = object->operation.generatePublicKey->curve;
        paramLength = curve->length;
        /* Round up the number of words required to store the curve parameters */
        numWords    = (uint8_t)((paramLength + 3) / 4);
    }
    else
    {
        curve       = object->operation.computeSharedSecret->curve;
        paramLength = curve->length;
        /* Round up the number of words required to store the curve parameters */
        numWords    = (uint8_t)((paramLength + 3) / 4);
        if (object->operation.computeSharedSecret->keyMaterialEndianness == ECDH_BIG_ENDIAN_KEY)
        {
            theirPublicKey = object->operation.computeSharedSecret->theirPublicKey->u.plaintext.keyMaterial +
                             OCTET_STRING_OFFSET;
        }
        else
        {
            theirPublicKey = object->operation.computeSharedSecret->theirPublicKey->u.plaintext.keyMaterial;
        }
    }

    if (paramLength == ECDHLPF3HSM_MAX_PRIVATE_KEY_SIZE)
    {
        paramLength += HSM_PADDED_PARAM_BYTES;
    }

    if ((numWords & 0x01U) == 0U)
    {
        paddingLength = PADDING_BYTES_EVEN;
    }
    else
    {
        paddingLength = PADDING_BYTES_ODD;
    }

    generatorYOffset = paramLength + paddingLength;

    /* The HSM expects padding words between the X and Y components
     * of the generator. The basePoint buffer allocates space to account
     * for this.
     */
    if (object->operationType == ECDH_OPERATION_TYPE_GENERATE_PUBLIC_KEY)
    {
        (void)memset(basePoint, 0, sizeof(basePoint));
        (void)memcpy(basePoint, curve->generatorX, paramLength);
        (void)memcpy(basePoint + generatorYOffset, curve->generatorY, paramLength);
    }
    else
    {
        (void)memset(basePoint, 0, sizeof(basePoint));
        (void)memcpy(basePoint, theirPublicKey, curve->length);
        /* Public keys stored in the operation struct do not have any padding between
         * key components
         */
        (void)memcpy(basePoint + generatorYOffset, theirPublicKey + curve->length, curve->length);
    }
}

/*
 *  ======== ECDHLPF3HSM_errorCleanup ========
 */
static void ECDHLPF3HSM_errorCleanup(ECDH_Handle handle)
{
    ECDHLPF3HSM_Object *object = handle->object;

    const ECCParams_CurveParams *curve;
    uint8_t *privKey;
    uint8_t *theirPubKey;

    /* Set key to the user's private key that has to be reverted to big-endian format,
     * since there was an error after already byte-wise reversing its data
     */
    if (object->operationType == ECDH_OPERATION_TYPE_GENERATE_PUBLIC_KEY)
    {
        privKey = object->operation.generatePublicKey->myPrivateKey->u.plaintext.keyMaterial;

        if (object->operation.generatePublicKey->keyMaterialEndianness == ECDH_BIG_ENDIAN_KEY)
        {
            curve = object->operation.generatePublicKey->curve;
            CryptoUtils_reverseBufferBytewise(privKey, curve->length);
        }
    }
    else
    {
        privKey = object->operation.computeSharedSecret->myPrivateKey->u.plaintext.keyMaterial;

        theirPubKey = object->operation.computeSharedSecret->theirPublicKey->u.plaintext.keyMaterial;

        if (object->operation.computeSharedSecret->keyMaterialEndianness == ECDH_BIG_ENDIAN_KEY)
        {
            curve = object->operation.computeSharedSecret->curve;

            CryptoUtils_reverseBufferBytewise(privKey, curve->length);

            /* Reverse X component */
            CryptoUtils_reverseBufferBytewise(theirPubKey + OCTET_STRING_OFFSET, curve->length);

            /* Reverse Y component */
            CryptoUtils_reverseBufferBytewise(theirPubKey + OCTET_STRING_OFFSET + curve->length, curve->length);
        }
    }

    HSMLPF3_releaseLock();

    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*
 *  ======== ECDHLPF3HSM_formatKeyResult ========
 */
static void ECDHLPF3HSM_formatKeyResult(ECDH_Handle handle, CryptoKey *key)
{
    ECDHLPF3HSM_Object *object = handle->object;
    const ECCParams_CurveParams *curve;
    ECDH_KeyMaterialEndianness keyMaterialEndianness;
    uint8_t *keyMaterial;
    uint8_t *xCoordinate;
    uint8_t *yCoordinate;
    uint8_t paddingLength;
    uint8_t resultPointOffsetY;

    if (object->operationType == ECDH_OPERATION_TYPE_GENERATE_PUBLIC_KEY)
    {
        curve                 = object->operation.generatePublicKey->curve;
        keyMaterialEndianness = object->operation.generatePublicKey->keyMaterialEndianness;
    }
    else
    {
        curve                 = object->operation.computeSharedSecret->curve;
        keyMaterialEndianness = object->operation.computeSharedSecret->keyMaterialEndianness;
    }

    xCoordinate = NULL;
    yCoordinate = NULL;

    /* Since keystore is not yet supported, and the key format has already been verified,
     * we can set keyMaterial directly to the plaintext material.
     */
    keyMaterial = key->u.plaintext.keyMaterial;

    paddingLength      = ECDHLPF3HSM_getPaddingLength(curve->length);
    resultPointOffsetY = curve->length + paddingLength;

    if (curve->length == ECDHLPF3HSM_MAX_PRIVATE_KEY_SIZE)
    {
        resultPointOffsetY += HSM_PADDED_PARAM_BYTES;
    }

    if (keyMaterialEndianness == ECDH_BIG_ENDIAN_KEY)
    {
        /* Set first byte of output to 0x04 to indicate x,y
         * big-endian coordinates in octet string format
         */
        keyMaterial[0] = OCTET_STRING_PREFIX;
        xCoordinate    = keyMaterial + OCTET_STRING_OFFSET;
        yCoordinate    = keyMaterial + curve->length + OCTET_STRING_OFFSET;

        /* The X and Y components need to be concatenated together to provide the result without the padding that
         * the HSM requires between them
         */
        (void)memcpy(xCoordinate, resultPoint, curve->length);
        (void)memcpy(yCoordinate, resultPoint + resultPointOffsetY, curve->length);

        /* Byte-reverse integer X coordinate for octet string format. Note that the HSM result is in
         * little-endian format.
         */
        CryptoUtils_reverseBufferBytewise(xCoordinate, curve->length);

        if (curve->curveType == ECCParams_CURVE_TYPE_MONTGOMERY)
        {
            /* Zero-out the Y coordinate as it not required for Montgomery curves */
            (void)memset(yCoordinate, 0x00, curve->length);
        }
        else
        {
            /* Byte-reverse integer Y coordinate for octet string format */
            CryptoUtils_reverseBufferBytewise(yCoordinate, curve->length);
        }
    }
    else
    {
        xCoordinate = keyMaterial;
        (void)memcpy(xCoordinate, resultPoint, curve->length);

        /* Y coordinate is not required for Montgomery curves */
        if (curve->curveType != ECCParams_CURVE_TYPE_MONTGOMERY)
        {
            yCoordinate = keyMaterial + curve->length;
            (void)memcpy(yCoordinate, resultPoint + resultPointOffsetY, curve->length);
        }
    }
}

/*
 *  ======== ECDHLPF3HSM_getPaddingLength ========
 */
static uint8_t ECDHLPF3HSM_getPaddingLength(size_t paramLength)
{
    /* Round up the number of words required to store the curve parameters */
    uint8_t numWords = (uint8_t)((paramLength + 3) / 4);
    uint8_t paddingLength;

    if ((numWords & 0x01U) == 0U)
    {
        paddingLength = PADDING_BYTES_EVEN;
    }
    else
    {
        paddingLength = PADDING_BYTES_ODD;
    }

    return paddingLength;
}

/*
 *  ======== ECDHLPF3HSM_getBasePointLength ========
 */
static inline uint32_t ECDHLPF3HSM_getBasePointLength(ECDH_Handle handle)
{
    ECDHLPF3HSM_Object *object = handle->object;
    size_t paramLength;
    uint32_t basePointLength;
    uint8_t paddingLength;

    if (object->operationType == ECDH_OPERATION_TYPE_GENERATE_PUBLIC_KEY)
    {
        paramLength = object->operation.generatePublicKey->curve->length;
    }
    else
    {
        paramLength = object->operation.computeSharedSecret->curve->length;
    }

    paddingLength = ECDHLPF3HSM_getPaddingLength(paramLength);

    basePointLength = (uint32_t)((2 * paramLength) + paddingLength);

    if (paramLength == ECDHLPF3HSM_MAX_PRIVATE_KEY_SIZE)
    {
        /* Multiply by two to apply padding for both components */
        basePointLength += (HSM_PADDED_PARAM_BYTES * 2);
    }

    return basePointLength;
}

/*
 *  ======== ECDHLPF3HSM_loadPKA ========
 */
static int_fast16_t ECDHLPF3HSM_loadPKA(ECDH_Handle handle, const uint8_t *buffer, uint8_t index, uint8_t length)
{
    int_fast16_t status    = ECDH_STATUS_ERROR;
    int_fast16_t hsmRetval = HSMLPF3_STATUS_ERROR;

    HSMLPF3_constructECDHnumLoadPhysicalToken(buffer, index, length);

    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING, NULL, (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            if ((HSMLPF3_getResultCode() & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
            {
                status = ECDH_STATUS_SUCCESS;
            }
        }
    }
    return status;
}

/*
 *  ======== ECDHLPF3HSM_acquireAndLoadPKA ========
 */
static int_fast16_t ECDHLPF3HSM_acquireAndLoadPKA(ECDH_Handle handle)
{
    ECDHLPF3HSM_Object *object = handle->object;
    const ECCParams_CurveParams *curve;
    uint8_t *keyMaterial;
    int_fast16_t status    = ECDH_STATUS_ERROR;
    int_fast16_t hsmRetval = HSMLPF3_STATUS_ERROR;
    uint8_t tokenNum;
    const uint8_t *buffer;
    uint8_t length;

    if (object->operationType == ECDH_OPERATION_TYPE_GENERATE_PUBLIC_KEY)
    {
        curve       = object->operation.generatePublicKey->curve;
        keyMaterial = object->operation.generatePublicKey->myPrivateKey->u.plaintext.keyMaterial;
    }
    else
    {
        curve       = object->operation.computeSharedSecret->curve;
        keyMaterial = object->operation.computeSharedSecret->myPrivateKey->u.plaintext.keyMaterial;
    }

    HSMLPF3_constructECDHnumSetPhysicalToken(((curve->length + 3) / 4));

    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING, NULL, (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            if ((HSMLPF3_getResultCode() & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
            {
                status = ECDH_STATUS_SUCCESS;
            }
        }
    }

    if (hsmRetval != HSMLPF3_STATUS_SUCCESS)
    {
        return status;
    }

    /* NISTP521 curves have a parameter length of 66 bytes. Round to 68, as the PK Number
     * Array requires loaded values to be a multiple of 4 bytes. Note that this is the only
     * curve supported with this issue.
     */
    if ((curve->length & 0x03) == 0)
    {
        length = curve->length;
    }
    else
    {
        length = curve->length + HSM_PADDED_PARAM_BYTES;
    }

    /* Now we must load the PK Number Array with the curve parameters and ECC multiplication arguments */
    for (tokenNum = 0U; tokenNum < 4U; tokenNum++)
    {
        switch (tokenNum)
        {
            case 0U:
                buffer = curve->prime;
                break;
            case 1U:
                buffer = curve->a;
                break;
            case 2U:
                buffer = curve->b;
                break;
            case 3U:
                buffer = keyMaterial;
                break;
            default:
                break;
        }

        status = ECDHLPF3HSM_loadPKA(handle, buffer, tokenNum, length);

        if (status != ECDH_STATUS_SUCCESS)
        {
            return status;
        }
    }

    return status;
}

/*
 *  ======== ECDHLPF3HSM_eccMultPostProcess ========
 */
static void ECDHLPF3HSM_eccMultPostProcess(uintptr_t driverHandle)
{
    ECDHLPF3HSM_Object *object = ((ECDH_Handle)driverHandle)->object;
    CryptoKey *key;
    uint8_t *keyToReverse;
    size_t curveLength;
    int32_t tokenResult = HSMLPF3_getResultCode();

    if (object->operationType == ECDH_OPERATION_TYPE_GENERATE_PUBLIC_KEY)
    {
        key = object->operation.generatePublicKey->myPublicKey;

        /* We must revert changes made to the user's CryptoKey material since we flipped it
         * in-place
         */
        if (object->operation.generatePublicKey->keyMaterialEndianness == ECDH_BIG_ENDIAN_KEY)
        {
            curveLength  = object->operation.generatePublicKey->curve->length;
            keyToReverse = object->operation.generatePublicKey->myPrivateKey->u.plaintext.keyMaterial;
            CryptoUtils_reverseBufferBytewise(keyToReverse, curveLength);
        }
    }
    else
    {
        key = object->operation.computeSharedSecret->sharedSecret;

        /* We must revert changes made to the user's CryptoKey material since we flipped it
         * in-place
         */
        if (object->operation.computeSharedSecret->keyMaterialEndianness == ECDH_BIG_ENDIAN_KEY)
        {
            curveLength  = object->operation.computeSharedSecret->curve->length;
            /* Reverse private key */
            keyToReverse = object->operation.computeSharedSecret->myPrivateKey->u.plaintext.keyMaterial;
            CryptoUtils_reverseBufferBytewise(keyToReverse, curveLength);

            /* Reverse X and Y components of big-endian public key that were provided */
            keyToReverse = object->operation.computeSharedSecret->theirPublicKey->u.plaintext.keyMaterial;
            CryptoUtils_reverseBufferBytewise(keyToReverse + OCTET_STRING_OFFSET, curveLength);
            CryptoUtils_reverseBufferBytewise(keyToReverse + OCTET_STRING_OFFSET + curveLength, curveLength);
        }
    }

    /* Check the output token result, particularly for a POINT_AT_INFINITY result
     * that is not considered an error by the HSM
     */
    if ((tokenResult & HSMLPF3_RETVAL_MASK) == HSM_POINT_AT_INFINITY)
    {
        object->returnStatus = ECDH_STATUS_POINT_AT_INFINITY;
    }
    else if ((tokenResult & HSMLPF3_RETVAL_MASK) > 0U)
    {
        object->returnStatus = ECDH_STATUS_ERROR;
    }
    else
    {
        /* Format key result */
        ECDHLPF3HSM_formatKeyResult((ECDH_Handle)driverHandle, key);

        key->encoding = CryptoKey_PLAINTEXT;

        object->returnStatus = ECDH_STATUS_SUCCESS;
    }

    if (ECDHLPF3HSM_releasePKA((ECDH_Handle)driverHandle) != ECDH_STATUS_SUCCESS)
    {
        object->returnStatus = ECDH_STATUS_ERROR;
    }

    (void)memset(basePoint, 0, ECDHLPF3HSM_ECC_POINT_LENGTH_BYTES);

    (void)memset(resultPoint, 0, ECDHLPF3HSM_ECC_POINT_LENGTH_BYTES);

    HSMLPF3_releaseLock();

    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

    object->operationInProgress = false;

    if (object->returnBehavior == ECDH_RETURN_BEHAVIOR_CALLBACK)
    {
        object->callbackFxn((ECDH_Handle)driverHandle, object->returnStatus, object->operation, object->operationType);
    }
}

/*
 *  ======== ECDHLPF3HSM_submitECCMultiplicationToken ========
 */
static int_fast16_t ECDHLPF3HSM_submitECCMultiplicationToken(ECDH_Handle handle)
{
    ECDHLPF3HSM_Object *object = handle->object;
    uint32_t basePointLength   = ECDHLPF3HSM_getBasePointLength(handle);
    int_fast16_t status        = ECDH_STATUS_ERROR;
    int_fast16_t hsmRetval     = HSMLPF3_STATUS_ERROR;
    uintptr_t key;
    uint8_t operation;

    if (object->operationType == ECDH_OPERATION_TYPE_GENERATE_PUBLIC_KEY)
    {
        if (object->operation.generatePublicKey->curve->curveType == ECCParams_CURVE_TYPE_MONTGOMERY)
        {
            operation = VEXTOKEN_PK_OP_ECMONTMUL;
        }
        else
        {
            operation = VEXTOKEN_PK_OP_ECCMUL;
        }
    }
    else
    {
        if (object->operation.computeSharedSecret->curve->curveType == ECCParams_CURVE_TYPE_MONTGOMERY)
        {
            operation = VEXTOKEN_PK_OP_ECMONTMUL;
        }
        else
        {
            operation = VEXTOKEN_PK_OP_ECCMUL;
        }
    }

    HSMLPF3_constructECDHPKAOperationPhysicalToken(operation, basePoint, resultPoint, basePointLength);

    /* Starting the operation and setting object->operationInProgress must be atomic */
    key = HwiP_disable();

    hsmRetval = HSMLPF3_submitToken((HSMLPF3_ReturnBehavior)object->returnBehavior,
                                    ECDHLPF3HSM_eccMultPostProcess,
                                    (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        object->operationInProgress = true;

        HwiP_restore(key);

        /* Will call the driver's post-processing fxn in the case of polling and blocking */
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            /* object->returnStatus would reflect an error state if it was found in
             * the postProcessFxn
             */
            status = object->returnStatus;
        }
    }
    else
    {
        HwiP_restore(key);
    }
    return status;
}

/*
 *  ======== ECDHLPF3HSM_releasePKA ========
 */
static int_fast16_t ECDHLPF3HSM_releasePKA(ECDH_Handle handle)
{
    int_fast16_t status    = ECDH_STATUS_ERROR;
    int_fast16_t hsmRetval = HSMLPF3_STATUS_ERROR;

    HSMLPF3_constructECDHnumSetPhysicalToken(0);

    hsmRetval = HSMLPF3_submitToken(HSMLPF3_RETURN_BEHAVIOR_POLLING, NULL, (uintptr_t)handle);

    if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
    {
        hsmRetval = HSMLPF3_waitForResult();

        if (hsmRetval == HSMLPF3_STATUS_SUCCESS)
        {
            if ((HSMLPF3_getResultCode() & HSMLPF3_RETVAL_MASK) == EIP130TOKEN_RESULT_SUCCESS)
            {
                status = ECDH_STATUS_SUCCESS;
            }
        }
    }

    return status;
}

/*
 *  ======== ECDH_init ========
 */
void ECDH_init(void)
{
    HSMLPF3_constructRTOSObjects();

    isInitialized = true;
}

/*
 *  ======== ECDH_Params_init ========
 */
void ECDH_Params_init(ECDH_Params *params)
{
    *params = ECDH_defaultParams;
}

/*
 *  ======== ECDH_construct ========
 */
ECDH_Handle ECDH_construct(ECDH_Config *config, const ECDH_Params *params)
{
    ECDH_Handle handle;
    ECDHLPF3HSM_Object *object;
    uint_fast8_t key;

    handle = (ECDH_Handle)config;
    object = handle->object;

    key = HwiP_disable();

    if (!isInitialized || object->isOpen)
    {
        HwiP_restore(key);
        return NULL;
    }

    object->isOpen              = true;
    object->operationInProgress = false;
    object->returnStatus        = ECDH_STATUS_SUCCESS;

    HwiP_restore(key);

    /* If params are NULL, use defaults */
    if (params == NULL)
    {
        params = (ECDH_Params *)&ECDH_defaultParams;
    }

    /* Since there is no alternative hardware accelerator for ECDH,
     * we can return a NULL handle
     */
    if (HSMLPF3_init() != HSMLPF3_STATUS_SUCCESS)
    {
        return NULL;
    }

    object->returnBehavior = params->returnBehavior;
    object->callbackFxn    = params->callbackFxn;
    object->accessTimeout  = params->timeout;

    Power_setDependency(PowerLPF3_PERIPH_HSM);

    return handle;
}

/*
 *  ======== ECDH_close ========
 */
void ECDH_close(ECDH_Handle handle)
{
    ECDHLPF3HSM_Object *object = handle->object;

    /* This is true only if in callback mode and waiting on a result */
    if (object->operationInProgress)
    {
        ECDH_cancelOperation(handle);
    }

    object->isOpen = false;
}

/*
 *  ======== ECDH_generatePublicKey ========
 */
int_fast16_t ECDH_generatePublicKey(ECDH_Handle handle, ECDH_OperationGeneratePublicKey *operation)
{
    ECDHLPF3HSM_Object *object = handle->object;
    uint8_t *myPrivateKeyMaterial;
    size_t myPrivateKeyLength;
    size_t myPublicKeyLength;
    int_fast16_t status;

    /*
     * KeyStore keys are not yet supported in the ECDH driver
     */
    if (operation->myPrivateKey->encoding == CryptoKey_PLAINTEXT)
    {
        myPrivateKeyMaterial = operation->myPrivateKey->u.plaintext.keyMaterial;
        myPrivateKeyLength   = operation->myPrivateKey->u.plaintext.keyLength;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (operation->myPrivateKey->encoding == CryptoKey_KEYSTORE)
    {
        return ECDH_STATUS_KEYSTORE_ERROR;
    }
#endif
    else
    {
        return ECDH_STATUS_ERROR;
    }

    if (operation->myPublicKey->encoding == CryptoKey_BLANK_PLAINTEXT)
    {
        myPublicKeyLength = operation->myPublicKey->u.plaintext.keyLength;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (operation->myPublicKey->encoding == CryptoKey_BLANK_KEYSTORE)
    {
        return ECDH_STATUS_KEYSTORE_ERROR;
    }
#endif
    else
    {
        return ECDH_STATUS_ERROR;
    }

    /* Validate public key sizes to ensure X-only public key format if using Montgomery curves
     * with little endian key representation. Other cases use both coordinates with additional
     * octet string format byte when using big endian representation
     */
    if (operation->keyMaterialEndianness == ECDH_LITTLE_ENDIAN_KEY)
    {
        if (operation->curve->curveType == ECCParams_CURVE_TYPE_MONTGOMERY)
        {
            /* X-only public key for Montgomery curves */
            if ((myPrivateKeyLength != operation->curve->length) || (myPublicKeyLength != operation->curve->length))
            {
                return ECDH_STATUS_INVALID_KEY_SIZE;
            }
        }
        else /* Little-endian keys for other curves */
        {
            if ((myPrivateKeyLength != operation->curve->length) || (myPublicKeyLength != 2 * operation->curve->length))
            {
                return ECDH_STATUS_INVALID_KEY_SIZE;
            }
        }
    }
    else /* Validate key sizes to make sure octet string format is used */
    {
        if ((myPrivateKeyLength != operation->curve->length) ||
            (myPublicKeyLength != 2 * operation->curve->length + OCTET_STRING_OFFSET))
        {
            return ECDH_STATUS_INVALID_KEY_SIZE;
        }
    }

    /* We must verify that the private key is in the interval [1, n -1] */
    if (CryptoUtils_isBufferAllZeros(myPrivateKeyMaterial, operation->curve->length))
    {
        return ECDH_STATUS_PRIVATE_KEY_ZERO;
    }

    /* If we are receiving the private keys in octet string format,
     * we need to convert them to little-endian form for use with the HSM.
     * If the private key is already in little-endian form, no action needed.
     */
    if (operation->keyMaterialEndianness == ECDH_BIG_ENDIAN_KEY)
    {
        /* Note that the octet string format is only for public keys / generator points */
        CryptoUtils_reverseBufferBytewise(myPrivateKeyMaterial, operation->curve->length);
    }

    object->operation.generatePublicKey = operation;
    object->operationType               = ECDH_OPERATION_TYPE_GENERATE_PUBLIC_KEY;

    /* As per RFC 7748, the private key of Curve25519 is pruned so that
     * the three LSB's are cleared, bit 255 is cleared, and bit 254 is
     * set.
     */
    if ((operation->curve->curveType == ECCParams_CURVE_TYPE_MONTGOMERY) &&
        (operation->curve->length == ECCParams_CURVE25519_LENGTH))
    {
        (void)ECDHLPF3HSM_formatCurve25519PrivateKey(myPrivateKeyMaterial);
    }

    /* NOTE: Verification that the private key is less than the upper bound
     * is not yet supported.
     */

    if (!HSMLPF3_acquireLock(object->accessTimeout, (uintptr_t)handle))
    {
        /* If we're unable to complete the operation, we should undo changes made
         * to the key material buffer
         */
        if (operation->keyMaterialEndianness == ECDH_BIG_ENDIAN_KEY)
        {
            CryptoUtils_reverseBufferBytewise(myPrivateKeyMaterial, operation->curve->length);
        }
        return ECDH_STATUS_RESOURCE_UNAVAILABLE;
    }

    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    status = ECDHLPF3HSM_acquireAndLoadPKA(handle);

    if (status != ECDH_STATUS_SUCCESS)
    {
        ECDHLPF3HSM_errorCleanup(handle);

        return status;
    }

    /* Populate the base point with the generator, in the HSM's desired format */
    ECDHLPF3HSM_getUncompressedHSMPoint(handle);

    status = ECDHLPF3HSM_submitECCMultiplicationToken(handle);

    if (status != ECDH_STATUS_SUCCESS)
    {
        ECDHLPF3HSM_errorCleanup(handle);

        return status;
    }

    return status;
}

/*
 *  ======== ECDH_computeSharedSecret ========
 */
int_fast16_t ECDH_computeSharedSecret(ECDH_Handle handle, ECDH_OperationComputeSharedSecret *operation)
{
    ECDHLPF3HSM_Object *object = handle->object;

    uint8_t *myPrivateKeyMaterial;
    size_t myPrivateKeyLength;
    uint8_t *theirPublicKeyMaterial;
    size_t theirPublicKeyLength;
    size_t sharedSecretKeyLength;
    int_fast16_t status;

    /* KeyStore keys are not yet supported in the ECDH driver */
    if (operation->myPrivateKey->encoding == CryptoKey_PLAINTEXT)
    {
        myPrivateKeyMaterial = operation->myPrivateKey->u.plaintext.keyMaterial;
        myPrivateKeyLength   = operation->myPrivateKey->u.plaintext.keyLength;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (operation->myPrivateKey->encoding == CryptoKey_KEYSTORE)
    {
        return ECDH_STATUS_KEYSTORE_ERROR;
    }
#endif
    else
    {
        return ECDH_STATUS_ERROR;
    }

    if (operation->theirPublicKey->encoding == CryptoKey_PLAINTEXT)
    {
        theirPublicKeyMaterial = operation->theirPublicKey->u.plaintext.keyMaterial;
        theirPublicKeyLength   = operation->theirPublicKey->u.plaintext.keyLength;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (operation->theirPublicKey->encoding == CryptoKey_KEYSTORE)
    {
        return ECDH_STATUS_KEYSTORE_ERROR;
    }
#endif
    else
    {
        return ECDH_STATUS_ERROR;
    }

    if (operation->sharedSecret->encoding == CryptoKey_BLANK_PLAINTEXT)
    {
        sharedSecretKeyLength = operation->sharedSecret->u.plaintext.keyLength;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (operation->sharedSecret->encoding == CryptoKey_BLANK_KEYSTORE)
    {
        return ECDH_STATUS_KEYSTORE_ERROR;
    }
#endif
    else
    {
        return ECDH_STATUS_ERROR;
    }

    /* Validate public key sizes to ensure X-only public key format if using Montgomery curves
     * with little endian key representation. Other cases use both coordinates with additional
     * octet string offset byte when using big endian representation
     */
    if (operation->keyMaterialEndianness == ECDH_LITTLE_ENDIAN_KEY)
    {
        /* X-only public key for Montgomery curves */
        if (operation->curve->curveType == ECCParams_CURVE_TYPE_MONTGOMERY)
        {
            if ((myPrivateKeyLength != operation->curve->length) ||
                (theirPublicKeyLength != operation->curve->length) ||
                (sharedSecretKeyLength != operation->curve->length))
            {
                return ECDH_STATUS_INVALID_KEY_SIZE;
            }
        }
        else /* Little-endian keys for other curves */
        {
            if ((myPrivateKeyLength != operation->curve->length) ||
                (theirPublicKeyLength != 2 * operation->curve->length) ||
                (sharedSecretKeyLength != 2 * operation->curve->length))
            {
                return ECDH_STATUS_INVALID_KEY_SIZE;
            }
        }
    }
    else /* Validate key sizes to make sure octet string format is used for short Weierstrass curves */
    {
        if ((myPrivateKeyLength != operation->curve->length) ||
            (theirPublicKeyLength != 2 * operation->curve->length + OCTET_STRING_OFFSET) ||
            (theirPublicKeyMaterial[0] != OCTET_STRING_PREFIX) ||
            (sharedSecretKeyLength != 2 * operation->curve->length + OCTET_STRING_OFFSET))
        {
            return ECDH_STATUS_INVALID_KEY_SIZE;
        }
    }

    /* Convert keys in octet string format to little-endian form for use with the PKA
     * If the keys is already in little-endian form, skip this conversion
     * and directly copy the key.
     */
    if (operation->keyMaterialEndianness == ECDH_BIG_ENDIAN_KEY)
    {
        CryptoUtils_reverseBufferBytewise(myPrivateKeyMaterial, operation->curve->length);

        /* Reverse X component */
        CryptoUtils_reverseBufferBytewise(theirPublicKeyMaterial + OCTET_STRING_OFFSET, operation->curve->length);

        /* Reverse Y component */
        CryptoUtils_reverseBufferBytewise(theirPublicKeyMaterial + OCTET_STRING_OFFSET + operation->curve->length,
                                          operation->curve->length);
    }

    object->operation.computeSharedSecret = operation;
    object->operationType                 = ECDH_OPERATION_TYPE_COMPUTE_SHARED_SECRET;

    /* As per RFC 7748, Curve25519 will mask the most significant bit
     * of the final byte for received u-coordinates.
     *
     * Furthermore, the private key of Curve25519 is pruned so that
     * the three LSB's are cleared, bit 255 is cleared, and bit 254 is
     * set.
     */
    if ((operation->curve->curveType == ECCParams_CURVE_TYPE_MONTGOMERY) &&
        (operation->curve->length == ECCParams_CURVE25519_LENGTH))
    {
        (void)ECDHLPF3HSM_formatCurve25519PrivateKey(myPrivateKeyMaterial);

        if (operation->keyMaterialEndianness == ECDH_BIG_ENDIAN_KEY)
        {
            theirPublicKeyMaterial[31 + OCTET_STRING_OFFSET] &= 0x7F;
        }
        else
        {
            theirPublicKeyMaterial[31] &= 0x7F;
        }
    }

    if (!HSMLPF3_acquireLock(object->accessTimeout, (uintptr_t)handle))
    {
        /* If we're unable to complete the operation, we should undo changes made
         * to the key material buffer
         */
        if (operation->keyMaterialEndianness == ECDH_BIG_ENDIAN_KEY)
        {
            CryptoUtils_reverseBufferBytewise(myPrivateKeyMaterial, operation->curve->length);

            /* Reverse X component */
            CryptoUtils_reverseBufferBytewise(theirPublicKeyMaterial + OCTET_STRING_OFFSET, operation->curve->length);

            /* Reverse Y component */
            CryptoUtils_reverseBufferBytewise(theirPublicKeyMaterial + OCTET_STRING_OFFSET + operation->curve->length,
                                              operation->curve->length);
        }
        return ECDH_STATUS_RESOURCE_UNAVAILABLE;
    }

    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    status = ECDHLPF3HSM_acquireAndLoadPKA(handle);

    if (status != ECDH_STATUS_SUCCESS)
    {
        ECDHLPF3HSM_errorCleanup(handle);

        return status;
    }

    /* Populate the base point with the generator (theirPublicKey),
     * in the HSM's desired format
     */
    ECDHLPF3HSM_getUncompressedHSMPoint(handle);

    status = ECDHLPF3HSM_submitECCMultiplicationToken(handle);

    if (status != ECDH_STATUS_SUCCESS)
    {
        ECDHLPF3HSM_errorCleanup(handle);

        return status;
    }

    return status;
}

/*
 *  ======== ECDH_cancelOperation ========
 */
int_fast16_t ECDH_cancelOperation(ECDH_Handle handle)
{
    ECDHLPF3HSM_Object *object = handle->object;

    (void)memset(basePoint, 0, ECDHLPF3HSM_ECC_POINT_LENGTH_BYTES);

    (void)memset(resultPoint, 0, ECDHLPF3HSM_ECC_POINT_LENGTH_BYTES);

    if (!object->operationInProgress)
    {
        /* Do nothing */
    }
    else
    {
        object->operationInProgress = false;

        (void)HSMLPF3_cancelOperation();

        ECDHLPF3HSM_releasePKA(handle);

        if (object->returnBehavior == ECDH_RETURN_BEHAVIOR_CALLBACK)
        {
            object->callbackFxn(handle, ECDH_STATUS_CANCELED, object->operation, object->operationType);
        }
    }

    return ECDH_STATUS_SUCCESS;
}