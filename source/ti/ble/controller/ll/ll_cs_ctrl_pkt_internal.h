
/******************************************************************************

 @file  ll_cs_ctrl_pkt_internal.h

 @brief This file includes the internal functions used by ll_cs_ctrl_pkt mgr

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2026, Texas Instruments Incorporated

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
#ifndef LL_CS_CTRL_PKT_INTERNAL_H
#define LL_CS_CTRL_PKT_INTERNAL_H
/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll_cs_common.h"
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
struct csReq_t
{
    uint8   configId:6;
    uint8   rfu:2;
    uint16  connEventCount;
    uint32  offsetMin:24;
    uint32  offsetMax:24;
    uint16  maxProcedureDur;
    uint16  eventInterval;           /* units of connInt */
    uint8   subEventsPerEvent;       /* num of CS SubEvents in a CS Event */
    uint16  subEventInterval;        /* units 625 us*/
    uint32  subEventLen:24;          /* tune ub 1250 ms */
    uint16  procedureInterval;
    uint16  procedureCount;
    csACI_e ACI;
    uint8   preferredPeerAntenna;
    uint8   phy;
    int8    pwrDelta;
    uint8   txSnrI:4;
    uint8   txSnrR:4;
 } __attribute__((packed));
 // Typedef separated from packed struct due to MisraC
 typedef struct csReq_t csReq_t;

struct csRsp_t
{
    uint8   configId:6;
    uint8   rfu:2;
    uint16  connEventCount;
    uint32  offsetMin:24;
    uint32  offsetMax:24;
    uint16  eventInterval;           /* units of connInt */
    uint8   subEventsPerEvent;       /* num of CS SubEvents in a CS Event */
    uint16  subEventInterval;        /* units 625 us*/
    uint32  subEventLen:24;          /* tune ub 1250 ms */
    csACI_e ACI;
    uint8   phy;
    int8    pwrDelta;
} __attribute__((packed));
// Typedef separated from packed struct due to MisraC
typedef struct csRsp_t csRsp_t;

struct csInd_t
{
    uint8   configId:6;
    uint8   rfu:2;
    uint16  connEventCount;
    uint32  offset:24;
    uint16  eventInterval;     /* units of connInt */
    uint8   subEventsPerEvent; /* num of CS SubEvents in a CS Event */
    uint16  subEventInterval;  /* units 625 us*/
    uint32  subEventLen:24;       /* tune ub 1250 ms */
    csACI_e ACI;
    uint8   phy;
    int8    pwrDelta;
}__attribute__((packed));
// Typedef separated from packed struct due to MisraC
typedef struct csInd_t csInd_t;

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */

/*******************************************************************************
 * @fn          llCsProcessCsCtrlProcedure
 *
 * @brief       Process a CS Control Packet Procedure.
 * It either sets up and transmits the CS control packet, or dequeues it when
 * it's done. This API is called by llProcessMaster/SlaveControlProcedures
 * upon Last Command Done interrupt if a CS Control Packet is queued.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connPtr - connection information pointer
 * @param       ctrlPkt - control packet opcode
 * @param       procedure - procedure type
 * @param       payloadLen - Length of payload
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
uint8 llCsProcessCsCtrlProcedure(llConnState_t* connPtr, uint8 ctrlPkt,
                                 uint8 procedure, uint8 payloadLen);


/*******************************************************************************
 * @fn          llCsCtrlPktCheckACI
 *
 * @brief       Checks that an ACI value complies with capabilities.
 *              In addition, checks that the peer didn't modify
 *              our number of antennas derived by ACI
 *
 * input parameters
 *
 * @param       configId - CS Config Id
 * @param       connId - Connection Id
 * @param       peerACI - ACI value sent by the peer
 * @param       procACI - Recent agreed ACI value
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      CS_STATUS_UNEXPECTED_PARAMETER -
 *                  If ACI not in range of 0 to 7.
 *                  If own\peer number of antennas is less-than number of antennas
 *                  derived by ACI value.
 *                  If own\peer N_AP derived by ACI value is greater-than corresponds
 *                  max antenna paths.
 *                  If the peer modified our number of antennas.
 *
 *              CS_STATUS_SUCCESS - otherwise
 */
