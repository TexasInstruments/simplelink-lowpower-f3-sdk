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

/*!****************************************************************************
 *  @file       HSMBareMetal.h
 *
 *  @brief      Interface to The HSM IP in a BareMetal-based implementation.
 *
 *  This module provides cryptographic functions for use of the Hardware Security Module.
 *
 *  Summary and List of Features:
 *  - HSM initialization sequence.
 *      * Turn on the HSM Clock.
 *      * Initialize and link the designated HSM mailbox for token exchanges
 *      * Boot up the HSM by sending a boot operation to the HSM to initialize the HSM FW.
 *  - HSM un-init sequence.
 *  - HSM Asset-related Operations:
 *      * Dynamic Asset creation and plaintext-only load to the HSM internal AssetStore.
 *      * Support for AES and MAC keys including HMAC.
 *      * In-API key generation:
 *        (The user can request to generate key, create an asset, and load key in HSM AssetStore in one API)
 *  - HSM Cryptography-related Operations:
 *      * AES operations, (CCM, GCM, CTR, ECB, CBC).
 *      * MAC operations, (128-bit CMAC, 128-bit CBC-MAC, HMAC).
 *      * Hash operations, (SHA2-224, SHA2-256, SHA2-384, SHA2-512).
 *      * RNG operations, Deterministic (RECOMMENDED) and non-deterministic (Raw).
 *      * ECC operations, (ECDH, ECDSA).
 *  - HSM Support for plaintext or AssetStore formats for keys for AES operations.
 *      * (CCM, GCM, CTR, ECB, CBC):
 *          1. For plaintext, the symmetric key material can be provided as part of the AES operation.
 *          2. For AssetStore, the symmetric key material must be loaded as an HSM asset and the asset ID generated has
 *             to be provided as part of the AES operation.
 *      * (CMAC, CBC-MAC, and HMAC):
 *          1. MAC-based operations that require the symmetric key to be pre-loaded in the HSM AssetStore and provide
 * the asset ID as part of the MAC operation.
 *  - HSM Support for (CCM, GCM, CTR, ECB, CBC, CMAC, CBC-MAC) operations with the following key sizes:
 *      * 128-bit (16 Bytes).
 *      * 192-bit (24 Bytes).
 *      * 256-bit (32 Bytes).
 *  - HSM Support for HMAC Algorithms:
 *      * SHA2-224 with key sizes within range 14-64 Bytes.
 *      * SHA2-256 with key sizes within range 14-64 Bytes.
 *      * SHA2-384 with key sizes within range 24-128 Bytes.
 *      * SHA2-512 with key sizes within range 32-128 Bytes.
 *  - HSM Support for HASH Algorithms:
 *      * SHA2-224 with digest length 28 Bytes.
 *      * SHA2-256 with digest length 32 Bytes.
 *      * SHA2-384 with digest length 48 Bytes.
 *      * SHA2-512 with digest length 64 Bytes.
 *  - HSM Support for RNG Operations:
 *      * Size must be 0 < x < (2^16 Bytes).
 *      * For DRBG data, size must be 32-bit aligned. (Multiple of 4 bytes).
 *      * For NRBG data, size can be anywhere between 1-256 bytes and multiple of 256 bytes if larger than 256 bytes.
 *  - HSM Support for ECC Operations:
 *      * ECDH public key generation.
 *      * ECDH shared secret generation.
 *      * ECDSA DS digest signature generation.
 *      * ECDSA DS digest signature verification.
 *  - HSM Support for ECC Curve types:
 *      * SEC_P_224_R1.
 *      * SEC_P_256_R1.
 *      * SEC_P_384_R1.
 *      * SEC_P_521_R1.
 *      * BRP_P_256_R1.
 *      * BRP_P_384_R1.
 *      * BRP_P_512_R1.
 *      * CURVE25519.
 *  - HSM limitations:
 *      * All buffer address must be word-aligned.
 *      * Input plaintext length for (CCM, GCM, CBC, CTR, ECB, CMAC, CBC-MAC, SHA2, HMAC) shall be upto 2^21 -1 Bytes
 * and block-size aligned.
 *      * Input and output buffers shall be a multiple of block length.
 *        Meaning that while the input data can be of any length, the buffers for the input and output data
 *        should be rounded up to the nearest size that is a multiple of block size.
 *          - For example, input data size of 24 Bytes must be in a buffer that is of size 32 bytes.
 *      * Additional Authenticated Data (AAD) for (CCM, GCM) length up to 2^16 - 2^8 Bytes.
 *      * The combined length of the input data and the AAD for (CCM, GCM) shall not be zero.
 *      * Nonce for (CCM) length for (7-13) Bytes.
 *      * Initialization Vector (IV) for (CCM, CBC, CTR) length IS EXACTLY 16 Bytes.
 *      * Tag length for AES-CCM shall be a multiple of 2 Bytes, [4, 6, 8, 10, 12, 14, 16].
 *      * Tag for AES-GCM length should be the following [4, 8, 12, 13, 14, 15, 16] Bytes.
 *      * MAC for (128-bit CMAC, 128-bit CBC-MAC) length IS EXACTLY 16 Bytes.
 *      * For ECC operations:
 *          - ECDSA sign and verify operations do not support curve type: CURVE25519.
 *          - All input keys must be supplied in plaintext.
 *          - ECC operations takes in a digest (not the input buffer).
 *              Therefore, the input buffer must be hashed using #HSMBareMetal_HASHOperation() API and supplying the
 * digest in the ECC operation.
 *          - Digest length must match curve length. Ex. for SEC_P_384_R1 and BRP_P_384_R1 curve types, the digest must
 * be generated using SHA2-384. For BRP_P_512_R1 and SEC_P_521_R1 curve types, the digest must be generated using
 * SHA2-512.
 *
 *  NOTE: The above features do not support segmented operations. All operations are performed as a 'one-shot'
 *  operation. NOTE: the above features and their APIs are synchonous and conduct their operations in a polling return
 *  behavior-like fashion.
 *
 *  # Usage #
 *
 *  ## Before starting a BareMetal operation #
 *
 *  Before starting a BareMetal operation
 *      - Call HSMBareMetal_init() to initialize the HSM IP and the SW architecture.
 *
 *  ## Starting a BareMetal operation #
 *
 *  The HSMBareMetal implementation provides a simplified approach to leverage the Hardware Security Module (HSM)
 *  functional capability while also bypassing the complex SW architecture of the HSM and the RTOS while offering a
 *  centralized suit of APIs to conduct all major cryptographic operations.
 *
 *  This implementation delivers multiple features in a switch-like approach. For example, it allows multiple
 *  modes of an Advanced Encryption Standard (AES) operations in one API by simply choosing from an Enum for
 *  all AEAD operations such as (CCM, GCM, CBC, CTR, ECB, CMAC, CBC-MAC), (ENC, DEC).
 *
 *  @anchor ti_sccm_HSMBareMetal_Synopsis
 *  ## Examples
 *  @code
 *
 *  // Import the HSMBareMetal defintions
 *  #include <ti/drivers/cryptoutils/hsm/HSMBareMetal.h>
 *
 *  int_fast16_t status;
 *  HSMBareMetal_AssetOperationStruct assetOperationStruct;
 *  HSMBareMetal_AESOperationStruct AESOperationStruct;
 *  HSMBareMetal_AESOperationMode aesOperationMode = HSMBareMetal_AES_MODE_CCM;
 *  HSMBareMetal_KeyInput keyInput = HSMBareMetal_KEY_INPUT_ASSETSTORE;
 *
 *  uint32_t encKeyAssetId   = 0U;
 *  uint32_t decKeyAssetId   = 0U;
 *  HSMBareMetal_AssetPairStruct assetPair;
 *  assetPair.encGenKeyAssetID = &encKeyAssetId;
 *  assetPair.decVrfyKeyAssetID = &decKeyAssetId;
 *  uint8_t nonce[] = "Thisisanonce";
 *  uint8_t aad[] = "This string will be authenticated but not encrypted.";
 *  uint8_t plaintext[] = "This string will be encrypted and authenticated.";
 *  uint8_t mac[16];
 *  uint8_t ciphertext[sizeof(plaintext)];
 *  uint8_t keyingMaterial[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
 *                                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
 *                                0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
 *                                0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
 *
 *  status = HSMBareMetal_init();
 *
 *  if (status != HSMBAREMETAL_STATUS_SUCCESS)
 *  {
 *      // handle error
 *  }
 *
 *  if (keyInput == HSMBareMetal_KEY_INPUT_ASSETSTORE)
 *  {
 *      HSMBareMetal_AssetOperation_init(&assetOperationStruct);
 *
 *      assetOperationStruct.keyAssetIDs = assetPair;
 *      assetOperationStruct.isKeyGenerated = true;
 *      assetOperationStruct.key = &keyingMaterial[0];
 *      assetOperationStruct.keyLength = sizeof(keyingMaterial);
 *      assetOperationStruct.algorithm = HSMBareMetal_OPERATION_ALGO_AES;
 *      assetOperationStruct.aesOperationMode = aesOperationMode;
 *
 *      status = HSMBareMetal_AssetOperation(&assetOperationStruct);
 *
 *      if (status != HSMBAREMETAL_STATUS_SUCCESS)
 *      {
 *          // handle error
 *      }
 *  }
 *
 *  HSMBareMetal_AESOperation_init(&AESOperationStruct);
 *
 *  if (keyInput == HSMBareMetal_KEY_INPUT_ASSETSTORE)
 *  {
 *      AESOperationStruct.keyAssetID = encKeyAssetId;
 *  }
 *  else
 *  {
 *      AESOperationStruct.key = (uint8_t *)keyingMaterial;
 *  }
 *
 *  AESOperationStruct.keyLength = sizeof(keyingMaterial);
 *  AESOperationStruct.isKeyInAssetStore = keyInput;
 *  AESOperationStruct.input = plaintext;
 *  AESOperationStruct.inputLength = sizeof(plaintext);
 *  AESOperationStruct.aad = aad;
 *  AESOperationStruct.aadLength = sizeof(aad);
 *  AESOperationStruct.mac = mac;
 *  AESOperationStruct.macLength = sizeof(mac);
 *  AESOperationStruct.nonce = nonce;
 *  AESOperationStruct.nonceLength = sizeof(nonce);
 *  AESOperationStruct.output = ciphertext;
 *  AESOperationStruct.operationMode = aesOperationMode;
 *  AESOperationStruct.operationDirection = HSMBareMetal_OPERATION_DIR_ENC_GEN;
 *
 *  status = HSMBareMetal_AESOperation(&AESOperationStruct);
 *
 *  if (status != HSMBAREMETAL_STATUS_SUCCESS)
 *  {
 *      // handle error
 *  }
 *
 *  if (keyInput == HSMBareMetal_KEY_INPUT_ASSETSTORE)
 *  {
 *      status = HSMBareMetal_freeAssetPair(assetPair);
 *  }
 *
 *  HSMBareMetal_deInit();
 *
 *  @endcode
 *
 */

