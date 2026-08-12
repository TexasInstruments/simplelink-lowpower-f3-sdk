/******************************************************************************
*  Filename:       hw_vims_h
******************************************************************************
*  Copyright (c) 2021 Texas Instruments Incorporated. All rights reserved.
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

#ifndef __HW_VIMS_H__
#define __HW_VIMS_H__

//*****************************************************************************
//
// This section defines the register offsets of
// VIMS component
//
//*****************************************************************************
// Module Description
#define VIMS_O_DESC                                                 0x00000000U

// Extended Module Description
#define VIMS_O_DESCEX                                               0x00000004U

// Waitstate for 1T mode.
#define VIMS_O_FLWS1T                                               0x00000008U

// Waitstate for 2T mode.
#define VIMS_O_FLWS2T                                               0x0000000CU

// Flash charge pump trim value.
#define VIMS_O_PTRMC0                                               0x00000018U

// Flash bank 0 trim value 1.
#define VIMS_O_B0TRMC1                                              0x0000001CU

// Flash bank 0 trim value 0.
#define VIMS_O_B0TRMC0                                              0x00000020U

// Flash block
#define VIMS_O_FLBLCK                                               0x00000100U

// Configuration
#define VIMS_O_CFG                                                  0x000003FCU

// Flash main region read protection configuration
#define VIMS_O_RDPRMN                                               0x00000400U

// Flash non main region read protection configuration
#define VIMS_O_RDPRNMN                                              0x00000404U

// Flash trim region read protection configuration
#define VIMS_O_RDPRTRM                                              0x00000408U

// Flash engr region read protection configuration
#define VIMS_O_RDPREGR                                              0x0000040CU

// Flash main region write/erase protection configuration 1
#define VIMS_O_WEPRA                                                0x00000410U

// Flash main region write/erase protection configuration 2
#define VIMS_O_WEPRB                                                0x00000414U

// Flash write/erase protection configuration for other regions
#define VIMS_O_WEPRAUX                                              0x0000041CU

// Flash status
#define VIMS_O_FLBSTAT                                              0x00000420U

// Cache control
#define VIMS_O_CCHCTRL                                              0x00000424U

// Digital Test Bus
#define VIMS_O_DTB                                                  0x00000800U

//*****************************************************************************
//
// Register: VIMS_O_DESC
//
//*****************************************************************************
// Field: [31:16] MODID
//
// Module identification contains a unique peripheral identification number.
#define VIMS_DESC_MODID_W                                                   16U
#define VIMS_DESC_MODID_M                                           0xFFFF0000U
#define VIMS_DESC_MODID_S                                                   16U

// Field: [15:12] STDIPOFF
//
// Standard IP registers offset. Value 0 indicates Standard IP registers are
// not present. Any other value between 1 to 15 indicates standard IP registers
// start from address offset 64 * STDIPOFF from base address.
#define VIMS_DESC_STDIPOFF_W                                                 4U
#define VIMS_DESC_STDIPOFF_M                                        0x0000F000U
#define VIMS_DESC_STDIPOFF_S                                                12U

// Field:  [11:8] INSTIDX
//
// Instance Index within the device. This will be a parameter to the RTL for
// modules that can have multiple instances.
#define VIMS_DESC_INSTIDX_W                                                  4U
#define VIMS_DESC_INSTIDX_M                                         0x00000F00U
#define VIMS_DESC_INSTIDX_S                                                  8U

// Field:   [7:4] MAJREV
//
// Major revision of IP
#define VIMS_DESC_MAJREV_W                                                   4U
#define VIMS_DESC_MAJREV_M                                          0x000000F0U
#define VIMS_DESC_MAJREV_S                                                   4U

// Field:   [3:0] MINREV
//
// Minor revision of IP
#define VIMS_DESC_MINREV_W                                                   4U
#define VIMS_DESC_MINREV_M                                          0x0000000FU
#define VIMS_DESC_MINREV_S                                                   0U

//*****************************************************************************
//
// Register: VIMS_O_DESCEX
//
//*****************************************************************************
// Field:    [27] NBANK
//
// Provides the FLASH Bank count
#define VIMS_DESCEX_NBANK                                           0x08000000U
#define VIMS_DESCEX_NBANK_M                                         0x08000000U
#define VIMS_DESCEX_NBANK_S                                                 27U

// Field: [26:15] FLSZ
//
// This provides the total FLASH size in Kilo Bytes. The total FLASH size is
// (FLSZ + 1)KB
#define VIMS_DESCEX_FLSZ_W                                                  12U
#define VIMS_DESCEX_FLSZ_M                                          0x07FF8000U
#define VIMS_DESCEX_FLSZ_S                                                  15U

// Field:  [14:0] ROMSZ
//
// Provides the size of ROM in Bytes.
#define VIMS_DESCEX_ROMSZ_W                                                 15U
#define VIMS_DESCEX_ROMSZ_M                                         0x00007FFFU
#define VIMS_DESCEX_ROMSZ_S                                                  0U

//*****************************************************************************
//
// Register: VIMS_O_FLWS1T
//
//*****************************************************************************
// Field:   [2:0] VAL
//
// Specifies the waitstate value.
// ENUMs:
// WS7                      Wait state value 7
// WS6                      Wait state value 6
// WS5                      Wait state value 5
// WS4                      Wait state value 4
// WS3                      Wait state value 3
// WS2                      Wait state value 2
// WS1                      Wait state value 1
// WS0                      Wait state value 0
#define VIMS_FLWS1T_VAL_W                                                    3U
#define VIMS_FLWS1T_VAL_M                                           0x00000007U
#define VIMS_FLWS1T_VAL_S                                                    0U
#define VIMS_FLWS1T_VAL_WS7                                         0x00000007U
#define VIMS_FLWS1T_VAL_WS6                                         0x00000006U
#define VIMS_FLWS1T_VAL_WS5                                         0x00000005U
#define VIMS_FLWS1T_VAL_WS4                                         0x00000004U
#define VIMS_FLWS1T_VAL_WS3                                         0x00000003U
#define VIMS_FLWS1T_VAL_WS2                                         0x00000002U
#define VIMS_FLWS1T_VAL_WS1                                         0x00000001U
#define VIMS_FLWS1T_VAL_WS0                                         0x00000000U

//*****************************************************************************
//
// Register: VIMS_O_FLWS2T
//
//*****************************************************************************
// Field:   [2:0] VAL
//
// Specifies the waitstate value.
// ENUMs:
// WS7                      Wait state value 7
// WS6                      Wait state value 6
// WS5                      Wait state value 5
// WS4                      Wait state value 4
// WS3                      Wait state value 3
// WS2                      Wait state value 2
// WS1                      Wait state value 1
// WS0                      Wait state value 0
#define VIMS_FLWS2T_VAL_W                                                    3U
#define VIMS_FLWS2T_VAL_M                                           0x00000007U
#define VIMS_FLWS2T_VAL_S                                                    0U
#define VIMS_FLWS2T_VAL_WS7                                         0x00000007U
#define VIMS_FLWS2T_VAL_WS6                                         0x00000006U
#define VIMS_FLWS2T_VAL_WS5                                         0x00000005U
#define VIMS_FLWS2T_VAL_WS4                                         0x00000004U
#define VIMS_FLWS2T_VAL_WS3                                         0x00000003U
#define VIMS_FLWS2T_VAL_WS2                                         0x00000002U
#define VIMS_FLWS2T_VAL_WS1                                         0x00000001U
#define VIMS_FLWS2T_VAL_WS0                                         0x00000000U

//*****************************************************************************
//
// Register: VIMS_O_PTRMC0
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Flash charge pump trim value.
#define VIMS_PTRMC0_VAL_W                                                   32U
#define VIMS_PTRMC0_VAL_M                                           0xFFFFFFFFU
#define VIMS_PTRMC0_VAL_S                                                    0U

//*****************************************************************************
//
// Register: VIMS_O_B0TRMC1
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Flash bank trim value.
#define VIMS_B0TRMC1_VAL_W                                                  32U
#define VIMS_B0TRMC1_VAL_M                                          0xFFFFFFFFU
#define VIMS_B0TRMC1_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VIMS_O_B0TRMC0
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Flash bank trim value.
#define VIMS_B0TRMC0_VAL_W                                                  32U
#define VIMS_B0TRMC0_VAL_M                                          0xFFFFFFFFU
#define VIMS_B0TRMC0_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VIMS_O_FLBLCK
//
//*****************************************************************************
// Field:     [0] VAL
//
// Used to allow or block flash operation.
// ENUMs:
// BLOCK                    Block
// ALLOW                    Allow
#define VIMS_FLBLCK_VAL                                             0x00000001U
#define VIMS_FLBLCK_VAL_M                                           0x00000001U
#define VIMS_FLBLCK_VAL_S                                                    0U
#define VIMS_FLBLCK_VAL_BLOCK                                       0x00000001U
#define VIMS_FLBLCK_VAL_ALLOW                                       0x00000000U

//*****************************************************************************
//
// Register: VIMS_O_CFG
//
//*****************************************************************************
// Field:     [2] ATTEST
//
// This bit is used to enable flash test mode.
#define VIMS_CFG_ATTEST                                             0x00000004U
#define VIMS_CFG_ATTEST_M                                           0x00000004U
#define VIMS_CFG_ATTEST_S                                                    2U

// Field:     [1] TRMVLID
//
// This bit indicates if flash charge pump and bank trim values are valid.
#define VIMS_CFG_TRMVLID                                            0x00000002U
#define VIMS_CFG_TRMVLID_M                                          0x00000002U
#define VIMS_CFG_TRMVLID_S                                                   1U

// Field:     [0] WEPRTRM
//
// This bit is used to write protect flash 1T & 2T waitstate registers, flash
// charge pump and bank trim registers, TRMVLID and ATTEST configuration
// registers. This register is sticky when written with value 0.
// ENUMs:
// ALLOW                    Allow
// RESTRICT                 Block
#define VIMS_CFG_WEPRTRM                                            0x00000001U
#define VIMS_CFG_WEPRTRM_M                                          0x00000001U
#define VIMS_CFG_WEPRTRM_S                                                   0U
#define VIMS_CFG_WEPRTRM_ALLOW                                      0x00000001U
#define VIMS_CFG_WEPRTRM_RESTRICT                                   0x00000000U

//*****************************************************************************
//
// Register: VIMS_O_RDPRMN
//
//*****************************************************************************
// Field:   [2:0] VAL
//
// Flash read protection configuration value.
#define VIMS_RDPRMN_VAL_W                                                    3U
#define VIMS_RDPRMN_VAL_M                                           0x00000007U
#define VIMS_RDPRMN_VAL_S                                                    0U

//*****************************************************************************
//
// Register: VIMS_O_RDPRNMN
//
//*****************************************************************************
// Field:   [5:0] VAL
//
// Flash read protection configuration value.
#define VIMS_RDPRNMN_VAL_W                                                   6U
#define VIMS_RDPRNMN_VAL_M                                          0x0000003FU
#define VIMS_RDPRNMN_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VIMS_O_RDPRTRM
//
//*****************************************************************************
// Field:   [5:0] VAL
//
// Flash read protection configuration value.
#define VIMS_RDPRTRM_VAL_W                                                   6U
#define VIMS_RDPRTRM_VAL_M                                          0x0000003FU
#define VIMS_RDPRTRM_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VIMS_O_RDPREGR
//
//*****************************************************************************
// Field:     [0] VAL
//
// Flash read protection configuration value.
#define VIMS_RDPREGR_VAL                                            0x00000001U
#define VIMS_RDPREGR_VAL_M                                          0x00000001U
#define VIMS_RDPREGR_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VIMS_O_WEPRA
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Flash write/erase protection configuration value.
#define VIMS_WEPRA_VAL_W                                                    32U
#define VIMS_WEPRA_VAL_M                                            0xFFFFFFFFU
#define VIMS_WEPRA_VAL_S                                                     0U

//*****************************************************************************
//
// Register: VIMS_O_WEPRB
//
//*****************************************************************************
// Field:  [11:0] VAL
//
// Flash write/erase protection configuration value.
#define VIMS_WEPRB_VAL_W                                                    12U
#define VIMS_WEPRB_VAL_M                                            0x00000FFFU
#define VIMS_WEPRB_VAL_S                                                     0U

//*****************************************************************************
//
// Register: VIMS_O_WEPRAUX
//
//*****************************************************************************
// Field:     [2] WEPREGR
//
// Flash engr region write/erase protection configuration value.
#define VIMS_WEPRAUX_WEPREGR                                        0x00000004U
#define VIMS_WEPRAUX_WEPREGR_M                                      0x00000004U
#define VIMS_WEPRAUX_WEPREGR_S                                               2U

// Field:     [1] WEPRTRM
//
// Flash trim region write/erase protection configuration value.
#define VIMS_WEPRAUX_WEPRTRM                                        0x00000002U
#define VIMS_WEPRAUX_WEPRTRM_M                                      0x00000002U
#define VIMS_WEPRAUX_WEPRTRM_S                                               1U

// Field:     [0] WEPRNMN
//
// Flash non main region write/erase protection configuration value.
#define VIMS_WEPRAUX_WEPRNMN                                        0x00000001U
#define VIMS_WEPRAUX_WEPRNMN_M                                      0x00000001U
#define VIMS_WEPRAUX_WEPRNMN_S                                               0U

//*****************************************************************************
//
// Register: VIMS_O_FLBSTAT
//
//*****************************************************************************
// Field:     [3] PARERR
//
// This bit indicates parity error on write/erase & read protection MMRs. This
// bit is sticky when set to 1 by hardware.
// ENUMs:
// ERROR                    Error
// NOERROR                  No Error
#define VIMS_FLBSTAT_PARERR                                         0x00000008U
#define VIMS_FLBSTAT_PARERR_M                                       0x00000008U
#define VIMS_FLBSTAT_PARERR_S                                                3U
#define VIMS_FLBSTAT_PARERR_ERROR                                   0x00000008U
#define VIMS_FLBSTAT_PARERR_NOERROR                                 0x00000000U

// Field:     [2] B0BSY
//
// This bit indicates if flash is busy.
// ENUMs:
// BUSY                     Busy
// IDLE                     Idle
#define VIMS_FLBSTAT_B0BSY                                          0x00000004U
#define VIMS_FLBSTAT_B0BSY_M                                        0x00000004U
#define VIMS_FLBSTAT_B0BSY_S                                                 2U
#define VIMS_FLBSTAT_B0BSY_BUSY                                     0x00000004U
#define VIMS_FLBSTAT_B0BSY_IDLE                                     0x00000000U

// Field:     [1] B2TRDY
//
// This bit indicates if flash is ready in 2T mode.
// ENUMs:
// READY                    Ready
// NOTREADY                 Not Ready
#define VIMS_FLBSTAT_B2TRDY                                         0x00000002U
#define VIMS_FLBSTAT_B2TRDY_M                                       0x00000002U
#define VIMS_FLBSTAT_B2TRDY_S                                                1U
#define VIMS_FLBSTAT_B2TRDY_READY                                   0x00000002U
#define VIMS_FLBSTAT_B2TRDY_NOTREADY                                0x00000000U

// Field:     [0] B1TRDY
//
// This bit indicates if flash is ready in 1T mode.
// ENUMs:
// READY                    Ready
// NOTREADY                 Not Ready
#define VIMS_FLBSTAT_B1TRDY                                         0x00000001U
#define VIMS_FLBSTAT_B1TRDY_M                                       0x00000001U
#define VIMS_FLBSTAT_B1TRDY_S                                                0U
#define VIMS_FLBSTAT_B1TRDY_READY                                   0x00000001U
#define VIMS_FLBSTAT_B1TRDY_NOTREADY                                0x00000000U

//*****************************************************************************
//
// Register: VIMS_O_CCHCTRL
//
//*****************************************************************************
// Field:     [2] CCHMPEN
//
// This bit is used to enable the micropredictor unit.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define VIMS_CCHCTRL_CCHMPEN                                        0x00000004U
#define VIMS_CCHCTRL_CCHMPEN_M                                      0x00000004U
#define VIMS_CCHCTRL_CCHMPEN_S                                               2U
#define VIMS_CCHCTRL_CCHMPEN_ENABLE                                 0x00000004U
#define VIMS_CCHCTRL_CCHMPEN_DISABLE                                0x00000000U

// Field:     [1] CCHPFEN
//
// This bit is used to enable the prefetch unit.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define VIMS_CCHCTRL_CCHPFEN                                        0x00000002U
#define VIMS_CCHCTRL_CCHPFEN_M                                      0x00000002U
#define VIMS_CCHCTRL_CCHPFEN_S                                               1U
#define VIMS_CCHCTRL_CCHPFEN_ENABLE                                 0x00000002U
#define VIMS_CCHCTRL_CCHPFEN_DISABLE                                0x00000000U

// Field:     [0] CCHEN
//
// This bit is used to enable the cache.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define VIMS_CCHCTRL_CCHEN                                          0x00000001U
#define VIMS_CCHCTRL_CCHEN_M                                        0x00000001U
#define VIMS_CCHCTRL_CCHEN_S                                                 0U
#define VIMS_CCHCTRL_CCHEN_ENABLE                                   0x00000001U
#define VIMS_CCHCTRL_CCHEN_DISABLE                                  0x00000000U

//*****************************************************************************
//
// Register: VIMS_O_DTB
//
//*****************************************************************************
// Field:   [3:0] SEL
//
// DTB MUX select pin value
// ENUMs:
// GRP15                    Selects test group 15
// GRP14                    Selects test group 14
// GRP13                    Selects test group 13
// GRP12                    Selects test group 12
// GRP11                    Selects test group 11
// GRP10                    Selects test group 10
// GRP9                     Selects test group 9
// GRP8                     Selects test group 8
// GRP7                     Selects test group 7
// GRP6                     Selects test group 6
// GRP5                     Selects test group 5
// GRP4                     Selects test group 4
// GRP3                     Selects test group 3
// GRP2                     Selects test group 2
// GRP1                     Selects test group 1
// DISABLE                  DTB output from peripheral is 0x0.
#define VIMS_DTB_SEL_W                                                       4U
#define VIMS_DTB_SEL_M                                              0x0000000FU
#define VIMS_DTB_SEL_S                                                       0U
#define VIMS_DTB_SEL_GRP15                                          0x0000000FU
#define VIMS_DTB_SEL_GRP14                                          0x0000000EU
#define VIMS_DTB_SEL_GRP13                                          0x0000000DU
#define VIMS_DTB_SEL_GRP12                                          0x0000000CU
#define VIMS_DTB_SEL_GRP11                                          0x0000000BU
#define VIMS_DTB_SEL_GRP10                                          0x0000000AU
#define VIMS_DTB_SEL_GRP9                                           0x00000009U
#define VIMS_DTB_SEL_GRP8                                           0x00000008U
#define VIMS_DTB_SEL_GRP7                                           0x00000007U
#define VIMS_DTB_SEL_GRP6                                           0x00000006U
#define VIMS_DTB_SEL_GRP5                                           0x00000005U
#define VIMS_DTB_SEL_GRP4                                           0x00000004U
#define VIMS_DTB_SEL_GRP3                                           0x00000003U
#define VIMS_DTB_SEL_GRP2                                           0x00000002U
#define VIMS_DTB_SEL_GRP1                                           0x00000001U
#define VIMS_DTB_SEL_DISABLE                                        0x00000000U


#endif // __VIMS__
