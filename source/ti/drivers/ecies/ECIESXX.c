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
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <ti/drivers/ecies/ECIESXX.h>
#include <ti/drivers/AESGCM.h>
#include <ti/drivers/ANSIX936KDF.h>
#include <ti/drivers/ECDH.h>
#include <ti/drivers/ECIES.h>
#include <ti/drivers/RNG.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKeyPlaintext.h>
#include <ti/drivers/cryptoutils/ecc/ECCParams.h>
#include <ti/drivers/cryptoutils/utils/CryptoUtils.h>

#include <ti/drivers/dpl/HwiP.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/aes.h)

/* Defines */
#define ECIES_DERIVED_KEY_SIZE 32U
#define ECIES_AES_KEY_SIZE     AES_128_KEY_LENGTH_BYTES
#define ECIES_AES_IV_SIZE      16U

/* Static globals */
static bool ECIES_isInitialized = false;

/* Forward declarations */
static ECIESXX_Object *ECIESXX_getObject(ECIES_Handle handle);
static inline int_fast16_t ECIES_generateScalarP256(const ECIESXX_Object *object, uint8_t *scalar, size_t scalarLen);
static inline int_fast16_t ECIES_generatePublicKey(const ECIESXX_Object *object,
                                                   const CryptoKey *privateKey,
                                                   uint8_t *publicKeyMaterial,
                                                   size_t publicKeyLen);
static inline int_fast16_t ECIES_computeSharedSecret(const ECIESXX_Object *object,
                                                     const CryptoKey *publicKey,
                                                     const CryptoKey *privateKey,
                                                     uint8_t *sharedSecretMaterial,
                                                     size_t sharedSecretLen);
static int_fast16_t ECIES_translateAESGCMStatus(int_fast16_t aesgcmStatus);
static int_fast16_t ECIES_translateECDHStatus(int_fast16_t ecdhStatus);
static int_fast16_t ECIES_translateKDFStatus(int_fast16_t kdfStatus);

/*
 *  ======== ECIESXX_getObject ========
 */
static inline ECIESXX_Object *ECIESXX_getObject(ECIES_Handle handle)
{
    return (ECIESXX_Object *)handle->object;
}

/*
 *  ======== ECIES_init ========
 */
void ECIES_init(void)
{
    AESGCM_init();
    ANSIX936KDF_init();
    ECDH_init();
    /* RNG initialization should be handled by the application after the RNG
     * driver is seeded with the noise input from Radio Control Layer.
     */

    ECIES_isInitialized = true;
}

/*
 *  ======== ECIES_construct ========
 */
