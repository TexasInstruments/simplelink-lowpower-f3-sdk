/******************************************************************************

@file  ranging_profile_client.c

@brief This file contains the ranging requester (RREQ) APIs and functionality.

Group: WCS, BTS
Target Device: cc23xx

******************************************************************************

 Copyright (c) 2025-2026, Texas Instruments Incorporated
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
#include "app_gatt_api.h"

//*****************************************************************************
//! Defines
//*****************************************************************************

// Macro to get the current state of an RREQ procedure by index
#define RREQ_CURRENT_STATE(index) (gRREQControlBlock.connInfo[(index)].procedureAttr.procedureState)
#define RREQ_IS_INIT_STATE_COMPLETED(index, state) \
            ((gRREQControlBlock.connInfo[(index)].procedureAttr.completedInitStatesBM & (state)) != 0)

#define RREQ_INIT_STATES_BITMAP ((uint16_t) (RREQ_STATE_INIT_DISCOVER_PRIM_SERVICE | \
                                             RREQ_STATE_INIT_DISCOVER_ALL_CHARS | \
                                             RREQ_STATE_INIT_DISCOVER_ALL_CHAR_DESCS | \
                                             RREQ_STATE_INIT_READ_CHAR_FEATURE))

// Macro to check if a subscription type is for enabling notifications/indications
#define RREQ_IS_SUBTYPE_ENABLED(subType)    ((subType) == RREQ_PREFER_NOTIFY || (subType) == RREQ_INDICATE)

// The index of characteristic properties handle
#define RREQ_APP_CHAR_PRO_HANDLE_INDEX              0x02
// The index of characteristic value handle
#define RREQ_APP_CHAR_VALUE_HANDLE_INDEX            0x03
// The index of characteristic UUID handle
#define RREQ_APP_CHAR_UUID_HANDLE_INDEX             0x05

// Profile number of characteristics
#define RREQ_NUM_CHARS                              6U

// Maps characteristic UUID to its array index
#define RREQ_UUID_TO_CHAR_INDEX(uuid)               ((uint8_t) ((uuid) - RAS_FEATURE_UUID))

// Maps characteristic UUID to its subscription bit
// Warning: Do not use for @ref RAS_FEATURE_UUID
#define RREQ_UUID_TO_CHAR_SUBSCRIBE_BIT(uuid)    ((uint8_t) (BV(RREQ_UUID_TO_CHAR_INDEX(uuid) - 1)))

// Macros to set/clear subscribe bits
#define RREQ_CLEAR_SUBSCRIBE_BIT(subscribeBitMap, uuid)  (subscribeBitMap &= ~RREQ_UUID_TO_CHAR_SUBSCRIBE_BIT(uuid))
#define RREQ_SET_SUBSCRIBE_BIT(subscribeBitMap, uuid)    (subscribeBitMap |= RREQ_UUID_TO_CHAR_SUBSCRIBE_BIT(uuid))

#define RREQ_FEATURE_CHAR_INDEX                     0U
#define RREQ_REAL_TIME_CHAR_INDEX                   1U
#define RREQ_ON_DEMAND_CHAR_INDEX                   2U
#define RREQ_CONTROL_POINT_CHAR_INDEX               3U
#define RREQ_DATA_READY_CHAR_INDEX                  4U
#define RREQ_DATA_OVERWRITTEN_CHAR_INDEX            5U

// 16-bit high and low index in the 16-bit custom UUID
#define RREQ_APP_LOW_UUID_INDEX                     0x00
#define RREQ_APP_HIGH_UUID_INDEX                    0x01

// CCCD handle offset
#define RREQ_APP_CCCD_OFFSET                        0x01
// CCCD value length
#define RREQ_APP_CCCD_VALUE_LEN                     0x02

// The maximum number of connections for the RAS client
#define RREQ_MAX_CONN                               MAX_NUM_BLE_CONNS

// Invalid CS procedure counter
#define RREQ_INVALID_CS_PROCEDURE_COUNTER           0xFFFFFFFF

// Invalid UUID
#define RREQ_INVALID_UUID                           0xFFFF

// Invalid index
#define RREQ_INVALID_INDEX                          0xFF

#define RREQ_INDEX_CONN_HANDLE(index)               (gRREQControlBlock.connInfo[(index)].connHandle)

// Notification and Indication properties bit mask
#define RAS_NOTIFICATION_PRO_MASK                   0x10
#define RAS_INDICATION_PRO_MASK                     0x20

// segmentation bit masks
#define RAS_FIRST_SEGMENT_BIT_MASK                  0x01
#define RAS_LAST_SEGMENT_BIT_MASK                   0x02

// Last 6 bits (LSB) of uint8_t
#define RAS_LAST_6_BITS_LSB(x)                      ((x & 0xFc) >> 2)
// First 2 bits (LSB) of uint8_t
#define RAS_FIRST_2_BITS_LSB(x)                     (x & 0x03)

/*********************************************************************
 * TYPEDEFS
 */

// RREQ Procedure States
typedef enum
{
    RREQ_STATE_IDLE                         = (uint16_t)BV(0),  // Unable to take new procedures; Set before initialization or when both modes are disabled
    RREQ_STATE_INIT_DISCOVER_PRIM_SERVICE   = (uint16_t)BV(1),  // Waiting for primary service discovery response (ATT_FIND_BY_TYPE_VALUE_RSP); Initialization phase
    RREQ_STATE_INIT_DISCOVER_ALL_CHARS      = (uint16_t)BV(2),  // Waiting for all characteristics discovery response (ATT_READ_BY_TYPE_RSP); Initialization phase
    RREQ_STATE_INIT_DISCOVER_ALL_CHAR_DESCS = (uint16_t)BV(3),  // Waiting for all characteristic descriptors discovery response (ATT_FIND_INFO_RSP); Initialization phase
    RREQ_STATE_INIT_READ_CHAR_FEATURE       = (uint16_t)BV(4),  // Waiting for reading feature characteristic response (ATT_READ_RSP); Initialization phase
    RREQ_STATE_INIT_REGISTERING             = (uint16_t)BV(5),  // Waiting for characteristic registration to complete (ATT_WRITE_RSP); Initialization phase
    RREQ_STATE_REGISTERING                  = (uint16_t)BV(6),  // Waiting for characteristic registration to complete (ATT_WRITE_RSP)
    RREQ_STATE_READY                        = (uint16_t)BV(7),  // Ready for new procedures
    RREQ_STATE_WAIT_FOR_DATA_READY          = (uint16_t)BV(8),  // Waiting for data ready event; Real-time \ On-demand ranging procedure is ongoing
    RREQ_STATE_WAIT_FOR_FIRST_SEGMENT       = (uint16_t)BV(9),  // Waiting for the first segment; Real-time \ On-demand ranging procedure is ongoing
    RREQ_STATE_WAIT_FOR_NEXT_SEGMENT        = (uint16_t)BV(10), // Waiting for the next segment or the complete data; Real-time \ On-demand ranging procedure is ongoing
    RREQ_STATE_SEND_ACK                     = (uint16_t)BV(11), // Pending to send an ACK after receiving all segments; On-demand ranging procedure is ongoing
    RREQ_STATE_WAIT_FOR_CONTROL_POINT_RSP   = (uint16_t)BV(12), // Waiting for the control point response, after sending and ACK; On-demand ranging procedure is ongoing
    RREQ_STATE_WAIT_FOR_ABORT               = (uint16_t)BV(13)  // Waiting for the abort response; On-demand ranging procedure is ongoing
} RREQProcedureStates_e;

// RREQ Subscription types
typedef enum
{
    RREQ_REAL_TIME_BIT      = (uint32_t)BV(0),  // Real-time ranging data subscription
    RREQ_ON_DEMAND_BIT      = (uint32_t)BV(1),  // On-demand ranging data subscription
    RREQ_CONTROL_POINT_BIT  = (uint32_t)BV(2),  // Control point command subscription
    RREQ_DATA_READY_BIT     = (uint32_t)BV(3),  // Data ready event subscription
    RREQ_OVERWRITTEN_BIT    = (uint32_t)BV(4)   // Data overwritten event subscription
}RREQ_SubscribeBitMap_e;

// RREQ characteristics structure
typedef struct
{
    uint16_t charHandle;
    uint16_t charProperties;
} RREQ_CharInfo_t ;

// RREQ Procedure Attribute structure
typedef struct
{
    RREQProcedureStates_e procedureState;   // Current state of the RREQ procedure
    uint16_t completedInitStatesBM;         // Bitmap of completed init states

    /* Ranging procedure counter.
       For Real-Time, determined when first segment received.
       For On-Demand, determined when @ref RREQ_GetRangingData is called */
    uint16_t rangingCounter;
} RREQProcedureAttr_t;

typedef struct
{
    uint8_t  rangingDbHandle;                           // Ranging DB handle associated with this entry.
    uint16_t connHandle;                                // Connection handle associated with this entry.
    uint32_t currentProcedureCounter;                   // CS procedure counter of the currently running by the controller.
    uint32_t featureCharValue;                          // Feature characteristic value; Set after reading the feature characteristic
    uint16_t startHandle;                               // Start handle of the service
    uint16_t endHandle;                                 // End handle of the service
    uint8_t  subscribeBitMap;                           // Subscription bit map
    uint8_t  lastSegmentFlag;                           // Flag to indicate if the last segment is received.
    uint16_t currentConfiguredCharUUID;                 // UUID of the characteristic currently being configured
    RREQConfigSubType_e currentConfiguredCharSubtype;   // Subscription type of the characteristic currently being configured
    RREQ_CharInfo_t charInfo[RREQ_NUM_CHARS];           // Characteristics information array; ordered according to UUID
    RREQProcedureAttr_t procedureAttr;                  // RAS Procedure attributes; Contain the current state and ranging counter of the current procedure
    RREQEnableModeType_e preferredMode;                 // Preferred mode to be enabled if possible; Set when @ref RREQ_Enable is called
    RREQEnableModeType_e enableMode;                    // Enable mode; Set when initialization is done
    BLEAppUtil_timerHandle timeoutHandle;               // Timer handle for the RREQ timeout
} RREQ_ConnInfo_t;

