/******************************************************************************

 @file  ctrl_stub_cs.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_cs.c.

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

#ifndef CTRL_CS_H_
#define CTRL_CS_H_

#include "ti/ble/controller/ll/ll_cs_mgr.h"
#include "ti/ble/controller/ll/ll_cs_procedure.h"
#include "ti/ble/controller/ll/ll_cs_ctrl_pkt_mgr.h"
#include "ti/ble/controller/ll/ll_cs_rcl.h"

// Function prototypes for the actual implementations
extern csStatus_e LL_CS_ReadLocalSupportedCapabilites(llCsCapabilities_t* pLocalCapabilities);
extern csStatus_e LL_CS_ReadRemoteSupportedCapabilities(uint16_t connId);
extern csStatus_e LL_CS_CreateConfig(uint16_t connId, const csConfigurationSet_t* pConfig, uint8_t createContext);
extern csStatus_e LL_CS_RemoveConfig(uint16_t connId, uint8_t configId);
extern csStatus_e LL_CS_SecurityEnable(uint16_t connId);
extern csStatus_e LL_CS_SetDefaultSettings(uint16_t connId, csDefaultSettings_t* defaultSettings);
extern csStatus_e LL_CS_ReadLocalFAETable(csFaeTbl_t* pFaeTable);
extern csStatus_e LL_CS_ReadRemoteFAETable(uint16_t connId);
extern csStatus_e LL_CS_WriteRemoteFAETable(uint16_t connId, int8* pFaeTbl);
extern csStatus_e LL_CS_SetChannelClassification(uint8_t* pChannelClassification);
extern csStatus_e LL_CS_GetRole(uint16_t connId, uint8_t configId, uint8_t* role);
extern csStatus_e LL_CS_SetProcedureParameters(uint16_t connId, uint8_t configId, csProcedureParams_t* csProcParams);
extern csStatus_e LL_CS_ProcedureEnable(uint16_t connId, uint8_t configId, uint8_t enable);
extern void HCI_CS_SubeventResultContinueCback(void* hdr, const void* data, uint16_t dataLength);
extern csStatus_e llCsProcessCsControlPacket(uint8_t ctrlType, llConnState_t* connPtr, uint8_t* pBuf);
extern uint8_t llCsProcessCsCtrlProcedures(llConnState_t* connPtr, uint8_t ctrlPkt);
extern uint8_t llCsInit(void);
extern uint8_t llCsDbIsCsCtrlProcedureInProgress(uint16_t connId);
extern uint8_t llCsDbGetProcedureDoneStatus(uint16_t connId);
extern uint8_t llCsInitDb(void);
extern void llCsClearConnProcedures(uint16_t connId);
extern void llCsFreeAll(void);
extern void llCsSetFeatureBit(void);
extern csStatus_e llCsStartProcedure(llConnState_t* connPtr);
extern uint8_t llCsStartStepListGen(uint16_t connId);
extern void llCsSubevent_PostProcess(void);
extern void llCsSteps_PostProcess(void);
extern void llCsResults_PostProcess(void);
extern void llCsError_PostProcess(void);
extern RCL_Command* llScheduler_FindPrimStartType(const taskInfo_t* pNextConnTask, uint8_t* startType);
extern uint32_t llScheduler_getSwitchTime(uint16_t taskID);
extern uint8_t hciCmdParserChannelSounding(uint8_t* pData, uint16_t cmdOpCode);
extern RCL_Handle llScheduler_getHandle(uint16_t taskID);
extern void llCsRcl_handleCsSubmitError(uint16_t taskID, RCL_Command * cmd);
extern void llCsPrecal_postProcess(void);
extern csStatus_e LL_CS_Handover_CnParseCnData(uint16 connHandle, const uint8_t * pParams);
extern void LL_CS_Handover_SnPopulateSnData(uint16 connHandle, uint8_t * pParams);

// Wrapper functions for the feature implementations
csStatus_e OPT_LL_CS_ReadLocalSupportedCapabilites(llCsCapabilities_t* pLocalCapabilities);
csStatus_e OPT_LL_CS_ReadRemoteSupportedCapabilities(uint16_t connId);
csStatus_e OPT_LL_CS_CreateConfig(uint16_t connId, const csConfigurationSet_t* pConfig, uint8_t createContext);
csStatus_e OPT_LL_CS_RemoveConfig(uint16_t connId, uint8_t configId);
csStatus_e OPT_LL_CS_SecurityEnable(uint16_t connId);
csStatus_e OPT_LL_CS_SetDefaultSettings(uint16_t connId, csDefaultSettings_t* defaultSettings);
csStatus_e OPT_LL_CS_ReadLocalFAETable(csFaeTbl_t* pFaeTable);
csStatus_e OPT_LL_CS_ReadRemoteFAETable(uint16_t connId);
csStatus_e OPT_LL_CS_WriteRemoteFAETable(uint16_t connId, int8* pFaeTbl);
csStatus_e OPT_LL_CS_SetChannelClassification(uint8_t* pChannelClassification);
csStatus_e OPT_LL_CS_GetRole(uint16_t connId, uint8_t configId, uint8_t* role);
csStatus_e OPT_LL_CS_SetProcedureParameters(uint16_t connId, uint8_t configId, csProcedureParams_t* csProcParams);
csStatus_e OPT_LL_CS_ProcedureEnable(uint16_t connId, uint8_t configId, uint8_t enable);
void OPT_HCI_CS_SubeventResultContinueCback(void* hdr, const void* data, uint16_t dataLength);
csStatus_e OPT_llCsProcessCsControlPacket(uint8_t ctrlType, llConnState_t* connPtr, uint8_t* pBuf);
uint8_t OPT_llCsProcessCsCtrlProcedures(llConnState_t* connPtr, uint8_t ctrlPkt);
uint8_t OPT_llCsInit(void);
uint8_t OPT_llCsDbIsCsCtrlProcedureInProgress(uint16_t connId);
uint8_t OPT_llCsDbGetProcedureDoneStatus(uint16_t connId);
uint8_t OPT_llCsInitDb(void);
void OPT_llCsClearConnProcedures(uint16_t connId);
void OPT_llCsFreeAll(void);
void OPT_llCsSetFeatureBit(void);
csStatus_e OPT_llCsStartProcedure(llConnState_t* connPtr);
uint8_t OPT_llCsStartStepListGen(uint16_t connId);
void OPT_llCsSubevent_PostProcess(void);
void OPT_llCsSteps_PostProcess(void);
void OPT_llCsResults_PostProcess(void);
void OPT_llCsError_PostProcess(void);
RCL_Command* OPT_llScheduler_FindPrimStartType(const taskInfo_t* pNextConnTask, uint8_t* startType);
uint32_t OPT_llScheduler_getSwitchTime(uint16_t taskID);
uint8_t OPT_hciCmdParserChannelSounding(uint8_t* pData, uint16_t cmdOpCode);
RCL_Handle OPT_llScheduler_getHandle(uint16_t taskID);
void OPT_llCsRcl_handleCsSubmitError(uint16_t taskID, RCL_Command * cmd);
void OPT_llCsPrecal_postProcess(void);
csStatus_e OPT_LL_CS_Handover_CnParseCnData(uint16 connHandle, const uint8_t * pParams);
void OPT_LL_CS_Handover_SnPopulateSnData(uint16 connHandle, uint8_t * pParams);

#endif /* CTRL_CS_H_ */
