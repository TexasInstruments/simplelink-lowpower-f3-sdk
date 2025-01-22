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
/*!*****************************************************************************
 *  @file       APULPF3.h
 *  @brief      <b>PRELIMINARY</b> APU driver interface
 *
 *  <b>WARNING</b> These APIs are <b>PRELIMINARY</b>, and subject to
 *  change in the next few months.
 *
 *  @anchor ti_drivers_APU_Overview
 *  # Overview
 *  The APU driver allows you to interact with the Algorithm Processing Unit
 *  (APU), a linear algebra accelerator peripheral using 64-bit complex numbers.
 *  Complex numbers are represented by the _float complex_ type from _complex.h_,
 *  where each number is made up of two 32-bit floats. The APU works with
 *  complex numbers in either Cartesian or Polar formats. In the Cartesian
 *  representation, the lower 32 bits are the real part and the upper 32 bits
 *  are the imaginary part. In Polar format, the lower 32 bits are the absolute
 *  part and the upper 32 bits are the angle, represented as pi radians.
 *  As long as arguments to the APU
 *  functions use the same format, the result will also be in that format.
 *  Mixing formats produces wrong results. The driver provides basic data types
 *  for vectors and matrices constructed from _float complex_, as well as many
 *  common linear algebra operations on these data types.
 *
 *  @anchor ti_drivers_APU_DataManagement
 *  ## Data management
 *  All the vector/matrix operations can accept input and output pointers that
 *  are inside or outside APU RAM. If all pointers provided to an operation are
 *  inside APU RAM, the APU will operate in <b> scratchpad mode </b>.
 *  This means the driver will assume that, given the current pointers,
 *  the input is already in APU memory and that input and result will not
 *  overlap each other. Therefore, no data will be copied, and the function
 *  output will be placed inside APU memory. This is the most efficient way to
 *  utilize the APU and is ideal for algorithms with multiple operations that
 *  feed into each other, such as
 *  MUSIC (https://en.wikipedia.org/wiki/MUSIC_(algorithm)). When chaining
 *  together operations, make sure as many as possible use vectors and matrices
 *  that are already in APU memory, to prevent unnecessary copying and overhead.
 *
 *  If any of the pointers are outside APU memory, the driver will copy input
 *  data to the start of its memory, place the result immediately following,
 *  and then copy the output back to the provided pointer. This may overwrite
 *  data that was already in this location.
 *
 *  @warning The APU memory has a memory access limitation that must be
 *  respected, as to not cause a bus fault.
 *  Software must not perform any combination of back-to-back read or write
 *  instructions to APU memory (RR/ WW/ WR/ RW). There must be some other
 *  instruction in-between. For safety, load data into APU memory using
 *  any of the APU operations, #APULPF3_loadArgMirrored() or
 *  #APULPF3_loadTriangular().
 *  Copying data back from APU memory is automatically handled by the driver,
 *  and happens in an interrupt when the result pointer is outside APU memory.
 *
 *  The primary purpose of this driver is executing the MUSIC algorithm for
 *  distance estimation in Bluetooth Channel Sounding. An implementation of
 *  MUSIC using the APU can be found in the apu_music example.
 *
 *  @anchor ti_drivers_APU_Usage
 *  # Usage
 *  This section will cover driver usage.
 *
 *  @anchor ti_drivers_APU_Synopsis
 *  ## Synopsis
 *  @anchor ti_drivers_APU_Synopsis_Code
 *  @code
 *
 *  APULPF3_init();
 *
 *  float complex *apuMem = (float complex *)APULPF3_MEM_BASE;
 *  float complex argA[10];
 *  float complex argB[10];
 *
 *  APULPF3_ComplexVector vecA = {.data = bufA, .size = 10};
 *  APULPF3_ComplexVector vecB = {.data = bufB, .size = 10};
 *  APULPF3_ComplexVector resultVec = {.data = apuMem, .size = 10};
 *
 *  // Get control of APU
 *  APULPF3_startOperationSequence();
 *
 *  // Perform element-wise product, placing the result in resultVec,
 *  // which is inside APU memory
 *  APULPF3_elemProduct(&argA, &argB, resultVec);
 *
 *  // Perform non-conjugated dot product inside of APU memory, which
 *  // reduces overhead.
 *  APULPF3_dotProduct(&resultVec, &resultVec, false, apuMem)
 *
 *  // Give up control of APU
 *  APULPF3_finishOperationSequence();
 *
 *  @endcode
 ***************************************************************************
 */
