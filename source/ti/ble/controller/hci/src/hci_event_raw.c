/******************************************************************************

 @file  hci_event_raw.c

 @brief This file send HCI events for the controller. It implements all the
        LL event callback and HCI events that send to host as raw message.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2026, Texas Instruments Incorporated

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
#include <string.h>
#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/controller/hci/hci_api.h"
#include "ti/ble/controller/hci/hci_event.h"
#include "ti/ble/controller/hci/hci_ext.h"
#include "ti/ble/controller/hci/hci_event_internal.h"
#include "ti/ble/controller/ll/ll_ble.h"
#include "ti/ble/stack_util/icall/app/icall_hci_tl.h"
#include "ti/ble/stack_util/lib_opt/map_direct.h"

#include "ti/ble/controller/ll/ll_cs_db.h"
#include "ti/ble/controller/ll/ll_pawr_advertiser.h"
#include <ti/drivers/utils/Math.h>
// Stub headers
#include "ti/ble/stack_util/lib_opt/ctrl_stub_connectable.h"
#include "ti/ble/stack_util/lib_opt/ctrl_stub_ble_health.h"
#include "ti/ble/stack_util/lib_opt/ctrl_stub_pscan.h"
#include "ti/ble/stack_util/lib_opt/ctrl_stub_pawr_scan.h"
#include "ti/ble/stack_util/lib_opt/ctrl_stub_pawr_advertiser.h"

/*******************************************************************************
 * MACROS
 */
#define GET_BYTE_INDEX(eventbit)         eventBit >> 3
#define GET_BIT_OFFSET(eventbit)         eventBit & 0x7

/*******************************************************************************
 * CONSTANTS
 */
// Note: Size of Extended Adv Report Less Appended two byte Vendor Specific DID field.
#define HCI_AE_EVENT_LENGTH                      (sizeof(aeExtAdvRptEvt_t) - sizeof(((aeExtAdvRptEvt_t *)0)->pData) - sizeof(uint16))
#define MAX_REPORT_DATA_SIZE                     200
#define HCI_MAX_EVT_PKT_SIZE                     0xFF          //!< Max event Packet Size
#define HCI_MAX_EVT_PKT_SIZE_EXT                 0xFFFF        //!< Max ext event Pakcet Size
// Maximum response data that can fit in one PAwR response report event
#define HCI_PADVA_MAX_SINGLE_RSP_DATA            (MAX_REPORT_DATA_SIZE - HCI_PADVA_RESPONSE_REPORT_EVENT_BASE_LEN - HCI_PADVA_RESPONSE_REPORT_ENTRY_BASE_LEN)

// Module Id Used to report system event (error or other)
#define HCI_MODULE_ID                            0

#define HCI_EVENT_MASK_NUM_OF_TABLES             3
#define HCI_EVENT_MASK_BLE                       0
#define HCI_EVENT_MASK_PAGE1                     1
#define HCI_EVENT_MASK_PAGE2                     2
/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL FUNCTIONS PROTOTYPES
 */
void hciSendVendorSpecificEvent( uint8 *pBuf, uint16 dataLen );
void hciSendSystemReport( uint8_t status, uint16_t info );

uint8* hciAllocAndPrepExtHciEvtPkt( uint8 **pData, uint16 hciPktLen );
uint8* hciAllocAndPrepHciEvtPkt( uint8 **pData, uint8 hciEvtType,
                                 uint8 hciPktLen );
uint8* hciAllocAndPrepHciLeEvtPkt( uint8 **pData, uint8 hciLeEvtType,
                                   uint8 hciPktLen );

uint16 HCI_getPacketLen( hciPacket_t *pEvt );

void hciCreateEventExtAdvSetTerminated( aeAdvSetTerm_t *pEvtData );
void hciCreateEventExtScanReqReceived( aeScanReqReceived_t *extAdvRpt );
void hciCreateEventExtAdv( uint8 eventId, uint8 handle );
void hciCreateEventExtAdvDataTruncated( aeAdvTrucData_t *extAdvTrunc );
void hciCreateEventExtAdvReport( aeExtAdvRptEvt_t *pExtAdvRpt );
void hciCreateEventAdvReport( aeExtAdvRptEvt_t *pExtAdvRpt );
void hciCreateEventExtScanTimeout();
void hciCreateEventExtScan( uint8 event );

uint8 hciCheckEventMask( uint8 eventBit, uint8 eventMaskTableIndex );
uint8 hciSetEventMask( uint8 *pEventMask, uint8 eventMaskTableIndex );

/*******************************************************************************
 * LOCAL VARIABLES
 */
static const hciControllerToHostCallbacks_t *pHciC2HCbs = NULL;

/*******************************************************************************
 * EXTERNAL FUNCTIONS
 */
extern void osal_bm_free( void *payload_ptr );

/*******************************************************************************
 * GLOBAL VARIABLES
 */

uint8 hciEvtMaskLe[B_EVENT_MASK_LEN]    = {0};
uint8 hciEvtMaskPage1[B_EVENT_MASK_LEN] = {0};
uint8 hciEvtMaskPage2[B_EVENT_MASK_LEN] = {0};

uint8 *hciEvtMask[HCI_EVENT_MASK_NUM_OF_TABLES] =
{
 hciEvtMaskLe,
 hciEvtMaskPage1,
 hciEvtMaskPage2,
};

/*********************************************************************
 * @fn      HCI_ControllerToHostSendCallbackEvent
 *
 * @brief   Send event to the host via proprietary callback.
 *          The callback will be executed in the caller context
 *          (No context switch will be done here)
 *
 * @param   pData          - a pointer to the data to parse.
 *          callbackFctPtr - function pointer that will parse the message.
 *
 * @return  status:
 *            true: always return TRUE
 */
static uint8_t HCI_ControllerToHostSendCallbackEvent(void *pData, void* callbackFctPtr)
{
  ((void (*)(void*))(callbackFctPtr))(pData);

  return TRUE;
}

/*******************************************************************************
 * @fn          HCI_CommandStatusCb
 *
 * @brief       This function is a wrapper to a callback provided by the Host.
 *              It was created to align the typecasts of the HCI_TL_CommandStatusCB_t
 *              and the pHciC2HCbs->send (the return type is different).
 *
 * input parameters
 *
 * @param       pBuf - Pointer to an HCI packet.
 * @param       len  - Length of the HCI packet.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      void
 */
static void HCI_CommandStatusCb(uint8_t *pBuf, uint16_t len)
{
  if (( NULL != pHciC2HCbs ) && ( NULL != pHciC2HCbs->send ))
  {
    (void) pHciC2HCbs->send(pBuf, len);
  }
}

/********************************************************************************
 * @fn            BLE_ServicesParamsInit
 *
 * @brief         The `BLE_ServicesParamsInit` function provides a default
 *                configuration for the bleServicesParams_t structure
 *
 * input parameters
 *
 * @param         pController2HostCallbacks - pointer to Controller-to-Host
 *                callbacks interface
 *
 * output parameters
 *
 * @return        SUCCESS - in case pServiceParams initialization succeed
 *                FAILURE in case of
 *                   - Compatibility/versions mismatch (the check is done based
 *                     on size of the hciControllerToHostCallbacks_t).
 *                   - Parameters validation
 *
 * */
uint32_t HCI_ControllerToHostCallbacksInit(hciControllerToHostCallbacks_t *pController2HostCallbacks)
{
  uint32_t status = FAILURE;

  if ( NULL != pController2HostCallbacks )
  {
    (void) memset(pController2HostCallbacks, 0, sizeof(hciControllerToHostCallbacks_t));

    status = SUCCESS;
  }

  return status;
}

/*******************************************************************************
 * @fn          HCI_ControllerToHostRegisterCb
 *
 * @brief       This function registers Host callbacks for HCI module
 *
 * input parameters
 *
 * @param       hciControllerToHostCallbacks_t cbs - pointer to the callbacks structure.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      SUCCESS / FAILURE.
 */
uint32_t HCI_ControllerToHostRegisterCb( const hciControllerToHostCallbacks_t *pCbs )
{
  uint32_t status = FAILURE;

  if ( NULL != pCbs )
  {
    pHciC2HCbs      = pCbs;

    HCI_TL_Init(NULL, HCI_CommandStatusCb, HCI_ControllerToHostSendCallbackEvent, 0);

    status           = SUCCESS;
  }

  return status;
}

/*******************************************************************************
 * @fn          HCI_SendEventToHost
 *
 * @brief       This function is responsible for sending the prepared event
 *              packet to the host. If a callback function is registered, the
 *              message will be sent to the callback function,
 *              otherwise, it will be sent to hci_task.
 *
 * input parameters
 *
 * @param       *pEvt -Pointer to prepared HCI event packet
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void HCI_SendEventToHost( uint8 *pEvt )
{
  if ( pEvt != NULL )
  {
    if (( NULL != pHciC2HCbs ) && ( NULL != pHciC2HCbs->send ))
    {
        hciPacket_t *pMsg = (hciPacket_t *)( pEvt );
        uint16 pktLen = HCI_getPacketLen( pMsg );

        (void) pHciC2HCbs->send( pMsg->pData, pktLen );

        if( pMsg->pData[0] == (uint8) HCI_ACL_DATA_PACKET )
        {
          MAP_osal_bm_free( pMsg->pData );
          pMsg->pData = NULL;
        }
        MAP_osal_msg_deallocate( pEvt );
    }
    else
    {
      // send the message
      (void) MAP_osal_msg_send( hciTaskID, pEvt );
    }
  }
}

/*******************************************************************************
 * Event masks API functions Init/Setters/Checkers
 */

/*******************************************************************************
 * @fn          HCI_InitEventMasks
 *
 * @brief       This routine initializes Bluetooth and BLE event masks to their
 *              default values.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void HCI_InitEventMasks( void )
{
  // Set default Bluetooth event mask bits
  hciEvtMaskPage1[0] = BT_EVT_MASK_BYTE0;
  hciEvtMaskPage1[1] = BT_EVT_MASK_BYTE1;
  hciEvtMaskPage1[2] = BT_EVT_MASK_BYTE2;
  hciEvtMaskPage1[3] = BT_EVT_MASK_BYTE3;
  hciEvtMaskPage1[4] = BT_EVT_MASK_BYTE4;
  hciEvtMaskPage1[5] = BT_EVT_MASK_BYTE5;
  hciEvtMaskPage1[6] = BT_EVT_MASK_BYTE6;
  hciEvtMaskPage1[7] = BT_EVT_MASK_BYTE7;

  // Set default Bluetooth event mask page 2 bits
  hciEvtMaskPage2[0] = BT_EVT_MASK2_BYTE0;
  hciEvtMaskPage2[1] = BT_EVT_MASK2_BYTE1;
  hciEvtMaskPage2[2] = BT_EVT_MASK2_BYTE2;
  hciEvtMaskPage2[3] = BT_EVT_MASK2_BYTE3;
  hciEvtMaskPage2[4] = BT_EVT_MASK2_BYTE4;
  hciEvtMaskPage2[5] = BT_EVT_MASK2_BYTE5;
  hciEvtMaskPage2[6] = BT_EVT_MASK2_BYTE6;
  hciEvtMaskPage2[7] = BT_EVT_MASK2_BYTE7;

  // Set default BLE event mask bits
  hciEvtMaskLe[0] = LE_EVT_MASK_BYTE0;
  hciEvtMaskLe[1] = LE_EVT_MASK_BYTE1;
  hciEvtMaskLe[2] = LE_EVT_MASK_BYTE2;
  hciEvtMaskLe[3] = LE_EVT_MASK_NONE;
  hciEvtMaskLe[4] = LE_EVT_MASK_BYTE4;
  hciEvtMaskLe[5] = LE_EVT_MASK_NONE;
  hciEvtMaskLe[6] = LE_EVT_MASK_NONE;
  hciEvtMaskLe[7] = LE_EVT_MASK_NONE;

}

/*******************************************************************************
 * @fn          HCI_SetEventMaskPage1
 *
 * @brief       This API is used to set the HCI event mask page 1, which is
 *              used to determine which events are supported.
 *
 * input parameters
 *
 * @param      *pEventMask          - Event mask to be copy
 *              eventMaskTableIndex - Which table to search in
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      SUCCESS/FAILURE
 */
uint8 HCI_SetEventMaskPage1( uint8 *pEventMask )
{
  return (hciSetEventMask( pEventMask, HCI_EVENT_MASK_PAGE1 ));
}

/*******************************************************************************
 * @fn          HCI_SetEventMaskPage2
 *
 * @brief       This API is used to set the HCI event mask page 2, which is
 *              used to determine which events are supported.
 *
 * input parameters
 *
 * @param      *pEventMask          - Event mask to be copy
 *              eventMaskTableIndex - Which table to search in
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      SUCCESS/FAILURE
 */
uint8 HCI_SetEventMaskPage2( uint8 *pEventMask )
{
  return (hciSetEventMask( pEventMask, HCI_EVENT_MASK_PAGE2 ));
}

/*******************************************************************************
 * @fn          HCI_SetEventMaskLe
 *
 * @brief       This API is used to set the LE HCI event mask, which is
 *              used to determine which LE events are supported.
 *
 * input parameters
 *
 * @param      *pEventMask          - Event mask to be copy
 *              eventMaskTableIndex - Which table to search in
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      SUCCESS/FAILURE
 */
uint8 HCI_SetEventMaskLe( uint8 *pEventMask )
{
  return (hciSetEventMask( pEventMask, HCI_EVENT_MASK_BLE));
}

/*******************************************************************************
 * @fn          HCI_CheckEventMaskPage1
 *
 * @brief       This API is used to check if input event bit is enable (page1)
 *
 * input parameters
 *
 * @param       eventBit            - Which bit to check
 *              eventMaskTableIndex - Which table to search in
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      TRUE/FALSE.
 */
uint8 HCI_CheckEventMaskPage1( uint8 eventBit )
{
  return (hciCheckEventMask( eventBit, HCI_EVENT_MASK_PAGE1 ));
}

/*******************************************************************************
 * @fn          HCI_CheckEventMaskPage2
 *
 * @brief       This API is used to check if input event bit is enable (page2)
 *
 * input parameters
 *
 * @param       eventBit            - Which bit to check
 *              eventMaskTableIndex - Which table to search in
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      TRUE/FALSE.
 */
uint8 HCI_CheckEventMaskPage2( uint8 eventBit )
{
  return (hciCheckEventMask( eventBit, HCI_EVENT_MASK_PAGE2 ));
}

/*******************************************************************************
 * @fn          HCI_CheckEventMaskLe
 *
 * @brief       This API is used to check if input LE event bit is enable
 *
 * input parameters
 *
 * @param       eventBit            - Which bit to check
 *              eventMaskTableIndex - Which table to search in
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      TRUE/FALSE.
 */
uint8 HCI_CheckEventMaskLe( uint8 eventBit )
{
  return (hciCheckEventMask( eventBit, HCI_EVENT_MASK_BLE ));
}

