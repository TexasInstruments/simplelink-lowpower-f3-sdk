/******************************************************************************

 @file  ll_padv_scan.h

 @brief This file contains the Link Layer (LL) types, constants,
        API's etc. for the Bluetooth Low Energy (BLE) Controller
        Periodic Advertising Scanner.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2009-2026, Texas Instruments Incorporated

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

#ifndef LL_PADV_SCAN_H
#define LL_PADV_SCAN_H

#ifdef __cplusplus
extern "C"
{
#endif
/*******************************************************************************
* INCLUDES
******************************************************************************/
#include "ti/ble/controller/ll/ll_ae.h"

/*******************************************************************************
* MACROS
******************************************************************************/

/*******************************************************************************
* CONSTANTS
******************************************************************************/

// Maximum margin time for periodic scanning in RAT ticks (2.576ms)
#define PERIODIC_SCAN_MAX_MARGIN_TIME_RAT_TICKS             (PERIODIC_SCAN_MARGIN_TIME_RAT_TICKS + LL_JITTER_CORRECTION + LL_RX_RAMP_OVERHEAD + RAT_TICKS_FOR_PERIODIC_SCAN_WIN_SIZE + LL_RX_SYNCH_OVERHEAD)

// Maximum number of events allowed for periodic syncing
#define PERIODIC_SYNCING_LIMIT_NUM_EVENTS                   6

// Maximum number of periodic scan handles
#define PERIODIC_SCAN_MAX_HANDLES                           0x0EFF

// Maximum number of events used for drift learning
#define PERIODIC_SCAN_DRIFT_LEARNING_MAX_NUM                10

// Maximum number of handles in the terminate list
#define PERIODIC_SCAN_TERMINATE_LIST_MAX_HANDLES            4

// Index reserved for create sync cancel in the terminate list
#define PERIODIC_SCAN_TERMINATE_LIST_CREATE_SYNC_INDEX      0

// Invalid handle value for the terminate list
#define PERIODIC_SCAN_TERMINATE_LIST_INVALID_HANDLE         0xFF

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

////////////////////////////////////////////////////////
//// Periodic Advertising Scanner structures ///////////
////////////////////////////////////////////////////////

typedef struct
{
  uint8                             addr[LL_DEVICE_ADDR_LEN]; // Advertiser address
  uint8                             addrType;                 // Advertiser address type
  uint8                             sid;                      // Advertiser SID retrieved from the ADI field
  uint8                             options;                  // Bitwise options - use list or/and disable reports
  uint8                             cteType;                  // Bitwise packet filter according to CTE type
  uint16                            skip;                     // Max number of events that can be skipped after a successful receive
  uint16                            timeout;                  // Synchronization timeout = N*10 ms
} llPeriodicScanSyncCmd_t;

typedef struct llPeriodicScanSet_t
{
  struct llPeriodicScanSet_t        *next;                 // Pointer to next handle in list, if any
  RCL_CmdBle5PeriodicScanner        rfCmd;                 // Periodic Scanner RF Command
  RCL_CtxPeriodicScanner            rfParam;               // Periodic Scanner RF Command Parameters
  llPeriodicScanSyncCmd_t           syncCmd;               // current periodic syncing params command
  llPeriodicAdvSyncInfo_t           syncInfo;              // Sync information in AUX_ADV_IND
  llPeriodicAdvChanMap_t            chanMap;               // channel map received from advertiser
  uint32                            startTime;             // scanning start time in ticks
  uint32                            totalOtaTime;          // total OTA time in RAT ticks per length and PHY
  uint16                            handle;                // identify a periodic advertising train (Range: 0x00 to 0xEF)
  uint16                            interval;              // Periodic advertising interval. (Range: 7.5 ms to 81918 ms)
  uint16                            eventCounter;          // incremental counter for each periodic event
  uint16                            numMissed;             // number of consecutive missed periodic events
  uint16                            chanMapUpdateEvent;    // channel map update instant
  int16                             driftFactor;           // drift time in RAT ticks per event (negative or positive value depends on the drift direction)
  uint8                             state;                 // periodic advertising scanner status
  uint8                             phy;                   // advertiser phy
  uint8                             reportEnable;          // send scan report to host flag
  uint8                             terminate;             // flag hold the reason for the termination if there is
  uint8                             ownAddrType;           // periodic scanner address type (public or random)
  uint8                             rxCount;               // number of received packets in current periodic event
  uint8                             cteRssiAntenna;        // first antenna which rssi was measured on while CTE sampling.
  uint8                             intPriority;           // internal priority: priority scale to use in periodic scan selection procedure
  uint8                             driftLearnCounter;     // counts number of valid events used for drift calculation (range 0 to PERIODIC_SCAN_DRIFT_LEARNING_MAX_NUM)
  uint8                             priority;              // priority as a secondary task.
  uint8_t*                          pPAwRParams;           // pPAwR params (if supported).

} llPeriodicScanSet_t;

