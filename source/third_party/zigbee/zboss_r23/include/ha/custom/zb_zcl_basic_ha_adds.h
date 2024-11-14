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
/* PURPOSE: ZCL Basic Cluster definitions - extension for HA IAS devices
*/
#ifndef ZB_ZCL_BASIC_HA_ADDS_H
#define ZB_ZCL_BASIC_HA_ADDS_H 1

/* WARNING: This file is LEGACY for INTERNAL usage only! DO NOT include it into any SDK! */

#include "zcl/zb_zcl_basic.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_HA_SECTION */

/** @addtogroup ZB_ZCL_BASIC_HA_ADDS
 *  @ingroup ZB_HA_CLUSTERS_ADDS
  * @{
  *   @details
  *   Extension for HA devices - DeviceEnable attribute shall be read-only and set to 1.
  */

/** @internal @name Basic cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

/** @brief Declare attribute list for Basic cluster
    @param attr_list - attribute list name
    @param zcl_version - pointer to variable to store zcl version  attribute value
    @param power_source - pointer to variable to store power source attribute value
*/
#define ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_HA_ADDS(attr_list, zcl_version, power_source) \
    zb_bool_t device_enable_##attr_list = ZB_TRUE;                             \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_BASIC) \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID, (zcl_version))      \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID, (power_source))    \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_DEVICE_ENABLED_ID,                  \
                    &(device_enable_##attr_list))                              \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/** @endcond */

 /**
 *  @brief Declare attribute list for Basic cluster with HA additions.
 *  @param attr_list [IN] - attribute list name.
 *  @param zcl_version [IN] - pointer to variable storing ZCL version  attribute value.
 *  @param app_version [IN] - pointer to the variable storing application version.
 *  @param stack_version [IN] - pointer to the variable storing stack version.
 *  @param hardware_version [IN] - pointer to the variable storing hardware version.
 *  @param manufacturer_name [IN] - pointer to the variable storing manufacturer name.
 *  @param model_id [IN] - pointer to the variable storing model identifier.
 *  @param date_code [IN] - pointer to the variable storing date code.
 *  @param power_source [IN] - pointer to variable storing power source attribute value.
 *  @param location_id [IN] - pointer to variable storing location description attribute value.
 *  @param ph_env [IN] - pointer to variable storing physical environment attribute value.
 */
#define ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_FULL(                              \
  attr_list,                                                                        \
  zcl_version,                                                                      \
  app_version,                                                                      \
  stack_version,                                                                    \
  hardware_version,                                                                 \
  manufacturer_name,                                                                \
  model_id,                                                                         \
  date_code,                                                                        \
  power_source,                                                                     \
  location_id,                                                                      \
  ph_env)                                                                           \
    zb_bool_t device_enable_##attr_list = ZB_TRUE;                                  \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_BASIC)      \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID, (zcl_version))           \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_APPLICATION_VERSION_ID, (app_version))   \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_STACK_VERSION_ID, (stack_version))       \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_HW_VERSION_ID, (hardware_version))       \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID, (manufacturer_name))\
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID, (model_id))         \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_DATE_CODE_ID, (date_code))               \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID, (power_source))         \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_LOCATION_DESCRIPTION_ID, (location_id))  \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_PHYSICAL_ENVIRONMENT_ID, (ph_env))       \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_DEVICE_ENABLED_ID,                       \
                    &(device_enable_##attr_list))                                   \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */ /* Basic cluster internals */

/*! @} */ /* ZCL Basic cluster definitions */

zb_uint8_t zb_zcl_get_cmd_list_basic(zb_bool_t is_client_generated, zb_uint8_t **cmd_list);

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* ZB_ZCL_BASIC_HA_ADDS_H */
