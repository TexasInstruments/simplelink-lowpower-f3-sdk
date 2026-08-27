/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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
 *  ======== CANCC27XXX10.c ========
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ti/drivers/can/CANCC27XX.h>
#include <ti/drivers/CAN.h>
#include <ti/drivers/cryptoutils/sharedresources/CommonResourceXXF3.h>
#include <ti/drivers/dma/UDMALPF3.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/TaskP.h>

#include <third_party/mcan/MCAN.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/can.h)
#include DeviceFamily_constructPath(driverlib/udma.h)
#include DeviceFamily_constructPath(inc/hw_canfd.h)

#define CANCC27XX_REG_WRITE_DMA_CHANNEL_NUM   10U
#define CANCC27XX_WRITE_REG_UDMA_CHANNEL_MASK ((uint32_t)1UL << CANCC27XX_REG_WRITE_DMA_CHANNEL_NUM)

/* DMA Control Table Entries */
ALLOCATE_CONTROL_TABLE_ENTRY(dmaChannel10ControlTableEntry, CANCC27XX_REG_WRITE_DMA_CHANNEL_NUM);

static volatile uDMAControlTableEntry *CANCC27XX_regWriteDmaTableEntry = &dmaChannel10ControlTableEntry;

/* Variable to hold register value to be written by uDMA */
static uint32_t CANCC27XX_regVal;

/* Task object for CAN IRQ handling */
static TaskP_Struct CANCC27XX_task;

/* Semaphore to block CAN IRQ handling task */
static SemaphoreP_Struct CANCC27XX_irqSemaphore;

/* Semaphore to ensure exclusive access to the Tx buffer */
static SemaphoreP_Struct CANCC27XX_writeSemaphore;

/* Array of critical MCAN registers that require special handling due to errata
 * SYS_211.
 */
static uint32_t CANCC27XX_mcanCriticalRegs[] = {
    /* Most frequently accessed registers are listed first */
    MCAN_TXBAR,
    MCAN_TXEFA,
    MCAN_RXF0A,
    MCAN_RXF1A,
    MCAN_NDAT1,
    MCAN_NDAT2,
    MCAN_CCCR,
    MCAN_TXBCR,
};

/* Forward declarations */
static bool CANCC27XX_isCriticalMcanReg(uint32_t offset);
static void CANCC27XX_writeRegDma(uint32_t addr, uint32_t value);
static void CANCC27XX_taskFxn(void *arg);

/* Externs */
extern void CANCC27XX_irqHandler(void *arg);

/*
 *  ======== CANCC27XX_isCriticalReg ========
 *  Returns true if the provided offset matches a critical MCAN register.
 */
static bool CANCC27XX_isCriticalMcanReg(uint32_t offset)
{
    bool found = false;
    size_t i;
    size_t numRegs = sizeof(CANCC27XX_mcanCriticalRegs) / sizeof(CANCC27XX_mcanCriticalRegs[0]);

    for (i = 0; i < numRegs; i++)
    {
        if (CANCC27XX_mcanCriticalRegs[i] == offset)
        {
            found = true;
            break;
        }
    }

    return found;
}

/*
 *  ======== CANCC27XX_writeRegDma ========
 *  This function writes a CAN register using the uDMA after ensuring other bus
 *  masters are inactive and all interrupts are disabled. This function cannot
 *  be called from a context where blocking is not allowed.
 */