#ifndef HSMBAREMETAL_H
#define HSMBAREMETAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*!
 *  @brief   Successful status code.
 *
 *  Functions return #HSMBAREMETAL_STATUS_SUCCESS if the function was executed
 *  successfully.
 */
#define HSMBAREMETAL_STATUS_SUCCESS ((int_fast16_t)0)

/*!
 *  @brief   Generic error status code.
 *
 *  Functions return #HSMBAREMETAL_STATUS_ERROR if the function was not executed
 *  successfully and no more specific error is applicable.
 */
#define HSMBAREMETAL_STATUS_ERROR ((int_fast16_t)-1)

/*!
 *  @brief   HSM IP has not been initialized yet.
 *  Call #HSMBareMetal_init()
 */
#define HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED ((int_fast16_t)-2)

/*!
 *  @brief   HSM IP reported a Hardware error. This renders the IP non-functional.
 *
 *  To fix this, a hard reset must be performed on the device.
 */
#define HSMBAREMETAL_STATUS_HW_ERROR ((int_fast16_t)-3)

/*!
 *  @brief   HSM IP is already initialized.
 */
#define HSMBAREMETAL_STATUS_HSM_ALREADY_INITIALIZED ((int_fast16_t)-4)

/*!
 *  @brief   The MAC verification failed.
 *
 *  A MAC operation-related error code.
 *
 *  Functions return #HSMBAREMETAL_STATUS_INVALID_MAC if the MAC computed
 *  for the provided (key, message) pair did not match the MAC provided.
 */
