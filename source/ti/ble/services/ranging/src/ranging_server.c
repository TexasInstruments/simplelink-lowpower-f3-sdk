/******************************************************************************

 @file  ranging_server.c

 @brief This file contains the ranging service implementation for use with BLE
        applications.

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
#include "ti/ble/app_util/framework/bleapputil_api.h"
#include "ti/ble/services/ranging/ranging_server.h"
 /* This Header file contains all BLE API and icall structure definition */
#include "ti/ble/stack_util/icall/app/icall_ble_api.h"

 /*********************************************************************
 * DEFINES
 */

// Position of Client Characteristic Configuration entries in the attribute array
#define RAS_REAL_TIME_CCC_TBL_INDEX         0xFF // Currently not supported
#define RAS_ON_DEMAND_CCC_TBL_INDEX         6    // On Demand Ranging Data CCC index
#define RAS_CP_CCC_TBL_INDEX                10   // Ranging Control Point CCC index
#define RAS_DATA_READY_TBL_CCC_INDEX        14   // Ranging Data Ready CCC index
#define RAS_DATA_OVERWRITTEN_CCC_TBL_INDEX  18   // Ranging Data Overwritten CCC index

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

 // RAS Service UUID: 0x180
GATT_BT_UUID( ras_serv_UUID, RANGING_SERVICE_UUID );

// RAS Feature UUID: 0x2C14
GATT_BT_UUID(ras_feat_UUID, RAS_FEATURE_UUID);

/* *** Not Supported yet ***
// RAS Ranging Data real-time UUID: 0x2C15
GATT_BT_UUID(ras_rt_UUID, RAS_REAL_TIME_UUID);
*/

// RAS Ranging Data on-demand UUID: 0x2C16
GATT_BT_UUID(ras_od_UUID, RAS_ON_DEMAND_UUID);

// RAS Control Point UUID: 0x2C17
GATT_BT_UUID(ras_CP_UUID, RAS_CONTROL_POINT_UUID);

// RAS Ranging Data is ready UUID: 0x2C18
GATT_BT_UUID(ras_ready_UUID, RAS_DATA_READY_UUID);

// RAS Ranging Data is overwritten UUID: 0x2C19
GATT_BT_UUID(ras_data_overwritten_UUID, RAS_DATA_OVERWRITTEN_UUID);

static RAS_cb_t *ras_profileCBs = NULL;

/*********************************************************************
 * Service Attributes - variables
 */

// RAS Service declaration
static const gattAttrType_t ras_service = { ATT_BT_UUID_SIZE, ras_serv_UUID };

// RAS Feature
static uint8_t ras_feat_props = GATT_PROP_READ;            // Characteristic properties
static uint32  ras_feat_val = 0;                           // Value variable
static uint8_t ras_feat_userDesp[] = "Ranging Feature";    // User description

/* *** Not Supported yet ***
// RAS real-time Ranging Data
static uint8_t ras_realTimeRD_props = GATT_PROP_INDICATE | GATT_PROP_NOTIFY;  // Characteristic properties
static uint8_t ras_realTimeRD_val = 0;                                        //  Value variable
static uint8_t ras_realTimeRD_userDesp[] = "RAS real-time Ranging Data";      // User description
static gattCharCfg_t *ras_realTimeRD_config = NULL;
*/

// RAS on-demand Ranging Data
static uint8_t ras_onDemandRD_props = GATT_PROP_INDICATE | GATT_PROP_NOTIFY;  // Characteristic properties
static uint8_t ras_onDemandRD_val = 0;                                        //  Value variable
static uint8_t ras_onDemandRD_userDesp[] = "RAS On-Demand Ranging Data";      // User description
static gattCharCfg_t *ras_onDemandRD_config = NULL;                           // Client Characteristic Configuration

// Ranging Control Point
static uint8_t ras_CP_props = GATT_PROP_WRITE_NO_RSP | GATT_PROP_INDICATE;  // Characteristic properties
static uint8_t ras_CP_val = 0;                                              //  Value variable
static uint8_t ras_CP_userDesp[] = "Ranging Control Point";                 // User description
static gattCharCfg_t *ras_CP_config = NULL;