typedef struct llPeriodicAcceptListItem_t
{
  uint8                             addr[B_ADDR_LEN];     // Peer address
  uint8                             addrType;             // Peer address type
  uint8                             sid;                  // Peer SID
  struct llPeriodicAcceptListItem_t  *next;                // Pointer to next item or NULL
} llPeriodicAcceptListItem_t;

typedef struct
{
  uint8                             numItems;             // number of items in the accept list
  llPeriodicAcceptListItem_t         *itemList;            // accept list head
} llPeriodicAcceptList_t;

typedef struct
{
  llPeriodicScanSet_t               *scanList;             // periodic advertising scanners list
  llPeriodicScanSet_t               *currentScan;          // current active periodic advertising scanner
  llPeriodicScanSet_t               *createSync;           // pointer to periodic scan set which in create sync process active periodic advertiser
  taskInfo_t                        *llTask;               // Pointer to associated BLE task block (define new task with ID LL_TASK_ID_PERIODIC_SCANNER 0x10)
  List_List                         rxBuffers;
  RCL_StatsAdvScanInit              rfOutput;              // shared Periodic Scanner RF Command Output
  llPeriodicAcceptList_t            acceptList;             // Periodic Scanner accept list
  uint8                             scanNumActive;         // current number of active periodic scanners
  uint8                             terminateList[PERIODIC_SCAN_TERMINATE_LIST_MAX_HANDLES]; // terminate handle array (first index reserved for create sync cancel)
} llPeriodicScan_t;


typedef struct __attribute__((packed))
{
  uint16_t syncHandle;   // Handle identifying the periodic advertising train
  int8_t   txPower;      // Transmit power level of the received packet
  int8_t   rssi;         // Received Signal Strength Indicator (RSSI) value
  uint8_t  cteType;      // Constant Tone Extension (CTE) type
  uint16_t eventCounter; // Event counter for the periodic advertising event
  uint8_t  subevent;     // Subevent index within the periodic advertising event
} llPeriodicEvtParams_t;

typedef struct __attribute__((packed))
{
  uint16_t syncHandle;                      // Handle identifying the periodic advertising train
  uint8_t  AdvertisingSID;                  // Advertising SID subfield in the ADI field used to identify the periodic advertising
  uint8_t  advAddrType;                     // Advertiser address type
  uint8_t  advAddress[LL_DEVICE_ADDR_LEN];  // Advertiser address
  uint8_t  padvPhy;                          // PHY used for periodic advertising
  uint16_t padvInterval;                    // Periodic advertising interval
  uint8_t  advClockAccuracy;                // Clock accuracy of the periodic advertising train
  uint8_t  numOfSubevents;                  // Number of subevents in the periodic advertising event
  uint8_t  subeventInterval;                // Interval between subevents
  uint8_t  rspSlotDelay;                    // Delay for response slots
  uint8_t  rspSlotSpacing;                  // Spacing between response slots
} llPadvSEstEventParams_t;

// @brief Periodic advertising sync transfer create sync parameters.
//        Used to synchronize with a periodic advertising train from an
//        periodic advertiser or periodic scanner and begin receiving
//        AUX_SYNC_IND.
typedef struct
{
  llPeriodicAdvSyncInfo_t           syncInfo;              // Sync information in AUX_ADV_IND
  llPeriodicAdvChanMap_t            chanMap;               // Channel map received from advertiser
  uint32_t                          absStartTime;          // Start time of the command on SYSTIM (0.25 us steps)
  uint32_t                          relGracefulStopTime;   // Time to initiate graceful stop of command
  uint16_t                          interval;              // Periodic advertising interval. (Range: 7.5 ms to 81918 ms)
  uint8_t                           *pPAwRParams;          // Pointer of PAwR params (if supported).
  uint8_t                           phy;                   // Advertiser phy
  uint8_t                           ownAddrType;           // Periodic scanner address type (public or random)
} llPASTCreateSync_t;

