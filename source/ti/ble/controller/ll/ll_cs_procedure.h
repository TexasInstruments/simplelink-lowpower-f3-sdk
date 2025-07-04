/******************************************************************************

 @file  ll_cs_procedure.h

 @brief LL CS Procedure contains the APIs that are responsible for Initialziing
        the CS module. Building the CS steps of a CS subevent.
        Manages the CS double buffers.
        Sends CS Step results to the Host.

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

#define CS_MAX_CHANNEL_REPETITIONS         3      //! CS Max Channel Map repetitions
#define CS_SUBEVENT_RESULT_OPCODE          0x31   //!< CS Subevent Result Event Opcode
#define CS_CONTINUE_SUBEVENT_RESULT_OPCODE 0x32   //!< CS Continue Subevent Result Event Opcode

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
 * @fn          llCsCalculateSubeventSteps
 *
 * @brief       Calculate the number of steps for the subevent
 * And the number of steps remaining for the procedure.
 * Resets the subevent counter.
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       configId - Configuration ID of the CS procedure to run
 * @param       numMainModeSteps - Number of main mode steps in the procedure
 *
 * output parameters
 *
 * @param      None
 *
 * @return      None
*/
void llCsCalculateSubeventSteps(uint16 connId, uint8 configId, uint16 numMainModeSteps);

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
 * @fn          llCsInitProcedure
 *
 * @brief       Initializes the Channel Sounding Procedure
 * This function initializes the CS procedure by building the Channel Arrays,
 * initializing StepList and Results buffers, initialzied the first subevent
 * of the procedure. Sets the number of needed to complete the procedure.
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
csStatus_e llCsInitProcedure(uint16 connId, uint8 configId);

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
 * @return      Status
 */
csStatus_e llCsStartProcedure(llConnState_t* connPtr);

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
 * @fn          llCsStartStepListGen
 *
 * @brief       If a CS_START procedure was completed or next procedure should
 *              begin, generate step list for the upcoming procedure.
 *
 * input parameters
 *
 * @param       connId - connection Identifier
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
uint8 llCsStartStepListGen(uint16 connId);

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
 * @param       stepMode - step mode (0, 1, 2, 3)
 * @param       connId - connection ID
 * @param       csConfig - pointer to CS config
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Selected Channel Index
 */
uint8 llCsSelectStepChannel(uint8 stepMode, uint16 connId, const csConfigurationSet_t* csConfig);

/*******************************************************************************
 * @fn         llCsShuffleIndexArray
 *
 * @brief      Shuffle Channel Index Array
 * This function chooses the channel selection algorithm to shuffle
 * the channel index array of the given cs mode (0 or non-0)
 *
 * input parameters
 *
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
uint8 llCsShuffleIndexArray(uint8 mode, uint8 numChan,
                            modeSpecificChanInfo_t* chanArr,
                            uint8* filteredArr);

/*******************************************************************************
 * @fn          llCsSelectAA
 *
 * @brief       Select Step's Access Address.
 *
 * input parameters
 *
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
void llCsSelectAA(uint8 csRole, uint32_t* aaRx, uint32_t* aaTx);

/*******************************************************************************
 * @fn          llCsSetTswByACI
 *
 * @brief       Selects T_SW value by ACI which is agreed by both initiator
 *              and reflector, and set it to the relevant DB.
 *              If an invalid ACI value was given - the DB won't be modified
 *
 * input parameters
 *
 * @param       configId - CS config ID
 * @param       connId - connection ID
 * @param       ACI - ACI value to be considered when setting the T_SW value
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsSetTswByACI(uint8 configId, uint16 connId, csACI_e ACI);

/*******************************************************************************
 * @fn          llCsGetRandomSequence
 *
 * @brief       Get Random Sequence from DRBG
 *
 * input parameters
 *
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
void llCsGetRandomSequence(uint8 csRole, uint32_t* pTx, uint32_t* pRx,
                           uint8 payloadLen);

/*******************************************************************************
 * @fn          llCsGetToneExtention
 *
 * @brief       Get Tone Extension bit from DRBG
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Tone extension bit
 */
uint8 llCsGetToneExtention(void);

/*******************************************************************************
 * @fn          llCsGetNextAntennaPermutation
 *
 * @brief       Get the next Antenna Permutation based on hr1 function.
 *
 * input parameters
 *
 * @param       ACI - Antenna Configuration Index
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Antenna permutation Index (0...24)
 */
uint8 llCsGetNextAntennaPermutation(csACI_e ACI);

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
 * @param       configId - CS config ID
 * @param       connId - connection ID
 * @param       csConfig - pointer to CS config
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      status
 */
csStatus_e llCsInitChanIdxArr(uint8 configId, uint16 connId,
                              const csConfigurationSet_t* csConfig);


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
 * @fn          llCsProcedureCleanup
 *
 * @brief       Clears all CS DB indications related to procedure cleanup.
 * Might be called as a successful procedure completion, as well as a procedure
 * failure.
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
void llCsProcedureCleanup( uint8 connId, uint8 configId);

#endif // LL_CS_PROCEDURE_H
