/******************************************************************************
*  Filename:       hw_nvic_h
******************************************************************************
*  Copyright (c) 2024 Texas Instruments Incorporated. All rights reserved.
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

#ifndef __HW_NVIC_H__
#define __HW_NVIC_H__

//*****************************************************************************
//
// This section defines the register offsets of
// NVIC component
//
//*****************************************************************************
// Enables or reads the enabled state of each group of 32 interrupts
#define NVIC_O_NVIC_ISER0                                           0x00000000U

// Enables or reads the enabled state of each group of 32 interrupts
#define NVIC_O_NVIC_ISER1                                           0x00000004U

// Clears or reads the enabled state of each group of 32 interrupts
#define NVIC_O_NVIC_ICER0                                           0x00000080U

// Clears or reads the enabled state of each group of 32 interrupts
#define NVIC_O_NVIC_ICER1                                           0x00000084U

// Enables or reads the pending state of each group of 32 interrupts
#define NVIC_O_NVIC_ISPR0                                           0x00000100U

// Enables or reads the pending state of each group of 32 interrupts
#define NVIC_O_NVIC_ISPR1                                           0x00000104U

// Clears or reads the pending state of each group of 32 interrupts
#define NVIC_O_NVIC_ICPR0                                           0x00000180U

// Clears or reads the pending state of each group of 32 interrupts
#define NVIC_O_NVIC_ICPR1                                           0x00000184U

// For each group of 32 interrupts, shows the active state of each interrupt
#define NVIC_O_NVIC_IABR0                                           0x00000200U

// For each group of 32 interrupts, shows the active state of each interrupt
#define NVIC_O_NVIC_IABR1                                           0x00000204U

// For each group of 32 interrupts, determines whether each interrupt targets
// Non-secure or Secure state
#define NVIC_O_NVIC_ITNS0                                           0x00000280U

// For each group of 32 interrupts, determines whether each interrupt targets
// Non-secure or Secure state
#define NVIC_O_NVIC_ITNS1                                           0x00000284U

// Sets or reads interrupt priorities
#define NVIC_O_NVIC_IPR0                                            0x00000300U

// Sets or reads interrupt priorities
#define NVIC_O_NVIC_IPR1                                            0x00000304U

// Sets or reads interrupt priorities
#define NVIC_O_NVIC_IPR2                                            0x00000308U

// Sets or reads interrupt priorities
#define NVIC_O_NVIC_IPR3                                            0x0000030CU

// Sets or reads interrupt priorities
#define NVIC_O_NVIC_IPR4                                            0x00000310U

// Sets or reads interrupt priorities
#define NVIC_O_NVIC_IPR5                                            0x00000314U

// Sets or reads interrupt priorities
#define NVIC_O_NVIC_IPR6                                            0x00000318U

// Sets or reads interrupt priorities
#define NVIC_O_NVIC_IPR7                                            0x0000031CU

// Sets or reads interrupt priorities
#define NVIC_O_NVIC_IPR8                                            0x00000320U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_ISER0
//
//*****************************************************************************
// Field:  [31:0] SETENA
//
// For SETENA[m] in NVIC_ISER*n, indicates whether interrupt 32*n + m is
// enabled
#define NVIC_NVIC_ISER0_SETENA_W                                            32U
#define NVIC_NVIC_ISER0_SETENA_M                                    0xFFFFFFFFU
#define NVIC_NVIC_ISER0_SETENA_S                                             0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_ISER1
//
//*****************************************************************************
// Field:  [31:0] SETENA
//
// For SETENA[m] in NVIC_ISER*n, indicates whether interrupt 32*n + m is
// enabled
#define NVIC_NVIC_ISER1_SETENA_W                                            32U
#define NVIC_NVIC_ISER1_SETENA_M                                    0xFFFFFFFFU
#define NVIC_NVIC_ISER1_SETENA_S                                             0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_ICER0
//
//*****************************************************************************
// Field:  [31:0] CLRENA
//
// For CLRENA[m] in NVIC_ICER*n, indicates whether interrupt 32*n + m is
// enabled
#define NVIC_NVIC_ICER0_CLRENA_W                                            32U
#define NVIC_NVIC_ICER0_CLRENA_M                                    0xFFFFFFFFU
#define NVIC_NVIC_ICER0_CLRENA_S                                             0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_ICER1
//
//*****************************************************************************
// Field:  [31:0] CLRENA
//
// For CLRENA[m] in NVIC_ICER*n, indicates whether interrupt 32*n + m is
// enabled
#define NVIC_NVIC_ICER1_CLRENA_W                                            32U
#define NVIC_NVIC_ICER1_CLRENA_M                                    0xFFFFFFFFU
#define NVIC_NVIC_ICER1_CLRENA_S                                             0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_ISPR0
//
//*****************************************************************************
// Field:  [31:0] SETPEND
//
// For SETPEND[m] in NVIC_ISPR*n, indicates whether interrupt 32*n + m is
// pending
#define NVIC_NVIC_ISPR0_SETPEND_W                                           32U
#define NVIC_NVIC_ISPR0_SETPEND_M                                   0xFFFFFFFFU
#define NVIC_NVIC_ISPR0_SETPEND_S                                            0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_ISPR1
//
//*****************************************************************************
// Field:  [31:0] SETPEND
//
// For SETPEND[m] in NVIC_ISPR*n, indicates whether interrupt 32*n + m is
// pending
#define NVIC_NVIC_ISPR1_SETPEND_W                                           32U
#define NVIC_NVIC_ISPR1_SETPEND_M                                   0xFFFFFFFFU
#define NVIC_NVIC_ISPR1_SETPEND_S                                            0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_ICPR0
//
//*****************************************************************************
// Field:  [31:0] CLRPEND
//
// For CLRPEND[m] in NVIC_ICPR*n, indicates whether interrupt 32*n + m is
// pending
#define NVIC_NVIC_ICPR0_CLRPEND_W                                           32U
#define NVIC_NVIC_ICPR0_CLRPEND_M                                   0xFFFFFFFFU
#define NVIC_NVIC_ICPR0_CLRPEND_S                                            0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_ICPR1
//
//*****************************************************************************
// Field:  [31:0] CLRPEND
//
// For CLRPEND[m] in NVIC_ICPR*n, indicates whether interrupt 32*n + m is
// pending
#define NVIC_NVIC_ICPR1_CLRPEND_W                                           32U
#define NVIC_NVIC_ICPR1_CLRPEND_M                                   0xFFFFFFFFU
#define NVIC_NVIC_ICPR1_CLRPEND_S                                            0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_IABR0
//
//*****************************************************************************
// Field:  [31:0] ACTIVE
//
// For ACTIVE[m] in NVIC_IABR*n, indicates the active state for interrupt
// 32*n+m
#define NVIC_NVIC_IABR0_ACTIVE_W                                            32U
#define NVIC_NVIC_IABR0_ACTIVE_M                                    0xFFFFFFFFU
#define NVIC_NVIC_IABR0_ACTIVE_S                                             0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_IABR1
//
//*****************************************************************************
// Field:  [31:0] ACTIVE
//
// For ACTIVE[m] in NVIC_IABR*n, indicates the active state for interrupt
// 32*n+m
#define NVIC_NVIC_IABR1_ACTIVE_W                                            32U
#define NVIC_NVIC_IABR1_ACTIVE_M                                    0xFFFFFFFFU
#define NVIC_NVIC_IABR1_ACTIVE_S                                             0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_ITNS0
//
//*****************************************************************************
// Field:  [31:0] ITNS
//
// For ITNS[m] in NVIC_ITNS*n, `IAAMO the target Security state for interrupt
// 32*n+m
#define NVIC_NVIC_ITNS0_ITNS_W                                              32U
#define NVIC_NVIC_ITNS0_ITNS_M                                      0xFFFFFFFFU
#define NVIC_NVIC_ITNS0_ITNS_S                                               0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_ITNS1
//
//*****************************************************************************
// Field:  [31:0] ITNS
//
// For ITNS[m] in NVIC_ITNS*n, `IAAMO the target Security state for interrupt
// 32*n+m
#define NVIC_NVIC_ITNS1_ITNS_W                                              32U
#define NVIC_NVIC_ITNS1_ITNS_M                                      0xFFFFFFFFU
#define NVIC_NVIC_ITNS1_ITNS_S                                               0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_IPR0
//
//*****************************************************************************
// Field: [26:24] PRI_N3
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR0_PRI_N3_W                                              3U
#define NVIC_NVIC_IPR0_PRI_N3_M                                     0x07000000U
#define NVIC_NVIC_IPR0_PRI_N3_S                                             24U

// Field: [18:16] PRI_N2
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR0_PRI_N2_W                                              3U
#define NVIC_NVIC_IPR0_PRI_N2_M                                     0x00070000U
#define NVIC_NVIC_IPR0_PRI_N2_S                                             16U

// Field:  [10:8] PRI_N1
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+1, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR0_PRI_N1_W                                              3U
#define NVIC_NVIC_IPR0_PRI_N1_M                                     0x00000700U
#define NVIC_NVIC_IPR0_PRI_N1_S                                              8U

// Field:   [2:0] PRI_N0
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+0, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR0_PRI_N0_W                                              3U
#define NVIC_NVIC_IPR0_PRI_N0_M                                     0x00000007U
#define NVIC_NVIC_IPR0_PRI_N0_S                                              0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_IPR1
//
//*****************************************************************************
// Field: [26:24] PRI_N3
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR1_PRI_N3_W                                              3U
#define NVIC_NVIC_IPR1_PRI_N3_M                                     0x07000000U
#define NVIC_NVIC_IPR1_PRI_N3_S                                             24U

// Field: [18:16] PRI_N2
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR1_PRI_N2_W                                              3U
#define NVIC_NVIC_IPR1_PRI_N2_M                                     0x00070000U
#define NVIC_NVIC_IPR1_PRI_N2_S                                             16U

// Field:  [10:8] PRI_N1
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+1, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR1_PRI_N1_W                                              3U
#define NVIC_NVIC_IPR1_PRI_N1_M                                     0x00000700U
#define NVIC_NVIC_IPR1_PRI_N1_S                                              8U

// Field:   [2:0] PRI_N0
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+0, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR1_PRI_N0_W                                              3U
#define NVIC_NVIC_IPR1_PRI_N0_M                                     0x00000007U
#define NVIC_NVIC_IPR1_PRI_N0_S                                              0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_IPR2
//
//*****************************************************************************
// Field: [26:24] PRI_N3
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR2_PRI_N3_W                                              3U
#define NVIC_NVIC_IPR2_PRI_N3_M                                     0x07000000U
#define NVIC_NVIC_IPR2_PRI_N3_S                                             24U

// Field: [18:16] PRI_N2
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR2_PRI_N2_W                                              3U
#define NVIC_NVIC_IPR2_PRI_N2_M                                     0x00070000U
#define NVIC_NVIC_IPR2_PRI_N2_S                                             16U

// Field:  [10:8] PRI_N1
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+1, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR2_PRI_N1_W                                              3U
#define NVIC_NVIC_IPR2_PRI_N1_M                                     0x00000700U
#define NVIC_NVIC_IPR2_PRI_N1_S                                              8U

// Field:   [2:0] PRI_N0
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+0, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR2_PRI_N0_W                                              3U
#define NVIC_NVIC_IPR2_PRI_N0_M                                     0x00000007U
#define NVIC_NVIC_IPR2_PRI_N0_S                                              0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_IPR3
//
//*****************************************************************************
// Field: [26:24] PRI_N3
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR3_PRI_N3_W                                              3U
#define NVIC_NVIC_IPR3_PRI_N3_M                                     0x07000000U
#define NVIC_NVIC_IPR3_PRI_N3_S                                             24U

// Field: [18:16] PRI_N2
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR3_PRI_N2_W                                              3U
#define NVIC_NVIC_IPR3_PRI_N2_M                                     0x00070000U
#define NVIC_NVIC_IPR3_PRI_N2_S                                             16U

// Field:  [10:8] PRI_N1
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+1, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR3_PRI_N1_W                                              3U
#define NVIC_NVIC_IPR3_PRI_N1_M                                     0x00000700U
#define NVIC_NVIC_IPR3_PRI_N1_S                                              8U

// Field:   [2:0] PRI_N0
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+0, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR3_PRI_N0_W                                              3U
#define NVIC_NVIC_IPR3_PRI_N0_M                                     0x00000007U
#define NVIC_NVIC_IPR3_PRI_N0_S                                              0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_IPR4
//
//*****************************************************************************
// Field: [26:24] PRI_N3
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR4_PRI_N3_W                                              3U
#define NVIC_NVIC_IPR4_PRI_N3_M                                     0x07000000U
#define NVIC_NVIC_IPR4_PRI_N3_S                                             24U

// Field: [18:16] PRI_N2
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR4_PRI_N2_W                                              3U
#define NVIC_NVIC_IPR4_PRI_N2_M                                     0x00070000U
#define NVIC_NVIC_IPR4_PRI_N2_S                                             16U

// Field:  [10:8] PRI_N1
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+1, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR4_PRI_N1_W                                              3U
#define NVIC_NVIC_IPR4_PRI_N1_M                                     0x00000700U
#define NVIC_NVIC_IPR4_PRI_N1_S                                              8U

// Field:   [2:0] PRI_N0
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+0, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR4_PRI_N0_W                                              3U
#define NVIC_NVIC_IPR4_PRI_N0_M                                     0x00000007U
#define NVIC_NVIC_IPR4_PRI_N0_S                                              0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_IPR5
//
//*****************************************************************************
// Field: [26:24] PRI_N3
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR5_PRI_N3_W                                              3U
#define NVIC_NVIC_IPR5_PRI_N3_M                                     0x07000000U
#define NVIC_NVIC_IPR5_PRI_N3_S                                             24U

// Field: [18:16] PRI_N2
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR5_PRI_N2_W                                              3U
#define NVIC_NVIC_IPR5_PRI_N2_M                                     0x00070000U
#define NVIC_NVIC_IPR5_PRI_N2_S                                             16U

// Field:  [10:8] PRI_N1
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+1, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR5_PRI_N1_W                                              3U
#define NVIC_NVIC_IPR5_PRI_N1_M                                     0x00000700U
#define NVIC_NVIC_IPR5_PRI_N1_S                                              8U

// Field:   [2:0] PRI_N0
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+0, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR5_PRI_N0_W                                              3U
#define NVIC_NVIC_IPR5_PRI_N0_M                                     0x00000007U
#define NVIC_NVIC_IPR5_PRI_N0_S                                              0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_IPR6
//
//*****************************************************************************
// Field: [26:24] PRI_N3
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR6_PRI_N3_W                                              3U
#define NVIC_NVIC_IPR6_PRI_N3_M                                     0x07000000U
#define NVIC_NVIC_IPR6_PRI_N3_S                                             24U

// Field: [18:16] PRI_N2
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR6_PRI_N2_W                                              3U
#define NVIC_NVIC_IPR6_PRI_N2_M                                     0x00070000U
#define NVIC_NVIC_IPR6_PRI_N2_S                                             16U

// Field:  [10:8] PRI_N1
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+1, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR6_PRI_N1_W                                              3U
#define NVIC_NVIC_IPR6_PRI_N1_M                                     0x00000700U
#define NVIC_NVIC_IPR6_PRI_N1_S                                              8U

// Field:   [2:0] PRI_N0
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+0, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR6_PRI_N0_W                                              3U
#define NVIC_NVIC_IPR6_PRI_N0_M                                     0x00000007U
#define NVIC_NVIC_IPR6_PRI_N0_S                                              0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_IPR7
//
//*****************************************************************************
// Field: [26:24] PRI_N3
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR7_PRI_N3_W                                              3U
#define NVIC_NVIC_IPR7_PRI_N3_M                                     0x07000000U
#define NVIC_NVIC_IPR7_PRI_N3_S                                             24U

// Field: [18:16] PRI_N2
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR7_PRI_N2_W                                              3U
#define NVIC_NVIC_IPR7_PRI_N2_M                                     0x00070000U
#define NVIC_NVIC_IPR7_PRI_N2_S                                             16U

// Field:  [10:8] PRI_N1
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+1, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR7_PRI_N1_W                                              3U
#define NVIC_NVIC_IPR7_PRI_N1_M                                     0x00000700U
#define NVIC_NVIC_IPR7_PRI_N1_S                                              8U

// Field:   [2:0] PRI_N0
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+0, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR7_PRI_N0_W                                              3U
#define NVIC_NVIC_IPR7_PRI_N0_M                                     0x00000007U
#define NVIC_NVIC_IPR7_PRI_N0_S                                              0U

//*****************************************************************************
//
// Register: NVIC_O_NVIC_IPR8
//
//*****************************************************************************
// Field: [18:16] PRI_N2
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+2, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR8_PRI_N2_W                                              3U
#define NVIC_NVIC_IPR8_PRI_N2_M                                     0x00070000U
#define NVIC_NVIC_IPR8_PRI_N2_S                                             16U

// Field:  [10:8] PRI_N1
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+1, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR8_PRI_N1_W                                              3U
#define NVIC_NVIC_IPR8_PRI_N1_M                                     0x00000700U
#define NVIC_NVIC_IPR8_PRI_N1_S                                              8U

// Field:   [2:0] PRI_N0
//
// For register NVIC_IPR*n, `IAAMO the priority of interrupt number 4*n+0, or
// is RES0 if the PE does not implement this interrupt
#define NVIC_NVIC_IPR8_PRI_N0_W                                              3U
#define NVIC_NVIC_IPR8_PRI_N0_M                                     0x00000007U
#define NVIC_NVIC_IPR8_PRI_N0_S                                              0U


#endif // __NVIC__
