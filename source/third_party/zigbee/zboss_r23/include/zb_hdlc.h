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

#ifndef ZB_HDLC_H
#define ZB_HDLC_H

#include "zb_types.h"
#include "zb_mux.h"

#define ZB_HDLC_FLAG   0x7Eu
#define ZB_HDLC_ESC    0x7Du
#define ZB_HDLC_XON    0x11u
#define ZB_HDLC_XOFF   0x13u
#define ZB_HDLC_VENDOR 0xF8u
#define ZB_HDLC_MASK   0x20

typedef struct
{
  zb_uint8_t *in_buf;
  zb_uint_t in_buf_size;
  zb_uint_t len;
  zb_bool_t sync;
  zb_bool_t packet_ready;
  zb_bool_t got_esc;
} zb_hdlc_ctx_t;

void spnl_hdlc_ctx_init(zb_hdlc_ctx_t *ctx, zb_uint8_t *in_buf, zb_uint_t in_buf_size);

zb_uint16_t spnl_hdlc_crc_initial(void);
zb_uint16_t spnl_hdlc_crc(zb_uint16_t crc, void *data, zb_uint_t len);
zb_uint16_t spnl_hdlc_crc_finish(zb_uint16_t crc);

zb_uint_t spnl_hdlc_tx_start_framing(zb_uint8_t *buf);
zb_uint_t spnl_hdlc_tx_put(zb_uint8_t *out_buf, zb_uint_t space, void *in_buf, zb_uint_t data_len);
zb_uint_t spnl_hdlc_tx_done_framing(zb_uint8_t *out_buf, zb_uint8_t space);

void      spnl_hdlc_rx_feed(zb_hdlc_ctx_t *ctx, zb_uint8_t byte);
void      spnl_hdlc_rx_flush(zb_hdlc_ctx_t *ctx);
zb_bool_t spnl_hdlc_rx_is_ready(zb_hdlc_ctx_t *ctx);
zb_uint_t spnl_hdlc_rx_get(zb_hdlc_ctx_t *ctx, zb_uint8_t **ppkt);

zb_uint_t spnl_to_hdlc_len_get(void *in_buf, zb_uint_t data_len, zb_uint16_t hdlc_crc);
zb_ret_t spnl_to_hdlc_rb(zb_mux_buf_t *rb, void *in_buf, zb_uint_t data_len);

#endif  /* ZB_HDLC_H */
