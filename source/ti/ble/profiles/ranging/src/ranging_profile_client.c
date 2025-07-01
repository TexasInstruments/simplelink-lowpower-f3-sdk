/******************************************************************************

@file  ranging_profile_client.c

@brief This file contains the ranging requester (RREQ) APIs and functionality.

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

#ifdef RANGING_CLIENT
//*****************************************************************************
//! Includes
//*****************************************************************************
#include <string.h>
#include "ti/ble/app_util/common/util.h"
#include "ti/ble/host/gatt/gatt.h"
#include "ti/ble/app_util/framework/bleapputil_api.h"
#include "ti/ble/app_util/framework/bleapputil_timers.h"
#include "ti/ble/profiles/ranging/ranging_profile_client.h"
#include "ti/ble/profiles/ranging/ranging_db_client.h"
#include "app_gatt_api.h"

//*****************************************************************************
//! Defines
//*****************************************************************************

// The index of characteristic properties handle
#define RREQ_APP_CHAR_PRO_HANDLE_INDEX               0x02
// The index of characteristic value handle
#define RREQ_APP_CHAR_VALUE_HANDLE_INDEX             0x03
// The index of characteristic UUID handle
#define RREQ_APP_CHAR_UUID_HANDLE_INDEX              0x05

// 16-bit high and low index in the 16-bit custom UUID
#define RREQ_APP_LOW_UUID_INDEX                      0x00
#define RREQ_APP_HIGH_UUID_INDEX                     0x01

// CCCD handle offset
#define RREQ_APP_CCCD_OFFSET                         0x01
// CCCD value length
#define RREQ_APP_CCCD_VALUE_LEN                      0x02

// The maximum number of connections for the RAS client
#define RREQ_MAX_CONN                                MAX_NUM_BLE_CONNS

// Notification and Indication properties bit mask
#define RAS_NOTIFICATION_PRO_MASK                    0x10
#define RAS_INDICATION_PRO_MASK                      0x20

// segmentation bit masks
#define RAS_FIRST_SEGMENT_BIT_MASK                   0x01
#define RAS_LAST_SEGMENT_BIT_MASK                    0x02

// Last 6 bits (LSB) of uint8_t
#define RAS_LAST_6_BITS_LSB(x)                       ((x & 0xFc) >> 2)
// First 2 bits (LSB) of uint8_t
#define RAS_FIRST_2_BITS_LSB(x)                      (x & 0x03)

/*********************************************************************
 * TYPEDEFS
 */

// RREQ Subscription types
typedef enum
{
    RREQ_REAL_TIME_BIT      = (uint32_t)BV(0),  // Real-time ranging data subscription
    RREQ_ON_DEMAND_BIT      = (uint32_t)BV(1),  // On-demand ranging data subscription
    RREQ_CONTROL_POINT_BIT  = (uint32_t)BV(2),  // Control point command subscription
    RREQ_DATA_READY_BIT     = (uint32_t)BV(3),  // Data ready event subscription
    RREQ_OVERWRITTEN_BIT    = (uint32_t)BV(4)   // Data overwritten event subscription
}RREQ_SubscribeBitMap_e;

typedef struct
{
    uint16_t startHandle;                           // Start handle of the service
    uint16_t endHandle;                             // End handle of the service
    uint16_t featureCharHandle;                     // Feature characteristic handle
    uint16_t featureCharValue;                      // Feature characteristic value
    uint16_t featureCharProperties;                 // Feature characteristic properties
    uint16_t realTimeRangingDataCharHandle;         // Real time ranging data characteristic handle
    uint16_t realTimeRangingDataCharProperties;     // Real time ranging data characteristic properties
    uint16_t onDemandRangingDataCharHandle;         // On demand ranging data characteristic handle
    uint16_t onDemandRangingDataCharProperties;     // On demand ranging data characteristic properties
    uint16_t controlPointCharHandle;                // Control point characteristic handle
    uint16_t controlPointCharProperties;            // Control point characteristic properties
    uint16_t rangingDataReadyCharHandle;            // Ranging data ready characteristic handle
    uint16_t rangingDataReadyCharProperties;        // Ranging data ready characteristic properties
    uint16_t rangingDataOverwrittenCharHandle;      // Ranging data overwritten characteristic handle
    uint16_t rangingDataOverwrittenCharProperties;  // Ranging data overwritten characteristic properties
    uint8_t  subscribeBitMap;                       // Subscribtion bit map
    RREQEnableModeType_e enableMode;                // Enable mode
    RREQSegmentsMGR_t    segmentMgr;                // Segments manager
} RREQ_ConnInfo_t;

typedef struct
{
    RREQ_ConnInfo_t connInfo[RREQ_MAX_CONN];  // Connection information
    const RREQConfig_t* config;               // Configuration for the RREQ
    const RREQCallbacks_t* callbacks;         // Application callbacks
    RREQProcedureAttr_t procedureAttr;        // Procedure attributes
    BLEAppUtil_timerHandle timeoutHandle;     // Timer handle for the RREQ timeout
} RREQ_ControlBlock_t;

/*********************************************************************
 * CONSTANTS
 */

// Length of the Ranging Counter
#define RANGING_COUNTER_LEN 2

//*****************************************************************************
//! Globals

// RREQ control block
RREQ_ControlBlock_t gRREQControlBlock = {0};

//*****************************************************************************
//!LOCAL FUNCTIONS
//*****************************************************************************
static bStatus_t rreq_discoverAllChars(uint16_t connHandle, uint16_t startHandle, uint16_t endHandle);
static RREQConfigSubType_e rreq_getCharProperties(RREQConfigSubType_e mode, uint8_t properties);
static bStatus_t rreq_discoverPrimServ(uint16_t connHandle);
static void rreq_handleFindByTypeValueRsp(gattMsgEvent_t *gattMsg);
static void rreq_handleReadByTypeRsp(gattMsgEvent_t *gattMsg);
static void rreq_handleValueNoti(uint16_t connHandle, attHandleValueNoti_t *handleValueNoti);
static void rreq_handleWriteRsp(gattMsgEvent_t *gattMsg);
static uint8_t rreq_sendControlPointWriteCmd(uint16_t connHandle, uint8_t cmd, uint16_t rangingCounter, uint8_t len);
static bStatus_t rreq_registerCharacteristics(uint16_t connHandle);
static bStatus_t rreq_enableNotification(uint16_t connHandle, RREQConfigSubType_e mode, uint16_t attHandle);
static void rreq_discoverAllCharDescriptors(uint16_t connHandle);
static void rreq_clearData(uint16_t connHandle);
static void rreq_readCharacteristicValue(uint16_t connHandle, uint16_t handle);
static void rreq_handleControlPointRsp(uint16_t connHandle, attHandleValueNoti_t *handleValueNoti);
static void rreq_handleSegmentReceived(uint16_t connHandle, attHandleValueNoti_t *handleValueNoti);
static void rreq_parseSegmentReceived(uint16_t connHandle, attHandleValueNoti_t *handleValueNoti);
static void rreq_handleRspCode(uint16_t connHandle, uint8_t rspValue);
static void rreq_procedureDone(void);
static uint8_t rreq_CheckDataComplete(uint16_t connHandle);

// Timer functions
static void rreq_startTimer( uint32_t timeout );
static void rreq_stopTimer( void );
static void rreq_timerCB(BLEAppUtil_timerHandle timerHandle, BLEAppUtil_timerTermReason_e reason, void *pData);

//*****************************************************************************
//!LOCAL VARIABLES
//*****************************************************************************

//*****************************************************************************
//!APPLICATION CALLBACK
//*****************************************************************************
static void RREQ_GATTEventHandler(uint32 event, BLEAppUtil_msgHdr_t *pMsgData);

