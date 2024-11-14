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
/* PURPOSE: Power Configuration cluster definitions
*/

#ifndef ZB_ZCL_POWER_CONFIG_H
#define ZB_ZCL_POWER_CONFIG_H 1

/*! \addtogroup zclPowerConfig */
/*! @{ */

/*! @brief Power Configuration cluster attribute identifiers
    @see ZCL spec, Power Configuration Cluster 3.3.2.2
*/
enum zb_zcl_power_config_attr_e
{
  /** @brief MainsVoltage attribute, ZCL spec 3.3.2.2.1.1 */
  ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_ID                     = 0x0000,
  /** @brief MainsFrequency attribute, ZCL spec 3.3.2.2.1.2 */
  ZB_ZCL_ATTR_POWER_CONFIG_MAINS_FREQUENCY_ID                   = 0x0001,
  /** @brief MainsAlarmMask attribute, ZCL spec 3.3.2.2.2.1 */
  ZB_ZCL_ATTR_POWER_CONFIG_MAINS_ALARM_MASK_ID                  = 0x0010,
  /** @brief MainsVoltageMinThreshold attribute, ZCL spec 3.3.2.2.2.2 */
  ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_MIN_THRESHOLD          = 0x0011,
  /** @brief MainsVoltageMaxThreshold attribute, ZCL spec 3.3.2.2.2.3 */
  ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_MAX_THRESHOLD          = 0x0012,
  /** @brief MainsVoltageDwellTripPoint attribute, ZCL spec 3.3.2.2.2.4 */
  ZB_ZCL_ATTR_POWER_CONFIG_MAINS_DWELL_TRIP_POINT               = 0x0013,

