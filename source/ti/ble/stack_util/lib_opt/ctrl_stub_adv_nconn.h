/******************************************************************************

 @file  ctrl_stub_adv_nconn.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_adv_nconn.c.

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

#ifndef CTRL_ADV_NCONN_H_
#define CTRL_ADV_NCONN_H_

#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll_ble.h"
#include "ti/ble/controller/ll/ll_ae.h"
#include "ti/ble/controller/hci/hci.h"
#include "ti/ble/stack_util/connection_monitor_types.h"

// Function prototypes for the actual implementations
extern void llProcessAdvAddrResolutionTimeout(void);
extern void llSetTaskAdv(uint8_t startType, void* nextSecCmd);
extern taskInfo_t* llSelectTaskAdv(uint8_t secTaskID, uint32_t timeGap);
extern llStatus_t LL_ClearAdvSets(void);
extern uint8_t LL_CountAdvSets(uint8_t type);
extern advSet_t* LL_GetAdvSet(uint8_t handle, uint8_t allocFlag);
extern llStatus_t LL_SetSecAdvChanMap(uint8_t* chanMap);
extern void llExtAdv_PostProcess(void);
extern hciStatus_t hciCmdParserAdvertiser(uint8_t* pData, uint16_t cmdOpCode);
extern hciStatus_t hciCmdParserVendorSpecificBroadcaster(uint8_t* pData, uint16_t cmdOpCode);
extern void LL_rclAdvRxEntryDone(void);
extern uint8_t LL_IsAdvertizeWithRandomAddress(void);
extern void LL_UpdateAdvSCAValue(uint16_t scaInPPM);
extern uint8_t llCheckAdvEvtType(uint8_t advEvtType);
extern uint8_t llHandoverDisableAdv(uint8_t advHandle);
extern uint8_t llCheckAdvHealth(uint32_t* currentTime, volatile uint32_t* advTime);
extern void* llFindNextAdvSet(void);
extern void LL_DisableAdvSets(void);
extern cmErrorCodes_e llCmDisableCurAdv(void);
extern uint8_t llCompareSecondaryPrimaryTasksQoSParam_adv(llConnState_t* primConnPtr);
extern void llSelectTasksetTimeGapForAdv(uint32_t* timeGap);
extern void llFreeTaskAdv(void);
extern void llCalculateAdvTimeGap(uint32_t* timeGap);
extern void hci_tl_ClearAdvSet(void);
extern uint8_t processAdvExtraHCICmd(hciPacket_t* pMsg);

// Wrapper functions for the feature implementations
void OPT_llProcessAdvAddrResolutionTimeout(void);
void OPT_llSetTaskAdv(uint8_t startType, void* nextSecCmd);
taskInfo_t* OPT_llSelectTaskAdv(uint8_t secTaskID, uint32_t timeGap);
llStatus_t OPT_LL_ClearAdvSets(void);
uint8_t OPT_LL_CountAdvSets(uint8_t type);
advSet_t* OPT_LL_GetAdvSet(uint8_t handle, uint8_t allocFlag);
llStatus_t OPT_LL_SetSecAdvChanMap(uint8_t* chanMap);
void OPT_llExtAdv_PostProcess(void);
hciStatus_t OPT_hciCmdParserAdvertiser(uint8_t* pData, uint16_t cmdOpCode);
hciStatus_t OPT_hciCmdParserVendorSpecificBroadcaster(uint8_t* pData, uint16_t cmdOpCode);
void OPT_LL_rclAdvRxEntryDone(void);
uint8_t OPT_LL_IsAdvertizeWithRandomAddress(void);
void OPT_LL_UpdateAdvSCAValue(uint16_t scaInPPM);
uint8_t OPT_llCheckAdvEvtType(uint8_t advEvtType);
uint8_t OPT_llHandoverDisableAdv(uint8_t advHandle);
uint8_t OPT_llCheckAdvHealth(uint32_t* currentTime, volatile uint32_t* advTime);
void* OPT_llFindNextAdvSet(void);
void OPT_LL_DisableAdvSets(void);
cmErrorCodes_e OPT_llCmDisableCurAdv(void);
uint8_t OPT_llCompareSecondaryPrimaryTasksQoSParam_adv(llConnState_t* primConnPtr);
void OPT_llSelectTasksetTimeGapForAdv(uint32_t* timeGap);
void OPT_llFreeTaskAdv(void);
void OPT_llCalculateAdvTimeGap(uint32_t* timeGap);
void OPT_hci_tl_ClearAdvSet(void);
uint8_t OPT_processAdvExtraHCICmd(hciPacket_t* pMsg);

#endif /* CTRL_ADV_NCONN_H_ */
