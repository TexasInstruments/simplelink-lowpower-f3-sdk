/******************************************************************************

 @file  ctrl_stub_connectable.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_connectable.c.

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

#ifndef CTRL_CONNECTABLE_H_
#define CTRL_CONNECTABLE_H_

#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll_ble.h"
#include "ti/ble/controller/ll/ll_scheduler.h"
#include "ti/ble/controller/ll/ll.h"
#include "ti/ble/controller/hci/hci.h"
#include "ti/ble/controller/ll/ll_enc.h"

// Function prototypes for the actual implementations
extern llStatus_t LL_ConnActive(uint16_t connId);
extern uint8_t llGetNextConn(void);
extern llConnState_t* llDataGetConnPtr(uint8_t connId);
extern uint8 llConnExists(uint8_t* peerAddr, uint8_t peerAddrType);
extern hciStatus_t hciCmdParserConnection(uint8_t* pData, uint16_t cmdOpCode);
extern hciStatus_t hciCmdParserVendorSpecificConnection(uint8_t* pData, uint16_t cmdOpCode);
extern uint8_t llRxEntryDoneEventHandleStateConnection(void);
extern void llProcessTxData(void);
extern void LL_RxDataCompleteCback(uint16_t connHandle, uint8_t* pBuf, uint16_t len, uint8_t fragFlag, int8_t rssi);
extern uint8_t llCheckConnHealth(uint32_t* currentTime, volatile uint32_t* connTime);
extern llStatus_t llDynamicAlloc_connectable(void);
extern void llDynamicFree_connectable(void);
extern void llChannelAlgo2SetFeatureBit(void);
extern void llSchedulerConnectable(taskInfo_t* curTask);
extern uint8_t llSchedulerSecondaryTasksConn(taskInfo_t* nextSecTask, void* nextSecCmd);
extern uint32_t llHandleConnDisconnectedImmed(uint32_t events);
extern void LL_clearAllActiveConns(void);
extern void LL_setComplPktsDefualtValues(void);
extern void LL_GetNumActiveConns(uint8_t* numActiveConns);
extern void HCI_TL_SendDataPkt(uint8_t* pMsg);
extern llStatus_t LL_RemoteConnParamReqReply(uint16_t connHandle, uint16_t connIntervalMin, uint16_t connIntervalMax, uint16_t connLatency, uint16_t connTimeout, uint16_t minLen, uint16_t maxLen);
extern void ll_eccInit(void);
extern void llsdaaEnableConnTXData(uint16_t currConnId);
extern void llsdaaDisableConnTXData(uint16_t connId);
extern void llConnSetRejectIndExt(llConnState_t * connPtr, uint8 rejectOpcode, uint8 errorCode);
extern void LL_GetConnTxUsageParams(llTxUsageParams_t* pConnTxParams);

// Wrapper functions for the feature implementations
llStatus_t OPT_LL_ConnActive(uint16_t connId);
uint8_t OPT_llGetNextConn(void);
llConnState_t* OPT_llDataGetConnPtr(uint8_t connId);
uint8 OPT_llConnExists(uint8_t* peerAddr, uint8_t peerAddrType);
hciStatus_t OPT_hciCmdParserConnection(uint8_t* pData, uint16_t cmdOpCode);
hciStatus_t OPT_hciCmdParserVendorSpecificConnection(uint8_t* pData, uint16_t cmdOpCode);
uint8_t OPT_llRxEntryDoneEventHandleStateConnection(void);
void OPT_llProcessTxData(void);
void OPT_LL_RxDataCompleteCback(uint16_t connHandle, uint8_t* pBuf, uint16_t len, uint8_t fragFlag, int8_t rssi);
uint8_t OPT_llCheckConnHealth(uint32_t* currentTime, volatile uint32_t* connTime);
llStatus_t OPT_llDynamicAlloc_connectable(void);
void OPT_llDynamicFree_connectable(void);
void OPT_llChannelAlgo2SetFeatureBit(void);
void OPT_llSchedulerConnectable(taskInfo_t* curTask);
uint8_t OPT_llSchedulerSecondaryTasksConn(taskInfo_t* nextSecTask, void* nextSecCmd);
uint32_t OPT_llHandleConnDisconnectedImmed(uint32_t events);
void OPT_LL_clearAllActiveConns(void);
void OPT_LL_setComplPktsDefualtValues(void);
void OPT_LL_GetNumActiveConns(uint8_t* numActiveConns);
void OPT_HCI_TL_SendDataPkt(uint8_t* pMsg);
llStatus_t OPT_LL_RemoteConnParamReqReply(uint16_t connHandle, uint16_t connIntervalMin, uint16_t connIntervalMax, uint16_t connLatency, uint16_t connTimeout, uint16_t minLen, uint16_t maxLen);
void OPT_ll_eccInit(void);
void OPT_llsdaaEnableConnTXData(uint16_t currConnId);
void OPT_llsdaaDisableConnTXData(uint16_t connId);
void OPT_llConnSetRejectIndExt(llConnState_t * connPtr, uint8 rejectOpcode, uint8 errorCode);
void OPT_LL_GetConnTxUsageParams(llTxUsageParams_t* pConnTxParams);

#endif /* CTRL_CONNECTABLE_H_ */
