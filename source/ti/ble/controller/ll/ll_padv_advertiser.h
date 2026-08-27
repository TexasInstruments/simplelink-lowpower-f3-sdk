/******************************************************************************
 @file  ll_padv_advertiser.h

 @brief Write description here.

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

 #ifndef LL_PADV_ADVERTISER_H
 #define LL_PADV_ADVERTISER_H
 #ifdef __cplusplus
 extern "C"
 {
 #endif

/*********************************************************************
 * INCLUDES
 */

#include <stdint.h>
#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll_ae.h"

/*********************************************************************
 * CONSTANTS
 */

// Invalid handle for periodic advertiser
#define PERIODIC_ADV_INVALID_HANDLE                         0xFF

// Periodic advertiser state
#define PERIODIC_ADV_STATE_DISABLE                          0x0
#define PERIODIC_ADV_STATE_PENDING_ENABLE                   0x1
#define PERIODIC_ADV_STATE_PENDING_TRIGGER                  0x2
#define PERIODIC_ADV_STATE_ENABLE                           0x3

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

typedef struct
{
  uint16                            intervalMin;           // Minimum advertising interval for periodic advertising. (Range: 0x0006 to 0xFFFF) Time = N * 1.25 ms
  uint16                            intervalMax;           // Maximum advertising interval for periodic advertising. (Range: 0x0006 to 0xFFFF) Time = N * 1.25 ms
  uint8                             props[2];              // Set bit number 6 for include TxPower in the advertising PDU
} llPeriodicAdvParamsCmd_t;

typedef struct
{
  RCL_CmdBle5PeriodicAdvertiser    perAdvCmd;                           // Command to run BLE periodic advertiser
  RCL_CtxPeriodicAdvertiser        perAdvParam;                         // Context for periodic advertiser command
  RCL_StatsAdvScanInit             perAdvOutput;                        // Statistics for periodic advertiser command
  aePacket                         txBuffer[PERIODIC_NUM_TX_BUFFERS];   // Tx buffers for periodic advertiser packets
  comExtPktFormat_t                comPkt;                              // Common Extended Packet Entry Format
  uint16                           phyFeatures;                         // PHY feature selector
  uint8                            extHdr[EXTHDR_TOTAL_BUF_SIZE];       // Pointer to the extended header
  uint8                            buffNo:2;                            // Marks the next txBuffer that should be used
  uint8                            rfu:6;
} periodicRf_t;

typedef struct llPeriodicAdvSet_t
{
  struct llPeriodicAdvSet_t         *next;                 // Pointer to next handle in list, if any
  periodicRf_t                      *pRfCmds;              // ptr to allocated RF command memory
  llPeriodicAdvParamsCmd_t          paramsCmd;             // Pointer to periodic params command
  aeSetDataCmd_t                    dataCmd;               // Pointer to periodic data command
  llPeriodicAdvSyncInfo_t           syncInfo;              // Sync information in AUX_ADV_IND
  uint32                            startTime;             // start of periodic event - when PAwR is in use, will always point to subevnet 0
  uint32                            totalOtaTime;          // total OTA time in RAT ticks per length and PHY
  llPeriodicChanMap_t               *pChanMap;             // pointer to current channel map
  uint8                             *pData;                // pointer to periodic data
  uint16                            otaTime;               // OTA time of single packet in us per length and PHY
  uint16                            interval;              // Periodic advertising interval. (Range: 7.5 ms to 81918 ms)
  uint16                            numMissed;             // number of missed adv events
  uint16                            eventCounter;          // incremental counter for each AUX_SYNC_IND
  uint16                            chanMapUpdateEvent;    // channel map update instant
  uint16                            dataLen;               // length of periodic data
  uint8                             dataUpdated;           // Flag indication for updated periodic data by Host
  uint8                             maxAvailData;          // max space available for adv data in pkt
  uint8                             txCount;               // hold number of tx in current periodic event
  uint8                             fragLen;               // length of fragment
  uint8                             lastFragLen;           // length of last fragment
  uint8                             numFrags;              // number of advertising data fragments
  uint8                             numChains;             // number of advertising fragments (max of data or cte count)
  uint8                             handle;                // Used to identify a periodic advertising train (Range: 0x00 to 0xEF)
  uint8                             state;                 // Enable, disable or pending the periodic advertising
  uint8                             currentChan;           // the current unmapped channel for the periodic adv
  int8                              txPowerValueDbm;       // tx power value in dBm
  uint8                             phy;                   // phy used in periodic train
  uint8                             pendingDisable;        // disable periodic adv flag
  uint8                             pendingChanUpdate;     // flag to indicate channel map update is pending
  uint8                             intPriority;           // internal priority: priority scale to use in periodic adv selection procedure
  uint8                             extHdrSize;            // periodic adv extended header size
  uint8                             extHdr[PERIODIC_ADV_HDR_TOTAL_BUF_SIZE]; // buffer for periodic header
  uint8                             priority;              // priority as a secondary task.
  uint32                            auxStartTime;          // start time of an AUX_ADV_IND that contains the SyncInfo field of periodic advertisings
  uint8_t                          *pPAwRParams;           // pointer to the PAwR Parameters. NULL if set is not PAwR
} llPeriodicAdvSet_t;

