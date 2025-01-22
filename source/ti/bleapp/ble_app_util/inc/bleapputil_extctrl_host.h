/******************************************************************************

@file  bleapputil_extctrl_host.h

@brief This file processes and send the data from the UART to the Applications
       and vice versa.


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


#ifndef BLEAPPUTIL_EXTCTRL_HOST_H
#define BLEAPPUTIL_EXTCTRL_HOST_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <bcomdef.h>

/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */
#define EXTCTRL_HOST_EVT_ERROR                     0x81           //!< EXTCTRL async event

/*********************************************************************
 * Structures
 */

/// @brief Enumeration for Host Message types
typedef enum
{
  HOST_ASYNC_REQ,
  HOST_ASYNC_RSP,
  HOST_SYNC_REQ,
  HOST_SYNC_RSP
} hostMsgType_e;

// Host message structure
typedef struct
{
  uint16_t            cmdId;       //!< Command Id
  hostMsgType_e       cmdType;     //!< Sync/Async
  uint16_t            dataLen;     //!< Length of data pointed by pData
  uint8_t             *pData;      //!< Pointer to the data
} DispatcherHostMsg_t;

/*********************************************************************
 * TYPEDEFS
 */
typedef void (*pfnExtCtrlProcessMsgCb)(DispatcherHostMsg_t *pMsg);

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      ExtCtrlHost_openHostIf
 *
 * @brief   Open Host interfaces
 *
 * @param   none
 *
 * @return  Status of Host Task_open
 */
uint8_t ExtCtrlHost_openHostIf(pfnExtCtrlProcessMsgCb ExtCtrlAppCb);


/*********************************************************************
 * @fn      ExtCtrlHost_sendMsg
 *
 * @brief   Build and send a Host command
 *
 * @param   cmdId - Command Id requested by Host
 * @param   cmdType - Sync/Async commands
 * @param   dataLen - Length of pData buffer
 * @param   pData - Pointer to a data buffer
 *
 * @return  status - 0 = success, 1 = failed
 */
uint8_t ExtCtrlHost_sendMsg(uint8_t cmdId, uint8_t cmdType, uint8_t *pData, uint16_t dataLen);

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

extern void AssertHandler(uint8_t assertCause, uint8_t assertSubcause);

#ifdef __cplusplus
}
#endif

#endif /* BLEAPPUTIL_EXTCTRL_HOST_H */
