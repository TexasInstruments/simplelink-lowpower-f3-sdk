/******************************************************************************

 @file  car_access_server.c

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

/*********************************************************************
 * INCLUDES
 */
#include <string.h>
#include <stdint.h>
#include "ti/ble/stack_util/icall/app/icall.h"
/* This Header file contains all BLE API and icall structure definition */
#include "ti/ble/stack_util/icall/app/icall_ble_api.h"

#include "ti/ble/services/car_access/car_access_server.h"
#include "ti/ble/app_util/framework/bleapputil_api.h"

/*********************************************************************
 * CONSTANTS
 */
// The size of the notification header is opcode + handle
#define CAS_IND_HDR_SIZE   (ATT_OPCODE_SIZE + 2)

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// Car Access Service UUID: 0xA0A0
GATT_UUID( casServiceUUID, CAS_SERVICE_UUID );

// Indication Characteristic UUID: 0xA0A1
GATT_UUID( casIndicationCharUUID, CAS_INDICATION_CHAR_UUID );

// Data In Characteristic UUID: 0xA0A2
GATT_UUID( casDataInCharUUID, CAS_DATA_IN_CHAR_UUID );

// Pairing Characteristic UUID: 0xA0A3
GATT_UUID( casPairingCharUUID, CAS_PAIRING_CHAR_UUID );

static CAS_cb_t *cas_profileCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

// Car Access Service declaration
static const gattAttrType_t casService = { ATT_UUID_SIZE, casServiceUUID };

// Characteristic "Indication" Properties
static uint8_t casIndicationProps = GATT_PROP_INDICATE;

// Characteristic "Indication" Value variable
static uint8_t casIndicationVal = 0;

// Characteristic "DataOut" Configuration each client has its own
// instantiation of the Client Characteristic Configuration. Reads of the
// Client Characteristic Configuration only shows the configuration for
// that client and writes only affect the configuration of that client.
static gattCharCfg_t *casIndicationConfig = NULL;

// Characteristic "Indication" User Description
static uint8_t casIndicationUserDesp[] = "Indication Characteristic";

// Characteristic "DataIn" Properties
static uint8_t casDataInProps = GATT_PROP_WRITE;

// Characteristic "DataIn" Value variable
static uint8_t casDataInVal = 0;

// Characteristic "DataIn" User Description
static uint8_t casDataInUserDesp[] = "Write Long Data";

// Characteristic "Pairing" Properties
static uint8_t casPairingProps = GATT_PROP_READ;

// Characteristic "Pairing" Value variable
static uint8_t casPairingVal = 0;