#define HSMBAREMETAL_STATUS_INVALID_MAC ((int_fast16_t)-5)

/*!
 *  @brief   One or more of the provided parameter(s) do not match the supported features.
 */
#define HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS ((int_fast16_t)-6)

/*!
 *  @brief   the NRBG engine is already in mode.
 */
#define HSMBAREMETAL_STATUS_NRBG_ALREADY_IN_MODE ((int_fast16_t)-7)

#define HSMBAREMETAL_STATUS_INVALID_PRIVATE_KEY_SIZE ((int_fast16_t)-8)

#define HSMBAREMETAL_STATUS_INVALID_PUBLIC_KEY_SIZE ((int_fast16_t)-9)

#define HSMBAREMETAL_STATUS_INVALID_PUBLIC_KEY_OCTET_VALUE ((int_fast16_t)-10)

#define HSMBAREMETAL_STATUS_INVALID_SHARED_SECRET_KEY_SIZE ((int_fast16_t)-11)

/*!
 *  @brief    Wait forever for a result token define
 */
#define HSMBareMetal_WAIT_FOREVER ~(0)

/*!
 *  @brief  Enum for entropy type.
 *
 *  For key generation, users MUST choose #ENTROPY_TYPE_DRBG.
 */
typedef enum
{
    ENTROPY_TYPE_DRBG = 0x0000,
    ENTROPY_TYPE_CRNG = 0x7264,
    ENTROPY_TYPE_TRNG = 0x5244,
} HSMBareMetal_entropyType;

/*!
 *  @brief  Enum for the current NRBG IP for the HSM to pull entropy from.
 *
 * By default, the HSM is set to use the #NRBG_TYPE_CRNG.
 */
typedef enum
{
    NRBG_TYPE_CRNG = 1,
    NRBG_TYPE_TRNG = 0,
} HSMBareMetal_NRBGMode;

/*!
 *  @brief  Enum for operation algorithm.
 *
 * These values to be used in Dynamic asset creation to determine the proper asset Policy for an HSM Key.
 */
typedef enum
{
    HSMBareMetal_OPERATION_ALGO_AES  = 1,
    HSMBareMetal_OPERATION_ALGO_MAC  = 2,
    HSMBareMetal_OPERATION_ALGO_HASH = 3,
    HSMBareMetal_OPERATION_ALGO_PK   = 4,
} HSMBareMetal_operationAlgorithm;

/*!
 *  @brief  Enum for the direction of an AES operation.
 *  - Encryption/Decryption for (CCM, GCM, CBC, CTR, ECB)
 *  - Generate/Verify for (CMAC, CBC-MAC, HMAC)
 */
