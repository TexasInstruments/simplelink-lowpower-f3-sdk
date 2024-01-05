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
    // [0]: length 36B
    struct {
        // 256b hash of the Customer owned public RSA key
        uint8_t publicKeyHash[32];
        uint8_t hsmSize;
            #define SCFG_HSM_SIZE_224   0x07
            #define SCFG_HSM_SIZE_192   0x06
            #define SCFG_HSM_SIZE_160   0x05
            #define SCFG_HSM_SIZE_128   0x04
            #define SCFG_HSM_SIZE_96    0x03
            #define SCFG_HSM_SIZE_64    0x02
            #define SCFG_HSM_SIZE_32    0x01
            #define SCFG_HSM_SIZE_0     0x00
        uint8_t res0[3];
    } hsmCfg;

    // Flash config
    // [36]: length 20B
    struct {
        // Flash Banks
        struct { // length: 2B
            uint16_t mode            : 1;
                #define SCFG_BANK_MODE_MONOLITHIC   0x00
                #define SCFG_BANK_MODE_SPLIT        0x01
            uint16_t isSwapAllowed   : 1;
                #define SCFG_BANK_SWAP_FORBID       0x00
                #define SCFG_BANK_SWAP_ALLOW        0x01
            uint16_t res0            : 14;
        } flashBanks;
        // Flash Layout
        struct { // length: 2B
            // There are always 2 slots. But there can be either ONE or TWO
            // images per slot.
            uint16_t numImagesPerSlot    : 1;
                #define SCFG_ONE_IMG_PER_SLOT   0x00
                #define SCFG_TWO_IMG_PER_SLOT   0x01
            uint16_t res0                : 15;
        } flashLayout;
        // Entry points
        uint32_t entryPoints[4]; // length: 16B
    } flashCfg;

    // Policy config
    // [56]: length 4B
    struct {
        uint32_t authMethod         : 3;
            #define SCFG_POLICY_NO_AUTH                 0x00
            #define SCFG_POLICY_ALWAYS_AUTH             0x01
            #define SCFG_POLICY_SINGLE_AUTH_LOCKED_HASH 0x02
        uint32_t authAlgorithm      : 3;
            #define SCFG_POLICY_ALG_RSA_3K_SHA256       0x00
            #define SCFG_POLICY_ALG_ECDSA_P256_SHA256   0x01
            #define SCFG_POLICY_ALG_ECDSA_P512_SHA512   0x02
        uint32_t res0               : 26;
    } policyCfg;

    // CRC accross hsmCfg through policyCfg
    // [60]: length 4B
    uint32_t crc32;


    // remaining reserved space
    uint8_t reserved[1024 - 64];
} scfg_t;

/*! \brief Extern declaration of the SCFG struct.
 *  This extern declaration can be used to access SCFG fields from other parts
 *  of the code base.
 *  This extern assumes that the scfg_t struct allocated in the system is called
 *  "scfg".
 */
extern const scfg_t scfg;

#endif // __HW_SCFG_H__
