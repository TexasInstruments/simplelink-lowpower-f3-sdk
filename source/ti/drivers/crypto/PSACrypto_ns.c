/*
 * Copyright (c) 2025, Texas Instruments Incorporated - http://www.ti.com
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

#include <string.h>

#include <ti/drivers/crypto/CryptoTFM_ns.h>
#include <ti/drivers/crypto/PSACrypto_s.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/power/PowerCC27XX.h>

#include <third_party/tfm/interface/include/psa/crypto.h>

/* TFM's PSA API interface prefixes the function names with 'tfm_crypto__' */
#define TFM_CRYPTO_API(func) tfm_crypto__##func

/* This macro implements a blocking return behavior PSA function waiting for a callback */
#define NS_WRAP_CALL_WITH_RETURN_CALLBACK(func, ...)         \
    ({                                                       \
        psa_status_t status;                                 \
                                                             \
        Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);     \
                                                             \
        /* Call corresponding TFM function */                \
        status = TFM_CRYPTO_API(func)(__VA_ARGS__);          \
                                                             \
        if (status == PSA_SUCCESS)                           \
        {                                                    \
            status = PSACrypto_ns_waitForResult();           \
        }                                                    \
        else if (status == PSA_OPERATION_COMPLETE)           \
        {                                                    \
            status = PSA_SUCCESS;                            \
        }                                                    \
                                                             \
        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY); \
                                                             \
        return status;                                       \
    })

/* This macro implements a polling return behavior PSA function */
#define NS_WRAP_CALL_WITH_RETURN(func, ...)       \
    ({                                            \
        /* Call corresponding TFM function */     \
        return TFM_CRYPTO_API(func)(__VA_ARGS__); \
    })

/* This macro implements a polling return behavior without a return value */
#define NS_WRAP_CALL(func, ...)               \
    ({                                        \
        /* Call corresponding TFM function */ \
        TFM_CRYPTO_API(func)(__VA_ARGS__);    \
    })

static bool PSACrypto_isInitialized = false;

static PSACrypto_s_SecureCallback PSACrypto_secureCB;

extern SemaphoreP_Struct CryptoTFM_ns_operationSemaphore;

/* Extern tfm_crypto__ functions */
extern psa_status_t tfm_crypto__psa_get_key_attributes(psa_key_id_t key, psa_key_attributes_t *attributes);

extern void tfm_crypto__psa_reset_key_attributes(psa_key_attributes_t *attributes);

extern psa_status_t tfm_crypto__psa_purge_key(psa_key_id_t key);

extern psa_status_t tfm_crypto__psa_copy_key(psa_key_id_t source_key,
                                             const psa_key_attributes_t *attributes,
                                             psa_key_id_t *target_key);

extern psa_status_t tfm_crypto__psa_destroy_key(psa_key_id_t key);

extern psa_status_t tfm_crypto__psa_import_key(const psa_key_attributes_t *attributes,
                                               const uint8_t *data,
                                               size_t data_length,
                                               psa_key_id_t *key);

extern psa_status_t tfm_crypto__psa_export_key(psa_key_id_t key, uint8_t *data, size_t data_size, size_t *data_length);

extern psa_status_t tfm_crypto__psa_export_public_key(psa_key_id_t key,
                                                      uint8_t *data,
                                                      size_t data_size,
                                                      size_t *data_length);

extern psa_status_t tfm_crypto__psa_hash_compute(psa_algorithm_t alg,
                                                 const uint8_t *input,
                                                 size_t input_length,
                                                 uint8_t *hash,
                                                 size_t hash_size,
                                                 size_t *hash_length);

extern psa_status_t tfm_crypto__psa_hash_compare(psa_algorithm_t alg,
                                                 const uint8_t *input,
                                                 size_t input_length,
                                                 const uint8_t *hash,
                                                 size_t hash_length);

extern psa_status_t tfm_crypto__psa_hash_setup(psa_hash_operation_t *operation, psa_algorithm_t alg);

extern psa_status_t tfm_crypto__psa_hash_update(psa_hash_operation_t *operation,
                                                const uint8_t *input,
                                                size_t input_length);

extern psa_status_t tfm_crypto__psa_hash_finish(psa_hash_operation_t *operation,
                                                uint8_t *hash,
                                                size_t hash_size,
                                                size_t *hash_length);

extern psa_status_t tfm_crypto__psa_hash_verify(psa_hash_operation_t *operation,
                                                const uint8_t *hash,
                                                size_t hash_length);

extern psa_status_t tfm_crypto__psa_hash_abort(psa_hash_operation_t *operation);

extern psa_status_t tfm_crypto__psa_hash_clone(const psa_hash_operation_t *source_operation,
                                               psa_hash_operation_t *target_operation);

extern psa_status_t tfm_crypto__psa_mac_compute(psa_key_id_t key,
                                                psa_algorithm_t alg,
                                                const uint8_t *input,
                                                size_t input_length,
                                                uint8_t *mac,
                                                size_t mac_size,
                                                size_t *mac_length);

extern psa_status_t tfm_crypto__psa_mac_verify(psa_key_id_t key,
                                               psa_algorithm_t alg,
                                               const uint8_t *input,
                                               size_t input_length,
                                               const uint8_t *mac,
                                               size_t mac_length);

extern psa_status_t tfm_crypto__psa_mac_sign_setup(psa_mac_operation_t *operation,
                                                   psa_key_id_t key,
                                                   psa_algorithm_t alg);

extern psa_status_t tfm_crypto__psa_mac_verify_setup(psa_mac_operation_t *operation,
                                                     psa_key_id_t key,
                                                     psa_algorithm_t alg);

extern psa_status_t tfm_crypto__psa_mac_update(psa_mac_operation_t *operation,
                                               const uint8_t *input,
                                               size_t input_length);

extern psa_status_t tfm_crypto__psa_mac_sign_finish(psa_mac_operation_t *operation,
                                                    uint8_t *mac,
                                                    size_t mac_size,
                                                    size_t *mac_length);

extern psa_status_t tfm_crypto__psa_mac_verify_finish(psa_mac_operation_t *operation,
                                                      const uint8_t *mac,
                                                      size_t mac_length);

extern psa_status_t tfm_crypto__psa_mac_abort(psa_mac_operation_t *operation);

