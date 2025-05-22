/*
 * Copyright (c) 2025 Texas Instruments Incorporated - http://www.ti.com
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

/*
 *  ======== SecureCallback_s.h ========
 */

#ifndef ti_drivers_tfm_SecureCallback_s__include
#define ti_drivers_tfm_SecureCallback_s__include

#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 *  @brief  Triggers the dispatcher targeting the specified callback.
 *
 *  @note   This API is only available to secure code.
 *
 *  @param[in]  handle  A #SecureCallback_Handle pointer.
 *
 *  @pre    The callback object must have been created by
 *          #SecureCallback_construct().
 */
void SecureCallback_s_post(SecureCallback_Handle handle);

/*!
 *  @brief  Immediately dispatches the specified callback, bypassing the
 *          SecureCallback's Hwi callback function.
 *
 *  @warning This function un-pends the #SecureCallback_Object. Care must be
 *          taken if both #SecureCallback_s_postSynchronous() and
 *          #SecureCallback_s_post() are used for the same
 *          #SecureCallback_Object.
 *
 *  @note   This API is only available to secure code.
 *
 *  @param[in]  handle  A #SecureCallback_Handle pointer.
 *
 *  @pre    The callback object must have been created by
 *          #SecureCallback_construct().
 */
void SecureCallback_s_postSynchronous(SecureCallback_Handle handle);

#if defined(__cplusplus)
}
#endif

#endif /* ti_drivers_tfm_SecureCallback_s__include */