/*
** HCI Controller Events
*/

/*******************************************************************************
 * @fn          HCI_SendCommandStatusEvent
 *
 * @brief       This generic function sends a Command Status event to the Host.
 *              It is provided as a direct call so the Host can use it directly.
 *
 * input parameters
 *
 * @param       eventCode - The event code.
 * @param       status    - The resulting status of the command.
 * @param       opcode    - The opcode of the command that generated this event.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void HCI_SendCommandStatusEvent( uint8 eventCode, uint16 status, uint16 opcode )
{
  uint8 data[HCI_CMD_STATUS_EVENT_LEN];

  data[0] = LO_UINT16(status);
  data[1] = 1;                   // number of HCI command packets
  data[2] = LO_UINT16( opcode ); // opcode (LSB)
  data[3] = HI_UINT16( opcode ); // opcode (MSB)

  MAP_HCI_SendControllerToHostEvent( eventCode, HCI_CMD_STATUS_EVENT_LEN,
                                     data );
}

/*******************************************************************************
 * @fn          HCI_SendControllerToHostEvent
 *
 * @brief       This generic function sends a Controller to Host Event.
 *
 * input parameters
 *
 * @param       eventCode - Bluetooth event code.
 * @param       dataLen   - Length of dataField.
 * @param       pData     - Pointer to data.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void HCI_SendControllerToHostEvent( uint8 eventCode, uint8 dataLen,
                                    uint8 *pPayload )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  pEvt = hciAllocAndPrepHciEvtPkt( &pData, eventCode, dataLen );

  if ( pEvt )
  {

    if ( dataLen > 0 )
    {
      // Copy data
      memcpy( pData, pPayload, dataLen );
    }

    // Send message
    HCI_SendEventToHost( pEvt );
  }
}

/*******************************************************************************
 * @fn          HCI_DataBufferOverflowEvent
 *
 * @brief       This function sends the Data Buffer Overflow Event to the Host.
 *
 * input parameters
 *
 * @param       linkType - HCI_LINK_TYPE_SCO_BUFFER_OVERFLOW,
 *                         HCI_LINK_TYPE_ACL_BUFFER_OVERFLOW
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void HCI_DataBufferOverflowEvent( uint8 linkType )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  pEvt = hciAllocAndPrepHciEvtPkt( &pData,
  HCI_DATA_BUFFER_OVERFLOW_EVENT,
                                   HCI_BUFFER_OVERFLOW_EVENT_LEN );
  if ( pEvt )
  {
    // Fill data
    *pData = linkType;

    // send message
    HCI_SendEventToHost( pEvt );
  }
}

/*******************************************************************************
 * @fn          LL_ReadRemoteVersionInfoCback
 *
 * @brief       This LL callback is used to generate a Read Remote Version
 *              Information Complete meta event when a Central makes this request
 *              of a Central.
 *
 * input parameters
 *
 * @param       status     - Status of callback.
 * @param       verNum     - Version of the Bluetooth Controller specification.
 * @param       connHandle - Company identifier of the manufacturer of the
 *                           Bluetooth Controller.
 * @param       subverNum  - A unique value for each implementation or revision
 *                           of an implementation of the Bluetooth Controller.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_ReadRemoteVersionInfoCback( hciStatus_t status, uint16 connHandle,
                                    uint8 verNum, uint16 comId,
                                    uint16 subverNum )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskPage1( BT_EVT_READ_REMOTE_VERSION_INFO_BIT ) )
  {
    pEvt = hciAllocAndPrepHciEvtPkt( &pData,
                                     HCI_READ_REMOTE_INFO_COMPLETE_EVENT_CODE,
                                     HCI_REMOTE_VERSION_INFO_EVENT_LEN );
    if ( pEvt )
    {

      *pData++ = status;
      *pData++ = LO_UINT16( connHandle );
      *pData++ = HI_UINT16( connHandle );
      *pData++ = verNum;
      *pData++ = LO_UINT16( comId );
      *pData++ = HI_UINT16( comId );
      *pData++ = LO_UINT16( subverNum );
      *pData = HI_UINT16( subverNum );

      // send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          LL_DirectTestEndDone Callback
 *
 * @brief       This Callback is used by the LL to notify the HCI that the
 *              Direct Test End command has completed.
 *
 *
 * input parameters
 *
 * @param       numPackets - The number of packets received. Zero for transmit.
 * @param       mode       - LL_DIRECT_TEST_MODE_TX or LL_DIRECT_TEST_MODE_RX.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
void LL_DirectTestEndDoneCback( uint16 numPackets, uint8 mode )
{
  uint8 rtnParam[HCI_DIRECT_TEST_END_LEN];

  // unused input parameter; PC-Lint error 715.
  (void) mode;

  rtnParam[0] = HCI_SUCCESS;

  rtnParam[1] = LO_UINT16( numPackets );
  rtnParam[2] = HI_UINT16( numPackets );

  HCI_CommandCompleteEvent( HCI_LE_TEST_END, HCI_DIRECT_TEST_END_LEN,
                            rtnParam );
}

/*******************************************************************************
 * @fn          HCI_HardwareErrorEvent_raw
 *
 * @brief       This function sends a Hardware Error Event to the Host.
 *
 * input parameters
 *
 * @param       hwErrorCode - The hardware error code.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void HCI_HardwareErrorEvent_raw( uint8 hwErrorCode )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskPage1( BT_EVT_HARDWARE_ERROR_BIT ) )
  {
    pEvt = hciAllocAndPrepHciEvtPkt( &pData,
                                     HCI_BLE_HARDWARE_ERROR_EVENT_CODE,
                                     HCI_HARDWARE_ERROR_EVENT_LEN );

    if ( pEvt )
    {
      // Fill data
      *pData = hwErrorCode;

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          HCI_SendCommandCompleteEvent
 *
 * @brief       This generic function sends a Command Complete or a Vendor
 *              Specific Command Complete Event to the Host.
 *
 * input parameters
 *
 * @param       eventCode - The event code.
 * @param       opcode    - The opcode of the command that generated this event.
 * @param       numParam  - The number of parameters in the event.
 * @param       param     - The event parameters associated with the command.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void HCI_SendCommandCompleteEvent( uint8 eventCode, uint16 opcode,
                                   uint8 numParam, uint8 *param )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;
  uint8 hciEvtType = eventCode;
  uint8 hciPktLen = numParam;

  hciPktLen +=
                ((eventCode != HCI_VE_EVENT_CODE) ?
                                          HCI_CMD_COMPLETE_EVENT_LEN :
                                          HCI_CMD_VS_COMPLETE_EVENT_LEN);

  pEvt = hciAllocAndPrepHciEvtPkt( &pData, hciEvtType, hciPktLen );

  if ( pEvt )
  {
    // check if this isn't a vendor specific event
    if ( eventCode != HCI_VE_EVENT_CODE )
    {
      *pData++ = 1;                   // event parameter 1
      *pData++ = LO_UINT16( opcode ); // event parameter 2
      *pData++ = HI_UINT16( opcode ); // event parameter 2

      // Remaining event parameters
      memcpy( pData, param, numParam );
    }
    else // it is a vendor specific event
    {
      *pData++ = param[0];            // event parameter 0: event opcode LSB
      *pData++ = param[1];            // event parameter 1: event opcode MSB
      *pData++ = param[2];            // event parameter 2: status
      *pData++ = LO_UINT16( opcode ); // event parameter 3: command opcode LSB
      *pData++ = HI_UINT16( opcode ); // event parameter 3: command opcode MSB

      // Remaining event parameters
      // Note: The event opcode and status were already placed in the msg packet.
      memcpy( pData, &param[3], numParam - HCI_EVENT_MIN_LENGTH );
    }

    // Send message
    HCI_SendEventToHost( pEvt );
  }
}

/*******************************************************************************
 * @fn          LL_ReadRemoteUsedFeaturesCompleteCback Callback
 *
 * @brief       This LL callback is used to generate a Read Remote Used Features
 *              Complete meta event when a Central makes this request of a Central.
 *
 * input parameters
 *
 * @param       status     - HCI status.
 * @param       connHandle - Connection handle.
 * @param       featureSet - Pointer to eight byte bit mask of LE features.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_ReadRemoteUsedFeaturesCompleteCback( hciStatus_t status,
                                             uint16 connHandle,
                                             uint8 *featureSet )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_READ_REMOTE_FEATURE_BIT ) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_READ_REMOTE_FEATURE_COMPLETE_EVENT,
                                       HCI_READ_REMOTE_FEATURE_COMPLETE_EVENT_LEN );
    if ( pEvt )
    {

      *pData++ = status;
      *pData++ = LO_UINT16( connHandle );
      *pData++ = HI_UINT16( connHandle );

      // Fill Feature set
      memcpy( pData, featureSet, B_FEATURE_SUPPORT_LENGTH );

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*********************************************************************
 * @fn          HCI_AeAdvCback
 *
 * @brief       Callback for the AE advertising event send by LL.
 *              This function will send an event to the host
 *
 * @param       event  - event trigging the callback.
 * @param       pData  - Pointer to the data that comes with the event
 *                      (this is an union).
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
*/
void HCI_AeAdvCback( uint8 event, void *pData )
{
  switch ( event )
  {
    case LL_CBACK_ADV_SET_TERMINATED:
    {
      hciCreateEventExtAdvSetTerminated( (aeAdvSetTerm_t*) pData );
      break;
    }
    case LL_CBACK_EXT_SCAN_REQ_RECEIVED:
    {
      hciCreateEventExtScanReqReceived( (aeScanReqReceived_t*) pData );
      break;
    }
    case LL_CBACK_ADV_START_AFTER_ENABLE:
    case LL_CBACK_ADV_END_AFTER_DISABLE:
    case LL_CBACK_ADV_START:
    case LL_CBACK_ADV_END:
    {
      uint8 handle = *((uint8*) pData);
      hciCreateEventExtAdv( event, handle );
      break;
    }
    case LL_CBACK_EXT_ADV_DATA_TRUNCATED:
    {
      hciCreateEventExtAdvDataTruncated( (aeAdvTrucData_t*) pData );
      break;
    }
    case LL_CBACK_OUT_OF_MEMORY:
    {
      hciSendSystemReport( LL_STATUS_ERROR_OUT_OF_HEAP, HCI_LE_EVENT_CODE );
      break;
    }
    default:
    {
      hciSendSystemReport( LL_STATUS_ERROR_UNKNOWN_ADV_EVT_TYPE,
                           HCI_BLE_EXTENDED_ADV_REPORT_EVENT );
      break;
    }
  }
}

/*********************************************************************
 * @fn          HCI_AeAdvCback
 *
 * @brief       Callback for the AE scan event send by LL.
 *              This function will send an event to the host
 *
 * @param       event  - event trigging the callback.
 * @param       pData  - Pointer to the data that comes with the event
 *                       (this is an union).
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
*/
void HCI_AeScanCback( uint8 event, void *pData )
{
  switch ( event )
  {
    case LL_CBACK_EXT_ADV_REPORT:
    {
      hciCreateEventExtAdvReport( (aeExtAdvRptEvt_t*) pData );
      break;
    }
    case LL_CBACK_EXT_SCAN_TIMEOUT:
    {
      hciCreateEventExtScanTimeout( );
      break;
    }
    case LL_CBACK_EXT_SCAN_START:
    case LL_CBACK_EXT_SCAN_PERIOD_END:
    case LL_CBACK_EXT_SCAN_END:
    case LL_CBACK_EXT_SCAN_WINDOW_END:
    case LL_CBACK_EXT_SCAN_INTERVAL_END:
    case LL_CBACK_EXT_SCAN_DURATION_END:
    {
      hciCreateEventExtScan( event );
      break;
    }
    default:
    {
      //Unknown Event, just free the message.
      hciSendSystemReport( LL_STATUS_ERROR_UNKNOWN_ADV_EVT_TYPE,
                           HCI_EXT_LE_SCAN_EVENT );
      break;
    }
  }
}

