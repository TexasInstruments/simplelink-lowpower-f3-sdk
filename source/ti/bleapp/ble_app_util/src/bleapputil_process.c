/******************************************************************************

@file  BLEAppUtil_process.c

@brief This file contains the BLEAppUtil module events processing functions

Group: WCS, BTS
Target Device: cc23xx

******************************************************************************

 Copyright (c) 2022-2024, Texas Instruments Incorporated
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


/*********************************************************************
 * INCLUDES
 */
#include <ti/bleapp/ble_app_util/inc/bleapputil_api.h>
#include <ti/bleapp/ble_app_util/inc/bleapputil_internal.h>

/*********************************************************************
 * MACROS
 */
#define BLEAPPUTIL_PAIR_STATE_TABLE_SIZE 7

#define BLEAPPUTIL_GAP_PERIODIC_TABLE_SIZE      14
#define BLEAPPUTIL_GAP_PERIODIC_EVENTS_OFFSET   0x19

#define BLEAPPUTIL_GAP_CONN_TABLE_SIZE          20
#define BLEAPPUTIL_GAP_CONN_EVENTS_OFFSET       0x05

#define BLEAPPUTIL_GATT_TABLE_SIZE              38

#define BLEAPPUTIL_L2CAP_SIGNAL_EVENTS_OFFSET   0x60
#define BLEAPPUTIL_L2CAP_SIGNAL_TABLE_SIZE      6

/*********************************************************************
* CONSTANTS
*/
// The following look up table is used to convert PairState events
// received from the BLE stack to BLEAppUtil PairState events
const uint8_t pairStateLookupTable[BLEAPPUTIL_PAIR_STATE_TABLE_SIZE] =
{
    BLEAPPUTIL_PAIRING_STATE_STARTED,
    BLEAPPUTIL_PAIRING_STATE_COMPLETE,
    BLEAPPUTIL_PAIRING_STATE_ENCRYPTED,
    BLEAPPUTIL_PAIRING_STATE_BOND_SAVED,
    BLEAPPUTIL_PAIRING_STATE_CAR_READ,
    BLEAPPUTIL_PAIRING_STATE_RPAO_READ,
    BLEAPPUTIL_GENERATE_ECC_DONE
};

// The following look up table is used to convert GAP periodic events
// received from the BLE stack to BLEAppUtil periodic events
const uint32_t periodicEventsLookupTable[BLEAPPUTIL_GAP_PERIODIC_TABLE_SIZE] =
{
    BLEAPPUTIL_ADV_SET_PERIODIC_ADV_PARAMS_EVENT,
    BLEAPPUTIL_ADV_SET_PERIODIC_ADV_DATA_EVENT,
    BLEAPPUTIL_ADV_SET_PERIODIC_ADV_ENABLE_EVENT,
    BLEAPPUTIL_SCAN_CREATE_SYNC_EVENT,
    BLEAPPUTIL_SCAN_SYNC_CANCEL_EVENT,
    BLEAPPUTIL_SCAN_TERMINATE_SYNC_EVENT,
    BLEAPPUTIL_SCAN_PERIODIC_RECEIVE_EVENT,
    BLEAPPUTIL_SCAN_ADD_DEVICE_ADV_LIST_EVENT,
    BLEAPPUTIL_SCAN_REMOVE_DEVICE_ADV_LIST_EVENT,
    BLEAPPUTIL_SCAN_READ_ADV_LIST_SIZE_EVENT,
    BLEAPPUTIL_SCAN_CLEAR_ADV_LIST_EVENT,
    BLEAPPUTIL_SCAN_PERIODIC_ADV_SYNC_EST_EVENT,
    BLEAPPUTIL_SCAN_PERIODIC_ADV_SYNC_LOST_EVENT,
    BLEAPPUTIL_SCAN_PERIODIC_ADV_REPORT_EVENT
};

// The following look up table is used to convert GAP connection related
// events received from the BLE stack to BLEAppUtil GAP Conn events
const uint32_t gapConnEventsLookupTable[BLEAPPUTIL_GAP_CONN_TABLE_SIZE] =
{
    BLEAPPUTIL_LINK_ESTABLISHED_EVENT,
    BLEAPPUTIL_LINK_TERMINATED_EVENT,
    BLEAPPUTIL_LINK_PARAM_UPDATE_EVENT,
    0,
    BLEAPPUTIL_SIGNATURE_UPDATED_EVENT,
    BLEAPPUTIL_AUTHENTICATION_COMPLETE_EVENT,
    BLEAPPUTIL_PASSKEY_NEEDED_EVENT,
    BLEAPPUTIL_PERIPHERAL_REQUESTED_SECURITY_EVENT,
    0,
    BLEAPPUTIL_BOND_COMPLETE_EVENT,
    BLEAPPUTIL_PAIRING_REQ_EVENT,
    BLEAPPUTIL_AUTHENTICATION_FAILURE_EVT,
    BLEAPPUTIL_LINK_PARAM_UPDATE_REQ_EVENT,
    0,
    0,
    0,
    BLEAPPUTIL_CONNECTING_CANCELLED_EVENT,
    0,
    BLEAPPUTIL_BOND_LOST_EVENT,
    BLEAPPUTIL_LINK_PARAM_UPDATE_REJECT_EVENT
};

