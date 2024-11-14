/* GreenBOSS Zigbee Green Power Device software protocol stack
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
 * www.dsr-iot.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 * This is unpublished proprietary source code of DSR Corporation
 * The copyright notice does not evidence any actual or intended
 * publication of such source code.
 *
 * GreenBOSS is a registered trademark of Data Storage Research LLC d/b/a DSR
 * Corporation
 *
 * Commercial Usage
 * Licensees holding valid DSR Commercial licenses may use
 * this file in accordance with the DSR Commercial License
 * Agreement provided with the Software or, alternatively, in accordance
 * with the terms contained in a written agreement between you and
 * DSR.
 */
/* PURPOSE: IAS Zone cluster definitions
*/

#ifndef ZB_ZCL_IAS_ZONE_H
#define ZB_ZCL_IAS_ZONE_H 1

/*! \addtogroup zclIasZone */
/*! @{ */

/*! @brief IAS Zone cluster attribute identifiers
    @see ZCL spec, IAS Zone Cluster 8.2.2.2
*/
enum zb_zcl_ias_zone_attr_e
{
  /*! @brief ZoneState attribute, ZCL spec 8.2.2.2.1.1 */
  ZB_ZCL_ATTR_IAS_ZONE_ZONESTATE_ID = 0x0000,
  /*! @brief ZoneType attribute, ZCL spec 8.2.2.2.1.2 */
  ZB_ZCL_ATTR_IAS_ZONE_ZONETYPE_ID = 0x0001,
  /*! @brief ZoneStatus attribute, ZCL spec 8.2.2.2.1.3 */
  ZB_ZCL_ATTR_IAS_ZONE_ZONESTATUS_ID = 0x0002,
  /*! @brief IAS_CIE_Address  attribute, ZCL spec 8.2.2.2.2.1. */
  ZB_ZCL_ATTR_IAS_ZONE_IAS_CIE_ADDRESS_ID = 0x0010,
  /*! @brief ZoneID attribute, ZCL spec 8.2.2.1.2.2 */
  ZB_ZCL_ATTR_IAS_ZONE_ZONEID_ID = 0x0011,
  /*! @brief NumberOfZoneSensitivityLevelsSupported attribute, ZCL spec 8.2.2.1.2.3 */
  ZB_ZCL_ATTR_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_ID = 0x0012,
  /*! @brief CurrentZoneSensitivityLevel attribute, ZCL spec 8.2.2.1.2.4 */
  ZB_ZCL_ATTR_IAS_ZONE_CURRENT_ZONE_SENSITIVITY_LEVEL_ID = 0x0013,
  /*! @brief Custom attribute - ZGP Calibration */
  ZB_ZCL_ATTR_CUSTOM_ZGP_CALIBRATION = 0x8000,
  /*! @brief Custom attribute - ZGP Cloud ACK */
  ZB_ZCL_ATTR_CUSTOM_ZGP_CLOUD_ACK = 0x8003,
  /*! @brief Custom attribute - IAS_CIE_Address can only be reassigned via Write attribute when this attribute is equal to ZB_FALSE */
  ZB_ZCL_ATTR_CUSTOM_CIE_ADDR_IS_SET = 0xE000,
  /*! @brief Custom attribute - store CIE Endpoint number */
  ZB_ZCL_ATTR_CUSTOM_CIE_EP = 0xE001,
  /*! @brief Custom attribute - store CIE short address */
  ZB_ZCL_ATTR_CUSTOM_CIE_SHORT_ADDR = 0xE002,
  /* TODO: move this attribute to IAS Zone ZCL implementation */
  /*! @brief Struct with pointers on User App callbacks */
  ZB_ZCL_ATTR_IAS_ZONE_INT_CTX_ID = 0xeffe,
};

/**
 * @brief IAS Zone ZoneState value
 */
enum zb_zcl_ias_zone_zonestate_e
{
  /** @brief ZoneState not enrolled value */
    ZB_ZCL_IAS_ZONE_ZONESTATE_NOT_ENROLLED  = 0,

  /** @brief ZoneState enrolled value */
    ZB_ZCL_IAS_ZONE_ZONESTATE_ENROLLED  = 1,
};

/** @brief IAS Zone ZoneState attribute default value */
#define ZB_ZCL_IAS_ZONE_ZONESTATE_DEF_VALUE            ZB_ZCL_IAS_ZONE_ZONESTATE_NOT_ENROLLED

/** @brief Default value for IAS Zone cluster revision global attribute */
#define ZB_ZCL_IAS_ZONE_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0002u)

/**
 * @brief IAS Zone ZoneType value
 */
