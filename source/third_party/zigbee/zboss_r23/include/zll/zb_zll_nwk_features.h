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
/* PURPOSE: ZLL network features
*/


#ifndef ZB_ZLL_NWK_FEATURES_H
#define ZB_ZLL_NWK_FEATURES_H 1

/**
 *  @internal
 *  @defgroup ZB_ZLL_NWK_FEATURES ZLL NWK features
 *  @ingroup ZB_ZLL
 *  @addtogroup ZB_ZLL_NWK_FEATURES
 *  ZLL NWK features.
 *  @{
 */


/* FIXME: why macro?? Implement as a function! */
/**
 *  @brief Perform network rejoin procedure.
 *  @param buffer [IN] - pointer to the @ref zb_buf_t "buffer" to put arguments to.
 *  @param ext_pan_id [IN] - extended Pan ID to use for rejoin.
 *  @param logical_channel [IN] - logical channel number to use for join.
 *  @param status [OUT] - result of the operation schedule (see @ref ZB_SCHEDULE_CALLBACK).
 */
#define ZLL_REJOIN_NWK(buffer, ext_pan_id, logical_channel, status)     \
{                                                                       \
  zb_channel_list_t channel_list;                                       \
  zb_channel_list_init(channel_list);                                   \
  zb_channel_page_list_set_2_4GHz_mask(channel_list,                    \
    (1u << ZB_PIBCACHE_CURRENT_CHANNEL()));                             \
  (status) = zdo_initiate_rejoin((buffer),                              \
                                 (ext_pan_id),                          \
                                 channel_list,                          \
                                 ZG->aps.authenticated,                 \
                                 ZB_DEFAULT_SCAN_DURATION);             \
}

/**
 *  @brief Perform network leave procedure.
 *  @param buffer [IN] - pointer to the @ref zb_buf_t "buffer" to put request data to.
 *  @param status [OUT] - result of the operation schedule (see @ref ZB_SCHEDULE_CALLBACK).
 */
#define ZLL_LEAVE_NWK(buffer, status)                                                             \
{                                                                                                 \
  ZB_BZERO(ZB_BUF_GET_PARAM((buffer), zb_nlme_leave_request_t), sizeof(zb_nlme_leave_request_t)); \
  ZB_SET_JOINED_STATUS(ZB_TRUE);                                                                \
  (status) = zb_schedule_callback(zb_nlme_leave_request, (buffer));              \
}


#if defined ZB_ROUTER_ROLE


#define ZB_ZLL_START_ROUTER(buffer, ext_pan_id_, short_pan_id_, channel_, short_addr_, ret)      \
{                                                                                   \
  zb_zll_start_router_param_t *start_param = ZB_BUF_GET_PARAM(buffer, zb_zll_start_router_param_t); \
                                                                                    \
  ZB_EXTPANID_COPY(start_param->ext_pan_id, (ext_pan_id_));                         \
  start_param->short_pan_id = (short_pan_id_);                                      \
  start_param->channel = (channel_);                                                \
  start_param->short_addr = short_addr_;                                            \
  (ret) = zb_schedule_callback(zll_start_router, buffer);          \
}

#if defined ZB_ENABLE_ZLL
/**
 *  @brief Starts router device in ZLL Commissioning call chain.
 *  @param param [IN] - reference to a @ref zb_buf_t "buffer" to put request to.
 */
void zll_start_router(zb_uint8_t param);
#endif /* ZB_ENABLE_ZLL */
#endif /* defined ZB_ROUTER_ROLE */

/**
 *  @brief Schedules direct join of the initiator device.
 *  Device capabilities for the initiator won't be filled in, because they'll be overwritten on
 *  device rejoin.
 *  @param buffer [IN] - pointer to the @ref zb_buf_t "buffer" to put request to.
 *  @param ieee_address [IN] - @ref zb_ieee_addr_t "IEEE address" of the initiator.
 *  @param status [OUT] - operation schedule status (see @ref zb_ret_t).
 */
