/******************************************************************************

 @file  ctrl_opt_adv_conn.c

 @brief Implementation of the feature wrapper functions that direct to the full
        feature implementations.
        Enabling the feature by linking the wrapper functions to the real
        implementations defined in the library.

        Usage:
        Include this file in the build only when the feature is enabled.
        Add to the project CTRL_ADV_CONN_CFG defines
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
#include "ti/ble/stack_util/lib_opt/ctrl_stub_adv_conn.h"

#if defined(CTRL_ADV_CONN_CFG)

#if !defined(CTRL_ADV_NCONN_CFG)
#error "One or more dependencies are missing! Please add them."
#endif

void OPT_llProcessPeripheralConnectionCreated(void)
{
    llProcessPeripheralConnectionCreated();
}

void OPT_llProcessPeripheralControlPacket(llConnState_t* connPtr, uint8_t* pPkt)
{
    llProcessPeripheralControlPacket(connPtr, pPkt);
}

uint8_t OPT_llCheckPeripheralTerminate(uint8_t connId)
{
    return llCheckPeripheralTerminate(connId);
}

RCL_Callback OPT_LL_GetRfPeripheralCallback(void)
{
    return LL_GetRfPeripheralCallback();
}

void OPT_llAlignToNextEvent(llConnState_t* connPtr)
{
    llAlignToNextEvent(connPtr);
}

uint8_t OPT_llLastCmdDoneEventHandleConnectRequest(void)
{
    return llLastCmdDoneEventHandleConnectRequest();
}

void OPT_llPeripheral_TaskEnd(void)
{
    llPeripheral_TaskEnd();
}

void OPT_llBuildCtrlPktPeri(llConnState_t* connPtr, uint8_t* pData, uint8_t ctrlPkt)
{
    llBuildCtrlPktPeri(connPtr, pData, ctrlPkt);
}

void OPT_llPostSetupCtrlPktPeri(llConnState_t* connPtr, uint8_t ctrlPkt)
{
    llPostSetupCtrlPktPeri(connPtr, ctrlPkt);
}

hciStatus_t OPT_hciCmdParserPeripheral(uint8_t* pData, uint16_t cmdOpCode)
{
    return hciCmdParserPeripheral(pData, cmdOpCode);
}

hciStatus_t OPT_hciCmdParserVendorSpecificPeripheral(uint8_t* pData, uint16_t cmdOpCode)
{
    return hciCmdParserVendorSpecificPeripheral(pData, cmdOpCode);
}

void OPT_llAdv_TaskConnect(void)
{
    llAdv_TaskConnect();
}

void OPT_llSetTaskPeripheral(uint8_t connId, void* nextConnCmd)
{
    llSetTaskPeripheral(connId, nextConnCmd);
}

uint8_t OPT_llCheckAdvEventType(advSet_t* pAdvSet)
{
    return llCheckAdvEventType(pAdvSet);
}

void OPT_llupdateAdvCmdForHDC(advSet_t* pAdvSet, aeLegacyRf_t* pRf)
{
    llupdateAdvCmdForHDC(pAdvSet, pRf);
}

void OPT_llSendRejectInd(llConnState_t* connPtr, uint8_t errorCode)
{
    llSendRejectInd(connPtr, errorCode);
}

void OPT_LL_setAdvConnDefualtValues(void)
{
    LL_setAdvConnDefualtValues();
}

void OPT_LE_SetExtAdvParamsAdvConn(aeSetParamCmd_t* pCmdParams, advSet_t* pAdvSet)
{
    LE_SetExtAdvParamsAdvConn(pCmdParams, pAdvSet);
}

llStatus_t OPT_LE_checkAdvConnStatus(advSet_t* pAdvSet, aeEnableCmd_t* pCmdParams)
{
    return LE_checkAdvConnStatus(pAdvSet, pCmdParams);
}

bool OPT_ll_AdvConnIsEnable(void)
{
    return ll_AdvConnIsEnable();
}

void OPT_llCmdAdvStartedEventHandle(void)
{
    llCmdAdvStartedEventHandle();
}

#endif /* defined(CTRL_ADV_CONN_CFG) */
