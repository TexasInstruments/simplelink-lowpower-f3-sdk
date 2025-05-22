/******************************************************************************

 @file  car_access_server.h

 @brief This file contains the Car Access service definitions and prototypes.
        Note: This is a TI custom service for demonstration purposes

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025, Texas Instruments Incorporated
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

#ifndef CAR_ACCESS_SERVER_H
#define CAR_ACCESS_SERVER_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */
// Service UUID
#define CAS_SERVICE_UUID 0xA0A0

// Characteristic defines
#define CAS_INDICATION_CHAR_ID   0
#define CAS_INDICATION_CHAR_LEN  1
#define CAS_INDICATION_CHAR_UUID 0xA0A1

// Characteristic defines
#define CAS_DATA_IN_CHAR_ID   1
#define CAS_DATA_IN_CHAR_UUID 0xA0A2
// Maximum allowed length for incoming data
#define CAS_DATA_IN_CHAR_MAX_LEN 512

// Characteristic defines
#define CAS_PAIRING_CHAR_ID   2
#define CAS_PAIRING_CHAR_LEN  1
#define CAS_PAIRING_CHAR_UUID 0xA0A3


/*********************************************************************
 * TYPEDEFS
 */
// Data structure used to store incoming data
typedef struct
{
  uint16_t connHandle;
  uint16_t len;
  char pValue[];
} CAS_dataIn_t;

// Data structure used to store ccc update
typedef struct
{
  uint16_t connHandle;
  uint16_t value;
} CAS_cccUpdate_t;

/*********************************************************************
 * Profile Callbacks
 */
// Callback to indicate client characteristic configuration has been updated
typedef void (*CAS_onCccUpdate_t)( char *pValue );

// Callback when data is received
typedef void (*CAS_incomingData_t)( char *pValue );

typedef struct
{
  CAS_onCccUpdate_t         pfnOnCccUpdateCB;     // Called when client characteristic configuration has been updated
  CAS_incomingData_t        pfnIncomingDataCB;    // Called when receiving data
} CAS_cb_t;

/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * @fn      CAS_addService
 *
 * @brief   This function initializes the Car Access service
 *          by registering GATT attributes with the GATT server.
 *
 * @return  SUCCESS or stack call status
 */
uint8_t CAS_addService( void );

/*********************************************************************
 * @fn      CAS_registerProfileCBs
 *
 * @brief   Registers the profile callback function. Only call
 *          this function once.
 *
 * @param   profileCallback - pointer to profile callback.
 *
 * @return  SUCCESS or INVALIDPARAMETER
 */
uint8_t CAS_registerProfileCBs( CAS_cb_t *profileCallback );

/*********************************************************************
 * @fn      CAS_setParameter
 *
 * @brief   Set a Car Access Service parameter.
 *
 * @param   param - Characteristic UUID
 * @param   pValue - pointer to data to write.  This is dependent on
 *          the parameter ID and will be cast to the appropriate
 *          data type (example: data type of uint16_t will be cast to
 *          uint16_t pointer).
 * @param   len - length of data to write
 *
 * @return  SUCCESS or stack call status
 */
uint8_t CAS_setParameter( uint8_t param, void *pValue, uint16_t len );

/*********************************************************************
 * @fn      CAS_sendIndication
 *
 * @brief   Transmits data over BLE indication to the provided
 *          connection handle.
 *
 * @param   connHandle - connection handle to send the indication to
 *
 * @return  SUCCESS, or stack call status
 */
uint8_t CAS_sendIndication( uint16_t connHandle );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* CAR_ACCESS_SERVER_H */