/*
 * @brief LL Periodic Scan Callback
 *
 * @note
 * This callback is triggered when a periodic advertising sync is successfully established
 * or when a timeout occurs during the sync process. It is only used if registered
 * by the periodic sync transfer module upon receiving an LL_PERIODIC_SYNC_IND.
 *
 * @param connHandle          - Connection handle associated with the periodic advertising sync.
 * @param pAdvSEstEventParams - Pointer to the periodic advertising sync established event parameters.
 * @param lenAdvSEstEvent     - Length of the periodic advertising sync established event parameters.
 *
 * @return None
 */
typedef void (*pfnPeriodicScanCB_t)(uint8_t *pSyncEstabEvent, uint8_t eventLength, uint8_t status );

/*********************************************************************
 * @fn      LE_PeriodicAdvCreateSync
 *
 * @brief   Used a scanner to synchronize with a periodic advertising train from
 *          an advertiser and begin receiving periodic advertising packets.
 *
 * @design /ref did_286039104
 *
 * @param   options     - Clear Bit 0 - Use the advSID, advAddrType, and advAddress
 *                                      parameters to determine which advertiser to listen to.
 *                        Set Bit 0   - Use the Periodic Advertiser List to determine which
 *                                      advertiser to listen to.
 *                        Clear Bit 1 - Reporting initially enabled.
 *                        Set Bit 1   - Reporting initially disabled.
 * @param   advSID      - Advertising SID subfield in the ADI field used to identify
 *                        the Periodic Advertising (Range: 0x00 to 0x0F)
 * @param   advAddrType - Advertiser address type - 0x00 - public ; 0x01 - random
 * @param   advAddress  - Advertiser address
 * @param   skip        - The maximum number of periodic advertising events that can be
 *                        skipped after a successful receive (Range: 0x0000 to 0x01F3)
 * @param   syncTimeout - Synchronization timeout for the periodic advertising train
 *                           Range: 0x000A to 0x4000 Time = N*10 ms Time Range: 100 ms to 163.84 s
 * @param   syncCteType - Set Bit 0 - Do not sync to packets with an AoA CTE
 *                        Set Bit 1 - Do not sync to packets with an AoD CTE with 1 us slots
 *                        Set Bit 2 - Do not sync to packets with an AoD CTE with 2 us slots
 *                        Set Bit 4 - Do not sync to packets without a CTE
 *
 * @return  HCI_Success
 */
extern llStatus_t LE_PeriodicAdvCreateSync( uint8  options,
                                            uint8  advSID,
                                            uint8  advAddrType,
                                            uint8  *advAddress,
                                            uint16 skip,
                                            uint16 syncTimeout,
                                            uint8  syncCteType );

/*********************************************************************
 * @fn      LE_PeriodicAdvCreateSyncCancel
 *
 * @brief   Used a scanner to cancel the HCI_LE_Periodic_Advertising_Create_Sync
 *          command while it is pending.
 *
 * @design /ref did_286039104
 *
 * @param   None
 *
 * @return  llStatus_t
 */
extern llStatus_t LE_PeriodicAdvCreateSyncCancel( void );

/*********************************************************************
 * @fn      LE_PeriodicAdvTerminateSync
 *
 * @brief   Used a scanner to stop reception of the periodic advertising
 *          train identified by the syncHandle parameter.
 *
 * @design /ref did_286039104
 *
 * @param   syncHandle - Handle identifying the periodic advertising train
 *                       (Range: 0x0000 to 0x0EFF)
 *                       The handle was assigned by the Controller while generating
 *                       the LE Periodic Advertising Sync Established event
 *
 * @return  llStatus_t
 */
extern llStatus_t LE_PeriodicAdvTerminateSync( uint16 syncHandle );