/*******************************************************************************
 * @fn          HCI_LegacyScanCback
 *
 * @brief       Callback for legacy (BT4.x) scanning events from the Link Layer.
 *              Converts extended advertising report format to legacy format
 *              for backward compatibility.
 *
 * input parameters
 *
 * @param       event - Event type (LL_CBACK_EXT_ADV_REPORT, etc.)
 * @param       pData - Pointer to event data (aeExtAdvRptEvt_t for adv reports)
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void HCI_LegacyScanCback( uint8 event, void *pData )
{
  switch ( event )
  {
    case LL_CBACK_EXT_ADV_REPORT:
    {
      // Convert extended advertising report to legacy format
      hciCreateEventAdvReport( (aeExtAdvRptEvt_t*) pData );
      break;
    }
    case LL_CBACK_EXT_SCAN_TIMEOUT:
    {
      // Legacy scan timeout event (same as extended)
      hciCreateEventExtScanTimeout( );
      break;
    }
    case LL_CBACK_EXT_SCAN_START:
    case LL_CBACK_EXT_SCAN_PERIOD_END:
    case LL_CBACK_EXT_SCAN_END:
    case LL_CBACK_EXT_SCAN_WINDOW_END:
    case LL_CBACK_EXT_SCAN_INTERVAL_END:
    case LL_CBACK_EXT_SCAN_DURATION_END:
    {
      // Legacy scan events (same as extended)
      hciCreateEventExtScan( event );
      break;
    }
    default:
    {
      // Unknown event, report error
      hciSendSystemReport( LL_STATUS_ERROR_UNKNOWN_ADV_EVT_TYPE,
                           HCI_EXT_LE_SCAN_EVENT );
      break;
    }
  }
}

/*******************************************************************************
 * @fn          HCI_CS_ReadRemoteSupportedCapabilitiesCback
 *
 * @brief       Callback for reading the remote CS capabilities.
 *
 * input parameters
 *
 * @param       status - status
 * @param       connHandle - connection handle
 * @param       peerCapabilities- peerCapabilities
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
WEAK_FUNC void HCI_CS_ReadRemoteSupportedCapabilitiesCback(uint8 status,
                                                           uint16 connHandle,
                                                           const llCsCapabilities_t* peerCapabilities)
{
  uint8* pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8* pData;

  pEvt = hciAllocAndPrepHciLeEvtPkt(
    &pData, HCI_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES_COMPLETE_EVENT,
    HCI_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES_COMPLETE_EVENT_LEN);

  if (NULL != pEvt)
  {
    *pData++ = status;                // status
    *pData++ = LO_UINT16(connHandle); // connection handle (LSB)
    *pData++ = HI_UINT16(connHandle); // connection handle (MSB)
    *pData++ = peerCapabilities->numConfig;
    *pData++ = LO_UINT16(peerCapabilities->maxProcedures);
    *pData++ = HI_UINT16(peerCapabilities->maxProcedures);
    *pData++ = peerCapabilities->numAntennas;
    *pData++ = peerCapabilities->maxAntPath;
    *pData++ = peerCapabilities->role;
    *pData++ = peerCapabilities->optionalModes;
    *pData++ = peerCapabilities->rttCap;
    *pData++ = peerCapabilities->rttAAOnlyN;
    *pData++ = peerCapabilities->rttSoundingN;
    *pData++ = peerCapabilities->rttRandomPayloadN;
    *pData++ = LO_UINT16(peerCapabilities->nadmSounding);
    *pData++ = HI_UINT16(peerCapabilities->nadmSounding);
    *pData++ = LO_UINT16(peerCapabilities->nadmRandomSeq);
    *pData++ = HI_UINT16(peerCapabilities->nadmRandomSeq);
    *pData++ = peerCapabilities->optionalCsSyncPhy;
    *pData++ = LO_UINT16( peerCapabilities->noFAE   << 1 |
                          peerCapabilities->chSel3c << 2 |
                          peerCapabilities->csBasedRanging << 3);
    *pData++ = HI_UINT16(0);
    *pData++ = LO_UINT16(peerCapabilities->tIp1Cap);
    *pData++ = HI_UINT16(peerCapabilities->tIp1Cap);
    *pData++ = LO_UINT16(peerCapabilities->tIp2Cap);
    *pData++ = HI_UINT16(peerCapabilities->tIp2Cap);
    *pData++ = LO_UINT16(peerCapabilities->tFcsCap);
    *pData++ = HI_UINT16(peerCapabilities->tFcsCap);
    *pData++ = LO_UINT16(peerCapabilities->tPmCsap);
    *pData++ = HI_UINT16(peerCapabilities->tPmCsap);
    *pData++ = peerCapabilities->tSwCap;
    *pData   = peerCapabilities->snrTxCap;

    // Send message
    HCI_SendEventToHost(pEvt);
  }
}

/*******************************************************************************
 * @fn          HCI_CS_ConfigCompleteCback
 *
 * @brief       Callback to send CS Config Complete event.
 *
 * input parameters
 *
 * @param       status     - status
 * @param       connHandle - connection handle
 * @param       csConfig   - peerCapabilities
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
WEAK_FUNC void HCI_CS_ConfigCompleteCback(uint16 connHandle, uint8_t configId, uint8 status)
{
  uint8* pEvt;
  const csConfigurationSet_t* csConfig = NULL;

  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8* pData;

  pEvt = hciAllocAndPrepHciLeEvtPkt(&pData, HCI_LE_CS_CONFIG_COMPLETE_EVENT,
                                    HCI_LE_CS_CONFIG_COMPLETE_EVENT_LEN);

if (NULL != pEvt)
  {
    *pData++ = status;                // status
    *pData++ = LO_UINT16(connHandle); // connection handle (LSB)
    *pData++ = HI_UINT16(connHandle); // connection handle (MSB)
    *pData++ = configId;              // configId
    if ( status == CS_STATUS_SUCCESS )
    {
      csConfig = llCsDbGetConfiguration(connHandle, configId);
      *pData++ = csConfig->action;
      *pData++ = csConfig->mainMode;
      *pData++ = csConfig->subMode;
      *pData++ = csConfig->mainModeMinSteps;
      *pData++ = csConfig->mainModeMaxSteps;
      *pData++ = csConfig->mainModeRepetition;
      *pData++ = csConfig->modeZeroSteps;
      *pData++ = csConfig->role;
      *pData++ = csConfig->rttType;
      *pData++ = csConfig->csSyncPhy;
      pData = MAP_osal_memcpy(pData, &csConfig->channelMap, CS_CHM_SIZE);
      *pData++ = csConfig->chMRepetition;
      *pData++ = csConfig->chSel;
      *pData++ = csConfig->ch3cShape;
      *pData++ = csConfig->ch3CJump;
      *pData++ = CS_RFU;
      *pData++ = csConfig->tIP1;
      *pData++ = csConfig->tIP2;
      *pData++ = csConfig->tFCs;
      *pData = csConfig->tPM;
    }
    // send the message
    HCI_SendEventToHost(pEvt);
  }
}

/*******************************************************************************
 * @fn          HCI_CS_ReadRemoteFAETableCompleteCback
 *
 * @brief       callback function that returns the result of reading the remote
 *              FAE table
 *
 * input parameters
 *
 * @param       status
 * @param       connHandle connection handle
 * @param       faeTable   pointer to remote FAE table
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
WEAK_FUNC void HCI_CS_ReadRemoteFAETableCompleteCback(uint8 status,
                                                      uint16 connHandle,
                                                      uint8* faeTable)
{
  uint8 eventLen = HCI_LE_CS_READ_REMOTE_FAE_TABLE_COMPLETE_EVENT_LEN;
  uint8* pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8* pData;

  pEvt = hciAllocAndPrepHciLeEvtPkt(
    &pData, HCI_LE_CS_READ_REMOTE_FAE_TABLE_COMPLETE_EVENT, eventLen);

  if (NULL != pEvt)
  {
    *pData++ = status;                // status
    *pData++ = LO_UINT16(connHandle); // connection handle (LSB)
    *pData++ = HI_UINT16(connHandle); // connection handle (MSB)
    if (NULL != faeTable)
    {
      for (uint8 i = 0; i <= (eventLen - 4); i++)
      {
        *pData++ = *faeTable++;
      }
    }

    // Send message
    HCI_SendEventToHost(pEvt);
  }
}

/*******************************************************************************
 * @fn          HCI_CS_SecurityEnableCompleteCback
 *
 * @brief       Security Enable complete callback function.
 *
 * input parameters
 *
 * @param       status     - event status
 * @param       connHanlde - connection identifier
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
WEAK_FUNC void HCI_CS_SecurityEnableCompleteCback(uint8 status, uint16 connHandle)
{
  uint8* pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8* pData;

  pEvt =
    hciAllocAndPrepHciLeEvtPkt(&pData, HCI_LE_CS_SECURITY_ENABLE_COMPLETE_EVENT,
                               HCI_LE_CS_SECURITY_ENABLE_COMPLETE_EVENT_LEN);

  if (NULL != pEvt)
  {
    *pData++ = status;                // status
    *pData++ = LO_UINT16(connHandle); // connection handle (LSB)
    *pData = HI_UINT16(connHandle);   // connection handle (MSB)

    // send the message
    HCI_SendEventToHost(pEvt);
  }
}

/*******************************************************************************
 * @fn          HCI_CS_ProcedureEnableCompleteCback
 *
 * @brief       Procedure Enable Complete Event callback
 *
 * input parameters
 *
 * @param       status     - event status
 * @param       connHanlde - connection identifier
 * @param       enable     - indicates enable or disable
 * @param       enableData - enable data
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
WEAK_FUNC void HCI_CS_ProcedureEnableCompleteCback(uint16 connHandle,
                                                   uint8_t configId,
                                                   uint8 enable,
                                                   uint8 status)
{
  uint8* pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8* pData;

  pEvt = hciAllocAndPrepHciLeEvtPkt(
    &pData, HCI_LE_CS_PROCEDURE_ENABLE_COMPLETE_EVENT,
    HCI_LE_CS_PROCEDURE_ENABLE_COMPLETE_EVENT_LEN);

  if (NULL != pEvt)
  {
    const csEnableProcedureCtrlData_t *pEnable = llCsDbGetProcedureData();

    *pData++ = status;
    *pData++ = LO_UINT16(connHandle); // connection handle (LSB)
    *pData++ = HI_UINT16(connHandle); // connection handle (MSB)
    *pData++ = configId;                               // config Id
    *pData++ = enable;                                 // State
    *pData++ = pEnable->ACI;                           // Tone_Antenna_Config_Selection
    *pData++ = (uint8_t)pEnable->pwrDelta;             // Selected_TX_Power
    *pData++ = BREAK_UINT32(pEnable->subEventLen, 0);
    *pData++ = BREAK_UINT32(pEnable->subEventLen, 1);
    *pData++ = BREAK_UINT32(pEnable->subEventLen, 2);
    *pData++ = pEnable->subEventsPerEvent;
    *pData++ = LO_UINT16(pEnable->subEventInterval);
    *pData++ = HI_UINT16(pEnable->subEventInterval);
    *pData++ = LO_UINT16(pEnable->eventInterval);
    *pData++ = HI_UINT16(pEnable->eventInterval);
    *pData++ = LO_UINT16(pEnable->procedureInterval);
    *pData++ = HI_UINT16(pEnable->procedureInterval);
    *pData++ = LO_UINT16(pEnable->procedureCount);
    *pData++ = HI_UINT16(pEnable->procedureCount);
    *pData++ = LO_UINT16(pEnable->maxProcedureDur);
    *pData = HI_UINT16(pEnable->maxProcedureDur);

    // send the message
    HCI_SendEventToHost(pEvt);
  }
}

/*******************************************************************************
 * @fn          HCI_CS_SubeventResultsProcess
 *
 * @brief       Subevent results callback
 *
 * input parameters
 *
 * @param       pRes - pointer to results data
 * @param       dataLength - length of data
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
WEAK_FUNC void HCI_CS_SubeventResultsProcess(const RCL_CmdBleCs_SubeventResults *subeventRes, uint16_t dataLength)
{
  if (subeventRes)
  {
    uint8* pEvt;
    // Pointer to data inside pEvt, that pointer point next slot to be filled
    uint8* pData;

    pEvt = hciAllocAndPrepHciEvtPkt(&pData, HCI_LE_EVENT_CODE, dataLength);

    if (NULL != pEvt)
    {
      if (NULL != pData)
      {
        // Copy results into pData
        MAP_osal_memcpy(pData, (void *)subeventRes, dataLength);
      }

      // send the message
      HCI_SendEventToHost(pEvt);
    }
  }
}

/*******************************************************************************
 * @fn          HCI_CS_SubeventContResultsProcess
 *
 * @brief       Subevent results callback
 *
 * input parameters
 *
 * @param       pRes - pointer to cont results data
 * @param       dataLength - length of data

 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */

WEAK_FUNC void HCI_CS_SubeventContResultsProcess(const RCL_CmdBleCs_SubeventResultsContinue* subeventRes, uint16_t dataLength)
{
  if (subeventRes)
  {
    uint8* pEvt;
    // Pointer to data inside pEvt, that pointer point next slot to be filled
    uint8* pData;

    pEvt = hciAllocAndPrepHciEvtPkt(&pData, HCI_LE_EVENT_CODE, dataLength);

    if (NULL != pEvt)
    {
      if (NULL != pData)
      {
        // Copy results into pData
        MAP_osal_memcpy(pData, (void *)subeventRes, dataLength);
      }

      // send the message
      HCI_SendEventToHost(pEvt);
    }
  }
}

/*******************************************************************************
 * @fn          HCI_CS_TestEndCompleteCback
 *
 * @brief       CS Test Command Complete Callback
 *
 * input parameters
 *
 * @param       status - Test Command Status
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void HCI_CS_TestEndCompleteCback(uint8 status)
{
  uint8* pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8* pData;

  pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                     HCI_LE_CS_TEST_END_COMPLETE_EVENT,
                                     HCI_LE_CS_TEST_END_COMPLETE_EVENT_LEN);

  if (NULL != pEvt)
  {
    *pData++ = status;

    // Send the message
    HCI_SendEventToHost(pEvt);
  }
}
// In the hci_event.c, there are additional implementations for the following functions.
#ifndef HOST_CONFIG

/*******************************************************************************
 * @fn          HCI_CommandCompleteEvent
 *
 * @brief       This function sends a Command Complete Event to the Host.
 *
 * input parameters
 *
 * @param       opcode   - The opcode of the command that generated this event.
 * @param       numParam - The number of parameters in the event.
 * @param      *param    - The event parameters associated with the command.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void HCI_CommandCompleteEvent( uint16 opcode, uint8 numParam, uint8 *param )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  pEvt = hciAllocAndPrepHciEvtPkt( &pData, HCI_COMMAND_COMPLETE_EVENT_CODE,
                                   numParam + HCI_CMD_COMPLETE_EVENT_LEN );

  if ( pEvt )
  {
    *pData++ = 1;                   // event parameter 1
    *pData++ = LO_UINT16( opcode ); // event parameter 2
    *pData++ = HI_UINT16( opcode ); // event parameter 2

    // Remaining event parameters
    memcpy( pData, param, numParam );

    // Send message
    HCI_SendEventToHost( pEvt );
  }
}

/*******************************************************************************
 * @fn          HCI_VendorSpecifcCommandCompleteEvent
 *
 * @brief       This function sends a Vendor Specific Command Complete Event to
 *              the Host.
 *
 * input parameters
 *
 * @param       opcode   - The opcode of the command that generated this event.
 * @param       numParam - The number of parameters in the event.
 * @param      *param    - The event parameters associated with the command.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void HCI_VendorSpecifcCommandCompleteEvent( uint16 opcode, uint8 numParam,
                                            uint8 *param )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  pEvt = hciAllocAndPrepHciEvtPkt( &pData, HCI_VE_EVENT_CODE,
                                   numParam + HCI_CMD_VS_COMPLETE_EVENT_LEN );

  if ( pEvt )
  {
    *pData++ = param[0];            // event parameter 0: event opcode LSB
    *pData++ = param[1];            // event parameter 1: event opcode MSB
    *pData++ = param[2];            // event parameter 2: status
    *pData++ = LO_UINT16( opcode ); // event parameter 3: command opcode LSB
    *pData++ = HI_UINT16( opcode ); // event parameter 3: command opcode MSB

    // Remaining event parameters
    // Note: The event opcode and status were already placed in the msg packet.
    memcpy( pData, &param[3], numParam - HCI_EVENT_MIN_LENGTH );

    // Send message
    HCI_SendEventToHost( pEvt );
  }
}

/*******************************************************************************
 * @fn          HCI_NumOfCompletedPacketsEvent
 *
 * @brief       This function sends the Number of Completed Packets Event to
 *              the Host.
 *
 *              Note: Currently, the number of handles is always one.
 *
 * input parameters
 *
 * @param       numHandles       - Number of handles.
 * @param       handlers         - Array of connection handles.
 * @param       numCompletedPkts - Array of number of completed packets for
 *                                 each handle.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void HCI_NumOfCompletedPacketsEvent( uint8 numHandles, uint16 *handles,
                                     uint16 *numCompletedPkts )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  // ALT: Use 1+ (numHandles * HCI_NUM_COMPLETED_PACKET_EVENT_LEN-1).
  uint8 hciPktLen = HCI_NUM_COMPLETED_PACKET_EVENT_LEN
      + ((numHandles - 1) * (HCI_NUM_COMPLETED_PACKET_EVENT_LEN - 1));

  pEvt = hciAllocAndPrepHciEvtPkt( &pData,
                                   HCI_NUM_OF_COMPLETED_PACKETS_EVENT_CODE,
                                   hciPktLen );
  if ( pEvt )
  {
    // Fill data
    *pData = numHandles;

    // For each handle, there's a number handle number and a number of
    // completed packets for that handle
    for ( uint8 i = 0; i < numHandles; i++ )
    {

      // For protect overflow we move the pointer here
      pData++;

      *pData++ = LO_UINT16( handles[i] );
      *pData++ = HI_UINT16( handles[i] );
      *pData++ = LO_UINT16( numCompletedPkts[i] );
      // For protect overflow that last pData without ++ and we add it in the beginning
      *pData = HI_UINT16( numCompletedPkts[i] );
    }

    // Send message
    HCI_SendEventToHost( pEvt );
  }
}

/*******************************************************************************
 * @fn          LL_DataLenExceedEventCback
 *
 * @brief       This EXT LL callback is used to generate an event after receiving
 *              L2CAP data packet and the L2CAP length was exceeded the MTU size.
 *
 * input parameters
 *
 * @param       status - error code:
 *                       HW_FAIL_PDU_SIZE_EXCEEDS_MTU or
 *                       HW_FAIL_PKT_LEN_EXCEEDS_PDU_SIZE
 * @param       handle - Connection handle.
 * @param       cid - L2CAP Channel ID.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_DataLenExceedEventCback( uint8 status, uint16 connHandle, uint16 cid )
{
  // in HCI test keep the same behavior as it was before
  MAP_HCI_HardwareErrorEvent( status );
}

/*******************************************************************************
 * @fn          LL_AuthPayloadTimeoutExpiredCback Callback
 *
 * @brief       This LL callback is used to generate an Authenticated Payload
 *              Timeout event when the APTO expires.
 *
 * input parameters
 *
 * @param       connHandle - Connection handle.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_AuthPayloadTimeoutExpiredCback( uint16 connHandle )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  // check if LE Meta-Events are enabled and this event is enabled
  if ( HCI_CheckEventMaskPage1( BT_EVT_LE_META_EVENT_BIT ) &&
       HCI_CheckEventMaskPage2(BT_EVT_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED_BIT ) )
  {
    pEvt = hciAllocAndPrepHciEvtPkt( &pData,
                                     HCI_APTO_EXPIRED_EVENT_CODE,
                                     HCI_APTO_EXPIRED_EVENT_LEN );

    if ( pEvt )
    {
      *pData++ = LO_UINT16( connHandle );
      *pData++ = HI_UINT16( connHandle );

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          LL_DisconnectCback Callback
 *
 * @brief       This LL callback is used to generate a Disconnect Complete meta
 *              event when a connection is disconnected by either a Central or
 *              a Central.
 *
 * input parameters
 *
 * @param       connHandle - Connection handle.
 * @param       reasonCode - Status of connection complete.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_DisconnectCback( uint16 connHandle, uint8 reasonCode )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskPage1( BT_EVT_DISCONNECT_COMPLETE_BIT ) )
  {
    pEvt = hciAllocAndPrepHciEvtPkt( &pData,
                                     HCI_DISCONNECTION_COMPLETE_EVENT_CODE,
                                     HCI_DISCONNECTION_COMPLETE_LEN );

    if ( pEvt )
    {
      // Populate data
      *pData++ = HCI_SUCCESS;                         // Status
      *pData++ = LO_UINT16( connHandle );             // connection handle (LSB)
      *pData++ = HI_UINT16( connHandle );             // connection handle (MSB)
      *pData = reasonCode;                          // reason code

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }

  /**** UPDATE DEBUG INFO MODULE ****/
  (void) OPT_llDbgInf_addConnTerm( connHandle, reasonCode );
}