// Events handlers struct, contains the handlers and event masks
// of the application data module
BLEAppUtil_EventHandler_t RREQGATTHandler =
{
    .handlerType    = BLEAPPUTIL_GATT_TYPE,
    .pEventHandler  = RREQ_GATTEventHandler,
    .eventMask      = BLEAPPUTIL_ATT_FIND_BY_TYPE_VALUE_RSP   |
                      BLEAPPUTIL_ATT_READ_BY_TYPE_RSP         |
                      BLEAPPUTIL_ATT_HANDLE_VALUE_IND         |
                      BLEAPPUTIL_ATT_HANDLE_VALUE_NOTI        |
                      BLEAPPUTIL_ATT_WRITE_RSP                |
                      BLEAPPUTIL_ATT_READ_RSP                 |
                      BLEAPPUTIL_ATT_ERROR_RSP                |
                      BLEAPPUTIL_ATT_FIND_INFO_RSP
};

//*****************************************************************************
//! Functions
//*****************************************************************************

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

 /*********************************************************************
 * @fn      RREQ_Start
 *
 * @brief   Initializes the ranging requester by saving the provided
 *          configuration and callbacks.
 *
 * input parameters
 *
 * @param   pCallbacks - Pointer to the callback structure.
 * @param   pConfig - Pointer to the configuration structure.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  SUCCESS - if the initialization was successful.
 *          INVALIDPARAMETER - if the input parameters are invalid.
 *
 */
uint8_t RREQ_Start(const RREQCallbacks_t *pCallbacks , const RREQConfig_t *pConfig)
{
    uint8_t status = SUCCESS;

    // Check input parameters
    if ( pCallbacks == NULL || pConfig == NULL )
    {
        return INVALIDPARAMETER;
    }

    // Save configuration and callbacks
    gRREQControlBlock.config = pConfig;
    gRREQControlBlock.callbacks = pCallbacks;

    // Reset Procedure Attr
    gRREQControlBlock.procedureAttr.connHandle = LINKDB_CONNHANDLE_INVALID;
    gRREQControlBlock.procedureAttr.rangingCounter = 0xFFFF;
    gRREQControlBlock.procedureAttr.procedureState = RREQ_STATE_IDLE;

    // Reset the timer handle
    gRREQControlBlock.timeoutHandle = BLEAPPUTIL_TIMER_INVALID_HANDLE;

    // Register the GATT event handler
    status = BLEAppUtil_registerEventHandler( &RREQGATTHandler );

    if ( status == SUCCESS )
    {
        // Init ranging client DB
        status = RangingDBClient_initDB();
    }

    return status;
}


/*********************************************************************
 * @fn      RREQ_Enable
 *
 * @brief   Enables the RREQ process.
 *          This function start the RREQ process by discovering the RAS (Ranging Service)
 *          service on the specified connection handle
 *
 * input parameters
 *
 * @param   connHandle - Connection handle.
 * @param   enableMode - Mode to enable.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  return SUCCESS or an error status indicating the failure reason.
 */
uint8_t RREQ_Enable(uint16_t connHandle, RREQEnableModeType_e enableMode)
{
    uint8_t status = SUCCESS;

    // Check if the connection handle is valid and the enable mode is supported
    if (connHandle >= RREQ_MAX_CONN || enableMode == RREQ_REAL_TIME)
    {
        status = INVALIDPARAMETER;
    }
    else
    {
        // Check if the RREQ is already enabled for this connection handle
        if (gRREQControlBlock.connInfo[connHandle].enableMode == RREQ_IDLE)
        {
            // Check if need to discover the RAS service
            if(gRREQControlBlock.connInfo[connHandle].endHandle == 0)
            {
                // Discover the RAS service
                status = rreq_discoverPrimServ(connHandle);
                if(status == SUCCESS)
                {
                    // Set the enable mode
                    gRREQControlBlock.connInfo[connHandle].enableMode = enableMode;
                }
            }
            else
            {
                // register to the server characteristics
                status = rreq_registerCharacteristics(connHandle);
            }
        }
        else
        {
            // Already enabled
            status = INVALIDPARAMETER;
        }

        // Open ranging client DB per connection handle
        if(status == SUCCESS)
        {
            status = RangingDBClient_procedureOpen(connHandle);
        }
    }
    return status;
}

/*********************************************************************
 * @fn      RREQ_Disable
 *
 * @brief   Disable the RREQ process.
 *          This function start the RREQ process by discovering the RAS (Ranging Service)
 *          service on the specified connection handle
 *
 * input parameters
 *
 * @param   connHandle - The connection handle for the RAS service.
 *
 * output parameters
 *
 * @param   None
 *
 * @return SUCCESS - if the RREQ process was successfully disabled.
 *         INVALIDPARAMETER - if the connection handle is invalid.
 */
uint8_t RREQ_Disable(uint16_t connHandle)
{
    uint8_t status = INVALIDPARAMETER;

    // Check if the connection handle is valid
    if (connHandle < RREQ_MAX_CONN)
    {
        // Unregister all peer notifications/indications
        if ((gRREQControlBlock.config->subConfig.controlPointSubType != RREQ_DISABLE_NOTIFY_INDICATE) &&
            ((gRREQControlBlock.connInfo[connHandle].subscribeBitMap & RREQ_CONTROL_POINT_BIT) != 0))
        {
            rreq_enableNotification(connHandle, RREQ_DISABLE_NOTIFY_INDICATE, gRREQControlBlock.connInfo[connHandle].controlPointCharHandle);
        }
        if ((gRREQControlBlock.config->subConfig.dataReadySubType != RREQ_DISABLE_NOTIFY_INDICATE) &&
            ((gRREQControlBlock.connInfo[connHandle].subscribeBitMap & RREQ_DATA_READY_BIT) != 0))
        {
            rreq_enableNotification(connHandle, RREQ_DISABLE_NOTIFY_INDICATE, gRREQControlBlock.connInfo[connHandle].rangingDataReadyCharHandle);
        }
        if ((gRREQControlBlock.config->subConfig.overwrittenSubType != RREQ_DISABLE_NOTIFY_INDICATE) &&
            ((gRREQControlBlock.connInfo[connHandle].subscribeBitMap & RREQ_OVERWRITTEN_BIT) != 0))
        {
            rreq_enableNotification(connHandle, RREQ_DISABLE_NOTIFY_INDICATE, gRREQControlBlock.connInfo[connHandle].rangingDataOverwrittenCharHandle);
        }
        if ((gRREQControlBlock.config->subConfig.onDemandSubType != RREQ_DISABLE_NOTIFY_INDICATE) &&
            ((gRREQControlBlock.connInfo[connHandle].subscribeBitMap & RREQ_ON_DEMAND_BIT) != 0))
        {
            rreq_enableNotification(connHandle, RREQ_DISABLE_NOTIFY_INDICATE, gRREQControlBlock.connInfo[connHandle].onDemandRangingDataCharHandle);
        }

        // Clear all connHandle saved data
        rreq_clearData(connHandle);

        status = SUCCESS;
    }

    return status;
}

/*********************************************************************
 * @fn      RREQ_GetRangingData
 *
 * @brief   Starts the process of reading data for a ranging request.
 *          This function initiates the data reading process for a specified connection
 *          handle and ranging count.
 *
 * input parameters
 *
 * @param   connHandle  - Connection handle.
 * @param   RangingCount - CS procedure counter.
 *
 * output parameters
 *
 * @param   None
 *
 * @return return SUCCESS or an error status indicating the failure reason.
 */
