/******************************************************************************

 *  @file  ecdsa_lpf3.h
 *
 *  @brief      ECDSA driver implementation for the LPF3 devices with the
 *              following restrictions to minimize code size
 *              - Polling return behaviour only
 *              - Verify only
 *              - NISTP256 curve only

 Group: CMCU
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025-2026, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

#ifndef ecdsa_lpf3__include
#define ecdsa_lpf3__include

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Performance of the ECC SW library is scaled via the window size parameter
 * which defines the number of bits of the scalar that will be processed together
 * during the execution of the scalar multiplication. A larger window size
 * will have higher performance at the cost of increased memory consumption.
 * A window size of 3 was selected for the best trade-off of performance and
 * memory consumption. WorkZone size was empirically measured.
 *
 * ---------------------------------
 * |             |    NIST P256    |
 * | Window Size |  WorkZone Size  |
 * |             |     (words)     |
 * |-------------|-----------------|
 * |      2      |       227       |
 * |      3      |       254       |
 * |      4      |       308       |
 * |      5      |       416       |
 * ---------------------------------
 */
#ifndef ECDSA_LPF3_ECC_WINDOW_SIZE
    #define ECDSA_LPF3_ECC_WINDOW_SIZE (3)
#endif

/*
 * ECC Workzone size is based on worst case empirical measurement of ECDH
 * operations on supported curves with the selected window size.
 */
#if (ECDSA_LPF3_ECC_WINDOW_SIZE == 2)
    #define ECDSA_LPF3_ECC_WORKZONE_WORDS (227)
#elif (ECDSA_LPF3_ECC_WINDOW_SIZE == 3)
    #define ECDSA_LPF3_ECC_WORKZONE_WORDS (254)
#elif (ECDSA_LPF3_ECC_WINDOW_SIZE == 4)
    #define ECDSA_LPF3_ECC_WORKZONE_WORDS (308)
#elif (ECDSA_LPF3_ECC_WINDOW_SIZE == 5)
    #define ECDSA_LPF3_ECC_WORKZONE_WORDS (416)
#else
    #error "Invalid ECDSA_LPF3_ECC_WINDOW_SIZE"
#endif

/*!
 *  @brief Length of NIST P256 curve parameters in bytes
 */
#define ECCParams_NISTP256_LENGTH (32)

/*!
 * @brief   Successful status code.
 *
 * Functions return ECDSA_STATUS_SUCCESS if the function was executed
 * successfully.
 */
#define ECDSA_STATUS_SUCCESS (0)
#define ECDH_STATUS_SUCCESS ECDSA_STATUS_SUCCESS

/*!
 * @brief   Generic error status code.
 *
 * Functions return ECDSA_STATUS_ERROR if the function was not executed
 * successfully.
 */
#define ECDSA_STATUS_ERROR (-1)
#define ECDH_STATUS_ERROR ECDSA_STATUS_ERROR
/*!
 *  @brief  The provided CryptoKey does not match the expected size
 *
 *  The driver expects the private key to have the same length as other curve
 *  parameters and the public key to have a length of twice that plus one.
 *  If the provided CryptoKeys for the public and private keys do not match this
 *  scheme, this error will be returned.
 */
#define ECDSA_STATUS_INVALID_KEY_SIZE (-10)

/* HMAC defines */

#define SHA2_DIGEST_LENGTH_BYTES_256 32
#define SHA2_BLOCK_SIZE_BYTES_256 64
#define HMAC_OPAD_BYTE 0x5C
#define HMAC_IPAD_BYTE 0x36

/*!
 *  @brief  Plaintext CryptoKey datastructure.
 *
 * This structure contains all the information necessary to access keying material stored
 * in plaintext form in flash or RAM.
 */
typedef struct
{
    uint8_t *keyMaterial;
    uint32_t keyLength;
} CryptoKey_Plaintext;

