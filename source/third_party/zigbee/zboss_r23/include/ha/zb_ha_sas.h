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
/* PURPOSE: HA startup attribute sets definitions
*/
#ifndef ZB_HA_SAS_H
#define ZB_HA_SAS_H 1

#include "zb_ha_config.h"

#if defined ZB_ENABLE_HA_SAS

#define ZB_HA_SAS_SHORT_ADDRESS ZB_NWK_BROADCAST_ALL_DEVICES
#define ZB_HA_SAS_EXT_PAN_ID {0, 0, 0, 0, 0, 0, 0, 0}
#define ZB_HA_SAS_PAN_ID ZB_BROADCAST_PAN_ID
#define ZB_HA_SAS_CHANNEL_MASK ZB_TRANSCEIVER_ALL_CHANNELS_MASK

#define ZB_HA_SAS_TRUST_CENTER_ADDR 0
#define ZB_HA_SAS_MASTER_KEY NULL
#define ZB_HA_SAS_NETWORK_KEY NULL
#define ZB_HA_SAS_DTC_LINK_KEY  \
  "\x5a\x69\x67\x42\x65\x65\x41\x6C\x6C\x69\x61\x6E\x63\x65\x30\x39"

#define ZB_HA_SAS_INSECURE_JOIN ZB_TRUE

#define ZB_HA_SAS_SCAN_ATTEMPTS 3

/* 5.3.2 Join Parameters
TimeBetweenScans
(1 second) Determines the number of seconds between each unsuccessful scan
attempt
 */
#define ZB_HA_SAS_TIME_BTWN_SCANS 1000

void zb_ha_process_sas(void);

#endif /* defined ZB_ENABLE_HA_SAS */

#endif /* ZB_HA_SAS_H */
