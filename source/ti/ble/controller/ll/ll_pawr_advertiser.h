/******************************************************************************

 @file  ll_pawr_advertiser.h

 @brief This file contains the Link Layer (LL) types, constants,
        API's etc. for the Bluetooth Low Energy (BLE) Controller
        Periodic Advertising with Responses Advertiser.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025-2026, Texas Instruments Incorporated

 All rights reserved not granted herein.
 Limited License.

 Texas Instruments Incorporated grants a world-wide, royalty-free,
 non-exclusive license under copyrights and patents it now or hereafter
 owns or controls to make, have made, use, import, offer to sell and sell
 ("Utilize") this software subject to the terms herein. With respect to the
 foregoing patent license, such license is granted solely to the extent that
 any such patent is necessary to Utilize the software alone. The patent
 license shall not apply to any combinations which include this software,
 other than combinations with devices manufactured by or for TI ("TI
 Devices"). No hardware patent is licensed hereunder.

 Redistributions must preserve existing copyright notices and reproduce
 this license (including the above copyright notice and the disclaimer and
 (if applicable) source code license limitations below) in the documentation
 and/or other materials provided with the distribution.

 Redistribution and use in binary form, without modification, are permitted
 provided that the following conditions are met:

   * No reverse engineering, decompilation, or disassembly of this software
     is permitted with respect to any software provided in binary form.
   * Any redistribution and use are licensed by TI for use only with TI Devices.
   * Nothing shall obligate TI to provide you with source code for the software
     licensed and provided to you in object code.

 If software source code is provided to you, modification and redistribution
 of the source code are permitted provided that the following conditions are
 met:

   * Any redistribution and use of the source code, including any resulting
     derivative works, are licensed by TI for use only with TI Devices.
   * Any redistribution and use of any object code compiled from the source
     code and any resulting derivative works, are licensed by TI for use
     only with TI Devices.

 Neither the name of Texas Instruments Incorporated nor the names of its
 suppliers may be used to endorse or promote products derived from this
 software without specific prior written permission.

 DISCLAIMER.

 THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

/*********************************************************************
 *
 * WARNING!!!
 *
 * THE API'S FOUND IN THIS FILE ARE FOR INTERNAL STACK USE ONLY!
 * FUNCTIONS SHOULD NOT BE CALLED DIRECTLY FROM APPLICATIONS, AND ANY
 * CALLS TO THESE FUNCTIONS FROM OUTSIDE OF THE STACK MAY RESULT IN
 * UNEXPECTED BEHAVIOR.
 *
 */

#ifndef LL_PAWR_ADVERTISER_H
#define LL_PAWR_ADVERTISER_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
* INCLUDES
******************************************************************************/
#include <stdint.h>
#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll_pawr_common.h"
#include "ti/ble/controller/ll/ll.h"
#include "ti/ble/controller/ll/ll_ble.h"
#include "ti/ble/controller/ll/ll_ae.h"
#include "ti/ble/controller/ll/ll_rat.h"

/*******************************************************************************
* MACROS
******************************************************************************/

/*******************************************************************************
* CONSTANTS
******************************************************************************/
#define SUBEVENT_LIST_SIZE 4
#define RESPONSE_LIST_SIZE 3
#define SUBEVENT_MAX_SIZE 251
#define RESPONSE_MAX_SIZE 251

#define INIT_SUBEVENT_DATA_REQUEST_PROC   0x00
#define SUBEVENT_DATA_REQUEST_PROC_ACTIVE 0x01

// txStatus values for HCI_PadvAResponseReportEvent
// Indicates whether AUX_SYNC_SUBEVENT_IND was transmitted for the subevent
#define PAWR_TX_STATUS_IND_TRANSMITTED      0x00  // AUX_SYNC_SUBEVENT_IND was transmitted
#define PAWR_TX_STATUS_IND_NOT_TRANSMITTED  0x01  // AUX_SYNC_SUBEVENT_IND was not transmitted (subevent missed)

/*******************************************************************************
* TYPEDEFS
******************************************************************************/

typedef struct responseReport_t
{
     int8_t  txPower;                            // TxPower value of the response report (signed)
     int8_t  rssi;                               // RSSI level for the response report (signed)
     uint8_t responseSlot;                       // Report response slot
     uint8_t dataStatus;                         // Data status of the event - received or not
     uint8_t dataLen;                            // Length of data
     uint8_t data[RESPONSE_MAX_SIZE];            // Data of the response
} responseReport_t;

