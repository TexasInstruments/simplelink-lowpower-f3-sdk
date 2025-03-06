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
/* PURPOSE: features for diagnostic ZBOSS core and apps
*/

#ifndef ZB_DIAG_H
#define ZB_DIAG_H 1

#include "zboss_api.h"

/**
 * Empty init-function
 *
 * @see zb_diag_init
 */
void zb_diag_key_exposure_init(void);

/**
 * Broadcast a key using non-standard NWK report (command id 6)
 *
 * To be used for debug purposes only.
 *
 * @param peer_addr - address of remote device, or NULL
 * @param key - APS or NWK key
 */
void zb_diag_bcast_key(zb_uint8_t *peer_addr, zb_uint8_t key[ZB_CCM_KEY_SIZE]);

/**
   Broadcast TCLK or other APS key. For debugging purposes only.

   That function broadcasts APS key stored in keypair for that long_addr. Wireshark understands that broadcast, so it can decode the traffic.
   That function has pure debug purposes. It must be explicitly called by the application.
   The function works in debug ZBOSS builds only.
   The function does nothing in release ZBOSS builds.

   @param long_addr - address of the remote device
   @return RET_OK if broadcast done ok, RET_NOT_FOUND if no such keypair, RET_ERROR if that function is disabled in that ZBOSS build

   @snippet r23_new_api/r23_zc.c debug_broadcast_key_snippet
 */
zb_ret_t zb_diag_broadcast_aps_key(zb_ieee_addr_t long_addr);

/**
   Broadcast NWK key.

   FOR DEBUGGING PURPOSES ONLY. It zeroes your network security. Never use it in productuion!

   That function broadcasts the current NWK key. Wireshark understands that broadcast, so it can decode the traffic.
   That function has pure debug purposes. It must be explicitly called by the application.

   The function works in debug ZBOSS builds only.
   The function does nothing in release ZBOSS builds.

   @return RET_OK if broadcast done ok, RET_ERROR if that function is disabled in that ZBOSS build

 */
zb_ret_t zb_diag_broadcast_nwk_key(void);

/**
 * Broadcast a key using non-standard NWK report (command id 6)
 *
 * The function was deprecated, now it's a wrapper for @see zb_diag_bcast_key
 *
 * @param peer_addr - address of remote device, or NULL
 * @param key - APS or NWK key
 */
#define zb_debug_bcast_key zb_diag_bcast_key

/**
 * @depreceted it's a wrapper for @see zb_diag_broadcast_aps_key()
 */
#define zb_debug_broadcast_aps_key zb_diag_broadcast_aps_key

/**
 * @depreceted it's a wrapper for @see zb_diag_broadcast_nwk_key()
 */
#define zb_debug_broadcast_nwk_key zb_diag_broadcast_nwk_key

/**
 * Empty init-function
 *
 * @see zb_diag_init
 */
void zb_diag_core_watchdog_init(void);

/**
 * Periodically trace a message from ZBOSS core to inform external systems (for example, TDF)
 *
 * @param param - unused; it allows you to use the ZBOSS scheduler
 *
 * You can redefine the following parameters in a vendor file:
 * - a message period, @see ZB_DIAG_CORE_WATCHDOG_TMO_MS
 * - base message text, @see ZB_DIAG_CORE_WATCHDOG_BASE_MSG
 */
void zb_diag_core_watchdog(zb_uint8_t param);

#endif /* ZB_DIAG_H */