typedef enum
{
    HSMBareMetal_OPERATION_DIR_ENC_GEN  = 1,
    HSMBareMetal_OPERATION_DIR_DEC_VRFY = 2,
} HSMBareMetal_operationDirection;

/*!
 *  @brief  Enum for AES operation modes.
 */
typedef enum
{
    HSMBareMetal_AES_MODE_ECB = 0,
    HSMBareMetal_AES_MODE_CBC = 1,
    HSMBareMetal_AES_MODE_CTR = 2,
    HSMBareMetal_AES_MODE_CCM = 5,
    HSMBareMetal_AES_MODE_GCM = 7,
} HSMBareMetal_AESOperationMode;

/*!
 *  @brief  Enum for the MAC operation mode.
 */
typedef enum
{
    HSMBareMetal_MAC_MODE_HMAC_224 = 2,
    HSMBareMetal_MAC_MODE_HMAC_256 = 3,
    HSMBareMetal_MAC_MODE_HMAC_384 = 4,
    HSMBareMetal_MAC_MODE_HMAC_512 = 5,
    HSMBareMetal_MAC_MODE_CMAC     = 8,
    HSMBareMetal_MAC_MODE_CBC_MAC  = 9,
} HSMBareMetal_MACOperationMode;

/*!
 *  @brief  Enum for Hash operation modes.
 */
typedef enum
{
    HSMBareMetal_HASH_MODE_SHA2_224 = 2,
    HSMBareMetal_HASH_MODE_SHA2_256 = 3,
    HSMBareMetal_HASH_MODE_SHA2_384 = 4,
    HSMBareMetal_HASH_MODE_SHA2_512 = 5,
} HSMBareMetal_HASHOperationMode;

/*!
 *  @brief  Enum for ECC operation curve types.
 */
typedef enum
{
    HSMBareMetal_PK_CURVE_TYPE_SEC_P_224_R1 = 1,
    HSMBareMetal_PK_CURVE_TYPE_SEC_P_256_R1 = 2,
    HSMBareMetal_PK_CURVE_TYPE_SEC_P_384_R1 = 3,
    HSMBareMetal_PK_CURVE_TYPE_SEC_P_521_R1 = 4,
    HSMBareMetal_PK_CURVE_TYPE_BRP_P_256_R1 = 5,
    HSMBareMetal_PK_CURVE_TYPE_BRP_P_384_R1 = 6,
    HSMBareMetal_PK_CURVE_TYPE_BRP_P_512_R1 = 7,
    HSMBareMetal_PK_CURVE_TYPE_25519 = 8, /* Only applicable for ECDH generate public key and compute shared secret
                                             operations. */
} HSMBareMetal_ECCOperationCurveType;

/*!
 *  @brief  Enum for ECC operation curve lengths.
 */
typedef enum
{
    HSMBareMetal_PK_CURVE_LENGTH_224 = 224,
    HSMBareMetal_PK_CURVE_LENGTH_255 = 255,
    HSMBareMetal_PK_CURVE_LENGTH_256 = 256,
    HSMBareMetal_PK_CURVE_LENGTH_384 = 384,
    HSMBareMetal_PK_CURVE_LENGTH_512 = 512,
    HSMBareMetal_PK_CURVE_LENGTH_521 = 521,
} HSMBareMetal_ECCOperationCurveLength;

/*!
 *  @brief  Enum for ECC operation digest lengths.
 */
typedef enum
{
    HSMBareMetal_PK_DIGEST_LENGTH_224 = 224,
    HSMBareMetal_PK_DIGEST_LENGTH_256 = 256,
    HSMBareMetal_PK_DIGEST_LENGTH_384 = 384,
    HSMBareMetal_PK_DIGEST_LENGTH_512 = 512,
} HSMBareMetal_ECCOperationDigestLength;

/*!
 *  @brief  Enum for ECC operation modes.
 */
typedef enum
{
    HSMBareMetal_PK_MODE_ECDH_GEN_PUB_KEY   = 1,
    HSMBareMetal_PK_MODE_ECDH_GEN_SHRD_SCRT = 2,
    HSMBareMetal_PK_MODE_ECDSA_SIGN         = 3,
    HSMBareMetal_PK_MODE_ECDSA_VERIFY       = 4,
} HSMBareMetal_ECCOperationMode;

/*!
 *  @brief  Enum for the AES operation mode.
 */
typedef enum
{
    HSMBareMetal_KEY_INPUT_PLAINTEXT  = 0,
    HSMBareMetal_KEY_INPUT_ASSETSTORE = 1,
} HSMBareMetal_KeyInput;

/* Operation Structures */

/*!
 * @brief Struct to store HSM system versions.
 */
typedef struct
{
    uint8_t patch;
    uint8_t minor;
    uint8_t major;
} HSMBareMetal_systemInfoVersionStruct;

/*!
 *  @brief  Struct containing the parameters required for AES operations.
 */
