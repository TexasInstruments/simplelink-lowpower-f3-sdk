/* testvectors_eddsa_data.h
 *
 * Description: Test vectors for EdDSA.
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

/* The EdDSA binary material. */

// 64 bytes message
static const uint8_t Vector_EdDSA_64_Message[] =
{
    0x4B, 0xAF, 0xDA, 0xC9, 0x09, 0x9D, 0x40, 0x57, 0xED, 0x6D, 0xD0, 0x8B, 0xCA, 0xEE, 0x87, 0x56,
    0xE9, 0xA4, 0x0F, 0x2C, 0xB9, 0x59, 0x80, 0x20, 0xEB, 0x95, 0x01, 0x95, 0x28, 0x40, 0x9B, 0xBE,
    0xA3, 0x8B, 0x38, 0x4A, 0x59, 0xF1, 0x19, 0xF5, 0x72, 0x97, 0xBF, 0xB2, 0xFA, 0x14, 0x2F, 0xC7,
    0xBB, 0x1D, 0x90, 0xDB, 0xDD, 0xDE, 0x77, 0x2B, 0xCD, 0xE4, 0x8C, 0x56, 0x70, 0xD5, 0xFA, 0x13
};
static const uint8_t Vector_EdDSA_64_PrivateKey[] =
{
    0x20, 0xEE, 0x55, 0x8B, 0x71, 0x62, 0x14, 0x34, 0xF9, 0xC1, 0x5B, 0x72, 0x81, 0x3B, 0x2F, 0x44,
    0x7A, 0xF3, 0x44, 0x40, 0x49, 0x26, 0x43, 0xE4, 0xA1, 0x67, 0xCE, 0xB2, 0x67, 0x6E, 0x4D, 0xBA
};
static const uint8_t Vector_EdDSA_64_PublicKey[] =
{
    0xCC, 0xC6, 0xE9, 0x87, 0x03, 0x01, 0xCA, 0x09, 0x68, 0xD1, 0xAD, 0x29, 0x7F, 0x8E, 0x1E, 0xA4,
    0x51, 0xE3, 0x67, 0xAD, 0xA5, 0xFD, 0xA5, 0x75, 0xB6, 0x6D, 0x4B, 0xF8, 0x76, 0xA0, 0x3F, 0xF7
};
static const uint8_t Vector_EdDSA_64_SignatureR[] =
{
    0x3A, 0x8F, 0x45, 0x7B, 0xE1, 0xFE, 0x06, 0x58, 0x30, 0xF1, 0xA0, 0x70, 0x9A, 0x1A, 0xAB, 0x8E,
    0xA1, 0xEA, 0x48, 0xDD, 0xE7, 0xBA, 0x21, 0x74, 0x83, 0x7F, 0x20, 0x11, 0xA7, 0xD2, 0xB9, 0x57
};
static const uint8_t Vector_EdDSA_64_SignatureS[] =
{
    0x06, 0x6A, 0xEE, 0x0E, 0x84, 0xC7, 0xFB, 0x32, 0xE6, 0x57, 0xA9, 0xF8, 0x41, 0x3B, 0x08, 0xF0,
    0x77, 0x0B, 0x00, 0x6F, 0x49, 0xE8, 0xC9, 0x0A, 0x3E, 0x4D, 0x83, 0xD1, 0x02, 0xB3, 0x64, 0x09
};

