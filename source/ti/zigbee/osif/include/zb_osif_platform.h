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

#ifndef ZB_OSIF_PLATFORM_H
#define ZB_OSIF_PLATFORM_H 1

/**
@par OS/HW abstraction
OS/HW platform abstraction is necessary to achieve high portability.
C language tool to separate abstraction layer is C preprocessor.

Main idea is to avoid number of ifdefs related to portability in the code, and
decrease number of ifdefs in the header files not related to the OS abstraction
layer.

Platform abstraction is implemented as C functions placed into OS abstraction
layers and platform-dependent global typedefs and definitions placed into header
files.  All platform-related stuff is in osif/ directory.  Global definitions
and typedefs can be used anywhere - that is why on the architecture picture OS
abstraction layer depicted as global.

Following things are platform-dependent:
*   typedefs for base types (8-bit controller vs 32-bit Linux device)
*   definitions for different 8051 compilers (SDCC and Keil)
*   transceiver i/o (interrupts handling for 8051 vs file i/o in Linux);
*   wait for i/o (device sleep for 8051, wait in select() in Linux)
*   trace i/o (UART for 8051, file in Linux);
*   MAC traffic dump (UART for 8051, file in Linux);
*   Timer (8051 timer at device, select() timeout in Linux)

 */

#ifdef __IAR_SYSTEMS_ICC__
#ifndef ZB_IAR
#define ZB_IAR
#endif
#endif

/**
  OSIF platform selection. One of pre-defined platform should be selected in
  zb_config.h configurations
*/
#ifdef ZB_TI_F3_ZBOSS
#include <string.h>
#include <stdint.h>
#include "zb_hal_utils.h"
#include "zb_hal_serial.h"
#include "zb_hal_flash.h"
#include "zb_hal_timer.h"
#include "zb_hal_crypto.h"
#include "zb_hal_led_buttons.h"
#include "mac_platform.h"
#else
#error Port me!
#endif

#endif /* ZB_OSIF_PLATFORM_H */
