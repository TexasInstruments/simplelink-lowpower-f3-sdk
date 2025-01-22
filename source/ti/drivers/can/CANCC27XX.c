/*
 * Copyright (c) 2023-2024, Texas Instruments Incorporated
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
 *  ======== CANCC27XX.c ========
 */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <ti/drivers/can/CANCC27XX.h>
#include <ti/drivers/CAN.h>
#include <ti/drivers/can/common/CANMCAN.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>

#include <ti/drivers/utils/StructRingBuf.h>
#include <ti/drivers/utils/Math.h>

#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/dpl/HwiP.h>

#include <third_party/mcan/MCAN.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/can.h)
#include DeviceFamily_constructPath(inc/hw_ckmd.h)
#include DeviceFamily_constructPath(inc/hw_fcfg.h) /* For AFOSC trim */
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_types.h)

/* CAN subsystem clock divider */
#define CANCC27XX_CANSS_CLK_DIVIDER CANSS_CLK_DIVIDE_BY_2

/* Oscillator frequency in kHz */
#define AFOSC_FREQ_KHZ 80000U

/* Hwi object for handling CAN interrupt */
static HwiP_Struct canHwi;

/* Counter for number of times Rx Ring buffer was full when there was a Rx
 * message available in Rx FIFO0/1 resulting in a lost message. This can be used
 * to fine tune the size of the Rx ring buffer when the CAN bus is under maximum
 * load.
 */
static uint32_t rxRingBufFullCnt = 0U;

static MCAN_TxBufElement txElem;
static MCAN_RxBufElement rxElem;

/* Default device-specific message RAM configuration:
 *  - Each standard filter element occupies 4-bytes.
 *  - Each extended filter element occupies 8-bytes.
 *  - Each Rx/Tx buffer occupies 72-bytes when CAN FD is enabled or 16-bytes
 *    for classic CAN.
 *  - Each Tx Event occupies 8-bytes.
 */
const CAN_MsgRamConfig CANCC27XX_defaultMsgRamConfig = {
    .stdFilterNum       = 0U,
    .extFilterNum       = 0U,
    .stdMsgIDFilterList = NULL,
    .extMsgIDFilterList = NULL,

    .rxFifoNum[0U]  = 30U,
    .rxFifoNum[1U]  = 0U,
    .rxBufNum       = 3U,
    .txBufNum       = 3U,
    .txFifoQNum     = 20U,
    .txFifoQMode    = 1U, /* Tx Queue mode */
    .txEventFifoNum = 8U,
};

/* Forward declarations */
static void CANCC27XX_hwiFxn(uintptr_t arg);
static bool CANCC27XX_isRxStructRingBufFull(CAN_Handle handle);
static void CANCC27XX_handleRxFifo(CAN_Handle handle, uint32_t fifoNum);
static void CANCC27XX_handleRxBuf(CAN_Handle handle);
static int_fast16_t CANCC27XX_setBitRate(const CAN_Config *config);
static int_fast16_t CANCC27XX_init(const CAN_Config *config,
                                   const CAN_MsgRamConfig *msgRamConfig,
                                   const CAN_BitRateTimingRaw *bitTiming,
                                   uint32_t tsPrescaler);
static void CANCC27XX_enableLoopback(bool externalModeEnable);

/* Definitions for extern functions defined in MCAN.h */

/*
 *  ======== MCAN_getMRAMOffset ========
 */
uint32_t MCAN_getMRAMOffset(void)
{
    return CANFD_SRAM_BASE;
}

/*
 *  ======== MCAN_writeReg ========
 */
void MCAN_writeReg(uint32_t offset, uint32_t value)
{
    uint32_t addr = offset;

    if (addr < CANFD_SRAM_BASE)
    {
        addr += CANFD_BASE;
    }

    HWREG(addr) = value;
}

/*
 *  ======== MCAN_writeMsgRam ========
 */
void MCAN_writeMsgRam(uint32_t offset, const uint8_t *src, size_t numBytes)
{
    (void)memcpy((void *)offset, src, numBytes);
}

/*
 *  ======== MCAN_readReg ========
 */
uint32_t MCAN_readReg(uint32_t offset)
{
    uint32_t addr = offset;

    if (addr < CANFD_SRAM_BASE)
    {
        addr += CANFD_BASE;
    }

    return HWREG(addr);
}