// The following look up table is used to convert GATT events received from
// the BLE stack to BLEAppUtil GATT events
const uint32_t gattEventsLookupTable[BLEAPPUTIL_GATT_TABLE_SIZE] =
{
     0,
     BLEAPPUTIL_ATT_ERROR_RSP,
     BLEAPPUTIL_ATT_EXCHANGE_MTU_REQ,
     BLEAPPUTIL_ATT_EXCHANGE_MTU_RSP,
     BLEAPPUTIL_ATT_FIND_INFO_REQ,
     BLEAPPUTIL_ATT_FIND_INFO_RSP,
     BLEAPPUTIL_ATT_FIND_BY_TYPE_VALUE_REQ,
     BLEAPPUTIL_ATT_FIND_BY_TYPE_VALUE_RSP,
     BLEAPPUTIL_ATT_READ_BY_TYPE_REQ,
     BLEAPPUTIL_ATT_READ_BY_TYPE_RSP,
     BLEAPPUTIL_ATT_READ_REQ,
     BLEAPPUTIL_ATT_READ_RSP,
     BLEAPPUTIL_ATT_READ_BLOB_REQ,
     BLEAPPUTIL_ATT_READ_BLOB_RSP,
     BLEAPPUTIL_ATT_READ_MULTI_REQ,
     BLEAPPUTIL_ATT_READ_MULTI_RSP,
     BLEAPPUTIL_ATT_READ_BY_GRP_TYPE_REQ,
     BLEAPPUTIL_ATT_READ_BY_GRP_TYPE_RSP,
     BLEAPPUTIL_ATT_WRITE_REQ,
     BLEAPPUTIL_ATT_WRITE_RSP,
     0,
     0,
     BLEAPPUTIL_ATT_PREPARE_WRITE_REQ,
     BLEAPPUTIL_ATT_PREPARE_WRITE_RSP,
     BLEAPPUTIL_ATT_EXECUTE_WRITE_REQ,
     BLEAPPUTIL_ATT_EXECUTE_WRITE_RSP,
     0,
     BLEAPPUTIL_ATT_HANDLE_VALUE_NOTI,
     0,
     BLEAPPUTIL_ATT_HANDLE_VALUE_IND,
     BLEAPPUTIL_ATT_HANDLE_VALUE_CFM,
     0,
     BLEAPPUTIL_ATT_UNSUPPORTED_METHOD,
     BLEAPPUTIL_ATT_WRITE_CMD,
     BLEAPPUTIL_ATT_SIGNED_WRITE_CMD,
     BLEAPPUTIL_ATT_TRANSACTION_READY_EVENT,
     BLEAPPUTIL_ATT_FLOW_CTRL_VIOLATED_EVENT,
     BLEAPPUTIL_ATT_MTU_UPDATED_EVENT
};

// The following look up table is used to convert L2CAP Signal events received
// from the BLE stack to BLEAppUtil L2CAP Signal events
const uint32_t l2capSignalEventsLookupTable[BLEAPPUTIL_L2CAP_SIGNAL_TABLE_SIZE] =
{
    BLEAPPUTIL_L2CAP_CHANNEL_ESTABLISHED_EVT,
    BLEAPPUTIL_L2CAP_CHANNEL_TERMINATED_EVT,
    BLEAPPUTIL_L2CAP_OUT_OF_CREDIT_EVT,
    BLEAPPUTIL_L2CAP_PEER_CREDIT_THRESHOLD_EVT,
    BLEAPPUTIL_L2CAP_SEND_SDU_DONE_EVT,
    BLEAPPUTIL_L2CAP_NUM_CTRL_DATA_PKT_EVT
};

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* GLOBAL VARIABLES
*/
StackInitDone_t appInitDoneHandler;

/*********************************************************************
* LOCAL VARIABLES
*/

/*********************************************************************
* LOCAL FUNCTIONS
*/


/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      BLEAppUtil_processStackEvents
 *
 * @brief   Process the stack event dequeued from the BLEAPPUtil queue
 *          and calls the relevant event handle using @refs BLEAppUtil_callEventHandler.
 *          There are 2 special cases:
 *          - GAP_DEVICE_INIT_DONE_EVENT - the application callback is called
 *                                         directly.
 *          - L2CAP_DATA_EVENT - there is only data attached, no sub events.
 *
 * @param   pMsg - The msg containing the data to send to the application
 * @param   bleAppUtilEventAndHandle - the event and handler
 *
 * @return  none
 */
void BLEAppUtil_processStackEvents(BLEAppUtil_msgHdr_t *pMsg ,BLEAppUtil_eventAndHandlerType_t bleAppUtilEventAndHandle)
{
    // If a device init done event was received handle it directly since it's independent,
    // it's not part of any of the event handler types (it has it's own callback).
    if((pMsg->event == GAP_MSG_EVENT) && ((gapEventHdr_t *)pMsg)->opcode == GAP_DEVICE_INIT_DONE_EVENT)
    {
        // This event will not be received in one of the event handlers
        // registered by the application, the StackInitDone_t handler provided
        // by BLEAppUtil_init is called.
        if(appInitDoneHandler != NULL)
        {
            appInitDoneHandler((gapDeviceInitDoneEvent_t *)pMsg);
        }
    }
    // All other BLE stack events are handled by the events handlers
    else
    {
        // Pass the event and msg to BLEAppUtil_callEventHandler which calls the
        // handler of the application
        BLEAppUtil_callEventHandler(bleAppUtilEventAndHandle.event,
                                    pMsg,
                                    bleAppUtilEventAndHandle.handlerType);

        // Free the data of a CTRL_TO_HOST_EVENT if exist
        if(pMsg->event == HCI_CTRL_TO_HOST_EVENT)
        {
            hciPacket_t *pBuf = (hciPacket_t *)pMsg;
            if(pBuf->pData != NULL)
            {
                switch (pBuf->pData[0])
                {
                    case HCI_ACL_DATA_PACKET:
                    case HCI_SCO_DATA_PACKET:
                        if(pBuf->pData != NULL)
                        {
                            BM_free(pBuf->pData);
                        }
                    default:
                      break;
                }
            }
        }
    }
}

/*********************************************************************
 * @fn      isStackEventRequired
 *
 * @brief   Checks if the event is required by the application
 *          level.
 *          If it is required return true, else return false.
 *
 * @param   pMsg - The msg containing the data to send to the application
 * @param   bleAppUtilEventAndHandle - structure of the event in BLE App Util
 *                                     format and handler type, the function
 *                                     fill these parameters
 *
 * @return  True/False
 */
