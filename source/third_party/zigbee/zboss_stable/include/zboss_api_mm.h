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
/*  PURPOSE: Public Multi-MAC layer API
*/
#ifndef ZBOSS_API_MM_H
#define ZBOSS_API_MM_H 1

/** \addtogroup mm_api */
/** @{ */

enum zb_mac_interfaces_counter_e {
  #if defined(ZB_MAC_MONOLITHIC)
    ZB_MAC_INTERFACE_MONOLITHIC,
  #endif
  #if defined(ZB_MACSPLIT_HOST)
    ZB_MAC_INTERFACE_MACSPLIT_HOST,
  #endif
  #if defined(ZB_MAC_BLE)
    ZB_MAC_INTERFACE_BLE,
  #endif
  #if defined(ZB_MACSPLIT_DEVICE)
    ZB_MAC_INTERFACE_MACSPLIT_DEVICE,
  #endif
  #if defined(ZB_MAC_SUBGHZ)
    ZB_MAC_INTERFACE_SUBGHZ,
  #endif
  #if defined(ZB_EXTMAC)
    ZB_MAC_INTERFACE_EXTMAC,
  #endif
    ZB_MAC_INTERFACE_MAX
};

#if defined(ZB_MAC_MONOLITHIC)
  #define ZB_MAC_INTERFACE_MONOLITHIC_ID ((zb_uint8_t)ZB_MAC_INTERFACE_MONOLITHIC)
#endif
#if defined(ZB_MACSPLIT_HOST)
  #define ZB_MAC_INTERFACE_MACSPLIT_HOST_ID ((zb_uint8_t)ZB_MAC_INTERFACE_MACSPLIT_HOST)
#endif
#if defined(ZB_EXTMAC)
  #define ZB_MAC_INTERFACE_EXTMAC_ID ((zb_uint8_t)ZB_MAC_INTERFACE_EXTMAC)
#endif
#if defined(ZB_MAC_BLE)
  #define ZB_MAC_INTERFACE_BLE_ID ((zb_uint8_t)ZB_MAC_INTERFACE_BLE)
#endif
#if defined(ZB_MACSPLIT_DEVICE)
  #define ZB_MAC_INTERFACE_MACSPLIT_DEVICE_ID ((zb_uint8_t)ZB_MAC_INTERFACE_MACSPLIT_DEVICE)
#endif
#if defined(ZB_MAC_SUBGHZ)
  #define ZB_MAC_INTERFACE_SUBGHZ_ID ((zb_uint8_t)ZB_MAC_INTERFACE_SUBGHZ)
#endif

/* C-STAT does not allow to assign ZB_MAC_INTERFACE_MAX to ZB_NWK_MAC_IFACE_TBL_SIZE,
 * so check configurations manually */
#if defined(ZB_MAC_MONOLITHIC) && defined(ZB_MACSPLIT_HOST)
  #define ZB_NWK_MAC_IFACE_TBL_SIZE 2U
#elif defined(ZB_MAC_MONOLITHIC) && defined(ZB_MAC_BLE)
  #define ZB_NWK_MAC_IFACE_TBL_SIZE 2U
#elif defined(ZB_MAC_MONOLITHIC) && !defined(ZB_MACSPLIT_HOST) && !defined(ZB_MAC_BLE) && !defined(ZB_MACSPLIT_DEVICE) && !defined(ZB_MAC_SUBGHZ)
  #define ZB_NWK_MAC_IFACE_TBL_SIZE 1U
#elif defined(ZB_MACSPLIT_HOST) && !defined(ZB_MAC_MONOLITHIC) && !defined(ZB_MAC_BLE) && !defined(ZB_MACSPLIT_DEVICE) && !defined(ZB_MAC_SUBGHZ)
  #define ZB_NWK_MAC_IFACE_TBL_SIZE 1U
#elif defined(ZB_MACSPLIT_DEVICE) && !defined(ZB_MAC_MONOLITHIC) && !defined(ZB_MAC_BLE) && !defined(ZB_MACSPLIT_HOST) && !defined(ZB_MAC_SUBGHZ)
  #define ZB_NWK_MAC_IFACE_TBL_SIZE 1U
  /* MAC-split SoC uses default MAC interface, so for building device with only SoC interface just declare monolithic MAC */
  #define ZB_MAC_INTERFACE_MONOLITHIC_ID ZB_MAC_INTERFACE_MACSPLIT_DEVICE_ID
#elif defined(NCP_MODE_HOST)
  /* Preserve default table size to keep previous stack behaviour, but
   * do not use any interfaces in that case */
  #define ZB_NWK_MAC_IFACE_TBL_SIZE 1U
#elif defined(ZB_EXTMAC)
  #define ZB_NWK_MAC_IFACE_TBL_SIZE 1U
#else
  #error Multi-MAC configuration is not supported, at least one interface should be enabled
#endif

#if ZB_NWK_MAC_IFACE_TBL_SIZE > 1U && defined(ZB_MAC_INTERFACE_SINGLE)
  #error ZB_MAC_INTERFACE_SINGLE requires to enable only one interface
#endif

/**
 * @brief Flag that is used to perform some action on all interfaces together
 */
#define ZB_NWK_MULTIMAC_ALL_INTERFACES 0xFFU

/** @} */ /* mm_api */

#endif /*ZBOSS_API_MM_H*/
