/******************************************************************************
*  Filename:       hw_icb_h
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

#ifndef __HW_ICB_H__
#define __HW_ICB_H__

//*****************************************************************************
//
// This section defines the register offsets of
// ICB component
//
//*****************************************************************************
// Provides information about the interrupt controller
#define ICB_O_ICTR                                                  0x00000004U

// Provides IMPLEMENTATION DEFINED configuration and control options
#define ICB_O_ACTLR                                                 0x00000008U

//*****************************************************************************
//
// Register: ICB_O_ICTR
//
//*****************************************************************************
// Field:  [31:4] RES0
//
// Reserved, RES0
#define ICB_ICTR_RES0_W                                                     28U
#define ICB_ICTR_RES0_M                                             0xFFFFFFF0U
#define ICB_ICTR_RES0_S                                                      4U

// Field:   [3:0] INTLINESNUM
//
// Indicates the number of the highest implemented register in each of the NVIC
// control register sets, or in the case of NVIC_IPR*n, 4×INTLINESNUM
#define ICB_ICTR_INTLINESNUM_W                                               4U
#define ICB_ICTR_INTLINESNUM_M                                      0x0000000FU
#define ICB_ICTR_INTLINESNUM_S                                               0U

//*****************************************************************************
//
// Register: ICB_O_ACTLR
//
//*****************************************************************************
// Field: [31:30] RES0
//
// Reserved, RES0
#define ICB_ACTLR_RES0_W                                                     2U
#define ICB_ACTLR_RES0_M                                            0xC0000000U
#define ICB_ACTLR_RES0_S                                                    30U

// Field:    [29] EXTEXCLALL
//
// External Exclusives Allowed with no MPU
#define ICB_ACTLR_EXTEXCLALL                                        0x20000000U
#define ICB_ACTLR_EXTEXCLALL_M                                      0x20000000U
#define ICB_ACTLR_EXTEXCLALL_S                                              29U

// Field: [28:14] RES0_1
//
// Reserved, RES0
#define ICB_ACTLR_RES0_1_W                                                  15U
#define ICB_ACTLR_RES0_1_M                                          0x1FFFC000U
#define ICB_ACTLR_RES0_1_S                                                  14U

// Field:    [13] SBIST
//
// Bit used internally by Software Test Library (STL)
#define ICB_ACTLR_SBIST                                             0x00002000U
#define ICB_ACTLR_SBIST_M                                           0x00002000U
#define ICB_ACTLR_SBIST_S                                                   13U

// Field:    [12] DISITMATBFLUSH
//
// Disable ATB Flush
#define ICB_ACTLR_DISITMATBFLUSH                                    0x00001000U
#define ICB_ACTLR_DISITMATBFLUSH_M                                  0x00001000U
#define ICB_ACTLR_DISITMATBFLUSH_S                                          12U

// Field:    [11] RES0_2
//
// Reserved, RES0
#define ICB_ACTLR_RES0_2                                            0x00000800U
#define ICB_ACTLR_RES0_2_M                                          0x00000800U
#define ICB_ACTLR_RES0_2_S                                                  11U

// Field:    [10] FPEXCODIS
//
// Disable FPU exception outputs
#define ICB_ACTLR_FPEXCODIS                                         0x00000400U
#define ICB_ACTLR_FPEXCODIS_M                                       0x00000400U
#define ICB_ACTLR_FPEXCODIS_S                                               10U

// Field:     [9] DISOOFP
//
// Disable out-of-order FP instruction completion
#define ICB_ACTLR_DISOOFP                                           0x00000200U
#define ICB_ACTLR_DISOOFP_M                                         0x00000200U
#define ICB_ACTLR_DISOOFP_S                                                  9U

// Field:   [8:3] RES0_3
//
// Reserved, RES0
#define ICB_ACTLR_RES0_3_W                                                   6U
#define ICB_ACTLR_RES0_3_M                                          0x000001F8U
#define ICB_ACTLR_RES0_3_S                                                   3U

// Field:     [2] DISFOLD
//
// Disable dual-issue.
#define ICB_ACTLR_DISFOLD                                           0x00000004U
#define ICB_ACTLR_DISFOLD_M                                         0x00000004U
#define ICB_ACTLR_DISFOLD_S                                                  2U

// Field:     [1] RES0_4
//
// Reserved, RES0
#define ICB_ACTLR_RES0_4                                            0x00000002U
#define ICB_ACTLR_RES0_4_M                                          0x00000002U
#define ICB_ACTLR_RES0_4_S                                                   1U

// Field:     [0] DISMCYCINT
//
// Disable dual-issue.
#define ICB_ACTLR_DISMCYCINT                                        0x00000001U
#define ICB_ACTLR_DISMCYCINT_M                                      0x00000001U
#define ICB_ACTLR_DISMCYCINT_S                                               0U


#endif // __ICB__
