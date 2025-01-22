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

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/aes.h)

/* Defines */
#define HSM_ASYM_DATA_VHEADER                (4U)
#define HSM_WORD_LENGTH                      (4U)
#define HSM_SIGNATURE_VCOUNT                 (2U)
#define HSM_ASYM_ECC_PUB_KEY_VCOUNT          (2U)
#define HSM_ASYM_CURVE25519_PUB_KEY_VCOUNT   (1U)
#define HSM_ASYM_ECC_PUB_KEY_UNCOMP_ENC_LENG (1U)
#define HSM_ASYM_ECC_UNCOMP_ENC_VALUE        (0x04U)
#define HSM_ASYM_ECC_SRD_SCRT_ENC_VALUE      (0U)
#define HSM_DRBG_RNG_BLOCK_SIZE              HSM_WORD_LENGTH
#define HSM_RAW_RNG_BLOCK_SIZE               (256U)
#define HSM_RAW_RNG_MAX_LENGTH               (65536U)
#define HSM_MAC_MAX_LENGTH                   (AES_BLOCK_SIZE)
#define HSM_AEAD_MIN_MAC_LENGTH              (2U)
#define HSM_AEAD_TEMP_ASSET_SIZE             (48U)
#define HSM_AES_128_KEY_LENGTH               (AES_128_KEY_LENGTH_BYTES)
#define HSM_AES_192_KEY_LENGTH               (AES_128_KEY_LENGTH_BYTES + 8)
#define HSM_AES_256_KEY_LENGTH               (2 * AES_128_KEY_LENGTH_BYTES)

#define BITS_TO_BYTES(bits)  (((size_t)(bits) + 7U) / 8U)
/* Left-shift by 3 to multiply by 8, to convert from bytes to bits */
#define BYTES_TO_BITS(bytes) ((bytes) << 3)

#define HSM_IS_SIZE_MULTIPLE_OF_WORD(x)           (((x) & (HSM_WORD_LENGTH - 1U)) == 0U)
#define HSM_IS_SIZE_MULTIPLE_OF_AES_BLOCK_SIZE(x) (((x) & (AES_BLOCK_SIZE - 1U)) == 0U)
#define HSM_IS_SIZE_MULTIPLE_OF_RAW_BLOCK(x)      (((x) & (HSM_RAW_RNG_BLOCK_SIZE - 1U)) == 0U)

/* HSM token-related Defines */

/* Asset Policy Codes */
#define HSM_ASSET_POLICY_SYM_HASH_MAC 0X00012800
#define HSM_ASSET_POLICY_SYM_AES_MAC  0X00022800
#define HSM_ASSET_POLICY_SYM_AES_BULK 0X00032800
#define HSM_ASSET_POLICY_SYM_AES_AUTH 0X00042800

#define HSM_ASSET_POLICY_NON_MODIFIABLE 0x00000001
#define HSM_ASSET_POLICY_TEMPORARY      0x00000002

#define HSM_ASSET_POLICY_DIR_ENC_GEN  0X00100000
#define HSM_ASSET_POLICY_DIR_DEC_VRFY 0X00200000

#define HSM_ASSET_POLICY_SYM_MODE_ECB     0X00000000
#define HSM_ASSET_POLICY_SYM_MODE_CBC     0X08000000
#define HSM_ASSET_POLICY_SYM_MODE_CTR     0X18000000
#define HSM_ASSET_POLICY_SYM_MODE_CCM     0X00000000
#define HSM_ASSET_POLICY_SYM_MODE_GCM     0X08000000
#define HSM_ASSET_POLICY_SYM_MODE_CMAC    0X00000000
#define HSM_ASSET_POLICY_SYM_MODE_CBC_MAC 0X08000000

/* Encryption Defines */
#define HSM_ENCRYPTION_TOKEN_WORD0         0x01000000
#define HSM_ENCRYPTION_TOKEN_WORD11        0x00004000
#define HSM_ENCRYPTION_TOKEN_WORD11_ENC    0x00008000
#define HSM_ENCRYPTION_TOKEN_WORD11_KEY128 0x00010000
#define HSM_ENCRYPTION_TOKEN_WORD11_KEY192 0x00020000
#define HSM_ENCRYPTION_TOKEN_WORD11_KEY256 0x00030000

#define HSM_ENCRYPTION_TOKEN_WORD11_CCM 0x00000050
#define HSM_ENCRYPTION_TOKEN_WORD11_GCM 0x00000070

#define HSM_ENCRYPTION_TOKEN_WORD11_SAVEIV 0x00001000
#define HSM_ENCRYPTION_TOKEN_WORD11_LOADIV 0X00000200