uint8_t BLEAppUtil_isStackEventRequired(BLEAppUtil_msgHdr_t *pMsg, BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle)
{
    uint8_t isRequired = false;

    switch (pMsg->event)
    {
        case GAP_MSG_EVENT:
            isRequired = BLEAppUtil_convertGAPEvents(pMsg, bleAppUtilEventAndHandle);
            break;

        case GATT_MSG_EVENT:
            isRequired = BLEAppUtil_convertGATTEvents(pMsg, bleAppUtilEventAndHandle);
            break;

        case L2CAP_DATA_EVENT:
            isRequired = BLEAppUtil_convertL2CAPDataMsg(pMsg, bleAppUtilEventAndHandle);
            break;

        case L2CAP_SIGNAL_EVENT:
            isRequired = BLEAppUtil_convertL2CAPSignalEvents(pMsg, bleAppUtilEventAndHandle);
            break;

        case HCI_GAP_EVENT_EVENT:
            isRequired = BLEAppUtil_convertHCIGAPEvents(pMsg, bleAppUtilEventAndHandle);
            break;

        case HCI_DATA_EVENT:
            isRequired = BLEAppUtil_convertHCIDataEvents(pMsg, bleAppUtilEventAndHandle);
            break;

        case HCI_SMP_EVENT_EVENT:
            isRequired = BLEAppUtil_convertHCISMPEvents(pMsg, bleAppUtilEventAndHandle);
            break;

        case HCI_SMP_META_EVENT_EVENT:
            isRequired = BLEAppUtil_convertHCISMPMetaEvents(pMsg, bleAppUtilEventAndHandle);
            break;

        case HCI_CTRL_TO_HOST_EVENT:
            isRequired = BLEAppUtil_convertHCICTRLToHostEvents(pMsg, bleAppUtilEventAndHandle);
            break;

        default:
            break;
    }
    return isRequired;
}

/*********************************************************************
 * @fn      BLEAppUtil_convertGAPEvents
 *
 * @brief   Process GAP events received from the BLE stack, called
 *          from BLE Stack context.
 *          This function converts the BLE stack events to BLEAppUtil
 *          events (which are bit mask events) and checks if the event
 *          is required by the application level.
 *          If the event is required, fills the bleAppUtilEventAndHandle
 *          structure with the event and handler type.
 *
 * @param   pMsg - The msg containing the data to send to the application
 * @param   bleAppUtilEventAndHandle - structure of the event in BLE App Util
 *                                     format and handler type
 *
 * @return  True/False
 */
uint8_t BLEAppUtil_convertGAPEvents(BLEAppUtil_msgHdr_t *pMsg, BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle)
{
    gapEventHdr_t * pMsgData = (gapEventHdr_t *)pMsg;
    BLEAppUtil_eventHandlerType_e handlerType;
    uint8_t isRequired = false;
    uint32_t event = 0;

    switch(pMsgData->opcode)
    {
        // This event will not be received in one of the event handlers
        // registered by the application, the StackInitDone_t handler provided
        // by BLEAppUtil_init is called.
        case GAP_DEVICE_INIT_DONE_EVENT:
        {
            isRequired = true;
            break;
        }

        // The following events are converted to events from type of
        // BLEAPPUTIL_GAP_CONN_TYPE
        case GAP_LINK_ESTABLISHED_EVENT:
        case GAP_LINK_TERMINATED_EVENT:
        case GAP_LINK_PARAM_UPDATE_EVENT:
        case GAP_SIGNATURE_UPDATED_EVENT:
        case GAP_AUTHENTICATION_COMPLETE_EVENT:
        case GAP_PASSKEY_NEEDED_EVENT:
        case GAP_PERIPHERAL_REQUESTED_SECURITY_EVENT:
        case GAP_BOND_COMPLETE_EVENT:
        case GAP_PAIRING_REQ_EVENT:
        case GAP_AUTHENTICATION_FAILURE_EVT:
        case GAP_UPDATE_LINK_PARAM_REQ_EVENT:
        case GAP_CONNECTING_CANCELLED_EVENT:
        case GAP_BOND_LOST_EVENT:
        case GAP_LINK_PARAM_UPDATE_REJECT_EVENT:
        {
            // Get the "BLE App Util" event format
            event = gapConnEventsLookupTable[pMsgData->opcode - BLEAPPUTIL_GAP_CONN_EVENTS_OFFSET];
            // Handler type for these events
            handlerType = BLEAPPUTIL_GAP_CONN_TYPE;
            // Get the event mask of handlers form BLEAPPUTIL_GAP_CONN_TYPE
            isRequired = BLEAppUtil_isEventEnabled(BLEAPPUTIL_GAP_CONN_TYPE, event);
            break;
        }

        // The following events are converted to events from type of
        // BLEAPPUTIL_GAP_PERIODIC_TYPE
        case GAP_ADV_SET_PERIODIC_ADV_PARAMS_EVENT:
        case GAP_ADV_SET_PERIODIC_ADV_DATA_EVENT:
        case GAP_ADV_SET_PERIODIC_ADV_ENABLE_EVENT:
        case GAP_SCAN_CREATE_SYNC_EVENT:
        case GAP_SCAN_SYNC_CANCEL_EVENT:
        case GAP_SCAN_TERMINATE_SYNC_EVENT:
        case GAP_SCAN_PERIODIC_RECEIVE_EVENT:
        case GAP_SCAN_ADD_DEVICE_ADV_LIST_EVENT:
        case GAP_SCAN_REMOVE_DEVICE_ADV_LIST_EVENT:
        case GAP_SCAN_READ_ADV_LIST_SIZE_EVENT:
        case GAP_SCAN_CLEAR_ADV_LIST_EVENT:
        case GAP_SCAN_PERIODIC_ADV_SYNC_EST_EVENT:
        case GAP_SCAN_PERIODIC_ADV_SYNC_LOST_EVENT:
        case GAP_SCAN_PERIODIC_ADV_REPORT_EVENT:
        {
            // Get the "BLE App Util" event format
            event = periodicEventsLookupTable[pMsgData->opcode - BLEAPPUTIL_GAP_PERIODIC_EVENTS_OFFSET];
            // Handler type for these events
            handlerType = BLEAPPUTIL_GAP_PERIODIC_TYPE;
            // Get the event mask of handlers form BLEAPPUTIL_GAP_PERIODIC_TYPE
            isRequired = BLEAppUtil_isEventEnabled(BLEAPPUTIL_GAP_PERIODIC_TYPE, event);
            break;
        }
        default:
        {
            break;
        }
    }

    // If the received event is required by the application, save
    // the event and handler
    if(isRequired && (pMsgData->opcode != GAP_DEVICE_INIT_DONE_EVENT))
    {
        bleAppUtilEventAndHandle->event = event;
        bleAppUtilEventAndHandle->handlerType = handlerType;
    }
    return isRequired;
}