csStatus_e llCsCtrlPktCheckACI(uint8 configId, uint16 connId, csACI_e peerACI,
                               csACI_e procACI);

/*******************************************************************************
 * @fn          llCsNegotiateCsReq
 *
 * @brief       Negotiate a received CS_REQ packet.
 *              Validates the procedure parameters against local configuration
 *              and capabilities. May modify pEnable (offsets, ACI, event interval)
 *              to suggest acceptable values. The (possibly modified) pEnable is
 *              then serialized into the CS_RSP or CS_IND PDU.
 *
 * input parameters
 *
 * @param       connPtr  - connection pointer
 * @param       configId - CS configuration ID
 * @param       pEnable  - procedure data to validate and adjust (in/out)
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      CS_STATUS_SUCCESS if negotiation succeeded, error otherwise
 */
csStatus_e llCsNegotiateCsReq(llConnState_t* connPtr, uint8_t configId, csEnableProcedureCtrlData_t *pEnable);

/*******************************************************************************
 * @fn          llCsNegotiateCsRsp
 *
 * @brief       Negotiate a received CS_RSP packet.
 *              Validates the procedure parameters against local configuration
 *              and capabilities. May modify pEnable (connEventCount, offsets,
 *              subevent length/interval) to bring them within acceptable bounds.
 *              The (possibly modified) pEnable is then serialized into the CS_IND PDU.
 *
 * input parameters
 *
 * @param       connPtr  - connection pointer
 * @param       configId - CS configuration ID
 * @param       pEnable  - procedure data to validate and adjust (in/out)
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      CS_STATUS_SUCCESS if negotiation succeeded, error otherwise
 */
csStatus_e llCsNegotiateCsRsp(llConnState_t* connPtr, uint8_t configId, csEnableProcedureCtrlData_t *pEnable);

/*******************************************************************************
 * @fn          llCsConfirmInd
 *
 * @brief       Final stage of the CS procedure negotiation.
 *              Validates the CS_IND parameters received from the central against
 *              local configuration and capabilities (subevent length, ACI,
 *              subEventInterval, instant). Does not modify pEnable.
 *
 * input parameters
 *
 * @param       connPtr  - connection pointer
 * @param       configId - CS configuration ID
 * @param       pEnable  - procedure data parsed from CS_IND (read-only)
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      CS_STATUS_SUCCESS if the IND is acceptable, error otherwise
 */
csStatus_e llCsConfirmInd(llConnState_t* connPtr, uint8_t configId, const csEnableProcedureCtrlData_t *pEnable);

/*******************************************************************************
 * @fn          llCsReq2Data
 *
 * @brief       Serialize CS_REQ procedure data into OTA raw bytes
 *
 * input parameters
 *
 * @param       connId    - connection identifier
 * @param       configId  - CS configuration ID
 * @param       pEnable   - pointer to procedure data to serialize
 * @param       pData     - pointer to output buffer
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsReq2Data(uint16_t connId, uint8_t configId, const csEnableProcedureCtrlData_t *pEnable, uint8_t *pData);

/*******************************************************************************
 * @fn          llCsRsp2Data
 *
 * @brief       Serialize CS_RSP procedure data into OTA raw bytes
 *
 * input parameters
 *
 * @param       connId    - connection identifier
 * @param       configId  - CS configuration ID
 * @param       pEnable   - pointer to procedure data to serialize
 * @param       pData     - pointer to output buffer
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsRsp2Data(uint16_t connId, uint8_t configId, const csEnableProcedureCtrlData_t* pEnable, uint8_t* pData);

/*******************************************************************************
 * @fn          llCsInd2Data
 *
 * @brief       Serialize CS_IND procedure data into OTA raw bytes
 *
 * input parameters
 *
 * @param       connId    - connection identifier
 * @param       configId  - CS configuration ID
 * @param       pEnable   - pointer to procedure data to serialize
 * @param       pData     - pointer to output buffer
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsInd2Data(uint16_t connId, uint8_t configId, const csEnableProcedureCtrlData_t* pEnable, uint8_t* pData);

/*******************************************************************************
 * @fn          llCsSelectACI
 *
 * @brief       This function gets a suggested ACI value and re-suggests
 *              if needed, depends on the local task ID (peripheral or central)
 *
 * input parameters
 *
 * @param       connPtr  - Connection pointer
 * @param       suggestedACI - suggested ACI value as comes from the peer
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      ACI
 */
