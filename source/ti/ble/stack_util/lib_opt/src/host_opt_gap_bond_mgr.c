/******************************************************************************

 @file  host_opt_gap_bond_mgr.c

 @brief Implementation of the feature wrapper functions that direct to the full
        feature implementations.
        Enabling the feature by linking the wrapper functions to the real
        implementations defined in the library.

        Usage:
        Include this file in the build only when the feature is enabled.
        Add to the project GAP_BOND_MGR defines
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
#include "ti/ble/stack_util/lib_opt/host_stub_gap_bond_mgr.h"

#if defined(GAP_BOND_MGR)

#if !defined(HOST_CONFIG)
#error "One or more dependencies are missing! Please add them."
#endif

bStatus_t OPT_GAPBondMgr_ReadLocalLTK(GAP_Peer_Addr_Types_t addrType, uint8_t* pDevAddr, gapBondLTK_t* pLocalLtk)
{
    return GAPBondMgr_ReadLocalLTK(addrType, pDevAddr, pLocalLtk);
}

uint8_t OPT_gapSafeToDealloc(uint8_t status, uint8_t* safeToDealloc)
{
    return gapSafeToDealloc(status, safeToDealloc);
}

void OPT_smProcessLTKRequest_gapBond(hciEvt_BLELTKReq_t* pPkt, uint8_t* safeToDealloc)
{
    smProcessLTKRequest_gapBond(pPkt, safeToDealloc);
}

bStatus_t OPT_GAPBondMgr_UpdateRandomAddr(GAP_Addr_Modes_t addrMode, uint8_t* pRandomAddr)
{
    return GAPBondMgr_UpdateRandomAddr(addrMode, pRandomAddr);
}

uint8_t OPT_GAPBondMgr_GetAuthTaskID(void)
{
    return GAPBondMgr_GetAuthTaskID();
}

void OPT_GAPBondMgr_ReadIRKFromNV(uint8_t* keyBuff)
{
    GAPBondMgr_ReadIRKFromNV(keyBuff);
}

void OPT_GAPBondMgr_ReadCSRKFromNV(uint8_t* keyBuff)
{
    GAPBondMgr_ReadCSRKFromNV(keyBuff);
}

void OPT_GAP_UpdateResolvingList(uint8_t* pIRK)
{
    GAP_UpdateResolvingList(pIRK);
}

void OPT_gapBondMgr_Register(gapBondCBs_t* pCB)
{
    gapBondMgr_Register(pCB);
}

bStatus_t OPT_gap_checkLtk_GapBondMgr(uint16_t connectionHandle)
{
    return gap_checkLtk_GapBondMgr(connectionHandle);
}

bool OPT_GAPBondMgr_isEnable(void)
{
    return GAPBondMgr_isEnable();
}

uint8_t OPT_gapBondMgr_CheckNVLen(uint16_t id, uint8_t len)
{
    return gapBondMgr_CheckNVLen(id, len);
}

bStatus_t OPT_gapBondMgr_ServiceChangeInd(uint16_t connectionHandle, uint8_t setParam)
{
    return gapBondMgr_ServiceChangeInd(connectionHandle, setParam);
}

bStatus_t OPT_gapBondMgr_PasscodeRsp(uint16_t connectionHandle, uint8_t status, uint32_t passcode)
{
    return gapBondMgr_PasscodeRsp(connectionHandle, status, passcode);
}

bStatus_t OPT_gapBondMgr_SCSetRemoteOOBParameters(gapBondOOBData_t* remoteOobData, uint8_t OOBDataFlag)
{
    return gapBondMgr_SCSetRemoteOOBParameters(remoteOobData, OOBDataFlag);
}

bStatus_t OPT_gapBondMgr_Pair(uint16_t connHandle)
{
    return gapBondMgr_Pair(connHandle);
}

bStatus_t OPT_gapBondMgr_FindAddr(uint8_t* pDevAddr, GAP_Peer_Addr_Types_t addrType, uint8_t* pIdx, GAP_Peer_Addr_Types_t* pIdentityAddrType, uint8_t* pIdentityAddr)
{
    return gapBondMgr_FindAddr(pDevAddr, addrType, pIdx, pIdentityAddrType, pIdentityAddr);
}

bStatus_t OPT_gapBondMgr_SCGetLocalOOBParameters(gapBondOOBData_t* localOobData)
{
    return gapBondMgr_SCGetLocalOOBParameters(localOobData);
}

uint8_t OPT_HCI_TL_EXT_gapBondSetParam(uint8_t* pBuf)
{
    return HCI_TL_EXT_gapBondSetParam(pBuf);
}

bStatus_t OPT_gapBondMgr_GetParameter(uint16_t param, void* pValue)
{
    return gapBondMgr_GetParameter(param, pValue);
}

#endif /* defined(GAP_BOND_MGR) */
