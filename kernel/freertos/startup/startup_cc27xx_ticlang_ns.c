/*
 * Copyright (c) 2022-2023, Texas Instruments Incorporated
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
#if !(defined(__clang__))
    #error "startup_cc27xx_ticlang_ns.c: Unsupported compiler!"
#endif

#include <stdint.h>
#include <string.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/interrupt.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)

#include <FreeRTOSConfig.h>

// Forward declaration of the default fault and interrupt handlers.
static void nmiISR(void);
static void faultISR(void);
static void busFaultHandler(void);
static void intDefaultHandler(void);
static void secureFaultHandler(void);

// FreeRTOS handlers
extern void SVC_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);

//*****************************************************************************
//
// The entry point for the application startup code.
//
//*****************************************************************************
extern void _c_int00(void);
void resetISR(void);

//*****************************************************************************
//
// Linker variables that marks the top and bottom of the stack.
//
//*****************************************************************************
extern unsigned long __STACK_END;
extern void *__stack;

//*****************************************************************************
//
// The vector table in Flash. Note that the proper constructs must be placed
// on this to ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__((section(".resetVecs"), retain)) static void (*const resetVectors[])(void) = {
    (void (*)(void))((unsigned long)&__STACK_END),
    resetISR,           // The reset handler
    nmiISR,             // The NMI handler
    faultISR,           // The hard fault handler
    intDefaultHandler,  // The MPU fault handler
    busFaultHandler,    // The bus fault handler
    intDefaultHandler,  // The usage fault handler
    secureFaultHandler, // The secure fault handler
    intDefaultHandler,  // Reserved
    intDefaultHandler,  // Reserved
    intDefaultHandler,  // Reserved
    SVC_Handler,        // SVCall handler
    intDefaultHandler,  // Debug monitor handler
    intDefaultHandler,  // Reserved
    PendSV_Handler,     // The PendSV handler
    SysTick_Handler,    // The SysTick handler
    intDefaultHandler,  // 16 IRQ0: Configurable source controlled by EVTSVT.CPUIRQ0SEL
    intDefaultHandler,  // 17 IRQ1: Configurable source controlled by EVTSVT.CPUIRQ1SEL
    intDefaultHandler,  // 18 IRQ2: Configurable source controlled by EVTSVT.CPUIRQ2SEL
    intDefaultHandler,  // 19 IRQ3: Configurable source controlled by EVTSVT.CPUIRQ3SEL
    intDefaultHandler,  // 20 IRQ4: Configurable source controlled by EVTSVT.CPUIRQ4SEL
    intDefaultHandler,  // 21 GPIO combined wake up interrupt, interrupt flags can be found here GPIO:MIS
    intDefaultHandler,  // 22 LRFD combined event, interrupt flags can be found here LRFD:MIS0
    intDefaultHandler,  // 23 LRFD combined event, interrupt flags can be found here LRFD:MIS1
    intDefaultHandler,  // 24 DMA combined dma_done interrupt, corresponding flags can be found here DMA:REQDONE
    intDefaultHandler,  // 25 AES accelerator combined interrupt request, interrupt flags can be found here AES:MIS
    intDefaultHandler,  // 26 SPI0 combined interrupt request, interrupt flags can be found here SPI0:MIS
    intDefaultHandler,  // 27 UART0 combined interrupt, interrupt flags are found here UART0:MIS
    intDefaultHandler,  // 28 Interrupt event from I2C0, interrupt flags can be found here I2C0:MIS
    intDefaultHandler,  // 29 LGPT0 combined interrupt, interrupt flags are found here LGPT0:MIS
    intDefaultHandler,  // 30 LGPT1 combined interrupt, interrupt flags are found here LGPT1:MIS
    intDefaultHandler,  // 31 ADC combined interrupt request, interrupt flags can be found here ADC:MIS
    intDefaultHandler,  // 32 IRQ16: Configurable source controlled by EVTSVT.CPUIRQ16SEL
    intDefaultHandler,  // 33 IRQ17: Configurable source controlled by EVTSVT.CPUIRQ17SEL
    intDefaultHandler,  // 34 LGPT2 combined interrupt, interrupt flags are found here LGPT2:MIS
    intDefaultHandler,  // 35 LGPT3 combined interrupt, interrupt flags are found here LGPT3:MIS
    intDefaultHandler,  // 36 I2S interrupt event, controlled by I2S:IRQMASK
    intDefaultHandler,  // 37 MCAN interrupt event, interrupt flags can be found here MCAN:MIS0
    intDefaultHandler,  // 38 UART1 combined interrupt, interrupt flags are found here UART0:MIS
    intDefaultHandler,  // 39 SPI1 combined interrupt request, interrupt flags can be found here SPI1:MIS
    intDefaultHandler,  // 40 VCE IRQ
    intDefaultHandler,  // 41 HSM Secure IRQ
    intDefaultHandler,  // 42 HSM Non-secure IRQ
    intDefaultHandler,  // 43 HSM OTP IRQ
    intDefaultHandler,  // 44 PMU combined interrupt request for BATMON, interrupt flags can be found here PMU:EVENT
    intDefaultHandler,  // 45 CKMD combined interrupt request, interrupt flags can be found here CKMD:MIS
    intDefaultHandler,  // 46 AON_RTC event, controlled by the RTC:IMASK setting
    intDefaultHandler,  // 47 AON LPCMP interrupt, controlled by SYS0:LPCMPCFG
    intDefaultHandler,  // 48 IOC synchronous combined event, controlled by IOC:EVTCFG
    intDefaultHandler,  // 49 AON VGM interrupt
};

//*****************************************************************************
//
// This function is called at reset entry early in the boot sequence.
//
//*****************************************************************************
void localProgramStart(void)
{
    unsigned long *vtor = (unsigned long *)0xE000ED08;
    uint32_t newBasePri;

    /* Do not call SetupTrimDevice() since that was already called by TFM */

    /* disable interrupts */
    __asm volatile(" mov %0, %1 \n"
                   " msr basepri, %0 \n"
                   " isb \n"
                   " dsb \n"
                   : "=r"(newBasePri)
                   : "i"(configMAX_SYSCALL_INTERRUPT_PRIORITY)
                   : "memory");

