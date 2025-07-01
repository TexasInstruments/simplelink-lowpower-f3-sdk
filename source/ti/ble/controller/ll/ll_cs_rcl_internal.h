/******************************************************************************

 @file  ll_cs_rcl_internal.h

 @brief Internal functions used by ll_cs_rcl module

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2022-2025, Texas Instruments Incorporated

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
 * @param       configId - Configuration Id
 * @param       role - CS Role, initiator or reflector
 * @param       subeventCount - CS subevent counter
 * @param       rclCmd - CS RCL command
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status - Success or Failure
 */
csStatus_e llCsSetupCmdStartTime( uint16 connId, uint8_t configId, uint8 role, uint16 subEventCount, RCL_CmdBleCs rclCmd );

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
 * @fn          llCsSetFinalAntennaGpioValues
 *
 * @brief       Sets the final antennas GPIO values to be used by the RCL for
 *              antenna switching, based on a preferred antenna parameter and
 *              a given mux values per antenna.
 *              In addtion, returns a mapping for each antenna index to its final
 *              index as set in the output array.
 *
 * input parameters
 *
 * @param       prefAnt - preffered antenna bits (only 4 lsb matters).
 *                        If all bits are on, this parameter doesn't matter.
 *
 * @param       antMuxVals - Mux values to be set per antenna.
 *                           2 bits per antenna, starting from LSB.
 *
 * output parameters
 *
 * @param       outputGpioVals - Final output buffer. assumed length of 4.
 *
 * @return      Mapping of the antenna indices as set in the output array.
 *              Each index represented by 2 bits.
 *              The first antenna index mapping represented by the two LSB.
 *              The fourth antenna index mapping represented by the two MSB.
 */
uint8_t llCsSetFinalAntennaGpioValues(uint8_t prefAnt, uint8_t antMuxVals,
                                      uint8_t outputGpioVals[CS_ANTENNAS_GPIOS_ARRAY_SIZE]);