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

/* !!!!!!!!!!!!! WARNING !!!!!!!!!!!!!
 * For CC27XX implementation, please refer to SHA2LPF3HSM.c instead
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#include <ti/drivers/SHA2.h>
#include <ti/drivers/sha2/SHA2LPF3SW.h>
#include <ti/drivers/cryptoutils/utils/CryptoUtils.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/hapi.h)

/* Outer and inner padding bytes used in HMAC */
#define HMAC_OPAD_WORD 0x5C5C5C5Cu
#define HMAC_IPAD_WORD 0x36363636u

/* Forward declarations */
static SHA2LPF3SW_Object *SHA2LPF3SW_getObject(SHA2_Handle handle);

static void SHA2LPF3SW_xorBufferWithWord(uint32_t *buffer, size_t bufferLength, uint32_t word);

/*
 *  ======== SHA2LPF3SW_getObject ========
 */
static inline SHA2LPF3SW_Object *SHA2LPF3SW_getObject(SHA2_Handle handle)
{
    return (SHA2LPF3SW_Object *)handle->object;
}

/*
 *  ======== SHA2LPF3SW_xorBufferWithWord ========
 */
static void SHA2LPF3SW_xorBufferWithWord(uint32_t *buffer, size_t bufferLength, uint32_t word)
{
    size_t i;

    for (i = 0; i < bufferLength; i++)
    {
        buffer[i] = buffer[i] ^ word;
    }
}

/*
 *  ======== SHA2_init ========
 */
void SHA2_init(void)
{
    /* No actions are needed for init. */
}

/*
 *  ======== SHA2_construct ========
 */
SHA2_Handle SHA2_construct(SHA2_Config *config, const SHA2_Params *params)
{
    SHA2_Handle handle;
    SHA2LPF3SW_Object *object;
    uintptr_t key;

    handle = (SHA2_Config *)config;
    object = SHA2LPF3SW_getObject(handle);

    key = HwiP_disable();

    if (object->isOpen == true)
    {
        HwiP_restore(key);
        handle = NULL;
    }
    else
    {
        object->isOpen = true;

        HwiP_restore(key);

        if (params == NULL)
        {
            params = &SHA2_defaultParams;
        }

        /* This implementation only supports SHA256 */
        if (params->hashType != SHA2_HASH_TYPE_256)
        {
            object->isOpen = false;
            handle         = NULL;
        }

        /* This implementation only supports POLLING and BLOCKING mode
         * Note that blocking mode is emulated and behaves exactly the same
         * as polling mode.
         */
        if (params->returnBehavior == SHA2_RETURN_BEHAVIOR_CALLBACK)
        {
            object->isOpen = false;
            handle         = NULL;
        }
    }

    return handle;
}

/*
 *  ======== SHA2_close ========
 */
void SHA2_close(SHA2_Handle handle)
{
    SHA2LPF3SW_Object *object = SHA2LPF3SW_getObject(handle);

    (void)SHA2_reset(handle);

    object->isOpen = false;
}

/*
 *  ======== SHA2_addData ========
 */
int_fast16_t SHA2_addData(SHA2_Handle handle, const void *data, size_t length)
{
    SHA2LPF3SW_Object *object = SHA2LPF3SW_getObject(handle);
    int_fast16_t libStatus    = SHA2SW_STATUS_SUCCESS;

    if (object->sha256swObject.bitsProcessed == 0)
    {
#if DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX
        /* On CC27XX, always return success as HAPI functions are read protected */
        libStatus = SHA2SW_STATUS_SUCCESS;
#else
        libStatus = HapiSha256SwStart(&object->sha256swObject);
#endif
    }

    if (libStatus == SHA2SW_STATUS_SUCCESS)
    {
#if DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX
        /* On CC27XX, always return success as HAPI functions are read protected */
        libStatus = SHA2SW_STATUS_SUCCESS;
#else
        libStatus = HapiSha256SwAddData(&object->sha256swObject, data, length);
#endif
    }

    return (libStatus == SHA2SW_STATUS_SUCCESS) ? SHA2_STATUS_SUCCESS : SHA2_STATUS_ERROR;
}