ECIES_Handle ECIES_construct(ECIES_Config *config, const ECIES_Params *params)
{
    ECIES_Handle handle            = (ECIES_Config *)config;
    ECIESXX_Object *object         = ECIESXX_getObject(handle);
    const ECIESXX_HWAttrs *hwAttrs = config->hwAttrs;
    AESGCM_Params aesParams;
    ECDH_Params ecdhParams;
    ANSIX936KDF_Params kdfParams;
    RNG_Params rngParams;
    uintptr_t key;

    key = HwiP_disable();

    if (!ECIES_isInitialized || object->isOpen)
    {
        HwiP_restore(key);
        return NULL;
    }

    object->isOpen = true;

    HwiP_restore(key);

    if (params == NULL)
    {
        params = &ECIES_defaultParams;
    }

    /* Zero out the AESGCM object to ensure AESGCM_construct() will not fail */
    memset(&object->aesObject, 0, sizeof(object->aesObject));
    object->aesConfig.object  = &object->aesObject;
    object->aesConfig.hwAttrs = &hwAttrs->aesHwAttrs;

    /* Zero out the ECDH object to ensure ECDH_construct() will not fail */
    memset(&object->ecdhObject, 0, sizeof(object->ecdhObject));
    object->ecdhConfig.object  = &object->ecdhObject;
    object->ecdhConfig.hwAttrs = &hwAttrs->ecdhHwAttrs;

    /* Zero out the KDF object to ensure ANSIX936KDF_construct() will not fail */
    memset(&object->kdfObject, 0, sizeof(object->kdfObject));
    object->kdfConfig.object  = &object->kdfObject;
    object->kdfConfig.hwAttrs = &hwAttrs->kdfHwAttrs;

    /* Zero out the RNG object to ensure RNG_construct() will not fail */
    memset(&object->rngObject, 0, sizeof(object->rngObject));
    object->rngConfig.object  = &object->rngObject;
    object->rngConfig.hwAttrs = &hwAttrs->rngHwAttrs;

    /* Initialize params */
    AESGCM_Params_init(&aesParams);
    ANSIX936KDF_Params_init(&kdfParams);
    ECDH_Params_init(&ecdhParams);
    RNG_Params_init(&rngParams);

    if (params->returnBehavior == ECIES_RETURN_BEHAVIOR_BLOCKING)
    {
        aesParams.returnBehavior  = AESGCM_RETURN_BEHAVIOR_BLOCKING;
        ecdhParams.returnBehavior = ECDH_RETURN_BEHAVIOR_BLOCKING;
        kdfParams.returnBehavior  = ANSIX936KDF_RETURN_BEHAVIOR_BLOCKING;
        rngParams.returnBehavior  = RNG_RETURN_BEHAVIOR_BLOCKING;
    }
    else /* ECIES_RETURN_BEHAVIOR_POLLING */
    {
        aesParams.returnBehavior  = AESGCM_RETURN_BEHAVIOR_POLLING;
        ecdhParams.returnBehavior = ECDH_RETURN_BEHAVIOR_POLLING;
        kdfParams.returnBehavior  = ANSIX936KDF_RETURN_BEHAVIOR_POLLING;
        rngParams.returnBehavior  = RNG_RETURN_BEHAVIOR_POLLING;
    }

    object->aesHandle  = AESGCM_construct(&object->aesConfig, &aesParams);
    object->ecdhHandle = ECDH_construct(&object->ecdhConfig, &ecdhParams);
    object->kdfHandle  = ANSIX936KDF_construct(&object->kdfConfig, &kdfParams);
    object->rngHandle  = RNG_construct(&object->rngConfig, &rngParams);

    if ((object->aesHandle == NULL) || (object->ecdhHandle == NULL) || (object->kdfHandle == NULL) ||
        (object->rngHandle == NULL))
    {
        ECIES_close(handle);
        handle = NULL;
    }

    return handle;
}

/*
 *  ======== ECIES_close ========
 */
void ECIES_close(ECIES_Handle handle)
{
    ECIESXX_Object *object = ECIESXX_getObject(handle);

    if (object->aesHandle != NULL)
    {
        AESGCM_close(object->aesHandle);
    }

    if (object->kdfHandle != NULL)
    {
        ANSIX936KDF_close(object->kdfHandle);
    }

    if (object->ecdhHandle != NULL)
    {
        ECDH_close(object->ecdhHandle);
    }

    if (object->rngHandle != NULL)
    {
        RNG_close(object->rngHandle);
    }

    object->isOpen = false;
}

/*
 *  ======== ECIES_translateAESGCMStatus ========
 */
static int_fast16_t ECIES_translateAESGCMStatus(int_fast16_t aesgcmStatus)
{
    int_fast16_t status;

    if (aesgcmStatus == AESGCM_STATUS_SUCCESS)
    {
        status = ECIES_STATUS_SUCCESS;
    }
    else if (aesgcmStatus == AESGCM_STATUS_MAC_INVALID)
    {
        status = ECIES_STATUS_MAC_INVALID;
    }
    else if (aesgcmStatus == AESGCM_STATUS_RESOURCE_UNAVAILABLE)
    {
        status = ECIES_STATUS_RESOURCE_UNAVAILABLE;
    }
    else if (aesgcmStatus == AESGCM_STATUS_UNALIGNED_IO_NOT_SUPPORTED)
    {
        status = ECIES_STATUS_UNALIGNED_IO_NOT_SUPPORTED;
    }
    else
    {
        status = ECIES_STATUS_ERROR;
    }

    return status;
}

