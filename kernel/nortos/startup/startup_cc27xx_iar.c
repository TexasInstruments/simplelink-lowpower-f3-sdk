/*
 * Copyright (c) 2023-2024, Texas Instruments Incorporated
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
// Check if compiler is IAR
//
//*****************************************************************************
#if !(defined(__IAR_SYSTEMS_ICC__))
    #error "startup_cc27xx_iar.c: Unsupported compiler!"
#endif

// We need intrinsic functions for IAR (if used in source code)
#include <intrinsics.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(driverlib/setup.h)
#include DeviceFamily_constructPath(driverlib/interrupt.h)
#include DeviceFamily_constructPath(cmsis/cc27xx.h)
#include DeviceFamily_constructPath(cmsis/core/core_cm33.h)

//*****************************************************************************
//
//! Forward declaration of the reset ISR and the default fault handlers.
//
//*****************************************************************************
static void nmiISR(void);
static void faultISR(void);
static void intDefaultHandler(void);
extern int main(void);

//*****************************************************************************
//
//! The entry point for the application startup code.
//
//*****************************************************************************
extern void __iar_program_start(void);
int localProgramStart(void);

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
__root void (*const __vector_table[])(void) @ ".resetVecs" = {
    (void (*)(void)) & STACK_TOP, //  0 The initial stack pointer
    __iar_program_start,          //  1 The reset handler
    nmiISR,                       //  2 The NMI handler
    faultISR,                     //  3 The hard fault handler
    intDefaultHandler,            //  4 The MPU fault handler
    intDefaultHandler,            //  5 The bus fault handler
    intDefaultHandler,            //  6 The usage fault handler
    intDefaultHandler,            //  7 The secure fault handler
    0,                            //  8 Reserved
    0,                            //  9 Reserved
    0,                            // 10 Reserved
    intDefaultHandler,            // 11 SVCall handler
    intDefaultHandler,            // 12 Debug monitor handler
    0,                            // 13 Reserved
    intDefaultHandler,            // 14 The PendSV handler
    intDefaultHandler,            // 15 The SysTick handler
    //--- External interrupts ---
    intDefaultHandler, // 16 IRQ0: Configurable source controlled by
                       // EVTSVT.CPUIRQ0SEL
    intDefaultHandler, // 17 IRQ1: Configurable source controlled by
                       // EVTSVT.CPUIRQ1SEL
    intDefaultHandler, // 18 IRQ2: Configurable source controlled by
                       // EVTSVT.CPUIRQ2SEL
    intDefaultHandler, // 19 IRQ3: Configurable source controlled by
                       // EVTSVT.CPUIRQ3SEL
    intDefaultHandler, // 20 IRQ4: Configurable source controlled by
                       // EVTSVT.CPUIRQ4SEL
    intDefaultHandler, // 21 GPIO combined wake up interrupt, interrupt flags
                       // can be found here GPIO:MIS
    intDefaultHandler, // 22 LRFD combined event, interrupt flags can be found
                       // here LRFDDBELL:MIS0
    intDefaultHandler, // 23 LRFD combined event, interrupt flags can be found
                       // here LRFDDBELL:MIS1
    intDefaultHandler, // 24 DMA combined done interrupt, corresponding flags
                       // can be found here DMA:REQDONE
    intDefaultHandler, // 25 AES accelerator combined interrupt request,
                       // interrupt flags can be found here AES:MIS
    intDefaultHandler, // 26 SPI0 combined interrupt request, interrupt flags
                       // can be found here SPI0:MIS
    intDefaultHandler, // 27 UART0 combined interrupt, interrupt flags are found
                       // here UART0:MIS
    intDefaultHandler, // 28 Interrupt event from I2C0, interrupt flags can be
                       // found here I2C0:MIS
    intDefaultHandler, // 29 LGPT0 combined interrupt, interrupt flags are found
                       // here LGPT0:MIS
    intDefaultHandler, // 30 LGPT1 combined interrupt, interrupt flags are found
                       // here LGPT1:MIS
    intDefaultHandler, // 31 ADC combined interrupt request, interrupt flags can
                       // be found here ADC:MIS0
    intDefaultHandler, // 32 IRQ16: Configurable source controlled by
                       // EVTSVT.CPUIRQ16SEL
    intDefaultHandler, // 33 IRQ17: Configurable source controlled by
                       // EVTSVT.CPUIRQ17SEL
    intDefaultHandler, // 34 LGPT2 combined interrupt, interrupt flags are found
                       // here LGPT2:MIS
    intDefaultHandler, // 35 LGPT3 combined interrupt, interrupt flags are found
                       // here LGPT3:MIS
    intDefaultHandler, // 36 I2S interrupt event, controlled by I2S:IRQMASK
    intDefaultHandler, // 37 MCAN interrupt event, interrupt flags can be found
                       // here MCAN:MIS0
    intDefaultHandler, // 38 UART1 combined interrupt, interrupt flags are found
                       // here UART1:MIS
    intDefaultHandler, // 39 SPI1 combined interrupt request, interrupt flags
                       // can be found here SPI1:MIS
    intDefaultHandler, // 40 APU IRQ
    intDefaultHandler, // 41 HSM Secure IRQ
    intDefaultHandler, // 42 HSM Non-secure IRQ
    intDefaultHandler, // 43 HSM OTP IRQ
    intDefaultHandler, // 44 PMU combined interrupt request for BATMON,
                       // interrupt flags can be found here PMUD:EVENT
    intDefaultHandler, // 45 CKMD combined interrupt request, interrupt flags
                       // can be found here CKMD:MIS
    intDefaultHandler, // 46 AON_RTC event, controlled by the RTC:IMASK setting
    intDefaultHandler, // 47 AON LPCMP interrupt, controlled by SYS0:LPCMPCFG
    intDefaultHandler, // 48 IOC synchronous combined event, controlled by
                       // IOC:EVTCFG
    intDefaultHandler, // 49 Software Triggered Interrupt 0
    intDefaultHandler, // 50 Software Triggered Interrupt 1
};

//*****************************************************************************
//
// Perform early-boot device initialisation.
//
//*****************************************************************************
int localProgramStart(void)
{
    unsigned long *vtor = (unsigned long *)0xE000ED08;

    /* do final trim of device */
    SetupTrimDevice();

    /* disable interrupts */
    __disable_irq();
    __DSB();
    __ISB();

    /*
     * set vector table base to point to above vectors in Flash; during
     * driverlib interrupt initialization this table will be copied to RAM
     */
    *vtor = (unsigned long)&__vector_table[0];

    /* Choose if segment initialization should be done or not.
     * Return: 0 to omit seg_init
     *         1 to run seg_init
     */
    return 1;
}

//*****************************************************************************
//
// This function is called by __iar_program_start() early in the boot sequence.
//
//*****************************************************************************
int __low_level_init(void)
{
    /*
     *  Initialize the stack pointer and branch to localProgramStart().
     *  Some debuggers do not load the stack pointer from the reset vector.
     *  This code ensures that the stack pointer is initialized.
     *  The first entry of the vector table is the address of the stack.
     */
    __asm("    mov32 r0, __vector_table\n"
          "    ldr r0, [r0]\n"
          "    mov sp, r0\n"
          "    b localProgramStart");

    // This code is unreachable but the compiler expects a return statement
    return 1;
}

//*****************************************************************************
//
//! This is the code that gets called when the processor receives an NMI. This
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