typedef struct
{
  llPeriodicAdvSet_t                *advList;              // periodic advertisers list
  llPeriodicAdvSet_t                *currentAdv;           // current active periodic set
  taskInfo_t                        *llTask;               // Pointer to associated BLE task block (define new task with ID LL_TASK_ID_PERIODIC_ADV 0x08)
  llPeriodicAdvChanMap_t            chanMap;               // channel map struct
  uint8                             advNumActive;          // current number of active periodic sets
} llPeriodicAdv_t;

 /*********************************************************************
 * GLOBAL VARIABLES
 */

/**********************************************************************
 * API's FUNCTIONS
 */

/*******************************************************************************
 * @fn          LL_PadvA_IsEnable
 *
 * @brief       Indicates if the periodic advertising feature is enabled or disabled.
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
bool LL_PadvA_IsEnable( void );

/*******************************************************************************
 * @fn      LE_SetPeriodicAdvParams
 *
 * @brief   Used by the Host to set the advertiser parameters for periodic
 *          advertising. This command will be called from V2 and V1 command,
 *          where in V1 the PAwR parameters will be NULL.
 *
 * @design  /ref did_286039104
 *
 * input parameters
 *
 * @param   advHandle              - Used to identify a periodic advertising train
 *                                   Created by LE Set Extended Advertising
 *                                   Parameters command
 * @param   periodicAdvIntervalMin - Minimum advertising interval for periodic advertising
 *                                   Range: 0x0006 to 0xFFFF Time = N * 1.25 ms Time Range: 7.5ms to 81.91875 s
 * @param   periodicAdvIntervalMax - Maximum advertising interval for periodic advertising
 *                                   Range: 0x0006 to 0xFFFF Time = N * 1.25 ms Time Range: 7.5ms to 81.91875 s
 * @param   periodicAdvProp        - Periodic advertising properties - set bit 6 for
 *                                   include TxPower in the advertising PDU
 * @param   pPAwRParams            - Pointer to PAwR parameters. NULL if the periodic adv set is not PAwR
 *
 * output parameters
 *
 * @param       None.
 *
 * @return  LL_STATUS_SUCCESS if successful.
 *          LL_STATUS_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER - If the Advertising_Handle does
 *                                                           not refer to a valid advertising set.
 *          LL_STATUS_ERROR_BAD_PARAMETER                  - If the parameters are out of range.
 *          LL_STATUS_ERROR_COMMAND_DISALLOWED             - If the periodic advertising is enabled.
 *          LL_STATUS_ERROR_PACKET_TOO_LONG                - If the periodic advertising data length
 *                                                           is greater than the maximum.
 *          LL_STATUS_ERROR_MEM_CAPACITY_EXCEEDED          - No available memory to execute the command
 */
uint8_t LE_SetPeriodicAdvParams( uint8_t advHandle,
                                 uint16_t periodicAdvIntervalMin,
                                 uint16_t periodicAdvIntervalMax,
                                 uint16_t periodicAdvProp,
                                 uint8_t *pPAwRParams );

/*******************************************************************************
 * @fn      LE_SetPeriodicAdvData
 *
 * @brief   Used to set the advertiser data used in periodic advertising PDUs.
 *          This command may be issued at any time after the advertising set
 *          identified by the Advertising_Handle parameter has been configured
 *          for periodic advertising using the
 *          HCI_LE_Set_Periodic_Advertising_Parameters command
 *
 * @design  /ref did_286039104
 *
 * input parameters
 *
 * @param   advHandle  - Used to identify a periodic advertising train
 * @param   operation  - 0x00 - Intermediate fragment of fragmented periodic
 *                              advertising data
 *                       0x01 - First fragment of fragmented periodic adv data
 *                       0x02 - Last fragment of fragmented periodic adv data
 *                       0x03 - Complete periodic advertising data
 * @param   dataLength - The number of bytes in the Advertising Data parameter
 * @param   data       - Periodic advertising data
 *
 * output parameters
 *
 * @param       None.
 *
 * @return  status
 */
