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
/* PURPOSE: Zigbee cluster common definitions
*/

#ifndef ZB_ZCL_COMMON_H
#define ZB_ZCL_COMMON_H 1

/*! \addtogroup zclCommon */
/*! @{ */

/**
 * @name ZCL cluster identifiers
 * @anchor zcl_cluster_id
 * @see ZCL spec, subclause 2.2.2
 */
/** @{ */
#define ZB_ZCL_CLUSTER_ID_BASIC                0x0000U /*!< Basic cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_POWER_CONFIG         0x0001U /*!< Power configuration cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_DEVICE_TEMP_CONFIG   0x0002U /*!< Device temperature configuration cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_IDENTIFY             0x0003U /*!< Identify cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_GROUPS               0x0004U /*!< Groups cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_SCENES               0x0005U /*!< Scenes cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_ON_OFF               0x0006U /*!< On/Off cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_ON_OFF_SWITCH_CONFIG 0x0007U /*!< On/Off switch configuration cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL        0x0008U /*!< Level control cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_ALARMS               0x0009U /*!< Alarms cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_TIME                 0x000aU /*!< Time cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_RSSI_LOCATION        0x000bU /*!< RSSI location cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_ANALOG_INPUT         0x000cU /*!< Analog input (basic) cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_ANALOG_OUTPUT        0x000dU /*!< Analog output (basic) cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_ANALOG_VALUE         0x000eU /*!< Analog value (basic) cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_BINARY_INPUT         0x000fU /*!< Binary input (basic) cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_BINARY_OUTPUT        0x0010U /*!< Binary output (basic) cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_BINARY_VALUE         0x0011U /*!< Binary value (basic) cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_MULTI_INPUT          0x0012U /*!< Multistate input (basic) cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_MULTI_OUTPUT         0x0013U /*!< Multistate output (basic) cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_MULTI_VALUE          0x0014U /*!< Multistate value (basic) cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_COMMISSIONING        0x0015U /*!< Commissioning cluster identifier. */

#define ZB_ZCL_CLUSTER_ID_OTA_UPGRADE 0x0019U /*!< Over The Air cluster identifier. */

/* General clusters */
#define ZB_ZCL_CLUSTER_ID_POLL_CONTROL 0x0020U /*!< Poll control cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_GREEN_POWER  0x0021U /*!< Green Power cluster identifier. */

#define ZB_ZCL_CLUSTER_ID_KEEP_ALIVE 0x0025U /*!< Keep Alive cluster identifier. */

/* Closures clusters */
#define ZB_ZCL_CLUSTER_ID_SHADE_CONFIG    0x0100U /*!< Shade configuration cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_DOOR_LOCK       0x0101U /*!< Door lock cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_WINDOW_COVERING 0x0102U /*!< Window covering cluster identifier. */
/* HVAC clusters */
#define ZB_ZCL_CLUSTER_ID_PUMP_CONFIG_CONTROL  0x200U /*!< Pump configuration and control cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_THERMOSTAT           0x201U /*!< Thermostat cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_FAN_CONTROL          0x202U /*!< Fan control cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_DEHUMID_CONTROL      0x203U /*!< Dehumidification control cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_THERMOSTAT_UI_CONFIG 0x204U /*!< Thermostat user interface configuration cluster identifier. */

/* Lighting clusters */
#define ZB_ZCL_CLUSTER_ID_COLOR_CONTROL  0x0300U /*!< Color control cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_BALLAST_CONFIG 0x0301U /*!< Ballast configuration cluster identifier. */

/* Measurement and Sensing */
#define ZB_ZCL_CLUSTER_ID_ILLUMINANCE_MEASUREMENT  0x0400U /*!< Illuminance measurement. */
#define ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT         0x0402U /*!< Temperature measurement. */
#define ZB_ZCL_CLUSTER_ID_PRESSURE_MEASUREMENT     0x0403U /*!< Pressure measurement. */
#define ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT 0x0405U /*!< Relative humidity measurement. */
#define ZB_ZCL_CLUSTER_ID_OCCUPANCY_SENSING        0x0406U /*!< Occupancy sensing. */

/* Measurement and Sensing: Concentration Measurement */
#define ZB_ZCL_CLUSTER_ID_CARBON_DIOXIDE_MEASUREMENT 0x040DU /*!< Carbon Dioxide (CO2) measurement. */
#define ZB_ZCL_CLUSTER_ID_PM2_5_MEASUREMENT          0x042AU /*!< PM2.5 measurement. */

