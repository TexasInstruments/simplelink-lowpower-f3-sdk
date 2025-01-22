/******************************************************************************

@file  bleapputil_internal.h

@brief This file contains the bleapputil_internal for use
with the Bluetooth Low Energy Protocol Stack.

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

#ifndef BLEAPPUTIL_INTERNAL_H
#define BLEAPPUTIL_INTERNAL_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * INCLUDES
 */
#include <bcomdef.h>

/*POSIX*/
#include <pthread.h>
#include <mqueue.h>
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */
// BLEAppUtil events
typedef enum BLEAppBLEAppUtil_Evt_e
{
    BLEAPPUTIL_EVT_STACK_CALLBACK,            // BLE stack events
    BLEAPPUTIL_EVT_ADV_CB_EVENT,              // Advertisement Callback events
    BLEAPPUTIL_EVT_SCAN_CB_EVENT,             // Scan Callback events
    BLEAPPUTIL_EVT_PAIRING_STATE_CB,          // Pairing states event from GapBond manager callback
    BLEAPPUTIL_EVT_PASSCODE_NEEDED_CB,        // Passcode confirm from GapBond manager callback
    BLEAPPUTIL_EVT_CONN_EVENT_CB,             // Connection event callback
    BLEAPPUTIL_EVT_HANDOVER_SN_EVENT_CB,      // Handover Serving node callback
    BLEAPPUTIL_EVT_HANDOVER_CN_EVENT_CB,      // Handover Candidate node callback
    BLEAPPUTIL_EVT_CALL_IN_BLEAPPUTIL_CONTEXT // switch context and call callback
} BLEAppBLEAppUtil_Evt_e;

typedef struct
{
    BLEAppUtil_EventHandler_t               *eventHandler;
    struct BLEAppUtil_EventHandlersList_t   *next;
} BLEAppUtil_EventHandlersList_t;

/** @internal data structure for the thread entity */
typedef struct
{
    pthread_t threadId;
    mqd_t     queueHandle;
} BLEAppUtil_TheardEntity_t;

/** @internal data structure for callback context switch*/
typedef struct
{
    // The callback that will be called from the BLE App Util context
    InvokeFromBLEAppUtilContext_t   callback;
    // The data that will be passed as an input to the callback
    char                            *data;
} BLEAppUtil_CallbackToInvoke_t;

typedef struct
{
    uint8_t event;                // event type
    void    *pData;               // pointer to message
} BLEAppUtil_appEvt_t;

// Used to pass the event from a "BLE App Util" event and it's handler
typedef struct
{
    uint32_t                        event;          // the BLE App Util event
    BLEAppUtil_eventHandlerType_e   handlerType;    // the event handler type
} BLEAppUtil_eventAndHandlerType_t;

// The stack msg format passed to the BLE App Util event queue
typedef struct
{
    BLEAppUtil_msgHdr_t               *pMessage;            // the stack event MSG
    BLEAppUtil_eventAndHandlerType_t   eventAndHandlerType; // the BLE App Util event + handler type
} BLEAppUtil_stackMsgData_t;

// The connection events notifications
typedef struct
{
    uint32_t event;
    Gap_ConnEventRpt_t *connEventReport;
} BLEAppUtil_connEventNoti_t;

typedef struct
{
    uint32_t event;
    uint32_t status;
} BLEAppUtil_handoverEvent_t;
/*********************************************************************
 * GLOBAL VARIABLES
 */

// BLEAppUtil parameters given in the init function
extern BLEAppUtil_GeneralParams_t *BLEAppUtilLocal_GeneralParams;

// Callback functions handlers
extern ErrorHandler_t errorHandlerCb;
extern StackInitDone_t appInitDoneHandler;
extern BLEAppUtil_EventHandlersList_t *BLEAppUtilEventHandlersHead;

extern pthread_mutex_t mutex;
extern BLEAppUtil_TheardEntity_t BLEAppUtil_theardEntity;

/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * Process stack events
 */
void BLEAppUtil_processStackEvents(BLEAppUtil_msgHdr_t *pMsg, BLEAppUtil_eventAndHandlerType_t bleAppUtilEventAndHandle);

/*********************************************************************
 * Process stack callbacks
 */
