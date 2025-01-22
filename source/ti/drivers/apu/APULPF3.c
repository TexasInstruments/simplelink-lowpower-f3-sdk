/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
#include DeviceFamily_constructPath(inc/hw_vce.h)
#include DeviceFamily_constructPath(driverlib/apu.h)

/* Forward declarations */
static void APULPF3_loadFW();
static bool APULPF3_inAPU(void *ptr);
static void APULPF3_copyBack();
static void APULPF3_hwiIntFxn(uintptr_t arg);

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

} APULPF3_Object;

static APULPF3_Object object = {0};
static SemaphoreP_Struct apuSem;
static SemaphoreP_Struct apuAccessSem;
extern void *apuHwAttrs;

static void APULPF3_hwiIntFxn(uintptr_t arg)
{
    uint32_t status;

    /* Get and clear the interrupt */
    status                       = HWREG(VCE_BASE + VCE_O_MIS);
    HWREG(VCE_BASE + VCE_O_ICLR) = status;

    /* APU API has finished */
    if (status && (VCE_IMASK_API))
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
        *((uint32_t *)VCE_TOPSMRAM_BASE + i) = lrf_fw_image_seq_apu[i];
    }
}

/*
 *  ======== APULPF3_inAPU ========
 * Checks if an address is in APU memory.
 */
static bool APULPF3_inAPU(void *ptr)
{
    return VCERAM_DATA0_BASE <= (uint32_t)ptr && (uint32_t)ptr < VCERAM_DATA0_BASE + VCERAM_DATA0_SIZE;
}

/*
 *  ======== APULPF3_copyBack ========
 * If not in scratchpad mode, copy data back from APU memory to a result buffer.
 */
static void APULPF3_copyBack()
{
    if (!object.scratchpad)
    {
        float *mem_ptr = (float *)object.result;
        float *result  = (float *)object.resultBuffer;
        float loadVal;
        for (uint32_t i = 0; i < 2 * object.resultSize; i++)
        {
            /* Load individual floats with a nop to
             *  prevent back-to-back memory accesses
             */
            loadVal = mem_ptr[i];
            __asm volatile("nop");
            result[i] = loadVal;
            __asm volatile("nop");
        }
    }
}

/*
 *  ======== APULPF3_init ========
 */
void APULPF3_init(void)
{
    if (!object.isInitialized)
    {
        APULPF3_HWAttrs *hwAttrs = (APULPF3_HWAttrs *)apuHwAttrs;
        HwiP_Params hwiParams;

        /* Clock the APU Peripheral*/
        Power_setDependency(PowerLPF3_PERIPH_APU);

        /* Load firmware into TOPSM memory */
        APULPF3_loadFW();

        /* Configure APU and wait for the configuration to complete */
        APUSetConfig(0);
        APUWaitOnIrq();

        /* Create Hwi object for this APU peripheral. */
        HwiP_Params_init(&hwiParams);
        hwiParams.priority = hwAttrs->intPriority;
        HwiP_construct(&(object.hwi), INT_VCE_IRQ, APULPF3_hwiIntFxn, &hwiParams);

        HWREG(VCE_BASE + VCE_O_IMSET) = VCE_IMASK_API;

        /* One semaphore to control when an API call has finished, and one to
         * control access into APU functions
         */
        SemaphoreP_constructBinary(&(apuSem), 0);
        SemaphoreP_constructBinary(&(apuAccessSem), 1);

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
    if (vecA->size == vecB->size)
    {
        object.scratchpad = false;
        if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(vecB->data) && APULPF3_inAPU(result))
        {
            object.scratchpad = true;
        }

        uint16_t inputSize = APULPF3_prepareVectors(vecA, vecB);
        APULPF3_prepareResult(1, inputSize, result);

        /* Tell the APU to do a dot product with the arguments we have provided */
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
        return APULPF3_STATUS_SUCCESS;
    }

    return APULPF3_STATUS_ERROR;
}

/*
 *  ======== APULPF3_vectorMult ========
 */
int_fast16_t APULPF3_vectorMult(APULPF3_ComplexVector *vecA,
                                APULPF3_ComplexVector *vecB,
                                bool conjugate,
                                APULPF3_ComplexVector *result)
{
    if (vecA->size == vecB->size)
    {
        object.scratchpad = false;
        if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(vecB->data) && APULPF3_inAPU(result->data))
        {
            object.scratchpad = true;
        }
        uint16_t inputSize = APULPF3_prepareVectors(vecA, vecB);

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
        return APULPF3_STATUS_SUCCESS;
    }
    return APULPF3_STATUS_ERROR;
}

/*
 *  ======== APULPF3_vectorSum ========
 */
