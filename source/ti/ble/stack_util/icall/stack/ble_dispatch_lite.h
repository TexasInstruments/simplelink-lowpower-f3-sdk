/******************************************************************************

 @file  ble_dispatch_lite.h

 @brief ICall BLE Dispatcher LITE Definitions.

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

#ifndef BLE_DISPATCH_LITE_H
#define BLE_DISPATCH_LITE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/icall/app/icall_ble_apimsg.h"
#ifndef CONTROLLER_ONLY
#include "ti/ble/host/sm/sm.h"
#include "ti/ble/host/gap/gap.h"
#include "ti/ble/host/gatt/gatt.h"
#include "ti/ble/host/gatt/gatt_uuid.h"
#include "ti/ble/host/gatt/gattservapp.h"
#include "ti/ble/host/gapbondmgr/gapbondmgr.h"
#endif // CONTROLLER_ONLY

/*********************************************************************
 * CONSTANTS
 */

// Stack Info field bitmaps
#define BLDREV_STK_IAR_PROJ                   0x01 // IAR used to build stack project
#define BLDREV_STK_CCS_PROJ                   0x02 // CCS used to build stack project

// Controller Info field bitmaps
#define BLDREV_CTRL_PING_CFG                  0x10 // Ping included
#define BLDREV_CTRL_SLV_FEAT_EXCHG_CFG        0x20 // Peripheral Feature Exchange included
#define BLDREV_CTRL_CONN_PARAM_REQ_CFG        0x40 // Connection Parameter Request included

// Host Info field bitmaps
#define BLDREV_HOST_GAP_BOND_MGR              0x10 // GAP Bond Manager included
#define BLDREV_HOST_L2CAP_CO_CHANNELS         0x20 // L2CAP CO Channels included

// BM Message Types by layer/module
#define BM_MSG_GATT                           1 // GATT layer
#define BM_MSG_L2CAP                          2 // L2CAP layer
#define BM_MSG_GENERIC                        3 // Lowest layer

/*******************************************************************************
 * TYPEDEFS
 */

// BM allocator and de-allocator function pointer types
typedef void* (*pfnBMAlloc_t)(uint8_t type, uint16_t size, uint16_t connHandle,
                              uint8_t opcode, uint16_t *pSizeAlloc);
typedef void  (*pfnBMFree_t)(uint8_t type, void *pMsg, uint8_t opcode);

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task Initialization for the task
 */
extern void ble_dispatch_liteInit(uint8_t taskId);

/*
 * Task Event Processor for the task
 */
extern uint32 ble_dispatch_liteProcess(uint8_t taskId, uint32 events);

/*
 * Implementation of the BM allocator functionality.
 */
extern void *bleDispatch_BMAlloc(uint8_t type, uint16_t size,
                                 uint16_t connHandle, uint8_t opcode,
                                 uint16_t *pSizeAlloc);
/*
 * Implementation of the BM de-allocator functionality.
 */
extern void bleDispatch_BMFree(uint8_t type, void *pBuf, uint8_t opcode);

/*
 * BLE direct API message parser
 */
extern void icall_liteMsgParser(void * msg);

/*
 * Error handler
 */
extern void icall_liteErrorFunction(void);

/*
 * Return revision of the stack
*/
extern uint8 buildRevision(ICall_BuildRevision *pBuildRev);


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* !BLE_DISPATCH_LITE_H */