/* IAS clusters */
#define ZB_ZCL_CLUSTER_ID_IAS_ZONE 0x0500U /*!< IAS Zone cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_IAS_ACE  0x0501U /*!< IAS ACE cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_IAS_WD   0x0502U /*!< IAS WD cluster identifier. */

/* Smart Energy */
#define ZB_ZCL_CLUSTER_ID_PRICE             0x0700U /*!< Price cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_DRLC              0x0701U /*!< Demand-Response cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_METERING          0x0702U /*!< Metering cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_MESSAGING         0x0703U /*!< Messaging cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_TUNNELING         0x0704U /*!< Tunneling cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_PREPAYMENT        0x0705U /*!< Prepayment cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_ENERGY_MANAGEMENT 0x0706U /*!< Energy Management cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_CALENDAR          0x0707U /*!< Calendar cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_DEVICE_MANAGEMENT 0x0708U /*!< Device Management cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_EVENTS            0x0709U /*!< Events cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_MDU_PAIRING       0x070AU /*!< MDU Pairing cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_SUB_GHZ           0x070BU /*!< Sub-GHz cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE    0x070DU /*!< Daily Schedule cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT 0x0800U /*!< Key Establishment cluster identifier. */


/* Home Automation */
#define ZB_ZCL_CLUSTER_ID_APPLIANCE_EVENTS_AND_ALERTS 0x0b02U /*!< Appliance events and alerts cluster identifier. */
/* HA specU rev. 29U ver. 1.2 - Electricity MeasurementU not sure that its Electrical Measurement */
#define ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT      0x0b04U /*!< Electrical Measurement cluster identifier. */
#define ZB_ZCL_CLUSTER_ID_DIAGNOSTICS                 0x0b05U /*!< Home Automation Diagnostics. */

#define ZB_ZCL_CLUSTER_ID_WWAH                    0xFC57U /*!< Works with All Hubs cluster identifier. */
/** @cond touchlink */
#define ZB_ZCL_CLUSTER_ID_TOUCHLINK_COMMISSIONING 0x1000U       /*!< Touchlink Commissioning cluster identifier. */
/** @endcond */ /* touchlink */

/************** Manufacturer specific clusters ****************/

#define ZB_ZCL_CLUSTER_ID_TUNNEL     0xfc00U /*!< Manufacturer specific Tunnel cluster. */
#define ZB_ZCL_CLUSTER_ID_IR_BLASTER 0xfc01U /*!< Manufacturer specific IR Blaster cluster. */

#define ZB_ZCL_CLUSTER_ID_CUSTOM_ATTR 0xffeeU /*!< Custom Attributes. */

#define ZB_ZCL_CLUSTER_ID_METER_IDENTIFICATION 0x0B01U /*!< Meter Identification cluster identifier. */

/* TODO: define other clusters IDs */
/* Measurements and sensing clusters */
/* Security and safety clusters */

/** @} */

/**
 * @name ZCL attribute data type values
 * @anchor zcl_attr_type
 * @note The enumeration is not full, and does not contain ZCL types not used in HA profile.
 * @see ZCL spec., subclause 2.5.2, Table 2-10. Data Types
 */
