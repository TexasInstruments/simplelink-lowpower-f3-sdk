/*
 * Copyright (c) 2018 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 *  ======== ble_common.js ========
 */
"use strict";


const advParamsRanges = {
  priAdvIntMinValue:            "20",                 // Min value of Primary Advertising Interval (ms)
  priAdvIntMaxValue:            "480000",             // Max value of Primary Advertising Interval (ms)
  priAdvIntMaxValueAdvData:     "40959.375",          // Max value of Advertising Interval (ms)
  advDataTXPowerMinValue:       "-127",               // Min value of TX Power (dBm)
  advDataTXPowerMaxValue:       "127",                // Max value of TX Power (dBm)
  advParamTXPowerMinValue:      "-127",               // Min value of Advertising TX Power (dBm)
  advParamTXPowerMaxValue:      "20",                 // Max value of Advertising TX Power (dBm)
  advSIDMinValue:               "0",                  // Min value of Advertising SID
  advSIDMaxValue:               "15"                  // Max value of Advertising SID
}

const connParamsRanges = {
  connectionIntMinValue:        "7.5",                // Min value of Connection Interval (ms)
  connectionIntMaxValue:        "4000",               // Max value of Connection Interval (ms)
  scanIntMinValue:              "2.5",                // Min value of Scan Interval (ms)
  scanIntMaxValue:              "40959.375",          // Max value of Scan Interval (ms)
  scanWinMinValue:              "2.5",                // Min value of Scan Window (ms)
  scanWinMaxValue:              "40959.375",          // Max value of Scan Window (ms)
  scanDurationMinValue:         "10",                 // Min value of Scan Duration (ms)
  scanDurationMaxValue:         "655350",             // Max value of Scan Duration (ms)
  scanPeriodMinValue:           "0",                  // Min value of Scan Period (sec)
  scanPeriodMaxValue:           "83884.8",            // Max value of Scan Period (sec)
  connLatencyMinValue:          "0",                  // Min value of Connection Latency
  connLatencyMaxValue:          "499",                // Max value of Connection Latency
  connTimeoutMinValue:          "100",                // Min value of Connection Timeout (ms)
  connTimeoutMaxValue:          "32000",              // Min value of Connection Timeout (ms)
  maxPDUSizeMinValue:           "27",                 // Min value of MAX_PDU_SIZE
  maxPDUSizeMaxValue:           "255"                 // Max value of MAX_PDU_SIZE
}

// Advertising Interval unit (ms)
const advIntUnit = 0.625;

// Connection Interval unit (ms)
const connectionIntUnit = 1.25;

// Maximum number of advertisement sets
const maxNumAdvSets = 20;

// Maximum length of legacy advertise data
const maxLegacyDataLen = 31;

// Maximum length of extended advertise data
const maxExtDataLen = 1650;
const maxExtConnDataLen = 254;

// Minimum length of extended headers data
const minExtHdrLen = 3; // Exntended Header Flags + AdvDataInfo
const advAHdrLen = 6;
const targetAHdrLen = 6;
const txPowerHdrLen = 1;

// Dictionary mapping a device name to default LaunchPad; used to discover the
// appropriate RF settings when a device is being used without a LaunchPad
const deviceToBoard = {
  CC2340R5: "LP_EM_CC2340R5",
  CC2340R53: "LP_EM_CC2340R53",
  CC2340R5_Q1: "LP_EM_CC2340R5_Q1",
  CC2340R22: "LP_EM_CC2340R22",
  CC2340R2: "LP_EM_CC2340R2",
  CC2745R10_Q1: "LP_EM_CC2745R10_Q1",
  CC2674R10RGZ: "LP_CC2674R10_RGZ",
  CC2674P10RGZ: "LP_CC2674P10_RGZ",
  CC2744R7RHAQ1: "LP_EM_CC2745R10_Q1",
  CC2755P105RHA: "LP_EM_CC2755P10"
};

const deviceToDefines = {
  "LP_CC2340R2": ["-DCC23X0"],
  "LP_CC2340R22": ["-DCC23X0"],
  "LP_CC2340R5": ["-DCC23X0"],
  "LP_CC2340R53": ["-DCC23X0"],
  "LP_CC2340R5_Q1": ["-DCC23X0"]
};

