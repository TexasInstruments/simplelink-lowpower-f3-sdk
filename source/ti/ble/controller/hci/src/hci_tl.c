/******************************************************************************

 @file  hci_tl.c

 @brief This file includes implementation for HCI task, event handler,
        HCI Command, Data, and Event processing and sending, for the
        BLE Transport Layer.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2009-2025, Texas Instruments Incorporated

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

#include "ti/ble/controller/hci/hci_event.h"
#include "ti/ble/controller/hci/hci_tl.h"
#include "ti/ble/stack_util/osal/osal_bufmgr.h"
#include "ti/ble/stack_util/lib_opt/map_direct.h"
#include "ti/ble/controller/ll/ll_common.h"

extern uint8 hciPTMenabled;

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * TYPEDEFS
 */

typedef hciStatus_t (*hciFunc_t)( uint8 *pBuf );

typedef struct
{
  uint16    opCode;
  hciFunc_t hciFunc;
} hciCmdFunc_t;

typedef const hciCmdFunc_t cmdPktTable_t;

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

uint8 hciTaskID;
uint8 hciTestTaskID;
//
uint8 hciGapTaskID;
uint8 hciL2capTaskID;
uint8 hciSmpTaskID;
//
uint8 hciVsEvtMask;

/*
** HCI OSAL API
*/

/*******************************************************************************
 * This is the HCI OSAL task initialization routine.
 *
 * Public function defined in hci.h.
 */
void HCI_Init( uint8 taskID )
{
  // initialize the task for HCI-Controller
  hciTaskID      = taskID;
  hciTestTaskID  = 0;
  hciGapTaskID   = 0;
  hciL2capTaskID = 0;
  hciSmpTaskID   = 0;

  // reset the Bluetooth and the BLE event mask bits
  MAP_HCI_InitEventMasks();

  // disable PTM runtime flag
  hciPTMenabled = FALSE;

  // disable HCI Vendor Specific Event Mask
  hciVsEvtMask = NO_FEATURES_ENABLED;

  return;
}

/******************************************************************************
 * @fn      HCI_ParamUpdateRjctEvtRegister
 *
 * @brief   Register to receive incoming HCI Events for rejection of peer device
 *          Connection parameter update request.
 *
 * @param   None.
 *
 * @return  void
 */
void HCI_ParamUpdateRjctEvtRegister()
{
  hciVsEvtMask |= PEER_PARAM_REJECT_ENABLED;
}

/*******************************************************************************
 * This is the application's registration as the controller of the HCI TL.
 *
 * Public function defined in hci.h.
 */
void HCI_TL_getCmdResponderID( uint8 taskID )
{
  // Override the hciTaskID.
  hciTaskID = taskID;
}

/*******************************************************************************
 * This is the HCI OSAL task process event handler.
 *
 * Public function defined in hci.h.
 */
uint32 HCI_ProcessEvent( uint8 task_id, uint32 events )
{

  // check for system messages
  if ( events & SYS_EVENT_MSG )
  {
#if defined(HCI_TL_NONE)
    osal_event_hdr_t *pMsg;
    if (( pMsg = (osal_event_hdr_t *)osal_msg_receive(hciTaskID) ))
    {
      if (pMsg->event == HCI_HOST_TO_CTRL_DATA_EVENT)
      {
        // deallocate data
        osal_bm_free( ((hciDataPacket_t *)pMsg)->pData );
      }
      else if (pMsg->event == HCI_CTRL_TO_HOST_EVENT)
      {
        // All HCI_CTRL_TO_HOST_EVENT messages are of type hciPacket_t
        osal_bm_free( ((hciPacket_t *)pMsg)->pData );
      }
      else
      {
          /* this else clause is required, even if the
            programmer expects this will never be reached
            Fix Misra-C Required: MISRA.IF.NO_ELSE */
      }
      // deallocate the message
      (void)osal_msg_deallocate( (uint8 *)pMsg );
    }
#endif // HCI_TL_NONE

    // clear unproccessed events
    events ^= SYS_EVENT_MSG;
  }

  // return unprocessed events
  return( events );
}

/*
** HCI Vendor Specific Handlers for Host
*/

/*******************************************************************************
 * Register GAP task ID with HCI.
 *
 * Public function defined in hci.h.
 */
void HCI_TestAppTaskRegister( uint8 taskID )
{
  hciTestTaskID = taskID;
}


/*******************************************************************************
 * Register GAP task ID with HCI.
 *
 * Public function defined in hci.h.
 */
void HCI_GAPTaskRegister( uint8 taskID )
{
  hciGapTaskID = taskID;
}


/*******************************************************************************
 * Register L2CAP task ID with HCI.
 *
 * Public function defined in hci.h.
 */
void HCI_L2CAPTaskRegister( uint8 taskID )
{
  hciL2capTaskID = taskID;
}


/*******************************************************************************
 * Register SMP task ID with HCI.
 *
 * Public function defined in hci.h.
 */
void HCI_SMPTaskRegister( uint8 taskID )
{
  hciSmpTaskID = taskID;
}

/*******************************************************************************
 */