/** @{ */
#define ZB_ZCL_ATTR_TYPE_NULL              0x00U /*!< Null data type. */
#define ZB_ZCL_ATTR_TYPE_8BIT              0x08U /*!< 8-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_16BIT             0x09U /*!< 16-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_24BIT             0x0aU /*!< 24-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_32BIT             0x0bU /*!< 32-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_40BIT             0x0cU /*!< 40-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_48BIT             0x0dU /*!< 48-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_56BIT             0x0eU /*!< 56-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_64BIT             0x0fU /*!< 64-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_BOOL              0x10U /*!< Boolean data type. */
#define ZB_ZCL_ATTR_TYPE_8BITMAP           0x18U /*!< 8-bit bitmap data type. */
#define ZB_ZCL_ATTR_TYPE_16BITMAP          0x19U /*!< 16-bit bitmap data type. */
#define ZB_ZCL_ATTR_TYPE_24BITMAP          0x1aU /*!< 24-bit bitmap data type. */
#define ZB_ZCL_ATTR_TYPE_32BITMAP          0x1bU /*!< 32-bit bitmap data type. */
#define ZB_ZCL_ATTR_TYPE_40BITMAP          0x1cU /*!< 40-bit bitmap data type. */
#define ZB_ZCL_ATTR_TYPE_48BITMAP          0x1dU /*!< 48-bit bitmap data type. */
#define ZB_ZCL_ATTR_TYPE_56BITMAP          0x1eU /*!< 56-bit bitmap data type. */
#define ZB_ZCL_ATTR_TYPE_64BITMAP          0x1fU /*!< 64-bit bitmap data type. */
#define ZB_ZCL_ATTR_TYPE_U8                0x20U /*!< Unsigned 8-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_U16               0x21U /*!< Unsigned 16-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_U24               0x22U /*!< Unsigned 24-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_U32               0x23U /*!< Unsigned 32-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_U40               0x24U /*!< Unsigned 40-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_U48               0x25U /*!< Unsigned 48-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_U56               0x26U /*!< Unsigned 56-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_U64               0x27U /*!< Unsigned 64-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_S8                0x28U /*!< Signed 8-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_S16               0x29U /*!< Signed 16-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_S24               0x2aU /*!< Signed 24-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_S32               0x2bU /*!< Signed 32-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_S40               0x2cU /*!< Signed 40-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_S48               0x2dU /*!< Signed 48-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_S56               0x2eU /*!< Signed 56-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_S64               0x2fU /*!< Signed 64-bit value data type. */
#define ZB_ZCL_ATTR_TYPE_8BIT_ENUM         0x30U /*!< 8-bit enumeration (U8 discrete) data type. */
#define ZB_ZCL_ATTR_TYPE_16BIT_ENUM        0x31U /*!< 16-bit enumeration (U16 discrete) data type. */
#define ZB_ZCL_ATTR_TYPE_SEMI              0x38U /*!< 2 byte floating point. */
#define ZB_ZCL_ATTR_TYPE_SINGLE            0x39U /*!< 4 byte floating point. */
#define ZB_ZCL_ATTR_TYPE_DOUBLE            0x3aU /*!< 8 byte floating point. */
#define ZB_ZCL_ATTR_TYPE_OCTET_STRING      0x41U /*!< Octet string data type. */
#define ZB_ZCL_ATTR_TYPE_CHAR_STRING       0x42U /*!< Character string (array) data type. */
#define ZB_ZCL_ATTR_TYPE_LONG_OCTET_STRING 0x43U /*!< Long octet string. */
#define ZB_ZCL_ATTR_TYPE_LONG_CHAR_STRING  0x44U /*!< Long character string. */
#define ZB_ZCL_ATTR_TYPE_ARRAY             0x48U /*!< Array data type 2 + sum of content length. */
#define ZB_ZCL_ATTR_TYPE_STRUCTURE         0x4cU /*!< Structure data type 2 + sum of content length. */
#define ZB_ZCL_ATTR_TYPE_SET               0x50U /*!< Collection:set, size = sum of length of content. */
#define ZB_ZCL_ATTR_TYPE_BAG               0x51U /*!< Collection:bag, size = sum of length of content. */
#define ZB_ZCL_ATTR_TYPE_TIME_OF_DAY       0xe0U /*!< Time of day, 4 bytes. */
#define ZB_ZCL_ATTR_TYPE_DATE              0xe1U /*!< Date, 4 bytes. */
#define ZB_ZCL_ATTR_TYPE_UTC_TIME          0xe2U /*!< UTC Time, 4 bytes. */
#define ZB_ZCL_ATTR_TYPE_CLUSTER_ID        0xe8U /*!< Cluster ID, 2 bytes. */
#define ZB_ZCL_ATTR_TYPE_ATTRIBUTE_ID      0xe9U /*!< Attribute ID, 2 bytes. */
#define ZB_ZCL_ATTR_TYPE_BACNET_OID        0xeaU /*!< BACnet OID, 4 bytes. */
#define ZB_ZCL_ATTR_TYPE_IEEE_ADDR         0xf0U /*!< IEEE address (U64) type. */
#define ZB_ZCL_ATTR_TYPE_128_BIT_KEY       0xf1U /*!< 128-bit security key. */

/** Custom array of 32 elements data type (now is equal to ZB_ZCL_ATTR_TYPE_ARRAY). */
#define ZB_ZCL_ATTR_TYPE_CUSTOM_32ARRAY 0x4aU

#define ZB_ZCL_ATTR_TYPE_INVALID 0xffU /*!< Invalid data type. */
/** @} */