/*********************************************************************
 * @fn      LE_AddDeviceToPeriodicAdvertiserList
 *
 * @brief   Used a scanner to add an entry, consisting of a single device address
 *          and SID, to the Periodic Advertiser list stored in the Controller.
 *
 * @design /ref did_286039104
 *
 * @param   advAddrType - Advertiser address type - 0x00 - Public or Public Identity Address
 *                                                  0x01 - Random or Random (static) Identity Address
 * @param   advAddress  - Advertiser address
 * @param   advSID      - Advertising SID subfield in the ADI field used to identify
 *                        the Periodic Advertising (Range: 0x00 to 0x0F)
 *
 * @return  llStatus_t
 */
extern llStatus_t LE_AddDeviceToPeriodicAdvList( uint8 advAddrType,
                                                 uint8 *advAddress,
                                                 uint8 advSID );

/*********************************************************************
 * @fn      LE_RemoveDeviceFromPeriodicAdvList
 *
 * @brief   Used a scanner to remove one entry from the list of Periodic Advertisers
 *          stored in the Controller.
 *
 * @design /ref did_286039104
 *
 * @param   advAddrType - Advertiser address type -
 *                        0x00 - Public or Public Identity Address
 *                        0x01 - Random or Random (static) Identity Address
 * @param   advAddress  - Advertiser address
 * @param   advSID      - Advertising SID subfield in the ADI field used to identify
 *                        the Periodic Advertising (Range: 0x00 to 0x0F)
 *
 * @return  llStatus_t
 */
extern llStatus_t LE_RemoveDeviceFromPeriodicAdvList( uint8 advAddrType,
                                                      uint8 *advAddress,
                                                      uint8 advSID );

/*********************************************************************
 * @fn      LE_ClearPeriodicAdvList
 *
 * @brief   Used a scanner to remove all entries from the list of Periodic
 *          Advertisers in the Controller.
 *
 * @design /ref did_286039104
 *
 * @return  llStatus_t
 */
extern llStatus_t LE_ClearPeriodicAdvList( void );

/*********************************************************************
 * @fn      LE_ReadPeriodicAdvListSize
 *
 * @brief   Used a scanner to read the total number of Periodic Advertiser
 *          list entries that can be stored in the Controller.
 *
 * @design /ref did_286039104
 *
 * @return  llStatus_t
 *          Periodic Advertiser List Size (Range: 0x01 to 0xFF)
 */
extern llStatus_t LE_ReadPeriodicAdvListSize( uint8 *listSize );

/*********************************************************************
 * @fn      LE_SetPeriodicAdvReceiveEnable
 *
 * @brief   Used a scanner to enable or disable reports for the periodic
 *          advertising train identified by the syncHandle parameter.
 *
 * @design /ref did_286039104
 *
 * @param   syncHandle - Handle identifying the periodic advertising train
 *                       (Range: 0x0000 to 0x0EFF)
 *                       The handle was assigned by the Controller while generating
 *                       the LE Periodic Advertising Sync Established event
 * @param   enable     - 0x00 - Reporting disable
 *                       0x01 - Reporting enable
 *
 * @return  llStatus_t
 */
extern llStatus_t LE_SetPeriodicAdvReceiveEnable( uint16 syncHandle,
                                                  uint8  enable );

/*******************************************************************************
* @fn          LL_PadvA_CheckSyncInfoCriteria
*
* @brief       This function checks if the synchronization information condition is met.
*
* @param       advPDUInfo - Pointer to the advertising PDU information.
*
* @return      checkSyncInfo - Boolean indicating whether the condition is met.
*/
bool LL_PadvA_CheckSyncInfoCriteria(llAdvPDUInfo *advPDUInfo);

/*******************************************************************************
* @fn          LL_PadvS_ProcessPeriodicSyncInfo
*
* @brief       This function process the sync info that was received in
*              current AUX_ADV_IND.
*
* @design      /ref did_286039104
*
* input parameters
*
* @param       pAdvInfo - Pointer to the advertising PDU information
*                         including pointer to the sync info and phy type
*                         of received AUX_ADV_IND.
* @param       advEvent  - A pointer to generated advertising report.
* @param       timeStamp - time stamp of received AUX_ADV_IND.
*
* output parameters
*
* @param       None.
*
* @return      None.
*/
void LL_PadvS_ProcessPeriodicSyncInfo(llExtAdvPDUInfo *pExtAdvInfo, aeExtAdvRptEvt_t *advEvent, uint32_t timeStamp);

