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
/*  PURPOSE: The biggest possible configuration: ZC, big net, high load, high complexity
*/
#ifndef ZB_MEM_CONFIG_MAXIMUM_H
#define ZB_MEM_CONFIG_MAXIMUM_H 1

/**
   @addtogroup configurable_mem
   @{
*/
/**
   Configure memory sizes for ZC device role
 */
#define ZB_CONFIG_ROLE_ZC

#ifdef DOXYGEN
/**
   Configure memory sizes for ZED device role
 */
#define ZB_CONFIG_ROLE_ZED
/**
   Configure memory sizes for ZR device role
 */
#define ZB_CONFIG_ROLE_ZR
#endif

/*#define ZB_CONFIG_ROLE_ZR*/
/*#define ZB_CONFIG_ROLE_ZED*/
/**
   Max total number of devices in Zigbee network.

   From 2 to 128.
 */
#ifdef ZB_CONFIG_OVERALL_NETWORK_SIZE
#undef ZB_CONFIG_OVERALL_NETWORK_SIZE
#endif
#define ZB_CONFIG_OVERALL_NETWORK_SIZE ZB_NWK_CONFIGURABLE_MEM_MAX_NETWORK_SIZE
/*#define ZB_CONFIG_OVERALL_NETWORK_SIZE 16*/
/*#define ZB_CONFIG_OVERALL_NETWORK_SIZE 32*/

/**
   High routing and application traffic from/to that device.
 */
#define ZB_CONFIG_HIGH_TRAFFIC
/*#define ZB_CONFIG_MODERATE_TRAFFIC*/
/*#define ZB_CONFIG_LIGHT_TRAFFIC*/

/**
   Complex user's application at that device: complex relations to other devices.
 */
#define ZB_CONFIG_APPLICATION_COMPLEX
/*#define ZB_CONFIG_APPLICATION_MODERATE*/
/*#define ZB_CONFIG_APPLICATION_SIMPLE*/

/**
    @}
*/

/* Now common logic derives numerical parameters from the defined configuration. */
#include "zb_mem_config_common.h"

/* Now if you REALLY know what you do, you can study zb_mem_config_common.h and redefine some configuration parameters, like:
#undef ZB_CONFIG_SCHEDULER_Q_SIZE
#define ZB_CONFIG_SCHEDULER_Q_SIZE 56
*/

/* Memory context definitions */
#include "zb_mem_config_context.h"

#endif /* ZB_MEM_CONFIG_MAXIMUM_H */