extern psa_status_t tfm_crypto__psa_cipher_encrypt(psa_key_id_t key,
                                                   psa_algorithm_t alg,
                                                   const uint8_t *input,
                                                   size_t input_length,
                                                   uint8_t *output,
                                                   size_t output_size,
                                                   size_t *output_length);

extern psa_status_t tfm_crypto__psa_cipher_decrypt(psa_key_id_t key,
                                                   psa_algorithm_t alg,
                                                   const uint8_t *input,
                                                   size_t input_length,
                                                   uint8_t *output,
                                                   size_t output_size,
                                                   size_t *output_length);

extern psa_status_t tfm_crypto__psa_cipher_encrypt_setup(psa_cipher_operation_t *operation,
                                                         psa_key_id_t key,
                                                         psa_algorithm_t alg);

extern psa_status_t tfm_crypto__psa_cipher_decrypt_setup(psa_cipher_operation_t *operation,
                                                         psa_key_id_t key,
                                                         psa_algorithm_t alg);

extern psa_status_t tfm_crypto__psa_cipher_generate_iv(psa_cipher_operation_t *operation,
                                                       uint8_t *iv,
                                                       size_t iv_size,
                                                       size_t *iv_length);

extern psa_status_t tfm_crypto__psa_cipher_set_iv(psa_cipher_operation_t *operation,
                                                  const uint8_t *iv,
                                                  size_t iv_length);

extern psa_status_t tfm_crypto__psa_cipher_update(psa_cipher_operation_t *operation,
                                                  const uint8_t *input,
                                                  size_t input_length,
                                                  uint8_t *output,
                                                  size_t output_size,
                                                  size_t *output_length);

extern psa_status_t tfm_crypto__psa_cipher_finish(psa_cipher_operation_t *operation,
                                                  uint8_t *output,
                                                  size_t output_size,
                                                  size_t *output_length);

extern psa_status_t tfm_crypto__psa_cipher_abort(psa_cipher_operation_t *operation);

extern psa_status_t tfm_crypto__psa_aead_encrypt(psa_key_id_t key,
                                                 psa_algorithm_t alg,
                                                 const uint8_t *nonce,
                                                 size_t nonce_length,
                                                 const uint8_t *additional_data,
                                                 size_t additional_data_length,
                                                 const uint8_t *plaintext,
                                                 size_t plaintext_length,
                                                 uint8_t *ciphertext,
                                                 size_t ciphertext_size,
                                                 size_t *ciphertext_length);

extern psa_status_t tfm_crypto__psa_aead_decrypt(psa_key_id_t key,
                                                 psa_algorithm_t alg,
                                                 const uint8_t *nonce,
                                                 size_t nonce_length,
                                                 const uint8_t *additional_data,
                                                 size_t additional_data_length,
                                                 const uint8_t *ciphertext,
                                                 size_t ciphertext_length,
                                                 uint8_t *plaintext,
                                                 size_t plaintext_size,
                                                 size_t *plaintext_length);

extern psa_status_t tfm_crypto__psa_aead_encrypt_setup(psa_aead_operation_t *operation,
                                                       psa_key_id_t key,
                                                       psa_algorithm_t alg);

extern psa_status_t tfm_crypto__psa_aead_decrypt_setup(psa_aead_operation_t *operation,
                                                       psa_key_id_t key,
                                                       psa_algorithm_t alg);

extern psa_status_t tfm_crypto__psa_aead_generate_nonce(psa_aead_operation_t *operation,
                                                        uint8_t *nonce,
                                                        size_t nonce_size,
                                                        size_t *nonce_length);

extern psa_status_t tfm_crypto__psa_aead_set_nonce(psa_aead_operation_t *operation,
                                                   const uint8_t *nonce,
                                                   size_t nonce_length);

extern psa_status_t tfm_crypto__psa_aead_set_lengths(psa_aead_operation_t *operation,
                                                     size_t ad_length,
                                                     size_t plaintext_length);

extern psa_status_t tfm_crypto__psa_aead_update_ad(psa_aead_operation_t *operation,
                                                   const uint8_t *input,
                                                   size_t input_length);

extern psa_status_t tfm_crypto__psa_aead_update(psa_aead_operation_t *operation,
                                                const uint8_t *input,
                                                size_t input_length,
                                                uint8_t *output,
                                                size_t output_size,
                                                size_t *output_length);

extern psa_status_t tfm_crypto__psa_aead_finish(psa_aead_operation_t *operation,
                                                uint8_t *ciphertext,
                                                size_t ciphertext_size,
                                                size_t *ciphertext_length,
                                                uint8_t *tag,
                                                size_t tag_size,
                                                size_t *tag_length);

extern psa_status_t tfm_crypto__psa_aead_verify(psa_aead_operation_t *operation,
                                                uint8_t *plaintext,
                                                size_t plaintext_size,
                                                size_t *plaintext_length,
                                                const uint8_t *tag,
                                                size_t tag_length);

extern psa_status_t tfm_crypto__psa_aead_abort(psa_aead_operation_t *operation);

extern psa_status_t tfm_crypto__psa_sign_message(psa_key_id_t key,
                                                 psa_algorithm_t alg,
                                                 const uint8_t *input,
                                                 size_t input_length,
                                                 uint8_t *signature,
                                                 size_t signature_size,
                                                 size_t *signature_length);

extern psa_status_t tfm_crypto__psa_verify_message(psa_key_id_t key,
                                                   psa_algorithm_t alg,
                                                   const uint8_t *input,
                                                   size_t input_length,
                                                   const uint8_t *signature,
                                                   size_t signature_length);

extern psa_status_t tfm_crypto__psa_sign_hash(psa_key_id_t key,
                                              psa_algorithm_t alg,
                                              const uint8_t *hash,
                                              size_t hash_length,
                                              uint8_t *signature,
                                              size_t signature_size,
                                              size_t *signature_length);

extern psa_status_t tfm_crypto__psa_verify_hash(psa_key_id_t key,
                                                psa_algorithm_t alg,
                                                const uint8_t *hash,
                                                size_t hash_length,
                                                const uint8_t *signature,
                                                size_t signature_length);

extern psa_status_t tfm_crypto__psa_asymmetric_encrypt(psa_key_id_t key,
                                                       psa_algorithm_t alg,
                                                       const uint8_t *input,
                                                       size_t input_length,
                                                       const uint8_t *salt,
                                                       size_t salt_length,
                                                       uint8_t *output,
                                                       size_t output_size,
                                                       size_t *output_length);

