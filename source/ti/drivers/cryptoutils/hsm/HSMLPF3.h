/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
/**
 *  @file       HSMLPF3.h
 *
 *  @brief      Interface to initialize and shutdown the HSM
 *
 *  This module provides functions for use of the Hardware Security Module.
 *
 *  The HSM initialization procedure must first enable the clock, then initialize the
 *  mailbox, and finally boot the device.
 *
 *  Crypto drivers that use the HSM may vary in usage of either HSMLPF3_Clock_Disable()
 *  or HSMLPF3_Sleep() when closing their handles.
 *
 *
 *
 */

/*!
 *  @brief   Successful status code.
 *
 *  Functions return HSMLPF3_STATUS_SUCCESS if the function was executed
 *  successfully.
 */
#define HSMLPF3_STATUS_SUCCESS ((int_fast16_t)0)

/*!
 *  @brief   Generic error status code.
 *
 *  Functions return HSMLPF3_STATUS_ERROR if the function was not executed
 *  successfully and no more specific error is applicable.
 */
#define HSMLPF3_STATUS_ERROR ((int_fast16_t)-1)

/*!
 *  @brief  Enables clock for HSM, readying it for boot.
 *
 *  Calling this function is the first step to making use of the
 *  HSM for crypto operations.
 */
void HSMLPF3_enableClock(void);

/*!
 *  @brief  Disables clock for HSM, effectively powering it off.
 *
 *  This function provides an alternative to putting the HSM in sleep
 *  when it no longer needs to be used.
 */
void HSMLPF3_disableClock(void);

/*!
 *  @brief  Boots the HSM, readying it for crypto operations.
 *
 *  This function submits a boot token to the HSM, and waits for a
 *  successful token result and for the firmware image to be accepted.
 *
 *  Calling this function when the HSM is already booted should return
 *  successfully without an unnecessary token submission.
 *
 *  @pre    #HSMLPF3_enableClock() has to be called first.
 *  @pre    #HSMLPF3_initMbox() has to be called first
 *
 *  @retval #HSMLPF3_STATUS_SUCCESS               HSM booted successfully.
 *  @retval #HSMLPF3_STATUS_ERROR                 Error. Error output token, or HSM in bad state.
 */
int_fast16_t HSMLPF3_boot(void);

/*!
 *  @brief  Initializes HSM mailbox, allowing token exchanges.
 *
 *  Calling this function allows for the boot token and all future tokens
 *  to be sent to the HSM. HSMLPF3_initMbox() links to the mailbox(es), and
 *  specifies lockout and secure/non-secure access.
 */
void HSMLPF3_initMbox(void);

/*!
 *  @brief  Puts the HSM to sleep, lowering its power consumption.
 *
 *  This function submits a sleep token to the HSM, and waits for a
 *  successful token result.
 *
 *  Calling this function when the HSM is already asleep will still
 *  submit a sleep token.
 *
 *  @pre    #HSMLPF3_boot() has to be called first.
 *
 *  @retval #HSMLPF3_STATUS_SUCCESS               HSM put to sleep successfully.
 *  @retval #HSMLPF3_STATUS_ERROR                 Error. Error output token, or HSM in bad state.
 */
int_fast16_t HSMLPF3_sleep(void);