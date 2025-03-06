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
/*  PURPOSE: MAC-Split transport for Windows
*/

#ifndef MACSPLIT_TRANSPORT_WINDOWS_H
#define MACSPLIT_TRANSPORT_WINDOWS_H 1

#ifdef ZB_WINDOWS

#include "zb_types.h"
#include <windows.h>

#define TRANSPORT_BUFFER_SIZE 1024
ZB_RING_BUFFER_DECLARE(zb_transport_buffer, zb_uint8_t, TRANSPORT_BUFFER_SIZE);

#define ZB_MAC_SPLIT_FROM_MCU_DUMP_FILE "from_mcu.dump"
#define ZB_MAC_SPLIT_TO_MCU_DUMP_FILE "to_mcu.dump"

void zb_mac_transport_init(void);

#define ZB_GO_IDLE()                      zb_windows_io_iteration(ZB_TRUE)

#define ZB_TRANSPORT_NONBLOCK_ITERATION() zb_windows_io_iteration(ZB_FALSE)

#define ZB_TIMER_INIT()

typedef struct zb_macsplit_io_ctx_s
{

#if defined ZB_MAC_TRANSPORT_DUMP
  FILE* host_dump;  /*!< dump file for data sent to MCU */
  FILE* mcu_dump; /*!< dump file for data got from MCU */
#endif

  HANDLE uart_handle;

  zb_bool_t is_running;
  zb_transport_buffer_t  in_buffer;
  zb_transport_buffer_t  out_buffer;

  zb_uint32_t prev_time_ms;
  zb_uint8_t write_flag;
  zb_uint32_t next_ping_time;

  DWORD io_sleep_ms;

} zb_macsplit_io_ctx_t;

typedef struct zb_io_uart_settings_s
{
  zb_uint8_t port_num;
  DWORD baud_rate;
} zb_io_uart_settings_t;

#define ZB_MAC_GET_TRANS_INT_FLAG() 0
#define ZB_MAC_TRANSCEIVER_GO_IDLE()

#endif /* defined ZB_WINDOWS */

#endif /* defined MACSPLIT_TRANSPORT_WINDOWS_H */