/*********************************************************************
 * @fn      BLEAppUtil_convertGATTEvents
 *
 * @brief   Process GATT events received from the BLE stack, called
 *          from BLE Stack context.
 *          This function converts the BLE stack events to BLEAppUtil
 *          events (which are bit mask events) and checks if the event
 *          is required by the application level.
 *          If the event is required, fills the bleAppUtilEventAndHandle
 *          structure with the event and handler type.
 *          All the events processed in this function will be received by
 *          handlers form type of BLEAPPUTIL_GATT_TYPE
 *
 * @param   pMsg - The msg containing the data to send to the application
 * @param   bleAppUtilEventAndHandle - structure of the event in BLE App Util
 *                                     format and handler type
 *
 * @return  True/False
 */
uint8_t BLEAppUtil_convertGATTEvents(BLEAppUtil_msgHdr_t *pMsg, BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle)
{
    gattMsgEvent_t * pMsgData = (gattMsgEvent_t *)pMsg;
    uint8_t isRequired = false;
    uint32_t event = 0;

    switch (pMsgData->method)
    {
        case ATT_WRITE_CMD:
        {
            event = BLEAPPUTIL_ATT_WRITE_CMD;
            break;
        }

        case ATT_SIGNED_WRITE_CMD:
        {
            event = BLEAPPUTIL_ATT_SIGNED_WRITE_CMD;
            break;
        }

        case ATT_TRANSACTION_READY_EVENT:
        {
            event = BLEAPPUTIL_ATT_TRANSACTION_READY_EVENT;
            break;
        }

        case ATT_FLOW_CTRL_VIOLATED_EVENT:
        {
            event = BLEAPPUTIL_ATT_FLOW_CTRL_VIOLATED_EVENT;
            break;
        }

        case ATT_MTU_UPDATED_EVENT:
        {
            event = BLEAPPUTIL_ATT_MTU_UPDATED_EVENT;
            break;
        }

        default:
        {
            event = gattEventsLookupTable[pMsgData->method];
            break;
        }
    }

    // Get the event mask of handlers form BLEAPPUTIL_GATT_TYPE
    isRequired = BLEAppUtil_isEventEnabled(BLEAPPUTIL_GATT_TYPE, event);
    // Check if the received event is required by the application
    if(isRequired)
    {
        bleAppUtilEventAndHandle->event = event;
        bleAppUtilEventAndHandle->handlerType = BLEAPPUTIL_GATT_TYPE;
    }
    return isRequired;
}

/*********************************************************************
 * @fn      BLEAppUtil_convertHCIGAPEvents
 *
 * @brief   Process HCI GAP events received from the BLE stack, called
 *          from BLE Stack context.
 *          This function converts the BLE stack events to BLEAppUtil
 *          events (which are bit mask events) and checks if the event
 *          is required by the application level.
 *          If the event is required, fills the bleAppUtilEventAndHandle
 *          structure with the event and handler type.
 *          All the events processed in this function will be received by
 *          handlers form type of BLEAPPUTIL_HCI_GAP_TYPE
 *
 * @param   pMsg - The msg containing the data to send to the application
 * @param   bleAppUtilEventAndHandle - structure of the event in BLE App Util
 *                                     format and handler type
 *
 * @return  True/False
 */
uint8_t BLEAppUtil_convertHCIGAPEvents(BLEAppUtil_msgHdr_t *pMsg, BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle)
{
    ICall_Hdr * pMsgData = (ICall_Hdr *)pMsg;
    uint8_t isRequired = false;
    uint32_t event = 0;

    switch(pMsgData->status)
    {
        case HCI_DISCONNECTION_COMPLETE_EVENT_CODE:
        {
            event = BLEAPPUTIL_HCI_DISCONNECTION_COMPLETE_EVENT_CODE;
            break;
        }

        case HCI_COMMAND_COMPLETE_EVENT_CODE:
        {
            event = BLEAPPUTIL_HCI_COMMAND_COMPLETE_EVENT_CODE;
            break;
        }

        case HCI_COMMAND_STATUS_EVENT_CODE:
        {
            event = BLEAPPUTIL_HCI_COMMAND_STATUS_EVENT_CODE;
            break;
        }

        case HCI_LE_EVENT_CODE:
        {
            event = BLEAPPUTIL_HCI_LE_EVENT_CODE;
            break;
        }

        case HCI_VE_EVENT_CODE:
        {
            event = BLEAPPUTIL_HCI_VE_EVENT_CODE;
            break;
        }

        case HCI_BLE_HARDWARE_ERROR_EVENT_CODE:
        {
            event = BLEAPPUTIL_HCI_BLE_HARDWARE_ERROR_EVENT_CODE;
            break;
        }

        default:
        {
            break;
        }
    }

    // Get the event mask of handlers form BLEAPPUTIL_HCI_GAP_TYPE
    isRequired = BLEAppUtil_isEventEnabled(BLEAPPUTIL_HCI_GAP_TYPE, event);
    // Check if the received event is required by the application
    if(isRequired)
    {
        bleAppUtilEventAndHandle->event = event;
        bleAppUtilEventAndHandle->handlerType = BLEAPPUTIL_HCI_GAP_TYPE;
    }
    return isRequired;
}

