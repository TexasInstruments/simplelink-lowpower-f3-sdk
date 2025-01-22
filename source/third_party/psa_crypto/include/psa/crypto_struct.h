/**
 * \file psa/crypto_struct.h
 *
 * \brief PSA cryptography module: Mbed TLS structured type implementations
 *
 * \note This file may not be included directly. Applications must
 * include psa/crypto.h.
 *
 * This file contains the definitions of some data structures with
 * implementation-specific definitions.
 *
 * In implementations with isolation between the application and the
 * cryptography module, it is expected that the front-end and the back-end
 * would have different versions of this file.
 *
 * <h3>Design notes about multipart operation structures</h3>
 *
 * For multipart operations without driver delegation support, each multipart
 * operation structure contains a `psa_algorithm_t alg` field which indicates
 * which specific algorithm the structure is for. When the structure is not in
 * use, `alg` is 0. Most of the structure consists of a union which is
 * discriminated by `alg`.
 *
 * For multipart operations with driver delegation support, each multipart
 * operation structure contains an `unsigned int id` field indicating which
 * driver got assigned to do the operation. When the structure is not in use,
 * 'id' is 0. The structure contains also a driver context which is the union
 * of the contexts of all drivers able to handle the type of multipart
 * operation.
 *
 * Note that when `alg` or `id` is 0, the content of other fields is undefined.
 * In particular, it is not guaranteed that a freshly-initialized structure
 * is all-zero: we initialize structures to something like `{0, 0}`, which
 * is only guaranteed to initializes the first member of the union;
 * GCC and Clang initialize the whole structure to 0 (at the time of writing),
 * but MSVC and CompCert don't.
 *
 * In Mbed TLS, multipart operation structures live independently from
 * the key. This allows Mbed TLS to free the key objects when destroying
 * a key slot. If a multipart operation needs to remember the key after
 * the setup function returns, the operation structure needs to contain a
 * copy of the key.
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  Copyright 2022-2024, Texas Instruments Incorporated
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  Modified by Texas Instruments to:
 *  - Support SimpleLink device crypto hardware drivers.
 *  - Change mbedtls_svc_key_id_t input params to psa_key_id_t.
 *  - Remove MBEDTLS_PSA_KA_xxxx defines.
 *  - Remove MBEDTLS_PSA_CRYPTO_SE_C specific code.
 */

#ifndef PSA_CRYPTO_STRUCT_H
#define PSA_CRYPTO_STRUCT_H

/* clang-format off */

/* Include the Mbed TLS configuration file if defined */
#if defined(MBEDTLS_CONFIG_FILE)
    #include MBEDTLS_CONFIG_FILE
#endif

/* Include the context definition for the compiled-in drivers for the primitive
 * algorithms.
 */
#include <ti/drivers/SHA2.h>
#include <ti/drivers/AESECB.h>
#include <ti/drivers/AESCCM.h>
#include <ti/drivers/AESGCM.h>

#ifdef TFM_BUILD
    /* Include for TFM builds */
    #include <third_party/tfm/secure_fw/partitions/crypto/tfm_crypto_api.h> /* For tfm_crypto_get_caller_id */
#elif (TFM_ENABLED == 1)
    /* Includes for NS library build */
    #include <third_party/tfm/interface/include/psa/crypto_client_struct.h>
#endif

#include <ti/devices/DeviceFamily.h>

#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2) || \
     (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4))
    #include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA.h>
    #include <third_party/psa_crypto/crypto_helper.h>
#elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    /* No headers needed */
#else
    #error "Device family not supported"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct psa_key_policy_s
{
    psa_key_usage_t usage;
    psa_algorithm_t alg;
    psa_algorithm_t alg2;
};
typedef struct psa_key_policy_s psa_key_policy_t;

#define PSA_KEY_POLICY_INIT \
    {                       \
        0, 0, 0             \
    }
static inline struct psa_key_policy_s psa_key_policy_init(void)
{
    const struct psa_key_policy_s v = PSA_KEY_POLICY_INIT;
    return (v);
}

/* The type used internally for key sizes.
 * Public interfaces use size_t, but internally we use a smaller type. */
typedef uint16_t psa_key_bits_t;

