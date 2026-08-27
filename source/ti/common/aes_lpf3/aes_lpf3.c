/******************************************************************************

 *  @file  aes_lpf3.c
 *
 *  @brief      AES CTR driver implementation for MCUboot

 Group: CMCU
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025-2026, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

#include <stdint.h>
#include <stddef.h>

#include "aes_lpf3.h"

/*
 *  ======== AESCTR_processData ========
 */
int_fast16_t AESCTR_processData(AESCTR_OneStepOperation *operation)
{
    int_fast16_t status = AES_STATUS_SUCCESS;
    const uint32_t *input32[operation->inputLength / 4];

    /* Only plaintext is supported currently */
    uint8_t * keyingMaterial = operation->key->keyMaterial;

    memcpy(input32, operation->input, operation->inputLength);

    if(!IS_WORD_ALIGNED(operation->output))
    {
        return -1;
    }

    AESWriteKEY(keyingMaterial);
    AESSetAUTOCFG(((uint32_t)AES_AUTOCFG_AESSRC_BUF | (uint32_t)AES_AUTOCFG_TRGAES_WRBUF3S | (uint32_t)AES_AUTOCFG_TRGAES_RDTXT3 | (uint32_t)AES_AUTOCFG_CTRSIZE_CTR128 | (uint32_t) AES_AUTOCFG_CTRENDN_BIGENDIAN  | (uint32_t)AES_AUTOCFG_BUSHALT_EN));

    AESWriteBUF32((const uint32_t *)operation->initialCounter);

    size_t blockSizeAlignedBytesRemaining = AES_GET_NUM_BLOCKS(AES_BLOCK_SIZE_MULTIPLE_LENGTH(operation->inputLength));

    AESProcessAlignedBlocksCTR((const uint32_t *) input32, (uint32_t *)operation->output , blockSizeAlignedBytesRemaining);

    AESAbort();
    return status;
}

/*
 *  ======== AES_open ========
 */
int_fast16_t AES_open()
{

    // power up and enable clock for dma
    uint8_t bitIndex = RESOURCE_BIT_INDEX(PowerLPF3_PERIPH_AES);
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENSET0) = 1U << bitIndex;

    //power up and enable clock for LAES
    bitIndex = RESOURCE_BIT_INDEX(PowerLPF3_PERIPH_AES);
    LRFDSetClockDependency(1U << bitIndex, LRFD_CLK_DEP_POWER);

    // IntDisableMaster();
    return AES_STATUS_SUCCESS;
}

/*
 *  ======== AES_close ========
 */
int_fast16_t AES_close(void)
{
    // power up and enable clock for dma
    uint8_t bitIndex = RESOURCE_BIT_INDEX(PowerLPF3_PERIPH_AES);
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENCLR0) = 1U << bitIndex;

    //power up and enable clock for LAES
    bitIndex = RESOURCE_BIT_INDEX(PowerLPF3_PERIPH_AES);
    LRFDReleaseClockDependency(1U << bitIndex, LRFD_CLK_DEP_POWER);

    // IntDisableMaster();
    return AES_STATUS_SUCCESS;
}

void AES_cancel(void)
{
    AESAbort();
}

