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
/* PURPOSE: Smart Energy Programmable Controlled Thermostat device definition
*/

#ifndef ZB_SE_PCT_H
#define ZB_SE_PCT_H 1

#if defined ZB_SE_DEFINE_DEVICE_PCT || defined DOXYGEN

/** @cond DOXYGEN_SE_SECTION */

/**
 *  @defgroup se_pct_device Programmable Communicating Thermostat (PCT) Device
 *  @ingroup se_devices
 *  @details The PCT device shall provide the capability to control the premises
 *  heating and cooling systems.
 *
 *  @par Supported clusters
 *  PCT Device has X clusters (see SE spec 1.4 subclauses 6.1, 6.3.4.1): \n
 *  <table>
 *    <tr>
 *      <th>Server roles</th> <th>Client roles</th>
 *    </tr>
 *    <tr>
 *      <td>@ref ZB_ZCL_BASIC </td> <td>@ref ZB_ZCL_KEC</td>
 *    </tr>
 *    <tr>
 *      <td>@ref ZB_ZCL_KEC</td> <td>@ref ZB_ZCL_METERING <i>(optional)</i></td>
 *    </tr>
 *    <tr>
 *      <td>@ref ZB_ZCL_ENERGY_MANAGEMENT <i>(optional)</i> </td> <td> @ref ZB_ZCL_MDU_PAIRING <i>(optional)</i> </td>
 *    </tr>
 *    <tr>
 *      <td> </td> <td> @ref ZB_ZCL_KEEP_ALIVE  <i>(optional)</i> </td>
 *    </tr>
 *    <tr>
 *      <td> </td> <td> @ref ZB_ZCL_PREPAYMENT <i>(optional)</i></td>
 *    </tr>
 *    <tr>
 *      <td> </td> <td> </td>
 *    </tr>
 *  </table>
 *
 *  @par Example
 *  @n Declare clusters
 *  @snippet samples/se/thermostat/se_pct_zed.c DECLARE_CLUSTERS
 *  @n Register device context
 *  @snippet samples/se/thermostat/se_pct_zed.c REGISTER_DEVICE_CTX
 *  @n Signal handler example
 *  @snippet samples/se/thermostat/se_pct_zed.c SIGNAL_HANDLER
 *  @{
 */

/** @def ZB_SE_DEVICE_VER_PCT
 *  @brief PCT device version
 */
#define ZB_SE_DEVICE_VER_PCT      0

/** @cond internal */

/** @def ZB_SE_PCT_IN_CLUSTER_NUM
 *  @brief PCT IN clusters number
 */
#define ZB_SE_PCT_IN_CLUSTER_NUM  3


/** @def ZB_SE_PCT_OUT_CLUSTER_NUM
 *  @brief PCT OUT clusters number
 */
#define ZB_SE_PCT_OUT_CLUSTER_NUM 4


/** @def ZB_SE_PCT_CLUSTER_NUM
 *  @brief Total number of clusters for In-Home Display SE device
 */
#define ZB_SE_PCT_CLUSTER_NUM \
  (ZB_SE_PCT_IN_CLUSTER_NUM + ZB_SE_PCT_OUT_CLUSTER_NUM)


/** @def ZB_SE_PCT_REPORT_ATTR_COUNT
 *  @brief Number of attributes for reporting on In-Home Display device
 */
#define ZB_SE_PCT_REPORT_ATTR_COUNT 0

/** @endcond */

#if defined ZB_SUBGHZ_BAND_ENABLED && defined ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ && !defined SNCP_MODE

#define ZB_ZCL_SUBGHZ_CLUSTER_DESC_CLIENT() \
  ZB_ZCL_CLUSTER_DESC(                               \
    ZB_ZCL_CLUSTER_ID_SUB_GHZ,                       \
    0, NULL,                                         \
    ZB_ZCL_CLUSTER_CLIENT_ROLE,                      \
    ZB_ZCL_MANUF_CODE_INVALID),

#define SUB_GHZ_CLUSTER_ENABLED_ID ZB_ZCL_CLUSTER_ID_SUB_GHZ,

#undef ZB_SE_PCT_OUT_CLUSTER_NUM
#define ZB_SE_PCT_OUT_CLUSTER_NUM  5

#else
#define SUB_GHZ_CLUSTER_ENABLED_ID
#define ZB_ZCL_SUBGHZ_CLUSTER_DESC_CLIENT()
#endif

/** @def ZB_SE_DECLARE_PCT_CLUSTER_LIST
 *  @brief Declare cluster list for In-Home Display device.
 *  @param cluster_list_name - cluster list variable name
 *  @param basic_attr_list - attribute list for Basic cluster
 *  @param kec_attr_list - attribute list for Key Establishment cluster
 *  @param energy_mgmt_attr_list - attribute list for Energy Management cluster
 */
