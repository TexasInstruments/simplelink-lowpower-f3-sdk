/* cs_umdevxs_ext.h
 *
 * Configuration Switches for UMDevXS driver
 */

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*   Module        : DDK-130_bsd                                              */
/*   Version       : 4.1.1                                                    */
/*   Configuration : DDK_EIP130_BSD                                           */
/*                                                                            */
/*   Date          : 2023-Mar-10                                              */
/*                                                                            */
/* Copyright (c) 2014-2023 by Rambus, Inc. and/or its subsidiaries.           */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or without         */
/* modification, are permitted provided that the following conditions are     */
/* met:                                                                       */
/*                                                                            */
/* 1. Redistributions of source code must retain the above copyright          */
/* notice, this list of conditions and the following disclaimer.              */
/*                                                                            */
/* 2. Redistributions in binary form must reproduce the above copyright       */
/* notice, this list of conditions and the following disclaimer in the        */
/* documentation and/or other materials provided with the distribution.       */
/*                                                                            */
/* 3. Neither the name of the copyright holder nor the names of its           */
/* contributors may be used to endorse or promote products derived from       */
/* this software without specific prior written permission.                   */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR      */
/* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT       */
/* HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT           */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,      */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY      */
/* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT        */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE      */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.       */
/* -------------------------------------------------------------------------- */

#ifndef INCLUDE_GUARD_CS_UMDEVXS_EXT_H
#define INCLUDE_GUARD_CS_UMDEVXS_EXT_H

//#define UMDEVXS_REMOVE_DEVICE_OF
#define UMDEVXS_REMOVE_PCI
#define UMDEVXS_REMOVE_SMALL_PCIWINDOW_SUPPORT

// Device name in the Device Tree Structure
#define UMDEVXS_PLATFORM_DEVICE_NAME  "EIP130"

// Index of the IRQ in the "interrupts" property of the Open Firmware device
// tree entry. 0 is the first IRQ listed, 1 is the second IRQ listed, etc.
#define UMDEVXS_PLATFORM_IRQ_IDX 1

#define UMDEVXS_LOG_PREFIX "UMDevXS: "

#define UMDEVXS_MODULENAME "umdevxs"

// definition of device resources
//                           Name      Start    Last
#define UMDEVXS_DEVICES \
      UMDEVXS_DEVICE_ADD("EIP130",          0x40000000, 0x401FFFFF), \
      UMDEVXS_DEVICE_ADD("EIP130_CFG",      0x48000000, 0x4807FFFF),


// Filter for tracing interrupts: 0 - no traces, 0xFFFFFFFF - all interrupts
#define UMDEVXS_INTERRUPT_TRACE_FILTER  0x0


#endif /* INCLUDE_GUARD_CS_UMDEVXS_EXT_H */

/* end of file cs_umdevxs_ext.h */
