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
/* PURPOSE: common definitions for Zigbee Smart Energy profile
*/

#ifndef ZBOSS_API_ZSE_H
#define ZBOSS_API_ZSE_H 1

#include "zb_address.h"
#include "zb_types.h"
#include "se/zb_se_device_config.h"
#include "zcl/zb_zcl_config.h"
#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

#include "se/zb_se_config.h"
#include "zcl/zb_zcl_keep_alive.h"
#if 0
#include "se/zb_se_metering_device.h"
#include "se/zb_se_metering_load_control.h"
#include "se/zb_se_in_home_display.h"
#include "se/zb_se_pct_device.h"
#include "se/zb_se_energy_service_interface.h"
#endif


/** @cond DOXYGEN_SE_SECTION */

/** @addtogroup ZB_ZCL_KEC
 *  @{
 */

/** @brief Key Establishment cluster's attributes IDs
 *
 *  The Information attribute set contains the attributes summarized in table below
 * <table>
 *  <caption> Key Establishment Attribute Sets </caption>
 *   <tr>
 *     <th> Identifier </th>
 *     <th> Name </th>
 *     <th> Type </th>
 *     <th> Range </th>
 *     <th> Access </th>
 *     <th> Default </th>
 *   </tr>
 *   <tr>
 *     <td> 0x0000 </td>
 *     <td> KeyEstablishmentSuite </td>
 *     <td> 16-bit Enumeration </td>
 *     <td> 0x0000-0xFFFF </td>
 *     <td> Readonly </td>
 *     <td> 0x0000 </td>
 *   </tr>
 * </table>
 * @see SE spec, C.3.1.2.2.1
 */
#define ZB_ZCL_ATTR_KEY_ESTABLISHMENT_SUITE_ID 0x0000U   /**< KeyEstablishmentSuite attribute */

/** @brief Default value for Key Establishment cluster revision global attribute */
#define ZB_ZCL_KEY_ESTABLISHMENT_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0002u)

/**
 * @name KeyEstablishmentSuite attribute values
 * @anchor kec_key_suite
 * @brief Table Values of the KeyEstablishmentSuite Attribute (Table C-4)
 */
/** @{ */
#define KEC_CS1 (1U << 0) /*!< Certificate-based Key Establishment Cryptographic Suite 1 (Crypto Suite 1)*/
#define KEC_CS2 (1U << 1) /*!< Certificate-based Key Establishment Cryptographic Suite 2 (Crypto Suite 2)*/
/** @} */

/**
 * @brief Type for KeyEstablishmentSuite attribute values.
 *
 * @deprecated holds one of @ref kec_key_suite. Kept only for backward compatibility as
 * @ref kec_key_suite were declared previously as enum. Can be removed in future releases.
 */
typedef zb_uint8_t zb_kec_key_suite_t;


/** @def ZB_KEC_SUPPORTED_CRYPTO_ATTR
 *  @brief Attribute value const (supported CryptoSuites)
 */
#define ZB_KEC_SUPPORTED_CRYPTO_ATTR (KEC_CS1 | KEC_CS2)

/** @cond internals_doc */

/** @brief Declare attribute list for Key Establishment cluster
 *  @param[in]  attr_list - attribute list variable name
 *  @param[in]  kec_key_establishment_suite - pointer to variable to store KeyEstablishmentSuite
 *              value
 */
#define ZB_ZCL_DECLARE_KEC_ATTRIB_LIST(attr_list, kec_key_establishment_suite)                     \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION_STATIC(attr_list, ZB_ZCL_KEY_ESTABLISHMENT)    \
  ZB_ZCL_SET_ATTR_DESC_M(ZB_ZCL_ATTR_KEY_ESTABLISHMENT_SUITE_ID, (kec_key_establishment_suite),    \
                         ZB_ZCL_ATTR_TYPE_16BIT_ENUM, ZB_ZCL_ATTR_ACCESS_READ_ONLY)                \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/** @endcond */
/**
 *  @brief Key Establishment cluster attributes
 */
 typedef struct zb_zcl_kec_attrs_s
 {
   /** @copydoc ZB_ZCL_ATTR_KEY_ESTABLISHMENT_SUITE_ID
    * @see ZB_ZCL_ATTR_KEY_ESTABLISHMENT_SUITE_ID
    */
   zb_uint16_t kec_suite;
 } zb_zcl_kec_attrs_t;


 /** @brief Declare attribute list for Key Establishment cluster
 *  @param[in]  attr_list - attribute list variable name
 *  @param[in]  attrs - pointer to @ref zb_zcl_kec_attrs_s structure
 */