typedef struct
{
    uint8_t *key;
    uint8_t keyLength;
    uint8_t *aad;        /*!< A buffer of length \c aadLength containing additional
                          *   authentication data to be authenticated/verified but not
                          *   encrypted/decrypted.
                          */
    uint8_t *input;      /*!<
                          *   - Encryption: The plaintext buffer to be encrypted and authenticated (CCM and GCM only)
                          *   in the AES operation.
                          *   - Decryption: The ciphertext to be decrypted and verified (CCM and GCM only).
                          */
    uint8_t *output;     /*!<
                          *   - Encryption: The output ciphertext buffer that the encrypted plaintext
                          *   is copied to.
                          *   - Decryption: The plaintext derived from the decrypted and verified
                          *   ciphertext is copied here.
                          */
    uint8_t *nonce;      /*!< A buffer containing a nonce. Nonces must be unique to
                          *   each CCM operation and may not be reused.
                          */
    uint8_t *iv;         /*!< A buffer containing an IV. IVs must be unique to
                          *   each CBC operation and may not be reused.
                          */
    uint8_t *mac;        /*!<
                          *   - Encryption: The buffer where the message authentication
                          *   code is copied.
                          *   - Decryption: The buffer containing the received message
                          *   authentication code.
                          */
    size_t aadLength;    /*!< Length of the total \c aad in bytes. Either \c aadLength or
                          *   \c inputLength must be non-zero.
                          */
    size_t inputLength;  /*!< Length of the input/output data in bytes. Either \c aadLength or
                          *   \c inputLength must be non-zero.
                          *   Max length supported may be limited depending on the return behavior.
                          */
    uint8_t nonceLength; /*!< Length of \c nonce in bytes.
                          *   Valid nonce lengths are [7, 8, ... 13].
                          */
    uint8_t ivLength;    /*!< Length of \c iv in bytes.
                          *   Valid length is 16 Bytes.
                          */
    uint8_t macLength;   /*!< Length of \c mac in bytes.
                          *   Valid MAC lengths are [0, 4, 6, 8, 10, 12, 14, 16].
                          *   A length of 0 disables authentication and verification. This is
                          *   only permitted when using CCM*.
                          */
    uint32_t keyAssetID; /* Asset ID for a previously-allocated asset. */
    HSMBareMetal_KeyInput isKeyInAssetStore;
    HSMBareMetal_operationDirection operationDirection;
    HSMBareMetal_AESOperationMode operationMode;
} HSMBareMetal_AESOperationStruct;

/*!
 *  @brief  Struct containing the parameters required for MAC operations.
 */
typedef struct
{
    uint32_t keyAssetID; /*!< The asset ID for the key in AssetStore */
    uint8_t keyLength;
    uint8_t *input;     /*!< - Sign: Pointer to the input message to
                         *     be authenticated.
                         *   - Verify: Pointer to the input message to be
                         *     verified.
                         */
    uint8_t *mac;       /*!< - Sign: Pointer to the output buffer to write
                         *     the generated MAC. Buffer size must be
                         *     at least equal to @a macLength.
                         *   - Verify: Pointer to the input MAC to be
                         *     used for verification.
                         */
    size_t inputLength; /*!< Length of the input message in bytes.
                         *   May be zero for CMAC but must be non-zero for CBC-MAC.
                         *   See function descriptions for further restrictions.
                         *   Max length supported may be limited depending on the return behavior.
                         */
    size_t macLength;   /*!< Length of the MAC in bytes.
                         *   Must be <= 16. A length of < 8 is not recommended and
                         *   should severely restrict MAC recomputation attempts.
                         *   See appendix A of NIST SP800-38b for more information.
                         */
    HSMBareMetal_MACOperationMode operationMode;
    HSMBareMetal_operationDirection operationDirection;
} HSMBareMetal_MACOperationStruct;

/*!
 *  @brief  Struct containing the two asset IDs returned for usage in a symmetric operation.
 */
typedef struct
{
    uint32_t *encGenKeyAssetID;  /* Asset ID for encryption/generation */
    uint32_t *decVrfyKeyAssetID; /* Asset ID for decryption/Verification */
} HSMBareMetal_AssetPairStruct;

/*!
 *  @brief  Struct containing the two asset IDs returned for ECC operations
 *
 *  The first asset is for the private key and the second asset is for the public key.
 */
typedef struct
{
    uint32_t *privateKeyAssetID; /* Asset ID for encryption/generation */
    uint32_t *publicKeyAssetID;  /* Asset ID for decryption/Verification */
} HSMBareMetal_AsymAssetPairStruct;

/*!
 *  @brief  Struct containing the parameters required for asset create and plaintext key load operations.
 */
typedef struct
{
    uint8_t *key;        /* Plaintext key buffer. */
    uint8_t keyLength;   /* Key length. */
    bool isKeyGenerated; /* When set, HSMBareMetal will leverage the #HSMBareMetal_RNGOperation to generate a DRBG key
                            internally and return the AssetIDPair only. */
    HSMBareMetal_AssetPairStruct keyAssetIDs; /* In a symmetric operation, for every key there exists two key Asset IDs.
                                                 One for ENC/GEN and the other for DEC/VRFY. */
    HSMBareMetal_operationAlgorithm algorithm; /* AES or MAC. */
    HSMBareMetal_AESOperationMode aesOperationMode;
    HSMBareMetal_MACOperationMode macOperationMode;
} HSMBareMetal_AssetOperationStruct;