/*********************************************************************
 * @fn      BLEAppUtil_convertHCIDataEvents
 *
 * @brief   Process HCI Data events received from the BLE stack, called
 *          from BLE Stack context.
 *          This function converts the BLE stack events to BLEAppUtil
 *          events (which are bit mask events) and checks if the event
 *          is required by the application level.
 *          If the event is required, fills the bleAppUtilEventAndHandle
 *          structure with the event and handler type.
 *          All the events processed in this function will be received by
 *          handlers form type of BLEAPPUTIL_HCI_DATA_TYPE
 *
 * @param   pMsg - The msg containing the data to send to the application
 * @param   bleAppUtilEventAndHandle - structure of the event in BLE App Util
 *                                     format and handler type
 *
 * @return  True/False
 */
uint8_t BLEAppUtil_convertHCIDataEvents(bleStack_msgHdt_t *pMsg, BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle)
{
    hciDataEvent_t * pMsgData = (hciDataEvent_t *)pMsg;
    uint8_t isRequired = false;
    uint32_t event = 0;

    switch(pMsgData->hdr.status)
    {
        case HCI_NUM_OF_COMPLETED_PACKETS_EVENT_CODE:
        {
            event = BLEAPPUTIL_HCI_NUM_OF_COMPLETED_PACKETS_EVENT_CODE;
            break;
        }

        case HCI_APTO_EXPIRED_EVENT_CODE:
        {
            event = BLEAPPUTIL_HCI_APTO_EXPIRED_EVENT_CODE;
            break;
        }

        default:
        {
            break;
        }
    }

    // Get the event mask of handlers form BLEAPPUTIL_HCI_DATA_TYPE
    isRequired = BLEAppUtil_isEventEnabled(BLEAPPUTIL_HCI_DATA_TYPE, event);
    // Check if the received event is required by the application
    if(isRequired)
    {
        bleAppUtilEventAndHandle->event = event;
        bleAppUtilEventAndHandle->handlerType = BLEAPPUTIL_HCI_DATA_TYPE;
    }
    return isRequired;
}

/*********************************************************************
 * @fn      BLEAppUtil_convertHCISMPEvents
 *
 * @brief   Process HCI SMP events received from the BLE stack, called
 *          from BLE Stack context.
 *          This function converts the BLE stack events to BLEAppUtil
 *          events (which are bit mask events) and checks if the event
 *          is required by the application level.
 *          If the event is required, fills the bleAppUtilEventAndHandle
 *          structure with the event and handler type.
 *          All the events processed in this function will be received by
 *          handlers form type of BLEAPPUTIL_HCI_SMP_TYPE
 *
 * @param   pMsg - The msg containing the data to send to the application
 * @param   bleAppUtilEventAndHandle - structure of the event in BLE App Util
 *                                     format and handler type
 *
 * @return  True/False
 */
uint8_t BLEAppUtil_convertHCISMPEvents(bleStack_msgHdt_t *pMsg, BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle)
{
    ICall_HciExtEvt * pMsgData = (ICall_HciExtEvt *)pMsg;
    uint8_t isRequired = false;
    uint32_t event = 0;

    switch(pMsgData->hdr.status)
    {
        case HCI_COMMAND_COMPLETE_EVENT_CODE:
        {
            event = BLEAPPUTIL_HCI_COMMAND_COMPLETE_EVENT_CODE;
            break;
        }

        case HCI_LE_EVENT_CODE:
        {
            event = BLEAPPUTIL_HCI_LE_EVENT_CODE;
            break;
        }

        default:
        {
            break;
        }
    }

    // Get the event mask of handlers form BLEAPPUTIL_HCI_SMP_TYPE
    isRequired = BLEAppUtil_isEventEnabled(BLEAPPUTIL_HCI_SMP_TYPE, event);
    // Check if the received event is required by the application
    if(isRequired)
    {
        bleAppUtilEventAndHandle->event = event;
        bleAppUtilEventAndHandle->handlerType = BLEAPPUTIL_HCI_SMP_TYPE;
    }
    return isRequired;
}

/*********************************************************************
 * @fn      BLEAppUtil_convertHCISMPMetaEvents
 *
 * @brief   Process HCI SMP Meta events received from the BLE stack, called
 *          from BLE Stack context.
 *          This function converts the BLE stack events to BLEAppUtil
 *          events (which are bit mask events) and checks if the event
 *          is required by the application level.
 *          If the event is required, fills the bleAppUtilEventAndHandle
 *          structure with the event and handler type.
 *          All the events processed in this function will be received by
 *          handlers form type of BLEAPPUTIL_HCI_SMP_META_TYPE
 *
 * @param   pMsg - The msg containing the data to send to the application
 * @param   bleAppUtilEventAndHandle - structure of the event in BLE App Util
 *                                     format and handler type
 *
 * @return  True/False
 */
uint8_t BLEAppUtil_convertHCICTRLToHostEvents(BLEAppUtil_msgHdr_t *pMsg, BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle)
{
    hciPacket_t *pBuf = (hciPacket_t *)pMsg;
    uint8_t isRequired = false;
    uint32_t event = 0;

    switch(pBuf->pData[0])
    {
        case HCI_EVENT_PACKET:
        {
            event = BLEAPPUTIL_HCI_EVENT_PACKET;
            break;
        }

        case HCI_ACL_DATA_PACKET:
        {
           event = BLEAPPUTIL_HCI_ACL_DATA_PACKET;
           break;
        }

        default:
        {
            break;
        }
    }

    // Get the event mask of handlers form BLEAPPUTIL_HCI_CTRL_TO_HOST_TYPE
    isRequired = BLEAppUtil_isEventEnabled(BLEAPPUTIL_HCI_CTRL_TO_HOST_TYPE, event);
    // Check if the received event is required by the application
    if(isRequired)
    {
        bleAppUtilEventAndHandle->event = event;
        bleAppUtilEventAndHandle->handlerType = BLEAPPUTIL_HCI_CTRL_TO_HOST_TYPE;
    }
    return isRequired;
}
/*********************************************************************
 * @fn      BLEAppUtil_convertHCISMPMetaEvents
 *
 * @brief   Process HCI SMP Meta events received from the BLE stack, called
 *          from BLE Stack context.
 *          This function converts the BLE stack events to BLEAppUtil
 *          events (which are bit mask events) and checks if the event
 *          is required by the application level.
 *          If the event is required, fills the bleAppUtilEventAndHandle
 *          structure with the event and handler type.
 *          All the events processed in this function will be received by
 *          handlers form type of BLEAPPUTIL_HCI_SMP_META_TYPE
 *
 * @param   pMsg - The msg containing the data to send to the application
 * @param   bleAppUtilEventAndHandle - structure of the event in BLE App Util
 *                                     format and handler type
 *
 * @return  True/False
 */
