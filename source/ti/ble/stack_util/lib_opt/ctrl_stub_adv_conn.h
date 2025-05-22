/******************************************************************************

 @file  ctrl_stub_adv_conn.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_adv_conn.c.

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

#ifndef CTRL_ADV_CONN_H_
#define CTRL_ADV_CONN_H_

#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll_ble.h"
#include "ti/ble/controller/ll/ll_ae.h"
#include "ti/ble/controller/hci/hci.h"

// Function prototypes for the actual implementations
extern void llProcessPeripheralConnectionCreated(void);
extern void llProcessPeripheralControlPacket(llConnState_t* connPtr, uint8_t* pPkt);
extern uint8_t llCheckPeripheralTerminate(uint8_t connId);
extern RCL_Callback LL_GetRfPeripheralCallback(void);
extern void llAlignToNextEvent(llConnState_t* connPtr);
extern uint8_t llLastCmdDoneEventHandleConnectRequest(void);
extern void llPeripheral_TaskEnd(void);
extern void llBuildCtrlPktPeri(llConnState_t* connPtr, uint8_t* pData, uint8_t ctrlPkt);
extern void llPostSetupCtrlPktPeri(llConnState_t* connPtr, uint8_t ctrlPkt);
extern hciStatus_t hciCmdParserPeripheral(uint8_t* pData, uint16_t cmdOpCode);
extern hciStatus_t hciCmdParserVendorSpecificPeripheral(uint8_t* pData, uint16_t cmdOpCode);
extern void llAdv_TaskConnect(void);
extern void llSetTaskPeripheral(uint8_t connId, void* nextConnCmd);
extern uint8_t llCheckAdvEventType(advSet_t* pAdvSet);
extern void llupdateAdvCmdForHDC(advSet_t* pAdvSet, aeLegacyRf_t* pRf);
extern void llSendRejectInd(llConnState_t* connPtr, uint8_t errorCode);
extern void LL_setAdvConnDefualtValues(void);
extern void LE_SetExtAdvParamsAdvConn(aeSetParamCmd_t* pCmdParams, advSet_t* pAdvSet);
extern llStatus_t LE_checkAdvConnStatus(advSet_t* pAdvSet, aeEnableCmd_t* pCmdParams);
extern bool ll_AdvConnIsEnable(void);
extern void llCmdAdvStartedEventHandle(void);

// Wrapper functions for the feature implementations
void OPT_llProcessPeripheralConnectionCreated(void);
void OPT_llProcessPeripheralControlPacket(llConnState_t* connPtr, uint8_t* pPkt);
uint8_t OPT_llCheckPeripheralTerminate(uint8_t connId);
RCL_Callback OPT_LL_GetRfPeripheralCallback(void);
void OPT_llAlignToNextEvent(llConnState_t* connPtr);
uint8_t OPT_llLastCmdDoneEventHandleConnectRequest(void);
void OPT_llPeripheral_TaskEnd(void);
void OPT_llBuildCtrlPktPeri(llConnState_t* connPtr, uint8_t* pData, uint8_t ctrlPkt);
void OPT_llPostSetupCtrlPktPeri(llConnState_t* connPtr, uint8_t ctrlPkt);
hciStatus_t OPT_hciCmdParserPeripheral(uint8_t* pData, uint16_t cmdOpCode);
hciStatus_t OPT_hciCmdParserVendorSpecificPeripheral(uint8_t* pData, uint16_t cmdOpCode);
void OPT_llAdv_TaskConnect(void);
void OPT_llSetTaskPeripheral(uint8_t connId, void* nextConnCmd);
uint8_t OPT_llCheckAdvEventType(advSet_t* pAdvSet);
void OPT_llupdateAdvCmdForHDC(advSet_t* pAdvSet, aeLegacyRf_t* pRf);
void OPT_llSendRejectInd(llConnState_t* connPtr, uint8_t errorCode);
void OPT_LL_setAdvConnDefualtValues(void);
void OPT_LE_SetExtAdvParamsAdvConn(aeSetParamCmd_t* pCmdParams, advSet_t* pAdvSet);
llStatus_t OPT_LE_checkAdvConnStatus(advSet_t* pAdvSet, aeEnableCmd_t* pCmdParams);
bool OPT_ll_AdvConnIsEnable(void);
void OPT_llCmdAdvStartedEventHandle(void);

#endif /* CTRL_ADV_CONN_H_ */