/*
 *  ======== ECIES_translateECDHStatus ========
 */
static int_fast16_t ECIES_translateECDHStatus(int_fast16_t ecdhStatus)
{
    int_fast16_t status;

    if (ecdhStatus == ECDH_STATUS_SUCCESS)
    {
        status = ECIES_STATUS_SUCCESS;
    }
    else if (ecdhStatus == ECDH_STATUS_RESOURCE_UNAVAILABLE)
    {
        status = ECIES_STATUS_RESOURCE_UNAVAILABLE;
    }
    else
    {
        status = ECIES_STATUS_ERROR;
    }

    return status;
}

/*
 *  ======== ECIES_translateKDFStatus ========
 */
static int_fast16_t ECIES_translateKDFStatus(int_fast16_t kdfStatus)
{
    int_fast16_t status;

    if (kdfStatus == ANSIX936KDF_STATUS_SUCCESS)
    {
        status = ECIES_STATUS_SUCCESS;
    }
    else if (kdfStatus == ANSIX936KDF_STATUS_RESOURCE_UNAVAILABLE)
    {
        status = ECIES_STATUS_RESOURCE_UNAVAILABLE;
    }
    else
    {
        status = ECIES_STATUS_ERROR;
    }

    return status;
}

/*
 *  ======== ECIES_generateScalarP256 ========
 *  Generates P-256 scalar 'k' in range [1, q) where q is the order of the
 *  curve.
 */
static inline int_fast16_t ECIES_generateScalarP256(const ECIESXX_Object *object, uint8_t *scalar, size_t scalarLen)
{
    int_fast16_t rngStatus;
    int_fast16_t status;
    size_t bitLength;
    uint32_t upperLimit[ECCParams_NISTP256_LENGTH >> 2U];

    /* Convert scalar length from bytes to bits */
    bitLength = scalarLen << 3U;

    /* Convert P-256 curve order (excluding length prefix) to big endian to
     * define upper limit.
     */
    CryptoUtils_reverseCopyPad(&ECCParams_NISTP256.order[ECC_LENGTH_PREFIX_BYTES], upperLimit, sizeof(upperLimit));

    /* Generate P-256 scalar 'k' in range [1, q).
     * RNG_getBERandomNumberInRange uses rejection sampling to avoid a biased
     * distribution.
     */
    rngStatus = RNG_getBERandomNumberInRange(object->rngHandle, CryptoUtils_limitOne, upperLimit, scalar, bitLength);

    if (rngStatus == RNG_STATUS_SUCCESS)
    {
        status = ECIES_STATUS_SUCCESS;
    }
    else if (rngStatus == RNG_STATUS_RESOURCE_UNAVAILABLE)
    {
        status = ECIES_STATUS_RESOURCE_UNAVAILABLE;
    }
    else
    {
        status = ECIES_STATUS_ERROR;
    }

    return status;
}

/*
 *  ======== ECIES_generatePublicKey ========
 *  publicKeyLen must be sufficient to store publicKeyMaterial in uncompressed
 *  string octet format (0x04 || x || y)
 */
