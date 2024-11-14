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
/* PURPOSE: trace into RAM area, nvram and network
*/

#ifndef ZB_NET_TRACE_H
#define ZB_NET_TRACE_H 1

#include "zb_ringbuffer.h"


/** @addtogroup  ZB_TRACE
 * @{
 */

/* Bitfield */
typedef enum zb_nettrace_mode_e
{
  ZB_NETTRACE_OFF,
  ZB_NETTRACE_MEM_ONLY,     /* Crash dump only */
  ZB_NETTRACE_NET,          /* Includes net trace and crash dump */
} zb_nettrace_mode_t;

typedef struct zb_nettrace_ctx_s
{
  zb_callback_t put2net_start_cb;
  zb_callback_t nettrace_startup_done;
  zb_nettrace_mode_t trace_mode;
  zb_bool_t net_put_is_running;
  zb_bool_t atboot_tracing_active;
  zb_bool_t          inited;
} zb_nettrace_ctx_t;

/**
 * @brief Init net trace.
 *
 * Check RAM (optionally NVRAM) for existing crash trace
 *
 * @param trace_mode - trace mode @ref zb_nettrace_mode_t
 */
void zb_nettrace_init(zb_nettrace_mode_t trace_mode);

/**
   Start net trace, optionally put trace to net.

   Must be called after network initialization is done.

   @param put2net_start_cb - callback to be used to start writing trace to net
   at runtime
   @param put2net_atboot_func - callback to be used to write trace to net just
   after boot, when applications, like gw, are not running yet
   @param nettrace_startup_done - callback to be called when startup process
   is completed and all atboot trace messages are processed
 */
zb_ret_t zb_nettrace_startup(
  zb_callback_t      put2net_start_cb,
  zb_callback_t      put2net_atboot_func,
  zb_callback_t      nettrace_startup_done);

/**
   Switch mode of trace to net

   @param arg - trace mode @ref zb_nettrace_mode_t
 */
void zb_nettrace_switch(zb_uint8_t arg);

/**
   Get data portion from the net trace buffer.

   To be used by mem-net trace transfer callback

   @return data length, 0 if buffer is empty
 */
zb_uint_t zb_nettrace_get_next_data(zb_uint8_t **buf_p, zb_size_t buf_len);

/**
   Flush the data portion from mem trace buffer.

   @param size - data size
 */
void zb_nettrace_flush(zb_size_t size);

/**
   Set trace mode

   @param trace_mode - trace mode @ref zb_nettrace_mode_t
 */
void zb_nettrace_set_mode(zb_nettrace_mode_t mode);


/**
   Stop trace running

 */
void zb_nettrace_stop(void);


/**
   Start trace running

 */
void zb_nettrace_start(void);


/**
  Get current net trace mode

  @return current trace mode
 */
zb_nettrace_mode_t zb_nettrace_get_mode(void);

/**
   Clean trace buffer

 */
void zb_nettrace_clean(void);


/**
 * @brief Checks whether net trace is blocked for some source file
 * @param file_id - ZB_TRACE_FILE_ID value of source file doing trace
 * @return 0 - trace is not blocked for this file, 1 - trace is blocked
 *
 * The reason is to not trace tracing process itself.
 */
int zb_nettrace_is_blocked_for(zb_uint16_t file_id);


/**
 * @brief Starts batch put to the trace ring buffer
 * @note This call should begin series of zb_nettrace_put_bytes() calls!
 */
void zb_nettrace_batch_start(zb_uint16_t batch_size);

/**
 * @brief Puts buffer to the internal trace buffer
 * @param buf - buffer
 * @param len - buffer length
 */
void zb_nettrace_put_bytes(const zb_uint8_t *buf, zb_short_t len);

/**
 * @brief Finishes using of current data batch
 * @note This call should be the last after series of zb_nettrace_put_bytes() calls!
 */
void zb_nettrace_batch_commit(void);

/*! @} */

#endif /* ZB_NET_TRACE_H */