// Holds parameters for a specific subevent.
typedef struct subeventProps_t
{
  uint8_t subeventNum;
  uint8_t rspSlotStart;                      // First response to listen
  uint8_t rspSlotCount;                      // How many responses to listen
  uint8_t subeventDataLen;                   // Length of the subevent data
  uint8_t subeventData[SUBEVENT_MAX_SIZE];   // Subevent data
} subeventProps_t;

typedef struct subeventDataTracker_t
{
    uint8_t state;                    // Tracker state: INIT_SUBEVENT_DATA_REQUEST_PROC or SUBEVENT_DATA_REQUEST_PROC_ACTIVE
    uint8_t batchSize;                // Number of subevents to request in one batch
    uint8_t firstSubeventRequested;   // First Subevent that the controller requested data for
    uint8_t lastSubeventRequested;    // Last Subevent that the controller requested data for
    uint8_t headIndex;                // Index of the first subevent in the tracker
    uint8_t tailIndex;                // Index of the last subevent in the tracker
    uint8_t subeventAtHead;           // Subevent number currently at headIndex
} subeventDataTracker_t;

// Holds all PAwR parameters for a specific Periodic Adv set.
typedef struct llPAwRAdvSet_t
{
  uint8_t  activeSet;                                      // Indicates if this PAwR Adv set is active
  uint32_t rspAA;                                          // Access address for the response
  uint32_t lastAbsStartTime;                               // Last subevent start time (for GenericRx response slot calculation)
  uint8_t  numOfSubevents;                                 // Number of subevents per event
  uint8_t  subeventInterval;                               // Interval between 2 subevents
  uint8_t  responseSlotDelay;                              // Time between subevent and the first response slot
  uint8_t  responseSlotSpacing;                            // Time between 2 response slots
  uint8_t  currentSubevent;                                // Current subevent being handled
  uint8_t  lastSubeventNum;                                // Last subevent number (for GenericRx response handling)
  uint8_t  numOfResponseSlots;                             // Number of response slots for a subevent
  uint8_t  currNumOfResponseSlots;                         // Number of response slots for the current subevent
  uint8_t  subeventToConnect;                              // Subevent to send connReq on. 0xFF means no connection request to handle
  uint8_t  currRspSlot;                                    // The current received response slot
  uint8_t  numOfReceivedResponses;                         // Number of received responses to the current subevent
  uint8_t  useGenericRx;                                   // TRUE if using GenericRx for responses (responseSlotDelay > threshold)
  uint8_t  genericRxPending;                               // Flag indicating GenericRx command is pending - only for useGenericRx == TRUE
  List_List rxBuffers;                                     // RX buffers for receiving response packets
  RCL_CmdBle5GenericRx rfRxCmd;                            // RX command for receiving responses
  RCL_CtxGenericRx     rfRxparam;                          // RX context for receiving responses
  subeventDataTracker_t subeventDataTracker;               // Tracker for the subevent data requests
  subeventProps_t      subeventList[SUBEVENT_LIST_SIZE];   // Array of subevents
  responseReport_t     responseList[RESPONSE_LIST_SIZE];   // List of received responses to send to the host
} llPAwRAdvSet_t;

typedef struct
{
    // Read-only input fields (copied from llPeriodicAdvSet_t)
    uint8_t  advHandle;         // Identify the periodic advertising train.
    uint16_t eventInterval;     // Periodic advertising interval.
    uint32_t lastAbsStartTime;  // Absolute start time of the last periodic advertising subevent.
    void    *pChanMapCurrent;   // Pointer to channel map.
    uint32_t accessAddr;        // Access address.
} llPAwRAPostProcessIn_t;

typedef struct
{
    // Output pointers to fields to be updated
    uint32_t *pStartTime;            // Start Time of the periodic event (also subevent 0)
    uint16_t *pEventCounter;         // Event counter of the next periodic advertising event.
    uint16_t *pNumMissed;            // Number of missed periodic advertising events.
    uint8_t  *pTxBuffers;            // Pointer to the Tx buffers list.
} llPAwRAPostProcessOut_t;

/*******************************************************************************
* LOCAL VARIABLES
******************************************************************************/

/*******************************************************************************
* GLOBAL VARIABLES
******************************************************************************/

/*******************************************************************************
* Functions
******************************************************************************/

/*******************************************************************************
 * HCI APIs
 */

