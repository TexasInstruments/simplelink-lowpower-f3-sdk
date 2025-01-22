/******************************************************************************

 @file  ll_cs_rcl_internal.h

 @brief Internal functions used by ll_cs_rcl module

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2022-2024, Texas Instruments Incorporated

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
#include <ti/drivers/rcl/commands/ble_cs.h>
#include <ti/drivers/rcl/handlers/ble_cs.h>
#include "ll_cs_common.h"
#include "ll_common.h"

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
 * @param       role - CS Role, initiator or reflector
 * @param       subeventCount - CS subevent counter
 * @param       rclCmd - CS RCL command
 *
 * output parameters
 *
 * @param       None.
 *
 * @return     cmdStartTime
 */
uint32_t llCsSetupCmdStartTime( uint16 connId, uint8 role, uint8 subEventCount,
                                RCL_CmdBleCs rclCmd );

/*******************************************************************************
 * @fn          llCsGetRxWidening
 *
 * @brief       Get RCL Rx Window Widening
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Window Widening
 *                  * 0 (wait forever) if Test Mode is enabled
 *                  * 250us otherwise
 */
uint16_t llCsGetRxWidening(void);

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
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      txPower
 */
RCL_Command_TxPower llCsRclGetTxPower(int8 maxTxPower);

/*******************************************************************************
 * @fn          llCsRClBufferSetup
 *
 * @brief       Setup the CS RCL double buffers.
 * Clear results buffers, init the steps and results buffers.
 * Put buffers in queue.
 *
 * input parameters
 *
 * @param       csRclDataInt - RCL command data
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsRClBufferSetup(csRclCmdData_t csRclDataInt);

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
