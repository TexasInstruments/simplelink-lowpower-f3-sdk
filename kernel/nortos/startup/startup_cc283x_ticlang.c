/*
 * Copyright (c) 2023-2026, Texas Instruments Incorporated
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

/* Check if compiler is TICLANG. */
#if !(defined(__clang__))
    #error "startup_cc27xx_ticlang.c: Unsupported compiler!"
#endif

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/setup.h)
#include DeviceFamily_constructPath(driverlib/interrupt.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(cmsis/device.h)

/* Forward declaration of the reset ISR and the default fault handlers. */
void resetISR(void);
static void nmiISR(void);
static void faultISR(void);
static void intDefaultHandler(void);

/* The entry point for the application startup code. */
extern void _c_int00(void);

/* Linker variable that marks the top of the stack. */
extern unsigned long __STACK_END;

/* The vector table. Note that the proper constructs must be placed on this to
 * ensure that it ends up at physical address 0x0000.0000 or at the start of
 * the program if located at a start address other than 0.
 */
__attribute__((section(".resetVecs"), retain)) void (*const resetVectors[])(void) = {
    (void (*)(void))((unsigned long)&__STACK_END), /*  0 The initial stack pointer. */
    resetISR,                                      /*  1 The reset handler. */
    nmiISR,                                        /*  2 The NMI handler. */
    faultISR,                                      /*  3 The hard fault handler. */
    intDefaultHandler,                             /*  4 The MPU fault handler. */
    intDefaultHandler,                             /*  5 The bus fault handler. */
    intDefaultHandler,                             /*  6 The usage fault handler. */
    0,                                             /*  7 Reserved. */
    0,                                             /*  8 Reserved. */
    0,                                             /*  9 Reserved. */
    0,                                             /* 10 Reserved. */
    intDefaultHandler,                             /* 11 SVCall handler. */
    intDefaultHandler,                             /* 12 Debug monitor handler. */
    0,                                             /* 13 Reserved. */
    intDefaultHandler,                             /* 14 The PendSV handler. */
    intDefaultHandler,                             /* 15 The SysTick handler. */
    /* --- External interrupts --- */
    intDefaultHandler, /* 16 IRQ0: Configurable source controlled by
                        * EVTSVT.CPUIRQ0SEL.
                        */
    intDefaultHandler, /* 17 IRQ1: Configurable source controlled by
                        * EVTSVT.CPUIRQ1SEL.
                        */
    intDefaultHandler, /* 18 IRQ2: Configurable source controlled by
                        * EVTSVT.CPUIRQ2SEL.
                        */
    intDefaultHandler, /* 19 IRQ3: Configurable source controlled by
                        * EVTSVT.CPUIRQ3SEL.
                        */
    intDefaultHandler, /* 20 IRQ4: Configurable source controlled by
                        * EVTSVT.CPUIRQ4SEL.
                        */
    intDefaultHandler, /* 21 IRQ5: Configurable source controlled by
                        * EVTSVT.CPUIRQ5SEL.
                        */
    intDefaultHandler, /* 22 IRQ6: Configurable source controlled by
                        * EVTSVT.CPUIRQ6SEL.
                        */
    intDefaultHandler, /* 23 IRQ7: Configurable source controlled by
                        * EVTSVT.CPUIRQ7SEL.
                        */
    intDefaultHandler, /* 24 IRQ8: Configurable source controlled by
                        * EVTSVT.CPUIRQ8SEL.
                        */
    intDefaultHandler, /* 25 IRQ9: Configurable source controlled by
                        * EVTSVT.CPUIRQ9SEL.
                        */
    intDefaultHandler, /* 26 IRQ10: Configurable source controlled by
                        * EVTSVT.CPUIRQ10SEL.
                        */
    intDefaultHandler, /* 27 IRQ11: Configurable source controlled by
                        * EVTSVT.CPUIRQ11SEL.
                        */
    intDefaultHandler, /* 28 IRQ12: Configurable source controlled by
                        * EVTSVT.CPUIRQ12SEL.
                        */
    intDefaultHandler, /* 29 CRNG combined interrupt. */
    intDefaultHandler, /* 30 SYSTIM Channel 1 event, event flag SYSTIM:MIS.EVT1. */
    intDefaultHandler, /* 31 UART0 combined interrupt, interrupt flags are found
                        * here UART0:MIS.
                        */
    intDefaultHandler, /* 32 AES accelerator combined interrupt request,
                        * interrupt flags can be found here AES:MIS.
                        */
    intDefaultHandler, /* 33 ADC secure combined interrupt request, interrupt
                        * flags can be found here ADC:MIS0.
                        */
    intDefaultHandler, /* 34 GPIO combined wake up interrupt, interrupt flags
                        * can be found here GPIO:MIS.
                        */
    intDefaultHandler, /* 35 LRFD combined event, interrupt flags can be found
                        * here LRFDDBELL:MIS0.
                        */
    intDefaultHandler, /* 36 LRFD combined event, interrupt flags can be found
                        * here LRFDDBELL:MIS1.
                        */
    intDefaultHandler, /* 37 DMA combined done interrupt, corresponding flags
                        * can be found here DMA:REQDONE.
                        */
    intDefaultHandler, /* 38 SPI0 combined interrupt request, interrupt flags
                        * can be found here SPI0:MIS.
                        */
    intDefaultHandler, /* 39 UART1 combined interrupt, interrupt flags are found
                        * here UART1:MIS.
                        */
    intDefaultHandler, /* 40 Interrupt event from I2C0, interrupt flags can be
                        * found here I2C0:MIS.
                        */
    intDefaultHandler, /* 41 LGPT0 combined interrupt, interrupt flags are found
                        * here LGPT0:MIS.
                        */
    intDefaultHandler, /* 42 LGPT1 combined interrupt, interrupt flags are found
                        * here LGPT1:MIS.
                        */
    intDefaultHandler, /* 43 LGPT2 combined interrupt, interrupt flags are found
                        * here LGPT2:MIS.
                        */
    intDefaultHandler, /* 44 LGPT3 combined interrupt, interrupt flags are found
                        * here LGPT3:MIS.
                        */
    intDefaultHandler, /* 45 PRCMULL0 combined interrupt request. */
    intDefaultHandler, /* 46 IOC synchronous combined event, controlled by
                        * IOC:EVTCFG.
                        */
};

