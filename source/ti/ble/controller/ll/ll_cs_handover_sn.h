/******************************************************************************

 @file  ll_cs_handover_sn.h

 @brief This file contains the data structures and APIs for handling
        Bluetooth Low Energy handover process for Channel Sounding on the
        serving node

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

/*********************************************************************
 *
 * WARNING!!!
 *
 * THE API'S FOUND IN THIS FILE ARE FOR INTERNAL STACK USE ONLY!
 * FUNCTIONS SHOULD NOT BE CALLED DIRECTLY FROM APPLICATIONS, AND ANY
 * CALLS TO THESE FUNCTIONS FROM OUTSIDE OF THE STACK MAY RESULT IN
 * UNEXPECTED BEHAVIOR.
 *
 */

#ifndef LL_CS_HANDOVER_SN_H
#define LL_CS_HANDOVER_SN_H

/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/comdef.h"
#include "ti/ble/controller/ll/ll_cs_common.h"
#include "ti/ble/controller/ll/ll_csdrbg.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */

/*******************************************************************************
 * @fn          LL_CS_Handover_SnGetSNDataSize
 *
 * @brief       Return the LL CS handover data size
 *
 * input parameters
 *
 * @param       connHandle - Connection Handle to be handled by handover
 *
 * output parameters
 *
 * @param       None
 *
 * @return      The controller handover CS data size
 */
uint32_t LL_CS_Handover_SnGetSNDataSize( uint16 connHandle );

/*******************************************************************************
 * @fn          LL_CS_Handover_SnPopulateSnData
 *
 * @brief Populate the Serving Node (SN) data for the CS Handover process.
 *
 * This function populates the Serving Node (SN) data for the CS Handover process
 * based on the given connection handle.
 *
 * input parameters
 *
 * @param connHandle The connection handle for which the SN data is to be populated.
 * @param pContBuf Pointer to the buffer where the SN data will be stored.
 *
 * output parameters
 *
 * @param pContBuf Pointer to the buffer where the SN data was stored.
 *
 * @return  None
 */
void LL_CS_Handover_SnPopulateSnData(uint16_t connHandle, uint8_t *pContBuf);

#ifdef CS_HANDOVER_TEST
csStatus_e llCs_HandoverTest(uint16_t connId);
#endif

#endif /* LL_CS_HANDOVER_SN_H */