uint8_t BLEAppUtil_convertHCISMPMetaEvents(bleStack_msgHdt_t *pMsg, BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle)
{
    ICall_HciExtEvt * pMsgData = (ICall_HciExtEvt *)pMsg;
    uint8_t isRequired = false;
    uint32_t event = 0;

    switch(pMsgData->hdr.status)
    {
        case HCI_LE_EVENT_CODE:
        {
            event = BLEAPPUTIL_HCI_LE_EVENT_CODE;
            break;
        }

        default:
        {
            break;
        }
    }

    // Get the event mask of handlers form BLEAPPUTIL_HCI_SMP_META_TYPE
    isRequired = BLEAppUtil_isEventEnabled(BLEAPPUTIL_HCI_SMP_META_TYPE, event);
    // Check if the received event is required by the application
    if(isRequired)
    {
        bleAppUtilEventAndHandle->event = event;
        bleAppUtilEventAndHandle->handlerType = BLEAPPUTIL_HCI_SMP_META_TYPE;
    }
    return isRequired;
}

/*********************************************************************
 * @fn      BLEAppUtil_convertL2CAPDataMsg
 *
 * @brief   Process L2CAP Data received from the BLE stack, called
 *          from BLE Stack context.
 *          This function calls @ref BLEAppUtil_callEventHandler.
 *          The handler is from the type of BLEAPPUTIL_L2CAP_DATA_TYPE
 *
 * @param   pMsg - The msg containing the data to send to the application
 * @param   bleAppUtilEventAndHandle - structure of the event in BLE App Util
 *                                     format and handler type
 *
 * @return  True/False
 */
uint8_t BLEAppUtil_convertL2CAPDataMsg(BLEAppUtil_msgHdr_t *pMsg, BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle)
{
    uint8_t isRequired = false;

    // Check if there is a registered handler from the BLEAPPUTIL_L2CAP_DATA_TYPE type
    isRequired = BLEAppUtil_isEventEnabled(BLEAPPUTIL_L2CAP_DATA_TYPE, 0);

    if(isRequired)
    {
        bleAppUtilEventAndHandle->handlerType = BLEAPPUTIL_L2CAP_DATA_TYPE;
    }

    return isRequired;
}

/*********************************************************************
 * @fn      BLEAppUtil_convertL2CAPSignalEvents
 *
 * @brief   Process L2CAP Signal events received from the BLE stack, called
 *          from BLE Stack context.
 *          This function converts the BLE stack events to BLEAppUtil
 *          events (which are bit mask events) and checks if the event
 *          is required by the application level.
 *          If the event is required, fills the bleAppUtilEventAndHandle
 *          structure with the event and handler type.
 *          All the events processed in this function will be received by
 *          handlers form type of BLEAPPUTIL_L2CAP_SIGNAL_TYPE
 *
 * @param   pMsg - The msg containing the data to send to the application
 * @param   bleAppUtilEventAndHandle - structure of the event in BLE App Util
 *                                     format and handler type
 *
 * @return  True/False
 */
uint8_t BLEAppUtil_convertL2CAPSignalEvents(BLEAppUtil_msgHdr_t *pMsg, BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle)
{
    l2capSignalEvent_t *pMsgData = (l2capSignalEvent_t *)pMsg;
    uint8_t isRequired = false;

    // Get the "BLE App Util" event format
    uint32_t event = l2capSignalEventsLookupTable[pMsgData->opcode - BLEAPPUTIL_L2CAP_SIGNAL_EVENTS_OFFSET];
    // Get the event mask of handlers form BLEAPPUTIL_L2CAP_SIGNAL_TYPE
    isRequired = BLEAppUtil_isEventEnabled(BLEAPPUTIL_L2CAP_SIGNAL_TYPE, event);
    // Check if the received event is required by the application
    if(isRequired)
    {
        bleAppUtilEventAndHandle->event = event;
        bleAppUtilEventAndHandle->handlerType = BLEAPPUTIL_L2CAP_SIGNAL_TYPE;
    }
    return isRequired;
}

/*********************************************************************
 * @fn      BLEAppUtil_processAdvEventMsg
 *
 * @brief   Process Adv events received from the BLE stack, called
 *          from BLEAppUtil task context.
 *          This function converts the BLE stack events to BLEAppUtil
 *          events (which are bit mask events) and calls @ref
 *          BLEAppUtil_callEventHandler.
 *          All the events processed in this function will be received by
 *          handlers form type of BLEAPPUTIL_GAP_ADV_TYPE
 *
 * @param   pMsg - the message the process
 *
 * @return  None
 */
void BLEAppUtil_processAdvEventMsg(BLEAppUtil_msgHdr_t *pMsg)
{
    // Pass the event and msg to BLEAppUtil_callEventHandler which calls the
    // handler of the application
    BLEAppUtil_callEventHandler(((BLEAppUtil_AdvEventData_t *)pMsg)->event,
                                pMsg, BLEAPPUTIL_GAP_ADV_TYPE);

    // Free the data that was received from the stack CB
    if (((BLEAppUtil_AdvEventData_t *)pMsg)->event != BLEAPPUTIL_ADV_INSUFFICIENT_MEMORY &&
        ((BLEAppUtil_AdvEventData_t *)pMsg)->pBuf)
    {
        BLEAppUtil_free(((BLEAppUtil_AdvEventData_t *)pMsg)->pBuf);
    }
}

