/******************************************************************************

 @file  ctrl_opt_pawr_scan.c

 @brief Implementation of the feature wrapper functions that direct to the full
        feature implementations.
        Enabling the feature by linking the wrapper functions to the real
        implementations defined in the library.

        Usage:
        Include this file in the build only when the feature is enabled.
        Add to the project USE_PAWR_SCAN defines
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
#include "ti/ble/stack_util/lib_opt/ctrl_stub_pawr_scan.h"

#if defined(USE_PAWR_SCAN)

#if !defined(USE_PERIODIC_SCAN)
#error "One or more dependencies are missing! Please add them."
#endif

uint8_t OPT_LE_SetPeriodicSyncSubevent(uint8_t* pPAwRParams, uint16_t perAdvProps, uint8_t numSubevents, uint8_t* subEvents)
{
    return LE_SetPeriodicSyncSubevent(pPAwRParams, perAdvProps, numSubevents, subEvents);
}

uint8_t OPT_LE_SetPeriodicAdvResponseData(uint16_t eventCounter, void* chanMap, uint32_t accessAddress, uint16_t interval, uint8_t* pPAwRParams, uint8_t* pRawRspParams, RCL_CmdBle5PeriodicScanner* rfCmd)
{
    return LE_SetPeriodicAdvResponseData(eventCounter, chanMap, accessAddress, interval, pPAwRParams, pRawRspParams, rfCmd);
}

void OPT_LL_PAwRS_SetFeatureBit(void)
{
    LL_PAwRS_SetFeatureBit();
}

void OPT_LL_PAwR_PostProcessRspData(void* chanMapCurrent, uint32_t accessAddress, uint32_t absStartTime, uint16_t eventInterval, uint16_t eventCounter, uint8_t* pPAwRParams)
{
    LL_PAwR_PostProcessRspData(chanMapCurrent, accessAddress, absStartTime, eventInterval, eventCounter, pPAwRParams);
}

llStatus_t OPT_LL_PAwRS_Terminate(uint8_t** pPAwRParams)
{
    return LL_PAwRS_Terminate(pPAwRParams);
}

uint8_t OPT_LL_PAwRS_GetSubeventNum(uint8_t* pPAwRParams)
{
    return LL_PAwRS_GetSubeventNum(pPAwRParams);
}

bool OPT_LL_PAwRS_IsEnable(void)
{
    return LL_PAwRS_IsEnable();
}

bool OPT_LL_PAwRS_IsPAwR(uint8_t* paramsSet)
{
    return LL_PAwRS_IsPAwR(paramsSet);
}

uint8_t OPT_LL_PAwRS_InitSet(uint8_t** pPAwRParams, uint8_t* pPAwRSyncInfoData, uint8_t* crcInit, RCL_CtxPeriodicScanner* rfParam, uint8_t advAddrType, uint8_t* advAddr, uint8_t phy)
{
    return LL_PAwRS_InitSet(pPAwRParams, pPAwRSyncInfoData, crcInit, rfParam, advAddrType, advAddr, phy);
}

bool OPT_LL_PAwRS_IsACADPAwR(uint8_t* pACAD)
{
    return LL_PAwRS_IsACADPAwR(pACAD);
}

void OPT_LL_PAwRS_Activate(uint8_t* pPAwRParams)
{
    LL_PAwRS_Activate(pPAwRParams);
}

void OPT_LL_PAwRS_PostProcess(const llPAwRPostProcessIn_t* in, llPAwRPostProcessOut_t* out, uint8_t* pPAwRParams)
{
    LL_PAwRS_PostProcess(in, out, pPAwRParams);
}

uint8_t OPT_LL_PAwRS_GetSyncParams(uint8_t* PAwRParams, llPAwRSyncInfo_t* syncParams)
{
    return LL_PAwRS_GetSyncParams(PAwRParams, syncParams);
}

uint32_t OPT_LL_PAWRS_GetRspPktTxCmd(uint8_t* PAwRParams)
{
    return LL_PAWRS_GetRspPktTxCmd(PAwRParams);
}

bool OPT_LL_PAwRS_ChooseRspPktOrSubeventSync(uint32_t selectedPadvSStartTime, uint8_t* PAwRParams)
{
    return LL_PAwRS_ChooseRspPktOrSubeventSync(selectedPadvSStartTime, PAwRParams);
}

void OPT_LL_PAwRS_AdjustNextStartTime(uint32_t* absStartTime, uint8_t* eventCounter, uint8_t* pPAwRParams, uint16_t eventInterval, uint8_t advSca)
{
    LL_PAwRS_AdjustNextStartTime(absStartTime, eventCounter, pPAwRParams, eventInterval, advSca);
}

void OPT_LL_PAwRS_UpdateRspDataCmdIfNeeded(uint8_t* pPAwRParams, uint16_t eventCounter, void* chanMapCurrent, uint32_t accessAddress, uint32_t absStartTime, uint16_t eventInterval)
{
    LL_PAwRS_UpdateRspDataCmdIfNeeded(pPAwRParams, eventCounter, chanMapCurrent, accessAddress, absStartTime, eventInterval);
}

bool OPT_LL_PAwRS_IsConnRspPkt(RCL_CtxPeriodicScanner* rfParam)
{
    return LL_PAwRS_IsConnRspPkt(rfParam);
}

uint8_t OPT_LL_PAwRS_BuildConnRspPkt(RCL_CtxPeriodicScanner* rfParam, void* pPAwRParams, uint8_t advAddrType, uint8_t* advAddr)
{
    return LL_PAwRS_BuildConnRspPkt(rfParam, pPAwRParams, advAddrType, advAddr);
}

#endif /* defined(USE_PAWR_SCAN) */
