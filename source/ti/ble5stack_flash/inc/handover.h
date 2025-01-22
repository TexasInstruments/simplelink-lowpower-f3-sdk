/******************************************************************************

 @file  handover.h

 @brief This file contains internal interfaces for the gapbondmgr.

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

#ifndef HANDOVER_H
#define HANDOVER_H

/*******************************************************************************
 * INCLUDES
 */
#include "hal_types.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * TYPEDEFS
 */
#define HANDOVER_L2CAP_COMPONENT_SIZE        4   // !< 4 bytes for the size of the component
#define HANDOVER_LL_COMPONENT_SIZE           4   // !< 4 bytes for the size of the component
// The host data header
#define HANDOVER_HEADER_SIZE                (HANDOVER_L2CAP_COMPONENT_SIZE + \
                                             HANDOVER_LL_COMPONENT_SIZE)
// Invalid connection handle, used for no connection handle
#define HANDOVER_CONNHANDLE_INVALID LL_CONNHANDLE_INVALID

// Handover Data Size defines
#define HANDOVER_HOST_DATA_SIZE     0
#define HANDOVER_INVALID_DATA_SIZE  0xFFFFFFFF

#define HANDOVER_DEFAULT_MAX_FAILED_CONN_EVENTS 6
/************************************
 *          Serving Node            *
 ************************************/

/*
* @brief Host Handover Start SN Callback
*
* @note
* This callback will trigger with a success status once it will finish filling
* the handover data. Otherwise, this callback will be called with an error code
*
* @param status - the stack status (SUCCESS, bleInvalidRange, bleNotConnected)
*
* @return None
*/
typedef void(*pfnHandoverStartSNCB_t)(uint16_t connHandle, uint32_t status);

/**
 * @brief Host Handover SN callbacks
 */
typedef struct
{
  pfnHandoverStartSNCB_t pfnHandoverStartSNCB;
} handoverSNCBs_t;

/**
 * @brief Host Handover Serving Node Configuration parameters
 */
typedef struct
{
  uint16_t connHandle;          //!< Connection handle
  uint32_t handoverDataSize;    //!< The stack handover data size
  uint8_t *pHandoverData;       //!< Pointer to the buffer the application allocated
  uint32_t maxNumConnEvtTries;  //!< Number of connection events the candidate will try to follow before determine the handover failed or not, 0 - try until connection supervision timeout
  uint8_t  handoverSnMode;      //!< 0 - Terminate the connection immediately on the serving node side, 1 - wait for the candidate response
} handoverSNParams_t;

/************************************
 *        Candidate Node            *
 ************************************/

/*
* @brief Host Handover Start CN Callback
*
* @note
* This callback will trigger with a success sta tus if the candidate was able
* to follow the new connection. Otherwise, a failure status will be returned
*
* @param status - the stack status (SUCCESS, FAILURE)
*
* @return None
*/
typedef void(*pfnHandoverStartCNCB_t)(uint16_t connHandle, uint32_t status);

/**
 * @brief Host Handover CN callbacks
 */
typedef struct
{
  pfnHandoverStartCNCB_t pfnHandoverStartCNCB;
} handoverCNCBs_t;

/**
 * @brief Host Handover Candidate Node Configuration parameters
 */
typedef struct
{
  uint8 *pHandoverData;       //!< The handover data given by the serving node
  uint8  appTaskId;           //!< Application task ID
  uint32 timeDeltaInUs;       //!< The time in us it took for the data to be transferred from the serving node to the candidate node
  uint32 timeDeltaMaxErrInUs; //!< The maximum deviation time in us
  uint16 connHandle;          //!< Connection handle given by the controller
  uint32 maxFailedConnEvents; //!< Number of connection events the candidate node will try to follow the central
                              //!< When this value is 0, it will try to follow the central device until it reaches supervision timeout
} handoverCNParams_t;

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
 *        Serving Node APIs         *
 ************************************/

