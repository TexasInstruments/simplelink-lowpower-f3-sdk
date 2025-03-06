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
/* PURPOSE: SE devices configuration
*/

#ifndef ZB_SE_DEVICE_CONFIG_H
#define ZB_SE_DEVICE_CONFIG_H 1

#if defined ZB_ENABLE_SE || defined DOXYGEN

/** @cond DOXYGEN_SE_SECTION */

/** @addtogroup se_devices
 *  @{
 */

/**
 * @addtogroup se_devices_id SE device identifiers
 * @{
 */
/** @enum zb_se_standard_devs_e
 *  @brief SE Device identifiers
 *  @details Table 5-13 – Devices Specified in the Smart Energy Profile
 */
enum zb_se_standard_devs_e
{
  ZB_SE_RANGE_EXTENDER_DEVICE_ID           = 0x0008,       /*!< Range Extender */
  ZB_SE_ENERGY_SERVICE_INTERFACE_DEVICE_ID = 0x0500,		/*!< Energy Service Interface */
  ZB_SE_METERING_DEVICE_ID                 = 0x0501,		/*!< Metering Device */
  ZB_SE_IN_HOME_DISPLAY_DEVICE_ID          = 0x0502,		/*!< In-Home Display */
  ZB_SE_PCT_DEVICE_ID                      = 0x0503,		/*!< Programmable Communicating Thermostat (PCT) */
  ZB_SE_LOAD_CONTROL_DEVICE_ID             = 0x0504,		/*!< Load Control Device */
  ZB_SE_SMART_APPLIANCE_DEVICE_ID          = 0x0505, 		/*!< Smart Appliance */
  ZB_SE_PREPAYMENT_TERMINAL_DEVICE_ID      = 0x0506,		/*!< Prepayment Terminal */
  ZB_SE_PHYSICAL_DEVICE_ID                 = 0x0507,		/*!< Physical Device */
  ZB_SE_REMOTE_COMMUNICATIONS_DEVICE_ID    = 0x0508,		/*!< Remote Communications Device */
  ZB_SE_ERL_INTERFACE_DEVICE_ID            = 0x0509, 		/*!< Reserved for Linky 'ERL Interface' Device */
  // Reserved 0x050A –0x5FF
  ZB_SE_TEST_DEVICE_ID                     = 0xfff0,		/*!< Custom SE device for test */
  ZB_SE_CUSTOM_TUNNEL_DEVICE_ID            = 0xfff1,		/*!< Custom Tunnel device (should declared in private profile) */
};
/** @} */ /*SE device identifiers*/

/** @cond internals_doc */
/* Define supported devices here */
#ifdef ZB_ALL_DEVICE_SUPPORT
#define ZB_SE_DEFINE_DEVICE_RANGE_EXTENDER
#define ZB_SE_DEFINE_DEVICE_ENERGY_SERVICE_INTERFACE
#define ZB_SE_DEFINE_DEVICE_METERING
#define ZB_SE_DEFINE_DEVICE_METERING_LOAD_CONTROL
#define ZB_SE_DEFINE_DEVICE_IN_HOME_DISPLAY
#define ZB_SE_DEFINE_DEVICE_PCT
#define ZB_SE_DEFINE_DEVICE_LOAD_CONTROL
#define ZB_SE_DEFINE_DEVICE_SMART_APPLIANCE
#define ZB_SE_DEFINE_DEVICE_PREPAYMENT_TERMINAL
#define ZB_SE_DEFINE_DEVICE_PHYSICAL
#define ZB_SE_DEFINE_DEVICE_REMOTE_COMMUNICATIONS
#define ZB_SE_DEFINE_DEVICE_ERL_INTERFACE

/************** Custom Devices ******************/
#define ZB_SE_DEFINE_DEVICE_CUSTOM_ATTR
#define ZB_SE_DEFINE_DEVICE_CUSTOM_TUNNEL

#define ZB_SE_DEFINE_DEVICE_TEST_DEVICE

#endif /* ZB_ALL_DEVICE_SUPPORT */

/*********************** devices to cluster configuration **********************/
/* This clusters are always supported in SE */
#ifndef ZB_ENABLE_SE_MIN_CONFIG
#define ZB_ZCL_SUPPORT_CLUSTER_BASIC  1
#endif
#define ZB_ZCL_SUPPORT_CLUSTER_KEC  1

#ifdef ZB_SE_DEFINE_DEVICE_METERING
#define ZB_ZCL_SUPPORT_CLUSTER_IDENTIFY 1
#define ZB_ZCL_SUPPORT_CLUSTER_METERING 1
#define ZB_ZCL_SUPPORT_CLUSTER_PRICE 1
#define ZB_ZCL_SUPPORT_CLUSTER_TUNNELING 1
#define ZB_ZCL_SUPPORT_CLUSTER_EVENTS 1
#define ZB_ZCL_SUPPORT_CLUSTER_CALENDAR 1
#define ZB_ZCL_SUPPORT_CLUSTER_PREPAYMENT 1
#define ZB_ZCL_SUPPORT_CLUSTER_MDU_PAIRING 1
#define ZB_ZCL_SUPPORT_CLUSTER_TIME 1
#define ZB_ZCL_SUPPORT_CLUSTER_DEVICE_MANAGEMENT 1
#endif /* ZB_SE_DEFINE_DEVICE_METERING */

