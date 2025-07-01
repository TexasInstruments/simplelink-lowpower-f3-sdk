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
/* PURPOSE: Frequency agility for GreenPower cluster
*/

#ifndef ZB_ZCL_GRPW_FREQ_AGILITY_H
#define ZB_ZCL_GRPW_FREQ_AGILITY_H 1

#define ZB_ZCL_CLUSTER_ID_GRPW_FREQ_AGILITY  0xFCFA

enum zb_zcl_grpw_freq_agility_attr_e
{
  ZB_ZCL_ATTR_GRPW_FREQ_AGILITY_BRIDGE_ADDR         = 0x0000,
  ZB_ZCL_ATTR_GRPW_FREQ_AGILITY_ACTIVE_CHANNEL_MASK = 0x0001,
};

#define ZB_ZCL_CLUSTER_ID_GRPW_FREQ_AGILITY_SERVER_ROLE_INIT (zb_zcl_cluster_init_t)NULL
#define ZB_ZCL_CLUSTER_ID_GRPW_FREQ_AGILITY_CLIENT_ROLE_INIT (zb_zcl_cluster_init_t)NULL

#endif /* ZB_ZCL_GRPW_FREQ_AGILITY_H */
