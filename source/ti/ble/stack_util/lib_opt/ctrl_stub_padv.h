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

#ifndef CTRL_PADV_H_
#define CTRL_PADV_H_

#include "ti/ble/stack_util/comdef.h"
#include "ti/ble/controller/hci/hci.h"
#include "ti/ble/controller/ll/ll_scheduler.h"
#include "ti/ble/controller/ll/ll_ae.h"
#include "ti/ble/controller/ll/ll.h"

// Function prototypes for the actual implementations
extern void llSetTaskPeriodicAdv(void);
extern void llClearPeriodicAdvSets(void);
extern void llSetPeriodicAdvChmapUpdate(uint8_t set);
extern void * llFindNextPeriodicAdv(void);
extern llStatus_t llSetupPeriodicAdv(advSet_t * pAdvSet);
extern void llSetPeriodicSyncInfo(advSet_t * pAdvSet, uint8_t * pBuf);
extern llPeriodicAdvSet_t * llGetCurrentPeriodicAdv(void);
extern llPeriodicAdvSet_t * llGetPeriodicAdv(uint8_t handle);
extern taskInfo_t * llSelectTaskPeriodicAdv(uint8_t secTaskID, uint32_t timeGap);
extern hciStatus_t hciCmdParserPeriodicAdv(uint8_t * pData, uint16_t cmdOpCode);
extern llStatus_t LE_SetPeriodicAdvParams(uint8_t advHandle, uint16_t periodicAdvIntervalMin, uint16_t periodicAdvIntervalMax, uint16_t periodicAdvProp);
extern llStatus_t LE_SetPeriodicAdvData(uint8_t advHandle, uint8_t operation, uint8_t dataLength, uint8_t * data);
extern llStatus_t LE_SetPeriodicAdvEnable(uint8_t enable, uint8_t advHandle);
extern void llExtAdv_PostProcess_padv(advSet_t * pAdvSet);
extern void llPostProcessExtendedAdv_padv(advSet_t * pAdvSet);
extern uint8_t llCompareSecondaryPrimaryTasksQoSParam_padv(llConnState_t * primConnPtr);
extern uint8_t llGetCurrentPeriodicAdvPriority(uint8_t curllStatePriority);
extern uint32_t llGetCurrentPeriodicAdvTotalOtaTime(uint32_t curTotalOtaTime);
extern llStatus_t LE_SetExtAdvParams_padv(aeSetParamCmd_t * pCmdParams);
extern void LE_SetExtAdvEnable_padv(advSet_t * pAdvSet);
extern void LL_EXT_SetQOSParameters_padv(uint32_t paramVal, uint16_t taskHandle);
extern uint32_t llHandlePeriodicAdvEvents(uint32_t events);
extern bool LL_PeriodicAdvIsEnable(void);
extern advSet_t* LL_AE_GetNextAdvSet(void);
extern void LL_GetPeriodicTxUsageParams(llTxUsageParams_t* pPeriodicTxParams);

// Wrapper functions for the feature implementations
void OPT_llSetTaskPeriodicAdv(void);
void OPT_llClearPeriodicAdvSets(void);
void OPT_llSetPeriodicAdvChmapUpdate(uint8_t set);
void * OPT_llFindNextPeriodicAdv(void);
llStatus_t OPT_llSetupPeriodicAdv(advSet_t * pAdvSet);
void OPT_llSetPeriodicSyncInfo(advSet_t * pAdvSet, uint8_t * pBuf);
llPeriodicAdvSet_t * OPT_llGetCurrentPeriodicAdv(void);
llPeriodicAdvSet_t * OPT_llGetPeriodicAdv(uint8_t handle);
taskInfo_t * OPT_llSelectTaskPeriodicAdv(uint8_t secTaskID, uint32_t timeGap);
hciStatus_t OPT_hciCmdParserPeriodicAdv(uint8_t * pData, uint16_t cmdOpCode);
llStatus_t OPT_LE_SetPeriodicAdvParams(uint8_t advHandle, uint16_t periodicAdvIntervalMin, uint16_t periodicAdvIntervalMax, uint16_t periodicAdvProp);
llStatus_t OPT_LE_SetPeriodicAdvData(uint8_t advHandle, uint8_t operation, uint8_t dataLength, uint8_t * data);
llStatus_t OPT_LE_SetPeriodicAdvEnable(uint8_t enable, uint8_t advHandle);
void OPT_llExtAdv_PostProcess_padv(advSet_t * pAdvSet);
void OPT_llPostProcessExtendedAdv_padv(advSet_t * pAdvSet);
uint8_t OPT_llCompareSecondaryPrimaryTasksQoSParam_padv(llConnState_t * primConnPtr);
uint8_t OPT_llGetCurrentPeriodicAdvPriority(uint8_t curllStatePriority);
uint32_t OPT_llGetCurrentPeriodicAdvTotalOtaTime(uint32_t curTotalOtaTime);
llStatus_t OPT_LE_SetExtAdvParams_padv(aeSetParamCmd_t * pCmdParams);
void OPT_LE_SetExtAdvEnable_padv(advSet_t * pAdvSet);
void OPT_LL_EXT_SetQOSParameters_padv(uint32_t paramVal, uint16_t taskHandle);
uint32_t OPT_llHandlePeriodicAdvEvents(uint32_t events);
bool OPT_LL_PeriodicAdvIsEnable(void);
advSet_t* OPT_LL_AE_GetNextAdvSet(void);
void OPT_LL_GetPeriodicTxUsageParams(llTxUsageParams_t* pPeriodicTxParams);

#endif /* CTRL_PADV_H_ */
