#ifndef LL_CS_PRECAL_H
#define LL_CS_PRECAL_H
/******************************************************************************

 @file  ll_cs_precal.h

 @brief This module manages the Channel Sounding Precalibration process.
 It communicates with the temperature driver, setup the precalibration command
 and handles the post processing.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025, Texas Instruments Incorporated

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
#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll_cs_common.h"
#include "ti/ble/stack_util/bcomdef.h"
#include <ti/drivers/Temperature.h>
#include <ti/drivers/rcl/commands/ble_cs.h>
#include <ti/drivers/rcl/handlers/ble_cs.h>

/*******************************************************************************
 * EXTERNS
 */

/*******************************************************************************
 * TYPEDEFS
 */
typedef struct RCL_CMD_BLE_CS_PRECAL_ENTRY_t llCsPrecal_entry_t;
typedef struct RCL_CMD_BLE_CS_PRECAL_TABLE_t llCsPrecal_table_t;
typedef struct RCL_CMD_BLE_CS_PRECAL_t       llCsPrecal_rclCmd_t;

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */

/*******************************************************************************
 * @fn          llCsPrecal_init
 *
 * @brief       Initialize Precalibration Module
 * Initialize the temperature driver, get current temperature and  register to
 * to the temperature notification.
 *
 * input parameters
 *
 * @param       cmdCallback - The CS Precal Command callback
 * @param       taskId - Task ID for the scheduler
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Initialization status
 */
uint8_t llCsPrecal_init(RCL_Callback cmdCallback, uint16_t taskId);

/*******************************************************************************
 * @fn          llCsPrecal_getTable
 *
 * @brief       Get the CS Precalibration Table
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Pointer to the CS precalibration table.
 */
llCsPrecal_table_t *llCsPrecal_getTable(void);

/*******************************************************************************
 * @fn          llCsPrecal_setupTestCmd
 *
 * @brief       Setup and execute the precal command for a CS Test
 *
 * input parameters
 *
 * @param       rclHandle - RCL handle
 *
 * output parameters
 *
 * @param       None
 *
 * @return      RCL command status
 */
RCL_CommandStatus llCsPrecal_setupTestCmd(RCL_Handle rclHandle);

/*******************************************************************************
 * @fn          llCsPrecal_postProcess
 *
 * @brief       Post Process the precalibration command
 * Checks the vality of all the bits of each entry of the precalibration table.
 * Determindes whether another precalibration is needed (partial or full)
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
void llCsPrecal_postProcess(void);

/*******************************************************************************
 * @fn          llCsPrecal_freeTask
 *
 * @brief       Free the CS Precal Task
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
void llCsPrecal_freeTask();

#endif