#ifndef ti_drivers_APULPF3__include
#define ti_drivers_APULPF3__include

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <complex.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_memmap.h)

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ========= APU datatype structs =========
 * These structs define the core data types used by APU functions,
 * which are vectors, full matrices and upper triangle matrices of
 * complex numbers. Matrices are column-major.
 */

/** @addtogroup APULPF3_DATATYPES
 *  @{
 */
/*!
 *  @brief APULPF3 Vector Struct
 *
 *  Struct representing a Vector data type. Vectors are stored in memory as an
 *  arry.
 *
 *  @note When allocating memory for this struct, provide at least:
 *  sizeof(complex float)* size bytes, where size is #APULPF3_ComplexVector.size.
 *
 */
typedef struct
{
    /*! Pointer to the vector data. */
    complex float *data;
    /*! Length of the vector. */
    uint16_t size;
} APULPF3_ComplexVector;

/*!
 *  @brief APULPF3 Matrix Struct
 *
 *  Struct representing a column-major Matrix data type. Column-major means that
 *  for an NxN matrix, N columns of size N will be stored after each other in
 *  memory.
 *
 *  @note When allocating memory for this struct, provide at least:
 *  sizeof(complex float)* rows * cols bytes, where rows is #APULPF3_ComplexMatrix.rows
 *  and cols is #APULPF3_ComplexMatrix.cols.
 */
typedef struct
{
    /*! Pointer to the matrix data. */
    complex float *data;
    /*! Number of rows in the matrix. */
    uint16_t rows;
    /*! Number of columns in the matrix. */
    uint16_t cols;
} APULPF3_ComplexMatrix;

/*!
 *  @brief APULPF3 Upper Triangle Matrix Struct
 *
 *  The complex triangle matrix struct represents square upper-triangle matrices,
 *  which are used by some APU operations to save space.
 *  In memory they are laid out in column major order, laid as below:
 *  <br> | <b> (0,0) </b> | <b> (0,1) </b>  | <b> (0,2) </b> |
 *  <br> |  (1,0)         | <b> (1,1) </b>  | <b> (1,2) </b> | is represented as [(0,0)], [(0,1), (1,1)], [(0,2), (1,2),
 *  (2,2)] <br> |  (2,0)         |   (2,1)         | <b> (2,2) </b> |
 *
 *
 * @note When allocating memory pointed to by #APULPF3_ComplexTriangleMatrix.size, provide at least:
 *  sizeof(complex float)* size *(size+1)/2 where size is #APULPF3_ComplexTriangleMatrix.size.
 */
typedef struct
{
    /*! Pointer to the matrix data. */
    complex float *data;
    /*! Number of rows and columns in the matrix. */
    uint16_t size;

} APULPF3_ComplexTriangleMatrix;

/** @}*/

/*!
 *  @brief
 *  Define the APU memory operation modes, which are the ways the APU expects
 *  data to be stored in its memory.
 */
typedef enum
{
    /*!< In mirrored mode elements are stored sequentially in APU memory. */
    APULPF3_OperationMode_MIRRORED
} APULPF3_OperationMode;

/*!
 *  @brief
 *  Define the APU memory scheduling modes, which are the ways APU memory operations
 *  are scheduled and pipelined.
 */
typedef enum
{
    /*! In pipelined mode, read operations proceed per clock cycle, and
     * only the incoming samples will regulate the data flow.
     */
    APULPF3_SchedulingMode_PIPELINED

} APULPF3_SchedulingMode;

/*!
 *  @brief  APULPF3 Global configuration
 */
typedef struct
{
    /*! Pointer to a driver specific data object */
    void *object;
    /*! Pointer to a driver specific hardware attributes structure */
    void const *hwAttrs;
} APULPF3_Config;

/*!
 *  @brief  APULPF3 Hardware attributes
 */
