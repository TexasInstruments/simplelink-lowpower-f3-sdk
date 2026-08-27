/******************************************************************************
 *  Filename:       flash.c
 *
 *  Description:    Driver for the flash peripheral.
 *
 *  Copyright (c) 2026 Texas Instruments Incorporated
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

#include "flash.h"

#ifndef DRIVERLIB_NS

    // CC27xxx20 has 4 flash banks:
    //  - Bank 0 (0x000000–0x07FFFF)
    //  - Bank 1 (0x080000–0x0FFFFF)
    //  - Bank 2 (0x100000–0x17FFFF)
    //  - Bank 3 (0x180000–0x1FFFFF)
    // Macros for the bank 1/bank 2 and bank 2/bank 3 boundaries.
    #define FLASH_BANK2_BASE 0x100000U
    #define FLASH_BANK3_BASE 0x180000U

//*****************************************************************************
//
// FlashProgram
//
// Programs unprotected flash sectors. Handles multi-bank flash by internally
// splitting writes that would cross bank boundaries.
//
//*****************************************************************************
uint32_t FlashProgram(uint8_t *dataBuffer, uint32_t address, uint32_t count)
{
    uint32_t retCode;
    uint32_t cchctl;
    uint8_t *srcBuf    = dataBuffer;
    uint32_t dstAddr   = address;
    uint32_t remaining = count;
    uint32_t writeSize;
    uint32_t nextBankBoundary;
    uint32_t startAddr;

    // Compute which bank boundary will be hit first.
    // For TFM enabled applications, bit 28 indicates secure/non-secure address
    // space and must be cleared before comparing against bank bases.
    nextBankBoundary = UINT32_MAX;
    startAddr        = dstAddr & (~0x10000000U);

    if (startAddr < FLASH_BANK2_BASE)
    {
        nextBankBoundary = FLASH_BANK2_BASE;
    }
    else if (startAddr < FLASH_BANK3_BASE)
    {
        nextBankBoundary = FLASH_BANK3_BASE;
    }

    // HapiFlashProgram() will fail for any single call that spans
    // FLASH_BANK2_BASE or FLASH_BANK3_BASE. FlashProgram() internally
    // splits writes that cross these boundaries across multiple program
    // operations so the caller does not need to handle this.
    while (remaining)
    {
        writeSize = remaining;

        // Clamp writeSize to stay within current flash bank. When write lands
        // on boundary (writeSize >= bytesToBoundary), advance nextBankBoundary
        // so subsequent iterations pick up correct limit.
        uint32_t writeAddr       = dstAddr & (~0x10000000U);
        uint32_t bytesToBoundary = nextBankBoundary - writeAddr;

        if (writeSize >= bytesToBoundary)
        {
            writeSize        = bytesToBoundary;
            nextBankBoundary = (nextBankBoundary == FLASH_BANK2_BASE) ? FLASH_BANK3_BASE : UINT32_MAX;
        }

        // Store current configuration and clear instruction cache.
        cchctl                           = HWREG(VIMS_BASE + VIMS_O_CCHCTL);
        HWREG(VIMS_BASE + VIMS_O_CCHCTL) = (VIMS_CCHCTL_CCHEN_DIS | VIMS_CCHCTL_LINEN_DIS);

        retCode = HapiFlashProgram(FLASH_API_KEY, srcBuf, dstAddr, writeSize);

        // Restore configuration.
        HWREG(VIMS_BASE + VIMS_O_CCHCTL) = cchctl;

        // Check for error.
        if (retCode != FAPI_STATUS_SUCCESS)
        {
            return retCode;
        }

        // Advance pointers for next iteration.
        remaining -= writeSize;
        srcBuf += writeSize;
        dstAddr += writeSize;
    }

    return FAPI_STATUS_SUCCESS;
}

#endif // DRIVERLIB_NS
