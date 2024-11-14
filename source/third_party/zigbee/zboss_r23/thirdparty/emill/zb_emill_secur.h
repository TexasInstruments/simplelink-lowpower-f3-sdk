#ifndef ZB_EMILL_SECUR_H
#define ZB_EMILL_SECUR_H

#include "zb_common.h"
#include "zb_emill_support_check.h"

#if ZB_IS_EMILL_SUPPORTED
/**
 * @brief Multiply provided point by a given scalar and store result in result_point
 * 
 * @param result_point              result of multiplication of @point by the @scalar of ZB_DLK_PUB_KEY_LEN(ZB_ECC_CURVE_25519) bytes in length
 * @param scalar                    integer scalar of ZB_DLK_PRV_KEY_LEN(ZB_ECC_CURVE_25519) bytes in length
 * @param point                     base point for multiplication by a scalar of ZB_DLK_PUB_KEY_LEN(ZB_ECC_CURVE_25519) bytes in length
 */
void zb_emill_crypto_scalarmult(zb_uint8_t *result_point,
                                const zb_uint8_t *scalar,
                                const zb_uint8_t *point);

#ifdef ZB_ECDHE_P256_ENABLED
/**
 * P256 public key generation procedure
 * @param [in]  prv_key  private key
 * @param [out] pub_key  generated public key
 *
 * @return RET_OK if key was generated succesfully, RET_ERROR otherwise
 */
zb_ret_t zb_emill_ecdh_p256_public_key(const zb_uint8_t prv_key[32], zb_uint8_t pub_key[64]);

/**
 * P256 shared secret generation procedure
 * @param [out] s       an array of 32 bytes for the shared secret to be stored in
 * @param [in]  prv_key local private key
 * @param [in]  pub_key public key
 *
 * @return RET_OK if key was generated succesfully, RET_ERROR otherwise
 */
zb_ret_t zb_emill_ecdh_p256_shared_secret(zb_uint8_t s[32],
                                          const zb_uint8_t *prv_key,
                                          const zb_uint8_t *pub_key);
#endif /* ZB_ECDHE_P256_ENABLED */
#endif /* ZB_IS_EMILL_SUPPORTED */

#endif /* ZB_EMILL_SECUR_H */
