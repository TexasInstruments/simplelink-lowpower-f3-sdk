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
/* PURPOSE: ZLL common definitions
*/

#ifndef ZB_ZLL_COMMON_H
#define ZB_ZLL_COMMON_H 1

#include <zboss_api_core.h> /* int types, zb_bufid_t */

/**
    @defgroup ZB_ZLL_COMMON ZLL Common constants, structures and functions
    @ingroup ZB_ZLL
    @addtogroup ZB_ZLL_COMMON
    ZLL Common constants, structures and functions
    @{
*/

#ifdef DEBUG
/* If defined, samples included to zll source files will be compiled - need for development only */
#define ZB_COMPILE_ZLL_SAMPLE
#endif

/************************** Common functions ****************************/

/**
 *  @brief Initializes ZLL device.
 */
void zll_init(void);

/**
 *  @brief Starts ZLL device.
 *  @returns Startup sequence schedule status (see @ref zb_ret_t):
 *  @li RET_OK on success;
 *  @li RET_OVERFLOW on callback ring buffer overflow.
 *
 *  On process start device invoke one or more times user function zb_zdo_startup_complete
 *  with zb_zll_transaction_task_status_t as buffer:
 *  @code
 *  zb_bufid_t  buffer = ZB_BUF_FROM_REF(param);
 *  zb_zll_transaction_task_status_t* task_status = ZB_BUF_GET_PARAM(buffer, zb_zll_transaction_task_status_t);
 *  TRACE_MSG(TRACE_ZLL3, "> zb_zdo_startup_complete %hd status %hd", (FMT__H_H, param, task_status->task));
 *  @endcode
 *  task_status->task - see @ref zb_zll_transaction_task_e
 *  task_status->status - see @ref zb_zll_task_status_e
 */
zb_ret_t zb_zll_dev_start(void);

/**
 *  @brief Finishes startup sequence.
 *  @param param [IN] - reference to the @ref zb_buf_t "buffer" to put task status notification to.
 */
void zll_continue_start(zb_uint8_t param);

/* TODO: [Multi-MAC] allow to set interface id */
/**
 *  @brief Changes channel.
 *  @param buf_idx [IN] - reference to the @ref zb_buf_t "buffer" to put request to.
 *  @param channel [IN] - channel number to set.
 *  @param callback [IN] - @ref zb_callback_t "callback" to be scheduled on operation finish.
 */
#define ZLL_SET_CHANNEL(buf_idx, channel, callback)                                 \
{                                                                                   \
  nwk_txstat_clear();                                                               \
  ZB_PIBCACHE_CURRENT_CHANNEL() = (channel);                                  \
  zb_nwk_pib_set((buf_idx), ZB_PIBCACHE_PRIMARY_IFACE(),                            \
    ZB_PHY_PIB_CURRENT_CHANNEL, &(channel), 1, (callback));                         \
}

/**
 *  @brief Main loop of process commissioning command
 *  @param param [IN] - reference to the @ref zb_buf_t "buffer" containing parameters of
 *  commissioning command @ref zb_zcl_parsed_hdr_s
 */
void zll_process_device_command(zb_uint8_t param);

/************************** Start new network ****************************/

/** @brief Start new network function parameters representation. */
typedef ZB_PACKED_PRE struct zb_zll_start_new_nwk_param_s
{
  zb_uint16_t pan_id;         /**< Short Pan ID to use for new network. */
  zb_ext_pan_id_t ext_pan_id; /**< Extended Pan ID to use for new network. */
  zb_uint8_t channel;         /**< Logical channel to start network on. */
  zb_ieee_addr_t dst_addr;    /**< Target device's address. */
} ZB_PACKED_STRUCT zb_zll_start_new_nwk_param_t;

#if defined ZB_ZLL_ENABLE_COMMISSIONING_CLIENT

zb_ret_t zb_zll_start_new_network(zb_uint8_t param);

