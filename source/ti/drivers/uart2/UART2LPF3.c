/*
 * Copyright (c) 2021-2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== UART2LPF3.c ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/dma/UDMALPF3.h>

#include <ti/drivers/uart2/UART2LPF3.h>
#include <ti/drivers/uart2/UART2Support.h>

/* driverlib header files */
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_evtsvt.h)
#include DeviceFamily_constructPath(driverlib/uart.h)
#include DeviceFamily_constructPath(driverlib/flash.h)

/* Headers required for intrinsics */
#if defined(__TI_COMPILER_VERSION__)
    #include <arm_acle.h>
#elif defined(__GNUC__)
    #include <arm_acle.h>
#elif defined(__IAR_SYSTEMS_ICC__)
    #include <intrinsics.h>
#else
    #error "Unsupported compiler"
#endif

/* Options for DMA write and read */
#define TX_CONTROL_OPTS (UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_4 | UDMA_MODE_BASIC)
#define RX_CONTROL_OPTS (UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_4 | UDMA_MODE_BASIC)

/* Maximum number of bytes that DMA can transfer */
#define MAX_DMA_SIZE (UDMA_XFER_SIZE_MAX)

/* Static functions */
static void UART2LPF3_eventCallback(UART2_Handle handle, uint32_t event, uint32_t data, void *userArg);
static void UART2LPF3_hwiIntFxn(uintptr_t arg);
static void UART2LPF3_initHw(UART2_Handle handle);
static void UART2LPF3_initIO(UART2_Handle handle);
static void UART2LPF3_finalizeIO(UART2_Handle handle);
static int UART2LPF3_preNotifyFxn(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg);
static int UART2LPF3_postNotifyFxn(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg);
static void UART2LPF3_hwiIntWrite(uintptr_t arg);
static void UART2LPF3_hwiIntRead(uintptr_t arg, uint32_t status);

/* Map UART2 data length to driverlib data length */
static const uint8_t dataLength[] = {
    UART_CONFIG_WLEN_5, /* UART2_DataLen_5 */
    UART_CONFIG_WLEN_6, /* UART2_DataLen_6 */
    UART_CONFIG_WLEN_7, /* UART2_DataLen_7 */
    UART_CONFIG_WLEN_8  /* UART2_DataLen_8 */
};

/* Map UART2 stop bits to driverlib stop bits */
static const uint8_t stopBits[] = {
    UART_CONFIG_STOP_ONE, /* UART2_StopBits_1 */
    UART_CONFIG_STOP_TWO  /* UART2_StopBits_2 */
};

/* Map UART2 parity type to driverlib parity type */
static const uint8_t parityType[] = {
    UART_CONFIG_PAR_NONE, /* UART2_Parity_NONE */
    UART_CONFIG_PAR_EVEN, /* UART2_Parity_EVEN */
    UART_CONFIG_PAR_ODD,  /* UART2_Parity_ODD */
    UART_CONFIG_PAR_ZERO, /* UART2_Parity_ZERO */
    UART_CONFIG_PAR_ONE   /* UART2_Parity_ONE */
};

/*
 *  ======== uartDmaEnable ========
 *  Atomic version of DriverLib UARTEnableDMA()
 */
static inline void uartDmaEnable(uint32_t base, uint32_t DMAFlags)
{
    uintptr_t key;

    key = HwiP_disable();
    /* Set the requested bits in the UART DMA control register. */
    UARTEnableDMA(base, DMAFlags);

    HwiP_restore(key);
}

/*
 *  ======== uartDmaDisable ========
 *  Atomic version of DriverLib UARTDisableDMA()
 */
static inline void uartDmaDisable(uint32_t base, uint32_t DMAFlags)
{
    uintptr_t key;

    key = HwiP_disable();
    /* Clear the requested bits in the UART DMA control register. */
    UARTDisableDMA(base, DMAFlags);

    HwiP_restore(key);
}

/*
 *  ======== UART2LPF3_getRxData ========
 *  Must be called with HWI disabled.
 */
static inline size_t UART2LPF3_getRxData(UART2_Handle handle, size_t size)
{
    UART2LPF3_Object *object         = handle->object;
    UART2LPF3_HWAttrs const *hwAttrs = handle->hwAttrs;
    size_t consumed                  = 0;
    uint8_t data;

    /* While RX FIFO is not empty */
    while (UARTCharAvailable(hwAttrs->baseAddr) && size)
    {
        /* Read directly from DATA register */
        data = HWREG(hwAttrs->baseAddr + UART_O_DR);
        RingBuf_put(&object->rxBuffer, data);
        ++consumed;
        --size;
    }

    return consumed;
}

/*
 *  ======== dmaChannelNum ========
 *  Get the channel number from the channel bit mask
 */
static inline uint32_t dmaChannelNum(uint32_t x)
{
#if defined(__TI_COMPILER_VERSION__)
    return ((uint32_t)__clz(__rbit(x)));
#elif defined(__GNUC__)
    return ((uint32_t)__builtin_ctz(x));
#elif defined(__IAR_SYSTEMS_ICC__)
    return ((uint32_t)__CLZ(__RBIT(x)));
#else
    #error "Unsupported compiler"
#endif
}

/*
 *  ======== UART2LPF3_getRxStatus ========
 *  Get the left-most bit set in the RX error status (OE, BE, PE, FE)
 *  read from the RSR register:
 *      bit#   3   2   1   0
 *             OE  BE  PE  FE
 *  e.g., if OE and FE are both set, OE wins.  This will make it easier
 *  to convert an RX error status to a UART2 error code.
 */
