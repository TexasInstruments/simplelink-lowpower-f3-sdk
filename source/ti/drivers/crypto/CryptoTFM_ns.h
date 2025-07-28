/*
 * Copyright (c) 2022-2025, Texas Instruments Incorporated - http://www.ti.com
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
 *  @file       CryptoTFM_ns.h
 *
 *  @brief      Shared resources to arbitrate access to Crypto engines on Secure/Nonsecure devices
 *
 */

#ifndef ti_drivers_crypto_CryptoTFM_ns__include
#define ti_drivers_crypto_CryptoTFM_ns__include

#include <stdint.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#include <ti/devices/DeviceFamily.h>

#include <psa/client.h>

/*! @cond NODOC */

extern SemaphoreP_Struct CryptoTFM_ns_accessSemaphore;
extern SemaphoreP_Struct CryptoTFM_ns_operationSemaphore;

/*!
 *  @brief   Non-secure client wrapper to make PSA calls to Crypto secure service with
 *           a single input and single output vector.
 *           Disables context switching around psa_call()
 *
 *  @pre     CryptoTFM_ns_init() and CryptoTFM_ns_acquireLock() must be called first
 *
 *  @param type      Secure message type
 *  @param invecs    Input vector pointer to secure message struct
 *  @param outvecs   Output vector pointer to storage for return value
 *  @return          Returns 0 on success, or -129 on PSA programmer error
 */
psa_status_t CryptoTFM_ns_call(int32_t type, psa_invec *invecs, psa_outvec *outvecs);

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*!
 *  @brief   Non-secure client wrapper to make PSA calls to Crypto secure service.
 *           Disables context switching and acquires access semaphore around psa_call()
 *
 *  @note    This API is for polling operations only.
 *
 *  @pre     CryptoTFM_ns_init() must be called first
 *
 *  @param handle     A dummy handle which is discarded to use TI crypto connection handle.
 *  @param type       The request type. Must be zero( PSA_IPC_CALL) or positive
 *  @param invecs     Input vector pointer to secure message struct
 *  @param numInvecs  Number of input vector structs
 *  @param outvecs    Output vector pointer to storage for return value
 *  @param numOutvecs Number of output vector structs
 *  @return           Returns 0 on success, or -129 on PSA programmer error, or -137 if the crypto engine is in use.
 */
psa_status_t CryptoTFM_ns_dispatch(psa_handle_t handle,
                                   int32_t type,
                                   psa_invec *invecs,
                                   size_t numInvecs,
                                   psa_outvec *outvecs,
                                   size_t numOutvecs);

/*!
 * @brief     Retrieve the version string of the TF-M image.
 *
 * @param[in] buf        Pointer to output buffer to write version string.
 * @param[in,out] size   Size of the output buffer. Updated with actual size written.
 *
 * @retval 0             Operation succeeded.
 * @retval -1            The buffer is not in NSPE with R/W access.
 *
 */
int32_t tfm_get_version(void *buf, size_t *size);

#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

/*!
 *  @brief   Initialization function constructs semaphores and inits SecureCB driver
 */
void CryptoTFM_ns_init(void);

/*!
 *  @brief   Get the CryptoTFM_ns_accessSemaphore (callback and blocking modes)
 *           Returns bool False on timeout.
 *
 *  @pre     CryptoTFM_ns_init() must be called first
 *
 *  @param timeout   Semaphore timeout value
 *  @return          Returns True on success, False on timeout
 */
bool CryptoTFM_ns_acquireLock(uint32_t timeout);

/*!
 *  @brief   Post the CryptoTFM_ns_accessSemaphore (callback and blocking modes)
 *
 *  @pre     CryptoTFM_ns_init() must be called first
 */
static inline void CryptoTFM_ns_releaseLock(void)
{
    SemaphoreP_post(&CryptoTFM_ns_accessSemaphore);
}

/*! @endcond */

#endif /* ti_drivers_crypto_CryptoTFM_ns__include */