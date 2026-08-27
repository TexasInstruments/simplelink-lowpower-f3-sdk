/*
 * Copyright (c) 2025, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Texas Instruments Incorporated nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file  npi_tl_serial_mux.c
 * @brief NPI Transport Layer for the TI Combined Serial MUX (NPI_USE_UART_MUX).
 *
 * This file implements the NPI transport interface using the Combined Serial
 * MUX layer instead of a dedicated UART or SPI port.  The MUX task owns the
 * physical UART; this transport registers as the BLE-NLI receive callback and
 * sends outbound HCI data through the MUX TX queue.
 *
 * Compared with the shim-based approach (mux_npi_uart_shim.h +
 * mux_virt_uart.c), this implementation talks directly to the MUX task API
 * without the UART2 abstraction layer:
 *
 *   RX path:
 *     MUX task → NPITLSerialMux_muxRxCb(buf, len)
 *         → memcpy to TransportRxBuf
 *         → npiTransmitCB(rxLen, 0)       [NPI layer reads synchronously]
 *         → TransportRxLen reset           [re-arms for next packet]
 *
 *   TX path:
 *     NPITLSerialMux_writeTransport(len)
 *         → MuxTask_sendPacket(MUX_NLI_BLE, TransportTxBuf, len)
 *         → npiTransmitCB(0, len)          [TX completion notification]
 *
 * Assumptions
 * -----------
 *   - NPI_FLOW_CTRL == 0  (no hardware flow control).
 *   - MuxTask_registerRxCb() is called (via initializeTransport) BEFORE
 *     MuxTask_create() so the callback is installed before the MUX task
 *     starts processing inbound frames.
 *   - The NPI layer reads TransportRxBuf synchronously inside npiTransmitCB,
 *     i.e. before it returns.  This is consistent with the UART transport
 *     behaviour (NPI_FLOW_CTRL == 0 path in npi_tl_uart.c).
 */

#if (defined(HCI_TL_FULL) || defined(PTM_MODE))

#include <string.h>
#include <stdint.h>

#include "ti/ble/stack_util/icall/app/icall.h"
#include "ti/ble/stack_util/comdef.h"

#include "ti/dmm/combined_serial/mux_common.h"
#include "ti/dmm/combined_serial/embedded/mux_task_app.h"

#include "npi_tl_serial_mux.h"

/*---------------------------------------------------------------------------
 * Module-private state
 *--------------------------------------------------------------------------*/

/*! NPI receive buffer pointer (set at init; owned by npi_tl.c) */
static char *TransportRxBuf;

/*! NPI transmit buffer pointer (set at init; owned by npi_tl.c) */
static char *TransportTxBuf;

/*! Number of valid bytes currently in TransportRxBuf */
static uint16 TransportRxLen = 0;

/*! Number of bytes most recently enqueued for transmission */
static uint16 TransportTxLen = 0;

/*! NPI transport completion callback (NPITL_transmissionCallBack) */
static npiCB_t npiTransmitCB = NULL;

/*---------------------------------------------------------------------------
 * Forward declarations
 *--------------------------------------------------------------------------*/

static void NPITLSerialMux_muxRxCb(const uint8_t *buf, uint16_t len);

/*===========================================================================
 * NPITLSerialMux_initializeTransport
 *==========================================================================*/

void NPITLSerialMux_initializeTransport(char *tRxBuf, char *tTxBuf,
                                        npiCB_t npiCBack)
{
    TransportRxBuf = tRxBuf;
    TransportTxBuf = tTxBuf;
    npiTransmitCB  = npiCBack;

    /*
     * Register the BLE-NLI receive callback with the MUX task.
     * MuxTask_create() is called from main() before vTaskStartScheduler();
     * this registration happens later, from the NPI task context, which is
     * safe because the BLE host will not send HCI commands before the BLE
     * stack has fully initialised.
     */
    MuxTask_registerRxCb(MUX_NLI_BLE, NPITLSerialMux_muxRxCb);

    /* Arm the first receive (reset the RX accumulator). */
    NPITLSerialMux_readTransport();
}

