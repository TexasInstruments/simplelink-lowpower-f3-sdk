/******************************************************************************

@file  ranging_profile_client.h

@brief This file contains the ranging requestert (RREQ) APIs and structures.

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

#ifndef RANGING_PROFILE_CLIENT_H
#define RANGING_PROFILE_CLIENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef RANGING_CLIENT
/*********************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/services/ranging/ranging_types.h"
#include "ti/ble/profiles/ranging/ranging_db_client.h"

/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */

// Maximum timeout for waiting for control point response in milliseconds (On-Demand mode only)
#define RREQ_MAX_TIMEOUT_CONTROL_POINT_RSP_MS   5000
// Maximum timeout for first segment event in milliseconds (On-Demand mode only)
#define RREQ_MAX_TIMEOUT_DATA_READY_MS          5000
// Maximum timeout for first segment event in milliseconds (On-Demand & Real-Time)
#define RREQ_MAX_TIMEOUT_FIRST_SEGMENT_MS       5000
// Maximum timeout for next segment event in milliseconds (On-Demand & Real-Time)
#define RREQ_MAX_TIMEOUT_NEXT_SEGMENT_MS        1000

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

// RREQ Client Status Codes
typedef enum
{
    /* API asynchronous statuses */
    RREQ_INIT_DONE = 0x00,            // Initialization done successfully after @ref RREQ_Enable; returned at @ref RREQ_StatusCallback
                                      // Raised after: @ref RREQ_Enable
                                      // statusData: NULL
                                      // Profile state: READY
                                      // Application action: Can start using the profile - call @ref RREQ_GetRangingData (On-Demand) or wait for data (Real-Time)

    RREQ_INIT_FAILED,                 // Initialization failed during @ref RREQ_Enable; returned at @ref RREQ_StatusCallback
                                      // Raised after: @ref RREQ_Enable
                                      // Profile state: INACTIVE
                                      // statusData: Contains the error code that caused the failure
                                      // Application action: Retry with @ref RREQ_Enable or cleanup with @ref RREQ_Disable

    RREQ_CHAR_CONFIGURATION_DONE,     // Characteristic registration/unregistration completed; returned at @ref RREQ_StatusCallback
                                      // Raised after: @ref RREQ_ConfigureCharRegistration
                                      // statusData: NULL
                                      // Application action: If registering - can proceed with operations; If unregistering - characteristic is disabled

    RREQ_CHAR_CONFIGURATION_FAILED,   // Characteristic configuration failed at host level; returned at @ref RREQ_StatusCallback
                                      // Raised after: @ref RREQ_ConfigureCharRegistration
                                      // statusData: Contains the error status from GATT operation
                                      // Application action: May retry the configuration operation or handle the error appropriately

    RREQ_CHAR_CONFIGURATION_REJECTED, // Server rejected characteristic configuration; returned at @ref RREQ_StatusCallback
                                      // Raised after: @ref RREQ_ConfigureCharRegistration
                                      // statusData: Contains the ATT error code from server
                                      // Application action: If unregistering, consider characteristic as disabled; If registering, handle rejection

    RREQ_DATA_INVALID,                // Data received is invalid or incomplete
                                      // On-Demand: returned at @ref RREQ_CompleteEventCallback; raised during @ref RREQ_GetRangingData ACK phase
                                      // Real-Time: returned at @ref RREQ_StatusCallback; raised by rreq_handleRealTimeFailure on segment error
                                      // segmentsReader (On-Demand only): Contains an empty Segments Reader
                                      // Cause: Last segment flag not received, out-of-order segment, or unexpected error during segment processing
                                      // Profile state (On-Demand): Returns to READY; registration is preserved
                                      // Profile state (Real-Time): Returns to IDLE; rreq_handleRealTimeFailure issues de-registration via
                                      //   @ref RREQ_ConfigureCharRegistration — @ref RREQ_CHAR_CONFIGURATION_DONE will follow
                                      // Application action (On-Demand): Discard the data; can start a new procedure immediately
                                      // Application action (Real-Time): Discard the data; wait for @ref RREQ_CHAR_CONFIGURATION_DONE,
                                      //   then re-register via @ref RREQ_ConfigureCharRegistration if desired

    /* Timeout statuses */
    RREQ_TIMEOUT_CONTROL_POINT_RSP,   // Timeout waiting for control point response; returned at @ref RREQ_StatusCallback
                                      // Raised after: @ref RREQ_GetRangingData (after sending ACK), @ref RREQ_Abort; On-Demand mode only
                                      // Profile state: Returns to READY
                                      // Context: Occurs after sending ACK or during abort procedure (On-Demand only)
                                      // statusData: Contains the ranging counter (2 bytes) for which timeout occurred
                                      // Application action: Procedure failed, can start a new procedure

    RREQ_TIMEOUT_DATA_READY,          // Timeout waiting for data ready event; returned at @ref RREQ_StatusCallback
                                      // Raised after: @ref RREQ_ProcedureStarted (On-Demand mode only)
                                      // Profile state: Returns to READY
                                      // Context: On-Demand mode only, after @ref RREQ_ProcedureStarted was called
                                      // statusData: Contains the ranging counter (2 bytes) for which timeout occurred
                                      // Application action: CS procedure may have failed or server didn't send notification; Can start a new procedure

    RREQ_TIMEOUT_SEGMENTS,            // Timeout waiting for first or next segment (On-Demand mode only); returned at @ref RREQ_StatusCallback
                                      // Raised after: @ref RREQ_GetRangingData
                                      // Profile state: If abort supported - calls @ref RREQ_Abort and waits for abort response; Otherwise returns to READY
                                      // Context: On-Demand mode only, can occur during any segment reception
                                      // statusData: Contains the ranging counter (2 bytes) for which timeout occurred
                                      // Application action: If abort supported - wait for abort completion; Otherwise procedure ended, can start new one

    RREQ_TIMEOUT_SEGMENTS_RT,         // Timeout waiting for first or next segment (Real-Time mode only); returned at @ref RREQ_StatusCallback
                                      // Raised after: segment reception timeout during Real-Time mode
                                      // Profile state: Returns to IDLE and calls @ref RREQ_ConfigureCharRegistration to unregister from Real-Time characteristic
                                      // Context: Real-Time mode only; @ref RREQ_CHAR_CONFIGURATION_DONE will follow once de-registration completes
                                      // statusData: Contains the ranging counter (2 bytes) for which timeout occurred
                                      // Application action: Wait for @ref RREQ_CHAR_CONFIGURATION_DONE to re-register if desired

    /* RAS Control point statuses */
    RREQ_ABORTED_SUCCESSFULLY,        // Abort command processed successfully by server; returned at @ref RREQ_StatusCallback
                                      // Raised after: @ref RREQ_Abort, or automatic abort after @ref RREQ_GetRangingData timeout
                                      // Profile state: Returns to READY
                                      // Triggered by: @ref RREQ_Abort (manual) or automatic abort on timeout (if server supports abort)
                                      // statusData: Contains the ranging counter (2 bytes) that was aborted
                                      // Application action: Procedure was aborted, can start a new procedure

    RREQ_ABORTED_UNSUCCESSFULLY,      // Server couldn't process abort request; returned at @ref RREQ_StatusCallback
                                      // Raised after: @ref RREQ_Abort, or automatic abort after @ref RREQ_GetRangingData timeout
                                      // Profile state: Returns to READY
                                      // Triggered by: @ref RREQ_Abort (manual) or automatic abort on timeout
                                      // statusData: Contains the ranging counter (2 bytes) for which abort was attempted
                                      // Application action: Procedure ended despite unsuccessful abort, can start a new procedure

    RREQ_SERVER_BUSY,                 // Server is busy, cannot process request; returned at @ref RREQ_StatusCallback
                                      // Raised after: @ref RREQ_GetRangingData
                                      // Profile state: Returns to READY
                                      // Context: Received after @ref RREQ_GetRangingData when server is processing another request
                                      // statusData: Contains the ranging counter (2 bytes) that was requested
                                      // Application action: Wait and retry the request later

    RREQ_PROCEDURE_NOT_COMPLETED,     // Procedure not completed successfully; returned at @ref RREQ_StatusCallback
                                      // Raised after: @ref RREQ_GetRangingData
                                      // Profile state: Returns to READY
                                      // Context: Can occur during segment reception or after sending ACK
                                      // statusData: Contains the ranging counter (2 bytes) for which procedure failed
                                      // Application action: Procedure failed on server side, can start a new procedure

    RREQ_NO_RECORDS,                  // No records found for requested ranging counter; returned at @ref RREQ_StatusCallback
                                      // Raised after: @ref RREQ_GetRangingData
                                      // Profile state: Returns to READY
                                      // Context: Server doesn't have data for the requested ranging counter
                                      // statusData: Contains the ranging counter (2 bytes) that was requested
                                      // Application action: Request different ranging counter or wait for new data to be available

    RREQ_DATA_OVERWRITTEN,            // Data overwritten on server before being read; returned at @ref RREQ_StatusCallback
                                      // Raised after: Asynchronous notification (can occur anytime in On-Demand mode when subscribed)
                                      // Context: Notification from Data Overwritten characteristic (On-Demand only)
                                      // statusData: Contains the ranging counter (2 bytes) that was overwritten
                                      // Application action: If ongoing procedure for this counter, it may fail; Request data before it gets overwritten

    RREQ_INVALID_STATUS_CODE = 0xFF   // Invalid status code, for internal use
} RREQClientStatus_e;