/*
 *  ======== MCAN_readMsgRam ========
 */
void MCAN_readMsgRam(uint8_t *dst, uint32_t offset, size_t numBytes)
{
    (void)memcpy(dst, (void *)offset, numBytes);
}

/*
 *  ======== CANCC27XX_hwiFxn ========
 */
static void CANCC27XX_hwiFxn(uintptr_t arg)
{
    CAN_Handle handle  = (CAN_Handle)arg;
    CAN_Object *object = (CAN_Object *)handle->object;
    int32_t rxCnt;
    MCAN_ProtocolStatus protStatus;
    MCAN_TxFifoQStatus fifoQStatus;
    MCAN_TxEventFifoStatus txEventFifoStatus;
    uint32_t canIntStatus;
    uint32_t event;
    uint32_t intStatus;
    uint32_t txOccurred = 0U;

    /* Get the masked CAN interrupt line 0 status */
    canIntStatus = CANGetMaskedIntStatus(CAN_INT_LINE0);

    /* Check MCAN interrupt line 0 event status */
    if ((canIntStatus & CAN_INT_INTL0) != 0U)
    {
        CANClearInt(CAN_INT_LINE0, CAN_INT_INTL0);

        /* Get the masked MCAN interrupt status */
        intStatus = MCAN_getIntStatus() & object->intMask;

        MCAN_clearIntStatus(intStatus);

        if ((intStatus & MCAN_INT_SRC_BUS_OFF_STATUS) != 0U)
        {
            MCAN_getProtocolStatus(&protStatus);

            /* Node is Bus Off when transmit error count >= 256 */
            if (protStatus.busOffStatus == 1U)
            {
                /* Error recovery - normal operation will resume after 129 occurrences of Bus Idle */
                MCAN_setOpMode(MCAN_OPERATION_MODE_NORMAL);
                event = CAN_EVENT_BUS_OFF;
            }
            else
            {
                event = CAN_EVENT_BUS_ON;
            }

            /* Bus Off interrupt is always enabled regardless of event mask
             * so we must check the event mask before executing the callback.
             */
            if ((event & object->eventMask) != 0U)
            {
                /* Call the event callback function provided by the application */
                object->eventCbk(handle, event, 0U, object->userArg);
            }
        }

        if ((intStatus & MCAN_INT_SRC_ERR_PASSIVE) != 0U)
        {
            MCAN_getProtocolStatus(&protStatus);

            if (protStatus.errPassive == 1U)
            {
                /* Node is Error Passive when either transmit or receiver error count >= 128 */
                event = CAN_EVENT_ERR_PASSIVE;
            }
            else
            {
                /* Node is Error Active when both transmit and receiver error count < 128 */
                event = CAN_EVENT_ERR_ACTIVE;
            }

            /* Call the event callback function provided by the application */
            object->eventCbk(handle, event, 0U, object->userArg);
        }

        if ((intStatus & MCAN_INT_SRC_RX_FIFO0_NEW_MSG) != 0U)
        {
            CANCC27XX_handleRxFifo(handle, MCAN_RX_FIFO_NUM_0);
        }

        if ((intStatus & MCAN_INT_SRC_RX_FIFO1_NEW_MSG) != 0U)
        {
            CANCC27XX_handleRxFifo(handle, MCAN_RX_FIFO_NUM_1);
        }

        if ((intStatus & MCAN_INT_SRC_DEDICATED_RX_BUFF_MSG) != 0U)
        {
            CANCC27XX_handleRxBuf(handle);
        }

        if ((intStatus & MCAN_INT_SRC_TRANS_COMPLETE) != 0U)
        {
            /* Read TX buffer transmission status if the Tx finished event mask
             * is set so it can be provided to the event callback.
             */
            if ((object->eventMask & CAN_EVENT_TX_FINISHED) != 0U)
            {
                txOccurred = MCAN_getTxBufTransmissionStatus();
            }

            if ((object->txFifoQNum != 0U) && (StructRingBuf_getCount(&object->txStructRingBuf) > 0))
            {
                MCAN_getTxFifoQStatus(&fifoQStatus);

                if (fifoQStatus.fifoFull == 0U)
                {
                    /* Return value can be ignored since count was checked */
                    (void)StructRingBuf_get(&object->txStructRingBuf, &txElem);

                    /* Return value can be ignored as the FIFO is not full */
                    (void)CAN_write(handle, &txElem);
                }
            }

            /* Source Tx complete interrupt is always enabled if the Tx ring
             * buffer size is non-zero so we must check the event mask before
             * executing the callback.
             */
            if ((object->eventMask & CAN_EVENT_TX_FINISHED) != 0U)
            {
                /* Call the event callback function provided by the application */
                object->eventCbk(handle, CAN_EVENT_TX_FINISHED, txOccurred, object->userArg);
            }
        }

        if ((intStatus & MCAN_INT_SRC_TX_EVT_FIFO_ELEM_LOST) != 0U)
        {
            if ((object->eventMask & CAN_EVENT_TX_EVENT_LOST) != 0U)
            {
                /* Call the event callback function provided by the application */
                object->eventCbk(handle, CAN_EVENT_TX_EVENT_LOST, 0U, object->userArg);
            }
        }

        if ((intStatus & MCAN_INT_SRC_TX_EVT_FIFO_NEW_ENTRY) != 0U)
        {
            MCAN_getTxEventFifoStatus(&txEventFifoStatus);

            if ((object->eventMask & CAN_EVENT_TX_EVENT_AVAIL) != 0U)
            {
                /* Call the event callback function provided by the application */
                object->eventCbk(handle, CAN_EVENT_TX_EVENT_AVAIL, txEventFifoStatus.fillLvl, object->userArg);
            }
        }

        if ((intStatus & MCAN_INT_SRC_RX_MASK) != 0U)
        {
            event = CAN_EVENT_RX_DATA_AVAIL;

            rxCnt = StructRingBuf_getCount(&object->rxStructRingBuf);

            if (rxCnt > 0)
            {
                /* Call the event callback function provided by the application */
                object->eventCbk(handle, CAN_EVENT_RX_DATA_AVAIL, (uint32_t)rxCnt, object->userArg);
            }
        }

        if ((intStatus & MCAN_INT_SRC_RX_FIFO0_MSG_LOST) != 0U)
        {
            /* Call the event callback function provided by the application */
            object->eventCbk(handle, CAN_EVENT_RX_FIFO_MSG_LOST, 0U, object->userArg);
        }

        if ((intStatus & MCAN_INT_SRC_RX_FIFO1_MSG_LOST) != 0U)
        {
            /* Call the event callback function provided by the application */
            object->eventCbk(handle, CAN_EVENT_RX_FIFO_MSG_LOST, 1U, object->userArg);
        }

        CANSSSetEndOfInt(CANSS_EOI_MCAN0);
    }
}

