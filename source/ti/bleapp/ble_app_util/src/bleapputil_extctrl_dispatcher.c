/******************************************************************************

@file  bleapputil_extctrl_dispatcher.c

@brief This file contains the BLEAppUtil external control dispatcher module
       function and related functionality, this module receive and send messages
       to the UART, and forward the messages to the
       registered application.

Group: WCS, BTS
Target Device: cc23xx

******************************************************************************

 Copyright (c) 2024, Texas Instruments Incorporated
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
#include <ti/bleapp/ble_app_util/inc/bleapputil_extctrl_dispatcher.h>
#include <ti/bleapp/ble_app_util/inc/bleapputil_extctrl_host.h>
#include "string.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
* CONSTANTS
*/

#define MSG_APP_SPECFIER_OFFSET      0          //!< Host msg app specifier offset
#define MSG_CMD_OP_OFFSET            1          //!< Host msg app cmdOp offset
#define MSG_DATA_LEN_LO              2          //!< Host msg app data len first offset
#define MSG_DATA_LEN_HI              3          //!< Host msg app data len second offset
#define MSG_PAYLOAD                  4          //!< Host msg app payload offset

#define NUM_APPLICATION              9          //!< The number of supported applications
#define EXTCTRL_HOST_CTRL_REV        3
/*********************************************************************
* TYPEDEFS
*/
// Dispatcher Host message structure
typedef struct
{
  uint16_t            dataLen;        // Data length
  uint8_t             appSpecifier;   // The app specifier
  uint8_t             cmdOp;          // Command op code
  hostMsgType_e       cmdType;        // Command type SYNC/ASYNC
  uint8_t             *pData;         // The payload data
} DispatcherAppMsg_t;


/*********************************************************************
* VARIABLES
*/

// Array with the registered application.
static MsgHandler_t gDispatcherAppLookup[NUM_APPLICATION] = {NULL};

// The capabilities that will be send when the module received the EXTCTRL_HOST_CMD_IDENTIFY command
static DispatcherCapabilities_t gExtCtrlCapab =
{
 .devId       = DeviceFamily_ID,
 .revNum      = EXTCTRL_HOST_CTRL_REV,
};

/*********************************************************************
* FUNCTIONS
*/

static void Dispatcher_sendMsgToApp(DispatcherAppMsg_t *pDispatcherAppMsg);
static void Dispatcher_processHostMsg(DispatcherHostMsg_t *pHostMsg);
static bStatus_t Dispatcher_processEvents(uint8_t *pData, uint16_t dataLen);

/*********************************************************************
 * @fn      CmdDispatcher_start
 *
 * @brief   This function is called after stack initialization,
 *          the purpose of this function is to initialize and
 *          open the host interface with the needed callback function.
 *
 * @param   None
 *
 * @return  The Status of starting the External host interface.
 */
bStatus_t Dispatcher_start(void)
{
  bStatus_t status = SUCCESS;

  // Initiate the External Host interface.
  status = ExtCtrlHost_openHostIf(&Dispatcher_processHostMsg); // register to the Host

  return status;
}

/*********************************************************************
 * @fn      Dispatcher_processIdentifyCmd
 *
 * @brief   This function process the @ref EXTCTRL_HOST_CMD_IDENTIFY command
 *          and sends the device Capabilities
 *
 * @return  None
 */
static void Dispatcher_processIdentifyCmd(void)
{
  // Send the capabilities to the host.
  ExtCtrlHost_sendMsg(EXTCTRL_HOST_CMD_IDENTIFY, HOST_SYNC_RSP, (uint8_t *)&gExtCtrlCapab, sizeof(DispatcherCapabilities_t));
}

/*********************************************************************
 * @fn      Dispatcher_processExtCtrlCmd
 *
 * @brief   This function process the @ref EXTCTRL_HOST_CMD command
 *          and sends the device Capabilities
 *
 * @param   pHostMsg - pointer to the host message
 *
 * @return  None
 */
static void Dispatcher_processExtCtrlCmd(DispatcherHostMsg_t *pHostMsg)
{
  DispatcherAppMsg_t dispatcherAppMsg;

  // If the length of the data is not predicted, then send error;
  // otherwise continue with the process.
  if (pHostMsg->dataLen >= DISPATCHER_MSG_HEADER)
  {
    dispatcherAppMsg.cmdType      = pHostMsg->cmdType;
    dispatcherAppMsg.appSpecifier = pHostMsg->pData[MSG_APP_SPECFIER_OFFSET];
    dispatcherAppMsg.cmdOp        = pHostMsg->pData[MSG_CMD_OP_OFFSET];
    dispatcherAppMsg.dataLen      = BUILD_UINT16(pHostMsg->pData[MSG_DATA_LEN_LO],
                                                 pHostMsg->pData[MSG_DATA_LEN_HI]);

    // If there is a payload, and the payload length with the header length are equal the message length,
    // then continue with the process ; otherwise, set an empty payload.
    if (dispatcherAppMsg.dataLen > 0 && (dispatcherAppMsg.dataLen + DISPATCHER_MSG_HEADER == pHostMsg->dataLen))
    {
      dispatcherAppMsg.pData = &(pHostMsg->pData[MSG_PAYLOAD]);
    }
    else
    {
      dispatcherAppMsg.dataLen = 0;
      dispatcherAppMsg.pData   = NULL;
    }

    // Send the message to the specified application.
    Dispatcher_sendMsgToApp(&dispatcherAppMsg);
  }
  else
  {
    // Send EXTCTRL_HOST_ILLEGAL_CMD to the host.
    DispatcherStatus_e status = EXTCTRL_HOST_ILLEGAL_CMD;
    ExtCtrlHost_sendMsg(EXTCTRL_HOST_CMD, HOST_SYNC_RSP, (uint8_t *)&status, sizeof(DispatcherStatus_e));
  }
}
/*********************************************************************
 * @fn      Dispatcher_processHostMsg
 *
 * @brief   This function receive the messages from the Control Host,
 *          processes the message and forward it to the relevant application.
 *
 * @param   pHostMsg - the host message received from the host.
 *
 * @return  None
 */