/*!
 *  @brief  Struct containing the parameters required for Hash operations.
 */
typedef struct
{
    uint8_t *input;
    size_t inputLength;
    uint8_t *digest;
    HSMBareMetal_HASHOperationMode operationMode;
} HSMBareMetal_HASHOperationStruct;

/*!
 *  @brief  Struct containing the parameters required for RNG operations.
 */
typedef struct
{
    uint8_t *random;
    size_t randomLength;
    HSMBareMetal_entropyType entropyType;
} HSMBareMetal_RNGOperationStruct;

/*!
 *  @brief  Struct containing the Plaintext-based crypto key data.
 */
typedef struct
{
    uint8_t *keyMaterial;
    uint32_t keyLength;
} HSMBareMetal_CryptoKeyPlaintextStruct;

/*!
 *  @brief  Struct containing the AssetStore-based crypto key data.
 */
typedef struct
{
    uint32_t assetId;
    uint32_t keyLength;
} HSMBareMetal_CryptoKeyAssetStoreStruct;

/*!
 *  @brief  Struct containing the parameters required for AES operations.
 */
typedef struct
{
    HSMBareMetal_KeyInput keyInputType;
    union
    {
        HSMBareMetal_CryptoKeyPlaintextStruct plaintext;
        HSMBareMetal_CryptoKeyAssetStoreStruct assetStore;
    } u;
} HSMBareMetal_CryptoKeyStruct;

/*!
 *  @brief  Struct containing the parameters required for ECC operations.
 */
typedef struct
{
    HSMBareMetal_CryptoKeyStruct *privateKey;
    HSMBareMetal_CryptoKeyStruct *publicKey;
    HSMBareMetal_CryptoKeyStruct *sharedSecret; /* Only applicable for ECDH generate public key or compute shared secret
                                                   operations */
    uint8_t *hashDigest;                        /* Only applicable for ECDSA sign and verify operations */
    uint8_t *signatureR;                        /* Only applicable for ECDSA sign and verify operations */
    uint8_t *signatureS;                        /* Only applicable for ECDSA sign and verify operations */
    HSMBareMetal_ECCOperationMode operationMode;
    HSMBareMetal_ECCOperationCurveType operationCurveType;
} HSMBareMetal_ECCOperationStruct;

/*!
 *  @brief  Initialize HSMBareMetal Module.
 *
 *  The initialization process include:
 *      1. Turning on the HSM IP by enabling the clock.
 *      2. Initializing the HSM Mailboxes and corresponding registers.
 *      3. Booting the HSM (Sending a boot token and ensuring that the HSM FW image is accepted).
 */
int_fast16_t HSMBareMetal_init(void);

/*!
 *  @brief  Close the HSMBareMetal Module.
 *
 *  This operation turns off the HSM and clears all static objects.
 */
int_fast16_t HSMBareMetal_deInit(void);

/*!
 *  @brief  Returns the HSM FW image version number (major, minor, and patch)
 *  @pre    HSMBareMetal must be initialized using #HSMBareMetal_init()
 *
 *  This function sends a system info token and extracts HSM version numbers and returns the FW version.
 *
 *  @retval #HSMBAREMETAL_STATUS_SUCCESS               System info token returned data successfully.
 *  @retval #HSMBAREMETAL_STATUS_ERROR                 Error. System info failed or HSMBareMetal is not initialized.
 */
int_fast16_t HSMBareMetal_getHSMFirmwareVersion(HSMBareMetal_systemInfoVersionStruct *firmwareVersionStruct);

/*
 *  ======== HSMBareMetal_RNGOperation_init ========
 */
void HSMBareMetal_RNGOperation_init(HSMBareMetal_RNGOperationStruct *operationStruct);

/*
 *  ======== HSMBareMetal_AssetOperation_init ========
 */
void HSMBareMetal_AssetOperation_init(HSMBareMetal_AssetOperationStruct *operationStruct);

/*
 *  ======== HSMBareMetal_HASHOperation_init ========
 */
void HSMBareMetal_HASHOperation_init(HSMBareMetal_HASHOperationStruct *operationStruct);

/*
 *  ======== HSMBareMetal_AESOperation_init ========
 */
void HSMBareMetal_AESOperation_init(HSMBareMetal_AESOperationStruct *operationStruct);

/*
 *  ======== HSMBareMetal_MACOperation_init ========
 */
void HSMBareMetal_MACOperation_init(HSMBareMetal_MACOperationStruct *operationStruct);

/*
 *  ======== HSMBareMetal_CryptoKey_init ========
 */
void HSMBareMetal_CryptoKey_init(HSMBareMetal_CryptoKeyStruct *operationStruct);

/*
 *  ======== HSMBareMetal_ECCOperation_init ========
 */
void HSMBareMetal_ECCOperation_init(HSMBareMetal_ECCOperationStruct *operationStruct);