#define ZB_SE_DECLARE_PCT_CLUSTER_LIST(cluster_list_name, \
                                       basic_attr_list,   \
                                       kec_attr_list,     \
                                       energy_mgmt_attr_list) \
  zb_zcl_cluster_desc_t cluster_list_name[] =             \
  {                                                       \
    ZB_ZCL_CLUSTER_DESC(                                  \
      ZB_ZCL_CLUSTER_ID_BASIC,                            \
      ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),  \
      (basic_attr_list),                                  \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                         \
      ZB_ZCL_MANUF_CODE_INVALID                           \
      ),                                                  \
    ZB_ZCL_CLUSTER_DESC(                                  \
      ZB_ZCL_CLUSTER_ID_ENERGY_MANAGEMENT,                \
      ZB_ZCL_ARRAY_SIZE(energy_mgmt_attr_list, zb_zcl_attr_t),                 \
      (energy_mgmt_attr_list),                                                 \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                         \
      ZB_ZCL_MANUF_CODE_INVALID                           \
      ),                                                  \
    ZB_ZCL_CLUSTER_DESC(                                  \
      ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT,                \
      ZB_ZCL_ARRAY_SIZE(kec_attr_list, zb_zcl_attr_t),    \
      (kec_attr_list),                                    \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                         \
      ZB_ZCL_MANUF_CODE_INVALID                           \
      ),                                                  \
    ZB_ZCL_CLUSTER_DESC(                                  \
      ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT,                \
      ZB_ZCL_ARRAY_SIZE(kec_attr_list, zb_zcl_attr_t),    \
      (kec_attr_list),                                    \
      ZB_ZCL_CLUSTER_CLIENT_ROLE,                         \
      ZB_ZCL_MANUF_CODE_INVALID                           \
      ),                                                  \
    ZB_ZCL_CLUSTER_DESC(                                  \
      ZB_ZCL_CLUSTER_ID_MDU_PAIRING,                      \
      0, NULL,                                            \
      ZB_ZCL_CLUSTER_CLIENT_ROLE,                         \
      ZB_ZCL_MANUF_CODE_INVALID                           \
    ),                                                    \
    ZB_ZCL_CLUSTER_DESC(                                  \
      ZB_ZCL_CLUSTER_ID_KEEP_ALIVE,                       \
      0, NULL,                                            \
      ZB_ZCL_CLUSTER_CLIENT_ROLE,                         \
      ZB_ZCL_MANUF_CODE_INVALID                           \
      ),                                                  \
    ZB_ZCL_CLUSTER_DESC(                                  \
      ZB_ZCL_CLUSTER_ID_PREPAYMENT,                       \
      0, NULL,                                            \
      ZB_ZCL_CLUSTER_CLIENT_ROLE,                         \
      ZB_ZCL_MANUF_CODE_INVALID                           \
      ),                                                  \
    ZB_ZCL_SUBGHZ_CLUSTER_DESC_CLIENT()                   \
  }

/** @cond internals_doc */

/** @def ZB_ZCL_DECLARE_PCT_SIMPLE_DESC
 *  @brief Declare simple descriptor for In-Home Display device
 *  @param ep_name - endpoint variable name
 *  @param ep_id - endpoint ID
 *  @param in_clust_num   - number of supported input clusters
 *  @param out_clust_num  - number of supported output clusters
 *
 *  @note in_clust_num, out_clust_num should be defined by numeric constants, not variables or any
 *  definitions, because these values are used to form simple descriptor type name
 */
#define ZB_ZCL_DECLARE_PCT_SIMPLE_DESC(ep_name,                                \
                                       ep_id,                                  \
                                       in_clust_num,                           \
                                       out_clust_num,                          \
                                       sub_ghz_token)                          \
  ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);                         \
  ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num)  simple_desc_##ep_name = \
  {                                                                            \
    ep_id,                                                                     \
    ZB_AF_SE_PROFILE_ID,                                                       \
    ZB_SE_PCT_DEVICE_ID,                                                       \
    ZB_SE_DEVICE_VER_PCT,                                                      \
    0,                                                                         \
    in_clust_num,                                                              \
    out_clust_num,                                                             \
    {                                                                          \
      ZB_ZCL_CLUSTER_ID_BASIC,                                                 \
      ZB_ZCL_CLUSTER_ID_ENERGY_MANAGEMENT,                                     \
      ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT,                                     \
      ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT,                                     \
      ZB_ZCL_CLUSTER_ID_MDU_PAIRING,                                           \
      ZB_ZCL_CLUSTER_ID_KEEP_ALIVE,                                            \
      ZB_ZCL_CLUSTER_ID_PREPAYMENT,                                            \
      sub_ghz_token                                                            \
    }                                                                          \
  }

/** @endcond */

/** @def ZB_SE_DECLARE_PCT_EP
 *  @brief Declare endpoint for In-Home Display device
 *  @param ep_name - endpoint variable name
 *  @param ep_id - endpoint ID
 *  @param cluster_list - endpoint cluster list
 */
#define ZB_SE_DECLARE_PCT_EP(ep_name, ep_id, cluster_list)                        \
  ZB_ZCL_DECLARE_PCT_SIMPLE_DESC(ep_name,                                         \
                                 ep_id,                                           \
                                 ZB_SE_PCT_IN_CLUSTER_NUM,                        \
                                 ZB_SE_PCT_OUT_CLUSTER_NUM,                       \
                                 SUB_GHZ_CLUSTER_ENABLED_ID);                     \
  ZB_AF_DECLARE_ENDPOINT_DESC(ep_name,                                  \
                              ep_id,                                    \
                          ZB_AF_SE_PROFILE_ID,                                    \
                          0,                                                      \
                          NULL,                                                   \
                          ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), \
                          cluster_list,                                           \
                              (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name, \
                              ZB_SE_PCT_REPORT_ATTR_COUNT, NULL,        \
                              0, NULL)

/** @def ZB_SE_DECLARE_PCT_CTX
 *  @brief Declares In-Home Display device context.
 *  @param device_ctx_name - device context variable name.
 *  @param ep_name - endpoint variable name.
 */
#define ZB_SE_DECLARE_PCT_CTX(device_ctx_name, ep_name)                       \
  ZBOSS_DECLARE_DEVICE_CTX_1_EP(device_ctx_name, ep_name)


/** @} */

/** @endcond */ /* DOXYGEN_SE_SECTION */

#endif /* ZB_SE_DEFINE_DEVICE_PCT */

#endif /* ZB_SE_PCT_H */
