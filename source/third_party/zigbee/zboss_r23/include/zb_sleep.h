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
/* PURPOSE:
*/
#ifndef ZB_SLEEP_H
#define ZB_SLEEP_H 1

#if defined ZB_USE_SLEEP

/* Sleep time when it is no delayed cbs. */
#ifdef ZB_SCHED_SLEEP_NO_TM_MS
#define ZB_SCHED_SLEEP_NO_TM_BI ZB_MILLISECONDS_TO_BEACON_INTERVAL(ZB_SCHED_SLEEP_NO_TM_MS)
#else
#define ZB_SCHED_SLEEP_NO_TM_BI ZB_MILLISECONDS_TO_BEACON_INTERVAL(10000U)
#endif

#define ZB_SLEEP_INVALID_VALUE  (zb_uint32_t)(~0u)

typedef struct zb_sleep_ctx_s
{
  zb_bool_t     permit_sleep_signal;
  zb_time_t     last_timestamp;
  zb_time_t     timer;
  zb_uint32_t   threshold;
  zb_bitbool_t  enabled:1;
  zb_bitbool_t  sleeping_now:1;
  zb_bitbool_t  intr:1;
} zb_sleep_ctx_t;

void zb_sleep_init(void);
zb_uint32_t zb_sleep_calc_sleep_tmo(void);
void zb_sleep_can_sleep(zb_uint32_t sleep_tmo);

#endif  /* ZB_USE_SLEEP */

#endif /* ZB_SLEEP_H */