#define ZB_ZCL_DECLARE_KEC_ATTR_LIST(attr_list, attrs)  \
  ZB_ZCL_DECLARE_KEC_ATTRIB_LIST(attr_list, &attrs.kec_suite)

/** @} */ /* ZB_ZCL_KEC */

/** @cond internals_doc */
/** Internal handler for Key Establishment Cluster commands */

void zb_zcl_kec_init_server(void);
void zb_zcl_kec_init_client(void);
#define ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT_SERVER_ROLE_INIT zb_zcl_kec_init_server
#define ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT_CLIENT_ROLE_INIT zb_zcl_kec_init_client

/** @fn void zb_kec_init()
 *  @brief Setup specific cluster initialization
 */
void zb_kec_init(void);

/** @endcond */ /* internals_doc */


/** @addtogroup se_secur
 *  @{
 */

/** @fn zb_uint8_t zb_se_load_ecc_cert(zb_uint16_t suite, zb_uint8_t *ca_public_key, zb_uint8_t *certificate, zb_uint8_t *private_key)
 *  @brief Loads device's certificate to NVRAM
 *  @param[in]  suite - CryptoSuite ID (@ref kec_key_suite)
 *  @param[in]  ca_public_key - buffer with Certification Authority's public key
 *  @param[in]  certificate - buffer with device's certificate
 *  @param[in]  private_key - buffer with device's private key
 *  @note This function is designed mainly for Trust Center devices as an additional method of
 *    adding certificates from several CAs.
 *  @par Example
 *  @snippet se/energy_service_interface/se_esi_zc_debug.c SIGNAL_HANDLER_LOAD_CERT
 *  @see zb_se_retry_cbke_with_tc
 *  @see ZB_SE_SIGNAL_CBKE_FAILED
 */
zb_ret_t zb_se_load_ecc_cert(zb_uint16_t suite,
                             zb_uint8_t *ca_public_key,
                             zb_uint8_t *certificate,
                             zb_uint8_t *private_key);


/** @fn zb_uint8_t zb_se_erase_ecc_cert(zb_uint8_t suite_no, zb_uint8_t *issuer, zb_uint8_t *subject)
 *  @brief Erases device's certificate from NVRAM
 *  @param[in]  suite_no - CryptoSuite #
 *  @param[in]  issuer - buffer with certificate's issuer
 *  @param[in]  subject - buffer MAC address (IEEE 802.15.4)
 *  @note This function is designed mainly for Trust Center devices as for purpose of erasing
 *    certificates from NVRAM by suite, issuer and subject(MAC address).
 */
zb_ret_t zb_se_erase_ecc_cert(zb_uint8_t suite_no,
                              zb_uint8_t *issuer,
                              zb_uint8_t *subject);

#ifdef ZB_SE_COMMISSIONING
/** @fn void zb_se_retry_cbke_with_tc(zb_uint8_t param)
 *  @brief Allows to retry CBKE procedure with Trust Center
 *  @param[in]  param - reference to the buffer which will be used for outgoing Match Descriptor
 *                      Request command or 0 if buffer should be allocated.
 *  @details This procedure should be used if application got @ref ZB_SE_SIGNAL_CBKE_FAILED signal
 *    from stack to repeat the CBKE using another certificate which should be loaded before.
 *  @see zb_se_load_ecc_cert - load certificate
 *  @see ZB_SE_SIGNAL_CBKE_FAILED - sample code of ZBOSS signal processing
 */
void zb_se_retry_cbke_with_tc(zb_uint8_t param);
#endif /* ZB_SE_COMMISSIONING */

/** @fn zb_bool_t zb_se_has_valid_key(zb_uint16_t addr)
 *  @brief Checks availability of valid keypair for this device.
 *  @param[in]  addr - short address of the remote device
 *  @return ZB_TRUE if valid key exists, ZB_FALSE otherwise.
 *  @details Valid key is either TCLK to TC established by CBKE procedure or partner APS Link key
 *    established using partner link keys establishment procedure.
 *  @par Example
 *  @code{.c}
 *    if (dev_addr != 0 && !zb_se_has_valid_key(dev_addr)
 *    {
 *      ZB_SCHEDULE_CALLBACK2(zb_se_start_aps_key_establishment, param, dev_addr);
 *    }
 *  @endcode
 *  @see zb_se_start_aps_key_establishment - start APS partner link key establishment
 */
