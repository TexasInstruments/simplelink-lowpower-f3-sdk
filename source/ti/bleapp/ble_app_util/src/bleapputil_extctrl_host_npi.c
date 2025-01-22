/******************************************************************************

 @file  bleapputil_extctrl_host_npi.c

 @brief This file contains the uNPI specific implementation of a ExtCtrl Host

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

#include <string.h>
#include <stdlib.h>

#include "bcomdef.h"

#include <ti_drivers_config.h>

#include "npi_data.h"
#include "npi_task.h"
#include "npi_util.h"
#include <ti/bleapp/ble_app_util/inc/bleapputil_extctrl_host.h>

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

// NPI Task configuration
#define NPI_MSG_BUFF_SIZE         500
#define NPI_TASK_STACK_SIZE       700

// NPI Message types
#define NPI_ASYNC_REQ             ((NPI_MSG_TYPE_ASYNC << 5) ^ RPC_SYS_EXTCTRL)
#define NPI_ASYNC_RSP             ((NPI_MSG_TYPE_ASYNC << 5) ^ RPC_SYS_EXTCTRL)
#define NPI_SYNC_REQ              ((NPI_MSG_TYPE_SYNCREQ << 5) ^ RPC_SYS_EXTCTRL)
#define NPI_SYNC_RSP              ((NPI_MSG_TYPE_SYNCRSP << 5) ^ RPC_SYS_EXTCTRL)

#define EXTCTRL_HOST_EVT_ASSERT                          0x80         //!< EXTCTRL async event
#define EXTCTRL_HOST_ASSERT_CAUSE_NULL_POINTER_EXCEPT    0x03         //!< EXTCTRL Assert Error code
/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// uNPI Parameters for opening serial port to ExtCtrl module
NPI_Params npiPortParams;

// EXTCTRL message processing function
pfnExtCtrlProcessMsgCb gExtCtrlProcessMsgCb = NULL;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

void ExtCtrlHost_processNpiMessage(_npiFrame_t *pNpiMsg);
uint8_t ExtCtrlHost_createAndSendNpiMessage(uint8_t cmdId, uint8_t cmdTypeNpi, uint8_t *pData, uint16_t dataLen);

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
/*********************************************************************
 * @fn      ExtCtrlHost_openHostIf
 *
 * @brief   Open NPI interfaces
 *
 * @param   none
 *
 * @return  Status of NPITask_open
 */
uint8_t ExtCtrlHost_openHostIf(pfnExtCtrlProcessMsgCb ExtCtrlAppCb)
{
  bStatus_t status = NPI_SUCCESS;
#ifdef APP_EXTERNAL_CONTROL
  // Initialize NPI Interface
  NPITask_Params_init(NPI_SERIAL_TYPE_UART, &npiPortParams);

  npiPortParams.stackSize = NPI_TASK_STACK_SIZE;
  npiPortParams.mrdyGpioIndex = 0 /*MRDY_GPIO*/;
  npiPortParams.srdyGpioIndex = 0 /*SRDY_GPIO*/;
  npiPortParams.bufSize   = NPI_MSG_BUFF_SIZE;
  npiPortParams.portParams.uartParams.baudRate = 460800;

  // Kick off NPI task
  status = NPITask_open(&npiPortParams);

  if(status == NPI_SUCCESS)
  {
    // Register callback and subsystem with NPI task
    NPITask_regSSFromHostCB(RPC_SYS_EXTCTRL ,ExtCtrlHost_processNpiMessage);

    // Change NPI assert subsystemId and command
    NPITask_chgAssertHdr(RPC_SYS_EXTCTRL + (NPI_MSG_TYPE_ASYNC<<5), EXTCTRL_HOST_EVT_ASSERT);

    gExtCtrlProcessMsgCb = (pfnExtCtrlProcessMsgCb)ExtCtrlAppCb;

    if(gExtCtrlProcessMsgCb == NULL)
    {
      AssertHandler(EXTCTRL_HOST_ASSERT_CAUSE_NULL_POINTER_EXCEPT, 0);
    }
  }
#endif // APP_EXTERNAL_CONTROL
  return status;
}

/*********************************************************************
 * @fn      ExtCtrlHost_createAndSendNpiMessage
 *
 * @brief   Process an incoming uNPI Message to ExtCtrl
 *          This is a uNPI abstraction of the real processing function
 *
 * @param   cmdId      - the command Id
 * @param   cmdTypeNpi - sync/async cmd
 * @param   pData      - pointer to the data that need to be sent.
 * @param   dataLen    - the length of the data.
 *
 * @return  status - 0 = success, 1 = failed
 *
 */
