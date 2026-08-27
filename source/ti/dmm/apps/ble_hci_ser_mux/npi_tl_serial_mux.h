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
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file  npi_tl_serial_mux.h
 * @brief NPI transport layer for the TI Combined Serial MUX (NPI_USE_UART_MUX).
 *
 * Included by npi_tl.c when NPI_USE_UART_MUX is defined (via the
 * #elif NPI_USE_UART_MUX branch added to npi_tl.c).  Provides:
 *
 *   1. transportInit / transportRead / transportWrite / transportStopTransfer /
 *      transportMrdyEvent macro redirections — consumed by npi_tl.c in place
 *      of the UART or SPI equivalents.
 *
 *   2. npiCB_t typedef — same signature as the one in npi_tl_uart.h so that
 *      npi_tl.c compiles without pulling in any UART headers.
 *
 *   3. Function prototypes for NPITLSerialMux_* — implemented in
 *      npi_tl_serial_mux.c.
 *
 * Data flow (NPI_FLOW_CTRL == 0):
 *
 *   Host → physical UART → MUX task decode → NPITLSerialMux_muxRxCb()
 *       → memcpy to TransportRxBuf → npiTransmitCB(rxLen, 0) → NPI layer
 *
 *   NPI layer → NPITLSerialMux_writeTransport(len)
 *       → MuxTask_sendPacket(MUX_NLI_BLE, TransportTxBuf, len)
 *       → MUX task encode → physical UART TX → Host
 */

#ifndef NPI_TL_SERIAL_MUX_H
#define NPI_TL_SERIAL_MUX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ti/ble/stack_util/comdef.h"

/*---------------------------------------------------------------------------
 * transportInit/Read/Write/StopTransfer/MrdyEvent macro redirections
 *
 * npi_tl.h normally defines these when NPI_USE_UART or NPI_USE_SPI is set.
 * For NPI_USE_UART_MUX the definitions live here, included directly by
 * npi_tl.c, so npi_tl.h requires no modification.
 *--------------------------------------------------------------------------*/
#define transportInit         NPITLSerialMux_initializeTransport
#define transportRead         NPITLSerialMux_readTransport
#define transportWrite        NPITLSerialMux_writeTransport
#define transportStopTransfer NPITLSerialMux_stopTransfer
#define transportMrdyEvent    NPITLSerialMux_handleMrdyEvent

/*---------------------------------------------------------------------------
 * npiCB_t
 *
 * NPI transport completion callback.  Mirrors the identical typedef in
 * npi_tl_uart.h; redeclared here so that npi_tl_uart.h need not be included
 * when building with NPI_USE_UART_MUX.
 *--------------------------------------------------------------------------*/
typedef void (*npiCB_t)(uint16 Rxlen, uint16 Txlen);

/*---------------------------------------------------------------------------
 * Function prototypes
 *--------------------------------------------------------------------------*/

/*!
 * @brief  Initialise the serial-mux NPI transport.
 *
 * Stores the NPI RX/TX buffer pointers and the completion callback, then
 * registers NPITLSerialMux_muxRxCb with the MUX task as the BLE-NLI receive
 * callback.  Registers the BLE-NLI RX callback with the MUX task.
 * MuxTask_create() is called from application main() before the scheduler
 * starts; this function is called later from the NPI task context.
 *
 * @param tRxBuf   NPI receive buffer (owned by npi_tl.c).
 * @param tTxBuf   NPI transmit buffer (owned by npi_tl.c).
 * @param npiCBack Completion callback invoked after each RX or TX transfer.
 */
void NPITLSerialMux_initializeTransport(char *tRxBuf, char *tTxBuf,
                                        npiCB_t npiCBack);

/*!
 * @brief  Re-arm the transport for the next inbound packet.
 *
 * Resets TransportRxLen to zero.  Called by npi_tl.c after it has consumed
 * the previous receive buffer contents.
 */
void NPITLSerialMux_readTransport(void);

/*!
 * @brief  Transmit len bytes from the NPI TX buffer via the MUX task.
 *
 * Enqueues the payload for the MUX TX queue (MUX_NLI_BLE channel) and
 * synchronously fires npiTransmitCB(0, len) to signal completion.
 *
 * @param len  Number of bytes to transmit from TransportTxBuf.
 * @return     Number of bytes accepted (len on success, 0 on queue-full).
 */
uint16 NPITLSerialMux_writeTransport(uint16 len);

/*!
 * @brief  Stop any pending transfer (no-op: NPI_FLOW_CTRL == 0).
 */
void NPITLSerialMux_stopTransfer(void);

/*!
 * @brief  Handle MRDY de-assert event (no-op: NPI_FLOW_CTRL == 0).
 */
void NPITLSerialMux_handleMrdyEvent(void);

#ifdef __cplusplus
}
#endif

#endif /* NPI_TL_SERIAL_MUX_H */
