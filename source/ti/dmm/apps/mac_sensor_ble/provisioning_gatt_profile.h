/******************************************************************************

 @file  provisioning_gatt_profile.h

 @brief This file contains the PROV GATT profile definitions and prototypes.

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

 #ifndef PROVGATTPROFILE_H
 #define PROVGATTPROFILE_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 /*********************************************************************
  * CONSTANTS
  */
 
 // PROV Profile Service UUID
 #define PROVGATTPROFILE_SERV_UUID               0x1190
 
 // PROV Profile Characteristic UUIDs
 #define PROVGATTPROFILE_NTWK_PAN_ID_CHAR_UUID   0x1191
 #define PROVGATTPROFILE_SENSOR_FREQ_CHAR_UUID   0x1192
 #define PROVGATTPROFILE_SENSOR_CHANNEL_CHAR_UUID 0x1193
 #define PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR_UUID 0x1194
 #define PROVGATTPROFILE_NTWK_KEY_CHAR_UUID      0x1195
 #define PROVGATTPROFILE_PROV_SENSOR_CHAR_UUID   0x1196
 #define PROVGATTPROFILE_PROV_STATE_CHAR_UUID    0x1197
 
 // PROV Profile Parameters
 #define PROVGATTPROFILE_NTWK_PAN_ID_CHAR        0  // RW uint8[2] - Network PAN ID characteristic
 #define PROVGATTPROFILE_SENSOR_FREQ_CHAR        1  // RW uint8 - Sensor Frequency characteristic
 #define PROVGATTPROFILE_SENSOR_CHANNEL_CHAR     2  // RW uint8[17] - Sensor Channel characteristic
 #define PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR      3  // RW uint8[8] - IEEE FFD Address characteristic
 #define PROVGATTPROFILE_NTWK_KEY_CHAR           4  // RW uint8[16] - Network Key characteristic
 #define PROVGATTPROFILE_PROV_SENSOR_CHAR        5  // RW uint8 - Provision Sensor characteristic
 #define PROVGATTPROFILE_PROV_STATE_CHAR         6  // RN uint8 - Provisioning State characteristic
 
 // Maximum user description length in bytes for characteristics
 #define PROVGATTPROFILE_MAX_DESCRIPTION_LEN     (50)
 
 // Lengths of characteristics
 #define PROVGATTPROFILE_NTWK_PAN_ID_CHAR_LEN    (2)
 #define PROVGATTPROFILE_SENSOR_FREQ_CHAR_LEN    (1)
 #define PROVGATTPROFILE_SENSOR_CHANNEL_CHAR_LEN (17)
 #define PROVGATTPROFILE_IEEE_FFD_ADDR_CHAR_LEN  (8)
 #define PROVGATTPROFILE_NTWK_KEY_CHAR_LEN       (16)
 #define PROVGATTPROFILE_PROV_SENSOR_CHAR_LEN    (1)
 #define PROVGATTPROFILE_PROV_STATE_CHAR_LEN     (1)
 
 /*********************************************************************
  * TYPEDEFS
  */
 
 // Callback when a characteristic value has changed
 typedef void (*provGattProfileChange_t)(uint8_t paramID);
 
 typedef struct
 {
   provGattProfileChange_t pfnProvGattProfileChange; // Called when characteristic value changes
 } ProvGattProfile_CBs_t;
 
 /*********************************************************************
  * API FUNCTIONS
  */
 
 /**
  * @brief Initializes the PROV GATT Profile service by registering GATT attributes with the GATT server.
  *
  * @return SUCCESS or stack call status
  */
 bStatus_t ProvGattProfile_addService(void);
 
 /**
  * @brief Registers the application callback function. Only call this function once.
  *
  * @param appCallbacks Pointer to application callback.
  * @return SUCCESS or INVALIDPARAMETER
  */
 bStatus_t ProvGattProfile_registerAppCBs(ProvGattProfile_CBs_t *appCallbacks);
 
 /**
  * @brief Set a PROV GATT Profile parameter.
  *
  * @param param Profile parameter ID
  * @param len Length of data to write
  * @param value Pointer to data to write
  * @return SUCCESS or INVALIDPARAMETER
  */
 bStatus_t ProvGattProfile_setParameter(uint8_t param, uint8_t len, void *value);
 
 /**
  * @brief Get a PROV GATT Profile parameter.
  *
  * @param param Profile parameter ID
  * @param value Pointer to data to read
  * @return SUCCESS or INVALIDPARAMETER
  */
 bStatus_t ProvGattProfile_getParameter(uint8_t param, void *value);
 
 /**
  * @brief Notify application of characteristic value changes.
  *
  * @param paramID ID of the changed parameter.
  */
 void ProvGattProfile_callback(uint8_t paramID);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* PROVGATTPROFILE_H */
 