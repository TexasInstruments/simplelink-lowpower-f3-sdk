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
#include DeviceFamily_constructPath(driverlib/interrupt.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(cmsis/cc23x0r5.h)
#include DeviceFamily_constructPath(cmsis/core/core_cm0plus.h)

#include <FreeRTOSConfig.h>

/* IAR includes */
#include <intrinsics.h>

//*****************************************************************************
//
// The entry point for the application startup code.
//
//*****************************************************************************
int localProgramStart(void);

// Place something in the CSTACK segment to get the stack check feature
// to work as expected
__root static void *dummy_stack @ ".stack";

//*****************************************************************************
//
// Reset vectors defined and populated in SysConfig.
//
//*****************************************************************************
extern void (*const __vector_table[])(void);

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
    __asm(" ldr r0, =__vector_table\n"
          " ldr r0, [r0]\n"
          " mov sp, r0\n"
          " b localProgramStart");

    /* Indicate that static and global variables shall be initialized */
    return 1;
}
