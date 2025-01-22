/******************************************************************************

@file  BLEAppUtil_task.c

@brief This file contains the BLEAppUtil module task function and related
       functionality

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


/*********************************************************************
* CONSTANTS
*/
// Task configuration
#define appTaskStack            NULL
#define BLEAPPUTIL_QUEUE_EVT    0x40000000
#define EVENT_PEND_FOREVER      0xFFFFFFFF

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
* LOCAL FUNCTIONS
*/
void *BLEAppUtil_Task(void *arg);

/*********************************************************************
 * EXTERN FUNCTIONS
*/

/*********************************************************************
* CALLBACKS
*/

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
* LOCAL FUNCTIONS
*/

/*********************************************************************
 * @fn      BLEAppUtil_createBLEAppUtilTask
 *
 * @brief   Create the BLEAppUtil task.
 *
 * @return  SUCCESS, otherwise retVal error.
 */
int BLEAppUtil_createBLEAppUtilTask(void)
{
    int retVal = 0;
    pthread_attr_t param_attribute;
    struct sched_param param;

    retVal =  pthread_attr_init(&param_attribute);
    param.sched_priority = BLEAppUtilLocal_GeneralParams->taskPriority;

    retVal |= pthread_attr_setschedparam(&param_attribute, &param);
    retVal |= pthread_attr_setstack(&param_attribute, appTaskStack, BLEAppUtilLocal_GeneralParams->taskStackSize);
    retVal |= pthread_attr_setdetachstate(&param_attribute, PTHREAD_CREATE_DETACHED);

    retVal |= pthread_create(&BLEAppUtil_theardEntity.threadId,
                             &param_attribute,
                             &BLEAppUtil_Task,
                             NULL);
    return retVal;
}

/*********************************************************************
 * @fn      BLEAppUtil_enqueueMsg
 *
 * @brief   Enqueue the message from the BLE stack to the application queue.
 *
 * @param   event - message event.
 * @param   pData - pointer to the message from BLE stack.
 *
 * @return  SUCCESS   - message was enqueued successfully
 * @return  otherwise - error value is returned
 */
status_t BLEAppUtil_enqueueMsg(uint8_t event, void *pData)
{
    int8_t status = SUCCESS;
    BLEAppUtil_appEvt_t msg;

    // Check if the queue is valid
    if (BLEAppUtil_theardEntity.queueHandle == (mqd_t)-1)
    {
        return(bleNotReady);
    }

    msg.event = event;
    msg.pData = pData;

    // Send the msg to the application queue
    status = mq_send(BLEAppUtil_theardEntity.queueHandle,(char*)&msg,sizeof(msg),1);

    return status;
}

/*********************************************************************
 * @fn      BLEAppUtil_Task
 *
 * @brief   The BLEAppUtil task function.
 *          This function registers to get stack events, call the stack
 *          initializing function and process the events that are
 *          enqueued to it's queue.
 *          Note: The data is freed in this function, the application
 *                needs to copy the data in order to save it in order
 *                to use it outside the event handler of the application.
 *
 * @return  None
 */