  /** @brief BatteryVoltage attribute, ZCL spec 3.3.2.2.3.1 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_ID                   = 0x0020,
  /*! @brief BatteryPercentageRemaining attribute, ZCL spec 3.3.2.2.3.2 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_REMAINING_ID      = 0x0021,

  /** BatteryManufacturer attribute is a maximum of 16 bytes in length
   *  and specifies the name of the battery manufacturer as a character string. */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_MANUFACTURER_ID              = 0x0030,
  /** @brief BatterySize attribute, ZCL spec 3.3.2.2.4.2 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_SIZE_ID                      = 0x0031,
  /** The BatteryAHrRating attribute is 16 bits in length and specifies
   *  the Ampere-hour rating of the battery, measured in units of 10mAHr. */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_A_HR_RATING_ID               = 0x0032,
  /** @brief BatteryQuantity attribute, ZCL spec 3.3.2.2.4.4 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_QUANTITY_ID                  = 0x0033,
  /** @brief BatteryRatedVoltage attribute, ZCL spec 3.3.2.2.4.5 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_RATED_VOLTAGE_ID             = 0x0034,
  /** @brief BatteryAlarmMask attribute, ZCL spec 3.3.2.2.4.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_MASK_ID                = 0x0035,
  /** @brief BatteryVoltageMinThreshold attribute, ZCL spec 3.3.2.2.4.7 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_MIN_THRESHOLD_ID     = 0x0036,
    /*! @brief BatteryVoltageThreshold1 attribute, ZCL spec 3.3.2.2.4.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD1_ID        = 0x0037,
  /*! @brief BatteryVoltageThreshold2 attribute, ZCL spec 3.3.2.2.4.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD2_ID        = 0x0038,
  /*! @brief BatteryVoltageThreshold3 attribute, ZCL spec 3.3.2.2.4.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD3_ID        = 0x0039,

  /*! @brief BatteryPercentageMinThreshold attribute, ZCL spec 3.3.2.2.4.9 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_MIN_THRESHOLD_ID  = 0x003a,

  /*! @brief BatteryPercentageThreshold1 attribute, ZCL spec 3.3.2.2.4.10 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD1_ID     = 0x003b,
  /*! @brief BatteryPercentageThreshold2 attribute, ZCL spec 3.3.2.2.4.10 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD2_ID     = 0x003c,
  /*! @brief BatteryPercentageThreshold3 attribute, ZCL spec 3.3.2.2.4.10 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD3_ID     = 0x003d,

  /*! @brief BatteryAlarmState attribute, ZCL spec 3.3.2.2.4.11 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_STATE_ID               = 0x003e,

  /*! @brief Battery Information 2 BatteryVoltage attribute, ZCL spec 3.3.2.2.5 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_ID                  = 0x0040,
  /*! @brief Battery Information 2 BatteryPercentageRemaining attribute, ZCL spec 3.3.2.2.5 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_REMAINING_ID     = 0x0041,

  /*! @brief Battery Settings 2 BatteryManufacturer attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_MANUFACTURER_ID             = 0x0050,
  /*! @brief Battery Settings 2 BatterySize attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_SIZE_ID                     = 0x0051,
  /*! @brief Battery Settings 2 BatteryAHrRating attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_A_HR_RATING_ID              = 0x0052,
  /*! @brief Battery Settings 2 BatteryQuantity attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_QUANTITY_ID                 = 0x0053,
  /*! @brief Battery Settings 2 BatteryRatedVoltage attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_RATED_VOLTAGE_ID            = 0x0054,
  /*! @brief Battery Settings 2 BatteryAlarmMask attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_ALARM_MASK_ID               = 0x0055,
  /*! @brief Battery Settings 2 BatteryVoltageMinThreshold attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_MIN_THRESHOLD_ID    = 0x0056,
  /*! @brief Battery Settings 2 BatteryVoltageThreshold1 attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD1_ID       = 0x0057,
  /*! @brief Battery Settings 2 BatteryVoltageThreshold2 attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD2_ID       = 0x0058,
  /*! @brief Battery Settings 2 BatteryVoltageThreshold3 attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD3_ID       = 0x0059,
  /*! @brief Battery Settings 2 BatteryPercentageMinThreshold attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_MIN_THRESHOLD_ID = 0x005a,
  /*! @brief Battery Settings 2 BatteryPercentageThreshold1 attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_THRESHOLD1_ID    = 0x005b,
  /*! @brief Battery Settings 2 BatteryPercentageThreshold2 attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_THRESHOLD2_ID    = 0x005c,
  /*! @brief Battery Settings 2 BatteryPercentageThreshold3 attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_THRESHOLD3_ID    = 0x005d,
  /*! @brief Battery Settings 2 BatteryAlarmState attribute, ZCL spec 3.3.2.2.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_ALARM_STATE_ID              = 0x005e,

  /*! @brief Battery Information 3 BatteryVoltage attribute, ZCL spec 3.3.2.2.7 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_VOLTAGE_ID                  = 0x0060,
  /*! @brief Battery Information 3 BatteryPercentageRemaining attribute, ZCL spec 3.3.2.2.7 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_PERCENTAGE_REMAINING_ID     = 0x0061,

  /*! @brief Battery Settings 3 BatteryManufacturer attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_MANUFACTURER_ID             = 0x0070,
  /*! @brief Battery Settings 3 BatterySize attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_SIZE_ID                     = 0x0071,
  /*! @brief Battery Settings 3 BatteryAHrRating attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_A_HR_RATING_ID              = 0x0072,
  /*! @brief Battery Settings 3 BatteryQuantity attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_QUANTITY_ID                 = 0x0073,
  /*! @brief Battery Settings 3 BatteryRatedVoltage attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_RATED_VOLTAGE_ID            = 0x0074,
  /*! @brief Battery Settings 3 BatteryAlarmMask attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_ALARM_MASK_ID               = 0x0075,
  /*! @brief Battery Settings 3 BatteryVoltageMinThreshold attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_VOLTAGE_MIN_THRESHOLD_ID    = 0x0076,
  /*! @brief Battery Settings 3 BatteryVoltageThreshold1 attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_VOLTAGE_THRESHOLD1_ID       = 0x0077,
  /*! @brief Battery Settings 3 BatteryVoltageThreshold2 attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_VOLTAGE_THRESHOLD2_ID       = 0x0078,
  /*! @brief Battery Settings 3 BatteryVoltageThreshold3 attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_VOLTAGE_THRESHOLD3_ID       = 0x0079,
  /*! @brief Battery Settings 3 BatteryPercentageMinThreshold attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_PERCENTAGE_MIN_THRESHOLD_ID = 0x007a,
  /*! @brief Battery Settings 3 BatteryPercentageThreshold1 attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_PERCENTAGE_THRESHOLD1_ID    = 0x007b,
  /*! @brief Battery Settings 3 BatteryPercentageThreshold2 attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_PERCENTAGE_THRESHOLD2_ID    = 0x007c,
  /*! @brief Battery Settings 3 BatteryPercentageThreshold3 attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_PERCENTAGE_THRESHOLD3_ID    = 0x007d,
  /*! @brief Battery Settings 3 BatteryAlarmState attribute, ZCL spec 3.3.2.2.8 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_ALARM_STATE_ID              = 0x007e,
  /** @cond internals_doc */
  /* Custom attributes */
  ZB_ZCL_ATTR_POWER_CONFIG_CUSTOM_BATTERY_VOLTAGE_SHARP_ID      = 0x8020
  /*! @}
   *  @endcond */ /* internals_doc */
};

