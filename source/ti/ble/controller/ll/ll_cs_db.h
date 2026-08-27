/******************************************************************************

 @file  ll_cs_db.h

 @brief This file contains the Data Structures and APIs for the Channel Souding
        Feature in the Bluetooth Low Energy (BLE) Controller.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2026, Texas Instruments Incorporated

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
#ifndef LL_CS_DB_H
#define LL_CS_DB_H
/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/controller/ll/ll_cs_common.h"
#include "ti/ble/controller/ll/ll_cs_test.h"

/*******************************************************************************
 * EXTERNS
 */

// Timing parameter lookup tables (defined in ll_cs_db.c)
extern const uint16 tIpTbl[];
extern const uint16 tFcsTbl[];
extern const uint16 tPmTbl[];

// Test override data (defined in ll_cs_db.c)
extern csTestOverrideData_t csTestOverrides;

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */

/*******************************************************************************
 * @fn          llCsInitDb
 *
 * @brief       Initializes the Database
 * Allocating the memory required the CS Database.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status
 *              CS_INSUFFICIENT_MEMORY
 *              CS_SUCCESS
 */
uint8 llCsInitDb(void);

/*******************************************************************************
 * @fn          llCsDbClearCsData
 *
 * @brief       Initializes and resets the CS connection data
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection identifier.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbClearCsConnData(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbClearProcedureInfo
 *
 * @brief       Clear CS procedure info
 * Used when CS procedure ends.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbClearProcedureInfo(void);

/*******************************************************************************
 * @fn          llCsDbClearFilteredChanIdxData
 *
 * @brief       Clear CS channel indexes data
 * Used when CS procedure ends and is not to be repeated.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbClearFilteredChanIdxData(void);

/*******************************************************************************
 * @fn          llCsDbFree
 *
 * @brief       Free the database
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbFree(void);

/*******************************************************************************
 * @fn          llCsDbConnFree
 *
 * @brief       Free the CS connection data
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbConnFree(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbMarkCtrlProcedureCompleted
 *
 * @brief       Marks the bit of the provided cs procedure ID as completed (on)
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection identifier.
 * @param       csProcedureId - the procedure identifier to be marked as
 * completed
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbMarkCtrlProcedureCompleted(uint16 connId, uint8 csProcedureId);

/*******************************************************************************
 * @fn          llCsDbClearProcedureCompleted
 *
 * @brief       Clear Procedure Completed Flag
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       csProcedureID - prcoedure ID
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbClearProcedureCompleted(uint16 connId, uint8 csProcedureId);

/*******************************************************************************
 * @fn          llCsDbIsProcedureCompleted
 *
 * @brief       Check if a CS procedure is completed
 * This function checks if a CS procedure is completed by checking
 * if the CS procedure's bit is active in the completedProcedure's bitmap.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Handle
 * @param       procedureId - Procedure bit, use the macros defined
 *                          in CS Ctrl Procedures
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      true if the proecure is completed, false otherwise.
 */
bool llCsDbIsProcedureCompleted(uint16 connId, uint8 procedureId);

/*******************************************************************************
 * @fn          llCsDbIsProcedureCompleted
 *
 * @brief       Get the CS completed procedure bitmap.
 * This function is used to indicate if CS configuration is present for this connId
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Handle
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      completedProcedure's bitmap.
 */
