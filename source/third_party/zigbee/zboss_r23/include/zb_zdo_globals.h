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
/* PURPOSE: ZDO globals definition
*/

#ifndef ZB_ZDO_GLOBALS_H
#define ZB_ZDO_GLOBALS_H 1

#ifdef ZB_TEST_PROFILE
#include "zb_test_profile.h"
#endif /* ZB_TEST_PROFILE */
#include "zb_zdo_rjb.h"
#include "zb_zdo.h"
#include "zdo_diagnostics.h"
#include "zboss_api_zdo.h"
#ifdef ZB_PHY_TESTING_MODE
#include "zboss_api_mac.h"
#endif /* ZB_PHY_TESTING_MODE */

/*! @cond internals_doc */
/*! \addtogroup zdo_ib */
/*! @{ */

#define ZB_ZDO_NODE_DESC() (&ZG->zdo.conf_attr.node_desc)
#define ZB_ZDO_NODE_POWER_DESC() (&ZG->zdo.conf_attr.node_power_desc)
#define ZB_ZDO_SIMPLE_DESC() (&ZG->zdo.conf_attr.zdo_simple_desc)
#define ZB_ZDO_SIMPLE_DESC_LIST() (ZG->zdo.conf_attr.simple_desc_list)
#define ZB_ZDO_SIMPLE_DESC_NUMBER() (ZG->zdo.conf_attr.simple_desc_number)

/**
  ZDO Configuration Attributes
 */
typedef struct zb_zdo_configuration_attributes_e
{
  /* nwk_time_btwn_scans and nwk_scan_attempts moved to zb_commissioning_ctx_t */

  zb_af_node_desc_t       node_desc;        /*!< Node Descriptors */
  zb_af_node_power_desc_t node_power_desc;  /*!< Node Power Descriptors */
  zb_af_simple_desc_8_9_t zdo_simple_desc; /* TODO: remove it, ZDO simple descriptor is not needed - simple descriptors for EP >= 1 are used */
  /* TODO: make real list support, if multiple EP are supported */
  /* TODO: each Zigbee device application declares a device context and a list of EPs. Each EP
     stores its simple descriptor => no need store it here, it is better to use that storage */
  zb_af_simple_desc_1_1_t *simple_desc_list[ZB_MAX_EP_NUMBER];  /*!< Simple Descriptors table */
  zb_uint8_t simple_desc_number;                                /*!< Number elements of Simple Descriptors table */

#ifndef ZB_LITE_NO_END_DEVICE_BIND
  zb_uint8_t enddev_bind_timeout; /* Timeout value in seconds employed
                                   * in End Device Binding */
#endif
  zb_uint8_t permit_join_duration; /*!< Permit join duration, 0x00 - disable
                                    * join, 0xff - join is allowed forever */


} zb_zdo_configuration_attributes_t;


/*! @} */

/*! \addtogroup ZB_ZDO */
/*! @{ */


/**
   Some ZDO private data
 */
#define ZB_TEST_DATA_SIZE 10U

#define ZB_ZDO_MAX_CLUSTER_LIST 15U

#define ZB_ZDO_MAX_JOINING_TRY_ATTEMPTS 3U

typedef struct zb_zdo_rejoin_ctx_s
{
  zb_channel_list_t channels_list;
  zb_ext_pan_id_t   ext_pan_id;
  zb_bool_t         secure_rejoin;
  zb_uint16_t       prev_parent;
  zb_uint8_t        scan_duration;
  zb_uint16_t       prev_panid;
  zb_uint8_t        prev_page;
  zb_uint8_t        prev_channel;
} zb_zdo_rejoin_ctx_t;


/**
   Some ZDO internal flags
 */
