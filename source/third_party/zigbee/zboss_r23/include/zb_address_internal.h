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
/* PURPOSE: Zigbee address management (internal)
*/

#ifndef ZB_ADDRESS_INTERNAL_H
#define ZB_ADDRESS_INTERNAL_H 1

/*! \addtogroup ZB_NWK_ADDR */
/*! @{ */

/*! @cond internals_doc */

/**
   Reset address map.

   It is the caller's responsibility to ensure that no addr ref is in use since all addr refs will be invalidated.

   @return RET_OK or error code
 */
void zb_address_reset(void);

/*! @endcond */

/*! @} */


#endif /* ZB_ADDRESS_INTERNAL_H */