/*******************************************************************************
 * @fn          LE_SetPeriodicAdvSubeventData
 *
 * @brief       Validate and sets the data for one or more subevents of a PAwR advertising
 *              train. The Host provides subevent data for subevents that have
 *              been requested via HCI_LE_Periodic_Advertising_Subevent_Data_Request
 *              event.
 *
 * input parameters
 *
 * @param       pPAwRParams   - Pointer to the PAwR advertiser set parameters.
 * @param       pRfCmd        - Pointer to the RCL periodic advertiser command.
 * @param       props         - Pointer to periodic advertising properties (for TxPower flag).
 * @param       numSubevents  - Number of subevents data entries in pRawSubEvents.
 * @param       pRawSubEvents - Pointer to raw subevent data array from HCI.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      uint8_t - LL_STATUS_SUCCESS if successful,
 *                        LL_STATUS_ERROR_BAD_PARAMETER if invalid parameters.
 */
uint8_t LE_SetPeriodicAdvSubeventData( uint8_t* pPAwRParams,
                                       RCL_CmdBle5PeriodicAdvertiser* pRfCmd,
                                       uint8_t* props,
                                       uint8_t  numSubevents,
                                       uint8_t* pRawSubEvents );

/*******************************************************************************
 * @fn          LL_PAwRA_CreateConn
 *
 * @brief       Creates a connection using a subevent of a PAwR advertiser
 *              to a synchronized scanner.
 *
 * input parameters
 *
 * @param       subevent          - The subevent number to send the connection request on.
 * @param       advHandle         - Advertising handle for the PAwR advertiser set.
 * @param       periodicAdvPhy    - PHY used for periodic advertising (AE format: 1=1M, 2=2M, 3=Coded).
 * @param       pPAwRParams       - Pointer to PAwR advertiser parameters.
 * @param       pCreateConnParams - Pointer to connection creation parameters.
 * @param       pPerAdvCmd        - Pointer to RCL periodic advertiser command.
 * @param       pPerAdvParam      - Pointer to RCL periodic advertiser context.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      uint8_t - LL_STATUS_SUCCESS if successful,
 *                        LL_STATUS_ERROR_BAD_PARAMETER if invalid parameters,
 *                        LL_STATUS_ERROR_COMMAND_DISALLOWED if connection in progress,
 *                        LL_STATUS_ERROR_CONNECTION_ALREADY_EXISTS if peer connected,
 *                        LL_STATUS_ERROR_DUE_TO_LIMITED_RESOURCES if no resources.
 */
uint8_t LL_PAwRA_CreateConn( uint8_t                        subevent,
                             uint8_t                        advHandle,
                             uint8_t                        periodicAdvPhy,
                             uint8_t*                       pPAwRParams,
                             aeCreateConnCmd_t*             pCreateConnParams,
                             RCL_CmdBle5PeriodicAdvertiser* pPerAdvCmd,
                             RCL_CtxPeriodicAdvertiser*     pPerAdvParam );

/*******************************************************************************
 * Inter module APIs
 */

/*******************************************************************************
 * @fn          LL_PAwRA_SetFeatureBit
 *
 * @brief       Set Periodic Advertising with Responses Advertiser bit in device
 *              Feature set
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None.
 */
void LL_PAwRA_SetFeatureBit(void);

/*******************************************************************************
 * @fn          LL_PAwRA_IsEnable
 *
 * @brief       Checks if PAwR Advertiser feature is enabled (compile-time).
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      bool - TRUE if PAwR Advertiser feature is enabled, FALSE otherwise.
 */
bool LL_PAwRA_IsEnable(void);

/*******************************************************************************
 * @fn          LL_PAwRA_IsActive
 *
 * @brief       Checks if any PAwR Advertiser set is currently active.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      bool - TRUE if any PAwR Advertiser is active, FALSE otherwise.
 */
bool LL_PAwRA_IsActive(void);

/*******************************************************************************
 * @fn          LL_PAwRA_IsCreateConnInProgress
 *
 * @brief       Returns whether a PAwR create connection is in progress.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      uint8_t - TRUE if create connection is in progress,
 *                        FALSE otherwise.
 */
uint8_t LL_PAwRA_IsCreateConnInProgress( void );

/*******************************************************************************
 * @fn          LL_PAwRA_GetConnId
 *
 * @brief       Returns the connection ID for the PAwR connection in progress.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      uint16_t - Connection ID, or LL_CONNHANDLE_INVALID if no
 *                         PAwR create connection is in progress.
 */