/**
 * @brief Power Configuration MainsAlarmMask value
 * @see ZCL spec 3.3.2.2.4.7
 */
enum zb_zcl_power_config_mains_alarm_mask_e
{
  /** @brief MainsAlarmMask - Mains Voltage too low */
  ZB_ZCL_POWER_CONFIG_MAINS_ALARM_MASK_VOLTAGE_LOW = 0x01,
  /** @brief MainsAlarmMask - Mains Voltage too high */
  ZB_ZCL_POWER_CONFIG_MAINS_ALARM_MASK_VOLTAGE_HIGH = 0x02,
  /** @brief MainsAlarmMask - Mains power supply lost/unavailable */
  ZB_ZCL_POWER_CONFIG_MAINS_ALARM_MASK_VOLTAGE_UNAVAIL = 0x04,

};

/** @brief Default value for Power Configuration cluster revision global attribute */
#define ZB_ZCL_POWER_CONFIG_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0002u)

/** @brief Default value for MainsAlarmMask attribute */
#define ZB_ZCL_POWER_CONFIG_MAINS_ALARM_MASK_DEFAULT_VALUE ((zb_uint8_t)0x00)

/** @brief Default value for MainsVoltageMinThreshold attribute */
#define ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_MIN_THRESHOLD_DEFAULT_VALUE ((zb_uint16_t)0x0000)

/** @brief Default value for MainsVoltageMaxThreshold attribute */
#define ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_MAX_THRESHOLD_DEFAULT_VALUE ((zb_uint16_t)0xFFFF)

/** @brief Default value for MainsDwellTripPoint attribute */
#define ZB_ZCL_POWER_CONFIG_MAINS_DWELL_TRIP_POINT_DEFAULT_VALUE ((zb_uint16_t)0x0000)

/** @brief BatteryVoltage attribute invalid value */
#define ZB_ZCL_POWER_CONFIG_BATTERY_VOLTAGE_INVALID         0xff

/** @brief Power Configuration BatteryPercentageRemaining attribute unknown value */
#define ZB_ZCL_POWER_CONFIG_BATTERY_REMAINING_UNKNOWN            0xff

/** @brief Default value for BatteryRemainingHA attribute */
#define ZB_ZCL_POWER_CONFIG_BATTERY_REMAINING_HA_DEFAULT_VALUE ((zb_uint8_t)0)

