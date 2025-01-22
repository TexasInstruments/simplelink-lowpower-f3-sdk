/******************************************************************************
*  @file      cc23x0r53.lds
*  @brief     CC23X0R53 linker configuration file for GNU toolchain.
*
*  Copyright (c) 2024 Texas Instruments Incorporated
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

/* Symbols used for checksum generation */
ti_utils_build_GenMap_sym_CRC_CCFG_BOOT_CFG_begin = 0x4e020000;
ti_utils_build_GenMap_sym_CRC_CCFG_BOOT_CFG_end = 0x4e02000b;
ti_utils_build_GenMap_sym_CRC_CCFG_begin = 0x4e020010;
ti_utils_build_GenMap_sym_CRC_CCFG_end = 0x4e02074b;
ti_utils_build_GenMap_sym_CRC_CCFG_DEBUG_begin = 0x4e0207d0;
ti_utils_build_GenMap_sym_CRC_CCFG_DEBUG_end = 0x4e0207fb;
ti_utils_build_GenMap_sym_CRC_CCFG_USER_RECORD_begin = 0x4e020750;
ti_utils_build_GenMap_sym_CRC_CCFG_USER_RECORD_end = 0x4e0207cb;

/* Entry Point */
ENTRY( ResetISR )

/* System memory map */
MEMORY
{
    /* Application is stored in and executes from internal flash */
    FLASH (RX) : ORIGIN = 0x0, LENGTH = 0x80000
    /* Customer Configuration Area (CCFG) */
    CCFG (R) : ORIGIN = 0x4E020000, LENGTH = 0x800
    /* Application uses internal RAM for data */
    SRAM (RWX) : ORIGIN = 0x20000000, LENGTH = 0x10000
}

/*. Highest address of the stack. Used in startup file .*/
_estack = ORIGIN(SRAM) + LENGTH(SRAM); /*end of SRAM .*/

/*. Generate a link error if heap and stack does not fit into RAM .*/
_Min_Heap_Size = 0;
_Min_Stack_Size = 0x100;


/* Section allocation in memory */
SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.resetVecs))
        *(.text*)
        *(.rodata*)
        *(.init)
        *(.fini*)
        *(.eh_frame*)
        _etext = .;
    } > FLASH = 0

   .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    .init_array :
    {
        _init_array = .;
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array*))
        _einit_array = .;
    } > FLASH

    .data :
    {
        _data = .;
        *(.data*)
        . = ALIGN (2);
        *(.TI.ramfunc)
        . = ALIGN (4);
        _edata = .;
    } > SRAM AT > FLASH
    _ldata = LOADADDR(.data);

    .ramVecs (NOLOAD) : ALIGN(256)
    {
    	*(.ramVecs)
    } > SRAM

    .bss :
    {
        __bss_start__ = .;
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
        __bss_end__ = .;
    } > SRAM

    .ccfg :
    {
        KEEP(*(.ccfg));
    } > CCFG

    /* User_heap_stack section, used to check that there is enough SRAM left */
    ._user_heap_stack :
    {
      . = ALIGN(4);
      . = . + _Min_Heap_Size;
      . = . + _Min_Stack_Size;
      . = ALIGN(4);
    } > SRAM
}
