/******************************************************************************

 @file  ctrl_stub_ae.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_ae.c.

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

#ifndef CTRL_AE_H_
#define CTRL_AE_H_

#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll_ae.h"

// Function prototypes for the actual implementations
extern llStatus_t llSetExtendedAdvParams(advSet_t* pAdvSet, aeSetParamCmd_t* pCmdParams);
extern uint8_t llParseExtAdvPduInfo(llAdvPDUInfo* pAdvInfo, RCL_Buffer_DataEntry* const pDataEntry);
extern llStatus_t llSetupExtAdv(advSet_t * pAdvSet);
extern llStatus_t llPostProcessExtendedAdv(advSet_t* pAdvSet);
extern llProcessEntryStatus llSetExtendedAdvReport(aeExtAdvRptEvt_t* extAdvRpt, llAdvPDUInfo* pAdvPDUInfo);
extern llStatus_t llAddExtAdvPacketToTx(advSet_t* pAdvSet, uint8_t pktType, uint8_t payloadLen);
extern llStatus_t llBuildExtAdvPacket(aePacket* pPkt, comExtPktFormat_t* comPkt, uint8_t pktType, uint8_t payloadLen, uint8_t peerAddrType, uint8_t ownAddrType);
extern llStatus_t llupdateAuxHdrPacket(advSet_t* pAdvSet);
extern uint32_t llExtAdvTxTime(advSet_t* pAdvSet, uint8_t primPhy, uint8_t secPhy);
extern uint32_t llEstimateAuxOtaTime(advSet_t* pAdvSet, uint8_t secPhy);
extern void llSetExtHdrFlags(aeSetDataCmd_t* pCmdParams);
extern void LL_rclAdvTxFinished(void);
extern llStatus_t llValidateAEAdvDataLength(aeSetDataCmd_t* pCmdParams, advSet_t* pAdvSet);
extern void llAdvProcessExtendedAdv(advSet_t* pAdvSet);
extern void llAEProcessAuxAdvInd(advSet_t* pAdvSet, uint8_t numActiveAdvSets);
extern void llupdateAuxAdvHeader(sortedAdv_t* pNextAdvSet);
extern bool LL_AeEnable(void);
extern uint16_t llCalculateTotalExtAdvTime(sortedAdv_t* aeNode);
extern void llSetPeerAddress(uint8_t* advPkt, llConnState_t* connPtr);
extern void llSetupExtendedAdvData(advSet_t* pAdvSet);
extern void llResetExtScanStateMachine(uint8_t* scanState);
extern llStatus_t llValidateAEAdvDataLen(aeSetDataCmd_t* pCmdParams);
extern llStatus_t llValidateScanRspDataForAE(aeEnableCmd_t* pCmdParams, advSet_t* pAdvSet);
extern llStatus_t llSetupExtendedAdvertising(advSet_t* pAdvSet);
extern void llAESetFeatureBit(void);
extern bool llAE_NextAdvIsExtended(void);
extern uint8 LL_AE_GetNextChannel(void);
extern void LL_AE_GetTxUsageParams(llTxUsageParams_t* pExtAdvTxParams);

// Wrapper functions for the feature implementations
llStatus_t OPT_llSetExtendedAdvParams(advSet_t* pAdvSet, aeSetParamCmd_t* pCmdParams);
uint8_t OPT_llParseExtAdvPduInfo(llAdvPDUInfo* pAdvInfo, RCL_Buffer_DataEntry* const pDataEntry);
llStatus_t OPT_llSetupExtAdv(advSet_t * pAdvSet);
llStatus_t OPT_llPostProcessExtendedAdv(advSet_t* pAdvSet);
llProcessEntryStatus OPT_llSetExtendedAdvReport(aeExtAdvRptEvt_t* extAdvRpt, llAdvPDUInfo* pAdvPDUInfo);
llStatus_t OPT_llAddExtAdvPacketToTx(advSet_t* pAdvSet, uint8_t pktType, uint8_t payloadLen);
llStatus_t OPT_llBuildExtAdvPacket(aePacket* pPkt, comExtPktFormat_t* comPkt, uint8_t pktType, uint8_t payloadLen, uint8_t peerAddrType, uint8_t ownAddrType);
llStatus_t OPT_llupdateAuxHdrPacket(advSet_t* pAdvSet);
uint32_t OPT_llExtAdvTxTime(advSet_t* pAdvSet, uint8_t primPhy, uint8_t secPhy);
uint32_t OPT_llEstimateAuxOtaTime(advSet_t* pAdvSet, uint8_t secPhy);
void OPT_llSetExtHdrFlags(aeSetDataCmd_t* pCmdParams);
void OPT_LL_rclAdvTxFinished(void);
llStatus_t OPT_llValidateAEAdvDataLength(aeSetDataCmd_t* pCmdParams, advSet_t* pAdvSet);
void OPT_llAdvProcessExtendedAdv(advSet_t* pAdvSet);
void OPT_llAEProcessAuxAdvInd(advSet_t* pAdvSet, uint8_t numActiveAdvSets);
void OPT_llupdateAuxAdvHeader(sortedAdv_t* pNextAdvSet);
bool OPT_LL_AeEnable(void);
uint16_t OPT_llCalculateTotalExtAdvTime(sortedAdv_t* aeNode);
void OPT_llSetPeerAddress(uint8_t* advPkt, llConnState_t* connPtr);
void OPT_llSetupExtendedAdvData(advSet_t* pAdvSet);
void OPT_llResetExtScanStateMachine(uint8_t* scanState);
llStatus_t OPT_llValidateAEAdvDataLen(aeSetDataCmd_t* pCmdParams);
llStatus_t OPT_llValidateScanRspDataForAE(aeEnableCmd_t* pCmdParams, advSet_t* pAdvSet);
llStatus_t OPT_llSetupExtendedAdvertising(advSet_t* pAdvSet);
void OPT_llAESetFeatureBit(void);
bool OPT_llAE_NextAdvIsExtended(void);
uint8 OPT_LL_AE_GetNextChannel(void);
void OPT_LL_AE_GetTxUsageParams(llTxUsageParams_t* pExtAdvTxParams);

#endif /* CTRL_AE_H_ */
