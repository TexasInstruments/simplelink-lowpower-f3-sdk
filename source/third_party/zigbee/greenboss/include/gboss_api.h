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
/* PURPOSE: GreenBOSS API header
*/

#ifndef GBOSS_API_H
#define GBOSS_API_H 1

#include "zb_config.h"
#include "zb_types.h"
#include "zb_errors.h"
#include "zb_magic_macros.h"
#include "gboss_osif.h"
#include "gboss_app.h"
#include "zb_trace.h"
#include "gboss_commissioning.h"
#include "zb_zcl_common.h"
#include "zb_zcl_ias_zone.h"
#include "zb_zcl_power_config.h"
#include "gboss_cmd_common.h"
#include "gboss_commands.h"

/** \defgroup grpFullApi Application GPD API
The module describes main GPD API that should be used to
make own Green Power device.

@{

@defgroup appconf Application Initialization and Configuration
@defgroup commis GPD Сommissioning API
@defgroup cmdtx GPD Commands API
@{
@defgroup grpCmdOnOff ON/OFF Сluster Сommands
@defgroup grpCmdScenes Scene Сluster Сommands
@defgroup grpCmdLevelControl Level Control Сluster Сommands
@defgroup grpCmdColorControl Color Control Сluster Сommands
@defgroup grpCmdDoorLock Door Lock Сluster Сommands
@defgroup grpCmdAttributeReporting ZCL Attribute Reporting
@{
@defgroup zclCommon Zigbee Cluster Common Definitions
@defgroup zclIasZone IAS Zone Cluster Definitions
@defgroup zclPowerConfig Power Configuration Cluster Definitions
@}

@defgroup bidir Bidirectional in Operational API
@ref bidir_specific
@{
@defgroup grpBidirCmd Commands to be sent by the Bidirectional in Operational application
@defgroup bidirCmdHandle Helper functions for handling received commands
@defgroup bidirRxTx Functions to provide Rx after Tx for Bidirectional in Operational application
@}

@}

@}

*/


#endif /* GBOSS_API_H */
