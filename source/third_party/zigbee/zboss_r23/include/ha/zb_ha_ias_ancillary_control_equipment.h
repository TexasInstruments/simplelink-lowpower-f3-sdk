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
/* PURPOSE: IAS Ancillary Control Equipment device definition
*/

#ifndef ZB_HA_IAS_ANCILLARY_H
#define ZB_HA_IAS_ANCILLARY_H 1

#if defined ZB_HA_DEFINE_DEVICE_IAS_ANCILLARY_CONTROL_EQUIPMENT || defined DOXYGEN

/** @cond DOXYGEN_HA_SECTION */

/**
 *  @defgroup ZB_HA_IAS_ANCILLARY IAS Ancillary Control Equipment
 *  @ingroup ZB_HA_DEVICES
 *  @{
    @details
    IAS Ancillary Control Equipment has 5 clusters (see spec 7.8.2): \n
        - @ref ZB_ZCL_BASIC \n
        - @ref ZB_ZCL_IDENTIFY \n
        - @ref ZB_ZCL_IAS_ZONE \n
        - @ref ZB_ZCL_IAS_ACE \n
        - @ref ZB_ZCL_IDENTIFY

    IAS Ancillary Control Equipment sample

    @par Example

    - Declaring clusters:

        @snippet HA/common/zcl_basic_attr_list.h BASIC_CLUSTER_COMMON
        @snippet HA/common/zcl_ias_zone_attr_list.h IAS_ZONE_CLUSTER_COMMON
        @snippet HA/ias_ancillary_control_equipment/sample_zc.c COMMON_DECLARATION

    - Registering device list:

        @snippet HA/ias_ancillary_control_equipment/sample_zc.c REGISTER

    - Example of command handler:

        @snippet HA/ias_ancillary_control_equipment/sample_zc.c COMMAND_HANDLER
        @snippet HA/ias_ancillary_control_equipment/sample_zc.c ZCL_COMMAND_HANDLER

    @par
*/

#define ZB_HA_IAS_ANCILLARY_CONTROL_EQUIPMENT_VER 0  /*!< IAS ACE device version */

/** @cond internal */

/** Input clusters number. */
#define ZB_HA_IAS_ANCILLARY_CONTROL_EQUIPMENT_IN_CLUSTER_NUM  3

/** Output clusters number. */
#define ZB_HA_IAS_ANCILLARY_CONTROL_EQUIPMENT_OUT_CLUSTER_NUM 2

/*! Number of attribute for reporting on IAS Ancillary Control Equipment device */
#define ZB_HA_IAS_ANCILLARY_CONTROL_EQUIPMENT_REPORT_ATTR_COUNT \
  (ZB_ZCL_IAS_ZONE_REPORT_ATTR_COUNT)

/** @endcond */

/**
 *  @brief Declare cluster list for IAS Ancillary Control Equipment
 *  @param cluster_list_name [IN] - cluster list variable name.
 *  @param basic_attr_list [IN] - attribute list for Basic cluster.
 *  @param identify_attr_list [IN] - attribute list for Identify cluster.
 *  @param ias_zone_attr_list [IN] - attribute list for IAS Zone cluster
 */
#define ZB_HA_DECLARE_IAS_ANCILLARY_CONTROL_EQUIPMENT_CLUSTER_LIST(            \
  cluster_list_name,                                                           \
  basic_attr_list,                                                             \
  identify_attr_list,                                                          \
  ias_zone_attr_list)                                                          \
      zb_zcl_cluster_desc_t cluster_list_name[] =                              \
      {                                                                        \
        ZB_ZCL_CLUSTER_DESC(                                                   \
          ZB_ZCL_CLUSTER_ID_BASIC,                                             \
          ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),                   \
          (basic_attr_list),                                                   \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                          \
          ZB_ZCL_MANUF_CODE_INVALID                                            \
        ),                                                                     \
        ZB_ZCL_CLUSTER_DESC(                                                   \
          ZB_ZCL_CLUSTER_ID_IDENTIFY,                                          \
          0,                                                            \
          NULL,                                                         \
          ZB_ZCL_CLUSTER_CLIENT_ROLE,                                   \
          ZB_ZCL_MANUF_CODE_INVALID                                     \
          ),                                                            \
        ZB_ZCL_CLUSTER_DESC(                                            \
          ZB_ZCL_CLUSTER_ID_IDENTIFY,                                   \
          ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),                \
          (identify_attr_list),                                                \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_MANUF_CODE_INVALID                                            \
        ),                                                                     \
        ZB_ZCL_CLUSTER_DESC(                                                   \
          ZB_ZCL_CLUSTER_ID_IAS_ZONE,                                          \
          ZB_ZCL_ARRAY_SIZE(ias_zone_attr_list, zb_zcl_attr_t),                \
          (ias_zone_attr_list),                                                \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                          \
          ZB_ZCL_MANUF_CODE_INVALID                                            \
        ),                                                                     \
        ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_IAS_ACE,  0, NULL, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_MANUF_CODE_INVALID ) \
      }