int_fast16_t APULPF3_vectorSum(APULPF3_ComplexVector *vecA, APULPF3_ComplexVector *vecB, APULPF3_ComplexVector *result)
{
    if (vecA->size == vecB->size)
    {
        object.scratchpad = false;
        if (APULPF3_inAPU(vecA->data) && APULPF3_inAPU(vecB->data) && APULPF3_inAPU(result->data))
        {
            object.scratchpad = true;
        }
        uint16_t inputSize = APULPF3_prepareVectors(vecA, vecB);

        APULPF3_prepareResult(vecA->size, inputSize, result->data);

        APUVectorSum(vecA->size, object.argA, object.argB, object.result);

        SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

        APULPF3_copyBack();

        return APULPF3_STATUS_SUCCESS;
    }
    return APULPF3_STATUS_ERROR;
}

/*
 *  ======== APULPF3_cartesianToPolarVector ========
 */
int_fast16_t APULPF3_cartesianToPolarVector(APULPF3_ComplexVector *vec, APULPF3_ComplexVector *result)
{
    object.scratchpad = false;
    if (APULPF3_inAPU(vec->data) && APULPF3_inAPU(result->data))
    {
        object.scratchpad = true;
    }
    uint16_t inputSize = APULPF3_prepareVectors(vec, vec);

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
    object.scratchpad = false;
    if (APULPF3_inAPU(vec->data) && APULPF3_inAPU(result->data) && APULPF3_inAPU(temp))
    {
        object.scratchpad = true;
    }
    else
    {
        /* Assign temp vector after input and result */
        temp = object.result + 2 * vec->size;
    }

    uint16_t inputSize = APULPF3_prepareVectors(vec, vec);

    APULPF3_prepareResult(vec->size, inputSize, result->data);

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
    object.scratchpad = false;
    if (APULPF3_inAPU(vec->data) && APULPF3_inAPU(result->data))
    {
        object.scratchpad = true;
    }

    uint16_t inputSize = APULPF3_prepareVectors(vec, vec);

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
    /* Check if size is a power of 2 */
    if ((vec->size & (vec->size - 1)) == 0)
    {
        /* FFT/ IFFT is a two-part operation. First the APU is configured,
         * which produces no result. Then the actual computation happens,
         * which may or may not use scratchpad mode
         */
        object.scratchpad = false;
        bool isInAPU      = false;
        if (APULPF3_inAPU(vec->data) && APULPF3_inAPU(result->data))
        {
            isInAPU = true;
            APUConfigFft(vec->size, vec->data);
        }
        else
        {
            APUConfigFft(vec->size, (void *)VCERAM_DATA0_BASE);
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

        return APULPF3_STATUS_SUCCESS;
    }
    return APULPF3_STATUS_ERROR;
}

/* Matrix functions */

/*
 *  ======== APULPF3_matrixMult ========
 */
int_fast16_t APULPF3_matrixMult(APULPF3_ComplexMatrix *matA, APULPF3_ComplexMatrix *matB, APULPF3_ComplexMatrix *result)
{
    if (matA->cols == matB->rows)
    {
        object.scratchpad = false;
        if (APULPF3_inAPU(matA->data) && APULPF3_inAPU(matB->data) && APULPF3_inAPU(result->data))
        {
            object.scratchpad = true;
        }

        uint16_t inputSize = APULPF3_prepareMatrices(matA, matB);
        APULPF3_prepareResult(matA->cols * matB->rows, inputSize, result->data);

        APUMatrixMult(matA->rows, matA->cols, matB->cols, object.argA, object.argB, object.result);

        SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

        APULPF3_copyBack();

        return APULPF3_STATUS_SUCCESS;
    }
    return APULPF3_STATUS_ERROR;
}

/*
 *  ======== APULPF3_matrixSum ========
 */
int_fast16_t APULPF3_matrixSum(APULPF3_ComplexMatrix *matA, APULPF3_ComplexMatrix *matB, APULPF3_ComplexMatrix *result)
{
    if (matA->rows == matB->rows && matA->cols == matB->cols)
    {
        object.scratchpad = false;
        if (APULPF3_inAPU(matA->data) && APULPF3_inAPU(matB->data) && APULPF3_inAPU(result->data))
        {
            object.scratchpad = true;
        }

        uint16_t inputSize = APULPF3_prepareMatrices(matA, matB);

        APULPF3_prepareResult(matA->rows * matA->cols, inputSize, result->data);

        APUMatrixSum(matA->rows, matA->cols, object.argA, object.argB, object.result);

        SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

        APULPF3_copyBack();

        return APULPF3_STATUS_SUCCESS;
    }

    return APULPF3_STATUS_ERROR;
}

/*
 *  ======== APULPF3_matrixScalarSum ========
 */
int_fast16_t APULPF3_matrixScalarSum(APULPF3_ComplexMatrix *mat, float complex *scalar, APULPF3_ComplexMatrix *result)
{
    object.scratchpad = false;
    float complex *tempArgB;
    if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(scalar) && APULPF3_inAPU(result->data))
    {
        object.scratchpad = true;
        tempArgB          = scalar;
    }
    else
    {
        void *argB = APULPF3_loadArgMirrored(1, mat->rows * mat->cols, scalar);
        tempArgB   = argB;
    }

    /* This function overwrites argB so we have saved it somewhere else */
    uint16_t inputSize = APULPF3_prepareMatrices(mat, mat);

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
    object.scratchpad = false;
    float complex *tempArgB;
    if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(scalar) && APULPF3_inAPU(result->data))
    {
        object.scratchpad = true;
        tempArgB          = scalar;
    }
    else
    {
        void *argB = APULPF3_loadArgMirrored(1, mat->rows * mat->cols, scalar);
        tempArgB   = argB;
    }

    uint16_t inputSize = APULPF3_prepareMatrices(mat, mat);

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
    object.scratchpad = false;
    if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(result))
    {
        object.scratchpad = true;
    }

    uint16_t inputSize = APULPF3_prepareMatrices(mat, mat);

    APULPF3_prepareResult(1, inputSize, result);

    APUMatrixNorm(mat->rows, mat->cols, object.argA, object.result);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    /* The APU actually returns the square of the norm */
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
                               APULPF3_ComplexVector *result)
{
    object.scratchpad = false;
    if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(result->data))
    {
        object.scratchpad = true;
    }

    APULPF3_ComplexMatrix EVDmat = {.data = mat->data, .rows = mat->size, .cols = mat->size};
    APULPF3_prepareMatrices(&EVDmat, &EVDmat);

    /* Produces both the upper triangular part of a NxN in-place matrix and an NxN matrix */
    uint16_t resultSize = (mat->size * mat->size) + ((mat->size * mat->size + mat->size) / 2);
    APULPF3_prepareResult(resultSize, APULPF3_RESULT_INPLACE, result->data);

    if (!object.scratchpad)
    {
        /* object.result will point to eigenvalues, which is followed by the eigenvectors*/
        float complex *eigVecs = object.result + ((mat->size * mat->size + mat->size) / 2);
        result->size           = resultSize;

        APUJacobiEVD(mat->size, object.argA, eigVecs, maxIter, stopThreshold);
    }
    else
    {
        /* object.result will point to just the eigenvectors. Eigenvalues have
         * overwritten argA
         */
        APUJacobiEVD(mat->size, object.argA, object.result, maxIter, stopThreshold);
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
        object.scratchpad = true;
    }

    APULPF3_prepareMatrices(mat, mat);

    APULPF3_prepareResult(mat->cols * mat->rows, APULPF3_RESULT_INPLACE, result->data);

    APUGaussJordanElim(mat->rows, mat->cols, object.argA, zeroThreshold);

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
            /* If they aren't the same put argB right after argA */
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
            /* If they aren't the same put argB right after argA */
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
    float *mem_ptr = (float *)(VCERAM_DATA0_BASE + offset * 8);
    float *source  = (float *)src;
    float loadVal;
    for (uint32_t i = 0; i < 2 * argSize; i++)
    {
        /* Load individual floats with a nop to
         *  prevent back-to-back memory accesses
         */
        loadVal = source[i];
        __asm volatile("nop");
        mem_ptr[i] = loadVal;
        __asm volatile("nop");
    }
    void *arg = (void *)(VCERAM_DATA0_BASE + offset * 8);

    return arg;
}