/**
 * Handover_InitSNParams
 *
 * Initialize the given parameter structure with default values
 *
 * @param   pParams - Pointer to the parameters structure
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
uint8_t Handover_InitSNParams(handoverSNParams_t *pParams);

/**
 * Handover_RegisterSNCBs
 *
 * Register the application callback function at the host
 *
 * @param   pCBs - pointer to the callback functions
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
uint8_t Handover_RegisterSNCBs(const handoverSNCBs_t *pCBs);


/**
 * Handover_GetSNDataSize
 *
 * Request the controller data size and returns the sum of
 * the controller and host data size
 *
 * @param   pParams - pointer to serving node parameters
 *
 * @param   connHandle - the connection handle of the requested handover link
 *
 * @return @ref SUCCESS
 * @return @ref FAILURE
 */
uint32_t Handover_GetSNDataSize(handoverSNParams_t *pParams);

/**
 * Handover_StartSN
 *
 * Starts the Handover process on the serving node side. This function will
 * save the application data size and will call the LL command for further
 * handling
 *
 * @param   pParams - Pointer to the serving node parameters
 *
 * @return  @ref SUCCESS
 * @return  @ref bleIncorrectMode
 * @return  @ref bleNotConnected
 * @return  @ref bleAlreadyInRequestedMode
 * @return  @ref INVALIDPARAMETER
 * @return  @ref LL_STATUS_SUCCESS
 * @return  @ref LL_STATUS_ERROR_INACTIVE_CONNECTION
 * @return  @ref LL_STATUS_ERROR_COMMAND_DISALLOWED
 * @return  @ref LL_STATUS_ERROR_BAD_PARAMETER
 * @return  @ref LL_STATUS_ERROR_DUE_TO_LIMITED_RESOURCES
 *
 * NOTE: The caller is responsible to free the pBuf upon failure.
 */
uint8_t Handover_StartSN(handoverSNParams_t *pParams);

/**
 * Handover_CloseSN
 *
 * Close the Handover process on the serving node side. If the handover was
 * successful this will cause the serving node to teminate the conenction.
 * Otherwise, it will continue the connection with the peer device.
 *
 * @param   pParams - Pointer to the serving node parameters
 * @param   handoverStatus - TRUE if the candidate was able to follow the
 *                           handover connection. Otherwise, FALSE
 *
 * @return  @ref SUCCESS
 * @return  @ref bleIncorrectMode
 */
uint8_t Handover_CloseSN( handoverSNParams_t *pParams, uint16_t handoverStatus );

/************************************
 *       Candidate Node APIs        *
 ************************************/

/**
 * Handover_RegisterCNCBs
 *
 * Register the application callback function at the host
 *
 * @param   pCBs - pointer to the callback functions
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
uint8_t Handover_RegisterCNCBs(const handoverCNCBs_t *pCBs);

/**
 * Handover_InitCNParams
 *
 * Initialize the given parameter structure with default values
 *
 * @param   pParams - Pointer to the parameters structure
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
uint8_t Handover_InitCNParams(handoverCNParams_t *pParams);

/**
 * Handover_StartCN
 *
 * Starts the Handover process on the serving node side. This function will
 * save the application data size and will call the LL command for further
 * handling
 *
 * @Design: BLE_LOKI-1495
 *
 * @param   pParams - Pointer to the candidate node parameters
 *
 * @return  @ref SUCCESS
 * @return  @ref bleNoResources
 * @return  @ref bleAlreadyInRequestedMode
 * @return  @ref bleInvalidRange
 * @return  @ref LL_STATUS_SUCCESS
 * @return  @ref LL_STATUS_ERROR_CONNECTION_LIMIT_EXCEEDED
 * @return  @ref LL_STATUS_ERROR_COMMAND_DISALLOWED
 * @return  @ref LL_STATUS_ERROR_BAD_PARAMETER
 */
uint8_t Handover_StartCN( handoverCNParams_t *pParams );

#endif /* HANDOVER_H */
