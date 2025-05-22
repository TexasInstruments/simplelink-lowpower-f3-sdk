/******************************************************************************

 @file  connection_monitor.h

 @brief This file contains interfaces for the Host Connection Monitor.

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

#ifndef CONNECTION_MONITOR_H
#define CONNECTION_MONITOR_H

/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/connection_monitor_types.h"
#include "ti/ble/stack_util/icall/app/icall.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
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

/************************************
 * Connecetion Monitor Serving APIs *
 ************************************/

/*******************************************************************************
 * @fn      CMS_RegisterCBs
 *
 * @brief   Register the application callback function at the Host for the CMS
 *
 * @param   pCBs - pointer to the callback functions
 *
 * @return  CM_SUCCESS, CM_INVALID_PARAMS, CM_UNSUPPORTED @ref cmErrorCodes_e
 */
cmErrorCodes_e CMS_RegisterCBs(const cmsCBs_t *pCBs);

/*******************************************************************************
 * @fn      CMS_GetConnDataSize
 *
 * @brief   Request the controller data size and returns the sum of
 *          the controller and host data size
 *
 * @return  The connection data size
 */
uint16_t CMS_GetConnDataSize(void);

/*******************************************************************************
 * @fn      CMS_InitConnDataParams
 *
 * @brief   Initialize the given parameter structure with default values
 *
 * @param   pParams - Pointer to the parameters structure
 *
 * @return  CM_SUCCESS, CM_UNSUPPORTED @ref cmErrorCodes_e
 *
 */
cmErrorCodes_e CMS_InitConnDataParams(cmsConnDataParams_t *pParams);

/*******************************************************************************
 * @fn      CMS_GetConnData
 *
 * @brief   Get the connection data
 *
 * @param   connHandle - the connection handle
 * @param   pData - pointer to the data buffer
 *
 * @return  CM_SUCCESS, CM_INCORRECT_MODE, CM_NOT_CONNECTED,
 *          CM_NO_RESOURCE, CM_UNSUPPORTED @ref cmErrorCodes_e
 */
cmErrorCodes_e CMS_GetConnData(uint16_t connHandle, cmsConnDataParams_t *pParams);


/************************************
 *     Connection Monitor APIs      *
 ************************************/

/*******************************************************************************
 * @fn      CM_RegisterCBs
 *
 * @brief   Register the application callback function at the Host
 *
 * @param   pCBs - pointer to the callback functions
 *
 * @return  CM_SUCCESS, CM_INVALID_PARAMS, CM_UNSUPPORTED @ref cmErrorCodes_e
 */
cmErrorCodes_e CM_RegisterCBs(const cmCBs_t *pCBs);

/*******************************************************************************
 * @fn      CM_InitStartMonitorParams
 *
 * @brief   Initialize the given parameter structure with default values
 *
 * @param   pParams - Pointer to the parameters structure
 *
 * @return  CM_SUCCESS, CM_UNSUPPORTED @ref cmErrorCodes_e
 */
cmErrorCodes_e CM_InitStartMonitorParams(cmStartMonitorParams_t *pParams);

/*******************************************************************************
 * @fn      CM_StartMonitor
 *
 * @brief   Starts the connection monitoring process.
 *
 * @param   pParams - Pointer to the monitor parameters
 *
 * @return  CM_SUCCESS, CM_NOT_CONNECTED, CM_ALREADY_REQUESTED,
 *          CM_INVALID_PARAMS, CM_NO_RESOURCE,
 *          CM_CONNECTION_LIMIT_EXCEEDED, CM_UNSUPPORTED @ref cmErrorCodes_e
 */
cmErrorCodes_e CM_StartMonitor(cmStartMonitorParams_t *pParams);

/*******************************************************************************
 * @fn      CM_StopMonitor
 *
 * @brief   Stops the connection monitoring process.
 *
 * @param   connHandle - the connection handle
 *
 * @return  CM_SUCCESS, CM_NOT_CONNECTED, CM_UNSUPPORTED @ref cmErrorCodes_e
 */
cmErrorCodes_e CM_StopMonitor(uint16_t connHandle);

/*******************************************************************************
 * @fn      CM_UpdateConn
 *
 * @brief   This function used to update monitored connection given by the CMS
 *          with the given connection data
 *
 * @param   pConnUpdateEvt - pointer to the update event
 *
 * @return  CM_SUCCESS, CM_NOT_CONNECTED, CM_ALREADY_REQUESTED,
 *          CM_INVALID_PARAMS,CM_UNSUPPORTED @ref cmErrorCodes_e
 */
cmErrorCodes_e CM_UpdateConn(cmConnUpdateEvt_t *pConnUpdateEvt);

#endif /* CONNECTION_MONITOR_H */
