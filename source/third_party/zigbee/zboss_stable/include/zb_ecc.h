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
/* PURPOSE: ECC security services and routines - internals
*/

#ifndef ZB_SECUR_ECC_H
#define ZB_SECUR_ECC_H 1
#include "zb_types.h"
#include "zboss_api_core.h"

/*! @cond internals_doc */
/*! \addtogroup ZB_SECUR */
/*! @{ */

#define ECC_MODE_FAST

#ifdef ECC_MODE_FAST
#define ECC_FAST_MODMUL
#define ECC_FAST_MODSQ
/*#define ECC_ENABLE_FIXED_SCALARMUL*/
#define ECC_FAST_RANDOM_SCALARMUL
#endif

#define ECC_SYNC_MODE_ENABLED

/*#ifdef ECC_MODE_KEY_GEN
#define ECC_FIXED_SCALAR
#define ECC_FAST_KEY_GEN
#define ECC_FAST_MODMUL
#endif*/

#define ZB_ECC_MAX_PUB_KEY_LEN_IN_DWORDS 10U
#define ZB_ECC_MAX_PUB_KEY_LEN_IN_BYTES 40U  /*ZB_ECC_MAX_PUB_KEY_LEN_IN_DWORDS * 4*/
#define ZB_ECC_MAX_CERT_LEN 48

#define ZB_ECC_HASH_LEN 16UL
/*ZB_SECUR_B6_HASH_BLOCK_SIZE*/
#define ZB_ECC_SECT163_CERT_LEN 48U
#define ZB_ECC_SECT283_CERT_LEN 74U

#define ZB_ECC_MAC_KEY_LEN ZB_ECC_HASH_LEN  /*must be equal to ZB_CCM_KEY_SIZE*/

#define ZB_BI_EQUAL 0
#define ZB_BI_LESS -1
#define ZB_BI_GREATER 1

#define ZB_PK_INVALID 0U
#define ZB_PK_VALID 1U

#define sect163k1 0U
#define sect283k1 1U

/*NOTE:n in bytes*/
#define XIS0(X, n){ ZB_BZERO((X), (n)); }
/*NOTE:n in dwords*/
#define XIS1(X, n){ (X)[0] = 1U; ZB_BZERO((X) + 1U, 4U*((n) - 1U)); }

/*For the curve sect163k1*/
#ifdef sect163k1
#define IF1_163(X)	       (X)[0] == 1U && (X)[1] == 0U && (X)[2] == 0U && (X)[3] == 0U && (X)[4] == 0U && (X)[5] == 0U
#define IFN1_163(X)	       (X)[0] != 1U || (X)[1] != 0U || (X)[2] != 0U || (X)[3] != 0U || (X)[4] != 0U || (X)[5] != 0U
#define IF0_163(X)	       (X)[0] == 0U && (X)[1] == 0U && (X)[2] == 0U && (X)[3] == 0U && (X)[4] == 0U && (X)[5] == 0U
#define IFN0_163(X)	       (X)[0] != 0U || (X)[1] != 0U || (X)[2] != 0U || (X)[3] != 0U || (X)[4] != 0U || (X)[5] != 0U
#endif

/*For the curve sect283k1*/
#ifdef sect283k1
#define IF1_283(X)	       (X)[0] == 1U && (X)[1] == 0U && (X)[2] == 0U && (X)[3] == 0U && (X)[4] == 0U && (X)[5] == 0U && (X)[6] == 0U && (X)[7] == 0U && (X)[8] == 0U // && X[9] == 0
#define IFN1_283(X)	       (X)[0] != 1U || (X)[1] != 0U || (X)[2] != 0U || (X)[3] != 0U || (X)[4] != 0U || (X)[5] != 0U || (X)[6] != 0U || (X)[7] != 0U || (X)[8] != 0U // || X[9] != 0
#define IF0_283(X)	       (X)[0] == 0U && (X)[1] == 0U && (X)[2] == 0U && (X)[3] == 0U && (X)[4] == 0U && (X)[5] == 0U && (X)[6] == 0U && (X)[7] == 0U && (X)[8] == 0U // && X[9] == 0
#define IFN0_283(X)	       (X)[0] != 0U || (X)[1] != 0U || (X)[2] != 0U || (X)[3] != 0U || (X)[4] != 0U || (X)[5] != 0U || (X)[6] != 0U || (X)[7] != 0U || (X)[8] != 0U // || X[9] != 0
#endif

/**
 * Define type for storing affine point (x, y).
 */
typedef struct zb_ecc_point_aff_s
{
  zb_uint32_t x[ZB_ECC_MAX_PUB_KEY_LEN_IN_DWORDS];
  zb_uint32_t y[ZB_ECC_MAX_PUB_KEY_LEN_IN_DWORDS];
} zb_ecc_point_aff_t;

ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_ecc_point_aff_t);

/**
 * Define type for storing projective point (X, Y, Z)
 */
typedef struct zb_ecc_point_pro_s
{
  zb_uint32_t X[ZB_ECC_MAX_PUB_KEY_LEN_IN_DWORDS];
  zb_uint32_t Y[ZB_ECC_MAX_PUB_KEY_LEN_IN_DWORDS];
  zb_uint32_t Z[ZB_ECC_MAX_PUB_KEY_LEN_IN_DWORDS];
} zb_ecc_point_pro_t;

ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_ecc_point_pro_t);

