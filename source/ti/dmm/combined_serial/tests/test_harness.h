/*
 * Copyright (c) 2025, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Texas Instruments Incorporated nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file  test_harness.h
 * @brief Minimal single-header unit-test framework for the Combined Serial
 *        MUX test suite.
 *
 * Design goals:
 *  - Zero external dependencies
 *  - Each test binary is a single .c file that includes this header once
 *  - Non-fatal expectations: a failing EXPECT records the failure but
 *    execution continues so that all assertions in a test function are
 *    evaluated before reporting
 *  - Exit code 0 = all tests passed; 1 = at least one failure
 *
 * Usage:
 * @code
 *   #include "test_harness.h"
 *
 *   static void test_example(void) {
 *       EXPECT_EQ(1 + 1, 2);
 *       EXPECT_TRUE(1 > 0);
 *   }
 *
 *   int main(void) {
 *       RUN(test_example);
 *       TEST_SUMMARY();
 *   }
 * @endcode
 */

#ifndef TEST_HARNESS_H
#define TEST_HARNESS_H

#include <stdio.h>

/* Counters — one set per translation unit (each test binary is one .c file) */
static int g_pass = 0;
static int g_fail = 0;
static int g_test_fail = 0;  /* failures within the current test function */

/*---------------------------------------------------------------------------
 * Expectation macros — non-fatal, execution continues after failure
 *--------------------------------------------------------------------------*/

#define EXPECT(cond)                                                            \
    do {                                                                        \
        if (cond) {                                                             \
            g_pass++;                                                           \
        } else {                                                                \
            fprintf(stderr, "    EXPECT FAILED  %s:%d: %s\n",                  \
                    __FILE__, __LINE__, #cond);                                 \
            g_fail++;                                                           \
            g_test_fail++;                                                      \
        }                                                                       \
    } while (0)

#define EXPECT_EQ(a, b)   EXPECT((a) == (b))
#define EXPECT_NE(a, b)   EXPECT((a) != (b))
#define EXPECT_TRUE(x)    EXPECT(!!(x))
#define EXPECT_FALSE(x)   EXPECT(!(x))
#define EXPECT_NULL(x)    EXPECT((x) == NULL)
#define EXPECT_NONNULL(x) EXPECT((x) != NULL)

/*---------------------------------------------------------------------------
 * Test runner macro
 *--------------------------------------------------------------------------*/

#define RUN(fn)                                                                 \
    do {                                                                        \
        g_test_fail = 0;                                                        \
        fn();                                                                   \
        if (g_test_fail == 0) {                                                 \
            printf("  [ PASS ]  %s\n", #fn);                                   \
        } else {                                                                \
            printf("  [ FAIL ]  %s  (%d expectation(s) failed)\n",             \
                   #fn, g_test_fail);                                           \
        }                                                                       \
    } while (0)

/*---------------------------------------------------------------------------
 * Summary macro — place at the end of main(), acts as 'return'
 *--------------------------------------------------------------------------*/

#define TEST_SUMMARY()                                                          \
    do {                                                                        \
        printf("\n%d passed, %d failed (out of %d expectations)\n",             \
               g_pass, g_fail, g_pass + g_fail);                               \
        return (g_fail > 0) ? 1 : 0;                                           \
    } while (0)

#endif /* TEST_HARNESS_H */
