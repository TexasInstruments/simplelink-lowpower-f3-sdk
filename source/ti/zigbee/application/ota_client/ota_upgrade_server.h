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
/* PURPOSE: HA OTA Upgrade server device definition
*/

#ifndef ZB_HA_OTA_UPGRADE_SERVER_DEVICE_H
#define ZB_HA_OTA_UPGRADE_SERVER_DEVICE_H 1

/**
 *    HA OTA Upgrade server device.
 *    OTA Upgrade server device has 2 clusters: \n
 *      - @ref ZB_ZCL_BASIC \n
 *      - @ref ZB_ZCL_OTA_UPGRADE_SERVER
 */

#define ZB_HA_DEVICE_VER_OTA_UPGRADE_SERVER         0       /**< Device version */
#define ZB_HA_OTA_UPGRADE_SERVER_DEVICE_ID         0xfff1   /**< Device ID */

#define ZB_HA_OTA_UPGRADE_SERVER_IN_CLUSTER_NUM    2 /**< OTA Upgrade server test input clusters number. */
#define ZB_HA_OTA_UPGRADE_SERVER_OUT_CLUSTER_NUM   0 /**< OTA Upgrade server test output clusters number. */

/**
 *  @brief Declare cluster list for OTA Upgrade server device.
 *  @param cluster_list_name [IN] - cluster list variable name.
 *  @param basic_attr_list [IN] - attribute list for Basic cluster.
 *  @param ota_upgrade_attr_list [OUT] - attribute list for OTA Upgrade server cluster.
 */
#define ZB_HA_DECLARE_OTA_UPGRADE_SERVER_CLUSTER_LIST(                \
  cluster_list_name,                                                  \
  basic_attr_list,                                                    \
  ota_upgrade_attr_list)                                              \
      zb_zcl_cluster_desc_t cluster_list_name[] =                     \
      {                                                               \
        ZB_ZCL_CLUSTER_DESC(                                          \
          ZB_ZCL_CLUSTER_ID_BASIC,                                    \
          ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),          \
          (basic_attr_list),                                          \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                 \
          ZB_ZCL_MANUF_CODE_INVALID                                   \
          ),                                                          \
        ZB_ZCL_CLUSTER_DESC(                                          \
          ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,                              \
          ZB_ZCL_ARRAY_SIZE(ota_upgrade_attr_list, zb_zcl_attr_t),    \
          (ota_upgrade_attr_list),                                    \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                 \
          ZB_ZCL_MANUF_CODE_INVALID                                   \
          )                                                           \
      }

/**
 *  @brief Declare simple descriptor for OTA Upgrade server device.
 *  @param ep_name - endpoint variable name.
 *  @param ep_id [IN] - endpoint ID.
 *  @param in_clust_num [IN] - number of supported input clusters.
 *  @param out_clust_num [IN] - number of supported output clusters.
 *  @note in_clust_num, out_clust_num should be defined by numeric constants, not variables or any
 *  definitions, because these values are used to form simple descriptor type name.
 */
#define ZB_ZCL_DECLARE_OTA_UPGRADE_SERVER_SIMPLE_DESC(ep_name, ep_id, in_clust_num, out_clust_num)   \
  ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);  \
  ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num)   \
    simple_desc_##ep_name =                             \
  {                                                     \
    ep_id,                                              \
    ZB_AF_HA_PROFILE_ID,                                \
    ZB_HA_OTA_UPGRADE_SERVER_DEVICE_ID,                 \
    ZB_HA_DEVICE_VER_OTA_UPGRADE_SERVER,                \
    0,                                                  \
    in_clust_num,                                       \
    out_clust_num,                                      \
    {                                                   \
      ZB_ZCL_CLUSTER_ID_BASIC,                          \
      ZB_ZCL_CLUSTER_ID_OTA_UPGRADE                     \
    }                                                   \
  }

/**
 *  @brief Declare endpoint for OTA Upgrade server device.
 *  @param ep_name [IN] - endpoint variable name.
 *  @param ep_id [IN] - endpoint ID.
 *  @param cluster_list [IN] - endpoint cluster list.
 */
#define ZB_HA_DECLARE_OTA_UPGRADE_SERVER_EP(ep_name, ep_id, cluster_list)     \
      ZB_ZCL_DECLARE_OTA_UPGRADE_SERVER_SIMPLE_DESC(                          \
          ep_name,                                                            \
          ep_id,                                                              \
          ZB_HA_OTA_UPGRADE_SERVER_IN_CLUSTER_NUM,                            \
          ZB_HA_OTA_UPGRADE_SERVER_OUT_CLUSTER_NUM);                          \
  ZB_AF_DECLARE_ENDPOINT_DESC(                                          \
    ep_name,                                                            \
            ep_id,                                                            \
            ZB_AF_HA_PROFILE_ID,                                              \
            0,                                                                \
            NULL,                                                             \
            ZB_ZCL_ARRAY_SIZE(                                                \
                cluster_list,                                                 \
                zb_zcl_cluster_desc_t),                                       \
            cluster_list,                                                     \
    (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name,                   \
    0, NULL, /* No reporting ctx */                                     \
    0, NULL)

#define ZB_HA_DECLARE_OTA_UPGRADE_SERVER_CTX(device_ctx, ep_name)       \
  ZBOSS_DECLARE_DEVICE_CTX_1_EP(device_ctx, ep_name)


#define ZB_ZCL_OTA_UPGRADE_FILE_SAMPLE  {                                                               \
      0x1e, 0xf1, 0xee, 0x0b, 0x00, 0x01, 0x3c, 0x00,  0x10, 0x00, 0xbb, 0xaa, 0xdd, 0xcc, 0x78, 0x56,  \
      0x34, 0x12, 0x02, 0x00, 0x54, 0x68, 0x65, 0x20,  0x6c, 0x61, 0x74, 0x65, 0x73, 0x74, 0x20, 0x61,  \
      0x6e, 0x64, 0x20, 0x67, 0x72, 0x65, 0x61, 0x74,  0x65, 0x73, 0x74, 0x20, 0x75, 0x70, 0x67, 0x72,  \
      0x61, 0x64, 0x65, 0x2e, 0x3c, 0x00, 0x00, 0x00,  0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,  \
      0x50, 0x00, 0x00, 0x00,                                                                           \
}

#endif /* ZB_HA_OTA_UPGRADE_SERVER_DEVICE_H */
