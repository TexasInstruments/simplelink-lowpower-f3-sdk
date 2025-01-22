/**
 * \file psa/crypto_struct.h
 *
 * \brief PSA cryptography module: structured type implementations
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
 */

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*   Module        : DDK-130_bsd                                              */
/*   Version       : 4.1.1                                                    */
/*   Configuration : DDK_EIP130_BSD                                           */
/*                                                                            */
/*   Date          : 2023-Mar-10                                              */
/*                                                                            */
/* Copyright (c) 2014-2023 by Rambus, Inc. and/or its subsidiaries.           */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or without         */
/* modification, are permitted provided that the following conditions are     */
/* met:                                                                       */
/*                                                                            */
/* 1. Redistributions of source code must retain the above copyright          */
/* notice, this list of conditions and the following disclaimer.              */
/*                                                                            */
/* 2. Redistributions in binary form must reproduce the above copyright       */
/* notice, this list of conditions and the following disclaimer in the        */
/* documentation and/or other materials provided with the distribution.       */
/*                                                                            */
/* 3. Neither the name of the copyright holder nor the names of its           */
/* contributors may be used to endorse or promote products derived from       */
/* this software without specific prior written permission.                   */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR      */
/* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT       */
/* HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT           */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,      */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY      */
/* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT        */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE      */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.       */
/* -------------------------------------------------------------------------- */

/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2024, Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*
 * TI Customizations: Transitioned psa_client_key_attributes_s to
 * psa_key_attributes_s to match Mbed TLS v3.5. Also changed
 * psa_key_id_t to mbedtls_svc_key_id_t in order to allow for the
 * encoding of the owner along with the key ID.
 */

