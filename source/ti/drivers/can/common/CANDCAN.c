/*
 * Copyright (c) 2023-2026, Texas Instruments Incorporated
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
 *  ======== CANDCAN.c ========
 */

#include <stdint.h>

#include <ti/drivers/can/common/CANDCAN.h>
#include <ti/drivers/CAN.h>

#include <third_party/dcan/DCAN.h>

#define DCAN_STD_FILTER_ELEM_MAX (128U)
#define DCAN_EXT_FILTER_ELEM_MAX (64U)
#define DCAN_RX_FIFO_ELEM_MAX    (64U)
#define DCAN_RX_BUFFERS_ELEM_MAX (64U)
#define DCAN_TX_BUFFERS_ELEM_MAX (32U)

/*
 *  ======== CANDCAN_setBitTimingRaw ========
 */
int_fast16_t CANDCAN_setBitTimingRaw(const CAN_BitRateTimingRaw *rawTiming)
{
    int_fast16_t status            = CAN_STATUS_SUCCESS;
    DCAN_BitTimingParams bitTiming = {0U};

    bitTiming.nomRatePrescaler  = rawTiming->nbrp;
    bitTiming.nomTimeSeg1       = rawTiming->ntSeg1;
    bitTiming.nomTimeSeg2       = rawTiming->ntSeg2;
    bitTiming.nomSynchJumpWidth = rawTiming->nsjw;

    if (DCAN_setBitTime(&bitTiming) != DCAN_STATUS_SUCCESS)
    {
        status = CAN_STATUS_ERROR;
    }

    return status;
}

/*  ======== CANDCAN_configMsgRam ========
 *   Message RAM sections are configured in this order:
 *     - Standard ID filters
 *     - Extended ID filters
 *     - RX FIFO0
 *     - RX FIFO1
 *     - RX Buffers
 *     - TX EventFIFO
 *     - TX Buffers
 *     - TX FIFO (or TX Queue)
 */
int_fast16_t CANDCAN_configMsgRam(const CAN_MsgRamConfig *config, uint32_t msgRamSize)
{
    int_fast16_t status = CAN_STATUS_SUCCESS;
    DCAN_MsgRamConfig msgRamConfig;
    uint_fast8_t i;
    uint32_t addr = 0U;
    uint32_t payloadSize;
    uint32_t msgRamUsage;
    uint32_t totalTxBufCnt;

    totalTxBufCnt = config->txBufNum + config->txFifoQNum;

    /* Validate the configuration */
    if ((DCAN_STD_FILTER_ELEM_MAX < config->stdFilterNum) || (DCAN_EXT_FILTER_ELEM_MAX < config->extFilterNum) ||
        (DCAN_RX_FIFO_ELEM_MAX < config->rxFifoNum[0]) || (DCAN_RX_FIFO_ELEM_MAX < config->rxFifoNum[1]) ||
        (DCAN_RX_BUFFERS_ELEM_MAX < config->rxBufNum) || (DCAN_TX_BUFFERS_ELEM_MAX < totalTxBufCnt))
    {
        return CAN_STATUS_ERROR;
    }

    /* Set common element size for all sections */
    msgRamConfig.rxBufElemSize   = DCAN_ELEM_SIZE_8BYTES;
    msgRamConfig.rxFifo0ElemSize = DCAN_ELEM_SIZE_8BYTES;
    msgRamConfig.rxFifo1ElemSize = DCAN_ELEM_SIZE_8BYTES;
    msgRamConfig.txBufElemSize   = DCAN_ELEM_SIZE_8BYTES;
    payloadSize                  = DCAN_MAX_PAYLOAD_SIZE;

    /* SID filters */
    msgRamConfig.sidFilterStartAddr = 0U;
    msgRamConfig.sidFilterListSize  = config->stdFilterNum;
    addr += DCAN_STD_ID_FILTER_ELEM_SIZE * config->stdFilterNum;

    /* XID filters */
    msgRamConfig.xidFilterStartAddr = addr;
    msgRamConfig.xidFilterListSize  = config->extFilterNum;
    addr += DCAN_EXT_ID_FILTER_ELEM_SIZE * config->extFilterNum;

    /* Rx FIFO 0 */
    msgRamConfig.rxFifo0StartAddr = addr;
    msgRamConfig.rxFifo0Size      = config->rxFifoNum[0];
    msgRamConfig.rxFifo0Watermark = 0U; /* 0 = watermark interrupt disabled */
    msgRamConfig.rxFifo0OpMode    = 0U; /* 0 = blocking mode */
    addr += config->rxFifoNum[0] * (DCAN_TX_RX_ELEMENT_HEADER_SIZE + payloadSize);

    /* Rx FIFO 1 */
    msgRamConfig.rxFifo1StartAddr = addr;
    msgRamConfig.rxFifo1Size      = config->rxFifoNum[1];
    msgRamConfig.rxFifo1Watermark = 0U; /* 0 = watermark interrupt disabled */
    msgRamConfig.rxFifo1OpMode    = 0U; /* 0 = blocking mode */
    addr += config->rxFifoNum[1] * (DCAN_TX_RX_ELEMENT_HEADER_SIZE + payloadSize);

    /* Rx Buffers */
    msgRamConfig.rxBufStartAddr = addr;
    addr += config->rxBufNum * (DCAN_TX_RX_ELEMENT_HEADER_SIZE + payloadSize);

    /* Tx Event FIFO */
    msgRamConfig.txEventFifoStartAddr = addr;
    msgRamConfig.txEventFifoSize      = config->txEventFifoNum;
    msgRamConfig.txEventFifoWatermark = 0U; /* 0 = watermark interrupt disabled */
    addr += config->txEventFifoNum * DCAN_TX_EVENT_ELEM_SIZE;

    /* Tx Buffers */
    msgRamConfig.txBufStartAddr = addr;
    msgRamConfig.txBufNum       = config->txBufNum;

    /* Tx FIFO/Q */
    msgRamConfig.txFifoQSize = config->txFifoQNum;
    msgRamConfig.txFifoQMode = config->txFifoQMode;

    msgRamUsage = addr + ((config->txBufNum + config->txFifoQNum) * (DCAN_TX_RX_ELEMENT_HEADER_SIZE + payloadSize));

    if (msgRamUsage > msgRamSize)
    {
        status = CAN_STATUS_ERROR;
    }
    else
    {
        DCAN_configMsgRam(&msgRamConfig);

        for (i = 0U; i < config->stdFilterNum; i++)
        {
            DCAN_addStdMsgIDFilter(i, &config->stdMsgIDFilterList[i]);
        }

        for (i = 0U; i < config->extFilterNum; i++)
        {
            DCAN_addExtMsgIDFilter(i, &config->extMsgIDFilterList[i]);
        }
    }

    return status;
}