/*********************************************************************
 * @fn      BLEAppUtil_processScanEventMsg
 *
 * @brief   Process Scan events received from the BLE stack, called
 *          from BLEAppUtil task context.
 *          This function converts the BLE stack events to BLEAppUtil
 *          events (which are bit mask events) and calls @ref
 *          BLEAppUtil_callEventHandler.
 *          All the events processed in this function will be received by
 *          handlers form type of BLEAPPUTIL_GAP_SCAN_TYPE
 *
 * @param   pMsg - the message the process
 *
 * @return  None
 */
void BLEAppUtil_processScanEventMsg(BLEAppUtil_msgHdr_t *pMsg)
{
    // Pass the event and msg to BLEAppUtil_callEventHandler which calls the
    // handler of the application
    BLEAppUtil_callEventHandler(((BLEAppUtil_ScanEventData_t *)pMsg)->event,
                                pMsg, BLEAPPUTIL_GAP_SCAN_TYPE);

    // Free the data that was received from the stack CB
    if (((BLEAppUtil_ScanEventData_t *)pMsg)->event == BLEAPPUTIL_ADV_REPORT &&
        ((BLEAppUtil_ScanEventData_t *)pMsg)->pBuf->pAdvReport.pData)
    {
        BLEAppUtil_free(((BLEAppUtil_ScanEventData_t *)pMsg)->pBuf->pAdvReport.pData);
    }
    if (((BLEAppUtil_ScanEventData_t *)pMsg)->event != BLEAPPUTIL_SCAN_INSUFFICIENT_MEMORY &&
        ((BLEAppUtil_ScanEventData_t *)pMsg)->pBuf)
    {
        BLEAppUtil_free(((BLEAppUtil_ScanEventData_t *)pMsg)->pBuf);
    }
}

/*********************************************************************
 * @fn      BLEAppUtil_processPairStateMsg
 *
 * @brief   Process PairState events received from the BLE stack, called
 *          from BLEAppUtil task context.
 *          This function converts the BLE stack events to BLEAppUtil
 *          events (which are bit mask events) and calls @ref
 *          BLEAppUtil_callEventHandler.
 *          All the events processed in this function will be received by
 *          handlers form type of BLEAPPUTIL_PAIR_STATE_TYPE
 *
 * @param   pMsg - the message the process
 *
 * @return  None
 */
void BLEAppUtil_processPairStateMsg(BLEAppUtil_msgHdr_t *pMsgData)
{
    // Pass the event and msg to BLEAppUtil_callEventHandler which calls the
    // handler of the application
    BLEAppUtil_callEventHandler(((BLEAppUtil_PairStateData_t *)pMsgData)->state,
                                pMsgData, BLEAPPUTIL_PAIR_STATE_TYPE);
}

/*********************************************************************
 * @fn      BLEAppUtil_processPasscodeMsg
 *
 * @brief   Process PassCode received from the BLE stack, called
 *          from BLEAppUtil task context.
 *          This function calls @ref BLEAppUtil_callEventHandler.
 *          The handler is from the type of BLEAPPUTIL_PASSCODE_TYPE
 *
 * @param   pMsg - the message the process
 *
 * @return  None
 */
void BLEAppUtil_processPasscodeMsg(BLEAppUtil_msgHdr_t *pMsgData)
{
    // Pass the msg to BLEAppUtil_callEventHandler which calls the
    // handler of the application
    // Note: The event is not needed since the passcode contains only data
    BLEAppUtil_callEventHandler(0, pMsgData, BLEAPPUTIL_PASSCODE_TYPE);
}

/*********************************************************************
 * @fn      BLEAppUtil_processConnEventMsg
 *
 * @brief   Process Connection events received from the BLE stack, called
 *          from BLEAppUtil task context.
 *          This function calls @ref BLEAppUtil_callEventHandler.
 *          The handler is from the type of BLEAPPUTIL_CONN_NOTI_TYPE
 *
 * @param   pMsg - the message the process
 *
 * @return  None
 */
void BLEAppUtil_processConnEventMsg(BLEAppUtil_connEventNoti_t *pMsg)
{
    // Pass the event and msg to BLEAppUtil_callEventHandler which calls the
    // handler of the application
    BLEAppUtil_callEventHandler(pMsg->event ,(BLEAppUtil_msgHdr_t *)pMsg->connEventReport, BLEAPPUTIL_CONN_NOTI_TYPE);
}

/*********************************************************************
 * @fn      BLEAppUtil_processHandoverEventMsg
 *
 * @brief   Process Connection Handover events received from the BLE
 *          stack, called from BLEAppUtil task context.
 *          This function calls @ref BLEAppUtil_callEventHandler.
 *          The handler is from the type of BLEAPPUTIL_HANDOVER_TYPE
 *
 * @param   event - event code. @ref BLEAppUtil_HandoverEventMaskFlags_e
 * @param   pMsg - the message the process
 *
 * @return  None
 */
void BLEAppUtil_processHandoverEventMsg(uint32_t event, BLEAppUtil_msgHdr_t *pMsgData)
{
    // Pass the event and msg to BLEAppUtil_callEventHandler which calls the
    // handler of the application
    BLEAppUtil_callEventHandler(event, pMsgData, BLEAPPUTIL_HANDOVER_TYPE);
}

/*********************************************************************
 * @fn      BLEAppUtil_isConnEventRequired
 *
 * @brief   Process Connection events received from the BLE stack, called
 *          from BLE stack context.
 *          This function converts the BLE stack events to BLEAppUtil
 *          events (which are bit mask events) and calls @ref
 *          BLEAppUtil_callEventHandler.
 *          All the events processed in this function will be received by
 *          handlers form type of BLEAPPUTIL_CONN_NOTI_TYPE
 *
 * @param   pMsg - the message the process
 *
 * @return  None
 */
