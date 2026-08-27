/******************************************************************************

 @file  ll_coex.h

 @brief This file declares the functions used by the link layer to setup BLE RCL
        commands with or without coex based on the global configuration.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2009-2026, Texas Instruments Incorporated

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

#ifndef LL_COEX_H
#define LL_COEX_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "ti/drivers/RCL.h"
#include "ti/drivers/rcl/commands/ble5.h"
#include "ti/ble/stack_util/lib_opt/ctrl_stub_coex.h"

RCL_CmdBle5Advertiser LL_COEX_RCL_CmdBle5Advertiser_DefaultRuntime(void);
RCL_CmdBle5AuxAdvertiser LL_COEX_RCL_CmdBle5AuxAdvertiser_DefaultRuntime(void);
RCL_CmdBle5PeriodicAdvertiser LL_COEX_RCL_CmdBle5PeriodicAdvertiser_DefaultRuntime(void);
RCL_CmdBle5Initiator LL_COEX_RCL_CmdInitiator_DefaultRuntime(void);
RCL_CmdBle5Scanner LL_COEX_RCL_CmdScanner_DefaultRuntime(void);
RCL_CmdBle5PeriodicScanner LL_COEX_RCL_CmdPeriodicScanner_DefaultRuntime(void);
RCL_CmdBle5Connection LL_COEX_RCL_CmdBle5Connection_DefaultRuntime(void);
RCL_CmdBle5GenericRx LL_COEX_RCL_CmdBle5GenericRx_DefaultRuntime(void);
RCL_CmdBle5GenericTx LL_COEX_RCL_CmdBle5GenericTx_DefaultRuntime(void);
RCL_CtxGenericTx LL_COEX_RCL_CtxGenericTx_DefaultRuntime(void);
RCL_CmdBle5ChAssessment LL_COEX_RCL_CmdBle5ChAssessment_DefaultRuntime(void);




#ifdef __cplusplus
}
#endif

#endif /* LL_COEX_H */