/*
 *  ======== CANCC27XX_isRxStructRingBufFull ========
 */
static bool CANCC27XX_isRxStructRingBufFull(CAN_Handle handle)
{
    CAN_Object *object = (CAN_Object *)handle->object;
    uint32_t isFull    = StructRingBuf_isFull(&object->rxStructRingBuf);

    if (isFull)
    {
        rxRingBufFullCnt++;

        if ((CAN_EVENT_RX_RING_BUFFER_FULL & object->eventMask) != 0U)
        {
            /* Call the event callback function provided by the application */
            object->eventCbk(handle, CAN_EVENT_RX_RING_BUFFER_FULL, rxRingBufFullCnt, object->userArg);
        }
    }

    return isFull;
}

/*
 *  ======== CANCC27XX_handleRxFifo ========
 */
static void CANCC27XX_handleRxFifo(CAN_Handle handle, uint32_t fifoNum)
{
    CAN_Object *object           = (CAN_Object *)handle->object;
    MCAN_RxFifoStatus fifoStatus = {0};

    MCAN_getRxFifoStatus(fifoNum, &fifoStatus);

    if ((fifoStatus.fillLvl > 0U) && !CANCC27XX_isRxStructRingBufFull(handle))
    {
        MCAN_readRxMsg(MCAN_MEM_TYPE_FIFO, fifoNum, &rxElem);
        /* Return value can be ignored since ring buffer is not full */
        (void)StructRingBuf_put(&object->rxStructRingBuf, &rxElem);

        fifoStatus.fillLvl--;

        while ((fifoStatus.fillLvl > 0U) && !CANCC27XX_isRxStructRingBufFull(handle))
        {
            MCAN_readRxMsg(MCAN_MEM_TYPE_FIFO, fifoNum, &rxElem);
            /* Return value can be ignored since ring buffer is not full */
            (void)StructRingBuf_put(&object->rxStructRingBuf, &rxElem);

            fifoStatus.fillLvl--;
            fifoStatus.getIdx++;

            /* Check for rollover */
            if (fifoStatus.getIdx >= object->rxFifoNum[fifoNum])
            {
                fifoStatus.getIdx = 0U;
            }
        }
    }

    /* Return value can be ignored since the inputs are known to be valid */
    (void)MCAN_setRxFifoAck(fifoNum, fifoStatus.getIdx);
}