// Ranging Data Ready
static uint8_t ras_dataReady_props = GATT_PROP_INDICATE | GATT_PROP_NOTIFY;   // Characteristic properties
static uint16 ras_dataReady_val = 0;                                          //  Value variable
static uint8_t ras_dataReady_userDesp[] = "RAS On-Demand Ranging Data";       // User description
static gattCharCfg_t *ras_dataReady_config = NULL;                            // Client Characteristic Configuration

// Ranging Data Overwritten
static uint8_t ras_dataOW_props = GATT_PROP_INDICATE | GATT_PROP_NOTIFY;    // Characteristic properties
static uint16 ras_dataOW_val = 0;                                           //  Value variable
static uint8_t ras_dataOW_userDesp[] = "RAS On-Demand Ranging Data";        // User description
static gattCharCfg_t *ras_dataOW_config = NULL;                             // Client Characteristic Configuration

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t ras_attrTbl[] =
{
 /*----------------------type---------------------*/ /*------------------permissions-------------------*/ /*---------------pValue---------------*/
    // ras Service
    GATT_BT_ATT( primaryServiceUUID,                    GATT_PERMIT_ENCRYPT_READ,                                    (uint8_t *) &ras_service ),

    // ras Feature Properties
    GATT_BT_ATT( characterUUID,                         GATT_PERMIT_ENCRYPT_READ,                                    &ras_feat_props ),
    // ras Feature Value
    GATT_BT_ATT( ras_feat_UUID,                         GATT_PERMIT_ENCRYPT_READ,                                    (uint8_t *) &ras_feat_val ),
    // ras Feature User Description
    GATT_BT_ATT( charUserDescUUID,                      GATT_PERMIT_ENCRYPT_READ,                                    ras_feat_userDesp ),

    /* *** Not Supported yet ***
    // RAS real-time Ranging Data Properties
    GATT_BT_ATT( characterUUID,                         GATT_PERMIT_ENCRYPT_READ,                                    &ras_realTimeRD_props ),
    // RAS real-time Ranging Data Value
    GATT_BT_ATT( ras_rt_UUID,                           0,                                                           &ras_realTimeRD_val ),
    // RAS real-time Ranging Data configuration
    GATT_BT_ATT( clientCharCfgUUID,                     GATT_PERMIT_ENCRYPT_READ | GATT_PERMIT_ENCRYPT_WRITE,        (uint8_t *) &ras_realTimeRD_config ),
    // RAS real-time Ranging Data User Description
    GATT_BT_ATT( charUserDescUUID,                      GATT_PERMIT_ENCRYPT_READ,                                    ras_realTimeRD_userDesp ),
    */
    // RAS on-demand Ranging Data Properties
    GATT_BT_ATT( characterUUID,                         GATT_PERMIT_ENCRYPT_READ,                                    &ras_onDemandRD_props ),
    // RAS on-demand Ranging Data Value
    GATT_BT_ATT( ras_od_UUID,                           0,                                                           &ras_onDemandRD_val ),
    // RAS on-demand Ranging Data configuration
    GATT_BT_ATT( clientCharCfgUUID,                     GATT_PERMIT_ENCRYPT_READ | GATT_PERMIT_ENCRYPT_WRITE,        (uint8_t *) &ras_onDemandRD_config ),
    // RAS on-demand Ranging Data User Description
    GATT_BT_ATT( charUserDescUUID,                      GATT_PERMIT_ENCRYPT_READ,                                    ras_onDemandRD_userDesp ),

    // Ranging Control Point Properties
    GATT_BT_ATT( characterUUID,                         GATT_PERMIT_ENCRYPT_READ,                                    &ras_CP_props ),
    // Ranging Control Point Value
    GATT_BT_ATT( ras_CP_UUID,                           GATT_PERMIT_ENCRYPT_WRITE,                                   &ras_CP_val ),
    // Ranging Control Point configuration
    GATT_BT_ATT( clientCharCfgUUID,                     GATT_PERMIT_ENCRYPT_READ | GATT_PERMIT_ENCRYPT_WRITE,        (uint8_t *) &ras_CP_config ),
    // Ranging Control Point User Description
    GATT_BT_ATT( charUserDescUUID,                      GATT_PERMIT_ENCRYPT_READ,                                    ras_CP_userDesp ),

    // Ranging Data Ready Properties
    GATT_BT_ATT( characterUUID,                         GATT_PERMIT_ENCRYPT_READ,                                    &ras_dataReady_props ),
    // Ranging Data Ready Value
    GATT_BT_ATT( ras_ready_UUID,                        0,                                                           (uint8_t *)&ras_dataReady_val ),
    // Ranging Data Ready configuration
    GATT_BT_ATT( clientCharCfgUUID,                     GATT_PERMIT_ENCRYPT_READ | GATT_PERMIT_ENCRYPT_WRITE,        (uint8_t *) &ras_dataReady_config ),
    // Ranging Data Ready User Description
    GATT_BT_ATT( charUserDescUUID,                      GATT_PERMIT_ENCRYPT_READ,                                    ras_dataReady_userDesp ),

    // Ranging Data Overwritten Properties
    GATT_BT_ATT( characterUUID,                         GATT_PERMIT_ENCRYPT_READ,                                    &ras_dataOW_props ),
    // Ranging Data Overwritten Value
    GATT_BT_ATT( ras_data_overwritten_UUID,             0,                                                           (uint8_t *)&ras_dataOW_val ),
    // Ranging Data Overwritten configuration
    GATT_BT_ATT( clientCharCfgUUID,                     GATT_PERMIT_ENCRYPT_READ | GATT_PERMIT_ENCRYPT_WRITE,        (uint8_t *) &ras_dataOW_config ),
    // Ranging Data Overwritten User Description
    GATT_BT_ATT( charUserDescUUID,                      GATT_PERMIT_ENCRYPT_READ,                                    ras_dataOW_userDesp ),
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static uint8_t rasSendNotiInd(uint16_t connHandle, uint8_t sendModePreference, uint8_t *pValue,
                              uint16 len, gattCharCfg_t *charCfgTbl, uint8_t *pAttValue);
static uint8_t rasInitCCC( void );
static uint8_t rasCCCWriteReq( uint16 connHandle, gattAttribute_t *pAttr,
                               uint8_t *pValue, uint16 len, uint16 offset,
                               uint16 validCfg, InvokeFromBLEAppUtilContext_t callback,
                               uint16_t uuid );
static uint8_t rasReadAttrCB( uint16_t connHandle, gattAttribute_t *pAttr,
                              uint8_t *pValue, uint16_t *pLen, uint16_t offset,
                              uint16_t maxLen, uint8_t method);
static uint8_t rasWriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                               uint8_t *pValue, uint16 len,uint16 offset,
                               uint8_t method);

