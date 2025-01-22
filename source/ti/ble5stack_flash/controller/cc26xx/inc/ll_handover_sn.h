/******************************************************************************

 @file  ll_handover_sn.h

 @brief This file contains the data structures and APIs for handling
        Bluetooth Low Energy handover process

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2009-2024, Texas Instruments Incorporated

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

/*********************************************************************
 *
 * WARNING!!!
 *
 * THE API'S FOUND IN THIS FILE ARE FOR INTERNAL STACK USE ONLY!
 * FUNCTIONS SHOULD NOT BE CALLED DIRECTLY FROM APPLICATIONS, AND ANY
 * CALLS TO THESE FUNCTIONS FROM OUTSIDE OF THE STACK MAY RESULT IN
 * UNEXPECTED BEHAVIOR.
 *
 */

#ifndef LL_HANDOVER_SN_H
#define LL_HANDOVER_SN_H

/*******************************************************************************
 * INCLUDES
 */
#include "hal_types.h"
#include "ll_common.h"
#include "ll_handover.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * TYPEDEFS
 */

/*
* @brief LL Handover Start Callback
*
* @Design: BLE_LOKI-1458
*
* @note
* This callback will trigger with a success status once it will finish filling
* the handover data. Otherwise, this callback will be called with an error code
*
* @param status - the controller status (LL_STATUS_SUCCESS,
LL_STATUS_ERROR_BAD_PARAMETER, LL_STATUS_ERROR_INACTIVE_CONNECTION)
*
* @return None
*/
typedef void(*pfnLLHandoverStartSNCB_t)(uint16 connHandle, uint32_t status);

/**
 *
 * @brief LL Handover SN callbacks
 *
 * @Design: BLE_LOKI-1458
 */
typedef struct
{
  pfnLLHandoverStartSNCB_t pfnHandoverStartSNCB;
} llHandoverSNCBs_t;


/**
 * @brief LL Handover SN parameters structure
 */
typedef struct
{
  uint16_t connHandle;          //!< Connection handle
  uint32_t handoverDataSize;    //!< The stack handover data size
  uint8_t *pHandoverData;       //!< Pointer to the buffer the application allocated
  uint32_t maxNumConnEvtTries;  //!< Number of connection events the candidate will try to follow before determine the handover failed or not, 0 - try until connection supervision timeout
  uint8_t  handoverSnMode;      //!< 0 - Terminate the connection immediately on the serving node side, 1 - wait for the candidate response
} llHandoverSNParams_t;

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */

/*
 * Serving Node functions
 */

/*******************************************************************************
 * @fn          LL_Handover_RegisterSNCb
 *
 * @brief       Register to the serving node callbacks
 *
 * @Design:     BLE_LOKI-1458
 *
 * input parameters
 *
 * @param       pCBs - pointer to the callback functions
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER
 */
uint8 LL_Handover_RegisterSNCb( const llHandoverSNCBs_t *pCBs );

/*******************************************************************************
 * @fn          LL_Handover_GetSNDataSize
 *
 * @brief       Return the LL handover data size
 *
 * @Design:     BLE_LOKI-1458
 *
 * input parameters
 *
 * @param       pParams - Pointer to the parameters structure
 *
 * output parameters
 *
 * @param       None
 *
 * @return      The controller handover data size
 */
uint32 LL_Handover_GetSNDataSize( llHandoverSNParams_t *pParams );

/*******************************************************************************
 * @fn          LL_Handover_StartSN
 *
 * @brief       Starts the Handover process on the serving node side at the LL.
 *              This function will save the upper layer data size and the connection
 *              handle on which the Handover should start
 *
 * @Design:     BLE_LOKI-1458
 *
 * input parameters
 *
 * @param       pParams - Pointer to the parameters structure
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_INACTIVE_CONNECTION,
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_DUE_TO_LIMITED_RESOURCES
 */
uint8 LL_Handover_StartSN( llHandoverSNParams_t *pParams );

/*******************************************************************************
 * @fn          LL_Handover_CloseSN
 *
 * @brief       Close the Handover process on the serving node side at the LL.
 *              If the handover was successful this function will generate disconnect
 *              command complete event. Otherwise, it will re-enable the connection.
 *
 * @Design:     BLE_LOKI-1458
 *
 * input parameters
 *
 * @param       pParams        - Pointer to the serving node parameters
 * @param       handoverStatus - TRUE if the candidate was able to follow the
 *                               handover connection. Otherwise, FALSE
 *
 * @return      LL_STATUS_SUCCESS
 */
uint8 LL_Handover_CloseSN( llHandoverSNParams_t *pParams, uint8 handoverStatus );

/*******************************************************************************
 * @fn          llHandoverTriggerDataTransfer
 *
 * @brief       This function will check if the data copy can start. It will verify
 *              the TX queue is empty. If so, it will fill the data needed for the
 *              handover. At the end it will trigger the SN start CB to the upper
 *              layer with the controller status
 *
 * @Design:     BLE_LOKI-1458
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_COMMAND_DISALLOWED,
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION, LL_STATUS_ERROR_BAD_PARAMETER,
 *              FAILURE
 */
uint8_t llHandoverTriggerDataTransfer( void );

/*******************************************************************************
 * @fn          llHandoverPopulateSnData
 *
 * @brief       This function fills the buffer with the handover data
 *
 * @Design:     BLE_LOKI-1458
 *
 * input parameters
 *
 * @param       connPtr  - Connection pointer
 * @param       pContBuf - Pointer to the data buffer
 * @param       pLinkCmd - Pointer to the link command
 *
 * output parameters
 *
 * @param       pContBuf - Handover data
 *
 * @return      None
 */
void llHandoverPopulateSnData(llConnState_t *connPtr, handoverDataFull_t *pContBuf, RCL_CmdBle5Connection *pLinkCmd);

/*******************************************************************************
 * @fn          llIsHandoverInProgress
 *
 * @brief       This function return the handover progress bit for the serving node
 *
 * input parameters
 *
 * @param       connPtr - Pointer to the connection
 *
 * @return      TRUE - Handover is in progress, otherwise, FALSE
 */
uint8 llIsHandoverInProgress( llConnState_t *connPtr );

/*******************************************************************************
 * @fn          llReturnNonHandoverConn
 *
 * @brief       In case there are two connections and one of the connections is
 *              in the middle of handover process it will return the connection
 *              handle of the other connection. If non of the connections are in
 *              middle of handover process it will return invalid connection handle
 *              to allow the connection selection process to continue as usual
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Connection ID.
 */
uint16 llReturnNonHandoverConn( void );

/*******************************************************************************
 * @fn          llHandoverCheckTermConnAndTerm
 *
 * @brief       This function is used to check if the connection needed to be
 *              terminated. This function must be called after a command was
 *              finished and before the scheduler is called again
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void llHandoverCheckTermConnAndTerm( void );
#endif /* LL_HANDOVER_SN_H */