typedef struct
{
    unsigned int intPriority;

    APULPF3_OperationMode operationMode;

    APULPF3_SchedulingMode schedulingMode;
} APULPF3_HWAttrs;

/** @addtogroup APULPF3_STATUS
 *  @{
 */
/*!
 * @brief   Successful status code.
 *
 */
#define APULPF3_STATUS_SUCCESS 0

/*!
 * @brief   Generic error status code.
 */
#define APULPF3_STATUS_ERROR 1

/*!
 * @brief   An error status code returned if the hardware or software resource
 * is currently unavailable.
 */
#define APULPF3_STATUS_RESOURCE_UNAVAILABLE 2
/** @}*/

/*!
 * @brief APU operation is in-place, overwriting the input.
 */
#define APULPF3_RESULT_INPLACE 0

/*!
 * @brief Start of APU RAM
 */
#define APULPF3_MEM_BASE VCERAM_DATA0_BASE

/*!
 * @brief Size of APU RAM in mirrored mode
 */
#define APULPF3_MEM_SIZE_MIRRORED APURAM_DATA0_SIZE

/*!
 *  @brief  APU init function
 *
 *  This function initializes the APU internal state. It sets power dependencies,
 *  loads the necessary firmware, configures the APU and sets up interrupts and
 *  semaphores
 *
 */
void APULPF3_init(void);

/*!
 *  @brief  APU function to prepare the start of an operation chain
 *
 *  This function disables standby and acquire exclusive access to the APU.
 *  It should be called before any sequence of APU operations, wrapping them
 *  together with APULPF3_stopOperationSequence().
 *  APU operations should not be called outside of a pair of APULPF3_startOperationSequence() and
 *  APULPF3_stopOperationSequence() functions.
 *
 *  @pre    APULPF3_init() has been called.
 *
 */
void APULPF3_startOperationSequence();

/*!
 *  @brief  APU function to finish an operation chain
 *
 *  This function enables standby and releases exclusive access to the APU.
 *  It should be called after any sequence of APU operations, wrapping them
 *  together with APULPF3_startOperationSequence().
 *  APU operations should not be called outside of a pair of APULPF3_startOperationSequence() and
 *  APULPF3_stopOperationSequence() functions.
 *
 *  @pre    APULPF3_init() and APULPF3_startOperationSequence() has been called.
 *
 */
void APULPF3_stopOperationSequence();

/* Vector functions */
/** @addtogroup APULPF3_VECTOR_VECTOR_FUNCTIONS
 *  @{
 */
/*!
 * @brief APU function for calculating the dot product of two vectors, with the
 * option to perform the complex conjugate on the second vector first.
 *
 * Defined as:
 * @code
 *  c = A dot B = sum(A[i] * B[i]), i = 0 to N-1
 * @endcode
 * or:
 * @code
 *  c = A dot conj(B) = sum(A[i] * conj(B[i])), i = 0 to N-1
 * @endcode
 * in which, A, B are complex vectors and c is a complex scalar.
 *
 * @pre    #APULPF3_init() has to be called first.
 *
 * @param[in] vecA a pointer to the first input vector
 *
 * @param[in] vecB a pointer to the second input vector
 *
 * @param[in] conjugate whether or not to conjugate @p vecB
 *
 * @param [out] result a pointer where the output will be placed
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 *  @retval #APULPF3_STATUS_ERROR The input vectors were of different sizes.
 */
int_fast16_t APULPF3_dotProduct(APULPF3_ComplexVector *vecA,
                                APULPF3_ComplexVector *vecB,
                                bool conjugate,
                                float complex *result);

/*!
 * @brief APU function for calculating the element-wise product of two vectors, with the
 * option to perform the complex conjugate on the second vector first.
 *
 * If both arguments and the result pointer are already within APU memory,
 * the function will be executed in scratchpad mode. This means that it will assume
 * pointers are placed so
 *
 * Defined as:
 * @code
 *  C = A .* B = [A[0] * B[0], A[1] * B[1], .., A[N-1] * B[N-1]]
 * @endcode
 * or:
 * @code
 *  C = A .* conj(B) = [A[0] * conj(B[0]), A[1] * conj(B[1]), .., A[N-1] * conj(B[N-1])]
 * @endcode
 * in which, A, B and C are complex vectors.
 *
 * @pre    #APULPF3_init() has to be called first.
 *
 * @param[in] vecA a pointer to the first input vector
 *
 * @param[in] vecB a pointer to the second input vector
 *
 * @param[in] conjugate whether or not to conjugate @p vecB
 *
 * @param [out] result a pointer to a vector where the result will be placed
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 *  @retval #APULPF3_STATUS_ERROR The input vectors were of different sizes.
 */
