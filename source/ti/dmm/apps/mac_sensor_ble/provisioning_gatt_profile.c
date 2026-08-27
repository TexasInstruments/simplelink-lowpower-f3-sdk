/******************************************************************************

 @file  provisioning_gatt_profile.c

 @brief This file contains the PROV GATT profile implementation.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2010-2026, Texas Instruments Incorporated
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
#include <provisioning_gatt_profile.h>

//*****************************************************************************
//! Globals
//*****************************************************************************

static ProvGattProfile_CBs_t *provGattProfile_appCBs = NULL;

// Profile Attributes - variables
GATT_UUID(provGattProfile_ServiceUUID, PROVGATTPROFILE_SERV_UUID);
GATT_UUID(provGattProfile_NtwkPanIdUUID, PROVGATTPROFILE_NTWK_PAN_ID_CHAR_UUID);
GATT_UUID(provGattProfile_SensorFreqUUID, PROVGATTPROFILE_SENSOR_FREQ_CHAR_UUID);
GATT_UUID(provGattProfile_SensorChannelUUID, PROVGATTPROFILE_SENSOR_CHANNEL_CHAR_UUID);
GATT_UUID(provGattProfile_IeeeFfdAddrUUID, PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR_UUID);
GATT_UUID(provGattProfile_NtwkKeyUUID, PROVGATTPROFILE_NTWK_KEY_CHAR_UUID);
GATT_UUID(provGattProfile_ProvSensorUUID, PROVGATTPROFILE_PROV_SENSOR_CHAR_UUID);
GATT_UUID(provGattProfile_ProvStateUUID, PROVGATTPROFILE_PROV_STATE_CHAR_UUID);

static const gattAttrType_t provService = { ATT_UUID_SIZE, provGattProfile_ServiceUUID };

// Network PAN ID Characteristic
static uint8_t provGattProfile_panId[PROVGATTPROFILE_NTWK_PAN_ID_CHAR_LEN] = {0x00, 0x00};
static uint8_t provGattProfile_panIdProps = GATT_PROP_READ | GATT_PROP_WRITE;
static uint8_t provGattProfile_panIdUserDesc[16] = "Network PAN ID";

// Sensor Frequency Characteristic
static uint8_t provGattProfile_sensorFreq = 0x00;
static uint8_t provGattProfile_sensorFreqProps = GATT_PROP_READ;
static uint8_t provGattProfile_sensorFreqUserDesc[16] = "Sensor Frequency";

// Sensor Channel Characteristic
static uint8_t provGattProfile_channel[PROVGATTPROFILE_SENSOR_CHANNEL_CHAR_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t provGattProfile_channelProps = GATT_PROP_READ | GATT_PROP_WRITE;
static uint8_t provGattProfile_channelUserDesc[14] = "Sensor Channel";

// IEEE FFD Address Characteristic
static uint8_t provGattProfile_ieeeAddr[PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t provGattProfile_ieeeAddrProps = GATT_PROP_READ;
static uint8_t provGattProfile_ieeeAddrUserDesc[18] = "IEEE FFD Address";

// Network Key Characteristic
static uint8_t provGattProfile_ntwkKey[PROVGATTPROFILE_NTWK_KEY_CHAR_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t provGattProfile_ntwkKeyProps = GATT_PROP_READ | GATT_PROP_WRITE;
static uint8_t provGattProfile_ntwkKeyUserDesc[12] = "Network Key";

// Provisioning Sensor Characteristic
static uint8_t provGattProfile_provSensor = 0x00;
static uint8_t provGattProfile_provSensorProps = GATT_PROP_READ | GATT_PROP_WRITE;
static uint8_t provGattProfile_provSensorUserDesc[19] = "Provision Sensor";

// Provisioning State Characteristic
static uint8_t provGattProfile_provState = 0x00;
static uint8_t provGattProfile_provStateProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
static gattCharCfg_t *provGattProfile_provStateConfig;
static uint8_t provGattProfile_provStateUserDesc[19] = "Provisioning State";

// Profile Attributes - Table
static gattAttribute_t provGattProfile_attrTbl[] =
{
  // PROV Profile Service Declaration
  GATT_BT_ATT(primaryServiceUUID, GATT_PERMIT_READ, (uint8_t *)&provService),

  // Network PAN ID Characteristic Declaration
  GATT_BT_ATT(characterUUID, GATT_PERMIT_READ, &provGattProfile_panIdProps),
  // Network PAN ID Characteristic Value
  GATT_ATT(provGattProfile_NtwkPanIdUUID, GATT_PERMIT_READ | GATT_PERMIT_WRITE, provGattProfile_panId),
  // Network PAN ID Characteristic User Description
  GATT_BT_ATT(charUserDescUUID, GATT_PERMIT_READ, provGattProfile_panIdUserDesc),

  // Sensor Frequency Characteristic Declaration
  GATT_BT_ATT(characterUUID, GATT_PERMIT_READ, &provGattProfile_sensorFreqProps),
  // Sensor Frequency Characteristic Value
  GATT_ATT(provGattProfile_SensorFreqUUID, GATT_PERMIT_READ, &provGattProfile_sensorFreq),
  // Sensor Frequency Characteristic User Description
  GATT_BT_ATT(charUserDescUUID, GATT_PERMIT_READ, provGattProfile_sensorFreqUserDesc),

  // Sensor Channel Characteristic Declaration
  GATT_BT_ATT(characterUUID, GATT_PERMIT_READ, &provGattProfile_channelProps),
  // Sensor Channel Characteristic Value
  GATT_ATT(provGattProfile_SensorChannelUUID, GATT_PERMIT_READ | GATT_PERMIT_WRITE, provGattProfile_channel),
  // Sensor Channel Characteristic User Description
  GATT_BT_ATT(charUserDescUUID, GATT_PERMIT_READ, provGattProfile_channelUserDesc),

  // IEEE FFD Address Characteristic Declaration
  GATT_BT_ATT(characterUUID, GATT_PERMIT_READ, &provGattProfile_ieeeAddrProps),
  // IEEE FFD Address Characteristic Value
  GATT_ATT(provGattProfile_IeeeFfdAddrUUID, GATT_PERMIT_READ, provGattProfile_ieeeAddr),
  // IEEE FFD Address Characteristic User Description
  GATT_BT_ATT(charUserDescUUID, GATT_PERMIT_READ, provGattProfile_ieeeAddrUserDesc),

  // Network Key Characteristic Declaration
  GATT_BT_ATT(characterUUID, GATT_PERMIT_READ, &provGattProfile_ntwkKeyProps),
  // Network Key Characteristic Value
  GATT_ATT(provGattProfile_NtwkKeyUUID, GATT_PERMIT_READ | GATT_PERMIT_WRITE, provGattProfile_ntwkKey),
  // Network Key Characteristic User Description
  GATT_BT_ATT(charUserDescUUID, GATT_PERMIT_READ, provGattProfile_ntwkKeyUserDesc),

  // Provisioning Sensor Characteristic Declaration
  GATT_BT_ATT(characterUUID, GATT_PERMIT_READ, &provGattProfile_provSensorProps),
  // Provisioning Sensor Characteristic Value
  GATT_ATT(provGattProfile_ProvSensorUUID, GATT_PERMIT_READ | GATT_PERMIT_WRITE, &provGattProfile_provSensor),
  // Provisioning Sensor Characteristic User Description
  GATT_BT_ATT(charUserDescUUID, GATT_PERMIT_READ, provGattProfile_provSensorUserDesc),

  // Provisioning State Characteristic Declaration
  GATT_BT_ATT(characterUUID, GATT_PERMIT_READ, &provGattProfile_provStateProps),
  // Provisioning State Characteristic Value
  GATT_ATT(provGattProfile_ProvStateUUID, GATT_PERMIT_READ, &provGattProfile_provState),
  // Provisioning State Characteristic Configuration
  GATT_BT_ATT(clientCharCfgUUID, GATT_PERMIT_READ | GATT_PERMIT_WRITE, (uint8_t *)&provGattProfile_provStateConfig),
  // Provisioning State Characteristic User Description
  GATT_BT_ATT(charUserDescUUID, GATT_PERMIT_READ, provGattProfile_provStateUserDesc),
};



//*****************************************************************************
//! Local Functions
//*****************************************************************************

static bStatus_t ProvGattProfile_readAttrCB(uint16_t connHandle,
                                           gattAttribute_t *pAttr,
                                           uint8_t *pValue, uint16_t *pLen,
                                           uint16_t offset, uint16_t maxLen,
                                           uint8_t method);

static bStatus_t ProvGattProfile_writeAttrCB(uint16_t connHandle,
                                            gattAttribute_t *pAttr,
                                            uint8_t *pValue, uint16_t len,
                                            uint16_t offset, uint8_t method);
void ProvGattProfile_callback( uint8 paramID  );
void ProvGattProfile_invokeFromFWContext( char *pData );
                                            
// Profile Callbacks
const gattServiceCBs_t provGattProfile_CBs =
{
  ProvGattProfile_readAttrCB,  // Read callback function pointer
  ProvGattProfile_writeAttrCB, // Write callback function pointer
  NULL                        // Authorization callback function pointer
};
//*****************************************************************************
//! Functions
//*****************************************************************************

bStatus_t ProvGattProfile_addService(void)
{
  uint8_t status = SUCCESS;

  provGattProfile_provStateConfig = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) * MAX_NUM_BLE_CONNS);
  if (provGattProfile_provStateConfig == NULL)
  {
    return bleMemAllocError;
  }

  GATTServApp_InitCharCfg(LINKDB_CONNHANDLE_INVALID, provGattProfile_provStateConfig);

  status = GATTServApp_RegisterService(provGattProfile_attrTbl,
                                       GATT_NUM_ATTRS(provGattProfile_attrTbl),
                                       GATT_MAX_ENCRYPT_KEY_SIZE,
                                       &provGattProfile_CBs);

  return status;
}

bStatus_t ProvGattProfile_registerAppCBs(ProvGattProfile_CBs_t *appCallbacks)
{
  if (appCallbacks)
  {
    provGattProfile_appCBs = appCallbacks;
    return SUCCESS;
  }
  else
  {
    return bleAlreadyInRequestedMode;
  }
}

bStatus_t ProvGattProfile_setParameter(uint8_t param, uint8_t len, void *value)
{
  bStatus_t status = SUCCESS;

  if (value == NULL) // Check for null pointer
  {
    return INVALIDPARAMETER;
  }

  switch (param)
  {
    case PROVGATTPROFILE_NTWK_PAN_ID_CHAR:
      if (len == PROVGATTPROFILE_NTWK_PAN_ID_CHAR_LEN)
      {
        memcpy(provGattProfile_panId, value, len);
      }
      else
      {
        status = bleInvalidRange;
      }
      break;

    case PROVGATTPROFILE_SENSOR_FREQ_CHAR:
      if (len == sizeof(uint8_t))
      {
        provGattProfile_sensorFreq = *((uint8_t *)value);
      }
      else
      {
        status = bleInvalidRange;
      }
      break;

    case PROVGATTPROFILE_SENSOR_CHANNEL_CHAR:
      if (len == PROVGATTPROFILE_SENSOR_CHANNEL_CHAR_LEN)
      {
        memcpy(provGattProfile_channel, value, len);
      }
      else
      {
        status = bleInvalidRange;
      }
      break;

    case PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR:
      if (len == PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR_LEN)
      {
        memcpy(provGattProfile_ieeeAddr, value, len);
      }
      else
      {
        status = bleInvalidRange;
      }
      break;

    case PROVGATTPROFILE_NTWK_KEY_CHAR:
      if (len == PROVGATTPROFILE_NTWK_KEY_CHAR_LEN)
      {
        memcpy(provGattProfile_ntwkKey, value, len);
      }
      else
      {
        status = bleInvalidRange;
      }
      break;

    case PROVGATTPROFILE_PROV_SENSOR_CHAR:
      if (len == sizeof(uint8_t))
      {
        provGattProfile_provSensor = *((uint8_t *)value);
      }
      else
      {
        status = bleInvalidRange;
      }
      break;

    case PROVGATTPROFILE_PROV_STATE_CHAR:
      if (len == sizeof(uint8_t))
      {
        provGattProfile_provState = *((uint8_t *)value);
        GATTServApp_ProcessCharCfg(provGattProfile_provStateConfig, &provGattProfile_provState, FALSE,
                                   provGattProfile_attrTbl, GATT_NUM_ATTRS(provGattProfile_attrTbl),
                                   INVALID_TASK_ID, ProvGattProfile_readAttrCB);
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

bStatus_t ProvGattProfile_getParameter(uint8_t param, void *value)
{
  bStatus_t status = SUCCESS;

  if (value == NULL) // Check for null pointer
  {
    return INVALIDPARAMETER;
  }

  switch (param)
  {
    case PROVGATTPROFILE_NTWK_PAN_ID_CHAR:
      memcpy(value, provGattProfile_panId, PROVGATTPROFILE_NTWK_PAN_ID_CHAR_LEN);
      break;

    case PROVGATTPROFILE_SENSOR_FREQ_CHAR:
      *((uint8_t *)value) = provGattProfile_sensorFreq;
      break;

    case PROVGATTPROFILE_SENSOR_CHANNEL_CHAR:
      memcpy(value, provGattProfile_channel, PROVGATTPROFILE_SENSOR_CHANNEL_CHAR_LEN);
      break;

    case PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR:
      memcpy(value, provGattProfile_ieeeAddr, PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR_LEN);
      break;

    case PROVGATTPROFILE_NTWK_KEY_CHAR:
      memcpy(value, provGattProfile_ntwkKey, PROVGATTPROFILE_NTWK_KEY_CHAR_LEN);
      break;

    case PROVGATTPROFILE_PROV_SENSOR_CHAR:
      *((uint8_t *)value) = provGattProfile_provSensor;
      break;

    case PROVGATTPROFILE_PROV_STATE_CHAR:
      *((uint8_t *)value) = provGattProfile_provState;
      break;

    default:
      status = INVALIDPARAMETER;
      break;
  }

  return status;
}

/*********************************************************************
 * @fn      ProvGattProfile_readAttrCB
 *
 * @brief   Read an attribute.
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be read
 * @param   pLen - length of data to be read
 * @param   offset - offset of the first octet to be read
 * @param   maxLen - maximum length of data to be read
 * @param   method - type of read message
 *
 * @return  SUCCESS, blePending or Failure
 */
