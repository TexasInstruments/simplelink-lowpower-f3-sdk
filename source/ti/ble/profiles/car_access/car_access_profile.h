/******************************************************************************

 @file  car_access_profile.h

 @brief This file contains the Car Access Profile definitions and APIs.
        Note: This is a TI custom profile for demonstration purposes

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

#ifndef CAR_ACCESS_PROFILE_H
#define CAR_ACCESS_PROFILE_H

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

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * Profile Callbacks
 */
// Callback to indicate client characteristic configuration has been updated
typedef void (*CAP_onCccUpdate_t)( uint16_t connHandle, uint16_t pValue);

// Callback when receiving data
typedef void (*CAP_incomingData_t)( uint16_t connHandle, uint8_t *pValue, uint16_t len );

// Callback to indicate indication confirmation has been received
typedef void (*CAP_indCnfReceived_t)( uint16_t connHandle, uint8_t status );

typedef struct
{
  CAP_onCccUpdate_t   pfnOnCccUpdateCb;  // Called when client characteristic configuration has been updated
  CAP_incomingData_t  pfnIncomingDataCB; // Called when data is receiving
  CAP_indCnfReceived_t pfnIndCnfReceived; // Called when indication confirmation is received
} CAP_cb_t;

/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * @fn      CAP_start
 *
 * @brief   This function adds the Car Access service,
 *          registers the application callback function and initializes
 *          buffer for incoming data.
 *
 * @return  SUCCESS or stack call status
 */
uint8_t CAP_start( CAP_cb_t *appCallbacks );

/*********************************************************************
 * @fn      dataStreamProfile_sendData
 *
 * @brief   Send indication to a specific connHandle
 *
 * @param   connHandle - the connection handle to send indication
 *
 * @return  SUCCESS or stack call status
 */
uint8_t CAP_sendInd( uint16_t connHandle );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* CAR_ACCESS_PROFILE_H */
