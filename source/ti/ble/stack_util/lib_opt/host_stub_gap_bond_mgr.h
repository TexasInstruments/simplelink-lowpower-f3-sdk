/******************************************************************************

 @file  host_stub_gap_bond_mgr.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by host_gap_bond_mgr.c.

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

#ifndef HOST_GAP_BOND_MGR_H_
#define HOST_GAP_BOND_MGR_H_

#include "ti/ble/host/gap/gap.h"
#include "ti/ble/host/gapbondmgr/gapbondmgr.h"
#include "ti/ble/host/gapbondmgr/gapbondmgr_internal.h"
#include "ti/ble/host/sm/sm_internal.h"
#include "ti/ble/controller/hci/hci.h"

// Function prototypes for the actual implementations
extern bStatus_t GAPBondMgr_ReadLocalLTK(GAP_Peer_Addr_Types_t addrType, uint8_t* pDevAddr, gapBondLTK_t* pLocalLtk);
extern uint8_t gapSafeToDealloc(uint8_t status, uint8_t* safeToDealloc);
extern void smProcessLTKRequest_gapBond(hciEvt_BLELTKReq_t* pPkt, uint8_t* safeToDealloc);
extern bStatus_t GAPBondMgr_UpdateRandomAddr(GAP_Addr_Modes_t addrMode, uint8_t* pRandomAddr);
extern uint8_t GAPBondMgr_GetAuthTaskID(void);
extern void GAPBondMgr_ReadIRKFromNV(uint8_t* keyBuff);
extern void GAPBondMgr_ReadCSRKFromNV(uint8_t* keyBuff);
extern void GAP_UpdateResolvingList(uint8_t* pIRK);
extern void gapBondMgr_Register(gapBondCBs_t* pCB);
extern bStatus_t gap_checkLtk_GapBondMgr(uint16_t connectionHandle);
extern bool GAPBondMgr_isEnable(void);
extern uint8_t gapBondMgr_CheckNVLen(uint16_t id, uint8_t len);
extern bStatus_t gapBondMgr_ServiceChangeInd(uint16_t connectionHandle, uint8_t setParam);
extern bStatus_t gapBondMgr_PasscodeRsp(uint16_t connectionHandle, uint8_t status, uint32_t passcode);
extern bStatus_t gapBondMgr_SCSetRemoteOOBParameters(gapBondOOBData_t* remoteOobData, uint8_t OOBDataFlag);
extern bStatus_t gapBondMgr_Pair(uint16_t connHandle);
extern bStatus_t gapBondMgr_FindAddr(uint8_t* pDevAddr, GAP_Peer_Addr_Types_t addrType, uint8_t* pIdx, GAP_Peer_Addr_Types_t* pIdentityAddrType, uint8_t* pIdentityAddr);
extern bStatus_t gapBondMgr_SCGetLocalOOBParameters(gapBondOOBData_t* localOobData);
extern uint8_t HCI_TL_EXT_gapBondSetParam(uint8_t* pBuf);
extern bStatus_t gapBondMgr_GetParameter(uint16_t param, void* pValue);

// Wrapper functions for the feature implementations
bStatus_t OPT_GAPBondMgr_ReadLocalLTK(GAP_Peer_Addr_Types_t addrType, uint8_t* pDevAddr, gapBondLTK_t* pLocalLtk);
uint8_t OPT_gapSafeToDealloc(uint8_t status, uint8_t* safeToDealloc);
void OPT_smProcessLTKRequest_gapBond(hciEvt_BLELTKReq_t* pPkt, uint8_t* safeToDealloc);
bStatus_t OPT_GAPBondMgr_UpdateRandomAddr(GAP_Addr_Modes_t addrMode, uint8_t* pRandomAddr);
uint8_t OPT_GAPBondMgr_GetAuthTaskID(void);
void OPT_GAPBondMgr_ReadIRKFromNV(uint8_t* keyBuff);
void OPT_GAPBondMgr_ReadCSRKFromNV(uint8_t* keyBuff);
void OPT_GAP_UpdateResolvingList(uint8_t* pIRK);
void OPT_gapBondMgr_Register(gapBondCBs_t* pCB);
bStatus_t OPT_gap_checkLtk_GapBondMgr(uint16_t connectionHandle);
bool OPT_GAPBondMgr_isEnable(void);
uint8_t OPT_gapBondMgr_CheckNVLen(uint16_t id, uint8_t len);
bStatus_t OPT_gapBondMgr_ServiceChangeInd(uint16_t connectionHandle, uint8_t setParam);
bStatus_t OPT_gapBondMgr_PasscodeRsp(uint16_t connectionHandle, uint8_t status, uint32_t passcode);
bStatus_t OPT_gapBondMgr_SCSetRemoteOOBParameters(gapBondOOBData_t* remoteOobData, uint8_t OOBDataFlag);
bStatus_t OPT_gapBondMgr_Pair(uint16_t connHandle);
bStatus_t OPT_gapBondMgr_FindAddr(uint8_t* pDevAddr, GAP_Peer_Addr_Types_t addrType, uint8_t* pIdx, GAP_Peer_Addr_Types_t* pIdentityAddrType, uint8_t* pIdentityAddr);
bStatus_t OPT_gapBondMgr_SCGetLocalOOBParameters(gapBondOOBData_t* localOobData);
uint8_t OPT_HCI_TL_EXT_gapBondSetParam(uint8_t* pBuf);
bStatus_t OPT_gapBondMgr_GetParameter(uint16_t param, void* pValue);

#endif /* HOST_GAP_BOND_MGR_H_ */