enum zb_zcl_ias_zone_zonetype_e
{
  /** @brief ZoneType Standard CIE System Alarm value */
    ZB_ZCL_IAS_ZONE_ZONETYPE_STANDARD_CIE  = 0x0000,

  /** @brief ZoneType Motion value */
    ZB_ZCL_IAS_ZONE_ZONETYPE_MOTION  = 0x000d,

  /** @brief ZoneType Contact switch value */
    ZB_ZCL_IAS_ZONE_ZONETYPE_CONTACT_SWITCH  = 0x0015,

  /** @brief ZoneType Fire sensor value */
    ZB_ZCL_IAS_ZONE_ZONETYPE_FIRE_SENSOR  = 0x0028,

  /** @brief ZoneType Water sensor value */
    ZB_ZCL_IAS_ZONE_ZONETYPE_WATER_SENSOR  = 0x002a,

  /** @brief ZoneType Gas sensor value */
    ZB_ZCL_IAS_ZONE_ZONETYPE_GAS_SENSOR  = 0x002b,

  /** @brief ZoneType Personal emergency value */
    ZB_ZCL_IAS_ZONE_ZONETYPE_PERSONAL_EMERGENCY  = 0x002c,

  /** @brief ZoneType Vibration / Movement sensor value */
    ZB_ZCL_IAS_ZONE_ZONETYPE_VIBRATION_MOVEMENT  = 0x002d,

  /** @brief ZoneType Remote Control value */
    ZB_ZCL_IAS_ZONE_ZONETYPE_REMOTE_CONTROL  = 0x010f,

  /** @brief ZoneType Key fob value */
    ZB_ZCL_IAS_ZONE_ZONETYPE_KEY_FOB  = 0x0115,

  /** @brief ZoneType Keypad value */
    ZB_ZCL_IAS_ZONE_ZONETYPE_KEYPAD  = 0x021d,

  /** @brief ZoneType Standard Warning Device  value */
    ZB_ZCL_IAS_ZONE_ZONETYPE_STANDARD_WARNING  = 0x0225,

  /** @brief Manufacturer specific ZoneType value */
    ZB_ZCL_IAS_ZONE_ZONETYPE_MANUF_SPEC  = 0x8000,

  /** @brief ZoneType Invalid ZoneType value */
    ZB_ZCL_IAS_ZONE_ZONETYPE_INVALID  = 0xffff,

};

/*! @brief IAS Zone ZoneStatus attribute flags
    @see ZCL spec 8.2.2.2.1.3
*/
enum zb_zcl_ias_zone_zonestatus_e
{
  /** Alarm 1 */
  ZB_ZCL_IAS_ZONE_ZONE_STATUS_ALARM1      = 1 << 0,
  /** Alarm 2 */
  ZB_ZCL_IAS_ZONE_ZONE_STATUS_ALARM2      = 1 << 1,
  /** Tamper */
  ZB_ZCL_IAS_ZONE_ZONE_STATUS_TAMPER      = 1 << 2,
  /** Battery */
  ZB_ZCL_IAS_ZONE_ZONE_STATUS_BATTERY     = 1 << 3,
  /** Supervision reports */
  ZB_ZCL_IAS_ZONE_ZONE_STATUS_SUPERVISION = 1 << 4,
  /** Restore reports */
  ZB_ZCL_IAS_ZONE_ZONE_STATUS_RESTORE     = 1 << 5,
  /** Trouble */
  ZB_ZCL_IAS_ZONE_ZONE_STATUS_TROUBLE     = 1 << 6,
  /** AC (mains) */
  ZB_ZCL_IAS_ZONE_ZONE_STATUS_AC_MAINS    = 1 << 7,
  /** Test */
  ZB_ZCL_IAS_ZONE_ZONE_STATUS_TEST            = 1 << 8,
  /** Battery Defect */
  ZB_ZCL_IAS_ZONE_ZONE_STATUS_BATTERY_DEFECT  = 1 << 9,
};

/** @brief IAS Zone ZoneStatus attribute default value */
#define ZB_ZCL_IAS_ZONE_ZONE_STATUS_DEF_VALUE            0

/** @brief Min value for NumberOfZoneSensitivityLevelsSupported attribute */
#define ZB_ZCL_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_MIN_VALUE ((zb_uint8_t)0x02)

/** @brief Default value for NumberOfZoneSensitivityLevelsSupported attribute */
#define ZB_ZCL_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_DEFAULT_VALUE \
  ZB_ZCL_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_MIN_VALUE

/** @brief Default value for CurrentZoneSensitivityLevel attribute */
#define ZB_ZCL_IAS_ZONE_CURRENT_ZONE_SENSITIVITY_LEVEL_DEFAULT_VALUE ((zb_uint8_t)0x00)

