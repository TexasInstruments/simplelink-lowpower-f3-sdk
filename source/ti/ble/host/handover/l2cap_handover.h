/******************************************************************************

 @file  l2cap_handover.h

 @brief This file contains the data structures and APIs for handling
        Bluetooth Low Energy handover process L2cap layer

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2025, Texas Instruments Incorporated

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

#ifndef L2CAP_HANDOVER_H
#define L2CAP_HANDOVER_H

/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/comdef.h"
#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/host/l2cap/l2cap.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */
#define HANDOVER_L2CAP_COMPONENT_SIZE         4U                                  // !< 4 bytes for the size of the component

#define L2CAP_HANDOVER_DATA_HEADER_SIZE       0x04U                               // !< The header of the l2cap header size <number of channels - 2 bytes>,
                                                                                  //                                        <number of psms - 2 bytes>,
#define L2CAP_HANDOVER_DATA_MAX_SIZE          0xFFF0U                             // !< The max l2cap handover data size
#define L2CAP_HANDOVER_TX_BLOCKED             LL_STATUS_ERROR_COMMAND_DISALLOWED  // !< The controller blocked the TX queue in order to start the handover process
#define L2CAP_HANDOVER_MODE_DISCONNECT_IMMED  0U                                  // !< Serving node mode 0 - Disconnect immediately

/*******************************************************************************
 * TYPEDEFS
 */

// The L2CAPS's Handover channel data structure
typedef struct
{
  // Channel info
  uint16_t CID;        // Local channel id
  uint8_t  state;      // Channel connection state
  uint8_t  id;         // Local identifier - matches responses with requests

  // Connection Oriented Channel info
  uint16_t credits;     // Number of LE-frames can be sent by local device
  uint16_t peerCID;     // Peer channel id

  uint16_t peerMtu;     // Maximum SDU size that can be received by peer device
  uint16_t peerMps;     // Maximum payload size that can be received by peer device

  uint16_t peerCredits; // Number of LE-frames can be sent by peer device

  // PSM info
  uint8_t psmIdx;       // Index of PSM that channel belongs to

  uint8_t status;      // Channel status
} l2capHandoverChannelData_t;

// The L2CAPS's Handover PSM data structure
typedef struct
{
  uint16_t psm;                      // Registered PSM
  uint16_t mtu;                      // The maximum size of payload data, in octets, that the upper layer entity
                                   // can accept (that is, the MTU corresponds to the maximum SDU size).
                                   // Note: This is different than ATT_MTU.
  uint16_t mps;                      // The maximum size of payload data in octets that the L2CAP layer entity
                                   // can accept (that is, the MPS corresponds to the maximum PDU payload size).
  uint16_t initPeerCredits;          // Number of LE-frames that peer device can send

  uint16_t peerCreditThreshold;      // Low threshold for peer credit count
  uint8_t maxNumChannels;            // Maximum number of CO Channels supported by PSM

  uint8_t psmIdx;                    // Index of PSM in the serving device

} l2capHandoverPsmData_t;

// The L2CAP handover data offsets
typedef enum
{
   L2CAP_NUM_CANNELS_OFFSET  = 0x00, // (uint16_t)
   L2CAP_NUM_PSMS_OFFSET     = 0x02, // (uint16_t)
   L2CAP_DATA_CANNELS_OFFSET = 0x04, // (l2capHandoverChannelData_t *)
} l2capHandoverDataOffset;

/*********************************************************************
 * FUNCTIONS
 */

/*******************************************************************************
 * @fn          L2CAP_HandoverInitSN
 *
 * @brief       Initialize the SN L2cap data.
 *
 */
void L2CAP_HandoverInitSN();

/*******************************************************************************
 * @fn          L2CAP_Handover_GetSNDataSize
 *
 * @brief       Return the l2cap layer handover data size
 *
 * @param       connHandle - the connection handle

 * @return      The l2cap layer handover data size
 */
uint32_t L2CAP_Handover_GetSNDataSize( uint16_t connHandle );