extern psa_status_t tfm_crypto__psa_asymmetric_decrypt(psa_key_id_t key,
                                                       psa_algorithm_t alg,
                                                       const uint8_t *input,
                                                       size_t input_length,
                                                       const uint8_t *salt,
                                                       size_t salt_length,
                                                       uint8_t *output,
                                                       size_t output_size,
                                                       size_t *output_length);

extern psa_status_t tfm_crypto__psa_key_derivation_setup(psa_key_derivation_operation_t *operation,
                                                         psa_algorithm_t alg);

extern psa_status_t tfm_crypto__psa_key_derivation_get_capacity(const psa_key_derivation_operation_t *operation,
                                                                size_t *capacity);

extern psa_status_t tfm_crypto__psa_key_derivation_set_capacity(psa_key_derivation_operation_t *operation,
                                                                size_t capacity);

extern psa_status_t tfm_crypto__psa_key_derivation_input_bytes(psa_key_derivation_operation_t *operation,
                                                               psa_key_derivation_step_t step,
                                                               const uint8_t *data,
                                                               size_t data_length);

extern psa_status_t tfm_crypto__psa_key_derivation_input_key(psa_key_derivation_operation_t *operation,
                                                             psa_key_derivation_step_t step,
                                                             psa_key_id_t key);

extern psa_status_t tfm_crypto__psa_key_derivation_key_agreement(psa_key_derivation_operation_t *operation,
                                                                 psa_key_derivation_step_t step,
                                                                 psa_key_id_t private_key,
                                                                 const uint8_t *peer_key,
                                                                 size_t peer_key_length);

extern psa_status_t tfm_crypto__psa_key_derivation_output_bytes(psa_key_derivation_operation_t *operation,
                                                                uint8_t *output,
                                                                size_t output_length);

extern psa_status_t tfm_crypto__psa_key_derivation_output_key(const psa_key_attributes_t *attributes,
                                                              psa_key_derivation_operation_t *operation,
                                                              psa_key_id_t *key);

extern psa_status_t tfm_crypto__psa_key_derivation_abort(psa_key_derivation_operation_t *operation);

extern psa_status_t tfm_crypto__psa_raw_key_agreement(psa_algorithm_t alg,
                                                      psa_key_id_t private_key,
                                                      const uint8_t *peer_key,
                                                      size_t peer_key_length,
                                                      uint8_t *output,
                                                      size_t output_size,
                                                      size_t *output_length);

extern psa_status_t tfm_crypto__psa_generate_random(uint8_t *output, size_t output_size);

extern psa_status_t tfm_crypto__psa_generate_key(const psa_key_attributes_t *attributes, psa_key_id_t *key);

/*
 *  ======== tfm_crypto__psa_key_derivation_input_integer ========
 */
psa_status_t tfm_crypto__psa_key_derivation_input_integer(psa_key_derivation_operation_t *operation,
                                                          psa_key_derivation_step_t step,
                                                          uint64_t value)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

/*
 *  ======== tfm_crypto__psa_key_derivation_verify_bytes ========
 */