/*===========================================================================
 * NPITLSerialMux_readTransport
 *
 * Re-arms the transport to accept the next inbound packet.  Called by
 * npi_tl.c after it has finished consuming TransportRxBuf.
 *==========================================================================*/

void NPITLSerialMux_readTransport(void)
{
    ICall_CSState key = ICall_enterCriticalSection();
    TransportRxLen = 0;
    ICall_leaveCriticalSection(key);
}

/*===========================================================================
 * NPITLSerialMux_writeTransport
 *
 * Enqueues len bytes from TransportTxBuf for the MUX BLE-NLI TX channel and
 * immediately notifies the NPI layer of completion.
 *
 * MuxTask_sendPacket() copies the payload into the TX queue so
 * TransportTxBuf may be reused by the caller once this function returns.
 *==========================================================================*/

uint16 NPITLSerialMux_writeTransport(uint16 len)
{
    ICall_CSState key = ICall_enterCriticalSection();
    TransportTxLen = len;
    ICall_leaveCriticalSection(key);

    MuxErr_t err = MuxTask_sendPacket(MUX_NLI_BLE,
                                      (const uint8_t *)TransportTxBuf,
                                      len);
    if (err != MUX_SUCCESS)
    {
        key = ICall_enterCriticalSection();
        TransportTxLen = 0;
        ICall_leaveCriticalSection(key);
        return 0;
    }

    /* Notify NPI that the TX transfer is complete. */
    if (npiTransmitCB)
    {
        npiTransmitCB(0, TransportTxLen);
    }

    return TransportTxLen;
}

/*===========================================================================
 * NPITLSerialMux_stopTransfer
 *
 * No-op: hardware flow control is not used (NPI_FLOW_CTRL == 0).
 *==========================================================================*/

void NPITLSerialMux_stopTransfer(void)
{
}

/*===========================================================================
 * NPITLSerialMux_handleMrdyEvent
 *
 * No-op: MRDY signalling is not used (NPI_FLOW_CTRL == 0).
 *==========================================================================*/

void NPITLSerialMux_handleMrdyEvent(void)
{
}

/*===========================================================================
 * NPITLSerialMux_muxRxCb  (static — MuxStackRxCb_t)
 *
 * Called by the MUX task whenever a decoded BLE-NLI packet arrives from the
 * Linux host.  Copies the payload into TransportRxBuf and notifies the NPI
 * layer via npiTransmitCB.
 *
 * The NPI layer reads TransportRxBuf synchronously inside npiTransmitCB
 * (consistent with NPI_FLOW_CTRL == 0 behaviour in npi_tl_uart.c), so
 * TransportRxLen can be safely reset to re-arm immediately after the callback
 * returns — just as npi_tl_uart.c resets it and re-posts UART2_read().
 *
 * If the incoming packet exceeds MAX_PDU_SIZE, the excess bytes are
 * silently dropped.  MAX_PDU_SIZE matches the NPI RX buffer size declared
 * by npi_tl.c (NPI_TL_BUF_SIZE), avoiding a dependency on npi_config.h.
 *==========================================================================*/

static void NPITLSerialMux_muxRxCb(const uint8_t *buf, uint16_t len)
{
    if ((buf == NULL) || (len == 0U))
    {
        return;
    }

    ICall_CSState key = ICall_enterCriticalSection();

    uint16_t copy = (len <= (uint16_t)MAX_PDU_SIZE) ? len
                                                    : (uint16_t)MAX_PDU_SIZE;
    memcpy(TransportRxBuf, buf, copy);
    TransportRxLen = copy;

    ICall_leaveCriticalSection(key);

    /*
     * Deliver to the NPI layer.  The NPI layer reads TransportRxBuf
     * synchronously before npiTransmitCB returns.
     */
    if (npiTransmitCB)
    {
        npiTransmitCB(TransportRxLen, 0);
    }

    /* Re-arm for the next inbound packet (mirrors UART readCallBack). */
    key = ICall_enterCriticalSection();
    TransportRxLen = 0;
    ICall_leaveCriticalSection(key);
}

#endif /* HCI_TL_FULL || PTM_MODE */
