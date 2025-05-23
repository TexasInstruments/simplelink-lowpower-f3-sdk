/* eip130_domain_ecc_curves.c
 *
 * This file implements the Standard (pre-defined) ECC curves interface.
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
#include <third_party/hsmddk/include/Kit/EIP130/DomainHelper/incl/eip130_domain_ecc_curves.h>

#include <DeviceFamily.h>

#ifndef EIP130DOMAIN_ECC_CURVES_192
// #define EIP130DOMAIN_ECC_CURVES_192
#endif

#ifndef EIP130DOMAIN_ECC_CURVES_224
#define EIP130DOMAIN_ECC_NIST_CURVES_224
// #define EIP130DOMAIN_ECC_BRP_CURVES_224
// #define EIP130DOMAIN_ECC_CURVES_224
#endif

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
    #define _CONST
#else
    #define _CONST const
#endif

    /* NIST curves */
#ifdef EIP130DOMAIN_ECC_CURVES_192
    /* NIST_P_192 = SEC_P_192_R1 */
    static _CONST uint8_t gl_ECC_SEC_P_192_R1[] =
    {
        0xC0, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x01, 0x07,
        0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x02, 0x07, 0xB1, 0xB9, 0x46, 0xC1,
        0xEC, 0xDE, 0xB8, 0xFE, 0x49, 0x30, 0x24, 0x72, 0xAB, 0xE9, 0xA7, 0x0F, 0xE7, 0x80, 0x9C, 0xE5,
        0x19, 0x05, 0x21, 0x64, 0xC0, 0x00, 0x03, 0x07, 0x31, 0x28, 0xD2, 0xB4, 0xB1, 0xC9, 0x6B, 0x14,
        0x36, 0xF8, 0xDE, 0x99, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xC0, 0x00, 0x04, 0x07, 0x12, 0x10, 0xFF, 0x82, 0xFD, 0x0A, 0xFF, 0xF4, 0x00, 0x88, 0xA1, 0x43,
        0xEB, 0x20, 0xBF, 0x7C, 0xF6, 0x90, 0x30, 0xB0, 0x0E, 0xA8, 0x8D, 0x18, 0xC0, 0x00, 0x05, 0x07,
        0x11, 0x48, 0x79, 0x1E, 0xA1, 0x77, 0xF9, 0x73, 0xD5, 0xCD, 0x24, 0x6B, 0xED, 0x11, 0x10, 0x63,
        0x78, 0xDA, 0xC8, 0xFF, 0x95, 0x2B, 0x19, 0x07, 0x01, 0x00, 0x06, 0x07, 0x01, 0x00, 0x00, 0x00
    };
#endif

#ifdef EIP130DOMAIN_ECC_NIST_CURVES_224
    /* NIST_P_224 = SEC_P_224_R1 */
    static _CONST uint8_t gl_ECC_SEC_P_224_R1[] =
    {
        0xE0, 0x00, 0x00, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xE0, 0x00, 0x01, 0x07, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xE0, 0x00, 0x02, 0x07, 0xB4, 0xFF, 0x55, 0x23, 0x43, 0x39, 0x0B, 0x27, 0xBA, 0xD8, 0xBF, 0xD7,
        0xB7, 0xB0, 0x44, 0x50, 0x56, 0x32, 0x41, 0xF5, 0xAB, 0xB3, 0x04, 0x0C, 0x85, 0x0A, 0x05, 0xB4,
        0xE0, 0x00, 0x03, 0x07, 0x3D, 0x2A, 0x5C, 0x5C, 0x45, 0x29, 0xDD, 0x13, 0x3E, 0xF0, 0xB8, 0xE0,
        0xA2, 0x16, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xE0, 0x00, 0x04, 0x07, 0x21, 0x1D, 0x5C, 0x11, 0xD6, 0x80, 0x32, 0x34, 0x22, 0x11, 0xC2, 0x56,
        0xD3, 0xC1, 0x03, 0x4A, 0xB9, 0x90, 0x13, 0x32, 0x7F, 0xBF, 0xB4, 0x6B, 0xBD, 0x0C, 0x0E, 0xB7,
        0xE0, 0x00, 0x05, 0x07, 0x34, 0x7E, 0x00, 0x85, 0x99, 0x81, 0xD5, 0x44, 0x64, 0x47, 0x07, 0x5A,
        0xA0, 0x75, 0x43, 0xCD, 0xE6, 0xDF, 0x22, 0x4C, 0xFB, 0x23, 0xF7, 0xB5, 0x88, 0x63, 0x37, 0xBD,
        0x01, 0x00, 0x06, 0x07, 0x01, 0x00, 0x00, 0x00
    };
