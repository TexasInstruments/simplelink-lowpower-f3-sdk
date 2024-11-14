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
/* PURPOSE: Dehumidification Control cluster definitions
*/

#ifndef ZB_ZCL_DEHUMIDIFICATION_CONTROL_H
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_H 1

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"


/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_DEHUMIDIFICATION
 * @{
 */


/* Cluster ZB_ZCL_CLUSTER_ID_DEHUMIDIFICATION_CONTROL */

/*! @name Dehumidification Control cluster attributes
    @{
*/

/*! @brief Dehumidification Control cluster attribute identifiers
    @see ZCL spec, subclause 6.4.2.2
*/
enum zb_zcl_dehumidification_control_attr_e
{
  /** The RelativeHumidity attribute is an 8-bit value that represents the current
   *  relative humidity (in %) measured by a local or remote sensor. */
  ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_RELATIVE_HUMIDITY_ID           = 0x0000,
  /** @brief Dehumidification Cooling attribute */
  ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_COOLING_ID    = 0x0001,
  /** @brief RHDehumidification Setpoint attribute */
  ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_RHDEHUMIDIFICATION_SETPOINT_ID = 0x0010,
  /** The RelativeHumidityMode attribute is an 8-bit value that specifies how
   *  the RelativeHumidity value is being updated. */
  ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_RELATIVE_HUMIDITY_MODE_ID      = 0x0011,
  /** The DehumidificationLockout attribute is an 8-bit value that specifies
   *  whether dehumidification is allowed or not. */
  ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_LOCKOUT_ID    = 0x0012,
  /** @brief Dehumidification Hysteresis attribute */
  ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_HYSTERESIS_ID = 0x0013,
  /** @brief Dehumidification Max Cool attribute */
  ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_MAX_COOL_ID   = 0x0014,
  /** The RelativeHumidityDisplay attribute is an 8-bit value that specifies
   *  whether the RelativeHumidity value is displayed to the user or not. */
  ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_RELATIVE_HUMIDITY_DISPLAY_ID   = 0x0015,
};

/** @brief Default value for Dehumidification Control cluster revision global attribute */
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0001u)

/** @brief Min value for RHDehumidification Setpoint attribute */
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_RHDEHUMIDIFICATION_SETPOINT_MIN_VALUE 0x1e

/** @brief Max value for RHDehumidification Setpoint attribute */
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_RHDEHUMIDIFICATION_SETPOINT_MAX_VALUE 0x64

/** @brief Default value for RHDehumidification Setpoint attribute */
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_RHDEHUMIDIFICATION_SETPOINT_DEFAULT_VALUE 0x32

/** @brief Default value for RelativeHumidityMode attribute */
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_RELATIVE_HUMIDITY_MODE_DEFAULT_VALUE ((zb_uint8_t)0x00)

/** @brief Default value for DehumidificationLockout attribute */
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_LOCKOUT_DEFAULT_VALUE ((zb_uint8_t)0x01)

/** @brief Min value for Dehumidification Hysteresis attribute */
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_HYSTERESIS_MIN_VALUE 0x02

/** @brief Max value for Dehumidification Hysteresis attribute */
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_HYSTERESIS_MAX_VALUE 0x14

/** @brief Default value for Dehumidification Hysteresis attribute */
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_HYSTERESIS_DEFAULT_VALUE 0x02

/** @brief Min value for Dehumidification Max Cool attribute */
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_MAX_COOL_MIN_VALUE 0x14

/** @brief Max value for Dehumidification Max Cool attribute */
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_MAX_COOL_MAX_VALUE 0x64

/** @brief Default value for Dehumidification Max Cool attribute */
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_MAX_COOL_DEFAULT_VALUE 0x14

/** @brief Default value for RelativeHumidityDisplay attribute */
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_RELATIVE_HUMIDITY_DISPLAY_DEFAULT_VALUE ((zb_uint8_t)0x00)

/** @brief Declare attribute list for Dehumidification Control cluster
    @param attr_list - attribute list name
    @param dehumid_cooling - pointer to variable to store Dehumidification Cooling attribute value
    @param dehumid_setpoint - pointer to variable to store Dehumidification Setpoint attribute value
    @param dehumid_hysteresis - pointer to variable to store Dehumidification Hysteresis attribute value
    @param dehumid_max_cool - pointer to variable to store Dehumidification Max Cool attribute value
*/
#define ZB_ZCL_DECLARE_DEHUMIDIFICATION_CONTROL_ATTRIB_LIST(attr_list, dehumid_cooling, dehumid_setpoint,         \
                                               dehumid_hysteresis, dehumid_max_cool)                              \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_DEHUMIDIFICATION_CONTROL)                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_COOLING_ID, (dehumid_cooling))       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_RHDEHUMIDIFICATION_SETPOINT_ID, (dehumid_setpoint))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_HYSTERESIS_ID, (dehumid_hysteresis)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_MAX_COOL_ID, (dehumid_max_cool))     \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */ /* Dehumidification Control cluster attributes */

/*! @name Dehumidification Control cluster commands
    @{
*/

/*! @} */ /* Dehumidification Control cluster commands */

/** @cond internals_doc */
/*! @internal @name Dehumidification Control cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_COOLING_ID(data_ptr)    \
{                                                                                                            \
  ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_COOLING_ID,                                          \
  ZB_ZCL_ATTR_TYPE_U8,                                                                                       \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                                               \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                                                        \
  (void*) data_ptr                                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_RHDEHUMIDIFICATION_SETPOINT_ID(data_ptr) \
{                                                                                                            \
  ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_RHDEHUMIDIFICATION_SETPOINT_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U8,                                                                                       \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                                                             \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                                                        \
  (void*) data_ptr                                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_HYSTERESIS_ID(data_ptr) \
{                                                                                                            \
  ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_HYSTERESIS_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U8,                                                                                       \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                                                             \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                                                        \
  (void*) data_ptr                                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_MAX_COOL_ID(data_ptr)   \
{                                                                                                            \
  ZB_ZCL_ATTR_DEHUMIDIFICATION_CONTROL_DEHUMIDIFICATION_MAX_COOL_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U8,                                                                                       \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                                                             \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                                                        \
  (void*) data_ptr                                                                                           \
}

/*! @internal Number of attributes mandatory for reporting in Dehumidification Control cluster */
#define ZB_ZCL_DEHUMIDIFICATION_CONTROL_REPORT_ATTR_COUNT 1

/*! @} */ /* Dehumidification Control cluster internals */
/*! @}
 *  @endcond */ /* internals_doc */

/*! @} */ /* ZCL HA Dehumidification Control cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

void zb_zcl_dehumidification_control_init_server(void);
void zb_zcl_dehumidification_control_init_client(void);
#define ZB_ZCL_CLUSTER_ID_DEHUMID_CONTROL_SERVER_ROLE_INIT zb_zcl_dehumidification_control_init_server
#define ZB_ZCL_CLUSTER_ID_DEHUMID_CONTROL_CLIENT_ROLE_INIT zb_zcl_dehumidification_control_init_client

#endif /* ZB_ZCL_DEHUMID_CONTROL_H */
