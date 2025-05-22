/******************************************************************************

 @file  ctrl_opt_adv_nconn.c

 @brief Implementation of the feature wrapper functions that direct to the full
        feature implementations.
        Enabling the feature by linking the wrapper functions to the real
        implementations defined in the library.

        Usage:
        Include this file in the build only when the feature is enabled.
        Add to the project CTRL_ADV_NCONN_CFG defines
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
#include "ti/ble/stack_util/lib_opt/ctrl_stub_adv_nconn.h"

#if defined(CTRL_ADV_NCONN_CFG)



void OPT_llProcessAdvAddrResolutionTimeout(void)
{
    llProcessAdvAddrResolutionTimeout();
}

void OPT_llSetTaskAdv(uint8_t startType, void* nextSecCmd)
{
    llSetTaskAdv(startType, nextSecCmd);
}

taskInfo_t* OPT_llSelectTaskAdv(uint8_t secTaskID, uint32_t timeGap)
{
    return llSelectTaskAdv(secTaskID, timeGap);
}

llStatus_t OPT_LL_ClearAdvSets(void)
{
    return LL_ClearAdvSets();
}

uint8_t OPT_LL_CountAdvSets(uint8_t type)
{
    return LL_CountAdvSets(type);
}

advSet_t* OPT_LL_GetAdvSet(uint8_t handle, uint8_t allocFlag)
{
    return LL_GetAdvSet(handle, allocFlag);
}

llStatus_t OPT_LL_SetSecAdvChanMap(uint8_t* chanMap)
{
    return LL_SetSecAdvChanMap(chanMap);
}

void OPT_llExtAdv_PostProcess(void)
{
    llExtAdv_PostProcess();
}

hciStatus_t OPT_hciCmdParserAdvertiser(uint8_t* pData, uint16_t cmdOpCode)
{
    return hciCmdParserAdvertiser(pData, cmdOpCode);
}

hciStatus_t OPT_hciCmdParserVendorSpecificBroadcaster(uint8_t* pData, uint16_t cmdOpCode)
{
    return hciCmdParserVendorSpecificBroadcaster(pData, cmdOpCode);
}

void OPT_LL_rclAdvRxEntryDone(void)
{
    LL_rclAdvRxEntryDone();
}

uint8_t OPT_LL_IsAdvertizeWithRandomAddress(void)
{
    return LL_IsAdvertizeWithRandomAddress();
}

void OPT_LL_UpdateAdvSCAValue(uint16_t scaInPPM)
{
    LL_UpdateAdvSCAValue(scaInPPM);
}

uint8_t OPT_llCheckAdvEvtType(uint8_t advEvtType)
{
    return llCheckAdvEvtType(advEvtType);
}

uint8_t OPT_llHandoverDisableAdv(uint8_t advHandle)
{
    return llHandoverDisableAdv(advHandle);
}

uint8_t OPT_llCheckAdvHealth(uint32_t* currentTime, volatile uint32_t* advTime)
{
    return llCheckAdvHealth(currentTime, advTime);
}

void* OPT_llFindNextAdvSet(void)
{
    return llFindNextAdvSet();
}

void OPT_LL_DisableAdvSets(void)
{
    LL_DisableAdvSets();
}

cmErrorCodes_e OPT_llCmDisableCurAdv(void)
{
    return llCmDisableCurAdv();
}

uint8_t OPT_llCompareSecondaryPrimaryTasksQoSParam_adv(llConnState_t* primConnPtr)
{
    return llCompareSecondaryPrimaryTasksQoSParam_adv(primConnPtr);
}

void OPT_llSelectTasksetTimeGapForAdv(uint32_t* timeGap)
{
    llSelectTasksetTimeGapForAdv(timeGap);
}

void OPT_llFreeTaskAdv(void)
{
    llFreeTaskAdv();
}

void OPT_llCalculateAdvTimeGap(uint32_t* timeGap)
{
    llCalculateAdvTimeGap(timeGap);
}

void OPT_hci_tl_ClearAdvSet(void)
{
    hci_tl_ClearAdvSet();
}

uint8_t OPT_processAdvExtraHCICmd(hciPacket_t* pMsg)
{
    return processAdvExtraHCICmd(pMsg);
}

#endif /* defined(CTRL_ADV_NCONN_CFG) */
