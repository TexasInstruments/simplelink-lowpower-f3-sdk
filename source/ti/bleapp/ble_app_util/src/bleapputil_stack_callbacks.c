/******************************************************************************

@file  BLEAppUtil_stack_callbacks.c

@brief This file contains the BLEAppUtil module stack callback functions

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
#include <string.h>
#include <stdarg.h>
#include <ti/bleapp/ble_app_util/inc/bleapputil_api.h>
#include <ti/bleapp/ble_app_util/inc/bleapputil_internal.h>

/*********************************************************************
 * MACROS
 */

/*********************************************************************
* CONSTANTS
*/

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* GLOBAL VARIABLES
*/

/*********************************************************************
* LOCAL VARIABLES
*/

/*********************************************************************
* CALLBACKS
*/

/*********************************************************************
 * @fn      BLEAppUtil_processStackMsgCB
 *
 * @brief   BLE stack callback
 *
 * @param   event    - todo - remove the event from BLEAppUtil_processStackMsgCB
 * @param   pMessage - message from ble stack
 *
 * @return  TRUE/FALSE  - whether if it is safe to dealloc the message.
 */
uint8_t BLEAppUtil_processStackMsgCB(uint8_t event, uint8_t *pMessage)
{
    uint8_t safeToDealloc = false;
    BLEAppUtil_eventAndHandlerType_t bleAppUtilEventAndHandle = {0};
    BLEAppUtil_stackMsgData_t *pData = NULL;

    // Call the process function and return the "BLEAppUtil" event and handler type
    // If their value is 0, this is an event that should be parsed first in the
    // task function (there should be a few events with this behavior
    if(BLEAppUtil_isStackEventRequired((BLEAppUtil_msgHdr_t *)pMessage, &bleAppUtilEventAndHandle) == true)
    {
        pData = BLEAppUtil_malloc(sizeof(BLEAppUtil_stackMsgData_t));

        if (pData != NULL)
        {
            // Fill the data
            pData->pMessage = (BLEAppUtil_msgHdr_t *)pMessage;
            pData->eventAndHandlerType = bleAppUtilEventAndHandle;

            // Enqueue the msg in order to be executed in the application context
            if (BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_STACK_CALLBACK, pData) != SUCCESS)
            {
                // Indicate that it is safe to dealloc since the enqueue failed
                safeToDealloc = true;
                // Free the data
                BLEAppUtil_free(pData);
            }
        }
    }
    else
    {
        // Indicate that it is safe to dealloc since the event is not
        // required by the application
        safeToDealloc = true;
    }

  return safeToDealloc;
}

/*********************************************************************
 * @fn      BLEAppUtil_pairStateCB
 *
 * @brief   Pairing state callback
 *
 * @param   connHandle - connection handle of the peer
 * @param   state      - state of the pairing process
 * @param   status     - status of the pairing process
 *
 * @return  None
 */
void BLEAppUtil_pairStateCB(uint16_t connHandle, uint8_t state, uint8_t status)
{
    BLEAppUtil_PairStateData_t *pData = NULL;
    uint32_t bleAppUtilEvent = 0;

    if(BLEAppUtil_isPairStateEventRequired(state, &bleAppUtilEvent) == true)
    {
        // Allocate the data if the the application is registered to it
        pData = BLEAppUtil_malloc(sizeof(BLEAppUtil_PairStateData_t));

        // Allocate space for the event data
        if (pData != NULL)
        {
            pData->connHandle = connHandle;
            pData->state = bleAppUtilEvent;
            pData->status = status;

            // Queue the event
            if (BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_PAIRING_STATE_CB, pData) != SUCCESS)
            {
                BLEAppUtil_free(pData);
            }
        }
    }
}

/*********************************************************************
 * @fn      BLEAppUtil_passcodeCB
 *
 * @brief   PassCode callback
 *
 * @param   *pDeviceAddr  - pointer to the peer address
 * @param   connHandle    - connection handle of the pairing peer
 * @param   uiInputs      - TRUE if the local device should accept a
 *                          passcode input
 * @param   uiOutputs     - TRUE if the local device should display
 *                          the passcode
 * @param   numComparison - Is zero until the passcode pairing is
 *                          complete. After that, it is the code that
 *                          should be displayed for numeric comparison pairing
 *
 * @return  None
 */
void BLEAppUtil_passcodeCB(uint8_t *pDeviceAddr,
                           uint16_t connHandle,
                           uint8_t uiInputs,
                           uint8_t uiOutputs,
                           uint32_t numComparison)
{
    BLEAppUtil_PasscodeData_t *pData = NULL;

    if(BLEAppUtil_isEventEnabled(BLEAPPUTIL_PASSCODE_TYPE, 0))
    {
        // Allocate space for the passcode event
        pData = BLEAppUtil_malloc(sizeof(BLEAppUtil_PasscodeData_t));
        if (pData != NULL)
        {
            pData->connHandle = connHandle;
            memcpy(pData->deviceAddr, pDeviceAddr, B_ADDR_LEN);
            pData->uiInputs = uiInputs;
            pData->uiOutputs = uiOutputs;
            pData->numComparison = numComparison;

            // Enqueue the event.
            if(BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_PASSCODE_NEEDED_CB, pData) != SUCCESS)
            {
                BLEAppUtil_free(pData);
            }
        }
    }
}

/*********************************************************************
 * @fn      BLEAppUtil_connEventCB
 *
 * @brief   Connection event callback
 *
 * @param   *pReport - pointer to connection event report
 *
 * @return  None
 */
