/******************************************************************************

@file  app_mac_sensor_gatt.c

@brief This file contains the MAC GATT Sensor application functionality.

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
#include <mac_gatt_sensor_profile.h>
#include "ti/ble/app_util/menu/menu_module.h"
#include <app_main.h>
#include <jdllc.h>
#include <zb_types.h>

extern uint32_t sensorGetReportingInterval(void);
extern uint16_t getTempSensorData(void);
extern  void Sensor_sendIdentifyLedRequest();
void MACGatt_syncSensorAttributes(char* unused);

//*****************************************************************************
//! Globals
//*****************************************************************************

static void MacSensorGatt_changeCB(uint8_t paramId);

// MAC GATT Sensor Profile Callbacks
static MacGattSensorProfile_CBs_t macGattSensorProfileCBs =
{
  MacSensorGatt_changeCB // MAC GATT Sensor Characteristic value change callback
};

//*****************************************************************************
//! Functions
//*****************************************************************************

/*********************************************************************
 * @fn      MacSensorGatt_changeCB
 *
 * @brief   Callback from MAC Sensor Profile indicating a characteristic
 *          value change.
 *
 * @param   paramId - parameter ID of the value that was changed.
 *
 * @return  None.
 */
static void MacSensorGatt_changeCB(uint8_t paramId)
{
  uint8_t newValue[4] = {0};

  switch (paramId)
  {
    case MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR:
      MacGattSensorProfile_getParameter(MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR, newValue);
      break;

    case MACGATTSENSORPROFILE_IDENTIFY_REQUEST_CHAR:
      MacGattSensorProfile_getParameter(MACGATTSENSORPROFILE_IDENTIFY_REQUEST_CHAR, newValue);
      if (newValue[0] == 1)
      {
        Sensor_sendIdentifyLedRequest();
      }
      break;

    default:
      break;
  }
}

/*********************************************************************
 * @fn      MacSensorGatt_start
 *
 * @brief   This function is called after stack initialization,
 *          the purpose of this function is to initialize and
 *          register the MAC GATT Sensor profile.
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t MacSensorGatt_start(void)
{
  bStatus_t status = SUCCESS;

  // Add MAC GATT Sensor service
  status = MacGattSensorProfile_addService();
  if (status != SUCCESS)
  {
    return status;
  }

  // Setup the MAC GATT Sensor Characteristic Values
  MACGatt_syncSensorAttributes(NULL);
  // Register callback with MAC GATT Sensor profile
  status = MacGattSensorProfile_registerAppCBs(&macGattSensorProfileCBs);

  return status;
}


/*********************************************************************
 * @fn      MACGatt_syncSensorAttributes
 *
 * @brief   Synchronize sensor attributes using MAC GATT Sensor APIs.
 *
 * @return  None.
 */
void MACGatt_syncSensorAttributes(char *unused)
{
    uint8_t sensorData[MACGATTSENSORPROFILE_SENSOR_DATA_CHAR_LEN];
    uint8_t sensorReportInterval[MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR_LEN];

    // Get sensor data
    uint16_t tempSensorData = getTempSensorData();
    sensorData[0] = (uint8_t)(tempSensorData & 0xFF);
    sensorData[1] = (uint8_t)((tempSensorData >> 8) & 0xFF);
    MacGattSensorProfile_setParameter(MACGATTSENSORPROFILE_SENSOR_DATA_CHAR, MACGATTSENSORPROFILE_SENSOR_DATA_CHAR_LEN, sensorData);

    // Get sensor report interval
    uint32_t reportingInterval = sensorGetReportingInterval();
    sensorReportInterval[0] = (uint8_t)(reportingInterval & 0xFF);
    sensorReportInterval[1] = (uint8_t)((reportingInterval >> 8) & 0xFF);
    sensorReportInterval[2] = (uint8_t)((reportingInterval >> 16) & 0xFF);
    sensorReportInterval[3] = (uint8_t)((reportingInterval >> 24) & 0xFF);
    MacGattSensorProfile_setParameter(MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR, MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR_LEN, sensorReportInterval);
}

void MACGatt_syncSensorAttributes_util(void)
{
  BLEAppUtil_invokeFunctionNoData(( InvokeFromBLEAppUtilContext_t ) MACGatt_syncSensorAttributes );
}