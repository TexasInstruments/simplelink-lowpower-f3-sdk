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
/* PURPOSE: HA configuration
*/

#ifndef ZB_HA_CONFIG_H
#define ZB_HA_CONFIG_H 1

#include "zb_vendor.h"

#if defined ZB_ENABLE_HA || defined DOXYGEN

/** @cond DOXYGEN_HA_SECTION */

/* Specify a list of devices that should be supported */
#define xZB_ALL_DEVICE_SUPPORT /* Attention! use this macro in testing purposes */

/* Normally startup attribute sets should be processed */
/* TODO Re-enable HA SAS processing when appropriate */
#define xZB_ENABLE_HA_SAS

/* NK: why Gateway is always defined? */
#define xZB_HA_DEFINE_DEVICE_GATEWAY


#include "zb_ha_device_config.h"
#include "zcl/zb_zcl_config.h"

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* ZB_ENABLE_HA */

#endif /* ZB_HA_CONFIG_H */
