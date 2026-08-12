/******************************************************************************

 @file  ll_cs_rcl.h

 @brief CS RF funcs

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

#ifndef LL_CS_RCL_H
#define LL_CS_RCL_H
/*******************************************************************************
 * INCLUDES
 */
#include <ti/drivers/rcl/commands/ble_cs.h>
#include <ti/drivers/rcl/handlers/ble_cs.h>
#include "ti/ble/controller/ll/ll_cs_common.h"

/*******************************************************************************
 * CONSTANTS
 */

/* Number of Buffers of step type (TX) */
/* ------------------------------------*/
#define CS_STEP_TX_BUFF_NUM                    2U

/* Number of Buffers of result type (RX) */
/* ------------------------------------*/
#define CS_RESULT_RX_BUFF_NUM                  2U

/* Steps TX MultiBuffer Length*/
/* ------------------------------------------*/
/**
 * Includes the number of steps in buffer, the size of the step struct and
 * and the headers of the multibuffer */
#define CS_STEP_MULTIBUFFER_LEN                RCL_MultiBuffer_len(sizeof(RCL_CmdBleCs_Step)*CS_MAX_NUM_STEPS_IN_TX_BUFF)

/* Result RX MultiBuffer Length */
/* ------------------------------------------*/
/**
 * Considers the multibuffer header by using the macro RCL_MultiBuffer_len
 * Also considers the final HCI result header 3 bytes (opcode, eventType and length)
 */
#define CS_RESULT_MULTIBUFFER_LEN              RCL_MultiBuffer_len(252)

/// @cond NODOC
/**
 * Due to RCL-985 the central anchor point saved in the stack is 70us less that the
 * actual TX seen over the air thus the start time of the CS command should be corrected
 */
#define CS_START_TIME_CORRECTION               RAT_TICKS_IN_70US
/// @endcond // NODOC

#define CS_START_TIME_MAX_RX_WINDOW_WIDENING   0xFFFF

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * TYPEDEFS
 */

/* CS Buffers */
/*------------*/
/* Includes steps (tx) buffers, results (rx) buffers and statistics output */
typedef struct
{
    RCL_MultiBuffer       *csStepsBuff[CS_STEP_TX_BUFF_NUM];         /* Steps (tx) buffer pool */
    RCL_MultiBuffer       *csStepResultsBuff[CS_RESULT_RX_BUFF_NUM]; /* Steps (rx) buffer pool */
    RCL_CmdBleCs_Stats    csOutput;                                 /* Statistics output */
 } csBuffers_t;

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * EXTERNS
 */

/*******************************************************************************
 * FUNCTIONS
 */

/*******************************************************************************
 * @fn          llCsSubevent_PostProcess
 *
 * @brief       A subevent was completed
 * Process subevent results, prepare for next subevent, event
 * or procedure if needed.
 * End things if procedure is done (no more steps needed)
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
extern void llCsSubevent_PostProcess(void);

/*******************************************************************************
 * @fn          llCsSteps_PostProcess
 *
 * @brief       Post Process when a steps buffer was consumed
 * Get a pointer the consumed step buffer, clear it.
 * If more steps are needed, generate more steps, and init the
 * buffer again.
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
extern void llCsSteps_PostProcess(void);

/*******************************************************************************
 * @fn          llCsCurrSubEventCont_PostProcess
 *
* @brief       This function populates the steps for the CURRENT subevent.
 * It retrieves a buffer from the csBufferListForCurrentSubEvent, fills it with the steps
 * required for the current subevent, and enqueues it into the current RCL command.
 * The csBufferListForCurrentSubEvent temporarily holds buffers after they are completed
 * by the RCL command, allowing them to be repopulated with new steps and posted back
 * to the RCL command.
 * Once the steps are populated, the buffer is returned to the currently running RCL command.
 * This function is executed in the LL context.
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
void llCsCurrSubEventCont_PostProcess(void);

/*******************************************************************************
 * @fn          llCsNextSubEvent_PostProcess
 *
 * @brief       This function prepares the steps for the NEXT subevent.
 * It retrieves a buffer from the csBufferListForNextSubEvent, populates it with the steps
 * required for the next subevent, and then enqueues it into the csStepBufferList.
 * The csBufferListForNextSubEvent temporarily holds buffers between the completion of one subevent
 * and the start of the next. Once populated, the buffers are moved to the csStepBufferList,
 * which holds buffers ready for processing in the next subevent.
 * This function is executed in the LL context.
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
void llCsNextSubEvent_PostProcess(void);

/*******************************************************************************
 * @fn          llCsRcl_PrepareNextStepBuffer
 *
 * @brief       This function populatקs the steps for the next subEvent.
 * It populates the steps according to the subEvent type (new or continue) and
 * to the number of steps for current procedure.
 * When completed, the function returns the filled buffer.
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       configId - configuration Id
 * @param       subEventType
 * @param       pBuffer
 *
 * output parameters
 *
 * @param       RCL_MultiBuffer.
 *
 * @return      None
 */
