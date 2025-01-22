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
 *  @file       HSMBareMetalECCUtility.h
 *
 *  @brief      Interface to The HSM IP in a BareMetal-based implementation.
 */

#ifndef HSMBAREMETALECCUTILITY_H
#define HSMBAREMETALECCUTILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define HSM_ASYM_DATA_VHEADER                (4U)
#define HSM_WORD_LENGTH                      (4U)
#define HSM_SIGNATURE_VCOUNT                 (2U)
#define HSM_ASYM_ECC_PUB_KEY_VCOUNT          (2U)
#define HSM_ASYM_CURVE25519_PUB_KEY_VCOUNT   (1U)
#define HSM_ASYM_ECC_PUB_KEY_UNCOMP_ENC_LENG (1U)
#define HSM_ASYM_ECC_UNCOMP_ENC_VALUE        (0x04U)
#define HSM_ASYM_ECC_SRD_SCRT_ENC_VALUE      (0U)
#define HSM_ASYM_COMPONENT_VECTOR_LENGTH     (144)

#define BITS_TO_BYTES(bits)  (((size_t)(bits) + 7U) / 8U)
/* Left-shift by 3 to multiply by 8, to convert from bytes to bits */
#define BYTES_TO_BITS(bytes) ((bytes) << 3)

#define HSM_IS_SIZE_MULTIPLE_OF_WORD(x)      (((x) & (HSM_RNG_WORD_LENGTH - 1U)) == 0U)
#define HSM_IS_SIZE_MULTIPLE_OF_RAW_BLOCK(x) (((x) & (HSM_RNG_RAW_DATA_BLOCK_SIZE - 1U)) == 0U)

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
 *  @brief  Get curve type parameter buffer pointer and size.
 *
 *  @param  [in]  curveType                     Curve type value.
 *  @param  [in]  curveParamsPtr                Curve parameters buffer pointer.
 *  @param  [in]  curveParamsSize               Curve parameters buffer size.
 *
 *  @retval #HSMBAREMETAL_STATUS_SUCCESS                    The operation succeeded.
 *  @retval #HSMBAREMETAL_STATUS_ERROR                      curveType value invalid.
 *
 */
int_fast16_t HSMBareMetalECCParams_getCurveParameters(uint8_t curveType,
                                                      const uint8_t **curveParamsPtr,
                                                      uint32_t *curveParamsSize);

/*!
 *  @brief  Reverse a buffer into the destination address
 *
 *  @param  [in]  dest                  Destination address.
 *  @param  [in]  src                   Source address.
 *  @param  [in]  size                  Size of the data to reverse and copy.
 */
void *HSMBareMetalECCParams_reverseMemCpy(void *dest, const void *src, size_t size);

/*!
 *  @brief  Uncompress a curve's generators into a single buffer.
 *
 *  @param  [in]  generatorX            Curve's generatorX component.
 *  @param  [in]  generatorY            Curve's generatorY component.
 *  @param  [in]  buffer                Output buffer.
 *  @param  [in]  curveLength           Curve lengths in bits.
 *  @param  [in]  length                Output buffer length.
 */
int_fast16_t HSMBareMetalECCParams_getUncompressedGeneratorPoint(uint8_t *generatorX,
                                                                 uint8_t *generatorY,
                                                                 uint8_t *buffer,
                                                                 size_t curveLength,
                                                                 size_t length);

/*!
 *  @brief  Format the header word in a vector component
 *
 *  @param  [in]  modulusSizeBits       Size of each sub-vector component in bits.
 *  @param  [in]  itemsLength           The number of sub-vector components.
 *  @param  [in]  itemIdx               The index of the sub-vector component.
 *  @param  [out] domainId              The domain to construct a vector header for (BrainPool or NIST).
 *  @param  [out] blob                  Buffer to copy the output vector to.
 */
void HSMBareMetalECCParams_asymVectorHeaderFormat(const size_t modulusSizeBits,
                                                  const uint8_t itemsLength,
                                                  const uint8_t itemIdx,
                                                  const uint8_t domainId,
                                                  uint32_t *blob);

/*!
 *  @brief  Constructs a vector component consistent with HSM format to hold the private key
 *
 *  @param  [in]  in                    Pointer of private key buffer
 *  @param  [in]  modulusSizeBits       Size of each sub-vector component in bits.
 *  @param  [out] domainId              The domain to construct a vector header for (BrainPool or NIST).
 *  @param  [out] blob                  Buffer to copy the output vector to.
 */
void HSMBareMetalECCParams_asymDHPriKeyToHW(uint8_t *in,
                                            const size_t modulusSizeBits,
                                            const uint8_t domainId,
                                            uint8_t *blob);

/*!
 *  @brief  Constructs a vector component consistent with HSM format to hold the public key
 *
 *  @param  [in]  in                    Pointer of public key buffer
 *  @param  [in]  modulusSizeBits       Size of each sub-vector component in bits.
 *  @param  [in]  itemsLength           The number of sub-vector components.
 *  @param  [in]  domainId              The domain to construct a vector header for (BrainPool or NIST).
 *  @param  [out] blob                  Buffer to copy the output vector to.
 */
void HSMBareMetalECCParams_asymDHPubKeyToHW(uint8_t *in,
                                            const size_t modulusSizeBits,
                                            const uint8_t itemsLength,
                                            const uint8_t domainId,
                                            uint8_t *blob);

/*!
 *  @brief  Extracts the public key from HSM-formatted output.
 *
 *
 *  @param  [in]  in                    Pointer to the input from HSM.
 *  @param  [in]  modulusSizeBits       Size of each sub-vector component in bits.
 *  @param  [in]  itemsLength           The number of sub-vector components.
 *  @param  [out] out_pubKey            Buffer to copy the public key to.
 */
void HSMBareMetalECCParams_asymDHPubKeyFromHW(const uint8_t *const in,
                                              const size_t modulusSizeBits,
                                              const uint8_t itemsLength,
                                              uint8_t *out_pubKey);

/*!
 *  @brief  Extracts the signature from HSM-formatted output
 *
 *  @param  [in]  in                    Pointer to the input from HSM.
 *  @param  [in]  modulusSizeBits       Size of each sub-vector component in bits.
 *  @param  [out] out_r                 Buffer to copy the R component of the signature to.
 *  @param  [out] out_s                 Buffer to copy the S component of the signature to.
 */
void HSMBareMetalECCParams_asymDsaSignatureFromHW(const uint8_t *const in,
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
void HSMBareMetalECCParams_asymDsaSignatureToHW(const uint8_t *const Signature_r,
                                                const uint8_t *const Signature_s,
                                                const size_t modulusSizeBits,
                                                uint8_t *const blob);

#ifdef __cplusplus
}
#endif

#endif /* HSMBAREMETALECCUTILITY_H */