/* ZBOSS Zigbee software protocol stack
 *
 * Copyright (c) 2012-2023 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: PM2.5 Measurement cluster definitions */

#ifndef ZB_ZCL_PM2_5_MEASUREMENT_H
#define ZB_ZCL_PM2_5_MEASUREMENT_H 1

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_CONCENTRATION_MEASUREMENT
 *  @{
 *    @details
 *    The server cluster provides an interface to concentration measurement
 *    functionality. The measurement is reportable and may be configured for
 *    reporting. Concentration measurements include, but are not limited to,
 *    levels in gases, such as CO, CO2, and ethylene, or in fluids and
 *    solids, such as dissolved oxygen, chemi8721 cals & pesticides.
 *
 *    PM2.5 (Particulate Matter 2.5 microns or less)
 */

/* Cluster ZB_ZCL_CLUSTER_ID_PM2_5_MEASUREMENT */

/** @name PM2.5 Measurement cluster attributes
 *  @{
 */

/** @brief PM2.5 Measurement cluster attribute identifiers
 *  @see ZCL spec, PM2.5 Measurement Cluster 4.14.1.4
 */
enum zb_zcl_pm2_5_measurement_attr_e
{
  /** @brief MeasuredValue attribute, ZCL spec 4.13.2.1.1 */
  ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MEASURED_VALUE_ID     = 0x0000,
  /** @brief MinMeasuredValue attribute, ZCL spec 4.13.2.1.2 */
  ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MIN_MEASURED_VALUE_ID = 0x0001,
  /** @brief MaxMeasuredValue attribute, ZCL spec 4.13.2.1.3 */
  ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MAX_MEASURED_VALUE_ID = 0x0002,
  /** @brief Tolerance attribute, ZCL spec 4.13.2.1.4 */
  ZB_ZCL_ATTR_PM2_5_MEASUREMENT_TOLERANCE_ID          = 0x0003,
};

/** @brief Default value for PM2.5 Measurement cluster revision global attribute */
#define ZB_ZCL_PM2_5_MEASUREMENT_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0001u)

/** @brief Default value for MeasuredValue attribute */
#define ZB_ZCL_PM2_5_MEASUREMENT_MEASURED_VALUE_DEFAULT_VALUE     (.0/.0)
        
/** @brief Default value for MinMeasuredValue attribute */
#define ZB_ZCL_PM2_5_MEASUREMENT_MIN_MEASURED_VALUE_DEFAULT_VALUE (.0/.0)

/** @brief Default value for MaxMeasuredValue attribute */
#define ZB_ZCL_PM2_5_MEASUREMENT_MAX_MEASURED_VALUE_DEFAULT_VALUE (.0/.0)


/** @brief MeasuredValue attribute unknown */
#define ZB_ZCL_PM2_5_MEASUREMENT_MEASURED_VALUE_UNKNOWN       (.0/.0)

/** @brief MinMeasuredValue attribute undefined */
#define ZB_ZCL_PM2_5_MEASUREMENT_MIN_MEASURED_VALUE_UNDEFINED (.0/.0)

/** @brief MaxMeasuredValue attribute undefined */
#define ZB_ZCL_PM2_5_MEASUREMENT_MAX_MEASURED_VALUE_UNDEFINED (.0/.0)

/** @brief MinMeasuredValue attribute minimum value */
#define ZB_ZCL_PM2_5_MEASUREMENT_MIN_MEASURED_VALUE_MIN_VALUE 0.0

/** @brief MaxMeasuredValue attribute maximum value */
#define ZB_ZCL_PM2_5_MEASUREMENT_MAX_MEASURED_VALUE_MAX_VALUE 1.0

/** @cond internals_doc */
/** @name PM2.5 Measurement cluster internals
 *  Internal structures for PM2.5 Measurement cluster
 *  @{
 */

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MEASURED_VALUE_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MEASURED_VALUE_ID,                  \
  ZB_ZCL_ATTR_TYPE_SINGLE,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,      \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MIN_MEASURED_VALUE_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MIN_MEASURED_VALUE_ID,              \
  ZB_ZCL_ATTR_TYPE_SINGLE,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                     \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MAX_MEASURED_VALUE_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MAX_MEASURED_VALUE_ID,              \
  ZB_ZCL_ATTR_TYPE_SINGLE,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                     \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                               \
  (void*) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PM2_5_MEASUREMENT_TOLERANCE_ID(data_ptr) \
{                                                                   \
  ZB_ZCL_ATTR_PM2_5_MEASUREMENT_TOLERANCE_ID,                       \
  ZB_ZCL_ATTR_TYPE_SINGLE,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                     \
  (ZB_ZCL_MANUFACTURER_SPECIFIC),                                   \
  (void*) data_ptr                                                  \
}

/** Number of attributes mandatory for reporting in PM2.5 Measurement cluster */
#define ZB_ZCL_PM2_5_MEASUREMENT_REPORT_ATTR_COUNT 1

/** @} */
/** @endcond */ /* PM2.5 Measurement cluster internals */

/** @brief Declare attribute list for PM2.5 Measurement cluster
 *  @param attr_list - attribute list name
 *  @param measured_value -  pointer to variable storing MeasuredValue attribute value
 *  @param min_measured_value - pointer to variable storing MinMeasuredValue attribute value
 *  @param max_measured_value - pointer to variable storing MaxMeasuredValue attribute value
 *  @param tolerance - pointer to variable storing Tolerance attribute value
 */
#define ZB_ZCL_DECLARE_PM2_5_MEASUREMENT_ATTRIB_LIST(                             \
    attr_list,                                                                    \
    measured_value,                                                               \
    min_measured_value,                                                           \
    max_measured_value,                                                           \
    tolerance)                                                                    \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_PM2_5_MEASUREMENT)          \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MEASURED_VALUE_ID, (measured_value))         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MIN_MEASURED_VALUE_ID, (min_measured_value)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MAX_MEASURED_VALUE_ID, (max_measured_value)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PM2_5_MEASUREMENT_TOLERANCE_ID, (tolerance))                   \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/**
 *  @brief PM2.5 Measurement cluster attributes
 */