const txPowerValueToIndex = [
  { displayName: "-20", name: "HCI_EXT_TX_POWER_MINUS_20_DBM"},
  { displayName: "-18", name: "HCI_EXT_TX_POWER_MINUS_18_DBM"},
  { displayName: "-15", name: "HCI_EXT_TX_POWER_MINUS_15_DBM"},
  { displayName: "-12", name: "HCI_EXT_TX_POWER_MINUS_12_DBM"},
  { displayName: "-10", name: "HCI_EXT_TX_POWER_MINUS_10_DBM"},
  { displayName: "-9",  name: "HCI_EXT_TX_POWER_MINUS_9_DBM"},
  { displayName: "-6",  name: "HCI_EXT_TX_POWER_MINUS_6_DBM"},
  { displayName: "-5",  name: "HCI_EXT_TX_POWER_MINUS_5_DBM"},
  { displayName: "-3",  name: "HCI_EXT_TX_POWER_MINUS_3_DBM"},
  { displayName: "0",   name: "HCI_EXT_TX_POWER_0_DBM"},
  { displayName: "1",   name: "HCI_EXT_TX_POWER_1_DBM"},
  { displayName: "2",   name: "HCI_EXT_TX_POWER_2_DBM"},
  { displayName: "3",   name: "HCI_EXT_TX_POWER_3_DBM"},
  { displayName: "4",   name: "HCI_EXT_TX_POWER_4_DBM"},
  { displayName: "5",   name: "HCI_EXT_TX_POWER_5_DBM"},
  { displayName: "6",   name: "HCI_EXT_TX_POWER_P2_14_DBM_P4_6_DBM"},
  { displayName: "7",   name: "HCI_EXT_TX_POWER_P2_15_DBM_P4_7_DBM"},
  { displayName: "8",   name: "HCI_EXT_TX_POWER_P2_16_DBM_P4_8_DBM"},
  { displayName: "9",   name: "HCI_EXT_TX_POWER_P2_17_DBM_P4_9_DBM"},
  { displayName: "10",  name: "HCI_EXT_TX_POWER_P2_18_DBM_P4_10_DBM"},
  { displayName: "14",  name: "HCI_EXT_TX_POWER_P2_14_DBM_P4_6_DBM"},
  { displayName: "15",  name: "HCI_EXT_TX_POWER_P2_15_DBM_P4_7_DBM"},
  { displayName: "16",  name: "HCI_EXT_TX_POWER_P2_16_DBM_P4_8_DBM"},
  { displayName: "17",  name: "HCI_EXT_TX_POWER_P2_17_DBM_P4_9_DBM"},
  { displayName: "18",  name: "HCI_EXT_TX_POWER_P2_18_DBM_P4_10_DBM"},
  { displayName: "19",  name: "HCI_EXT_TX_POWER_P2_19_DBM"},
  { displayName: "20",  name: "HCI_EXT_TX_POWER_P2_20_DBM"}];

// Settings for ti/devices/CCFG module
const bleCentralCCFGSettings = {
  CC1312R1_LAUNCHXL_CCFG_SETTINGS: {},
  CC1352R1_LAUNCHXL_CCFG_SETTINGS: {},
  CC1352P1_LAUNCHXL_CCFG_SETTINGS: {},
  CC1352P_2_LAUNCHXL_CCFG_SETTINGS: {},
  CC1352P_4_LAUNCHXL_CCFG_SETTINGS: {},
  CC26X2R1_LAUNCHXL_CCFG_SETTINGS: {},
  LP_CC2652RSIP_CCFG_SETTINGS: {},
  LP_CC2652PSIP_CCFG_SETTINGS: {},
  LP_CC2652R7_CCFG_SETTINGS: {},
  LP_CC1352P7_1_CCFG_SETTINGS: {},
  LP_CC1352P7_4_CCFG_SETTINGS: {},
  LP_CC2651P3_CCFG_SETTINGS: {},
  LP_CC2651R3_CCFG_SETTINGS: {},
  LP_CC2651R3SIPA_CCFG_SETTINGS: {},
  LP_CC2652RB_CCFG_SETTINGS: {
    srcClkLF: "Derived from HF XOSC"
  },
  LP_EM_CC1354P10_1_CCFG_SETTINGS: {},
  LP_EM_CC1354P10_6_CCFG_SETTINGS: {},
  LP_CC2674R10_RGZ_CCFG_SETTINGS: {},
  LP_CC2674P10_RGZ_CCFG_SETTINGS: {}
};

const profiles_list = [
    { displayName: "Simple Gatt",    name: "65520"  },
    { displayName: "CGM",    name: "6175" },
    { displayName: "Glucose",    name: "6152" },
    { displayName: "HT", name: "6153" },
    { displayName: "BA",    name: "6159" },
]

// The list of CC23xx LPs and devices that can be migrated to
const CC23XXMigration = [
  {target: "LP_EM_CC2340R5"},
  {target: "LP_EM_CC2340R53"},
  {target: "LP_EM_CC2340R53_WCSP_SOCKET"},
  {target: "LP_EM_CC2340R53_WCSP"},
  {target: "LP_EM_CC2340R5_Q1"},
  {target: "LP_EM_CC2340R53_Q1"},
  {target: "LP_EM_CC2340R2"},
  {target: "LP_EM_CC2340R22"},
  {target: "CC2340R5RKP"},
  {target: "CC2340R53RKP"},
  {target: "CC2340R53RHBQ1"},
  {target: "CC2340R53YBG"},
  {target: "CC2340R5RGE"},
  {target: "CC2340R5RHB"},
  {target: "CC2340R2RGE"},
  {target: "CC2340R22RKP"}
]

