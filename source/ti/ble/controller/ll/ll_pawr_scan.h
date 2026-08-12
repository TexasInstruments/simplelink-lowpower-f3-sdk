/******************************************************************************

 @file  ll_pawr_scan.h

 @brief This file contains the Link Layer (LL) types, constants,
        API's etc. for the Bluetooth Low Energy (BLE) Controller
        Periodic Advertising with responses Scanner.

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

#ifndef LL_PAWR_SCAN_H
#define LL_PAWR_SCAN_H

#ifdef __cplusplus
extern "C"
{
#endif
/*******************************************************************************
* INCLUDES
******************************************************************************/
#include <stdint.h>
#include <ti/drivers/rcl/RCL.h>
#include <ti/drivers/rcl/commands/ble5.h>
#include "ti/ble/controller/ll/ll_ble.h"
#include "ti/ble/controller/ll/ll_al.h"
#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll_ae.h"
#include "ti/ble/controller/ll/ll_privacy.h"

/*******************************************************************************
* MACROS
******************************************************************************/

#define RSP_DATA_PKT_EXTHDR_TOTAL_BUF_SIZE     EXTHDR_FLAGS_SIZE + EXTHDR_FLAG_TXPWR_SIZE
#define LL_ACAD_PAWR_SYNC_INFO_INDEX           2 // Offset of PAwR sync info from ACAD

/*******************************************************************************
* CONSTANTS
******************************************************************************/

/*******************************************************************************
* TYPEDEFS
******************************************************************************/

/*******************************************************************************
* LOCAL VARIABLES
******************************************************************************/

/*******************************************************************************
* GLOBAL VARIABLES
******************************************************************************/

/*******************************************************************************
* Functions
******************************************************************************/

/************************************
 *****        API structs      ******
 ************************************/

typedef struct
{
    // Read-only input fields (copied from llPeriodicScanSet_t)
    uint16_t syncHandle;        // Identify the periodic advertising train.
    uint16_t interval;          // Periodic advertising interval.
    uint8_t  sca;               // Sleep clock accuracy of the advertiser.
    uint8_t  numMissed;         // Number of periodic advertising events that missed in a row.
    uint16_t skip;              // Number of periodic advertising events to skip before the next event.
    uint32_t lastAbsStartTime;  // Absolute start time of the last periodic advertising subevent.
    uint32_t eventCounter;      // Event counter of the last periodic advertising event received.
    void    *chanMapCurrent;    // pointer to channel map.
    uint32_t accessAddr;        // pointer to access address.
} llPAwRPostProcessIn_t;

typedef struct
{
    // Output pointers to fields to be updated
    uint32_t *absStartTime;          // Absolute start time of the next periodic advertising subevent.
    int32_t  *relGracefulStopTime;   // Relative soft stop time for the next periodic advertising subevent.
    uint16_t *eventCounter;          // Event counter of the next periodic advertising event.
    uint8_t  *channel;               //  Channel to use for the next periodic advertising subevent.
} llPAwRPostProcessOut_t;

/*
 * llPAwRSyncInfo_t
 * ----------------
 * This struct is used by other modules to communicate with the PAwR module.
 * It contains information about the PAwR synchronization, such as the access address,
 * number of subevents, subevent interval, response slot delay, and response slot spacing.
 */
typedef struct
{
  uint32_t                            rspAA;                 // access address for the response
  uint8_t                             numOfSubevents;        // Number of subevents.
  uint8_t                             subeventInterval;      // Interval between subevents.
  uint8_t                             rspSlotDelay;          // Time between the advertising packet in a subevent and the first response slot.
  uint8_t                             rspSlotSpacing;        // Time between response slots.
} llPAwRSyncInfo_t;

/************************************
 *****     Internal structs     *****
 ************************************/

/*** PAwR Response Data Structures ***/

/*
 * llPAwRRspParams_t
 * -----------------
 * This struct defines the parameters for a PAwR response.
 * It identifies the periodic event and subevent the host is responding to,
 * as well as the event, subevent, and slot for the response, and includes
 * a pointer to the response data.
 */
