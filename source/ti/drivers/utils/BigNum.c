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

#include <ti/drivers/cryptoutils/utils/CryptoUtils.h>
#include <ti/drivers/utils/BigNum.h>
#include <ti/drivers/cryptoutils/ecc/ECCParams.h>

#include <third_party/bearssl/inc/inner.h>

/* Maximum modulus length is determined by the largest ECC curve supported
 * by BigNum, which is NIST P-256.
 */
#define BigNum_MAX_MOD_LEN_IN_WORDS (ECCParams_NISTP256_LENGTH / 4U)

/* BearSSL 'i32' function APIs require inputs to be formatted in internal representation as an array
 * of 32-bit integers, x[], where the first word, x[0], contains the bit length of the array,
 * and the rest of the elements of x[] contain the input array in little-endian format.
 */
#define BigNum_MAX_MOD_LEN_INTERNAL_IN_WORDS (BigNum_MAX_MOD_LEN_IN_WORDS + 1U)

int_fast16_t BigNum_modularReduction(uint32_t *result,
                                     size_t resultLenBytes,
                                     const uint32_t *input,
                                     size_t inputLenBytes,
                                     const uint32_t *mod,
                                     size_t modLenBytes)
{
    /* Check if result length is sufficient */
    if ((resultLenBytes < modLenBytes) || (modLenBytes == 0) ||
        (inputLenBytes > (BigNum_MOD_REDUCTION_MAX_INPUT_LEN_IN_WORDS * 4)))
    {
        return BigNum_STATUS_ERROR;
    }

    uint32_t modInternal[BigNum_MAX_MOD_LEN_INTERNAL_IN_WORDS];
    uint32_t inputInternal[BigNum_MOD_REDUCTION_MAX_INPUT_LEN_IN_WORDS + 1];
    uint32_t resultInternal[BigNum_MAX_MOD_LEN_INTERNAL_IN_WORDS];
    size_t resultLenInternal;

    /* Copy modulus input to BearSSL's internal representation array */
    modInternal[0] = modLenBytes * 8;
    CryptoUtils_copyPad(mod, &modInternal[1], modLenBytes);

    /* Copy input to BearSSL's internal representation array */
    inputInternal[0] = inputLenBytes * 8;
    CryptoUtils_copyPad(input, &inputInternal[1], inputLenBytes);

    /* BearSSL modular reduction API */
    br_i32_reduce(resultInternal, inputInternal, modInternal);

    resultLenInternal = resultInternal[0] / 8;
    if (resultLenInternal <= resultLenBytes)
    {
        /* Copy output from BearSSL's internal representation array */
        CryptoUtils_copyPad(&resultInternal[1], result, modLenBytes);
        return BigNum_STATUS_SUCCESS;
    }
    else
    {
        return BigNum_STATUS_ERROR;
    }
}