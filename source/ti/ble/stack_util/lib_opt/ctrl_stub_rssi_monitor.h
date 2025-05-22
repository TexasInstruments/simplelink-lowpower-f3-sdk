/******************************************************************************

 @file  ctrl_stub_rssi_monitor.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_rssi_monitor.c.

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

#ifndef CTRL_RSSI_MONITOR_H_
#define CTRL_RSSI_MONITOR_H_

#include "ti/ble/controller/ll/ll_rssi_monitor.h"
#include "ti/ble/controller/hci/hci.h"
#include "ti/ble/controller/ll/ll.h"

// Function prototypes for the actual implementations
extern llStatus_t LL_RssiMon_Open(void);
extern llStatus_t LL_RssiMon_Close(void);
extern llStatus_t LL_RssiMon_GetRssiStat(uint8_t connId, int8_t* pRssiStat);
extern llRssiMonCbHandle_t LL_RssiMon_Reg(uint8_t connId, int8_t lowRssiThresh, int8_t highRssiThresh, pfnRssiMonThreshCB_t pfnCB);
extern llStatus_t LL_RssiMon_Unreg(llRssiMonCbHandle_t regHandle);
extern llStatus_t LL_RssiMon_UnregByConnId(uint8_t connId);
extern llStatus_t LL_RssiMon_SetConfig(int8_t incomeRssiWeight, uint8_t minRssiSamples);
extern llStatus_t LL_RssiMon_GetConfig(int8_t* pIncomeRssiWeight, uint8_t* pMinRssiSamples);
extern llStatus_t LL_RssiMon_Execute(uint8_t connId);
extern llStatus_t LL_RssiMon_ClearConnData(uint8_t connId);
extern void HCI_EXT_RssiMon_ReportCB(uint8_t handle, int8_t threshPass);
extern hciStatus_t hci_ext_RssiMon_RegCmd(uint16_t connHandle, int8_t lowRssiThresh, int8_t highRssiThresh);
extern hciStatus_t hci_ext_RssiMon_UnregCmd(llRssiMonCbHandle_t regHandle);
extern hciStatus_t hci_ext_RssiMon_SetConfigCmd(int8_t incomeRssiWeight, uint8_t minRssiSamples);
extern hciStatus_t hci_ext_RssiMon_GetConfigCmd(void);
extern hciStatus_t hci_ext_RssiMon_GetRssiStatCmd(uint16_t connHandle);

// Wrapper functions for the feature implementations
llStatus_t OPT_LL_RssiMon_Open(void);
llStatus_t OPT_LL_RssiMon_Close(void);
llStatus_t OPT_LL_RssiMon_GetRssiStat(uint8_t connId, int8_t* pRssiStat);
llRssiMonCbHandle_t OPT_LL_RssiMon_Reg(uint8_t connId, int8_t lowRssiThresh, int8_t highRssiThresh, pfnRssiMonThreshCB_t pfnCB);
llStatus_t OPT_LL_RssiMon_Unreg(llRssiMonCbHandle_t regHandle);
llStatus_t OPT_LL_RssiMon_UnregByConnId(uint8_t connId);
llStatus_t OPT_LL_RssiMon_SetConfig(int8_t incomeRssiWeight, uint8_t minRssiSamples);
llStatus_t OPT_LL_RssiMon_GetConfig(int8_t* pIncomeRssiWeight, uint8_t* pMinRssiSamples);
llStatus_t OPT_LL_RssiMon_Execute(uint8_t connId);
llStatus_t OPT_LL_RssiMon_ClearConnData(uint8_t connId);
void OPT_HCI_EXT_RssiMon_ReportCB(uint8_t handle, int8_t threshPass);
hciStatus_t OPT_hci_ext_RssiMon_RegCmd(uint16_t connHandle, int8_t lowRssiThresh, int8_t highRssiThresh);
hciStatus_t OPT_hci_ext_RssiMon_UnregCmd(llRssiMonCbHandle_t regHandle);
hciStatus_t OPT_hci_ext_RssiMon_SetConfigCmd(int8_t incomeRssiWeight, uint8_t minRssiSamples);
hciStatus_t OPT_hci_ext_RssiMon_GetConfigCmd(void);
hciStatus_t OPT_hci_ext_RssiMon_GetRssiStatCmd(uint16_t connHandle);

#endif /* CTRL_RSSI_MONITOR_H_ */