static void Dispatcher_processHostMsg(DispatcherHostMsg_t *pHostMsg)
{
  // Note that messages that stop in this module should be freed here
  // Messages that are passed to the applications will be copied and send the copy.
  if (pHostMsg->cmdType == HOST_SYNC_REQ || pHostMsg->cmdType == HOST_ASYNC_REQ)
  {
    switch (pHostMsg->cmdId)
    {
      case EXTCTRL_HOST_CMD_IDENTIFY:
      {
        Dispatcher_processIdentifyCmd();
      }
      break;

      case EXTCTRL_HOST_CMD:
      {
        Dispatcher_processExtCtrlCmd(pHostMsg);
        break;
      }

      default:
      {
        // Send EXTCTRL_HOST_ILLEGAL_CMD to the host.
        DispatcherStatus_e status = EXTCTRL_HOST_ILLEGAL_CMD;
        ExtCtrlHost_sendMsg(EXTCTRL_HOST_EVT_ERROR, HOST_ASYNC_RSP, (uint8_t *)&status, sizeof(DispatcherStatus_e));
        break;
      }
    }
  }

  if (pHostMsg->dataLen > 0 && pHostMsg->pData != NULL)
  {
    // We need to free pData if it exists.
    BLEAppUtil_free(pHostMsg->pData);
  }
  // Free the host message
  BLEAppUtil_free(pHostMsg);
}

/*********************************************************************
 * @fn      Dispatcher_sendMsgToApp
 *
 * @brief   This function forwards the message to the relevant application.
 *
 * @param   pDispatcherHostMsg - The message the need to be forwarded to the app.
 *
 * @return  None
 */
static void Dispatcher_sendMsgToApp(DispatcherAppMsg_t *pDispatcherAppMsg)
{
  // Check that the app specifier is in the range, and the application itself is registered.
  if (pDispatcherAppMsg->appSpecifier >= NUM_APPLICATION      ||
      gDispatcherAppLookup[pDispatcherAppMsg->appSpecifier] == NULL)
  {
    // The app specifier is not in the range of the application is yet not registered.
    // Send EXTCTRL_HOST_ILLEGAL_CMD.
    DispatcherStatus_e status = EXTCTRL_HOST_ILLEGAL_CMD;
    ExtCtrlHost_sendMsg(EXTCTRL_HOST_CMD, HOST_ASYNC_RSP, (uint8_t *)&status, sizeof(DispatcherStatus_e));
  }
  else
  {
    // Send the message to the application's message handler.
    gDispatcherAppLookup[pDispatcherAppMsg->appSpecifier]((uint8_t *)pDispatcherAppMsg);
  }
}

/*********************************************************************
 * @fn      Dispatcher_registerMsgHandler
 *
 * @brief   This function called by the applications to register their message handler.
 *          and store the registered application into the global array,
 *          and append their capabilities.
 *
 * @param   appSpecifier - The app specifier to be registered to this module
 * @param   msgHandler  - pointer that contains the message handler and the appSpecifier.
 * @param   appCapab     - the capabilities that the application support.
 *
 * @return  The function that processes the events in this module if success; otherwise
 *          return NULL
 */
ExtCtrlHost_eventHandler_t Dispatcher_registerMsgHandler(uint8_t appSpecifier, MsgHandler_t msgHandler, uint32_t appCapab)
{
  bStatus_t status = FAILURE;

  // Check if the appSpecifier in the range of the configured gDispatcherAppLookup array.
  if (appSpecifier > NUM_APPLICATION)
  {
    status = FAILURE;
  }
  else
  {
    // Set the parameters passed from the application to the gDispatcherAppLookup array.
    gDispatcherAppLookup[appSpecifier]  = msgHandler;

    // Add the capability of the application to the general capability of this module.
    gExtCtrlCapab.capab = gExtCtrlCapab.capab | appCapab;
    status = SUCCESS;
  }

  if (status == SUCCESS)
  {
    return &Dispatcher_processEvents;
  }
  else
  {
    return NULL;
  }
}

/*********************************************************************
 * @fn      Dispatcher_processEvents
 *
 * @brief   This function called by the applications when there is an event,
 *          and this function will forward it to the Host.
 *
 * @param   pData   - the event data.
 * @param   dataLen - the event length.
 *
 * @return  SUCCESS/FAILURE
 */
static bStatus_t Dispatcher_processEvents(uint8_t *pData, uint16_t dataLen)
{
  bStatus_t status = FAILURE;

  if (pData != NULL)
  {
    status = ExtCtrlHost_sendMsg(EXTCTRL_HOST_CMD, HOST_ASYNC_RSP, pData, dataLen);
  }

  return status;
}