typedef struct
{
  uint16_t                    reqEvent;                // Identify the periodic event the host is responding to.
  uint8_t                     reqSubEvent;             // Identify the subevent the host is responding to.
  uint8_t                     rspSubEvent;             // Identify the subevent the response should be sent in.
  uint8_t                     rspSlot;                 // Identify the Slot for response should be sent in.
  uint8_t                     rspDatalen;              // Length of the response data.
  uint8_t*                    pRspData;                // Pointer to the response data.
} llPAwRRspParams_t;

/*
 * rspNode_t
 * ---------
 * This struct defines a node in a linked list of PAwR responses.
 * Each node contains the response parameters and a pointer to the next node.
 * The list is destroyed when LE_SetPeriodicSyncSubevent is called.
 */
typedef struct rspNode_t
{
  llPAwRRspParams_t     params;                   // PAwR response Parameters
  uint16_t              rspEvent;                 // The response event the packed will be sent on
  struct rspNode_t*     pNext;                    // Pointer to the next element in the list.
} rspNode_t;

/*** PAwR Sync to subevents structs ***/

/*
 * syncParams_t
 * ------------
 * This struct defines the PAwR sync subevents invoked by the LE_SetPeriodicSyncSubevent command.
 * It provides information on which subevents to sync with and properties related to the response advertisement.
 * The structure is destroyed and re-initialized whenever the LE_SetPeriodicSyncSubevent command is called by the host.
 */
typedef struct
{
  uint8_t                             rspAdvProps[2];      // Indicates which fields should be included in the AUX_SYNC_SUBEVENT_RSP PDUs.
  uint8_t                             numOfSubevents;      // Number of subevents to sync with.
  uint8_t*                            pSubeventList;       // pointer to List of subevents to sync with
  uint8_t                             isNewSubeventList;   // Flag that indicates there is a new subevent list
  uint8_t                             currSubevent;        // Current subevent to sync with. This value will be used to determine the next subevent to schedule.
  uint8_t                             currSubeventIndex;   // Current subevent index. if there is a new SubeventList, this value will point next subevent to schedule.
} syncParams_t;

/*** PAwR Parameters for each set ***/

/*
 * llPAwRParamsSet_t
 * -----------------
 * This struct holds all parameters and state for a PAwR sync set, including
 * response access address, subevent configuration, RCL command structures,
 * sync parameters, and a linked list of response data.
 */
typedef struct
{
  llPAwRSyncInfo_t                    pawrSyncInfo;
  aePacket                            rspDataPkt;            // Response data packet ready for the RCL
  comExtPktFormat_t                   comRspDataPkt;         // Common Rsp Data Pkt that will be converted to RCL pkt
  aePacket                            connRspPkt;            // AUX_CONN_RSP PDU that will be sent if AUX_CONN_REQ will be received.
  uint8_t                             rspDataExtHdr[RSP_DATA_PKT_EXTHDR_TOTAL_BUF_SIZE]; // Response data extended header
  RCL_CmdBle5GenericTx                rclTxCmd;              // PAwR RCL Tx Command
  RCL_CtxGenericTx                    rclTxParam;            // PAwR RCL Tx Command Parameters
  syncParams_t                        syncParams;            // Struct that holds subevent list and the sync params.
  uint32_t                            lastStartTime;         // Start Time of the last subevent
  uint16_t                            lastEvent;             // Number of last Event that was proccessed
  rspNode_t                           *pRspDataHead;         // Linked list of response data info
} llPAwRParamsSet_t;

/*******************************************************************************
 * @fn          LL_PAwRS_SetFeatureBit
 *
 * @brief       Set Periodic Advertising with Responses Scanner bit in device
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
void LL_PAwRS_SetFeatureBit(void);

/*******************************************************************************
 * @fn          LL_PAwRS_IsEnable
 *
 * @brief       Returns TRUE if PAwR feature is enabled.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      TRUE.
 */
bool LL_PAwRS_IsEnable(void);