typedef void (*RREQ_DataReadyCallback)(uint16_t connHandle, uint16_t rangingCount);
typedef void (*RREQ_CompleteEventCallback)(uint16_t connHandle, uint16_t rangingCount, uint8_t status, RangingDBClient_procedureSegmentsReader_t segmentsReader);
typedef void (*RREQ_StatusCallback)(uint16_t connHandle, RREQClientStatus_e statusCode, uint8_t statusDataLen, uint8_t* statusData);

/*********************************************************************
 * Structures
 */

 // RREQ Configuration Subscription types
typedef enum
{
    RREQ_DISABLE_NOTIFY_INDICATE = 0x00,  // Disable notifications and indications
    RREQ_PREFER_NOTIFY,                   // Prefer notifications over indications
    RREQ_INDICATE,                        // Use indications for notifications
    RREQ_SUBTYPE_INVALID,                 // Invalid subscription type - for internal use
} RREQConfigSubType_e;

// RREQ Enable modes
typedef enum
{
    RREQ_MODE_NONE = 0x00,  // Data exchange mode not set
    RREQ_MODE_ON_DEMAND,    // On-demand ranging request
    RREQ_MODE_REAL_TIME,    // Real-time ranging request
} RREQEnableModeType_e;

// RREQ On-Demand configuration structure
typedef struct
{
    RREQConfigSubType_e onDemandSubType;       // On-demand data subscription type
    RREQConfigSubType_e controlPointSubType;   // Control point subscription type
    RREQConfigSubType_e dataReadySubType;      // Data ready event subscription type
    RREQConfigSubType_e overwrittenSubType;    // Data overwritten event subscription type
} RREQOnDemandSubConfig_t;