static inline int_fast16_t ECIES_generatePublicKey(const ECIESXX_Object *object,
                                                   const CryptoKey *privateKey,
                                                   uint8_t *publicKeyMaterial,
                                                   size_t publicKeyLen)
{
    CryptoKey publicKey;
    ECDH_OperationGeneratePublicKey operation;
    int_fast16_t ecdhStatus;

    CryptoKeyPlaintext_initBlankKey(&publicKey, publicKeyMaterial, publicKeyLen);

    ECDH_OperationGeneratePublicKey_init(&operation);
    operation.curve                 = &ECCParams_NISTP256;
    operation.myPrivateKey          = privateKey;
    operation.myPublicKey           = &publicKey;
    operation.keyMaterialEndianness = ECDH_BIG_ENDIAN_KEY;

    /* Generate the public key Q = k . G where G is base point of the P-256 curve.
     * Private key validation is performed as part of ECDH_generatePublicKey.
     */
    ecdhStatus = ECDH_generatePublicKey(object->ecdhHandle, &operation);

    return (ECIES_translateECDHStatus(ecdhStatus));
}

/*
 *  ======== ECIES_computeSharedSecret ========
 *  publicKey must be in uncompressed string octet format (0x04 || x || y).
 *  sharedSecretLen must be sufficient to store sharedSecretMaterial output in
 *  uncompressed string octet format (0x04 || x || y).
 */
static inline int_fast16_t ECIES_computeSharedSecret(const ECIESXX_Object *object,
                                                     const CryptoKey *publicKey,
                                                     const CryptoKey *privateKey,
                                                     uint8_t *sharedSecretMaterial,
                                                     size_t sharedSecretLen)
{
    CryptoKey sharedSecret;
    ECDH_OperationComputeSharedSecret operation;
    int_fast16_t ecdhStatus;

    CryptoKeyPlaintext_initBlankKey(&sharedSecret, sharedSecretMaterial, sharedSecretLen);

    ECDH_OperationComputeSharedSecret_init(&operation);
    operation.curve                 = &ECCParams_NISTP256;
    operation.myPrivateKey          = privateKey;
    operation.sharedSecret          = &sharedSecret;
    operation.theirPublicKey        = publicKey;
    operation.keyMaterialEndianness = ECDH_BIG_ENDIAN_KEY;

    /* Public key validation is performed as part of ECDH_computeSharedSecret */
    ecdhStatus = ECDH_computeSharedSecret(object->ecdhHandle, &operation);

    return (ECIES_translateECDHStatus(ecdhStatus));
}

/*
 *  ======== ECIES_encrypt ========
 */
