/*
 * Copyright (c) 2024-2025, Texas Instruments Incorporated
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
 *  ======== APULPF3.c ========
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <math.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/dma/UDMALPF3.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/apu/APULPF3.h>
#include <ti/drivers/apu/APULPF3_FW.h>
#include <ti/drivers/cryptoutils/sharedresources/CommonResourceXXF3.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_apu.h)
#include DeviceFamily_constructPath(driverlib/apu.h)
#include DeviceFamily_constructPath(driverlib/interrupt.h)
#include DeviceFamily_constructPath(driverlib/udma.h)
#include DeviceFamily_constructPath(cmsis/core/cmsis_compiler.h)

/* Generic functions for getting the highest set bit in a value. Internal use
 * only. CLZ is an ARM instruction for `count leading zeroes`.
 */
#if defined(__IAR_SYSTEMS_ICC__)
    #include <intrinsics.h>
    #define APU_GET_HIGHEST_SET_BIT(value) (31 - __CLZ(value))
#elif defined(__TI_COMPILER_VERSION__)
    #include <arm_acle.h>
    #define APU_GET_HIGHEST_SET_BIT(value) (31 - __clz(value))
#elif defined(__GNUC__) && !defined(__TI_COMPILER_VERSION__)
    #include <arm_acle.h>
    #define APU_GET_HIGHEST_SET_BIT(value) (31 - __builtin_clz(value))
#endif

/* Forward declarations */
static void APULPF3_loadFW();
static bool APULPF3_inAPU(void *ptr);
static void APULPF3_copyBack();
static void APULPF3_hwiIntFxn(uintptr_t arg);
static void APULPF3_initHw(void);
static int APULPF3_postNotifyFxn(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg);
/* The below functions are copies of Driverlib functions, but they account for
 * errata SYS_211. The Driverlib functions are deprecated and shall not be used.
 */
static void APULPF3_jacobiEVDDma(uint16_t N,
                                 void *pInput,
                                 void *pResultV,
                                 uint16_t maxIter,
                                 float minSum,
                                 float epsTol);
static void APULPF3_gaussJordanElimDma(uint16_t M, uint16_t N, void *pInput, float epsTol);
static void APULPF3_vectorMaxMinDma(uint16_t N, void *pInput, float thresh, uint16_t op, void *pResult);

/*
 *  APU object structure
 */
typedef struct
{
    HwiP_Struct hwi;

    float complex *result; /* Temporary result pointer, in APU memory */

    float complex *argA; /* Argument pointer, in APU memory */

    float complex *argB; /* Argument pointer, in APU memory */

    float complex *resultBuffer; /* Final result pointer, may be in APU memory*/

    bool scratchpad; /* Whether or not to copy to/from APU memory */

    uint16_t resultSize; /* APU result size */

    bool isInitialized; /* Has open() been called */

    Power_NotifyObj postNotify; /* For restoring register state after standby */

} APULPF3_Object;

static APULPF3_Object object = {0};
static SemaphoreP_Struct apuSem;
static SemaphoreP_Struct apuAccessSem;
extern void *apuHwAttrs;

/* To avoid starving other processes the maximum transfer size is set to 256.
 * This value must be the same as the arbitration size set in
 * hwAttrs->dmaTableEntry.control.
 */
#define APU_UDMA_MAX_TRANSFER_BLOCK_SIZE 256
/* SRAM cache to copy data stored in flash. */
__attribute__((aligned(4))) static float dmaCache[APU_UDMA_MAX_TRANSFER_BLOCK_SIZE];

static void APULPF3_hwiIntFxn(uintptr_t arg)
{
    uint32_t status;

    /* Get and clear the interrupt. */
    status                       = HWREG(APU_BASE + APU_O_MIS);
    /* Clear all interrupts even if only one is used, to avoid unintended side
     * effects
     */
    HWREG(APU_BASE + APU_O_ICLR) = 0x0000000F;

    /* APU API has finished. */
    if (status & (APU_IMASK_API))
    {
        SemaphoreP_post(&apuSem);
    }
}

/*
 *  ======== APULPF3_loadFW ========
 * Used to copy topsm firmware into APU program memory, if needed.
 */
static void APULPF3_loadFW()
{
    for (uint16_t i = 0; i < APULPF3_FW_SIZE; i++)
    {
        *((uint32_t *)APU_TOPSMRAM_BASE + i) = lrf_fw_image_seq_apu[i];
    }
}

/*
 *  ======== APULPF3_inAPU ========
 * Checks if an address is in APU memory.
 */
static bool APULPF3_inAPU(void *ptr)
{
    return APURAM_DATA0_BASE <= (uint32_t)ptr && (uint32_t)ptr < APURAM_DATA0_BASE + APURAM_DATA0_SIZE;
}

/*
 *  ======== APULPF3_copyBack ========
 * If not in scratchpad mode, copy data back from APU memory to a result buffer.
 */
static void APULPF3_copyBack()
{
    float *memPtr;
    float *result;

    if (!object.scratchpad)
    {
        memPtr = (float *)object.result;
        result = (float *)object.resultBuffer;

        /* Multiply resultSize by 2 because resultSize represents number of
         * complex floats (64 bits) but we transfer them in blocks of 32 bits.
         */
        APULPF3_dataMemTransfer(memPtr, result, object.resultSize * 2);
    }
}

/*
 *  ======== APULPF3_initHw ========
 * Initialize the APU hardware registers
 */
static void APULPF3_initHw(void)
{
    /* Configure APU and wait for the configuration to complete. */
    APUSetConfig(APU_LSECTL_MEMORY_MIRRORED);
    APUWaitOnIrq();

    HWREG(APU_BASE + APU_O_IMASK) = APU_IMASK_API;
}