#ifndef PSA_CRYPTO_STRUCT_H
#define PSA_CRYPTO_STRUCT_H
#include <third_party/hsmddk/include/Integration/Adapter_PSA/Adapter_mbedTLS/incl/private_access.h>
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_asset.h>
#ifdef __cplusplus
extern "C" {
#endif


/* The structure and initialization for a hash operation. */
struct psa_hash_operation_s
{
    uint32_t alg;
    uint32_t block_size;
    uint32_t TempAssetId;
    uint64_t TotalMessageLength;
    uint32_t leftover_nbytes;
    uint8_t leftover_buf[144];
    uint8_t inter_digest[64];
    uint8_t fUpdateDone;
    uint8_t hash_state;
};

#define PSA_HASH_OPERATION_INIT {0}
static inline struct psa_hash_operation_s
psa_hash_operation_init(void)
{
    const struct psa_hash_operation_s v = PSA_HASH_OPERATION_INIT;
    return (v);
}

/* The structure and initialization for a MAC operation. */
struct psa_mac_operation_s
{
    uint32_t alg;
    mbedtls_svc_key_id_t key;
    uint32_t block_size;
    uint64_t TotalMessageLength;
    uint32_t TempAssetId;
    uint32_t leftover_nbytes;
    uint8_t leftover_buf[144];
    uint8_t fVerify;
    uint8_t fUpdateDone;
    uint8_t mac_state;
};

#define PSA_MAC_OPERATION_INIT {0}
static inline struct psa_mac_operation_s
psa_mac_operation_init(void)
{
    const struct psa_mac_operation_s v = PSA_MAC_OPERATION_INIT;
    return (v);
}

/* The structure and initialization for a symmetric cipher operation. */
struct psa_cipher_operation_s
{
    mbedtls_svc_key_id_t key;
    psa_algorithm_t Mode;
    uint32_t alg;
    uint32_t block_size;
    uint32_t TempAssetId;
    uint32_t leftover_nbytes;
    uint8_t leftover_buf[16];
    uint8_t tweak[16];
    uint8_t cipher_state;
    size_t IVSize;
    uint8_t fEncrypt;
};

#define PSA_CIPHER_OPERATION_INIT {0}
static inline struct psa_cipher_operation_s
psa_cipher_operation_init(void)
{
    const struct psa_cipher_operation_s v = PSA_CIPHER_OPERATION_INIT;
    return (v);
}

/* The structure and initialization for a symmetric authenticated cipher
 * operation. */
struct psa_aead_operation_s
{
    mbedtls_svc_key_id_t key;
    psa_algorithm_t Mode;
    uint32_t plaintext_length;
    uint32_t ad_length;
    uint32_t leftover_nbytes_data;
    uint32_t leftover_nbytes_AAD;
    uint32_t TotalDataLength;
    uint32_t TotalAADLength;
    uint32_t TempAssetId;
    uint8_t nonce[16];
    uint8_t nonce_length;
    uint8_t leftover_data[16];
    uint8_t leftover_AAD[16];
    uint8_t aead_state;
    uint8_t fUpdateDone;
    uint8_t fUpdateAadDone;
    uint8_t fEncrypt;
};

#define PSA_AEAD_OPERATION_INIT {0}
static inline struct psa_aead_operation_s
psa_aead_operation_init(void)
{
    const struct psa_aead_operation_s v = PSA_AEAD_OPERATION_INIT;
    return (v);
}

#ifndef PSA_KDF_LABEL_MAX_SIZE
#define PSA_KDF_LABEL_MAX_SIZE       (224U - 20U)
#endif
/* The structure and initialization for key derivation operation(s). */
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


/* The type used internally for key sizes.
 * Public interfaces use size_t, but internally a smaller type is used. */
typedef uint16_t psa_key_bits_t;
/* The maximum value of the type used to represent bit-sizes.
 * This is used to mark an invalid key size. */
#define PSA_KEY_BITS_TOO_LARGE ( (psa_key_bits_t) ( -1 ) )
/* The maximum size of a key in bits.
 * Currently defined as the maximum that can be represented, rounded down
 * to a whole number of bytes.
 * This is an uncast value so that it can be used in preprocessor
 * conditionals. */
#define PSA_MAX_KEY_BITS 0xfff8U

/** A mask of flags that can be stored in key attributes.
 *
 * This type is also used internally to store flags in slots. Internal
 * flags are defined in library/psa_crypto_core.h. Internal flags may have
 * the same value as external flags if they are properly handled during
 * key creation and in psa_get_key_attributes.
 */
typedef uint16_t psa_key_attributes_flag_t;

#define PSA_KEY_POLICY_INIT { 0, 0, 0 }

#define PSA_CORE_KEY_ATTRIBUTES_INIT { PSA_KEY_TYPE_NONE, 0,            \
                                       PSA_KEY_LIFETIME_VOLATILE,       \
                                       MBEDTLS_SVC_KEY_ID_INIT,         \
                                       PSA_KEY_POLICY_INIT, 0 }

#define PSA_KEY_ATTRIBUTES_INIT { PSA_CORE_KEY_ATTRIBUTES_INIT, NULL, 0 }

struct psa_key_policy_s {
    psa_key_usage_t MBEDTLS_PRIVATE(usage);
    psa_algorithm_t MBEDTLS_PRIVATE(alg);
    psa_algorithm_t MBEDTLS_PRIVATE(alg2);
};
typedef struct psa_key_policy_s psa_key_policy_t;

typedef struct {
    psa_key_type_t MBEDTLS_PRIVATE(type);
    psa_key_bits_t MBEDTLS_PRIVATE(bits);
    psa_key_lifetime_t MBEDTLS_PRIVATE(lifetime);
    mbedtls_svc_key_id_t MBEDTLS_PRIVATE(id);
    psa_key_policy_t MBEDTLS_PRIVATE(policy);
    psa_key_attributes_flag_t MBEDTLS_PRIVATE(flags);
} psa_core_key_attributes_t;

struct psa_key_attributes_s {
    psa_core_key_attributes_t MBEDTLS_PRIVATE(core);
    void *MBEDTLS_PRIVATE(domain_parameters);
    size_t MBEDTLS_PRIVATE(domain_parameters_size);
};

static inline struct psa_key_attributes_s
psa_key_attributes_init(void)
{
    const struct psa_key_attributes_s v = PSA_KEY_ATTRIBUTES_INIT;
    return (v);
}