extern const zb_uint8_t ZB_ECC_PRIVATE_KEY_LEN[2];
extern const zb_uint8_t ZB_ECC_PUBLIC_KEY_LEN[2];
extern const zb_uint8_t ZB_ECC_PUB_KEY_LEN_IN_DWORDS[2];

/**
 * Get error from last executed function
 * @return           last ecc error
 */
zb_ret_t zb_ecc_get_last_error(void);

/**
 * Function for elliptic curve crypto library initialization. You must call this function before
 * work with elliptic curve crypto library.
 * @param ecc_curve_id - curve_id for chosen crypto suite(sect163k1 or sect283k1)
 */
void zb_ecc_set_curve(const zb_uint8_t ecc_curve_id);

/**
 * Keyed hash function.
 *
 * @param mac_key    - security key in length.
 * @param input      - input data to compute MAC.
 * @param input_len  - length of input data
 * @param hash       - MAC computed over security key and given input data
 */
 void zb_ecc_key_hmac(const zb_uint8_t *mac_key, const zb_uint8_t *input, const zb_uint16_t input_len, zb_bufid_t hash);

#ifdef ECC_SYNC_MODE_ENABLED
/**
* Key pair generation procedure
* @param rnd     - rnd  a random number (21-byte for sect163k1 and 36-byte for sect283k1)
* @param prv_key - generated private key
* @param pub_key - generated public key
*/
void zb_ecc_key_generation(const zb_uint8_t *rnd, zb_uint8_t *prv_key, zb_uint8_t *pub_key);
#endif

/**
 * Key pair generation procedure. Async mode.
 * @param async_cb       - callback which will be executed after generation procedure.
 * @param async_cb_param - parameter which will be passed to @async_cb function. It is unchanged during execution
 * @param rnd            - rnd  a random number (21-byte for sect163k1 and 36-byte for sect283k1)
 * @param prv_key        - generated private key
 * @param pub_key        - generated public key
 */
void zb_ecc_key_generation_async(zb_callback_t async_cb, const zb_uint8_t async_cb_param, const zb_uint8_t *rnd, zb_uint8_t *prv_key, zb_uint8_t *pub_key);

#ifdef ECC_SYNC_MODE_ENABLED
/**
* Elliptic Curve Qu-Vanstone implicit certificate scheme. Public key extraction procedure.
*
* @param QCAx           - the CA's public key in compressed form
* @param pub_rec        - reconstruction public key in original form
* @param CertA          - an implicit certificate for user idA
* @param QAx            - a user A's public key in compressed form
* @return               - return RET_OK in success and return RET_ERROR in fail.
*/
zb_ret_t zb_ecc_ecqv_extraction(zb_uint8_t *QCAx, zb_uint8_t *pub_rec, zb_uint8_t *CertA, zb_uint8_t *QAx);
#endif

/**
 * Elliptic Curve Qu-Vanstone implicit certificate scheme. Public key extraction procedure. Async mode
 *
 * @param async_cb       - callback which will be executed after extraction procedure.
 * @param async_cb_param - parameter which will be passed to @async_cb function. It is unchanged during execution.
 * @param QCAx           - the CA's public key in compressed form
 * @param pub_rec        - reconstruction public key in original form
 * @param CertA          - an implicit certificate for user idA
 * @param QAx            - a user A's public key in compressed form
 * @return               - return RET_OK in success and return RET_ERROR in fail.
 */
void zb_ecc_ecqv_extraction_async(zb_callback_t async_cb, const zb_uint8_t async_cb_param, zb_uint8_t *QCAx, zb_uint8_t *pub_rec, zb_uint8_t *CertA, zb_uint8_t *QAx);

#ifdef ECC_SYNC_MODE_ENABLED
/**
* Elliptic curve MQV scheme for shared secret generation
*
* @param prvA1    - A's private key
* @param prvA2    - A's ephemeral private key
* @param pubA2    - A's ephemeral public key
* @param pubB1    - B's public key
* @param pubB2    - B's ephemeral public key
* @param skey     - a shared key between A and B
* @return         - return RET_OK in success and return RET_ERROR in fail.
*/
zb_ret_t zb_ecc_ecmqv(zb_uint8_t *prvA1, zb_uint8_t *prvA2,
                      zb_uint8_t *pubA2, zb_uint8_t *pubB1, zb_uint8_t *pubB2, zb_uint8_t *skey);
#endif

/**
 * Elliptic curve MQV scheme for shared secret generation. Async mode
 *
 * @param async_cb       - callback which will be executed after extraction procedure.
 * @param async_cb_param - parameter which will be passed to @async_cb function. It is unchanged during execution.
 * @param prvA1          - A's private key
 * @param prvA2          - A's ephemeral private key
 * @param pubA2          - A's ephemeral public key
 * @param pubB1          - B's public key
 * @param pubB2          - B's ephemeral public key
 * @param skey           - a shared key between A and B
 * @return               - return RET_OK in success and return RET_ERROR in fail.
 */
void zb_ecc_ecmqv_async(zb_callback_t async_cb, const zb_uint8_t async_cb_param,
                             zb_uint8_t *prvA1, zb_uint8_t *prvA2, zb_uint8_t *pubA2,
                             zb_uint8_t *pubB1, zb_uint8_t *pubB2, zb_uint8_t *skey);

/*! @} */
/*! @endcond */

#endif