psa_status_t tfm_crypto__psa_key_derivation_verify_bytes(psa_key_derivation_operation_t *operation,
                                                         const uint8_t *expected_output,
                                                         size_t output_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

/*
 *  ======== tfm_crypto__psa_key_derivation_verify_key ========
 */
psa_status_t tfm_crypto__psa_key_derivation_verify_key(psa_key_derivation_operation_t *operation, psa_key_id_t expected)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

/*
 *  ======== tfm_crypto__psa_hash_resume ========
 */
psa_status_t tfm_crypto__psa_hash_resume(psa_hash_operation_t *operation,
                                         const uint8_t *hash_state,
                                         size_t hash_state_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

/*
 *  ======== tfm_crypto__psa_hash_suspend ========
 */
psa_status_t tfm_crypto__psa_hash_suspend(psa_hash_operation_t *operation,
                                          uint8_t *hash_state,
                                          size_t hash_state_size,
                                          size_t *hash_state_length)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

/*
 *  ======== PSACrypto_ns_callbackFxn ========
 */
void PSACrypto_ns_callbackFxn(uintptr_t arg)
{
    (void)arg;

    /* Unblock the pending task */
    SemaphoreP_post(&CryptoTFM_ns_operationSemaphore);
}

/*
 *  ======== PSACrypto_ns_registerCallback ========
 */
static psa_status_t PSACrypto_ns_registerCallback(void)
{
    PSACrypto_s_SecureCallback *callback = &PSACrypto_secureCB;
    struct psa_invec invecs[1];
    struct psa_outvec outvecs[1];

    /* Construct SecureCallback object */
    SecureCallback_construct(&PSACrypto_secureCB.object, PSACrypto_ns_callbackFxn, (uintptr_t)NULL);

    /* Initialize handle */
    PSACrypto_secureCB.tfmCryptoOperationHandle = NULL;

    /* Pointer to secure callback struct */
    invecs[0].base = &callback;
    invecs[0].len  = sizeof(callback);

    /* Setup interface for return value */
    outvecs[0].base = NULL;
    outvecs[0].len  = 0;

    /* PSA call to secure driver */
    return CryptoTFM_ns_call(PSA_CRYPTO_REGISTER_SECURE_CALLBACK_TYPE, invecs, outvecs);
}

/*
 *  ======== PSACrypto_ns_waitForResult ========
 */
static psa_status_t PSACrypto_ns_waitForResult(void)
{
    /* Block task with operation semaphore */
    SemaphoreP_pend(&CryptoTFM_ns_operationSemaphore, SemaphoreP_WAIT_FOREVER);

    /* After the secure callback posts the operation semaphore to unblock the
     * task, return the status.
     */
    return PSACrypto_secureCB.status;
}

/*
 *  ======== psa_crypto_init ========
 */
/* The PSA Crypto Wrapper in mbedTLS does not define this API, so this definition does not
 * need to be prefixed to avoid collision.
 */
psa_status_t psa_crypto_init(void)
{
    psa_status_t status = PSA_SUCCESS;

    if (!PSACrypto_isInitialized)
    {
        CryptoTFM_ns_init();

        /* Register secure callback */
        status = PSACrypto_ns_registerCallback();

        PSACrypto_isInitialized = true;
    }

    /* PSA Crypto within the TFM is automatically initialized during startup so
     * no call the secure side is required.
     */

    return status;
}

/*
 *  ======== ti_psa_get_key_attributes ========
 */
psa_status_t ti_psa_get_key_attributes(psa_key_id_t key, psa_key_attributes_t *attributes)
{
    NS_WRAP_CALL_WITH_RETURN(psa_get_key_attributes, key, attributes);
}

/*
 *  ======== ti_psa_reset_key_attributes ========
 */
void ti_psa_reset_key_attributes(psa_key_attributes_t *attributes)
{
    NS_WRAP_CALL(psa_reset_key_attributes, attributes);
}

/*
 *  ======== ti_psa_purge_key ========
 */
psa_status_t ti_psa_purge_key(psa_key_id_t key)
{
    NS_WRAP_CALL_WITH_RETURN(psa_purge_key, key);
}

/*
 *  ======== ti_psa_copy_key ========
 */
psa_status_t ti_psa_copy_key(psa_key_id_t source_key, const psa_key_attributes_t *attributes, psa_key_id_t *target_key)
{
    NS_WRAP_CALL_WITH_RETURN(psa_copy_key, source_key, attributes, target_key);
}

/*
 *  ======== ti_psa_destroy_key ========
 */
psa_status_t ti_psa_destroy_key(psa_key_id_t key)
{
    NS_WRAP_CALL_WITH_RETURN(psa_destroy_key, key);
}

/*
 *  ======== ti_psa_import_key ========
 */
psa_status_t ti_psa_import_key(const psa_key_attributes_t *attributes,
                               const uint8_t *data,
                               size_t data_length,
                               psa_key_id_t *key)
{
    NS_WRAP_CALL_WITH_RETURN(psa_import_key, attributes, data, data_length, key);
}

/*
 *  ======== ti_psa_export_key ========
 */
psa_status_t ti_psa_export_key(psa_key_id_t key, uint8_t *data, size_t data_size, size_t *data_length)
{
    NS_WRAP_CALL_WITH_RETURN(psa_export_key, key, data, data_size, data_length);
}

/*
 *  ======== ti_psa_export_public_key ========
 */
psa_status_t ti_psa_export_public_key(psa_key_id_t key, uint8_t *data, size_t data_size, size_t *data_length)
{
    NS_WRAP_CALL_WITH_RETURN(psa_export_public_key, key, data, data_size, data_length);
}

/*
 *  ======== ti_psa_hash_compute ========
 */
psa_status_t ti_psa_hash_compute(psa_algorithm_t alg,
                                 const uint8_t *input,
                                 size_t input_length,
                                 uint8_t *hash,
                                 size_t hash_size,
                                 size_t *hash_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_hash_compute, alg, input, input_length, hash, hash_size, hash_length);
}

/*
 *  ======== ti_psa_hash_compare ========
 */
psa_status_t ti_psa_hash_compare(psa_algorithm_t alg,
                                 const uint8_t *input,
                                 size_t input_length,
                                 const uint8_t *hash,
                                 size_t hash_length)
{
    psa_status_t status;
    int rc;

    /* Call corresponding TFM function */
    status = TFM_CRYPTO_API(psa_hash_compare)(alg, input, input_length, hash, hash_length);

    if (status == PSA_SUCCESS)
    {
        status = PSACrypto_ns_waitForResult();
    }

    if (status != PSA_SUCCESS)
    {
        /* In case the length was not correct for the output, we report a
         * invalid signature. Check crypto_values.h under the
         * PSA_ERROR_INVALID_ARGUMENT macro for further explanation (Only in
         * compare operation).
         */
        if (status == PSA_ERROR_BUFFER_TOO_SMALL)
        {
            status = PSA_ERROR_INVALID_ARGUMENT;
        }
    }
    else
    {
        /* Compare the actual hash with the provided hash */
        rc = memcmp(&PSACrypto_secureCB.buffer[0], hash, hash_length);

        if (rc != 0)
        {
            status = PSA_ERROR_INVALID_SIGNATURE;
        }
    }

    return status;
}

/*
 *  ======== ti_psa_hash_setup ========
 */
psa_status_t ti_psa_hash_setup(psa_hash_operation_t *operation, psa_algorithm_t alg)
{
    NS_WRAP_CALL_WITH_RETURN(psa_hash_setup, operation, alg);
}

/*
 *  ======== ti_psa_hash_update ========
 */
psa_status_t ti_psa_hash_update(psa_hash_operation_t *operation, const uint8_t *input, size_t input_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_hash_update, operation, input, input_length);
}

/*
 *  ======== ti_psa_hash_finish ========
 */
psa_status_t ti_psa_hash_finish(psa_hash_operation_t *operation, uint8_t *hash, size_t hash_size, size_t *hash_length)
{
    psa_status_t status;

    /* Store pointer to the handle so it can be used to free the operation context in the SPE */
    PSACrypto_secureCB.tfmCryptoOperationHandle = &operation->handle;

    /* Call corresponding TFM function */
    status = TFM_CRYPTO_API(psa_hash_finish)(operation, hash, hash_size, hash_length);

    if (status == PSA_SUCCESS)
    {
        status = PSACrypto_ns_waitForResult();
    }

    /* Per the PSA Crypto API spec: If the function returns an error, the
     * application is responsible for calling psa_hash_abort() to cleanup and
     * free the allocated operation context in the SPE.
     */

    return status;
}

/*
 *  ======== ti_psa_hash_verify ========
 */
psa_status_t ti_psa_hash_verify(psa_hash_operation_t *operation, const uint8_t *hash, size_t hash_length)
{
    psa_status_t status;
    size_t actualHashLen;

    /* Store pointer to the handle so it can be used to free the operation context in the SPE */
    PSACrypto_secureCB.tfmCryptoOperationHandle = &operation->handle;

    /* Call corresponding TFM function */
    status = TFM_CRYPTO_API(psa_hash_verify)(operation, hash, hash_length);

    if (status == PSA_SUCCESS)
    {
        status = PSACrypto_ns_waitForResult();
    }

    if (status == PSA_SUCCESS)
    {
        actualHashLen = PSACrypto_secureCB.length;

        /* For verification the hash_length provided must be the same as the
         * actual_hash_length.
         */
        if (actualHashLen != hash_length)
        {
            status = PSA_ERROR_INVALID_SIGNATURE;
        }
        else
        {
            status = memcmp(hash, &PSACrypto_secureCB.buffer[0], actualHashLen);

            if (status != 0)
            {
                status = PSA_ERROR_INVALID_SIGNATURE;
            }
            else
            {
                status = PSA_SUCCESS;
            }
        }
    }

    /* Per the PSA Crypto API spec: If the function returns an error, the
     * application is responsible for calling psa_hash_abort() to cleanup and
     * free the allocated operation context in the SPE.
     */

    return status;
}

