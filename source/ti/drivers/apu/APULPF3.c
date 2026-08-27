/*
 * Copyright (c) 2024-2026, Texas Instruments Incorporated
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
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/apu/APULPF3.h>
#include <ti/drivers/apu/APULPF3_FW.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_apu.h)
#include DeviceFamily_constructPath(driverlib/apu.h)

/* Forward declarations */
void APULPF3_loadFW();
bool APULPF3_inAPU(void *ptr);
void APULPF3_hwiIntFxn(uintptr_t arg);
void APULPF3_initHw(void);
int APULPF3_postNotifyFxn(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg);

/* Externs */
extern void APULPF3_copyBack();
extern void APULPF3_init(void);
extern void *APULPF3_loadArgMirrored(uint16_t argSize, uint16_t offset, float complex *src);

APULPF3_Object apuObject = {0};
SemaphoreP_Struct apuSem;
SemaphoreP_Struct apuAccessSem;

void APULPF3_hwiIntFxn(uintptr_t arg)
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
void APULPF3_loadFW()
{
    for (size_t i = 0; i < APULPF3_FW_image.size; i++)
    {
        HWREG(APU_TOPSMRAM_BASE + i * sizeof(uint32_t)) = APULPF3_FW_image.data[i];
    }
}

/*
 *  ======== APULPF3_inAPU ========
 * Checks if an address is in APU memory.
 */
bool APULPF3_inAPU(void *ptr)
{
    return APURAM_DATA0_BASE <= (uint32_t)ptr && (uint32_t)ptr < APURAM_DATA0_BASE + APURAM_DATA0_SIZE;
}

/*
 *  ======== APULPF3_initHw ========
 * Initialize the APU hardware registers
 */
void APULPF3_initHw(void)
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
int APULPF3_postNotifyFxn(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg)
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
 *  ======== APULPF3_startOperationSequence ========
 */
void APULPF3_startOperationSequence()
{
    SemaphoreP_pend(&apuAccessSem, SemaphoreP_WAIT_FOREVER);
    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*
 *  ======== APULPF3_stopOperationSequence ========
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
        apuObject.scratchpad = false;
        if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(vecB->data) && APULPF3_inAPU(result))
        {
            /* If params are in APU memory, use them as is and do not copy
             * result back to CPU memory.
             */
            apuObject.scratchpad = true;
        }

        inputSize = APULPF3_prepareVectors(vecA, vecB);
        APULPF3_prepareResult(1, inputSize, result);

        /* Tell the APU to do a dot product with the arguments we have provided. */
        if (conjugate)
        {
            APUVectorDotConj(vecA->size, apuObject.argA, apuObject.argB, apuObject.result);
        }
        else
        {
            APUVectorDot(vecA->size, apuObject.argA, apuObject.argB, apuObject.result);
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
        apuObject.scratchpad = false;
        if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(vecB->data) && APULPF3_inAPU(result->data))
        {
            /* If params are in APU memory, use them as is and do not copy
             * result back to CPU memory.
             */
            apuObject.scratchpad = true;
        }
        inputSize = APULPF3_prepareVectors(vecA, vecB);

        APULPF3_prepareResult(vecA->size, inputSize, result->data);

        if (conjugate)
        {
            APUVectorMultConj(vecA->size, apuObject.argA, apuObject.argB, apuObject.result);
        }
        else
        {
            APUVectorMult(vecA->size, apuObject.argA, apuObject.argB, apuObject.result);
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
        apuObject.scratchpad = false;
        if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(vecB->data) && APULPF3_inAPU(result->data))
        {
            /* If params are in APU memory, use them as is and do not copy
             * result back to CPU memory.
             */
            apuObject.scratchpad = true;
        }
        inputSize = APULPF3_prepareVectors(vecA, vecB);

        APULPF3_prepareResult(vecA->size, inputSize, result->data);

        APUVectorSum(vecA->size, apuObject.argA, apuObject.argB, subtraction, apuObject.result);

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

    apuObject.scratchpad = false;
    if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(scalar) && APULPF3_inAPU(result->data))
    {
        /* If params are in APU memory, use them as is and do not copy result
         * back to CPU memory.
         */
        apuObject.scratchpad = true;
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

    APUVectorScalarSum(vecA->size, apuObject.argA, apuObject.argB, op, apuObject.result);

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

    apuObject.scratchpad = false;
    if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(scalar) && APULPF3_inAPU(result->data))
    {
        /* If params are in APU memory, use them as is and do not copy result
         * back to CPU memory.
         */
        apuObject.scratchpad = true;
    }

    vecB.data = scalar;
    vecB.size = 1;
    inputSize = APULPF3_prepareVectors(vecA, &vecB);

    APULPF3_prepareResult(vecA->size, inputSize, result->data);

    APUVectorScalarMult(vecA->size, apuObject.argA, apuObject.argB, apuObject.result);

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

    apuObject.scratchpad = false;
    if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(vecB->data) && APULPF3_inAPU(result->data))
    {
        /* If params are in APU memory, use them as is and do not copy result
         * back to CPU memory.
         */
        apuObject.scratchpad = true;
    }

    inputSize = APULPF3_prepareVectors(vecA, vecB);

    APULPF3_prepareResult(vecA->size, inputSize, result->data);

    APUVectorR2C(vecA->size, apuObject.argA, apuObject.argB, operator, apuObject.result);

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

    apuObject.scratchpad = false;
    if (APULPF3_inAPU(vec->data) && APULPF3_inAPU(result->data))
    {
        /* If params are in APU memory, use them as is and do not copy result
         * back to CPU memory.
         */
        apuObject.scratchpad = true;
    }
    inputSize = APULPF3_prepareVectors(vec, vec);

    APULPF3_prepareResult(vec->size, inputSize, result->data);

    APUVectorCart2Pol(vec->size, apuObject.argA, apuObject.result);

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

    apuObject.scratchpad = false;
    if (APULPF3_inAPU(vec->data) && APULPF3_inAPU(result->data) && APULPF3_inAPU(temp))
    {
        /* If params are in APU memory, use them as is and do not copy result
         * back to CPU memory.
         */
        apuObject.scratchpad = true;
    }

    inputSize = APULPF3_prepareVectors(vec, vec);

    APULPF3_prepareResult(vec->size, inputSize, result->data);

    if (!apuObject.scratchpad)
    {
        /* Assign temp vector after input and result */
        temp = apuObject.result + 2 * vec->size;
    }

    APUVectorPol2Cart(vec->size, apuObject.argA, apuObject.result, temp);

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
    apuObject.scratchpad = false;
    if (APULPF3_inAPU(vec->data) && APULPF3_inAPU(result->data))
    {
        /* If params are in APU memory, use them as is and do not copy result
         * back to CPU memory.
         */
        apuObject.scratchpad = true;
    }

    APULPF3_prepareVectors(vec, vec);

    APULPF3_prepareResult(vec->size, APULPF3_RESULT_INPLACE, result->data);

    APUVectorSort(vec->size, apuObject.argA);

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

    apuObject.scratchpad = false;
    if (APULPF3_inAPU(vec->data) && APULPF3_inAPU(result->data))
    {
        /* If params are in APU memory, use them as is and do not copy result
         * back to CPU memory.
         */
        apuObject.scratchpad = true;
    }

    inputSize = APULPF3_prepareVectors(vec, vec);

    APULPF3_prepareResult(covMatrixSize * covMatrixSize, inputSize, result->data);

    APUSpSmoothCovMatrix(vec->size, apuObject.argA, covMatrixSize, apuObject.result, fbAveraging);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

