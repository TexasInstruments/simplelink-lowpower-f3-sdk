/*
 * Copyright (c) 2022-2025, Texas Instruments Incorporated
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
// Check if compiler is GNU Compiler
//
//*****************************************************************************
#if !(defined(__GNUC__))
    #error "startup_cc23x0r5_gcc.c: Unsupported compiler!"
#endif

#include <string.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/setup.h)
#include DeviceFamily_constructPath(driverlib/interrupt.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(cmsis/cc23x0r5.h)
#include DeviceFamily_constructPath(cmsis/core/core_cm0plus.h)

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void resetISR(void);
static void nmiISR(void);
static void faultISR(void);
static void defaultHandler(void);

//*****************************************************************************
//
// External declaration for the reset handler that is to be called when the
// processor is started
//
//*****************************************************************************
extern void _c_int00(void);

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);

//*****************************************************************************
//
// linker variable that marks the top of stack.
//
//*****************************************************************************
extern unsigned long _stack_end;

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__((section(".resetVecs"), used)) void (*const resetVectors[16])(void) = {
    (void (*)(void))((uint32_t)&_stack_end),
    // The initial stack pointer
    resetISR,       // The reset handler
    nmiISR,         // The NMI handler
    faultISR,       // The hard fault handler
    defaultHandler, // Reserved
    defaultHandler, // Reserved
    defaultHandler, // The usage fault handler
    0,              // Reserved
    0,              // Reserved
    0,              // Reserved
    0,              // Reserved
    defaultHandler, // SVCall handler
    defaultHandler, // Debug monitor handler
    0,              // Reserved
    defaultHandler, // The PendSV handler
    defaultHandler  // The SysTick handler
};

//*****************************************************************************
//
// The following are arrays of pointers to constructor functions that need to
// be called during startup to initialize global objects.
//
//*****************************************************************************
extern void (*__init_array_start[])(void);
extern void (*__init_array_end[])(void);

//*****************************************************************************
//
// The following global variable is required for C++ support.
//
//*****************************************************************************
void *__dso_handle = (void *)&__dso_handle;

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern uint32_t __bss_start__, __bss_end__;
extern uint32_t __data_load__, __data_start__, __data_end__;

//
//*****************************************************************************
//
// Initialize the .data and .bss sections and copy the first 16 vectors from
// the read-only/reset table to the runtime RAM table. Fill the remaining
// vectors with a stub. This vector table will be updated at runtime.
//
//*****************************************************************************
//

/* Marked as used due to being removed by LTO, but is used in resetISR() */
__attribute__((used)) void localProgramStart(void)
{
    uint32_t *bs;
    uint32_t *be;
    uint32_t *dl;
    uint32_t *ds;
    uint32_t *de;
    uint32_t count;
    uint32_t i;

    IntDisableMaster();

    /* Final trim of device */
    SetupTrimDevice();

    /* initiailize .bss to zero */
    bs = &__bss_start__;
    be = &__bss_end__;
    while (bs < be)
    {
        *bs = 0;
        bs++;
    }

    /* relocate the .data section */
    dl = &__data_load__;
    ds = &__data_start__;
    de = &__data_end__;
    if (dl != ds)
    {
        while (ds < de)
        {
            *ds = *dl;
            dl++;
            ds++;
        }
    }

    /* Run any constructors */
    count = (uint32_t)(__init_array_end - __init_array_start);
    for (i = 0; i < count; i++)
    {
        __init_array_start[i]();
    }

    /* Call the application's entry point. */
    main();

    /* If we ever return signal Error */
    faultISR();
}

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
// .ltorg is added to avoid literal pool errors when LTO is enabled.
//
//*****************************************************************************
void __attribute__((naked)) resetISR(void)
{
    __asm__ __volatile__(" ldr r0, =resetVectors\n"
                         " ldr r0, [r0]\n"
                         " mov sp, r0\n"
                         " bl localProgramStart\n"
                         " .ltorg\n");
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
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
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void defaultHandler(void)
{
    /* Enter an infinite loop. */
    while (1) {}
}

//*****************************************************************************
//
// This function is called by __libc_fini_array which gets called when exit()
// is called. In order to support exit(), an empty _fini() stub function is
// required.
//
//*****************************************************************************
void _fini(void)
{
    /* Function body left empty intentionally */
}
