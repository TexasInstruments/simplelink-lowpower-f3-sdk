/******************************************************************************

 @file  ctrl_stub_pawr_scan.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_pawr_scan.c.

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

#ifndef CTRL_PAWR_SCAN_H_
#define CTRL_PAWR_SCAN_H_

#include "ti/ble/stack_util/comdef.h"
#include "ti/ble/controller/hci/hci.h"
#include "ti/ble/controller/ll/ll_scheduler.h"
#include "ti/ble/controller/ll/ll_ae.h"
#include "ti/ble/controller/ll/ll_padv_scan.h"
#include "ti/ble/controller/ll/ll_pawr_scan.h"
#include "ti/ble/controller/ll/ll.h"

// Function prototypes for the actual implementations
extern uint8_t LE_SetPeriodicSyncSubevent(uint8_t* pPAwRParams, uint16_t perAdvProps, uint8_t numSubevents, uint8_t* subEvents);
extern uint8_t LE_SetPeriodicAdvResponseData(uint16_t eventCounter, void* chanMap, uint32_t accessAddress, uint16_t interval, uint8_t* pPAwRParams, uint8_t* pRawRspParams, RCL_CmdBle5PeriodicScanner* rfCmd);
extern void LL_PAwRS_SetFeatureBit(void);
extern void LL_PAwR_PostProcessRspData(void* chanMapCurrent, uint32_t accessAddress, uint32_t absStartTime, uint16_t eventInterval, uint16_t eventCounter, uint8_t* pPAwRParams);
extern llStatus_t LL_PAwRS_Terminate(uint8_t** pPAwRParams);
extern uint8_t LL_PAwRS_GetSubeventNum(uint8_t* pPAwRParams);
extern bool LL_PAwRS_IsEnable(void);
extern bool LL_PAwRS_IsPAwR(uint8_t* paramsSet);
extern uint8_t LL_PAwRS_InitSet(uint8_t** pPAwRParams, uint8_t* pPAwRSyncInfoData, uint8_t* crcInit, RCL_CtxPeriodicScanner* rfParam, uint8_t advAddrType, uint8_t* advAddr, uint8_t phy);
extern bool LL_PAwRS_IsACADPAwR(uint8_t* pACAD);
extern void LL_PAwRS_Activate(uint8_t* pPAwRParams);
extern void LL_PAwRS_PostProcess(const llPAwRPostProcessIn_t* in, llPAwRPostProcessOut_t* out, uint8_t* pPAwRParams);
extern uint8_t LL_PAwRS_GetSyncParams(uint8_t* PAwRParams, llPAwRSyncInfo_t* syncParams);
extern uint32_t LL_PAWRS_GetRspPktTxCmd(uint8_t* PAwRParams);
extern bool LL_PAwRS_ChooseRspPktOrSubeventSync(uint32_t selectedPadvSStartTime, uint8_t* PAwRParams);
extern void LL_PAwRS_AdjustNextStartTime(uint32_t* absStartTime, uint8_t* eventCounter, uint8_t* pPAwRParams, uint16_t eventInterval, uint8_t advSca);
extern void LL_PAwRS_UpdateRspDataCmdIfNeeded(uint8_t* pPAwRParams, uint16_t eventCounter, void* chanMapCurrent, uint32_t accessAddress, uint32_t absStartTime, uint16_t eventInterval);
extern bool LL_PAwRS_IsConnRspPkt(RCL_CtxPeriodicScanner* rfParam);
extern uint8_t LL_PAwRS_BuildConnRspPkt(RCL_CtxPeriodicScanner* rfParam, void* pPAwRParams, uint8_t advAddrType, uint8_t* advAddr);


// Wrapper functions for the feature implementations
uint8_t OPT_LE_SetPeriodicSyncSubevent(uint8_t* pPAwRParams, uint16_t perAdvProps, uint8_t numSubevents, uint8_t* subEvents);
uint8_t OPT_LE_SetPeriodicAdvResponseData(uint16_t eventCounter, void* chanMap, uint32_t accessAddress, uint16_t interval, uint8_t* pPAwRParams, uint8_t* pRawRspParams, RCL_CmdBle5PeriodicScanner* rfCmd);
void OPT_LL_PAwRS_SetFeatureBit(void);
void OPT_LL_PAwR_PostProcessRspData(void* chanMapCurrent, uint32_t accessAddress, uint32_t absStartTime, uint16_t eventInterval, uint16_t eventCounter, uint8_t* pPAwRParams);
llStatus_t OPT_LL_PAwRS_Terminate(uint8_t** pPAwRParams);
uint8_t OPT_LL_PAwRS_GetSubeventNum(uint8_t* pPAwRParams);
bool OPT_LL_PAwRS_IsEnable(void);
bool OPT_LL_PAwRS_IsPAwR(uint8_t* paramsSet);
uint8_t OPT_LL_PAwRS_InitSet(uint8_t** pPAwRParams, uint8_t* pPAwRSyncInfoData, uint8_t* crcInit, RCL_CtxPeriodicScanner* rfParam, uint8_t advAddrType, uint8_t* advAddr, uint8_t phy);
bool OPT_LL_PAwRS_IsACADPAwR(uint8_t* pACAD);
void OPT_LL_PAwRS_Activate(uint8_t* pPAwRParams);
void OPT_LL_PAwRS_PostProcess(const llPAwRPostProcessIn_t* in, llPAwRPostProcessOut_t* out, uint8_t* pPAwRParams);
uint8_t OPT_LL_PAwRS_GetSyncParams(uint8_t* PAwRParams, llPAwRSyncInfo_t* syncParams);
uint32_t OPT_LL_PAWRS_GetRspPktTxCmd(uint8_t* PAwRParams);
bool OPT_LL_PAwRS_ChooseRspPktOrSubeventSync(uint32_t selectedPadvSStartTime, uint8_t* PAwRParams);
void OPT_LL_PAwRS_AdjustNextStartTime(uint32_t* absStartTime, uint8_t* eventCounter, uint8_t* pPAwRParams, uint16_t eventInterval, uint8_t advSca);
void OPT_LL_PAwRS_UpdateRspDataCmdIfNeeded(uint8_t* pPAwRParams, uint16_t eventCounter, void* chanMapCurrent, uint32_t accessAddress, uint32_t absStartTime, uint16_t eventInterval);
bool OPT_LL_PAwRS_IsConnRspPkt(RCL_CtxPeriodicScanner* rfParam);
uint8_t OPT_LL_PAwRS_BuildConnRspPkt(RCL_CtxPeriodicScanner* rfParam, void* pPAwRParams, uint8_t advAddrType, uint8_t* advAddr);

#endif /* CTRL_PAWR_SCAN_H_ */
