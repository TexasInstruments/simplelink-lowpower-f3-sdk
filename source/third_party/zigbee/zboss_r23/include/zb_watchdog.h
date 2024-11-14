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
/* PURPOSE: General purpose software watchdog for ZB apps
*/

#ifndef ZB_WATCHDOG_H
#define ZB_WATCHDOG_H 1

#define ZB_WATCHDOG_DISABLED 0U
#define ZB_WATCHDOG_ENABLED  1U

#include "zb_common.h"
typedef struct zb_watchdow_s
{
  zb_time_t timeout;
  zb_time_t last_kick;
  zb_uint8_t state;             /* 0 - disabled, 1 - enabled */
} zb_watchdog_t;

typedef enum zb_watchdog_types_e
{
  ZB_WD_ZB_TRAFFIC,

  ZB_WD_CHECKIN,

  ZB_N_STD_WATCHDOGS
} zb_watchdog_types_t;

#define ZB_N_WATCHDOG 8U

/* error codes for error indication */
/* a watchdog type will be put in additional_info */
#define ZB_ERROR_WATCHDOG_TRIGGERED 1U

#ifdef USE_ZB_WATCHDOG

void zb_add_watchdog(zb_uint8_t wd_number, zb_time_t timeout);
void zb_kick_watchdog(zb_uint8_t wd_number);
void zb_watchdog_scheduler(zb_uint8_t param);
void zb_stop_watchdog(zb_uint8_t wd_number);
void zb_enable_watchdog(zb_uint8_t wd_number);
void zb_disable_watchdog(zb_uint8_t wd_number);

#define ZB_KICK_WATCHDOG(n) zb_kick_watchdog((zb_uint8_t)(n))
#define ZB_ENABLE_WATCHDOG(n) zb_enable_watchdog((zb_uint8_t)(n))
#define ZB_DISABLE_WATCHDOG(n) zb_disable_watchdog((zb_uint8_t)(n))

#else

#define ZB_KICK_WATCHDOG(n)
#define ZB_ENABLE_WATCHDOG(n)
#define ZB_DISABLE_WATCHDOG(n)

#endif

#ifdef ZB_USE_CHECKIN_WATCHDOG

#ifndef ZB_WD_CHECKIN_MIN_TO
#define ZB_WD_CHECKIN_MIN_TO (60U*60U*ZB_TIME_ONE_SECOND)
#endif

#endif /* ZB_USE_CHECKIN_WATCHDOG */


#ifdef ZB_USE_ZB_TRAFFIC_WATCHDOG

#ifndef ZB_WD_ZB_TRAFFIC_TO
#define ZB_WD_ZB_TRAFFIC_TO (240U*60U*ZB_TIME_ONE_SECOND)
#endif

#endif /* ZB_USE_ZB_TRAFFIC_WATCHDOG */

#endif /* ZB_WATCHDOG_H */
