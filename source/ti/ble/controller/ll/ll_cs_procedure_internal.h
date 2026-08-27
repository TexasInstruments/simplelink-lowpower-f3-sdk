#ifndef LL_CS_PROCEDURE_INTERNAL_H
#define LL_CS_PROCEDURE_INTERNAL_H

/******************************************************************************

 @file  ll_cs_procedure_internal.h

 @brief LL CS Procedure contains the internal functionality that are responsible
        for procedure handling of the CS module.

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

/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/controller/ll/ll_cs_common.h"
#include "ti/ble/controller/ll/ll_csdrbg.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

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
 * @fn          llCsShuffleMainModeChannelIndexArray
 *
 * @brief       This function shuffles the main mode channel index array
 * It must be used only when it is time to get the first non mode 0 channel.
 *
 * input parameters
 *
 * @param       connId - connection Identifier
 * @param       configId - cs config Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsShuffleMainModeChannelIndexArray(uint16 connId, uint8_t configId);


/*******************************************************************************
 * @fn          llCsNumStepsPerSubEvent
 *
 * @brief       Calculate number of steps per subevent
 * Includes mode-0, main mode, sub mode and their repetition.
 * This function only considers the timings (subevent len) and
 * step mode len. Also considers CS_MAX_STEPS_PER_SUBEVENT as
 * defined by the SPEC.
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       configId - cs config Id
 * @param       uint32 - subEventLen
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      number of steps per subevent
 */
uint8 llCsNumStepsPerSubEvent(uint16 connId, uint8_t configId, uint32  subEventLen);

/*******************************************************************************
 * @fn          llCsMainModeDur
 *
 *
 * @brief       Get Main Mode Step Duration
 * This function returns the provided main mode steps duration
 * The duration depends on the config and the step mode.
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       configId - cs config Id
 * @param       mode - step mode
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Main Mode Step Duration in microseconds
 */
uint16 llCsMainModeDur(uint16 connId, uint8_t configId, uint8 mode);

/*******************************************************************************
 * @fn          llCsSetupStep
 *
 * @brief       Builds the CS Step depending on the stepMode that was provided
 * Selects the step's channel, its Access Address, Tone Extention,
 * Random Sequence and Antenna.
 * It is assumed that this function is called with either one of
 * the following step modes 0, 1, 2, 3.
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       configId - cs config Id
 * @param       stepMode -  Step Mode 0, 1, 2, 3
 * @param       isRepetition - is a repeated main mode step
 * @param       stepData - pointer to step from stepList
 *
 * output parameters
 *
 * @param       pStep.
 *
 * @return      Status
 *
 */
csStatus_e llCsSetupStep(uint16 connId, uint8_t configId, uint8 stepMode,
                         RCL_CmdBleCs_Step* stepData);

/*******************************************************************************
 * @fn          llCsSetupStep0
 *
 * @brief       Setup mode 0 step
 *
 * input parameters
 *
 * @param       connId   - connection ID
 * @param       configId - cs config Id
 * @param       stepData - pointer to step data
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSetupStep0(uint16 connId, uint8_t configId,  RCL_CmdBleCs_Step* stepData);

/*******************************************************************************
 * @fn          llCsSetupStep1
 *
 * @brief       Setup mode 1 step
 *
 * input parameters
 *
 * @param       connId   - connection ID
 * @param       configId - cs config Id
 * @param       stepData - pointer to step data
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSetupStep1(uint16 connId, uint8_t configId,  RCL_CmdBleCs_Step* stepData);

/*******************************************************************************
 * @fn          llCsSetupStep2
 *
 * @brief       Setup mode 2 step
 *
 * input parameters
 *
 * @param       connId   - connection ID
 * @param       configId - cs config Id
 * @param       stepData - pointer to step data
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSetupStep2(uint16 connId, uint8_t configId,  RCL_CmdBleCs_Step* stepData);

/*******************************************************************************
 * @fn          llCsSetupStep3
 *
 * @brief       Setup mode 3 step
 *
 * input parameters
 *
 * @param       connId   - connection ID
 * @param       configId - cs config Id
 * @param       stepData - pointer to step data
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSetupStep3(uint16 connId, uint8_t configId, RCL_CmdBleCs_Step* stepData);

/*******************************************************************************
 * @fn          llCsConvertRttType
 *
 * @brief       Converts RTT type to RTT Type index
 *
 * input parameters
 *
 * @param       rttType - rttType
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
uint16 llCsConvertRttType(uint8 rttType);

/*******************************************************************************
 * @fn          llCsAASelectionRules
 *
 * @brief       CS Access Address Selection Rules
 *
 * @design      BLE_LOKI-506
 * input parameters
 *
 * @param       si - The 1st sequence of the si, si+1 pair
 * @param       sj - The 2nd sequence of the si, si+1 pair
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Selected 32 bit uint
 */