/** @brief IAS Zone ZoneID attribute default value */
#define ZB_ZCL_IAS_ZONEID_ID_DEF_VALUE            0xff

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CUSTOM_CIE_ADDR_IS_SET(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_CUSTOM_CIE_ADDR_IS_SET,                   \
  ZB_ZCL_ATTR_TYPE_U8,                                  \
  ZB_ZCL_ATTR_ACCESS_INTERNAL,                          \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CUSTOM_CIE_EP(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_CUSTOM_CIE_EP,                            \
  ZB_ZCL_ATTR_TYPE_U8,                                  \
  ZB_ZCL_ATTR_ACCESS_INTERNAL,                          \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CUSTOM_CIE_SHORT_ADDR(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_CUSTOM_CIE_SHORT_ADDR,                    \
  ZB_ZCL_ATTR_TYPE_U16,                                 \
  ZB_ZCL_ATTR_ACCESS_INTERNAL,                          \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}


#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IAS_ZONE_ZONESTATE_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_IAS_ZONE_ZONESTATE_ID,                    \
  ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                         \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IAS_ZONE_ZONETYPE_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_IAS_ZONE_ZONETYPE_ID,                     \
  ZB_ZCL_ATTR_TYPE_16BIT_ENUM,                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                         \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IAS_ZONE_ZONESTATUS_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_IAS_ZONE_ZONESTATUS_ID,                           \
  ZB_ZCL_ATTR_TYPE_16BITMAP,                                    \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,  \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                           \
  (void*) data_ptr                                              \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IAS_ZONE_IAS_CIE_ADDRESS_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_IAS_ZONE_IAS_CIE_ADDRESS_ID,              \
  ZB_ZCL_ATTR_TYPE_IEEE_ADDR,                           \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IAS_ZONE_ZONEID_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_IAS_ZONE_ZONEID_ID,                       \
  ZB_ZCL_ATTR_TYPE_U8,                                  \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                         \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_ID(data_ptr) \
{                                                                                                   \
  ZB_ZCL_ATTR_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_ID,                              \
  ZB_ZCL_ATTR_TYPE_U8,                                                                              \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                     \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                                               \
  (void*) data_ptr                                                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IAS_ZONE_CURRENT_ZONE_SENSITIVITY_LEVEL_ID(data_ptr) \
{                                                                                               \
  ZB_ZCL_ATTR_IAS_ZONE_CURRENT_ZONE_SENSITIVITY_LEVEL_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U8,                                                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                                                \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                                           \
  (void*) data_ptr                                                                              \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IAS_ZONE_INT_CTX_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_IAS_ZONE_INT_CTX_ID,                      \
  ZB_ZCL_ATTR_TYPE_NULL,                                \
  ZB_ZCL_ATTR_ACCESS_INTERNAL,                          \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}

/*! @internal Number of attributes mandatory for reporting in IAS Zone cluster */
#define ZB_ZCL_IAS_ZONE_REPORT_ATTR_COUNT 1

/** @brief Declares attribute list for IAS Zone cluster - server side.
    @param attr_list Attribute list name
    @param zone_state Pointer to variable to store ZoneState attribute
    @param zone_type Pointer to variable to store ZoneType attribute
    @param zone_status Pointer to variable to store ZoneStatus attribute
    @param ias_cie_address Pointer to variable to store IAS-CIE address attribute
    @param cie_short_addr Custom attribute to store CIE short address
    @param cie_ep Custom attribute to store CIE Endpoint number
*/
/* FIXME: declare custom attributes internally */
#define ZB_ZCL_DECLARE_IAS_ZONE_ATTRIB_LIST(                                                 \
  attr_list, zone_state, zone_type, zone_status,ias_cie_address,                             \
  cie_short_addr, cie_ep)                                                                    \
  zb_uint8_t cie_addr_is_set_##attr_list;                                                    \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IAS_ZONE_ZONESTATE_ID, (zone_state))                      \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IAS_ZONE_ZONETYPE_ID, (zone_type))                        \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IAS_ZONE_ZONESTATUS_ID, (zone_status))                    \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IAS_ZONE_IAS_CIE_ADDRESS_ID, (ias_cie_address))           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CUSTOM_CIE_SHORT_ADDR, (cie_short_addr))                  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CUSTOM_CIE_EP, (cie_ep))                                  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CUSTOM_CIE_ADDR_IS_SET, &(cie_addr_is_set_##attr_list))   \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */

#endif /* ZB_ZCL_IAS_ZONE_H */
