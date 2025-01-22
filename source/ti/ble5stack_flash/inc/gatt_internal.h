/******************************************************************************

 @file  gatt_internal.h

 @brief This file contains internal interfaces for the Generic
        Attribute Profile (GATT) module.

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
 * UNEXPECTED BEHAVIOR
 *
 */


#ifndef GATT_INTERNAL_H
#define GATT_INTERNAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "osal_cbtimer.h"

#include "att.h"
#include "gatt.h"
#include "gattservapp.h"

/*********************************************************************
 * MACROS
 */
#define TIMER_VALID( id )                ( ( (id) != INVALID_TIMER_ID ) && \
                                           ( (id) != TIMEOUT_TIMER_ID ) )

#define TIMER_STATUS( id )               ( (id) == TIMEOUT_TIMER_ID ? bleTimeout : \
                                           (id) == INVALID_TIMER_ID ? SUCCESS : blePending )

/*********************************************************************
 * CONSTANTS
 */

// ATT Flow Control Violated bit (bit 7)
#define ATT_FCV_BIT                      0x80

/*********************************************************************
 * TYPEDEFS
 */

// Function prototype to parse an attribute protocol response message
typedef bStatus_t (*gattParseRsp_t)( uint8 *pParams, uint16 len, attMsg_t *pMsg );

// Function prototype to process an attribute protocol message
typedef bStatus_t (*gattProcessMsg_t)( uint16 connHandle,  attPacket_t *pPkt, uint8 *pSafeToDealloc );

// Structure to keep Client info
typedef struct
{
  // Info maintained for Client that expecting a response back
  uint16 connHandle;           // connection message was sent out
  uint8 method;                // type of response to be received
  gattParseRsp_t pfnParseRsp;  // function to parse response to be received
  uint8 timerId;               // response timeout timer id
  uint8 taskId;                // task to be notified of response

  uint8 pendingInd;            // pending indication sent to app, and flow
                               // control violated bit (bit 7)
  // GATT Request message
  gattMsg_t req;               // request message

  // Info maintained for GATT Response message
  uint8 numRsps;               // number of responses received

  uint8 nextTransactionTaskId; // taskID of stack layer waiting to send a request.
} gattClientInfo_t;

// Structure to keep Attribute Server info
typedef struct
{
  // Info maintained for Handle Value Confirmation message
  uint16 connHandle;    // connection message was sent on
  uint8 timerId;        // confirmation timeout timer id
  uint8 taskId;         // task to be notified of confirmation

  uint8 pendingReq;     // pending request sent to app, and flow control
                        // violated bit (bit 7)
} gattServerInfo_t;

/*********************************************************************
 * VARIABLES
 */

extern uint8 gattTaskID;

/*********************************************************************
 * GATT Task Functions
 */

extern bStatus_t gattNotifyEvent( uint8 taskId, uint16 connHandle, uint8 status, uint8 method, gattMsg_t *pMsg );

extern void gattSendFlowCtrlEvt( uint16 connHandle, uint8 opcode, uint8 pendingOpcode );

extern void gattStartTimer( pfnCbTimer_t pfnCbTimer, uint8 *pData, uint16 timeout, uint8 *pTimerId );

extern void gattStopTimer( uint8 *pTimerId );

extern uint8 *gattGetPayload( gattMsg_t *pMsg, uint8 opcode );


/*********************************************************************
 * GATT Client Functions
 */

extern void gattRegisterClient( gattProcessMsg_t pfnProcessMsg );

extern bStatus_t gattGetClientStatus( uint16 connHandle, gattClientInfo_t **p2pClient );


/*********************************************************************
 * GATT Client Functions
 */

extern void gattRegisterServer( gattProcessMsg_t pfnProcessMsg );

extern bStatus_t gattGetServerStatus( uint16 connHandle, gattServerInfo_t **p2pServer );


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GATT_INTERNAL_H */
