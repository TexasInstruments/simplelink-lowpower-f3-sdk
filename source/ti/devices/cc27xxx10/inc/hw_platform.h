/******************************************************************************
*  Copyright (c) 2021-2025 Texas Instruments Incorporated. All rights reserved.
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
*  3) Neither the name of the copyright holder nor the names of its contributors
*     may be used to endorse or promote products derived from this software
*     without specific prior written permission.
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
******************************************************************************/
#ifndef __HW_PLATFORM_H__
#define __HW_PLATFORM_H__
#include "hw_memmap.h"
#include "hw_pmctl.h"
#include "hw_sys0.h"
#include "hw_systim.h"

//*****************************************************************************
//
// Address, offset and constant definitions
//
//*****************************************************************************
// These definitions are used to generate an include file to the linker script
// in ../../linker/<target-device>.icf or ../../linker/common.icf.
// See also <target-device>/device.h

// --- ROM definitions ---
/// Address of ROM FW revision structure
#define ROM_FWREV_ADDR                          (ROM_BASE+0x0048)
/// Base address of HAPI table in ROM
#define ROM_HAPI_BASE                           (ROM_BASE+0x004C)
/// Address of CRC32 calculated over entire ROM
#define ROM_CRC_ADDR                            (ROM_BASE+ROM_SIZE-4)

// --- SRAM definitions ---
/// Magic value used in Fcfg.criticalTrim.sramRepair.magicWord to
/// indicate that SRAM repair information is valid
#define SRAMREP_MAGICWORD                       0x40008100



//*****************************************************************************
//
// Boot status definitions (available through PMCTL::BOOTSTA) in addition to
// the defines listed in hw_pmctl.h
//
//*****************************************************************************
// TODO: Move the two XX_START_INIT defines to hw_pmctl.h (LPRFROM-661).

    /// Mode mask
    #define PMCTL_BOOTSTA_FLAG_MODE_M                 (PMCTL_BOOTSTA_FLAG_MODE_APP)
/// Boot sequence completed
#define PMCTL_BOOTSTA_BOOT_COMPLETE               (PMCTL_BOOTSTA_FLAG_MODE_BLDR)
/// Bootloader start initiated
#define PMCTL_BOOTSTA_BLDR_START_INIT             (PMCTL_BOOTSTA_FLAG_MODE_BLDR | 0x02)
/// ROM serial bootloader complete
#define PMCTL_BOOTSTA_BLDR_COMPLETE               (PMCTL_BOOTSTA_FLAG_MODE_APP)
/// Application start initiated
#define PMCTL_BOOTSTA_APP_START_INIT              (PMCTL_BOOTSTA_FLAG_MODE_APP | 0x02)


//-----------------------------------------------------------------------------
// Define one-hot encodings (single bit set)  for the various lifecycle states
//
// This allows tests against multiple possible lifecycles to be done
// against the bmLifecycle variable efficiently and in a legible way in code
//-----------------------------------------------------------------------------
#define BM_LC_P1STBDAY (1U << SYS0_LIFECYC_VAL_LCYCLE_POSSIBLE_1STBDAY) ///< One-hot encoding of SYS0_LIFECYC_VAL_LCYCLE_POSSIBLE_1STBDAY
#define BM_LC_1STBDAY  (1U << SYS0_LIFECYC_VAL_LCYCLE_1STBDAY)          ///< One-hot encoding of SYS0_LIFECYC_VAL_LCYCLE_1STBDAY
#define BM_LC_TESTPT   (1U << SYS0_LIFECYC_VAL_LCYCLE_TESTPT)           ///< One-hot encoding of SYS0_LIFECYC_VAL_LCYCLE_TESTPT
#define BM_LC_TESTFT   (1U << SYS0_LIFECYC_VAL_LCYCLE_TESTFT)           ///< One-hot encoding of SYS0_LIFECYC_VAL_LCYCLE_TESTFT
#define BM_LC_ENGRDEV  (1U << SYS0_LIFECYC_VAL_LCYCLE_ENGRDEV)          ///< One-hot encoding of SYS0_LIFECYC_VAL_LCYCLE_ENGRDEV
#define BM_LC_PRODDEV  (1U << SYS0_LIFECYC_VAL_LCYCLE_PRODDEV)          ///< One-hot encoding of SYS0_LIFECYC_VAL_LCYCLE_PRODDEV
//-----------------------------------------------------------------------------
    // This macro converts the one-hot encoded macros above into a two-hot (TH)
    // encoded value for the purposes of an input param to BootSetLifecycleState().
    #define TH_LC(bm) (((uint16_t)bm & 0x00FFU) | (((uint16_t)bm << 8) & 0xFF00U))  ///< Convert a one-hot encoded bm to a two-hot encoded bm


#if !(defined(__ASM_INCLUDE__) || defined(GENERATE_LINKER_INCLUDE_FILE))
// Only included in C files

    /// Data type for passing flags to bootloader/application entry function
    typedef union {
        uint32_t val32;                         ///< 32b value of word
        struct {
            uint32_t bBldrCcfgValid         : 1;    ///< Is CCFG valid?
            uint32_t bBldrAppCanBoot        : 1;    ///< Does a bootable application exist?
            uint32_t bChipEraseAllowed  : 1;    ///< Is ChipErase operation allowed?
            uint32_t bParamsFromCcfg    : 1;    ///< Entry function params argument is CCFG(1) or FCFG(0)
            uint32_t bBldrAllowDbg      : 1;    ///< Is debugging of bootloader allowed?
            uint32_t res0               : 27;   ///< (Reserved for future use)
        } bldr;
    } bldrEntryFlags_t;

#endif //!(defined(__ASM_INCLUDE__) || defined(GENERATE_LINKER_INCLUDE_FILE))


#endif // __PLATFORM_H__
