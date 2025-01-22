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
/*
 *  ======== Exception.h ========
 */

#ifndef kernel_freertos_exception_Exception__include
#define kernel_freertos_exception_Exception__include

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*!
 * @brief Exception context struct
 *
 * Snapshot of the CPU register state when the exception was caused.
 */
typedef struct Exception_ExceptionContext
{
    uint32_t sp;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
} Exception_ExceptionContext;

/*!
 * @brief Application-provided callback function to be called after a fault
 *        occurs.
 *
 * This is an optional hook function that an application may implement to
 * receive a callback after a fault occurs.
 *
 * This function is called with interrupts disabled.
 *
 * This function may not make any calls to RTOS-dependent functions or the RTOS
 * itself.
 *
 * @param exceptionContext Pointer to struct containing a copy of the CPU
 *                         registers when the exception occured
 */
void Exception_hookFxn(Exception_ExceptionContext *exceptionContext);

/*!
 * @brief Exception handler implementation that decodes and logs the exception
 *        and calls a hook function.
 *
 * This exception handler will save the CPU state when the exception occurred
 * and fully decode the fault that triggered the exception. The log module used
 * by the handler only uses log level #Log_ERROR and can be routed only to
 * synchronous log sinks as RTOS services are not available in fault handlers.
 *
 * This handler will also call the application-configurable hook function
 * #Exception_hookFxn with interrupts disabled.
 *
 * If the application does not trigger a software reset from the hook function,
 * this handler will spin in a loop until the device is reset externally or by
 * the watchdog.
 */
void Exception_handlerMax(void);

/*!
 * @brief Exception handler implementation that decodes the exception and calls
 *        a hook function.
 *
 * This exception handler will decode the CPU state when the exception occured.
 * The handler will then call the application-configurable hook function
 * #Exception_hookFxn with interrupts disabled.
 *
 * If the application does not trigger a software reset from the hook function,
 * this handler will spin in a loop until the device is reset externally or by
 * the watchdog.
 */
void Exception_handlerMin(void);

/*!
 * @brief Exception handler implementation spins until the device is reset.
 *
 * This handler will only spin in a loop until the device is reset externally or
 * by the watchdog.
 */
void Exception_handlerSpin(void);

#endif /* kernel_freertos_exception_Exception__include */
