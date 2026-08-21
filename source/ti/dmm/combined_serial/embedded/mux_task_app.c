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
 * @file  mux_task_app.c
 * @brief FreeRTOS MUX RX/TX task implementation for the embedded TI Combined
 *        Serial Interface.
 *
 * See mux_task_app.h for the architecture overview and thread-safety contract.
 *
 * Assert policy (same as rest of combined_serial layer):
 *   Assert before every error return that represents a programming mistake.
 *   Do NOT assert MUX_ERR_CRC (runtime line-noise event).
 *   Do NOT assert MUX_ERR_NO_PACKET (normal drain-complete condition).
 *   configASSERT() is used for FreeRTOS object creation failures — these are
 *   unrecoverable and must be caught in development.
 */

#include "mux_task_app.h"
#include "mux_uart.h"

#include "../hdlc_spinel.h"

#include <assert.h>
#include <string.h>   /* memset */

/* FreeRTOS */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

/*---------------------------------------------------------------------------
 * Per-NLI OT (Thread/spinel, MUX_NLI_OT) frame instrumentation — counts
 * complete frames, not raw bytes (see gMuxUartBytesTx/Rx in mux_uart.c for
 * the byte-level counterparts). gMuxOtFramesTx increments only after
 * MuxUart_write() returns success for an OT-tagged frame — i.e. the encoded
 * spinel reply/request actually left the device on the wire.
 * gMuxOtFramesRx increments when a decoded frame's NLI is MUX_NLI_OT, right
 * before dispatch to the registered OT RX callback. Non-static for
 * debugger visibility, matching the radio.c/ncp_base_radio.cpp counters.
 *--------------------------------------------------------------------------*/
volatile uint32_t gMuxOtFramesTx;
volatile uint32_t gMuxOtFramesRx;

/*---------------------------------------------------------------------------
 * Module-private state  (singletons — one RX state block and one TX task
 * per device — there is no RX task; see MuxRxState_t below)
 *--------------------------------------------------------------------------*/

/*!
 * RX state. Owns the incremental HDLC frame assembler and per-NLI dispatch
 * callbacks. frameBuf doubles as the assembler's accumulation buffer.
 *
 * Accessed only from muxUart_rxCallback()'s ISR context via
 * muxRx_handleBytes() (see below) — there is no RX task and no other
 * writer, so no locking is needed around any of this state: a single
 * execution context (the UART ISR, which cannot preempt itself) owns it
 * exclusively.
 */
typedef struct
{
    bool                   asmInFrame;       /* opening 0x7E seen */
    bool                   asmPayloadSeen;   /* non-flag byte seen since opening flag */
    uint16_t               asmLen;           /* bytes accumulated in frameBuf */

    uint8_t                frameBuf[MAX_FRAME_SIZE];         /* 2048 B */
    uint8_t                decodedBuf[MUX_SPINEL_BUF_MAX];  /*  517 B */

    /* Per-NLI RX callbacks registered before MuxTask_create(). Called
     * directly from ISR context — every registered callback (and anything
     * it calls transitively) must be ISR-safe. */
    MuxStackRxCb_t         rxCbs[MUX_NLI_COUNT];
} MuxRxState_t;

/*!
 * TX task state. Owns the encode scratch buffers and the TX lock, and is
 * solely responsible for sending the periodic keepalive.
 *
 * MuxTask_sendPacket() encodes and writes to the UART synchronously, in the
 * calling task's own context — no queue, no notify, no task switch. txLock
 * only serializes access to the shared scratch buffers (encodedBuf,
 * spinelEncBuf) and the UART write itself against concurrent callers (BLE
 * task, Zigbee/OT task) and the TX task's own periodic keepalive; it is
 * held only for the duration of one encode + one UART2 write, never across
 * a blocking wait on anything else.
 */
typedef struct
{
    TaskHandle_t           taskHandle;

    SemaphoreHandle_t      txLock;

    uint8_t                encodedBuf[MAX_FRAME_SIZE];       /* 2048 B */
    uint8_t                spinelEncBuf[MUX_SPINEL_BUF_MAX]; /*  517 B — scratch for MuxSpinelHdlc_encode() */

    /* Static FreeRTOS object storage — avoids heap allocation for the TCB/stack
     * and the TX mutex. */
    StaticTask_t           taskTcb;
    StackType_t            taskStack[MUX_TX_TASK_STACK_BYTES / sizeof(StackType_t)];
    StaticSemaphore_t      txLockBuf;
} MuxTxTaskState_t;

static MuxRxState_t gMuxRx;
static MuxTxTaskState_t gMuxTxTask;

/*---------------------------------------------------------------------------
 * Forward declarations
 *--------------------------------------------------------------------------*/