int_fast16_t APULPF3_vectorMult(APULPF3_ComplexVector *vecA,
                                APULPF3_ComplexVector *vecB,
                                bool conjugate,
                                APULPF3_ComplexVector *result);

/*!
 * @brief APU function for calculating the element-wise sum of two vectors.
 *
 * Defined as:
 * @code
 *  C = A + B = [A[0] + B[0], A[1] + B[1], .., A[N-1] + B[N-1]]
 * @endcode
 * in which, A, B and C are complex vectors.
 *
 * @pre    #APULPF3_init() has to be called first.
 *
 * @param[in] vecA a pointer to the first input vector
 *
 * @param[in] vecB a pointer to the second input vector
 *
 * @param [out] result a pointer to a vector where the output will be placed.
 *                     Its size should be the same as that of the inputs.
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 *  @retval #APULPF3_STATUS_ERROR The input vectors were of different sizes.
 */
int_fast16_t APULPF3_vectorSum(APULPF3_ComplexVector *vecA, APULPF3_ComplexVector *vecB, APULPF3_ComplexVector *result);
/** @}*/

/** @addtogroup APULPF3_VECTOR_ALGORITHMS
 *  @{
 */
/*!
 * @brief APU function for converting a complex vector in cartesian format to polar format.
 *
 * @pre    #APULPF3_init() has to be called first.
 *
 * @param[in] vec a pointer to an input vector in cartesian format
 *
 * @param [out] result a pointer to the vector where the output will be placed.
 *              Its size should be the same as that of the inputs.
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 */
int_fast16_t APULPF3_cartesianToPolarVector(APULPF3_ComplexVector *vec, APULPF3_ComplexVector *result);

/*!
 * @brief APU function for converting a complex vector in polar format to cartesian format.
 *
 * @pre    #APULPF3_init() has to be called first.
 *
 * @param[in] vec a pointer to an input vector in cartesian format
 *
 * @param[in] temp a pointer to a temporary vector of the same length as @p vec to store temporary results.
 *                 If the @p vec and @p result are not in APU memory, this argument is ignored and
 *                 the temp vector is placed after the result in APU memory.
 *
 * @param [out] result a pointer to a vector where the output will be placed.
 *              Its size should be the same as that of the inputs.
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 *  @retval #APULPF3_STATUS_ERROR The @p temp vector was not in APU space.
 */
int_fast16_t APULPF3_polarToCartesianVector(APULPF3_ComplexVector *vec,
                                            float complex *temp,
                                            APULPF3_ComplexVector *result);

/*!
 * @brief APU function for sorting the real parts of a complex vector in descending order. This function ignores
 * the complex parts of each element and makes no guarantees to their contents after the operation
 * is complete.
 *
 * @pre    #APULPF3_init() has to be called first.
 *
 * @param[in] vec a pointer to an input vector to be sorted
 *
 * @param [out] result a pointer to a vector where the output will be placed.
 *               Its size should be the same as that of the inputs.
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 */
int_fast16_t APULPF3_sortVector(APULPF3_ComplexVector *vec, APULPF3_ComplexVector *result);