uint8_t RREQ_GetRangingData(uint16_t connHandle, uint16_t rangingCount)
{
    uint8_t status = SUCCESS;

    // Check if the procedure state is idle
    if (gRREQControlBlock.procedureAttr.procedureState == RREQ_STATE_IDLE)
    {
        // Check if the connection handle is valid
        // and if the configuration is not NULL
        if ((connHandle >= RREQ_MAX_CONN) || (gRREQControlBlock.config == NULL))
        {
            return INVALIDPARAMETER;
        }

        // Send write command to control point characteristic handle
        status = rreq_sendControlPointWriteCmd(connHandle, RAS_CP_OPCODE_GET_RANGING_DATA, rangingCount, RAS_CP_GET_DATA_CMD_LEN);

        // Init the procedure Attr
        if(status == SUCCESS)
        {
            gRREQControlBlock.procedureAttr.procedureState = RREQ_STATE_WAIT_FOR_FIRST_SEGMENT;
            gRREQControlBlock.procedureAttr.connHandle = connHandle;
            gRREQControlBlock.procedureAttr.rangingCounter = rangingCount;
            // Start the timer for timeout
            rreq_startTimer(gRREQControlBlock.config->timeOutDataReady);
        }
        else
        {
            // Reset the procedure state if the command failed
            gRREQControlBlock.procedureAttr.procedureState = RREQ_STATE_IDLE;
        }
    }
    else
    {
        // The procedure is already in progress
        status = bleIncorrectMode;
    }

    return status;
}

/*********************************************************************
 * @fn      RREQ_Abort
 *
 * @brief   Aborts the ongoing ranging request.
 *          This function is responsible for aborting the ongoing ranging request
 *          for a specified connection handle.
 *
 * input parameters
 *
 * @param   connHandle - Connection handle.
 *
 * output parameters
 *
 * @param   None
 *
 * @return return SUCCESS or an error status indicating the failure reason.
 */
uint8_t RREQ_Abort(uint16_t connHandle)
{
    uint8_t status = bleDisallowed;

    // Check if the procedure is in progress
    if (gRREQControlBlock.procedureAttr.procedureState != RREQ_STATE_IDLE &&
        gRREQControlBlock.procedureAttr.connHandle == connHandle)
    {
        // Check if the connection handle is valid
        if (connHandle >= RREQ_MAX_CONN)
        {
            return INVALIDPARAMETER;
        }

        // Check if the abort operation is allowed
        if (gRREQControlBlock.connInfo[gRREQControlBlock.procedureAttr.connHandle].featureCharValue & RAS_FEATURES_ABORT_OPERATION )
        {
            // Send Abort command to the server
            status = rreq_sendControlPointWriteCmd(gRREQControlBlock.procedureAttr.connHandle, RAS_CP_OPCODE_ABORT_OPERATION,
                                          gRREQControlBlock.procedureAttr.rangingCounter, RAS_CP_ABORT_CMD_LEN);
            if( status == SUCCESS)
            {
                // Reset the procedure state to wait for abort response
                gRREQControlBlock.procedureAttr.procedureState = RREQ_STATE_WAIT_FOR_ABORT;
            }
        }
    }

    return status;
}

 /*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      RREQ_GATTEventHandler
 *
 * @brief   The purpose of this function is to handle GATT events
 *          that rise from the GATT and were registered in
 *          @ref BLEAppUtil_RegisterGAPEvent
 *
 * input parameters
 *
 * @param   event - message event.
 * @param   pMsgData - pointer to message data.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  none
 */
static void RREQ_GATTEventHandler(uint32 event, BLEAppUtil_msgHdr_t *pMsgData)
{
    gattMsgEvent_t *gattMsg = ( gattMsgEvent_t * )pMsgData;

    // Check if the connection handle valid
    if ( gattMsg->connHandle >= RREQ_MAX_CONN )
    {
        return;
    }
    switch ( event )
    {
        case BLEAPPUTIL_ATT_FIND_BY_TYPE_VALUE_RSP:
        {
            rreq_handleFindByTypeValueRsp( gattMsg );
            break;
        }

        case BLEAPPUTIL_ATT_READ_BY_TYPE_RSP:
        {
            rreq_handleReadByTypeRsp( gattMsg );
            break;
        }

        case BLEAPPUTIL_ATT_HANDLE_VALUE_NOTI:
        {
            // Handle the notification
            rreq_handleValueNoti( gattMsg->connHandle, &(gattMsg->msg.handleValueNoti) );
            break;
        }

        case BLEAPPUTIL_ATT_HANDLE_VALUE_IND:
        {
            // Send an indication confirmation
            ATT_HandleValueCfm(gattMsg->connHandle);

            // Handle the indication
            attHandleValueNoti_t handleValueNoti =
            {
             .handle = gattMsg->msg.handleValueInd.handle,
             .len    = gattMsg->msg.handleValueInd.len,
             .pValue = gattMsg->msg.handleValueInd.pValue
            };
            rreq_handleValueNoti( gattMsg->connHandle, &handleValueNoti );
            break;
        }

        case BLEAPPUTIL_ATT_WRITE_RSP:
        {
            rreq_handleWriteRsp( gattMsg );
            break;
        }

        case BLEAPPUTIL_ATT_ERROR_RSP:
        {
          // TODO: implement ATT ERROR handling
            break;
        }

        case BLEAPPUTIL_ATT_READ_RSP:
        {
            // Save RAS feature characteristic value
            memcpy(&gRREQControlBlock.connInfo[gattMsg->connHandle].featureCharValue, gattMsg->msg.readRsp.pValue, sizeof(uint32_t));

            // Register to RAS server characteristics
            if(gattMsg->hdr.status == SUCCESS)
            {
                // TODO: check if its the correct handle
                // Register to the server characteristics
                rreq_registerCharacteristics(gattMsg->connHandle);
            }
            break;
        }

        case BLEAPPUTIL_ATT_FIND_INFO_RSP:
        {
            // TODO: parse the find info response to find handles ccc.
            if( gattMsg->hdr.status == bleProcedureComplete )
            {
                // read from "read feature" characteristic
                rreq_readCharacteristicValue(gattMsg->connHandle, gRREQControlBlock.connInfo[gattMsg->connHandle].featureCharHandle);
            }
        }

        default:
            break;
    }
}

/*********************************************************************
 * @fn      rreq_discoverAllChars
 *
 * @brief   Car Access client discover all service characteristics,
 *          ATT_READ_BY_TYPE_RSP
 *
 * input parameters
 *
 * @param   connHandle - connection message was received on
 *
 * output parameters
 *
 * @param   None
 *
 * @return  SUCCESS or stack call status
 */
static bStatus_t rreq_discoverAllChars(uint16_t connHandle, uint16_t startHandle, uint16_t endHandle)
{
    bStatus_t status;

    // Discovery simple service
    status = GATT_DiscAllChars(connHandle, startHandle, endHandle, BLEAppUtil_getSelfEntity());

    return status;
}

/*********************************************************************
 * @fn      rreq_readCharacteristicValue
 *
 * @brief   Reads the value of a specified characteristic from a remote device.
 *          This function initiates a read request for the value of a characteristic
 *          identified by its handle on a remote device. The result of the read operation
 *          will be provided asynchronously through event ATT_READ_RSP.
 *
 * input parameters
 *
 * @param   connHandle  - Connection handle.
 * @param   handle - Handle of the characteristic to read.
 *
 * output parameters
 *
 * @param   None
 *
 * @return Status code indicating the success or failure of the operation.
 */
static void rreq_readCharacteristicValue(uint16_t connHandle, uint16_t handle)
{
    attReadReq_t req;
    req.handle = handle;

    // Read the characteristic value
    GATT_ReadCharValue(connHandle, &req, BLEAppUtil_getSelfEntity());
}

/*********************************************************************
 * @fn      rreq_handleWriteRsp
 *
 * @brief   The purpose of this function is to handle
 *          BLEAPPUTIL_ATT_WRITE_RSP events
 *          that rise from the GATT.
 *
 * input parameters
 *
 * @param   gattMsg - pointer to the GATT message event structure
 *
 * output parameters
 *
 * @param   None
 *
 * @return  none
 */