static void muxTxTask_fn(void *arg);
static MuxErr_t muxTx_encodeAndSend(uint8_t nli, uint32_t cmd, const uint8_t *buf, uint16_t len);
static void muxRx_handleBytes(const uint8_t *buf, uint16_t len);
static void muxTxTask_sendKeepalive(void);
static bool muxRxAsm_feedByte(uint8_t b, uint16_t *outFrameLen);
static void muxRx_processFrame(const uint8_t *frame, uint16_t frameLen);

/*---------------------------------------------------------------------------
 * MuxTask_registerRxCb
 *--------------------------------------------------------------------------*/

void MuxTask_registerRxCb(uint8_t nli, MuxStackRxCb_t cb)
{
    assert(nli < MUX_NLI_COUNT);   /* NLI index out of range */

    if (nli >= MUX_NLI_COUNT)
    {
        return;
    }

    gMuxRx.rxCbs[nli] = cb;
}

/*---------------------------------------------------------------------------
 * MuxTask_create
 *--------------------------------------------------------------------------*/

MuxErr_t MuxTask_create(uint8_t uartIndex, uint32_t baudRate)
{
    MuxErr_t err;

    /*
     * Reset only the assembler state, not the whole gMuxRx block: rxCbs
     * must already be populated by MuxTask_registerRxCb() calls made before
     * this function per its documented contract, and must survive here.
     */
    gMuxRx.asmInFrame     = false;
    gMuxRx.asmPayloadSeen = false;
    gMuxRx.asmLen         = 0U;

    memset(&gMuxTxTask, 0, sizeof(gMuxTxTask));

    /* ------------------------------------------------------------------
     * 1. TX mutex — serializes the encode scratch buffers and the UART
     *    write itself across concurrent MuxTask_sendPacket() callers and
     *    the TX task's periodic keepalive.
     * ------------------------------------------------------------------ */
    gMuxTxTask.txLock = xSemaphoreCreateMutexStatic(&gMuxTxTask.txLockBuf);

    configASSERT(gMuxTxTask.txLock != NULL);

    /* ------------------------------------------------------------------
     * 2. Create the TX FreeRTOS task (static allocation — no heap).
     *    There is no RX task: RX is decoded and dispatched directly from
     *    the UART ISR via muxRx_handleBytes(), passed to MuxUart_open()
     *    below as its rxHandler.
     * ------------------------------------------------------------------ */
    gMuxTxTask.taskHandle = xTaskCreateStatic(
        muxTxTask_fn,
        "MuxTxTask",
        (uint32_t)(MUX_TX_TASK_STACK_BYTES / sizeof(StackType_t)),
        NULL,
        (UBaseType_t)MUX_TX_TASK_PRIORITY,
        gMuxTxTask.taskStack,
        &gMuxTxTask.taskTcb);

    configASSERT(gMuxTxTask.taskHandle != NULL);

    /* ------------------------------------------------------------------
     * 3. Open UART (arms first UART2_read internally), passing
     *    muxRx_handleBytes as the ISR-context RX handler.
     * ------------------------------------------------------------------ */
    err = MuxUart_open(uartIndex, baudRate, muxRx_handleBytes);

    if (err != MUX_SUCCESS)
    {
        /* UART open failure is a runtime error — return it to the caller */
        return err;
    }

    return MUX_SUCCESS;
}

/*---------------------------------------------------------------------------
 * muxTx_encodeAndSend  (private — encodes into the shared scratch buffers
 * and writes to the UART; caller must hold gMuxTxTask.txLock)
 *--------------------------------------------------------------------------*/

static MuxErr_t muxTx_encodeAndSend(uint8_t nli, uint32_t cmd, const uint8_t *buf, uint16_t len)
{
    uint16_t encodedLen = 0U;
    MuxErr_t err;

    err = MuxSpinelHdlc_encode(
            nli,
            cmd,
            buf,
            len,
            gMuxTxTask.spinelEncBuf,
            (uint16_t)sizeof(gMuxTxTask.spinelEncBuf),
            gMuxTxTask.encodedBuf,
            (uint16_t)sizeof(gMuxTxTask.encodedBuf),
            &encodedLen);

    assert(err == MUX_SUCCESS);   /* encoding error — buffer sizing or invalid NLI */

    if (err != MUX_SUCCESS)
    {
        return err;
    }

    err = MuxUart_write(gMuxTxTask.encodedBuf, encodedLen);

    if (err == MUX_SUCCESS && nli == (uint8_t)MUX_NLI_OT)
    {
        gMuxOtFramesTx++;
    }

    return err;
}