static void CANCC27XX_writeRegDma(uint32_t addr, uint32_t value)
{
    uint32_t dmaDoneMask;
    uint32_t primask;

    /* Due to errata SYS_211, acquire lock to arbitrate access to the HSM, CAN,
     * and APU.
     */
    CommonResourceXXF3_acquireLock(SemaphoreP_WAIT_FOREVER);

    /* Store the value in SRAM */
    CANCC27XX_regVal = value;

    /* Store current PRIMASK and set PRIMASK=1 to disable interrupts.
     * HwiP_disable() is not used here since it may write BASEPRI which leaves
     * high-priority interrupts enabled.
     */
    primask = __get_PRIMASK();
    __set_PRIMASK(1);

    /* Configure the CAN reg write DMA control structure for a single 32-bit
     * (word-aligned) transaction in auto-request mode with no address
     * incrementing.
     */
    CANCC27XX_regWriteDmaTableEntry->control = (UDMA_SIZE_32 | UDMA_SRC_INC_NONE | UDMA_DST_INC_NONE | UDMA_ARB_1 |
                                                UDMA_MODE_AUTO);

    /* Set DMA channel to high priority to minimize the critical section duration.
     * It is assumed no other drivers in the system are using this DMA channel and
     * thus we do not need to save and restore the previous priority.
     */
    uDMASetChannelPriority(CANCC27XX_WRITE_REG_UDMA_CHANNEL_MASK);

    /* Configure the DMA transfer source and destination end addresses. The end
     * addresses are the same as the start addresses since address incrementing
     * is disabled in the control struct.
     */
    CANCC27XX_regWriteDmaTableEntry->pSrcEndAddr = (void *)(&CANCC27XX_regVal);
    CANCC27XX_regWriteDmaTableEntry->pDstEndAddr = (void *)(addr);

    /* Make sure all DMA interrupts are disabled except for the channel being
     * used. Save the mask to restore it when the transaction is done.
     */
    dmaDoneMask                      = HWREG(DMA_BASE + DMA_O_DONEMASK);
    HWREG(DMA_BASE + DMA_O_DONEMASK) = CANCC27XX_WRITE_REG_UDMA_CHANNEL_MASK;

    /* Clear potential DMA channel request done for the channel being used */
    uDMAClearInt(CANCC27XX_WRITE_REG_UDMA_CHANNEL_MASK);

    /* Read any DMA register to ensure the DMA channel request done clear above
     * has completed before clearing the DMA interrupt.
     */
    HWREG(DMA_BASE + DMA_O_DONEMASK);

    /* Clear potential pending DMA interrupt */
    HwiP_clearInterrupt(INT_DMA_DONE_COMB);

    /* Enable the DMA channel; it will be disabled automatically by the uDMA
     * controller when the transfer is complete.
     */
    uDMAEnableChannel(CANCC27XX_WRITE_REG_UDMA_CHANNEL_MASK);

    /* Start the uDMA transfer. Due to errata SYS_211, there cannot be any
     * system bus accesses from this point until the transfer is done.
     */
    uDMARequestChannel(CANCC27XX_WRITE_REG_UDMA_CHANNEL_MASK);

    /* Wait until the uDMA transaction is finished by polling the NVIC interrupt
     * set pending register. This code is written in assembly using only CPU
     * registers to prevent accessing system bus while the DMA transaction is
     * ongoing. The assembly is equivalent to:
     *     while (IntGetPend(INT_DMA_DONE_COMB) == false) {};
     */
    __asm volatile("1:\n\t"                    /* Numerical local label */
                   "ldr r0, =0xE000E200\n\t"   /* Load NVIC ISPR (Interrupt Set-Pending Register) address */
                   "ldr r0, [r0]\n\t"          /* Read NVIC ISPR */
                   "ands.w r0, r0, #0x100\n\t" /* Mask NVIC (1 << (24-16 = 8) -> 0x100) */
                   "beq 1b\n\t"                /* Branch back to label if result is 0 */
                   :                           /* No output operands */
                   :                           /* No input operands */
                   : "r0", "cc", "memory");    /* Clobber list: r0, condition codes, memory */

    /* Clear the DMA channel request done */
    uDMAClearInt(CANCC27XX_WRITE_REG_UDMA_CHANNEL_MASK);

    /* Read any DMA register to ensure the DMA channel request done clear above
     * has completed before clearing the DMA interrupt.
     */
    HWREG(DMA_BASE + DMA_O_DONEMASK);

    /* Clear the pending DMA interrupt */
    HwiP_clearInterrupt(INT_DMA_DONE_COMB);

    /* Read the pending interrupt status to ensure the write above has taken
     * effect before re-enabling interrupts.
     */
    (void)IntGetPend(INT_DMA_DONE_COMB);

    /* Restore DMA interrupt mask */
    HWREG(DMA_BASE + DMA_O_DONEMASK) = dmaDoneMask;

    /* Restore PRIMASK */
    __set_PRIMASK(primask);

    CommonResourceXXF3_releaseLock();
}