uint8_t BLEAppUtil_isConnEventRequired(Gap_ConnEventRpt_t *pMsg,
                                       uint32_t *event)
{
    Gap_ConnEventRpt_t * pMsgData = (Gap_ConnEventRpt_t *)pMsg;

    switch(pMsgData->eventType)
    {
        case GAP_CB_CONN_ESTABLISHED:
        {
            *event = BLEAPPUTIL_CONN_NOTI_CONN_ESTABLISHED;
            break;
        }

        case GAP_CB_PHY_UPDATE:
        {
            *event = BLEAPPUTIL_CONN_NOTI_PHY_UPDATE;
            break;
        }

        case GAP_CB_CONN_EVENT_ALL:
        {
            *event = BLEAPPUTIL_CONN_NOTI_CONN_EVENT_ALL;
            break;
        }

        default:
        {
            break;
        }
    }
    return BLEAppUtil_isEventEnabled(BLEAPPUTIL_CONN_NOTI_TYPE, *event);
}

/*********************************************************************
 * @fn      BLEAppUtil_isPairStateEventRequired
 *
 * @brief   Checks if the event is required by the application
 *          level.
 *          If it is required return true, else return false.
 *
 * @param   event - The event received
 *
 * @return  True/False
 */
uint8_t BLEAppUtil_isPairStateEventRequired(uint8_t event,
                                            uint32_t *bleAppUtilEvent)
{
    uint8_t isRequired = false;

    // If the event is in valid
    if(event <= BLEAPPUTIL_PAIR_STATE_TABLE_SIZE)
    {
        // Convert the event
        *bleAppUtilEvent = pairStateLookupTable[event];
        // Check if the event is required by the application
        isRequired = BLEAppUtil_isEventEnabled(BLEAPPUTIL_PAIR_STATE_TYPE, *bleAppUtilEvent);
    }

    return isRequired;
}

/*********************************************************************
 * @fn      BLEAppUtil_callEventHandler
 *
 * @brief   Send the received msg to the handlers from the received
 *          type if it's eventMask contains the received event or
 *          if are from the type receiving data only:
 *          BLEAPPUTIL_PASSCODE_TYPE and BLEAPPUTIL_L2CAP_DATA_TYPE
 *
 * @param   event   - The event the application handler will receive
 * @param   pMsg    - The msg the application handler will receive
 * @param   type    - The handler type to call
 *
 * @return  None
 */
void BLEAppUtil_callEventHandler(uint32_t event, BLEAppUtil_msgHdr_t *pMsg, BLEAppUtil_eventHandlerType_e type)
{
    BLEAppUtil_EventHandlersList_t *handler = NULL;

    // Lock the Mutex
    pthread_mutex_lock(&mutex);

    // Get the handler if exist
    handler = BLEAppUtil_getEventHandler(handler, type);
    // Iterate over the handlers list
    while(handler != NULL)
    {
        // If the handler is from PASSCODE or L2CAP_DATA types or
        // (for all other types) the event is part of the event mask,
        // call the handler
        if((type == BLEAPPUTIL_PASSCODE_TYPE) ||
           (type == BLEAPPUTIL_L2CAP_DATA_TYPE) ||
           (handler->eventHandler->eventMask & event))
        {
            handler->eventHandler->pEventHandler(event, pMsg);
        }

        // Get the next handler if exist
        handler = BLEAppUtil_getEventHandler(handler, type);
    }

    // Unlock the Mutex - handlers were called
    pthread_mutex_unlock(&mutex);
}

/*********************************************************************
 * @fn      BLEAppUtil_isEventEnabled
 *
 * @brief   Returns the event mask of all registered event handlers from
 *          a specific event handler type.
 *
 * @param   eventHandlerType - Handler type to get the event masks for
 * @param   event - The event to verify that is required
 *
 * @return  The bieMask of all relevant event handlers
 */
uint8_t BLEAppUtil_isEventEnabled(BLEAppUtil_eventHandlerType_e eventHandlerType,
                                  uint32_t event)
{
    uint8_t isRequired = false;
    BLEAppUtil_EventHandlersList_t *handler = NULL;

    // Get the handler if exist
    handler = BLEAppUtil_getEventHandler(handler, eventHandlerType);
    while(handler != NULL)
    {
        // Check if it's event mask contains the event
        if((eventHandlerType == BLEAPPUTIL_PASSCODE_TYPE) ||
           (eventHandlerType == BLEAPPUTIL_L2CAP_DATA_TYPE) ||
           (handler->eventHandler->eventMask & event))
        {
            isRequired = true;
            break;
        }

        // Get the next handler if exist
        handler = BLEAppUtil_getEventHandler(handler, eventHandlerType);
    }

    return isRequired;
}

/*********************************************************************
 * @fn      BLEAppUtil_getEventHandler
 *
 * @brief   Search for an event handler from a given type starting
 *          from the given handler pointer of from the head of the
 *          handlers list if NULL is received.
 *
 * @param   handler - The handler to start the search from (start from head if NULL)
 * @param   eventHandlerType - Handler type to get the event masks for
 *
 * @return  The bieMask of all relevant event handlers
 */
BLEAppUtil_EventHandlersList_t *BLEAppUtil_getEventHandler(BLEAppUtil_EventHandlersList_t *handler,
                                                           BLEAppUtil_eventHandlerType_e eventHandlerType)
{
    BLEAppUtil_EventHandlersList_t *iter = NULL;

    // If the header value is NULL start the search from the head of the list
    if(handler == NULL)
    {
        iter = BLEAppUtilEventHandlersHead;
    }
    // Start the search from the next handler
    else
    {
        iter = (BLEAppUtil_EventHandlersList_t *)handler->next;
    }

    // Iterate over the handlers list
    while(iter != NULL)
    {
        // If this is an handler from the required type
        if(iter->eventHandler->handlerType == eventHandlerType)
        {
            // Break from the loop to return the current item in the list
            break;
        }

        // Next item in the list
        iter = (BLEAppUtil_EventHandlersList_t *)iter->next;
    }

    return iter;
}