/*---------------------------------------------------------------------------
 * MuxTask_sendPacket
 *
 * Encodes and writes to the UART synchronously, in the calling task's own
 * context. No queue, no notify, no task switch: the encode and MuxUart_write
 * (blocking — UART2_Mode_BLOCKING) both happen here before this returns.
 * txLock serializes this against concurrent callers (BLE task, Zigbee/OT
 * task) and the TX task's own periodic keepalive, so the shared scratch
 * buffers and the UART write itself are never touched by two contexts at
 * once — but a caller only ever waits on whichever context currently holds
 * txLock to finish one encode + one UART write, never on a separate task
 * being scheduled to drain a queue.
 *--------------------------------------------------------------------------*/

MuxErr_t MuxTask_sendPacket(uint8_t nli, const uint8_t *buf, uint16_t len)
{
    MuxErr_t err;

    assert(buf != NULL);                /* NULL payload pointer       */
    assert(len != 0U);                  /* zero-length packet is a bug */
    assert(len <= MUX_MSG_BUF_LEN);    /* payload exceeds queue message size */
    assert(nli < MUX_NLI_COUNT);       /* NLI out of range           */

    if (!buf || len == 0U || len > MUX_MSG_BUF_LEN || nli >= MUX_NLI_COUNT)
    {
        return MUX_ERR_INVALID;
    }

    (void)xSemaphoreTake(gMuxTxTask.txLock, portMAX_DELAY);
    err = muxTx_encodeAndSend(nli, (uint32_t)SPINEL_CMD_PROP_VALUE_IS, buf, len);
    xSemaphoreGive(gMuxTxTask.txLock);

    assert(err == MUX_SUCCESS);   /* UART TX error */
    return err;
}

/*---------------------------------------------------------------------------
 * muxTxTask_sendKeepalive  (private — called from the TX task only)
 *--------------------------------------------------------------------------*/

static void muxTxTask_sendKeepalive(void)
{
    MuxErr_t err;

    /*
     * Keepalive frame has no application payload (payload = NULL, len = 0).
     * MuxSpinelHdlc_encode handles the zero-length payload case.
     */
    (void)xSemaphoreTake(gMuxTxTask.txLock, portMAX_DELAY);
    err = muxTx_encodeAndSend((uint8_t)MUX_NLI_KEEPALIVE, (uint32_t)CMD_KEEPALIVE, NULL, 0U);
    xSemaphoreGive(gMuxTxTask.txLock);

    assert(err == MUX_SUCCESS);   /* UART TX error during keepalive */
    (void)err;
}

/*---------------------------------------------------------------------------
 * muxRxAsm_feedByte  (private — called from UART ISR context only, via
 * muxRx_handleBytes() below)
 *
 * Incremental HDLC frame assembler. Feeds one byte at a time and reports
 * when a complete frame (opening flag .. closing flag, inclusive) has been
 * accumulated in gMuxRx.frameBuf. Mirrors the garbage-skip /
 * inter-frame-fill-collapse / closing-flag rules that MuxBuf_extractFrame()
 * implemented as a multi-pass scan, but processes each byte exactly once
 * with no rescanning and no shared buffer with the ISR.
 *
 * @param b            Next raw byte from the UART RX stream.
 * @param[out] outFrameLen  Set to the completed frame's length when this
 *                          function returns true.
 *
 * @return true   A complete frame is ready in gMuxRx.frameBuf.
 * @return false  No complete frame yet — keep feeding bytes.
 *--------------------------------------------------------------------------*/

static bool muxRxAsm_feedByte(uint8_t b, uint16_t *outFrameLen)
{
    if (!gMuxRx.asmInFrame)
    {
        if (b != HDLC_FLAG)
        {
            /* Garbage before the first flag — discard. Not asserted:
             * legitimate at start-up or after a line glitch. */
            return false;
        }
        gMuxRx.frameBuf[0]    = HDLC_FLAG;
        gMuxRx.asmLen         = 1U;
        gMuxRx.asmInFrame     = true;
        gMuxRx.asmPayloadSeen = false;
        return false;
    }

    if (b == HDLC_FLAG)
    {
        if (!gMuxRx.asmPayloadSeen)
        {
            /* Repeated flag before any payload — inter-frame fill, ignore. */
            return false;
        }

        /*
         * Closing flag — frame complete. This same byte doubles as the
         * opening flag for the next frame, collapsing consecutive flags
         * exactly like the original multi-pass extractor did.
         */
        gMuxRx.frameBuf[gMuxRx.asmLen++] = HDLC_FLAG;
        *outFrameLen = gMuxRx.asmLen;

        gMuxRx.frameBuf[0]    = HDLC_FLAG;
        gMuxRx.asmLen         = 1U;
        gMuxRx.asmPayloadSeen = false;
        return true;
    }

    if (gMuxRx.asmLen >= sizeof(gMuxRx.frameBuf))
    {
        /* Oversized/unterminated frame — discard rather than stalling the
         * consumer, and resync on the next flag byte. Not asserted —
         * runtime robustness for corrupted or partial data. */
        gMuxRx.asmInFrame = false;
        gMuxRx.asmLen     = 0U;
        return false;
    }

    gMuxRx.frameBuf[gMuxRx.asmLen++] = b;
    gMuxRx.asmPayloadSeen = true;
    return false;
}

