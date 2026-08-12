/*
 * Copyright (c) 2021-2024, Texas Instruments Incorporated
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
    #error "startup_cc23x0r5_ticlang.c: Unsupported compiler!"
#endif

#include <stdint.h>
#include <string.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/setup.h)

#include DeviceFamily_constructPath(cmsis/cc23x0r5.h)
#include DeviceFamily_constructPath(cmsis/core/core_cm0plus.h)

#include <FreeRTOSConfig.h>

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
// Reset vectors defined and populated in SysConfig.
//
//*****************************************************************************
extern void (*const resetVectors[])(void);

//*****************************************************************************
//
// Optimization is disabled for this inline function to avoid compiler
// optimizations that use the stack.
//
//*****************************************************************************
__attribute__((optnone)) static inline void initializeStack(uint32_t *start, uint32_t *end)
{
    while (start < end)
    {
        *start++ = (uint32_t)0xa5a5a5a5;
    }
}

//*****************************************************************************
//
// This function is called at reset entry early in the boot sequence.
//
//*****************************************************************************
void localProgramStart(void)
{
    /* Stack variable used as limit during stack init */
    uint32_t stackInitLimit;
    /* Do final trim of device */
    SetupTrimDevice();

    /* Disable interrupts */
    __disable_irq();

#if configENABLE_ISR_STACK_INIT
    /* Initialize ISR stack to known value for Runtime Object View. */
    initializeStack((uint32_t *)&__stack, (uint32_t *)&stackInitLimit);
#endif

    /*
     * Set vector table base to point to above vectors in Flash; during
     * driverlib interrupt initialization this table will be copied to RAM
     */
    SCB->VTOR = (unsigned long)&resetVectors[0];

    /* Jump to the C initialization routine. */
    __asm(" .global _c_int00\n"
          " bl      _c_int00");
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
    __asm__ __volatile__(" ldr r0, =resetVectors\n"
                         " ldr r0, [r0]\n"
                         " mov sp, r0\n"
                         " bl localProgramStart");
}
