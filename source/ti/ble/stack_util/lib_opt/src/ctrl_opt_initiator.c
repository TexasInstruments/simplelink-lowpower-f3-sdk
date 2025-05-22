/******************************************************************************

 @file  ctrl_opt_initiator.c

 @brief Implementation of the feature wrapper functions that direct to the full
        feature implementations.
        Enabling the feature by linking the wrapper functions to the real
        implementations defined in the library.

        Usage:
        Include this file in the build only when the feature is enabled.
        Add to the project CTRL_INITIATOR_CFG defines
        The real functions must be implemented in the linked library.

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

#include "ti/ble/stack_util/lib_opt/opt_dependencies.h"
#include "ti/ble/stack_util/lib_opt/ctrl_stub_initiator.h"

#if defined(CTRL_INITIATOR_CFG)

#if !defined(CTRL_SCANNER_CFG)
#error "One or more dependencies are missing! Please add them."
#endif

void OPT_llProcessCentralConnectionCreated(void)
{
    llProcessCentralConnectionCreated();
}

void OPT_llProcessCentralControlPacket(llConnState_t* connPtr, uint8_t* pPkt)
{
    llProcessCentralControlPacket(connPtr, pPkt);
}

void OPT_llSetTaskInit(uint8_t startType, taskInfo_t* nextSecTask, void* nextSecCmd, void* nextConnCmd)
{
    llSetTaskInit(startType, nextSecTask, nextSecCmd, nextConnCmd);
}

void OPT_llSetTaskCentral(uint8_t connId, void* nextConnCmd)
{
    llSetTaskCentral(connId, nextConnCmd);
}

taskInfo_t* OPT_llSelectTaskInit(uint8_t secTaskID, uint32_t timeGap)
{
    return llSelectTaskInit(secTaskID, timeGap);
}

llStatus_t OPT_LL_ChanMapUpdate(uint8_t* chanMap, uint16_t connID)
{
    return LL_ChanMapUpdate(chanMap, connID);
}

void OPT_llCentral_TaskEnd(void)
{
    llCentral_TaskEnd();
}

void OPT_llExtInit_PostProcess(void)
{
    llExtInit_PostProcess();
}

llStatus_t OPT_LL_CreateConnCancel(void)
{
    return LL_CreateConnCancel();
}

void OPT_LL_SetConnIdInExtInit(uint8_t connId)
{
    LL_SetConnIdInExtInit(connId);
}

void OPT_llPostSetupCtrlPktCent(llConnState_t* connPtr, uint8_t ctrlPkt)
{
    llPostSetupCtrlPktCent(connPtr, ctrlPkt);
}

hciStatus_t OPT_hciCmdParserInitiator(uint8_t* pData, uint16_t cmdOpCode)
{
    return hciCmdParserInitiator(pData, cmdOpCode);
}

llStatus_t OPT_HCI_TL_create_conn(uint16_t opcode, uint8_t* pHciParams)
{
    return HCI_TL_create_conn(opcode, pHciParams);
}

llStatus_t OPT_HCI_TL_ext_create_conn(uint16_t opcode, uint8_t* pHciParams)
{
    return HCI_TL_ext_create_conn(opcode, pHciParams);
}

hciStatus_t OPT_hciCmdParserVendorSpecificInitiator(uint8_t* pData, uint16_t cmdOpCode)
{
    return hciCmdParserVendorSpecificInitiator(pData, cmdOpCode);
}

void OPT_LL_rclInitRxEntryDone(void)
{
    LL_rclInitRxEntryDone();
}

void OPT_llInit_TaskConnect(void)
{
    llInit_TaskConnect();
}

void OPT_llExtInit_ResolveConnRsp(void)
{
    llExtInit_ResolveConnRsp();
}

void OPT_llBuildCtrlPktCent(llConnState_t* connPtr, uint8_t* pData, uint8_t ctrlPkt)
{
    llBuildCtrlPktCent(connPtr, pData, ctrlPkt);
}

void OPT_llCmdScanStartedEventHandle(void)
{
    llCmdScanStartedEventHandle();
}

uint8_t OPT_llIsInitPriorityValid(uint16_t taskID, uint8_t priority, uint8_t connPriority)
{
    return llIsInitPriorityValid(taskID, priority, connPriority);
}

llStatus_t OPT_llAllocExtInitInfo(void)
{
    return llAllocExtInitInfo();
}

void OPT_llSecTaskInitiatorHandle(taskInfo_t* secTask, RCL_Command* secCmd, llConnState_t* nextConnPtr, uint32_t* timeGap, uint32_t curTime)
{
    llSecTaskInitiatorHandle(secTask, secCmd, nextConnPtr, timeGap, curTime);
}

void OPT_llUpdateTimeGapForInitiator(uint32_t* timeGap)
{
    llUpdateTimeGapForInitiator(timeGap);
}

uint8_t OPT_llChackInitiatorUseAcceptList(void)
{
    return llChackInitiatorUseAcceptList();
}

uint8_t OPT_llCheckInitHealth(volatile uint32_t* initTime, volatile uint32_t* currentTime)
{
    return llCheckInitHealth(initTime, currentTime);
}

uint8_t OPT_LL_IsInitiatingWithRandomAddress(void)
{
    return LL_IsInitiatingWithRandomAddress();
}

void OPT_LL_UpdateInitiatorInfoScanValue(uint16_t scaInPPM)
{
    LL_UpdateInitiatorInfoScanValue(scaInPPM);
}

void OPT_LL_DisableScanningForInitiator(void)
{
    LL_DisableScanningForInitiator();
}

uint8_t OPT_LL_validateChannelMap(uint8_t* chanMap)
{
    return LL_validateChannelMap(chanMap);
}

RCL_Callback OPT_LL_GetRfCentralCallback(void)
{
    return LL_GetRfCentralCallback();
}

void OPT_LL_InitiatorReadSupportedStates(uint8_t* states)
{
    LL_InitiatorReadSupportedStates(states);
}

uint8_t OPT_LL_PRIV_ValidatePeerAddress(uint8_t* peerAddr, uint8_t peerAddrType, uint8_t peerRLIndex)
{
    return LL_PRIV_ValidatePeerAddress(peerAddr, peerAddrType, peerRLIndex);
}

#endif /* defined(CTRL_INITIATOR_CFG) */