typedef struct zp_zdo_handle_s
{
  zb_bitbool_t rejoin:1;                    /*!< if !0, this is rejoin  */
  zb_bitbool_t tc_significance:1;           /*!< data for permit_joining */
  zb_bitbool_t channel_update_disabled:1;   /*!< if !0, Channel update is disabled */
  zb_bitbool_t tx_fail_debug_enabled:1;     /*!< if !0, ZC broadcasts zdo_unsol_enh_update_notify with tx failures info */
  zb_uint8_t permit_joining_param;          /*!< if !0, nlme-permit_joining will
                                             * be executed */
  zb_uint8_t permit_duration;               /*!< data for permit_joining */
  zb_uint8_t dev_annce_param;               /*!< if !0, this is buffer id - device annonce
                                             * is sent */
  zb_uint8_t key_sw;                        /*!< if !0, key switch is sent and must switch
                                             * the key after this buffer sent  */
  zb_uint8_t mgmt_leave_resp_buf;           /*!< if !0, nlme_leave_request for itself will be executed
                                             * set after device was requested to leave the network
                                             * via mgmt_leave_req */
  zb_uint8_t key_upd_resp_param;            /* if !0, this is buffer id - key update resp is sent */
  zb_uint8_t verify_key_param;              /* if !0, this is buffer id - verify key is sent */
  zb_uint8_t secur_challenge_rsp_param;     /* if !0, this is buffer id - sec_callenge_rsp is sent */
  zb_uint8_t auth_token_rsp_param;          /* if !0, this is buffer id - get_auth_token_rsp is sent */
  zb_zdo_rejoin_ctx_t rejoin_ctx;
#ifdef ZB_MACSPLIT_HOST
  zb_bool_t start_no_autostart;              /*!< if ZB_TRUE, device started with start_no_autostart
                                              * Used only for macsplit host*/
#endif
 } zp_zdo_handle_t;

/* Flag to set and check if channel update is disabled */
#define ZB_ZDO_NETWORK_MGMT_CHANNEL_UPDATE_IS_DISABLED() (ZG->zdo.handle.channel_update_disabled != 0U)

#define ZB_ZDO_TX_FAIL_DEBUG_ENABLED() (ZG->zdo.handle.tx_fail_debug_enabled != 0U)

/* Flag to set and check if channel update is disabled */
#define ZB_ZDO_IS_REJOIN_SECURE() (ZG->zdo.handle.rejoin_ctx.secure_rejoin != 0U)

typedef struct zb_zdo_pim_s
{
  zb_time_t fast_poll_interval;
  zb_time_t fast_poll_timeout;
  zb_time_t long_poll_interval;
  zb_time_t turbo_poll_interval;
  zb_time_t turbo_poll_min;
  zb_time_t turbo_poll_max;
  zb_time_t turbo_poll_timeout;
  zb_time_t work_poll_max;
  zb_time_t work_poll_interval;
  zb_uint_t turbo_poll_n_packets;
  zb_time_t first_poll_time;
  zb_uint_t n_polls_per_data;
  zb_time_t turbo_leave_alarm_time;
  zb_uint_t poll_repeat;
  zb_uint8_t mac_poll_retry_count;
  zb_time_t mac_poll_failure_wait_time;
#ifdef SNCP_MODE
  zb_callback_t single_poll_cb;     /* set when executed Single Poll from Host */
#endif
  zb_uint8_t turbo_poll_not_received_data_pkt_cnt;
  zb_bitbool_t turbo_prohibit:1;
  zb_bitbool_t fast_poll_on:1;
  zb_bitbool_t poll_in_progress:1;
  zb_bitbool_t poll_stop:1;
  zb_bitbool_t was_in_turbo_poll:1; /* special value to indicate that pkt was not received,
                                      * the turbo poll should be restarted*/
  zb_bitbool_t enable_turbo_poll_retry_if_pkt_not_received:1;
} zb_zdo_pim_t;

/* Callback types */
#define CB_TYPE_DEFAULT       0x00U
#define CB_TYPE_TSN           0x01U

typedef ZB_PACKED_PRE struct zdo_cb_hash_ent_s
{
  zb_callback_t func;
  zb_uint8_t    tsn;
  zb_uint8_t    resp_counter;
  zb_bitfield_t    type:2;
  zb_bitfield_t    clock_counter:6; /*!< Used to track entry's life-time in hash */
} ZB_PACKED_STRUCT zdo_cb_hash_ent_t;