#define HSM_ENCRYPTION_TOKEN_WORD16_GCM_IV 0x01000000

/* MAC Defines */
#define HSM_MAC_TOKEN_WORD0 0x13000000

#define HSM_MAC_TOKEN_WORD6_CMAC    0x00000008
#define HSM_MAC_TOKEN_WORD6_CBC_MAC 0x00000009

#define HSM_MAC_TOKEN_WORD6_INIT2FINAL 0X00000000
#define HSM_MAC_TOKEN_WORD6_CONT2FINAL 0X00000010
#define HSM_MAC_TOKEN_WORD6_INIT2CONT  0X00000020
#define HSM_MAC_TOKEN_WORD6_CONT2CONT  0X00000030

typedef enum
{
    HSMLPF3_BIG_ENDIAN_KEY    = 0,
    HSMLPF3_LITTLE_ENDIAN_KEY = 1,
} HSMLPF3_KeyMaterialEndianness;

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
 *  @brief  Reverse a buffer into the destination address
 *
 *  @param  [in]  dest                  Destination address.
 *  @param  [in]  src                   Source address.
 *  @param  [in]  size                  Size of the data to reverse and copy.
 */
void *HSMLPF3_reverseMemCpy(void *dest, const void *src, size_t size);

/*!
 *  @brief  Format the header word in a vector component
 *
 *  @param  [in]  modulusSizeBits       Size of each sub-vector component in bits.
 *  @param  [in]  itemsLength           The number of sub-vector components.
 *  @param  [in]  itemIdx               The index of the sub-vector component.
 *  @param  [out] domainId              The domain to construct a vector header for (BrainPool or NIST).
 *  @param  [out] blob                  Buffer to copy the output vector to.
 */
void HSMLPF3_asymVectorHeaderFormat(const size_t modulusSizeBits,
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
 *  @param  [in]  endianness            The endianness format to copy the output in.
 *  @param  [out] blob                  Buffer to copy the output vector to.
 */
void HSMLPF3_asymDHPriKeyToHW(uint8_t *in,
                              const size_t modulusSizeBits,
                              const uint8_t domainId,
                              HSMLPF3_KeyMaterialEndianness endianness,
                              uint8_t *blob);

/*!
 *  @brief  Constructs a vector component consistent with HSM format to hold the public key
 *
 *  @param  [in]  in                    Pointer of public key buffer
 *  @param  [in]  modulusSizeBits       Size of each sub-vector component in bits.
 *  @param  [in]  itemsLength           The number of sub-vector components.
 *  @param  [in]  domainId              The domain to construct a vector header for (BrainPool or NIST).
 *  @param  [in]  endianness            The endianness format to copy the output in.
 *  @param  [out] blob                  Buffer to copy the output vector to.
 */
void HSMLPF3_asymDHPubKeyToHW(uint8_t *in,
                              const size_t modulusSizeBits,
                              const uint8_t itemsLength,
                              const uint8_t domainId,
                              HSMLPF3_KeyMaterialEndianness endianness,
                              uint8_t *blob);

/*!
 *  @brief  Extracts the public key from HSM-formatted output.
 *
 *
 *  @param  [in]  in                    Pointer to the input from HSM.
 *  @param  [in]  modulusSizeBits       Size of each sub-vector component in bits.
 *  @param  [in]  itemsLength           The number of sub-vector components.
 *  @param  [in]  endianness            The endianness format to copy the output in.
 *  @param  [out] out_pubKey            Buffer to copy the public key to.
 */
void HSMLPF3_asymDHPubKeyFromHW(const uint8_t *const in,
                                const size_t modulusSizeBits,
                                const uint8_t itemsLength,
                                HSMLPF3_KeyMaterialEndianness endianness,
                                uint8_t *out_pubKey);

/*!
 *  @brief  Extracts the signature from HSM-formatted output
 *
 *  @param  [in]  in                    Pointer to the input from HSM.
 *  @param  [in]  modulusSizeBits       Size of each sub-vector component in bits.
 *  @param  [out] out_r                 Buffer to copy the R component of the signature to.
 *  @param  [out] out_s                 Buffer to copy the S component of the signature to.
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
 *  @param  [in]  modulusSizeBits       Size of each sub-vector component in bits.
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
 *  @param  [in]  modulusSizeBits       Size of each sub-vector component in bits.
 *  @param  [out] domainId              The domain to construct a vector header for (BrainPool or NIST).
 *  @param  [out] blob                  Buffer to copy the output vector to.
 */
void HSMLPF3_asymDsaPriKeyToHW(const uint8_t *const pubKey,
                               const size_t modulusSizeBits,
                               const uint8_t domainId,
                               uint8_t *const blob);