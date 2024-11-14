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
/*  PURPOSE: Public Network layer API
*/
#ifndef ZB_ZBOSS_API_NWK_H
#define ZB_ZBOSS_API_NWK_H 1

/** \addtogroup nwk_api */
/** @{ */

/** @addtogroup nwk_multi_mac Multi-MAC configuration
 * @{
 */

/* C-STAT does not allow to assign ZB_MAC_INTERFACE_MAX to ZB_NWK_MAC_IFACE_TBL_SIZE,
 * so check configurations manually */
#if defined(ZB_MAC_MONOLITHIC) && defined(ZB_MACSPLIT_HOST)
  #define ZB_NWK_MAC_IFACE_TBL_SIZE 2U
#elif defined(ZB_MACSPLIT_HOST) && defined(ZB_MAC_BLE)
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
#elif defined(NCP_MODE_HOST)
  /* Preserve default table size to keep previous stack behaviour, but
   * do not use any interfaces in that case */
  #define ZB_NWK_MAC_IFACE_TBL_SIZE 1U
#elif defined(ZB_EXTMAC)
  #define ZB_NWK_MAC_IFACE_TBL_SIZE 1U
#else
  #error Multi-MAC configuration is not supported, at least one interface should be enabled
#endif

#if ZB_NWK_MAC_IFACE_TBL_SIZE > 1U && defined(ZB_MAC_INTERFACE_SINGLE) && !defined(ZB_ZGPD_ROLE)
  #error ZB_MAC_INTERFACE_SINGLE requires to enable only one interface
#endif

/**
 * @brief Flag that is used to perform some action on all interfaces together
 */
#define ZB_NWK_MULTIMAC_ALL_INTERFACES 0xFFU

/** @} */ /* nwk_multi_mac */

/** @addtogroup nwk_common_constants NWK common constants
 * @{
 */

/**
 * @name Network coordinator address
*/
#define ZB_NWK_COORDINATOR_ADDR 0x0000U

/**
 * @name Network broadcast addresses types
 * @anchor nwk_broadcast_addresses
*/
/** @{ */
#define ZB_NWK_BROADCAST_ALL_DEVICES        0xFFFFu /**< All devices in PAN */
#define ZB_NWK_BROADCAST_RX_ON_WHEN_IDLE    0xFFFDu /**< macRxOnWhenIdle = TRUE */
#define ZB_NWK_BROADCAST_ROUTER_COORDINATOR 0xFFFCu /**< All routers and coordinator */
#define ZB_NWK_BROADCAST_LOW_POWER_ROUTER   0xFFFBu /**< Low power routers only */

#define ZB_NWK_BROADCAST_RESERVED           0xFFF8u /**< Reserved value */
/** @} */ /* nwk_broadcast_addresses */

/** @} */ /* nwk_common_constants */

/** @addtogroup nwk_management_service NWK management service
 *  @{
 */
/** @brief Check that address is broadcast.
  * @param addr - 16-bit address
  * @return TRUE if address is broadcast, FALSE otherwise
  */
/* #define ZB_NWK_IS_ADDRESS_BROADCAST(addr) ( ((addr) & 0xFFF0) == 0xFFF0 ) */
#define ZB_NWK_IS_ADDRESS_BROADCAST(addr) ( (addr) >= ZB_NWK_BROADCAST_RESERVED )
/** @} */ /* nwk_management_service */

/** @addtogroup nwk_common_constants NWK common constants
 * @{
 */

/**
 * @name Network device type
 * @anchor nwk_device_type
*/
/** @{ */
#define ZB_NWK_DEVICE_TYPE_COORDINATOR 0U /*!< Device - Coordinator */
#define ZB_NWK_DEVICE_TYPE_ROUTER      1U /*!< Device - Router */
#define ZB_NWK_DEVICE_TYPE_ED          2U /*!< Device - End device */
#define ZB_NWK_DEVICE_TYPE_NONE        3U /*!< Unknown Device */
/** @} */

/**
 * @brief Type for network device type
 *
 * Holds one of @ref nwk_device_type. Kept for backward compatibility as
 * @ref nwk_device_type were declared previously as enum.
 */
typedef zb_uint8_t zb_nwk_device_type_t;

/**
 * @name Leave types
 * @anchor nwk_leave_type
 */
/** @{ */
#define ZB_NWK_LEAVE_TYPE_RESET  0x00U /**< Leave without rejoin */
#define ZB_NWK_LEAVE_TYPE_REJOIN 0x01U /**< Leave with rejoin */
/** @} */

/**
 * @brief Type for leave types
 *
 * Holds one of @ref nwk_leave_type. Kept for backward compatibility as
 * @ref nwk_leave_type were declared previously as enum.
 */
typedef zb_uint8_t zb_nwk_leave_type_t;


/** @cond internals_doc */
/**
 * @name Network command status codes
 * @anchor nwk_command_status
 */