static inline uint32_t UART2LPF3_getRxStatus(uint32_t bitMask)
{
#if defined(__TI_COMPILER_VERSION__)
    return ((uint32_t)(bitMask & (0x80000000 >> __clz(bitMask))));
#elif defined(__GNUC__)
    return ((uint32_t)(bitMask & (0x80000000 >> __builtin_clz(bitMask))));
#elif defined(__IAR_SYSTEMS_ICC__)
    return ((uint32_t)(bitMask & (0x80000000 >> __CLZ(bitMask))));
#else
    #error "Unsupported compiler"
#endif
}

/*
 * Function for checking whether flow control is enabled.
 */
static inline bool UART2LPF3_isFlowControlEnabled(UART2LPF3_HWAttrs const *hwAttrs)
{
    return hwAttrs->flowControl == UART2_FLOWCTRL_HARDWARE;
}

/*
 *  ======== UART2_close ========
 */
void UART2_close(UART2_Handle handle)
{
    UART2LPF3_Object *object         = handle->object;
    UART2LPF3_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Disable UART and interrupts. */
    UARTDisableInt(hwAttrs->baseAddr,
                   UART_INT_TX | UART_INT_RX | UART_INT_RT | UART_INT_OE | UART_INT_BE | UART_INT_PE | UART_INT_FE |
                       UART_INT_CTS | UART_INT_TXDMADONE | UART_INT_RXDMADONE);

    /* Both these functions will only run conditionally on writeInUse and
     * readInUse, respectively
     */
    UART2_writeCancel(handle);
    UART2_readCancel(handle);

    /* Releases Power constraint if rx is enabled. */
    UART2_rxDisable(handle);

    /* Disable UART. This function will wait until TX FIFO is empty before
     * shutting down peripheral, otherwise the BUSY-bit will remain set and
     * can cause the peripheral to get stuck.
     */
    UARTDisable(hwAttrs->baseAddr);
    object->state.txEnabled = false;

    /* Deallocate pins */
    UART2LPF3_finalizeIO(handle);

    HwiP_destruct(&(object->hwi));
    SemaphoreP_destruct(&(object->writeSem));
    SemaphoreP_destruct(&(object->readSem));

    /* Unregister power notification objects */
    Power_unregisterNotify(&object->preNotify);
    Power_unregisterNotify(&object->postNotify);

    /* Release power dependency - i.e. potentially power down serial domain. */
    Power_releaseDependency(hwAttrs->powerID);

    Power_releaseDependency(PowerLPF3_PERIPH_DMA);

    object->state.opened = false;
}

/*
 *  ======== UART2_flushRx ========
 */
void UART2_flushRx(UART2_Handle handle)
{
    UART2LPF3_Object *object         = handle->object;
    UART2LPF3_HWAttrs const *hwAttrs = handle->hwAttrs;
    uintptr_t key;

    key = HwiP_disable();
    UART2Support_dmaStopRx(handle);
    HwiP_restore(key);

    RingBuf_flush(&object->rxBuffer);

    /* Read RX FIFO until empty */
    while (UARTCharAvailable(hwAttrs->baseAddr))
    {
        UARTGetCharNonBlocking(hwAttrs->baseAddr);
    }

    /* Clear any read errors */
    UARTClearRxError(hwAttrs->baseAddr);

    key = HwiP_disable();
    /* Start new RX transaction */
    UART2Support_dmaStartRx(handle);
    HwiP_restore(key);
}

/*
 *  ======== UART2_open ========
 */
