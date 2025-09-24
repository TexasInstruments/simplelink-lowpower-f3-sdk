/******************************************************************************

 @file  ranging_profile.c

 @brief This file contains the Ranging profile sample (RRSP) for use with the Car Node sample application.

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
#include "ti/ble/profiles/ranging/ranging_profile.h"
#include "ti/ble/profiles/ranging/ranging_db_server.h"
#include "ti/ble/services/ranging/ranging_types.h"
/* This Header file contains all BLE API and icall structure definition */
#include "ti/ble/app_util/framework/bleapputil_api.h"

/*********************************************************************
 * DEFINES
 */

#define SUBEVENT_STATUSES_INDEX            0x04
#define SUBEVENT_STATUSES_SIZE             0x01
#define SUBEVENT_STEPS_NUM_INDEX           0x07
#define RRSP_RANGING_COUNTER_INVALID_VALUE 0xFF
#define RRSP_ABORT_REASON_MASK             0x0F
#define RRSP_SIZE_OF_STEP                  0x02
#define RRSP_GATT_HEADER_SIZE              0x04
#define RRSP_SEGMENT_HDR_SIZE              0x01
// Mask to get the 6 LSB of the segment index
#define RRSP_6_LSB_MASK                    0x3F

/*********************************************************************
* MACROS
*/

#define RRSP_SEGMENT_MAX_SIZE(mtu)         ((mtu) - RRSP_SEGMENT_HDR_SIZE - RRSP_GATT_HEADER_SIZE)

/*********************************************************************/

/*********************************************************************
 * TYPEDEFS
 */

// Control block struct for all global variables in this file
typedef struct {
    RRSP_cb_t *ranging_appCB;
    RRSP_Segmentation_Process_t rrspSegmentationProcess;
    RRSP_receive_subevent_t receivingSubeventProcess;
    RRSP_RegistrationStatus_e rrspRegisteredData[MAX_NUM_BLE_CONNS];
} RRSP_ControlBlock_t;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

void rrspSaveRegistrationStatus(RAS_cccUpdate_t *rasCCCUpdate);
uint8_t rrspSendErrorResponse(uint16_t connHandle, uint8_t responseCode, uint8_t reason);
uint8_t rrspSendSegmentsIndi(uint16_t connHandle, uint16_t rangingCounter);
uint8_t rrspSendSegmentsNoti(uint16_t connHandle, uint16_t rangingCounter);
uint16_t rrspCalcSegmentOffset(uint8_t segmentIndex, uint16_t connMtu);
void rrspHandleAckRangingData(uint16_t connHandle, uint16_t rangingCounter);
void rrspHandleGetRangingData(uint16_t connHandle, uint16_t rangingCounter);
void rrspConnEventHandler(uint32 event, BLEAppUtil_msgHdr_t *pMsgData);
void rrspL2CapEventHandler(uint32 event, BLEAppUtil_msgHdr_t *pMsgData);
void rrspGattEventHandler(uint32 event, BLEAppUtil_msgHdr_t *pMsgData);
uint8_t sendOverWritten(uint16_t connHandle, uint16_t rangingCounter);
uint8_t rangingSubeventParser(RRSP_csSubEventResults_t* subeventResult, uint8_t *pParsedData);
uint8_t rangingSubeventContParser(RRSP_csSubEventResultsContinue_t* subeventResultCont, uint8_t *pParsedData);
uint8_t RangingDataParser(uint8_t *pData, uint16_t Datalen, uint8_t numSteps, uint8_t *pParsedData);
// Callback functions
void rrspCccUpdateCB(char *pValue);
void rrspCPReqCB( char *pValue);

/*********************************************************************
 * LOCAL VARIABLES
 */

// Global control block instance for this file
static RRSP_ControlBlock_t gRAPControlBlock = {0};

// Handler for GAP connection events
BLEAppUtil_EventHandler_t gRAPConnectionConnHandler =
{
    .handlerType    = BLEAPPUTIL_GAP_CONN_TYPE,
    .pEventHandler  = rrspConnEventHandler,
    .eventMask      = BLEAPPUTIL_LINK_TERMINATED_EVENT
};

// Handler for L2CAP events
BLEAppUtil_EventHandler_t gRAPL2CAPHandler =
{
    .handlerType    = BLEAPPUTIL_L2CAP_SIGNAL_TYPE,
    .pEventHandler  = rrspL2CapEventHandler,
    .eventMask      = BLEAPPUTIL_L2CAP_NUM_CTRL_DATA_PKT_EVT
};

// GATT event handler
BLEAppUtil_EventHandler_t gRAPGATTHandler =
{
    .handlerType    = BLEAPPUTIL_GATT_TYPE,
    .pEventHandler  = rrspGattEventHandler,
    .eventMask      = BLEAPPUTIL_ATT_HANDLE_VALUE_CFM
};
/*********************************************************************
 * SERVER CALLBACKS
 */