/** @{ */
#define ZB_NWK_COMMAND_STATUS_NO_ROUTE_AVAILABLE          0x00U /**< No route available */
#define ZB_NWK_COMMAND_STATUS_TREE_LINK_FAILURE           0x01U /**< Tree link failure */
#define ZB_NWK_COMMAND_STATUS_NONE_TREE_LINK_FAILURE      0x02U /**< None-tree link failure */
#define ZB_NWK_COMMAND_STATUS_LOW_BATTERY_LEVEL           0x03U /**< Low battery level */
#define ZB_NWK_COMMAND_STATUS_NO_ROUTING_CAPACITY         0x04U /**< No routing capacity */
#define ZB_NWK_COMMAND_STATUS_NO_INDIRECT_CAPACITY        0x05U /**< No indirect capacity */
#define ZB_NWK_COMMAND_STATUS_INDIRECT_TRANSACTION_EXPIRY 0x06U /**< Indirect transaction expiry */
#define ZB_NWK_COMMAND_STATUS_TARGET_DEVICE_UNAVAILABLE   0x07U /**< Target device unavailable */
#define ZB_NWK_COMMAND_STATUS_TARGET_ADDRESS_UNALLOCATED  0x08U /**< Target address unallocated */
#define ZB_NWK_COMMAND_STATUS_PARENT_LINK_FAILURE         0x09U /**< Parent link failure */
#define ZB_NWK_COMMAND_STATUS_VALIDATE_ROUTE              0x0aU /**< Validate route */
#define ZB_NWK_COMMAND_STATUS_SOURCE_ROUTE_FAILURE        0x0bU /**< Source route failure */
#define ZB_NWK_COMMAND_STATUS_MANY_TO_ONE_ROUTE_FAILURE   0x0cU /**< Many-to-one route failure */
#define ZB_NWK_COMMAND_STATUS_ADDRESS_CONFLICT            0x0dU /**< Address conflict */
#define ZB_NWK_COMMAND_STATUS_VERIFY_ADDRESS              0x0eU /**< Verify address */
#define ZB_NWK_COMMAND_STATUS_PAN_IDENTIFIER_UPDATE       0x0fU /**< Pan ID update */
#define ZB_NWK_COMMAND_STATUS_NETWORK_ADDRESS_UPDATE      0x10U /**< Network address update */
#define ZB_NWK_COMMAND_STATUS_BAD_FRAME_COUNTER           0x11U /**< Bad frame counter  */
#define ZB_NWK_COMMAND_STATUS_BAD_KEY_SEQUENCE_NUMBER     0x12U /**< Bad key sequence number */
#define ZB_NWK_COMMAND_STATUS_UNKNOWN_COMMAND             0x13U /**< Command received is not known *//** @} */
/** @} */

/**
 * @brief Type for network command status codes
 *
 * Holds one of @ref nwk_command_status. Kept for backward compatibility as
 * @ref nwk_command_status were declared previously as enum.
 */
typedef zb_uint8_t zb_nwk_command_status_t;

/** @endcond */ /* internals_doc */
/** @} */ /* nwk_common_constants */

/** @cond internals_doc */
/** @brief 3.4.3 Network Status Command: Arguments of the NLME-STATUS.indication routine. */
typedef ZB_PACKED_PRE struct zb_nlme_status_indication_s
{
  zb_uint8_t status; /**< Error code associated with the failure */
  zb_uint16_t network_addr;  /**< Network device address associated with the status information */
  zb_uint8_t unknown_command_id; /**< Unknown command ID
                                    (required for ZB_NWK_COMMAND_STATUS_UNKNOWN_COMMAND) */
} ZB_PACKED_STRUCT
zb_nlme_status_indication_t;
/** @endcond */ /* internals_doc */

/** @addtogroup nwk_common_constants NWK common constants
 * @{
 */
/**
   LQI undefined value
 */
#define ZB_MAC_LQI_UNDEFINED 0U
/**
   RSSI undefined value
 */
#define ZB_MAC_RSSI_UNDEFINED 0x7fU

/**
   Number of the first channel
*/
/* #define ZB_MAC_START_CHANNEL_NUMBER 11 */
/**
   Maximal number of the channels
 */
#define ZB_MAC_MAX_CHANNEL_NUMBER   26U
/**
   Total number of supported channels
 */
#define ZB_MAC_SUPPORTED_CHANNELS   (ZB_MAC_MAX_CHANNEL_NUMBER + 1U)

/** @cond DOXYGEN_SE_SECTION */
/* See D.10.2.1.3.2 Channel Pages */

/* North America and Mexico region Page 23 */

/**
   The first logical channel for Page 23 Sub GHz
*/
#define ZB_PAGE23_SUB_GHZ_START_LOGICAL_CHANNEL 0U

/**
   Number of the first channel in channel mask for Page 23 Sub GHz
*/
#define ZB_PAGE23_SUB_GHZ_START_CHANNEL_NUMBER 0U
/**
   The last logical channel for Page 23 Sub GHz
*/
#define ZB_PAGE23_SUB_GHZ_MAX_LOGICAL_CHANNEL 24U
/**
   Number of the last channel in channel mask for Page 23 Sub GHz
*/
#define ZB_PAGE23_SUB_GHZ_MAX_CHANNEL_NUMBER (ZB_PAGE23_SUB_GHZ_MAX_LOGICAL_CHANNEL - \
                                              ZB_PAGE23_SUB_GHZ_START_LOGICAL_CHANNEL)

/* Europe region Pages 24 - 27 */

/**
   The first logical channel for Page 24 Sub GHz
*/
#define ZB_PAGE24_SUB_GHZ_START_LOGICAL_CHANNEL 56U

/**
   Number of the first channel in channel mask for Page 24 Sub GHz
*/
#define ZB_PAGE24_SUB_GHZ_START_CHANNEL_NUMBER 0U
/**
   The last logical channel for Page 24 Sub GHz
*/
#define ZB_PAGE24_SUB_GHZ_MAX_LOGICAL_CHANNEL 76U
/**
   Number of the last channel in channel mask for Page 24 Sub GHz
*/
#define ZB_PAGE24_SUB_GHZ_MAX_CHANNEL_NUMBER (ZB_PAGE24_SUB_GHZ_MAX_LOGICAL_CHANNEL - \
                                              ZB_PAGE24_SUB_GHZ_START_LOGICAL_CHANNEL)