#define ZB_ZDO_CHECK_FAILS_MASK     1U      /* flag to check if channel interference check is in progress */
#define ZB_ZDO_SEND_WITH_ACK_MASK   (1U << 1U) /* flag to specify sending with acknowledgement */
#define ZB_ZDO_CHANNEL_CHANGED_MASK (1U << 2U) /* flag shows that channel change operation was performed recently (server side) */
#define ZB_ZDO_TC_REJOIN_ENABLE_MASK (1U << 3U) /* flag that allows or disallows Trust Center Rejoin (Unsequred rejoin) */
#define ZB_ZDO_UNAUTH_MASK (1U << 4U) /* indicates unauthenticated join case */

/* TODO: kill bitmasks here - use bitfields */
#define ZB_ZDO_SET_CHECK_FAILS()   (ZG->zdo.flags |= ZB_ZDO_CHECK_FAILS_MASK)
#define ZB_ZDO_CLEAR_CHECK_FAILS() (ZG->zdo.flags &= ~ZB_ZDO_CHECK_FAILS_MASK)
#define ZB_ZDO_GET_CHECK_FAILS()   (ZG->zdo.flags & ZB_ZDO_CHECK_FAILS_MASK)

#define ZB_ZDO_SET_SEND_WITH_ACK()   (ZG->zdo.flags |= ZB_ZDO_SEND_WITH_ACK_MASK)
#define ZB_ZDO_CLEAR_SEND_WITH_ACK() (ZG->zdo.flags &= ~ZB_ZDO_SEND_WITH_ACK_MASK)
#define ZB_ZDO_GET_SEND_WITH_ACK()   (ZG->zdo.flags & ZB_ZDO_SEND_WITH_ACK_MASK)

#define ZB_ZDO_SET_CHANNEL_CHANGED()   (ZG->zdo.flags |= ZB_ZDO_CHANNEL_CHANGED_MASK)
#define ZB_ZDO_CLEAR_CHANNEL_CHANGED() (ZG->zdo.flags &= ~ZB_ZDO_CHANNEL_CHANGED_MASK)
#define ZB_ZDO_GET_CHANNEL_CHANGED()   (ZG->zdo.flags & ZB_ZDO_CHANNEL_CHANGED_MASK)

#define ZB_ZDO_SET_UNAUTH()   (ZG->zdo.flags |= ZB_ZDO_UNAUTH_MASK)
#define ZB_ZDO_CLEAR_UNAUTH() (ZG->zdo.flags &= ~ZB_ZDO_UNAUTH_MASK)
#define ZB_ZDO_GET_UNAUTH()   (ZG->zdo.flags & ZB_ZDO_UNAUTH_MASK)
#define ZB_ZDO_SET_LEAVE_REQ_ALLOWED( v ) (ZG->zdo.leave_req_allowed = ( v ))
#define MAX_N_CLUSTER_FILTERS 2U

typedef struct zdo_cluster_filters_s
{
  zb_uint_t n_filters;
  zdo_cluster_filter_t filters[MAX_N_CLUSTER_FILTERS];
} zdo_cluster_filters_t;

struct zb_zdo_device_annce_s;
/*! @endcond */

#if defined ZB_JOINING_LIST_SUPPORT && defined ZB_ROUTER_ROLE
typedef struct zb_joining_list_ctx_s
{
  zb_joining_list_op_q_t operation_queue;
  zb_callback_t current_op_cb;
  zb_bool_t is_consistent;
  zb_uint8_t update_id;
  zb_uint16_t list_expiry_interval; /* mibIeeeExpiryInterval (minutes) -  must be in mac PIB,
                                       but the strict implementation isn't worth the effort for now */

  /* context for zdo_nwk_joining_list_resp */
  zb_uint8_t tsn;
  zb_uint16_t dst_addr;
  zb_callback_t broadcast_confirm_cb;
  zb_uint8_t original_buffer;

  /* context for announce */
  zb_uint8_t next_start_index;
  zb_uint8_t current_list_size;

  /* context for request */
  zb_uint8_t prev_update_id;
} zb_joining_list_ctx_t;
#endif /* defined ZB_JOINING_LIST_SUPPORT && defined ZB_ROUTER_ROLE */

