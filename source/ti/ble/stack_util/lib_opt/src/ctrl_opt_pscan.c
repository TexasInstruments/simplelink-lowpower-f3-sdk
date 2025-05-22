/******************************************************************************

 @file  ctrl_opt_pscan.c

 @brief Implementation of the feature wrapper functions that direct to the full
        feature implementations.
        Enabling the feature by linking the wrapper functions to the real
        implementations defined in the library.

        Usage:
        Include this file in the build only when the feature is enabled.
        Add to the project USE_PERIODIC_SCAN defines
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
#include "ti/ble/stack_util/lib_opt/ctrl_stub_pscan.h"

#if defined(USE_PERIODIC_SCAN)

#if !defined(CTRL_SCANNER_CFG)
#error "One or more dependencies are missing! Please add them."
#endif

void OPT_llSetTaskPeriodicScan(void)
{
    llSetTaskPeriodicScan();
}

taskInfo_t * OPT_llSelectTaskPeriodicScan(uint8_t secTaskID, uint32_t timeGap)
{
    return llSelectTaskPeriodicScan(secTaskID, timeGap);
}

bool OPT_llCheckSyncInfoCriteria(llAdvPDUInfo * pAdvPDUInfo)
{
    return llCheckSyncInfoCriteria(pAdvPDUInfo);
}

void OPT_llProcessPeriodicScanSyncInfo(llExtAdvPDUInfo * pExtAdvInfo, aeExtAdvRptEvt_t * advEvent, uint32_t timeStamp)
{
    llProcessPeriodicScanSyncInfo(pExtAdvInfo, advEvent, timeStamp);
}

void OPT_llEndPeriodicScanTask(llPeriodicScanSet_t* pPeriodicScan)
{
    llEndPeriodicScanTask(pPeriodicScan);
}

void OPT_llPeriodicScan_PostProcess(void)
{
    llPeriodicScan_PostProcess();
}

void OPT_llProcessPeriodicScanRxFIFO(void)
{
    llProcessPeriodicScanRxFIFO();
}

void * OPT_llFindNextPeriodicScan(void)
{
    return llFindNextPeriodicScan();
}

void OPT_llTerminatePeriodicScan(void)
{
    llTerminatePeriodicScan();
}

llPeriodicScanSet_t* OPT_llGetCurrentPeriodicScan(uint8_t state)
{
    return llGetCurrentPeriodicScan(state);
}

llPeriodicScanSet_t* OPT_llGetPeriodicScan(uint16_t handle)
{
    return llGetPeriodicScan(handle);
}

void OPT_llClearPeriodicScanSets(void)
{
    llClearPeriodicScanSets();
}

void OPT_llUpdatePADVBParamsInScanCmd(void)
{
    llUpdatePADVBParamsInScanCmd();
}

hciStatus_t OPT_hciCmdParserPeriodicScan(uint8_t * pData, uint16_t cmdOpCode)
{
    return hciCmdParserPeriodicScan(pData, cmdOpCode);
}

uint32_t OPT_llReturnCurrentPeriodicStartTime(void)
{
    return llReturnCurrentPeriodicStartTime();
}

uint8_t OPT_llCheckPeriodicScanPriority(uint16_t taskID, uint8_t connPriority)
{
    return llCheckPeriodicScanPriority(taskID, connPriority);
}

bool OPT_LL_PeriodicScanIsEnable(void)
{
    return LL_PeriodicScanIsEnable();
}

taskInfo_t * OPT_llSelectTaskPeriodicScanHandle(uint32_t* timeGap, uint16_t* secTaskID)
{
    return llSelectTaskPeriodicScanHandle(timeGap, secTaskID);
}

void OPT_llUpdatePeriodicScanTimeGap(uint16_t taskID, uint32_t* timeGap)
{
    llUpdatePeriodicScanTimeGap(taskID, timeGap);
}

#endif /* defined(USE_PERIODIC_SCAN) */