/*******************************************************************************
 * @fn          LL_PAwRS_IsPAwR
 *
 * @brief       This function checks if the given Periodic Set use PAwR.
 *              This is done by checking the llPAwRParamsSet_t struct pointer.
 *
 * input parameters
 *
 * @param       pPAwRParams  - Pointer to the llPAwRParamsSet_t struct.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      TRUE if paramsSet is not NULL, FALSE otherwise.
 */
bool LL_PAwRS_IsPAwR(uint8_t *pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRS_InitSet
 *
 * @brief       Initializes PAwR for a periodic scan set.
 *
 * input parameters
 *
 * @param       pPAwRSyncInfoData - Pointer to PAwR sync info data.
 * @param       crcInit           - Pointer to the CRC initialization value (24 bits).
 * @param       phy               - The phy the response data will be sent on
 *
 * output parameters
 *
 * @param       pPAwRParams       - Pointer to a pointer that will hold the
 *                                  allocated PAwR params set.
 *
 * @return      Status            - Success to allocate data structure and parse
 *                                  PAwR sync info data.
 *                                - Failure if memory allocation fails or PAwR
 *                                  sync info parsing fails.
 *
 */
uint8_t LL_PAwRS_InitSet(uint8_t **pPAwRParams, uint8_t* pPAwRSyncInfoData, uint8_t *crcInit,
                         RCL_CtxPeriodicScanner* rfParam, uint8_t advAddrType,
                         uint8_t* advAddr, uint8_t phy);

/*******************************************************************************
 * @fn          LL_PAwRS_Activate
 *
 * @brief       When the periodic advertising train supports PAwR, and the
 *              periodic advertising train has been synced, this function
 *              Activate the PAwR module, including syncing to the subevent
 *              default list (Sync to subevent 0).
 *
 * @param       pPAwRParams - Pointer to PAwR params set.
 */
void LL_PAwRS_Activate(uint8_t* pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRS_GetSubeventNum
 *
 * @brief       This function gets the subevent number when the Periodic Train supports PAWR.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to the llPAwRParamsSet_t struct.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      The subevent number, or 0xFF if PAWR is not supported.
 */
uint8 LL_PAwRS_GetSubeventNum(uint8_t *pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRS_IsACADPAwR
 *
 * @brief       Checks if the ACAD data is for PAwR.
 *
 * input parameters
 *
 * @param       pACAD - Pointer to ACAD data.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      TRUE if ACAD is PAwR, FALSE otherwise.
 */
bool LL_PAwRS_IsACADPAwR(uint8_t* pACAD);

/*******************************************************************************
 * @fn          LL_PAwRPostProcessing
 *
 * @brief       Performs post-processing for periodic advertising synchronization.
 *              This function use the input parameters that received from the padv module,
 *              and the PAwR params set to calculate the next sync parameters.
 *
 * input parameters
 *
 * @param       pPadvParamsIn  - Pointer to input periodic parameters that will be used for post-processing.
 *
 * output parameters
 *
 * @param       pPadvParamsOut - Pointer to output periodic parameters fthat will be updated after post-processing.
 * @param       pPAwRParams    - Pointer to PAwR params set that will be used for post-processing.
 *
 * @return      None.
 */
void LL_PAwRS_PostProcess(const llPAwRPostProcessIn_t *pPadvParamsIn, llPAwRPostProcessOut_t *pPadvParamsOut, uint8_t *pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRS_BuildConnRspPkt
 *
 * @brief       This function builds the AUX_CONN_RSP packet for the PAwR module.
 *
 * input parameters
 *
 * @param       rfParam      - Pointer to the RCL context for the periodic scanner.
 * @param       pPAwRParams  - Pointer to the PAwR parameters.
 * @param       advAddrType  - Advertising address type.
 * @param       advAddr      - Pointer to the advertising address.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status of the operation (0 = success, otherwise error).
 */
uint8_t LL_PAwRS_BuildConnRspPkt(RCL_CtxPeriodicScanner* rfParam, void* pPAwRParams,
                                 uint8_t advAddrType, uint8_t* advAddr);

/*******************************************************************************
 * @fn          LL_PAwR_PostProcessRspData
 *
 * @brief       This function processes the response data command for the PAwR module.
 *              It removes old response data packets and set the next response data tx
 *              command based on the following input parameters.
 *
 * input parameters
 *
 * @param       chanMapCurrent  - Pointer to the current channel map.
 * @param       accessAddr      - Access address for the periodic advertising train.
 * @param       absStartTime    - Absolute start time of the periodic advertising subevent.
 * @param       eventInterval   - Interval of the periodic event.
 * @param       eventCounter    - Event counter of the periodic advertising event.
 *
 * output parameters
 *
 * @param       pPAwRparams     - Pointer to PAwR parameters set.
 *
 * @return      None.
 */
void LL_PAwR_PostProcessRspData(void* chanMapCurrent, uint32_t accessAddr, uint32_t absStartTime,
                                uint16_t eventInterval, uint16_t eventCounter, uint8_t* pPAwRparams);
/*******************************************************************************
 * @fn          LL_PAwRS_GetSyncParams
 *
 * @brief       This function retrieves the sync parameters from the given
 *              llPAwRParamsSet_t struct.
 * input parameters
 *
 * @param       PAwRParams - Pointer to the llPAwRParamsSet_t struct.
 *
 * output parameters
 *
 * @param       syncParams - Pointer to the sync parameters to be retrieved.
 *
 * @return      Status.
 */
uint8_t LL_PAwRS_GetSyncParams(uint8_t* pPAwRParams, llPAwRSyncInfo_t* syncParams);

/*******************************************************************************
 * @fn          LE_SetPeriodicSyncSubevent
 *
 * @brief       This function sets the subevents for periodic advertising synchronization.
 *
 * input parameters
 *
 * @param       syncHandle   - Handle of the periodic advertising synchronization.
 * @param       perAdvProps  - Properties for periodic advertising.
 * @param       numSubevents - Number of subevents to sync with.
 * @param       subEvents    - Pointer to the list of subevents to sync with.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status of the operation (0 = success, otherwise error).
 */
uint8_t LE_SetPeriodicSyncSubevent(uint8_t* pPAwRParams,
                                   uint16_t perAdvProps,
                                   uint8_t  numSubevents,
                                   uint8_t* subEvents);

/*******************************************************************************
 * @fn          LE_SetPeriodicAdvResponseData
 *
 * @brief       This function sets the response data for periodic advertising.
 *
 * input parameters
 *
 * @param       eventCounter    - Event counter of the periodic advertising event.
 * @param       chanMap         - Pointer to the channel map.
 * @param       accessAddress   - Access address for the periodic advertising.
 * @param       interval        - Interval of the periodic advertising event.
 * @param       pPAwRParams     - Pointer to the llPAwRParamsSet_t struct.
 * @param       pRawRspParams   - Pointer to the raw response parameters
 *                                 (including response data) from the host.
 * @param       rfCmd           - Pointer to the RCL command structure for periodic scanning.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status of the operation (0 = success, otherwise error).
 */
uint8_t LE_SetPeriodicAdvResponseData(uint16_t eventCounter, void* chanMap, uint32_t accessAddress,
                                      uint16_t interval, uint8_t* pPAwRParams, uint8_t* pRawRspParams,
                                      RCL_CmdBle5PeriodicScanner* rfCmd);

/*******************************************************************************
 * @fn          LL_PAwRS_Terminate
 *
 * @brief       This function is responsible for freeing all PAwR structures.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to the llPAwRParamsSet_t struct.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status of the termination (llStatus_t).
 */
llStatus_t LL_PAwRS_Terminate(uint8_t **pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRS_AdjustNextStartTime
 *
 * @brief       Adjusts the next start time for the periodic scan based on the
 *              current and next subevent, and increment the event Counter if
 *              necessary.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to the llPAwRParamsSet_t struct.
 * @param       eventInterval - Interval of the periodic event.
 * @param       advSca - Sleep clock accuracy of the advertiser.
 *
 * output parameters
 *
 * @param       absStartTime - Pointer to the absolute start time of the next periodic event
 * @param       eventCounter - Pointer to the event counter of the current periodic event.
 *
 * @return      None.
 */
void LL_PAwRS_AdjustNextStartTime(uint32_t* absStartTime, uint8_t* eventCounter,
                                  uint8_t* pPAwRParams, uint16_t eventInterval,
                                  uint8_t advSca);

/*******************************************************************************
 * @fn          LL_PAWRS_GetRspPktTxCmd
 *
 * @brief       This function retrieves the RCL Tx command address for the
 *              response packet.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to the llPAwRParamsSet_t struct.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Pointer to the RCL Tx command for the response packet.
 */
uint32_t LL_PAWRS_GetRspPktTxCmd(uint8_t* pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRS_UpdateRspDataCmdIfNeeded
 *
 * @brief       This function checks if repsonse data is in the past for any reason,
 *              and if it is, it will remove it and schedule a new one if there is one.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to the llPAwRParamsSet_t struct containing PAwR parameters.
 * @param       eventCounter - Current event counter.
 * @param       chanMapCurrent - Current channel map.
 * @param       accessAddress - Access address for the periodic advertising.
 * @param       absStartTime - Absolute start time of the periodic advertising event.
 * @param       eventInterval - Interval between periodic advertising events in 1.25 ms units.
 *
 * output parameters
 *
 * @return      true if there is response data to send, false otherwise.
 */
void LL_PAwRS_UpdateRspDataCmdIfNeeded(uint8_t* pPAwRParams, uint16_t eventCounter, void* chanMapCurrent,
                                       uint32_t accessAddress, uint32_t absStartTime, uint16_t eventInterval);

/*******************************************************************************
 * @fn          LL_PAwRS_ChooseRspPktOrSubeventSync
 *
 * @brief       This function chooses whether to send a response packet or synchronize
 *              to the next subevent, based on the earlier start Time.
 *
 * input parameters
 *
 * @param       selectedPadvSStartTime - Selected periodic subevent start time.
 * @param       pPAwRParams - Pointer to the llPAwRParamsSet_t struct containing PAwR parameters.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      True - choose Response packe
 *              False - choose to sync to a subevent
 *
 */
bool LL_PAwRS_ChooseRspPktOrSubeventSync(uint32_t selectedPadvSStartTime, uint8_t* pPAwRParams);

/*******************************************************************************
 * @fn          LL_PAwRS_TaskConn
 *
 * @brief       This function is called after receiving connection request from
 *              a periodic advertiser that supports PAwR on a subevent.
 *              then it will process the request and establish a connection
 *              as peripheral.
 *
 * input parameters
 *
 * @param       pPAwRParams - Pointer to the llPAwRParamsSet_t struct containing PAwR parameters.
 * @param       rfParam - Pointer to the RCL context for periodic scanner.
 * @param       syncHandle - handle of the periodic train
 * @param       absStartTime - Absolute start time of the connection event.
 * @param       advAddrType - Advertising address type of the PAwR advertiser.
 * @param       advAddr - Pointer to the advertising address of the PAwR advertiser.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_PAwRS_TaskConn(uint8_t* pPAwRParams, RCL_CtxPeriodicScanner* rfParam,
                       uint16_t synchandle, uint32_t absStartTime, uint8_t advAddrType,
                       uint8_t* advAddr);

/*******************************************************************************
 * @fn          LL_PAwRS_IsConnRspPkt
 *
 * @brief       Checks if the next packet in the RCL Rx buffer is a connection request PDU.
 *
 * @param       rfParam - Pointer to the periodic scanner RF context.
 *
 * @return      TRUE if the packet is a connection request PDU, FALSE otherwise.
 */
bool LL_PAwRS_IsConnRspPkt(RCL_CtxPeriodicScanner* rfParam);

#ifdef __cplusplus
}
#endif

#endif /* LL_PAWR_SCAN_H */
