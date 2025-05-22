/******************************************************************************

 @file  ctrl_opt_power_control.c

 @brief Implementation of the feature wrapper functions that direct to the full
        feature implementations.
        Enabling the feature by linking the wrapper functions to the real
        implementations defined in the library.

        Usage:
        Include this file in the build only when the feature is enabled.
        Add to the project POWER_CONTROL defines
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
#include "ti/ble/stack_util/lib_opt/ctrl_stub_power_control.h"

#if defined(POWER_CONTROL)



void OPT_LL_PwrCtrl_Close(void)
{
    LL_PwrCtrl_Close();
}

void OPT_LL_PwrCtrl_CloseConn(uint8_t connId)
{
    LL_PwrCtrl_CloseConn(connId);
}

llStatus_t OPT_LL_PwrCtrl_Open(void)
{
    return LL_PwrCtrl_Open();
}

void OPT_LL_PwrCtrl_HandlePhyChangeEvent(llConnState_t* connPtr)
{
    LL_PwrCtrl_HandlePhyChangeEvent(connPtr);
}

llStatus_t OPT_LL_PwrCtrl_HandleProcedure(llConnState_t* connPtr, uint8_t ctrlPktType)
{
    return LL_PwrCtrl_HandleProcedure(connPtr, ctrlPktType);
}

void OPT_LL_PwrCtrl_SetupPwrCtrlPkt(const llConnState_t* connPtr, uint8_t ctrlPktType, uint8_t* pCtrlPktPayload)
{
    LL_PwrCtrl_SetupPwrCtrlPkt(connPtr, ctrlPktType, pCtrlPktPayload);
}

void OPT_LL_PwrCtrl_OpenConn(uint8_t connId)
{
    LL_PwrCtrl_OpenConn(connId);
}

void OPT_LL_PwrCtrl_UpdatePwrLevelAllConn(void)
{
    LL_PwrCtrl_UpdatePwrLevelAllConn();
}

llStatus_t OPT_LL_PwrCtrl_SetFeatureBit(void)
{
    return LL_PwrCtrl_SetFeatureBit();
}

void OPT_LL_PwrCtrl_HandleRxPackets(llConnState_t* connPtr, const uint8_t* pCtrlPktPayload, uint8_t opcode)
{
    LL_PwrCtrl_HandleRxPackets(connPtr, pCtrlPktPayload, opcode);
}

llStatus_t OPT_LL_PwrCtrl_ReadRemoteTransPwrLevelCmd(uint16_t connHandle, uint8_t txPhy)
{
    return LL_PwrCtrl_ReadRemoteTransPwrLevelCmd(connHandle, txPhy);
}

llStatus_t OPT_LL_PwrCtrl_EnhancedReadTransPwrLevelCmd(uint16_t connHandle, uint8_t txPhy, int8_t* pCurrentTxPower, int8_t* pMaxTxPower)
{
    return LL_PwrCtrl_EnhancedReadTransPwrLevelCmd(connHandle, txPhy, pCurrentTxPower, pMaxTxPower);
}

llStatus_t OPT_LL_PwrCtrl_SetTransPwrRptEnableCmd(uint16_t connHandle, uint8_t localEnable, uint8_t remoteEnable)
{
    return LL_PwrCtrl_SetTransPwrRptEnableCmd(connHandle, localEnable, remoteEnable);
}

llStatus_t OPT_LL_EXT_PwrCtrl_SendPwrCtrlReqCmd(uint16_t connHandle, uint8_t txPhy, int8_t deltaPowerDb, uint8_t aprEnable)
{
    return LL_EXT_PwrCtrl_SendPwrCtrlReqCmd(connHandle, txPhy, deltaPowerDb, aprEnable);
}

void OPT_HCI_TransPwrRptCB(uint8_t status, uint8_t connHandle, uint8_t reason, uint8_t txPhy, int8_t txPowerLevel, uint8_t txPowerMaxMinReached, int8_t delta)
{
    HCI_TransPwrRptCB(status, connHandle, reason, txPhy, txPowerLevel, txPowerMaxMinReached, delta);
}

hciStatus_t OPT_hci_ext_SendPowerControlRequestCmd(uint16_t connHandle, uint8_t txPhy, int8_t deltaPowerDb, uint8_t aprEnable)
{
    return hci_ext_SendPowerControlRequestCmd(connHandle, txPhy, deltaPowerDb, aprEnable);
}

hciStatus_t OPT_hci_le_SetTransmitPowerReportingEnableCmd(uint16_t connHandle, uint8_t localEnable, uint8_t remoteEnable)
{
    return hci_le_SetTransmitPowerReportingEnableCmd(connHandle, localEnable, remoteEnable);
}

hciStatus_t OPT_hci_le_EnhancedReadTransmitPowerLevelCmd(uint16_t connHandle, uint8_t txPhy)
{
    return hci_le_EnhancedReadTransmitPowerLevelCmd(connHandle, txPhy);
}

hciStatus_t OPT_hci_le_ReadRemoteTransmitPowerLevelCmd(uint16_t connHandle, uint8_t txPhy)
{
    return hci_le_ReadRemoteTransmitPowerLevelCmd(connHandle, txPhy);
}

#endif /* defined(POWER_CONTROL) */
