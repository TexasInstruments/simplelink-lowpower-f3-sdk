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

/* Processor mode when exception occurred. 0 for handler mode and 1 for thread
 * mode
 */
#define EXC_RETURN_MODE  (0x00000008UL)
/* Stack pointer used when exception occurred. 0 for MSP and 1 for PSP */
#define EXC_RETURN_SPSEL (0x00000004UL)

/* The C handler function needs to be public to link to them from the inline
 * assembly of the assembly handler shim.
 */
void Exception_handlerMaxC(Exception_ExceptionContext *exceptionContext, uint32_t exceptionReturn);
void Exception_handlerMinC(Exception_ExceptionContext *exceptionContext);

static void Exception_decodeNmi(void);
static void Exception_decodeHardFault(Exception_ExceptionContext *exceptionContext);
static void Exception_decodeMemFault(Exception_ExceptionContext *exceptionContext);
static void Exception_decodeBusFault(Exception_ExceptionContext *exceptionContext);
static void Exception_decodeUsageFault(Exception_ExceptionContext *exceptionContext);
static void Exception_decodeDebugMon(void);
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

        /* Check whether we need to offset the SP for the regular or extended
         * exception frame
         */
        "tst    lr,     #16                 \n"
        "ite    NE                          \n"
        "movne  r3,     #32                 \n"
        "moveq  r3,     #104                \n"

        /* Did the exception occur on PSP? */
        "tst     lr,    #4                  \n"
        "beq    _MSPMIN                     \n"

        /* We were using the PSP */

        /* We need to manipulate the PSP address in a register and then write it
         * back since we cannot manipulate the PSP / MSP registers directly
         */
        "mrs    r0,     psp                 \n"

        /* Compute the original stack pointer when the exception occured */
        "add    r3,     r0                  \n"

        /* Save sp, r4-r11 to the PSP. We now have sp, r4-r11 at the top of the
         * stack followed by the original exception frame r0-r3, r12, LR, PC, PSR
         */
        "stmfd  r0!,    {r3-r11}            \n"

        /* Update PSP register with address after pushing sp, r4-r11 */
        "msr    psp,    r0                  \n"

        /* Branch to exception handler */
        "bl     Exception_handlerMinC       \n"

        /* We were on MSP when the exception occurred */
        "_MSPMIN:                           \n"

        /* Compute the original stack pointer when the exception occured */
        "add    r3,     sp                  \n"

        /* Save sp, r4-r11 to the MSP. We can address the SP directly since the
         * SP is currently MSP.
         *
         * We now have sp, r4-r11 at the top of the stack followed by the
         * original exception frame r0-r3, r12, LR, PC, PSR.
         */
        "stmfd  sp!,    {r3-r11}            \n"

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

        /* Pass exception return to Exception_handlerMaxC function call */
        "mov     r1,    lr                  \n"

        /* Check whether we need to offset the SP for the regular or extended
         * exception frame
         */
        "tst    lr,     #16                 \n"
        "ite    NE                          \n"
        "movne  r3,     #32                 \n"
        "moveq  r3,     #104                \n"

        /* Did the exception occur on PSP? */
        "tst     lr,    #4                  \n"
        "beq    _MSPMAX                     \n"

        /* We were using the PSP */

        /* We need to manipulate the PSP address in a register and then write it
         * back since we cannot manipulate the PSP / MSP registers directly
         */
        "mrs    r0,     psp                 \n"

        /* Compute the original stack pointer when the exception occured */
        "add    r3,     r0                  \n"

        /* Save sp, r4-r11 to the PSP. We now have sp, r4-r11 at the top of the
         * stack followed by the original exception frame r0-r3, r12, LR, PC, PSR
         */
        "stmfd  r0!,    {r3-r11}            \n"

        /* Update PSP register with address after pushing sp, r4-r11 */
        "msr    psp,    r0                  \n"

        /* Branch to exception handler */
        "bl     Exception_handlerMaxC       \n"

        /* We were on MSP when the exception occurred */
        "_MSPMAX:                           \n"

        /* Compute the original stack pointer when the exception occured */
        "add    r3,     sp                  \n"

        /* Save sp, r4-r11 to the MSP. We can address the SP directly since the
         * SP is currently MSP.
         *
         * We now have sp, r4-r11 at the top of the stack followed by the
         * original exception frame r0-r3, r12, LR, PC, PSR.
         */
        "stmfd  sp!,    {r3-r11}            \n"

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
            Exception_decodeHardFault(exceptionContext);
            break;
        case INT_MEMMANAGE_FAULT:
            Exception_decodeMemFault(exceptionContext);
            break;
        case INT_BUS_FAULT:
            Exception_decodeBusFault(exceptionContext);
            break;
        case INT_USAGE_FAULT:
            Exception_decodeUsageFault(exceptionContext);
            break;
        case INT_SVCALL:
            Exception_decodeSvCall();
            break;
        case INT_DEBUG:
            Exception_decodeDebugMon();
            break;
        case 7:
        case 8:
        case 9:
        case 10:
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
static void Exception_decodeHardFault(Exception_ExceptionContext *exceptionContext)
{
    if (SCB->HFSR & SCB_HFSR_FORCED_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeHardFault: HFSR indicates a configurable fault was escalated to a HardFault (FORCED).");
        Exception_decodeUsageFault(exceptionContext);
        Exception_decodeBusFault(exceptionContext);
        Exception_decodeMemFault(exceptionContext);
    }
    else if (SCB->HFSR & SCB_HFSR_DEBUGEVT_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeHardFault: HFSR indicates a debug event while the debug subsystem was disabled. (DEBUGEVT)");
        Exception_decodeDebugMon();
    }
    else if (SCB->HFSR & SCB_HFSR_VECTTBL_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeHardFault: HFSR indicates a error reading from the vector table. (VECTBL)");
    }
}

