/******************************************************************************

 @file  battery_server.c

 @brief This file contains the battery service sample for use with the Basic BLE
        Profiles sample application.

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
#include <ti/ble/services/battery/battery_server.h>

/*********************************************************************
 * DEFINES
 */

// The size of the notification header is opcode + handle
#define BAS_NOTI_HDR_SIZE   ( ATT_OPCODE_SIZE + 2 )

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
// HTS Service UUID: 0x180F
GATT_BT_UUID( bas_serv_UUID, BAS_SERV_UUID );

// BAS Level Characteristic UUID: 0x2A19
GATT_BT_UUID( bas_level_UUID, BAS_LEVEL_UUID );

static BAS_cb_t *bas_profileCBs = NULL;

/*********************************************************************
 * Service Attributes - variables
 */
// BAS Service declaration
static const gattAttrType_t bas_service = { ATT_BT_UUID_SIZE, bas_serv_UUID };

// BAS Level
static uint8_t bas_level_props = GATT_PROP_READ | GATT_PROP_NOTIFY;   // Characteristic properties
static uint8_t bas_level_val = 0;                                     // Value variable
static uint8_t bas_level_userDesp[] = "Battery Level";                // User description
static gattCharCfg_t *bas_level_config;                               // Client Characteristic Configuration

/*********************************************************************
 * Profile Attributes - Table
 */
