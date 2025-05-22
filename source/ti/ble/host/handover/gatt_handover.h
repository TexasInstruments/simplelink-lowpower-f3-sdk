/******************************************************************************

@file  gatt_handover.h

@brief This file contains handover APIs.

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

#ifndef GATT_HANDOVER_H
#define GATT_HANDOVER_H

/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/host/handover/handover.h"
#include "ti/ble/controller/ll/ll_handover_sn.h"
/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */
#define HANDOVER_GATT_COMPONENT_SIZE 4U

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
/////////////////////////////////
//     Serving Node APIs       //
/////////////////////////////////
/*******************************************************************************
 * @fn          GATT_Handover_GetSnDataSize
 *
 * @brief       Return the GATT layer handover data size
 *
 * @param       pParams - Pointer to the handover data

 * @return      The GATT layer handover data size
 */
uint32_t GATT_Handover_GetSnDataSize(handoverSNParams_t *pParams);

/*******************************************************************************
 * @fn          GATT_handover_StartSN
 *
 * @brief       Starts the handover of the GATT layer. Extract the data needed
 *              and store it in the data buffer
 *
 * @param       pParams - Pointer to the handover data
 *
 * @return      Pointer to the end of the GATT buffer
 *
 * @output      status - SUCCESS, FAILURE, bleNotConnected
 */
uint8_t *GATT_Handover_StartSN(handoverSNParams_t *pParams, uint8_t *status);

/*******************************************************************************
 * @fn          GATT_handover_CloseSN
 *
 * @brief       Close the handover of the GATT layer
 *
 * @param       connHandle - Connection handle

 * @return      @ref SUCCESS
 *              @ref FAILURE
 */
uint8_t GATT_Handover_CloseSN(uint16_t connHandle, uint32_t handoverStatus);

/*******************************************************************************
 * @fn          GATT_Handover_TriggerDataTransfer
 *
 * @brief       Stores the GATT handover data in the handover data buffer
 *
 * @param       None

 * @return      @ref SUCCESS
 *              @ref FAILURE
 */
uint8_t GATT_Handover_TriggerDataTransfer(void);

/*******************************************************************************
 * @fn          GATT_Handover_procedureStatus
 *
 * @brief       This function returns whether the GATT is in the middle of a procedure
 *
 * @param       connHandle - the connection handle
 *
 * @output      None
 *
 * @return      TRUE/FALSE
 */
uint8_t GATT_Handover_procedureStatus(uint16_t connHandle);

/////////////////////////////////
//    Candidate Node APIs      //
/////////////////////////////////
/*******************************************************************************
 * @fn          GATT_Handover_StartCN
 *
 * @brief       Starts the Candidate node GATT layer
 *
 * @param       pHandoverData - Pointer to the handover data buffer
 *
 * @return      Pointer to the new location in the buffer
 *
 * @output      status - SUCCESS, INVALIDPARAMETER
 */
uint8_t *GATT_Handover_StartCN(uint8_t *pHandoverData, uint8_t *status);

/*******************************************************************************
 * @fn          GATT_Handover_ApplyDataCN
 *
 * @brief       Applies the GATT data on candidate node side based on the
 *              connection handle allocated by the controller. This will
 *              be called only when connection complete event was received
 *              at the host
 *
 * @param       connHandle - Connection handle
 *
 * @return      @ref SUCCESS
 *              @ref FAILURE
 */
uint8_t GATT_Handover_ApplyDataCN(uint16_t connHandle);

/*******************************************************************************
 * @fn          GATT_Handover_CloseCN
 *
 * @brief       This function is used to initialize GATT handover parameters
 *              on the candidate node size
 *
 * @param       None
 *
 * @return      None
 */
void GATT_Handover_CloseCN(void);

#endif /* GATT_HANDOVER_H */