typedef struct
{
    RREQ_ConnInfo_t connInfo[RANGING_DB_CLIENT_MAX_NUM_PROC];   // Connection information
    const RREQConfig_t* config;                                 // Configuration for the RREQ
    const RREQCallbacks_t* callbacks;                           // Application callbacks
    bool l2capRegistered;                                       // Flag to indicate if L2CAP events are registered
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

/* State transitioning functions. Non-state dependent */
static bStatus_t rreq_discoverPrimServ(uint8_t index);
static bStatus_t rreq_discoverAllChars(uint8_t index);
static bStatus_t rreq_discoverAllCharDescriptors(uint8_t index);
static bStatus_t rreq_readFeaturesCharValue(uint8_t index);
static bStatus_t rreq_registerConfigChars(uint8_t index);
static bStatus_t rreq_registerOnDemandConfigChars(uint8_t index);
static bStatus_t rreq_registerRealTimeConfigChars(uint8_t index);
static void rreq_sendAck(uint8_t index);
static void rreq_handleStateInitError(uint8_t index, uint8_t error);
static void rreq_handlePostRegistrationState(uint8_t index, uint8_t status, bool isRejectedByServer);
static void rreq_procedureDone(uint8_t index);

/* Server responses handling. State depended, State transitioning */
static void rreq_handleControlPointNotification(uint8_t index, attHandleValueNoti_t *handleValueNoti);
static void rreq_handleDataReady(uint8_t index, attHandleValueNoti_t *handleValueNoti);
static void rreq_handleRspCode(uint8_t index, uint8_t rspValue);
static void rreq_handleOnDemandSegmentReceived(uint8_t index, attHandleValueNoti_t *handleValueNoti);
static void rreq_handleRealTimeSegmentReceived(uint8_t index, attHandleValueNoti_t *handleValueNoti);

/* GATT events handlers. State depended, State transitioning */
static void rreq_handleFindByTypeValueRsp(uint8_t index, gattMsgEvent_t *gattMsg);
static void rreq_handleReadByTypeRsp(uint8_t index, gattMsgEvent_t *gattMsg);
static void rreq_handleFindInfoRsp(uint8_t index, gattMsgEvent_t *gattMsg);
static void rreq_handleReadRsp(uint8_t index, gattMsgEvent_t *gattMsg);
static void rreq_handleWriteRsp(uint8_t index, gattMsgEvent_t *gattMsg);
static void rreq_handleValueNoti(uint8_t index, attHandleValueNoti_t *handleValueNoti);
static void rreq_handleOnDemandValueNoti(uint8_t index, attHandleValueNoti_t *handleValueNoti);
static void rreq_handleRealTimeValueNoti(uint8_t index, attHandleValueNoti_t *handleValueNoti);
static void rreq_handleErrorRsp(uint8_t index, gattMsgEvent_t *gattMsg);

/* Helper functions */
static bStatus_t rreq_configureCharRegistration(uint8_t index, uint16_t charUUID, RREQConfigSubType_e subType);
static bStatus_t rreq_registerCharacteristic(uint8_t index, uint16_t charUUID, RREQConfigSubType_e mode);
static bStatus_t rreq_unregisterCharacteristic(uint8_t index, uint16_t charUUID);
static bStatus_t rreq_enableNotification(uint8_t index, RREQConfigSubType_e mode, uint16_t attHandle);
static bStatus_t rreq_sendControlPointWriteCmd(uint8_t index, uint8_t cmd, uint16_t rangingCounter, uint8_t len);

/* Internal DB functions */
static uint8_t rreq_getIndexByConnHandle(uint16_t connHandle);
static uint8_t rreq_getEmptyIndex(void);
static RREQConfigSubType_e rreq_getCharProperties(RREQConfigSubType_e preferredSubType, uint8_t properties);
static bool rreq_checkConfigRegistration(uint8_t index, uint16_t uuid, RREQConfigSubType_e configSubType);
static bool rreq_checkRegistration(uint8_t subscribeBitMap, uint16_t uuid);
static bool rreq_checkRegistrationOnDemand(uint8_t index);
static bool rreq_checkRegistrationRealTime(uint8_t index);
static RREQEnableModeType_e rreq_getConfigModeToEnable(uint8_t index);
static void rreq_setCurrentConfiguredCharInfo(uint8_t index, uint16_t charUUID, RREQConfigSubType_e charSubtype);
static void rreq_clearCurrentConfiguredCharInfo(uint8_t index);
static bool rreq_isInitDone(uint8_t index);
static void rreq_setProcedureState(uint8_t index, RREQProcedureStates_e stateToSet);
static void rreq_setInitStateCompleted(uint8_t index, RREQProcedureStates_e state);
static void rreq_parseSegmentReceived(uint8_t index, attHandleValueNoti_t *handleValueNoti);
static void rreq_clearData(uint8_t index);
static void rreq_clearLastSegmentFlag(uint8_t index);

/* Application callback handlers */
static void rreq_sendStatusCB(uint16_t connHandle, RREQClientStatus_e statusCode, uint8_t statusDataLen, uint8_t* statusData);

/* Timer functions */
static void rreq_startTimer( uint32_t timeout, uint8_t index );
static void rreq_stopTimer( uint8_t index );
static void rreq_handleTimeoutRealTime(uint8_t index);
static void rreq_handleTimeoutOnDemand(uint8_t index);
static void rreq_timerCB(BLEAppUtil_timerHandle timerHandle, BLEAppUtil_timerTermReason_e reason, void *pData);

/* L2CAP event handlers */
void rreq_L2CapEventHandler(uint32 event, BLEAppUtil_msgHdr_t *pMsgData);
void rreq_checkAndUnregisterL2CAP(void);

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

// Handler for L2CAP events
BLEAppUtil_EventHandler_t gRREQL2CAPHandler =
{
    .handlerType    = BLEAPPUTIL_L2CAP_SIGNAL_TYPE,
    .pEventHandler  = rreq_L2CapEventHandler,
    .eventMask      = BLEAPPUTIL_L2CAP_NUM_CTRL_DATA_PKT_EVT
};

//*****************************************************************************
//! Functions
//*****************************************************************************

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*******************************************************************************
 * Public function defined in ranging_profile_client.h.
 */
uint8_t RREQ_Start(const RREQCallbacks_t *pCallbacks , const RREQConfig_t *pConfig)
{
    uint8_t status = SUCCESS;

    // Check input parameters and check if this function already called once
    if ( pCallbacks == NULL ||
         pConfig == NULL ||
         gRREQControlBlock.config != NULL ||
         gRREQControlBlock.callbacks != NULL)
    {
        return INVALIDPARAMETER;
    }

    // Check that at least one mode is selected
    if (!RREQ_IS_SUBTYPE_ENABLED(pConfig->onDemandSubConfig.onDemandSubType) &&
        !RREQ_IS_SUBTYPE_ENABLED(pConfig->realTimeSubConfig.realTimeSubType))
    {
        return INVALIDPARAMETER;
    }

    // If on-demand mode is selected - check depended characteristics subscription types
    if (RREQ_IS_SUBTYPE_ENABLED(pConfig->onDemandSubConfig.onDemandSubType) &&
        (!RREQ_IS_SUBTYPE_ENABLED(pConfig->onDemandSubConfig.controlPointSubType) ||
         !RREQ_IS_SUBTYPE_ENABLED(pConfig->onDemandSubConfig.dataReadySubType) ||
         !RREQ_IS_SUBTYPE_ENABLED(pConfig->onDemandSubConfig.overwrittenSubType)))
    {
        return INVALIDPARAMETER;
    }

    // Check timeouts for both modes - first segment and next segment
    if (pConfig->timeoutConfig.timeOutFirstSegment > RREQ_MAX_TIMEOUT_FIRST_SEGMENT_MS ||
        pConfig->timeoutConfig.timeOutNextSegment > RREQ_MAX_TIMEOUT_NEXT_SEGMENT_MS ||
        pConfig->timeoutConfig.timeOutControlPointRsp > RREQ_MAX_TIMEOUT_CONTROL_POINT_RSP_MS ||
        pConfig->timeoutConfig.timeOutDataReady > RREQ_MAX_TIMEOUT_DATA_READY_MS)
    {
        return INVALIDPARAMETER;
    }

    // Register the GATT event handler
    status = BLEAppUtil_registerEventHandler(&RREQGATTHandler);

    if ( status == SUCCESS )
    {
        // Init ranging client DB
        status = RangingDBClient_initDB();
    }

    // If everything succeeded - save configurations, clear the profile data and register to L2CAP flow control events
    if (status == SUCCESS)
    {
        // Save configuration and callbacks
        gRREQControlBlock.config = pConfig;
        gRREQControlBlock.callbacks = pCallbacks;
        gRREQControlBlock.l2capRegistered = false;

        // Clear all connections data
        for (uint8_t index = 0; index < RANGING_DB_CLIENT_MAX_NUM_PROC; index++)
        {
            rreq_clearData(index);
        }

        // Register the application callback function for L2CAP Flow Control
        L2CAP_RegisterFlowCtrlTask(BLEAppUtil_getSelfEntity());
    }

    return status;
}

/*******************************************************************************
 * Public function defined in ranging_profile_client.h.
 */
uint8_t RREQ_Enable(uint16_t connHandle, RREQEnableModeType_e enableMode)
{
    uint8_t status = SUCCESS;
    uint8_t index;
    uint8_t rangingDbHandle = RANGING_DB_CLIENT_INVALID_HANDLE;

    if (connHandle >= RREQ_MAX_CONN ||
        (enableMode != RREQ_MODE_ON_DEMAND && enableMode != RREQ_MODE_REAL_TIME))
    {
        // Invalid connection handle or enable mode
        status = INVALIDPARAMETER;
    }
    else
    {
        // Get the index by connection handle
        index = rreq_getIndexByConnHandle(connHandle);

        // If not found
        if (index == RREQ_INVALID_INDEX)
        {
            // Get an empty index, as this is the first time enabling RREQ for this connection
            index = rreq_getEmptyIndex();

            // If not found, return failure
            if (index == RREQ_INVALID_INDEX)
            {
                status = FAILURE;
            }
            else
            {
                // Try to open a new DB entry for this connection
                rangingDbHandle = RangingDBClient_procedureOpen();

                if (rangingDbHandle == RANGING_DB_CLIENT_INVALID_HANDLE)
                {
                    // No available entry in the DB
                    status = FAILURE;
                }
                else
                {
                    // Found a new entry - update the rangingDbHandle in the connection info
                    gRREQControlBlock.connInfo[index].rangingDbHandle = rangingDbHandle;
                }
            }
        }
        else
        {
            // Already opened, check if initialization the current state
            if (RREQ_CURRENT_STATE(index) != RREQ_STATE_IDLE)
            {
                // Proceed only in IDLE state
                status = bleIncorrectMode;
            }
        }

        if (status == SUCCESS)
        {
            // Update connection handle for this index in case this is the first time it's opened
            gRREQControlBlock.connInfo[index].connHandle = connHandle;

            // Set the preferred mode as given by the caller
            gRREQControlBlock.connInfo[index].preferredMode = enableMode;

            // Proceed with the initialization procedure for the RAS service
            // Start from the last not completed procedure state

            if (RREQ_IS_INIT_STATE_COMPLETED(index, RREQ_STATE_INIT_DISCOVER_PRIM_SERVICE) == false)
            {
                status = rreq_discoverPrimServ(index);
            }
            else if (RREQ_IS_INIT_STATE_COMPLETED(index, RREQ_STATE_INIT_DISCOVER_ALL_CHARS) == false)
            {
                status = rreq_discoverAllChars(index);
            }
            else if (RREQ_IS_INIT_STATE_COMPLETED(index, RREQ_STATE_INIT_DISCOVER_ALL_CHAR_DESCS) == false)
            {
                status = rreq_discoverAllCharDescriptors(index);
            }
            else if (RREQ_IS_INIT_STATE_COMPLETED(index, RREQ_STATE_INIT_READ_CHAR_FEATURE) == false)
            {
                status = rreq_readFeaturesCharValue(index);
            }
            else
            {
                // We already have the feature characteristic value, proceed to register configuration characteristics
                status = rreq_registerConfigChars(index);
            }
        }
    }
    return status;
}

/*******************************************************************************
 * Public function defined in ranging_profile_client.h.
 */
uint8_t RREQ_Disable(uint16_t connHandle)
{
    uint8_t status = SUCCESS;
    uint8_t registrationStatus = SUCCESS;
    uint8_t index = RREQ_INVALID_INDEX;

    // Check connection handle and get index if exists
    if (connHandle >= RREQ_MAX_CONN ||
        (index = rreq_getIndexByConnHandle(connHandle)) == RREQ_INVALID_INDEX)
    {
        status = INVALIDPARAMETER;
    }

    // Unregister On-Demand or Real-Time characteristics if connected and registered
    if (status == SUCCESS)
    {
        if (rreq_checkRegistration(gRREQControlBlock.connInfo[index].subscribeBitMap, RAS_ON_DEMAND_UUID))
        {
            registrationStatus = rreq_configureCharRegistration(index, RAS_ON_DEMAND_UUID, RREQ_DISABLE_NOTIFY_INDICATE);
        }
        else if (rreq_checkRegistration(gRREQControlBlock.connInfo[index].subscribeBitMap, RAS_REAL_TIME_UUID))
        {
            registrationStatus = rreq_configureCharRegistration(index, RAS_REAL_TIME_UUID, RREQ_DISABLE_NOTIFY_INDICATE);
        }

        // If connected - take the status returned from @ref rreq_configureCharRegistration.
        // If not connected - no need to unregister, success will be returned.
        if (registrationStatus != bleNotConnected)
        {
            status = registrationStatus;
        }
    }

    // Clear all data related to this connection handle
    if (status == SUCCESS)
    {
        // Stop the timer in case it's running
        rreq_stopTimer(index);

        // Close the ranging client DB (must be done before rreq_clearData which invalidates the handle)
        RangingDBClient_procedureClose(gRREQControlBlock.connInfo[index].rangingDbHandle);

        // Clear all index saved data
        rreq_clearData(index);
    }

    return status;
}

/*******************************************************************************
 * Public function defined in ranging_profile_client.h.
 */
uint8_t RREQ_ConfigureCharRegistration(uint16_t connHandle, uint16_t charUUID, RREQConfigSubType_e subType)
{
    uint8_t status = SUCCESS;
    uint8_t index;

    // Check connection handle and subType
    if (connHandle >= RREQ_MAX_CONN || subType >= RREQ_SUBTYPE_INVALID ||
        (index = rreq_getIndexByConnHandle(connHandle)) == RREQ_INVALID_INDEX)
    {
        status = INVALIDPARAMETER;
    }

    // Check that initialization is done
    if (status == SUCCESS && rreq_isInitDone(index) == false)
    {
        status = bleIncorrectMode;
    }

    // If current state is not one of IDLE or READY - reject
    if (status == SUCCESS &&
        RREQ_CURRENT_STATE(index) != RREQ_STATE_IDLE &&
        RREQ_CURRENT_STATE(index) != RREQ_STATE_READY)
    {
        status = bleIncorrectMode;
    }

    if (status == SUCCESS)
    {
        switch(charUUID)
        {
            case RAS_REAL_TIME_UUID:
            case RAS_ON_DEMAND_UUID:
            case RAS_CONTROL_POINT_UUID:
            case RAS_DATA_READY_UUID:
            case RAS_DATA_OVERWRITTEN_UUID:
            {
                status = rreq_configureCharRegistration(index, charUUID, subType);

                if (status == SUCCESS)
                {
                    rreq_setProcedureState(index, RREQ_STATE_REGISTERING);
                }

                break;
            }

            case RAS_FEATURE_UUID:
            default:
            {
                status = INVALIDPARAMETER;
                break;
            }
        }
    }

    return status;
}

/*******************************************************************************
 * Public function defined in ranging_profile_client.h.
 */
uint8_t RREQ_GetRangingData(uint16_t connHandle, uint16_t rangingCount)
{
    uint8_t status = SUCCESS;
    uint8_t index;

    // Check if the connection handle is valid, the configuration is not NULL,
    // there is an active procedure, and that the enabled mode is On-Demand
    if ((connHandle >= RREQ_MAX_CONN) || (gRREQControlBlock.config == NULL) ||
        (index = rreq_getIndexByConnHandle(connHandle)) == RREQ_INVALID_INDEX ||
        (gRREQControlBlock.connInfo[index].enableMode != RREQ_MODE_ON_DEMAND))
    {
        status = INVALIDPARAMETER;
    }

    // Check if the procedure state is ready
    if (status == SUCCESS)
    {
        if (RREQ_CURRENT_STATE(index) == RREQ_STATE_READY)
        {
            // Send write command to control point characteristic handle
            status = rreq_sendControlPointWriteCmd(index, RAS_CP_OPCODE_GET_RANGING_DATA, rangingCount, RAS_CP_GET_DATA_CMD_LEN);

            // Init the procedure Attr
            if(status == SUCCESS)
            {
                rreq_setProcedureState(index, RREQ_STATE_WAIT_FOR_FIRST_SEGMENT);
                gRREQControlBlock.connInfo[index].procedureAttr.rangingCounter = rangingCount;

                // Start the timer for receiving first segment
                rreq_startTimer(gRREQControlBlock.config->timeoutConfig.timeOutFirstSegment, index);
            }
        }
        else
        {
            // The procedure is already in progress
            status = bleIncorrectMode;
        }
    }

    return status;
}

/*******************************************************************************
 * Public function defined in ranging_profile_client.h.
 */
uint8_t RREQ_Abort(uint16_t connHandle)
{
    uint8_t status = SUCCESS;
    uint8_t index;

    // Check if the connection handle is valid
    if (connHandle >= RREQ_MAX_CONN ||
        (index = rreq_getIndexByConnHandle(connHandle)) == RREQ_INVALID_INDEX)
    {
        status = INVALIDPARAMETER;
    }

    if (status == SUCCESS)
    {
        // Ensure:
        // 1. Abort operation is allowed
        // 2. Enabled mode is On-Demand
        // 3. Procedure is in progress
        if ((gRREQControlBlock.connInfo[index].featureCharValue & RAS_FEATURES_ABORT_OPERATION) &&
            gRREQControlBlock.connInfo[index].enableMode == RREQ_MODE_ON_DEMAND &&
            (RREQ_CURRENT_STATE(index) == RREQ_STATE_WAIT_FOR_FIRST_SEGMENT ||
             RREQ_CURRENT_STATE(index) == RREQ_STATE_WAIT_FOR_NEXT_SEGMENT))
        {
            // Send Abort command to the server
            status = rreq_sendControlPointWriteCmd(index, RAS_CP_OPCODE_ABORT_OPERATION,
                                                   gRREQControlBlock.connInfo[index].procedureAttr.rangingCounter,
                                                   RAS_CP_ABORT_CMD_LEN);
            if( status == SUCCESS)
            {
                // Reset the procedure state to wait for abort response
                rreq_setProcedureState(index, RREQ_STATE_WAIT_FOR_ABORT);

                // Start timer to wait for Rsp Code
                rreq_startTimer(gRREQControlBlock.config->timeoutConfig.timeOutControlPointRsp, index);
            }
        }
        else
        {
            status = bleIncorrectMode;
        }
    }

    return status;
}

/*******************************************************************************
 * Public function defined in ranging_profile_client.h.
 */
uint8_t RREQ_ProcedureStarted(uint16_t connHandle, uint16_t procedureCounter)
{
    uint8_t index;

    // Ensure the connection handle is valid
    if (connHandle >= RREQ_MAX_CONN ||
        (index = rreq_getIndexByConnHandle(connHandle)) == RREQ_INVALID_INDEX)
    {
        return INVALIDPARAMETER;
    }

    // Ensure Real-Time mode is enabled and that no other procedure is in progress
    if (RREQ_CURRENT_STATE(index) != RREQ_STATE_READY ||
        (gRREQControlBlock.connInfo[index].enableMode != RREQ_MODE_REAL_TIME &&
         gRREQControlBlock.connInfo[index].enableMode != RREQ_MODE_ON_DEMAND))
    {
        return bleIncorrectMode;
    }

    // Ensure that the procedure counter is different than the last notified one
    if (gRREQControlBlock.connInfo[index].currentProcedureCounter == procedureCounter)
    {
        return bleAlreadyInRequestedMode;
    }

    // Keep track of the CS procedure counter for this connection
    gRREQControlBlock.connInfo[index].currentProcedureCounter = procedureCounter;

    if (gRREQControlBlock.connInfo[index].enableMode == RREQ_MODE_REAL_TIME)
    {
        // Set the state to wait for the first segment and update the ranging counter
        rreq_setProcedureState(index, RREQ_STATE_WAIT_FOR_FIRST_SEGMENT);

        // Start the timer for the first segment
        rreq_startTimer(gRREQControlBlock.config->timeoutConfig.timeOutFirstSegment, index);
    }
    else // On-Demand mode
    {
        // Set the state to wait for the first segment and update the ranging counter
        rreq_setProcedureState(index, RREQ_STATE_WAIT_FOR_DATA_READY);

        // Start the timer for the data ready event
        rreq_startTimer(gRREQControlBlock.config->timeoutConfig.timeOutDataReady, index);
    }


    return SUCCESS;
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
    uint8_t index;

    // Check if there is an active RREQ procedure for this connection handle
    if (gattMsg->connHandle >= RREQ_MAX_CONN ||
        (index = rreq_getIndexByConnHandle(gattMsg->connHandle)) == RREQ_INVALID_INDEX )
    {
        return;
    }
    switch ( event )
    {
        case BLEAPPUTIL_ATT_FIND_BY_TYPE_VALUE_RSP:
        {
            // Handle the find by type value response
            rreq_handleFindByTypeValueRsp(index, gattMsg);
            break;
        }

        case BLEAPPUTIL_ATT_READ_BY_TYPE_RSP:
        {
            // Handle the read by type response
            rreq_handleReadByTypeRsp(index, gattMsg);
            break;
        }

        case BLEAPPUTIL_ATT_FIND_INFO_RSP:
        {
            // Handle the find info response
            rreq_handleFindInfoRsp( index, gattMsg );
            break;
        }

        case BLEAPPUTIL_ATT_READ_RSP:
        {
            // Handle the read response
            rreq_handleReadRsp(index, gattMsg);
            break;
        }

        case BLEAPPUTIL_ATT_WRITE_RSP:
        {
            // Handle the write response
            rreq_handleWriteRsp(index, gattMsg);
            break;
        }

        case BLEAPPUTIL_ATT_HANDLE_VALUE_NOTI:
        {
            // Handle the notification
            rreq_handleValueNoti(index, &(gattMsg->msg.handleValueNoti));
            break;
        }

        case BLEAPPUTIL_ATT_HANDLE_VALUE_IND:
        {
            // Send an indication confirmation
            ATT_HandleValueCfm(RREQ_INDEX_CONN_HANDLE(index));

            // Handle the indication
            attHandleValueNoti_t handleValueNoti =
            {
             .handle = gattMsg->msg.handleValueInd.handle,
             .len    = gattMsg->msg.handleValueInd.len,
             .pValue = gattMsg->msg.handleValueInd.pValue
            };
            rreq_handleValueNoti(index, &handleValueNoti);
            break;
        }

        case BLEAPPUTIL_ATT_ERROR_RSP:
        {
            // Handle error response
            rreq_handleErrorRsp(index, gattMsg);
            break;
        }

        default:
            break;
    }
}

/*********************************************************************
 * @fn      rreq_discoverAllChars
 *
 * @brief   Finds all characteristics of the RAS service on a remote device.
 *
 * This function initiates a discovery request for all characteristics
 * The result of the read operation.
 * The result will be provided asynchronously through event ATT_READ_RSP
 * or ATT_ERROR_RSP.
 * When executed successfully, the function updates the procedure state.
 *
 * input parameters
 *
 * @param   index - Index of the connection info for which to discover all
 *                  characteristics.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  SUCCESS or stack call status
 */
static bStatus_t rreq_discoverAllChars(uint8_t index)
{
    bStatus_t status = INVALIDPARAMETER;

    // Check if the index is valid
    if (index < RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        status = GATT_DiscAllChars(RREQ_INDEX_CONN_HANDLE(index),
                                   gRREQControlBlock.connInfo[index].startHandle,
                                   gRREQControlBlock.connInfo[index].endHandle,
                                   BLEAppUtil_getSelfEntity());
    }

    if (status == SUCCESS)
    {
        // Update state
        rreq_setProcedureState(index, RREQ_STATE_INIT_DISCOVER_ALL_CHARS);
    }

    return status;
}

/*********************************************************************
 * @fn      rreq_readFeaturesCharValue
 *
 * @brief   Reads the value of a RAS Feature characteristic from a remote device.
 *
 * This function initiates a read request for the value of a feature char
 * identified by its handle on a remote device. The result of the read operation
 * will be provided asynchronously through event ATT_READ_RSP or ATT_ERROR_RSP.
 * When executed successfully, the function updates the procedure state.
 *
 * input parameters
 *
 * @param   index - Index associated with the connection handle for which
 *                  to read the feature characteristic value.
 *
 * output parameters
 *
 * @param   None
 *
 * @return Status code indicating the success or failure of the operation.
 */
static bStatus_t rreq_readFeaturesCharValue(uint8_t index)
{
    attReadReq_t req;
    bStatus_t status = INVALIDPARAMETER;

    // Check if the index is valid
    if (index < RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        // Read the characteristic value
        req.handle = gRREQControlBlock.connInfo[index].charInfo[RREQ_FEATURE_CHAR_INDEX].charHandle;
        status = GATT_ReadCharValue(RREQ_INDEX_CONN_HANDLE(index), &req, BLEAppUtil_getSelfEntity());

        if (status == SUCCESS)
        {
            // Update state
            rreq_setProcedureState(index, RREQ_STATE_INIT_READ_CHAR_FEATURE);
        }
    }

    return status;
}

/*********************************************************************
 * @fn      rreq_handleWriteRsp
 *
 * @brief   Handles write response GATT event.
 *
 * This function processes the @ref BLEAPPUTIL_ATT_WRITE_RSP event
 * and continue the registration procedure.
 * Processing depends on the current procedure state.
 * The relevant states are:
 * @ref RREQ_STATE_INIT_REGISTERING
 * @ref RREQ_STATE_REGISTERING
 *
 * input parameters
 *
 * @param index   - Index of the connection info
 * @param gattMsg - pointer to the GATT message event structure
 *
 * output parameters
 *
 * @param   None
 *
 * @return  none
 */
static void rreq_handleWriteRsp(uint8_t index, gattMsgEvent_t *gattMsg)
{
    bStatus_t status = SUCCESS;

    if(gattMsg == NULL || index >= RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        return;
    }

    status = gattMsg->hdr.status;

    switch(RREQ_CURRENT_STATE(index))
    {
        case RREQ_STATE_INIT_REGISTERING:
        {
            // Keep processing only if we are already in the middle of registration state,
            // until all characteristics are registered.
            if(status == SUCCESS)
            {
                RREQ_SET_SUBSCRIBE_BIT(gRREQControlBlock.connInfo[index].subscribeBitMap,
                                       gRREQControlBlock.connInfo[index].currentConfiguredCharUUID);

                status = rreq_registerConfigChars(index);
            }

            if (status != SUCCESS)
            {
                rreq_handleStateInitError(index, status);
            }
            break;
        }
        case RREQ_STATE_REGISTERING:
        {
            // Handle post state registering
            rreq_handlePostRegistrationState(index, status, false);
            break;
        }

        default:
        {
            break;
        }
    }
}

/*********************************************************************
 * @fn      rreq_handleReadRsp
 *
 * @brief   Handles read response GATT event.
 *
 * This function processes the @ref BLEAPPUTIL_ATT_READ_RSP event
 * and continue the initialization procedure.
 * Processing depends on the current procedure state.
 * The relevant states are:
 * @ref RREQ_STATE_INIT_READ_CHAR_FEATURE
 *
 * input parameters
 *
 * @param index   - Index of the connection info
 * @param gattMsg - pointer to the GATT message event structure
 *
 * output parameters
 *
 * @param   None
 *
 * @return  none
 */
static void rreq_handleReadRsp(uint8_t index, gattMsgEvent_t *gattMsg)
{
    bStatus_t status = SUCCESS;

    if (gattMsg == NULL || index >= RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        return;
    }

    status = gattMsg->hdr.status;

    switch(RREQ_CURRENT_STATE(index))
    {
        case RREQ_STATE_INIT_READ_CHAR_FEATURE:
        {
            // Register to RAS server characteristics
            if(status == SUCCESS)
            {
                // Save RAS feature characteristic value
                memcpy(&gRREQControlBlock.connInfo[index].featureCharValue, gattMsg->msg.readRsp.pValue, sizeof(uint32_t));

                // Mark the read feature state as completed
                rreq_setInitStateCompleted(index, RREQ_STATE_INIT_READ_CHAR_FEATURE);

                // After reading the feature characteristic value, proceed to register to the characteristics based on the features and config preferred mode
                status = rreq_registerConfigChars(index);
            }

            if (status != SUCCESS)
            {
                rreq_handleStateInitError(index, status);
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
 * @fn      rreq_handleErrorRsp
 *
 * @brief   Handles error response GATT event.
 *
 * This function processes the @ref BLEAPPUTIL_ATT_ERROR_RSP event
 * and abort the current active procedure.
 * Processing depends on the current procedure state.
 * The relevant states are:
 * @ref RREQ_STATE_INIT_DISCOVER_PRIM_SERVICE
 * @ref RREQ_STATE_INIT_DISCOVER_ALL_CHARS
 * @ref RREQ_STATE_INIT_DISCOVER_ALL_CHAR_DESCS
 * @ref RREQ_STATE_INIT_READ_CHAR_FEATURE
 * @ref RREQ_STATE_INIT_REGISTERING
 * @ref RREQ_STATE_REGISTERING
 *
 * input parameters
 *
 * @param   index   - Index of the connection info
 * @param   gattMsg - pointer to the GATT message event structure
 *
 * output parameters
 *
 * @param   None
 *
 * @return  none
 */
static void rreq_handleErrorRsp(uint8_t index, gattMsgEvent_t *gattMsg)
{
    attErrorRsp_t pErrorRsp;

    if (gattMsg == NULL || index >= RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        return;
    }

    pErrorRsp = gattMsg->msg.errorRsp;

    switch (RREQ_CURRENT_STATE(index))
    {
        case RREQ_STATE_INIT_DISCOVER_PRIM_SERVICE:
        case RREQ_STATE_INIT_DISCOVER_ALL_CHARS:
        case RREQ_STATE_INIT_DISCOVER_ALL_CHAR_DESCS:
        case RREQ_STATE_INIT_READ_CHAR_FEATURE:
        case RREQ_STATE_INIT_REGISTERING:
        {
            rreq_handleStateInitError(index, pErrorRsp.errCode);

            break;
        }
        case RREQ_STATE_REGISTERING:
        {
            rreq_handlePostRegistrationState(index, pErrorRsp.errCode, true);
            break;
        }

        // Handle other states if needed
        default:
        {
            break;
        }
    }
}

/*********************************************************************
 * @fn      rreq_handleStateInitError
 *
 * @brief   Handles initialization error during RREQ initialization procedure.
 *
 * This function is called when an error occurs during the initialization
 * procedure of the RREQ. It clears the current registering characteristic
 * information, updates the procedure state back to IDLE, and notifies
 * the application about the initialization failure.
 *
 * input parameters
 *
 * @param   index - Index of the connection info.
 * @param   error - Error code indicating the reason for failure.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  none
 */
static void rreq_handleStateInitError(uint8_t index, uint8_t error)
{
    if (index < RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        // Clear current registering char info
        rreq_clearCurrentConfiguredCharInfo(index);

        // Update state back to IDLE
        rreq_setProcedureState(index, RREQ_STATE_IDLE);

        // Notify application about init failure
        rreq_sendStatusCB(RREQ_INDEX_CONN_HANDLE(index), RREQ_INIT_FAILED, sizeof(error), &error);
    }
}

/*********************************************************************
 * @fn      rreq_handlePostRegistrationState
 *
 * @brief   Handles post-registration state for RREQ characteristic registration
 *          procedure executed by @ref RREQ_ConfigureCharRegistration API.
 *
 * This function is called after attempting to register or unregister
 * a characteristic. It updates the subscription bitmap based on the
 * registration result, adjusts the enable mode and procedure state,
 * clears the current registering characteristic information, and notifies
 * the application about the registration outcome.
 *
 * input parameters
 *
 * @param   index               - Index of the connection info.
 * @param   status              - Status of the registration attempt.
 * @param   isRejectedByServer  - Indicates if the registration was rejected by
 *                                the server or failed internally.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  none
 */
static void rreq_handlePostRegistrationState(uint8_t index, uint8_t status, bool isRejectedByServer)
{
    if (index < RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        // If the configuration set to registering - set bit in subscribe bitmap
        if (status == SUCCESS &&
            (gRREQControlBlock.connInfo[index].currentConfiguredCharSubtype == RREQ_PREFER_NOTIFY ||
             gRREQControlBlock.connInfo[index].currentConfiguredCharSubtype == RREQ_INDICATE))
        {
            RREQ_SET_SUBSCRIBE_BIT(gRREQControlBlock.connInfo[index].subscribeBitMap,
                                    gRREQControlBlock.connInfo[index].currentConfiguredCharUUID);
        }

        // If the configuration set to unregistering, no matter success or failure - we consider
        // the characteristic as unregistered, so clear its bit from the subscribe bitmap
        if (gRREQControlBlock.connInfo[index].currentConfiguredCharSubtype == RREQ_DISABLE_NOTIFY_INDICATE)
        {
            RREQ_CLEAR_SUBSCRIBE_BIT(gRREQControlBlock.connInfo[index].subscribeBitMap,
                                     gRREQControlBlock.connInfo[index].currentConfiguredCharUUID);
        }

        /* Update the enable mode and state depends on current registration */

        if (rreq_checkRegistrationOnDemand(index))
        {
            // If On-Demand is registered, set enable mode and state accordingly
            gRREQControlBlock.connInfo[index].enableMode = RREQ_MODE_ON_DEMAND;
            rreq_setProcedureState(index, RREQ_STATE_READY);
        }
        else if (rreq_checkRegistrationRealTime(index))
        {
            // If Real-Time is registered, set enable mode and state accordingly
            gRREQControlBlock.connInfo[index].enableMode = RREQ_MODE_REAL_TIME;
            rreq_setProcedureState(index, RREQ_STATE_READY);
        }
        else
        {
            // neither mode is fully registered - set mode to NONE and state to IDLE
            gRREQControlBlock.connInfo[index].enableMode = RREQ_MODE_NONE;
            rreq_setProcedureState(index, RREQ_STATE_IDLE);
        }

        // Clear current registering char info
        rreq_clearCurrentConfiguredCharInfo(index);

        // If we got here through ATT_ERROR_RSP
        if (isRejectedByServer)
        {
            // The registration was rejected by the server - status represents the error code.
            // Notify application about rejection
            rreq_sendStatusCB(RREQ_INDEX_CONN_HANDLE(index), RREQ_CHAR_CONFIGURATION_REJECTED, sizeof(status), &status);
        }
        else
        {
            // We got here through ATT_WRITE_RSP, notify application about success or failure
            if (status == SUCCESS)
            {
                rreq_sendStatusCB(RREQ_INDEX_CONN_HANDLE(index), RREQ_CHAR_CONFIGURATION_DONE, 0, NULL);
            }
            else
            {
                rreq_sendStatusCB(RREQ_INDEX_CONN_HANDLE(index), RREQ_CHAR_CONFIGURATION_FAILED, sizeof(status), &status);
            }
        }
    }
}

/***********************************************************************
 * @fn    rreq_checkRegistration
 *
 * @brief This function determines whether a required subscription
 *        (notification or indication) is registered based on the provided
 *        subscription bitmap.
 *
 * @note  Not relevant for @ref RAS_FEATURE_UUID
 *
 * @param subscribeBitMap - Bitmap representing enabled subscriptions.
 * @param uuid            - UUID of the characteristic to check.
 *
 * @return true  - If the required subscription is registered.
 * @return false - If the required subscription is not registered
 *                 or if the UUID is out of range.
 */
static bool rreq_checkRegistration(uint8_t subscribeBitMap, uint16_t uuid)
{
    uint8_t subscribeBit;

    if (uuid < RAS_REAL_TIME_UUID || uuid > RAS_DATA_OVERWRITTEN_UUID)
    {
        return false;
    }

    subscribeBit = RREQ_UUID_TO_CHAR_SUBSCRIBE_BIT(uuid);

    if ((subscribeBitMap & subscribeBit) == 0)
    {
        return false;
    }

    return true;
}

/***********************************************************************
 * @fn    rreq_checkConfigRegistration
 *
 * @brief This function checks if the required subscription
 *        (notification or indication) is registered for a given
 *        index, characteristic UUID, depends on a
 *        configuration subtype.
 *
 * @note  Not relevant for @ref RAS_FEATURE_UUID
 *
 * @param index          - Index to work on.
 * @param uuid           - UUID of the characteristic to check.
 * @param configSubType  - Configuration subtype.
 *
 * @return true  - If the required subscription is registered or
 *                 not needed (depends on configSubType).
 * @return false - If the required subscription is not registered,
 *                 or if the parameters are out of range.
 */
static bool rreq_checkConfigRegistration(uint8_t index, uint16_t uuid, RREQConfigSubType_e configSubType)
{
    if(index < RANGING_DB_CLIENT_MAX_NUM_PROC ||
       uuid < RAS_REAL_TIME_UUID ||
       uuid > RAS_DATA_OVERWRITTEN_UUID)
    {
        if (configSubType == RREQ_PREFER_NOTIFY ||
            configSubType == RREQ_INDICATE)
        {
            // Check if the required subscription is registered
            return rreq_checkRegistration(gRREQControlBlock.connInfo[index].subscribeBitMap, uuid);
        }
        else if (configSubType == RREQ_DISABLE_NOTIFY_INDICATE)
        {
            // If the configuration is to disable notification/indication,
            // consider it valid (no need for subscription)
            return true;
        }
    }

    return false;
}

/***********************************************************************
 * @fn    rreq_checkRegistrationOnDemand
 *
 * @brief This function checks if all required subscriptions are
 *        registered for On-Demand mode.
 *
 * @param index - Index to work on.
 *
 * @return true  - If all required subscriptions are registered.
 * @return false - If any required subscription is not registered
 *                 or if the given index is out of range.
 */
static bool rreq_checkRegistrationOnDemand(uint8_t index)
{
    if(index < RANGING_DB_CLIENT_MAX_NUM_PROC &&
       rreq_checkRegistration(gRREQControlBlock.connInfo[index].subscribeBitMap, RAS_CONTROL_POINT_UUID) &&
       rreq_checkRegistration(gRREQControlBlock.connInfo[index].subscribeBitMap, RAS_DATA_READY_UUID) &&
       rreq_checkRegistration(gRREQControlBlock.connInfo[index].subscribeBitMap, RAS_ON_DEMAND_UUID) &&
       rreq_checkRegistration(gRREQControlBlock.connInfo[index].subscribeBitMap, RAS_DATA_OVERWRITTEN_UUID))
    {
        return true;
    }

    return false;
}

/***********************************************************************
 * @fn    rreq_checkRegistrationRealTime
 *
 * @brief This function checks if the required subscription is
 *        registered for Real-Time mode.
 *
 * @param index - Index to work on.
 *
 * @return true  - If the required subscription is registered.
 * @return false - If the required subscription is not registered
 *                 or if the given index is out of range.
 */
static bool rreq_checkRegistrationRealTime(uint8_t index)
{
    if (index < RANGING_DB_CLIENT_MAX_NUM_PROC &&
        rreq_checkRegistration(gRREQControlBlock.connInfo[index].subscribeBitMap, RAS_REAL_TIME_UUID))
    {
        return true;
    }

    return false;
}

/*********************************************************************
 * @fn      rreq_getConfigModeToEnable
 *
 * @brief   Determines the mode to enable (On-Demand or Real-Time)
 *          based on application preference given in @ref RREQ_Enable API
 *          and server features.
 *
 * input parameters
 *
 * @param   index - Index to work on.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  RREQ_MODE_ON_DEMAND - On-Demand mode should be enabled.
 *          RREQ_MODE_REAL_TIME - Real-Time mode should be enabled.
 *          RREQ_MODE_NONE      - Index is invalid.
 */
static RREQEnableModeType_e rreq_getConfigModeToEnable(uint8_t index)
{
    RREQEnableModeType_e modeToEnable = RREQ_MODE_NONE;

    if (index < RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        // Set the mode depends on the application preference and the server features
        if ((gRREQControlBlock.connInfo[index].featureCharValue & RAS_FEATURES_REAL_TIME) &&
            gRREQControlBlock.connInfo[index].preferredMode == RREQ_MODE_REAL_TIME)
        {
            modeToEnable = RREQ_MODE_REAL_TIME;
        }
        else
        {
            // Whether the application preferred to use On-Demand, or the server doesn't
            // support Real-Time - use On-Demand
            modeToEnable = RREQ_MODE_ON_DEMAND;
        }
    }

    return modeToEnable;
}

/*********************************************************************
 * @fn      rreq_registerConfigChars
 *
 * @brief Registers characteristics for the RREQ,
 * using the config set by @ref RREQ_Start API.
 * This function is responsible for registering the characteristics
 * that the RREQ will use to communicate with the server.
 * It ensures that the necessary characteristics
 * are properly initialized and made available for use.
 * The characteristic that will be registered depends on the mode
 * that has been chosen - @ref RREQ_MODE_ON_DEMAND or @ref RREQ_MODE_REAL_TIME
 *
 * @param index - Index associated with the connection handle for which
 *                to register the characteristics.
 *
 * @return INVALIDPARAMETER - if the index is invalid
 * @return bleIncorrectMode - if the client is not registered to
 *                            either On-Demand or Real-Time characteristics.
 * @return SUCCESS          - otherwise.
 */
static bStatus_t rreq_registerConfigChars(uint8_t index)
{
    bStatus_t status = SUCCESS;
    RREQEnableModeType_e modeToEnable;

    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        status = INVALIDPARAMETER;
    }

    if (status == SUCCESS)
    {
        modeToEnable = rreq_getConfigModeToEnable(index);

        if (modeToEnable == RREQ_MODE_ON_DEMAND)
        {
            status = rreq_registerOnDemandConfigChars(index);
        }
        else if(modeToEnable == RREQ_MODE_REAL_TIME)
        {
            status = rreq_registerRealTimeConfigChars(index);
        }
        else
        {
            status = bleIncorrectMode;
        }
    }

    return status;
}

/*********************************************************************
 * @fn      rreq_registerOnDemandConfigChars
 *
 * @brief Registers On-Demand characteristics for the RREQ,
 * using the config set by @ref RREQ_Start API.
 * This function is responsible for registering the characteristics
 * that the RREQ will use to communicate with the server.
 * It ensures that the necessary characteristics
 * are properly initialized and made available for use.
 *
 * @param index - Index associated with the connection handle for which
 *                to register the On-Demand characteristics.
 *
 * @return INVALIDPARAMETER - if the index is invalid or the global
 *                            configuration is NULL.
 * @return SUCCESS          - otherwise.
 */
static bStatus_t rreq_registerOnDemandConfigChars(uint8_t index)
{
    bStatus_t status = SUCCESS;

    // Check parameters and configuration
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        gRREQControlBlock.config == NULL)
    {
        status = INVALIDPARAMETER;
    }

    if (status == SUCCESS)
    {
        // Clear current registering char info
        rreq_clearCurrentConfiguredCharInfo(index);

        // Register to the "control point" characteristic handle
        if (gRREQControlBlock.config->onDemandSubConfig.controlPointSubType != RREQ_DISABLE_NOTIFY_INDICATE &&
            (gRREQControlBlock.connInfo[index].subscribeBitMap & RREQ_CONTROL_POINT_BIT) == 0)
        {
            status = rreq_configureCharRegistration(index,
                                                    RAS_CONTROL_POINT_UUID,
                                                    gRREQControlBlock.config->onDemandSubConfig.controlPointSubType);
        }

        // Register to the "data ready" characteristic handle
        else if (gRREQControlBlock.config->onDemandSubConfig.dataReadySubType != RREQ_DISABLE_NOTIFY_INDICATE &&
                 (gRREQControlBlock.connInfo[index].subscribeBitMap & RREQ_DATA_READY_BIT) == 0)
        {
            status = rreq_configureCharRegistration(index,
                                                    RAS_DATA_READY_UUID,
                                                    gRREQControlBlock.config->onDemandSubConfig.dataReadySubType);
        }

        // Register to the "data overwritten" characteristic handle
        else if (gRREQControlBlock.config->onDemandSubConfig.overwrittenSubType != RREQ_DISABLE_NOTIFY_INDICATE &&
                 (gRREQControlBlock.connInfo[index].subscribeBitMap & RREQ_OVERWRITTEN_BIT) == 0)
        {
            status = rreq_configureCharRegistration(index,
                                                    RAS_DATA_OVERWRITTEN_UUID,
                                                    gRREQControlBlock.config->onDemandSubConfig.overwrittenSubType);
        }

        // Register to the "on-demand" characteristic handle
        else if (gRREQControlBlock.config->onDemandSubConfig.onDemandSubType != RREQ_DISABLE_NOTIFY_INDICATE &&
                 (gRREQControlBlock.connInfo[index].subscribeBitMap & RREQ_ON_DEMAND_BIT) == 0)
        {
            status = rreq_configureCharRegistration(index,
                                                    RAS_ON_DEMAND_UUID,
                                                    gRREQControlBlock.config->onDemandSubConfig.onDemandSubType);
        }

        if (status == SUCCESS)
        {
            // Update state
            rreq_setProcedureState(index, RREQ_STATE_INIT_REGISTERING);
        }

        if (rreq_checkConfigRegistration(index, RAS_CONTROL_POINT_UUID, gRREQControlBlock.config->onDemandSubConfig.controlPointSubType) &&
            rreq_checkConfigRegistration(index, RAS_DATA_READY_UUID, gRREQControlBlock.config->onDemandSubConfig.dataReadySubType) &&
            rreq_checkConfigRegistration(index, RAS_ON_DEMAND_UUID, gRREQControlBlock.config->onDemandSubConfig.onDemandSubType) &&
            rreq_checkConfigRegistration(index, RAS_DATA_OVERWRITTEN_UUID, gRREQControlBlock.config->onDemandSubConfig.overwrittenSubType))
        {
            // If all relevant characteristics for on-demand mode are registered
            if (rreq_checkRegistrationOnDemand(index))
            {
                // Set enable mode
                gRREQControlBlock.connInfo[index].enableMode = RREQ_MODE_ON_DEMAND;

                // Done registering to all relevant characteristics, set state to Ready
                rreq_setProcedureState(index, RREQ_STATE_READY);
            }
            else
            {
                // Not all relevant characteristics are registered, considered as NONE mode
                gRREQControlBlock.connInfo[index].enableMode = RREQ_MODE_NONE;

                // Not all relevant characteristics are registered - set state to IDLE
                rreq_setProcedureState(index, RREQ_STATE_IDLE);
            }

            // Notify application that init is done
            rreq_sendStatusCB(RREQ_INDEX_CONN_HANDLE(index), RREQ_INIT_DONE, 0, NULL);
        }
    }

    return status;
}

/*********************************************************************
 * @fn      rreq_registerRealTimeConfigChars
 *
 * @brief Registers Real-Time characteristics for the RREQ,
 * using the config set by @ref RREQ_Start API.
 * This function is responsible for registering the characteristics
 * that the RREQ will use to communicate with the server.
 * It ensures that the necessary characteristics
 * are properly initialized and made available for use.
 *
 * @param index - Index associated with the connection handle for which
 *                to register the Real-Time characteristics.
 *
 * @return INVALIDPARAMETER - If the index is invalid or the global
 *                            configuration is NULL.
 * @return SUCCESS          - otherwise.
 */
static bStatus_t rreq_registerRealTimeConfigChars(uint8_t index)
{
    bStatus_t status = SUCCESS;

    // Check parameters and configuration
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        gRREQControlBlock.config == NULL)
    {
        status = INVALIDPARAMETER;
    }

    if (status == SUCCESS)
    {
        // Clear current registering char info
        rreq_clearCurrentConfiguredCharInfo(index);

        // Register to the "Real-Time" characteristic handle
        if(gRREQControlBlock.config->realTimeSubConfig.realTimeSubType != RREQ_DISABLE_NOTIFY_INDICATE &&
           (gRREQControlBlock.connInfo[index].subscribeBitMap & RREQ_REAL_TIME_BIT) == 0)
        {
            status = rreq_configureCharRegistration(index,
                                                    RAS_REAL_TIME_UUID,
                                                    gRREQControlBlock.config->realTimeSubConfig.realTimeSubType);
        }

        if (status == SUCCESS)
        {
            // Update state
            rreq_setProcedureState(index, RREQ_STATE_INIT_REGISTERING);
        }

        if (rreq_checkConfigRegistration(index, RAS_REAL_TIME_UUID, gRREQControlBlock.config->realTimeSubConfig.realTimeSubType))
        {
            // If all relevant characteristics for real-time mode are registered
            if(rreq_checkRegistrationRealTime(index))
            {
                // Set enable mode
                gRREQControlBlock.connInfo[index].enableMode = RREQ_MODE_REAL_TIME;

                // Done registering to all relevant characteristics, set state to Ready
                rreq_setProcedureState(index, RREQ_STATE_READY);
            }
            else
            {
                // Not all relevant characteristics are registered, considered as NONE mode
                gRREQControlBlock.connInfo[index].enableMode = RREQ_MODE_NONE;

                // Not all relevant characteristics are registered - set state to IDLE
                rreq_setProcedureState(index, RREQ_STATE_IDLE);
            }

            // Notify application
            rreq_sendStatusCB(RREQ_INDEX_CONN_HANDLE(index), RREQ_INIT_DONE, 0, NULL);
        }
    }

    return status;
}

/*********************************************************************
 * @fn      rreq_configureCharRegistration
 *
 * @brief Configures the registration for a characteristic in the Ranging Profile Client.
 *
 * This function registers or unregisters for notifications or indications on a specific
 * characteristic identified by its UUID for a given index.
 *
 * @param index    - The index associated with the connection handle for the BLE connection.
 * @param charUUID - The UUID of the characteristic to configure.
 * @param subType  - The type of configuration to apply (see RREQConfigSubType_e).
 *
 * @return SUCCESS          - if the configuration was successful.
 * @return INVALIDPARAMETER - if the index is invalid, the characteristic
 *                            UUID is not supported, the characteristic is already
 *                            registered\unregistered, or if trying to register to both
 *                            Real-Time and On-Demand characteristics.
 *
 * @note Does not accept RAS_FEATURE_UUID for configuration.
 */
static bStatus_t rreq_configureCharRegistration(uint8_t index, uint16_t charUUID, RREQConfigSubType_e subType)
{
    bStatus_t status = INVALIDPARAMETER;

    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        charUUID < RAS_REAL_TIME_UUID ||
        charUUID > RAS_DATA_OVERWRITTEN_UUID ||
        subType >= RREQ_SUBTYPE_INVALID)
    {
        return status;
    }

    switch(subType)
    {
        case RREQ_DISABLE_NOTIFY_INDICATE:
        {
            status = rreq_unregisterCharacteristic(index, charUUID);

            break;
        }
        case RREQ_PREFER_NOTIFY:
        case RREQ_INDICATE:
        {
            // Check if trying to register to both Real-Time and On-Demand characteristics
            if ((charUUID == RAS_REAL_TIME_UUID &&
                 (gRREQControlBlock.connInfo[index].subscribeBitMap & RREQ_UUID_TO_CHAR_SUBSCRIBE_BIT(RAS_ON_DEMAND_UUID)) != 0) ||
                (charUUID == RAS_ON_DEMAND_UUID &&
                 (gRREQControlBlock.connInfo[index].subscribeBitMap & RREQ_UUID_TO_CHAR_SUBSCRIBE_BIT(RAS_REAL_TIME_UUID)) != 0))
            {
                status = INVALIDPARAMETER;
            }
            else
            {
                status = rreq_registerCharacteristic(index, charUUID, subType);
            }

            break;
        }
        default:
        {
            break;
        }
    }

    if (status == SUCCESS)
    {
        // Set current configured char uuid and subtype
        rreq_setCurrentConfiguredCharInfo(index, charUUID, subType);
    }

    return status;
}

/*********************************************************************
 * @fn      rreq_registerCharacteristic
 *
 * @brief Registers a characteristic for the ranging request profile client.
 *
 * This function registers a characteristic with the specified UUID on the given index,
 * configuring it according to the provided mode.
 *
 * @param index    - The index associated with the connection handle for the BLE connection.
 * @param charUUID - The UUID of the characteristic to register.
 * @param mode     - The configuration mode (of type RREQConfigSubType_e) for the characteristic.
 *
 * @return SUCCESS - if the configuration was successful.
 *         INVALIDPARAMETER - if the index is invalid or the characteristic
 *                            UUID is not supported.
 *
 * @note Does not accept RAS_FEATURE_UUID for configuration.
 */
static bStatus_t rreq_registerCharacteristic(uint8_t index, uint16_t charUUID, RREQConfigSubType_e mode)
{
    bStatus_t status = INVALIDPARAMETER;
    RREQConfigSubType_e charSubType;
    uint8_t subscribeBit;
    RREQ_CharInfo_t charInfo;

    if (index < RANGING_DB_CLIENT_MAX_NUM_PROC &&
        mode < RREQ_SUBTYPE_INVALID &&
        charUUID >= RAS_REAL_TIME_UUID &&
        charUUID <= RAS_DATA_OVERWRITTEN_UUID)
    {
        subscribeBit = RREQ_UUID_TO_CHAR_SUBSCRIBE_BIT(charUUID);
        charInfo = gRREQControlBlock.connInfo[index].charInfo[RREQ_UUID_TO_CHAR_INDEX(charUUID)];

        if ((gRREQControlBlock.connInfo[index].subscribeBitMap & subscribeBit) == 0)
        {
            // Get the characteristic properties mode
            charSubType = rreq_getCharProperties(mode, charInfo.charProperties);
            // Enable notifications/indications for the characteristic
            status = rreq_enableNotification(index, charSubType, charInfo.charHandle);
        }
    }

    return status;
}

/*********************************************************************
 * @fn      rreq_unregisterCharacteristic
 *
 * @brief Unregisters a characteristic for a given index and characteristic UUID.
 *
 * This function removes the registration of a characteristic, identified by its UUID,
 * from the specified index. After unregistration, notifications or indications
 * for this characteristic will no longer be received for the given index.
 *
 * @param index    - The index associated with the connection handle for the BLE connection.
 * @param charUUID - The UUID of the characteristic to unregister.
 *
 * @return INVALIDPARAMETER - Index or char UUID are invalid.
 * @return SUCCESS - The command was sent successfully.
 * @return Other status derived by @ref GATT_WriteCharValue otherwise
 *
 * @note Does not accept RAS_FEATURE_UUID for unregistration.
 */
static bStatus_t rreq_unregisterCharacteristic(uint8_t index, uint16_t charUUID)
{
    bStatus_t status = INVALIDPARAMETER;
    uint8_t subscribeBit;
    uint16_t charHandle;

    if (index < RANGING_DB_CLIENT_MAX_NUM_PROC &&
        charUUID >= RAS_REAL_TIME_UUID &&
        charUUID <= RAS_DATA_OVERWRITTEN_UUID)
    {
        subscribeBit = RREQ_UUID_TO_CHAR_SUBSCRIBE_BIT(charUUID);
        charHandle = gRREQControlBlock.connInfo[index].charInfo[RREQ_UUID_TO_CHAR_INDEX(charUUID)].charHandle;

        if ((gRREQControlBlock.connInfo[index].subscribeBitMap & subscribeBit) != 0)
        {
            // Unregister peer notifications/indications
            status = rreq_enableNotification(index, RREQ_DISABLE_NOTIFY_INDICATE, charHandle);
        }
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
 * @param index     - Index identifying the BLE connection.
 * @param gattMsg   - Pointer to the GATT message event structure containing
 *                    the response data.
 */
static void rreq_handleValueNoti(uint8_t index, attHandleValueNoti_t *handleValueNoti)
{
    if (index < RANGING_DB_CLIENT_MAX_NUM_PROC &&
        handleValueNoti != NULL)
    {
        if (gRREQControlBlock.connInfo[index].enableMode == RREQ_MODE_ON_DEMAND)
        {
            rreq_handleOnDemandValueNoti(index, handleValueNoti);
        }
        else if(gRREQControlBlock.connInfo[index].enableMode == RREQ_MODE_REAL_TIME)
        {
            rreq_handleRealTimeValueNoti(index, handleValueNoti);
        }
        else
        {
            // Don't do anything
        }
    }
}

/*********************************************************************
 * @fn      rreq_handleOnDemandValueNoti
 *
 * @brief Handles on-demand value notification for the ranging profile client.
 *
 * This function processes incoming ATT Handle Value Notification messages
 * related to on-demand values for a specific index. It is typically
 * invoked when a notification is received from the server, allowing the client
 * to react to updated ranging data or status.
 *
 * @param index             - Index identifying the BLE connection.
 * @param handleValueNoti   - Pointer to the ATT Handle Value Notification structure
 *                            containing the notification data.
 */
static void rreq_handleOnDemandValueNoti(uint8_t index, attHandleValueNoti_t *handleValueNoti)
{
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC || handleValueNoti == NULL)
    {
        return;
    }

    // if get notification from "on-demand" characteristic handle
    if(handleValueNoti->handle == gRREQControlBlock.connInfo[index].charInfo[RREQ_ON_DEMAND_CHAR_INDEX].charHandle)
    {
        rreq_handleOnDemandSegmentReceived(index, handleValueNoti);
    }

    // if get notification from "control point" characteristic handle
    else if(handleValueNoti->handle == gRREQControlBlock.connInfo[index].charInfo[RREQ_CONTROL_POINT_CHAR_INDEX].charHandle)
    {
        rreq_handleControlPointNotification(index, handleValueNoti);
    }

    // if get notification from "data ready" characteristic handle
    else if(handleValueNoti->handle == gRREQControlBlock.connInfo[index].charInfo[RREQ_DATA_READY_CHAR_INDEX].charHandle)
    {
        rreq_handleDataReady(index, handleValueNoti);
    }

    // if get notification from "data overwritten" characteristic handle
    else if(handleValueNoti->handle == gRREQControlBlock.connInfo[index].charInfo[RREQ_DATA_OVERWRITTEN_CHAR_INDEX].charHandle)
    {
        // send OverWritten status to App
        uint16_t rangingCounter = BUILD_UINT16(handleValueNoti->pValue[0], handleValueNoti->pValue[1]);
        rreq_sendStatusCB(RREQ_INDEX_CONN_HANDLE(index), RREQ_DATA_OVERWRITTEN, RANGING_COUNTER_LEN, (uint8_t*)&rangingCounter);
    }
}

/*********************************************************************
 * @fn      rreq_handleRealTimeValueNoti
 *
 * @brief Handles real-time value notification for the ranging profile client.
 *
 * This function processes incoming ATT Handle Value Notification messages
 * related to real-time values from the server. It is typically called when
 * a notification is received on the corresponding characteristic.
 *
 * @param index             - Index identifying the BLE connection.
 * @param handleValueNoti   - Pointer to the ATT Handle Value Notification structure
 *                            containing the notification data.
 */
static void rreq_handleRealTimeValueNoti(uint8_t index, attHandleValueNoti_t *handleValueNoti)
{
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC || handleValueNoti == NULL)
    {
        return;
    }

    // if get notification from "Real-Time" characteristic handle
    if(handleValueNoti->handle == gRREQControlBlock.connInfo[index].charInfo[RREQ_REAL_TIME_CHAR_INDEX].charHandle )
    {
        rreq_handleRealTimeSegmentReceived(index, handleValueNoti);
    }
}

/*********************************************************************
 * @fn    rreq_handleControlPointNotification
 *
 * @brief Handles the control point indications
 *
 * @param index           - The index associated with the connection handle.
 * @param handleValueNoti - Pointer to the handle value indication structure.
 *
 * @return None.
 */
void rreq_handleControlPointNotification(uint8_t index, attHandleValueNoti_t *handleValueNoti)
{
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC || handleValueNoti == NULL ||
        gRREQControlBlock.connInfo[index].enableMode != RREQ_MODE_ON_DEMAND)
    {
        return;
    }

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
                // Expect the Complete Data Response only when waiting for next segment
                if (RREQ_CURRENT_STATE(index) == RREQ_STATE_WAIT_FOR_NEXT_SEGMENT)
                {
                    // RangingCounter received from CompleteData msg
                    uint16_t CompleteDataRangingCounter = BUILD_UINT16(handleValueNoti->pValue[1], handleValueNoti->pValue[2]);

                    // Check that the received rangingCounter is the expected one.
                    if (gRREQControlBlock.connInfo[index].procedureAttr.rangingCounter == CompleteDataRangingCounter)
                    {
                        // Send an ack to the server anyway due to the fact that there is no support in Get Lost Segments
                        rreq_sendAck(index);
                    }
                }

                break;
            }
            case RAS_CP_OPCODE_RSP_CODE:
            {
                // Notify App
                uint8_t rspValue = handleValueNoti->pValue[1];
                rreq_handleRspCode(index, rspValue);
                break;
            }
            default:
                // Unknown command, do nothing
                break;
        }
    }
}

