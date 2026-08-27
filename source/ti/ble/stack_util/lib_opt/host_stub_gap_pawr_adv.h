/******************************************************************************

 @file  host_stub_gap_pawr_adv.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by host_gap_pawr_adv.c.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025-2026, Texas Instruments Incorporated

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

#ifndef HOST_GAP_PAWR_ADV_H_
#define HOST_GAP_PAWR_ADV_H_

#include "ti/ble/host/gap/gap_advertiser.h"
#include "ti/ble/host/gap/gap_initiator.h"

// Function prototypes for the actual implementations
extern uint8_t GapAdv_SetPeriodicAdvSubeventData(GapAdv_padvSubEvtData_t* pPadvSubEvtData);
extern uint8_t GapAdv_SetPeriodicAdvParamsV2(uint8_t advHandle, GapAdv_periodicAdvParams_t* periodicAdvParams, GapAdv_PAwRParams_t* pPAwRParams);
extern uint8_t gapAdv_processBLEPeriodicAdvCBs(osal_event_hdr_t* pMsg);
extern uint8_t GapInit_connectV2(uint8_t advHandle, uint8_t subevent, GAP_Peer_Addr_Types_t peerAddrType, uint8_t* pPeerAddress, uint8_t phys, uint16_t timeout);


// Wrapper functions for the feature implementations
uint8_t OPT_GapAdv_SetPeriodicAdvSubeventData(GapAdv_padvSubEvtData_t* pPadvSubEvtData);
uint8_t OPT_GapAdv_SetPeriodicAdvParamsV2(uint8_t advHandle, GapAdv_periodicAdvParams_t* periodicAdvParams, GapAdv_PAwRParams_t* pPAwRParams);
uint8_t OPT_gapAdv_processBLEPeriodicAdvCBs(osal_event_hdr_t* pMsg);
uint8_t OPT_GapInit_connectV2(uint8_t advHandle, uint8_t subevent, GAP_Peer_Addr_Types_t peerAddrType, uint8_t* pPeerAddress, uint8_t phys, uint16_t timeout);

#endif /* HOST_GAP_PAWR_ADV_H_ */