void *BLEAppUtil_Task(void *arg)
{
    // Register to the stack and create queue and event
    BLEAppUtil_stackRegister();

    // Init the ble stack
    BLEAppUtil_stackInit();

    // Application main loop
    for (;;)
    {
        BLEAppUtil_appEvt_t pAppEvt;

        // wait until receive queue message
        if (mq_receive(BLEAppUtil_theardEntity.queueHandle, (char*)&pAppEvt, sizeof(pAppEvt), NULL) > 0)
        {
            BLEAppUtil_msgHdr_t *pMsgData = (BLEAppUtil_msgHdr_t *)pAppEvt.pData;
            bool freeMsg = FALSE;

            switch (pAppEvt.event)
            {
              case BLEAPPUTIL_EVT_STACK_CALLBACK:
              {
                  // Cast to the stack MSG pointer format passed to the enqueue function
                  BLEAppUtil_stackMsgData_t *stackMsg = (BLEAppUtil_stackMsgData_t *)pAppEvt.pData;

                  if(stackMsg != NULL)
                  {
                      // Set the flag to true to indicate that BLEAppUtil_freeMsg
                      // should be used to free the msg
                      freeMsg = TRUE;
                      // Point to the msg that was received from the stack,
                      // it will be freed in the end of this function
                      pMsgData = stackMsg->pMessage;

                      // Process the stack event received
                      BLEAppUtil_processStackEvents(stackMsg->pMessage, stackMsg->eventAndHandlerType);

                      // Free the stack msg
                      BLEAppUtil_free(stackMsg);
                  }
                break;
              }
              case BLEAPPUTIL_EVT_ADV_CB_EVENT:
                  BLEAppUtil_processAdvEventMsg(pMsgData);
                  break;

              case BLEAPPUTIL_EVT_SCAN_CB_EVENT:
                  BLEAppUtil_processScanEventMsg(pMsgData);
                  break;

              case BLEAPPUTIL_EVT_PAIRING_STATE_CB:
                  BLEAppUtil_processPairStateMsg(pMsgData);
                  break;

              case BLEAPPUTIL_EVT_PASSCODE_NEEDED_CB:
                  BLEAppUtil_processPasscodeMsg(pMsgData);
                  break;

              case BLEAPPUTIL_EVT_CONN_EVENT_CB:
              {
                  // Cast to a msg from the type allocated in the callback
                  BLEAppUtil_connEventNoti_t *connNotiData = (BLEAppUtil_connEventNoti_t *)pMsgData;

                  // Point to the msg that was received from the stack,
                  // it will be freed in the end of this function
                  pMsgData = (BLEAppUtil_msgHdr_t *)connNotiData->connEventReport;

                  BLEAppUtil_processConnEventMsg(connNotiData);

                  // Free the data allocated in the callback
                  BLEAppUtil_free(connNotiData);
                  break;
              }

              case BLEAPPUTIL_EVT_CALL_IN_BLEAPPUTIL_CONTEXT:
              {
                  ((BLEAppUtil_CallbackToInvoke_t *)pMsgData)->callback(((BLEAppUtil_CallbackToInvoke_t *)pMsgData)->data);

                  // Verify that the data is not NULL before freeing it
                  if(((BLEAppUtil_CallbackToInvoke_t *)pMsgData)->data != NULL)
                  {
                      BLEAppUtil_free(((BLEAppUtil_CallbackToInvoke_t *)pMsgData)->data);
                  }
                  break;
              }

              case BLEAPPUTIL_EVT_HANDOVER_SN_EVENT_CB:
              {
                  BLEAppUtil_processHandoverEventMsg(BLEAPPUTIL_HANDOVER_START_SERVING_EVENT_CODE, pMsgData);
                  break;
              }

              case BLEAPPUTIL_EVT_HANDOVER_CN_EVENT_CB:
              {
                  BLEAppUtil_processHandoverEventMsg(BLEAPPUTIL_HANDOVER_START_CANDIDATE_EVENT_CODE, pMsgData);
                  break;
              }

              default:
                  break;
            }


            // Free the data
            if (pMsgData && freeMsg)
            {
                // Use freeMsg
                BLEAppUtil_freeMsg(pMsgData);
            }
            else if (pMsgData)
            {
                // Use free
                BLEAppUtil_free(pMsgData);
                pMsgData = NULL;
            }
            else
            {
                /* this else clause is required, even if the
                programmer expects this will never be reached
                Fix Misra-C Required: MISRA.IF.NO_ELSE */
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////
// Help functions
/////////////////////////////////////////////////////////////////////////

/*********************************************************************
 * @fn      BLEAppUtil_convertBdAddr2Str
 *
 * @brief   Convert Bluetooth address to string.
 *
 * @param   pAddr - BD address
 *
 * @return  BD address as a string
 */
char *BLEAppUtil_convertBdAddr2Str(uint8_t *pAddr)
{
    uint8_t     charCnt;
    char        hex[] = "0123456789ABCDEF";
    static char str[(2*B_ADDR_LEN)+3];
    char        *pStr = str;

    *pStr++ = '0';
    *pStr++ = 'x';

    // Start from end of addr
    pAddr += B_ADDR_LEN;

    for (charCnt = B_ADDR_LEN; charCnt > 0; charCnt--)
    {
        *pStr++ = hex[*--pAddr >> 4];
        *pStr++ = hex[*pAddr & 0x0F];
    }
  *pStr = 0;

    return str;
}
