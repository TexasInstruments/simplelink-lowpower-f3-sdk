/* testvectors_chacha20.h
 *
 * Description: Test vectors for ChaCha20 encrypt.
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

#ifndef INCLUDE_GUARD_TESTVECTORS_CHACHA20_H
#define INCLUDE_GUARD_TESTVECTORS_CHACHA20_H

#include <third_party/hsmddk/include/Kit/DriverFramework/Basic_Defs_API/incl/basic_defs.h>

typedef struct
{
    const uint8_t * Key_p;
    const uint8_t * Nonce_p;
    const uint8_t * Counter_p;
    const uint8_t * PlainData_p;
    const uint8_t * CipherData_p;
    uint32_t KeyLen;
    uint32_t NonceLen;
    uint32_t CounterLen;
    uint32_t DataLen;
} TestVector_ChaCha20_Rec_t;

typedef const TestVector_ChaCha20_Rec_t * TestVector_ChaCha20_t;


/* The function API for accessing the vectors. */
uint32_t
test_vectors_chacha20_num(void);

TestVector_ChaCha20_t
test_vectors_chacha20_get(uint32_t Index);

void
test_vectors_chacha20_release(TestVector_ChaCha20_t Vector_p);


#endif /* INCLUDE_GUARD_TESTVECTORS_CHACHA20_H */

/* end of file testvectors_chacha20.h */