/*
 *  ======== CANCC27XX_handleRxBuf ========
 */
static void CANCC27XX_handleRxBuf(CAN_Handle handle)
{
    CAN_Object *object                      = (CAN_Object *)handle->object;
    MCAN_RxNewDataStatus clearNewDataStatus = {0U};
    MCAN_RxNewDataStatus newDataStatus;
    uint32_t bufNum;

    MCAN_getNewDataStatus(&newDataStatus);

    /* Check for Rx messages in buffers 0-31 */
    if (newDataStatus.statusLow != 0U)
    {
        for (bufNum = 0U; bufNum < Math_MIN(object->rxBufNum, 32U); bufNum++)
        {
            if ((newDataStatus.statusLow & ((uint32_t)1U << bufNum)) != 0U)
            {
                if (!CANCC27XX_isRxStructRingBufFull(handle))
                {
                    MCAN_readRxMsg(MCAN_MEM_TYPE_BUF, bufNum, &rxElem);

                    (void)StructRingBuf_put(&object->rxStructRingBuf, &rxElem);
                }

                clearNewDataStatus.statusLow |= ((uint32_t)1U << bufNum);
            }
        }
    }

    if (object->rxBufNum > 32U)
    {
        /* Check for Rx messages in buffers 32-63 */
        if (newDataStatus.statusHigh != 0U)
        {
            for (bufNum = 0U; bufNum < Math_MIN((object->rxBufNum - 32U), 32U); bufNum++)
            {
                if ((newDataStatus.statusHigh & ((uint32_t)1U << bufNum)) != 0U)
                {
                    if (!CANCC27XX_isRxStructRingBufFull(handle))
                    {
                        MCAN_readRxMsg(MCAN_MEM_TYPE_BUF, (bufNum + 32U), &rxElem);

                        (void)StructRingBuf_put(&object->rxStructRingBuf, &rxElem);
                    }

                    clearNewDataStatus.statusHigh |= ((uint32_t)1U << bufNum);
                }
            }
        }
    }

    MCAN_clearNewDataStatus(&clearNewDataStatus);
}

/*
 *  ======== CANCC27XX_setBitRate ========
 *  This function is hard-coded for 40 MHz MCAN clock from AFOSC source.
 */
