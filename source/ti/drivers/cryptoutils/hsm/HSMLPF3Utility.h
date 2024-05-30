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

/*!****************************************************************************
 *  @file       HSMLPF3Utility.h
 *
 *  @brief      Interface for helper functions for HSM-related operations
 *
 */

#include <stdint.h>

/* Defines */
#define HSM_ASYM_DATA_VHEADER (4U)
#define HSM_WORD_LENGTH       (4U)
#define HSM_SIGNATURE_VCOUNT  (2U)

#define BITS_TO_BYTES(bits) (((size_t)(bits) + 7U) / 8U)

/*!
 * HSM_ASYM_DATA_SIZE_B2W
 *
 * Converts a size in bits to the # of 32-bit words;
 */
static inline uint32_t HSM_ASYM_DATA_SIZE_B2W(const size_t x)
{
    return (uint32_t)((x + 31U) / 32U);
}

/*!
 * HSM_ASYM_DATA_SIZE_B2WB
 *
 * Converts a size in bits to a size in bytes aligned on a word boundary.
 */
static inline uint32_t HSM_ASYM_DATA_SIZE_B2WB(const size_t x)
{
    return (uint32_t)(HSM_WORD_LENGTH * (HSM_ASYM_DATA_SIZE_B2W(x)));
}

/*!
 * HSM_ASYM_DATA_SIZE_VWB
 *
 * Converts a size in bits to a size in bytes aligned on a word boundary + Header size.
 */
static inline uint32_t HSM_ASYM_DATA_SIZE_VWB(const size_t x)
{
    return (uint32_t)(HSM_ASYM_DATA_VHEADER + HSM_ASYM_DATA_SIZE_B2WB(x));
}

/*!
 *  @brief  Returns 32-bit aligned adjusted input length
 *
 *  @param  [in]  inputLength           Original input length
 */
static inline uint32_t HSMLPF3_getOutputBufferLength(const uint32_t inputLength)
{
#ifdef VEX_CHECK_DMA_WITH_TOKEN_ID
    /* 32-bit alignment + extra size for the TokenID */
    return (((inputLength + 3U) & (size_t)~3UL) + sizeof(size_t));
#else
    /* 32-bit alignment */
    return ((inputLength + 3U) & (size_t)~3UL);
#endif
}

/*!
 *  @brief  Extracts the signature from HSM-formatted output
 *
 *  @param  [in]  in                    Pointer to the input from HSM.
 *  @param  [in]  modulusSizeBits       size of each sub-vector component in bits.
 *  @param  [out] outu_r                Buffer to copy the R component of the signature to.
 *  @param  [out] outu_s                Buffer to copy the S component of the signature to.
 */
void HSMLPF3_asymDsaSignatureFromHW(const uint8_t *const in,
                                    const size_t modulusSizeBits,
                                    uint8_t *out_r,
                                    uint8_t *out_s);

/*!
 *  @brief  Constructs a vector component consistent with HSM format to hold the signature
 *
 *  @param  [in]  Signature_r           Pointer to R component of the signature to copy from.
 *  @param  [in]  Signature_s           Pointer to S component of the signature to copy from.
 *  @param  [out] modulusSizeBits       size of each sub-vector component in bits.
 *  @param  [out] blob                  Buffer to copy the output vector to.
 */
void HSMLPF3_asymDsaSignatureToHW(const uint8_t *const Signature_r,
                                  const uint8_t *const Signature_s,
                                  const size_t modulusSizeBits,
                                  uint8_t *const blob);

/*!
 *  @brief  Constructs a vector component consistent with HSM format to hold a verify operation's public key.
 *
 *  @param  [in]  pubKey                Pointer to public key location to copy.
 *  @param  [in]  modulusSizeBits       size of each sub-vector component in bits.
 *  @param  [out] domainId              The domain to construct a vector header for (BrainPool or NIST).
 *  @param  [out] blob                  Buffer to copy the output vector to.
 */
void HSMLPF3_asymDsaPubKeyToHW(const uint8_t *const pubKey,
                               const size_t modulusSizeBits,
                               const uint8_t domainId,
                               uint8_t *const blob);

/*!
 *  @brief  Constructs a vector component consistent with HSM format to hold a sign operation's private key.
 *
 *  @param  [in]  pubKey                Pointer to private key location to copy.
 *  @param  [in]  modulusSizeBits       size of each sub-vector component in bits.
 *  @param  [out] domainId              The domain to construct a vector header for (BrainPool or NIST).
 *  @param  [out] blob                  Buffer to copy the output vector to.
 */
void HSMLPF3_asymDsaPriKeyToHW(const uint8_t *const pubKey,
                               const size_t modulusSizeBits,
                               const uint8_t domainId,
                               uint8_t *const blob);