/******************************************************************************

 @file  ctrl_opt_adaptivity.c

 @brief Implementation of the feature wrapper functions that direct to the full
        feature implementations.
        Enabling the feature by linking the wrapper functions to the real
        implementations defined in the library.

        Usage:
        Include this file in the build only when the feature is enabled.
        Add to the project SDAA_ENABLE defines
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
#include "ti/ble/stack_util/lib_opt/ctrl_stub_adaptivity.h"

#if defined(SDAA_ENABLE)



uint8 OPT_LL_SDAA_ControlTxAndGetStartType(taskInfo_t* nextConnTask, taskInfo_t* secTask, uint8 startTaskType)
{
    return LL_SDAA_ControlTxAndGetStartType(nextConnTask, secTask, startTaskType);
}

taskInfo_t* OPT_LL_SDAA_GetChnlAssessmentTask(void)
{
    return LL_SDAA_GetChnlAssessmentTask();
}

void OPT_LL_SDAA_RecordTxUsageExtAdv(advSet_t* pAdvSet)
{
    LL_SDAA_RecordTxUsageExtAdv(pAdvSet);
}

void OPT_LL_rclChannelAssessmentCallback(RCL_Command* cmd, LRF_Events lrfEvents, RCL_Events events)
{
    LL_rclChannelAssessmentCallback(cmd, lrfEvents, events);
}

void OPT_LL_SDAA_RecordTxUsageConnection(llConnState_t* connPtr, uint8_t lastPayloadLen)
{
    LL_SDAA_RecordTxUsageConnection(connPtr, lastPayloadLen);
}

void OPT_LL_SDAA_RecordTxUsagePeriodic(llPeriodicAdvSet_t* pPeriodicAdv)
{
    LL_SDAA_RecordTxUsagePeriodic(pPeriodicAdv);
}

void OPT_LL_SDAA_HandleLastCmdDone(uint32 event, taskInfo_t* const pCurrTask)
{
    LL_SDAA_HandleLastCmdDone(event, pCurrTask);
}

void OPT_LL_SDAA_Init(void)
{
    LL_SDAA_Init();
}

void OPT_LL_SDAA_FreeDynamicMemory(void)
{
    LL_SDAA_FreeDynamicMemory();
}

void OPT_LL_SDAA_PostProcess(void)
{
    LL_SDAA_PostProcess();
}

#endif /* defined(SDAA_ENABLE) */