static int_fast16_t CANCC27XX_setBitRate(const CAN_Config *config)
{
    const CAN_HWAttrs *hwAttrs     = config->hwAttrs;
    int_fast16_t status            = CAN_STATUS_SUCCESS;
    MCAN_BitTimingParams bitTiming = {0};

    /* NOTE: Add 1 to each programmed bit time to get functional value and +1 for for sync segment.
     *    Bit Time = TSEG1 + TSEG2 + 1
     *    Bit Rate = (MCAN clock / Prescaler) / (Bit Time)
     *    Sample Point % = ((TSEG1 + 1) / (Bit Time)) * 100
     * - All TSEG1 and TSEG2 values above refer to the functional values
     *
     * Sampling Point % was chosen to be < 80 according to CiA 601 CAN FD Node
     * and System Design, Part 3 System Design Recommendation v1.0.0.
     */
    switch (hwAttrs->nominalBitRate)
    {
        case 125000U:
            /* 125kbps nominal with 40MHz clk and 75% sample point: ((40E6 / 2) / (119 + 40 + 1) = 125E3) */
            bitTiming.nomRatePrescaler  = 1U;
            bitTiming.nomTimeSeg1       = 118U;
            bitTiming.nomTimeSeg2       = 39U;
            bitTiming.nomSynchJumpWidth = 39U; /* typically set equal to seg 2 */
            break;

        case 250000U:
            /* 250kbps nominal with 40MHz clk and 75% sample point: ((40E6 / 1) / (119 + 40 + 1) = 250E3) */
            bitTiming.nomRatePrescaler  = 0U;
            bitTiming.nomTimeSeg1       = 118U;
            bitTiming.nomTimeSeg2       = 39U;
            bitTiming.nomSynchJumpWidth = 39U; /* typically set equal to seg 2 */
            break;

        case 500000U:
            /* 500kbps nominal with 40MHz clk and 75% sample point ((40E6 / 1) / (59 + 20 + 1) = 500E3) */
            bitTiming.nomRatePrescaler  = 0U;
            bitTiming.nomTimeSeg1       = 58U;
            bitTiming.nomTimeSeg2       = 19U;
            bitTiming.nomSynchJumpWidth = 19U; /* typically set equal to seg 2 */
            break;

        case 1000000U:
            /* 1Mbps nominal with 40MHz clk and 75% sample point ((40E6 / 1) / (29 + 10 + 1) = 1E6) */
            bitTiming.nomRatePrescaler  = 0U;
            bitTiming.nomTimeSeg1       = 28U;
            bitTiming.nomTimeSeg2       = 9U;
            bitTiming.nomSynchJumpWidth = 9U; /* typically set equal to seg 2 */
            break;

        default:
            status = CAN_STATUS_ERROR;
            break;
    }

    if (hwAttrs->enableBRS)
    {
        switch (hwAttrs->dataBitRate)
        {
            case 125000U:
                /* 125kbps with 40MHz clk and 75% sample point: ((40E6 / 10) / (23 + 8 + 1) = 125E3) */
                bitTiming.dataRatePrescaler  = 9U;
                bitTiming.dataTimeSeg1       = 22U;
                bitTiming.dataTimeSeg2       = 7U;
                bitTiming.dataSynchJumpWidth = 7U; /* typically set equal to seg 2 */
                break;

            case 250000U:
                /* 250kbps with 40MHz clk and 75% sample point: ((40E6 / 8) / (14 + 5 + 1) = 250E3) */
                bitTiming.dataRatePrescaler  = 7U;
                bitTiming.dataTimeSeg1       = 13U;
                bitTiming.dataTimeSeg2       = 4U;
                bitTiming.dataSynchJumpWidth = 4U; /* typically set equal to seg 2 */
                break;

            case 500000U:
                /* 500kbps with 40MHz clk and 75% sample point ((40E6 / 2) / (29 + 10 + 1) = 500E3) */
                bitTiming.dataRatePrescaler  = 1U;
                bitTiming.dataTimeSeg1       = 28U;
                bitTiming.dataTimeSeg2       = 9U;
                bitTiming.dataSynchJumpWidth = 9U; /* typically set equal to seg 2 */
                break;

            case 1000000U:
                /* 1Mbps with 40MHz clk and 75% sample point ((40E6 / 1) / (29 + 10 + 1) = 1E6) */
                bitTiming.dataRatePrescaler  = 0U;
                bitTiming.dataTimeSeg1       = 28U;
                bitTiming.dataTimeSeg2       = 9U;
                bitTiming.dataSynchJumpWidth = 9U; /* typically set equal to seg 2 */
                break;

            case 2000000U:
                /* 2Mbps with 40MHz clk and 75% sample point ((40E6 / 1) / (14 + 5 + 1) = 2E6) */
                bitTiming.dataRatePrescaler  = 0U;
                bitTiming.dataTimeSeg1       = 13U;
                bitTiming.dataTimeSeg2       = 4U;
                bitTiming.dataSynchJumpWidth = 4U;  /* typically set equal to seg 2 */
                bitTiming.tdcConfig.tdco     = 13U; /* typically set equal to seg 1 */
                break;

            case 4000000U:
                /* 4Mbps with 40MHz clk and 70% sample point ((40E6 / 1) / (6 + 3 + 1) = 4E6) */
                bitTiming.dataRatePrescaler  = 0U;
                bitTiming.dataTimeSeg1       = 5U;
                bitTiming.dataTimeSeg2       = 2U;
                bitTiming.dataSynchJumpWidth = 2U; /* typically set equal to seg 2 */
                bitTiming.tdcConfig.tdco     = 5U; /* typically set equal to seg 1 */
                break;

            case 5000000U:
                /* 5Mbps with 40MHz clk and 62.5% sample point ((40E6 / 1) / (4 + 3 + 1) = 5E6) */
                bitTiming.dataRatePrescaler  = 0U;
                bitTiming.dataTimeSeg1       = 3U;
                bitTiming.dataTimeSeg2       = 2U;
                bitTiming.dataSynchJumpWidth = 2U; /* typically set equal to seg 2 */
                bitTiming.tdcConfig.tdco     = 3U; /* typically set equal to seg 1 */
                break;

            default:
                status = CAN_STATUS_ERROR;
                break;
        }
    }

    if (status == CAN_STATUS_SUCCESS)
    {
        if (MCAN_setBitTime(&bitTiming) != MCAN_STATUS_SUCCESS)
        {
            status = CAN_STATUS_ERROR;
        }
    }

    return status;
}