#endif

    /* NIST_P_256 = SEC_P_256_R1 */
    static _CONST uint8_t gl_ECC_SEC_P_256_R1[] =
    {
        0x00, 0x01, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x01, 0x07, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x02, 0x07, 0x4B, 0x60, 0xD2, 0x27,
        0x3E, 0x3C, 0xCE, 0x3B, 0xF6, 0xB0, 0x53, 0xCC, 0xB0, 0x06, 0x1D, 0x65, 0xBC, 0x86, 0x98, 0x76,
        0x55, 0xBD, 0xEB, 0xB3, 0xE7, 0x93, 0x3A, 0xAA, 0xD8, 0x35, 0xC6, 0x5A, 0x00, 0x01, 0x03, 0x07,
        0x51, 0x25, 0x63, 0xFC, 0xC2, 0xCA, 0xB9, 0xF3, 0x84, 0x9E, 0x17, 0xA7, 0xAD, 0xFA, 0xE6, 0xBC,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x01, 0x04, 0x07, 0x96, 0xC2, 0x98, 0xD8, 0x45, 0x39, 0xA1, 0xF4, 0xA0, 0x33, 0xEB, 0x2D,
        0x81, 0x7D, 0x03, 0x77, 0xF2, 0x40, 0xA4, 0x63, 0xE5, 0xE6, 0xBC, 0xF8, 0x47, 0x42, 0x2C, 0xE1,
        0xF2, 0xD1, 0x17, 0x6B, 0x00, 0x01, 0x05, 0x07, 0xF5, 0x51, 0xBF, 0x37, 0x68, 0x40, 0xB6, 0xCB,
        0xCE, 0x5E, 0x31, 0x6B, 0x57, 0x33, 0xCE, 0x2B, 0x16, 0x9E, 0x0F, 0x7C, 0x4A, 0xEB, 0xE7, 0x8E,
        0x9B, 0x7F, 0x1A, 0xFE, 0xE2, 0x42, 0xE3, 0x4F, 0x01, 0x00, 0x06, 0x07, 0x01, 0x00, 0x00, 0x00
    };

    /* NIST_P_384 = SEC_P_384_R1 */
    static _CONST uint8_t gl_ECC_SEC_P_384_R1[] =
    {
        0x80, 0x01, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x01, 0x01, 0x07, 0xFC, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x01, 0x02, 0x07, 0xEF, 0x2A, 0xEC, 0xD3,
        0xED, 0xC8, 0x85, 0x2A, 0x9D, 0xD1, 0x2E, 0x8A, 0x8D, 0x39, 0x56, 0xC6, 0x5A, 0x87, 0x13, 0x50,
        0x8F, 0x08, 0x14, 0x03, 0x12, 0x41, 0x81, 0xFE, 0x6E, 0x9C, 0x1D, 0x18, 0x19, 0x2D, 0xF8, 0xE3,
        0x6B, 0x05, 0x8E, 0x98, 0xE4, 0xE7, 0x3E, 0xE2, 0xA7, 0x2F, 0x31, 0xB3, 0x80, 0x01, 0x03, 0x07,
        0x73, 0x29, 0xC5, 0xCC, 0x6A, 0x19, 0xEC, 0xEC, 0x7A, 0xA7, 0xB0, 0x48, 0xB2, 0x0D, 0x1A, 0x58,
        0xDF, 0x2D, 0x37, 0xF4, 0x81, 0x4D, 0x63, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x80, 0x01, 0x04, 0x07, 0xB7, 0x0A, 0x76, 0x72, 0x38, 0x5E, 0x54, 0x3A, 0x6C, 0x29, 0x55, 0xBF,
        0x5D, 0xF2, 0x02, 0x55, 0x38, 0x2A, 0x54, 0x82, 0xE0, 0x41, 0xF7, 0x59, 0x98, 0x9B, 0xA7, 0x8B,
        0x62, 0x3B, 0x1D, 0x6E, 0x74, 0xAD, 0x20, 0xF3, 0x1E, 0xC7, 0xB1, 0x8E, 0x37, 0x05, 0x8B, 0xBE,
        0x22, 0xCA, 0x87, 0xAA, 0x80, 0x01, 0x05, 0x07, 0x5F, 0x0E, 0xEA, 0x90, 0x7C, 0x1D, 0x43, 0x7A,
        0x9D, 0x81, 0x7E, 0x1D, 0xCE, 0xB1, 0x60, 0x0A, 0xC0, 0xB8, 0xF0, 0xB5, 0x13, 0x31, 0xDA, 0xE9,
        0x7C, 0x14, 0x9A, 0x28, 0xBD, 0x1D, 0xF4, 0xF8, 0x29, 0xDC, 0x92, 0x92, 0xBF, 0x98, 0x9E, 0x5D,
        0x6F, 0x2C, 0x26, 0x96, 0x4A, 0xDE, 0x17, 0x36, 0x01, 0x00, 0x06, 0x07, 0x01, 0x00, 0x00, 0x00
    };

    /* NIST_P_521 = SEC_P_521_R1 */
    static _CONST uint8_t gl_ECC_SEC_P_521_R1[] =
    {
        0x09, 0x02, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x09, 0x02, 0x01, 0x07, 0xFC, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00,
        0x09, 0x02, 0x02, 0x07, 0x00, 0x3F, 0x50, 0x6B, 0xD4, 0x1F, 0x45, 0xEF, 0xF1, 0x34, 0x2C, 0x3D,
        0x88, 0xDF, 0x73, 0x35, 0x07, 0xBF, 0xB1, 0x3B, 0xBD, 0xC0, 0x52, 0x16, 0x7B, 0x93, 0x7E, 0xEC,
        0x51, 0x39, 0x19, 0x56, 0xE1, 0x09, 0xF1, 0x8E, 0x91, 0x89, 0xB4, 0xB8, 0xF3, 0x15, 0xB3, 0x99,
        0x5B, 0x72, 0xDA, 0xA2, 0xEE, 0x40, 0x85, 0xB6, 0xA0, 0x21, 0x9A, 0x92, 0x1F, 0x9A, 0x1C, 0x8E,
        0x61, 0xB9, 0x3E, 0x95, 0x51, 0x00, 0x00, 0x00, 0x09, 0x02, 0x03, 0x07, 0x09, 0x64, 0x38, 0x91,
        0x1E, 0xB7, 0x6F, 0xBB, 0xAE, 0x47, 0x9C, 0x89, 0xB8, 0xC9, 0xB5, 0x3B, 0xD0, 0xA5, 0x09, 0xF7,
        0x48, 0x01, 0xCC, 0x7F, 0x6B, 0x96, 0x2F, 0xBF, 0x83, 0x87, 0x86, 0x51, 0xFA, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00,
        0x09, 0x02, 0x04, 0x07, 0x66, 0xBD, 0xE5, 0xC2, 0x31, 0x7E, 0x7E, 0xF9, 0x9B, 0x42, 0x6A, 0x85,
        0xC1, 0xB3, 0x48, 0x33, 0xDE, 0xA8, 0xFF, 0xA2, 0x27, 0xC1, 0x1D, 0xFE, 0x28, 0x59, 0xE7, 0xEF,
        0x77, 0x5E, 0x4B, 0xA1, 0xBA, 0x3D, 0x4D, 0x6B, 0x60, 0xAF, 0x28, 0xF8, 0x21, 0xB5, 0x3F, 0x05,
        0x39, 0x81, 0x64, 0x9C, 0x42, 0xB4, 0x95, 0x23, 0x66, 0xCB, 0x3E, 0x9E, 0xCD, 0xE9, 0x04, 0x04,
        0xB7, 0x06, 0x8E, 0x85, 0xC6, 0x00, 0x00, 0x00, 0x09, 0x02, 0x05, 0x07, 0x50, 0x66, 0xD1, 0x9F,
        0x76, 0x94, 0xBE, 0x88, 0x40, 0xC2, 0x72, 0xA2, 0x86, 0x70, 0x3C, 0x35, 0x61, 0x07, 0xAD, 0x3F,
        0x01, 0xB9, 0x50, 0xC5, 0x40, 0x26, 0xF4, 0x5E, 0x99, 0x72, 0xEE, 0x97, 0x2C, 0x66, 0x3E, 0x27,
        0x17, 0xBD, 0xAF, 0x17, 0x68, 0x44, 0x9B, 0x57, 0x49, 0x44, 0xF5, 0x98, 0xD9, 0x1B, 0x7D, 0x2C,
        0xB4, 0x5F, 0x8A, 0x5C, 0x04, 0xC0, 0x3B, 0x9A, 0x78, 0x6A, 0x29, 0x39, 0x18, 0x01, 0x00, 0x00,
        0x01, 0x00, 0x06, 0x07, 0x01, 0x00, 0x00, 0x00
    };

    /* Brainpool curves */
