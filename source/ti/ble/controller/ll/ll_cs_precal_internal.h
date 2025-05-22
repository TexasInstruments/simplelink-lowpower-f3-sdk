#ifndef _LL_CS_PRECAL_INTERNAL_H_
#define _LL_CS_PRECAL_INTERNAL_H_
/******************************************************************************

 @file  ll_cs_precal_internal.h

 @brief CS Precalibration module, internal functions

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
#include "ti/ble/controller/ll/ll_cs_precal.h"

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
 * @fn          llCsPrecal_setupTable
 *
 * @brief       Setup the precalibration table
 * This function iterates through the given precalibration table entries and
 * creates a new table that contains only the invalid entries in order to
 * execute precalibration on those entries.
 *
 * input parameters
 *
 * @param       precalTable - Pointer to the precalibration table to setup
 *
 * output parameters
 *
 * @param       precalTable - Pointer to the precalibration table, updated
 *
 * @return      None
 */
void llCsPrecal_setupTable(llCsPrecal_table_t *precalTable);

/*******************************************************************************
 * @fn          llCsPrecal_setupCommand
 *
 * @brief       Setup the precalibration command
 * This function prepares the precalibration command for execution.
 * It recieves the precalTable as an input and it assumes that the table has
 * been setup using llCsPrecal_setupTable
 *
 * input parameters
 *
 * @param       precalTable - Pointer to the precalibration table
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Status
 */
csStatus_e llCsPrecal_setupCommand(llCsPrecal_table_t *precalTable);

/*******************************************************************************
 * @fn          llCsPrecal_finalTable
 *
 * @brief       Setup the final precalibration table includes all entries
 * Here we setup the final precalibration table that will be used in the
 * Channel Sounding procedure. This shall done once all the 8 entries have a
 * valid high & low values.
 *
 * input parameters
 *
 * @param       precalTable - Pointer to the precalibration table
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Status
 */
void llCsPrecal_finalTable(llCsPrecal_table_t *precalTable);

/*******************************************************************************
 * @fn          llCsPrecal_registerCallback
 *
 * @brief       Register the precalibration command callback
 * Register a callback to be called when the precalibration command is done.
 *
 * input parameters
 *
 * @param       callback - Pointer to the callback function
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Status
 */
csStatus_e llCsPrecal_registerCallback(RCL_Callback callback);

/*******************************************************************************
 * @fn          llCsPrecal_initTemperature
 *
 * @brief       Initialize the temperature driver
 * Initialize the temperature driver and register to the notification.
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
 */
csStatus_e llCsPrecal_initTemperature(void);

/*******************************************************************************
 * @fn          llCsPrecal_notifyTemperatureCb
 *
 * @brief       Temperature notification callback for precal
 * This callback is triggered when the temperature changes above or below the
 * threshold, 10 degrees C.
 * It resets the valid bits of the precalibration table to 0.
 * It reregisters to the temperature notification with the current temperature.
 * It notifies the Link Layer that a precalibration is pending.
 *
 * input parameters
 *
 * @param       currentTemperature - Current temperature
 * @param       thresholdTemperature - Threshold temperature
 * @param       clientArg - Client argument - the precal table in this case
 * @param       notifyObject - Notification object
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsPrecal_notifyTemperatureCb(int16_t                currentTemperature,
                                    int16_t                thresholdTemperature,
                                    uintptr_t              clientArg,
                                    Temperature_NotifyObj *notifyObject);

/*******************************************************************************
 * @fn          llCsPrecal_reconstructEntries
 *
 * @brief       Reconstruct precalibration table entries
 * This function iterates through the given precalibration table entries and
 * and places them in their correct position within the full precalibration
 * entries. The position is based on the channel.
 * To clarify,
 * In the full table entries are:
 * {5,15,25,35,45,55,65,75}
 * The (srcTable->entry[i].channel / 10) is the index in the full table.
 * Think, hashmap.
 * @note This operation is relevant after calibration is completed.
 *
 * input parameters
 *
 * @param       srcTable - Pointer to the source precalibration table
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsPrecal_reconstructEntries(llCsPrecal_table_t *srcTable);

#endif