uint8_t LE_SetPeriodicAdvData( uint8_t advHandle,
                               uint8_t operation,
                               uint8_t dataLength,
                               uint8_t *data );

/*******************************************************************************
 * @fn      LE_SetPeriodicAdvEnable
 *
 * @brief   Used to request the advertiser to enable or disable
 *          the periodic advertising for the advertising set
 *
 * @design  /ref did_286039104
 *
 * input parameters
 *
 * @param   enable    - 0x00 - Periodic advertising is disabled (default)
 *                      0x01 - Periodic advertising is enabled
 * @param   advHandle - Used to identify a periodic advertising train
 *
 * output parameters
 *
 * @param       None.
 *
 * @return  status
 */
uint8_t LE_SetPeriodicAdvEnable( uint8_t enable,
                                 uint8_t advHandle );

/*******************************************************************************
 * @fn          LL_PadvA_GetSetByHandle
 *
 * @brief       This function is used to get the periodic adv by handle
 *
 * @design  /ref did_286039104
 *
 * input parameters
 *
 * @param       handle - periodic advertiser handle.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      pPeriodicAdv - A pointer to periodic advertiser.
 */
llPeriodicAdvSet_t *LL_PadvA_GetSetByHandle( uint8_t handle );

/*******************************************************************************
 * @fn          LL_PadvA_AddACADToAdv
 *
 * @brief       This routine is used to add ACAD to periodic advertising
 *              packet when PAwR is enabled.
 *
 * @Design:     BLE_LOKI-1795
 * @Design      /ref did_286039104
 *
 * input parameters
 *
 * @param       handle    - periodic advertiser handle.
 * @param       pBuf     - pointer to packet header.
 * @param       pAcadLen  - pointer to ACAD length.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
uint8_t LL_PadvA_AddACADToAdv( uint8_t handle, uint8_t *pBuf, uint8 *pAcadLen );

/*******************************************************************************
 * @fn          LL_PadvA_IsExistByHandle
 *
 * @brief       Checks if a periodic advertising set exists for the given handle.
 *
 * input parameters
 *
 * @param       handle - periodic advertiser handle.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      TRUE if the periodic advertising set exists,
 *              FALSE otherwise.
 */
bool LL_PadvA_IsExistByHandle( uint8_t handle );

/*******************************************************************************
 * @fn          LL_PadvA_GetCurrent
 *
 * @brief       This function is used to get the current periodic Adv
 *
 * @design  /ref did_286039104
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      pPeriodicAdv - A pointer to the current periodic advertiser.
 */
llPeriodicAdvSet_t *LL_PadvA_GetCurrent( void );

/*******************************************************************************
 * @fn          LL_PadvA_PostProcess
 *
 * @brief       This routine is used to post process the periodic Advertising
 *              command.
 *
 * @Design:  BLE_LOKI-1795
 * @Design   /ref did_286039104
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
void LL_PadvA_PostProcess( void );

/*******************************************************************************
 * @fn          LL_PadvA_BuildSyncInfo
 *
 * @brief       This routine is used to set the content of the sync info
 *              in AUX_ADV_IND
 *
 * @Design:     BLE_LOKI-1795
 * @Design      /ref did_286039104
 *
 *
 * input parameters
 *
 * @param       pAdvSet   - Pointer to the advertising set for this command.
 * @param       pBuf  - pointer to packet header.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_PadvA_BuildSyncInfo( advSet_t *pAdvSet, uint8_t *pBuf );

/*******************************************************************************
 * @fn          LL_PadvA_SetupCommand
 *
 * @brief       This routine is used to setup the periodic advertising command.
 *
 * @Design:     BLE_LOKI-1795
 * @Design      /ref did_286039104
 *
 * input parameters
 *
 * @param       pAdvSet - Pointer to the advertising set for this command.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
uint8_t         LL_PadvA_SetupCommand( advSet_t *pAdvSet );

/*******************************************************************************
 * @fn          LL_PadvA_SetChanMapUpdate
 *
 * @brief       This function is used to set the periodic advertiser
 *              channel map update procedure
 *
 * @design  /ref did_286039104
 *
 * input parameters
 *
 * @param       set - TRUE - Host was updated the channel map,
 *                    FALSE - all periodics moved to new channel map
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_PadvA_SetChanMapUpdate( uint8_t set );

/*******************************************************************************
 * @fn          LL_PadvA_UpdateChainPacket
 *
 * @brief       This routine is used to update RF command in case chain
 *              packet (AUX_CHAIN_IND) should be send on current periodic adv set
 *
 * @Design:     BLE_LOKI-1795
 * @Design      /ref did_286039104
 *
 * input parameters
 *
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_PadvA_UpdateChainPacket( void );

/*******************************************************************************
 * @fn      LL_PadvA_ValidateExtAdvParams
 *
 * @brief   Validates extended advertising parameters for a periodic
 *          advertising set. Checks that, when periodic advertising is
 *          enabled for the specified advertising set, the PHY and
 *          event properties are compatible with periodic advertising
 *          requirements.
 *
 * input parameters
 *
 * @param   pCmdParams - Pointer to the extended advertising parameter
 *                       command structure.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  LL_STATUS_SUCCESS if the parameters are valid,
 *          otherwise returns an error code.
 */