/*
 *  ======== APULPF3_loadTriangular ========
 */
void *APULPF3_loadTriangular(APULPF3_ComplexMatrix *mat, uint16_t offset)
{
    APULPF3_ComplexMatrix matrix = *mat;
    float *mem_ptr               = (float *)(VCERAM_DATA0_BASE + offset * 8);
    float *source                = (float *)matrix.data;
    float loadVal;
    uint16_t elemCount = 0;
    uint16_t shift     = 0;
    for (uint32_t i = 1; i < matrix.cols + 1; i++)
    {

        for (uint32_t j = 0; j < i; j++)
        {
            loadVal = source[shift + 2 * j];
            __asm volatile("nop");
            mem_ptr[elemCount] = loadVal;
            elemCount++;
            __asm volatile("nop");
            loadVal = source[shift + 2 * j + 1];
            __asm volatile("nop");
            mem_ptr[elemCount] = loadVal;
            elemCount++;
            __asm volatile("nop");
        }
        shift += 2 * matrix.cols;
    }
    void *arg = (void *)(VCERAM_DATA0_BASE + offset * 8);

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
        /* If not in scratchpad mode, place at a known location in APU memory */
        object.result = (void *)(VCERAM_DATA0_BASE + 8 * offset);
    }
    else
    {
        /* Otherwise result goes where the user has placed it in APU memory */
        object.result = resultBuffer;
    }
}
