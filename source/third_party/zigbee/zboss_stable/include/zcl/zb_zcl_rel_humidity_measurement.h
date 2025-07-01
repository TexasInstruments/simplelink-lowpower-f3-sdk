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
/* PURPOSE: Relative Humidity Measurement cluster definitions
*/

#ifndef ZB_ZCL_REL_HUMIDITY_MEASUREMENT_H
#define ZB_ZCL_REL_HUMIDITY_MEASUREMENT_H 1

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_WATER_CONTENT_MEASUREMENT
 *  @{
 */

/* Cluster ZB_ZCL_WATER_CONTENT_MEASUREMENT */

/*! @name Water Content Measurement cluster attributes
    @{
*/

/*! @brief Water Content Measurement cluster attribute identifiers
    @see ZCL specification revision 7, Relative Humidity MeasuremenWater Content Measurement Cluster 4.7.2.1
*/
enum zb_zcl_rel_humidity_measurement_attr_e
{
  /** @brief MeasuredValue, ZCL specification revision 7 subsection 4.7.2.1.1 MeasuredValue Attribute */
  ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_ID     = 0x0000,
  /** @brief MinMeasuredValue, ZCL specification revision 7 subsection 4.7.2.1.2 MinMeasuredValue Attribute*/
  ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_ID = 0x0001,
  /** @brief MaxMeasuredValue, ZCL specification revision 7 subsection 4.7.2.1.3 MaxMeasuredValue Attribute*/
  ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_ID = 0x0002,
  /** The Tolerance attribute SHALL indicate the magnitude of the
   *  possible error that is associated with MeasuredValue, using
   *  the same units and resolution.
   *  @brief Tolerance, ZCL specification revision 7 subsection 4.7.2.1.4 Tolerance Attribute
   */
  ZB_ZCL_ATTR_REL_HUMIDITY_TOLERANCE_ID             = 0x0003,
};

/** @brief Default value for Water Content Measurement cluster revision global attribute */
#define ZB_ZCL_WATER_CONTENT_MEASUREMENT_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0002u)

/** @brief MeasuredValue attribute unknown value */
#define ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_UNKNOWN        0xFFFF

/** @brief MinMeasuredValue attribute minimum value */
#define ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_MIN_VALUE  0x0000

/** @brief MinMeasuredValue attribute maximum value */
#define ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_MAX_VALUE  0x270F

/** @brief MinMeasuredValue attribute undefined value */
#define ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_UNDEFINED  0xFFFF

/** @brief MaxMeasuredValue attribute minimum value */
#define ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_MIN_VALUE  0x0001

/** @brief MaxMeasuredValue attribute maximum value */
#define ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_MAX_VALUE  0x2710

/** @brief MaxMeasuredValue attribute value not defined */
#define ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_UNDEFINED  0xFFFF

/** @brief Default value for MeasurementValue attribute */
#define ZB_ZCL_REL_HUMIDITY_MEASUREMENT_VALUE_DEFAULT_VALUE ((zb_uint16_t)0xFFFF)

/** @brief Default value for MeasurementMinValue attribute */
#define ZB_ZCL_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_DEFAULT_VALUE ((zb_uint16_t)0xFFFF)

/** @brief Default value for MeasurementMaxValue attribute */
#define ZB_ZCL_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_DEFAULT_VALUE ((zb_uint16_t)0xFFFF)

/** @cond internals_doc */
/*! @internal @name Relative Humidity Measurement cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_ID,                \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,  \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                           \
  (void*) data_ptr                                              \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_ID,    \
  ZB_ZCL_ATTR_TYPE_U16,                                 \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                         \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_ID,    \
  ZB_ZCL_ATTR_TYPE_U16,                                 \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                         \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}

#define ZB_ZCL_REL_HUMIDITY_MEASUREMENT_REPORT_ATTR_COUNT 1

/*! @} */ /* Relative Humidity Measurement cluster internals */
/*! @}
 *  @endcond */ /* internals_doc */

/** @brief Declare attribute list for Relative Humidity Measurement cluster - server side
    @param attr_list - attribute list name
    @param value - pointer to variable to store MeasuredValue attribute
    @param min_value - pointer to variable to store MinMeasuredValue attribute
    @param max_value - pointer to variable to store MAxMeasuredValue attribute
*/
#define ZB_ZCL_DECLARE_REL_HUMIDITY_MEASUREMENT_ATTRIB_LIST(attr_list,          \
    value, min_value, max_value)                                                \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_WATER_CONTENT_MEASUREMENT) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_ID, (value))          \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_ID, (min_value))  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_ID, (max_value))  \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */ /* Relative Humidity Measurement cluster attributes */

/*! @name Relative Humidity Measurement cluster commands
    @{
*/

/*! @} */ /* Relative Humidity Measurement cluster commands */

/*! @} */ /* ZCL Relative Humidity Measurement cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

void zb_zcl_rel_humidity_init_server(void);
void zb_zcl_rel_humidity_init_client(void);
#define ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT_SERVER_ROLE_INIT zb_zcl_rel_humidity_init_server
#define ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT_CLIENT_ROLE_INIT zb_zcl_rel_humidity_init_client

#endif /* ZB_ZCL_REL_HUMIDITY_MEASUREMENT_H */