uint8_t LL_PadvA_ValidateExtAdvParams( aeSetParamCmd_t *pCmdParams );

/*******************************************************************************
 * @fn          LL_PadvA_GetStateByHandle
 *
 * @brief       This function is used to get the periodic Adv state by handle.
 *
 * input parameters
 *
 * @param       handle - The handle identifying the periodic advertising set.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      The state of the periodic advertising set.
 *              PERIODIC_ADV_INVALID_HANDLE for invalid handle.
 *              Otherwise, one of the following:
 *              - PERIODIC_ADV_STATE_DISABLE
 *              - PERIODIC_ADV_STATE_PENDING_ENABLE
 *              - PERIODIC_ADV_STATE_PENDING_TRIGGER
 *              - PERIODIC_ADV_STATE_ENABLE
 */
uint8_t LL_PadvA_GetStateByHandle( uint8_t handle );

/*******************************************************************************
 * @fn          LL_PadvA_GetPendingDisableFlag
 *
 * @brief       Returns TRUE if the periodic advertising set identified by the
 *              handle has pendingDisable set, otherwise returns FALSE.
 *
 * input parameters
 *
 * @param       handle - The handle identifying the periodic advertising set.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      PERIODIC_ADV_INVALID_HANDLE for invalid handle.
 *              Otherwise, the value of the pendingDisable flag (TRUE or FALSE).
 */
uint8_t LL_PadvA_GetPendingDisableFlag( uint8_t handle );

/*******************************************************************************
 * @fn          LL_PadvA_InitPAwRSubeventDataList
 *
 * @brief       This routine initializes the PAwR subevent data request procedure,
 *              for PAdv Set that use PAwR. If PAwR is not used, this function does nothing.
 *              It sends HCI_PadvASubeventDataRequestEvent to the host to request
 *              subevent data. This should be called early (after command complete
 *              but before trigger) to give the host time to respond.
 *
 * input parameters
 *
 * @param       handle - The advertising handle for the periodic advertising set.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_PadvA_InitPAwRSubeventDataList( uint8_t handle );

/*******************************************************************************
 * @fn          LL_PadvA_TriggerCommand
 *
 * @brief       This routine is used to execute the periodic advertising command.
 *
 * @Design:     BLE_LOKI-1795
 * @Design      /ref did_286039104
 *
 * input parameters
 *
 * @param       pAdvSet      - Pointer to the advertising set for this command.
 * @param       pPeriodicAdv - Pointer to the periodic advertising set for this
 *                             command.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS                     - Command executed successfully.
 *              LL_STATUS_ERROR_BAD_PARAMETER         - Invalid input parameter.
 *              LL_STATUS_ERROR_MEM_CAPACITY_EXCEEDED - No available memory
 *                                                      to execute the command
 */
uint8_t LL_PadvA_TriggerCommand( advSet_t           *pAdvSet,
                                 llPeriodicAdvSet_t *pPeriodicAdv );

/*******************************************************************************
 * @fn          LL_PadvA_ClearAllSets
 *
 * @brief       This function is used to clear all periodic adv sets
 *
 * @Design:     BLE_LOKI-1795
 * @Design      /ref did_286039104
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
void LL_PadvA_ClearAllSets( void );

/*******************************************************************************
 * @fn          LL_PadvA_GetNextSet
 *
 * @brief       This routine is used find next Periodic Adv Set in periodic adv
 *              sorted list.
 *
 * @Design:     BLE_LOKI-1795
 * @Design      /ref did_286039104
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Pointer
 */