#define ZLL_DIRECT_JOIN(buffer, ieee_address, status)                                             \
{                                                                                                 \
  zb_nlme_direct_join_request_t* req = ZB_BUF_GET_PARAM((buffer), zb_nlme_direct_join_request_t); \
  (void)zb_buf_reuse((buffer));                                                                   \
  ZB_BZERO(req, sizeof(*req));                                                                    \
  ZB_IEEE_ADDR_COPY(req->device_address, (ieee_address));                                         \
  (status) = zb_schedule_callback(zb_nlme_direct_join_request, (buffer)); \
}

#if defined ZB_ENABLE_ZLL
/**
 *  @brief ZLL processing for NLME-LEAVE.confirm.
 *  @param param [IN] - reference to the @ref zb_buf_t "buffer" containing NLME-LEAVE.confirm data.
 */
void zll_leave_nwk_confirm(zb_uint8_t param);
#endif /* ZB_ENABLE_ZLL */

/**
 * @}
 */

#if defined ZB_ENABLE_ZLL
void zll_send_mgmt_nwk_update_change_channel_cb(zb_uint8_t param);
zb_bool_t zll_check_channel_for_mgmt_nwk_update_change_channel(zb_uint8_t channel);
#endif /* ZB_ENABLE_ZLL */

/** @brief Send zdo channel change request with internal, predefined callback (Mgmt_NWK_Update_req)
After successful confirm on this call automatic rejoin will be started
*/
#define ZB_ZLL_NWK_UPDATE_SEND_CHANGE_CHANNEL_REQ(buffer_, channel_)                              \
{                                                                                                 \
  ZB_ZLL_NWK_UPDATE_SEND_CHANGE_CHANNEL_REQ_WITH_CB(buffer_,                                      \
                                                    channel_,                                     \
                                                    zll_send_mgmt_nwk_update_change_channel_cb);  \
}

/** @brief Send zdo channel change request with callback macro (Mgmt_NWK_Update_req) */
#define ZB_ZLL_NWK_UPDATE_SEND_CHANGE_CHANNEL_REQ_WITH_CB(buffer_, channel_, cb_)                 \
{                                                                                                 \
  ZB_ZLL_NWK_UPDATE_SEND_REQ(buffer_,                                                             \
                             channel_,                                                            \
                             ZB_ZDO_NEW_ACTIVE_CHANNEL,                                           \
                             ZB_NWK_BROADCAST_RX_ON_WHEN_IDLE,                                    \
                             0,                                                                   \
                             0,                                                                   \
                             cb_);                                                                \
}

/** @brief Macro wrapper for zdo function (Mgmt_NWK_Update_req) */
/* @note Current channel will be changed (by calling void zb_zdo_set_channel_cb function)
and saved in local pibcache, then it will be stored in nvram (if enabled),
see zb_zdo_set_channel_conf
*/
#define ZB_ZLL_NWK_UPDATE_SEND_REQ(                                                               \
    buffer_,                                                                                      \
    scan_channels_,                                                                               \
    scan_duration_,                                                                               \
    dst_addr_,                                                                                    \
    scan_count_,                                                                                  \
    manager_addr_,                                                                                \
    cb_)                                                                                          \
{                                                                                                 \
  zb_zdo_mgmt_nwk_update_req_t *_req = ZB_BUF_GET_PARAM((buffer_), zb_zdo_mgmt_nwk_update_req_t); \
  (void)zb_buf_reuse((buffer_));                                                                  \
  ZB_BZERO(_req, sizeof(zb_zdo_mgmt_nwk_update_req_t));                                           \
  _req->hdr.scan_channels = (scan_channels_);                                                     \
  _req->hdr.scan_duration = (scan_duration_);                                                     \
  _req->dst_addr = (dst_addr_);                                                                   \
  _req->scan_count = (scan_count_);                                                               \
  _req->manager_addr = (manager_addr_);                                                           \
  zb_zdo_mgmt_nwk_update_req(ZB_REF_FROM_BUF(buffer_), cb_);                                      \
}

#endif /* ZB_ZLL_NWK_FEATURES_H */