typedef struct zb_survey_beacons_ctx_s
{
  /**
   *  @brief Reference to a buffer with Survey Beacons Response parameters
   */
  zb_bufid_t  resp_ref;
  zb_uint16_t dst_addr;
  zb_uint8_t  tsn;
  zb_bool_t   aps_secur;
  zb_bool_t   is_zdo;
  zb_uint8_t  scan_type;
  zb_uint8_t  current_cnt;
  zb_uint8_t  channel_page_cnt;
  zb_uint32_t channel_page_list[ZB_SURVEY_BEACON_MAX_CHANNEL_PAGE_CNT];

} zb_survey_beacons_ctx_t;

typedef struct zb_zdo_hubs_context_s
{
  zb_bitfield_t configuration_mode_disabled:1;       /* note: enabled by default */
  /* Flags and vars moved there from WWAH cluster ctx or used instead of direct access to WWZH cluster attributes. */
  zb_bitfield_t parent_classification_enabled:1;
  zb_bitfield_t require_lk_encryption:1; /* note: allowed by default */
  zb_bitfield_t leave_without_rejoin_disallowed:1;   /* note: allowed by default */
  zb_bitfield_t interpan_disabled:1;                 /* note: enabled by default */
  /* ======= Survey Beacon related context ======= */
  zb_survey_beacons_ctx_t survey_beacons_ctx;
} zb_zdo_hubs_context_t;

typedef struct zb_zdo_channel_panid_change_ctx_s
{
  zb_ret_t   state;
  zb_uint8_t buffers_max;
  zb_uint8_t buffers_used;
  zb_bool_t is_panid_change;
  zb_uint16_t next_panid;
  zb_uint32_t next_channel;
  zb_uint8_t current_key_pair_idx;
  zb_uint16_t error_cnt;
  zb_bool_t reiterated;
  zb_callback_t cb;
} zb_zdo_channel_panid_change_ctx_t;

typedef zb_bool_t (*zb_zdo_match_desc_resp_handler_t)(zb_bufid_t);

typedef struct zb_zdo_func_selector_s
{
  zb_zdo_match_desc_resp_handler_t match_desc_resp_handler;
  zb_callback_t app_link_key_ind_handler;
} zb_zdo_func_selector_t;

/**
    ZDO_CTX structure
 */