/** @brief Default value for BatteryManufacturer attribute */
#define ZB_ZCL_POWER_CONFIG_BATTERY_MANUFACTURER_DEFAULT_VALUE {0}

/** @brief BatterySize attribute default value */
#define ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_DEFAULT_VALUE          0xff

/** @brief BatteryAlarmMask attribute default value */
#define ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_MASK_DEFAULT_VALUE    0x00

/** @brief Default value for BatteryVoltageMinThreshold attribute */
#define ZB_ZCL_POWER_CONFIG_BATTERY_VOLTAGE_MIN_THRESHOLD_DEFAULT_VALUE ((zb_uint8_t)0x0000)

/** @brief Default value for BatteryVoltageThreshold1 attribute */
#define ZB_ZCL_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD1_DEFAULT_VALUE ((zb_uint8_t)0x00)

/** @brief Default value for BatteryVoltageThreshold2 attribute */
#define ZB_ZCL_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD2_DEFAULT_VALUE ((zb_uint8_t)0x00)

/** @brief Default value for BatteryVoltageThreshold3 attribute */
#define ZB_ZCL_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD3_DEFAULT_VALUE ((zb_uint8_t)0x00)

/** @brief Default value for BatteryPercentageMinThreshold attribute */
#define ZB_ZCL_POWER_CONFIG_BATTERY_PERCENTAGE_MIN_THRESHOLD_DEFAULT_VALUE ((zb_uint8_t)0x00)

/** @brief Default value for BatteryPercentageThreshold1 attribute */
#define ZB_ZCL_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD1_DEFAULT_VALUE ((zb_uint8_t)0x00)

/** @brief Default value for BatteryPercentageThreshold2 attribute */
#define ZB_ZCL_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD2_DEFAULT_VALUE ((zb_uint8_t)0x00)

/** @brief Default value for BatteryPercentageThreshold3 attribute */
#define ZB_ZCL_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD3_DEFAULT_VALUE ((zb_uint8_t)0x00)

/** @brief Default value for BatteryAlarmState attribute */
#define ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_DEFAULT_VALUE ((zb_uint32_t)0x00000000)

/** @brief MainsVoltageMinThreshold and MainsVoltageMaxThreshold values when alarm should not be generated*/
#define ZB_ZCL_POWER_CONFIG_THRESHOLD_ALARM_OMISSION_VALUE ((zb_uint16_t)0xFFFF)

/** @brief Mains attribute set the least significant nibble*/
#define ZB_ZCL_POWER_CONFIG_MAINS_ATTRIBUTE_SET 0

/** @brief Battery attribute set the least significant nibble*/
#define ZB_ZCL_POWER_CONFIG_BATTERY_ATTRIBUTE_SET 0

/** @brief Battery Source 2 attribute set the least significant nibble*/
#define ZB_ZCL_POWER_CONFIG_BATTERY_SOURCE_2_ATTRIBUTE_SET 0x20

/** @brief Battery Source 3 attribute set the least significant nibble*/
#define ZB_ZCL_POWER_CONFIG_BATTERY_SOURCE_3_ATTRIBUTE_SET 0x40

/**
 * @brief Power Configuration BatterySize value
 * @see ZCL spec 3.3.2.2.4.2
 */
enum zb_zcl_power_config_battery_size_e
{
  /** @brief BatterySize - no battery*/
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_NO_BATTERY = 0,
  /** @brief BatterySize - built in */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_BUILT_IN   = 1,
  /** @brief BatterySize - other */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_OTHER      = 2,
  /** @brief BatterySize - AA */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_AA         = 3,
  /** @brief BatterySize - AAA */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_AAA        = 4,
  /** @brief BatterySize - C */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_C          = 5,
  /** @brief BatterySize - D */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_D          = 6,
  /** @brief BatterySize - CR2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_CR2        = 7,
  /** @brief BatterySize - CR123A */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_CR123A     = 8,
  /** @brief BatterySize - unknown */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_UNKNOWN    = 0xff,
};