// The list of CC27xx LPs and devices that can be migrated to
const CC27XXMigration = [
  {target: "LP_EM_CC2745R10_Q1"},
  {target: "LP_EM_CC2755P10"},
  {target: "LP_EM_CC2755R10_BG"},
  {target: "CC2745R10RHAQ1"},
  {target: "CC2745P10RHAQ1"},
  {target: "CC2755P105RHA"},
  {target: "CC2755R105RHA"},
  {target: "CC2745R7RHAQ1"},
  {target: "CC2755R105YCJ"}
]

const supportedMigrations = {
  // Boards
  LP_EM_CC2340R5:               CC23XXMigration,
  LP_EM_CC2340R53:              CC23XXMigration,
  LP_EM_CC2340R53_WCSP_SOCKET:  CC23XXMigration,
  LP_EM_CC2340R53_WCSP:         CC23XXMigration,
  LP_EM_CC2340R5_Q1:            CC23XXMigration,
  LP_EM_CC2340R53_Q1:           CC23XXMigration,
  LP_EM_CC2340R2:               CC23XXMigration,
  LP_EM_CC2340R22:              CC23XXMigration,
  LP_EM_CC2745R10_Q1:           CC27XXMigration,
  LP_EM_CC2755P10:              CC27XXMigration,
  LP_EM_CC2755R10_BG:           CC27XXMigration,
  //Devices
  CC2340R5RKP:    CC23XXMigration,
  CC2340R53RKP:   CC23XXMigration,
  CC2340R53YBG:   CC23XXMigration,
  CC2340R53RHBQ1: CC23XXMigration,
  CC2340R5RGE:    CC23XXMigration,
  CC2340R5RHB:    CC23XXMigration,
  CC2340R2RGE:    CC23XXMigration,
  CC2340R22RKP:   CC23XXMigration,
  CC2745P10RHAQ1: CC27XXMigration,
  CC2745R10RHAQ1: CC27XXMigration,
  CC2755P105RHA:  CC27XXMigration,
  CC2755R105RHA:  CC27XXMigration,
  CC2745R7RHAQ1:  CC27XXMigration,
  CC2744R7RHAQ1:  CC27XXMigration,
  CC2755R105YCJ:  CC27XXMigration
};

const boardName = getBoardOrLaunchPadName(true);
const centralRoleCcfgSettings = bleCentralCCFGSettings[boardName + "_CCFG_SETTINGS"];

/*
 * ======== convertTxPowerIndexToValue ========
 * Get's a Tx Power table index and returns the Tx Power in dBm.
 * It uses the txPowerValueToIndex list which maps the index to value.
 *
 * @param txPowerIndex - index to conver
 * @returns the Tx Power value in dBm
 */
function convertTxPowerIndexToValue(txPowerIndex)
{
  let txPowerValue = txPowerValueToIndex.find((option) => {
  if(option.name == txPowerIndex)
      return option;
  });

  if(txPowerValue)
  {
    txPowerValue = txPowerValue.displayName;
  }
  else
  {
    txPowerValue = "0";
  }

  return txPowerValue;
}

/*
 * ======== convertTxPowerIndexToValue ========
 * Get's a Tx Power value and returns it's index in the Tx Power table.
 * It uses the txPowerValueToIndex list which maps the index to value.
 *
 * @param txPowerValue - value to conver
 * @returns index of the value in the Tx Power table
 */
function convertTxPowerValueToIndex(txPowerValue)
{
  let txPowerIndex = txPowerValueToIndex.find((option) => {
  if(option.displayName == txPowerValue)
      return option;
  });

  if(txPowerIndex)
  {
    txPowerIndex = txPowerIndex.name;
  }
  else
  {
    txPowerIndex = "HCI_EXT_TX_POWER_0_DBM";
  }

  return txPowerIndex;
}

/*
 * ======== alphanumeric ========
 * Check if the string contains only letters, numbers and underscore
 *
 * @param inputtxt       - string number to be checked
 * @returns True if the string contains only letters, numbers and underscore otherwise return False.
 */
function alphanumeric(inputtxt)
{
  var letterNumber = /^[a-zA-Z0-9_]+$/;
  return (inputtxt.match(letterNumber))? true : false;
}

/*
 * ======== validateConnInterval ========
 * Validate this inst's configuration
 *
 * @param inst       - instance to be validated
 * @param validation - object to hold detected validation issues
 * @param minValue
 */