UART2_Handle UART2_open(uint_least8_t index, UART2_Params *params)
{
    UART2_Handle handle = NULL;
    uintptr_t key;
    UART2LPF3_Object *object;
    UART2LPF3_HWAttrs const *hwAttrs;
    HwiP_Params hwiParams;

    if (index < UART2_count)
    {
        handle  = (UART2_Handle) & (UART2_config[index]);
        hwAttrs = handle->hwAttrs;
        object  = handle->object;
    }
    else
    {
        return NULL;
    }

    /* Check for callback when in UART2_Mode_CALLBACK */
    if (((params->readMode == UART2_Mode_CALLBACK) && (params->readCallback == NULL)) ||
        ((params->writeMode == UART2_Mode_CALLBACK) && (params->writeCallback == NULL)))
    {
        return NULL;
    }

    key = HwiP_disable();

    if (object->state.opened)
    {
        HwiP_restore(key);
        return NULL;
    }
    object->state.opened = true;

    HwiP_restore(key);

    object->state.rxEnabled       = false;
    object->state.txEnabled       = false;
    object->state.rxCancelled     = false;
    object->state.txCancelled     = false;
    object->state.overrunActive   = false;
    object->state.inReadCallback  = false;
    object->state.inWriteCallback = false;
    object->state.overrunCount    = 0;
    object->state.readMode        = params->readMode;
    object->state.writeMode       = params->writeMode;
    object->state.readReturnMode  = params->readReturnMode;
    object->readCallback          = params->readCallback;
    object->writeCallback         = params->writeCallback;
    object->eventCallback         = params->eventCallback;
    object->eventMask             = params->eventMask;
    object->baudRate              = params->baudRate;
    object->stopBits              = params->stopBits;
    object->dataLength            = params->dataLength;
    object->parityType            = params->parityType;
    object->userArg               = params->userArg;

    /* Set UART transaction variables to defaults. */
    object->writeBuf   = NULL;
    object->readBuf    = NULL;
    object->writeCount = 0;
    object->readCount  = 0;
    object->writeSize  = 0;
    object->readSize   = 0;
    object->rxStatus   = 0;
    object->txStatus   = 0;
    object->rxSize     = 0;
    object->txSize     = 0;
    object->readInUse  = false;
    object->writeInUse = false;

    /* Set the event mask to 0 if the callback is NULL to simplify checks */
    if (object->eventCallback == NULL)
    {
        object->eventCallback = UART2LPF3_eventCallback;
        object->eventMask     = 0;
    }

    /* Register power dependency - i.e. power up and enable clock for UART. */
    Power_setDependency(hwAttrs->powerID);

    /* Register power dependency on UDMA driver */
    Power_setDependency(PowerLPF3_PERIPH_DMA);

    RingBuf_construct(&object->rxBuffer, hwAttrs->rxBufPtr, hwAttrs->rxBufSize);
    RingBuf_construct(&object->txBuffer, hwAttrs->txBufPtr, hwAttrs->txBufSize);

    UARTDisable(hwAttrs->baseAddr);

    /* Initialize the UART hardware module. Enable the UART and FIFO, but do not enable RX or TX yet. */
    UART2LPF3_initHw(handle);

    /* Configure IOs */
    UART2LPF3_initIO(handle);

    /* Register notification functions */
    Power_registerNotify(&object->preNotify, PowerLPF3_ENTERING_STANDBY, UART2LPF3_preNotifyFxn, (uintptr_t)handle);
    Power_registerNotify(&object->postNotify, PowerLPF3_AWAKE_STANDBY, UART2LPF3_postNotifyFxn, (uintptr_t)handle);

    /* Create Hwi object for this UART peripheral. */
    HwiP_Params_init(&hwiParams);
    hwiParams.arg      = (uintptr_t)handle;
    hwiParams.priority = hwAttrs->intPriority;
    HwiP_construct(&(object->hwi), hwAttrs->intNum, UART2LPF3_hwiIntFxn, &hwiParams);

    SemaphoreP_constructBinary(&(object->readSem), 0);
    SemaphoreP_constructBinary(&(object->writeSem), 0);

    /* Set rx src and tx dst addresses in open, since these won't change */
    hwAttrs->dmaRxTableEntryPri->pSrcEndAddr = (void *)(hwAttrs->baseAddr + UART_O_DR);
    hwAttrs->dmaTxTableEntryPri->pDstEndAddr = (void *)(hwAttrs->baseAddr + UART_O_DR);

    /* UART opened successfully */
    return handle;
}

/*
 *  ======== UART2Support_disableRx ========
 */
void UART2Support_disableRx(UART2_HWAttrs const *hwAttrs)
{
    UARTDisableInt(hwAttrs->baseAddr,
                   UART_INT_OE | UART_INT_BE | UART_INT_PE | UART_INT_FE | UART_INT_RT | UART_INT_RX |
                       UART_INT_RXDMADONE);

    UARTClearInt(hwAttrs->baseAddr,
                 UART_INT_OE | UART_INT_BE | UART_INT_PE | UART_INT_FE | UART_INT_RT | UART_INT_RX |
                     UART_INT_RXDMADONE);

    HWREG(hwAttrs->baseAddr + UART_O_CTL) &= ~UART_CTL_RXE;
}

/*
 *  ======== UART2Support_disableTx ========
 */
void UART2Support_disableTx(UART2_HWAttrs const *hwAttrs)
{
    HWREG(hwAttrs->baseAddr + UART_O_CTL) &= ~(UART_CTL_TXE);
}

/*
 *  ======== UART2Support_dmaStartRx ========
 *  For mutual exclusion, must be called with HWI disabled.
 */