csACI_e llCsSelectACI(llConnState_t* connPtr, csACI_e suggestedACI);

/*******************************************************************************
 * @fn          llCsParseCsIndData
 *
 * @brief       Parse the data from the LL_CS_IND PKT
 *
 * input parameters
 *
 * @param       pEnable - procedure data to populate
 * @param       pBuf    - packet data buffer
 *
 * @design      BLE_LOKI-506
 *
 * output parameters
 *
 * @param       pEnable
 *
 * @return      None
 */
void llCsParseCsIndData(csEnableProcedureCtrlData_t *pEnable, uint8_t *pBuf);

/*******************************************************************************
 * @fn          llCsParseCsRspData
 *
 * @brief       Parse the data from the LL_CS_RSP PKT into local procedure data.
 *              Only RSP-specific fields are written; all others keep their
 *              existing values (caller should pre-load from DB if needed).
 *
 * input parameters
 *
 * @param       pEnable - procedure data to update with RSP fields
 * @param       pBuf    - packet data buffer
 *
 * output parameters
 *
 * @param       pEnable
 *
 * @return      None
 */
void llCsParseCsRspData(csEnableProcedureCtrlData_t *pEnable, uint8_t *pBuf);

/*******************************************************************************
 * @fn          llCsParseCsReqData
 *
 * @brief       Parse the data from the LL_CS_REQ PKT into local procedure data.
 *              configId is extracted separately via pConfigId.
 *
 * input parameters
 *
 * @param       pEnable   - procedure data to populate
 * @param       pBuf      - packet data buffer
 * @param       pConfigId - output: CS configuration ID from PDU
 *
 * output parameters
 *
 * @param       pEnable, pConfigId
 *
 * @return      None
 */
void llCsParseCsReqData(csEnableProcedureCtrlData_t *pEnable, uint8_t *pBuf, uint8_t *pConfigId);

/*******************************************************************************
 * @fn          llCsSetupCtrlPkt
 *
 * @brief       Build CS Control Packet
 * This function is used to build the different CS control packets
 * for sending over the air. The control packets are build
 * according to the Sepcification. We assume that the caller of
 * this function sends only ctrlType that belong to CS. The packet
 * that was build is added to the Tx Queue for the RF to transmit.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connPtr - connection information pointer
 * @param       ctrlPkt - control packet opcode
 * @param       ctrlLen - length of the packet
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status
 *              CS_STATUS_INVALID_CONN_PTR - if connPts is NULL
 *              CS_STATUS_CONNECTION_TERMINATED - if connection got terminated
 *              CS_STATUS_INVALID_PKT_LEN - if ctrlLen is invalid
 *              CS_STATUS_SUCCESS - if packet was processed successfully
 * @note it is assumed that this API is used only when ctrlType is a CS ctrl
 * opcode.
 */
csStatus_e llCsSetupCtrlPkt(llConnState_t* connPtr, uint8 ctrlType, uint8 ctrlLen);

