/******************************************************************************

 @file  ble_dispatch_lite.c

 @brief ICall BLE Stack Dispatcher for embedded and NP/Serial messages.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2016-2025, Texas Instruments Incorporated
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
#include "ti/ble/stack_util/osal/osal_snv.h"
#include "ti/ble/stack_util/osal/osal_bufmgr.h"

#include "ti/ble/stack_util/icall/stack/ble_dispatch_lite.h"
#include "ti/ble/stack_util/lib_opt/map_direct.h"
//stub headers
#include "ti/ble/stack_util/lib_opt/host_stub_gap_bond_mgr.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#if !defined(STACK_REVISION)
  #define STACK_REVISION                 0x010200
#endif // STACK_REVISION

extern const uint16 ll_buildRevision;

/*********************************************************************
 * TYPEDEFS
 */
#if defined(ICALL_LITE_12_PARAMS)
typedef uint32_t (*directAPIFctPtr_t)(uint32_t param1, uint32_t param2,
                                      uint32_t param3, uint32_t param4,
                                      uint32_t param5, uint32_t param6,
                                      uint32_t param7, uint32_t param8,
                                      uint32_t param9, uint32_t param10,
                                      uint32_t param11, uint32_t param12);
#else
typedef uint32_t (*directAPIFctPtr_t)(uint32_t param1, uint32_t param2,
                                      uint32_t param3, uint32_t param4,
                                      uint32_t param5, uint32_t param6,
                                      uint32_t param7, uint32_t param8);
#endif

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */
uint8_t icall_liteTaskId;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * NETWORK LAYER CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */
static void sendLiteCmdStatus(uint8 taskId);
void icall_liteTranslation(icall_directAPIMsg_t *pMsg);

/*********************************************************************
 * @fn      bleDispatch_BMAlloc
 *
 * @brief   Implementation of the BM allocator functionality.
 *
 *          Note: This function should only be called by the upper
 *                layer protocol/application.
 *
 * @param   type - type of the message to allocate.
 * @param   size - number of bytes to allocate from the heap.
 * @param   connHandle - connection that GATT message is to be sent on
 *                       (applicable only to BM_MSG_GATT type).
 * @param   opcode - opcode of GATT message that buffer to be allocated for
 *                   (applicable only to BM_MSG_GATT type).
 * @param   pSizeAlloc - number of bytes allocated for the caller from the heap
 *                       (applicable only to BM_MSG_GATT type).
 *
 * @return  pointer to the heap allocation; NULL if error or failure.
 */
void *bleDispatch_BMAlloc(uint8_t type, uint16_t size, uint16_t connHandle,
                          uint8_t opcode, uint16_t *pSizeAlloc)
{
  void *pBuf;

  switch (type)
  {
#if defined(HOST_CONFIG) && (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG))
    case BM_MSG_GATT:
      pBuf = GATT_bm_alloc(connHandle, opcode, size, pSizeAlloc);
      break;

    case BM_MSG_L2CAP:
      pBuf = L2CAP_bm_alloc(size);
      break;

#endif /* defined(HOST_CONFIG) && (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG)) */
    default:
      pBuf = BM_alloc(size);
      break;
  }

  return (pBuf);
}

/*********************************************************************
 * @fn      bleDispatch_BMFree
 *
 * @brief   Implementation of the BM de-allocator functionality.
 *
 * @param   type - type of the message to free.
 * @param   pBuf - pointer to the memory to free.
 * @param   opcode - opcode of GATT message (applicable only to BM_MSG_GATT
 *                   type).
 *
 * @return  none
 */
void bleDispatch_BMFree(uint8_t type, void *pBuf, uint8_t opcode)
{
  switch (type)
  {
#if defined(HOST_CONFIG) && (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG))
    case BM_MSG_GATT:
      GATT_bm_free((gattMsg_t *)pBuf, opcode);
      break;

    case BM_MSG_L2CAP:
      /*lint --fallthrough */
#endif /* defined(HOST_CONFIG) && (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG)) */
    default:
      BM_free(pBuf);
      break;
  }
}