/* The maximum value of the type used to represent bit-sizes.
 * This is used to mark an invalid key size. */
#define PSA_KEY_BITS_TOO_LARGE ((psa_key_bits_t)(-1))
/* The maximum size of a key in bits.
 * Currently defined as the maximum that can be represented, rounded down
 * to a whole number of bytes.
 * This is an uncast value so that it can be used in preprocessor
 * conditionals. */
#define PSA_MAX_KEY_BITS       0xfff8

/** A mask of flags that can be stored in key attributes.
 *
 * This type is also used internally to store flags in slots. Internal
 * flags are defined in library/psa_crypto_core.h. Internal flags may have
 * the same value as external flags if they are properly handled during
 * key creation and in psa_get_key_attributes.
 */
typedef uint16_t psa_key_attributes_flag_t;

/* Default owner ID to match with client ID in TFM-enabled builds */
#define PSA_CRYPTO_KEY_ID_DEFAULT_OWNER -1


/* NOTE: this must match the attributes struct needed by CryptoKeyKeyStore.h */
typedef struct
{
    psa_key_type_t type;
    psa_key_bits_t bits;
    psa_key_lifetime_t lifetime;
    mbedtls_svc_key_id_t id;
    psa_key_policy_t policy;
    psa_key_attributes_flag_t flags;
} psa_core_key_attributes_t;

#define PSA_CORE_KEY_ATTRIBUTES_INIT \
    (psa_core_key_attributes_t)      \
    {                                \
        0                            \
    }

#if (TFM_ENABLED == 0) || defined(TFM_BUILD)
struct psa_key_attributes_s
{
    psa_core_key_attributes_t core;
    void *domain_parameters;
    size_t domain_parameters_size;
};
#endif

#define PSA_KEY_ATTRIBUTES_INIT \
    (psa_key_attributes_t)      \
    {                           \
        0                       \
    }

#if (TFM_ENABLED == 0) || defined(TFM_BUILD)
#ifdef MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER
static inline void mbedtls_set_key_owner_id(psa_key_attributes_t *attributes,
                                            mbedtls_key_owner_id_t owner)
{
    attributes->core.id.owner = owner;
}
#endif
#endif

static inline psa_key_attributes_t psa_key_attributes_init(void)
{
    const psa_key_attributes_t v = PSA_KEY_ATTRIBUTES_INIT;
    return v;
}

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)

static inline void psa_extend_key_usage_flags(psa_key_usage_t *usage_flags)
{
    if (*usage_flags & PSA_KEY_USAGE_SIGN_HASH) {
        *usage_flags |= PSA_KEY_USAGE_SIGN_MESSAGE;
    }

    if (*usage_flags & PSA_KEY_USAGE_VERIFY_HASH) {
        *usage_flags |= PSA_KEY_USAGE_VERIFY_MESSAGE;
    }
}

#if (TFM_ENABLED == 0) || defined(TFM_BUILD)

static inline void psa_set_key_id(psa_key_attributes_t *attributes,
                                  psa_key_id_t key)
{
    psa_key_lifetime_t lifetime = attributes->core.lifetime;

    attributes->core.id.key_id = key;
#ifdef TFM_BUILD
    /* tfm_crypto_get_caller_id always returns PSA_SUCCESS */
    (void)tfm_crypto_get_caller_id(&attributes->core.id.owner);
#else
    attributes->core.id.owner = PSA_CRYPTO_KEY_ID_DEFAULT_OWNER;
#endif

    if (PSA_KEY_LIFETIME_IS_VOLATILE(lifetime)) {
        attributes->core.lifetime =
            PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(
                PSA_KEY_LIFETIME_PERSISTENT,
                PSA_KEY_LIFETIME_GET_LOCATION(lifetime));
    }
}

static inline psa_key_id_t psa_get_key_id(
    const psa_key_attributes_t *attributes)
{
    return attributes->core.id.key_id;
}

static inline void psa_set_key_lifetime(psa_key_attributes_t *attributes,
                                        psa_key_lifetime_t lifetime)
{
    attributes->core.lifetime = lifetime;
    if (PSA_KEY_LIFETIME_IS_VOLATILE(lifetime)) {
        attributes->core.id.key_id = 0;
    }
}

