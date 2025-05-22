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
 *  ======== SecureCallback_s.c ========
 */
#include <arm_cmse.h> /* ARM Cortex-M Security Extension intrinsics */

#include "SecureCallback.h"
#include "SecureCallback_s.h"

/* Driverlib includes*/
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/interrupt.h)

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)
    #define IntSetPend IntPendSet
#endif

/* Non-secure function declaration */
typedef void (*SecureCallback_s_nsFuncPtr)(uintptr_t arg) __attribute__((cmse_nonsecure_call));

/* This object must be provided within the secure image and must implement the
 * same interrupt number which was used to build the application image.
 */
extern const SecureCallback_Config SecureCallback_config;

/*
 *  ======== SecureCallback_s_post ========
 */
void SecureCallback_s_post(SecureCallback_Handle handle)
{
    handle->pending = true;
    IntSetPend(SecureCallback_config.intNumber);
}

/*
 *  ======== SecureCallback_s_postSynchronous ========
 */
void SecureCallback_s_postSynchronous(SecureCallback_Handle handle)
{
    SecureCallback_s_nsFuncPtr nsFuncPtr;

    /* Bypassing the SecureCallback's Hwi callback function, so make sure the
     * pending flag is cleared.
     */
    handle->pending = false;

    /* Create a non-secure function pointer. */
    nsFuncPtr = (SecureCallback_s_nsFuncPtr)cmse_nsfptr_create(handle->fxn);

    if (cmse_is_nsfptr(nsFuncPtr))
    {
        /* Call the non-secure function. */
        nsFuncPtr(handle->arg);
    }
}