/*
 *  ======== MCAN_writeReg ========
 *  Write a value to a MCAN register or message RAM location.
 */
void MCAN_writeReg(uint32_t offset, uint32_t value)
{
    uint32_t addr;

    /* Determine if the offset is for an MCAN register or message RAM */
    if (offset < CANFD_SRAM_BASE)
    {
        /* MCAN register: add the offset to CAN-FD peripheral base address */
        addr = CANFD_BASE + offset;

        if (CANCC27XX_isCriticalMcanReg(offset))
        {
            /* Due to errata SYS_211, use DMA to write the register */
            CANCC27XX_writeRegDma(addr, value);
        }
        else
        {
            HWREG(addr) = value;
        }
    }
    else
    {
        /* MCAN message RAM location: directly use the offset as the address */
        addr = offset;

        HWREG(addr) = value;
    }
}

/*
 *  ======== CANSSSetEndOfInt ========
 */
void CANSSSetEndOfInt(uint32_t eoi)
{
    CANCC27XX_writeRegDma(CANFD_BASE + CANFD_O_MCANSS_EOI, eoi);
}

/*
 *  ======== CANClearInt ========
 */
void CANClearInt(uint8_t lineNum, uint32_t flags)
{
    uint32_t offset = (lineNum == CAN_INT_LINE0) ? CANFD_O_ICLR0 : CANFD_O_ICLR1;

    CANCC27XX_writeRegDma(CANFD_BASE + offset, flags);
}

/*
 *  ======== CANSSSetClkStopCtrl ========
 */
void CANSSSetClkStopCtrl(uint32_t flags)
{
    CANCC27XX_writeRegDma(CANFD_BASE + CANFD_O_MCANSS_CLKCTL, flags);
}

/*
 *  ======== CANSSClearClkStopCtrl ========
 */
void CANSSClearClkStopCtrl(uint32_t flags)
{
    CANCC27XX_writeRegDma(CANFD_BASE + CANFD_O_MCANSS_CLKCTL, (uint32_t)~flags);
}

/*
 *  ======== CANCC27XX_hwiFxn ========
 */
void CANCC27XX_hwiFxn(uintptr_t arg)
{
    (void)arg; /* unused arg */

    /* Due to errata SYS_211, IRQ handling must be deferred to a task to avoid
     * system deadlock, missed interrupts, and priority inversion.
     */

    /* Disable the CAN interrupt until the task can process it */
    IntDisable(INT_CAN_IRQ);

    /* Unblock task to handle the interrupt */
    SemaphoreP_post(&CANCC27XX_irqSemaphore);
}

/*
 *  ======== CANCC27XX_taskFxn ========
 */
static void CANCC27XX_taskFxn(void *arg)
{
    while (1)
    {
        /* Wait for CANCC27XX_irqHandler to post semaphore. No need to check
         * return value when waiting forever.
         */
        (void)SemaphoreP_pend(&CANCC27XX_irqSemaphore, SemaphoreP_WAIT_FOREVER);

        CANCC27XX_irqHandler(arg);

        /* Re-enable the CAN interrupt */
        IntEnable(INT_CAN_IRQ);
    }
}

/*
 *  ======== CAN_write ========
 */
int_fast16_t CAN_write(CAN_Handle handle, const MCAN_TxBufElement *elem)
{
    CAN_Object *object             = handle->object;
    int_fast16_t status            = CAN_STATUS_ERROR;
    MCAN_TxFifoQStatus fifoQStatus = {0};

    if (object->txFifoQNum != 0U)
    {
        /* Ensure exclusive access to the Tx buffer until the Tx request is added.
         * No need to check return value when waiting forever.
         */
        (void)SemaphoreP_pend(&CANCC27XX_writeSemaphore, SemaphoreP_WAIT_FOREVER);

        MCAN_getTxFifoQStatus(&fifoQStatus);

        if (fifoQStatus.fifoFull == 0U)
        {
            MCAN_writeTxMsg(fifoQStatus.putIdx, elem);

            MCAN_setTxBufAddReq(fifoQStatus.putIdx);
            status = CAN_STATUS_SUCCESS;
        }
        else
        {
            if (StructRingBuf_put(&object->txStructRingBuf, elem) < 0)
            {
                status = CAN_STATUS_TX_BUF_FULL;
            }
            else
            {
                status = CAN_STATUS_SUCCESS;
            }
        }

        SemaphoreP_post(&CANCC27XX_writeSemaphore);
    }

    return status;
}