/*
 *  ======== ti_psa_hash_abort ========
 */
psa_status_t ti_psa_hash_abort(psa_hash_operation_t *operation)
{
    NS_WRAP_CALL_WITH_RETURN(psa_hash_abort, operation);
}

/*
 *  ======== ti_psa_hash_clone ========
 */
psa_status_t ti_psa_hash_clone(const psa_hash_operation_t *source_operation, psa_hash_operation_t *target_operation)
{
    NS_WRAP_CALL_WITH_RETURN(psa_hash_clone, source_operation, target_operation);
}

/*
 *  ======== ti_psa_mac_compute ========
 */
psa_status_t ti_psa_mac_compute(psa_key_id_t key,
                                psa_algorithm_t alg,
                                const uint8_t *input,
                                size_t input_length,
                                uint8_t *mac,
                                size_t mac_size,
                                size_t *mac_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_mac_compute, key, alg, input, input_length, mac, mac_size, mac_length);
}

/*
 *  ======== ti_psa_mac_verify ========
 */
psa_status_t ti_psa_mac_verify(psa_key_id_t key,
                               psa_algorithm_t alg,
                               const uint8_t *input,
                               size_t input_length,
                               const uint8_t *mac,
                               size_t mac_length)
{
    psa_status_t status;
    int rc;

    /* Call corresponding TFM function */
    status = TFM_CRYPTO_API(psa_mac_verify)(key, alg, input, input_length, mac, mac_length);

    if (status == PSA_SUCCESS)
    {
        status = PSACrypto_ns_waitForResult();
    }

    if (PSA_ALG_IS_HMAC(alg) && (status == PSA_SUCCESS))
    {
        /* For HMAC, compare the actual mac with the provided mac */
        rc = memcmp(&PSACrypto_secureCB.buffer[0], mac, mac_length);

        if (rc != 0)
        {
            status = PSA_ERROR_INVALID_SIGNATURE;
        }
    }

    return status;
}

/*
 *  ======== ti_psa_mac_sign_setup ========
 */
psa_status_t ti_psa_mac_sign_setup(psa_mac_operation_t *operation, psa_key_id_t key, psa_algorithm_t alg)
{
    NS_WRAP_CALL_WITH_RETURN(psa_mac_sign_setup, operation, key, alg);
}

/*
 *  ======== ti_psa_mac_verify_setup ========
 */
psa_status_t ti_psa_mac_verify_setup(psa_mac_operation_t *operation, psa_key_id_t key, psa_algorithm_t alg)
{
    /* Store the algorithm in NS client's operation struct */
    operation->alg = PSA_ALG_FULL_LENGTH_MAC(alg);

    NS_WRAP_CALL_WITH_RETURN(psa_mac_verify_setup, operation, key, alg);
}

/*
 *  ======== ti_psa_mac_update ========
 */
psa_status_t ti_psa_mac_update(psa_mac_operation_t *operation, const uint8_t *input, size_t input_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_mac_update, operation, input, input_length);
}

/*
 *  ======== ti_psa_mac_sign_finish ========
 */
psa_status_t ti_psa_mac_sign_finish(psa_mac_operation_t *operation, uint8_t *mac, size_t mac_size, size_t *mac_length)
{
    psa_status_t status;
    size_t opMacSize;

    /* Store pointer to the handle so it can be used to free the operation context in the SPE */
    PSACrypto_secureCB.tfmCryptoOperationHandle = &operation->handle;

    /* Call corresponding TFM function */
    status = TFM_CRYPTO_API(psa_mac_sign_finish)(operation, mac, mac_size, mac_length);

    if (status == PSA_SUCCESS)
    {
        status = PSACrypto_ns_waitForResult();
    }

    if (status == PSA_SUCCESS)
    {
        opMacSize = PSACrypto_secureCB.length;
    }
    else
    {
        opMacSize = 0;
    }

    *mac_length = opMacSize;

    /* In case of success, set the potential excess room in the output buffer to
     * an invalid value, to avoid potentially leaking a longer MAC. In case of
     * error, set the output length and content to a safe default, such that in
     * case the caller misses an error check, the output would be an
     * unachievable MAC.
     */
    if (mac_size > opMacSize)
    {
        (void)memset(&mac[opMacSize], '!', mac_size - opMacSize);
    }

    /* Per the PSA Crypto API spec: If the function returns an error, the
     * application is responsible for calling psa_mac_abort() to cleanup and
     * free the allocated operation context in the SPE.
     */

    return status;
}

/*
 *  ======== ti_psa_mac_verify_finish ========
 */
psa_status_t ti_psa_mac_verify_finish(psa_mac_operation_t *operation, const uint8_t *mac, size_t mac_length)
{
    psa_status_t status;
    int rc;

    /* Store pointer to the handle so it can be used to free the operation context in the SPE */
    PSACrypto_secureCB.tfmCryptoOperationHandle = &operation->handle;

    /* Call corresponding TFM function */
    status = TFM_CRYPTO_API(psa_mac_verify_finish)(operation, mac, mac_length);

    if (status == PSA_SUCCESS)
    {
        status = PSACrypto_ns_waitForResult();
    }

    if (PSA_ALG_IS_HMAC(operation->alg) && (status == PSA_SUCCESS))
    {
        /* For HMAC, compare the actual mac with the provided mac */
        rc = memcmp(&PSACrypto_secureCB.buffer[0], mac, mac_length);

        if (rc != 0)
        {
            status = PSA_ERROR_INVALID_SIGNATURE;
        }
    }

    /* Per the PSA Crypto API spec: If the function returns an error, the
     * application is responsible for calling psa_mac_abort() to cleanup and
     * free the allocated operation context in the SPE.
     */

    return status;
}

/*
 *  ======== ti_psa_mac_abort ========
 */
psa_status_t ti_psa_mac_abort(psa_mac_operation_t *operation)
{
    NS_WRAP_CALL_WITH_RETURN(psa_mac_abort, operation);
}

/*
 *  ======== ti_psa_cipher_encrypt ========
 */