static void rreq_handleWriteRsp(gattMsgEvent_t *gattMsg)
{
    if(gattMsg->hdr.status == SUCCESS)
    {
        // register to the server characteristics
        rreq_registerCharacteristics(gattMsg->connHandle);
    }
}

/*********************************************************************
 * @fn      rreq_registerCharacteristics
 *
 * @brief Registers characteristics for the RREQ.
 * This function is responsible for registering the characteristics
 * that the RREQ will use to communicate with the server.
 * It ensures that the necessary characteristics
 * are properly initialized and made available for use.
 *
 * @param connHandle The connection handle for the RREQ.
 * @return None.
 */
static bStatus_t rreq_registerCharacteristics(uint16_t connHandle)
{
    bStatus_t status = SUCCESS;
    // The supported subscription types
    RREQConfigSubType_e supSubType;

    // Check if the connection handle is valid
    // and if the configuration is not NULL
    if ((connHandle >= RREQ_MAX_CONN) || (gRREQControlBlock.config == NULL))
    {
        return bleInvalidRange;
    }

    // Register to the "control point" characteristic handle
    if( (gRREQControlBlock.config->subConfig.controlPointSubType != RREQ_DISABLE_NOTIFY_INDICATE) &&
        ((gRREQControlBlock.connInfo[connHandle].subscribeBitMap & RREQ_CONTROL_POINT_BIT) == 0))
    {
        // Get the characteristic properties mode
        supSubType = rreq_getCharProperties(gRREQControlBlock.config->subConfig.controlPointSubType, gRREQControlBlock.connInfo[connHandle].controlPointCharProperties);
        // Enable notifications/indications for the characteristic
        rreq_enableNotification(connHandle, supSubType, gRREQControlBlock.connInfo[connHandle].controlPointCharHandle);
        gRREQControlBlock.connInfo[connHandle].subscribeBitMap |= RREQ_CONTROL_POINT_BIT;
    }

    // Register to the "data ready" characteristic handle
    else if((gRREQControlBlock.config->subConfig.dataReadySubType != RREQ_DISABLE_NOTIFY_INDICATE) &&
            ((gRREQControlBlock.connInfo[connHandle].subscribeBitMap & RREQ_DATA_READY_BIT) == 0))
    {
        // Get the characteristic properties mode
        supSubType = rreq_getCharProperties(gRREQControlBlock.config->subConfig.dataReadySubType, gRREQControlBlock.connInfo[connHandle].rangingDataReadyCharProperties);
        // Enable notifications/indications for the characteristic
        rreq_enableNotification(connHandle, supSubType, gRREQControlBlock.connInfo[connHandle].rangingDataReadyCharHandle);
        gRREQControlBlock.connInfo[connHandle].subscribeBitMap |= RREQ_DATA_READY_BIT;
    }

    // Register to the "data overwritten" characteristic handle
    else if((gRREQControlBlock.config->subConfig.overwrittenSubType != RREQ_DISABLE_NOTIFY_INDICATE) &&
            ((gRREQControlBlock.connInfo[connHandle].subscribeBitMap & RREQ_OVERWRITTEN_BIT) == 0))
    {
        // Get the characteristic properties mode
        supSubType = rreq_getCharProperties(gRREQControlBlock.config->subConfig.overwrittenSubType, gRREQControlBlock.connInfo[connHandle].rangingDataOverwrittenCharProperties);
        // Enable notifications/indications for the characteristic
        rreq_enableNotification(connHandle, supSubType, gRREQControlBlock.connInfo[connHandle].rangingDataOverwrittenCharHandle);
        gRREQControlBlock.connInfo[connHandle].subscribeBitMap |= RREQ_OVERWRITTEN_BIT;
    }

    // Register to the "on-demand" characteristic handle
    else if ((gRREQControlBlock.config->subConfig.onDemandSubType != RREQ_DISABLE_NOTIFY_INDICATE) &&
             ((gRREQControlBlock.connInfo[connHandle].subscribeBitMap & RREQ_ON_DEMAND_BIT) == 0))
    {
        // Get the characteristic properties mode
        supSubType = rreq_getCharProperties(gRREQControlBlock.config->subConfig.onDemandSubType, gRREQControlBlock.connInfo[connHandle].onDemandRangingDataCharProperties);
        // Enable notifications/indications for the characteristic
        rreq_enableNotification(connHandle, supSubType, gRREQControlBlock.connInfo[connHandle].onDemandRangingDataCharHandle);
        gRREQControlBlock.connInfo[connHandle].subscribeBitMap |= RREQ_ON_DEMAND_BIT;
    }

    return status;
}

/*********************************************************************
 * @fn      rreq_handleValueNoti
 *
 * @brief Handles the GATT "BLEAPPUTIL_ATT_HANDLE_VALUE_NOTI".
 * This function processes the response received from the GATT server
 * after getting "BLEAPPUTIL_ATT_HANDLE_VALUE_NOTI".
 * It is responsible for handling the data returned in the response
 * and performing any necessary actions based on the received information.
 *
 * @param gattMsg Pointer to the GATT message event structure containing
 *                the response data.
 */
static void rreq_handleValueNoti(uint16_t connHandle, attHandleValueNoti_t *handleValueNoti)
{
    // Check if the pointer and connection handle are valid
    if(handleValueNoti == NULL || connHandle >= RREQ_MAX_CONN)
    {
        return;
    }

    // if get notification from "on-demand" characteristic handle
    if(handleValueNoti->handle == gRREQControlBlock.connInfo[connHandle].onDemandRangingDataCharHandle )
    {
        rreq_handleSegmentReceived(connHandle, handleValueNoti);
    }

    // if get notification from "control point" characteristic handle
    else if(handleValueNoti->handle == gRREQControlBlock.connInfo[connHandle].controlPointCharHandle )
    {
        rreq_handleControlPointRsp(connHandle, handleValueNoti);
    }

    // if get notification from "data ready" characteristic handle
    else if(handleValueNoti->handle == gRREQControlBlock.connInfo[connHandle].rangingDataReadyCharHandle )
    {
        // check the notification length
        if(handleValueNoti->len >= 2)
        {
            // Get from the notification the ranging counter
            uint8_t *pData = handleValueNoti->pValue;
            uint16_t rangingCounter = BUILD_UINT16(pData[0], pData[1]);

            if((gRREQControlBlock.callbacks != NULL) && (gRREQControlBlock.callbacks->pDataReadyCallback != NULL))
            {
                // Call pDataReadyCallback function
                gRREQControlBlock.callbacks->pDataReadyCallback(connHandle, rangingCounter);
            }
        }
    }

    // if get notification from "data overwritten" characteristic handle
    else if((gRREQControlBlock.callbacks != NULL) &&
            (gRREQControlBlock.callbacks->pStatusCallback != NULL) &&
            (handleValueNoti->handle == gRREQControlBlock.connInfo[connHandle].rangingDataOverwrittenCharHandle))
    {
        // send OverWritten status to App
        uint16_t rangingCounter = BUILD_UINT16(handleValueNoti->pValue[0], handleValueNoti->pValue[1]);
        gRREQControlBlock.callbacks->pStatusCallback(connHandle, RREQ_DATA_OVERWRITTEN, RANGING_COUNTER_LEN, (uint8_t*)&rangingCounter);
    }
}
uint8_t count = 0;
/*********************************************************************
 * @fn    rreq_handleControlPointRsp
 *
 * @brief Handles the control point indications
 *
 * @param connHandle The connection handle of the ranging profile.
 * @param handleValueNoti Pointer to the handle value indication structure.
 *
 * @return None.
 */