#ifdef EIP130DOMAIN_ECC_CURVES_192
    /* BRP_P_192_R1 */
    static _CONST uint8_t gl_ECC_BRP_P_192_R1[] =
    {
        0xC0, 0x00, 0x00, 0x17, 0x97, 0x62, 0xA8, 0xE1, 0x6D, 0x47, 0xCE, 0x8F, 0xB7, 0x8D, 0xD1, 0x93,
        0x30, 0x46, 0xA3, 0xA7, 0xCD, 0x36, 0x2A, 0x93, 0x1D, 0xF4, 0x02, 0xC3, 0xC0, 0x00, 0x01, 0x07,
        0xEF, 0x28, 0x9A, 0xC6, 0xE5, 0x40, 0xE0, 0xCA, 0xC1, 0x85, 0x86, 0xFE, 0x31, 0xC0, 0x39, 0x9C,
        0xE1, 0xE0, 0xB1, 0x76, 0x40, 0x17, 0x91, 0x6A, 0xC0, 0x00, 0x02, 0x07, 0xC9, 0x25, 0xBF, 0x6F,
        0x14, 0xF4, 0x7E, 0xCA, 0xBC, 0x96, 0x44, 0x4F, 0x04, 0x1D, 0x72, 0xDC, 0xA3, 0xCC, 0x28, 0x7C,
        0xEF, 0x28, 0x9A, 0x46, 0xC0, 0x00, 0x03, 0x07, 0xC1, 0xAC, 0xC4, 0x9A, 0x02, 0xF1, 0xE8, 0x5B,
        0x6B, 0x91, 0x9E, 0x9E, 0x2F, 0x46, 0xA3, 0xA7, 0xCD, 0x36, 0x2A, 0x93, 0x1D, 0xF4, 0x02, 0xC3,
        0xC0, 0x00, 0x04, 0x07, 0xD6, 0x5F, 0x37, 0x53, 0x48, 0x5C, 0x2F, 0x0A, 0x90, 0xF0, 0xB0, 0x6C,
        0xC5, 0x33, 0xB0, 0x53, 0x87, 0xA4, 0xB6, 0xAA, 0x7E, 0x64, 0xA0, 0xC0, 0xC0, 0x00, 0x05, 0x07,
        0x8F, 0x9B, 0x29, 0xFA, 0xA2, 0x3F, 0x77, 0xE6, 0x02, 0x00, 0x49, 0xC1, 0x28, 0x48, 0x5F, 0x8B,
        0xB8, 0x5B, 0xBD, 0x6A, 0x86, 0x90, 0xB6, 0x14, 0x01, 0x00, 0x06, 0x07, 0x01, 0x00, 0x00, 0x00
    };
