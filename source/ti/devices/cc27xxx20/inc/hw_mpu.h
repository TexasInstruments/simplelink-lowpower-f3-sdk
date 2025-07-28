/******************************************************************************
*  Filename:       hw_mpu_h
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

#ifndef __HW_MPU_H__
#define __HW_MPU_H__

//*****************************************************************************
//
// This section defines the register offsets of
// MPU component
//
//*****************************************************************************
// The MPU Type Register indicates how many regions the MPU `FTSSS supports
#define MPU_O_MPU_TYPE                                              0x00000000U

// Enables the MPU and, when the MPU is enabled, controls whether the default
// memory map is enabled as a background region for privileged accesses, and
// whether the MPU is enabled for HardFaults, NMIs, and exception handlers when
// FAULTMASK is set to 1
#define MPU_O_MPU_CTRL                                              0x00000004U

// Selects the region currently accessed by MPU_RBAR and MPU_RLAR
#define MPU_O_MPU_RNR                                               0x00000008U

// Provides indirect read and write access to the base address of the currently
// selected MPU region `FTSSS
#define MPU_O_MPU_RBAR                                              0x0000000CU

// Provides indirect read and write access to the limit address of the
// currently selected MPU region `FTSSS
#define MPU_O_MPU_RLAR                                              0x00000010U

// Provides indirect read and write access to the base address of the MPU
// region selected by MPU_RNR[7:2]:(1[1:0]) `FTSSS
#define MPU_O_MPU_RBAR_A1                                           0x00000014U

// Provides indirect read and write access to the limit address of the
// currently selected MPU region selected by MPU_RNR[7:2]:(1[1:0]) `FTSSS
#define MPU_O_MPU_RLAR_A1                                           0x00000018U

// Provides indirect read and write access to the base address of the MPU
// region selected by MPU_RNR[7:2]:(2[1:0]) `FTSSS
#define MPU_O_MPU_RBAR_A2                                           0x0000001CU

// Provides indirect read and write access to the limit address of the
// currently selected MPU region selected by MPU_RNR[7:2]:(2[1:0]) `FTSSS
#define MPU_O_MPU_RLAR_A2                                           0x00000020U

// Provides indirect read and write access to the base address of the MPU
// region selected by MPU_RNR[7:2]:(3[1:0]) `FTSSS
#define MPU_O_MPU_RBAR_A3                                           0x00000024U

// Provides indirect read and write access to the limit address of the
// currently selected MPU region selected by MPU_RNR[7:2]:(3[1:0]) `FTSSS
#define MPU_O_MPU_RLAR_A3                                           0x00000028U

// Along with MPU_MAIR1, provides the memory attribute encodings corresponding
// to the AttrIndex values
#define MPU_O_MPU_MAIR0                                             0x00000030U

// Along with MPU_MAIR0, provides the memory attribute encodings corresponding
// to the AttrIndex values
#define MPU_O_MPU_MAIR1                                             0x00000034U

//*****************************************************************************
//
// Register: MPU_O_MPU_TYPE
//
//*****************************************************************************
// Field: [31:16] RES0
//
// Reserved, RES0
#define MPU_MPU_TYPE_RES0_W                                                 16U
#define MPU_MPU_TYPE_RES0_M                                         0xFFFF0000U
#define MPU_MPU_TYPE_RES0_S                                                 16U

// Field:  [15:8] DREGION
//
// Number of regions supported by the MPU
#define MPU_MPU_TYPE_DREGION_W                                               8U
#define MPU_MPU_TYPE_DREGION_M                                      0x0000FF00U
#define MPU_MPU_TYPE_DREGION_S                                               8U

// Field:   [7:1] RES0_1
//
// Reserved, RES0
#define MPU_MPU_TYPE_RES0_1_W                                                7U
#define MPU_MPU_TYPE_RES0_1_M                                       0x000000FEU
#define MPU_MPU_TYPE_RES0_1_S                                                1U

// Field:     [0] SEPARATE
//
// Indicates support for separate instructions and data address regions
#define MPU_MPU_TYPE_SEPARATE                                       0x00000001U
#define MPU_MPU_TYPE_SEPARATE_M                                     0x00000001U
#define MPU_MPU_TYPE_SEPARATE_S                                              0U

//*****************************************************************************
//
// Register: MPU_O_MPU_CTRL
//
//*****************************************************************************
// Field:  [31:3] RES0
//
// Reserved, RES0
#define MPU_MPU_CTRL_RES0_W                                                 29U
#define MPU_MPU_CTRL_RES0_M                                         0xFFFFFFF8U
#define MPU_MPU_CTRL_RES0_S                                                  3U

// Field:     [2] PRIVDEFENA
//
// Controls whether the default memory map is enabled for privileged software
#define MPU_MPU_CTRL_PRIVDEFENA                                     0x00000004U
#define MPU_MPU_CTRL_PRIVDEFENA_M                                   0x00000004U
#define MPU_MPU_CTRL_PRIVDEFENA_S                                            2U

// Field:     [1] HFNMIENA
//
// Controls whether handlers executing with priority less than 0 access memory
// with the MPU enabled or disabled. This applies to HardFaults, NMIs, and
// exception handlers when FAULTMASK is set to 1
#define MPU_MPU_CTRL_HFNMIENA                                       0x00000002U
#define MPU_MPU_CTRL_HFNMIENA_M                                     0x00000002U
#define MPU_MPU_CTRL_HFNMIENA_S                                              1U

// Field:     [0] ENABLE
//
// Enables the MPU
#define MPU_MPU_CTRL_ENABLE                                         0x00000001U
#define MPU_MPU_CTRL_ENABLE_M                                       0x00000001U
#define MPU_MPU_CTRL_ENABLE_S                                                0U

//*****************************************************************************
//
// Register: MPU_O_MPU_RNR
//
//*****************************************************************************
// Field:  [31:4] RES0
//
// Reserved, RES0
#define MPU_MPU_RNR_RES0_W                                                  28U
#define MPU_MPU_RNR_RES0_M                                          0xFFFFFFF0U
#define MPU_MPU_RNR_RES0_S                                                   4U

// Field:     [3] RES0_3
//
// Reserved, RES0
#define MPU_MPU_RNR_RES0_3                                          0x00000008U
#define MPU_MPU_RNR_RES0_3_M                                        0x00000008U
#define MPU_MPU_RNR_RES0_3_S                                                 3U

// Field:   [2:0] REGION
//
// Indicates the memory region accessed by MPU_RBAR and MPU_RLAR
#define MPU_MPU_RNR_REGION_W                                                 3U
#define MPU_MPU_RNR_REGION_M                                        0x00000007U
#define MPU_MPU_RNR_REGION_S                                                 0U

//*****************************************************************************
//
// Register: MPU_O_MPU_RBAR
//
//*****************************************************************************
// Field:  [31:5] BASE
//
// Contains bits [31:5] of the lower inclusive limit of the selected MPU memory
// region. This value is zero extended to provide the base address to be
// checked against
#define MPU_MPU_RBAR_BASE_W                                                 27U
#define MPU_MPU_RBAR_BASE_M                                         0xFFFFFFE0U
#define MPU_MPU_RBAR_BASE_S                                                  5U

// Field:   [4:3] SH
//
// Defines the Shareability domain of this region for Normal memory
#define MPU_MPU_RBAR_SH_W                                                    2U
#define MPU_MPU_RBAR_SH_M                                           0x00000018U
#define MPU_MPU_RBAR_SH_S                                                    3U

// Field:   [2:1] AP
//
// Defines the access permissions for this region
#define MPU_MPU_RBAR_AP_W                                                    2U
#define MPU_MPU_RBAR_AP_M                                           0x00000006U
#define MPU_MPU_RBAR_AP_S                                                    1U

// Field:     [0] XN
//
// Defines whether code can be executed from this region
#define MPU_MPU_RBAR_XN                                             0x00000001U
#define MPU_MPU_RBAR_XN_M                                           0x00000001U
#define MPU_MPU_RBAR_XN_S                                                    0U

//*****************************************************************************
//
// Register: MPU_O_MPU_RLAR
//
//*****************************************************************************
// Field:  [31:5] LIMIT
//
// Contains bits [31:5] of the upper inclusive limit of the selected MPU memory
// region. This value is postfixed with 0x1F to provide the limit address to be
// checked against
#define MPU_MPU_RLAR_LIMIT_W                                                27U
#define MPU_MPU_RLAR_LIMIT_M                                        0xFFFFFFE0U
#define MPU_MPU_RLAR_LIMIT_S                                                 5U

// Field:     [4] RES0
//
// Reserved, RES0
#define MPU_MPU_RLAR_RES0                                           0x00000010U
#define MPU_MPU_RLAR_RES0_M                                         0x00000010U
#define MPU_MPU_RLAR_RES0_S                                                  4U

// Field:   [3:1] AttrIndx
//
// Associates a set of attributes in the MPU_MAIR0 and MPU_MAIR1 fields
#define MPU_MPU_RLAR_ATTRINDX_W                                              3U
#define MPU_MPU_RLAR_ATTRINDX_M                                     0x0000000EU
#define MPU_MPU_RLAR_ATTRINDX_S                                              1U

// Field:     [0] EN
//
// Region enable
#define MPU_MPU_RLAR_EN                                             0x00000001U
#define MPU_MPU_RLAR_EN_M                                           0x00000001U
#define MPU_MPU_RLAR_EN_S                                                    0U

//*****************************************************************************
//
// Register: MPU_O_MPU_RBAR_A1
//
//*****************************************************************************
// Field:  [31:5] BASE
//
// Contains bits [31:5] of the lower inclusive limit of the selected MPU memory
// region. This value is zero extended to provide the base address to be
// checked against
#define MPU_MPU_RBAR_A1_BASE_W                                              27U
#define MPU_MPU_RBAR_A1_BASE_M                                      0xFFFFFFE0U
#define MPU_MPU_RBAR_A1_BASE_S                                               5U

// Field:   [4:3] SH
//
// Defines the Shareability domain of this region for Normal memory
#define MPU_MPU_RBAR_A1_SH_W                                                 2U
#define MPU_MPU_RBAR_A1_SH_M                                        0x00000018U
#define MPU_MPU_RBAR_A1_SH_S                                                 3U

// Field:   [2:1] AP
//
// Defines the access permissions for this region
#define MPU_MPU_RBAR_A1_AP_W                                                 2U
#define MPU_MPU_RBAR_A1_AP_M                                        0x00000006U
#define MPU_MPU_RBAR_A1_AP_S                                                 1U

// Field:     [0] XN
//
// Defines whether code can be executed from this region
#define MPU_MPU_RBAR_A1_XN                                          0x00000001U
#define MPU_MPU_RBAR_A1_XN_M                                        0x00000001U
#define MPU_MPU_RBAR_A1_XN_S                                                 0U

//*****************************************************************************
//
// Register: MPU_O_MPU_RLAR_A1
//
//*****************************************************************************
// Field:  [31:5] LIMIT
//
// Contains bits [31:5] of the upper inclusive limit of the selected MPU memory
// region. This value is postfixed with 0x1F to provide the limit address to be
// checked against
#define MPU_MPU_RLAR_A1_LIMIT_W                                             27U
#define MPU_MPU_RLAR_A1_LIMIT_M                                     0xFFFFFFE0U
#define MPU_MPU_RLAR_A1_LIMIT_S                                              5U

// Field:     [4] RES0
//
// Reserved, RES0
#define MPU_MPU_RLAR_A1_RES0                                        0x00000010U
#define MPU_MPU_RLAR_A1_RES0_M                                      0x00000010U
#define MPU_MPU_RLAR_A1_RES0_S                                               4U

// Field:   [3:1] AttrIndx
//
// Associates a set of attributes in the MPU_MAIR0 and MPU_MAIR1 fields
#define MPU_MPU_RLAR_A1_ATTRINDX_W                                           3U
#define MPU_MPU_RLAR_A1_ATTRINDX_M                                  0x0000000EU
#define MPU_MPU_RLAR_A1_ATTRINDX_S                                           1U

// Field:     [0] EN
//
// Region enable
#define MPU_MPU_RLAR_A1_EN                                          0x00000001U
#define MPU_MPU_RLAR_A1_EN_M                                        0x00000001U
#define MPU_MPU_RLAR_A1_EN_S                                                 0U

//*****************************************************************************
//
// Register: MPU_O_MPU_RBAR_A2
//
//*****************************************************************************
// Field:  [31:5] BASE
//
// Contains bits [31:5] of the lower inclusive limit of the selected MPU memory
// region. This value is zero extended to provide the base address to be
// checked against
#define MPU_MPU_RBAR_A2_BASE_W                                              27U
#define MPU_MPU_RBAR_A2_BASE_M                                      0xFFFFFFE0U
#define MPU_MPU_RBAR_A2_BASE_S                                               5U

// Field:   [4:3] SH
//
// Defines the Shareability domain of this region for Normal memory
#define MPU_MPU_RBAR_A2_SH_W                                                 2U
#define MPU_MPU_RBAR_A2_SH_M                                        0x00000018U
#define MPU_MPU_RBAR_A2_SH_S                                                 3U

// Field:   [2:1] AP
//
// Defines the access permissions for this region
#define MPU_MPU_RBAR_A2_AP_W                                                 2U
#define MPU_MPU_RBAR_A2_AP_M                                        0x00000006U
#define MPU_MPU_RBAR_A2_AP_S                                                 1U

// Field:     [0] XN
//
// Defines whether code can be executed from this region
#define MPU_MPU_RBAR_A2_XN                                          0x00000001U
#define MPU_MPU_RBAR_A2_XN_M                                        0x00000001U
#define MPU_MPU_RBAR_A2_XN_S                                                 0U

//*****************************************************************************
//
// Register: MPU_O_MPU_RLAR_A2
//
//*****************************************************************************
// Field:  [31:5] LIMIT
//
// Contains bits [31:5] of the upper inclusive limit of the selected MPU memory
// region. This value is postfixed with 0x1F to provide the limit address to be
// checked against
#define MPU_MPU_RLAR_A2_LIMIT_W                                             27U
#define MPU_MPU_RLAR_A2_LIMIT_M                                     0xFFFFFFE0U
#define MPU_MPU_RLAR_A2_LIMIT_S                                              5U

// Field:     [4] RES0
//
// Reserved, RES0
#define MPU_MPU_RLAR_A2_RES0                                        0x00000010U
#define MPU_MPU_RLAR_A2_RES0_M                                      0x00000010U
#define MPU_MPU_RLAR_A2_RES0_S                                               4U

// Field:   [3:1] AttrIndx
//
// Associates a set of attributes in the MPU_MAIR0 and MPU_MAIR1 fields
#define MPU_MPU_RLAR_A2_ATTRINDX_W                                           3U
#define MPU_MPU_RLAR_A2_ATTRINDX_M                                  0x0000000EU
#define MPU_MPU_RLAR_A2_ATTRINDX_S                                           1U

// Field:     [0] EN
//
// Region enable
#define MPU_MPU_RLAR_A2_EN                                          0x00000001U
#define MPU_MPU_RLAR_A2_EN_M                                        0x00000001U
#define MPU_MPU_RLAR_A2_EN_S                                                 0U

//*****************************************************************************
//
// Register: MPU_O_MPU_RBAR_A3
//
//*****************************************************************************
// Field:  [31:5] BASE
//
// Contains bits [31:5] of the lower inclusive limit of the selected MPU memory
// region. This value is zero extended to provide the base address to be
// checked against
#define MPU_MPU_RBAR_A3_BASE_W                                              27U
#define MPU_MPU_RBAR_A3_BASE_M                                      0xFFFFFFE0U
#define MPU_MPU_RBAR_A3_BASE_S                                               5U

// Field:   [4:3] SH
//
// Defines the Shareability domain of this region for Normal memory
#define MPU_MPU_RBAR_A3_SH_W                                                 2U
#define MPU_MPU_RBAR_A3_SH_M                                        0x00000018U
#define MPU_MPU_RBAR_A3_SH_S                                                 3U

// Field:   [2:1] AP
//
// Defines the access permissions for this region
#define MPU_MPU_RBAR_A3_AP_W                                                 2U
#define MPU_MPU_RBAR_A3_AP_M                                        0x00000006U
#define MPU_MPU_RBAR_A3_AP_S                                                 1U

// Field:     [0] XN
//
// Defines whether code can be executed from this region
#define MPU_MPU_RBAR_A3_XN                                          0x00000001U
#define MPU_MPU_RBAR_A3_XN_M                                        0x00000001U
#define MPU_MPU_RBAR_A3_XN_S                                                 0U

//*****************************************************************************
//
// Register: MPU_O_MPU_RLAR_A3
//
//*****************************************************************************
// Field:  [31:5] LIMIT
//
// Contains bits [31:5] of the upper inclusive limit of the selected MPU memory
// region. This value is postfixed with 0x1F to provide the limit address to be
// checked against
#define MPU_MPU_RLAR_A3_LIMIT_W                                             27U
#define MPU_MPU_RLAR_A3_LIMIT_M                                     0xFFFFFFE0U
#define MPU_MPU_RLAR_A3_LIMIT_S                                              5U

// Field:     [4] RES0
//
// Reserved, RES0
#define MPU_MPU_RLAR_A3_RES0                                        0x00000010U
#define MPU_MPU_RLAR_A3_RES0_M                                      0x00000010U
#define MPU_MPU_RLAR_A3_RES0_S                                               4U

// Field:   [3:1] AttrIndx
//
// Associates a set of attributes in the MPU_MAIR0 and MPU_MAIR1 fields
#define MPU_MPU_RLAR_A3_ATTRINDX_W                                           3U
#define MPU_MPU_RLAR_A3_ATTRINDX_M                                  0x0000000EU
#define MPU_MPU_RLAR_A3_ATTRINDX_S                                           1U

// Field:     [0] EN
//
// Region enable
#define MPU_MPU_RLAR_A3_EN                                          0x00000001U
#define MPU_MPU_RLAR_A3_EN_M                                        0x00000001U
#define MPU_MPU_RLAR_A3_EN_S                                                 0U

//*****************************************************************************
//
// Register: MPU_O_MPU_MAIR0
//
//*****************************************************************************
// Field: [31:24] Attr3
//
// Memory attribute encoding for MPU regions with an AttrIndex of 3
#define MPU_MPU_MAIR0_ATTR3_W                                                8U
#define MPU_MPU_MAIR0_ATTR3_M                                       0xFF000000U
#define MPU_MPU_MAIR0_ATTR3_S                                               24U

// Field: [23:16] Attr2
//
// Memory attribute encoding for MPU regions with an AttrIndex of 2
#define MPU_MPU_MAIR0_ATTR2_W                                                8U
#define MPU_MPU_MAIR0_ATTR2_M                                       0x00FF0000U
#define MPU_MPU_MAIR0_ATTR2_S                                               16U

// Field:  [15:8] Attr1
//
// Memory attribute encoding for MPU regions with an AttrIndex of 1
#define MPU_MPU_MAIR0_ATTR1_W                                                8U
#define MPU_MPU_MAIR0_ATTR1_M                                       0x0000FF00U
#define MPU_MPU_MAIR0_ATTR1_S                                                8U

// Field:   [7:0] Attr0
//
// Memory attribute encoding for MPU regions with an AttrIndex of 0
#define MPU_MPU_MAIR0_ATTR0_W                                                8U
#define MPU_MPU_MAIR0_ATTR0_M                                       0x000000FFU
#define MPU_MPU_MAIR0_ATTR0_S                                                0U

//*****************************************************************************
//
// Register: MPU_O_MPU_MAIR1
//
//*****************************************************************************
// Field: [31:24] Attr7
//
// Memory attribute encoding for MPU regions with an AttrIndex of 7
#define MPU_MPU_MAIR1_ATTR7_W                                                8U
#define MPU_MPU_MAIR1_ATTR7_M                                       0xFF000000U
#define MPU_MPU_MAIR1_ATTR7_S                                               24U

// Field: [23:16] Attr6
//
// Memory attribute encoding for MPU regions with an AttrIndex of 6
#define MPU_MPU_MAIR1_ATTR6_W                                                8U
#define MPU_MPU_MAIR1_ATTR6_M                                       0x00FF0000U
#define MPU_MPU_MAIR1_ATTR6_S                                               16U

// Field:  [15:8] Attr5
//
// Memory attribute encoding for MPU regions with an AttrIndex of 5
#define MPU_MPU_MAIR1_ATTR5_W                                                8U
#define MPU_MPU_MAIR1_ATTR5_M                                       0x0000FF00U
#define MPU_MPU_MAIR1_ATTR5_S                                                8U

// Field:   [7:0] Attr4
//
// Memory attribute encoding for MPU regions with an AttrIndex of 4
#define MPU_MPU_MAIR1_ATTR4_W                                                8U
#define MPU_MPU_MAIR1_ATTR4_M                                       0x000000FFU
#define MPU_MPU_MAIR1_ATTR4_S                                                0U


#endif // __MPU__