/*******************************************************************************
 * @fn          LL_EncChangeCback Callback
 *
 * @brief       This Callback is used by the LL to indicate to the Host that
 *              an encryption change has taken place. This results when
 *              the host performs a LL_StartEncrypt when encryption is not
 *              already enabled.
 *
 *              Note: If the key request was rejected, then encryption will
 *                    remain off.
 *
 * input parameters
 *
 * @param       connHandle - The LL connection ID for new connection.
 * @param       reason     - LL_ENC_KEY_REQ_ACCEPTED or LL_ENC_KEY_REQ_REJECTED.
 * @param       encEnab    - LL_ENCRYPTION_OFF or LL_ENCRYPTION_ON.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_EncChangeCback( uint16 connHandle, uint8 reason, uint8 encEnab )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskPage1( BT_EVT_ENCRYPTION_CHANGE_BIT ) )
  {
    pEvt = hciAllocAndPrepHciEvtPkt( &pData,
                                     HCI_ENCRYPTION_CHANGE_EVENT_CODE,
                                     HCI_ENCRYPTION_CHANGE_EVENT_LEN );

    if ( pEvt )
    {
      // Populate data
      *pData++ = reason;                            // Reason
      *pData++ = LO_UINT16( connHandle );           // connection handle (LSB)
      *pData++ = HI_UINT16( connHandle );           // connection handle (MSB)
      *pData = encEnab;                             // encrypt is enable

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          LL_EncKeyRefreshCback Callback
 *
 * @brief       This Callback is used by the LL to indicate to the Host that
 *              an encryption key refresh has taken place. This results when
 *              the host performs a LL_StartEncrypt when encryption is already
 *              enabled.
 *
 * input parameters
 *
 * @param       connHandle - The LL connection ID for new connection.
 * @param       reason    - LL_ENC_KEY_REQ_ACCEPTED, LL_CTRL_PKT_TIMEOUT_TERM
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_EncKeyRefreshCback( uint16 connHandle, uint8 reason )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskPage1( BT_EVT_KEY_REFRESH_COMPLETE_BIT ) )
  {
    pEvt = hciAllocAndPrepHciEvtPkt( &pData,
                                     HCI_KEY_REFRESH_COMPLETE_EVENT_CODE,
                                     HCI_KEY_REFRESH_COMPLETE_EVENT_LEN );
    if ( pEvt )
    {
      // Populate data
      *pData++ = reason;                              // Reason
      *pData++ = LO_UINT16( connHandle );             // connection handle (LSB)
      *pData   = HI_UINT16( connHandle );             // connection handle (MSB)

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          HCI_CommandStatusEvent
 *
 * @brief       This function sends a Command Status Event to the Host.
 *
 * input parameters
 *
 * @param       status - The resulting status of the command.
 * @param       opcode - The opcode of the command that generated this event.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void HCI_CommandStatusEvent( hciStatus_t status, uint16 opcode )
{
  MAP_HCI_SendCommandStatusEvent( HCI_COMMAND_STATUS_EVENT_CODE, status,
                                  opcode );
}

/*******************************************************************************
 * @fn          LL_EXT_ChanMapUpdateCback Callback
 *
 * @brief       This LL callback is used to generate a vendor specific channel map
 *              update event
 *
 * input parameters
 *
 * @param       connHandle - connection for which channel map was updated
 * @param       newChanMap - new channel map
 * @param       nextDataChan - the next channel we will be using
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_EXT_ChanMapUpdateCback( uint16 connHandle, uint8 *newChanMap,
                                uint8 nextDataChan )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                     HCI_BLE_CHANNEL_MAP_UPDATE_EVENT,
                                     HCI_BLE_CHANNEL_MAP_UPDATE_EVENT_LEN );
  if ( pEvt )
  {

    *pData++ = LO_UINT16( connHandle );
    *pData++ = HI_UINT16( connHandle );
    *pData++ = nextDataChan; // next data channel

    memcpy( pData, newChanMap, LL_NUM_BYTES_FOR_CHAN_MAP ); // new channel map

    // Send message
    HCI_SendEventToHost( pEvt );
  }
}

/*******************************************************************************
 * @fn          LL_RemoteConnParamReqCback Callback
 *
 * @brief       This LL callback is used to generate a Remote Connection
 *              Parameter Request meta event to provide to the Host the peer's
 *              connection parameter request parameters (min connection
 *              interval, max connection interval, peripheral latency, and connection
 *              timeout), and to request the Host's acceptance or rejection of
 *              this parameters.
 *
 * input parameters
 *
 * @param       connHandle   - Connection handle.
 * @param       Interval_Min - Lower limit for connection interval.
 * @param       Interval_Max - Upper limit for connection interval.
 * @param       Latency      - Peripheral latency.
 * @param       Timeout      - Connection timeout.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_RemoteConnParamReqCback( uint16 connHandle, uint16 Interval_Min,
                                 uint16 Interval_Max, uint16 Latency,
                                 uint16 Timeout )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_REMOTE_CONN_PARAM_REQUEST_BIT ) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt(&pData,
                                      HCI_BLE_REMOTE_CONN_PARAM_REQUEST_EVENT,
                                      HCI_REMOTE_CONNECTION_PARAMETER_REQUEST_LEN );
    if ( pEvt )
    {

      // Populate data
      *pData++ = LO_UINT16( connHandle );           // connection handle (LSB)
      *pData++ = HI_UINT16( connHandle );           // connection handle (MSB)
      *pData++ = LO_UINT16( Interval_Min );         // connection interval (LSB)
      *pData++ = HI_UINT16( Interval_Min );         // connection interval (MSB)
      *pData++ = LO_UINT16( Interval_Max );         // connection interval (LSB)
      *pData++ = HI_UINT16( Interval_Max );         // connection interval (MSB)
      *pData++ = LO_UINT16( Latency );              // peripheral latency (LSB)
      *pData++ = HI_UINT16( Latency );              // peripheral latency (MSB)
      *pData++ = LO_UINT16( Timeout );              // connection timeout (LSB)
      *pData   = HI_UINT16( Timeout );              // connection timeout (MSB)

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
  else
  {
    // The event mask is not set for this event, so accept as is
    OPT_LL_RemoteConnParamReqReply( connHandle, Interval_Min, Interval_Max,
                                    Latency, Timeout, 0, 0 );
  }
}

/*******************************************************************************
 * @fn          LL_PhyUpdateCompleteEventCback Callback
 *
 * @brief       This LL callback is used to indicate that the Controller has
 *              has changed the transmitter or receiver PHY in use or that the
 *              LL_SetPhy command has failed.
 *
 * input parameters
 *
 * @param       status     - Status of LL_SetPhy command.
 * @param       connHandle - Connection handle.
 * @param       txPhy      - Bit map of PHY used for Tx.
 * @param       rxPhy      - Bit map of PHY used for Rx.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_PhyUpdateCompleteEventCback( llStatus_t status, uint16 connHandle,
                                     uint8 txPhy, uint8 rxPhy )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_PHY_UPDATE_COMPLETE_BIT ) )
  {
    // Qualification fix - Bit map of PHY could not be 0
    if ( (status != LL_STATUS_SUCCESS) && (txPhy == 0) && (rxPhy == 0) )
    {
      txPhy = LL_PHY_1_MBPS;
      rxPhy = LL_PHY_1_MBPS;
    }

    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_PHY_UPDATE_COMPLETE_EVENT,
                                       HCI_PHY_UPDATE_COMPLETE_EVENT_LEN );
    if ( pEvt )
    {

      // Populate data
      *pData++ = status;                               // status
      *pData++ = LO_UINT16( connHandle );              // connection handle (LSB)
      *pData++ = HI_UINT16( connHandle );              // connection handle (MSB)
      *pData++ = LL_ConvertPhy( txPhy );               // TX PHY
      *pData   = LL_ConvertPhy( rxPhy );               // RX PHY

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          LL_DataLengthChangeEventCback Callback
 *
 * @brief       This LL callback is used to indicate that the Controller has
 *              has changed the either the maximum payload length or the
 *              maximum transmit time of data channel PDUs in either direction.
 *              The values reported are the maximum taht will actually be used
 *              on the connection following the change.
 *
 * input parameters
 *
 * @param       connHandle  - Connection handle.
 * @param       maxTxOctets - Maximum number of transmit payload bytes.
 * @param       maxTxTime   - Maximum transmit time.
 * @param       maxRxOctets - Maximum number of receive payload bytes.
 * @param       maxRxTime   - Maximum receive time.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_DataLengthChangeEventCback( uint16 connHandle, uint16 maxTxOctets,
                                    uint16 maxTxTime, uint16 maxRxOctets,
                                    uint16 maxRxTime )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_DATA_LENGTH_CHANGE_BIT ) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_DATA_LENGTH_CHANGE_EVENT,
                                       HCI_DATA_LENGTH_CHANGE_EVENT_LEN );

    if ( pEvt )
    {
      // Populate data
      *pData++ = LO_UINT16( connHandle );              // connection handle (LSB)
      *pData++ = HI_UINT16( connHandle );              // connection handle (MSB)
      *pData++ = LO_UINT16( maxTxOctets );             // max Tx bytes (LSB)
      *pData++ = HI_UINT16( maxTxOctets );             // max Tx bytes (MSB)
      *pData++ = LO_UINT16( maxTxTime );               // max Tx time (LSB)
      *pData++ = HI_UINT16( maxTxTime );               // max Tx time (MSB)
      *pData++ = LO_UINT16( maxRxOctets );             // max Rx bytes (LSB)
      *pData++ = HI_UINT16( maxRxOctets );             // max Rx bytes (MSB)
      *pData++ = LO_UINT16( maxRxTime );               // max Rx time (LSB)
      *pData   = HI_UINT16( maxRxTime );               // max Rx time (MSB)

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          LL_ReadLocalP256PublicKeyCompleteEventCback Callback
 *
 * @brief       This LL callback is used to indicate the Controller has
 *              completed the generation of the P256 public key.
 *
 * @param       None.
 *
 * input parameters
 *
 * output parameters
 *
 * @param       status   - Operation status.
 * @param       p256KeyX - P256 public key (first 32 bytes X=0..31).
 * @param       p256KeyY - P256 public key (second 32 bytes Y=32..63).
 *
 * @return      None.
 */