static inline psa_key_lifetime_t psa_get_key_lifetime(
    const psa_key_attributes_t *attributes)
{
    return attributes->core.lifetime;
}

static inline void psa_set_key_usage_flags(psa_key_attributes_t *attributes,
                                           psa_key_usage_t usage_flags)
{
    psa_extend_key_usage_flags(&usage_flags);
    attributes->core.policy.usage = usage_flags;
}

static inline psa_key_usage_t psa_get_key_usage_flags(
    const psa_key_attributes_t *attributes)
{
    return attributes->core.policy.usage;
}

static inline void psa_set_key_algorithm(psa_key_attributes_t *attributes,
                                         psa_algorithm_t alg)
{
    attributes->core.policy.alg = alg;
}

static inline psa_algorithm_t psa_get_key_algorithm(
    const psa_key_attributes_t *attributes)
{
    return attributes->core.policy.alg;
}

/* This function is declared in crypto_extra.h, which comes after this
 * header file, but we need the function here, so repeat the declaration. */
extern psa_status_t KeyMgmt_psa_set_key_domain_parameters(psa_key_attributes_t *attributes,
                                                      psa_key_type_t type,
                                                      const uint8_t *data,
                                                      size_t data_length);

static inline void psa_set_key_type(psa_key_attributes_t *attributes,
                                    psa_key_type_t type)
{
    if (attributes->domain_parameters == NULL) {
        /* Common case: quick path */
        attributes->core.type = type;
    } else {
        /* Call the bigger function to free the old domain parameters.
         * Ignore any errors which may arise due to type requiring
         * non-default domain parameters, since this function can't
         * report errors. */
        (void)KeyMgmt_psa_set_key_domain_parameters(attributes, type, NULL, 0);
    }
}

static inline psa_key_type_t psa_get_key_type(
    const psa_key_attributes_t *attributes)
{
    return attributes->core.type;
}

static inline void psa_set_key_bits(psa_key_attributes_t *attributes,
                                    size_t bits)
{
    if (bits > PSA_MAX_KEY_BITS) {
        attributes->core.bits = PSA_KEY_BITS_TOO_LARGE;
    } else {
        attributes->core.bits = (psa_key_bits_t)bits;
    }
}

static inline size_t psa_get_key_bits(
    const psa_key_attributes_t *attributes)
{
    return attributes->core.bits;
}

#else

/* NS application (for TFM-enabled builds) uses these APIs providing client key
 * attributes.
 */

static inline void psa_set_key_id(psa_key_attributes_t *attributes,
                                  psa_key_id_t key)
{
    psa_key_lifetime_t lifetime = attributes->lifetime;

    attributes->id = key;

    if (PSA_KEY_LIFETIME_IS_VOLATILE(lifetime)) {
        attributes->lifetime =
            PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(
                PSA_KEY_LIFETIME_PERSISTENT,
                PSA_KEY_LIFETIME_GET_LOCATION(lifetime));
    }
}

static inline psa_key_id_t psa_get_key_id(
    const psa_key_attributes_t *attributes)
{
    return attributes->id;
}

static inline void psa_set_key_lifetime(psa_key_attributes_t *attributes,
                                        psa_key_lifetime_t lifetime)
{
    attributes->lifetime = lifetime;
    if (PSA_KEY_LIFETIME_IS_VOLATILE(lifetime)) {
        attributes->id = 0;
    }
}

static inline psa_key_lifetime_t psa_get_key_lifetime(
    const psa_key_attributes_t *attributes)
{
    return attributes->lifetime;
}

static inline void psa_set_key_usage_flags(psa_key_attributes_t *attributes,
                                           psa_key_usage_t usage_flags)
{
    psa_extend_key_usage_flags(&usage_flags);
    attributes->usage = usage_flags;
}

static inline psa_key_usage_t psa_get_key_usage_flags(
    const psa_key_attributes_t *attributes)
{
    return attributes->usage;
}

static inline void psa_set_key_algorithm(psa_key_attributes_t *attributes,
                                         psa_algorithm_t alg)
{
    attributes->alg = alg;
}

static inline psa_algorithm_t psa_get_key_algorithm(
    const psa_key_attributes_t *attributes)
{
    return attributes->alg;
}

