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
/*  PURPOSE: The custom configuration: ZC, network size of 20, light traffic load, medium app complexity, including source matching
*/
#ifndef ZB_MEM_CONFIG_LPRF3_H
#define ZB_MEM_CONFIG_LPRF3_H 1

#include "ti_zigbee_config.h"

/**
   @addtogroup configurable_mem
   @{
   @cond skip_this
   @{
*/

/*#define ZB_CONFIG_OVERALL_NETWORK_SIZE 128*/
/*#define ZB_CONFIG_OVERALL_NETWORK_SIZE 32*/
#define ZB_CONFIG_OVERALL_NETWORK_SIZE MAX_CHILDREN
/*#define ZB_CONFIG_OVERALL_NETWORK_SIZE 16*/

/**
   @}
   @endcond
*/

/*#define ZB_CONFIG_HIGH_TRAFFIC*/
/*#define ZB_CONFIG_MODERATE_TRAFFIC*/
/**
   Light routing and application traffic from/to that device.
 */
#define ZB_CONFIG_LIGHT_TRAFFIC

/*#define ZB_CONFIG_APPLICATION_COMPLEX*/
/**
   Medium routing and application traffic from/to that device.
 */
#define ZB_CONFIG_APPLICATION_MODERATE
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

#endif /* ZB_MEM_CONFIG_LPRF3_H */
