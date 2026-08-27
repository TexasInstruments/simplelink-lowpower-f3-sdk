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
 * @file  mux_uart.h
 * @brief UART2 driver integration for the embedded TI Combined Serial MUX.
 *
 * This module owns the UART2 handle used by the MUX layer.  It provides:
 *   - A blocking TX path for MuxTask_sendPacket() to write encoded HDLC
 *     frames (see mux_task_app.h).
 *   - A callback-driven RX path that decodes and dispatches received bytes
 *     directly from UART ISR context — there is no RX task and no
 *     intermediate chunk buffer.
 *
 * Architecture
 * ------------
 *
 *   any task context (TX)
 *       │
 *       ▼  MuxUart_write(buf, len)
 *   UART2_write() — blocking; caller must serialize (see mux_task_app.c's
 *   txLock)
 *       │
 *       ▼  wire (UART TX pin)
 *
 *   wire (UART RX pin)
 *       │
 *       ▼  DMA → driver internal ring buffer
 *   muxUart_rxCallback()  ← called from UART ISR
 *       │
 *       ▼  rxHandler(buf, count)   — called synchronously, still in ISR context
 *   incremental HDLC frame assembler → decode → per-NLI dispatch callback
 *   (see mux_task_app.c's MuxUart_RxHandler_t passed to MuxUart_open())
 *
 * Since rxHandler and everything it calls (the assembler, the decoder, and
 * every registered per-stack RX callback) runs synchronously inside the
 * UART ISR, all of that code must be ISR-safe: no blocking calls, no
 * FreeRTOS/ICall API that isn't its *FromISR variant, and bounded execution
 * time (it directly extends UART interrupt latency for every other
 * interrupt on the system while it runs).
 *
 * SysConfig note
 * --------------
 * The MUX UART must be configured as a **separate** UART2 instance in
 * SysConfig with both TX and RX pins assigned.  Pass the generated index
 * (e.g. CONFIG_MUX_UART) to MuxUart_open().
 *
 * The existing CONFIG_DISPLAY_UART instance (TX-only, display/debug) is
 * unrelated and must not be reused for the MUX.
 *
 * Thread-safety
 * -------------
 * - MuxUart_write() is not internally synchronized and may be called from
 *   any task context, but concurrent calls are not safe — the caller must
 *   serialize access. mux_task_app.c does this via gMuxTxTask.txLock, so
 *   every call in this codebase already goes through that; do not call
 *   MuxUart_write() directly from a new call site without equivalent
 *   serialization.
 * - MuxUart_open() and MuxUart_close() must be called before and after,
 *   respectively, any MuxUart_write() calls or RX activity.
 * - The rxHandler passed to MuxUart_open() runs in UART ISR context on
 *   every call — see the ISR-safety note above.
 */

#ifndef MUX_UART_H
#define MUX_UART_H

#include "../mux_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------
 * Configuration constants
 *--------------------------------------------------------------------------*/

/*!
 * Default baud rate for the MUX UART.
 * Override by passing a different value to MuxUart_open().
 * Common options: 115200, 921600, 1000000.
 */
#define MUX_UART_DEFAULT_BAUD   (115200UL)

/*!
 * Number of bytes requested per UART2_read() call (RX staging chunk size).
 * Should be >= the UART2 driver's internal DMA ring buffer size (default 32).
 * Larger values reduce callback frequency at the cost of slightly higher
 * per-callback time spent in ISR context (see the ISR-safety note above).
 */
#define MUX_UART_RX_CHUNK       (64U)

/*---------------------------------------------------------------------------
 * Types
 *--------------------------------------------------------------------------*/

/*!
 * @brief RX handler invoked synchronously from UART ISR context for every
 *        chunk of received bytes.
 *
 * @param buf  Pointer to the received bytes. Valid only for the duration of
 *             this call.
 * @param len  Number of valid bytes in @p buf. Always > 0.
 */
typedef void (*MuxUart_RxHandler_t)(const uint8_t *buf, uint16_t len);

/*---------------------------------------------------------------------------
 * API
 *--------------------------------------------------------------------------*/

/*!
 * @brief Open the UART2 instance and start background RX.
 *
 * Configures UART2 with:
 *   - 8N1 framing, no flow control
 *   - TX: blocking mode (MuxUart_write blocks until all bytes sent)
 *   - RX: callback mode  (muxUart_rxCallback fires for each completed read)
 *
 * Issues the first UART2_read() call to arm the RX path.  Subsequent reads
 * are re-armed automatically from within the RX callback.
 *
 * @param uartIndex  UART2 instance index from SysConfig (e.g. CONFIG_MUX_UART).
 * @param baudRate   Baud rate in bits/second (e.g. MUX_UART_DEFAULT_BAUD).
 * @param rxHandler  Called synchronously, in UART ISR context, with every
 *                   chunk of received bytes. Must be non-NULL and ISR-safe
 *                   (see the file-level ISR-safety note above).
 *
 * @return MUX_SUCCESS      UART opened and RX armed.
 * @return MUX_ERR_INVALID  NULL pointer argument.
 * @return MUX_ERR_UART     UART2_open() or initial UART2_read() failed.
 */
MuxErr_t MuxUart_open(uint8_t uartIndex, uint32_t baudRate,
                      MuxUart_RxHandler_t rxHandler);

/*!
 * @brief Close the UART2 instance and stop the RX path.
 *
 * Safe to call only when no further MuxUart_write() calls will be made and
 * RX activity has stopped.
 */
void MuxUart_close(void);

/*!
 * @brief Blocking write of @p len bytes to the MUX UART.
 *
 * Must be called only from task context (not from an ISR).
 * Blocks until all bytes have been transmitted or a hardware error occurs.
 *
 * @param buf  Pointer to the data to transmit (encoded HDLC frame).
 * @param len  Number of bytes to transmit.  Must be > 0.
 *
 * @return MUX_SUCCESS     All bytes transmitted.
 * @return MUX_ERR_INVALID NULL pointer or zero length.
 * @return MUX_ERR_UART    UART2_write() reported a hardware error.
 */
MuxErr_t MuxUart_write(const uint8_t *buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* MUX_UART_H */