/*
 *  ======== CAN_writeBuffer ========
 */
int_fast16_t CAN_writeBuffer(CAN_Handle handle, uint32_t bufIdx, const MCAN_TxBufElement *elem)
{
    CAN_Object *object  = handle->object;
    int_fast16_t status = CAN_STATUS_ERROR;
    uint32_t pendingTx;

    if (bufIdx < object->txBufNum)
    {
        /* Ensure exclusive access to the Tx buffer until the Tx request is added.
         * No need to check return value when waiting forever.
         */
        (void)SemaphoreP_pend(&CANCC27XX_writeSemaphore, SemaphoreP_WAIT_FOREVER);

        pendingTx = MCAN_getTxBufReqPend();

        if ((((uint32_t)1U << bufIdx) & pendingTx) == 0U)
        {
            MCAN_writeTxMsg(bufIdx, elem);

            MCAN_setTxBufAddReq(bufIdx);

            status = CAN_STATUS_SUCCESS;
        }

        SemaphoreP_post(&CANCC27XX_writeSemaphore);
    }

    return status;
}

/*
 *  ======== CANCC27XXXX_init ========
 */
int_fast16_t CANCC27XXXX_init(const CAN_Config *config)
{
    int_fast16_t status = CAN_STATUS_SUCCESS;
    TaskP_Params taskParams;

    /* Initialize CommonResource access semaphore, needed due to errata SYS_211 */
    CommonResourceXXF3_constructRTOSObjects();

    /* Create binary semaphore for IRQ handling */
    if (SemaphoreP_constructBinary(&CANCC27XX_irqSemaphore, 0) == NULL)
    {
        status = CAN_STATUS_ERROR;
    }

    if (status == CAN_STATUS_SUCCESS)
    {
        /* Create binary semaphore for CAN_write() */
        if (SemaphoreP_constructBinary(&CANCC27XX_writeSemaphore, 1) == NULL)
        {
            SemaphoreP_destruct(&CANCC27XX_irqSemaphore);
            status = CAN_STATUS_ERROR;
        }
    }

    if (status == CAN_STATUS_SUCCESS)
    {
        /* Initialize task params */
        TaskP_Params_init(&taskParams);
        taskParams.name      = "CANCC27XX";
        taskParams.priority  = CANCC27XXX10_config.taskPri;
        taskParams.stack     = CANCC27XXX10_config.taskStack;
        taskParams.stackSize = CANCC27XXX10_config.taskStackSize;
        taskParams.arg       = (void *)config;

        /* Construct a task for handling CANCC27XX interrupts */
        if (TaskP_construct(&CANCC27XX_task, CANCC27XX_taskFxn, &taskParams) == NULL)
        {
            SemaphoreP_destruct(&CANCC27XX_irqSemaphore);
            SemaphoreP_destruct(&CANCC27XX_writeSemaphore);
            status = CAN_STATUS_ERROR;
        }
    }

    if (status == CAN_STATUS_SUCCESS)
    {
        /* Set a power dependency on DMA */
        Power_setDependency(PowerLPF3_PERIPH_DMA);
    }

    return status;
}

/*
 *  ======== CANCC27XXXX_close ========
 */
void CANCC27XXXX_close(CAN_Handle handle)
{
    (void)handle; /* unused arg */

    /* Destroy the IRQ handling task */
    TaskP_destruct(&CANCC27XX_task);

    /* Destroy the IRQ handling semaphore */
    SemaphoreP_destruct(&CANCC27XX_irqSemaphore);

    /* Destroy the write semaphore */
    SemaphoreP_destruct(&CANCC27XX_writeSemaphore);

    /* Release the DMA power dependency */
    Power_releaseDependency(PowerLPF3_PERIPH_DMA);
}
