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

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(driverlib/apu.h)

/* Forward declarations */
void APULPF3_copyBack();

/* Externs */
extern void APULPF3_loadFW();
extern bool APULPF3_inAPU(void *ptr);
extern void APULPF3_hwiIntFxn(uintptr_t arg);
extern void APULPF3_initHw(void);
extern int APULPF3_postNotifyFxn(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg);

extern APULPF3_Object apuObject;
extern SemaphoreP_Struct apuSem;
extern SemaphoreP_Struct apuAccessSem;
extern void *apuHwAttrs;

/*
 *  ======== APULPF3_copyBack ========
 * If not in scratchpad mode, copy data back from APU memory to a result buffer.
 */
void APULPF3_copyBack()
{
    float *memPtr;
    float *result;

    if (!apuObject.scratchpad)
    {
        memPtr = (float *)apuObject.result;
        result = (float *)apuObject.resultBuffer;

        /* Multiply resultSize by 2 because resultSize represents number of
         * complex floats (64 bits) but we transfer them in blocks of 32 bits.
         */
        memcpy(result, memPtr, (apuObject.resultSize * sizeof(float) * 2));
    }
}

void APULPF3_dataMemTransfer(const float *src, float *dst, size_t numElements)
{
    memcpy(dst, src, (numElements * sizeof(float)));
}

/*
 *  ======== APULPF3_init ========
 */
void APULPF3_init(void)
{
    HwiP_Params hwiParams;
    uintptr_t key;

    key = HwiP_disable();

    if (apuObject.isInitialized)
    {
        HwiP_restore(key);
        return;
    }

    apuObject.isInitialized = true;
    HwiP_restore(key);

    APULPF3_HWAttrs *hwAttrs = (APULPF3_HWAttrs *)apuHwAttrs;

    /* Clock the APU Peripheral. */
    Power_setDependency(PowerLPF3_PERIPH_APU);

    /* Load firmware into TOPSM memory. */
    APULPF3_loadFW();

    /* Set up APU peripheral */
    APULPF3_initHw();

    /* One semaphore to control when an API call has finished, and one to
     * control access into APU functions.
     */
    SemaphoreP_constructBinary(&(apuSem), 0);
    SemaphoreP_constructBinary(&(apuAccessSem), 1);

    /* Need to restore APU registers in SVT-domain after standby */
    Power_registerNotify(&(apuObject.postNotify), PowerLPF3_AWAKE_STANDBY, APULPF3_postNotifyFxn, (uintptr_t)NULL);

    /* Create Hwi object for this APU peripheral. */
    HwiP_Params_init(&hwiParams);
    hwiParams.priority = hwAttrs->intPriority;
    HwiP_construct(&(apuObject.hwi), INT_APU_IRQ, APULPF3_hwiIntFxn, &hwiParams);
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

    op = APU_OP_MAX;
    if (min)
    {
        op = APU_OP_MIN;
    }

    APUVectorMaxMin(vec->size, apuObject.argA, scalarThreshold, op, apuObject.result);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
}

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

    apuObject.scratchpad = false;
    if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(result->data))
    {
        /* If params are in APU memory, use them as is and do not copy result
         * back to CPU memory.
         */
        apuObject.scratchpad = true;
    }

    EVDmat.data = mat->data;
    EVDmat.rows = mat->size;
    EVDmat.cols = mat->size;
    APULPF3_prepareMatrices(&EVDmat, &EVDmat);

    /* Produces both the upper triangular part of a NxN in-place matrix and an
     * NxN matrix.
     */
    resultSize = (mat->size * mat->size) + ((mat->size * mat->size + mat->size) / 2);
    APULPF3_prepareResult(resultSize, APULPF3_RESULT_INPLACE, result->data);

    if (!apuObject.scratchpad)
    {
        /* apuObject.result will point to eigenvalues, which is followed by the
         * eigenvectors.
         */
        eigVecs      = apuObject.result + ((mat->size * mat->size + mat->size) / 2);
        result->size = resultSize;

        APUJacobiEVD(mat->size, apuObject.argA, eigVecs, maxIter, stopThreshold, epsTol);
    }
    else
    {
        /* apuObject.result will point to just the eigenvectors. Eigenvalues have
         * overwritten argA.
         */
        APUJacobiEVD(mat->size, apuObject.argA, apuObject.result, maxIter, stopThreshold, epsTol);
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
    apuObject.scratchpad = false;
    if (APULPF3_inAPU(mat->data) && APULPF3_inAPU(result->data))
    {
        /* If params are in APU memory, use them as is and do not copy result
         * back to CPU memory.
         */
        apuObject.scratchpad = true;
    }

    APULPF3_prepareMatrices(mat, mat);

    APULPF3_prepareResult(mat->cols * mat->rows, APULPF3_RESULT_INPLACE, result->data);

    APUGaussJordanElim(mat->rows, mat->cols, apuObject.argA, zeroThreshold);

    SemaphoreP_pend(&apuSem, SemaphoreP_WAIT_FOREVER);

    APULPF3_copyBack();

    return APULPF3_STATUS_SUCCESS;
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
    memcpy(memPtr, source, (argSize * sizeof(float) * 2));

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
            memcpy(memPtr + (elemCount), source + (shift + 2 * j), (sizeof(float) * 2));
            elemCount += 2;
        }

        shift += 2 * matrix.cols;
    }

    void *arg = (void *)(APURAM_DATA0_BASE + offset * 8);

    return arg;
}