/**
   The first logical channel for Page 25 Sub GHz
*/
#define ZB_PAGE25_SUB_GHZ_START_LOGICAL_CHANNEL 0U

/**
   Number of the first channel in channel mask for Page 25 Sub GHz
*/
#define ZB_PAGE25_SUB_GHZ_START_CHANNEL_NUMBER 0U
/**
   The last logical channel for Page 25 Sub GHz
*/
#define ZB_PAGE25_SUB_GHZ_MAX_LOGICAL_CHANNEL 26U
/**
   Number of the last channel in channel mask for Page 25 Sub GHz
*/
#define ZB_PAGE25_SUB_GHZ_MAX_CHANNEL_NUMBER (ZB_PAGE25_SUB_GHZ_MAX_LOGICAL_CHANNEL - \
                                              ZB_PAGE25_SUB_GHZ_START_LOGICAL_CHANNEL)

/**
   The first logical channel for Page 26 Sub GHz
*/
#define ZB_PAGE26_SUB_GHZ_START_LOGICAL_CHANNEL 27U

/**
   Number of the first channel in channel mask for Page 26 Sub GHz
*/
#define ZB_PAGE26_SUB_GHZ_START_CHANNEL_NUMBER 0U
/**
   The last logical channel for Page 26 Sub GHz
*/
#define ZB_PAGE26_SUB_GHZ_MAX_LOGICAL_CHANNEL 34U
/**
   Number of the last channel in channel mask for Page 26 Sub GHz
*/
#define ZB_PAGE26_SUB_GHZ_MAX_CHANNEL_NUMBER (ZB_PAGE26_SUB_GHZ_MAX_LOGICAL_CHANNEL - \
                                              ZB_PAGE26_SUB_GHZ_START_LOGICAL_CHANNEL)

/**
   The first logical channel for Page 27 Sub GHz
*/
#define ZB_PAGE27_SUB_GHZ_START_LOGICAL_CHANNEL 35U

/**
   Number of the first channel in channel mask for Page 27 Sub GHz
*/
#define ZB_PAGE27_SUB_GHZ_START_CHANNEL_NUMBER 0U
/**
   The last logical channel for Page 27 Sub GHz
*/
#define ZB_PAGE27_SUB_GHZ_MAX_LOGICAL_CHANNEL 55U
/**
   Number of the last channel in channel mask for Page 27 Sub GHz
*/
#define ZB_PAGE27_SUB_GHZ_MAX_CHANNEL_NUMBER (ZB_PAGE27_SUB_GHZ_MAX_LOGICAL_CHANNEL - \
                                              ZB_PAGE27_SUB_GHZ_START_LOGICAL_CHANNEL)

/* Great Britain region Pages 28 - 31 */

/**
   The first logical channel for Page 28 Sub GHz
*/
#define ZB_PAGE28_SUB_GHZ_START_LOGICAL_CHANNEL 0U

/* 08/21/2017 EE CR:MINOR Am I right this value is always 0 for any page? Do we ever need separate constants? */
/**
   Number of the first channel in channel mask for Page 28 Sub GHz
*/
#define ZB_PAGE28_SUB_GHZ_START_CHANNEL_NUMBER 0U
/**
   The last logical channel for Page 28 Sub GHz
*/
#define ZB_PAGE28_SUB_GHZ_MAX_LOGICAL_CHANNEL 26U
/**
   Number of the last channel in channel mask for Page 28 Sub GHz
*/
#define ZB_PAGE28_SUB_GHZ_MAX_CHANNEL_NUMBER (ZB_PAGE28_SUB_GHZ_MAX_LOGICAL_CHANNEL - \
                                              ZB_PAGE28_SUB_GHZ_START_LOGICAL_CHANNEL)

/**
   The first logical channel for Page 29 Sub GHz
*/
#define ZB_PAGE29_SUB_GHZ_START_LOGICAL_CHANNEL 27U
/**
   Number of the first channel in channel mask for Page 29 Sub GHz
*/
#define ZB_PAGE29_SUB_GHZ_START_CHANNEL_NUMBER 0U
/**
   The last logical channel for Page 29 Sub GHz (for solid range 27-34)
*/
#define ZB_PAGE29_SUB_GHZ_PRE_MAX_LOGICAL_CHANNEL 34U
/**
   The last logical channel for Page 29 Sub GHz
*/
#define ZB_PAGE29_SUB_GHZ_MAX_LOGICAL_CHANNEL 62U
/**
   Number of the last channel in channel mask for Page 29 Sub GHz
*/
#define ZB_PAGE29_SUB_GHZ_MAX_CHANNEL_NUMBER (ZB_PAGE29_SUB_GHZ_PRE_MAX_LOGICAL_CHANNEL - \
                                              ZB_PAGE29_SUB_GHZ_START_LOGICAL_CHANNEL + 1U /* 62 channel */)