/*********************************************************************
* Service CALLBACKS
*/

/*
* Ranging Server Callbacks
* \note When an operation on a characteristic requires authorization and
* pfnAuthorizeAttrCB is not defined for that characteristic's service, the
* Stack will report a status of ATT_ERR_UNLIKELY to the client.  When an
* operation on a characteristic requires authorization the Stack will call
* pfnAuthorizeAttrCB to check a client's authorization prior to calling
* pfnReadAttrCB or pfnWriteAttrCB, so no checks for authorization need to be
* made within these functions.
*
*/
const gattServiceCBs_t ras_servCB =
{
    rasReadAttrCB,   // Read callback function pointer
    rasWriteAttrCB,  // Write callback function pointer
    NULL              // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*******************************************************************************
 * Public function defined in ranging_server.h
 */
uint8_t RAS_addService(void)
{
    uint8_t status = FAILURE;

    // Allocate and initialize Client Characteristic Configuration tables
    status = rasInitCCC();

    if (status == SUCCESS)
    {
        // Register GATT attribute list and callbacks with GATT Server
        status = GATTServApp_RegisterService( ras_attrTbl,
                                                GATT_NUM_ATTRS(ras_attrTbl),
                                                GATT_MAX_ENCRYPT_KEY_SIZE,
                                                &ras_servCB );
    }

    // Return status value
    return status;
}

/*******************************************************************************
 * Public function defined in ranging_server.h
 */
uint8_t RAS_registerProfileCBs(RAS_cb_t *profileCallback)
{
  uint8_t status = SUCCESS;

  if ( profileCallback )
  {
    ras_profileCBs = profileCallback;
  }
  else
  {
    status = INVALIDPARAMETER;
  }

  // Return status value
  return ( status );
}

/*******************************************************************************
 * Public function defined in ranging_server.h
 */
uint8_t RAS_setParameter(uint16_t connHandle, uint8_t param, void *pValue, uint16 len)
{
    uint8_t status = bleInvalidRange;

    // Verify input parameters
    if ( pValue == NULL)
    {
        return ( INVALIDPARAMETER );
    }

    switch ( param )
    {
        // RAS real-time Ranging Data
        /* *** Not Supported yet ***
        case RAS_REAL_TIME_ID:
            if (len <= ATT_MAX_MTU_SIZE)
            {
                // Send data over BLE notification
                status = rasSendNotiInd(connHandle, GATT_CLIENT_CFG_NOTIFY, pValue, len, ras_realTimeRD_config, &ras_realTimeRD_val);
            }
            break;
        */

        // RAS on-demand Ranging Data
        case RAS_ON_DEMAND_ID:
            if (len <= ATT_MAX_MTU_SIZE)
            {
                // Send data over BLE notification
                status = rasSendNotiInd(connHandle, GATT_CLIENT_CFG_NOTIFY, pValue, len, ras_onDemandRD_config, &ras_onDemandRD_val);
            }
            break;

        // RAS Feature
        case RAS_FEAT_ID:
            if (len ==  RAS_FEAT_LEN)
            {
                status = SUCCESS;
                VOID memcpy( &ras_feat_val, pValue, RAS_FEAT_LEN );
            }
            break;

        // Ranging Control Point
        case RAS_CONTROL_POINT_ID:
            if (len < RAS_CP_RSP_MAX_LEN )
            {
                // Send data over BLE indication
                status = rasSendNotiInd(connHandle, GATT_CLIENT_CFG_INDICATE, pValue, len, ras_CP_config, &ras_CP_val);
            }
            break;

        // Ranging Data Ready
        case RAS_READY_ID:
            if (len == RAS_DATA_READY_LEN)
            {
                // Send data over BLE notification
                status = rasSendNotiInd(connHandle, GATT_CLIENT_CFG_INDICATE, pValue, len, ras_dataReady_config, (uint8_t*)&ras_dataReady_val);
            }
            break;

        // Ranging Data Overwritten
        case RAS_OVERWRITTEN_ID:
            if (len == RAS_OVERWRITTEN_LEN)
            {
                // Send data over BLE notification
                status = rasSendNotiInd(connHandle, GATT_CLIENT_CFG_INDICATE, pValue, len, ras_dataOW_config, (uint8_t*)&ras_dataOW_val);
            }

        default:
            status = INVALIDPARAMETER;
            break;
    }

    // Return status value
    return ( status );
}

/*******************************************************************************
 * Public function defined in ranging_server.h
 */
uint8_t RAS_getParameter(uint8_t param, void *pValue)
{
  uint8_t status;
  switch ( param )
  {
    // RAS Features
    case RAS_FEAT_ID:
      VOID memcpy( pValue, &ras_feat_val, RAS_FEAT_LEN);
      status = SUCCESS;
      break;

    default:
      status = INVALIDPARAMETER;
      break;
  }

  // Return status value
  return ( status );
}

/*********************************************************************
 * @fn      rasInitCCC
 *
 * @brief   Allocate and initialize Client Characteristic Configuration table
 *
 * @return  SUCCESS, or bleMemAllocError
 */
static uint8_t rasInitCCC(void)
{
    gattCharCfg_t **configs[] = { &ras_onDemandRD_config, &ras_CP_config, &ras_dataReady_config, &ras_dataOW_config };
    uint8_t numConfigs = sizeof(configs) / sizeof(configs[0]);

    // Allocate and initialize each Client Characteristic Configuration table
    for (uint8_t i = 0; i < numConfigs; i++)
    {
        *configs[i] = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) * MAX_NUM_BLE_CONNS);
        if (*configs[i] == NULL)
        {
            // Free previously allocated configurations on failure
            for (uint8_t j = 0; j < i; j++)
            {
                ICall_free(*configs[j]);
                *configs[j] = NULL;
            }
            return bleMemAllocError;
        }
        GATTServApp_InitCharCfg(LINKDB_CONNHANDLE_INVALID, *configs[i]);
    }

    return SUCCESS;
}