void LL_ReadLocalP256PublicKeyCompleteEventCback( uint8_t status, uint8_t *p256KeyX,
                                                  uint8_t *p256KeyY )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_BIT ) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_EVENT,
                                       HCI_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_EVENT_LEN );
    if ( pEvt )
    {
      // Populate data
      *pData++ = status;

      if ( status == HCI_SUCCESS )
      {
        // Copy the P256 key
        memcpy( pData, p256KeyX, LL_SC_P256_KEY_LEN / 2 );
        pData += (LL_SC_P256_KEY_LEN / 2);
        memcpy( pData, p256KeyY, LL_SC_P256_KEY_LEN / 2 );
      }
      // Clear the key to avoid confusion
      else
      {
        (void) MAP_osal_memset( pData, 0, LL_SC_P256_KEY_LEN );
      }

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          LL_GenerateDHKeyCompleteEventCback Callback
 *
 * @brief       This LL callback is used to indicate teh Controller has
 *              completed the generation of the Diffie Hellman key.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       status - Operation status.
 * @param       dhKey  - Diffie Hellman key (32 bytes).
 *
 * @return      None.
 */
void LL_GenerateDHKeyCompleteEventCback( uint8_t status, uint8_t *dhKey )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_GENERATE_DHKEY_COMPLETE_BIT ) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_GENERATE_DHKEY_COMPLETE_EVENT,
                                       HCI_GENERATE_DHKEY_COMPLETE_EVENT_LEN );
    if ( pEvt )
    {
      // Populate data
      *pData++ = status;

      if ( status == HCI_SUCCESS )
      {
        // Copy the DH key
        memcpy( pData, dhKey, LL_SC_DHKEY_LEN );
      }

      else // Clear the key to avoid confusion
      {
        (void) MAP_osal_memset( pData, 0xFF, LL_SC_DHKEY_LEN );
      }

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          LL_EnhancedConnectionCompleteCback Callback
 *
 * @brief       This LL callback is used to generate an Enhanced Connection
 *              Complete meta event when a connection is established by either
 *              an Advertiser or an Initiator.
 *
 * input parameters
 *
 * @param       reasonCode    - Status of connection complete.
 * @param       connHandle    - Connection handle.
 * @param       role          - Connection formed as Central or Central.
 * @param       peerAddrType  - Peer address as Public or Random.
 * @param       peerAddr      - Pointer to peer device address.
 * @param       localRPA      - Pointer to local RPA.
 * @param       peerRPA       - Pointer to peer RPA.
 * @param       connInterval  - Connection interval.
 * @param       peripheralLatency  - Peripheral latency.
 * @param       connTimeout   - Connection timeout.
 * @param       clockAccuracy - Sleep clock accuracy (from Central only).
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_EnhancedConnectionCompleteCback( uint8 reasonCode, uint16 connHandle,
                                         uint8 role, uint8 peerAddrType,
                                         uint8 *peerAddr, uint8 *localRPA,
                                         uint8 *peerRPA, uint16 connInterval,
                                         uint16 peripheralLatency,
                                         uint16 connTimeout,
                                         uint8 clockAccuracy,
                                         uint8 advHandle,
                                         uint16 syncHandle)
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;
  uint8 enhanceConn = TRUE;
  uint8 hciEvtType;
  uint8 hciPktLen;


  if (((OPT_LL_PAwRS_IsEnable() == UTRUE) || (OPT_LL_PAwRA_IsEnable() == UTRUE)) &&
      (HCI_CheckEventMaskLe( LE_EVT_ENH_CONN_COMPLETE_BIT_V2 ) == UTRUE))
  {
        hciPktLen = HCI_ENH_CONNECTION_COMPLETE_EVENT_LEN_V2;
        hciEvtType = HCI_BLE_ENHANCED_CONNECTION_COMPLETE_EVENT_V2;
  }
  else if (HCI_CheckEventMaskLe( LE_EVT_ENH_CONN_COMPLETE_BIT_V1 ) == UTRUE)
  {
        hciPktLen = HCI_ENH_CONNECTION_COMPLETE_EVENT_LEN_V1;
        hciEvtType = HCI_BLE_ENHANCED_CONNECTION_COMPLETE_EVENT_V1;
  }
  else if ( HCI_CheckEventMaskLe( LE_EVT_CONN_COMPLETE_BIT ) )
  {
    enhanceConn = FALSE;
    hciPktLen = HCI_CONNECTION_COMPLETE_EVENT_LEN;
    hciEvtType = HCI_BLE_CONNECTION_COMPLETE_EVENT;
  }
  else // event not enable in event mask
  {
    hciEvtType = FALSE;
  }
  if ( hciEvtType != FALSE )
  {

    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData, hciEvtType, hciPktLen );

    if ( pEvt )
    {
      // Populate data
      *pData++ = reasonCode;                                   // reason code
      *pData++ = LO_UINT16( connHandle );                      // connection handle (LSB)
      *pData++ = HI_UINT16( connHandle );                      // connection handle (MSB)
      *pData++ = role;                                         // role (central/peripheral)

      if ( enhanceConn )
      {
        *pData++ = peerAddrType;                               // address type
      }
      else // Connection Complete
      {
        *pData++ = peerAddrType & LL_DEV_ADDR_TYPE_MASK;       // address type
      }

      // Copy peer address
      if ( peerAddr != NULL )
      {
        MAP_osal_memcpy( pData, peerAddr, B_ADDR_LEN );
      }
      else
      {
        MAP_osal_memset( pData, 0, B_ADDR_LEN );
      }

      pData += B_ADDR_LEN;

      if ( enhanceConn )
      {
        // Local and peer RPA
        (localRPA != NULL) ? MAP_osal_memcpy( pData, localRPA, B_ADDR_LEN ) :
        MAP_osal_memset( pData, 0, B_ADDR_LEN );

        pData += B_ADDR_LEN;

        (peerRPA != NULL) ? MAP_osal_memcpy( pData, peerRPA, B_ADDR_LEN ) :
        MAP_osal_memset( pData, 0, B_ADDR_LEN );

        pData += B_ADDR_LEN;
      }

      *pData++ = LO_UINT16( connInterval );        // connection interval (LSB)
      *pData++ = HI_UINT16( connInterval );        // connection interval (MSB)
      *pData++ = LO_UINT16( peripheralLatency );   // peripheral latency (LSB)
      *pData++ = HI_UINT16( peripheralLatency );   // peripheral latency (LSB)
      *pData++ = LO_UINT16( connTimeout );         // connection timeout (LSB)
      *pData++ = HI_UINT16( connTimeout );         // connection timeout (MSB)
      *pData = clockAccuracy;                      // clock accuracy

      if (((OPT_LL_PAwRS_IsEnable() == UTRUE) || (OPT_LL_PAwRA_IsEnable() == UTRUE)) &&
          (HCI_CheckEventMaskLe( LE_EVT_ENH_CONN_COMPLETE_BIT_V2 ) == UTRUE))
      {
          pData++;
          *pData++ = advHandle; // Advertising handle
          *pData++ = LO_UINT16( syncHandle ); // Sync handle (LSB)
          *pData = HI_UINT16( syncHandle ); // Sync handle (MSB)
      }
      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }

  /**** UPDATE DEBUG INFO MODULE ****/
  (void) OPT_DbgInf_addConnEst( connHandle, role, UFALSE );
}

/*******************************************************************************
 * @fn          LL_ChannelSelectionAlgorithmCback Callback
 *
 * @brief       This LL callback is used to indicate which channel selection
 *              algorithm is used on a data channel connection.
 *
 * input parameters
 *
 * @param       connHandle - Connection handle.
 * @param       chSelAlgo  - LL_CHANNEL_SELECT_ALGO_1 | LL_CHANNEL_SELECT_ALGO_2
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_ChannelSelectionAlgorithmCback( uint16 connHandle, uint8 chSelAlgo )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_CHANNEL_SELECTION_ALGORITHM_BIT ) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_CHANNEL_SELECTION_ALGORITHM_EVENT,
                                       HCI_CHANNEL_SELECTION_ALGORITHM_EVENT_LEN );
    if ( pEvt )
    {
      // Populate data
      *pData++ = LO_UINT16( connHandle );               // Connection handle (LSB)
      *pData++ = HI_UINT16( connHandle );               // Connection handle (MSB)
      *pData   = chSelAlgo;                             // Channel selection algo

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          LL_AdvReportCback Callback
 *
 * @brief       This LL callback is used to generate a Advertisment Report meta
 *              event when an Advertisment or Scan Response is received by a
 *              Scanner.
 *
 * input parameters
 *
 * @param       advEvt      - Advertise event type, or Scan Response event type.
 * @param       advAddrType - Public or Random address type.
 * @param       advAddr     - Pointer to device address.
 * @param       dataLen     - Length of data in bytes.
 * @param       advData     - Pointer to data.
 * @param       rssi        - The RSSI of received packet.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_AdvReportCback( uint8 advEvt, uint8 advAddrType, uint8 *advAddr,
                        uint8 dataLen, uint8 *advData, int8 rssi )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_ADV_REPORT_BIT ) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_ADV_REPORT_EVENT,
                                       (HCI_ADV_REPORT_EVENT_LEN + dataLen) );
    if ( pEvt )
    {
      // Populate data
      *pData++ = 1;                          // Number of devices; assume 1 for now
      *pData++ = advEvt;                     // Advertisement event type
      *pData++ = advAddrType;                // Address type

      memcpy( pData, advAddr, B_ADDR_LEN );  // Address
      pData += B_ADDR_LEN;

      *pData++ = dataLen;                    // Data length

      memcpy( pData, advData, dataLen );     // Data
      pData += dataLen;

      *pData = rssi;                         // RSSI

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          LL_ConnectionCompleteCback Callback
 *
 * @brief       This LL callback is used to generate a Connection Complete meta
 *              event when a connection is established by either an Advertiser
 *              or an Initiator.
 *
 * input parameters
 *
 * @param       reasonCode    - Status of connection complete.
 * @param       connHandle    - Connection handle.
 * @param       role          - Connection formed as Central or Central.
 * @param       peerAddrType  - Peer address as Public or Random.
 * @param       peerAddr      - Pointer to peer device address.
 * @param       connInterval  - Connection interval.
 * @param       peripheralLatency  - Peripheral latency.
 * @param       connTimeout   - Connection timeout.
 * @param       clockAccuracy - Sleep clock accuracy (from Central only).
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_ConnectionCompleteCback( uint8 reasonCode, uint16 connHandle,
                                 uint8 role, uint8 peerAddrType,
                                 uint8 *peerAddr, uint16 connInterval,
                                 uint16 peripheralLatency, uint16 connTimeout,
                                 uint8 clockAccuracy )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_CONN_COMPLETE_BIT ) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_CONNECTION_COMPLETE_EVENT,
                                       HCI_CONNECTION_COMPLETE_EVENT_LEN );
    if ( pEvt )
    {
      // Populate data
      *pData++ = reasonCode;                         // Reason code
      *pData++ = LO_UINT16( connHandle );            // Connection handle (LSB)
      *pData++ = HI_UINT16( connHandle );            // Connection handle (MSB)
      *pData++ = role;                               // Role (central/peripheral)
      *pData++ = peerAddrType;                       // Address type

      memcpy( pData, peerAddr, B_ADDR_LEN );         // Copy address
      pData += B_ADDR_LEN;

      *pData++ = LO_UINT16( connInterval );          // Connection interval (LSB)
      *pData++ = HI_UINT16( connInterval );          // Connection interval (MSB)
      *pData++ = LO_UINT16( peripheralLatency );     // Peripheral latency (LSB)
      *pData++ = HI_UINT16( peripheralLatency );     // Peripheral latency (LSB)
      *pData++ = LO_UINT16( connTimeout );           // Connection timeout (LSB)
      *pData++ = HI_UINT16( connTimeout );           // Connection timeout (MSB)
      *pData   = clockAccuracy;                      // Clock accuracy

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          LL_ConnParamUpdateCback Callback
 *
 * @brief       This LL callback is used to generate a Connection Update
 *              Complete meta event when a connection's parameters are updated
 *              by the Central, or if an error occurs (e.g. during the
 *              Connection Parameter Request control procedure).
 *
 * input parameters
 *
 * @param       status       - Status of update complete event.
 * @param       connHandle   - Connection handle.
 * @param       connInterval - Connection interval.
 * @param       peripheralLatency - Peripheral latency.
 * @param       connTimeout  - Connection timeout.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_ConnParamUpdateCback( llStatus_t status, uint16 connHandle,
                              uint16 connInterval, uint16 connLatency,
                              uint16 connTimeout )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_CONN_UPDATE_COMPLETE_BIT ) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_CONN_UPDATE_COMPLETE_EVENT,
                                       HCI_CONN_UPDATE_COMPLETE_LEN );
    if ( pEvt )
    {
      // Populate data
      *pData++ = status;                            // Status
      *pData++ = LO_UINT16( connHandle );           // Connection handle (LSB)
      *pData++ = HI_UINT16( connHandle );           // Connection handle (MSB)
      *pData++ = LO_UINT16( connInterval );         // Connection interval (LSB)
      *pData++ = HI_UINT16( connInterval );         // Connection interval (MSB)
      *pData++ = LO_UINT16( connLatency );          // Peripheral latency (LSB)
      *pData++ = HI_UINT16( connLatency );          // Peripheral latency (MSB)
      *pData++ = LO_UINT16( connTimeout );          // Connection timeout (LSB)
      *pData   = HI_UINT16( connTimeout );          // Connection timeout (MSB)

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          LL_EncLtkReqCback Callback
 *
 * @brief       This LL callback is used to generate a Encryption LTK Request
 *              meta event to provide to the Host the Central's random number
 *              and encryption diversifier, and to request the Host's Long Term
 *              Key (LTK).
 *
 * input parameters
 *
 * @param       connHandle - The LL connection ID for new connection.
 * @param       randNum    - Random vector used in device identification.
 * @param       encDiv     - Encrypted diversifier.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_EncLtkReqCback( uint16 connHandle, uint8 *randNum, uint8 *encDiv )
{
  uint8 *pEvt;
  uint8 *pData; // Pointer to data inside pEvt, that pointer point next slot to be fille

  if ( HCI_CheckEventMaskLe( LE_EVT_LTK_REQUEST_BIT ) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_LTK_REQUESTED_EVENT,
                                       HCI_LTK_REQUESTED_EVENT_LEN );

    if ( pEvt )
    {
      // Populate data
      *pData++ = LO_UINT16( connHandle );             // Connection handle (LSB)
      *pData++ = HI_UINT16( connHandle );             // Connection handle (MSB)

      memcpy( pData, randNum, B_RANDOM_NUM_SIZE );    // Random number
      pData += B_RANDOM_NUM_SIZE;

      *pData++ = *encDiv++;                           // Encryption diversifier (LSB)
      *pData   = *encDiv;                             // Encryption diversifier (MSB)

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
  else
  {
    // The event mask is not set for this event, so reject immediately
    MAP_LL_EncLtkNegReply( connHandle );
  }
}

/*******************************************************************************
 * @fn          LL_ConnParamUpdateRejectCback Callback
 *
 * @brief       This function will send an event to the GAP after the device
 *              has rejected a peer device connection parameter update request
 *
 *
 * input parameters
 *
 * @param       connHandle    - Connection handle.
 * @param       status        - Link Layer error code.
 *
 * @param       connInterval  - Connection interval.
 * @param       connLatency   - Peripheral latency.
 * @param       connTimeout   - Connection timeout.
 *
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_ConnParamUpdateRejectCback( llStatus_t status, uint16 connHandle,
                                    uint16 connInterval, uint16 connLatency,
                                    uint16 connTimeout )

{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  // check if the feature was enabled by the application.
  if ( (hciVsEvtMask & PEER_PARAM_REJECT_ENABLED) == 0 )
  {
    // the feature was not enabled
    return;
  }
  else
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData, HCI_BLE_CONN_UPDATE_REJECT_EVENT,
                                       HCI_CONN_UPDATE_COMPLETE_LEN );

    if ( pEvt )
    {
      *pData++ = status;                             // status
      *pData++ = LO_UINT16( connHandle );            // connection handle (LSB)
      *pData++ = HI_UINT16( connHandle );            // connection handle (MSB)
      *pData++ = LO_UINT16( connInterval );          // connection interval (LSB)
      *pData++ = HI_UINT16( connInterval );          // connection interval (MSB)
      *pData++ = LO_UINT16( connLatency );           // peripheral latency (LSB)
      *pData++ = HI_UINT16( connLatency );           // peripheral latency (MSB)
      *pData++ = LO_UINT16( connTimeout );           // connection timeout (LSB)
      *pData = HI_UINT16( connTimeout );             // connection timeout (MSB)

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          HCI_TransPwrRptCB
 *
 * @brief       Power Control reporting event callback. Will be called if we got
 *              a power control response packet, power control indication packet
 *              or Change in our Tx Power level in one of our connections.
 *              Also depends if the reporting flag is enabled
 *
 * input parameters
 *
 * @param       status       - pointer to results data
 * @param       connId       - Connection ID
 * @param       reason       - Reason of the report
 * @param       txPhy        - Which phy the report is for
 * @param       txPowerLevel - Tx power level
 * @param       txPowerFlag  - Reached minimum or maximum level
 * @param       delta        - Change from previous power level

 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void HCI_TransPwrRptCB( uint8_t status, uint8_t connHandle, uint8_t reason,
                        uint8_t txPhy, int8_t txPowerLevel, uint8_t txPowerMaxMinReached,
                        int8_t delta )
{
  uint8_t *pEvt;

  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8_t *pData;

  if (HCI_CheckEventMaskLe(LE_EVT_TRANS_POWER_REPORT_BIT))
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_TRANSMIT_POWER_REPORTING_EVENT,
                                       HCI_LE_TRANSMIT_POWER_REPORTING_EVENT_LEN );

    if ( pEvt )
    {
      *pData++ = status;                // Staus - controller errors or 0x00
      *pData++ = LO_UINT16(connHandle); // Connection handle (LSB)
      *pData++ = HI_UINT16(connHandle); // Connection handle (MSB)
      *pData++ = reason;                // Who trigger the event - local or remote
      *pData++ = txPhy;                 // On which phy the report is trigger
      *pData++ = (uint8_t)txPowerLevel;   // Amount of power transmitting
      *pData++ = txPowerMaxMinReached;  // Tx Power at minimum or maximum level
      *pData++ = (uint8_t)delta;          // The change of the Tx Power from previous power level

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          HCI_EXT_RssiMon_ReportCB
 *
 * @brief       This callback function is called when the RSSI monitoring reports a
 *              threshold pass.
 *
 * @design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       handle - RSSI monitor callback handle
 * @param       threshPass - RSSI threshold pass
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void HCI_EXT_RssiMon_ReportCB(uint8_t handle, int8_t threshPass)
{
  uint8 data[HCI_EXT_RSSI_MON_EVENT_LEN];

  data[0] = LO_UINT16( HCI_EXT_RSSI_MON_CB_EVENT );
  data[1] = HI_UINT16( HCI_EXT_RSSI_MON_CB_EVENT );
  data[2] = handle;
  data[3] = threshPass;

  hciSendVendorSpecificEvent(data, HCI_EXT_RSSI_MON_EVENT_LEN);
}

/*******************************************************************************
 * Public function defined in ll.h
 */