#if configENABLE_ISR_STACK_INIT
    /* Initialize ISR stack to known value for Runtime Object View */
    register uint32_t *top = (uint32_t *)&__stack;
    register uint32_t *end = (uint32_t *)&newBasePri;
    while (top < end)
    {
        *top++ = (uint32_t)0xa5a5a5a5;
    }
#endif

    /*
     * set vector table base to point to above vectors in Flash; during
     * driverlib interrupt initialization this table will be copied to RAM
     */
    *vtor = (unsigned long)&resetVectors[0];

    /* jump to the C initialization routine. */
    __asm(" .global _c_int00\n"
          " b.w     _c_int00");
}

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary steps are performed,
// after which the application supplied entry routine is called.
//
//*****************************************************************************
void resetISR(void)
{
    /*
     * Set stack pointer based on the stack value stored in the vector table.
     * This is necessary to ensure that the application is using the correct
     * stack when using a debugger since a reset within the debugger will
     * load the stack pointer from the bootloader's vector table at address '0'.
     */
    __asm__ __volatile__(" movw r0, #:lower16:resetVectors\n"
                         " movt r0, #:upper16:resetVectors\n"
                         " ldr r0, [r0]\n"
                         " mov sp, r0\n"
                         " b localProgramStart");
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
static void nmiISR(void)
{
    /* Enter an infinite loop. */
    while (1) {}
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void faultISR(void)
{
    /* Enter an infinite loop. */
    while (1) {}
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a bus fault.
// This simply enters an infinite loop, preserving the system state for
// examination by a debugger.
//
//*****************************************************************************
static void busFaultHandler(void)
{
    /* Enter an infinite loop. */
    while (1) {}
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void intDefaultHandler(void)
{
    /* Enter an infinite loop. */
    while (1) {}
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a secure fault
// interrupt. This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void secureFaultHandler(void)
{
    /* Enters an infinite loop */
    while (1) {}
}
