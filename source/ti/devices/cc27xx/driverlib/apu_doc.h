/******************************************************************************
 *  Filename:       apu_doc.h
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
//! \addtogroup apu_api
//! @{
//! \section sec_apu Introduction
//!
//! The APU - Algorithm Processing Unit - is a generic mathematical acceleration module that is able
//! to operate with single-precision floating point numbers (IEEE 754 format) and is optimized to work with complex
//! numbers. This module is able to deal with vector (and matrix) operations efficiently
//!
//! \section sec_apu_number_format Numerical Representation Supported by APU
//!
//! In this material, a "number" is meant to represent a complex number, or two single precision IEEE 754
//! floating point numbers. There are two possible representations for complex numbers, cartesian or polar.
//!  - Cartesian Representation: In cartesian representation a complex number is stored as two real numbers,
//!                              with bits 63:32 being the imaginary part and bits 31:0 the real part.
//!  - Polar Representation: In polar representation, a complex number is stored as two real numbers,
//!                          where bits 63:32 are the phase (theta) and bits 31:0 the magnitude (r).
//!                          In both representations, 64 bit numbers can indicate a real number simply by making the
//!                          upper 32-bits all zero.
//! \note Most of APU API work with Cartesian respresentation. Therefore, unless clearly stated otherwise in the API
//! function description, the API input/output numbers are assumed to be in Cartesian representation. The APU HW will
//! NOT automatically take care of number representation conversions, it is up to the FW and the user to make sure that
//! the proper conversions are employed before an operation takes place between two complex numbers stored using
//! different representations. In other words, adding two numbers one stored in Cartesian format and the other in Polar
//! format, produces wrong results.
//!
//! \section sec_apu_data_memory APU Data Memory
//! \subsection subsec_memory_configuration Memory Configurations
//! The data memory can be used by the APU in two different configurations, depending on the application.
//! These configurations are known as "Interleaved Mode" (RAID 0) or "Mirrored Mode" (RAID 1).
//! The APU shall be configured to either of those operation modes.
//!   - Interleaved Mode: In this mode of operation, even word addresses are mapped to Bank 0 and odd word addresses are
//!   mapped to Bank 1. A word in this context is a 64-bit word (8 Bytes).
//!   - Mirrored Mode: In this mode of operation, a unified memory map of 1024 words is available for the application.
//!                    Read addresses towards bank 0 and read addresses towards bank 1 can proceed in parallel.
//! In summary, Mirrored mode reduces the total number of available words to 1024,
//! but gives the application total flexibility around the storage of those elements
//! and allows unrestricted access to two elements in parallel.
//! Interleaved mode offers dual the storage (2048 words) with the limitations of simultaneous
//! reads being restricted to even/odd addresses. Note that for the advanced algorithms are implemented exclusively on
//! Mirrored modes.
//!
//! \subsection subsec_data_memory_view Data Memory View
//! The data memories is seen differently from the CPU system side and from the APU side.
//! Essentially, the CPU will observe the normal 32-bit, byte oriented, memory map,
//! whereas the APU will operate and observe a 64-bit wide word-oriented map.
//! The CPU will operate with a 32-bit global address and then APU will only need a 12 bit local address.
//! This table illustrates the differences in the two views.
//! ~~~
//! |--------------------------------------------------------------------------------------------------------------|
//! | System Address Scheme                                                          | APU Address Scheme          |
//! |--------------------------------------|----|----|----|---------|----|---|-------|-----------------------------|
//! | imag(x)                                             | real(x)                  | x                           |
//! |--------------------------------------|----|----|----|---------|----|---|-------|-----------------------------|
//! | 7                                    | 6  | 5  | 4  | 3       | 2  | 1 | 0     | 0                           |
//! | 15                                   | 14 | 13 | 12 | 11      | 10 | 9 | 8     | 1                           |
//! |                                      |    |    |    |         |    |   |       | ...                         |
//! | 8*k+7                                |    |    |    |         |    |   | 8*k   | k                           |
//! |                                      |    |    |    |         |    |   |       | ...                         |
//! | 8191                                 |    |    |    |         |    |   | 8194  | 1023                        |
//! |                                      |    |    |    |         |    |   |       | ...                         |
//! | 16383                                |    |    |    |         |    |   | 16376 | 2047                        |
//! |--------------------------------------|----|----|----|---------|----|---|-------|-----------------------------|
//! ~~~
//!
//! \section sec_vector_matrix_storage Vector/Matrix Storage
//! \subsection subsec_vector_storage Vector Storage
//! Vector X with length N is stored by N units in APU memory from address start_address to end_address = start_address
//! + N - 1. Generally, element X[i] (with i = 0 to N-1) is stored at start_address + i in APU memory.
//!
//! From system address scheme:
//! \code
//! Address(real(X[i])) = start_address + i * 8;
//! Address(imag(X[i])) = start_address + i * 8 + 4;
//! \endcode
//!
//! \subsection subsec_matrix_storage Matrix Storage
//! Matrix A[MxN] with M rows and N columns is stored by M*N units in APU memory in the column-major order from
//! start_address to end_address = start_address + M*N - 1. Generally, element A[i,j] (i = 0 to M-1, j = 0 to N-1) is
//! stored at start_addres + j * M + i in APU memory
//!
//! From system address scheme:
//! \code
//! Address(real(A[i,j])) = start_address + (j * M + i) * 8;
//! Address(imag(A[i,j])) = start_address + (j * M + i) * 8 + 4;
//! \endcode
//!
//! \subsection subsec_triangular_matrix_storage Triangular Matrix Storage
//! Triangular matrix T[NxN] with N rows and N columns can be stored in an efficient manner by N*(N+1)/2 units
//! in the data memory from address start_address to end_address = start_address + N*(N+1)/2 - 1.
//! A simple way to access these matrices is to create the following LUT in the system CPU:
//! LUT =  [0,1,3,6,10,15,21,28,36,45,55,66,78,91,105,120,136,153,171,190,210,231,253,276,300,325,351,378,406,435,465]
//! with LUT[j] (j = 0 to N-1) is basically the order of element T[0,j] in APU memory.
//! Generally, element T[i,j] (i,j = 0 to N-1) is stored at start_address + LUT[j] + i in APU memory.
//! \note This equation only holds true for values of i <=j (which is the definition of an upper triangular matrix).
//! Symmetric matrices and hermitian matrices can also be stored in APU memory as upper triangular matrices as
//! described.
//!
//! From system address scheme:
//! \code
//! Address(real(A[i,j])) = start_address + (LUT[j] + i)*8;
//! Address(imag(A[i,j])) = start_address + (LUT[j] + i)*8 + 4;
//! \endcode
//!
//! \section sec_apu_api API
//!
//! The API functions can be grouped like this:
//!
//! APU Control Operations:
//! - \ref APUWaitOnIrq()
//! - \ref APUOperationDone()
//! - \ref APUWaitOp()
//! - \ref APUSetConfig()
//! - \ref APUNop()
//!
//! Vector Operations:
//! - \ref APUVectorDot()
//! - \ref APUVectorDotConj()
//! - \ref APUVectorMult()
//! - \ref APUVectorSum()
//! - \ref APUVectorCart2Pol()
//! - \ref APUVectorPol2Cart()
//! - \ref APUVectorSort()
//!
//! Matrix Operations:
//! - \ref APUMatrixMult()
//! - \ref APUMatrixMultHerm()
//! - \ref APUMatrixMultSym()
//! - \ref APUMatrixSum()
//! - \ref APUMatrixScalarMult()
//! - \ref APUMatrixNorm()
//! - \ref APUMatrixScalarSum()
//! - \ref APUMatrixScalarSum()
//!
//! Advanced Operations:
//! - \ref APUSpSmoothCovMatrix()
//! - \ref APUJacobiEVD()
//! - \ref APUGaussJordanElim()
//! - \ref APUConfigFft()
//! - \ref APUComputeFft()
//! - \ref APUComputeIfft()
//! - \ref APUUnitCircle()
//!
//! @}
