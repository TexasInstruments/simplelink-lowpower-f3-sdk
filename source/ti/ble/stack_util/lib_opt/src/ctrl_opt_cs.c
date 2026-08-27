/******************************************************************************

 @file  ctrl_opt_cs.c

 @brief Implementation of the feature wrapper functions that direct to the full
        feature implementations.
        Enabling the feature by linking the wrapper functions to the real
        implementations defined in the library.

        Usage:
        Include this file in the build only when the feature is enabled.
        Add to the project CHANNEL_SOUNDING defines
        The real functions must be implemented in the linked library.

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

#include "ti/ble/stack_util/lib_opt/opt_dependencies.h"
#include "ti/ble/stack_util/lib_opt/ctrl_stub_cs.h"

#if defined(CHANNEL_SOUNDING)



csStatus_e OPT_LL_CS_ReadLocalSupportedCapabilites(llCsCapabilities_t* pLocalCapabilities)
{
    return LL_CS_ReadLocalSupportedCapabilites(pLocalCapabilities);
}

csStatus_e OPT_LL_CS_ReadRemoteSupportedCapabilities(uint16_t connId)
{
    return LL_CS_ReadRemoteSupportedCapabilities(connId);
}

csStatus_e OPT_LL_CS_WriteCachedRemoteSupportedCapabilities(uint16_t connId, llCsCapabilities_t* pPeerCapabilitiesRaw)
{
    return LL_CS_WriteCachedRemoteSupportedCapabilities(connId, pPeerCapabilitiesRaw);
}

csStatus_e OPT_LL_CS_CreateConfig(uint16_t connId, const csConfigurationSet_t* pConfig, uint8_t createContext)
{
    return LL_CS_CreateConfig(connId, pConfig, createContext);
}

csStatus_e OPT_LL_CS_RemoveConfig(uint16_t connId, uint8_t configId)
{
    return LL_CS_RemoveConfig(connId, configId);
}

csStatus_e OPT_LL_CS_SecurityEnable(uint16_t connId)
{
    return LL_CS_SecurityEnable(connId);
}

csStatus_e OPT_LL_CS_SetDefaultSettings(uint16_t connId, csDefaultSettings_t* defaultSettings)
{
    return LL_CS_SetDefaultSettings(connId, defaultSettings);
}

csStatus_e OPT_LL_CS_ReadLocalFAETable(csFaeTbl_t* pFaeTable)
{
    return LL_CS_ReadLocalFAETable(pFaeTable);
}

csStatus_e OPT_LL_CS_ReadRemoteFAETable(uint16_t connId)
{
    return LL_CS_ReadRemoteFAETable(connId);
}

csStatus_e OPT_LL_CS_WriteCachedRemoteFAETable(uint16_t connId, int8* pFaeTbl)
{
    return LL_CS_WriteCachedRemoteFAETable(connId, pFaeTbl);
}

csStatus_e OPT_LL_CS_SetChannelClassification(uint8_t* pChannelClassification)
{
    return LL_CS_SetChannelClassification(pChannelClassification);
}

csStatus_e OPT_LL_CS_GetRole(uint16_t connId, uint8_t configId, uint8_t* role)
{
    return LL_CS_GetRole(connId, configId, role);
}

csStatus_e OPT_LL_CS_SetProcedureParameters(uint16_t connId, uint8_t configId, csProcedureParams_t* csProcParams)
{
    return LL_CS_SetProcedureParameters(connId, configId, csProcParams);
}

csStatus_e OPT_LL_CS_ProcedureEnable(uint16_t connId, uint8_t configId, uint8_t enable)
{
    return LL_CS_ProcedureEnable(connId, configId, enable);
}

csStatus_e OPT_llCsReceiveCsControlPacket(uint8_t ctrlType, llConnState_t* connPtr, uint8_t* pBuf)
{
    return llCsReceiveCsControlPacket(ctrlType, connPtr, pBuf);
}

uint8_t OPT_llCsTransmitCsCtrlProcedure(llConnState_t* connPtr, uint8_t ctrlPkt)
{
    return llCsTransmitCsCtrlProcedure(connPtr, ctrlPkt);
}

uint8_t OPT_llCsInitPrecal(void)
{
    return llCsInitPrecal();
}

void OPT_llCsReset(void)
{
    llCsReset();
}

bool OPT_llCsDbIsCsCtrlProcedureInProgress(uint16_t connId)
{
    return llCsDbIsCsCtrlProcedureInProgress(connId);
}

uint8_t OPT_llCsInit(void)
{
    return llCsInit();
}

void OPT_llCsClearConnProcedures(uint16_t connId)
{
    llCsClearConnProcedures(connId);
}

void OPT_llCsFreeAll(void)
{
    llCsFreeAll();
}

void OPT_llCsSetFeatureBit(void)
{
    llCsSetFeatureBit();
}

void OPT_llCsStartProcedure(llConnState_t* connPtr)
{
    llCsStartProcedure(connPtr);
}

void OPT_llCsSubevent_PostProcess(void)
{
    llCsSubevent_PostProcess();
}

void OPT_llCsSteps_PostProcess(void)
{
    llCsSteps_PostProcess();
}

void OPT_llCsCurrSubEventCont_PostProcess(void)
{
    llCsCurrSubEventCont_PostProcess();
}

void OPT_llCsNextSubEvent_PostProcess(void)
{
    llCsNextSubEvent_PostProcess();
}

void OPT_llCsResults_PostProcess(void)
{
    llCsResults_PostProcess();
}

void OPT_llCsProcedureError(void)
{
    llCsProcedureError();
}

RCL_Command* OPT_llScheduler_FindPrimStartType(const taskInfo_t* pNextConnTask, uint8_t* startType)
{
    return llScheduler_FindPrimStartType(pNextConnTask, startType);
}

uint32_t OPT_llScheduler_getSwitchTime(uint16_t taskID)
{
    return llScheduler_getSwitchTime(taskID);
}

uint8_t OPT_hciCmdParserChannelSounding(uint8_t* pData, uint16_t cmdOpCode)
{
    return hciCmdParserChannelSounding(pData, cmdOpCode);
}

RCL_Handle OPT_llScheduler_getHandle(uint16_t taskID)
{
    return llScheduler_getHandle(taskID);
}

void OPT_llCsPrecal_postProcess(void)
{
    llCsPrecal_postProcess();
}

csStatus_e OPT_LL_CS_Handover_CnParseCnData(uint16 connHandle, const uint8_t * pParams)
{
    return LL_CS_Handover_CnParseCnData(connHandle, pParams);
}

void OPT_LL_CS_Handover_SnPopulateSnData(uint16 connHandle, uint8_t * pParams)
{
    LL_CS_Handover_SnPopulateSnData(connHandle, pParams);
}

uint16 OPT_llConnGetMissCountMargin(void)
{
    return llConnGetMissCountMargin();
}

bool OPT_LL_CS_isCsInProgress(uint16_t connId)
{
    return LL_CS_isCsInProgress(connId);
}

bool OPT_llCsIsChannelClassificationAllowed(uint32_t currentTime)
{
    return llCsIsChannelClassificationAllowed(currentTime);
}

uint32_t OPT_LL_CS_Handover_SnGetSNDataSize(uint16 connHandle)
{
    return LL_CS_Handover_SnGetSNDataSize(connHandle);
}

void OPT_llCsPrecal_clear(void)
{
    llCsPrecal_clear();
}

uint8_t OPT_LL_CS_GetTswByACI(csACI_e ACI, uint8_t initTsw, uint8_t reflTsw)
{
    return LL_CS_GetTswByACI(ACI, initTsw, reflTsw);
}

const csConfigurationSet_t* OPT_llCsDbGetConfiguration(uint16 connId, uint8 configId)
{
    return llCsDbGetConfiguration(connId, configId);
}

#endif /* defined(CHANNEL_SOUNDING) */
