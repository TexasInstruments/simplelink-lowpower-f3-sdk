/******************************************************************************
*  Filename:       hw_fpe_h
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

#ifndef __HW_FPE_H__
#define __HW_FPE_H__

//*****************************************************************************
//
// This section defines the register offsets of
// FPE component
//
//*****************************************************************************
// Holds control data for the Floating-point extension
#define FPE_O_FPCCR                                                 0x00000004U

// Holds the location of the unpopulated floating-point register space
// allocated on an exception stack frame
#define FPE_O_FPCAR                                                 0x00000008U

// Holds the default values for the floating-point status control data that the
// PE assigns to the FPSCR when it creates a new floating-point context
#define FPE_O_FPDSCR                                                0x0000000CU

// Describes the features provided by the Floating-point Extension
#define FPE_O_MVFR0                                                 0x00000010U

// Describes the features provided by the Floating-point Extension
#define FPE_O_MVFR1                                                 0x00000014U

// Describes the features provided by the Floating-point Extension
#define FPE_O_MVFR2                                                 0x00000018U

//*****************************************************************************
//
// Register: FPE_O_FPCCR
//
//*****************************************************************************
// Field:    [31] ASPEN
//
// When this bit is set to 1, execution of a floating-point instruction sets
// the CONTROL.FPCA bit to 1
#define FPE_FPCCR_ASPEN                                             0x80000000U
#define FPE_FPCCR_ASPEN_M                                           0x80000000U
#define FPE_FPCCR_ASPEN_S                                                   31U

// Field:    [30] LSPEN
//
// Enables lazy context save of floating-point state
#define FPE_FPCCR_LSPEN                                             0x40000000U
#define FPE_FPCCR_LSPEN_M                                           0x40000000U
#define FPE_FPCCR_LSPEN_S                                                   30U

// Field:    [29] LSPENS
//
// This bit controls whether the LSPEN bit is writeable from the Non-secure
// state
#define FPE_FPCCR_LSPENS                                            0x20000000U
#define FPE_FPCCR_LSPENS_M                                          0x20000000U
#define FPE_FPCCR_LSPENS_S                                                  29U

// Field:    [28] CLRONRET
//
// Clear floating-point caller saved registers on exception return
#define FPE_FPCCR_CLRONRET                                          0x10000000U
#define FPE_FPCCR_CLRONRET_M                                        0x10000000U
#define FPE_FPCCR_CLRONRET_S                                                28U

// Field:    [27] CLRONRETS
//
// This bit controls whether the CLRONRET bit is writeable from the Non-secure
// state
#define FPE_FPCCR_CLRONRETS                                         0x08000000U
#define FPE_FPCCR_CLRONRETS_M                                       0x08000000U
#define FPE_FPCCR_CLRONRETS_S                                               27U

// Field:    [26] TS
//
// Treat floating-point registers as Secure enable
#define FPE_FPCCR_TS                                                0x04000000U
#define FPE_FPCCR_TS_M                                              0x04000000U
#define FPE_FPCCR_TS_S                                                      26U

// Field: [25:11] RES0
//
// Reserved, RES0
#define FPE_FPCCR_RES0_W                                                    15U
#define FPE_FPCCR_RES0_M                                            0x03FFF800U
#define FPE_FPCCR_RES0_S                                                    11U

// Field:    [10] UFRDY
//
// Indicates whether the software executing when the PE allocated the
// floating-point stack frame was able to set the UsageFault exception to
// pending
#define FPE_FPCCR_UFRDY                                             0x00000400U
#define FPE_FPCCR_UFRDY_M                                           0x00000400U
#define FPE_FPCCR_UFRDY_S                                                   10U

// Field:     [9] SPLIMVIOL
//
// This bit is banked between the Security states and indicates whether the
// floating-point context violates the stack pointer limit that was active when
// lazy state preservation was activated. SPLIMVIOL modifies the lazy
// floating-point state preservation behavior
#define FPE_FPCCR_SPLIMVIOL                                         0x00000200U
#define FPE_FPCCR_SPLIMVIOL_M                                       0x00000200U
#define FPE_FPCCR_SPLIMVIOL_S                                                9U

// Field:     [8] MONRDY
//
// Indicates whether the software executing when the PE allocated the
// floating-point stack frame was able to set the DebugMonitor exception to
// pending
#define FPE_FPCCR_MONRDY                                            0x00000100U
#define FPE_FPCCR_MONRDY_M                                          0x00000100U
#define FPE_FPCCR_MONRDY_S                                                   8U

// Field:     [7] SFRDY
//
// Indicates whether the software executing when the PE allocated the
// floating-point stack frame was able to set the SecureFault exception to
// pending. This bit is only present in the Secure version of the register, and
// behaves as RAZ/WI when accessed from the Non-secure state
#define FPE_FPCCR_SFRDY                                             0x00000080U
#define FPE_FPCCR_SFRDY_M                                           0x00000080U
#define FPE_FPCCR_SFRDY_S                                                    7U

// Field:     [6] BFRDY
//
// Indicates whether the software executing when the PE allocated the
// floating-point stack frame was able to set the BusFault exception to pending
#define FPE_FPCCR_BFRDY                                             0x00000040U
#define FPE_FPCCR_BFRDY_M                                           0x00000040U
#define FPE_FPCCR_BFRDY_S                                                    6U

// Field:     [5] MMRDY
//
// Indicates whether the software executing when the PE allocated the
// floating-point stack frame was able to set the MemManage exception to
// pending
#define FPE_FPCCR_MMRDY                                             0x00000020U
#define FPE_FPCCR_MMRDY_M                                           0x00000020U
#define FPE_FPCCR_MMRDY_S                                                    5U

// Field:     [4] HFRDY
//
// Indicates whether the software executing when the PE allocated the
// floating-point stack frame was able to set the HardFault exception to
// pending
#define FPE_FPCCR_HFRDY                                             0x00000010U
#define FPE_FPCCR_HFRDY_M                                           0x00000010U
#define FPE_FPCCR_HFRDY_S                                                    4U

// Field:     [3] THREAD
//
// Indicates the PE mode when it allocated the floating-point stack frame
#define FPE_FPCCR_THREAD                                            0x00000008U
#define FPE_FPCCR_THREAD_M                                          0x00000008U
#define FPE_FPCCR_THREAD_S                                                   3U

// Field:     [2] S
//
// Security status of the floating-point context. This bit is only present in
// the Secure version of the register, and behaves as RAZ/WI when accessed from
// the Non-secure state. This bit is updated whenever lazy state preservation
// is activated, or when a floating-point instruction is executed
#define FPE_FPCCR_S                                                 0x00000004U
#define FPE_FPCCR_S_M                                               0x00000004U
#define FPE_FPCCR_S_S                                                        2U

// Field:     [1] USER
//
// Indicates the privilege level of the software executing when the PE
// allocated the floating-point stack frame
#define FPE_FPCCR_USER                                              0x00000002U
#define FPE_FPCCR_USER_M                                            0x00000002U
#define FPE_FPCCR_USER_S                                                     1U

// Field:     [0] LSPACT
//
// Indicates whether lazy preservation of the floating-point state is active
#define FPE_FPCCR_LSPACT                                            0x00000001U
#define FPE_FPCCR_LSPACT_M                                          0x00000001U
#define FPE_FPCCR_LSPACT_S                                                   0U

//*****************************************************************************
//
// Register: FPE_O_FPCAR
//
//*****************************************************************************
// Field:  [31:3] ADDRESS
//
// The location of the unpopulated floating-point register space allocated on
// an exception stack frame
#define FPE_FPCAR_ADDRESS_W                                                 29U
#define FPE_FPCAR_ADDRESS_M                                         0xFFFFFFF8U
#define FPE_FPCAR_ADDRESS_S                                                  3U

// Field:   [2:0] RES0
//
// Reserved, RES0
#define FPE_FPCAR_RES0_W                                                     3U
#define FPE_FPCAR_RES0_M                                            0x00000007U
#define FPE_FPCAR_RES0_S                                                     0U

//*****************************************************************************
//
// Register: FPE_O_FPDSCR
//
//*****************************************************************************
// Field: [31:27] RES0
//
// Reserved, RES0
#define FPE_FPDSCR_RES0_W                                                    5U
#define FPE_FPDSCR_RES0_M                                           0xF8000000U
#define FPE_FPDSCR_RES0_S                                                   27U

// Field:    [26] AHP
//
// Default value for FPSCR.AHP
#define FPE_FPDSCR_AHP                                              0x04000000U
#define FPE_FPDSCR_AHP_M                                            0x04000000U
#define FPE_FPDSCR_AHP_S                                                    26U

// Field:    [25] DN
//
// Default value for FPSCR.DN
#define FPE_FPDSCR_DN                                               0x02000000U
#define FPE_FPDSCR_DN_M                                             0x02000000U
#define FPE_FPDSCR_DN_S                                                     25U

// Field:    [24] FZ
//
// Default value for FPSCR.FZ
#define FPE_FPDSCR_FZ                                               0x01000000U
#define FPE_FPDSCR_FZ_M                                             0x01000000U
#define FPE_FPDSCR_FZ_S                                                     24U

// Field: [23:22] RMode
//
// Default value for FPSCR.RMode
#define FPE_FPDSCR_RMODE_W                                                   2U
#define FPE_FPDSCR_RMODE_M                                          0x00C00000U
#define FPE_FPDSCR_RMODE_S                                                  22U

// Field:  [21:0] RES0_1
//
// Reserved, RES0
#define FPE_FPDSCR_RES0_1_W                                                 22U
#define FPE_FPDSCR_RES0_1_M                                         0x003FFFFFU
#define FPE_FPDSCR_RES0_1_S                                                  0U

//*****************************************************************************
//
// Register: FPE_O_MVFR0
//
//*****************************************************************************
// Field: [31:28] FPRound
//
// Indicates the rounding modes supported by the FP Extension
#define FPE_MVFR0_FPROUND_W                                                  4U
#define FPE_MVFR0_FPROUND_M                                         0xF0000000U
#define FPE_MVFR0_FPROUND_S                                                 28U

// Field: [27:24] RES0
//
// Reserved, RES0
#define FPE_MVFR0_RES0_W                                                     4U
#define FPE_MVFR0_RES0_M                                            0x0F000000U
#define FPE_MVFR0_RES0_S                                                    24U

// Field: [23:20] FPSqrt
//
// Indicates the support for FP square root operations
#define FPE_MVFR0_FPSQRT_W                                                   4U
#define FPE_MVFR0_FPSQRT_M                                          0x00F00000U
#define FPE_MVFR0_FPSQRT_S                                                  20U

// Field: [19:16] FPDivide
//
// Indicates the support for FP divide operations
#define FPE_MVFR0_FPDIVIDE_W                                                 4U
#define FPE_MVFR0_FPDIVIDE_M                                        0x000F0000U
#define FPE_MVFR0_FPDIVIDE_S                                                16U

// Field: [15:12] RES0_1
//
// Reserved, RES0
#define FPE_MVFR0_RES0_1_W                                                   4U
#define FPE_MVFR0_RES0_1_M                                          0x0000F000U
#define FPE_MVFR0_RES0_1_S                                                  12U

// Field:  [11:8] FPDP
//
// Indicates support for FP double-precision operations
#define FPE_MVFR0_FPDP_W                                                     4U
#define FPE_MVFR0_FPDP_M                                            0x00000F00U
#define FPE_MVFR0_FPDP_S                                                     8U

// Field:   [7:4] FPSP
//
// Indicates support for FP single-precision operations
#define FPE_MVFR0_FPSP_W                                                     4U
#define FPE_MVFR0_FPSP_M                                            0x000000F0U
#define FPE_MVFR0_FPSP_S                                                     4U

// Field:   [3:0] SIMDReg
//
// Indicates size of FP register file
#define FPE_MVFR0_SIMDREG_W                                                  4U
#define FPE_MVFR0_SIMDREG_M                                         0x0000000FU
#define FPE_MVFR0_SIMDREG_S                                                  0U

//*****************************************************************************
//
// Register: FPE_O_MVFR1
//
//*****************************************************************************
// Field: [31:28] FMAC
//
// Indicates whether the FP Extension implements the fused multiply accumulate
// instructions
#define FPE_MVFR1_FMAC_W                                                     4U
#define FPE_MVFR1_FMAC_M                                            0xF0000000U
#define FPE_MVFR1_FMAC_S                                                    28U

// Field: [27:24] FPHP
//
// Indicates whether the FP Extension implements half-precision FP conversion
// instructions
#define FPE_MVFR1_FPHP_W                                                     4U
#define FPE_MVFR1_FPHP_M                                            0x0F000000U
#define FPE_MVFR1_FPHP_S                                                    24U

// Field:  [23:8] RES0
//
// Reserved, RES0
#define FPE_MVFR1_RES0_W                                                    16U
#define FPE_MVFR1_RES0_M                                            0x00FFFF00U
#define FPE_MVFR1_RES0_S                                                     8U

// Field:   [7:4] FPDNaN
//
// Indicates whether the FP hardware implementation supports NaN propagation
#define FPE_MVFR1_FPDNAN_W                                                   4U
#define FPE_MVFR1_FPDNAN_M                                          0x000000F0U
#define FPE_MVFR1_FPDNAN_S                                                   4U

// Field:   [3:0] FPFtZ
//
// Indicates whether subnormals are always flushed-to-zero
#define FPE_MVFR1_FPFTZ_W                                                    4U
#define FPE_MVFR1_FPFTZ_M                                           0x0000000FU
#define FPE_MVFR1_FPFTZ_S                                                    0U

//*****************************************************************************
//
// Register: FPE_O_MVFR2
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define FPE_MVFR2_RES0_W                                                    24U
#define FPE_MVFR2_RES0_M                                            0xFFFFFF00U
#define FPE_MVFR2_RES0_S                                                     8U

// Field:   [7:4] FPMisc
//
// Indicates support for miscellaneous FP features
#define FPE_MVFR2_FPMISC_W                                                   4U
#define FPE_MVFR2_FPMISC_M                                          0x000000F0U
#define FPE_MVFR2_FPMISC_S                                                   4U

// Field:   [3:0] RES0_1
//
// Reserved, RES0
#define FPE_MVFR2_RES0_1_W                                                   4U
#define FPE_MVFR2_RES0_1_M                                          0x0000000FU
#define FPE_MVFR2_RES0_1_S                                                   0U


#endif // __FPE__
