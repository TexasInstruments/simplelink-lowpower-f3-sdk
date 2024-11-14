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
/* PURPOSE: ZLL startup attribute sets definitions
*/
#ifndef ZB_ZLL_SAS_H
#define ZB_ZLL_SAS_H 1

#if defined ZB_ENABLE_ZLL_SAS || defined DOXYGEN

/**
 *  @internal
 *  @defgroup ZB_ZLL_SAS ZLL SAS
 *  @ingroup ZB_ZLL
 *  @addtogroup ZB_ZLL_SAS
 *  ZLL startup attribute sets definitions
 *  @{
 */


/** SAS short address */
#define ZB_ZLL_SAS_SHORT_ADDRESS ZB_NWK_BROADCAST_ALL_DEVICES

/** SAS extension Pan ID*/
#define ZB_ZLL_SAS_EXT_PAN_ID {0, 0, 0, 0, 0, 0, 0, 0}

/** SAS 16bit Pan ID */
#define ZB_ZLL_SAS_PAN_ID ZB_BROADCAST_PAN_ID

/** SAS channel mask */
#define ZB_ZLL_SAS_CHANNEL_MASK ZB_TRANSCEIVER_ALL_CHANNELS_MASK

/** SAS permit join is enable */
#define ZB_ZLL_SAS_DISABLE_PERMIT_JOIN  0

/** SAS insecure join is true */
#define ZB_ZLL_SAS_INSECURE_JOIN ZB_TRUE

/** @brief SAS procedure */
void zb_zll_process_sas(void);

#endif /* defined ZB_ENABLE_ZLL_SAS */

/**
 * @}
 */

#endif /* ZB_ZLL_SAS_H */