#ifdef ZB_SE_DEFINE_DEVICE_IN_HOME_DISPLAY
#define ZB_ZCL_SUPPORT_CLUSTER_PRICE 1
#define ZB_ZCL_SUPPORT_CLUSTER_EVENTS 1
#define ZB_ZCL_SUPPORT_CLUSTER_CALENDAR 1
#define ZB_ZCL_SUPPORT_CLUSTER_PREPAYMENT 1
#define ZB_ZCL_SUPPORT_CLUSTER_MDU_PAIRING 1
#define ZB_ZCL_SUPPORT_CLUSTER_TIME 1
#define ZB_ZCL_SUPPORT_CLUSTER_ENERGY_MANAGEMENT 1
#define ZB_ZCL_SUPPORT_CLUSTER_DEVICE_MANAGEMENT 1
#endif /* ZB_SE_DEFINE_DEVICE_IN_HOME_DISPLAY */

#ifdef ZB_SE_DEFINE_DEVICE_ENERGY_SERVICE_INTERFACE
#define ZB_ZCL_SUPPORT_CLUSTER_PRICE 1
#define ZB_ZCL_SUPPORT_CLUSTER_MESSAGING 1
#define ZB_ZCL_SUPPORT_CLUSTER_TIME 1
#define ZB_ZCL_SUPPORT_CLUSTER_DRLC 1
#define ZB_ZCL_SUPPORT_CLUSTER_TUNNELING 1
#define ZB_ZCL_SUPPORT_CLUSTER_CALENDAR 1
#define ZB_ZCL_SUPPORT_CLUSTER_MDU_PAIRING 1
#define ZB_ZCL_SUPPORT_CLUSTER_DEVICE_MANAGEMENT 1
#endif /* ZB_SE_DEFINE_DEVICE_ENERGY_SERVICE_INTERFACE */

#ifdef ZB_SE_DEFINE_DEVICE_PCT
#define ZB_ZCL_SUPPORT_CLUSTER_MDU_PAIRING 1
#define ZB_ZCL_SUPPORT_CLUSTER_ENERGY_MANAGEMENT 1
#endif /* ZB_SE_DEFINE_DEVICE_PCT */

#ifdef ZB_SE_DEFINE_DEVICE_LOAD_CONTROL
#define ZB_ZCL_SUPPORT_CLUSTER_ENERGY_MANAGEMENT 1
#endif /* ZB_SE_DEFINE_DEVICE_LOAD_CONTROL */

#ifdef ZB_SE_DEFINE_DEVICE_RANGE_EXTENDER
#endif /* ZB_SE_DEFINE_DEVICE_RANGE_EXTENDER */

#ifdef ZB_SE_DEFINE_DEVICE_SMART_APPLIANCE
#define ZB_ZCL_SUPPORT_CLUSTER_PRICE 1
#define ZB_ZCL_SUPPORT_CLUSTER_TIME 1
#endif /* ZB_SE_DEFINE_DEVICE_SMART_APPLIANCE */

#ifdef ZB_SE_DEFINE_DEVICE_PREPAYMENT_TERMINAL
#define ZB_ZCL_SUPPORT_CLUSTER_PRICE 1
#define ZB_ZCL_SUPPORT_CLUSTER_TIME 1
#define ZB_ZCL_SUPPORT_CLUSTER_PREPAYMENT 1
#endif /* ZB_SE_DEFINE_DEVICE_PREPAYMENT_TERMINAL */

#ifdef ZB_SE_DEFINE_DEVICE_PHYSICAL
#endif /* ZB_SE_DEFINE_DEVICE_PHYSICAL */

#ifdef ZB_SE_DEFINE_DEVICE_REMOTE_COMMUNICATIONS
#endif /* ZB_SE_DEFINE_DEVICE_REMOTE_COMMUNICATIONS */

#ifdef ZB_SE_DEFINE_DEVICE_ERL_INTERFACE
#endif /* ZB_SE_DEFINE_DEVICE_ERL_INTERFACE */

/* BDB uses identify */
#if defined ZB_BDB_MODE && !defined ZB_ZCL_SUPPORT_CLUSTER_IDENTIFY
#define ZB_ZCL_SUPPORT_CLUSTER_IDENTIFY 1
#endif

/** @endcond */

#if !defined ZB_SUBGHZ_BAND_ENABLED
#ifdef ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ
#undef ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ
#endif
#endif /* !ZB_SUBGHZ_BAND_ENABLED */

/** @} */ /* se_devices */

/** @endcond */ /* DOXYGEN_SE_SECTION */

#endif /* ZB_ENABLE_SE */

#endif /* ZB_SE_DEVICE_CONFIG_H */