static inline void
psa_set_key_id(psa_key_attributes_t *attributes,
               mbedtls_svc_key_id_t key)
{
    psa_key_lifetime_t lifetime = attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(lifetime);

#ifdef MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER
    attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(id).MBEDTLS_PRIVATE(key_id) = key.MBEDTLS_PRIVATE(key_id);
    attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(id).MBEDTLS_PRIVATE(owner) = key.MBEDTLS_PRIVATE(owner);
#else
    attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(id) = key;
#endif

    if (PSA_KEY_LIFETIME_IS_VOLATILE(lifetime)) {
        attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(lifetime) =
            PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(
                PSA_KEY_LIFETIME_PERSISTENT,
                PSA_KEY_LIFETIME_GET_LOCATION(lifetime));
    }
}

static inline mbedtls_svc_key_id_t
psa_get_key_id(const psa_key_attributes_t *attributes)
{
    return attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(id);
}

static inline void
psa_set_key_lifetime(psa_key_attributes_t *attributes,
                     psa_key_lifetime_t lifetime)
{
    attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(lifetime) = lifetime;
    if (PSA_KEY_LIFETIME_IS_VOLATILE(lifetime)) {
#ifdef MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER
        attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(id).MBEDTLS_PRIVATE(key_id) = 0;
#else
        attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(id) = 0;
#endif
    }
}

static inline psa_key_lifetime_t
psa_get_key_lifetime(const psa_key_attributes_t *attributes)
{
    return attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(lifetime);
}

static inline void
psa_extend_key_usage_flags(psa_key_usage_t *usage_flags)
{
    if (*usage_flags & PSA_KEY_USAGE_SIGN_HASH) {
        *usage_flags |= PSA_KEY_USAGE_SIGN_MESSAGE;
    }

    if (*usage_flags & PSA_KEY_USAGE_VERIFY_HASH) {
        *usage_flags |= PSA_KEY_USAGE_VERIFY_MESSAGE;
    }
}

static inline void
psa_set_key_usage_flags(psa_key_attributes_t *attributes,
                        psa_key_usage_t usage_flags)
{
    psa_extend_key_usage_flags(&usage_flags);
    attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(policy).MBEDTLS_PRIVATE(usage) = usage_flags;
}

static inline psa_key_usage_t
psa_get_key_usage_flags(const psa_key_attributes_t *attributes)
{
    return attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(policy).MBEDTLS_PRIVATE(usage);
}

static inline void
psa_set_key_algorithm(psa_key_attributes_t *attributes,
                      psa_algorithm_t alg)
{
    attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(policy).MBEDTLS_PRIVATE(alg) = alg;
}

static inline psa_algorithm_t
psa_get_key_algorithm(const psa_key_attributes_t *attributes)
{
    return attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(policy).MBEDTLS_PRIVATE(alg);
}

psa_status_t psa_set_key_domain_parameters(psa_key_attributes_t *attributes,
                                           psa_key_type_t type,
                                           const uint8_t *data,
                                           size_t data_length);

static inline void
psa_set_key_type(psa_key_attributes_t *attributes,
                 psa_key_type_t type)
{
    if (attributes->MBEDTLS_PRIVATE(domain_parameters) == NULL) {
        /* Common case: quick path */
        attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(type) = type;
    } else {
        /* Call the bigger function to free the old domain parameters.
         * Ignore any errors which may arise due to type requiring
         * non-default domain parameters, since this function can't
         * report errors. */
        (void) psa_set_key_domain_parameters(attributes, type, NULL, 0);
    }
}

static inline psa_key_type_t
psa_get_key_type(const psa_key_attributes_t *attributes)
{
    return attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(type);
}

static inline void
psa_set_key_bits(psa_key_attributes_t *attributes,
                 size_t bits)
{
    if (bits > PSA_MAX_KEY_BITS) {
        attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(bits) = PSA_KEY_BITS_TOO_LARGE;
    } else {
        attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(bits) = (psa_key_bits_t) bits;
    }
}

static inline size_t
psa_get_key_bits(const psa_key_attributes_t *attributes)
{
    return attributes->MBEDTLS_PRIVATE(core).MBEDTLS_PRIVATE(bits);
}

#ifdef __cplusplus
}
#endif

#endif /* PSA_CRYPTO_STRUCT_H */

/* end of file crypto_struct.h */