/*!
 *  @brief  Perform a Bare Metal Random Number Generation Operation.
 *  @pre    HSMBareMetal must be initialized using #HSMBareMetal_init()
 *
 *  !!!!IMPORTANT!!!!
 *      1. Size must be 0 < x < (2^16 Bytes).
 *      2. For DRBG data, size must be 32-bit aligned. (Multiple of 4 bytes).
 *      3. For NRBG data, size can be anywhere between 1-256 bytes and multiple of 256 bytes
 *         if larger than 256 bytes.
 *
 *  For symmetric key generation, users MUST choose #ENTROPY_TYPE_DRBG. However, the
 *  #HSMBareMetal_AssetOperation() API provide an In-API key generation feature that
 *  generates a key, creates two assets (ENC/GEN and DEC/VRFY) and loads the key into
 *  both assets and returns an a pair of asset IDs.
 *
 *  @param  [in] operationStruct                            Pointer to the operation's structure.
 *
 *  @retval #HSMBAREMETAL_STATUS_SUCCESS                    The operation succeeded.
 *  @retval #HSMBAREMETAL_STATUS_ERROR                      The operation failed.
 *  @retval #HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED        Call #HSMBareMetal_init() first.
 *  @retval #HSMBAREMETAL_STATUS_HW_ERROR                   HSM in fatal mode. Rest Device.
 *  @retval #HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS   One or more of the input parameters
 *                                                          is incorrect.
 *
 */
int_fast16_t HSMBareMetal_RNGOperation(HSMBareMetal_RNGOperationStruct *operationStruct);

/*!
 *  @brief  Switch the current NRBG engine: CRNG -> TRNG or vice-versa.
 *  @pre    HSMBareMetal must be initialized using #HSMBareMetal_init()
 *
 *  @param  [in] NRBGMode                                   Used to select which NRBG noise source to use.
 *
 *  @retval #HSMBAREMETAL_STATUS_SUCCESS                    The operation succeeded.
 *  @retval #HSMBAREMETAL_STATUS_ERROR                      The operation failed.
 *  @retval #HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED        Call #HSMBareMetal_init() first.
 *  @retval #HSMBAREMETAL_STATUS_HW_ERROR                   HSM in fatal mode. Rest Device.
 *  @retval #HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS   One or more of the input parameters
 *                                                          is incorrect.
 *
 */
int_fast16_t HSMBareMetal_RNGSwitchNRBGMode(HSMBareMetal_NRBGMode NRBGMode);

/*!
 *  @brief  Initializes a CryptoKey type

 *  @param [in]     keyHandle   Pointer to a CryptoKey which will be initialized
 *                              to type HSMBareMetal_KEY_INPUT_PLAINTEXT
 *                              and ready for use
 *  @param [in]     key         Pointer to keying material
 *
 *  @param [in]     keyLength   Length of keying material in bytes
 *
 */
void HSMBareMetal_CryptoKeyPlaintext_initKey(HSMBareMetal_CryptoKeyStruct *cryptoKey, uint8_t *key, size_t keyLength);

/*!
 *  @brief  Perform a Bare Metal Asset Create and Load Operations.
 *  @pre    HSMBareMetal must be initialized using #HSMBareMetal_init()
 *
 *  !!!!Important!!!!
 *  When operationStruct.isKeyGenerated is set to true, the HSMBareMetal module will IGNORE the key input parameter and
 *  instead generate a random key internally and use that as the data for an asset create and load operations.
 *
 *  The user MUST specify buffers for both #HSMBareMetal_AssetPairStruct.encGenKeyAssetID and
 *  #HSMBareMetal_AssetPairStruct.decVrfyKeyAssetID inside of #HSMBareMetal_AssetPairStruct.
 *  This API creates TWO assets and returns the corresponding asset IDs for them.
 *
 *  @param  [in] operationStruct                            Pointer to the operation's structure.
 *
 *  @retval #HSMBAREMETAL_STATUS_SUCCESS                    The operation succeeded.
 *  @retval #HSMBAREMETAL_STATUS_ERROR                      The operation failed.
 *  @retval #HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED        Call #HSMBareMetal_init() first.
 *  @retval #HSMBAREMETAL_STATUS_HW_ERROR                   HSM in fatal mode. Rest Device.
 *  @retval #HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS   One or more of the input parameters
 *                                                          is incorrect.
 *
 */
int_fast16_t HSMBareMetal_AssetOperation(HSMBareMetal_AssetOperationStruct *operationStruct);

/*!
 *  @brief  Perform a Bare Metal Asset Delete Operations.
 *  @pre    HSMBareMetal must be initialized using #HSMBareMetal_init() and a
 *          previous call to #HSMBareMetal_AssetOperation()
 *
 *  @param  [in] keyAssetPair                               ENC/GEN and DEC/VRFY asset IDs
 *
 *  @retval #HSMBAREMETAL_STATUS_SUCCESS                    The operation succeeded.
 *  @retval #HSMBAREMETAL_STATUS_ERROR                      The operation failed.
 *  @retval #HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED        Call #HSMBareMetal_init() first.
 *  @retval #HSMBAREMETAL_STATUS_HW_ERROR                   HSM in fatal mode. Rest Device.
 *  @retval #HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS   One or more of the input parameters
 *                                                          is incorrect.
 *
 */