uint32_t llCsAASelectionRules(uint32_t si, uint32_t sj);

/*******************************************************************************
 * @fn          llCsAutoCorrelation
 *
 * @brief       Calculates the Autocorrelaction of a 32bit uint
 *
 * @design      BLE_LOKI-506
 * input parameters
 *
 * @param       s - 32 bit vecore
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Auto Correlation Score of s
 */
uint8 llCsAutoCorrelation(uint32_t s);


/*******************************************************************************
 * @fn          llCsChm2FilteredChanArr
 *
 * @brief       Filter Channel Map Array
 * This function is meant to take array of bits, check which
 * bit is valid (set to 1), calculate the value of the location of
 * the bit in decimal format and put it in a new array with the
 * decimal value
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 * @param       pDecimalArray - array with the size of the amount of 1 bit
 *                              in the pChannelBitMapArray
 * @param       pBitMapArray  - holds the bit array (1 - valid, 0 - not valid)
 * @param       mapSize - the amount of bytes in
 *                                        the pChannelBitMapArray
 *
 * output parameters
 *
 * @param       pDecimalArray  - array with decimal value of all one bits
 *                               there is in pChannelBitMapArray
 *
 * @return      None.
 */
uint8 llCsChm2FilteredChanArr(uint8_t *pDecimalArray, const uint8_t *pBitMapArray,
                          uint8_t mapSize);

/*******************************************************************************
 * @fn          llCsReversePayload
 *
 * @brief       Reverse the Random Payload for RTT
 * The final bit received from the DRBG is the LSB of the Random Sequence, and
 * it is transmitted first.
 * The first bit received from the DRBG is the MSB of the Random Sequence, and
 * it is transmitted last.
 * To Achieve this bytes from the DRBG should be reversed.
 *
 * input parameters
 *
 * @param       pl1 - the first payload to reverse
 * @param       pl2 - the second paylaod to reverse
 * @param       size - the size of the payload
 *
 * output parameters
 *
 * @param       pl1 - reversed
 * @param       pl2 - reversed
 *
 * @return      None
 */
void llCsReversePayload(uint8* pl1, uint8* pl2, uint8 size);

/*******************************************************************************
 * @fn          llCsGetTSync
 *
 * @brief       Calculate the synchronization time
 *
 * input parameters
 *
 * @param       phy     - PHY type
 * @param       plSize  - Payload size
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Synchronization time
 */
uint8 llCsGetTSync(uint8 phy, uint8 plSize);

/*******************************************************************************
 * @fn          llCsMode0Duration
 *
 * @brief       Calculate the duration for Mode 0
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       configId - cs config Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Duration for Mode 0
 */