function validateConnInterval(inst,validation,minValue,minName,maxValue,maxName)
{
  if(minValue > maxValue)
  {
      validation.logError("Shall be greater than or equal to connectin interval min"
                          , inst, maxName);
      validation.logError("Shall be less than or equal to connection interval max"
                          , inst, minName);
  }
  if(minValue < connParamsRanges.connectionIntMinValue || minValue > connParamsRanges.connectionIntMaxValue)
  {
      validation.logError("The Range of connection interval is " + connParamsRanges.connectionIntMinValue +
                          " ms to "+ connParamsRanges.connectionIntMaxValue +" ms", inst, minName);
  }
  if(maxValue < connParamsRanges.connectionIntMinValue || maxValue > connParamsRanges.connectionIntMaxValue)
  {
      validation.logError("The Range of connection interval is " + connParamsRanges.connectionIntMinValue +
                          " ms to " + connParamsRanges.connectionIntMaxValue + " ms" ,inst, maxName);
  }
  if(maxValue % connectionIntUnit != 0)
  {
    validation.logError("The value Shall be a multiple of " + connectionIntUnit, inst, maxName);
  }
  if(minValue % connectionIntUnit != 0)
  {
    validation.logError("The value Shall be a multiple of " + connectionIntUnit, inst, minName);
  }
}

/*
 * ======== validateAdvInterval ========
 * Validate the Adv Interval configuration
 *
 * @param inst       - instance to be validated
 * @param validation - object to hold detected validation issues
 * @param minValue
 */
function validateAdvInterval(inst,validation,minValue,minName,maxValue,maxName)
{
  if(minValue > maxValue)
  {
      validation.logError("Shall be greater than or equal to connectin interval min"
                          , inst, maxName);
      validation.logError("Shall be less than or equal to connection interval max"
                          , inst, minName);
  }
  if(minValue < advParamsRanges.priAdvIntMinValue || minValue > advParamsRanges.priAdvIntMaxValue)
  {
      validation.logError("The Range of connection interval is " + advParamsRanges.priAdvIntMinValue +
                          " ms to "+ advParamsRanges.priAdvIntMaxValue +" ms", inst, minName);
  }
  if(maxValue < advParamsRanges.priAdvIntMinValue || maxValue > advParamsRanges.priAdvIntMaxValue)
  {
      validation.logError("The Range of connection interval is " + advParamsRanges.priAdvIntMinValue +
                          " ms to " + advParamsRanges.priAdvIntMaxValue + " ms" ,inst, maxName);
  }
  if(maxValue % advIntUnit != 0)
  {
      validation.logError("The value Shall be a multiple of " + advIntUnit, inst, maxName);
  }
  if(minValue % advIntUnit != 0)
  {
      validation.logError("The value Shall be a multiple of " + advIntUnit, inst, minName);
  }
}

/*
 * ======== validateUUIDLength ========
 * Validate the UUIDs configurable length
 *
 * @param inst       - instance to be validated
 * @param validation - object to hold detected validation issues
 * @param numOfUUIDs - the number of UUIDs that where added by the user
 * @params uuid      - the name of the uuid parameter to validate
 * @length           - the length of each uuid
 */
function validateUUIDLength(inst,validation,numOfUUIDs,uuid,length)
{
  for(let i = 0; i < numOfUUIDs; i++)
  {
    if(inst["UUID" + i + uuid].toString(16).length > length)
    {
        validation.logError("The number of bytes is " + _.divide(length,2) ,inst, "UUID" + i + uuid);
    }
  }
}

/*
 * ======== validateNumOfUUIDs ========
 * Validate the number of UUIDs configurable
 *
 * @param inst       - instance to be validated
 * @param validation - object to hold detected validation issues
 * @param numOfUUIDs - the number of UUIDs that where added by the user
 * @params uuid      - the name of the uuid parameter to validate
 * @length           - the length of each uuid
 */
function validateNumOfUUIDs(inst,validation,numOfUUIDs,uuid,length)
{
  //Check if the number of UUIDs is valid
  if(inst[numOfUUIDs] < 0 || inst[numOfUUIDs] > 10)
  {
      validation.logError("Please enter a number between 0 - 10", inst, numOfUUIDs);
  }
  // Validate each UUID length
  else
  {
      validateUUIDLength(inst,validation,inst[numOfUUIDs],uuid,length);
  }
}

/*
 * ======== validateAdditionalData ========
 * Validate that the additional data is hex
 *
 * @param inst       - instance to be validated
 * @param validation - object to hold detected validation issues
 * @param addDataValue - the value of the additional data
 * @params addDataName      - the name of the additional data parameter to validate
 */
function validateAdditionalData(inst, validation, addDataValue, addDataName)
{
  const hexRegex = new RegExp('^(0[xX])?[0-9A-Fa-f]+$');
  if(!hexRegex.test(addDataValue)){
    validation.logError("Please enter a valid HEX number", inst, addDataName);
  }
}

/*
 *  ======== addPeerAddress ========
 *  Gets an address in hex format and return it
 *  in the following format { 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa }
 *
 *  @param param  - An address in hex format
 *
 *  @returns the address in the following format: { 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa }
 */
