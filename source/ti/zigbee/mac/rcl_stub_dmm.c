/******************************************************************************

 @file  rcl_stub_dmm.c

 @brief This file contains default stub implementations for the feature wrapper
        functions. These weak placeholder implementations do nothing by default,
        allowing the feature to remain disabled without causing any issues in
        the build process.
        When the feature is enabled, these stubs are replaced by the actual
        implementations directed by rcl_dmm.c.

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

#include <rcl_stub_dmm.h>

DMM_WEAK_FUNC RCL_CommandStatus OVRDE_RCL_Command_submit(RCL_Handle h, RCL_Command_Handle c)
{
    return RCL_Command_submit(h, c);
}

DMM_WEAK_FUNC RCL_CommandStatus OVRDE_RCL_Command_pend(RCL_Command_Handle c)
{
    return RCL_Command_pend(c);
}

DMM_WEAK_FUNC RCL_CommandStatus OVRDE_RCL_Command_stop(RCL_Command_Handle c, RCL_StopType stopType)
{
    return RCL_Command_stop(c, stopType);
}

DMM_WEAK_FUNC RCL_Handle OVRDE_RCL_open(RCL_Client* c, const LRF_Config* lrfConfig)
{
    return RCL_open(c, lrfConfig);
}

DMM_WEAK_FUNC void OVRDE_RCL_close(RCL_Handle h)
{
    RCL_close(h);
}

DMM_WEAK_FUNC void OVRDE_RCL_init(void)
{
    RCL_init();
}

DMM_WEAK_FUNC RCL_IEEE_UpdateResult OVRDE_RCL_IEEE_updateRxAction(RCL_CmdIeeeRxTx *cmd)
{
    return RCL_IEEE_updateRxAction(cmd);
}

DMM_WEAK_FUNC RCL_IEEE_UpdateResult OVRDE_RCL_IEEE_updateSourceMatchingTableShort(RCL_CmdIeeeRxTx *cmd, RCL_IEEE_SourceMatchingUpdate description, RCL_CmdIeee_PanIdAddr newPanIdAddr)
{
    return RCL_IEEE_updateSourceMatchingTableShort(cmd, description, newPanIdAddr);
}

DMM_WEAK_FUNC RCL_IEEE_UpdateResult OVRDE_RCL_IEEE_updateSourceMatchingTableExt(RCL_CmdIeeeRxTx *cmd, RCL_IEEE_SourceMatchingUpdate description, const uint64_t *newAddr)
{
    return RCL_IEEE_updateSourceMatchingTableExt(cmd, description, newAddr);
}

DMM_WEAK_FUNC RCL_IEEE_UpdateResult OVRDE_RCL_IEEE_updateTxPower(RCL_CmdIeeeRxTx *cmd, RCL_Command_TxPower newTxPower)
{
    return RCL_IEEE_updateTxPower(cmd, newTxPower);
}

DMM_WEAK_FUNC RCL_IEEE_AckEntryResult OVRDE_RCL_IEEE_enterAck(RCL_CmdIeeeRxTx *cmd, uint32_t *ackData, uint8_t numWords)
{
    return RCL_IEEE_enterAck(cmd, ackData, numWords);
}

DMM_WEAK_FUNC RCL_IEEE_AckEntryResult OVRDE_RCL_IEEE_cancelAck(RCL_CmdIeeeRxTx *cmd)
{
    return RCL_IEEE_cancelAck(cmd);
}

DMM_WEAK_FUNC size_t OVRDE_RCL_IEEE_readPartialFrame(RCL_CmdIeeeRxTx *cmd, RCL_Buffer_DataEntry *dataEntry, size_t entrySize)
{
    return RCL_IEEE_readPartialFrame(cmd, dataEntry, entrySize);
}

DMM_WEAK_FUNC RCL_CommandStatus OVRDE_RCL_IEEE_Tx_submit(RCL_CmdIeeeRxTx *cmd, RCL_CmdIeee_TxAction *txAction)
{
    return RCL_IEEE_Tx_submit(cmd, txAction);
}

DMM_WEAK_FUNC RCL_CommandStatus OVRDE_RCL_IEEE_Tx_stop(RCL_CmdIeeeRxTx *cmd, RCL_StopType stopType)
{
    return RCL_IEEE_Tx_stop(cmd, stopType);
}

/*****************************************************************************/