uint8_t ExtCtrlHost_createAndSendNpiMessage(uint8_t cmdId, uint8_t cmdTypeNpi, uint8_t *pData, uint16_t dataLen)
{
  _npiFrame_t npiMsg;

  // Build and send the NPI message
  npiMsg.dataLen = dataLen;
  npiMsg.cmd0 = cmdTypeNpi;
  npiMsg.cmd1 = cmdId;

  // If we have any data to send
  if ((pData != NULL) && (0 != dataLen))
  {
    npiMsg.pData = NPIUtil_malloc(dataLen);
    if (NULL != npiMsg.pData)
    {
      memcpy(npiMsg.pData, pData, dataLen);
    }
  }
  else
  {
    npiMsg.pData = NULL;
  }

  // Forward npiFrame to uNPI
  if (NPITask_sendToHost(&npiMsg) != NPI_SUCCESS)
  {
    NPITask_freeFrameData(&npiMsg);
    return FAILURE;
  }
  return SUCCESS;
}

/*********************************************************************
 * @fn      ExtCtrlHost_sendMsg
 *
 * @brief   Build and send a uNPI command
 *
 * @param   cmdId - Command Id requested by Host
 * @param   cmdType - Sync/Async commands
 * @param   dataLen - Length of pData buffer
 * @param   pData - Pointer to a data buffer
 *
 * @return  status - 0 = success, 1 = failed
 */
uint8_t ExtCtrlHost_sendMsg(uint8_t cmdId, uint8_t cmdType, uint8_t *pData, uint16_t dataLen)
{
#ifdef APP_EXTERNAL_CONTROL
  uint8_t cmdTypeNpi;
  // First, translate Host message to NPI message
  switch (cmdType)
  {
    case HOST_ASYNC_RSP:
    {
      cmdTypeNpi = NPI_ASYNC_RSP;
    }
    break;

    case HOST_SYNC_RSP:
    {
      cmdTypeNpi = NPI_SYNC_RSP;
    }
    break;

    default:
      return FAILURE;
  }

  return ExtCtrlHost_createAndSendNpiMessage( cmdId, cmdTypeNpi, pData, dataLen);
#else // APP_EXTERNAL_CONTROL
  return SUCCESS;
#endif // APP_EXTERNAL_CONTROL
}

/*********************************************************************
 * @fn      ExtCtrlHost_processNpiMessage
 *
 * @brief   Process an incoming uNPI Message to ExtCtrl
 *          This is a uNPI abstraction of the real processing function
 *
 * @param   pMsg - message to process
 *
 * @return  None
 */
void ExtCtrlHost_processNpiMessage(_npiFrame_t *pNpiMsg)
{
#ifdef APP_EXTERNAL_CONTROL
  DispatcherHostMsg_t *pHostMsg;

  if (pNpiMsg == NULL)
  {
    return;
  }

  pHostMsg = (DispatcherHostMsg_t *)NPIUtil_malloc(sizeof(DispatcherHostMsg_t));

  // If we could not allocate space for the message, drop it
  if (!pHostMsg)
  {
    NPITask_freeFrameData(pNpiMsg);
    return;
  }

  pHostMsg->cmdId = pNpiMsg->cmd1;
  pHostMsg->dataLen = pNpiMsg->dataLen;

  if ((pNpiMsg->dataLen != 0) && (NULL != pNpiMsg->pData))
  {
    pHostMsg->pData = (uint8_t *)NPIUtil_malloc(pNpiMsg->dataLen);

    // Check that we could allocate payload (only if the message is not empty)
    if (NULL == pHostMsg->pData)
    {
      NPITask_freeFrameData(pNpiMsg);

      NPIUtil_free((uint8_t *)pHostMsg);
      return;
    }

    memcpy(pHostMsg->pData, pNpiMsg->pData, pNpiMsg->dataLen);
  }

  // NPI to Host
  switch (pNpiMsg->cmd0)
  {
    case NPI_SYNC_REQ:
    {
      pHostMsg->cmdType = HOST_SYNC_REQ;
    }
    break;

    case NPI_ASYNC_REQ:
    {
      pHostMsg->cmdType = HOST_ASYNC_REQ;
    }
    break;
  }

  NPITask_freeFrameData(pNpiMsg);

  gExtCtrlProcessMsgCb(pHostMsg);
#endif // APP_EXTERNAL_CONTROL
}