/*********************************************************************
 * @fn    rreq_handleDataReady
 *
 * @brief This function handles the "data ready" notification
 *        received from the server. It extracts the ranging counter
 *        from the notification and calls the application callback
 *        to notify that new data is ready.
 *
 * @param  index           - The index associated with the connection handle.
 * @param  handleValueNoti - Pointer to the handle value notification structure.
 *
 * @return None.
 *
 */
static void rreq_handleDataReady(uint8_t index, attHandleValueNoti_t *handleValueNoti)
{
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        handleValueNoti == NULL                 ||
        handleValueNoti->len < 2                ||
        RREQ_CURRENT_STATE(index) != RREQ_STATE_WAIT_FOR_DATA_READY)
    {
        return;
    }

    // Get from the notification the ranging counter
    uint8_t *pData = handleValueNoti->pValue;
    uint16_t rangingCounter = BUILD_UINT16(pData[0], pData[1]);

    // Ensure the current CS procedure matches the received ranging counter
    if (rangingCounter == gRREQControlBlock.connInfo[index].currentProcedureCounter)
    {
        // Stop the timer
        rreq_stopTimer(index);

        // Get back to Ready
        rreq_setProcedureState(index, RREQ_STATE_READY);

        if((gRREQControlBlock.callbacks != NULL) && (gRREQControlBlock.callbacks->pDataReadyCallback != NULL))
        {
            // Call pDataReadyCallback function
            gRREQControlBlock.callbacks->pDataReadyCallback(RREQ_INDEX_CONN_HANDLE(index), rangingCounter);
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
 * @param  index - The index associated with the connection handle.
 *
 * @return None.
 *
 */
static void rreq_procedureDone(uint8_t index)
{
    bStatus_t status = SUCCESS;
    RangingDBClient_procedureSegmentsReader_t segmentsReader;
    uint16_t rangingCounterTemp;
    uint8_t lastSegmentFlagTemp;

    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        return;
    }

    // Save temporary values of the current db so we can call the callback after resetting the procedure,
    // so in case the app calls another RREQ function from the callback, the procedure attributes will be valid
    rangingCounterTemp  = gRREQControlBlock.connInfo[index].procedureAttr.rangingCounter;
    lastSegmentFlagTemp = gRREQControlBlock.connInfo[index].lastSegmentFlag;

    // Clear last segment flag
    rreq_clearLastSegmentFlag(index);
    // Stop any active timer
    rreq_stopTimer(index);

    // Reset Procedure counter
    gRREQControlBlock.connInfo[index].procedureAttr.rangingCounter = 0xFFFF;

    // Reset procedure state to READY
    rreq_setProcedureState(index, RREQ_STATE_READY);

    // Notify APP ( Data Complete )
    if((gRREQControlBlock.callbacks != NULL) && (gRREQControlBlock.callbacks->pDataCompleteEventCallback != NULL))
    {
        // Check if we received the last segment
        if (lastSegmentFlagTemp != TRUE)
        {
            status = RREQ_DATA_INVALID;
        }

        if(status == SUCCESS)
        {
            // Get the data from the database
            status = RangingDBClient_getData(gRREQControlBlock.connInfo[index].rangingDbHandle, &segmentsReader);
        }

        // If status is not SUCCESS, the data was not retrieved from the DB (ownership not transferred),
        // so we need to clear the DB to avoid memory leak
        if (status != SUCCESS)
        {
            RangingDBClient_clearProcedure(gRREQControlBlock.connInfo[index].rangingDbHandle);
        }

        // Call the data complete callback when the procedure is done in any case, to notify the end of the current procedure.
        // Note: Callback is called last to ensure all internal cleanup is done before notifying the application.
        gRREQControlBlock.callbacks->pDataCompleteEventCallback(RREQ_INDEX_CONN_HANDLE(index), rangingCounterTemp, status, segmentsReader);

        // If status is not SUCCESS, the data was not retrieved from the DB (ownership not transferred),
        // so we need to clear the DB to avoid memory leak
        if (status != SUCCESS)
        {
            RangingDBClient_clearProcedure(gRREQControlBlock.connInfo[index].rangingDbHandle);
        }
    }
    else
    {
        // If there is no callback - clear the data from the database
        RangingDBClient_clearProcedure(gRREQControlBlock.connInfo[index].rangingDbHandle);
    }
}

/*********************************************************************
 * @fn    rreq_sendAck
 *
 * @brief Sends and ack as a response for a @ref RAS_CP_OPCODE_COMPLETE_DATA_RSP
 *
 * This function sends an ack to the server, and starts a timer to wait for
 * the response code.
 * If blePending is returned by the GATT api, the function will set the state to
 * RREQ_STATE_SEND_ACK, to try sending the ack again when possible.
 *
 * If status other than SUCCESS or blePending is returned by the GATT api,
 * the function will set the state to READY as it is an unexpected status.
 *
 * @param  index - The index associated with the connection handle.
 *
 * @return None.
 *
 */
static void rreq_sendAck(uint8_t index)
{
    bStatus_t status;

    // Stop the timer, if any
    rreq_stopTimer(index);

    // Send Ack to the server
    status = rreq_sendControlPointWriteCmd(index,
                                           RAS_CP_OPCODE_ACK_RANGING_DATA,
                                           gRREQControlBlock.connInfo[index].procedureAttr.rangingCounter,
                                           RAS_CP_ACK_DATA_CMD_LEN);

    // If the command was sent successfully
    if (status == SUCCESS)
    {
        // Wait for the response code
        rreq_setProcedureState(index, RREQ_STATE_WAIT_FOR_CONTROL_POINT_RSP);

        // Start timer to wait for Rsp Code
        rreq_startTimer(gRREQControlBlock.config->timeoutConfig.timeOutControlPointRsp, index);
    }
    else if (status == blePending)
    {
        // Command not send - register to L2CAP events if not already registered and try
        // again when @ref BLEAPPUTIL_L2CAP_NUM_CTRL_DATA_PKT_EVT is received
        if (gRREQControlBlock.l2capRegistered == TRUE ||
            (gRREQControlBlock.l2capRegistered == FALSE &&
             BLEAppUtil_registerEventHandler(&gRREQL2CAPHandler) == SUCCESS))
        {
            // Mark that L2CAP is registered
            gRREQControlBlock.l2capRegistered = TRUE;

            // Update state to send ack when possible
            rreq_setProcedureState(index, RREQ_STATE_SEND_ACK);
        }
        else
        {
            // Unexpected error
            status = FAILURE;
        }
    }

    if (status != SUCCESS && status != blePending)
    {
        // Unexpected error - get back to READY
        rreq_setProcedureState(index, RREQ_STATE_READY);

        // Notify application of a failure during getting ranging data
        if((gRREQControlBlock.callbacks != NULL) && (gRREQControlBlock.callbacks->pDataCompleteEventCallback != NULL))
        {
            RangingDBClient_procedureSegmentsReader_t segmentsReader = {0};

            gRREQControlBlock.callbacks->pDataCompleteEventCallback(RREQ_INDEX_CONN_HANDLE(index),
                                                                    gRREQControlBlock.connInfo[index].procedureAttr.rangingCounter,
                                                                    RREQ_DATA_INVALID,
                                                                    segmentsReader);
        }
    }
}

/*********************************************************************
 * @fn      rreq_sendStatusCB
 *
 * @brief   Sends status to the application via the registered callback.
 *
 * @param   connHandle    - The connection handle for which the status is sent.
 * @param   statusCode    - The status code to be sent.
 * @param   statusDataLen - The length of the status data.
 * @param   statusData    - Pointer to the status data.
 *
 * @return  None.
 */
static void rreq_sendStatusCB(uint16_t connHandle, RREQClientStatus_e statusCode, uint8_t statusDataLen, uint8_t* statusData)
{
    if(connHandle < RREQ_MAX_CONN &&
       gRREQControlBlock.callbacks != NULL &&
       gRREQControlBlock.callbacks->pStatusCallback != NULL)
    {
        // send status to App
        gRREQControlBlock.callbacks->pStatusCallback(connHandle, statusCode, statusDataLen, statusData);
    }
}

/*********************************************************************
 * @fn      rreq_handleRspCode
 *
 * @brief   Handles the response code received from the server.
 *          This function processes the response code and calls the
 *          appropriate callback function based on the response value.
 *
 * @param   index    - The index associated with the connection handle.
 * @param   rspValue - The response value received from the server.
 *
 * @return  None.
 */
static void rreq_handleRspCode(uint8_t index, uint8_t rspValue)
{
    // Initialize internal status variable to an invalid value. If set - we need to call the status callback
    RREQClientStatus_e status = RREQ_INVALID_STATUS_CODE;

    // Check parameters and that enable mode is On-Demand
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        gRREQControlBlock.connInfo[index].enableMode != RREQ_MODE_ON_DEMAND)
    {
        return;
    }

    // Handle the response code based on its value
    switch(rspValue)
    {
        case RAS_CP_RSP_CODE_VAL_SUCCESS:
        {
            if(RREQ_CURRENT_STATE(index) == RREQ_STATE_WAIT_FOR_ABORT)
            {
                status = RREQ_ABORTED_SUCCESSFULLY;
            }
            else if(RREQ_CURRENT_STATE(index) == RREQ_STATE_WAIT_FOR_CONTROL_POINT_RSP)
            {
                // handle the procedure completion - will also stop the timers and set the state to READY
                rreq_procedureDone(index);
            }

            break;
        }
        case RAS_CP_RSP_CODE_ABORT_UNSUCCESSFUL:
        {
            if(RREQ_CURRENT_STATE(index) == RREQ_STATE_WAIT_FOR_ABORT)
            {
                status = RREQ_ABORTED_UNSUCCESSFULLY;
            }
            break;
        }
        case RAS_CP_RSP_CODE_SERVER_BUSY:
        {
            // Expect this only when waiting for first segment (i.e getRangingData sent by us)
            if (RREQ_CURRENT_STATE(index) == RREQ_STATE_WAIT_FOR_FIRST_SEGMENT)
            {
                status = RREQ_SERVER_BUSY;
            }
            break;
        }
        case RAS_CP_RSP_CODE_NO_RECORDS_FOUND:
        {
            // Expect this only when waiting for first segment or after sending an ACK
            if (RREQ_CURRENT_STATE(index) == RREQ_STATE_WAIT_FOR_FIRST_SEGMENT ||
                RREQ_CURRENT_STATE(index) == RREQ_STATE_WAIT_FOR_CONTROL_POINT_RSP)
            {
                status = RREQ_NO_RECORDS;
            }
            break;
        }
        case RAS_CP_RSP_CODE_PROCEDURE_NOT_COMP:
        {
            // Expect this only when waiting for first segment (i.e getRangingData sent by us), next segment or after sending an ACK
            if (RREQ_CURRENT_STATE(index) == RREQ_STATE_WAIT_FOR_FIRST_SEGMENT ||
                RREQ_CURRENT_STATE(index) == RREQ_STATE_WAIT_FOR_NEXT_SEGMENT ||
                RREQ_CURRENT_STATE(index) == RREQ_STATE_WAIT_FOR_CONTROL_POINT_RSP)
            {
                status = RREQ_PROCEDURE_NOT_COMPLETED;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    // If status is initialized by any of the above cases, set a new state and call the status callback
    if (status != RREQ_INVALID_STATUS_CODE)
    {
        // Reset the procedure state to ready
        rreq_setProcedureState(index, RREQ_STATE_READY);

        // Stop the timer
        rreq_stopTimer(index);

        // Notify application
        rreq_sendStatusCB(RREQ_INDEX_CONN_HANDLE(index),
                          status,
                          RANGING_COUNTER_LEN,
                          (uint8_t*)&gRREQControlBlock.connInfo[index].procedureAttr.rangingCounter);
    }
}

/*********************************************************************
 * @fn      rreq_parseSegmentReceived
 *
 * @brief Handles the "on-demand" notification.
 *
 * @param index - The index associated with the connection handle of the
 *                received segment.
 * @param handleValueNoti - Pointer to the handle value notification structure.
 *
 * @return None.
 */
static void rreq_parseSegmentReceived(uint8_t index, attHandleValueNoti_t *handleValueNoti)
{
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        handleValueNoti == NULL)
    {
        return;
    }

    // split data to segment (1 byte) and the rest of the data
    uint8_t *pData = handleValueNoti->pValue;
    uint8_t segmentNum = RAS_LAST_6_BITS_LSB(pData[0]); // Extract the last 6 bits (LSB)
    uint8_t segmentFlag = RAS_FIRST_2_BITS_LSB(pData[0]); // Extract the first 2 bits (LSB)
    uint16_t dataLen = handleValueNoti->len - 1; // Exclude the segment byte

    // Check if the segment is the first one
    if( (segmentFlag & RAS_FIRST_SEGMENT_BIT_MASK) != 0 )
    {
        // Clear the procedure DB
        RangingDBClient_clearProcedure(gRREQControlBlock.connInfo[index].rangingDbHandle);

        // Clear last segment flag
        rreq_clearLastSegmentFlag(index);
    }

    // Check if the segment is the last one
    if( (segmentFlag & RAS_LAST_SEGMENT_BIT_MASK) != 0 )
    {
        gRREQControlBlock.connInfo[index].lastSegmentFlag = TRUE;
    }

    // Add the data to the procedure DB
    RangingDBClient_addData(gRREQControlBlock.connInfo[index].rangingDbHandle, segmentNum, dataLen, &pData[1]);
}

/*********************************************************************
 * @fn      rreq_sendControlPointWriteCmd
 *
 * @brief Sends a control point write command to the RAS client.
 * This function is responsible for transmitting a write command to the
 * control point of the RAS client.
 *
 * @param index          - The index associated with the connection handle for
 *                         the RAS client.
 * @param cmd            - The command to be sent to the control point.
 * @param rangingCounter - The ranging counter value to be included in the command.
 * @param len            - The length of the command data to be sent.
 *
 * @return INVALIDPARAMETER: Index is invalid or command length is out of range.
 * @return bleMemAllocError: Memory allocation error occurred.
 * @return A status generated by @ref GATT_WriteNoRsp otherwise
 */
static bStatus_t rreq_sendControlPointWriteCmd(uint8_t index, uint8_t cmd, uint16_t rangingCounter, uint8_t len)
{
    bStatus_t status = SUCCESS;
    attWriteReq_t req;
    uint8 dataValue[RAS_CP_COMMANDS_MAX_LEN] = {0};

    // Check if the index and command length are valid
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        len < RAS_CP_COMMANDS_MIN_LEN ||
        len > RAS_CP_COMMANDS_MAX_LEN)
    {
        status = INVALIDPARAMETER;
    }

    if (status == SUCCESS)
    {
        // Set the command to be sent
        dataValue[0] = cmd;
        dataValue[1] = LO_UINT16(rangingCounter);
        dataValue[2] = HI_UINT16(rangingCounter);

        // Allocate buffer for the write request
        req.pValue = GATT_bm_alloc(RREQ_INDEX_CONN_HANDLE(index), ATT_WRITE_REQ, len, NULL);

        // Send the write request for indications enable / disable
        if (req.pValue != NULL)
        {
            req.handle = gRREQControlBlock.connInfo[index].charInfo[RREQ_CONTROL_POINT_CHAR_INDEX].charHandle;
            req.len = len;
            memcpy(req.pValue, dataValue, len);
            req.cmd = TRUE;
            req.sig = FALSE;

            // Send the write request
            status = GATT_WriteNoRsp(RREQ_INDEX_CONN_HANDLE(index), &req);

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
 * @param index   - Index identifying the BLE connection.
 * @param gattMsg - Pointer to the GATT message event structure containing
 *                  the response data.
 */
static void rreq_handleFindByTypeValueRsp(uint8_t index, gattMsgEvent_t *gattMsg)
{
    // Check parameters and procedure state
    if ((gattMsg == NULL) || index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        RREQ_CURRENT_STATE(index) != RREQ_STATE_INIT_DISCOVER_PRIM_SERVICE)
    {
        return;
    }

    if( gattMsg->hdr.status == SUCCESS )
    {
        // Save the start and end handles of the RAS service
        gRREQControlBlock.connInfo[index].startHandle = ATT_ATTR_HANDLE(gattMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
        gRREQControlBlock.connInfo[index].endHandle = ATT_GRP_END_HANDLE(gattMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);

        // Mark this state as completed
        rreq_setInitStateCompleted(index, RREQ_STATE_INIT_DISCOVER_PRIM_SERVICE);
    }
    else if( gattMsg->hdr.status == bleProcedureComplete )
    {
        // Find all characteristics within the Car Access service
        bStatus_t status = rreq_discoverAllChars(index);

        if (status != SUCCESS)
        {
            rreq_handleStateInitError(index, status);
        }
    }
    else
    {
        rreq_handleStateInitError(index, gattMsg->hdr.status);
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
 * @param index   - Index identifying the BLE connection.
 * @param gattMsg - Pointer to the GATT message event structure containing
 *                  the response data.
 */
static void rreq_handleReadByTypeRsp(uint8_t index, gattMsgEvent_t *gattMsg)
{
    attReadByTypeRsp_t att;

    if (gattMsg == NULL || index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        RREQ_CURRENT_STATE(index) != RREQ_STATE_INIT_DISCOVER_ALL_CHARS)
    {
        return;
    }

    att = gattMsg->msg.readByTypeRsp;

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
            uint8_t charIndex = i * att.len;

            // characteristic properties
            uint8_t charPro = att.pDataList[charIndex + RREQ_APP_CHAR_PRO_HANDLE_INDEX];
            // ATT handle
            uint16_t currAttHandle = BUILD_UINT16(att.pDataList[charIndex + RREQ_APP_CHAR_VALUE_HANDLE_INDEX],
                                                  att.pDataList[charIndex + RREQ_APP_CHAR_VALUE_HANDLE_INDEX + 1]);

            // Copy the full 16-bit custom UUID
            memcpy(customUUID, &att.pDataList[charIndex + RREQ_APP_CHAR_UUID_HANDLE_INDEX], ATT_BT_UUID_SIZE);

            // Build the 16-bit UUID of the characteristic
            charUUID = BUILD_UINT16(customUUID[RREQ_APP_LOW_UUID_INDEX],
                                    customUUID[RREQ_APP_HIGH_UUID_INDEX]);

            if (charUUID >= RAS_FEATURE_UUID && charUUID <= RAS_DATA_OVERWRITTEN_UUID)
            {
                gRREQControlBlock.connInfo[index].charInfo[RREQ_UUID_TO_CHAR_INDEX(charUUID)].charHandle = currAttHandle;
                gRREQControlBlock.connInfo[index].charInfo[RREQ_UUID_TO_CHAR_INDEX(charUUID)].charProperties = charPro;
            }
        }
    }
    else if( gattMsg->hdr.status == bleProcedureComplete )
    {
        // Mark this state as completed
        rreq_setInitStateCompleted(index, RREQ_STATE_INIT_DISCOVER_ALL_CHARS);

        // discover all characteristic Descriptors
        bStatus_t status = rreq_discoverAllCharDescriptors(index);

        if (status != SUCCESS)
        {
            rreq_handleStateInitError(index, status);
        }
    }
    else
    {
        rreq_handleStateInitError(index, gattMsg->hdr.status);
    }
}

/*********************************************************************
 * @fn      rreq_handleFindInfoRsp
 *
 * @brief Handles the GATT "BLEAPPUTIL_ATT_FIND_INFO_RSP".
 * This function processes the response received from the GATT server
 * after getting "BLEAPPUTIL_ATT_FIND_INFO_RSP".
 * It is responsible for handling the data returned in the response
 * and performing any necessary actions based on the received information.
 *
 * @param index     - Index identifying the BLE connection.
 * @param gattMsg   - Pointer to the GATT message event structure containing
 *                    the response data.
 */
static void rreq_handleFindInfoRsp(uint8_t index, gattMsgEvent_t *gattMsg)
{
    bStatus_t status = SUCCESS;

    if (gattMsg == NULL || index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        RREQ_CURRENT_STATE(index) != RREQ_STATE_INIT_DISCOVER_ALL_CHAR_DESCS)
    {
        return;
    }

    status = gattMsg->hdr.status;

    // Discover chars procedure ends only when the response status is bleProcedureComplete or with ATT_ERROR_RSP event
    if(status == bleProcedureComplete)
    {
        rreq_setInitStateCompleted(index, RREQ_STATE_INIT_DISCOVER_ALL_CHAR_DESCS);

        // read from "features" characteristic
        status = rreq_readFeaturesCharValue(index);
    }

    if (status != SUCCESS)
    {
        rreq_handleStateInitError(index, status);
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
 * @param index - The index associated with the connection handle for which
 *                the characteristic descriptors are to be discovered.
 *
 * @return INVALIDPARAMETER - Invalid index
 * @return Status generated by @ref GATT_DiscAllCharDescs
 */
static bStatus_t rreq_discoverAllCharDescriptors(uint8_t index)
{
    bStatus_t status = SUCCESS;

    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        return INVALIDPARAMETER;
    }

    // Discover all characteristic descriptors
    status = GATT_DiscAllCharDescs(RREQ_INDEX_CONN_HANDLE(index),
                                   gRREQControlBlock.connInfo[index].startHandle,
                                   gRREQControlBlock.connInfo[index].endHandle,
                                   BLEAppUtil_getSelfEntity());

    if (status == SUCCESS)
    {
        // Update state
        rreq_setProcedureState(index, RREQ_STATE_INIT_DISCOVER_ALL_CHAR_DESCS);
    }

    return status;
}

/*********************************************************************
 * @fn      rreq_discoverPrimServ
 *
 * @brief   RAS client discover primary service by UUID,
 *          ATT_FIND_BY_TYPE_VALUE_RSP
 *
 * @param   index - Index associated with the connection handle for
 *                  which to discover the primary service.
 *
 * @return  INVALIDPARAMETER: Index is invalid
 * @return  Status generated by @ref GATT_DiscPrimaryServiceByUUID
 */
static bStatus_t rreq_discoverPrimServ(uint8_t index)
{
    bStatus_t status = SUCCESS;

    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        status = INVALIDPARAMETER;
    }

    if (status == SUCCESS)
    {
        // Discovery Car Access service
        GATT_BT_UUID(rasUUID, RANGING_SERVICE_UUID);
        status = GATT_DiscPrimaryServiceByUUID(RREQ_INDEX_CONN_HANDLE(index), rasUUID, ATT_BT_UUID_SIZE, BLEAppUtil_getSelfEntity());
    }

    if (status == SUCCESS)
    {
        // Set state to discover primary service
        rreq_setProcedureState(index, RREQ_STATE_INIT_DISCOVER_PRIM_SERVICE);
    }

    return status;
}

/*********************************************************************
 * @fn      rreq_getIndexByConnHandle
 *
 * @brief   Retrieves the index of the connection information
 *          based on the provided connection handle.
 *
 * @param   connHandle - The connection handle for which the index is to be retrieved.
 *
 * @return  The index of the connection information if found
 * @return  RREQ_INVALID_INDEX - if the connection handle is not found.
 */
static uint8_t rreq_getIndexByConnHandle(uint16_t connHandle)
{
    uint8_t index = RREQ_INVALID_INDEX;

    if (connHandle < RREQ_MAX_CONN && connHandle != LINKDB_CONNHANDLE_INVALID)
    {
        for (uint8_t i = 0; i < RANGING_DB_CLIENT_MAX_NUM_PROC; i++)
        {
            if (gRREQControlBlock.connInfo[i].connHandle == connHandle)
            {
                index = i;
                break;
            }
        }
    }

    return index;
}

/*********************************************************************
 * @fn      rreq_getEmptyIndex
 *
 * @brief   Retrieves an empty index in the connection information array.
 *
 * @return  RREQ_INVALID_INDEX - if no empty index is found.
 * @return  The index of the first empty slot if found, otherwise
 */
static uint8_t rreq_getEmptyIndex(void)
{
    uint8_t index = RREQ_INVALID_INDEX;

    for (uint8_t i = 0; i < RANGING_DB_CLIENT_MAX_NUM_PROC; i++)
    {
        if (gRREQControlBlock.connInfo[i].connHandle == LINKDB_CONNHANDLE_INVALID)
        {
            index = i;
            break;
        }
    }

    return index;
}

/*********************************************************************
 * @fn      rreq_getCharProperties
 *
 * @brief Retrieves the characteristic subscription type, derived by
 *        preferred subscription and characteristic properties.
 *
 * @param preferredSubType - The request subscription type to be used
 *                           for retrieving the properties.
 * @param properties       - The characteristic properties to be processed.
 *
 * @return The final subscription type that should be used for the characteristic
 *         @ref RREQ_INDICATE or @ref RREQ_PREFER_NOTIFY
 */
static RREQConfigSubType_e rreq_getCharProperties(RREQConfigSubType_e preferredSubType, uint8_t properties)
{
    // set default value to indicatation
    RREQConfigSubType_e charPro = RREQ_INDICATE;

    // check if the characteristic supports notification and the desired preferredSubType is notification
    if( ((properties & RAS_NOTIFICATION_PRO_MASK) != 0) && (preferredSubType == RREQ_PREFER_NOTIFY) )
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
 * over a specified index. It allows the application to receive updates from the
 * remote device when the value of the attribute changes.
 *
 * @param index     - The index associated with the connection handle for the remote device.
 * @param mode      - The request mode type, specifying how the notification request should be handled.
 * @param attHandle - The attribute handle for which notifications are to be enabled.
 *
 * @return INVALIDPARAMETER: The provided index or attribute handle is invalid.
 * @return bleMemAllocError: Memory allocation error occurred.
 * @return A status generated by @ref GATT_WriteCharValue otherwise
 */
static bStatus_t rreq_enableNotification(uint8_t index, RREQConfigSubType_e mode, uint16_t attHandle)
{
    bStatus_t status = SUCCESS;
    attWriteReq_t req;
    // Set the default value to disable notification
    uint8_t dataValue[RREQ_APP_CCCD_VALUE_LEN] = {0};

    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        attHandle == 0)
    {
        status = INVALIDPARAMETER;
    }

    if (status == SUCCESS)
    {
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

        // Allocate buffer for the write request
        req.pValue = GATT_bm_alloc(RREQ_INDEX_CONN_HANDLE(index), ATT_WRITE_REQ, RREQ_APP_CCCD_VALUE_LEN, NULL);

        // Send the write request for Notifications enable/diable
        if (req.pValue != NULL)
        {
            req.handle = attHandle + RREQ_APP_CCCD_OFFSET;
            req.len = RREQ_APP_CCCD_VALUE_LEN;
            memcpy(req.pValue, dataValue, RREQ_APP_CCCD_VALUE_LEN);
            req.cmd = FALSE;
            req.sig = FALSE;

            status = GATT_WriteCharValue(RREQ_INDEX_CONN_HANDLE(index), &req, BLEAppUtil_getSelfEntity());

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

    return status;
}

/*********************************************************************
 * @fn      rreq_setCurrentConfiguredCharInfo
 *
 * @brief   Sets the current configured characteristic information for a
 *          given index.
 *
 * @note not relevant for @ref RAS_FEATURE_UUID
 *
 * @param index         - The index for which the characteristic
 *                        information is to be set.
 * @param charUUID      - The UUID of the characteristic to be set.
 * @param charSubtype   - The subtype of the characteristic to be set.
 */
static void rreq_setCurrentConfiguredCharInfo(uint8_t index, uint16_t charUUID, RREQConfigSubType_e charSubtype)
{
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        charSubtype >= RREQ_SUBTYPE_INVALID ||
        charUUID < RAS_REAL_TIME_UUID ||
        charUUID > RAS_DATA_OVERWRITTEN_UUID)
    {
        return;
    }

    gRREQControlBlock.connInfo[index].currentConfiguredCharUUID = charUUID;
    gRREQControlBlock.connInfo[index].currentConfiguredCharSubtype = charSubtype;
}

/*********************************************************************
 * @fn      rreq_clearCurrentConfiguredCharInfo
 *
 * @brief   Clears the current configured characteristic information for a
 *          given index, by setting it to @ref RREQ_INVALID_UUID
 *
 * @param index - The index for which the characteristic information is to be cleared.
 */
static void rreq_clearCurrentConfiguredCharInfo(uint8_t index)
{
    rreq_setCurrentConfiguredCharInfo(index, RREQ_INVALID_UUID, RREQ_SUBTYPE_INVALID);
}

/*********************************************************************
 * @fn      rreq_isInitDone
 *
 * @brief   Checks if the initialization procedure is completed for a
 *          given index.
 *
 * @param index - The index to check.
 *
 * @return true if the initialization procedure is completed, false otherwise.
 */
static bool rreq_isInitDone(uint8_t index)
{
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        return false;
    }

    return (gRREQControlBlock.connInfo[index].procedureAttr.completedInitStatesBM & RREQ_INIT_STATES_BITMAP) == RREQ_INIT_STATES_BITMAP;
}

/*********************************************************************
 * @fn      rreq_setProcedureState
 *
 * @brief Sets the procedure state for a given index.
 *
 * @param index      - Index to work on.
 * @param stateToSet - The state to set.
 */
static void rreq_setProcedureState(uint8_t index, RREQProcedureStates_e stateToSet)
{
    if (index < RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        gRREQControlBlock.connInfo[index].procedureAttr.procedureState = stateToSet;
    }
}

/*********************************************************************
 * @fn      rreq_setInitStateCompleted
 *
 * @brief   Marks a specific initialization state as completed for a
 *          given index.
 *
 * @param index - The index which its state is to be marked as completed.
 * @param state - The initialization state to be marked as completed.
 *                Valid states:
 *                @ref RREQ_STATE_INIT_DISCOVER_PRIM_SERVICE
 *                @ref RREQ_STATE_INIT_DISCOVER_ALL_CHARS
 *                @ref RREQ_STATE_INIT_DISCOVER_ALL_CHAR_DESCS
 *                @ref RREQ_STATE_INIT_READ_CHAR_FEATURE
 */
static void rreq_setInitStateCompleted(uint8_t index, RREQProcedureStates_e state)
{
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        state < RREQ_STATE_INIT_DISCOVER_PRIM_SERVICE ||
        state > RREQ_STATE_INIT_READ_CHAR_FEATURE)
    {
        return;
    }

    gRREQControlBlock.connInfo[index].procedureAttr.completedInitStatesBM |= state;
}

/*********************************************************************
 * @fn      rreq_clearData
 *
 * @brief Clears all data associated with the specified index.
 * This function is responsible for clearing all data related to a specific
 * index. It resets the relevant variables and structures to ensure that
 * no stale data remains.
 *
 * @param index - The index of the connection for which data should be cleared.
 */
static void rreq_clearData(uint8_t index)
{
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        return;
    }

    gRREQControlBlock.connInfo[index].rangingDbHandle = RANGING_DB_CLIENT_INVALID_HANDLE;
    gRREQControlBlock.connInfo[index].connHandle = LINKDB_CONNHANDLE_INVALID;

    // Clear the start/end handles
    gRREQControlBlock.connInfo[index].startHandle = 0;
    gRREQControlBlock.connInfo[index].endHandle = 0;

    // Clear the characteristic info
    memset(gRREQControlBlock.connInfo[index].charInfo, 0, sizeof(gRREQControlBlock.connInfo[index].charInfo));

    gRREQControlBlock.connInfo[index].preferredMode = RREQ_MODE_ON_DEMAND;
    gRREQControlBlock.connInfo[index].enableMode = RREQ_MODE_NONE;
    rreq_clearCurrentConfiguredCharInfo(index);
    gRREQControlBlock.connInfo[index].subscribeBitMap = 0;
    gRREQControlBlock.connInfo[index].featureCharValue = 0;
    gRREQControlBlock.connInfo[index].currentProcedureCounter = RREQ_INVALID_CS_PROCEDURE_COUNTER;
    gRREQControlBlock.connInfo[index].timeoutHandle = BLEAPPUTIL_TIMER_INVALID_HANDLE;

    // Clear procedure attribute
    rreq_setProcedureState(index, RREQ_STATE_IDLE);
    gRREQControlBlock.connInfo[index].procedureAttr.completedInitStatesBM = 0;
    gRREQControlBlock.connInfo[index].procedureAttr.rangingCounter = 0xFFFF;

    // Clear last segment flag
    rreq_clearLastSegmentFlag(index);
}

/*********************************************************************
 * @fn      rreq_clearLastSegmentFlag
 *
 * @brief   Clears the last segment flag for a given index.
 *
 * @param index - The index of the connection information to clear the
 *                last segment flag for.
 */
static void rreq_clearLastSegmentFlag(uint8_t index)
{
    gRREQControlBlock.connInfo[index].lastSegmentFlag = FALSE;
}

/*********************************************************************
 * @fn      rreq_handleOnDemandSegmentReceived
 *
 * @brief   Handles the reception of an on-demand segment notification from the server.
 * This function processes a received ATT Handle Value Notification containing an on-demand
 * segment from the server for a specific index. It is typically called when
 * a notification is received on the characteristic associated with on-demand data segments.
 *
 * @param index           - The index identifying the BLE connection.
 * @param handleValueNoti - Pointer to the handle value notification structure.
 *
 * @return None.
 */
static void rreq_handleOnDemandSegmentReceived(uint8_t index, attHandleValueNoti_t *handleValueNoti)
{
    // Check parameters and configuration
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        handleValueNoti == NULL ||
        gRREQControlBlock.config == NULL ||
        gRREQControlBlock.connInfo[index].enableMode != RREQ_MODE_ON_DEMAND)
    {
        return;
    }

    switch (RREQ_CURRENT_STATE(index))
    {
        case RREQ_STATE_WAIT_FOR_FIRST_SEGMENT:
        {
            // Set state to wait for next segment
            rreq_setProcedureState(index, RREQ_STATE_WAIT_FOR_NEXT_SEGMENT);

            // Start the timer for timeout
            rreq_startTimer(gRREQControlBlock.config->timeoutConfig.timeOutNextSegment, index);

            // Handle the segment received
            rreq_parseSegmentReceived(index, handleValueNoti);
            break;
        }
        case RREQ_STATE_WAIT_FOR_NEXT_SEGMENT:
        {
            // Start the timer for timeout
            rreq_startTimer(gRREQControlBlock.config->timeoutConfig.timeOutNextSegment, index);

            // Handle the segment received
            rreq_parseSegmentReceived(index, handleValueNoti);
            break;
        }

        default:
            // Unknown state, do nothing
            break;
    }
}

/*********************************************************************
 * @fn      rreq_handleRealTimeSegmentReceived
 *
 * @brief Handles the reception of a real-time segment notification from the server.
 *
 * This function processes a received ATT Handle Value Notification containing a real-time
 * segment from the server for a specific index. It is typically called when
 * a notification is received on the characteristic associated with real-time data segments.
 *
 * @param index           - The index identifying the BLE connection.
 * @param handleValueNoti - Pointer to the ATT Handle Value Notification structure containing
 *                          the received data segment.
 */
static void rreq_handleRealTimeSegmentReceived(uint8_t index, attHandleValueNoti_t *handleValueNoti)
{
    bStatus_t status = SUCCESS;
    uint8_t segmentFlag;
    bool isLastSegment = FALSE;

    // Check parameters and configuration
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        handleValueNoti == NULL                 ||
        handleValueNoti->len == 0               ||
        gRREQControlBlock.config == NULL        ||
        gRREQControlBlock.connInfo[index].enableMode != RREQ_MODE_REAL_TIME)
    {
        return;
    }

    // Get segment numbers
    segmentFlag = RAS_FIRST_2_BITS_LSB(handleValueNoti->pValue[0]); // Extract the first 2 bits (LSB)

    switch (RREQ_CURRENT_STATE(index))
    {
        case RREQ_STATE_WAIT_FOR_FIRST_SEGMENT:
        {
            if ( (segmentFlag & RAS_FIRST_SEGMENT_BIT_MASK) != 0)
            {
                // Update state and the current ranging counter
                rreq_setProcedureState(index, RREQ_STATE_WAIT_FOR_NEXT_SEGMENT);
                gRREQControlBlock.connInfo[index].procedureAttr.rangingCounter = gRREQControlBlock.connInfo[index].currentProcedureCounter;
                // Stop the first segment timer
                rreq_stopTimer(index);

                // If this is the last segment
                if( (segmentFlag & RAS_LAST_SEGMENT_BIT_MASK) != 0 )
                {
                    // Mark as last segment
                    isLastSegment = TRUE;
                }
                else
                {
                    // This is not the last segment - start the timer for the next one
                    rreq_startTimer(gRREQControlBlock.config->timeoutConfig.timeOutNextSegment, index);
                }

                // Handle the segment received
                rreq_parseSegmentReceived(index, handleValueNoti);
            }
            else
            {
                // Lost first segment, consider as failure
                status = FAILURE;
                rreq_procedureDone(index);
            }

            break;
        }
        case RREQ_STATE_WAIT_FOR_NEXT_SEGMENT:
        {
            // If this is the last segment
            if( (segmentFlag & RAS_LAST_SEGMENT_BIT_MASK) != 0 )
            {
                isLastSegment = TRUE;

                // stop the timer
                rreq_stopTimer(index);
            }
            else
            {
                // This is not the last segment - start the timer for the next one
                rreq_startTimer(gRREQControlBlock.config->timeoutConfig.timeOutNextSegment, index);
            }

            // Handle the received segment
            rreq_parseSegmentReceived(index, handleValueNoti);

            break;
        }

        default:
        {
            // Unknown state, do nothing
            status = FAILURE;
            break;
        }
    }

    // If received the last segment - send the data to the application if possible
    if (status == SUCCESS && isLastSegment == true)
    {
        rreq_procedureDone(index);
    }
}

/*********************************************************************
 * @fn      rreq_startTimer
 *
 * @brief   Start the timer for the RREQ procedure.
 *
 * @param   timeout - The new timeout value in milliseconds.
 * @param   index   - The index of the connection info for which the timer is started.
 *
 * @return  None
 */
static void rreq_startTimer( uint32_t timeout, uint8_t index )
{
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        return;
    }

    if (gRREQControlBlock.connInfo[index].timeoutHandle != BLEAPPUTIL_TIMER_INVALID_HANDLE)
    {
        // Stop the timer if it is already running
        BLEAppUtil_abortTimer(gRREQControlBlock.connInfo[index].timeoutHandle);
    }

    // Start the timer
    gRREQControlBlock.connInfo[index].timeoutHandle = BLEAppUtil_startTimer(rreq_timerCB, timeout, false, (void*) ((uint32_t) index));
}