/*********************************************************************
 * @fn      rasWriteAttrCB
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
static uint8_t rasWriteAttrCB(uint16 connHandle, gattAttribute_t *pAttr,
                              uint8_t *pValue, uint16 len,
                              uint16 offset, uint8_t method)
{
    uint8_t status = SUCCESS;

    /******************************************************/
    /****** Client Characteristic Configuration ***********/
    /******************************************************/
    if ( ! memcmp( pAttr->type.uuid, clientCharCfgUUID, pAttr->type.len ) )
    {
        // Real Time CC writing **Not Supported**
        /*
        if ( pAttr->handle == ras_attrTbl[RAS_REAL_TIME_CCC_TBL_INDEX].handle )
        {

            status = rasCCCWriteReq( connHandle, pAttr, pValue, len, offset,
                                      ras_realTimeRD_props,
                                      ras_profileCBs->pfnCccUpdateCB);

        }
        */
        if ( pAttr->handle == ras_attrTbl[RAS_ON_DEMAND_CCC_TBL_INDEX].handle )
        {

            status = rasCCCWriteReq( connHandle, pAttr, pValue, len, offset,
                                     GATT_CLIENT_CFG_NOTIFY | GATT_CLIENT_CFG_INDICATE,
                                     ras_profileCBs->pfnCccUpdateCB,
                                     RAS_ON_DEMAND_UUID );
        }

        if ( pAttr->handle == ras_attrTbl[RAS_DATA_READY_TBL_CCC_INDEX].handle )
        {

            status = rasCCCWriteReq( connHandle, pAttr, pValue, len, offset,
                                     GATT_CLIENT_CFG_NOTIFY | GATT_CLIENT_CFG_INDICATE,
                                     ras_profileCBs->pfnCccUpdateCB,
                                     RAS_DATA_READY_UUID );
        }

        if ( pAttr->handle == ras_attrTbl[RAS_DATA_OVERWRITTEN_CCC_TBL_INDEX].handle )
        {

            status = rasCCCWriteReq( connHandle, pAttr, pValue, len, offset,
                                     GATT_CLIENT_CFG_NOTIFY | GATT_CLIENT_CFG_INDICATE,
                                     ras_profileCBs->pfnCccUpdateCB,
                                     RAS_DATA_OVERWRITTEN_UUID );
        }

        if ( pAttr->handle == ras_attrTbl[RAS_CP_CCC_TBL_INDEX].handle )
        {

            status = rasCCCWriteReq( connHandle, pAttr, pValue, len, offset,
                                     GATT_CLIENT_CFG_INDICATE,
                                     ras_profileCBs->pfnCccUpdateCB,
                                     RAS_CONTROL_POINT_UUID );
        }
    }

    /******************************************************/
    /************** Ranging Control Point *****************/
    /******************************************************/
    else if ( ! memcmp( pAttr->type.uuid, ras_CP_UUID, pAttr->type.len ) )
    {

        // Only notify profile if the data is in appropriate size
        if (( len >= RAS_CP_COMMANDS_MIN_LEN &&  len <= RAS_CP_COMMANDS_MAX_LEN ) &&
        ( ras_profileCBs && ras_profileCBs->pfnCPReqCB ) )
        {
            RAS_CPCB_t *pCPReq = NULL;

            // This allocation will be free by bleapp_util
            pCPReq = (RAS_CPCB_t *)ICall_malloc( sizeof( RAS_CPCB_t ));
            if ( pCPReq != NULL )
            {
                memset( pCPReq, 0, sizeof( RAS_CPCB_t ));
                // Copy the data and send it to the profile
                pCPReq->connHandle = connHandle;
                pCPReq->msgLength = len;
                if( len > RAS_CP_RSP_MAX_LEN )
                {
                    // Limit the length to max response length
                    len = RAS_CP_RSP_MAX_LEN;
                }
                VOID memcpy(&pCPReq->CPMsg, pValue, len);

                // Callback function to notify profile of change on CP characteristic
                BLEAppUtil_invokeFunction( ras_profileCBs->pfnCPReqCB, (char *)pCPReq );
            }
        }
    }
    else
    {
        // If we get here, that means you've forgotten to add an if clause for a
        // characteristic value attribute in the attribute table that has WRITE permissions.
        status = ATT_ERR_ATTR_NOT_FOUND;
    }

    // Return status value
    return ( status );
}