int_fast16_t ECIES_encrypt(ECIES_Handle handle,
                           const CryptoKey *publicKey,
                           const void *input,
                           size_t inputLen,
                           void *paddedOutput,
                           size_t paddedOutputLen)
{
    AESGCM_OneStepOperation aesOperation;
    CryptoKey aesKey;
    CryptoKey privateKey;
    ECIESXX_Object *object = ECIESXX_getObject(handle);
    int_fast16_t aesStatus;
    int_fast16_t kdfStatus;
    int_fast16_t status;
    uint8_t derivedKey[ECIES_DERIVED_KEY_SIZE];
    uint8_t k[ECCParams_NISTP256_LENGTH];
    uint8_t QP[2U * ECIES_PUBLIC_KEY_SIZE];
    uint8_t sharedSecret[ECIES_PUBLIC_KEY_SIZE];
    uint8_t *outputBytePtr = (uint8_t *)paddedOutput + ECIES_PADDING_BYTES;

    /* Verify outputLen is sufficient to store the output */
    if (paddedOutputLen < (ECIES_PADDING_BYTES + ECIES_PUBLIC_KEY_SIZE + inputLen + ECIES_TAG_SIZE))
    {
        return ECIES_STATUS_INSUFFICIENT_OUTPUT_LENGTH;
    }

    /* Generate P-256 scalar 'k' in range [1, q) where q is the order of the
     * curve.
     */
    status = ECIES_generateScalarP256(object, k, sizeof(k));
    if (status != ECIES_STATUS_SUCCESS)
    {
        return status;
    }

    /* Initialize a private key using the scalar 'k' for the key material */
    (void)CryptoKeyPlaintext_initKey(&privateKey, k, sizeof(k));

    /* Compute the public key Q from the scalar 'k' private key */
    status = ECIES_generatePublicKey(object, &privateKey, &QP[0], ECIES_PUBLIC_KEY_SIZE);
    if (status != ECIES_STATUS_SUCCESS)
    {
        return status;
    }

    /* Copy generated public key to output */
    (void)memcpy(outputBytePtr, QP, ECIES_PUBLIC_KEY_SIZE);

    /* Compute the shared secret Z from the scalar 'k' private key and the
     * provided public key P.
     */
    status = ECIES_computeSharedSecret(object, publicKey, &privateKey, sharedSecret, sizeof(sharedSecret));
    if (status != ECIES_STATUS_SUCCESS)
    {
        return status;
    }

    /* Concatenate the generated public key Q with the provided public key P */
    (void)memcpy(&QP[ECIES_PUBLIC_KEY_SIZE], publicKey->u.plaintext.keyMaterial, publicKey->u.plaintext.keyLength);

    /* Derive 32 bytes of keying material as V = ANSI-X9.63-KDF(x(Z), Q || P)
     * where x(Z) is the x coordinate of computed shared secret which has
     * the format (0x04 || x || y).
     */
    kdfStatus = ANSIX936KDF_deriveKey(object->kdfHandle,
                                      &sharedSecret[1],
                                      ECCParams_NISTP256_LENGTH,
                                      QP,
                                      sizeof(QP),
                                      derivedKey,
                                      sizeof(derivedKey));

    status = ECIES_translateKDFStatus(kdfStatus);

    if (status != ECIES_STATUS_SUCCESS)
    {
        return status;
    }

    /* ========================================================================
     * Encrypt message M as (C,T) = AES-128-GCM(K, IV, M) without any additional
     * authenticated data. K is the 128-bit AES key, IV is the initialization
     * vector, C is the ciphertext, and T is the 16-byte authentication tag.
     * ========================================================================
     */

    AESGCM_OneStepOperation_init(&aesOperation);

    /* Initialize AES-128 key K with the first 16 bytes of the derived key V */
    (void)CryptoKeyPlaintext_initKey(&aesKey, &derivedKey[0], ECIES_AES_KEY_SIZE);
    aesOperation.key = &aesKey;

    /* Set IV to the last 16 bytes of the derived key V */
    aesOperation.iv       = &derivedKey[ECIES_AES_KEY_SIZE];
    aesOperation.ivLength = ECIES_AES_IV_SIZE;

    /* No additional authentication data */
    aesOperation.aad         = NULL;
    aesOperation.aadLength   = 0;
    aesOperation.input       = (uint8_t *)input;
    aesOperation.inputLength = inputLen;
    /* Generated ciphertext is in the output buffer after the public key */
    aesOperation.output      = &outputBytePtr[ECIES_PUBLIC_KEY_SIZE];
    /* Generated authentication tag is in the output buffer after the ciphertext */
    aesOperation.mac         = &outputBytePtr[ECIES_PUBLIC_KEY_SIZE + inputLen];
    aesOperation.macLength   = ECIES_TAG_SIZE;

    aesStatus = AESGCM_oneStepEncrypt(object->aesHandle, &aesOperation);

    status = ECIES_translateAESGCMStatus(aesStatus);

    return status;
}

/*
 *  ======== ECIES_decrypt ========
 */