/*!
 * @brief APU function for covariance matrix computation using spatial smoothing
 *  and optionally forward-backward averaging.
 *
 * Given a received signal length N, a smaller matrix (LxL) with L < N is created
 * by averaging (N-L+1) overlapped covariance matrices. When forward-backward averaging
 * is applied, the output matrix becomes:
 * @code
 * Rfb = 1/2(R + J * R' * J)
 * @endcode
 * Where R is the spacially smoother matrix and J is the exchange matrix.
 * R' signifies the conjugate transpose of R.
 * @pre    #APULPF3_init() has to be called first.
 *
 * @param[in] vec a pointer to an input vector
 *
 * @param[in] covMatrixSize size of the output covariance matrix. The output triangular matrix
 *                          will have rows and columns equal to this value.
 *
 * @param[in] fbAveraging whether or not to perform forward-backwards averaging
 *
 * @param [out] result a pointer to a upper triangle matrix where the output will be placed
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 */
int_fast16_t APULPF3_covMatrixSpatialSmoothing(APULPF3_ComplexVector *vec,
                                               uint16_t covMatrixSize,
                                               bool fbAveraging,
                                               APULPF3_ComplexTriangleMatrix *result);

/*!
 * @brief APU function for computing the Discrete Fourier transform (DFT) of a complex vector
 *  using the Fast Fourier Transform (FFT) algorithm. Optionally, the Inverse DFT can be computed.
 *  Combines two APU operations; first configuring the APU for a fourier transform, then actually computing it.
 *
 * @pre    #APULPF3_init() has to be called first.
 *
 * @param[in] vec a pointer to an input vector. The vector needs to have a size equal to a power of two.
 *
 * @param[in] inverse whether or not to perform IFFT instead of DFT
 *
 * @param [out] result a pointer to a vector where the output will be placed
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 */
int_fast16_t APULPF3_computeFFT(APULPF3_ComplexVector *vec, bool inverse, APULPF3_ComplexVector *result);
/** @}*/

/* Matrix functions */
/** @addtogroup APULPF3_MATRIX_FUNCTIONS
 *  @{
 */

/*!
 * @brief APU function for multiplying two matrices. The number of rows in the first matrix must be equal
 * to the number of columns in the second matrix.
 *
 * Defined as:
 * @code
 * C[MxP] = A[MxN] * B[NxP]
 * @endcode
 * in which A, B, and C are complex matrices with size [MxP], [MxN], and [NxP] respectively.
 *
 * @pre    #APULPF3_init() has to be called first.
 *
 * @param[in] matA a pointer to the first input matrix
 *
 * @param[in] matB a pointer to the second input matrix
 *
 * @param [out] result a pointer to a matrix where the output will be placed
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 *  @retval #APULPF3_STATUS_ERROR The input matrices were incompatible, i.e.
 *                            the number of colums in @p matA were not equal to the number of rows in @p matB.
 */
int_fast16_t APULPF3_matrixMult(APULPF3_ComplexMatrix *matA,
                                APULPF3_ComplexMatrix *matB,
                                APULPF3_ComplexMatrix *result);

/*!
 * @brief APU function for adding two matrices. The matrices must be of exact same sizes.
 *
 * Defined as:
 * @code
 * C[MxN] = A[MxN] + B[MxN]
 * @endcode
 * in which A, B, and C are complex matrices with size [MxN].
 *
 * @pre    #APULPF3_init() has to be called first.
 *
 * @param[in] matA a pointer to the first input matrix
 *
 * @param[in] matB a pointer to the second input matrix
 *
 * @param [out] result a pointer to a matrix where the output will be placed
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 *  @retval #APULPF3_STATUS_ERROR The input matrices were incompatible,
 *                            i.e. the matrices were not of the exact same size.
 */
int_fast16_t APULPF3_matrixSum(APULPF3_ComplexMatrix *matA, APULPF3_ComplexMatrix *matB, APULPF3_ComplexMatrix *result);

/*!
 * @brief APU function for adding a scalar to a each of a matrix' elements.
 *
 * Defined as:
 * @code
 * C[MxN] = _a_ + B[MxN]
 * @endcode
 * in which _a_ is a scalar and B, and C are complex matrices with size [MxN].
 *
 * @pre    #APULPF3_init() has to be called first.
 *
 * @param[in] mat a pointer to the input matrix
 *
 * @param[in] scalar a pointer to a scalar
 *
 * @param [out] result a pointer to a matrix where the output will be placed
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 */
int_fast16_t APULPF3_matrixScalarSum(APULPF3_ComplexMatrix *mat, float complex *scalar, APULPF3_ComplexMatrix *result);

