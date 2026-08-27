/******************************************************************************

 @file  mac_gatt_sensor_profile.c

 @brief This file contains the MAC GATT Sensor profile implementation.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2022-2026, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

//*****************************************************************************
//! Includes
//*****************************************************************************
#include <string.h>
#include "ti/ble/stack_util/icall/app/icall.h"
#include "ti/ble/stack_util/icall/app/icall_ble_api.h"
#include "ti/ble/app_util/framework/bleapputil_api.h"
#include <mac_gatt_sensor_profile.h>

//*****************************************************************************
//! Globals
//*****************************************************************************

static MacGattSensorProfile_CBs_t *macGattSensorProfile_appCBs = NULL;

// Profile Attributes - variables
GATT_UUID(macGattSensorProfile_ServiceUUID, MACGATTSENSORPROFILE_SERV_UUID);
GATT_UUID(macGattSensorProfile_SensorDataUUID, MACGATTSENSORPROFILE_SENSOR_DATA_CHAR_UUID);
GATT_UUID(macGattSensorProfile_SensorReportIntervalUUID, MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR_UUID);
GATT_UUID(macGattSensorProfile_IdentifyRequestUUID, MACGATTSENSORPROFILE_IDENTIFY_REQUEST_CHAR_UUID);

static const gattAttrType_t macSensorService = { ATT_UUID_SIZE, macGattSensorProfile_ServiceUUID };

// Sensor Data Characteristic
static uint8_t macGattSensorProfile_sensorData[MACGATTSENSORPROFILE_SENSOR_DATA_CHAR_LEN] = {0x00, 0x00};
static uint8_t macGattSensorProfile_sensorDataProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
static gattCharCfg_t *macGattSensorProfile_sensorDataConfig;
static uint8_t macGattSensorProfile_sensorDataUserDesc[12] = "Sensor Data";

// Sensor Report Interval Characteristic
static uint8_t macGattSensorProfile_sensorReportInterval[MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR_LEN] = {0x00, 0x00, 0x00, 0x00};
static uint8_t macGattSensorProfile_sensorReportIntervalProps = GATT_PROP_READ | GATT_PROP_WRITE;
static uint8_t macGattSensorProfile_sensorReportIntervalUserDesc[23] = "Sensor Report Interval";

// Identify Request Characteristic
static uint8_t macGattSensorProfile_identifyRequest[MACGATTSENSORPROFILE_IDENTIFY_REQUEST_CHAR_LEN] = {0x00};
static uint8_t macGattSensorProfile_identifyRequestProps = GATT_PROP_WRITE;
static uint8_t macGattSensorProfile_identifyRequestUserDesc[27] = "Collector Identify Request";

// Profile Attributes - Table
static gattAttribute_t macGattSensorProfile_attrTbl[] =
{
  // MAC Sensor Profile Service Declaration
  GATT_BT_ATT(primaryServiceUUID, GATT_PERMIT_READ, (uint8_t *)&macSensorService),

  // Sensor Data Characteristic Declaration
  GATT_BT_ATT(characterUUID, GATT_PERMIT_READ, &macGattSensorProfile_sensorDataProps),
  // Sensor Data Characteristic Value
  GATT_ATT(macGattSensorProfile_SensorDataUUID, GATT_PERMIT_READ, macGattSensorProfile_sensorData),
  // Sensor Data Characteristic Configuration
  GATT_BT_ATT(clientCharCfgUUID, GATT_PERMIT_READ | GATT_PERMIT_WRITE, (uint8_t *)&macGattSensorProfile_sensorDataConfig),
  // Sensor Data Characteristic User Description
  GATT_BT_ATT(charUserDescUUID, GATT_PERMIT_READ, macGattSensorProfile_sensorDataUserDesc),

  // Sensor Report Interval Characteristic Declaration
  GATT_BT_ATT(characterUUID, GATT_PERMIT_READ, &macGattSensorProfile_sensorReportIntervalProps),
  // Sensor Report Interval Characteristic Value
  GATT_ATT(macGattSensorProfile_SensorReportIntervalUUID, GATT_PERMIT_READ, macGattSensorProfile_sensorReportInterval),
  // Sensor Report Interval Characteristic User Description
  GATT_BT_ATT(charUserDescUUID, GATT_PERMIT_READ, macGattSensorProfile_sensorReportIntervalUserDesc),

  // Identify Request Characteristic Declaration
  GATT_BT_ATT(characterUUID, GATT_PERMIT_READ, &macGattSensorProfile_identifyRequestProps),
  // Identify Request Characteristic Value
  GATT_ATT(macGattSensorProfile_IdentifyRequestUUID, GATT_PERMIT_WRITE, macGattSensorProfile_identifyRequest),
  // Identify Request Characteristic User Description
  GATT_BT_ATT(charUserDescUUID, GATT_PERMIT_READ, macGattSensorProfile_identifyRequestUserDesc),
};

//*****************************************************************************
//! Local Functions
//*****************************************************************************

static bStatus_t MacGattSensorProfile_readAttrCB(uint16_t connHandle,
                                                 gattAttribute_t *pAttr,
                                                 uint8_t *pValue, uint16_t *pLen,
                                                 uint16_t offset, uint16_t maxLen,
                                                 uint8_t method);

static bStatus_t MacGattSensorProfile_writeAttrCB(uint16_t connHandle,
                                                  gattAttribute_t *pAttr,
                                                  uint8_t *pValue, uint16_t len,
                                                  uint16_t offset, uint8_t method);

// Profile Callbacks
const gattServiceCBs_t macGattSensorProfile_CBs =
{
  MacGattSensorProfile_readAttrCB,  // Read callback function pointer
  MacGattSensorProfile_writeAttrCB, // Write callback function pointer
  NULL                              // Authorization callback function pointer
};

//*****************************************************************************
//! Functions
//*****************************************************************************

bStatus_t MacGattSensorProfile_addService(void)
{
  uint8_t status = SUCCESS;

  // Allocate Client Characteristic Configuration table for Sensor Data
  macGattSensorProfile_sensorDataConfig = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) * MAX_NUM_BLE_CONNS);
  if (macGattSensorProfile_sensorDataConfig == NULL)
  {
    return bleMemAllocError;
  }

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg(LINKDB_CONNHANDLE_INVALID, macGattSensorProfile_sensorDataConfig);

  // Register GATT attribute list and callbacks with GATT Server App
  status = GATTServApp_RegisterService(macGattSensorProfile_attrTbl,
                                       GATT_NUM_ATTRS(macGattSensorProfile_attrTbl),
                                       GATT_MAX_ENCRYPT_KEY_SIZE,
                                       &macGattSensorProfile_CBs);

  return status;
}

bStatus_t MacGattSensorProfile_registerAppCBs(MacGattSensorProfile_CBs_t *appCallbacks)
{
  if (appCallbacks)
  {
    macGattSensorProfile_appCBs = appCallbacks;
    return SUCCESS;
  }
  else
  {
    return bleAlreadyInRequestedMode;
  }
}

bStatus_t MacGattSensorProfile_setParameter(uint8_t param, uint8_t len, void *value)
{
  bStatus_t status = SUCCESS;

  if (value == NULL) // Check for null pointer
  {
    return INVALIDPARAMETER;
  }

  switch (param)
  {
    case MACGATTSENSORPROFILE_SENSOR_DATA_CHAR:
      if (len == MACGATTSENSORPROFILE_SENSOR_DATA_CHAR_LEN)
      {
        memcpy(macGattSensorProfile_sensorData, value, len);

        // Process notifications for Sensor Data characteristic
        GATTServApp_ProcessCharCfg(macGattSensorProfile_sensorDataConfig, macGattSensorProfile_sensorData, FALSE,
                                   macGattSensorProfile_attrTbl, GATT_NUM_ATTRS(macGattSensorProfile_attrTbl),
                                   INVALID_TASK_ID, MacGattSensorProfile_readAttrCB);
      }
      else
      {
        status = bleInvalidRange;
      }
      break;

    case MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR:
      if (len == MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR_LEN)
      {
        memcpy(macGattSensorProfile_sensorReportInterval, value, len);
      }
      else
      {
        status = bleInvalidRange;
      }
      break;

    case MACGATTSENSORPROFILE_IDENTIFY_REQUEST_CHAR: // New characteristic handling
      if (len == MACGATTSENSORPROFILE_IDENTIFY_REQUEST_CHAR_LEN)
      {
        memcpy(macGattSensorProfile_identifyRequest, value, len);
      }
      else
      {
        status = bleInvalidRange;
      }
      break;

    default:
      status = INVALIDPARAMETER;
      break;
  }

  return status;
}

bStatus_t MacGattSensorProfile_getParameter(uint8_t param, void *value)
{
  bStatus_t status = SUCCESS;

  if (value == NULL) // Check for null pointer
  {
    return INVALIDPARAMETER;
  }

  switch (param)
  {
    case MACGATTSENSORPROFILE_SENSOR_DATA_CHAR:
      memcpy(value, macGattSensorProfile_sensorData, MACGATTSENSORPROFILE_SENSOR_DATA_CHAR_LEN);
      break;

    case MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR:
      memcpy(value, macGattSensorProfile_sensorReportInterval, MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR_LEN);
      break;

    case MACGATTSENSORPROFILE_IDENTIFY_REQUEST_CHAR:
      memcpy(value, macGattSensorProfile_identifyRequest, MACGATTSENSORPROFILE_IDENTIFY_REQUEST_CHAR_LEN);
      break;

    default:
      status = INVALIDPARAMETER;
      break;
  }

  return status;
}

static bStatus_t MacGattSensorProfile_readAttrCB(uint16_t connHandle,
                                                 gattAttribute_t *pAttr,
                                                 uint8_t *pValue, uint16_t *pLen,
                                                 uint16_t offset, uint16_t maxLen,
                                                 uint8_t method)
{
  bStatus_t status = SUCCESS;

  if (offset > 0)
  {
    return ATT_ERR_ATTR_NOT_LONG;
  }

  if (pAttr->type.len == ATT_UUID_SIZE)
  {
    if (!memcmp(pAttr->type.uuid, macGattSensorProfile_SensorDataUUID, pAttr->type.len))
    {
      *pLen = MACGATTSENSORPROFILE_SENSOR_DATA_CHAR_LEN;
      memcpy(pValue, pAttr->pValue, MACGATTSENSORPROFILE_SENSOR_DATA_CHAR_LEN);
    }
    else if (!memcmp(pAttr->type.uuid, macGattSensorProfile_SensorReportIntervalUUID, pAttr->type.len))
    {
      *pLen = MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR_LEN;
      memcpy(pValue, pAttr->pValue, MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR_LEN);
    }
    else
    {
      *pLen = 0;
      status = ATT_ERR_ATTR_NOT_FOUND;
    }
  }
  else
  {
    *pLen = 0;
    status = ATT_ERR_INVALID_HANDLE;
  }

  return status;
}

static bStatus_t MacGattSensorProfile_writeAttrCB(uint16_t connHandle,
                                                  gattAttribute_t *pAttr,
                                                  uint8_t *pValue, uint16_t len,
                                                  uint16_t offset, uint8_t method)
{
  bStatus_t status = SUCCESS;

  if (pAttr->type.len == ATT_UUID_SIZE)
  {
    if (!memcmp(pAttr->type.uuid, macGattSensorProfile_SensorReportIntervalUUID, pAttr->type.len))
    {
      if (offset == 0 && len == MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR_LEN)
      {
        memcpy(pAttr->pValue, pValue, MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR_LEN);

        // Notify application of the write event
        if (macGattSensorProfile_appCBs && macGattSensorProfile_appCBs->pfnMacGattSensorProfileChange)
        {
          macGattSensorProfile_appCBs->pfnMacGattSensorProfileChange(MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR);
        }
      }
      else
      {
        status = ATT_ERR_INVALID_VALUE_SIZE;
      }
    }
    else if (!memcmp(pAttr->type.uuid, macGattSensorProfile_IdentifyRequestUUID, pAttr->type.len))
    {
      if (offset == 0 && len == MACGATTSENSORPROFILE_IDENTIFY_REQUEST_CHAR_LEN)
      {
        memcpy(pAttr->pValue, pValue, MACGATTSENSORPROFILE_IDENTIFY_REQUEST_CHAR_LEN);

        // Notify application of the write event
        if (macGattSensorProfile_appCBs && macGattSensorProfile_appCBs->pfnMacGattSensorProfileChange)
        {
          macGattSensorProfile_appCBs->pfnMacGattSensorProfileChange(MACGATTSENSORPROFILE_IDENTIFY_REQUEST_CHAR);
        }
      }
      else
      {
        status = ATT_ERR_INVALID_VALUE_SIZE;
      }
    }
    else
    {
      status = ATT_ERR_ATTR_NOT_FOUND;
    }
  }
  else if ((pAttr->type.len == ATT_BT_UUID_SIZE) &&
           (BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]) == GATT_CLIENT_CHAR_CFG_UUID))
  {
    status = GATTServApp_ProcessCCCWriteReq(connHandle, pAttr, pValue, len,
                                            offset, GATT_CLIENT_CFG_NOTIFY);
  }
  else
  {
    status = ATT_ERR_INVALID_HANDLE;
  }

  return status;
}
