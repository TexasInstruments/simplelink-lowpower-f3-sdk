/*
 * Copyright (c) 2024-2025, Texas Instruments Incorporated. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Customized project config file provided via compile definition
 * `PROJECT_CONFIG_HEADER_FILE` which overrides the settings configured in
 * `config_base.h`.
 *
 */

#ifndef __CONFIG_TFM_PROJECT__H__
#define __CONFIG_TFM_PROJECT__H__

/******************************* Crypto Config ********************************/

/* Comment out any of the following ENABLE_TI_CRYPTO_* defines to remove support
 * for that crypto driver and reduce the secure image size. Any PSA crypto
 * functions and algorithms which use disabled driver(s) will return
 * PSA_ERROR_NOT_SUPPORTED.
 */
#define ENABLE_TI_CRYPTO_AESCBC
#define ENABLE_TI_CRYPTO_AESCCM
#define ENABLE_TI_CRYPTO_AESCMAC
#define ENABLE_TI_CRYPTO_AESCTR
#define ENABLE_TI_CRYPTO_AESECB
#define ENABLE_TI_CRYPTO_AESGCM
#define ENABLE_TI_CRYPTO_ECDH
#define ENABLE_TI_CRYPTO_ECDSA
#define ENABLE_TI_CRYPTO_EDDSA
#define ENABLE_TI_CRYPTO_SHA2
#define ENABLE_TI_CRYPTO_RNG

/***************************** Key Storage Config *****************************
 * A key's persistence determines where the key material is stored:
 * - Persistent keys are stored in flash and a limited number are also cached
 *   in RAM for usage.
 * - Volatile keys are stored in RAM.
 * - HSM Asset Store keys are stored in the HSM's DRAM
 *
 * If a key's location is "HSM Asset Store", it will be encrypted whenever
 * stored in key store Flash or RAM.
 ******************************************************************************/

/* The volatile key memory pool size can be determined by calculating the key
 * size for each key type to be stored at a given time and totaling them.
 *
 *   Size of each key:
 *     key_bytes + 32B overhead + 16B (if encrypted)
 *
 *   If an *encrypted* AES key will be used for both encrypt and decrypt,
 *   multiply the key size by 2.
 *
 * For example:
 * - Five plaintext volatile AES-128 keys:
 *    5 keys * (16B + 32B) = 240
 *
 * - Two plaintext volatile AES-256 keys:
 *    2 keys * (32B + 32B) = 128
 *
 * - Two encrypted volatile AES-256 keys (with encrypt *or* decrypt usage):
 *    2 keys * (32B + 32B + 16B) = 160
 *
 * - Two encrypted volatile AES-256 keys (with encrypt *and* decrypt usage):
 *    2 keys * ((32B + 32B + 16B) * 2) = 320
 *
 * - One encrypted volatile ECC-521 key:
 *    1 keys * (66B + 32B + 16B) = 114
 */
#define KEYSTORE_VOLATILE_KEY_MEMORY_POOL_SIZE   1600

/* The maximum number of volatile keys that can be stored at a given time */
#define KEYSTORE_VOLATILE_SLOT_COUNT             10

/* The maximum number of keys that can be actively stored in the HSM Asset
 * Store. Asset store slot count must be <= 5 to leave space in HSM DRAM for
 * driver operations. This value must be non-zero.
 */
#define KEYSTORE_ASSET_STORE_SLOT_COUNT          3

/* The number of slots that will be reserved to cache persistent keys metadata in RAM */
#define KEYSTORE_PERSISTENT_SLOT_COUNT           3

/* The amount of flash storage space for persistent keys - must be 4, 8, or 10KB.
 *
 * Assuming the largest key size (encrypted AES-256 key with encrypt and decrypt
 * usage), these are the number of persistent keys that can be stored:
 *     4KB: 11 persistent keys
 *     8KB: 23 persistent keys
 *    10KB: 35 persistent keys
 */
#define KEYSTORE_FLASH_SIZE                      (8 * 1024)

/* Determine the number of persistent keys from the key store flash size */
#if (KEYSTORE_FLASH_SIZE == (4 * 1024))
#define KEYSTORE_PERSISTENT_NUM_KEYS             11
#elif (KEYSTORE_FLASH_SIZE == (8 * 1024))
#define KEYSTORE_PERSISTENT_NUM_KEYS             23
#elif (KEYSTORE_FLASH_SIZE == (10 * 1024))
#define KEYSTORE_PERSISTENT_NUM_KEYS             35
#else
#error "Invalid value for KEYSTORE_FLASH_SIZE"
#endif

/**************************** ITS Partition Config ****************************/

/* The maximum number of assets to be stored in the Internal Trusted Storage
 * is equal to the maximum number of persistent keys since the ITS is
 * only used for key storage.
 */
#define ITS_NUM_ASSETS                           KEYSTORE_PERSISTENT_NUM_KEYS

/*********************** Attestation Partition Config *************************/

/* Include optional verification service claim and certification reference
 * claims in initial attestation token.
 *
 * If optional claims are enabled, the certification reference value in
 * <SDK>/source/third_party/tfm/platform/ext/target/ti/cc27xx/attest_hal.c must
 * be updated with the certification number provided by the certification
 * authority. The verification_service_url[] string should also be updated
 * if necessary.
 */
#define ATTEST_INCLUDE_OPTIONAL_CLAIMS         0

/************************** System Partition Config ***************************/

/* Override HFXT Cap Array values.
 * This is equivalent to the overrideHfxtCapArray config in the CCFG SysConfig
 * module in S-only applications.
 */
#define CONFIG_SYSTEM_OVERRIDE_CAP_ARRAY       1

/* HFXT Cap Array Q1 value.
 * This is equivalent to the hfxtCapArrayQ1 config in the CCFG SysConfig module
 * in S-only applications.
 */
#define CONFIG_SYSTEM_CAP_ARRAY_Q1             33U

/* HFXT Cap Array Q2 value.
 * This is equivalent to the hfxtCapArrayQ2 config in the CCFG SysConfig module
 * in S-only applications.
 */
#define CONFIG_SYSTEM_CAP_ARRAY_Q2             33U


#endif /* __CONFIG_TFM_PROJECT__H__ */
