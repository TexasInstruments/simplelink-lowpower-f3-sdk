/******************************************************************************
 *  Filename:       apu.c
 *
 *  Description:    Driver for the APU peripheral.
 *
 *  Copyright (c) 2024-2025 Texas Instruments Incorporated
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
#include "../inc/hw_apu.h"
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
    APU_API_R2C               = 0x0012,
    APU_API_MATRIXNORM        = 0x0013,
    APU_API_FFT               = 0x0014,
    APU_API_DCT               = 0x0015,
    APU_API_SORT              = 0x0016,
    APU_API_GAUSS             = 0x0017,
    APU_API_HERMLO            = 0x0018,
    APU_API_MAXMIN            = 0x0019,
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
    while (!(HWREG(APU_BASE + APU_O_RIS) & APU_RIS_API_M)) {}
    // Clear RIS API
    HWREG(APU_BASE + APU_O_ICLR) = APU_ICLR_API_YES;
}

//*****************************************************************************
//
// Checks the APU message box to see if it has completed its operation.
//
//*****************************************************************************
bool APUOperationDone(void)
{
    return ((HWREG(APU_BASE + APU_O_MSGBOX) & APU_MSGBOX_CMDOK) == APU_MSGBOX_CMDOK);
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
    HWREG(APU_BASE + APU_O_ENABLE) = APU_ENABLE_TOPSM_ONE;
    HWREG(APU_BASE + APU_O_INIT)   = APU_INIT_TOPSM_ONE;
    // Ensure APU is in wait before calling API
    // Prevent back-to-back-writes
    ASM_4_NOPS();
    // Prevent back-to-back-writes
    ASM_4_NOPS();

    // Set memory mode: interleaving or mirrored
    if (memConfig == APU_MEMORY_INTERLEAVED)
    {
        HWREG(APU_BASE + APU_O_CMDPAR0) = APU_LSECTL_MEMORY_INTERLEAVED;
    }
    else if (memConfig == APU_MEMORY_MIRRORED)
    {
        HWREG(APU_BASE + APU_O_CMDPAR0) = APU_LSECTL_MEMORY_MIRRORED;
    }
    else
    {
        // Wrong config, set to default mirrored mode
        HWREG(APU_BASE + APU_O_CMDPAR0) = APU_LSECTL_MEMORY_MIRRORED;
    }

    HWREG(APU_BASE + APU_O_API) = APU_API_CONFIG;
}

//*****************************************************************************
//
// Configure the APU to do nothing
//
//*****************************************************************************
void APUNop(void)
{
    HWREG(APU_BASE + APU_O_API) = APU_API_NOP;
}

//*****************************************************************************
//
// Configure the APU to calculate the scalar product (dot product) of two
// vectors
//
//*****************************************************************************
void APUVectorDot(uint16_t N, void *pInputA, void *pInputB, void *pResult)
{
    // Length of vectors
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // Input vector A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    // Instruct APU not to do conjugate input B before dot product
    HWREG(APU_BASE + APU_O_CMDPAR1) = 0;

    HWREG(APU_BASE + APU_O_API) = APU_API_DOTPROD;
}

//*****************************************************************************
//
// Configure the APU to calculate the scalar product (dot product) of two
// vectors, but vector B is conjugated.
//
//*****************************************************************************
void APUVectorDotConj(uint16_t N, void *pInputA, void *pInputB, void *pResult)
{
    // Length of vectors
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // Input vector A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    // Instruct APU to do conjugate input B before dot product
    HWREG(APU_BASE + APU_O_CMDPAR1) = 1;

    HWREG(APU_BASE + APU_O_API) = APU_API_DOTPROD;
}

//*****************************************************************************
//
// Configure the APU to calculate the element by element multiplication of two
// vectors
//
//*****************************************************************************
void APUVectorMult(uint16_t N, void *pInputA, void *pInputB, void *pResult)
{
    // Length of vectors
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // Input vector A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);
    // Increment for pInputB, increment 1 indicates vector and increment 0
    // Indicates scalar
    HWREG(APU_BASE + APU_O_CMDPAR5) = 1;

    // Using CMDPAR1 to conjugate
    HWREG(APU_BASE + APU_O_CMDPAR1) = 0;

    HWREG(APU_BASE + APU_O_API) = APU_API_VECTMULT;
}

//*****************************************************************************
//
// Configure the APU to calculate the multiplication of a vector and a scalar
//
//*****************************************************************************
void APUVectorScalarMult(uint16_t N, void *pInputA, void *pInputB, void *pResult)
{
    // Length of vectors
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // Input vector A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);
    // Increment for pInputB, increment 1 indicates vector and increment 0
    // indicates scalar
    HWREG(APU_BASE + APU_O_CMDPAR5) = 0;

    // Using CMDPAR1 to conjugate
    HWREG(APU_BASE + APU_O_CMDPAR1) = 0;

    HWREG(APU_BASE + APU_O_API) = APU_API_VECTMULT;
}

//*****************************************************************************
//
// Configure the APU to calculate the element by element multiplication of two
// vectors, but the second vector is conjugated
//
//*****************************************************************************

void APUVectorMultConj(uint16_t N, void *pInputA, void *pInputB, void *pResult)
{

    // Length of vectors
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // Input vector A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);
    // Increment for pInputB, 1 for vector and 0 for scalar
    HWREG(APU_BASE + APU_O_CMDPAR5) = 1;

    // Using CMDPAR1 to conjugate
    HWREG(APU_BASE + APU_O_CMDPAR1) = 1;

    HWREG(APU_BASE + APU_O_API) = APU_API_VECTMULT;
}

//*****************************************************************************
//
// Configure the APU to perform the sum of two vectors
//
//*****************************************************************************
void APUVectorSum(uint16_t N, void *pInputA, void *pInputB, uint16_t op, void *pResult)
{
    // Length of vector
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // op (ADDSUB_OP_ADD (0), ADDSUB_OP_SUB (1))
    HWREG(APU_BASE + APU_O_CMDPAR1) = op;
    // Input vector A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);
    // increment of vector B (1 for vector + vector, 0 for vector + scalar)
    HWREG(APU_BASE + APU_O_CMDPAR5) = 1;

    HWREG(APU_BASE + APU_O_API) = APU_API_VECTSUM;
}

//*****************************************************************************
//
// Configure the APU to perform the sum of a vector and a scalar
//
//*****************************************************************************
void APUVectorScalarSum(uint16_t N, void *pInputA, void *pInputB, uint16_t op, void *pResult)
{
    // Length of vector
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // op (ADDSUB_OP_ADD (0), ADDSUB_OP_SUB (1))
    HWREG(APU_BASE + APU_O_CMDPAR1) = op;
    // Input vector A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);
    // Increment of vector B (1 for vector + vector, 0 for vector + scalar)
    HWREG(APU_BASE + APU_O_CMDPAR5) = 0;

    HWREG(APU_BASE + APU_O_API) = APU_API_VECTSUM;
}

//*****************************************************************************
//
// Configure the APU to calculate the matrix multiplication of two matrices
//
//*****************************************************************************
void APUMatrixMult(uint16_t M, uint16_t N, uint16_t P, void *pInputA, void *pInputB, void *pResult)
{
    // Number of rows of matrix A
    HWREG(APU_BASE + APU_O_CMDPAR0) = M;
    // Number of columns of matrix A
    HWREG(APU_BASE + APU_O_CMDPAR1) = N;
    // Number of columns of matrix B
    HWREG(APU_BASE + APU_O_CMDPAR5) = P;
    // Input matrix A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(APU_BASE + APU_O_API) = APU_API_MATMATMULT;
}

//*****************************************************************************
//
// Configure the APU to calculate the hermitian matrix multiplication
//
//*****************************************************************************
void APUMatrixMultHerm(uint16_t M, void *pInputA, void *pInputB, void *pResult)
{
    // Number of rows of matrix A
    HWREG(APU_BASE + APU_O_CMDPAR0) = M;
    // Number of columns of matrix A
    HWREG(APU_BASE + APU_O_CMDPAR1) = M;
    // Number of columns of matrix B
    HWREG(APU_BASE + APU_O_CMDPAR5) = M;
    // Input matrix A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(APU_BASE + APU_O_API) = APU_API_HERMATRIXMULT;
}

//*****************************************************************************
//
// Configure the APU to calculate the symmetric matrix multiplication
//
//*****************************************************************************
void APUMatrixMultSym(uint16_t M, void *pInputA, void *pInputB, void *pResult)
{
    // Number of rows of matrix A
    HWREG(APU_BASE + APU_O_CMDPAR0) = M;
    // Number of columns of matrix A
    HWREG(APU_BASE + APU_O_CMDPAR1) = M;
    // Number of columns of matrix B
    HWREG(APU_BASE + APU_O_CMDPAR5) = M;
    // Input matrix A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(APU_BASE + APU_O_API) = APU_API_SYMMATRIXMULT;
}

//*****************************************************************************
//
// Configure the APU to calculate the matrix sum
//
//*****************************************************************************
void APUMatrixSum(uint16_t M, uint16_t N, void *pInputA, void *pInputB, void *pResult)
{
    // Number of rows of matrix A and B
    HWREG(APU_BASE + APU_O_CMDPAR0) = M;
    // Number of columns of matrix A and B
    HWREG(APU_BASE + APU_O_CMDPAR1) = N;
    // Input matrix A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // Result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(APU_BASE + APU_O_API) = APU_API_MATRIXSUM;
}

//*****************************************************************************
//
// Configure the APU to calculate the multiplication of a scalar and a matrix
//
//*****************************************************************************
void APUMatrixScalarMult(uint16_t M, uint16_t N, void *pInputA, void *pInputB, void *pResult)
{
    // Matrix[M][N] is stored as vector[MxN]
    APUVectorScalarMult(M * N, pInputA, pInputB, pResult);
}

//*****************************************************************************
//
// Configure the APU to calculate the sum of a scalar and a matrix
//
//*****************************************************************************
void APUMatrixScalarSum(uint16_t M, uint16_t N, void *pInputA, void *pInputB, void *pResult)
{
    // Matrix[M][N] is stored as vector[MxN]
    APUVectorScalarSum(M * N, pInputA, pInputB, 0, pResult);
}

//*****************************************************************************
//
// Configure the APU to convert a vector from cartesian to polar
//
//*****************************************************************************
void APUVectorCart2Pol(uint16_t N, void *pInput, void *pResult)
{
    // Length of vector
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // Input cartesian value A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);
    // Result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(APU_BASE + APU_O_API) = APU_API_POLAR;
}

//*****************************************************************************
//
// Configure the APU to convert a vector from polar to cartesian
//
//*****************************************************************************
void APUVectorPol2Cart(uint16_t N, void *pInput, void *pResult, void *pTemp)
{
    // Length of vector
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // Input polar value A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);
    // Temp space offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pTemp);
    // Result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    HWREG(APU_BASE + APU_O_API) = APU_API_CARTESIAN;
}

//*****************************************************************************
//
// Configure the APU to sort a vector in descending order
//
//*****************************************************************************
void APUVectorSort(uint16_t N, void *pInput)
{
    // Length of vector
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    HWREG(APU_BASE + APU_O_CMDPAR1) = 1; // Max to min

    // Input vector A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);

    HWREG(APU_BASE + APU_O_API) = APU_API_SORT;
}

//*****************************************************************************
//
// Configure the APU to perform spatial smoothing
//
//*****************************************************************************
void APUSpSmoothCovMatrix(uint16_t N, void *pInput, uint16_t L, void *pResult, uint16_t fb)
{
    // Vector size N of pInput
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // Matrix output size L
    HWREG(APU_BASE + APU_O_CMDPAR1) = L;
    // Input vector A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);
    // Result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pResult);
    // Option: forward-backword averaging
    HWREG(APU_BASE + APU_O_CMDPAR4) = fb;

    // HWREG(APU_BASE + APU_O_CMDPAR) = length;
    HWREG(APU_BASE + APU_O_API) = APU_API_COVMATRIX;
}

//*****************************************************************************
//
// Configure the APU to calculate jacobi eigenvalue decomposition of
// a hermitian matrix
//
//*****************************************************************************
void APUJacobiEVD(uint16_t N, void *pInput, void *pResultV, uint16_t maxIter, float minSum, float epsTol)
{
    // Matrix size N of input Hermitian matrix
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // Maximum number of iterations
    HWREG(APU_BASE + APU_O_CMDPAR1) = maxIter;
    // Input matrix A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);
    // V result offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResultV);
    // Always save min_sum at: heap_base + 7
    volatile float *mem_ptr         = (float *)(APU_GET_DATA_MEM_ABS(APU_HEAP_ADDR + 7));
    *mem_ptr                        = minSum; // real part
    // Prevent back-to-back-writes
    ASM_4_NOPS();
    // Imaginary part
    *(mem_ptr + 1) = 0;
    // Prevent back-to-back-writes
    ASM_4_NOPS();
    volatile float *mem_ptr_eps = (float *)(APU_GET_DATA_MEM_ABS(APU_HEAP_ADDR + 30));
    // Real part
    *mem_ptr_eps                = epsTol;
    // Prevent back-to-back-writes
    ASM_4_NOPS();
    // Imaginary part
    *(mem_ptr_eps + 1) = 0;

    // Start APU
    HWREG(APU_BASE + APU_O_API) = APU_API_EIGEN;
}

//*****************************************************************************
//
// Configure the APU to perform Gauss-Jordan Elimination on a matrix
//
//*****************************************************************************
void APUGaussJordanElim(uint16_t M, uint16_t N, void *pInput, float epsTol)
{
    // Number of rows of input/output matrices
    HWREG(APU_BASE + APU_O_CMDPAR0) = M;
    // Number of columns of input/output matrices
    HWREG(APU_BASE + APU_O_CMDPAR1) = N;
    // Input matrix A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);
    // Threshold epsilon at heap addres + 1
    volatile float *mem_ptr;
    mem_ptr  = (float *)(APU_GET_DATA_MEM_ABS((APU_HEAP_ADDR + 1)));
    *mem_ptr = epsTol; // real part
    // Prevent back-to-back-writes
    ASM_4_NOPS();
    // Imaginary part
    *(mem_ptr + 1) = 0;

    // Start APU
    HWREG(APU_BASE + APU_O_API) = APU_API_GAUSS;
}

//*****************************************************************************
//
// Configure the APU to calculate the matrix norm
//
//*****************************************************************************
void APUMatrixNorm(uint16_t M, uint16_t N, void *pInput, void *pResult)
{
    // Number of rows of input matrix
    HWREG(APU_BASE + APU_O_CMDPAR0) = M;
    // Number of columns of input matrix
    HWREG(APU_BASE + APU_O_CMDPAR1) = N;
    // Input matrix A offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInput);
    // Output matrix offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR3) = APU_GET_DATA_MEM_OFFSET(pResult);

    // Start APU
    HWREG(APU_BASE + APU_O_API) = APU_API_MATRIXNORM;
}

//*****************************************************************************
//
// Configure the APU to calculate the Fast Fourier Transform
//
//*****************************************************************************
void APUComputeFft(uint16_t N, void *pX)
{
    // Length of vector
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // FFT = 0 , FFT CFG = 1 or IFFT = 2
    HWREG(APU_BASE + APU_O_CMDPAR1) = 0;
    // Input vector offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pX);

    // Start APU
    HWREG(APU_BASE + APU_O_API) = APU_API_FFT;
}

//*****************************************************************************
//
// Configure the APU to calculate the Inverse Fast Fourier Transform
//
//*****************************************************************************
void APUComputeIfft(uint16_t N, void *pX)
{
    // Length of vector
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // FFT = 0 , FFT CFG = 1 or IFFT = 2
    HWREG(APU_BASE + APU_O_CMDPAR1) = 2;
    // Input vector offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pX);

    // Start APU
    HWREG(APU_BASE + APU_O_API) = APU_API_FFT;
}

//*****************************************************************************
//
// Configure the APU to calculate the Fast Fourier Transform memory config
//
//*****************************************************************************
void APUConfigFft(uint16_t N, void *pX)
{
    // Length of vector
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // FFT = 0 , FFT CFG = 1 or IFFT = 2
    HWREG(APU_BASE + APU_O_CMDPAR1) = 1;
    // Input vector offset in memory
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pX);

    // Start APU
    HWREG(APU_BASE + APU_O_API) = APU_API_FFT;
}

//*****************************************************************************
//
// Configure the APU to output a unit circle
//
//*****************************************************************************
void APUUnitCircle(uint16_t N, uint16_t M, uint16_t phase, uint16_t conj, void *pResult)
{
    // Unit circle size
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // Constant M
    HWREG(APU_BASE + APU_O_CMDPAR1) = M;
    // Phase
    HWREG(APU_BASE + APU_O_CMDPAR2) = phase;
    // Conj
    HWREG(APU_BASE + APU_O_CMDPAR3) = conj;
    // Result
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    // Start APU
    HWREG(APU_BASE + APU_O_API) = APU_API_UNITCIRC;
}
//*****************************************************************************
//
// Configure the APU to compute max/min of a vector and a real value scalar
//
//*****************************************************************************
void APUVectorMaxMin(uint16_t N, void *pInput, float thresh, uint16_t op, void *pResult)
{
    // Vector Length
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // Input vector
    HWREG(APU_BASE + APU_O_CMDPAR1) = APU_GET_DATA_MEM_OFFSET(pInput);

    // Threshold epsilon at heap addres + 1
    volatile float *mem_ptr = (float *)(APU_GET_DATA_MEM_ABS((APU_HEAP_ADDR + 1)));
    *mem_ptr                = thresh; // real part
    // Prevent back-to-back-writes
    ASM_4_NOPS();
    *(mem_ptr + 1)                  = 0; // imaginary part
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(mem_ptr);

    // op for max (APU_MAXMIN_OP_MAX) or min (APU_MAXMIN_OP_MIN)
    HWREG(APU_BASE + APU_O_CMDPAR3) = op;
    // Result
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    // Start APU
    HWREG(APU_BASE + APU_O_API) = APU_API_MAXMIN;
}

//*****************************************************************************
//
// Configure the APU to convert real vector to complex vector and vice versa
//
//*****************************************************************************
void APUVectorR2C(uint16_t N, void *pInputA, void *pInputB, uint16_t op, void *pResult)
{
    // Vector length
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // Input vector A
    HWREG(APU_BASE + APU_O_CMDPAR1) = APU_GET_DATA_MEM_OFFSET(pInputA);
    // Input vector B
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pInputB);
    // op for R2C operation
    HWREG(APU_BASE + APU_O_CMDPAR3) = op;
    // Result
    HWREG(APU_BASE + APU_O_CMDPAR4) = APU_GET_DATA_MEM_OFFSET(pResult);

    // Start APU
    HWREG(APU_BASE + APU_O_API) = APU_API_R2C;
}

//*****************************************************************************
//
// Configure the APU to convert Hermitian upper-triangular to lower-triangular
//
//*****************************************************************************
void APUHermLo(uint16_t N, void *pInput, void *pResult)
{
    // Unit circle size
    HWREG(APU_BASE + APU_O_CMDPAR0) = N;
    // Input vector A
    HWREG(APU_BASE + APU_O_CMDPAR1) = APU_GET_DATA_MEM_OFFSET(pInput);
    // Input vector B
    HWREG(APU_BASE + APU_O_CMDPAR2) = APU_GET_DATA_MEM_OFFSET(pResult);

    // Start APU
    HWREG(APU_BASE + APU_O_API) = APU_API_HERMLO;
}