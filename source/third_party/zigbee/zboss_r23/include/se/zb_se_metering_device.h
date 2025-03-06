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
/* PURPOSE: Smart Energy metering device definition
*/

#ifndef ZB_SE_METERING_H
#define ZB_SE_METERING_H 1

#if defined ZB_SE_DEFINE_DEVICE_METERING || defined DOXYGEN

/** @cond DOXYGEN_SE_SECTION */

/**
 *  @defgroup se_metering_device Metering
 *  @ingroup se_devices
 *  @details The Metering end device is a meter (electricity, gas, water, heat, etc.) that is fitted
 *  with a Zigbee device. Depending on what is being metered, the device may be capable of immediate
 *  (requested) reads or it will autonomously send readings periodically. A Metering end device may
 *  also be capable of communicating certain status indicators (e.g. battery low, tamper detected).
 *
 *  @par Supported Clusters
 *  Metering Device has X clusters (see SE spec 1.4 subclauses 6.1, 6.3.2.1): \n
 *  <table>
 *    <tr>
 *      <th>Server roles</th> <th>Client roles</th>
 *    </tr>
 *    <tr>
 *      <td>@ref ZB_ZCL_BASIC</td> <td>@ref ZB_ZCL_KEC</td>
 *    </tr>
 *    <tr>
 *      <td>@ref ZB_ZCL_KEC</td> <td>@ref ZB_ZCL_KEEP_ALIVE <i>(optional)</i></td>
 *    </tr>
 *    <tr>
 *      <td>@ref ZB_ZCL_METERING</td> <td>@ref ZB_ZCL_TUNNELING <i>(optional)</i></td>
 *    </tr>
 *    <tr>
 *      <td>@ref ZB_ZCL_IDENTIFY <i>(optional)</i></td>  <td></td>
 *    </tr>
 *    <tr>
 *      <td>@ref ZB_ZCL_EVENTS <i>(optional)</i></td>  <td></td>
 *    </tr>
 *    <tr>
 *      <td>@ref ZB_ZCL_PREPAYMENT <i>(optional)</i></td>  <td></td>
 *    </tr>
 *  </table>
 *
 *  @par Example
 *  @n Declare clusters
 *  @snippet samples/se/metering/se_el_metering_zed.c DECLARE_CLUSTERS
 *  @n Register device context
 *  @snippet samples/se/metering/se_el_metering_zed.c REGISTER_DEVICE_CTX
 *  @n Signal handler example
 *  @snippet samples/se/metering/se_el_metering_zed.c SIGNAL_HANDLER
 *  @{
 */

/** @def ZB_SE_DEVICE_VER_METERING
 *  @brief Metering device version
 */
#define ZB_SE_DEVICE_VER_METERING          0

/** @cond internal */

/** @def ZB_SE_METERING_DEV_IN_CLUSTER_NUM
 *  @brief Metering IN clusters number
 */
#define ZB_SE_METERING_DEV_IN_CLUSTER_NUM  6

/** @def ZB_SE_METERING_DEV_OUT_CLUSTER_NUM
 *  @brief Metering OUT clusters number
 */
#define ZB_SE_METERING_DEV_OUT_CLUSTER_NUM 3

/** @def ZB_SE_METERING_DEV_CLUSTER_NUM
 *  @brief Number of clusters for Metering SE device
 */
#define ZB_SE_METERING_DEV_CLUSTER_NUM \
  (ZB_SE_METERING_DEV_IN_CLUSTER_NUM + ZB_SE_METERING_DEV_OUT_CLUSTER_NUM)

/** @def ZB_SE_METERING_DEV_REPORT_ATTR_COUNT
 *  @brief Number of attributes for reporting on Metering device
 */
#define ZB_SE_METERING_DEV_REPORT_ATTR_COUNT (ZB_ZCL_METERING_REPORT_ATTR_COUNT)

/** @endcond */

/* DV: Identify cluster was supported as an example. It is not required
      for SE Metering device and can be removed if extra.

   Zigbee-16-05033-011 SE spec, 6.3.2.1 Supported clusters:
   In addition to those specified in Table 6-1, the Metering Device shall support the clusters
   listed in Table 6-4. If a SE cluster is not listed ... in the following table or in the
   COMMON table, then that cluster shall be prohibited on a Metering device endpoint.

   Zigbee-16-05033-011 SE spec, 6.1 Common clusters: (see Table 6-1)
*/

#if defined ZB_SUBGHZ_BAND_ENABLED && defined ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ && !defined SNCP_MODE

