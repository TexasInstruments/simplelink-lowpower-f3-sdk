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
 * @file  mux_uart.c
 * @brief UART2 driver integration for the embedded TI Combined Serial MUX.
 *
 * Assert policy (same as rest of combined_serial layer):
 *   Programming errors (NULL pointers, bad state) are asserted before the
 *   runtime error return.  Hardware-level errors (UART2 open failure, DMA
 *   overrun) are NOT asserted — they are runtime events returned as
 *   MUX_ERR_UART so the caller can handle or log them.
 */

#include "mux_uart.h"

#include <assert.h>

/* TI-Drivers */
#include <ti/drivers/UART2.h>

/*---------------------------------------------------------------------------
 * Raw byte-level TX/RX instrumentation — every byte physically written to
 * or read from the UART2 peripheral, regardless of which stack (OT/BLE/ZB/
 * keepalive) it belongs to. Non-static so a debugger can watch them
 * directly. Compare against the per-NLI OT-frame counters in
 * mux_task_app.c (gMuxOtFramesTx/gMuxOtFramesRx) to localize a loss: if
 * bytes left the UART here but the corresponding OT-frame counter didn't
 * increment, the loss is above this layer (encode/dispatch); if bytes never
 * reached here at all, the loss is below (UART hardware/driver).
 *--------------------------------------------------------------------------*/
volatile uint32_t gMuxUartBytesTx;
volatile uint32_t gMuxUartBytesRx;

/*---------------------------------------------------------------------------
 * Module-private state  (singleton — one MUX UART per device)
 *--------------------------------------------------------------------------*/

typedef struct
{
    UART2_Handle         handle;                          /*!< Open UART2 handle          */
    MuxUart_RxHandler_t  rxHandler;                        /*!< Called from ISR per chunk  */
    uint8_t              rxStagingBuf[MUX_UART_RX_CHUNK]; /*!< DMA staging (ISR only)  */
    bool                 isOpen;                          /*!< Guard against double-open  */
} MuxUartState_t;

static MuxUartState_t gUart = { 0 };

/*---------------------------------------------------------------------------
 * Forward declaration
 *--------------------------------------------------------------------------*/

static void muxUart_rxCallback(UART2_Handle handle, void *buf, size_t count,
                                void *userArg, int_fast16_t status);

/*---------------------------------------------------------------------------
 * MuxUart_open
 *--------------------------------------------------------------------------*/

MuxErr_t MuxUart_open(uint8_t uartIndex, uint32_t baudRate,
                      MuxUart_RxHandler_t rxHandler)
{
    UART2_Params params;
    int_fast16_t status;

    assert(rxHandler != NULL);   /* NULL RX handler           */
    assert(!gUart.isOpen);       /* MuxUart_open called twice */

    if (!rxHandler)
    {
        return MUX_ERR_INVALID;
    }

    /* Store the handler before opening so the callback can use it
     * immediately after UART2_open() arms the first read. */
    gUart.rxHandler = rxHandler;

    /* Configure UART2 parameters */
    UART2_Params_init(&params);
    params.baudRate       = baudRate;
    params.dataLength     = UART2_DataLen_8;
    params.stopBits       = UART2_StopBits_1;
    params.parityType     = UART2_Parity_NONE;

    /* TX: blocking — MuxUart_write() blocks until all bytes are sent */
    params.writeMode      = UART2_Mode_BLOCKING;

    /* RX: callback — muxUart_rxCallback() fires when bytes arrive */
    params.readMode           = UART2_Mode_CALLBACK;
    params.readCallback       = muxUart_rxCallback;
    params.readReturnMode     = UART2_ReadReturnMode_PARTIAL;

    gUart.handle = UART2_open(uartIndex, &params);

    if (gUart.handle == NULL)
    {
        /* UART2_open() failed — hardware not available or index invalid */
        gUart.rxHandler = NULL;
        return MUX_ERR_UART;
    }

    /* Arm the first read — subsequent reads are re-armed from the callback */
    status = UART2_read(gUart.handle,
                        gUart.rxStagingBuf,
                        MUX_UART_RX_CHUNK,
                        NULL);

    if (status != UART2_STATUS_SUCCESS)
    {
        UART2_close(gUart.handle);
        gUart.handle    = NULL;
        gUart.rxHandler = NULL;
        return MUX_ERR_UART;
    }

    gUart.isOpen = true;
    return MUX_SUCCESS;
}

/*---------------------------------------------------------------------------
 * MuxUart_close
 *--------------------------------------------------------------------------*/

void MuxUart_close(void)
{
    assert(gUart.isOpen);   /* close called without a matching open */

    if (gUart.handle != NULL)
    {
        UART2_close(gUart.handle);
        gUart.handle = NULL;
    }

    gUart.rxHandler = NULL;
    gUart.isOpen    = false;
}

/*---------------------------------------------------------------------------
 * MuxUart_write
 *--------------------------------------------------------------------------*/

MuxErr_t MuxUart_write(const uint8_t *buf, uint16_t len)
{
    size_t       bytesWritten = 0U;
    int_fast16_t status;

    assert(buf  != NULL);     /* NULL buffer pointer      */
    assert(len  != 0U);       /* zero-length write is bug */
    assert(gUart.isOpen);     /* write before open        */

    if (!buf || len == 0U)
    {
        return MUX_ERR_INVALID;
    }

    status = UART2_write(gUart.handle, buf, (size_t)len, &bytesWritten);

    assert(status == UART2_STATUS_SUCCESS);   /* UART TX hardware error */

    if (status != UART2_STATUS_SUCCESS)
    {
        return MUX_ERR_UART;
    }

    assert(bytesWritten == (size_t)len);   /* partial write — should not happen in blocking mode */

    gMuxUartBytesTx += (uint32_t)bytesWritten;

    return MUX_SUCCESS;
}

/*---------------------------------------------------------------------------
 * muxUart_rxCallback  (ISR context)
 *
 * Called by the UART2 driver after each completed UART2_read().
 * Execution context: UART hardware interrupt (ISR).
 *
 * Dispatches the received bytes directly to gUart.rxHandler — synchronously,
 * still in ISR context — then re-arms the next UART2_read(). There is no
 * intermediate buffer and no task hand-off: rxHandler (and everything it
 * calls — the frame assembler, decoder, and per-NLI RX callbacks in
 * mux_task_app.c) must be ISR-safe. See the file-level ISR-safety note in
 * mux_uart.h.
 *--------------------------------------------------------------------------*/

static void muxUart_rxCallback(UART2_Handle handle, void *buf, size_t count,
                                void *userArg, int_fast16_t status)
{
    (void)userArg;   /* unused — module state accessed via gUart */

    if (status == UART2_STATUS_SUCCESS && count != 0U)
    {
        gMuxUartBytesRx += (uint32_t)count;
        gUart.rxHandler((const uint8_t *)buf, (uint16_t)count);
    }
    /*
     * Hardware error (framing, overrun, break) or zero-byte callback: skip
     * dispatch, just re-arm below. Not asserted — hardware errors are
     * runtime events on a real link.
     */

    /*
     * Re-arm the next read.  UART2_read() in callback mode is safe to call
     * from ISR context.  The staging buffer is not reused until this
     * call completes (next callback invocation), so reuse is safe here.
     */
    UART2_read(handle, gUart.rxStagingBuf, MUX_UART_RX_CHUNK, NULL);
}
