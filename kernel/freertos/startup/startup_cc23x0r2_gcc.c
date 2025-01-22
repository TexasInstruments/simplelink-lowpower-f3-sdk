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

#include <stdint.h>
#include <string.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/setup.h)
#include DeviceFamily_constructPath(cmsis/cc23x0r2.h)
#include DeviceFamily_constructPath(cmsis/core/core_cm0plus.h)

#include <FreeRTOSConfig.h>

// Forward declaration of the default fault and interrupt handlers.
void resetISR(void);

// The reset handler called when processor is started
extern void _c_int00(void);

// The entry point for the application.
extern int main(void);

// Linker variable that marks the top and bottom of the stack.
extern void *__stack;
extern unsigned long _stack_end;

//*****************************************************************************
//
// Reset vectors defined and populated in SysConfig.
//
//*****************************************************************************
extern void (*const resetVectors[])(void);

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
// This is the code that gets called when the processor starts execution
// following a reset.  Only the absolutely necessary steps are performed,
// after which the application supplied entry routine is called.
//
//*****************************************************************************
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __data_load__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;

void localProgramStart(void)
{
    uint32_t *vtor = (unsigned long *)0xE000ED08;
    uint32_t *bs;
    uint32_t *be;
    uint32_t *dl;
    uint32_t *ds;
    uint32_t *de;
    uint32_t count;
    uint32_t i;

    /* do final trim of device */
    SetupTrimDevice();

    /* Disable interrupts */
    __disable_irq();
    __DSB();
    __ISB();

#if configENABLE_ISR_STACK_INIT
    /* Initialize ISR stack to known value for Runtime Object View */
    register uint32_t *top = (uint32_t *)&__stack;
    register uint32_t *end = (uint32_t *)&i;
    while (top < end)
    {
        *top++ = (uint32_t)0xa5a5a5a5;
    }
#endif

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

    /* run any constructors */
    count = (uint32_t)(__init_array_end - __init_array_start);
    for (i = 0; i < count; i++)
    {
        __init_array_start[i]();
    }

    /*
     * set vector table base to point to above vectors in Flash; during
     * driverlib interrupt initialization this table will be copied to RAM
     */
    *vtor = (uint32_t)&resetVectors[0];

    /* call the application's entry point. */
    main();
}

//*****************************************************************************
//
// This is the code that gets called when the processor is reset.
//
//*****************************************************************************
void __attribute__((naked)) resetISR(void)
{
    /*
     *  Some debuggers do not load the stack pointer from the reset vector.
     *  This code ensures that the stack pointer is initialized.  We branch
     *  to localProgramStart() so that nothing is pushed to the stack
     *  before it has been initialized.
     *
     *  For freertos/gcc specifically on this platform, the ti_freertos_config
     *  file is sufficiently large that it requires a full BL instruction
     *  rather than the limited range of the simple B instruction.
     */
    __asm__ __volatile__(" ldr r0, =resetVectors\n"
                         " ldr r0, [r0]\n"
                         " mov sp, r0\n"
                         " bl localProgramStart");
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