psa_status_t ti_psa_cipher_encrypt(psa_key_id_t key,
                                   psa_algorithm_t alg,
                                   const uint8_t *input,
                                   size_t input_length,
                                   uint8_t *output,
                                   size_t output_size,
                                   size_t *output_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_cipher_encrypt,
                                      key,
                                      alg,
                                      input,
                                      input_length,
                                      output,
                                      output_size,
                                      output_length);
}

/*
 *  ======== ti_psa_cipher_decrypt ========
 */
psa_status_t ti_psa_cipher_decrypt(psa_key_id_t key,
                                   psa_algorithm_t alg,
                                   const uint8_t *input,
                                   size_t input_length,
                                   uint8_t *output,
                                   size_t output_size,
                                   size_t *output_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_cipher_decrypt,
                                      key,
                                      alg,
                                      input,
                                      input_length,
                                      output,
                                      output_size,
                                      output_length);
}

/*
 *  ======== ti_psa_cipher_encrypt_setup ========
 */
psa_status_t ti_psa_cipher_encrypt_setup(psa_cipher_operation_t *operation, psa_key_id_t key, psa_algorithm_t alg)
{
    NS_WRAP_CALL_WITH_RETURN(psa_cipher_encrypt_setup, operation, key, alg);
}

/*
 *  ======== ti_psa_cipher_decrypt_setup ========
 */
psa_status_t ti_psa_cipher_decrypt_setup(psa_cipher_operation_t *operation, psa_key_id_t key, psa_algorithm_t alg)
{
    NS_WRAP_CALL_WITH_RETURN(psa_cipher_decrypt_setup, operation, key, alg);
}

/*
 *  ======== ti_psa_cipher_generate_iv ========
 */
psa_status_t ti_psa_cipher_generate_iv(psa_cipher_operation_t *operation,
                                       uint8_t *iv,
                                       size_t iv_size,
                                       size_t *iv_length)
{
    NS_WRAP_CALL_WITH_RETURN(psa_cipher_generate_iv, operation, iv, iv_size, iv_length);
}

/*
 *  ======== ti_psa_cipher_set_iv ========
 */
psa_status_t ti_psa_cipher_set_iv(psa_cipher_operation_t *operation, const uint8_t *iv, size_t iv_length)
{
    NS_WRAP_CALL_WITH_RETURN(psa_cipher_set_iv, operation, iv, iv_length);
}

/*
 *  ======== ti_psa_cipher_update ========
 */
psa_status_t ti_psa_cipher_update(psa_cipher_operation_t *operation,
                                  const uint8_t *input,
                                  size_t input_length,
                                  uint8_t *output,
                                  size_t output_size,
                                  size_t *output_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_cipher_update,
                                      operation,
                                      input,
                                      input_length,
                                      output,
                                      output_size,
                                      output_length);
}

/*
 *  ======== ti_psa_cipher_finish ========
 */
psa_status_t ti_psa_cipher_finish(psa_cipher_operation_t *operation,
                                  uint8_t *output,
                                  size_t output_size,
                                  size_t *output_length)
{
    /* Store pointer to the handle so it can be used to free the operation context in the SPE */
    PSACrypto_secureCB.tfmCryptoOperationHandle = &operation->handle;

    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_cipher_finish, operation, output, output_size, output_length);
}

/*
 *  ======== ti_psa_cipher_abort ========
 */
psa_status_t ti_psa_cipher_abort(psa_cipher_operation_t *operation)
{
    NS_WRAP_CALL_WITH_RETURN(psa_cipher_abort, operation);
}

/*
 *  ======== ti_psa_aead_encrypt ========
 */
psa_status_t ti_psa_aead_encrypt(psa_key_id_t key,
                                 psa_algorithm_t alg,
                                 const uint8_t *nonce,
                                 size_t nonce_length,
                                 const uint8_t *additional_data,
                                 size_t additional_data_length,
                                 const uint8_t *plaintext,
                                 size_t plaintext_length,
                                 uint8_t *ciphertext,
                                 size_t ciphertext_size,
                                 size_t *ciphertext_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_aead_encrypt,
                                      key,
                                      alg,
                                      nonce,
                                      nonce_length,
                                      additional_data,
                                      additional_data_length,
                                      plaintext,
                                      plaintext_length,
                                      ciphertext,
                                      ciphertext_size,
                                      ciphertext_length);
}

/*
 *  ======== ti_psa_aead_decrypt ========
 */
psa_status_t ti_psa_aead_decrypt(psa_key_id_t key,
                                 psa_algorithm_t alg,
                                 const uint8_t *nonce,
                                 size_t nonce_length,
                                 const uint8_t *additional_data,
                                 size_t additional_data_length,
                                 const uint8_t *ciphertext,
                                 size_t ciphertext_length,
                                 uint8_t *plaintext,
                                 size_t plaintext_size,
                                 size_t *plaintext_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_aead_decrypt,
                                      key,
                                      alg,
                                      nonce,
                                      nonce_length,
                                      additional_data,
                                      additional_data_length,
                                      ciphertext,
                                      ciphertext_length,
                                      plaintext,
                                      plaintext_size,
                                      plaintext_length);
}

/*
 *  ======== ti_psa_aead_encrypt_setup ========
 */
psa_status_t ti_psa_aead_encrypt_setup(psa_aead_operation_t *operation, psa_key_id_t key, psa_algorithm_t alg)
{
    NS_WRAP_CALL_WITH_RETURN(psa_aead_encrypt_setup, operation, key, alg);
}

/*
 *  ======== ti_psa_aead_decrypt_setup ========
 */
psa_status_t ti_psa_aead_decrypt_setup(psa_aead_operation_t *operation, psa_key_id_t key, psa_algorithm_t alg)
{
    NS_WRAP_CALL_WITH_RETURN(psa_aead_decrypt_setup, operation, key, alg);
}

/*
 *  ======== ti_psa_aead_generate_nonce ========
 */
psa_status_t ti_psa_aead_generate_nonce(psa_aead_operation_t *operation,
                                        uint8_t *nonce,
                                        size_t nonce_size,
                                        size_t *nonce_length)
{
    NS_WRAP_CALL_WITH_RETURN(psa_aead_generate_nonce, operation, nonce, nonce_size, nonce_length);
}

/*
 *  ======== ti_psa_aead_set_nonce ========
 */
psa_status_t ti_psa_aead_set_nonce(psa_aead_operation_t *operation, const uint8_t *nonce, size_t nonce_length)
{
    NS_WRAP_CALL_WITH_RETURN(psa_aead_set_nonce, operation, nonce, nonce_length);
}

