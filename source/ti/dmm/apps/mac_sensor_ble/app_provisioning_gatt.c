/******************************************************************************

@file  app_154_gatt.c

@brief This file contains the PROV GATT application functionality

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
#include "ti/ble/app_util/framework/bleapputil_api.h"
#include <provisioning_gatt_profile.h>
#include "ti/ble/app_util/menu/menu_module.h"
#include <app_main.h>
#include <jdllc.h>
#include <zb_types.h>
#include <mac_gatt_sensor_profile.h>
#include <sensor.h>

// Bytes required to initiate the sensor start process
#define SENSOR_ASSOC 0xAA
// Bytes required to initiate the sensor disassociation process
#define SENSOR_DISASSOC 0xDD

//*****************************************************************************
//! Globals
//*****************************************************************************

static void ProvGatt_changeCB(uint8_t paramId);
void PROVGatt_syncProvisioningAttributes(char *unused);

// PROV GATT Profile Callbacks
static ProvGattProfile_CBs_t provGatt_profileCBs =
{
  ProvGatt_changeCB // PROV GATT Characteristic value change callback
};

//*****************************************************************************
//! Functions
//*****************************************************************************
/*!
 Build a uint16_t out of 2 uint8_t variables

 Public function defined in mac_util.h
 */
extern zb_uint16_t Util_buildUint16(zb_uint8_t loByte, zb_uint8_t hiByte);
extern zb_uint32_t Util_buildUint32(zb_uint8_t byte0, zb_uint8_t byte1, zb_uint8_t byte2,
  zb_uint8_t byte3);

/*********************************************************************
 * @fn      ProvGatt_changeCB
 *
 * @brief   Callback from PROV Profile indicating a characteristic
 *          value change.
 *
 * @param   paramId - parameter Id of the valuse that was changed.
 *
 * @return  None.
 */
static void ProvGatt_changeCB(uint8_t paramId)
{
  uint8_t newValue[17] = {0};

  switch (paramId)
  {
    case PROVGATTPROFILE_NTWK_PAN_ID_CHAR:
      ProvGattProfile_getParameter(PROVGATTPROFILE_NTWK_PAN_ID_CHAR, newValue);
      Jdllc_setJoiningPanId(Util_buildUint16(newValue[1], newValue[0]));
      break;

    case PROVGATTPROFILE_SENSOR_CHANNEL_CHAR:
      ProvGattProfile_getParameter(PROVGATTPROFILE_SENSOR_CHANNEL_CHAR, newValue);

      Jdllc_setChanMask(Util_buildUint32(newValue[0], newValue[1], newValue[2], newValue[3]));
      break;

    case PROVGATTPROFILE_PROV_SENSOR_CHAR:
      ProvGattProfile_getParameter(PROVGATTPROFILE_PROV_SENSOR_CHAR, newValue);

      if (newValue[0] == SENSOR_ASSOC)
      {
        triggerSensorStartEvt();
      }
      else if (newValue[0] == SENSOR_DISASSOC)
      {
        Jdllc_sendDisassociationRequest();
      }
      break;

    case PROVGATTPROFILE_PROV_STATE_CHAR:
      ProvGattProfile_setParameter(PROVGATTPROFILE_PROV_STATE_CHAR, sizeof(uint8_t), newValue);
      break;

      // Add option for Poll rate?
    default:
      break;
  }
}

/*********************************************************************
 * @fn      ProvGatt_start
 *
 * @brief   This function is called after stack initialization,
 *          the purpose of this function is to initialize and
 *          register the PROV GATT profile.
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t ProvGatt_start(void)
{
  bStatus_t status = SUCCESS;

  // Add PROV GATT service
  status = ProvGattProfile_addService();
  if (status != SUCCESS)
  {
    return status;
  }

  PROVGatt_syncProvisioningAttributes(NULL);

  // Register callback with PROV GATT profile
  status = ProvGattProfile_registerAppCBs(&provGatt_profileCBs);

  return status;
}

void PROVGatt_syncProvisioningAttributes_util(void)
{
  BLEAppUtil_invokeFunctionNoData(( InvokeFromBLEAppUtilContext_t ) PROVGatt_syncProvisioningAttributes );
}

/*********************************************************************
 * @fn      PROVGatt_syncProvisioningAttributes
 *
 * @brief   Synchronize provisioning attributes using PROV GATT APIs.
 *
 * @return  None.
 */
void PROVGatt_syncProvisioningAttributes(char *unused)
{
    uint8_t buffer[PROVGATTPROFILE_NTWK_KEY_CHAR_LEN]; // Maximum size needed for any attribute

    // Synchronize PAN ID
    {
        uint16_t panId;
        uint8_t tmpByteSwap;

        Jdllc_getJoiningPanId(&panId);
        Util_bufferUint16(buffer, panId);

        // Switch byte order
        tmpByteSwap = buffer[0];
        buffer[0] = buffer[1];
        buffer[1] = tmpByteSwap;

        ProvGattProfile_setParameter(PROVGATTPROFILE_NTWK_PAN_ID_CHAR, PROVGATTPROFILE_NTWK_PAN_ID_CHAR_LEN, buffer);
    }

    // Synchronize Frequency
    {
        buffer[0] = Jdllc_getFreq();
        ProvGattProfile_setParameter(PROVGATTPROFILE_SENSOR_FREQ_CHAR, sizeof(uint8_t), buffer);
    }

    // Synchronize Sensor Channel Mask
    {
        Jdllc_getChanMask(buffer);
        ProvGattProfile_setParameter(PROVGATTPROFILE_SENSOR_CHANNEL_CHAR, PROVGATTPROFILE_SENSOR_CHANNEL_CHAR_LEN, buffer);
    }

    // Synchronize IEEE FFD Address
    {
        Jdllc_getFfdAddr((zb_ieee_addr_t *)buffer);
        ProvGattProfile_setParameter(PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR, PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR_LEN, buffer);
    }

#ifdef FEATURE_MAC_SECURITY
    // Synchronize Network Key
    {
        Jdllc_getDefaultKey(buffer);
        ProvGattProfile_setParameter(PROVGATTPROFILE_NTWK_KEY_CHAR, PROVGATTPROFILE_NTWK_KEY_CHAR_LEN, buffer);
    }
#endif

    // Synchronize Provisioning State
    {
        buffer[0] = Jdllc_getProvState();
        ProvGattProfile_setParameter(PROVGATTPROFILE_PROV_STATE_CHAR, sizeof(uint8_t), buffer);
    }
}