/**
 *  @brief Puts 8-bit value to the packet.
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA8(ptr, val) (*(ptr) = (val), (ptr)++)

/**
  Puts 16-bit value to the packet.
  @param ptr Pointer to the place to put value to
  @param val Pointer to the value to put into the packet
*/
#define ZB_ZCL_PACKET_PUT_DATA16(ptr, val)      \
do                                         \
 {                                              \
   ZB_HTOLE16((ptr), (val));                    \
   (ptr) += 2;                                  \
} while (ZB_FALSE)

/**
 *  @brief Puts 32-bit value to the packet.
 *  @param ptr Pointer to the place to put value to
 *  @param val Pointer to the value to put into the packet
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA32_VAL(ptr, val)  ZB_PUT_NEXT_HTOLE32((ptr),(val))

/**
 *  @brief Puts 32-bit value to the packet.
 *  @param ptr Pointer to the place to put value to
 *  @param val Pointer to the value to put into the packet
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA32(ptr, val)       \
   (ZB_HTOLE32(ptr, val), (ptr) += 4)

/**
 *  @brief Puts 48-bit value to the packet.
 *  @param ptr Pointer to the place to put value to
 *  @param val Value to put into the packet
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA48_VAL(ptr, val) \
    (PUT_DATA48_VAL(ptr, val))

/**
 *  @brief Puts 48-bit value to the packet.
 *  @param ptr Pointer to the place to put value to
 *  @param val Pointer to the value to put into the packet
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA48(ptr, val)       \
   (PUT_DATA48(ptr, val))

/**
 *  @brief Puts 24-bit value to the packet.
 *  @param ptr Pointer to the place to put value to
 *  @param val Value to put into the packet
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA24_VAL(ptr, val) \
    (PUT_DATA24_VAL(ptr, val))

/**
 *  @brief Puts 24-bit value to the packet.
 *  @param ptr Pointer to the place to put value to
 *  @param val Pointer to the value to put into the packet
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA24(ptr, val)       \
    (PUT_DATA24(ptr, val))



/**
 *  @brief Puts 64-bit value to the packet.
 *  @param ptr Pointer to the place to put value to
 *  @param val Pointer to the value to put into the packet
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA64(ptr, val) (ZB_HTOLE64(ptr, val), (ptr) += 8)

/**
 *  @brief Puts N byte data to the packet.
 *  @param ptr Pointer to the place to put value to
 *  @param val Pointer to the value to put into the packet
 *  @param n Number of bytes to be copied
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA_N(ptr, val, n) \
    (ZB_MEMCPY(ptr, val, n), (ptr) += (n))

/** @brief Get 8-bit value from packet.
  * @param dst_ptr - pointer to the memory to store value (pointer value preserved).
  * @param src_ptr - pointer to the memory to get value from (will be incremented).
  */
#define ZB_ZCL_PACKET_GET_DATA8(dst_ptr, src_ptr) \
{                                                 \
  (dst_ptr) = *((src_ptr)++);                     \
}

/**
 *  @brief Put N byte data to packet.
 *  @param ptr - pointer to the place to put value to.
 *  @param val - pointer to the value to put into packet.
 *  @param n - number of bytes to be copied.
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA_N(ptr, val, n) \
    (ZB_MEMCPY(ptr, val, n), (ptr) += (n))

/** @brief Get 16-bit value from packet.
  * @param dst_ptr - pointer to the memory to store value (pointer value preserved).
  * @param src_ptr - pointer to the memory to get value from (will be incremented by value size).
  */
#define ZB_ZCL_PACKET_GET_DATA16(dst_ptr, src_ptr) \
{                                                  \
  ZB_LETOH16((dst_ptr), (src_ptr));                \
  (src_ptr) += sizeof(zb_uint16_t);                \
}

/** @brief Get 24-bit value from packet.
  * @param dst_ptr - pointer to the memory to store value (pointer value preserved).
  * @param src_ptr - pointer to the memory to get value from (will be incremented by value size).
  */
#define ZB_ZCL_PACKET_GET_DATA24(dst_ptr, src_ptr) \
{                                                  \
  ZB_LETOH24(dst_ptr, src_ptr);                    \
  (src_ptr) += ZB_24BIT_SIZE;                \
}


/** @brief Get 48-bit value from packet.
  * @param dst_ptr - pointer to the memory to store value (pointer value preserved).
  * @param src_ptr - pointer to the memory to get value from (will be incremented by value size).
  */