uint16_t LL_PAwRA_GetConnId( void );

/*******************************************************************************
 * @fn          LL_PAwRA_GetCreateConnParams
 *
 * @brief       Returns the pointer to the PAwR create connection parameters.
 *              This is used by the privacy module to get the peer address
 *              during PAwR connection establishment.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      aeCreateConnCmd_t* - Pointer to create connection parameters,
 *                                   or NULL if no PAwR create connection is in progress.
 */
aeCreateConnCmd_t* LL_PAwRA_GetCreateConnParams( void );

/*******************************************************************************
 * @fn          LL_PAwRA_ClearCreateConn
 *
 * @brief       Clears the PAwR create connection in progress flag.
 *              This function is called when a PAwR connection has been created
 *              to reset the create connection state.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_PAwRA_ClearCreateConn( void );

/*******************************************************************************
 * @fn          LL_PAwRA_GetCreateConnAdvHandle
 *
 * @brief       Returns the advertising handle for the PAwR connection in progress.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      uint8_t - Advertising handle, or LL_ENHANCED_CONN_NO_ADV_HANDLE
 *                        if no PAwR create connection is in progress.
 */
uint8_t LL_PAwRA_GetCreateConnAdvHandle( void );

/*******************************************************************************
 * @fn          LL_PAwRA_CleanupCreateConn
 *
 * @brief       Cleans up PAwR create connection state. Releases allocated
 *              resources and clears global variables.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to PAwR parameters.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_PAwRA_CleanupCreateConn( uint8_t* pPAwRParams );

/*******************************************************************************
 * @fn          LL_PAwRA_FillTimingInfo
 *
 * @brief       Fills the timing info buffer with PAwR parameters for AE ACAD or the PAST.
 *              for AE with ACAD, pAcadlen will be filled too.
 *
 * input parameters
 *
 * @param       pPAwRParams       - Pointer to PAwR parameters.
 *
 * output parameters
 *
 * @param       pTimingInfoBuffer - Filled with PAwR timing information.
 * @param       pAcadLen          - Updated with ACAD length - only for ACAD case.
 *
 * @return      uint8_t - LL_STATUS_SUCCESS if successful.
 */
uint8_t LL_PAwRA_FillTimingInfo(uint8_t* pPAwRParams, uint8_t* pTimingInfoBuffer, uint8_t* pAcadLen);

/*******************************************************************************
 * @fn          LL_PAwRA_ValidateParams
 *
 * @brief       Validate the PAwR parameters that were set using the
 *              LE Set Periodic Advertising Parameters Command V2.
 *
 * input parameters
 *
 * @param       minPeriodicInterval - Minimum periodic advertising interval
 * @param       pPAwRAParams - Include all PAwR Advertiser params:
 *              Num of subevents, subevent Interval, response Slot delay,
 *              response slot spacing and number of response slots.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t - Status of the operation.
 */
