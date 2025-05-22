/******************************************************************************

 @file  ctrl_stub_initiator.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_initiator.c.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025, Texas Instruments Incorporated

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

#ifndef CTRL_INITIATOR_H_
#define CTRL_INITIATOR_H_

#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll_ble.h"
#include "ti/ble/controller/ll/ll_scheduler.h"
#include "ti/ble/controller/ll/ll.h"
#include "ti/ble/controller/hci/hci.h"

// Function prototypes for the actual implementations
extern void llProcessCentralConnectionCreated(void);
extern void llProcessCentralControlPacket(llConnState_t* connPtr, uint8_t* pPkt);
extern void llSetTaskInit(uint8_t startType, taskInfo_t* nextSecTask, void* nextSecCmd, void* nextConnCmd);
extern void llSetTaskCentral(uint8_t connId, void* nextConnCmd);
extern taskInfo_t* llSelectTaskInit(uint8_t secTaskID, uint32_t timeGap);
extern llStatus_t LL_ChanMapUpdate(uint8_t* chanMap, uint16_t connID);
extern void llCentral_TaskEnd(void);
extern void llExtInit_PostProcess(void);
extern llStatus_t LL_CreateConnCancel(void);
extern void LL_SetConnIdInExtInit(uint8_t connId);
extern void llPostSetupCtrlPktCent(llConnState_t* connPtr, uint8_t ctrlPkt);
extern hciStatus_t hciCmdParserInitiator(uint8_t* pData, uint16_t cmdOpCode);
extern llStatus_t HCI_TL_create_conn(uint16_t opcode, uint8_t* pHciParams);
extern llStatus_t HCI_TL_ext_create_conn(uint16_t opcode, uint8_t* pHciParams);
extern hciStatus_t hciCmdParserVendorSpecificInitiator(uint8_t* pData, uint16_t cmdOpCode);
extern void LL_rclInitRxEntryDone(void);
extern void llInit_TaskConnect(void);
extern void llExtInit_ResolveConnRsp(void);
extern void llBuildCtrlPktCent(llConnState_t* connPtr, uint8_t* pData, uint8_t ctrlPkt);
extern void llCmdScanStartedEventHandle(void);
extern uint8_t llIsInitPriorityValid(uint16_t taskID, uint8_t priority, uint8_t connPriority);
extern llStatus_t llAllocExtInitInfo(void);
extern void llSecTaskInitiatorHandle(taskInfo_t* secTask, RCL_Command* secCmd, llConnState_t* nextConnPtr, uint32_t* timeGap, uint32_t curTime);
extern void llUpdateTimeGapForInitiator(uint32_t* timeGap);
extern uint8_t llChackInitiatorUseAcceptList(void);
extern uint8_t llCheckInitHealth(volatile uint32_t* initTime, volatile uint32_t* currentTime);
extern uint8_t LL_IsInitiatingWithRandomAddress(void);
extern void LL_UpdateInitiatorInfoScanValue(uint16_t scaInPPM);
extern void LL_DisableScanningForInitiator(void);
extern uint8_t LL_validateChannelMap(uint8_t* chanMap);
extern RCL_Callback LL_GetRfCentralCallback(void);
extern void LL_InitiatorReadSupportedStates(uint8_t* states);
extern uint8_t LL_PRIV_ValidatePeerAddress(uint8_t* peerAddr, uint8_t peerAddrType, uint8_t peerRLIndex);

// Wrapper functions for the feature implementations
void OPT_llProcessCentralConnectionCreated(void);
void OPT_llProcessCentralControlPacket(llConnState_t* connPtr, uint8_t* pPkt);
void OPT_llSetTaskInit(uint8_t startType, taskInfo_t* nextSecTask, void* nextSecCmd, void* nextConnCmd);
void OPT_llSetTaskCentral(uint8_t connId, void* nextConnCmd);
taskInfo_t* OPT_llSelectTaskInit(uint8_t secTaskID, uint32_t timeGap);
llStatus_t OPT_LL_ChanMapUpdate(uint8_t* chanMap, uint16_t connID);
void OPT_llCentral_TaskEnd(void);
void OPT_llExtInit_PostProcess(void);
llStatus_t OPT_LL_CreateConnCancel(void);
void OPT_LL_SetConnIdInExtInit(uint8_t connId);
void OPT_llPostSetupCtrlPktCent(llConnState_t* connPtr, uint8_t ctrlPkt);
hciStatus_t OPT_hciCmdParserInitiator(uint8_t* pData, uint16_t cmdOpCode);
llStatus_t OPT_HCI_TL_create_conn(uint16_t opcode, uint8_t* pHciParams);
llStatus_t OPT_HCI_TL_ext_create_conn(uint16_t opcode, uint8_t* pHciParams);
hciStatus_t OPT_hciCmdParserVendorSpecificInitiator(uint8_t* pData, uint16_t cmdOpCode);
void OPT_LL_rclInitRxEntryDone(void);
void OPT_llInit_TaskConnect(void);
void OPT_llExtInit_ResolveConnRsp(void);
void OPT_llBuildCtrlPktCent(llConnState_t* connPtr, uint8_t* pData, uint8_t ctrlPkt);
void OPT_llCmdScanStartedEventHandle(void);
uint8_t OPT_llIsInitPriorityValid(uint16_t taskID, uint8_t priority, uint8_t connPriority);
llStatus_t OPT_llAllocExtInitInfo(void);
void OPT_llSecTaskInitiatorHandle(taskInfo_t* secTask, RCL_Command* secCmd, llConnState_t* nextConnPtr, uint32_t* timeGap, uint32_t curTime);
void OPT_llUpdateTimeGapForInitiator(uint32_t* timeGap);
uint8_t OPT_llChackInitiatorUseAcceptList(void);
uint8_t OPT_llCheckInitHealth(volatile uint32_t* initTime, volatile uint32_t* currentTime);
uint8_t OPT_LL_IsInitiatingWithRandomAddress(void);
void OPT_LL_UpdateInitiatorInfoScanValue(uint16_t scaInPPM);
void OPT_LL_DisableScanningForInitiator(void);
uint8_t OPT_LL_validateChannelMap(uint8_t* chanMap);
RCL_Callback OPT_LL_GetRfCentralCallback(void);
void OPT_LL_InitiatorReadSupportedStates(uint8_t* states);
uint8_t OPT_LL_PRIV_ValidatePeerAddress(uint8_t* peerAddr, uint8_t peerAddrType, uint8_t peerRLIndex);

#endif /* CTRL_INITIATOR_H_ */