#endif

#ifdef EIP130DOMAIN_ECC_BRP_CURVES_224
    /* BRP_P_224_R1 */
    static _CONST uint8_t gl_ECC_BRP_P_224_R1[] =
    {
        0xE0, 0x00, 0x00, 0x17, 0xFF, 0xC0, 0xC8, 0x7E, 0xF5, 0x89, 0xDA, 0x97, 0x57, 0x07, 0x9F, 0xB0,
        0x87, 0xD7, 0xD1, 0x75, 0x25, 0x30, 0x18, 0x2A, 0x86, 0x66, 0x43, 0x26, 0xAA, 0x34, 0xC1, 0xD7,
        0xE0, 0x00, 0x01, 0x07, 0x43, 0x9F, 0xD2, 0xCA, 0x59, 0x2A, 0x04, 0xB0, 0xD8, 0x2A, 0x18, 0x4E,
        0x51, 0x0B, 0x53, 0xC1, 0xA6, 0x03, 0x98, 0x29, 0x1C, 0x6C, 0xCE, 0xA9, 0x2C, 0xE6, 0xA5, 0x68,
        0xE0, 0x00, 0x02, 0x07, 0x0B, 0x40, 0x6C, 0x38, 0x72, 0xB3, 0xDB, 0x66, 0xD2, 0x35, 0x21, 0x3E,
        0xE3, 0x69, 0x23, 0xA9, 0xB1, 0x13, 0x07, 0x87, 0x38, 0x41, 0xE4, 0xCF, 0x3C, 0xF6, 0x80, 0x25,
        0xE0, 0x00, 0x03, 0x07, 0x9F, 0x93, 0xA7, 0xA5, 0xA3, 0xBC, 0xDE, 0x6D, 0x4B, 0xBC, 0x16, 0xD1,
        0x98, 0xFB, 0xD0, 0x75, 0x25, 0x30, 0x18, 0x2A, 0x86, 0x66, 0x43, 0x26, 0xAA, 0x34, 0xC1, 0xD7,
        0xE0, 0x00, 0x04, 0x07, 0x7D, 0xC0, 0x12, 0xEE, 0xFD, 0x6E, 0x1E, 0x4C, 0x17, 0xE3, 0x4C, 0x9E,
        0x8C, 0xC6, 0x7D, 0xA8, 0xB2, 0x23, 0x08, 0x34, 0xF4, 0x5C, 0x7E, 0x2C, 0xAD, 0x29, 0x90, 0x0D,
        0xE0, 0x00, 0x05, 0x07, 0xCD, 0x02, 0x14, 0x76, 0xD3, 0xF6, 0xA3, 0xCA, 0x99, 0x9E, 0x4B, 0x35,
        0x24, 0xAC, 0xCD, 0x4E, 0x9E, 0xB8, 0xC6, 0x24, 0x6F, 0x72, 0xC0, 0x72, 0xF7, 0x56, 0xAA, 0x58,
        0x01, 0x00, 0x06, 0x07, 0x01, 0x00, 0x00, 0x00
    };
