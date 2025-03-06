/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2025, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

#include "ti_utils_build_linker.cmd.genmap"

--stack_size=1024
--heap_size=0
--entry_point resetISR

/* Retain interrupt vector table variable                                    */
--retain "*(.resetVecs)"

/* Suppress warnings and errors:                                             */
/* - 10063: Warning about entry point not being _c_int00                     */
/* - 16011, 16012: 8-byte alignment errors. Observed when linking in object  */
/*   files compiled using Keil (ARM compiler)                                */
--diag_suppress=10063,16011,16012

/* Set severity of diagnostics to Remark instead of Warning                  */
/* - 10068: Warning about no matching log_ptr* sections                      */
--diag_remark=10068

/* The starting address of the application.  Normally the interrupt vectors  */
/* must be located at the beginning of the application.                      */

#if defined(OTA_ONCHIP) || defined(OTA_OFFCHIP)

#if defined(OTA_ONCHIP)
// The slot size must match in the post build step and in MCUBoot configuration.
#define MCUBOOT_MAX_SLOT_SIZE   0x0003D800
#define PRIMARY_SLOT_BASE       0x00004000
#define SECONDARY_SLOT_BASE     0x00041800

#else // OTA_OFFCHIP
// The slot size must match in the post build step and in MCUBoot configuration.
#define MCUBOOT_MAX_SLOT_SIZE   0x00070000
#define PRIMARY_SLOT_BASE       0x00004000
#define SECONDARY_SLOT_BASE     0x0
#endif // OTA_ONCHIP

#define MCUBOOT_IMG_HDR_BASE    PRIMARY_SLOT_BASE
#define MCUBOOT_IMG_HDR_SIZE    0x100
#define MCUBOOT_TRAIL_SIZE      0x1000

#define FLASH_BASE              MCUBOOT_IMG_HDR_BASE  + MCUBOOT_IMG_HDR_SIZE
#define FLASH_SIZE              MCUBOOT_MAX_SLOT_SIZE - MCUBOOT_IMG_HDR_SIZE - MCUBOOT_TRAIL_SIZE

_MCUBOOT_MAX_SLOT_SIZE = MCUBOOT_MAX_SLOT_SIZE;
_PRIMARY_SLOT_BASE = PRIMARY_SLOT_BASE;
_SECONDARY_SLOT_BASE = SECONDARY_SLOT_BASE;
#else

#define FLASH_BASE              ti_utils_build_GenMap_FLASH0_BASE
#define FLASH_SIZE              ti_utils_build_GenMap_FLASH0_SIZE - ti_utils_build_GenMap_NVS_CONFIG_NVSINTERNAL_ZB_SIZE

#endif // defined(OTA_ONCHIP) || defined(OTA_OFFCHIP)

#define NVRAM_BASE             ti_utils_build_GenMap_NVS_CONFIG_NVSINTERNAL_ZB_BASE
#define NVRAM_SIZE             ti_utils_build_GenMap_NVS_CONFIG_NVSINTERNAL_ZB_SIZE

#define RAM_BASE                ti_utils_build_GenMap_RAM0_BASE
#define RAM_SIZE                ti_utils_build_GenMap_RAM0_SIZE
#if defined(ti_utils_build_GenMap_S2RRAM_BASE) && \
    defined(ti_utils_build_GenMap_S2RRAM_SIZE)
#define S2RRAM_BASE             ti_utils_build_GenMap_S2RRAM_BASE
#define S2RRAM_SIZE             ti_utils_build_GenMap_S2RRAM_SIZE
#endif
#define CCFG_BASE               ti_utils_build_GenMap_CCFG_BASE
#define CCFG_SIZE               ti_utils_build_GenMap_CCFG_SIZE
#if defined(ti_utils_build_GenMap_SCFG_BASE) && \
    defined(ti_utils_build_GenMap_SCFG_SIZE)
#define SCFG_BASE               ti_utils_build_GenMap_SCFG_BASE
#define SCFG_SIZE               ti_utils_build_GenMap_SCFG_SIZE
#endif
#if defined(ti_utils_build_GenMap_HSMOTP_BASE) && \
    defined(ti_utils_build_GenMap_HSMOTP_SIZE)
#define HSMOTP_BASE             ti_utils_build_GenMap_HSMOTP_BASE
#define HSMOTP_SIZE             ti_utils_build_GenMap_HSMOTP_SIZE
#endif

/* System memory map */
MEMORY
{
    /* Application stored in and executes from internal flash */
    FLASH (RX) : origin = FLASH_BASE, length = FLASH_SIZE
    NVRAM (RX) : origin = NVRAM_BASE, length = NVRAM_SIZE
    /* Application uses internal RAM for data */
    SRAM (RWX) : origin = RAM_BASE, length = RAM_SIZE

#if defined(S2RRAM_BASE) && defined(S2RRAM_SIZE)
    /* S2RRAM is intended for the S2R radio module, but it can also be used by
     * the application with some limitations. Please refer to the s2rram example.
     */
    S2RRAM (RW) : origin = S2RRAM_BASE, length = S2RRAM_SIZE
#endif

    /* Configuration region */
    CCFG (R) : origin = CCFG_BASE, length = CCFG_SIZE

#if defined(SCFG_BASE) && defined(SCFG_SIZE)
    /* Security configuration region */
    SCFG (R): origin = SCFG_BASE, length = SCFG_SIZE
#endif
#if defined(HSMOTP_BASE) && defined(HSMOTP_SIZE)
    /* HSM OTP region */
    HSMOTP (R): origin = HSMOTP_BASE, length = HSMOTP_SIZE
#endif

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

#if defined(SCFG_BASE) && defined(SCFG_SIZE)
    .scfg           :   > SCFG
#endif
#if defined(HSMOTP_BASE) && defined(HSMOTP_SIZE)
    .hsmotp         :   > HSMOTP
#endif

    .ramVecs        :   > SRAM, type = NOLOAD, ALIGN(256)
    .data           :   > SRAM
    .bss            :   > SRAM
    .sysmem         :   > SRAM
    .stack          :   > SRAM (HIGH)
    .nonretenvar    :   > SRAM

#if defined(S2RRAM_BASE) && defined(S2RRAM_SIZE)
    /* Placing the section .s2rram in S2RRAM region. Only uninitialized
     * objects may be placed in this section.
     */
    .s2rram         :   > S2RRAM, type = NOINIT
#endif

    .log_data       :   > LOG_DATA, type = COPY
    .log_ptr        : { *(.log_ptr*) } > LOG_PTR align 4, type = COPY
}
