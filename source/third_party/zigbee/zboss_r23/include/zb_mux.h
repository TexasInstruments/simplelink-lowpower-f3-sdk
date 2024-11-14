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
/*  PURPOSE: MUX - module to get packet from multipan source, split them on
 *           packets for each pan
 */

#ifndef ZB_MUX_H
#define ZB_MUX_H

#include "zb_common.h"
#include "zb_ringbuffer.h"

#define ZB_MUX_MAX_BUF_SIZE 2048

/* Uncomment to debug HDLC buffer wrap with SPI */
//#define ZB_MUX_MAX_BUF_SIZE (512-67)

/* MAX NLIs items supported by the mux */
#define ZB_MUX_MAX_NLI_CNT 4
/* MAX NLIs supported by protocol */
#define SPINEL_MAX_NLI_CNT 4

#define OT_NLI_ID 0u
#define ZB_NLI_ID 2u
#define INTERNAL_NLI_ID 3u  ///< NLI for communications between host muxer and device muxer.

ZB_RING_BUFFER_DECLARE(zb_mux_buf, zb_uint8_t, ZB_MUX_MAX_BUF_SIZE);

typedef struct {
  zb_mux_buf_t *rx;
  zb_mux_buf_t *tx;
  void         *app_data;
} zb_mux_iface_t;

/* combined HDLC device */
typedef struct {
  zb_mux_iface_t iface;
  zb_mux_buf_t   rx;
  zb_mux_buf_t   tx;
} zb_mux_combined_device_t;

/* single HDLC device */
typedef struct {
  zb_mux_iface_t iface;
  zb_uint8_t     nli;
  zb_mux_buf_t   rx;
  zb_mux_buf_t   tx;
} zb_mux_nli_device_t;

typedef ZB_PACKED_PRE struct {
  zb_uint8_t status[SPINEL_MAX_NLI_CNT];
  zb_uint8_t counter;
} ZB_PACKED_STRUCT zb_mux_keepalive_cmd_t;

/**
   reads incoming bytes from the transport, fills ptr buffer
   @param iface - the mux interface of rx/tx
   @param ptr - pointer to the memory the read operation should put the bytes
   @param size - size of the available space of ptr memory region
   @return the number of bytes that was read, otherwise negative error code, like RET_NO_MEMORY
*/
typedef zb_int32_t (*zb_mux_read_cb_t)(const zb_mux_iface_t *iface, zb_uint8_t *ptr, zb_uint16_t size);
/**
   write bytes to the transport
   @param iface - the mux interface of rx/tx
   @param ptr - pointer to the memory the write operation should fetch the bytes
   @param size - size of the data
   @return the number of bytes that was written, otherwise negative error code
*/
typedef zb_int32_t (*zb_mux_write_cb_t)(const zb_mux_iface_t *iface, const zb_uint8_t *ptr, zb_uint16_t size);

void zb_mux_buf_init(zb_mux_buf_t *buf);
zb_ret_t zb_mux_buf_rx(const zb_mux_iface_t *iface, zb_mux_read_cb_t cb);
zb_int_t zb_mux_buf_tx(const zb_mux_iface_t *iface, zb_ushort_t offset, zb_mux_write_cb_t cb);
zb_ret_t zb_mux_buf_tx_flush(const zb_mux_iface_t *iface, zb_ushort_t size);

void zb_mux_combined_device_init(zb_mux_combined_device_t *device, void *app_data);
void zb_mux_combined_device_deinit(zb_mux_combined_device_t *device);
void zb_mux_combined_device_io_rbuf_reset(zb_mux_combined_device_t *device);
void zb_mux_nli_device_init(zb_mux_nli_device_t *device, void *app_data, zb_uint8_t nli);
void zb_mux_nli_device_io_rbuf_reset(zb_mux_nli_device_t *device);

zb_ret_t zb_mux_hdlc_stream_mux(zb_mux_nli_device_t *nli_device, zb_mux_combined_device_t *combined_device);
zb_ret_t zb_mux_hdlc_stream_demux(zb_mux_combined_device_t *combined_device);
zb_ret_t zb_mux_hdlc_intercept(zb_mux_nli_device_t *nli_device, zb_uint32_t frame_size);
zb_mux_nli_device_t * zb_mux_nli_device_get(zb_uint8_t nli);

zb_ret_t zb_mux_put_bytes(const zb_uint8_t *aBuf, zb_uint16_t aBufLength);

#endif  /* ZB_MUX_H */