/*
 *  ======== CANDCAN_getInterruptMask ========
 */
uint32_t CANDCAN_getInterruptMask(uint32_t eventMask)
{
    /* Bus Off interrupt source is always enabled to allow for recovery */
    uint32_t intMask = (uint32_t)DCAN_INT_SRC_BUS_OFF_STATUS;

    if ((eventMask & CAN_EVENT_RX_DATA_AVAIL) != 0U)
    {
        intMask |= DCAN_INT_SRC_RX_MASK;
    }

    if ((eventMask & CAN_EVENT_TX_FINISHED) != 0U)
    {
        intMask |= (uint32_t)DCAN_INT_SRC_TRANS_COMPLETE;
    }

    if ((eventMask & CAN_EVENT_TX_EVENT_AVAIL) != 0U)
    {
        intMask |= (uint32_t)DCAN_INT_SRC_TX_EVT_FIFO_NEW_ENTRY;
    }

    if ((eventMask & CAN_EVENT_TX_EVENT_LOST) != 0U)
    {
        intMask |= (uint32_t)DCAN_INT_SRC_TX_EVT_FIFO_ELEM_LOST;
    }

    if ((eventMask & (CAN_EVENT_ERR_PASSIVE | CAN_EVENT_ERR_ACTIVE)) != 0U)
    {
        intMask |= (uint32_t)DCAN_INT_SRC_ERR_PASSIVE;
    }

    if ((eventMask & CAN_EVENT_RX_FIFO_MSG_LOST) != 0U)
    {
        intMask |= (uint32_t)DCAN_INT_SRC_RX_FIFO0_MSG_LOST;
        intMask |= (uint32_t)DCAN_INT_SRC_RX_FIFO1_MSG_LOST;
    }

    if ((eventMask & CAN_EVENT_BIT_ERR_UNCORRECTED) != 0U)
    {
        intMask |= (uint32_t)DCAN_INT_SRC_BIT_ERR_UNCORRECTED;
    }

    return intMask;
}