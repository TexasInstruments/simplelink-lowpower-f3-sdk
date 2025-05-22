/******************************************************************************

 @file  car_access_profile.c

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

/*********************************************************************
 * INCLUDES
 */
#include <string.h>
#include "ti/ble/stack_util/icall/app/icall.h"
/* This Header file contains all BLE API and icall structure definition */
#include "ti/ble/stack_util/icall/app/icall_ble_api.h"

#include "ti/ble/services/car_access/car_access_server.h"
#include "ti/ble/profiles/car_access/car_access_profile.h"
#include "ti/ble/app_util/framework/bleapputil_api.h"

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static CAP_cb_t *cap_appCB = NULL;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void CAP_handoverCCCUpdateCB( char *pValue );
static void CAP_incomingDataCB( char *pValue );
static void CAP_gattEventHandler(uint32 event, BLEAppUtil_msgHdr_t *pMsgData);

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Car access profile callback functions
static CAS_cb_t cap_serverCB =
{
  .pfnOnCccUpdateCB   = CAP_handoverCCCUpdateCB,
  .pfnIncomingDataCB  = CAP_incomingDataCB
};

BLEAppUtil_EventHandler_t capGATTHandler =
{
    .handlerType    = BLEAPPUTIL_GATT_TYPE,
    .pEventHandler  = CAP_gattEventHandler,
    .eventMask      = BLEAPPUTIL_ATT_HANDLE_VALUE_CFM
};

/*********************************************************************
 * PUBLIC FUNCTIONS
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
uint8_t CAP_start( CAP_cb_t *appCallbacks )
{
  uint8_t status = SUCCESS;

  // Add Car access service
  status = CAS_addService();

  if ( status == SUCCESS )
  {
      // Register to service callback function
      status = CAS_registerProfileCBs( &cap_serverCB );

      if ( status == SUCCESS )
      {
          // Registers the application callback function
          if ( appCallbacks )
          {
              cap_appCB = appCallbacks;
          }

          // Register the GATT event handler
          status = BLEAppUtil_registerEventHandler(&capGATTHandler);
      }
  }
  // Return status value
  return ( status );
}

/*********************************************************************
 * @fn      CAP_sendInd
 *
 * @brief   Send an indication to a specific connHandle
 *
 * @param   connHandle - the connection handle to send indication
 *
 * @return  SUCCESS or stack call status
 */
uint8_t CAP_sendInd( uint16_t connHandle )
{
  return CAS_sendIndication(connHandle);
}

/*********************************************************************
 * @fn      CAP_handoverCCCUpdateCB
 *
 * @brief   Callback from BAS Server indicating CCC has been update
 *          for Level characteristic
 *
 * @param   pValue - pointer to data
 *
 * @return  None
 */
static void CAP_handoverCCCUpdateCB( char *pValue )
{
    CAS_cccUpdate_t *cccUpdate = ( CAS_cccUpdate_t * )pValue;

    // Verify input parameters
    if ( pValue != NULL )
    {
        // If the callback function is not NULL, notify the application
        if ( ( cap_appCB != NULL ) && ( cap_appCB->pfnOnCccUpdateCb != NULL ) )
        {
            cap_appCB->pfnOnCccUpdateCb( cccUpdate->connHandle, cccUpdate->value );
        }
    }
}

/*********************************************************************
 * @fn      CAP_incomingDataCB
 *
 * @brief   Callback from Car Access service indicating incoming data
 *
 * @param   pValue - pointer to the incoming data
 *
 * @return  none
 */
static void CAP_incomingDataCB( char *pValue )
{
  CAS_dataIn_t *dataIn = ( CAS_dataIn_t * )pValue;

  if ( pValue != NULL )
  {
    if ( ( cap_appCB!= NULL ) && ( cap_appCB->pfnIncomingDataCB!= NULL ) )
    {
      cap_appCB->pfnIncomingDataCB( dataIn->connHandle, (uint8_t*)dataIn->pValue, dataIn->len );
    }
  }
}

/*********************************************************************
 * @fn      CAP_gattEventHandler
 *
 * @brief   The purpose of this function is to handle GATT related
 *          events that rise from the GATT and were registered in
 *          @ref BLEAppUtil_registerEventHandler
 *
 * @param   event - message event.
 * @param   pMsgData - pointer to message data.
 *
 * @return  none
 */
static void CAP_gattEventHandler(uint32 event, BLEAppUtil_msgHdr_t *pMsgData)
{
    gattMsgEvent_t *gattMsg = (gattMsgEvent_t *)pMsgData;
    switch(event)
    {
      // An indication confirmation packet was received
      case BLEAPPUTIL_ATT_HANDLE_VALUE_CFM:
      {
        if ( ( cap_appCB != NULL ) && ( cap_appCB->pfnIndCnfReceived != NULL ) )
        {
          cap_appCB->pfnIndCnfReceived( gattMsg->connHandle, gattMsg->hdr.status );
        }
        break;
      }

      default:
      {
          break;
      }
    }
}

/*********************************************************************
*********************************************************************/
