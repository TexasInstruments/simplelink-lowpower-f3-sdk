/** @file cs_umdevxs.h
 *
 * @brief Configuration Settings for the UMDevXS driver (Linux user-space).
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

#ifndef INCLUDE_GUARD_CS_UMDEVXS_H
#define INCLUDE_GUARD_CS_UMDEVXS_H

/*----------------------------------------------------------------------------
 * Definitions and macros
 */

/** logging level */
#ifndef LOG_SEVERITY_MAX
#define LOG_SEVERITY_MAX LOG_SEVERITY_WARN
#endif

// uncomment to remove selected functionality
//#define UMDEVXS_REMOVE_DEVICE
//#define UMDEVXS_REMOVE_DMABUF
//#define UMDEVXS_REMOVE_SMBUF
#define UMDEVXS_REMOVE_SMBUF_PROVIDER
#define UMDEVXS_REMOVE_SMBUF_OBTAINER
#define UMDEVXS_REMOVE_SIMULATION
//#define UMDEVXS_REMOVE_INTERRUPT

// Enable support for kernel modules for SHDevXS
//#define UMDEVXS_ENABLE_KERNEL_SUPPORT

// Use user-space buffer address for CPU Data Cache control (flush/invalidate)
#define UMDEVXS_DCACHE_CTRL_UADDR

#if defined(ARCH_ARM) || defined(ARCH_ARM64)
// Use non-cached DMA buffer mapping to user-space
#define UMDEVXS_SMBUF_UNCACHED_MAPPING

// If defined then the CPU D-Cache control for DMA buffers
// must be done entirely in user space
#define UMDEVXS_DCACHE_CTRL_USERMODE

// If defined then the low-level UMDevXS functions will be used for
// the direct D-Cache control (flush/invalidate) for DMA buffers
// unless UMDEVXS_SMBUF_CACHE_COHERENT is defined
//#define UMDEVXS_SMBUF_DIRECT_DCACHE_CONTROL

#endif // defined(ARCH_ARM) || defined(ARCH_ARM64)

#define UMDEVXS_PCI_DEVICE_RESOURCE_BYTE_COUNT 0

// Board or hardware platform specific extensions
#include "cs_umdevxs_ext.h"


#endif /* INCLUDE_GUARD_CS_UMDEVXS_H */

/* end of file cs_umdevxs.h */