void BLEAppUtil_processPasscodeMsg(bleStack_msgHdt_t *pMsgData);
void BLEAppUtil_processPairStateMsg(bleStack_msgHdt_t *pMsgData);
void BLEAppUtil_processConnEventMsg(BLEAppUtil_connEventNoti_t *pMsg);
void BLEAppUtil_processScanEventMsg(bleStack_msgHdt_t *pMsg);
void BLEAppUtil_processAdvEventMsg(bleStack_msgHdt_t *pMsg);
void BLEAppUtil_processHandoverEventMsg(uint32_t event, BLEAppUtil_msgHdr_t *pMsgData);

/*********************************************************************
 * Stack callbacks
 */
uint8_t BLEAppUtil_processStackMsgCB(uint8_t event, uint8_t *pMsg);
void BLEAppUtil_passcodeCB(uint8_t *pDeviceAddr, uint16_t connHandle,
                           uint8_t uiInputs, uint8_t uiOutputs,
                           uint32_t numComparison);
void BLEAppUtil_pairStateCB(uint16_t connHandle, uint8_t state, uint8_t status);
void BLEAppUtil_connEventCB(Gap_ConnEventRpt_t *pReport);
void BLEAppUtil_scanCB(uint32_t event, GapScan_data_t *pBuf, uint32_t *arg);
void BLEAppUtil_advCB(uint32_t event, GapAdv_data_t *pBuf, uint32_t *arg);
void BLEAppUtil_HandoverSNCB(uint16_t connHandle, uint32_t status);
void BLEAppUtil_HandoverCNCB(uint16_t connHandle, uint32_t status);

/*********************************************************************
 * General Functions
 */
int BLEAppUtil_createBLEAppUtilTask(void);
void BLEAppUtil_stackRegister(void);
void BLEAppUtil_stackInit(void);
status_t BLEAppUtil_enqueueMsg(uint8_t event, void *pData);

/*********************************************************************
 * Event Handlers Related Functions
 */
void BLEAppUtil_callEventHandler(uint32_t event, BLEAppUtil_msgHdr_t *pMsg,
                                 BLEAppUtil_eventHandlerType_e type);
uint8_t BLEAppUtil_isEventEnabled(BLEAppUtil_eventHandlerType_e eventHandlerType,
                                  uint32_t event);
BLEAppUtil_EventHandlersList_t *BLEAppUtil_getEventHandler(BLEAppUtil_EventHandlersList_t *handler,
                                                           BLEAppUtil_eventHandlerType_e eventHandlerType);

/*********************************************************************
 * Convert and validate received events before enqueue
 */
uint8_t BLEAppUtil_convertGAPEvents(bleStack_msgHdt_t *pMsg,
                                    BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle);
uint8_t BLEAppUtil_convertGATTEvents(BLEAppUtil_msgHdr_t *pMsg,
                                     BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle);
uint8_t BLEAppUtil_convertHCIGAPEvents(bleStack_msgHdt_t *pMsg,
                                       BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle);
uint8_t BLEAppUtil_convertHCIDataEvents(bleStack_msgHdt_t *pMsg,
                                        BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle);
uint8_t BLEAppUtil_convertHCISMPEvents(bleStack_msgHdt_t *pMsg,
                                       BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle);
uint8_t BLEAppUtil_convertHCISMPMetaEvents(bleStack_msgHdt_t *pMsg,
                                           BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle);
uint8_t BLEAppUtil_convertL2CAPDataMsg(bleStack_msgHdt_t *pMsg,
                                       BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle);
uint8_t BLEAppUtil_convertL2CAPSignalEvents(bleStack_msgHdt_t *pMsg,
                                            BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle);
uint8_t BLEAppUtil_convertHCICTRLToHostEvents(bleStack_msgHdt_t *pMsg,
                                              BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle);
uint8_t BLEAppUtil_isConnEventRequired(Gap_ConnEventRpt_t *pMsg,
                                       uint32_t *event);
uint8_t BLEAppUtil_isStackEventRequired(BLEAppUtil_msgHdr_t *pMsg,
                                        BLEAppUtil_eventAndHandlerType_t *bleAppUtilEventAndHandle);
uint8_t BLEAppUtil_isPairStateEventRequired(uint8_t event,
                                            uint32_t *bleAppUtilEvent);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* BLEAPPUTIL_INTERNAL_H */