/*********************************************************************
 * @fn      rreq_stopTimer
 *
 * @brief   Stop the timer for the RREQ procedure.
 *
 * @param   index - The index of the connection info for which the timer is stopped.
 *
 * @return  None
 */
static void rreq_stopTimer( uint8_t index )
{
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        return;
    }

    if (gRREQControlBlock.connInfo[index].timeoutHandle != BLEAPPUTIL_TIMER_INVALID_HANDLE)
    {
        // Abort the active timer and reset the timer handle
        BLEAppUtil_abortTimer(gRREQControlBlock.connInfo[index].timeoutHandle);
        gRREQControlBlock.connInfo[index].timeoutHandle = BLEAPPUTIL_TIMER_INVALID_HANDLE;
    }
}

/*********************************************************************
 * @fn      rreq_handleTimeoutRealTime
 *
 * @brief   Handles timeout events when Real-Time mode is enabled.
 *
 * This function is called when a timeout occurs during the
 * real-time mode procedure for a specific index.
 * It initializes an unregistration procedure of the Real-Time
 * characteristic.
 *
 * input parameters
 *
 * @param   index - The index of the connection info for which the timeout occurred.
 *
 * output parameters
 *
 * @param   None
 *
 * @return None
 */
static void rreq_handleTimeoutRealTime(uint8_t index)
{
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        gRREQControlBlock.connInfo[index].enableMode != RREQ_MODE_REAL_TIME)
    {
        return;
    }

    switch (RREQ_CURRENT_STATE(index))
    {
        case RREQ_STATE_WAIT_FOR_FIRST_SEGMENT:
        case RREQ_STATE_WAIT_FOR_NEXT_SEGMENT:
        {
            // Clear any partial segment data from the DB to avoid memory leak
            RangingDBClient_clearProcedure(gRREQControlBlock.connInfo[index].rangingDbHandle);

            // Update the procedure state to IDLE
            rreq_setProcedureState(index, RREQ_STATE_IDLE);

            // TODO: Handle failure of this function
            // Unregister Real-Time characteristic.
            RREQ_ConfigureCharRegistration(index, RAS_REAL_TIME_UUID, RREQ_DISABLE_NOTIFY_INDICATE);

            // Notify the application about the timeout
            rreq_sendStatusCB(RREQ_INDEX_CONN_HANDLE(index),
                              RREQ_TIMEOUT_SEGMENTS,
                              RANGING_COUNTER_LEN,
                              (uint8_t*)&gRREQControlBlock.connInfo[index].procedureAttr.rangingCounter);
            break;
        }
        default:

            // Unknown state, do nothing
            break;
    }
}

