/******************************************************************************
*  Filename:       hw_sys0_h
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

#ifndef __HW_SYS0_H__
#define __HW_SYS0_H__

//*****************************************************************************
//
// This section defines the register offsets of
// SYS0 component
//
//*****************************************************************************
// This register identifies the peripheral
#define SYS0_O_DESC                                                 0x00000000U

// This register identifies the configuration of the peripheral
#define SYS0_O_DESCEX                                               0x00000004U

// Global Lock Register. Locks both registers in both mutable and immutable
// sections
#define SYS0_O_GLOCK                                                0x00000008U

// Unlocks registers in mutable section
#define SYS0_O_MUNLOCK                                              0x0000000CU

// Lock Status Register
#define SYS0_O_STATUS                                               0x00000010U

// This register is used to configure analog switches in ATEST module
#define SYS0_O_ATESTCFG                                             0x00000100U

// This register is used to configure I2V (current to voltage converter)
// module.
#define SYS0_O_I2VCFG                                               0x00000104U

// This register is used to configure temperature sensor module.
#define SYS0_O_TSENSCFG                                             0x00000108U

// This register is used to configure and check the status of low-power
// comparator (LPCOMP) module.
#define SYS0_O_LPCMPCFG                                             0x0000010CU

// This register is used to configure the VGM module.
#define SYS0_O_VGMCFG                                               0x00000110U

// This register is used to trim and debug VGM module.
#define SYS0_O_VGMTRIMDBG                                           0x00000114U

// This register is used to set or clear the bits in 3V register bank for TSD
// comparator test control.
#define SYS0_O_TSDTEST                                              0x00000120U

// This register provides Device ID information.
#define SYS0_O_DEVICEID                                             0x000003FCU

// TIMMUTE0 trim Register
#define SYS0_O_TIMMUTE0                                             0x00000400U

// TIMMUTE1 trim Register
#define SYS0_O_TIMMUTE1                                             0x00000404U

// TIMMUTE2 trim Register
#define SYS0_O_TIMMUTE2                                             0x00000408U

// TIMMUTE3 trim Register
#define SYS0_O_TIMMUTE3                                             0x0000040CU

// This register is programmed by boot code with Part ID information.Note: This
// 32-bit register value is provided as output to DEBUGSS
#define SYS0_O_PARTID                                               0x000007F8U

// This register is programmed by boot code with device life cycle information
#define SYS0_O_LIFECYC                                              0x000007FCU

// TMUTE0 trim Register
#define SYS0_O_TMUTE0                                               0x00000800U

// TMUTE1 trim Register
#define SYS0_O_TMUTE1                                               0x00000804U

// TMUTE2 trim Register
#define SYS0_O_TMUTE2                                               0x00000808U

// TMUTE3 trim Register
#define SYS0_O_TMUTE3                                               0x0000080CU

// TMUTE4 trim Register
#define SYS0_O_TMUTE4                                               0x00000810U

// TMUTE5 trim Register
#define SYS0_O_TMUTE5                                               0x00000814U

//*****************************************************************************
//
// Register: SYS0_O_DESC
//
//*****************************************************************************
// Field: [31:16] MODID
//
// Module identification contains a unique peripheral identification number.
#define SYS0_DESC_MODID_W                                                   16U
#define SYS0_DESC_MODID_M                                           0xFFFF0000U
#define SYS0_DESC_MODID_S                                                   16U

// Field: [15:12] STDIPOFF
//
// Standard IP registers offset. Value 0 indicates Standard IP registers are
// not present. Any other value between 1 to 15 indicates standard IP registers
// start from address offset 64 * STDIPOFF from base address.
#define SYS0_DESC_STDIPOFF_W                                                 4U
#define SYS0_DESC_STDIPOFF_M                                        0x0000F000U
#define SYS0_DESC_STDIPOFF_S                                                12U

// Field:  [11:8] INSTIDX
//
// Instance Index within the device. This will be a parameter to the RTL for
// modules that can have multiple instances.
#define SYS0_DESC_INSTIDX_W                                                  4U
#define SYS0_DESC_INSTIDX_M                                         0x00000F00U
#define SYS0_DESC_INSTIDX_S                                                  8U

// Field:   [7:4] MAJREV
//
// Major revision of the IP
#define SYS0_DESC_MAJREV_W                                                   4U
#define SYS0_DESC_MAJREV_M                                          0x000000F0U
#define SYS0_DESC_MAJREV_S                                                   4U

// Field:   [3:0] MINREV
//
// Minor revision of the IP
#define SYS0_DESC_MINREV_W                                                   4U
#define SYS0_DESC_MINREV_M                                          0x0000000FU
#define SYS0_DESC_MINREV_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_DESCEX
//
//*****************************************************************************
// Field:  [15:0] TBITS
//
// Total number of trim bits
#define SYS0_DESCEX_TBITS_W                                                 16U
#define SYS0_DESCEX_TBITS_M                                         0x0000FFFFU
#define SYS0_DESCEX_TBITS_S                                                  0U

//*****************************************************************************
//
// Register: SYS0_O_GLOCK
//
//*****************************************************************************
// Field:     [0] LOCK
//
// When LOCK is set, write access to registers in mutable and immutable section
// are not allowed. Registers in mutable section can be temporarily unlocked by
// writing the KEY to UNLOCK.KEY
// ENUMs:
// SET                      Global Lock is set
// NOEFFECT                 No effect
#define SYS0_GLOCK_LOCK                                             0x00000001U
#define SYS0_GLOCK_LOCK_M                                           0x00000001U
#define SYS0_GLOCK_LOCK_S                                                    0U
#define SYS0_GLOCK_LOCK_SET                                         0x00000001U
#define SYS0_GLOCK_LOCK_NOEFFECT                                    0x00000000U

//*****************************************************************************
//
// Register: SYS0_O_MUNLOCK
//
//*****************************************************************************
// Field:  [31:0] KEY
//
// Write the unlock key 0xC5AF_6927 to temporarily unlock registers in mutable
// section. The lock is set automatically if no write accesses, to the mutable
// section, is detected for consecutive 32 CLKULL (24MHz) clock cycles. Writing
// any value other that the unlock key will immediately lock the mutable
// register space for write access.
#define SYS0_MUNLOCK_KEY_W                                                  32U
#define SYS0_MUNLOCK_KEY_M                                          0xFFFFFFFFU
#define SYS0_MUNLOCK_KEY_S                                                   0U

//*****************************************************************************
//
// Register: SYS0_O_STATUS
//
//*****************************************************************************
// Field:    [31] VGMTIE
//
// Internal VGM Enable Signal
#define SYS0_STATUS_VGMTIE                                          0x80000000U
#define SYS0_STATUS_VGMTIE_M                                        0x80000000U
#define SYS0_STATUS_VGMTIE_S                                                31U

// Field:    [30] EMTIE
//
// Internal EM Sensor Enable Signal
#define SYS0_STATUS_EMTIE                                           0x40000000U
#define SYS0_STATUS_EMTIE_M                                         0x40000000U
#define SYS0_STATUS_EMTIE_S                                                 30U

// Field:     [1] MLOCK
//
// Lock status of registers in mutable section.
// ENUMs:
// LOCKED                   Registers in the mutable section are locked for
//                          write
// UNLOCKED                 Registers in the mutable section are not locked,
//                          and are writable
#define SYS0_STATUS_MLOCK                                           0x00000002U
#define SYS0_STATUS_MLOCK_M                                         0x00000002U
#define SYS0_STATUS_MLOCK_S                                                  1U
#define SYS0_STATUS_MLOCK_LOCKED                                    0x00000002U
#define SYS0_STATUS_MLOCK_UNLOCKED                                  0x00000000U

// Field:     [0] GLOCK
//
// Status of Global lock
// ENUMs:
// LOCKED                   Locked
// UNLOCKED                 Unlocked
#define SYS0_STATUS_GLOCK                                           0x00000001U
#define SYS0_STATUS_GLOCK_M                                         0x00000001U
#define SYS0_STATUS_GLOCK_S                                                  0U
#define SYS0_STATUS_GLOCK_LOCKED                                    0x00000001U
#define SYS0_STATUS_GLOCK_UNLOCKED                                  0x00000000U

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
// VDDA                     selects VDDA
// VDDBST                   selects VDDBOOST
#define SYS0_ATESTCFG_VSEL                                          0x00000100U
#define SYS0_ATESTCFG_VSEL_M                                        0x00000100U
#define SYS0_ATESTCFG_VSEL_S                                                 8U
#define SYS0_ATESTCFG_VSEL_VDDA                                     0x00000100U
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
// Register: SYS0_O_I2VCFG
//
//*****************************************************************************
// Field:   [3:1] RESVAL
//
// Enables I2V module and selects resistor value.
// ENUMs:
// OFF_RES                  I2V module is disabled
// MO1                      resistor value is 1MOhm
// KO100                    resistor value is 100kOhm
// KO50                     resistor value is 50kOhm
// KO20                     resistor value is 20kOhm
// KO10                     resistor value is 10kOhm
// KO2                      resistor value is 2kOhm
// OFF                      I2V module is disabled
#define SYS0_I2VCFG_RESVAL_W                                                 3U
#define SYS0_I2VCFG_RESVAL_M                                        0x0000000EU
#define SYS0_I2VCFG_RESVAL_S                                                 1U
#define SYS0_I2VCFG_RESVAL_OFF_RES                                  0x0000000EU
#define SYS0_I2VCFG_RESVAL_MO1                                      0x0000000CU
#define SYS0_I2VCFG_RESVAL_KO100                                    0x0000000AU
#define SYS0_I2VCFG_RESVAL_KO50                                     0x00000008U
#define SYS0_I2VCFG_RESVAL_KO20                                     0x00000006U
#define SYS0_I2VCFG_RESVAL_KO10                                     0x00000004U
#define SYS0_I2VCFG_RESVAL_KO2                                      0x00000002U
#define SYS0_I2VCFG_RESVAL_OFF                                      0x00000000U

// Field:     [0] SEL
//
// Selects ATEST_A0 between VR and VA domains and forwards to ATEST.
// ENUMs:
// VA_ATEST_A0              selects VA_ATEST_A0
// VR_ATEST_A0              selects VR_ATEST_A0
#define SYS0_I2VCFG_SEL                                             0x00000001U
#define SYS0_I2VCFG_SEL_M                                           0x00000001U
#define SYS0_I2VCFG_SEL_S                                                    0U
#define SYS0_I2VCFG_SEL_VA_ATEST_A0                                 0x00000001U
#define SYS0_I2VCFG_SEL_VR_ATEST_A0                                 0x00000000U

//*****************************************************************************
//
// Register: SYS0_O_TSENSCFG
//
//*****************************************************************************
// Field: [31:26] SPARE1
//
// Spare bits
// ENUMs:
// MAX                      maximum value
// MIN                      minimum value
#define SYS0_TSENSCFG_SPARE1_W                                               6U
#define SYS0_TSENSCFG_SPARE1_M                                      0xFC000000U
#define SYS0_TSENSCFG_SPARE1_S                                              26U
#define SYS0_TSENSCFG_SPARE1_MAX                                    0xFC000000U
#define SYS0_TSENSCFG_SPARE1_MIN                                    0x00000000U

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

// Field:  [10:8] SPARE
//
// Spare bits
// ENUMs:
// MAX                      maximum value
// MIN                      minimum value
#define SYS0_TSENSCFG_SPARE_W                                                3U
#define SYS0_TSENSCFG_SPARE_M                                       0x00000700U
#define SYS0_TSENSCFG_SPARE_S                                                8U
#define SYS0_TSENSCFG_SPARE_MAX                                     0x00000F00U
#define SYS0_TSENSCFG_SPARE_MIN                                     0x00000000U

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
// Spare bit for LPCOMP
#define SYS0_LPCMPCFG_HYSPOL                                        0x40000000U
#define SYS0_LPCMPCFG_HYSPOL_M                                      0x40000000U
#define SYS0_LPCMPCFG_HYSPOL_S                                              30U

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
// Event flag. The event flag is set when the comparator output transition is
// qualified based on the edge polarity configuration in EDGCFG.
// ENUMs:
// SET
// CLR                      clear
#define SYS0_LPCMPCFG_EVTIFG                                        0x01000000U
#define SYS0_LPCMPCFG_EVTIFG_M                                      0x01000000U
#define SYS0_LPCMPCFG_EVTIFG_S                                              24U
#define SYS0_LPCMPCFG_EVTIFG_SET                                    0x01000000U
#define SYS0_LPCMPCFG_EVTIFG_CLR                                    0x00000000U

// Field:    [21] COUTEN
//
// Enables LPCOMP output on device pin.
// ENUMs:
// ENABLED                  Enabled
// DISABLED                 Disabled
#define SYS0_LPCMPCFG_COUTEN                                        0x00200000U
#define SYS0_LPCMPCFG_COUTEN_M                                      0x00200000U
#define SYS0_LPCMPCFG_COUTEN_S                                              21U
#define SYS0_LPCMPCFG_COUTEN_ENABLED                                0x00200000U
#define SYS0_LPCMPCFG_COUTEN_DISABLED                               0x00000000U

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
// ENABLE                   Enable
// DISABLE                  Disable
#define SYS0_LPCMPCFG_WUENSB                                        0x00040000U
#define SYS0_LPCMPCFG_WUENSB_M                                      0x00040000U
#define SYS0_LPCMPCFG_WUENSB_S                                              18U
#define SYS0_LPCMPCFG_WUENSB_ENABLE                                 0x00040000U
#define SYS0_LPCMPCFG_WUENSB_DISABLE                                0x00000000U

// Field:    [17] EVTEN
//
// Enables event generation. Comparator module will produce event on ULL event
// fabric when EVTIFG is set.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define SYS0_LPCMPCFG_EVTEN                                         0x00020000U
#define SYS0_LPCMPCFG_EVTEN_M                                       0x00020000U
#define SYS0_LPCMPCFG_EVTEN_S                                               17U
#define SYS0_LPCMPCFG_EVTEN_ENABLE                                  0x00020000U
#define SYS0_LPCMPCFG_EVTEN_DISABLE                                 0x00000000U

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
// VDDD                     selects VDDD
// VDDA                     selects VDDA
// VA_PAD_A3                selects VA_PAD_A3
// VA_PAD_A2                selects VA_PAD_A2
// OPEN                     all switches are open
#define SYS0_LPCMPCFG_NSEL_W                                                 3U
#define SYS0_LPCMPCFG_NSEL_M                                        0x00007000U
#define SYS0_LPCMPCFG_NSEL_S                                                12U
#define SYS0_LPCMPCFG_NSEL_VDDD                                     0x00004000U
#define SYS0_LPCMPCFG_NSEL_VDDA                                     0x00003000U
#define SYS0_LPCMPCFG_NSEL_VA_PAD_A3                                0x00002000U
#define SYS0_LPCMPCFG_NSEL_VA_PAD_A2                                0x00001000U
#define SYS0_LPCMPCFG_NSEL_OPEN                                     0x00000000U

// Field:  [11:8] PSEL
//
// Positive input selection. Setting values 0x9-0xF will open all the switches.
// ENUMs:
// VDDA                     selects VDDA
// VA_ATEST_A1              selects VA_ATEST_A1
// VA_ATEST_A0              selects VA_ATEST_A0
// VR_ATEST_A1              selects VR_ATEST_A1
// VR_ATEST_A0              selects VR_ATEST_A0
// VA_PAD_A3                selects VA_PAD_A3
// VA_PAD_A2                selects VA_PAD_A2
// VA_PAD_A1                selects VA_PAD_A1
// OPEN                     all switches are open
#define SYS0_LPCMPCFG_PSEL_W                                                 4U
#define SYS0_LPCMPCFG_PSEL_M                                        0x00000F00U
#define SYS0_LPCMPCFG_PSEL_S                                                 8U
#define SYS0_LPCMPCFG_PSEL_VDDA                                     0x00000800U
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
// Used to enable and select hysteresis level.
// ENUMs:
// VAL7                     hysteresis value: TBD
// VAL6                     hysteresis value: TBD
// VAL5                     hysteresis value: TBD
// VAL4                     hysteresis value: TBD
// VAL3                     hysteresis value: TBD
// VAL2                     hysteresis value: TBD
// VAL1                     hysteresis value: TBD
// VAL0                     hysteresis is disabled
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
// PSIDE                    divider is applied on P-side
// NSIDE                    divider is applied on N-side
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
// VAL4                     divide value is 1/4
// VAL3                     divide value is 1/3
// VAL2                     divide value is 1/2
// VAL1                     divide value is 3/4
// VAL0                     divide value is 1
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
// ENABLE                   Enable
// DISABLE                  Disable
#define SYS0_LPCMPCFG_EN                                            0x00000001U
#define SYS0_LPCMPCFG_EN_M                                          0x00000001U
#define SYS0_LPCMPCFG_EN_S                                                   0U
#define SYS0_LPCMPCFG_EN_ENABLE                                     0x00000001U
#define SYS0_LPCMPCFG_EN_DISABLE                                    0x00000000U

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

// Field: [19:16] SPARE
//
// Spare bits
#define SYS0_VGMCFG_SPARE_W                                                  4U
#define SYS0_VGMCFG_SPARE_M                                         0x000F0000U
#define SYS0_VGMCFG_SPARE_S                                                 16U

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
// Register: SYS0_O_TSDTEST
//
//*****************************************************************************
// Field:   [5:3] ATBCLR
//
// Clears ATB mux select in TSD comparator.
#define SYS0_TSDTEST_ATBCLR_W                                                3U
#define SYS0_TSDTEST_ATBCLR_M                                       0x00000038U
#define SYS0_TSDTEST_ATBCLR_S                                                3U

// Field:   [2:0] ATBSET
//
// Sets ATB mux select in TSD comparator.
// ENUMs:
// RESERVED                 Not used
// TSDOUT                   Brings out final TSD output before gating based on
//                          mux select bits
// COMPOUT                  Brings out comparator output
// EN_DELAY                 Brings out delayed enable signal
// EA_CUR                   Brings out 100nA current from error amplifier
// VREF                     Used for VREF trimming
// VSENSE                   Used for VSENSE trimming
// OPEN                     ATB is disabled. All switches are open in ATB mux
#define SYS0_TSDTEST_ATBSET_W                                                3U
#define SYS0_TSDTEST_ATBSET_M                                       0x00000007U
#define SYS0_TSDTEST_ATBSET_S                                                0U
#define SYS0_TSDTEST_ATBSET_RESERVED                                0x00000007U
#define SYS0_TSDTEST_ATBSET_TSDOUT                                  0x00000006U
#define SYS0_TSDTEST_ATBSET_COMPOUT                                 0x00000005U
#define SYS0_TSDTEST_ATBSET_EN_DELAY                                0x00000004U
#define SYS0_TSDTEST_ATBSET_EA_CUR                                  0x00000003U
#define SYS0_TSDTEST_ATBSET_VREF                                    0x00000002U
#define SYS0_TSDTEST_ATBSET_VSENSE                                  0x00000001U
#define SYS0_TSDTEST_ATBSET_OPEN                                    0x00000000U

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
// Register: SYS0_O_TIMMUTE0
//
//*****************************************************************************
// Field:    [27] GLDODISANA
//
// PMUREG: GLDO disable control selection between digital and analog schemes.
// 0: digital control, 1: analog control.
#define SYS0_TIMMUTE0_GLDODISANA                                    0x08000000U
#define SYS0_TIMMUTE0_GLDODISANA_M                                  0x08000000U
#define SYS0_TIMMUTE0_GLDODISANA_S                                          27U

// Field:    [26] GLDOCOMPDIS
//
// PMUREG: Disables DCDC load meter based comparison and GLDO turn off scheme.
#define SYS0_TIMMUTE0_GLDOCOMPDIS                                   0x04000000U
#define SYS0_TIMMUTE0_GLDOCOMPDIS_M                                 0x04000000U
#define SYS0_TIMMUTE0_GLDOCOMPDIS_S                                         26U

// Field:    [25] DISSAHYST
//
// PMUREG: Disable hysteresis in the supply awareness circuit.
#define SYS0_TIMMUTE0_DISSAHYST                                     0x02000000U
#define SYS0_TIMMUTE0_DISSAHYST_M                                   0x02000000U
#define SYS0_TIMMUTE0_DISSAHYST_S                                           25U

// Field:    [24] VDDROKHYST
//
// PMUREG: Increase the hysteresis for when VDDR is considered ok.
#define SYS0_TIMMUTE0_VDDROKHYST                                    0x01000000U
#define SYS0_TIMMUTE0_VDDROKHYST_M                                  0x01000000U
#define SYS0_TIMMUTE0_VDDROKHYST_S                                          24U

// Field:    [23] SPARE
//
// REFSYS: Spare
#define SYS0_TIMMUTE0_SPARE                                         0x00800000U
#define SYS0_TIMMUTE0_SPARE_M                                       0x00800000U
#define SYS0_TIMMUTE0_SPARE_S                                               23U

// Field: [22:21] TSENSE
//
// REFSYS: Temperature sensor trim
#define SYS0_TIMMUTE0_TSENSE_W                                               2U
#define SYS0_TIMMUTE0_TSENSE_M                                      0x00600000U
#define SYS0_TIMMUTE0_TSENSE_S                                              21U

// Field: [20:16] IREF
//
// REFSYS: IREF trim
#define SYS0_TIMMUTE0_IREF_W                                                 5U
#define SYS0_TIMMUTE0_IREF_M                                        0x001F0000U
#define SYS0_TIMMUTE0_IREF_S                                                16U

// Field:    [15] BGTRIMEN
//
// REFSYS: BOD BG trim enable
#define SYS0_TIMMUTE0_BGTRIMEN                                      0x00008000U
#define SYS0_TIMMUTE0_BGTRIMEN_M                                    0x00008000U
#define SYS0_TIMMUTE0_BGTRIMEN_S                                            15U

// Field: [14:10] VDDSBOD
//
// REFSYS: VDDS BOD trim
#define SYS0_TIMMUTE0_VDDSBOD_W                                              5U
#define SYS0_TIMMUTE0_VDDSBOD_M                                     0x00007C00U
#define SYS0_TIMMUTE0_VDDSBOD_S                                             10U

// Field:   [9:4] VBG
//
// REFSYS: VBG trim
#define SYS0_TIMMUTE0_VBG_W                                                  6U
#define SYS0_TIMMUTE0_VBG_M                                         0x000003F0U
#define SYS0_TIMMUTE0_VBG_S                                                  4U

// Field:   [3:0] VREF
//
// REFSYS: VREF trim
#define SYS0_TIMMUTE0_VREF_W                                                 4U
#define SYS0_TIMMUTE0_VREF_M                                        0x0000000FU
#define SYS0_TIMMUTE0_VREF_S                                                 0U

//*****************************************************************************
//
// Register: SYS0_O_TIMMUTE1
//
//*****************************************************************************
// Field: [30:28] DELTA
//
// PMUREG: Delta trim
#define SYS0_TIMMUTE1_DELTA_W                                                3U
#define SYS0_TIMMUTE1_DELTA_M                                       0x70000000U
#define SYS0_TIMMUTE1_DELTA_S                                               28U

// Field: [27:24] COARSE
//
// PMUREG: Coarse trim
#define SYS0_TIMMUTE1_COARSE_W                                               4U
#define SYS0_TIMMUTE1_COARSE_M                                      0x0F000000U
#define SYS0_TIMMUTE1_COARSE_S                                              24U

// Field: [23:20] DIG
//
// PMUREG: DIG LDO trim
#define SYS0_TIMMUTE1_DIG_W                                                  4U
#define SYS0_TIMMUTE1_DIG_M                                         0x00F00000U
#define SYS0_TIMMUTE1_DIG_S                                                 20U

// Field: [19:16] UDIG
//
// PMUREG: UDIG LDO trim
#define SYS0_TIMMUTE1_UDIG_W                                                 4U
#define SYS0_TIMMUTE1_UDIG_M                                        0x000F0000U
#define SYS0_TIMMUTE1_UDIG_S                                                16U

// Field: [15:12] BOD
//
// PMUREG: BOD trim
#define SYS0_TIMMUTE1_BOD_W                                                  4U
#define SYS0_TIMMUTE1_BOD_M                                         0x0000F000U
#define SYS0_TIMMUTE1_BOD_S                                                 12U

// Field:    [11] EN_LOW_IQQ_GLDO
//
// This bit is used to enable low IQQ mode for GLDO.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define SYS0_TIMMUTE1_EN_LOW_IQQ_GLDO                               0x00000800U
#define SYS0_TIMMUTE1_EN_LOW_IQQ_GLDO_M                             0x00000800U
#define SYS0_TIMMUTE1_EN_LOW_IQQ_GLDO_S                                     11U
#define SYS0_TIMMUTE1_EN_LOW_IQQ_GLDO_ENABLE                        0x00000800U
#define SYS0_TIMMUTE1_EN_LOW_IQQ_GLDO_DISABLE                       0x00000000U

// Field:    [10] DIS_HS_DCDC_COMP
//
//  This bit is used to disable high speed DCDC comparator.
// ENUMs:
// DISABLE                  Disable
// ENABLE                   Enable
#define SYS0_TIMMUTE1_DIS_HS_DCDC_COMP                              0x00000400U
#define SYS0_TIMMUTE1_DIS_HS_DCDC_COMP_M                            0x00000400U
#define SYS0_TIMMUTE1_DIS_HS_DCDC_COMP_S                                    10U
#define SYS0_TIMMUTE1_DIS_HS_DCDC_COMP_DISABLE                      0x00000400U
#define SYS0_TIMMUTE1_DIS_HS_DCDC_COMP_ENABLE                       0x00000000U

// Field:   [9:5] VDDRSL
//
// PMUREG: VDDR sleep trim
#define SYS0_TIMMUTE1_VDDRSL_W                                               5U
#define SYS0_TIMMUTE1_VDDRSL_M                                      0x000003E0U
#define SYS0_TIMMUTE1_VDDRSL_S                                               5U

// Field:   [4:0] VDDR
//
// PMUREG: VDDR trim
#define SYS0_TIMMUTE1_VDDR_W                                                 5U
#define SYS0_TIMMUTE1_VDDR_M                                        0x0000001FU
#define SYS0_TIMMUTE1_VDDR_S                                                 0U

//*****************************************************************************
//
// Register: SYS0_O_TIMMUTE2
//
//*****************************************************************************
// Field:    [31] CKMDIGLOCKSEL
//
// CKMKDIG: Selects between soft lock and global lock protection for HFOSCCTL
// register. 0: Soft lock, 1: Global lock.
#define SYS0_TIMMUTE2_CKMDIGLOCKSEL                                 0x80000000U
#define SYS0_TIMMUTE2_CKMDIGLOCKSEL_M                               0x80000000U
#define SYS0_TIMMUTE2_CKMDIGLOCKSEL_S                                       31U

// Field: [30:25] WTOSSEP
//
// PMCTL: Separation between last stage with weak switch and first strong
// switch
#define SYS0_TIMMUTE2_WTOSSEP_W                                              6U
#define SYS0_TIMMUTE2_WTOSSEP_M                                     0x7E000000U
#define SYS0_TIMMUTE2_WTOSSEP_S                                             25U

// Field: [24:19] WSTGSEP
//
// PMCTL: Separation between weak switch stages
#define SYS0_TIMMUTE2_WSTGSEP_W                                              6U
#define SYS0_TIMMUTE2_WSTGSEP_M                                     0x01F80000U
#define SYS0_TIMMUTE2_WSTGSEP_S                                             19U

// Field: [18:17] SPARE
//
// PMUREG: Spare
#define SYS0_TIMMUTE2_SPARE_W                                                2U
#define SYS0_TIMMUTE2_SPARE_M                                       0x00060000U
#define SYS0_TIMMUTE2_SPARE_S                                               17U

// Field:    [16] GLDOEACDIS
//
// PMUREG: GLDO error amplifier clamp disable
#define SYS0_TIMMUTE2_GLDOEACDIS                                    0x00010000U
#define SYS0_TIMMUTE2_GLDOEACDIS_M                                  0x00010000U
#define SYS0_TIMMUTE2_GLDOEACDIS_S                                          16U

// Field:    [15] GLDOISINKEN
//
// PMUREG: GLDO current sink enable
#define SYS0_TIMMUTE2_GLDOISINKEN                                   0x00008000U
#define SYS0_TIMMUTE2_GLDOISINKEN_M                                 0x00008000U
#define SYS0_TIMMUTE2_GLDOISINKEN_S                                         15U

// Field: [14:13] VDDRBCMP
//
// PMUREG: VDDR boost mode compensation
#define SYS0_TIMMUTE2_VDDRBCMP_W                                             2U
#define SYS0_TIMMUTE2_VDDRBCMP_M                                    0x00006000U
#define SYS0_TIMMUTE2_VDDRBCMP_S                                            13U

// Field:  [12:8] RECOMPOS
//
// PMUREG: Recharge comparator offset trim
#define SYS0_TIMMUTE2_RECOMPOS_W                                             5U
#define SYS0_TIMMUTE2_RECOMPOS_M                                    0x00001F00U
#define SYS0_TIMMUTE2_RECOMPOS_S                                             8U

// Field:   [7:5] ITUDIG
//
// PMUREG: ITRIM UDIG LDO
#define SYS0_TIMMUTE2_ITUDIG_W                                               3U
#define SYS0_TIMMUTE2_ITUDIG_M                                      0x000000E0U
#define SYS0_TIMMUTE2_ITUDIG_S                                               5U

// Field:   [4:2] ITDIG
//
// PMUREG: ITRIM DIG LDO
#define SYS0_TIMMUTE2_ITDIG_W                                                3U
#define SYS0_TIMMUTE2_ITDIG_M                                       0x0000001CU
#define SYS0_TIMMUTE2_ITDIG_S                                                2U

// Field:   [1:0] ITDUMMY
//
// PMUREG: ITRIM dummy
#define SYS0_TIMMUTE2_ITDUMMY_W                                              2U
#define SYS0_TIMMUTE2_ITDUMMY_M                                     0x00000003U
#define SYS0_TIMMUTE2_ITDUMMY_S                                              0U

//*****************************************************************************
//
// Register: SYS0_O_TIMMUTE3
//
//*****************************************************************************
// Field:    [27] ATBRESBYPCLK
//
// TSD ATB series resistance bypass clear value.
#define SYS0_TIMMUTE3_ATBRESBYPCLK                                  0x08000000U
#define SYS0_TIMMUTE3_ATBRESBYPCLK_M                                0x08000000U
#define SYS0_TIMMUTE3_ATBRESBYPCLK_S                                        27U

// Field:    [26] HYDISCLR
//
// TSD hysteresis disable clear value.
#define SYS0_TIMMUTE3_HYDISCLR                                      0x04000000U
#define SYS0_TIMMUTE3_HYDISCLR_M                                    0x04000000U
#define SYS0_TIMMUTE3_HYDISCLR_S                                            26U

// Field: [25:20] TSENSCLR
//
// TSD comparator temperature sensor trim clear value
#define SYS0_TIMMUTE3_TSENSCLR_W                                             6U
#define SYS0_TIMMUTE3_TSENSCLR_M                                    0x03F00000U
#define SYS0_TIMMUTE3_TSENSCLR_S                                            20U

// Field:    [19] TMODECLR
//
// TSD comparator bandgap reference trim clear value
#define SYS0_TIMMUTE3_TMODECLR                                      0x00080000U
#define SYS0_TIMMUTE3_TMODECLR_M                                    0x00080000U
#define SYS0_TIMMUTE3_TMODECLR_S                                            19U

// Field: [18:14] BGRCLR
//
// TSD comparator bandgap reference trim clear value
#define SYS0_TIMMUTE3_BGRCLR_W                                               5U
#define SYS0_TIMMUTE3_BGRCLR_M                                      0x0007C000U
#define SYS0_TIMMUTE3_BGRCLR_S                                              14U

// Field:    [13] ATBRESBYPSET
//
// TSD ATB series resistance bypass set value.
// ENUMs:
// BYPASS                   Resistor bypassed
// CONN                     Resistor connected
#define SYS0_TIMMUTE3_ATBRESBYPSET                                  0x00002000U
#define SYS0_TIMMUTE3_ATBRESBYPSET_M                                0x00002000U
#define SYS0_TIMMUTE3_ATBRESBYPSET_S                                        13U
#define SYS0_TIMMUTE3_ATBRESBYPSET_BYPASS                           0x00002000U
#define SYS0_TIMMUTE3_ATBRESBYPSET_CONN                             0x00000000U

// Field:    [12] HYSDISSET
//
// TSD hysteresis disable set value.
// ENUMs:
// DIS                      Disable
// EN                       Enable
#define SYS0_TIMMUTE3_HYSDISSET                                     0x00001000U
#define SYS0_TIMMUTE3_HYSDISSET_M                                   0x00001000U
#define SYS0_TIMMUTE3_HYSDISSET_S                                           12U
#define SYS0_TIMMUTE3_HYSDISSET_DIS                                 0x00001000U
#define SYS0_TIMMUTE3_HYSDISSET_EN                                  0x00000000U

// Field:  [11:6] TSENSSET
//
// TSD comparator temperature sensor trim set value
#define SYS0_TIMMUTE3_TSENSSET_W                                             6U
#define SYS0_TIMMUTE3_TSENSSET_M                                    0x00000FC0U
#define SYS0_TIMMUTE3_TSENSSET_S                                             6U

// Field:     [5] TMODESET
//
// TSD test mode select set value for trim and test.
// ENUMs:
// ADC                      ADC mode
// ATB                      ATB mode
#define SYS0_TIMMUTE3_TMODESET                                      0x00000020U
#define SYS0_TIMMUTE3_TMODESET_M                                    0x00000020U
#define SYS0_TIMMUTE3_TMODESET_S                                             5U
#define SYS0_TIMMUTE3_TMODESET_ADC                                  0x00000020U
#define SYS0_TIMMUTE3_TMODESET_ATB                                  0x00000000U

// Field:   [4:0] BGRSET
//
// TSD comparator bandgap reference trim set value
#define SYS0_TIMMUTE3_BGRSET_W                                               5U
#define SYS0_TIMMUTE3_BGRSET_M                                      0x0000001FU
#define SYS0_TIMMUTE3_BGRSET_S                                               0U

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
// enough that users of the device may have to revise PCB or or software design
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
// Bit pattern uniquely identifying a part
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
// Device life cycle value
// ENUMs:
// LCYCLE_ILLEGAL           Illegal
// LCYCLE_RETEST            Re-test
// LCYCLE_PRODDEV           Production device
// LCYCLE_ENGRDEV           Engineering device
// LCYCLE_TESTFT            Test FT
// LCYCLE_TESTPT            Test PT
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
// SOCADC: Lower 32 bits of CDAC trim. These bits are used in DTC.
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
// SOCADC: Middle 32 bits of CDAC trim. These bits are used in DTC.
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
// BATMON: Battery calculation coefficient 1
#define SYS0_TMUTE3_BATC1_W                                                  6U
#define SYS0_TMUTE3_BATC1_M                                         0xFC000000U
#define SYS0_TMUTE3_BATC1_S                                                 26U

// Field: [25:19] BATC0
//
// BATMON: Battery calculation coefficient 0
#define SYS0_TMUTE3_BATC0_W                                                  7U
#define SYS0_TMUTE3_BATC0_M                                         0x03F80000U
#define SYS0_TMUTE3_BATC0_S                                                 19U

// Field: [18:14] TEMPC2
//
// BATMON: Temperature calculation coefficient 2
#define SYS0_TMUTE3_TEMPC2_W                                                 5U
#define SYS0_TMUTE3_TEMPC2_M                                        0x0007C000U
#define SYS0_TMUTE3_TEMPC2_S                                                14U

// Field:  [13:8] TEMPC1
//
// BATMON: Temperature calculation coefficient 1
#define SYS0_TMUTE3_TEMPC1_W                                                 6U
#define SYS0_TMUTE3_TEMPC1_M                                        0x00003F00U
#define SYS0_TMUTE3_TEMPC1_S                                                 8U

// Field:   [7:0] TEMPC0
//
// BATMON: Temperature calculation coefficient 0
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
// PMUREG: Recharge comparator reference level. Valid values are 0x2 to 0xA.
// Recommended default value is 0x9. Setting 0x2 corresponds to VDDR = 1.35V
// and 0xA corresponds to VDDR = 1.47V with 15mV linear increase for every
// step.
#define SYS0_TMUTE4_RECHCOMPREFLVL_W                                         4U
#define SYS0_TMUTE4_RECHCOMPREFLVL_M                                0xF0000000U
#define SYS0_TMUTE4_RECHCOMPREFLVL_S                                        28U

// Field: [27:26] IOSTRCFG2
//
// BATMON: IO drive strength conversion parameter for the second stage
#define SYS0_TMUTE4_IOSTRCFG2_W                                              2U
#define SYS0_TMUTE4_IOSTRCFG2_M                                     0x0C000000U
#define SYS0_TMUTE4_IOSTRCFG2_S                                             26U

// Field: [25:22] IOSTRCFG1
//
// BATMON: IO drive strength conversion parameter for the first stage
#define SYS0_TMUTE4_IOSTRCFG1_W                                              4U
#define SYS0_TMUTE4_IOSTRCFG1_M                                     0x03C00000U
#define SYS0_TMUTE4_IOSTRCFG1_S                                             22U

// Field: [21:19] MAX
//
// IOC: Maximum IO drive strength
#define SYS0_TMUTE4_MAX_W                                                    3U
#define SYS0_TMUTE4_MAX_M                                           0x00380000U
#define SYS0_TMUTE4_MAX_S                                                   19U

// Field: [18:16] MED
//
// IOC: Medium IO drive strength
#define SYS0_TMUTE4_MED_W                                                    3U
#define SYS0_TMUTE4_MED_M                                           0x00070000U
#define SYS0_TMUTE4_MED_S                                                   16U

// Field: [15:13] MIN
//
// IOC: Minimum IO drive strength
#define SYS0_TMUTE4_MIN_W                                                    3U
#define SYS0_TMUTE4_MIN_M                                           0x0000E000U
#define SYS0_TMUTE4_MIN_S                                                   13U

// Field: [12:11] DCDCLOAD
//
// DCDC: Set DCDC load current threshold
#define SYS0_TMUTE4_DCDCLOAD_W                                               2U
#define SYS0_TMUTE4_DCDCLOAD_M                                      0x00001800U
#define SYS0_TMUTE4_DCDCLOAD_S                                              11U

// Field:  [10:8] IPEAK
//
// DCDC: Set inductor peak current
#define SYS0_TMUTE4_IPEAK_W                                                  3U
#define SYS0_TMUTE4_IPEAK_M                                         0x00000700U
#define SYS0_TMUTE4_IPEAK_S                                                  8U

// Field:   [7:6] DTIME
//
// DCDC: Dead time trim
#define SYS0_TMUTE4_DTIME_W                                                  2U
#define SYS0_TMUTE4_DTIME_M                                         0x000000C0U
#define SYS0_TMUTE4_DTIME_S                                                  6U

// Field:   [5:3] LENSEL
//
// DCDC: Control NMOS switch strength
#define SYS0_TMUTE4_LENSEL_W                                                 3U
#define SYS0_TMUTE4_LENSEL_M                                        0x00000038U
#define SYS0_TMUTE4_LENSEL_S                                                 3U

// Field:   [2:0] HENSEL
//
// DCDC: Control PMOS switch strength
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
// DCDC: Driver drive strength configuration
#define SYS0_TMUTE5_DCDCDRVDS_W                                              3U
#define SYS0_TMUTE5_DCDCDRVDS_M                                     0x00001C00U
#define SYS0_TMUTE5_DCDCDRVDS_S                                             10U

// Field:   [9:5] GLDOISCLR
//
// GLDO current source trim clear value
#define SYS0_TMUTE5_GLDOISCLR_W                                              5U
#define SYS0_TMUTE5_GLDOISCLR_M                                     0x000003E0U
#define SYS0_TMUTE5_GLDOISCLR_S                                              5U

// Field:   [4:0] GLDOISSET
//
// GLDO current source trim set value
#define SYS0_TMUTE5_GLDOISSET_W                                              5U
#define SYS0_TMUTE5_GLDOISSET_M                                     0x0000001FU
#define SYS0_TMUTE5_GLDOISSET_S                                              0U


#endif // __SYS0__
