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
******************************************************************************
*  \file       hw_ccfg.h
*
*  \brief      Customer Configuration (CCFG) header file.
******************************************************************************/

#ifndef __HW_CCFG_H__
#define __HW_CCFG_H__

#include <stdint.h>
#include "hw_device.h"

/*! CCFG sector data structure definition
 *  This structure should be allocated at the base of CCFG section defined in
 *  the linker file.
 *
 *  \note The CCFG section is only accessible from secure context.
 */
typedef struct {

    // Bootloader/application configuration
    struct {    // [0]: length 16B
        // Pointer to user bootloader vector table
        void *pBldrVtor;
            #define CCFG_BC_PBLDR_USE_FCFG ((void*)((uint32_t*)0xFFFFFFF0U))
            #define XCFG_BC_PBLDR_FORBID   ((void*)((uint32_t*)0xFFFFFFFCU))
            #define XCFG_BC_PBLDR_UNDEF    ((void*)((uint32_t*)0xFFFFFFFFU))
            #define CCFG_BC_PBLDR_VALID(x) ((x) < CCFG_BC_PBLDR_USE_FCFG)
        // Parameter passed to bootloader
        union {
            uint32_t val32;
                #define CCFG_BC_BLDRCFG_UNDEF   0xFFFFFFFFU
            // Serial ROM bootloader parameters (also used in FCFG.h)
            struct serialRomBldrParam_struct {
                uint32_t bldrEnabled        : 1;
                    #define XCFG_BC_BLDR_DIS  0
                    #define XCFG_BC_BLDR_EN   1
                uint32_t pinTriggerEnabled  : 1;
                    #define XCFG_BC_PINTRIG_DIS  0
                    #define XCFG_BC_PINTRIG_EN   1
                uint32_t pinTriggerLevel   : 1;
                    #define XCFG_BC_PINTRIG_LEVEL_LO  0U
                    #define XCFG_BC_PINTRIG_LEVEL_HI  1
                uint32_t res0 : 13;
                uint32_t pinTriggerDio : 6;
                uint32_t res1 : 2;
                uint32_t serialIoCfgIndex : 3;
                    #define XCFG_BC_IOCFGIND_DEFAULT  0
                uint32_t res2 : 5;
            } serialRomBldrParamStruct;
        } bldrParam;
        // Pointer to application VTOR table
        void *pAppVtor;
            #define CCFG_BC_PAPP_NONE  ((void*)((uint32_t*)0xFFFFFFFFU))
        uint32_t crc32;
    } bootCfg;

    uint32_t res0[2];

    // Device permissions   [24]: length 4 B
    // This is maximally-restrictive combined with similar fields in FCFG and SCFG
    struct {
        #define CCFG_PERMISSION_ALLOW  0xAU
        #define CCFG_PERMISSION_FORBID 0x0U
        // (all other value other than ALLOW are interpreted as FORBID)
        uint32_t allowReturnToFactory : 4;
        uint32_t allowFakeStby        : 4;
        uint32_t allowToolsClientMode : 4;
        uint32_t allowChipErase       : 4;
        uint32_t allowFlashProgram    : 4;
        uint32_t allowFlashVerify     : 4;
        uint32_t allowEnergyTrace     : 4;
        uint32_t allowDebugPort       : 4;
    } permissions;

    // Miscellaneous fields         [28]: length 4B
    struct {
        // SACI timeout is infinite when 0, else (2^saciTimeoutExp)*64 ms
        // Fcfg timeout applied instead if CCfg.saciTimeoutOverride==0
        uint32_t saciTimeoutExp      : 3;
            #define XCFG_MISC_SACITOEXP_8SEC        7
            #define XCFG_MISC_SACITOEXP_1SEC        4
            #define XCFG_MISC_SACITOEXP_INFINITE    0
        uint32_t saciTimeoutOverride : 1;
        uint32_t allowMainAppErase   : 4;    // Extension of permissions above
        uint32_t res0                : 24;
    } misc;

    // Flash protection     [32]: length 32 B
    // This is maximally-restrictive combined with similar field in FCFG
    struct {
        struct {
            // Sticky-0 bits written to VIMS.WEPRA (sectors 0-31, 1/bit)
            uint32_t mainSectors0_31;
            // Sticky-0 bits written to VIMS.WEPRB(0) (sectors 32-255, 8/bit)
            uint32_t mainSectors32_255;
            // Sticky-0 bits written to VIMS.WEPRB1 (sectors 256-511, 8/bit)
            uint32_t mainSectors256_511;
            // Sticky-0 bit written to VIMS.WEPRAUX
            union {
                uint32_t auxSectors;
                struct {
                    uint32_t ccfgSector  : 1;
                    uint32_t fcfgSector  : 1;
                    uint32_t engrSector  : 1;
                    uint32_t scfgSector  : 1;
                    uint32_t vlogSector  : 1;
                    uint32_t res1        : 27;
                };
            };
        } writeEraseProt;
        union {
            uint32_t val32;
            struct {
                // Sticky-0 bits written to VIMS.RDPRMN (0-4 2K blocks at beginning of main)
                uint32_t mainSectors    : 4;
                uint32_t res0           : 4;
                // Sticky-0 bits written to VIMS.RDPRNMN (watermark at end-x*16B)
                uint32_t ccfgSector     : 6;
                    #define XCFG_READPROT_16B_BLOCKS(x) ( (~(x)) & 0x3F )
                uint32_t res1           : 18;
            };
        } readProt;
        struct {
            // Set bits (sectors 0-31, 1/bit) define what a chip erase command can optionally retain
            uint32_t mainSectors0_31;
            // Set bits (sectors 32-255, 8/bit) define what a chip erase command can optionally retain
            uint32_t mainSectors32_255;
            // Set bits (sectors 256-511, 8/bit) define what a chip erase command can optionally retain
            uint32_t mainSectors256_511;
        } chipEraseRetain;
    } flashProt;

    // Optional HW initialization copy-list   [64]: length x B
    // Copy list applied before user application is entered. May be used by customer/SYSCFG to
    // initialize hardware right before application is entered.
    // Also used to pad out CCFG to correct size
    uint32_t hwInitCopyList[(FLASH_1T_SECTOR_SIZE / 4U) - 61U];
        // Simple macros to assist in initializing copy lists
        // NOTE: Addresses to CPYLIST_CPY must fulfill ((a&0x0FF00003)==0).
        //       The memory map ensures this for SRAM and peripherals.
        //       There is no check as ELF only supports {symbol + const offset}
        #define CPYLST_EOL 0
        #define CPYLST_NOP               0x10000000
        #define CPYLST_WAIT(x)           ( 0x10000000 + (((x)<<2)&0x000FFFFC) )
        #define CPYLST_CPY(a, n)         ( ((uint32_t)(a)) + (((n)<<20)&0x0FF00000) )
        #define CPYLST_CPYFULLADDR(a)    (((uint32_t)(a)) + 1)
        #define CPYLST_JUMP(a)           (((uint32_t)(a)) + 2)
        #define CPYLST_CALL(a)           (((uint32_t)(a)) + 3)

    // CRC of content after bootCfg through hwInitCopyList
    // [End-180]: length 4B
    uint32_t crc32;

    // User record (programmable also through separate SACI command), no dependencies in boot code
    // User record size is fixed at 128 B. Last word assumed to be CRC over first 124 B (optional)
    #define CCFG_USER_RECORD_SIZE  128
    union {    // [End-176]: length 128B
        struct {
            union {
                // Generic 32b record layout
                uint32_t val32[(CCFG_USER_RECORD_SIZE/4)-1];
                // Generic 8b record layout
                uint8_t  val8[CCFG_USER_RECORD_SIZE-4];
            };
            // CRC field across first 124B of userRecord (supported by SACI verifyCcfg command)
            uint32_t crc32;
        };
    } userRecord;

    // *******************************************************
    // *** Read protection security barrier (16 B aligned) ***
    // *******************************************************
    // Note: The CCFG field flashProt.readProt.ccfgSector can set the security barrier here or
    //       at some point in the userRecord above. debugCfg.pwdId only readable through
    //       SACI command if read protection barrier set here [on purpose]

    // Debug configuration and password
    struct {    // [End-48]: length 48B
        // Debug authorization requirements
        uint8_t authorization;
            #define CCFG_DBGAUTH_REQAUTH            0xA5U
            #define CCFG_DBGAUTH_DBGOPEN            0x5AU
            #define CCFG_DBGAUTH_ONLY_NON_INVASIVE  0xC3U
            #define CCFG_DBGAUTH_DBGFORBID          0x00U
            // (and any other value)
        uint8_t res0[43];
        // CRC32 of previous fields in debugCfg
        uint32_t crc32;
    } debugCfg;
} ccfg_t;


/*! Typedef of substruct as it is also used in hw_fcfg.h and in the bootloader itself */
typedef struct serialRomBldrParam_struct serialRomBldrParam_t;


#ifndef DRIVERLIB_NS // CCFG is not accessible in non-secure mode.
/*! \brief Extern declaration of the CCFG struct.
 *  This extern declaration can be used to access CCFG fields from other parts
 *  of the code base.
 *  This extern assumes that the ccfg_t struct allocated in the system is called
 *  "ccfg".
 *
 *  \note The CCFG section is only accessible from secure context.
 */
extern const ccfg_t ccfg;
#endif // DRIVERLIB_NS

#endif // __HW_CCFG_H__