#define ZB_ZCL_SUBGHZ_CLUSTER_DESC_CLIENT() \
  ZB_ZCL_CLUSTER_DESC(                               \
    ZB_ZCL_CLUSTER_ID_SUB_GHZ,                       \
    0, NULL,                                         \
    ZB_ZCL_CLUSTER_CLIENT_ROLE,                      \
    ZB_ZCL_MANUF_CODE_INVALID),

#define SUB_GHZ_CLUSTER_ENABLED_ID ZB_ZCL_CLUSTER_ID_SUB_GHZ,

#undef ZB_SE_METERING_DEV_OUT_CLUSTER_NUM
#define ZB_SE_METERING_DEV_OUT_CLUSTER_NUM  4

#else
#define SUB_GHZ_CLUSTER_ENABLED_ID
#define ZB_ZCL_SUBGHZ_CLUSTER_DESC_CLIENT()
#endif

/** @def ZB_SE_DECLARE_METERING_DEV_CLUSTER_LIST
 *  @brief Declare clusters list for Metering device.
 *  @param cluster_list_name - cluster list variable name
 *  @param basic_attr_list - attribute list for Basic cluster
 *  @param kec_attr_list - attribute list for Key Establishment cluster
 *  @param identify_attr_list - attribute list for Identify cluster
 *  @param metering_attr_list - attribute list for Metering cluster
 *  @param prepayment_attr_list - attribute list for Prepayment cluster
 */
#define ZB_SE_DECLARE_METERING_DEV_CLUSTER_LIST(cluster_list_name,    \
                                                basic_attr_list,      \
                                                kec_attr_list,        \
                                                identify_attr_list,   \
                                                metering_attr_list,   \
                                                prepayment_attr_list) \
  zb_zcl_cluster_desc_t cluster_list_name[] =                         \
  {                                                                   \
    ZB_ZCL_CLUSTER_DESC(                                              \
      ZB_ZCL_CLUSTER_ID_BASIC,                                        \
      ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),              \
      (basic_attr_list),                                              \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                     \
      ZB_ZCL_MANUF_CODE_INVALID                                       \
      ),                                                              \
    ZB_ZCL_CLUSTER_DESC(                                              \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                     \
      ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),           \
      (identify_attr_list),                                           \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                     \
      ZB_ZCL_MANUF_CODE_INVALID                                       \
      ),                                                              \
    ZB_ZCL_CLUSTER_DESC(                                              \
      ZB_ZCL_CLUSTER_ID_METERING,                                     \
      ZB_ZCL_ARRAY_SIZE(metering_attr_list, zb_zcl_attr_t),           \
      (metering_attr_list),                                           \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                     \
      ZB_ZCL_MANUF_CODE_INVALID                                       \
      ),                                                              \
    ZB_ZCL_CLUSTER_DESC(                                              \
      ZB_ZCL_CLUSTER_ID_EVENTS,                                       \
      0, NULL,                                                        \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                     \
      ZB_ZCL_MANUF_CODE_INVALID                                       \
      ),                                                              \
    ZB_ZCL_CLUSTER_DESC(                                              \
      ZB_ZCL_CLUSTER_ID_PREPAYMENT,                                   \
      ZB_ZCL_ARRAY_SIZE(prepayment_attr_list, zb_zcl_attr_t),         \
      (prepayment_attr_list),                                         \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                     \
      ZB_ZCL_MANUF_CODE_INVALID                                       \
      ),                                                              \
    ZB_ZCL_CLUSTER_DESC(                                              \
      ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT,                            \
      ZB_ZCL_ARRAY_SIZE(kec_attr_list, zb_zcl_attr_t),                \
      (kec_attr_list),                                                \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                     \
      ZB_ZCL_MANUF_CODE_INVALID                                       \
      ),                                                              \
    ZB_ZCL_CLUSTER_DESC(                                              \
      ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT,                            \
      ZB_ZCL_ARRAY_SIZE(kec_attr_list, zb_zcl_attr_t),                \
      (kec_attr_list),                                                \
      ZB_ZCL_CLUSTER_CLIENT_ROLE,                                     \
      ZB_ZCL_MANUF_CODE_INVALID                                       \
      ),                                                              \
    ZB_ZCL_CLUSTER_DESC(                                              \
      ZB_ZCL_CLUSTER_ID_KEEP_ALIVE,                                   \
      0, NULL,                                                        \
      ZB_ZCL_CLUSTER_CLIENT_ROLE,                                     \
      ZB_ZCL_MANUF_CODE_INVALID                                       \
      ),                                                              \
    ZB_ZCL_CLUSTER_DESC(                                              \
      ZB_ZCL_CLUSTER_ID_TUNNELING,                                    \
      0, NULL,                                                        \
      ZB_ZCL_CLUSTER_CLIENT_ROLE,                                     \
      ZB_ZCL_MANUF_CODE_INVALID                                       \
      ),                                                              \
    ZB_ZCL_SUBGHZ_CLUSTER_DESC_CLIENT()                               \
  }