/*
 *  ======== APULPF3_computeFFT ========
 */
int_fast16_t APULPF3_computeFFT(APULPF3_ComplexVector *vec, bool inverse, APULPF3_ComplexVector *result)
{
    int_fast16_t returnVal = APULPF3_STATUS_ERROR;

    /* Check if size is a power of 2. */
    if ((vec->size & (vec->size - 1)) == 0)
    {
        /* FFT/IFFT requires two APU operations.
         * - APU is configured for FFT for a APU memory location where input vector will be found.
         * - Then the input vector can be copied into the given APU memory location (must happen after config)
         * - APU computes FFT/IFFT on input vector in-place.
         * - If not using scratchpad mode, the result is copied out.
         */
        apuObject.scratchpad = false;

        /* Check if input vector is in APU memory */
        if (APULPF3_inAPU(vec->data))
        {
            /* Currently not supported. Cannot configure if input is already inside APU memory. */
            returnVal = APULPF3_STATUS_ERROR;
        }
        else
        {
            /* Check if result vector is inside APU memory */
            if (APULPF3_inAPU(result->data))
            {
                /* When result is inside APU mem, do APU config and compute at the location for the result */
                APUConfigFft(vec->size, result->data);
                SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

                /* Transfer input vector into APU memory, at location of result */
                APULPF3_dataMemTransfer((float *)vec->data, (float *)result->data, vec->size * 2);
                apuObject.argA = result->data;

                /* After calculation, skip copying of result */
                apuObject.scratchpad = true;
            }
            else
            {
                /* When result is outside of APU memory, APU config and compute FFT happens in APU memory offset 0 */
                APUConfigFft(vec->size, (void *)APURAM_DATA0_BASE);
                SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

                APULPF3_prepareVectors(vec, vec);
            }

            APULPF3_prepareResult(vec->size, APULPF3_RESULT_INPLACE, result->data);

            if (inverse)
            {
                APUComputeIfft(vec->size, apuObject.argA);
            }
            else
            {
                APUComputeFft(vec->size, apuObject.argA);
            }

            SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

            APULPF3_copyBack();

            returnVal = APULPF3_STATUS_SUCCESS;
        }
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
        apuObject.scratchpad = false;
        if (APULPF3_inAPU(matA->data) && APULPF3_inAPU(matB->data) && APULPF3_inAPU(result->data))
        {
            /* If params are in APU memory, use them as is and do not copy
             * result back to CPU memory.
             */
            apuObject.scratchpad = true;
        }

        inputSize = APULPF3_prepareMatrices(matA, matB);
        APULPF3_prepareResult(matA->rows * matB->cols, inputSize, result->data);

        APUMatrixMult(matA->rows, matA->cols, matB->cols, apuObject.argA, apuObject.argB, apuObject.result);

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
        apuObject.scratchpad = false;
        if (APULPF3_inAPU(matA->data) && APULPF3_inAPU(matB->data) && APULPF3_inAPU(result->data))
        {
            /* If params are in APU memory, use them as is and do not copy
             * result back to CPU memory.
             */
            apuObject.scratchpad = true;
        }

        inputSize = APULPF3_prepareMatrices(matA, matB);

        APULPF3_prepareResult(matA->rows * matA->cols, inputSize, result->data);

        APUMatrixSum(matA->rows, matA->cols, apuObject.argA, apuObject.argB, apuObject.result);

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
        apuObject.scratchpad = false;
        if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(result->data))
        {
            /* If params are in APU memory, use them as is and do not copy
             * result back to CPU memory.
             */
            apuObject.scratchpad = true;
        }
        length                       = (mat->size * (mat->size + 1)) >> 1;
        APULPF3_ComplexVector vecIn  = {.data = mat->data, .size = length};
        APULPF3_ComplexVector vecOut = {.data = result->data, .size = length};
        inputSize                    = APULPF3_prepareVectors(&vecIn, &vecIn);

        APULPF3_prepareResult(vecOut.size, inputSize, vecOut.data);

        APUHermLo(mat->size, apuObject.argA, apuObject.result);

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

    apuObject.scratchpad = false;
    if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(scalar) && APULPF3_inAPU(result->data))
    {
        /* If params are in APU memory, use them as is and do not copy result
         * back to CPU memory.
         */
        apuObject.scratchpad = true;
        tempArgB             = scalar;
    }
    else
    {
        argB     = APULPF3_loadArgMirrored(1, mat->rows * mat->cols, scalar);
        tempArgB = argB;
    }

    /* This function overwrites argB so we have saved it somewhere else. */
    inputSize = APULPF3_prepareMatrices(mat, mat);

    apuObject.argB = tempArgB;

    APULPF3_prepareResult(mat->rows * mat->cols, 1 + inputSize, result->data);

    APUMatrixScalarSum(mat->rows, mat->cols, apuObject.argA, apuObject.argB, apuObject.result);

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

    apuObject.scratchpad = false;
    if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(scalar) && APULPF3_inAPU(result->data))
    {
        /* If params are in APU memory, use them as is and do not copy result
         * back to CPU memory.
         */
        apuObject.scratchpad = true;
        tempArgB             = scalar;
    }
    else
    {
        argB     = APULPF3_loadArgMirrored(1, mat->rows * mat->cols, scalar);
        tempArgB = argB;
    }

    inputSize = APULPF3_prepareMatrices(mat, mat);

    apuObject.argB = tempArgB;

    APULPF3_prepareResult(mat->rows * mat->cols, 1 + inputSize, result->data);

    APUMatrixScalarMult(mat->rows, mat->cols, apuObject.argA, apuObject.argB, apuObject.result);

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

    apuObject.scratchpad = false;
    if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(result))
    {
        /* If params are in APU memory, use them as is and do not copy result
         * back to CPU memory.
         */
        apuObject.scratchpad = true;
    }

    inputSize = APULPF3_prepareMatrices(mat, mat);

    APULPF3_prepareResult(1, inputSize, result);

    APUMatrixNorm(mat->rows, mat->cols, apuObject.argA, apuObject.result);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    /* The APU actually returns the square of the norm. */
    *result = sqrtf(*result);

    return APULPF3_STATUS_SUCCESS;
}

