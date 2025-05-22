/******************************************************************************

 @file  ctrl_opt_scanner.c

 @brief Implementation of the feature wrapper functions that direct to the full
        feature implementations.
        Enabling the feature by linking the wrapper functions to the real
        implementations defined in the library.

        Usage:
        Include this file in the build only when the feature is enabled.
        Add to the project CTRL_SCANNER_CFG defines
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
#include "ti/ble/stack_util/lib_opt/ctrl_stub_scanner.h"

#if defined(CTRL_SCANNER_CFG)



void OPT_llProcessScanTimeout(void)
{
    llProcessScanTimeout();
}

void OPT_llSetTaskScan(uint8_t startType, taskInfo_t* nextSecTask, void* nextSecCommand, void* nextConnCmd)
{
    llSetTaskScan(startType, nextSecTask, nextSecCommand, nextConnCmd);
}

taskInfo_t* OPT_llSelectTaskScan(uint8_t secTaskID, uint32_t timeGap)
{
    return llSelectTaskScan(secTaskID, timeGap);
}

void OPT_llHandleScanRxEntry(RCL_Buffer_DataEntry* pDataEntry)
{
    llHandleScanRxEntry(pDataEntry);
}

void OPT_llExtScan_PostProcess(void)
{
    llExtScan_PostProcess();
}

void OPT_LL_rclScanRxEntryDone(void)
{
    LL_rclScanRxEntryDone();
}

uint8_t OPT_llChackScannerUseAcceptList(void)
{
    return llChackScannerUseAcceptList();
}

uint8_t OPT_llCheckScanHealth(uint32_t* currentTime, volatile uint32_t* scanTime)
{
    return llCheckScanHealth(currentTime, scanTime);
}

llStatus_t OPT_llAllocExtScanInfo(void)
{
    return llAllocExtScanInfo();
}

void OPT_LL_DisableScanning(void)
{
    LL_DisableScanning();
}

uint8_t OPT_LL_EXT_ValidateScanTaskType(uint8_t taskType, uint16_t taskHandle, llPeriodicScanSet_t** pPeriodicScan)
{
    return LL_EXT_ValidateScanTaskType(taskType, taskHandle, pPeriodicScan);
}

void OPT_LL_EXT_SetQOSParameters_scan(uint8_t taskType, uint32_t paramVal, llPeriodicScanSet_t* pPeriodicScan)
{
    LL_EXT_SetQOSParameters_scan(taskType, paramVal, pPeriodicScan);
}

uint8_t OPT_llCheckScanPriority(uint16_t taskID, uint8_t connPriority)
{
    return llCheckScanPriority(taskID, connPriority);
}

uint8_t OPT_LL_IsScannerWithRandomAddress(void)
{
    return LL_IsScannerWithRandomAddress();
}

void OPT_llUpdateScannSecTaskTimeGap(uint32_t* timeGap)
{
    llUpdateScannSecTaskTimeGap(timeGap);
}

void OPT_llUpdateTimeGapForScanWindow(taskInfo_t* secTask, llConnState_t* nextConnPtr, RCL_Command* secCmd, uint32_t* timeGap, uint32_t curTime)
{
    llUpdateTimeGapForScanWindow(secTask, nextConnPtr, secCmd, timeGap, curTime);
}

llStatus_t OPT_HCI_TL_set_scan_param(uint16_t opcode, uint8_t* pHciParams, aeSetScanParamCmd_t* hci_tl_cmdScanParams)
{
    return HCI_TL_set_scan_param(opcode, pHciParams, hci_tl_cmdScanParams);
}

llStatus_t OPT_HCI_TL_set_ext_scan_param(uint16_t opcode, uint8_t* pHciParams, aeSetScanParamCmd_t* hci_tl_cmdScanParams)
{
    return HCI_TL_set_ext_scan_param(opcode, pHciParams, hci_tl_cmdScanParams);
}

llStatus_t OPT_HCI_TL_set_ext_scan_enable(uint16_t opcode, uint8_t* pHciParams, aeEnableScanCmd_t* hci_tl_cmdScanEnable, aeSetScanParamCmd_t* hci_tl_cmdScanParams)
{
    return HCI_TL_set_ext_scan_enable(opcode, pHciParams, hci_tl_cmdScanEnable, hci_tl_cmdScanParams);
}

llStatus_t OPT_HCI_TL_set_scan_enable(uint16_t opcode, uint8_t* pHciParams, aeEnableScanCmd_t* hci_tl_cmdScanEnable, aeSetScanParamCmd_t* hci_tl_cmdScanParams)
{
    return HCI_TL_set_scan_enable(opcode, pHciParams, hci_tl_cmdScanEnable, hci_tl_cmdScanParams);
}

void OPT_llStopCurrentScan(void)
{
    llStopCurrentScan();
}

#endif /* defined(CTRL_SCANNER_CFG) */