void *LL_PadvA_GetNextSet( void );

/*******************************************************************************
 * @fn          LL_PadvA_GetCurrentTotalOtaTime
 *
 * @brief       This function is used to get the current periodic Adv Total
 *              OTA Time
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       curTotalOtaTime - Current TotalOtaTime
 *
 * @return      llPeriodicAdv.currentAdv->totalOtaTime if currentAdv is valid
 *              Otherwise, return curTotalOtaTime
 */
uint32_t LL_PadvA_GetCurrentTotalOtaTime( uint32_t curTotalOtaTime );

/*******************************************************************************
 * @fn          LL_PadvA_GetCurrentQOSPriority
 *
 * @brief       This function is used to get the current periodic Adv state
 *              priority
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       curllStatePriority - Current ll state Priority.
 *
 * @return      llPeriodicAdv.currentAdv->priority if currentAdv is valid
 *              Otherwise, return curllStatePriority
 */
uint8_t LL_PadvA_GetCurrentQOSPriority( uint8_t curllStatePriority );

/*******************************************************************************
 * @fn          LL_PadvA_CompareQosPriorityToConn
 *
 * @brief       Compares the QoS priority of the current periodic advertising
 *              task to the connection priority.
 *
 * input parameters
 *
 * @param       primConnPtr - Pointer to the primary connection state.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      TRUE if periodic advertising QoS priority is higher or equal,
 *              FALSE otherwise.
 */
uint8_t LL_PadvA_CompareQosPriorityToConn(llConnState_t *primConnPtr);

/*****************************************************************************
 * @fn      LL_PadvA_SetQOSPriority
 *
 * @brief   This function sets the Quality of Service (QoS) priority for a
 *          periodic advertising task.
 *
 * input parameters
 *
 * @param   priority    - The QoS priority value to be set for the periodic
 *                        advertising task.
 * @param   handle      - The handle of the periodic advertising for which
 *                        the QoS priority is being set.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  None
 */
void LL_PadvA_SetQOSPriority( uint32_t priority, uint16_t handle );

/*****************************************************************************
 * @fn      LL_PadvA_GetTxUsageParams
 *
 * @brief   This function retrieves the transmission usage parameters
 *          for the next active periodic advertising task. It calculates
 *          the total transmission time for all fragments in the TX buffer
 *          and retrieves the current channel and TX power for the
 *          periodic advertising task.
 *
 * input parameters
 *
 * @param   pExtAdvTxParams - Pointer to the structure where the periodic
 *                            advertising TX usage parameters will be stored.
 *                            The structure includes:
 *                            - totalTxTime: Total transmission time in
 *                                           microseconds.
 *                            - txPower: Transmit power in dBm.
 *                            - channel: BLE channel.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  None
 */
void LL_PadvA_GetTxUsageParams(llTxUsageParams_t *pPeriodicTxParams);

/*******************************************************************************
 * @fn          LL_PadvA_GetSyncTransferInfoByHandle
 *
 * @brief       This function is used to get the periodic sync transfer info
 *              by advertising handle.
 *
 * input parameters
 *
 * @param       advHandle                 - Advertising handle of the periodic
 *                                          advertising set.
 * @param       pPeriodicSyncTransferData - Pointer to the structure where the
 *                                          periodic sync transfer data will be
 *                                          stored.
 *
 * output parameters
 *
 * @param       pPeriodicSyncTransferData - Pointer to the structure that will
 *                                          get the periodic sync transfer data.
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS when operation succeeds.
 *              Otherwise:
 *              LL_STATUS_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER
 */
uint8_t LL_PadvA_GetSyncTransferInfoByHandle( uint8_t                      advHandle,
                                              llPeriodicSyncTransferInfo_t *pPeriodicSyncTransferData);

/*******************************************************************************
* @fn          LL_PadvA_GetTypeByHandle
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
*              - LL_PERIODIC_ADV_TYPE_UNKNOWN_IDENTIFIER
*/
llPeriodicAdvSetType_e LL_PadvA_GetTypeByHandle( uint8_t advHandle );

#ifdef __cplusplus
}
#endif

#endif /* LL_PADV_ADVERTISER_H */