/**
   The first logical channel for Page 30 Sub GHz
*/
#define ZB_PAGE30_SUB_GHZ_START_LOGICAL_CHANNEL 35U
/**
   Number of the first channel in channel mask for Page 30 Sub GHz
*/
#define ZB_PAGE30_SUB_GHZ_START_CHANNEL_NUMBER 0U
/**
   The last logical channel for Page 30 Sub GHz
*/
#define ZB_PAGE30_SUB_GHZ_MAX_LOGICAL_CHANNEL 61U
/**
   Number of the last channel in channel mask for Page 30 Sub GHz
*/
#define ZB_PAGE30_SUB_GHZ_MAX_CHANNEL_NUMBER (ZB_PAGE30_SUB_GHZ_MAX_LOGICAL_CHANNEL - \
                                              ZB_PAGE30_SUB_GHZ_START_LOGICAL_CHANNEL)

/**
   The first logical channel for Page 31 Sub GHz
*/
#define ZB_PAGE31_SUB_GHZ_START_LOGICAL_CHANNEL 0U
/**
   Number of the first channel in channel mask for Page 31 Sub GHz
*/
#define ZB_PAGE31_SUB_GHZ_START_CHANNEL_NUMBER 0U
/**
   The last logical channel for Page 31 Sub GHz
*/
#define ZB_PAGE31_SUB_GHZ_MAX_LOGICAL_CHANNEL 26U
/**
   Number of the last channel in channel mask for Page 31 Sub GHz
*/
#define ZB_PAGE31_SUB_GHZ_MAX_CHANNEL_NUMBER (ZB_PAGE31_SUB_GHZ_MAX_LOGICAL_CHANNEL - \
                                              ZB_PAGE31_SUB_GHZ_START_LOGICAL_CHANNEL)
  /** @endcond */ /* DOXYGEN_SE_SECTION */
/* ZB_MAC_START_CHANNEL_NUMBER */
/**
   The first logical channel for Page 0 2.4GHz
*/
#define ZB_PAGE0_2_4_GHZ_START_LOGICAL_CHANNEL 11U
/* Let's keep compatibility with bits layout in ZBOSS without sub-gig: start from 11 in 2.4.
   Actually, only subgig pages 28 and 31 are ok too (logical == bit#). Pages 29 and 30 are special case.
 */
/**
   Number of the first channel in channel mask for Page 0 2.4GHz
*/
#define ZB_PAGE0_2_4_GHZ_START_CHANNEL_NUMBER ZB_PAGE0_2_4_GHZ_START_LOGICAL_CHANNEL
/**
   The last logical channel in channel mask for Page 0 2.4GHz
*/
#define ZB_PAGE0_2_4_GHZ_MAX_LOGICAL_CHANNEL ZB_MAC_MAX_CHANNEL_NUMBER
/**
   Number of the last channel in channel mask for Page 0 2.4GHz
*/
#define ZB_PAGE0_2_4_GHZ_MAX_CHANNEL_NUMBER  ZB_MAC_MAX_CHANNEL_NUMBER
/* (ZB_PAGE0_2_4_GHZ_MAX_LOGICAL_CHANNEL - \ */
/*  ZB_PAGE0_2_4_GHZ_START_LOGICAL_CHANNEL) */

/** @cond DOXYGEN_SE_SECTION */
/**
   Maximal number of channels for Page 23 Sub GHz
*/
#define ZB_PAGE23_SUB_GHZ_MAX_CHANNELS_COUNT (ZB_PAGE23_SUB_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE23_SUB_GHZ_START_CHANNEL_NUMBER + 1U)
/**
   Maximal number of channels for Page 24 Sub GHz
*/
#define ZB_PAGE24_SUB_GHZ_MAX_CHANNELS_COUNT (ZB_PAGE24_SUB_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE24_SUB_GHZ_START_CHANNEL_NUMBER + 1U)
/**
   Maximal number of channels for Page 25 Sub GHz
*/
#define ZB_PAGE25_SUB_GHZ_MAX_CHANNELS_COUNT (ZB_PAGE25_SUB_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE25_SUB_GHZ_START_CHANNEL_NUMBER + 1U)
/**
   Maximal number of channels for Page 26 Sub GHz
*/
#define ZB_PAGE26_SUB_GHZ_MAX_CHANNELS_COUNT (ZB_PAGE26_SUB_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE26_SUB_GHZ_START_CHANNEL_NUMBER + 1U)
/**
   Maximal number of channels for Page 27 Sub GHz
*/
#define ZB_PAGE27_SUB_GHZ_MAX_CHANNELS_COUNT (ZB_PAGE27_SUB_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE27_SUB_GHZ_START_CHANNEL_NUMBER + 1U)

/**
   Maximal number of channels for Page 28 Sub GHz
*/
#define ZB_PAGE28_SUB_GHZ_MAX_CHANNELS_COUNT (ZB_PAGE28_SUB_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE28_SUB_GHZ_START_CHANNEL_NUMBER + 1U)
/**
   Maximal number of channels for Page 29 Sub GHz
*/
#define ZB_PAGE29_SUB_GHZ_MAX_CHANNELS_COUNT (ZB_PAGE29_SUB_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE29_SUB_GHZ_START_CHANNEL_NUMBER + 1U)
/**
   Maximal number of channels for Page 30 Sub GHz
*/
#define ZB_PAGE30_SUB_GHZ_MAX_CHANNELS_COUNT (ZB_PAGE30_SUB_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE30_SUB_GHZ_START_CHANNEL_NUMBER + 1U)
/**
   Maximal number of channels for Page 31 Sub GHz
*/
#define ZB_PAGE31_SUB_GHZ_MAX_CHANNELS_COUNT (ZB_PAGE31_SUB_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE31_SUB_GHZ_START_CHANNEL_NUMBER + 1U)
/** @endcond */ /* DOXYGEN_SE_SECTION */
/**
   Maximal number of channels for Page 0 2.4GHz
*/
#define ZB_PAGE0_2_4_GHZ_MAX_CHANNELS_COUNT  (ZB_PAGE0_2_4_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE0_2_4_GHZ_START_CHANNEL_NUMBER  + 1U)
/** @cond DOXYGEN_SE_SECTION */
/**
   Maximal number of ed scan channels for all pages
*/
#define ZB_ED_SCAN_MAX_CHANNELS_COUNT                                   \
  ((ZB_IO_BUF_SIZE - sizeof(zb_uint8_t)) / sizeof(zb_energy_detect_channel_info_t))
