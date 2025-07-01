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
/* PURPOSE: specific definitions for ZGP profile
*/

#ifndef ZB_ZGP_GP_SPEC_H
#define ZB_ZGP_GP_SPEC_H 1

#include "zb_common.h"

#ifdef ZB_ENABLE_ZGP_SINK
/********************************************************************/
/********* Type declarations specific to ZGP profile ****************/
/********************************************************************/

/** @addtogroup zgp_common */
/** @{ */


/**
 * @brief ZGP Basic cluster version attributes
 * @see ZGP Generic Requirements 2.6 (GP_P312_FS_07121_Generic_Requirements.pdf) */
enum zb_zgp_gp_basic_version_attr_e
{
  ZB_ZGP_GP_ATTR_BASIC_APPLICATION_VERSION_ID = 0x0011,
  ZB_ZGP_GP_ATTR_BASIC_STACK_VERSION_ID       = 0x0012,
  ZB_ZGP_GP_ATTR_BASIC_HW_VERSION_ID          = 0x0013,
  ZB_ZGP_GP_ATTR_BASIC_MODEL_IDENTIFIER_ID    = 0x0015,
};


/** @} */ //@addtogroup zgp_common

#endif  /* ZB_ENABLE_ZGP_SINK */
#endif  /* ZB_ZGP_GP_SPEC_H */
