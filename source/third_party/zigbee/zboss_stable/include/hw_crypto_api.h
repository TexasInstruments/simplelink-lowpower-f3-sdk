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

#ifndef ZB_SOFT_SECURITY

extern zb_ret_t
hw_encrypt_n_auth(
  zb_uint8_t *key,
  zb_uint8_t *nonce,
  zb_uint8_t *string_a,
  zb_ushort_t string_a_len,
  zb_uint8_t *string_m,
  zb_ushort_t string_m_len,
  zb_bufid_t crypted_text);

extern zb_ret_t
hw_decrypt_n_auth(
  zb_uint8_t *key,
  zb_uint8_t *nonce,
  zb_bufid_t  buf,
  zb_ushort_t string_a_len,
  zb_ushort_t string_c_len);


#define zb_ccm_encrypt_n_auth hw_encrypt_n_auth

#define zb_ccm_decrypt_n_auth hw_decrypt_n_auth

#define zb_ccm_decrypt_n_auth_stdsecur zb_ccm_decrypt_n_auth

#endif  /* ZB_SOFT_SECURITY */

#ifdef ZB_HW_ZB_AES128
void hw_aes128(zb_uint8_t *key, zb_uint8_t *msg, zb_uint8_t *c);

#define zb_aes128 hw_aes128
#endif

#ifdef ZB_HW_ZB_AES128_DEC
void hw_aes128_dec(zb_uint8_t *key, zb_uint8_t *msg, zb_uint8_t *c);

#define zb_aes128_dec hw_aes128_dec
#endif

#endif  /* HW_CRYPTO_API_H */
