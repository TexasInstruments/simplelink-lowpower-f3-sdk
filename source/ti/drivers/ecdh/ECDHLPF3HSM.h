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

#ifndef ti_drivers_ecdh_ECDHLPF3HSM__include
#define ti_drivers_ecdh_ECDHLPF3HSM__include

#include <stdint.h>
#include <stdbool.h>
#include <ti/drivers/ECDH.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief Hardware-specific configuration attributes
 *
 *  ECDHLPF3HSM hardware attributes are used in the board file by the
 *  #ECDH_Config struct.
 */
typedef struct
{
    uint8_t reserved1;
} ECDHLPF3HSM_HWAttrs;

/*!
 *  @brief ECDHLPF3HSM Object
 *
 *  \note The application must not access any member variables of this structure!
 */
typedef struct
{
    ECDH_Operation operation;
    uint32_t accessTimeout;
    int_fast16_t returnStatus;
    ECDH_CallbackFxn callbackFxn;
    /* This flag is only used in callback mode to signify that a
     * token has been submitted, but its corresponding callback
     * has not yet posted
     */
    bool operationInProgress;
    bool isOpen;
    ECDH_ReturnBehavior returnBehavior;
    ECDH_OperationType operationType;
} ECDHLPF3HSM_Object;

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_ecdh_ECDHLPF3HSM__include */