/*********************************************************************
 * @fn      rreq_handleTimeoutOnDemand
 *
 * @brief   Handles timeout events when On-Demand mode is enabled.
 *
 * This function is called when a timeout occurs during the
 * on-demand mode procedure for a specific index.
 * It starts the Abort procedure for the current ranging procedure.
 *
 * input parameters
 *
 * @param   index - The index of the connection info for which the timeout occurred.
 *
 * output parameters
 *
 * @param   None
 *
 * @return None
 */
static void rreq_handleTimeoutOnDemand(uint8_t index)
{
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        gRREQControlBlock.connInfo[index].enableMode != RREQ_MODE_ON_DEMAND)
    {
        return;
    }

    switch (RREQ_CURRENT_STATE(index))
    {
        case RREQ_STATE_WAIT_FOR_DATA_READY:
        {
            // Update the procedure state to READY and notify the application
            rreq_setProcedureState(index, RREQ_STATE_READY);
            rreq_sendStatusCB(RREQ_INDEX_CONN_HANDLE(index), RREQ_TIMEOUT_DATA_READY, RANGING_COUNTER_LEN, (uint8_t*)&gRREQControlBlock.connInfo[index].procedureAttr.rangingCounter);
            break;
        }
        case RREQ_STATE_WAIT_FOR_FIRST_SEGMENT:
        case RREQ_STATE_WAIT_FOR_NEXT_SEGMENT:
        {
            // Send Abort if Server supports abort operation
            if (gRREQControlBlock.connInfo[index].featureCharValue & RAS_FEATURES_ABORT_OPERATION)
            {
                // TODO: Handle failure of this function
                RREQ_Abort(RREQ_INDEX_CONN_HANDLE(index));
            }
            else
            {
                // Update state back to ready
                rreq_setProcedureState(index, RREQ_STATE_READY);
            }

            // Notify the application about the timeout
            rreq_sendStatusCB(RREQ_INDEX_CONN_HANDLE(index), RREQ_TIMEOUT_SEGMENTS, RANGING_COUNTER_LEN, (uint8_t*)&gRREQControlBlock.connInfo[index].procedureAttr.rangingCounter);
            break;
        }
        case RREQ_STATE_WAIT_FOR_CONTROL_POINT_RSP:
        case RREQ_STATE_WAIT_FOR_ABORT:
        {
            // Update state back to ready
            rreq_setProcedureState(index, RREQ_STATE_READY);

            // Notify the application about the timeout
            rreq_sendStatusCB(RREQ_INDEX_CONN_HANDLE(index), RREQ_TIMEOUT_CONTROL_POINT_RSP, RANGING_COUNTER_LEN, (uint8_t*)&gRREQControlBlock.connInfo[index].procedureAttr.rangingCounter);
            break;
        }
        default:

            // Unknown state, do nothing
            break;
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
    uint8_t index = (uint8_t) (((uint32_t) pData) & 0xFFFF);

    // Reset the timer handle
    gRREQControlBlock.connInfo[index].timeoutHandle = BLEAPPUTIL_TIMER_INVALID_HANDLE;

    if (gRREQControlBlock.connInfo[index].enableMode == RREQ_MODE_ON_DEMAND)
    {
        rreq_handleTimeoutOnDemand(index);
    }
    else if (gRREQControlBlock.connInfo[index].enableMode == RREQ_MODE_REAL_TIME)
    {
        rreq_handleTimeoutRealTime(index);
    }
  }
}

