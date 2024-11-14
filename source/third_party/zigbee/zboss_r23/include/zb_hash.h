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
/* PURPOSE: Macros for hash calculation
*/

#ifndef ZB_HASH_H
#define ZB_HASH_H 1

/*! @cond internals_doc */
/*! \addtogroup ZB_BASE */
/*! @{ */

/* DJB2 algorithm magic number */
#define ZB_HASH_MAGIC_VAL 5381UL

/**
 * DJB2 hash function step
 *
 * See: [Hash Functions](www.cse.yorku.ca/~oz/hash.html)
 */
#define ZB_HASH_FUNC_STEP(hash_var, v) ((((hash_var) << 5) + (hash_var)) + (v))
#define ZB_4INT_HASH_FUNC(v1, v2, v3, v4) (ZB_HASH_FUNC_STEP(ZB_HASH_FUNC_STEP( ZB_HASH_FUNC_STEP( ZB_HASH_FUNC_STEP(ZB_HASH_MAGIC_VAL, (v1)), (v2)), (v3)), (v4)) & ZB_INT_MASK)
#define ZB_2INT_HASH_FUNC(v1, v2)      (ZB_HASH_FUNC_STEP(ZB_HASH_FUNC_STEP(ZB_HASH_MAGIC_VAL, (v1)), (v2)) & ZB_INT_MASK)

#define ZB_1INT_HASH_FUNC(v1)          (ZB_HASH_FUNC_STEP(ZB_HASH_MAGIC_VAL, (v1)) & ZB_INT_MASK)

/* Calculate a hash using DJB2 hash function */
zb_uint32_t zb_64bit_hash(zb_uint8_t const *data);

/**
 * Calculate Fowler-Noll-Vo hash.
 *
 * See: [FNV Hash](www.isthe.com/chongo/tech/comp/fnv/)
 */
zb_uint32_t zb_fnv_32a_uint16(zb_uint16_t v);

/*! @} */
/*! @endcond */

#endif /* ZB_HASH_H */
