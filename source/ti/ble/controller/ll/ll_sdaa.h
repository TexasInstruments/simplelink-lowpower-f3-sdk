/******************************************************************************

 @file  ll_sdaa.h

 @brief This file contains the SDAA (Selective Detect And Avoid) moudule,
        This module is responsible for monitoring and limiting TX consumption
        per channel, the module will be activated by SDAA_ENABLE define

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

#include <stdint.h>
#include <ti/drivers/rcl/commands/ble5.h>
#include "ti/ble/controller/ll/ll_scheduler.h"
#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll.h"

// Stub headers
#include "ti/ble/stack_util/lib_opt/ctrl_stub_ae.h"

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
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */
void LL_rclChannelAssessmentCallback( RCL_Command *cmd,
                                      LRF_Events lrfEvents,
                                      RCL_Events events );

/*********************************************************************
 * @fn      LL_SDAA_GetChnlAssessmentTask
 *
 * @brief   This function used to get the pointer of the Channel
 *          assessment task.
 *
 * input parameters
 *
 * @param   None
 *
 * output parameters
 *
 * @param   None.
 *
 * @return  pointer to channel assessment task.
 */
taskInfo_t *LL_SDAA_GetChnlAssessmentTask(void);

/*********************************************************************
 * @fn      LL_SDAA_PostProcess
 *
 * @brief   This function used to get the status of the channel
 *          assessment and update the channel state.
 *
 * input parameters
 *
 * @param   None
 *
 * output parameters
 *
 * @param   None
 *
 * @return  None
 */
void LL_SDAA_PostProcess(void);

/*********************************************************************
 * @fn      LL_SDAA_HandleLastCmdDone
 *
 * @brief   This function handles the last command done events for
 *          various transmit BLE tasks, such as connection,
 *          periodic advertising, and extended advertising.
 *          It calculates the dwell time for the completed task and
 *          records it for dynamic observation period calculations.
 *
 * input parameters
 *
 * @param   events    - The LL Events flags
 * @param   pCurrTask - pointer to the current task
 *
 * output parameters
 *
 * None
 *
 * @return  None
 */
void LL_SDAA_HandleLastCmdDone(uint32            events,
                               taskInfo_t* const pCurrTask);

/*********************************************************************
* @fn          LL_SDAA_ControlTxAndGetStartType
*
* @brief       This function used to handle and control SDAA (Selective Detect
*              And Avoid) in the scheduler.
*              The function will determine whether to schedule the
*              primary/secondary task, opening RX window for overload channel,
*              TX limitation the task with block channel.
*
* input parameters
*
* @param   nextConnTask           - ptr to the next prim task.
* @param   secTask                - ptr to the next secondary task.
* @param   startType              - the current start type of the task type.
*
* @return  the task type of the next task
*          LL_SDAA_SCHED_HANDLED  - when RX window scheduled.
*          LL_SCHED_START_IMMED   - schedule secondary task immediately.
*          LL_SCHED_START_EVENT   - schedule secondary task at start time of
*                                   the task.
*          LL_SCHED_START_PRIMARY - schedule primary task.
*/
uint8_t LL_SDAA_ControlTxAndGetStartType(taskInfo_t      *nextConnTask,
                                         taskInfo_t      *secTask,
                                         uint8           startTaskType);

/*********************************************************************
 * @fn      LL_SDAA_RecordTxUsagePeriodic
 *
 * @brief   This function records the tx usage for periodic
 *          advertising. It calculates the total transmission time
 *          based on the PHY and payload length, and updates
 *          the TX usage for the specified channel.
 *
 * input parameters
 *
 * @param   pPeriodicAdv - Pointer to the periodic advertising set structure.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  None
 */
void LL_SDAA_RecordTxUsagePeriodic(llPeriodicAdvSet_t *pPeriodicAdv);

/*********************************************************************
 * @fn      LL_SDAA_RecordTxUsageExtAdv
 *
 * @brief   This function records the tx usage for extended
 *          advertising. It calculates the total transmission time
 *          based on the PHY and payload length and updates
 *          the TX usage for the specified channel.
 *
 * input parameters
 *
 * @param   pAdvSet - Pointer to the advertising set structure.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  None
 */
void LL_SDAA_RecordTxUsageExtAdv(advSet_t *pAdvSet);

/*********************************************************************
 * @fn      LL_SDAA_RecordTxUsageConnection
 *
 * @brief   This function records the tx usage for a
 *          connection. It calculates the total transmission time
 *          based on the PHY, payload length, and encryption
 *          status and updates the TX usage for the specified channel.
 *
 * input parameters
 *
 * @param   connPtr         - Pointer to the connection state structure.
 * @param   lastPayloadLen  - Length of the last transmitted payload.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  None
 */
void LL_SDAA_RecordTxUsageConnection(llConnState_t *connPtr,
                                     uint8_t         lastPayloadLen);

/*********************************************************************
 * @fn      LL_SDAA_Init
 *
 * @brief   This function initializes the sdaa module.
 *          It sets up the channel assessment task with default
 *          parameters, resets the database, and allocates memory for
 *          dynamic observation period calculations if enabled.
 *
 * input parameters
 *
 * @param   None
 *
 * output parameters
 *
 * @param   None
 *
 * @return  None
 */
void LL_SDAA_Init(void);

/*********************************************************************
 * @fn      LL_SDAA_FreeDynamicMemory
 *
 * @brief   This function frees all dynamically allocated memory used
 *          by the SDAA module.
 *
 * input parameters
 *
 * None
 *
 * output parameters
 *
 * None
 *
 * @return  None
 */
void LL_SDAA_FreeDynamicMemory(void);

/*******************************************************************************
 * LOCAL FUNCTION DECLARATIONS
 */
