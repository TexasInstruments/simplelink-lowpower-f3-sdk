/******************************************************************************

 @file  ctrl_stub_cm.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_cm.c.

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

#ifndef CTRL_CM_H_
#define CTRL_CM_H_

#include "ti/ble/controller/ll/ll_connection_monitor.h"

// Function prototypes for the actual implementations
extern void llCmUpdateRclCmdPhyFeatures(uint8_t connId, uint16_t phyFeatures);
extern void llCmUpdateRclCmdChannel(llConnState_t* connPtr, uint8_t channel);
extern llStatus_t llCmDynamicAlloc(void);
extern void llCmDynamicFree(void);
extern void llCmUpdateRxBuffersForActiveCm(uint8_t connId, List_List* rxBuffers);
extern void llCmCleanCmdRxBuffers(uint8_t connId);
extern void llCmInitStats(void);
extern void llCmRxEntryDoneEventHandler(RCL_Buffer_DataEntry* pDataEntry);
extern void llCm_TaskEnd(void);
extern void llCmsConnUpdateInd(llConnState_t* connPtr, uint8_t updateType);
extern void llSetTaskCm(uint8_t connId, void* nextConnCmd);
extern void llCmUpdateScanDuration(llConnState_t* connPtr);

// Wrapper functions for the feature implementations
void OPT_llCmUpdateRclCmdPhyFeatures(uint8_t connId, uint16_t phyFeatures);
void OPT_llCmUpdateRclCmdChannel(llConnState_t* connPtr, uint8_t channel);
llStatus_t OPT_llCmDynamicAlloc(void);
void OPT_llCmDynamicFree(void);
void OPT_llCmUpdateRxBuffersForActiveCm(uint8_t connId, List_List* rxBuffers);
void OPT_llCmCleanCmdRxBuffers(uint8_t connId);
void OPT_llCmInitStats(void);
void OPT_llCmRxEntryDoneEventHandler(RCL_Buffer_DataEntry* pDataEntry);
void OPT_llCm_TaskEnd(void);
void OPT_llCmsConnUpdateInd(llConnState_t* connPtr, uint8_t updateType);
void OPT_llSetTaskCm(uint8_t connId, void* nextConnCmd);
void OPT_llCmUpdateScanDuration(llConnState_t* connPtr);

#endif /* CTRL_CM_H_ */
