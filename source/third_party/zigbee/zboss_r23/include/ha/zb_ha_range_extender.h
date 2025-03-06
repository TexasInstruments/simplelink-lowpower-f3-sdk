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
/* PURPOSE: Range extender device definition
*/

#ifndef ZB_HA_RANGE_EXTENDER_H
#define ZB_HA_RANGE_EXTENDER_H 1

#if defined ZB_HA_DEFINE_DEVICE_RANGE_EXTENDER || defined DOXYGEN

/** @cond DOXYGEN_HA_SECTION */

/**
 *  @defgroup ha_range_extender Range Extender
 *  @ingroup ZB_HA_DEVICES
    @{
    @details
    Range Extender device has 2 clusters (see spec 7.4.8): \n
        - @ref ZB_ZCL_IDENTIFY \n
        - @ref ZB_ZCL_BASIC

    Range Extender device sample

    @par Example

    - Declaring clusters:

        @snippet HA/common/zcl_basic_attr_list.h BASIC_CLUSTER_COMMON
        @snippet HA/range_extender/sample_zc.c COMMON_DECLARATION

    - Registering device list:

        @snippet HA/range_extender/sample_zc.c REGISTER

    - Example of command handler:

        @snippet HA/range_extender/sample_zc.c COMMAND_HANDLER
        @snippet HA/range_extender/sample_zc.c ZCL_COMMAND_HANDLER

    @par
*/

#define ZB_HA_DEVICE_VER_RANGE_EXTENDER 0  /*!< Range extender device version */

/** @cond internals_doc */
#define ZB_HA_RANGE_EXTENDER_IN_CLUSTER_NUM 2   /*!< Range extender IN (server) clusters
                                                  number */
#define ZB_HA_RANGE_EXTENDER_OUT_CLUSTER_NUM 0  /*!< Range extender OUT (client) clusters
                                                  number */

#define ZB_HA_RANGE_EXTENDER_CLUSTER_NUM                                       \
  (ZB_HA_RANGE_EXTENDER_IN_CLUSTER_NUM + ZB_HA_RANGE_EXTENDER_OUT_CLUSTER_NUM)

/** Number of attribute for reporting on Range extender device */
#define ZB_HA_RANGE_EXTENDER_REPORT_ATTR_COUNT    0

/** @endcond */

/** @brief Declare cluster list for Range extender device
    @param cluster_list_name - cluster list variable name
    @param basic_attr_list - attribute list for Basic cluster
    @param identify_attr_list - attribute list for Identify cluster
 */
#define ZB_HA_DECLARE_RANGE_EXTENDER_CLUSTER_LIST(        \
    cluster_list_name,                                    \
    basic_attr_list,                                      \
    identify_attr_list)                                   \
zb_zcl_cluster_desc_t cluster_list_name[] =               \
{                                                         \
  ZB_ZCL_CLUSTER_DESC(                                    \
    ZB_ZCL_CLUSTER_ID_IDENTIFY,                           \
    ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t), \
    (identify_attr_list),                                 \
    ZB_ZCL_CLUSTER_SERVER_ROLE,                           \
    ZB_ZCL_MANUF_CODE_INVALID                             \
  ),                                                      \
  ZB_ZCL_CLUSTER_DESC(                                    \
    ZB_ZCL_CLUSTER_ID_BASIC,                              \
    ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),    \
    (basic_attr_list),                                    \
    ZB_ZCL_CLUSTER_SERVER_ROLE,                           \
    ZB_ZCL_MANUF_CODE_INVALID                             \
  )                                                       \
}

/** @cond internals_doc */

/** @brief Declare simple descriptor for Range extender device
    @param ep_name - endpoint variable name
    @param ep_id - endpoint ID
    @param in_clust_num - number of supported input clusters
    @param out_clust_num - number of supported output clusters
*/
#define ZB_ZCL_DECLARE_RANGE_EXTENDER_SIMPLE_DESC(ep_name, ep_id, in_clust_num, out_clust_num) \
  ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);                                         \
  ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num) simple_desc_##ep_name =                  \
  {                                                                                            \
    ep_id,                                                                                     \
    ZB_AF_HA_PROFILE_ID,                                                                       \
    ZB_HA_RANGE_EXTENDER_DEVICE_ID,                                                            \
    ZB_HA_DEVICE_VER_RANGE_EXTENDER,                                                           \
    0,                                                                                         \
    in_clust_num,                                                                              \
    out_clust_num,                                                                             \
    {                                                                                          \
      ZB_ZCL_CLUSTER_ID_BASIC,                                                                 \
      ZB_ZCL_CLUSTER_ID_IDENTIFY                                                               \
    }                                                                                          \
  }

/** @endcond */ /* internals_doc */

/** @brief Declare endpoint for Range extender device
    @param ep_name - endpoint variable name
    @param ep_id - endpoint ID
    @param cluster_list - endpoint cluster list
 */
#define ZB_HA_DECLARE_RANGE_EXTENDER_EP(ep_name, ep_id, cluster_list)                           \
  ZB_ZCL_DECLARE_RANGE_EXTENDER_SIMPLE_DESC(ep_name, ep_id,                                     \
      ZB_HA_RANGE_EXTENDER_IN_CLUSTER_NUM, ZB_HA_RANGE_EXTENDER_OUT_CLUSTER_NUM);               \
  ZB_AF_DECLARE_ENDPOINT_DESC(ep_name, ep_id, ZB_AF_HA_PROFILE_ID, 0, NULL,                              \
                          ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), cluster_list, \
                              (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name,                 \
                              0, NULL, /* No reporting ctx */                                   \
                              0, NULL) /* No CVC ctx */


/** @brief Declare Range Extender device context.
    @param device_ctx - device context variable name.
    @param ep_name - endpoint variable name.
*/
#define ZB_HA_DECLARE_RANGE_EXTENDER_CTX(device_ctx, ep_name)           \
  ZBOSS_DECLARE_DEVICE_CTX_1_EP(device_ctx, ep_name)

/*! @} */

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* ZB_HA_DEFINE_DEVICE_RANGE_EXTENDER */

#endif /* ZB_HA_RANGE_EXTENDER_H */