uint8 llCsDbGetProcedureCompleted(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbSetActiveCtrlProcedure
 *
 * @brief       Set Active CS Procedure.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Handle
 * @param       procedureId - Procedure bit, use the macros defined
 *                          in CS Ctrl Procedures
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetActiveCtrlProcedure(uint16 connId, uint8 procedureId);

/*******************************************************************************
 * @fn          llCsDbSetInactiveCtrlProcedure
 *
 * @brief       Set InActive status in the Active CS Procedure.
 *
 * input parameters
 *
 * @param       connId - connection Handle
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetInactiveCtrlProcedure(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbGetActiveCtrlProcedure
 *
 * @brief       Set Active CS Procedure.
 *
 * input parameters
 *
 * @param       connId - connection Handle
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
uint8 llCsDbGetActiveCtrlProcedure(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbIsCsCtrlProcedureInProgress
 *
 * @brief Check if the connection is in active procedure state
 *
 * This function checks whether a specific procedure is currently active
 * for a given connection.
 *
 * input parameters
 *
 * @param       connId - connection Handle
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      true if the procedure is active,
 *              false otherwise.
 */
bool llCsDbIsCsCtrlProcedureInProgress(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbGetLocalCapabilities
 *
 * @brief       Gets the local CS capabilities.
 * This API is called once a Read Local CS capabilities HCI command is received.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       pLocalCapabilities - pointer to local Capabilities.
 *
 * output parameters
 *
 * @param       pLocalCapabilities
 *
 * @return      None
 */
const llCsCapabilities_t* llCsDbGetLocalCapabilities( void );

/*******************************************************************************
 * @fn          llCsDbSetPeerCapabilities
 *
 * @brief       Sets the CS Peer Capabilities of the connection
 * Note: in the future it should also be stored in NV.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       pRemoteCapabilities - pointer to remote Capabilities.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetPeerCapabilities(uint16 connId,
                               const llCsCapabilities_t* pRemoteCapabilities);

/*******************************************************************************
 * @fn          llCsDbGetPeerCapabilities
 *
 * @brief       Gets the CS Peer Capabilities of the connection
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      pRemoteCapabilities - pointer to remote Capabilities.
 */
const llCsCapabilities_t* llCsDbGetPeerCapabilities(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbGetLocalFaeTbl
 *
 * @brief       Get Local FAE table
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       pFae - pointer to local FAE table
 *
 * output parameters
 *
 * @param       pFae - pointer to FAE table.
 *
 * @return      Status
 *              CS_STATUS_UNSUPPORTED_FEATURE if NO FAE
 *              CS_STATUS_SUCCESS if FAE
 */
csStatus_e llCsDbGetLocalFaeTbl(csFaeTbl_t* pFae);

/*******************************************************************************
 * @fn          llCsDbSetRemoteFaeTbl
 *
 * @brief       Set Remote FAE table
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       pFae - pointer to FAE table to set.
 * @param       connId - connection handle
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      status
 */
csStatus_e llCsDbSetRemoteFaeTbl(uint16 connId, const csFaeTbl_t* pFae);

/*******************************************************************************
 * @fn          llCsDbGetRemoteFaeTbl
 *
 * @brief       Get Remote Fae Table
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection handle
 *
 * output parameters
 *
 * @return       pFae.
 *
 */
const csFaeTbl_t* llCsDbGetRemoteFaeTbl(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbApplyConfigReqCtrlPkt
 *
 * @brief       Set CS configuration by configuration Id.
 * CS configuration memory is allocated, then the data is set.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       pConfig - pointer to configuration set
 * output parameters
 *
 * @param       None
 *
 * @return      Status
 *              CS_STATUS_INSUFFICIENT_MEMORY if mem alloc failed
 */
csStatus_e llCsDbApplyConfigReqCtrlPkt(uint16 connId, const csConfigurationSet_t* pConfig);

/*******************************************************************************
 * @fn          llCsDbRemoveConfiguration
 *
 * @brief       Disable configuration
 * Triggered by Remove configuration HCI command. This frees the memory
 * allocated for this configuration Then sets it as NULL.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection handle
 * @param       configId - configuration Id to disable
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbRemoveConfiguration(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbGetpCurrentConfigRspId
 *
 * @brief       Get current configuration response Id
 * This is the configuration Id that was last sent in a CS Config Request
 *
 * input parameters
 *
 * @param       connId - connection ID
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      current configuration response Id
 */
uint8_t llCsDbGetpCurrentConfigRspId(uint16_t connId);

/*******************************************************************************
 * @fn          llCsDbSetpCurrentConfigRspId
 *
 * @brief       Set current configuration response Id
 * This is the configuration Id that was last sent in a CS Config Request
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       configId - configuration Id to set as current response Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetpCurrentConfigRspId(uint16_t connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsDbGetConfigState
 *
 * @brief       Get CS configuration State (enabled or disabled)
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       configID - cs config ID
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      CS_DISABLED - if config is disabled
 *              CS_ENABLED  - if config is enabled
 */
uint8 llCsDbGetConfigState(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDBGetConfigRole
 *
 * @brief       Get CS Role from CS config
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       configID - cs config ID
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      CS_ROLE_INITIATOR or CS_ROLE_REFLECTOR
 */
uint8_t llCsDbGetConfigRole(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbGetConfigPeerRole
 *
 * @brief       Get CS peer Role from CS config
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       configID - cs config ID
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      CS_ROLE_INITIATOR or CS_ROLE_REFLECTOR
 */
uint8_t llCsDbGetConfigPeerRole(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbGetConfiguration
 *
 * @brief       Gets CS configuration by configuration Id
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       configId - CS configuration Id
 *
 * output parameters
 *
 *
 * @return      const pointer to configuration set
 */
const csConfigurationSet_t* llCsDbGetConfiguration(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbSetChanMapIndData
 *
 * @brief       Sets the Channel Map Indication data received from peer device
 *              in the CS DB.
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       pNewChanMap - pointer to new channel map received from peer device
 * @param       instanteEvt - event instant when the new channel map shall be applied
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetChanMapIndData( uint8_t connId , csChm_t* pNewChanMap, uint16_t instanteEvt);

/*******************************************************************************
 * @fn          llCsDbSetPeerChannelMapClassification
 *
 * @brief       Restores the full peer channel-map classification state in the
 *              CS DB. Intended for use during connection handover (CN side).
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       pChanMapClass - pointer to the classification state to restore
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetPeerChannelMapClassification(uint8_t connId, const llCsChannelMapClassification_t *pChanMapClass);

/*******************************************************************************
 * @fn          llCsDbApplyChanMapUpdate
 *
 * @brief       Applies the Channel Map update received from peer device
 *              in the CS DB.
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbApplyChanMapUpdate(uint8_t connId);

/*******************************************************************************
 * @fn          llCsDbStartUsingClassifiedPeerChanMap
 *
 * @brief       Start using the classified peer channel map as current channel map
 *              This is used when a CS procedure starts, to ensure that the
 *              classified channel map is used for the procedure.
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbStartUsingClassifiedPeerChanMap(uint8_t connId);

/*******************************************************************************
 * @fn          llCsDbGetClassifiedPeerChannelMap
 *
 * @brief       Gets CS Channel Map structure by connection Id
 *
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 *
 * @return      const pointer to CS Channel Map structure
 */
const llCsChannelMapClassification_t *llCsDbGetClassifiedPeerChannelMap(uint8 connId);

/*******************************************************************************
 * @fn          llCsDbGetCs
 *
 * @brief       Gets the whole CS configuration by configuration Id
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 *
 * @return      - const pointer to configuration set
 */
const llCs_t* llCsDbGetCs(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbSetCurrentConfigId
 *
 * @brief       Set current configuration id used for active procedure.
 * This is needed because there is an array of configurations
 * And when setting up we may lose the current index
 * This is used to store it
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       configId - CS configuration Id
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetCurrentConfigId(uint8 configId);

/*******************************************************************************
 * @fn          llCsDbRemobeCurrentConfigId
 *
 * @brief       Remove current config ID
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbRemoveCurrentConfigId(void);

/*******************************************************************************
 * @fn          llCsDbGetCurrentConfigId
 *
 * @brief       Get current configuration id used for active procedure.
 * This is needed because there is an array of configurations
 * And when setting up we may lose the current index
 * This is used to get it
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       None
 * output parameters
 *
 * @param       None
 *
 * @return      current config Id
 */
uint8 llCsDbGetCurrentConfigId(void);

/*******************************************************************************
 * @fn          llCsDbGetCtrlPktConfigReqId
 *
 * @brief       Get the configId for configuration request.
 * This is required because the control procedures might run for one configId while
 * there is an active CS procedure for another configId. They should not override each other
 *
 * input parameters
 *
 * @param       connId - connection Id
 * output parameters
 *
 * @param       None
 *
 * @return      configuration config Id
 */
uint8 llCsDbGetCtrlPktConfigReqId(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbSetCtrlPktConfigReqId
 *
 * @brief        Set the configId for configuration request.
 * This is required because the control procedures might run for one configId while
 * there is an active CS procedure for another configId. They should not override each other
 *
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       configurationConfigId - CS configuration Id
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetCtrlPktConfigReqId(uint16 connId, uint8 configurationConfigId);

/*******************************************************************************
 * @fn          llCsDbCreateCandidateConfigReq
 *
 * @brief       Create Candidate CS configuration request control packet set.
 *              The Set will be store in a pending state till the confirmation arrives.
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       pCsConfigReq - pointer to configuration set
 * output parameters
 *
 * @param       None
 *
 * @return      Status
 *              CS_STATUS_INSUFFICIENT_MEMORY if mem alloc failed
 *              CS_STATUS_ERROR_TRANSACTION_COLLISION if there is already a pending config req
 *              CS_STATUS_SUCCESS if success
 */
csStatus_e llCsDbCreateCandidateConfigReq( uint16_t connId, const csConfigurationSet_t *pCsConfigReq );

/*******************************************************************************
 * @fn          llCsDbRemovePendingConfigReqCtrlSet
 *
 * @brief       Remove CS configuration request control packet set.
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Status
 *              CS_STATUS_UNEXPECTED_PARAMETER if there is no config req to remove
 *              CS_STATUS_SUCCESS if success
 */
uint8_t llCsDbRemovePendingConfigReqCtrlSet(uint16_t connId);

/*******************************************************************************
 * @fn          llCsDbGetConfigReqCtrlPkt
 *
 * @brief       Get CS configuration request control packet set.
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       configId - CS configuration Id
 * output parameters
 *
 * @param       None
 *
 * @return      const pointer to configuration set
 */
const csConfigurationSet_t* llCsDbGetConfigReqCtrlPkt(uint16_t connId);

/*******************************************************************************
 * @fn          llCsDbIsCsProcedureInProgress
 *
 * @brief       Is any CS procedure enabled per provided connection ID
 *
 * input parameters
 *
 * @param       connId - connection ID
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      true in case any CS procedure is enabled, false otherwise
 */
uint8 llCsDbIsCsProcedureInProgress(void);

/*******************************************************************************
 * @fn          llCsDbGetSubEventDoneStatus
 *
 * @brief       Get the SubEvent Done status
 *
 * input parameters
 *
 * @param       connId - connection ID
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      SubEvent Done status
 */
uint8_t llCsDbGetSubEventDoneStatus(void);

/*******************************************************************************
 * @fn          llCsDbSetSubEventDoneStatus
 *
 * @brief       Set the SubEvent Done status
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       subeventDoneStatus - SubEvent Done status
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetSubEventDoneStatus(uint8_t subeventDoneStatus);

/*******************************************************************************
 * @fn          llCsDbGetProcedureDoneReason
 *
 * @brief       Get the CS Procedure Done Error Reason
 *
 * input parameters
 *
 * @param       connId - Connection Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      The error code for the temrination reason
 */
uint8 llCsDbGetProcedureDoneReason(void);

/*******************************************************************************
 * @fn          llCsDbSetProcedureDoneAborted
 *
 * @brief       Set the CS Procedure Done Error Status & Reason
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       reason - The reason code for the temrination reason
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetProcedureDoneAborted(uint8 reason);

/*******************************************************************************
 * @fn          llCsDbClearSubEventDone
 *
 * @brief       Clear the CS SubEvent Done Status and Reason
 *
 * input parameters
 *
 * @param       connId - Connection Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbClearSubEventDone(void);

/*******************************************************************************
 * @fn          llCsDbGetSubEventDoneReason
 *
 * @brief       Get the CS SubEvent Terminate Reason
 *
 * input parameters
 *
 * @param       connId - Connection ID
 * output parameters
 *
 * @param       None
 *
 * @return      SubEvent Error Code (Termination Reason)
 */
uint8 llCsDbGetSubEventDoneReason(void);

/*******************************************************************************
 * @fn          llCsDbSetSubEventDoneAborted
 *
 * @brief       Set the CS Sub Event Termination Reason
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       reason - The error code for the temrination reason
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetSubEventDoneAborted(uint8 reason);


/*******************************************************************************
 * @fn          llCsDbGetEnableProcedureCount
 *
 * @brief       Get the procedure enable procedure count parameter
 *
 * input parameters
 *
 * @param       connId    - connection Id
 * @param       configId  - CS configuration Id
 * output parameters
 *
 * @param       None
 *
 * @return      The number of the procedure repetitions
 */
uint16 llCsDbGetEnableProcedureCount(void);

/*******************************************************************************
 * @fn          llCsDbGetEnableProcedureInterval
 *
 * @brief       Get the procedure enable procedure event interval parameter
 *
 * input parameters
 *
 * @param       connId   - connection Id
 * @param       configId - CS configuration Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Procedure interval
 */
uint16_t llCsDbGetEnableProcedureInterval(void);

/*******************************************************************************
 * @fn          llCsDbSetProcedureDoneStatus
 *
 * @brief       Set the procedure done status for a given connection ID.
 *
 * input parameters
 *
 * @param       connId              - connection Id
 * @param       procedureDoneStatus - The procedure done status to be set
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetProcedureDoneStatus(uint8_t procedureDoneStatus);

/*******************************************************************************
 * @fn          llCsDbGetProcedureDoneStatus
 *
 * @brief       Get the procedure done status for a given connection ID.
 *
 * input parameters
 *
 * @param       connId - connection Id
 * output parameters
 *
 * @param       None
 *
 * @return      The procedure done status for the specified connection ID
 */
uint8_t llCsDbGetProcedureDoneStatus(void);

/*******************************************************************************
 * @fn          llCsDbIsCsProcedureParamsSet
 *
 * @brief       Indicates whether a Set Procedure Parameters command has been
 *              issued by the local device, for a specific config ID.
 *
 * input parameters
 *
 * @param       connId   - connection Id
 * @param       configId - configuration Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      true - if the Set Procedure Parameters command has been issued
 * @return      false - if the Set Procedure Parameters command has not been issued
 */
bool llCsDbIsCsProcedureParamsSet(uint16_t connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsDbUpdateLocalChm
 *
 * @brief       Update the local channel map in the CS DB.
 *              This is used when @ref LL_CS_SetChannelClassification is called.
 *
 * input parameters
 *
 * @param       pNewChm - pointer to new channel map
 *
 * output parameters
 *
 * @param       None
 *
 * @return      SUCCESS - if the channel map was updated successfully
 *              ERROR_UNEXPECTED_PARAMETER - if the provided channel map is invalid
 *              ERROR_INSUFFICIENT_MEMORY - no memory to allocate channel map
 */
csStatus_e llCsDbUpdateLocalChm(uint8_t *pNewChm);

/*******************************************************************************
 * @fn          llCsDbSetChannelClassificationReqTime
 *
 * @brief       Set the time when the Channel Classification update API has called
 *              @ref LL_CS_SetChannelClassification.
 *
 * input parameters
 *
 * @param       requestTime - time when the request was sent
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetChannelClassificationReqTime( uint32_t requestTime );

/*******************************************************************************
 * @fn          llCsDbSetDefaultSettings
 *
 * @brief       Sets the default settings in the CS DB
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       defaultSettings - default settings: role enable and
 *                                csSyncAntennaSelection
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetDefaultSettings(uint16 connId,
                              const csDefaultSettings_t* defaultSettings);

/*******************************************************************************
 * @fn          llCsDbGetDefaultSettings
 *
 * @brief       Gets the default settings from the CS DB
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       defaultSettings - default settings: role enable and
 *                                csSyncAntennaSelection
 * output parameters
 *
 * @param       None
 *
 * @return      defaultSettings
 */
void llCsDbGetDefaultSettings(uint16 connId,
                              csDefaultSettings_t* defaultSettings);

/*******************************************************************************
 * @fn          llCsDbGetDefaultMaxTxPower
 *
 * @brief       Gets the Max Tx Power from the CS Default settings for
 *              a given connection
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Max Tx Power
 */
int8_t llCsDbGetDefaultMaxTxPower(uint16_t connId);

/*******************************************************************************
 * @fn          llCsDbGetDefaultCsSyncAntennaSelection
 *
 * @brief       Gets the CS Sync Antenna Selection from the CS Default settings for
 *              a given connection
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      CS Sync Antenna Selection
 */
uint8_t llCsDbGetDefaultCsSyncAntennaSelection(uint16_t connId);

/*******************************************************************************
 * @fn          llCsDbSetProcedureCsSyncAntennaSelection
 *
 * @brief       This function Sets the current CS Sync Antenna Selection in
 *              procedureInfo for the given connection.
 *
 * input parameters
 *
 * @param       connId - connection Id to be used
 * @param       csSyncAntennaSelection - CS Sync Antenna Selection value to set
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetProcedureCsSyncAntennaSelection(csSyncAntennaSelection_t csSyncAntennaSelection);

/*******************************************************************************
 * @fn          llCsDbGetProcedureCsSyncAntennaSelection
 *
 * @brief       This function gets the current CS Sync Antenna Selection in
 *              procedureInfo for the given connection.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      CS Sync Antenna Selection
 */
csSyncAntennaSelection_t llCsDbGetProcedureCsSyncAntennaSelection();

/*******************************************************************************
 * @fn          llCsDbSetProcedureData
 *
 * @brief       Stores the negotiated CS procedure data from a REQ/RSP exchange.
 *              Called after negotiation succeeds; also initialises the
 *              connEvent repetitions tracker.
 *
 * input parameters
 *
 * @param       connId    - connection Id
 * @param       configId  - configuration Id
 * @param       pEnable - negotiated procedure data
 *
 * output parameters
 *
 * @param       None
 *
 * @return      void
 */
void llCsDbSetProcedureData(const csEnableProcedureCtrlData_t *pEnable);
/*******************************************************************************
 * @fn          llCsDbSetProcedureParams
 *
 * @brief       Stores the CS procedure parameters for a given connection and
 *              configuration. These are the host-configured maximums used when
 *              building a CS_REQ (max count, duration, interval, etc.).
 *
 * input parameters
 *
 * @param       connId   - connection Id
 * @param       configId - configuration Id
 * @param       pParams  - procedure parameters to store
 *
 * output parameters
 *
 * @param       None
 *
 * @return      void
 */
void llCsDbSetProcedureParams(uint16 connId, uint8 configId,
                              const csProcedureParams_t *pParams);

/*******************************************************************************
 * @fn          llCsDbGetProcedureParams
 *
 * @brief       Returns a pointer to the stored CS procedure parameters for
 *              the given connection and configuration.
 *
 * input parameters
 *
 * @param       connId   - connection Id
 * @param       configId - configuration Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Pointer to the stored procedure parameters (never NULL)
 */
const csProcedureParams_t *llCsDbGetProcedureParams(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbGetProcedureData
 *
 * @brief       Returns a const pointer to the stored CS procedure data.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Pointer to the stored procedure data (never NULL after setup)
 */
const csEnableProcedureCtrlData_t *llCsDbGetProcedureData();

/*******************************************************************************
 * @fn          llCsDbGetSubeventsPerEvent
 *
 * @brief       Get the number of subevents per event in a CS procedure
 *
 * @param       connId - connection Id
 * @param       configId - configuration Id
 *
 * @return      Number of subevents per event
 */
uint8 llCsDbGetSubeventsPerEvent(void);

/*******************************************************************************
 * @fn          llCsDbGetProcedureEnableOffset
 *
 * @brief       Get the offset for CS procedure
 *
 * @param       None
 *
 * @return      Number of subevents per event
 */
uint32_t llCsDbGetProcedureEnableOffset(void);

/*******************************************************************************
 * @fn          llCsDbGetProcedureEnableSubEventInterval
 *
 * @brief       Get the sub-event interval for CS procedure
 *
 * @param       connId - connection Id
 * @param       configId - configuration Id
 *
 * @return      Number of subevents per event
 */
uint16 llCsDbGetProcedureEnableSubEventInterval(void);

/*******************************************************************************
 * @fn          llCsDbGetSubeventsLen
 *
 * @brief       Get the subevent lenght in a CS procedure
 *
 * @param       None
 *
 * @return      Number of subevents per event
 */
uint32 llCsDbGetSubeventsLen(void);


/*******************************************************************************
 * @fn          llCsDbGetNextProcedureConnEvent
 *
 * @brief       Get the ACL counter that the CS is pending
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      ACL Counter
 */
uint16 llCsDbGetNextProcedureConnEvent(void);

/*******************************************************************************
 * @fn          llCsDbSetNextProcedureConnEvent
 *
 * @brief       Set the connEvent that the next CS Event or Procedure should
 *              begin from
 *
 * input parameters
 *
 * @param       connId    - connectionId
 * @param       configId  - config Id
 * @param       connEvent - connection event
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetNextProcedureConnEvent(uint16 connEvtCount);

/*******************************************************************************
 * @fn          llCsDbSetProcedureDataFromInd
 *
 * @brief       Updates the stored CS procedure data with IND-specific fields
 *              (offset, connEventCount, and other negotiated values).
 *              Called when a CS_IND is received and validated.
 *
 * input parameters
 *
 * @param       connId    - connection Id
 * @param       configId  - configuration Id
 * @param       pEnable - procedure data parsed from CS_IND
 *
 * output parameters
 *
 * @param       None
 *
 * @return      none
 */
void llCsDbSetProcedureDataFromInd(const csEnableProcedureCtrlData_t *pEnable);

/*******************************************************************************
 * @fn          llCsDbGetProcedureEnableACI
 *
 * @brief       Get the ACI from the active procedure enable state.
 *
 * input parameters
 *
 * @param       connId   - Connection Id
 * @param       configId - Config Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
csACI_e llCsDbGetProcedureEnableACI(void);

/*******************************************************************************
 * @fn          llCsDbUpdateLocalChm
 *
 * @brief       Update the local channel map
 *
 * @param       pNewChm - pointer to the new channel map
 *
 * @return      None.
 */
void lCsDbUpdateLocalChm(uint8_t *pNewChm);

/*******************************************************************************
 * @fn          llCsDbGetLocalChannelMap
 *
 * @brief       Get the local Channel Map classification
 *
 * @param       None.
 *
 * @return      const pointer to local Channel Map
 */
const csChm_t* llCsDbGetLocalChannelMap( void );

/*******************************************************************************
 * @fn          llCsDbGetLocalClassifiedChMStruct
 *
 * @brief       Get the local Channel Map classification structure
 *
 * @param       None.
 *
 * @return      const pointer to local Channel Map structure
 */
const csLocalChm_t* llCsDbGetLocalClassifiedChMStruct( void );

/*******************************************************************************
 * @fn          llCsDbEnableChannelMapChannelUpdate
 *
 * @brief       Enable the Channel Map classification update
 *              This is used when @ref LL_CS_SetChannelClassification is called.
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
void llCsDbEnableChannelMapChannelUpdate( void );

/*******************************************************************************
 * @fn          llCsDbFilterChannelMap
 *
 * @brief       Filter the Channel Map classification
 *
 * input parameters
 *
 * @param       pChm - const pointer to input channel map
 *
 * output parameters
 *
 * @param       pFilteredChm - pointer to filterd channel map
 *
 * @return      None
 *
 */
void llCsDbFilterChannelMap(const csChm_t* pChM, csChm_t* pFilteredChM);

/*******************************************************************************
 * @fn          llCsDbGetAciNumAnt
 *
 * @brief       Get number of antennas derived by ACI and role
 *
 * input parameters
 *
 * @param       ACI - Antenna Config Index
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      number of antennas paths
 */
uint8 llCsDbGetAciNumAnt(csACI_e ACI, uint8_t role);

/*******************************************************************************
 * @fn          llCsDbGetFactorialValue
 *
 * @brief       Get Factorial value of a number between 0 to 4.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      factorial value of the given number.
 *              0 if the given number is greater than 4.
 */
uint8 llCsDbGetFactorialValue(uint8 num);

/*******************************************************************************
 * @fn          llCsDbGetTip
 *
 * @brief       Get the T_IP time from tIpTbl
 *
 * @note        Inline function for zero-overhead timing index conversion
 *
 * input parameters
 *
 * @param       idx - Index of the T_IP
 *
 * output parameters
 *
 * @param       None
 *
 * @return      T_IP in microseconds
 */
__attribute__((always_inline)) inline uint16 llCsDbGetTip(uint8 idx)
{
    return tIpTbl[idx];
}

/*******************************************************************************
 * @fn          llCsDbGetTfcs
 *
 * @brief       Get T_FCS from tFcsTbl
 *
 * @note        Inline function for zero-overhead timing index conversion
 *
 * input parameters
 *
 * @param       idx - Index of F_CS value
 *
 * output parameters
 *
 * @param       None
 *
 * @return      T_FCS in microseconds
 */
__attribute__((always_inline)) inline uint16 llCsDbGetTfcs(uint8 idx)
{
    return tFcsTbl[idx];
}

/*******************************************************************************
 * @fn          llCsDbGetTpm
 *
 * @brief       Get T_PM from tPmTbl
 *
 * @note        Inline function for zero-overhead timing index conversion.
 *              Handles special case for stable phase test mode.
 *
 * input parameters
 *
 * @param       idx - Index of tPM value
 *
 * output parameters
 *
 * @param       None
 *
 * @return      T_PM - Phase Measurement Time in us
 */
__attribute__((always_inline)) inline uint16 llCsDbGetTpm(uint8 idx)
{
    uint16 tPm = 0;
    if (csTestOverrides.parsedParams.stablePhase == 1U)
    {
        /* Need to generate a mode-2 step with a 652us long tone.
           We will generate a 2x 326us by using active tone extension. */
        tPm = CS_STABLE_PHASE_TONE_DURATION_US;
    }
    else
    {
        tPm = tPmTbl[idx];
    }
    return tPm;
}

/*******************************************************************************
 * @fn          llCsDbInitChanIndexInfo
 *
 * @brief       Initialize the channel Index info struct.
 * There are two channel Index arrays, mode0 and nonMode0.
 * The arrays are initialized to the filtered channel map (chanArray)
 * Later it will be shuffled.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param      connId    - connection Id
 * @param      configId  - CS config Id
 * @param      numChan   - number of channels to allocate
 * @param      chanArray - filtered channel index array to use
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      status: SUCCESS or INFSUFFICIENT_MEMORY
 */
csStatus_e llCsDbInitChanIndexInfo(uint16 connId, uint8 configId, uint8 numChan,
                                 uint8* chanIdxArr);

/*******************************************************************************
 * @fn          llCsDbGetChanInfo
 *
 * @brief       Get the channel info of given connId, configId
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      pointer to channel info
 */
csChanInfo_t* llCsDbGetChanInfo(void);

/*******************************************************************************
 * @fn          llCsDbGetNumChan
 *
 * @brief       Get the number of channels for the given connId and configId.
 *
 * input parameters
 *
 * @param       connId - connection Handle
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      numChans
 */
uint8 llCsDbGetNumChan(void);

/*******************************************************************************
 * @fn          llCsDbSetRemainingMmSteps
 *
 * @brief       Set number of remaining main mode steps.
 * Number of main mode steps for the procedure is calculated when
 * The first subevent is setup. Some (if not all) the main mode
 * steps will be done in the subevent, however, the leftover steps
 * will be saved here.
 *
 * input parameters
 *
 * @param       connId - connection handle
 * @param       mmSteps - number of main mode steps
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetRemainingMmSteps(uint16 mmSteps);

/*******************************************************************************
 * @fn          llCsDbGetRemainingMmSteps
 *
 * @brief       Get the number of steps remaining for the procedure.
 *
 * input parameters
 *
 * @param       connId - connection handle
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
uint16 llCsDbGetRemainingMmSteps(void);

/*******************************************************************************
 * @fn          llCsDbIncrementRemainingMmSteps
 *
 * @brief       Increment the number of steps remaining for the procedure.
 *
 * input parameters
 *
 * @param       connId - connection handle
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbIncrementRemainingMmSteps(void);

/*******************************************************************************
 * @fn          llCsDbGetChMRepetition
 *
 * @brief       Get the number of Channel repetitions for the procedure.
 *
 * input parameters
 *
 * @param       connId - connection handle
 * @param       configId - cs config identifier
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
uint16 llCsDbGetChMRepetition(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbSubModeInsertionIsCompletedSet
 *
 * @brief       Set SubMode insertion completed indication
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       complete - true/false
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void llCsDbSubModeInsertionIsCompletedSet(bool complete);

/*******************************************************************************
 * @fn          llCsDbSubModeInsertionIsCompletedGet
 *
 * @brief       Get SubMode insertion completed indication
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      complete - true/false
 */
bool llCsDbSubModeInsertionIsCompletedGet(void);

/*******************************************************************************
 * @fn          llCsDbSubModeInsertionChannelIdxSet
 *
 * @brief       Get Channel Index for the subMode insertion
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       channelIdx - channel index
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSubModeInsertionChannelIdxSet(uint8_t channelIdx);

/*******************************************************************************
 * @fn          llCsDbSubModeInsertionChannelIdxGet
 *
 * @brief       Set Channel Index for the subMode insertion
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      channel index
 */
uint8_t llCsDbSubModeInsertionChannelIdxGet(void);

/*******************************************************************************
* @fn          llCsDbResetSubModeInsertionStepCount
*
* @brief       Reset the SubMode insertion step counter
*
* @details     Clears the counter that tracks SubMode insertions, typically
*              called at the beginning of a new subevent or when restarting
*              the SubMode insertion process
*
* @param       connId - Connection identifier
*
* @return      None
*/
void llCsDbResetSubModeInsertionStepCount(void);

/*******************************************************************************
* @fn          llCsDbResetSubModeInsertionInfo
*
* @brief       Reset all SubMode insertion tracking information
*
* @details     Completely clears all SubMode insertion state information,
*              including step counts, insertion indices, and completion status.
*              Typically called when starting a new procedure or when aborting
*              and restarting the SubMode insertion process.
*
* @param       connId - Connection identifier
*
* @return      None
*/
void llCsDbResetSubModeInsertionInfo(void);

/*******************************************************************************
 * @fn          llCsDbGetSubeventCount
 *
 * @brief       Get Subevent info from the DB
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       configId - cs config identifier
 * @param       type   - info type:
 *                       CS_SE_INFO_STEPS_NUM to get the num of steps
 *                       CS_SE_INFO_STEPS_C to get the step count
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      numSteps or stepCount per type
 */
uint16 llCsDbGetSubeventCount(csSubeventInfo_e type);

/*******************************************************************************
 * @fn          llCsDbSetSubeventCount
 *
 * @brief       Set Subevent info into the DB
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       type   - info type per csSubeventInfo_e
 * @param       count  - count of given type
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void llCsDbSetSubeventCount(csSubeventInfo_e type, uint16 count);

/*******************************************************************************
* @fn          llCsDbGetMModeRepetitionsChannelIndex
*
* @brief       Retrieve the channel index used in previous subevent's final Main Mode step
*
* @details     Returns the channel index that was used in the last Main Mode step of the
*              previous subevent. This channel will be reused for Main Mode Repetition
*              steps in the current subevent to maintain measurement continuity.
*
* @param       connId - Connection identifier
*
* @return      Channel index for Main Mode Repetition step
*/
uint8_t llCsDbGetMModeRepetitionsChannelIndex(void);

/*******************************************************************************
* @fn          llCsDbGetMModeRepetitionsCount
*
* @brief       Get the number of stored Main Mode channels from previous subevent
*
* @details     Returns the count of Main Mode channels that were stored during the
*              previous subevent's execution. These channels are available for
*              repetition in the current subevent.
*
* @param       connId - Connection identifier
*
* @return      Number of stored channel indices available for repetition
*/
uint8_t llCsDbGetMModeRepetitionsCount(void);

/*******************************************************************************
* @fn          llCsDbGetMModeRepetitionsIsFull
*
* @brief       Check if the Main Mode channel circular buffer is fully populated
*
* @details     Indicates whether the circular buffer storing Main Mode channel indices
*              has been completely filled. This status helps ensure channels are
*              retrieved in the same sequence they were originally transmitted.
*
* @param       connId - Connection identifier
*
* @return      true if all configured channel slots are filled, false otherwise
*/
bool llCsDbGetMModeRepetitionsIsFull(void);

/*******************************************************************************
* @fn          llCsDbIncrementMModeRepetitionsArrayChannelIndex
*
* @brief       Advance to the next position in the Main Mode channel storage buffer
*
* @details     Increments the index pointer in the circular buffer used to store
*              Main Mode channel indices for future repetitions. The index wraps
*              around when it reaches the maximum repetition count specified in
*              the configuration.
*
* @param       connId - Connection identifier
* @param       configId - Configuration identifier
*
* @return      None
*/
void llCsDbIncrementMModeRepetitionsArrayChannelIndex(uint16_t connId, uint8_t configId);

/*******************************************************************************
* @fn          llCsDbIncrementMModeRepetitionsCount
*
* @brief       Increment the count of stored Main Mode channels
*
* @details     Increases the counter tracking how many Main Mode channel indices
*              have been stored for future repetition. The counter is capped at
*              the maximum repetition count specified in the user configuration.
*
* @param       connId - Connection identifier
* @param       configId - Configuration identifier
*
* @return      None
*/
void llCsDbIncrementMModeRepetitionsCount(uint16_t connId, uint8_t configId);

/*******************************************************************************
* @fn          llCsDbSetSubModeInsertionNextIndex
*
* @brief       Set the index for the next SubMode insertion
*
* @details     Updates the position counter that determines how many MainMode steps
*              should be executed before the next SubMode step is inserted
*
* @param       connId - Connection identifier
* @param       nextIndex - Number of MainMode steps to perform before next SubMode insertion
*
* @return      None
*/
void llCsDbSetSubModeInsertionNextIndex(uint8_t nextIndex);

/*******************************************************************************
* @fn          llCsDbGetSubModeInsertionNextIndex
*
* @brief       Get the index for the next SubMode insertion
*
* @details     Returns the current position counter that determines how many MainMode
*              steps remain before the next SubMode insertion should occur
*
* @param       connId - Connection identifier
*
* @return      Number of MainMode steps before next SubMode insertion
*/
uint8_t llCsDbGetSubModeInsertionNextIndex(void);

/*******************************************************************************
* @fn          llCsDbIncrementSubModeInsertionStepCount
*
* @brief       Increment the SubMode insertion step counter
*
* @details     Increases the counter tracking how many SubMode insertions have been
*              performed in the current subevent
*
* @param       connId - Connection identifier
*
* @return      None
*/
void llCsDbIncrementSubModeInsertionStepCount(void);

/*******************************************************************************
* @fn          llCsDbGetSubModeInsertionStepCount
*
* @brief       Get the current SubMode insertion step count
*
* @details     Returns the number of SubMode steps that have been inserted
*              in the current subevent
*
* @param       connId - Connection identifier
*
* @return      Number of SubMode insertions performed in current subevent
*/
uint8_t llCsDbGetSubModeInsertionStepCount(void);

/*******************************************************************************
 * @fn          llCsDbIncrementSubeventCount
 *
 * @brief       Increment a subeventInfo counter by 1
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       type   - info type
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void llCsDbIncrementSubeventCount(csSubeventInfo_e type);

/*******************************************************************************
 * @fn          llCsDbResetSubeventCount
 *
 * @brief       Reset a subeventInfo counter
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       type   - info type
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void llCsDbResetSubeventCount(csSubeventInfo_e type);

/*******************************************************************************
 * @fn          llCsDbIncrementProcedureCount
 *
 * @brief       Increment procedure counter
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       counter - the type of procedure counter
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
uint16 llCsDbIncrementProcedureCount(csProcedureCounter_e counter);

/*******************************************************************************
 * @fn          llCsDbResetProcedureCount
 *
 * @brief       Reset Procedure Counter
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       counter - the type of procedure counter
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbResetProcedureCount(csProcedureCounter_e counter);

/*******************************************************************************
 * @fn          llCsDbGetProcedureCount
 *
 * @brief       Get procedure countern
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       counter - the type of procedure counter
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
uint16 llCsDbGetProcedureCount(csProcedureCounter_e counter);

/*******************************************************************************
 * @fn          llCsDbSetEventAnchorPoint
 *
 * @brief       Set event anchor point
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       anchorPoint - anchor point
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetEventAnchorPoint(uint32_t anchorPoint);

/*******************************************************************************
 * @fn          llCsDbGetEventAnchorPoint
 *
 * @brief       Get event anchor point
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
uint32_t llCsDbGetEventAnchorPoint(void);

/*******************************************************************************
* @fn          llCsDbGetProcedureRepetitionsCount
*
* @brief       Get the current count of procedure repetitions
*
* @details     Returns the number of times the current procedure has been repeated
*
* input parameters
*
* @param       connId - connection Id
*
* output parameters
*
* @param       None.
*
* @return      Current procedure repetitions count
*/
uint16 llCsDbGetProcedureRepetitionsCount(void);

/*******************************************************************************
* @fn          llCsDbIncrementProcedureRepetitionsCount
*
* @brief       Increment the procedure repetitions counter
*
* @details     Increases the counter tracking how many times the current
*              procedure has been repeated and returns the new value
*
* input parameters
*
* @param       connId - connection Id
*
* output parameters
*
* @param       None.
*
* @return      New procedure repetitions count after increment
*/
uint16 llCsDbIncrementProcedureRepetitionsCount(void);

/*******************************************************************************
* @fn          llCsDbResetProcedureRepetitionsCount
*
* @brief       Reset the procedure repetitions counter to zero
*
* @details     Clears the counter that tracks how many times a procedure
*              has been repeated
*
* input parameters
*
* @param       connId - connection Id
*
* output parameters
*
* @param       None.
*
* @return      None
*/
void llCsDbResetProcedureRepetitionsCount(uint16 connId);

/*******************************************************************************
* @fn          llCsDbSetProcedureRepetitionsPreviousProcedureStatus
*
* @brief       Store the status of the preceding CS procedure
*
* @details     Records whether the previous procedure was successfully synchronized.
*              This information is used to enforce the specification requirement
*              that repetitions must be terminated if two consecutive procedures
*              fail to synchronize.
*
* input parameters
*
* @param       connId - connection Id
* @param       status - 0 if previous procedure was synchronized, 1 otherwise
*
* output parameters
*
* @param       None.
*
* @return      None
*/
void llCsDbSetProcedureRepetitionsPreviousProcedureStatus(bool status);

/*******************************************************************************
* @fn          llCsDbGetProcedureRepetitionsPreviousProcedureStatus
*
* @brief       Get the synchronization status of the preceding CS procedure
*
* @details     Returns whether the previous procedure was successfully synchronized.
*              This information is used to enforce the specification requirement
*              that repetitions must be terminated if two consecutive procedures
*              fail to synchronize.
*
* input parameters
*
* @param       connId - connection Id
*
* output parameters
*
* @param       None.
*
* @return      0 if previous procedure was synchronized, 1 otherwise
*/
bool llCsDbGetProcedureRepetitionsPreviousProcedureStatus(void);

/*******************************************************************************
 * @fn          llCsDbInitDRBGCache
 *
 * @brief       Allocate and initialize the DRBG cache
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status
 */
csStatus_e llCsDbInitDRBGCache(void);

/*******************************************************************************
 * @fn          llCsDbResetDRBGCache
 *
 * @brief       Reset the DRBG cache
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      status
 */
csStatus_e llCsDbResetDRBGCache(void);

/*******************************************************************************
 * @fn          llCsDbFreeDRBGCache
 *
 * @brief       Free DRBG cache structs
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbFreeDRBGCache(void);

/*******************************************************************************
 * @fn          llCsDbSetRandomBitsCache
 *
 * @brief       Sets random bits to cache by transaction Id
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       transactionId - CS transaction Id
 * @param       pRandomBits - pointer to random bits
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetRandomBitsCache(uint8 transactionId, uint8* pRandomBits);

/*******************************************************************************
 * @fn          llCsDbRandomBitsAvailable
 *
 * @brief       Check if enough bits are available in cache.
 * This is to know if we need to generate more bits or we can use
 * the cashed buffer.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       transactionId - CS transaction Id
 * @param       numBitsRequired - number of required bits
 *
 * output parameters
 *
 * @param       pRandomBits
 *
 * @return      None
 */
uint8 llCsDbRandomBitsAvailable(uint8 transactionId, uint8 numBitsRequired);

/*******************************************************************************
 * @fn          llCsDbGetRandomBitsFromCache
 *
 * @brief       Gets random bits from cache by transaction Id
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       transactionId - CS transaction Id
 * @param       numBitsRequired - number of required bits
 * @param       pRandomBits - pointer to random bits
 * output parameters
 *
 * @param       pRandomBits
 *
 * @return      None
 */
void llCsDbGetRandomBitsFromCache(uint8 transactionId, uint8 numBitsRequired, uint8* pRandomBits);

/*******************************************************************************
 * @fn          llCsDbGetChannelIdxArray
 *
 * @brief       Get Channel Index Array from the DB
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       mode      - CS mode
 * @param       connId    - connection Id
 * @param       configId  - config Id
 * @param       chanData  - chan Data
 * @param       numChan   - numChan
 *
 * output parameters
 *
 * @param       chanData   - channel Index Array struct
 * @param       numChan    - num of channels in the array
 *
 * @return      none
 */
void llCsDbGetChannelIdxArray(uint8 mode, uint16 connId, uint8 configId,
                              modeSpecificChanInfo_t* chanData, uint8* numChan);

/*******************************************************************************
 * @fn          llCsDbGetChannelIndex
 *
 * @brief       Get Channel Index from the channel Index Array
 *              increment the channels used counter
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       configId - CS config Id
 * @param       mode - CS mode
 * output parameters
 *
 * @param       None
 *
 * @return      Channel Index
 */
uint8 llCsDbGetChannelIndex(uint16 connId, uint8 configId, uint8 mode);

/*******************************************************************************
 * @fn          llCsDbGetBits
 *
 * @brief       The function returns an integer containing the bits starting
 *              from the specified index.
 *
 * @design      BLE_LOKI-506
 * input parameters
 *
 * @param       num      - The number you want to extract bits from.
 * @param       numBits - The number of bits you want to extract.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      integer containing the bits starting from the specified index.
 */
uint8 llCsDbGetBits(uint8 num, uint8 startIdx, uint8 numBits);

/*******************************************************************************
 * @fn          llCsDbGetDefaultChMap
 *
 * @brief       Get the default Channel Map
 *
 * input parameters
 *
 * output parameters
 *
 * @return      const pointer to CS config
 */
const csChm_t* llCsDbGetDefaultChMap(void);

/*******************************************************************************
 * @fn          llCsDbSetTestMode
 *
 * @brief       Enable or Disable CS Test
 *
 * input parameters
 *
 * @param       mode - CS_TEST_MODE_ENABLE or CS_TEST_MODE_DISABLE
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetTestMode(csTestMode_e mode);

/*******************************************************************************
 * @fn          llCsDbGetTestMode
 *
 * @brief       Returns Whether CS Test Mode is enabled or disabled
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      CS_TEST_MODE_DISABLE if CS Test is disabled
 *              CS_TEST_MODE_ENABLE if CS Test is enabled
 */
csTestMode_e llCsDbGetTestMode(void);

/*******************************************************************************
 * @fn          llCsDbSetSwitchTime
 *
 * @brief       Set an antenna switching time value for a specific connection
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       tSw    - Antenna switch time in us
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetSwitchTime(uint8 tSw);

/*******************************************************************************
 * @fn          llCsDbGetSwitchTime
 *
 * @brief       Get antenna switching time value of a specific connection
 *
 * input parameters
 *
 * @param       connId - Connection Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Antenna Switch Time in us
 */
uint16 llCsDbGetSwitchTime(void);

/*******************************************************************************
 * @fn          llCsDbSetTestConfig
 *
 * @brief       Set the CS Config based on the test parameters.
 *
 * input parameters
 *
 * @param       pTestParams - Pointer to Test Parameter
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetTestConfig(csTestParams_t* pTestParams);

/*******************************************************************************
 * @fn          llCsDbSetTestDefaultSettings
 *
 * @brief       Set CS Default Settings based on test parameters
 *
 * input parameters
 *
 * @param       pTestParams - pointer to test parameters
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetTestDefaultSettings(csTestParams_t* pTestParams);

/*******************************************************************************
 * @fn          llCsDbSetTestProcedureEnable
 *
 * @brief       Set Procedure Enable data based on Test Parameters
 *
 * input parameters
 *
 * @param       pTestParams - Pointer to Test Parameters
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetTestProcedureEnable(csTestParams_t* pTestParams);

/*******************************************************************************
 * @fn          llCsDbSetTestProcedureParams
 *
 * @brief       Set Test Parameters in the DB
 *
 * input parameters
 *
 * @param       pTestParams - pointer to test params
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetTestProcedureParams(csTestParams_t* pTestParams);

/*******************************************************************************
 * @fn          llCsDbClearTestParams
 *
 * @brief       Clear Test Parameters in the DB
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbClearTestParams(void);

/*******************************************************************************
 * @fn          llCsDbSetTestOverrideData
 *
 * @brief       Set the CS Test Override Data in CS DB
 * This function will take the overrides data from the test parameters and
 * store it in the override data in the DB.
 * It also check the validity of the parameters.
 *
 * input parameters
 *
 * @param       pTestParams - pointer to test parameters
 *
 * output parameters
 *
 * @param       None
 *
 * @return      CS_STATUS_UNEXPECTED_PARAMETER if parameter value is not expected
 *              CS_STATUS_SUCCESS otherwise
 */
csStatus_e llCsDbSetTestOverrideData(csTestParams_t* pTestParams);

/*******************************************************************************
 * @fn          llCsDbGetTestOverrideData
 *
 * @brief       Returns the CS test overrides data
 *
 * input parameters
 *
 *
 * output parameters
 *
 * @param       pCsTestOverrideData
 *
 * @return      None
 */
const csTestOverrideData_t* llCsDbGetTestOverrideData( void );

/*******************************************************************************
 * @fn          llCsDbGetChanOverrideCfg
 *
 * @brief       Returns the bit for channel config in overrideConfig
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Channel Config Bit
 */
uint8 llCsDbGetChanOverrideCfg(void);

/*******************************************************************************
 * @fn          llCsDbGetOverrideCfg
 *
 * @brief       Get the CS Override Config
 * This function checks if CS Test Mode is used, returns TRUE and copies the
 * content of the test override config into the provided pointer
 *
 * input parameters
 *
 * @param       pOverrideConfig - Pointer to override config
 *
 * output parameters
 *
 * @param       pOverrideCfg - Pointer to Override Config
 *
 * @return      TRUE - if CS Test Mode is enabled
 *              FALSE - if CS Test Mode is disabled
 */
bool llCsDbGetOverrideCfg(csOverrideCfg_t* pOverrideCfg);

/*******************************************************************************
 * @fn          llCsDbOverrideChanIndexArr
 *
 * @brief       Override the shuffled channel index array
 * This function is to be used when the override config bit 0 is enabled.
 * It is assumed that the filtered channel map is initialized, namely, the
 * the function llCsDbInitChanIndexInfo was used.
 * This function will free the shuflled arrays since they will be overriden.
 * Instead, they will both point to filteredChanIndex->filteredChanArr which
 * shall contain the override channels.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbOverrideChanIndexArr(void);

/*******************************************************************************
 * @fn          llCsDbGetOverrideToneExt
 *
 * @brief       Get Override Tone Extension
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Override Tone Extension
 */
uint8 llCsDbGetOverrideToneExt(void);

/*******************************************************************************
 * @fn          llCsDbSetNextOverrideToneExt
 *
 * @brief       Set Next Override Tone Extension
 * This function should only be used when the inital override tone extension is
 * 0x04. In this case since the Tone Extension value shall loop over 0-3.
 * This function shall help achieve this by storing the value:
 *     (usedToneExtension+1) | 0x04
 * When the value is retrieved, the MSB is dropped giving the values 0,1,2,3.
 *
 * input parameters
 *
 * @param       toneExt - used tone extension
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetNextOverrideToneExt(uint8 toneExt);

/*******************************************************************************
 * @fn          llCsDbGetAAOverride
 *
 * @brief       Get the Access Address Override
 *
 * input parameters
 *
 * @param       initTxAA - pointer to initiator Tx AA
 * @param       refTxAA - pointer to reflector Tx AA
 *
 * output parameters
 *
 * @param       initTxAA - pointer to initiator Tx AA
 * @param       refTxAA - pointer to reflector Tx AA
 *
 * @return      None
 */
void llCsDbGetAAOverride(uint32_t* initTxAA, uint32_t* refTxAA);

/*******************************************************************************
 * @fn          llCsDbGetPayloadOverride
 *
 * @brief       Get the Payload Override
 *
 * input parameters
 *
 * @param       pPlTx - pointer to Tx Payload
 * @param       pPlRx - pointer to Rx Payload
 *
 * output parameters
 *
 * @param       pPlTx - pointer to Tx Payload
 * @param       pPlRx - pointer to Rx Payload
 *
 * @return      payload Pattern
 */
uint8 llCsDbGetPayloadOverride(uint32_t* pPlTx, uint32_t* pPlRx);

/*******************************************************************************
 * @fn          llCsDbGetNextOverrideAntennaPermutation
 *
 * @brief       Get Next Antenna Permutation Override value.
 *              Used for CS_TEST Antenna Permutation index.
 *              If loop is configured:
 *                  1. The next index will be returned.
 *                  2. Updates the index in the DB (override parameters).
 *              Otherwise: this function will return a constant index.
 *
 * input parameters
 *
 * @param       ACI - ACI index to be used to determine the next
 *                    permutation index.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      The next Antenna Permutation index value.
 */
uint8 llCsDbGetNextOverrideAntennaPermutation(csACI_e ACI);

/*******************************************************************************
 * @fn          llCsGetNextAntennaPermutationWrapper
 *
 * @brief       Get next Antenna Permutation Index wrapper.
 *              Disigned to Distinguish between cases we use CS_TEST or not.
 *
 *              When not using CS_TEST, this function won't be called.
 *
 *              When using CS_TEST, this function will check if override
 *              configuration has been set for Antenna Permutation Index
 *              parameter, and use the override data if needed.
 *              If override is not set, this function will call
 *              @ref llCsGetNextAntennaPermutation function.
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       ACI - ACI index to be used to determine the next
 *                    permutation index.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      The next Antenna Permutation index value.
 */
uint8_t llCsGetNextAntennaPermutationWrapper(uint16 connId, csACI_e ACI);

/*******************************************************************************
 * @fn          llCsDbSetGpioVals
 *
 * @brief       Set antennas GPIO values for the current procedure in the DB
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       gpioValsToSet - pointer to GPIO values to set.
 *                              Assumed size @ref CS_ANTENNAS_GPIOS_ARRAY_SIZE
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetGpioVals(uint8_t gpioValsToSet[CS_ANTENNAS_GPIOS_ARRAY_SIZE]);

/*******************************************************************************
 * @fn          llCsDbGetGpioVals
 *
 * @brief       Get antennas GPIO values array for the current procedure
 *
 * input parameters
 *
 * @param       connId - Connection Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Pointer to GPIO values array
 */
const uint8_t* llCsDbGetGpioVals(void);

/*******************************************************************************
 * @fn          llCsDbSetPhysicalAntToRCLMapping
 *
 * @brief       Sets antennas indices mapping into CS DB.
 *              This maps an antenna index to its actual index in RCL command.
 *
 * input parameters
 *
 * @param       connId - connection Id to be used.
 * @param       mappingToSet - antennas indices mapping to set.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetPhysicalAntToRCLMapping(uint8_t mappingToSet);

/*******************************************************************************
 * @fn          llCsDbGetPhysicalAntToRCLMapping
 *
 * @brief       Gets antennas indices mapping from procedureInfo DB.
 *              This maps an antenna index to its actual index in RCL command.
 *
 * input parameters
 *
 * @param       connId - connection Id to be used.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      The antenna index mapping.
 */
uint8_t llCsDbGetPhysicalAntToRCLMapping(void);

/*******************************************************************************
 * @fn          llCsDbSetRCLAntToPhysicalMapping
 *
 * @brief       Sets RCL antennas indices mapping into CS DB.
 *              This maps an antenna index set in RCL command to its physical index.
 *
 * input parameters
 *
 * @param       connId - connection Id to be used.
 * @param       mappingToSet - antennas indices mapping to set.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetRCLAntToPhysicalMapping(uint8_t mappingToSet);

/*******************************************************************************
 * @fn          llCsDbGetRCLAntToPhysicalMapping
 *
 * @brief       Gets RCL antennas indices mapping from procedureInfo DB.
 *              This maps an antenna index set in RCL command to its physical index.
 *
 * input parameters
 *
 * @param       connId - connection Id to be used.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      The antenna index mapping.
 */
uint8_t llCsDbGetRCLAntToPhysicalMapping(void);

/*******************************************************************************
 * @fn          llCsDbSetProcedureRepetitionsInfo
 * @brief       Set the Procedure Info for a specific connection.
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 * @param       pProcedureInfo - pointer to the procedure info
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetProcedureRepetitionsInfo(const csProcRepetitions_t *pProcedureRepetitionsInfo);

/*******************************************************************************
 * @fn          llCsDbGetProcedureRepetitionsInfo
 *
 * @brief       Returns a const pointer to the CS procedure repetitions info.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Pointer to the stored repetitions info (never NULL after setup)
 */
const csProcRepetitions_t *llCsDbGetProcedureRepetitionsInfo(void);

/*******************************************************************************
 * @fn          llCsDbGetSubModeInsertionRange
 * @brief       Get the 'main mode number of steps' minimum and maximum values
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 * @param       configId  - configuration Id to update
 *
 * output parameters
 *
 * @param       pNumStepsMin  - minimum number of steps
 * @param       pNumStepsMax  - maximum number of steps
 *
 * @return      None
 */
void llCsDbGetSubModeInsertionRange(uint16 connId, uint8 configId, uint8 *pNumStepsMin, uint8 *pNumStepsMax);

/*******************************************************************************
 * @fn          llCsDbGetMainModeRepetition
 * @brief       Get the main mode repetition number, as configured in the config procedure
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 * @param       configId  - configuration Id to update
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Number of main mode repetitions
 */
uint8_t llCsDbGetMainModeRepetition(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbGetMainModeRepetition
 * @brief       Get the main mode repetition number, as configured in the config procedure
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 * @param       configId  - configuration Id to update
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Number of main mode repetitions
 */
uint8_t llCsDbGetMainModeRepetition(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbDrbgGetTransactionId
 *
 * @brief       Get the Transactrion Id
 *
 * input parameters
 *
 * @param       connId   - connId
 *
 *
 * output parameters
 *
 * @param       transactionId   - transaction Id
 *
 * @return      None
 */
uint16 llCsDbDrbgGetTransactionId( uint16 connId );

/*******************************************************************************
 * @fn          llCsDbDrbgGetStepCount
 *
 * @brief       Get DRBG param step count
 *
 * input parameters
 *
 * @param       connId   - connId
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      DRBG num steps
 */
uint16 llCsDbDrbgGetStepCount( uint16 connId );

/*******************************************************************************
 * @fn          llCsDbDrbgGetLastGenConnEventCount
 *
 * @brief       Get the last conn event count that a steps were successfully generated
 *
 * input parameters
 *
 * @param       connId   - connId
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      connection event counter
 */
uint16 llCsDbDrbgGetLastGenConnEventCount( uint16 connId );

/*******************************************************************************
 * @fn          llCsDbDrbgSetLastGenConnEventCount
 *
 * @brief       Set the last conn event count that a steps were successfully generated
 *
 * input parameters
 *
 * @param       connId   - connId
 * @param       connEvent - connection event counter to set
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbDrbgSetLastGenConnEventCount(uint16 connId, uint16 connEvent);

/*******************************************************************************
 * @fn          llCsDbDrbgGetProcedureCount
 *
 * @brief       Get DRBG param procedure count
 *
 * input parameters
 *
 * @param       connId   - connId
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      DRBG num procedures
 */
uint16 llCsDbDrbgGetProcedureCount( uint16 connId );

/*******************************************************************************
 * @fn          llCsDbDrbgIncrementProcedureCount
 *
 * @brief       Increment DRBG param procedure count
 *
 * input parameters
 *
 * @param       connId   - connId
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbDrbgIncrementProcedureCount( uint16 connId );

/*******************************************************************************
 * @fn          llCsDbDrbgSetProcedureCount
 *
 * @brief       Set DRBG param procedure count
 * This API shall be used only in case of CS Procedure Termination when CS
 * Procedure Count in the CS_TERMINATE_RSP is larger than that of the
 * CS_TERMINATE_REQ. In these case we must use the larger value to keep the
 * Link Layers sychronized.
 *
 * input parameters
 *
 * @param       connId   - connId
 * @param       procCnt - Procedure Count
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbDrbgSetProcedureCount(uint16 connId, uint16 procCnt);

/*******************************************************************************
 * @fn          llCsSecIncProcCounter
 *
 * @brief       Increment CS procedure counter
 *
 * input parameters
 *
 * @param       connId   - connId
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSecIncProcCounter( uint16 connId );

/*******************************************************************************
 * @fn          llCsDbDrbgIncreaseStepCount
 *
 * @brief       Increase the step counter
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId   - connId
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbDrbgIncreaseStepCount( uint16 connId );

/*******************************************************************************
 * @fn          llCsDbDrbgSetTransactionCount
 *
 * @brief       Set the Transactrion Count
 *
 * input parameters
 *
 * @param       connId   - connId
 * @param       transactionCounter   - transaction Counter
 *
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbDrbgSetTransactionCount( uint16 connId, uint8 transactionCounter );

/*******************************************************************************
 * @fn          llCsDbDrbgSetTransactionCount
 *
 * @brief       Increment the Transactrion Count
 *
 * input parameters
 *
 * @param       connId   - connId
 *
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbDrbgIncrementTransactionCount( uint16 connId );

/*******************************************************************************
 * @fn          llCsDbDrbgSetTransactionId
 *
 * @brief       Set the Transactrion Id
 *
 * input parameters
 *
 * @param       connId   - connId
 * @param       transactionId   - transaction Id
 *
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbDrbgSetTransactionId( uint16 connId, uint8 transactionId );

/*******************************************************************************
 * @fn          llCsDbSetRepeatProcedureConnEvent
 * @brief       Set the connection event counter the next procedure should
 *              start when using procedure repetitions
 *
 * input parameters
 *
 * @param       connId       - connection Id.
 * @param       connEvtCount - connection event count for the repeat procedure
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetRepeatProcedureConnEvent(uint16 connEvtCount);

/*******************************************************************************
 * @fn          llCsDbGetRepeatProcedureConnEvent
 * @brief       Return the connection event counter of the current connection
 *              event the procedure should start when using procedure repetitions
 *
 * input parameters
 *
 * @param       connId    - connection Id.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Connection event count for the repeat procedure
 */
uint16_t llCsDbGetRepeatProcedureConnEvent(void);

/*******************************************************************************
 * @fn          llCsDbSetFirstProcedureAfterSecurity
 *
 * @brief       Set the first CS procedure after security procedure flag
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 * @param       enable    - true to enable the first procedure after security flag,
 *                          false to disable it
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetFirstProcedureAfterSecurity(uint16 connId, bool enable);

/*******************************************************************************
 * @fn          llCsDbGetFirstProcedureAfterSecurity
 *
 * @brief       Get the first CS procedure after security procedure flag
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      true if this flag is enabled, false otherwise
 */
bool llCsDbGetFirstProcedureAfterSecurity(uint16_t connId);

/*******************************************************************************
 * @fn          llCsDbSetMissedProcedureFlag
 *
 * @brief       Set the missed procedure flag
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 * @param       enable    - true to enable the missed procedure flag,
 *                          false to disable it
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetValidProcedureFlag(bool valid);

/*******************************************************************************
 * @fn          llCsDbGetMissedProcedureFlag
 *
 * @brief       Get the missed procedure flag
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      true if this flag is enabled, false otherwise
 */
bool llCsDbGetValidProcedureFlag(void);

/*******************************************************************************
 * @fn          llCsDbSetCsReqIntiatedByPeerFlag
 *
 * @brief       Set peer intiated CS Req flag
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 * @param       enable    - true to enable the peer intiated the CS req,
 *                          false to disable it
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetCsReqIntiatedByPeerFlag(bool enable);

/*******************************************************************************
 * @fn          llCsDbGetCsReqIntiatedByPeerFlag
 *
 * @brief       Get the peer intiated a CS Req flag
 *
 * input parameters
 *
 * @param       connId    - connection Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      true if this flag is enabled, false otherwise
 */
bool llCsDbGetCsReqIntiatedByPeerFlag(void);

/*******************************************************************************
 * @fn          llCsDbSetTerminatePeerProcCount
 *
 * @brief       Set the terminate peer procedure count
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 * @param       peerCount - Peer procedure count
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetTerminatePeerProcCount(uint16_t peerCount);

/*******************************************************************************
 * @fn          llCsDbGetTerminatePeerProcCount
 *
 * @brief       Get the terminate peer procedure count
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Peer procedure counter
 */
uint16_t llCsDbGetTerminatePeerProcCount(void);

/*******************************************************************************
 * @fn          llCsDbGetModeZeroSteps
 * @brief       Get the number of Mode0 steps for this procedure
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 * @param       configId  - configuration Id to update
 *
 * output parameters
 *
 * @param       None
 *
 * @return      The number of Mode0 steps for this procedure
 */
uint8_t llCsDbGetModeZeroSteps(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbGetMainMode
 * @brief       Get the Main Mode type for this procedure
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 * @param       configId  - configuration Id to update
 *
 * output parameters
 *
 * @param       None
 *
 * @return      The Main Mode type for this procedure
 */
uint8_t llCsDbGetMainMode(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbGetSubMode
 * @brief       Get the Sub Mode type for this procedure
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 * @param       configId  - configuration Id to update
 *
 * output parameters
 *
 * @param       None
 *
 * @return      The Sub Mode type for this procedure
 */
uint8_t llCsDbGetSubMode(uint16 connId, uint8 configId);


/*******************************************************************************
 * @fn          llCsDbGetRttType
 * @brief       Get the RTT type for this procedure
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 * @param       configId  - configuration Id to update
 *
 * output parameters
 *
 * @param       None
 *
 * @return      The RTT type for this procedure
 */
uint8_t llCsDbGetRttType(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbSetMModeRepetitionsChannelArray
 * @brief       This function saves the channels for M-Mode repetitions
 *              measurements for a given connection. M-Mode repetitions
 *              allow for multiple consecutive measurements to improve accuracy.
 *
 * input parameters
 *
 * @param       connId        - connection Id
 * @param       index         - Index in the DB for the channelInd to be saved in
 * @param       channelIdx    - The channelIdx to be saved
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetMModeRepetitionsChannelArray(uint8_t index, uint8_t channelIdx);

/*******************************************************************************
 * @fn          llCsDbGetMModeRepetitionsChannelArray
 * @brief       This function gets the channels for M-Mode repetitions
 *              measurements for a given connection. M-Mode repetitions
 *              allow for multiple consecutive measurements to improve accuracy.
 *
 * input parameters
 *
 * @param       connId    - connection Id
 * @param       index     - Index in the DB for the channelInd to be retreive from
 *
 * output parameters
 *
 * @param       None
 *
 * @return      The retreived channelIdx
 *
 */
uint8_t llCsDbGetMModeRepetitionsChannelArray(uint8_t index);

/*******************************************************************************
 * @fn          llCsDbClearMModeRepetitions
 * @brief       This function clears the channels for M-Mode repetitions
 *              measurements for a given connection. M-Mode repetitions
 *              allow for multiple consecutive measurements to improve accuracy.
 *
 * input parameters
 *
 * @param       connId    - connection Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 *
 */
void llCsDbClearMModeRepetitions(void);

/*******************************************************************************
 * @fn          llCsDbResetLocalClassifiedChM
 * @brief       Reset the local classified channel map to system initial values.
 * This includes freeing the allocated memory for the Channel Map.
 * Freeing is necessary because the functionality relies on NULL checks to determine
 * when and when not to use the local classified channel map.
 *
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 *
 */
void llCsDbResetLocalClassifiedChM(void);

/*******************************************************************************
 * @fn          llCsDbReset
 * @brief       Reset the CS DB to system initial values.
 *
 *
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 *
 */
void llCsDbReset(void);

/*******************************************************************************
 * @fn          llCsDbGetProcedureRepetitionsDoneStatus
 *
 * @brief       Get the completion status of CS procedure repetitions
 *
 * @details     Returns the current status code that indicates whether the
 *              procedure repetitions sequence is complete and if any errors
 *              were encountered during execution.
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Procedure repetitions completion status code
 */
uint8 llCsDbGetProcedureRepetitionsDoneStatus(void);

/*******************************************************************************
 * @fn          llCsDbGetProcedureRepetitionsDoneReason
 *
 * @brief       Get the completion reason of CS procedure repetitions
 *
 * @details     Returns the code that indicates the reason for procedure
 *              repetitions termination.
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Procedure repetitions completion status code
 */
uint8 llCsDbGetProcedureRepetitionsDoneReason(void);

/*******************************************************************************
 * @fn          llCsDbSetProcedureRepetitionsDoneStatus
 *
 * @brief       Set the completion status of CS procedure repetitions
 *
 * @details     Updates the status code that tracks completion of the procedure
 *              repetitions sequence, including any error conditions that may
 *              have occurred.
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       reason - procedure completion status or error code
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetProcedureRepetitionsDoneAborted(uint8 reason);

/*******************************************************************************
 * @fn          llCsDbProcedureReset
 *
 * @brief       Clear the OTA-negotiated procedure parameters (enableProcCtrlData)
 *
 * @details     Resets csEnableProcedureCtrlData_t so that stale procedureCount,
 *              procedureInterval, and connEventCount values cannot affect the
 *              next procedure's negotiation or scheduling.  Must be called as
 *              part of procedure cleanup (llCs_finishAndResetProcedure).
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
void llCsDbProcedureReset(void);

/*******************************************************************************
 * @fn          llCsDbSetEventStartedFlag
 *
 * @brief       Marks the status of the event
 *                - TRUE if properly started
 *                - FALSE otherwise
 * @details
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       eventStatus - event Status
 *                             - TRUE if properly started
 *                             - FALSE otherwise
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetEventStartedFlag(bool eventStatus);

/*******************************************************************************
 * @fn          llCsDbGetEventStartedFlag
 *
 * @brief       Retrieves the status of the event
 *                - TRUE if properly started
 *                - FALSE otherwise
 *
 * @details
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      eventStatus - event Status
 *                             - TRUE if properly started
 *                             - FALSE otherwise
 */
bool llCsDbGetEventStartedFlag(void);

/*******************************************************************************
 * @fn          llCsDbSetProcedureCounterIncrementedFlag
 *
 * @brief        Marks the indication whether the Procedure Counter was incremented for this procedure
 *                - TRUE if was incremented
 *                - FALSE otherwise
 * @details
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       eventStatus - event Status
 *                             - TRUE if properly started
 *                             - FALSE otherwise
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetProcedureCounterIncrementedFlag(bool valid);

/*******************************************************************************
 * @fn          llCsDbGetProcedureCounterIncrementedFlag
 *
 * @brief       Retrieves the indication whether the Procedure Counter was incremented for this procedure
 *                - TRUE was incremented
 *                - FALSE otherwise
 *
 * @details
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      eventStatus - event Status
 *                             - TRUE if was incremented
 *                             - FALSE otherwise
 */
bool llCsDbGetProcedureCounterIncrementedFlag(void);

/*******************************************************************************
 * @fn          llCsDbGetActiveConnId
 *
 * @brief      Get active connection ID
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       none
 *
 * output parameters
 *
 * @param       ConnId.
 *
 * @return      None
 */
uint16 llCsDbGetActiveConnId(void);

/*******************************************************************************
* @fn          llCsProcSetActiveConnId
*
* @brief       Set active connection ID
*
* @details     Stores the connection ID for the currently active CS procedure.
*              This parameter should be used instead of llConns.currentConn
*              as in case of multiple connections, another connection might be
*              scheduled between the CS events and results processing time, which
*              would lead to an irrelevant connId when accessing the DB and
*              reporting this invalid connId to the application.
*
* input parameters
*
* @param       connId - connection Id
*
* output parameters
*
* @param       None.
*
* @return      None
*/
void llCsProcSetActiveConnId(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbGetTxPowerDelta
 *
 * @brief       Get the Tx Power Delta for a given configuration.
 *              If the host has set this value to @ref CS_TX_PWR_DELTA_NO_RECOMMENDATION,
 *              or not provided a value at all, this function will return 0.
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used
 * @param       configId  - configuration Id to update
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Tx Power Delta value for the given configuration.
 *              0 if no recommendation or configuration provided by the host.
 */
int8_t llCsDbGetTxPowerDelta(uint16_t connId, uint8_t configId);

#endif //LL_CS_DB_H