/**
 *  @brief Requests a target to start a new network.
 *  @param buffer [IN] - @ref zb_buf_t "buffer" containing request parameters (see
 *  @param panid - short Pan ID to use for new network
 *  @param ext_panid - extended Pan ID to use for new network
 *  @param channel_index - logical channel to start network on
 *  @param dest_addr - target device's IEEE address
 *  @param status - return code, RET_OK on success, other value on error
 *  List of return codes  - see @ref zb_ret_t
 *    @li RET_OK on success;
 *    @li RET_INVALID_STATE if no active transaction found;
 *    @li RET_INVALID_PARAMETER if could not extract parameters from the buffer;
 *    @li RET_ERROR if could not schedule network start request for sending.
 */
#define ZB_ZLL_START_NEW_NETWORK(buffer, panid, ext_panid, channel_index, dest_addr, status) \
{                                                                                       \
    zb_zll_start_new_nwk_param_t *macro_param =                                               \
        (zb_zll_start_new_nwk_param_t*)ZB_BUF_GET_PARAM(buffer, zb_zll_start_new_nwk_param_t); \
    (void)zb_buf_reuse((buffer));                                                      \
    macro_param->pan_id = (panid);                                                            \
    ZB_IEEE_ADDR_COPY(&(macro_param->ext_pan_id), (ext_panid));                               \
    macro_param->channel = (channel_index);                                                   \
    ZB_IEEE_ADDR_COPY(&(macro_param->dst_addr), (dest_addr));                                 \
    (status) = zb_zll_start_new_network((buffer) );                   \
}

#else

#define ZB_ZLL_START_NEW_NETWORK(buffer, panid, ext_panid, channel_index, dest_addr, status)

#endif /* ZB_ZLL_ENABLE_COMMISSIONING_CLIENT */

/**
 *  @brief Finishes NLME-DIRECT-JOIN.confirm handling for start new network procedure.
 *  @param param [IN] - reference to the @ref zb_buf_t "buffer" containing operation status
 *  information.
 */
void zll_direct_join_confirm(zb_uint8_t param);

/*************************** ZLL transaction/task ***************************/

/** @brief ZLL transaction states */
enum zb_zll_transaction_task_e
{
  ZB_ZLL_NO_TASK,                       /**< No specific task. */
  ZB_ZLL_DEVICE_START_TASK,             /**< Device startup. */
  ZB_ZLL_DEVICE_DISCOVERY_TASK,         /**< Device info gathering (device discovery routine). */
  ZB_ZLL_IDENTIFY_TASK,                 /**< Identify process requested/started. Initiator only */
  ZB_ZLL_TRANSACTION_NWK_START_TASK,    /**< Start network. Initiator only */
  ZB_ZLL_TRANSACTION_NWK_START_TASK_TGT,/**< Start network. Target only */
  ZB_ZLL_TRANSACTION_JOIN_ROUTER_TASK,  /**< Network join router process requested/started. */
  ZB_ZLL_TRANSACTION_JOIN_ROUTER_TASK_TGT, /**< Network join router process requested/started (target side) */
  ZB_ZLL_TRANSACTION_JOIN_ED_TASK,      /**< Network join end device requested/started. */
  ZB_ZLL_TRANSACTION_JOIN_ED_TASK_TGT,  /**< Network join end device process requested/started (target side) */
  ZB_ZLL_TRANSACTION_RESET,             /**< Reset process requested  */
  ZB_ZLL_START_COMMISSIONING,           /**< Device start commissioning */
  ZB_ZLL_COMMISSIONED                   /**< Fake task id. It is used to inform user App on target side that device
                                             commissioning is complete */
};

/** @brief Possible values of transaction task status. */
enum zb_zll_task_status_e
{
  ZB_ZLL_TASK_STATUS_OK = 0,
  ZB_ZLL_TASK_STATUS_TIMED_OUT,
  ZB_ZLL_TASK_STATUS_SCHEDULE_FAILED,
  ZB_ZLL_TASK_STATUS_SCHEDULED,
  ZB_ZLL_TASK_STATUS_NO_CALLBACK,
  ZB_ZLL_TASK_STATUS_FINISHED,
  ZB_ZLL_TASK_STATUS_FAILED,
  ZB_ZLL_TASK_STATUS_WRONG_SIDE,
  ZB_ZLL_TASK_STATUS_NETWORK_UPDATED
}; /* enum zb_zll_task_status_e */