// RREQ Real-Time configuration structure
typedef struct
{
    RREQConfigSubType_e realTimeSubType;       // Real-time data subscription type
} RREQRealTimeSubConfig_t;

// RREQ Timeout configuration structure
typedef struct
{
    uint32_t timeOutControlPointRsp;    // Timeout for control point response, On-Demand only
    uint32_t timeOutDataReady;          // Timeout for data ready event, On-Demand only
    uint32_t timeOutFirstSegment;       // Timeout for receiving first segment
    uint32_t timeOutNextSegment;        // Timeout for next segment event in milliseconds
} RREQTimeoutConfig_t;

// RREQ Configuration structure
typedef struct
{
    RREQOnDemandSubConfig_t onDemandSubConfig;  // Subscriptions configuration for On-Demand
    RREQRealTimeSubConfig_t realTimeSubConfig;  // Subscriptions configuration for Real-Time
    RREQTimeoutConfig_t timeoutConfig;          // Timeout configurations
} RREQConfig_t;

// RREQ Callbacks structure
typedef struct
{
    RREQ_DataReadyCallback pDataReadyCallback;              // Callback for data ready events
    RREQ_CompleteEventCallback pDataCompleteEventCallback;  // Callback for complete events
    RREQ_StatusCallback pStatusCallback;                    // Callback for status events
} RREQCallbacks_t;

