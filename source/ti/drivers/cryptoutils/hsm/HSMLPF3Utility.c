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
#include <stdint.h>
#include <string.h>

#include <ti/drivers/cryptoutils/hsm/HSMLPF3Utility.h>

/* Forward declarations for helper functions */
static void *HSMLPF3_reverseMemCpy(void *dest, const void *src, size_t Size);
static void HSMLPF3_asymBigIntToHw(const uint8_t *const data,
                                   const size_t modulusSizeBits,
                                   const uint8_t beginItem,
                                   const uint8_t items,
                                   uint8_t *out);
static void HSMLPF3_asymRsaSignatureFromHw(const uint8_t *const in, const size_t modulusSizeBits, uint8_t *out);

/*
 *  ======== HSMLPF3_reverseMemCpy ========
 */
static void *HSMLPF3_reverseMemCpy(void *dest, const void *src, size_t size)
{
    uint8_t *dp       = (uint8_t *)dest;
    const uint8_t *sp = (const uint8_t *)src;
    size_t tempSize   = size;

    sp = &sp[(tempSize - 1U)];
    while (tempSize > 0U)
    {
        *dp = *sp;
        dp++;
        sp--;
        tempSize--;
    }

    return dest;
}

/*
 *  ======== HSMLPF3_asymRsaSignatureFromHw ========
 */
static void HSMLPF3_asymRsaSignatureFromHw(const uint8_t *const in, const size_t modulusSizeBits, uint8_t *out)
{
    uint32_t Size = (uint32_t)BITS_TO_BYTES(modulusSizeBits);

    /* Convert big integer from HW to application format */
    HSMLPF3_reverseMemCpy(out, &in[HSM_ASYM_DATA_VHEADER], Size);
}

/*
 *  ======== HSMLPF3_asymBigIntToHw ========
 */
static void HSMLPF3_asymBigIntToHw(const uint8_t *const data,
                                   const size_t modulusSizeBits,
                                   const uint8_t beginItem,
                                   const uint8_t items,
                                   uint8_t *out)
{
    uint8_t *ptr             = out;
    uint32_t RemainingLength = HSM_ASYM_DATA_SIZE_B2WB((uint32_t)modulusSizeBits);
    uint32_t CopySize        = (uint32_t)BITS_TO_BYTES(modulusSizeBits);

    /* Convert big integer from application to HW format */
    /* - Initialize header */
    *ptr = (uint8_t)(modulusSizeBits);
    ptr++;
    *ptr = (uint8_t)(modulusSizeBits >> 8);
    ptr++;
    *ptr = (uint8_t)(beginItem);
    ptr++;
    *ptr = (uint8_t)(items);
    ptr++;
    /* - Copy data reversed */
    if (CopySize > RemainingLength)
    {
        /* Prevent buffer overrun */
        CopySize = RemainingLength;
    }
    else
    {
        /* MISRA - Intentially empty */
    }

    HSMLPF3_reverseMemCpy(ptr, data, CopySize);
    /* - Zeroize remaining part if needed */
    RemainingLength -= CopySize;
    if (RemainingLength != 0U)
    {
        (void)memset(&ptr[CopySize], 0, RemainingLength);
    }
    else
    {
        /* MISRA - Intentially empty */
    }
}

/*
 *  ======== HSMLPF3_asymDsaSignatureFromHW ========
 */
void HSMLPF3_asymDsaSignatureFromHW(const uint8_t *const in,
                                    const size_t modulusSizeBits,
                                    uint8_t *out_r,
                                    uint8_t *out_s)
{
    /* Convert Signature from HW to application format */
    HSMLPF3_asymRsaSignatureFromHw(in, modulusSizeBits, out_r);
    HSMLPF3_asymRsaSignatureFromHw(&in[HSM_ASYM_DATA_SIZE_VWB(modulusSizeBits)], modulusSizeBits, out_s);
}

/*
 *  ======== HSMLPF3_asymDsaSignatureToHW ========
 */
void HSMLPF3_asymDsaSignatureToHW(const uint8_t *const signature_r,
                                  const uint8_t *const signature_s,
                                  const size_t modulusSizeBits,
                                  uint8_t *const blob)
{
    /* Convert Signature from application to HW format */
    /* - Signature.r */
    HSMLPF3_asymBigIntToHw(signature_r, modulusSizeBits, 0, 2, blob);
    /* - Signature.s */
    HSMLPF3_asymBigIntToHw(signature_s, modulusSizeBits, 1, 2, &blob[HSM_ASYM_DATA_SIZE_VWB(modulusSizeBits)]);
}

/*
 *  ======== HSMLPF3_asymDsaPubKeyToHW ========
 */
void HSMLPF3_asymDsaPubKeyToHW(const uint8_t *const pubKey,
                               const size_t modulusSizeBits,
                               const uint8_t domainId,
                               uint8_t *const blob)
{
    uint8_t lastByte = 2 | (domainId << 4);

    /* Convert Public Key from OS format to HW format */
    /* - publicKey.x */
    HSMLPF3_asymBigIntToHw(pubKey, modulusSizeBits, 0, lastByte, blob);
    /* - publicKey.y */
    HSMLPF3_asymBigIntToHw(pubKey + BITS_TO_BYTES(modulusSizeBits),
                           modulusSizeBits,
                           1,
                           lastByte,
                           &blob[HSM_ASYM_DATA_SIZE_VWB(modulusSizeBits)]);
}

/*
 *  ======== HSMLPF3_asymDsaPriKeyToHW ========
 */
void HSMLPF3_asymDsaPriKeyToHW(const uint8_t *const priKey,
                               const size_t modulusSizeBits,
                               const uint8_t domainId,
                               uint8_t *const blob)
{
    uint8_t lastByte = 1 | (domainId << 4);

    /* Convert Private Key from OS format to HW format */
    HSMLPF3_asymBigIntToHw(priKey, modulusSizeBits, 0, lastByte, blob);
}