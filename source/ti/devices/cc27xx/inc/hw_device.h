/******************************************************************************
*  Copyright (c) 2021-2024 Texas Instruments Incorporated. All rights reserved.
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
#ifndef __HW_DEVICE_H__
#define __HW_DEVICE_H__

#include "hw_memmap.h"
#include "hw_platform.h"

//*****************************************************************************
//
// Address, offset and size definitions
//
//*****************************************************************************

// --- ROM definitions ---
/// Size of the call stack used in ROM, in number of bytes
#define BOOT_CSTACK_SIZE                        (2048U-sizeof(fcfg_appTrims_t))

// --- Flash definitions ---
/// Size of a 1T flash sector, in number of bytes
#define FLASH_1T_SECTOR_SIZE                    2048U
/// Flash word width in number of bits
#define FLASH_DATAWIDTH_BITS                    128U
/// Size of a 2T flash sector, in number of bytes
#define FLASH_2T_SECTOR_SIZE                    (FLASH_1T_SECTOR_SIZE/2U)
/// Size of a MAIN flash sector, in number of bytes
#define FLASH_MAIN_SECTOR_SIZE                  (FLASH_1T_SECTOR_SIZE)
/// Size allocated for HSM firmware image
#define HSM_FW_SIZE                             0x18000U
/// Available flash space for SW when used together with HSM firmware
#define FLASH_MAIN_SW_SIZE                      (FLASH_MAIN_SIZE-HSM_FW_SIZE)

// --- Factory Configuration (FCFG) definitions ---
/// Size of generalTrims section in FCFG, in number of 32-bit words
#define FCFG_GENERALTRIMS_SIZE                  ((FLASH_2T_SECTOR_SIZE / 4U) - 128U)

// --- Customer Configuration (CCFG) definitions ---
/*! Macro for default CCFG configuration. All fields can be overwritten by
 *  setting the field to another value further down in the struct definition
 *  within a C file.
 */
#define CCFG_DEFAULT_VALUES \
    .bootCfg = { \
        .pBldrVtor = XCFG_BC_PBLDR_UNDEF, \
        .bldrParam = { 0x00000000 }, \
        .pAppVtor = CCFG_BC_PAPP_NONE, \
        .crc32 = 0x0BAD0BAD \
    }, \
    .hwOpts[0] = 0xFFFFFFFFU, \
    .hwOpts[1] = 0xFFFFFFFFU, \
    .permissions = { \
        .allowReturnToFactory   = CCFG_PERMISSION_ALLOW, \
        .allowFakeStby          = CCFG_PERMISSION_ALLOW, \
        .allowToolsClientMode   = CCFG_PERMISSION_ALLOW, \
        .allowChipErase         = CCFG_PERMISSION_ALLOW, \
        .allowFlashProgram      = CCFG_PERMISSION_ALLOW, \
        .allowFlashVerify       = CCFG_PERMISSION_ALLOW, \
        .allowEnergyTrace       = CCFG_PERMISSION_ALLOW, \
        .allowDebugPort         = CCFG_PERMISSION_ALLOW, \
    }, \
    .misc = { \
        .saciTimeoutOverride = 0, \
        .allowMainAppErase   = CCFG_PERMISSION_ALLOW, \
    }, \
    .flashProt = { \
        .writeEraseProt = { \
            .mainSectors0_31    = 0xFFFFFFFFU, \
            .mainSectors32_255  = 0xFFFFFFFFU, \
            .mainSectors256_511 = 0xFFFFFFFFU, \
            .auxSectors         = 0xFFFFFFFFU, \
        }, \
        .readProt = { 0xFFFFFFFFU }, \
        .chipEraseRetain = { \
            .mainSectors0_31    = 0, \
            .mainSectors32_255  = 0, \
            .mainSectors256_511 = 0, \
        }, \
    }, \
    .hwInitCopyList = { CPYLST_EOL }, \
    .crc32 = 0x0BAD0BAD, \
    .userRecord.crc32 = 0x0BAD0BAD, \
    .debugCfg = { \
        .authorization = CCFG_DBGAUTH_DBGOPEN, \
        .res0 = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, \
        .crc32 = 0x0BAD0BAD, \
    }