/*---------------------------------------------------------------------------
 * muxRx_processFrame  (private — called from UART ISR context only)
 *
 * Decodes one complete raw HDLC frame (as assembled by muxRxAsm_feedByte()),
 * parses the Spinel header, and dispatches the payload to the appropriate
 * per-NLI callback. The callback (gMuxRx.rxCbs[nli]) runs synchronously
 * here, still in ISR context — it and everything it calls must be ISR-safe.
 *--------------------------------------------------------------------------*/

static void muxRx_processFrame(const uint8_t *frame, uint16_t frameLen)
{
    MuxErr_t        err;
    uint16_t        decodedLen;
    uint8_t         nli;
    uint32_t        cmd;
    const uint8_t  *payloadPtr;
    uint16_t        payloadLen;

    /* ------------------------------------------------------------------
     * HDLC decode: unescape and verify CRC
     * ------------------------------------------------------------------ */
    decodedLen = (uint16_t)sizeof(gMuxRx.decodedBuf);
    err        = MuxHdlc_decode(frame, frameLen,
                                gMuxRx.decodedBuf, decodedLen,
                                &decodedLen);

    if (err == MUX_ERR_CRC)
    {
        /* CRC mismatch is a recoverable runtime event (line noise).
         * Not asserted — discard. */
        return;
    }

    if (err != MUX_SUCCESS)
    {
        /* Other decode errors (framing, overflow).  Not asserted — could
         * be caused by partial frames at startup or link glitches. */
        return;
    }

    /* ------------------------------------------------------------------
     * Spinel parse: extract NLI, CMD, and payload pointer
     * ------------------------------------------------------------------ */
    err = MuxSpinel_parseFrame(gMuxRx.decodedBuf, decodedLen,
                               &nli, &cmd,
                               &payloadPtr, &payloadLen);

    if (err != MUX_SUCCESS)
    {
        /* Malformed Spinel frame — discard */
        return;
    }

    /* ------------------------------------------------------------------
     * Dispatch by NLI
     * ------------------------------------------------------------------ */
    if (nli == (uint8_t)MUX_NLI_KEEPALIVE)
    {
        if (cmd == (uint32_t)CMD_KEEPALIVE_ACK)
        {
            /*
             * Host acknowledged our keepalive.
             * Dead-host detection and recovery are left for a future phase.
             * For now, just acknowledge receipt silently.
             */
        }
        /* Any other keepalive-channel CMD is ignored */
    }
    else if (nli < (uint8_t)MUX_NLI_COUNT)
    {
        if (nli == (uint8_t)MUX_NLI_OT)
        {
            gMuxOtFramesRx++;
        }

        if (gMuxRx.rxCbs[nli] != NULL)
        {
            gMuxRx.rxCbs[nli](payloadPtr, payloadLen);
        }
        /* If no callback is registered, the packet is silently dropped.
         * Not asserted — stacks may not register callbacks during init. */
    }
    /* NLI values >= MUX_NLI_COUNT are silently ignored */
}

/*---------------------------------------------------------------------------
 * muxRx_handleBytes  (private — this IS the UART ISR context; passed to
 * MuxUart_open() as its rxHandler)
 *
 * Feeds each received byte into the incremental HDLC frame assembler and
 * processes any frame it completes — synchronously, before returning to
 * muxUart_rxCallback(). There is no task hand-off and no intermediate
 * buffer: this function, muxRxAsm_feedByte(), muxRx_processFrame(), and
 * every registered per-NLI callback all run in UART ISR context and must
 * be ISR-safe (no blocking calls, no non-FromISR FreeRTOS/ICall APIs,
 * bounded execution time).
 *--------------------------------------------------------------------------*/

static void muxRx_handleBytes(const uint8_t *buf, uint16_t len)
{
    uint16_t frameLen;
    uint16_t i;

    for (i = 0U; i < len; i++)
    {
        if (muxRxAsm_feedByte(buf[i], &frameLen))
        {
            muxRx_processFrame(gMuxRx.frameBuf, frameLen);
        }
    }
}

/*---------------------------------------------------------------------------
 * muxTxTask_fn  (TX task entry point)
 *
 * Data sends no longer go through this task — MuxTask_sendPacket() writes
 * synchronously in the caller's own context (see above). This task now
 * exists solely to send the periodic keepalive.
 *--------------------------------------------------------------------------*/

static void muxTxTask_fn(void *arg)
{
    (void)arg;

    /* Send an initial keepalive so the host knows the device is up */
    muxTxTask_sendKeepalive();

    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(MUX_KEEPALIVE_PERIOD_MS));
        muxTxTask_sendKeepalive();
    }
}
