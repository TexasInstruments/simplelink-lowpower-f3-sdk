/******************************************************************************
*  Filename:       hw_sys0.h
******************************************************************************
*  Copyright (c) 2025 Texas Instruments Incorporated. All rights reserved.
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

#ifndef __HW_SYS0_H__
#define __HW_SYS0_H__

//*****************************************************************************
//
// This section defines the register offsets of
// SYS0 component
//
//*****************************************************************************
// Description Register
#define SYS0_O_DESC                                                 0x00000000U

// Mutable section Unlock
#define SYS0_O_MUNLOCK                                              0x0000000CU

// ATEST Configuration
#define SYS0_O_ATESTCFG                                             0x00000100U

// TSENSE Configuration
#define SYS0_O_TSENSCFG                                             0x00000108U

// LPCMP configuration
#define SYS0_O_LPCMPCFG                                             0x0000010CU

// This register is used to configure the VGM module.
#define SYS0_O_VGMCFG                                               0x00000110U

// This register is used to trim and debug VGM module.
#define SYS0_O_VGMTRIMDBG                                           0x00000114U

// Device ID
#define SYS0_O_DEVICEID                                             0x000003FCU

// Debug Authentication.
#define SYS0_O_DBGAUTH                                              0x00000410U

// Part ID
#define SYS0_O_PARTID                                               0x000007F8U

// This register is programmed by boot code with device life cycle information
#define SYS0_O_LIFECYC                                              0x000007FCU

// Internal. Only to be used through TI provided API.
#define SYS0_O_TMUTE0                                               0x00000800U

// Internal. Only to be used through TI provided API.
#define SYS0_O_TMUTE1                                               0x00000804U

// TMUTE2 trim Register
#define SYS0_O_TMUTE2                                               0x00000808U

// Internal. Only to be used through TI provided API.
#define SYS0_O_TMUTE3                                               0x0000080CU

// TMUTE4 trim Register
#define SYS0_O_TMUTE4                                               0x00000810U

// Internal. Only to be used through TI provided API.
#define SYS0_O_TMUTE5                                               0x00000814U

// Config
#define SYS0_O_CFG                                                  0x00000C00U

// Preamble
#define SYS0_O_PRE                                                  0x00000C01U

// Sync Word Byte 0
#define SYS0_O_SWB0                                                 0x00000C02U

// Sync Word Byte 1
#define SYS0_O_SWB1                                                 0x00000C03U

// Sync Word Byte 2
#define SYS0_O_SWB2                                                 0x00000C04U

// Sync Word Byte 3
#define SYS0_O_SWB3                                                 0x00000C05U

// Correlator
#define SYS0_O_CORR                                                 0x00000C06U

// Time 0
#define SYS0_O_TIM0                                                 0x00000C07U

// Time 1
#define SYS0_O_TIM1                                                 0x00000C08U

// Time 2
#define SYS0_O_TIM2                                                 0x00000C09U

// Trim 0
#define SYS0_O_TRIM0                                                0x00000C0AU

// Trim 1
#define SYS0_O_TRIM1                                                0x00000C0BU

// Frontend Configuration
#define SYS0_O_FE                                                   0x00000C0CU

// Analog spare register
#define SYS0_O_ANASPARE                                             0x00000C0DU

// Spare register
#define SYS0_O_SPARE                                                0x00000C0EU

// Read-only register
#define SYS0_O_READONLY                                             0x00000C0FU

// Set for ADI Byte Offsets 0 to 3
#define SYS0_O_SET03                                                0x00000C10U

// Set for ADI Byte Offsets 4 to 7
#define SYS0_O_SET47                                                0x00000C14U

// Set for ADI Byte Offsets 8 to 11
#define SYS0_O_SET811                                               0x00000C18U

// Set for ADI Byte Offsets 12 to 15
#define SYS0_O_SET1215                                              0x00000C1CU

// Clear for ADI Byte Offsets 0 to 3
#define SYS0_O_CLR03                                                0x00000C20U

// Clear for ADI Byte Offsets 4 to 7
#define SYS0_O_CLR47                                                0x00000C24U

// Clear for ADI Byte Offsets 8 to 11
#define SYS0_O_CLR811                                               0x00000C28U

// Clear for ADI Byte Offsets 12 to 15
#define SYS0_O_CLR1215                                              0x00000C2CU

// Synchronization
#define SYS0_O_SYNC                                                 0x00000C30U

// ADI Configuration
#define SYS0_O_ADICFG                                               0x00000C38U

// Masked Access (4m/4d), Byte Offsets 0 and 1.
#define SYS0_O_MASK4B01                                             0x00000C40U

// Masked Access (4m/4d), Byte Offsets 2 and 3.
#define SYS0_O_MASK4B23                                             0x00000C44U

// Masked Access (4m/4d), Byte Offsets 4 and 5.
#define SYS0_O_MASK4B45                                             0x00000C48U

// Masked Access (4m/4d), Byte Offsets 6 and 7.
#define SYS0_O_MASK4B67                                             0x00000C4CU

// Masked Access (4m/4d), Byte Offsets 8 and 9.
#define SYS0_O_MASK4B89                                             0x00000C50U

// Masked Access (4m/4d), Byte Offsets 10 and 11.
#define SYS0_O_MASK4B1011                                           0x00000C54U

// Masked Access (4m/4d), Byte Offsets 12 and 13.
#define SYS0_O_MASK4B1213                                           0x00000C58U

// Masked Access (4m/4d), Byte Offsets 14 and 15.
#define SYS0_O_MASK4B1415                                           0x00000C5CU

// Masked Access (8m/8d), Byte Offsets 0 and 1.
#define SYS0_O_MASK8B01                                             0x00000C60U

// Masked Access (8m/8d), Byte Offsets 2 and 3.
#define SYS0_O_MASK8B23                                             0x00000C64U

// Masked Access (8m/8d), Byte Offsets 4 and 5.
#define SYS0_O_MASK8B45                                             0x00000C68U

// Masked Access (8m/8d), Byte Offsets 6 and 7.
#define SYS0_O_MASK8B67                                             0x00000C6CU

// Masked Access (8m/8d), Byte Offsets 8 and 9.
#define SYS0_O_MASK8B89                                             0x00000C70U

// Masked Access (8m/8d), Byte Offsets 10 and 11.
#define SYS0_O_MASK8B1011                                           0x00000C74U

// Masked Access (8m/8d), Byte Offsets 12 and 13.
#define SYS0_O_MASK8B1213                                           0x00000C78U

// Masked Access (8m/8d), Byte Offsets 14 and 15.
#define SYS0_O_MASK8B1415                                           0x00000C7CU

// Masked Access (16m/16d), Byte Offsets 0 and 1.
#define SYS0_O_MASK16B01                                            0x00000C80U

// Masked Access (16m/16d), Byte Offsets 2 and 3.
#define SYS0_O_MASK16B23                                            0x00000C84U

// Masked Access (16m/16d), Byte Offsets 4 and 5.
#define SYS0_O_MASK16B45                                            0x00000C88U

// Masked Access (16m/16d), Byte Offsets 6 and 7.
#define SYS0_O_MASK16B67                                            0x00000C8CU

// Masked Access (16m/16d), Byte Offsets 8 and 9.
#define SYS0_O_MASK16B89                                            0x00000C90U

// Masked Access (16m/16d), Byte Offsets 10 and 11.
#define SYS0_O_MASK16B1011                                          0x00000C94U

// Masked Access (16m/16d), Byte Offsets 12 and 13.
#define SYS0_O_MASK16B1213                                          0x00000C98U

// Masked Access (16m/16d), Byte Offsets 14 and 15.
#define SYS0_O_MASK16B1415                                          0x00000C9CU

//*****************************************************************************
//
// Register: SYS0_O_DESC
//
//*****************************************************************************
// Field: [31:16] MODID
//
// Module Identifier is used to uniquely identify this IP
#define SYS0_DESC_MODID_W                                                   16U
#define SYS0_DESC_MODID_M                                           0xFFFF0000U
#define SYS0_DESC_MODID_S                                                   16U

// Field: [15:12] STDIPOFF
//
// Standard IP MMR block offset. Standard IP MMRs are the set from aggregated
// IRQ registers till DTB.
//
// 0: Standard IP MMRs do not exist
//
// 0x1-0xF: Standard IP MMRs begin at offset of (64*STDIPOFF from the base IP
// address)
#define SYS0_DESC_STDIPOFF_W                                                 4U
#define SYS0_DESC_STDIPOFF_M                                        0x0000F000U
#define SYS0_DESC_STDIPOFF_S                                                12U

// Field:  [11:8] INSTIDX
//
// IP Instance ID number. If multiple instances of IP exist in the device, this
// field can identify the instance number (0-15).
#define SYS0_DESC_INSTIDX_W                                                  4U
#define SYS0_DESC_INSTIDX_M                                         0x00000F00U
#define SYS0_DESC_INSTIDX_S                                                  8U

// Field:   [7:4] MAJREV
//
// Major revision of IP (0-15).
#define SYS0_DESC_MAJREV_W                                                   4U
#define SYS0_DESC_MAJREV_M                                          0x000000F0U
#define SYS0_DESC_MAJREV_S                                                   4U

// Field:   [3:0] MINREV
//
// Minor revision of IP (0-15).
#define SYS0_DESC_MINREV_W                                                   4U
#define SYS0_DESC_MINREV_M                                          0x0000000FU
#define SYS0_DESC_MINREV_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_MUNLOCK
//
//*****************************************************************************
// Field:  [31:0] KEY
//
// Write the unlock key 0xC5AF6927 to temporarily unlock registers in mutable
// section. The lock is set automatically if no write accesses, to the mutable
// section, are detected for consecutive 32 CLKULL (24MHz) clock cycles.
// Writing any value other than that, the unlock key will immediately lock the
// mutable register space for write access.
// ENUMs:
// UNLOCK                   Unlock registers in the mutable section
// LOCK                     Lock registers in the mutable section
#define SYS0_MUNLOCK_KEY_W                                                  32U
#define SYS0_MUNLOCK_KEY_M                                          0xFFFFFFFFU
#define SYS0_MUNLOCK_KEY_S                                                   0U
#define SYS0_MUNLOCK_KEY_UNLOCK                                     0xC5AF6927U
#define SYS0_MUNLOCK_KEY_LOCK                                       0x00000000U

//*****************************************************************************
//
// Register: SYS0_O_ATESTCFG
//
//*****************************************************************************
// Field: [31:24] KEY
//
// Key must be written with value 0x5A for successful write to ATESTCFG and to
// unlock register state.
// Write with any value other than 0x5A to KEY will be ignored and register
// content is not updated.
// It is recommended to write this register with incorrect KEY to lock back
// register state after necessary ATESTCFG updates are done.
// Read value of KEY is 0x0.
#define SYS0_ATESTCFG_KEY_W                                                  8U
#define SYS0_ATESTCFG_KEY_M                                         0xFF000000U
#define SYS0_ATESTCFG_KEY_S                                                 24U

// Field:     [8] VSEL
//
// Selects supply for ATEST switches.
// ENUMs:
// VDDS                     Selects VDDS
// VDDBST                   Selects VDDBOOST
#define SYS0_ATESTCFG_VSEL                                          0x00000100U
#define SYS0_ATESTCFG_VSEL_M                                        0x00000100U
#define SYS0_ATESTCFG_VSEL_S                                                 8U
#define SYS0_ATESTCFG_VSEL_VDDS                                     0x00000100U
#define SYS0_ATESTCFG_VSEL_VDDBST                                   0x00000000U

// Field:     [7] VA2VA1
//
// Enables isolation switch between VA_ATEST_A1 and VA_PAD_A1.
// ENUMs:
// CLOSE                    Switch is closed
// OPEN                     Switch is open
#define SYS0_ATESTCFG_VA2VA1                                        0x00000080U
#define SYS0_ATESTCFG_VA2VA1_M                                      0x00000080U
#define SYS0_ATESTCFG_VA2VA1_S                                               7U
#define SYS0_ATESTCFG_VA2VA1_CLOSE                                  0x00000080U
#define SYS0_ATESTCFG_VA2VA1_OPEN                                   0x00000000U

// Field:     [6] VA2VA0
//
// Enables isolation switch between VA_ATEST_A0 and VA_PAD_A0.
// ENUMs:
// CLOSE                    Switch is closed
// OPEN                     Switch is open
#define SYS0_ATESTCFG_VA2VA0                                        0x00000040U
#define SYS0_ATESTCFG_VA2VA0_M                                      0x00000040U
#define SYS0_ATESTCFG_VA2VA0_S                                               6U
#define SYS0_ATESTCFG_VA2VA0_CLOSE                                  0x00000040U
#define SYS0_ATESTCFG_VA2VA0_OPEN                                   0x00000000U

// Field:     [5] VR2VA1
//
// Enables isolation switch between VR_ATEST_A1 and VA_ATEST_A1.
// ENUMs:
// CLOSE                    Switch is closed
// OPEN                     Switch is open
#define SYS0_ATESTCFG_VR2VA1                                        0x00000020U
#define SYS0_ATESTCFG_VR2VA1_M                                      0x00000020U
#define SYS0_ATESTCFG_VR2VA1_S                                               5U
#define SYS0_ATESTCFG_VR2VA1_CLOSE                                  0x00000020U
#define SYS0_ATESTCFG_VR2VA1_OPEN                                   0x00000000U

// Field:     [4] VR2VA0
//
// Enables isolation switch between VR_ATEST_A0 and VA_ATEST_A0.
// ENUMs:
// CLOSE                    Switch is closed
// OPEN                     Switch is open
#define SYS0_ATESTCFG_VR2VA0                                        0x00000010U
#define SYS0_ATESTCFG_VR2VA0_M                                      0x00000010U
#define SYS0_ATESTCFG_VR2VA0_S                                               4U
#define SYS0_ATESTCFG_VR2VA0_CLOSE                                  0x00000010U
#define SYS0_ATESTCFG_VR2VA0_OPEN                                   0x00000000U

// Field:     [3] SHTVA1
//
// Shorts VA_ATEST_A1 to ground.
// ENUMs:
// CLOSE                    Switch is closed
// OPEN                     Switch is open
#define SYS0_ATESTCFG_SHTVA1                                        0x00000008U
#define SYS0_ATESTCFG_SHTVA1_M                                      0x00000008U
#define SYS0_ATESTCFG_SHTVA1_S                                               3U
#define SYS0_ATESTCFG_SHTVA1_CLOSE                                  0x00000008U
#define SYS0_ATESTCFG_SHTVA1_OPEN                                   0x00000000U

// Field:     [2] SHTVA0
//
// Shorts VA_ATEST_A0 to ground.
// ENUMs:
// CLOSE                    Switch is closed
// OPEN                     Switch is open
#define SYS0_ATESTCFG_SHTVA0                                        0x00000004U
#define SYS0_ATESTCFG_SHTVA0_M                                      0x00000004U
#define SYS0_ATESTCFG_SHTVA0_S                                               2U
#define SYS0_ATESTCFG_SHTVA0_CLOSE                                  0x00000004U
#define SYS0_ATESTCFG_SHTVA0_OPEN                                   0x00000000U

// Field:     [1] SHTVR1
//
// Shorts VR_ATEST_A1 to ground.
// ENUMs:
// CLOSE                    Switch is closed
// OPEN                     Switch is open
#define SYS0_ATESTCFG_SHTVR1                                        0x00000002U
#define SYS0_ATESTCFG_SHTVR1_M                                      0x00000002U
#define SYS0_ATESTCFG_SHTVR1_S                                               1U
#define SYS0_ATESTCFG_SHTVR1_CLOSE                                  0x00000002U
#define SYS0_ATESTCFG_SHTVR1_OPEN                                   0x00000000U

// Field:     [0] SHTVR0
//
// Shorts VR_ATEST_A0 to ground.
// ENUMs:
// CLOSE                    Switch is closed
// OPEN                     Switch is open
#define SYS0_ATESTCFG_SHTVR0                                        0x00000001U
#define SYS0_ATESTCFG_SHTVR0_M                                      0x00000001U
#define SYS0_ATESTCFG_SHTVR0_S                                               0U
#define SYS0_ATESTCFG_SHTVR0_CLOSE                                  0x00000001U
#define SYS0_ATESTCFG_SHTVR0_OPEN                                   0x00000000U

//*****************************************************************************
//
// Register: SYS0_O_TSENSCFG
//
//*****************************************************************************
// Field:    [11] TSENS2EN
//
// This is the enable bit for the second temperature sensor in AUX.
// ENUMs:
// EN                       Second temperature sensor is enabled.
// DIS                      Second temperature sensor is disabled.
#define SYS0_TSENSCFG_TSENS2EN                                      0x00000800U
#define SYS0_TSENSCFG_TSENS2EN_M                                    0x00000800U
#define SYS0_TSENSCFG_TSENS2EN_S                                            11U
#define SYS0_TSENSCFG_TSENS2EN_EN                                   0x00000800U
#define SYS0_TSENSCFG_TSENS2EN_DIS                                  0x00000000U

// Field:   [1:0] SEL
//
// Used to enable and configure temperature sensor module. Setting the value to
// 0x3 will disable the temperature sensor.
// ENUMs:
// GND                      20uA current is injected on VR_ATEST_A0 and ground
//                          measured on VR_ATEST_A1
// VALUE                    20uA current is injected on VR_ATEST_A0 and
//                          voltage measured on VR_ATEST_A1
// DISABLE                  Temperature sensor is disabled
#define SYS0_TSENSCFG_SEL_W                                                  2U
#define SYS0_TSENSCFG_SEL_M                                         0x00000003U
#define SYS0_TSENSCFG_SEL_S                                                  0U
#define SYS0_TSENSCFG_SEL_GND                                       0x00000002U
#define SYS0_TSENSCFG_SEL_VALUE                                     0x00000001U
#define SYS0_TSENSCFG_SEL_DISABLE                                   0x00000000U

//*****************************************************************************
//
// Register: SYS0_O_LPCMPCFG
//
//*****************************************************************************
// Field:    [30] HYSPOL
//
// Hysteresis polarity
// ENUMs:
// INV                      Inverted
// NOINV                    Not inverted
#define SYS0_LPCMPCFG_HYSPOL                                        0x40000000U
#define SYS0_LPCMPCFG_HYSPOL_M                                      0x40000000U
#define SYS0_LPCMPCFG_HYSPOL_S                                              30U
#define SYS0_LPCMPCFG_HYSPOL_INV                                    0x40000000U
#define SYS0_LPCMPCFG_HYSPOL_NOINV                                  0x00000000U

// Field: [29:28] ATESTMUX
//
// Used to configure ATEST mux in comparator module and provides chosen output
// on VA_ATEST_A0.
// Note: This bit field is write-protected using global lock indicator on
// production device.
// ENUMs:
// IBIASOUT                 Selects bias current output
// COMP_VIN_NEG             Selects voltage divider output
// COMPOUT                  Selects comparator output
// OFF                      ATEST mux is off
#define SYS0_LPCMPCFG_ATESTMUX_W                                             2U
#define SYS0_LPCMPCFG_ATESTMUX_M                                    0x30000000U
#define SYS0_LPCMPCFG_ATESTMUX_S                                            28U
#define SYS0_LPCMPCFG_ATESTMUX_IBIASOUT                             0x30000000U
#define SYS0_LPCMPCFG_ATESTMUX_COMP_VIN_NEG                         0x20000000U
#define SYS0_LPCMPCFG_ATESTMUX_COMPOUT                              0x10000000U
#define SYS0_LPCMPCFG_ATESTMUX_OFF                                  0x00000000U

// Field:    [24] EVTIFG
//
// Event flag
//
// The event flag is set when the comparator output transition is qualified
// based on the edge polarity configuration in EDGCFG.
// ENUMs:
// SET
// CLR                      Clear
#define SYS0_LPCMPCFG_EVTIFG                                        0x01000000U
#define SYS0_LPCMPCFG_EVTIFG_M                                      0x01000000U
#define SYS0_LPCMPCFG_EVTIFG_S                                              24U
#define SYS0_LPCMPCFG_EVTIFG_SET                                    0x01000000U
#define SYS0_LPCMPCFG_EVTIFG_CLR                                    0x00000000U

// Field:    [21] COUTEN
//
// Enables LPCOMP output on device pin.
// ENUMs:
// EN                       Enabled
// DIS                      Disabled
#define SYS0_LPCMPCFG_COUTEN                                        0x00200000U
#define SYS0_LPCMPCFG_COUTEN_M                                      0x00200000U
#define SYS0_LPCMPCFG_COUTEN_S                                              21U
#define SYS0_LPCMPCFG_COUTEN_EN                                     0x00200000U
#define SYS0_LPCMPCFG_COUTEN_DIS                                    0x00000000U

// Field:    [20] COUT
//
// LPCOMP output status. This bit captures the value LPCOMP raw output.
// ENUMs:
// HIGH                     Output is high
// LOW                      Output is low
#define SYS0_LPCMPCFG_COUT                                          0x00100000U
#define SYS0_LPCMPCFG_COUT_M                                        0x00100000U
#define SYS0_LPCMPCFG_COUT_S                                                20U
#define SYS0_LPCMPCFG_COUT_HIGH                                     0x00100000U
#define SYS0_LPCMPCFG_COUT_LOW                                      0x00000000U

// Field:    [18] WUENSB
//
// Enables lpcmpcfg output to wake device from standby.
// ENUMs:
// EN                       Enable
// DIS                      Disable
#define SYS0_LPCMPCFG_WUENSB                                        0x00040000U
#define SYS0_LPCMPCFG_WUENSB_M                                      0x00040000U
#define SYS0_LPCMPCFG_WUENSB_S                                              18U
#define SYS0_LPCMPCFG_WUENSB_EN                                     0x00040000U
#define SYS0_LPCMPCFG_WUENSB_DIS                                    0x00000000U

// Field:    [17] EVTEN
//
// Enables event generation. Comparator module will produce event on ULL event
// fabric when EVTIFG is set.
// ENUMs:
// EN                       Enable
// DIS                      Disable
#define SYS0_LPCMPCFG_EVTEN                                         0x00020000U
#define SYS0_LPCMPCFG_EVTEN_M                                       0x00020000U
#define SYS0_LPCMPCFG_EVTEN_S                                               17U
#define SYS0_LPCMPCFG_EVTEN_EN                                      0x00020000U
#define SYS0_LPCMPCFG_EVTEN_DIS                                     0x00000000U

// Field:    [16] EDGCFG
//
// Selects positive edge or negative edge detection on comparator output to set
// the event flag
// ENUMs:
// FALL                     Fall edge detection
// RISE                     Rise edge detection
#define SYS0_LPCMPCFG_EDGCFG                                        0x00010000U
#define SYS0_LPCMPCFG_EDGCFG_M                                      0x00010000U
#define SYS0_LPCMPCFG_EDGCFG_S                                              16U
#define SYS0_LPCMPCFG_EDGCFG_FALL                                   0x00010000U
#define SYS0_LPCMPCFG_EDGCFG_RISE                                   0x00000000U

// Field: [14:12] NSEL
//
// Negative input selection. Setting values 0x5-0x7 will open all the switches.
// ENUMs:
// VDDD                     Selects VDDD
// VDDS                     Selects VDDS
// VA_PAD_A3                Selects VA_PAD_A3
// VA_PAD_A2                Selects VA_PAD_A2
// OPEN                     All switches are open
#define SYS0_LPCMPCFG_NSEL_W                                                 3U
#define SYS0_LPCMPCFG_NSEL_M                                        0x00007000U
#define SYS0_LPCMPCFG_NSEL_S                                                12U
#define SYS0_LPCMPCFG_NSEL_VDDD                                     0x00004000U
#define SYS0_LPCMPCFG_NSEL_VDDS                                     0x00003000U
#define SYS0_LPCMPCFG_NSEL_VA_PAD_A3                                0x00002000U
#define SYS0_LPCMPCFG_NSEL_VA_PAD_A2                                0x00001000U
#define SYS0_LPCMPCFG_NSEL_OPEN                                     0x00000000U

// Field:  [11:8] PSEL
//
// Positive input selection. Setting values 0x9-0xF will open all the switches.
// ENUMs:
// VDDS                     Selects VDDS
// VA_ATEST_A1              Selects VA_ATEST_A1
// VA_ATEST_A0              Selects VA_ATEST_A0
// VR_ATEST_A1              Selects VR_ATEST_A1
// VR_ATEST_A0              Selects VR_ATEST_A0
// VA_PAD_A3                Selects VA_PAD_A3
// VA_PAD_A2                Selects VA_PAD_A2
// VA_PAD_A1                Selects VA_PAD_A1
// OPEN                     All switches are open
#define SYS0_LPCMPCFG_PSEL_W                                                 4U
#define SYS0_LPCMPCFG_PSEL_M                                        0x00000F00U
#define SYS0_LPCMPCFG_PSEL_S                                                 8U
#define SYS0_LPCMPCFG_PSEL_VDDS                                     0x00000800U
#define SYS0_LPCMPCFG_PSEL_VA_ATEST_A1                              0x00000700U
#define SYS0_LPCMPCFG_PSEL_VA_ATEST_A0                              0x00000600U
#define SYS0_LPCMPCFG_PSEL_VR_ATEST_A1                              0x00000500U
#define SYS0_LPCMPCFG_PSEL_VR_ATEST_A0                              0x00000400U
#define SYS0_LPCMPCFG_PSEL_VA_PAD_A3                                0x00000300U
#define SYS0_LPCMPCFG_PSEL_VA_PAD_A2                                0x00000200U
#define SYS0_LPCMPCFG_PSEL_VA_PAD_A1                                0x00000100U
#define SYS0_LPCMPCFG_PSEL_OPEN                                     0x00000000U

// Field:   [7:5] HYSSEL
//
// Used to enable and select hysteresis level
//
// Hysteresis is disabled when HYSSEL = 0 and enabled for other values of
// HYSSEL from 1 to 7. Refer to device specific datasheet for individual
// hysteresis values.
// ENUMs:
// VAL7                     Hysteresis value 7
// VAL6                     Hysteresis value 6
// VAL5                     Hysteresis value 5
// VAL4                     Hysteresis value 4
// VAL3                     Hysteresis value 3
// VAL2                     Hysteresis value 2
// VAL1                     Hysteresis value 1
// VAL0                     Hysteresis is disabled
#define SYS0_LPCMPCFG_HYSSEL_W                                               3U
#define SYS0_LPCMPCFG_HYSSEL_M                                      0x000000E0U
#define SYS0_LPCMPCFG_HYSSEL_S                                               5U
#define SYS0_LPCMPCFG_HYSSEL_VAL7                                   0x000000E0U
#define SYS0_LPCMPCFG_HYSSEL_VAL6                                   0x000000C0U
#define SYS0_LPCMPCFG_HYSSEL_VAL5                                   0x000000A0U
#define SYS0_LPCMPCFG_HYSSEL_VAL4                                   0x00000080U
#define SYS0_LPCMPCFG_HYSSEL_VAL3                                   0x00000060U
#define SYS0_LPCMPCFG_HYSSEL_VAL2                                   0x00000040U
#define SYS0_LPCMPCFG_HYSSEL_VAL1                                   0x00000020U
#define SYS0_LPCMPCFG_HYSSEL_VAL0                                   0x00000000U

// Field:     [4] DIVPATH
//
// Used to select the path on which voltage divider is applied
// ENUMs:
// PSIDE                    Divider is applied on P-side
// NSIDE                    Divider is applied on N-side
#define SYS0_LPCMPCFG_DIVPATH                                       0x00000010U
#define SYS0_LPCMPCFG_DIVPATH_M                                     0x00000010U
#define SYS0_LPCMPCFG_DIVPATH_S                                              4U
#define SYS0_LPCMPCFG_DIVPATH_PSIDE                                 0x00000010U
#define SYS0_LPCMPCFG_DIVPATH_NSIDE                                 0x00000000U

// Field:   [3:1] DIV
//
// Used to configure reference divider. Setting values 0x5-0x7 will set the
// divide value to 1.
// ENUMs:
// VAL4                     Divide value is 1/4
// VAL3                     Divide value is 1/3
// VAL2                     Divide value is 1/2
// VAL1                     Divide value is 3/4
// VAL0                     Divide value is 1
#define SYS0_LPCMPCFG_DIV_W                                                  3U
#define SYS0_LPCMPCFG_DIV_M                                         0x0000000EU
#define SYS0_LPCMPCFG_DIV_S                                                  1U
#define SYS0_LPCMPCFG_DIV_VAL4                                      0x00000008U
#define SYS0_LPCMPCFG_DIV_VAL3                                      0x00000006U
#define SYS0_LPCMPCFG_DIV_VAL2                                      0x00000004U
#define SYS0_LPCMPCFG_DIV_VAL1                                      0x00000002U
#define SYS0_LPCMPCFG_DIV_VAL0                                      0x00000000U

// Field:     [0] EN
//
// Used to enable comparator module.
// ENUMs:
// EN                       Enable
// DIS                      Disable
#define SYS0_LPCMPCFG_EN                                            0x00000001U
#define SYS0_LPCMPCFG_EN_M                                          0x00000001U
#define SYS0_LPCMPCFG_EN_S                                                   0U
#define SYS0_LPCMPCFG_EN_EN                                         0x00000001U
#define SYS0_LPCMPCFG_EN_DIS                                        0x00000000U

//*****************************************************************************
//
// Register: SYS0_O_VGMCFG
//
//*****************************************************************************
// Field: [31:24] KEY
//
// Key must be written with value 0x5A for successful write to VGMCFG and to
// unlock register state.
// Write with any value other than 0x5A to KEY will be ignored and the register
// content is not updated.
// Read value of KEY is 0x0.
#define SYS0_VGMCFG_KEY_W                                                    8U
#define SYS0_VGMCFG_KEY_M                                           0xFF000000U
#define SYS0_VGMCFG_KEY_S                                                   24U

// Field:  [11:8] ATBMUXSEL
//
// These bits are used to generate VGM ATB mux selection control.
#define SYS0_VGMCFG_ATBMUXSEL_W                                              4U
#define SYS0_VGMCFG_ATBMUXSEL_M                                     0x00000F00U
#define SYS0_VGMCFG_ATBMUXSEL_S                                              8U

// Field:     [1] OSHDETDIS
//
// Disables overshoot detector in VGM.
// ENUMs:
// DIS                      Disable
// EN                       Enable
#define SYS0_VGMCFG_OSHDETDIS                                       0x00000002U
#define SYS0_VGMCFG_OSHDETDIS_M                                     0x00000002U
#define SYS0_VGMCFG_OSHDETDIS_S                                              1U
#define SYS0_VGMCFG_OSHDETDIS_DIS                                   0x00000002U
#define SYS0_VGMCFG_OSHDETDIS_EN                                    0x00000000U

// Field:     [0] USHTDETDIS
//
// Disables undershoot detector in VGM.
// ENUMs:
// DIS                      Disable
// EN                       Enable
#define SYS0_VGMCFG_USHTDETDIS                                      0x00000001U
#define SYS0_VGMCFG_USHTDETDIS_M                                    0x00000001U
#define SYS0_VGMCFG_USHTDETDIS_S                                             0U
#define SYS0_VGMCFG_USHTDETDIS_DIS                                  0x00000001U
#define SYS0_VGMCFG_USHTDETDIS_EN                                   0x00000000U

//*****************************************************************************
//
// Register: SYS0_O_VGMTRIMDBG
//
//*****************************************************************************
// Field:    [30] TMODE5
//
// Test mode bit for glitchy supply mux selection.
#define SYS0_VGMTRIMDBG_TMODE5                                      0x40000000U
#define SYS0_VGMTRIMDBG_TMODE5_M                                    0x40000000U
#define SYS0_VGMTRIMDBG_TMODE5_S                                            30U

// Field: [29:27] TMODE4
//
// Test mode bits for VREF mux selection for overshoot detector.
#define SYS0_VGMTRIMDBG_TMODE4_W                                             3U
#define SYS0_VGMTRIMDBG_TMODE4_M                                    0x38000000U
#define SYS0_VGMTRIMDBG_TMODE4_S                                            27U

// Field: [26:24] TMODE3
//
// Test mode bits for VREF mux selection for undershoot detector.
#define SYS0_VGMTRIMDBG_TMODE3_W                                             3U
#define SYS0_VGMTRIMDBG_TMODE3_M                                    0x07000000U
#define SYS0_VGMTRIMDBG_TMODE3_S                                            24U

// Field: [21:20] TMODE2
//
// Test mode for level shifter isolation.
#define SYS0_VGMTRIMDBG_TMODE2_W                                             2U
#define SYS0_VGMTRIMDBG_TMODE2_M                                    0x00300000U
#define SYS0_VGMTRIMDBG_TMODE2_S                                            20U

// Field: [19:18] TMODE1
//
// Test mode bits for low voltage SR latch reset.
#define SYS0_VGMTRIMDBG_TMODE1_W                                             2U
#define SYS0_VGMTRIMDBG_TMODE1_M                                    0x000C0000U
#define SYS0_VGMTRIMDBG_TMODE1_S                                            18U

// Field: [17:16] TMODE0
//
// Test mode bits for 3V SR latch reset.
#define SYS0_VGMTRIMDBG_TMODE0_W                                             2U
#define SYS0_VGMTRIMDBG_TMODE0_M                                    0x00030000U
#define SYS0_VGMTRIMDBG_TMODE0_S                                            16U

// Field:  [11:8] IBPROG
//
// These bits are used to program VGM bias current generator.
#define SYS0_VGMTRIMDBG_IBPROG_W                                             4U
#define SYS0_VGMTRIMDBG_IBPROG_M                                    0x00000F00U
#define SYS0_VGMTRIMDBG_IBPROG_S                                             8U

// Field:   [7:4] OSHDETTRIM
//
// These bits are used to trim VGM overshoot detector.
#define SYS0_VGMTRIMDBG_OSHDETTRIM_W                                         4U
#define SYS0_VGMTRIMDBG_OSHDETTRIM_M                                0x000000F0U
#define SYS0_VGMTRIMDBG_OSHDETTRIM_S                                         4U

// Field:   [3:0] USHTDETTRIM
//
// These bits are used to trim VGM undershoot detector.
#define SYS0_VGMTRIMDBG_USHTDETTRIM_W                                        4U
#define SYS0_VGMTRIMDBG_USHTDETTRIM_M                               0x0000000FU
#define SYS0_VGMTRIMDBG_USHTDETTRIM_S                                        0U

//*****************************************************************************
//
// Register: SYS0_O_DEVICEID
//
//*****************************************************************************
// Field: [31:28] VERSION
//
// Monotonic increasing value indicating new hardware revision. A newer
// hardware revision shall never have a lower version than an older revision of
// hardware.
#define SYS0_DEVICEID_VERSION_W                                              4U
#define SYS0_DEVICEID_VERSION_M                                     0xF0000000U
#define SYS0_DEVICEID_VERSION_S                                             28U

// Field: [27:12] DEVICE
//
// Value generated by RAMP for the SOC. This value uniquely identifies the die
// from any other TI device.
#define SYS0_DEVICEID_DEVICE_W                                              16U
#define SYS0_DEVICEID_DEVICE_M                                      0x0FFFF000U
#define SYS0_DEVICEID_DEVICE_S                                              12U

// Field:  [11:1] MANFACTURER
//
// JEP 106 assigned manufacturer ID. This field identifies the device as a
// Texas Instruments device.
#define SYS0_DEVICEID_MANFACTURER_W                                         11U
#define SYS0_DEVICEID_MANFACTURER_M                                 0x00000FFEU
#define SYS0_DEVICEID_MANFACTURER_S                                          1U

// Field:     [0] ALWAYSONE
//
// Value 1 in this bit field means that a 32-bit scan register exists.
#define SYS0_DEVICEID_ALWAYSONE                                     0x00000001U
#define SYS0_DEVICEID_ALWAYSONE_M                                   0x00000001U
#define SYS0_DEVICEID_ALWAYSONE_S                                            0U

//*****************************************************************************
//
// Register: SYS0_O_DBGAUTH
//
//*****************************************************************************
// Field: [17:16] CLR
//
// Clears Authentication Status
#define SYS0_DBGAUTH_CLR_W                                                   2U
#define SYS0_DBGAUTH_CLR_M                                          0x00030000U
#define SYS0_DBGAUTH_CLR_S                                                  16U

// Field:   [1:0] SET
//
// Sets Authentication Status
// ENUMs:
// SEC                      Secure Debug Authorized
// NONSEC                   Non-Secure Debug Authorized
// NONINV                   Only Non-Invasive Debug Authorized
// UNAUTH                   Debug Unauthorized
#define SYS0_DBGAUTH_SET_W                                                   2U
#define SYS0_DBGAUTH_SET_M                                          0x00000003U
#define SYS0_DBGAUTH_SET_S                                                   0U
#define SYS0_DBGAUTH_SET_SEC                                        0x00000003U
#define SYS0_DBGAUTH_SET_NONSEC                                     0x00000002U
#define SYS0_DBGAUTH_SET_NONINV                                     0x00000001U
#define SYS0_DBGAUTH_SET_UNAUTH                                     0x00000000U

//*****************************************************************************
//
// Register: SYS0_O_PARTID
//
//*****************************************************************************
// Field:    [31] START
//
// Start bit
// ENUMs:
// SET
// CLR                      Clear
#define SYS0_PARTID_START                                           0x80000000U
#define SYS0_PARTID_START_M                                         0x80000000U
#define SYS0_PARTID_START_S                                                 31U
#define SYS0_PARTID_START_SET                                       0x80000000U
#define SYS0_PARTID_START_CLR                                       0x00000000U

// Field: [30:28] MAJORREV
//
// Monotonic increasing value indicating a new revision of the SKU significant
// enough that users of the device may have to revise PCB or software design
#define SYS0_PARTID_MAJORREV_W                                               3U
#define SYS0_PARTID_MAJORREV_M                                      0x70000000U
#define SYS0_PARTID_MAJORREV_S                                              28U

// Field: [27:24] MINORREV
//
// Monotonic increasing value indicating a new revision of the SKU that
// preserves compatibility with lesser MINORREV values
#define SYS0_PARTID_MINORREV_W                                               4U
#define SYS0_PARTID_MINORREV_M                                      0x0F000000U
#define SYS0_PARTID_MINORREV_S                                              24U

// Field: [23:16] VARIANT
//
// Bit pattern uniquely identifying a variant of a part
#define SYS0_PARTID_VARIANT_W                                                8U
#define SYS0_PARTID_VARIANT_M                                       0x00FF0000U
#define SYS0_PARTID_VARIANT_S                                               16U

// Field:  [15:0] PART
//
// Bit pattern uniquely identifying a part
#define SYS0_PARTID_PART_W                                                  16U
#define SYS0_PARTID_PART_M                                          0x0000FFFFU
#define SYS0_PARTID_PART_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_LIFECYC
//
//*****************************************************************************
// Field:   [7:0] VAL
//
// Device life cycle value.
// ENUMs:
// LCYCLE_ILLEGAL           Illegal
// LCYCLE_RETEST            Re-test
// LCYCLE_PRODDEV           Production device
// LCYCLE_ENGRDEV           Engineering device
// LCYCLE_TESTFT            Test Final Test
// LCYCLE_TESTPT            Test Production Test
// LCYCLE_1STBDAY           First birthday
// LCYCLE_POSSIBLE_1STBDAY  Possible first birthday
#define SYS0_LIFECYC_VAL_W                                                   8U
#define SYS0_LIFECYC_VAL_M                                          0x000000FFU
#define SYS0_LIFECYC_VAL_S                                                   0U
#define SYS0_LIFECYC_VAL_LCYCLE_ILLEGAL                             0x000000FFU
#define SYS0_LIFECYC_VAL_LCYCLE_RETEST                              0x00000006U
#define SYS0_LIFECYC_VAL_LCYCLE_PRODDEV                             0x00000005U
#define SYS0_LIFECYC_VAL_LCYCLE_ENGRDEV                             0x00000004U
#define SYS0_LIFECYC_VAL_LCYCLE_TESTFT                              0x00000003U
#define SYS0_LIFECYC_VAL_LCYCLE_TESTPT                              0x00000002U
#define SYS0_LIFECYC_VAL_LCYCLE_1STBDAY                             0x00000001U
#define SYS0_LIFECYC_VAL_LCYCLE_POSSIBLE_1STBDAY                    0x00000000U

//*****************************************************************************
//
// Register: SYS0_O_TMUTE0
//
//*****************************************************************************
// Field:  [31:0] CDACL
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE0_CDACL_W                                                 32U
#define SYS0_TMUTE0_CDACL_M                                         0xFFFFFFFFU
#define SYS0_TMUTE0_CDACL_S                                                  0U

//*****************************************************************************
//
// Register: SYS0_O_TMUTE1
//
//*****************************************************************************
// Field:  [31:0] CDACM
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE1_CDACM_W                                                 32U
#define SYS0_TMUTE1_CDACM_M                                         0xFFFFFFFFU
#define SYS0_TMUTE1_CDACM_S                                                  0U

//*****************************************************************************
//
// Register: SYS0_O_TMUTE2
//
//*****************************************************************************
// Field: [30:26] IBTRIM
//
// LPCOMP: Bias current trim, 250nA to be terminated across I2V, 1M ohm
// setting.  Resulting target trim voltage 250mV.
#define SYS0_TMUTE2_IBTRIM_W                                                 5U
#define SYS0_TMUTE2_IBTRIM_M                                        0x7C000000U
#define SYS0_TMUTE2_IBTRIM_S                                                26U

// Field: [25:23] TRIM
//
// ADC REFBUF trim bits.
#define SYS0_TMUTE2_TRIM_W                                                   3U
#define SYS0_TMUTE2_TRIM_M                                          0x03800000U
#define SYS0_TMUTE2_TRIM_S                                                  23U

// Field: [22:16] LATCH
//
// SOC ADC: Latch trim bits. These bits are used in the analog IP.
#define SYS0_TMUTE2_LATCH_W                                                  7U
#define SYS0_TMUTE2_LATCH_M                                         0x007F0000U
#define SYS0_TMUTE2_LATCH_S                                                 16U

// Field:  [15:4] OFFSET
//
// SOCADC: Offset trim bits. These bits are used in DTC.
#define SYS0_TMUTE2_OFFSET_W                                                12U
#define SYS0_TMUTE2_OFFSET_M                                        0x0000FFF0U
#define SYS0_TMUTE2_OFFSET_S                                                 4U

// Field:   [3:2] RES
//
// SOCADC: Resistor trim bits. These bits are used in the analog IP.
#define SYS0_TMUTE2_RES_W                                                    2U
#define SYS0_TMUTE2_RES_M                                           0x0000000CU
#define SYS0_TMUTE2_RES_S                                                    2U

// Field:   [1:0] CDACU
//
// SOCADC: Upper 2 bits of CDAC trim. These bits are used in DTC.
#define SYS0_TMUTE2_CDACU_W                                                  2U
#define SYS0_TMUTE2_CDACU_M                                         0x00000003U
#define SYS0_TMUTE2_CDACU_S                                                  0U

//*****************************************************************************
//
// Register: SYS0_O_TMUTE3
//
//*****************************************************************************
// Field: [31:26] BATC1
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE3_BATC1_W                                                  6U
#define SYS0_TMUTE3_BATC1_M                                         0xFC000000U
#define SYS0_TMUTE3_BATC1_S                                                 26U

// Field: [25:19] BATC0
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE3_BATC0_W                                                  7U
#define SYS0_TMUTE3_BATC0_M                                         0x03F80000U
#define SYS0_TMUTE3_BATC0_S                                                 19U

// Field: [18:14] TEMPC2
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE3_TEMPC2_W                                                 5U
#define SYS0_TMUTE3_TEMPC2_M                                        0x0007C000U
#define SYS0_TMUTE3_TEMPC2_S                                                14U

// Field:  [13:8] TEMPC1
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE3_TEMPC1_W                                                 6U
#define SYS0_TMUTE3_TEMPC1_M                                        0x00003F00U
#define SYS0_TMUTE3_TEMPC1_S                                                 8U

// Field:   [7:0] TEMPC0
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE3_TEMPC0_W                                                 8U
#define SYS0_TMUTE3_TEMPC0_M                                        0x000000FFU
#define SYS0_TMUTE3_TEMPC0_S                                                 0U

//*****************************************************************************
//
// Register: SYS0_O_TMUTE4
//
//*****************************************************************************
// Field: [31:28] RECHCOMPREFLVL
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE4_RECHCOMPREFLVL_W                                         4U
#define SYS0_TMUTE4_RECHCOMPREFLVL_M                                0xF0000000U
#define SYS0_TMUTE4_RECHCOMPREFLVL_S                                        28U

// Field: [27:26] IOSTRCFG2
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE4_IOSTRCFG2_W                                              2U
#define SYS0_TMUTE4_IOSTRCFG2_M                                     0x0C000000U
#define SYS0_TMUTE4_IOSTRCFG2_S                                             26U

// Field: [25:22] IOSTRCFG1
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE4_IOSTRCFG1_W                                              4U
#define SYS0_TMUTE4_IOSTRCFG1_M                                     0x03C00000U
#define SYS0_TMUTE4_IOSTRCFG1_S                                             22U

// Field: [21:19] MAX
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE4_MAX_W                                                    3U
#define SYS0_TMUTE4_MAX_M                                           0x00380000U
#define SYS0_TMUTE4_MAX_S                                                   19U

// Field: [18:16] MED
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE4_MED_W                                                    3U
#define SYS0_TMUTE4_MED_M                                           0x00070000U
#define SYS0_TMUTE4_MED_S                                                   16U

// Field: [15:13] MIN
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE4_MIN_W                                                    3U
#define SYS0_TMUTE4_MIN_M                                           0x0000E000U
#define SYS0_TMUTE4_MIN_S                                                   13U

// Field: [12:11] DCDCLOAD
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE4_DCDCLOAD_W                                               2U
#define SYS0_TMUTE4_DCDCLOAD_M                                      0x00001800U
#define SYS0_TMUTE4_DCDCLOAD_S                                              11U

// Field:  [10:8] IPEAK
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE4_IPEAK_W                                                  3U
#define SYS0_TMUTE4_IPEAK_M                                         0x00000700U
#define SYS0_TMUTE4_IPEAK_S                                                  8U

// Field:   [7:6] DTIME
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE4_DTIME_W                                                  2U
#define SYS0_TMUTE4_DTIME_M                                         0x000000C0U
#define SYS0_TMUTE4_DTIME_S                                                  6U

// Field:   [5:3] LENSEL
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE4_LENSEL_W                                                 3U
#define SYS0_TMUTE4_LENSEL_M                                        0x00000038U
#define SYS0_TMUTE4_LENSEL_S                                                 3U

// Field:   [2:0] HENSEL
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE4_HENSEL_W                                                 3U
#define SYS0_TMUTE4_HENSEL_M                                        0x00000007U
#define SYS0_TMUTE4_HENSEL_S                                                 0U

//*****************************************************************************
//
// Register: SYS0_O_TMUTE5
//
//*****************************************************************************
// Field: [12:10] DCDCDRVDS
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE5_DCDCDRVDS_W                                              3U
#define SYS0_TMUTE5_DCDCDRVDS_M                                     0x00001C00U
#define SYS0_TMUTE5_DCDCDRVDS_S                                             10U

// Field:   [9:5] GLDOISCLR
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE5_GLDOISCLR_W                                              5U
#define SYS0_TMUTE5_GLDOISCLR_M                                     0x000003E0U
#define SYS0_TMUTE5_GLDOISCLR_S                                              5U

// Field:   [4:0] GLDOISSET
//
// Internal. Only to be used through TI provided API.
#define SYS0_TMUTE5_GLDOISSET_W                                              5U
#define SYS0_TMUTE5_GLDOISSET_M                                     0x0000001FU
#define SYS0_TMUTE5_GLDOISSET_S                                              0U

//*****************************************************************************
//
// Register: SYS0_O_CFG
//
//*****************************************************************************
// Field:   [7:6] RES
//
// Timer Resolution
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field determines the resolution of the timer values in registers SYS0.TIM0
// SYS0.TIM1 SYS0.TIM2.
// 1 LSB in the timer values = (4 << [SYS0.CFG.RES]) ms.
// ENUMs:
// _32Ms                    One LSB in the timer values = 32 ms
// _16MS                    One LSB in the timer values = 16 ms
// _8MS                     One LSB in the timer values = 8 ms
// _4MS                     One LSB in the timer values = 4 ms
#define SYS0_CFG_RES_W                                                       2U
#define SYS0_CFG_RES_M                                              0x000000C0U
#define SYS0_CFG_RES_S                                                       6U
#define SYS0_CFG_RES__32MS                                          0x000000C0U
#define SYS0_CFG_RES__16MS                                          0x00000080U
#define SYS0_CFG_RES__8MS                                           0x00000040U
#define SYS0_CFG_RES__4MS                                           0x00000000U

// Field:     [5] SNIFFMODE
//
// Sniff Mode Enable
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field enables sniff mode, where WuRX will duty cycle on and off while active
// until incoming energy is detected. When [SYS0.CFG.MODE] == WAKE0, sniff mode
// requires [SYS0.PRE.LEN] to be 8BIT or 16BIT.
// ENUMs:
// EN                       Sniff mode enabled
// DIS                      Sniff mode disabled
#define SYS0_CFG_SNIFFMODE                                          0x00000020U
#define SYS0_CFG_SNIFFMODE_M                                        0x00000020U
#define SYS0_CFG_SNIFFMODE_S                                                 5U
#define SYS0_CFG_SNIFFMODE_EN                                       0x00000020U
#define SYS0_CFG_SNIFFMODE_DIS                                      0x00000000U

// Field:     [4] WAKEWINEXT
//
// Wake Window Extension
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field determines if wake window should be extended when preamble is detected
// and remaining active time is too short for sync word detection. When
// [SYS0.CFG.SNIFFMODE] == DIS, wake window extension requires [SYS0.PRE.LEN]
// to be nonzero.
// ENUMs:
// EN                       Wake window extension enabled
// DIS                      Wake window extension disabled
#define SYS0_CFG_WAKEWINEXT                                         0x00000010U
#define SYS0_CFG_WAKEWINEXT_M                                       0x00000010U
#define SYS0_CFG_WAKEWINEXT_S                                                4U
#define SYS0_CFG_WAKEWINEXT_EN                                      0x00000010U
#define SYS0_CFG_WAKEWINEXT_DIS                                     0x00000000U

// Field:   [3:2] MODE
//
// Mode of Operation
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field determines the mode of operation.
// ENUMs:
// ATEST                    Analog test mode
// TIMER                    Timer mode
// WAKE1                    Wake mode1, where patterns are sent back to back
// WAKE0                    Wake mode 0, where there are silent intervals
//                          between patterns
#define SYS0_CFG_MODE_W                                                      2U
#define SYS0_CFG_MODE_M                                             0x0000000CU
#define SYS0_CFG_MODE_S                                                      2U
#define SYS0_CFG_MODE_ATEST                                         0x0000000CU
#define SYS0_CFG_MODE_TIMER                                         0x00000008U
#define SYS0_CFG_MODE_WAKE1                                         0x00000004U
#define SYS0_CFG_MODE_WAKE0                                         0x00000000U

// Field:     [1] WEVCLR
//
// Wakeup Event Clear
//
// This field may be written to at any time.
//
// Field determines if the output event flags are enabled or disabled. When
// enabled, events can be generated, and any event flag set high will be kept
// high indefinitely. When disabled, events will not be generated, and any
// event flags already set high will be set low. While [SYS0.CFG.CTL] == DIS,
// [SYS0.CFG.WEVCLR] will be overridden as if it is set to its disabled state.
// ENUMs:
// EN                       Events enabled and kept indefinitely
// DIS                      Events disabled and cleared
#define SYS0_CFG_WEVCLR                                             0x00000002U
#define SYS0_CFG_WEVCLR_M                                           0x00000002U
#define SYS0_CFG_WEVCLR_S                                                    1U
#define SYS0_CFG_WEVCLR_EN                                          0x00000002U
#define SYS0_CFG_WEVCLR_DIS                                         0x00000000U

// Field:     [0] CTL
//
// Control
//
// This field may be written to at any time.
//
// Field determines if the controller is enabled or disabled. Field shall be 0
// when the module is being configured and must only be set to 1 to enable
// functional operation.
// ENUMs:
// EN                       Module is enabled
// DIS                      Module is disabled
#define SYS0_CFG_CTL                                                0x00000001U
#define SYS0_CFG_CTL_M                                              0x00000001U
#define SYS0_CFG_CTL_S                                                       0U
#define SYS0_CFG_CTL_EN                                             0x00000001U
#define SYS0_CFG_CTL_DIS                                            0x00000000U

//*****************************************************************************
//
// Register: SYS0_O_PRE
//
//*****************************************************************************
// Field:   [7:6] LEN
//
// Preamble Length
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field determines the preamble length. Preamble length must be nonzero when
// [SYS0.CFG.SNIFFMODE] == DIS && [SYS0.CFG.WAKEWINEXT] == EN. Preamble length
// must be 8BIT or 16BIT when [SYS0.CFG.SNIFFMODE] == EN && [SYS0.CFG.MODE] ==
// WAKE0.
// ENUMs:
// _16BIT                   Preamble length = 16 bits
// _8BIT                    Preamble length = 8 bits
// _4BIT                    Preamble length = 4 bits
// _0BIT                    Preamble length = 0 bits
#define SYS0_PRE_LEN_W                                                       2U
#define SYS0_PRE_LEN_M                                              0x000000C0U
#define SYS0_PRE_LEN_S                                                       6U
#define SYS0_PRE_LEN__16BIT                                         0x000000C0U
#define SYS0_PRE_LEN__8BIT                                          0x00000080U
#define SYS0_PRE_LEN__4BIT                                          0x00000040U
#define SYS0_PRE_LEN__0BIT                                          0x00000000U

// Field:   [5:4] THR
//
// Preamble Correlation Threshold
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field determines the threshold value used by wake receiver to indicate a
// preamble correlation event, and is only used if [SYS0.CFG.WAKEWINEXT] = EN.
// Correlation trigger = Number of non-correlating bits ≤ [SYS0.PRE.THR].
// ENUMs:
// _3BIT                    Preamble correlation when up to 3 symbols do not
//                          match.
// _2BIT                    Preamble correlation when up to 2 symbols do not
//                          match.
// _1BIT                    Preamble correlation when up to 1 symbol does not
//                          match.
// _0BIT                    Preamble correlation when all symbols match.
#define SYS0_PRE_THR_W                                                       2U
#define SYS0_PRE_THR_M                                              0x00000030U
#define SYS0_PRE_THR_S                                                       4U
#define SYS0_PRE_THR__3BIT                                          0x00000030U
#define SYS0_PRE_THR__2BIT                                          0x00000020U
#define SYS0_PRE_THR__1BIT                                          0x00000010U
#define SYS0_PRE_THR__0BIT                                          0x00000000U

// Field:   [3:0] SYM
//
// Preamble Symbols
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field determines baseband symbols in preamble. Wake receiver expects a
// preamble equal to this 4-bit pattern repeated [SYS0.PRE.LEN]/4 times, with
// the MSB transmitted first.
#define SYS0_PRE_SYM_W                                                       4U
#define SYS0_PRE_SYM_M                                              0x0000000FU
#define SYS0_PRE_SYM_S                                                       0U

//*****************************************************************************
//
// Register: SYS0_O_SWB0
//
//*****************************************************************************
// Field:   [7:0] SYM
//
// Sync Word Symbols 7:0
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field determines baseband symbols 7:0 in sync word.
// Necessary for all [SYS0.CORR.SWLEN] options.
#define SYS0_SWB0_SYM_W                                                      8U
#define SYS0_SWB0_SYM_M                                             0x000000FFU
#define SYS0_SWB0_SYM_S                                                      0U

//*****************************************************************************
//
// Register: SYS0_O_SWB1
//
//*****************************************************************************
// Field:   [7:0] SYM
//
// Sync Word Symbols 15:8
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field determines baseband symbols 15:8 in sync word.
// Necessary when [SYS0.CORR.SWLEN] == 1BYTE/2BYTE/3BYTE.
#define SYS0_SWB1_SYM_W                                                      8U
#define SYS0_SWB1_SYM_M                                             0x000000FFU
#define SYS0_SWB1_SYM_S                                                      0U

//*****************************************************************************
//
// Register: SYS0_O_SWB2
//
//*****************************************************************************
// Field:   [7:0] SYM
//
// Sync Word Symbols 23:16
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field determines baseband symbols 23:16 in sync word.
// Necessary when [SYS0.CORR.SWLEN] == 2BYTE/3BYTE.
#define SYS0_SWB2_SYM_W                                                      8U
#define SYS0_SWB2_SYM_M                                             0x000000FFU
#define SYS0_SWB2_SYM_S                                                      0U

//*****************************************************************************
//
// Register: SYS0_O_SWB3
//
//*****************************************************************************
// Field:   [7:0] SYM
//
// Sync Word Symbols 31:24
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field determines baseband symbols 31:24 in sync word.
// Necessary when [SYS0.CORR.SWLEN] == 3BYTE.
#define SYS0_SWB3_SYM_W                                                      8U
#define SYS0_SWB3_SYM_M                                             0x000000FFU
#define SYS0_SWB3_SYM_S                                                      0U

//*****************************************************************************
//
// Register: SYS0_O_CORR
//
//*****************************************************************************
// Field:     [7] EVDLYPAD
//
// Event Delay Padding
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// If set, delay between event generation and event signalling when
// [SYS0.CFG.MODE] == WAKE1 is padded by an extra 12.5%. The nominal delay is
// one active period, as set by [SYS0.TIM0.ONLSB] and [SYS0.TIM1.ONMSB].
// ENUMs:
// EN                       Event delay padding enabled
// DIS                      Event delay padding disabled
#define SYS0_CORR_EVDLYPAD                                          0x00000080U
#define SYS0_CORR_EVDLYPAD_M                                        0x00000080U
#define SYS0_CORR_EVDLYPAD_S                                                 7U
#define SYS0_CORR_EVDLYPAD_EN                                       0x00000080U
#define SYS0_CORR_EVDLYPAD_DIS                                      0x00000000U

// Field:     [6] GLITCHDET
//
// Glitch Detect Enable
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field enables glitch detection and correction ahead of the decoder. If
// enabled, a sampled value that is different from both the sample before it
// and after it will be corrected to be equal to these, i.e. three consecutive
// samples of 010 or 101 will be corrected to 000 or 111, respectively.
// ENUMs:
// EN                       Glitch detection enabled
// DIS                      Glitch detection disabled
#define SYS0_CORR_GLITCHDET                                         0x00000040U
#define SYS0_CORR_GLITCHDET_M                                       0x00000040U
#define SYS0_CORR_GLITCHDET_S                                                6U
#define SYS0_CORR_GLITCHDET_EN                                      0x00000040U
#define SYS0_CORR_GLITCHDET_DIS                                     0x00000000U

// Field:     [5] QUAL
//
// Correlation Qualification
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// If set, field ensures no correlation can be signalled until a number of
// samples equal to the programmed sync word length [SYS0.CORR.SWLEN] have been
// processed.
// ENUMs:
// EN                       Correlation qualification enabled
// DIS                      Correlation qualification disabled
#define SYS0_CORR_QUAL                                              0x00000020U
#define SYS0_CORR_QUAL_M                                            0x00000020U
#define SYS0_CORR_QUAL_S                                                     5U
#define SYS0_CORR_QUAL_EN                                           0x00000020U
#define SYS0_CORR_QUAL_DIS                                          0x00000000U

// Field:     [4] CODESCHEME
//
// Manchester Code Scheme
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field determines the On-Off Keying (OOK) Manchester Coding Scheme. Same
// scheme is used for preamble and sync word.
// ENUMs:
// IEEE                     Bit value 0 corresponds to high level followed by
//                          low level, opposite for bit value 1
// GETHOMAS                 Bit value 0 corresponds to low level followed by
//                          high level, opposite for bit value 1
#define SYS0_CORR_CODESCHEME                                        0x00000010U
#define SYS0_CORR_CODESCHEME_M                                      0x00000010U
#define SYS0_CORR_CODESCHEME_S                                               4U
#define SYS0_CORR_CODESCHEME_IEEE                                   0x00000010U
#define SYS0_CORR_CODESCHEME_GETHOMAS                               0x00000000U

// Field:   [3:2] SWLEN
//
// Sync Word Length
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// For sync word of length N bytes, the lower N bytes of registers SYS0.SWB0
// SYS0.SWB1 SYS0.SWB2 SYS0.SWB3 are used.
// ENUMs:
// _4BYTE                   Sync word length = 4 bytes
// _3BYTE                   Sync word length = 3 bytes
// _2BYTE                   Sync word length = 2 bytes
// _1BYTE                   Sync word length = 1 byte
#define SYS0_CORR_SWLEN_W                                                    2U
#define SYS0_CORR_SWLEN_M                                           0x0000000CU
#define SYS0_CORR_SWLEN_S                                                    2U
#define SYS0_CORR_SWLEN__4BYTE                                      0x0000000CU
#define SYS0_CORR_SWLEN__3BYTE                                      0x00000008U
#define SYS0_CORR_SWLEN__2BYTE                                      0x00000004U
#define SYS0_CORR_SWLEN__1BYTE                                      0x00000000U

// Field:   [1:0] SWTHR
//
// Sync Word Correlation Threshold
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field determines the threshold value used by wake receiver to indicate a
// sync word correlation event.
// Correlation trigger = Number of non-correlating bits ≤ [SYS0.CORR.SWTHR].
// ENUMs:
// _3BIT                    Sync word correlation when up to 3 symbols do not
//                          match.
// _2BIT                    Sync word correlation when up to 2 symbols do not
//                          match.
// _1BIT                    Sync word correlation when up to 1 symbol does not
//                          match.
// _0BIT                    Sync word correlation when all symbols match.
#define SYS0_CORR_SWTHR_W                                                    2U
#define SYS0_CORR_SWTHR_M                                           0x00000003U
#define SYS0_CORR_SWTHR_S                                                    0U
#define SYS0_CORR_SWTHR__3BIT                                       0x00000003U
#define SYS0_CORR_SWTHR__2BIT                                       0x00000002U
#define SYS0_CORR_SWTHR__1BIT                                       0x00000001U
#define SYS0_CORR_SWTHR__0BIT                                       0x00000000U

//*****************************************************************************
//
// Register: SYS0_O_TIM0
//
//*****************************************************************************
// Field:   [7:0] ONLSB
//
// On-time Value Bits 7:0
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// On-time value = ([SYS0.TIM1.ONMSB] << 8) + [SYS0.TIM0.ONLSB], in units of (4
// << [SYS0.CFG.RES]) ms.
//
// When [SYS0.CFG.MODE] == WAKE0, this is the active duration of the receiver,
// where it searches for a transmission.
// When [SYS0.CFG.MODE] == WAKE1, this is the active duration of the receiver,
// where it searches for a transmission. It is also the delay between an
// incoming sync word being detected and the output event flag being set high,
// optionally padded by an extra 12.5% if [SYS0.CORR.EVDLYPAD] == EN.
// When [SYS0.CFG.MODE] == TIMER/ATEST, this is unused.
#define SYS0_TIM0_ONLSB_W                                                    8U
#define SYS0_TIM0_ONLSB_M                                           0x000000FFU
#define SYS0_TIM0_ONLSB_S                                                    0U

//*****************************************************************************
//
// Register: SYS0_O_TIM1
//
//*****************************************************************************
// Field:   [7:2] OFFLSB
//
// Off-time Value Bits 5:0
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Off-time value = ([SYS0.TIM2.OFFMSB] << 6) + [SYS0.TIM1.OFFLSB], in units of
// (4 << [SYS0.CFG.RES]) ms.
//
// When [SYS0.CFG.MODE] == WAKE0/WAKE1, this is the inactive duration of the
// receiver.
// When [SYS0.CFG.MODE] == TIMER, this is the period of the wake-up events.
// When [SYS0.CFG.MODE] == ATEST, this is unused.
#define SYS0_TIM1_OFFLSB_W                                                   6U
#define SYS0_TIM1_OFFLSB_M                                          0x000000FCU
#define SYS0_TIM1_OFFLSB_S                                                   2U

// Field:   [1:0] ONMSB
//
// On-time Value Bits 9:8
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// On-time value = (SYS0.ONMSB << 8) + [SYS0.TIM0.ONLSB], in units of (4 <<
// [SYS0.CFG.RES]) ms.
//
// When [SYS0.CFG.MODE] == WAKE0, this is the active duration of the receiver,
// where it searches for a transmission.
// When [SYS0.CFG.MODE] == WAKE1, this is the active duration of the receiver,
// where it searches for a transmission. It is also the delay between an
// incoming sync word being detected and the output event flag being set high,
// optionally padded by an extra 12.5% if [SYS0.CORR.EVDLYPAD] == EN.
// When [SYS0.CFG.MODE] == TIMER/ATEST, this is unused.
#define SYS0_TIM1_ONMSB_W                                                    2U
#define SYS0_TIM1_ONMSB_M                                           0x00000003U
#define SYS0_TIM1_ONMSB_S                                                    0U

//*****************************************************************************
//
// Register: SYS0_O_TIM2
//
//*****************************************************************************
// Field:   [7:0] OFFMSB
//
// Off-time Value Bits 13:6
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Off-time value = ([SYS0.TIM2.OFFMSB] << 6) + [SYS0.TIM1.OFFLSB], in units of
// (4 << [SYS0.CFG.RES]) ms.
//
// When [SYS0.CFG.MODE] == WAKE0/WAKE1, this is the inactive duration of the
// receiver.
// When [SYS0.CFG.MODE] == TIMER, this is the period of the wake-up events.
// When [SYS0.CFG.MODE] == ATEST, this is unused.
#define SYS0_TIM2_OFFMSB_W                                                   8U
#define SYS0_TIM2_OFFMSB_M                                          0x000000FFU
#define SYS0_TIM2_OFFMSB_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_TRIM0
//
//*****************************************************************************
// Field:   [7:0] OSCTRIMLSB
//
// Oscillator Trim MSB
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// The trim value is located in FCFG.
// The oscillator trim =  ([SYS0.TRIM1.OSCTRIMMSB] << 8) +
// [SYS0.TRIM0.OSCTRIMLSB].
//
// The trim is unsigned and software must flip the MSB to create the final
// value.
// With the MSB flipped, this results in:
// - Lowest frequency corresponds to logical trim value 0 and field value
// 9'd256.
// - Highest frequency corresponds to logical trim value 511 and field value
// 9'd255."
#define SYS0_TRIM0_OSCTRIMLSB_W                                              8U
#define SYS0_TRIM0_OSCTRIMLSB_M                                     0x000000FFU
#define SYS0_TRIM0_OSCTRIMLSB_S                                              0U

//*****************************************************************************
//
// Register: SYS0_O_TRIM1
//
//*****************************************************************************
// Field:   [7:1] ENVDTRIM
//
// Envelope Detector Trim
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// The trim value is located in FCFG.
// Field configures envelope detector trim.
#define SYS0_TRIM1_ENVDTRIM_W                                                7U
#define SYS0_TRIM1_ENVDTRIM_M                                       0x000000FEU
#define SYS0_TRIM1_ENVDTRIM_S                                                1U

// Field:     [0] OSCTRIMMSB
//
// Oscillator Trim MSB
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// The trim value is located in FCFG.
// The oscillator trim =  ([SYS0.TRIM1.OSCTRIMMSB] << 8) +
// [SYS0.TRIM0.OSCTRIMLSB].
//
// The trim is unsigned and software must flip the MSB to create the final
// value.
// With the MSB flipped, this results in:
// - Lowest frequency corresponds to logical trim value 0 and field value
// 9'd256.
// - Highest frequency corresponds to logical trim value 511 and field value
// 9'd255.
#define SYS0_TRIM1_OSCTRIMMSB                                       0x00000001U
#define SYS0_TRIM1_OSCTRIMMSB_M                                     0x00000001U
#define SYS0_TRIM1_OSCTRIMMSB_S                                              0U

//*****************************************************************************
//
// Register: SYS0_O_FE
//
//*****************************************************************************
// Field:     [7] BBAMPGAIN
//
// Baseband Amplifier Gain
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field configures analog gain control found in characterization for the
// baseband amplifier.
// The gain control enable/disable value is the same for all devices, and must
// be set to this value.
// ENUMs:
// EN                       Baseband amplifier gain enabled
// DIS                      Baseband amplifier gain disabled
#define SYS0_FE_BBAMPGAIN                                           0x00000080U
#define SYS0_FE_BBAMPGAIN_M                                         0x00000080U
#define SYS0_FE_BBAMPGAIN_S                                                  7U
#define SYS0_FE_BBAMPGAIN_EN                                        0x00000080U
#define SYS0_FE_BBAMPGAIN_DIS                                       0x00000000U

// Field:     [6] ENVDGAIN
//
// Envelope Detector Gain
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field configures analog gain control for the envelope detector.
// ENUMs:
// EN                       Envelope detector gain enabled
// DIS                      Envelope detector gain disabled
#define SYS0_FE_ENVDGAIN                                            0x00000040U
#define SYS0_FE_ENVDGAIN_M                                          0x00000040U
#define SYS0_FE_ENVDGAIN_S                                                   6U
#define SYS0_FE_ENVDGAIN_EN                                         0x00000040U
#define SYS0_FE_ENVDGAIN_DIS                                        0x00000000U

// Field:     [5] ENVDMATCH
//
// Envelope Detector Match Enable
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field configures analog envelope detector matching found in
// characterization.
// The envelope detector match enable/disable value is the same for all
// devices, and must be set to this value.
// ENUMs:
// EN                       Envelope detector match enabled
// DIS                      Envelope detector match disabled
#define SYS0_FE_ENVDMATCH                                           0x00000020U
#define SYS0_FE_ENVDMATCH_M                                         0x00000020U
#define SYS0_FE_ENVDMATCH_S                                                  5U
#define SYS0_FE_ENVDMATCH_EN                                        0x00000020U
#define SYS0_FE_ENVDMATCH_DIS                                       0x00000000U

// Field:     [4] IBBOOST
//
// Ibias Current Boost Enable
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field configures Ibias current boost.
// The Ibias current boost enable/disable value is located in FCFG.
// ENUMs:
// EN                       Ibias boost enabled
// DIS                      Ibias boost disabled
#define SYS0_FE_IBBOOST                                             0x00000010U
#define SYS0_FE_IBBOOST_M                                           0x00000010U
#define SYS0_FE_IBBOOST_S                                                    4U
#define SYS0_FE_IBBOOST_EN                                          0x00000010U
#define SYS0_FE_IBBOOST_DIS                                         0x00000000U

// Field:   [3:0] ATB
//
// Analog Test Bus
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
//
// Field determines which analog node connects to the Analog Test Bus (ATB).
// SW must always ensure one-hot value, and always perform break-before-make
// (0-value) between different one-hot values.
// ENUMs:
// ULFOSC                   ATB connected to ULFOSC
// CMPOUT                   ATB connected to comparator output
// BBOUTN                   ATB connected to baseband amplifier negative
//                          output
// BBOUTP                   ATB connected to baseband amplifier positive
//                          output
// NC                       ATB not connected
#define SYS0_FE_ATB_W                                                        4U
#define SYS0_FE_ATB_M                                               0x0000000FU
#define SYS0_FE_ATB_S                                                        0U
#define SYS0_FE_ATB_ULFOSC                                          0x00000008U
#define SYS0_FE_ATB_CMPOUT                                          0x00000004U
#define SYS0_FE_ATB_BBOUTN                                          0x00000002U
#define SYS0_FE_ATB_BBOUTP                                          0x00000001U
#define SYS0_FE_ATB_NC                                              0x00000000U

//*****************************************************************************
//
// Register: SYS0_O_ANASPARE
//
//*****************************************************************************
// Field:   [7:0] SPARE
//
// Spare
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
#define SYS0_ANASPARE_SPARE_W                                                8U
#define SYS0_ANASPARE_SPARE_M                                       0x000000FFU
#define SYS0_ANASPARE_SPARE_S                                                0U

//*****************************************************************************
//
// Register: SYS0_O_SPARE
//
//*****************************************************************************
// Field:   [7:0] SPARE
//
// Spare
//
// This field shall only be written to when [SYS0.CFG.CTL] == DIS.
#define SYS0_SPARE_SPARE_W                                                   8U
#define SYS0_SPARE_SPARE_M                                          0x000000FFU
#define SYS0_SPARE_SPARE_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_READONLY
//
//*****************************************************************************
// Field:   [7:0] READONLY
//
// Read-only
//
// This field is read-only.
#define SYS0_READONLY_READONLY_W                                             8U
#define SYS0_READONLY_READONLY_M                                    0x000000FFU
#define SYS0_READONLY_READONLY_S                                             0U

//*****************************************************************************
//
// Register: SYS0_O_SET03
//
//*****************************************************************************
// Field: [31:24] S3
//
// A high bit value will set the corresponding bit in ADI register 3. Read
// returns 0.
#define SYS0_SET03_S3_W                                                      8U
#define SYS0_SET03_S3_M                                             0xFF000000U
#define SYS0_SET03_S3_S                                                     24U

// Field: [23:16] S2
//
// A high bit value will set the corresponding bit in ADI register 2. Read
// returns 0.
#define SYS0_SET03_S2_W                                                      8U
#define SYS0_SET03_S2_M                                             0x00FF0000U
#define SYS0_SET03_S2_S                                                     16U

// Field:  [15:8] S1
//
// A high bit value will set the corresponding bit in ADI register 1. Read
// returns 0.
#define SYS0_SET03_S1_W                                                      8U
#define SYS0_SET03_S1_M                                             0x0000FF00U
#define SYS0_SET03_S1_S                                                      8U

// Field:   [7:0] S0
//
// A high bit value will set the corresponding bit in ADI register 0. Read
// returns 0.
#define SYS0_SET03_S0_W                                                      8U
#define SYS0_SET03_S0_M                                             0x000000FFU
#define SYS0_SET03_S0_S                                                      0U

//*****************************************************************************
//
// Register: SYS0_O_SET47
//
//*****************************************************************************
// Field: [31:24] S3
//
// A high bit value will set the corresponding bit in ADI register 7. Read
// returns 0.
#define SYS0_SET47_S3_W                                                      8U
#define SYS0_SET47_S3_M                                             0xFF000000U
#define SYS0_SET47_S3_S                                                     24U

// Field: [23:16] S2
//
// A high bit value will set the corresponding bit in ADI register 6. Read
// returns 0.
#define SYS0_SET47_S2_W                                                      8U
#define SYS0_SET47_S2_M                                             0x00FF0000U
#define SYS0_SET47_S2_S                                                     16U

// Field:  [15:8] S1
//
// A high bit value will set the corresponding bit in ADI register 5. Read
// returns 0.
#define SYS0_SET47_S1_W                                                      8U
#define SYS0_SET47_S1_M                                             0x0000FF00U
#define SYS0_SET47_S1_S                                                      8U

// Field:   [7:0] S0
//
// A high bit value will set the corresponding bit in ADI register 4. Read
// returns 0.
#define SYS0_SET47_S0_W                                                      8U
#define SYS0_SET47_S0_M                                             0x000000FFU
#define SYS0_SET47_S0_S                                                      0U

//*****************************************************************************
//
// Register: SYS0_O_SET811
//
//*****************************************************************************
// Field: [31:24] S3
//
// A high bit value will set the corresponding bit in ADI register 11. Read
// returns 0.
#define SYS0_SET811_S3_W                                                     8U
#define SYS0_SET811_S3_M                                            0xFF000000U
#define SYS0_SET811_S3_S                                                    24U

// Field: [23:16] S2
//
// A high bit value will set the corresponding bit in ADI register 10. Read
// returns 0.
#define SYS0_SET811_S2_W                                                     8U
#define SYS0_SET811_S2_M                                            0x00FF0000U
#define SYS0_SET811_S2_S                                                    16U

// Field:  [15:8] S1
//
// A high bit value will set the corresponding bit in ADI register 9. Read
// returns 0.
#define SYS0_SET811_S1_W                                                     8U
#define SYS0_SET811_S1_M                                            0x0000FF00U
#define SYS0_SET811_S1_S                                                     8U

// Field:   [7:0] S0
//
// A high bit value will set the corresponding bit in ADI register 8. Read
// returns 0.
#define SYS0_SET811_S0_W                                                     8U
#define SYS0_SET811_S0_M                                            0x000000FFU
#define SYS0_SET811_S0_S                                                     0U

//*****************************************************************************
//
// Register: SYS0_O_SET1215
//
//*****************************************************************************
// Field: [31:24] S3
//
// A high bit value will set the corresponding bit in ADI register 15. Read
// returns 0.
#define SYS0_SET1215_S3_W                                                    8U
#define SYS0_SET1215_S3_M                                           0xFF000000U
#define SYS0_SET1215_S3_S                                                   24U

// Field: [23:16] S2
//
// A high bit value will set the corresponding bit in ADI register 14. Read
// returns 0.
#define SYS0_SET1215_S2_W                                                    8U
#define SYS0_SET1215_S2_M                                           0x00FF0000U
#define SYS0_SET1215_S2_S                                                   16U

// Field:  [15:8] S1
//
// A high bit value will set the corresponding bit in ADI register 13. Read
// returns 0.
#define SYS0_SET1215_S1_W                                                    8U
#define SYS0_SET1215_S1_M                                           0x0000FF00U
#define SYS0_SET1215_S1_S                                                    8U

// Field:   [7:0] S0
//
// A high bit value will set the corresponding bit in ADI register 12. Read
// returns 0.
#define SYS0_SET1215_S0_W                                                    8U
#define SYS0_SET1215_S0_M                                           0x000000FFU
#define SYS0_SET1215_S0_S                                                    0U

//*****************************************************************************
//
// Register: SYS0_O_CLR03
//
//*****************************************************************************
// Field: [31:24] S3
//
// A high bit value will clear the corresponding bit in ADI register 3. Read
// returns 0.
#define SYS0_CLR03_S3_W                                                      8U
#define SYS0_CLR03_S3_M                                             0xFF000000U
#define SYS0_CLR03_S3_S                                                     24U

// Field: [23:16] S2
//
// A high bit value will clear the corresponding bit in ADI register 2. Read
// returns 0.
#define SYS0_CLR03_S2_W                                                      8U
#define SYS0_CLR03_S2_M                                             0x00FF0000U
#define SYS0_CLR03_S2_S                                                     16U

// Field:  [15:8] S1
//
// A high bit value will clear the corresponding bit in ADI register 1. Read
// returns 0.
#define SYS0_CLR03_S1_W                                                      8U
#define SYS0_CLR03_S1_M                                             0x0000FF00U
#define SYS0_CLR03_S1_S                                                      8U

// Field:   [7:0] S0
//
// A high bit value will clear the corresponding bit in ADI register 0. Read
// returns 0.
#define SYS0_CLR03_S0_W                                                      8U
#define SYS0_CLR03_S0_M                                             0x000000FFU
#define SYS0_CLR03_S0_S                                                      0U

//*****************************************************************************
//
// Register: SYS0_O_CLR47
//
//*****************************************************************************
// Field: [31:24] S3
//
// A high bit value will clear the corresponding bit in ADI register 7. Read
// returns 0.
#define SYS0_CLR47_S3_W                                                      8U
#define SYS0_CLR47_S3_M                                             0xFF000000U
#define SYS0_CLR47_S3_S                                                     24U

// Field: [23:16] S2
//
// A high bit value will clear the corresponding bit in ADI register 6. Read
// returns 0.
#define SYS0_CLR47_S2_W                                                      8U
#define SYS0_CLR47_S2_M                                             0x00FF0000U
#define SYS0_CLR47_S2_S                                                     16U

// Field:  [15:8] S1
//
// A high bit value will clear the corresponding bit in ADI register 5. Read
// returns 0.
#define SYS0_CLR47_S1_W                                                      8U
#define SYS0_CLR47_S1_M                                             0x0000FF00U
#define SYS0_CLR47_S1_S                                                      8U

// Field:   [7:0] S0
//
// A high bit value will clear the corresponding bit in ADI register 4. Read
// returns 0.
#define SYS0_CLR47_S0_W                                                      8U
#define SYS0_CLR47_S0_M                                             0x000000FFU
#define SYS0_CLR47_S0_S                                                      0U

//*****************************************************************************
//
// Register: SYS0_O_CLR811
//
//*****************************************************************************
// Field: [31:24] S3
//
// A high bit value will clear the corresponding bit in ADI register 11. Read
// returns 0.
#define SYS0_CLR811_S3_W                                                     8U
#define SYS0_CLR811_S3_M                                            0xFF000000U
#define SYS0_CLR811_S3_S                                                    24U

// Field: [23:16] S2
//
// A high bit value will clear the corresponding bit in ADI register 10. Read
// returns 0.
#define SYS0_CLR811_S2_W                                                     8U
#define SYS0_CLR811_S2_M                                            0x00FF0000U
#define SYS0_CLR811_S2_S                                                    16U

// Field:  [15:8] S1
//
// A high bit value will clear the corresponding bit in ADI register 9. Read
// returns 0.
#define SYS0_CLR811_S1_W                                                     8U
#define SYS0_CLR811_S1_M                                            0x0000FF00U
#define SYS0_CLR811_S1_S                                                     8U

// Field:   [7:0] S0
//
// A high bit value will clear the corresponding bit in ADI register 8. Read
// returns 0.
#define SYS0_CLR811_S0_W                                                     8U
#define SYS0_CLR811_S0_M                                            0x000000FFU
#define SYS0_CLR811_S0_S                                                     0U

//*****************************************************************************
//
// Register: SYS0_O_CLR1215
//
//*****************************************************************************
// Field: [31:24] S3
//
// A high bit value will clear the corresponding bit in ADI register 15. Read
// returns 0.
#define SYS0_CLR1215_S3_W                                                    8U
#define SYS0_CLR1215_S3_M                                           0xFF000000U
#define SYS0_CLR1215_S3_S                                                   24U

// Field: [23:16] S2
//
// A high bit value will clear the corresponding bit in ADI register 14. Read
// returns 0.
#define SYS0_CLR1215_S2_W                                                    8U
#define SYS0_CLR1215_S2_M                                           0x00FF0000U
#define SYS0_CLR1215_S2_S                                                   16U

// Field:  [15:8] S1
//
// A high bit value will clear the corresponding bit in ADI register 13. Read
// returns 0.
#define SYS0_CLR1215_S1_W                                                    8U
#define SYS0_CLR1215_S1_M                                           0x0000FF00U
#define SYS0_CLR1215_S1_S                                                    8U

// Field:   [7:0] S0
//
// A high bit value will clear the corresponding bit in ADI register 12. Read
// returns 0.
#define SYS0_CLR1215_S0_W                                                    8U
#define SYS0_CLR1215_S0_M                                           0x000000FFU
#define SYS0_CLR1215_S0_S                                                    0U

//*****************************************************************************
//
// Register: SYS0_O_SYNC
//
//*****************************************************************************
// Field:     [1] REQ
//
// Read current value of DI_REQ signal. Writing 0 to this bit forces a sync
// with slave, ensuring that req will be 0. It is recommended to write 0 to
// this register before power down of the master.
#define SYS0_SYNC_REQ                                               0x00000002U
#define SYS0_SYNC_REQ_M                                             0x00000002U
#define SYS0_SYNC_REQ_S                                                      1U

// Field:     [0] ACK
//
// Read current value of DI_ACK signal
#define SYS0_SYNC_ACK                                               0x00000001U
#define SYS0_SYNC_ACK_M                                             0x00000001U
#define SYS0_SYNC_ACK_S                                                      0U

//*****************************************************************************
//
// Register: SYS0_O_ADICFG
//
//*****************************************************************************
// Field:     [7] LOCK
//
// This register is no longer accessible when this bit is set (unless
// sticky_bit_overwrite is asserted on top module).
#define SYS0_ADICFG_LOCK                                            0x00000080U
#define SYS0_ADICFG_LOCK_M                                          0x00000080U
#define SYS0_ADICFG_LOCK_S                                                   7U

// Field:     [2] WAIT_FOR_ACK
//
// A transaction on the ADI interface does not end until ack has been received
// from the slave when this bit is set.
#define SYS0_ADICFG_WAIT_FOR_ACK                                    0x00000004U
#define SYS0_ADICFG_WAIT_FOR_ACK_M                                  0x00000004U
#define SYS0_ADICFG_WAIT_FOR_ACK_S                                           2U

// Field:   [1:0] CLK_DIV
//
// Sets the period of ADI transactions. ADI clock rising edge occurs in the
// middle of the period. Data and control to slave is set up in the beginning
// of the cycle, and data from slave is read after the transaction.
// ENUMs:
// DIV16                    One ADI transaction takes 16 master clock cycles
// DIV8                     One ADI transaction takes 8 master clock cycles
// DIV4                     One ADI transaction takes 4 master clock cycles
// DIV2                     One ADI transaction takes 2 master clock cycles
#define SYS0_ADICFG_CLK_DIV_W                                                2U
#define SYS0_ADICFG_CLK_DIV_M                                       0x00000003U
#define SYS0_ADICFG_CLK_DIV_S                                                0U
#define SYS0_ADICFG_CLK_DIV_DIV16                                   0x00000003U
#define SYS0_ADICFG_CLK_DIV_DIV8                                    0x00000002U
#define SYS0_ADICFG_CLK_DIV_DIV4                                    0x00000001U
#define SYS0_ADICFG_CLK_DIV_DIV2                                    0x00000000U

//*****************************************************************************
//
// Register: SYS0_O_MASK4B01
//
//*****************************************************************************
// Field: [31:28] M1H
//
// Mask for bits [7:4] in ADI register 1
#define SYS0_MASK4B01_M1H_W                                                  4U
#define SYS0_MASK4B01_M1H_M                                         0xF0000000U
#define SYS0_MASK4B01_M1H_S                                                 28U

// Field: [27:24] D1H
//
// Data for bits [7:4] in ADI register 1, only bits selected by mask M1H will
// be affected by access
#define SYS0_MASK4B01_D1H_W                                                  4U
#define SYS0_MASK4B01_D1H_M                                         0x0F000000U
#define SYS0_MASK4B01_D1H_S                                                 24U

// Field: [23:20] M1L
//
// Mask for bits [3:0] in ADI register 1
#define SYS0_MASK4B01_M1L_W                                                  4U
#define SYS0_MASK4B01_M1L_M                                         0x00F00000U
#define SYS0_MASK4B01_M1L_S                                                 20U

// Field: [19:16] D1L
//
// Data for bits [3:0] in ADI register 1, only bits selected by mask M1L will
// be affected by access
#define SYS0_MASK4B01_D1L_W                                                  4U
#define SYS0_MASK4B01_D1L_M                                         0x000F0000U
#define SYS0_MASK4B01_D1L_S                                                 16U

// Field: [15:12] M0H
//
// Mask for bits [7:4] in ADI register 0
#define SYS0_MASK4B01_M0H_W                                                  4U
#define SYS0_MASK4B01_M0H_M                                         0x0000F000U
#define SYS0_MASK4B01_M0H_S                                                 12U

// Field:  [11:8] D0H
//
// Data for bits [7:4] in ADI register 0, only bits selected by mask M0H will
// be affected by access
#define SYS0_MASK4B01_D0H_W                                                  4U
#define SYS0_MASK4B01_D0H_M                                         0x00000F00U
#define SYS0_MASK4B01_D0H_S                                                  8U

// Field:   [7:4] M0L
//
// Mask for bits [3:0] in ADI register 0
#define SYS0_MASK4B01_M0L_W                                                  4U
#define SYS0_MASK4B01_M0L_M                                         0x000000F0U
#define SYS0_MASK4B01_M0L_S                                                  4U

// Field:   [3:0] D0L
//
// Data for bits [3:0] in ADI register 0, only bits selected by mask M0L will
// be affected by access
#define SYS0_MASK4B01_D0L_W                                                  4U
#define SYS0_MASK4B01_D0L_M                                         0x0000000FU
#define SYS0_MASK4B01_D0L_S                                                  0U

//*****************************************************************************
//
// Register: SYS0_O_MASK4B23
//
//*****************************************************************************
// Field: [31:28] M1H
//
// Mask for bits [7:4] in ADI register 3
#define SYS0_MASK4B23_M1H_W                                                  4U
#define SYS0_MASK4B23_M1H_M                                         0xF0000000U
#define SYS0_MASK4B23_M1H_S                                                 28U

// Field: [27:24] D1H
//
// Data for bits [7:4] in ADI register 3, only bits selected by mask M1H will
// be affected by access
#define SYS0_MASK4B23_D1H_W                                                  4U
#define SYS0_MASK4B23_D1H_M                                         0x0F000000U
#define SYS0_MASK4B23_D1H_S                                                 24U

// Field: [23:20] M1L
//
// Mask for bits [3:0] in ADI register 3
#define SYS0_MASK4B23_M1L_W                                                  4U
#define SYS0_MASK4B23_M1L_M                                         0x00F00000U
#define SYS0_MASK4B23_M1L_S                                                 20U

// Field: [19:16] D1L
//
// Data for bits [3:0] in ADI register 3, only bits selected by mask M1L will
// be affected by access
#define SYS0_MASK4B23_D1L_W                                                  4U
#define SYS0_MASK4B23_D1L_M                                         0x000F0000U
#define SYS0_MASK4B23_D1L_S                                                 16U

// Field: [15:12] M0H
//
// Mask for bits [7:4] in ADI register 2
#define SYS0_MASK4B23_M0H_W                                                  4U
#define SYS0_MASK4B23_M0H_M                                         0x0000F000U
#define SYS0_MASK4B23_M0H_S                                                 12U

// Field:  [11:8] D0H
//
// Data for bits [7:4] in ADI register 2, only bits selected by mask M0H will
// be affected by access
#define SYS0_MASK4B23_D0H_W                                                  4U
#define SYS0_MASK4B23_D0H_M                                         0x00000F00U
#define SYS0_MASK4B23_D0H_S                                                  8U

// Field:   [7:4] M0L
//
// Mask for bits [3:0] in ADI register 2
#define SYS0_MASK4B23_M0L_W                                                  4U
#define SYS0_MASK4B23_M0L_M                                         0x000000F0U
#define SYS0_MASK4B23_M0L_S                                                  4U

// Field:   [3:0] D0L
//
// Data for bits [3:0] in ADI register 2, only bits selected by mask M0L will
// be affected by access
#define SYS0_MASK4B23_D0L_W                                                  4U
#define SYS0_MASK4B23_D0L_M                                         0x0000000FU
#define SYS0_MASK4B23_D0L_S                                                  0U

//*****************************************************************************
//
// Register: SYS0_O_MASK4B45
//
//*****************************************************************************
// Field: [31:28] M1H
//
// Mask for bits [7:4] in ADI register 5
#define SYS0_MASK4B45_M1H_W                                                  4U
#define SYS0_MASK4B45_M1H_M                                         0xF0000000U
#define SYS0_MASK4B45_M1H_S                                                 28U

// Field: [27:24] D1H
//
// Data for bits [7:4] in ADI register 5, only bits selected by mask M1H will
// be affected by access
#define SYS0_MASK4B45_D1H_W                                                  4U
#define SYS0_MASK4B45_D1H_M                                         0x0F000000U
#define SYS0_MASK4B45_D1H_S                                                 24U

// Field: [23:20] M1L
//
// Mask for bits [3:0] in ADI register 5
#define SYS0_MASK4B45_M1L_W                                                  4U
#define SYS0_MASK4B45_M1L_M                                         0x00F00000U
#define SYS0_MASK4B45_M1L_S                                                 20U

// Field: [19:16] D1L
//
// Data for bits [3:0] in ADI register 5, only bits selected by mask M1L will
// be affected by access
#define SYS0_MASK4B45_D1L_W                                                  4U
#define SYS0_MASK4B45_D1L_M                                         0x000F0000U
#define SYS0_MASK4B45_D1L_S                                                 16U

// Field: [15:12] M0H
//
// Mask for bits [7:4] in ADI register 4
#define SYS0_MASK4B45_M0H_W                                                  4U
#define SYS0_MASK4B45_M0H_M                                         0x0000F000U
#define SYS0_MASK4B45_M0H_S                                                 12U

// Field:  [11:8] D0H
//
// Data for bits [7:4] in ADI register 4, only bits selected by mask M0H will
// be affected by access
#define SYS0_MASK4B45_D0H_W                                                  4U
#define SYS0_MASK4B45_D0H_M                                         0x00000F00U
#define SYS0_MASK4B45_D0H_S                                                  8U

// Field:   [7:4] M0L
//
// Mask for bits [3:0] in ADI register 4
#define SYS0_MASK4B45_M0L_W                                                  4U
#define SYS0_MASK4B45_M0L_M                                         0x000000F0U
#define SYS0_MASK4B45_M0L_S                                                  4U

// Field:   [3:0] D0L
//
// Data for bits [3:0] in ADI register 4, only bits selected by mask M0L will
// be affected by access
#define SYS0_MASK4B45_D0L_W                                                  4U
#define SYS0_MASK4B45_D0L_M                                         0x0000000FU
#define SYS0_MASK4B45_D0L_S                                                  0U

//*****************************************************************************
//
// Register: SYS0_O_MASK4B67
//
//*****************************************************************************
// Field: [31:28] M1H
//
// Mask for bits [7:4] in ADI register 7
#define SYS0_MASK4B67_M1H_W                                                  4U
#define SYS0_MASK4B67_M1H_M                                         0xF0000000U
#define SYS0_MASK4B67_M1H_S                                                 28U

// Field: [27:24] D1H
//
// Data for bits [7:4] in ADI register 7, only bits selected by mask M1H will
// be affected by access
#define SYS0_MASK4B67_D1H_W                                                  4U
#define SYS0_MASK4B67_D1H_M                                         0x0F000000U
#define SYS0_MASK4B67_D1H_S                                                 24U

// Field: [23:20] M1L
//
// Mask for bits [3:0] in ADI register 7
#define SYS0_MASK4B67_M1L_W                                                  4U
#define SYS0_MASK4B67_M1L_M                                         0x00F00000U
#define SYS0_MASK4B67_M1L_S                                                 20U

// Field: [19:16] D1L
//
// Data for bits [3:0] in ADI register 7, only bits selected by mask M1L will
// be affected by access
#define SYS0_MASK4B67_D1L_W                                                  4U
#define SYS0_MASK4B67_D1L_M                                         0x000F0000U
#define SYS0_MASK4B67_D1L_S                                                 16U

// Field: [15:12] M0H
//
// Mask for bits [7:4] in ADI register 6
#define SYS0_MASK4B67_M0H_W                                                  4U
#define SYS0_MASK4B67_M0H_M                                         0x0000F000U
#define SYS0_MASK4B67_M0H_S                                                 12U

// Field:  [11:8] D0H
//
// Data for bits [7:4] in ADI register 6, only bits selected by mask M0H will
// be affected by access
#define SYS0_MASK4B67_D0H_W                                                  4U
#define SYS0_MASK4B67_D0H_M                                         0x00000F00U
#define SYS0_MASK4B67_D0H_S                                                  8U

// Field:   [7:4] M0L
//
// Mask for bits [3:0] in ADI register 6
#define SYS0_MASK4B67_M0L_W                                                  4U
#define SYS0_MASK4B67_M0L_M                                         0x000000F0U
#define SYS0_MASK4B67_M0L_S                                                  4U

// Field:   [3:0] D0L
//
// Data for bits [3:0] in ADI register 6, only bits selected by mask M0L will
// be affected by access
#define SYS0_MASK4B67_D0L_W                                                  4U
#define SYS0_MASK4B67_D0L_M                                         0x0000000FU
#define SYS0_MASK4B67_D0L_S                                                  0U

//*****************************************************************************
//
// Register: SYS0_O_MASK4B89
//
//*****************************************************************************
// Field: [31:28] M1H
//
// Mask for bits [7:4] in ADI register 9
#define SYS0_MASK4B89_M1H_W                                                  4U
#define SYS0_MASK4B89_M1H_M                                         0xF0000000U
#define SYS0_MASK4B89_M1H_S                                                 28U

// Field: [27:24] D1H
//
// Data for bits [7:4] in ADI register 9, only bits selected by mask M1H will
// be affected by access
#define SYS0_MASK4B89_D1H_W                                                  4U
#define SYS0_MASK4B89_D1H_M                                         0x0F000000U
#define SYS0_MASK4B89_D1H_S                                                 24U

// Field: [23:20] M1L
//
// Mask for bits [3:0] in ADI register 9
#define SYS0_MASK4B89_M1L_W                                                  4U
#define SYS0_MASK4B89_M1L_M                                         0x00F00000U
#define SYS0_MASK4B89_M1L_S                                                 20U

// Field: [19:16] D1L
//
// Data for bits [3:0] in ADI register 9, only bits selected by mask M1L will
// be affected by access
#define SYS0_MASK4B89_D1L_W                                                  4U
#define SYS0_MASK4B89_D1L_M                                         0x000F0000U
#define SYS0_MASK4B89_D1L_S                                                 16U

// Field: [15:12] M0H
//
// Mask for bits [7:4] in ADI register 8
#define SYS0_MASK4B89_M0H_W                                                  4U
#define SYS0_MASK4B89_M0H_M                                         0x0000F000U
#define SYS0_MASK4B89_M0H_S                                                 12U

// Field:  [11:8] D0H
//
// Data for bits [7:4] in ADI register 8, only bits selected by mask M0H will
// be affected by access
#define SYS0_MASK4B89_D0H_W                                                  4U
#define SYS0_MASK4B89_D0H_M                                         0x00000F00U
#define SYS0_MASK4B89_D0H_S                                                  8U

// Field:   [7:4] M0L
//
// Mask for bits [3:0] in ADI register 8
#define SYS0_MASK4B89_M0L_W                                                  4U
#define SYS0_MASK4B89_M0L_M                                         0x000000F0U
#define SYS0_MASK4B89_M0L_S                                                  4U

// Field:   [3:0] D0L
//
// Data for bits [3:0] in ADI register 8, only bits selected by mask M0L will
// be affected by access
#define SYS0_MASK4B89_D0L_W                                                  4U
#define SYS0_MASK4B89_D0L_M                                         0x0000000FU
#define SYS0_MASK4B89_D0L_S                                                  0U

//*****************************************************************************
//
// Register: SYS0_O_MASK4B1011
//
//*****************************************************************************
// Field: [31:28] M1H
//
// Mask for bits [7:4] in ADI register 11
#define SYS0_MASK4B1011_M1H_W                                                4U
#define SYS0_MASK4B1011_M1H_M                                       0xF0000000U
#define SYS0_MASK4B1011_M1H_S                                               28U

// Field: [27:24] D1H
//
// Data for bits [7:4] in ADI register 11, only bits selected by mask M1H will
// be affected by access
#define SYS0_MASK4B1011_D1H_W                                                4U
#define SYS0_MASK4B1011_D1H_M                                       0x0F000000U
#define SYS0_MASK4B1011_D1H_S                                               24U

// Field: [23:20] M1L
//
// Mask for bits [3:0] in ADI register 11
#define SYS0_MASK4B1011_M1L_W                                                4U
#define SYS0_MASK4B1011_M1L_M                                       0x00F00000U
#define SYS0_MASK4B1011_M1L_S                                               20U

// Field: [19:16] D1L
//
// Data for bits [3:0] in ADI register 11, only bits selected by mask M1L will
// be affected by access
#define SYS0_MASK4B1011_D1L_W                                                4U
#define SYS0_MASK4B1011_D1L_M                                       0x000F0000U
#define SYS0_MASK4B1011_D1L_S                                               16U

// Field: [15:12] M0H
//
// Mask for bits [7:4] in ADI register 10
#define SYS0_MASK4B1011_M0H_W                                                4U
#define SYS0_MASK4B1011_M0H_M                                       0x0000F000U
#define SYS0_MASK4B1011_M0H_S                                               12U

// Field:  [11:8] D0H
//
// Data for bits [7:4] in ADI register 10, only bits selected by mask M0H will
// be affected by access
#define SYS0_MASK4B1011_D0H_W                                                4U
#define SYS0_MASK4B1011_D0H_M                                       0x00000F00U
#define SYS0_MASK4B1011_D0H_S                                                8U

// Field:   [7:4] M0L
//
// Mask for bits [3:0] in ADI register 10
#define SYS0_MASK4B1011_M0L_W                                                4U
#define SYS0_MASK4B1011_M0L_M                                       0x000000F0U
#define SYS0_MASK4B1011_M0L_S                                                4U

// Field:   [3:0] D0L
//
// Data for bits [3:0] in ADI register 10, only bits selected by mask M0L will
// be affected by access
#define SYS0_MASK4B1011_D0L_W                                                4U
#define SYS0_MASK4B1011_D0L_M                                       0x0000000FU
#define SYS0_MASK4B1011_D0L_S                                                0U

//*****************************************************************************
//
// Register: SYS0_O_MASK4B1213
//
//*****************************************************************************
// Field: [31:28] M1H
//
// Mask for bits [7:4] in ADI register 13
#define SYS0_MASK4B1213_M1H_W                                                4U
#define SYS0_MASK4B1213_M1H_M                                       0xF0000000U
#define SYS0_MASK4B1213_M1H_S                                               28U

// Field: [27:24] D1H
//
// Data for bits [7:4] in ADI register 13, only bits selected by mask M1H will
// be affected by access
#define SYS0_MASK4B1213_D1H_W                                                4U
#define SYS0_MASK4B1213_D1H_M                                       0x0F000000U
#define SYS0_MASK4B1213_D1H_S                                               24U

// Field: [23:20] M1L
//
// Mask for bits [3:0] in ADI register 13
#define SYS0_MASK4B1213_M1L_W                                                4U
#define SYS0_MASK4B1213_M1L_M                                       0x00F00000U
#define SYS0_MASK4B1213_M1L_S                                               20U

// Field: [19:16] D1L
//
// Data for bits [3:0] in ADI register 13, only bits selected by mask M1L will
// be affected by access
#define SYS0_MASK4B1213_D1L_W                                                4U
#define SYS0_MASK4B1213_D1L_M                                       0x000F0000U
#define SYS0_MASK4B1213_D1L_S                                               16U

// Field: [15:12] M0H
//
// Mask for bits [7:4] in ADI register 12
#define SYS0_MASK4B1213_M0H_W                                                4U
#define SYS0_MASK4B1213_M0H_M                                       0x0000F000U
#define SYS0_MASK4B1213_M0H_S                                               12U

// Field:  [11:8] D0H
//
// Data for bits [7:4] in ADI register 12, only bits selected by mask M0H will
// be affected by access
#define SYS0_MASK4B1213_D0H_W                                                4U
#define SYS0_MASK4B1213_D0H_M                                       0x00000F00U
#define SYS0_MASK4B1213_D0H_S                                                8U

// Field:   [7:4] M0L
//
// Mask for bits [3:0] in ADI register 12
#define SYS0_MASK4B1213_M0L_W                                                4U
#define SYS0_MASK4B1213_M0L_M                                       0x000000F0U
#define SYS0_MASK4B1213_M0L_S                                                4U

// Field:   [3:0] D0L
//
// Data for bits [3:0] in ADI register 12, only bits selected by mask M0L will
// be affected by access
#define SYS0_MASK4B1213_D0L_W                                                4U
#define SYS0_MASK4B1213_D0L_M                                       0x0000000FU
#define SYS0_MASK4B1213_D0L_S                                                0U

//*****************************************************************************
//
// Register: SYS0_O_MASK4B1415
//
//*****************************************************************************
// Field: [31:28] M1H
//
// Mask for bits [7:4] in ADI register 15
#define SYS0_MASK4B1415_M1H_W                                                4U
#define SYS0_MASK4B1415_M1H_M                                       0xF0000000U
#define SYS0_MASK4B1415_M1H_S                                               28U

// Field: [27:24] D1H
//
// Data for bits [7:4] in ADI register 15, only bits selected by mask M1H will
// be affected by access
#define SYS0_MASK4B1415_D1H_W                                                4U
#define SYS0_MASK4B1415_D1H_M                                       0x0F000000U
#define SYS0_MASK4B1415_D1H_S                                               24U

// Field: [23:20] M1L
//
// Mask for bits [3:0] in ADI register 15
#define SYS0_MASK4B1415_M1L_W                                                4U
#define SYS0_MASK4B1415_M1L_M                                       0x00F00000U
#define SYS0_MASK4B1415_M1L_S                                               20U

// Field: [19:16] D1L
//
// Data for bits [3:0] in ADI register 15, only bits selected by mask M1L will
// be affected by access
#define SYS0_MASK4B1415_D1L_W                                                4U
#define SYS0_MASK4B1415_D1L_M                                       0x000F0000U
#define SYS0_MASK4B1415_D1L_S                                               16U

// Field: [15:12] M0H
//
// Mask for bits [7:4] in ADI register 14
#define SYS0_MASK4B1415_M0H_W                                                4U
#define SYS0_MASK4B1415_M0H_M                                       0x0000F000U
#define SYS0_MASK4B1415_M0H_S                                               12U

// Field:  [11:8] D0H
//
// Data for bits [7:4] in ADI register 14, only bits selected by mask M0H will
// be affected by access
#define SYS0_MASK4B1415_D0H_W                                                4U
#define SYS0_MASK4B1415_D0H_M                                       0x00000F00U
#define SYS0_MASK4B1415_D0H_S                                                8U

// Field:   [7:4] M0L
//
// Mask for bits [3:0] in ADI register 14
#define SYS0_MASK4B1415_M0L_W                                                4U
#define SYS0_MASK4B1415_M0L_M                                       0x000000F0U
#define SYS0_MASK4B1415_M0L_S                                                4U

// Field:   [3:0] D0L
//
// Data for bits [3:0] in ADI register 14, only bits selected by mask M0L will
// be affected by access
#define SYS0_MASK4B1415_D0L_W                                                4U
#define SYS0_MASK4B1415_D0L_M                                       0x0000000FU
#define SYS0_MASK4B1415_D0L_S                                                0U

//*****************************************************************************
//
// Register: SYS0_O_MASK8B01
//
//*****************************************************************************
// Field: [31:24] M1
//
// Mask for ADI register 1
#define SYS0_MASK8B01_M1_W                                                   8U
#define SYS0_MASK8B01_M1_M                                          0xFF000000U
#define SYS0_MASK8B01_M1_S                                                  24U

// Field: [23:16] D1
//
// Data for ADI register 1, only bits selected by mask M1 will be affected by
// access
#define SYS0_MASK8B01_D1_W                                                   8U
#define SYS0_MASK8B01_D1_M                                          0x00FF0000U
#define SYS0_MASK8B01_D1_S                                                  16U

// Field:  [15:8] M0
//
// Mask for ADI register 0
#define SYS0_MASK8B01_M0_W                                                   8U
#define SYS0_MASK8B01_M0_M                                          0x0000FF00U
#define SYS0_MASK8B01_M0_S                                                   8U

// Field:   [7:0] D0
//
// Data for ADI register 0, only bits selected by mask M0 will be affected by
// access
#define SYS0_MASK8B01_D0_W                                                   8U
#define SYS0_MASK8B01_D0_M                                          0x000000FFU
#define SYS0_MASK8B01_D0_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_MASK8B23
//
//*****************************************************************************
// Field: [31:24] M1
//
// Mask for ADI register 3
#define SYS0_MASK8B23_M1_W                                                   8U
#define SYS0_MASK8B23_M1_M                                          0xFF000000U
#define SYS0_MASK8B23_M1_S                                                  24U

// Field: [23:16] D1
//
// Data for ADI register 3, only bits selected by mask M1 will be affected by
// access
#define SYS0_MASK8B23_D1_W                                                   8U
#define SYS0_MASK8B23_D1_M                                          0x00FF0000U
#define SYS0_MASK8B23_D1_S                                                  16U

// Field:  [15:8] M0
//
// Mask for ADI register 2
#define SYS0_MASK8B23_M0_W                                                   8U
#define SYS0_MASK8B23_M0_M                                          0x0000FF00U
#define SYS0_MASK8B23_M0_S                                                   8U

// Field:   [7:0] D0
//
// Data for ADI register 2, only bits selected by mask M0 will be affected by
// access
#define SYS0_MASK8B23_D0_W                                                   8U
#define SYS0_MASK8B23_D0_M                                          0x000000FFU
#define SYS0_MASK8B23_D0_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_MASK8B45
//
//*****************************************************************************
// Field: [31:24] M1
//
// Mask for ADI register 5
#define SYS0_MASK8B45_M1_W                                                   8U
#define SYS0_MASK8B45_M1_M                                          0xFF000000U
#define SYS0_MASK8B45_M1_S                                                  24U

// Field: [23:16] D1
//
// Data for ADI register 5, only bits selected by mask M1 will be affected by
// access
#define SYS0_MASK8B45_D1_W                                                   8U
#define SYS0_MASK8B45_D1_M                                          0x00FF0000U
#define SYS0_MASK8B45_D1_S                                                  16U

// Field:  [15:8] M0
//
// Mask for ADI register 4
#define SYS0_MASK8B45_M0_W                                                   8U
#define SYS0_MASK8B45_M0_M                                          0x0000FF00U
#define SYS0_MASK8B45_M0_S                                                   8U

// Field:   [7:0] D0
//
// Data for ADI register 4, only bits selected by mask M0 will be affected by
// access
#define SYS0_MASK8B45_D0_W                                                   8U
#define SYS0_MASK8B45_D0_M                                          0x000000FFU
#define SYS0_MASK8B45_D0_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_MASK8B67
//
//*****************************************************************************
// Field: [31:24] M1
//
// Mask for ADI register 7
#define SYS0_MASK8B67_M1_W                                                   8U
#define SYS0_MASK8B67_M1_M                                          0xFF000000U
#define SYS0_MASK8B67_M1_S                                                  24U

// Field: [23:16] D1
//
// Data for ADI register 7, only bits selected by mask M1 will be affected by
// access
#define SYS0_MASK8B67_D1_W                                                   8U
#define SYS0_MASK8B67_D1_M                                          0x00FF0000U
#define SYS0_MASK8B67_D1_S                                                  16U

// Field:  [15:8] M0
//
// Mask for ADI register 6
#define SYS0_MASK8B67_M0_W                                                   8U
#define SYS0_MASK8B67_M0_M                                          0x0000FF00U
#define SYS0_MASK8B67_M0_S                                                   8U

// Field:   [7:0] D0
//
// Data for ADI register 6, only bits selected by mask M0 will be affected by
// access
#define SYS0_MASK8B67_D0_W                                                   8U
#define SYS0_MASK8B67_D0_M                                          0x000000FFU
#define SYS0_MASK8B67_D0_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_MASK8B89
//
//*****************************************************************************
// Field: [31:24] M1
//
// Mask for ADI register 9
#define SYS0_MASK8B89_M1_W                                                   8U
#define SYS0_MASK8B89_M1_M                                          0xFF000000U
#define SYS0_MASK8B89_M1_S                                                  24U

// Field: [23:16] D1
//
// Data for ADI register 9, only bits selected by mask M1 will be affected by
// access
#define SYS0_MASK8B89_D1_W                                                   8U
#define SYS0_MASK8B89_D1_M                                          0x00FF0000U
#define SYS0_MASK8B89_D1_S                                                  16U

// Field:  [15:8] M0
//
// Mask for ADI register 8
#define SYS0_MASK8B89_M0_W                                                   8U
#define SYS0_MASK8B89_M0_M                                          0x0000FF00U
#define SYS0_MASK8B89_M0_S                                                   8U

// Field:   [7:0] D0
//
// Data for ADI register 8, only bits selected by mask M0 will be affected by
// access
#define SYS0_MASK8B89_D0_W                                                   8U
#define SYS0_MASK8B89_D0_M                                          0x000000FFU
#define SYS0_MASK8B89_D0_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_MASK8B1011
//
//*****************************************************************************
// Field: [31:24] M1
//
// Mask for ADI register 11
#define SYS0_MASK8B1011_M1_W                                                 8U
#define SYS0_MASK8B1011_M1_M                                        0xFF000000U
#define SYS0_MASK8B1011_M1_S                                                24U

// Field: [23:16] D1
//
// Data for ADI register 11, only bits selected by mask M1 will be affected by
// access
#define SYS0_MASK8B1011_D1_W                                                 8U
#define SYS0_MASK8B1011_D1_M                                        0x00FF0000U
#define SYS0_MASK8B1011_D1_S                                                16U

// Field:  [15:8] M0
//
// Mask for ADI register 10
#define SYS0_MASK8B1011_M0_W                                                 8U
#define SYS0_MASK8B1011_M0_M                                        0x0000FF00U
#define SYS0_MASK8B1011_M0_S                                                 8U

// Field:   [7:0] D0
//
// Data for ADI register 10, only bits selected by mask M0 will be affected by
// access
#define SYS0_MASK8B1011_D0_W                                                 8U
#define SYS0_MASK8B1011_D0_M                                        0x000000FFU
#define SYS0_MASK8B1011_D0_S                                                 0U

//*****************************************************************************
//
// Register: SYS0_O_MASK8B1213
//
//*****************************************************************************
// Field: [31:24] M1
//
// Mask for ADI register 13
#define SYS0_MASK8B1213_M1_W                                                 8U
#define SYS0_MASK8B1213_M1_M                                        0xFF000000U
#define SYS0_MASK8B1213_M1_S                                                24U

// Field: [23:16] D1
//
// Data for ADI register 13, only bits selected by mask M1 will be affected by
// access
#define SYS0_MASK8B1213_D1_W                                                 8U
#define SYS0_MASK8B1213_D1_M                                        0x00FF0000U
#define SYS0_MASK8B1213_D1_S                                                16U

// Field:  [15:8] M0
//
// Mask for ADI register 12
#define SYS0_MASK8B1213_M0_W                                                 8U
#define SYS0_MASK8B1213_M0_M                                        0x0000FF00U
#define SYS0_MASK8B1213_M0_S                                                 8U

// Field:   [7:0] D0
//
// Data for ADI register 12, only bits selected by mask M0 will be affected by
// access
#define SYS0_MASK8B1213_D0_W                                                 8U
#define SYS0_MASK8B1213_D0_M                                        0x000000FFU
#define SYS0_MASK8B1213_D0_S                                                 0U

//*****************************************************************************
//
// Register: SYS0_O_MASK8B1415
//
//*****************************************************************************
// Field: [31:24] M1
//
// Mask for ADI register 15
#define SYS0_MASK8B1415_M1_W                                                 8U
#define SYS0_MASK8B1415_M1_M                                        0xFF000000U
#define SYS0_MASK8B1415_M1_S                                                24U

// Field: [23:16] D1
//
// Data for ADI register 15, only bits selected by mask M1 will be affected by
// access
#define SYS0_MASK8B1415_D1_W                                                 8U
#define SYS0_MASK8B1415_D1_M                                        0x00FF0000U
#define SYS0_MASK8B1415_D1_S                                                16U

// Field:  [15:8] M0
//
// Mask for ADI register 14
#define SYS0_MASK8B1415_M0_W                                                 8U
#define SYS0_MASK8B1415_M0_M                                        0x0000FF00U
#define SYS0_MASK8B1415_M0_S                                                 8U

// Field:   [7:0] D0
//
// Data for ADI register 14, only bits selected by mask M0 will be affected by
// access
#define SYS0_MASK8B1415_D0_W                                                 8U
#define SYS0_MASK8B1415_D0_M                                        0x000000FFU
#define SYS0_MASK8B1415_D0_S                                                 0U

//*****************************************************************************
//
// Register: SYS0_O_MASK16B01
//
//*****************************************************************************
// Field: [31:16] M
//
// Mask for ADI register 0 and 1
#define SYS0_MASK16B01_M_W                                                  16U
#define SYS0_MASK16B01_M_M                                          0xFFFF0000U
#define SYS0_MASK16B01_M_S                                                  16U

// Field:  [15:0] D
//
// Data for ADI register 0 and 1, only bits selected by mask M will be affected
// by access
#define SYS0_MASK16B01_D_W                                                  16U
#define SYS0_MASK16B01_D_M                                          0x0000FFFFU
#define SYS0_MASK16B01_D_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_MASK16B23
//
//*****************************************************************************
// Field: [31:16] M
//
// Mask for ADI register 2 and 3
#define SYS0_MASK16B23_M_W                                                  16U
#define SYS0_MASK16B23_M_M                                          0xFFFF0000U
#define SYS0_MASK16B23_M_S                                                  16U

// Field:  [15:0] D
//
// Data for ADI register 2 and 3, only bits selected by mask M will be affected
// by access
#define SYS0_MASK16B23_D_W                                                  16U
#define SYS0_MASK16B23_D_M                                          0x0000FFFFU
#define SYS0_MASK16B23_D_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_MASK16B45
//
//*****************************************************************************
// Field: [31:16] M
//
// Mask for ADI register 4 and 5
#define SYS0_MASK16B45_M_W                                                  16U
#define SYS0_MASK16B45_M_M                                          0xFFFF0000U
#define SYS0_MASK16B45_M_S                                                  16U

// Field:  [15:0] D
//
// Data for ADI register 4 and 5, only bits selected by mask M will be affected
// by access
#define SYS0_MASK16B45_D_W                                                  16U
#define SYS0_MASK16B45_D_M                                          0x0000FFFFU
#define SYS0_MASK16B45_D_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_MASK16B67
//
//*****************************************************************************
// Field: [31:16] M
//
// Mask for ADI register 6 and 7
#define SYS0_MASK16B67_M_W                                                  16U
#define SYS0_MASK16B67_M_M                                          0xFFFF0000U
#define SYS0_MASK16B67_M_S                                                  16U

// Field:  [15:0] D
//
// Data for ADI register 6 and 7, only bits selected by mask M will be affected
// by access
#define SYS0_MASK16B67_D_W                                                  16U
#define SYS0_MASK16B67_D_M                                          0x0000FFFFU
#define SYS0_MASK16B67_D_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_MASK16B89
//
//*****************************************************************************
// Field: [31:16] M
//
// Mask for ADI register 8 and 9
#define SYS0_MASK16B89_M_W                                                  16U
#define SYS0_MASK16B89_M_M                                          0xFFFF0000U
#define SYS0_MASK16B89_M_S                                                  16U

// Field:  [15:0] D
//
// Data for ADI register 8 and 9, only bits selected by mask M will be affected
// by access
#define SYS0_MASK16B89_D_W                                                  16U
#define SYS0_MASK16B89_D_M                                          0x0000FFFFU
#define SYS0_MASK16B89_D_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_MASK16B1011
//
//*****************************************************************************
// Field: [31:16] M
//
// Mask for ADI register 10 and 11
#define SYS0_MASK16B1011_M_W                                                16U
#define SYS0_MASK16B1011_M_M                                        0xFFFF0000U
#define SYS0_MASK16B1011_M_S                                                16U

// Field:  [15:0] D
//
// Data for ADI register 10 and 11, only bits selected by mask M will be
// affected by access
#define SYS0_MASK16B1011_D_W                                                16U
#define SYS0_MASK16B1011_D_M                                        0x0000FFFFU
#define SYS0_MASK16B1011_D_S                                                 0U

//*****************************************************************************
//
// Register: SYS0_O_MASK16B1213
//
//*****************************************************************************
// Field: [31:16] M
//
// Mask for ADI register 12 and 13
#define SYS0_MASK16B1213_M_W                                                16U
#define SYS0_MASK16B1213_M_M                                        0xFFFF0000U
#define SYS0_MASK16B1213_M_S                                                16U

// Field:  [15:0] D
//
// Data for ADI register 12 and 13, only bits selected by mask M will be
// affected by access
#define SYS0_MASK16B1213_D_W                                                16U
#define SYS0_MASK16B1213_D_M                                        0x0000FFFFU
#define SYS0_MASK16B1213_D_S                                                 0U

//*****************************************************************************
//
// Register: SYS0_O_MASK16B1415
//
//*****************************************************************************
// Field: [31:16] M
//
// Mask for ADI register 14 and 15
#define SYS0_MASK16B1415_M_W                                                16U
#define SYS0_MASK16B1415_M_M                                        0xFFFF0000U
#define SYS0_MASK16B1415_M_S                                                16U

// Field:  [15:0] D
//
// Data for ADI register 14 and 15, only bits selected by mask M will be
// affected by access
#define SYS0_MASK16B1415_D_W                                                16U
#define SYS0_MASK16B1415_D_M                                        0x0000FFFFU
#define SYS0_MASK16B1415_D_S                                                 0U


#endif // __SYS0__
