/******************************************************************************
 @file  ll_padv_sync_transfer_receiver.h

 @brief This file dfines the periodic advertising sync transfer feature,
        enabling BLE devices to share synchronization data for improved
        efficiency. The module is activated by defining USE_PAST_RECEIVER.

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

 #ifndef LL_PADV_SYNC_TRANSFER_RECEIVER_H
 #define LL_PADV_SYNC_TRANSFER_RECEIVER_H
 #ifdef __cplusplus
 extern "C"
 {
 #endif

/*********************************************************************
 * INCLUDES
 */

#include <stdint.h>
#include "ti/ble/controller/ll/ll_ble.h"

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
 * @fn          LE_SetPASTParam
 *
 * @brief       Periodic advertising sync transfer parameters command is used to
 *              specify how the controller will process periodic advertising
 *              synchronization information (syncInfo) received from the device
 *              identified by the Connection_Handle parameter.
 *
 * @Design      BLE_LOKI-2753
 *
 * input parameters
 *
 * @param       connHandle    - Connection handle.
 * @param       mode          - Action to be taken when periodic advertising
 *                              synchronization information is received:
 *                              0x00 - No attempt is made to synchronize to the periodic
 *                                     advertising and no event is sent to host
 *                              0x01 - Attempt to synchronize to periodic advertising,
 *                                     Event is raised if we succeed to sync or not to the host
 *                                     Periodic advertisements reports will not raised to the host
 *                              0x02 - Attempt to synchronize to periodic advertising,
 *                                     Event is raised to if we succeed to sync or not
 *                                     Periodic advertisements reports will be raised to the host
 *                                     with duplicate filtering is disabled
 *                              0x03 - Attempt to synchronize to periodic advertising,
 *                                     Event is raised to if we succeed to sync or not
 *                                     Periodic advertisements reports will be raised to the host
 *                                     with duplicate filtering is enabled
 * @param       skip          - Number of consecutive periodic advertising packets
 *                              the device may skip after successfully receiving
 *                              a periodic advertising packet
 * @param       syncTimeout  - Maximum permitted time between successful receives
 * @param       cteType      - Whether to only synchronize to periodic advertising
 *                             with certain types of Constant Tone Extension
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS when validation passes.
 *              Otherwise:
 *              LL_STATUS_ERROR_UNKNOWN_CONN_HANDLE
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED
 *              LL_STATUS_ERROR_BAD_PARAMETER
 */
uint8_t LE_SetPASTParam( uint16_t  connHandle,
                         uint8_t   mode,
                         uint16_t  skip,
                         uint16_t  syncTimeout,
                         uint8_t   cteType );

/*******************************************************************************
 * @fn          LE_SetDefaultPASTParam
 *
 * @brief       Periodic advertising sync transfer set default parameters command
 *              is used to specify the initial value for the periodic advertising
 *              sync transfer parameters
 *
 * @Design      BLE_LOKI-2753
 *
 * input parameters
 *
 * @param       mode          - Action to be taken when periodic advertising
 *                              synchronization information is received:
 *                              0x00 - No attempt is made to synchronize to the periodic
 *                                     advertising and no event is sent to host
 *                              0x01 - Attempt to synchronize to periodic advertising,
 *                                     Event is raised if we succeed to sync or not to the host
 *                                     Periodic advertisements reports will not raised to the host
 *                              0x02 - Attempt to synchronize to periodic advertising,
 *                                     Event is raised to if we succeed to sync or not
 *                                     Periodic advertisements reports will be raised to the host
 *                                     with duplicate filtering is disabled
 *                              0x03 - Attempt to synchronize to periodic advertising,
 *                                     Event is raised to if we succeed to sync or not
 *                                     Periodic advertisements reports will be raised to the host
 * @param       skip          - Number of consecutive periodic advertising packets
 *                              the device may skip after successfully receiving
 *                              a periodic advertising packet
 * @param       syncTimeout  - Maximum permitted time between successful receives
 * @param       cteType      - Whether to only synchronize to periodic advertising
 *                             with certain types of Constant Tone Extension
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS when validation passes.
 *              Otherwise:
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED
 *              LL_STATUS_ERROR_BAD_PARAMETER
 */