void HCI_PadvSyncEstabEventV1( uint8 status, uint8_t* llPadvSEstEventParams)
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_PERIODIC_ADV_SYNC_ESTABLISHED_BIT_V1 ) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_PADV_SYNC_ESTAB_EVENT_V1,
                                       HCI_PADV_SYNC_ESTAB_EVENT_V1_LEN );
    if ( pEvt )
    {
      // Populate data
      *pData++ = status;
      osal_memcpy( pData, llPadvSEstEventParams, HCI_PADV_SYNC_ESTAB_EVENT_V1_LEN - 2 );

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * Public function defined in ll.h
 */
void HCI_PadvSyncEstabEventV2( uint8 status, uint8_t* llPadvSEstEventParams)
{
  uint8_t *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8_t *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_PERIODIC_ADV_SYNC_ESTABLISHED_BIT_V2 ) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_PADV_SYNC_ESTAB_EVENT_V2,
                                       HCI_PADV_SYNC_ESTAB_EVENT_V2_LEN );
    if ( pEvt )
    {
      // Populate data
      *pData++ = status;
      osal_memcpy( pData, llPadvSEstEventParams, HCI_PADV_SYNC_ESTAB_EVENT_V2_LEN - 2 );

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * Public function defined in ll.h
 */
void HCI_PeriodicAdvReportEventV1( uint8_t* periodicEvtParams, uint8_t dataStatus, uint8 dataLen, uint8 *data )
{
 uint8_t *pEvt;
 // Pointer to data inside pEvt, that pointer point next slot to be filled
 uint8_t *pData;

 if ( HCI_CheckEventMaskLe( LE_EVT_PERIODIC_ADV_REPORT_BIT_V1 ) )
 {
   uint8_t dataLength;
   uint8_t eventLength;
   uint8_t dataOffset = 0;

   do
   {
     // Data length
     dataLength = Math_MIN( dataLen, HCI_PERIODIC_ADV_REPORT_V1_MAX_DATA );
     dataLen -= dataLength;
     eventLength = HCI_PADV_REPORT_EVENT_V1_LEN + dataLength;

     pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                        HCI_BLE_PADV_REPORT_EVENT_V1,
                                        eventLength );
     if ( pEvt )
     {
       // Populate data
       osal_memcpy( pData, periodicEvtParams, HCI_PADV_REPORT_EVENT_V1_PARAMS_LEN );
       pData += HCI_PADV_REPORT_EVENT_V1_PARAMS_LEN;
       *pData++ = (dataLen > 0) ? HCI_PERIODIC_ADV_REPORT_DATA_INCOMPLETE : dataStatus;
       *pData++ = dataLength;

       if ( (data != NULL) && (dataLength > 0) )
       {
         osal_memcpy( pData, data + dataOffset, dataLength );
         dataOffset += dataLength;
       }

       // Send message
       HCI_SendEventToHost( pEvt );
     }
   }
   while ( dataLen > 0 );
 }
}

/*******************************************************************************
 * Public function defined in ll.h
 */
void HCI_PeriodicAdvReportEventV2( uint8_t* periodicEvtParams, uint8_t dataStatus, uint8 dataLen, uint8 *data )
{
  uint8_t *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8_t *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_PERIODIC_ADV_REPORT_BIT_V2 ) )
  {
    uint8_t dataLength;
    uint8_t eventLength;
    uint8_t dataOffset = 0;

    do
    {
      // Data length
      dataLength = Math_MIN( dataLen, HCI_PERIODIC_ADV_REPORT_V2_MAX_DATA );
      dataLen -= dataLength;
      eventLength = HCI_PADV_REPORT_EVENT_V2_LEN + dataLength;

      pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                         HCI_BLE_PADV_REPORT_EVENT_V2,
                                         eventLength );
      if ( pEvt )
      {
        // Populate data
        osal_memcpy( pData, periodicEvtParams, HCI_PADV_REPORT_EVENT_V2_PARAMS_LEN );
        pData += HCI_PADV_REPORT_EVENT_V2_PARAMS_LEN;
        *pData++ = (dataLen > 0) ? HCI_PERIODIC_ADV_REPORT_DATA_INCOMPLETE : dataStatus;
        *pData++ = dataLength;

        if ( (data != NULL) && (dataLength > 0) )
        {
          osal_memcpy( pData, data + dataOffset, dataLength );
          dataOffset += dataLength;
        }

        // Send message
        HCI_SendEventToHost( pEvt );
      }
    }
    while ( dataLen > 0 );
  }
}

/*********************************************************************
 * @fn      HCI_PeriodicAdvSyncLostEvent
 *
 * @brief   This event indicates the scanner that the Controller has not
 *          received a Periodic Advertising packet from the train identified
 *          by syncHandle within the timeout period.
 *
 * @design  /ref did_286039104
 *
 * @param   syncHandle - Handle identifying the periodic advertising train
 *
 * @return  void
 */
void HCI_PeriodicAdvSyncLostEvent( uint16 syncHandle )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_PERIODIC_ADV_SYNC_LOST_BIT ) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_PERIODIC_ADV_SYNCH_LOST_EVENT,
                                       HCI_PERIODIC_ADV_SYNCH_LOST_EVENT_LEN );

    if ( pEvt )
    {
      // Populate data
      *pData++ = LO_UINT16( syncHandle );              // Sync handle (LSB)
      *pData   = HI_UINT16( syncHandle );              // Sync handle (MSB)

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * Public function defined in ll.h
 */
void HCI_PASTReceivedEventV1( uint8_t status, uint8_t* pPASTReceivedInfo )
{
  uint8_t *pEvt = NULL;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8_t *pData = NULL;

  // Check if the periodic advertising sync transfer received event V1 is enabled
  if ( HCI_CheckEventMaskLe( LE_EVT_PADV_SYNC_TRANSFER_RECEIVED_BIT_V1 ) == UTRUE )
  {
    // Allocate and prepare the event V1
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_PADV_SYNC_TRANSFER_RECEIVED_EVENT_V1,
                                       HCI_PADV_SYNC_TRANSFER_RECEIVED_LEN_V1 );
    if ( pEvt != NULL )
    {
      // Populate data
      *pData++ = status;
      // Copy the received info, excluding the first two bytes (status and subevent code),
      // which are already set
      osal_memcpy( pData, pPASTReceivedInfo, HCI_PADV_SYNC_TRANSFER_RECEIVED_LEN_V1 - 2 );

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * Public function defined in ll.h
 */
void HCI_PASTReceivedEventV2( uint8_t status, uint8_t* pPASTReceivedInfo )
{
  uint8_t *pEvt = NULL;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8_t *pData = NULL;

  // Check if the periodic advertising sync transfer received event V2 is enabled
  if ( HCI_CheckEventMaskLe( LE_EVT_PADV_SYNC_TRANSFER_RECEIVED_BIT_V2 ) == UTRUE )
  {
    // Allocate and prepare the event V2
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_PADV_SYNC_TRANSFER_RECEIVED_EVENT_V2,
                                       HCI_PADV_SYNC_TRANSFER_RECEIVED_LEN_V2 );
    if ( pEvt != NULL )
    {
      // Populate data
      *pData++ = status;
      // Copy the received info, excluding the first two bytes (status and subevent code),
      // which are already set
      osal_memcpy( pData, pPASTReceivedInfo, HCI_PADV_SYNC_TRANSFER_RECEIVED_LEN_V2 - 2 );

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          HCI_PadvASubeventDataRequestEvent
 *
 * @brief       Sends a request to the host for subevent data.
 *
 * input parameters
 *
 * @param      advHandle - The advertiser handle.
 * @param      subeventStart - The starting subevent.
 * @param      subeventCount - The number of subevents to request.
 *
 * output parameters
 *
 * @param      None.
 *
 * @return     None.
*/
void HCI_PadvASubeventDataRequestEvent(uint8_t advHandle, uint8_t subeventStart, uint8_t subeventCount)
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_PADVA_SUBEVENT_DATA_REQUEST_EVENT_BIT ) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_PADVA_SUBEVENT_DATA_REQUEST_EVENT,
                                       HCI_PADVA_SUBEVENT_DATA_REQUEST_EVENT_LEN );

    if ( pEvt )
    {
      // Populate data
      *pData++ = advHandle;              // Advertising handle
      *pData++ = subeventStart;          // Subevent start
      *pData   = subeventCount;          // Subevent count

      // Send message
      HCI_SendEventToHost( pEvt );
    }
  }
}

/*******************************************************************************
 * @fn          hciSendFragmentedPadvAResponse
 *
 * @brief       Sends a single large PAwR response that needs fragmentation
 *              across multiple HCI events.
 *
 * input parameters
 *
 * @param      advHandle - The advertiser handle.
 * @param      subevent  - The subevent number.
 * @param      txStatus  - TX status.
 * @param      pReport   - Pointer to the response report to fragment.
 *
 * output parameters
 *
 * @param      None.
 *
 * @return     SUCCESS if successful, FAILURE if out of memory.
*/
static uint8_t hciSendFragmentedPadvAResponse(uint8_t advHandle,
                                              uint8_t subevent,
                                              uint8_t txStatus,
                                              responseReport_t *pReport)
{
  uint8 *pEvt;
  uint8 *pData;
  uint8_t remainingLen = pReport->dataLen;
  uint8_t dataOffset = 0;

  while (remainingLen > 0)
  {
    uint8_t fragLen = (remainingLen > HCI_PADVA_MAX_SINGLE_RSP_DATA) ? HCI_PADVA_MAX_SINGLE_RSP_DATA : remainingLen;
    uint16 totalLen = HCI_PADVA_RESPONSE_REPORT_EVENT_BASE_LEN + HCI_PADVA_RESPONSE_REPORT_ENTRY_BASE_LEN + fragLen;

    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_PADVA_RESPONSE_REPORT_EVENT,
                                       totalLen );

    if ( pEvt )
    {
      *pData++ = advHandle;
      *pData++ = subevent;
      *pData++ = txStatus;
      *pData++ = 1;  // Single response per event when fragmenting

      *pData++ = pReport->txPower;
      *pData++ = pReport->rssi;
      *pData++ = 0xFF;  // CTE Type (0xFF = No CTE)
      *pData++ = pReport->responseSlot;

      // Set data status: 0x01 = incomplete more to come, original status for last fragment
      *pData++ = (remainingLen > HCI_PADVA_MAX_SINGLE_RSP_DATA) ? 0x01 : pReport->dataStatus;
      *pData++ = fragLen;

      osal_memcpy(pData, &pReport->data[dataOffset], fragLen);

      HCI_SendEventToHost( pEvt );

      dataOffset += fragLen;
      remainingLen -= fragLen;
    }
    else
    {
      return FAILURE;
    }
  }

  return SUCCESS;
}