/*!
 * @brief APU function for multiplying each of a matrix' elements by a scalar.
 *
 * Defined as:
 * @code
 * C[MxN] = _a_ * B[MxN]
 * @endcode
 * in which _a_ is a scalar and B, and C are complex matrices with size [MxN].
 *
 * @pre    #APULPF3_init() has to be called first.
 *
 * @param[in] mat a pointer to the input matrix
 *
 * @param[in] scalar a pointer to a scalar
 *
 * @param [out] result a pointer to a matrix where the output will be placed
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 */
int_fast16_t APULPF3_matrixScalarMult(APULPF3_ComplexMatrix *mat, float complex *scalar, APULPF3_ComplexMatrix *result);

/*!
 * @brief Compute the Frobenius norm of a matrix.
 *
 * Defined as:
 * @code
 * _c_ = ||X|| = sqrt(sum(xij * xij')), i = 1 to M and j = 1 to N
 * @endcode
 * in which, X[MxN] is the input matrix and _c_ is the Frobenius norm
 *
 * @param[in] mat the input matrix
 *
 * @param [out] result a pointer where the output will be placed
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 */
int_fast16_t APULPF3_matrixNorm(APULPF3_ComplexMatrix *mat, float complex *result);
/** @}*/

/* Matrix algorithms */
/** @addtogroup APULPF3_MATRIX_ALGORITHMS
 *  @{
 */

/*!
 * @brief APU function to compute the Jacobi Eigen-Decomposition (EVD) of a triangular Hermitian Matrix.
 * This results in a triangular, diagonal matrix of eigenvalues sorted in descending order replacing the original
 * matrix, and a full matrix, where the eigenvectors are the columns. The eigenvector matrix can be placed anywhere in
 * APU memory except for where the input matrix is placed. When not using scratchpad mode, the eigenvector matrix will
 * be placed next to the eigenvector matrix.
 *
 * Defined as:
 * @code
 * (V, D) = eigen_decomposition(A), such that V' * A * V = D
 * @endcode
 * in which A[NxN] is a triangular Hermitian matrix V[NxN] is a triangular, diagonal matrix of eigenvalues,
 * D[NxN] is a full matrix of eigenvectors and V' signifies the conjugate transpose of V.
 *
 * @note Since A is the Hermitian matrix, eigenvalues are all real and positive numbers.
 * In the APU, the output eigenvalues are already sorted in descending order. To save
 * memory, the input matrix A is updated by in-place rotation, resulting in the diagonal matrix D.
 * If not in scratchpad mode, i.e one of the provided vectors/matrices have their data outside of APU space,
 * this function will return a Vector whose data contains the data of both the eigenvalue triangular matrix and the
 * eigenvalue full matrix. The eigenvector data begins at an offset of (L*L + L)/2.
 *
 * @pre    #APULPF3_init() has to be called first.
 *
 * @param[in] mat the input matrix
 *
 * @param[in] maxIter the maximum number of iterations for the Jacobi algorithm (typically 3)
 *
 * @param[in] stopThreshold threshold for stopping early. Triggers if summation of the off-digonal values is smaller
 * than this threshold.
 *
 * @param [out] result a pointer to a vector where the output will be placed. If not in scratchpad mode,
 *              this vector will contain both the eigenvalues and the eigenvectors.
 *              Otherwise it will contain only the eigenvectors.
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 *  @retval #APULPF3_STATUS_ERROR The input matrix was not square, and therefore not Hermitian.
 */

int_fast16_t APULPF3_jacobiEVD(APULPF3_ComplexTriangleMatrix *mat,
                               uint16_t maxIter,
                               float stopThreshold,
                               APULPF3_ComplexVector *result);

/*!
 * @brief Reduce the input matrix A[MxN] to reduced echelon form using Gauss-Jordan Elimination.
 *
 * Defined as:
 * @code
 * C[MxN] = gauss_elimination(A[MxN])
 * @endcode
 * in which, A[MxN] is the input matrix and C[MxN] is the output matrix in reduced echelon form
 * (in other words, C[M,1:M] is the identity matrix).
 *
 * @param[in] mat the input matrix
 *
 * @param[in] zeroThreshold matrix values are considered zero if they are smaller than this value
 *
 * @param [out] result a pointer to a matrix where the output will be placed
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 */
int_fast16_t APULPF3_gaussJordanElim(APULPF3_ComplexMatrix *mat, float zeroThreshold, APULPF3_ComplexMatrix *result);