/*
 *  ======== Exception_decodeMemFault ========
 */
static void Exception_decodeMemFault(Exception_ExceptionContext *exceptionContext)
{
    uint8_t mmfsr = SCB->CFSR & SCB_CFSR_MEMFAULTSR_Msk;

    /* Decode MMFSR to determinte what kind of MemFault it is. */
    if (mmfsr & SCB_CFSR_MSTKERR_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeMemFault: MMFSR indicates a stacking error occured (RD/WR failed), stack push. (MSTKERR)");
    }
    else if (mmfsr & SCB_CFSR_MUNSTKERR_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeMemFault: MMFSR indicates an unstacking error occured (RD/WR failed), stack pop. (MUNSTKERR)");
    }
    else if (mmfsr & SCB_CFSR_DACCVIOL_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeMemFault: MMFSR indicates a data accessed error occured (RD/WR failed). (DACCVIOL)");
    }
    else if (mmfsr & SCB_CFSR_IACCVIOL_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeMemFault: MMFSR indicates an MPU or ExecuteNever (XN) fault. (IACCVIOL)");
    }

    /* If MMFAR is valid, print it */
    if (mmfsr & SCB_CFSR_MMARVALID_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeMemFault: MMFSR indicates valid MMFAR. MemFault triggered by address 0x%x",
                   SCB->MMFAR);
    }
}

/*
 *  ======== Exception_decodeBusFault ========
 */
static void Exception_decodeBusFault(Exception_ExceptionContext *exceptionContext)
{
    uint8_t bfsr = (SCB->CFSR & SCB_CFSR_BUSFAULTSR_Msk) >> SCB_CFSR_BUSFAULTSR_Pos;

    /* Decode BFSR to determinte what kind of MemFault it is. */
    if (bfsr & SCB_CFSR_STKERR_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeBusFault: BusFault caused by stack push. (STKERR)");
    }
    else if (bfsr & SCB_CFSR_UNSTKERR_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeBusFault: BusFault caused by stack pop. (UNSTKERR)");
    }
    else if (bfsr & SCB_CFSR_IMPRECISERR_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeBusFault: Delayed BusFault, exact addr unknown. (IMPRECISERR)");
    }
    else if (bfsr & SCB_CFSR_PRECISERR_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeBusFault: Immediate BusFault, exact addr known. (PRECISERR)");
    }
    else if (bfsr & SCB_CFSR_IBUSERR_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeBusFault: Instruction Access Violation. (IBUSERR)");
    }

    /* If BFAR is valid, print it */
    if (bfsr & SCB_CFSR_BFARVALID_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeBusFault: BFSR indicates valid BFAR. BusFault triggered by address 0x%x",
                   SCB->BFAR);
    }
}