static inline void psa_set_key_type(psa_key_attributes_t *attributes,
                                    psa_key_type_t type)
{
    attributes->type = type;
}

static inline psa_key_type_t psa_get_key_type(
    const psa_key_attributes_t *attributes)
{
    return attributes->type;
}

static inline void psa_set_key_bits(psa_key_attributes_t *attributes,
                                    size_t bits)
{
    if (bits > PSA_MAX_KEY_BITS) {
        attributes->bits = PSA_KEY_BITS_TOO_LARGE;
    } else {
        attributes->bits = (psa_key_bits_t)bits;
    }
}

static inline size_t psa_get_key_bits(
    const psa_key_attributes_t *attributes)
{
    return attributes->bits;
}

#endif /* #if (TFM_ENABLED == 0) || defined(TFM_BUILD) */

#elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)

/*
 *  ======== psa_set_key_id ========
 */
static inline void psa_set_key_id(psa_key_attributes_t *attributes, psa_key_id_t key)
{
    KeyStore_PSA_setKeyId(attributes, toKeyStoreKeyID(key));
}

/*
 *  ======== psa_get_key_id ========
 */
static inline psa_key_id_t psa_get_key_id(const psa_key_attributes_t *attributes)
{
    return toKeyID(KeyStore_PSA_getKeyId((psa_key_attributes_t *)attributes));
}

/*
 *  ======== psa_set_key_lifetime ========
 */
static inline void psa_set_key_lifetime(psa_key_attributes_t *attributes, psa_key_lifetime_t lifetime)
{
    KeyStore_PSA_setKeyLifetime(attributes, lifetime);
}

/*
 *  ======== psa_get_key_lifetime ========
 */
static inline psa_key_lifetime_t psa_get_key_lifetime(const psa_key_attributes_t *attributes)
{
    return KeyStore_PSA_getKeyLifetime((psa_key_attributes_t *)attributes);
}

/*
 *  ======== psa_set_key_usage_flags ========
 */
static inline void psa_set_key_usage_flags(psa_key_attributes_t *attributes, psa_key_usage_t usage_flags)
{
    KeyStore_PSA_setKeyUsageFlags(attributes, usage_flags);
}

/*
 *  ======== psa_get_key_usage_flags ========
 */
static inline psa_key_usage_t psa_get_key_usage_flags(const psa_key_attributes_t *attributes)
{
    return KeyStore_PSA_getKeyUsageFlags((psa_key_attributes_t *)attributes);
}

/*
 *  ======== psa_set_key_algorithm ========
 */
static inline void psa_set_key_algorithm(psa_key_attributes_t *attributes, psa_algorithm_t alg)
{
    KeyStore_PSA_setKeyAlgorithm(attributes, alg);
}

/*
 *  ======== psa_get_key_algorithm ========
 */
static inline psa_algorithm_t psa_get_key_algorithm(const psa_key_attributes_t *attributes)
{
    return KeyStore_PSA_getKeyAlgorithm((psa_key_attributes_t *)attributes);
}

/*
 *  ======== psa_set_key_type ========
 */
static inline void psa_set_key_type(psa_key_attributes_t *attributes, psa_key_type_t type)
{
    KeyStore_PSA_setKeyType(attributes, type);
}

/*
 *  ======== psa_get_key_type ========
 */
static inline psa_key_type_t psa_get_key_type(const psa_key_attributes_t *attributes)
{
    return KeyStore_PSA_getKeyType((psa_key_attributes_t *)attributes);
}

/*
 *  ======== psa_set_key_bits ========
 */
static inline void psa_set_key_bits(psa_key_attributes_t *attributes, size_t bits)
{
    KeyStore_PSA_setKeyBits(attributes, bits);
}

/*
 *  ======== psa_get_key_bits ========
 */
static inline size_t psa_get_key_bits(const psa_key_attributes_t *attributes)
{
    return KeyStore_PSA_getKeyBits((psa_key_attributes_t *)attributes);
}

#else

#error "Device not supported"

#endif /* DeviceFamily_CC27XX */

/* ############################################################################### */

