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
/* PURPOSE: NaCl Curve25519 general API
*/

#ifndef ZB_SECUR_CURVE25519_API_H
#define ZB_SECUR_CURVE25519_API_H 1

#ifndef ZB_HW_CRYPTO_SCALARMULT
/**
 * Multiply provided @point by a given @scalar and store result in @result_point
 * @result_point                    - result of multiplication of @point by the @scalar of @ZB_DLK_PUB_KEY_LEN(ZB_ECC_CURVE_25519) bytes in length
 * @scalar                          - integer scalar of @ZB_DLK_PRV_KEY_LEN(ZB_ECC_CURVE_25519) bytes in length
 * @point                           - base point for multiplication by a @scalar of @ZB_DLK_PUB_KEY_LEN(ZB_ECC_CURVE_25519) bytes in length
 * @return       - 0 in success.
 */
zb_int_t zb_sw_crypto_scalarmult(zb_uint8_t *result_point,
                                 const zb_uint8_t *scalar,
                                 const zb_uint8_t *point);
#endif /* ZB_HW_CRYPTO_SCALARMULT */

/**
 * Perform multiplication of base point x=9 by a provided @scalar
 * @result_point - result of multiplication of base point x = 9 by the @scalar of @ZB_DLK_PUB_KEY_LEN(ZB_ECC_CURVE_25519) bytes in length
 * @scalar       - integer scalar of @ZB_DLK_PRV_KEY_LEN(ZB_ECC_CURVE_25519) bytes in length
 * @return       - 0 in success.
 */
zb_int_t    crypto_scalarmult_base(zb_uint8_t *result_point, const zb_uint8_t *scalar);

void        crypto_set_curve25519_base(zb_uint8_t *base_point);

#endif  /* ZB_SECUR_CURVE25519_API_H */