/*
 *  ======== APULPF3_postNotifyFxn ========
 *  Called by Power module when waking up from standby
 */
static int APULPF3_postNotifyFxn(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg)
{
    (void)eventArg;
    (void)clientArg;
    /* Reconfigure the hardware if returning from sleep */
    if (eventType == PowerLPF3_AWAKE_STANDBY)
    {
        /* Restore the SVT (nonretained) portions of the peripheral
         * The APU FW is stored in retained memory (ULL) so it doesn't need to
         * be reloaded after standby.
         */
        APULPF3_initHw();
    }

    return Power_NOTIFYDONE;
}

/*
 *  ======== APULPF3_dataMemTransfer ========
 * This function transfers data between the APU data memory and a selected
 * source or destination using the uDMA while all interrupts are disabled. To
 * avoid starving other processes, the transfer is broken down into blocks
 * of APU_UDMA_MAX_TRANSFER_BLOCK_SIZE words.
 */
void APULPF3_dataMemTransfer(const float *src, float *dst, size_t length)
{
    uintptr_t key;
    size_t wordsLeftToTransfer = length;
    size_t transferSize        = APU_UDMA_MAX_TRANSFER_BLOCK_SIZE;
    uint32_t arbitrationSize;
    /* Use a temp pointer to prevent overwriting the source pointer as a
     * consequence of the operations below involving data coming from flash.
     */
    float *srcTemp;

    APULPF3_HWAttrs *hwAttrs = (APULPF3_HWAttrs *)apuHwAttrs;

    /* Transfer all data, reducing the block size until all data has been
     * transferred.
     */
    while (wordsLeftToTransfer > 0)
    {
        if (wordsLeftToTransfer >= transferSize)
        {
            /* Select the DMA arbitration size. transferSize is expected to be
             * a power of 2 that maps into the sequential UDMA arbitration size
             * macros.
             */
            arbitrationSize = APU_GET_HIGHEST_SET_BIT(transferSize) << UDMA_ARB_S;

            /* Compute how many complete blocks of the selected size we can
             * send.
             */
            size_t blocksToTransfer = (wordsLeftToTransfer / transferSize);

            /* Transfer as many blocks of the selected size as possible */
            for (size_t transfer = 0; transfer < blocksToTransfer; transfer++)
            {
                /* Update source pointer. */
                size_t wordsTransferred = (length - wordsLeftToTransfer);
                srcTemp                 = (float *)src + wordsTransferred;

                /* Due to errata SYS_211, copy FLASH or APU data to SRAM before
                 * starting a DMA transfer.
                 *
                 * Check for both S and NS ranges.
                 */
                if ((((uint32_t)srcTemp >= FLASH_MAIN_NS_BASE) &&
                     ((uint32_t)srcTemp < (FLASH_MAIN_NS_BASE + FLASH_MAIN_SIZE))) ||
                    ((uint32_t)srcTemp < (FLASH_MAIN_S_BASE + FLASH_MAIN_SIZE)))
                {
                    memcpy(dmaCache, srcTemp, sizeof(float) * transferSize);
                    srcTemp = (float *)dmaCache;
                }
                /* Due to errata SYS_211, when both source and destination are
                 * in the APU, we need to move the data out before moving it
                 * back in to the new location. This recursion should only ever
                 * go one call deep, since the destination is in SRAM.
                 */
                else if (APULPF3_inAPU((void *)srcTemp) && APULPF3_inAPU((void *)(dst + wordsTransferred)))
                {
                    APULPF3_dataMemTransfer(srcTemp, dmaCache, transferSize);
                    srcTemp = (float *)dmaCache;
                }

                /* Due to errata SYS_211, get HSM lock to avoid AHB bus master
                 * transactions. For now, there is no protection against I2S, so
                 * I2S must not be used at the same time as the APU.
                 */
                CommonResourceXXF3_acquireLock(SemaphoreP_WAIT_FOREVER);

                /* Due to errata SYS_211, disable all interrupts with PRIMASK to
                 * ensure no one else uses the DMA.
                 */
                key = __get_PRIMASK();
                __set_PRIMASK(1);

                /* Configure the uDMA transfer. */
                hwAttrs->dmaTableEntry->control = (UDMA_SIZE_32 | UDMA_SRC_INC_32 | UDMA_DST_INC_32 | arbitrationSize |
                                                   UDMA_MODE_AUTO);

                uDMASetChannelTransfer(hwAttrs->dmaTableEntry,
                                       UDMA_MODE_AUTO,
                                       (void *)(srcTemp),
                                       (void *)(dst + wordsTransferred),
                                       transferSize);

                /* Set maximum priority to re-enable interrupts as soon as
                 * possible.
                 */
                uDMASetChannelPriority(hwAttrs->dmaChannelMask);

                uDMAEnableChannel(hwAttrs->dmaChannelMask);

                /* Make sure all DMA interrupts are disabled except for the
                 * channel being used. Save the mask to restore it when the
                 * transaction is done.
                 */
                uint32_t dmaDoneMask             = HWREG(DMA_BASE + DMA_O_DONEMASK);
                HWREG(DMA_BASE + DMA_O_DONEMASK) = hwAttrs->dmaChannelMask;
                /* Clear potentially pending interrupt from this channel. */
                uDMAClearInt(hwAttrs->dmaChannelMask);
                /* Read any DMA register to ensure the write above has taken
                 * effect, before clearing the interrupt.
                 */
                HWREG(DMA_BASE + DMA_O_DONEMASK);
                HwiP_clearInterrupt(INT_DMA_DONE_COMB);

                /* Start the uDMA transfer. Due to errata SYS_211, there cannot
                 * be any bus accesses from this point until the transfer is
                 * done.
                 */
                uDMARequestChannel(hwAttrs->dmaChannelMask);

                /* Wait until the uDMA transaction is finished by polling at the
                 * NVIC pending register. This is written in assembly to only
                 * use CPU registers and avoid using the SRAM while the
                 * transaction is ongoing. The assembly is equivalent to:
                 *
                 * while (IntGetPend(INT_DMA_DONE_COMB) == false) {};
                 *
                 */
                __asm volatile("1:\n\t"                    /* Local label */
                               "ldr r0, =0xE000E200\n\t"   /* Load NVIC address */
                               "ldr r0, [r0]\n\t"          /* Read NVIC */
                               "ands.w r0, r0, #0x100\n\t" /* Mask NVIC (1 << (24-16 = 8) -> 0x100) */
                               "beq 1b\n\t"                /* Branch back if 0 */
                               :                           /* No output operands */
                               :                           /* No input operands */
                               : "r0", "cc", "memory");    /* Clobber list: r0, condition codes, memory */

                /* Disable the uDMA. */
                uDMADisableChannel(hwAttrs->dmaChannelMask);
                uDMADisableSwEventInt(hwAttrs->dmaChannelMask);
                uDMAClearInt(hwAttrs->dmaChannelMask);
                /* Read any DMA register to ensure the write above has taken
                 * effect, before clearing the interrupt.
                 */
                HWREG(DMA_BASE + DMA_O_DONEMASK);
                HwiP_clearInterrupt(INT_DMA_DONE_COMB);
                /* Read the pending status to ensure the write above has taken
                 * effect before re-enabling interrupts.
                 */
                (void)IntGetPend(INT_DMA_DONE_COMB);

                /* Re-enable interrupts. */
                __set_PRIMASK(key);

                CommonResourceXXF3_releaseLock();

                /* Restore interrupt mask for other uDMA channels. */
                HWREG(DMA_BASE + DMA_O_DONEMASK) = dmaDoneMask;

                wordsLeftToTransfer -= transferSize;
            }
        }
        else
        {
            /* Try the next smallest possible transfer block size. */
            transferSize = transferSize >> 1;
        }
    }
}

