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

#if defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_SERVER

zb_bool_t zb_wwah_check_nwk_key_commands_broadcast_allowed(void);
void zb_wwah_set_nwk_key_commands_broadcast_allowed(zb_bool_t allowed);

zb_bool_t zb_wwah_in_configuration_mode(void);
void zb_wwah_set_configuration_mode(zb_bool_t allowed);
zb_bool_t zb_wwah_check_zdo_command(zb_apsde_data_indication_t *di);

zb_bool_t zb_wwah_check_if_leave_without_rejoin_allowed(void);
void zb_wwah_set_leave_without_rejoin_allowed(zb_bool_t allowed);

zb_bool_t zb_wwah_check_if_interpan_supported(void);
void zb_wwah_set_interpan_supported(zb_bool_t enabled);

#endif /* ZB_ZCL_SUPPORT_CLUSTER_WWAH && ZB_ZCL_ENABLE_WWAH_SERVER */

#endif  /* ZDO_HUBS_H */
