/******************************************************************************

 @file  ctrl_stub_cs_test.h

 @brief This file contains prototype of default stub implementations for the
        feature wrapper functions.
        These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by ctrl_cs_test.c.

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

#ifndef CTRL_CS_TEST_H_
#define CTRL_CS_TEST_H_

#include "ti/ble/controller/ll/ll_cs_mgr.h"
#include "ti/ble/controller/ll/ll_cs_procedure.h"
#include "ti/ble/controller/ll/ll_cs_ctrl_pkt_mgr.h"
#include "ti/ble/controller/ll/ll_cs_rcl.h"

// Function prototypes for the actual implementations
extern csStatus_e LL_CS_Test(csTestParams_t* pParams);
extern csStatus_e LL_CS_TestEnd(void);
extern void HCI_CS_TestEndCompleteCback(uint8_t status);
extern uint8_t llCsInitChanIdxArrWrapper(uint8_t configId, uint16_t connId, const uint8_t* pCsConfig);
extern uint8_t llCsSelectStepChanWrapper(uint8_t stepMode, uint16_t connId, const uint8_t* pCsConfig);
extern uint8_t llCsGetNextAntennaPermutationWrapper(csACI_e ACI);
extern void llCsSelectAAWrapper(uint8_t csRole, uint32_t* aaRx, uint32_t* aaTx);
extern void llCsGetRandomSequenceWrapper(uint8_t csRole, uint32_t* pTx, uint32_t* pRx, uint8_t plLen);
extern uint8_t llCsGetToneExtentionWrapper(void);

// Wrapper functions for the feature implementations
csStatus_e OPT_LL_CS_Test(csTestParams_t* pParams);
csStatus_e OPT_LL_CS_TestEnd(void);
void OPT_HCI_CS_TestEndCompleteCback(uint8_t status);
uint8_t OPT_llCsInitChanIdxArrWrapper(uint8_t configId, uint16_t connId, const uint8_t* pCsConfig);
uint8_t OPT_llCsSelectStepChanWrapper(uint8_t stepMode, uint16_t connId, const uint8_t* pCsConfig);
uint8_t OPT_llCsGetNextAntennaPermutationWrapper(csACI_e ACI);
void OPT_llCsSelectAAWrapper(uint8_t csRole, uint32_t* aaRx, uint32_t* aaTx);
void OPT_llCsGetRandomSequenceWrapper(uint8_t csRole, uint32_t* pTx, uint32_t* pRx, uint8_t plLen);
uint8_t OPT_llCsGetToneExtentionWrapper(void);

#endif /* CTRL_CS_TEST_H_ */