/** @}*/

/* Utility functions */

/** @addtogroup APULPF3_MISC
 *  @{
 */

/*!
 * @brief Generate points evenly distributed on a unit circle
 * APU generates a unit circle as follow: exp(-j*2*pi*(k*M+phase)/1024 * (-1)^(conjugate))
 * - k = 0:N-1
 * - M = 10-bit constant
 * - phase = 10-bit constant
 * - conjugate = 0 or 1
 *
 * @param[in] numPoints the number of points to generate (k)
 *
 * @param[in] constant a 10-bit constant (M)
 *
 * @param[in] phase a constant in (k*M+phase)
 *
 * @param[in] conjugate whether or not to conjugate the output
 *
 * @param [out] result a pointer to a vector where the output will be placed
 *
 * @note See @ref ti_drivers_APU_DataManagement for directions on efficient APU memory management.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 */
int_fast16_t APULPF3_unitCircle(uint16_t numPoints,
                                uint16_t constant,
                                uint16_t phase,
                                bool conjugate,
                                APULPF3_ComplexVector *result);

/** @}*/

/*!
 * @brief Configure the APU pointers for temporary (in APU memory) and final results for a APU operation.
 *        This function is intended to be used inside APU operations, such as dot product.
 *
 * @param[in] resultSize the size of an operation result
 *
 * @param[in] inputSize the size of an operation input
 *
 * @param [out] resultBuffer a pointer where the operation result will be placed in APU memory.
 *                           If not using scratchpad mode, usually when an external buffer is supplied,
                             configure to place the result at the start of APU memory, before copying it
                             to the buffer.
 *
 * @return A status code indicating whether the APU operation was a success.
 *
 *  @retval #APULPF3_STATUS_SUCCESS The call was successful.
 */
void APULPF3_prepareResult(uint16_t resultSize, uint16_t inputSize, complex float *resultBuffer);

/*!
 * @brief Configure the APU for vector operation inputs. If not in scratchpad mode, the vectors
 *        are loaded one after the other into the start of APU memory.
 *
 * @param[in] vecA an input vector
 *
 * @param[in] vecB an input vector, potentially the same as @p vecA
 *
 * @return The total input size. If the input vectors are the same, this is the vector's size,
 *         otherwise it is the sum of the vector sizes.
 */
uint16_t APULPF3_prepareVectors(APULPF3_ComplexVector *vecA, APULPF3_ComplexVector *vecB);

/*!
 * @brief Configure the APU for matrix operation inputs. If not in scratchpad mode, the matrices
 *        are loaded one after the other into the start of APU memory.
 *
 * @param[in] matA a pointer to an input matrix
 *
 * @param[in] matB a pointer to an input matrix, potentially the same as @p matA
 *
 * @return The total input size. If the input matrices are the same, this is the matrix's size,
 *         otherwise it is the sum of the vector sizes.
 */
uint16_t APULPF3_prepareMatrices(APULPF3_ComplexMatrix *matA, APULPF3_ComplexMatrix *matB);

/*!
 * @brief Loads the upper triangular part of a full matrix into APU memory.
 *
 * @param[in] mat a pointer to a source matrix
 *
 * @param[in] offset offset into APU memory to load to
 *
 * @return Pointer in APU memory where the argument is stored.
 */
void *APULPF3_loadTriangular(APULPF3_ComplexMatrix *mat, uint16_t offset);

/*!
 * @brief Load operation arguments into APU memory, assuming the APU is
 *        in mirrored mode. This means memory is viewed as one block that
 *        can fit 1024 complex numbers.
 *
 * @param[in] argSize how many complex numbers to load
 *
 * @param[in] offset offset into APU memory to load to
 *
 * @param[in] src data source pointer
 *
 * @return Pointer in APU memory where the argument is stored.
 */
void *APULPF3_loadArgMirrored(uint16_t argSize, uint16_t offset, float complex *src);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_APU__include */