/*******************************************************************************
 * @fn          llCsSetupConfigData
 *
 * @brief       Build the CS Config packet data
 *
 * input parameters
 *
 * @param       connId - connection identifier
 * @param       data - data pointer
 *
 * @design      BLE_LOKI-506
 *
 * output parameters
 *
 * @param       data - updated with the CS config packet data
 *
 * @return      status
 */
csStatus_e llCsSetupConfigData(uint16 connId, uint8* data);

/*******************************************************************************
 * @fn          llCsParseSecurityData
 *
 * @brief       Parses security data that is recieved from the ctrl packet
 *              LL_CS_SEC_REQ/RSP and sets it in the ll_cs_db
 *
 * input parameters
 *
 * @param       data - data pointer
 * @param       connId - connection identifier
 * @param       offset - data offset
 *
 * @design      BLE_LOKI-506
 *
 * output parameters
 *
 * @param       data - updated with the CS config packet data
 *
 * @return      None
 */
void llCsParseSecurityData(uint8* data, uint16 connId, uint8 offset);

/*******************************************************************************
 * @fn          llCsSetupFaeTblData
 *
 * @brief       Setup the data for the LL_CS_FAE_RSP PKT
 *
 * input parameters
 *
 * @param       data - data pointer
 *
 * @design      BLE_LOKI-506
 *
 * output parameters
 *
 * @param       data - updated with the FAE table
 *
 * @return      None
 */
void llCsSetupFaeTblData(uint8* data);

/*******************************************************************************
 * @fn          llCsSetupCSReq
 *
 * @brief       Setup the data for the LL_CS_REQ PKT
 *
 * input parameters
 *
 * @param       data - data pointer
 * @param       connPtr - connection pointer
 *
 * @design      BLE_LOKI-506
 *
 * output parameters
 *
 * @param       data
 *
 * @return      Success - if packet was setup, fail otherwise.
 */
csStatus_e llCsSetupCsReq(uint8* data, llConnState_t* connPtr);

/*******************************************************************************
 * @fn          llCsSetupCsRsp
 *
 * @brief       Setup the data for the LL_CS_RSP PKT
 *
 * input parameters
 *
 * @param       data - data pointer
 * @param       connId - connection identifier
 *
 * @design      BLE_LOKI-506
 *
 * output parameters
 *
 * @param       data
 *
 * @return      Success - if packet was setup, fail otherwise.
 */
csStatus_e llCsSetupCsRsp(uint8* data, uint16 connId);

/*******************************************************************************
 * @fn          llCsSetupCsInd
 *
 * @brief       Setup the data for the LL_CS_IND PKT
 *
 * input parameters
 *
 * @param       data - data pointer
 * @param       connId - connection identifier
 *
 * @design      BLE_LOKI-506
 *
 * output parameters
 *
 * @param       data
 *
 * @return      Success - if packet was setup, fail otherwise.
 */
csStatus_e llCsSetupCsInd(uint8* data, uint16 connId);

/*******************************************************************************
 * @fn          llCsSetupTerminateReqOrRsp
 *
 * @brief       Setup the data for the LL_CS_TERMINATE_REQ/RSP PKT
 *
 * input parameters
 *
 * @param       connId - connection identifier
 * @param       data - data pointer
 *
 * @design      BLE_LOKI-506
 *
 * output parameters
 *
 * @param       data
 *
 * @return      Success - if packet was setup, fail otherwise.
 */
csStatus_e llCsSetupTerminateReqOrRsp(uint16 connId, uint8* data);

/*******************************************************************************
 * @fn          llCsSetupChmInd
 *
 * @brief       Setup the data for the LL_CS_CHANNEL_IND PKT
 *
 * input parameters
 *
 * @param       connId         - connection identifier
 * @param       pChanMapIndPkt - updated with the channel map indication data
 *
 * @return      None
 */
void llCsSetupChmInd(uint16_t connId, uint8_t *pChanMapIndPkt);

#endif //LL_CS_CTRL_PKT_INTERNAL_H
