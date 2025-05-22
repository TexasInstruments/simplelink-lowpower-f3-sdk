/******************************************************************************

 @file  ctrl_opt_padv.c

 @brief Implementation of the feature wrapper functions that direct to the full
        feature implementations.
        Enabling the feature by linking the wrapper functions to the real
        implementations defined in the library.

        Usage:
        Include this file in the build only when the feature is enabled.
        Add to the project USE_PERIODIC_ADV defines
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
#include "ti/ble/stack_util/lib_opt/ctrl_stub_padv.h"

#if defined(USE_PERIODIC_ADV)

#if !defined(USE_AE)
#error "One or more dependencies are missing! Please add them."
#endif

void OPT_llSetTaskPeriodicAdv(void)
{
    llSetTaskPeriodicAdv();
}

void OPT_llClearPeriodicAdvSets(void)
{
    llClearPeriodicAdvSets();
}

void OPT_llSetPeriodicAdvChmapUpdate(uint8_t set)
{
    llSetPeriodicAdvChmapUpdate(set);
}

void * OPT_llFindNextPeriodicAdv(void)
{
    return llFindNextPeriodicAdv();
}

llStatus_t OPT_llSetupPeriodicAdv(advSet_t * pAdvSet)
{
    return llSetupPeriodicAdv(pAdvSet);
}

void OPT_llSetPeriodicSyncInfo(advSet_t * pAdvSet, uint8_t * pBuf)
{
    llSetPeriodicSyncInfo(pAdvSet, pBuf);
}

llPeriodicAdvSet_t * OPT_llGetCurrentPeriodicAdv(void)
{
    return llGetCurrentPeriodicAdv();
}

llPeriodicAdvSet_t * OPT_llGetPeriodicAdv(uint8_t handle)
{
    return llGetPeriodicAdv(handle);
}

taskInfo_t * OPT_llSelectTaskPeriodicAdv(uint8_t secTaskID, uint32_t timeGap)
{
    return llSelectTaskPeriodicAdv(secTaskID, timeGap);
}

hciStatus_t OPT_hciCmdParserPeriodicAdv(uint8_t * pData, uint16_t cmdOpCode)
{
    return hciCmdParserPeriodicAdv(pData, cmdOpCode);
}

llStatus_t OPT_LE_SetPeriodicAdvParams(uint8_t advHandle, uint16_t periodicAdvIntervalMin, uint16_t periodicAdvIntervalMax, uint16_t periodicAdvProp)
{
    return LE_SetPeriodicAdvParams(advHandle, periodicAdvIntervalMin, periodicAdvIntervalMax, periodicAdvProp);
}

llStatus_t OPT_LE_SetPeriodicAdvData(uint8_t advHandle, uint8_t operation, uint8_t dataLength, uint8_t * data)
{
    return LE_SetPeriodicAdvData(advHandle, operation, dataLength, data);
}

llStatus_t OPT_LE_SetPeriodicAdvEnable(uint8_t enable, uint8_t advHandle)
{
    return LE_SetPeriodicAdvEnable(enable, advHandle);
}

void OPT_llExtAdv_PostProcess_padv(advSet_t * pAdvSet)
{
    llExtAdv_PostProcess_padv(pAdvSet);
}

void OPT_llPostProcessExtendedAdv_padv(advSet_t * pAdvSet)
{
    llPostProcessExtendedAdv_padv(pAdvSet);
}

uint8_t OPT_llCompareSecondaryPrimaryTasksQoSParam_padv(llConnState_t * primConnPtr)
{
    return llCompareSecondaryPrimaryTasksQoSParam_padv(primConnPtr);
}

uint8_t OPT_llGetCurrentPeriodicAdvPriority(uint8_t curllStatePriority)
{
    return llGetCurrentPeriodicAdvPriority(curllStatePriority);
}

uint32_t OPT_llGetCurrentPeriodicAdvTotalOtaTime(uint32_t curTotalOtaTime)
{
    return llGetCurrentPeriodicAdvTotalOtaTime(curTotalOtaTime);
}

llStatus_t OPT_LE_SetExtAdvParams_padv(aeSetParamCmd_t * pCmdParams)
{
    return LE_SetExtAdvParams_padv(pCmdParams);
}

void OPT_LE_SetExtAdvEnable_padv(advSet_t * pAdvSet)
{
    LE_SetExtAdvEnable_padv(pAdvSet);
}

void OPT_LL_EXT_SetQOSParameters_padv(uint32_t paramVal, uint16_t taskHandle)
{
    LL_EXT_SetQOSParameters_padv(paramVal, taskHandle);
}

uint32_t OPT_llHandlePeriodicAdvEvents(uint32_t events)
{
    return llHandlePeriodicAdvEvents(events);
}

bool OPT_LL_PeriodicAdvIsEnable(void)
{
    return LL_PeriodicAdvIsEnable();
}

advSet_t* OPT_LL_AE_GetNextAdvSet(void)
{
    return LL_AE_GetNextAdvSet();
}

void OPT_LL_GetPeriodicTxUsageParams(llTxUsageParams_t* pPeriodicTxParams)
{
    LL_GetPeriodicTxUsageParams(pPeriodicTxParams);
}

#endif /* defined(USE_PERIODIC_ADV) */
