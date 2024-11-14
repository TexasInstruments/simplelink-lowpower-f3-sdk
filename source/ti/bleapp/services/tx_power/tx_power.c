/******************************************************************************

 @file  tx_power.c

 @brief This file contains the TX Power service sample GATT service
        for use with the BLE sample application.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2010-2024, Texas Instruments Incorporated
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
#include <icall.h>
/* This Header file contains all BLE API and icall structure definition */
#include "icall_ble_api.h"

#include <ti/bleapp/services/tx_power/tx_power.h>

/*********************************************************************
 * CONSTANTS
 */

// TPL Service UUID: 0x1804
GATT_BT_UUID( txPwrLevelServUUID, TPL_SERV_UUID );

// TPL Characteristic UUID: 0x2A07
GATT_BT_UUID( tx_power_level_UUID, TPLS_TX_POWER_LEVEL_UUID );

// Tx Power Level Service.
static const gattAttrType_t txPwrLevelService = { ATT_BT_UUID_SIZE,
                                                  txPwrLevelServUUID };

/*********************************************************************
 * LOCAL VARIABLES
 */


/*********************************************************************
 * Service Attributes - variables
 */

// Tx Power Level Characteristic Properties.
static uint8 txPwrLevelCharProps = GATT_PROP_READ;
// Tx Power Level attribute.
// This attribute represents the range of transmit power levels in dBm with
// a range from -100 to +20
static int8 txPwrLevelVal = 0;
// User description
static uint8 txPwrLevelCharUserDesp[] = "TX Power Level";

/*********************************************************************
 * Service Attributes - Table
 */

static gattAttribute_t tpls_attrTbl[] =
{
 /*----------------------type---------------------*/ /*--------permissions---------*/ /*---------------pValue---------------*/
   // TPL Service
   GATT_BT_ATT( primaryServiceUUID,                    GATT_PERMIT_READ,                    (uint8 *) &txPwrLevelService ),

   // TPL TX Power Level Properties
   GATT_BT_ATT( characterUUID,                         GATT_PERMIT_READ,                    &txPwrLevelCharProps ),
   // TPL TX Power Level Value
   GATT_BT_ATT( tx_power_level_UUID,                   GATT_PERMIT_READ,                    (uint8 *) &txPwrLevelVal ),
   // TPL TX Power Level User Description
   GATT_BT_ATT( charUserDescUUID,                      GATT_PERMIT_READ,                    txPwrLevelCharUserDesp ),

};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t TPLS_readAttrCB( uint16_t connHandle, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint16_t *pLen, uint16_t offset,
                                  uint16_t maxLen, uint8_t method );

/*********************************************************************
 * Service CALLBACKS
 */

/*
 * TPL Server Callbacks
 * \note When an operation on a characteristic requires authorization and
 * pfnAuthorizeAttrCB is not defined for that characteristic's service, the
 * Stack will report a status of ATT_ERR_UNLIKELY to the client.  When an
 * operation on a characteristic requires authorization the Stack will call
 * pfnAuthorizeAttrCB to check a client's authorization prior to calling
 * pfnReadAttrCB or pfnWriteAttrCB, so no checks for authorization need to be
 * made within these functions.
 *
 */
const gattServiceCBs_t tpls_servCB =
{
 TPLS_readAttrCB,   // Read callback function pointer
  NULL,  // Write callback function pointer
  NULL               // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      TPLS_addService
 *
 * @brief   This function initializes the TX Power Level service
 *          by registering GATT attributes with the GATT server.
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t TPLS_addService( void )
{
  uint8 status = SUCCESS;

  // Register GATT attribute list and CBs with GATT Server
  status = GATTServApp_RegisterService( tpls_attrTbl,
                                        GATT_NUM_ATTRS( tpls_attrTbl ),
                                        GATT_MAX_ENCRYPT_KEY_SIZE,
                                        &tpls_servCB );

  // Return status value
  return ( status );
}

/*********************************************************************
 * @fn      TPLS_setParameter
 *
 * @brief   Set an TX Power Level Service parameter.
 *
 * @param   param - Characteristic UUID
 * @param   pValue - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 * @param   len - length of data to write
 *
 * @return  SUCCESS or error status
 */
bStatus_t TPLS_setParameter(uint8 param, void *pValue, uint16 len)
{
  bStatus_t status = bleInvalidRange;

  // Verify input parameters
  if ( pValue == NULL)
  {
    return ( INVALIDPARAMETER );
  }

  switch ( param )
  {
    // TX Power Level
    case TPLS_TX_POWER_LEVEL_ID:
      if ( len ==  TPLS_TX_POWER_LEVEL_LEN )
      {
        status = SUCCESS;
        VOID memcpy( &txPwrLevelVal, pValue, TPLS_TX_POWER_LEVEL_LEN );
      }
      break;

    default:
      status = INVALIDPARAMETER;
      break;
  }

  // Return status value
  return ( status );
}

/*********************************************************************
 * @fn      TPLS_getParameter
 *
 * @brief   Get an TX Power Level parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  SUCCESS or error status
 */
bStatus_t TPLS_getParameter( uint8 param, void *pValue )
{
  bStatus_t status = SUCCESS;
  switch ( param )
  {
    // TX Power Level
    case TPLS_TX_POWER_LEVEL_ID:
      VOID memcpy( pValue, &txPwrLevelVal, TPLS_TX_POWER_LEVEL_LEN);
      break;

    default:
      status = INVALIDPARAMETER;
      break;
  }

  // Return status value
  return ( status );
}

/*********************************************************************
 * @fn          TPLS_readAttrCB
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
static bStatus_t TPLS_readAttrCB( uint16_t connHandle, gattAttribute_t *pAttr,
                           uint8_t *pValue, uint16_t *pLen, uint16_t offset,
                           uint16_t maxLen, uint8_t method )
{
  bStatus_t status = SUCCESS;

  if ( ! memcmp( pAttr->type.uuid, tx_power_level_UUID, pAttr->type.len ) )
  {
    *pLen = TPLS_TX_POWER_LEVEL_LEN;
    VOID memcpy( pValue, pAttr->pValue, TPLS_TX_POWER_LEVEL_LEN );
  }

  // Return status value
  return ( status );
}

/*********************************************************************
*********************************************************************/