void rreq_handleControlPointRsp(uint16_t connHandle, attHandleValueNoti_t *handleValueNoti)
{
    // Check if the notification length is valid
    if(handleValueNoti->len <= RAS_CP_RSP_MAX_LEN)
    {
        // Get the command from the notification
        uint8_t RspOpCode = handleValueNoti->pValue[0];

        // Handle the command based on its type
        switch(RspOpCode)
        {
            case RAS_CP_OPCODE_COMPLETE_DATA_RSP:
            {
                // RangingCounter received from CompleteData msg
                uint16_t CompleteDataRangingCounter = BUILD_UINT16(handleValueNoti->pValue[1], handleValueNoti->pValue[2]);

                // check that the received rangingCounter is the expected one.
                if (gRREQControlBlock.procedureAttr.rangingCounter == CompleteDataRangingCounter)
                {
                    // wait for the response code
                    gRREQControlBlock.procedureAttr.procedureState = RREQ_STATE_WAIT_FOR_CONTROL_POINT_RSP;

                    // Stop the timer
                    rreq_stopTimer();

                    // Send Ack to the server anyway due to the fact that there is no support in Get Lost Segments
                    rreq_sendControlPointWriteCmd(connHandle, RAS_CP_OPCODE_ACK_RANGING_DATA, CompleteDataRangingCounter,
                                                RAS_CP_ACK_DATA_CMD_LEN);
                }
                break;
            }
            case RAS_CP_OPCODE_RSP_CODE:
            {
                // Notify App
                uint8_t rspValue = handleValueNoti->pValue[1];
                rreq_handleRspCode(connHandle, rspValue);
                break;
            }
            default:
                // Unknown command, do nothing or log an error
                break;
        }
    }
}

/*********************************************************************
 * @fn    rreq_procedureDone
 *
 * @brief This function handles the completion of the RREQ procedure.
 *        It checks if the data is complete using a bitmask,
 *        notifies the application about the completion status,
 *        and resets the procedure attributes.
 *
 * @param  None.
 *
 * @return None.
 *
 */
static void rreq_procedureDone(void)
{
    uint8_t* data = NULL;
    uint16_t datalen = 0;
    // Notify APP ( Data Complete )
    if((gRREQControlBlock.callbacks != NULL) || (gRREQControlBlock.callbacks->pDataCompleteEventCallback != NULL))
    {
        // Check if the data is complete using the bitmask
        uint8_t status = rreq_CheckDataComplete(gRREQControlBlock.procedureAttr.connHandle);

        if(status == SUCCESS)
        {
            // Get the data from the database
            data = RangingDBClient_getData(gRREQControlBlock.procedureAttr.connHandle);
            datalen = gRREQControlBlock.connInfo[gRREQControlBlock.procedureAttr.connHandle].segmentMgr.totalDataLen;
        }

        // Call the data complete callback function
        gRREQControlBlock.callbacks->pDataCompleteEventCallback(gRREQControlBlock.procedureAttr.connHandle,
                                                    gRREQControlBlock.procedureAttr.rangingCounter,
                                                    status,
                                                    datalen,
                                                    data);
    }

    // Reset Procedure Attributes
    gRREQControlBlock.procedureAttr.connHandle = LINKDB_CONNHANDLE_INVALID;
    gRREQControlBlock.procedureAttr.rangingCounter = 0xFFFF;
    gRREQControlBlock.procedureAttr.procedureState = RREQ_STATE_IDLE;
}
/*********************************************************************
 * @fn      rreq_handleRspCode
 *
 * @brief   Handles the response code received from the server.
 *          This function processes the response code and calls the
 *          appropriate callback function based on the response value.
 *
 * @param   connHandle - The connection handle for the RAS client.
 * @param   rspValue - The response value received from the server.
 *
 * @return  None.
 */
static void rreq_handleRspCode(uint16_t connHandle, uint8_t rspValue)
{
    // Check if the callback is set
    if((gRREQControlBlock.callbacks != NULL) && (gRREQControlBlock.callbacks->pStatusCallback != NULL))
    {
        // Handle the response code based on its value
        switch(rspValue)
        {
            case RAS_CP_RSP_CODE_VAL_SUCCESS:
            {
                if(gRREQControlBlock.procedureAttr.procedureState == RREQ_STATE_WAIT_FOR_ABORT)
                {
                    // Procedure aborted successfully
                    gRREQControlBlock.callbacks->pStatusCallback(connHandle, RREQ_ABORTED_SUCCESSFULLY, RANGING_COUNTER_LEN, (uint8_t*)&gRREQControlBlock.procedureAttr.rangingCounter);
                    gRREQControlBlock.procedureAttr.procedureState = RREQ_STATE_IDLE;
                }
                else if(gRREQControlBlock.procedureAttr.procedureState == RREQ_STATE_WAIT_FOR_CONTROL_POINT_RSP)
                {
                    // handle the procedure completion
                    rreq_procedureDone();
                }
                break;
            }
            case RAS_CP_RSP_CODE_ABORT_UNSUCCESSFUL:
            {
                if(gRREQControlBlock.procedureAttr.procedureState == RREQ_STATE_WAIT_FOR_ABORT)
                {
                    // Reset the procedure state to idle
                    gRREQControlBlock.procedureAttr.procedureState = RREQ_STATE_IDLE;
                }
                gRREQControlBlock.callbacks->pStatusCallback(connHandle, RREQ_ABORTED_UNSUCCESSFULLY, RANGING_COUNTER_LEN, (uint8_t*)&gRREQControlBlock.procedureAttr.rangingCounter);

                break;
            }
            case RAS_CP_RSP_CODE_SERVER_BUSY:
            {
                // Procedure not completed
                gRREQControlBlock.callbacks->pStatusCallback(connHandle, RREQ_SERVER_BUSY , RANGING_COUNTER_LEN, (uint8_t*)&gRREQControlBlock.procedureAttr.rangingCounter);
                // Reset the procedure state to idle
                gRREQControlBlock.procedureAttr.procedureState = RREQ_STATE_IDLE;
                // Stop the timer
                rreq_stopTimer();
                break;
            }
            case RAS_CP_RSP_CODE_NO_RECORDS_FOUND:
            {
                // No records found
                gRREQControlBlock.callbacks->pStatusCallback(connHandle, RREQ_NO_RECORDS, RANGING_COUNTER_LEN, (uint8_t*)&gRREQControlBlock.procedureAttr.rangingCounter);
                // Reset the procedure state to idle
                gRREQControlBlock.procedureAttr.procedureState = RREQ_STATE_IDLE;
                // Stop the timer
                rreq_stopTimer();
                break;
            }
            case RAS_CP_RSP_CODE_PROCEDURE_NOT_COMP:
            {
                gRREQControlBlock.callbacks->pStatusCallback(connHandle, RREQ_PROCEDURE_NOT_COMPLETED, RANGING_COUNTER_LEN, (uint8_t*)&gRREQControlBlock.procedureAttr.rangingCounter);
                // Reset the procedure state to idle
                gRREQControlBlock.procedureAttr.procedureState = RREQ_STATE_IDLE;
                // Stop the timer
                rreq_stopTimer();
                break;
            }
        }
    }
}
/*********************************************************************
 * @fn     rreq_CheckDataComplete
 *
 * @brief Checks if the data is complete and valid.
 * This function verifies whether the required data has been fully received
 * and meets the necessary conditions for further processing. It ensures
 * that no critical data is missing or corrupted.
 *
 * @return Returns a boolean value:
 *         - true: If the data is complete and valid.
 *         - false: If the data is incomplete or invalid.
 */
static uint8_t rreq_CheckDataComplete(uint16_t connHandle)
{
    uint8_t status = RREQ_DATA_INVALID;
    // Check if all segments are received
    if (gRREQControlBlock.connInfo[connHandle].segmentMgr.lastSegmentFlag == TRUE)
    {
        // Create a mask with the required number of bits set to 1
        uint64_t requiredMask = 1;
        requiredMask = (requiredMask << (gRREQControlBlock.connInfo[connHandle].segmentMgr.lastSegmentValue + 1)) - 1;
        // Check if all required bits are set in the bit mask
        if ((gRREQControlBlock.connInfo[connHandle].segmentMgr.bitMask & requiredMask) == requiredMask)
        {
            // All required bits are set
            status = SUCCESS;
        }
    }
    return status;
}