static bStatus_t ProvGattProfile_readAttrCB(uint16_t connHandle,
                                           gattAttribute_t *pAttr,
                                           uint8_t *pValue, uint16_t *pLen,
                                           uint16_t offset, uint16_t maxLen,
                                           uint8_t method)
{
  bStatus_t status = SUCCESS;

  // Ensure it's not a blob operation (no attributes in the profile are long)
  if (offset > 0)
  {
    return ATT_ERR_ATTR_NOT_LONG;
  }

  if (pAttr->type.len == ATT_UUID_SIZE)
  {
    if (!memcmp(pAttr->type.uuid, provGattProfile_NtwkPanIdUUID, pAttr->type.len))
    {
      *pLen = PROVGATTPROFILE_NTWK_PAN_ID_CHAR_LEN;
      memcpy(pValue, pAttr->pValue, PROVGATTPROFILE_NTWK_PAN_ID_CHAR_LEN);
    }
    else if (!memcmp(pAttr->type.uuid, provGattProfile_SensorFreqUUID, pAttr->type.len))
    {
      *pLen = sizeof(uint8_t);
      *pValue = *pAttr->pValue;
    }
    else if (!memcmp(pAttr->type.uuid, provGattProfile_SensorChannelUUID, pAttr->type.len))
    {
      *pLen = PROVGATTPROFILE_SENSOR_CHANNEL_CHAR_LEN;
      memcpy(pValue, pAttr->pValue, PROVGATTPROFILE_SENSOR_CHANNEL_CHAR_LEN);
    }
    else if (!memcmp(pAttr->type.uuid, provGattProfile_IeeeFfdAddrUUID, pAttr->type.len))
    {
      *pLen = PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR_LEN;
      memcpy(pValue, pAttr->pValue, PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR_LEN);
    }
    else if (!memcmp(pAttr->type.uuid, provGattProfile_NtwkKeyUUID, pAttr->type.len))
    {
      *pLen = PROVGATTPROFILE_NTWK_KEY_CHAR_LEN;
      memcpy(pValue, pAttr->pValue, PROVGATTPROFILE_NTWK_KEY_CHAR_LEN);
    }
    else if (!memcmp(pAttr->type.uuid, provGattProfile_ProvSensorUUID, pAttr->type.len))
    {
      *pLen = sizeof(uint8_t);
      *pValue = *pAttr->pValue;
    }
    else if (!memcmp(pAttr->type.uuid, provGattProfile_ProvStateUUID, pAttr->type.len))
    {
      *pLen = sizeof(uint8_t);
      *pValue = *pAttr->pValue;
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

/*********************************************************************
 * @fn      ProvGattProfile_writeAttrCB
 *
 * @brief   Validate attribute data prior to a write operation.
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   method - type of write message
 *
 * @return  SUCCESS, blePending or Failure
 */
static bStatus_t ProvGattProfile_writeAttrCB(uint16_t connHandle,
                                            gattAttribute_t *pAttr,
                                            uint8_t *pValue, uint16_t len,
                                            uint16_t offset, uint8_t method)
{
  bStatus_t status = SUCCESS;
  uint8_t notifyApp = 0xFF;

  if (pAttr->type.len == ATT_UUID_SIZE)
  {
    if (!memcmp(pAttr->type.uuid, provGattProfile_NtwkPanIdUUID, pAttr->type.len))
    {
      if (offset == 0 && len == PROVGATTPROFILE_NTWK_PAN_ID_CHAR_LEN)
      {
        memcpy(pAttr->pValue, pValue, PROVGATTPROFILE_NTWK_PAN_ID_CHAR_LEN);
        notifyApp = PROVGATTPROFILE_NTWK_PAN_ID_CHAR;
      }
      else
      {
        status = ATT_ERR_INVALID_VALUE_SIZE;
      }
    }
    else if (!memcmp(pAttr->type.uuid, provGattProfile_SensorFreqUUID, pAttr->type.len))
    {
      if (offset == 0 && len == sizeof(uint8_t))
      {
        *pAttr->pValue = pValue[0];
        notifyApp = PROVGATTPROFILE_SENSOR_FREQ_CHAR;
      }
      else
      {
        status = ATT_ERR_INVALID_VALUE_SIZE;
      }
    }
    else if (!memcmp(pAttr->type.uuid, provGattProfile_SensorChannelUUID, pAttr->type.len))
    {
      if (offset == 0 && len == PROVGATTPROFILE_SENSOR_CHANNEL_CHAR_LEN)
      {
        memcpy(pAttr->pValue, pValue, PROVGATTPROFILE_SENSOR_CHANNEL_CHAR_LEN);
        notifyApp = PROVGATTPROFILE_SENSOR_CHANNEL_CHAR;
      }
      else
      {
        status = ATT_ERR_INVALID_VALUE_SIZE;
      }
    }
    else if (!memcmp(pAttr->type.uuid, provGattProfile_IeeeFfdAddrUUID, pAttr->type.len))
    {
      if (offset == 0 && len == PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR_LEN)
      {
        memcpy(pAttr->pValue, pValue, PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR_LEN);
        notifyApp = PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR;
      }
      else
      {
        status = ATT_ERR_INVALID_VALUE_SIZE;
      }
    }
    else if (!memcmp(pAttr->type.uuid, provGattProfile_NtwkKeyUUID, pAttr->type.len))
    {
      if (offset == 0 && len == PROVGATTPROFILE_NTWK_KEY_CHAR_LEN)
      {
        memcpy(pAttr->pValue, pValue, PROVGATTPROFILE_NTWK_KEY_CHAR_LEN);
        notifyApp = PROVGATTPROFILE_NTWK_KEY_CHAR;
      }
      else
      {
        status = ATT_ERR_INVALID_VALUE_SIZE;
      }
    }
    else if (!memcmp(pAttr->type.uuid, provGattProfile_ProvSensorUUID, pAttr->type.len))
    {
      if (offset == 0 && len == sizeof(uint8_t))
      {
        *pAttr->pValue = pValue[0];
        notifyApp = PROVGATTPROFILE_PROV_SENSOR_CHAR;
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

  if (notifyApp != 0xFF && provGattProfile_appCBs && provGattProfile_appCBs->pfnProvGattProfileChange)
  {
    ProvGattProfile_callback(notifyApp);
  }

  return status;
}

/*********************************************************************
 * @fn      ProvGattProfile_callback
 *d
 * @brief   Notify application of characteristic value changes.
 *
 * @param   paramID - ID of the changed parameter.
 *
 * @return  None.
 */
void ProvGattProfile_callback(uint8_t paramID)
{
  char *pData = ICall_malloc(sizeof(char));

  if (pData == NULL)
  {
    return;
  }

  pData[0] = paramID;

  BLEAppUtil_invokeFunction(ProvGattProfile_invokeFromFWContext, pData);
}

/*********************************************************************
 * @fn      ProvGattProfile_invokeFromFWContext
 *
 * @brief   Invoke application callback from firmware context.
 *
 * @param   pData - Pointer to data containing parameter ID.
 *
 * @return  None.
 */
void ProvGattProfile_invokeFromFWContext(char *pData)
{
  if (provGattProfile_appCBs && provGattProfile_appCBs->pfnProvGattProfileChange)
  {
    provGattProfile_appCBs->pfnProvGattProfileChange(pData[0]);
  }

  ICall_free(pData);
}