/*********************************************************************
 * FUNCTIONS
 */

 /*********************************************************************
 * @fn      RREQ_Start
 *
 * @brief   Initializes the ranging requester and
 *          stores the provided configuration and callbacks.
 *
 * @note    This function should be called once.
 *
 * @note    pConfig is considered valid as long as:
 *          1. Not NULL
 *          2. At least one mode is selected (On-Demand or Real-Time)
 *          3. If On-Demand mode is selected - all depended characteristics
 *             subscription types are set.
 *          4. All timeouts are set to non-zero values and do not exceed
 *             the maximum allowed timeouts:
 *             - Control Point Response: @ref RREQ_MAX_TIMEOUT_CONTROL_POINT_RSP_MS
 *             - Data Ready:             @ref RREQ_MAX_TIMEOUT_DATA_READY_MS
 *             - First Segment:          @ref RREQ_MAX_TIMEOUT_FIRST_SEGMENT_MS
 *             - Next Segment:           @ref RREQ_MAX_TIMEOUT_NEXT_SEGMENT_MS
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
 * @return  SUCCESS          - Initialization was successful.
 *          INVALIDPARAMETER - Input parameters are invalid or RREQ already started.
 */
uint8_t RREQ_Start(const RREQCallbacks_t *pCallbacks , const RREQConfig_t *pConfig);

/*********************************************************************
 * @fn      RREQ_Enable
 *
 * @brief   Enables the RREQ for a given connection handle.
 *
 * This function initiating the following procedures
 * 1. discovering primary service
 * 2. discovering all characteristics
 * 3. discovering all characteristic descriptors
 * 4. reading feature characteristic
 * 5. configuration of the discovered characteristics according to
 *    configuration provided in @ref RREQ_Start function
 *
 *  When enableMode parameter is set to @RREQ_REAL_TIME, a registration
 *  for the Real-Time characteristic will be done as long as the
 *  server allows it, otherwise - will register to the relevant
 *  On-Demand characteristics.
 *
 *  When this function returns SUCCESS, the initialization procedure
 *  will begin.
 *  The procedure will finish when one of the following events are
 *  received within the configured @ref RREQ_StatusCallback:
 *  @ref RREQ_INIT_DONE
 *  @ref RREQ_INIT_FAILED
 *
 *  If failed asynchronously, the application can call this function
 *  again in order to retry the initialization procedure, or call
 *  @ref RREQ_Disable to cleanup the profile data.
 *
 * input parameters
 *
 * @param   connHandle - Connection handle.
 * @param   enableMode - Preferred mode to enable.
 *
 * output parameters
 *
 * @param   None
 *
 * @return SUCCESS          - Successfully started the RREQ initialization procedure.
 * @return INVALIDPARAMETER - Connection handle or enableMode are invalid.
 * @return bleIncorrectMode - RREQ is already enabled.
 * @return FAILURE          - No resources in the internal database to start
 *                            the ranging procedure for this connection.
 *
 * @return status derived by one the following GATT operations:
 *         @ref GATT_DiscPrimaryServiceByUUID
 *         @ref GATT_DiscAllChars
 *         @ref GATT_DiscAllCharDescs
 *         @ref GATT_ReadCharValue
 *         @ref GATT_WriteCharValue
 *         If so - an RREQ DB is already associated with the connection handle,
 *         therefore call this function again if you wish to try again,
 *         or @ref RREQ_Disable to clear the existing RREQ DB.
 */
uint8_t RREQ_Enable(uint16_t connHandle, RREQEnableModeType_e enableMode);

/*********************************************************************
 * @fn      RREQ_Disable
 *
 * @brief Disables the RREQ for a given connection handle.
 *
 * This function disables the RREQ by unregistering from the active
 * ranging data characteristic and clearing all the RREQ related data.
 * It stops any ongoing ranging procedure and releases all resources
 * for the given connection handle.
 *
 * - If the connection is not active - clears the db.
 * - If the connection is active and the RREQ is registered to the
 *   ranging data characteristic (On-demand\Real-Time) - will send a command
 *   to the server in order to unregister from the characteristic and will
 *   clear the db depends on the following:
 *   1. If the host fails to send the command, the function will return the
 *      status returned by @ref GATT_WriteCharValue and won't clear the db.
 *   2. If the host succeeds, the function will clear the db and return SUCCESS.
 *
 * input parameters
 *
 * @param   connHandle - The connection handle for the RAS service.
 *
 * output parameters
 *
 * @param   None
 *
 * @return SUCCESS              - RREQ process was successfully disabled.
 * @return INVALIDPARAMETER     - connection handle is invalid.
 *
 * @return status derived by @ref GATT_WriteCharValue if it failed to execute
 *         the characteristic write on the host level
 */