/**
 * @brief Power Configuration BatteryAlarmMask value
 * @see ZCL spec 3.3.2.2.4.7
 */
enum zb_zcl_power_config_battery_alarm_mask_e
{
  /** @brief BatteryAlarmMask - Battery voltage too low */
  ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_MASK_VOLTAGE_LOW = 0,
  /** @brief BatteryAlarmMask - Alarm1 */
  ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_MASK_ALARM1 = 1,
  /** @brief BatteryAlarmMask - Alarm2 */
  ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_MASK_ALARM2 = 2,
  /** @brief BatteryAlarmMask - Alarm3 */
  ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_MASK_ALARM3 = 3,
};

/*! @brief Power Configuration Alarm Code for Alarm
    @see ZCL spec 3.3.2.2.4.7, Table 3-25
*/
enum zb_zcl_power_config_battery_alarm_code_e
{
  /** @brief MainsVoltageAlarmCode - MainsVoltageMinThreshold reached for Mains Voltage**/
    ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_ALARM_CODE_MIN_THRESHOLD = 0x00,
  /** @brief MainsVoltageAlarmCode - MainsVoltageMaxThreshold reached for Mains Voltage**/
    ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_ALARM_CODE_MAX_THRESHOLD = 0x01,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold or
   * BatteryPercentageMinThreshold reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE1_MIN_THRESHOLD = 0x10,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold1 or
   * BatteryPercentageMinThreshold1 reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE1_VOLTAGE1   = 0x11,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold2 or
   * BatteryPercentageMinThreshold2 reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE1_VOLTAGE2   = 0x12,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold3 or
   * BatteryPercentageMinThreshold3 reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE1_VOLTAGE3   = 0x13,

  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold or
   * BatteryPercentageMinThreshold reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE2_MIN_THRESHOLD = 0x20,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold1 or
   * BatteryPercentageMinThreshold1 reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE2_VOLTAGE1   = 0x21,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold2 or
   * BatteryPercentageMinThreshold2 reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE2_VOLTAGE2   = 0x22,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold3 or
   * BatteryPercentageMinThreshold3 reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE2_VOLTAGE3   = 0x23,

  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold or
   * BatteryPercentageMinThreshold reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE3_MIN_THRESHOLD = 0x30,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold1 or
   * BatteryPercentageMinThreshold1 reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE3_VOLTAGE1   = 0x31,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold2 or
   * BatteryPercentageMinThreshold2 reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE3_VOLTAGE2   = 0x32,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold3 or
   * BatteryPercentageMinThreshold3 reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE3_VOLTAGE3   = 0x33,
  /** @brief Mains power supply lost/unavailable (i.e., device is running on battery) */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_MAINS_POWER_SUPPLY_LOST_UNAVAILABLE   = 0x3a,
};


/*! @brief Power Configuration BatteryAlarmState
    @see ZCL spec 3.3.2.2.4.11, Table 3-26
*/
enum zb_zcl_power_config_battery_alarm_state_e
{
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold or
   * BatteryPercentageMinThreshold reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_MIN_THRESHOLD = 1 << 0,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold1 or
   * BatteryPercentageMinThreshold1 reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE1   = 1 << 1,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold2 or
   * BatteryPercentageMinThreshold2 reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE2   = 1 << 2,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold3 or
   * BatteryPercentageMinThreshold3 reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE3   = 1 << 3,

  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold or
   * BatteryPercentageMinThreshold reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE2_MIN_THRESHOLD = 1 << 10,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold1 or
   * BatteryPercentageMinThreshold1 reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE2_VOLTAGE1   = 1 << 11,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold2 or
   * BatteryPercentageMinThreshold2 reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE2_VOLTAGE2   = 1 << 12,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold3 or
   * BatteryPercentageMinThreshold3 reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE2_VOLTAGE3   = 1 << 13,