/*
 *  ======== CANCC27XX_init ========
 */
static int_fast16_t CANCC27XX_init(const CAN_Config *config,
                                   const CAN_MsgRamConfig *msgRamConfig,
                                   const CAN_BitRateTimingRaw *bitTiming,
                                   uint32_t tsPrescaler)
{
    const CAN_HWAttrs *hwAttrs     = config->hwAttrs;
    CAN_Object *object             = config->object;
    int_fast16_t status            = CAN_STATUS_SUCCESS;
    MCAN_ConfigParams configParams = {0U};
    MCAN_InitParams initParams     = {0U};
    MCAN_RxNewDataStatus newDataStatus;
    const CAN_MsgRamConfig *tempMsgRamConfig;

    /* MCAN SW init mode should be set already but explicitly set it again */
    MCAN_setOpMode(MCAN_OPERATION_MODE_SW_INIT);

    /* Clear all CAN subsystem interrupt enables */
    CANSSDisableInt(CANSS_INT_EXT_TIMESTAMP_OVERFLOW);

    /* Clear all pending CAN subsystem interrupts */
    CANSSClearInt(CANSS_INT_EXT_TIMESTAMP_OVERFLOW);

    /* Clear interrupt masks */
    CANClearIntMask(CAN_INT_LINE0, CAN_INT_MASK);
    CANClearIntMask(CAN_INT_LINE1, CAN_INT_MASK);

    /* Clear all interrupts */
    CANClearInt(CAN_INT_LINE0, CAN_INT_MASK);
    CANClearInt(CAN_INT_LINE1, CAN_INT_MASK);

    /* Set CAN functional clock divider */
    CANSSSetClockDivider(CANCC27XX_CANSS_CLK_DIVIDER);

    /* Set external timestamp counter prescaler */
    CANSSSetExtTimestampPrescaler(tsPrescaler);

    /* Enable external timestamp counter */
    CANSSSetControl(CANSS_CTRL_EXT_TIMESTAMP_EN);

    /* Set CAN IRQ interrupt mask for MCAN Interrupt line 0 events */
    CANSetIntMask(CAN_INT_LINE0, CAN_INT_INTL0);

    /* Set FD mode if supported */
    if ((CANSSGetStatus() & CANSS_STATUS_ENABLE_FDOE) != 0U)
    {
        initParams.fdMode = hwAttrs->enableCANFD ? 1U : 0U;
    }

    /* Set bit rate switching */
    initParams.brsEnable = hwAttrs->enableBRS ? 1U : 0U;

    if (MCAN_init(&initParams) != MCAN_STATUS_SUCCESS)
    {
        status = CAN_STATUS_ERROR;
    }

    if (status == CAN_STATUS_SUCCESS)
    {
        /* CAN FD requires using "external" timestamp value. "External" means
         * external to MCAN IP.
         */
        configParams.tsSelect = MCAN_TSCC_COUNTER_EXTERNAL;

        /* Reject remote frames since they are rarely used and do not even exist
         * in the CAN FD format.
         */
        configParams.filterConfig.rrfs = 1U;
        configParams.filterConfig.rrfe = 1U;

        if (hwAttrs->rejectNonMatchingMsgs)
        {
            /* Reject incoming messages that do not match a filter, the default
             * is to accept them into Rx FIFO0.
             */
            configParams.filterConfig.anfs = MCAN_GFC_NM_REJECT;
            configParams.filterConfig.anfe = MCAN_GFC_NM_REJECT;
        }

        if (MCAN_config(&configParams) != MCAN_STATUS_SUCCESS)
        {
            status = CAN_STATUS_ERROR;
        }
    }

    if (status == CAN_STATUS_SUCCESS)
    {
        /* Translate the provided CAN event mask to the MCAN interrupt mask */
        object->intMask = CANMCAN_getInterruptMask(object->eventMask);

        /* Always enable transmit complete IRQ if there is a Tx FIFO/Queue
         * and the Tx ring buffer size is non-zero.
         */
        if ((object->txFifoQNum != 0U) && (hwAttrs->txRingBufSize != 0U))
        {
            object->intMask |= (uint32_t)MCAN_INT_SRC_TRANS_COMPLETE;
        }

        MCAN_setIntLineSel(object->intMask, MCAN_INT_LINE_NUM_0);
        MCAN_enableInt(object->intMask);
        MCAN_enableIntLine(MCAN_INT_LINE_NUM_0);

        if ((object->intMask & MCAN_INT_SRC_TRANS_COMPLETE) != 0U)
        {
            /* Enable transmission interrupt for all buffers */
            MCAN_enableTxBufTransInt(0xFFFFFFFFU);
        }

        if (bitTiming != NULL)
        {
            status = CANMCAN_setBitTimingRaw(bitTiming);
        }
        else
        {
            status = CANCC27XX_setBitRate(config);
        }
    }

    if (status == CAN_STATUS_SUCCESS)
    {
        if (msgRamConfig == NULL)
        {
            /* If msg RAM config is NULL use default */
            tempMsgRamConfig = &CANCC27XX_defaultMsgRamConfig;
        }
        else
        {
            tempMsgRamConfig = msgRamConfig;
        }

        /* Copy config attributes needed for run-time */
        object->txBufNum       = tempMsgRamConfig->txBufNum;
        object->txFifoQNum     = tempMsgRamConfig->txFifoQNum;
        object->txEventFifoNum = tempMsgRamConfig->txEventFifoNum;
        object->rxBufNum       = tempMsgRamConfig->rxBufNum;
        object->rxFifoNum[0]   = tempMsgRamConfig->rxFifoNum[0];
        object->rxFifoNum[1]   = tempMsgRamConfig->rxFifoNum[1];

        /* Setup message RAM sections and filters */
        status = CANMCAN_configMsgRam(tempMsgRamConfig, CANCC27XX_MRAM_SIZE, hwAttrs->enableCANFD);
    }

    if (status == CAN_STATUS_SUCCESS)
    {
        MCAN_clearIntStatus(object->intMask);
        newDataStatus.statusLow  = 0xFFFFFFFFU;
        newDataStatus.statusHigh = 0xFFFFFFFFU;
        MCAN_clearNewDataStatus(&newDataStatus);
    }

    return status;
}