uint8_t RREQ_Disable(uint16_t connHandle);

/*********************************************************************
 * @fn      RREQ_ConfigureCharRegistration
 *
 * @brief Configures the registration for a characteristic in the Ranging Profile Client.
 *
 * This function registers or unregisters for notifications or indications on a
 * specific characteristic identified by its UUID for a given connection handle.
 *
 * When successfully executed, the configuration process is considered initiated and
 * will complete when one of the following events is received within the
 * configured @ref RREQ_StatusCallback:
 * @ref RREQ_CHAR_CONFIGURATION_DONE     - The configuration process completed successfully.
 * @ref RREQ_CHAR_CONFIGURATION_FAILED   - The configuration process failed to execute.
 * @ref RREQ_CHAR_CONFIGURATION_REJECTED - The server rejected the configuration request.
 *                                         In a case we tried to unregister, the profile
 *                                         will consider the characteristic as unregistered.
 *
 * @param connHandle - The connection handle identifying the BLE connection.
 * @param charUUID   - The UUID of the characteristic to configure.
 * @param subType    - The type of configuration to apply (see RREQConfigSubType_e).
 *
 * @note Does not accept @ref RAS_FEATURE_UUID for configuration.
 *
 * @return SUCCESS          - Configuration was successfully executed.
 * @return INVALIDPARAMETER - Connection handle or subType are invalid, the characteristic
 *                            UUID is not supported, or if trying to register to both
 *                            Real-Time and On-Demand characteristics.
 * @return bleIncorrectMode - In case this API is called before @ref RREQ_Enable has completed
 *                            all required initialization procedures or RREQ is currently busy.
 *
 * @return status derived by @ref GATT_WriteCharValue if it fails to execute on the host level.
 */
uint8_t RREQ_ConfigureCharRegistration(uint16_t connHandle, uint16_t charUUID, RREQConfigSubType_e subType);

/*********************************************************************
 * @fn      RREQ_GetRangingData
 *
 * @brief   Starts the process of reading data for a ranging request.
 *
 * This function initiates the data reading process for a specified connection
 * handle and ranging counter.
 *
 * The data reading process will complete once one of the following occurs:
 *
 * 1. The configured @ref RREQ_CompleteEventCallback function is called.
 * 2. The configured @ref RREQ_StatusCallback function is called with
 *    one of the following status codes:
 *    - @ref RREQ_SERVER_BUSY event is received.
 *    - @ref RREQ_PROCEDURE_NOT_COMPLETED event is received.
 *    - @ref RREQ_NO_RECORDS event is received.
 *    - @ref RREQ_TIMEOUT_SEGMENTS event is received. In such case, the profile
 *      will call to @ref RREQ_Abort API automatically as long as the server
 *      allows it, and the procedure will finish according to the abort procedure.
 *
 * input parameters
 *
 * @param   connHandle   - Connection handle.
 * @param   RangingCount - CS procedure counter.
 *
 * output parameters
 *
 * @param   None
 *
 * @return SUCCESS          - Data reading process was successfully started.
 * @return INVALIDPARAMETER - Input parameters are invalid.
 * @return bleIncorrectMode - On-Demand mode is not enabled or another
 *                            procedure is already in progress.
 *
 * @return status derived by @ref GATT_WriteNoRsp if it fails to execute on
 *         the host level.
 */
uint8_t RREQ_GetRangingData(uint16_t connHandle, uint16_t rangingCount);

/*********************************************************************
 * @fn      RREQ_Abort
 *
 * @brief   Aborts the ongoing ranging request. Relevant for On-Demand mode only.
 *
 * This function is responsible for aborting the ongoing ranging request
 * for a specified connection handle.
 * Should be called after @ref RREQ_GetRangingData was called and
 * before the data reading process is completed.
 *
 * When executed successfully, the function sends an abort command
 * to the server and will start the Abort procedure.
 * The procedure will finish when one of the following events are received within
 * the configure @ref RREQ_StatusCallback:
 * @ref RREQ_TIMEOUT_CONTROL_POINT_RSP - Timeout waiting for control point response.
 * @ref RREQ_ABORTED_SUCCESSFULLY      - The server processed the request and
 *                                       the procedure was aborted successfully.
 * @ref RREQ_ABORTED_UNSUCCESSFULLY    - The server couldn't process the request.
 *
 * input parameters
 *
 * @param   connHandle - Connection handle.
 *
 * output parameters
 *
 * @param   None
 *
 * @return SUCCESS          - Abort command was successfully sent.
 * @return INVALIDPARAMETER - Connection handle is invalid.
 * @return bleIncorrectMode - No ongoing procedure to abort, the server
 *                            the server does not support aborting procedures,
 *                            or On-Demand mode is not enabled.
 *
 * @return status derived by @ref GATT_WriteNoRsp if it fails to execute on
 *         the host level.
 */
