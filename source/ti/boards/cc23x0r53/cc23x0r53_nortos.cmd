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
#include "ti/boards/lpf3/cc23xx_crc_linker_symbols.cmd"

--stack_size=1024   /* C stack is also used for ISR stack */

--heap_size=8192

/* Retain interrupt vector table variable                                    */
--retain "*(.resetVecs)"
/* Override default entry point.                                             */
--entry_point resetISR
/* Allow main() to take args                                                 */
--args 0x8
/* Suppress warnings and errors:                                             */
/* - 10063: Warning about entry point not being _c_int00                     */
/* - 16011, 16012: 8-byte alignment errors. Observed when linking in object  */
/*   files compiled using Keil (ARM compiler)                                */
--diag_suppress=10063,16011,16012

/* Set severity of diagnostics to Remark instead of Warning                  */
/* - 10068: Warning about no matching log_ptr* sections                      */
--diag_remark=10068

/* The following command line options are set as part of the CCS project.    */
/* If you are building using the command line, or for some reason want to    */
/* define them here, you can uncomment and modify these lines as needed.     */
/* If you are using CCS for building, it is probably better to make any such */
/* modifications in your CCS project and leave this file alone.              */
/*                                                                           */
/* --heap_size=0                                                             */
/* --stack_size=2048                                                         */

/* The starting address of the application.  Normally the interrupt vectors  */
/* must be located at the beginning of the application.                      */
#define FLASH_BASE              0x0
#define FLASH_SIZE              0x80000
#define RAM_BASE                0x20000000
#define RAM_SIZE                0x10000
#define S2RRAM_BASE             0x40098000
#define S2RRAM_SIZE             0x1000
#define CCFG_BASE               0x4E020000
#define CCFG_SIZE               0x800


/* System memory map */

MEMORY
{
    /* Application stored in and executes from internal flash */
    FLASH (RX) : origin = FLASH_BASE, length = FLASH_SIZE
    /* Application uses internal RAM for data */
    SRAM (RWX) : origin = RAM_BASE, length = RAM_SIZE
    /* S2RRAM is intended for the S2R radio module, but it can also be used by
     * the application with some limitations. Please refer to the s2rram example.
     */
    S2RRAM (RW) : origin = S2RRAM_BASE, length = S2RRAM_SIZE
    /* Configuration region */
    CCFG (R) : origin = CCFG_BASE, length = CCFG_SIZE
    /* Explicitly placed off target for the storage of logging data.
     * The ARM memory map allocates 1 GB of external memory from 0x60000000 - 0x9FFFFFFF.
     * Unlikely that all of this will be used, so we are using the upper parts of the region.
     * ARM memory map: https://developer.arm.com/documentation/ddi0337/e/memory-map/about-the-memory-map*/
    LOG_DATA (R) : origin = 0x90000000, length = 0x40000        /* 256 KB */
    LOG_PTR  (R) : origin = 0x94000008, length = 0x40000        /* 256 KB */
}

/* Section allocation in memory */

SECTIONS
{
    .resetVecs      :   > FLASH_BASE
    .text           :   > FLASH
    .TI.ramfunc     : {} load=FLASH, run=SRAM, table(BINIT)
    .const          :   > FLASH
    .constdata      :   > FLASH
    .rodata         :   > FLASH
    .binit          :   > FLASH
    .cinit          :   > FLASH
    .pinit          :   > FLASH
    .init_array     :   > FLASH
    .emb_text       :   > FLASH
    .ccfg           :   > CCFG

    .ramVecs        :   > SRAM, type = NOLOAD, ALIGN(256)
    .data           :   > SRAM
    .bss            :   > SRAM
    .sysmem         :   > SRAM
    .stack          :   > SRAM (HIGH)
    .nonretenvar    :   > SRAM

    /* Placing the section .s2rram in S2RRAM region. Only uninitialized
     * objects may be placed in this section.
     */
    .s2rram         :   > S2RRAM, type = NOINIT

    .log_data       :   > LOG_DATA, type = COPY
    .log_ptr        : { *(.log_ptr*) } > LOG_PTR align 4, type = COPY
}