RCL_MultiBuffer*  llCsRcl_PrepareNextStepBuffer(uint16 connId, uint8 configId, csSubeventType_e subEventType, RCL_MultiBuffer* pBuffer);

/*******************************************************************************
 * @fn          llCsResults_PostProcess
 *
 * @brief       Post Process when CS results are available.
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
extern void llCsResults_PostProcess(void);

/*******************************************************************************
 * @fn          llCsProcessResults
 *
 * @brief       Process CS results and notify Host
 *
 * When synchronization is lost on Mode-0 steps, we handle this by:
 *
 * 1. Setting the appropriate abort reason in the subevent results to indicate
 *    synchronization loss. This ensures the host receives accurate status information.
 *
 * 2. Preventing any continuation results from being sent for this subevent, as they
 *    would be invalid without proper synchronization. Only the initial results packet
 *    with the error status is transmitted.
 *
 * 3. If this is the final subevent in the procedure, we additionally mark the entire
 *    procedure as complete by setting the procedure done status flag. This ensures
 *    the system moves cleanly to the next procedure rather than attempting to
 *    continue with an unsynchronized state.
 *
 * input parameters
 *
 * @param       resBuf - pointer to results buffer
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsProcessResults(RCL_MultiBuffer* resBuf);

/*******************************************************************************
 * @fn          llCsSetupRcl
 *
 * @brief       Setup CS RCL command, parameters, and output registers
 *
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
 * @return      CS_STATUS_DISABLED_CONFIG_ID - if active config Id is disabled
 *              CS_STATUS_SUCCESS - otherwise
 */
csStatus_e llCsSetupRcl(uint16 connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsSetRclNsteps
 *
 * @brief       Set the number of steps for the subEvent in the CS RCL command
 *
 *
 * input parameters
 *
 * @param       nSteps - Total number of steps for this subEvent.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSetRclNsteps(uint8_t nSteps);

/*******************************************************************************
 * @fn          llCsInitRclCmd
 *
 * @brief       Initialize CS RCL command
 * This is used when a brand new procedure is started.
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
 * @return      None
 */
void llCsInitRclCmd(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsSubmitTestCmd
 *
 * @brief       Submit CS Test Command
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
 *              Success if the test command was submitted
 *              Unexpected Params if this is not a test mode
 */
csStatus_e llCsSubmitTestCmd(void);

/*******************************************************************************
 * @fn          llCsRclCallback
 *
 * @brief       CS RCL callback
 *
 * input parameters
 *
 * @param       cmd - pointer to rcl command
 * @param       lrfEvents - lrfEvents
 * @param       rclEvents - rclEvents
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsRclCallback(RCL_Command* cmd, LRF_Events lrfEvents,
                     RCL_Events rclEvents);

/*******************************************************************************
 * @fn          llCsRclAbort
 *
 * @brief       Abort the ongoing RCL command
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
void llCsRclAbort(void);

/*******************************************************************************
 * @fn          llCsClearRclBuffers
 *
 * @brief       Clear Rcl command buffers when done
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
void llCsClearRclBuffers( void );


/*******************************************************************************
 * @fn          llCsAllocCsTask
 *
 * @brief       Allocate the CS Task
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      csStatus_e
 */
csStatus_e llCsAllocCsTask( void );

/*******************************************************************************
 * @fn          llCsFreeCsTask
 *
 * @brief       Free the CS Task
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
void llCsFreeCsTask( void );

/*******************************************************************************
 * @fn          llCsTaskSetupCallback
 *
 * @brief       Setup function for the CS Task.
 *              This function will set the ProcedureDoneStatus to Active state.
 *              In case the CS task would not be scheduled for any reason,
 *              the absence of Active ProcedureDoneStatus state would trigger
 *              the CS cleanup process.
 *
 * input parameters
 *
 * @param       llTask - The task this setup function is called for
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsTaskSetupCallback( taskInfo_t *llTask );

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
 * @fn          llCsProcedureErrorSendSubEventResults
 *
 * @brief       Send the Error result to the Host
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsProcedureErrorSendSubEventResults(uint16 connId);

/*******************************************************************************
 * @fn          llCsRclIsStepBufferEmpty
 *
 * @brief       Returns fullness indication of the RCL command.
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      TRUE in case there are no buffers scheduled in current RCL command
 *              FALSE in case there are steps to be transmitted.
 */
bool llCsRclIsStepBufferEmpty(void);

#endif
