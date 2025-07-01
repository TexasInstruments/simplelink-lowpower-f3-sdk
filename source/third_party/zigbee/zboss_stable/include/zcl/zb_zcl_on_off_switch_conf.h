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
/* PURPOSE: On/Off switch configuration definitions
*/

#ifndef ZB_ZCL_ON_OFF_SWITCH_CONF_H
#define ZB_ZCL_ON_OFF_SWITCH_CONF_H 1

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_OOSC
 *  @{
 *    @details
 *    On/Off Switch Configuration cluster has no cluster-specific command support. Cluster
 *    attributes can be queried with @ref ZB_ZCL_COMMANDS "common commands".
 *
 *    Sample switch config cluster attributes data
 *    @code
 *    zb_uint8_t attr_switch_type = ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_TYPE_TOGGLE;
 *    zb_uint8_t attr_switch_actions = ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_ACTIONS_DEFAULT_VALUE;
 *
 *    ZB_ZCL_DECLARE_ON_OFF_SWITCH_CONFIGURATION_ATTRIB_LIST(switch_cfg_attr_list, &attr_switch_type, &attr_switch_actions);
 *    @endcode
 *
 *
 */

/*! @name On/off switch configuration cluster attributes
    @{
*/

/*! @brief On/off switch configuration cluster attribute identifiers
    @see ZCL spec, subclause 3.9.2.2
*/
enum zb_zcl_on_off_switch_configuration_attr_e
{
  /*! Switch type attribute identifier (switch information) */
  ZB_ZCL_ATTR_ON_OFF_SWITCH_CONFIGURATION_SWITCH_TYPE_ID    = 0x0000,
  /*! Switch actions attribute identifier (switch settings) */
  ZB_ZCL_ATTR_ON_OFF_SWITCH_CONFIGURATION_SWITCH_ACTIONS_ID = 0x0010
};

/*! @brief Permissible values for switch type attribute
    @see ZCL spec, subclause 3.9.2.2.2
*/
enum zb_zcl_on_off_switch_configuration_switch_type_e
{
  /*! Toggle switch */
  ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_TYPE_TOGGLE        = 0,
  /*! Momentary switch */
  ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_TYPE_MOMENTARY     = 1,
  /*! Multifunction switch */
  ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_TYPE_MULTIFUNCTION = 2
};

/** @brief Default value for On/off switch configuration cluster revision global attribute */
#define ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0001u)

/*! @brief Maximal value for switch type attribute */
#define ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_TYPE_MAX_VALUE \
  (ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_TYPE_MULTIFUNCTION)

/* Switch type attribute has no default value */

/*! @brief Permissible values for switch actions attribute
    @see ZCL spec, subclause 3.9.2.2.3.1
*/
enum zb_zcl_on_off_switch_configuration_switch_actions_e
{
  /*! @brief First type command pattern

      @li Send "On" on arriving to State2 from State1;
      @li Send "Off" on arriving to State1 from State2.
  */
  ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_ACTIONS_TYPE1  = 0,
  /*! @brief Second type command pattern

      @li Send "On" on arriving to State1 from State2;
      @li Send "Off" on arriving to State2 from State1.
  */
  ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_ACTIONS_TYPE2  = 1,
  /*! @brief Toggle command pattern

      Send "Toggle" on state change
  */
  ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_ACTIONS_TOGGLE = 2
};

/*! @brief Maximal value for switch actions attribute */
#define ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_ACTIONS_MAX_VALUE \
  (ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_ACTIONS_TOGGLE)

/** @brief Default value for Switch actions attribute */
#define ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_ACTIONS_DEFAULT_VALUE \
  (ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_ACTIONS_TYPE1)

/*! @} */ /* On/off switch configuration cluster attributes */

/** @cond internals_doc
    @internal @name On/off switch configuration cluster attribute structures
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ON_OFF_SWITCH_CONFIGURATION_SWITCH_TYPE_ID(data_ptr)  \
  {                                                                       \
    ZB_ZCL_ATTR_ON_OFF_SWITCH_CONFIGURATION_SWITCH_TYPE_ID,               \
    ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                                           \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                         \
    (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                   \
    (void*) data_ptr                                                      \
  }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ON_OFF_SWITCH_CONFIGURATION_SWITCH_ACTIONS_ID(data_ptr) \
  {                                                                         \
    ZB_ZCL_ATTR_ON_OFF_SWITCH_CONFIGURATION_SWITCH_ACTIONS_ID,              \
    ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                                             \
    ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                          \
    (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                     \
    (void*) data_ptr                                                        \
  }

/** @internal Number of attributes mandatory for reporting on On/Off switch configuration cluster */
#define ZB_ZCL_ON_OFF_SWITCH_CONFIG_REPORT_ATTR_COUNT 0

/*! @}
 *  @endcond */ /* internals_doc */

/** @brief Declare attribute list for ON/OFF Switch configuration cluster
    @param attr_list - attribute list name
    @param switch_type - pointer to variable to store switch type attribute value
    @param switch_actions - pointer to variable to store switch action attribute value
*/
#define ZB_ZCL_DECLARE_ON_OFF_SWITCH_CONFIGURATION_ATTRIB_LIST(attr_list, switch_type, switch_actions) \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION)  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ON_OFF_SWITCH_CONFIGURATION_SWITCH_TYPE_ID, (switch_type))       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ON_OFF_SWITCH_CONFIGURATION_SWITCH_ACTIONS_ID, (switch_actions)) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

void zb_zcl_on_off_switch_config_init_server(void);
void zb_zcl_on_off_switch_config_init_client(void);
#define ZB_ZCL_CLUSTER_ID_ON_OFF_SWITCH_CONFIG_SERVER_ROLE_INIT zb_zcl_on_off_switch_config_init_server
#define ZB_ZCL_CLUSTER_ID_ON_OFF_SWITCH_CONFIG_CLIENT_ROLE_INIT zb_zcl_on_off_switch_config_init_client

#endif /* ZB_ZCL_ON_OFF_SWITCH_CONF_H */