/*********************************************************************
 * @fn      buildRevision
 *
 * @brief   Read the Build Revision used to build the BLE stack.
 *
 * @param   pBuildRev - pointer to variable to copy build revision into
 *
 * @return  SUCCESS: Operation was successfully.
 *          INVALIDPARAMETER: Invalid parameter.
 */
uint8 buildRevision(ICall_BuildRevision *pBuildRev)
{
  if (pBuildRev!= NULL)
  {
    pBuildRev->stackVersion = (uint32)STACK_REVISION;
    pBuildRev->buildVersion = (uint16)ll_buildRevision;

    // Stack info (Byte 5)
    //  Bit    0: IAR used to build stack project (0: no, 1: yes)
    //  Bit    1: CCS used to build stack project (0: no, 1: yes)
    //  Bits 2-3: Reserved
    //  Bit    4: IAR used to build stack library (0: no, 1: yes)
    //  Bits 5-6: Reserved
    //  Bit    7: ROM build (0: no, 1: yes)
    pBuildRev->stackInfo =
#if defined(__IAR_SYSTEMS_ICC__)
      BLDREV_STK_IAR_PROJ |
#endif // __IAR_SYSTEMS_ICC__
#if defined(__TI_COMPILER_VERSION__)
      BLDREV_STK_CCS_PROJ |
#endif // __TI_COMPILER_VERSION__
      0;

    // Controller info - part 1 (Byte 6)
    //  Bit  0: ADV_NCONN_CFG (0: not included, 1: included)
    //  Bit  1: ADV_CONN_CFG (0: not included, 1: included)
    //  Bit  2: SCAN_CFG (0: not included, 1: included)
    //  Bit  3: INIT_CFG (0: not included, 1: included)
    //  Bit  4: PING_CFG (0: not included, 1: included)
    //  Bit  5: SLV_FEAT_EXCHG_CFG (0: not included, 1: included)
    //  Bit  6: CONN_PARAM_REQ_CFG (0: not included, 1: included)
    //  Bit  7: Reserved
    pBuildRev->ctrlInfo =
#if defined(CTRL_CONFIG)
      CTRL_CONFIG                    |
#endif // CTRL_CONFIG
      BLDREV_CTRL_PING_CFG           |
      BLDREV_CTRL_SLV_FEAT_EXCHG_CFG |
      BLDREV_CTRL_CONN_PARAM_REQ_CFG |
      0;

    // Host info - part 1 (Byte 8)
    //  Bit    0: BROADCASTER_CFG (0: not included, 1: included)
    //  Bit    1: OBSERVER_CFG (0: not included, 1: included)
    //  Bit    2: PERIPHERAL_CFG (0: not included, 1: included)
    //  Bit    3: CENTRAL_CFG (0: not included, 1: included)
    //  Bit    4: GAP_BOND_MGR (0: not included, 1: included)
    //  Bit    5: L2CAP_CO_CHANNELS (0: not included, 1: included)
    //  Bits 6-7: Reserved
    pBuildRev->hostInfo =
#if defined(HOST_CONFIG)
      (HOST_CONFIG)                 |
#endif // HOST_CONFIG
#if defined(BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
      BLDREV_HOST_L2CAP_CO_CHANNELS |
#endif //(BLE_V41_FEATURES & L2CAP_COC_CFG)
      0;

#if defined(HOST_CONFIG)
    if ( OPT_GAPBondMgr_isEnable() == TRUE )
    {
        pBuildRev->hostInfo |= BLDREV_HOST_GAP_BOND_MGR;
    }
#endif // HOST_CONFIG

    return (SUCCESS);
  }

  return (INVALIDPARAMETER);
}