zb_bool_t zb_se_has_valid_key(zb_uint16_t addr);

zb_bool_t zb_se_has_valid_key_by_ieee(zb_ieee_addr_t addr);

/**
 * Get APS link key or TCLK for remote device.
 * To be used mainly for debug purposes. Check key availability using zb_se_has_valid_key() before this call.
 *
 * @param addr - short address of remote device
 * @param link_key - buffer for the key
 * @return RET_OK or error code
 */
zb_ret_t zb_se_debug_get_link_key(zb_uint16_t addr, zb_uint8_t link_key[ZB_CCM_KEY_SIZE]);


/**
 * Get APS link key or TCLK for remote device by its long address.
 * To be used mainly for debug purposes.
 *
 * @param ieee - long address of remote device
 * @param link_key - buffer for the key
 * @return RET_OK or error code
 */
zb_ret_t zb_se_debug_get_link_key_by_long(zb_ieee_addr_t ieee, zb_uint8_t link_key[ZB_CCM_KEY_SIZE]);

/**
 * Get current NWK key,
 * To be used mainly for debug purposes.
 *
 * @param key - buffer for the key
 * @return RET_OK or error code
 */
zb_ret_t zb_se_debug_get_nwk_key(zb_uint8_t key[ZB_CCM_KEY_SIZE]);

#ifdef ZB_ENABLE_SE
void zb_se_delete_cbke_link_key(zb_ieee_addr_t ieee_address);
#endif /* ZB_ENABLE_SE */


/**
 * Get APS key generated from the current installcode.
 * To be used mainly for debug purposes.
 *
 * @param key - buffer for the key
 * @return RET_OK or error code
 */
zb_ret_t zb_se_debug_get_ic_key(zb_uint8_t key[ZB_CCM_KEY_SIZE]);

/** @fn void zb_se_start_aps_key_establishment(zb_uint8_t param, zb_uint16_t addr)
 *  @brief Starts procedure of partner APS Link key establishment with specified device.
 *  @param[in]  param - reference to the buffer which will be used for outgoing Match Descriptor
 *                      Request command
 *  @param[in]  addr - short address of the remote device
 *  @details ZBOSS indicates completion status of this procedure by passing @ref ZB_SE_SIGNAL_APS_KEY_READY
 *    and @ref ZB_SE_SIGNAL_APS_KEY_FAIL signals to the application's signal handler. Application
 *    should process this signals if needed.
 *  @see ZB_SE_SIGNAL_APS_KEY_READY - signal description
 *  @see ZB_SE_SIGNAL_APS_KEY_FAIL - signal description
 *  @see ZB_SE_SIGNAL_CBKE_OK - example code of signal handling
 *  @see zb_se_has_valid_key - check if there is a valid APS key with the device
 */
void zb_se_start_aps_key_establishment(zb_uint8_t param, zb_uint16_t addr);


/** @} */ /* se_secur */


/** @addtogroup se_comm
 *  @{
 */

#ifdef ZB_SE_COMMISSIONING
#ifdef ZB_COORDINATOR_ROLE
/** @fn void zb_se_set_network_coordinator_role(zb_uint32_t channel_mask)
 *  @brief Initiates SE device as a Zigbee 2.4 GHz coordinator
 *  @param[in] channel_mask - Zigbee channel mask
 *  @par Example
 *  @snippet se/energy_service_interface/se_esi_zc.c ESI_DEV_DEFINE_PARAMS
 *  @snippet se/energy_service_interface/se_esi_zc.c ESI_DEV_INIT
 *  @snippet se/energy_service_interface/se_esi_zc.c ESI_DEV_SET_ROLE
 */
void zb_se_set_network_coordinator_role(zb_uint32_t channel_mask);
#endif /* ZB_COORDINATOR_ROLE */



#ifdef ZB_ED_FUNC
/** @fn void zb_se_set_network_ed_role(zb_uint32_t channel_mask)
 *  @brief Initiates SE device as a Zigbee 2.4 GHz End Device
 *  @param[in] channel_mask - Zigbee channel mask
 *  @par Example
 *  @snippet se/metering/se_el_metering_zed.c METERING_DEV_DEFINE_PARAMS
 *  @snippet se/metering/se_el_metering_zed.c METERING_DEV_INIT
 *  @snippet se/metering/se_el_metering_zed.c METERING_DEV_SET_ROLE
 */
