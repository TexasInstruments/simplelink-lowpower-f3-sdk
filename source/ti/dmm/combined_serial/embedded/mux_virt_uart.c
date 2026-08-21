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
 * @file  mux_virt_uart.c
 * @brief Virtual UART-2 port (MUX_VIRT_PORT_0) for the TI Combined Serial MUX.
 *
 * The mux_npi_uart_shim.h header (force-included via -include on the
 * npi_tl_uart.obj build rule only) maps UART2_open → MuxVirtUart_npiOpen,
 * and so on, at the preprocessor level.  This file is compiled WITHOUT that
 * shim, so its own calls to UART2_open / UART2_read etc. reach the real
 * hardware driver unmodified.
 *
 * For index == CONFIG_UART2_0 (the virtual port) each MuxVirtUart_npi* call
 * is handled here; for any other index the call falls through to the real
 * hardware driver via the standard UART2_* API.
 *
 * See mux_virt_uart.h for the full architecture description.
 */

#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <ti/drivers/UART2.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti_drivers_config.h>

/* FreeRTOS — needed for the BLE RX delivery task */
#include <FreeRTOS.h>
#include <task.h>

#include "../mux_common.h"
#include "mux_task_app.h"
#include "mux_virt_uart.h"

/*---------------------------------------------------------------------------
 * Constants
 *--------------------------------------------------------------------------*/

/*!
 * RX ring-buffer size in bytes.  Must be a power of 2.
 * Sized at 2 × MUX_MSG_BUF_LEN so that one full MUX payload can be buffered
 * while NPI is processing the previous one.
 */
#define VIRT_RX_RING_SIZE  (MUX_MSG_BUF_LEN * 2U)  /* 1024 B — power of 2 */

/*!
 * BLE RX delivery task configuration.
 *
 * This task exists solely to call virtUart_serviceRead() from TASK context
 * so that the NPI readCb chain (readCb → NPITL_transmissionCallBack →
 * NPITask_transportRXCallBack → EventP_post) runs from a task rather than
 * from the UART ISR.
 *
 * When EventP_post is called from ISR context it uses
 * xEventGroupSetBitsFromISR(), which defers the actual bit-set to the
 * FreeRTOS timer/daemon task via a queue. If that queue is full, or the
 * timer task is starved, the event is silently dropped and NPI never wakes
 * up to process the incoming HCI command — causing BlueZ scan timeouts.
 *
 * By running virtUart_serviceRead() from task context, EventP_post uses
 * xEventGroupSetBits() directly, which sets the bit immediately with no
 * timer-task intermediary.
 *
 * Priority: same as NPI task (NPITASK_PRIORITY = 7). They round-robin;
 * NPI processes the event as soon as it is posted.
 */
#define MUX_BLE_RX_TASK_PRIORITY    (7U)
#define MUX_BLE_RX_TASK_STACK_WORDS (256U)

/*---------------------------------------------------------------------------
 * Virtual UART state
 *--------------------------------------------------------------------------*/

typedef struct
{
    bool isOpen;

    /* Callbacks saved from UART2_Params at UART2_open() time */
    UART2_Callback       readCb;
    UART2_Callback       writeCb;
    UART2_EventCallback  eventCb;
    void                   *userArg;
    uint32_t                eventMask;

    /* RX circular ring buffer (head/tail as free-running uint16_t counters) */
    uint8_t  rxRing[VIRT_RX_RING_SIZE];
    uint16_t rxHead;    /* next write position */
    uint16_t rxTail;    /* next read  position */

    /* Pending async read posted via UART2_read() */
    void    *pendBuf;
    size_t   pendSize;
    bool     readPending;

    /*
     * Re-entrancy guard.
     * Set true while readCb is executing so that a concurrent
     * MuxVirtUart_rxNotify → virtUart_serviceRead call returns immediately
     * without attempting recursive callback delivery.  The outer
     * virtUart_serviceRead loop will pick up any bytes left in the ring once
     * readCb returns and has reposted a UART2_read().
     */
    bool inReadCb;

    /*
     * Sentinel handle returned by MuxVirtUart_npiOpen when
     * index == CONFIG_UART2_0. Any subsequent MuxVirtUart_npi* call that
     * receives this pointer is handled by the virtual implementation; the
     * physical driver never sees it.
     */
    UART2_Config cfg;

} MuxVirtUartState_t;

