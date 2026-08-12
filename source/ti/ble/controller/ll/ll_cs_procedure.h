/******************************************************************************

 @file  ll_cs_procedure.h

 @brief LL CS Procedure contains the APIs that are responsible for Initialziing
        the CS module. Building the CS steps of a CS subevent.
        Manages the CS double buffers.
        Sends CS Step results to the Host.

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
#ifndef LL_CS_PROCEDURE_H
#define LL_CS_PROCEDURE_H

/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/controller/ll/ll_cs_common.h"
#include "ti/ble/controller/ll/ll_csdrbg.h"
#include "ti/ble/controller/ll/ll_cs_rcl.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */
//!< CS Subevent Result Event Opcode
#define CS_SUBEVENT_RESULT_OPCODE          RCL_CMD_BLE_CS_SUBEVENT_RESULTS_OPCODE
//!< CS Continue Subevent Result Event Opcode
#define CS_CONTINUE_SUBEVENT_RESULT_OPCODE RCL_CMD_BLE_CS_SUBEVENT_RESULTS_CONTINUE_OPCODE

#define CS_PREVIOUS_PROCEDURE_VALID        0
#define CS_PREVIOUS_PROCEDURE_INVALID      1

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
 * @fn          llCsInit
 *
 * @brief       Initialize the CS feature
 * This function calls llCsInit Db which in turn allocates the
 * memory needed for CS, and initializes the structures and their
 * initial settings for usage
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Status
 *              CS_INSUFFICIENT_MEMORY
 *              CS_SUCCESS
 */
uint8 llCsInit(void);

/*******************************************************************************
 * @fn          llCsNewSubEvent_getSubEventType
 *
 * @brief       This function returns the type of the subevent, based on the status of the csStepBufferList.
 * In case it empty - we are starting a new subevent, otherwise we are continuing an existing one.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      CS_NEW_SUBEVENT
 *              CS_CONTINUE_SUBEVENT
 *
 */
csSubeventType_e llCsNewSubEvent_getSubEventType(void);

/*******************************************************************************
 * @fn          llCsClearConnProcedures
 *
 * @brief       Clear Procedures per connection (usually due to termination)
 *
 * input parameters
 *
 * @param       connId - connection Identifier
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsClearConnProcedures(uint16 connId);

/*******************************************************************************
 * @fn          llCsFreeAll
 *
 * @brief       Call llCsDbFree to free all memory allocated for CS.
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
void llCsFreeAll(void);

/*******************************************************************************
 * @fn          llCsSetFeatureBit
 *
 * @brief       Set the CS feature bit in deviceFeatureSet.featureSet
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
void llCsSetFeatureBit(void);

/*******************************************************************************
 * @fn          llCsIsHostFeatureBitSet
 *
 * @brief       Check if CS Host Feature bit is set.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      True if CS Host Feature bit is set, False otherwise
 */
bool llCsIsHostFeatureBitSet(void);

/*******************************************************************************
 * @fn          llCsInitSubevent
 *
 * @brief       Initializes the Channel Sounding Subevent
 * Calculates the number of steps needed in the subevent.
 * Sets the number of steps remaining to complete the procedure.
 *
 * input parameters
 *
 * @param       connId  -  Connection Id
 * @param       configId - Configuration ID of the CS procedure to run
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsInitSubevent(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsStartStepListGen
 *
 * @brief       If a CS_START procedure was completed or next procedure should
 *              begin, generate step list for the upcoming procedure.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId  -  Connection Id
 * @param       configId - Configuration ID of the CS procedure to run
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status
 *              LL_CS_STATUS_NO_PROCEDURE - if no CS procedure is enabled
 *              LL_CS_STATUS_INVALID_CONN_PTR - invalid connection pointer
 *              LL_CS_STATUS_SUCCESS
 */
