/* GreenBOSS Zigbee Green Power Device software protocol stack
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
 * www.dsr-iot.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 * This is unpublished proprietary source code of DSR Corporation
 * The copyright notice does not evidence any actual or intended
 * publication of such source code.
 *
 * GreenBOSS is a registered trademark of Data Storage Research LLC d/b/a DSR
 * Corporation
 *
 * Commercial Usage
 * Licensees holding valid DSR Commercial licenses may use
 * this file in accordance with the DSR Commercial License
 * Agreement provided with the Software or, alternatively, in accordance
 * with the terms contained in a written agreement between you and
 * DSR.
 */
/* PURPOSE: Common include file for ZigBee
*/

#ifndef ZB_COMMON_H
#define ZB_COMMON_H 1

#include "zb_config.h"
#include "zb_types.h"
#include "gboss_api.h"
#include "zb_bufpool.h"
#include "zb_debug.h"
#include "zb_errors.h"
#include "zb_osif.h"
#include "zb_trace.h"
#include "zb_cmd_commissioning.h"
#include "zb_mac.h"

#ifdef ZB_TRACE_TO_PORT
#include "zb_ringbuffer.h"
#endif

/** \defgroup grpLLApi GreenBOSS Internals
* The module describes some internal GreenBOSS interfaces.

@{

@defgroup base_types Basic Typedefs
@defgroup macll Interface to the Radio
@defgroup flashll Interface to the Flash
@defgroup secur GPD Security
@defgroup mac MAC Level
@defgroup nwk Network Level
@defgroup comm_int Commissioning Internals
@defgroup gbdebug GreenBOSS Debug Capabilities
@defgroup buildconfig Build Configurations


@}
*/


#endif