uint8_t RREQ_Abort(uint16_t connHandle);

/*********************************************************************
 * @fn      RREQ_ProcedureStarted
 *
 * @brief   Notifies the RREQ that a Channel Sounding procedure has been started.
 *
 * This function notifies the RREQ module that a Channel Sounding procedure
 * has been started between the client and server.
 * When successfully executed, the RREQ module will start a timer for
 * the Data Ready event according to the configured timeout value.
 * The function won't take any action if the given procedure counter
 * is the same as the last notified one.
 *
 * Once the timer expires, the configured @ref RREQ_StatusCallback
 * will be called with @ref RREQ_TIMEOUT_DATA_READY event.
 *
 * If Data Ready notification is received before the timer expires,
 * the timer will be stopped and the configured @ref RREQ_DataReadyCallback
 * will be called.
 *
 * @param connHandle       - Connection handle.
 * @param procedureCounter - CS procedure counter that has been started.
 *
 * @return SUCCESS                   - Notification was successfully processed.
 * @return INVALIDPARAMETER          - Connection handle is invalid.
 * @return bleIncorrectMode          - Real-Time\On-demand mode is not enabled or
 *                                     the RREQ is not ready for new procedures.
 * @return bleAlreadyInRequestedMode - The given procedure counter is the same
 *                                     as the last notified one.
 */
uint8_t RREQ_ProcedureStarted(uint16_t connHandle, uint16_t procedureCounter);

/*********************************************************************
 * @fn      RREQ_getConnInfoSize
 *
 * @brief   Returns the size of the RREQ data required.
 *          This function calculates the size needed to transfer the RREQ
 *          connection information.
 *
 * @param   connHandle - Connection handle.
 *
 * @return  The size of the RREQ data in bytes if connection is valid.
 * @return  0 if the connection handle is invalid or RREQ is not enabled for this connection.
 */
uint32_t RREQ_getConnInfoSize(uint16_t connHandle);

/*********************************************************************
 * @fn      RREQ_getConnInfoData
 *
 * @brief   Populates the RREQ ConnInfo data to the provided buffer.
 *
 * @param   connHandle      - Connection handle.
 * @param   pData           - Pointer to the buffer where the data will be stored.
 *                          The buffer must be at least @ref RREQ_getConnInfoSize() bytes.
 *
 * @return  SUCCESS.
 * @return  INVALIDPARAMETER.
 */
uint8_t RREQ_getConnInfoData(uint16_t connHandle, uint8_t *pData);

/*********************************************************************
 * @fn      RREQ_isAvailableSlot
 *
 * @brief   Check if there is available slots for new connection.
 *
 * input parameters
 * @param   None
 *
 * @return  SUCCESS.
 * @return  FAILURE.
 */
uint8_t RREQ_isAvailableSlot(void);

/*********************************************************************
 * @fn      RREQ_populateConnInfoData
 *
 * @brief   Apply the data to the actual RREQ table.
 *
 * @param   connHandle      - Connection handle.
 * @param   pData           - Pointer to the buffer where the RREQ data is stored.
 * @param   length          - Length of the data buffer.
 *
 * @return  SUCCESS.
 * @return  FAILURE.
 */
uint8_t RREQ_populateConnInfoData(uint16_t connHandle, uint8_t* pData, uint32_t length);

/*********************************************************************
 * @fn      RREQ_localDisable
 *
 * @brief   Disable the RREQ process on this device, without notifying the peer.
 *
 * @param   connHandle      - Connection handle.
 *
 * @return  SUCCESS.
 * @return  INVALIDPARAMETER.
 */
uint8_t RREQ_localDisable(uint16_t connHandle);

#endif // RANGING_CLIENT

#ifdef __cplusplus
}
#endif

#endif /* RANGING_PROFILE_CLIENT_H */