static MuxVirtUartState_t gVirt;

#define IS_VIRT(h)  ((h) == (UART2_Handle)&gVirt.cfg)

/*---------------------------------------------------------------------------
 * BLE RX delivery task state (static allocation — no heap).
 *--------------------------------------------------------------------------*/
static TaskHandle_t gBleRxTaskHandle;
static StaticTask_t gBleRxTaskTcb;
static StackType_t  gBleRxTaskStack[MUX_BLE_RX_TASK_STACK_WORDS];

/* Forward declaration — defined after virtUart_serviceRead(). */
static void muxBleRxTask_fn(void *arg);

/*---------------------------------------------------------------------------
 * Ring-buffer helpers
 * Callers must hold an HwiP key when reading or writing head/tail.
 *--------------------------------------------------------------------------*/

static inline uint16_t ringCount(void)
{
    /* Unsigned wrap-around subtraction is correct for all head/tail values. */
    return (uint16_t)(gVirt.rxHead - gVirt.rxTail);
}

static inline void ringWrite(const uint8_t *src, uint16_t n)
{
    /* Silently clamp to available space; excess bytes are dropped. */
    uint16_t space = (uint16_t)(VIRT_RX_RING_SIZE - ringCount());
    if (n > space)
    {
        n = space;
    }

    for (uint16_t i = 0U; i < n; i++)
    {
        gVirt.rxRing[gVirt.rxHead & (VIRT_RX_RING_SIZE - 1U)] = src[i];
        gVirt.rxHead++;
    }
}

static inline uint16_t ringRead(void *dst, uint16_t maxLen)
{
    uint16_t n = ringCount();
    if (n > maxLen)
    {
        n = maxLen;
    }

    uint8_t *d = (uint8_t *)dst;
    for (uint16_t i = 0U; i < n; i++)
    {
        d[i] = gVirt.rxRing[gVirt.rxTail & (VIRT_RX_RING_SIZE - 1U)];
        gVirt.rxTail++;
    }
    return n;
}

/*---------------------------------------------------------------------------
 * virtUart_serviceRead
 *
 * Delivers buffered RX bytes to the pending read slot and invokes readCb.
 * Must NOT be called while holding an HwiP key because the callback may
 * itself call UART2_read() (which re-acquires the key).
 *
 * The while loop handles the back-to-back case: after readCb returns and
 * has reposted a UART2_read(), more data may already be in the ring; the
 * loop drains it without waiting for another rxNotify call.
 *--------------------------------------------------------------------------*/

static void virtUart_serviceRead(void)
{
    /* If we are already inside readCb (recursive entry), bail out.
     * The outer virtUart_serviceRead call will pick up residual bytes
     * once readCb returns. */
    if (gVirt.inReadCb)
    {
        return;
    }

    while (true)
    {
        /* --- critical section: snap and clear the pending read state --- */
        uintptr_t key = HwiP_disable();

        if (!gVirt.readPending || (ringCount() == 0U))
        {
            HwiP_restore(key);
            break;   /* nothing to deliver */
        }

        void  *buf  = gVirt.pendBuf;
        size_t want = gVirt.pendSize;
        gVirt.readPending = false;

        uint16_t got = ringRead(buf, (uint16_t)want);

        HwiP_restore(key);
        /* --- end critical section --- */

        /* Invoke the NPI read callback outside the critical section. */
        if (gVirt.readCb != NULL)
        {
            gVirt.inReadCb = true;
            gVirt.readCb((UART2_Handle)&gVirt.cfg,
                         buf, (size_t)got,
                         gVirt.userArg,
                         UART2_STATUS_SUCCESS);
            gVirt.inReadCb = false;
        }

        /* readCb will have called UART2_read() again (resetting readPending).
         * Loop to check whether more data has already arrived. */
    }
}