/*
 *  ======== APULPF3_init ========
 */
void APULPF3_init(void)
{
    HwiP_Params hwiParams;
    uintptr_t key;

    key = HwiP_disable();

    if (object.isInitialized)
    {
        HwiP_restore(key);
        return;
    }
    else
    {

        object.isInitialized = true;
        HwiP_restore(key);

        APULPF3_HWAttrs *hwAttrs = (APULPF3_HWAttrs *)apuHwAttrs;

        /* Clock the APU Peripheral. */
        Power_setDependency(PowerLPF3_PERIPH_APU);

        /* Load firmware into TOPSM memory. */
        APULPF3_loadFW();

        /* Set up APU peripheral */
        APULPF3_initHw();

        /* Due to errata SYS_211, DMA is used to read and write data between
         * SRAM/FLASH and the APU data memory.
         */
        Power_setDependency(PowerLPF3_PERIPH_DMA);
        UDMALPF3_init();

        /*
         * One semaphore to control when an API call has finished, and one to
         * control access into APU functions.
         */
        SemaphoreP_constructBinary(&(apuSem), 0);
        SemaphoreP_constructBinary(&(apuAccessSem), 1);

        /* Need to restore APU registers in SVT-domain after standby */
        Power_registerNotify(&(object.postNotify), PowerLPF3_AWAKE_STANDBY, APULPF3_postNotifyFxn, (uintptr_t)NULL);

        /* Create Hwi object for this APU peripheral. */
        HwiP_Params_init(&hwiParams);
        hwiParams.priority = hwAttrs->intPriority;
        HwiP_construct(&(object.hwi), INT_APU_IRQ, APULPF3_hwiIntFxn, &hwiParams);

        /* Initialize the CommonResource access semaphore, needed due to errata SYS_211. */
        CommonResourceXXF3_constructRTOSObjects();

        object.isInitialized = true;
    }
}

/*
 *  ======== APULPF3_startOperationSequence ========
 */
void APULPF3_startOperationSequence()
{
    SemaphoreP_pend(&apuAccessSem, SemaphoreP_WAIT_FOREVER);
    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*
 *  ======== APULPF3_startOperationSequence ========
 */
void APULPF3_stopOperationSequence()
{
    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
    SemaphoreP_post(&apuAccessSem);
}
/* Vector-vector functions */

/*
 *  ======== APULPF3_dotProduct ========
 */
int_fast16_t APULPF3_dotProduct(APULPF3_ComplexVector *vecA,
                                APULPF3_ComplexVector *vecB,
                                bool conjugate,
                                float complex *result)
{
    uint16_t inputSize;
    uint16_t returnVal = APULPF3_STATUS_ERROR;

    if (vecA->size == vecB->size)
    {
        object.scratchpad = false;
        if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(vecB->data) && APULPF3_inAPU(result))
        {
            /*
             * If params are in APU memory, use them as is
             * and do not copy result back to CPU memory.
             */
            object.scratchpad = true;
        }

        inputSize = APULPF3_prepareVectors(vecA, vecB);
        APULPF3_prepareResult(1, inputSize, result);

        /* Tell the APU to do a dot product with the arguments we have provided. */
        if (conjugate)
        {
            APUVectorDotConj(vecA->size, object.argA, object.argB, object.result);
        }
        else
        {
            APUVectorDot(vecA->size, object.argA, object.argB, object.result);
        }

        SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

        APULPF3_copyBack();
        returnVal = APULPF3_STATUS_SUCCESS;
    }

    return returnVal;
}