typedef struct zb_zdo_globals_s
{
  zp_zdo_handle_t handle;                           /*!< */
  zb_uint8_t      tsn;                              /*!< */
  zb_zdo_tsn_policy_t tsn_policy;                   /*!< see zb_zdo_tsn_policy_e */
#ifdef ZBOSS_ZDO_APP_TSN_ENABLE
  zb_callback_t       tsn_lock_cb;                  /*!< callback to be called to sync app tsn allocate  */
  zdo_app_tsn_entry_t app_tsn_table[ZBOSS_ZDO_APP_TSN_TABLE_SIZE];
#endif
  zb_device_handler_t   af_data_cb;                 /*!< Callback of zb_apsde_data_indication
                                                     * function */
  /*!< callback which called when ZDO command is sent (except cases when it
   * failed to be sent, or when no answers are supposed - in such case ZDO
   * callback passed to ZDO call is called) */
  zb_callback_t   af_zdo_data_conf_cb;
  zb_uint8_t      max_parent_threshold_retry;       /*!< */
  zb_uint8_t      parent_threshold_retry;           /*!< */
  zdo_cb_hash_ent_t zdo_cb[ZDO_TRAN_TABLE_SIZE];    /*!< */
#ifndef ZB_LITE_NO_ZDO_SYSTEM_SERVER_DISCOVERY
  zb_callback_t   system_server_discovery_cb;       /*!< Callback of System server discovery */
  zb_uint8_t      system_server_discovery_tsn;      /*!< */
#endif
#ifndef ZB_LITE_NO_OLD_CB
  zb_callback_t   app_leave_cb;                     /*!< Indication for application that leave
                                                     * performed and no further actions scheduled*/
#endif
  zb_callback_t   app_addr_resp_cb;                 /*!< Callback of zb_zdo_addr_resp_handle
                                                     * function */
  /* TODO: change flags to bitfields */
  zb_uint8_t      flags;                            /*!< */
  zb_bitfield_t   zdo_ed_scan_busy:1;
  zb_bitfield_t   leave_req_allowed:1; /*! allow/deny ZDO leave request */
  zb_uint8_t      long_timer; /*!< Long timer, in minutes */
  zb_callback_t   long_timer_cb; /*!< Callback for long timer */
  zb_uint8_t      long_timer_param; /*!< Parameter for long timer */
#ifdef ZB_ED_FUNC
  zb_uint8_t      parent_link_failure;              /*!< */
#endif
  zb_uint16_t     num_packets;                      /*!< Number of packets for diag purpose*/
  zb_uint16_t     num_retries;                      /*!< Number of retries for diag purpose*/

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE
  zb_uint8_t      permit_join_time; /*!< LAst permit join time in seconds */
#endif

#ifdef ZB_ROUTER_ROLE
/*NBT element to start parent_annce from */
  zb_uint8_t parent_annce_position;
#endif

#if defined ZB_ENABLE_INTER_PAN_NON_DEFAULT_CHANNEL
  zb_af_inter_pan_handler_t af_inter_pan_data_cb;   /*!< Application callback called when inter-pan packet is received */
#endif

  zb_zdo_configuration_attributes_t conf_attr; /*!< Device Configuration
                                                * Attributes */
  union zb_zdo_ctx_u
  {
    struct zb_nwk_upd_req_ctx_s
    {
      zb_uint8_t tsn;                       /*!< */
      zb_uint16_t dst_addr;                 /*!< */
      zb_callback_t cb;
      zb_bitfield_t ed_scan_busy:1;
      zb_bitfield_t enhanced:1;
    }
    nwk_upd_req;
    struct zb_get_peer_short_addr_s
    {
      zb_address_ieee_ref_t dst_addr_ref;   /*!< */
      zb_callback_t cb;                     /*!< */
      zb_uint8_t param;                     /*!< */
    }
    get_short_addr_ctx;
#ifndef ZB_LITE_NO_END_DEVICE_BIND
    struct zb_zdo_end_device_bind_ctx_s
    {
#define ZB_ZDO_BIND_DEV_1 0U
#define ZB_ZDO_BIND_DEV_2 1U
#define ZB_ZDO_PEER_DEVICE_NUM(num) (((num) + 1U) % 2U)

      struct zb_zdo_bind_device_info_s
      {
        zb_uint8_t end_device_bind_param; /*!< Reference to buffer with
                                           * end_device_bind_req command */
        zb_uint16_t binding_target;  /*!< The address of the target for the binding */
        zb_uint8_t  src_endp;        /*!< The endpoint on the binding device */
        zb_uint16_t cluster_list[ZB_ZDO_MAX_CLUSTER_LIST]; /* matched clusters list */
        zb_uint8_t cluster_num;      /*!< number of matched clusters,
                                    * found during end device bind */
      } bind_device_info[2];
      zb_uint8_t current_device; /*!< current device to send bind/unbind requests */
    }
    end_device_bind_ctx;
#endif  /* #ifndef ZB_LITE_NO_END_DEVICE_BIND */
  }
  zdo_ctx;                                  /*!< */

#ifdef ZB_TEST_PROFILE
  union zb_test_profile_ctx_u
  {
    struct zb_tp_counted_packets_s
    {
      zb_tp_transmit_counted_packets_param_t params; /*!< */
      zb_callback_t user_cb;                        /*!< */
      zb_uint16_t counter;                          /*!< */
    }
    tp_counted_packets;
    struct zb_tp_buffer_test_request_s
    {
      zb_callback_t user_cb;                        /*!< */
    }
    zb_tp_buffer_test_request;
  }
  test_prof_ctx;
#endif

#ifdef ZB_REJOIN_BACKOFF
  zb_zdo_rejoin_backoff_t zdo_rejoin_backoff;
#endif

  zb_device_annce_cb_t device_annce_cb;  /*!< This CB is called on receiving device_annce command */
  zb_callback_t leave_ind_cb;  /*!< Callback for NWK leave indication from child */
  zb_zdo_responce_cb_t zb_zdo_responce_cb;
  zb_zdo_set_channel_confirm_cb_t set_channel_confirm_cb;
  zb_apsme_update_device_ind_cb update_device_ind_cb  /*!< This callback will be called when application receives APSME_UPDATE_DEVICE */;
#ifdef ZB_MAC_DUTY_CYCLE_MONITORING
  zb_zdo_duty_cycle_mode_ind_cb_t duty_cycle_mode_ind_cb;  /*!< This callback will be called when MAC Duty Cycle mode changed */
#endif
#ifndef ZB_DISABLE_ASSERT_INDICATION
  zb_assert_indication_cb_t assert_indication_cb; /*!< This callback will be called when application falls into ZB_ASSERT() */
#endif

  zb_zdo_pim_t pim;

  zb_uint8_t nwk_upd_notify_pkt_limit;

#ifndef ZB_CONFIGURABLE_MEM
  zb_uint8_t nwk_addr_req_pending_tsns[ZB_N_BUF_IDS]; /* Address search feature: TSNs of nwk_addr_req pkt for
                                             * every buffer */
  zb_uint8_t nwk_addr_req_pending_mask[(ZB_N_BUF_IDS + 7U) / 8U];  /* Address search feature: mask of pending pkt for
                                             * every buffer */
#else
  /* Pointers are set in zb_init_configurable_mem() */
  zb_uint8_t *nwk_addr_req_pending_tsns;
  zb_uint8_t *nwk_addr_req_pending_mask;
#endif  /* ZB_CONFIGURABLE_MEM */

#ifdef APS_FRAGMENTATION
#ifndef ZB_CONFIGURABLE_MEM
  zb_uint8_t node_desc_req_pending_mask[(ZB_N_BUF_IDS + 7U) / 8U];  /* Node Desc feature: mask of pending pkt for
                                             * every buffer */
#else
  zb_uint8_t *node_desc_req_pending_mask;
#endif /* ZB_CONFIGURABLE_MEM */
#endif /* APS_FRAGMENTATION */

  zb_callback_t continue_start_after_nwk_cb;
  zb_callback_t nlme_reset_cb;

#ifdef ZB_FILTER_OUT_CLUSTERS
  zdo_cluster_filters_t cluster_filters;
#endif

#if defined ZB_JOINING_LIST_SUPPORT && defined ZB_ROUTER_ROLE
  zb_joining_list_ctx_t joining_list_ctx;
#endif /* defined ZB_JOINING_LIST_SUPPORT && defined ZB_ROUTER_ROLE */
  /* Part of WWAH context noved into SoC part of NCP / r23 All Hubs context */
  zb_zdo_hubs_context_t hubs_ctx;
  zb_uint8_t supported_key_neg_methods;           /*!< Supported Key Negotiation Methods.
                                                    *  See Table I.5.1-4-54
                                                    *  @see zb_tlv_key_negotiation_methods_t */
  zb_uint8_t supported_psk_secrets;
  zb_uint16_t supported_key_neg_methods_direct; /*!< Supported Key Neg Methods for ZBDirect */
  zb_uint8_t supported_psk_secrets_direct;
  /*!< Storage of the data for key negotiation procedure.
   *   For the TC some simultaneouse key negotiations are possible. */
#ifndef ZB_CONFIGURABLE_MEM
  zb_secur_ecdhe_common_ctx_t key_negotiation_ctx[ZB_ZDO_KEY_NEGOTIATIONS_NUM];
#else
  zb_secur_ecdhe_common_ctx_t *key_negotiation_ctx;
#endif

#if defined(ZDO_DIAGNOSTICS)
  zdo_diagnostics_ctx_t diagnostics_ctx;
#endif /* defined(ZDO_DIAGNOSTICS) */

  zb_zdo_func_selector_t selector;
#ifdef ZB_PHY_TESTING_MODE
  zb_phy_test_mode_notification_cb_t test_mode_notification_cb;
#endif
#if defined ZB_COORDINATOR_ROLE
  zb_zdo_channel_panid_change_ctx_t channel_panid_change_ctx;
#endif
} zb_zdo_globals_t;

