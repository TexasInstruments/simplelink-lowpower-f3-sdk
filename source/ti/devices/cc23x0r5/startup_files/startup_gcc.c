/******************************************************************************
 *  Filename:       startup_gcc.c
 *
 *  Description:    Startup code for CC23X0R5 device family for use with GCC.
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
// Check if compiler is GNU Compiler
//
//*****************************************************************************
#if !(defined(__GNUC__))
    #error "startup_gcc.c: Unsupported compiler!"
#endif

#include "../inc/hw_types.h"
#include "../driverlib/setup.h"

//*****************************************************************************
//
// Macro for weak symbol aliasing
//
//*****************************************************************************
#define WEAK_ALIAS(x) __attribute__((weak, alias(#x)))

//*****************************************************************************
//
// Forward declaration of the reset ISR and the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);
static void NmiISRHandler(void);
static void FaultISRHandler(void);
static void IntDefaultHandler(void);
extern int main(void);

// Default interrupt handlers
void NmiISR(void) WEAK_ALIAS(NmiISRHandler);
void FaultISR(void) WEAK_ALIAS(FaultISRHandler);
void SVCallIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void PendSVIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void SysTickIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void CPUIRQ0IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void CPUIRQ1IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void CPUIRQ2IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void CPUIRQ3IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void CPUIRQ4IntHandler(void) WEAK_ALIAS(IntDefaultHandler);

void GPIOIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void LRF0IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void LRF1IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void uDMAIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void AESIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void SPI0IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void UART0IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void I2C0IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void LGPT0IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void LGPT1IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void ADC0IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void CPUIRQ16IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void LGPT2IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void LGPT3IntHandler(void) WEAK_ALIAS(IntDefaultHandler);

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.
//
//*****************************************************************************
extern uint32_t _ldata;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t _estack;

//*****************************************************************************
//
//! The vector table. Note that the proper constructs must be placed on this to
//! ensure that it ends up at physical address 0x0000.0000 or at the start of
//! the program if located at a start address other than 0.
//
//*****************************************************************************
__attribute__((section(".resetVecs"), used)) void (*const resetVectors[])(void) = {
    (void (*)(void))((unsigned long)&_estack),
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
    SVCallIntHandler,  // 11 Supervisor Call (SVCall)
    0,                 // 12 Reserved
    0,                 // 13 Reserved
    PendSVIntHandler,  // 14 The PendSV handler
    SysTickIntHandler, // 15 The SysTick handler
    //--- External interrupts ---
    CPUIRQ0IntHandler,  // 16 CPUIRQ0 configurable source
    CPUIRQ1IntHandler,  // 17 CPUIRQ1 configurable source
    CPUIRQ2IntHandler,  // 18 CPUIRQ2 configurable source
    CPUIRQ3IntHandler,  // 19 CPUIRQ3 configurable source
    CPUIRQ4IntHandler,  // 20 CPUIRQ4 configurable source
    GPIOIntHandler,     // 21 GPIO event interrupt
    LRF0IntHandler,     // 22 LRF interrupt 0
    LRF1IntHandler,     // 23 LRF interrupt 1
    uDMAIntHandler,     // 24 uDMA done events
    AESIntHandler,      // 25 AES interrupt
    SPI0IntHandler,     // 26 SPI0 combined interrupt
    UART0IntHandler,    // 27 UART0 combined interrupt
    I2C0IntHandler,     // 28 I2C0 combined interrupt
    LGPT0IntHandler,    // 29 LGPT0 interrupt
    LGPT1IntHandler,    // 30 LGPT1 interrupt
    ADC0IntHandler,     // 31 ADC0 interrupt
    CPUIRQ16IntHandler, // 32 CPUIRQ16 configurable source
    LGPT2IntHandler,    // 33 LGPT2 interrupt
    LGPT3IntHandler     // 34 LGPT3 interrupt
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
    uint32_t *pSrc;
    uint32_t *pDest;
    uint32_t *bs;
    uint32_t *be;

    // Final trim of device
    SetupTrimDevice();

    // Copy the data segment initializers from FLASH to SRAM.
    pSrc = &_ldata;
    for (pDest = &_data; pDest < &_edata;)
    {
        *pDest++ = *pSrc++;
    }

    // Initialize .bss to zero
    bs = &__bss_start__;
    be = &__bss_end__;
    while (bs < be)
    {
        *bs = 0;
        bs++;
    }

    // Call the application's entry point.
    main();

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
static void NmiISRHandler(void)
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
static void FaultISRHandler(void)
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
static void IntDefaultHandler(void)
{
    // Enter an infinite loop.
    while (1) {}
}