/** @brief Transaction task status report structure. */
typedef ZB_PACKED_PRE struct zb_zll_transaction_task_status_s
{
  /** @brief Transaction task status.
    *
    * For possible value list see @ref zb_zll_task_status_e.
    */
  zb_uint8_t status;
  /** @brief Finished @ref zb_zll_transaction_task_e "task." */
  zb_uint8_t task;
} ZB_PACKED_STRUCT zb_zll_transaction_task_status_t;

/************************** Device discovery ****************************/

/** @brief Device discovery request parameters. */
typedef ZB_PACKED_PRE struct zb_zll_device_discovery_req_s
{
  zb_uint8_t ext_scan;  /**< Perform extended scan sequence. */
} ZB_PACKED_STRUCT zb_zll_device_discovery_req_t;

/** @brief Fills in device discovery request and schedules device discovery process for execution.
  * @param buffer - buffer to put transaction packets to.
  * @param extended_scan - extended device scan procedure flag. 0 means standard scan procedure (on
  * primary channels only, and any non-zero value selects extended scan procedure (both primary and
  * secondary channel sets explored).
  * @param callback - callback function (@ref zb_callback_t type) that
  * should be called upon transaction state changed.
  * @param status - return code, RET_OK on success, other value on error
  */
#define ZB_ZLL_START_DEVICE_DISCOVERY(buffer, extended_scan, callback, status)  \
{                                                                               \
  (void)zb_buf_reuse((buffer));                                                 \
  ((zb_zll_device_discovery_req_t*)ZB_BUF_GET_PARAM(                            \
    buffer,                                                                     \
      zb_zll_device_discovery_req_t))->ext_scan = ((extended_scan) != ZB_FALSE);\
                                                                                \
  (status) = zb_schedule_callback(zb_zll_start_device_discovery, ZB_REF_FROM_BUF((buffer))); \
}

/** @brief Starts device discovery.
  * @param param - reference to the buffer to put packets to.
  * @note It is not recommended to call the function directly. Use @ref
  * ZB_ZLL_START_DEVICE_DISCOVERY() macro instead.
  */
void zb_zll_start_device_discovery(zb_uint8_t param);

/************************** Join router ****************************/

/** @brief Join router request parameters. */
typedef struct zb_zll_join_router_param_s
{
  zb_uchar_t device_index;  /**< Device index in device discovery results */
}
zb_zll_join_router_param_t;

/** @brief Perform Join router commissioning.
  * @param param - reference to the buffer with parameters.
  * @note It is not recommended to call the function directly. Use @ref
  * ZB_ZLL_JOIN_ROUTER() macro instead.
  */
void zb_zll_join_router(zb_uint8_t param);

/** @brief Fills in parameters for Join router and schedules it for execution.
  * @param buffer - buffer to put transaction packets to.
  * @param dev_index - Device index in device discovery results, it
  * points remote target device
  * @param callback - callback function (@ref zb_callback_t type) that
  * should be called upon transaction state changed.
  * @param status - return code, RET_OK on success, other value on error
  */
#define ZB_ZLL_JOIN_ROUTER(buffer, dev_index, callback, status)      \
  {                                                                  \
    zb_zll_join_router_param_t *macro_param;                               \
    (void)zb_buf_reuse((buffer));                                    \
    macro_param = (zb_zll_join_router_param_t*)ZB_BUF_GET_PARAM(buffer, zb_zll_device_discovery_req_t); \
    macro_param->device_index = (dev_index);                               \
                                                                     \
    (status) = zb_schedule_callback(zb_zll_join_router, (buffer)); \
}

/************************** Join end device ****************************/

/** @brief Join end device parameters representation. */
typedef struct zb_zll_join_end_device_param_s
{
  zb_uchar_t device_index;      /**< Target device index of device information table */
}
zb_zll_join_end_device_param_t;