/*===========================================================================
 * MuxVirtUart_npiOpen
 *
 * For CONFIG_UART2_0: initialise virtual state from params and return the
 * sentinel handle.  For any other index: delegate to the real driver.
 *==========================================================================*/

UART2_Handle MuxVirtUart_npiOpen(uint_least8_t index, UART2_Params *params)
{
    if (index != CONFIG_UART2_0)
    {
        return UART2_open(index, params);
    }

    memset(&gVirt, 0, sizeof(gVirt));

    if (params != NULL)
    {
        gVirt.readCb    = params->readCallback;
        gVirt.writeCb   = params->writeCallback;
        gVirt.eventCb   = params->eventCallback;
        gVirt.userArg   = params->userArg;
        gVirt.eventMask = params->eventMask;
    }

    gVirt.isOpen = true;

    /* Create the BLE RX delivery task once. It calls virtUart_serviceRead()
     * from task context so EventP_post uses xEventGroupSetBits() (immediate)
     * rather than xEventGroupSetBitsFromISR() (deferred via timer task,
     * lossy when the timer task command queue is full). */
    if (gBleRxTaskHandle == NULL)
    {
        gBleRxTaskHandle = xTaskCreateStatic(
            muxBleRxTask_fn,
            "BleRxDlvr",
            MUX_BLE_RX_TASK_STACK_WORDS,
            NULL,
            (UBaseType_t)MUX_BLE_RX_TASK_PRIORITY,
            gBleRxTaskStack,
            &gBleRxTaskTcb);
    }

    return (UART2_Handle)&gVirt.cfg;
}

/*===========================================================================
 * MuxVirtUart_npiClose
 *==========================================================================*/

void MuxVirtUart_npiClose(UART2_Handle handle)
{
    if (!IS_VIRT(handle))
    {
        UART2_close(handle);
        return;
    }

    memset(&gVirt, 0, sizeof(gVirt));
}

/*===========================================================================
 * MuxVirtUart_npiRead
 *
 * Posts an async read request.  If bytes are already in the ring buffer
 * (from a prior MuxVirtUart_rxNotify call), they are delivered immediately
 * via readCb.  Otherwise the request waits until the next rxNotify.
 *==========================================================================*/

int_fast16_t MuxVirtUart_npiRead(UART2_Handle handle, void *buffer,
                                  size_t size, size_t *bytesRead)
{
    if (!IS_VIRT(handle))
    {
        return UART2_read(handle, buffer, size, bytesRead);
    }

    uintptr_t key = HwiP_disable();
    gVirt.pendBuf     = buffer;
    gVirt.pendSize    = size;
    gVirt.readPending = true;
    HwiP_restore(key);

    /* Deliver immediately if data is already buffered. */
    virtUart_serviceRead();

    return UART2_STATUS_SUCCESS;
}

/*===========================================================================
 * MuxVirtUart_npiReadCancel
 *==========================================================================*/

int_fast16_t MuxVirtUart_npiReadCancel(UART2_Handle handle)
{
    if (!IS_VIRT(handle))
    {
        UART2_readCancel(handle);
        return UART2_STATUS_SUCCESS;
    }

    uintptr_t key   = HwiP_disable();
    bool wasPending = gVirt.readPending;
    void *buf       = gVirt.pendBuf;
    gVirt.readPending = false;
    HwiP_restore(key);

    /* Fire readCb with zero bytes to match physical driver behaviour. */
    if (wasPending && (gVirt.readCb != NULL))
    {
        gVirt.readCb((UART2_Handle)&gVirt.cfg,
                     buf, 0U,
                     gVirt.userArg,
                     UART2_STATUS_SUCCESS);
    }

    return UART2_STATUS_SUCCESS;
}