function addPeerAddress(param)
{
  let address = "{ ";
  let prefix = "0x";

  address += prefix;
  for (let i = 0; i < param.length; i++)
  {
    address += param[i];
    if (i%2 != 0 && i < (param.length -1))
    {
      address += ", " + prefix;
    }
  }
  address += " }";

  return(address);
}

/*
 *  ======== reverseBytes ========
 *  Reverse the bytes of an hex string
 *
 *  @stringToReverse hex string to reverse its bytes
 */
function reverseBytes(stringToReverse)
{
  var str = stringToReverse.match(/.{1,2}/g);
  return str.reverse().toString(16).replace(/,/g,'');
}

/*
 *  ======== checkTXPower ========
 *  Help function for the advertisement params
 *  inst, TX Power
 *
 *  @param param
 *  @param value
 *  @returns the total length of an advertisement data
 */
function checkTXPower(param,value)
{
  return param == "GAP_ADV_TX_POWER_NO_PREFERENCE" ? param : value;
}

/*
 *  ======== getChanMap ========
 *  Returns the channel map according to the selected channels
 *
 *  @param array of selected channels
 *  @returns channel map
 */
function getChanMap(value)
{
  const allChan = ["GAP_ADV_CHAN_37", "GAP_ADV_CHAN_38", "GAP_ADV_CHAN_39"];
  if(_.isEqual(value, allChan))
  {
    return "GAP_ADV_CHAN_ALL";
  }
  else if(_.isEqual(value, ["GAP_ADV_CHAN_37", "GAP_ADV_CHAN_38"]))
  {
    return "GAP_ADV_CHAN_37_38";
  }
  else if(_.isEqual(value, ["GAP_ADV_CHAN_37", "GAP_ADV_CHAN_39"]))
  {
    return "GAP_ADV_CHAN_37_39";
  }
  else if(_.isEqual(value, ["GAP_ADV_CHAN_38", "GAP_ADV_CHAN_39"]))
  {
    return "GAP_ADV_CHAN_38_39";
  }
  else if(_.isEqual(value, ["GAP_ADV_CHAN_37"]))
  {
    return "GAP_ADV_CHAN_37";
  }
  else if(_.isEqual(value, ["GAP_ADV_CHAN_38"]))
  {
    return "GAP_ADV_CHAN_38";
  }
  else if(_.isEqual(value, ["GAP_ADV_CHAN_39"]))
  {
    return "GAP_ADV_CHAN_39";
  }
}

/*
 *  ======== addZeroFromLeft ========
 *  Adds zero to the left of an Hex format number
 *  that is uneven
 *
 *  @param hexNum
 *  @returns an Hex number
 */
function addZeroFromLeft(hexNum)
{
    if(hexNum.length % 2 != 0)
    {
        hexNum = '0' + hexNum;
    }
    return hexNum;
}

/*
 *  ======== getSelectedDataList ========
 *  Gets the full advertisement data list
 *
 *  @param inst
 *  @param config
 *  @returns the selected parameters list
 */
function getSelectedDataList(inst, config)
{
  let values = [];
  for(let i = 0; i < config.length; i++)
  {
    if(config[i].name.includes("GAP_ADTYPE") && inst[config[i].name])
    {
      values.push(config[i]);
      i++;
      while(i < config.length && !(config[i].name.includes("GAP_ADTYPE")))
      {
        values.push(config[i]);
        i++;
      }
      i--;
    }
  }
  return values;
}

/*
 *  ======== advDataTotalLength ========
 *  Calculates the total length of advertisement data
 *
 *  @param inst
 *  @param config
 *  @returns the total length of an advertisement data
 */
function advDataTotalLength(inst, config)
{
    let values = getSelectedDataList(inst, config);

    let totalLength = 0;

    for(let i =0; i < values.length; i++)
    {
      if(values[i].name.includes("GAP_ADTYPE"))
      {
        totalLength += 2;
      }
      if(values[i].name == "advertisingFlags" || values[i].name == "TXPower")
      {
        totalLength += 1;
      }
      else if(values[i].name.includes("numOfUUIDs") && values[i].name.includes("16"))
      {
        totalLength += 2*inst[values[i].name];
      }
      else if(values[i].name.includes("numOfUUIDs") && values[i].name.includes("32"))
      {
        totalLength += 4*inst[values[i].name];
      }
      else if(values[i].name.includes("numOfUUIDs") && values[i].name.includes("128"))
      {
        totalLength += 16*inst[values[i].name];
      }
      else if(values[i].name.includes("numOf") && values[i].name.includes("Addresses"))
      {
        totalLength += 6*inst[values[i].name];
      }
      else if(values[i].name.includes("LocalName"))
      {
        totalLength += inst[values[i].name].toString().length;
      }
      else if(values[i].name.includes("ConnInterval") || values[i].name.includes("advIntValue")
              || values[i].name.includes("appearanceValue"))
      {
        totalLength += 2;
      }
      else if(values[i].name.includes("companyIdentifier") || values[i].name.includes("additionalData"))
      {
        totalLength += _.divide(addZeroFromLeft(inst[values[i].name].toString(16)).length, 2);
      }
      else if(values[i].name.includes("16SDData") && inst.GAP_ADTYPE_SERVICE_DATA)
      {
        if(values[i].name[4] < inst.numOfUUIDs16SD)
        {
          totalLength += _.divide(addZeroFromLeft(inst[values[i].name].toString(16)).length,2);
        }
      }
      else if(values[i].name.includes("32SDData") && inst.GAP_ADTYPE_SERVICE_DATA_32BIT)
      {
        if(values[i].name[4] < inst.numOfUUIDs32SD)
        {
          totalLength += _.divide(addZeroFromLeft(inst[values[i].name].toString(16)).length,2);
        }
      }
      else if(values[i].name.includes("128SDData") && inst.GAP_ADTYPE_SERVICE_DATA_128BIT)
      {
        if(values[i].name[4] < inst.numOfUUIDs128SD)
        {
          totalLength += _.divide(addZeroFromLeft(inst[values[i].name].toString(16)).length,2);
        }
      }
    }
    return totalLength;
}

