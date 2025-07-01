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
/*  PURPOSE: Rejoin backoff
*/

#ifndef ZB_ZDO_REJOIN_BACKOFF_H
#define ZB_ZDO_REJOIN_BACKOFF_H 1

#ifdef ZB_REJOIN_BACKOFF

#define ZB_ZDO_REJOIN_BACKOFF_MAX_TIMEOUT  (30U*60U * ZB_TIME_ONE_SECOND) /* 30 minutes */
#define ZB_ZDO_REJOIN_BACKOFF_MAX_COUNT 11U
#define ZB_ZDO_REJOIN_BACKOFF_TIMEOUT(_cnt) (((_cnt) == 11U) ? ZB_ZDO_REJOIN_BACKOFF_MAX_TIMEOUT : (1U << (_cnt)) * ZB_TIME_ONE_SECOND) /* timeout value: 2, 4, 8, ... */

enum zb_zdo_rejoin_backoff_state_e
{
  ZB_ZDO_REJOIN_BACKOFF_IDLE = 0,
  ZB_ZDO_REJOIN_BACKOFF_TIMER_RUNNING,
  ZB_ZDO_REJOIN_BACKOFF_COMMAND_SCHEDULED,
  ZB_ZDO_REJOIN_BACKOFF_COMMAND_SENT,
  ZB_ZDO_REJOIN_BACKOFF_COMMAND_DONE
};

/** Rejoin backoff context */
typedef struct zb_zdo_rejoin_backoff_s
{
  enum zb_zdo_rejoin_backoff_state_e rjb_state;
  zb_uint8_t                         rjb_cnt;
  /* Determines whether device goes secure rejoin. */
  zb_bool_t                          rjb_do_secure;
  /* If set to ZB_FALSE device will search on current channel only */
  zb_bool_t                          rjb_scan_all_chan_mask;
  zb_bool_t                          rjb_insecure;
  zb_channel_page_t                  rjb_chan_mask_list[ZB_CHANNEL_PAGES_NUM];
  zb_uint8_t                         rjb_scan_duration;
}
zb_zdo_rejoin_backoff_t;

/** Set ZB_ZDO_REJOIN_BACKOFF_COMMAND_DONE state. For internal use. */
void zb_zdo_rejoin_backoff_iteration_done(void);

#endif /* ZB_REJOIN_BACKOFF */

#endif /* ZB_ZDO_REJOIN_BACKOFF_H */