/*
 *  ======== SHA2_finalize ========
 */
int_fast16_t SHA2_finalize(SHA2_Handle handle, void *digest)
{
    SHA2LPF3SW_Object *object = SHA2LPF3SW_getObject(handle);
    int_fast16_t libStatus;
    uintptr_t digestPtrValue = (uintptr_t)digest;

    /* SHA256SW library's digest output must be word aligned */
    if ((digestPtrValue & 0x03u) == 0u)
    {
#if DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX
        /* On CC27XX, always return success as HAPI functions are read protected */
        libStatus = SHA2SW_STATUS_SUCCESS;
#else
        libStatus = HapiSha256SwFinalize(&object->sha256swObject, digest);
#endif
    }
    else
    {
#if DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX
        /* On CC27XX, always return success as HAPI functions are read protected */
        libStatus = SHA2SW_STATUS_SUCCESS;
#else
        libStatus = HapiSha256SwFinalize(&object->sha256swObject, object->digestBuffer);
#endif
        (void)memcpy(digest, object->digestBuffer, (size_t)SHA2_DIGEST_LENGTH_BYTES_256);
    }

    return (libStatus == SHA2SW_STATUS_SUCCESS) ? SHA2_STATUS_SUCCESS : SHA2_STATUS_ERROR;
}

/*
 *  ======== SHA2_hashData ========
 */
int_fast16_t SHA2_hashData(SHA2_Handle handle, const void *data, size_t dataLength, void *digest)
{
    int_fast16_t returnStatus;

    returnStatus = SHA2_addData(handle, data, dataLength);

    if (returnStatus == SHA2_STATUS_SUCCESS)
    {
        returnStatus = SHA2_finalize(handle, digest);
    }

    return returnStatus;
}

/*
 *  ======== SHA2_setupHmac ========
 *
 *  This function starts an HMAC operation and computes as much of the
 *  intermediate results as it can using only the key.
 *
 *  HMAC requires concatenation of intermediate results and the application's
 *  message. We do not have the memory to do that kind of concatenation nor
 *  would it be runtime efficient to do that much copying.
 *  Instead, we use segmented hashes to start the computation of the hashes
 *  and then add in each segment without moving it in memory.
 *
 *  We can compute all operations where the keying material is required.
 *  That way, we do not need to store the intermediate keying material
 *  for future use but only store the intermediate hash result.
 *
 *  - It computes the intermediate key, k0 based on the input key's length.
 *  - It starts a segmented hash of the k0^opad part of
 *    H(k0 ^ opad || H(k0 ^ ipad || message))
 *      - The intermediate output is saved in SHA2LPF3SW_Object.digestBuffer
 *  - It starts a segmented hash with the k0^ipad part of
 *    H(k0  ^ipad || message)
 *      - The intermediate output is saved by the SHA2 driver as usual in
 *        SHA2LPF3SW_Object.sha256swObject
 */