/*******************************************************************************
 * @fn          HCI_PadvAResponseReportEvent
 *
 * @brief       Sends a periodic advertising response report to the host.
 *
 * input parameters
 *
 * @param      advHandle    - The advertiser handle.
 * @param      subevent     - The subevent number for which responses are reported.
 * @param      txStatus     - 0x00: AUX_SYNC_SUBEVENT_IND was transmitted,
 *                            0x01: AUX_SYNC_SUBEVENT_IND was not transmitted.
 * @param      numResponses - Number of response reports.
 * @param      pResponses   - Pointer to array of response reports (responseReport_t).
 *
 * output parameters
 *
 * @param      None.
 *
 * @return     None.
*/
void HCI_PadvAResponseReportEvent(uint8_t advHandle,
                                  uint8_t subevent,
                                  uint8_t txStatus,
                                  uint8_t numResponses,
                                  void   *pResponses)
{
  uint8 *pEvt;
  uint8 *pData;

  if ( HCI_CheckEventMaskLe( LE_EVT_PADVA_RESPONSE_REPORT_EVENT_BIT ) )
  {
    responseReport_t *pReports = (responseReport_t *)pResponses;
    uint8_t rspIdx = 0;

    // Process all responses. Each HCI event is limited to MAX_REPORT_DATA_SIZE (200 bytes).
    // Two cases are handled:
    // 1. Large response (data > HCI_PADVA_MAX_SINGLE_RSP_DATA): Fragment across multiple events
    // 2. Small responses: Batch multiple responses into one event until size limit is reached
    while (rspIdx < numResponses)
    {
      // Case 1: Large response - needs fragmentation across multiple HCI events
      if (pReports[rspIdx].dataLen > HCI_PADVA_MAX_SINGLE_RSP_DATA)
      {
        if (hciSendFragmentedPadvAResponse(advHandle, subevent, txStatus, &pReports[rspIdx]) != SUCCESS)
        {
          return;  // Out of memory
        }
        rspIdx++;
      }
      else
      {
        // Case 2: Small responses - batch as many as possible into one HCI event
        uint16 totalLen = HCI_PADVA_RESPONSE_REPORT_EVENT_BASE_LEN;
        uint8_t count = 0;

        // Calculate how many responses can fit in this event
        for (uint8_t i = rspIdx; i < numResponses; i++)
        {
          // Stop if next response needs fragmentation (will be handled in next iteration)
          if (pReports[i].dataLen > HCI_PADVA_MAX_SINGLE_RSP_DATA)
          {
            break;
          }

          uint16 entryLen = HCI_PADVA_RESPONSE_REPORT_ENTRY_BASE_LEN + pReports[i].dataLen;

          // Stop if adding this response would exceed the HCI event size limit
          if ((totalLen + entryLen) > MAX_REPORT_DATA_SIZE)
          {
            break;
          }

          totalLen += entryLen;
          count++;
        }

        // Skip to next response if none could be batched (shouldn't happen normally)
        if (count == 0)
        {
          rspIdx++;
          continue;
        }

        // Allocate and send the batched event
        pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                           HCI_BLE_PADVA_RESPONSE_REPORT_EVENT,
                                           totalLen );

        if ( pEvt )
        {
          // Populate event header
          *pData++ = advHandle;
          *pData++ = subevent;
          *pData++ = txStatus;
          *pData++ = count;

          // Populate each response entry
          LL_PAwRA_PackResponseReports(pData, &pReports[rspIdx], count);

          HCI_SendEventToHost( pEvt );
        }
        else
        {
          break;  // Out of memory
        }

        rspIdx += count;
      }
    }
  }
}

#endif //HOST_CONFIG

/*
** Internal Functions
*/

/*******************************************************************************
 * @fn          hciSendVendorSpecificEvent
 *
 * @brief       This function allocation memory to new event packet with length
 *              longer than uint8
 *
 * input parameters
 *
 * @param       **pData    - Pointer to pointer that point to buffer to be fill.
 *              hciEvtType - HCI event type.
 *              hciPktLen  - HCI event len.

 *
 * output parameters
 *
 * @param       **pData - Pointer to pointer that point to next slot to be fill.
 *
 * @return      None.
 */
void hciSendVendorSpecificEvent( uint8 *pBuf, uint16 dataLen )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  // Check for overlap - dataLen is too big, drop packet
  if ( sizeof(hciPacket_t) + HCI_EVENT_MIN_LENGTH + (uint32)dataLen > HCI_MAX_EVT_PKT_SIZE_EXT )
  {
    return;
  }
  // If data length is larger than 0x00FF bytes, process as extended event
  if ( dataLen > HCI_MAX_EVT_PKT_SIZE )
  {
    pEvt = hciAllocAndPrepExtHciEvtPkt( &pData, dataLen );
  }

  else
  {
    pEvt = hciAllocAndPrepHciEvtPkt( &pData, HCI_VE_EVENT_CODE, dataLen );
  }

  if ( pEvt )
  {
    // Copy data
    if ( dataLen )
    {
      memcpy( pData, pBuf, dataLen );
    }

    // Send message
    HCI_SendEventToHost( pEvt );
  }
}

/*******************************************************************************
 * @fn          hciSendSystemReport
 *
 * @brief       Used to return specific system error over UART.
 *
 * @param       status - type of error
 *              info -   more information linked to the error or the module
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void hciSendSystemReport( uint8_t status, uint16_t info )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  pEvt = hciAllocAndPrepHciEvtPkt( &pData, HCI_VE_EVENT_CODE,
                                   HCI_VS_SYSTEM_REPORT_LEN );

  if ( pEvt )
  {
    *pData++ = LO_UINT16( HCI_EXT_UTIL_SYSTEM_ERROR );
    *pData++ = HI_UINT16( HCI_EXT_UTIL_SYSTEM_ERROR );
    *pData++ = status;                                 // Status
    *pData++ = HCI_MODULE_ID;                          // Module
    *pData++ = LO_UINT16( info );                      // Event Generating the error
    *pData   = HI_UINT16( info );

    // Send message
    HCI_SendEventToHost( pEvt );
  }
}

/*******************************************************************************
 * @fn          hciAllocAndPrepExtHciEvtPkt
 *
 * @brief       This function allocation memory to new event packet with length
 *              longer than uint8
 *
 * input parameters
 *
 * @param       **pData    - Pointer to pointer that point to buffer to be fill.
 *              hciEvtType - HCI event type.
 *              hciPktLen  - HCI event len.

 *
 * output parameters
 *
 * @param       **pData - Pointer to pointer that point to next slot to be fill.
 *
 * @return      None.
 */
uint8* hciAllocAndPrepExtHciEvtPkt( uint8 **pData, uint16 hciPktLen )
{
  hciPacket_t *pEvt;
  uint16 totalLength;

  // OSAL message header(4) - not part of packet sent to HCI Host!
  // Minimum Event Data: Packet Type(1) + Event Code(1) + Length(1) + hciPktLen
  totalLength = sizeof(hciPacket_t) + HCI_EVENT_MIN_LENGTH + hciPktLen;

  pEvt = (hciPacket_t*) MAP_osal_msg_allocate( totalLength );

  if ( pEvt )
  {
    // create message header
    pEvt->hdr.event = HCI_CTRL_TO_HOST_EVENT;
    pEvt->hdr.status = 0xFF;
    pEvt->pData = (uint8*) (pEvt + 1);

    // Create hci header
    pEvt->pData[0] = HCI_EXTENDED_EVENT_PACKET;
    pEvt->pData[1] = LO_UINT16( hciPktLen );
    pEvt->pData[2] = HI_UINT16( hciPktLen );

    *pData = &(pEvt->pData[HCI_EVENT_DATA_OFFSET]);
  }

  else // Out of heap!
  {
    *pData = NULL;

    /*******************/
    /*** OUT OF HEAP ***/
    /*******************/
    // Send indication to the host
    MAP_HCI_HardwareErrorEvent( HCI_ERROR_CODE_MEM_CAP_EXCEEDED );
  }

  // else pEvt == NUll
  return (uint8*)pEvt;
}

/*******************************************************************************
 * @fn          hciAllocAndPrepHciEvtPkt
 *
 * @brief       This function allocation memory to new event packet and fill the
 *              common HCI event data ,event type and len (bytes 0-2)
 *
 * input parameters
 *
 * @param       **pData    - Pointer to pointer that point to buffer to be fill.
 *              hciEvtType - HCI event type.
 *              hciPktLen  - HCI event len.

 *
 * output parameters
 *
 * @param       **pData - Pointer to pointer that point to next slot to be fill.
 *
 * @return      None.
 */
uint8* hciAllocAndPrepHciEvtPkt( uint8 **pData, uint8 hciEvtType,
                                 uint8 hciPktLen )
{
  hciPacket_t *pEvt;
  uint16 totalLength;

  // OSAL message header(4) - not part of packet sent to HCI Host!
  // Minimum Event Data: Packet Type(1) + Event Code(1) + Length(1) + hciPktLen
  totalLength = sizeof(hciPacket_t) + HCI_EVENT_MIN_LENGTH + hciPktLen;

  pEvt = (hciPacket_t*) MAP_osal_msg_allocate( totalLength );

  if ( pEvt )
  {
    // create message header
    pEvt->hdr.status = 0xFF;
    pEvt->hdr.event = HCI_CTRL_TO_HOST_EVENT;
    pEvt->pData = (uint8*) (pEvt + 1);

    // Create hci header
    pEvt->pData[0] = HCI_EVENT_PACKET;
    pEvt->pData[1] = hciEvtType;
    pEvt->pData[2] = hciPktLen;

    *pData = &(pEvt->pData[HCI_EVENT_DATA_OFFSET]);
  }

  else // Out of heap!
  {
    *pData = NULL;

    /*******************/
    /*** OUT OF HEAP ***/
    /*******************/
    // Send indication to the host
    if (hciEvtType != HCI_BLE_HARDWARE_ERROR_EVENT_CODE)
    {
      MAP_HCI_HardwareErrorEvent( HCI_ERROR_CODE_MEM_CAP_EXCEEDED );
    }
  }

  // else pEvt == NUll
  return (uint8*)pEvt;
}

/*******************************************************************************
 * @fn          hciAllocAndPrepHciLeEvtPkt
 *
 * @brief       This function is wrapper to hciAllocAndPrepHciEvtPkt and only
 *              add the LE specific event
 *
 * input parameters
 *
 * @param       **pData    - Pointer to pointer that point to buffer to be fill.
 *              hciEvtType - HCI event type.
 *              hciPktLen  - HCI event len.

 *
 * output parameters
 *
 * @param       **pData - Pointer to pointer that point to next slot to be fill.
 *
 * @return      None.
 */
uint8* hciAllocAndPrepHciLeEvtPkt( uint8 **pData, uint8 hciLeEvtType,
                                   uint8 hciPktLen )
{
  uint8 *pEvt = hciAllocAndPrepHciEvtPkt( pData,
                                          HCI_LE_EVENT_CODE,
                                          hciPktLen );
  if ( pEvt )
  {
    ((hciPacket_t *)(pEvt))->pData[HCI_LE_EVENT_CODE_INDEX] = hciLeEvtType;

    *pData = &(((hciPacket_t *)(pEvt))->pData[HCI_EVENT_LE_DATA_OFFSET]);
  }

  // Else pEvt == NUll
  return pEvt;
}

/*******************************************************************************
 * @fn          HCI_getPacketLen
 *
 * @brief       This function calculates and returns the length of the
 *              input HCI packet.
 *
 * input parameters
 *
 * @param       *pEvt   - HCI packet.
 *
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      pktLen - length of input packet or zero for invalid input.
 */
uint16 HCI_getPacketLen( hciPacket_t *pEvt )
{
  uint16 pktLen = 0;
  uint16 cmdLen = 0;

  if ( pEvt != NULL )
  {
    switch ( pEvt->pData[0] )
    {
      case HCI_EVENT_PACKET:
      {
        pktLen = HCI_EVENT_MIN_LENGTH + pEvt->pData[2];
        break;
      }
      case HCI_CMD_PACKET:
      {
        pktLen = HCI_CMD_MIN_LENGTH + pEvt->pData[3];
        break;
      }
      case HCI_EXTENDED_CMD_PACKET:
      {
        cmdLen = BUILD_UINT16( pEvt->pData[3], pEvt->pData[4] );
        // Check for overflow / wraparound of cmdLen
        pktLen = (cmdLen < HCI_MAX_EVT_PKT_SIZE_EXT - HCI_EXT_CMD_MIN_LENGTH) ? HCI_EXT_CMD_MIN_LENGTH + cmdLen : 0;
        break;
      }
      case HCI_EXTENDED_EVENT_PACKET:
      {
        cmdLen = BUILD_UINT16( pEvt->pData[1], pEvt->pData[2] );
        // Check for overflow / wraparound of cmdLen
        pktLen = (cmdLen < HCI_MAX_EVT_PKT_SIZE_EXT - HCI_EVENT_MIN_LENGTH) ? HCI_EVENT_MIN_LENGTH + cmdLen : 0;
        break;
      }
      case HCI_ACL_DATA_PACKET:
      {
        cmdLen = BUILD_UINT16( pEvt->pData[3], pEvt->pData[4] );
        // Check for overflow / wraparound of cmdLen
        pktLen = (cmdLen < HCI_MAX_EVT_PKT_SIZE_EXT - HCI_DATA_MIN_LENGTH) ? HCI_DATA_MIN_LENGTH + cmdLen : 0;
        break;
      }
      default:
      {
        pktLen = 0;
        break;
      }
    }
  }

  return pktLen;
}

