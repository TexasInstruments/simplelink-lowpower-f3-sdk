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
/* PURPOSE: Configuration for Zigbee cluster library
*/

#ifndef ZB_ZCL_CONFIG_H
#define ZB_ZCL_CONFIG_H 1

/** @cond DOXYGEN_ZCL_SECTION */

/************ ZCL specific configuration  ******************/

/**
 ZCL: transactions table size
*/
#define ZCL_TRAN_TABLE_SIZE 16U

/* DA: range check error */
/* #define ZB_DISABLE_TEMP_MEASUREMENT_TOLERANCE_ID */

/* #define ZB_ZCL_OTA_INCREMENTAL_QUERY_INTERVAL */

/* Enable CVC feature  */
#define ZB_ZCL_ENABLE_CVC

/* Disable Poll Control Check-Ins during OTA upgrade (to reduce the load) */
#define xZB_ZCL_NO_CHECKINS_DURING_OTA


/*********************** Cluster to command configuration **********************/

/* General commands default processing */
#define ZB_ZCL_ENABLE_DEFAULT_REPORT_ATTR_PROCESSING

/* Control per-cluster default command processing */

#if defined (ZB_ZCL_SUPPORT_CLUSTER_GROUPS)
#define ZB_ZCL_ENABLE_DEFAULT_GROUPS_PROCESSING
#endif /* ZB_ZCL_SUPPORT_CLUSTER_GROUPS */

#if defined (ZB_ZCL_SUPPORT_CLUSTER_GROUPS_CLIENT) 
#define ZB_ZCL_ENABLE_DEFAULT_GROUPS_PROCESSING_CLIENT
#endif /* ZB_ZCL_SUPPORT_CLUSTER_GROUPS_CLIENT */

/* Control per-cluster default scene extension processing */
#if defined ZB_ZCL_SUPPORT_CLUSTER_SCENES

#if defined ZB_ZCL_SUPPORT_CLUSTER_THERMOSTAT
#define ZB_ZCL_SUPPORT_THERMOSTAT_SCENE_EXTENSIONS
#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_THERMOSTAT */

#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_SCENES */

#if defined ZB_ZCL_SUPPORT_CLUSTER_POLL_CONTROL
#define ZB_HA_ENABLE_POLL_CONTROL_SERVER
#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_POLL_CONTROL */

/* TODO: Divide Server and Client processing and turn on server part only when it is really needed -
 * OTA cluster is rather big in terms of ROM size! */
#if defined ZB_ZCL_SUPPORT_CLUSTER_OTA_UPGRADE
#define ZB_ZCL_ENABLE_DEFAULT_OTA_UPGRADE_PROCESSING
#define ZB_HA_ENABLE_OTA_UPGRADE_SERVER     1
#define ZB_HA_ENABLE_OTA_UPGRADE_CLIENT     1
#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_OTA_UPGRADE */

#if defined ZB_ZCL_SUPPORT_CLUSTER_TUNNEL
#define ZB_ZCL_ENABLE_DEFAULT_TUNNEL_PROCESSING
#endif /* defined ZB_ENABLE_TUNNEL_CLUSTER */

#if defined ZB_ZCL_SUPPORT_CLUSTER_IR_BLASTER
#define ZB_ZCL_ENABLE_DEFAULT_IR_BLASTER_PROCESSING
#endif /* defined ZB_ENABLE_IR_BLASTER_CLUSTER */

#if defined  ZB_ZCL_SUPPORT_CLUSTER_DIAGNOSTICS
#define ZB_MAC_DIAGNOSTICS
#define ZDO_DIAGNOSTICS
#endif /* ZB_ZCL_SUPPORT_CLUSTER_DIAGNOSTICS */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

#endif /* ZB_ZCL_CONFIG_H */
