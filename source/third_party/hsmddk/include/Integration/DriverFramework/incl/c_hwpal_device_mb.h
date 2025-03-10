/* c_hwpal_device_mb.h
 *
 * Configuration Handling for Driver Framework Device API implementation.
 * A build-level configuration file is included and sanity-checked.
 * Do not edit this file. Edit cs_hwpal.h instead.
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

/*----------------------------------------------------------------
 * get configuration settings from product config file
 */

// Top-level Hardware platform configuration
// DriverLib interface
#include <DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_memmap.h)

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
    #include <third_party/hsmddk/include/Config/cc35xx/cs_hwpal_mb.h>
#else
    #include <third_party/hsmddk/include/Config/cc27xx/cs_hwpal_mb.h>
#endif

// Enables strict argument checking for input parameters
//#define HWPAL_STRICT_ARGS_CHECK

// choose from LOG_SEVERITY_INFO, LOG_SEVERITY_WARN, LOG_SEVERITY_CRIT
#ifndef HWPAL_LOG_SEVERITY
#define HWPAL_LOG_SEVERITY  LOG_SEVERITY_INFO
#endif

#ifndef HWPAL_MAX_DEVICE_NAME_LENGTH
#error "Expected HWPAL_MAX_DEVICE_NAME_LENGTH defined by cs_hwpal_umdevxs.h"
#endif

#ifndef HWPAL_DEVICES
#error "Expected HWPAL_DEVICES defined by cs_hwpal_umdevxs.h"
#endif

#ifndef HWPAL_REMAP_ADDRESSES
#error "Expected HWPAL_REMAP_ADDRESSES defined by cs_hwpal_umdevxs.h"
#endif

// Define to remove the PCI configuration space support
//#define HWPAL_REMOVE_DEVICE_PCICONFIGSPACE

/* HWPAL_DEVICE_READ_DELAY_US
 *
 * Device_Read32() function will busy wait for the specified number
 * of microseconds before actually performing the read operation.
 *
 * Note: This is a workaround for posted-writes implementation of
 *       the MMIO register slave interface
 *       (when too fast write-read sequence does not work)
 *
 *
 */
//#define HWPAL_DEVICE_READ_DELAY_US    100

// Maximum number of supported devices,
// must be equal to or greater than the number of devices in the HWPAL_DEVICES
//#define HWPAL_DEVICE_COUNT      1

// Address where the device list is mapped in the MMIO

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
    #define HWPAL_DEVICE_MEM_ADDR   HSM_BASE
#else
    #define HWPAL_DEVICE_MEM_ADDR   HSMCRYPTO_BASE
#endif

/* end of file c_hwpal_device_mb.h */