/*********************************************************************
 * @fn      icall_liteMsgParser
 *
 * @brief   parsing of the ble specific icall direct API message.
 *
 * @param   *msg - pointer to message receive
 *
 * @return  None
*/
void icall_liteMsgParser(void * msg)
{
  osal_msg_hdr_t *hdr;

  hdr = (osal_msg_hdr_t *) msg - 1;

  if (hdr->format == ICALL_MSG_FORMAT_DIRECT_API_ID)
  {
      osal_msg_send(icall_liteTaskId, msg);
  }
}

/*********************************************************************
 * @fn      ble_dispatch_liteProcess
 *
 * @brief   Process osal message send to the icall lite task. This allows icall
 *          lite direct API messages to be parse with the lowest priority, and
 *          not diverge from the previous implementation.
 *
 * @param   taskId.
 *          events
 *
 * @return  None
 */
uint32 ble_dispatch_liteProcess(uint8_t  taskId, uint32 events)
{
  uint8 *pMsg;
  VOID taskId;

  if ( events & SYS_EVENT_MSG )
  {
    if ( (pMsg = MAP_osal_msg_receive( icall_liteTaskId )) != NULL )
    {
      icall_directAPIMsg_t* appMsg = (icall_directAPIMsg_t*) pMsg;
      icall_liteTranslation(appMsg);
    }

    // Return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }
  return 0;
}

/*********************************************************************
 * @fn      ble_dispatch_liteInit
 *
 * @brief   initialize the icall lite osal task.
 *
 * @param   taskId.
 *
 * @return  None
 */
void ble_dispatch_liteInit(uint8_t  taskId)
{
  icall_liteTaskId = taskId;
}


/*********************************************************************
 * @fn      sendLiteCmdStatus
 *
 * @brief   Send command status message to the API callee.
 *
 * @param   taskId -task Id of the API callee.
 *
 * @return  None
 */
static void sendLiteCmdStatus(uint8 taskId)
{
  ICall_LiteCmdStatus *pMsg =
    (ICall_LiteCmdStatus *)osal_msg_allocate(sizeof(ICall_LiteCmdStatus));

  if (pMsg)
  {
    pMsg->cmdId = ICALL_LITE_DIRECT_API_DONE_CMD_ID;
#ifdef ICALL_EVENTS
    osal_service_complete(taskId, (uint8 *)pMsg);
#else
    osal_msg_send(taskId, (uint8 *)pMsg);
#endif
  }
}

/*********************************************************************
 * @fn      icall_liteTranslation
 *
 * @brief   Translate the icall direct API Message to a stack API call.
 *
 * @param   pMsg - pointer to the received message.
 *
 * @return  None
 */
void icall_liteTranslation(icall_directAPIMsg_t *pMsg)
{
  osal_msg_hdr_t *hdr;
  uint8 taskId;  //msg_ptr->hciExtCmd.srctaskid;

  pMsg->pointerStack[0] = ((directAPIFctPtr_t)(pMsg->directAPI))
#if defined(ICALL_LITE_12_PARAMS)
                                                   (pMsg->pointerStack[0],
                                                    pMsg->pointerStack[1],
                                                    pMsg->pointerStack[2],
                                                    pMsg->pointerStack[3],
                                                    pMsg->pointerStack[4],
                                                    pMsg->pointerStack[5],
                                                    pMsg->pointerStack[6],
                                                    pMsg->pointerStack[7],
                                                    pMsg->pointerStack[8],
                                                    pMsg->pointerStack[9],
                                                    pMsg->pointerStack[10],
                                                    pMsg->pointerStack[11]);
#else
                                                   (pMsg->pointerStack[0],
                                                    pMsg->pointerStack[1],
                                                    pMsg->pointerStack[2],
                                                    pMsg->pointerStack[3],
                                                    pMsg->pointerStack[4],
                                                    pMsg->pointerStack[5],
                                                    pMsg->pointerStack[6],
                                                    pMsg->pointerStack[7]);
#endif

  hdr = (osal_msg_hdr_t *) pMsg - 1;
  taskId = osal_alien2proxy(hdr->srcentity);

  // post Message confirming the end of the API call.
  sendLiteCmdStatus(taskId);
}

/*********************************************************************
*********************************************************************/
