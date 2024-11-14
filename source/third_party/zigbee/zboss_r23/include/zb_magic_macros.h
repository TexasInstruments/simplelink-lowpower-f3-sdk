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
/* PURPOSE: Some macros for fast calculation

*/
#ifndef MAGIC_MACROSES_H
#define MAGIC_MACROSES_H 1

/*! @cond internals_doc */
/**
   @addtogroup ZB_BASE
   @{
*/

/**
 * These algorithms are published by Henry Gordon Dietz (University of Kentucky)
 * in the article "The Aggregate Magic Algorithms".
 *
 * See: [The Aggregate Magic Algorithms](aggregate.org/MAGIC/)
 */

#define MAGIC_ROUND_TO_4(a) (((a) + 3U) / 4U * 4U)

#define MAGIC_TRUNC_TO_8(a) ((a) & (~7U))

#define MAGIC_LEAST_SIGNIFICANT_BIT_MASK(x) ((x) & (zb_uint_t)(-((zb_int_t)(x))))

/**
   Return 1 if the number is a power of 2, works only if x > 0
 */
#define MAGIC_IS_POWER_OF_TWO(x) ( ((x) & ((x) - 1U)) == 0U )


/**
 * Following set of macros implements compile-time calculation of # of bits in the word.
 *
 * See: [The Aggregate Magic Algorithms: Population Count](aggregate.org/MAGIC/#Population%20Count%20(Ones%20Count))
 */

#define MAGIC_ONE_1(x) ((x) - (((x) >> 1U) & 0x55555555U))
#define MAGIC_ONE_2(x) (((MAGIC_ONE_1(x) >> 2U) & 0x33333333U) + (MAGIC_ONE_1(x) & 0x33333333U))
#define MAGIC_ONE_4(x) (((MAGIC_ONE_2(x) >> 4U) + MAGIC_ONE_2(x)) & 0x0f0f0f0fU)
#define MAGIC_ONE_8(x) (MAGIC_ONE_4(x) + (MAGIC_ONE_4(x) >> 8U))
#define MAGIC_ONE_16(x) (MAGIC_ONE_8(x) + (MAGIC_ONE_8(x) >> 16U))
#define MAGIC_ONE_32(x) (MAGIC_ONE_16(x) & 0x0000003fU)


#define MAGIC_LOG2_1(x) ((x) | ((x) >> 1U))
#define MAGIC_LOG2_2(x) (MAGIC_LOG2_1(x) | (MAGIC_LOG2_1(x) >> 2U))
#define MAGIC_LOG2_4(x) (MAGIC_LOG2_2(x) | (MAGIC_LOG2_2(x) >> 4U))
#define MAGIC_LOG2_8(x) (MAGIC_LOG2_4(x) | (MAGIC_LOG2_4(x) >> 8U))
#define MAGIC_LOG2_16(x) (MAGIC_LOG2_8(x) | (MAGIC_LOG2_8(x) >> 16U))

#define MAGIC_LOG2_32(x) (MAGIC_ONE_32(MAGIC_LOG2_16(x)) - 1U)

/*! @} */
/*! @endcond */

#endif /* MAGIC_MACROSES_H */