/*********************************************************************
 * @fn          rasReadAttrCB
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
static uint8_t rasReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                             uint8_t *pValue, uint16_t *pLen, uint16_t offset,
                             uint16_t maxLen, uint8_t method)
{
    uint8_t status = SUCCESS;

    // Make sure it's not a blob operation (no attributes in the profile are long)
    if ( offset > 0 )
    {
        return ( ATT_ERR_ATTR_NOT_LONG );
    }

    if ( pAttr->type.len == ATT_BT_UUID_SIZE )
    {
        // 16-bit UUID
        uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);

        switch ( uuid )
        {
            // RAS Features
            case RAS_FEATURE_UUID:
            {
                *pLen = RAS_FEAT_LEN;
                VOID memcpy( pValue, &ras_feat_val, RAS_FEAT_LEN);
                break;
            }

            default:
            {
                status = INVALIDPARAMETER;
                break;
            }
        }
    }
    else // Wrong Size
    {
        // 128-bit UUID
        *pLen = 0;
        status = ATT_ERR_INVALID_HANDLE;
    }

    // Return status value
    return ( status );
}

/*********************************************************************
 * @fn      rasSendNotiInd
 *
 * @brief   Transmits data over BLE Notification/Indication.
 *
 * @param   connHandle - connection handle to send data on.
 * @param   sendModePreference - GATT_CLIENT_CFG_NOTIFY or GATT_CLIENT_CFG_INDICATE preference to send data.
 * @param   pValue - pointer to data to be written.
 * @param   len - length of data to be written.
 * @param   charCfgTbl - characteristic configuration table.
 * @param   pAttValue - pointer to attribute value.
 *
 *
 * @return  SUCCESS, or stack call status
 */