/*
 *  ======== Exception_decodeUsageFault ========
 */
static void Exception_decodeUsageFault(Exception_ExceptionContext *exceptionContext)
{
    uint8_t ufsr = (SCB->CFSR & SCB_CFSR_USGFAULTSR_Msk) >> SCB_CFSR_USGFAULTSR_Pos;

    if (ufsr & SCB_CFSR_UNDEFINSTR_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeUsageFault: Undefined instruction executed. (UNDEFINSTR)");
    }
    else if (ufsr & SCB_CFSR_INVSTATE_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeUsageFault: Invalid EPSR and instruction combination (INVSTATE)");
    }
    else if (ufsr & SCB_CFSR_INVPC_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeUsageFault: Invalid PC (INVPC)");
    }
    else if (ufsr & SCB_CFSR_NOCP_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeUsageFault: Attempting to use co-processor when it is not present or disabled. (NOCP)");
    }
    else if (ufsr & SCB_CFSR_UNALIGNED_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeUsageFault: Unaligned memory access. (UNALIGNED)");
    }
    else if (ufsr & SCB_CFSR_DIVBYZERO_Msk)
    {
        Log_printf(LogModule_Exception,
                   Log_ERROR,
                   "Exception_decodeUsageFault: Divided by zero. (DIVBYZERO)");
    }
}

/*
 *  ======== Exception_decodeDebugMon ========
 */
static void Exception_decodeDebugMon(void)
{
    if (SCB->DFSR & SCB_DFSR_EXTERNAL_Msk)
    {
        Log_printf(LogModule_Exception, Log_ERROR, "Exception_decodeDebugMon: EXTERNAL");
    }
    else if (SCB->DFSR & SCB_DFSR_VCATCH_Msk)
    {
        Log_printf(LogModule_Exception, Log_ERROR, "Exception_decodeDebugMon: VCATCH");
    }
    else if (SCB->DFSR & SCB_DFSR_DWTTRAP_Msk)
    {
        Log_printf(LogModule_Exception, Log_ERROR, "Exception_decodeDebugMon: DWTTRAP");
    }
    else if (SCB->DFSR & SCB_DFSR_BKPT_Msk)
    {
        Log_printf(LogModule_Exception, Log_ERROR, "Exception_decodeDebugMon: BKPT");
    }
    else if (SCB->DFSR & SCB_DFSR_HALTED_Msk)
    {
        Log_printf(LogModule_Exception, Log_ERROR, "Exception_decodeDebugMon: HALTED");
    }
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
    /* Check if EXC_RETURN::Mode indicates thread mode or handler mode */
    if (exceptionReturn & EXC_RETURN_MODE)
    {
        /* We were in thread mode */
        if (exceptionReturn & EXC_RETURN_SPSEL)
        {
            /* We were on PSP, indicating an RTOS task */
            Log_printf(LogModule_Exception,
                       Log_ERROR,
                       "Exception_dumpRegisters: Exception occurred in thread mode on PSP, indicating RTOS task context.");
        }
        else
        {
            /* We were on MSP in thread mode, indicating the main function. */
            Log_printf(LogModule_Exception,
                       Log_ERROR,
                       "Exception_dumpRegisters: Exception occurred in thread mode on MSP, indicating pre-RTOS main context.");
        }
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
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: MMFSR = 0x%x", SCB->CFSR & 0xFFU);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: BFSR = 0x%x", (SCB->CFSR >> 8) & 0xFFU);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: UFSR = 0x%x", (SCB->CFSR >> 16) & 0xFFFFFU);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: HFSR = 0x%x", SCB->HFSR);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: DFSR = 0x%x", SCB->DFSR);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: MMFAR = 0x%x", SCB->MMFAR);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: BFAR = 0x%x", SCB->BFAR);
    Log_printf(LogModule_Exception, Log_ERROR, "Exception_dumpRegisters: AFSR = 0x%x", SCB->AFSR);
}
