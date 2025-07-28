/******************************************************************************

 @file  ctrl_stub_dmm_dynamic_priority.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_dmm_dynamic_priority.c.

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

#ifndef CTRL_DMM_DYNAMIC_PRIORITY_H_
#define CTRL_DMM_DYNAMIC_PRIORITY_H_

#include "ti/ble/controller/ll/ll_scheduler.h"
#include "ti/ble/controller/ll/ll_ae.h"
#include "ti/ble/controller/ll/ll_dmm_dynamic_priority.h"
#include "ti/drivers/rcl/RCL_Command.h"

// Function prototypes for the actual implementations
extern void llDmmDynamicPri_init(void);
extern uint8_t llDmmDynamicPri_isCmdPreempted(RCL_CommandStatus status);
extern void llDmmDynamicPri_SetAdvHandle(uint8 handle, uint8 clearHandle);
extern llDmmDynamicPriThresholdAdv_t * llDmmDynamicPri_GetAdvPriStruct(uint8 handle);
extern void llDmmDynamicPri_SetDMMPriority(RCL_Command * cmd, uint32_t dmmActivityPriority);
extern void llDmmDynamicPri_SetCoexPriority(RCL_Command * cmd, RCL_Command_CoexPriority coexPriority);
extern RCL_Command_CoexPriority llDmmDynamicPri_GetCoexPriority(uint32_t dmmActivityPriority);
extern uint32_t llDMMDynamicPri_GetDMMPriority(RCL_Command * cmd);
extern void llDmmDynamicPri_updatePreemptionCounters(uint16_t cmdId, RCL_CommandStatus cmdStatus, uint8_t advHandle);
extern void llDmmDynamicPri_SetCmdPreemptionCounters(uint16_t cmdId, uint8_t advHandle, uint8_t reset);


// Wrapper functions for the feature implementations
void OPT_llDmmDynamicPri_init(void);
uint8_t OPT_llDmmDynamicPri_isCmdPreempted(RCL_CommandStatus status);
void OPT_llDmmDynamicPri_SetAdvHandle(uint8 handle, uint8 clearHandle);
llDmmDynamicPriThresholdAdv_t * OPT_llDmmDynamicPri_GetAdvPriStruct(uint8 handle);
void OPT_llDmmDynamicPri_SetDMMPriority(RCL_Command * cmd, uint32_t dmmActivityPriority);
void OPT_llDmmDynamicPri_SetCoexPriority(RCL_Command * cmd, RCL_Command_CoexPriority coexPriority);
RCL_Command_CoexPriority OPT_llDmmDynamicPri_GetCoexPriority(uint32_t dmmActivityPriority);
uint32_t OPT_llDMMDynamicPri_GetDMMPriority(RCL_Command * cmd);
void OPT_llDmmDynamicPri_updatePreemptionCounters(uint16_t cmdId, RCL_CommandStatus cmdStatus, uint8_t advHandle);
void OPT_llDmmDynamicPri_SetCmdPreemptionCounters(uint16_t cmdId, uint8_t advHandle, uint8_t reset);

#endif /* CTRL_DMM_DYNAMIC_PRIORITY_H_ */