/**
   Max # of network descriptors which can fit into a single buffer.
*/
#define ZB_ACTIVE_SCAN_MAX_NETWORK_COUNT                                   \
  ((ZB_IO_BUF_SIZE - sizeof(zb_nlme_network_discovery_confirm_t)) / sizeof(zb_nlme_network_descriptor_t))

#define ZB_CHANNEL_PAGE_MAX_CHANNELS_COUNT ZB_PAGE28_SUB_GHZ_MAX_CHANNELS_COUNT
/** @endcond */ /* DOXYGEN_SE_SECTION */
/** @} */ /* nwk_common_constants */

/** @cond internals_doc */
/** @addtogroup nwk_pib_cache PIB cache
 *  @{
 */

/**
  Data structure used to cache frequently used PIB attributes.

  Useful when using alien MAC layer without direct access to PIB: blockable "get"
  interface to PIB is too complex and slow.
 */
typedef struct zb_nwk_pib_cache_s
{
  zb_uint16_t          mac_short_address;                              /*!< The 16-bit address that the device uses
                                                                              to communicate in the PAN. */
  zb_uint16_t          mac_pan_id;                                     /*!< The 16-bit identifier of the PAN on which
                                                                              the device is operating. If this value is 0xffff,
                                                                              the device is not associated. */
  zb_ieee_addr_t       mac_extended_address;                           /*!< The 64-bit (IEEE) address assigned to the device. */

  zb_uint8_t           mac_association_permit;                         /*!< Indication of whether a coordinator is currently
                                                                              allowing association. A value of TRUE indicates*/
  zb_uint8_t           mac_rx_on_when_idle;                            /*!< Indication of whether the MAC sublayer is to enable
                                                                              its receiver during idle periods. */
  zb_uint8_t           phy_current_page[ZB_NWK_MAC_IFACE_TBL_SIZE];    /*!< Index of current physical channel page  */
  zb_uint8_t           phy_current_channel[ZB_NWK_MAC_IFACE_TBL_SIZE]; /*!< Index of current physical channel */
  zb_uint8_t           phy_primary_iface;                              /*!< Index of MAC interface that is used for joining */
} zb_nwk_pib_cache_t;

/** @} */


/**
  * @name PIB cache mirror
  * \par Here are cache of PIB / NIB mirror of PIB values.
  *
  * After that values modified in NWK or upper, must sync it with MAC using MLME-SET.
  *
  * @{
  */

/**
  Function used to access PIB cache.
  @return pointer to PIB cache.
  */
zb_nwk_pib_cache_t *zb_nwk_get_pib_cache(void);
/**
  Macro used to access PIB cache.
  @return pointer to PIB cache.
  */


/* We get false-positive violations of MISRA rules 5.2 and 5.4 here
 * because an internal header redefine ZB_PIB_CACHE macro for
 * optimization purposes, and C-STAT can not properly handle
 * #undef usage. Only one macro definition is valid at a time,
 * but MISRA rules 5.2 and 5.4 prohibit two definitions of a
 * symbol simultaneously.
 * We ensure that only one instance of the macro is effectively
 * defined here. */
#ifdef ZB_PIB_CACHE
#error Please ensure that you are not including any internal headers which redefine ZB_PIB_CACHE macro before including this file
#else /* ZB_PIB_CACHE */
/*cstat !MISRAC2012-Rule-5.2_c99 !MISRAC2012-Rule-5.4_c99 */
#define ZB_PIB_CACHE() zb_nwk_get_pib_cache()
#endif /* ZB_PIB_CACHE */

#ifndef NCP_MODE_HOST
/** Cached value of device network address */
#define ZB_PIBCACHE_NETWORK_ADDRESS()  ZB_PIB_CACHE()->mac_short_address
/** Cached value of device Pan ID */
#define ZB_PIBCACHE_PAN_ID()           ZB_PIB_CACHE()->mac_pan_id
/** Cached value of device extended address */
#define ZB_PIBCACHE_EXTENDED_ADDRESS() ZB_PIB_CACHE()->mac_extended_address
/** Cached value of RxOnWhenIdle attribute */
#ifdef ZB_ED_RX_OFF_WHEN_IDLE
#define ZB_PIBCACHE_RX_ON_WHEN_IDLE() ZB_B2U(!ZB_IS_DEVICE_ZED())
#else
#define ZB_PIBCACHE_RX_ON_WHEN_IDLE()  ZB_PIB_CACHE()->mac_rx_on_when_idle
#endif
/** Cached value of AssociationPermit attribute */
#define ZB_PIBCACHE_ASSOCIATION_PERMIT() ZB_PIB_CACHE()->mac_association_permit