/*
 *  ======== APULPF3_vectorMult ========
 */
int_fast16_t APULPF3_vectorMult(APULPF3_ComplexVector *vecA,
                                APULPF3_ComplexVector *vecB,
                                bool conjugate,
                                APULPF3_ComplexVector *result)
{
    uint16_t inputSize;
    int_fast16_t returnVal = APULPF3_STATUS_ERROR;

    if (vecA->size == vecB->size)
    {
        object.scratchpad = false;
        if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(vecB->data) && APULPF3_inAPU(result->data))
        {
            /*
             * If params are in APU memory, use them as is
             * and do not copy result back to CPU memory.
             */
            object.scratchpad = true;
        }
        inputSize = APULPF3_prepareVectors(vecA, vecB);

        APULPF3_prepareResult(vecA->size, inputSize, result->data);

        if (conjugate)
        {
            APUVectorMultConj(vecA->size, object.argA, object.argB, object.result);
        }
        else
        {
            APUVectorMult(vecA->size, object.argA, object.argB, object.result);
        }

        SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

        APULPF3_copyBack();
        returnVal = APULPF3_STATUS_SUCCESS;
    }

    return returnVal;
}

/*
 *  ======== APULPF3_vectorSum ========
 */
int_fast16_t APULPF3_vectorSum(APULPF3_ComplexVector *vecA,
                               APULPF3_ComplexVector *vecB,
                               bool subtraction,
                               APULPF3_ComplexVector *result)
{
    uint16_t inputSize;
    int_fast16_t returnVal = APULPF3_STATUS_ERROR;

    if (vecA->size == vecB->size)
    {
        object.scratchpad = false;
        if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(vecB->data) && APULPF3_inAPU(result->data))
        {
            /*
             * If params are in APU memory, use them as is
             * and do not copy result back to CPU memory.
             */
            object.scratchpad = true;
        }
        inputSize = APULPF3_prepareVectors(vecA, vecB);

        APULPF3_prepareResult(vecA->size, inputSize, result->data);

        APUVectorSum(vecA->size, object.argA, object.argB, subtraction, object.result);

        SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

        APULPF3_copyBack();

        returnVal = APULPF3_STATUS_SUCCESS;
    }

    return returnVal;
}

/*
 *  ======== APULPF3_vectorScalarSum ========
 */
int_fast16_t APULPF3_vectorScalarSum(APULPF3_ComplexVector *vecA,
                                     float complex *scalar,
                                     bool subtraction,
                                     APULPF3_ComplexVector *result)
{
    APULPF3_ComplexVector vecB;
    uint16_t inputSize;
    uint16_t op;

    object.scratchpad = false;
    if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(scalar) && APULPF3_inAPU(result->data))
    {
        /*
         * If params are in APU memory, use them as is
         * and do not copy result back to CPU memory.
         */
        object.scratchpad = true;
    }

    vecB.data = scalar;
    vecB.size = 1;

    inputSize = APULPF3_prepareVectors(vecA, &vecB);

    APULPF3_prepareResult(vecA->size, inputSize, result->data);
    op = APU_OP_ADD;
    if (subtraction)
    {
        op = APU_OP_SUB;
    }

    APUVectorScalarSum(vecA->size, object.argA, object.argB, op, object.result);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

/*
 *  ======== APULPF3_vectorScalarMult ========
 */
int_fast16_t APULPF3_vectorScalarMult(APULPF3_ComplexVector *vecA, float complex *scalar, APULPF3_ComplexVector *result)
{
    APULPF3_ComplexVector vecB;
    uint16_t inputSize;

    object.scratchpad = false;
    if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(scalar) && APULPF3_inAPU(result->data))
    {
        /*
         * If params are in APU memory, use them as is
         * and do not copy result back to CPU memory.
         */
        object.scratchpad = true;
    }

    vecB.data = scalar;
    vecB.size = 1;
    inputSize = APULPF3_prepareVectors(vecA, &vecB);

    APULPF3_prepareResult(vecA->size, inputSize, result->data);

    APUVectorScalarMult(vecA->size, object.argA, object.argB, object.result);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

/*
 *  ======== APULPF3_vectorR2C ========
 */
int_fast16_t APULPF3_vectorR2C(APULPF3_ComplexVector *vecA,
                               APULPF3_ComplexVector *vecB,
                               APULPF3_R2COp
                               operator,
                               APULPF3_ComplexVector * result)
{
    uint16_t inputSize;

    object.scratchpad = false;
    if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(vecB->data) && APULPF3_inAPU(result->data))
    {
        /*
         * If params are in APU memory, use them as is
         * and do not copy result back to CPU memory.
         */
        object.scratchpad = true;
    }

    inputSize = APULPF3_prepareVectors(vecA, vecB);

    APULPF3_prepareResult(vecA->size, inputSize, result->data);

    APUVectorR2C(vecA->size, object.argA, object.argB, operator, object.result);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

/*
 *  ======== APULPF3_vectorMaxMin ========
 */