// 96 bytes message
static const uint8_t Vector_EdDSA_96_Message[] =
{
    0xFE, 0x6C, 0x1A, 0x31, 0x06, 0x8E, 0x33, 0x2D, 0x12, 0xAA, 0xB3, 0x7D, 0x99, 0x40, 0x65, 0x68,
    0xDE, 0xAA, 0x36, 0xBD, 0xB2, 0x77, 0xCE, 0xE5, 0x53, 0x04, 0x63, 0x3B, 0xD0, 0xA2, 0x67, 0xA8,
    0x50, 0xE2, 0x03, 0xBB, 0x3F, 0xAB, 0xE5, 0x11, 0x0B, 0xCC, 0x1C, 0xA4, 0x31, 0x66, 0x98, 0xAB,
    0x1C, 0xF0, 0x0F, 0x0B, 0x0F, 0x1D, 0x97, 0xEF, 0x21, 0x80, 0x88, 0x7F, 0x0E, 0xC0, 0x99, 0x1E,
    0x8C, 0x11, 0x11, 0xF0, 0xC0, 0xE1, 0xD2, 0xB7, 0x12, 0x43, 0x3A, 0xD2, 0xB3, 0x07, 0x1B, 0xD6,
    0x6E, 0x1D, 0x81, 0xF7, 0xFA, 0x47, 0xBB, 0x4B, 0xB3, 0x1A, 0xC0, 0xF0, 0x59, 0xBB, 0x3C, 0xB8
};
static const uint8_t Vector_EdDSA_96_PrivateKey[] =
{
    0xAC, 0xBD, 0xFE, 0x5E, 0x19, 0xE3, 0x94, 0xA9, 0x75, 0xCD, 0xA2, 0x73, 0x5D, 0x4A, 0xB0, 0xA3,
    0x13, 0x9F, 0x74, 0x74, 0x0A, 0x30, 0x4B, 0xDD, 0x77, 0xC9, 0xFB, 0xD3, 0xA3, 0xE3, 0xDC, 0x3A
};
static const uint8_t Vector_EdDSA_96_PublicKey[] =
{
    0x19, 0xAC, 0xF5, 0xCF, 0x54, 0x3E, 0xE8, 0x27, 0x01, 0x02, 0x5E, 0x38, 0x2E, 0xA5, 0x96, 0xF5,
    0x0B, 0x7F, 0x40, 0xC6, 0x45, 0x48, 0xC7, 0xD5, 0x51, 0x48, 0xD6, 0x18, 0x1F, 0x9B, 0xF1, 0x6F
};
static const uint8_t Vector_EdDSA_96_SignatureR[] =
{
    0xC6, 0x5F, 0xAE, 0xCE, 0x85, 0xA7, 0x65, 0xCF, 0xC7, 0x0E, 0xA3, 0xDD, 0x63, 0x5B, 0x71, 0x4A,
    0x09, 0xF7, 0x53, 0x49, 0xDE, 0xC6, 0xD8, 0x5A, 0x9F, 0x53, 0x40, 0x5B, 0xF8, 0x89, 0xDA, 0x7D
};
static const uint8_t Vector_EdDSA_96_SignatureS[] =
{
    0x06, 0x8A, 0x28, 0x3B, 0x18, 0x8A, 0x89, 0xA0, 0x08, 0x17, 0x68, 0x09, 0x44, 0xC6, 0x83, 0x6D,
    0xEE, 0x39, 0xFC, 0xD8, 0x37, 0xEE, 0xE7, 0xCB, 0xCE, 0x82, 0xE6, 0x0B, 0xE0, 0x7E, 0x70, 0x88
};

