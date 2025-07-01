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
/* PURPOSE: SE global definitions
*/

#ifndef ZB_SE_H
#define ZB_SE_H 1

#include "se/zb_se_config.h"

#include "se/zb_se_sas.h"
#include "se/zb_se_kec.h"
#include "zb_nwk_neighbor.h"

#include "zcl/zb_zcl_subghz.h"

#include "se/zb_se_device_config.h"
//#include "se/zb_se_custom_attr.h"
//#include "se/zb_zcl_reporting_se_adds.h"


/* FIXME: remove that includes out of this file. This file is an internal header. */
#if 0
#include "se/zb_se_metering_device.h"
#include "se/zb_se_metering_load_control.h"
#include "se/zb_se_in_home_display.h"
#include "se/zb_se_pct_device.h"
#include "se/zb_se_energy_service_interface.h"
#endif

#if defined ZB_ENABLE_SE_MIN_CONFIG
#if 0
#define ZB_ZCL_DECLARE_SE_SIMPLE_DESC(                                  \
    ep_name, ep_id, in_clust_num, out_clust_num, dev_id)                \
   ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num)  simple_desc_##ep_name = \
   {                                                                    \
     ep_id,                                                             \
     ZB_AF_SE_PROFILE_ID,                                               \
     dev_id,                                                            \
     ZB_SE_VER,                                                         \
     0,                                                                 \
     in_clust_num,                                                      \
     out_clust_num,                                                     \
     {                                                                  \
       ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT,                             \
       ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT                              \
     }                                                                  \
   }


