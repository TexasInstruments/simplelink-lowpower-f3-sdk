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
/* PURPOSE: Configuration for Zigbee Smart Energy
*/

#ifndef ZB_ZSE_CONFIG_H
#define ZB_ZSE_CONFIG_H 1

#if defined ZB_ENABLE_SE_MIN_CONFIG || defined DOXYGEN

/** @cond DOXYGEN_SE_SECTION */

/************ ZCL specific configuration  ******************/

/**
 ZSE: transactions table size
*/
#define ZSE_TRAN_TABLE_SIZE 16U

#define ZB_ZSE_CERTDB_TBL_SIZE 20U

/** @def ZB_SE_METERING_DATA_UPDATE_RATE
 *  The frequency and timeliness of updating metering data contained in the Metering Cluster
 *  attributes.
 *
 *  ZCL spec 10.4.3.3:
 *    Updates of the metering data should not cause delivery of the information to end devices
 *    more often than once every 30 seconds. End devices should also not request information
 *    more often than than once every 30 seconds.
 */
#define ZB_SE_METERING_DATA_UPDATE_RATE   30U


/*********************** Cluster to command configuration **********************/

#define ZB_SE_ENABLE_KEC_SERVER
#define ZB_SE_ENABLE_KEC_CLIENT

/* Control per-cluster default command processing */

#if (defined ZB_SE_ENABLE_KEC_SERVER || defined ZB_SE_ENABLE_KEC_CLIENT)
#define ZB_ZCL_ENABLE_DEFAULT_KEC_PROCESSING
#ifndef ZB_SE_ENABLE_KEC_CLUSTER
#define ZB_SE_ENABLE_KEC_CLUSTER
#endif
#endif /* ZB_SE_ENABLE_KEC_CLUSTER */

/** @endcond */ /* DOXYGEN_SE_SECTION */

#endif  /* defined ZB_ENABLE_SE_MIN_CONFIG || defined DOXYGEN */

#endif /* ZB_ZCL_CONFIG_H */