uint8_t LE_SetDefaultPASTParam( uint8_t   mode,
                                uint16_t  skip,
                                uint16_t  syncTimeout,
                                uint8_t   cteType );

/*******************************************************************************
 * @fn          LL_PAST_R_DynamicFree
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
void LL_PAST_R_DynamicFree( void );

/*******************************************************************************
 * @fn          LL_PAST_R_Init
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
uint8_t LL_PAST_R_Init( void );

/*******************************************************************************
 * @fn          LL_PAST_R_IsEnable
 *
 * @brief       Check if the periodic advertising sync transfer feature is enabled.
 *              This function returns TRUE when USE_PAST_RECEIVER flag is set.
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
bool LL_PAST_R_IsEnable( void );

/*******************************************************************************
 * @fn          LL_PAST_R_SetFeatureBit
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
void LL_PAST_R_SetFeatureBit( void );

/*******************************************************************************
 * @fn          LL_PAST_R_ResetConnParamsToDefault
 *
 * @brief       This function resets the periodic advertising sync transfer
 *              receiver parameters for a specific connection handle to the
 *              default PAST parameters.
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
void LL_PAST_R_ResetConnParamsToDefault( uint16_t connHandle );

/*******************************************************************************
 * @fn          LL_PAST_R_SetMaxRXOctets
 *
 * @brief       Set the maximum RX octets for periodic advertising sync transfer
 *              receiver.
 *              This function updates the maximum RX octets to the length of the
 *              PAST control packets based on the PAwR support.
 *
 * input parameters
 * @param       connPtr - Connection pointer.
 *
 * output parameters
 * @param       None
 *
 * @return      None
 */
void LL_PAST_R_SetMaxRXOctets( llConnState_t  *connPtr );

/*******************************************************************************
 * @fn          LL_PAST_R_ProcessIndPackets
 *
 * @brief       Process the periodic advertising sync transfer indication packets.
 *              This function processes the received control packets for periodic
 *              advertising sync transfer, handling the creation of periodic
 *              advertising sync based on the provided parameters.
 *
 * input parameters
 *
 * @param       connPtr            - Pointer to the connection state.
 * @param       pControlPacket     - Pointer to the control packet data.
 * @param       controlPacketType  - Type of the control packet received
 *                                   (LL_PERIODIC_SYNC_IND or LL_PERIODIC_SYNC_WR_IND).
 *                                   Indicates if the periodic advertising is
 *                                   PAwr or not.
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
 void LL_PAST_R_ProcessIndPackets( llConnState_t  *connPtr,
                                   uint8_t        *pControlPacket,
                                   uint8_t        controlPacketType );

/*******************************************************************************
 * @fn          LL_PAST_R_CreateReceivedEvent
 * @brief       Create and send a periodic advertising sync transfer received event.
 *              This function constructs the event data based on the
 *              periodic advertising sync established event and the status of the
 *              periodic advertising sync transfer operation.
 *
 * input parameters
 *
 * @param       pSyncEstabEvent - Pointer to the periodic advertising sync
 *                                established event data
 * @param       eventLength     - Length of the periodic advertising sync
 *                                established event data
 * @param       status          - Status of the periodic advertising sync
 *                                transfer operation
 *
 * output parameters
 * @param       None
 *
 * @return      None
 */
 void LL_PAST_R_CreateReceivedEvent( uint8_t *pSyncEstabEvent,
                                     uint8_t eventLength,
                                     uint8_t status );

/********************************************************************************
 * @fn          LL_PAST_R_SetLastRxPacketLen
 *
 * @brief       Set the length of the last received packet from central to
 *              peripheral.
 *
 * input parameters
 *
 * @param       connPtr           - Connection pointer.
 * @param       lastRXPktLength - Length of the last packet received from
 *                                  central.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void LL_PAST_R_SetLastRxPacketLen( llConnState_t   *connPtr,
                                   uint8_t         lastRXPktLength);

#ifdef __cplusplus
}
#endif

#endif /* LL_PADV_SYNC_TRANSFER_RECEIVER_H */