/** @brief Perform Join end device commissioning.
  * @param param - reference to the buffer with parameters.
  * @note It is not recommended to call the function directly. Use @ref
  * ZB_ZLL_JOIN_ED() macro instead.
  */
void zb_zll_join_ed(zb_uint8_t param);

/** @brief Fills in parameters for Join end device and schedules it for execution.
  * @param buffer - buffer to put transaction packets to.
  * @param dev_index - Device index in device discovery results, it
  * points remote target device
  * @param callback - callback function (@ref zb_callback_t type) that
  * should be called upon transaction state changed.
  * @param status - return code, RET_OK on success, other value on error
  */
#define ZB_ZLL_JOIN_ED(buffer, dev_index, callback, status)         \
  {                                                                 \
    zb_zll_join_end_device_param_t *macro_param;                          \
    (void)zb_buf_reuse((buffer));                                   \
    macro_param = (zb_zll_join_end_device_param_t*)ZB_BUF_GET_PARAM(buffer, zb_zll_device_discovery_req_t); \
    macro_param->device_index = (dev_index);                              \
                                                                    \
    (status) = zb_schedule_callback(zb_zll_join_ed, (buffer)); \
}

/************************** Start commissioning ****************************/

/** @brief Perform Start commissioning.
  * @param param - reference to the buffer without parameters.
  * @return command status
  */
zb_ret_t zb_zll_start_commissioning(zb_uint8_t param);

 /** @brief Perform Add device to network.
  * @param param - reference to the buffer without parameters.
  * @note It recommended to call the function after finish call
  * Device discovery.
  */
void zll_add_device_to_network(zb_uint8_t param);

/** @brief Find device index with max rssi from device_infos table
  * @param route_only - find from all device or router only
  * @return Device index by device_infos table
  * @note It recommended to call the function after finish call
  * Device discovery.
  */
zb_int8_t zll_find_device_info_by_max_rssi(zb_bool_t route_only);

/**
   Find endpoint of known ZLL device by its nwk address

   @param addr - nwk address
   @return endpoint if success, 0xff if error
 */
zb_uint8_t zll_find_device_ep_by_short_addr(zb_uint16_t addr);

/**
   Find info (address & endpoint) about known ZLL device by its index

   @param i - index in the endpoints array
   @param addr - (out) found nwk address of remote device
   @param ep - (out) endpoint
   @return 0 if success, -1 if no ep number i - to be able to use
   zll_get_ep_info as an iterator
 */
zb_int_t zll_get_ep_info(zb_int_t i, zb_uint16_t *addr, zb_uint8_t *ep);

void zll_network_start_continue(zb_uint8_t param);

void zll_save_nwk_prefs(zb_ext_pan_id_t ext_pan_id, zb_uint16_t pan_id,
                             zb_uint16_t short_addr, zb_uint8_t channel);

/**
    @}

    @addtogroup ZB_ZLL_COMMON
    @internal
    @{
*/

/** @brief Get current inter-PAN ASDU handle value */
#define ZB_ZLL_GET_ASDU_HANDLE() (ZLL_TRAN_CTX.intrp_asdu_handle)

/** @brief Get new inter-PAN ASDU handle */
#define ZB_ZLL_GET_NEW_ASDU_HANDLE()  (++ZB_ZLL_GET_ASDU_HANDLE())

/** @brief Get address range size */
#define ZB_ZLL_ADDR_RANGE_SIZE(r) ((r).addr_end - (r).addr_begin)

/** @brief Get group ID range size */
#define ZB_ZLL_GROUP_RANGE_SIZE(r) ((r).group_id_end - (r).group_id_begin)

/** @brief ZLL security key index values */
enum zb_zll_key_index_value_e
{
  ZB_ZLL_DEVELOPMENT_KEY_INDEX = 0,
  ZB_ZLL_MASTER_KEY_INDEX = 4,
  ZB_ZLL_CERTIFICATION_KEY_INDEX = 15
};