#define ZB_ZCL_PACKET_GET_DATA48(dst_ptr, src_ptr) \
{                                                  \
  ZB_LETOH48(dst_ptr, src_ptr);                    \
  (src_ptr) += ZB_48BIT_SIZE;                \
}


/** @brief Get 32-bit value from packet.
  * @param dst_ptr - pointer to the memory to store value (pointer value preserved).
  * @param src_ptr - pointer to the memory to get value from (will be incremented by value size).
  */
#define ZB_ZCL_PACKET_GET_DATA32(dst_ptr, src_ptr) \
{                                                  \
  ZB_LETOH32((dst_ptr), (src_ptr));                \
  (src_ptr) += sizeof(zb_uint32_t);                \
}

/** @brief Get 64-bit value from packet.
  * @param dst_ptr - pointer to the memory to store value (pointer value preserved).
  * @param src_ptr - pointer to the memory to get value from (will be incremented by value size).
  */
#define ZB_ZCL_PACKET_GET_DATA64(dst_ptr, src_ptr) \
{                                                  \
  ZB_LETOH64((dst_ptr), (src_ptr));                \
  (src_ptr) += 8;                                  \
}

/** @brief Get N-byte value from packet.
  * @param dst_ptr - pointer to the memory to store value (will be incremented by value size).
  * @param src_ptr - pointer to the memory to get value from (pointer value preserved).
  * @param n - data length.
  */
#define ZB_ZCL_PACKET_GET_DATA_N(dst_ptr, src_ptr, n) \
{                                                     \
  ZB_MEMCPY((dst_ptr), (src_ptr), (n));              \
  (src_ptr) += (n);                                  \
}

/** @cond internals_doc */
/** @internal @brief Calculates array size */
#define ZB_ZCL_ARRAY_SIZE(ar, type) (sizeof(ar)/sizeof(type))

/** @internal @brief Calculates byte array size (add 2 bytes for full length). */
#define ZB_ZCL_ARRAY_GET_SIZE(ar, val) ZB_LETOH16(ar, val)
#define ZB_ZCL_ARRAY_SET_SIZE(ar, val) ZB_HTOLE16_VAL(ar, val)

/** @internal @brief Calculates 32-byte array size (add 2 bytes for full length). */
#define ZB_BYTE_32ARRAY_GET_SIZE(ar, val) { ZB_ZCL_ARRAY_GET_SIZE(ar, val); *(zb_uint16_t*)(ar) *= 4U; }
#define ZB_BYTE_32ARRAY_SET_SIZE(ar, val) { ZB_ZCL_ARRAY_SET_SIZE(ar, val); *(zb_uint16_t*)(ar) /= 4U; }

#define ZB_ZCL_NULL_EP_ID (zb_uint8_t)(-1)
#define ZB_ZCL_NULL_ID (zb_uint16_t)(-1)
#define ZB_ZCL_NULL_STRING (zb_uint8_t)(0)

/** @internal @brief Maximum size of Character String (with Length octet) */
#define ZB_ZCL_MAX_STRING_SIZE 0xFFU
#define ZB_ZCL_INVALID_STRING_VALUE 0xFFU
#define ZB_ZCL_INVALID_ARRAY_VALUE 0xFFFFU

/** @endcond */

typedef ZB_PACKED_PRE  struct zb_zcl_attr_s
{
  zb_uint16_t id;     /*!< Attribute id */
  zb_uint8_t type;    /*!< Attribute type see @ref zcl_attr_type */
  zb_uint8_t access;  /*!< Attribute access options according to @ref zcl_attr_access */
  zb_uint16_t manuf_code; /*!< Manufacturer specific ID */
  void* data_p;  /*!< Pointer to data */
} ZB_PACKED_STRUCT
zb_zcl_attr_t;

/**
 * @name Global attributes that are included to each cluster
 * @anchor zcl_attr_global
 * @see ZCL spec - 6.0
 */
/** @{ */
#define ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID           0xfffdU /*!< Cluster revision. */
#define ZB_ZCL_ATTR_GLOBAL_ATTRIBUTE_REPORTING_STATUS_ID 0xfffeU /*!< Attribute reporting status. */
/** @} */

/**
 * @name ZCL attribute access values
 * @anchor zcl_attr_access
 */
