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
/*  PURPOSE: Internal header for stack - NCP protocol interaction
*/
#ifndef ZB_NCP_INTERNAL_H
#define ZB_NCP_INTERNAL_H 1

/**
 * @name NCP Signals
 * @anchor ncp_signal
 *
 * Note: These values were members of `enum ncp_signal_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define NCP_SIGNAL_NWK_FORMATION_OK              0U
#define NCP_SIGNAL_NWK_FORMATION_FAILED          1U
#define NCP_SIGNAL_NWK_PERMIT_JOINING_COMPLETED  2U
#define NCP_SIGNAL_NWK_JOIN_FAILED               3U
#define NCP_SIGNAL_NWK_JOIN_AUTH_FAILED          4U
#define NCP_SIGNAL_NWK_JOIN_DONE                 5U
#define NCP_SIGNAL_NWK_REJOIN_FAILED             6U
#define NCP_SIGNAL_NWK_REJOIN_DONE               7U
#define NCP_SIGNAL_SECUR_TCLK_EXCHANGE_COMPLETED 8U
#define NCP_SIGNAL_SECUR_TCLK_EXCHANGE_FAILED    9U
#define NCP_SIGNAL_NLME_PARENT_LOST              10U
/** @} */

/**
 * @brief Type for NCP signals.
 *
 * Holds one of @ref ncp_signal. Kept only for backward compatibility as
 * @ref ncp_signal were declared previously as enum. Can be removed in future
 * releases.
 */
typedef zb_uint8_t ncp_signal_t;

#if defined NCP_MODE && !defined NCP_MODE_HOST
void ncp_signal(ncp_signal_t signal, zb_uint8_t param);
void ncp_signal_exec(ncp_signal_t signal, zb_uint8_t param);
#define NCP_CALL_COMPLETED(sig, param) ncp_signal(sig, param)

zb_bool_t ncp_catch_zcl_packet(zb_uint8_t param, zb_zcl_parsed_hdr_t *cmd_info, zb_uint8_t zcl_parse_status);
#define NCP_CATCH_ZCL_PACKET(param, cmd_info, status) ncp_catch_zcl_packet(param, cmd_info, status)

zb_bool_t ncp_catch_aps_data_conf(zb_uint8_t param);
#define NCP_CATCH_APS_DATA_CONF(param) ncp_catch_aps_data_conf(param)

zb_bool_t ncp_catch_nwk_disc_conf(zb_uint8_t param);
#define NCP_CATCH_NWK_DISC_CFM(param) ncp_catch_nwk_disc_conf(param)

#ifdef ZB_ENABLE_SE_MIN_CONFIG
void ncp_se_signal(zse_commissioning_signal_t signal, zb_uint8_t param);
void ncp_se_signal_exec(zse_commissioning_signal_t signal, zb_uint8_t param);

zb_bool_t ncp_partner_lk_failed(zb_uint8_t param);
#define NCP_CATCH_PARTNER_LK_FAILED(param) ncp_partner_lk_failed(param)

#endif /* ZB_ENABLE_SE_MIN_CONFIG */

void ncp_address_update_ind(zb_uint16_t short_address);
void ncp_apsme_remote_bind_unbind_ind(zb_uint8_t param, zb_bool_t bind);

#ifdef ZB_APSDE_REQ_ROUTING_FEATURES
void ncp_nwk_route_req_send_ind(zb_nwk_cmd_rreq_t *rreq);
void ncp_nwk_route_send_rrec_ind(zb_nwk_cmd_rrec_t *rrec);
void ncp_route_reply_ind(zb_nwk_cmd_rrep_t *rrep);
#endif /* ZB_APSDE_REQ_ROUTING_FEATURES*/

#else

#define NCP_CALL_COMPLETED(sig, param) if (param) { zb_buf_free(param); }
#define NCP_CATCH_ZCL_PACKET(param, cmd_info, status) ZB_FALSE
#define NCP_CATCH_APS_DATA_CONF(param) ZB_FALSE
#define NCP_CATCH_NWK_DISC_CFM(param) ZB_FALSE
#define NCP_CATCH_PARTNER_LK_FAILED(param) ZB_FALSE

#endif

#ifdef SNCP_MODE
void sncp_auto_turbo_poll_ed_timeout(zb_bool_t is_on);
void sncp_auto_turbo_poll_aps_rx(zb_bool_t is_on);
void sncp_auto_turbo_poll_aps_tx(zb_bool_t is_on);
void sncp_auto_turbo_poll_stop(void);
#endif

#endif /* ZB_NCP_INTERNAL_H */