void UART2Support_dmaStartRx(UART2_Handle handle)
{
    UART2LPF3_Object *object                   = handle->object;
    UART2LPF3_HWAttrs const *hwAttrs           = handle->hwAttrs;
    volatile uDMAControlTableEntry *rxDmaEntry = hwAttrs->dmaRxTableEntryPri;
    unsigned char *dstAddr;

    if (object->state.rxEnabled == false)
    {
        /* DMA RX not enabled. No need to return a status code, this function should never be called if RX is disabled
         */
        return;
    }

    /* Stop DMA RX which updates the ring buffer count */
    UART2Support_dmaStopRx(handle);

    /* Decide whether to either read from the FIFO into the ring buffer, into the user-buffer, or do nothing */
    if ((object->rxSize == 0 && (object->readBuf == NULL || object->readCount == 0)) || /* a) */
        (RingBuf_getCount(&object->rxBuffer) >= object->readCount) ||                   /* b) */
        (object->state.readMode == UART2_Mode_NONBLOCKING))                             /* c) */
    {
        /* Setup a DMA transaction from FIFO to ring buffer if either
         * a) Currently no active DMA RX transaction, and (no user-buffer available or no more bytes left to read)
         * b) There are enough bytes in the ring buffer for this current read operation
         * c) The driver is setup for a nonblocking read
         */
        object->rxSize              = RingBuf_putPointer(&object->rxBuffer, &dstAddr);
        object->state.readToRingbuf = true;
    }
    else if (object->rxSize > 0 && (object->readBuf == NULL || object->readCount == 0))
    {
        /* If there is an active DMA transaction into the ring buffer, and there is no user-buffer available,
         * then keep doing that transaction. Do nothing further here
         */
        return;
    }
    else if (object->readBuf != NULL)
    {
        /* If there is a user-buffer available, and neither of the cases above are true, then the driver should setup
         * a DMA transaction into the the user-buffer. Offset the transaction with the number of bytes we already have
         * in the ring buffer. The bytes in the ring buffer will be copied out at a later stage, in UART2_read
         */
        object->rxSize              = object->readCount - RingBuf_getCount(&object->rxBuffer);
        dstAddr                     = object->readBuf + object->bytesRead + RingBuf_getCount(&object->rxBuffer);
        object->state.readToRingbuf = false;
    }
    else
    {
        /* It should not be possible to reach here */
        return;
    }

    if (object->rxSize > 0)
    {
        object->state.overrunActive = false;

        /* Limit the transaction to the maximum DMA transfer size */
        if (object->rxSize > MAX_DMA_SIZE)
        {
            object->rxSize = MAX_DMA_SIZE;
        }

        rxDmaEntry->pDstEndAddr = dstAddr + object->rxSize - 1;
        rxDmaEntry->control     = RX_CONTROL_OPTS;

        /* Set the size in the control options */
        rxDmaEntry->control |= UDMALPF3_SET_TRANSFER_SIZE(object->rxSize);

        /* enable burst mode */
        HWREG(DMA_BASE + DMA_O_SETBURST) = 1 << dmaChannelNum(hwAttrs->rxChannelMask);
        UDMALPF3_channelEnable(hwAttrs->rxChannelMask);

        uartDmaEnable(hwAttrs->baseAddr, UART_DMA_RX);

        UARTEnableInt(hwAttrs->baseAddr, UART_INT_RXDMADONE);
    }
}

/*
 *  ======== UART2Support_dmaStartTx ========
 *  For mutual exclusion, must be called with HWI disabled.
 */
void UART2Support_dmaStartTx(UART2_Handle handle)
{
    UART2LPF3_Object *object         = handle->object;
    UART2LPF3_HWAttrs const *hwAttrs = handle->hwAttrs;
    volatile uDMAControlTableEntry *txDmaEntry;
    unsigned char *srcAddr;

    if (object->txSize > 0)
    {
        /* DMA TX already in progress */
        return;
    }

    /* If nonblocking, set txSize to available bytes in ring buffer. Set source address to the ring buffer. */
    if (object->state.writeMode == UART2_Mode_NONBLOCKING)
    {
        object->txSize = RingBuf_getPointer(&object->txBuffer, &srcAddr);
    }
    else
    {
        /* Blocking or callback mode */
        object->txSize = object->writeCount;
        srcAddr        = (unsigned char *)object->writeBuf + object->bytesWritten;
    }

    if (object->txSize > 0)
    {
        UARTDisableInt(hwAttrs->baseAddr, UART_INT_EOT);

        if ((object->eventMask & UART2_EVENT_TX_BEGIN) && object->eventCallback)
        {
            object->eventCallback(handle, UART2_EVENT_TX_BEGIN, 0, object->userArg);
        }
        if (object->txSize > MAX_DMA_SIZE)
        {
            object->txSize = MAX_DMA_SIZE;
        }

        txDmaEntry              = hwAttrs->dmaTxTableEntryPri;
        txDmaEntry->pSrcEndAddr = srcAddr + object->txSize - 1;

        txDmaEntry->control = TX_CONTROL_OPTS;

        /* Set the size in the control options */
        txDmaEntry->control |= UDMALPF3_SET_TRANSFER_SIZE(object->txSize);

        UDMALPF3_channelEnable(hwAttrs->txChannelMask);

        uartDmaEnable(hwAttrs->baseAddr, UART_DMA_TX);

        UARTEnableInt(hwAttrs->baseAddr, UART_INT_TXDMADONE);

        if (object->state.txEnabled == false)
        {
            /* Set standby constraint to guarantee transaction. Also set flash constraint if necessary */
            UART2Support_powerSetConstraint(handle, true);
            object->state.txEnabled = true;
        }
    }
}

/*
 *  ======== UART2Support_dmaStopRx ========
 *  For mutual exclusion, must be called with HWI disabled.
 */
void UART2Support_dmaStopRx(UART2_Handle handle)
{
    UART2LPF3_Object *object         = handle->object;
    UART2LPF3_HWAttrs const *hwAttrs = handle->hwAttrs;
    uint32_t bytesRemaining;
    uint32_t rxCount;

    /* If there is a currently active DMA RX transaction */
    if (object->rxSize > 0)
    {
        /* Disable the RX DMA channel and stop the transfer. Disable and clear interrupts */
        UDMALPF3_channelDisable(hwAttrs->rxChannelMask);
        uartDmaDisable(hwAttrs->baseAddr, UART_DMA_RX);
        UDMALPF3_clearInterrupt(hwAttrs->rxChannelMask);
        UARTDisableInt(hwAttrs->baseAddr, UART_INT_RXDMADONE);
        UARTClearInt(hwAttrs->baseAddr, UART_INT_RXDMADONE);

        /* Get the number of bytes that remain to be transferred */
        bytesRemaining = uDMAGetChannelSize(hwAttrs->dmaRxTableEntryPri);
        rxCount        = object->rxSize - bytesRemaining;

        /* If the driver is currently reading data into the ring buffer, update the ring buffer count with the
         * number of bytes transferred into it through DMA
         */
        if (object->state.readToRingbuf)
        {
            RingBuf_putAdvance(&object->rxBuffer, rxCount);
        }
        else
        {
            /* If the driver was reading data into the user-buffer, we update the number of bytes read,
             * and number of bytes still to read
             */
            object->readCount -= rxCount;
            object->bytesRead += rxCount;
        }

        /* Set the DMA rxsize to 0 to indicate that there is no active DMA transaction ongoing */
        object->rxSize = 0;
    }
}

