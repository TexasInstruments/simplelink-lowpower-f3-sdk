/* ZBOSS Zigbee software protocol stack
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 * This is unpublished proprietary source code of DSR Corporation
 * The copyright notice does not evidence any actual or intended
 * publication of such source code.
 *
 * ZBOSS is a registered trademark of Data Storage Research LLC d/b/a DSR
 * Corporation
 *
 * Commercial Usage
 * Licensees holding valid DSR Commercial licenses may use
 * this file in accordance with the DSR Commercial License
 * Agreement provided with the Software or, alternatively, in accordance
 * with the terms contained in a written agreement between you and
 * DSR.
 */
/* PURPOSE: HW crypto API
*/

#ifndef HW_CRYPTO_API_H
#define HW_CRYPTO_API_H 1

#include "zb_common.h"

/**
 * If ZB_HW_XXXX macro is defined, then hardware implementation is ALWAYS used instead of software
 *
 * @note  When ZB_USE_CORTEX_SW_SECURITY is defined, then software implementation optimized for cortex is used if it is possible.
 *        ZB_USE_CORTEX_SW_SECURITY is more weak, than ZB_HW_XXXX
 */

#ifdef ZB_HW_ZB_AES128
void hw_aes128(const zb_uint8_t *key, const zb_uint8_t *msg, zb_uint8_t *c);
#define ZB_AES128 hw_aes128
#else
#define ZB_AES128 zb_sw_aes128
#endif

#ifdef ZB_HW_ZB_AES128_DEC
void hw_aes128_dec(const zb_uint8_t *key, const zb_uint8_t *msg, zb_uint8_t *c);

#define ZB_AES128_DEC hw_aes128_dec
#else
#define ZB_AES128_DEC zb_sw_aes128_dec
#endif

#ifdef ZB_HW_ECDH_P256_PUBLIC_KEY
zb_ret_t zb_hw_ecdh_p256_public_key(const zb_uint8_t prv_key[32], zb_uint8_t pub_key[64]);

#define ZB_ECDH_P256_PUBLIC_KEY zb_hw_ecdh_p256_public_key
#else
#define ZB_ECDH_P256_PUBLIC_KEY zb_sw_ecdh_p256_public_key
#endif

#ifdef ZB_HW_ECDH_P256_SHARED_SECRET
zb_ret_t zb_hw_ecdh_p256_shared_secret(zb_uint8_t s[32],
                                       const zb_uint8_t *prv_key,
                                       const zb_uint8_t *pub_key);

#define ZB_ECDH_P256_SHARED_SECRET zb_hw_ecdh_p256_shared_secret
#else
#define ZB_ECDH_P256_SHARED_SECRET zb_sw_ecdh_p256_shared_secret
#endif

#if defined(ZB_HW_HMAC_SHA_256) //&& defined(ZB_DIRECT_ENABLED)
zb_ret_t zb_hw_hmac_sha_256(const zb_uint8_t *key, const zb_size_t key_len,
                         const zb_uint8_t *input, const zb_size_t input_len, zb_uint8_t *output);

#define ZB_HMAC_SHA_256 zb_hw_hmac_sha_256
#else
#define ZB_HMAC_SHA_256 zb_sw_hmac_sha_256
#endif /* ZB_HW_HMAC_SHA_256 && ZB_DIRECT_ENABLED */

#if defined(ZB_HW_SHA_256)
void zb_hw_sha_256(const zb_uint8_t *input, const zb_size_t input_len, zb_uint8_t output[32]);

#define ZB_SHA_256 zb_hw_sha_256
#else
#define ZB_SHA_256 zb_sw_sha_256
#endif /* ZB_HW_SHA_256 */


#ifdef ZB_HW_CCM_ENCRYPT_N_AUTH_RAW
zb_ret_t zb_hw_ccm_encrypt_n_auth_raw(zb_uint8_t *key,
                                      const zb_uint8_t *nonce,
                                      const zb_uint8_t *string_a,
                                      zb_uint32_t string_a_len,
                                      const zb_uint8_t *string_m,
                                      zb_uint32_t string_m_len,
                                      zb_uint8_t *dest_buf,
                                      zb_uint16_t dest_len);

#define ZB_CCM_ENCRYPT_N_AUTH_RAW zb_hw_ccm_encrypt_n_auth_raw
#else
#define ZB_CCM_ENCRYPT_N_AUTH_RAW zb_sw_ccm_encrypt_n_auth_raw
#endif

#ifdef ZB_HW_CCM_DECRYPT_N_AUTH_RAW
zb_ret_t zb_hw_ccm_decrypt_n_auth_raw(zb_uint8_t *key,
                                      const zb_uint8_t *nonce,
                                      const zb_uint8_t *string_a,
                                      zb_uint32_t string_a_len,
                                      const zb_uint8_t *string_c_u,
                                      zb_uint16_t string_c_u_len,
                                      zb_uint8_t *string_dest,
                                      zb_uint16_t *string_dest_len);

#define ZB_CCM_DECRYPT_N_AUTH_RAW zb_hw_ccm_decrypt_n_auth_raw
#else
#define ZB_CCM_DECRYPT_N_AUTH_RAW zb_sw_ccm_decrypt_n_auth_raw
#endif

#ifdef ZB_HW_CRYPTO_SCALARMULT
zb_int_t zb_hw_crypto_scalarmult(zb_uint8_t *result_point,
                                 const zb_uint8_t *scalar,
                                 const zb_uint8_t *point);

#define ZB_CRYPTO_SCALARMULT zb_hw_crypto_scalarmult
#else
#define ZB_CRYPTO_SCALARMULT zb_sw_crypto_scalarmult
#endif

#endif  /* HW_CRYPTO_API_H */