int_fast16_t APULPF3_vectorMaxMin(APULPF3_ComplexVector *vec,
                                  float scalarThreshold,
                                  bool min,
                                  APULPF3_ComplexVector *result)
{
    uint16_t inputSize;
    uint16_t op;

    object.scratchpad = false;
    if (APULPF3_inAPU(vec->data) && APULPF3_inAPU(result->data))
    {
        /*
         * If params are in APU memory, use them as is
         * and do not copy result back to CPU memory.
         */
        object.scratchpad = true;
    }

    inputSize = APULPF3_prepareVectors(vec, vec);

    APULPF3_prepareResult(vec->size, inputSize, result->data);

    op = APU_OP_MAX;
    if (min)
    {
        op = APU_OP_MIN;
    }

    APULPF3_vectorMaxMinDma(vec->size, object.argA, scalarThreshold, op, object.result);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

/*
 *  ======== APULPF3_cartesianToPolarVector ========
 */
int_fast16_t APULPF3_cartesianToPolarVector(APULPF3_ComplexVector *vec, APULPF3_ComplexVector *result)
{
    uint16_t inputSize;

    object.scratchpad = false;
    if (APULPF3_inAPU(vec->data) && APULPF3_inAPU(result->data))
    {
        /*
         * If params are in APU memory, use them as is
         * and do not copy result back to CPU memory.
         */
        object.scratchpad = true;
    }
    inputSize = APULPF3_prepareVectors(vec, vec);

    APULPF3_prepareResult(vec->size, inputSize, result->data);

    APUVectorCart2Pol(vec->size, object.argA, object.result);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

/*
 *  ======== APULPF3_polarToCartesianVector ========
 */
int_fast16_t APULPF3_polarToCartesianVector(APULPF3_ComplexVector *vec,
                                            float complex *temp,
                                            APULPF3_ComplexVector *result)
{
    uint16_t inputSize;

    object.scratchpad = false;
    if (APULPF3_inAPU(vec->data) && APULPF3_inAPU(result->data) && APULPF3_inAPU(temp))
    {
        /*
         * If params are in APU memory, use them as is
         * and do not copy result back to CPU memory.
         */
        object.scratchpad = true;
    }

    inputSize = APULPF3_prepareVectors(vec, vec);

    APULPF3_prepareResult(vec->size, inputSize, result->data);

    if (!object.scratchpad)
    {
        /* Assign temp vector after input and result */
        temp = object.result + 2 * vec->size;
    }

    APUVectorPol2Cart(vec->size, object.argA, object.result, temp);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

/* Vector algorithms */

/*
 *  ======== APULPF3_sortVector ========
 */
int_fast16_t APULPF3_sortVector(APULPF3_ComplexVector *vec, APULPF3_ComplexVector *result)
{
    object.scratchpad = false;
    if (APULPF3_inAPU(vec->data) && APULPF3_inAPU(result->data))
    {
        /*
         * If params are in APU memory, use them as is
         * and do not copy result back to CPU memory.
         */
        object.scratchpad = true;
    }

    APULPF3_prepareVectors(vec, vec);

    APULPF3_prepareResult(vec->size, APULPF3_RESULT_INPLACE, result->data);

    APUVectorSort(vec->size, object.argA);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

/*
 *  ======== APULPF3_covMatrixSpatialSmoothing ========
 */
int_fast16_t APULPF3_covMatrixSpatialSmoothing(APULPF3_ComplexVector *vec,
                                               uint16_t covMatrixSize,
                                               bool fbAveraging,
                                               APULPF3_ComplexTriangleMatrix *result)
{
    uint16_t inputSize;

    object.scratchpad = false;
    if (APULPF3_inAPU(vec->data) && APULPF3_inAPU(result->data))
    {
        /*
         * If params are in APU memory, use them as is
         * and do not copy result back to CPU memory.
         */
        object.scratchpad = true;
    }

    inputSize = APULPF3_prepareVectors(vec, vec);

    APULPF3_prepareResult(covMatrixSize * covMatrixSize, inputSize, result->data);

    APUSpSmoothCovMatrix(vec->size, object.argA, covMatrixSize, object.result, fbAveraging);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

/*
 *  ======== APULPF3_computeFFT ========
 */
int_fast16_t APULPF3_computeFFT(APULPF3_ComplexVector *vec, bool inverse, APULPF3_ComplexVector *result)
{
    bool isInAPU;
    int_fast16_t returnVal = APULPF3_STATUS_ERROR;

    /* Check if size is a power of 2. */
    if ((vec->size & (vec->size - 1)) == 0)
    {
        /* FFT/ IFFT is a two-part operation. First the APU is configured,
         * which produces no result. Then the actual computation happens,
         * which may or may not use scratchpad mode.
         */
        object.scratchpad = false;
        isInAPU           = false;
        if (APULPF3_inAPU(vec->data) && APULPF3_inAPU(result->data))
        {
            isInAPU = true;
            APUConfigFft(vec->size, vec->data);
        }
        else
        {
            APUConfigFft(vec->size, (void *)APURAM_DATA0_BASE);
        }

        SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

        object.scratchpad = isInAPU;

        APULPF3_prepareVectors(vec, vec);

        APULPF3_prepareResult(vec->size, APULPF3_RESULT_INPLACE, result->data);

        if (inverse)
        {
            APUComputeIfft(vec->size, object.argA);
        }
        else
        {
            APUComputeFft(vec->size, object.argA);
        }

        SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

        APULPF3_copyBack();

        returnVal = APULPF3_STATUS_SUCCESS;
    }

    return returnVal;
}

/* Matrix functions */

/*
 *  ======== APULPF3_matrixMult ========
 */
int_fast16_t APULPF3_matrixMult(APULPF3_ComplexMatrix *matA, APULPF3_ComplexMatrix *matB, APULPF3_ComplexMatrix *result)
{
    uint16_t inputSize;
    int_fast16_t returnVal = APULPF3_STATUS_ERROR;

    if (matA->cols == matB->rows)
    {
        object.scratchpad = false;
        if (APULPF3_inAPU(matA->data) && APULPF3_inAPU(matB->data) && APULPF3_inAPU(result->data))
        {
            /*
             * If params are in APU memory, use them as is
             * and do not copy result back to CPU memory.
             */
            object.scratchpad = true;
        }

        inputSize = APULPF3_prepareMatrices(matA, matB);
        APULPF3_prepareResult(matA->cols * matB->rows, inputSize, result->data);

        APUMatrixMult(matA->rows, matA->cols, matB->cols, object.argA, object.argB, object.result);

        SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

        APULPF3_copyBack();

        returnVal = APULPF3_STATUS_SUCCESS;
    }

    return returnVal;
}

/*
 *  ======== APULPF3_matrixSum ========
 */
int_fast16_t APULPF3_matrixSum(APULPF3_ComplexMatrix *matA, APULPF3_ComplexMatrix *matB, APULPF3_ComplexMatrix *result)
{
    uint16_t inputSize;
    int_fast16_t returnVal = APULPF3_STATUS_ERROR;

    if (matA->rows == matB->rows && matA->cols == matB->cols)
    {
        object.scratchpad = false;
        if (APULPF3_inAPU(matA->data) && APULPF3_inAPU(matB->data) && APULPF3_inAPU(result->data))
        {
            /*
             * If params are in APU memory, use them as is
             * and do not copy result back to CPU memory.
             */
            object.scratchpad = true;
        }

        inputSize = APULPF3_prepareMatrices(matA, matB);

        APULPF3_prepareResult(matA->rows * matA->cols, inputSize, result->data);

        APUMatrixSum(matA->rows, matA->cols, object.argA, object.argB, object.result);

        SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

        APULPF3_copyBack();

        returnVal = APULPF3_STATUS_SUCCESS;
    }

    return returnVal;
}
/*
 *  ======== APULPF3_HermLo ========
 */
int_fast16_t APULPF3_HermLo(APULPF3_ComplexTriangleMatrix *mat, APULPF3_ComplexTriangleMatrix *result)
{
    uint16_t inputSize;
    uint16_t length;
    uint_fast16_t returnVal = APULPF3_STATUS_ERROR;

    if (mat->size == result->size)
    {
        object.scratchpad = false;
        if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(result->data))
        {
            /*
             * If params are in APU memory, use them as is
             * and do not copy result back to CPU memory.
             */
            object.scratchpad = true;
        }
        length                       = (mat->size * (mat->size + 1)) >> 1;
        APULPF3_ComplexVector vecIn  = {.data = mat->data, .size = length};
        APULPF3_ComplexVector vecOut = {.data = result->data, .size = length};
        inputSize                    = APULPF3_prepareVectors(&vecIn, &vecIn);

        APULPF3_prepareResult(vecOut.size, inputSize, vecOut.data);

        APUHermLo(mat->size, object.argA, object.result);

        SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

        APULPF3_copyBack();

        returnVal = APULPF3_STATUS_SUCCESS;
    }

    return returnVal;
}

/*
 *  ======== APULPF3_matrixScalarSum ========
 */
int_fast16_t APULPF3_matrixScalarSum(APULPF3_ComplexMatrix *mat, float complex *scalar, APULPF3_ComplexMatrix *result)
{
    float complex *tempArgB;
    void *argB;
    uint16_t inputSize;

    object.scratchpad = false;
    if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(scalar) && APULPF3_inAPU(result->data))
    {
        /*
         * If params are in APU memory, use them as is
         * and do not copy result back to CPU memory.
         */
        object.scratchpad = true;
        tempArgB          = scalar;
    }
    else
    {
        argB     = APULPF3_loadArgMirrored(1, mat->rows * mat->cols, scalar);
        tempArgB = argB;
    }

    /* This function overwrites argB so we have saved it somewhere else. */
    inputSize = APULPF3_prepareMatrices(mat, mat);

    object.argB = tempArgB;

    APULPF3_prepareResult(mat->rows * mat->cols, 1 + inputSize, result->data);

    APUMatrixScalarSum(mat->rows, mat->cols, object.argA, object.argB, object.result);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

/*
 *  ======== APULPF3_matrixScalarMult ========
 */
int_fast16_t APULPF3_matrixScalarMult(APULPF3_ComplexMatrix *mat, float complex *scalar, APULPF3_ComplexMatrix *result)
{
    float complex *tempArgB;
    void *argB;
    uint16_t inputSize;

    object.scratchpad = false;
    if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(scalar) && APULPF3_inAPU(result->data))
    {
        /*
         * If params are in APU memory, use them as is
         * and do not copy result back to CPU memory.
         */
        object.scratchpad = true;
        tempArgB          = scalar;
    }
    else
    {
        argB     = APULPF3_loadArgMirrored(1, mat->rows * mat->cols, scalar);
        tempArgB = argB;
    }

    inputSize = APULPF3_prepareMatrices(mat, mat);

    object.argB = tempArgB;

    APULPF3_prepareResult(mat->rows * mat->cols, 1 + inputSize, result->data);

    APUMatrixScalarMult(mat->rows, mat->cols, object.argA, object.argB, object.result);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

/*
 *  ======== APULPF3_matrixNorm ========
 */
int_fast16_t APULPF3_matrixNorm(APULPF3_ComplexMatrix *mat, float complex *result)
{
    uint16_t inputSize;

    object.scratchpad = false;
    if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(result))
    {
        /*
         * If params are in APU memory, use them as is
         * and do not copy result back to CPU memory.
         */
        object.scratchpad = true;
    }

    inputSize = APULPF3_prepareMatrices(mat, mat);

    APULPF3_prepareResult(1, inputSize, result);

    APUMatrixNorm(mat->rows, mat->cols, object.argA, object.result);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    /* The APU actually returns the square of the norm. */
    *result = sqrtf(*result);

    return APULPF3_STATUS_SUCCESS;
}

/* Matrix algorithms */

/*
 *  ======== APULPF3_jacobiEVD ========
 */
int_fast16_t APULPF3_jacobiEVD(APULPF3_ComplexTriangleMatrix *mat,
                               uint16_t maxIter,
                               float stopThreshold,
                               float epsTol,
                               APULPF3_ComplexVector *result)
{
    APULPF3_ComplexMatrix EVDmat;
    uint16_t resultSize;
    float complex *eigVecs;

    object.scratchpad = false;
    if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(result->data))
    {
        /*
         * If params are in APU memory, use them as is
         * and do not copy result back to CPU memory.
         */
        object.scratchpad = true;
    }

    EVDmat.data = mat->data;
    EVDmat.rows = mat->size;
    EVDmat.cols = mat->size;
    APULPF3_prepareMatrices(&EVDmat, &EVDmat);

    /* Produces both the upper triangular part of a NxN in-place matrix and an NxN matrix. */
    resultSize = (mat->size * mat->size) + ((mat->size * mat->size + mat->size) / 2);
    APULPF3_prepareResult(resultSize, APULPF3_RESULT_INPLACE, result->data);

    if (!object.scratchpad)
    {
        /* object.result will point to eigenvalues, which is followed by the eigenvectors. */
        eigVecs      = object.result + ((mat->size * mat->size + mat->size) / 2);
        result->size = resultSize;

        APULPF3_jacobiEVDDma(mat->size, object.argA, eigVecs, maxIter, stopThreshold, epsTol);
    }
    else
    {
        /* object.result will point to just the eigenvectors. Eigenvalues have overwritten argA. */
        APULPF3_jacobiEVDDma(mat->size, object.argA, object.result, maxIter, stopThreshold, epsTol);
        result->size = (mat->size * mat->size);
    }

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

/*
 *  ======== APULPF3_gaussJordanElim ========
 */
int_fast16_t APULPF3_gaussJordanElim(APULPF3_ComplexMatrix *mat, float zeroThreshold, APULPF3_ComplexMatrix *result)
{
    object.scratchpad = false;
    if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(result->data))
    {
        /*
         * If params are in APU memory, use them as is
         * and do not copy result back to CPU memory.
         */
        object.scratchpad = true;
    }

    APULPF3_prepareMatrices(mat, mat);

    APULPF3_prepareResult(mat->cols * mat->rows, APULPF3_RESULT_INPLACE, result->data);

    APULPF3_gaussJordanElimDma(mat->rows, mat->cols, object.argA, zeroThreshold);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

/*
 *  ======== APULPF3_unitCircle ========
 */
int_fast16_t APULPF3_unitCircle(uint16_t numPoints,
                                uint16_t constant,
                                uint16_t phase,
                                bool conjugate,
                                APULPF3_ComplexVector *result)
{
    object.scratchpad = false;
    if (APULPF3_inAPU(result->data))
    {
        /*
         * If params are in APU memory, use them as is
         * and do not copy result back to CPU memory.
         */
        object.scratchpad = true;
    }
    APULPF3_prepareResult(numPoints, APULPF3_RESULT_INPLACE, result->data);

    APUUnitCircle(numPoints, constant, phase, conjugate, object.result);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

/* Utility functions */
/*
 *  ======== APULPF3_prepareVectors ========
 */
uint16_t APULPF3_prepareVectors(APULPF3_ComplexVector *vecA, APULPF3_ComplexVector *vecB)
{
    complex float *argA = vecA->data;
    complex float *argB = vecB->data;
    uint16_t inputSize  = 0;
    if (!object.scratchpad)
    {
        argA = APULPF3_loadArgMirrored(vecA->size, 0, vecA->data);
        /* Are the vectors the same? */
        if ((vecA->data == vecB->data) && (vecA->size == vecB->size))
        {
            argB      = argA;
            inputSize = vecA->size;
        }
        else
        {
            /* If they aren't the same put argB right after argA. */
            argB      = APULPF3_loadArgMirrored(vecB->size, vecA->size, vecB->data);
            inputSize = vecA->size + vecB->size;
        }
    }

    object.argA = argA;
    object.argB = argB;

    return inputSize;
}

/*
 *  ======== APULPF3_prepareMatrices ========
 */
uint16_t APULPF3_prepareMatrices(APULPF3_ComplexMatrix *matA, APULPF3_ComplexMatrix *matB)
{
    complex float *argA = matA->data;
    complex float *argB = matB->data;
    uint16_t inputSize  = 0;

    uint16_t sizeA = matA->cols * matA->rows;
    uint16_t sizeB = matB->cols * matB->rows;

    if (!object.scratchpad)
    {
        argA = APULPF3_loadArgMirrored(sizeA, 0, matA->data);
        /* Are the matrices the same? */
        if ((matA->data == matB->data) && (matA->rows == matB->rows) && (matA->cols == matB->cols))
        {
            argB      = argA;
            inputSize = sizeA;
        }
        else
        {
            /* If they aren't the same put argB right after argA. */
            argB      = APULPF3_loadArgMirrored(sizeB, sizeA, matB->data);
            inputSize = sizeA + sizeB;
        }
    }

    object.argA = argA;
    object.argB = argB;

    return inputSize;
}

/*
 *  ======== APULPF3_loadArgMirrored ========
 */
void *APULPF3_loadArgMirrored(uint16_t argSize, uint16_t offset, float complex *src)
{
    float *memPtr = (float *)(APURAM_DATA0_BASE + offset * 8);
    float *source = (float *)src;

    /* Multiply argSize by 2 because argSize represents number of complex floats
     * (64 bits) but we transfer them in blocks of 32 bits.
     */
    APULPF3_dataMemTransfer(source, memPtr, argSize * 2);

    void *arg = (void *)(APURAM_DATA0_BASE + offset * 8);

    return arg;
}

/*
 *  ======== APULPF3_loadTriangular ========
 */
void *APULPF3_loadTriangular(APULPF3_ComplexMatrix *mat, uint16_t offset)
{
    APULPF3_ComplexMatrix matrix = *mat;
    float *memPtr                = (float *)(APURAM_DATA0_BASE + offset * 8);
    float *source                = (float *)matrix.data;
    uint16_t elemCount           = 0;
    uint16_t shift               = 0;

    for (uint32_t i = 1; i < matrix.cols + 1; i++)
    {

        for (uint32_t j = 0; j < i; j++)
        {
            APULPF3_dataMemTransfer(source + (shift + 2 * j), memPtr + (elemCount), 2);
            elemCount += 2;
        }

        shift += 2 * matrix.cols;
    }

    void *arg = (void *)(APURAM_DATA0_BASE + offset * 8);

    return arg;
}

/*
 *  ======== APULPF3_prepareResult ========
 */
void APULPF3_prepareResult(uint16_t resultSize, uint16_t offset, complex float *resultBuffer)
{
    object.resultBuffer = resultBuffer;
    object.resultSize   = resultSize;
    if (!object.scratchpad)
    {
        /* If not in scratchpad mode, place at a known location in APU memory. */
        object.result = (void *)(APURAM_DATA0_BASE + 8 * offset);
    }
    else
    {
        /* Otherwise result goes where the user has placed it in APU memory. */
        object.result = resultBuffer;
    }
}

static void APULPF3_jacobiEVDDma(uint16_t N, void *pInput, void *pResultV, uint16_t maxIter, float minSum, float epsTol)
{
    /* Value to store, composed of Real and Imaginary part */
    float value[2];

    /* Matrix size N of input Hermitian matrix */
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    /* Maximum number of iterations */
    HWREG(APU_BASE + APU_O_CMDPAR1) = maxIter;
    /* Input matrix A offset in memory */
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);
    /* V result offset in memory */
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResultV);

    /* Always save min_sum at: heap_base + 7 */
    volatile float *mem_ptr = (float *)(APU_GET_DATA_MEM_ABS(APU_HEAP_ADDR + 7));
    value[0]                = minSum;
    value[1]                = 0.0;
    APULPF3_dataMemTransfer(value, (float *)mem_ptr, 2);

    /* Always save epsTol at: heap_base + 30 */
    volatile float *mem_ptr_eps = (float *)(APU_GET_DATA_MEM_ABS(APU_HEAP_ADDR + 30));
    value[0]                    = epsTol;
    value[1]                    = 0.0;
    APULPF3_dataMemTransfer(value, (float *)mem_ptr_eps, 2);

    /* Start APU */
    HWREG(APU_BASE + APU_O_API)    = APU_API_EIGEN;
    HWREG(APU_BASE + APU_O_MSGBOX) = APU_MSGBOX_VAL_ALLONES;
}

static void APULPF3_gaussJordanElimDma(uint16_t M, uint16_t N, void *pInput, float epsTol)
{
    /* Number of rows of input/output matrices */
    HWREG(APU_BASE + APU_O_CMDPAR0) = M;
    /* Number of columns of input/output matrices */
    HWREG(APU_BASE + APU_O_CMDPAR1) = N;
    /* Input matrix A offset in memory */
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);

    /* Threshold epsilon at heap addres + 1 */
    volatile float *mem_ptr;
    mem_ptr        = (float *)(APU_GET_DATA_MEM_ABS((APU_HEAP_ADDR + 1)));
    /* Value to store, composed of Real and Imaginary part */
    float value[2] = {epsTol, 0.0};
    APULPF3_dataMemTransfer(value, (float *)mem_ptr, 2);

    /* Start APU */
    HWREG(APU_BASE + APU_O_API)    = APU_API_GAUSS;
    HWREG(APU_BASE + APU_O_MSGBOX) = APU_MSGBOX_VAL_ALLONES;
}

static void APULPF3_vectorMaxMinDma(uint16_t N, void *pInput, float thresh, uint16_t op, void *pResult)
{
    /* Vector Length */
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    /* Input vector */
    HWREG(APU_BASE + APU_O_CMDPAR1) = APU_GET_DATA_MEM_OFFSET(pInput);

    /* Threshold epsilon at heap addres + 1 */
    volatile float *mem_ptr = (float *)(APU_GET_DATA_MEM_ABS((APU_HEAP_ADDR + 1)));
    /* Value to store, composed of Real and Imaginary part */
    float value[2]          = {thresh, 0.0};
    APULPF3_dataMemTransfer(value, (float *)mem_ptr, 2);

    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(mem_ptr);

    /* op for max (APU_MAXMIN_OP_MAX) or min (APU_MAXMIN_OP_MIN) */
    HWREG(APU_BASE + APU_O_CMDPAR3) = op;
    /* Result */
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    /* Start APU */
    HWREG(APU_BASE + APU_O_API)    = APU_API_MAXMIN;
    HWREG(APU_BASE + APU_O_MSGBOX) = APU_MSGBOX_VAL_ALLONES;
}
