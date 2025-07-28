/******************************************************************************
*  Filename:       hw_fpb_h
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

#ifndef __HW_FPB_H__
#define __HW_FPB_H__

//*****************************************************************************
//
// This section defines the register offsets of
// FPB component
//
//*****************************************************************************
// Provides FPB implementation information, and the global enable for the FPB
// unit
#define FPB_O_FP_CTRL                                               0x00000000U

// Indicates whether the implementation supports Flash Patch remap and, if it
// does, holds the target address for remap
#define FPB_O_FP_REMAP                                              0x00000004U

// Holds an address for comparison.
#define FPB_O_FP_COMP0                                              0x00000008U

// Holds an address for comparison.
#define FPB_O_FP_COMP1                                              0x0000000CU

// Holds an address for comparison.
#define FPB_O_FP_COMP2                                              0x00000010U

// Holds an address for comparison.
#define FPB_O_FP_COMP3                                              0x00000014U

// Holds an address for comparison.
#define FPB_O_FP_COMP4                                              0x00000018U

// Holds an address for comparison.
#define FPB_O_FP_COMP5                                              0x0000001CU

// Holds an address for comparison.
#define FPB_O_FP_COMP6                                              0x00000020U

// Holds an address for comparison.
#define FPB_O_FP_COMP7                                              0x00000024U

// Provides CoreSight discovery information for the FPB
#define FPB_O_FP_DEVARCH                                            0x00000FBCU

// Provides CoreSight discovery information for the FPB
#define FPB_O_FP_DEVTYPE                                            0x00000FCCU

// Provides CoreSight discovery information for the FP
#define FPB_O_FP_PIDR4                                              0x00000FD0U

// Provides CoreSight discovery information for the FP
#define FPB_O_FP_PIDR5                                              0x00000FD4U

// Provides CoreSight discovery information for the FP
#define FPB_O_FP_PIDR6                                              0x00000FD8U

// Provides CoreSight discovery information for the FP
#define FPB_O_FP_PIDR7                                              0x00000FDCU

// Provides CoreSight discovery information for the FP
#define FPB_O_FP_PIDR0                                              0x00000FE0U

// Provides CoreSight discovery information for the FP
#define FPB_O_FP_PIDR1                                              0x00000FE4U

// Provides CoreSight discovery information for the FP
#define FPB_O_FP_PIDR2                                              0x00000FE8U

// Provides CoreSight discovery information for the FP
#define FPB_O_FP_PIDR3                                              0x00000FECU

// Provides CoreSight discovery information for the FP
#define FPB_O_FP_CIDR0                                              0x00000FF0U

// Provides CoreSight discovery information for the FP
#define FPB_O_FP_CIDR1                                              0x00000FF4U

// Provides CoreSight discovery information for the FP
#define FPB_O_FP_CIDR2                                              0x00000FF8U

// Provides CoreSight discovery information for the FP
#define FPB_O_FP_CIDR3                                              0x00000FFCU

//*****************************************************************************
//
// Register: FPB_O_FP_CTRL
//
//*****************************************************************************
// Field: [31:28] REV
//
// Flash Patch and Breakpoint Unit architecture revision
#define FPB_FP_CTRL_REV_W                                                    4U
#define FPB_FP_CTRL_REV_M                                           0xF0000000U
#define FPB_FP_CTRL_REV_S                                                   28U

// Field: [27:15] RES0
//
// Reserved, RES0
#define FPB_FP_CTRL_RES0_W                                                  13U
#define FPB_FP_CTRL_RES0_M                                          0x0FFF8000U
#define FPB_FP_CTRL_RES0_S                                                  15U

// Field: [14:12] NUM_CODE_14_12_
//
// Indicates the number of implemented instruction address comparators. Zero
// indicates no Instruction Address comparators are implemented. The
// Instruction Address comparators are numbered from 0 to NUM_CODE - 1
#define FPB_FP_CTRL_NUM_CODE_14_12__W                                        3U
#define FPB_FP_CTRL_NUM_CODE_14_12__M                               0x00007000U
#define FPB_FP_CTRL_NUM_CODE_14_12__S                                       12U

// Field:  [11:8] NUM_LIT
//
// Indicates the number of implemented literal address comparators. The Literal
// Address comparators are numbered from NUM_CODE to NUM_CODE + NUM_LIT - 1
#define FPB_FP_CTRL_NUM_LIT_W                                                4U
#define FPB_FP_CTRL_NUM_LIT_M                                       0x00000F00U
#define FPB_FP_CTRL_NUM_LIT_S                                                8U

// Field:   [7:4] NUM_CODE_7_4_
//
// Indicates the number of implemented instruction address comparators. Zero
// indicates no Instruction Address comparators are implemented. The
// Instruction Address comparators are numbered from 0 to NUM_CODE - 1
#define FPB_FP_CTRL_NUM_CODE_7_4__W                                          4U
#define FPB_FP_CTRL_NUM_CODE_7_4__M                                 0x000000F0U
#define FPB_FP_CTRL_NUM_CODE_7_4__S                                          4U

// Field:   [3:2] RES0_1
//
// Reserved, RES0
#define FPB_FP_CTRL_RES0_1_W                                                 2U
#define FPB_FP_CTRL_RES0_1_M                                        0x0000000CU
#define FPB_FP_CTRL_RES0_1_S                                                 2U

// Field:     [1] KEY
//
// Writes to the FP_CTRL are ignored unless KEY is concurrently written to one
#define FPB_FP_CTRL_KEY                                             0x00000002U
#define FPB_FP_CTRL_KEY_M                                           0x00000002U
#define FPB_FP_CTRL_KEY_S                                                    1U

// Field:     [0] ENABLE
//
// Enables the FPB
#define FPB_FP_CTRL_ENABLE                                          0x00000001U
#define FPB_FP_CTRL_ENABLE_M                                        0x00000001U
#define FPB_FP_CTRL_ENABLE_S                                                 0U

//*****************************************************************************
//
// Register: FPB_O_FP_REMAP
//
//*****************************************************************************
// Field: [31:30] RES0
//
// Reserved, RES0
#define FPB_FP_REMAP_RES0_W                                                  2U
#define FPB_FP_REMAP_RES0_M                                         0xC0000000U
#define FPB_FP_REMAP_RES0_S                                                 30U

// Field:    [29] RMPSPT
//
// Indicates whether the FPB unit supports the Flash Patch remap function
#define FPB_FP_REMAP_RMPSPT                                         0x20000000U
#define FPB_FP_REMAP_RMPSPT_M                                       0x20000000U
#define FPB_FP_REMAP_RMPSPT_S                                               29U

// Field:  [28:5] REMAP
//
// Holds the bits[28:5] of the Flash Patch remap address
#define FPB_FP_REMAP_REMAP_W                                                24U
#define FPB_FP_REMAP_REMAP_M                                        0x1FFFFFE0U
#define FPB_FP_REMAP_REMAP_S                                                 5U

// Field:   [4:0] RES0_1
//
// Reserved, RES0
#define FPB_FP_REMAP_RES0_1_W                                                5U
#define FPB_FP_REMAP_RES0_1_M                                       0x0000001FU
#define FPB_FP_REMAP_RES0_1_S                                                0U

//*****************************************************************************
//
// Register: FPB_O_FP_COMP0
//
//*****************************************************************************
// Field:  [31:1] BPADDR
//
// Specifies bits[31:1] of the breakpoint instruction address
#define FPB_FP_COMP0_BPADDR_W                                               31U
#define FPB_FP_COMP0_BPADDR_M                                       0xFFFFFFFEU
#define FPB_FP_COMP0_BPADDR_S                                                1U

// Field:     [0] BE
//
// Selects between remapping and breakpoint functionality
#define FPB_FP_COMP0_BE                                             0x00000001U
#define FPB_FP_COMP0_BE_M                                           0x00000001U
#define FPB_FP_COMP0_BE_S                                                    0U

//*****************************************************************************
//
// Register: FPB_O_FP_COMP1
//
//*****************************************************************************
// Field:  [31:1] BPADDR
//
// Specifies bits[31:1] of the breakpoint instruction address
#define FPB_FP_COMP1_BPADDR_W                                               31U
#define FPB_FP_COMP1_BPADDR_M                                       0xFFFFFFFEU
#define FPB_FP_COMP1_BPADDR_S                                                1U

// Field:     [0] BE
//
// Selects between remapping and breakpoint functionality
#define FPB_FP_COMP1_BE                                             0x00000001U
#define FPB_FP_COMP1_BE_M                                           0x00000001U
#define FPB_FP_COMP1_BE_S                                                    0U

//*****************************************************************************
//
// Register: FPB_O_FP_COMP2
//
//*****************************************************************************
// Field:  [31:1] BPADDR
//
// Specifies bits[31:1] of the breakpoint instruction address
#define FPB_FP_COMP2_BPADDR_W                                               31U
#define FPB_FP_COMP2_BPADDR_M                                       0xFFFFFFFEU
#define FPB_FP_COMP2_BPADDR_S                                                1U

// Field:     [0] BE
//
// Selects between remapping and breakpoint functionality
#define FPB_FP_COMP2_BE                                             0x00000001U
#define FPB_FP_COMP2_BE_M                                           0x00000001U
#define FPB_FP_COMP2_BE_S                                                    0U

//*****************************************************************************
//
// Register: FPB_O_FP_COMP3
//
//*****************************************************************************
// Field:  [31:1] BPADDR
//
// Specifies bits[31:1] of the breakpoint instruction address
#define FPB_FP_COMP3_BPADDR_W                                               31U
#define FPB_FP_COMP3_BPADDR_M                                       0xFFFFFFFEU
#define FPB_FP_COMP3_BPADDR_S                                                1U

// Field:     [0] BE
//
// Selects between remapping and breakpoint functionality
#define FPB_FP_COMP3_BE                                             0x00000001U
#define FPB_FP_COMP3_BE_M                                           0x00000001U
#define FPB_FP_COMP3_BE_S                                                    0U

//*****************************************************************************
//
// Register: FPB_O_FP_COMP4
//
//*****************************************************************************
// Field:  [31:1] BPADDR
//
// Specifies bits[31:1] of the breakpoint instruction address
#define FPB_FP_COMP4_BPADDR_W                                               31U
#define FPB_FP_COMP4_BPADDR_M                                       0xFFFFFFFEU
#define FPB_FP_COMP4_BPADDR_S                                                1U

// Field:     [0] BE
//
// Selects between remapping and breakpoint functionality
#define FPB_FP_COMP4_BE                                             0x00000001U
#define FPB_FP_COMP4_BE_M                                           0x00000001U
#define FPB_FP_COMP4_BE_S                                                    0U

//*****************************************************************************
//
// Register: FPB_O_FP_COMP5
//
//*****************************************************************************
// Field:  [31:1] BPADDR
//
// Specifies bits[31:1] of the breakpoint instruction address
#define FPB_FP_COMP5_BPADDR_W                                               31U
#define FPB_FP_COMP5_BPADDR_M                                       0xFFFFFFFEU
#define FPB_FP_COMP5_BPADDR_S                                                1U

// Field:     [0] BE
//
// Selects between remapping and breakpoint functionality
#define FPB_FP_COMP5_BE                                             0x00000001U
#define FPB_FP_COMP5_BE_M                                           0x00000001U
#define FPB_FP_COMP5_BE_S                                                    0U

//*****************************************************************************
//
// Register: FPB_O_FP_COMP6
//
//*****************************************************************************
// Field:  [31:1] BPADDR
//
// Specifies bits[31:1] of the breakpoint instruction address
#define FPB_FP_COMP6_BPADDR_W                                               31U
#define FPB_FP_COMP6_BPADDR_M                                       0xFFFFFFFEU
#define FPB_FP_COMP6_BPADDR_S                                                1U

// Field:     [0] BE
//
// Selects between remapping and breakpoint functionality
#define FPB_FP_COMP6_BE                                             0x00000001U
#define FPB_FP_COMP6_BE_M                                           0x00000001U
#define FPB_FP_COMP6_BE_S                                                    0U

//*****************************************************************************
//
// Register: FPB_O_FP_COMP7
//
//*****************************************************************************
// Field:  [31:1] BPADDR
//
// Specifies bits[31:1] of the breakpoint instruction address
#define FPB_FP_COMP7_BPADDR_W                                               31U
#define FPB_FP_COMP7_BPADDR_M                                       0xFFFFFFFEU
#define FPB_FP_COMP7_BPADDR_S                                                1U

// Field:     [0] BE
//
// Selects between remapping and breakpoint functionality
#define FPB_FP_COMP7_BE                                             0x00000001U
#define FPB_FP_COMP7_BE_M                                           0x00000001U
#define FPB_FP_COMP7_BE_S                                                    0U

//*****************************************************************************
//
// Register: FPB_O_FP_DEVARCH
//
//*****************************************************************************
// Field: [31:21] ARCHITECT
//
// Defines the architect of the component. Bits [31:28] are the JEP106
// continuation code (JEP106 bank ID, minus 1) and bits [27:21] are the JEP106
// ID code.
#define FPB_FP_DEVARCH_ARCHITECT_W                                          11U
#define FPB_FP_DEVARCH_ARCHITECT_M                                  0xFFE00000U
#define FPB_FP_DEVARCH_ARCHITECT_S                                          21U

// Field:    [20] PRESENT
//
// Defines that the DEVARCH register is present
#define FPB_FP_DEVARCH_PRESENT                                      0x00100000U
#define FPB_FP_DEVARCH_PRESENT_M                                    0x00100000U
#define FPB_FP_DEVARCH_PRESENT_S                                            20U

// Field: [19:16] REVISION
//
// Defines the architecture revision of the component
#define FPB_FP_DEVARCH_REVISION_W                                            4U
#define FPB_FP_DEVARCH_REVISION_M                                   0x000F0000U
#define FPB_FP_DEVARCH_REVISION_S                                           16U

// Field: [15:12] ARCHVER
//
// Defines the architecture version of the component
#define FPB_FP_DEVARCH_ARCHVER_W                                             4U
#define FPB_FP_DEVARCH_ARCHVER_M                                    0x0000F000U
#define FPB_FP_DEVARCH_ARCHVER_S                                            12U

// Field:  [11:0] ARCHPART
//
// Defines the architecture of the component
#define FPB_FP_DEVARCH_ARCHPART_W                                           12U
#define FPB_FP_DEVARCH_ARCHPART_M                                   0x00000FFFU
#define FPB_FP_DEVARCH_ARCHPART_S                                            0U

//*****************************************************************************
//
// Register: FPB_O_FP_DEVTYPE
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define FPB_FP_DEVTYPE_RES0_W                                               24U
#define FPB_FP_DEVTYPE_RES0_M                                       0xFFFFFF00U
#define FPB_FP_DEVTYPE_RES0_S                                                8U

// Field:   [7:4] SUB
//
// Component sub-type
#define FPB_FP_DEVTYPE_SUB_W                                                 4U
#define FPB_FP_DEVTYPE_SUB_M                                        0x000000F0U
#define FPB_FP_DEVTYPE_SUB_S                                                 4U

// Field:   [3:0] MAJOR
//
// Component major type
#define FPB_FP_DEVTYPE_MAJOR_W                                               4U
#define FPB_FP_DEVTYPE_MAJOR_M                                      0x0000000FU
#define FPB_FP_DEVTYPE_MAJOR_S                                               0U

//*****************************************************************************
//
// Register: FPB_O_FP_PIDR4
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define FPB_FP_PIDR4_RES0_W                                                 24U
#define FPB_FP_PIDR4_RES0_M                                         0xFFFFFF00U
#define FPB_FP_PIDR4_RES0_S                                                  8U

// Field:   [7:4] SIZE
//
// See CoreSight Architecture Specification
#define FPB_FP_PIDR4_SIZE_W                                                  4U
#define FPB_FP_PIDR4_SIZE_M                                         0x000000F0U
#define FPB_FP_PIDR4_SIZE_S                                                  4U

// Field:   [3:0] DES_2
//
// See CoreSight Architecture Specification
#define FPB_FP_PIDR4_DES_2_W                                                 4U
#define FPB_FP_PIDR4_DES_2_M                                        0x0000000FU
#define FPB_FP_PIDR4_DES_2_S                                                 0U

//*****************************************************************************
//
// Register: FPB_O_FP_PIDR5
//
//*****************************************************************************
// Field:  [31:0] RES0
//
// Reserved, RES0
#define FPB_FP_PIDR5_RES0_W                                                 32U
#define FPB_FP_PIDR5_RES0_M                                         0xFFFFFFFFU
#define FPB_FP_PIDR5_RES0_S                                                  0U

//*****************************************************************************
//
// Register: FPB_O_FP_PIDR6
//
//*****************************************************************************
// Field:  [31:0] RES0
//
// Reserved, RES0
#define FPB_FP_PIDR6_RES0_W                                                 32U
#define FPB_FP_PIDR6_RES0_M                                         0xFFFFFFFFU
#define FPB_FP_PIDR6_RES0_S                                                  0U

//*****************************************************************************
//
// Register: FPB_O_FP_PIDR7
//
//*****************************************************************************
// Field:  [31:0] RES0
//
// Reserved, RES0
#define FPB_FP_PIDR7_RES0_W                                                 32U
#define FPB_FP_PIDR7_RES0_M                                         0xFFFFFFFFU
#define FPB_FP_PIDR7_RES0_S                                                  0U

//*****************************************************************************
//
// Register: FPB_O_FP_PIDR0
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define FPB_FP_PIDR0_RES0_W                                                 24U
#define FPB_FP_PIDR0_RES0_M                                         0xFFFFFF00U
#define FPB_FP_PIDR0_RES0_S                                                  8U

// Field:   [7:0] PART_0
//
// See CoreSight Architecture Specification
#define FPB_FP_PIDR0_PART_0_W                                                8U
#define FPB_FP_PIDR0_PART_0_M                                       0x000000FFU
#define FPB_FP_PIDR0_PART_0_S                                                0U

//*****************************************************************************
//
// Register: FPB_O_FP_PIDR1
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define FPB_FP_PIDR1_RES0_W                                                 24U
#define FPB_FP_PIDR1_RES0_M                                         0xFFFFFF00U
#define FPB_FP_PIDR1_RES0_S                                                  8U

// Field:   [7:4] DES_0
//
// See CoreSight Architecture Specification
#define FPB_FP_PIDR1_DES_0_W                                                 4U
#define FPB_FP_PIDR1_DES_0_M                                        0x000000F0U
#define FPB_FP_PIDR1_DES_0_S                                                 4U

// Field:   [3:0] PART_1
//
// See CoreSight Architecture Specification
#define FPB_FP_PIDR1_PART_1_W                                                4U
#define FPB_FP_PIDR1_PART_1_M                                       0x0000000FU
#define FPB_FP_PIDR1_PART_1_S                                                0U

//*****************************************************************************
//
// Register: FPB_O_FP_PIDR2
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define FPB_FP_PIDR2_RES0_W                                                 24U
#define FPB_FP_PIDR2_RES0_M                                         0xFFFFFF00U
#define FPB_FP_PIDR2_RES0_S                                                  8U

// Field:   [7:4] REVISION
//
// See CoreSight Architecture Specification
#define FPB_FP_PIDR2_REVISION_W                                              4U
#define FPB_FP_PIDR2_REVISION_M                                     0x000000F0U
#define FPB_FP_PIDR2_REVISION_S                                              4U

// Field:     [3] JEDEC
//
// See CoreSight Architecture Specification
#define FPB_FP_PIDR2_JEDEC                                          0x00000008U
#define FPB_FP_PIDR2_JEDEC_M                                        0x00000008U
#define FPB_FP_PIDR2_JEDEC_S                                                 3U

// Field:   [2:0] DES_1
//
// See CoreSight Architecture Specification
#define FPB_FP_PIDR2_DES_1_W                                                 3U
#define FPB_FP_PIDR2_DES_1_M                                        0x00000007U
#define FPB_FP_PIDR2_DES_1_S                                                 0U

//*****************************************************************************
//
// Register: FPB_O_FP_PIDR3
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define FPB_FP_PIDR3_RES0_W                                                 24U
#define FPB_FP_PIDR3_RES0_M                                         0xFFFFFF00U
#define FPB_FP_PIDR3_RES0_S                                                  8U

// Field:   [7:4] REVAND
//
// See CoreSight Architecture Specification
#define FPB_FP_PIDR3_REVAND_W                                                4U
#define FPB_FP_PIDR3_REVAND_M                                       0x000000F0U
#define FPB_FP_PIDR3_REVAND_S                                                4U

// Field:   [3:0] CMOD
//
// See CoreSight Architecture Specification
#define FPB_FP_PIDR3_CMOD_W                                                  4U
#define FPB_FP_PIDR3_CMOD_M                                         0x0000000FU
#define FPB_FP_PIDR3_CMOD_S                                                  0U

//*****************************************************************************
//
// Register: FPB_O_FP_CIDR0
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define FPB_FP_CIDR0_RES0_W                                                 24U
#define FPB_FP_CIDR0_RES0_M                                         0xFFFFFF00U
#define FPB_FP_CIDR0_RES0_S                                                  8U

// Field:   [7:0] PRMBL_0
//
// See CoreSight Architecture Specification
#define FPB_FP_CIDR0_PRMBL_0_W                                               8U
#define FPB_FP_CIDR0_PRMBL_0_M                                      0x000000FFU
#define FPB_FP_CIDR0_PRMBL_0_S                                               0U

//*****************************************************************************
//
// Register: FPB_O_FP_CIDR1
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define FPB_FP_CIDR1_RES0_W                                                 24U
#define FPB_FP_CIDR1_RES0_M                                         0xFFFFFF00U
#define FPB_FP_CIDR1_RES0_S                                                  8U

// Field:   [7:4] CLASS
//
// See CoreSight Architecture Specification
#define FPB_FP_CIDR1_CLASS_W                                                 4U
#define FPB_FP_CIDR1_CLASS_M                                        0x000000F0U
#define FPB_FP_CIDR1_CLASS_S                                                 4U

// Field:   [3:0] PRMBL_1
//
// See CoreSight Architecture Specification
#define FPB_FP_CIDR1_PRMBL_1_W                                               4U
#define FPB_FP_CIDR1_PRMBL_1_M                                      0x0000000FU
#define FPB_FP_CIDR1_PRMBL_1_S                                               0U

//*****************************************************************************
//
// Register: FPB_O_FP_CIDR2
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define FPB_FP_CIDR2_RES0_W                                                 24U
#define FPB_FP_CIDR2_RES0_M                                         0xFFFFFF00U
#define FPB_FP_CIDR2_RES0_S                                                  8U

// Field:   [7:0] PRMBL_2
//
// See CoreSight Architecture Specification
#define FPB_FP_CIDR2_PRMBL_2_W                                               8U
#define FPB_FP_CIDR2_PRMBL_2_M                                      0x000000FFU
#define FPB_FP_CIDR2_PRMBL_2_S                                               0U

//*****************************************************************************
//
// Register: FPB_O_FP_CIDR3
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define FPB_FP_CIDR3_RES0_W                                                 24U
#define FPB_FP_CIDR3_RES0_M                                         0xFFFFFF00U
#define FPB_FP_CIDR3_RES0_S                                                  8U

// Field:   [7:0] PRMBL_3
//
// See CoreSight Architecture Specification
#define FPB_FP_CIDR3_PRMBL_3_W                                               8U
#define FPB_FP_CIDR3_PRMBL_3_M                                      0x000000FFU
#define FPB_FP_CIDR3_PRMBL_3_S                                               0U


#endif // __FPB__
