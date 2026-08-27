/******************************************************************************

 @file  ctrl_stub_padv.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_padv.c.

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

#ifndef CTRL_PADV_H_
#define CTRL_PADV_H_

#include "ti/ble/stack_util/comdef.h"
#include "ti/ble/controller/hci/hci.h"
#include "ti/ble/controller/ll/ll_scheduler.h"
#include "ti/ble/controller/ll/ll_ae.h"
#include "ti/ble/controller/ll/ll.h"
#include "ti/ble/controller/ll/ll_padv_advertiser.h"

// Function prototypes for the actual implementations
extern void llSetTaskPeriodicAdv(void);
extern void LL_PadvA_ClearAllSets(void);
extern void LL_PadvA_SetChanMapUpdate(uint8_t set);
extern void * LL_PadvA_GetNextSet(void);
extern llStatus_t LL_PadvA_SetupCommand(advSet_t * pAdvSet);
extern void LL_PadvA_BuildSyncInfo(advSet_t * pAdvSet, uint8_t * pBuf);
extern llPeriodicAdvSet_t * LL_PadvA_GetCurrent(void);
extern llPeriodicAdvSet_t * LL_PadvA_GetSetByHandle(uint8_t handle);
extern taskInfo_t * llSelectTaskPeriodicAdv(uint8_t secTaskID, uint32_t timeGap);
extern hciStatus_t hciCmdParserPeriodicAdv(uint8_t * pData, uint16_t cmdOpCode);
extern llStatus_t LE_SetPeriodicAdvParams(uint8_t advHandle, uint16_t periodicAdvIntervalMin, uint16_t periodicAdvIntervalMax, uint16_t periodicAdvProp, uint8_t* pPAwRParams);
extern llStatus_t LE_SetPeriodicAdvData(uint8_t advHandle, uint8_t operation, uint8_t dataLength, uint8_t * data);
extern llStatus_t LE_SetPeriodicAdvEnable(uint8_t enable, uint8_t advHandle);
extern uint8_t LL_PadvA_CompareQosPriorityToConn(llConnState_t * primConnPtr);
extern uint8_t LL_PadvA_GetCurrentQOSPriority(uint8_t curllStatePriority);
extern uint32_t LL_PadvA_GetCurrentTotalOtaTime(uint32_t curTotalOtaTime);
extern uint8_t LL_PadvA_ValidateExtAdvParams(aeSetParamCmd_t * pCmdParams);
extern uint8_t LL_PadvA_AddACADToAdv(uint8_t handle, uint8_t * pBuf, uint8 * pAcadLen);
extern void LL_PadvA_SetQOSPriority(uint32_t priority, uint16_t handle);
extern uint32_t llHandlePeriodicAdvEvents(uint32_t events);
extern bool LL_PadvA_IsEnable(void);
extern bool LL_PadvA_IsExistByHandle(uint8_t advHandle);
extern void LL_PadvA_GetTxUsageParams(llTxUsageParams_t* pPeriodicTxParams);
extern llPeriodicAdvSetType_e LL_PadvA_GetTypeByHandle(uint8_t advHandle);
extern llStatus_t LL_PadvA_GetSyncTransferInfoByHandle(uint8_t advHandle, llPeriodicSyncTransferInfo_t* pPeriodicSyncTransferData);
extern void LL_PadvA_PostProcess(void);
extern void LL_PadvA_UpdateChainPacket(void);
extern uint8_t LL_PadvA_GetStateByHandle(uint8_t handle);
extern uint8_t LL_PadvA_GetPendingDisableFlag(uint8_t handle);
extern void LL_PadvA_InitPAwRSubeventDataList(uint8_t handle);
extern uint8_t LL_PadvA_TriggerCommand(advSet_t* pAdvSet, llPeriodicAdvSet_t* pPeriodicAdv);
extern void llExtAdvPeriodicHandler(advSet_t* pAdvSet);
extern void llExtAdvTriggerPeriodicIfNeeded(advSet_t* pAdvSet);


// Wrapper functions for the feature implementations
void OPT_llSetTaskPeriodicAdv(void);
void OPT_LL_PadvA_ClearAllSets(void);
void OPT_LL_PadvA_SetChanMapUpdate(uint8_t set);
void * OPT_LL_PadvA_GetNextSet(void);
llStatus_t OPT_LL_PadvA_SetupCommand(advSet_t * pAdvSet);
void OPT_LL_PadvA_BuildSyncInfo(advSet_t * pAdvSet, uint8_t * pBuf);
llPeriodicAdvSet_t * OPT_LL_PadvA_GetCurrent(void);
llPeriodicAdvSet_t * OPT_LL_PadvA_GetSetByHandle(uint8_t handle);
taskInfo_t * OPT_llSelectTaskPeriodicAdv(uint8_t secTaskID, uint32_t timeGap);
hciStatus_t OPT_hciCmdParserPeriodicAdv(uint8_t * pData, uint16_t cmdOpCode);
llStatus_t OPT_LE_SetPeriodicAdvParams(uint8_t advHandle, uint16_t periodicAdvIntervalMin, uint16_t periodicAdvIntervalMax, uint16_t periodicAdvProp, uint8_t* pPAwRParams);
llStatus_t OPT_LE_SetPeriodicAdvData(uint8_t advHandle, uint8_t operation, uint8_t dataLength, uint8_t * data);
llStatus_t OPT_LE_SetPeriodicAdvEnable(uint8_t enable, uint8_t advHandle);
uint8_t OPT_LL_PadvA_CompareQosPriorityToConn(llConnState_t * primConnPtr);
uint8_t OPT_LL_PadvA_GetCurrentQOSPriority(uint8_t curllStatePriority);
uint32_t OPT_LL_PadvA_GetCurrentTotalOtaTime(uint32_t curTotalOtaTime);
uint8_t OPT_LL_PadvA_ValidateExtAdvParams(aeSetParamCmd_t * pCmdParams);
uint8_t OPT_LL_PadvA_AddACADToAdv(uint8_t handle, uint8_t * pBuf, uint8 * pAcadLen);
void OPT_LL_PadvA_SetQOSPriority(uint32_t priority, uint16_t handle);
uint32_t OPT_llHandlePeriodicAdvEvents(uint32_t events);
bool OPT_LL_PadvA_IsEnable(void);
bool OPT_LL_PadvA_IsExistByHandle(uint8_t advHandle);
void OPT_LL_PadvA_GetTxUsageParams(llTxUsageParams_t* pPeriodicTxParams);
llPeriodicAdvSetType_e OPT_LL_PadvA_GetTypeByHandle(uint8_t advHandle);
llStatus_t OPT_LL_PadvA_GetSyncTransferInfoByHandle(uint8_t advHandle, llPeriodicSyncTransferInfo_t* pPeriodicSyncTransferData);
void OPT_LL_PadvA_PostProcess(void);
void OPT_LL_PadvA_UpdateChainPacket(void);
uint8_t OPT_LL_PadvA_GetStateByHandle(uint8_t handle);
uint8_t OPT_LL_PadvA_GetPendingDisableFlag(uint8_t handle);
void OPT_LL_PadvA_InitPAwRSubeventDataList(uint8_t handle);
uint8_t OPT_LL_PadvA_TriggerCommand(advSet_t* pAdvSet, llPeriodicAdvSet_t* pPeriodicAdv);
void OPT_llExtAdvPeriodicHandler(advSet_t* pAdvSet);
void OPT_llExtAdvTriggerPeriodicIfNeeded(advSet_t* pAdvSet);

#endif /* CTRL_PADV_H_ */