#define ZB_ZCL_DECLARE_SE_EP(ep_name, ep_id, cluster_list)              \
  ZB_ZCL_DECLARE_SE_SIMPLE_DESC(                                        \
          ep_name,                                                      \
          ep_id,                                                        \
          1,                                                            \
          1,                                                            \
          0/*ZGP_DEVICE_COMBO_BASIC*/);                                 \
        ZB_AF_START_DECLARE_ENDPOINT_LIST(ep_name)                      \
        ZB_AF_SET_ENDPOINT_DESC(                                        \
          ep_id,                                                        \
          ZB_AF_SE_PROFILE_ID,                                          \
          0,                                                            \
          NULL,                                                         \
          ZB_ZCL_ARRAY_SIZE(                                            \
            cluster_list,                                               \
            zb_zcl_cluster_desc_t),                                     \
          cluster_list,                                                 \
          (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name)             \
        ZB_AF_FINISH_DECLARE_ENDPOINT_LIST


#define ZB_ZCL_DECLARE_SE_CTX(device_ctx, ep_name)                      \
  ZBOSS_DECLARE_DEVICE_CTX(device_ctx, ep_name,                         \
                           ZB_ZCL_ARRAY_SIZE(ep_name, zb_af_endpoint_desc_t), \
                           0, NULL, /* No reporting ctx */              \
                           0, NULL) /* No CVC ctx */
#endif

//#if defined ZB_ENABLE_SE && defined ZB_SE_ENABLE_KEC_CLUSTER
//#if defined ZB_USE_NVRAM || defined DOXYGEN
/**
   CERT structure for NVRAM
 */
typedef ZB_PACKED_PRE struct zse_cert_nvram_s
{
  zb_uint8_t      suite_no;
  zb_kec_qe_t     ca;
  zb_kec_pr_t     pr;
  zb_kec_icu_t    cert;
//  zb_uint8_t      align[2];
} ZB_PACKED_STRUCT zse_cert_nvram_t;

zse_cert_nvram_t * zse_certdb_get_from_tc_storage(zb_uint8_t suite_no, zb_uint8_t *issuer, zse_cert_nvram_t * buf);
/**
   Certificates hash for TC

   Do not store installcode itself in RAM - keep only 4 bytes per entry in RAM.
 */
typedef ZB_PACKED_PRE struct zse_cert_storage_s
{
  zb_uint32_t    nvram_offset:28;      /*!< offset of zb_aps_installcode_nvram_t
                                          record in nvram. 28 bit supports 256k
                                          page - hope, it is enough  */
  zb_bitfield_t  nvram_page:4;         /*!< nvram page. Hope 16 pages are enough  */
} ZB_PACKED_STRUCT zse_cert_storage_t;

typedef struct zse_cert_add_s
{
  zse_cert_nvram_t full_cert;
  zb_uint8_t*      issuer;
  zb_uint8_t*      subject;
  zb_uint8_t       suite_no;
  zb_uint8_t       do_update;
} zse_cert_add_t;

/**
 * @name NVRAM dataset certificate database version
 * @anchor nvram_dataset_certdb_versions
 *
 * Note: These values were members of `enum zb_nvram_dataset_certdb_versions_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_NVRAM_SE_CERTDB_DS_VER_1 0U
/** @} */

#define ZB_NVRAM_SE_CERTDB_DS_VER ZB_NVRAM_SE_CERTDB_DS_VER_1

//#endif
//#endif / * defined ZB_USE_NVRAM || defined DOXYGEN * /

typedef enum zse_startup_control_e
{
  /* For us much more convenient if uncommissioned == initialized by zero at startup time */
  ZSE_STARTUP_UNCOMMISSIONED,   /* 2 in SE specification */
  ZSE_STARTUP_COMMISSIONED
} zse_startup_control_t;


#if defined(ZB_SE_ENABLE_SERVICE_DISCOVERY_PROCESSING)

typedef enum zse_service_disc_step_s
{
  ZSE_SERVICE_DISC_DISCOVER_IEEE = 0,
#define ZSE_SERVICE_DISC_FIRST_STEP ZSE_SERVICE_DISC_DISCOVER_IEEE
  ZSE_SERVICE_DISC_GEN_KEY,
#ifdef ZB_ENABLE_TIME_SYNC
  ZSE_SERVICE_DISC_GET_TIME_SERVER,  /* Determine the most authoritative Time server */
#endif
  ZSE_SERVICE_DISC_READ_COMMODITY,
  ZSE_SERVICE_DISC_BIND,
  ZSE_SERVICE_DISC_BOUND_OK,
  ZSE_SERVICE_DISC_INVALID = 0xFF,     /* entry is free */
} zse_service_disc_step_t;

typedef struct zse_service_disc_dev_s
{
/* 07/27/2017 EE CR:MINOR Use addr ref? */
  zb_uint16_t short_addr;
  zb_uint8_t ep;
  zse_service_disc_step_t step;
  zb_uint16_t cluster_id;
  zb_uint8_t op_in_progress;
  zb_uint8_t commodity_type;
  zb_uint8_t zcl_tsn;
} zse_service_disc_dev_t;

#define ZSE_SERVICE_DISCOVERY_MAX_DEVICES 10

typedef struct zse_service_disc_data_s
{
  zb_uint8_t current_cluster;
  zb_uint8_t source_ep;
  zse_service_disc_dev_t devices[ZSE_SERVICE_DISCOVERY_MAX_DEVICES]; /* TODO: how much? */
  zb_uint8_t match_desc_tsn;
  //zb_uint8_t multiple_commodity_enabled;
  zb_uint8_t mode;
} zse_service_disc_data_t;

#define ZSE_SERVICE_DISCOVERY_GET_MULTIPLE_COMMODITY() ZB_CHECK_BIT_IN_BIT_VECTOR(&ZSE_CTXC().service_disc.mode, 0U)
#define ZSE_SERVICE_DISCOVERY_SET_MULTIPLE_COMMODITY() ZB_SET_BIT_IN_BIT_VECTOR(&ZSE_CTXC().service_disc.mode, 0U)
#define ZSE_SERVICE_DISCOVERY_CLR_MULTIPLE_COMMODITY() ZB_CLR_BIT_IN_BIT_VECTOR(&ZSE_CTXC().service_disc.mode, 0U)

#define ZSE_SERVICE_DISCOVERY_GET_MDU_PAIRING_MODE() ZB_CHECK_BIT_IN_BIT_VECTOR(&ZSE_CTXC().service_disc.mode, 1U)
#define ZSE_SERVICE_DISCOVERY_SET_MDU_PAIRING_MODE() ZB_SET_BIT_IN_BIT_VECTOR(&ZSE_CTXC().service_disc.mode, 1U)
#define ZSE_SERVICE_DISCOVERY_CLR_MDU_PAIRING_MODE() ZB_CLR_BIT_IN_BIT_VECTOR(&ZSE_CTXC().service_disc.mode, 1U)

#endif


#if defined(ZB_SE_ENABLE_STEADY_STATE_PROCESSING)

typedef enum zse_steady_state_tc_poll_method_e
{
  ZSE_TC_POLL_READ_KEEPALIVE = 0,
  ZSE_TC_POLL_READ_METERING_CONSUMPTION,
  ZSE_TC_POLL_READ_PRICING_INFO,
  ZSE_TC_POLL_NOT_SUPPORTED,
} zse_steady_state_tc_poll_method_t;

typedef struct zse_steady_state_data_s
{
  zse_steady_state_tc_poll_method_t poll_method;
  zb_uint8_t tsn;
  zb_uint8_t endpoint;
  zb_uint8_t failure_cnt;
  zb_uint32_t countdown;
  zb_uint8_t keepalive_base;
  zb_uint16_t keepalive_jitter;
} zse_steady_state_data_t;

#endif


/**
   SE states as defined in the specification
 */
typedef enum zse_commissioning_state_e
{
  SE_STATE_FAILED,
  SE_STATE_FORMATION,
  SE_STATE_PRECOMMISSIONED_REJOIN,
  SE_STATE_AUTO_JOIN,
  SE_STATE_SERVICE_DISCOVERY_MDU,
  SE_STATE_SERVICE_DISCOVERY,
  SE_STATE_TC_STEADY,
  SE_STATE_DEVICE_STEADY,
  SE_STATE_REJOIN_RECOVERY,
  SE_STATE_JOINED_NODEV,
} zse_commissioning_state_t;


/**
 * @name ZSE commissioning signal
 * @anchor commissioning_signal
 *
 * Note: These values were members of `enum zse_commissioning_signal_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define SE_COMM_SIGNAL_BAD                          0U
#define SE_COMM_SIGNAL_START_AUTO_JOIN              1U
#define SE_COMM_SIGNAL_START_REJOIN                 2U
#define SE_COMM_SIGNAL_NWK_DISCOVERY_FAILED         3U
#define SE_COMM_SIGNAL_NWK_JOIN_FAILED              4U
#define SE_COMM_SIGNAL_NWK_AUTH_OK                  5U
#define SE_COMM_SIGNAL_NWK_AUTH_FAILED              6U
#define SE_COMM_SIGNAL_NWK_LEAVE                    7U

#define SE_COMM_SIGNAL_ENTER_REJOIN_RECOVERY        8U
#define SE_COMM_SIGNAL_START_REJOIN_RECOVERY        9U
#define SE_COMM_SIGNAL_REJOIN_RECOVERY_TRY_REJOIN   10U
#define SE_COMM_SIGNAL_REJOIN_RECOVERY_OK           11U

#define SE_COMM_SIGNAL_START_CBKE                   12U
#define SE_COMM_SIGNAL_RETRY_CBKE                   13U
#define SE_COMM_SIGNAL_CBKE_OK                      14U
#define SE_COMM_SIGNAL_CBKE_FAILED                  15U

#define SE_COMM_SIGNAL_SERVICE_DISCOVERY_OK         16U
#define SE_COMM_SIGNAL_SERVICE_DISCOVERY_FAILED     17U

#define SE_COMM_SIGNAL_STEADY_START_TC_POLL         18U
#define SE_COMM_SIGNAL_STEADY_TC_POLL_STARTED       19U
#define SE_COMM_SIGNAL_STEADY_TC_POLL_FAILED        20U
#define SE_COMM_SIGNAL_STEADY_TC_POLL_NOT_SUPPORTED 21U

#define SE_COMM_SIGNAL_NWK_FORMATION_OK             22U
#define SE_COMM_SIGNAL_NWK_START_ROUTER_CONF        23U
#define SE_COMM_SIGNAL_NWK_FORMATION_FAILED         24U
#define SE_COMM_SIGNAL_NWK_AUTHENTICATE_REMOTE      25U
#define SE_COMM_SIGNAL_CHILD_SECURED_REJOIN         26U
#define SE_COMM_SIGNAL_JOINED_DEVICE_CBKE_FAILED    27U
#define SE_COMM_SIGNAL_JOINED_DEVICE_CBKE_TIMED_OUT 28U
#define SE_COMM_SIGNAL_JOINED_DEVICE_CBKE_OK        29U
#define SE_COMM_SIGNAL_JOINED_DEVICE_NON_CBKE_OK    30U

#define SE_COMM_SIGNAL_REJOIN_RECOVERY_INTERNAL     31U
#define SE_COMM_SIGNAL_ENTER_REJOIN_ONLY            32U
#define SE_COMM_SIGNAL_START_REJOIN_ONLY            33U

#define SE_COMM_N_SIGNALS                           34U
/** @} */

/**
 * @brief Type for ZSE commissioning signals.
 *
 * Holds one of @ref commissioning_signal. Kept only for backward compatibility as
 * @ref commissioning_signal were declared previously as enum. Can be removed in future
 * releases.
 */
typedef zb_uint8_t zse_commissioning_signal_t;


typedef enum se_rr_state_e
{
  SE_REJOIN_RECOVERY_IDLE,
  SE_REJOIN_RECOVERY_SECURE_REJOIN_CUR,
  SE_REJOIN_RECOVERY_TC_REJOIN_CUR,
  SE_REJOIN_RECOVERY_REJOINS_RETRY
} se_rr_state_t;

typedef struct zse_commissioning_ctx_s
{
  zse_commissioning_state_t state;
  zse_commissioning_signal_t signal;
  zb_time_t auto_join_start_time;
  zb_uint8_t cbke_retries;
  zb_uint8_t auto_join;          /*!< is a global setting meaning: fall into SE_STATE_AUTO_JOIN state at failure.  */
  zb_uint8_t startup_control;    /*!< @see zse_startup_control_e */
  zb_uint8_t formation_retries;
  zb_uint32_t permit_join_rest;
  zb_kec_term_cmd_t ke_term_info;

  zb_uint8_t rr_sv_device_type; /* @see nwk_device_type */
  zb_uint16_t rr_sv_parent_short;
  zb_ieee_addr_t rr_sv_parent_long;
  zb_neighbor_tbl_ent_t rr_sv_parent_nent;
  zb_uint8_t rr_sv_authenticated;
  zb_uint8_t rr_retries;
  zb_uint8_t rr_ignore_start_router_conf;
  zb_uint8_t just_booted;
  zb_uint_t rr_global_retries;
  zb_uint8_t rr_skip_savepoint;
#ifdef ZB_SE_BDB_MIXED
  zb_bool_t allow_bdb_in_se_mode;
  void (*switch_to_bdb_commissioning)(zb_bdb_commissioning_mode_mask_t);
#endif
} zse_commissioning_ctx_t;

#define ZB_ZCL_MDU_PAIRING_HAN_TABLE_SIZE 10U

typedef struct zse_mdu_pairing_ctx_s
{
  zb_addr_u mdu_parent;
  //zb_ieee_addr_t mdu_parent_long;
  zb_uint8_t mdu_parent_ep;
  zb_uint8_t virtual_han_size;
  zb_ieee_addr_t virtual_han_table[ZB_ZCL_MDU_PAIRING_HAN_TABLE_SIZE];
  zb_uint16_t virtual_han_table_short[ZB_ZCL_MDU_PAIRING_HAN_TABLE_SIZE];
  zb_uint8_t short_address_scan_pos;
  zb_uint16_t cluster_id;
} zse_mdu_pairing_ctx_t;


typedef struct zse_time_server_ctx_s
{
  zb_uint32_t time;  /* gotten time from Time server */
  zb_uint8_t server_ep;
  zb_uint16_t server_short_addr;
  zb_uint8_t server_auth_level;  /* Authoritative time server level */
  zb_time_t receiving_time;  /* Used for network time correction */
} zse_time_server_ctx_t;

/** @brief Server-side Sub-GHz cluster context
 */
typedef struct zb_subghz_srv_ctx_s
{
#if !defined ZB_CONFIGURABLE_MEM
  /* FIXME: rewrite to exclude additional memory usage. */
  zb_address_ieee_ref_t dev_list[ZB_NEIGHBOR_TABLE_SIZE];   /**< ref list to Sub-GHz devices in neighbor table */
#else
  zb_address_ieee_ref_t *dev_list;
#endif
  zb_uint8_t mode;                               /**< MAC Duty Cycle Mode */
} zb_subghz_srv_ctx_t;

/** @brief Client-side Sub-GHz cluster context
 */
typedef struct zb_subghz_cli_ctx_s
{
  zb_uint8_t zc_ep;                   /**< endpoint with Sub-GHz cluster on Coordinator */
  zb_bool_t suspend_zcl_messages;     /**< if TRUE, device is suspended by Coordinator */
} zb_subghz_cli_ctx_t;

typedef struct zse_hifreq_s
{
  zb_uint16_t clusterid;
  zb_time_t timestamp;
} zse_hifreq_t;

/** @brief Sub-GHz mode context
 */
typedef struct zb_subghz_ctx_s
{
#ifndef ZB_ED_ROLE
  zb_subghz_srv_ctx_t srv;      /**< server context */
#endif
  zb_subghz_cli_ctx_t cli;      /**< client context */

  /* common fields */
  zb_uint8_t ep;               /**< endpoint of Sub-GHz cluster on this device */
  zse_hifreq_t hifreq[ZB_SE_HI_FREQ_MSG_N_SLOTS];
} zb_subghz_ctx_t;

typedef enum zse_ke_whitelist_op_e {
  ZSE_KE_WHITELIST_ADD,
  ZSE_KE_WHITELIST_DEL,
  ZSE_KE_WHITELIST_DEL_ALL,
  ZSE_KE_WHITELIST_NONE
} zse_whitelist_op_t;

typedef struct zse_ke_whitelist_s {
  /* current page/position of page in nvram */
  zb_uint8_t page;
  zb_uint32_t pos;
  zb_uint16_t num_entries;

  /* Outstanding operation to perform */
  zb_ieee_addr_t addr;
  zse_whitelist_op_t op;
} zse_ke_whitelist_t;

/** @brief ZSE context data */
typedef struct zb_zse_ctx_s
{
  zb_ke_ctx_t ke;  //Key-Establishment context of current key establishment process
#if defined ZB_SUBGHZ_BAND_ENABLED
  zb_subghz_ctx_t subghz;         /* Sub-GHz context of current device */
#endif /* ZB_SUBGHZ_BAND_ENABLED */
#ifdef ZB_ZCL_SUPPORT_CLUSTER_MDU_PAIRING
  zse_mdu_pairing_ctx_t mdu_pairing;
#endif  /* ZB_ZCL_SUPPORT_CLUSTER_MDU_PAIRING */
#ifdef ZB_ZCL_SUPPORT_CLUSTER_TIME
  zse_time_server_ctx_t time_server;
  zse_time_server_ctx_t second_time_server;
#endif  /* ZB_ZCL_SUPPORT_CLUSTER_TIME */
  zse_cert_storage_t certdb[ZB_ZSE_CERTDB_TBL_SIZE];
  zse_cert_add_t *cert_to_add;
#ifdef ZB_SE_KE_WHITELIST
  zse_ke_whitelist_t ke_whitelist;
#endif /* ZB_SE_KE_WHITELIST */
  zb_uint16_t supported_key_suite;
#if defined(ZB_SE_ENABLE_SERVICE_DISCOVERY_PROCESSING)
  zse_service_disc_data_t service_disc;
#endif
#if defined(ZB_SE_ENABLE_STEADY_STATE_PROCESSING)
  zse_steady_state_data_t steady_state;
#endif
  zse_commissioning_ctx_t commissioning;
} zb_zse_ctx_t;

extern zb_zse_ctx_t g_zse_ctx;

void zb_zse_init(void);

/**
   Direct access to SE context.

   To be used internally only.
 */
#define ZSE_CTXC() (g_zse_ctx)

/* Internal SE commissioning routines and definitions */
#ifdef ZB_ENABLE_SE_MIN_CONFIG
void se_commissioning_signal(zse_commissioning_signal_t sig, zb_uint8_t param);
#endif /* ZB_ENABLE_SE_MIN_CONFIG */

#ifdef ZB_SE_COMMISSIONING
void se_initiate_commissioning(zb_uint8_t param);
void se_commissioning_machine(zb_uint8_t param);
void se_commissioning_force_link(void);
void se_formation_force_link(void);
#endif /* ZB_SE_COMMISSIONING */

#if defined(ZB_SE_ENABLE_SERVICE_DISCOVERY_PROCESSING)

zb_bool_t zb_se_service_discovery_match_desc_resp_handle(zb_uint8_t param);
zb_bool_t zb_se_service_discovery_read_commodity_attr_handle(zb_uint8_t param);
void zb_se_service_discovery_reset_dev();
void zb_se_service_discovery_send_start_signal_delayed(zb_uint8_t param);
void zb_se_service_discovery_send_start_signal(zb_uint8_t param);
zb_bool_t zb_se_service_discovery_block_zcl_cmd(zb_uint16_t short_addr, zb_uint8_t ep, zb_uint8_t zcl_tsn);

#ifdef ZB_ENABLE_TIME_SYNC
zb_bool_t zb_se_service_discovery_read_time_attrs_handle(zb_uint8_t param);
#endif

#endif

#ifdef ZB_SE_COMMISSIONING
#ifdef ZB_COORDINATOR_ROLE
void se_pref_channels_create_mask(zb_bool_t first_time, zb_channel_list_t list);
#endif /* ZB_COORDINATOR_ROLE */
#endif /* ZB_SE_COMMISSIONING */

/* EVS: renamed due to MISRA violations related to name clash */
zb_uint8_t zb_zdo_init_ieee_addr_req_with_cb(zb_uint8_t param, zb_uint16_t nwk_addr, zb_callback_t cb);


#if defined(ZB_SE_ENABLE_STEADY_STATE_PROCESSING)

zb_bool_t zb_se_steady_state_match_desc_resp_handle(zb_uint8_t param);
void zb_se_steady_state_start_periodic_tc_poll();
void zb_se_steady_state_stop_periodic_tc_poll();


#if defined(ZB_ZCL_SUPPORT_CLUSTER_PRICE)

zb_ret_t zb_se_steady_publish_price_handle(zb_uint8_t param);

#endif

zb_bool_t zb_se_steady_state_block_zcl_cmd(zb_uint16_t short_addr, zb_uint8_t ep, zb_uint8_t zcl_tsn);

zb_bool_t zb_se_steady_state_read_attr_handle(zb_uint8_t param);

#endif

#ifdef ZB_SE_COMMISSIONING
zb_bool_t se_is_high_freq_msg(zb_uint8_t param);
zb_bool_t se_cbke_exchange_schedule_alarm(zb_uint8_t param, zb_callback_t alarm);
#ifdef ZB_COORDINATOR_ROLE
void se_comm_device_left(zb_uint8_t addr_ref);
#endif /* ZB_COORDINATOR_ROLE */
#endif /* ZB_SE_COMMISSIONING */

void se_send_key_est_match_desc(zb_uint8_t param);
void se_send_key_est_match_desc_addr(zb_uint8_t param, zb_uint16_t addr_of_interest);

zb_uint8_t se_get_ke_status_code(void);

#if defined ZB_SE_COMMISSIONING && defined ZB_COORDINATOR_ROLE
void se_minimal_tc_init(void);
#endif /* ZB_SE_COMMISSIONING && ZB_COORDINATOR_ROLE */

#ifdef ZSE_TEST_HACKS
#define ZSE_HACKS_SKIP_KEC
#endif

void zse_certdb_init(void);
zb_ret_t zse_certdb_set(zb_uint8_t *ic);

#endif /* ZB_ENABLE_SE_MIN_CONFIG */

void se_handle_link_key_indication(zb_bufid_t param);

#if defined ZB_SE_COMMISSIONING || defined SNCP_MODE
void se_new_binding_handler(zb_uint16_t bind_tbl_idx);
#endif /* ZB_SE_COMMISSIONING || SNCP_MODE */

void zb_kec_cluster_nvram_init(void);

#endif /* ZB_SE_H */