/**
 *  @brief Declare simple descriptor for IAS Ancillary Control Equipment
 *  @param ep_name - endpoint variable name.
 *  @param ep_id [IN] - endpoint ID.
 *  @param in_clust_num [IN] - number of supported input clusters.
 *  @param out_clust_num [IN] - number of supported output clusters.
 *  @note in_clust_num, out_clust_num should be defined by numeric constants, not variables or any
 *  definitions, because these values are used to form simple descriptor type name.
 */
#define ZB_HA_DECLARE_IAS_ANCILLARY_CONTROL_EQUIPMENT_SIMPLE_DESC(                 \
  ep_name, ep_id, in_clust_num, out_clust_num)                                     \
      ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);                         \
      ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num)  simple_desc_##ep_name = \
      {                                                                            \
        ep_id,                                                                     \
        ZB_AF_ZLL_PROFILE_ID,                                                      \
        ZB_HA_IAS_CONTROL_INDICATING_EQUIPMENT_ID,                                 \
        ZB_HA_IAS_ANCILLARY_CONTROL_EQUIPMENT_VER,                                 \
        0,                                                                         \
        in_clust_num,                                                              \
        out_clust_num,                                                             \
        {                                                                          \
          ZB_ZCL_CLUSTER_ID_BASIC,                                                 \
          ZB_ZCL_CLUSTER_ID_IDENTIFY,                                              \
          ZB_ZCL_CLUSTER_ID_IAS_ZONE,                                              \
          ZB_ZCL_CLUSTER_ID_IAS_ACE,                                               \
          ZB_ZCL_CLUSTER_ID_IDENTIFY,                                              \
        }                                                                          \
      }

/**
 *  @brief Declare endpoint for IAS Ancillary Control Equipment
 *  @param ep_name [IN] - endpoint variable name.
 *  @param ep_id [IN] - endpoint ID.
 *  @param cluster_list [IN] - endpoint cluster list.
 */
#define ZB_HA_DECLARE_IAS_ANCILLARY_CONTROL_EQUIPMENT_EP(ep_name, ep_id, cluster_list)      \
      ZB_HA_DECLARE_IAS_ANCILLARY_CONTROL_EQUIPMENT_SIMPLE_DESC(                    \
          ep_name,                                                                  \
          ep_id,                                                                    \
          ZB_HA_IAS_ANCILLARY_CONTROL_EQUIPMENT_IN_CLUSTER_NUM,                     \
          ZB_HA_IAS_ANCILLARY_CONTROL_EQUIPMENT_OUT_CLUSTER_NUM);                   \
      ZBOSS_DEVICE_DECLARE_REPORTING_CTX(reporting_info## ep_name,                  \
          ZB_HA_IAS_ANCILLARY_CONTROL_EQUIPMENT_REPORT_ATTR_COUNT);                 \
      ZB_AF_DECLARE_ENDPOINT_DESC(                                                  \
        ep_name,                                                                    \
            ep_id,                                                                  \
            ZB_AF_HA_PROFILE_ID,                                                    \
            0,                                                                      \
            NULL,                                                                   \
            ZB_ZCL_ARRAY_SIZE(                                                      \
                cluster_list,                                                       \
                zb_zcl_cluster_desc_t),                                             \
            cluster_list,                                                           \
        (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name,                           \
        ZB_HA_IAS_ANCILLARY_CONTROL_EQUIPMENT_REPORT_ATTR_COUNT, reporting_info## ep_name, 0, NULL)

/**
   @brief Declare application's device context for Ancillary Control Equipment
   @param device_ctx - device context variable
   @param ep_name - endpoint variable name
*/
#define ZB_HA_DECLARE_IAS_ANCILLARY_CONTROL_EQUIPMENT_CTX(device_ctx, ep_name)      \
  ZBOSS_DECLARE_DEVICE_CTX_1_EP(device_ctx, ep_name)

/**
 * @}
 */

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* defined ZB_HA_DEFINE_DEVICE_IAS_ANCILLARY_CONTROL_EQUIPMENT || defined DOXYGEN */

#endif /* ZB_HA_IAS_ANCILLARY_H */