// 1023 bytes message
static const uint8_t Vector_EdDSA_1023_Message[] =
{
    0x08, 0xB8, 0xB2, 0xB7, 0x33, 0x42, 0x42, 0x43, 0x76, 0x0F, 0xE4, 0x26, 0xA4, 0xB5, 0x49, 0x08,
    0x63, 0x21, 0x10, 0xA6, 0x6C, 0x2F, 0x65, 0x91, 0xEA, 0xBD, 0x33, 0x45, 0xE3, 0xE4, 0xEB, 0x98,
    0xFA, 0x6E, 0x26, 0x4B, 0xF0, 0x9E, 0xFE, 0x12, 0xEE, 0x50, 0xF8, 0xF5, 0x4E, 0x9F, 0x77, 0xB1,
    0xE3, 0x55, 0xF6, 0xC5, 0x05, 0x44, 0xE2, 0x3F, 0xB1, 0x43, 0x3D, 0xDF, 0x73, 0xBE, 0x84, 0xD8,
    0x79, 0xDE, 0x7C, 0x00, 0x46, 0xDC, 0x49, 0x96, 0xD9, 0xE7, 0x73, 0xF4, 0xBC, 0x9E, 0xFE, 0x57,
    0x38, 0x82, 0x9A, 0xDB, 0x26, 0xC8, 0x1B, 0x37, 0xC9, 0x3A, 0x1B, 0x27, 0x0B, 0x20, 0x32, 0x9D,
    0x65, 0x86, 0x75, 0xFC, 0x6E, 0xA5, 0x34, 0xE0, 0x81, 0x0A, 0x44, 0x32, 0x82, 0x6B, 0xF5, 0x8C,
    0x94, 0x1E, 0xFB, 0x65, 0xD5, 0x7A, 0x33, 0x8B, 0xBD, 0x2E, 0x26, 0x64, 0x0F, 0x89, 0xFF, 0xBC,
    0x1A, 0x85, 0x8E, 0xFC, 0xB8, 0x55, 0x0E, 0xE3, 0xA5, 0xE1, 0x99, 0x8B, 0xD1, 0x77, 0xE9, 0x3A,
    0x73, 0x63, 0xC3, 0x44, 0xFE, 0x6B, 0x19, 0x9E, 0xE5, 0xD0, 0x2E, 0x82, 0xD5, 0x22, 0xC4, 0xFE,
    0xBA, 0x15, 0x45, 0x2F, 0x80, 0x28, 0x8A, 0x82, 0x1A, 0x57, 0x91, 0x16, 0xEC, 0x6D, 0xAD, 0x2B,
    0x3B, 0x31, 0x0D, 0xA9, 0x03, 0x40, 0x1A, 0xA6, 0x21, 0x00, 0xAB, 0x5D, 0x1A, 0x36, 0x55, 0x3E,
    0x06, 0x20, 0x3B, 0x33, 0x89, 0x0C, 0xC9, 0xB8, 0x32, 0xF7, 0x9E, 0xF8, 0x05, 0x60, 0xCC, 0xB9,
    0xA3, 0x9C, 0xE7, 0x67, 0x96, 0x7E, 0xD6, 0x28, 0xC6, 0xAD, 0x57, 0x3C, 0xB1, 0x16, 0xDB, 0xEF,
    0xEF, 0xD7, 0x54, 0x99, 0xDA, 0x96, 0xBD, 0x68, 0xA8, 0xA9, 0x7B, 0x92, 0x8A, 0x8B, 0xBC, 0x10,
    0x3B, 0x66, 0x21, 0xFC, 0xDE, 0x2B, 0xEC, 0xA1, 0x23, 0x1D, 0x20, 0x6B, 0xE6, 0xCD, 0x9E, 0xC7,
    0xAF, 0xF6, 0xF6, 0xC9, 0x4F, 0xCD, 0x72, 0x04, 0xED, 0x34, 0x55, 0xC6, 0x8C, 0x83, 0xF4, 0xA4,
    0x1D, 0xA4, 0xAF, 0x2B, 0x74, 0xEF, 0x5C, 0x53, 0xF1, 0xD8, 0xAC, 0x70, 0xBD, 0xCB, 0x7E, 0xD1,
    0x85, 0xCE, 0x81, 0xBD, 0x84, 0x35, 0x9D, 0x44, 0x25, 0x4D, 0x95, 0x62, 0x9E, 0x98, 0x55, 0xA9,
    0x4A, 0x7C, 0x19, 0x58, 0xD1, 0xF8, 0xAD, 0xA5, 0xD0, 0x53, 0x2E, 0xD8, 0xA5, 0xAA, 0x3F, 0xB2,
    0xD1, 0x7B, 0xA7, 0x0E, 0xB6, 0x24, 0x8E, 0x59, 0x4E, 0x1A, 0x22, 0x97, 0xAC, 0xBB, 0xB3, 0x9D,
    0x50, 0x2F, 0x1A, 0x8C, 0x6E, 0xB6, 0xF1, 0xCE, 0x22, 0xB3, 0xDE, 0x1A, 0x1F, 0x40, 0xCC, 0x24,
    0x55, 0x41, 0x19, 0xA8, 0x31, 0xA9, 0xAA, 0xD6, 0x07, 0x9C, 0xAD, 0x88, 0x42, 0x5D, 0xE6, 0xBD,
    0xE1, 0xA9, 0x18, 0x7E, 0xBB, 0x60, 0x92, 0xCF, 0x67, 0xBF, 0x2B, 0x13, 0xFD, 0x65, 0xF2, 0x70,
    0x88, 0xD7, 0x8B, 0x7E, 0x88, 0x3C, 0x87, 0x59, 0xD2, 0xC4, 0xF5, 0xC6, 0x5A, 0xDB, 0x75, 0x53,
    0x87, 0x8A, 0xD5, 0x75, 0xF9, 0xFA, 0xD8, 0x78, 0xE8, 0x0A, 0x0C, 0x9B, 0xA6, 0x3B, 0xCB, 0xCC,
    0x27, 0x32, 0xE6, 0x94, 0x85, 0xBB, 0xC9, 0xC9, 0x0B, 0xFB, 0xD6, 0x24, 0x81, 0xD9, 0x08, 0x9B,
    0xEC, 0xCF, 0x80, 0xCF, 0xE2, 0xDF, 0x16, 0xA2, 0xCF, 0x65, 0xBD, 0x92, 0xDD, 0x59, 0x7B, 0x07,
    0x07, 0xE0, 0x91, 0x7A, 0xF4, 0x8B, 0xBB, 0x75, 0xFE, 0xD4, 0x13, 0xD2, 0x38, 0xF5, 0x55, 0x5A,
    0x7A, 0x56, 0x9D, 0x80, 0xC3, 0x41, 0x4A, 0x8D, 0x08, 0x59, 0xDC, 0x65, 0xA4, 0x61, 0x28, 0xBA,
    0xB2, 0x7A, 0xF8, 0x7A, 0x71, 0x31, 0x4F, 0x31, 0x8C, 0x78, 0x2B, 0x23, 0xEB, 0xFE, 0x80, 0x8B,
    0x82, 0xB0, 0xCE, 0x26, 0x40, 0x1D, 0x2E, 0x22, 0xF0, 0x4D, 0x83, 0xD1, 0x25, 0x5D, 0xC5, 0x1A,
    0xDD, 0xD3, 0xB7, 0x5A, 0x2B, 0x1A, 0xE0, 0x78, 0x45, 0x04, 0xDF, 0x54, 0x3A, 0xF8, 0x96, 0x9B,
    0xE3, 0xEA, 0x70, 0x82, 0xFF, 0x7F, 0xC9, 0x88, 0x8C, 0x14, 0x4D, 0xA2, 0xAF, 0x58, 0x42, 0x9E,
    0xC9, 0x60, 0x31, 0xDB, 0xCA, 0xD3, 0xDA, 0xD9, 0xAF, 0x0D, 0xCB, 0xAA, 0xAF, 0x26, 0x8C, 0xB8,
    0xFC, 0xFF, 0xEA, 0xD9, 0x4F, 0x3C, 0x7C, 0xA4, 0x95, 0xE0, 0x56, 0xA9, 0xB4, 0x7A, 0xCD, 0xB7,
    0x51, 0xFB, 0x73, 0xE6, 0x66, 0xC6, 0xC6, 0x55, 0xAD, 0xE8, 0x29, 0x72, 0x97, 0xD0, 0x7A, 0xD1,
    0xBA, 0x5E, 0x43, 0xF1, 0xBC, 0xA3, 0x23, 0x01, 0x65, 0x13, 0x39, 0xE2, 0x29, 0x04, 0xCC, 0x8C,
    0x42, 0xF5, 0x8C, 0x30, 0xC0, 0x4A, 0xAF, 0xDB, 0x03, 0x8D, 0xDA, 0x08, 0x47, 0xDD, 0x98, 0x8D,
    0xCD, 0xA6, 0xF3, 0xBF, 0xD1, 0x5C, 0x4B, 0x4C, 0x45, 0x25, 0x00, 0x4A, 0xA0, 0x6E, 0xEF, 0xF8,
    0xCA, 0x61, 0x78, 0x3A, 0xAC, 0xEC, 0x57, 0xFB, 0x3D, 0x1F, 0x92, 0xB0, 0xFE, 0x2F, 0xD1, 0xA8,
    0x5F, 0x67, 0x24, 0x51, 0x7B, 0x65, 0xE6, 0x14, 0xAD, 0x68, 0x08, 0xD6, 0xF6, 0xEE, 0x34, 0xDF,
    0xF7, 0x31, 0x0F, 0xDC, 0x82, 0xAE, 0xBF, 0xD9, 0x04, 0xB0, 0x1E, 0x1D, 0xC5, 0x4B, 0x29, 0x27,
    0x09, 0x4B, 0x2D, 0xB6, 0x8D, 0x6F, 0x90, 0x3B, 0x68, 0x40, 0x1A, 0xDE, 0xBF, 0x5A, 0x7E, 0x08,
    0xD7, 0x8F, 0xF4, 0xEF, 0x5D, 0x63, 0x65, 0x3A, 0x65, 0x04, 0x0C, 0xF9, 0xBF, 0xD4, 0xAC, 0xA7,
    0x98, 0x4A, 0x74, 0xD3, 0x71, 0x45, 0x98, 0x67, 0x80, 0xFC, 0x0B, 0x16, 0xAC, 0x45, 0x16, 0x49,
    0xDE, 0x61, 0x88, 0xA7, 0xDB, 0xDF, 0x19, 0x1F, 0x64, 0xB5, 0xFC, 0x5E, 0x2A, 0xB4, 0x7B, 0x57,
    0xF7, 0xF7, 0x27, 0x6C, 0xD4, 0x19, 0xC1, 0x7A, 0x3C, 0xA8, 0xE1, 0xB9, 0x39, 0xAE, 0x49, 0xE4,
    0x88, 0xAC, 0xBA, 0x6B, 0x96, 0x56, 0x10, 0xB5, 0x48, 0x01, 0x09, 0xC8, 0xB1, 0x7B, 0x80, 0xE1,
    0xB7, 0xB7, 0x50, 0xDF, 0xC7, 0x59, 0x8D, 0x5D, 0x50, 0x11, 0xFD, 0x2D, 0xCC, 0x56, 0x00, 0xA3,
    0x2E, 0xF5, 0xB5, 0x2A, 0x1E, 0xCC, 0x82, 0x0E, 0x30, 0x8A, 0xA3, 0x42, 0x72, 0x1A, 0xAC, 0x09,
    0x43, 0xBF, 0x66, 0x86, 0xB6, 0x4B, 0x25, 0x79, 0x37, 0x65, 0x04, 0xCC, 0xC4, 0x93, 0xD9, 0x7E,
    0x6A, 0xED, 0x3F, 0xB0, 0xF9, 0xCD, 0x71, 0xA4, 0x3D, 0xD4, 0x97, 0xF0, 0x1F, 0x17, 0xC0, 0xE2,
    0xCB, 0x37, 0x97, 0xAA, 0x2A, 0x2F, 0x25, 0x66, 0x56, 0x16, 0x8E, 0x6C, 0x49, 0x6A, 0xFC, 0x5F,
    0xB9, 0x32, 0x46, 0xF6, 0xB1, 0x11, 0x63, 0x98, 0xA3, 0x46, 0xF1, 0xA6, 0x41, 0xF3, 0xB0, 0x41,
    0xE9, 0x89, 0xF7, 0x91, 0x4F, 0x90, 0xCC, 0x2C, 0x7F, 0xFF, 0x35, 0x78, 0x76, 0xE5, 0x06, 0xB5,
    0x0D, 0x33, 0x4B, 0xA7, 0x7C, 0x22, 0x5B, 0xC3, 0x07, 0xBA, 0x53, 0x71, 0x52, 0xF3, 0xF1, 0x61,
    0x0E, 0x4E, 0xAF, 0xE5, 0x95, 0xF6, 0xD9, 0xD9, 0x0D, 0x11, 0xFA, 0xA9, 0x33, 0xA1, 0x5E, 0xF1,
    0x36, 0x95, 0x46, 0x86, 0x8A, 0x7F, 0x3A, 0x45, 0xA9, 0x67, 0x68, 0xD4, 0x0F, 0xD9, 0xD0, 0x34,
    0x12, 0xC0, 0x91, 0xC6, 0x31, 0x5C, 0xF4, 0xFD, 0xE7, 0xCB, 0x68, 0x60, 0x69, 0x37, 0x38, 0x0D,
    0xB2, 0xEA, 0xAA, 0x70, 0x7B, 0x4C, 0x41, 0x85, 0xC3, 0x2E, 0xDD, 0xCD, 0xD3, 0x06, 0x70, 0x5E,
    0x4D, 0xC1, 0xFF, 0xC8, 0x72, 0xEE, 0xEE, 0x47, 0x5A, 0x64, 0xDF, 0xAC, 0x86, 0xAB, 0xA4, 0x1C,
    0x06, 0x18, 0x98, 0x3F, 0x87, 0x41, 0xC5, 0xEF, 0x68, 0xD3, 0xA1, 0x01, 0xE8, 0xA3, 0xB8, 0xCA,
    0xC6, 0x0C, 0x90, 0x5C, 0x15, 0xFC, 0x91, 0x08, 0x40, 0xB9, 0x4C, 0x00, 0xA0, 0xB9, 0xD0
};
static const uint8_t Vector_EdDSA_1023_PrivateKey[] =
{
    0xE5, 0x0E, 0xCC, 0xF5, 0x55, 0xF2, 0x6B, 0x4C, 0x74, 0x13, 0xC0, 0x3B, 0x58, 0xCC, 0x60, 0x81,
    0x6C, 0xB8, 0x76, 0x68, 0x22, 0x0F, 0x63, 0x17, 0x95, 0x31, 0x53, 0xF1, 0x7C, 0x76, 0xE5, 0xF5
};
static const uint8_t Vector_EdDSA_1023_PublicKey[] =
{
    0x6E, 0x42, 0x1D, 0x7F, 0x59, 0x7C, 0xEF, 0x1F, 0xC5, 0xC9, 0x28, 0x24, 0x2B, 0xBF, 0xFF, 0xCE,
    0x86, 0x83, 0x6E, 0x31, 0xF2, 0xD0, 0x67, 0x0F, 0x34, 0x72, 0x4C, 0x14, 0xFC, 0x17, 0x81, 0x27
};
static const uint8_t Vector_EdDSA_1023_SignatureR[] =
{
    0x50, 0xC3, 0xFE, 0x60, 0xE4, 0xF6, 0xCB, 0x67, 0xDB, 0xCB, 0xB2, 0x43, 0x48, 0x5E, 0xDF, 0x87,
    0x3A, 0x6A, 0x32, 0x63, 0x46, 0xDF, 0x7C, 0x4D, 0xE2, 0xB3, 0x01, 0x05, 0x90, 0x4C, 0xAB, 0x0A
};
static const uint8_t Vector_EdDSA_1023_SignatureS[] =
{
    0x03, 0x8A, 0x18, 0x4A, 0x70, 0xA9, 0x00, 0xAC, 0xA6, 0x30, 0x1E, 0x68, 0x4F, 0xCD, 0x8F, 0x5E,
    0x4B, 0xD9, 0x36, 0xC4, 0x23, 0xEA, 0xEC, 0x28, 0x45, 0x9F, 0x8F, 0x50, 0xB1, 0x71, 0x53, 0xAA
};

