/******************************************************************************
*  Copyright (c) 2023 Texas Instruments Incorporated. All rights reserved.
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
*  \file       hw_scfg.h
*
*  \brief      Security Configuration (SCFG) header file.
******************************************************************************/

#ifndef __HW_SCFG_H__
#define __HW_SCFG_H__
#include <stdint.h>
#include "hw_device.h"

/*! SCFG sector data structure definition
 *  This structure should be allocated at the base of SCFG section defined in
 *  the linker file.
 */
typedef struct {
    // HSM Config
    // [0]: length 32B
    struct {
        // SHA-256 hash of the Customer owned public RSA key
        // This public key hash will be initialized, by ROM, into a HSM static asset with a asset number of 94
        uint8_t publicKeyHash[32];
    } hsmCfg;


    // Debug Auth Config
    // [32]: length 88B
    struct {
        // Secure Key
        struct { // length: 40B
            // 8B Key ID used for lookup in customer database
            uint8_t keyId[8];
            // SHA-256 hash of the debug key (key type depends on secBootCfg.policyCfg.authAlgorithm)
            uint8_t publicKeyHash[32];
        } secureKey;
        // Non Secure Key
        struct { // length: 40B
            // 8B Key ID used for lookup in customer database
            uint8_t keyId[8];
            // SHA-256 hash of the debug key (key type depends on secBootCfg.policyCfg.authAlgorithm)
            uint8_t publicKeyHash[32];
        } nonSecureKey;
        // Challenge Vector
        struct { // length: 8B
            // Lifetime
            uint32_t lifetime;
                #define SCFG_DBGAUTH_EPHEMERAL_LIFETIME 0xF1A1A5A5U
                #define SCFG_DBGAUTH_ENDLESS_LIFETIME   0x51445A5AU
            // Device Constant
            uint32_t deviceConst;
                #define SCFG_DBGAUTH_DEVICE_MAC_CONST   0x3262A5A5U
                #define SCFG_DBGAUTH_ZERO_CONST         0x62BB5A5AU
        } challengeVector;
    } debugAuthCfg;


    // Flash config
    // [120]: length 44B
    struct {
        // Flash Layout
        struct { // length: 40B
            struct slot {
                uint32_t addr;
                    #define SCFG_SLOT_ADDR_UNDEF    0xFFFFFFFFU
                uint32_t len;
                    #define SCFG_SLOT_LEN_UNDEF     0xFFFFFFFFU
            } primaryAppSlots[2], secondaryAppSlots[2], bldrSlot;
        } flashLayout;
        uint8_t res0[4];
    } flashCfg;


    // Secure boot config
    // [164]: length 44B
    struct {
        // Secure boot policy config
        struct { // length: 12B
            uint32_t authMethod;
                #define SCFG_POLICY_NO_AUTH     0x2F8EA5A5U /* Disables secure boot entirely */
                #define SCFG_POLICY_SIGNATURE   0x8B635A5AU
                #define SCFG_POLICY_HASH_LOCK   0xA9F0C3C3U
            uint32_t authAlgorithm;
                #define SCFG_POLICY_ALG_RSA_3K_SHA256       0x429CA5A5U
                #define SCFG_POLICY_ALG_ECDSA_P256_SHA256   0x03B95A5AU
                #define SCFG_POLICY_ALG_ECDSA_P521_SHA512   0x0C7FC3C3U
            uint32_t mode;
                #define SCFG_POLICY_OVRWRT                  0x92B1A5A5U /* Required if >=3 appSlots are defined */
                #define SCFG_POLICY_XIP_REVERT_ENABLED      0x15D45A5AU /* Previous image is NOT erased */
                #define SCFG_POLICY_XIP_REVERT_DISABLED     0x35AFC3C3U /* Previous will be erased */
        } policyCfg;
        // SHA-256 hash of the key update key (key type depends on secBootCfg.policyCfg.authAlgorithm)
        uint8_t keyUpdateKeyHash[32];
    } secBootCfg;


    // Device permissions
    // [208]: length 8B
    // This is maximally-restrictive combined with similar field in FCFG & CCFG
    struct {
        #define SCFG_PERMISSION_ALLOW   0xAU
        #define SCFG_PERMISSION_FORBID  0x0U
        // (all other value other than ALLOW are interpreted as FORBID)
        uint32_t allowReturnToFactory : 4;
        uint32_t allowFakeStby        : 4;
        uint32_t allowToolsClientMode : 4;
        uint32_t allowChipErase       : 4;
        uint32_t allowFlashProgram    : 4;
        uint32_t allowFlashVerify     : 4;
        uint32_t allowEnergyTrace     : 4;
        uint32_t allowDebugPort       : 4;
        uint32_t allowMainAppErase    : 4;
        uint32_t res0                 : 28;
    } permissions;


    // EM Sensor config
    // [216]: length 4B
    uint32_t emSensorCfg;
        /* All values other than '*_ENABLE' and '*_DISABLE' will be treated as '*_ENABLE' */
        #define SCFG_EMSENSOR_ENABLE    0xB306D9B1U
        #define SCFG_EMSENSOR_DISABLE   0x9A33A1EfU

    // Boot Seed offset from RAM base, in multiples of 16-byte blocks, up to an offset
    // of 208 bytes (13 16-byte blocks). Min: 0, Max: 13.
    // [220]: length 1B
    uint8_t bootSeedOffset;
        #define SCFG_BOOT_SEED_DISABLED             0xffU
        #define SCFG_BOOT_SEED_OFFSET_16B_BLOCKS(x) ( ((x) >> 4) & 0x0f )

    // Reserved
    // [221]: length 7B
    uint8_t res0[7];

    // CRC accross hsmCfg through res0
    // [228]: length 4B
    uint32_t crc32;


    // Key ring config
    // [232]: length 792B
    struct {
        // Key Entries
        #define SCFG_KEY_HASH_RING_NUM  18U
        // Note: The key ring is not designed to hold many keys at a time. It is  meant to be provisioned with at most
        // one key per type (APP/BLDR) at program load time. If or when it becomes necessary to use a different  key, a
        // key update process can be completed. The key ring can hold up to a total of `SCFG_KEY_HASH_RING_NUM` keys.
        // By the nature of the SCFG's flash there is a requirement that the keyRing be correctly populated. To do so,
        // please ensure the following.
        //  1. Only one key per type is provisioned at compile/load time. These keys must populate the lower keyEntry[]
        //      slots first.
        //  2. All remaining keyEntry[] slots must be set to `SCFG_UNDEFINED_KEY_HASH`
        //      a. !! If not done, future key updates will not be possible !!
        // If you wish to permenantly disallow for future key updates, you can program any/all remaining keyEntry[] with
        // 0x00's instead. Any keyEntry[] slot which has this done will not be usable for future key updates.
        struct keyRingEntry { // length: 792B
            // SHA-256 hash a public key (key type depends on secBootCfg.policyCfg.authAlgorithm)
            uint8_t keyHash[32];
            // ID corresponding to type of keyEntry
            uint32_t type;
                #define SCFG_KEY_ENTRY_TYPE_APP     0x3F68A5A5U
                #define SCFG_KEY_ENTRY_TYPE_BLDR    0x6C715A5AU
            // CRC accross keyHash through type
            uint32_t crc32;
            // Status is intentionally left out of crc32 coverage
            uint32_t status;
                #define SCFG_KEY_ENTRY_STATUS_ACTIVE    0x59183d3bU
        } keyEntries[SCFG_KEY_HASH_RING_NUM];
    } keyRingCfg;


} scfg_t;

typedef struct keyRingEntry keyRingEntry_t;

// Macro for default undefined key hash
#define SCFG_UNDEFINED_KEY_HASH \
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, }

#define SCFG_UNDEFINED_KEY_ENTRY \
    .keyHash = SCFG_UNDEFINED_KEY_HASH , \
    .type = 0xFFFFFFFFU, .crc32 = 0xFFFFFFFFU, .status = 0xFFFFFFFFU

#define SCFG_DUMMY_KEY_ENTRY \
    .keyHash = { 0x0U }, .type = 0x0U, .crc32 = 0x0U, .status = 0x0U

#endif // __SCFG_H__
