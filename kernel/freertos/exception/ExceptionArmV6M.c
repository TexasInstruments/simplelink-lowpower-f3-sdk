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

#include <stdint.h>
#include <string.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(cmsis/device.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)

#include <ti/log/Log.h>

#include <exception/Exception.h>

/* The C handler function needs to be public to link to them from the inline
 * assembly of the assembly handler shim.
 */
void Exception_handlerMaxC(Exception_ExceptionContext *exceptionContext, uint32_t exceptionReturn);
void Exception_handlerMinC(Exception_ExceptionContext *exceptionContext);
static void Exception_decodeNmi(void);
static void Exception_decodeHardFault(void);
static void Exception_decodeSvCall(void);
static void Exception_decodeReserved(void);
static void Exception_decodeNoFault(void);
static void Exception_dumpRegisters(uint32_t exceptionReturn, Exception_ExceptionContext *exceptionContext);

/* Weak empty exception hook function. Typically overridden by the application
 * to capture state and gracefully reset.
 */
__attribute__((weak)) void Exception_hookFxn(Exception_ExceptionContext *exceptionContext)
{
    /* Empty default hook fxn implementation. */
}

/*
 *  ======== Exception_handlerSpin ========
 */
void Exception_handlerSpin(void)
{
    __disable_irq();

    while (1) {}
}

/*
 *  ======== Exception_handlerMin ========
 */
void Exception_handlerMin(void)
{
    /* We need to code this in assembly to avoid the compiler pushing anything
     * to the stack if we were already on the MSP before we can get the
     * exception frame address and push sp, r4-r11 onto it as well.
     */
    __asm volatile(
        /* Disable interrupts */
        "cpsid  i                           \n"

        /* Did the exception occur on PSP? */
        "mov    r1,    lr                   \n"
        "movs   r2,     #4                  \n"
        "tst    r1,     r2                  \n"
        "beq    _MSPMIN                     \n"

        /* We were using the PSP */

        /* Save PSP to register to use it in computations. Save MSP for later
         * restoration when we want to switch back addresses.
         */
        "mrs    r0,     psp                 \n"
        "mrs    r1,     msp                 \n"

        /* Compute the original stack pointer when the exception occured */
        "movs   r3,     #32                 \n"
        "add    r3,     r0                  \n"

        /* Swap to PSP address so we can push new values onto the stack. Since
         * we are in handler mode, we are technically still running on MSP but
         * that is now pointing at the PSP.
         */
        "mov    sp,     r0                  \n"

        /* push instructions can only target r0-r7 on ARM v6m. Load r8-r11 into
         * registers and push one at a time.
         */
        "mov    r2,     r11                 \n"
        "push   {r2}                        \n"
        "mov    r2,     r10                 \n"
        "push   {r2}                        \n"
        "mov    r2,     r9                  \n"
        "push   {r2}                        \n"
        "mov    r2,     r8                  \n"
        "push   {r2}                        \n"

        /* Save sp, r4-r7 to the PSP. We can address the SP directly since the
         * SP is currently PSP.
         *
         * We now have sp, r4-r11 at the top of the stack followed by the
         * original exception frame r0-r3, r12, LR, PC, PSR.
         */
        "push   {r3-r7}                     \n"

        /* Pass updated PSP to function call */
        "mov    r0,     sp                  \n"

        /* Restore MSP to original value */
        "mov    sp,     r1                  \n"

        /* Branch to exception handler */
        "bl     Exception_handlerMinC       \n"

        /* We were on MSP when the exception occurred */
        "_MSPMIN:                           \n"

        /* Compute the original stack pointer when the exception occured */
        "movs   r3,     #32                 \n"
        "add    r3,     sp                  \n"

        /* push instructions can only target r0-r7 on ARM v6m. Load r8-r11 into
         * registers and push one at a time.
         */
        "mov    r2,     r11                 \n"
        "push   {r2}                        \n"
        "mov    r2,     r10                 \n"
        "push   {r2}                        \n"
        "mov    r2,     r9                  \n"
        "push   {r2}                        \n"
        "mov    r2,     r8                  \n"
        "push   {r2}                        \n"

        /* Save sp, r4-r7 to the MSP. We can address the SP directly since the
         * SP is currently MSP.
         *
         * We now have sp, r4-r11 at the top of the stack followed by the
         * original exception frame r0-r3, r12, LR, PC, PSR.
         */
        "push   {r3-r7}                     \n"

        /* Pass updated MSP to function call */
        "mov    r0,     sp                  \n"

        /* Branch to exception handler */
        "bl     Exception_handlerMinC       \n");
}

