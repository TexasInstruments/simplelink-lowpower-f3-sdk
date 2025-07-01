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
/* PURPOSE:
*/

#ifndef ZDO_WWAH_PARENT_CLASSIFICATION_H
#define ZDO_WWAH_PARENT_CLASSIFICATION_H 1

#include "zb_config.h"
#include "zb_common.h"

#ifdef ZB_PARENT_CLASSIFICATION

/** @cond DOXYGEN_ZCL_SECTION */

/**
 *  @addtogroup ZB_ZCL_WWAH
 *  @{
 */

/**
 *  @addtogroup ZDO_WWAH_PARENT_CLASSIFICATION
 *  @{
 */

/**
 *  @brief Attribute to determine min RSSI for receiving packets
 *
 *         See Zigbee WWAH Requirements spec,
 *         6.2 Commissioning Requirements, C-20
 *
 *         @ZDO_WWAH_RSSI_FOR_GOOD_LINK_QUALITY_GROUP
 */
/*
 * TODO: ZDO_WWAH_MIN_RSSI_FOR_RECEIVING_PACKETS is platform dependent!!!
 *
 * Now ZDO_WWAH_MIN_RSSI_FOR_RECEIVING_PACKETS and 
 * ZDO_WWAH_RSSI_FOR_RECEIVING_PACKETS_ADDING are zero to put all
 * beacons in the Good Link Quality group and check only TC Connectivity
 * and Long Uptime flags.
 */
#define ZDO_WWAH_MIN_RSSI_FOR_RECEIVING_PACKETS 0

/**
 *  @brief Additional RSSI value to determine beacons by two groups:
 *         Good and Marginal Link Quality
 *
 *         See Zigbee WWAH Requirements spec,
 *         6.2 Commissioning Requirements, C-20
 *
 *         @ZDO_WWAH_RSSI_FOR_GOOD_LINK_QUALITY_GROUP
 */
/* TODO: maybe need to move it into the MAC module */
#define ZDO_WWAH_RSSI_FOR_RECEIVING_PACKETS_ADDING 0

/**
 *  @brief Min RSSI to determine a beacon in the Good Link Quality Group
 *
 *         Received beacons with an RSSI less that value,
 *         are put in the Marginal Link Quality group
 *
 *         See Zigbee WWAH Requirements spec,
 *         6.2 Commissioning Requirements, C-20
 */
#define ZDO_WWAH_RSSI_FOR_GOOD_LINK_QUALITY_GROUP  \
  (ZDO_WWAH_MIN_RSSI_FOR_RECEIVING_PACKETS         \
   + ZDO_WWAH_RSSI_FOR_RECEIVING_PACKETS_ADDING)

/**
 *  @brief Return ZB_TRUE if RSSI value is valid for Good Link Quality group
 */
#define ZDO_WWAH_GET_LINK_QUALITY_GROUP_BY_RSSI(rssi) \
  (zb_bool_t)((rssi) >= ZDO_WWAH_RSSI_FOR_GOOD_LINK_QUALITY_GROUP)

#endif  /* #ifdef ZB_PARENT_CLASSIFICATION */

/* Let's compile that macro always */

/* low bit for TC Connectivity; high bit for Long Uptime */
#define ZB_ZDO_TC_CONNECTIVITY_BIT_POS 0U
#define ZB_ZDO_LONG_UPTIME_BIT_POS 1U

#define ZB_ZDO_SET_TC_CONNECTIVITY_BIT_VALUE(mask, val)               \
  if (ZB_U2B(val))                                                    \
  {                                                                   \
    ZB_SET_BIT_IN_BIT_VECTOR((mask), ZB_ZDO_TC_CONNECTIVITY_BIT_POS); \
  }                                                                   \
  else                                                                \
  {                                                                   \
    ZB_CLR_BIT_IN_BIT_VECTOR((mask), ZB_ZDO_TC_CONNECTIVITY_BIT_POS); \
  }

#define ZB_ZDO_SET_LONG_UPTIME_BIT_VALUE(mask, val)                   \
  if (ZB_U2B(val))                                                    \
  {                                                                   \
    ZB_SET_BIT_IN_BIT_VECTOR((mask), ZB_ZDO_LONG_UPTIME_BIT_POS);     \
  }                                                                   \
  else                                                                \
  {                                                                   \
    ZB_CLR_BIT_IN_BIT_VECTOR((mask), ZB_ZDO_LONG_UPTIME_BIT_POS);     \
  }

#ifdef ZB_PARENT_CLASSIFICATION


/* 0b00 */
#define ZB_ZCL_WWAH_NO_TC_CONNECTIVITY_AND_SHORT_UPTIME_MASK \
  (zb_uint8_t)(0)

/* 0b10 */
#define ZB_ZCL_WWAH_NO_TC_CONNECTIVITY_AND_LONG_UPTIME_MASK \
  (zb_uint8_t)(1 << ZB_ZDO_LONG_UPTIME_BIT_POS)

/* 0b01 */
#define ZB_ZCL_WWAH_TC_CONNECTIVITY_AND_SHORT_UPTIME_MASK \
  (zb_uint8_t)(1 << ZB_ZDO_TC_CONNECTIVITY_BIT_POS)

/* 0b11 */
#define ZB_ZCL_WWAH_TC_CONNECTIVITY_AND_LONG_UPTIME_MASK \
  (zb_uint8_t)(1 << ZB_ZDO_TC_CONNECTIVITY_BIT_POS | 1 << ZB_ZDO_LONG_UPTIME_BIT_POS)

/** After this interval ZB_NIB().nwk_long_uptime bit will be set to 1 */
#define ZDO_WWAH_LONG_UPTIME_INTERVAL (24*60*60*ZB_TIME_ONE_SECOND)

/**
 *  @param Set 'WWAH Parent Classification is enabled' in ZDO ctx
 *
 *  @param enable ZB_TRUE if WWAH Parent Classification is enabled; ZB_FALSE otherwise
 */
void zdo_wwah_parent_classification_set(zb_bool_t enable);

/**
 *  @brief Get @zb_zcl_wwah_parent_priority_t by @zb_zcl_wwah_classification_mask_t
 *
 *  @param mask - one byte which contain TC Connectivity and Long Uptime bits values
 *
 *  @return @ZB_ZCL_WWAH_PARENT_PRIORITY_INVALID if invalid parameter uses;
 *          @zb_zcl_wwah_parent_priority_t value otherwise
 */
zb_zcl_wwah_parent_priority_t zdo_wwah_get_parent_priority_by_classification_mask(
  zb_uint8_t mask);


/**
 *  @brief Compare neighbors by RSSI and WWAH Parent Priorities
 *         (see @zb_zcl_wwah_parent_priority_t) if WWAH Parent
 *         Classification is enabled; otherwise will be used only LQI value
 *
 *  @param nbte_first - pointer to the first neighbor table entry
 *  @param nbte_second - pointer to the second neighbor table entry
 */
zb_bool_t zdo_wwah_compare_neighbors(
  zb_ext_neighbor_tbl_ent_t *nbte_first, zb_ext_neighbor_tbl_ent_t *nbte_second);

/** @} */ /* @addtogroup ZDO_WWAH_PARENT_CLASSIFICATION */

/** @} */ /* @addtogroup ZB_ZCL_WWAH */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

#endif /* ZB_PARENT_CLASSIFICATION */

#endif /* ZDO_WWAH_PARENT_CLASSIFICATION_H */
