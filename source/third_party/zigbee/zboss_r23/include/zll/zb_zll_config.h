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
/* PURPOSE: ZLL profile configuration
*/

#ifndef ZB_ZLL_CONFIG_H
#define ZB_ZLL_CONFIG_H 1

#include "zcl/zb_zcl_config.h"

/*!
    @addtogroup ZB_ZLL_COMMON
    @internal
    @{
*/

/* Normally startup attribute sets should be processed */
#define ZB_ENABLE_ZLL_SAS

/********************** General ZLL config parameters **********************/

/** Defines default time for device to identify (in seconds)\. Shan't be zero. */
#define ZB_ZLL_IDENTIFY_TIME_DEFAULT_VALUE 2

/**
 *  @brief Defines length of a group identifier range provided to the address-assignment-capable
 *  commissioning target device.
 */
#define ZB_ZLL_GROUP_ID_RANGE_ALLOC_LENGTH  1000

/**
 *  @brief Defines length of network address range provided to the address-assignment-capable
 *  commissioning target device.
 */
#define ZB_ZLL_ADDR_RANGE_ALLOC_LENGTH 1000

/**
 *  @brief NLME-NETWORK-DISCOVERY.request.ScanDuration value.
 *  Being used in start new network procedure on the target side.
 *  @see ZLL spec, subclause 8.4.3.2.
 */
#define ZB_ZLL_NWK_DISC_DURATION  3

#if defined ZB_ZLL_ADDR_ASSIGN_CAPABLE
/** @brief Default address range begin value. */
#define ZB_ZLL_ADDR_BEGIN_INIT_VALUE  0x0001

/** @brief Default address range end value. */
#define ZB_ZLL_ADDR_END_INIT_VALUE  0xfff7

/** @brief Default group identifier range begin value. */
#define ZB_ZLL_GROUP_ID_BEGIN_INIT_VALUE  0x0001

/** @brief Default group identifier range end value. */
#define ZB_ZLL_GROUP_ID_END_INIT_VALUE  0xfeff

#endif /* defined ZB_ZLL_ADDR_ASSIGN_CAPABLE */

/** @brief Default RSSI correction value. */
#define ZB_ZLL_DEFAULT_RSSI_CORRECTION  0x00

/** @brief Default minimum RSSI value to process ScanRequest packets.
 *  According to spec, this value is manufacturer specific.
 *  @see ZLL spec, subclause 8.4.1.2.
 */
#define ZB_ZLL_DEFAULT_RSSI_THRESHOLD -64

/**
 * @}
 */

#endif /* ZB_ZLL_CONFIG_H */