/*
 *  ======== CAN_initDevice ========
 */
int_fast16_t CAN_initDevice(uint_least8_t index, CAN_Params *params)
{
    const CAN_Config *config   = &CAN_config[index];
    const CAN_HWAttrs *hwAttrs = config->hwAttrs;
    HwiP_Handle hwiHandle      = NULL;
    HwiP_Params hwiParams;
    int_fast16_t status;

    /* Try to start and take ownership of AFOSC. Only a single driver is
     * permitted to have ownership of AFOSC at a time.
     */
    if (PowerLPF3_startAFOSC(PowerLPF3_AFOSC_FREQ_80MHZ) != Power_SOK)
    {
        /* Failed to start AFOSC */
        return CAN_STATUS_ERROR;
    }

    /* Set CAN functional clock to use AFOSC (80 MHz) */
    HWREG(CKMD_BASE + CKMD_O_CANCLKSEL) |= CKMD_CANCLKSEL_SRC_CLKAF;

    /* Power up and enable host clock for MCAN peripheral */
    Power_setDependency(PowerLPF3_PERIPH_MCAN);

    /* Wait for memory initialization to complete */
    while ((CANSSGetStatus() & CANSS_STATUS_MEM_INIT_DONE) == 0U) {}

    status = CANCC27XX_init(config, params->msgRamConfig, params->bitTiming, params->tsPrescaler);

    if (status == CAN_STATUS_SUCCESS)
    {
        /* Setup HWI handler */
        HwiP_Params_init(&hwiParams);
        hwiParams.arg      = (uintptr_t)config;
        hwiParams.priority = hwAttrs->intPriority;
        hwiHandle          = HwiP_construct(&canHwi, INT_CAN_IRQ, CANCC27XX_hwiFxn, &hwiParams);
    }

    if (hwiHandle != NULL)
    {
        /* Configure IOs */
        GPIO_setConfigAndMux(hwAttrs->rxPin, GPIO_CFG_IN_PU, hwAttrs->rxPinMux);
        GPIO_setConfigAndMux(hwAttrs->txPin, GPIO_CFG_IN_PU, hwAttrs->txPinMux);

        MCAN_setOpMode(MCAN_OPERATION_MODE_NORMAL);

        /* Disallow standby since CAN messages could be received from the external
         * CAN transceiver at any time.
         */
        Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);
    }
    else
    {
        PowerLPF3_stopAFOSC();

        /* Power down and disable clock for MCAN peripheral */
        Power_releaseDependency(PowerLPF3_PERIPH_MCAN);

        status = CAN_STATUS_ERROR;
    }

    return status;
}