/*******************************************************************************
 * @fn          L2CAP_Handover_StartSN
 *
 * @brief       Start the handover as serving device in L2cap layer.
 *              Validate the given params, and store the buffer needs to
 *              be filled with the L2cap data.
 *
 * @param       connHandle - Connection Handle
 * @param       pBuf       - pointer to the handover buffer
 * @param       bufSize    - the available space in the buffer
 * @param       snMode     - Serving node handover mode
 *
 * @return @ref LL_STATUS_ERROR_DUE_TO_LIMITED_RESOURCES
 *         @ref LL_STATUS_ERROR_INACTIVE_CONNECTION
 *         @ref SUCCESS
 */
uint8_t L2CAP_Handover_StartSN( uint16_t connHandle, uint8_t *pBuf, uint32_t bufSize, uint8_t snMode );

/*******************************************************************************
 * @fn          L2CAP_Handover_CloseSN
 *
 * @brief       Close the Handover process on the serving node side at the L2CAP.
 *              If the handover was successful.
 *
 * @param       connHandle     - Connection Handle
 * @param       handoverStatus - Handover status received from the candidate
 *
 * @return      SUCCESS, INVALIDPARAMETER
 */
uint8_t L2CAP_Handover_CloseSN(uint16_t connHandle, uint16_t handoverStatus);

/*******************************************************************************
 * @fn          L2CAP_HandoverTriggerDataTransfer
 *
 * @brief       Start storing the L2cap layer data into the allocated buffer.
 *
 * @param       connHandle - Connection Handle
 *
 * @return      @ref LL_STATUS_ERROR_INACTIVE_CONNECTION
 *              @ref SUCCESS
 */
extern bStatus_t L2CAP_HandoverTriggerDataTransfer( void );


/*******************************************************************************
 * @fn          L2CAP_Handover_StartCN
 *
 * @brief       Starts the Handover process on the candidate node side at the L2cap.
 *              This function will save the upper layer data size and the data as copy
 *              on which the Handover should start
 *
 * @param       pHandoverData - pointer to the hadover data
 *
 * @return      SUCCESS, FAILURE, INVALID_MEM_SIZE, bleMemAllocError, bleInvalidRange
 */
bStatus_t L2CAP_Handover_StartCN(uint8_t *pHandoverData);

/*******************************************************************************
 * @fn          L2CAP_Handover_CloseCN
 *
 * @brief       Close the Handover process on the candidate node side at the L2cap.
 *
 * @return      SUCCESS
 */
bStatus_t L2CAP_Handover_CloseCN( void );

/*******************************************************************************
 * @fn          L2CAP_HandoverApplyDataCN
 *
 * @brief       This function applies the data of the candidate L2cap data -
 *              it adds the channels data to the connection handle given.
 *
 * @param       connHandle - the connection handle that been created with the handover data.
 *
 * @return      FAILURE in case the process didn't succeed; otherwise SUCCESS
 */
extern bStatus_t L2CAP_HandoverApplyDataCN(uint16_t connHandle);

/*******************************************************************************
 * @fn          l2capHandoverGetPsm
 *
 * @brief       This function returns PSM with the same details as the given data,
 *              so the channel can be created under this PSM.
 *              if the PSM doesn't exist, register a new one under the same PSM number
 *              if possible; otherwise register a new one with different number.
 *
 * @param       pHandoverPsmData - pointer to the PSM data received from the serving node.
 *
 * @return      NULL in case of failure ; or pointer to the l2cap PSM.
 */
extern l2capPsm_t *l2capHandoverGetPsm(const l2capHandoverPsmData_t *pHandoverPsmData);

/*******************************************************************************
 * @fn      l2capHandoverFindAndReplaceChannelStatus
 *
 *          This function searches for the correct channel and replaces the handover
 *          TX status
 *
 * @param   connHandle - Connection handle
 * @param   newStatus  - The new status needed to be set
 *
 * @return  SUCCESS, INVALIDPARAMETER
 *
 * @output  None
 */
uint8_t l2capHandoverFindAndReplaceChannelStatus(uint16_t connHandle, uint8_t newStatus);

/*******************************************************************************
 * @fn          l2cap_handoverAdvanceDataPointer
 *
 * @brief       This function reads the L2CAP size and advanced the pointer
 *              to the end of the L2CAP data
 *
 * @param       pHandoverData - pointer to the handover data
 *
 * @return      NULL or pointer to the new location
 */
uint8_t *l2cap_handoverAdvanceDataPointer(uint8_t *pHandoverData);

#endif /* L2CAP_HANDOVER_H */