/** @cond internals_doc */

/** @def ZB_ZCL_DECLARE_METERING_DEV_SIMPLE_DESC
 *  @brief Declare simple descriptor for Metering device
 *  @param ep_name - endpoint variable name
 *  @param ep_id - endpoint ID
 *  @param in_clust_num   - number of supported input clusters
 *  @param out_clust_num  - number of supported output clusters
 *  @note in_clust_num, out_clust_num should be defined by numeric constants, not variables or any
 *  definitions, because these values are used to form simple descriptor type name
 */
#define ZB_ZCL_DECLARE_METERING_DEV_SIMPLE_DESC(ep_name,                       \
                                                ep_id,                         \
                                                in_clust_num,                  \
                                                out_clust_num,                 \
                                                sub_ghz_token )                \
  ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);                         \
  ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num)  simple_desc_##ep_name = \
  {                                                                            \
    ep_id,                                                                     \
    ZB_AF_SE_PROFILE_ID,                                                       \
    ZB_SE_METERING_DEVICE_ID,                                                  \
    ZB_SE_DEVICE_VER_METERING,                                                 \
    0,                                                                         \
    in_clust_num,                                                              \
    out_clust_num,                                                             \
    {                                                                          \
      ZB_ZCL_CLUSTER_ID_BASIC,                                                 \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                              \
      ZB_ZCL_CLUSTER_ID_METERING,                                              \
      ZB_ZCL_CLUSTER_ID_EVENTS,                                                \
      ZB_ZCL_CLUSTER_ID_PREPAYMENT,                                            \
      ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT,                                     \
      ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT,                                     \
      ZB_ZCL_CLUSTER_ID_KEEP_ALIVE,                                            \
      ZB_ZCL_CLUSTER_ID_TUNNELING,                                             \
      sub_ghz_token                                                            \
    }                                                                          \
  }

/** @endcond */

/** @def ZB_SE_DECLARE_METERING_DEV_EP
 *  @brief Declare endpoint for Metering device
 *  @param ep_name - endpoint variable name
 *  @param ep_id - endpoint ID
 *  @param cluster_list - endpoint cluster list
 */
#define ZB_SE_DECLARE_METERING_DEV_EP(ep_name, ep_id, cluster_list)                 \
  ZB_ZCL_DECLARE_METERING_DEV_SIMPLE_DESC(ep_name,                                  \
                                          ep_id,                                    \
                                          ZB_SE_METERING_DEV_IN_CLUSTER_NUM,        \
                                          ZB_SE_METERING_DEV_OUT_CLUSTER_NUM,       \
                                          SUB_GHZ_CLUSTER_ENABLED_ID);              \
  ZBOSS_DEVICE_DECLARE_REPORTING_CTX(reporting_info## ep_name,                      \
                                     ZB_SE_METERING_DEV_REPORT_ATTR_COUNT);         \
  ZB_AF_DECLARE_ENDPOINT_DESC(ep_name,                                  \
                              ep_id,                                    \
                              ZB_AF_SE_PROFILE_ID,                      \
                              0,                                        \
                              NULL,                                     \
                              ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), \
                              cluster_list,                             \
                              (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name, \
                              ZB_SE_METERING_DEV_REPORT_ATTR_COUNT, reporting_info##ep_name, \
                              0, NULL)                                  \

/** @def ZB_SE_DECLARE_METERING_DEV_CTX
 *  @brief Declare Metering device context.
 *  @param device_ctx_name - device context variable name.
 *  @param ep_name - endpoint variable name.
 */
#define ZB_SE_DECLARE_METERING_DEV_CTX(device_ctx_name, ep_name)                                  \
  ZB_AF_START_DECLARE_ENDPOINT_LIST(ep_list_##device_ctx_name)                                  \
    &ep_name,                                                                                     \
  ZB_AF_FINISH_DECLARE_ENDPOINT_LIST;                                                             \
  ZBOSS_DECLARE_DEVICE_CTX(device_ctx_name, ep_list_##device_ctx_name,                            \
                           ZB_ZCL_ARRAY_SIZE(ep_list_##device_ctx_name, zb_af_endpoint_desc_t*))

/**  @} */

/** @endcond */ /* DOXYGEN_SE_SECTION */

#endif /* ZB_SE_DEFINE_DEVICE_METERING */

#endif /* ZB_SE_METERING_H */