/*
 *  ======== ti_psa_aead_set_lengths ========
 */
psa_status_t ti_psa_aead_set_lengths(psa_aead_operation_t *operation, size_t ad_length, size_t plaintext_length)
{
    NS_WRAP_CALL_WITH_RETURN(psa_aead_set_lengths, operation, ad_length, plaintext_length);
}

/*
 *  ======== ti_psa_aead_update_ad ========
 */
psa_status_t ti_psa_aead_update_ad(psa_aead_operation_t *operation, const uint8_t *input, size_t input_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_aead_update_ad, operation, input, input_length);
}

/*
 *  ======== ti_psa_aead_update ========
 */
psa_status_t ti_psa_aead_update(psa_aead_operation_t *operation,
                                const uint8_t *input,
                                size_t input_length,
                                uint8_t *output,
                                size_t output_size,
                                size_t *output_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_aead_update,
                                      operation,
                                      input,
                                      input_length,
                                      output,
                                      output_size,
                                      output_length);
}

/*
 *  ======== ti_psa_aead_finish ========
 */
psa_status_t ti_psa_aead_finish(psa_aead_operation_t *operation,
                                uint8_t *ciphertext,
                                size_t ciphertext_size,
                                size_t *ciphertext_length,
                                uint8_t *tag,
                                size_t tag_size,
                                size_t *tag_length)
{
    /* Store pointer to the handle so it can be used to free the operation context in the SPE */
    PSACrypto_secureCB.tfmCryptoOperationHandle = &operation->handle;

    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_aead_finish,
                                      operation,
                                      ciphertext,
                                      ciphertext_size,
                                      ciphertext_length,
                                      tag,
                                      tag_size,
                                      tag_length);
}

/*
 *  ======== ti_psa_aead_verify ========
 */
psa_status_t ti_psa_aead_verify(psa_aead_operation_t *operation,
                                uint8_t *plaintext,
                                size_t plaintext_size,
                                size_t *plaintext_length,
                                const uint8_t *tag,
                                size_t tag_length)
{
    /* Store pointer to the handle so it can be used to free the operation context in the SPE */
    PSACrypto_secureCB.tfmCryptoOperationHandle = &operation->handle;

    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_aead_verify,
                                      operation,
                                      plaintext,
                                      plaintext_size,
                                      plaintext_length,
                                      tag,
                                      tag_length);
}

/*
 *  ======== ti_psa_aead_abort ========
 */
psa_status_t ti_psa_aead_abort(psa_aead_operation_t *operation)
{
    NS_WRAP_CALL_WITH_RETURN(psa_aead_abort, operation);
}

/*
 *  ======== ti_psa_sign_message ========
 */
psa_status_t ti_psa_sign_message(psa_key_id_t key,
                                 psa_algorithm_t alg,
                                 const uint8_t *input,
                                 size_t input_length,
                                 uint8_t *signature,
                                 size_t signature_size,
                                 size_t *signature_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_sign_message,
                                      key,
                                      alg,
                                      input,
                                      input_length,
                                      signature,
                                      signature_size,
                                      signature_length);
}

/*
 *  ======== ti_psa_verify_message ========
 */
psa_status_t ti_psa_verify_message(psa_key_id_t key,
                                   psa_algorithm_t alg,
                                   const uint8_t *input,
                                   size_t input_length,
                                   const uint8_t *signature,
                                   size_t signature_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_verify_message, key, alg, input, input_length, signature, signature_length);
}

/*
 *  ======== ti_psa_sign_hash ========
 */
psa_status_t ti_psa_sign_hash(psa_key_id_t key,
                              psa_algorithm_t alg,
                              const uint8_t *hash,
                              size_t hash_length,
                              uint8_t *signature,
                              size_t signature_size,
                              size_t *signature_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_sign_hash,
                                      key,
                                      alg,
                                      hash,
                                      hash_length,
                                      signature,
                                      signature_size,
                                      signature_length);
}

/*
 *  ======== ti_psa_verify_hash ========
 */
psa_status_t ti_psa_verify_hash(psa_key_id_t key,
                                psa_algorithm_t alg,
                                const uint8_t *hash,
                                size_t hash_length,
                                const uint8_t *signature,
                                size_t signature_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_verify_hash, key, alg, hash, hash_length, signature, signature_length);
}

/*
 *  ======== ti_psa_asymmetric_encrypt ========
 */
psa_status_t ti_psa_asymmetric_encrypt(psa_key_id_t key,
                                       psa_algorithm_t alg,
                                       const uint8_t *input,
                                       size_t input_length,
                                       const uint8_t *salt,
                                       size_t salt_length,
                                       uint8_t *output,
                                       size_t output_size,
                                       size_t *output_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_asymmetric_encrypt,
                                      key,
                                      alg,
                                      input,
                                      input_length,
                                      salt,
                                      salt_length,
                                      output,
                                      output_size,
                                      output_length);
}

/*
 *  ======== ti_psa_asymmetric_decrypt ========
 */
psa_status_t ti_psa_asymmetric_decrypt(psa_key_id_t key,
                                       psa_algorithm_t alg,
                                       const uint8_t *input,
                                       size_t input_length,
                                       const uint8_t *salt,
                                       size_t salt_length,
                                       uint8_t *output,
                                       size_t output_size,
                                       size_t *output_length)
{
    NS_WRAP_CALL_WITH_RETURN_CALLBACK(psa_asymmetric_decrypt,
                                      key,
                                      alg,
                                      input,
                                      input_length,
                                      salt,
                                      salt_length,
                                      output,
                                      output_size,
                                      output_length);
}

/*
 *  ======== ti_psa_key_derivation_setup ========
 */
psa_status_t ti_psa_key_derivation_setup(psa_key_derivation_operation_t *operation, psa_algorithm_t alg)
{
    NS_WRAP_CALL_WITH_RETURN(psa_key_derivation_setup, operation, alg);
}

/*
 *  ======== ti_psa_key_derivation_get_capacity ========
 */
psa_status_t ti_psa_key_derivation_get_capacity(const psa_key_derivation_operation_t *operation, size_t *capacity)
{
    NS_WRAP_CALL_WITH_RETURN(psa_key_derivation_get_capacity, operation, capacity);
}

/*
 *  ======== ti_psa_key_derivation_set_capacity ========
 */