uint8_t LL_PAwRA_ValidateParams(uint16_t minPeriodicInterval, uint8_t* pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRA_SetParams
 *
 * @brief       Sets the PAwR Advertiser parameters inside the Periodic that has been received
 *              via the LE Set Periodic Advertising Parameters command V2.
 *
 * input parameters
 *
 * @param       pRawPAwRParams - Raw PAwR parameters received from HCI command.
 *
 * output parameters
 *
 * @param       pPAwRAParams - Include all PAwR Advertiser params:
 *              Num of subevents, subevent Interval, response Slot delay,
 *              response slot spacing and number of response slots.
 *
 * @return      llStatus_t - Status of the operation.
 */
uint8_t LL_PAwRA_SetParams(uint8_t** pPAwRParams, uint8_t* pRawPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRA_Init
 *
 * @brief       This function initializes the PAwR advertiser module. called once
 *              when the LL is initialized.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      llStatus_t - Status of the operation.
 */
uint8_t LL_PAwRA_Init(void);

/*******************************************************************************
 * @fn          LL_PAwRA_SetupCmd
 *
 * @brief       This function sets up the RCL Rx command and context for PAwR
 *              advertiser.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to PAwR parameters.
 * output parameters
 *
 * @param       pRxCmd      - Pointer to RCL Rx command.
 * @param       pRxCtx      - Pointer to RCL Rx context.
 *
 * @return      llStatus_t - Status of the operation.
 */
uint8_t LL_PAwRA_SetupCmd(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pRxCmd,
                          RCL_CtxPeriodicAdvertiser* pRxCtx);

/*******************************************************************************
 * @fn          LL_PAwRA_IsPAwR
 *
 * @brief       Checks if the given parameters correspond to a PAwR set.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to PAwR parameters.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      bool - TRUE if PAwR, FALSE otherwise.
 */
bool LL_PAwRA_IsPAwR(uint8_t* pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRA_PostProcess
 *
 * @brief       Post-processes the PAwR advertiser after an event.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to PAwR parameters.
 * @param       pInParams - Pointer to input parameters structure.
 *
 * output parameters
 *
 * @param       pRfCmd - Pointer to RCL Rx command.
 * @param       pOutParams - Pointer to output parameters structure.
 *
 * @return      uint8_t - Status of the operation.
 */
uint8_t LL_PAwRA_PostProcess(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser *pRfCmd,
                             llPAwRAPostProcessIn_t *pInParams, llPAwRAPostProcessOut_t *pOutParams);

/*******************************************************************************
 * @fn          LL_PAwRA_AdjusmentSet
 *
 * @brief       Adjusts the PAwR advertiser set based on missed events.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to PAwR parameters.
 * @param       pInParams - Pointer to input parameters structure.
 *
 * output parameters
 *
 * @param       rfCmd - Pointer to RCL Rx command.
 * @param       pOutParams - Pointer to output parameters structure.
 *
 * @return      uint8_t - Status of the operation.
 */
uint8_t LL_PAwRA_AdjusmentSet(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser *rfCmd,
                             llPAwRAPostProcessIn_t *pInParams, llPAwRAPostProcessOut_t *pOutParams);

/*******************************************************************************
 * @fn          LL_PAwRA_HandleSubeventDataList
 *
 * @brief       Handles the subevent data list for PAwR advertiser.
 *              Uses the internal tracker state to determine if initialization
 *              is needed or if the procedure is already active.
 *
 * input parameters
 *
 * @param       handle - Advertising handle.
 * @param       pPAwRParams - Pointer to PAwR parameters.
 * @param       rfCmd - Pointer to RCL Rx command.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      uint8_t - Status of the operation.
 */
uint8_t LL_PAwRA_HandleSubeventDataList(uint8_t handle, uint8_t* pPAwRParams,
                                        RCL_CmdBle5PeriodicAdvertiser *rfCmd);

/*******************************************************************************
 * @fn          LL_PAwRA_ClearSet
 *
 * @brief       Clears a PAwR Advertiser set.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       pSet - Pointer to the PAwR Advertiser set to clear.
 *
 * @return      uint8_t - Status of the operation.
*/
uint8_t LL_PAwRA_ClearSet(uint8_t* pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRA_UpdateResponseParams
 *
 * @brief       Updates the RCL command response parameters and the PAwR set params
 *              based on the current subevent's rspSlotStart and rspSlotCount.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to PAwR parameters.
 *
 * output parameters
 *
 * @param       pAdvCmd - Pointer to RCL periodic advertiser command.
 *
 * @return      uint8_t - Status of the operation.
 */
uint8_t LL_PAwRA_UpdateResponseParams(uint8_t* pPAwRParams,
                                      RCL_CmdBle5PeriodicAdvertiser* pAdvCmd);

/*******************************************************************************
 * @fn          LL_PAwRA_SetSubeventDataToPacket
 *
 * @brief       Sets the subevent data to the advertising packet.
 *
 * input parameters
 *
 * @param       comPkt - Pointer to the common extended packet format.
 * @param       pPAwRParams - Pointer to PAwR parameters.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      uint8_t - Status of the operation.
 */
uint8_t LL_PAwRA_SetSubeventDataToPacket(comExtPktFormat_t* comPkt, uint8_t* pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRA_RspRxFifo
 *
 * @brief       Processes received AUX_SYNC_SUBEVENT_RSP packets from the
 *              RX buffer. Extracts response data, stores in responseList,
 *              and sends to host when we have all expected responses or
 *              3 responses (whichever comes first).
 *
 * input parameters
 *
 * @param       advHandle - Advertising handle for the periodic advertising set.
 * @param       pPAwRParams - Pointer to PAwR parameters.
 * @param       pAdvCmd - Pointer to RCL periodic advertiser command (for timing).
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      uint8_t - Number of responses processed.
 */
uint8_t LL_PAwRA_RspRxFifo(uint8_t advHandle, uint8_t* pPAwRParams,
                          RCL_CmdBle5PeriodicAdvertiser* pAdvCmd);

/*******************************************************************************
 * @fn          LL_PAwRA_GenericRxPostProcess
 *
 * @brief       Post-processes GenericRx completion. This function is called
 *              when GenericRx mode is in use (responseSlotDelay > threshold).
 *              It processes responses, clears the RX buffer, resets genericRxPending,
 *              and prepares for the next subevent.
 *
 * input parameters
 *
 * @param       advHandle - Advertising handle for the periodic advertising set.
 * @param       pPAwRParams - Pointer to PAwR parameters.
 * @param       pAdvCmd - Pointer to RCL periodic advertiser command.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      uint8_t - Status of the operation.
 */
uint8_t LL_PAwRA_GenericRxPostProcess(uint8_t advHandle, uint8_t* pPAwRParams,
                                      RCL_CmdBle5PeriodicAdvertiser* pAdvCmd);

/*******************************************************************************
 * @fn          LL_PAwRA_GetPendingRxCmd
 *
 * @brief       Returns the pending GenericRx command if genericRxPending is set.
 *              Used by the scheduler to determine which command to schedule.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to PAwR parameters.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      void* - Pointer to the GenericRx command if pending, NULL otherwise.
 */
void* LL_PAwRA_GetPendingRxCmd(uint8_t* pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRA_IsCurrentSubeventZero
 *
 * @brief       Checks if the current subevent is subevent 0.
 *              Used by llPadvASetupHeader to determine if ACAD should be
 *              included in the extended header for PAwR.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to PAwR parameters.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      bool - TRUE if current subevent is 0, FALSE otherwise.
 */
bool LL_PAwRA_IsCurrentSubeventZero(uint8_t* pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRA_IsGenericRxPending
 *
 * @brief       Checks if a GenericRx command is pending for the given PAwR set.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to PAwR parameters.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      uint8_t - TRUE if GenericRx is pending, FALSE otherwise.
 */
uint8_t LL_PAwRA_IsGenericRxPending(uint8_t* pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRA_IsCurrentSubeventToConnect
 *
 * @brief       Checks if the current subevent matches the subevent to connect.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to PAwR parameters.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      TRUE if pPAwRParams is valid and the current subevent matches
 *              the subevent to connect, FALSE otherwise.
 */
bool LL_PAwRA_IsCurrentSubeventToConnect(uint8_t* pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRA_HandleConnection
 *
 * @brief       This function is used to process the AUX_CONNECT_RSP from the
 *              peer (or handle the case when no response is received) and
 *              determine whether to establish the connection.
 *
 * input parameters
 *
 * @param       advHandle   - Advertising handle.
 * @param       pPAwRParams - Pointer to PAwR parameters.
 * @param       pAdvCmd     - Pointer to RCL periodic advertiser command.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_PAwRA_HandleConnection(uint8_t                        advHandle,
                               uint8_t*                       pPAwRParams,
                               RCL_CmdBle5PeriodicAdvertiser* pAdvCmd);

/*******************************************************************************
 * @fn          LL_PAwRA_InsertConnReqToTxIfNeeded
 *
 * @brief       Checks if AUX_CONNECT_REQ should be added to the TX buffer for
 *              PAwR connection establishment. If conditions are met, sets up
 *              the TX buffer with the connection request packet and configures
 *              PAwR Mode 2.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to PAwR parameters.
 * @param       pPerAdvCmd  - Pointer to RCL periodic advertiser command.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      bool - TRUE if AUX_CONNECT_REQ was added to TX buffer,
 *                     FALSE otherwise.
 */
bool LL_PAwRA_InsertConnReqToTxIfNeeded(uint8_t*                       pPAwRParams,
                                        RCL_CmdBle5PeriodicAdvertiser* pPerAdvCmd);

/*******************************************************************************
 * @fn          LL_PAwRA_PackResponseReports
 *
 * @brief       Pack response reports into a contiguous buffer for HCI event.
 *              Each response is packed as: txPower, rssi, cteType, responseSlot,
 *              dataStatus, dataLen, data[dataLen].
 *
 * @param       pDst         - Destination buffer to pack into.
 * @param       pReports     - Source array of response reports.
 * @param       numResponses - Number of responses to pack.
 *
 * @return      None.
 */
void LL_PAwRA_PackResponseReports(uint8_t *pDst, responseReport_t *pReports, uint8_t numResponses);

#ifdef __cplusplus
}
#endif

#endif /* LL_PAWR_ADVERTISER_H */