/*
 *  ======== CAN_close ========
 */
void CAN_close(CAN_Handle handle)
{
    CAN_Object *object         = handle->object;
    const CAN_HWAttrs *hwAttrs = handle->hwAttrs;

    HwiP_destruct(&canHwi);

    GPIO_resetConfig(hwAttrs->rxPin);
    GPIO_resetConfig(hwAttrs->txPin);

    PowerLPF3_stopAFOSC();

    /* Power down and disable clock for MCAN peripheral */
    Power_releaseDependency(PowerLPF3_PERIPH_MCAN);

    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

    object->isOpen = false;
}

/*
 *  ======== CANCC27XX_enableLoopback ========
 */
static void CANCC27XX_enableLoopback(bool externalModeEnable)
{
    MCAN_setOpMode(MCAN_OPERATION_MODE_SW_INIT);

    if (externalModeEnable)
    {
        MCAN_enableLoopbackMode(MCAN_LPBK_MODE_EXTERNAL);
    }
    else
    {
        MCAN_enableLoopbackMode(MCAN_LPBK_MODE_INTERNAL);
    }

    MCAN_setOpMode(MCAN_OPERATION_MODE_NORMAL);
}

/*
 *  ======== CAN_enableLoopbackExt ========
 */
int_fast16_t CAN_enableLoopbackExt(CAN_Handle handle)
{
    (void)handle; /* unused arg */

    CANCC27XX_enableLoopback(true);

    return CAN_STATUS_SUCCESS;
}

/*
 *  ======== CAN_enableLoopbackInt ========
 */
int_fast16_t CAN_enableLoopbackInt(CAN_Handle handle)
{
    (void)handle; /* unused arg */

    CANCC27XX_enableLoopback(false);

    return CAN_STATUS_SUCCESS;
}

/*
 *  ======== CAN_disableLoopback ========
 */
int_fast16_t CAN_disableLoopback(CAN_Handle handle)
{
    (void)handle; /* unused arg */

    MCAN_setOpMode(MCAN_OPERATION_MODE_SW_INIT);

    MCAN_disableLoopbackMode();

    MCAN_setOpMode(MCAN_OPERATION_MODE_NORMAL);

    return CAN_STATUS_SUCCESS;
}

/*
 *  ======== CAN_getBitTiming ========
 */
void CAN_getBitTiming(CAN_Handle handle, CAN_BitTimingParams *bitTiming, uint32_t *clkFreq)
{
    (void)handle; /* unused arg */

    MCAN_getBitTime(bitTiming);

    /* Shift the oscillator frequency by the divider macro value to calculate
     * the CAN functional clock frequency.
     */
    *clkFreq = AFOSC_FREQ_KHZ >> CANCC27XX_CANSS_CLK_DIVIDER;
}
