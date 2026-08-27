/******************************************************************************
 @file  ll_padv_sync_transfer_sender.h

 @brief This file dfines the periodic advertising sync transfer feature,
        enabling BLE devices to share synchronization data for improved
        efficiency. The module is activated by defining USE_PAST_SENDER.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025-2026, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

 #ifndef LL_PADV_SYNC_TRANSFER_SENDER_H
 #define LL_PADV_SYNC_TRANSFER_SENDER_H
 #ifdef __cplusplus
 extern "C"
 {
 #endif

/*********************************************************************
 * INCLUDES
 */

#include <stdint.h>
#include "ti/ble/controller/ll/ll_common.h"

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * API's FUNCTIONS
 */

/*******************************************************************************
 * @fn          LE_PAdvSetInfoTransfer
 *
 * @brief       Periodic advertising sync transfer command is used to instruct
 *              the controller to send synchronization information about the
 *              periodic advertising in an advertising set to a connected
 *              device
 *
 * @Design      BLE_LOKI-2753
 *
 * input parameters
 *
 * @param       connHandle  - Connection handle.
 * @param       serviceData - Value provided by the host for use by the Host
 *                            of the peer device.
 * @param       advHandle   - Advertising set of the periodic advertising.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS when success.
 *              Otherwise:
 *              LL_STATUS_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED
 *              LL_STATUS_ERROR_UNKNOWN_CONN_HANDLE
 *              LL_STATUS_ERROR_UNSUPPORTED_REMOTE_FEATURE
 */
uint8_t LE_PAdvSetInfoTransfer( uint16_t  connHandle,
                                uint16_t  serviceData,
                                uint8_t   advHandle );

/*******************************************************************************
 * @fn          LE_PAdvSyncTransfer
 *
 * @brief       Periodic advertising sync transfer command is used to instruct
 *              the controller to send synchronization information about the
 *              periodic advertising train identified by the Sync_Handle parameter
 *              to a connected device
 *
 * @Design      BLE_LOKI-2753
 *
 * input parameters
 *
 * @param       connHandle  - Connection handle.
 * @param       serviceData - Value provided by the host for use by the Host
 *                            of the peer device.
 * @param       syncHandle  - Sync handle of the periodic advertising train.
 *
 * output parameters
 *
 * @param       None
 * @return      LL_STATUS_SUCCESS when success.
 *              Otherwise:
 *              LL_STATUS_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED
 *              LL_STATUS_ERROR_UNKNOWN_CONN_HANDLE
 *              LL_STATUS_ERROR_UNSUPPORTED_REMOTE_FEATURE
 */
uint8_t LE_PAdvSyncTransfer( uint16_t  connHandle,
                             uint16_t  serviceData,
                             uint16_t  syncHandle );

/*******************************************************************************
 * @fn          LL_PAST_S_DynamicFree
 *
 * @brief       This function frees all dynamically allocated memory used
 *              by the periodic adv sync transfer module.
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
void LL_PAST_S_DynamicFree( void );

/*******************************************************************************
 * @fn          LL_PAST_S_Init
 *
 * @brief       Initializes the Periodic Advertising Sync Transfer (PAST) feature.
 *              This function allocates memory for the PAST sender and receiver
 *              parameter structures and initializes them to invalid values to indicate
 *              that they have not been configured yet.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS when initialization succeeds.
 *              Otherwise:
 *              LL_STATUS_ERROR_MEM_CAPACITY_EXCEEDED
 */
uint8_t LL_PAST_S_Init( void );

/*******************************************************************************
 * @fn          LL_PAST_S_IsEnable
 *
 * @brief       Check if the periodic advertising sync transfer feature is enabled.
 *              This function returns TRUE when USE_PAST_SENDER flag is set.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      TRUE indicating that the feature is enabled.
 */
bool LL_PAST_S_IsEnable( void );

/*******************************************************************************
 * @fn          LL_PAST_S_SetFeatureBit
 *
 * @brief       Set the feature bit for periodic advertising sync transfer in the
 *              device feature set.
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
void LL_PAST_S_SetFeatureBit( void );

/*******************************************************************************
 * @fn          LL_PAST_S_ResetConnParams
 *
 * @brief       This function sets the initial values for the
 *              periodic advertising sync transfer sender and receiver parameters
 *              for a specific connection handle.
 *
 * input parameters
 *
 * @param       connHandle - Connection handle identifying the connection.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void LL_PAST_S_ResetConnParams( uint16_t connHandle );

/*******************************************************************************
* @fn          LL_PAST_S_SetupCtrlPkt
*
* @brief       Periodic advertising sync transfer setup control packet is used
*              to setup the control packet to be send
*
* @Design      BLE_LOKI-2753
*
* input parameters
*
* @param       connPtr        - Pointer to the current connection
* @param       ctrlPktType    - Packet type of the control packet to be setup.
*
* output parameters
*
* @param       pCtrlPktData   - Pointer to the control packet data payload to fill
*
* @return      None.
*/
void LL_PAST_S_SetupCtrlPkt( const llConnState_t *connPtr,
                             uint8_t             ctrlPktType,
                             uint8_t             *pCtrlPktPayload );

/*******************************************************************************
* @fn          LL_PAST_S_ProcessCtrlPacketProcedure
*
* @brief       Processes the control packet procedure for periodic advertising
*              sync transfer. This function handles tasks such as removing or
*              setting up the control packet for transmission and terminating
*              the connection in case of a timeout.
*
* input parameters
*
* @param       connPtr     - Connection pointer.
* @param       ctrlPktType - Type of the control packet to be processed.
*
* output parameters
*
* @param       None
*
* @return      LL_CTRL_PROC_STATUS_SUCCESS when processing is successful.
*              otherwise:
*              LL_CTRL_PROC_STATUS_NOT_PROCESSED
*              LL_CTRL_PROC_STATUS_TERMINATE
*/
uint8_t LL_PAST_S_ProcessCtrlPacketProcedure( llConnState_t *connPtr,
                                              uint8_t        ctrlPktType);

#ifdef __cplusplus
}
#endif

#endif /* LL_PADV_SYNC_TRANSFER_SENDER_H */
