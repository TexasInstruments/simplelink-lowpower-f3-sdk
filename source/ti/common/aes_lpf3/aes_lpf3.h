/******************************************************************************

 *  @file  aes_lpf3.h
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

#ifndef aes_lpf3__include
#define aes_lpf3__include

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ti/common/ecdsa_lpf3/ecdsa_lpf3.h"
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/aes.h)
#include DeviceFamily_constructPath(driverlib/udma.h)
#include DeviceFamily_constructPath(driverlib/lrfd.h)
#include DeviceFamily_constructPath(inc/hw_clkctl.h)
#include DeviceFamily_constructPath(inc/hw_evtsvt.h)

#ifdef __cplusplus
extern "C" {
#endif
/* Defines */

/*!
 * @brief   Successful status code.
 *
 * Functions return #AES_STATUS_SUCCESS if the function was executed
 * successfully.
 */
#define AES_STATUS_SUCCESS ((int_fast16_t)0)

/*!
 * @brief   Generic error status code.
 *
 * Functions return #AES_STATUS_ERROR if the function was not executed
 * successfully and no more pertinent error code could be returned.
 */
#define AES_STATUS_ERROR ((int_fast16_t)-1)


#define RESOURCE_BIT_INDEX(resourceId) ((resourceId) & (0x00FFU))
#define PowerLPF3_PERIPH_DMA ((0x0000U) | CLKCTL_DESCEX0_DMA_S)
#define PowerLPF3_PERIPH_AES ((0x0000U) | CLKCTL_DESCEX0_LAES_S)


/*!
 *  @brief  Struct containing the parameters required for encrypting/decrypting
 *          a message using a one-step operation.
 *
 *  The driver may access it at any point during the operation. It must remain
 *  in scope for the entire duration of the operation.
 */
typedef struct
{
    const CryptoKey_Plaintext *key; /*!< Pointer to a previously initialized CryptoKey. */
    const uint8_t *input;           /*!<
                                     *   - Encryption: The plaintext buffer to be
                                     *     encrypted in the CTR operation.
                                     *   - Decryption: The ciphertext to be decrypted.
                                     */
    uint8_t *output;                /*!<
                                     *   - Encryption: The output ciphertext buffer that
                                     *     the encrypted plaintext is copied to.
                                     *   - Decryption: The plaintext derived from the
                                     *     decrypted ciphertext is copied here.
                                     *   Size of the output buffer must be greater than
                                     *   or equal to the inputLength.
                                     */
    const uint8_t *initialCounter;  /*!< A buffer containing an initial counter. Under
                                     *   the same key, each counter value may only be
                                     *   used to encrypt or decrypt a single input
                                     *   block. If NULL, zero will be used for the
                                     *   initial counter value. The buffer's size must
                                     *   be at least 16-bytes.
                                     */
    size_t inputLength;             /*!< Length of the input in bytes. An equal number
                                     *   of bytes will be output by the operation.
                                     *   Max length supported may be limited depending on
                                     *   the return behavior.
                                     */
} AESCTR_OneStepOperation;

int_fast16_t AESCTR_processData(AESCTR_OneStepOperation *operation);

int_fast16_t AES_open();

int_fast16_t AES_close(void);

void AES_cancel(void);

#ifdef __cplusplus
}
#endif

#endif /* ecdsa_lpf3__include */
