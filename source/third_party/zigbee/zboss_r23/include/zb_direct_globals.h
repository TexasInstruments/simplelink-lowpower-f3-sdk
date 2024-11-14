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
/* PURPOSE: Zigbee Direct subsystem globals.
*/

#ifndef ZB_DIRECT_GLOBALS_H
#define ZB_DIRECT_GLOBALS_H 1

#include "zb_direct_common.h"

/*! @cond internals_doc */
/*! \addtogroup ZB_DIRECT */
/*! @{ */

typedef enum zb_zbd_capability_e
{
  ZB_ZBD_CAPABILITY_NONE,
  /*ZB_COMMON_BLE_MAC_CAPABILITY_SUPPORT,*/
  ZB_ZBD_CAPABILITY_ZVD,
  ZB_ZBD_CAPABILITY_ZDD
} zb_zbd_capability_t;

#ifdef ZB_DIRECT_ENABLED
/* Some callbacks are defined below. General purpose is to allow ZBOSS core to interact with
Zigbee Direct.

Because Zigbee Direct is an optional library and may or may not be linked to the core, a
dedicated Zigbee Direct context has been introduced. The context is included into global context and
contains pointers to necessary functions. If Zigbee Direct is used it registers itself and fills
the context with proper pointers letting the stack know that Zigbee Direct is available.

*/
typedef zb_bool_t (*zbd_is_network_zbd_aware_func_t)(void);
typedef zb_bool_t (*zbd_process_tunneled_frame_if_needed_func_t)(zb_address_ieee_ref_t ref, zb_uint8_t param);
typedef zb_bool_t (*zbd_is_frame_from_zvd_allowed_func_t)(zb_uint8_t);
typedef zb_bool_t (*zbd_is_neighbor_zvd_func_t)(const zb_neighbor_tbl_ent_t*);
typedef zb_bool_t (*zbd_is_zbd_provisioned_func_t)(void);

typedef zb_bool_t (*zbd_disconnect_from_all_peers_t)(void);
typedef zb_bool_t (*zbd_zdd_is_enhanced_provisioning_session_active_t)(zb_address_ieee_ref_t ref);

typedef zb_ret_t (*zbd_zvd_calc_destination_t)(zb_uint16_t, zb_uint16_t*, zb_uint8_t*);

typedef struct zb_direct_context_s
{
  zb_uint8_t iface_id;
  zb_uint16_t device_capability_extension_value;
  zb_zbd_capability_t zbd_capability;
  zbd_process_tunneled_frame_if_needed_func_t process_tunneled_frame_if_needed;
  zbd_is_frame_from_zvd_allowed_func_t is_frame_from_zvd_allowed;
  zbd_is_neighbor_zvd_func_t is_neighbor_zvd;
  zbd_is_zbd_provisioned_func_t is_zbd_provisioned;

  zb_callback_t zvd_apsme_transport_key_indication;
  zbd_disconnect_from_all_peers_t disconnect_from_all_peers;
  zb_bool_t is_ephemeral_session_started;
  zb_ieee_addr_t comm_status_tc_address;

  zb_callback_t zbd_zdd_forget_zvd;

  zbd_zdd_is_enhanced_provisioning_session_active_t zdd_is_enhanced_session_active;

  zb_callback_t establish_trusted_link_continue;

  /**
   * @brief Handles Network Commissioning Response from ZDD to ZVD TC (on ZDD side)
   */
  zb_callback2_t trusted_link_process_sent_commissioning_resp;

  /**
   * @brief Calculates NWK address and MAC interface of destination hop
   * to send packets from ZVD TC
   */
  zbd_zvd_calc_destination_t zvd_calc_destination;

  zb_bool_t is_trusted_link_initiator;
  zb_bool_t is_zvd_routing_allowed;
} zb_direct_context_t;

#endif /* ZB_DIRECT_ENABLED */

/*! @} */
/*! @endcond */

#endif /* ZB_DIRECT_GLOBALS_H */