struct psa_hash_operation_s
{
    /** Unique ID indicating which driver got assigned to do the
     * operation. Since driver contexts are driver-specific, swapping
     * drivers halfway through the operation is not supported.
     * ID values are auto-generated in psa_driver_wrappers.h.
     * ID value zero means the context is not valid or not assigned to
     * any driver (i.e. the driver context is not active, in use). */

    /* this is not used as only one driver is needed */
    unsigned int id;
    /* alg identirifer */
    psa_algorithm_t alg;
    /* added this to prevent out of sequence operations */
    size_t hashSize;
    /* operations set */
    bool hashIsSet;
};

#define PSA_HASH_OPERATION_INIT \
    (psa_hash_operation_t)      \
    {                           \
        0                       \
    }
static inline struct psa_hash_operation_s psa_hash_operation_init(void)
{
    const struct psa_hash_operation_s v = PSA_HASH_OPERATION_INIT;
    return (v);
}

struct psa_cipher_operation_s
{
    /** Unique ID indicating which driver got assigned to do the
     * operation. Since driver contexts are driver-specific, swapping
     * drivers halfway through the operation is not supported.
     * ID values are auto-generated in psa_crypto_driver_wrappers.h
     * ID value zero means the context is not valid or not assigned to
     * any driver (i.e. none of the driver contexts are active). */
    unsigned int id;
    unsigned int iv_required:1;
    unsigned int iv_set:1;
    unsigned int is_encrypt:1;
    unsigned int in_error_state:1;
    uint8_t default_iv_length;

    /* passed in length */
    uint8_t iv_length;
    /* length used by PSA algorithm */
    uint8_t block_length;
    psa_algorithm_t alg;

    /** Number of Bytes that have not been processed yet. */
    size_t unprocessed_len;

    /* buffer for data that has not been processed yet */
    uint8_t unprocessed_data[PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE];

    CryptoKey cryptoKey;

    /** IV size in Bytes, for ciphers with variable-length IVs. */
    size_t iv_size;
};

#define PSA_CIPHER_OPERATION_INIT \
    (psa_cipher_operation_t)      \
    {                             \
        0                         \
    }
static inline struct psa_cipher_operation_s psa_cipher_operation_init(void)
{
    const struct psa_cipher_operation_s v = PSA_CIPHER_OPERATION_INIT;
    return (v);
}

struct psa_mac_operation_s
{
    /** Unique ID indicating which driver got assigned to do the
     * operation. Since driver contexts are driver-specific, swapping
     * drivers halfway through the operation is not supported.
     * ID values are auto-generated in psa_driver_wrappers.h
     * ID value zero means the context is not valid or not assigned to
     * any driver (i.e. none of the driver contexts are active). */
    unsigned int id;
    uint8_t mac_size;
    unsigned int is_sign:1;
    psa_algorithm_t alg;
    size_t unprocessed_len;
    uint8_t unprocessedData[PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE];
    CryptoKey cryptoKey;
    bool lastBlockSet;
    size_t fullMacSize;
};

#define PSA_MAC_OPERATION_INIT \
    (psa_mac_operation_t)      \
    {                          \
        0                      \
    }
static inline struct psa_mac_operation_s psa_mac_operation_init(void)
{
    const struct psa_mac_operation_s v = PSA_MAC_OPERATION_INIT;
    return (v);
}

struct psa_aead_operation_s
{
    /** Unique ID indicating which driver got assigned to do the
     * operation. Since driver contexts are driver-specific, swapping
     * drivers halfway through the operation is not supported.
     * ID values are auto-generated in psa_driver_wrappers.h
     * ID value zero means the context is not valid or not assigned to
     * any driver (i.e. none of the driver contexts are active). */
    unsigned int id;
    psa_algorithm_t alg;
    unsigned int key_set:1;
    unsigned int iv_set:1;
    uint8_t done_updating_ad;
    uint8_t length_set;
    unsigned int adLength;
    unsigned int plaintextLength;
    unsigned int runningADLength;
    unsigned int runningPlaintextLength;
    uint8_t iv_size;
    uint8_t in_error_state;
    uint8_t tagSize;
    uint8_t block_size;
    uint8_t unprocessedData[PSA_BLOCK_CIPHER_BLOCK_MAX_SIZE];
    CryptoKey cryptoKey;
    size_t unprocessed_len;
};