// RAS profile callback functions
 RAS_cb_t rrsp_serverCB =
{
  rrspCccUpdateCB,
  rrspCPReqCB
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      RRSP_start
 *
 * @brief   This function adds the Ranging Server service,
 *          registers the service callback function and initializes
 *          the service characteristics
 *
 * @param   appCallbacks - pointer to application callback
 * @param   features - Ranging feature value
 *
 * @return  SUCCESS or stack call status
 */
uint8_t RRSP_start(RRSP_cb_t *appCallbacks, uint32_t features)
{
    uint8 status = FAILURE;

    if ( appCallbacks == NULL )
    {
        status = INVALIDPARAMETER;
    }
    else
    {
        // Init the control block
        memset(&gRAPControlBlock, 0, sizeof(RRSP_ControlBlock_t));

        // Registers to GAP connection events
        status = BLEAppUtil_registerEventHandler(&gRAPConnectionConnHandler);


        if (status == SUCCESS)
        {
            // Registers to L2CAP events
            status = BLEAppUtil_registerEventHandler(&gRAPL2CAPHandler);

            if (status == SUCCESS)
            {
                // Register the GATT event handler
                status = BLEAppUtil_registerEventHandler(&gRAPGATTHandler);


                if (status == SUCCESS)
                {

                    // Add Ranging Service
                    status = RAS_addService();

                    if ( status == SUCCESS )
                    {
                        // Init ranging data DB
                        RangingDBServer_InitDB();

                        // Register to service callback function
                        status = RAS_registerProfileCBs( &rrsp_serverCB );
                        if ( status != SUCCESS )
                        {
                            // Registers the application callback function
                            gRAPControlBlock.ranging_appCB = appCallbacks;

                            // Set Ranging Feature value in the service table
                            status = RAS_setParameter(LINKDB_CONNHANDLE_INVALID, RAS_FEAT_ID, &features, RAS_FEAT_LEN );
                        }
                    }
                }
            }
        }
    }

    // Return status value
    return ( status );
}

/*********************************************************************
 * @fn      RRSP_ProcedureStarted
 *
 * @brief   This function is called to indicate that a ranging procedure has started.
 *
 * @param   connHandle - Connection handle of the device that started the procedure.
 * @param   rangingCounter - The ranging counter for the procedure.
 * @param   pRangingHeader - Pointer to the ranging header containing procedure information.
 *
 * @retun  None
 * */
uint8_t RRSP_ProcedureStarted(uint16_t connHandle, uint16_t rangingCounter, uint8_t *pRangingHeader)
{
  uint8_t status = USUCCESS;
  RangingDBServer_procedureId_t procedureID;

  // Set procedureID
  procedureID.connHandle = connHandle;
  procedureID.rangingCounter = rangingCounter;

  gRAPControlBlock.rrspSegmentationProcess.waitForConf = FALSE;
  gRAPControlBlock.rrspSegmentationProcess.waitForNoti = FALSE;

  status = RangingDBServer_procedureStart(&procedureID, pRangingHeader, RAS_RANGING_HEADER_LEN);

  // If FAILURE, procedure ID already exists in the database.
  if (status == FAILURE)
  {
      // clear procedure from DB
      status = RangingDBServer_ClearProcedure(procedureID);
      if (status == SUCCESS)
      {
          // Start the procedure again
          status = RangingDBServer_procedureStart(&procedureID, pRangingHeader, RAS_RANGING_HEADER_LEN);
      }

  }

  // If procedureID is not identical, the received procedeID is an overwritten procedure
  if ((procedureID.connHandle != connHandle) || (procedureID.rangingCounter != rangingCounter))
  {
      // send overwritten message to the peer
      sendOverWritten(procedureID.connHandle, procedureID.rangingCounter);
  }

  return status;
}

/*********************************************************************
 * @fn      RRSP_ProcedureDone
 *
 * @brief   This function is called to indicate that a ranging procedure has completed
 *          and now the profile aknowledge that he is ready to transfer data.
 *
 * @param   connHandle - Connection handle of the device that completed the procedure.
 * @param   rangingCounter - The ranging counter for the procedure.
 *
 * @return  None
 */
uint8_t RRSP_ProcedureDone(uint16_t connHandle, uint16_t rangingCounter)
{
  uint8_t status = USUCCESS;

  // Send data ready message to the client peer
  RAS_setParameter(connHandle, RAS_READY_ID, &rangingCounter, RAS_DATA_READY_LEN);

  return status;
}

/*********************************************************************
 * @fn      RRSP_RegistrationStatus
 *
 * @brief   This function is called to check the registration status
 *          of a connection handle
 *
 * @param   connHandle - Connection handle of the device to check registration status.
 *
 * @retun   registration status of the connection handle (@ref RRSP_RegistrationStatus_e)
 */
RRSP_RegistrationStatus_e RRSP_RegistrationStatus(uint16_t connHandle)
{
    RRSP_RegistrationStatus_e retVal = RRSP_UNREGISTER;
    // Check if the connection handle is registered to RAS service
    if (connHandle < MAX_NUM_BLE_CONNS)
    {
        retVal = gRAPControlBlock.rrspRegisteredData[connHandle];
    }

    return retVal;
}

/*********************************************************************
 * @fn      RRSP_AddSubeventResult
 *
 * @brief   This function adds a subevent result to the ranging procedure DB.
 *
 * @param   pSubeventResult - Pointer to the subevent result data to be added.
 *
 * @return SUCCESS or stack call status
 */
uint8_t RRSP_AddSubeventResult(RRSP_csSubEventResults_t* pSubeventResult)
{
    // Result to send to the RAS profile
    uint8_t status;
    RangingDBServer_procedureId_t procedureID;
    uint16_t parsedResultLen;
    uint8_t *parsedResultData = NULL;
    // Set procedureID
    procedureID.connHandle = pSubeventResult->connHandle;
    procedureID.rangingCounter = pSubeventResult->procedureCounter;

    // set receivingSubeventProcess
    gRAPControlBlock.receivingSubeventProcess.rangingCounter = pSubeventResult->procedureCounter;
    gRAPControlBlock.receivingSubeventProcess.subeventNumOfSteps = pSubeventResult->numStepsReported;
    gRAPControlBlock.receivingSubeventProcess.startOfSubeventoffset = RangingDBServer_GetCurrentDataLen(procedureID);

    // Parse the subevent from CS subevent to RAS CS subevent
    parsedResultLen = pSubeventResult->dataLen + sizeof(Ranging_subEventHeader_t) - pSubeventResult->numStepsReported * 2;
    parsedResultData = (uint8_t *)ICall_malloc(parsedResultLen);
    if (parsedResultData == NULL)
    {
        status = bleNoResources;
    }
    else
    {
        // Parse the subevent
        status = rangingSubeventParser(pSubeventResult, parsedResultData);

        // If parsed data successfully add it to the Database
        if(status == SUCCESS)
        {
            // Send the merged data to the database
            status = RangingDBServer_AddData(procedureID, parsedResultData, parsedResultLen);
        }

        // Free parsedResultData
        ICall_free(parsedResultData);
    }

    return status;
}

/*********************************************************************
 * @fn      RRSP_AddSubeventContinueResult
 *
 * @brief   This function adds a subevent continue result to the ranging procedure DB.
 *
 * @param   pSubeventContResult - Pointer to the continue subevent result data to be added.
 *
 * @return SUCCESS or stack call status
 */
uint8_t RRSP_AddSubeventContinueResult(RRSP_csSubEventResultsContinue_t* pSubeventContResult)
{
    // Result to send to the RAS profile
    uint8_t status;
    uint8_t eventStatusData; // Holds both procedure and subevent statuses
    RangingDBServer_procedureId_t procedureID;
    uint16_t parsedResultLen;
    uint8_t *parsedResultData = NULL;
    // Set procedureID
    procedureID.connHandle = pSubeventContResult->connHandle;
    procedureID.rangingCounter = gRAPControlBlock.receivingSubeventProcess.rangingCounter;

    // Increment the numOfStep in this subevent. Will be used in the end of the subevent.
    gRAPControlBlock.receivingSubeventProcess.subeventNumOfSteps += pSubeventContResult->numStepsReported;

    // Parse the continue subevent to a RAS CS subevent
    parsedResultLen = pSubeventContResult->dataLen - pSubeventContResult->numStepsReported * 2;
    parsedResultData = (uint8_t *)ICall_malloc(parsedResultLen);
    if (parsedResultData == NULL)
    {
        status = bleNoResources;
    }
    else
    {
        // Parse the subevent
        status = rangingSubeventContParser(pSubeventContResult, parsedResultData);

        if(status == SUCCESS)
        {
            // Send the merged data to the database
            status = RangingDBServer_AddData(procedureID, parsedResultData, parsedResultLen);
        }

        // Update subevent and procedure statuses in the subevent header
        if (status == SUCCESS)
        {
            eventStatusData = BUILD_UINT8(pSubeventContResult->subeventDoneStatus, pSubeventContResult->procedureDoneStatus);
            status = RangingDBServer_UpdateData(procedureID, &eventStatusData, SUBEVENT_STATUSES_SIZE,
                                                gRAPControlBlock.receivingSubeventProcess.startOfSubeventoffset + SUBEVENT_STATUSES_INDEX);
        }

        // The number of steps is known only in the end of the subevent,
        // so when the subevent has ended, update the num of steps in the DB.
        if (status == SUCCESS &&
            pSubeventContResult->subeventDoneStatus == CS_PROCEDURE_DONE)
        {
            status = RangingDBServer_UpdateData(procedureID,
                                                (uint8_t*)&gRAPControlBlock.receivingSubeventProcess.subeventNumOfSteps,
                                                sizeof(gRAPControlBlock.receivingSubeventProcess.subeventNumOfSteps),
                                                gRAPControlBlock.receivingSubeventProcess.startOfSubeventoffset + SUBEVENT_STEPS_NUM_INDEX);
        }

        // Free parsedResultData
        ICall_free(parsedResultData);
    }

    return status;
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      rrspCccUpdateCB
 *
 * @brief   Callback from Ranging Server indicating CCC has been updated
 *
 * @param   pValue - pointer to CP request
 *
 * @return  None
 */
void rrspCccUpdateCB(char *pValue)
{
  RAS_cccUpdate_t *rasCCCUpdate = (RAS_cccUpdate_t*)pValue;

  // Save the CCC update in the control block
  rrspSaveRegistrationStatus(rasCCCUpdate);

  if( (gRAPControlBlock.ranging_appCB) != NULL &&
      (gRAPControlBlock.ranging_appCB->pfnOnCccUpdateCB != NULL) )
  {
    // Send the CCC update to the application
    gRAPControlBlock.ranging_appCB->pfnOnCccUpdateCB(rasCCCUpdate->connHandle, rasCCCUpdate->value);
  }
}

/*********************************************************************
 * @fn      rrspCPReqCB
 *
 * @brief   Callback from Ranging Server indicating CP request has been updated
 *          for Record Access Control Point characteristic
 *
 * @param   pValue - pointer to CP request
 *
 * @return  None
 */
void rrspCPReqCB(char *pValue)
{
    RAS_CPCB_t *CPReq;
    RangingDBServer_procedureId_t procedureID;
    uint8_t status = SUCCESS;
    // Verify input parameters
    if ( pValue != NULL )
    {
        // Cast the pointer to the correct type
        CPReq = (RAS_CPCB_t *)pValue;
        // Set procedure ID
        procedureID.connHandle = CPReq->connHandle;
        procedureID.rangingCounter = CPReq->CPMsg.param1;


        if (gRAPControlBlock.rrspSegmentationProcess.busy == TRUE)
        {
            // Server is busy, send response
            status = rrspSendErrorResponse(procedureID.connHandle, RAS_CP_OPCODE_RSP_CODE, RAS_CP_RSP_CODE_SERVER_BUSY);
            if (status != SUCCESS)
            {
                // Mark server sould send "server busy" error response
                gRAPControlBlock.rrspSegmentationProcess.waitForBusy = TRUE;
            }
        }
        else
        {
            // Find out the request opCode
            switch ( CPReq->CPMsg.opCode )
            {
                /******************************************************/
                /******** Request to send stored recodes **************/
                /******************************************************/
                case RAS_CP_OPCODE_GET_RANGING_DATA:
                {
                    if ( CPReq->msgLength != RAS_CP_GET_DATA_CMD_LEN )
                    {
                        // Invalid length of the request, send response
                        rrspSendErrorResponse(procedureID.connHandle, RAS_CP_OPCODE_RSP_CODE, RAS_CP_RSP_CODE_VAL_INVALID_PARAM);
                    }
                    else if (RangingDBServer_isProcedureExist(procedureID) == FALSE)
                    {
                        // Procedure ID does not exist in the database, send response
                        rrspSendErrorResponse(procedureID.connHandle, RAS_CP_OPCODE_RSP_CODE, RAS_CP_RSP_CODE_NO_RECORDS_FOUND);
                    }
                    else
                    {
                        // Handle the request to get ranging data
                        rrspHandleGetRangingData(procedureID.connHandle, procedureID.rangingCounter);
                    }
                    break;
                }
                case RAS_CP_OPCODE_ACK_RANGING_DATA:
                {
                    if ( CPReq->msgLength != RAS_CP_ACK_DATA_CMD_LEN )
                    {
                        // Invalid length of the request, send response
                        rrspSendErrorResponse(procedureID.connHandle, RAS_CP_OPCODE_RSP_CODE, RAS_CP_RSP_CODE_VAL_INVALID_PARAM);
                    }
                    else if (RangingDBServer_isProcedureExist(procedureID) == FALSE)
                    {
                        // Procedure ID does not exist in the database, send response
                        rrspSendErrorResponse(procedureID.connHandle, RAS_CP_OPCODE_RSP_CODE, RAS_CP_RSP_CODE_NO_RECORDS_FOUND);
                    }
                    else
                    {
                        // Handle the acknowledgment of ranging data
                        rrspHandleAckRangingData(procedureID.connHandle, procedureID.rangingCounter);
                    }
                    break;
                }
                default:
                {
                    // Unsupported opcode, send response
                    rrspSendErrorResponse(procedureID.connHandle, RAS_CP_OPCODE_RSP_CODE, RAS_CP_RSP_CODE_VAL_OPCODE_NOT_SUPPORTED);
                    break;
                }
            }
        }
    }

}

/**********************************************************************
 * @fn      rrspSaveRegistrationStatus
 *
 * @brief   This function saves the registration status of the CCC update
 *
 * @param   rasCCCUpdate - Pointer to the CCC update structure
 *
 * @return  None
 */
void rrspSaveRegistrationStatus(RAS_cccUpdate_t *rasCCCUpdate)
{

  // Check if the CCC update is valid
  if( (rasCCCUpdate != NULL) && (rasCCCUpdate->connHandle < MAX_NUM_BLE_CONNS) )
  {
    // Check if the UUID is RAS_REAL_TIME_UUID
    if(rasCCCUpdate->uuid == RAS_REAL_TIME_UUID)
    {
        // Check the value of the CCCD
        if( (rasCCCUpdate->value & GATT_CLIENT_CFG_NOTIFY) != 0 )
        {
            gRAPControlBlock.rrspRegisteredData[rasCCCUpdate->connHandle] = RRSP_REAL_TIME_NOTI;
        }
        else if( (rasCCCUpdate->value & GATT_CLIENT_CFG_INDICATE) != 0 )
        {
            gRAPControlBlock.rrspRegisteredData[rasCCCUpdate->connHandle] = RRSP_REAL_TIME_IND;
        }
        else // rasCCCUpdate->value == 0
        {
            gRAPControlBlock.rrspRegisteredData[rasCCCUpdate->connHandle] = RRSP_UNREGISTER;
        }
    }
    // Check if the UUID is RAS_ON_DEMAND_UUID
    else if(rasCCCUpdate->uuid == RAS_ON_DEMAND_UUID)
    {
        // Check the value of the CCCD
        if( (rasCCCUpdate->value & GATT_CLIENT_CFG_NOTIFY) != 0 )
        {
            gRAPControlBlock.rrspRegisteredData[rasCCCUpdate->connHandle] = RRSP_ON_DEMAND_NOTI;
        }
        else if( (rasCCCUpdate->value & GATT_CLIENT_CFG_INDICATE) != 0 )
        {
            gRAPControlBlock.rrspRegisteredData[rasCCCUpdate->connHandle] = RRSP_ON_DEMAND_IND;
        }
        else // rasCCCUpdate->value == 0
        {
            gRAPControlBlock.rrspRegisteredData[rasCCCUpdate->connHandle] = RRSP_UNREGISTER;
        }
    }
  }
}

/**********************************************************************
 * @fn      rrspSendErrorResponse
 *
 * @brief   This function sends an error response to the client.
 *
 * @param   connHandle - The connection handle to reply to
 * @param   responseCode - The response code to send
 * @param   reason - The reason for the error
 *
 * @return  SUCCESS or error status
 */
uint8_t rrspSendErrorResponse(uint16_t connHandle, uint8_t responseCode, uint8_t reason)
{
    uint8_t rsp[RAS_CP_RSP_CODE_LEN];
    // Ranging Response Opcode
    rsp[0] = responseCode;
    // Reason for the error
    rsp[1] = reason;

    // send response
    return RAS_setParameter(connHandle, RAS_CONTROL_POINT_ID, rsp, RAS_CP_RSP_CODE_LEN );
}

/*********************************************************************
 * @fn      rrspHandleAckRangingData
 *
 * @brief   Handles the acknowledgment of ranging data.
 *
 * @param   connHandle - Connection handle of the device.
 * @param   rangingCounter - Counter for the ranging procedure.
 *
 * @return  None
 */
void rrspHandleAckRangingData(uint16_t connHandle, uint16_t rangingCounter)
{
    RAS_cpMsg_t CPMsgRsp = {0};
    RangingDBServer_procedureId_t procedureID;
    // Prepare the response for the Ranging Data ack
    CPMsgRsp.opCode = RAS_CP_OPCODE_RSP_CODE;
    CPMsgRsp.param1 = RAS_CP_RSP_CODE_VAL_SUCCESS;

    // Send the response
    RAS_setParameter(connHandle, RAS_CONTROL_POINT_ID, &CPMsgRsp, RAS_CP_RSP_CODE_LEN);

    procedureID.connHandle = connHandle;
    procedureID.rangingCounter = rangingCounter;
    // Clear the procedure for the given connection handle
    RangingDBServer_ClearProcedure(procedureID);

    // Reset the segmentation process
    gRAPControlBlock.rrspSegmentationProcess.segmentCounter = 0;
    // Reset the last segment flag
    gRAPControlBlock.rrspSegmentationProcess.lastSegment = FALSE;
    // Set the ranging counter to an invalid value
    gRAPControlBlock.rrspSegmentationProcess.rangingCounter = RRSP_RANGING_COUNTER_INVALID_VALUE;

    if(gRAPControlBlock.ranging_appCB != NULL && gRAPControlBlock.ranging_appCB->pfnStatusUpdateCB != NULL)
    {
      gRAPControlBlock.ranging_appCB->pfnStatusUpdateCB(RRSP_SENDING_PROCEDURE_ENDED, connHandle, rangingCounter);
    }
}

/*********************************************************************
 * @fn      rrspHandleGetRangingData
 *
 * @brief   Handles the process of retrieving and sending ranging data.
 *
 * @param   connHandle - Connection handle of the device.
 * @param   rangingCounter - Counter for the ranging procedure.
 *
 * @return  None
 */
void rrspHandleGetRangingData(uint16_t connHandle, uint16_t rangingCounter)
{
    // Mark the segmentation process as busy
    gRAPControlBlock.rrspSegmentationProcess.busy = TRUE;

    // Reset the segment counter
    gRAPControlBlock.rrspSegmentationProcess.segmentCounter = 0;

    // Reset the last segment flag
    gRAPControlBlock.rrspSegmentationProcess.lastSegment = FALSE;

    // Set the ranging counter
    gRAPControlBlock.rrspSegmentationProcess.rangingCounter = rangingCounter;

    if(gRAPControlBlock.rrspRegisteredData[connHandle] == RRSP_ON_DEMAND_NOTI ||
       gRAPControlBlock.rrspRegisteredData[connHandle] == RRSP_REAL_TIME_NOTI)
    {
        // Register the application callback function for Flow Control
        L2CAP_RegisterFlowCtrlTask(BLEAppUtil_getSelfEntity());

       // If the connection handle is registered for notification, send all segments synchronously
        rrspSendSegmentsNoti(connHandle, rangingCounter);
    }
    else if( gRAPControlBlock.rrspRegisteredData[connHandle] == RRSP_ON_DEMAND_IND ||
             gRAPControlBlock.rrspRegisteredData[connHandle] == RRSP_REAL_TIME_IND)
    {
        // If the connection handle is registered for indication, send the segments one by one
        rrspSendSegmentsIndi(connHandle, rangingCounter);
    }

    if(gRAPControlBlock.ranging_appCB != NULL && gRAPControlBlock.ranging_appCB->pfnStatusUpdateCB != NULL)
    {
      gRAPControlBlock.ranging_appCB->pfnStatusUpdateCB(RRSP_SENDING_PROCEDURE_STARTED, connHandle, rangingCounter);
    }
}

/*********************************************************************
 * @fn      rrspSendSegmentsIndi
 *
 * @brief   This function sends segments for indication mode.
 *          It sends one segment at a time and waits for an asynchronous
 *          confirmation event (indication acknowledgment) from the peer
 *          device before sending the next segment, continuing this process
 *          until all data is sent.
 *
 * @param   connHandle - Connection handle of the device to send data to.
 * @param   rangingCounter - Counter for the ranging procedure.
 *
 * @return  uint8_t - Status of the operation (SUCCESS or error code).
 */
uint8_t rrspSendSegmentsIndi(uint16_t connHandle, uint16_t rangingCounter)
{
    uint8_t status = USUCCESS;
    RRSP_Segment_t segment;
    uint16_t offset;
    uint8_t endOfData;
    linkDBInfo_t connInfo = {0};
    RangingDBServer_procedureId_t procedureID;

    if( gRAPControlBlock.rrspSegmentationProcess.segmentCounter == 0)
    {
        // Mark the first segment
        segment.segmentationHeader.firstSegment = TRUE;
    }
    else
    {
        // Mark the first segment as not the first one
        segment.segmentationHeader.firstSegment = FALSE;
    }

    // get connection information
    status = linkDB_GetInfo(connHandle, &connInfo);

    if(status == SUCCESS)
    {
        // Set procedureID
        procedureID.connHandle = connHandle;
        procedureID.rangingCounter = rangingCounter;

        // Set the ranging counter in the segmentation process
        segment.segmentationHeader.segmentIndex = (gRAPControlBlock.rrspSegmentationProcess.segmentCounter & RRSP_6_LSB_MASK);

        // Calculate the offset for the segment
        offset = rrspCalcSegmentOffset(gRAPControlBlock.rrspSegmentationProcess.segmentCounter, connInfo.MTU);

        // Get the length of the data for the segment
        uint16_t len = RRSP_SEGMENT_MAX_SIZE(connInfo.MTU);

        uint8_t* pData = RangingDBServer_GetData(procedureID, &len, offset, &endOfData); // Get the data for the segment

        if ((pData != NULL) && (len <= ATT_MAX_MTU_SIZE) && (len <= sizeof(segment.segmentData)))
        {
            // Set the segment data
            memcpy(segment.segmentData, pData, len); // Copy the data to the segment data

            // Check if this is the last segment
            if(endOfData == UTRUE)
            {
                segment.segmentationHeader.lastSegment = TRUE; // Mark the segment itself as the last one.
                gRAPControlBlock.rrspSegmentationProcess.lastSegment = TRUE; // Set the last segment flag in the segmentation process
            }
            else
            {
                segment.segmentationHeader.lastSegment = FALSE; // Set the segment itself as not the last one.
            }

            // Set waiting for new packet flag
            gRAPControlBlock.rrspSegmentationProcess.waitForConf = TRUE;

            // Increment the segment counter for next request
            gRAPControlBlock.rrspSegmentationProcess.segmentCounter++;

            // Send the segment via server
            status = RAS_setParameter(connHandle, RAS_ON_DEMAND_ID, &segment, len + RRSP_SEGMENT_HDR_SIZE);
        }
        else
        {
          status = bleInvalidRange; // No data available for the given connection handle
        }
    }
    return status;
}

/*********************************************************************
 * @fn      rrspSendSegmentsNoti
 *
 * @brief   This function attempts to send all segments for notification mode
 *          in a loop, but stops if the TX buffer is full (e.g., blePending is returned).
 *          In that case, it waits for an asynchronous event indicating that
 *          data has been sent, and then this function is called again to continue
 *          sending the remaining data.
 *
 * @param   connHandle - Connection handle of the device to send data to.
 * @param   rangingCounter - Counter for the ranging procedure.
 *
 * @return  uint8_t - Status of the operation (SUCCESS or error code).
 */
uint8_t rrspSendSegmentsNoti(uint16_t connHandle, uint16_t rangingCounter)
{
    uint8_t status = USUCCESS;
    RRSP_Segment_t segment = {0};
    uint16_t offset = 0;
    uint8_t endOfData = FALSE;
    linkDBInfo_t connInfo = {0};
    RangingDBServer_procedureId_t procedureID;
    uint16_t len = 0;
    uint8_t* pData = NULL;
    RAS_cpMsg_t CPRsp = {0};

    // Get connection information
    status = linkDB_GetInfo(connHandle, &connInfo);
    if(status != SUCCESS)
    {
        return status;
    }

    // Set the procedure ID
    procedureID.connHandle = connHandle;
    procedureID.rangingCounter = rangingCounter;

    // Mark as busy to prevent other requests
    gRAPControlBlock.rrspSegmentationProcess.busy = TRUE;

    // Mark server as waiting for sending more segments
    gRAPControlBlock.rrspSegmentationProcess.waitForNoti = TRUE;

    // Save the connHandle for later use in L2CAP event handler
    gRAPControlBlock.rrspSegmentationProcess.currentConnHandle = connHandle;

    // Send segments until all data is sent or blePending action is returned.
    // If blePending is returned, the process will be continued in the callback function. @ref rrspL2CapEventHandler
    // The callback will be called when the L2CAP flow control task is ready to send more segments.
    do
    {
        // Prepare the segment header
        segment.segmentationHeader.segmentIndex = (gRAPControlBlock.rrspSegmentationProcess.segmentCounter & RRSP_6_LSB_MASK);
        if(segment.segmentationHeader.segmentIndex == 0)
        {
            segment.segmentationHeader.firstSegment = TRUE;
        }
        else
        {
            segment.segmentationHeader.firstSegment = FALSE;
        }

        // Calculate the offset for the segment
        offset = rrspCalcSegmentOffset(gRAPControlBlock.rrspSegmentationProcess.segmentCounter, connInfo.MTU);

        // Set the len for the segment
        len = RRSP_SEGMENT_MAX_SIZE(connInfo.MTU);

        // Get the data for the segment
        pData = RangingDBServer_GetData(procedureID, &len, offset, &endOfData);

        if(pData == NULL)
        {
            status = bleInvalidRange;
            // Unregister the application callback function for Flow Control
            L2CAP_RegisterFlowCtrlTask(INVALID_TASK_ID);
            return status;
        }

        // Copy the data to the segment
        memcpy(segment.segmentData, pData, len);

        // Check if this is the last segment
        if(endOfData == UTRUE)
        {
            segment.segmentationHeader.lastSegment = TRUE;
            gRAPControlBlock.rrspSegmentationProcess.lastSegment = TRUE;
        }
        else
        {
            segment.segmentationHeader.lastSegment = FALSE;
            gRAPControlBlock.rrspSegmentationProcess.lastSegment = FALSE;
        }

        // Send the segment via server
        status = RAS_setParameter(connHandle, RAS_ON_DEMAND_ID, &segment, len + RRSP_SEGMENT_HDR_SIZE);

        if(status == blePending)
        {
            // If the segment sending failed, mark the last segment as FALSE
            // to indicate that the process is not complete
            gRAPControlBlock.rrspSegmentationProcess.lastSegment = FALSE;
            return status;
        }

        // Update the segment index
        gRAPControlBlock.rrspSegmentationProcess.segmentCounter++;

    } while(endOfData == FALSE);

    // Send complete data response to client
    CPRsp.opCode = RAS_CP_OPCODE_COMPLETE_DATA_RSP;
    CPRsp.param1 = rangingCounter;
    status = RAS_setParameter(connHandle, RAS_CONTROL_POINT_ID, &CPRsp, RAS_CP_RSP_COMPLETE_DATA_RSP_LEN);

    if(status != blePending)
    {
        // Unregister the application callback function for Flow Control
        L2CAP_RegisterFlowCtrlTask(INVALID_TASK_ID);

        // Mark as not busy after all segments sent and Complete Data Response sent
        gRAPControlBlock.rrspSegmentationProcess.busy = FALSE;

        // Mark the segmentation process as ended
        gRAPControlBlock.rrspSegmentationProcess.waitForNoti = FALSE;
    }

    return status;
}

/*********************************************************************
 * @fn      rrspCalcSegmentOffset
 *
 * @brief   This function calculates the offset for the segment based on the segment index.
 *
 * @param   segmentIndex - Index of the segment to calculate the offset for.
 *
 * @return  Offset value for the segment.
 */
uint16_t rrspCalcSegmentOffset(uint8_t segmentIndex, uint16_t connMtu)
{
  // Calculate the offset based on the segment index and the segment raw data size
  return (segmentIndex * RRSP_SEGMENT_MAX_SIZE(connMtu));
}

/*********************************************************************
  * @fn      sendOverWritten
  *
  * @brief   This function sends an overwritten message to the peer device.
  *
  * @param   connHandle - Connection handle of the device.
  * @param   rangingCounter - Counter for the ranging procedure.
  *
  * @return  uint8_t - Status of the operation (SUCCESS or error code).
  */
uint8_t sendOverWritten(uint16_t connHandle, uint16_t rangingCounter)
{
    uint8_t status = RAS_setParameter(connHandle, RAS_OVERWRITTEN_ID, &rangingCounter, RAS_OVERWRITTEN_LEN );
    return status;
}

/*********************************************************************
 * @fn      rangingSubeventParser
 * @brief   Parses a ranging subevent result, including its header and data.
 *
 * @param   subeventResult - Pointer to the subevent result structure.
 * @param   pParsedData    - Pointer to the buffer where parsed data will be stored.
 *
 * @return  SUCCESS (0) or FAILURE (1)
 */
uint8_t rangingSubeventParser(RRSP_csSubEventResults_t* subeventResult, uint8_t *pParsedData)
{
    Ranging_subEventHeader_t subEventAttr;
    uint8_t status = SUCCESS;

    /** Parse subeventHeader **/
    subEventAttr.freqCompenstation = subeventResult->frequencyCompensation;
    subEventAttr.numStepsReported = subeventResult->numStepsReported;
    // abort reason is the 0-3 bites of the rangingAbortReason
    subEventAttr.rangingAbortReason = subeventResult->abortReason & RRSP_ABORT_REASON_MASK;
    // rangingDoneStatus set this only when receiving this status
    subEventAttr.rangingDoneStatus = subeventResult->procedureDoneStatus;
    subEventAttr.referencePowerLvl = subeventResult->referencePowerLevel;
    subEventAttr.startAclConnEvt = subeventResult->startAclConnectionEvent;
    // subeventAbortReason is the 4-7 bites of the rangingAbortReason
    subEventAttr.subeventAbortReason = subeventResult->abortReason >> 4;
    // subeventDoneStatus set this only when receiving this status
    subEventAttr.subeventDoneStatus = subeventResult->subeventDoneStatus;

    // copy the subevent header to the parsed data buffer
    memcpy(pParsedData, &subEventAttr, sizeof(Ranging_subEventHeader_t));

    // Parse the subevent data
    status = RangingDataParser(subeventResult->data, subeventResult->dataLen, subeventResult->numStepsReported,
                               pParsedData + sizeof(Ranging_subEventHeader_t));

    return status;
}

/*********************************************************************
 * @fn      rangingSubeventContParser
 * @brief   Parses a ranging subevent continuation result, Need to parse the data only
 *
 * @param   subeventResultCont - Pointer to the subevent continuation result structure.
 * @param   pParsedData        - Pointer to the buffer where parsed data will be stored.
 *
 * @return  SUCCESS (0) or FAILURE (1)
 */
uint8_t rangingSubeventContParser(RRSP_csSubEventResultsContinue_t* subeventResultCont, uint8_t *pParsedData)
{
    uint8_t status = SUCCESS;

    // Parse the continuation data
    status = RangingDataParser(subeventResultCont->data, subeventResultCont->dataLen, subeventResultCont->numStepsReported, pParsedData);

    return status;
}

/*********************************************************************
 * @fn    RangingDataParser
 * @brief This function parses raw ranging subevent data.
 *
 * @param  pData       - Pointer to the raw input data.
 * @param  datalen     - Length of the raw input data.
 * @param  numSteps    - Number of steps reported in the data.
 * @param  pParsedData - Pointer to the buffer where parsed data will be stored.
 *
 * @return SUCCESS (0) or FAILURE (1)
 */
uint8_t RangingDataParser(uint8_t *pData, uint16_t datalen, uint8_t numSteps, uint8_t *pParsedData)
{
    // Ensure the input pointers are valid
    if (pData == NULL || pParsedData == NULL)
    {
        return FAILURE; // Invalid input
    }

    // Ensure the input data length is sufficient for the number of steps
    if (datalen < (numSteps * RRSP_SIZE_OF_STEP))
    {
        return FAILURE; // Not enough data for the reported steps
    }

    // Iterate over the input data
    for (uint8_t step = 0; step < numSteps; step++)
    {
        // Copy stepMode
        memcpy(pParsedData, pData, 1);

        // Increment pointers
        pParsedData ++;
        pData ++;
        datalen --;

        // Ensure there is enough data for stepChannel and stepLength
        if (datalen < RRSP_SIZE_OF_STEP)
        {
            return FAILURE; // Not enough data, parsing failed
        }

        // Extract stepChannel (1 byte) and stepLength (1 byte)
        uint8_t stepChannel = *pData++;
        uint8_t stepLength = *pData++;
        datalen -= 2;

        // Ensure stepLength is valid and does not exceed remaining data
        if (stepLength > datalen)
        {
            return FAILURE; // Invalid data, parsing failed
        }

        // Copy stepData (stepLength bytes) using memcpy
        memcpy(pParsedData, pData, stepLength);
        pParsedData += stepLength;

        // Move the input data pointer to the next step start
        pData += stepLength;
        datalen -= stepLength;
    }

    // Parsing completed successfully
    return SUCCESS;
}

/*********************************************************************
 * @fn      rrspConnEventHandler
 *
 * @brief   The purpose of this function is to handle connection related
 *          events that rise from the GAP and were registered in
 *          @ref BLEAppUtil_registerEventHandler
 *
 * @param   event - message event.
 * @param   pMsgData - pointer to message data.
 *
 * @return  none
 */
void rrspConnEventHandler(uint32 event, BLEAppUtil_msgHdr_t *pMsgData)
{
  if (pMsgData != NULL)
  {
      if (event == BLEAPPUTIL_LINK_TERMINATED_EVENT)
      {
          // Remove the connection from the registration list
          gapTerminateLinkEvent_t *pGapTermMsg = (gapTerminateLinkEvent_t *)pMsgData;
          if (pGapTermMsg->connectionHandle < MAX_NUM_BLE_CONNS)
          {
              // Reset the registration status for the connection handle
              gRAPControlBlock.rrspRegisteredData[pGapTermMsg->connectionHandle] = RRSP_UNREGISTER;

              // Clear the segmentation process for the connection handle
              RangingDBServer_ClearConnHandle(pGapTermMsg->connectionHandle);
          }
      }
  }
}

/*********************************************************************
 * @fn      rrspL2CapEventHandler
 *
 * @brief   Handles L2CAP events (BLEAPPUTIL_L2CAP_NUM_CTRL_DATA_PKT_EVT events).
 *
 * @param   event - message event.
 * @param   pMsgData - pointer to message data.
 *
 * @return  none
 */
void rrspL2CapEventHandler(uint32 event, BLEAppUtil_msgHdr_t *pMsgData)
{
    RAS_cpMsg_t CPRsp = {0};
    uint8_t status = SUCCESS;
    // Use the saved connHandle from the segmentation process
    uint16_t connHandle = gRAPControlBlock.rrspSegmentationProcess.currentConnHandle;
    uint8_t rangingCounter = gRAPControlBlock.rrspSegmentationProcess.rangingCounter;
    switch (event)
    {
        case BLEAPPUTIL_L2CAP_NUM_CTRL_DATA_PKT_EVT:
        {
            if (gRAPControlBlock.rrspSegmentationProcess.waitForBusy == TRUE)
            {
                // If the server is waiting for sending busy response, send it now
                status = rrspSendErrorResponse(connHandle, RAS_CP_OPCODE_RSP_CODE, RAS_CP_RSP_CODE_SERVER_BUSY);
                if (status == SUCCESS)
                {
                    // Mark server not waiting for sending busy response
                    gRAPControlBlock.rrspSegmentationProcess.waitForBusy = FALSE;
                }
            }
            else if (gRAPControlBlock.rrspSegmentationProcess.waitForNoti == TRUE)
            {
                // If the server is waiting for sending segments, continue sending segments
                if(gRAPControlBlock.rrspSegmentationProcess.lastSegment == FALSE)
                {
                    rrspSendSegmentsNoti(connHandle, rangingCounter);
                }
                else // if(gRAPControlBlock.rrspSegmentationProcess.lastSegment == TRUE)
                {
                    // Send complete data response to client
                    CPRsp.opCode = RAS_CP_OPCODE_COMPLETE_DATA_RSP;
                    CPRsp.param1 = rangingCounter;
                    RAS_setParameter(connHandle, RAS_CONTROL_POINT_ID, &CPRsp, RAS_CP_RSP_COMPLETE_DATA_RSP_LEN);

                    // Unregister the application callback function for Flow Control
                    L2CAP_RegisterFlowCtrlTask(INVALID_TASK_ID);
                }
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
 * @fn      rrspGattEventHandler
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
void rrspGattEventHandler(uint32 event, BLEAppUtil_msgHdr_t *pMsgData)
{
    gattMsgEvent_t *gattMsg;
    uint16_t connHandle;
    RAS_cpMsg_t CPRsp = {0};
    if (pMsgData != NULL)
    {
        switch(event)
        {
            // An indication confirmation packet was received
            case BLEAPPUTIL_ATT_HANDLE_VALUE_CFM:
            {
                // Check if the status is success
                if(pMsgData->status == SUCCESS)
                {
                    gattMsg = (gattMsgEvent_t *)pMsgData;
                    connHandle = gattMsg->connHandle;

                    // Check if the profile waits for confirmation
                    if(gRAPControlBlock.rrspSegmentationProcess.waitForConf == TRUE)
                    {
                        if(gRAPControlBlock.rrspSegmentationProcess.lastSegment == TRUE)
                        {
                            // Sending is finished, don't wait for next packet.
                            gRAPControlBlock.rrspSegmentationProcess.waitForConf = FALSE;

                            // Prepare the response for the Ranging Data request
                            CPRsp.opCode = RAS_CP_OPCODE_COMPLETE_DATA_RSP;

                            // The ranging counter for the request
                            CPRsp.param1 = gRAPControlBlock.rrspSegmentationProcess.rangingCounter;

                            // Send the response
                            RAS_setParameter(connHandle, RAS_CONTROL_POINT_ID, &CPRsp, RAS_CP_RSP_COMPLETE_DATA_RSP_LEN);

                            // Set profile as not busy
                            gRAPControlBlock.rrspSegmentationProcess.busy = FALSE;
                        }
                        else
                        {
                            // Continue sending segments
                            rrspSendSegmentsIndi(connHandle, gRAPControlBlock.rrspSegmentationProcess.rangingCounter);
                        }
                    }
                }
                break;
            }

            default:
            {
                break;
            }
        }
    }

    return;
}