/* Matrix algorithms */

/*
 *  ======== APULPF3_unitCircle ========
 */
int_fast16_t APULPF3_unitCircle(uint16_t numPoints,
                                uint16_t constant,
                                uint16_t phase,
                                bool conjugate,
                                APULPF3_ComplexVector *result)
{
    apuObject.scratchpad = false;
    if (APULPF3_inAPU(result->data))
    {
        /* If params are in APU memory, use them as is and do not copy result
         * back to CPU memory.
         */
        apuObject.scratchpad = true;
    }
    APULPF3_prepareResult(numPoints, APULPF3_RESULT_INPLACE, result->data);

    APUUnitCircle(numPoints, constant, phase, conjugate, apuObject.result);

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
    if (!apuObject.scratchpad)
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

    apuObject.argA = argA;
    apuObject.argB = argB;

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

    if (!apuObject.scratchpad)
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

    apuObject.argA = argA;
    apuObject.argB = argB;

    return inputSize;
}

/*
 *  ======== APULPF3_prepareResult ========
 */
void APULPF3_prepareResult(uint16_t resultSize, uint16_t offset, complex float *resultBuffer)
{
    apuObject.resultBuffer = resultBuffer;
    apuObject.resultSize   = resultSize;
    if (!apuObject.scratchpad)
    {
        /* If not in scratchpad mode, place at a known location in APU memory. */
        apuObject.result = (void *)(APURAM_DATA0_BASE + 8 * offset);
    }
    else
    {
        /* Otherwise result goes where the user has placed it in APU memory. */
        apuObject.result = resultBuffer;
    }
}
