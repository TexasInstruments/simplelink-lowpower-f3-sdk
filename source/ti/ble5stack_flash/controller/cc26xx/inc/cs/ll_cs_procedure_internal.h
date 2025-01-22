/******************************************************************************

 @file  ll_cs_procedure_internal.h

 @brief LL CS Procedure contains the APIs that are responsible for Initialziing
        the CS module. Building the CS steps of a CS subevent.
        Manages the CS double buffers.
        Sends CS Step results to the Host.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2024, Texas Instruments Incorporated

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
#include "ll_cs_common.h"
#include "ll_csdrbg.h"

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
 * @param       isFirstSE - is this the first Subevent
 * @param       connId - connection Identifier
 * @param       csConfig - pointer to config struct
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsShuffleMainModeChannelIndexArray(uint8 isFirstSE, uint16 connId,
                                      csConfigurationSet_t* csConfig);

/*******************************************************************************
 * @fn          llCsInitStepBuffers
 *
 * @brief       This function initalizes the step buffers
 * Allocates  memory for step buffers 0 and 1, and the step results
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      SUCCESS if all buffers were successfully allocated.
 *              INSUFFICIENT MEMORY otherwise
 */
csStatus_e llCsInitStepBuffers(void);

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
 * @param       config - pointer to config
 * @param       procParams - pointer to params
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      number of steps per subevent
 */
uint8 llCsNumStepsPerSubEvent(csConfigurationSet_t* config,
                              csProcedureEnable_t* procParams);

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
 * @param       mode - step mode
 * @param       pConfig - pointer to CS config
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Main Mode Step Duration in microseconds
 */
uint16 llCsMainModeDur(uint8 mode, csConfigurationSet_t* pConfig);

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
 * @param       stepMode -  Step Mode 0, 1, 2, 3
 * @param       connId - connection ID
 * @param       isRepetition - is a repeated main mode step
 * @param       stepData - pointer to step from stepList
 * @param       csConfig - pointer to CS config
 *
 * output parameters
 *
 * @param       pStep.
 *
 * @return      Status
 *
 */
csStatus_e llCsSetupStep(uint8 stepMode, uint16 connId,
                         RCL_CmdBleCs_Step* stepData,
                         csConfigurationSet_t* csConfig);

/*******************************************************************************
 * @fn          llCsSetupStep0
 *
 * @brief       Setup mode 0 step
 *
 * input parameters
 *
 * @param       role - CS initiator or reflector
 * @param       stepData - pointer to step data
 * @param       rttType - RTT type
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSetupStep0(uint8 role, RCL_CmdBleCs_Step* stepData, uint8 rttType);

/*******************************************************************************
 * @fn          llCsSetupStep1
 *
 * @brief       Setup mode 1 step
 *
 * input parameters
 *
 * @param       role - CS initiator or reflector
 * @param       stepData - pointer to step data
 * @param       rttType - RTT type
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSetupStep1(uint8 role, RCL_CmdBleCs_Step* stepData, uint8 rttType);

/*******************************************************************************
 * @fn          llCsSetupStep2
 *
 * @brief       Setup mode 2 step
 *
 * input parameters
 *
 * @param       role - CS initiator or reflector
 * @param       stepData - pointer to step data
 * @param       rttType - RTT type
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSetupStep2(uint8 role, RCL_CmdBleCs_Step* stepData, uint8 rttType);

/*******************************************************************************
 * @fn          llCsSetupStep3
 *
 * @brief       Setup mode 3 step
 *
 * input parameters
 *
 * @param       role - CS initiator or reflector
 * @param       stepData - pointer to step data
 * @param       rttType - RTT type
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSetupStep3(uint8 role, RCL_CmdBleCs_Step* stepData, uint8 rttType);

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
 * @fn          llCsGetNumMainModeSteps
 *
 * @brief       Get Number of Main Mode steps Using the hr1 function
 *
 * @design      BLE_LOKI-506
 * input parameters
 *
 * @param       mainModeMaxSteps - max main mode CS steps
 * @param       mainModeMinSteps - min main mode CS steps
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      numMainModeSteps
 */
uint8 llCsGetNumMainModeSteps(uint8 mainModeMaxSteps, uint8 mainModeMinSteps);

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
uint8 llCsChm2FilteredChanArr(uint8* pDecimalArray, uint8* pBitMapArray,
                          uint8 mapSize);

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
