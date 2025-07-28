/*
 * Copyright (c) 2025, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * These constant variables and structures are used to configure the TI drivers
 * and are typically generated automatically by SysConfig. However, SysConfig
 * is not supported for building the secure image at this time.
 *
 */

#include "config_tfm.h"

#include <ti/drivers/RNG.h>

const RNG_ReturnBehavior RNGXXF3HSM_returnBehavior = RNG_RETURN_BEHAVIOR_POLLING;

/*
 *  ============================= Key Store Config =============================
 */
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_key_management.h>

#if ((KEYSTORE_ASSET_STORE_SLOT_COUNT == 0) || (KEYSTORE_ASSET_STORE_SLOT_COUNT > 5))
    #error "KEYSTORE_ASSET_STORE_SLOT_COUNT must be between 1 and 5"
#endif

#define KEYSTORE_TOTAL_SLOT_COUNT \
    (KEYSTORE_VOLATILE_SLOT_COUNT + KEYSTORE_ASSET_STORE_SLOT_COUNT + KEYSTORE_PERSISTENT_SLOT_COUNT)

/* Amount of bytes necessary to store a key's metadata in KeyStore. Keys of
 * every type require a slot, each with a constant size.
 */
#define KEYSTORE_SLOT_METADATA_SIZE 83

/* The largest key that can be stored is an encrypted AES256 key.
 * The breakdown is as follows: For each key, there is 32 bytes of data.
 * Each key is wrapped separately, due to an HSM limitation involving asset policy.
 * This adds 16 bytes to each key. Furthermore, each of the two keys requires 32 bytes
 * of memory allocation overhead. (32 + 16 + 32) * 2 = 160 bytes.
 */
#define KEYSTORE_KEY_ITEM_SIZE_MAX 160

/* volatileMemoryPoolSize defines the size of the memory pool used for an internal memory
 * allocator in KeyStore. This is primarily for key material, but there are some intermediate
 * use cases of this volatile memory. These extra overhead bytes are to ensure there is enough
 * space in the pool for the key material and the intermediate operations. See
 * psa_persistent_key_storage_format in
 * source\third_party\hsmddk\include\Integration\Adapter_PSA\Adapter_mbedTLS\src\psa_crypto_storage.c
 * for details.
 */
#define KEYSTORE_RAM_OVERHEAD 40

#define KEYSTORE_VOLATILE_MEMORY_POOL_SIZE                                                                          \
    (KEYSTORE_VOLATILE_KEY_MEMORY_POOL_SIZE + ((KEYSTORE_PERSISTENT_SLOT_COUNT + 1) * KEYSTORE_KEY_ITEM_SIZE_MAX) + \
     KEYSTORE_RAM_OVERHEAD)

uint8_t volatileAllocBuffer[KEYSTORE_VOLATILE_MEMORY_POOL_SIZE];
const size_t volatileAllocBufferSizeBytes = KEYSTORE_VOLATILE_MEMORY_POOL_SIZE;

const size_t MBEDTLS_KEY_VOLATILE_COUNT    = KEYSTORE_VOLATILE_SLOT_COUNT;
const size_t MBEDTLS_KEY_ASSET_STORE_COUNT = KEYSTORE_ASSET_STORE_SLOT_COUNT;
/* For cache slots */
const size_t MBEDTLS_KEY_PERSISTENT_COUNT  = KEYSTORE_PERSISTENT_SLOT_COUNT;

psa_key_context_t gl_PSA_Key[KEYSTORE_TOTAL_SLOT_COUNT];