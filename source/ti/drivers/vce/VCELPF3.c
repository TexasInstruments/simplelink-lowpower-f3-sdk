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
 *  ======== VCELPF3.c ========
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
#include <ti/drivers/vce/VCELPF3.h>
#include <ti/drivers/vce/VCELPF3_FW.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_vce.h)
#include DeviceFamily_constructPath(driverlib/vce.h)

/* Forward declarations */
static void VCELPF3_loadFW();
static bool VCELPF3_inVCE(void *ptr);
static void VCELPF3_copyBack();
static void VCELPF3_hwiIntFxn(uintptr_t arg);

/*
 *  VCE object structure
 */
typedef struct
{
    HwiP_Struct hwi;

    float complex *result; /* Temporary result pointer, in VCE memory */

    float complex *argA; /* Argument pointer, in VCE memory */

    float complex *argB; /* Argument pointer, in VCE memory */

    float complex *resultBuffer; /* Final result pointer, may be in VCE memory*/

    bool scratchpad; /* Whether or not to copy to/from VCE memory */

    uint16_t resultSize; /* VCE result size */

    bool isInitialized; /* Has open() been called */

} VCELPF3_Object;

static VCELPF3_Object object = {0};
static SemaphoreP_Struct vceSem;
static SemaphoreP_Struct vceAccessSem;
extern void *vceHwAttrs;

static void VCELPF3_hwiIntFxn(uintptr_t arg)
{
    uint32_t status;

    /* Get and clear the interrupt */
    status                       = HWREG(VCE_BASE + VCE_O_MIS);
    HWREG(VCE_BASE + VCE_O_ICLR) = status;

    /* VCE API has finished */
    if (status && (VCE_IMASK_API))
    {
        SemaphoreP_post(&vceSem);
    }
}

/*
 *  ======== VCELPF3_loadFW ========
 * Used to copy topsm firmware into VCE program memory, if needed.
 */
static void VCELPF3_loadFW()
{
    for (uint16_t i = 0; i < VCELPF3_FW_SIZE; i++)
    {
        *((uint32_t *)VCE_TOPSMRAM_BASE + i) = lrf_fw_image_seq_vce[i];
    }
}

/*
 *  ======== VCELPF3_inVCE ========
 * Checks if an address is in VCE memory.
 */
static bool VCELPF3_inVCE(void *ptr)
{
    return VCERAM_DATA0_BASE <= (uint32_t)ptr && (uint32_t)ptr < VCERAM_DATA0_BASE + VCERAM_DATA0_SIZE;
}

/*
 *  ======== VCELPF3_copyBack ========
 * If not in scratchpad mode, copy data back from VCE memory to a result buffer.
 */
static void VCELPF3_copyBack()
{
    if (!object.scratchpad)
    {
        float *mem_ptr = (float *)object.result;
        float *result  = (float *)object.resultBuffer;
        float loadVal;
        for (uint32_t i = 0; i < 2 * object.resultSize; i++)
        {
            /* We have to insert NOPs when doing back-to-back memory operations
             * in VCE memory, otherwise there may be a crash
             */
            loadVal = mem_ptr[i];
            __asm volatile("nop");
            __asm volatile("nop");
            result[i] = loadVal;
        }
    }
}

/*
 *  ======== VCELPF3_init ========
 */
void VCELPF3_init(void)
{
    if (!object.isInitialized)
    {
        VCELPF3_HWAttrs *hwAttrs = (VCELPF3_HWAttrs *)vceHwAttrs;
        HwiP_Params hwiParams;

        /* Clock the VCE Peripheral*/
        Power_setDependency(PowerLPF3_PERIPH_VCE);

        /* Load firmware into TOPSM memory */
        VCELPF3_loadFW();

        /* Configure VCE and wait for the configuration to complete */
        VCESetConfig(0);
        VCEWaitOnIrq();

        /* Create Hwi object for this VCE peripheral. */
        HwiP_Params_init(&hwiParams);
        hwiParams.priority = hwAttrs->intPriority;
        HwiP_construct(&(object.hwi), INT_VCE_IRQ, VCELPF3_hwiIntFxn, &hwiParams);

        HWREG(VCE_BASE + VCE_O_IMSET) = VCE_IMASK_API;

        /* One semaphore to control when an API call has finished, and one to
         * control access into VCE functions
         */
        SemaphoreP_constructBinary(&(vceSem), 0);
        SemaphoreP_constructBinary(&(vceAccessSem), 1);

        object.isInitialized = true;
    }
}

