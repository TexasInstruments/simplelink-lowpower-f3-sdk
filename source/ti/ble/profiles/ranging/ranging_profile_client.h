/******************************************************************************

@file  ranging_profile_client.h

@brief This file contains the ranging requestert (RREQ) APIs and structures.

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
#include "ti/ble/services/ranging/ranging.h"

/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

typedef void (*RREQ_DataReadyCallback)(uint16_t connHandle, uint16_t rangingCount);
typedef void (*RREQ_SubeventDataCallback)(uint16_t connHandle, uint16_t rangingCount, void *pCSSubEvent);
typedef void (*RREQ_CompleteEventCallback)(uint16_t connHandle, uint16_t rangingCount, uint8_t status, uint16_t dataLen, uint8_t* pData);
typedef void (*RREQ_StatusCallback)(uint16_t connHandle, uint8_t statusCode, uint8_t statusDataLen, uint8_t* statusData);

/*********************************************************************
 * Structures
 */

 // RREQ Configuration Subscription types
typedef enum
{
    RREQ_DISABLE_NOTIFY_INDICATE = 0x00,  // Disable notifications and indications
    RREQ_PREFER_NOTIFY,                   // Prefer notifications over indications
    RREQ_INDICATE,                        // Use indications for notifications
} RREQConfigSubType_e;

// RREQ Enable modes
typedef enum
{
    RREQ_IDLE = 0x00,  // Idle state, not enabled
    RREQ_ON_DEMAND,    // On-demand ranging request
    RREQ_REAL_TIME,    // Real-time ranging request
} RREQEnableModeType_e;

// RREQ Subscriptions configuration structure
typedef struct
{
    RREQConfigSubType_e onDemandSubType;       // On-demand data subscription type
    RREQConfigSubType_e controlPointSubType;   // Control point subscription type
    RREQConfigSubType_e dataReadySubType;      // Data ready event subscription type
    RREQConfigSubType_e overwrittenSubType;    // Data overwritten event subscription type
} RREQSubConfig_t;

// RREQ Configuration structure
typedef struct
{
    RREQSubConfig_t subConfig;      // Subscriptions configuration
    uint32_t timeOutDataReady;      // Timeout for data ready event in milliseconds
    uint32_t timeOutNextSegment;    // Timeout for next segment event in milliseconds
    uint32_t timeOutCompleteEvent;  // Timeout for complete event in milliseconds
} RREQConfig_t;

// RREQ Callbacks structure
typedef struct
{
    RREQ_DataReadyCallback pDataReadyCallback;              // Callback for data ready events
    RREQ_CompleteEventCallback pDataCompleteEventCallback;  // Callback for complete events
    RREQ_StatusCallback pStatusCallback;                    // Callback for status events
} RREQCallbacks_t;

// RREQ Procedure Attribute structure
typedef struct
{
    uint8_t procedureState;   // Current state of the RREQ procedure
    uint16_t connHandle;      // Connection handle for the RREQ
    uint16_t rangingCounter;  // Counter for the ranging procedure
} RREQProcedureAttr_t;

// RREQ Procedure States
typedef enum
{
    RREQ_STATE_IDLE = 0x00,                 // Idle state
    RREQ_STATE_WAIT_FOR_FIRST_SEGMENT,      // Waiting for the first segment of the ranging data
    RREQ_STATE_WAIT_FOR_NEXT_SEGMENT,       // Waiting for the next segment or the complete data
    RREQ_STATE_WAIT_FOR_CONTROL_POINT_RSP,  // Waiting for the control point response
    RREQ_STATE_WAIT_FOR_ABORT               // Waiting for the abort response
} RREQProcedureStates_e;

// RREQ Client Status Codes
typedef enum
{
  RREQ_TIMEOUT = 0x00,            // Timeout occurred during the procedure
  RREQ_ABORTED_SUCCESSFULLY,      // Procedure aborted successfully
  RREQ_ABORTED_UNSUCCESSFULLY,    // Procedure aborted unsuccessfully
  RREQ_SERVER_BUSY,               // Server is busy, cannot process the request
  RREQ_PROCEDURE_NOT_COMPLETED,   // Procedure not completed successfully
  RREQ_NO_RECORDS,                // No records found for the request
  RREQ_DATA_INVALID,              // Data received is invalid
  RREQ_DATA_OVERWRITTEN,          // Data has been overwritten
} RREQClientStatus_e;

// RREQ Segments Manager structure
typedef struct
{
    uint64_t bitMask;           // Bitmask to indicate if all segments are received.
    uint16_t totalDataLen;      // Total length of the data received.
    uint16_t segmentSize;       // Size of each segment.
    uint8_t lastSegmentValue;   // Value of the last segment.
    uint8_t lastSegmentFlag;    // Flag to indicate if the last segment is received.
} RREQSegmentsMGR_t;

/*********************************************************************
 * FUNCTIONS
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
uint8_t RREQ_Start(const RREQCallbacks_t *pCallbacks , const RREQConfig_t *pConfig);

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
uint8_t RREQ_Enable(uint16_t connHandle, RREQEnableModeType_e enableMode);

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
uint8_t RREQ_Disable(uint16_t connHandle);

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
uint8_t RREQ_GetRangingData(uint16_t connHandle, uint16_t rangingCount);

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
uint8_t RREQ_Abort(uint16_t connHandle);

#endif // RANGING_CLIENT

#ifdef __cplusplus
}
#endif

#endif /* RANGING_PROFILE_CLIENT_H */