csStatus_e llCsStartStepListGen(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsInitSubeventStepBuffers
 *
 * @brief       Initialize the subevent (tx) buffers for a new procedure
 * This function is to be used before starting a new procedure, or a procedure
 * repetition. It will setup CS_STEP_TX_BUFF_NUM for initial use.
 *
 * input parameters
 *
 * @param       connId - Connection ID for the CS Procedure.
 * @param       configId - Configuration ID for the CS Procedure.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void llCsInitSubeventStepBuffers(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsSetupStepBuffers
 *
 * @brief       Setup Step Buffers
 * Setup Step Buffers for a new or an ongiong subevent.
 * If a new subevent, the first buffer is built and begins with mode 0 steps.
 * If it's a continue buffer of a subevent, then we do not begin with mode 0.
 *
 * input parameters
 *
 * @param       connId       - Connection Id
 * @param       configId     - CS Config Id
 * @param       isNewSubevent- flag that indicates if this is a new subevent
 * @param       csStepsBuff  - Pointer to steps buffer to set up
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */

void llCsSetupStepBuffers(uint16 connId, uint8 configId, csSubeventType_e isNewSubevent,
                                                         RCL_MultiBuffer* csStepsBuff);

/*******************************************************************************
 * @fn          llCsStartProcedure
 *
 * @brief       Start a CS Procedure
 * This function checks whether all the requirements for starting a CS procedure
 * have been met, and if they are a CS procedure starts by building the RCL
 * command and a CS task.
 *
 * input parameters
 *
 * @param       connPtr - Pointer to the current connection

 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void llCsStartProcedure(llConnState_t* connPtr);

/*******************************************************************************
 * @fn          llCsStartTestProcedure
 *
 * @brief       Start a CS Test Procedure
 * This function is the equivalent of llCsStartProcedure but for test mode.
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
csStatus_e llCsStartTestProcedure(void);

/*******************************************************************************
 * @fn          llCsStopTestProcedure
 *
 * @brief       Stop a CS Test Procedure
 * This function will terminate ongoing CS tests.
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
void llCsStopTestProcedure(void);

/*******************************************************************************
 * @fn          llCsNewSubEvent_getSubEventType
 *
 * @brief       This function returns the type of the subevent, based on the status of the csStepBufferList.
 * In case it empty - we are starting a new subevent, otherwise we are continuing an existing one.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      CS_NEW_SUBEVENT
 *              CS_CONTINUE_SUBEVENT
 *
 */
csSubeventType_e llCsNewSubEvent_getSubEventType(void);

/*******************************************************************************
 * @fn          llCsDisableProcedure
 *
 * @brief       Disable the current procedure
 * This routine gracefully disables a procedure.
 * Use this routine if the host requested to disable the procedure.
 * Or should an error occur while attempting to start a procedure.
 *
 * input parameters
 *
 * @param       connId - Connection Identifier
 * @param       configId - Configuration Identifier
 * @param       status - Status of the procedure
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsDisableProcedure(uint16 connId, uint8 configId, uint8 status);

/*******************************************************************************
 * @fn          llCsSelectStepChannel
 *
 * @brief       Selects the step's channel based on the step mode
 * The only caller for this function is llCsSetupStep.
 * The CS Channel Index is  a number between 2-78
 * With Expections.
 * See Table in CS SPEC:
 * Channel Sounding physical channels
 * Mapping of CS channel index to RF physical channel
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       configId - Configuration Identifier
 * @param       stepMode - step mode (0, 1, 2, 3)
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Selected Channel Index
 */
uint8 llCsSelectStepChannel(uint16 connId, uint8_t configId, uint8 stepMode);

/*******************************************************************************
 * @fn         llCsShuffleIndexArray
 *
 * @brief      Shuffle Channel Index Array
 * This function chooses the channel selection algorithm to shuffle
 * the channel index array of the given cs mode (0 or non-0)
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       mode - cs step mode (0 or non-0)
 * @param       numchan - number of channels (array size)
 * @param       chanArr - struct with info about the channel array
 * @param       filteredArr - filtered channel array
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
uint8 llCsShuffleIndexArray(uint16 connId, uint8 mode, uint8 numChan,
                            modeSpecificChanInfo_t* chanArr,
                            uint8* filteredArr);

/*******************************************************************************
 * @fn          llCsSelectAA
 *
 * @brief       Select Step's Access Address.
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       csRole - CS role initiator or reflector
 * @param       aaRx - pointer to the first part of the access address
 * @param       aaTx - pointer to the second part of the access address
 *
 * output parameters
 *
 * @param       aaRx
 * @param       aaTx
 *
 * @return      None
 */
void llCsSelectAA(uint16 connId, uint8 csRole, uint32_t* aaRx, uint32_t* aaTx);

/*******************************************************************************
 * @fn          llCsSetTswByACI
 *
 * @brief       Selects T_SW value by ACI which is agreed by both initiator
 *              and reflector, and set it to the relevant DB.
 *              If an invalid ACI value was given - the DB won't be modified
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       role - CS role
 * @param       ACI - ACI value to be considered when setting the T_SW value
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsSetTswByACI(uint16 connId, uint8 role, csACI_e ACI);

/*******************************************************************************
 * @fn          llCsGetRandomSequence
 *
 * @brief       Get Random Sequence from DRBG
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       csRole - CS role
 * @param       pTx - pointer to transmitted Random Sequence
 * @param       pRx - pointer to the recvd Random Sequence
 * @param       payloadLen - payload length
 * output parameters
 *
 * @param       pTx
 * @param       pTx
 *
 * @return      None
 */
void llCsGetRandomSequence(uint16 connId, uint8 csRole, uint32_t* pTx, uint32_t* pRx,
                           uint8 payloadLen);

/*******************************************************************************
 * @fn          llCsGetToneExtention
 *
 * @brief       Get Tone Extension bit from DRBG
 *
 * input parameters
 *
 * @param       connId - connection ID
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Tone extension bit
 */
uint8 llCsGetToneExtention( uint16 connId );

/*******************************************************************************
 * @fn          llCsGetNextAntennaPermutation
 *
 * @brief       Get the next Antenna Permutation based on hr1 function.
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       ACI - Antenna Configuration Index
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Antenna permutation Index (0...24)
 */
uint8 llCsGetNextAntennaPermutation(uint16 connId, csACI_e ACI);

/*******************************************************************************
 * @fn          llCsGenerateAntennaGpioValues
 *
 * @brief       Generates the final antennas GPIO values to be used by the RCL for
 *              antenna switching, based on a preferred antenna parameter and
 *              a given mux values per antenna.
 *              In addition, stores a mapping for each antenna index to its final
 *              index as set in the output array into the DB.
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       configId - Configuration Id
 *
 * @return      None
 */
void llCsGenerateAntennaGpioValues(uint16_t connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsDbResetCsSyncAntennaSelectionLoop
 *
 * @brief       This function resets CS Sync Antenna Selection in procedureInfo
 *              to its initial value based on the default settings for the given
 *              connection.
 *
 * input parameters
 *
 * @param       connId - connection Id to be used for the reset
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsDbResetCsSyncAntennaSelectionLoop(uint16_t connId);

/*******************************************************************************
 * @fn          llCsGetCsSyncAntennaSelection
 *
 * @brief       Get the CS Sync Antenna Selection value for a CS step.
 *              If repeat is configured:
 *                  1. The next value will be returned.
 *                  2. Updates the value in the DB.
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
uint8_t llCsGetCsSyncAntennaSelection(uint16_t connId);

/*******************************************************************************
 * @fn          llCsInitChanIdxArr
 *
 * @brief       Initialize Channel Index array
 * This function turns the filtered channel map to a channel index
 * array. It sets the initial channel index info in the struct.
 * It shuffles the mode 0 channel index array which will be used
 * immediately.
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       configId - CS config ID
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      status
 */
csStatus_e llCsInitChanIdxArr(uint16_t connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsInitStepAndResultBuffers
 *
 * @brief       This function initalizes the step buffers
 * Uses RCL_Multibuffers_init to initialize the step buffers
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return       None
 * */
void llCsInitStepAndResultBuffers(void);

/*******************************************************************************
 * @fn          llCs_finishAndResetProcedure
 *
 * @brief       Finishes and resets the completed procedure
 * Disables the procedure params, sets the next procedure flag to FALSE,
 * resets the procedure counter, clears the RCL buffers, and frees the RCL task.
 * Also, ends the test mode if it was enabled.
 *
 * input parameters
 *
 * @param       connId  - Connection ID
 * @param       configId - Configuration ID
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCs_finishAndResetProcedure(uint16_t connId, uint8_t configId);


/*******************************************************************************
 * @fn          llCs_finishAndIncrementProcedure
 *
 * @brief       Handles cleanup of the procedure in case of error in starting
 *              the procedure. Must be called ONLY in case the procedure didn't complete the init.
 * Increments the ProcedureCount and runs backtracking.
 * Calls llCs_finishAndResetProcedure
 *
 * input parameters
 *
 * @param       connId  - Connection ID
 * @param       configId - Configuration ID
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCs_finishAndIncrementProcedure(uint16_t connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsProcedureCleanup
 *
 * @brief       Clears all CS DB indications related to procedure cleanup.
 *              This should be called when CS Start Procedure is complete.
 *
 * input parameters
 *
 * @param       connId  - Connection ID
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsProcedureCleanup( uint16 connId);

/*******************************************************************************
 * @fn          llCsCalculateSubEventLen
 *
 * @brief       Clears all CS DB indications related to procedure cleanup.
 *              This should be called when CS Start Procedure is complete.
 *
 * input parameters
 *
 * @param       connId  - Connection ID
 * @param       configId - Configuration ID
 * @param       subEventLen - Proposed subEvent lenght
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Calculated subEvent Lentgh
 */
uint32 llCsCalculateSubEventLen(int16 connId, uint8 configId, uint32 subEventLen);

/*******************************************************************************
 * @fn          llCsProcedureError_SendResultsAndCleanup
 *
 * @brief       Processes error conditions during a Channel Sounding procedure by properly
 *              terminating the active procedure and returning the system to normal BLE
 *              operation. This function marks the procedure as failed, cleans up resources,
 *              and ensures the connection returns to standard operation.
 *
 *              The function sends CS results with an error status in the procedure status
 *              field to notify higher layers about the failure.
 *
 * input parameters
 *
 * @param       connId  - Connection ID
 * @param       configId - Configuration ID
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsProcedureError_SendResultsAndCleanup(uint16_t connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsProcedureError
 *
 * @brief       Error processing for the CS procedure
 *              This function only has effect when called from within the CS task context.
 *              It verifies that a CS task is currently allocated and active before
 *              performing any operations.
 *              Then, it calls to llCsProcedureError_SendResultsAndCleanup
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
extern void llCsProcedureError(void);

void llCsStartProcedure(llConnState_t* connPtr);

/*******************************************************************************
 * @fn          llCsRealignProceduresRepetition
 *
 * @brief       Realign to the next procedure in case there were missed procedures.
 *              If the procedure count exceeds the maximum allowed, it will realign
 *              the parameters to the last procedure and finish the CS.
 *
 * input parameters
 *
 * @param       connPtr  - Pointer to the current connection
 * @param       configId - Configuration ID
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void llCsRealignProceduresRepetition(llConnState_t* connPtr, uint8_t configId);

/*******************************************************************************
 * @fn          llCsPrepareNextProcedure
 *
 * @brief       Prepare the relevant parameters needed for the next procedure.
 *
 * input parameters
 *
 * @param       connId   - Connection Id
 * @param       configId - Configuration Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      TRUE in case the procedure was terminated
                FALSE otherwiase
*/
bool llCsPrepareNextProcedure(uint16_t connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsSetLastStepGenConnEventCount
 *
 * @brief       Set the last connection event count that we were able to generate steps
 *
 * input parameters
 *
 * @param       connId   - Connection Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsSetLastStepGenConnEventCount(uint16_t connId);

/*******************************************************************************
 * @fn          llCsTerminateCsProcedure
 *
 * @brief       Handle the CS termination process. This includes align the procedure
 *              counter, finish and reset, and notify the host
 *
 * input parameters
 *
 * @param       connId     - Connection Id
 * @param       configId   - Configuration Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsTerminateCsProcedure(uint16_t connId, uint8_t configId);

/*******************************************************************************
* @fn          llCsHandleProcedureDesync
*
* @brief       Handle procedure desynchronization
*
* @details     Manages the procedure desynchronization based on
*              synchronization history. If two consecutive procedures fail to
*              synchronize, initiates termination of procedure repetitions
*              according to specification requirements.
*
*              The function tracks procedure validity state and maintains history
*              to properly detect consecutive synchronization failures.
*
* input parameters
*
* @param       connId - connection Id
* @param       configId - configuration Id
*
* output parameters
*
* @param       None.
*
* @return      TRUE in case the procedure was terminated
               FALSE otherwiase
*/
bool llCsHandleProcedureDesync(uint16_t connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsProcIsTestModeEnabled
 *
 * @brief       Check if Test Mode is enabled
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      TRUE - if CS Test mode is ON
 *              FALSE - otherwise
 */
bool llCsProcIsTestModeEnabled(void);

/*******************************************************************************
 * @fn          llCsProcGetActiveConnId
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
uint16 llCsProcGetActiveConnId(void);

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
 * @fn          llCsProcGetReportedConnId
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
uint16 llCsProcGetReportedConnId(void);

/*******************************************************************************
* @fn          llCsProcGetConnIdAndConfigId
*
* @brief       Retrieve active connection and configuration IDs for CS procedure
*
* @details     Retrieves the connection ID and configuration ID for the currently
*              active CS procedure. These identifiers are essential for proper
*              database access and procedure management.
*
* input parameters
*
* @param       pConnId - pointer to store the connection ID
* @param       pConfigId - pointer to store the configuration ID
*
* output parameters
*
* @param       pConnId - retrieved connection ID value
* @param       pConfigId - retrieved configuration ID value
*
* @return      TRUE if valid IDs were retrieved, FALSE otherwise
*/
bool llCsProcGetConnIdAndConfigId(uint16_t *pConnId, uint8_t *pConfigId);

/*******************************************************************************
* @fn          llCsIsSingleProcedure
*
* @brief       Determines if current procedure is a single procedure or repetitions
*
* @details
*
* input parameters
*
* @param       connId - connection Id
* @param       configId - configuration Id
*
* output parameters
*
* @param
*
* @return      TRUE in case of single procedure
*              FALSE in case repetitions
*/
bool llCsIsSingleProcedure(uint16_t connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsResetConnMaxTime
 *
 * @brief       Resets connection max time by clearing the external update flag.
 *              The scheduler will recalculate connMaxTimeLength when it
 *              schedules the next connection event.
 *
 * @param       connId - Connection ID
 *
 * @return      None
 */
void llCsResetConnMaxTime(uint16 connId);

#endif // LL_CS_PROCEDURE_H