/*
 *  ======== advDataHexValues ========
 *  Gets Hex format number and return it
 *  in the following format 0xaa,\n 0xaa,\n ....
 *
 *  @param param  - Hex format number
 *
 *  @returns the Hex format number in the following format: 0xaa,\n 0xaa,\n ....
 */
function advDataHexValues(param)
{
  let address = "";
  let prefix = "0x";

  address += prefix;
  for (let i = 0; i < param.length; i++)
  {
    address += param[i];
    if (i%2 != 0)
    {
      address += ",\n"
      if(i < (param.length -1))
        {
          address += "  " + prefix;
        }
    }
  }

  return(address);
}
/*
 *  ======== listOfHexValues ========
 *  Gets Hex format number and return it
 *  in the following format 0xaa,0xaa,....
 *
 *  @param param  - Hex format number
 *
 *  @returns the Hex format number in the following format: 0xaa,\n 0xaa,\n ....
 */
function listOfHexValues(param)
{
  let hexList = "";
  let prefix = "0x";

  hexList += prefix;
  for (let i = 0; i < param.length; i++)
  {
    hexList += param[i];
    if (i%2 != 0)
    {
      if(i < (param.length -1))
        {
          hexList += ","
          hexList += prefix;
        }
    }
  }

  return(hexList);
}

/*!
 *  ======== device2DeviceFamily ========
 *  Map a pimux deviceID to a TI-driver device family string
 *
 *  @param deviceId  - a pinmux deviceId (system.deviceData)
 *
 *  @returns String - the corresponding "DeviceFamily_xxxx" string
 *                    used by driverlib header files.
 */
function device2DeviceFamily(deviceId)
{
    let driverString = null;

    /* Determine libraries required by device name. */
    if(deviceId.match(/CC2340R5/))
    {
        driverString = "DeviceFamily_CC23X0R5";
    }
    else if(deviceId.match(/CC2340R53/))
    {
        driverString = "DeviceFamily_CC23X0R53"; // Need to verify with CoreSDK team !!!!!!
    }
    else if(deviceId.match(/CC2340R2/))
    {
        driverString = "DeviceFamily_CC23X0R2";
    }
    else if(deviceId.match(/CC27../))
    {
        driverString = "DeviceFamily_CC27XX";
    }
    else if(deviceId.match(/CC23.0R22/))
    {
        driverString = "DeviceFamily_CC23X0R22";
    }
    else
    {
        driverString = "";
    }

    return(driverString);
}

/*!
 *  ======== getBoardOrLaunchPadName ========
 *  Get the name of the board (or device)
 *
 *  @param convertToBoard - Boolean. When true, return the associated LaunchPad
 *                          name if a device is being used without a LaunchPad
 *
 *  @returns String - Name of the board with prefix /ti/boards and
 *                    suffix .syscfg.json stripped off.  If no board
 *                    was specified, the device name is returned.
 */
