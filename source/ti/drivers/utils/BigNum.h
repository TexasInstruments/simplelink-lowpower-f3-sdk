/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
/** ============================================================================
 *  @file       BigNum.h
 *
 *  @brief      BigNum module provides APIs for performing big number operations.
 *
 * # Limitations #
 * - BigNum module only supports limited big number arithmetic operations,
 *   such as modular reduction, with scope for additional API support in the future.
 * - The input size for modular reduction is arbitrarily limited at 16 words.
 */

#ifndef ti_drivers_utils_BigNum__include
#define ti_drivers_utils_BigNum__include

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief  Successful status code.
 *
 *  Functions return BigNum_STATUS_SUCCESS if the function was executed
 *  successfully.
 */
#define BigNum_STATUS_SUCCESS ((int_fast16_t)0)

/*!
 *  @brief  Generic error status code.
 *
 *  Functions return BigNum_STATUS_ERROR if the function was not executed
 *  successfully and no more specific error is applicable.
 */
#define BigNum_STATUS_ERROR ((int_fast16_t)-1)

/*!
 *  @brief  Maximum input length in words supported by modular reduction API
 */
#define BigNum_MOD_REDUCTION_MAX_INPUT_LEN_IN_WORDS 16

/*!
 *  @brief Perform modular reduction on large integer array
 *
 *  This function supports inputs up to #BigNum_MOD_REDUCTION_MAX_INPUT_LEN_IN_WORDS in word length.
 *
 *  @param  result          Pointer to a word-aligned buffer to store result in little-endian,
 *                          where result[0] contains the least significant 32-bits of result.
 *  @param  resultLenBytes  Result buffer length in bytes.
 *  @param  input           Pointer to the word-aligned buffer containing input in little-endian,
 *                          where input[0] contains the least significant 32-bits of input.
 *  @param  inputLenBytes   Input buffer length in bytes.
 *  @param  mod             Pointer to a word-aligned buffer containing the modulus in little-endian,
 *                          where mod[0] contains the least significant 32-bits of mod.
 *  @param  modLenBytes     Modulus buffer length in bytes.
 *
 *  @retval #BigNum_STATUS_SUCCESS   The operation succeeded.
 *  @retval #BigNum_STATUS_ERROR     The operation failed.
 */
int_fast16_t BigNum_modularReduction(uint32_t *result,
                                     size_t resultLenBytes,
                                     const uint32_t *input,
                                     size_t inputLenBytes,
                                     const uint32_t *mod,
                                     size_t modLenBytes);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_utils_BigNum__include */