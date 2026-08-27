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
 * @file  mux_virt_uart.h
 * @brief Virtual UART-2 port (MUX_VIRT_PORT_0) for the TI Combined Serial MUX.
 *
 * Overview
 * --------
 * This module presents a software UART2 port to the NPI transport layer so
 * that NPI continues to call the standard UART2 API (UART2_open / UART2_read /
 * UART2_write) without any source changes.  At compile time, the build system
 * force-includes mux_npi_uart_shim.h (via -include) when building
 * npi_tl_uart.c only, which maps UART2_open → MuxVirtUart_npiOpen etc. at the
 * preprocessor level.  Calls for UART index CONFIG_UART2_0 (0) are handled by
 * the virtual port; all other indices fall through to the real hardware driver
 * via the standard UART2_* API.
 *
 * Data-flow: BLE channel
 * ----------------------
 *   Host → physical UART (CONFIG_MUX_UART) → MUX task decode
 *       → MuxVirtUart_rxNotify(buf, len)        [NLI_BLE RX callback]
 *       → ring buffer → NPI readCallback         [NPI: host→device path]
 *
 *   NPI writeCallback → UART2_write(virtual)
 *       → __wrap_UART2_write → MuxTask_sendPacket(MUX_NLI_BLE, buf, len)
 *       → MUX task encode → physical UART TX     [NPI: device→host path]
 *
 * Usage
 * -----
 *  1. Register the RX callback BEFORE creating the MUX task:
 *       MuxTask_registerRxCb(MUX_NLI_BLE, MuxVirtUart_rxNotify);
 *  2. Create the MUX task on the PHYSICAL uart index, not the virtual one:
 *       MuxTask_create(CONFIG_MUX_UART, MUX_UART_DEFAULT_BAUD);
 *  3. NPI calls UART2_open(CONFIG_UART2_0, ...) as usual; the virtual port
 *     intercepts it transparently.
 *
 * Thread safety
 * -------------
 * MuxVirtUart_rxNotify() is called from the MUX task context and uses
 * HwiP_disable/restore to guard shared state.  The __wrap_UART2_* functions
 * are called from the NPI task context.  The inReadCb flag prevents recursive
 * callback delivery.
 *
 * Limitation
 * ----------
 * Assumes NPI_FLOW_CTRL == 0 (no hardware flow control).  Under
 * NPI_FLOW_CTRL == 1, npi_tl_uart.c directly dereferences uartHandle->hwAttrs
 * for UARTCharAvailable(); that path is not interceptable by the virtual port.
 */

#ifndef MUX_VIRT_UART_H
#define MUX_VIRT_UART_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief MUX BLE-channel RX callback (MuxStackRxCb_t compatible).
 *
 * Called by the MUX task whenever a decoded NLI_BLE packet arrives from the
 * Linux host.  Appends @p len bytes from @p buf to the virtual UART RX ring
 * buffer and triggers delivery to the NPI read callback.
 *
 * Register before MuxTask_create():
 * @code
 *   MuxTask_registerRxCb(MUX_NLI_BLE, MuxVirtUart_rxNotify);
 * @endcode
 *
 * @param buf  Pointer to received payload bytes (NPI-framed HCI data).
 * @param len  Number of bytes in @p buf.
 */
void MuxVirtUart_rxNotify(const uint8_t *buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* MUX_VIRT_UART_H */