/* This is the code that gets called when the processor first starts execution
 * following a reset event. Only the absolutely necessary set is performed,
 * after which the application supplied entry() routine is called. Any fancy
 * actions (such as making decisions based on the reset cause register, and
 * resetting the bits in that register) are left solely in the hands of the
 * application.
 */
void resetISR(void)
{
    IntDisableMaster();

    /* Final trim of device. */
    SetupTrimDevice();

    /* Jump to the C Initialization Routine. */
    __asm(" .global _c_int00\n"
          " bl      _c_int00");

    /* If we ever return, signal Error. */
    faultISR();
}

/* This is the code that gets called when the processor receives an NMI. This
 * simply enters an infinite loop, preserving the system state for examination
 * by a debugger.
 */
static void nmiISR(void)
{
    /* Enter an infinite loop. */
    while (1) {}
}

/* This is the code that gets called when the processor receives a fault
 * interrupt. This simply enters an infinite loop, preserving the system state
 * for examination by a debugger.
 */
static void faultISR(void)
{
    /* Enter an infinite loop. */
    while (1) {}
}

/* This is the code that gets called when the processor receives an unexpected
 * interrupt. This simply enters an infinite loop, preserving the system state
 * for examination by a debugger.
 */
static void intDefaultHandler(void)
{
    /* Enter an infinite loop. */
    while (1) {}
}
