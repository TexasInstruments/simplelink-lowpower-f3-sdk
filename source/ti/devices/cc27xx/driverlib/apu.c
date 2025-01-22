/******************************************************************************
 *  Filename:       apu.c
 *
 *  Description:    Driver for the APU peripheral.
 *
 *  Copyright (c) 2024 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1) Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2) Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *  3) Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/
#include "../inc/hw_vce.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_types.h"
#include "apu.h"

//*****************************************************************************
//
// Enum definnes for the APU_API register, to control APU operation.
//
//*****************************************************************************
enum APUApi
{
    APU_API_NOP               = 0x0000,
    APU_API_CONFIG            = 0x0001,
    APU_API_DOTPROD           = 0x0002,
    APU_API_VECTMULT          = 0x0003,
    APU_API_VECTSUM           = 0x0004,
    APU_API_MATMATMULT        = 0x0005,
    APU_API_UNITCIRC          = 0x0006,
    APU_API_SYMMATRIXVECTPROD = 0x0007,
    APU_API_MATRIXMULT        = 0x0008,
    APU_API_HERMATRIXMULT     = 0x0009,
    APU_API_SYMMATRIXMULT     = 0x000A,
    APU_API_MATRIXSUM         = 0x000B,
    APU_API_SCALARMULT        = 0x000C,
    APU_API_MATRIXSCALARSUM   = 0x000D,
    APU_API_POLAR             = 0x000E,
    APU_API_CARTESIAN         = 0x000F,
    APU_API_COVMATRIX         = 0x0010,
    APU_API_EIGEN             = 0x0011,
    APU_API_MATRIXINV         = 0x0012,
    APU_API_MATRIXNORM        = 0x0013,
    APU_API_FFT               = 0x0014,
    APU_API_DCT               = 0x0015,
    APU_API_SORT              = 0x0016,
    APU_API_GAUSS             = 0x0017
};

//****************************************************************************
// Generic Definition
//****************************************************************************
#define APU_MSGBOX_CMDOK 0x0001 //!< Indicate operation has completed
#define APU_HEAP_ADDR    0x03CE //!< 50 positions for Heap, from 974->1023

//*****************************************************************************
//
// Wait for the APU interrupt
//
//*****************************************************************************
void APUWaitOnIrq(void)
{
    // Wait for RIS API to be set
    while (!(HWREG(VCE_BASE + VCE_O_RIS) & VCE_RIS_API_M)) {}
    // Clear RIS API
    HWREG(VCE_BASE + VCE_O_ICLR) = VCE_ICLR_API_YES;
}

//*****************************************************************************
//
// Checks the APU message box to see if it has completed its operation.
//
//*****************************************************************************
bool APUOperationDone(void)
{
    return ((HWREG(VCE_BASE + VCE_O_MSGBOX) & APU_MSGBOX_CMDOK) == APU_MSGBOX_CMDOK);
}

//*****************************************************************************
//
// Wait for the APU to both begin and complete its operation.
//
//*****************************************************************************
void APUWaitOp(void)
{
    // Wait for start
    while (APUOperationDone()) {}
    // Wait for completion
    while (!APUOperationDone()) {}
}

//*****************************************************************************
//
// Enable the APU, initialize it and configure its memory mode.
//
//*****************************************************************************
void APUSetConfig(uint32_t memConfig)
{
    // Enable APU
    HWREG(VCE_BASE + VCE_O_ENABLE) = VCE_ENABLE_TOPSM_ONE;
    HWREG(VCE_BASE + VCE_O_INIT)   = VCE_INIT_TOPSM_ONE;
    // Ensure APU is in wait before calling API
    __asm volatile(" NOP");
    __asm volatile(" NOP");
    __asm volatile(" NOP");
    __asm volatile(" NOP");
    __asm volatile(" NOP");
    __asm volatile(" NOP");
    __asm volatile(" NOP");
    __asm volatile(" NOP");

    // Set memory mode: interleaving or mirrored
    if (memConfig == APU_MEMORY_INTERLEAVED)
    {
        HWREG(VCE_BASE + VCE_O_CMDPAR0) = VCE_LSECTL_MEMORY_INTERLEAVED;
    }
    else if (memConfig == APU_MEMORY_MIRRORED)
    {
        HWREG(VCE_BASE + VCE_O_CMDPAR0) = VCE_LSECTL_MEMORY_MIRRORED;
    }
    else
    {
        // Wrong config, set to default mirrored mode
        HWREG(VCE_BASE + VCE_O_CMDPAR0) = VCE_LSECTL_MEMORY_MIRRORED;
    }

    HWREG(VCE_BASE + VCE_O_API) = APU_API_CONFIG;
}

//*****************************************************************************
//
// Configure the APU to do nothing
//
//*****************************************************************************
void APUNop(void)
{
    HWREG(VCE_BASE + VCE_O_API) = APU_API_NOP;
}

//*****************************************************************************
//
// Configure the APU to calculate the scalar product (dot product) of two vectors
//
//*****************************************************************************
void APUVectorDot(uint16_t N, void *pInputA, void *pInputB, void *pResult)
{
    // Length of vectors
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = N;
    // Input vector A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    // Instruct APU not to do conjugate input B before dot product
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = 0;

    HWREG(VCE_BASE + VCE_O_API) = APU_API_DOTPROD;
}

//*****************************************************************************
//
// Configure the APU to calculate the scalar product (dot product) of two vectors,
// but vector B is conjugated.
//
//*****************************************************************************
void APUVectorDotConj(uint16_t N, void *pInputA, void *pInputB, void *pResult)
{
    // Length of vectors
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = N;
    // Input vector A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    // Instruct VCE to do conjugate input B before dot product
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = 1;

    HWREG(VCE_BASE + VCE_O_API) = APU_API_DOTPROD;
}

//*****************************************************************************
//
// Configure the APU to calculate the element by element multiplication of two vectors
//
//*****************************************************************************
void APUVectorMult(uint16_t N, void *pInputA, void *pInputB, void *pResult)
{
    // Length of vectors
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = N;
    // Input vector A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(VCE_BASE + VCE_O_API) = APU_API_VECTMULT;
}

//*****************************************************************************
//
// Configure the APU to calculate the element by element multiplication of two vectors,
// but the second vector is conjugated
//
//*****************************************************************************

void APUVectorMultConj(uint16_t N, void *pInputA, void *pInputB, void *pResult)
{

    // Length of vectors
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = N;
    // input vector A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // input vector B offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    // Using CMDPAR1 to conjugate
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = 1;

    HWREG(VCE_BASE + VCE_O_API) = APU_API_VECTMULT;
}

//*****************************************************************************
//
// Configure the APU to perform the sum of two vectors
//
//*****************************************************************************
void APUVectorSum(uint16_t N, void *pInputA, void *pInputB, void *pResult)
{
    // Length of vector
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = N;
    // Input vector A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(VCE_BASE + VCE_O_API) = APU_API_VECTSUM;
}

//*****************************************************************************
//
// Configure the APU to calculate the matrix multiplication of two matrices
//
//*****************************************************************************
void APUMatrixMult(uint16_t M, uint16_t N, uint16_t P, void *pInputA, void *pInputB, void *pResult)
{
    // Number of rows of matrix A
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = M;
    // Number of columns of matrix A
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = N;
    // Number of columns of matrix B
    HWREG(VCE_BASE + VCE_O_CMDPAR5) = P;
    // Input matrix A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(VCE_BASE + VCE_O_API) = APU_API_MATMATMULT;
}

//*****************************************************************************
//
// Configure the APU to calculate the hermitian matrix multiplication
//
//*****************************************************************************
void APUMatrixMultHerm(uint16_t M, void *pInputA, void *pInputB, void *pResult)
{
    // Number of rows of matrix A
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = M;
    // Number of columns of matrix A
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = M;
    // Number of columns of matrix B
    HWREG(VCE_BASE + VCE_O_CMDPAR5) = M;
    // Input matrix A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(VCE_BASE + VCE_O_API) = APU_API_HERMATRIXMULT;
}

//*****************************************************************************
//
// Configure the APU to calculate the symmetric matrix multiplication
//
//*****************************************************************************
void APUMatrixMultSym(uint16_t M, void *pInputA, void *pInputB, void *pResult)
{
    // Number of rows of matrix A
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = M;
    // Number of columns of matrix A
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = M;
    // Number of columns of matrix B
    HWREG(VCE_BASE + VCE_O_CMDPAR5) = M;
    // Input matrix A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(VCE_BASE + VCE_O_API) = APU_API_SYMMATRIXMULT;
}

//*****************************************************************************
//
// Configure the APU to calculate the matrix sum
//
//*****************************************************************************
void APUMatrixSum(uint16_t M, uint16_t N, void *pInputA, void *pInputB, void *pResult)
{
    // Number of rows of matrix A and B
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = M;
    // Number of columns of matrix A and B
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = N;
    // Input matrix A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(VCE_BASE + VCE_O_API) = APU_API_MATRIXSUM;
}

//*****************************************************************************
//
// Configure the APU to calculate the multiplication of a scalar and a matrix
//
//*****************************************************************************
void APUMatrixScalarMult(uint16_t M, uint16_t N, void *pInputA, void *pInputB, void *pResult)
{
    // Number of rows of matrix A and B
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = M;
    // Number of columns of matrix A and B
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = N;
    // Input matrix A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(VCE_BASE + VCE_O_API) = APU_API_SCALARMULT;
}

//*****************************************************************************
//
// Configure the APU to calculate the sum of a scalar and a matrix
//
//*****************************************************************************
void APUMatrixScalarSum(uint16_t M, uint16_t N, void *pInputA, void *pInputB, void *pResult)
{
    // Number of rows of matrix A and B
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = M;
    // Number of columns of matrix A and B
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = N;
    // Input matrix A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(VCE_BASE + VCE_O_API) = APU_API_MATRIXSCALARSUM;
}

//*****************************************************************************
//
// Configure the APU to convert a vector from cartesian to polar
//
//*****************************************************************************
void APUVectorCart2Pol(uint16_t N, void *pInput, void *pResult)
{
    // Length of vector
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = N;
    // Input cartesian value A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);
    // Result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(VCE_BASE + VCE_O_API) = APU_API_POLAR;
}

//*****************************************************************************
//
// Configure the APU to convert a vector from polar to cartesian
//
//*****************************************************************************
void APUVectorPol2Cart(uint16_t N, void *pInput, void *pResult, void *pTemp)
{
    // Length of vector
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = N;
    // Input polar value A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);
    // Temp space offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pTemp);
    // Result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(VCE_BASE + VCE_O_API) = APU_API_CARTESIAN;
}

//*****************************************************************************
//
// Configure the APU to sort a vector in descending order
//
//*****************************************************************************
void APUVectorSort(uint16_t N, void *pInput)
{
    // Length of vector
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = N;
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = 1; // Max to min

    // Input vector A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);

    HWREG(VCE_BASE + VCE_O_API) = APU_API_SORT;
}

//*****************************************************************************
//
// Configure the APU to perform spatial smoothing
//
//*****************************************************************************
void APUSpSmoothCovMatrix(uint16_t N, void *pInput, uint16_t L, void *pResult, uint16_t fb)
{
    // Vector size N of pInput
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = N;
    // Matrix output size L
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = L;
    // Input vector A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);
    // Result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pResult);
    // Option: forward-backword averaging
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = fb;

    // HWREG(VCE_BASE + VCE_O_CMDPAR) = length;
    HWREG(VCE_BASE + VCE_O_API) = APU_API_COVMATRIX;
}

//*****************************************************************************
//
// Configure the APU to calculate jacobi eigenvalue decomposition of
// a hermitian matrix
//
//*****************************************************************************
void APUJacobiEVD(uint16_t N, void *pInput, void *pResultV, uint16_t maxIter, float minSum)
{
    // Matrix size N of input Hermitian matrix
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = N;
    // Maximum number of iterations
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = maxIter;
    // input matrix A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);
    // V result offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResultV);
    // Always save min_sum at: heap_base + 7
    volatile float *mem_ptr         = (float *)(APU_GET_DATA_MEM_ABS(APU_HEAP_ADDR + 7));
    *mem_ptr                        = minSum; // real part
    __asm volatile(" NOP");
    __asm volatile(" NOP");
    __asm volatile(" NOP");
    __asm volatile(" NOP");
    *(mem_ptr + 1) = 0; // imaginary part

    // Start APU
    HWREG(VCE_BASE + VCE_O_API) = APU_API_EIGEN;
}

//*****************************************************************************
//
// Configure the APU to perform Gauss-Jordan Elimination on a matrix
//
//*****************************************************************************
void APUGaussJordanElim(uint16_t M, uint16_t N, void *pInput, float epsTol)
{
    // Number of rows of input/output matrices
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = M;
    // Number of columns of input/output matrices
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = N;
    // Input matrix A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);
    // Threshold epsilon at heap addres + 1
    volatile float *mem_ptr;
    mem_ptr  = (float *)(APU_GET_DATA_MEM_ABS((APU_HEAP_ADDR + 1)));
    *mem_ptr = epsTol; // real part
    __asm volatile(" NOP");
    __asm volatile(" NOP");
    __asm volatile(" NOP");
    __asm volatile(" NOP");
    *(mem_ptr + 1) = 0; // imaginary part

    // Start APU
    HWREG(VCE_BASE + VCE_O_API) = APU_API_GAUSS;
}

//*****************************************************************************
//
// Configure the APU to calculate the matrix norm
//
//*****************************************************************************
void APUMatrixNorm(uint16_t M, uint16_t N, void *pInput, void *pResult)
{
    // Number of rows of input matrix
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = M;
    // Number of columns of input matrix
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = N;
    // Input matrix A offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);
    // Output matrix offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pResult);

    // Start APU
    HWREG(VCE_BASE + VCE_O_API) = APU_API_MATRIXNORM;
}

//*****************************************************************************
//
// Configure the APU to calculate the Fast Fourier Transform
//
//*****************************************************************************
void APUComputeFft(uint16_t N, void *pX)
{
    // Length of vector
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = N;
    // FFT = 0 , FFT CFG = 1 or IFFT = 2
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = 0;
    // Input vector offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pX);

    // Start APU
    HWREG(VCE_BASE + VCE_O_API) = APU_API_FFT;
}

//*****************************************************************************
//
// Configure the APU to calculate the Inverse Fast Fourier Transform
//
//*****************************************************************************
void APUComputeIfft(uint16_t N, void *pX)
{
    // Length of vector
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = N;
    // FFT = 0 , FFT CFG = 1 or IFFT = 2
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = 2;
    // Input vector offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pX);

    // Start APU
    HWREG(VCE_BASE + VCE_O_API) = APU_API_FFT;
}

//*****************************************************************************
//
// Configure the APU to calculate the Fast Fourier Transform memory config
//
//*****************************************************************************
void APUConfigFft(uint16_t N, void *pX)
{
    // Length of vector
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = N;
    // FFT = 0 , FFT CFG = 1 or IFFT = 2
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = 1;
    // Input vector offset in memory
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pX);

    // Start APU
    HWREG(VCE_BASE + VCE_O_API) = APU_API_FFT;
}

//*****************************************************************************
//
// Configure the APU to output a unit circle
//
//*****************************************************************************
void APUUnitCircle(uint16_t N, uint16_t M, uint16_t phase, uint16_t conj, void *pResult)
{
    // Unit circle size
    HWREG(VCE_BASE + VCE_O_CMDPAR0) = N;
    // Constant M
    HWREG(VCE_BASE + VCE_O_CMDPAR1) = M;
    // Phase
    HWREG(VCE_BASE + VCE_O_CMDPAR2) = phase;
    // Conj
    HWREG(VCE_BASE + VCE_O_CMDPAR3) = conj;
    // Result
    HWREG(VCE_BASE + VCE_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    // Start APU
    HWREG(VCE_BASE + VCE_O_API) = APU_API_UNITCIRC;
}
