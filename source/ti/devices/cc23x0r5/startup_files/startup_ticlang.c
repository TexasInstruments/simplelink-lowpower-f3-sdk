/******************************************************************************
 *  Filename:       startup_ticlang.c
 *
 *  Description:    Startup code for CC23X0R5 device family for use with
 *                  TI Clang/LLVM.
 *
 *  Copyright (c) 2022 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1) Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2) Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *  3) Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

//*****************************************************************************
//
// Check if compiler is TICLANG
//
//*****************************************************************************
#if !(defined(__clang__))
    #error "startup_ticlang.c: Unsupported compiler!"
#endif

#include "../inc/hw_types.h"
#include "../driverlib/setup.h"

//*****************************************************************************
//
//! Forward declaration of the reset ISR and the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);
static void NmiISR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);
extern int main(void);

//*****************************************************************************
//
// The entry point for the application startup code and device trim fxn.
//
//*****************************************************************************
extern void _c_int00(void);

//*****************************************************************************
//
// TIClang: Linker variable that marks the top of the stack.
//
//*****************************************************************************
extern unsigned long __STACK_END;

//*****************************************************************************
//
//! The vector table. Note that the proper constructs must be placed on this to
//! ensure that it ends up at physical address 0x0000.0000 or at the start of
//! the program if located at a start address other than 0.
//
//*****************************************************************************
__attribute__((section(".resetVecs"), retain)) void (*const resetVectors[])(void) = {
    (void (*)(void))((unsigned long)&__STACK_END),
    //  0 The initial stack pointer
    ResetISR,          //  1 The reset handler
    NmiISR,            //  2 The NMI handler
    FaultISR,          //  3 The hard fault handler
    0,                 //  4 Reserved
    0,                 //  5 Reserved
    0,                 //  6 Reserved
    0,                 //  7 Reserved
    0,                 //  8 Reserved
    0,                 //  9 Reserved
    0,                 // 10 Reserved
    IntDefaultHandler, // 11 The SVCall handler
    0,                 // 12 Reserved
    0,                 // 13 Reserved
    IntDefaultHandler, // 14 The PendSV handler
    IntDefaultHandler, // 15 The SysTick handler
    //--- External interrupts ---
    IntDefaultHandler, // 16 CPUIRQ0 configurable source
    IntDefaultHandler, // 17 CPUIRQ1 configurable source
    IntDefaultHandler, // 18 CPUIRQ2 configurable source
    IntDefaultHandler, // 19 CPUIRQ3 configurable source
    IntDefaultHandler, // 20 CPUIRQ4 configurable source
    IntDefaultHandler, // 21 GPIO event interrupt
    IntDefaultHandler, // 22 LRF interrupt 0
    IntDefaultHandler, // 23 LRF interrupt 1
    IntDefaultHandler, // 24 uDMA done events
    IntDefaultHandler, // 25 AES interrupt
    IntDefaultHandler, // 26 SPI0 combined interrupt
    IntDefaultHandler, // 27 UART0 combined interrupt
    IntDefaultHandler, // 28 I2C0 combined interrupt
    IntDefaultHandler, // 29 LGPT0 interrupt
    IntDefaultHandler, // 30 LGPT1 interrupt
    IntDefaultHandler, // 31 ADC0 interrupt
    IntDefaultHandler, // 32 CPUIRQ16 configurable source
    IntDefaultHandler, // 33 LGPT2 interrupt
    IntDefaultHandler  // 34 LGPT3 interrupt
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
void ResetISR(void)
{
    // Final trim of device
    SetupTrimDevice();

    // Jump to the CCS C Initialization Routine.
    __asm("    .global _c_int00\n"
          "    bl      _c_int00");

    // If we ever return signal Error
    FaultISR();
}

//*****************************************************************************
//
//! This is the code that gets called when the processor receives a NMI. This
//! simply enters an infinite loop, preserving the system state for examination
//! by a debugger.
//
//*****************************************************************************
__attribute__((noreturn)) static void NmiISR(void)
{
    // Enter an infinite loop.
    while (1) {}
}

//*****************************************************************************
//
//! This is the code that gets called when the processor receives a fault
//! interrupt. This simply enters an infinite loop, preserving the system state
//! for examination by a debugger.
//
//*****************************************************************************
__attribute__((noreturn)) static void FaultISR(void)
{
    // Enter an infinite loop.
    while (1) {}
}

//*****************************************************************************
//
//! This is the code that gets called when the processor receives an unexpected
//! interrupt. This simply enters an infinite loop, preserving the system state
//! for examination by a debugger.
//
//*****************************************************************************
__attribute__((noreturn)) static void IntDefaultHandler(void)
{
    // Enter an infinite loop.
    while (1) {}
}
