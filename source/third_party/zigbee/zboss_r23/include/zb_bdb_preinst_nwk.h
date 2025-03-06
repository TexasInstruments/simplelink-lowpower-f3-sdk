/* ZBOSS Zigbee software protocol stack
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
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
/*  PURPOSE: NOT standard feature: allow factory new devices join using pre-installed network parameters
*/

#ifndef ZB_BDB_PREINST_NWK_COMMISSIONING_H
#define ZB_BDB_PREINST_NWK_COMMISSIONING_H 1

#ifdef ZB_BDB_PREINST_NWK_JOINING

#ifdef ZB_JOIN_CLIENT
/**
 * @brief Set pre-installed extended PAN ID. Mandatory for pre-installed network.
 *
 * @param panid - extended PAN ID to be set for joining pre-installed network.
 */
void zb_bdb_preinst_nwk_set_extended_pan_id(const zb_ext_pan_id_t panid);

/**
 * @brief Set pre-installed network key. Mandatory for pre-installed network.
 *
 * @param nwk_key - network key to be set after joining pre-installed network.
 * @param key_seq_num - sequence number of network key.
 */
void zb_bdb_preinst_nwk_set_nwk_key(const zb_uint8_t nwk_key[ZB_CCM_KEY_SIZE], zb_uint32_t key_seq_num);

/**
 * @brief Set pre-installed TC long address. Optional for pre-installed network.
 *
 * Allows to skip requesting long address of ZC on join.
 *
 * @param nwk_key - network key to be set after joining pre-installed network.
 * @param key_seq_num - sequence number of network key.
 */
void zb_bdb_preinst_nwk_set_tc_long_address(const zb_ieee_addr_t tc_ieee);

/**
 * @brief Set pre-installed TCLK. Optional for pre-installed network.
 *
 * The very same key must be pre-installed to ZC providing IEEE address of this joiner
 * @see zb_bdb_preinst_nwk_set_joiner_tclk
 *
 * @param tclk - TCLK to be set after joining pre-installed network.
 */
void zb_bdb_preinst_nwk_set_tclk(const zb_uint8_t tclk[ZB_CCM_KEY_SIZE]);
#endif /* ZB_JOIN_CLIENT */

#ifdef ZB_COORDINATOR_ROLE

/**
 * @brief Set pre-installed joiner's TCLK. For testing purposes only.
 *
 * @param joiner_address - long address of a joiner device.
 * @param tclk - TCLK of the joiner.
 */
void zb_bdb_preinst_nwk_set_joiner_tclk(const zb_ieee_addr_t joiner_address, const zb_uint8_t tclk[ZB_CCM_KEY_SIZE]);

#endif /* ZB_COORDINATOR_ROLE */

/*****************************************************************************/
#endif /* ZB_BDB_PREINST_NWK_JOINING */
#endif /* ZB_BDB_PREINST_NWK_COMMISSIONING_H */