/** @{ */
#define ZB_ZCL_ATTR_ACCESS_READ_ONLY  0x01U /*!< Attribute is read only. */
#define ZB_ZCL_ATTR_ACCESS_WRITE_ONLY 0x02U /*!< Attribute is write only. */
/** Attribute is read/write. */
#define ZB_ZCL_ATTR_ACCESS_READ_WRITE (ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_WRITE_ONLY)
#define ZB_ZCL_ATTR_ACCESS_REPORTING  0x04U /*!< Attribute is allowed for reporting. */
  /** @cond internals_doc */
/** Attribute is read only, but may be marked as writable due to ZCL specification (using
                                                                      * ZB_ZCL_SET_ATTR_WRITABLE) */
#define ZB_ZCL_ATTR_ACCESS_WRITE_OPTIONAL (ZB_ZCL_ATTR_ACCESS_READ_ONLY)
  /** @endcond */ /* internals_doc */
#define ZB_ZCL_ATTR_ACCESS_SINGLETON  0x08U /*!< Attribute is singleton. */
#define ZB_ZCL_ATTR_ACCESS_SCENE      0x10U /*!< Attribute is accessed through scene. */

  /* Use free bit in access attribute field to save RAM */
#define ZB_ZCL_ATTR_MANUF_SPEC        0x20U /*!< Attribute is manufacturer specific. */
#define ZB_ZCL_ATTR_ACCESS_INTERNAL   0x40U /*!< ZBOSS Internal access only Attribute. */
/** @} */

#define ZB_ZCL_MANUFACTURER_SPECIFIC     0x01U

#define ZB_ZCL_NON_MANUFACTURER_SPECIFIC 0xFFFFU

/** @brief ZCL 6.0: Default value for cluster revision global attribute,
 *  see @ref zcl_attr_global
 */
#define ZB_ZCL_CLUSTER_REVISION_DEFAULT 1

/*! @internal @brief Sets attribute description.
    @param attr_id Attribute identifier (defined individually for any particular cluster)
    @param data_ptr Pointer to attribute's value storage

    Creates attribute description value (@ref zb_zcl_attr_s) initialized with provided pointer to
    attribute's data.
*/

/*! @internal @brief Start declaration of attributes list */
#define ZB_ZCL_START_DECLARE_ATTRIB_LIST(attrs_desc_name)                                          \
  zb_uint16_t cluster_revision_##attrs_desc_name = ZB_ZCL_CLUSTER_REVISION_DEFAULT;                \
  zb_zcl_attr_t attrs_desc_name [] = {                                                             \
  {                                                                                                \
    ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID,                                                        \
    ZB_ZCL_ATTR_TYPE_U16,                                                                          \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                  \
    ZB_ZCL_NON_MANUFACTURER_SPECIFIC,                                                              \
    (void*) &(cluster_revision_##attrs_desc_name)                                                  \
  },

#define ZB_ZCL_SET_ATTR_DESC(attr_id, data_ptr) ZB_SET_ATTR_DESCR_WITH_##attr_id(data_ptr),

/*! @internal @brief End declaration of attributes list */
#define ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST \
  {                                       \
    ZB_ZCL_NULL_ID,                       \
    0,                                    \
    0,                                    \
    ZB_ZCL_NON_MANUFACTURER_SPECIFIC,     \
    NULL                                  \
  }                                       \
}

/*! @internal @brief Start declaration of attributes list with cluster revision */
#define ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attrs_desc_name, cluster_name)           \
  zb_uint16_t cluster_revision_##attrs_desc_name = cluster_name##_CLUSTER_REVISION_DEFAULT;        \
  zb_zcl_attr_t attrs_desc_name [] = {                                                             \
  {                                                                                                \
    ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID,                                                        \
    ZB_ZCL_ATTR_TYPE_U16,                                                                          \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                  \
    ZB_ZCL_NON_MANUFACTURER_SPECIFIC,                                                              \
    (void*) &(cluster_revision_##attrs_desc_name)                                                  \
  },

/* Attribute count on a list, except for the cluster revision ID at the 
beginning and the empty one at the end */
#define ZB_ZCL_ATTRIB_LIST_GET_COUNT(list)    ZB_ARRAY_SIZE((list)) - 2

/* Pointer of the first attribute on a list, after cluster revision ID */
#define ZB_ZCL_ATTRIB_LIST_GET_PTR(list)      (zb_zcl_attr_t *)(list) + 1

/*! @} */

#endif /* ZB_ZCL_COMMON_H */
