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
/* PURPOSE: Illuminance Measurement cluster definitions
*/

#ifndef ZB_ZCL_ILLUMINANCE_MEASUREMENT_H
#define ZB_ZCL_ILLUMINANCE_MEASUREMENT_H 1

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_ILLUMINANCE_MEASUREMENT
 *  @{
 */


/* Cluster ZB_ZCL_CLUSTER_ID_ILLUMINANCE_MEASUREMENT */

/*! @name Illuminance Measurement cluster attributes
    @{
*/

/*! @brief Illuminance Measurement cluster attribute identifiers
    @see ZCL spec, Illuminance Measurement Cluster 4.2.2.2.1
*/
enum zb_zcl_illuminance_measurement_attr_e
{
  /** @brief MeasuredValue, ZCL spec 4.2.2.2.1.1 */
  ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MEASURED_VALUE_ID     = 0x0000,
  /** @brief MinMeasuredValue, ZCL spec 4.2.2.2.1.2 */
  ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MIN_MEASURED_VALUE_ID = 0x0001,
  /** @brief MaxMeasuredValue, ZCL spec 4.2.2.2.1.3 */
  ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MAX_MEASURED_VALUE_ID = 0x0002,
  /** The Tolerance attribute SHALL indicate the magnitude of the
   *  possible error that is associated with MeasuredValue, using the
   *  same units and resolution. */
  ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_TOLERANCE_ID          = 0x0003,
  /** The LightSensorType attribute specifies the electronic type of
   *  the light sensor. */
  ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_LIGHT_SENSOR_TYPE_ID  = 0x0004,
};

/** @brief Default value for Illuminance Measurement cluster revision global attribute */
#define ZB_ZCL_ILLUMINANCE_MEASUREMENT_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0002u)

/** @brief MeasuredValue attribute too-low value */
#define ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MEASURED_VALUE_TOO_LOW       0

/** @brief MeasuredValue attribute invalid value */
#define ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MEASURED_VALUE_INVALID       0xFFFF

/** @brief MeasuredValue attribute default value */
#define ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MEASURED_VALUE_DEFAULT \
  ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MEASURED_VALUE_TOO_LOW

/** @brief Default value for LightSensorType attribute */
#define ZB_ZCL_ILLUMINANCE_MEASUREMENT_LIGHT_SENSOR_TYPE_DEFAULT_VALUE ((zb_uint8_t)0xFF)

/** @brief MinMeasuredValue attribute minimum value */
#define ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MIN_MEASURED_VALUE_MIN_VALUE 0x0001

/** @brief MinMeasuredValue attribute maximum value */
#define ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MIN_MEASURED_VALUE_MAX_VALUE 0xFFFD

/** @brief MinMeasuredValue attribute not-defined value */
#define ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MIN_MEASURED_VALUE_UNDEFINED 0xFFFF

/** @brief MaxMeasuredValue attribute minimum value */
#define ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MAX_MEASURED_VALUE_MIN_VALUE 0x0002

/** @brief MaxMeasuredValue attribute maximum value */
#define ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MAX_MEASURED_VALUE_MAX_VALUE 0xFFFE

/** @brief MaxMeasuredValue attribute not-defined value */
#define ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MAX_MEASURED_VALUE_UNDEFINED 0xFFFF


/** @cond internals_doc */
/*! @internal @name Illuminance Measurement cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MEASURED_VALUE_ID(data_ptr) \
{                                                                                              \
  ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MEASURED_VALUE_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U16,                                                                        \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                                 \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                                          \
  (void*) data_ptr                                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MIN_MEASURED_VALUE_ID(data_ptr) \
{                                                                                                  \
  ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MIN_MEASURED_VALUE_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U16,                                                                            \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                    \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                                              \
  (void*) data_ptr                                                                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MAX_MEASURED_VALUE_ID(data_ptr) \
{                                                                                                  \
  ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MAX_MEASURED_VALUE_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U16,                                                                            \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                    \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                                              \
  (void*) data_ptr                                                                                 \
}

#define ZB_ZCL_ILLUMINANCE_MEASUREMENT_REPORT_ATTR_COUNT 1

/*! @} */ /* Illuminance Measurement cluster internals */
/*! @}
 *  @endcond */ /* internals_doc */

/** @brief Declare attribute list for Illuminance Measurement cluster - server side
    @param attr_list - attribute list name
    @param value - pointer to variable to store MeasuredValue attribute
    @param min_value - pointer to variable to store MinMeasuredValue attribute
    @param max_value - pointer to variable to store MAxMeasuredValue attribute
*/
#define ZB_ZCL_DECLARE_ILLUMINANCE_MEASUREMENT_ATTRIB_LIST(attr_list,                          \
    value, min_value, max_value)                                                               \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_ILLUMINANCE_MEASUREMENT) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MEASURED_VALUE_ID, (value))         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MIN_MEASURED_VALUE_ID, (min_value)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ILLUMINANCE_MEASUREMENT_MAX_MEASURED_VALUE_ID, (max_value)) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */ /* Illuminance Measurement cluster attributes */

/*! @name Illuminance Measurement cluster commands
    @{
*/

/*! @} */ /* Illuminance Measurement cluster commands */

/*! @} */ /* ZCL Illuminance Measurement cluster definitions */

/** @endcond */

void zb_zcl_illuminance_measurement_init_server(void);
void zb_zcl_illuminance_measurement_init_client(void);
#define ZB_ZCL_CLUSTER_ID_ILLUMINANCE_MEASUREMENT_SERVER_ROLE_INIT zb_zcl_illuminance_measurement_init_server
#define ZB_ZCL_CLUSTER_ID_ILLUMINANCE_MEASUREMENT_CLIENT_ROLE_INIT zb_zcl_illuminance_measurement_init_client

#endif /* ZB_ZCL_ILLUMINANCE_MEASUREMENT_H */