/*******************************************************************************
 * @fn          hciCreateEventExtAdvSetTerminated
 *
 * @brief       This function create event for AE advertise set terminate
 *
 * input parameters
 *
 * @param       *pEvtData  - Pointer to event's payload
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void hciCreateEventExtAdvSetTerminated( aeAdvSetTerm_t *pEvtData )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( (pEvtData != NULL)
      && (pEvtData->subCode == HCI_BLE_ADV_SET_TERMINATED_EVENT) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_ADV_SET_TERMINATED_EVENT,
                                       sizeof(aeAdvSetTerm_t) );
    if ( pEvt )
    {
      *pData++ = pEvtData->status;
      *pData++ = pEvtData->handle;
      *pData++ = LO_UINT16( pEvtData->connHandle );
      *pData++ = HI_UINT16( pEvtData->connHandle );
      *pData = pEvtData->numCompAdvEvts;

      // Send message
      HCI_SendEventToHost( pEvt );
    }

    // Free message
    ICall_free( pEvtData );
  }

  else
  {
    hciSendSystemReport( LL_STATUS_ERROR_UNKNOWN_ADV_EVT_TYPE,
                         HCI_BLE_EXTENDED_ADV_REPORT_EVENT );
  }
}

/*******************************************************************************
 * @fn          hciCreateEventExtScanReqReceived
 *
 * @brief       This function create event for AE scan request received
 *
 * input parameters
 *
 * @param       *extAdvRpt  - Pointer to event's payload
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void hciCreateEventExtScanReqReceived( aeScanReqReceived_t *extAdvRpt )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  if ( (extAdvRpt != NULL) && (extAdvRpt->subCode == HCI_BLE_SCAN_REQUEST_RECEIVED_EVENT) )
  {
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_SCAN_REQUEST_RECEIVED_EVENT,
#ifdef QUAL_TEST
                                       // According to specification the channel and rssi are not part of the event packet so exclude them
                                       (sizeof(aeScanReqReceived_t)) - 2 );
#else
                                       sizeof(aeScanReqReceived_t) );
#endif //QUAL_TEST

    if ( pEvt )
    {
      *pData++ = extAdvRpt->handle;
      *pData++ = extAdvRpt->scanAddrType;

      memcpy( pData, extAdvRpt->scanAddr, B_ADDR_LEN );

#ifndef QUAL_TEST
      pData +=  B_ADDR_LEN;

      *pData++ = extAdvRpt->channel;
      *pData   = extAdvRpt->rssi;
#endif //QUAL_TEST

      // Send message
      HCI_SendEventToHost( pEvt );
    }

    //Free message
    ICall_free( extAdvRpt );
  }

  else
  {
    hciSendSystemReport( LL_STATUS_ERROR_UNKNOWN_ADV_EVT_TYPE,
                         HCI_BLE_EXTENDED_ADV_REPORT_EVENT );
  }
}

/*******************************************************************************
 * @fn          hciCreateEventExtAdv
 *
 * @brief       Create and send vendor specific event for Advertising Extension
 *
 * input parameters
 *
 * @param       eventId  - event id
 *              handle - handle of Adv set
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void hciCreateEventExtAdv( uint8 eventId, uint8 handle )
{
  uint8 data[HCI_EXT_ADV_EVENT_LEN];

  data[0] = LO_UINT16( HCI_EXT_LE_ADV_EVENT );
  data[1] = HI_UINT16( HCI_EXT_LE_ADV_EVENT );
  data[2] = eventId;
  data[3] = handle;

  hciSendVendorSpecificEvent( data, HCI_EXT_ADV_EVENT_LEN );
}

/*******************************************************************************
 * @fn          hciCreateEventExtAdvDataTruncated
 *
 * @brief       This function create event for AE advertise data truncated
 *
 * input parameters
 *
 * @param       *extAdvTrunc  - Pointer to event's payload
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void hciCreateEventExtAdvDataTruncated( aeAdvTrucData_t *extAdvTrunc )
{
  uint8 data[HCI_EXT_ADV_DATA_TRUNCATED_EVENT_LEN];

  if ( extAdvTrunc != NULL )
  {
    data[0] = LO_UINT16( HCI_EXT_LE_ADV_EVENT );
    data[1] = HI_UINT16( HCI_EXT_LE_ADV_EVENT );
    data[2] = LL_CBACK_EXT_ADV_DATA_TRUNCATED;
    data[3] = extAdvTrunc->handle;
    data[4] = LO_UINT16( extAdvTrunc->advDataLen );
    data[5] = HI_UINT16( extAdvTrunc->advDataLen );
    data[6] = extAdvTrunc->availAdvDataLen;

    hciSendVendorSpecificEvent( data, HCI_EXT_ADV_DATA_TRUNCATED_EVENT_LEN );

    ICall_free( extAdvTrunc );
  }
}

/*******************************************************************************
 * @fn          hciCreateEventExtAdvReport
 *
 * @brief       This function create event for AE advertise report
 *
 * input parameters
 *
 * @param       *pExtAdvRpt  - Pointer to event's payload
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void hciCreateEventExtAdvReport( aeExtAdvRptEvt_t *pExtAdvRpt )
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  // DEBUG CODE.
  // Add a Filter on RSSI to avoid being flooded and doomed by
  // all the crazy IOT devicein the world.
  // Uncoment the following to only report Adv with RSSI high enough
  // so they match teh board that are less than 1m from each other.
  //if (pExtAdvRpt->rssi < -37)
  //{
  //  // Reject this report...
  //
  //}
  //else
  // END of Debug Code

  if ( pExtAdvRpt->subCode == HCI_BLE_EXTENDED_ADV_REPORT_EVENT )
  {
    uint8 dataLen = MAX_REPORT_DATA_SIZE;
    uint8 remainingLength = pExtAdvRpt->dataLen;
    // Got the Report, Map it to the Extended Report Event...
    do
    {
      //Check Length, if shorter than MAX_REPORT_DATA_SIZE, change datalen to remainingLength
      //Otherwise send fragment packet with max size
      if ( remainingLength < MAX_REPORT_DATA_SIZE )
      {
        dataLen = remainingLength;
      }

      pEvt = hciAllocAndPrepHciEvtPkt( &pData,
                                       HCI_LE_EVENT_CODE,
                                       HCI_AE_EVENT_LENGTH + dataLen );

      // Save head for update extType later
      uint8 *pHead = pData;

      if ( pEvt )
      {
        // We keep all the information the same across report, only the data type will change.
        *pData++ = pExtAdvRpt->subCode;
        *pData++ = pExtAdvRpt->numRpts;
        *pData++ = LO_UINT16( pExtAdvRpt->evtType );
        *pData++ = HI_UINT16( pExtAdvRpt->evtType );

        // Update the type to incomplete with more data to follow
        if ( remainingLength > MAX_REPORT_DATA_SIZE )
        {
          // This is not the last packet
          ((aeExtAdvRptEvt_t*) pHead)->evtType &= AE_EVT_TYPE_COMPLETE_MASK;
          ((aeExtAdvRptEvt_t*) pHead)->evtType |= AE_EVT_TYPE_INCOMPLETE_MORE_TO_COME;
        }

        *pData++ = pExtAdvRpt->addrType;

        memcpy( pData, pExtAdvRpt->addr, B_ADDR_LEN );
        pData += B_ADDR_LEN;

        *pData++ = pExtAdvRpt->primPhy;
        *pData++ = pExtAdvRpt->secPhy;
        *pData++ = pExtAdvRpt->advSid;
        *pData++ = pExtAdvRpt->txPower;
        *pData++ = pExtAdvRpt->rssi;
        *pData++ = LO_UINT16( pExtAdvRpt->periodicAdvInt );
        *pData++ = HI_UINT16( pExtAdvRpt->periodicAdvInt );
        *pData++ = pExtAdvRpt->directAddrType;

        memcpy( pData, pExtAdvRpt->directAddr, B_ADDR_LEN );
        pData += B_ADDR_LEN;

        *pData = dataLen;

        if ( dataLen )
        {
          pData++;
          memcpy( pData,
                  pExtAdvRpt->pData + (pExtAdvRpt->dataLen - remainingLength),
                  dataLen );
        }

        // Send message
        HCI_SendEventToHost( pEvt );
      }

      else
      {
        // Out of memory event send inside hciAllocAndPrepHciLeEvtPkt()
        break;
      }

      // Update the local variable to send the rest of the payload.
      remainingLength -= dataLen;
    }
    while ( remainingLength > 0 );
  }

  else
  {
    hciSendSystemReport( LL_STATUS_ERROR_UNKNOWN_ADV_EVT_TYPE,
                         HCI_BLE_EXTENDED_ADV_REPORT_EVENT );
  }

  if ( pExtAdvRpt->pData )
  {
    ICall_free( pExtAdvRpt->pData );
  }

  ICall_free( pExtAdvRpt );
}

/*******************************************************************************
 * @fn          hciCreateEventAdvReport
 *
 * @brief       This function creates a legacy advertising report event from
 *              an extended advertising report. It converts the extended format
 *              (BT5+) to legacy format (BT4.x) for backward compatibility.
 *
 * input parameters
 *
 * @param       *pExtAdvRpt  - Pointer to extended advertising report data
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void hciCreateEventAdvReport( aeExtAdvRptEvt_t *pExtAdvRpt )
{
  uint8 *pEvt;
  uint8 *pData;
  uint8 dataLen;
  uint8 legacyEvtType;

  // Validate input
  if ( (pExtAdvRpt == NULL) || (pExtAdvRpt->subCode != HCI_BLE_EXTENDED_ADV_REPORT_EVENT) )
  {
    hciSendSystemReport( LL_STATUS_ERROR_UNKNOWN_ADV_EVT_TYPE,
                         HCI_BLE_EXTENDED_ADV_REPORT_EVENT );
    return;
  }

#ifdef QUAL_TEST
  // Handle directed advertising with unresolved RPA (qualification test requirement)
  if ( pExtAdvRpt->directAddrType == AE_EXT_ADV_RPT_DIR_ADDR_TYPE_UNRESOLVED_RPA )
  {
    // Create HCI_BLE_DIRECT_ADVERTISING_REPORT_EVENT for directed advertising
    pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                       HCI_BLE_DIRECT_ADVERTISING_REPORT_EVENT,
                                       HCI_ADV_DIRECTED_REPORT_EVENT_LEN );

    if ( pEvt )
    {
      *pData++ = pExtAdvRpt->numRpts;        // Number of reports
      *pData++ = 1;                          // Event type: Connectable directed advertising (ADV_DIRECT_IND)
      *pData++ = pExtAdvRpt->addrType;       // Address type

      memcpy( pData, pExtAdvRpt->addr, B_ADDR_LEN );  // Advertiser address
      pData += B_ADDR_LEN;

      *pData++ = LL_DEV_ADDR_TYPE_RANDOM;    // Direct address type (random for unresolved RPA)

      memcpy( pData, pExtAdvRpt->directAddr, B_ADDR_LEN );  // Direct address (target)
      pData += B_ADDR_LEN;

      *pData = pExtAdvRpt->rssi;             // RSSI

      // Send message to host
      HCI_SendEventToHost( pEvt );
    }

    // Free the extended advertising report memory
    if ( pExtAdvRpt->pData )
    {
      ICall_free( pExtAdvRpt->pData );
    }
    ICall_free( pExtAdvRpt );
    return;
  }
#endif

  // Check if LE Meta-Events are enabled and this event is enabled
  if ( MAP_HCI_CheckEventMaskLe( LE_EVT_ADV_REPORT_BIT ) == 0 )
  {
    // Event mask not set for this event, skip it
    if ( pExtAdvRpt->pData )
    {
      ICall_free( pExtAdvRpt->pData );
    }
    ICall_free( pExtAdvRpt );
    return;
  }

  // Convert extended advertising event type to legacy event type
  switch ( pExtAdvRpt->evtType )
  {
    case AE_EXT_ADV_RPT_EVT_TYPE_ADV_IND:
      legacyEvtType = 0;  // ADV_IND
      break;
    case AE_EXT_ADV_RPT_EVT_TYPE_DIRECT_IND:
      legacyEvtType = 1;  // ADV_DIRECT_IND
      break;
    case AE_EXT_ADV_RPT_EVT_TYPE_SCAN_IND:
      legacyEvtType = 2;  // ADV_SCAN_IND
      break;
    case AE_EXT_ADV_RPT_EVT_TYPE_NONCONN_IND:
      legacyEvtType = 3;  // ADV_NONCONN_IND
      break;
    case AE_EXT_ADV_RPT_EVT_TYPE_SCAN_RSP_ADV_IND:
    case AE_EXT_ADV_RPT_EVT_TYPE_SCAN_RSP_ADV_SCAN_IND:
    case AE_EXT_ADV_RPT_EVT_TYPE_SCAN_RSP:
      legacyEvtType = 4;  // SCAN_RSP
      break;
    default:
      // Unsupported event type for legacy format, discard
      if ( pExtAdvRpt->pData )
      {
        ICall_free( pExtAdvRpt->pData );
      }
      ICall_free( pExtAdvRpt );
      return;
  }

  // Allocate and prepare HCI LE event packet
  dataLen = pExtAdvRpt->dataLen;
  pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                     HCI_BLE_ADV_REPORT_EVENT,
                                     HCI_ADV_REPORT_EVENT_LEN + dataLen );

  if ( pEvt )
  {
    // Populate legacy advertising report data
    *pData++ = pExtAdvRpt->numRpts;      // Number of reports (always 1)
    *pData++ = legacyEvtType;            // Event type (converted to legacy)
    *pData++ = pExtAdvRpt->addrType;     // Address type

    memcpy( pData, pExtAdvRpt->addr, B_ADDR_LEN );  // Device address
    pData += B_ADDR_LEN;

    *pData++ = dataLen;                  // Data length

    if ( dataLen )
    {
      memcpy( pData, pExtAdvRpt->pData, dataLen );  // Advertising data
      pData += dataLen;
    }

    *pData = pExtAdvRpt->rssi;           // RSSI

    // Send message to host
    HCI_SendEventToHost( pEvt );
  }

  // Free the extended advertising report memory
  if ( pExtAdvRpt->pData )
  {
    ICall_free( pExtAdvRpt->pData );
  }
  ICall_free( pExtAdvRpt );
}

/*******************************************************************************
 * @fn          hciCreateEventExtScanTimeout
 *
 * @brief       This function create event for AE scan timeout
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void hciCreateEventExtScanTimeout()
{
  uint8 *pEvt;
  // Pointer to data inside pEvt, that pointer point next slot to be filled
  uint8 *pData;

  pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                     HCI_BLE_SCAN_TIMEOUT_EVENT,
                                     sizeof(aeScanTimeout_t) );
  if ( pEvt )
  {
    // Send message
    HCI_SendEventToHost( pEvt );
  }
}

/*******************************************************************************
 * @fn          hciCreateEventExtScan
 *
 * @brief       This function create event for AE scan
 *
 * input parameters
 *
 * @param       event  - Scan event type
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void hciCreateEventExtScan( uint8 event )
{
  uint8_t data[HCI_EXT_SCAN_EVENT_LEN];

  data[0] = LO_UINT16( HCI_EXT_LE_SCAN_EVENT );
  data[1] = HI_UINT16( HCI_EXT_LE_SCAN_EVENT );
  data[2] = event;

  hciSendVendorSpecificEvent( data, HCI_EXT_SCAN_EVENT_LEN );
}

/*******************************************************************************
 * @fn          hciCheckEventMask
 *
 * @brief       This API is used to check if input event bit is enable
 *
 * input parameters
 *
 * @param       eventBit            - Which bit to check
 *              eventMaskTableIndex - Which table to search in
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      TRUE/FALSE.
 */
uint8 hciCheckEventMask( uint8 eventBit, uint8 eventMaskTableIndex )
{
  uint8 status = FALSE;
  uint8 byteIndex = GET_BYTE_INDEX( eventbit );
  uint8 bitOffset = GET_BIT_OFFSET( eventbit );

  // Sanity check and check if LE event bit is enable
  if ( bitOffset < B_EVENT_MASK_LEN
      && (hciEvtMask[eventMaskTableIndex][byteIndex] >> bitOffset) & TRUE )
  {
    status = TRUE;
  }

  return status;
}

/*******************************************************************************
 * @fn          hciSetEventMask
 *
 * @brief       This function is used to set the HCI event mask, which is
 *              used to determine which events are supported.
 *
 * input parameters
 *
 * @param      *pEventMask          - Event mask to be copy
 *              eventMaskTableIndex - Which table to search in
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      SUCCESS/FAILURE
 */
uint8 hciSetEventMask( uint8 *pEventMask, uint8 eventMaskTableIndex )
{
  uint8 status = HCI_ERROR_CODE_INVALID_HCI_CMD_PARAMS;

  if ( pEventMask != NULL )
  {
    memcpy( hciEvtMask[eventMaskTableIndex], pEventMask, B_EVENT_MASK_LEN );
    status = SUCCESS;
  }

  return status;
}

/*******************************************************************************
 */