void zb_se_set_network_ed_role(zb_uint32_t channel_mask);
#endif /* ZB_ED_FUNC */

#ifdef ZB_ROUTER_ROLE
/** @fn void zb_se_set_network_router_role(zb_uint32_t channel_mask)
 *  @brief Initiates SE device as a Zigbee 2.4 GHz router
 *  @param[in] channel_mask - Zigbee channel mask
 *  @par Example
 *  @snippet se/in_home_display/se_ihd_zr.c IHD_DEV_DEFINE_PARAMS
 *  @snippet se/in_home_display/se_ihd_zr.c IHD_DEV_INIT
 *  @snippet se/in_home_display/se_ihd_zr.c IHD_DEV_SET_ROLE
 */
void zb_se_set_network_router_role(zb_uint32_t channel_mask);
#endif /* ZB_ROUTER_ROLE */

#ifdef ZB_COORDINATOR_ROLE
/**
 *  Initiate device as a Zigbee MultiMAC Select device coordinator
 *  Note that Select device ZC if impossible in SE profile.
 *  This API supposes channel_list can contain either single 2.4 GHz entry or 1 or more sub-gig entries,
 *  so ZC will be either 2.4-only or Sub-GHz-only depending on channel_list contents.
 *
 *  @param channel_list - pointer to Zigbee channel list
 */
void zb_se_set_network_coordinator_role_select_device(zb_channel_list_t channel_list);

/**
   Initiate device as a Zigbee MultiMAC Switch device coordinator
   Note: Requires 2 MAC interfaces. Not every HW supports Switch ZC role.
   @param channel_list - pointer to Zigbee channel list
*/
void zb_se_set_network_coordinator_role_switch_device(zb_channel_list_t channel_list);


/** @fn void zb_se_permit_joining(zb_time_t timeout_s)
 *  @brief Opens network for joining for specified amount of time
 *  @param[in]  timeout_s    amount of time (sec) after which joining will be disabled
 *  @note This function should be called at Trust Center only
 *  @par Example
 *  @snippet se/energy_service_interface/se_esi_zc.c ESI_DEV_DEFINE_PARAMS
 *  @snippet se/energy_service_interface/se_esi_zc.c PERMIT_JOINING
 */
void zb_se_permit_joining(zb_time_t timeout_s);
#endif /* ZB_COORDINATOR_ROLE */

#ifdef ZB_ED_FUNC
/**
   Initiate device as a Zigbee MultiMAC Select device end device
   If channel_list contains 2.4 only entry, work as 2.4-only device.
   If channel_list contains Sub-GHz entries and no 2.4 entry, work as Sub-GHz-only device.
   If channel_list contains both 2.4 and Sub-GHz entries, work as true Select device trying to join at 2.4 then at Sub-GHz.
   @param channel_list - pointer to Zigbee channel list
*/
void zb_se_set_network_ed_role_select_device(zb_channel_list_t channel_list);
#endif /* ZB_ED_FUNC */


/** @fn zb_ret_t zb_se_auto_join_start(zb_uint8_t param)
 *  @brief Puts the device into Auto-Joining state
 *  @param[in]  param - reference to the buffer which will be used for outgoing network discovery
 *                      request or 0 if buffer should be allocated.
 *  @details Auto-Joining state implies performing of PAN joining attempts with defined
 *    timeouts between retries.
 *  @par Example
 *  @snippet se/in_home_display/se_ihd_zr.c AUTO_JOIN
 */
zb_ret_t zb_se_auto_join_start(zb_uint8_t param);


/** @fn zb_ret_t zb_se_auto_join_stop()
 *  @brief Stops the Auto-Join process started with @ref zb_se_auto_join_start
 *  @par Example
 *  @snippet se/in_home_display/se_ihd_zr.c AUTO_JOIN
 */
zb_ret_t zb_se_auto_join_stop(void);
#endif /* ZB_SE_COMMISSIONING */


#if defined(ZB_SE_ENABLE_SERVICE_DISCOVERY_PROCESSING) || defined(DOXYGEN)