/*********************************************************************
 * @fn      rreq_L2CapEventHandler
 *
 * @brief   Handles L2CAP events.
 *
 * @param   event - message event.
 * @param   pMsgData - pointer to message data.
 *
 * @return  None
 */
void rreq_L2CapEventHandler(uint32 event, BLEAppUtil_msgHdr_t *pMsgData)
{
    uint8_t index;

    if (pMsgData == NULL)
    {
        return;
    }

    if (event == BLEAPPUTIL_L2CAP_NUM_CTRL_DATA_PKT_EVT)
    {
        // Loop over all active connection (indices), and execute any pending tasks
        for (index = 0; index < RANGING_DB_CLIENT_MAX_NUM_PROC; index++)
        {
            // Skip this index if it is not active
            if (gRREQControlBlock.connInfo[index].connHandle == LINKDB_CONNHANDLE_INVALID)
            {
                continue;
            }

            switch(RREQ_CURRENT_STATE(index))
            {
                case RREQ_STATE_SEND_ACK:
                {
                    rreq_sendAck(index);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }

    // Unregister from L2CAP events if no pending tasks left
    rreq_checkAndUnregisterL2CAP();
}

/*********************************************************************
 * @fn      rreq_checkAndUnregisterL2CAP
 *
 * @brief   Unregisters from L2CAP events if no pending tasks are left.
 *
 * @param   None
 *
 * @return  None
 */
void rreq_checkAndUnregisterL2CAP(void)
{
    uint8_t index;
    bool isPendingTask = false;

    // Loop again and check if there is still a connection with a pending state
    for (index = 0; index < RANGING_DB_CLIENT_MAX_NUM_PROC; index++)
    {
        // Skip this index if it is not active
        if (gRREQControlBlock.connInfo[index].connHandle == LINKDB_CONNHANDLE_INVALID)
        {
            continue;
        }

        switch(RREQ_CURRENT_STATE(index))
        {
            case RREQ_STATE_SEND_ACK:
            {
                isPendingTask = true;
                break;
            }
            default:
            {
                break;
            }
        }
    }

    // If didn't found a connection with a pending task - unregister from L2CAP events
    if (isPendingTask == false)
    {
        // Unregister from L2CAP events
        BLEAppUtil_unRegisterEventHandler(&gRREQL2CAPHandler);

        // Update flag
        gRREQControlBlock.l2capRegistered = FALSE;
    }
}

#endif // RANGING_CLIENT
