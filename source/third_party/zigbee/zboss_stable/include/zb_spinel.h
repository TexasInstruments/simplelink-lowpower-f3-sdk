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
/*  PURPOSE: SPINEL layer
*/

#ifndef ZB_SPINEL_H
#define ZB_SPINEL_H

typedef ZB_PACKED_PRE struct zb_spinel_hdr_s
{
  zb_uint8_t hdr;
  zb_uint8_t cmd;
} ZB_PACKED_STRUCT zb_spinel_hdr_t;

typedef ZB_PACKED_PRE struct zb_spinel_simple_property_s
{
  zb_uint8_t hdr;
  zb_uint8_t cmd;
  zb_uint8_t prop_key;
  zb_uint8_t prop_value;
} ZB_PACKED_STRUCT zb_spinel_simple_property_t;

/**
 *   0   1   2   3   4   5   6   7
 * +---+---+---+---+---+---+---+---+
 * |  FLG  |  NLI  |      TID      |
 * +---+---+---+---+---+---+---+---+
 */
#define ZB_SPINEL_NLI_GET(spinel_hdr) (((spinel_hdr) >> 4) & 0x03)
#define ZB_SPINEL_TID_GET(spinel_hdr) (((spinel_hdr) >> 0) & 0x0f)

#define ZB_SPINEL_ENCAP_VERSION       0u
#define ZB_SPINEL_FLG                 0x80
#define ZB_SPINEL_PROP_INTERFACE_TYPE 3u
#define ZB_SPINEL_PROP_STREAM_NET     (0x70+2)
#define ZB_SPINEL_CMD_RESET           1u
#define ZB_SPINEL_CMD_PROP_VALUE_GET  2u
#define ZB_SPINEL_CMD_PROP_VALUE_SET  3u
#define ZB_SPINEL_CMD_PROP_VALUE_IS   6u

#define ZB_SPINEL_PROP_MCU_POWER_STATE 13u

#define ZB_SPINEL_MCU_POWER_STATE_ON        0u
#define ZB_SPINEL_MCU_POWER_STATE_LOW_POWER 1u
#define ZB_SPINEL_MCU_POWER_STATE_OFF       2u

/* 15,360 ... 16,383 Private Use */
#define INTERNAL_CMD_KEEPALIVE 15360
#define INTERNAL_CMD_KEEPALIVE_ACK 15361

#endif  /* ZB_SPINEL_H */
