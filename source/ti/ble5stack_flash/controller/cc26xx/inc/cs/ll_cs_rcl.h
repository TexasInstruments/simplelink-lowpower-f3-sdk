/******************************************************************************

 @file  ll_cs_rcl.h

 @brief CS RF funcs

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

#ifndef LL_CS_RCL_H
#define LL_CS_RCL_H
/*******************************************************************************
 * INCLUDES
 */
#include <ti/drivers/rcl/commands/ble_cs.h>
#include <ti/drivers/rcl/handlers/ble_cs.h>
#include "ll_cs_common.h"

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * TYPEDEFS
 */
typedef struct
{
    struct
    {
        List_Elem __elem__;
        RCL_BufferState state; /*!< Buffer state */
        uint16_t length;       /*!< Number of bytes in the data field */
        uint16_t headIndex;    /*!< Number of bytes consumed */
        uint16_t tailIndex;    /*!< Number of bytes written */
    } header;
    RCL_CmdBleCs_Step steps[];
} ble_cs_steps_buffer_t;

typedef struct
{
    ble_cs_steps_buffer_t *csStepsBuff0;
    ble_cs_steps_buffer_t *csStepsBuff1;
    uint8_t               *csStepResultsBuff0;
    RCL_CmdBleCs_Stats    *csOutput;
    uint8                 numSteps;
    uint8                 buffsAllocated; /* flag that indicates if the buffers are allocated */
 } csRclCmdData_t;

typedef struct
{
    const LRF_Config* lrfConfigPtr;
} csLrfConfig_t;

/*******************************************************************************
 * LOCAL VARIABLES
 */
extern csLrfConfig_t csLrfConfig;

/*******************************************************************************
 * EXTERNS
 */
extern uint16 ble_cs_steps_buffer_size;
extern uint16 ble_cs_step_results_buffer_size;

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
 * @fn          llCsResults_PostProcess
 *
 * @brief       Post Process when CS results are available.
 *
 * input parameters
 *
 * @param       procedureDone - flag indicates if a procedure is done
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
extern void llCsResults_PostProcess(uint8 procedureDone);

/*******************************************************************************
 * @fn          llCsProcessResults
 *
 * @brief       Process CS results and notify Host
 *
 * input parameters
 *
 * @param       resBuf - pointer to results buffer
 * @param       isProcedureDone - is procedure done
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsProcessResults(RCL_CmdBleCs_SubeventResults* resBuf,
                        uint8 isProcedureDone);

/*******************************************************************************
 * @fn          llCsSetupRcl
 *
 * @brief       Setup CS RCL command, parameters, and output registers
 *
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       csRclDataInt - pointer to CS RCL cmd
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      CS_STATUS_DISABLED_CONFIG_ID - if active config Id is disabled
 *              CS_STATUS_SUCCESS - otherwise
 */
csStatus_e llCsSetupRcl(uint16 connId, csRclCmdData_t csRclDataInt);

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
 * @fn          llCsProcessResultsCb
 *
 * @brief       Process results callback
 *
 * input parameters
 *
 * @param       procedureDoneSt - procedure done status
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsProcessResultsCb(csProcDoneStat_e procedureDoneSt);

/*******************************************************************************
 * @fn          llCsGenerateMoreSteps
 *
 * @brief       Generate More CS Steps
 * Used when need to switch Step Buffers.
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       numSteps - number of steps to generate
 * @param       stepListBuf - pointer to the stepList
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsGenerateMoreSteps(uint16 connId, uint8 numSteps,
                           ble_cs_steps_buffer_t* stepListBuf);

/*******************************************************************************
 * @fn          llCsRclFreeTask
 *
 * @brief       Free CS Task
 * Free Steps and Steps results buffers.
 * Clear procedure flags.
 * Free the CS Task
 *
 * input parameters
 *
 * @param       connHandle - connection handle aka id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsRclFreeTask(uint16 connHandle);

#endif
