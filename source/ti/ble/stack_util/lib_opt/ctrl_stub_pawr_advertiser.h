/******************************************************************************

 @file  ctrl_stub_pawr_advertiser.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_pawr_advertiser.c.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025-2026, Texas Instruments Incorporated

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

#ifndef CTRL_PAWR_ADVERTISER_H_
#define CTRL_PAWR_ADVERTISER_H_

#include "ti/ble/stack_util/comdef.h"
#include "ti/ble/controller/hci/hci.h"
#include "ti/ble/controller/ll/ll_scheduler.h"
#include "ti/ble/controller/ll/ll_pawr_advertiser.h"
#include "ti/ble/controller/ll/ll.h"

// Function prototypes for the actual implementations
extern uint8_t LE_SetPeriodicAdvSubeventData(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pRfCmd, uint8_t* props, uint8_t numSubevents, uint8_t* subEvents);
extern bool LL_PAwRA_IsEnable(void);
extern void LL_PAwRA_SetFeatureBit(void);
extern bool LL_PAwRA_IsActive(void);
extern uint8_t LL_PAwRA_IsCreateConnInProgress(void);
extern uint16_t LL_PAwRA_GetConnId(void);
extern aeCreateConnCmd_t* LL_PAwRA_GetCreateConnParams(void);
extern void LL_PAwRA_ClearCreateConn(void);
extern uint8_t LL_PAwRA_GetCreateConnAdvHandle(void);
extern void LL_PAwRA_CleanupCreateConn(uint8_t* pPAwRParams);
extern uint8_t LL_PAwRA_FillTimingInfo(uint8_t* pPAwRParams, uint8_t* pTimingInfoBuffer, uint8_t* pAcadLen);
extern bool LL_PAwRA_IsPAwR(uint8_t* pPAwRParams);
extern uint8_t LL_PAwRA_ValidateParams(uint16_t minPeriodicInterval, uint8_t* pPAwRParams);
extern uint8_t LL_PAwRA_SetParams(uint8_t** pPAwRParams, uint8_t* pRawPAwRParams);
extern uint8_t LL_PAwRA_SetupCmd(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pRxCmd, RCL_CtxPeriodicAdvertiser* pRxCtx);
extern uint8_t LL_PAwRA_Init(void);
extern uint8_t LL_PAwRA_PostProcess(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pRxCmd, llPAwRAPostProcessIn_t* pInParams, llPAwRAPostProcessOut_t* pOutParams);
extern uint8_t LL_PAwRA_ClearSet(uint8_t* pPAwRParams);
extern uint8_t LL_PAwRA_AdjusmentSet(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* rfCmd, llPAwRAPostProcessIn_t* pInParams, llPAwRAPostProcessOut_t* pOutParams);
extern uint8_t LL_PAwRA_HandleSubeventDataList(uint8_t advHandle, uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pRfCmd);
extern uint8_t LL_PAwRA_SetSubeventDataToPacket(comExtPktFormat_t* comPkt, uint8_t* pPAwRParams);
extern uint8_t LL_PAwRA_UpdateResponseParams(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pAdvCmd);
extern uint8_t LL_PAwRA_RspRxFifo(uint8_t advHandle, uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pAdvCmd);
extern uint8_t LL_PAwRA_GenericRxPostProcess(uint8_t advHandle, uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pAdvCmd);
extern void* LL_PAwRA_GetPendingRxCmd(uint8_t* pPAwRParams);
extern bool LL_PAwRA_IsCurrentSubeventZero(uint8_t* pPAwRParams);
extern uint8_t LL_PAwRA_IsGenericRxPending(uint8_t* pPAwRParams);
extern uint8_t LL_PAwRA_CreateConn(uint8_t subevent, uint8_t advHandle, uint8_t periodicAdvPhy, uint8_t* pPAwRParams, aeCreateConnCmd_t* pCreateConnParams, RCL_CmdBle5PeriodicAdvertiser* pPerAdvCmd, RCL_CtxPeriodicAdvertiser* pPerAdvParam);
extern bool LL_PAwRA_IsCurrentSubeventToConnect(uint8_t* pPAwRParams);
extern void LL_PAwRA_HandleConnection(uint8_t advHandle, uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pAdvCmd);
extern bool LL_PAwRA_InsertConnReqToTxIfNeeded(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pPerAdvCmd);


// Wrapper functions for the feature implementations
uint8_t OPT_LE_SetPeriodicAdvSubeventData(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pRfCmd, uint8_t* props, uint8_t numSubevents, uint8_t* subEvents);
bool OPT_LL_PAwRA_IsEnable(void);
void OPT_LL_PAwRA_SetFeatureBit(void);
bool OPT_LL_PAwRA_IsActive(void);
uint8_t OPT_LL_PAwRA_IsCreateConnInProgress(void);
uint16_t OPT_LL_PAwRA_GetConnId(void);
aeCreateConnCmd_t* OPT_LL_PAwRA_GetCreateConnParams(void);
void OPT_LL_PAwRA_ClearCreateConn(void);
uint8_t OPT_LL_PAwRA_GetCreateConnAdvHandle(void);
void OPT_LL_PAwRA_CleanupCreateConn(uint8_t* pPAwRParams);
uint8_t OPT_LL_PAwRA_FillTimingInfo(uint8_t* pPAwRParams, uint8_t* pTimingInfoBuffer, uint8_t* pAcadLen);
bool OPT_LL_PAwRA_IsPAwR(uint8_t* pPAwRParams);
uint8_t OPT_LL_PAwRA_ValidateParams(uint16_t minPeriodicInterval, uint8_t* pPAwRParams);
uint8_t OPT_LL_PAwRA_SetParams(uint8_t** pPAwRParams, uint8_t* pRawPAwRParams);
uint8_t OPT_LL_PAwRA_SetupCmd(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pRxCmd, RCL_CtxPeriodicAdvertiser* pRxCtx);
uint8_t OPT_LL_PAwRA_Init(void);
uint8_t OPT_LL_PAwRA_PostProcess(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pRxCmd, llPAwRAPostProcessIn_t* pInParams, llPAwRAPostProcessOut_t* pOutParams);
uint8_t OPT_LL_PAwRA_ClearSet(uint8_t* pPAwRParams);
uint8_t OPT_LL_PAwRA_AdjusmentSet(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* rfCmd, llPAwRAPostProcessIn_t* pInParams, llPAwRAPostProcessOut_t* pOutParams);
uint8_t OPT_LL_PAwRA_HandleSubeventDataList(uint8_t advHandle, uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pRfCmd);
uint8_t OPT_LL_PAwRA_SetSubeventDataToPacket(comExtPktFormat_t* comPkt, uint8_t* pPAwRParams);
uint8_t OPT_LL_PAwRA_UpdateResponseParams(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pAdvCmd);
uint8_t OPT_LL_PAwRA_RspRxFifo(uint8_t advHandle, uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pAdvCmd);
uint8_t OPT_LL_PAwRA_GenericRxPostProcess(uint8_t advHandle, uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pAdvCmd);
void* OPT_LL_PAwRA_GetPendingRxCmd(uint8_t* pPAwRParams);
bool OPT_LL_PAwRA_IsCurrentSubeventZero(uint8_t* pPAwRParams);
uint8_t OPT_LL_PAwRA_IsGenericRxPending(uint8_t* pPAwRParams);
uint8_t OPT_LL_PAwRA_CreateConn(uint8_t subevent, uint8_t advHandle, uint8_t periodicAdvPhy, uint8_t* pPAwRParams, aeCreateConnCmd_t* pCreateConnParams, RCL_CmdBle5PeriodicAdvertiser* pPerAdvCmd, RCL_CtxPeriodicAdvertiser* pPerAdvParam);
bool OPT_LL_PAwRA_IsCurrentSubeventToConnect(uint8_t* pPAwRParams);
void OPT_LL_PAwRA_HandleConnection(uint8_t advHandle, uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pAdvCmd);
bool OPT_LL_PAwRA_InsertConnReqToTxIfNeeded(uint8_t* pPAwRParams, RCL_CmdBle5PeriodicAdvertiser* pPerAdvCmd);

#endif /* CTRL_PAWR_ADVERTISER_H_ */