#define PSA_AEAD_OPERATION_INIT \
    (psa_aead_operation_t)      \
    {                           \
        0                       \
    }
static inline struct psa_aead_operation_s psa_aead_operation_init(void)
{
    const struct psa_aead_operation_s v = PSA_AEAD_OPERATION_INIT;
    return (v);
}

#if defined(MBEDTLS_PSA_BUILTIN_ALG_HKDF)
typedef struct
{
    uint8_t *info;
    size_t info_length;
    psa_mac_operation_t hmac;
    uint8_t prk[PSA_HASH_MAX_SIZE];
    uint8_t output_block[PSA_HASH_MAX_SIZE];
    #if PSA_HASH_MAX_SIZE > 0xff
        #error "PSA_HASH_MAX_SIZE does not fit in uint8_t"
    #endif
    uint8_t offset_in_block;
    uint8_t block_number;
    unsigned int state:2;
    unsigned int info_set:1;
} psa_hkdf_key_derivation_t;
#endif /* MBEDTLS_PSA_BUILTIN_ALG_HKDF */

#if defined(MBEDTLS_PSA_BUILTIN_ALG_TLS12_PRF) || defined(MBEDTLS_PSA_BUILTIN_ALG_TLS12_PSK_TO_MS)
typedef enum
{
    PSA_TLS12_PRF_STATE_INIT,      /* no input provided */
    PSA_TLS12_PRF_STATE_SEED_SET,  /* seed has been set */
    PSA_TLS12_PRF_STATE_KEY_SET,   /* key has been set */
    PSA_TLS12_PRF_STATE_LABEL_SET, /* label has been set */
    PSA_TLS12_PRF_STATE_OUTPUT     /* output has been started */
} psa_tls12_prf_key_derivation_state_t;

typedef struct psa_tls12_prf_key_derivation_s
{
    #if PSA_HASH_MAX_SIZE > 0xff
        #error "PSA_HASH_MAX_SIZE does not fit in uint8_t"
    #endif

    /* Indicates how many bytes in the current HMAC block have
     * not yet been read by the user. */
    uint8_t left_in_block;

    /* The 1-based number of the block. */
    uint8_t block_number;

    psa_tls12_prf_key_derivation_state_t state;

    uint8_t *secret;
    size_t secret_length;
    uint8_t *seed;
    size_t seed_length;
    uint8_t *label;
    size_t label_length;

    uint8_t Ai[PSA_HASH_MAX_SIZE];

    /* `HMAC_hash( prk, A(i) + seed )` in the notation of RFC 5246, Sect. 5. */
    uint8_t output_block[PSA_HASH_MAX_SIZE];
} psa_tls12_prf_key_derivation_t;
#endif /* MBEDTLS_PSA_BUILTIN_ALG_TLS12_PRF) || \
        * MBEDTLS_PSA_BUILTIN_ALG_TLS12_PSK_TO_MS */

/** The maximum length of the label used in the Key Derivation Function (KDF). */
#ifndef PSA_KDF_LABEL_MAX_SIZE
#define PSA_KDF_LABEL_MAX_SIZE       (224U - 20U)
#endif
struct psa_key_derivation_s
{
    psa_algorithm_t alg;
    mbedtls_svc_key_id_t inputKey;
    /* Should be at least 53 bytes of label data. Has to also fit an end marker
     * between the label and the context, which is 1 byte (0x00). It should also
     * fit four bytes for the length of the key to derive.
     */
    uint8_t fixedInputData[PSA_KDF_LABEL_MAX_SIZE];
    size_t labelSize;
    size_t contextSize;
    uint32_t handle;
    uint32_t capacity;
    bool canDerive;
    bool capacitySet;
};

#define PSA_KEY_DERIVATION_OPERATION_INIT {0, MBEDTLS_SVC_KEY_ID_INIT, {0, 0, 0, 0, 0}, false, false}
static inline struct psa_key_derivation_s
psa_key_derivation_operation_init(void)
{
    const struct psa_key_derivation_s v = PSA_KEY_DERIVATION_OPERATION_INIT;
    return (v);
}

#ifdef __cplusplus
}
#endif

#endif /* PSA_CRYPTO_STRUCT_H */