// --- Security Configuration (SCFG) definitions ---
/*! Macro for default SCFG configuration. All fields can be overwritten by
 *  setting the field to another value further down in the struct definition
 *  within a C file.
 */
// Macro for default SCFG configuration.
#define SCFG_DEFAULT_VALUES \
    .hsmCfg = { \
        .publicKeyHash =   SCFG_UNDEFINED_KEY_HASH , \
    }, \
    .debugAuthCfg = { \
        .secureKey = { \
            .keyId = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, \
            .publicKeyHash =  SCFG_UNDEFINED_KEY_HASH , \
        }, \
        .nonSecureKey = { \
            .keyId = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, \
            .publicKeyHash =  SCFG_UNDEFINED_KEY_HASH , \
        }, \
        .challengeVector = { \
            .lifetime = SCFG_DBGAUTH_EPHEMERAL_LIFETIME, \
            .deviceConst = SCFG_DBGAUTH_DEVICE_MAC_CONST, \
        }, \
    }, \
    .flashCfg = { \
        .flashLayout = { \
            .primaryAppSlots[0] = { .addr = SCFG_SLOT_ADDR_UNDEF, .len = SCFG_SLOT_LEN_UNDEF}, \
            .primaryAppSlots[1] = { .addr = SCFG_SLOT_ADDR_UNDEF, .len = SCFG_SLOT_LEN_UNDEF}, \
            .secondaryAppSlots[0] = { .addr = SCFG_SLOT_ADDR_UNDEF, .len = SCFG_SLOT_LEN_UNDEF}, \
            .secondaryAppSlots[1] = { .addr = SCFG_SLOT_ADDR_UNDEF, .len = SCFG_SLOT_LEN_UNDEF}, \
            .bldrSlot = { .addr = SCFG_SLOT_ADDR_UNDEF, .len = SCFG_SLOT_LEN_UNDEF}, \
        }, \
        .res0 = { 0x00, 0x00, 0x00, 0x00 }, \
    }, \
    .secBootCfg = { \
        .policyCfg = { \
            .authMethod = SCFG_POLICY_NO_AUTH, \
            .authAlgorithm = SCFG_POLICY_ALG_RSA_3K_SHA256, \
            .mode = SCFG_POLICY_XIP_REVERT_DISABLED, \
        }, \
        .keyUpdateKeyHash =  SCFG_UNDEFINED_KEY_HASH , \
    }, \
    .permissions = { \
        .allowReturnToFactory   = SCFG_PERMISSION_ALLOW, \
        .allowFakeStby          = SCFG_PERMISSION_ALLOW, \
        .allowToolsClientMode   = SCFG_PERMISSION_ALLOW, \
        .allowChipErase         = SCFG_PERMISSION_ALLOW, \
        .allowFlashProgram      = SCFG_PERMISSION_ALLOW, \
        .allowFlashVerify       = SCFG_PERMISSION_ALLOW, \
        .allowEnergyTrace       = SCFG_PERMISSION_ALLOW, \
        .allowDebugPort         = SCFG_PERMISSION_ALLOW, \
        .allowMainAppErase      = SCFG_PERMISSION_ALLOW, \
    }, \
    .emSensorCfg = SCFG_EMSENSOR_ENABLE, \
    .bootSeedOffset = SCFG_BOOT_SEED_DISABLED, \
    .res0 = { \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
    }, \
    .crc32 = 0x0BAD0BADU, /* Inserted in build */ \
    .keyRingCfg = { \
        .keyEntries = { \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
            { SCFG_UNDEFINED_KEY_ENTRY }, \
        }, \
    }


#endif // __HW_DEVICE_H__
