/******************************************************************************

 @file  npi_frame_hci.c

 @brief This file contains the Network Processor Interface (NPI) data frame
        specific function implementations for the MT serial interface.

 Group: WCS, LPC, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2015-2025, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

// ****************************************************************************
// includes
// ****************************************************************************

#include <string.h>
#ifndef FREERTOS
#include <xdc/std.h>
#endif

#include "ti/ble/app_util/npi/npi_rxbuf.h"
#include "ti/ble/app_util/npi/npi_frame.h"
#include "ti/ble/stack_util/osal/osal.h"
#include "ti/ble/stack_util/icall/app/icall.h"
#include <ti_drivers_config.h>
#include "ti/ble/stack_util/comdef.h"
#include "ti/ble/controller/hci/hci_tl.h"

#include "ti/ble/app_util/npi/npi_ble.h"

#include "ti/ble/stack_util/icall/app/icall_ble_api.h"

// ****************************************************************************
// defines
// ****************************************************************************

#define NPIFRAMEHCI_CMD_PKT_HDR_LEN 4
#define NPIFRAMEHCI_EXTENDED_CMD_PKT_HDR_LEN 5
#define NPIFRAMEHCI_DATA_PKT_HDR_LEN 5
#define NPIFRAMEHCI_SPNP_CMD_PKT_HDR_LEN 5

// States for Command and Data packet parser
#define NPIFRAMEHCI_STATE_PKT_TYPE              0
//
#define NPIFRAMEHCI_CMD_STATE_OPCODE0           1
#define NPIFRAMEHCI_CMD_STATE_OPCODE1           2
#define NPIFRAMEHCI_CMD_STATE_LENGTH0           3
#define NPIFRAMEHCI_CMD_STATE_HEADER            4
#define NPIFRAMEHCI_CMD_STATE_DATA              5
//
#define NPIFRAMEHCI_DATA_STATE_HANDLE0          6
#define NPIFRAMEHCI_DATA_STATE_HANDLE1          7
#define NPIFRAMEHCI_DATA_STATE_LENGTH0          8
#define NPIFRAMEHCI_DATA_STATE_HEADER           9
#define NPIFRAMEHCI_DATA_STATE_DATA             10
//
#define NPIFRAMEHCI_STATE_FLUSH                 11

#define NPIFRAMEHCI_SPNP_CMD_STATE_OPCODE0 12
#define NPIFRAMEHCI_SPNP_CMD_STATE_OPCODE1 13
#define NPIFRAMEHCI_SPNP_CMD_STATE_LENGTH0 14
#define NPIFRAMEHCI_SPNP_CMD_STATE_LENGTH1 15
#define NPIFRAMEHCI_SPNP_CMD_STATE_DATA 16

// ****************************************************************************
// typedefs
// ****************************************************************************

//*****************************************************************************
// globals
//*****************************************************************************
npiIncomingFrameCBack_t incomingFrameCBFunc = NULL;

//! \name HCI serial collection globals
//@{
static uint8_t state = NPIFRAMEHCI_STATE_PKT_TYPE;
static uint8_t PKT_Token = 0;
static uint16_t LEN_Token = 0;
static uint16_t OPCODE_Token = 0;
static uint16_t bytesToFlush = 0;
#ifdef NPI_RAW
hciRawData_t *pHciRawMsg;
static uint16_t payloadOffset = 0;
#else
static hciPacket_t *pCmdMsg = NULL;
static hciDataPacket_t *pDataMsg = NULL;
#endif

static uint16_t HANDLE_Token = 0;
static uint16_t tempDataLen = 0;
static uint16_t headerLength = 0;

extern ICall_EntityID npiAppEntityID;

//*****************************************************************************
// function prototypes
//*****************************************************************************

// ----------------------------------------------------------------------------
//! \brief      Initialize Frame module with NPI callbacks.
//!
//! \param[in]  incomingFrameCB   Call back for complete inbound (from host)
//!                               messages
//!
//! \return     void
// ----------------------------------------------------------------------------
void NPIFrame_initialize(npiIncomingFrameCBack_t incomingFrameCB)
{
    incomingFrameCBFunc = incomingFrameCB;
}

// ----------------------------------------------------------------------------
//! \brief      Bundles message into HCI Transport Layer frame.  When complete,
//!             this function should return the framed message buffer via the
//!             callback function above: npiFrameCompleteCBack_t.
//!
//!             NOTE: The passed in container pMsg is used to return the framed
//!             message and should not be free'd!
//!
//! \param[in]  pData     Pointer to NPI container w/HCI message buffer.
//!
//! \return     void
// ----------------------------------------------------------------------------
NPIMSG_msg_t *NPIFrame_frameMsg(uint8_t *pMsg)
{
    NPIMSG_msg_t *npiMsg = NULL;
    npiPkt_t *pFromStackMsg = NULL;
    if (NULL != pMsg)
    {
        // Cast generic array into appropriate container
        // All messages we receive from stack will be of type npiPacket_t
        pFromStackMsg = (npiPkt_t *)pMsg;

        // Alloc NPI Message
        npiMsg = (NPIMSG_msg_t *)ICall_mallocLimited(sizeof(NPIMSG_msg_t));

        if (npiMsg)
        {
            // All HCI messages are Asynchronous
            npiMsg->msgType = NPIMSG_Type_ASYNC;

            // Allocate memory for container, and pkt payload
            npiMsg->pBuf = (uint8_t *)ICall_allocMsgLimited(pFromStackMsg->pktLen);
            npiMsg->pBufSize = pFromStackMsg->pktLen;

            if (npiMsg->pBuf)
            {
                // Payload
                memcpy(npiMsg->pBuf,pFromStackMsg->pData,pFromStackMsg->pktLen);
            }
            else
            {
                ICall_free(npiMsg);
                npiMsg = NULL;
            }
        }

        // Free original message and send new NPIMSG to a TX Queue
        ICall_freeMsg(pMsg);
    }

    return npiMsg;
}

#ifdef NPI_RAW
// -----------------------------------------------------------------------------
//! \brief      Allocate npiPkt_t pointer from raw data.
//!
//! \return     void
// -----------------------------------------------------------------------------
uint8_t* NPIFrame_createNpiPkt(uint8_t* pMsg)
{
    npiPkt_t* pNpiPkt = NULL;

    if (pMsg)
    {
        hciRawData_t *pHciData = (hciRawData_t *)pMsg;
        // Create NPI message from HCI packet.
        pNpiPkt = (npiPkt_t *)ICall_allocMsgLimited(sizeof(npiPkt_t));
        if( pNpiPkt != NULL)
        {
            pNpiPkt->hdr.event = pHciData->pData[0];
            pNpiPkt->hdr.status = 0xff;
            pNpiPkt->pktLen = pHciData->pktLen;
            pNpiPkt->pData  = pHciData->pData;
        }
    }

     return (uint8_t *)pNpiPkt;
}

// ----------------------------------------------------------------------------
//! \brief  HCI command packet format:
//!         Packet Type + Command opcode + length + command payload
//!         | 1 octet   |      2         |   1   |      n        |
//!
//!         HCI data packet format:
//!         Packet Type +   Conn Handle  + length + data payload
//!         | 1 octet   |      2         |   2   |      n      |
//!
//! \return     void
// ----------------------------------------------------------------------------
void NPIFrame_collectFrameData(void)
{
    uint16_t rxBufLen = 0;
    uint8_t ch;

    while (NPIRxBuf_GetRxBufCount())
    {
        NPIRxBuf_ReadFromRxBuf(&ch, 1);
        switch (state)
        {
            // Packet Type
            case NPIFRAMEHCI_STATE_PKT_TYPE:
                PKT_Token = ch;
                switch (ch)
                {
                    case HCI_CMD_PACKET:
                    case HCI_EXTENDED_CMD_PACKET:
                        state = NPIFRAMEHCI_CMD_STATE_OPCODE0;
                        break;
                    case HCI_ACL_DATA_PACKET:
                    case HCI_SCO_DATA_PACKET:
                        state = NPIFRAMEHCI_DATA_STATE_HANDLE0;
                        break;
                    default:
                        // If we do not receive a legal packet type we will
                        // skip this byte.
                        state = NPIFRAMEHCI_STATE_PKT_TYPE;
                }
                break;

            // Command Opcode Byte 0
            case NPIFRAMEHCI_CMD_STATE_OPCODE0:
                OPCODE_Token = ch;
                state = NPIFRAMEHCI_CMD_STATE_OPCODE1;
                break;

            // Command Opcode Byte 1
            case NPIFRAMEHCI_CMD_STATE_OPCODE1:
                OPCODE_Token |= ((uint16_t)ch << 8);
                if (PKT_Token == HCI_EXTENDED_CMD_PACKET)
                {
                    state = NPIFRAMEHCI_CMD_STATE_LENGTH0;
                }
                else
                {
                    state = NPIFRAMEHCI_CMD_STATE_HEADER;
                }
                break;

            // Extented Command Payload Length 0
            case NPIFRAMEHCI_CMD_STATE_LENGTH0:
                LEN_Token = ch;
                state = NPIFRAMEHCI_CMD_STATE_HEADER;
                break;

            // Command Payload Length
            case NPIFRAMEHCI_CMD_STATE_HEADER:
                if (PKT_Token == HCI_EXTENDED_CMD_PACKET)
                {
                    headerLength = NPIFRAMEHCI_EXTENDED_CMD_PKT_HDR_LEN;
                    LEN_Token |= ((uint16_t)ch << 8);
                }
                else
                {
                    headerLength = NPIFRAMEHCI_CMD_PKT_HDR_LEN;
                    LEN_Token = (uint16_t)ch;
                }

                tempDataLen = 0;
                payloadOffset = 0;

                if (LEN_Token > NPI_TL_BUF_SIZE)
                {
                    // Length of packet is greater than RX Buf size. Flush packet
                    bytesToFlush = LEN_Token;
                    state = NPIFRAMEHCI_STATE_FLUSH;
                }
                else
                {
                    /* Allocate memory for the data */
                    pHciRawMsg = (hciRawData_t *) ICall_allocMsg( sizeof(hciRawData_t) + headerLength + LEN_Token );

                    if (pHciRawMsg)
                    {
                        pHciRawMsg->pktLen = LEN_Token + headerLength;
                        pHciRawMsg->pData = (uint8_t *)(pHciRawMsg + 1);

                        pHciRawMsg->pData[payloadOffset++] = PKT_Token;
                        pHciRawMsg->pData[payloadOffset++] = ((uint8_t *)&OPCODE_Token)[0];
                        pHciRawMsg->pData[payloadOffset++] = ((uint8_t *)&OPCODE_Token)[1];
                        if (PKT_Token == HCI_EXTENDED_CMD_PACKET)
                        {
                          pHciRawMsg->pData[payloadOffset++] = LO_UINT16(LEN_Token);
                          pHciRawMsg->pData[payloadOffset++] = HI_UINT16(LEN_Token);;
                        }
                        else
                        {
                          pHciRawMsg->pData[payloadOffset++] = LEN_Token;

                        }

                        // check if a Controller Link Layer VS command
                        if (((OPCODE_Token >> 10) == VENDOR_SPECIFIC_OGF) &&
                            (((OPCODE_Token >> 7) & 0x07) != HCI_OPCODE_CSG_LINK_LAYER))
                        {
                            // this is a vendor specific command
                            // so strip the OGF (i.e. the most significant 6
                                                    // bits of the opcode)
                          pHciRawMsg->pData[2] &= 0x03;
                        }

                        state = NPIFRAMEHCI_CMD_STATE_DATA;

                        if (LEN_Token == 0)
                        {
                            // No Payload to read so go ahead and send to Stack
                            if (incomingFrameCBFunc)
                            {
                                incomingFrameCBFunc(sizeof(hciRawData_t) + pHciRawMsg->pktLen,
                                                    (uint8_t *)pHciRawMsg, NPIMSG_Type_ASYNC);
                            }
                            state = NPIFRAMEHCI_STATE_PKT_TYPE;
                        }
                    }
                }
                break;

            // Command Payload
            case NPIFRAMEHCI_CMD_STATE_DATA:
                // ch is the first char of the payload - we have just received it while this function was called.
                // The rest of the payload will be copied by NPIRxBuf_ReadFromRxBuf()
                pHciRawMsg->pData[payloadOffset++] = ch;
                tempDataLen++;

                rxBufLen = NPIRxBuf_GetRxBufCount();

                /* If the remain of the data is there, read them all,
                   otherwise, just read enough */
                if (rxBufLen <= LEN_Token - tempDataLen)
                {
                    NPIRxBuf_ReadFromRxBuf(&pHciRawMsg->pData[payloadOffset], rxBufLen);
                    payloadOffset  += rxBufLen;
                    tempDataLen += rxBufLen;
                }
                else
                {
                    NPIRxBuf_ReadFromRxBuf(&pHciRawMsg->pData[payloadOffset], LEN_Token - tempDataLen);

                    payloadOffset  += (LEN_Token - tempDataLen);
                    tempDataLen += (LEN_Token - tempDataLen);
                }

                /* If number of bytes read is equal to data length,
                   send msg back to NPI */
                if (tempDataLen == LEN_Token)
                {
                    if (incomingFrameCBFunc)
                    {
                        incomingFrameCBFunc(sizeof(hciRawData_t) + pHciRawMsg->pktLen,
                                            (uint8_t *)pHciRawMsg, NPIMSG_Type_ASYNC);
                    }
                    state = NPIFRAMEHCI_STATE_PKT_TYPE;
                }
                break;

            // Data Handle Byte 0
            case NPIFRAMEHCI_DATA_STATE_HANDLE0:
                HANDLE_Token = ch;
                state = NPIFRAMEHCI_DATA_STATE_HANDLE1;
                break;

            // Data Handle Byte 1
            case NPIFRAMEHCI_DATA_STATE_HANDLE1:
                HANDLE_Token |= ((uint16_t)ch << 8);
                state = NPIFRAMEHCI_DATA_STATE_LENGTH0;
                break;

            // Data Len Byte 0
            case NPIFRAMEHCI_DATA_STATE_LENGTH0:
                LEN_Token = ch;
                state = NPIFRAMEHCI_DATA_STATE_HEADER;
                break;

            // Data Len Byte 1
            case NPIFRAMEHCI_DATA_STATE_HEADER:
                LEN_Token |= ((uint16_t)ch << 8);
                tempDataLen = 0;
                payloadOffset = 0;

                if (LEN_Token > NPI_TL_BUF_SIZE)
                {
                    // Length of packet is greater than RX Buf size. Flush packet
                    bytesToFlush = LEN_Token;
                    state = NPIFRAMEHCI_STATE_FLUSH;
                }
                else
                {
                    /* Allocate memory for the data */
                    pHciRawMsg = (hciRawData_t *) ICall_allocMsg( sizeof(hciRawData_t) +
                                                                   NPIFRAMEHCI_DATA_PKT_HDR_LEN + LEN_Token );

                    if (pHciRawMsg)
                    {
                        pHciRawMsg->pData = (uint8_t *)(pHciRawMsg + 1);
                        pHciRawMsg->pktLen = LEN_Token + NPIFRAMEHCI_DATA_PKT_HDR_LEN;

                        pHciRawMsg->pData[payloadOffset++] = PKT_Token;
                        pHciRawMsg->pData[payloadOffset++] = LO_UINT16(HANDLE_Token);
                        pHciRawMsg->pData[payloadOffset++] = HI_UINT16(HANDLE_Token);
                        pHciRawMsg->pData[payloadOffset++] = LO_UINT16(LEN_Token);
                        pHciRawMsg->pData[payloadOffset++] = HI_UINT16(LEN_Token);

                        // Set pData to the first byte after the hciPacket bytes
                        state = NPIFRAMEHCI_DATA_STATE_DATA;

                        if (LEN_Token == 0)
                        {
                            if (incomingFrameCBFunc)
                            {
                                incomingFrameCBFunc(sizeof(hciRawData_t) + pHciRawMsg->pktLen,
                                                    (uint8_t *)pHciRawMsg, NPIMSG_Type_ASYNC);
                            }
                            state = NPIFRAMEHCI_STATE_PKT_TYPE;
                        }
                    }
                    else
                    {
                        state = NPIFRAMEHCI_STATE_PKT_TYPE;
                        return;
                    }

                }
                break;

            // Data Payload
            case NPIFRAMEHCI_DATA_STATE_DATA:
                // ch is the first char of the payload - we have just received it while this function was called.
                // The rest of the payload will be copied by NPIRxBuf_ReadFromRxBuf()
                pHciRawMsg->pData[payloadOffset++] = ch;
                tempDataLen++;

                rxBufLen = NPIRxBuf_GetRxBufCount();

                /* If the remain of the data is there, read them all,
                   otherwise, just read enough */
                if (rxBufLen <= LEN_Token - tempDataLen)
                {
                    NPIRxBuf_ReadFromRxBuf(&pHciRawMsg->pData[payloadOffset],
                                             rxBufLen);

                    tempDataLen += rxBufLen;
                    payloadOffset += rxBufLen;
                }
                else
                {
                    NPIRxBuf_ReadFromRxBuf(&pHciRawMsg->pData[payloadOffset],
                                             LEN_Token - tempDataLen);

                    tempDataLen += (LEN_Token - tempDataLen);
                    payloadOffset += (LEN_Token - tempDataLen);
                }

                /* If number of bytes read is equal to data length,
                   send msg back to NPI */
                if (tempDataLen == LEN_Token)
                {
                    if (incomingFrameCBFunc)
                    {
                        incomingFrameCBFunc(sizeof(hciRawData_t) + pHciRawMsg->pktLen,
                                            (uint8_t *)pHciRawMsg, NPIMSG_Type_ASYNC);
                    }
                    state = NPIFRAMEHCI_STATE_PKT_TYPE;
                }
                break;
            case NPIFRAMEHCI_STATE_FLUSH:
                bytesToFlush--;

                if (bytesToFlush == 0)
                {
                    // Flushed all bytes in frame. Return to initial state to
                    // parse next frame
                    state = NPIFRAMEHCI_STATE_PKT_TYPE;
                }

                break;
            default:
                break;
        }
    }
    return;
}
#else //NPI_RAW
// ----------------------------------------------------------------------------
//! \brief  HCI command packet format:
//!         Packet Type + Command opcode + length + command payload
//!         | 1 octet   |      2         |   1   |      n        |
//!
//!         HCI data packet format:
//!         Packet Type +   Conn Handle  + length + data payload
//!         | 1 octet   |      2         |   2   |      n      |
//!
//! \return     void
// ----------------------------------------------------------------------------
void NPIFrame_collectFrameData(void)
{
    uint16_t rxBufLen = 0;
    uint8_t ch;

    while (NPIRxBuf_GetRxBufCount())
    {
        NPIRxBuf_ReadFromRxBuf(&ch, 1);
        switch (state)
        {
            // Packet Type
            case NPIFRAMEHCI_STATE_PKT_TYPE:
                PKT_Token = ch;
                switch (ch)
                {
                    case HCI_CMD_PACKET:
                    case HCI_EXTENDED_CMD_PACKET:
                        state = NPIFRAMEHCI_CMD_STATE_OPCODE0;
                        break;
                    case HCI_ACL_DATA_PACKET:
                    case HCI_SCO_DATA_PACKET:
                        state = NPIFRAMEHCI_DATA_STATE_HANDLE0;
                        break;
                    default:
                        // If we do not receive a legal packet type we will
                        // skip this byte.
                        state = NPIFRAMEHCI_STATE_PKT_TYPE;
                }
                break;

            // Command Opcode Byte 0
            case NPIFRAMEHCI_CMD_STATE_OPCODE0:
                OPCODE_Token = ch;
                state = NPIFRAMEHCI_CMD_STATE_OPCODE1;
                break;

            // Command Opcode Byte 1
            case NPIFRAMEHCI_CMD_STATE_OPCODE1:
                OPCODE_Token |= ((uint16_t)ch << 8);
                if (PKT_Token == HCI_EXTENDED_CMD_PACKET)
                {
                    state = NPIFRAMEHCI_CMD_STATE_LENGTH0;
                }
                else
                {
                    state = NPIFRAMEHCI_CMD_STATE_HEADER;
                }
                break;

            // Extented Command Payload Length 0
            case NPIFRAMEHCI_CMD_STATE_LENGTH0:
                LEN_Token = ch;
                state = NPIFRAMEHCI_CMD_STATE_HEADER;
                break;

            // Command Payload Length
            case NPIFRAMEHCI_CMD_STATE_HEADER:
                if (PKT_Token == HCI_EXTENDED_CMD_PACKET)
                {
                    headerLength = NPIFRAMEHCI_EXTENDED_CMD_PKT_HDR_LEN;
                    LEN_Token |= ((uint16_t)ch << 8);
                }
                else
                {
                    headerLength = NPIFRAMEHCI_CMD_PKT_HDR_LEN;
                    LEN_Token = (uint16_t)ch;
                }

                tempDataLen = 0;

                if (LEN_Token > NPI_TL_BUF_SIZE)
                {
                    // Length of packet is greater than RX Buf size. Flush packet
                    bytesToFlush = LEN_Token;
                    state = NPIFRAMEHCI_STATE_FLUSH;
                }
                else
                {
                    /* Allocate memory for the data */
                    pCmdMsg = (hciPacket_t *) ICall_allocMsg(sizeof(hciPacket_t) +
                                              headerLength + LEN_Token);

                    if (pCmdMsg)
                    {
                        // Set pData to the first byte after the hciPacket bytes
                        pCmdMsg->pData = (uint8_t *)pCmdMsg + sizeof(hciPacket_t);

                        pCmdMsg->pData[0] = PKT_Token;
                        pCmdMsg->pData[1] = ((uint8_t *)&OPCODE_Token)[0];
                        pCmdMsg->pData[2] = ((uint8_t *)&OPCODE_Token)[1];
                        if (PKT_Token == HCI_EXTENDED_CMD_PACKET)
                        {
                           pCmdMsg->pData[3] = LO_UINT16(LEN_Token);
                           pCmdMsg->pData[4] = HI_UINT16(LEN_Token);;
                        }
                        else
                        {
                           pCmdMsg->pData[3] = LEN_Token;

                        }

                        // set header specific fields
                        pCmdMsg->hdr.status = 0xFF;

                        // check if a Controller Link Layer VS command
                        if (((OPCODE_Token >> 10) == VENDOR_SPECIFIC_OGF) &&
                            (((OPCODE_Token >> 7) & 0x07) != HCI_OPCODE_CSG_LINK_LAYER))
                        {
                            // this is a vendor specific command
                            pCmdMsg->hdr.event = HCI_EXT_CMD_EVENT;

                            // so strip the OGF (i.e. the most significant 6
                                                    // bits of the opcode)
                            pCmdMsg->pData[2] &= 0x03;
                        }
                        else // specification specific command
                        {
                            // this is a normal host-to-controller event
                            pCmdMsg->hdr.event = HCI_HOST_TO_CTRL_CMD_EVENT;
                        }


                        state = NPIFRAMEHCI_CMD_STATE_DATA;

                        if (LEN_Token == 0)
                        {
                            // No Payload to read so go ahead and send to Stack
                            if (incomingFrameCBFunc)
                            {
                                incomingFrameCBFunc(sizeof(hciPacket_t) + headerLength,
                                                      (uint8_t *)pCmdMsg, NPIMSG_Type_ASYNC);
                            }
                            state = NPIFRAMEHCI_STATE_PKT_TYPE;
                        }
                    }
                }
                break;

            // Command Payload
            case NPIFRAMEHCI_CMD_STATE_DATA:
                pCmdMsg->pData[headerLength + tempDataLen++] = ch;

                rxBufLen = NPIRxBuf_GetRxBufCount();

                /* If the remain of the data is there, read them all,
				   otherwise, just read enough */
                if (rxBufLen <= LEN_Token - tempDataLen)
                {
                    NPIRxBuf_ReadFromRxBuf(&pCmdMsg->pData[headerLength + tempDataLen],
                                               rxBufLen);

                    tempDataLen += rxBufLen;
                }
                else
                {
                    NPIRxBuf_ReadFromRxBuf(&pCmdMsg->pData[headerLength + tempDataLen],
                                               LEN_Token - tempDataLen);

                    tempDataLen += (LEN_Token - tempDataLen);
                }

                /* If number of bytes read is equal to data length,
				   send msg back to NPI */
                if (tempDataLen == LEN_Token)
                {
                    if (incomingFrameCBFunc)
                    {
                        incomingFrameCBFunc(sizeof(hciPacket_t) + headerLength + LEN_Token,
                                              (uint8_t *)pCmdMsg, NPIMSG_Type_ASYNC);
                    }
                    state = NPIFRAMEHCI_STATE_PKT_TYPE;
                }
                break;

            // Data Handle Byte 0
            case NPIFRAMEHCI_DATA_STATE_HANDLE0:
                HANDLE_Token = ch;
                state = NPIFRAMEHCI_DATA_STATE_HANDLE1;
                break;

            // Data Handle Byte 1
            case NPIFRAMEHCI_DATA_STATE_HANDLE1:
                HANDLE_Token |= ((uint16_t)ch << 8);
                state = NPIFRAMEHCI_DATA_STATE_LENGTH0;
                break;

            // Data Len Byte 0
            case NPIFRAMEHCI_DATA_STATE_LENGTH0:
                LEN_Token = ch;
                state = NPIFRAMEHCI_DATA_STATE_HEADER;
                break;

            // Data Len Byte 1
            case NPIFRAMEHCI_DATA_STATE_HEADER:
                LEN_Token |= ((uint16_t)ch << 8);
                tempDataLen = 0;

                if (LEN_Token > NPI_TL_BUF_SIZE)
                {
                    // Length of packet is greater than RX Buf size. Flush packet
                    bytesToFlush = LEN_Token;
                    state = NPIFRAMEHCI_STATE_FLUSH;
                }
                else
                {
                    /* Allocate memory for the data */
                    pDataMsg = (hciDataPacket_t *) ICall_allocMsg(sizeof(hciDataPacket_t) + LEN_Token);

                    if (pDataMsg)
                    {
                        pDataMsg->hdr.event = HCI_HOST_TO_CTRL_DATA_EVENT;
                        pDataMsg->hdr.status = 0xFF;

                        pDataMsg->pktType = PKT_Token;
                        // mask out PB and BC Flags
                        pDataMsg->connHandle = HANDLE_Token & 0x0FFF;
                        //isolate PB Flag
                        pDataMsg->pbFlag = (HANDLE_Token & 0x3000) >> 12;
                        pDataMsg->pktLen = LEN_Token;

                        // Set pData to the first byte after the hciPacket bytes
                        pDataMsg->pData = (uint8_t *)pDataMsg + sizeof(hciDataPacket_t);

                        state = NPIFRAMEHCI_DATA_STATE_DATA;

                        if (LEN_Token == 0)
                        {
                            if (incomingFrameCBFunc)
                            {
                                incomingFrameCBFunc(sizeof(hciDataPacket_t),
                                                      (uint8_t *)pDataMsg, NPIMSG_Type_ASYNC);
                            }
                            state = NPIFRAMEHCI_STATE_PKT_TYPE;
                        }
                    }
                    else
                    {
                        state = NPIFRAMEHCI_STATE_PKT_TYPE;
                        return;
                    }
                }
                break;

            // Data Payload
            case NPIFRAMEHCI_DATA_STATE_DATA:
                pDataMsg->pData[tempDataLen++] = ch;

                rxBufLen = NPIRxBuf_GetRxBufCount();

                /* If the remain of the data is there, read them all,
				   otherwise, just read enough */
                if (rxBufLen <= LEN_Token - tempDataLen)
                {
                    NPIRxBuf_ReadFromRxBuf(&pDataMsg->pData[tempDataLen],
                                             rxBufLen);

                    tempDataLen += rxBufLen;
                }
                else
                {
                    NPIRxBuf_ReadFromRxBuf(&pDataMsg->pData[tempDataLen],
                                             LEN_Token - tempDataLen);

                    tempDataLen += (LEN_Token - tempDataLen);
                }

                /* If number of bytes read is equal to data length,
				   send msg back to NPI */
                if (tempDataLen == LEN_Token)
                {
                    if (incomingFrameCBFunc)
                    {
                        incomingFrameCBFunc(sizeof(hciDataPacket_t) + LEN_Token,
                                              (uint8_t *)pDataMsg, NPIMSG_Type_ASYNC);
                    }
                    state = NPIFRAMEHCI_STATE_PKT_TYPE;
                }
                break;
            case NPIFRAMEHCI_STATE_FLUSH:
                bytesToFlush--;

                if (bytesToFlush == 0)
                {
                    // Flushed all bytes in frame. Return to initial state to
                    // parse next frame
                    state = NPIFRAMEHCI_STATE_PKT_TYPE;
                }

                break;
            default:
                break;
        }
    }
    return;
}
#endif //NPI_RAW
