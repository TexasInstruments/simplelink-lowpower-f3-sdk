/******************************************************************************

 @file  ll_cs_ctrl_pkt_mgr.h

 @brief The LL CS Ctrl Packet manager contains the APIs that build CS control
        Packets, send CS control pakcets, handles received CS control pakcets.
        Manages the CS procedure that is started by the contol packets
        Uses the CS DB to read/write CS data.
        Notifies the host when a CS procedure was completed.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2024, Texas Instruments Incorporated

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
/*******************************************************************************
 * INCLUDES
 */
#include "ll_common.h"
#include "ll_cs_common.h"
/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * EXTERNS
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
 * @fn          llCsProcessCsControlPacket
 *
 * @brief       This API is used to handle the reception of a CS packet.
 * Once a Rx interrupt occurs, the BLE module is called and in case it's a
 * CS Control packet that was received, llCsProcessCsControlPacket
 * is called to parse and handle the received packet. The size of
 * pBuf depends on ctrlType.
 *
 * The size of the received packet is checked in the ISR function
 * prior to this function, therefore there is no need to check
 * it again. We assume that this function is called only when
 * ctrlType belongs to a CS control packet.
 *
 * This function also builds the response packet for the control
 * packets that require a response
 *
 * @design      BLE_LOKI-506
 *  parameters
 *
 * @param       ctrlType - Type of the received CS control packet
 * @param       connPtr - Pointer to connection
 * @param       pBuf  - Pointer to the data in the control packet
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status
 *              CS_STATUS_INVALID_CONN_PTR connPtr is NULL
 *              CS_STATUS_INVALID_BUFFER pBuf is NULL
 *              CS_STATUS_UNSUPPORTED_FEATURE CS is not supported
 *              CS_STATUS_INSUFFICIENT_MEMORY insufficient memory to setup ctrl
 *              CS_STATUS_SUCCESS - if packet was processed successfully
 * @note it is assumed that this API is used only when ctrlType is a CS ctrl
 * opcode.
 */
csStatus_e llCsProcessCsControlPacket(uint8 ctrlType, llConnState_t* connPtr,
                                    uint8* pBuf);

/*******************************************************************************
 * @fn          llCsProcessCsCtrlProcedures
 *
 * @brief       Process CS Control Procedure Depending on type.
 * This processes the packets to be transmitted.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connPtr - connection information pointer
 * @param       ctrlPkt - control packet opcode
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status
 *              CS_STATUS_INVALID_CONN_PTR - if connPts is NULL
 *              CS_STATUS_CONNECTION_TERMINATED - if connection got terminated
 *              CS_STATUS_SUCCESS - if packet was processed successfully
 * @note it is assumed that this API is used only when ctrlType is a CS ctrl
 * opcode.
 */
uint8 llCsProcessCsCtrlProcedures(llConnState_t* connPtr, uint8 ctrlPkt);
