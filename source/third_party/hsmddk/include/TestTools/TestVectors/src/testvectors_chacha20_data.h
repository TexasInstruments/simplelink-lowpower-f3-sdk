/* testvectors_chacha20_data.h
 *
 * Description: ChaCha20 test vector data take from
 *              RFC7539    - "ChaCha20 and Poly1305 for IETF protocols"
 *                           Y. Nir, A. Langley, Google Inc.,
 *                           May, 2015
 *              IETF Draft - "Test vectors for the Stream Cipher ChaCha"
 *                           J. Strombergson, Secworks Sweden AB,
 *                           December 31, 2013
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

#ifndef INCLUDE_GUARD_TESTVECTORS_CHACHA20_DATA_H
#define INCLUDE_GUARD_TESTVECTORS_CHACHA20_DATA_H

#include <third_party/hsmddk/include/Kit/DriverFramework/Basic_Defs_API/incl/basic_defs.h>

/* RFC7539 */
static const uint8_t ChaCha20_KAT1_Key[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};
static const uint8_t ChaCha20_KAT1_Nonce[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4A, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_KAT1_Counter[] = {
    0x01, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_KAT1_PlainData[] = {
    0x4C, 0x61, 0x64, 0x69, 0x65, 0x73, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x47, 0x65, 0x6E, 0x74, 0x6C,
    0x65, 0x6D, 0x65, 0x6E, 0x20, 0x6F, 0x66, 0x20, 0x74, 0x68, 0x65, 0x20, 0x63, 0x6C, 0x61, 0x73,
    0x73, 0x20, 0x6F, 0x66, 0x20, 0x27, 0x39, 0x39, 0x3A, 0x20, 0x49, 0x66, 0x20, 0x49, 0x20, 0x63,
    0x6F, 0x75, 0x6C, 0x64, 0x20, 0x6F, 0x66, 0x66, 0x65, 0x72, 0x20, 0x79, 0x6F, 0x75, 0x20, 0x6F
};
static const uint8_t ChaCha20_KAT1_CipherData[] = {
    0x6E, 0x2E, 0x35, 0x9A, 0x25, 0x68, 0xF9, 0x80, 0x41, 0xBA, 0x07, 0x28, 0xDD, 0x0D, 0x69, 0x81,
    0xE9, 0x7E, 0x7A, 0xEC, 0x1D, 0x43, 0x60, 0xC2, 0x0A, 0x27, 0xAF, 0xCC, 0xFD, 0x9F, 0xAE, 0x0B,
    0xF9, 0x1B, 0x65, 0xC5, 0x52, 0x47, 0x33, 0xAB, 0x8F, 0x59, 0x3D, 0xAB, 0xCD, 0x62, 0xB3, 0x57,
    0x16, 0x39, 0xD6, 0x24, 0xE6, 0x51, 0x52, 0xAB, 0x8F, 0x53, 0x0C, 0x35, 0x9F, 0x08, 0x61, 0xD8
};

/* RFC7539 */
static const uint8_t ChaCha20_KAT2_Key[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};
static const uint8_t ChaCha20_KAT2_Nonce[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4A, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_KAT2_Counter[] = {
    0x01, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_KAT2_PlainData[] = {
    0x4C, 0x61, 0x64, 0x69, 0x65, 0x73, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x47, 0x65, 0x6E, 0x74, 0x6C,
    0x65, 0x6D, 0x65, 0x6E, 0x20, 0x6F, 0x66, 0x20, 0x74, 0x68, 0x65, 0x20, 0x63, 0x6C, 0x61, 0x73,
    0x73, 0x20, 0x6F, 0x66, 0x20, 0x27, 0x39, 0x39, 0x3A, 0x20, 0x49, 0x66, 0x20, 0x49, 0x20, 0x63,
    0x6F, 0x75, 0x6C, 0x64, 0x20, 0x6F, 0x66, 0x66, 0x65, 0x72, 0x20, 0x79, 0x6F, 0x75, 0x20, 0x6F,
    0x6E, 0x6C, 0x79, 0x20, 0x6F, 0x6E, 0x65, 0x20, 0x74, 0x69, 0x70, 0x20, 0x66, 0x6F, 0x72, 0x20,
    0x74, 0x68, 0x65, 0x20, 0x66, 0x75, 0x74, 0x75, 0x72, 0x65, 0x2C, 0x20, 0x73, 0x75, 0x6E, 0x73,
    0x63, 0x72, 0x65, 0x65, 0x6E, 0x20, 0x77, 0x6F, 0x75, 0x6C, 0x64, 0x20, 0x62, 0x65, 0x20, 0x69,
    0x74, 0x2E
};
static const uint8_t ChaCha20_KAT2_CipherData[] = {
    0x6E, 0x2E, 0x35, 0x9A, 0x25, 0x68, 0xF9, 0x80, 0x41, 0xBA, 0x07, 0x28, 0xDD, 0x0D, 0x69, 0x81,
    0xE9, 0x7E, 0x7A, 0xEC, 0x1D, 0x43, 0x60, 0xC2, 0x0A, 0x27, 0xAF, 0xCC, 0xFD, 0x9F, 0xAE, 0x0B,
    0xF9, 0x1B, 0x65, 0xC5, 0x52, 0x47, 0x33, 0xAB, 0x8F, 0x59, 0x3D, 0xAB, 0xCD, 0x62, 0xB3, 0x57,
    0x16, 0x39, 0xD6, 0x24, 0xE6, 0x51, 0x52, 0xAB, 0x8F, 0x53, 0x0C, 0x35, 0x9F, 0x08, 0x61, 0xD8,
    0x07, 0xCA, 0x0D, 0xBF, 0x50, 0x0D, 0x6A, 0x61, 0x56, 0xA3, 0x8E, 0x08, 0x8A, 0x22, 0xB6, 0x5E,
    0x52, 0xBC, 0x51, 0x4D, 0x16, 0xCC, 0xF8, 0x06, 0x81, 0x8C, 0xE9, 0x1A, 0xB7, 0x79, 0x37, 0x36,
    0x5A, 0xF9, 0x0B, 0xBF, 0x74, 0xA3, 0x5B, 0xE6, 0xB4, 0x0B, 0x8E, 0xED, 0xF2, 0x78, 0x5E, 0x42,
    0x87, 0x4D
};

/* RFC7539 */
static const uint8_t ChaCha20_KAT3_Key[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};
static const uint8_t ChaCha20_KAT3_Nonce[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4A, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_KAT3_Counter[] = {
    0x01, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_KAT3_PlainData[] = {
    0x6E, 0x2E, 0x35, 0x9A, 0x25, 0x68, 0xF9, 0x80, 0x41, 0xBA, 0x07, 0x28, 0xDD, 0x0D, 0x69, 0x81,
    0xE9, 0x7E, 0x7A, 0xEC, 0x1D, 0x43, 0x60, 0xC2, 0x0A, 0x27, 0xAF, 0xCC, 0xFD, 0x9F, 0xAE, 0x0B,
    0xF9, 0x1B, 0x65, 0xC5, 0x52, 0x47, 0x33, 0xAB, 0x8F, 0x59, 0x3D, 0xAB, 0xCD, 0x62, 0xB3, 0x57,
    0x16, 0x39, 0xD6, 0x24, 0xE6, 0x51, 0x52, 0xAB, 0x8F, 0x53, 0x0C, 0x35, 0x9F, 0x08, 0x61, 0xD8,
    0x07, 0xCA, 0x0D, 0xBF, 0x50, 0x0D, 0x6A, 0x61, 0x56, 0xA3, 0x8E, 0x08, 0x8A, 0x22, 0xB6, 0x5E,
    0x52, 0xBC, 0x51, 0x4D, 0x16, 0xCC, 0xF8, 0x06, 0x81, 0x8C, 0xE9, 0x1A, 0xB7, 0x79, 0x37, 0x36,
    0x5A, 0xF9, 0x0B, 0xBF, 0x74, 0xA3, 0x5B, 0xE6, 0xB4, 0x0B, 0x8E, 0xED, 0xF2, 0x78, 0x5E, 0x42,
    0x87, 0x4D
};
static const uint8_t ChaCha20_KAT3_CipherData[] = {
    0x4C, 0x61, 0x64, 0x69, 0x65, 0x73, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x47, 0x65, 0x6E, 0x74, 0x6C,
    0x65, 0x6D, 0x65, 0x6E, 0x20, 0x6F, 0x66, 0x20, 0x74, 0x68, 0x65, 0x20, 0x63, 0x6C, 0x61, 0x73,
    0x73, 0x20, 0x6F, 0x66, 0x20, 0x27, 0x39, 0x39, 0x3A, 0x20, 0x49, 0x66, 0x20, 0x49, 0x20, 0x63,
    0x6F, 0x75, 0x6C, 0x64, 0x20, 0x6F, 0x66, 0x66, 0x65, 0x72, 0x20, 0x79, 0x6F, 0x75, 0x20, 0x6F,
    0x6E, 0x6C, 0x79, 0x20, 0x6F, 0x6E, 0x65, 0x20, 0x74, 0x69, 0x70, 0x20, 0x66, 0x6F, 0x72, 0x20,
    0x74, 0x68, 0x65, 0x20, 0x66, 0x75, 0x74, 0x75, 0x72, 0x65, 0x2C, 0x20, 0x73, 0x75, 0x6E, 0x73,
    0x63, 0x72, 0x65, 0x65, 0x6E, 0x20, 0x77, 0x6F, 0x75, 0x6C, 0x64, 0x20, 0x62, 0x65, 0x20, 0x69,
    0x74, 0x2E
};

/* RFC7539 */
static const uint8_t ChaCha20_KAT4_Key[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_KAT4_Nonce[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_KAT4_Counter[] = {
    0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_KAT4_PlainData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_KAT4_CipherData[] = {
    0x76, 0xB8, 0xE0, 0xAD, 0xA0, 0xF1, 0x3D, 0x90, 0x40, 0x5D, 0x6A, 0xE5, 0x53, 0x86, 0xBD, 0x28,
    0xBD, 0xD2, 0x19, 0xB8, 0xA0, 0x8D, 0xED, 0x1A, 0xA8, 0x36, 0xEF, 0xCC, 0x8B, 0x77, 0x0D, 0xC7,
    0xDA, 0x41, 0x59, 0x7C, 0x51, 0x57, 0x48, 0x8D, 0x77, 0x24, 0xE0, 0x3F, 0xB8, 0xD8, 0x4A, 0x37,
    0x6A, 0x43, 0xB8, 0xF4, 0x15, 0x18, 0xA1, 0x1C, 0xC3, 0x87, 0xB6, 0x69, 0xB2, 0xEE, 0x65, 0x86
};

/* RFC7539 */
static const uint8_t ChaCha20_KAT5_Key[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
};
static const uint8_t ChaCha20_KAT5_Nonce[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
};
static const uint8_t ChaCha20_KAT5_Counter[] = {
    0x01, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_KAT5_PlainData[] = {
    0x41, 0x6E, 0x79, 0x20, 0x73, 0x75, 0x62, 0x6D, 0x69, 0x73, 0x73, 0x69, 0x6F, 0x6E, 0x20, 0x74,
    0x6F, 0x20, 0x74, 0x68, 0x65, 0x20, 0x49, 0x45, 0x54, 0x46, 0x20, 0x69, 0x6E, 0x74, 0x65, 0x6E,
    0x64, 0x65, 0x64, 0x20, 0x62, 0x79, 0x20, 0x74, 0x68, 0x65, 0x20, 0x43, 0x6F, 0x6E, 0x74, 0x72,
    0x69, 0x62, 0x75, 0x74, 0x6F, 0x72, 0x20, 0x66, 0x6F, 0x72, 0x20, 0x70, 0x75, 0x62, 0x6C, 0x69,
    0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x61, 0x73, 0x20, 0x61, 0x6C, 0x6C, 0x20, 0x6F, 0x72,
    0x20, 0x70, 0x61, 0x72, 0x74, 0x20, 0x6F, 0x66, 0x20, 0x61, 0x6E, 0x20, 0x49, 0x45, 0x54, 0x46,
    0x20, 0x49, 0x6E, 0x74, 0x65, 0x72, 0x6E, 0x65, 0x74, 0x2D, 0x44, 0x72, 0x61, 0x66, 0x74, 0x20,
    0x6F, 0x72, 0x20, 0x52, 0x46, 0x43, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x61, 0x6E, 0x79, 0x20, 0x73,
    0x74, 0x61, 0x74, 0x65, 0x6D, 0x65, 0x6E, 0x74, 0x20, 0x6D, 0x61, 0x64, 0x65, 0x20, 0x77, 0x69,
    0x74, 0x68, 0x69, 0x6E, 0x20, 0x74, 0x68, 0x65, 0x20, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x78, 0x74,
    0x20, 0x6F, 0x66, 0x20, 0x61, 0x6E, 0x20, 0x49, 0x45, 0x54, 0x46, 0x20, 0x61, 0x63, 0x74, 0x69,
    0x76, 0x69, 0x74, 0x79, 0x20, 0x69, 0x73, 0x20, 0x63, 0x6F, 0x6E, 0x73, 0x69, 0x64, 0x65, 0x72,
    0x65, 0x64, 0x20, 0x61, 0x6E, 0x20, 0x22, 0x49, 0x45, 0x54, 0x46, 0x20, 0x43, 0x6F, 0x6E, 0x74,
    0x72, 0x69, 0x62, 0x75, 0x74, 0x69, 0x6F, 0x6E, 0x22, 0x2E, 0x20, 0x53, 0x75, 0x63, 0x68, 0x20,
    0x73, 0x74, 0x61, 0x74, 0x65, 0x6D, 0x65, 0x6E, 0x74, 0x73, 0x20, 0x69, 0x6E, 0x63, 0x6C, 0x75,
    0x64, 0x65, 0x20, 0x6F, 0x72, 0x61, 0x6C, 0x20, 0x73, 0x74, 0x61, 0x74, 0x65, 0x6D, 0x65, 0x6E,
    0x74, 0x73, 0x20, 0x69, 0x6E, 0x20, 0x49, 0x45, 0x54, 0x46, 0x20, 0x73, 0x65, 0x73, 0x73, 0x69,
    0x6F, 0x6E, 0x73, 0x2C, 0x20, 0x61, 0x73, 0x20, 0x77, 0x65, 0x6C, 0x6C, 0x20, 0x61, 0x73, 0x20,
    0x77, 0x72, 0x69, 0x74, 0x74, 0x65, 0x6E, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x65, 0x6C, 0x65, 0x63,
    0x74, 0x72, 0x6F, 0x6E, 0x69, 0x63, 0x20, 0x63, 0x6F, 0x6D, 0x6D, 0x75, 0x6E, 0x69, 0x63, 0x61,
    0x74, 0x69, 0x6F, 0x6E, 0x73, 0x20, 0x6D, 0x61, 0x64, 0x65, 0x20, 0x61, 0x74, 0x20, 0x61, 0x6E,
    0x79, 0x20, 0x74, 0x69, 0x6D, 0x65, 0x20, 0x6F, 0x72, 0x20, 0x70, 0x6C, 0x61, 0x63, 0x65, 0x2C,
    0x20, 0x77, 0x68, 0x69, 0x63, 0x68, 0x20, 0x61, 0x72, 0x65, 0x20, 0x61, 0x64, 0x64, 0x72, 0x65,
    0x73, 0x73, 0x65, 0x64, 0x20, 0x74, 0x6F
};
static const uint8_t ChaCha20_KAT5_CipherData[] = {
    0xA3, 0xFB, 0xF0, 0x7D, 0xF3, 0xFA, 0x2F, 0xDE, 0x4F, 0x37, 0x6C, 0xA2, 0x3E, 0x82, 0x73, 0x70,
    0x41, 0x60, 0x5D, 0x9F, 0x4F, 0x4F, 0x57, 0xBD, 0x8C, 0xFF, 0x2C, 0x1D, 0x4B, 0x79, 0x55, 0xEC,
    0x2A, 0x97, 0x94, 0x8B, 0xD3, 0x72, 0x29, 0x15, 0xC8, 0xF3, 0xD3, 0x37, 0xF7, 0xD3, 0x70, 0x05,
    0x0E, 0x9E, 0x96, 0xD6, 0x47, 0xB7, 0xC3, 0x9F, 0x56, 0xE0, 0x31, 0xCA, 0x5E, 0xB6, 0x25, 0x0D,
    0x40, 0x42, 0xE0, 0x27, 0x85, 0xEC, 0xEC, 0xFA, 0x4B, 0x4B, 0xB5, 0xE8, 0xEA, 0xD0, 0x44, 0x0E,
    0x20, 0xB6, 0xE8, 0xDB, 0x09, 0xD8, 0x81, 0xA7, 0xC6, 0x13, 0x2F, 0x42, 0x0E, 0x52, 0x79, 0x50,
    0x42, 0xBD, 0xFA, 0x77, 0x73, 0xD8, 0xA9, 0x05, 0x14, 0x47, 0xB3, 0x29, 0x1C, 0xE1, 0x41, 0x1C,
    0x68, 0x04, 0x65, 0x55, 0x2A, 0xA6, 0xC4, 0x05, 0xB7, 0x76, 0x4D, 0x5E, 0x87, 0xBE, 0xA8, 0x5A,
    0xD0, 0x0F, 0x84, 0x49, 0xED, 0x8F, 0x72, 0xD0, 0xD6, 0x62, 0xAB, 0x05, 0x26, 0x91, 0xCA, 0x66,
    0x42, 0x4B, 0xC8, 0x6D, 0x2D, 0xF8, 0x0E, 0xA4, 0x1F, 0x43, 0xAB, 0xF9, 0x37, 0xD3, 0x25, 0x9D,
    0xC4, 0xB2, 0xD0, 0xDF, 0xB4, 0x8A, 0x6C, 0x91, 0x39, 0xDD, 0xD7, 0xF7, 0x69, 0x66, 0xE9, 0x28,
    0xE6, 0x35, 0x55, 0x3B, 0xA7, 0x6C, 0x5C, 0x87, 0x9D, 0x7B, 0x35, 0xD4, 0x9E, 0xB2, 0xE6, 0x2B,
    0x08, 0x71, 0xCD, 0xAC, 0x63, 0x89, 0x39, 0xE2, 0x5E, 0x8A, 0x1E, 0x0E, 0xF9, 0xD5, 0x28, 0x0F,
    0xA8, 0xCA, 0x32, 0x8B, 0x35, 0x1C, 0x3C, 0x76, 0x59, 0x89, 0xCB, 0xCF, 0x3D, 0xAA, 0x8B, 0x6C,
    0xCC, 0x3A, 0xAF, 0x9F, 0x39, 0x79, 0xC9, 0x2B, 0x37, 0x20, 0xFC, 0x88, 0xDC, 0x95, 0xED, 0x84,
    0xA1, 0xBE, 0x05, 0x9C, 0x64, 0x99, 0xB9, 0xFD, 0xA2, 0x36, 0xE7, 0xE8, 0x18, 0xB0, 0x4B, 0x0B,
    0xC3, 0x9C, 0x1E, 0x87, 0x6B, 0x19, 0x3B, 0xFE, 0x55, 0x69, 0x75, 0x3F, 0x88, 0x12, 0x8C, 0xC0,
    0x8A, 0xAA, 0x9B, 0x63, 0xD1, 0xA1, 0x6F, 0x80, 0xEF, 0x25, 0x54, 0xD7, 0x18, 0x9C, 0x41, 0x1F,
    0x58, 0x69, 0xCA, 0x52, 0xC5, 0xB8, 0x3F, 0xA3, 0x6F, 0xF2, 0x16, 0xB9, 0xC1, 0xD3, 0x00, 0x62,
    0xBE, 0xBC, 0xFD, 0x2D, 0xC5, 0xBC, 0xE0, 0x91, 0x19, 0x34, 0xFD, 0xA7, 0x9A, 0x86, 0xF6, 0xE6,
    0x98, 0xCE, 0xD7, 0x59, 0xC3, 0xFF, 0x9B, 0x64, 0x77, 0x33, 0x8F, 0x3D, 0xA4, 0xF9, 0xCD, 0x85,
    0x14, 0xEA, 0x99, 0x82, 0xCC, 0xAF, 0xB3, 0x41, 0xB2, 0x38, 0x4D, 0xD9, 0x02, 0xF3, 0xD1, 0xAB,
    0x7A, 0xC6, 0x1D, 0xD2, 0x9C, 0x6F, 0x21, 0xBA, 0x5B, 0x86, 0x2F, 0x37, 0x30, 0xE3, 0x7C, 0xFD,
    0xC4, 0xFD, 0x80, 0x6C, 0x22, 0xF2, 0x21
};

/* RFC7539 */
static const uint8_t ChaCha20_KAT6_Key[] = {
    0x1C, 0x92, 0x40, 0xA5, 0xEB, 0x55, 0xD3, 0x8A, 0xF3, 0x33, 0x88, 0x86, 0x04, 0xF6, 0xB5, 0xF0,
    0x47, 0x39, 0x17, 0xC1, 0x40, 0x2B, 0x80, 0x09, 0x9D, 0xCA, 0x5C, 0xBC, 0x20, 0x70, 0x75, 0xC0
};
static const uint8_t ChaCha20_KAT6_Nonce[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
};
static const uint8_t ChaCha20_KAT6_Counter[] = {
    0x2A, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_KAT6_PlainData[] = {
    0x27, 0x54, 0x77, 0x61, 0x73, 0x20, 0x62, 0x72, 0x69, 0x6C, 0x6C, 0x69, 0x67, 0x2C, 0x20, 0x61,
    0x6E, 0x64, 0x20, 0x74, 0x68, 0x65, 0x20, 0x73, 0x6C, 0x69, 0x74, 0x68, 0x79, 0x20, 0x74, 0x6F,
    0x76, 0x65, 0x73, 0x0A, 0x44, 0x69, 0x64, 0x20, 0x67, 0x79, 0x72, 0x65, 0x20, 0x61, 0x6E, 0x64,
    0x20, 0x67, 0x69, 0x6D, 0x62, 0x6C, 0x65, 0x20, 0x69, 0x6E, 0x20, 0x74, 0x68, 0x65, 0x20, 0x77,
    0x61, 0x62, 0x65, 0x3A, 0x0A, 0x41, 0x6C, 0x6C, 0x20, 0x6D, 0x69, 0x6D, 0x73, 0x79, 0x20, 0x77,
    0x65, 0x72, 0x65, 0x20, 0x74, 0x68, 0x65, 0x20, 0x62, 0x6F, 0x72, 0x6F, 0x67, 0x6F, 0x76, 0x65,
    0x73, 0x2C, 0x0A, 0x41, 0x6E, 0x64, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6D, 0x6F, 0x6D, 0x65, 0x20,
    0x72, 0x61, 0x74, 0x68, 0x73, 0x20, 0x6F, 0x75, 0x74, 0x67, 0x72, 0x61, 0x62, 0x65, 0x2E
};
static const uint8_t ChaCha20_KAT6_CipherData[] = {
    0x62, 0xE6, 0x34, 0x7F, 0x95, 0xED, 0x87, 0xA4, 0x5F, 0xFA, 0xE7, 0x42, 0x6F, 0x27, 0xA1, 0xDF,
    0x5F, 0xB6, 0x91, 0x10, 0x04, 0x4C, 0x0D, 0x73, 0x11, 0x8E, 0xFF, 0xA9, 0x5B, 0x01, 0xE5, 0xCF,
    0x16, 0x6D, 0x3D, 0xF2, 0xD7, 0x21, 0xCA, 0xF9, 0xB2, 0x1E, 0x5F, 0xB1, 0x4C, 0x61, 0x68, 0x71,
    0xFD, 0x84, 0xC5, 0x4F, 0x9D, 0x65, 0xB2, 0x83, 0x19, 0x6C, 0x7F, 0xE4, 0xF6, 0x05, 0x53, 0xEB,
    0xF3, 0x9C, 0x64, 0x02, 0xC4, 0x22, 0x34, 0xE3, 0x2A, 0x35, 0x6B, 0x3E, 0x76, 0x43, 0x12, 0xA6,
    0x1A, 0x55, 0x32, 0x05, 0x57, 0x16, 0xEA, 0xD6, 0x96, 0x25, 0x68, 0xF8, 0x7D, 0x3F, 0x3F, 0x77,
    0x04, 0xC6, 0xA8, 0xD1, 0xBC, 0xD1, 0xBF, 0x4D, 0x50, 0xD6, 0x15, 0x4B, 0x6D, 0xA7, 0x31, 0xB1,
    0x87, 0xB5, 0x8D, 0xFD, 0x72, 0x8A, 0xFA, 0x36, 0x75, 0x7A, 0x79, 0x7A, 0xC1, 0x88, 0xD1
};

/* IETF Draft */
static const uint8_t ChaCha20_TV1_Key[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_TV1_Nonce[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_TV1_Counter[] = {
    0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_TV1_PlainData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_TV1_CipherData[] = {
    0x89, 0x67, 0x09, 0x52, 0x60, 0x83, 0x64, 0xFD, 0x00, 0xB2, 0xF9, 0x09, 0x36, 0xF0, 0x31, 0xC8,
    0xE7, 0x56, 0xE1, 0x5D, 0xBA, 0x04, 0xB8, 0x49, 0x3D, 0x00, 0x42, 0x92, 0x59, 0xB2, 0x0F, 0x46,
    0xCC, 0x04, 0xF1, 0x11, 0x24, 0x6B, 0x6C, 0x2C, 0xE0, 0x66, 0xBE, 0x3B, 0xFB, 0x32, 0xD9, 0xAA,
    0x0F, 0xDD, 0xFB, 0xC1, 0x21, 0x23, 0xD4, 0xB9, 0xE4, 0x4F, 0x34, 0xDC, 0xA0, 0x5A, 0x10, 0x3F,
    0x6C, 0xD1, 0x35, 0xC2, 0x87, 0x8C, 0x83, 0x2B, 0x58, 0x96, 0xB1, 0x34, 0xF6, 0x14, 0x2A, 0x9D,
    0x4D, 0x8D, 0x0D, 0x8F, 0x10, 0x26, 0xD2, 0x0A, 0x0A, 0x81, 0x51, 0x2C, 0xBC, 0xE6, 0xE9, 0x75,
    0x8A, 0x71, 0x43, 0xD0, 0x21, 0x97, 0x80, 0x22, 0xA3, 0x84, 0x14, 0x1A, 0x80, 0xCE, 0xA3, 0x06,
    0x2F, 0x41, 0xF6, 0x7A, 0x75, 0x2E, 0x66, 0xAD, 0x34, 0x11, 0x98, 0x4C, 0x78, 0x7E, 0x30, 0xAD
};

/* IETF Draft */
static const uint8_t ChaCha20_TV2_Key[] = {
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_TV2_Nonce[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_TV2_Counter[] = {
    0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_TV2_PlainData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_TV2_CipherData[] = {
    0xAE, 0x56, 0x06, 0x0D, 0x04, 0xF5, 0xB5, 0x97, 0x89, 0x7F, 0xF2, 0xAF, 0x13, 0x88, 0xDB, 0xCE,
    0xFF, 0x5A, 0x2A, 0x49, 0x20, 0x33, 0x5D, 0xC1, 0x7A, 0x3C, 0xB1, 0xB1, 0xB1, 0x0F, 0xBE, 0x70,
    0xEC, 0xE8, 0xF4, 0x86, 0x4D, 0x8C, 0x7C, 0xDF, 0x00, 0x76, 0x45, 0x3A, 0x82, 0x91, 0xC7, 0xDB,
    0xEB, 0x3A, 0xA9, 0xC9, 0xD1, 0x0E, 0x8C, 0xA3, 0x6B, 0xE4, 0x44, 0x93, 0x76, 0xED, 0x7C, 0x42,
    0xFC, 0x3D, 0x47, 0x1C, 0x34, 0xA3, 0x6F, 0xBB, 0xF6, 0x16, 0xBC, 0x0A, 0x0E, 0x7C, 0x52, 0x30,
    0x30, 0xD9, 0x44, 0xF4, 0x3E, 0xC3, 0xE7, 0x8D, 0xD6, 0xA1, 0x24, 0x66, 0x54, 0x7C, 0xB4, 0xF7,
    0xB3, 0xCE, 0xBD, 0x0A, 0x50, 0x05, 0xE7, 0x62, 0xE5, 0x62, 0xD1, 0x37, 0x5B, 0x7A, 0xC4, 0x45,
    0x93, 0xA9, 0x91, 0xB8, 0x5D, 0x1A, 0x60, 0xFB, 0xA2, 0x03, 0x5D, 0xFA, 0xA2, 0xA6, 0x42, 0xD5
};

/* IETF Draft */
static const uint8_t ChaCha20_TV3_Key[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
static const uint8_t ChaCha20_TV3_Nonce[] = {
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
static const uint8_t ChaCha20_TV3_Counter[] = {
    0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_TV3_PlainData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t ChaCha20_TV3_CipherData[] = {
    0x99, 0x29, 0x47, 0xC3, 0x96, 0x61, 0x26, 0xA0, 0xE6, 0x60, 0xA3, 0xE9, 0x5D, 0xB0, 0x48, 0xDE,
    0x09, 0x1F, 0xB9, 0xE0, 0x18, 0x5B, 0x1E, 0x41, 0xE4, 0x10, 0x15, 0xBB, 0x7E, 0xE5, 0x01, 0x50,
    0x39, 0x9E, 0x47, 0x60, 0xB2, 0x62, 0xF9, 0xD5, 0x3F, 0x26, 0xD8, 0xDD, 0x19, 0xE5, 0x6F, 0x5C,
    0x50, 0x6A, 0xE0, 0xC3, 0x61, 0x9F, 0xA6, 0x7F, 0xB0, 0xC4, 0x08, 0x10, 0x6D, 0x02, 0x03, 0xEE,
    0x40, 0xEA, 0x3C, 0xFA, 0x61, 0xFA, 0x32, 0xA2, 0xFD, 0xA8, 0xD1, 0x23, 0x8A, 0x21, 0x35, 0xD9,
    0xD4, 0x17, 0x87, 0x75, 0x24, 0x0F, 0x99, 0x00, 0x70, 0x64, 0xA6, 0xA7, 0xF0, 0xC7, 0x31, 0xB6,
    0x7C, 0x22, 0x7C, 0x52, 0xEF, 0x79, 0x6B, 0x6B, 0xED, 0x9F, 0x90, 0x59, 0xBA, 0x06, 0x14, 0xBC,
    0xF6, 0xDD, 0x6E, 0x38, 0x91, 0x7F, 0x3B, 0x15, 0x0E, 0x57, 0x63, 0x75, 0xBE, 0x50, 0xED, 0x67
};


#endif /* INCLUDE_GUARD_TESTVECTORS_CHACHA20_DATA_H */

/* end of file testvectors_chacha20_data.h */