psa_status_t ti_psa_key_derivation_set_capacity(psa_key_derivation_operation_t *operation, size_t capacity)
{
    NS_WRAP_CALL_WITH_RETURN(psa_key_derivation_set_capacity, operation, capacity);
}

/*
 *  ======== ti_psa_key_derivation_input_bytes ========
 */
psa_status_t ti_psa_key_derivation_input_bytes(psa_key_derivation_operation_t *operation,
                                               psa_key_derivation_step_t step,
                                               const uint8_t *data,
                                               size_t data_length)
{
    NS_WRAP_CALL_WITH_RETURN(psa_key_derivation_input_bytes, operation, step, data, data_length);
}

/*
 *  ======== ti_psa_key_derivation_input_integer ========
 */
psa_status_t ti_psa_key_derivation_input_integer(psa_key_derivation_operation_t *operation,
                                                 psa_key_derivation_step_t step,
                                                 uint64_t value)
{
    NS_WRAP_CALL_WITH_RETURN(psa_key_derivation_input_integer, operation, step, value);
}

/*
 *  ======== ti_psa_key_derivation_input_key ========
 */
psa_status_t ti_psa_key_derivation_input_key(psa_key_derivation_operation_t *operation,
                                             psa_key_derivation_step_t step,
                                             psa_key_id_t key)
{
    NS_WRAP_CALL_WITH_RETURN(psa_key_derivation_input_key, operation, step, key);
}

/*
 *  ======== ti_psa_key_derivation_key_agreement ========
 */
psa_status_t ti_psa_key_derivation_key_agreement(psa_key_derivation_operation_t *operation,
                                                 psa_key_derivation_step_t step,
                                                 psa_key_id_t private_key,
                                                 const uint8_t *peer_key,
                                                 size_t peer_key_length)
{
    NS_WRAP_CALL_WITH_RETURN(psa_key_derivation_key_agreement, operation, step, private_key, peer_key, peer_key_length);
}

/*
 *  ======== ti_psa_key_derivation_output_bytes ========
 */
psa_status_t ti_psa_key_derivation_output_bytes(psa_key_derivation_operation_t *operation,
                                                uint8_t *output,
                                                size_t output_length)
{
    NS_WRAP_CALL_WITH_RETURN(psa_key_derivation_output_bytes, operation, output, output_length);
}

/*
 *  ======== ti_psa_key_derivation_output_key ========
 */
psa_status_t ti_psa_key_derivation_output_key(const psa_key_attributes_t *attributes,
                                              psa_key_derivation_operation_t *operation,
                                              psa_key_id_t *key)
{
    NS_WRAP_CALL_WITH_RETURN(psa_key_derivation_output_key, attributes, operation, key);
}

/*
 *  ======== ti_psa_key_derivation_verify_bytes ========
 */
psa_status_t ti_psa_key_derivation_verify_bytes(psa_key_derivation_operation_t *operation,
                                                const uint8_t *expected_output,
                                                size_t output_length)
{
    NS_WRAP_CALL_WITH_RETURN(psa_key_derivation_verify_bytes, operation, expected_output, output_length);
}

/*
 *  ======== ti_psa_key_derivation_verify_key ========
 */
psa_status_t ti_psa_key_derivation_verify_key(psa_key_derivation_operation_t *operation, psa_key_id_t expected)
{
    NS_WRAP_CALL_WITH_RETURN(psa_key_derivation_verify_key, operation, expected);
}

/*
 *  ======== ti_psa_key_derivation_abort ========
 */
psa_status_t ti_psa_key_derivation_abort(psa_key_derivation_operation_t *operation)
{
    NS_WRAP_CALL_WITH_RETURN(psa_key_derivation_abort, operation);
}

/*
 *  ======== ti_psa_raw_key_agreement ========
 */
psa_status_t ti_psa_raw_key_agreement(psa_algorithm_t alg,
                                      psa_key_id_t private_key,
                                      const uint8_t *peer_key,
                                      size_t peer_key_length,
                                      uint8_t *output,
                                      size_t output_size,
                                      size_t *output_length)
{
    psa_status_t status;
    size_t curveBytes;
    size_t keyBits;

    /* Call corresponding TFM function */
    status = TFM_CRYPTO_API(
        psa_raw_key_agreement)(alg, private_key, peer_key, peer_key_length, output, output_size, output_length);

    if (status == PSA_SUCCESS)
    {
        status = PSACrypto_ns_waitForResult();
    }

    if (status == PSA_SUCCESS)
    {
        keyBits    = PSACrypto_secureCB.keyBits;
        curveBytes = PSACrypto_secureCB.length;

        if (keyBits == 255)
        {
            /* For Curve25519, only the 'x' component is returned by the ECDH
             * driver thus it can be copied directly as the shared secret.
             */
            (void)memcpy(output, &PSACrypto_secureCB.buffer[0], curveBytes);
        }
        else
        {
            /* The ECDH driver returns the shared secret in uncompressed point
             * format (0x04 || x || y). Copy only the 'x' component in raw
             * encoding format as the shared secret.
             */
            (void)memcpy(output, &PSACrypto_secureCB.buffer[1], curveBytes);
        }

        *output_length = curveBytes;
    }

    return status;
}

/*
 *  ======== ti_psa_generate_random ========
 */
psa_status_t ti_psa_generate_random(uint8_t *output, size_t output_size)
{
    NS_WRAP_CALL_WITH_RETURN(psa_generate_random, output, output_size);
}

/*
 *  ======== ti_psa_generate_key ========
 */
psa_status_t ti_psa_generate_key(const psa_key_attributes_t *attributes, psa_key_id_t *key)
{
    NS_WRAP_CALL_WITH_RETURN(psa_generate_key, attributes, key);
}

/*
 *  ======== ti_psa_hash_resume ========
 */
psa_status_t ti_psa_hash_resume(psa_hash_operation_t *operation, const uint8_t *hash_state, size_t hash_state_length)
{
    NS_WRAP_CALL_WITH_RETURN(psa_hash_resume, operation, hash_state, hash_state_length);
}

/*
 *  ======== ti_psa_hash_suspend ========
 */
psa_status_t ti_psa_hash_suspend(psa_hash_operation_t *operation,
                                 uint8_t *hash_state,
                                 size_t hash_state_size,
                                 size_t *hash_state_length)
{
    NS_WRAP_CALL_WITH_RETURN(psa_hash_suspend, operation, hash_state, hash_state_size, hash_state_length);
}