/** @brief ZLL security key index values */
enum zb_zll_key_value_e
{
  ZB_ZLL_DEVELOPMENT_KEY = 0x0001,
  ZB_ZLL_MASTER_KEY = 0x0010,
  ZB_ZLL_CERTIFICATION_KEY = 0x8000 //AT: fix (see: Table 67 Key encryption algorithms)
};

/** @cond touchlink */

/** @brief Enumeration of touchlink actions, that should be approved
 * by end user application */
enum zb_zll_touchlink_action_e
{
  ZB_ZLL_ACTION_START_NEW_NETWORK,
  ZB_ZLL_ACTION_JOIN_ROUTER,
  ZB_ZLL_ACTION_JOIN_ED
};

/** @endcond */ /* touchlink */

/** Sets user identify start/stop handle callback. */
#define ZB_ZLL_REGISTER_IDENTIFYCALLBACK(callback) (ZLL_DEVICE_INFO().identify_handler = (callback))

/* ZLL network information */
#if OBSOLETE
/* Currently, network structure is not needed, all values are stored in NIB and PIB */
typedef ZB_PACKED_PRE struct zb_zll_network_ctx_s
{
  /* zb_uint8_t nwk_update_id; - ZB_NIB_UPDATE_ID() */
  /* zb_uint8_t logical_channel; - ZB_PIBCACHE_CURRENT_CHANNEL() */
  /* zb_ext_pan_id_t extended_pan_id; - ZB_NIB_EXT_PAN_ID() */
  /* zb_uint16_t short_pan_id; - ZB_PIBCACHE_PAN_ID()  */
  /*zb_uint8_t short_addr; -  ZB_PIBCACHE_NETWORK_ADDRESS() */
} ZB_PACKED_STRUCT zb_zll_network_ctx_t;
#endif

/** Sets select device callback */
#define ZB_ZLL_SET_SELECT_DEVICE_CB(cb)         ZLL_TRAN_CTX().select_device = (cb)

/**
 *  @brief Looks up device information table in the transaction context and sets current device
 *  information record index according to lookup results.
 *  If appropriate device was not found, current device record index will be set to the number of
 *  the currently stored records.
 *  @param ieee_addr [IN] - @ref zb_ieee_addr_t "IEEE address" of the device to be found.
 *  @param status [OUT] - device lookup status. Takes value of RET_OK on success, and
 *  RET_ERROR if no device was found. See @ref zb_ret_t
 */
#define ZB_ZLL_FIND_DEVICE_INFO_BY_ADDR(ieee_addr, status)                               \
{                                                                                        \
  (status) = RET_ERROR;                                                                  \
  for ( ZLL_TRAN_CTX().current_dev_info_idx = 0;                                         \
        ZLL_TRAN_CTX().current_dev_info_idx < ZLL_TRAN_CTX().n_device_infos;             \
        ++ZLL_TRAN_CTX().current_dev_info_idx)                                           \
  {                                                                                      \
    if (ZB_IEEE_ADDR_CMP(                                                                \
          (ieee_addr),                                                                   \
          ZLL_TRAN_CTX().device_infos[ZLL_TRAN_CTX().current_dev_info_idx].device_addr)) \
    {                                                                                    \
      (status) = RET_OK;                                                                 \
      break;                                                                             \
    }                                                                                    \
  }                                                                                      \
}

/**
 *  @brief Reports number of group identifiers required by the current device in the transaction
 *  context.
 */
#define ZB_ZLL_TRANS_GET_CURRENT_DEV_GROUP_ID_RANGE_LEN()                                    \
  (   (ZLL_TRAN_CTX().current_dev_info_idx == ZLL_TRAN_CTX().n_device_infos)                 \
   ?  0                                                                                      \
   :  ZLL_TRAN_CTX().device_infos[ZLL_TRAN_CTX().current_dev_info_idx].total_group_id_count)

