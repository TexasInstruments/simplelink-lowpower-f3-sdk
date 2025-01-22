/******************************************************************************
*  Filename:       hw_vims_h
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

// Flash bank 0 trim value 1.
#define VIMS_O_B1TRMC1                                              0x00000024U

// Flash bank 0 trim value 0.
#define VIMS_O_B1TRMC0                                              0x00000028U

// Flash block
#define VIMS_O_FLBLCK                                               0x00000100U

// Configuration
#define VIMS_O_CFG                                                  0x000003FCU

// Flash main region read protection confg.
#define VIMS_O_RDPRMN                                               0x00000400U

// Flash non main region read protection confg.
#define VIMS_O_RDPRNMN                                              0x00000404U

// Flash trim region read protection config.
#define VIMS_O_RDPRTRM                                              0x00000408U

// Flash engr region read protection config.
#define VIMS_O_RDPREGR                                              0x0000040CU

// Flash main region write/erase protection config.A
#define VIMS_O_WEPRA                                                0x00000410U

// Flash main region write/erase protection config.B0
#define VIMS_O_WEPRB0                                               0x00000414U

// Flash main region write/erase protection config.B1
#define VIMS_O_WEPRB1                                               0x00000418U

// Flash Aux region write/erase protection config.
#define VIMS_O_WEPRAUX                                              0x0000041CU

// Flash status
#define VIMS_O_FLBSTA                                               0x00000420U

// Cache control
#define VIMS_O_CCHCTL                                               0x00000424U

// Cache Status
#define VIMS_O_CCHSTA                                               0x00000428U

// Cache Hit Counter
#define VIMS_O_CNTHIT                                               0x00000430U

// Cache Miss Counter
#define VIMS_O_CNTMISS                                              0x00000434U

// Control register
#define VIMS_O_CTL                                                  0x000004FCU

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
// Standard IP MMR block offset. Standard IP MMRs are the set of from
// aggregated IRQ registers till DTB.
// 0: Standard IP MMRs do not exist
// 0x1-0xF: Standard IP MMRs begin at offset of (64*STDIPOFF from the base IP
// address)
#define VIMS_DESC_STDIPOFF_W                                                 4U
#define VIMS_DESC_STDIPOFF_M                                        0x0000F000U
#define VIMS_DESC_STDIPOFF_S                                                12U

// Field:  [11:8] INSTIDX
//
// IP Instance ID number. If multiple instances of IP exist in the device, this
// field can identify the instance number (0-15).
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
// Field: [28:27] NBANK
//
// Number of Flash banks
#define VIMS_DESCEX_NBANK_W                                                  2U
#define VIMS_DESCEX_NBANK_M                                         0x18000000U
#define VIMS_DESCEX_NBANK_S                                                 27U

// Field: [26:15] FLSZ
//
// FLASH size in Kilo Bytes. The total FLASH size is (FLSZ + 1) KB
#define VIMS_DESCEX_FLSZ_W                                                  12U
#define VIMS_DESCEX_FLSZ_M                                          0x07FF8000U
#define VIMS_DESCEX_FLSZ_S                                                  15U

// Field:  [14:0] ROMSZ
//
// ROM size in Kilo Bytes. The total ROM size is (ROMSZ + 1) KB
#define VIMS_DESCEX_ROMSZ_W                                                 15U
#define VIMS_DESCEX_ROMSZ_M                                         0x00007FFFU
#define VIMS_DESCEX_ROMSZ_S                                                  0U

//*****************************************************************************
//
// Register: VIMS_O_FLWS1T
//
//*****************************************************************************
// Field:   [3:0] VAL
//
// Flash read wait states for 1T accesses
// ENUMs:
// WS15                     Wait state value 15
// WS14                     Wait state value 14
// WS13                     Wait state value 13
// WS12                     Wait state value 12
// WS11                     Wait state value 11
// WS10                     Wait state value 10
// WS9                      Wait state value 9
// WS8                      Wait state value 8
// WS7                      Wait state value 7
// WS6                      Wait state value 6
// WS5                      Wait state value 5
// WS4                      Wait state value 4
// WS3                      Wait state value 3
// WS2                      Wait state value 2
// WS1                      Wait state value 1
// WS0                      Wait state value 0
#define VIMS_FLWS1T_VAL_W                                                    4U
#define VIMS_FLWS1T_VAL_M                                           0x0000000FU
#define VIMS_FLWS1T_VAL_S                                                    0U
#define VIMS_FLWS1T_VAL_WS15                                        0x0000000FU
#define VIMS_FLWS1T_VAL_WS14                                        0x0000000EU
#define VIMS_FLWS1T_VAL_WS13                                        0x0000000DU
#define VIMS_FLWS1T_VAL_WS12                                        0x0000000CU
#define VIMS_FLWS1T_VAL_WS11                                        0x0000000BU
#define VIMS_FLWS1T_VAL_WS10                                        0x0000000AU
#define VIMS_FLWS1T_VAL_WS9                                         0x00000009U
#define VIMS_FLWS1T_VAL_WS8                                         0x00000008U
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
// Field:   [3:0] VAL
//
// Flash read wait states for 2T accesses
// ENUMs:
// WS15                     Wait state value 15
// WS14                     Wait state value 14
// WS13                     Wait state value 13
// WS12                     Wait state value 12
// WS11                     Wait state value 11
// WS10                     Wait state value 10
// WS9                      Wait state value 9
// WS8                      Wait state value 8
// WS7                      Wait state value 7
// WS6                      Wait state value 6
// WS5                      Wait state value 5
// WS4                      Wait state value 4
// WS3                      Wait state value 3
// WS2                      Wait state value 2
// WS1                      Wait state value 1
// WS0                      Wait state value 0
#define VIMS_FLWS2T_VAL_W                                                    4U
#define VIMS_FLWS2T_VAL_M                                           0x0000000FU
#define VIMS_FLWS2T_VAL_S                                                    0U
#define VIMS_FLWS2T_VAL_WS15                                        0x0000000FU
#define VIMS_FLWS2T_VAL_WS14                                        0x0000000EU
#define VIMS_FLWS2T_VAL_WS13                                        0x0000000DU
#define VIMS_FLWS2T_VAL_WS12                                        0x0000000CU
#define VIMS_FLWS2T_VAL_WS11                                        0x0000000BU
#define VIMS_FLWS2T_VAL_WS10                                        0x0000000AU
#define VIMS_FLWS2T_VAL_WS9                                         0x00000009U
#define VIMS_FLWS2T_VAL_WS8                                         0x00000008U
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
// Field:  [27:0] VAL
//
// Flash bank trim value.
#define VIMS_B0TRMC1_VAL_W                                                  28U
#define VIMS_B0TRMC1_VAL_M                                          0x0FFFFFFFU
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
// Register: VIMS_O_B1TRMC1
//
//*****************************************************************************
// Field:  [27:0] VAL
//
// Flash bank trim value.
#define VIMS_B1TRMC1_VAL_W                                                  28U
#define VIMS_B1TRMC1_VAL_M                                          0x0FFFFFFFU
#define VIMS_B1TRMC1_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VIMS_O_B1TRMC0
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Flash bank trim value.
#define VIMS_B1TRMC0_VAL_W                                                  32U
#define VIMS_B1TRMC0_VAL_M                                          0xFFFFFFFFU
#define VIMS_B1TRMC0_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VIMS_O_FLBLCK
//
//*****************************************************************************
// Field:     [0] ACCESS
//
// Flash Access
// ENUMs:
// BLOCK                    Block
// ALLOW                    Allow
#define VIMS_FLBLCK_ACCESS                                          0x00000001U
#define VIMS_FLBLCK_ACCESS_M                                        0x00000001U
#define VIMS_FLBLCK_ACCESS_S                                                 0U
#define VIMS_FLBLCK_ACCESS_BLOCK                                    0x00000001U
#define VIMS_FLBLCK_ACCESS_ALLOW                                    0x00000000U

//*****************************************************************************
//
// Register: VIMS_O_CFG
//
//*****************************************************************************
// Field:  [10:8] HSMSZ
//
// HSM Size. Number of 32KB blocks allocated to HSM
// ENUMs:
// SIZE_224                 224KB
// SIZE_192                 192KB
// SIZE_160                 160KB
// SIZE_128                 128KB
// SIZE_96                  96KB
// SIZE_64                  64KB
// SIZE_32                  32KB
// SIZE_0                   No space allocated to HSM
#define VIMS_CFG_HSMSZ_W                                                     3U
#define VIMS_CFG_HSMSZ_M                                            0x00000700U
#define VIMS_CFG_HSMSZ_S                                                     8U
#define VIMS_CFG_HSMSZ_SIZE_224                                     0x00000700U
#define VIMS_CFG_HSMSZ_SIZE_192                                     0x00000600U
#define VIMS_CFG_HSMSZ_SIZE_160                                     0x00000500U
#define VIMS_CFG_HSMSZ_SIZE_128                                     0x00000400U
#define VIMS_CFG_HSMSZ_SIZE_96                                      0x00000300U
#define VIMS_CFG_HSMSZ_SIZE_64                                      0x00000200U
#define VIMS_CFG_HSMSZ_SIZE_32                                      0x00000100U
#define VIMS_CFG_HSMSZ_SIZE_0                                       0x00000000U

// Field:     [4] SPLMODE
//
// Split Mode. The logical address space is split into two equal regions
// ENUMs:
// EN                       Enable
// DIS                      Disable
#define VIMS_CFG_SPLMODE                                            0x00000010U
#define VIMS_CFG_SPLMODE_M                                          0x00000010U
#define VIMS_CFG_SPLMODE_S                                                   4U
#define VIMS_CFG_SPLMODE_EN                                         0x00000010U
#define VIMS_CFG_SPLMODE_DIS                                        0x00000000U

// Field:     [3] RDPRROM
//
// ROM Read Protection Disable
// ENUMs:
// DIS                      Disable
// EN                       Enable
#define VIMS_CFG_RDPRROM                                            0x00000008U
#define VIMS_CFG_RDPRROM_M                                          0x00000008U
#define VIMS_CFG_RDPRROM_S                                                   3U
#define VIMS_CFG_RDPRROM_DIS                                        0x00000008U
#define VIMS_CFG_RDPRROM_EN                                         0x00000000U

// Field:     [2] ATTEST
//
// This bit is used to enable flash test mode.
// ENUMs:
// EN                       Enable
// DIS                      Disable
#define VIMS_CFG_ATTEST                                             0x00000004U
#define VIMS_CFG_ATTEST_M                                           0x00000004U
#define VIMS_CFG_ATTEST_S                                                    2U
#define VIMS_CFG_ATTEST_EN                                          0x00000004U
#define VIMS_CFG_ATTEST_DIS                                         0x00000000U

// Field:     [1] TRMVLID
//
// TRIM Valid. This bit indicates if flash charge pump and bank trim values are
// as applicable.
// ENUMs:
// VLID                     Valid
// INVLID                   Invalid
#define VIMS_CFG_TRMVLID                                            0x00000002U
#define VIMS_CFG_TRMVLID_M                                          0x00000002U
#define VIMS_CFG_TRMVLID_S                                                   1U
#define VIMS_CFG_TRMVLID_VLID                                       0x00000002U
#define VIMS_CFG_TRMVLID_INVLID                                     0x00000000U

// Field:     [0] LOCK
//
// Lock. Lock VIMS configuration. This bit is sticky '0' and when 0 write
// protects following registers
// FLWS1T
// FLWS2T
// PTRMC0
// B0TRMC1
// B0TRMC0
// B1TRMC1
// B1TRMC0
// FLBLCK
// CFG
// ENUMs:
// DIS                      Configuration registers are open for writing
// EN                       Configuration registers are locked for writing
#define VIMS_CFG_LOCK                                               0x00000001U
#define VIMS_CFG_LOCK_M                                             0x00000001U
#define VIMS_CFG_LOCK_S                                                      0U
#define VIMS_CFG_LOCK_DIS                                           0x00000001U
#define VIMS_CFG_LOCK_EN                                            0x00000000U

//*****************************************************************************
//
// Register: VIMS_O_RDPRMN
//
//*****************************************************************************
// Field:   [3:0] VAL
//
// Protection configuration value.
//  Undefined values will read protect whole protectable region
// ENUMs:
// PROT_0KB                 Protection is Disabled
// PROT_2KB                 Protect First 2KB
// PROT_4KB                 Protect First 4KB
// PROT_6KB                 Protect First 6KB
// PROT_8KB                 Protect First 8KB
// PROT_10KB                Protect First 10KB
// PROT_12KB                Protect First 12KB
// PROT_14KB                Protect First 14KB
// PROT_16KB                Protect First 16KB
#define VIMS_RDPRMN_VAL_W                                                    4U
#define VIMS_RDPRMN_VAL_M                                           0x0000000FU
#define VIMS_RDPRMN_VAL_S                                                    0U
#define VIMS_RDPRMN_VAL_PROT_0KB                                    0x0000000FU
#define VIMS_RDPRMN_VAL_PROT_2KB                                    0x0000000EU
#define VIMS_RDPRMN_VAL_PROT_4KB                                    0x0000000DU
#define VIMS_RDPRMN_VAL_PROT_6KB                                    0x0000000CU
#define VIMS_RDPRMN_VAL_PROT_8KB                                    0x0000000BU
#define VIMS_RDPRMN_VAL_PROT_10KB                                   0x0000000AU
#define VIMS_RDPRMN_VAL_PROT_12KB                                   0x00000009U
#define VIMS_RDPRMN_VAL_PROT_14KB                                   0x00000008U
#define VIMS_RDPRMN_VAL_PROT_16KB                                   0x00000007U

//*****************************************************************************
//
// Register: VIMS_O_RDPRNMN
//
//*****************************************************************************
// Field:     [7] VLOG
//
// VLOG read protection configuration. This field is sticky 0
#define VIMS_RDPRNMN_VLOG                                           0x00000080U
#define VIMS_RDPRNMN_VLOG_M                                         0x00000080U
#define VIMS_RDPRNMN_VLOG_S                                                  7U

// Field:   [5:0] CCFG
//
// CCFG read protection configuration. Last 512 bytes of CCFG can be protected
// with granularity of 16Bytes. This field is sticky 0
#define VIMS_RDPRNMN_CCFG_W                                                  6U
#define VIMS_RDPRNMN_CCFG_M                                         0x0000003FU
#define VIMS_RDPRNMN_CCFG_S                                                  0U

//*****************************************************************************
//
// Register: VIMS_O_RDPRTRM
//
//*****************************************************************************
// Field:     [7] SCFG
//
// SCFG read protection configuration. This field is sticky 0
#define VIMS_RDPRTRM_SCFG                                           0x00000080U
#define VIMS_RDPRTRM_SCFG_M                                         0x00000080U
#define VIMS_RDPRTRM_SCFG_S                                                  7U

// Field:   [5:0] FCFG
//
// FCFG read protection configuration. Last 512 bytes of CCFG can be protected
// with granularity of 16Bytes. This register is sticky 0
#define VIMS_RDPRTRM_FCFG_W                                                  6U
#define VIMS_RDPRTRM_FCFG_M                                         0x0000003FU
#define VIMS_RDPRTRM_FCFG_S                                                  0U

//*****************************************************************************
//
// Register: VIMS_O_RDPREGR
//
//*****************************************************************************
// Field:     [0] ENGR
//
// ENGR read protection configuration.
#define VIMS_RDPREGR_ENGR                                           0x00000001U
#define VIMS_RDPREGR_ENGR_M                                         0x00000001U
#define VIMS_RDPREGR_ENGR_S                                                  0U

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
// Register: VIMS_O_WEPRB0
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Flash write/erase protection configuration value.
#define VIMS_WEPRB0_VAL_W                                                   32U
#define VIMS_WEPRB0_VAL_M                                           0xFFFFFFFFU
#define VIMS_WEPRB0_VAL_S                                                    0U

//*****************************************************************************
//
// Register: VIMS_O_WEPRB1
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Flash write/erase protection configuration value.
#define VIMS_WEPRB1_VAL_W                                                   32U
#define VIMS_WEPRB1_VAL_M                                           0xFFFFFFFFU
#define VIMS_WEPRB1_VAL_S                                                    0U

//*****************************************************************************
//
// Register: VIMS_O_WEPRAUX
//
//*****************************************************************************
// Field:     [4] VLOG
//
// Write/Erase Protection for VLOG
#define VIMS_WEPRAUX_VLOG                                           0x00000010U
#define VIMS_WEPRAUX_VLOG_M                                         0x00000010U
#define VIMS_WEPRAUX_VLOG_S                                                  4U

// Field:     [3] SCFG
//
// Write/Erase Protection for SCFG
#define VIMS_WEPRAUX_SCFG                                           0x00000008U
#define VIMS_WEPRAUX_SCFG_M                                         0x00000008U
#define VIMS_WEPRAUX_SCFG_S                                                  3U

// Field:     [2] ENGR
//
// Write/Erase Protection for ENGR
#define VIMS_WEPRAUX_ENGR                                           0x00000004U
#define VIMS_WEPRAUX_ENGR_M                                         0x00000004U
#define VIMS_WEPRAUX_ENGR_S                                                  2U

// Field:     [1] FCFG
//
// Write/Erase Protection for FCFG
#define VIMS_WEPRAUX_FCFG                                           0x00000002U
#define VIMS_WEPRAUX_FCFG_M                                         0x00000002U
#define VIMS_WEPRAUX_FCFG_S                                                  1U

// Field:     [0] CCFG
//
// Write/Erase Protection for CCFG
#define VIMS_WEPRAUX_CCFG                                           0x00000001U
#define VIMS_WEPRAUX_CCFG_M                                         0x00000001U
#define VIMS_WEPRAUX_CCFG_S                                                  0U

//*****************************************************************************
//
// Register: VIMS_O_FLBSTA
//
//*****************************************************************************
// Field:     [5] B1BSY
//
// Bank 1 Busy. This bit indicates if flash Bank 1 is busy.
// ENUMs:
// BUSY                     Busy
// IDLE                     Idle
#define VIMS_FLBSTA_B1BSY                                           0x00000020U
#define VIMS_FLBSTA_B1BSY_M                                         0x00000020U
#define VIMS_FLBSTA_B1BSY_S                                                  5U
#define VIMS_FLBSTA_B1BSY_BUSY                                      0x00000020U
#define VIMS_FLBSTA_B1BSY_IDLE                                      0x00000000U

// Field:     [4] B0BSY
//
// Bank 0 Busy. This bit indicates if flash Bank 0 is busy.
// ENUMs:
// BUSY                     Busy
// IDLE                     Idle
#define VIMS_FLBSTA_B0BSY                                           0x00000010U
#define VIMS_FLBSTA_B0BSY_M                                         0x00000010U
#define VIMS_FLBSTA_B0BSY_S                                                  4U
#define VIMS_FLBSTA_B0BSY_BUSY                                      0x00000010U
#define VIMS_FLBSTA_B0BSY_IDLE                                      0x00000000U

// Field:     [3] PARERR
//
// Parity Error. This bit indicates parity error on write/erase & read
// protection MMRs. This bit is sticky when set to 1 by hardware.
// ENUMs:
// ERR                      Error
// NOERR                    No Error
#define VIMS_FLBSTA_PARERR                                          0x00000008U
#define VIMS_FLBSTA_PARERR_M                                        0x00000008U
#define VIMS_FLBSTA_PARERR_S                                                 3U
#define VIMS_FLBSTA_PARERR_ERR                                      0x00000008U
#define VIMS_FLBSTA_PARERR_NOERR                                    0x00000000U

// Field:     [2] BUSY
//
// This bit indicates if flash is busy.
// ENUMs:
// BUSY                     Busy
// IDLE                     Idle
#define VIMS_FLBSTA_BUSY                                            0x00000004U
#define VIMS_FLBSTA_BUSY_M                                          0x00000004U
#define VIMS_FLBSTA_BUSY_S                                                   2U
#define VIMS_FLBSTA_BUSY_BUSY                                       0x00000004U
#define VIMS_FLBSTA_BUSY_IDLE                                       0x00000000U

// Field:     [1] FL2TRDY
//
// Flash 2T Ready. This bit indicates if flash is ready in 2T mode.
// ENUMs:
// RDY                      Ready
// NOTRDY                   Not Ready
#define VIMS_FLBSTA_FL2TRDY                                         0x00000002U
#define VIMS_FLBSTA_FL2TRDY_M                                       0x00000002U
#define VIMS_FLBSTA_FL2TRDY_S                                                1U
#define VIMS_FLBSTA_FL2TRDY_RDY                                     0x00000002U
#define VIMS_FLBSTA_FL2TRDY_NOTRDY                                  0x00000000U

// Field:     [0] FL1TRDY
//
// Flash 1T Ready. This bit indicates if flash is ready in 1T mode.
// ENUMs:
// RDY                      Ready
// NOTRDY                   Not Ready
#define VIMS_FLBSTA_FL1TRDY                                         0x00000001U
#define VIMS_FLBSTA_FL1TRDY_M                                       0x00000001U
#define VIMS_FLBSTA_FL1TRDY_S                                                0U
#define VIMS_FLBSTA_FL1TRDY_RDY                                     0x00000001U
#define VIMS_FLBSTA_FL1TRDY_NOTRDY                                  0x00000000U

//*****************************************************************************
//
// Register: VIMS_O_CCHCTL
//
//*****************************************************************************
// Field:    [16] HCHFLUSH
//
// Cache Flush for HSM Cache. This bit is used to flush the cache on HSM bus
// Interface. This bit is self clearing
// ENUMs:
// TRG                      Trigger Flush
#define VIMS_CCHCTL_HCHFLUSH                                        0x00010000U
#define VIMS_CCHCTL_HCHFLUSH_M                                      0x00010000U
#define VIMS_CCHCTL_HCHFLUSH_S                                              16U
#define VIMS_CCHCTL_HCHFLUSH_TRG                                    0x00010000U

// Field:     [8] CNTCLR
//
// Cache Hit/Miss Count Clear. Writing 0 to this bit has no affect
// ENUMs:
// CLR                      Clear Hit/Miss Counter
#define VIMS_CCHCTL_CNTCLR                                          0x00000100U
#define VIMS_CCHCTL_CNTCLR_M                                        0x00000100U
#define VIMS_CCHCTL_CNTCLR_S                                                 8U
#define VIMS_CCHCTL_CNTCLR_CLR                                      0x00000100U

// Field:     [7] CNTEN
//
// Cache Hit/Miss Count Enable.
// ENUMs:
// EN                       Enable
// DIS                      Disable
#define VIMS_CCHCTL_CNTEN                                           0x00000080U
#define VIMS_CCHCTL_CNTEN_M                                         0x00000080U
#define VIMS_CCHCTL_CNTEN_S                                                  7U
#define VIMS_CCHCTL_CNTEN_EN                                        0x00000080U
#define VIMS_CCHCTL_CNTEN_DIS                                       0x00000000U

// Field:     [6] RPOLICY
//
// Replacement policy for Cache.
// ENUMs:
// SEQ                      Sequencial Replacement Policy
// RND                      Random Replacement Policy
#define VIMS_CCHCTL_RPOLICY                                         0x00000040U
#define VIMS_CCHCTL_RPOLICY_M                                       0x00000040U
#define VIMS_CCHCTL_RPOLICY_S                                                6U
#define VIMS_CCHCTL_RPOLICY_SEQ                                     0x00000040U
#define VIMS_CCHCTL_RPOLICY_RND                                     0x00000000U

// Field:     [5] LINFLUSH
//
// Line Buffer Flush. This bit is used to flush the Line buffer on system bus
// Interface. This bit is self clearing. Writing 0 to this bit has no affect
// ENUMs:
// TRG                      Trigger Flush
#define VIMS_CCHCTL_LINFLUSH                                        0x00000020U
#define VIMS_CCHCTL_LINFLUSH_M                                      0x00000020U
#define VIMS_CCHCTL_LINFLUSH_S                                               5U
#define VIMS_CCHCTL_LINFLUSH_TRG                                    0x00000020U

// Field:     [4] LINEN
//
// Line Buffer Enable. This bit is used to enable the Line buffer on system bus
// Interface. Disabling the Line buffer will flush it
// ENUMs:
// EN                       Enable
// DIS                      Disable
#define VIMS_CCHCTL_LINEN                                           0x00000010U
#define VIMS_CCHCTL_LINEN_M                                         0x00000010U
#define VIMS_CCHCTL_LINEN_S                                                  4U
#define VIMS_CCHCTL_LINEN_EN                                        0x00000010U
#define VIMS_CCHCTL_LINEN_DIS                                       0x00000000U

// Field:     [3] CCHFLUSH
//
// Cache Flush. This bit is used to flush the cache on code bus Interface. This
// bit is self clearing. Writing 0 to this bit has no affect
// ENUMs:
// TRG                      Trigger Flush
#define VIMS_CCHCTL_CCHFLUSH                                        0x00000008U
#define VIMS_CCHCTL_CCHFLUSH_M                                      0x00000008U
#define VIMS_CCHCTL_CCHFLUSH_S                                               3U
#define VIMS_CCHCTL_CCHFLUSH_TRG                                    0x00000008U

// Field:     [0] CCHEN
//
// Cache Enable. This bit is used to enable the cache. Disabling the cache will
// flush it
// ENUMs:
// EN                       Enable
// DIS                      Disable
#define VIMS_CCHCTL_CCHEN                                           0x00000001U
#define VIMS_CCHCTL_CCHEN_M                                         0x00000001U
#define VIMS_CCHCTL_CCHEN_S                                                  0U
#define VIMS_CCHCTL_CCHEN_EN                                        0x00000001U
#define VIMS_CCHCTL_CCHEN_DIS                                       0x00000000U

//*****************************************************************************
//
// Register: VIMS_O_CCHSTA
//
//*****************************************************************************
// Field:     [1] BUSY
//
// Busy. This bit indicate Cache is changing state
// ENUMs:
// BUSY                     Busy
// NBUSY                    Not Busy
#define VIMS_CCHSTA_BUSY                                            0x00000002U
#define VIMS_CCHSTA_BUSY_M                                          0x00000002U
#define VIMS_CCHSTA_BUSY_S                                                   1U
#define VIMS_CCHSTA_BUSY_BUSY                                       0x00000002U
#define VIMS_CCHSTA_BUSY_NBUSY                                      0x00000000U

// Field:     [0] EN
//
// Enabled. This bit indicate whether cache is enabled or disabled
// ENUMs:
// EN                       Enabled
// DIS                      Disabled
#define VIMS_CCHSTA_EN                                              0x00000001U
#define VIMS_CCHSTA_EN_M                                            0x00000001U
#define VIMS_CCHSTA_EN_S                                                     0U
#define VIMS_CCHSTA_EN_EN                                           0x00000001U
#define VIMS_CCHSTA_EN_DIS                                          0x00000000U

//*****************************************************************************
//
// Register: VIMS_O_CNTHIT
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Hit Counter Value. This counter is cleared by writing CCHCTL.CNTCLR
#define VIMS_CNTHIT_VAL_W                                                   32U
#define VIMS_CNTHIT_VAL_M                                           0xFFFFFFFFU
#define VIMS_CNTHIT_VAL_S                                                    0U

//*****************************************************************************
//
// Register: VIMS_O_CNTMISS
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Miss Counter Value. This counter is cleared by writing CCHCTL.CNTCLR
#define VIMS_CNTMISS_VAL_W                                                  32U
#define VIMS_CNTMISS_VAL_M                                          0xFFFFFFFFU
#define VIMS_CNTMISS_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VIMS_O_CTL
//
//*****************************************************************************
// Field:    [31] SWAPLOCK
//
// Swap Lock. If set, CTL.SWAP bit is blocked for writing. This bit is sticky 1
// ENUMs:
// LOCK                     Locked
// UNLOCK                   Unlocked
#define VIMS_CTL_SWAPLOCK                                           0x80000000U
#define VIMS_CTL_SWAPLOCK_M                                         0x80000000U
#define VIMS_CTL_SWAPLOCK_S                                                 31U
#define VIMS_CTL_SWAPLOCK_LOCK                                      0x80000000U
#define VIMS_CTL_SWAPLOCK_UNLOCK                                    0x00000000U

// Field:     [0] SWAP
//
// Swaps the logical to physical bank mapping. This bit has no effect if
// CTL.SPLMODE is 0 and is writable ony when CTL.SWAPLOCK is 0
// ENUMs:
// SWAPED                   Bank is swapped
// NOSWAP                   Bank is not swapped
#define VIMS_CTL_SWAP                                               0x00000001U
#define VIMS_CTL_SWAP_M                                             0x00000001U
#define VIMS_CTL_SWAP_S                                                      0U
#define VIMS_CTL_SWAP_SWAPED                                        0x00000001U
#define VIMS_CTL_SWAP_NOSWAP                                        0x00000000U

//*****************************************************************************
//
// Register: VIMS_O_DTB
//
//*****************************************************************************
// Field:   [3:0] SEL
//
// Digital test bus selection mux control.
// Non-zero select values output a 16 bit selected group of signals per value.
// ENUMs:
// DIS                      DTB output from peripheral is 0x0
#define VIMS_DTB_SEL_W                                                       4U
#define VIMS_DTB_SEL_M                                              0x0000000FU
#define VIMS_DTB_SEL_S                                                       0U
#define VIMS_DTB_SEL_DIS                                            0x00000000U


#endif // __VIMS__