/*********************************************************************
 * @fn      rreq_parseSegmentReceived
 *
 * @brief Handles the "on-demand" notification.
 *
 * @param connHandle - The connection handle for the RAS client.
 * @param handleValueNoti - Pointer to the handle value notification structure.
 *
 * @return None.
 */
static void rreq_parseSegmentReceived(uint16_t connHandle, attHandleValueNoti_t *handleValueNoti)
{
    // split data to segment (1 byte) and the rest of the data
    uint8_t *pData = handleValueNoti->pValue;
    uint8_t segmentNum = RAS_LAST_6_BITS_LSB(pData[0]); // Extract the last 6 bits (LSB)
    uint8_t lastSegmentFlag = RAS_FIRST_2_BITS_LSB(pData[0]); // Extract the first 2 bits (LSB)
    uint16_t dataLen = handleValueNoti->len - 1; // Exclude the segment byte
    uint64_t segmentBitMask = 1;// Bit mask for the segment

    // Check if the segment is the first one
    if( (lastSegmentFlag & RAS_FIRST_SEGMENT_BIT_MASK) != 0 )
    {
        // Clear the procedure DB
        RangingDBClient_clearProcedure(connHandle);
        // Clear the segment manager
        memset(&gRREQControlBlock.connInfo[connHandle].segmentMgr, 0, sizeof(RREQSegmentsMGR_t));
        gRREQControlBlock.connInfo[connHandle].segmentMgr.segmentSize = dataLen;
    }

    // Check if the segment is the last one
    if( (lastSegmentFlag & RAS_LAST_SEGMENT_BIT_MASK) != 0 )
    {
        gRREQControlBlock.connInfo[connHandle].segmentMgr.lastSegmentFlag = TRUE;
        gRREQControlBlock.connInfo[connHandle].segmentMgr.lastSegmentValue = segmentNum;
        gRREQControlBlock.connInfo[connHandle].segmentMgr.totalDataLen = ( (gRREQControlBlock.connInfo[connHandle].segmentMgr.segmentSize * segmentNum) + dataLen );
    }
    // Add segment to the segment manager bit mask
    gRREQControlBlock.connInfo[connHandle].segmentMgr.bitMask |= (segmentBitMask << segmentNum);

    // Add the data to the procedure DB
    RangingDBClient_addData(connHandle, (gRREQControlBlock.connInfo[connHandle].segmentMgr.segmentSize * segmentNum), dataLen, &pData[1]);
}

/*********************************************************************
 * @fn      rreq_sendControlPointWriteCmd
 *
 * @brief Sends a control point write command to the RAS client.
 * This function is responsible for transmitting a write command to the
 * control point of the RAS client.
 *
 * @param connHandle The connection handle for the RAS client.
 * @param cmd The command to be sent to the control point.
 * @param rangingCounter The ranging counter value to be included in the command.
 * @param len The length of the command data to be sent.
 *
 * @return return SUCCESS or an error status indicating the failure reason.
 */
static uint8_t rreq_sendControlPointWriteCmd(uint16_t connHandle, uint8_t cmd, uint16_t rangingCounter, uint8_t len)
{
    bStatus_t status = SUCCESS;
    attWriteReq_t req;
    uint8 dataValue[RAS_CP_COMMANDS_MAX_LEN] = {0};

    // Check if the connHandle and command length are valid
    if ((connHandle >= RREQ_MAX_CONN) ||
        (len < RAS_CP_COMMANDS_MIN_LEN || len > RAS_CP_COMMANDS_MAX_LEN))
    {
        status = bleInvalidRange;
    }
    else
    {
        // Set the command to be sent
        dataValue[0] = cmd;
        dataValue[1] = LO_UINT16(rangingCounter);
        dataValue[2] = HI_UINT16(rangingCounter);

        // Allocate buffer for the write request
        req.pValue = GATT_bm_alloc(connHandle, ATT_WRITE_REQ, len, NULL);

        // Send the write request for indications enable/diable
        if (req.pValue != NULL)
        {
            req.handle = gRREQControlBlock.connInfo[connHandle].controlPointCharHandle;
            req.len = len;
            memcpy(req.pValue, dataValue, len);
            req.cmd = TRUE;
            req.sig = FALSE;
            status = GATT_WriteNoRsp(connHandle, &req);
            // If the write request failed, free the buffer
            if ( status != SUCCESS )
            {
                GATT_bm_free((gattMsg_t *)&req, ATT_WRITE_REQ);
            }
        }
    }

    return status;
}

/*********************************************************************
 * @fn      rreq_handleFindByTypeValueRsp
 *
 * @brief Handles the GATT "BLEAPPUTIL_ATT_FIND_BY_TYPE_VALUE_RSP".
 * This function processes the response received from the GATT server
 * after getting "BLEAPPUTIL_ATT_FIND_BY_TYPE_VALUE_RSP".
 * It is responsible for handling the data returned in the response
 * and performing any necessary actions based on the received information.
 *
 * @param gattMsg Pointer to the GATT message event structure containing
 *                the response data.
 */