#define ZB_ZLL_TRANS_GET_CURRENT_DEV_EP_INFO()                                                                       \
  ( ( ( ZLL_TRAN_CTX().current_dev_info_idx > ZLL_TRAN_CTX().n_device_infos ) ||                                     \
      ( ZLL_TRAN_CTX().device_infos[ZLL_TRAN_CTX().current_dev_info_idx].ep_info_idx > ZLL_TRAN_CTX().n_ep_infos ) ) \
    ? NULL                                                                                                           \
    : &(ZLL_TRAN_CTX().ep_infos[ZLL_TRAN_CTX().device_infos[ZLL_TRAN_CTX().current_dev_info_idx].ep_info_idx]) )

/**
 *  @brief Reports address assign capability of the current device selected in the transaction
 *  context.
 *  @returns @ref ZB_FALSE if device has no address assign capabilities, or no device selected
 *  either, and @ref ZB_TRUE otherwise.
 */
#define ZB_ZLL_TRANS_GET_CURRENT_DEV_ADDR_ASSIGN_CAPABILITY()                    \
  ((   ZLL_TRAN_CTX().device_infos[ZLL_TRAN_CTX().current_dev_info_idx].zll_info \
    &  ZB_ZLL_INFO_ADDR_ASSIGNMENT) != 0)

/**
 *  @brief Sets "report task result" callback.
 *  @param callback [IN] - @ref zb_callback_t "callback" to set.
 */
#define ZB_ZLL_REGISTER_COMMISSIONING_CB(callback)  \
  (ZLL_DEVICE_INFO().report_task_result = (callback))

/**
 *  @brief Sets "check action allowed" callback.
 *  @param callback [IN] - @ref zb_zll_is_action_allowed_cb_t "callback" to set.
 */
#define ZB_ZLL_REGISTER_ACTION_CHECK_CB(callback) (ZLL_TRAN_CTX().check_action_allowed = (callback))

/** Special transaction identifier value: no transaction. */
#define ZLL_NO_TRANSACTION_ID ((zb_uint32_t)0)

/** @brief Zigbee information device type subfield values */
enum zb_zll_zigbee_info_e
{
  ZB_ZLL_ZB_INFO_COORD_DEVICE_TYPE = 0x00,
  ZB_ZLL_ZB_INFO_ROUTER_DEVICE_TYPE = 0x01,
  ZB_ZLL_ZB_INFO_ED_DEVICE_TYPE = 0x02,
};

/** @brief Bit mask for ZigbeeInfo.DeviceType 2-bit subfield extraction. */
#define ZB_ZLL_ZB_INFO_RX_ON_WHEN_IDLE_MASK 0x4

/** @brief Bit mask for ZigbeeInfo.RxOnWhenIdle 1-bit subfield extraction. */
#define ZB_ZLL_ZB_INFO_DEVICE_TYPE_MASK 0x03

/* TODO Do we really need following 4 macros?
 * Yes, at last ZB_ZLL_ZB_INFO_GET_DEVICE_TYPE */
/** @brief Set ZigbeeInfo.DeviceType subfield.
  * @attention Assumes that dst was pre-zeroed.
  */
#define ZB_ZLL_ZB_INFO_SET_DEVICE_TYPE(dst, value) \
  ((dst) |= ((value) & ZB_ZLL_ZB_INFO_DEVICE_TYPE_MASK))

/** @brief Get ZigbeeInfo.DeviceType subfield.
  */
#define ZB_ZLL_ZB_INFO_GET_DEVICE_TYPE(zb_info) \
  ((zb_info)  & ZB_ZLL_ZB_INFO_DEVICE_TYPE_MASK)

/** @brief Set ZigbeeInfo.RxOnWhenIdle subfield.
  * @attention Assumes that dst was pre-zeroed.
  */
#define ZB_ZLL_ZB_INFO_SET_RX_ON_WHEN_IDLE(dst, value) \
    ((dst) |= ((value) & ZB_ZLL_ZB_INFO_RX_ON_WHEN_IDLE_MASK))

#define ZB_ZLL_ZB_INFO_GET_RX_ON_WHEN_IDLE(zb_info)              \
  (((zb_info) & ZB_ZLL_COMMISSIONING_SCAN_REQ_RX_ON_WHEN_IDLE_MASK) >> 2)

/**
 * @}
 */

#endif /* ZB_ZLL_COMMON_H */