#define ZB_PIBCACHE_PRIMARY_IFACE()  ZB_PIB_CACHE()->phy_primary_iface
#define ZB_PIBCACHE_PRIMARY_IFACE_PAGE() ZB_PIB_CACHE()->phy_current_page[ZB_PIB_CACHE()->phy_primary_iface]
#define ZB_PIBCACHE_PRIMARY_IFACE_CHANNEL() ZB_PIB_CACHE()->phy_current_channel[ZB_PIB_CACHE()->phy_primary_iface]

/** Cached value of CurrentChannel attribute */
#define ZB_PIBCACHE_CURRENT_CHANNEL()  ZB_PIBCACHE_PRIMARY_IFACE_CHANNEL()
#define ZB_PIBCACHE_CURRENT_CHANNEL_BY_IFACE(iface_id)  ZB_PIB_CACHE()->phy_current_channel[iface_id]
#define ZB_PIBCACHE_CURRENT_CHANNELS_LIST() ZB_PIB_CACHE()->phy_current_channel
/** Cached value of CurrentPage attribute */
#define ZB_PIBCACHE_CURRENT_PAGE()  ZB_PIBCACHE_PRIMARY_IFACE_PAGE()
#define ZB_PIBCACHE_CURRENT_PAGE_BY_IFACE(iface_id)  ZB_PIB_CACHE()->phy_current_page[iface_id]
#define ZB_PIBCACHE_CURRENT_PAGES_LIST() ZB_PIB_CACHE()->phy_current_page

#else /* NCP_MODE_HOST */

/** Cached value of device network address */
#define ZB_PIBCACHE_NETWORK_ADDRESS()  zb_get_short_address()
/** Cached value of RxOnWhenIdle attribute */
#ifdef ZB_ED_RX_OFF_WHEN_IDLE
/* Make simpler checks if ZED always rx-off-when-idle */
#undef ZB_PIBCACHE_RX_ON_WHEN_IDLE
#define ZB_PIBCACHE_RX_ON_WHEN_IDLE() ZB_B2U(!ZB_IS_DEVICE_ZED())
#else
#define ZB_PIBCACHE_RX_ON_WHEN_IDLE()  ZB_B2U(zb_get_rx_on_when_idle())
#endif
#endif /* !NCP_MODE_HOST */

/**
   Arguments of the NLME-PERMIT_JOINING.request routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_permit_joining_request_s
{
  zb_uint8_t permit_duration; /**< Time in seconds during which the coordinator
                               * or router will allow associations */
} ZB_PACKED_STRUCT
zb_nlme_permit_joining_request_t;

/**
   NLME-PERMIT-JOINING.request primitive

   Allow/disallow network joining

   @param param - buffer containing parameters - @see
   zb_nlme_network_formation_request_t
   @return RET_OK on success, error code otherwise.

   @snippet doxygen_snippets.dox zb_nlme_permit_joining_request
 */
void zb_nlme_permit_joining_request(zb_uint8_t param);


/** @} */
/** @endcond */ /* internals_doc */

/** @addtogroup nwk_panid_conflicts NWK PAN ID conflicts
 *  @{
 */

/**
   @brief Callback parameters for zb_prepare_network_for_channel_change() and zb_prepare_network_for_panid_change()
   Stack sends set_configuration_req to the all devices and count responses with incorrect status
  */
typedef struct zb_channel_panid_change_preparation_s
{
  zb_uint16_t error_cnt;  /*!< Number of not OK responses from remote devices for next channel/panid change */
} zb_channel_panid_change_preparation_t;


/**
   @brief Parameters for zb_prepare_network_for_channel_change() and zb_start_channel_change()
  */
typedef struct zb_channel_change_parameters_s
{
  zb_uint32_t next_channel_change; /*!< This field indicates the next channel
                                        that will be used once a start channel change command is received.
                                        Only 1 channel page and channel bit shall be set */
}zb_channel_change_parameters_t;

/**
   @brief Parameters for zb_prepare_network_for_panid_change() and zb_start_panid_change()
  */
typedef struct zb_panid_change_parameters_s
{
  zb_uint16_t next_panid_change; /*!< This field indicates the next PAN ID
                                      that will be used once a pan id change command is received.
                                      If value is 0x0000, CURRENT PAN ID or 0xFFFF then will be generate a new random value */
}zb_panid_change_parameters_t;

#if defined ZB_COORDINATOR_ROLE

/**
   This function set nwkNextChannelChange and sends set_configuration_req  for all remote devices.
   Parameters zb_channel_change_parameters_t will be using for zb_start_channel_change()
   @param param - buffer with parameters @ref zb_channel_change_parameters_t
   @param  cb   - callback function, @ref zb_channel_panid_change_preparation_t

   @snippet r23_new_api/r23_zc.c change_channel_snippet
 */
zb_ret_t zb_prepare_network_for_channel_change(zb_uint8_t param, zb_callback_t cb);

/**
   This function set nwkNextPanId and sends set_configuration_req for all remote devices
   Parameters zb_panid_change_parameters_t will be using for zb_start_panid_change()
   @param param - buffer with parameters @ref zb_panid_change_parameters_t
   @param cb    - callback function, @ref zb_channel_panid_change_preparation_t

   @snippet r23_new_api/r23_zc.c change_panid_snippet
 */
zb_ret_t zb_prepare_network_for_panid_change(zb_uint8_t param, zb_callback_t cb);
#endif /* ZB_COORDINATOR_ROLE */

#if defined ZB_ROUTER_ROLE
/**
   Broadcast mgmt_nwk_update_req and change active channel for all devices in network.
   In centralized network the parameter zb_channel_change_parameters_t
   will be discarded @see zb_prepare_network_for_channel_change()

   @param param - buffer with parameters @ref zb_channel_change_parameters_t

   @snippet r23_new_api/r23_zc.c change_channel_snippet
 */