/*
 *  ======== VCELPF3_startOperationSequence ========
 */
void VCELPF3_startOperationSequence()
{
    SemaphoreP_pend(&vceAccessSem, SemaphoreP_WAIT_FOREVER);
    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*
 *  ======== VCELPF3_startOperationSequence ========
 */
void VCELPF3_stopOperationSequence()
{
    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
    SemaphoreP_post(&vceAccessSem);
}
/* Vector-vector functions */

/*
 *  ======== VCELPF3_dotProduct ========
 */
int_fast16_t VCELPF3_dotProduct(VCELPF3_ComplexVector *vecA,
                                VCELPF3_ComplexVector *vecB,
                                bool conjugate,
                                float complex *result)
{
    if (vecA->size == vecB->size)
    {
        object.scratchpad = false;
        if (VCELPF3_inVCE(vecA->data) && VCELPF3_inVCE(vecB->data) && VCELPF3_inVCE(result))
        {
            object.scratchpad = true;
        }

        uint16_t inputSize = VCELPF3_prepareVectors(vecA, vecB);
        VCELPF3_prepareResult(1, inputSize, result);

        /* Tell the VCE to do a dot product with the arguments we have provided */
        if (conjugate)
        {
            VCEVectorDotConj(vecA->size, object.argA, object.argB, object.result);
        }
        else
        {
            VCEVectorDot(vecA->size, object.argA, object.argB, object.result);
        }

        SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

        VCELPF3_copyBack();
        return VCELPF3_STATUS_SUCCESS;
    }

    return VCELPF3_STATUS_ERROR;
}

/*
 *  ======== VCELPF3_vectorMult ========
 */
int_fast16_t VCELPF3_vectorMult(VCELPF3_ComplexVector *vecA,
                                VCELPF3_ComplexVector *vecB,
                                bool conjugate,
                                VCELPF3_ComplexVector *result)
{
    if (vecA->size == vecB->size)
    {
        object.scratchpad = false;
        if (VCELPF3_inVCE(vecA->data) && VCELPF3_inVCE(vecB->data) && VCELPF3_inVCE(result->data))
        {
            object.scratchpad = true;
        }
        uint16_t inputSize = VCELPF3_prepareVectors(vecA, vecB);

        VCELPF3_prepareResult(vecA->size, inputSize, result->data);

        if (conjugate)
        {
            VCEVectorMultConj(vecA->size, object.argA, object.argB, object.result);
        }
        else
        {
            VCEVectorMult(vecA->size, object.argA, object.argB, object.result);
        }

        SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

        VCELPF3_copyBack();
        return VCELPF3_STATUS_SUCCESS;
    }
    return VCELPF3_STATUS_ERROR;
}

/*
 *  ======== VCELPF3_vectorSum ========
 */
int_fast16_t VCELPF3_vectorSum(VCELPF3_ComplexVector *vecA, VCELPF3_ComplexVector *vecB, VCELPF3_ComplexVector *result)
{
    if (vecA->size == vecB->size)
    {
        object.scratchpad = false;
        if (VCELPF3_inVCE(vecA->data) && VCELPF3_inVCE(vecB->data) && VCELPF3_inVCE(result->data))
        {
            object.scratchpad = true;
        }
        uint16_t inputSize = VCELPF3_prepareVectors(vecA, vecB);

        VCELPF3_prepareResult(vecA->size, inputSize, result->data);

        VCEVectorSum(vecA->size, object.argA, object.argB, object.result);

        SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

        VCELPF3_copyBack();

        return VCELPF3_STATUS_SUCCESS;
    }
    return VCELPF3_STATUS_ERROR;
}

/*
 *  ======== VCELPF3_cartesianToPolarVector ========
 */
int_fast16_t VCELPF3_cartesianToPolarVector(VCELPF3_ComplexVector *vec, VCELPF3_ComplexVector *result)
{
    object.scratchpad = false;
    if (VCELPF3_inVCE(vec->data) && VCELPF3_inVCE(result->data))
    {
        object.scratchpad = true;
    }
    uint16_t inputSize = VCELPF3_prepareVectors(vec, vec);

    VCELPF3_prepareResult(vec->size, inputSize, result->data);

    VCEVectorCart2Pol(vec->size, object.argA, object.result);

    SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

    VCELPF3_copyBack();

    return VCELPF3_STATUS_SUCCESS;
}

/*
 *  ======== VCELPF3_polarToCartesianVector ========
 */
int_fast16_t VCELPF3_polarToCartesianVector(VCELPF3_ComplexVector *vec,
                                            float complex *temp,
                                            VCELPF3_ComplexVector *result)
{
    object.scratchpad = false;
    if (VCELPF3_inVCE(vec->data) && VCELPF3_inVCE(result->data) && VCELPF3_inVCE(temp))
    {
        object.scratchpad = true;
    }
    else
    {
        /* Assign temp vector after input and result */
        temp = object.result + 2 * vec->size;
    }

    uint16_t inputSize = VCELPF3_prepareVectors(vec, vec);

    VCELPF3_prepareResult(vec->size, inputSize, result->data);

    VCEVectorPol2Cart(vec->size, object.argA, object.result, temp);

    SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

    VCELPF3_copyBack();
    return VCELPF3_STATUS_SUCCESS;
}

/* Vector algorithms */

/*
 *  ======== VCELPF3_sortVector ========
 */
int_fast16_t VCELPF3_sortVector(VCELPF3_ComplexVector *vec, VCELPF3_ComplexVector *result)
{
    object.scratchpad = false;
    if (VCELPF3_inVCE(vec->data) && VCELPF3_inVCE(result->data))
    {
        object.scratchpad = true;
    }

    VCELPF3_prepareVectors(vec, vec);

    VCELPF3_prepareResult(vec->size, VCELPF3_RESULT_INPLACE, result->data);

    VCEVectorSort(vec->size, object.argA);

    SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

    VCELPF3_copyBack();

    return VCELPF3_STATUS_SUCCESS;
}

/*
 *  ======== VCELPF3_covMatrixSpatialSmoothing ========
 */
int_fast16_t VCELPF3_covMatrixSpatialSmoothing(VCELPF3_ComplexVector *vec,
                                               uint16_t covMatrixSize,
                                               bool fbAveraging,
                                               VCELPF3_ComplexTriangleMatrix *result)
{
    object.scratchpad = false;
    if (VCELPF3_inVCE(vec->data) && VCELPF3_inVCE(result->data))
    {
        object.scratchpad = true;
    }

    uint16_t inputSize = VCELPF3_prepareVectors(vec, vec);

    VCELPF3_prepareResult(covMatrixSize * covMatrixSize, inputSize, result->data);

    VCESpSmoothCovMatrix(vec->size, object.argA, covMatrixSize, object.result, fbAveraging);

    SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

    VCELPF3_copyBack();

    return VCELPF3_STATUS_SUCCESS;
}

/*
 *  ======== VCELPF3_computeFFT ========
 */
int_fast16_t VCELPF3_computeFFT(VCELPF3_ComplexVector *vec, bool inverse, VCELPF3_ComplexVector *result)
{
    /* Check if size is a power of 2 */
    if ((vec->size & (vec->size - 1)) == 0)
    {
        /* FFT/ IFFT is a two-part operation. First the VCE is configured,
         * which produces no result. Then the actual computation happens,
         * which may or may not use scratchpad mode
         */
        object.scratchpad = false;
        bool isInVCE      = false;
        if (VCELPF3_inVCE(vec->data) && VCELPF3_inVCE(result->data))
        {
            isInVCE = true;
            VCEConfigFft(vec->size, vec->data);
        }
        else
        {
            VCEConfigFft(vec->size, (void *)VCERAM_DATA0_BASE);
        }

        SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

        object.scratchpad = isInVCE;

        VCELPF3_prepareVectors(vec, vec);

        VCELPF3_prepareResult(vec->size, VCELPF3_RESULT_INPLACE, result->data);

        if (inverse)
        {
            VCEComputeIfft(vec->size, object.argA);
        }
        else
        {
            VCEComputeFft(vec->size, object.argA);
        }

        SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

        VCELPF3_copyBack();

        return VCELPF3_STATUS_SUCCESS;
    }
    return VCELPF3_STATUS_ERROR;
}

/* Matrix functions */

/*
 *  ======== VCELPF3_matrixMult ========
 */
int_fast16_t VCELPF3_matrixMult(VCELPF3_ComplexMatrix *matA, VCELPF3_ComplexMatrix *matB, VCELPF3_ComplexMatrix *result)
{
    if (matA->cols == matB->rows)
    {
        object.scratchpad = false;
        if (VCELPF3_inVCE(matA->data) && VCELPF3_inVCE(matB->data) && VCELPF3_inVCE(result->data))
        {
            object.scratchpad = true;
        }

        uint16_t inputSize = VCELPF3_prepareMatrices(matA, matB);
        VCELPF3_prepareResult(matA->cols * matB->rows, inputSize, result->data);

        VCEMatrixMult(matA->rows, matA->cols, matB->cols, object.argA, object.argB, object.result);

        SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

        VCELPF3_copyBack();

        return VCELPF3_STATUS_SUCCESS;
    }
    return VCELPF3_STATUS_ERROR;
}

/*
 *  ======== VCELPF3_matrixSum ========
 */
int_fast16_t VCELPF3_matrixSum(VCELPF3_ComplexMatrix *matA, VCELPF3_ComplexMatrix *matB, VCELPF3_ComplexMatrix *result)
{
    if (matA->rows == matB->rows && matA->cols == matB->cols)
    {
        object.scratchpad = false;
        if (VCELPF3_inVCE(matA->data) && VCELPF3_inVCE(matB->data) && VCELPF3_inVCE(result->data))
        {
            object.scratchpad = true;
        }

        uint16_t inputSize = VCELPF3_prepareMatrices(matA, matB);

        VCELPF3_prepareResult(matA->rows * matA->cols, inputSize, result->data);

        VCEMatrixSum(matA->rows, matA->cols, object.argA, object.argB, object.result);

        SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

        VCELPF3_copyBack();

        return VCELPF3_STATUS_SUCCESS;
    }

    return VCELPF3_STATUS_ERROR;
}

/*
 *  ======== VCELPF3_matrixScalarSum ========
 */
int_fast16_t VCELPF3_matrixScalarSum(VCELPF3_ComplexMatrix *mat, float complex *scalar, VCELPF3_ComplexMatrix *result)
{
    object.scratchpad = false;
    float complex *tempArgB;
    if (VCELPF3_inVCE(mat->data) && VCELPF3_inVCE(scalar) && VCELPF3_inVCE(result->data))
    {
        object.scratchpad = true;
        tempArgB          = scalar;
    }
    else
    {
        void *argB = VCELPF3_loadArgMirrored(1, mat->rows * mat->cols, scalar);
        tempArgB   = argB;
    }

    /* This function overwrites argB so we have saved it somewhere else */
    uint16_t inputSize = VCELPF3_prepareMatrices(mat, mat);

    object.argB = tempArgB;

    VCELPF3_prepareResult(mat->rows * mat->cols, 1 + inputSize, result->data);

    VCEMatrixScalarSum(mat->rows, mat->cols, object.argA, object.argB, object.result);

    SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

    VCELPF3_copyBack();

    return VCELPF3_STATUS_SUCCESS;
}

/*
 *  ======== VCELPF3_matrixScalarMult ========
 */
int_fast16_t VCELPF3_matrixScalarMult(VCELPF3_ComplexMatrix *mat, float complex *scalar, VCELPF3_ComplexMatrix *result)
{
    object.scratchpad = false;
    float complex *tempArgB;
    if (VCELPF3_inVCE(mat->data) && VCELPF3_inVCE(scalar) && VCELPF3_inVCE(result->data))
    {
        object.scratchpad = true;
        tempArgB          = scalar;
    }
    else
    {
        void *argB = VCELPF3_loadArgMirrored(1, mat->rows * mat->cols, scalar);
        tempArgB   = argB;
    }

    uint16_t inputSize = VCELPF3_prepareMatrices(mat, mat);

    object.argB = tempArgB;

    VCELPF3_prepareResult(mat->rows * mat->cols, 1 + inputSize, result->data);

    VCEMatrixScalarMult(mat->rows, mat->cols, object.argA, object.argB, object.result);

    SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

    VCELPF3_copyBack();

    return VCELPF3_STATUS_SUCCESS;
}

/*
 *  ======== VCELPF3_matrixNorm ========
 */
int_fast16_t VCELPF3_matrixNorm(VCELPF3_ComplexMatrix *mat, float complex *result)
{
    object.scratchpad = false;
    if (VCELPF3_inVCE(mat->data) && VCELPF3_inVCE(result))
    {
        object.scratchpad = true;
    }

    uint16_t inputSize = VCELPF3_prepareMatrices(mat, mat);

    VCELPF3_prepareResult(1, inputSize, result);

    VCEMatrixNorm(mat->rows, mat->cols, object.argA, object.result);

    SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

    VCELPF3_copyBack();

    /* The VCE actually returns the square of the norm */
    *result = sqrtf(*result);
    return VCELPF3_STATUS_SUCCESS;
}

/* Matrix algorithms */

/*
 *  ======== VCELPF3_jacobiEVD ========
 */
int_fast16_t VCELPF3_jacobiEVD(VCELPF3_ComplexTriangleMatrix *mat,
                               uint16_t maxIter,
                               float stopThreshold,
                               VCELPF3_ComplexVector *result)
{
    object.scratchpad = false;
    if (VCELPF3_inVCE(mat->data) && VCELPF3_inVCE(result->data))
    {
        object.scratchpad = true;
    }

    VCELPF3_ComplexMatrix EVDmat = {.data = mat->data, .rows = mat->size, .cols = mat->size};
    VCELPF3_prepareMatrices(&EVDmat, &EVDmat);

    /* Produces both the upper triangular part of a NxN in-place matrix and an NxN matrix */
    uint16_t resultSize = (mat->size * mat->size) + ((mat->size * mat->size + mat->size) / 2);
    VCELPF3_prepareResult(resultSize, VCELPF3_RESULT_INPLACE, result->data);

    if (!object.scratchpad)
    {
        /* object.result will point to eigenvalues, which is followed by the eigenvectors*/
        float complex *eigVecs = object.result + ((mat->size * mat->size + mat->size) / 2);
        result->size           = resultSize;

        VCEJacobiEVD(mat->size, object.argA, eigVecs, maxIter, stopThreshold);
    }
    else
    {
        /* object.result will point to just the eigenvectors. Eigenvalues have
         * overwritten argA
         */
        VCEJacobiEVD(mat->size, object.argA, object.result, maxIter, stopThreshold);
        result->size = (mat->size * mat->size);
    }

    SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

    VCELPF3_copyBack();

    return VCELPF3_STATUS_SUCCESS;
}

/*
 *  ======== VCELPF3_gaussJordanElim ========
 */
int_fast16_t VCELPF3_gaussJordanElim(VCELPF3_ComplexMatrix *mat, float zeroThreshold, VCELPF3_ComplexMatrix *result)
{
    object.scratchpad = false;
    if (VCELPF3_inVCE(mat->data) && VCELPF3_inVCE(result->data))
    {
        object.scratchpad = true;
    }

    VCELPF3_prepareMatrices(mat, mat);

    VCELPF3_prepareResult(mat->cols * mat->rows, VCELPF3_RESULT_INPLACE, result->data);

    VCEGaussJordanElim(mat->rows, mat->cols, object.argA, zeroThreshold);

    SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

    VCELPF3_copyBack();

    return VCELPF3_STATUS_SUCCESS;
}

/*
 *  ======== VCELPF3_unitCircle ========
 */
int_fast16_t VCELPF3_unitCircle(uint16_t numPoints,
                                uint16_t constant,
                                uint16_t phase,
                                bool conjugate,
                                VCELPF3_ComplexVector *result)
{
    object.scratchpad = false;
    if (VCELPF3_inVCE(result->data))
    {
        object.scratchpad = true;
    }
    VCELPF3_prepareResult(numPoints, VCELPF3_RESULT_INPLACE, result->data);

    VCEUnitCircle(numPoints, constant, phase, conjugate, object.result);

    SemaphoreP_pend(&vceSem, SemaphoreP_WAIT_FOREVER);

    VCELPF3_copyBack();

    return VCELPF3_STATUS_SUCCESS;
}

/* Utility functions */
/*
 *  ======== VCELPF3_prepareVectors ========
 */
uint16_t VCELPF3_prepareVectors(VCELPF3_ComplexVector *vecA, VCELPF3_ComplexVector *vecB)
{
    complex float *argA = vecA->data;
    complex float *argB = vecB->data;
    uint16_t inputSize  = 0;
    if (!object.scratchpad)
    {
        argA = VCELPF3_loadArgMirrored(vecA->size, 0, vecA->data);
        /* Are the vectors the same? */
        if ((vecA->data == vecB->data) && (vecA->size == vecB->size))
        {
            argB      = argA;
            inputSize = vecA->size;
        }
        else
        {
            /* If they aren't the same put argB right after argA */
            argB      = VCELPF3_loadArgMirrored(vecB->size, vecA->size, vecB->data);
            inputSize = vecA->size + vecB->size;
        }
    }

    object.argA = argA;
    object.argB = argB;

    return inputSize;
}

/*
 *  ======== VCELPF3_prepareMatrices ========
 */
uint16_t VCELPF3_prepareMatrices(VCELPF3_ComplexMatrix *matA, VCELPF3_ComplexMatrix *matB)
{
    complex float *argA = matA->data;
    complex float *argB = matB->data;
    uint16_t inputSize  = 0;

    uint16_t sizeA = matA->cols * matA->rows;
    uint16_t sizeB = matB->cols * matB->rows;

    if (!object.scratchpad)
    {
        argA = VCELPF3_loadArgMirrored(sizeA, 0, matA->data);
        /* Are the matrices the same? */
        if ((matA->data == matB->data) && (matA->rows == matB->rows) && (matA->cols == matB->cols))
        {
            argB      = argA;
            inputSize = sizeA;
        }
        else
        {
            /* If they aren't the same put argB right after argA */
            argB      = VCELPF3_loadArgMirrored(sizeB, sizeA, matB->data);
            inputSize = sizeA + sizeB;
        }
    }

    object.argA = argA;
    object.argB = argB;

    return inputSize;
}

/*
 *  ======== VCELPF3_loadArgMirrored ========
 */
void *VCELPF3_loadArgMirrored(uint16_t argSize, uint16_t offset, float complex *src)
{
    float *mem_ptr = (float *)(VCERAM_DATA0_BASE + offset * 8);
    float *source  = (float *)src;
    float loadVal;
    for (uint32_t i = 0; i < 2 * argSize; i++)
    {
        /* We have to insert NOPs when doing back-to-back memory operations
         * in VCE memory, otherwise there may be a crash
         */
        loadVal = source[i];
        __asm volatile("nop");
        __asm volatile("nop");
        mem_ptr[i] = loadVal;
    }
    void *arg = (void *)(VCERAM_DATA0_BASE + offset * 8);

    return arg;
}

/*
 *  ======== VCELPF3_loadTriangular ========
 */
void *VCELPF3_loadTriangular(VCELPF3_ComplexMatrix *mat, uint16_t offset)
{
    VCELPF3_ComplexMatrix matrix = *mat;
    float *mem_ptr               = (float *)(VCERAM_DATA0_BASE + offset * 8);
    float *source                = (float *)matrix.data;
    float loadVal;
    uint16_t elemCount = 0;
    uint16_t shift     = 0;
    for (uint32_t i = 1; i < matrix.cols + 1; i++)
    {

        for (uint32_t j = 0; j < i; j++)
        {
            /* We have to insert NOPs when doing back-to-back memory operations
             * in VCE memory, otherwise there may be a crash
             */
            loadVal = source[shift + 2 * j];
            __asm volatile("nop");
            mem_ptr[elemCount] = loadVal;
            elemCount++;
            loadVal = source[shift + 2 * j + 1];
            __asm volatile("nop");
            mem_ptr[elemCount] = loadVal;
            elemCount++;
        }
        shift += 2 * matrix.cols;
    }
    void *arg = (void *)(VCERAM_DATA0_BASE + offset * 8);

    return arg;
}

/*
 *  ======== VCELPF3_prepareResult ========
 */
void VCELPF3_prepareResult(uint16_t resultSize, uint16_t offset, complex float *resultBuffer)
{
    object.resultBuffer = resultBuffer;
    object.resultSize   = resultSize;
    if (!object.scratchpad)
    {
        /* If not in scratchpad mode, place at a known location in VCE memory */
        object.result = (void *)(VCERAM_DATA0_BASE + 8 * offset);
    }
    else
    {
        /* Otherwise result goes where the user has placed it in VCE memory */
        object.result = resultBuffer;
    }
}