static void rreq_handleFindByTypeValueRsp(gattMsgEvent_t *gattMsg)
{
    if( gattMsg->hdr.status == SUCCESS )
    {
        // Save the start and end handles of the RAS service
        gRREQControlBlock.connInfo[gattMsg->connHandle].startHandle = ATT_ATTR_HANDLE(gattMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
        gRREQControlBlock.connInfo[gattMsg->connHandle].endHandle = ATT_GRP_END_HANDLE(gattMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
    }

    else if( gattMsg->hdr.status == bleProcedureComplete )
    {
        // Find all characteristics within the Car Access service
        rreq_discoverAllChars(gattMsg->connHandle, gRREQControlBlock.connInfo[gattMsg->connHandle].startHandle, gRREQControlBlock.connInfo[gattMsg->connHandle].endHandle);
    }
}

/*********************************************************************
 * @fn      rreq_handleReadByTypeRsp
 *
 * @brief Handles the GATT "BLEAPPUTIL_ATT_READ_BY_TYPE_RSP".
 * This function processes the response received from the GATT server
 * after getting "BLEAPPUTIL_ATT_READ_BY_TYPE_RSP".
 * It is responsible for handling the data returned in the response
 * and performing any necessary actions based on the received information.
 *
 * @param gattMsg Pointer to the GATT message event structure containing
 *                the response data.
 */
static void rreq_handleReadByTypeRsp(gattMsgEvent_t *gattMsg)
{
    attReadByTypeRsp_t att = gattMsg->msg.readByTypeRsp;

    if ( ( gattMsg->hdr.status == SUCCESS ) && ( att.numPairs > 0 ) )
    {
        for (uint8_t i = 0; i < att.numPairs; i++)
        {
            // The format of the data in att.pDataList for each characteristic is:
            // Properties handle: 2 bytes
            // Properties value: 1 byte
            // Char value handle: 2 bytes
            // Char value UUID: 2 bytes
            // Since the handle and value of the properties are not used in this case,
            // they are skipped for each characteristic
            uint16_t charUUID = 0;
            uint8_t customUUID[ATT_BT_UUID_SIZE];
            uint8_t index = i * att.len;

            // characteristic properties
            uint8_t CharPro = att.pDataList[index + RREQ_APP_CHAR_PRO_HANDLE_INDEX];
            // ATT handle
            uint16_t currAttHandle = BUILD_UINT16(att.pDataList[index + RREQ_APP_CHAR_VALUE_HANDLE_INDEX],
                                                  att.pDataList[index + RREQ_APP_CHAR_VALUE_HANDLE_INDEX + 1]);

            // Copy the full 16-bit custom UUID
            memcpy(customUUID, &att.pDataList[index + RREQ_APP_CHAR_UUID_HANDLE_INDEX], ATT_BT_UUID_SIZE);

            // Build the 16-bit UUID of the characteristic
            charUUID = BUILD_UINT16(customUUID[RREQ_APP_LOW_UUID_INDEX],
                                    customUUID[RREQ_APP_HIGH_UUID_INDEX]);

            // Check if this is the Feature Characteristic UUID
            if (charUUID == RAS_FEATURE_UUID)
            {
                gRREQControlBlock.connInfo[gattMsg->connHandle].featureCharHandle = currAttHandle;
                gRREQControlBlock.connInfo[gattMsg->connHandle].featureCharProperties = CharPro;
            }

            // Check if this is the Real-Time Ranging Data Characteristic UUID
            if (charUUID == RAS_REAL_TIME_UUID)
            {
                gRREQControlBlock.connInfo[gattMsg->connHandle].realTimeRangingDataCharHandle = currAttHandle;
                gRREQControlBlock.connInfo[gattMsg->connHandle].realTimeRangingDataCharProperties = CharPro;
            }

            // Check if this is the On-Demand Ranging Data Characteristic UUID
            if (charUUID == RAS_ON_DEMAND_UUID)
            {
                gRREQControlBlock.connInfo[gattMsg->connHandle].onDemandRangingDataCharHandle = currAttHandle;
                gRREQControlBlock.connInfo[gattMsg->connHandle].onDemandRangingDataCharProperties = CharPro;
            }

            // Check if this is the Control Point Characteristic UUID
            if (charUUID == RAS_CONTROL_POINT_UUID)
            {
                gRREQControlBlock.connInfo[gattMsg->connHandle].controlPointCharHandle = currAttHandle;
                gRREQControlBlock.connInfo[gattMsg->connHandle].controlPointCharProperties = CharPro;
            }

            // Check if this is the Ranging Data Ready Characteristic UUID
            if (charUUID == RAS_DATA_READY_UUID)
            {
                gRREQControlBlock.connInfo[gattMsg->connHandle].rangingDataReadyCharHandle = currAttHandle;
                gRREQControlBlock.connInfo[gattMsg->connHandle].rangingDataReadyCharProperties = CharPro;
            }

            // Check if this is the Ranging Data Overwritten Characteristic UUID
            if (charUUID == RAS_DATA_OVERWRITTEN_UUID)
            {
                gRREQControlBlock.connInfo[gattMsg->connHandle].rangingDataOverwrittenCharHandle = currAttHandle;
                gRREQControlBlock.connInfo[gattMsg->connHandle].rangingDataOverwrittenCharProperties = CharPro;
            }
        }
    }
    else if( gattMsg->hdr.status == bleProcedureComplete )
    {
        // discover all characteristic Descriptors
        rreq_discoverAllCharDescriptors(gattMsg->connHandle);
    }
}

/*********************************************************************
 * @fn      rreq_discoverAllCharDescriptors
 *
 * @brief Discovers all characteristic descriptors for the RAS client.
 * This function is responsible for discovering all characteristic
 * descriptors associated with the RAS client.
 * It ensures that the necessary descriptors are properly initialized
 * and made available for use.
 *
 * @param connHandle The connection handle for the RAS client.
 * @return None.
 */
static void rreq_discoverAllCharDescriptors(uint16_t connHandle)
{
    bStatus_t status;

    // Discover all characteristic descriptors
    status = GATT_DiscAllCharDescs(connHandle, gRREQControlBlock.connInfo[connHandle].startHandle, gRREQControlBlock.connInfo[connHandle].endHandle, BLEAppUtil_getSelfEntity());

    if (status != SUCCESS)
    {
        // Handle the error if needed
    }
}

/*********************************************************************
 * @fn      rreq_discoverPrimServ
 *
 * @brief   RAS client discover primary service by UUID,
 *          ATT_FIND_BY_TYPE_VALUE_RSP
 *
 * @param   connHandle - connection message was received on
 *
 * @return  SUCCESS or stack call status
 */
static bStatus_t rreq_discoverPrimServ(uint16_t connHandle)
{
    uint8_t status;

    // Discovery Car Access service
    GATT_BT_UUID(rasUUID, RANGING_SERVICE_UUID);
    status = GATT_DiscPrimaryServiceByUUID(connHandle, rasUUID, ATT_BT_UUID_SIZE, BLEAppUtil_getSelfEntity());

    return status;
}

/*********************************************************************
 * @fn      rreq_getCharProperties
 *
 * @brief Retrieves the characteristic properties for a given connection handle and mode.
 * This function is used to obtain the characteristic properties associated with a specific
 * connection handle and request mode. It processes the provided properties and returns
 * the corresponding mode type.
 *
 * @param mode The request mode type to be used for retrieving the properties.
 * @param properties The characteristic properties to be processed.
 *
 * @return The mode type corresponding to the processed properties.
 */
static RREQConfigSubType_e rreq_getCharProperties(RREQConfigSubType_e mode, uint8_t properties)
{
    // set default value to indicatation
    RREQConfigSubType_e charPro = RREQ_INDICATE;

    // check if the characteristic supports notification and the desired mode is notification
    if( ((properties & RAS_NOTIFICATION_PRO_MASK) != 0) && (mode == RREQ_PREFER_NOTIFY) )
    {
        charPro = RREQ_PREFER_NOTIFY;
    }
    return charPro;
}

/*********************************************************************
 * @fn      rreq_enableNotification
 *
 * @brief Enables notifications for a specific attribute on a remote device.
 * This function enables notifications for a given attribute handle on a remote device
 * over a specified connection. It allows the application to receive updates from the
 * remote device when the value of the attribute changes.
 *
 * @param connHandle The connection handle identifying the link to the remote device.
 * @param mode The request mode type, specifying how the notification request should be handled.
 * @param attHandle The attribute handle for which notifications are to be enabled.
 *
 * @return A status code of type bStatus_t indicating the success or failure of the operation.
 *         Possible values include:
 *         - SUCCESS: The notification was successfully enabled.
 *         - FAILURE: The operation failed due to an error.
 */
static bStatus_t rreq_enableNotification(uint16_t connHandle, RREQConfigSubType_e mode, uint16_t attHandle)
{
    bStatus_t status = SUCCESS;
    attWriteReq_t req;
    // Set the default value to disable notification
    uint8_t dataValue[RREQ_APP_CCCD_VALUE_LEN] = {0};


    if (mode == RREQ_PREFER_NOTIFY)
    {
        // Set the value to enable notification
        dataValue[0] = LO_UINT16(GATT_CLIENT_CFG_NOTIFY);
    }
    else if(mode == RREQ_INDICATE)
    {
        // Set the value to enable indication
        dataValue[0] = LO_UINT16(GATT_CLIENT_CFG_INDICATE);
    }
    else // mode == RREQ_DISABLE_NOTIFY_INDICATE
    {
        // Set the value to disable notification/indication
        dataValue[0] = 0;
    }

    if ( attHandle != 0 )
    {
        // Allocate buffer for the write request
        req.pValue = GATT_bm_alloc(connHandle, ATT_WRITE_REQ, RREQ_APP_CCCD_VALUE_LEN, NULL);

        // Send the write request for Notifications enable/diable
        if (req.pValue != NULL)
        {
            req.handle = attHandle + RREQ_APP_CCCD_OFFSET;
            req.len = RREQ_APP_CCCD_VALUE_LEN;
            memcpy(req.pValue, dataValue, RREQ_APP_CCCD_VALUE_LEN);
            req.cmd = FALSE;
            req.sig = FALSE;
            status = GATT_WriteCharValue(connHandle, &req, BLEAppUtil_getSelfEntity());
            // If the write request failed, free the buffer
            if ( status != SUCCESS )
            {
                GATT_bm_free((gattMsg_t *)&req, ATT_WRITE_REQ);
            }
        }
        else
        {
            status = bleMemAllocError;
        }
    }
    else
    {
        status = bleIncorrectMode;
    }

    return status;
}

/*********************************************************************
 * @fn      rreq_clearData
 *
 * @brief Clears all data associated with the specified connection handle.
 * This function is responsible for clearing all data related to a specific
 * connection handle. It resets the relevant variables and structures to
 * ensure that no stale data remains.
 *
 * @param connHandle The connection handle for which data should be cleared.
 */
static void rreq_clearData(uint16_t connHandle)
{
    // Clear the start/end handles
    gRREQControlBlock.connInfo[connHandle].startHandle = 0;
    gRREQControlBlock.connInfo[connHandle].endHandle = 0;

    // Clear the characteristic handles
    gRREQControlBlock.connInfo[connHandle].featureCharHandle = 0;
    gRREQControlBlock.connInfo[connHandle].realTimeRangingDataCharHandle = 0;
    gRREQControlBlock.connInfo[connHandle].onDemandRangingDataCharHandle = 0;
    gRREQControlBlock.connInfo[connHandle].controlPointCharHandle = 0;
    gRREQControlBlock.connInfo[connHandle].rangingDataReadyCharHandle = 0;
    gRREQControlBlock.connInfo[connHandle].rangingDataOverwrittenCharHandle = 0;

    // Clear the characteristic properties
    gRREQControlBlock.connInfo[connHandle].featureCharProperties = 0;
    gRREQControlBlock.connInfo[connHandle].realTimeRangingDataCharProperties = 0;
    gRREQControlBlock.connInfo[connHandle].onDemandRangingDataCharProperties = 0;
    gRREQControlBlock.connInfo[connHandle].controlPointCharProperties = 0;
    gRREQControlBlock.connInfo[connHandle].rangingDataReadyCharProperties = 0;
    gRREQControlBlock.connInfo[connHandle].rangingDataOverwrittenCharProperties = 0;

    gRREQControlBlock.connInfo[connHandle].enableMode = RREQ_IDLE;
    gRREQControlBlock.connInfo[connHandle].subscribeBitMap = 0;
    gRREQControlBlock.connInfo[connHandle].featureCharValue = 0;

    // Close the ranging client DB
    RangingDBClient_procedureClose(connHandle);

    // Clear the segment manager
    memset(&gRREQControlBlock.connInfo[connHandle].segmentMgr, 0, sizeof(RREQSegmentsMGR_t));
}

/*********************************************************************
 * @fn      rreq_handleSegmentReceived
 *
 * @brief   Handles the reception of a segment from the RAS server.
 * This function processes the received segment and updates the state
 * of the RREQ accordingly.
 *
 * @param connHandle The connection handle for the RAS server.
 * @param handleValueNoti Pointer to the handle value notification structure.
 *
 * @return None.
 */
static void rreq_handleSegmentReceived(uint16_t connHandle, attHandleValueNoti_t *handleValueNoti)
{
    // Check if the connection handle is valid
    // and if the configuration is not NULL
    if ((connHandle >= RREQ_MAX_CONN) || (gRREQControlBlock.config == NULL) || (handleValueNoti == NULL))
    {
        return;
    }

    switch (gRREQControlBlock.procedureAttr.procedureState)
    {
        case RREQ_STATE_WAIT_FOR_FIRST_SEGMENT:
        {
            gRREQControlBlock.procedureAttr.procedureState = RREQ_STATE_WAIT_FOR_NEXT_SEGMENT;
            // Start the timer for timeout
            rreq_startTimer(gRREQControlBlock.config->timeOutNextSegment);
            // Handle the segment received
            rreq_parseSegmentReceived(connHandle, handleValueNoti);
            break;
        }

        case RREQ_STATE_WAIT_FOR_NEXT_SEGMENT:
        {
            // Start the timer for timeout
            rreq_startTimer(gRREQControlBlock.config->timeOutNextSegment);
            // Handle the segment received
            rreq_parseSegmentReceived(connHandle, handleValueNoti);
            break;
        }

        default:
            // Unknown state, do nothing
            break;
    }
}

/*********************************************************************
 * @fn      rreq_startTimer
 *
 * @brief   Start the timer for the RREQ procedure.
 *
 * @param   timeout - The new timeout value in milliseconds.
 *
 * @return  None
 */
static void rreq_startTimer( uint32_t timeout )
{
    if (gRREQControlBlock.timeoutHandle != BLEAPPUTIL_TIMER_INVALID_HANDLE)
    {
        // Stop the timer if it is already running
        BLEAppUtil_abortTimer(gRREQControlBlock.timeoutHandle);
    }

    // Start the timer
    gRREQControlBlock.timeoutHandle = BLEAppUtil_startTimer(rreq_timerCB, timeout, false, NULL);
}

/*********************************************************************
 * @fn      rreq_stopTimer
 *
 * @brief   Stop the timer for the RREQ procedure.
 *
 * @param   timeout - The new timeout value in milliseconds.
 *
 * @return  None
 */
static void rreq_stopTimer( void )
{
    if (gRREQControlBlock.timeoutHandle != BLEAPPUTIL_TIMER_INVALID_HANDLE)
    {
        int32_t status = SUCCESS;

        // Stop the timer
        status = BLEAppUtil_abortTimer(gRREQControlBlock.timeoutHandle);
        if (status == SUCCESS)
        {
            // Reset the timer handle
            gRREQControlBlock.timeoutHandle = BLEAPPUTIL_TIMER_INVALID_HANDLE;
        }
    }
}

/*********************************************************************
 * @fn      rreq_timerCB
 *
 * @brief   Timer callback function for the RREQ enable process.
 *          This function is called when the timer expires.
 *          It checks the reason for the timer expiration and
 *          performs the necessary actions.
 *
 * input parameters
 *
 * @param   timerHandle - The handle of the timer that expired.
 * @param   reason - The reason for the timer expiration.
 * @param   pData - Pointer to the data associated with the timer.
 *
 * output parameters
 *
 * @param   None
 *
 * @return None
 */
static void rreq_timerCB(BLEAppUtil_timerHandle timerHandle, BLEAppUtil_timerTermReason_e reason, void *pData)
{
  if (reason == BLEAPPUTIL_TIMER_TIMEOUT)
  {
    switch (gRREQControlBlock.procedureAttr.procedureState)
    {
        case RREQ_STATE_WAIT_FOR_FIRST_SEGMENT:
        case RREQ_STATE_WAIT_FOR_NEXT_SEGMENT:
        {
            if((gRREQControlBlock.callbacks != NULL) && (gRREQControlBlock.callbacks->pStatusCallback != NULL))
            {
                // Check if server supports Abort operation
                if(gRREQControlBlock.connInfo[gRREQControlBlock.procedureAttr.connHandle].featureCharValue & RAS_FEATURES_ABORT_OPERATION )
                {
                    RREQ_Abort(gRREQControlBlock.procedureAttr.connHandle);
                }

                // Notify App of timeout event
                gRREQControlBlock.callbacks->pStatusCallback(gRREQControlBlock.procedureAttr.connHandle, RREQ_TIMEOUT , RANGING_COUNTER_LEN, (uint8_t*)&gRREQControlBlock.procedureAttr.rangingCounter);

            }
            gRREQControlBlock.procedureAttr.procedureState = RREQ_STATE_IDLE; // Reset the procedure state

            break;
        }
        default:

            // Unknown state, do nothing
            break;
    }

    // Reset the timer handle
    gRREQControlBlock.timeoutHandle = BLEAPPUTIL_TIMER_INVALID_HANDLE;
  }
}

#endif // RANGING_CLIENT
