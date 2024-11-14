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
/* PURPOSE: Routines to support WWAH moved there from ZCL, then All Hubs in r23
*/

#ifndef ZDO_HUBS_H
#define ZDO_HUBS_H

zb_bool_t zb_is_wwah_server(void);

zb_bool_t zb_wwah_check_nwk_upd_bcast_allowed(void);
zb_bool_t zb_wwah_check_require_lk_encryption(void);
void zb_wwah_set_require_lk_encryption(zb_bool_t require);

zb_bool_t zb_wwah_in_configuration_mode(void);
void zb_wwah_set_configuration_mode(zb_bool_t allowed);
zb_ret_t zb_wwah_check_zdo_command(zb_apsde_data_indication_t *di);
void zb_zcl_wwah_update_tc_connection(zb_uint16_t short_addr);


zb_bool_t zb_wwah_check_if_leave_without_rejoin_allowed(void);
void zb_wwah_set_leave_without_rejoin_allowed(zb_bool_t allowed);

zb_bool_t zb_wwah_check_if_interpan_supported(void);
void zb_wwah_set_interpan_supported(zb_bool_t enabled);

#ifdef ZB_CERTIFICATION_HACKS
void zb_wwah_overwrite_hub_connectivity(zb_bool_t overwrite, zb_bool_t val);
#endif

void zb_wwah_set_pending_channel(zb_uint32_t channel_mask);

void zb_wwah_set_pending_panid(zb_uint16_t next_pan_id);

void zdo_schedule_set_long_uptime(zb_uint8_t param);
void zdo_schedule_cancel_long_uptime(zb_uint8_t param);

#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
zb_ret_t zb_zcl_wwah_set_pending_channel(zb_uint32_t channel_mask);
zb_ret_t zb_zcl_wwah_set_pending_panid(zb_uint16_t next_pan_id);
zb_ret_t zb_zcl_wwah_set_leave_without_rejoin(zb_bool_t allowed);
zb_ret_t zb_zcl_wwah_set_configuration_mode(zb_bool_t allowed);
zb_ret_t zb_zcl_wwah_set_require_lk_encryption(zb_bool_t require);
#endif

#endif  /* ZDO_HUBS_H */
