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
/* PURPOSE: Deprecated Definitions of SE's Keep-Alive cluster (SE spec, Annex A.3)
*/

#ifndef ZB_SE_KEEP_ALIVE_H
#define ZB_SE_KEEP_ALIVE_H 1

/** @cond DOXYGEN_SE_SECTION */

/** @addtogroup ZB_ZCL_KEEP_ALIVE_HEADER_DEPRECATED
 *  @{
 *    @details
 *    Using of this SE cluster header is deprecated. The header was moved from SE to ZCL in ZCL8. Use ZCL Keep Alive Cluster instead (include/zcl/zb_zcl_keep_alive.h).
 *    @deprecated
 *    This cluster header as a part of SE will be removed in February 2023 in favor of its heir ZCL Keep Alive Cluster.
 *
 */

/* Deprecated header ZB_SE_KEEP_ALIVE_H */

/*! @} */ /* SE Keep-Alive cluster definitions */

/** @endcond */ /* DOXYGEN_SE_SECTION */

#include "zcl/zb_zcl_keep_alive.h"

#endif /* ZB_SE_KEEP_ALIVE_H */
