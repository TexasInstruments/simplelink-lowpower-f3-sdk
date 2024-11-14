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
/* PURPOSE: ECC security services and routines - internal functions
*/

#ifndef ZB_ECC_INTERNAL
#define ZB_ECC_INTERNAL 1

#include "zb_common.h"
#include "zb_ecc.h"

zb_ret_t zb_ecc_point_decompression(const zb_uint32_t *Pc, zb_ecc_point_aff_t *P);

void zb_ecc_point_negation(const zb_ecc_point_aff_t *P, zb_ecc_point_aff_t *negP);

void zb_ecc_project_to_affine(const zb_ecc_point_pro_t *Qp, zb_ecc_point_aff_t *Qa);

void zb_ecc_point_doubling(const zb_ecc_point_pro_t *P, zb_ecc_point_pro_t *Q);

void zb_ecc_affine_to_project(const zb_ecc_point_aff_t *Qa, zb_ecc_point_pro_t *Qp);

zb_uint8_t zb_ecc_pk_validation(const zb_ecc_point_aff_t *Q);

void zb_ecc_inverse_bytes(zb_uint8_t *ptr, zb_uint32_t len);

void zb_ecc_modsq(const zb_uint32_t *a, zb_uint32_t *b);

void zb_ecc_point_frobenius(zb_ecc_point_pro_t *P);

void zb_ecc_mixed_addition(zb_ecc_point_pro_t *P, const zb_ecc_point_aff_t *Q, zb_ecc_point_pro_t *R);

#endif  /* ZB_ECC_INTERNAL  */
