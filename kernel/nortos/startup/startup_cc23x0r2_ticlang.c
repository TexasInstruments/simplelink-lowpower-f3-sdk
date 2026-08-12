/*
 * Copyright (c) 2022-2024, Texas Instruments Incorporated
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

//*****************************************************************************
//
// Check if compiler is TICLANG
//
//*****************************************************************************
#if !(defined(__clang__))
    #error "startup_cc23x0r2_ticlang.c: Unsupported compiler!"
#endif

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/setup.h)
#include DeviceFamily_constructPath(driverlib/interrupt.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(cmsis/cc23x0r2.h)
#include DeviceFamily_constructPath(cmsis/core/core_cm0plus.h)

//*****************************************************************************
//
//! Forward declaration of the reset ISR and the default fault handlers.
//
//*****************************************************************************
void resetISR(void);
static void nmiISR(void);
static void faultISR(void);
static void intDefaultHandler(void);

//*****************************************************************************
//
//! The entry point for the application startup code.
//
//*****************************************************************************
extern void _c_int00(void);

//*****************************************************************************
//
// Linker variable that marks the top of the stack.
//
//*****************************************************************************
extern unsigned long __STACK_END;

//! The vector table. Note that the proper constructs must be placed on this to
//! ensure that it ends up at physical address 0x0000.0000 or at the start of
//! the program if located at a start address other than 0.
//
//*****************************************************************************
__attribute__((section(".resetVecs"), retain)) void (*const resetVectors[])(void) = {
    (void (*)(void))((unsigned long)&__STACK_END),
    resetISR,          //  1 The reset handler
    nmiISR,            //  2 The NMI handler
    faultISR,          //  3 The hard fault handler
    intDefaultHandler, //  4 Reserved
    intDefaultHandler, //  5 Reserved
    intDefaultHandler, //  6 The usage fault handler
    intDefaultHandler, //  7 Reserved
    intDefaultHandler, //  8 Reserved
    intDefaultHandler, //  9 Reserved
    intDefaultHandler, // 10 Reserved
    intDefaultHandler, // 11 SVCall handler
    intDefaultHandler, // 12 Debug monitor handler
    intDefaultHandler, // 13 Reserved
    intDefaultHandler, // 14 The PendSV handler
    intDefaultHandler, // 15 The SysTick handler
    intDefaultHandler, // 16 CPUIRQ0
    intDefaultHandler, // 17 CPUIRQ1
    intDefaultHandler, // 18 CPUIRQ2
    intDefaultHandler, // 19 CPUIRQ3
    intDefaultHandler, // 20 CPUIRQ4
    intDefaultHandler, // 21 GPIO event interrupt
    intDefaultHandler, // 22 LRF interrupt 0
    intDefaultHandler, // 23 LRF interrupt 1
    intDefaultHandler, // 24 uDMA done events
    intDefaultHandler, // 25 AES interrupt
    intDefaultHandler, // 26 SPI0 combined interrupt
    intDefaultHandler, // 27 UART0 combined interrupt
    intDefaultHandler, // 28 I2C0 combined interrupt
    intDefaultHandler, // 29 LGPT0 interrupt
    intDefaultHandler, // 30 LGPT1 interrupt
    intDefaultHandler, // 31 ADC interrupt
    intDefaultHandler, // 32 CPUIRQ16
    intDefaultHandler, // 33 LGPT2 interrupt
    intDefaultHandler, // 34 LGPT3 interrupt
};

//*****************************************************************************
//
//! This is the code that gets called when the processor first starts execution
//! following a reset event. Only the absolutely necessary set is performed,
//! after which the application supplied entry() routine is called. Any fancy
//! actions (such as making decisions based on the reset cause register, and
//! resetting the bits in that register) are left solely in the hands of the
//! application.
//
//*****************************************************************************
void resetISR(void)
{
    IntDisableMaster();

    //
    // Final trim of device
    //
    SetupTrimDevice();

    //
    // Jump to the C Initialization Routine.
    //
    __asm(" .global _c_int00\n"
          " bl      _c_int00");

    //
    // If we ever return signal Error
    //
    faultISR();
}

//*****************************************************************************
//
//! This is the code that gets called when the processor receives a NMI. This
//! simply enters an infinite loop, preserving the system state for examination
//! by a debugger.
//
//*****************************************************************************
static void nmiISR(void)
{
    //
    // Enter an infinite loop.
    //
    while (1) {}
}

//*****************************************************************************
//
//! This is the code that gets called when the processor receives a fault
//! interrupt. This simply enters an infinite loop, preserving the system state
//! for examination by a debugger.
//
//*****************************************************************************
static void faultISR(void)
{
    //
    // Enter an infinite loop.
    //
    while (1) {}
}

//*****************************************************************************
//
//! This is the code that gets called when the processor receives an unexpected
//! interrupt. This simply enters an infinite loop, preserving the system state
//! for examination by a debugger.
//
//*****************************************************************************
static void intDefaultHandler(void)
{
    //
    // Enter an infinite loop.
    //
    while (1) {}
}
