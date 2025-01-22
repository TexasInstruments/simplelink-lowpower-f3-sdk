/******************************************************************************
 *  Filename:       startup_gcc.c
 *
 *  Description:    Startup code for CC27XX device family for use with GCC.
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
void MPUFaultIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void BusFaultIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void UsageFaultIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void SecureFaultIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void SVCallIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void DebugMonIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
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
void SPI1IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void UART0IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void UART1IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void I2C0IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void LGPT0IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void LGPT1IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void LGPT2IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void LGPT3IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void ADC0IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void CPUIRQ16IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void CPUIRQ17IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void I2S0IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void MCANIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void APUIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void HSMSecIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void HSMNonSecIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void HSMOtpIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void PMUIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void CKMDIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void AONRTCIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void AONLPCMPIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void IOCIntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void SW0IntHandler(void) WEAK_ALIAS(IntDefaultHandler);
void SW1IntHandler(void) WEAK_ALIAS(IntDefaultHandler);

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
    ResetISR,              //  1 The reset handler
    NmiISR,                //  2 The NMI handler
    FaultISR,              //  3 The hard fault handler
    MPUFaultIntHandler,    //  4 Memory Management (MemManage) Fault
    BusFaultIntHandler,    //  5 The bus fault handler
    UsageFaultIntHandler,  //  6 The usage fault handler
    SecureFaultIntHandler, //  7 The secure fault handler
    0,                     //  8 Reserved
    0,                     //  9 Reserved
    0,                     // 10 Reserved
    SVCallIntHandler,      // 11 Supervisor Call (SVCall)
    DebugMonIntHandler,    // 12 Debug monitor handler
    0,                     // 13 Reserved
    PendSVIntHandler,      // 14 The PendSV handler
    SysTickIntHandler,     // 15 The SysTick handler
    //--- External interrupts ---
    CPUIRQ0IntHandler,   // 16 IRQ0: Configurable source controlled by
                         // EVTSVT.CPUIRQ0SEL
    CPUIRQ1IntHandler,   // 17 IRQ1: Configurable source controlled by
                         // EVTSVT.CPUIRQ1SEL
    CPUIRQ2IntHandler,   // 18 IRQ2: Configurable source controlled by
                         // EVTSVT.CPUIRQ2SEL
    CPUIRQ3IntHandler,   // 19 IRQ3: Configurable source controlled by
                         // EVTSVT.CPUIRQ3SEL
    CPUIRQ4IntHandler,   // 20 IRQ4: Configurable source controlled by
                         // EVTSVT.CPUIRQ4SEL
    GPIOIntHandler,      // 21 GPIO combined wake up interrupt, interrupt flags
                         // can be found here GPIO:MIS
    LRF0IntHandler,      // 22 LRFD combined event, interrupt flags can be found
                         // here LRFDDBELL:MIS0
    LRF1IntHandler,      // 23 LRFD combined event, interrupt flags can be found
                         // here LRFDDBELL:MIS1
    uDMAIntHandler,      // 24 DMA combined done interrupt, corresponding flags
                         // can be found here DMA:REQDONE
    AESIntHandler,       // 25 AES accelerator combined interrupt request,
                         // interrupt flags can be found here AES:MIS
    SPI0IntHandler,      // 26 SPI0 combined interrupt request, interrupt flags
                         // can be found here SPI0:MIS
    UART0IntHandler,     // 27 UART0 combined interrupt, interrupt flags are
                         // found here UART0:MIS
    I2C0IntHandler,      // 28 Interrupt event from I2C0, interrupt flags can be
                         // found here I2C0:MIS
    LGPT0IntHandler,     // 29 LGPT0 combined interrupt, interrupt flags are
                         // found here LGPT0:MIS
    LGPT1IntHandler,     // 30 LGPT1 combined interrupt, interrupt flags are
                         // found here LGPT1:MIS
    ADC0IntHandler,      // 31 ADC combined interrupt request, interrupt flags
                         // can be found here ADC:MIS0
    CPUIRQ16IntHandler,  // 32 IRQ16: Configurable source controlled by
                         // EVTSVT.CPUIRQ16SEL
    CPUIRQ17IntHandler,  // 33 IRQ17: Configurable source controlled by
                         // EVTSVT.CPUIRQ17SEL
    LGPT2IntHandler,     // 34 LGPT2 combined interrupt, interrupt flags are
                         // found here LGPT2:MIS
    LGPT3IntHandler,     // 35 LGPT3 combined interrupt, interrupt flags are
                         // found here LGPT3:MIS
    I2S0IntHandler,      // 36 I2S interrupt event, controlled by I2S:IRQMASK
    MCANIntHandler,      // 37 MCAN interrupt event, interrupt flags can be
                         // found here MCAN:MIS0
    UART1IntHandler,     // 38 UART1 combined interrupt, interrupt flags are
                         // found here UART1:MIS
    SPI1IntHandler,      // 39 SPI1 combined interrupt request, interrupt flags
                         // can be found here SPI1:MIS
    APUIntHandler,       // 40 APU IRQ
    HSMSecIntHandler,    // 41 HSM Secure IRQ
    HSMNonSecIntHandler, // 42 HSM Non-secure IRQ
    HSMOtpIntHandler,    // 43 HSM OTP IRQ
    PMUIntHandler,       // 44 PMU combined interrupt request for BATMON,
                         // interrupt flags can be found here PMUD:EVENT
    CKMDIntHandler,      // 45 CKMD combined interrupt request, interrupt flags
                         // can be found here CKMD:MIS
    AONRTCIntHandler,    // 46 AON_RTC event, controlled by the RTC:IMASK
                         // setting
    AONLPCMPIntHandler,  // 47 AON LPCMP interrupt, controlled by SYS0:LPCMPCFG
    IOCIntHandler,       // 48 IOC synchronous combined event, controlled by
                         // IOC:EVTCFG
    SW0IntHandler,       // 49 Software Triggered Interrupt 0
    SW1IntHandler,       // 50 Software Triggered Interrupt 1
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

    // Enable the FPU
    // CPACR is located at address 0xE000ED88
    // Set bits 20-23 in CPACR to enable CP10 and CP11 coprocessors
    __asm("    ldr.w   r0, =0xE000ED88\n"
          "    ldr     r1, [r0]\n"
          "    orr     r1, r1, #(0xF << 20)\n"
          "    str     r1, [r0]\n");

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