/*
 *  ======== UART2Support_dmaStopTx ========
 *  For mutual exclusion, must be called with HWI disabled.
 */
uint32_t UART2Support_dmaStopTx(UART2_Handle handle)
{
    UART2LPF3_Object *object         = handle->object;
    UART2LPF3_HWAttrs const *hwAttrs = handle->hwAttrs;
    uint32_t bytesRemaining          = 0;
    uint32_t txCount;

    /* If there is currently an active DMA TX transaction */
    if (object->txSize > 0)
    {
        /* Disable the TX DMA channel and stop the transfer. Disable and clear interrupts */
        UDMALPF3_channelDisable(hwAttrs->txChannelMask);
        uartDmaDisable(hwAttrs->baseAddr, UART_DMA_TX);
        UDMALPF3_clearInterrupt(hwAttrs->txChannelMask);
        UARTDisableInt(hwAttrs->baseAddr, UART_INT_TXDMADONE);
        UARTClearInt(hwAttrs->baseAddr, UART_INT_TXDMADONE);

        /* Get the number of bytes that remain to be transferred */
        bytesRemaining = uDMAGetChannelSize(hwAttrs->dmaTxTableEntryPri);
        txCount        = object->txSize - bytesRemaining;

        /* If the driver is currently doing a nonblocking write, update the ring buffer */
        if (object->state.writeMode == UART2_Mode_NONBLOCKING)
        {
            RingBuf_getConsume(&object->txBuffer, txCount);
        }
        else
        {
            /* If the driver was writing data from the user-buffer, we update the number of bytes written,
             * and number of bytes still to write
             */
            object->bytesWritten += txCount;
            object->writeCount -= txCount;
        }

        /* Set the DMA txSize to 0 to indicate that there is no active DMA transaction ongoing */
        object->txSize = 0;
    }

    return bytesRemaining;
}

/*
 *  ======== UART2Support_enableInts ========
 *  Function to enable receive, receive timeout, and error interrupts
 */
void UART2Support_enableInts(UART2_Handle handle)
{
    UART2LPF3_Object *object         = handle->object;
    UART2LPF3_HWAttrs const *hwAttrs = handle->hwAttrs;

    if (object->eventCallback)
    {
        if (object->eventMask & UART2_EVENT_OVERRUN)
        {
            UARTClearInt(hwAttrs->baseAddr, UART_INT_OE);
            UARTEnableInt(hwAttrs->baseAddr, UART_INT_OE);
        }
    }
    UARTEnableInt(hwAttrs->baseAddr, UART_INT_RT);
}

/*
 *  ======== UART2Support_enableRx ========
 *  Call with interrupts disabled
 */
void UART2Support_enableRx(UART2_HWAttrs const *hwAttrs)
{
    /* Enable RX but not interrupts, since we may be using DMA */
    HWREG(hwAttrs->baseAddr + UART_O_CTL) |= UART_CTL_RXE;
}

/*
 *  ======== UART2Support_enableTx ========
 *  Enable TX - interrupts must be disabled
 */
void UART2Support_enableTx(UART2_HWAttrs const *hwAttrs)
{
    HWREG(hwAttrs->baseAddr + UART_O_CTL) |= UART_CTL_TXE;
}

/*
 *  ======== UART2Support_powerRelConstraint ========
 */
