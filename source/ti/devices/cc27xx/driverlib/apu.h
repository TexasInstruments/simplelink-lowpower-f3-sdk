
/******************************************************************************
 *  Filename:       apu.h
 *
 *  Description:    Defines and prototypes for the APU peripheral.
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
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER I25N
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#ifndef __APU_H__
#define __APU_H__

//*****************************************************************************
//
//! \addtogroup peripheral_group
//! @{
//! \addtogroup apu_api
//! @{
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

//****************************************************************************
// Generic Definition
//****************************************************************************
#define APU_FBA_ENABLE         1 //!< Enable Forward/Backward Averaging
#define APU_FBA_DISABLE        0 //!< Disable Forward/Backward Averaging
#define APU_MEMORY_INTERLEAVED 1 //!< Set APU memory in interleaved mode
#define APU_MEMORY_MIRRORED    0 //!< Set APU memory in mirrored mode

// Macro to convert absolute-address to offset-address used by APU
#define APU_GET_DATA_MEM_OFFSET(x) ((uint32_t)x - (uint32_t)VCERAM_DATA0_BASE) >> 3
// Macro to convert offset-address used by APU to absolute-address
#define APU_GET_DATA_MEM_ABS(x)    ((uint32_t)VCERAM_DATA0_BASE + (((uint32_t)x) << 3))

//*****************************************************************************
//
// API Functions and prototypes
//
//*****************************************************************************

//****************************************************************************
// Generic Utils
//****************************************************************************

//*****************************************************************************
//
//! \brief Wait for the APU interrupt.
//!
//! This function busy-waits until the APU API interrupt is set.
//!
//! \return None
//
//*****************************************************************************
void APUWaitOnIrq(void);

//*****************************************************************************
//
//! \brief Check if the APU has completed its operation.
//!
//! Checks the APU message box to see if it has completed its operation.
//!
//! \return None
//
//*****************************************************************************
bool APUOperationDone(void);

//*****************************************************************************
//
//! \brief Wait for the APU to start and finish.
//!
//! Wait for the APU to both begin and complete its operation.
//!
//! \return None
//
//*****************************************************************************
void APUWaitOp(void);

//*****************************************************************************
//
//! \brief Configure the APU.
//!
//! Enable the APU, initialize it and configure its memory mode.
//!
//! \param memConfig is the memory configuration mode, either mirrored or
//! interleaved. Must be one of the following:
//!   - \ref APU_MEMORY_INTERLEAVED
//!   - \ref APU_MEMORY_MIRRORED
//!
//! \return None
//
//*****************************************************************************
void APUSetConfig(uint32_t memConfig);

//*****************************************************************************
//
//! \brief APU NOP.
//!
//! Configure the APU to do nothing.
//!
//! \return None
//
//*****************************************************************************
void APUNop(void);

//****************************************************************************
// Generic Linear Algebra
//****************************************************************************

//*****************************************************************************
//
//! \addtogroup apu_api_vector_operations
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief APU accelerator for vector dot product c = A dot B
//!
//! Calculate the scalar product (dot product/inner product) of two vectors.
//!
//! Defined as:
//! \code
//!  c = A dot B = sum(A[i] * B[i]), i = 0 to N-1
//! \endcode
//! in which, A, B are complex vectors, c is a complex scalar
//!
//! \param N is the size of the input vectors. Both vectors must be the same size
//! \param pInputA a pointer to the base of the first input vector, in APU memory
//! \param pInputB a pointer to the base of the second input vector, in APU memory
//! \param pResult a pointer to where the result will be placed, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUVectorDot(uint16_t N, void *pInputA, void *pInputB, void *pResult);

//*****************************************************************************
//
//! \brief APU accelerator for vector dot product c = A dot conj(B)
//!
//! Calculate the scalar product (dot product/inner product) of vector A and conjugate
//! of vector B.
//!
//! Defined as:
//! \code
//!    c = A dot conj(B) = sum(A[i] * conj(B[i])), i = 0 to N-1
//! \endcode
//! in which, A, B are complex vectors, c is a complex scalar
//!
//! \param N is the size of the input vectors. Both vectors must be the same size
//! \param pInputA a pointer to the base of the first input vector, in APU memory
//! \param pInputB a pointer to the base of the second input vector, in APU memory
//! \param pResult a pointer to where the result will be placed, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUVectorDotConj(uint16_t N, void *pInputA, void *pInputB, void *pResult);

//*****************************************************************************
//
//! \brief APU accelerator for element-wise product of two vectors
//!
//! Calculate the element-wise product (also called Hadamard product) of
//! two vectors.
//!
//! Defined as:
//! \code
//!    C = A .* B = [A[0] * B[0], A[1] * B[1], .., A[N-1] * B[N-1]]
//! \endcode
//! in which, A, B and C are complex vectors
//!
//! \param N is the size of the input vectors. Both vectors must be the same size
//! \param pInputA a pointer to the base of the first input vector, in APU memory
//! \param pInputB a pointer to the base of the second input vector, in APU memory
//! \param pResult a pointer to where the result will be placed, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUVectorMult(uint16_t N, void *pInputA, void *pInputB, void *pResult);

//*****************************************************************************
//
//! \brief APU accelerator for element-wise product of vector A with the
//!  conjugate of vector B.
//!
//! Defined as:
//! \code
//!    C = A .* conj(B) = [A[0] * conj(B[0]), A[1] * conj(B[1]), .., A[N-1] * conj(B[N-1])]
//! \endcode
//! in which, A, B and C are complex vectors
//!
//! \param N is the size of the input vectors. Both vectors must be the same size
//! \param pInputA a pointer to the base of the first input vector, in APU memory
//! \param pInputB a pointer to the base of the second input vector, in APU memory
//! \param pResult a pointer to where the result will be placed, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUVectorMultConj(uint16_t N, void *pInputA, void *pInputB, void *pResult);

//*****************************************************************************
//
//! \brief APU accelerator for addition of two vectors
//!
//! Calculate vector addition of two vectors.
//!
//! Defined as:
//! \code
//!    C = A + B = [A[0] + B[0], A[1] + B[1], .., A[N-1] + B[N-1]]
//! \endcode
//! in which, A, B and C are complex vectors
//!
//! \param N is the size of the input vectors. Both vectors must be the same size
//! \param pInputA a pointer to the base of the first input vector, in APU memory
//! \param pInputB a pointer to the base of the second input vector, in APU memory
//! \param pResult a pointer to where the result will be placed, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUVectorSum(uint16_t N, void *pInputA, void *pInputB, void *pResult);

//*****************************************************************************
//
//! \brief APU accelerator for element-wise Cartesian-to-Polar transformation of
//! a vector
//!
//! Calculate element-wise Cartesian-to-Polar transformation.
//!
//! Defined as:
//! \code
//!  C[i] = cartesian_to_polar(A[i]), i = 0 to N-1
//! \endcode
//! in which A, C are two complex vectors.
//!
//! \note The elements of input vector A are assumed in Cartesian
//! representation. The elements of output vector C are in Polar representation.
//!
//! \param N length of the vector
//! \param pInput a pointer to the base of the first input vector A, in APU memory
//! \param pResult a pointer to where the result will be placed, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUVectorCart2Pol(uint16_t N, void *pInput, void *pResult);

//*****************************************************************************
//
//! \brief APU accelerator for element-wise Polar-to-Cartesian transformation of
//! a vector
//!
//! Calculate element-wise Polar-to-Cartesian transformation.
//!
//! Defined as:
//! \code
//!  C[i] = polar_to_cartesian(A[i]), i = 0 to N-1
//! \endcode
//! //! in which A, C are two complex vectors.
//!
//! \note The elements of input vector A are assumed in Polar representation.
//! The elements of output vector C are in Cartesian representation. This API
//! requires a temporary vector pTemp of length N to store temporary results.
//! Instead of using fixed and pre-defined allocation, this API allows users
//! flexibly select the location of this temporary vector for a better
//! utilization of available memory.
//!
//! \param N length of the vector
//! \param pInput a pointer to the base of the first input vector A, in APU memory
//! \param pResult a pointer to where the result will be placed, in APU memory
//! \param pTemp a pointer to a temporary vector of length N in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUVectorPol2Cart(uint16_t N, void *pInput, void *pResult, void *pTemp);

//*****************************************************************************
//
//! \brief APU accelerator for in-place sorting of a vector
//!
//! Sort (in-place) an input vector in descending order based on the real
//! component of each element.
//!
//! Defined as:
//! \code
//!    C = sort(A, "descend", "ComparisonMethod", "real")
//! \endcode
//! in which A, C are two complex vectors.
//!
//! \note This is an in-place sorting, which means the input vector will be
//! overwritten by its sorted version.
//!
//! \param N length of the vector A/C
//! \param pInput a pointer to the base of the input vector A, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUVectorSort(uint16_t N, void *pInput);

//*****************************************************************************
//
//! Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup apu_api_matrix_operations
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief APU accelerator for matrix multiplication of two matrices C = A*B
//!
//! Calculate matrix multiplication of two matrices.
//!
//! Defined as:
//! \code
//!    C[MxP] = A[MxN] * B[NxP]
//! \endcode
//! in which A, B, and C are complex matrices with size [MxP], [MxN], and [NxP]
//! respectively.
//!
//! \param M         number of rows of matrix A
//! \param N         number of columns of matrix A (number of rows of matrix B)
//! \param P         number of columns of matrix B
//! \param pInputA a pointer to the base of the first input matrix A, in APU memory
//! \param pInputB a pointer to the base of the second input matrix B, in APU memory
//! \param pResult a pointer to where the result will be placed, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUMatrixMult(uint16_t M, uint16_t N, uint16_t P, void *pInputA, void *pInputB, void *pResult);

//*****************************************************************************
//
//! \brief APU accelerator for matrix multiplication of two Hermitian matrices C = A*B
//!
//! Calculate matrix multiplication of two Hermitian matrices.
//!
//! Defined as:
//! \code
//!    C[MxM] = A[MxM] * B[MxM]
//! \endcode
//! in which A, B, and C are Hermitian matrices with the same size [MxM]
//!
//! \note APU stores only upper/right triangular part of Hermitian matrices for memory
//! saving. A Hermitian matrix (MxM) will take M*(M+1)/2 element spaces
//!
//! \param M         number of rows and columns of matrix A/B/C
//! \param pInputA a pointer to the base of the first input matrix A, in APU memory
//! \param pInputB a pointer to the base of the second input matrix B, in APU memory
//! \param pResult a pointer to where the result will be placed, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUMatrixMultHerm(uint16_t M, void *pInputA, void *pInputB, void *pResult);

//*****************************************************************************
//
//! \brief APU accelerator for matrix multiplication of two symmetric matrices
//! C = A*B
//!
//! Calculate matrix multiplication of two symmetric matrices.
//!
//! Defined as:
//! \code
//!    C[MxM] = A[MxM] * B[MxM]
//! \endcode
//! in which A, B are symmetric matrices with the same size [MxM]
//!
//! \note APU stores only upper/right triangular part of symmetric matrices for
//! memory saving. A symmetric matrix (MxM) will take M*(M+1)/2 element spaces
//!
//! \param M         number of rows and columns of matrix A/B/C
//! \param pInputA a pointer to the base of the first input matrix A, in APU memory
//! \param pInputB a pointer to the base of the second input matrix B, in APU memory
//! \param pResult a pointer to where the result will be placed, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUMatrixMultSym(uint16_t M, void *pInputA, void *pInputB, void *pResult);

//*****************************************************************************
//
//! \brief APU accelerator for matrix addition of two matrices C = A+B
//!
//! Calculate matrix addition of two matrices.
//!
//! Defined as:
//! \code
//!    C[MxN] = A[MxN] + B[MxN]
//! \endcode
//! in which, A, B, and C are matrices with the same size [MxN]
//!
//! \param M         number of rows of matrix A/B/C
//! \param N         number of columns of matrix A/B/C
//! \param pInputA a pointer to the base of the first input matrix A, in APU memory
//! \param pInputB a pointer to the base of the second input matrix B, in APU memory
//! \param pResult a pointer to where the result will be placed, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUMatrixSum(uint16_t M, uint16_t N, void *pInputA, void *pInputB, void *pResult);

//*****************************************************************************
//
//! \brief APU accelerator for multiplication of a matrix A and a scalar b
//!
//! Calculate multiplication of a matrix A and a scalar b.
//!
//! Defined as:
//! \code
//!   C[MxN] = b * A[MxN]
//! \endcode
//! in which, A and C are complex matrices with the same size [MxN], b is
//! complex scalar
//!
//! \param M         number of rows of matrix A
//! \param N         number of columns of matrix A
//! \param pInputA a pointer to the base of the first input matrix A, in APU memory
//! \param pInputB a pointer to the base of the scalar b, in APU memory
//! \param pResult a pointer to where the result will be placed, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUMatrixScalarMult(uint16_t M, uint16_t N, void *pInputA, void *pInputB, void *pResult);

//*****************************************************************************
//
//! \brief APU accelerator for addition of a matrix A and a scalar b
//!
//! Calculate addition of a matrix A and a scalar b.
//!
//! Defined as:
//! \code
//!  C[MxN] = b + A[MxN] (add scalar b to each of A's elements)
//! \endcode
//! in which, A and C are complex matrices with the same size [MxN], b is a complex scalar
//!
//! \param M         number of rows of matrix A/C
//! \param N         number of columns of matrix A/C
//! \param pInputA a pointer to the base of the first input matrix A, in APU memory
//! \param pInputB a pointer to the base of the scalar b, in APU memory
//! \param pResult a pointer to where the result will be placed, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUMatrixScalarSum(uint16_t M, uint16_t N, void *pInputA, void *pInputB, void *pResult);

//*****************************************************************************
//
//! \brief APU accelerator for Frobenius norm computation of a matrix
//!
//! Calculates the Frobenius norm of a matrix.
//!
//! Defined as:
//! \code
//! c = ||X|| = sqrt(sum(xij * xij')), i = 1 to M and j = 1 to N
//! \endcode
//!
//! \param M number of rows of input matrix
//! \param N number of columns of input matrix
//! \param pInput a pointer to the base of the input matrix, in APU memory
//! \param pResult a pointer to output norm value, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUMatrixNorm(uint16_t M, uint16_t N, void *pInput, void *pResult);

//*****************************************************************************
//
//! Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup apu_api_advanced_operations
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief APU accelerator for covariance matrix computation using spatial
//! smoothing and forward-backward averaging (optional)
//!
//! Calculate covariance matrix with spatial smoothing and forward-backward
//! averaging. Given a received signal length N, a smaller matrix (LxL) with
//! L < N is created by averaging (N-L+1) overlapped covariance matrices. The
//! spatially smoothed matrix can also be applied forward-backward averaging
//! (optional).
//!
//! \param N length of the received signal vector
//! \param pInput a pointer to the base of the input vector, in APU memory
//! \param L size of the output square covariance matrix
//! \param pResult a pointer to the output covariance matrix, in APU memory
//! \param fb forward-backward averaging (FBA) option
//!   - APU_FBA_DISABLE : No FBA
//!   - APU_FBA_ENABLE  : Applying FBA on top of spatially smoothed matrix
//! \return None
//
//*****************************************************************************
void APUSpSmoothCovMatrix(uint16_t N, void *pInput, uint16_t L, void *pResult, uint16_t fb);

//*****************************************************************************
//
//! \brief APU accelerator for Jacobi Eigen-Decomposition (EVD) of Hermitian Matrix
//!
//! Calculate the eigenvalues and eigenvectors of a Hermitian matrix.
//!
//! Defined as:
//! \code
//! [V D] = eigen_decomposition(A), such that V' * A * V = D
//! \endcode
//! in which, A[NxN] is Hermitian matrix, D[NxN] is a diagonal matrix of
//! eigenvalues, and V[NxN] is a matrix whose columns are the corresponding
//! eigenvectors
//!
//! \note Since A is the Hermitian matrix, eigenvalues are all real and positive
//! numbers. In addition, in APU, the output eigenvalues are already sorted in
//! descending order. To save memory, the input matrix A is updated by in-place
//! rotation, resulting in diagonal matrix D
//!
//! \param N size of the input/output square matrices
//! \param pInput a pointer to the base of the input Hermitian matrix and the
//!     output diagonal matrix D, in APU memory
//! \param pResultV a pointer to output eigenvectors, in APU memory
//! \param maxIter the maximum number of iterations (Jacobi sweeps), typically 3
//! \param minSum  threshold for early stopping condition (if summation of the
//!     off-diagonal values is smaller than this threshold)
//!
//! \return None
//
//*****************************************************************************
void APUJacobiEVD(uint16_t N, void *pInput, void *pResultV, uint16_t maxIter, float minSum);

//*****************************************************************************
//
//! \brief APU accelerator for Gauss-Jordan Elimination of a rectable complex
//! matrix
//!
//! Reduce the input rectangle matrix A[MxN] to reduced echelon form using
//! Gauss-Jordan Elimination.
//!
//! Defined as:
//! \code
//! C[MxN] = gauss_elimination(A[MxN])
//! \endcode
//! in which, A[MxN] is input matrix, C[MxN] is the output matrix in reduced
//! echelon form (in other words, C[M,1:M] is the identity matrix).
//!
//! \note Gauss-Jordan reduces the matrix to REDUCED echelon form (instead of
//! echelon form with Gauss) In addition, to save memory, the input matrix A is
//! overwritten by its reduced echelon form C using in-place transformation. If
//! out-place ever needed, the input matrix must be copied before calling this
//! function.
//!
//! \param M number of rows of input/output matrices
//! \param N number of columns of input/output matrices (N >= M)
//! \param pInput a pointer to the base of the input matrix and the output
//!     matrix, in APU memory
//! \param epsTol threshold that smaller than it, a value considered zero
//!
//! \return None
//
//*****************************************************************************
void APUGaussJordanElim(uint16_t M, uint16_t N, void *pInput, float epsTol);

//*****************************************************************************
//
//! \brief Configure APU accelerator for Fast Fourier transform
//!
//! Computes the Discrete Fourier transform (DFT) of a vector using a fast
//! Fourier transform (FFT) algorithm.
//!
//! Defined as:
//! \code
//!  Y = DFT(X, N)
//! \endcode
//! in which, X is the input vector length N, Y is the DFT of X with same length.
//!
//! \note This is the config function, must be called BEFORE feeding input vector
//! to the APU memory
//!
//! \param N length of input/output vectors
//! \param pX a pointer to the base of the input/output vector (since this is
//!     config, this pointer is just a placeholder), in APU memory
//!
//! \return None
//!
//! \sa APUComputeFft()
//
//*****************************************************************************
void APUConfigFft(uint16_t N, void *pX);

//*****************************************************************************
//
//! \brief APU accelerator for Fast Fourier transform
//!
//! Computes the Discrete Fourier transform (DFT) of a vector using a fast
//! Fourier transform (FFT) algorithm.
//!
//! Defined as:
//! \code
//!  Y = DFT(X, N)
//! \endcode
//! in which, X is the input vector length N, Y is the DFT of X with same length.
//!
//! \note This is where the FFT computation happen. It should be called AFTER
//! @ref APUConfigFft(). In addition, to save memory, the input is overwritten
//! by the output using in-place transformation. If out-place ever needed,
//! the input vector must be copied before calling this function.
//!
//! \param N length of input/output vectors
//! \param pX a pointer to the base of the input/output vector, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUComputeFft(uint16_t N, void *pX);

//*****************************************************************************
//
//! \brief APU accelerator for invert Fast Fourier transform
//!
//! Computes the invert Discrete Fourier transform (DFT) of a vector using a
//! fast Fourier transform (FFT) algorithm.
//!
//! Defined as:
//! \code
//!  Y = IFFT(X, N)
//! \endcode
//! in which, X is the input vector length N, Y is the invert DFT of X with same
//! length.
//!
//! \note This is where the IFFT computation happen. It should be called AFTER
//! @ref APUConfigFft(). In addition, to save memory, the input is overwritten
//! by the output using in-place transformation. If out-place ever needed,
//! the input vector must be copied before calling this function.
//!
//! \param N length of input/output vectors
//! \param pX a pointer to the base of the input/output vector, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUComputeIfft(uint16_t N, void *pX);

//*****************************************************************************
//
//! \brief APU accelerator for generating points evenly distributed on unit circle
//!
//! APU generates a unit circle as follows:
//! exp(-j*2*pi*(k*M+phase)/1024 * (-1)^(conj))
//! here k is iterated internally by the APU from 0 to N-1.
//! The result is stored at pResult
//! - k = 0:N-1 (Iterated internally)
//! - M = 10-bit constant
//! - phase = 10-bit constant
//! - conj = 0 or 1
//!
//! \param N number of points to be generated
//! \param M constant M in k*M+phase
//! \param phase constant phase in k*M+phase
//! \param conj
//!    - conj = 0 : Without conjugate the output
//!    - conj = 1 : Conjugate output
//! \param pResult a pointer to the output vector of points, in APU memory
//!
//! \return None
//
//*****************************************************************************
void APUUnitCircle(uint16_t N, uint16_t M, uint16_t phase, uint16_t conj, void *pResult);

//*****************************************************************************
//
//! Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//*****************************************************************************
//
//! Close the Doxygen group.
//! @}
//! @}
//
//*****************************************************************************

#endif //  __APU_H__
