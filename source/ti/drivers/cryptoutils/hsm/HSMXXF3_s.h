/*
 * Copyright (c) 2024-2025, Texas Instruments Incorporated
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

#ifndef ti_drivers_cryptoutils_hsm_HSMXXF3_s__include
#define ti_drivers_cryptoutils_hsm_HSMXXF3_s__include

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief  Provisions the HUK to the HSM
 *
 *  @pre    HSMXXF3_init() to initialize and boot up the HSM.
 *
 *  @retval HSMXXF3_STATUS_SUCCESS                       Provisioning the HUK into the HSM was successful
 *  @retval HSMXXF3_STATUS_ERROR                         Provisioning the HUK failed
 *  @retval HSMXXF3_STATUS_RESOURCE_UNAVAILABLE          Error when acquiring a semaphore
 */
int_fast16_t HSMXXF3_provisionHUK_veneer(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_cryptoutils_hsm_HSMXXF3_s__include */