int_fast16_t ECIES_decrypt(ECIES_Handle handle,
                           const CryptoKey *privateKey,
                           const void *paddedInput,
                           size_t paddedInputLen,
                           void *output,
                           size_t outputLen)
{
    AESGCM_OneStepOperation aesOperation;
    CryptoKey aesKey;
    CryptoKey theirPublicKey;
    ECIESXX_Object *object = ECIESXX_getObject(handle);
    int_fast16_t aesStatus;
    int_fast16_t kdfStatus;
    int_fast16_t status;
    size_t plaintextLen = (paddedInputLen - ECIES_PUBLIC_KEY_SIZE - ECIES_TAG_SIZE - ECIES_PADDING_BYTES);
    uint8_t derivedKey[ECIES_DERIVED_KEY_SIZE];
    uint8_t QP[2U * ECIES_PUBLIC_KEY_SIZE];
    uint8_t sharedSecret[ECIES_PUBLIC_KEY_SIZE];
    uint8_t *inputBytePtr = (uint8_t *)paddedInput + ECIES_PADDING_BYTES;

    /* Verify outputLen is sufficient to store the output plaintext */
    if (outputLen < plaintextLen)
    {
        return ECIES_STATUS_INSUFFICIENT_OUTPUT_LENGTH;
    }

    /* Initialize a public key using the first ECIES_PUBLIC_KEY_SIZE bytes of
     * the input for the key material.
     */
    (void)CryptoKeyPlaintext_initKey(&theirPublicKey, inputBytePtr, ECIES_PUBLIC_KEY_SIZE);

    /* Compute the shared secret */
    status = ECIES_computeSharedSecret(object, &theirPublicKey, privateKey, sharedSecret, sizeof(sharedSecret));
    if (status != ECIES_STATUS_SUCCESS)
    {
        return status;
    }

    /* Compute the public key P from the provided private key */
    status = ECIES_generatePublicKey(object, privateKey, &QP[ECIES_PUBLIC_KEY_SIZE], ECIES_PUBLIC_KEY_SIZE);
    if (status != ECIES_STATUS_SUCCESS)
    {
        return status;
    }

    /* Copy the public key Q from the input */
    (void)memcpy(&QP[0], inputBytePtr, ECIES_PUBLIC_KEY_SIZE);

    /* Derive 32 bytes of keying material as V = ANSI-X9.63-KDF(x(Z), Q || P)
     * where x(Z) is the x coordinate of computed shared secret which has
     * the format (0x04 || x || y).
     */
    kdfStatus = ANSIX936KDF_deriveKey(object->kdfHandle,
                                      &sharedSecret[1],
                                      ECCParams_NISTP256_LENGTH,
                                      QP,
                                      sizeof(QP),
                                      derivedKey,
                                      sizeof(derivedKey));

    status = ECIES_translateKDFStatus(kdfStatus);

    if (status != ECIES_STATUS_SUCCESS)
    {
        return status;
    }

    /* ========================================================================
     * Decrypt message M as (P,T) = AES-128-GCM(K, IV, M) without any additional
     * authenticated data. K is the 128-bit AES key, IV is the initialization
     * vector, P is the plaintext, and T is the 16-byte authentication tag.
     * ========================================================================
     */

    AESGCM_OneStepOperation_init(&aesOperation);

    /* Initialize AES-128 key K with the first 16 bytes of the derived key V */
    (void)CryptoKeyPlaintext_initKey(&aesKey, &derivedKey[0], ECIES_AES_KEY_SIZE);
    aesOperation.key = &aesKey;

    /* Set IV to the last 16 bytes of the derived key V */
    aesOperation.iv       = &derivedKey[ECIES_AES_KEY_SIZE];
    aesOperation.ivLength = ECIES_AES_IV_SIZE;

    /* No additional authentication data */
    aesOperation.aad         = NULL;
    aesOperation.aadLength   = 0;
    /* Ciphertext is in the input buffer after the public key */
    aesOperation.input       = &inputBytePtr[ECIES_PUBLIC_KEY_SIZE];
    aesOperation.inputLength = plaintextLen;
    aesOperation.output      = output;
    /* Received authentication tag is in the input buffer after the ciphertext */
    aesOperation.mac         = &inputBytePtr[ECIES_PUBLIC_KEY_SIZE + plaintextLen];
    aesOperation.macLength   = ECIES_TAG_SIZE;

    aesStatus = AESGCM_oneStepDecrypt(object->aesHandle, &aesOperation);

    status = ECIES_translateAESGCMStatus(aesStatus);

    return status;
}