/*******************************************************************************
* @fn          LL_PadvS_PostProcess
*
* @brief       This routine is used to post process the periodic Advertising scan
*              command.
*
* @design      /ref did_286039104
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
void LL_PadvS_PostProcess(void);

/*******************************************************************************
* @fn          LL_PadvS_ProcessRxFIFO
*
* @brief       This function used to process received periodic asvertising packet
*
* @design      /ref did_286039104
*
* input parameters
*
* @param       None.
*
* output parameters
*
* @param       None.
*
* @return      None
*/
void LL_PadvS_ProcessRxFIFO(void);

/*******************************************************************************
* @fn          LL_PadvS_FindNextSet
*
* @brief       This routine is used find next Periodic Scan Set.
*
* @design      /ref did_286039104
*
* input parameters
*
* @param       None.
*
* output parameters
*
* @param       None.
*
* @return      Pointer to periodic scan rf command
*/
void *LL_PadvS_FindNextSet(uint16_t scanMaxNumMiss);

/*******************************************************************************
* @fn          LL_PadvS_Terminate
*
* @brief       This function is used to end all periodic scan sets in terminate list
*
* @design  /ref did_286039104
*
* input parameters
*
* @param       void.
*
* output parameters
*
* @param       None.
*
* @return      None.
*/
void LL_PadvS_Terminate(void);

/*******************************************************************************
* @fn          LL_PadvS_GetCurrent
*
* @brief       This function is used to get the current periodic scan according
*              to the requested periodic scan state.
*
* @design      /ref did_286039104
*
* input parameters
*
* @param       state - the state of requested Set.
*
* output parameters
*
* @param       None.
*
* @return      pPeriodicScan - A pointer to syncing periodic scanner.
*/
llPeriodicScanSet_t *LL_PadvS_GetCurrent(uint8 state);

/*******************************************************************************
* @fn          LL_PadvS_GetSetByHandle
*
* @brief       This function is used to get the periodic scan by handle
*
* @design      /ref did_286039104
*
* input parameters
*
* @param       handle - periodic scanner handle.
*
* output parameters
*
* @param       None.
*
* @return      pPeriodicScan - A pointer to periodic scanner.
*/
llPeriodicScanSet_t *LL_PadvS_GetSetByHandle(uint16 handle);

/*******************************************************************************
* @fn          LL_PadvS_ClearSets
*
* @brief       This function is used to clear all periodic scan sets
*
* @design  /ref did_286039104
*
* input parameters
*
* @param       void.
*
* output parameters
*
* @param       None.
*
* @return      None.
*/
void LL_PadvS_ClearSets(void);

/*******************************************************************************
* @fn          LL_PadvS_UpdateCmdParams
*
* @brief       This function is used to set the accept sync info bit in
*              the extended scan filter RF param byte
*
* input parameters
*
* @param       void.
*
* output parameters
*
* @param       None.
*
* @return      None.
*/
void LL_PadvS_UpdateCmdParams(void);

/*******************************************************************************
* @fn          LL_PadvS_ReturnCurrentSetStartTime
*
* @brief       This function returns the closest periodic scan start time
*
* input parameters
*
* @param       None.
*
* output parameters
*
* @param       None.
*
* @return      Closest periodic scan start time. zero value means no active periodic scan
*/
uint32_t LL_PadvS_ReturnCurrentSetStartTime(void);

/*******************************************************************************
* @fn          LL_PadvS_IsEnable
*
* @brief       Indication if the periodic scanning feature is enabled or disabled.
*
* input parameters
*
* @param       None.
*
* output parameters
*
* @param       None.
*
* @return      TRUE if the feature is enabled, otherwise FALSE.
*/
bool LL_PadvS_IsEnable(void);

/*******************************************************************************
* @brief Check if the periodic scan task has a higher priority than
*        the primary connection task.
*
* This function checks if the given task ID corresponds to a periodic scan task.
* If it does,it retrieves the current periodic scan and compares its priority with
* the primary connection task's priority. If the periodic scan's priority is
* higher and within the valid range, the function returns TRUE.
* Otherwise, it returns FALSE.
*
* @param taskID The ID of the task to check.
* @param connPriority The connection priority to consider.
* @return TRUE if the periodic scan task has a higher priority than the primary connection task,
*         FALSE otherwise.
*/
uint8_t LL_PadvS_CheckPriority(uint16_t taskID, uint8_t connPriority);