/** @fn zb_ret_t zb_se_service_discovery_start(zb_uint8_t endpoint)
 *  @brief Puts the device into Service Discovery state
 *  @param[in]  endpoint - source endpoint for service discovery
 *  @retval RET_OK on success
 *  @retval RET_INVALID_PARAMETER_1 if specified endpoint is invalid (zero, non-SE, has no client clusters)
 *
 *  @details Service Discovery mechanism is used to discover other devices on the network that have
 *    services that match with the device's. The initiator device will receive series of devices
 *    matching the discovery criteria. It should decide whether to bind found device or not depending
 *    on cluster_id and commodity_type values. Process stops when no more devices left.
 *  @par Example
 *  @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_GET_SIGNAL
 *  @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_START_DISCOVERY
 */
zb_ret_t zb_se_service_discovery_start(zb_uint8_t endpoint);


/** @fn void zb_se_service_discovery_stop()
 *  @brief Stops Service Discovery process started with @ref zb_se_service_discovery_start
 *  @details This call allows to terminate receiving of matched devices prematurely without
 *    waiting when process finished. It is not mandatory call for stopping Service Discovery
 *    but it can be useful if initiator decides to break the discovery for some reason (for
 *    example, it doesn't need to bind multiple devices).
 */
void zb_se_service_discovery_stop();


/** @fn void zb_se_service_discovery_set_multiple_commodity_enabled(zb_uint8_t enabled)
 *  @brief Allows to enable or disable support of multiple commodity network
 *  @param[in]  enabled - flag (0|1) that says whether multiple commodity should be enabled or not
 *  @details In networks that support multiple commodities Service Discovery process will return
 *    multiple instances of certain clusters. Multiple commodity feature allows application to
 *    distinguish what type of metering/price/etc it has just discovered by analyzing commodity_type
 *    attribute. In single-commodity network this attribute is not supported meaning all commodities
 *    are of the same type.
 *  @note Multiple commodity feature is enabled by default in ZBOSS stack.
 *  @par Example
 *  @snippet se/in_home_display/se_ihd_zr.c IHD_DEV_INIT
 *  @code{.c}
 *      // disable multiple commodity
 *      zb_se_service_discovery_set_multiple_commodity_enabled(0);
 *  @endcode
 */
void zb_se_service_discovery_set_multiple_commodity_enabled(zb_uint8_t enabled);


/** @fn void zb_se_service_discovery_bind_req(zb_uint8_t param, zb_ieee_addr_t dst_ieee, zb_uint16_t dst_ep)
 *  @brief Sends Bind Request to the discovered SE device
 *  @note This call should be used only after application got signal @ref ZB_SE_SIGNAL_SERVICE_DISCOVERY_DO_BIND.
 *  @param[in]  param - reference to the buffer which will be used for outgoing Bind Request command
 *  @param[in]  dst_ieee - address of the found device
 *  @param[in]  dst_ep  - device's endpoint
 *  @details Device info (address, endpoint) should be taken from a signal's parameter.
 *    Cluster ID will be taken automatically by stack.
 *  @par Example
 *  @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_GET_SIGNAL
 *  @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_DO_BIND
 *  @snippet se/in_home_display/se_ihd_zr.c SIGNAL_HANDLER_BIND_DEV
 *  @see zb_se_signal_service_discovery_bind_params_t
 *  @see ZB_SE_SIGNAL_SERVICE_DISCOVERY_DO_BIND
 */
void zb_se_service_discovery_bind_req(zb_uint8_t param, zb_ieee_addr_t dst_ieee, zb_uint16_t dst_ep);

#endif

/** @name SE Runtime
 *  @{
 */

#ifdef ZB_SE_COMMISSIONING
/**
   Inform ZBOSS that application is starting high-frequency mode at given cluster.

   This routine is meaningful when working in Sub-Ghz mode only.

   High frequency messages are messages which transmitted more
   frequently than once per 30 seconds. SE 1.4 specification does not
   define what is "message" so ZBOSS treats it as "packets using same
   cluster".
   High frequency messages must not require APS ACK and must set
   "Disable Default Response" bit.

   Normally ZBOSS detects high-frequency messages
   automatically, so only second message over same cluster can be
   "high frequency message".  But SE certification testing requires
   message to be "high frequency" starting from the first message, so
   ZBOSS implemented explicit routine.

   @param clusterid - cluster ID.
 */
void zb_start_high_freq_msgs(zb_uint16_t clusterid);
#endif /* ZB_SE_COMMISSIONING */

/** @} */ /* se_runtime */


/** @} */ /* se_comm */

/** @endcond */ /* DOXYGEN_SE_SECTION */

#endif /* ZBOSS_API_ZSE_H */