#define ZDO_CTX() ZG->zdo

#define ZDO_SELECTOR() ZDO_CTX().selector

/* IN-core part of WWAH context / r23 All Hubs context */
#define HUBS_CTX() ZDO_CTX().hubs_ctx

#if defined ZB_COORDINATOR_ROLE
#define CHANNEL_PANID_CHANGE_CTX() ZDO_CTX().channel_panid_change_ctx
#endif

#if defined(ZDO_DIAGNOSTICS)
#define ZDO_DIAGNOSTICS_CTX() ZDO_CTX().diagnostics_ctx
#endif /* defined(ZDO_DIAGNOSTICS) */

#define ZB_ZDO_SET_ED_SCAN_BUSY()   (ZG->zdo.zdo_ed_scan_busy = 1U)
#define ZB_ZDO_CLEAR_ED_SCAN_BUSY() (ZG->zdo.zdo_ed_scan_busy = 0U)
#define ZB_ZDO_GET_ED_SCAN_BUSY() ZG->zdo.zdo_ed_scan_busy

/* Internally inside the stack use direct context access */
#ifndef ZB_JOINED
#define ZB_JOINED() ZB_TCPOL().node_is_on_a_network
#endif

#define ZB_ZDO_JOINER_KEY_NEGOTIATION_CTX() (*(ZG->zdo.key_negotiation_ctx))
#define ZB_SET_JOINED_STATUS(v) ZB_TCPOL().node_is_on_a_network = (v)

