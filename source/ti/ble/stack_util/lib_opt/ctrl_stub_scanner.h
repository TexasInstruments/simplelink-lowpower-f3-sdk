/******************************************************************************

 @file  ctrl_stub_scanner.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_scanner.c.

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

#ifndef CTRL_SCANNER_H_
#define CTRL_SCANNER_H_

#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll_scheduler.h"
#include "ti/ble/controller/ll/ll_ae.h"
#include "ti/ble/controller/hci/hci.h"

// Function prototypes for the actual implementations
extern void llProcessScanTimeout(void);
extern void llSetTaskScan(uint8_t startType, taskInfo_t* nextSecTask, void* nextSecCommand, void* nextConnCmd);
extern taskInfo_t* llSelectTaskScan(uint8_t secTaskID, uint32_t timeGap);
extern void llHandleScanRxEntry(RCL_Buffer_DataEntry* pDataEntry);
extern void llExtScan_PostProcess(void);
extern void LL_rclScanRxEntryDone(void);
extern uint8_t llChackScannerUseAcceptList(void);
extern uint8_t llCheckScanHealth(uint32_t* currentTime, volatile uint32_t* scanTime);
extern llStatus_t llAllocExtScanInfo(void);
extern void LL_DisableScanning(void);
extern uint8_t LL_EXT_ValidateScanTaskType(uint8_t taskType, uint16_t taskHandle, llPeriodicScanSet_t** pPeriodicScan);
extern void LL_EXT_SetQOSParameters_scan(uint8_t taskType, uint32_t paramVal, llPeriodicScanSet_t* pPeriodicScan);
extern uint8_t llCheckScanPriority(uint16_t taskID, uint8_t connPriority);
extern uint8_t LL_IsScannerWithRandomAddress(void);
extern void llUpdateScannSecTaskTimeGap(uint32_t* timeGap);
extern void llUpdateTimeGapForScanWindow(taskInfo_t* secTask, llConnState_t* nextConnPtr, RCL_Command* secCmd, uint32_t* timeGap, uint32_t curTime);
extern llStatus_t HCI_TL_set_scan_param(uint16_t opcode, uint8_t* pHciParams, aeSetScanParamCmd_t* hci_tl_cmdScanParams);
extern llStatus_t HCI_TL_set_ext_scan_param(uint16_t opcode, uint8_t* pHciParams, aeSetScanParamCmd_t* hci_tl_cmdScanParams);
extern llStatus_t HCI_TL_set_ext_scan_enable(uint16_t opcode, uint8_t* pHciParams, aeEnableScanCmd_t* hci_tl_cmdScanEnable, aeSetScanParamCmd_t* hci_tl_cmdScanParams);
extern llStatus_t HCI_TL_set_scan_enable(uint16_t opcode, uint8_t* pHciParams, aeEnableScanCmd_t* hci_tl_cmdScanEnable, aeSetScanParamCmd_t* hci_tl_cmdScanParams);
extern void llStopCurrentScan(void);

// Wrapper functions for the feature implementations
void OPT_llProcessScanTimeout(void);
void OPT_llSetTaskScan(uint8_t startType, taskInfo_t* nextSecTask, void* nextSecCommand, void* nextConnCmd);
taskInfo_t* OPT_llSelectTaskScan(uint8_t secTaskID, uint32_t timeGap);
void OPT_llHandleScanRxEntry(RCL_Buffer_DataEntry* pDataEntry);
void OPT_llExtScan_PostProcess(void);
void OPT_LL_rclScanRxEntryDone(void);
uint8_t OPT_llChackScannerUseAcceptList(void);
uint8_t OPT_llCheckScanHealth(uint32_t* currentTime, volatile uint32_t* scanTime);
llStatus_t OPT_llAllocExtScanInfo(void);
void OPT_LL_DisableScanning(void);
uint8_t OPT_LL_EXT_ValidateScanTaskType(uint8_t taskType, uint16_t taskHandle, llPeriodicScanSet_t** pPeriodicScan);
void OPT_LL_EXT_SetQOSParameters_scan(uint8_t taskType, uint32_t paramVal, llPeriodicScanSet_t* pPeriodicScan);
uint8_t OPT_llCheckScanPriority(uint16_t taskID, uint8_t connPriority);
uint8_t OPT_LL_IsScannerWithRandomAddress(void);
void OPT_llUpdateScannSecTaskTimeGap(uint32_t* timeGap);
void OPT_llUpdateTimeGapForScanWindow(taskInfo_t* secTask, llConnState_t* nextConnPtr, RCL_Command* secCmd, uint32_t* timeGap, uint32_t curTime);
llStatus_t OPT_HCI_TL_set_scan_param(uint16_t opcode, uint8_t* pHciParams, aeSetScanParamCmd_t* hci_tl_cmdScanParams);
llStatus_t OPT_HCI_TL_set_ext_scan_param(uint16_t opcode, uint8_t* pHciParams, aeSetScanParamCmd_t* hci_tl_cmdScanParams);
llStatus_t OPT_HCI_TL_set_ext_scan_enable(uint16_t opcode, uint8_t* pHciParams, aeEnableScanCmd_t* hci_tl_cmdScanEnable, aeSetScanParamCmd_t* hci_tl_cmdScanParams);
llStatus_t OPT_HCI_TL_set_scan_enable(uint16_t opcode, uint8_t* pHciParams, aeEnableScanCmd_t* hci_tl_cmdScanEnable, aeSetScanParamCmd_t* hci_tl_cmdScanParams);
void OPT_llStopCurrentScan(void);

#endif /* CTRL_SCANNER_H_ */
