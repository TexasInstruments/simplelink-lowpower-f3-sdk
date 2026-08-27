/******************************************************************************

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2009-2026, Texas Instruments Incorporated

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

/**
 *  @defgroup GapPAST Gap Periodic Advertising Sync Transfer
 *  @brief This module implements the Host Periodic Advertising Sync Transfer
 *  @{
 *  @file  gap_past.h
 *  @brief      GAP Periodic Advertising Sync Transfer layer interface
 */

#ifndef GAP_PAST_H
#define GAP_PAST_H

#ifdef __cplusplus
extern "C"
{
#endif

/*-------------------------------------------------------------------
 * INCLUDES
 */
#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/stack_util/osal/osal.h"

/*-------------------------------------------------------------------
 * MACROS
 */

/*-------------------------------------------------------------------
 * CONSTANTS
 */

/*-------------------------------------------------------------------
 * Structures
 */

 /// Periodic advertising sync transfer params structure
/**
 * mode            - Action to be taken when periodic advertising
 *                synchronization information is received:
 *                0x00 - No attempt is made to synchronize to the periodic
 *                       advertising and no event is sent to host
 *                0x01 - Attempt to synchronize to periodic advertising,
 *                       Event is raised if we succeed to sync or not to the host
 *                       Periodic advertisements reports will not raised to the host
 *                0x02 - Attempt to synchronize to periodic advertising,
 *                       Event is raised to if we succeed to sync or not
 *                       Periodic advertisements reports will be raised to the host
 *                       with duplicate filtering is disabled
 *                0x03 - Attempt to synchronize to periodic advertising,
 *                       Event is raised to if we succeed to sync or not
 *                       Periodic advertisements reports will be raised to the host
 *                       with duplicate filtering is enabled
 * skip         - Number of consecutive periodic advertising packets
 *                the device may skip after successfully receiving
 *                a periodic advertising packet
 * syncTimeout  - Maximum permitted time between successful receives
 * cteType      - Whether to only synchronize to periodic advertising
 *                with certain types of Constant Tone Extension
 */
typedef struct
{
  uint8_t mode;
  uint16_t skip;
  uint16_t syncTimeout;
  uint8_t cteType;
} Gap_PASTParams_t;

/// Periodic advertising sync transfer V1 event struct
typedef struct
{
  osal_event_hdr_t hdr;             //!< OSAL Event Header
  uint8_t  opcode;                  //!< GAP type of command
  uint8_t  BLEEventCode;            //!< BLE Event Code
  uint16_t connHandle;              //!< sync handle
  uint16_t serviceData;             //!< service data
  uint16_t syncHandle;              //!< Sync handle
  uint8_t  sid;                     //!< Peer SID
  uint8_t  addrType;                //!< Peer address type
  uint8_t  address[B_ADDR_LEN];     //!< Peer address
  uint8_t  phy;                     //!< Peer PHY
  uint16_t periodicInterval;        //!< Periodic interval
  uint8_t  clockAccuracy;           //!< Peer Clock Accuracy
} GapPAST_PeriodicAdvSyncTransRcvV1_t;

/// Periodic advertising sync transfer V2 event struct
typedef struct
{
  osal_event_hdr_t hdr;             //!< OSAL Event Header
  uint8_t  opcode;                  //!< GAP type of command
  uint8_t  BLEEventCode;            //!< BLE Event Code
  uint16_t connHandle;              //!< sync handle
  uint16_t serviceData;             //!< service data
  uint16_t syncHandle;              //!< Sync handle
  uint8_t  sid;                     //!< Peer SID
  uint8_t  addrType;                //!< Peer address type
  uint8_t  address[B_ADDR_LEN];     //!< Peer address
  uint8_t  phy;                     //!< Peer PHY
  uint16_t periodicInterval;        //!< Periodic interval
  uint8_t  clockAccuracy;           //!< Peer Clock Accuracy
  uint8_t  numSubevents;            //!< Number of Subevents
  uint8_t  subeventInterval;        //!< Subevent interval
  uint8_t  responseSlotDelay;       //!< Response slot delay
  uint8_t  responseSlotSpacing;     //!< Response slot spacing
} GapPAST_PeriodicAdvSyncTransRcvV2_t;

/*-------------------------------------------------------------------
 * API's
 */

/**
 * Gap_SetPeriodicAdvSyncTransferParams
 *
 * Set periodic advertising sync transfer parameters command is
 * used to specify how the controller will process periodic advertising
 * synchronization information (syncInfo) received from the device
 * identified by the connHandle parameter.
 *
 * @param       connHandle    - Connection handle.
 * @param       pastParams    - Pointer to periodic advertising sync transfer parameters.
 *
 * @return @ref SUCCESS
 * @return @ref bleInvalidRange
 */
bStatus_t Gap_SetPeriodicAdvSyncTransferParams( uint16_t connHandle, Gap_PASTParams_t *pastParams );

/**
 * Gap_SetDefaultPeriodicAdvSyncTransferParams
 *
 * Set default periodic advertising sync transfer parameters command is
 * used to specify the initial value for the periodic advertising sync
 * transfer parameters.
 *
 * @param       pastParams    - Pointer to periodic advertising sync transfer parameters.
 *
 * @return @ref SUCCESS
 * @return @ref bleInvalidRange
 */
bStatus_t Gap_SetDefaultPeriodicAdvSyncTransferParams( Gap_PASTParams_t *pastParams );

/**
 * Gap_PeriodicAdvSetInfoTransfer
 *
 * Set periodic advertising info transfer command is used to instruct
 * the controller to send synchronization information about the periodic
 * advertising in an advertising set to a connected device.
 *
 * @param       connHandle    - Connection handle.
 * @param       serviceData   - Value provided by the host for use by the Host
 *                              of the peer device.
 * @param       advHandle     - Advertising set of the periodic advertising.
 *
 * @return @ref SUCCESS
 */
bStatus_t Gap_PeriodicAdvSetInfoTransfer( uint16_t connHandle,
                                          uint16_t serviceData,
                                          uint8_t  advHandle );

/**
 * Gap_PeriodicAdvSyncTransfer
 *
 * Periodic advertising sync transfer command - used to instruct the
 * controller to send synchronization information about the periodic
 * advertising train identified by the Sync_Handle parameter to a
 * connected device.
 *
 * @param       connHandle    - Connection handle.
 * @param       serviceData   - Value provided by the host for use by the Host
 *                              of the peer device.
 * @param       syncHandle    - Sync handle of the periodic advertising train.
 *
 * @return @ref SUCCESS
 */
bStatus_t Gap_PeriodicAdvSyncTransfer( uint16_t connHandle,
                                       uint16_t serviceData,
                                       uint16_t syncHandle );
/*-------------------------------------------------------------------
-------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* GAP_PAST_H */

/** @} End GapPAST */