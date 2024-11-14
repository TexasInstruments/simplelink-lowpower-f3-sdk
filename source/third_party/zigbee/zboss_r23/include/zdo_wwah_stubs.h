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
/* PURPOSE: WWAH ZDO Level selector for ZCL WWAH Stubs for all func.
*/

#ifndef ZDO_WWAH_STUBS_H
#define ZDO_WWAH_STUBS_H 1

#include "zdo_hubs.h"

#if (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_SERVER)
#include "zcl/zb_zcl_wwah.h"

#define ZB_ZDO_CHECK_IF_WWAH_SERVER_BEHAVIOR() zb_is_wwah_server() /* used in zcl */
#define ZB_ZDO_CHECK_NEW_CHANNEL(new_channel_mask) zb_zcl_wwah_check_new_channel(new_channel_mask)
#define ZB_ZDO_CHECK_NEW_PANID(new_panid) zb_zcl_wwah_check_new_panid(new_panid)
#define ZB_ZDO_GET_MAX_PARENT_THRESHOLD_RETRY() ZDO_CTX().max_parent_threshold_retry * (1U + zb_zcl_wwah_mac_poll_cca_retry_count())

 /* Common logic when we have problems with parent polling:
  * 1) Data Request + 3 MAC Retries
  * 2) Wait MACPollFailureWaitTime
  * 3) Repeat steps 1-2 wwah_retry_cnt times
  * 4) Wait work_poll_interval
  * 5) Repeat steps 1-4 ZDO_CTX().max_parent_threshold_retry times
  */

 /* Note:
  *   1) if current retry count(crc) equals zero it means that last poll was success
  *   2) crc >= maximum_retry_count leads to polling stop
  *   3) crc % (wwah_retry_cnt + 1) shows current iteration of additional polling,
  *      i.e. loop steps 1-2 above. If remainder is zero - wait for standard timeout,
  *      or additional timeout otherwise.
  */

#define ZB_ZDO_GET_CURRENT_POLL_ITERATION_INTERVAL(wait_time)                              \
   if (zb_zcl_wwah_mac_poll_cca_retry_count() &&                                          \
    ZDO_CTX().parent_threshold_retry % (1 + zb_zcl_wwah_mac_poll_cca_retry_count()))      \
   {                                                                                      \
     (wait_time) = zb_zcl_wwah_mac_poll_failure_wait_time();                              \
   }                                                                                      \
   else                                                                                   \
   {                                                                                      \
     (wait_time) = ZDO_CTX().pim.work_poll_interval;                                      \
   }

#define ZB_ZDO_CHECK_IF_APS_ACK_NEEDED(cluster_id) zb_zcl_wwah_check_if_aps_ack_needed(cluster_id) /* used in zcl */
#define ZB_ZDO_CHECK_IF_APS_LINK_KEY_AUTHORIZATION_NEEDED(cluster_id) zb_zcl_wwah_check_if_aps_link_key_authorization_needed(cluster_id) /* used in zcl */
#define ZB_ZDO_CHECK_IF_FORCED_TO_USE_TC(cluster_id) zb_zcl_wwah_check_if_forced_to_use_tc(cluster_id) /* used in zcl */
#define ZB_ZDO_CHECK_CLUSTER_PERMISSION(nwk_addr, cluster_id) zb_zcl_wwah_check_cluster_permission(nwk_addr, cluster_id) /* used in zcl */
#define ZB_ZDO_SEND_POWER_DESCRIPTOR_CHANGE() zb_zcl_wwah_schedule_send_power_descriptor_change() /* AF to ZCL signal - keep */
#define ZB_ZDO_CHECK_IF_INTERPAN_SUPPORTED() zb_zcl_wwah_check_if_interpan_supported() /* moved */
#define ZB_ZDO_CHECK_SRC_ADDRESS_FOR_CLUSTER(short_addr, cluster_id) zb_zcl_wwah_check_src_addr_for_cluster(short_addr, cluster_id) /* used in zcl */
#else
#define ZB_ZDO_CHECK_IF_WWAH_SERVER_BEHAVIOR() ZB_FALSE
#define ZB_ZDO_CHECK_NEW_CHANNEL(new_channel_mask) ZB_TRUE
#define ZB_ZDO_CHECK_NEW_PANID(new_panid) ZB_TRUE
#define ZB_ZDO_GET_MAX_PARENT_THRESHOLD_RETRY() ZDO_CTX().max_parent_threshold_retry
#define ZB_ZDO_GET_CURRENT_POLL_ITERATION_INTERVAL(wait_time) (wait_time) = ZDO_CTX().pim.work_poll_interval
#define ZB_ZDO_CHECK_IF_APS_ACK_NEEDED(cluster_id) ZB_TRUE
#define ZB_ZDO_CHECK_IF_APS_LINK_KEY_AUTHORIZATION_NEEDED(cluster_id) ZB_FALSE
#define ZB_ZDO_CHECK_IF_FORCED_TO_USE_TC(cluster_id) ZB_FALSE
#define ZB_ZDO_CHECK_CLUSTER_PERMISSION(nwk_addr, cluster_id) ZB_TRUE
#define ZB_ZDO_SEND_POWER_DESCRIPTOR_CHANGE()
#define ZB_ZDO_CHECK_IF_INTERPAN_SUPPORTED() ZB_TRUE
#define ZB_ZDO_CHECK_SRC_ADDRESS_FOR_CLUSTER(short_addr, cluster_id) ZB_TRUE
#endif /* (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_SERVER) */

#endif /* ZDO_WWAH_STUBS_H */
