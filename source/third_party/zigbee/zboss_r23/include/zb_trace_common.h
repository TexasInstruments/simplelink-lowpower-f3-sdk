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
/* PURPOSE: internal definitions for ZigBee trace.
*/
#ifndef ZB_TRACE_COMMON_H
#define ZB_TRACE_COMMON_H 1

#include "zboss_api_core.h"
#include "zb_mac_transport.h"

#if defined(ZB_TRACE_LEVEL) || defined(ZB_TRAFFIC_DUMP_ON)


#define TRACE_HDR_SIG0 0xdeU
#define TRACE_HDR_SIG1 0xadU

#ifndef ZB_TRACE_TO_PORT
extern zb_osif_file_t *s_trace_file;
#endif

struct trace_hdr_s
{
  zb_uint8_t sig[2];
  zb_mac_transport_hdr_t h;
} ZB_PACKED_STRUCT;

void zb_trace_inc_counter(void);
void zb_trace_flush_bytes(void);
void zb_serial_trace_init(const char *portname);

zb_bool_t zb_trace_check(zb_uint_t level, zb_uint_t mask);

#if defined(ZB_BINARY_TRACE)
zb_uint16_t zb_trace_rec_size(zb_uint16_t *args_size);
void zb_trace_put_hdr(zb_uint16_t file_id, zb_uint16_t trace_rec_len);
void zb_trace_put_u16(zb_uint16_t file_id, zb_uint16_t val);
void zb_trace_put_vararg(zb_uint16_t file_id, zb_minimal_vararg_t val);
/* should be implemented in the caller module */
void zb_trace_put_bytes(zb_uint16_t file_id, const zb_uint8_t *buf, zb_short_t len);
#endif /* ZB_BINARY_TRACE */

void zb_print2x(zb_uint8_t byte, zb_char_t *buf, zb_uint16_t *pos);

#ifdef ZB_TRAFFIC_DUMP_ON
zb_uint16_t zb_dump_rec_size(zb_uint16_t dump_size);
void zb_dump_put_hdr(zb_uint_t dump_rec_len, zb_uint8_t mask);
void zb_trace_msg_port_do();
#endif /* ZB_TRAFFIC_DUMP_ON */

#endif /* ZB_TRACE_LEVEL || ZB_TRAFFIC_DUMP_ON */

#endif /* ZB_TRACE_COMMON_H */