/* Converts a buffer with data into a packed signal with the data */
void zb_app_signal_pack_with_data(zb_uint8_t param, zb_uint32_t signal_code, zb_int16_t status);
/* Buffer is replaced with signal information, data_size bytes are reserved for additional signal data */
void *zb_app_signal_pack(zb_uint8_t param, zb_uint32_t signal_code, zb_int16_t status, zb_uint8_t data_size);

void *zb_app_signal_pack_with_detailed_status(zb_uint8_t param, zb_uint32_t signal_code,
                                              zb_ret_t status, zb_uint8_t data_size);
/* NWK channel manager */
void zb_zdo_check_channel_conditions(zb_uint8_t param);

#if defined ZB_JOINING_LIST_SUPPORT && defined ZB_ROUTER_ROLE
zb_bool_t zb_ieee_joining_list_schedule(zb_callback_t func, zb_uint8_t param);
void zb_ieee_joining_list_cb_completed(void);
void zb_ieee_joining_list_op_delay(void);
zb_bool_t zb_ieee_joining_list_put_cb(zb_callback_t func, zb_uint8_t param);
void zb_joining_list_reset_clear_timer(void);
#endif /* defined ZB_JOINING_LIST_SUPPORT && defined ZB_ROUTER_ROLE */

void zb_zdo_forced_parent_link_failure(zb_uint8_t param);

#ifdef SNCP_MODE
void zb_zdo_poll_parent_single(zb_uint8_t param, zb_callback_t cb);
#endif

/*! @endcond */

#endif /* ZB_ZDO_GLOBALS_H */
