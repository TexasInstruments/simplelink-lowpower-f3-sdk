/******************************************************************************

 @file  ll_cs_db.h

 @brief This file contains the Data Structures and APIs for the Channel Souding
        Feature in the Bluetooth Low Energy (BLE) Controller.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2025, Texas Instruments Incorporated

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

/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/controller/ll/ll_cs_common.h"
#include "ti/ble/controller/ll/ll_cs_test.h"

/*******************************************************************************
 * EXTERNS
 */

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
 * @fn          llCsDbInitDefaultParams
 *
 * @brief       Set Default Parameters at CS initialization
 *
 *
 * input parameters
 *
 * @param       connId - connection identifier.
 * @param       configId - Config Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbInitDefaultParams(uint16_t connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsDbClearProcedureData
 *
 * @brief       Clear CS procedure data
 * Used when CS is initialized and when a procedure ends and is not to be
 * repeated.
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       configId - Config Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbClearProcedureData(uint16 connId, uint8 configId);

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
 * @fn          llCsDbMarkProcedureCompleted
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
void llCsDbMarkProcedureCompleted(uint16 connId, uint8 csProcedureId);

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
 * @return      TRUE if the proecure is completed, FALSE otherwise.
 */
uint8 llCsDbIsProcedureCompleted(uint16 connId, uint8 procedureId);

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
 * @fn          llCsDbResetProcedureCompletedFlag
 *
 * @brief       Reset Procedure COmpleted Flag
 * This function clears the Complete Procedure flag of CS procedures that can
 * be run more than once.
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
void llCsDbResetProcedureCompletedFlag(uint16 connId, uint8 procedureId);

/*******************************************************************************
 * @fn          llCsDbSetActiveProcedure
 *
 * @brief       Set Active CS Procedure.
 * Ensure no other CS procedure is already active
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
 * @return      CS_STATUS_SUCCESS is procedure is activated successfully
 *              CS_STATUS_PROCEDURE_IN_PROGRESS cannot activate this procedure
 *              since another procedure is progress
 */
uint8 llCsDbSetActiveProcedure(uint16 connId, uint8 procedureId);

/*******************************************************************************
 * @fn          llCsDbSetInActiveProcedure
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
void llCsDbSetInActiveProcedure(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbGetActiveProcedure
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
uint8 llCsDbGetActiveProcedure(uint16 connId);

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
uint8 llCsDbIsCsCtrlProcedureInProgress(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbSetSecurityVectors
 *
 * @brief       Sets Security Vectors in CS Database.
 * This API is used by the CS Security Module after it generates the CS
 * Security Vectors. The security vectors come in two parts, a central and a
 * peripheral. When both parts of the vectors are available locally after the
 * security procedure is complete, the vectors are combined such
 * that:
 *     The central's part starts from: 0
 *     central part starts from: VECTOR_SIZE/2
 *
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       pSecurityVectors - Pointer to Security Vectors.
 * @param       offset - the offset to save the security vectors
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetSecurityVectors(uint16 connId, const csSecVectors_t* pSecurityVectors,
                              uint8 offset);

/*******************************************************************************
 * @fn          llCsDbGetSecurityVectors
 *
 * @brief       Gets Security Vectors in CS Database.
 * The CS DRBG uses this API to get the security vectors and use them.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       pSecurityVectors - Pointer to Security Vectors
 *
 * output parameters
 *
 * @param       pSecurityVectors
 *
 * @return      None
 */
void llCsDbGetSecurityVectors(uint16 connId, csSecVectors_t* pSecurityVectors);

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
void llCsDbGetLocalCapabilities(llCsCapabilities_t* pLocalCapabilities);

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
 * @param       pRemoteCapabilities - pointer to remote Capabilities.
 * output parameters
 *
 * @param       pLocalCapabilities
 *
 * @return      None
 */
void llCsDbGetPeerCapabilities(uint16 connId,
                               llCsCapabilities_t* pRemoteCapabilities);

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
 * @fn          llCsDbSetConfiguration
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
csStatus_e llCsDbSetConfiguration(uint16 connId, const csConfigurationSet_t* pConfig);

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
 * @brief       Set current configuration.
 * This is needed because there is an array of configurations
 * And when setting up we may lose the current index
 * This is used to store it
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       configId - CS configuration Id
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetCurrentConfigId(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbRemobeCurrentConfigId
 *
 * @brief       Remove current config ID
 *
 * input parameters
 *
 * @param       connId - connection ID
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbRemoveCurrentConfigId(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbGetCurrentConfigId
 *
 * @brief       Get current configuration.
 * This is needed because there is an array of configurations
 * And when setting up we may lose the current index
 * This is used to get it
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * output parameters
 *
 * @param       None
 *
 * @return      current config Id
 */
uint8 llCsDbGetCurrentConfigId(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbSetProcedureParams
 *
 * @brief       Sets CS Procedure Parameters by configuration Id
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       configId - CS configuration Id
 * @param       pProcParams - pointer to procedure parameters
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetProcedureParams(uint16 connId, uint8 configId,
                              const csProcedureParams_t* pProcParams);

/*******************************************************************************
 * @fn          llCsDbGetProcedureParamStatus
 *
 * @brief       Get Procedure Params Status
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       configId - CS configuration Id
 * output parameters
 *
 * @param       None
 *
 * @return      Success - if procedure params were previously set
 *              Failure - if procedure params were not set
 */
bool llCsDbGetProcedureParamStatus(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbSetProcedureParamStatus
 *
 * @brief       Set Procedure Parameters status
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       configId - CS configuration Id
 * @param       status - procedure parameters status
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetProcedureParamStatus(uint16 connId, uint8 configId, bool status);

/*******************************************************************************
 * @fn          llCsDbGetProcedureParams
 *
 * @brief       Gets CS Procedure Parameters by configuration Id
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       configId - CS configuration Id
 * @param       pProcParams - pointer to procedure parameters
 * output parameters
 *
 * @param       pProcParams
 *
 * @return      None
 */
void llCsDbGetProcedureParams(uint16 connId, uint8 configId,
                              csProcedureParams_t* pProcParams);

/*******************************************************************************
 * @fn          llCsDbEnableProcedureParams
 *
 * @brief       Enables or disables the procedure parameter set
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId   - connection Id
 * @param       configId - CS configuration Id
 * @param       enable   - enable or disable procedure params
 * output parameters
 *
 * @param       pProcParams
 *
 * @return      None
 */
void llCsDbEnableProcedureParams(uint16 connId, uint8 configId, uint8 enable);

/*******************************************************************************
 * @fn          llCsDbIsProcedureEnabled
 *
 * @brief       Is CS procedure enabled
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
 * @return      None
 */
uint8 llCsDbIsProcedureEnabled(uint16 connId, uint8 configId);

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
uint8 llCsDbIsCsProcedureInProgress(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbGetProcedureTerminateState
 *
 * @brief       Get current terminateState.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId   - connection Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      terminateState
 */
csTerminateState_e llCsDbGetProcedureTerminateState(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbSetProcedureTerminateState
 *
 * @brief       Set terminateState parameter
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId         - connection Id
 * @param       terminateState - indicate if the procedure state should be
 *                    updated to CS_DISABLE. The following values are optional:
 *                    CS_TERMINATE_RECEIVED and CS_TERMINATE_DISABLE
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetProcedureTerminateState(uint16 connId, csTerminateState_e terminateState);

/*******************************************************************************
 * @fn          llCsDbGetTerminateReason
 *
 * @brief       Get the CS Terminate Reason
 *
 * input parameters
 *
 * @param       connId - Connection ID
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Error Code (Termination Reason)
 */
uint8 llCsDbGetTerminateReason(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbSetTerminateReason
 *
 * @brief       Set the CS Termination Reason
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       errorCode - The error code for the temrination reason
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetTerminateReason(uint16 connId, uint8 errorCode);

/*******************************************************************************
 * @fn          llCsDbSetEnableProcedureDuration
 *
 * @brief       Set the procedure enable duration parameter
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId   - connection Id
 * @param       configId - CS configuration Id
 * @param       duration - Procedure duration
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetEnableProcedureDuration(uint16 connId, uint8 configId,
                                      uint16 duration);

/*******************************************************************************
 * @fn          llCsDbSetEnableProcedureCount
 *
 * @brief       Set the procedure enable procedure count parameter
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId    - connection Id
 * @param       configId  - CS configuration Id
 * @param       count     - The number of the procedure repetitions
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetEnableProcedureCount(uint16 connId, uint8 configId, uint16 count);

/*******************************************************************************
 * @fn          llCsDbSetEnableProcedureInterval
 *
 * @brief       Set the procedure enable procedure event interval parameter
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId            - connection Id
 * @param       configId          - CS configuration Id
 * @param       procedureInterval - The number of connection interval between
 *                                  two consecutive procedures
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetEnableProcedureInterval(uint16 connId, uint8 configId,
                                      uint16 procedureInterval);

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
void llCsDbSetProcedureDoneStatus(uint16 connId, uint8_t procedureDoneStatus);

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
uint8_t llCsDbGetProcedureDoneStatus(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbIsAnyProcedureActive
 *
 * @brief       Is any CS procedure currently active.
 * We assume that only a single procedure is enabled at a time.
 *
 * input parameters
 *
 * @param       None
 * output parameters
 *
 * @param       None
 *
 * @return      True if any procedure is active, False otherwise.
 */
bool llCsDbIsAnyProcedureActive(void);

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
 * @fn          llCsDbSetProcedureEnableData
 *
 * @brief       Sets the procedure enable data that is built when sending
 * LL_CS_REQ packet. Used to update the enable data once LL_CS_RSP or LL_CS_IND
 * is received
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId   - connection Id
 * @param       configId - configuration Id
 * @param       enData   - enable data
 *
 * output parameters
 *
 * @param       None
 *
 * @return      void
 */
void llCsDbSetProcedureEnableData(uint16 connId, uint8 configId,
                                  const csProcedureEnable_t* enData);

/*******************************************************************************
 * @fn          llCsDbGetProcedureEnableData
 *
 * @brief       Gets the procedure enable data that was set when LL_CS_REQ pkt
 *              was built.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId   - connection Id
 * @param       configId - configuration Id
 * @param       enData   - enable data
 *
 * output parameters
 *
 * @param       enData
 *
 * @return      none
 */
void llCsDbGetProcedureEnableData(uint16 connId, uint8 configId,
                                  csProcedureEnable_t* enData);

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
uint8 llCsDbGetSubeventsPerEvent(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbCompareProcedureData
 *
 * @brief       Compare procedure data saved in the csdb, to what is provided
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       configId - config Id
 * @param       procData - procedure enable data
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
uint8 llCsDbCompareProcedureData(uint16 connId, uint8 configId,
                                 csProcedureEnable_t* procData);

/*******************************************************************************
 * @fn          llCsDbGetAclCounter
 *
 * @brief       Get the ACL counter that the CS is pending
 *
 * @design      BLE_LOKI-506
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
 * @return      ACL Counter
 */
uint16 llCsDbGetAclCounter(uint16 connId, uint8 configId);

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
void llCsDbSetNextProcedureConnEvent(uint16 connId, uint8 configId,
                                     uint16 connEvtCount);

/*******************************************************************************
 * @fn          llCsDbSetProcedureEnableIndData
 *
 * @brief       Sets the procedure enable data received over the LL_CS_IND pkt
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId   - connection Id
 * @param       configId - configuration Id
 * @param       enData   - enable data
 *
 * output parameters
 *
 * @param       enData
 *
 * @return      none
 */
void llCsDbSetProcedureEnableIndData(uint16 connId, uint8 configId,
                                     const csProcedureEnable_t* enData);

/*******************************************************************************
 * @fn          llCsDbGetNextProcedureFlag
 *
 * @brief       Get the next procedure flag
 *
 * input parameters
 *
 * @param       connId   - connection Id
 * @param       configId - configuration Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
uint8 llCsDbGetNextProcedureFlag(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbSetNextProcedureFlag
 *
 * @brief       Set the next procedure flag
 *
 * input parameters
 *
 * @param       connId   - connection Id
 * @param       next     - flag to set
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetNextProcedureFlag(uint16 connId, uint8 next);

/*******************************************************************************
 * @fn          llCsDbGetProcedureEnableACI
 *
 * @brief       Get the last ACI as in the procedureEnableData DB,
 *              for a specific connection and config id.
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
csACI_e llCsDbGetProcedureEnableACI(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbGetNextSubeventFlag
 *
 * @brief       Get the next subevent flag for a given connection.
 *
 * @param       connId - connection Id
 *
 * @return      Next subevent flag
 */
csNextSubevent_e llCsDbGetNextSubeventFlag(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbSetNextSubeventFlag
 *
 * @brief       Set the next subevent flag for a given connection.
 *
 * @param       connId - connection Id
 * @param       next - next subevent flag
 *
 * @return      None
 */
void llCsDbSetNextSubeventFlag(uint16 connId, csNextSubevent_e next);

/*******************************************************************************
 * @fn          llCsDbUpdateChannelMap
 *
 * @brief       Update channel map
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       pChM - pointer to channel map update
 * @param       currentTime - current RAT time
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status
 *              CS_STATUS_INVALID_CHM If the channel map combination results in
 * zero
 */
csStatus_e llCsDbUpdateChannelMap(uint8* pChM, uint32 currentTime);

/*******************************************************************************
 * @fn          llCsDbGetLastChmUpdateTime
 *
 * @brief       Get Last Channel Map Update time
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
 * @return      Last Channel Map Update Time
 */
uint32 llCsDbGetLastChmUpdateTime(void);

/*******************************************************************************
 * @fn          llCsDbGetChannelMap
 *
 * @brief       Get the Channel Map classification
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       pChM - pointer to channel map
 *
 * output parameters
 *
 * @param       pChm.
 *
 * @return      None
 */
void llCsDbGetChannelMap(uint8* pChm);

/*******************************************************************************
 * @fn          llCsDbSetFilterChannelMap
 *
 * @brief       Update the Channel Map classification in the DB
 *
 * input parameters
 *
 * @param       pChm - pointer to input channel map
 *
 * output parameters
 *
 * @return      None
 */
void llCsDbSetFilterChannelMap(uint8 connId, uint8 configId, const csChm_t* pChM);


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
 * @fn          llCsDbSetActiveConnId
 *
 * @brief      Set Active Conn Id
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection identifier
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetActiveConnId(uint16 connId);

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
 * @fn          llCsDbSetBleRole
 *
 * @brief       Set BLE role in the CS DB
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       role - BLE role, central or peripheral
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetBleRole(csBleRole role);

/*******************************************************************************
 * @fn          llCsDbGetBleRole
 *
 * @brief       Get BLE role: Central or Peripheral.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       none
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      BLE Role
 */
csBleRole llCsDbGetBleRole(void);

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
 * @fn          llCsDbGetIip
 *
 * @brief       Get the T_IP time from tIpTbl
 *
 * input parameters
 *
 * @param       idx - Index of the T_IP
 *
 * output parameters
 *
 * @param       None
 *
 * @return      T_IP
 */
uint8 llCsDbGetTip(uint8 idx);

/*******************************************************************************
 * @fn          llCsDbGetTfcs
 *
 * @brief       Get T_FCS from tFcsTbl
 *
 * input parameters
 *
 * @param       idx - Index of F_CS value
 *
 * output parameters
 *
 * @param       None
 *
 * @return      T_FCS
 */
uint8 llCsDbGetTfcs(uint8 idx);

/*******************************************************************************
 * @fn          llCsDbGetTpm
 *
 * @brief       Get T_PM from tPmTbl
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
uint16 llCsDbGetTpm(uint8 idx);

/*******************************************************************************
 * @fn          llCsDbInitChanIndexInfo
 *
 * @brief       Initialize the channel Index info struct.
 * There are two channel Index arrays, mode0 and nonMode0.
 * They are allocated according to the size numChan.
 * And initialized to the filtered channel map (chanArray)
 * Later it will be shuffled.
 *
 * @note        The allocated memory is freed in llCsDbFreeChannelIndexArray
 *              If the config is removed. And when all CS is freed.
 *              Also, when need to a allocate a different size.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param      connId    - connection Id
 * @param      configId  - CS config Id
 * @param      numCha    - number of channels to allocate
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
 * @param       connId - connection handle
 * @param       configId - cs config Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      pointer to channel info
 */
csChanInfo_t* llCsDbGetChanInfo(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbGetNumChan
 *
 * @brief       Get the number of channels for the given connId and configId.
 *
 * input parameters
 *
 * @param       connId - connection Handle
 * @param       configId - CS config Identifier
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      numChans
 */
uint8 llCsDbGetNumChan(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbUpdateChanIndexArray
 *
 * @brief       Update Channel Index Array of a specific connId and config Id
 *              In the DB.
 *              Main use: update after shuffling the array.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param      mode      - CS stepMode
 * @param      connId    - connection Id
 * @param      configId  - CS config Id
 * @param      chanArray - channel Index Array Info to update in the DB
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbUpdateChanIndexArray(uint8 mode, uint16 connId, uint8 configId,
                                const modeSpecificChanInfo_t* chanArr);

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
 * @param       configId - cs config identifier
 * @param       mmSteps - number of main mode steps
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetRemainingMmSteps(uint16 connId, uint8 configId, uint16 mmSteps);

/*******************************************************************************
 * @fn          llCsDbGetRemainingMmSteps
 *
 * @brief       Get the number of steps remaining for the procedure.
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
uint16 llCsDbGetRemainingMmSteps(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsDbGetSubeventInfo
 *
 * @brief       Get Subevent info from the DB
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       type   - info type:
 *                       CS_SE_INFO_NUM_STEPS to get the num of steps
 *                       CS_SE_INFO_STEP_COUNT to get the step count
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      numSteps or stepCount per type
 */
uint16 llCsDbGetSubeventInfo(uint16 connId, csSubeventInfo_e type);

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
void llCsDbSetSubeventCount(uint16 connId, csSubeventInfo_e type, uint16 count);

/*******************************************************************************
 * @fn          llCsDbIncrementSubeventInfoCounter
 *
 * @brief       Increment a subeventInfo counter by incVal
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       type   - info type
 * @param       incVal - incremention value
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void llCsDbIncrementSubeventInfoCounter(uint16 connId, csSubeventInfo_e type,
                                        uint16 incVal);

/*******************************************************************************
 * @fn          llCsDbIncrementProcCounter
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
uint16 llCsDbIncrementProcCounter(uint16 connId, csProcedureCounter_e counter);

/*******************************************************************************
 * @fn          llCsDbResetProcCounter
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
void llCsDbResetProcCounter(uint16 connId, csProcedureCounter_e counter);

/*******************************************************************************
 * @fn          llCsDbGetProcCounter
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
uint16 llCsDbGetProcCounter(uint16 connId, csProcedureCounter_e counter);

/*******************************************************************************
 * @fn          llCsDbSetEventsPerProcedure
 *
 * @brief       Set events per procedure
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       eventsPerProcedure - events per procedure
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDbSetEventsPerProcedure(uint16 connId, uint16 eventsPerProcedure);

/*******************************************************************************
 * @fn          llCsDbGetEventsPerProcedure
 *
 * @brief       Get Events Per Procedure
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
uint16 llCsDbGetEventsPerProcedure(uint16 connId);

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
void llCsDbSetEventAnchorPoint(uint16 connId, uint32_t anchorPoint);

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
uint32_t llCsDbGetEventAnchorPoint(uint16 connId);

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
 * @fn          llCsDbFreeChannelIndexArray
 *
 * @brief       Free channel Index array
 * The channel index array is dynamically allocated
 * Hence we should make sure to use this API to free it when done
 * There are two scenarios in which we would want to free:
 * * CS is done
 * * Channel Index array size is changed (if the CHM classification
 *   changed) then we need to free and allocate according to the
 *   new size
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       configId - cs config Id
 *
 * output parameters
 *
 * @param  None
 *
 * @return None
 */
void llCsDbFreeChannelIndexArray(uint16 connId, uint8 configId);

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
const csChm_t* llCsDbGetDefaultChMap();

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
void llCsDbSetSwitchTime(uint16 connId, uint8 tSw );

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
uint16 llCsDbGetSwitchTime(uint16 connId);

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
 * @param       pCsTestOverrideData - pointer to CS test overrides
 *
 * output parameters
 *
 * @param       pCsTestOverrideData
 *
 * @return      None
 */
void llCsDbGetTestOverrideData(csTestOverrideData_t* pCsTestOverrideData);

/*******************************************************************************
 * @fn          llCsDbGetReportedConnId
 *
 * @brief       Get Reported Connection ID
 * This function is needed because in test mode we assume a connId = 0
 * However, the spec specifies connID = 0xFFFF
 * We can't use this value in the rest of the code because the llCs DB
 * is based on the maxNumConns (1-8). 0xFFFF would create an attempt to
 * access beyond the boundaries of the llCs
 * So only when it comes to reporting the connId, it will be reported as 0xFFFF
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      ConnId
 */
uint16 llCsDbGetReportedConnId(void);

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
uint8 llCsDbGetOverrideCfg(csOverrideCfg_t* pOverrideCfg);

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
 * @param       ACI - ACI index to be used to determine the next
 *                    permutation index.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      The next Antenna Permutation index value.
 */
uint8_t llCsGetNextAntennaPermutationWrapper(csACI_e ACI);

/*******************************************************************************
 * @fn          llCsGetNextCsSyncAntennaSelection
 *
 * @brief       Get the CS Sync Antenna Selection value for a CS step.
 *              If loop is configured (only relevant for CS_TEST):
 *                  1. The next value will be returned.
 *                  2. Updates the value in the DB (defaultSettings).
 *
 *
 * input parameters
 *
 * @param       connId - Connection Id to get the value for
 *
 * output parameters
 *
 * @param       None
 *
 * @return      CS Sync Antenna Selection value
 */
uint8 llCsGetNextCsSyncAntennaSelection(uint8 connId);

/*******************************************************************************
 * @fn          llCsDbResetCsSyncAntennaSelectionLoop
 *
 * @brief       This function resets CS Sync Antenna Selection to its initial value.
 *              If loop is configured - Reset to @ref CS_SYNC_ANT_SELECT_REPETITIVE.
 *              Otherwise - doesn't reset.
 *
 * input parameters
 *
 * @param       connId - connection Id to be used for the reset.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbResetCsSyncAntennaSelectionLoop(uint16 connId);

/*******************************************************************************
 * @fn          llCsDbSetAntIndicesRCLMapping
 *
 * @brief       Sets antennas indices mapping into CS DB.
 *              This maps an antenna index to its actual index in RCL command.
 *
 * input parameters
 *
 * @param       connId - connection Id to be used for the reset.
 * @param       mappingToSet - antennas indices mapping to set.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetAntIndicesRCLMapping( uint16 connId, uint8 mappingToSet);

/*******************************************************************************
 * @fn          llCsDbSetTiming
 *
 * @brief Set the timing parameters for a specific connection and configuration.
 *
 * This function sets the timing parameters for a given connection and configuration ID
 * in the Connection State Database.
 *
 * input parameters
 *
 * @param connId The connection identifier.
 * @param configId The configuration identifier.
 * @param tIP1 Timing parameter IP1.
 * @param tIP2 Timing parameter IP2.
 * @param tFCs Timing parameter FCs.
 * @param tPM Timing parameter PM.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetTiming(uint16 connId, uint8 configId,
                                    uint8 tIP1,
                                    uint8 tIP2,
                                    uint8 tFCs,
                                    uint8 tPM);

/*******************************************************************************
 * @fn          llCsDbHandoverSetShuffledChanArr
 * @brief       Set the shuffled channel arrays for a specific connection and
 *              configuration.
 *              This function sets the shuffled channel arrays for Mode 0 and
 *              non-Mode 0 for a given connection and configuration ID in the
 *              Channel Sounding Database.
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 * @param       configId  - configuration Id to update
 * @param       pShuffledChanArrMode0    Pointer to the shuffled channel array for Mode 0.
 * @param       pShuffledChanArrNonMode0 Pointer to the shuffled channel array for non-Mode 0.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbHandoverSetShuffledChanArr(uint16 connId, uint8 configId, const uint8_t* pShuffledChanArrMode0, const uint8_t* pShuffledChanArrNonMode0);

/*******************************************************************************
 * @fn          llCsDbSetProcedureInfo
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
void llCsDbSetProcedureInfo(uint16 connId, const csProcedureInfo_t *pProcedureInfo);

/*******************************************************************************
 * @fn          llCsDbSetMainModeSteps
 * @brief       Set the 'main mode number of steps' minimum and maximum values
 *
 * input parameters
 *
 * @param       connId    - connection Id to be used for the reset.
 * @param       configId  - configuration Id to update
 * @param       numStepsMin  - minimum number of steps
 * @param       numStepsMax  - maximum number of steps
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbSetMainModeSteps(uint16 connId, uint8 configId, uint8 numStepsMin, uint8 numStepsMax);