void UART2Support_powerRelConstraint(__attribute__((unused)) UART2_Handle handle,
                                     __attribute__((unused)) bool relFlashConstraint)
{
    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*
 *  ======== UART2Support_powerSetConstraint ========
 */
void UART2Support_powerSetConstraint(__attribute__((unused)) UART2_Handle handle,
                                     __attribute__((unused)) bool setFlashConstraint)
{
    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*
 *  ======== UART2Support_rxStatus2ErrorCode ========
 *  Convert RX status (OE, BE, PE, FE) to a UART2 error code.
 */
int_fast16_t UART2Support_rxStatus2ErrorCode(uint32_t errorData)
{
    uint32_t status;

    status = UART2LPF3_getRxStatus(errorData);
    return -((int_fast16_t)status);
}

/*
 *  ======== UART2Support_sendData ========
 *  Function to send data
 */
uint32_t UART2Support_sendData(UART2_HWAttrs const *hwAttrs, size_t size, uint8_t *buf)
{
    uint32_t writeCount = 0;

    while (size && UARTSpaceAvailable(hwAttrs->baseAddr))
    {
        UARTPutCharNonBlocking(hwAttrs->baseAddr, *buf);
        buf++;
        writeCount++;
        size--;
    }

    return writeCount;
}

/*
 *  ======== UART2Support_txDone ========
 *  Used when interrupts are not enabled.
 */
bool UART2Support_txDone(UART2_HWAttrs const *hwAttrs)
{
    return !(UARTBusy(hwAttrs->baseAddr));
}

/*
 *  ======== UART2Support_uartRxError ========
 *  Function to clear RX errors
 */
int UART2Support_uartRxError(UART2_HWAttrs const *hwAttrs)
{
    int status = UART2_STATUS_SUCCESS;
    uint32_t errStatus;

    /* Check for RX error since the last read */
    errStatus = UARTGetRxError(hwAttrs->baseAddr);
    status    = UART2Support_rxStatus2ErrorCode(errStatus);
    UARTClearRxError(hwAttrs->baseAddr); /* Clear receive errors */

    return status;
}

/*
 *  ======== UART2LPF3_eventCallback ========
 *  A dummy event callback function in case the user didn't provide one
 */
static void UART2LPF3_eventCallback(UART2_Handle handle, uint32_t event, uint32_t data, void *userArg)
{}

/*
 *  ======== UART2LPF3_hwiIntRead ========
 *  Function called by Hwi to handle read-related interrupt
 */
static void UART2LPF3_hwiIntRead(uintptr_t arg, uint32_t status)
{
    UART2_Handle handle              = (UART2_Handle)arg;
    UART2LPF3_HWAttrs const *hwAttrs = handle->hwAttrs;
    UART2LPF3_Object *object         = handle->object;
    void *readBufCopy;

    /* Temporarily stop RX transaction */
    UART2Support_dmaStopRx(handle);

    /* Read timeout interrupt */
    if (status & UART_INT_RT)
    {
        /* If the read transaction has a user buffer as destination,
         * copy as much as we can from the FIFO to the buffer
         */
        if (!(object->state.readToRingbuf))
        {
            while (UARTCharAvailable(hwAttrs->baseAddr) && object->readCount)
            {
                uint8_t data                           = HWREG(hwAttrs->baseAddr + UART_O_DR);
                *(object->readBuf + object->bytesRead) = data;
                object->bytesRead++;
                object->readCount--;
            }
        }

        /* If the read transaction is setup with the ring buffer as destination then copy as much
         * as we can can from the FIFO into the ring buffer
         */
        if (object->state.readToRingbuf)
        {
            UART2LPF3_getRxData(handle, RingBuf_space(&object->rxBuffer));
        }
    }

    /* Do not invoke callback or post semaphore if there is no active read, or readMode is nonblocking */
    if (object->readInUse && object->state.readMode != UART2_Mode_NONBLOCKING)
    {
        /* Read-transaction is complete if either
         * a) ReadReturnMode is partial and we received a read-timeout
         * b) There are no more bytes to read
         */
        if (((object->state.readReturnMode == UART2_ReadReturnMode_PARTIAL) && (status & UART_INT_RT)) ||
            (object->readCount == 0))
        {
            object->readInUse = false;
            object->readCount = 0;

            /* Set readBuf to NULL, but first make a copy to pass to the callback. We cannot set it to NULL after the
             * callback in case another read was issued from the callback.
             */
            readBufCopy     = object->readBuf;
            object->readBuf = NULL;

            if (object->state.readMode == UART2_Mode_CALLBACK)
            {
                object->readCallback(handle, readBufCopy, object->bytesRead, object->userArg, UART2_STATUS_SUCCESS);
            }
            else
            {
                /* Blocking mode. Post semaphore to unblock reading task */
                SemaphoreP_post(&object->readSem);
            }
        }
    }

    /* Start another RX transaction */
    UART2Support_dmaStartRx(handle);
}

/*
 *  ======== UART2LPF3_hwiIntWrite ========
 *  Function called by Hwi to handle write-related interrupt
 */
static void UART2LPF3_hwiIntWrite(uintptr_t arg)
{
    UART2_Handle handle              = (UART2_Handle)arg;
    UART2LPF3_HWAttrs const *hwAttrs = handle->hwAttrs;
    UART2LPF3_Object *object         = handle->object;

    /* DMA transaction finished. Restart in case there are more bytes to write */
    UART2Support_dmaStopTx(handle);
    UART2Support_dmaStartTx(handle);

    if ((object->state.writeMode == UART2_Mode_CALLBACK) && (object->writeCount == 0) && object->writeInUse)
    {
        object->writeInUse = false;
        object->writeCallback(handle,
                              (void *)object->writeBuf,
                              object->bytesWritten,
                              object->userArg,
                              UART2_STATUS_SUCCESS);
    }

    if (object->txSize == 0)
    {
        /* No more data pending in the TX buffer, wait for it to finish
         * shifting out of the transmit shift register.
         */
        UARTEnableInt(hwAttrs->baseAddr, UART_INT_EOT);
    }
}

/*
 *  ======== UART2LPF3_hwiIntFxn ========
 *  Hwi function that processes UART interrupts.
 */
static void UART2LPF3_hwiIntFxn(uintptr_t arg)
{
    uint32_t status;
    uint32_t errStatus = 0;
    uint32_t event;
    UART2_Handle handle              = (UART2_Handle)arg;
    UART2LPF3_HWAttrs const *hwAttrs = handle->hwAttrs;
    UART2LPF3_Object *object         = handle->object;

    /* Clear interrupts */
    status = UARTIntStatus(hwAttrs->baseAddr, true);
    UARTClearInt(hwAttrs->baseAddr, status);

    if ((status & (UART_INT_OE | UART_INT_BE | UART_INT_PE | UART_INT_FE)) && object->eventCallback)
    {
        if (status & UART_INT_OE)
        {
            /* Overrun error occurred, get what we can. No need to stop
             * the DMA, should already have stopped by now.
             */
            UART2LPF3_getRxData(handle, RingBuf_space(&object->rxBuffer));
            /* Throw away the rest in order to clear the overrun */
            while (!(HWREG(hwAttrs->baseAddr + UART_O_FR) & UART_FR_RXFE))
            {
                volatile uint8_t data = HWREG(hwAttrs->baseAddr + UART_O_DR);
                (void)data;
            }

            object->state.overrunCount++;

            if (object->state.overrunActive == false)
            {
                object->state.overrunActive = true;
            }
        }

        errStatus = UARTGetRxError(hwAttrs->baseAddr);
        event     = UART2LPF3_getRxStatus(errStatus & object->eventMask);

        if (event && object->eventCallback)
        {
            object->eventCallback(handle, event, object->state.overrunCount, object->userArg);
        }
        object->rxStatus = UART2Support_rxStatus2ErrorCode(errStatus);
    }

    /* If receive timeout or RX DMA done interrupt */
    if (status & (UART_INT_RT | UART_INT_RXDMADONE))
    {
        UART2LPF3_hwiIntRead(arg, status);
    }

    if (status & (UART_INT_TXDMADONE) && (object->txSize > 0))
    {
        UART2LPF3_hwiIntWrite(arg);
    }

    if (status & (UART_INT_EOT))
    {
        /* End of Transmission occurred. Make sure TX FIFO is truly empty before disabling TX */
        while (HWREG(hwAttrs->baseAddr + UART_O_FR) & UART_FR_BUSY) {}
        /* Disable TX */
        HWREG(hwAttrs->baseAddr + UART_O_CTL) &= ~UART_CTL_TXE;

        if (object->state.txEnabled)
        {
            object->state.txEnabled = false;

            if ((object->eventMask & UART2_EVENT_TX_FINISHED) && object->eventCallback)
            {
                object->eventCallback(handle, UART2_EVENT_TX_FINISHED, 0, object->userArg);
            }

            /* Release standby constraint because there are no active transactions. Also release flash constraint */
            UART2Support_powerRelConstraint(handle, true);
        }

        UARTDisableInt(hwAttrs->baseAddr, UART_INT_EOT);

        if (object->state.writeMode == UART2_Mode_BLOCKING)
        {
            /* Unblock write-function, which is waiting for EOT */
            SemaphoreP_post(&object->writeSem);
        }
    }
}

/*
 *  ======== UART2LPF3_initHw ========
 */
static void UART2LPF3_initHw(UART2_Handle handle)
{
    ClockP_FreqHz freq;
    UART2LPF3_Object *object         = handle->object;
    UART2LPF3_HWAttrs const *hwAttrs = handle->hwAttrs;

    ClockP_getCpuFreq(&freq);
#if DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX
    /* On CC27XX, the UART reference clock is half of SVTCLK */
    freq.lo /= 2;
#endif

    /* Configure frame format and baudrate. UARTConfigSetExpClk() disables
     * the UART and does not re-enable it, so call this function first.
     */
    UARTConfigSetExpClk(hwAttrs->baseAddr,
                        freq.lo,
                        object->baudRate,
                        dataLength[object->dataLength] | stopBits[object->stopBits] | parityType[object->parityType]);

    /* Clear all UART interrupts */
    UARTClearInt(hwAttrs->baseAddr,
                 UART_INT_OE | UART_INT_BE | UART_INT_PE | UART_INT_FE | UART_INT_RT | UART_INT_TX | UART_INT_RX |
                     UART_INT_CTS | UART_INT_TXDMADONE | UART_INT_RXDMADONE);

    /* Set TX interrupt FIFO level and RX interrupt FIFO level */
    UARTSetFifoLevel(hwAttrs->baseAddr, UART_FIFO_TX4_8, UART_FIFO_RX6_8);

    /* If Flow Control is enabled, configure hardware flow control for CTS and/or RTS. */
    if (UART2LPF3_isFlowControlEnabled(hwAttrs) && (hwAttrs->ctsPin != GPIO_INVALID_INDEX))
    {
        UARTEnableCTS(hwAttrs->baseAddr);
    }
    else
    {
        UARTDisableCTS(hwAttrs->baseAddr);
    }

    if (UART2LPF3_isFlowControlEnabled(hwAttrs) && (hwAttrs->rtsPin != GPIO_INVALID_INDEX))
    {
        UARTEnableRTS(hwAttrs->baseAddr);
    }
    else
    {
        UARTDisableRTS(hwAttrs->baseAddr);
    }

    if (hwAttrs->codingScheme == UART2LPF3_CODING_SIR)
    {
        /* Enable IrDA SIR Encoder/decoder */
        HWREG(hwAttrs->baseAddr + UART_O_CTL) |= UART_CTL_SIREN;
    }

    if (hwAttrs->codingScheme == UART2LPF3_CODING_SIR_LP)
    {
        /* Enable IrDA SIR low-power Encoder/decoder */
        HWREG(hwAttrs->baseAddr + UART_O_CTL) |= (UART_CTL_SIREN | UART_CTL_SIRLP);
        HWREG(hwAttrs->baseAddr + UART_O_UARTILPR) = hwAttrs->irLPClkDivider;
    }

    /* Enable UART FIFOs */
    UARTEnableFifo(hwAttrs->baseAddr);

    /* Enable the UART module, but not RX or TX */
    HWREG(hwAttrs->baseAddr + UART_O_CTL) |= UART_CTL_UARTEN;

    /* Enable FIFO concatenation if selected */
    if (hwAttrs->concatenateFIFO)
    {
        HWREG(hwAttrs->baseAddr + UART_O_CTL) |= UART_CTL_FCEN;
    }

    if ((hwAttrs->rxChannelMask > 0) && (hwAttrs->txChannelMask > 0))
    {
        /* Prevent DMA interrupt on UART2LPF3_open() */
        uartDmaDisable(hwAttrs->baseAddr, UART_DMA_RX);
        UDMALPF3_channelDisable(hwAttrs->rxChannelMask);
        UDMALPF3_clearInterrupt(hwAttrs->rxChannelMask);

        /* Disable the alternate DMA structure */
        UDMALPF3_disableAttribute(hwAttrs->rxChannelMask, UDMA_ATTR_ALTSELECT);
        UDMALPF3_disableAttribute(hwAttrs->txChannelMask, UDMA_ATTR_ALTSELECT);

        /* Mux RX DMA channel to uart peripheral */
        HWREG(EVTSVT_BASE + EVTSVT_O_DMACH0SEL + dmaChannelNum(hwAttrs->rxChannelMask) * sizeof(uint32_t)) = hwAttrs->rxChannelEvtMux;

        /* Mux RX DMA channel to uart peripheral */
        HWREG(EVTSVT_BASE + EVTSVT_O_DMACH0SEL + dmaChannelNum(hwAttrs->txChannelMask) * sizeof(uint32_t)) = hwAttrs->txChannelEvtMux;
    }
}

/*
 *  ======== UART2LPF3_initIO ========
 */
static void UART2LPF3_initIO(UART2_Handle handle)
{
    UART2LPF3_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Make sure RX and CTS pins have their input buffers enabled, then apply the correct mux */
    GPIO_setConfigAndMux(hwAttrs->txPin, GPIO_CFG_NO_DIR, hwAttrs->txPinMux);
    GPIO_setConfigAndMux(hwAttrs->rxPin, GPIO_CFG_INPUT, hwAttrs->rxPinMux);

    if (UART2LPF3_isFlowControlEnabled(hwAttrs))
    {
        GPIO_setConfigAndMux(hwAttrs->ctsPin, GPIO_CFG_INPUT, hwAttrs->ctsPinMux);
        GPIO_setConfigAndMux(hwAttrs->rtsPin, GPIO_CFG_NO_DIR, hwAttrs->rtsPinMux);
    }
}

/*
 *  ======== UART2LPF3_finalizeIO ========
 */
static void UART2LPF3_finalizeIO(UART2_Handle handle)
{
    UART2LPF3_HWAttrs const *hwAttrs = handle->hwAttrs;

    GPIO_resetConfig(hwAttrs->txPin);
    GPIO_resetConfig(hwAttrs->rxPin);

    if (UART2LPF3_isFlowControlEnabled(hwAttrs))
    {
        GPIO_resetConfig(hwAttrs->ctsPin);
        GPIO_resetConfig(hwAttrs->rtsPin);
    }
}

/*
 *  ======== UART2LPF3_preNotifyFxn ========
 *  Called by Power module when entering LPDS.
 */
static int UART2LPF3_preNotifyFxn(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg)
{
    /* Reconfigure the IO pins back to their GPIO configuration.
     * This ensures their states are retained while in standby.
     */
    UART2_Handle handle              = (UART2_Handle)clientArg;
    UART2LPF3_HWAttrs const *hwAttrs = handle->hwAttrs;

    if (eventType == PowerLPF3_ENTERING_STANDBY)
    {
        GPIO_PinConfig txConfig;

        if (hwAttrs->codingScheme == UART2LPF3_CODING_UART)
        {
            /* If UART-encoding is used then the pin must be set high during standby */
            txConfig = GPIO_CFG_OUTPUT_INTERNAL | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH;
        }
        else
        {
            /* If SIR-encoding is used then the pin must be set low during standby */
            txConfig = GPIO_CFG_OUTPUT_INTERNAL | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW;
        }

        GPIO_setConfigAndMux(hwAttrs->txPin, txConfig, GPIO_MUX_GPIO);

        if (UART2LPF3_isFlowControlEnabled(hwAttrs))
        {
            /* Assert RTS while in standby to be safe */
            GPIO_setConfigAndMux(hwAttrs->rtsPin,
                                 GPIO_CFG_OUTPUT_INTERNAL | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,
                                 GPIO_MUX_GPIO);
        }
    }

    return Power_NOTIFYDONE;
}

/*
 *  ======== UART2LPF3_postNotifyFxn ========
 *  Called by Power module when waking up from LPDS.
 */
static int UART2LPF3_postNotifyFxn(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg)
{
    /* Reconfigure the hardware if returning from sleep */
    if (eventType == PowerLPF3_AWAKE_STANDBY)
    {
        UART2LPF3_initHw((UART2_Handle)clientArg);
        /* Reconfigure the IOs from GPIO back to the UART peripheral *after* the peripheral has been reconfigured */
        UART2LPF3_initIO((UART2_Handle)clientArg);
    }

    return Power_NOTIFYDONE;
}