int_fast16_t SHA2_setupHmac(SHA2_Handle handle, const CryptoKey *key)
{
    uint32_t xorBuffer[(uint32_t)SHA2_BLOCK_SIZE_BYTES_256 / sizeof(uint32_t)];
    SHA2LPF3SW_Object *object = SHA2LPF3SW_getObject(handle);
    int_fast16_t returnStatus;
    int_fast16_t libResult;
    size_t keyLength        = key->u.plaintext.keyLength;
    uint8_t *keyingMaterial = key->u.plaintext.keyMaterial;

    /* Prepare the buffer of the derived key. We set the entire buffer to 0x00
     * so we do not need to pad it to the block size after copying the keying
     * material provided or the hash thereof there.
     */
    (void)memset(xorBuffer, 0x00, (size_t)SHA2_BLOCK_SIZE_BYTES_256);

    /* If the keying material fits in the derived key buffer, copy it there.
     * Otherwise, we need to hash it first and copy the digest there. Since
     * We filled the entire buffer with 0x00, we do not need to pad to the block
     * size.
     */
    if (keyLength <= (size_t)SHA2_BLOCK_SIZE_BYTES_256)
    {
        (void)memcpy((uint8_t *)xorBuffer, keyingMaterial, keyLength);
        returnStatus = SHA2_STATUS_SUCCESS;
    }
    else
    {
        returnStatus = SHA2_hashData(handle, keyingMaterial, keyLength, xorBuffer);
    }

    if (returnStatus == SHA2_STATUS_SUCCESS)
    {
#if DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX
        /* On CC27XX, always return success as HAPI functions are read protected */
        libResult = SHA2SW_STATUS_SUCCESS;
#else
        libResult = HapiSha256SwStart(&object->sha256swObject);
#endif

        if (libResult != SHA2SW_STATUS_SUCCESS)
        {
            returnStatus = SHA2_STATUS_ERROR;
        }
    }

    if (returnStatus == SHA2_STATUS_SUCCESS)
    {
        /* Compute k0 ^ opad */
        SHA2LPF3SW_xorBufferWithWord(xorBuffer, sizeof(xorBuffer) / sizeof(uint32_t), HMAC_OPAD_WORD);

        /* Start a hash of k0 ^ opad.
         * The intermediate result will be stored in the object for later
         * use when the application calls SHA2_addData on its actual message.
         */
        returnStatus = SHA2_addData(handle, xorBuffer, (size_t)SHA2_BLOCK_SIZE_BYTES_256);
    }

    if (returnStatus == SHA2_STATUS_SUCCESS)
    {
        /* Copy the intermediate state of H(k0 ^ opad). */
        (void)memcpy(object->digestBuffer, &object->sha256swObject.digest32[0], (size_t)SHA2_DIGEST_LENGTH_BYTES_256);

        /* Undo k0 ^ opad and construct k0 ^ ipad. Use the memory of k0 instead
         * of allocating a new copy on the stack to save RAM.
         */
        SHA2LPF3SW_xorBufferWithWord(xorBuffer, sizeof(xorBuffer) / sizeof(uint32_t), HMAC_OPAD_WORD ^ HMAC_IPAD_WORD);

/* Reset object state to prepare to start a new hash */
#if DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX
        /* On CC27XX, always return success as HAPI functions are read protected */
        libResult = SHA2SW_STATUS_SUCCESS;
#else
        libResult = HapiSha256SwStart(&object->sha256swObject);
#endif

        if (libResult != SHA2SW_STATUS_SUCCESS)
        {
            returnStatus = SHA2_STATUS_ERROR;
        }
    }

    /* Start a hash of k0 ^ ipad.
     * Use top-level fxn to invoke callback if needed
     */
    if (returnStatus == SHA2_STATUS_SUCCESS)
    {
        returnStatus = SHA2_addData(handle, xorBuffer, (size_t)SHA2_BLOCK_SIZE_BYTES_256);
    }

    return returnStatus;
}

/*
 *  ======== SHA2_finalizeHmac ========
 *
 *  This function completes the HMAC operation once all application data
 *  has been added through SHA_addData().
 *
 *  - It finalizes  H((k0 ^ ipad) || data)
 *  - It adds H((k0 ^ ipad) || data) to the previously started hash that already
 *    includes k0 ^ opad.
 *  - It finalizes H(k0 ^ opad || H((k0 ^ ipad) || data))
 */
