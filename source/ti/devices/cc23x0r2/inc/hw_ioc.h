/******************************************************************************
*  Filename:       hw_ioc_h
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

#ifndef __HW_IOC_H__
#define __HW_IOC_H__

//*****************************************************************************
//
// This section defines the register offsets of
// IOC component
//
//*****************************************************************************
// Module Description
#define IOC_O_DESC                                                  0x00000000U

// Extended Module Description
#define IOC_O_DESCEX                                                0x00000004U

// Digital Test Bus
#define IOC_O_DTB                                                   0x00000064U

// Configuration
#define IOC_O_IOC3                                                  0x0000010CU

// Configuration
#define IOC_O_IOC4                                                  0x00000110U

// Configuration
#define IOC_O_IOC6                                                  0x00000118U

// Configuration
#define IOC_O_IOC8                                                  0x00000120U

// Configuration
#define IOC_O_IOC11                                                 0x0000012CU

// Configuration
#define IOC_O_IOC12                                                 0x00000130U

// Configuration
#define IOC_O_IOC13                                                 0x00000134U

// Configuration
#define IOC_O_IOC16                                                 0x00000140U

// Configuration
#define IOC_O_IOC17                                                 0x00000144U

// Configuration
#define IOC_O_IOC20                                                 0x00000150U

// Configuration
#define IOC_O_IOC21                                                 0x00000154U

// Configuration
#define IOC_O_IOC24                                                 0x00000160U

// DTB configuration
#define IOC_O_DTBCFG                                                0x00000C00U

// DTB output enable
#define IOC_O_DTBOE                                                 0x00000C04U

// Event configuration
#define IOC_O_EVTCFG                                                0x00000C08U

// Test
#define IOC_O_TEST                                                  0x00000C0CU

// DTB status
#define IOC_O_DTBSTAT                                               0x00000C10U

//*****************************************************************************
//
// Register: IOC_O_DESC
//
//*****************************************************************************
// Field: [31:16] MODID
//
// Module identification contains a unique peripheral identification number.
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define IOC_DESC_MODID_W                                                    16U
#define IOC_DESC_MODID_M                                            0xFFFF0000U
#define IOC_DESC_MODID_S                                                    16U
#define IOC_DESC_MODID_MAXIMUM                                      0xFFFF0000U
#define IOC_DESC_MODID_MINIMUM                                      0x00000000U

// Field: [15:12] STDIPOFF
//
// Standard IP registers offset. Value 0 indicates Standard IP registers are
// not present. Any other value between 1 to 15 indicates standard IP registers
// start from address offset 64 * STDIPOFF from base address.
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define IOC_DESC_STDIPOFF_W                                                  4U
#define IOC_DESC_STDIPOFF_M                                         0x0000F000U
#define IOC_DESC_STDIPOFF_S                                                 12U
#define IOC_DESC_STDIPOFF_MAXIMUM                                   0x0000F000U
#define IOC_DESC_STDIPOFF_MINIMUM                                   0x00000000U

// Field:  [11:8] INSTIDX
//
// Instance Index within the device. This will be a parameter to the RTL for
// modules that can have multiple instances.
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define IOC_DESC_INSTIDX_W                                                   4U
#define IOC_DESC_INSTIDX_M                                          0x00000F00U
#define IOC_DESC_INSTIDX_S                                                   8U
#define IOC_DESC_INSTIDX_MAXIMUM                                    0x00000F00U
#define IOC_DESC_INSTIDX_MINIMUM                                    0x00000000U

// Field:   [7:4] MAJREV
//
// Major rev of the IP
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define IOC_DESC_MAJREV_W                                                    4U
#define IOC_DESC_MAJREV_M                                           0x000000F0U
#define IOC_DESC_MAJREV_S                                                    4U
#define IOC_DESC_MAJREV_MAXIMUM                                     0x000000F0U
#define IOC_DESC_MAJREV_MINIMUM                                     0x00000000U

// Field:   [3:0] MINREV
//
// Minor rev of the IP
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define IOC_DESC_MINREV_W                                                    4U
#define IOC_DESC_MINREV_M                                           0x0000000FU
#define IOC_DESC_MINREV_S                                                    0U
#define IOC_DESC_MINREV_MAXIMUM                                     0x0000000FU
#define IOC_DESC_MINREV_MINIMUM                                     0x00000000U

//*****************************************************************************
//
// Register: IOC_O_DESCEX
//
//*****************************************************************************
// Field: [15:12] NUMDTBIO
//
// Number of DTB IOs supported. Total DTB IOs supported is NUMDTBIO value +1.
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define IOC_DESCEX_NUMDTBIO_W                                                4U
#define IOC_DESCEX_NUMDTBIO_M                                       0x0000F000U
#define IOC_DESCEX_NUMDTBIO_S                                               12U
#define IOC_DESCEX_NUMDTBIO_MAXIMUM                                 0x0000F000U
#define IOC_DESCEX_NUMDTBIO_MINIMUM                                 0x00000000U

// Field:  [11:7] NUMHDIO
//
// Number of HD IOs supported. Total HD IOs supported is NUMHDIO value +1.
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define IOC_DESCEX_NUMHDIO_W                                                 5U
#define IOC_DESCEX_NUMHDIO_M                                        0x00000F80U
#define IOC_DESCEX_NUMHDIO_S                                                 7U
#define IOC_DESCEX_NUMHDIO_MAXIMUM                                  0x00000F80U
#define IOC_DESCEX_NUMHDIO_MINIMUM                                  0x00000000U

// Field:     [6] HDIO
//
// High drive IO supported by IOC.
// ENUMs:
// PRESENT                  HD IO supported by IOC
// ABSENT                   HD IO not supported by IOC
#define IOC_DESCEX_HDIO                                             0x00000040U
#define IOC_DESCEX_HDIO_M                                           0x00000040U
#define IOC_DESCEX_HDIO_S                                                    6U
#define IOC_DESCEX_HDIO_PRESENT                                     0x00000040U
#define IOC_DESCEX_HDIO_ABSENT                                      0x00000000U

// Field:   [5:0] NUMDIO
//
// Number of DIOs supported. Total DIOs supported is NUMDIO value +1.
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define IOC_DESCEX_NUMDIO_W                                                  6U
#define IOC_DESCEX_NUMDIO_M                                         0x0000003FU
#define IOC_DESCEX_NUMDIO_S                                                  0U
#define IOC_DESCEX_NUMDIO_MAXIMUM                                   0x0000003FU
#define IOC_DESCEX_NUMDIO_MINIMUM                                   0x00000000U

//*****************************************************************************
//
// Register: IOC_O_DTB
//
//*****************************************************************************
// Field:   [2:0] SEL
//
// This bit field is used to select DTB mux digital output signals.
// ENUMs:
// GRP7                     Selects test group 7
// GRP6                     Selects test group 6
// GRP5                     Selects test group 5
// GRP4                     Selects test group 4
// GRP3                     Selects test group 3
// GRP2                     Selects test group 2
// GRP1                     Selects test group 1
// DISABLE                  DTB output from peripheral is 0x0
#define IOC_DTB_SEL_W                                                        3U
#define IOC_DTB_SEL_M                                               0x00000007U
#define IOC_DTB_SEL_S                                                        0U
#define IOC_DTB_SEL_GRP7                                            0x00000007U
#define IOC_DTB_SEL_GRP6                                            0x00000006U
#define IOC_DTB_SEL_GRP5                                            0x00000005U
#define IOC_DTB_SEL_GRP4                                            0x00000004U
#define IOC_DTB_SEL_GRP3                                            0x00000003U
#define IOC_DTB_SEL_GRP2                                            0x00000002U
#define IOC_DTB_SEL_GRP1                                            0x00000001U
#define IOC_DTB_SEL_DISABLE                                         0x00000000U

//*****************************************************************************
//
// Register: IOC_O_IOC3
//
//*****************************************************************************
// Field:    [30] HYSTEN
//
// Input hysteresis enable
// ENUMs:
// ENABLE                   Input hysteresis enabled
// DISABLE                  Input hysteresis disabled
#define IOC_IOC3_HYSTEN                                             0x40000000U
#define IOC_IOC3_HYSTEN_M                                           0x40000000U
#define IOC_IOC3_HYSTEN_S                                                   30U
#define IOC_IOC3_HYSTEN_ENABLE                                      0x40000000U
#define IOC_IOC3_HYSTEN_DISABLE                                     0x00000000U

// Field:    [29] INPEN
//
// Input enable
// ENUMs:
// ENABLE                   Input enabled
// DISABLE                  Input disabled
#define IOC_IOC3_INPEN                                              0x20000000U
#define IOC_IOC3_INPEN_M                                            0x20000000U
#define IOC_IOC3_INPEN_S                                                    29U
#define IOC_IOC3_INPEN_ENABLE                                       0x20000000U
#define IOC_IOC3_INPEN_DISABLE                                      0x00000000U

// Field: [26:24] IOMODE
//
// IO Mode. Setting this to value 0x6 or 0x7 will default to normal IO
// behavior.
// ENUMs:
// OPENS_INV                Open Source, inverted IO
// OPENS                    Open Source, normal IO
// OPEND_INV                Open Drain, inverted IO
// OPEND                    Open Drain, normal IO
// INVERTED                 Inverted IO
// NORMAL                   Normal IO
#define IOC_IOC3_IOMODE_W                                                    3U
#define IOC_IOC3_IOMODE_M                                           0x07000000U
#define IOC_IOC3_IOMODE_S                                                   24U
#define IOC_IOC3_IOMODE_OPENS_INV                                   0x05000000U
#define IOC_IOC3_IOMODE_OPENS                                       0x04000000U
#define IOC_IOC3_IOMODE_OPEND_INV                                   0x03000000U
#define IOC_IOC3_IOMODE_OPEND                                       0x02000000U
#define IOC_IOC3_IOMODE_INVERTED                                    0x01000000U
#define IOC_IOC3_IOMODE_NORMAL                                      0x00000000U

// Field: [21:20] WUCFGSD
//
// Wakeup configuration from shutdown
// ENUMs:
// WAKE_HIGH                wakeup triggered when pad level is high
// WAKE_LOW                 wakeup triggered when pad level is low
// DISABLED_1               wakeup disabled
// DISABLED_0               wakeup disabled
#define IOC_IOC3_WUCFGSD_W                                                   2U
#define IOC_IOC3_WUCFGSD_M                                          0x00300000U
#define IOC_IOC3_WUCFGSD_S                                                  20U
#define IOC_IOC3_WUCFGSD_WAKE_HIGH                                  0x00300000U
#define IOC_IOC3_WUCFGSD_WAKE_LOW                                   0x00200000U
#define IOC_IOC3_WUCFGSD_DISABLED_1                                 0x00100000U
#define IOC_IOC3_WUCFGSD_DISABLED_0                                 0x00000000U

// Field:    [18] WUENSB
//
// Wakeup enable from standby
// ENUMs:
// ENABLE                   wakeup enabled (effective only if EDGEDET is
//                          enabled)
// DISABLE                  wakeup disabled
#define IOC_IOC3_WUENSB                                             0x00040000U
#define IOC_IOC3_WUENSB_M                                           0x00040000U
#define IOC_IOC3_WUENSB_S                                                   18U
#define IOC_IOC3_WUENSB_ENABLE                                      0x00040000U
#define IOC_IOC3_WUENSB_DISABLE                                     0x00000000U

// Field: [17:16] EDGEDET
//
// Edge detect configuration
// ENUMs:
// EDGE_BOTH                Positive and negative edge detection
// EDGE_POS                 Positive edge detection
// EDGE_NEG                 Negative edge detection
// EDGE_DIS                 No edge detection
#define IOC_IOC3_EDGEDET_W                                                   2U
#define IOC_IOC3_EDGEDET_M                                          0x00030000U
#define IOC_IOC3_EDGEDET_S                                                  16U
#define IOC_IOC3_EDGEDET_EDGE_BOTH                                  0x00030000U
#define IOC_IOC3_EDGEDET_EDGE_POS                                   0x00020000U
#define IOC_IOC3_EDGEDET_EDGE_NEG                                   0x00010000U
#define IOC_IOC3_EDGEDET_EDGE_DIS                                   0x00000000U

// Field: [14:13] PULLCTL
//
// Pull control. Setting this to value 0x3 disables pull.
// ENUMs:
// PULL_UP                  Pull up enabled
// PULL_DOWN                Pull down enabled
// PULL_DIS                 No pull
#define IOC_IOC3_PULLCTL_W                                                   2U
#define IOC_IOC3_PULLCTL_M                                          0x00006000U
#define IOC_IOC3_PULLCTL_S                                                  13U
#define IOC_IOC3_PULLCTL_PULL_UP                                    0x00004000U
#define IOC_IOC3_PULLCTL_PULL_DOWN                                  0x00002000U
#define IOC_IOC3_PULLCTL_PULL_DIS                                   0x00000000U

// Field:   [2:0] PORTCFG
//
// Port configuration.
// ENUMs:
// DTB                      Digital Test Bus function
// ANA                      Analog function
// PFUNC5                   Digital peripheral function-5
// PFUNC4                   Digital peripheral function-4
// PFUNC3                   Digital peripheral function-3
// PFUNC2                   Digital peripheral function-2
// PFUNC1                   Digital peripheral function-1
// BASE                     Base function
#define IOC_IOC3_PORTCFG_W                                                   3U
#define IOC_IOC3_PORTCFG_M                                          0x00000007U
#define IOC_IOC3_PORTCFG_S                                                   0U
#define IOC_IOC3_PORTCFG_DTB                                        0x00000007U
#define IOC_IOC3_PORTCFG_ANA                                        0x00000006U
#define IOC_IOC3_PORTCFG_PFUNC5                                     0x00000005U
#define IOC_IOC3_PORTCFG_PFUNC4                                     0x00000004U
#define IOC_IOC3_PORTCFG_PFUNC3                                     0x00000003U
#define IOC_IOC3_PORTCFG_PFUNC2                                     0x00000002U
#define IOC_IOC3_PORTCFG_PFUNC1                                     0x00000001U
#define IOC_IOC3_PORTCFG_BASE                                       0x00000000U

//*****************************************************************************
//
// Register: IOC_O_IOC4
//
//*****************************************************************************
// Field:    [30] HYSTEN
//
// Input hysteresis enable
// ENUMs:
// ENABLE                   Input hysteresis enabled
// DISABLE                  Input hysteresis disabled
#define IOC_IOC4_HYSTEN                                             0x40000000U
#define IOC_IOC4_HYSTEN_M                                           0x40000000U
#define IOC_IOC4_HYSTEN_S                                                   30U
#define IOC_IOC4_HYSTEN_ENABLE                                      0x40000000U
#define IOC_IOC4_HYSTEN_DISABLE                                     0x00000000U

// Field:    [29] INPEN
//
// Input enable
// ENUMs:
// ENABLE                   Input enabled
// DISABLE                  Input disabled
#define IOC_IOC4_INPEN                                              0x20000000U
#define IOC_IOC4_INPEN_M                                            0x20000000U
#define IOC_IOC4_INPEN_S                                                    29U
#define IOC_IOC4_INPEN_ENABLE                                       0x20000000U
#define IOC_IOC4_INPEN_DISABLE                                      0x00000000U

// Field: [26:24] IOMODE
//
// IO Mode. Setting this to value 0x6 or 0x7 will default to normal IO
// behavior.
// ENUMs:
// OPENS_INV                Open Source, inverted IO
// OPENS                    Open Source, normal IO
// OPEND_INV                Open Drain, inverted IO
// OPEND                    Open Drain, normal IO
// INVERTED                 Inverted IO
// NORMAL                   Normal IO
#define IOC_IOC4_IOMODE_W                                                    3U
#define IOC_IOC4_IOMODE_M                                           0x07000000U
#define IOC_IOC4_IOMODE_S                                                   24U
#define IOC_IOC4_IOMODE_OPENS_INV                                   0x05000000U
#define IOC_IOC4_IOMODE_OPENS                                       0x04000000U
#define IOC_IOC4_IOMODE_OPEND_INV                                   0x03000000U
#define IOC_IOC4_IOMODE_OPEND                                       0x02000000U
#define IOC_IOC4_IOMODE_INVERTED                                    0x01000000U
#define IOC_IOC4_IOMODE_NORMAL                                      0x00000000U

// Field: [21:20] WUCFGSD
//
// Wakeup configuration from shutdown
// ENUMs:
// WAKE_HIGH                wakeup triggered when pad level is high
// WAKE_LOW                 wakeup triggered when pad level is low
// DISABLED_1               wakeup disabled
// DISABLED_0               wakeup disabled
#define IOC_IOC4_WUCFGSD_W                                                   2U
#define IOC_IOC4_WUCFGSD_M                                          0x00300000U
#define IOC_IOC4_WUCFGSD_S                                                  20U
#define IOC_IOC4_WUCFGSD_WAKE_HIGH                                  0x00300000U
#define IOC_IOC4_WUCFGSD_WAKE_LOW                                   0x00200000U
#define IOC_IOC4_WUCFGSD_DISABLED_1                                 0x00100000U
#define IOC_IOC4_WUCFGSD_DISABLED_0                                 0x00000000U

// Field:    [18] WUENSB
//
// Wakeup enable from standby
// ENUMs:
// ENABLE                   wakeup enabled (effective only if EDGEDET is
//                          enabled)
// DISABLE                  wakeup disabled
#define IOC_IOC4_WUENSB                                             0x00040000U
#define IOC_IOC4_WUENSB_M                                           0x00040000U
#define IOC_IOC4_WUENSB_S                                                   18U
#define IOC_IOC4_WUENSB_ENABLE                                      0x00040000U
#define IOC_IOC4_WUENSB_DISABLE                                     0x00000000U

// Field: [17:16] EDGEDET
//
// Edge detect configuration
// ENUMs:
// EDGE_BOTH                Positive and negative edge detection
// EDGE_POS                 Positive edge detection
// EDGE_NEG                 Negative edge detection
// EDGE_DIS                 No edge detection
#define IOC_IOC4_EDGEDET_W                                                   2U
#define IOC_IOC4_EDGEDET_M                                          0x00030000U
#define IOC_IOC4_EDGEDET_S                                                  16U
#define IOC_IOC4_EDGEDET_EDGE_BOTH                                  0x00030000U
#define IOC_IOC4_EDGEDET_EDGE_POS                                   0x00020000U
#define IOC_IOC4_EDGEDET_EDGE_NEG                                   0x00010000U
#define IOC_IOC4_EDGEDET_EDGE_DIS                                   0x00000000U

// Field: [14:13] PULLCTL
//
// Pull control. Setting this to value 0x3 disables pull.
// ENUMs:
// PULL_UP                  Pull up enabled
// PULL_DOWN                Pull down enabled
// PULL_DIS                 No pull
#define IOC_IOC4_PULLCTL_W                                                   2U
#define IOC_IOC4_PULLCTL_M                                          0x00006000U
#define IOC_IOC4_PULLCTL_S                                                  13U
#define IOC_IOC4_PULLCTL_PULL_UP                                    0x00004000U
#define IOC_IOC4_PULLCTL_PULL_DOWN                                  0x00002000U
#define IOC_IOC4_PULLCTL_PULL_DIS                                   0x00000000U

// Field:   [2:0] PORTCFG
//
// Port configuration.
// ENUMs:
// DTB                      Digital Test Bus function
// ANA                      Analog function
// PFUNC5                   Digital peripheral function-5
// PFUNC4                   Digital peripheral function-4
// PFUNC3                   Digital peripheral function-3
// PFUNC2                   Digital peripheral function-2
// PFUNC1                   Digital peripheral function-1
// BASE                     Base function
#define IOC_IOC4_PORTCFG_W                                                   3U
#define IOC_IOC4_PORTCFG_M                                          0x00000007U
#define IOC_IOC4_PORTCFG_S                                                   0U
#define IOC_IOC4_PORTCFG_DTB                                        0x00000007U
#define IOC_IOC4_PORTCFG_ANA                                        0x00000006U
#define IOC_IOC4_PORTCFG_PFUNC5                                     0x00000005U
#define IOC_IOC4_PORTCFG_PFUNC4                                     0x00000004U
#define IOC_IOC4_PORTCFG_PFUNC3                                     0x00000003U
#define IOC_IOC4_PORTCFG_PFUNC2                                     0x00000002U
#define IOC_IOC4_PORTCFG_PFUNC1                                     0x00000001U
#define IOC_IOC4_PORTCFG_BASE                                       0x00000000U

//*****************************************************************************
//
// Register: IOC_O_IOC6
//
//*****************************************************************************
// Field:    [30] HYSTEN
//
// Input hysteresis enable
// ENUMs:
// ENABLE                   Input hysteresis enabled
// DISABLE                  Input hysteresis disabled
#define IOC_IOC6_HYSTEN                                             0x40000000U
#define IOC_IOC6_HYSTEN_M                                           0x40000000U
#define IOC_IOC6_HYSTEN_S                                                   30U
#define IOC_IOC6_HYSTEN_ENABLE                                      0x40000000U
#define IOC_IOC6_HYSTEN_DISABLE                                     0x00000000U

// Field:    [29] INPEN
//
// Input enable
// ENUMs:
// ENABLE                   Input enabled
// DISABLE                  Input disabled
#define IOC_IOC6_INPEN                                              0x20000000U
#define IOC_IOC6_INPEN_M                                            0x20000000U
#define IOC_IOC6_INPEN_S                                                    29U
#define IOC_IOC6_INPEN_ENABLE                                       0x20000000U
#define IOC_IOC6_INPEN_DISABLE                                      0x00000000U

// Field: [26:24] IOMODE
//
// IO Mode. Setting this to value 0x6 or 0x7 will default to normal IO
// behavior.
// ENUMs:
// OPENS_INV                Open Source, inverted IO
// OPENS                    Open Source, normal IO
// OPEND_INV                Open Drain, inverted IO
// OPEND                    Open Drain, normal IO
// INVERTED                 Inverted IO
// NORMAL                   Normal IO
#define IOC_IOC6_IOMODE_W                                                    3U
#define IOC_IOC6_IOMODE_M                                           0x07000000U
#define IOC_IOC6_IOMODE_S                                                   24U
#define IOC_IOC6_IOMODE_OPENS_INV                                   0x05000000U
#define IOC_IOC6_IOMODE_OPENS                                       0x04000000U
#define IOC_IOC6_IOMODE_OPEND_INV                                   0x03000000U
#define IOC_IOC6_IOMODE_OPEND                                       0x02000000U
#define IOC_IOC6_IOMODE_INVERTED                                    0x01000000U
#define IOC_IOC6_IOMODE_NORMAL                                      0x00000000U

// Field: [21:20] WUCFGSD
//
// Wakeup configuration from shutdown
// ENUMs:
// WAKE_HIGH                wakeup triggered when pad level is high
// WAKE_LOW                 wakeup triggered when pad level is low
// DISABLED_1               wakeup disabled
// DISABLED_0               wakeup disabled
#define IOC_IOC6_WUCFGSD_W                                                   2U
#define IOC_IOC6_WUCFGSD_M                                          0x00300000U
#define IOC_IOC6_WUCFGSD_S                                                  20U
#define IOC_IOC6_WUCFGSD_WAKE_HIGH                                  0x00300000U
#define IOC_IOC6_WUCFGSD_WAKE_LOW                                   0x00200000U
#define IOC_IOC6_WUCFGSD_DISABLED_1                                 0x00100000U
#define IOC_IOC6_WUCFGSD_DISABLED_0                                 0x00000000U

// Field:    [18] WUENSB
//
// Wakeup enable from standby
// ENUMs:
// ENABLE                   wakeup enabled (effective only if EDGEDET is
//                          enabled)
// DISABLE                  wakeup disabled
#define IOC_IOC6_WUENSB                                             0x00040000U
#define IOC_IOC6_WUENSB_M                                           0x00040000U
#define IOC_IOC6_WUENSB_S                                                   18U
#define IOC_IOC6_WUENSB_ENABLE                                      0x00040000U
#define IOC_IOC6_WUENSB_DISABLE                                     0x00000000U

// Field: [17:16] EDGEDET
//
// Edge detect configuration
// ENUMs:
// EDGE_BOTH                Positive and negative edge detection
// EDGE_POS                 Positive edge detection
// EDGE_NEG                 Negative edge detection
// EDGE_DIS                 No edge detection
#define IOC_IOC6_EDGEDET_W                                                   2U
#define IOC_IOC6_EDGEDET_M                                          0x00030000U
#define IOC_IOC6_EDGEDET_S                                                  16U
#define IOC_IOC6_EDGEDET_EDGE_BOTH                                  0x00030000U
#define IOC_IOC6_EDGEDET_EDGE_POS                                   0x00020000U
#define IOC_IOC6_EDGEDET_EDGE_NEG                                   0x00010000U
#define IOC_IOC6_EDGEDET_EDGE_DIS                                   0x00000000U

// Field: [14:13] PULLCTL
//
// Pull control. Setting this to value 0x3 disables pull.
// ENUMs:
// PULL_UP                  Pull up enabled
// PULL_DOWN                Pull down enabled
// PULL_DIS                 No pull
#define IOC_IOC6_PULLCTL_W                                                   2U
#define IOC_IOC6_PULLCTL_M                                          0x00006000U
#define IOC_IOC6_PULLCTL_S                                                  13U
#define IOC_IOC6_PULLCTL_PULL_UP                                    0x00004000U
#define IOC_IOC6_PULLCTL_PULL_DOWN                                  0x00002000U
#define IOC_IOC6_PULLCTL_PULL_DIS                                   0x00000000U

// Field:   [2:0] PORTCFG
//
// Port configuration.
// ENUMs:
// DTB                      Digital Test Bus function
// ANA                      Analog function
// PFUNC5                   Digital peripheral function-5
// PFUNC4                   Digital peripheral function-4
// PFUNC3                   Digital peripheral function-3
// PFUNC2                   Digital peripheral function-2
// PFUNC1                   Digital peripheral function-1
// BASE                     Base function
#define IOC_IOC6_PORTCFG_W                                                   3U
#define IOC_IOC6_PORTCFG_M                                          0x00000007U
#define IOC_IOC6_PORTCFG_S                                                   0U
#define IOC_IOC6_PORTCFG_DTB                                        0x00000007U
#define IOC_IOC6_PORTCFG_ANA                                        0x00000006U
#define IOC_IOC6_PORTCFG_PFUNC5                                     0x00000005U
#define IOC_IOC6_PORTCFG_PFUNC4                                     0x00000004U
#define IOC_IOC6_PORTCFG_PFUNC3                                     0x00000003U
#define IOC_IOC6_PORTCFG_PFUNC2                                     0x00000002U
#define IOC_IOC6_PORTCFG_PFUNC1                                     0x00000001U
#define IOC_IOC6_PORTCFG_BASE                                       0x00000000U

//*****************************************************************************
//
// Register: IOC_O_IOC8
//
//*****************************************************************************
// Field:    [30] HYSTEN
//
// Input hysteresis enable
// ENUMs:
// ENABLE                   Input hysteresis enabled
// DISABLE                  Input hysteresis disabled
#define IOC_IOC8_HYSTEN                                             0x40000000U
#define IOC_IOC8_HYSTEN_M                                           0x40000000U
#define IOC_IOC8_HYSTEN_S                                                   30U
#define IOC_IOC8_HYSTEN_ENABLE                                      0x40000000U
#define IOC_IOC8_HYSTEN_DISABLE                                     0x00000000U

// Field:    [29] INPEN
//
// Input enable
// ENUMs:
// ENABLE                   Input enabled
// DISABLE                  Input disabled
#define IOC_IOC8_INPEN                                              0x20000000U
#define IOC_IOC8_INPEN_M                                            0x20000000U
#define IOC_IOC8_INPEN_S                                                    29U
#define IOC_IOC8_INPEN_ENABLE                                       0x20000000U
#define IOC_IOC8_INPEN_DISABLE                                      0x00000000U

// Field: [26:24] IOMODE
//
// IO Mode. Setting this to value 0x6 or 0x7 will default to normal IO
// behavior.
// ENUMs:
// OPENS_INV                Open Source, inverted IO
// OPENS                    Open Source, normal IO
// OPEND_INV                Open Drain, inverted IO
// OPEND                    Open Drain, normal IO
// INVERTED                 Inverted IO
// NORMAL                   Normal IO
#define IOC_IOC8_IOMODE_W                                                    3U
#define IOC_IOC8_IOMODE_M                                           0x07000000U
#define IOC_IOC8_IOMODE_S                                                   24U
#define IOC_IOC8_IOMODE_OPENS_INV                                   0x05000000U
#define IOC_IOC8_IOMODE_OPENS                                       0x04000000U
#define IOC_IOC8_IOMODE_OPEND_INV                                   0x03000000U
#define IOC_IOC8_IOMODE_OPEND                                       0x02000000U
#define IOC_IOC8_IOMODE_INVERTED                                    0x01000000U
#define IOC_IOC8_IOMODE_NORMAL                                      0x00000000U

// Field: [21:20] WUCFGSD
//
// Wakeup configuration from shutdown
// ENUMs:
// WAKE_HIGH                wakeup triggered when pad level is high
// WAKE_LOW                 wakeup triggered when pad level is low
// DISABLED_1               wakeup disabled
// DISABLED_0               wakeup disabled
#define IOC_IOC8_WUCFGSD_W                                                   2U
#define IOC_IOC8_WUCFGSD_M                                          0x00300000U
#define IOC_IOC8_WUCFGSD_S                                                  20U
#define IOC_IOC8_WUCFGSD_WAKE_HIGH                                  0x00300000U
#define IOC_IOC8_WUCFGSD_WAKE_LOW                                   0x00200000U
#define IOC_IOC8_WUCFGSD_DISABLED_1                                 0x00100000U
#define IOC_IOC8_WUCFGSD_DISABLED_0                                 0x00000000U

// Field:    [18] WUENSB
//
// Wakeup enable from standby
// ENUMs:
// ENABLE                   wakeup enabled (effective only if EDGEDET is
//                          enabled)
// DISABLE                  wakeup disabled
#define IOC_IOC8_WUENSB                                             0x00040000U
#define IOC_IOC8_WUENSB_M                                           0x00040000U
#define IOC_IOC8_WUENSB_S                                                   18U
#define IOC_IOC8_WUENSB_ENABLE                                      0x00040000U
#define IOC_IOC8_WUENSB_DISABLE                                     0x00000000U

// Field: [17:16] EDGEDET
//
// Edge detect configuration
// ENUMs:
// EDGE_BOTH                Positive and negative edge detection
// EDGE_POS                 Positive edge detection
// EDGE_NEG                 Negative edge detection
// EDGE_DIS                 No edge detection
#define IOC_IOC8_EDGEDET_W                                                   2U
#define IOC_IOC8_EDGEDET_M                                          0x00030000U
#define IOC_IOC8_EDGEDET_S                                                  16U
#define IOC_IOC8_EDGEDET_EDGE_BOTH                                  0x00030000U
#define IOC_IOC8_EDGEDET_EDGE_POS                                   0x00020000U
#define IOC_IOC8_EDGEDET_EDGE_NEG                                   0x00010000U
#define IOC_IOC8_EDGEDET_EDGE_DIS                                   0x00000000U

// Field: [14:13] PULLCTL
//
// Pull control. Setting this to value 0x3 disables pull.
// ENUMs:
// PULL_UP                  Pull up enabled
// PULL_DOWN                Pull down enabled
// PULL_DIS                 No pull
#define IOC_IOC8_PULLCTL_W                                                   2U
#define IOC_IOC8_PULLCTL_M                                          0x00006000U
#define IOC_IOC8_PULLCTL_S                                                  13U
#define IOC_IOC8_PULLCTL_PULL_UP                                    0x00004000U
#define IOC_IOC8_PULLCTL_PULL_DOWN                                  0x00002000U
#define IOC_IOC8_PULLCTL_PULL_DIS                                   0x00000000U

// Field:   [2:0] PORTCFG
//
// Port configuration.
// ENUMs:
// DTB                      Digital Test Bus function
// ANA                      Analog function
// PFUNC5                   Digital peripheral function-5
// PFUNC4                   Digital peripheral function-4
// PFUNC3                   Digital peripheral function-3
// PFUNC2                   Digital peripheral function-2
// PFUNC1                   Digital peripheral function-1
// BASE                     Base function
#define IOC_IOC8_PORTCFG_W                                                   3U
#define IOC_IOC8_PORTCFG_M                                          0x00000007U
#define IOC_IOC8_PORTCFG_S                                                   0U
#define IOC_IOC8_PORTCFG_DTB                                        0x00000007U
#define IOC_IOC8_PORTCFG_ANA                                        0x00000006U
#define IOC_IOC8_PORTCFG_PFUNC5                                     0x00000005U
#define IOC_IOC8_PORTCFG_PFUNC4                                     0x00000004U
#define IOC_IOC8_PORTCFG_PFUNC3                                     0x00000003U
#define IOC_IOC8_PORTCFG_PFUNC2                                     0x00000002U
#define IOC_IOC8_PORTCFG_PFUNC1                                     0x00000001U
#define IOC_IOC8_PORTCFG_BASE                                       0x00000000U

//*****************************************************************************
//
// Register: IOC_O_IOC11
//
//*****************************************************************************
// Field:    [30] HYSTEN
//
// Input hysteresis enable
// ENUMs:
// ENABLE                   Input hysteresis enabled
// DISABLE                  Input hysteresis disabled
#define IOC_IOC11_HYSTEN                                            0x40000000U
#define IOC_IOC11_HYSTEN_M                                          0x40000000U
#define IOC_IOC11_HYSTEN_S                                                  30U
#define IOC_IOC11_HYSTEN_ENABLE                                     0x40000000U
#define IOC_IOC11_HYSTEN_DISABLE                                    0x00000000U

// Field:    [29] INPEN
//
// Input enable
// ENUMs:
// ENABLE                   Input enabled
// DISABLE                  Input disabled
#define IOC_IOC11_INPEN                                             0x20000000U
#define IOC_IOC11_INPEN_M                                           0x20000000U
#define IOC_IOC11_INPEN_S                                                   29U
#define IOC_IOC11_INPEN_ENABLE                                      0x20000000U
#define IOC_IOC11_INPEN_DISABLE                                     0x00000000U

// Field: [26:24] IOMODE
//
// IO Mode. Setting this to value 0x6 or 0x7 will default to normal IO
// behavior.
// ENUMs:
// OPENS_INV                Open Source, inverted IO
// OPENS                    Open Source, normal IO
// OPEND_INV                Open Drain, inverted IO
// OPEND                    Open Drain, normal IO
// INVERTED                 Inverted IO
// NORMAL                   Normal IO
#define IOC_IOC11_IOMODE_W                                                   3U
#define IOC_IOC11_IOMODE_M                                          0x07000000U
#define IOC_IOC11_IOMODE_S                                                  24U
#define IOC_IOC11_IOMODE_OPENS_INV                                  0x05000000U
#define IOC_IOC11_IOMODE_OPENS                                      0x04000000U
#define IOC_IOC11_IOMODE_OPEND_INV                                  0x03000000U
#define IOC_IOC11_IOMODE_OPEND                                      0x02000000U
#define IOC_IOC11_IOMODE_INVERTED                                   0x01000000U
#define IOC_IOC11_IOMODE_NORMAL                                     0x00000000U

// Field: [21:20] WUCFGSD
//
// Wakeup configuration from shutdown
// ENUMs:
// WAKE_HIGH                wakeup triggered when pad level is high
// WAKE_LOW                 wakeup triggered when pad level is low
// DISABLED_1               wakeup disabled
// DISABLED_0               wakeup disabled
#define IOC_IOC11_WUCFGSD_W                                                  2U
#define IOC_IOC11_WUCFGSD_M                                         0x00300000U
#define IOC_IOC11_WUCFGSD_S                                                 20U
#define IOC_IOC11_WUCFGSD_WAKE_HIGH                                 0x00300000U
#define IOC_IOC11_WUCFGSD_WAKE_LOW                                  0x00200000U
#define IOC_IOC11_WUCFGSD_DISABLED_1                                0x00100000U
#define IOC_IOC11_WUCFGSD_DISABLED_0                                0x00000000U

// Field:    [18] WUENSB
//
// Wakeup enable from standby
// ENUMs:
// ENABLE                   wakeup enabled (effective only if EDGEDET is
//                          enabled)
// DISABLE                  wakeup disabled
#define IOC_IOC11_WUENSB                                            0x00040000U
#define IOC_IOC11_WUENSB_M                                          0x00040000U
#define IOC_IOC11_WUENSB_S                                                  18U
#define IOC_IOC11_WUENSB_ENABLE                                     0x00040000U
#define IOC_IOC11_WUENSB_DISABLE                                    0x00000000U

// Field: [17:16] EDGEDET
//
// Edge detect configuration
// ENUMs:
// EDGE_BOTH                Positive and negative edge detection
// EDGE_POS                 Positive edge detection
// EDGE_NEG                 Negative edge detection
// EDGE_DIS                 No edge detection
#define IOC_IOC11_EDGEDET_W                                                  2U
#define IOC_IOC11_EDGEDET_M                                         0x00030000U
#define IOC_IOC11_EDGEDET_S                                                 16U
#define IOC_IOC11_EDGEDET_EDGE_BOTH                                 0x00030000U
#define IOC_IOC11_EDGEDET_EDGE_POS                                  0x00020000U
#define IOC_IOC11_EDGEDET_EDGE_NEG                                  0x00010000U
#define IOC_IOC11_EDGEDET_EDGE_DIS                                  0x00000000U

// Field: [14:13] PULLCTL
//
// Pull control. Setting this to value 0x3 disables pull.
// ENUMs:
// PULL_UP                  Pull up enabled
// PULL_DOWN                Pull down enabled
// PULL_DIS                 No pull
#define IOC_IOC11_PULLCTL_W                                                  2U
#define IOC_IOC11_PULLCTL_M                                         0x00006000U
#define IOC_IOC11_PULLCTL_S                                                 13U
#define IOC_IOC11_PULLCTL_PULL_UP                                   0x00004000U
#define IOC_IOC11_PULLCTL_PULL_DOWN                                 0x00002000U
#define IOC_IOC11_PULLCTL_PULL_DIS                                  0x00000000U

// Field:   [2:0] PORTCFG
//
// Port configuration.
// ENUMs:
// DTB                      Digital Test Bus function
// ANA                      Analog function
// PFUNC5                   Digital peripheral function-5
// PFUNC4                   Digital peripheral function-4
// PFUNC3                   Digital peripheral function-3
// PFUNC2                   Digital peripheral function-2
// PFUNC1                   Digital peripheral function-1
// BASE                     Base function
#define IOC_IOC11_PORTCFG_W                                                  3U
#define IOC_IOC11_PORTCFG_M                                         0x00000007U
#define IOC_IOC11_PORTCFG_S                                                  0U
#define IOC_IOC11_PORTCFG_DTB                                       0x00000007U
#define IOC_IOC11_PORTCFG_ANA                                       0x00000006U
#define IOC_IOC11_PORTCFG_PFUNC5                                    0x00000005U
#define IOC_IOC11_PORTCFG_PFUNC4                                    0x00000004U
#define IOC_IOC11_PORTCFG_PFUNC3                                    0x00000003U
#define IOC_IOC11_PORTCFG_PFUNC2                                    0x00000002U
#define IOC_IOC11_PORTCFG_PFUNC1                                    0x00000001U
#define IOC_IOC11_PORTCFG_BASE                                      0x00000000U

//*****************************************************************************
//
// Register: IOC_O_IOC12
//
//*****************************************************************************
// Field:    [30] HYSTEN
//
// Input hysteresis enable
// ENUMs:
// ENABLE                   Input hysteresis enabled
// DISABLE                  Input hysteresis disabled
#define IOC_IOC12_HYSTEN                                            0x40000000U
#define IOC_IOC12_HYSTEN_M                                          0x40000000U
#define IOC_IOC12_HYSTEN_S                                                  30U
#define IOC_IOC12_HYSTEN_ENABLE                                     0x40000000U
#define IOC_IOC12_HYSTEN_DISABLE                                    0x00000000U

// Field:    [29] INPEN
//
// Input enable
// ENUMs:
// ENABLE                   Input enabled
// DISABLE                  Input disabled
#define IOC_IOC12_INPEN                                             0x20000000U
#define IOC_IOC12_INPEN_M                                           0x20000000U
#define IOC_IOC12_INPEN_S                                                   29U
#define IOC_IOC12_INPEN_ENABLE                                      0x20000000U
#define IOC_IOC12_INPEN_DISABLE                                     0x00000000U

// Field: [26:24] IOMODE
//
// IO Mode. Setting this to value 0x6 or 0x7 will default to normal IO
// behavior.
// ENUMs:
// OPENS_INV                Open Source, inverted IO
// OPENS                    Open Source, normal IO
// OPEND_INV                Open Drain, inverted IO
// OPEND                    Open Drain, normal IO
// INVERTED                 Inverted IO
// NORMAL                   Normal IO
#define IOC_IOC12_IOMODE_W                                                   3U
#define IOC_IOC12_IOMODE_M                                          0x07000000U
#define IOC_IOC12_IOMODE_S                                                  24U
#define IOC_IOC12_IOMODE_OPENS_INV                                  0x05000000U
#define IOC_IOC12_IOMODE_OPENS                                      0x04000000U
#define IOC_IOC12_IOMODE_OPEND_INV                                  0x03000000U
#define IOC_IOC12_IOMODE_OPEND                                      0x02000000U
#define IOC_IOC12_IOMODE_INVERTED                                   0x01000000U
#define IOC_IOC12_IOMODE_NORMAL                                     0x00000000U

// Field: [21:20] WUCFGSD
//
// Wakeup configuration from shutdown
// ENUMs:
// WAKE_HIGH                wakeup triggered when pad level is high
// WAKE_LOW                 wakeup triggered when pad level is low
// DISABLED_1               wakeup disabled
// DISABLED_0               wakeup disabled
#define IOC_IOC12_WUCFGSD_W                                                  2U
#define IOC_IOC12_WUCFGSD_M                                         0x00300000U
#define IOC_IOC12_WUCFGSD_S                                                 20U
#define IOC_IOC12_WUCFGSD_WAKE_HIGH                                 0x00300000U
#define IOC_IOC12_WUCFGSD_WAKE_LOW                                  0x00200000U
#define IOC_IOC12_WUCFGSD_DISABLED_1                                0x00100000U
#define IOC_IOC12_WUCFGSD_DISABLED_0                                0x00000000U

// Field:    [18] WUENSB
//
// Wakeup enable from standby
// ENUMs:
// ENABLE                   wakeup enabled (effective only if EDGEDET is
//                          enabled)
// DISABLE                  wakeup disabled
#define IOC_IOC12_WUENSB                                            0x00040000U
#define IOC_IOC12_WUENSB_M                                          0x00040000U
#define IOC_IOC12_WUENSB_S                                                  18U
#define IOC_IOC12_WUENSB_ENABLE                                     0x00040000U
#define IOC_IOC12_WUENSB_DISABLE                                    0x00000000U

// Field: [17:16] EDGEDET
//
// Edge detect configuration
// ENUMs:
// EDGE_BOTH                Positive and negative edge detection
// EDGE_POS                 Positive edge detection
// EDGE_NEG                 Negative edge detection
// EDGE_DIS                 No edge detection
#define IOC_IOC12_EDGEDET_W                                                  2U
#define IOC_IOC12_EDGEDET_M                                         0x00030000U
#define IOC_IOC12_EDGEDET_S                                                 16U
#define IOC_IOC12_EDGEDET_EDGE_BOTH                                 0x00030000U
#define IOC_IOC12_EDGEDET_EDGE_POS                                  0x00020000U
#define IOC_IOC12_EDGEDET_EDGE_NEG                                  0x00010000U
#define IOC_IOC12_EDGEDET_EDGE_DIS                                  0x00000000U

// Field: [14:13] PULLCTL
//
// Pull control. Setting this to value 0x3 disables pull.
// ENUMs:
// PULL_UP                  Pull up enabled
// PULL_DOWN                Pull down enabled
// PULL_DIS                 No pull
#define IOC_IOC12_PULLCTL_W                                                  2U
#define IOC_IOC12_PULLCTL_M                                         0x00006000U
#define IOC_IOC12_PULLCTL_S                                                 13U
#define IOC_IOC12_PULLCTL_PULL_UP                                   0x00004000U
#define IOC_IOC12_PULLCTL_PULL_DOWN                                 0x00002000U
#define IOC_IOC12_PULLCTL_PULL_DIS                                  0x00000000U

// Field:    [12] SLEWRED
//
// Slew rate configuration
// ENUMs:
// REDUCED                  Reduced slew rate
// NORMAL                   Normal slew rate
#define IOC_IOC12_SLEWRED                                           0x00001000U
#define IOC_IOC12_SLEWRED_M                                         0x00001000U
#define IOC_IOC12_SLEWRED_S                                                 12U
#define IOC_IOC12_SLEWRED_REDUCED                                   0x00001000U
#define IOC_IOC12_SLEWRED_NORMAL                                    0x00000000U

// Field: [11:10] IOCURR
//
// Output current configuration. Writing value 0x3 defaults to 2mA current
// setting.
// ENUMs:
// CUR_8MA
// CUR_4MA
// CUR_2MA
#define IOC_IOC12_IOCURR_W                                                   2U
#define IOC_IOC12_IOCURR_M                                          0x00000C00U
#define IOC_IOC12_IOCURR_S                                                  10U
#define IOC_IOC12_IOCURR_CUR_8MA                                    0x00000800U
#define IOC_IOC12_IOCURR_CUR_4MA                                    0x00000400U
#define IOC_IOC12_IOCURR_CUR_2MA                                    0x00000000U

// Field:   [9:8] IOSTR
//
// Drive strength configuration
// ENUMs:
// MAX                      Maximum drive strength
// MEDIUM                   Medium drive strength
// MIN                      Minimum drive stregnth
// AUTO                     Automatic drive strength adjustment
#define IOC_IOC12_IOSTR_W                                                    2U
#define IOC_IOC12_IOSTR_M                                           0x00000300U
#define IOC_IOC12_IOSTR_S                                                    8U
#define IOC_IOC12_IOSTR_MAX                                         0x00000300U
#define IOC_IOC12_IOSTR_MEDIUM                                      0x00000200U
#define IOC_IOC12_IOSTR_MIN                                         0x00000100U
#define IOC_IOC12_IOSTR_AUTO                                        0x00000000U

// Field:   [2:0] PORTCFG
//
// Port configuration.
// ENUMs:
// DTB                      Digital Test Bus function
// ANA                      Analog function
// PFUNC5                   Digital peripheral function-5
// PFUNC4                   Digital peripheral function-4
// PFUNC3                   Digital peripheral function-3
// PFUNC2                   Digital peripheral function-2
// PFUNC1                   Digital peripheral function-1
// BASE                     Base function
#define IOC_IOC12_PORTCFG_W                                                  3U
#define IOC_IOC12_PORTCFG_M                                         0x00000007U
#define IOC_IOC12_PORTCFG_S                                                  0U
#define IOC_IOC12_PORTCFG_DTB                                       0x00000007U
#define IOC_IOC12_PORTCFG_ANA                                       0x00000006U
#define IOC_IOC12_PORTCFG_PFUNC5                                    0x00000005U
#define IOC_IOC12_PORTCFG_PFUNC4                                    0x00000004U
#define IOC_IOC12_PORTCFG_PFUNC3                                    0x00000003U
#define IOC_IOC12_PORTCFG_PFUNC2                                    0x00000002U
#define IOC_IOC12_PORTCFG_PFUNC1                                    0x00000001U
#define IOC_IOC12_PORTCFG_BASE                                      0x00000000U

//*****************************************************************************
//
// Register: IOC_O_IOC13
//
//*****************************************************************************
// Field:    [30] HYSTEN
//
// Input hysteresis enable
// ENUMs:
// ENABLE                   Input hysteresis enabled
// DISABLE                  Input hysteresis disabled
#define IOC_IOC13_HYSTEN                                            0x40000000U
#define IOC_IOC13_HYSTEN_M                                          0x40000000U
#define IOC_IOC13_HYSTEN_S                                                  30U
#define IOC_IOC13_HYSTEN_ENABLE                                     0x40000000U
#define IOC_IOC13_HYSTEN_DISABLE                                    0x00000000U

// Field:    [29] INPEN
//
// Input enable
// ENUMs:
// ENABLE                   Input enabled
// DISABLE                  Input disabled
#define IOC_IOC13_INPEN                                             0x20000000U
#define IOC_IOC13_INPEN_M                                           0x20000000U
#define IOC_IOC13_INPEN_S                                                   29U
#define IOC_IOC13_INPEN_ENABLE                                      0x20000000U
#define IOC_IOC13_INPEN_DISABLE                                     0x00000000U

// Field: [26:24] IOMODE
//
// IO Mode. Setting this to value 0x6 or 0x7 will default to normal IO
// behavior.
// ENUMs:
// OPENS_INV                Open Source, inverted IO
// OPENS                    Open Source, normal IO
// OPEND_INV                Open Drain, inverted IO
// OPEND                    Open Drain, normal IO
// INVERTED                 Inverted IO
// NORMAL                   Normal IO
#define IOC_IOC13_IOMODE_W                                                   3U
#define IOC_IOC13_IOMODE_M                                          0x07000000U
#define IOC_IOC13_IOMODE_S                                                  24U
#define IOC_IOC13_IOMODE_OPENS_INV                                  0x05000000U
#define IOC_IOC13_IOMODE_OPENS                                      0x04000000U
#define IOC_IOC13_IOMODE_OPEND_INV                                  0x03000000U
#define IOC_IOC13_IOMODE_OPEND                                      0x02000000U
#define IOC_IOC13_IOMODE_INVERTED                                   0x01000000U
#define IOC_IOC13_IOMODE_NORMAL                                     0x00000000U

// Field: [21:20] WUCFGSD
//
// Wakeup configuration from shutdown
// ENUMs:
// WAKE_HIGH                wakeup triggered when pad level is high
// WAKE_LOW                 wakeup triggered when pad level is low
// DISABLED_1               wakeup disabled
// DISABLED_0               wakeup disabled
#define IOC_IOC13_WUCFGSD_W                                                  2U
#define IOC_IOC13_WUCFGSD_M                                         0x00300000U
#define IOC_IOC13_WUCFGSD_S                                                 20U
#define IOC_IOC13_WUCFGSD_WAKE_HIGH                                 0x00300000U
#define IOC_IOC13_WUCFGSD_WAKE_LOW                                  0x00200000U
#define IOC_IOC13_WUCFGSD_DISABLED_1                                0x00100000U
#define IOC_IOC13_WUCFGSD_DISABLED_0                                0x00000000U

// Field:    [18] WUENSB
//
// Wakeup enable from standby
// ENUMs:
// ENABLE                   wakeup enabled (effective only if EDGEDET is
//                          enabled)
// DISABLE                  wakeup disabled
#define IOC_IOC13_WUENSB                                            0x00040000U
#define IOC_IOC13_WUENSB_M                                          0x00040000U
#define IOC_IOC13_WUENSB_S                                                  18U
#define IOC_IOC13_WUENSB_ENABLE                                     0x00040000U
#define IOC_IOC13_WUENSB_DISABLE                                    0x00000000U

// Field: [17:16] EDGEDET
//
// Edge detect configuration
// ENUMs:
// EDGE_BOTH                Positive and negative edge detection
// EDGE_POS                 Positive edge detection
// EDGE_NEG                 Negative edge detection
// EDGE_DIS                 No edge detection
#define IOC_IOC13_EDGEDET_W                                                  2U
#define IOC_IOC13_EDGEDET_M                                         0x00030000U
#define IOC_IOC13_EDGEDET_S                                                 16U
#define IOC_IOC13_EDGEDET_EDGE_BOTH                                 0x00030000U
#define IOC_IOC13_EDGEDET_EDGE_POS                                  0x00020000U
#define IOC_IOC13_EDGEDET_EDGE_NEG                                  0x00010000U
#define IOC_IOC13_EDGEDET_EDGE_DIS                                  0x00000000U

// Field: [14:13] PULLCTL
//
// Pull control. Setting this to value 0x3 disables pull.
// ENUMs:
// PULL_UP                  Pull up enabled
// PULL_DOWN                Pull down enabled
// PULL_DIS                 No pull
#define IOC_IOC13_PULLCTL_W                                                  2U
#define IOC_IOC13_PULLCTL_M                                         0x00006000U
#define IOC_IOC13_PULLCTL_S                                                 13U
#define IOC_IOC13_PULLCTL_PULL_UP                                   0x00004000U
#define IOC_IOC13_PULLCTL_PULL_DOWN                                 0x00002000U
#define IOC_IOC13_PULLCTL_PULL_DIS                                  0x00000000U

// Field:   [2:0] PORTCFG
//
// Port configuration.
// ENUMs:
// DTB                      Digital Test Bus function
// ANA                      Analog function
// PFUNC5                   Digital peripheral function-5
// PFUNC4                   Digital peripheral function-4
// PFUNC3                   Digital peripheral function-3
// PFUNC2                   Digital peripheral function-2
// PFUNC1                   Digital peripheral function-1
// BASE                     Base function
#define IOC_IOC13_PORTCFG_W                                                  3U
#define IOC_IOC13_PORTCFG_M                                         0x00000007U
#define IOC_IOC13_PORTCFG_S                                                  0U
#define IOC_IOC13_PORTCFG_DTB                                       0x00000007U
#define IOC_IOC13_PORTCFG_ANA                                       0x00000006U
#define IOC_IOC13_PORTCFG_PFUNC5                                    0x00000005U
#define IOC_IOC13_PORTCFG_PFUNC4                                    0x00000004U
#define IOC_IOC13_PORTCFG_PFUNC3                                    0x00000003U
#define IOC_IOC13_PORTCFG_PFUNC2                                    0x00000002U
#define IOC_IOC13_PORTCFG_PFUNC1                                    0x00000001U
#define IOC_IOC13_PORTCFG_BASE                                      0x00000000U

//*****************************************************************************
//
// Register: IOC_O_IOC16
//
//*****************************************************************************
// Field:    [30] HYSTEN
//
// Input hysteresis enable
// ENUMs:
// ENABLE                   Input hysteresis enabled
// DISABLE                  Input hysteresis disabled
#define IOC_IOC16_HYSTEN                                            0x40000000U
#define IOC_IOC16_HYSTEN_M                                          0x40000000U
#define IOC_IOC16_HYSTEN_S                                                  30U
#define IOC_IOC16_HYSTEN_ENABLE                                     0x40000000U
#define IOC_IOC16_HYSTEN_DISABLE                                    0x00000000U

// Field:    [29] INPEN
//
// Input enable
// ENUMs:
// ENABLE                   Input enabled
// DISABLE                  Input disabled
#define IOC_IOC16_INPEN                                             0x20000000U
#define IOC_IOC16_INPEN_M                                           0x20000000U
#define IOC_IOC16_INPEN_S                                                   29U
#define IOC_IOC16_INPEN_ENABLE                                      0x20000000U
#define IOC_IOC16_INPEN_DISABLE                                     0x00000000U

// Field: [26:24] IOMODE
//
// IO Mode. Setting this to value 0x6 or 0x7 will default to normal IO
// behavior.
// ENUMs:
// OPENS_INV                Open Source, inverted IO
// OPENS                    Open Source, normal IO
// OPEND_INV                Open Drain, inverted IO
// OPEND                    Open Drain, normal IO
// INVERTED                 Inverted IO
// NORMAL                   Normal IO
#define IOC_IOC16_IOMODE_W                                                   3U
#define IOC_IOC16_IOMODE_M                                          0x07000000U
#define IOC_IOC16_IOMODE_S                                                  24U
#define IOC_IOC16_IOMODE_OPENS_INV                                  0x05000000U
#define IOC_IOC16_IOMODE_OPENS                                      0x04000000U
#define IOC_IOC16_IOMODE_OPEND_INV                                  0x03000000U
#define IOC_IOC16_IOMODE_OPEND                                      0x02000000U
#define IOC_IOC16_IOMODE_INVERTED                                   0x01000000U
#define IOC_IOC16_IOMODE_NORMAL                                     0x00000000U

// Field: [21:20] WUCFGSD
//
// Wakeup configuration from shutdown
// ENUMs:
// WAKE_HIGH                wakeup triggered when pad level is high
// WAKE_LOW                 wakeup triggered when pad level is low
// DISABLED_1               wakeup disabled
// DISABLED_0               wakeup disabled
#define IOC_IOC16_WUCFGSD_W                                                  2U
#define IOC_IOC16_WUCFGSD_M                                         0x00300000U
#define IOC_IOC16_WUCFGSD_S                                                 20U
#define IOC_IOC16_WUCFGSD_WAKE_HIGH                                 0x00300000U
#define IOC_IOC16_WUCFGSD_WAKE_LOW                                  0x00200000U
#define IOC_IOC16_WUCFGSD_DISABLED_1                                0x00100000U
#define IOC_IOC16_WUCFGSD_DISABLED_0                                0x00000000U

// Field:    [18] WUENSB
//
// Wakeup enable from standby
// ENUMs:
// ENABLE                   wakeup enabled (effective only if EDGEDET is
//                          enabled)
// DISABLE                  wakeup disabled
#define IOC_IOC16_WUENSB                                            0x00040000U
#define IOC_IOC16_WUENSB_M                                          0x00040000U
#define IOC_IOC16_WUENSB_S                                                  18U
#define IOC_IOC16_WUENSB_ENABLE                                     0x00040000U
#define IOC_IOC16_WUENSB_DISABLE                                    0x00000000U

// Field: [17:16] EDGEDET
//
// Edge detect configuration
// ENUMs:
// EDGE_BOTH                Positive and negative edge detection
// EDGE_POS                 Positive edge detection
// EDGE_NEG                 Negative edge detection
// EDGE_DIS                 No edge detection
#define IOC_IOC16_EDGEDET_W                                                  2U
#define IOC_IOC16_EDGEDET_M                                         0x00030000U
#define IOC_IOC16_EDGEDET_S                                                 16U
#define IOC_IOC16_EDGEDET_EDGE_BOTH                                 0x00030000U
#define IOC_IOC16_EDGEDET_EDGE_POS                                  0x00020000U
#define IOC_IOC16_EDGEDET_EDGE_NEG                                  0x00010000U
#define IOC_IOC16_EDGEDET_EDGE_DIS                                  0x00000000U

// Field: [14:13] PULLCTL
//
// Pull control. Setting this to value 0x3 disables pull.
// ENUMs:
// PULL_UP                  Pull up enabled
// PULL_DOWN                Pull down enabled
// PULL_DIS                 No pull
#define IOC_IOC16_PULLCTL_W                                                  2U
#define IOC_IOC16_PULLCTL_M                                         0x00006000U
#define IOC_IOC16_PULLCTL_S                                                 13U
#define IOC_IOC16_PULLCTL_PULL_UP                                   0x00004000U
#define IOC_IOC16_PULLCTL_PULL_DOWN                                 0x00002000U
#define IOC_IOC16_PULLCTL_PULL_DIS                                  0x00000000U

// Field:    [12] SLEWRED
//
// Slew rate configuration
// ENUMs:
// REDUCED                  Reduced slew rate
// NORMAL                   Normal slew rate
#define IOC_IOC16_SLEWRED                                           0x00001000U
#define IOC_IOC16_SLEWRED_M                                         0x00001000U
#define IOC_IOC16_SLEWRED_S                                                 12U
#define IOC_IOC16_SLEWRED_REDUCED                                   0x00001000U
#define IOC_IOC16_SLEWRED_NORMAL                                    0x00000000U

// Field: [11:10] IOCURR
//
// Output current configuration. Writing value 0x3 defaults to 2mA current
// setting.
// ENUMs:
// CUR_8MA
// CUR_4MA
// CUR_2MA
#define IOC_IOC16_IOCURR_W                                                   2U
#define IOC_IOC16_IOCURR_M                                          0x00000C00U
#define IOC_IOC16_IOCURR_S                                                  10U
#define IOC_IOC16_IOCURR_CUR_8MA                                    0x00000800U
#define IOC_IOC16_IOCURR_CUR_4MA                                    0x00000400U
#define IOC_IOC16_IOCURR_CUR_2MA                                    0x00000000U

// Field:   [9:8] IOSTR
//
// Drive strength configuration
// ENUMs:
// MAX                      Maximum drive strength
// MEDIUM                   Medium drive strength
// MIN                      Minimum drive stregnth
// AUTO                     Automatic drive strength adjustment
#define IOC_IOC16_IOSTR_W                                                    2U
#define IOC_IOC16_IOSTR_M                                           0x00000300U
#define IOC_IOC16_IOSTR_S                                                    8U
#define IOC_IOC16_IOSTR_MAX                                         0x00000300U
#define IOC_IOC16_IOSTR_MEDIUM                                      0x00000200U
#define IOC_IOC16_IOSTR_MIN                                         0x00000100U
#define IOC_IOC16_IOSTR_AUTO                                        0x00000000U

// Field:   [2:0] PORTCFG
//
// Port configuration.
// ENUMs:
// DTB                      Digital Test Bus function
// ANA                      Analog function
// PFUNC5                   Digital peripheral function-5
// PFUNC4                   Digital peripheral function-4
// PFUNC3                   Digital peripheral function-3
// PFUNC2                   Digital peripheral function-2
// PFUNC1                   Digital peripheral function-1
// BASE                     Base function
#define IOC_IOC16_PORTCFG_W                                                  3U
#define IOC_IOC16_PORTCFG_M                                         0x00000007U
#define IOC_IOC16_PORTCFG_S                                                  0U
#define IOC_IOC16_PORTCFG_DTB                                       0x00000007U
#define IOC_IOC16_PORTCFG_ANA                                       0x00000006U
#define IOC_IOC16_PORTCFG_PFUNC5                                    0x00000005U
#define IOC_IOC16_PORTCFG_PFUNC4                                    0x00000004U
#define IOC_IOC16_PORTCFG_PFUNC3                                    0x00000003U
#define IOC_IOC16_PORTCFG_PFUNC2                                    0x00000002U
#define IOC_IOC16_PORTCFG_PFUNC1                                    0x00000001U
#define IOC_IOC16_PORTCFG_BASE                                      0x00000000U

//*****************************************************************************
//
// Register: IOC_O_IOC17
//
//*****************************************************************************
// Field:    [30] HYSTEN
//
// Input hysteresis enable
// ENUMs:
// ENABLE                   Input hysteresis enabled
// DISABLE                  Input hysteresis disabled
#define IOC_IOC17_HYSTEN                                            0x40000000U
#define IOC_IOC17_HYSTEN_M                                          0x40000000U
#define IOC_IOC17_HYSTEN_S                                                  30U
#define IOC_IOC17_HYSTEN_ENABLE                                     0x40000000U
#define IOC_IOC17_HYSTEN_DISABLE                                    0x00000000U

// Field:    [29] INPEN
//
// Input enable
// ENUMs:
// ENABLE                   Input enabled
// DISABLE                  Input disabled
#define IOC_IOC17_INPEN                                             0x20000000U
#define IOC_IOC17_INPEN_M                                           0x20000000U
#define IOC_IOC17_INPEN_S                                                   29U
#define IOC_IOC17_INPEN_ENABLE                                      0x20000000U
#define IOC_IOC17_INPEN_DISABLE                                     0x00000000U

// Field: [26:24] IOMODE
//
// IO Mode. Setting this to value 0x6 or 0x7 will default to normal IO
// behavior.
// ENUMs:
// OPENS_INV                Open Source, inverted IO
// OPENS                    Open Source, normal IO
// OPEND_INV                Open Drain, inverted IO
// OPEND                    Open Drain, normal IO
// INVERTED                 Inverted IO
// NORMAL                   Normal IO
#define IOC_IOC17_IOMODE_W                                                   3U
#define IOC_IOC17_IOMODE_M                                          0x07000000U
#define IOC_IOC17_IOMODE_S                                                  24U
#define IOC_IOC17_IOMODE_OPENS_INV                                  0x05000000U
#define IOC_IOC17_IOMODE_OPENS                                      0x04000000U
#define IOC_IOC17_IOMODE_OPEND_INV                                  0x03000000U
#define IOC_IOC17_IOMODE_OPEND                                      0x02000000U
#define IOC_IOC17_IOMODE_INVERTED                                   0x01000000U
#define IOC_IOC17_IOMODE_NORMAL                                     0x00000000U

// Field: [21:20] WUCFGSD
//
// Wakeup configuration from shutdown
// ENUMs:
// WAKE_HIGH                wakeup triggered when pad level is high
// WAKE_LOW                 wakeup triggered when pad level is low
// DISABLED_1               wakeup disabled
// DISABLED_0               wakeup disabled
#define IOC_IOC17_WUCFGSD_W                                                  2U
#define IOC_IOC17_WUCFGSD_M                                         0x00300000U
#define IOC_IOC17_WUCFGSD_S                                                 20U
#define IOC_IOC17_WUCFGSD_WAKE_HIGH                                 0x00300000U
#define IOC_IOC17_WUCFGSD_WAKE_LOW                                  0x00200000U
#define IOC_IOC17_WUCFGSD_DISABLED_1                                0x00100000U
#define IOC_IOC17_WUCFGSD_DISABLED_0                                0x00000000U

// Field:    [18] WUENSB
//
// Wakeup enable from standby
// ENUMs:
// ENABLE                   wakeup enabled (effective only if EDGEDET is
//                          enabled)
// DISABLE                  wakeup disabled
#define IOC_IOC17_WUENSB                                            0x00040000U
#define IOC_IOC17_WUENSB_M                                          0x00040000U
#define IOC_IOC17_WUENSB_S                                                  18U
#define IOC_IOC17_WUENSB_ENABLE                                     0x00040000U
#define IOC_IOC17_WUENSB_DISABLE                                    0x00000000U

// Field: [17:16] EDGEDET
//
// Edge detect configuration
// ENUMs:
// EDGE_BOTH                Positive and negative edge detection
// EDGE_POS                 Positive edge detection
// EDGE_NEG                 Negative edge detection
// EDGE_DIS                 No edge detection
#define IOC_IOC17_EDGEDET_W                                                  2U
#define IOC_IOC17_EDGEDET_M                                         0x00030000U
#define IOC_IOC17_EDGEDET_S                                                 16U
#define IOC_IOC17_EDGEDET_EDGE_BOTH                                 0x00030000U
#define IOC_IOC17_EDGEDET_EDGE_POS                                  0x00020000U
#define IOC_IOC17_EDGEDET_EDGE_NEG                                  0x00010000U
#define IOC_IOC17_EDGEDET_EDGE_DIS                                  0x00000000U

// Field: [14:13] PULLCTL
//
// Pull control. Setting this to value 0x3 disables pull.
// ENUMs:
// PULL_UP                  Pull up enabled
// PULL_DOWN                Pull down enabled
// PULL_DIS                 No pull
#define IOC_IOC17_PULLCTL_W                                                  2U
#define IOC_IOC17_PULLCTL_M                                         0x00006000U
#define IOC_IOC17_PULLCTL_S                                                 13U
#define IOC_IOC17_PULLCTL_PULL_UP                                   0x00004000U
#define IOC_IOC17_PULLCTL_PULL_DOWN                                 0x00002000U
#define IOC_IOC17_PULLCTL_PULL_DIS                                  0x00000000U

// Field:    [12] SLEWRED
//
// Slew rate configuration
// ENUMs:
// REDUCED                  Reduced slew rate
// NORMAL                   Normal slew rate
#define IOC_IOC17_SLEWRED                                           0x00001000U
#define IOC_IOC17_SLEWRED_M                                         0x00001000U
#define IOC_IOC17_SLEWRED_S                                                 12U
#define IOC_IOC17_SLEWRED_REDUCED                                   0x00001000U
#define IOC_IOC17_SLEWRED_NORMAL                                    0x00000000U

// Field: [11:10] IOCURR
//
// Output current configuration. Writing value 0x3 defaults to 2mA current
// setting.
// ENUMs:
// CUR_8MA
// CUR_4MA
// CUR_2MA
#define IOC_IOC17_IOCURR_W                                                   2U
#define IOC_IOC17_IOCURR_M                                          0x00000C00U
#define IOC_IOC17_IOCURR_S                                                  10U
#define IOC_IOC17_IOCURR_CUR_8MA                                    0x00000800U
#define IOC_IOC17_IOCURR_CUR_4MA                                    0x00000400U
#define IOC_IOC17_IOCURR_CUR_2MA                                    0x00000000U

// Field:   [9:8] IOSTR
//
// Drive strength configuration
// ENUMs:
// MAX                      Maximum drive strength
// MEDIUM                   Medium drive strength
// MIN                      Minimum drive stregnth
// AUTO                     Automatic drive strength adjustment
#define IOC_IOC17_IOSTR_W                                                    2U
#define IOC_IOC17_IOSTR_M                                           0x00000300U
#define IOC_IOC17_IOSTR_S                                                    8U
#define IOC_IOC17_IOSTR_MAX                                         0x00000300U
#define IOC_IOC17_IOSTR_MEDIUM                                      0x00000200U
#define IOC_IOC17_IOSTR_MIN                                         0x00000100U
#define IOC_IOC17_IOSTR_AUTO                                        0x00000000U

// Field:   [2:0] PORTCFG
//
// Port configuration.
// ENUMs:
// DTB                      Digital Test Bus function
// ANA                      Analog function
// PFUNC5                   Digital peripheral function-5
// PFUNC4                   Digital peripheral function-4
// PFUNC3                   Digital peripheral function-3
// PFUNC2                   Digital peripheral function-2
// PFUNC1                   Digital peripheral function-1
// BASE                     Base function
#define IOC_IOC17_PORTCFG_W                                                  3U
#define IOC_IOC17_PORTCFG_M                                         0x00000007U
#define IOC_IOC17_PORTCFG_S                                                  0U
#define IOC_IOC17_PORTCFG_DTB                                       0x00000007U
#define IOC_IOC17_PORTCFG_ANA                                       0x00000006U
#define IOC_IOC17_PORTCFG_PFUNC5                                    0x00000005U
#define IOC_IOC17_PORTCFG_PFUNC4                                    0x00000004U
#define IOC_IOC17_PORTCFG_PFUNC3                                    0x00000003U
#define IOC_IOC17_PORTCFG_PFUNC2                                    0x00000002U
#define IOC_IOC17_PORTCFG_PFUNC1                                    0x00000001U
#define IOC_IOC17_PORTCFG_BASE                                      0x00000000U

//*****************************************************************************
//
// Register: IOC_O_IOC20
//
//*****************************************************************************
// Field:    [30] HYSTEN
//
// Input hysteresis enable
// ENUMs:
// ENABLE                   Input hysteresis enabled
// DISABLE                  Input hysteresis disabled
#define IOC_IOC20_HYSTEN                                            0x40000000U
#define IOC_IOC20_HYSTEN_M                                          0x40000000U
#define IOC_IOC20_HYSTEN_S                                                  30U
#define IOC_IOC20_HYSTEN_ENABLE                                     0x40000000U
#define IOC_IOC20_HYSTEN_DISABLE                                    0x00000000U

// Field:    [29] INPEN
//
// Input enable
// ENUMs:
// ENABLE                   Input enabled
// DISABLE                  Input disabled
#define IOC_IOC20_INPEN                                             0x20000000U
#define IOC_IOC20_INPEN_M                                           0x20000000U
#define IOC_IOC20_INPEN_S                                                   29U
#define IOC_IOC20_INPEN_ENABLE                                      0x20000000U
#define IOC_IOC20_INPEN_DISABLE                                     0x00000000U

// Field: [26:24] IOMODE
//
// IO Mode. Setting this to value 0x6 or 0x7 will default to normal IO
// behavior.
// ENUMs:
// OPENS_INV                Open Source, inverted IO
// OPENS                    Open Source, normal IO
// OPEND_INV                Open Drain, inverted IO
// OPEND                    Open Drain, normal IO
// INVERTED                 Inverted IO
// NORMAL                   Normal IO
#define IOC_IOC20_IOMODE_W                                                   3U
#define IOC_IOC20_IOMODE_M                                          0x07000000U
#define IOC_IOC20_IOMODE_S                                                  24U
#define IOC_IOC20_IOMODE_OPENS_INV                                  0x05000000U
#define IOC_IOC20_IOMODE_OPENS                                      0x04000000U
#define IOC_IOC20_IOMODE_OPEND_INV                                  0x03000000U
#define IOC_IOC20_IOMODE_OPEND                                      0x02000000U
#define IOC_IOC20_IOMODE_INVERTED                                   0x01000000U
#define IOC_IOC20_IOMODE_NORMAL                                     0x00000000U

// Field: [21:20] WUCFGSD
//
// Wakeup configuration from shutdown
// ENUMs:
// WAKE_HIGH                wakeup triggered when pad level is high
// WAKE_LOW                 wakeup triggered when pad level is low
// DISABLED_1               wakeup disabled
// DISABLED_0               wakeup disabled
#define IOC_IOC20_WUCFGSD_W                                                  2U
#define IOC_IOC20_WUCFGSD_M                                         0x00300000U
#define IOC_IOC20_WUCFGSD_S                                                 20U
#define IOC_IOC20_WUCFGSD_WAKE_HIGH                                 0x00300000U
#define IOC_IOC20_WUCFGSD_WAKE_LOW                                  0x00200000U
#define IOC_IOC20_WUCFGSD_DISABLED_1                                0x00100000U
#define IOC_IOC20_WUCFGSD_DISABLED_0                                0x00000000U

// Field:    [18] WUENSB
//
// Wakeup enable from standby
// ENUMs:
// ENABLE                   wakeup enabled (effective only if EDGEDET is
//                          enabled)
// DISABLE                  wakeup disabled
#define IOC_IOC20_WUENSB                                            0x00040000U
#define IOC_IOC20_WUENSB_M                                          0x00040000U
#define IOC_IOC20_WUENSB_S                                                  18U
#define IOC_IOC20_WUENSB_ENABLE                                     0x00040000U
#define IOC_IOC20_WUENSB_DISABLE                                    0x00000000U

// Field: [17:16] EDGEDET
//
// Edge detect configuration
// ENUMs:
// EDGE_BOTH                Positive and negative edge detection
// EDGE_POS                 Positive edge detection
// EDGE_NEG                 Negative edge detection
// EDGE_DIS                 No edge detection
#define IOC_IOC20_EDGEDET_W                                                  2U
#define IOC_IOC20_EDGEDET_M                                         0x00030000U
#define IOC_IOC20_EDGEDET_S                                                 16U
#define IOC_IOC20_EDGEDET_EDGE_BOTH                                 0x00030000U
#define IOC_IOC20_EDGEDET_EDGE_POS                                  0x00020000U
#define IOC_IOC20_EDGEDET_EDGE_NEG                                  0x00010000U
#define IOC_IOC20_EDGEDET_EDGE_DIS                                  0x00000000U

// Field: [14:13] PULLCTL
//
// Pull control. Setting this to value 0x3 disables pull.
// ENUMs:
// PULL_UP                  Pull up enabled
// PULL_DOWN                Pull down enabled
// PULL_DIS                 No pull
#define IOC_IOC20_PULLCTL_W                                                  2U
#define IOC_IOC20_PULLCTL_M                                         0x00006000U
#define IOC_IOC20_PULLCTL_S                                                 13U
#define IOC_IOC20_PULLCTL_PULL_UP                                   0x00004000U
#define IOC_IOC20_PULLCTL_PULL_DOWN                                 0x00002000U
#define IOC_IOC20_PULLCTL_PULL_DIS                                  0x00000000U

// Field:   [2:0] PORTCFG
//
// Port configuration.
// ENUMs:
// DTB                      Digital Test Bus function
// ANA                      Analog function
// PFUNC5                   Digital peripheral function-5
// PFUNC4                   Digital peripheral function-4
// PFUNC3                   Digital peripheral function-3
// PFUNC2                   Digital peripheral function-2
// PFUNC1                   Digital peripheral function-1
// BASE                     Base function
#define IOC_IOC20_PORTCFG_W                                                  3U
#define IOC_IOC20_PORTCFG_M                                         0x00000007U
#define IOC_IOC20_PORTCFG_S                                                  0U
#define IOC_IOC20_PORTCFG_DTB                                       0x00000007U
#define IOC_IOC20_PORTCFG_ANA                                       0x00000006U
#define IOC_IOC20_PORTCFG_PFUNC5                                    0x00000005U
#define IOC_IOC20_PORTCFG_PFUNC4                                    0x00000004U
#define IOC_IOC20_PORTCFG_PFUNC3                                    0x00000003U
#define IOC_IOC20_PORTCFG_PFUNC2                                    0x00000002U
#define IOC_IOC20_PORTCFG_PFUNC1                                    0x00000001U
#define IOC_IOC20_PORTCFG_BASE                                      0x00000000U

//*****************************************************************************
//
// Register: IOC_O_IOC21
//
//*****************************************************************************
// Field:    [30] HYSTEN
//
// Input hysteresis enable
// ENUMs:
// ENABLE                   Input hysteresis enabled
// DISABLE                  Input hysteresis disabled
#define IOC_IOC21_HYSTEN                                            0x40000000U
#define IOC_IOC21_HYSTEN_M                                          0x40000000U
#define IOC_IOC21_HYSTEN_S                                                  30U
#define IOC_IOC21_HYSTEN_ENABLE                                     0x40000000U
#define IOC_IOC21_HYSTEN_DISABLE                                    0x00000000U

// Field:    [29] INPEN
//
// Input enable
// ENUMs:
// ENABLE                   Input enabled
// DISABLE                  Input disabled
#define IOC_IOC21_INPEN                                             0x20000000U
#define IOC_IOC21_INPEN_M                                           0x20000000U
#define IOC_IOC21_INPEN_S                                                   29U
#define IOC_IOC21_INPEN_ENABLE                                      0x20000000U
#define IOC_IOC21_INPEN_DISABLE                                     0x00000000U

// Field: [26:24] IOMODE
//
// IO Mode. Setting this to value 0x6 or 0x7 will default to normal IO
// behavior.
// ENUMs:
// OPENS_INV                Open Source, inverted IO
// OPENS                    Open Source, normal IO
// OPEND_INV                Open Drain, inverted IO
// OPEND                    Open Drain, normal IO
// INVERTED                 Inverted IO
// NORMAL                   Normal IO
#define IOC_IOC21_IOMODE_W                                                   3U
#define IOC_IOC21_IOMODE_M                                          0x07000000U
#define IOC_IOC21_IOMODE_S                                                  24U
#define IOC_IOC21_IOMODE_OPENS_INV                                  0x05000000U
#define IOC_IOC21_IOMODE_OPENS                                      0x04000000U
#define IOC_IOC21_IOMODE_OPEND_INV                                  0x03000000U
#define IOC_IOC21_IOMODE_OPEND                                      0x02000000U
#define IOC_IOC21_IOMODE_INVERTED                                   0x01000000U
#define IOC_IOC21_IOMODE_NORMAL                                     0x00000000U

// Field: [21:20] WUCFGSD
//
// Wakeup configuration from shutdown
// ENUMs:
// WAKE_HIGH                wakeup triggered when pad level is high
// WAKE_LOW                 wakeup triggered when pad level is low
// DISABLED_1               wakeup disabled
// DISABLED_0               wakeup disabled
#define IOC_IOC21_WUCFGSD_W                                                  2U
#define IOC_IOC21_WUCFGSD_M                                         0x00300000U
#define IOC_IOC21_WUCFGSD_S                                                 20U
#define IOC_IOC21_WUCFGSD_WAKE_HIGH                                 0x00300000U
#define IOC_IOC21_WUCFGSD_WAKE_LOW                                  0x00200000U
#define IOC_IOC21_WUCFGSD_DISABLED_1                                0x00100000U
#define IOC_IOC21_WUCFGSD_DISABLED_0                                0x00000000U

// Field:    [18] WUENSB
//
// Wakeup enable from standby
// ENUMs:
// ENABLE                   wakeup enabled (effective only if EDGEDET is
//                          enabled)
// DISABLE                  wakeup disabled
#define IOC_IOC21_WUENSB                                            0x00040000U
#define IOC_IOC21_WUENSB_M                                          0x00040000U
#define IOC_IOC21_WUENSB_S                                                  18U
#define IOC_IOC21_WUENSB_ENABLE                                     0x00040000U
#define IOC_IOC21_WUENSB_DISABLE                                    0x00000000U

// Field: [17:16] EDGEDET
//
// Edge detect configuration
// ENUMs:
// EDGE_BOTH                Positive and negative edge detection
// EDGE_POS                 Positive edge detection
// EDGE_NEG                 Negative edge detection
// EDGE_DIS                 No edge detection
#define IOC_IOC21_EDGEDET_W                                                  2U
#define IOC_IOC21_EDGEDET_M                                         0x00030000U
#define IOC_IOC21_EDGEDET_S                                                 16U
#define IOC_IOC21_EDGEDET_EDGE_BOTH                                 0x00030000U
#define IOC_IOC21_EDGEDET_EDGE_POS                                  0x00020000U
#define IOC_IOC21_EDGEDET_EDGE_NEG                                  0x00010000U
#define IOC_IOC21_EDGEDET_EDGE_DIS                                  0x00000000U

// Field: [14:13] PULLCTL
//
// Pull control. Setting this to value 0x3 disables pull.
// ENUMs:
// PULL_UP                  Pull up enabled
// PULL_DOWN                Pull down enabled
// PULL_DIS                 No pull
#define IOC_IOC21_PULLCTL_W                                                  2U
#define IOC_IOC21_PULLCTL_M                                         0x00006000U
#define IOC_IOC21_PULLCTL_S                                                 13U
#define IOC_IOC21_PULLCTL_PULL_UP                                   0x00004000U
#define IOC_IOC21_PULLCTL_PULL_DOWN                                 0x00002000U
#define IOC_IOC21_PULLCTL_PULL_DIS                                  0x00000000U

// Field:   [2:0] PORTCFG
//
// Port configuration.
// ENUMs:
// DTB                      Digital Test Bus function
// ANA                      Analog function
// PFUNC5                   Digital peripheral function-5
// PFUNC4                   Digital peripheral function-4
// PFUNC3                   Digital peripheral function-3
// PFUNC2                   Digital peripheral function-2
// PFUNC1                   Digital peripheral function-1
// BASE                     Base function
#define IOC_IOC21_PORTCFG_W                                                  3U
#define IOC_IOC21_PORTCFG_M                                         0x00000007U
#define IOC_IOC21_PORTCFG_S                                                  0U
#define IOC_IOC21_PORTCFG_DTB                                       0x00000007U
#define IOC_IOC21_PORTCFG_ANA                                       0x00000006U
#define IOC_IOC21_PORTCFG_PFUNC5                                    0x00000005U
#define IOC_IOC21_PORTCFG_PFUNC4                                    0x00000004U
#define IOC_IOC21_PORTCFG_PFUNC3                                    0x00000003U
#define IOC_IOC21_PORTCFG_PFUNC2                                    0x00000002U
#define IOC_IOC21_PORTCFG_PFUNC1                                    0x00000001U
#define IOC_IOC21_PORTCFG_BASE                                      0x00000000U

//*****************************************************************************
//
// Register: IOC_O_IOC24
//
//*****************************************************************************
// Field:    [30] HYSTEN
//
// Input hysteresis enable
// ENUMs:
// ENABLE                   Input hysteresis enabled
// DISABLE                  Input hysteresis disabled
#define IOC_IOC24_HYSTEN                                            0x40000000U
#define IOC_IOC24_HYSTEN_M                                          0x40000000U
#define IOC_IOC24_HYSTEN_S                                                  30U
#define IOC_IOC24_HYSTEN_ENABLE                                     0x40000000U
#define IOC_IOC24_HYSTEN_DISABLE                                    0x00000000U

// Field:    [29] INPEN
//
// Input enable
// ENUMs:
// ENABLE                   Input enabled
// DISABLE                  Input disabled
#define IOC_IOC24_INPEN                                             0x20000000U
#define IOC_IOC24_INPEN_M                                           0x20000000U
#define IOC_IOC24_INPEN_S                                                   29U
#define IOC_IOC24_INPEN_ENABLE                                      0x20000000U
#define IOC_IOC24_INPEN_DISABLE                                     0x00000000U

// Field: [26:24] IOMODE
//
// IO Mode. Setting this to value 0x6 or 0x7 will default to normal IO
// behavior.
// ENUMs:
// OPENS_INV                Open Source, inverted IO
// OPENS                    Open Source, normal IO
// OPEND_INV                Open Drain, inverted IO
// OPEND                    Open Drain, normal IO
// INVERTED                 Inverted IO
// NORMAL                   Normal IO
#define IOC_IOC24_IOMODE_W                                                   3U
#define IOC_IOC24_IOMODE_M                                          0x07000000U
#define IOC_IOC24_IOMODE_S                                                  24U
#define IOC_IOC24_IOMODE_OPENS_INV                                  0x05000000U
#define IOC_IOC24_IOMODE_OPENS                                      0x04000000U
#define IOC_IOC24_IOMODE_OPEND_INV                                  0x03000000U
#define IOC_IOC24_IOMODE_OPEND                                      0x02000000U
#define IOC_IOC24_IOMODE_INVERTED                                   0x01000000U
#define IOC_IOC24_IOMODE_NORMAL                                     0x00000000U

// Field: [21:20] WUCFGSD
//
// Wakeup configuration from shutdown
// ENUMs:
// WAKE_HIGH                wakeup triggered when pad level is high
// WAKE_LOW                 wakeup triggered when pad level is low
// DISABLED_1               wakeup disabled
// DISABLED_0               wakeup disabled
#define IOC_IOC24_WUCFGSD_W                                                  2U
#define IOC_IOC24_WUCFGSD_M                                         0x00300000U
#define IOC_IOC24_WUCFGSD_S                                                 20U
#define IOC_IOC24_WUCFGSD_WAKE_HIGH                                 0x00300000U
#define IOC_IOC24_WUCFGSD_WAKE_LOW                                  0x00200000U
#define IOC_IOC24_WUCFGSD_DISABLED_1                                0x00100000U
#define IOC_IOC24_WUCFGSD_DISABLED_0                                0x00000000U

// Field:    [18] WUENSB
//
// Wakeup enable from standby
// ENUMs:
// ENABLE                   wakeup enabled (effective only if EDGEDET is
//                          enabled)
// DISABLE                  wakeup disabled
#define IOC_IOC24_WUENSB                                            0x00040000U
#define IOC_IOC24_WUENSB_M                                          0x00040000U
#define IOC_IOC24_WUENSB_S                                                  18U
#define IOC_IOC24_WUENSB_ENABLE                                     0x00040000U
#define IOC_IOC24_WUENSB_DISABLE                                    0x00000000U

// Field: [17:16] EDGEDET
//
// Edge detect configuration
// ENUMs:
// EDGE_BOTH                Positive and negative edge detection
// EDGE_POS                 Positive edge detection
// EDGE_NEG                 Negative edge detection
// EDGE_DIS                 No edge detection
#define IOC_IOC24_EDGEDET_W                                                  2U
#define IOC_IOC24_EDGEDET_M                                         0x00030000U
#define IOC_IOC24_EDGEDET_S                                                 16U
#define IOC_IOC24_EDGEDET_EDGE_BOTH                                 0x00030000U
#define IOC_IOC24_EDGEDET_EDGE_POS                                  0x00020000U
#define IOC_IOC24_EDGEDET_EDGE_NEG                                  0x00010000U
#define IOC_IOC24_EDGEDET_EDGE_DIS                                  0x00000000U

// Field: [14:13] PULLCTL
//
// Pull control. Setting this to value 0x3 disables pull.
// ENUMs:
// PULL_UP                  Pull up enabled
// PULL_DOWN                Pull down enabled
// PULL_DIS                 No pull
#define IOC_IOC24_PULLCTL_W                                                  2U
#define IOC_IOC24_PULLCTL_M                                         0x00006000U
#define IOC_IOC24_PULLCTL_S                                                 13U
#define IOC_IOC24_PULLCTL_PULL_UP                                   0x00004000U
#define IOC_IOC24_PULLCTL_PULL_DOWN                                 0x00002000U
#define IOC_IOC24_PULLCTL_PULL_DIS                                  0x00000000U

// Field:    [12] SLEWRED
//
// Slew rate configuration
// ENUMs:
// REDUCED                  Reduced slew rate
// NORMAL                   Normal slew rate
#define IOC_IOC24_SLEWRED                                           0x00001000U
#define IOC_IOC24_SLEWRED_M                                         0x00001000U
#define IOC_IOC24_SLEWRED_S                                                 12U
#define IOC_IOC24_SLEWRED_REDUCED                                   0x00001000U
#define IOC_IOC24_SLEWRED_NORMAL                                    0x00000000U

// Field: [11:10] IOCURR
//
// Output current configuration. Writing value 0x3 defaults to 2mA current
// setting.
// ENUMs:
// CUR_8MA
// CUR_4MA
// CUR_2MA
#define IOC_IOC24_IOCURR_W                                                   2U
#define IOC_IOC24_IOCURR_M                                          0x00000C00U
#define IOC_IOC24_IOCURR_S                                                  10U
#define IOC_IOC24_IOCURR_CUR_8MA                                    0x00000800U
#define IOC_IOC24_IOCURR_CUR_4MA                                    0x00000400U
#define IOC_IOC24_IOCURR_CUR_2MA                                    0x00000000U

// Field:   [9:8] IOSTR
//
// Drive strength configuration
// ENUMs:
// MAX                      Maximum drive strength
// MEDIUM                   Medium drive strength
// MIN                      Minimum drive stregnth
// AUTO                     Automatic drive strength adjustment
#define IOC_IOC24_IOSTR_W                                                    2U
#define IOC_IOC24_IOSTR_M                                           0x00000300U
#define IOC_IOC24_IOSTR_S                                                    8U
#define IOC_IOC24_IOSTR_MAX                                         0x00000300U
#define IOC_IOC24_IOSTR_MEDIUM                                      0x00000200U
#define IOC_IOC24_IOSTR_MIN                                         0x00000100U
#define IOC_IOC24_IOSTR_AUTO                                        0x00000000U

// Field:   [2:0] PORTCFG
//
// Port configuration.
// ENUMs:
// DTB                      Digital Test Bus function
// ANA                      Analog function
// PFUNC5                   Digital peripheral function-5
// PFUNC4                   Digital peripheral function-4
// PFUNC3                   Digital peripheral function-3
// PFUNC2                   Digital peripheral function-2
// PFUNC1                   Digital peripheral function-1
// BASE                     Base function
#define IOC_IOC24_PORTCFG_W                                                  3U
#define IOC_IOC24_PORTCFG_M                                         0x00000007U
#define IOC_IOC24_PORTCFG_S                                                  0U
#define IOC_IOC24_PORTCFG_DTB                                       0x00000007U
#define IOC_IOC24_PORTCFG_ANA                                       0x00000006U
#define IOC_IOC24_PORTCFG_PFUNC5                                    0x00000005U
#define IOC_IOC24_PORTCFG_PFUNC4                                    0x00000004U
#define IOC_IOC24_PORTCFG_PFUNC3                                    0x00000003U
#define IOC_IOC24_PORTCFG_PFUNC2                                    0x00000002U
#define IOC_IOC24_PORTCFG_PFUNC1                                    0x00000001U
#define IOC_IOC24_PORTCFG_BASE                                      0x00000000U

//*****************************************************************************
//
// Register: IOC_O_DTBCFG
//
//*****************************************************************************
// Field:    [23] DTB0DIV
//
// This bit is used to divide DTB[0] output by 8.
// ENUMs:
// ENABLE                   Divide DTB[0] output by 8
// DISABLE                  No divide
#define IOC_DTBCFG_DTB0DIV                                          0x00800000U
#define IOC_DTBCFG_DTB0DIV_M                                        0x00800000U
#define IOC_DTBCFG_DTB0DIV_S                                                23U
#define IOC_DTBCFG_DTB0DIV_ENABLE                                   0x00800000U
#define IOC_DTBCFG_DTB0DIV_DISABLE                                  0x00000000U

// Field: [18:16] PADSEL
//
// Selects which 3 DTB lines out of total 16 are routed to DTB pins 15 to 13.
// ENUMs:
// DTB2TO0                  DTB[2:0] selected
// DTB5TO3                  DTB[5:3] selected
// DTB8TO6                  DTB[8:6] selected
// DTB11TO9                 DTB[11:9] selected
// DTB14TO12                DTB[14:12] selected
// DTB15TO13                DTB[15:13] selected
#define IOC_DTBCFG_PADSEL_W                                                  3U
#define IOC_DTBCFG_PADSEL_M                                         0x00070000U
#define IOC_DTBCFG_PADSEL_S                                                 16U
#define IOC_DTBCFG_PADSEL_DTB2TO0                                   0x00050000U
#define IOC_DTBCFG_PADSEL_DTB5TO3                                   0x00040000U
#define IOC_DTBCFG_PADSEL_DTB8TO6                                   0x00030000U
#define IOC_DTBCFG_PADSEL_DTB11TO9                                  0x00020000U
#define IOC_DTBCFG_PADSEL_DTB14TO12                                 0x00010000U
#define IOC_DTBCFG_PADSEL_DTB15TO13                                 0x00000000U

// Field:  [12:8] ULLSEL
//
// ULL DTB Mux selection
// ENUMs:
// MAXIMUM                  Maximum value
// MINIMUM                  Minimum value
#define IOC_DTBCFG_ULLSEL_W                                                  5U
#define IOC_DTBCFG_ULLSEL_M                                         0x00001F00U
#define IOC_DTBCFG_ULLSEL_S                                                  8U
#define IOC_DTBCFG_ULLSEL_MAXIMUM                                   0x00001F00U
#define IOC_DTBCFG_ULLSEL_MINIMUM                                   0x00000000U

// Field:   [4:0] SVTSEL
//
// SVT DTB Mux selection
// ENUMs:
// MAXIMUM                  Maximum value
// MINIMUM                  Minimum value
#define IOC_DTBCFG_SVTSEL_W                                                  5U
#define IOC_DTBCFG_SVTSEL_M                                         0x0000001FU
#define IOC_DTBCFG_SVTSEL_S                                                  0U
#define IOC_DTBCFG_SVTSEL_MAXIMUM                                   0x0000001FU
#define IOC_DTBCFG_SVTSEL_MINIMUM                                   0x00000000U

//*****************************************************************************
//
// Register: IOC_O_DTBOE
//
//*****************************************************************************
// Field:    [15] EN15
//
// Enables DTB output 15
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN15                                              0x00008000U
#define IOC_DTBOE_EN15_M                                            0x00008000U
#define IOC_DTBOE_EN15_S                                                    15U
#define IOC_DTBOE_EN15_ENABLE                                       0x00008000U
#define IOC_DTBOE_EN15_DISABLE                                      0x00000000U

// Field:    [14] EN14
//
// Enables DTB output 14
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN14                                              0x00004000U
#define IOC_DTBOE_EN14_M                                            0x00004000U
#define IOC_DTBOE_EN14_S                                                    14U
#define IOC_DTBOE_EN14_ENABLE                                       0x00004000U
#define IOC_DTBOE_EN14_DISABLE                                      0x00000000U

// Field:    [13] EN13
//
// Enables DTB output 13
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN13                                              0x00002000U
#define IOC_DTBOE_EN13_M                                            0x00002000U
#define IOC_DTBOE_EN13_S                                                    13U
#define IOC_DTBOE_EN13_ENABLE                                       0x00002000U
#define IOC_DTBOE_EN13_DISABLE                                      0x00000000U

// Field:    [12] EN12
//
// Enables DTB output 12
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN12                                              0x00001000U
#define IOC_DTBOE_EN12_M                                            0x00001000U
#define IOC_DTBOE_EN12_S                                                    12U
#define IOC_DTBOE_EN12_ENABLE                                       0x00001000U
#define IOC_DTBOE_EN12_DISABLE                                      0x00000000U

// Field:    [11] EN11
//
// Enables DTB output 11
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN11                                              0x00000800U
#define IOC_DTBOE_EN11_M                                            0x00000800U
#define IOC_DTBOE_EN11_S                                                    11U
#define IOC_DTBOE_EN11_ENABLE                                       0x00000800U
#define IOC_DTBOE_EN11_DISABLE                                      0x00000000U

// Field:    [10] EN10
//
// Enables DTB output 10
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN10                                              0x00000400U
#define IOC_DTBOE_EN10_M                                            0x00000400U
#define IOC_DTBOE_EN10_S                                                    10U
#define IOC_DTBOE_EN10_ENABLE                                       0x00000400U
#define IOC_DTBOE_EN10_DISABLE                                      0x00000000U

// Field:     [9] EN9
//
// Enables DTB output 9
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN9                                               0x00000200U
#define IOC_DTBOE_EN9_M                                             0x00000200U
#define IOC_DTBOE_EN9_S                                                      9U
#define IOC_DTBOE_EN9_ENABLE                                        0x00000200U
#define IOC_DTBOE_EN9_DISABLE                                       0x00000000U

// Field:     [8] EN8
//
// Enables DTB output 8
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN8                                               0x00000100U
#define IOC_DTBOE_EN8_M                                             0x00000100U
#define IOC_DTBOE_EN8_S                                                      8U
#define IOC_DTBOE_EN8_ENABLE                                        0x00000100U
#define IOC_DTBOE_EN8_DISABLE                                       0x00000000U

// Field:     [7] EN7
//
// Enables DTB output 7
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN7                                               0x00000080U
#define IOC_DTBOE_EN7_M                                             0x00000080U
#define IOC_DTBOE_EN7_S                                                      7U
#define IOC_DTBOE_EN7_ENABLE                                        0x00000080U
#define IOC_DTBOE_EN7_DISABLE                                       0x00000000U

// Field:     [6] EN6
//
// Enables DTB output 6
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN6                                               0x00000040U
#define IOC_DTBOE_EN6_M                                             0x00000040U
#define IOC_DTBOE_EN6_S                                                      6U
#define IOC_DTBOE_EN6_ENABLE                                        0x00000040U
#define IOC_DTBOE_EN6_DISABLE                                       0x00000000U

// Field:     [5] EN5
//
// Enables DTB output 5
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN5                                               0x00000020U
#define IOC_DTBOE_EN5_M                                             0x00000020U
#define IOC_DTBOE_EN5_S                                                      5U
#define IOC_DTBOE_EN5_ENABLE                                        0x00000020U
#define IOC_DTBOE_EN5_DISABLE                                       0x00000000U

// Field:     [4] EN4
//
// Enables DTB output 4
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN4                                               0x00000010U
#define IOC_DTBOE_EN4_M                                             0x00000010U
#define IOC_DTBOE_EN4_S                                                      4U
#define IOC_DTBOE_EN4_ENABLE                                        0x00000010U
#define IOC_DTBOE_EN4_DISABLE                                       0x00000000U

// Field:     [3] EN3
//
// Enables DTB output 3
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN3                                               0x00000008U
#define IOC_DTBOE_EN3_M                                             0x00000008U
#define IOC_DTBOE_EN3_S                                                      3U
#define IOC_DTBOE_EN3_ENABLE                                        0x00000008U
#define IOC_DTBOE_EN3_DISABLE                                       0x00000000U

// Field:     [2] EN2
//
// Enables DTB output 2
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN2                                               0x00000004U
#define IOC_DTBOE_EN2_M                                             0x00000004U
#define IOC_DTBOE_EN2_S                                                      2U
#define IOC_DTBOE_EN2_ENABLE                                        0x00000004U
#define IOC_DTBOE_EN2_DISABLE                                       0x00000000U

// Field:     [1] EN1
//
// Enables DTB output 1
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN1                                               0x00000002U
#define IOC_DTBOE_EN1_M                                             0x00000002U
#define IOC_DTBOE_EN1_S                                                      1U
#define IOC_DTBOE_EN1_ENABLE                                        0x00000002U
#define IOC_DTBOE_EN1_DISABLE                                       0x00000000U

// Field:     [0] EN0
//
// Enables DTB output 0
// ENUMs:
// ENABLE                   DTB output enabled
// DISABLE                  DTB output disabled
#define IOC_DTBOE_EN0                                               0x00000001U
#define IOC_DTBOE_EN0_M                                             0x00000001U
#define IOC_DTBOE_EN0_S                                                      0U
#define IOC_DTBOE_EN0_ENABLE                                        0x00000001U
#define IOC_DTBOE_EN0_DISABLE                                       0x00000000U

//*****************************************************************************
//
// Register: IOC_O_EVTCFG
//
//*****************************************************************************
// Field:     [8] EVTIFG
//
// Event flag. It is set when edge is detected on selected DIO.
// Note: The edge detector flop is cleared for the selected DIO when EVTIFG is
// cleared by software.
// ENUMs:
// SET
// CLR                      Clear
#define IOC_EVTCFG_EVTIFG                                           0x00000100U
#define IOC_EVTCFG_EVTIFG_M                                         0x00000100U
#define IOC_EVTCFG_EVTIFG_S                                                  8U
#define IOC_EVTCFG_EVTIFG_SET                                       0x00000100U
#define IOC_EVTCFG_EVTIFG_CLR                                       0x00000000U

// Field:     [7] EVTEN
//
// Enables IOC to publish event on AON event fabric when EVTIFG is set.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define IOC_EVTCFG_EVTEN                                            0x00000080U
#define IOC_EVTCFG_EVTEN_M                                          0x00000080U
#define IOC_EVTCFG_EVTEN_S                                                   7U
#define IOC_EVTCFG_EVTEN_ENABLE                                     0x00000080U
#define IOC_EVTCFG_EVTEN_DISABLE                                    0x00000000U

// Field:   [5:0] DIOSEL
//
// This is used to select DIO for event generation. For example, DIOSEL = 0x0
// selects DIO0 and DIOSEL = 0x8 selects DIO8.
// ENUMs:
// MAXIMUM                  Maximum value
// MINIMUM                  Minimum value
#define IOC_EVTCFG_DIOSEL_W                                                  6U
#define IOC_EVTCFG_DIOSEL_M                                         0x0000003FU
#define IOC_EVTCFG_DIOSEL_S                                                  0U
#define IOC_EVTCFG_DIOSEL_MAXIMUM                                   0x0000003FU
#define IOC_EVTCFG_DIOSEL_MINIMUM                                   0x00000000U

//*****************************************************************************
//
// Register: IOC_O_TEST
//
//*****************************************************************************
// Field:     [0] SEL
//
// This is used to drive SWDIO output data and output enable from debug
// sub-system onto DIO12 (TDO) pad.
// ENUMs:
// HIGH                     Output data and output enable driven based on
//                          debug sub-system inputs
// LOW                      Output data and output enable managed by IOC
#define IOC_TEST_SEL                                                0x00000001U
#define IOC_TEST_SEL_M                                              0x00000001U
#define IOC_TEST_SEL_S                                                       0U
#define IOC_TEST_SEL_HIGH                                           0x00000001U
#define IOC_TEST_SEL_LOW                                            0x00000000U

//*****************************************************************************
//
// Register: IOC_O_DTBSTAT
//
//*****************************************************************************
// Field:  [15:0] VAL
//
// This bit field captures the final 16-bit value of DTB signals provided from
// IOC to DTB device pins.
#define IOC_DTBSTAT_VAL_W                                                   16U
#define IOC_DTBSTAT_VAL_M                                           0x0000FFFFU
#define IOC_DTBSTAT_VAL_S                                                    0U


#endif // __IOC__