/*
 *  ======== Exception_handlerMinC ========
 */
void Exception_handlerMinC(Exception_ExceptionContext *exceptionContext)
{
    /* Call user's exception hook */
    Exception_hookFxn(exceptionContext);

    while (1) {}
}

/*
 *  ======== Exception_handlerMax ========
 */
void Exception_handlerMax(void)
{
    /* We need to code this in assembly to avoid the compiler pushing anything
     * to the stack if we were already on the MSP before we can get the
     * exception frame address and push sp, r4-r11 onto it as well.
     */
    __asm volatile(
        /* Disable interrupts */
        "cpsid  i                           \n"

        /* Did the exception occur on PSP? */
        "mov    r1,    lr                   \n"
        "movs   r2,     #4                  \n"
        "tst    r1,     r2                  \n"
        "beq    _MSPMAX                     \n"

        /* We were using the PSP */

        /* Save PSP to register to use it in computations. Save MSP for later
         * restoration when we want to switch back addresses.
         */
        "mrs    r0,     psp                 \n"
        "mrs    r1,     msp                 \n"

        /* Compute the original stack pointer when the exception occured */
        "movs   r3,     #32                 \n"
        "add    r3,     r0                  \n"

        /* Swap to PSP address so we can push new values onto the stack. Since
         * we are in handler mode, we are technically still running on MSP but
         * that is now pointing at the PSP.
         */
        "mov    sp,     r0                  \n"

        /* push instructions can only target r0-r7 on ARM v6m. Load r8-r11 into
         * registers and push one at a time.
         */
        "mov    r2,     r11                 \n"
        "push   {r2}                        \n"
        "mov    r2,     r10                 \n"
        "push   {r2}                        \n"
        "mov    r2,     r9                  \n"
        "push   {r2}                        \n"
        "mov    r2,     r8                  \n"
        "push   {r2}                        \n"

        /* Save sp, r4-r7 to the PSP. We can address the SP directly since the
         * SP is currently PSP.
         *
         * We now have sp, r4-r11 at the top of the stack followed by the
         * original exception frame r0-r3, r12, LR, PC, PSR.
         */
        "push   {r3-r7}                     \n"

        /* Pass updated PSP to function call */
        "mov    r0,     sp                  \n"

        /* Restore MSP to original value */
        "mov    sp,     r1                  \n"

        /* Pass exception return to Exception_handlerMaxC function call */
        "mov    r1,    lr                   \n"

        /* Branch to exception handler */
        "bl     Exception_handlerMaxC       \n"

        /* We were on MSP when the exception occurred */
        "_MSPMAX:                           \n"

        /* Compute the original stack pointer when the exception occured */
        "movs   r3,     #32                 \n"
        "add    r3,     sp                  \n"

        /* push instructions can only target r0-r7 on ARM v6m. Load r8-r11 into
         * registers and push one at a time.
         */
        "mov    r2,     r11                 \n"
        "push   {r2}                        \n"
        "mov    r2,     r10                 \n"
        "push   {r2}                        \n"
        "mov    r2,     r9                  \n"
        "push   {r2}                        \n"
        "mov    r2,     r8                  \n"
        "push   {r2}                        \n"

        /* Save sp, r4-r7 to the MSP. We can address the SP directly since the
         * SP is currently MSP.
         *
         * We now have sp, r4-r11 at the top of the stack followed by the
         * original exception frame r0-r3, r12, LR, PC, PSR.
         */
        "push   {r3-r7}                     \n"

        /* Pass updated MSP to function call */
        "mov    r0,     sp                  \n"

        /* Branch to exception handler */
        "bl     Exception_handlerMaxC       \n");
}

/*
 *  ======== Exception_handlerMaxC ========
 */