int_fast16_t SHA2_finalizeHmac(SHA2_Handle handle, void *hmac)
{
    SHA2LPF3SW_Object *object = SHA2LPF3SW_getObject(handle);
    uint32_t tmpDigest[(size_t)SHA2_DIGEST_LENGTH_BYTES_256 / sizeof(uint32_t)];
    int_fast16_t returnStatus;
    int_fast16_t libResult;

    /* Finalize H((k0 ^ ipad) || data) */
    returnStatus = SHA2_finalize(handle, tmpDigest);

    if (returnStatus == SHA2_STATUS_SUCCESS)
    {
#if DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX
        /* On CC27XX, always return success as HAPI functions are read protected */
        libResult = SHA2SW_STATUS_SUCCESS;
#else
        libResult = HapiSha256SwStart(&object->sha256swObject);
#endif

        if (libResult != SHA2SW_STATUS_SUCCESS)
        {
            returnStatus = SHA2_STATUS_ERROR;
        }
    }

    if (returnStatus == SHA2_STATUS_SUCCESS)
    {
        /* Restore H(k0 ^ opad) intermediate result */
        (void)memcpy(&object->sha256swObject.digest32[0],
                     &object->digestBuffer[0],
                     (size_t)SHA2_DIGEST_LENGTH_BYTES_256);

        /* We already processed one block of input earlier */
        object->sha256swObject.bitsProcessed = (uint32_t)SHA2_BLOCK_SIZE_BYTES_256 << 3u;

        /* Add the temporary digest computed earlier to the current digest */
        returnStatus = SHA2_addData(handle, tmpDigest, (size_t)SHA2_DIGEST_LENGTH_BYTES_256);
    }

    if (returnStatus == SHA2_STATUS_SUCCESS)
    {
        /* Finalize H(k0 ^ opad || H((k0 ^ ipad) || data)) */
        returnStatus = SHA2_finalize(handle, hmac);
    }

    return returnStatus;
}

/*
 *  ======== SHA2_hmac ========
 *
 *  This is practically just a convenience function. Because of the need for
 *  segmented hashes to construct the HMAC without actually allocating memory
 *  to concatenate intermediate results and the message, this function is not
 *  actually faster than an application using the segmented APIs.
 */
int_fast16_t SHA2_hmac(SHA2_Handle handle, const CryptoKey *key, const void *data, size_t dataLength, void *hmac)
{
    int_fast16_t result;

    result = SHA2_setupHmac(handle, key);

    if (result == SHA2_STATUS_SUCCESS)
    {
        /* Add the input message to the hash */
        result = SHA2_addData(handle, data, dataLength);
    }

    if (result == SHA2_STATUS_SUCCESS)
    {
        result = SHA2_finalizeHmac(handle, hmac);
    }

    return result;
}

/*
 *  ======== SHA2_reset ========
 */
void SHA2_reset(SHA2_Handle handle)
{
    SHA2LPF3SW_Object *object = SHA2LPF3SW_getObject(handle);

    CryptoUtils_memset(&object->sha256swObject, sizeof(object->sha256swObject), 0, sizeof(object->sha256swObject));

    CryptoUtils_memset(&object->digestBuffer, sizeof(object->digestBuffer), 0, sizeof(object->digestBuffer));
}

/*
 *  ======== SHA2_cancelOperation ========
 */
int_fast16_t SHA2_cancelOperation(SHA2_Handle handle)
{

    /* Cancel is not supported. No need to use/reference handle. */
    return SHA2_STATUS_UNSUPPORTED;
}

/*
 *  ======== SHA2_setHashType ========
 */
int_fast16_t SHA2_setHashType(SHA2_Handle handle, SHA2_HashType type)
{
    /*
     * We only support SHA256. Trying to switch to any other hash type returns
     * an error. Since object must have already been setup with SHA256,
     * no need to reference handle.
     */
    int_fast16_t result = SHA2_STATUS_UNSUPPORTED;

    if (type == SHA2_HASH_TYPE_256)
    {
        result = SHA2_STATUS_SUCCESS;
    }

    return result;
}
