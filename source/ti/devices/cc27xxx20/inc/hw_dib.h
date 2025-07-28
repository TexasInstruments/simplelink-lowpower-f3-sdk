/******************************************************************************
*  Filename:       hw_dib_h
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

#ifndef __HW_DIB_H__
#define __HW_DIB_H__

//*****************************************************************************
//
// This section defines the register offsets of
// DIB component
//
//*****************************************************************************
// Provides CoreSight discovery information for the SCS
#define DIB_O_DLAR                                                  0x00000000U

// Provides CoreSight discovery information for the SCS
#define DIB_O_DLSR                                                  0x00000004U

// Provides CoreSight discovery information for the SCS
#define DIB_O_DAUTHSTATUS                                           0x00000008U

// Provides CoreSight discovery information for the SCS
#define DIB_O_DDEVARCH                                              0x0000000CU

// Provides CoreSight discovery information for the SCS
#define DIB_O_DDEVTYPE                                              0x0000001CU

// Provides CoreSight discovery information for the SCS
#define DIB_O_DPIDR4                                                0x00000020U

// Provides CoreSight discovery information for the SCS
#define DIB_O_DPIDR5                                                0x00000024U

// Provides CoreSight discovery information for the SCS
#define DIB_O_DPIDR6                                                0x00000028U

// Provides CoreSight discovery information for the SCS
#define DIB_O_DPIDR7                                                0x0000002CU

// Provides CoreSight discovery information for the SCS
#define DIB_O_DPIDR0                                                0x00000030U

// Provides CoreSight discovery information for the SCS
#define DIB_O_DPIDR1                                                0x00000034U

// Provides CoreSight discovery information for the SCS
#define DIB_O_DPIDR2                                                0x00000038U

// Provides CoreSight discovery information for the SCS
#define DIB_O_DPIDR3                                                0x0000003CU

// Provides CoreSight discovery information for the SCS
#define DIB_O_DCIDR0                                                0x00000040U

// Provides CoreSight discovery information for the SCS
#define DIB_O_DCIDR1                                                0x00000044U

// Provides CoreSight discovery information for the SCS
#define DIB_O_DCIDR2                                                0x00000048U

// Provides CoreSight discovery information for the SCS
#define DIB_O_DCIDR3                                                0x0000004CU

//*****************************************************************************
//
// Register: DIB_O_DLAR
//
//*****************************************************************************
// Field:     [0] KEY
//
// Indicates whether Non-secure invasive debug is allowed
#define DIB_DLAR_KEY                                                0x00000001U
#define DIB_DLAR_KEY_M                                              0x00000001U
#define DIB_DLAR_KEY_S                                                       0U

//*****************************************************************************
//
// Register: DIB_O_DLSR
//
//*****************************************************************************
// Field:     [2] nTT
//
// Indicates whether Secure invasive debug is implemented and allowed
#define DIB_DLSR_NTT                                                0x00000004U
#define DIB_DLSR_NTT_M                                              0x00000004U
#define DIB_DLSR_NTT_S                                                       2U

// Field:     [1] SLK
//
// Indicates whether Non-secure non-invasive debug is allowed
#define DIB_DLSR_SLK                                                0x00000002U
#define DIB_DLSR_SLK_M                                              0x00000002U
#define DIB_DLSR_SLK_S                                                       1U

// Field:     [0] SLI
//
// Indicates whether Non-secure invasive debug is allowed
#define DIB_DLSR_SLI                                                0x00000001U
#define DIB_DLSR_SLI_M                                              0x00000001U
#define DIB_DLSR_SLI_S                                                       0U

//*****************************************************************************
//
// Register: DIB_O_DAUTHSTATUS
//
//*****************************************************************************
// Field:     [3] SNID
//
// Indicates whether Secure non-invasive debug is implemented and allowed
#define DIB_DAUTHSTATUS_SNID                                        0x00000008U
#define DIB_DAUTHSTATUS_SNID_M                                      0x00000008U
#define DIB_DAUTHSTATUS_SNID_S                                               3U

// Field:     [2] SID
//
// Indicates whether Secure invasive debug is implemented and allowed
#define DIB_DAUTHSTATUS_SID                                         0x00000004U
#define DIB_DAUTHSTATUS_SID_M                                       0x00000004U
#define DIB_DAUTHSTATUS_SID_S                                                2U

// Field:     [1] NSNID
//
// Indicates whether Non-secure non-invasive debug is allowed
#define DIB_DAUTHSTATUS_NSNID                                       0x00000002U
#define DIB_DAUTHSTATUS_NSNID_M                                     0x00000002U
#define DIB_DAUTHSTATUS_NSNID_S                                              1U

// Field:     [0] NSID
//
// Indicates whether Non-secure invasive debug is allowed
#define DIB_DAUTHSTATUS_NSID                                        0x00000001U
#define DIB_DAUTHSTATUS_NSID_M                                      0x00000001U
#define DIB_DAUTHSTATUS_NSID_S                                               0U

//*****************************************************************************
//
// Register: DIB_O_DDEVARCH
//
//*****************************************************************************
// Field: [31:21] ARCHITECT
//
// Defines the architect of the component. Bits [31:28] are the JEP106
// continuation code (JEP106 bank ID, minus 1) and bits [27:21] are the JEP106
// ID code.
#define DIB_DDEVARCH_ARCHITECT_W                                            11U
#define DIB_DDEVARCH_ARCHITECT_M                                    0xFFE00000U
#define DIB_DDEVARCH_ARCHITECT_S                                            21U

// Field:    [20] PRESENT
//
// Defines that the DEVARCH register is present
#define DIB_DDEVARCH_PRESENT                                        0x00100000U
#define DIB_DDEVARCH_PRESENT_M                                      0x00100000U
#define DIB_DDEVARCH_PRESENT_S                                              20U

// Field: [19:16] REVISION
//
// Defines the architecture revision of the component
#define DIB_DDEVARCH_REVISION_W                                              4U
#define DIB_DDEVARCH_REVISION_M                                     0x000F0000U
#define DIB_DDEVARCH_REVISION_S                                             16U

// Field: [15:12] ARCHVER
//
// Defines the architecture version of the component
#define DIB_DDEVARCH_ARCHVER_W                                               4U
#define DIB_DDEVARCH_ARCHVER_M                                      0x0000F000U
#define DIB_DDEVARCH_ARCHVER_S                                              12U

// Field:  [11:0] ARCHPART
//
// Defines the architecture of the component
#define DIB_DDEVARCH_ARCHPART_W                                             12U
#define DIB_DDEVARCH_ARCHPART_M                                     0x00000FFFU
#define DIB_DDEVARCH_ARCHPART_S                                              0U

//*****************************************************************************
//
// Register: DIB_O_DDEVTYPE
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define DIB_DDEVTYPE_RES0_W                                                 24U
#define DIB_DDEVTYPE_RES0_M                                         0xFFFFFF00U
#define DIB_DDEVTYPE_RES0_S                                                  8U

// Field:   [7:4] SUB
//
// Component sub-type
#define DIB_DDEVTYPE_SUB_W                                                   4U
#define DIB_DDEVTYPE_SUB_M                                          0x000000F0U
#define DIB_DDEVTYPE_SUB_S                                                   4U

// Field:   [3:0] MAJOR
//
// CoreSight major type
#define DIB_DDEVTYPE_MAJOR_W                                                 4U
#define DIB_DDEVTYPE_MAJOR_M                                        0x0000000FU
#define DIB_DDEVTYPE_MAJOR_S                                                 0U

//*****************************************************************************
//
// Register: DIB_O_DPIDR4
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define DIB_DPIDR4_RES0_W                                                   24U
#define DIB_DPIDR4_RES0_M                                           0xFFFFFF00U
#define DIB_DPIDR4_RES0_S                                                    8U

// Field:   [7:4] SIZE
//
// See CoreSight Architecture Specification
#define DIB_DPIDR4_SIZE_W                                                    4U
#define DIB_DPIDR4_SIZE_M                                           0x000000F0U
#define DIB_DPIDR4_SIZE_S                                                    4U

// Field:   [3:0] DES_2
//
// See CoreSight Architecture Specification
#define DIB_DPIDR4_DES_2_W                                                   4U
#define DIB_DPIDR4_DES_2_M                                          0x0000000FU
#define DIB_DPIDR4_DES_2_S                                                   0U

//*****************************************************************************
//
// Register: DIB_O_DPIDR5
//
//*****************************************************************************
// Field:  [31:0] RES0
//
// Reserved, RES0
#define DIB_DPIDR5_RES0_W                                                   32U
#define DIB_DPIDR5_RES0_M                                           0xFFFFFFFFU
#define DIB_DPIDR5_RES0_S                                                    0U

//*****************************************************************************
//
// Register: DIB_O_DPIDR6
//
//*****************************************************************************
// Field:  [31:0] RES0
//
// Reserved, RES0
#define DIB_DPIDR6_RES0_W                                                   32U
#define DIB_DPIDR6_RES0_M                                           0xFFFFFFFFU
#define DIB_DPIDR6_RES0_S                                                    0U

//*****************************************************************************
//
// Register: DIB_O_DPIDR7
//
//*****************************************************************************
// Field:  [31:0] RES0
//
// Reserved, RES0
#define DIB_DPIDR7_RES0_W                                                   32U
#define DIB_DPIDR7_RES0_M                                           0xFFFFFFFFU
#define DIB_DPIDR7_RES0_S                                                    0U

//*****************************************************************************
//
// Register: DIB_O_DPIDR0
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define DIB_DPIDR0_RES0_W                                                   24U
#define DIB_DPIDR0_RES0_M                                           0xFFFFFF00U
#define DIB_DPIDR0_RES0_S                                                    8U

// Field:   [7:0] PART_0
//
// See CoreSight Architecture Specification
#define DIB_DPIDR0_PART_0_W                                                  8U
#define DIB_DPIDR0_PART_0_M                                         0x000000FFU
#define DIB_DPIDR0_PART_0_S                                                  0U

//*****************************************************************************
//
// Register: DIB_O_DPIDR1
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define DIB_DPIDR1_RES0_W                                                   24U
#define DIB_DPIDR1_RES0_M                                           0xFFFFFF00U
#define DIB_DPIDR1_RES0_S                                                    8U

// Field:   [7:4] DES_0
//
// See CoreSight Architecture Specification
#define DIB_DPIDR1_DES_0_W                                                   4U
#define DIB_DPIDR1_DES_0_M                                          0x000000F0U
#define DIB_DPIDR1_DES_0_S                                                   4U

// Field:   [3:0] PART_1
//
// See CoreSight Architecture Specification
#define DIB_DPIDR1_PART_1_W                                                  4U
#define DIB_DPIDR1_PART_1_M                                         0x0000000FU
#define DIB_DPIDR1_PART_1_S                                                  0U

//*****************************************************************************
//
// Register: DIB_O_DPIDR2
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define DIB_DPIDR2_RES0_W                                                   24U
#define DIB_DPIDR2_RES0_M                                           0xFFFFFF00U
#define DIB_DPIDR2_RES0_S                                                    8U

// Field:   [7:4] REVISION
//
// See CoreSight Architecture Specification
#define DIB_DPIDR2_REVISION_W                                                4U
#define DIB_DPIDR2_REVISION_M                                       0x000000F0U
#define DIB_DPIDR2_REVISION_S                                                4U

// Field:     [3] JEDEC
//
// See CoreSight Architecture Specification
#define DIB_DPIDR2_JEDEC                                            0x00000008U
#define DIB_DPIDR2_JEDEC_M                                          0x00000008U
#define DIB_DPIDR2_JEDEC_S                                                   3U

// Field:   [2:0] DES_1
//
// See CoreSight Architecture Specification
#define DIB_DPIDR2_DES_1_W                                                   3U
#define DIB_DPIDR2_DES_1_M                                          0x00000007U
#define DIB_DPIDR2_DES_1_S                                                   0U

//*****************************************************************************
//
// Register: DIB_O_DPIDR3
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define DIB_DPIDR3_RES0_W                                                   24U
#define DIB_DPIDR3_RES0_M                                           0xFFFFFF00U
#define DIB_DPIDR3_RES0_S                                                    8U

// Field:   [7:4] REVAND
//
// See CoreSight Architecture Specification
#define DIB_DPIDR3_REVAND_W                                                  4U
#define DIB_DPIDR3_REVAND_M                                         0x000000F0U
#define DIB_DPIDR3_REVAND_S                                                  4U

// Field:   [3:0] CMOD
//
// See CoreSight Architecture Specification
#define DIB_DPIDR3_CMOD_W                                                    4U
#define DIB_DPIDR3_CMOD_M                                           0x0000000FU
#define DIB_DPIDR3_CMOD_S                                                    0U

//*****************************************************************************
//
// Register: DIB_O_DCIDR0
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define DIB_DCIDR0_RES0_W                                                   24U
#define DIB_DCIDR0_RES0_M                                           0xFFFFFF00U
#define DIB_DCIDR0_RES0_S                                                    8U

// Field:   [7:0] PRMBL_0
//
// See CoreSight Architecture Specification
#define DIB_DCIDR0_PRMBL_0_W                                                 8U
#define DIB_DCIDR0_PRMBL_0_M                                        0x000000FFU
#define DIB_DCIDR0_PRMBL_0_S                                                 0U

//*****************************************************************************
//
// Register: DIB_O_DCIDR1
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define DIB_DCIDR1_RES0_W                                                   24U
#define DIB_DCIDR1_RES0_M                                           0xFFFFFF00U
#define DIB_DCIDR1_RES0_S                                                    8U

// Field:   [7:4] CLASS
//
// See CoreSight Architecture Specification
#define DIB_DCIDR1_CLASS_W                                                   4U
#define DIB_DCIDR1_CLASS_M                                          0x000000F0U
#define DIB_DCIDR1_CLASS_S                                                   4U

// Field:   [3:0] PRMBL_1
//
// See CoreSight Architecture Specification
#define DIB_DCIDR1_PRMBL_1_W                                                 4U
#define DIB_DCIDR1_PRMBL_1_M                                        0x0000000FU
#define DIB_DCIDR1_PRMBL_1_S                                                 0U

//*****************************************************************************
//
// Register: DIB_O_DCIDR2
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define DIB_DCIDR2_RES0_W                                                   24U
#define DIB_DCIDR2_RES0_M                                           0xFFFFFF00U
#define DIB_DCIDR2_RES0_S                                                    8U

// Field:   [7:0] PRMBL_2
//
// See CoreSight Architecture Specification
#define DIB_DCIDR2_PRMBL_2_W                                                 8U
#define DIB_DCIDR2_PRMBL_2_M                                        0x000000FFU
#define DIB_DCIDR2_PRMBL_2_S                                                 0U

//*****************************************************************************
//
// Register: DIB_O_DCIDR3
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define DIB_DCIDR3_RES0_W                                                   24U
#define DIB_DCIDR3_RES0_M                                           0xFFFFFF00U
#define DIB_DCIDR3_RES0_S                                                    8U

// Field:   [7:0] PRMBL_3
//
// See CoreSight Architecture Specification
#define DIB_DCIDR3_PRMBL_3_W                                                 8U
#define DIB_DCIDR3_PRMBL_3_M                                        0x000000FFU
#define DIB_DCIDR3_PRMBL_3_S                                                 0U


#endif // __DIB__
