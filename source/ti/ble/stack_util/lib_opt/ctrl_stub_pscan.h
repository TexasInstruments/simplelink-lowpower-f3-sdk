/******************************************************************************

 @file  ctrl_stub_pscan.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_pscan.c.

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

#ifndef CTRL_PSCAN_H_
#define CTRL_PSCAN_H_

#include "ti/ble/stack_util/comdef.h"
#include "ti/ble/controller/hci/hci.h"
#include "ti/ble/controller/ll/ll_scheduler.h"
#include "ti/ble/controller/ll/ll_ae.h"
#include "ti/ble/controller/ll/ll.h"

// Function prototypes for the actual implementations
extern void llSetTaskPeriodicScan(void);
extern taskInfo_t * llSelectTaskPeriodicScan(uint8_t secTaskID, uint32_t timeGap);
extern bool llCheckSyncInfoCriteria(llAdvPDUInfo * pAdvPDUInfo);
extern void llProcessPeriodicScanSyncInfo(llExtAdvPDUInfo * pExtAdvInfo, aeExtAdvRptEvt_t * advEvent, uint32_t timeStamp);
extern void llEndPeriodicScanTask(llPeriodicScanSet_t* pPeriodicScan);
extern void llPeriodicScan_PostProcess(void);
extern void llProcessPeriodicScanRxFIFO(void);
extern void * llFindNextPeriodicScan(void);
extern void llTerminatePeriodicScan(void);
extern llPeriodicScanSet_t* llGetCurrentPeriodicScan(uint8_t state);
extern llPeriodicScanSet_t* llGetPeriodicScan(uint16_t handle);
extern void llClearPeriodicScanSets(void);
extern void llUpdatePADVBParamsInScanCmd(void);
extern hciStatus_t hciCmdParserPeriodicScan(uint8_t * pData, uint16_t cmdOpCode);
extern uint32_t llReturnCurrentPeriodicStartTime(void);
extern uint8_t llCheckPeriodicScanPriority(uint16_t taskID, uint8_t connPriority);
extern bool LL_PeriodicScanIsEnable(void);
extern taskInfo_t * llSelectTaskPeriodicScanHandle(uint32_t* timeGap, uint16_t* secTaskID);
extern void llUpdatePeriodicScanTimeGap(uint16_t taskID, uint32_t* timeGap);

// Wrapper functions for the feature implementations
void OPT_llSetTaskPeriodicScan(void);
taskInfo_t * OPT_llSelectTaskPeriodicScan(uint8_t secTaskID, uint32_t timeGap);
bool OPT_llCheckSyncInfoCriteria(llAdvPDUInfo * pAdvPDUInfo);
void OPT_llProcessPeriodicScanSyncInfo(llExtAdvPDUInfo * pExtAdvInfo, aeExtAdvRptEvt_t * advEvent, uint32_t timeStamp);
void OPT_llEndPeriodicScanTask(llPeriodicScanSet_t* pPeriodicScan);
void OPT_llPeriodicScan_PostProcess(void);
void OPT_llProcessPeriodicScanRxFIFO(void);
void * OPT_llFindNextPeriodicScan(void);
void OPT_llTerminatePeriodicScan(void);
llPeriodicScanSet_t* OPT_llGetCurrentPeriodicScan(uint8_t state);
llPeriodicScanSet_t* OPT_llGetPeriodicScan(uint16_t handle);
void OPT_llClearPeriodicScanSets(void);
void OPT_llUpdatePADVBParamsInScanCmd(void);
hciStatus_t OPT_hciCmdParserPeriodicScan(uint8_t * pData, uint16_t cmdOpCode);
uint32_t OPT_llReturnCurrentPeriodicStartTime(void);
uint8_t OPT_llCheckPeriodicScanPriority(uint16_t taskID, uint8_t connPriority);
bool OPT_LL_PeriodicScanIsEnable(void);
taskInfo_t * OPT_llSelectTaskPeriodicScanHandle(uint32_t* timeGap, uint16_t* secTaskID);
void OPT_llUpdatePeriodicScanTimeGap(uint16_t taskID, uint32_t* timeGap);

#endif /* CTRL_PSCAN_H_ */
