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
 * @file  mux_task_app.h
 * @brief FreeRTOS MUX tasks for the embedded TI Combined Serial Interface.
 *
 * There is no RX task and, as of the synchronous-send redesign, no TX
 * queue either — the only FreeRTOS task this layer creates is the TX task,
 * which exists solely to send the periodic keepalive:
 *   - RX: decoded and dispatched entirely inside the UART ISR. Every
 *     received byte is fed to the incremental HDLC assembler, and any
 *     complete frame is decoded, Spinel-parsed, and handed to the
 *     registered per-stack RX callback — all synchronously, in ISR
 *     context, before the ISR re-arms the next UART2_read() and returns.
 *   - TX: MuxTask_sendPacket() encodes outbound packets (from BLE/Zigbee/OT
 *     stacks) as Spinel+HDLC frames and writes them to the UART
 *     synchronously, in the calling task's own context — no queue, no
 *     notify, no task switch.
 *   - The TX task sends periodic CMD_KEEPALIVE frames to the Linux host
 *     (processing CMD_KEEPALIVE_ACK replies happens inline in the RX ISR
 *     path, since that's where the ACK frame arrives).
 *
 * Because RX runs entirely in the ISR, it is inherently independent of
 * whatever any task is doing: MuxTask_sendPacket() blocking inside
 * MuxUart_write() — a genuine blocking call in UART2_Mode_BLOCKING — blocks
 * only the calling task (BLE/Zigbee/OT/NPI); it cannot delay RX processing,
 * since RX doesn't wait on the scheduler at all. The UART2 driver
 * explicitly supports concurrent UART2_read()/UART2_write(); the only
 * restriction is two concurrent operations in the *same* direction.
 *
 * ISR-safety requirement on RX callbacks
 * ----------------------------------------
 * Every callback registered via MuxTask_registerRxCb() — and everything it
 * calls transitively — now runs in real UART hardware interrupt context.
 * It must not block, must not call any FreeRTOS/ICall API that isn't
 * explicitly documented as ISR-safe (i.e. a *FromISR variant, or one that
 * internally branches on HwiP_inISR() like this SDK's mq_send()), and
 * should keep execution time bounded, since it directly extends UART
 * interrupt latency for every other interrupt in the system while it runs.
 * Verified ISR-safe today: the BLE path (MuxVirtUart_rxNotify(), which is
 * how NPI's UART2_Mode_CALLBACK contract already expects to be invoked) and
 * the OT/Thread path (ThreadMux_rxNotify() → platformUartMuxDeliver(),
 * whose mq_send()/platformUartSignal() calls are already used from real
 * ISR context elsewhere in this SDK's non-MUX UART callback). Zigbee's
 * ZbMux_muxRxCb() is not currently registered in this build (Zigbee is
 * disabled) and has not been audited for ISR-safety — audit it before
 * enabling Zigbee on this path.
 *
 * Data flow overview
 * ------------------
 *
 *   BLE task / Zigbee task / OT task
 *       │
 *       ▼  MuxTask_sendPacket(nli, buf, len)  [any task context, blocks here]
 *   take txLock → MuxSpinelHdlc_encode() → MuxUart_write() → give txLock
 *       │
 *       ▼
 *   UART TX pin → host
 *
 *   UART RX pin → ISR (muxUart_rxCallback, see mux_uart.h)
 *       │
 *       ▼  muxRx_handleBytes(buf, len) — still in ISR context
 *   incremental HDLC assembler (muxRxAsm_feedByte)
 *       → MuxHdlc_decode() → MuxSpinel_parseFrame()
 *       │
 *       ├── NLI = BLE  →  gMuxRx.rxCbs[MUX_NLI_BLE](payload, len)
 *       ├── NLI = ZB   →  gMuxRx.rxCbs[MUX_NLI_ZB](payload, len)
 *       └── NLI = KA   →  keepalive ACK handling (no callback)
 *
 * Task priority and scheduling
 * ----------------------------
 * The TX (keepalive-only) task runs at MUX_TX_TASK_PRIORITY (7), higher
 * than both the BLE task (6) and the Zigbee task (1). MuxTask_sendPacket()
 * runs in the calling task's own priority, not this one. RX has no task
 * priority at all — it runs at UART interrupt priority.
 *
 * Blocking strategy
 * ------------------
 * The TX task just sleeps for MUX_KEEPALIVE_PERIOD_MS (vTaskDelay) between
 * keepalives — it doesn't wait on any notification, since nothing defers
 * work to it.
 *
 * TX lock — synchronous send, not a queue
 * ----------------------------------------
 * MuxTask_sendPacket() may be called concurrently from multiple task
 * contexts (BLE task, Zigbee/OT task), and the TX task's own keepalive also
 * needs the same encode scratch buffers and UART. gMuxTxTask.txLock (a
 * mutex) serializes exactly that: a caller takes it, encodes into
 * gMuxTxTask.spinelEncBuf/encodedBuf, writes to the UART, and releases it —
 * held only for the duration of one encode + one UART2 write, never across
 * a wait on anything else. There is no MPSC array and no separate consumer
 * task to depend on: a blocked caller is only ever waiting on whichever
 * context currently holds txLock to finish that one send.
 *
 * Keepalive
 * ---------
 * The embedded device sends CMD_KEEPALIVE (15555) to the host every
 * MUX_KEEPALIVE_PERIOD_MS milliseconds — owned entirely by the TX task,
 * since sending it is a TX action (encode + MuxUart_write()). The host is
 * expected to reply with CMD_KEEPALIVE_ACK (15556); the RX ISR path
 * receives and acknowledges it. Dead-host detection and recovery are left
 * for a future phase.
 *
 * Thread safety
 * -------------
 * - MuxTask_registerRxCb() must be called before MuxTask_create().
 * - MuxTask_sendPacket() is thread-safe and may be called from any task.
 *   It no longer depends on a *separate task* being scheduled to make
 *   progress (there is no queue/consumer task to wait on) — but
 *   MuxUart_write() still uses UART2_Mode_BLOCKING internally, which waits
 *   on a semaphore signaled by the UART2 TX-complete interrupt. Calling
 *   this from inside a critical section that disables interrupts (e.g.
 *   NPIUtil_EnterCS()/ICall_enterCriticalSection()) will still deadlock,
 *   since that interrupt can never fire. Callers must not hold such a
 *   critical section across this call.
 * - gMuxRx (RX frame-staging state) is touched only from UART ISR context;
 *   gMuxTxTask's encode scratch buffers are touched only while holding
 *   txLock. The two are disjoint module-static structs — RX never touches
 *   gMuxTxTask's buffers, and MuxTask_sendPacket() never touches gMuxRx's.
 */

#ifndef MUX_TASK_APP_H
#define MUX_TASK_APP_H

#include "../mux_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------
 * Task configuration
 *--------------------------------------------------------------------------*/

/*!
 * FreeRTOS task priority for the MUX TX task (keepalive-only — see the
 * file-level doc comment above). There is no RX task: RX runs at UART
 * interrupt priority, not a FreeRTOS task priority.
 */
#define MUX_TX_TASK_PRIORITY       (7U)

/*!
 * MUX TX task stack size in bytes.
 * Large intermediate buffers (encoded TX frame, Spinel encode scratch) are
 * module-static, so the task stack only needs to accommodate function call
 * frames (encode → write).
 */
#define MUX_TX_TASK_STACK_BYTES    (1024U)

/*!
 * Interval between CMD_KEEPALIVE transmissions (milliseconds).
 * Also used as the TX task's xTaskNotifyWait() timeout — the task wakes at
 * least this often even when idle.
 */
#define MUX_KEEPALIVE_PERIOD_MS (5000U)

/*---------------------------------------------------------------------------
 * API
 *--------------------------------------------------------------------------*/

/*!
 * @brief Register a per-NLI RX callback.
 *
 * Must be called before MuxTask_create() — see MuxTask_create()'s doc.
 * The callback is invoked synchronously, in UART ISR context, when a
 * decoded inbound packet arrives for the given NLI channel. It (and
 * everything it calls) must be ISR-safe: see the file-level ISR-safety
 * note above.
 *
 * @param nli  NLI channel (MUX_NLI_BLE or MUX_NLI_ZB).
 * @param cb   Callback function; NULL to deregister.
 */
void MuxTask_registerRxCb(uint8_t nli, MuxStackRxCb_t cb);

/*!
 * @brief Initialise MUX resources, create the TX (keepalive) task, and
 *        open the UART.
 *
 * Performs in order:
 *   1. Resets the RX frame assembler state (asmInFrame/asmPayloadSeen/
 *      asmLen) — NOT the RX callback table, which must already be
 *      populated by MuxTask_registerRxCb() calls made before this
 *      function, per its documented contract.
 *   2. Creates the TX mutex (gMuxTxTask.txLock).
 *   3. Creates the TX task at MUX_TX_TASK_PRIORITY (does not run until
 *      vTaskStartScheduler() is called).
 *   4. Opens the UART via MuxUart_open(), passing muxRx_handleBytes as the
 *      RX handler the UART ISR calls directly for every received chunk.
 *      There is no RX task to wake — RX is fully handled inside the ISR.
 *
 * Any OS resource allocation failure triggers a configASSERT() halt.
 * Call this function once from application main() before vTaskStartScheduler().
 * MUX clients (BLE HCI, Zigbee) must NOT call this — they only register
 * their RX callbacks via MuxTask_registerRxCb().
 *
 * @param uartIndex  UART2 instance index from SysConfig (CONFIG_MUX_UART).
 * @param baudRate   Baud rate in bits/second (e.g. MUX_UART_DEFAULT_BAUD).
 *
 * @return MUX_SUCCESS   TX task created and UART open.
 * @return MUX_ERR_UART  UART2_open() or initial UART2_read() failed.
 */
MuxErr_t MuxTask_create(uint8_t uartIndex, uint32_t baudRate);

/*!
 * @brief Encode and transmit a packet to the Linux host.
 *
 * Thread-safe: may be called concurrently from any task context (BLE task,
 * Zigbee/OT task). Encodes and writes to the UART synchronously, in the
 * calling task's own context — see the "TX lock" section of the file-level
 * doc comment above. The caller's buffer is not referenced after this
 * function returns.
 *
 * Blocking
 * --------
 * This call blocks for as long as it takes to take txLock (if another
 * context is mid-send) plus one encode and one blocking UART2 write. There
 * is no unbounded queue to fill up — the caller is only ever waiting on
 * whichever single context currently holds txLock, never on a separate
 * consumer task making progress.
 *
 * @param nli  NLI channel (MUX_NLI_BLE or MUX_NLI_ZB).
 * @param buf  Payload bytes to transmit (BLE HCI packet or Zigbee MAC frame).
 * @param len  Payload length.  Must be in [1, MUX_MSG_BUF_LEN].
 *
 * @return MUX_SUCCESS      Message encoded and written successfully.
 * @return MUX_ERR_INVALID  NULL pointer, zero length, length > MUX_MSG_BUF_LEN,
 *                          or NLI is out of range.
 */
MuxErr_t MuxTask_sendPacket(uint8_t nli, const uint8_t *buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* MUX_TASK_APP_H */
