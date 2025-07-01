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
/* PURPOSE: Security - API to be used from the applications level
*/

#ifndef ZB_SECUR_API_H
#define ZB_SECUR_API_H 1
/** @cond (!DOXYGEN_ERL_SECTION) */
/*! \addtogroup secur_api */
/*! @{ */

#ifdef ZB_COORDINATOR_ROLE
/**
   Send new network key to all devices in the net via broadcast

   4.6.3.4  Network Key Update
   4.6.3.4.1  Trust Center Operation

   @param param - buffer with single parameter - short broadcast address. Valid
          values are 0xffff, 0xfffd

 */
void zb_secur_send_nwk_key_update_br(zb_uint8_t param);



/**
   Generate switch key.

   According to test 14.24TP/SEC/BV-01-I Security NWK Key Switch (No Pre-
   configured Key)-ZR, this command can be send either to broadcast or unicast
   to all rx-on-when-idle from the neighbor.

   When send unicast, it encrypted by the new (!) key, when send proadcast - by
   the old key.

   That mean, switch our key _after_ this frame transfer and secure - in the
   command send confirm.

   @param param - packet buffer with single parameter - broadcast address. If 0,
          send unicast.
 */
void zb_secur_send_nwk_key_switch(zb_uint8_t param);
#endif /* ZB_COORDINATOR_ROLE */

/*! @} */
/** @endcond */ /* (!DOXYGEN_ERL_SECTION) */
#endif /* ZB_SECUR_API_H */
