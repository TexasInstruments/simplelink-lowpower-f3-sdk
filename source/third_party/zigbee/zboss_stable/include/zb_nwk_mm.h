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
/*  PURPOSE: Header file for NWK MultiMAC
*/
#ifndef ZB_NWK_MM_H
#define ZB_NWK_MM_H 1

#define ZB_NWK_MAC_IFACE_SCAN_TYPE_ACTIVE     0U
#define ZB_NWK_MAC_IFACE_SCAN_TYPE_ENH_ACTIVE 1U

void zb_nwk_mm_mac_iface_table_init(void);

zb_uint8_t zb_nwk_mm_get_freq_band(void);

void zb_nlme_set_interface_request(zb_uint8_t param);

void zb_nlme_get_interface_request(zb_uint8_t param);

zb_uint8_t zb_multimac_get_iface_id_by_channel_page(zb_uint8_t page_idx, zb_uint8_t channel_idx);

/* Multi-MAC proxy functions */
void zb_nwk_handle_mlme_start_confirm(zb_uint8_t param);
void zb_nwk_handle_mlme_reset_confirm(zb_uint8_t param);
void zb_nwk_handle_mcps_data_confirm(zb_uint8_t param);

void zb_multimac_mlme_start_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_start_confirm_proxy(zb_uint8_t param);
void zb_multimac_mcps_data_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_set_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_reset_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_reset_confirm_proxy(zb_uint8_t param);
void zb_multimac_mlme_get_request_proxy(zb_uint8_t param);

void zb_multimac_mlme_scan_request_proxy(zb_uint8_t param);
void zb_multimac_mlme_scan_confirm_proxy(zb_uint8_t param);

void zb_multimac_mcps_data_confirm_proxy(zb_uint8_t param);

#ifdef ZB_MACSPLIT_HOST
void zb_mac_init_macsplit(void);
void zb_mac_deinit_macsplit(void);
#endif /* ZB_MACSPLIT_HOST */
void zb_multimac_mac_init(void);
void zb_multimac_mac_deinit(void);

#define ZB_MAC_INIT() zb_multimac_mac_init()

#define ZB_MAC_DEINIT() zb_multimac_mac_deinit()

/* MAC split interface API */
#ifdef ZB_MACSPLIT_HOST
void zb_mcps_data_request_macsplit(zb_uint8_t param);
void zb_mlme_get_request_macsplit(zb_uint8_t param);
void zb_mlme_set_request_macsplit(zb_uint8_t param);
void zb_mlme_reset_request_macsplit(zb_uint8_t param);
void zb_mlme_scan_request_macsplit(zb_uint8_t param);
void zb_mlme_associate_request_macsplit(zb_uint8_t param);
void zb_mlme_associate_response_macsplit(zb_uint8_t param);
void zb_mlme_poll_request_macsplit(zb_uint8_t param);
void zb_mlme_orphan_response_macsplit(zb_uint8_t param);
void zb_mlme_start_request_macsplit(zb_uint8_t param);
void zb_mac_resp_by_empty_frame_macsplit(zb_uint8_t param);

zb_ret_t zb_mac_logic_iteration_macsplit();
#endif /* ZB_MACSPLIT_HOST */

/**
 * @brief Synchronizes association parameters (MAC short address, coordinator extended address)
 * from primary interface with other interfaces
 *
 * @param param
 */
void zb_nwk_mm_sync_assoc(zb_uint8_t param);


/**
   Get the primary MAC interface.
*/
zb_uint8_t zb_get_primary_interface(void);

#endif /* ZB_NWK_MM_H */
