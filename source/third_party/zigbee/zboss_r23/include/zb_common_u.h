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
/* PURPOSE: Common utilities include file for ZigBee
*/
#ifndef ZB_COMMON_U_H
#define ZB_COMMON_U_H 1

#include "zboss_api_core.h"

/**
   Generate random value between 0 to 0xffffffff (32 bit) for jitters
 */
zb_uint32_t zb_random_jitter(void);

/**
   Used only for jitters. Need to add a premonition to randoms
 */
#define ZB_RANDOM_JTR(max_value) (zb_random_jitter() / (ZB_RAND_MAX / (zb_uint32_t)(max_value)))

/*! @} */

/*! \internal \addtogroup ZB_BASE */
/*! @{ */

/**
   Generate CRC8 hash
 */
zb_uint8_t zb_crc8(const zb_uint8_t *p, zb_uint8_t crc, zb_uint_t len);

/**
   Generate CRC16 hash for string
 */
zb_uint16_t zb_crc16(const zb_uint8_t *p, zb_uint16_t crc, zb_uint_t len);


/**
   Generate CRC32 hash
 */
zb_uint32_t zb_crc32(const zb_uint8_t *p, zb_uint_t len);

zb_uint32_t zb_crc32_next(zb_uint32_t prev_crc, zb_uint8_t *p, zb_uint_t len);

zb_uint32_t zb_crc32_next_v2(zb_uint32_t prev_crc, zb_uint8_t *p, zb_uint16_t len);

/**
 * Calculates number of '1' in 16bit bitmask
 */
zb_uint8_t zb_bit_cnt16(zb_uint16_t a);

/**
 * Calculates number of '1' in 32bit bitmask
 */
zb_uint8_t zb_bit_cnt32(zb_uint32_t a);

void zb_inverse_bytes(zb_uint8_t *ptr, zb_uint32_t len);

zb_uint_t zb_high_bit_number(zb_uint32_t mask);

zb_uint_t zb_low_bit_number(zb_uint32_t mask);

/*! @} */

#ifdef ZB_TOOL
void zb_init_tool(void);
#endif

#endif /* ZB_COMMON_U_H */
