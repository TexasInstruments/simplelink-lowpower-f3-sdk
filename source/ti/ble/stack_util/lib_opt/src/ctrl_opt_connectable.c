/******************************************************************************

 @file  ctrl_opt_connectable.c

 @brief Implementation of the feature wrapper functions that direct to the full
        feature implementations.
        Enabling the feature by linking the wrapper functions to the real
        implementations defined in the library.

        Usage:
        Include this file in the build only when the feature is enabled.
        Add to the project CTRL_CONNECTABLE_CFG defines
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
#include "ti/ble/stack_util/lib_opt/ctrl_stub_connectable.h"

#if defined(CTRL_CONNECTABLE_CFG)

#if !defined(CTRL_ADV_CONN_CFG) && !defined(CTRL_INITIATOR_CFG)
#error "One or more dependencies are missing! Please add them."
#endif

llStatus_t OPT_LL_ConnActive(uint16_t connId)
{
    return LL_ConnActive(connId);
}

uint8_t OPT_llGetNextConn(void)
{
    return llGetNextConn();
}

llConnState_t* OPT_llDataGetConnPtr(uint8_t connId)
{
    return llDataGetConnPtr(connId);
}

uint8 OPT_llConnExists(uint8_t* peerAddr, uint8_t peerAddrType)
{
    return llConnExists(peerAddr, peerAddrType);
}

hciStatus_t OPT_hciCmdParserConnection(uint8_t* pData, uint16_t cmdOpCode)
{
    return hciCmdParserConnection(pData, cmdOpCode);
}

hciStatus_t OPT_hciCmdParserVendorSpecificConnection(uint8_t* pData, uint16_t cmdOpCode)
{
    return hciCmdParserVendorSpecificConnection(pData, cmdOpCode);
}

uint8_t OPT_llRxEntryDoneEventHandleStateConnection(void)
{
    return llRxEntryDoneEventHandleStateConnection();
}

void OPT_llProcessTxData(void)
{
    llProcessTxData();
}

void OPT_LL_RxDataCompleteCback(uint16_t connHandle, uint8_t* pBuf, uint16_t len, uint8_t fragFlag, int8_t rssi)
{
    LL_RxDataCompleteCback(connHandle, pBuf, len, fragFlag, rssi);
}

uint8_t OPT_llCheckConnHealth(uint32_t* currentTime, volatile uint32_t* connTime)
{
    return llCheckConnHealth(currentTime, connTime);
}

llStatus_t OPT_llDynamicAlloc_connectable(void)
{
    return llDynamicAlloc_connectable();
}

void OPT_llDynamicFree_connectable(void)
{
    llDynamicFree_connectable();
}

void OPT_llChannelAlgo2SetFeatureBit(void)
{
    llChannelAlgo2SetFeatureBit();
}

void OPT_llSchedulerConnectable(taskInfo_t* curTask)
{
    llSchedulerConnectable(curTask);
}

uint8_t OPT_llSchedulerSecondaryTasksConn(taskInfo_t* nextSecTask, void* nextSecCmd)
{
    return llSchedulerSecondaryTasksConn(nextSecTask, nextSecCmd);
}

uint32_t OPT_llHandleConnDisconnectedImmed(uint32_t events)
{
    return llHandleConnDisconnectedImmed(events);
}

void OPT_LL_clearAllActiveConns(void)
{
    LL_clearAllActiveConns();
}

void OPT_LL_setComplPktsDefualtValues(void)
{
    LL_setComplPktsDefualtValues();
}

void OPT_LL_GetNumActiveConns(uint8_t* numActiveConns)
{
    LL_GetNumActiveConns(numActiveConns);
}

void OPT_HCI_TL_SendDataPkt(uint8_t* pMsg)
{
    HCI_TL_SendDataPkt(pMsg);
}

llStatus_t OPT_LL_RemoteConnParamReqReply(uint16_t connHandle, uint16_t connIntervalMin, uint16_t connIntervalMax, uint16_t connLatency, uint16_t connTimeout, uint16_t minLen, uint16_t maxLen)
{
    return LL_RemoteConnParamReqReply(connHandle, connIntervalMin, connIntervalMax, connLatency, connTimeout, minLen, maxLen);
}

void OPT_ll_eccInit(void)
{
    ll_eccInit();
}

void OPT_llsdaaEnableConnTXData(uint16_t currConnId)
{
    llsdaaEnableConnTXData(currConnId);
}

void OPT_llsdaaDisableConnTXData(uint16_t connId)
{
    llsdaaDisableConnTXData(connId);
}

void OPT_llConnSetRejectIndExt(llConnState_t * connPtr, uint8 rejectOpcode, uint8 errorCode)
{
    llConnSetRejectIndExt(connPtr, rejectOpcode, errorCode);
}

void OPT_LL_GetConnTxUsageParams(llTxUsageParams_t* pConnTxParams)
{
    LL_GetConnTxUsageParams(pConnTxParams);
}

#endif /* defined(CTRL_CONNECTABLE_CFG) */