static uint8_t rasSendNotiInd(uint16_t connHandle, uint8_t sendModePreference, uint8_t *pValue,
                              uint16 len, gattCharCfg_t *charCfgTbl, uint8_t *pAttValue)
{
    uint8_t status = FAILURE;
    gattAttribute_t *pAttr = NULL;
    attHandleValueNoti_t noti = {0};
    linkDBInfo_t connInfo = {0};
    uint8_t i = 0;

    // Verify input parameters
    if (( charCfgTbl == NULL ) || ( pValue == NULL ) || ( pAttValue == NULL ))
    {
        return ( INVALIDPARAMETER );
    }

    // Find the characteristic value attribute
    pAttr = GATTServApp_FindAttr(ras_attrTbl, GATT_NUM_ATTRS(ras_attrTbl), pAttValue);

    if ( pAttr != NULL )
    {
        // Search for the connection handle in the characteristic configuration table
        for ( i = 0; i < MAX_NUM_BLE_CONNS; i++ )
        {
            gattCharCfg_t *pItem = &( charCfgTbl[i] );

            // Check if found the desired connection handle and if the value is not GATT_CFG_NO_OPERATION
            if ( ( pItem->connHandle == connHandle ) && ( pItem->value != GATT_CFG_NO_OPERATION) )
            {
                status = linkDB_GetInfo(connHandle, &connInfo);

                // Check if the connection is still valid
                if( status != bleTimeout && status != bleNotConnected )
                {
                    // Determine allocation size
                    noti.len = len;
                    noti.pValue = (uint8_t *)GATT_bm_alloc( connHandle, ATT_HANDLE_VALUE_NOTI, len, 0 );
                    if ( noti.pValue != NULL )
                    {
                        // If allocation was successful, copy out data and send it
                        VOID memcpy(noti.pValue, pValue, noti.len);
                        noti.handle = pAttr->handle;
                        if( ( (pItem->value & GATT_CLIENT_CFG_NOTIFY) != 0 ) &&
                            ( (pItem->value & GATT_CLIENT_CFG_INDICATE) != 0 ) )
                        {
                            // If Client Characteristic Configuration is both notifications and indication
                            if(sendModePreference == GATT_CLIENT_CFG_NOTIFY)
                            {
                                // Send the data over BLE notifications
                                status = GATT_Notification( connHandle, &noti, FALSE );
                            }
                            else if(sendModePreference == GATT_CLIENT_CFG_INDICATE)
                            {
                                // Send the data over BLE indication
                                status = GATT_Indication( connHandle, (attHandleValueInd_t *)&noti, FALSE, BLEAppUtil_getSelfEntity() );
                            }
                        }
                        else if( (pItem->value & GATT_CLIENT_CFG_NOTIFY) != 0 )
                        {
                            // Send the data over BLE notifications
                            status = GATT_Notification( connHandle, &noti, FALSE );

                        }
                        else if ( (pItem->value & GATT_CLIENT_CFG_INDICATE) != 0 )
                        {
                            // Send the data over BLE indication
                            status = GATT_Indication( connHandle, (attHandleValueInd_t *)&noti, FALSE, BLEAppUtil_getSelfEntity() );
                        }
                        else
                        {
                            // If Client Characteristic Configuration is not notifications/indication
                            status = INVALIDPARAMETER;
                        }

                        // If unable to send the data, free allocated buffers and return
                        if ( status != SUCCESS )
                        {
                            GATT_bm_free( (gattMsg_t *)&noti, ATT_HANDLE_VALUE_NOTI );
                        }
                    }
                    else
                    {
                        status = bleNoResources;
                    }
                }

                // Found the connection handle, exit the loop
                break;
            }
        }
    }

    // Return status value
    return ( status );
}