/*******************************************************************************
* @fn          LL_PadvS_GetRxBuffers
*
* @brief       This function returns a pointer to the periodic scan RX buffers.
*
* input parameters
*
* @param       None.
*
* output parameters
*
* @param       None.
*
* @return      Pointer to the RX buffers list.
*/
List_List* LL_PadvS_GetRxBuffers(void);

/*******************************************************************************
* @fn          LL_PadvS_GetPerodicTypeBySyncHandle
*
* @brief       This function is used to get the type of the periodic
*              advertising set based on the provided sync handle.
*
* input parameters
*
* @param       syncHandle - The sync handle identifying the periodic
*                           advertising set.
*
* output parameters
*
* @param       None.
*
* @return      llPeriodicAdvSetType_e
*              - LL_PERIODIC_ADV_TYPE_WITH_RESPONSES
*              - LL_PERIODIC_ADV_TYPE_WITHOUT_RESPONSES
*              - LL_PERIODIC_ADV_TYPE_INVALID_IDENTIFIER
*/
llPeriodicAdvSetType_e LL_PadvS_GetPerodicTypeBySyncHandle(uint16_t syncHandle);

/*******************************************************************************
 * @fn          LL_PadvS_GetPerodicSyncTransferInfo
 *
 * @brief       Retrieve periodic sync transfer information for a given sync handle.
 *              This function fills the provided structure with details about the
 *              periodic advertising sync, including channel map, interval, event count,
 *              PHY, SID, advertiser address, and synchronization information.
 *
 * input parameters
 *
 * @param       syncHandle                - Sync handle of the periodic advertising set.
 * @param       pPeriodicSyncTransferData - Pointer to the structure where periodic sync
 *                                          transfer information will be stored.
 *
 * output parameters
 *
 * @param       pPeriodicSyncTransferData - Pointer to the structure that will be
 *                                          filled with periodic sync transfer info.
 *
 * @return      LL_STATUS_SUCCESS when the operation succeeds.
 *              Otherwise:
 *              LL_STATUS_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER
 */
uint8_t LL_PadvS_GetPerodicSyncTransferInfo( uint16_t syncHandle,
                                             llPeriodicSyncTransferInfo_t* pPeriodicSyncTransferData);

/*******************************************************************************
 * @fn          LL_PadvS_SetupAndTriggerSync
 *
 * @brief       This function sets up the periodic sync parameters and triggers
 *              the periodic scan.
 *
 * input parameters
 *
 * @param       pPeriodicSyncParams - Pointer to the periodic sync parameters.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 *              LL_STATUS_ERROR_INVALID_PARAMS
 */
uint8_t LL_PadvS_SetupAndTriggerSync( llPASTCreateSync_t *pPeriodicSyncParams );

/*******************************************************************************
 * @fn          LL_PadvS_RegisterCB
 *
 * @brief       This function registers a callback function for periodic scan.

 *
 * input parameters
 *
 * @param       pfnCB - Pointer to the callback function to be registered.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_PadvS_RegisterCB( pfnPeriodicScanCB_t pfnCB );

/*******************************************************************************
 * @fn          LL_PadvS_UnregisterCB
 *
 * @brief       This function is used to unregister the periodic scan callback.
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
void LL_PadvS_UnregisterCB( void );

/*********************************************************************************
 * @fn          LL_PadvS_SetAdvASyncCmd
 *
 * @brief       This function sets the advertising address and address type for
 *              the periodic scan sync command. Returns a status indicating if
 *              the update was successful.
 *
 * input parameters
 *
 * @param       advAddress  - Pointer to the periodic advertising address.
 * @param       advAddrType - Periodic advertising address type.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      USUCCESS if the address and type were set successfully,
 *              otherwise UFAILURE.
 */
uint8_t LL_PadvS_SetAdvASyncCmd( uint8_t *advAddress, uint8_t advAddrType );

#ifdef __cplusplus
}
#endif

#endif /* LL_PADV_SCAN_H */