    /* use 1l to exclude overflow at 16-bit CPU: force 32-bit enum */
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold or
   * BatteryPercentageMinThreshold reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE3_MIN_THRESHOLD = 1l << 20,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold1 or
   * BatteryPercentageMinThreshold1 reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE3_VOLTAGE1   = 1l << 21,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold2 or
   * BatteryPercentageMinThreshold2 reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE3_VOLTAGE2   = 1l << 22,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold3 or
   * BatteryPercentageMinThreshold3 reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE3_VOLTAGE3   = 1l << 23,
  /** @brief Mains power supply lost/unavailable (i.e., device is running on battery) */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_MAINS_POWER_SUPPLY_LOST_UNAVAILABLE   = 1l << 30,
};

/** @cond internals_doc */
/*! @internal @name Power Configuration cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_ID,                    \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                           \
  (void*) data_ptr                                              \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_MAINS_FREQUENCY_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_MAINS_FREQUENCY_ID,                  \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                           \
  (void*) data_ptr                                              \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_MAINS_ALARM_MASK_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_MAINS_ALARM_MASK_ID,         \
  ZB_ZCL_ATTR_TYPE_8BITMAP,                             \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_MIN_THRESHOLD(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_MIN_THRESHOLD, \
  ZB_ZCL_ATTR_TYPE_U16,                                 \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_MAX_THRESHOLD(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_MAX_THRESHOLD, \
  ZB_ZCL_ATTR_TYPE_U16,                                 \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_MAINS_DWELL_TRIP_POINT(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_MAINS_DWELL_TRIP_POINT,      \
  ZB_ZCL_ATTR_TYPE_U16,                                 \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_ID(data_ptr, bat_num) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_VOLTAGE_ID,       \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                           \
  (void*) data_ptr                                              \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_SIZE_ID(data_ptr, bat_num) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_SIZE_ID,  \
  ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                           \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                   \
  (void*) data_ptr                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_QUANTITY_ID(data_ptr, bat_num) \
{                                                           \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_QUANTITY_ID,  \
  ZB_ZCL_ATTR_TYPE_U8,                                      \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                            \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                       \
  (void*) data_ptr                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_RATED_VOLTAGE_ID(data_ptr, bat_num) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_RATED_VOLTAGE_ID, \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                           \
  (void*) data_ptr                                              \
}

/* DA: HA12 change */
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_MASK_ID(data_ptr, bat_num) \
{                                                             \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_ALARM_MASK_ID,  \
  ZB_ZCL_ATTR_TYPE_8BITMAP,                                   \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                              \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                         \
  (void*) data_ptr                                            \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_MIN_THRESHOLD_ID(data_ptr, bat_num) \
{                                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_VOLTAGE_MIN_THRESHOLD_ID, \
  ZB_ZCL_ATTR_TYPE_U8,                                                  \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                        \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                   \
  (void*) data_ptr                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_REMAINING_ID(data_ptr, bat_num) \
{                                                                          \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_PERCENTAGE_REMAINING_ID,     \
  ZB_ZCL_ATTR_TYPE_U8,                                                     \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,             \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                      \
  (void*) data_ptr                                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD1_ID(data_ptr, bat_num) \
{                                                                     \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_VOLTAGE_THRESHOLD1_ID,  \
  ZB_ZCL_ATTR_TYPE_U8,                                                \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                      \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                 \
  (void*) data_ptr                                                    \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD2_ID(data_ptr, bat_num) \
{                                                                     \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_VOLTAGE_THRESHOLD2_ID,  \
  ZB_ZCL_ATTR_TYPE_U8,                                                \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                      \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                 \
  (void*) data_ptr                                                    \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD3_ID(data_ptr, bat_num) \
{                                                                     \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_VOLTAGE_THRESHOLD3_ID,  \
  ZB_ZCL_ATTR_TYPE_U8,                                                \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                      \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                 \
  (void*) data_ptr                                                    \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_MIN_THRESHOLD_ID(data_ptr, bat_num) \
{                                                                          \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_PERCENTAGE_MIN_THRESHOLD_ID, \
  ZB_ZCL_ATTR_TYPE_U8,                                                     \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                           \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                      \
  (void*) data_ptr                                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD1_ID(data_ptr, bat_num) \
{                                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_PERCENTAGE_THRESHOLD1_ID, \
  ZB_ZCL_ATTR_TYPE_U8,                                                  \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                        \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                   \
  (void*) data_ptr                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD2_ID(data_ptr, bat_num) \
{                                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_PERCENTAGE_THRESHOLD2_ID, \
  ZB_ZCL_ATTR_TYPE_U8,                                                  \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                        \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                   \
  (void*) data_ptr                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD3_ID(data_ptr, bat_num) \
{                                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_PERCENTAGE_THRESHOLD3_ID, \
  ZB_ZCL_ATTR_TYPE_U8,                                                  \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                        \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                   \
  (void*) data_ptr                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_STATE_ID(data_ptr, bat_num) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_ALARM_STATE_ID,   \
  ZB_ZCL_ATTR_TYPE_32BITMAP,                                    \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,  \
  (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                           \
  (void*) data_ptr                                              \
}

/*! @internal Number of attributes mandatory for reporting in Power
 *  Configuration cluster for 1 battery pack */
#undef ZB_ZCL_POWER_CONFIG_REPORT_ATTR_COUNT
#define ZB_ZCL_POWER_CONFIG_REPORT_ATTR_COUNT 2 /* Voltage + Alarm State */
/*! @internal Number of attributes mandatory for reporting in Power
 *  Configuration cluster for 2 Battery packs included */
#define ZB_ZCL_POWER_CONFIG_BAT_PACK_2_REPORT_ATTR_COUNT 4 /* Voltage + Voltage 2 + Alarm State +
                                                            * Alarm State 2 */
/*! @internal Number of attributes mandatory for reporting in Power
 *  Configuration cluster for 3 Battery packs included */
#define ZB_ZCL_POWER_CONFIG_BAT_PACK_3_REPORT_ATTR_COUNT 6 /* Voltage + Voltage 2 + Voltage 3 +
                                                            * Alarm State + Alarm State 2 + Alarm State 3*/

/*! @internal Alarm Code for Power Configuration cluster */
#define ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_MIN_THRESHOLD_ALARM_CODE 0x00
#define ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_MAX_THRESHOLD_ALARM_CODE 0x01

/*! @} */ /* Power Configuration cluster internals */
/*! @}
 *  @endcond */ /* internals_doc */

/** @brief Declares attribute list for Power Configuration cluster - server side.
    @param attr_list Attribute list name
    @param voltage Pointer to a variable to store BatteryVoltage attribute
    @param size Pointer to a variable to store BatterySize attribute
    @param quantity Pointer to a variable to store BatteryQuantity attribute
    @param rated_voltage Pointer to a variable to store BatteryRatedVoltage attribute
    @param alarm_mask Pointer to a variable to store BatteryAlarmMask attribute
    @param voltage_min_threshold Pointer to a variable to store BatteryVoltageMinThreshold attribute
*/
#define ZB_ZCL_DECLARE_POWER_CONFIG_ATTRIB_LIST(attr_list,                                   \
                                                voltage, size, quantity, rated_voltage,      \
                                                alarm_mask, voltage_min_threshold)           \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_POWER_CONFIG)          \
  ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_ID(voltage, ),             \
  ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_SIZE_ID(size, ),                   \
  ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_QUANTITY_ID(quantity, ),           \
  ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_RATED_VOLTAGE_ID(rated_voltage, ), \
  ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_MASK_ID(alarm_mask, ),       \
  ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_MIN_THRESHOLD_ID(voltage_min_threshold, ), \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */

#endif /* ZB_ZCL_POWER_CONFIG_H */