#endif

    /* BRP_P_256_R1 */
    static _CONST uint8_t gl_ECC_BRP_P_256_R1[] =
    {
        0x00, 0x01, 0x00, 0x17, 0x77, 0x53, 0x6E, 0x1F, 0x1D, 0x48, 0x13, 0x20, 0x28, 0x20, 0x26, 0xD5,
        0x23, 0xF6, 0x3B, 0x6E, 0x72, 0x8D, 0x83, 0x9D, 0x90, 0x0A, 0x66, 0x3E, 0xBC, 0xA9, 0xEE, 0xA1,
        0xDB, 0x57, 0xFB, 0xA9, 0x00, 0x01, 0x01, 0x07, 0xD9, 0xB5, 0x30, 0xF3, 0x44, 0x4B, 0x4A, 0xE9,
        0x6C, 0x5C, 0xDC, 0x26, 0xC1, 0x55, 0x80, 0xFB, 0xE7, 0xFF, 0x7A, 0x41, 0x30, 0x75, 0xF6, 0xEE,
        0x57, 0x30, 0x2C, 0xFC, 0x75, 0x09, 0x5A, 0x7D, 0x00, 0x01, 0x02, 0x07, 0xB6, 0x07, 0x8C, 0xFF,
        0x18, 0xDC, 0xCC, 0x6B, 0xCE, 0xE1, 0xF7, 0x5C, 0x29, 0x16, 0x84, 0x95, 0xBF, 0x7C, 0xD7, 0xBB,
        0xD9, 0xB5, 0x30, 0xF3, 0x44, 0x4B, 0x4A, 0xE9, 0x6C, 0x5C, 0xDC, 0x26, 0x00, 0x01, 0x03, 0x07,
        0xA7, 0x56, 0x48, 0x97, 0x82, 0x0E, 0x1E, 0x90, 0xF7, 0xA6, 0x61, 0xB5, 0xA3, 0x7A, 0x39, 0x8C,
        0x71, 0x8D, 0x83, 0x9D, 0x90, 0x0A, 0x66, 0x3E, 0xBC, 0xA9, 0xEE, 0xA1, 0xDB, 0x57, 0xFB, 0xA9,
        0x00, 0x01, 0x04, 0x07, 0x62, 0x32, 0xCE, 0x9A, 0xBD, 0x53, 0x44, 0x3A, 0xC2, 0x23, 0xBD, 0xE3,
        0xE1, 0x27, 0xDE, 0xB9, 0xAF, 0xB7, 0x81, 0xFC, 0x2F, 0x48, 0x4B, 0x2C, 0xCB, 0x57, 0x7E, 0xCB,
        0xB9, 0xAE, 0xD2, 0x8B, 0x00, 0x01, 0x05, 0x07, 0x97, 0x69, 0x04, 0x2F, 0xC7, 0x54, 0x1D, 0x5C,
        0x54, 0x8E, 0xED, 0x2D, 0x13, 0x45, 0x77, 0xC2, 0xC9, 0x1D, 0x61, 0x14, 0x1A, 0x46, 0xF8, 0x97,
        0xFD, 0xC4, 0xDA, 0xC3, 0x35, 0xF8, 0x7E, 0x54, 0x01, 0x00, 0x06, 0x07, 0x01, 0x00, 0x00, 0x00
    };

    /* BRP_P_384_R1 */
    static _CONST uint8_t gl_ECC_BRP_P_384_R1[] =
    {
        0x80, 0x01, 0x00, 0x17, 0x53, 0xEC, 0x07, 0x31, 0x13, 0x00, 0x47, 0x87, 0x71, 0x1A, 0x1D, 0x90,
        0x29, 0xA7, 0xD3, 0xAC, 0x23, 0x11, 0xB7, 0x7F, 0x19, 0xDA, 0xB1, 0x12, 0xB4, 0x56, 0x54, 0xED,
        0x09, 0x71, 0x2F, 0x15, 0xDF, 0x41, 0xE6, 0x50, 0x7E, 0x6F, 0x5D, 0x0F, 0x28, 0x6D, 0x38, 0xA3,
        0x82, 0x1E, 0xB9, 0x8C, 0x80, 0x01, 0x01, 0x07, 0x26, 0x28, 0xCE, 0x22, 0xDD, 0xC7, 0xA8, 0x04,
        0xEB, 0xD4, 0x3A, 0x50, 0x4A, 0x81, 0xA5, 0x8A, 0x0F, 0xF9, 0x91, 0xBA, 0xEF, 0x65, 0x91, 0x13,
        0x87, 0x27, 0xB2, 0x4F, 0x8E, 0xA2, 0xBE, 0xC2, 0xA0, 0xAF, 0x05, 0xCE, 0x0A, 0x08, 0x72, 0x3C,
        0x0C, 0x15, 0x8C, 0x3D, 0xC6, 0x82, 0xC3, 0x7B, 0x80, 0x01, 0x02, 0x07, 0x11, 0x4C, 0x50, 0xFA,
        0x96, 0x86, 0xB7, 0x3A, 0x94, 0xC9, 0xDB, 0x95, 0x02, 0x39, 0xB4, 0x7C, 0xD5, 0x62, 0xEB, 0x3E,
        0xA5, 0x0E, 0x88, 0x2E, 0xA6, 0xD2, 0xDC, 0x07, 0xE1, 0x7D, 0xB7, 0x2F, 0x7C, 0x44, 0xF0, 0x16,
        0x54, 0xB5, 0x39, 0x8B, 0x26, 0x28, 0xCE, 0x22, 0xDD, 0xC7, 0xA8, 0x04, 0x80, 0x01, 0x03, 0x07,
        0x65, 0x65, 0x04, 0xE9, 0x02, 0x32, 0x88, 0x3B, 0x10, 0xC3, 0x7F, 0x6B, 0xAF, 0xB6, 0x3A, 0xCF,
        0xA7, 0x25, 0x04, 0xAC, 0x6C, 0x6E, 0x16, 0x1F, 0xB3, 0x56, 0x54, 0xED, 0x09, 0x71, 0x2F, 0x15,
        0xDF, 0x41, 0xE6, 0x50, 0x7E, 0x6F, 0x5D, 0x0F, 0x28, 0x6D, 0x38, 0xA3, 0x82, 0x1E, 0xB9, 0x8C,
        0x80, 0x01, 0x04, 0x07, 0x1E, 0xAF, 0xD4, 0x47, 0xE2, 0xB2, 0x87, 0xEF, 0xAA, 0x46, 0xD6, 0x36,
        0x34, 0xE0, 0x26, 0xE8, 0xE8, 0x10, 0xBD, 0x0C, 0xFE, 0xCA, 0x7F, 0xDB, 0xE3, 0x4F, 0xF1, 0x7E,
        0xE7, 0xA3, 0x47, 0x88, 0x6B, 0x3F, 0xC1, 0xB7, 0x81, 0x3A, 0xA6, 0xA2, 0xFF, 0x45, 0xCF, 0x68,
        0xF0, 0x64, 0x1C, 0x1D, 0x80, 0x01, 0x05, 0x07, 0x15, 0x53, 0x3C, 0x26, 0x41, 0x03, 0x82, 0x42,
        0x11, 0x81, 0x91, 0x77, 0x21, 0x46, 0x46, 0x0E, 0x28, 0x29, 0x91, 0xF9, 0x4F, 0x05, 0x9C, 0xE1,
        0x64, 0x58, 0xEC, 0xFE, 0x29, 0x0B, 0xB7, 0x62, 0x52, 0xD5, 0xCF, 0x95, 0x8E, 0xEB, 0xB1, 0x5C,
        0xA4, 0xC2, 0xF9, 0x20, 0x75, 0x1D, 0xBE, 0x8A, 0x01, 0x00, 0x06, 0x07, 0x01, 0x00, 0x00, 0x00
    };

    /* BRP_P_512_R1 */
    static _CONST uint8_t gl_ECC_BRP_P_512_R1[] =
    {
        0x00, 0x02, 0x00, 0x17, 0xF3, 0x48, 0x3A, 0x58, 0x56, 0x60, 0xAA, 0x28, 0x85, 0xC6, 0x82, 0x2D,
        0x2F, 0xFF, 0x81, 0x28, 0xE6, 0x80, 0xA3, 0xE6, 0x2A, 0xA1, 0xCD, 0xAE, 0x42, 0x68, 0xC6, 0x9B,
        0x00, 0x9B, 0x4D, 0x7D, 0x71, 0x08, 0x33, 0x70, 0xCA, 0x9C, 0x63, 0xD6, 0x0E, 0xD2, 0xC9, 0xB3,
        0xB3, 0x8D, 0x30, 0xCB, 0x07, 0xFC, 0xC9, 0x33, 0xAE, 0xE6, 0xD4, 0x3F, 0x8B, 0xC4, 0xE9, 0xDB,
        0xB8, 0x9D, 0xDD, 0xAA, 0x00, 0x02, 0x01, 0x07, 0xCA, 0x94, 0xFC, 0x77, 0x4D, 0xAC, 0xC1, 0xE7,
        0xB9, 0xC7, 0xF2, 0x2B, 0xA7, 0x17, 0x11, 0x7F, 0xB5, 0xC8, 0x9A, 0x8B, 0xC9, 0xF1, 0x2E, 0x0A,
        0xA1, 0x3A, 0x25, 0xA8, 0x5A, 0x5D, 0xED, 0x2D, 0xBC, 0x63, 0x98, 0xEA, 0xCA, 0x41, 0x34, 0xA8,
        0x10, 0x16, 0xF9, 0x3D, 0x8D, 0xDD, 0xCB, 0x94, 0xC5, 0x4C, 0x23, 0xAC, 0x45, 0x71, 0x32, 0xE2,
        0x89, 0x3B, 0x60, 0x8B, 0x31, 0xA3, 0x30, 0x78, 0x00, 0x02, 0x02, 0x07, 0x23, 0xF7, 0x16, 0x80,
        0x63, 0xBD, 0x09, 0x28, 0xDD, 0xE5, 0xBA, 0x5E, 0xB7, 0x50, 0x40, 0x98, 0x67, 0x3E, 0x08, 0xDC,
        0xCA, 0x94, 0xFC, 0x77, 0x4D, 0xAC, 0xC1, 0xE7, 0xB9, 0xC7, 0xF2, 0x2B, 0xA7, 0x17, 0x11, 0x7F,
        0xB5, 0xC8, 0x9A, 0x8B, 0xC9, 0xF1, 0x2E, 0x0A, 0xA1, 0x3A, 0x25, 0xA8, 0x5A, 0x5D, 0xED, 0x2D,
        0xBC, 0x63, 0x98, 0xEA, 0xCA, 0x41, 0x34, 0xA8, 0x10, 0x16, 0xF9, 0x3D, 0x00, 0x02, 0x03, 0x07,
        0x69, 0x00, 0xA9, 0x9C, 0x82, 0x96, 0x87, 0xB5, 0xDD, 0xDA, 0x5D, 0x08, 0x81, 0xD3, 0xB1, 0x1D,
        0x47, 0x10, 0xAC, 0x7F, 0x19, 0x61, 0x86, 0x41, 0x19, 0x26, 0xA9, 0x4C, 0x41, 0x5C, 0x3E, 0x55,
        0x70, 0x08, 0x33, 0x70, 0xCA, 0x9C, 0x63, 0xD6, 0x0E, 0xD2, 0xC9, 0xB3, 0xB3, 0x8D, 0x30, 0xCB,
        0x07, 0xFC, 0xC9, 0x33, 0xAE, 0xE6, 0xD4, 0x3F, 0x8B, 0xC4, 0xE9, 0xDB, 0xB8, 0x9D, 0xDD, 0xAA,
        0x00, 0x02, 0x04, 0x07, 0x22, 0xF8, 0xB9, 0xBC, 0x09, 0x22, 0x35, 0x8B, 0x68, 0x5E, 0x6A, 0x40,
        0x47, 0x50, 0x6D, 0x7C, 0x5F, 0x7D, 0xB9, 0x93, 0x7B, 0x68, 0xD1, 0x50, 0x8D, 0xD4, 0xD0, 0xE2,
        0x78, 0x1F, 0x3B, 0xFF, 0x8E, 0x09, 0xD0, 0xF4, 0xEE, 0x62, 0x3B, 0xB4, 0xC1, 0x16, 0xD9, 0xB5,
        0x70, 0x9F, 0xED, 0x85, 0x93, 0x6A, 0x4C, 0x9C, 0x2E, 0x32, 0x21, 0x5A, 0x64, 0xD9, 0x2E, 0xD8,
        0xBD, 0xE4, 0xAE, 0x81, 0x00, 0x02, 0x05, 0x07, 0x92, 0x08, 0xD8, 0x3A, 0x0F, 0x1E, 0xCD, 0x78,
        0x06, 0x54, 0xF0, 0xA8, 0x2F, 0x2B, 0xCA, 0xD1, 0xAE, 0x63, 0x27, 0x8A, 0xD8, 0x4B, 0xCA, 0x5B,
        0x5E, 0x48, 0x5F, 0x4A, 0x49, 0xDE, 0xDC, 0xB2, 0x11, 0x81, 0x1F, 0x88, 0x5B, 0xC5, 0x00, 0xA0,
        0x1A, 0x7B, 0xA5, 0x24, 0x00, 0xF7, 0x09, 0xF2, 0xFD, 0x22, 0x78, 0xCF, 0xA9, 0xBF, 0xEA, 0xC0,
        0xEC, 0x32, 0x63, 0x56, 0x5D, 0x38, 0xDE, 0x7D, 0x01, 0x00, 0x06, 0x07, 0x01, 0x00, 0x00, 0x00
    };

    /* Bernstein curves */
    /* curve25519 */
    static _CONST uint8_t gl_ECC_CURVE25519[] =
    {
        0xFF, 0x00, 0x00, 0x07, 0xED, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0x00, 0x01, 0x07, 0x06, 0x6D, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x02, 0x07, 0x01, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x03, 0x07,
        0xED, 0xD3, 0xF5, 0x5C, 0x1A, 0x63, 0x12, 0x58, 0xD6, 0x9C, 0xF7, 0xA2, 0xDE, 0xF9, 0xDE, 0x14,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
        0xFF, 0x00, 0x04, 0x07, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x05, 0x07, 0x14, 0x2C, 0x31, 0x81, 0x5D, 0x3A, 0x16, 0xD6,
        0x4D, 0x9E, 0x83, 0x92, 0x81, 0xB2, 0xC2, 0x6D, 0xB3, 0x2E, 0xB7, 0x88, 0xD3, 0x22, 0xE1, 0x1F,
        0x4B, 0x79, 0x5F, 0x47, 0x5E, 0xE6, 0x51, 0x5F, 0x04, 0x00, 0x06, 0x07, 0x08, 0x00, 0x00, 0x00
    };

    /* Edwards25519 */
    static _CONST uint8_t gl_ECC_ED25519[] =
    {
        0xFF, 0x00, 0x00, 0x17, 0xED, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0x00, 0x01, 0x07, 0xA3, 0x78, 0x59, 0x13, 0xCA, 0x4D, 0xEB, 0x75,
        0xAB, 0xD8, 0x41, 0x41, 0x4D, 0x0A, 0x70, 0x00, 0x98, 0xE8, 0x79, 0x77, 0x79, 0x40, 0xC7, 0x8C,
        0x73, 0xFE, 0x6F, 0x2B, 0xEE, 0x6C, 0x03, 0x52, 0xFF, 0x00, 0x02, 0x07, 0x06, 0x7E, 0x45, 0xFF,
        0xAA, 0x04, 0x6E, 0xCC, 0x82, 0x1A, 0x7D, 0x4B, 0xD1, 0xD3, 0xA1, 0xC5, 0x7E, 0x4F, 0xFC, 0x03,
        0xDC, 0x08, 0x7B, 0xD2, 0xBB, 0x06, 0xA0, 0x60, 0xF4, 0xED, 0x26, 0x0F, 0xFF, 0x00, 0x03, 0x07,
        0xED, 0xD3, 0xF5, 0x5C, 0x1A, 0x63, 0x12, 0x58, 0xD6, 0x9C, 0xF7, 0xA2, 0xDE, 0xF9, 0xDE, 0x14,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
        0xFF, 0x00, 0x04, 0x07, 0x1A, 0xD5, 0x25, 0x8F, 0x60, 0x2D, 0x56, 0xC9, 0xB2, 0xA7, 0x25, 0x95,
        0x60, 0xC7, 0x2C, 0x69, 0x5C, 0xDC, 0xD6, 0xFD, 0x31, 0xE2, 0xA4, 0xC0, 0xFE, 0x53, 0x6E, 0xCD,
        0xD3, 0x36, 0x69, 0x21, 0xFF, 0x00, 0x05, 0x07, 0x58, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
        0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
        0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x04, 0x00, 0x06, 0x07, 0x08, 0x00, 0x00, 0x00
    };

