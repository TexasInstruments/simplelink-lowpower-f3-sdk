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
 * @file  mux_npi_uart_shim.h
 * @brief Compile-time shim that redirects NPI's UART2_* calls to the MUX
 *        virtual UART port (mux_virt_uart.c).
 *
 * Applied via -include to the npi_tl_uart.obj build rule ONLY.  No other
 * translation unit sees these macro redirections.
 *
 * When npi_tl_uart.c includes <ti/drivers/UART2.h> (after this shim is
 * force-included), every UART2_open declaration/call in UART2.h becomes
 * MuxVirtUart_npiOpen, and so on.  The definitions live in mux_virt_uart.c
 * which is compiled WITHOUT this shim, so its own calls to UART2_open (for
 * the physical-port fallthrough path) resolve to the real SDK driver.
 */

#ifndef MUX_NPI_UART_SHIM_H
#define MUX_NPI_UART_SHIM_H

#define UART2_open        MuxVirtUart_npiOpen
#define UART2_close       MuxVirtUart_npiClose
#define UART2_read        MuxVirtUart_npiRead
#define UART2_readCancel  MuxVirtUart_npiReadCancel
#define UART2_write       MuxVirtUart_npiWrite

#endif /* MUX_NPI_UART_SHIM_H */
