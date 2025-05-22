/******************************************************************************

 @file  ctrl_opt_ae.c

 @brief Implementation of the feature wrapper functions that direct to the full
        feature implementations.
        Enabling the feature by linking the wrapper functions to the real
        implementations defined in the library.

        Usage:
        Include this file in the build only when the feature is enabled.
        Add to the project USE_AE defines
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
#include "ti/ble/stack_util/lib_opt/ctrl_stub_ae.h"

#if defined(USE_AE)



llStatus_t OPT_llSetExtendedAdvParams(advSet_t* pAdvSet, aeSetParamCmd_t* pCmdParams)
{
    return llSetExtendedAdvParams(pAdvSet, pCmdParams);
}

uint8_t OPT_llParseExtAdvPduInfo(llAdvPDUInfo* pAdvInfo, RCL_Buffer_DataEntry* const pDataEntry)
{
    return llParseExtAdvPduInfo(pAdvInfo, pDataEntry);
}

llStatus_t OPT_llSetupExtAdv(advSet_t * pAdvSet)
{
    return llSetupExtAdv(pAdvSet);
}

llStatus_t OPT_llPostProcessExtendedAdv(advSet_t* pAdvSet)
{
    return llPostProcessExtendedAdv(pAdvSet);
}

llProcessEntryStatus OPT_llSetExtendedAdvReport(aeExtAdvRptEvt_t* extAdvRpt, llAdvPDUInfo* pAdvPDUInfo)
{
    return llSetExtendedAdvReport(extAdvRpt, pAdvPDUInfo);
}

llStatus_t OPT_llAddExtAdvPacketToTx(advSet_t* pAdvSet, uint8_t pktType, uint8_t payloadLen)
{
    return llAddExtAdvPacketToTx(pAdvSet, pktType, payloadLen);
}

llStatus_t OPT_llBuildExtAdvPacket(aePacket* pPkt, comExtPktFormat_t* comPkt, uint8_t pktType, uint8_t payloadLen, uint8_t peerAddrType, uint8_t ownAddrType)
{
    return llBuildExtAdvPacket(pPkt, comPkt, pktType, payloadLen, peerAddrType, ownAddrType);
}

llStatus_t OPT_llupdateAuxHdrPacket(advSet_t* pAdvSet)
{
    return llupdateAuxHdrPacket(pAdvSet);
}

uint32_t OPT_llExtAdvTxTime(advSet_t* pAdvSet, uint8_t primPhy, uint8_t secPhy)
{
    return llExtAdvTxTime(pAdvSet, primPhy, secPhy);
}

uint32_t OPT_llEstimateAuxOtaTime(advSet_t* pAdvSet, uint8_t secPhy)
{
    return llEstimateAuxOtaTime(pAdvSet, secPhy);
}

void OPT_llSetExtHdrFlags(aeSetDataCmd_t* pCmdParams)
{
    llSetExtHdrFlags(pCmdParams);
}

void OPT_LL_rclAdvTxFinished(void)
{
    LL_rclAdvTxFinished();
}

llStatus_t OPT_llValidateAEAdvDataLength(aeSetDataCmd_t* pCmdParams, advSet_t* pAdvSet)
{
    return llValidateAEAdvDataLength(pCmdParams, pAdvSet);
}

void OPT_llAdvProcessExtendedAdv(advSet_t* pAdvSet)
{
    llAdvProcessExtendedAdv(pAdvSet);
}

void OPT_llAEProcessAuxAdvInd(advSet_t* pAdvSet, uint8_t numActiveAdvSets)
{
    llAEProcessAuxAdvInd(pAdvSet, numActiveAdvSets);
}

void OPT_llupdateAuxAdvHeader(sortedAdv_t* pNextAdvSet)
{
    llupdateAuxAdvHeader(pNextAdvSet);
}

bool OPT_LL_AeEnable(void)
{
    return LL_AeEnable();
}

uint16_t OPT_llCalculateTotalExtAdvTime(sortedAdv_t* aeNode)
{
    return llCalculateTotalExtAdvTime(aeNode);
}

void OPT_llSetPeerAddress(uint8_t* advPkt, llConnState_t* connPtr)
{
    llSetPeerAddress(advPkt, connPtr);
}

void OPT_llSetupExtendedAdvData(advSet_t* pAdvSet)
{
    llSetupExtendedAdvData(pAdvSet);
}

void OPT_llResetExtScanStateMachine(uint8_t* scanState)
{
    llResetExtScanStateMachine(scanState);
}

llStatus_t OPT_llValidateAEAdvDataLen(aeSetDataCmd_t* pCmdParams)
{
    return llValidateAEAdvDataLen(pCmdParams);
}

llStatus_t OPT_llValidateScanRspDataForAE(aeEnableCmd_t* pCmdParams, advSet_t* pAdvSet)
{
    return llValidateScanRspDataForAE(pCmdParams, pAdvSet);
}

llStatus_t OPT_llSetupExtendedAdvertising(advSet_t* pAdvSet)
{
    return llSetupExtendedAdvertising(pAdvSet);
}

void OPT_llAESetFeatureBit(void)
{
    llAESetFeatureBit();
}

bool OPT_llAE_NextAdvIsExtended(void)
{
    return llAE_NextAdvIsExtended();
}

uint8 OPT_LL_AE_GetNextChannel(void)
{
    return LL_AE_GetNextChannel();
}

void OPT_LL_AE_GetTxUsageParams(llTxUsageParams_t* pExtAdvTxParams)
{
    LL_AE_GetTxUsageParams(pExtAdvTxParams);
}

#endif /* defined(USE_AE) */