static gattAttribute_t bas_attrTbl[] =
{
    /*----------------------type---------------------*//*------------------permissions-------------------*//*---------------pValue---------------*/
    // bas Service
    GATT_BT_ATT( primaryServiceUUID ,                    GATT_PERMIT_READ,                                  (uint8_t *) &bas_service ),

    // BAS Properties
    GATT_BT_ATT( characterUUID,                          GATT_PERMIT_READ,                                  &bas_level_props ),
    // BAS Level Value
    GATT_BT_ATT( bas_level_UUID,                         GATT_PERMIT_READ,                                  (uint8_t *) &bas_level_val ),
    // BAS Level configuration
    GATT_BT_ATT( clientCharCfgUUID,                      GATT_PERMIT_READ | GATT_PERMIT_WRITE,              (uint8_t *) &bas_level_config ),
    // BAS Level User Description
    GATT_BT_ATT( charUserDescUUID,                       GATT_PERMIT_READ,                                  bas_level_userDesp ),
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static bStatus_t BAS_readAttrCB( uint16_t connHandle, gattAttribute_t *pAttr,
                                uint8_t *pValue, uint16_t *pLen,
                                uint16_t offset, uint16_t maxLen,
                                uint8_t method );
static bStatus_t BAS_writeAttrCB( uint16_t connHandle, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint16_t len, uint16_t offset,
                                  uint8_t method );
static bStatus_t BAS_sendNotification( uint8_t *pValue, uint16_t len,
                                      gattCharCfg_t *charCfgTbl,
                                      uint8_t *pAttValue );
static bStatus_t BAS_initCCC( void );

/*********************************************************************
 * Service CALLBACKS
 */

/*
 * BAS Server Callbacks
 * \note When an operation on a characteristic requires authorization and
 * pfnAuthorizeAttrCB is not defined for that characteristic's service, the
 * Stack will report a status of ATT_ERR_UNLIKELY to the client.  When an
 * operation on a characteristic requires authorization the Stack will call
 * pfnAuthorizeAttrCB to check a client's authorization prior to calling
 * pfnReadAttrCB or pfnWriteAttrCB, so no checks for authorization need to be
 * made within these functions.
 *
 */
const gattServiceCBs_t bas_servCB =
{
    BAS_readAttrCB,             // Read callback function pointer
    BAS_writeAttrCB,            // Write callback function pointer
    NULL                        // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      BAS_addService
 *
 * @brief   This function initializes the Battery Server service
 *          by registering GATT attributes with the GATT server.
 */
bStatus_t BAS_addService( void )
{
    uint8_t status = SUCCESS;

    // Allocate Client Characteristic Configuration table
    status = BAS_initCCC();

    // Register GATT attribute list and CBs with GATT Server
    status = GATTServApp_RegisterService( bas_attrTbl,
                                         GATT_NUM_ATTRS( bas_attrTbl ),
                                         GATT_MAX_ENCRYPT_KEY_SIZE,
                                         &bas_servCB );

    // Return status value
    return ( status );
}

/*********************************************************************
 * @fn      BAS_registerProfileCBs
 *
 * @brief   Registers the profile callback function. Only call
 *          this function once.
 *
 * @param   profileCallback - pointer to profile callback.
 *
 * @return  SUCCESS or INVALIDPARAMETER
 */
bStatus_t BAS_registerProfileCBs( BAS_cb_t *profileCallback )
{
    bStatus_t status = SUCCESS;

    if ( NULL != profileCallback )
    {
        bas_profileCBs = profileCallback;
    }
    else
    {
        status = INVALIDPARAMETER;
    }

    // Return status value
    return ( status );
}

/*********************************************************************
 * @fn      BAS_setParameter
 *
 * @brief   Set a BAS Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to right
 * @param   value - pointer to data to write. This is dependent on
 *                  the parameter ID and will be cast to the appropriate
 *                  data type (example: data type of uint16_t will be cast to
 *                  uint16_t pointer).
 *
 * @return  SUCCESS or INVALIDPARAMETER
 */
bStatus_t BAS_setParameter( uint8_t param, uint8_t len, void *value )
{
    bStatus_t status = SUCCESS;

    switch ( param )
    {
        case BAS_LEVEL_ID:
        {
            if ( len == sizeof( uint8_t ) )
            {
                uint8_t newVal = *((uint8_t*) value);
                if ( newVal <= BAS_LEVEL_MAX )
                {
                    bas_level_val = newVal;
                    // Send data over BLE Notification
                    status = BAS_sendNotification( value, len, bas_level_config, &bas_level_val );
                }
                else
                {
                    status = INVALIDPARAMETER;
                }
            }
            else
            {
                status = bleInvalidRange;
            }
            break;
        }

        default:
        {
            status = INVALIDPARAMETER;
            break;
        }
    }

    // Return status value
    return ( status );
}

/*********************************************************************
 * @fn      BAS_getParameter
 *
 * @brief   Get a BAS parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and will be cast to the appropriate
 *          data type (example: data type of uint16_t will be cast to
 *          uint16_t pointer).
 *
 * @return  SUCCESS or error status
 */
bStatus_t BAS_getParameter( uint8_t param, void *pValue )
{
    bStatus_t status;
    switch ( param )
    {
        case BAS_LEVEL_ID:
        {
            VOID memcpy( pValue, &bas_level_val, BAS_LEVEL_CHAR_LEN );
            status = SUCCESS;
            break;
        }
        default:
        {
            status = INVALIDPARAMETER;
            break;
        }
    }

    // Return status value
    return ( status );
}

/*********************************************************************
 * @fn      BAS_writeAttrCB
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
static bStatus_t BAS_writeAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                 uint8_t *pValue, uint16_t len, uint16_t offset,
                                 uint8_t method)
{
    bStatus_t status = SUCCESS;

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

        if ( !memcmp( pAttr->type.uuid, clientCharCfgUUID, pAttr->type.len ) )
        {
            BAS_cccUpdate_t *cccUpdate;

            // Allow only notifications
            status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                                     offset, GATT_CLIENT_CFG_NOTIFY );

            // Notify profile
            if ( ( status == SUCCESS ) && ( NULL != bas_profileCBs ) && ( NULL != bas_profileCBs->sfnLevelOnCCCUpdateCB ) )
            {
                cccUpdate = (BAS_cccUpdate_t *)ICall_malloc( sizeof( BAS_cccUpdate_t ) );
                if ( cccUpdate == NULL )
                {
                  // Return error status
                  status =  bleMemAllocError;
                }
                else
                {
                    // Copy the data and send it to the profile
                    cccUpdate->connHandle = connHandle;
                    cccUpdate->value = BUILD_UINT16( pValue[0], pValue[1] );

                    // Callback function to notify profile of change on the client characteristic configuration
                    BLEAppUtil_invokeFunction( bas_profileCBs->sfnLevelOnCCCUpdateCB, (char *)cccUpdate );
                }
            }
        }
    }

    // Return status value
    return ( status );
}

/*********************************************************************
 * @fn          BAS_readAttrCB
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
static bStatus_t BAS_readAttrCB( uint16_t connHandle, gattAttribute_t *pAttr,
                                uint8_t *pValue, uint16_t *pLen,
                                uint16_t offset, uint16_t maxLen,
                                uint8_t method )
{
    bStatus_t status = SUCCESS;

    // Make sure it's not a blob operation (no attributes in the profile are long)
    if ( offset > 0 )
    {
        status = ATT_ERR_ATTR_NOT_LONG;
    }
    else
    {
        // Check if this is the correct UUID with correct length
        if ( ( ! memcmp( pAttr->type.uuid, bas_level_UUID, pAttr->type.len ) ) && ( pAttr->type.len == ATT_BT_UUID_SIZE ) )
        {
            *pLen = BAS_LEVEL_CHAR_LEN;
            VOID memcpy( pValue, &bas_level_val, BAS_LEVEL_CHAR_LEN );
        }
        else // Wrong Size
        {
            // 128-bit UUID
            *pLen = 0;
            status = ATT_ERR_INVALID_HANDLE;
        }
    }

    // Return status value
    return ( status );
}

/*********************************************************************
 * @fn      BAS_sendNotification
 *
 * @brief   Transmits data over BLE Notification.
 *
 * @param   pValue - pointer to data to be written
 * @param   len - length of data to be written
 * @param   charCfgTbl - characteristic configuration table.
 * @param   pAttValue - pointer to attribute value.
 *
 *
 * @return  SUCCESS, or stack call status
 */
static bStatus_t BAS_sendNotification( uint8_t *pValue, uint16_t len,
                                      gattCharCfg_t *charCfgTbl,
                                      uint8_t *pAttValue )
{
    bStatus_t status = SUCCESS;
    gattAttribute_t *pAttr = NULL;
    attHandleValueNoti_t noti = { 0 };
    linkDBInfo_t connInfo = { 0 };
    uint16_t offset = 0;
    uint8_t i = 0;

    // Verify input parameters
    if ( pValue == NULL )
    {
        status = INVALIDPARAMETER;
    }
    else
    {
        // Find the characteristic value attribute
        pAttr = GATTServApp_FindAttr( bas_attrTbl, GATT_NUM_ATTRS(bas_attrTbl),
                                     &bas_level_val );
        if ( pAttr != NULL )
        {

            // Check the CCC value for each BLE connection
            for ( i = 0; i < MAX_NUM_BLE_CONNS; i++ )
            {
                gattCharCfg_t *pItem = &(bas_level_config[i]);

                // If the connection has register for notifications
                if ( ( pItem->connHandle != LINKDB_CONNHANDLE_INVALID )
                        && ( pItem->value == GATT_CLIENT_CFG_NOTIFY ) )
                {
                    // Find out what the maximum MTU size is for each connection
                    status = linkDB_GetInfo( pItem->connHandle, &connInfo );
                    offset = 0;

                    while ( ( status != bleTimeout ) && ( status != bleNotConnected ) && ( len > offset ) )
                    {
                        // Determine allocation size
                        uint16_t allocLen = ( len - offset );
                        if ( allocLen > ( connInfo.MTU - BAS_NOTI_HDR_SIZE ) )
                        {
                            // If len > MTU split data to chunks of MTU size
                            allocLen = connInfo.MTU - BAS_NOTI_HDR_SIZE;
                        }

                        noti.len = allocLen;
                        noti.pValue = (uint8_t*) GATT_bm_alloc( pItem->connHandle,
                                                             ATT_HANDLE_VALUE_NOTI,
                                                             allocLen, 0 );
                        if ( noti.pValue != NULL )
                        {
                            // If allocation was successful, copy out data and send it
                            memcpy( noti.pValue, pValue + offset, noti.len );
                            noti.handle = pAttr->handle;

                            // Send the data over BLE notifications
                            status = GATT_Notification( pItem->connHandle, &noti,
                                                       FALSE );

                            // If unable to send the data, free allocated buffers and return
                            if ( status != SUCCESS )
                            {
                                GATT_bm_free( ( gattMsg_t* ) &noti, ATT_HANDLE_VALUE_NOTI );
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
                    } // End of while
                }
            } // End of for
        } // End of if
    }
    // Return status value
    return ( status );
}

/*********************************************************************
 * @fn      BAS_initCCC
 *
 * @brief   Allocate and initialize Client Characteristic Configuration table
 *
 * @return  SUCCESS, or bleMemAllocError
 */
static bStatus_t BAS_initCCC( void )
{
    bStatus_t status = SUCCESS;

    // Allocate client characteristic configuration table for BAS Level
    bas_level_config = ( gattCharCfg_t* ) ICall_malloc( sizeof( gattCharCfg_t ) * MAX_NUM_BLE_CONNS );
    if ( bas_level_config != NULL )
    {
        // Initialize Client Characteristic Configuration attributes
        GATTServApp_InitCharCfg( LINKDB_CONNHANDLE_INVALID, bas_level_config );
    }
    else
    {
        // Allocation fails return error status
        status = bleMemAllocError;
    }

    return ( status );
}

/*********************************************************************
 *********************************************************************/