// Characteristic "Pairing" User Description
static uint8_t casPairingUserDesp[] = "Triggers Pairing";

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t cas_attrTbl[] =
{
 /*--------------------type-------------------*/ /*------------permissions-------------*/ /*------------------pValue--------------------*/
   // Car Access Service
   GATT_BT_ATT( primaryServiceUUID,                 GATT_PERMIT_READ,                       (uint8_t *) &casService ),

   // Indication Characteristic Properties
   GATT_BT_ATT( characterUUID,                      GATT_PERMIT_READ,                       &casIndicationProps ),
   // Indication Characteristic Value
   GATT_ATT( casIndicationCharUUID,                 0,                                      &casIndicationVal ),
   // Indication Characteristic configuration
   GATT_BT_ATT( clientCharCfgUUID,                  GATT_PERMIT_WRITE,                      (uint8_t *) &casIndicationConfig ),
   // Indication Characteristic User Description
   GATT_BT_ATT( charUserDescUUID,                   GATT_PERMIT_READ,                       casIndicationUserDesp ),

   // DataIn Characteristic Properties
   GATT_BT_ATT( characterUUID,                      GATT_PERMIT_READ,                       &casDataInProps ),
   // DataIn Characteristic Value
   GATT_ATT( casDataInCharUUID,                     GATT_PERMIT_WRITE,                      &casDataInVal ),
   // DataIn Characteristic User Description
   GATT_BT_ATT( charUserDescUUID,                   GATT_PERMIT_READ,                       casDataInUserDesp ),

   // Pairing Characteristic Properties
   GATT_BT_ATT( characterUUID,                      GATT_PERMIT_READ,                       &casPairingProps ),
   // Pairing Characteristic Value
   // TODO: Change the permission to GATT_PERMIT_AUTHEN_READ when OOB is implemented
   GATT_ATT( casPairingCharUUID,                    GATT_PERMIT_ENCRYPT_READ,               &casPairingVal ),
   // Pairing Characteristic User Description
   GATT_BT_ATT( charUserDescUUID,                   GATT_PERMIT_READ,                       casPairingUserDesp ),
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8_t CAS_writeAttrCB( uint16_t connHandle,
                                  gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint16_t len,
                                  uint16_t offset, uint8_t method );
static uint8_t CAS_readAttrCB( uint16_t connHandle, gattAttribute_t *pAttr,
                                uint8_t *pValue, uint16_t *pLen,
                                uint16_t offset, uint16_t maxLen,
                                uint8_t method );

/*********************************************************************
 * PROFILE CALLBACKS
 */

// Car Access Server Callbacks
// Note: When an operation on a characteristic requires authorization and
// pfnAuthorizeAttrCB is not defined for that characteristic's service, the
// Stack will report a status of ATT_ERR_UNLIKELY to the client.  When an
// operation on a characteristic requires authorization the Stack will call
// pfnAuthorizeAttrCB to check a client's authorization prior to calling
// pfnReadAttrCB or pfnWriteAttrCB, so no checks for authorization need to be
// made within these functions.
const gattServiceCBs_t cas_servCBs =
{
  CAS_readAttrCB,                 // Read callback function pointer
  CAS_writeAttrCB,                // Write callback function pointer
  NULL                            // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      CAS_addService
 *
 * @brief   This function initializes the Car Access Server service
 *          by registering GATT attributes with the GATT server.
 *
 * @return  SUCCESS or stack call status
 */
uint8_t CAS_addService( void )
{

  uint8_t status = SUCCESS;

  // Allocate Client Characteristic Configuration table
  casIndicationConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * MAX_NUM_BLE_CONNS );
  if ( casIndicationConfig != NULL )
  {
    // Initialize Client Characteristic Configuration attributes
    GATTServApp_InitCharCfg( LINKDB_CONNHANDLE_INVALID, casIndicationConfig );

    // Register GATT attribute list and CBs with GATT Server
    status = GATTServApp_RegisterService( cas_attrTbl,
                                          GATT_NUM_ATTRS( cas_attrTbl ),
                                          GATT_MAX_ENCRYPT_KEY_SIZE,
                                          &cas_servCBs );
  }
  else
  {
    status = bleMemAllocError;
  }

  // Return status value
  return ( status );
}

/*********************************************************************
 * @fn      CAS_registerProfileCBs
 *
 * @brief   Registers the profile callback function. Only call
 *          this function once.
 *
 * @param   profileCallback - pointer to profile callback functions.
 *
 * @return  SUCCESS or INVALIDPARAMETER
 */
uint8_t CAS_registerProfileCBs( CAS_cb_t *profileCallback )
{
  uint8_t status = SUCCESS;

  if ( profileCallback )
  {
    cas_profileCBs = profileCallback;
  }
  else
  {
    status = INVALIDPARAMETER;
  }

  // Return status value
  return ( status );
}

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
uint8_t CAS_setParameter(uint8_t param, void *pValue, uint16_t len)
{
  bStatus_t status = SUCCESS;
  switch (param)
  {
    case CAS_PAIRING_CHAR_ID:
      // Validate the value
      if (len == sizeof(uint8_t))
      {
        casPairingVal = *((uint8_t*)pValue);
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

  // Return status
  return ( status );
}

/*********************************************************************
 * @fn          CAS_readAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 * @param       method - type of read message
 *
 * @return      SUCCESS, blePending or Failure
 */
uint8_t CAS_readAttrCB(uint16_t connHandle,
                       gattAttribute_t *pAttr,
                       uint8_t *pValue, uint16_t *pLen,
                       uint16_t offset, uint16_t maxLen,
                       uint8_t method)
{
  uint8_t status = SUCCESS;
  // Make sure it's not a blob operation (no attributes in the profile are long)
  if ( offset > 0 )
  {
    status = ATT_ERR_ATTR_NOT_LONG;
  }
  else
  {
    // Check if this is the correct UUID
    if ( ! memcmp( pAttr->type.uuid, casPairingCharUUID, pAttr->type.len ) )
    {
      *pLen = CAS_PAIRING_CHAR_LEN;
      pValue[0] = casPairingVal;
    }
    else
    {
      *pLen = 0;
      status = ATT_ERR_ATTR_NOT_FOUND;
    }
  }

  // Return status value
  return ( status );
}

/*********************************************************************
 * @fn      CAS_writeAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   method - type of write message
 *
 * @return  SUCCESS or stack call status
 */
static uint8_t CAS_writeAttrCB( uint16_t connHandle,
                                gattAttribute_t *pAttr,
                                uint8_t *pValue, uint16_t len,
                                uint16_t offset, uint8_t method )
{
  uint8_t status = SUCCESS;

  // Verify input parameters
  if ( pAttr == NULL || pValue == NULL )
  {
    status = INVALIDPARAMETER;
  }
  else
  {
    /******************************************************/
    /****** Client Characteristic Configuration ***********/
    /******************************************************/

    if ( ! memcmp( pAttr->type.uuid, clientCharCfgUUID, pAttr->type.len ) )
    {
      CAS_cccUpdate_t *pCCCUpdate;

      // Allow only notifications
      status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                              offset, GATT_CLIENT_CFG_INDICATE );

      // Notify profile
      if ( status == SUCCESS )
      {
        // This allocation will be free by bleapp_util
        pCCCUpdate = (CAS_cccUpdate_t *)ICall_malloc( sizeof( CAS_cccUpdate_t ) );
        if ( pCCCUpdate == NULL )
        {
          // Error status
          status = bleMemAllocError;
        }
        else
        {
          // Copy the data and send it to the profile
          pCCCUpdate->connHandle = connHandle;
          pCCCUpdate->value = BUILD_UINT16( pValue[0], pValue[1] );

          // Callback function to notify profile of change on the client characteristic configuration
          BLEAppUtil_invokeFunction( cas_profileCBs->pfnOnCccUpdateCB, (char *)pCCCUpdate );
        }
      }
    }

    /******************************************************/
    /*********** Data In Characteristic  ******************/
    /******************************************************/
    else if ( ! memcmp( pAttr->type.uuid, casDataInCharUUID, pAttr->type.len ) )
    {
      // Only notify profile if there is any data in the payload
      if ( len > 0  && cas_profileCBs && cas_profileCBs->pfnIncomingDataCB)
      {
        CAS_dataIn_t *dataIn;

        if ( len > CAS_DATA_IN_CHAR_MAX_LEN )
        {
          len = 0;
        }

        // This allocation will be free by bleapp_util
        dataIn = (CAS_dataIn_t *)ICall_malloc( sizeof( CAS_dataIn_t ) + len);
        if ( dataIn == NULL )
        {
          // Error status
          status = bleMemAllocError;
        }
        else
        {
          // If allocation was successful,
          // Copy the data and send it to the profile
          if ( len > 0 )
          {
            memcpy( dataIn->pValue, pValue, len );
          }
          dataIn->connHandle = connHandle;
          dataIn->len = len;

          // Callback function to notify profile of change on the client characteristic configuration
          status = BLEAppUtil_invokeFunction( cas_profileCBs->pfnIncomingDataCB, (char *)dataIn );
        }
      }
    }

    // If we get here, that means you've forgotten to add an if clause for a
    // characteristic value attribute in the attribute table that has WRITE permissions.
    else
    {
      status = ATT_ERR_ATTR_NOT_FOUND;
    }

  }

  // Return status value
  return ( status );
}

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
uint8_t CAS_sendIndication(uint16_t connHandle)
{
  uint8_t status = SUCCESS;
  gattAttribute_t *pAttr = NULL;
  attHandleValueInd_t ind = { 0 };
  linkDBInfo_t connInfo = { 0 };
  uint16_t offset = 0;

  // Find the characteristic value attribute
  pAttr = GATTServApp_FindAttr( cas_attrTbl, GATT_NUM_ATTRS( cas_attrTbl ),
                                &casIndicationVal );

  if ( pAttr != NULL )
  {
    // Find out what the maximum MTU size is for each connection
    status = linkDB_GetInfo(connHandle, &connInfo);

    // Check if the connection is still valid
    if ( status != FAILURE )
    {
      // Check the CCCD value to indicate if notifications are enabled
      uint16_t value = GATTServApp_ReadCharCfg( connHandle, casIndicationConfig );
      if ( value & GATT_CLIENT_CFG_INDICATE )
      {
        // Determine allocation size
        uint16_t allocLen = (CAS_INDICATION_CHAR_LEN - offset);
        if ( allocLen > ( connInfo.MTU - CAS_IND_HDR_SIZE ) )
        {
          // If len > MTU split data to chunks of MTU size
          allocLen = connInfo.MTU - CAS_IND_HDR_SIZE;
        }

        ind.len = allocLen;
        ind.pValue = (uint8 *)GATT_bm_alloc( connHandle, ATT_HANDLE_VALUE_NOTI, allocLen, 0 );
        ind.handle = pAttr->handle;

        if ( ind.pValue != NULL )
        {
          // If allocation was successful, copy out data and send it
          memcpy( ind.pValue, pAttr->pValue + offset, ind.len );

          // Send the data over BLE notifications
          status = GATT_Indication( connHandle, &ind, FALSE, BLEAppUtil_getSelfEntity() );

          // If unable to send the data, free allocated buffers and return
          if ( status != SUCCESS )
          {
            GATT_bm_free( (gattMsg_t *)&ind, ATT_HANDLE_VALUE_NOTI );
          }
          else
          {
            // Increment data offset
            offset += allocLen;
          }
        }
        else
        {
          status = bleNoResources;
        }
      }
    }
  }

  // Return status value
  return ( status );
}

/*********************************************************************
*********************************************************************/
