/******************************************************************************
 *  Filename:       startup_ticlang.c
 *
 *  Description:    Startup code for CC27XX device family for use with
 *                  TI Clang/LLVM.
 *
 *  Copyright (c) 2022-2024 Texas Instruments Incorporated
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
    IntDefaultHandler, //  4 The MPU fault handler
    IntDefaultHandler, //  5 The bus fault handler
    IntDefaultHandler, //  6 The usage fault handler
    IntDefaultHandler, //  7 The secure fault handler
    0,                 //  8 Reserved
    0,                 //  9 Reserved
    0,                 // 10 Reserved
    IntDefaultHandler, // 11 The SVCall handler
    IntDefaultHandler, // 12 Debug monitor handler
    0,                 // 13 Reserved
    IntDefaultHandler, // 14 The PendSV handler
    IntDefaultHandler, // 15 The SysTick handler
    //--- External interrupts ---
    IntDefaultHandler, // 16 IRQ0: Configurable source controlled by
                       // EVTSVT.CPUIRQ0SEL
    IntDefaultHandler, // 17 IRQ1: Configurable source controlled by
                       // EVTSVT.CPUIRQ1SEL
    IntDefaultHandler, // 18 IRQ2: Configurable source controlled by
                       // EVTSVT.CPUIRQ2SEL
    IntDefaultHandler, // 19 IRQ3: Configurable source controlled by
                       // EVTSVT.CPUIRQ3SEL
    IntDefaultHandler, // 20 IRQ4: Configurable source controlled by
                       // EVTSVT.CPUIRQ4SEL
    IntDefaultHandler, // 21 GPIO combined wake up interrupt, interrupt flags
                       // can be found here GPIO:MIS
    IntDefaultHandler, // 22 LRFD combined event, interrupt flags can be found
                       // here LRFDDBELL:MIS0
    IntDefaultHandler, // 23 LRFD combined event, interrupt flags can be found
                       // here LRFDDBELL:MIS1
    IntDefaultHandler, // 24 DMA combined done interrupt, corresponding flags
                       // can be found here DMA:REQDONE
    IntDefaultHandler, // 25 AES accelerator combined interrupt request,
                       // interrupt flags can be found here AES:MIS
    IntDefaultHandler, // 26 SPI0 combined interrupt request, interrupt flags
                       // can be found here SPI0:MIS
    IntDefaultHandler, // 27 UART0 combined interrupt, interrupt flags are found
                       // here UART0:MIS
    IntDefaultHandler, // 28 Interrupt event from I2C0, interrupt flags can be
                       // found here I2C0:MIS
    IntDefaultHandler, // 29 LGPT0 combined interrupt, interrupt flags are found
                       // here LGPT0:MIS
    IntDefaultHandler, // 30 LGPT1 combined interrupt, interrupt flags are found
                       // here LGPT1:MIS
    IntDefaultHandler, // 31 ADC combined interrupt request, interrupt flags can
                       // be found here ADC:MIS0
    IntDefaultHandler, // 32 IRQ16: Configurable source controlled by
                       // EVTSVT.CPUIRQ16SEL
    IntDefaultHandler, // 33 IRQ17: Configurable source controlled by
                       // EVTSVT.CPUIRQ17SEL
    IntDefaultHandler, // 34 LGPT2 combined interrupt, interrupt flags are found
                       // here LGPT2:MIS
    IntDefaultHandler, // 35 LGPT3 combined interrupt, interrupt flags are found
                       // here LGPT3:MIS
    IntDefaultHandler, // 36 I2S interrupt event, controlled by I2S:IRQMASK
    IntDefaultHandler, // 37 MCAN interrupt event, interrupt flags can be found
                       // here MCAN:MIS0
    IntDefaultHandler, // 38 UART1 combined interrupt, interrupt flags are found
                       // here UART1:MIS
    IntDefaultHandler, // 39 SPI1 combined interrupt request, interrupt flags
                       // can be found here SPI1:MIS
    IntDefaultHandler, // 40 APU IRQ
    IntDefaultHandler, // 41 HSM Secure IRQ
    IntDefaultHandler, // 42 HSM Non-secure IRQ
    IntDefaultHandler, // 43 HSM OTP IRQ
    IntDefaultHandler, // 44 PMU combined interrupt request for BATMON,
                       // interrupt flags can be found here PMUD:EVENT
    IntDefaultHandler, // 45 CKMD combined interrupt request, interrupt flags
                       // can be found here CKMD:MIS
    IntDefaultHandler, // 46 AON_RTC event, controlled by the RTC:IMASK setting
    IntDefaultHandler, // 47 AON LPCMP interrupt, controlled by SYS0:LPCMPCFG
    IntDefaultHandler, // 48 IOC synchronous combined event, controlled by
                       // IOC:EVTCFG
    IntDefaultHandler, // 49 Software Triggered Interrupt 0
    IntDefaultHandler, // 50 Software Triggered Interrupt 1
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