/*!
 *  @brief  Struct containing the parameters required for verifying a message.
 */
typedef struct
{
    const CryptoKey_Plaintext *theirPublicKey; /*!< A pointer to the public key of the party
                                                *   that signed the hash of the message
                                                */
    const uint8_t *hash;                       /*!< A pointer to the hash of the message in
                                                *   octet string format.
                                                *   Must be the same length as the other curve parameters.
                                                */
    const uint8_t *r;                          /*!< A pointer to the r component of the received
                                                *   signature.
                                                *   Formatted in octet string format.
                                                *   Must be of the same length
                                                *   as other params of the curve used.
                                                */
    const uint8_t *s;                          /*!< A pointer to the s component of the received
                                                *   signature.
                                                *   Formatted in octet string format.
                                                *   Must be of the same length
                                                *   as other params of the curve used.
                                                */
} ECDSA_OperationVerify;

/*!
 *  @brief  Struct containing the parameters required for verifying a message.
 */
typedef struct
{
    const CryptoKey_Plaintext *privateKey;     /*!< A pointer to the private ECC key which will be used in to
                                                *   compute the shared secret.
                                                */
    const CryptoKey_Plaintext *publicKey;      /*!< A pointer to the public key of the party with whom the
                                                *   shared secret will be generated.
                                                */
    CryptoKey_Plaintext *sharedSecret;         /*!< A pointer to a CryptoKey which has been initialized blank.
                                                *   The shared secret will be placed here.
                                                *   The formatting byte will be filled in by the driver if the
                                                *   keyMaterialEndianness requires it.
                                                */
} ECDH_computeSharedSecretStruct;

/*!
 *  @brief Number of bytes for the length word prepended before all parameters
 *  passed into the ECC SW library functions.
 */
#define ECC_LENGTH_PREFIX_BYTES 4

/*!
 *  @brief Length in bytes of NISTP256 curve parameters including the prepended
 *  length word.
 */
#define ECC_NISTP256_PARAM_LENGTH_WITH_PREFIX_BYTES (ECCParams_NISTP256_LENGTH + ECC_LENGTH_PREFIX_BYTES)

/*!
 *  @brief Union to access ECC_NISTP256 curve params in bytes or words.
 */
typedef union
{
    uint8_t byte[ECC_NISTP256_PARAM_LENGTH_WITH_PREFIX_BYTES];
    uint32_t word[ECC_NISTP256_PARAM_LENGTH_WITH_PREFIX_BYTES / sizeof(uint32_t)];
} ECC_NISTP256_Param;

/*!
 *  @brief  Verifies a received signature matches a hash and public key
 *
 *  @param [in]     operation       A struct containing the pointers to the
 *                                  buffers necessary to perform the operation.
 *                                  All struct members must be initialized.
 *
 *  @retval #ECDSA_STATUS_SUCCESS                       The operation succeeded.
 *  @retval #ECDSA_STATUS_ERROR                         The operation failed. This is the return status if the signature
 * did not match.
 *  @retval #ECDSA_STATUS_INVALID_KEY_SIZE              The provided public key size does not match curve parameters
 */
int_fast16_t ECDSA_verify(ECDSA_OperationVerify *operation);

int_fast16_t ECDH_computeSharedSecret(ECDH_computeSharedSecretStruct *operation);

/*!
 *  @brief Initializes a plaintext CryptoKey
 *
 *  @param [in]     keyHandle   Pointer to a CryptoKey_Plaintext
 *  @param [in]     key         Pointer to keying material
 *  @param [in]     keyLength   Length of keying material in bytes
 */
void CryptoKeyPlaintext_initKey(CryptoKey_Plaintext *keyHandle, uint8_t *key, size_t keyLength);

void SHA2_xorBufferWithByte(uint8_t *buffer, size_t bufferLength, uint8_t byte);

#ifdef __cplusplus
}
#endif

#endif /* ecdsa_lpf3__include */
