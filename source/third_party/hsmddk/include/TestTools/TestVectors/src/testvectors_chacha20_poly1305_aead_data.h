/* testvectors_chacha20_poly1305_aead_data.h
 *
 * Description: ChaCha20-Poly1305-AEAD test vector data take from
 *              RFC7539    - "ChaCha20 and Poly1305 for IETF protocols"
 *                           Y. Nir, A. Langley, Google Inc.,
 *                           May, 2015
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

#ifndef INCLUDE_GUARD_TESTVECTORS_CHACHA20_POLY1305_AEAD_DATA_H
#define INCLUDE_GUARD_TESTVECTORS_CHACHA20_POLY1305_AEAD_DATA_H

#include <third_party/hsmddk/include/Kit/DriverFramework/Basic_Defs_API/incl/basic_defs.h>

// "RFC7539 (2.8.2) - Example and Test Vector for the AEAD_CHACHA20_POLY1305",
static const uint8_t ChaCha20_Poly1305_TV1_Key[] =
{
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F
};
static const uint8_t ChaCha20_Poly1305_TV1_AAD[] =
{
    0x50, 0x51, 0x52, 0x53, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7
};
static const uint8_t ChaCha20_Poly1305_TV1_Nonce[] =
{
    0x07, 0x00, 0x00, 0x00, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47
};
static const uint8_t ChaCha20_Poly1305_TV1_PlainData[] =
{
    0x4C, 0x61, 0x64, 0x69, 0x65, 0x73, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x47, 0x65, 0x6E, 0x74, 0x6C,
    0x65, 0x6D, 0x65, 0x6E, 0x20, 0x6F, 0x66, 0x20, 0x74, 0x68, 0x65, 0x20, 0x63, 0x6C, 0x61, 0x73,
    0x73, 0x20, 0x6F, 0x66, 0x20, 0x27, 0x39, 0x39, 0x3A, 0x20, 0x49, 0x66, 0x20, 0x49, 0x20, 0x63,
    0x6F, 0x75, 0x6C, 0x64, 0x20, 0x6F, 0x66, 0x66, 0x65, 0x72, 0x20, 0x79, 0x6F, 0x75, 0x20, 0x6F,
    0x6E, 0x6C, 0x79, 0x20, 0x6F, 0x6E, 0x65, 0x20, 0x74, 0x69, 0x70, 0x20, 0x66, 0x6F, 0x72, 0x20,
    0x74, 0x68, 0x65, 0x20, 0x66, 0x75, 0x74, 0x75, 0x72, 0x65, 0x2C, 0x20, 0x73, 0x75, 0x6E, 0x73,
    0x63, 0x72, 0x65, 0x65, 0x6E, 0x20, 0x77, 0x6F, 0x75, 0x6C, 0x64, 0x20, 0x62, 0x65, 0x20, 0x69,
    0x74, 0x2E
};
static const uint8_t ChaCha20_Poly1305_TV1_CipherData[] =
{
    0xD3, 0x1A, 0x8D, 0x34, 0x64, 0x8E, 0x60, 0xDB, 0x7B, 0x86, 0xAF, 0xBC, 0x53, 0xEF, 0x7E, 0xC2,
    0xA4, 0xAD, 0xED, 0x51, 0x29, 0x6E, 0x08, 0xFE, 0xA9, 0xE2, 0xB5, 0xA7, 0x36, 0xEE, 0x62, 0xD6,
    0x3D, 0xBE, 0xA4, 0x5E, 0x8C, 0xA9, 0x67, 0x12, 0x82, 0xFA, 0xFB, 0x69, 0xDA, 0x92, 0x72, 0x8B,
    0x1A, 0x71, 0xDE, 0x0A, 0x9E, 0x06, 0x0B, 0x29, 0x05, 0xD6, 0xA5, 0xB6, 0x7E, 0xCD, 0x3B, 0x36,
    0x92, 0xDD, 0xBD, 0x7F, 0x2D, 0x77, 0x8B, 0x8C, 0x98, 0x03, 0xAE, 0xE3, 0x28, 0x09, 0x1B, 0x58,
    0xFA, 0xB3, 0x24, 0xE4, 0xFA, 0xD6, 0x75, 0x94, 0x55, 0x85, 0x80, 0x8B, 0x48, 0x31, 0xD7, 0xBC,
    0x3F, 0xF4, 0xDE, 0xF0, 0x8E, 0x4B, 0x7A, 0x9D, 0xE5, 0x76, 0xD2, 0x65, 0x86, 0xCE, 0xC6, 0x4B,
    0x61, 0x16
};
static const uint8_t ChaCha20_Poly1305_TV1_Tag[] =
{
    0x1A, 0xE1, 0x0B, 0x59, 0x4F, 0x09, 0xE2, 0x6A, 0x7E, 0x90, 0x2E, 0xCB, 0xD0, 0x60, 0x06, 0x91
};

// RFC7539 (A.5)
static const uint8_t ChaCha20_Poly1305_TV2_Key[] =
{
    0x1C, 0x92, 0x40, 0xA5, 0xEB, 0x55, 0xD3, 0x8A, 0xF3, 0x33, 0x88, 0x86, 0x04, 0xF6, 0xB5, 0xF0,
    0x47, 0x39, 0x17, 0xC1, 0x40, 0x2B, 0x80, 0x09, 0x9D, 0xCA, 0x5C, 0xBC, 0x20, 0x70, 0x75, 0xC0
};
static const uint8_t ChaCha20_Poly1305_TV2_AAD[] =
{
    0xF3, 0x33, 0x88, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4E, 0x91
};
static const uint8_t ChaCha20_Poly1305_TV2_Nonce[] =
{
    0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
};
static const uint8_t ChaCha20_Poly1305_TV2_PlainData[] =
{
    0x49, 0x6E, 0x74, 0x65, 0x72, 0x6E, 0x65, 0x74, 0x2D, 0x44, 0x72, 0x61, 0x66, 0x74, 0x73, 0x20,
    0x61, 0x72, 0x65, 0x20, 0x64, 0x72, 0x61, 0x66, 0x74, 0x20, 0x64, 0x6F, 0x63, 0x75, 0x6D, 0x65,
    0x6E, 0x74, 0x73, 0x20, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x20, 0x66, 0x6F, 0x72, 0x20, 0x61, 0x20,
    0x6D, 0x61, 0x78, 0x69, 0x6D, 0x75, 0x6D, 0x20, 0x6F, 0x66, 0x20, 0x73, 0x69, 0x78, 0x20, 0x6D,
    0x6F, 0x6E, 0x74, 0x68, 0x73, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x6D, 0x61, 0x79, 0x20, 0x62, 0x65,
    0x20, 0x75, 0x70, 0x64, 0x61, 0x74, 0x65, 0x64, 0x2C, 0x20, 0x72, 0x65, 0x70, 0x6C, 0x61, 0x63,
    0x65, 0x64, 0x2C, 0x20, 0x6F, 0x72, 0x20, 0x6F, 0x62, 0x73, 0x6F, 0x6C, 0x65, 0x74, 0x65, 0x64,
    0x20, 0x62, 0x79, 0x20, 0x6F, 0x74, 0x68, 0x65, 0x72, 0x20, 0x64, 0x6F, 0x63, 0x75, 0x6D, 0x65,
    0x6E, 0x74, 0x73, 0x20, 0x61, 0x74, 0x20, 0x61, 0x6E, 0x79, 0x20, 0x74, 0x69, 0x6D, 0x65, 0x2E,
    0x20, 0x49, 0x74, 0x20, 0x69, 0x73, 0x20, 0x69, 0x6E, 0x61, 0x70, 0x70, 0x72, 0x6F, 0x70, 0x72,
    0x69, 0x61, 0x74, 0x65, 0x20, 0x74, 0x6F, 0x20, 0x75, 0x73, 0x65, 0x20, 0x49, 0x6E, 0x74, 0x65,
    0x72, 0x6E, 0x65, 0x74, 0x2D, 0x44, 0x72, 0x61, 0x66, 0x74, 0x73, 0x20, 0x61, 0x73, 0x20, 0x72,
    0x65, 0x66, 0x65, 0x72, 0x65, 0x6E, 0x63, 0x65, 0x20, 0x6D, 0x61, 0x74, 0x65, 0x72, 0x69, 0x61,
    0x6C, 0x20, 0x6F, 0x72, 0x20, 0x74, 0x6F, 0x20, 0x63, 0x69, 0x74, 0x65, 0x20, 0x74, 0x68, 0x65,
    0x6D, 0x20, 0x6F, 0x74, 0x68, 0x65, 0x72, 0x20, 0x74, 0x68, 0x61, 0x6E, 0x20, 0x61, 0x73, 0x20,
    0x2F, 0xE2, 0x80, 0x9C, 0x77, 0x6F, 0x72, 0x6B, 0x20, 0x69, 0x6E, 0x20, 0x70, 0x72, 0x6F, 0x67,
    0x72, 0x65, 0x73, 0x73, 0x2E, 0x2F, 0xE2, 0x80, 0x9D
};
static const uint8_t ChaCha20_Poly1305_TV2_CipherData[] =
{
    0x64, 0xA0, 0x86, 0x15, 0x75, 0x86, 0x1A, 0xF4, 0x60, 0xF0, 0x62, 0xC7, 0x9B, 0xE6, 0x43, 0xBD,
    0x5E, 0x80, 0x5C, 0xFD, 0x34, 0x5C, 0xF3, 0x89, 0xF1, 0x08, 0x67, 0x0A, 0xC7, 0x6C, 0x8C, 0xB2,
    0x4C, 0x6C, 0xFC, 0x18, 0x75, 0x5D, 0x43, 0xEE, 0xA0, 0x9E, 0xE9, 0x4E, 0x38, 0x2D, 0x26, 0xB0,
    0xBD, 0xB7, 0xB7, 0x3C, 0x32, 0x1B, 0x01, 0x00, 0xD4, 0xF0, 0x3B, 0x7F, 0x35, 0x58, 0x94, 0xCF,
    0x33, 0x2F, 0x83, 0x0E, 0x71, 0x0B, 0x97, 0xCE, 0x98, 0xC8, 0xA8, 0x4A, 0xBD, 0x0B, 0x94, 0x81,
    0x14, 0xAD, 0x17, 0x6E, 0x00, 0x8D, 0x33, 0xBD, 0x60, 0xF9, 0x82, 0xB1, 0xFF, 0x37, 0xC8, 0x55,
    0x97, 0x97, 0xA0, 0x6E, 0xF4, 0xF0, 0xEF, 0x61, 0xC1, 0x86, 0x32, 0x4E, 0x2B, 0x35, 0x06, 0x38,
    0x36, 0x06, 0x90, 0x7B, 0x6A, 0x7C, 0x02, 0xB0, 0xF9, 0xF6, 0x15, 0x7B, 0x53, 0xC8, 0x67, 0xE4,
    0xB9, 0x16, 0x6C, 0x76, 0x7B, 0x80, 0x4D, 0x46, 0xA5, 0x9B, 0x52, 0x16, 0xCD, 0xE7, 0xA4, 0xE9,
    0x90, 0x40, 0xC5, 0xA4, 0x04, 0x33, 0x22, 0x5E, 0xE2, 0x82, 0xA1, 0xB0, 0xA0, 0x6C, 0x52, 0x3E,
    0xAF, 0x45, 0x34, 0xD7, 0xF8, 0x3F, 0xA1, 0x15, 0x5B, 0x00, 0x47, 0x71, 0x8C, 0xBC, 0x54, 0x6A,
    0x0D, 0x07, 0x2B, 0x04, 0xB3, 0x56, 0x4E, 0xEA, 0x1B, 0x42, 0x22, 0x73, 0xF5, 0x48, 0x27, 0x1A,
    0x0B, 0xB2, 0x31, 0x60, 0x53, 0xFA, 0x76, 0x99, 0x19, 0x55, 0xEB, 0xD6, 0x31, 0x59, 0x43, 0x4E,
    0xCE, 0xBB, 0x4E, 0x46, 0x6D, 0xAE, 0x5A, 0x10, 0x73, 0xA6, 0x72, 0x76, 0x27, 0x09, 0x7A, 0x10,
    0x49, 0xE6, 0x17, 0xD9, 0x1D, 0x36, 0x10, 0x94, 0xFA, 0x68, 0xF0, 0xFF, 0x77, 0x98, 0x71, 0x30,
    0x30, 0x5B, 0xEA, 0xBA, 0x2E, 0xDA, 0x04, 0xDF, 0x99, 0x7B, 0x71, 0x4D, 0x6C, 0x6F, 0x2C, 0x29,
    0xA6, 0xAD, 0x5C, 0xB4, 0x02, 0x2B, 0x02, 0x70, 0x9B
};
static const uint8_t ChaCha20_Poly1305_TV2_Tag[] =
{
    0xEE, 0xAD, 0x9D, 0x67, 0x89, 0x0C, 0xBB, 0x22, 0x39, 0x23, 0x36, 0xFE, 0xA1, 0x85, 0x1F, 0x38
};


#endif /* INCLUDE_GUARD_TESTVECTORS_CHACHA20_POLY1305_AEAD_DATA_H */

/* end of file testvectors_chacha20_poly1305_aead_data.h */