typedef struct zb_zcl_pm2_5_measurement_attrs_s
{
  /** @copydoc ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MEASURED_VALUE_ID
   * @see ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MEASURED_VALUE_ID
   */
  zb_single_t measured_value;

  /** @copydoc ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MIN_MEASURED_VALUE_ID
   * @see ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MIN_MEASURED_VALUE_ID
   */
  zb_single_t min_measured_value;

  /** @copydoc ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MAX_MEASURED_VALUE_ID
   * @see ZB_ZCL_ATTR_PM2_5_MEASUREMENT_MAX_MEASURED_VALUE_ID
   */
  zb_single_t max_measured_value;

  /** @copydoc ZB_ZCL_ATTR_PM2_5_MEASUREMENT_TOLERANCE_ID
   * @see ZB_ZCL_ATTR_PM2_5_MEASUREMENT_TOLERANCE_ID
   */
  zb_single_t tolerance;
} zb_zcl_pm2_5_measurement_attrs_t;

/** @brief Declare attribute list for PM2.5 Measurement cluster
 *  @param[in]  attr_list - attribute list variable name
 *  @param[in]  attrs - variable of @ref zb_zcl_pm2_5_measurement_attrs_t type
 *                      (containing PM2.5 Measurement cluster attributes)
 */
#define ZB_ZCL_DECLARE_PM2_5_MEASUREMENT_ATTR_LIST(attr_list, attrs)      \
  ZB_ZCL_DECLARE_PM2_5_MEASUREMENT_ATTRIB_LIST(attr_list,                 \
                                               &attrs.measured_value,     \
                                               &attrs.min_measured_value, \
                                               &attrs.max_measured_value, \
                                               &attrs.tolerance)


/** @} */ /* end of ZB_ZCL_PM2_5_MEASUREMENT_ATTRS_GROUP group */

/** @} */ /* ZCL PM2.5 Measurement cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

void zb_zcl_pm2_5_measurement_init_server(void);
void zb_zcl_pm2_5_measurement_init_client(void);
#define ZB_ZCL_CLUSTER_ID_PM2_5_MEASUREMENT_SERVER_ROLE_INIT zb_zcl_pm2_5_measurement_init_server
#define ZB_ZCL_CLUSTER_ID_PM2_5_MEASUREMENT_CLIENT_ROLE_INIT zb_zcl_pm2_5_measurement_init_client

#endif /* ZB_ZCL_PM2_5_MEASUREMENT_H */