/*===========================================================================
 * MuxVirtUart_npiWrite
 *
 * Enqueues the BLE payload for the MUX task TX queue, then fires the TX
 * completion callbacks synchronously.
 *
 * NPI's npi_tl_uart.c expects the TX completion sequence:
 *   1. eventCallback(UART2_EVENT_TX_FINISHED) — sets UartTxFinishedFlag
 *   2. writeCallback called from inside eventCallback
 * We replicate this by calling eventCb; NPI's NPITLUART_eventCallBack then
 * calls NPITLUART_writeCallBack internally.
 *==========================================================================*/

int_fast16_t MuxVirtUart_npiWrite(UART2_Handle handle, const void *buffer,
                                   size_t size, size_t *bytesWritten)
{
    if (!IS_VIRT(handle))
    {
        return UART2_write(handle, buffer, size, bytesWritten);
    }

    /* Enqueue NPI-framed BLE payload for the MUX task TX path. */
    MuxErr_t err = MuxTask_sendPacket(MUX_NLI_BLE,
                                      (const uint8_t *)buffer,
                                      (uint16_t)size);
    if (err != MUX_SUCCESS)
    {
        return UART2_STATUS_EOVERRUN;
    }

    if (bytesWritten != NULL)
    {
        *bytesWritten = size;
    }

    /*
     * Signal TX completion in the way NPI expects.
     * NPITLUART_eventCallBack sets UartTxFinishedFlag then calls
     * NPITLUART_writeCallBack internally, so we must fire eventCb rather
     * than writeCallback directly.
     */
    if ((gVirt.eventMask & UART2_EVENT_TX_FINISHED) && (gVirt.eventCb != NULL))
    {
        gVirt.eventCb((UART2_Handle)&gVirt.cfg,
                      UART2_EVENT_TX_FINISHED, 0U, gVirt.userArg);
    }
    else if (gVirt.writeCb != NULL)
    {
        /* Fallback for code that does not subscribe to TX_FINISHED. */
        gVirt.writeCb((UART2_Handle)&gVirt.cfg,
                      (void *)buffer, size,
                      gVirt.userArg, UART2_STATUS_SUCCESS);
    }

    return UART2_STATUS_SUCCESS;
}

/*===========================================================================
 * muxBleRxTask_fn
 *
 * BLE RX delivery task. Blocks on a direct task notification posted by
 * MuxVirtUart_rxNotify() from ISR context via vTaskNotifyGiveFromISR().
 * Calls virtUart_serviceRead() from TASK context so that the downstream
 * EventP_post() call uses xEventGroupSetBits() (immediate, reliable) rather
 * than xEventGroupSetBitsFromISR() (deferred to timer task, lossy).
 *==========================================================================*/

static void muxBleRxTask_fn(void *arg)
{
    (void)arg;
    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        virtUart_serviceRead();
    }
}

/*===========================================================================
 * MuxVirtUart_rxNotify
 *
 * Called by the MUX UART ISR on the NLI_BLE receive path with NPI-framed
 * HCI bytes decoded from the host HDLC/Spinel frame. Appends to the ring
 * buffer and wakes the BLE RX delivery task via vTaskNotifyGiveFromISR().
 *
 * vTaskNotifyGiveFromISR increments the task notification counter directly
 * in the TCB — no timer/daemon task intermediary, cannot be dropped
 * silently regardless of system load.
 *==========================================================================*/

void MuxVirtUart_rxNotify(const uint8_t *buf, uint16_t len)
{
    if (!gVirt.isOpen || (buf == NULL) || (len == 0U))
    {
        return;
    }

    uintptr_t key = HwiP_disable();
    ringWrite(buf, len);
    HwiP_restore(key);

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(gBleRxTaskHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