zb_ret_t zb_start_channel_change(zb_uint8_t param);


/**
   Broadcast mgmt_nwk_update_req and change PAN ID for all devices in network.
   In centralized network the parameter zb_channel_change_parameters_t
   will be discarded @see zb_prepare_network_for_panid_change()
   This function can be used during pan id conflict resolutions, but R23 specification recommends
   to ignore the pan id conflicts

   @param param - buffer with parameters @ref zb_panid_change_parameters_t

   @snippet r23_new_api/r23_zc.c change_panid_snippet
 */
zb_ret_t  zb_start_panid_change(zb_uint8_t param);
#endif  /* #ifdef ZB_ROUTER_ROLE */


/**
   Toggles panid conflict resolution.

   @deprecated Enabling/disabling of panid conflict resolution is deprecated in r23 codebase snd does nothing.

   Call of that function allows switching on/off panid conflict resolution and detection
   logic.

   @deprecated Kept only for backward compatibility.
   PANID conflict detection is always enabled at R23 and can not be disabled. Application is responsible for starting of conflict resolution.

   @param status - if ZB_TRUE, enable conflict resolution, else disable
 */
void zb_enable_panid_conflict_resolution(zb_bool_t status);

/**
   Toggles automatic panid conflict resolution.

   @deprecated Enabling/disabling of automatic panid conflict resolution is deprecated in r23 codebase and does nothing.

   Call of that function allows switching on/off automatic panid conflict resolution.

   @param status - if ZB_TRUE, enable conflict resolution, else disable
 */
void zb_enable_auto_pan_id_conflict_resolution(zb_bool_t status);

/** @} */ /* nwk_panid_conflicts */


/** @addtogroup nwk_management_service NWK management service
 * @{
 */
#ifdef ZB_LOW_SECURITY_MODE
/**
    Public API to set device security level to 0
    @deprecated This function will be removed in the next Major release after june 2023.
                Now security level is always equal to 5.
*/
void zb_disable_nwk_security(void);

/**
    Public API to set device security level to 5
    @deprecated This function will be removed in the next Major release after june 2023.
                Now security level is always equal to 5.
*/
void zb_enable_nwk_security(void);
#endif /* ZB_LOW_SECURITY_MODE */

#ifdef ZB_NWK_CONFIGURABLE_DST_IEEE_IN_HDR
/**
   Configure Destination IEEE policy for out NWK frames

   @param put_always - if ZB_TRUE, put DST IEEE for all NWK frame types, if it is set to ZB_FALSE -
   put it for NWK Command frame type only
   Default value is ZB_TRUE.
*/
void zb_nwk_set_ieee_policy(zb_bool_t put_always);
#endif /* ZB_NWK_CONFIGURABLE_DST_IEEE_IN_HDR */
/** @} */ /* nwk_management_service */

/** @addtogroup nwk_mtorr NWK MTORR functionality
 * @{
 */

#if defined ZB_COORDINATOR_ROLE || defined DOXYGEN
/**
   Enable Concentrator mode for the device (disabled by default).
   It's possible to call this function to send MTORR immediately, e.g. after a new device joined the network.
   It does affect only for Coordinator role.

   @param radius - the hop count radius for concentrator route discoveries.
   If the value is set zero then the default radius will be used.
   @param disc_time - the time in seconds between concentrator route discoveries.
   If the value is set to zero, the route discoveries are done by the application layer only.
*/
void zb_start_concentrator_mode(zb_uint8_t radius, zb_uint32_t disc_time);

/**
   Disable Concentrator mode for the device.
   It does affect only for Coordinator role.
*/
void zb_stop_concentrator_mode(void);
#endif /* ZB_COORDINATOR_ROLE */


#if defined(ZB_PRO_STACK) && !defined(ZB_LITE_NO_SOURCE_ROUTING) && defined(ZB_ROUTER_ROLE)
/**
   When Concentrator mode is enabled, force single unplanned MTORR
 */
void zb_concentrator_force_mtorr(void);
#endif /* ZB_PRO_STACK && !ZB_LITE_NO_SOURCE_ROUTING && ZB_ROUTER_ROLE */
/** @} */ /* nwk_mtorr */

/** @cond internals_doc */
/**
  Change Transmit Failure Threshold for Neighbor table.
  This threshold defines how many packets may fail the delivery before stack will start Address
  Discovery.
  Formula for discovery start: if device is a neighbor and packet can not be delivered, try to
  - do "Transmit Failure Threshold" attempts of packet delivery
  - if packet is not delivered, wait "Transmit Failure Timeout"
  - if packet is not delivered, remove device from neighbors and start address discovery procedure
  @param transmit_failure_cnt - new threshold (1 to 255)
  Default value - @ref ZB_ZDO_NEIGHBOR_FAILURE_PKT_THRESHOLD
*/
void zb_set_nbt_transmit_failure_threshold(zb_uint8_t transmit_failure_cnt);
/**
  Change Transmit Failure Timeout for Neighbor table.
  This timeout defines how much time stack will wait before starting Network Address
  discovery. Countdown starts after Transmit Failure Threshold condition is met.
  @param transmit_failure_timeout - new timeout in seconds (0 to 255)
  Default value - @ref ZB_ZDO_NEIGHBOR_UNREACHABLE_TIMEOUT
*/
void zb_set_nbt_transmit_failure_timeout(zb_uint8_t transmit_failure_timeout);
/** @endcond */ /* internals_doc */

