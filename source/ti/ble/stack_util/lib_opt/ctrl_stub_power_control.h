/******************************************************************************

 @file  ctrl_stub_power_control.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_power_control.c.

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

#ifndef CTRL_POWER_CONTROL_H_
#define CTRL_POWER_CONTROL_H_

#include "ti/ble/controller/ll/ll_power_control.h"
#include "ti/ble/controller/hci/hci.h"

// Function prototypes for the actual implementations
extern void LL_PwrCtrl_Close(void);
extern void LL_PwrCtrl_CloseConn(uint8_t connId);
extern llStatus_t LL_PwrCtrl_Open(void);
extern void LL_PwrCtrl_HandlePhyChangeEvent(llConnState_t* connPtr);
extern llStatus_t LL_PwrCtrl_HandleProcedure(llConnState_t* connPtr, uint8_t ctrlPktType);
extern void LL_PwrCtrl_SetupPwrCtrlPkt(const llConnState_t* connPtr, uint8_t ctrlPktType, uint8_t* pCtrlPktPayload);
extern void LL_PwrCtrl_OpenConn(uint8_t connId);
extern void LL_PwrCtrl_UpdatePwrLevelAllConn(void);
extern llStatus_t LL_PwrCtrl_SetFeatureBit(void);
extern void LL_PwrCtrl_HandleRxPackets(llConnState_t* connPtr, const uint8_t* pCtrlPktPayload, uint8_t opcode);
extern llStatus_t LL_PwrCtrl_ReadRemoteTransPwrLevelCmd(uint16_t connHandle, uint8_t txPhy);
extern llStatus_t LL_PwrCtrl_EnhancedReadTransPwrLevelCmd(uint16_t connHandle, uint8_t txPhy, int8_t* pCurrentTxPower, int8_t* pMaxTxPower);
extern llStatus_t LL_PwrCtrl_SetTransPwrRptEnableCmd(uint16_t connHandle, uint8_t localEnable, uint8_t remoteEnable);
extern llStatus_t LL_EXT_PwrCtrl_SendPwrCtrlReqCmd(uint16_t connHandle, uint8_t txPhy, int8_t deltaPowerDb, uint8_t aprEnable);
extern void HCI_TransPwrRptCB(uint8_t status, uint8_t connHandle, uint8_t reason, uint8_t txPhy, int8_t txPowerLevel, uint8_t txPowerMaxMinReached, int8_t delta);
extern hciStatus_t hci_ext_SendPowerControlRequestCmd(uint16_t connHandle, uint8_t txPhy, int8_t deltaPowerDb, uint8_t aprEnable);
extern hciStatus_t hci_le_SetTransmitPowerReportingEnableCmd(uint16_t connHandle, uint8_t localEnable, uint8_t remoteEnable);
extern hciStatus_t hci_le_EnhancedReadTransmitPowerLevelCmd(uint16_t connHandle, uint8_t txPhy);
extern hciStatus_t hci_le_ReadRemoteTransmitPowerLevelCmd(uint16_t connHandle, uint8_t txPhy);

// Wrapper functions for the feature implementations
void OPT_LL_PwrCtrl_Close(void);
void OPT_LL_PwrCtrl_CloseConn(uint8_t connId);
llStatus_t OPT_LL_PwrCtrl_Open(void);
void OPT_LL_PwrCtrl_HandlePhyChangeEvent(llConnState_t* connPtr);
llStatus_t OPT_LL_PwrCtrl_HandleProcedure(llConnState_t* connPtr, uint8_t ctrlPktType);
void OPT_LL_PwrCtrl_SetupPwrCtrlPkt(const llConnState_t* connPtr, uint8_t ctrlPktType, uint8_t* pCtrlPktPayload);
void OPT_LL_PwrCtrl_OpenConn(uint8_t connId);
void OPT_LL_PwrCtrl_UpdatePwrLevelAllConn(void);
llStatus_t OPT_LL_PwrCtrl_SetFeatureBit(void);
void OPT_LL_PwrCtrl_HandleRxPackets(llConnState_t* connPtr, const uint8_t* pCtrlPktPayload, uint8_t opcode);
llStatus_t OPT_LL_PwrCtrl_ReadRemoteTransPwrLevelCmd(uint16_t connHandle, uint8_t txPhy);
llStatus_t OPT_LL_PwrCtrl_EnhancedReadTransPwrLevelCmd(uint16_t connHandle, uint8_t txPhy, int8_t* pCurrentTxPower, int8_t* pMaxTxPower);
llStatus_t OPT_LL_PwrCtrl_SetTransPwrRptEnableCmd(uint16_t connHandle, uint8_t localEnable, uint8_t remoteEnable);
llStatus_t OPT_LL_EXT_PwrCtrl_SendPwrCtrlReqCmd(uint16_t connHandle, uint8_t txPhy, int8_t deltaPowerDb, uint8_t aprEnable);
void OPT_HCI_TransPwrRptCB(uint8_t status, uint8_t connHandle, uint8_t reason, uint8_t txPhy, int8_t txPowerLevel, uint8_t txPowerMaxMinReached, int8_t delta);
hciStatus_t OPT_hci_ext_SendPowerControlRequestCmd(uint16_t connHandle, uint8_t txPhy, int8_t deltaPowerDb, uint8_t aprEnable);
hciStatus_t OPT_hci_le_SetTransmitPowerReportingEnableCmd(uint16_t connHandle, uint8_t localEnable, uint8_t remoteEnable);
hciStatus_t OPT_hci_le_EnhancedReadTransmitPowerLevelCmd(uint16_t connHandle, uint8_t txPhy);
hciStatus_t OPT_hci_le_ReadRemoteTransmitPowerLevelCmd(uint16_t connHandle, uint8_t txPhy);

#endif /* CTRL_POWER_CONTROL_H_ */