/*----------------------------------------------------------------------------
 * Eip130Domain_ECC_GetCurve
 */
bool
Eip130Domain_ECC_GetCurve(const Eip130Domain_ECCurveFamily_t CurveFamily,
                          const uint32_t CurveBits,
                          const uint8_t ** ppCurveParams,
                          uint32_t * pCurveParamsSize)
{
    bool status = true;

    if (EIP130DOMAIN_ECC_FAMILY_NIST_P == CurveFamily)
    {
#ifdef EIP130DOMAIN_ECC_CURVES_192
        if (192U == CurveBits)
        {
            *ppCurveParams = gl_ECC_SEC_P_192_R1;
            *pCurveParamsSize = sizeof(gl_ECC_SEC_P_192_R1);
        }
        else
#endif
#ifdef EIP130DOMAIN_ECC_NIST_CURVES_224
        if (224U == CurveBits)
        {
            *ppCurveParams = gl_ECC_SEC_P_224_R1;
            *pCurveParamsSize = sizeof(gl_ECC_SEC_P_224_R1);
        }
        else
#endif
        if (256U == CurveBits)
        {
            *ppCurveParams = gl_ECC_SEC_P_256_R1;
            *pCurveParamsSize = sizeof(gl_ECC_SEC_P_256_R1);
        }
        else if (384U == CurveBits)
        {
            *ppCurveParams = gl_ECC_SEC_P_384_R1;
            *pCurveParamsSize = sizeof(gl_ECC_SEC_P_384_R1);
        }
        else if (521U == CurveBits)
        {
            *ppCurveParams = gl_ECC_SEC_P_521_R1;
            *pCurveParamsSize = sizeof(gl_ECC_SEC_P_521_R1);
        }
        else
        {
            *ppCurveParams = NULL;
            *pCurveParamsSize = 0U;
            status = false;
        }
    }
    else if (EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1 == CurveFamily)
    {
#ifdef EIP130DOMAIN_ECC_CURVES_192
        if (192U == CurveBits)
        {
            *ppCurveParams = gl_ECC_BRP_P_192_R1;
            *pCurveParamsSize = sizeof(gl_ECC_BRP_P_192_R1);
        }
        else
#endif
#ifdef EIP130DOMAIN_ECC_BRP_CURVES_224
        if (224U == CurveBits)
        {
            *ppCurveParams = gl_ECC_BRP_P_224_R1;
            *pCurveParamsSize = sizeof(gl_ECC_BRP_P_224_R1);
        }
        else
#endif
        if (256U == CurveBits)
        {
            *ppCurveParams = gl_ECC_BRP_P_256_R1;
            *pCurveParamsSize = sizeof(gl_ECC_BRP_P_256_R1);
        }
        else if (384U == CurveBits)
        {
            *ppCurveParams = gl_ECC_BRP_P_384_R1;
            *pCurveParamsSize = sizeof(gl_ECC_BRP_P_384_R1);
        }
        else if (512U == CurveBits)
        {
            *ppCurveParams = gl_ECC_BRP_P_512_R1;
            *pCurveParamsSize = sizeof(gl_ECC_BRP_P_512_R1);
        }
        else
        {
            *ppCurveParams = NULL;
            *pCurveParamsSize = 0U;
            status = false;
        }
    }
    else if (EIP130DOMAIN_ECC_FAMILY_MONTGOMERY == CurveFamily)
    {
        if (255U == CurveBits)
        {
            *ppCurveParams = gl_ECC_CURVE25519;
            *pCurveParamsSize = sizeof(gl_ECC_CURVE25519);
        }
        else
        {
            *ppCurveParams = NULL;
            *pCurveParamsSize = 0U;
            status = false;
        }
    }
    else if (EIP130DOMAIN_ECC_FAMILY_TWISTED_EDWARDS == CurveFamily)
    {
        if (255U == CurveBits)
        {
            *ppCurveParams =  gl_ECC_ED25519;
            *pCurveParamsSize = sizeof(gl_ECC_ED25519);
        }
        else
        {
            *ppCurveParams = NULL;
            *pCurveParamsSize = 0U;
            status = false;
        }
    }
    else
    {
        *ppCurveParams = NULL;
        *pCurveParamsSize = 0U;
        status = false;
    }

    return status;
}

/* end of file eip130_domain_ecc_curves.c */
