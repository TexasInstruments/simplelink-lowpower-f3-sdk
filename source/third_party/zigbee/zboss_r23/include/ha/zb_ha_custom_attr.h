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
/* PURPOSE: Window Covering device definition
*/

#ifndef ZB_HA_CUSTOM_ATTR_H
#define ZB_HA_CUSTOM_ATTR_H 1

/******************* Specific definitions for window covering device **************************/

#if defined ZB_HA_DEFINE_DEVICE_WINDOW_COVERING || defined DOXYGEN

/** @cond (DOXYGEN_HA_SECTION && internals_doc)*/

/** @addtogroup ha_api
 * @{
 */

#define ZB_HA_DEVICE_VER_CUSTOM_ATTR 0

#define ZB_HA_CUSTOM_ATTR_IN_CLUSTER_NUM 6
#define ZB_HA_CUSTOM_ATTR_OUT_CLUSTER_NUM 0

#define ZB_HA_CUSTOM_ATTR_CLUSTER_NUM (ZB_HA_CUSTOM_ATTR_IN_CLUSTER_NUM + \
  ZB_HA_CUSTOM_ATTR_OUT_CLUSTER_NUM)

/*! Number of attribute for reporting on custom attr device */
#define ZB_HA_CUSTOM_ATTR_REPORT_ATTR_COUNT         \
  (ZB_ZCL_CUSTOM_ATTR_REPORT_ATTR_COUNT)

/*!
  @brief Declare cluster list for custom attr device
  @param cluster_list_name - cluster list variable name
  @param basic_attr_list - attribute list for Basic cluster
  @param identify_attr_list - attribute list for Identify cluster
  @param groups_attr_list - attribute list for Groups cluster
  @param scenes_attr_list - attribute list for Scenes cluster
  @param window_covering_attr_list - attribute list for Window covering cluster
  @param custom_attr_attr_list - attribute list for Custom Attributes cluster
 */
#define ZB_HA_DECLARE_CUSTOM_ATTR_CLUSTER_LIST(                     \
  cluster_list_name,                                                    \
  basic_attr_list,                                                      \
  identify_attr_list,                                                   \
  groups_attr_list,                                                     \
  scenes_attr_list,                                                     \
  window_covering_attr_list,                                            \
  custom_attr_attr_list)                                                \
  zb_zcl_cluster_desc_t cluster_list_name[] =                           \
  {                                                                     \
    ZB_ZCL_CLUSTER_DESC(                                                \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                       \
      ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),             \
      (identify_attr_list),                                             \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                       \
      ZB_ZCL_MANUF_CODE_INVALID                                         \
    ),                                                                  \
    ZB_ZCL_CLUSTER_DESC(                                                \
      ZB_ZCL_CLUSTER_ID_BASIC,                                          \
      ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),                \
      (basic_attr_list),                                                \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                       \
      ZB_ZCL_MANUF_CODE_INVALID                                         \
    ),                                                                  \
    ZB_ZCL_CLUSTER_DESC(                                                \
      ZB_ZCL_CLUSTER_ID_WINDOW_COVERING,                                \
      ZB_ZCL_ARRAY_SIZE(window_covering_attr_list, zb_zcl_attr_t),      \
      (window_covering_attr_list),                                      \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                       \
      ZB_ZCL_MANUF_CODE_INVALID                                         \
    ),                                                                  \
    ZB_ZCL_CLUSTER_DESC(                                                \
      ZB_ZCL_CLUSTER_ID_SCENES,                                         \
      ZB_ZCL_ARRAY_SIZE(scenes_attr_list, zb_zcl_attr_t),               \
      (scenes_attr_list),                                               \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                       \
      ZB_ZCL_MANUF_CODE_INVALID                                         \
    ),                                                                  \
    ZB_ZCL_CLUSTER_DESC(                                                \
      ZB_ZCL_CLUSTER_ID_GROUPS,                                         \
      ZB_ZCL_ARRAY_SIZE(groups_attr_list, zb_zcl_attr_t),               \
      (groups_attr_list),                                               \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                       \
      ZB_ZCL_MANUF_CODE_INVALID                                         \
    ),                                                                  \
    ZB_ZCL_CLUSTER_DESC(                                                \
      ZB_ZCL_CLUSTER_ID_CUSTOM_ATTR,                                    \
      ZB_ZCL_ARRAY_SIZE(custom_attr_attr_list, zb_zcl_attr_t),          \
      (custom_attr_attr_list),                                          \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                       \
      ZB_ZCL_MANUF_CODE_INVALID                                         \
    )                                                                   \
  }


/*!
  @brief Declare simple descriptor for custom attr device
  @param ep_name - endpoint variable name
  @param ep_id - endpoint ID
  @param in_clust_num - number of supported input clusters
  @param out_clust_num - number of supported output clusters
*/
#define ZB_ZCL_DECLARE_CUSTOM_ATTR_SIMPLE_DESC(ep_name, ep_id, in_clust_num, out_clust_num)     \
  ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);                                          \
  ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num) simple_desc_##ep_name =                   \
  {                                                                                             \
    ep_id,                                                                                      \
    ZB_AF_HA_PROFILE_ID,                                                                        \
    ZB_HA_CUSTOM_ATTR_DEVICE_ID,                                                                \
    ZB_HA_DEVICE_VER_CUSTOM_ATTR,                                                               \
    0,                                                                                          \
    in_clust_num,                                                                               \
    out_clust_num,                                                                              \
    {                                                                                           \
      ZB_ZCL_CLUSTER_ID_BASIC,                                                                  \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                                               \
      ZB_ZCL_CLUSTER_ID_WINDOW_COVERING,                                                        \
	    ZB_ZCL_CLUSTER_ID_SCENES,                                                                 \
      ZB_ZCL_CLUSTER_ID_GROUPS,                                                                 \
      ZB_ZCL_CLUSTER_ID_CUSTOM_ATTR                                                             \
    }                                                                                           \
  }

/*!
  @brief Declare endpoint for custom attr device
  @param ep_name - endpoint variable name
  @param ep_id - endpoint ID
  @param cluster_list - endpoint cluster list
 */
#define ZB_HA_DECLARE_CUSTOM_ATTR_EP(ep_name, ep_id, cluster_list)        \
  ZB_ZCL_DECLARE_CUSTOM_ATTR_SIMPLE_DESC(ep_name, ep_id,                  \
    ZB_HA_CUSTOM_ATTR_IN_CLUSTER_NUM, ZB_HA_CUSTOM_ATTR_OUT_CLUSTER_NUM); \
  ZBOSS_DEVICE_DECLARE_REPORTING_CTX(reporting_info## ep_name, 20);  \
  ZB_AF_DECLARE_ENDPOINT_DESC(ep_name, ep_id, ZB_AF_HA_PROFILE_ID,                    \
    0,                                                                    \
    NULL,                                                                 \
    ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), cluster_list, \
                          (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name,  \
                          ZB_ZCL_ARRAY_SIZE(ep_name, zb_af_endpoint_desc_t), \
                          20, reporting_info## ep_name, 0, NULL)


#define ZB_HA_DECLARE_CUSTOM_ATTR_CTX(device_ctx, ep_name)              \
  ZBOSS_DECLARE_DEVICE_CTX_1_EP(device_ctx, ep_name)

/*! @} */

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* ZB_HA_DEFINE_DEVICE_CUSTOM_ATTR */

#endif /* ZB_HA_CUSTOM_ATTR_H */