void Exception_handlerMaxC(Exception_ExceptionContext *exceptionContext, uint32_t exceptionReturn)
{
    uint32_t exceptionNumber = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);

    Log_printf(LogModule_Exception,
               Log_ERROR,
               "Exception_handlerMax: Exception encountered! Exception number = %d",
               exceptionNumber);

    switch (exceptionNumber)
    {
        case INT_NMI_FAULT:
            Exception_decodeNmi();
            break;
        case INT_HARD_FAULT:
            Exception_decodeHardFault();
            break;
        case INT_SVCALL:
            Exception_decodeSvCall();
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 12:
        case 13:
            Exception_decodeReserved();
            break;
        default:
            Exception_decodeNoFault();
            break;
    }

    Exception_dumpRegisters(exceptionReturn, exceptionContext);

    /* Call user's exception hook */
    Exception_hookFxn(exceptionContext);

    Log_printf(LogModule_Exception, Log_ERROR, "Exception_handlerMax: End of fault handler. Spinning until reset.");

    /* Spin forever until the device is reset. Typically, an application will
     * either use the exception hook to manually do so or wait for the watchdog.
     */
    while (1) {}
}

/*
 *  ======== Exception_decodeNmi ========
 */
static void Exception_decodeNmi(void)
{
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_decodeNmi: Non-maskable interrupt triggered exception handler. ");
}

/*
 *  ======== Exception_decodeHardFault ========
 */
static void Exception_decodeHardFault(void)
{
    Log_printf(LogModule_Exception,
               Log_ERROR,
               "Exception_decodeHardFault: Hard fault triggered.");
}

/*
 *  ======== Exception_decodeSvCall ========
 */
static void Exception_decodeSvCall(void)
{
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_decodeSvCall: Unexpected SvCall interrupt triggered exception handler. ");
}

/*
 *  ======== Exception_decodeReserved ========
 */
static void Exception_decodeReserved(void)
{
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_decodeReserved: Reserved exception.");
}

/*
 *  ======== Exception_decodeNoFault ========
 */
static void Exception_decodeNoFault(void)
{
    Log_printf(LogModule_Exception,
               Log_ERROR,
               "Exception_decodeNoFault: Unexpected exception triggered.");
}

/*
 *  ======== Exception_dumpRegisters ========
 */
static void Exception_dumpRegisters(uint32_t exceptionReturn, Exception_ExceptionContext *exceptionContext)
{
    if (exceptionReturn == EXC_RETURN_THREAD_PSP)
    {
        /* We were on PSP in thread mode, indicating an RTOS task */
        Log_printf(LogModule_Exception,
                    Log_ERROR,
                    "Exception_dumpRegisters: Exception occurred in thread mode on PSP, indicating RTOS task context.");
    }
    else if (exceptionReturn == EXC_RETURN_THREAD_MSP)
    {
        /* We were on MSP in thread mode, indicating the main function. */
        Log_printf(LogModule_Exception,
                    Log_ERROR,
                    "Exception_dumpRegisters: Exception occurred in thread mode on MSP, indicating pre-RTOS main context.");
    }
    else
    {
        /* We were in handler mode, indicating that we were executing from an
         * ISR
         */
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_dumpRegisters: Exception occurred in handler mode, indicating ISR context.");
    }

    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: R0 = 0x%x", exceptionContext->r0);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: R1 = 0x%x", exceptionContext->r1);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: R2 = 0x%x", exceptionContext->r2);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: R3 = 0x%x", exceptionContext->r3);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: R4 = 0x%x", exceptionContext->r4);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: R5 = 0x%x", exceptionContext->r5);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: R6 = 0x%x", exceptionContext->r6);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: R7 = 0x%x", exceptionContext->r7);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: R8 = 0x%x", exceptionContext->r8);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: R9 = 0x%x", exceptionContext->r9);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: R10 = 0x%x", exceptionContext->r10);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: R11 = 0x%x", exceptionContext->r11);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: R12 = 0x%x", exceptionContext->r12);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: SP (R13) = 0x%x", exceptionContext->sp);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: LP (R14) = 0x%x", exceptionContext->lr);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: PC (R15) = 0x%x", exceptionContext->pc);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: PSR = 0x%x", exceptionContext->psr);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: ICSR = 0x%x", SCB->ICSR);
}