void BLEAppUtil_connEventCB(Gap_ConnEventRpt_t *pReport)
{
    BLEAppUtil_connEventNoti_t *pData = NULL;
    uint8_t freeData = true;
    uint32_t event = 0;

    // Check if the event is required by the application
    if(BLEAppUtil_isConnEventRequired(pReport, &event) == true)
    {
        pData = BLEAppUtil_malloc(sizeof(BLEAppUtil_connEventNoti_t));

        if(pData != NULL)
        {
            pData->event = event;
            pData->connEventReport = pReport;

            // Enqueue the event msg
            if(BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_CONN_EVENT_CB, pData) == SUCCESS)
            {
                freeData = false;
            }
            else
            {
                BLEAppUtil_free(pData);
            }
        }
    }
    if(freeData == true)
    {
        BLEAppUtil_free(pReport);
    }
}

/*********************************************************************
 * @fn      BLEAppUtil_scanCB
 *
 * @brief   Scan callback
 *
 * @param   event - The event to handle
 * @param   *pBuf - Data potentially accompanying event
 * @param   *arg  - Custom application argument that can be return through
 *                  this callback
 *
 * @return  None
 */
void BLEAppUtil_scanCB(uint32_t event, GapScan_data_t *pBuf, uint32_t *arg)
{
    uint8_t freeData = true;
    BLEAppUtil_ScanEventData_t *pData = NULL;

    if(BLEAppUtil_isEventEnabled(BLEAPPUTIL_GAP_SCAN_TYPE, event) == true)
    {
        pData = BLEAppUtil_malloc(sizeof(BLEAppUtil_ScanEventData_t));

        if (pData != NULL)
        {
            pData->event = event;
            pData->pBuf = pBuf;
            pData->arg = arg;

            // Enqueue the event
            if(BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_SCAN_CB_EVENT, pData) == SUCCESS)
            {
                freeData = false;
            }
            else
            {
                BLEAppUtil_free(pData);
            }
        }
    }

    // Free the data if something went wrong
    if(freeData == true && pBuf != NULL)
    {
      if (event == BLEAPPUTIL_ADV_REPORT &&
          pBuf->pAdvReport.pData)
      {
        BLEAppUtil_free(pBuf->pAdvReport.pData);
      }
      if (event != BLEAPPUTIL_SCAN_INSUFFICIENT_MEMORY)
      {
        BLEAppUtil_free(pBuf);
      }
    }
}

/*********************************************************************
 * @fn      BLEAppUtil_advCB
 *
 * @brief   Advertise callback
 *
 * @param   event - The event to handle
 * @param   *pBuf - Data potentially accompanying event
 * @param   *arg  - Custom application argument that can be return through
 *                  this callback
 *
 * @return  none
 */
void BLEAppUtil_advCB(uint32_t event, GapAdv_data_t *pBuf, uint32_t *arg)
{
    uint8_t freeData = true;
    BLEAppUtil_AdvEventData_t *pData = NULL;

    if(BLEAppUtil_isEventEnabled(BLEAPPUTIL_GAP_ADV_TYPE, event) == true)
    {
        pData = BLEAppUtil_malloc(sizeof(BLEAppUtil_AdvEventData_t));

        if (pData != NULL)
        {
            pData->event = event;
            pData->pBuf = pBuf;
            pData->arg = arg;

            // Enqueue the event
            if(BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_ADV_CB_EVENT, pData) == SUCCESS)
            {
                freeData = false;
            }
            else
            {
                BLEAppUtil_free(pData);
            }
        }
    }

    // Free the data if something went wrong
    if(freeData == true && pBuf != NULL)
    {
      // Free the data that was received from the stack CB
      if (event != BLEAPPUTIL_ADV_INSUFFICIENT_MEMORY)
      {
        BLEAppUtil_free(pBuf);
      }
    }
}

/*********************************************************************
 * @fn      BLEAppUtil_HandoverSNCB
 *
 * @brief   Replace the Serving node callback to BLEAppUtill Event
 *
 * @param   status - the stack status starting the serving node
 *
 * @return  none
 */
void BLEAppUtil_HandoverSNCB(uint16_t connHandle, uint32_t status)
{
    // Replace the callback to BLEAppUtill Event
    // Queue the event
    uint8_t *pData = BLEAppUtil_malloc(sizeof(uint16_t) + sizeof(uint32_t));

    if(pData != NULL)
    {
        uint8_t *pEvt = pData;
        // Copy the connection handle
        memcpy(pEvt, &connHandle, sizeof(uint16));
        pEvt += sizeof(uint16);

        // Copy the status
        memcpy(pEvt, &status, sizeof(uint32_t));

        BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_HANDOVER_SN_EVENT_CB, pData);
    }
}


/*********************************************************************
 * @fn      BLEAppUtil_HandoverCNCB
 *
 * @brief   Replace the Candidate node callback to BLEAppUtill Event
 *
 * @param   status - the stack status to starting the candidate node
 *
 * @return  none
 */
void BLEAppUtil_HandoverCNCB(uint16_t connHandle, uint32_t status)
{
    // Replace the callback to BLEAppUtill Event
    // Queue the event
    uint8_t *pData = BLEAppUtil_malloc(sizeof(uint16_t) + sizeof(uint32_t));

    if(pData != NULL)
    {
        uint8_t *pEvt = pData;

        // Copy the connection handle
        memcpy(pEvt, &connHandle, sizeof(uint16));
        pEvt += sizeof(uint16);

        // Copy the status
        memcpy(pEvt, &status, sizeof(uint32_t));

        BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_HANDOVER_CN_EVENT_CB, pData);
    }
}
