/* testvectors_ecies_data.h
 *
 * Description: Test vectors for ECIES.
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

#include <third_party/hsmddk/include/Kit/DriverFramework/Basic_Defs_API/incl/basic_defs.h>

const uint8_t P256_PrivateKey[] =
{
    0xf4, 0x70, 0x51, 0x4e, 0xfa, 0xc3, 0xcf, 0xda, 0x38, 0xc0, 0x1c,
    0x1f, 0x3b, 0xc8, 0xf1, 0xdd, 0x6d, 0x41, 0x3f, 0xc2, 0xbe, 0xe5,
    0x3b, 0x2e, 0x7f, 0x44, 0x77, 0xdd, 0xaf, 0x8b, 0x7d, 0x6b
};

const uint8_t P256_PublicKey[] =
{
    0x2f, 0x3f, 0xd2, 0xaa, 0x97, 0x9c, 0xc4, 0x3a, 0x21, 0x4e, 0x67,
    0x72, 0xfc, 0x64, 0xc3, 0x3c, 0xc3, 0x4b, 0xbd, 0xce, 0xa6, 0x3f,
    0x1a, 0x61, 0x26, 0x6d, 0x07, 0x12, 0x6e, 0x07, 0x90, 0x0e, 0xf8,
    0x47, 0xb3, 0x27, 0x8a, 0xf0, 0x68, 0xd4, 0xb5, 0x8e, 0x0e, 0xdb,
    0x04, 0xde, 0x71, 0xcf, 0x9e, 0x14, 0xd4, 0x70, 0xfc, 0x6c, 0x3f,
    0x8a, 0xf3, 0x96, 0x4e, 0x5a, 0x39, 0xfe, 0xd4, 0x59
};

const uint8_t P384_PrivateKey[] =
{
    0xa0, 0x8d, 0x45, 0x4a, 0x7b, 0xe7, 0xc0, 0xb8, 0x16, 0x89, 0x20,
    0x26, 0x6f, 0x0c, 0xec, 0x85, 0xcf, 0x15, 0xde, 0x42, 0x09, 0xf6,
    0xf5, 0x6c, 0x13, 0xd0, 0xbe, 0x1a, 0x59, 0x1e, 0x4d, 0xdb, 0x95,
    0x44, 0x8f, 0xc6, 0xee, 0xb6, 0x87, 0xaf, 0xa0, 0x06, 0x7b, 0x45,
    0x16, 0x52, 0x23, 0xf3
};

const uint8_t P384_PublicKey[] =
{
    0x39, 0x32, 0x62, 0x83, 0x45, 0x6f, 0x5b, 0x9f, 0xf8, 0x65, 0xf0,
    0x9b, 0xd3, 0x03, 0xe2, 0xc6, 0x83, 0x5d, 0x32, 0x80, 0x8a, 0x65,
    0x14, 0x2d, 0x05, 0x63, 0x34, 0x15, 0xcc, 0xd5, 0x3e, 0xc6, 0xa4,
    0x49, 0x56, 0x60, 0x2c, 0xd4, 0x7a, 0x23, 0xb0, 0x81, 0x54, 0xfe,
    0x0b, 0x25, 0x8d, 0x1a, 0xfb, 0xe1, 0x99, 0x4c, 0x00, 0x37, 0xeb,
    0x58, 0x15, 0x37, 0xf1, 0x50, 0x72, 0xfc, 0x58, 0x5c, 0x1b, 0x44,
    0x71, 0x49, 0x60, 0x44, 0x53, 0x5b, 0xad, 0xa6, 0x60, 0x28, 0x42,
    0xa2, 0x0d, 0x21, 0x52, 0x4b, 0x32, 0xd6, 0x89, 0x9f, 0x32, 0xe2,
    0xcd, 0x4a, 0x23, 0x14, 0x04, 0xc2, 0xb4, 0x0a
};

const uint8_t P521_PrivateKey[] =
{
     0x01, 0x97, 0x2c, 0x12, 0x54, 0x59, 0x26, 0x79, 0x5b, 0xa4, 0x20,
     0x42, 0x6e, 0xf2, 0x87, 0x51, 0x65, 0x68, 0x0b, 0xca, 0x10, 0x00,
     0x08, 0x30, 0xa1, 0x62, 0x1a, 0x65, 0x90, 0xc9, 0x64, 0x33, 0x8e,
     0xcd, 0x86, 0xaa, 0xc7, 0xf5, 0x03, 0x1c, 0xd9, 0x0e, 0x0e, 0x0d,
     0x93, 0x62, 0x2e, 0xc5, 0x87, 0xd4, 0x42, 0xa0, 0x59, 0x05, 0x9b,
     0xa4, 0x2a, 0x8a, 0x2f, 0x9a, 0x14, 0xb1, 0x12, 0x33, 0x3c, 0xed
};

const uint8_t P521_PublicKey[] =
{
    0x00, 0x55, 0x8c, 0x75, 0xbb, 0xd3, 0xb6, 0xee, 0x7c, 0x68, 0x22,
    0x1c, 0xda, 0xbe, 0x13, 0x8c, 0x34, 0xf0, 0x8e, 0xab, 0x9c, 0x7b,
    0xe1, 0x60, 0xce, 0xe3, 0x40, 0x95, 0x56, 0x1e, 0x52, 0x77, 0x5b,
    0xad, 0x76, 0xcd, 0xf1, 0x13, 0x23, 0x46, 0x35, 0x99, 0x12, 0x02,
    0x1d, 0x7f, 0x6f, 0x08, 0x06, 0xda, 0x3a, 0xbd, 0x4a, 0x1c, 0xb0,
    0xac, 0x83, 0x7f, 0x0e, 0xc2, 0x35, 0x48, 0x9f, 0x51, 0x26, 0x59,
    0x00, 0xba, 0x1b, 0xbe, 0x37, 0x19, 0x2f, 0x7c, 0xce, 0x59, 0x05,
    0x88, 0xb6, 0x12, 0x9d, 0x5a, 0x86, 0x79, 0x95, 0x14, 0x16, 0x21,
    0xad, 0xd2, 0xd8, 0xd7, 0xdd, 0x1a, 0xe7, 0x84, 0x42, 0x3c, 0xaf,
    0x2e, 0x6f, 0x94, 0xdf, 0xf7, 0xa6, 0x17, 0x7c, 0x3f, 0x39, 0xcc,
    0xa3, 0x1f, 0x27, 0x05, 0xae, 0x95, 0xec, 0x74, 0x62, 0xe4, 0xfe,
    0x45, 0xc9, 0x9e, 0x4d, 0x62, 0x7d, 0xc6, 0x25, 0x54, 0x24, 0xc0
};

static const TestVector_ECIES_Rec_t test_vectors_ecies[] =
{
    {
        &ECurve_NIST_P256,
        P256_PrivateKey, sizeof(P256_PrivateKey),
        P256_PublicKey,  sizeof(P256_PublicKey),
        (256/8)
    },
    {
        &ECurve_NIST_P384,
        P384_PrivateKey, sizeof(P384_PrivateKey),
        P384_PublicKey,  sizeof(P384_PublicKey),
        (384/8)
    },
    {
        &ECurve_NIST_P521,
        P521_PrivateKey, sizeof(P521_PrivateKey),
        P521_PublicKey,  sizeof(P521_PublicKey),
        (512/8)
    },
};

/* end of file testvectors_ecies_data.h */