uint16 llCsMode0Duration(uint16_t connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsMode1Duration
 *
 * @brief       Calculate the duration for Mode 1
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       configId - cs config Id
 * @param       payloadSize - Size of the payload
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Duration for Mode 1
 */
uint16 llCsMode1Duration(uint16_t connId, uint8_t configId, uint8 payloadSize);

/*******************************************************************************
 * @fn          llCsMode2Duration
 *
 * @brief       Calculate the duration for Mode 2
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       configId - cs config Id
 * @param       nPath   - Number of paths
 * @param       tSw     - Antenna Switching Time in us
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Duration for Mode 2
 */
uint16 llCsMode2Duration(uint16_t connId, uint8_t configId, uint8 nPath, uint16 tSw);

/*******************************************************************************
 * @fn          llCsMode3Duration
 *
 * @brief       Calculate the duration for Mode 3
 *
 * input parameters
 *
 * @param       connId - connection ID
 * @param       configId - cs config Id
 * @param       nPath   - Number of paths
 * @param       tSw     - Antenna Switching Time in us
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Duration for Mode 3
 */
uint16 llCsMode3Duration(uint16_t connId, uint8_t configId, uint8 nPath, uint16 tSw);

/*******************************************************************************
 * @fn          llCsClearStepBuffers
 *
 * @brief       Clear the internal buffers lists
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
void llCsClearStepBuffers(void);

/*******************************************************************************
 * @fn          llCsInitProcedure
 *
 * @brief       Initializes the Channel Sounding Procedure
 * This function initializes the CS procedure by building the Channel Arrays,
 * initializing StepList and Results buffers, initialzied the first subevent
 * of the procedure. Sets the number of needed to complete the procedure.
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
csStatus_e llCsInitProcedure(uint16 connId);

/*******************************************************************************
 * @fn          llCsUpdateProcedureCounter
 *
 * @brief       Check if incrementing the procedure counter is needed
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
void llCsUpdateProcedureCounter(uint16_t connId);

/*******************************************************************************
 * @fn          llCsTerminateAlignProcCounter
 *
 * @brief       Check if the peer procedure counter is larger than the local one
 *              If it is, it calculates the delta between the two counters and update
 *              the local counter to match the peer counter.
 *
 * input parameters
 *
 * @param       connId     - Connection Id
 * @param       peerCount  - Peer procedure counter
 * @param       localCount - Local procedure counter
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsTerminateAlignProcCounter(uint16_t connId, uint16_t peerCount, uint16_t localCount);

/*******************************************************************************
 * @fn          llCsFillBuffer
 *
 * @brief       Fill CS Buffer with step details
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       configId - config Id
 * @param       mode - mode
 * @param       numSteps - number of steps
 * @param       stepType - step type to indicate if this is repetitions/subModeInsertion/normal step
 * @param       steps - pointer to steps
 *
 * output parameters
 * @param       csSteps
 *
 * @return      Status
 */
csStatus_e llCsFillBuffer(uint16 connId, uint8_t configId, uint8 mode, uint16 numSteps, csStepType_e stepType, RCL_CmdBleCs_Step* steps);

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
 * @fn          llCsGetChannelIndex
 *
 * @brief       Get Channel Index for the step. In case this is a main mode repetition step,
 *              load the Channel Index from the DB. Otherwise, a new channel shall be selected from
 *              the Channel Selection Shuffled Channels.
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       configId - Configuration ID of the CS procedure to run
 * @param       mode - mode
 * @param       stepType - step type to indicate if this is repetitions/subModeInsertion/normal step
 *
 * output parameters
 *
 * @param      None
 *
 * @return      Channel index
*/
uint8_t llCsGetChannelIndex(uint16 connId, uint8_t configId, uint8 mode, csStepType_e stepType);


/*******************************************************************************
 * @fn          llCsGetMModeRepetitionsIndex
 *
 * @brief       Get Main Mode Repetitions Channel Array Index for the step. This will
 *              point to the 'oldest' value in the array, as the channels in the Main
 *              Mode repetitions steps should be transmitted in the same order as in
 *              original subEvent.
 *
 * input parameters
 *
 * output parameters
 *
 * @param      None
 *
 * @return      Channel Array index
*/
uint8_t llCsGetMModeRepetitionsIndex(void);

/*******************************************************************************
 * @fn          llCsDbSaveChannelIdx
 *
 * @brief       Save the channel index of the last main mode step. Those will be reused in
 *              the main mode repetition and sub Mode insertion steps.
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       configId - Configuration ID of the CS procedure to run
 * @param       mode - mode
 * @param       stepType - step type to indicate if this is repetitions/subModeInsertion/normal step
 * @param       channelIdx - channel indec to save
 *
 * output parameters
 *
 * @param      None
 *
 * @return      None
*/
void llCsDbSaveChannelIdx(uint16 connId, uint8_t configId, uint8 mode, csStepType_e stepType, uint8_t channelIdx);

/*******************************************************************************
 * @fn          llCsUpdateStepCount
 *
 * @brief       Increment the step count for the subevent handling.
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       configId - Configuration ID of the CS procedure to run
 * @param       stepType - step type to indicate if this is repetitions/subModeInsertion/normal step
 *
 * output parameters
 *
 * @param      None
 *
 * @return      None
*/
void llCsUpdateStepCount(uint16 connId, uint8_t configId, csStepType_e stepType);

 /*******************************************************************************
 * @fn          llCsAdjustSubEventStepCount
 *
 * @brief       Adjust main mode step count when submode insertion affects timing.
 *              When submode insertion steps take longer than planned, this function
 *              adjusts the main mode step count to maintain proper timing. It compensates
 *              for any difference between planned and actual step counts, carrying over
 *              steps to the next subevent if necessary.
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       configId - Configuration ID of the CS procedure to run
 *
 * output parameters
 *
 * @param      None
 *
 * @return      None
*/
void llCsAdjustSubEventStepCount(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsSetConnMaxTimeForCS
 *
 * @brief       Sets connection max time to ensure connection event ends before
 *              CS procedure starts. Uses the CS offset (time from connection
 *              anchor point to CS start) as the maximum connection time.
 *
 *              This function is called when the NEXT connection event will
 *              have CS execution to prevent the connection event from overlapping
 *              with the CS procedure timing.
 *
 *              Per BLE Core Spec 6.2, Vol 6, Part B, Section 4.5.18.1:
 *              CS procedures occur at a specific offset from the connection
 *              anchor point (T_CS_OFFSET), measured in microseconds.
 *
 *              Note: The scheduler automatically applies
 *              LL_MARGIN_TIME_FOR_TIMER_HANDLING_RAT_TICKS (1200us) margin
 *              in llLinkSchedSetup() to account for timer handling overhead
 *              and RCL switch time.
 *
 * input parameters
 *
 * @param       connPtr  - Pointer to connection state structure
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSetConnMaxTimeForCS(llConnState_t* connPtr);

#endif // LL_CS_PROCEDURE_INTERNAL_H