/******************************************************************************

 @file  ll_cs_rcl_internal.h

 @brief Internal functions used by ll_cs_rcl module

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2022-2026, Texas Instruments Incorporated

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
#ifndef LL_CS_RCL_INTERNAL_H
#define LL_CS_RCL_INTERNAL_H

/*******************************************************************************
 * INCLUDES
 */
#include <ti/drivers/rcl/commands/ble_cs.h>
#include <ti/drivers/rcl/handlers/ble_cs.h>
#include "ti/ble/controller/ll/ll_cs_common.h"
#include "ti/ble/controller/ll/ll_common.h"

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * MACROS
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
 * @fn          llCsSetupCmdStartTime
 *
 * @brief       Setup the CS Command start time
 * The selected start time depends on the CS Role and whether the
 * SubeventCount is 0 or more.
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       configId - configuration Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status - Success or Failure
 */
csStatus_e llCsSetupCmdStartTime(uint16 connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsSetupNextStepBuffer
 *
 * @brief       Setup the switch buffer
 * This routine sets up the buffer to-switch-to by calculating the number of
 * steps needed based on the number of steps required for the current subevent
 * and step count done for the current subevent. In case all subevent steps are
 * done, it sets up the switch buffer for the next subevent.
 *
 * input parameters
 *
 * @param       pBuffer - Pointer to the buffer to switch to.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status
 */
csStatus_e llCsSetupNextStepBuffer(RCL_MultiBuffer* pBuffer);

/*******************************************************************************
 * @fn          llCsRclScheduleNextSubevent
 *
 * @brief       Schedule Next Subevent
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
void llCsRclScheduleNextSubevent(void);

/*******************************************************************************
 * @fn          llCsRclGetTxPower
 *
 * @brief       Get Tx Power for the RCL command
 *
 * input parameters
 *
 * @param       txPower - Tx Power value to be converted
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      txPower
 */
RCL_Command_TxPower llCsRclGetTxPower(int8 txPower);

/*******************************************************************************
 * @fn          llCsRClBufferSetup
 *
 * @brief       Setup the CS RCL double buffers.
 * Clear results buffers, init the steps and results buffers.
 * Put buffers in queue.
 *
 * input parameters
 * @param       None
 *
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsRClBufferSetup(void);

/*******************************************************************************
 * @fn          llCsSetupPrecalCmd
 *
 * @brief       Setup the pre-calibration command for the RCL
 *
 * input parameters
 *
 * @param       rclHandle - Handle to the RCL instance
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSetupPrecalCmd(RCL_Handle rclHandle);

/*******************************************************************************
 * @fn          llCsSendSubEventResults
 *
 * @brief       Send the result to the Host
 *
 * input parameters
 *
 * @param       pBuffer - Procedure results
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsSendSubEventResults(RCL_MultiBuffer* pBuffer);

/*******************************************************************************
 * @fn          llCsSendSubEventContResults
 *
 * @brief       Send the result continue to the Host
 *
 * input parameters
 *
 * @param       pBuffer - Procedure results
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsSendSubEventContResults(RCL_MultiBuffer* pBuffer);

/*******************************************************************************
 * @fn          llCsProcessResultsIsModeZeroValid
 *
 * @brief       Verify that the Mode-0 steps are valid for a given subEvent.
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       pBuffer - Buffer with subEvent step results
 *
 * output parameters
 *
 * @param       None
 *
 * @return      true in case at least one of the mode-0 steps is valid. false otherwise.
 */
bool llCsProcessResultsIsModeZeroValid(uint16 connId, const RCL_CmdBleCs_SubeventResults *pBuffer);

/*******************************************************************************
 * @fn          llCsRetrieveProcedureCompletionStatus
 *
 * @brief       Determine the completion status of a Channel Sounding procedure
 *
 * @details     Evaluates whether the current CS procedure has completed based on buffer
 *              state, procedure flags, and synchronization status. A procedure can be
 *              marked as complete when either normal completion occurs or when a
 *              synchronization error is detected in the final subevent.
 *
 * input parameters
 *
 * @param       connId - Connection Id
 * @param       configId - configuration Id
 * @param       pBuffer - Buffer with subEvent step results
 *
 * output parameters
 *
 * @param       None
 *
 * @return      CS_PROCEDURE_DONE if procedure is complete, CS_PROCEDURE_ACTIVE otherwise
 */
uint8_t llCsRetrieveProcedureCompletionStatus(uint16 connId, uint8 configId, RCL_MultiBuffer* pBuffer);

/*******************************************************************************
 * @fn          llCsSetupRclRelativeOffset
 *
 * @brief       Calculate the relative offset for RCL command timing
 *
 * @details     Determines the appropriate time offset from the connection anchor point
 *              for scheduling CS procedures based on current configuration.
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
 * @return      Calculated relative offset in microseconds
 */
uint32_t llCsSetupRclRelativeOffset(uint16 connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsSetupRclAnchorPoint
 *
 * @brief       Set up the anchor point for RCL timing
 *
 * @details     Calculates and prepares the base anchor point timing for CS procedures,
 *              including necessary adjustments based on PHY and other connection parameters.
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
 * @return      Anchor point timestamp in microseconds
 */
uint32_t llCsSetupRclAnchorPoint(uint16 connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsSetupRclTimerDrift
 *
 * @brief       Calculate timer drift compensation for CS procedures
 *
 * @details     Determines the expected timer drift based on connection parameters
 *              and sleep clock accuracy to ensure proper synchronization during CS.
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
 * @return      Calculated timer drift in microseconds
 */
uint32_t llCsSetupRclTimerDrift(uint16 connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsSetupRclRxWidening
 *
 * @brief       Calculate receive window widening for CS operations
 *
 * @details     Computes the appropriate receive window widening based on timer drift
 *              to compensate for clock inaccuracies between devices.
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       timerDrift - calculated timer drift in microseconds
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Receive window widening value in microseconds
 */
uint16_t llCsSetupRclRxWidening(uint16 connId, uint32_t timerDrift);

/*******************************************************************************
 * @fn          llCsSetupRclAllowDelay
 *
 * @brief       Determine if delayed CS procedure execution is allowed
 *
 * @details     Checks system conditions to determine if a CS procedure can be
 *              safely delayed without compromising synchronization.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      TRUE if delay is allowed, FALSE otherwise
 */
bool llCsSetupRclAllowDelay(void);

/*******************************************************************************
 * @fn          llCsGetScaFactor
 *
 * @brief       Get the Sleep Clock Accuracy factor for CS timing calculations
 *
 * @details     Retrieves the SCA factor associated with the specified connection.
 *              This factor is used to adjust timing calculations to compensate
 *              for clock drift between devices.
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      SCA factor value used in timing calculations
 */
uint16_t llCsGetScaFactor(uint16 connId);

#endif //LL_CS_RCL_INTERNAL_H