int_fast16_t HSMBareMetal_freeAssetPair(HSMBareMetal_AssetPairStruct keyAssetPair);

/*!
 *  @brief  Perform a Bare Metal Asset Delete Operation.
 *  @pre    HSMBareMetal must be initialized using #HSMBareMetal_init() and a
 *          previous call to #HSMBareMetal_AssetOperation()
 *
 *  @param  [in] keyAssetID                                 Asset ID
 *
 *  @retval #HSMBAREMETAL_STATUS_SUCCESS                    The operation succeeded.
 *  @retval #HSMBAREMETAL_STATUS_ERROR                      The operation failed.
 *  @retval #HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED        Call #HSMBareMetal_init() first.
 *  @retval #HSMBAREMETAL_STATUS_HW_ERROR                   HSM in fatal mode. Rest Device.
 *
 */
int_fast16_t HSMBareMetal_freeKeyAsset(uint32_t *keyAssetID);

/*!
 *  @brief  Perform a Bare Metal AES Operation.
 *  @pre    HSMBareMetal must be initialized using #HSMBareMetal_init()
 *
 *  @param  [in] operationStruct                            Pointer to the operation's structure.
 *
 *  @retval #HSMBAREMETAL_STATUS_SUCCESS                    The operation succeeded.
 *  @retval #HSMBAREMETAL_STATUS_ERROR                      The operation failed.
 *  @retval #HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED        Call #HSMBareMetal_init() first.
 *  @retval #HSMBAREMETAL_STATUS_HW_ERROR                   HSM in fatal mode. Rest Device.
 *  @retval #HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS   One or more of the input parameters
 *                                                          is incorrect.
 *
 */
int_fast16_t HSMBareMetal_AESOperation(HSMBareMetal_AESOperationStruct *operationStruct);

/*!
 *  @brief  Perform a Bare Metal MAC Operation.
 *  @pre    HSMBareMetal must be initialized using #HSMBareMetal_init()
 *
 *  @param  [in] operationStruct                            Pointer to the operation's structure.
 *
 *  @retval #HSMBAREMETAL_STATUS_SUCCESS                    The operation succeeded.
 *  @retval #HSMBAREMETAL_STATUS_ERROR                      The operation failed.
 *  @retval #HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED        Call #HSMBareMetal_init() first.
 *  @retval #HSMBAREMETAL_STATUS_HW_ERROR                   HSM in fatal mode. Rest Device.
 *  @retval #HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS   One or more of the input parameters
 *                                                          is incorrect.
 *
 */
int_fast16_t HSMBareMetal_MACOperation(HSMBareMetal_MACOperationStruct *operationStruct);

/*!
 *  @brief  Perform a Bare Metal Hash Operation.
 *  @pre    HSMBareMetal must be initialized using #HSMBareMetal_init()
 *
 *  @param  [in] operationStruct                            Pointer to the operation's structure.
 *
 *  @retval #HSMBAREMETAL_STATUS_SUCCESS                    The operation succeeded.
 *  @retval #HSMBAREMETAL_STATUS_ERROR                      The operation failed.
 *  @retval #HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED        Call #HSMBareMetal_init() first.
 *  @retval #HSMBAREMETAL_STATUS_HW_ERROR                   HSM in fatal mode. Rest Device.
 *  @retval #HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS   One or more of the input parameters
 *                                                          is incorrect.
 *
 */
int_fast16_t HSMBareMetal_HASHOperation(HSMBareMetal_HASHOperationStruct *operationStruct);

/*!
 *  @brief  Perform a Bare Metal Public Key Operation.
 *  @pre    HSMBareMetal must be initialized using #HSMBareMetal_init()
 *
 *  !!!!Important!!!!
 *  For ECC operations:
 *      - ECDSA sign and verify operations do not support curve type: CURVE25519.
 *      - All input keys must be supplied in plaintext.
 *      - ECC operations takes in a digest (not the input buffer).
 *          Therefore, the input buffer must be hashed using #HSMBareMetal_HASHOperation() API
 *          and supplying the digest in the ECC operation.
 *      - Digest length must match curve length.
 *          Ex. For SEC_P_384_R1 and BRP_P_384_R1 curve types, the digest must be generated using SHA2-384.
 *          Ex. For BRP_P_512_R1 and SEC_P_521_R1 curve types, the digest must be generated using SHA2-512.
 *
 *  @param  [in] operationStruct                            Pointer to the operation's structure.
 *
 *  @retval #HSMBAREMETAL_STATUS_SUCCESS                    The operation succeeded.
 *  @retval #HSMBAREMETAL_STATUS_ERROR                      The operation failed.
 *  @retval #HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED        Call #HSMBareMetal_init() first.
 *  @retval #HSMBAREMETAL_STATUS_HW_ERROR                   HSM in fatal mode. Rest Device.
 *  @retval #HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS   One or more of the input parameters
 *                                                          is incorrect.
 *
 */
int_fast16_t HSMBareMetal_ECCOperation(HSMBareMetal_ECCOperationStruct *operationStruct);

#ifdef __cplusplus
}
#endif

#endif /* HSMBAREMETAL_H */