function getBoardOrLaunchPadName(convertToBoard)
{
    let name = system.deviceData.deviceId;

    if(system.deviceData.board != null)
    {
        name = system.deviceData.board.source;

        /* Strip off everything up to and including the last '/' */
        name = name.replace(/.*\//, "");

        /* Strip off everything after and including the first '.' */
        name = name.replace(/\..*/, "");
    }

    // Check if this is a standalone device without a LaunchPad
    if(convertToBoard && !name.includes("LAUNCHXL") && !name.includes("LP_"))
    {
        // Find the LaunchPad name in deviceToBoard dictionary
        let key = null;
        for(key in deviceToBoard)
        {
            if(name.includes(key))
            {
                name = deviceToBoard[key];
                break;
            }
        }
    }

    return(name);
}

/*
 * ======== getRadioScript ========
 * Determines which rf_defaults script to use based on device or rfDesign
 *
 * @param rfDesign - the value of rfDesign parameter
 * @param deviceId - device being used
 *
 * @returns radioSettings - the rf_defaults script according to the selected
 *                          device/reDesign.
 *                          If device is not supported, returns null
 */
function getRadioScript(rfDesign, deviceId)
{
    let radioSettings = null;

    if(rfDesign !== null)
    {
        if(rfDesign === "LP_EM_CC2340R5_RGE_4X4_IS24")
        {
            radioSettings = system.getScript("/ti/ble/rf_config/"
                + "LP_EM_CC2340R2_rf_defaults.js");
        }
        else if(rfDesign === "LP_EM_CC2340R5")
        {
            radioSettings = system.getScript("/ti/ble/rf_config/"
                + "LP_EM_CC2340R5_rf_defaults.js");
        }
        else if(rfDesign === "LP_EM_CC2340R53")
        {
            radioSettings = system.getScript("/ti/ble/rf_config/"
                + "LP_EM_CC2340R5_rf_defaults.js");
        }
        else if(rfDesign === "LP_EM_CC2340R53_Q1")
        {
            radioSettings = system.getScript("/ti/ble/rf_config/"
                + "LP_EM_CC2340R5_Q1_rf_defaults.js");
        }
        else if(rfDesign === "LP_EM_CC2340R5_Q1")
        {
            radioSettings = system.getScript("/ti/ble/rf_config/"
                + "LP_EM_CC2340R5_Q1_rf_defaults.js");
        }
        else if(rfDesign === "LP_EM_CC2340R53_WCSP")
        {
            radioSettings = system.getScript("/ti/ble/rf_config/"
                + "LP_EM_CC2340R5_rf_defaults.js");
        }
        else if( rfDesign === "LP_EM_CC2745R10_Q1" )
        {
            radioSettings = system.getScript("/ti/ble/rf_config/"
            + "LP_EM_CC2745R10_Q1_rf_defaults.js");
        }
        else if( rfDesign === "LP_EM_CC2755R10_BG" )
        {
            radioSettings = system.getScript("/ti/ble/rf_config/"
            + "LP_EM_CC2755R10_BG_rf_defaults.js");
        }
        else if( rfDesign === "LP_EM_CC2755P10" )
        {
            radioSettings = system.getScript("/ti/ble/rf_config/"
            + "LP_EM_CC2755P10_rf_defaults.js");
        }
    }

    return(radioSettings);
}

/*
 *  ======== hideGroup ========
 *  Hide or UnHide an entire group
 *
 *  @param group   - The group config
 *  @param toHide  - True(Hide)/false(UnHide)
 *  @param ui      - The User Interface object
 */
function hideGroup(group, toHide, ui)
{
  let namesArray = _.map(group,function(n) {return n.name});
  _.each(namesArray, (cfg) => {cfg.includes("hide") ||
         cfg.includes("numOfDefAdvSets") || cfg == "DeviceInfo" ? true : ui[cfg].hidden = toHide;});
}

/*
 *  ======== getGroupByName ========
 *  Get a list of groups and a group name.
 *  Returns the group config array.
 *
 *
 *  @param groupList   - List of groups
 *  @param groupName   - The name of the group to return
 */
function getGroupByName(groupList, groupName)
{
  for(let i = 0; i < groupList.length; i++)
  {
    if(groupList[i].name == groupName)
    {
      return groupList[i].config;
    }
  }
}

function decimalToHexString(number)
{
  if (number < 0)
  {
    number = 0xFFFFFFFF + number + 1;
  }

  return '0x' + number.toString(16).toUpperCase();
}

/*
* ======== isMigrationValid ========
* Determines whether a migration from one board/device to another board/device
* is supported by the EasyLink module.
*
* @returns One of the following Objects:
*    - {} <--- Empty object if migration is valid
*    - {warn: "Warning markdown text"} <--- Object with warn property
*                                           if migration is valid but
*                                           might require user action
*    - {disable: "Disable markdown text"} <--- Object with disable property
*                                              if migration is not valid
*/
function isMigrationValid(currentTarget, migrationTarget)
{
  let migrationSupported = {disable: "Migration to this target is not supported via SysConfig. Consider starting from a more similar example to your desired migration target in <SDK_INSTALL_DIR>/examples/"};
  let currTarget = null;

  for( currTarget in supportedMigrations )
  {
    if( currTarget == currentTarget )
    {
      supportedMigrations[currTarget].forEach(target =>
      {
        if( target.target == migrationTarget )
        {
          migrationSupported = {};
        }
      });
    }
  }

  return migrationSupported;
}

/*
 * ======== migrate ========
 * Perform stack specific changes to the SysConfig env POST migration
 *
 * @param currTarget - Board/device being migrated FROM
 * @param migrationTarget - Board/device being migrated TO
 * @param env - SysConfig environment providing access to all configurables
 * @param projectName - Optional name of the project being migrated
 *
 * @returns boolean - true when migration is supported and succesful, false when
 *                    migration is not supported and/or unsuccesful
 */
function migrate(currTarget, migrationTarget, env, projectName = null)
{
  const migrationInfo = isMigrationValid(currTarget, migrationTarget);
  let migrationValid = true;

  // This is not a valid migration
  if(migrationInfo.disable)
  {
    migrationValid = false;
  }

  return migrationValid;
}

/*
* ======== getMigrationMarkdown ========
* Returns text in markdown format that customers can use to aid in migrating a
* project between device/boards. It is recommended to provide no more
* than 3 bullet points with up to 120 characters per line.
*
* @param currTarget - Board/device being migrated FROM
*
* @returns string - Markdown formatted string
*/
function getMigrationMarkdown(currTarget)
{
  // At this moment no message needed
  const migrationText = ``

  return(migrationText);
}

/* ======== defaultBondValue ========
* Returns maxBonds default value based on the
* device type
*
* @returns 5 - for CC23X0, else, 10
*/
function defaultBondValue()
{
    if( (device2DeviceFamily(system.deviceData.deviceId) == "DeviceFamily_CC23X0R5")  ||
        (device2DeviceFamily(system.deviceData.deviceId) == "DeviceFamily_CC23X0R53") ||
        (device2DeviceFamily(system.deviceData.deviceId) == "DeviceFamily_CC23X0R2")  ||
        (device2DeviceFamily(system.deviceData.deviceId) == "DeviceFamily_CC23X0R22") ||
        (device2DeviceFamily(system.deviceData.deviceId) == "DeviceFamily_CC27XX") )
    {
        return 5;
    }
    return 10;
}

/* ======== isChannelSoundingSupported ========
 * Determines whether channel sounding is
 * supported for the current device.
 *
 * @returns - Returns `true` if channel sounding
 *            is supported, otherwise `false`.
 */
function isChannelSoundingSupported()
{
    if( (device2DeviceFamily(system.deviceData.deviceId) == "DeviceFamily_CC23X0R2")  ||
        (device2DeviceFamily(system.deviceData.deviceId) == "DeviceFamily_CC23X0R22") )
    {
        return false;
    }
    return true;
}

/* ======== peripheralConnIntervalRange ========
* Returns the proper define for CONN_INTERVAL_RANGE
*
* @returns GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE or GAP_ADTYPE_PERIPHERAL_CONN_INTERVAL_RANGE
*/
function getPeripheralConnIntervalRange()
{
  return "GAP_ADTYPE_PERIPHERAL_CONN_INTERVAL_RANGE";
}

exports = {
    convertTxPowerIndexToValue: convertTxPowerIndexToValue,
    convertTxPowerValueToIndex: convertTxPowerValueToIndex,
    alphanumeric: alphanumeric,
    advParamsRanges: advParamsRanges,
    connParamsRanges: connParamsRanges,
    maxNumAdvSets: maxNumAdvSets,
    maxLegacyDataLen: maxLegacyDataLen,
    maxExtDataLen: maxExtDataLen,
    maxExtConnDataLen: maxExtConnDataLen,
    minExtHdrLen: minExtHdrLen,
    advAHdrLen: advAHdrLen,
    targetAHdrLen: targetAHdrLen,
    txPowerHdrLen: txPowerHdrLen,
    deviceToDefines: deviceToDefines,
    txPowerValueToIndex: txPowerValueToIndex,
    addPeerAddress: addPeerAddress,
    reverseBytes: reverseBytes,
    checkTXPower: checkTXPower,
    centralRoleCcfgSettings: centralRoleCcfgSettings,
    getChanMap: getChanMap,
    getSelectedDataList: getSelectedDataList,
    validateConnInterval: validateConnInterval,
    validateAdvInterval: validateAdvInterval,
    validateUUIDLength: validateUUIDLength,
    validateNumOfUUIDs: validateNumOfUUIDs,
    validateAdditionalData: validateAdditionalData,
    advDataTotalLength: advDataTotalLength,
    addZeroFromLeft: addZeroFromLeft,
    advDataHexValues: advDataHexValues,
    listOfHexValues: listOfHexValues,
    getBoardOrLaunchPadName: getBoardOrLaunchPadName,
    device2DeviceFamily: device2DeviceFamily,
    getRadioScript: getRadioScript,
    hideGroup: hideGroup,
    getGroupByName: getGroupByName,
    decimalToHexString: decimalToHexString,
    isMigrationValid: isMigrationValid,
    migrate: migrate,
    getMigrationMarkdown: getMigrationMarkdown,
    defaultBondValue: defaultBondValue,
    getPeripheralConnIntervalRange: getPeripheralConnIntervalRange,
    isChannelSoundingSupported: isChannelSoundingSupported,
    profiles_list: profiles_list
};
