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
/*  PURPOSE: MAC-Split - Spinel/HDLC layer of MAC-Split transport over Spinel/HDLC-lite
*/

#ifndef ZB_MACSPLIT_SPINEL_H
#define ZB_MACSPLIT_SPINEL_H

#include "zb_hdlc.h"
#include "zb_spinel.h"


typedef ZB_PACKED_PRE struct zb_macsplit_spnl_encap_s
{
  zb_uint8_t hdr;
  zb_uint8_t cmd;
  zb_uint8_t prop;
  zb_uint16_t frame_len;
} ZB_PACKED_STRUCT
zb_macsplit_spnl_encap_t;

#endif  /* ZB_MACSPLIT_SPINEL_H */