/*********************************************************************
 * @fn      rasCCCWriteReq
 *
 * @brief   Process CCC Write request and update the profile
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   validCfg - valid configuration
 * @param   callback - profile callback function
 * @param   uuid - UUID of the characteristic whose CCC is being written
 *
 * @return  SUCCESS or stack call status
 */
static uint8_t rasCCCWriteReq( uint16 connHandle, gattAttribute_t *pAttr,
                               uint8_t *pValue, uint16 len, uint16 offset,
                               uint16 validCfg, InvokeFromBLEAppUtilContext_t callback,
                               uint16_t uuid )
{
    uint8_t status = SUCCESS;
    RAS_cccUpdate_t *cccUpdate = NULL;

    // Process CCC Write request
    status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len, offset, validCfg );

    if ( status == SUCCESS )
    {
        // This allocation will be free by bleapp_util
        cccUpdate = (RAS_cccUpdate_t *)ICall_malloc( sizeof( RAS_cccUpdate_t ) );
        if ( cccUpdate != NULL )
        {
            // Copy the data and send it to the profile
            cccUpdate->connHandle = connHandle;
            cccUpdate->uuid =  uuid;
            cccUpdate->value = BUILD_UINT16( pValue[0], pValue[1] );

            // Callback function to notify profile of change on the client characteristic configuration
            BLEAppUtil_invokeFunction( callback, (char *)cccUpdate );
        }
    }

    // Return status value
    return ( status );
}
