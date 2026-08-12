/******************************************************************************
 *  Filename:       startup_iar.c
 *
 *  Description:    Startup code for CC27XX device family for use with IAR.
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
// Check if compiler is IAR
//
//*****************************************************************************
#if !(defined(__IAR_SYSTEMS_ICC__))
    #error "startup_iar.c: Unsupported compiler!"
#endif

// We need intrinsic functions for IAR (if used in source code)
#include <intrinsics.h>
#include "../inc/hw_types.h"
#include "../driverlib/setup.h"

//*****************************************************************************
//
//! Forward declaration of the default fault handlers.
//
//*****************************************************************************
static void NmiISRHandler(void);
static void FaultISRHandler(void);
static void IntDefaultHandler(void);
extern void NmiISR(void);
extern void FaultISR(void);
extern void MPUFaultIntHandler(void);
extern void BusFaultIntHandler(void);
extern void UsageFaultIntHandler(void);
extern void SecureFaultIntHandler(void);
extern void SVCallIntHandler(void);
extern void DebugMonIntHandler(void);
extern void PendSVIntHandler(void);
extern void SysTickIntHandler(void);

extern void CPUIRQ0IntHandler(void);
extern void CPUIRQ1IntHandler(void);
extern void CPUIRQ2IntHandler(void);
extern void CPUIRQ3IntHandler(void);
extern void CPUIRQ4IntHandler(void);
extern void GPIOIntHandler(void);
extern void LRF0IntHandler(void);
extern void LRF1IntHandler(void);
extern void uDMAIntHandler(void);
extern void AESIntHandler(void);
extern void SPI0IntHandler(void);
extern void SPI1IntHandler(void);
extern void UART0IntHandler(void);
extern void UART1IntHandler(void);
extern void I2C0IntHandler(void);
extern void LGPT0IntHandler(void);
extern void LGPT1IntHandler(void);
extern void LGPT2IntHandler(void);
extern void LGPT3IntHandler(void);
extern void ADC0IntHandler(void);
extern void CPUIRQ16IntHandler(void);
extern void LGPT2IntHandler(void);
extern void LGPT3IntHandler(void);
extern void CPUIRQ16IntHandler(void);
extern void CPUIRQ17IntHandler(void);
extern void I2S0IntHandler(void);
extern void MCANIntHandler(void);
extern void APUIntHandler(void);
extern void HSMSecIntHandler(void);
extern void HSMNonSecIntHandler(void);
extern void HSMOtpIntHandler(void);
extern void PMUIntHandler(void);
extern void CKMDIntHandler(void);
extern void AONRTCIntHandler(void);
extern void AONLPCMPIntHandler(void);
extern void IOCIntHandler(void);
extern void SW0IntHandler(void);
extern void SW1IntHandler(void);

// Default interrupt handlers
#pragma weak NmiISR                = NmiISRHandler
#pragma weak FaultISR              = FaultISRHandler
#pragma weak MPUFaultIntHandler    = IntDefaultHandler
#pragma weak BusFaultIntHandler    = IntDefaultHandler
#pragma weak UsageFaultIntHandler  = IntDefaultHandler
#pragma weak SecureFaultIntHandler = IntDefaultHandler
#pragma weak SVCallIntHandler      = IntDefaultHandler
#pragma weak DebugMonIntHandler    = IntDefaultHandler
#pragma weak PendSVIntHandler      = IntDefaultHandler
#pragma weak SysTickIntHandler     = IntDefaultHandler

#pragma weak CPUIRQ0IntHandler   = IntDefaultHandler
#pragma weak CPUIRQ1IntHandler   = IntDefaultHandler
#pragma weak CPUIRQ2IntHandler   = IntDefaultHandler
#pragma weak CPUIRQ3IntHandler   = IntDefaultHandler
#pragma weak CPUIRQ4IntHandler   = IntDefaultHandler
#pragma weak GPIOIntHandler      = IntDefaultHandler
#pragma weak LRF0IntHandler      = IntDefaultHandler
#pragma weak LRF1IntHandler      = IntDefaultHandler
#pragma weak uDMAIntHandler      = IntDefaultHandler
#pragma weak AESIntHandler       = IntDefaultHandler
#pragma weak SPI0IntHandler      = IntDefaultHandler
#pragma weak SPI1IntHandler      = IntDefaultHandler
#pragma weak UART0IntHandler     = IntDefaultHandler
#pragma weak UART1IntHandler     = IntDefaultHandler
#pragma weak I2C0IntHandler      = IntDefaultHandler
#pragma weak LGPT0IntHandler     = IntDefaultHandler
#pragma weak LGPT1IntHandler     = IntDefaultHandler
#pragma weak LGPT2IntHandler     = IntDefaultHandler
#pragma weak LGPT3IntHandler     = IntDefaultHandler
#pragma weak ADC0IntHandler      = IntDefaultHandler
#pragma weak CPUIRQ16IntHandler  = IntDefaultHandler
#pragma weak LGPT2IntHandler     = IntDefaultHandler
#pragma weak LGPT3IntHandler     = IntDefaultHandler
#pragma weak CPUIRQ16IntHandler  = IntDefaultHandler
#pragma weak CPUIRQ17IntHandler  = IntDefaultHandler
#pragma weak I2S0IntHandler      = IntDefaultHandler
#pragma weak MCANIntHandler      = IntDefaultHandler
#pragma weak APUIntHandler       = IntDefaultHandler
#pragma weak HSMSecIntHandler    = IntDefaultHandler
#pragma weak HSMNonSecIntHandler = IntDefaultHandler
#pragma weak HSMOtpIntHandler    = IntDefaultHandler
#pragma weak PMUIntHandler       = IntDefaultHandler
#pragma weak CKMDIntHandler      = IntDefaultHandler
#pragma weak AONRTCIntHandler    = IntDefaultHandler
#pragma weak AONLPCMPIntHandler  = IntDefaultHandler
#pragma weak IOCIntHandler       = IntDefaultHandler
#pragma weak SW0IntHandler       = IntDefaultHandler
#pragma weak SW1IntHandler       = IntDefaultHandler

//*****************************************************************************
//
//! The entry point for the application startup code.
//
//*****************************************************************************
extern void __iar_program_start(void);

//*****************************************************************************
//
//! Get stack start (highest address) symbol from linker file.
//
//*****************************************************************************
extern const void *STACK_TOP;

// It is required to place something in the CSTACK segment to get the stack
// check feature in IAR to work as expected
__root static void *dummy_stack @ ".stack";

//*****************************************************************************
//
//! The vector table. Note that the proper constructs must be placed on this to
//! ensure that it ends up at physical address 0x0000.0000 or at the start of
//! the program if located at a start address other than 0.
//
//*****************************************************************************
__root void (*const resetVectors[])(void) @ ".resetVecs" = {
    (void (*)(void)) & STACK_TOP, //  0 The initial stack pointer
    __iar_program_start,          //  1 The reset handler
    NmiISR,                       //  2 The NMI handler
    FaultISR,                     //  3 The hard fault handler
    MPUFaultIntHandler,           //  4 Memory Management (MemManage) Fault
    BusFaultIntHandler,           //  5 The bus fault handler
    UsageFaultIntHandler,         //  6 The usage fault handler
    SecureFaultIntHandler,        //  7 The secure fault handler
    0,                            //  8 Reserved
    0,                            //  9 Reserved
    0,                            // 10 Reserved
    SVCallIntHandler,             // 11 Supervisor Call (SVCall)
    DebugMonIntHandler,           // 12 Debug monitor handler
    0,                            // 13 Reserved
    PendSVIntHandler,             // 14 The PendSV handler
    SysTickIntHandler,            // 15 The SysTick handler
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
    UART0IntHandler,     // 27 UART0 combined interrupt, interrupt flags are found
                         // here UART0:MIS
    I2C0IntHandler,      // 28 Interrupt event from I2C0, interrupt flags can be
                         // found here I2C0:MIS
    LGPT0IntHandler,     // 29 LGPT0 combined interrupt, interrupt flags are found
                         // here LGPT0:MIS
    LGPT1IntHandler,     // 30 LGPT1 combined interrupt, interrupt flags are found
                         // here LGPT1:MIS
    ADC0IntHandler,      // 31 ADC combined interrupt request, interrupt flags can
                         // be found here ADC:MIS0
    CPUIRQ16IntHandler,  // 32 IRQ16: Configurable source controlled by
                         // EVTSVT.CPUIRQ16SEL
    CPUIRQ17IntHandler,  // 33 IRQ17: Configurable source controlled by
                         // EVTSVT.CPUIRQ17SEL
    LGPT2IntHandler,     // 34 LGPT2 combined interrupt, interrupt flags are found
                         // here LGPT2:MIS
    LGPT3IntHandler,     // 35 LGPT3 combined interrupt, interrupt flags are found
                         // here LGPT3:MIS
    I2S0IntHandler,      // 36 I2S interrupt event, controlled by I2S:IRQMASK
    MCANIntHandler,      // 37 MCAN interrupt event, interrupt flags can be found
                         // here MCAN:MIS0
    UART1IntHandler,     // 38 UART1 combined interrupt, interrupt flags are found
                         // here UART1:MIS
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
    AONRTCIntHandler,    // 46 AON_RTC event, controlled by the RTC:IMASK setting
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
void localProgramStart(void)
{
    // Do final trim of device
    SetupTrimDevice();

    // Set vector table base to point to above vectors in Flash; during
    // driverlib interrupt initialization this table will be copied to RAM.
    unsigned long *vtor = (unsigned long *)0xE000ED08;
    *vtor               = (unsigned long)&resetVectors[0];
}

int __low_level_init(void)
{
    // Initialize the stack pointer and branch to localProgramStart().
    // Some debuggers do not load the stack pointer from the reset vector.
    // This code ensures that the stack pointer is initialized.
    // The first entry of the vector table is the address of the stack.
    __asm(" ldr r0, =resetVectors\n"
          " ldr r0, [r0]\n"
          " mov sp, r0\n"
          " b localProgramStart");

    // Return 1 to signal that data sections shall be initialized by the
    // system startup code.
    return 1;
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