/** @addtogroup nwk_management_service NWK management service
 * @{
 */

/**
   Get own device type.

   @return One of the values from @ref nwk_device_type.

   @snippet onoff_server/on_off_output_zc.c zb_get_device_type_example
 */
zb_nwk_device_type_t zb_get_device_type(void);

/**
   Get short address of the parent node.

   @return Short address of the parent node or ZB_UNKNOWN_SHORT_ADDR if the device isn't joined to a network.
 */
zb_uint16_t zb_nwk_get_parent(void);


#define ZB_NWK_NBR_ITERATOR_INDEX_EOT 0xFFFFU /*! Index, indicating that the iterator reached boundaries of the neighbour table. */

typedef ZB_PACKED_PRE struct zb_nwk_nbr_iterator_cb_params_s {
   zb_uint16_t index;        /*!< In the callback function:
                              *     Index of the returned neighbour table entry.
                              *     The value of ZB_NWK_NBR_ITERATOR_INDEX_EOT
                              *     indicates that the entry was not returned and
                              *     the buffer payload should be ignored.
                              *   If the structure is passed as the API call parameters:
                              *     Index, from which the next neighbour table
                              *     entry should be searched.
                              */
   zb_uint32_t update_count; /*!< In the callback function:
                              *     The current value of the table update counter.
                              *     This parameter is ignored in the API call parameters.
                              */
} ZB_PACKED_STRUCT
zb_nwk_nbr_iterator_params_t;

typedef ZB_PACKED_PRE struct zb_nwk_nbr_iterator_entry_s
{
  zb_ieee_addr_t  ieee_addr;            /*!< Long address (EUI64) of the device. */
  zb_uint16_t     short_addr;           /*!< Short address (network address) of the device. */

  zb_uint8_t      device_type;          /*!< Neighbor device type - @see @ref nwk_device_type */
  zb_uint8_t      depth;                /*!< The network depth of this device.
                                         *    A value of 0x00 indicates that this device is the
                                         *    Zigbee coordinator for the network.
                                         */
  zb_uint8_t      rx_on_when_idle;      /*!< Indicates if neighbour receiver enabled during idle periods:
                                         *     TRUE = Receiver is on
                                         *     FALSE = Receiver is off
                                         *   This field should be present for entries that record the parent or
                                         *   children of a Zigbee router or Zigbee coordinator.
                                         */
  zb_uint8_t      relationship;         /*!< The relationship between the neighbour and the current device.
                                         *   This field shall be present in every neighbour table entry.
                                         *   @if DOXYGEN_INTERNAL_DOC See @ref nwk_relationship @endif
                                         */
  zb_uint8_t      send_via_routing;     /*!< That field is deprecated. Removed
                                         * from zb_neighbor_tbl_ent_t, always 0
                                         * here. */

  zb_uint8_t      keepalive_received;   /*!< This value indicates at least one keepalive
                                         *   has been received from the end device since
                                         *   the router has rebooted.
                                         */
  zb_uint8_t      mac_iface_idx;        /*!< An index into the MAC Interface Table
                                         * indicating what interface the neighbour or
                                         * child is bound to.
                                         */

  zb_uint8_t      transmit_failure_cnt; /*!< Transmit failure counter (used to initiate
                                         * device address search).
                                         */
  zb_uint8_t      lqi;                  /*!< Link quality. Also used to calculate
                                         * incoming cost
                                         */
  zb_int8_t       rssi;                 /*!< Received signal strength indicator */
  zb_uint8_t      outgoing_cost;        /*!< The cost of an outgoing link. Got from link status. */
  zb_uint8_t      age;                  /*!< Counter value for router aging.
                                         *   The number of nwkLinkStatusPeriod intervals since a
                                         *   link status command was received.
                                         */
  zb_uint32_t     device_timeout;       /*!< Configured end device timeout, in seconds. */
  zb_uint32_t     timeout_counter;      /*!< Timeout value ED aging, in milliseconds. */
} ZB_PACKED_STRUCT
zb_nwk_nbr_iterator_entry_t;

/**
   Read the next active entry from the NWK neighbour table.
   The index indicates the point, from which the entry will be searched in the neighbour table.
   This API returns neighbour table entry inside the buffer payload, that are connected to the same PAN
   and their entries are not marked as stale or timed out.
   The index of the entry is passed as buffer parameters.

   @param  bufid  The ZBOSS buffer, containing arguments defined by zb_nwk_nbr_iterator_params_t structure, passed as buffer parameters.
   @param  cb     Callback function, that will get the next neighbour table entry.
 */
zb_ret_t zb_nwk_nbr_iterator_next(zb_uint8_t bufid, zb_callback_t cb);

/** @} */ /* nwk_management_service */

#if defined ZB_ROUTER_ROLE

/**
   Set the number of end device a device is allow to have.

   @return RET_OK if successful
 */
zb_ret_t zb_nwk_set_max_ed_capacity(zb_uint8_t value);

/**
   Get the number of end devices a device is allow to have.

   @return number of end devices
 */
zb_uint8_t zb_nwk_get_max_ed_capacity(void);

/**
   Get the maximum number of siblings and children that a device can have.

   @return Size of neighbor table
 */
zb_uint8_t zb_nwk_get_total_capacity(void);

#endif /* ZB_ROUTER_ROLE */
/** @} */ /* nwk_api */

#endif /*#ifndef ZB_ZBOSS_API_NWK_H*/