/* Actual test vectors. */
static const TestVector_EdDSA_Rec_t test_vectors_eddsa[] =
{
    {
        &ECurve_Ed25519,
        Vector_EdDSA_64_PrivateKey, sizeof(Vector_EdDSA_64_PrivateKey),
        Vector_EdDSA_64_PublicKey,  sizeof(Vector_EdDSA_64_PublicKey),
        Vector_EdDSA_64_Message,    sizeof(Vector_EdDSA_64_Message),
        Vector_EdDSA_64_SignatureR, sizeof(Vector_EdDSA_64_SignatureR),
        Vector_EdDSA_64_SignatureS, sizeof(Vector_EdDSA_64_SignatureS),
    },
    {
        &ECurve_Ed25519,
        Vector_EdDSA_96_PrivateKey, sizeof(Vector_EdDSA_96_PrivateKey),
        Vector_EdDSA_96_PublicKey,  sizeof(Vector_EdDSA_96_PublicKey),
        Vector_EdDSA_96_Message,    sizeof(Vector_EdDSA_96_Message),
        Vector_EdDSA_96_SignatureR, sizeof(Vector_EdDSA_96_SignatureR),
        Vector_EdDSA_96_SignatureS, sizeof(Vector_EdDSA_96_SignatureS),
    },
    {
        &ECurve_Ed25519,
        Vector_EdDSA_1023_PrivateKey, sizeof(Vector_EdDSA_1023_PrivateKey),
        Vector_EdDSA_1023_PublicKey,  sizeof(Vector_EdDSA_1023_PublicKey),
        Vector_EdDSA_1023_Message,    sizeof(Vector_EdDSA_1023_Message),
        Vector_EdDSA_1023_SignatureR, sizeof(Vector_EdDSA_1023_SignatureR),
        Vector_EdDSA_1023_SignatureS, sizeof(Vector_EdDSA_1023_SignatureS),
    },
};

/* end of file testvectors_eddsa_data.h */