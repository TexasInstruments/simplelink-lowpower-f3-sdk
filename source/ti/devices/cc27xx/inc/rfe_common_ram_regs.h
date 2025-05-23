/*
 * Copyright (c) 2018-2025, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 /*
 *  ======== rfe_common_ram_regs.h ========
*/

#ifndef __RFE_COMMON_RAM_REGS_H
#define __RFE_COMMON_RAM_REGS_H

//******************************************************************************
// REGISTER OFFSETS
//******************************************************************************
// RFE Synth Controls
#define RFE_COMMON_RAM_O_SYNTHCTL                                    0x00000804U

// TDC Calibration 0
#define RFE_COMMON_RAM_O_TDCCAL0                                     0x00000806U

// TDC Calibration 1
#define RFE_COMMON_RAM_O_TDCCAL1                                     0x00000808U

// TDC Calibration 2
#define RFE_COMMON_RAM_O_TDCCAL2                                     0x0000080AU

// TDC Stop Time PLL Configuration
#define RFE_COMMON_RAM_O_TDCPLL                                      0x0000080CU

// Derived Constant For KDCO Gain Estimation
#define RFE_COMMON_RAM_O_K1LSB                                       0x0000080EU

// Derived Constant For KDCO Gain Estimation
#define RFE_COMMON_RAM_O_K1MSB                                       0x00000810U

// Derived Constant Used to Calculate Pre-Lock Loop Filter Ki
#define RFE_COMMON_RAM_O_K2BL                                        0x00000812U

// Derived Constant Used to Calculate Post-Lock Loop Filter Ki
#define RFE_COMMON_RAM_O_K2AL                                        0x00000814U

// Derived Constant Used to Calculate Pre-Lock Loop Filter Kp
#define RFE_COMMON_RAM_O_K3BL                                        0x00000816U

// Derived Constant Used to Calculate Post-Lock Loop Filter Kp
#define RFE_COMMON_RAM_O_K3AL                                        0x00000818U

// Derived Constant Used to Calculate DTX Gain
#define RFE_COMMON_RAM_O_K5                                          0x0000081AU

// Receive Intermediate Frequency
#define RFE_COMMON_RAM_O_RXIF                                        0x0000081CU

// Transmit Intermediate Frequency
#define RFE_COMMON_RAM_O_TXIF                                        0x0000081EU

// RTRIM Offset
#define RFE_COMMON_RAM_O_RTRIMOFF                                    0x00000820U

// Minimum RTRIM Value
#define RFE_COMMON_RAM_O_RTRIMMIN                                    0x00000822U

// Divider Initial Control
#define RFE_COMMON_RAM_O_DIVI                                        0x00000824U

// Divider Final Control
#define RFE_COMMON_RAM_O_DIVF                                        0x00000826U

// Divider LDO Initial Control
#define RFE_COMMON_RAM_O_DIVLDOI                                     0x00000828U

// Divider LDO Final Control
#define RFE_COMMON_RAM_O_DIVLDOF                                     0x0000082AU

// 
#define RFE_COMMON_RAM_O_DIVLDOIOFF                                  0x0000082CU

// ALO Power Up LDO Settling Time
#define RFE_COMMON_RAM_O_LDOSETTLE                                   0x0000082EU

// Charge Injection Settling Time
#define RFE_COMMON_RAM_O_CHRGSETTLE                                  0x00000830U

// DCOLDO Settling Time
#define RFE_COMMON_RAM_O_DCOSETTLE                                   0x00000832U

// 
#define RFE_COMMON_RAM_O_IFAMPRFLDOTX                                0x00000834U

// 
#define RFE_COMMON_RAM_O_IFAMPRFLDODEFAULT                           0x00000836U

// Loop Filter Pre-Lock Ki 
#define RFE_COMMON_RAM_O_LFKIBL                                      0x00000838U

// Loop Filter Pre-Lock Kp
#define RFE_COMMON_RAM_O_LFKPBL                                      0x0000083AU

// Phy specific RSSI offset
#define RFE_COMMON_RAM_O_PHYRSSIOFFSET                               0x0000083CU

// Shadow register for SPARE0
#define RFE_COMMON_RAM_O_SPARE0SHADOW                                0x0000083EU

// Shadow register for SPARE1
#define RFE_COMMON_RAM_O_SPARE1SHADOW                                0x00000840U

// AGC type information
#define RFE_COMMON_RAM_O_AGCINFO                                     0x00000842U

// COEX GRANT pin information
#define RFE_COMMON_RAM_O_GRANTPIN                                    0x00000844U

// PA Trim for Mode 0 and Mode 1
#define RFE_COMMON_RAM_O_PATRIM01                                    0x00000846U

// PA Trim for Mode 2 and Mode 3
#define RFE_COMMON_RAM_O_PATRIM23                                    0x00000848U

// Estimated 2^24/(KDCO)
#define RFE_COMMON_RAM_O_IKT                                         0x0000084AU

//******************************************************************************
// Register: SYNTHCTL
//******************************************************************************
// Field: [9:9] pheadj
//
// Phase error adjust control.
#define RFE_COMMON_RAM_SYNTHCTL_PHEADJ                                   0x0200U
#define RFE_COMMON_RAM_SYNTHCTL_PHEADJ_M                                 0x0200U
#define RFE_COMMON_RAM_SYNTHCTL_PHEADJ_S                                      9U
#define RFE_COMMON_RAM_SYNTHCTL_PHEADJ_DIS                               0x0000U
#define RFE_COMMON_RAM_SYNTHCTL_PHEADJ_EN                                0x0200U

// Field: [8:8] lfsyncatgrshft
//
// Loop filter synchronization at gear shift
#define RFE_COMMON_RAM_SYNTHCTL_LFSYNCATGRSHFT                           0x0100U
#define RFE_COMMON_RAM_SYNTHCTL_LFSYNCATGRSHFT_M                         0x0100U
#define RFE_COMMON_RAM_SYNTHCTL_LFSYNCATGRSHFT_S                              8U
#define RFE_COMMON_RAM_SYNTHCTL_LFSYNCATGRSHFT_DIS                       0x0000U
#define RFE_COMMON_RAM_SYNTHCTL_LFSYNCATGRSHFT_EN                        0x0100U

// Field: [7:7] chrgfilt
//
// Control dynamic control of CHRGFILT
#define RFE_COMMON_RAM_SYNTHCTL_CHRGFILT                                 0x0080U
#define RFE_COMMON_RAM_SYNTHCTL_CHRGFILT_M                               0x0080U
#define RFE_COMMON_RAM_SYNTHCTL_CHRGFILT_S                                    7U

// Field: [6:6] vrefbp
//
// Control dynamic control of VREFBYPASS
#define RFE_COMMON_RAM_SYNTHCTL_VREFBP                                   0x0040U
#define RFE_COMMON_RAM_SYNTHCTL_VREFBP_M                                 0x0040U
#define RFE_COMMON_RAM_SYNTHCTL_VREFBP_S                                      6U

// Field: [5:5] txwaitmod
//
// Wait for modem or not when starting TX
#define RFE_COMMON_RAM_SYNTHCTL_TXWAITMOD                                0x0020U
#define RFE_COMMON_RAM_SYNTHCTL_TXWAITMOD_M                              0x0020U
#define RFE_COMMON_RAM_SYNTHCTL_TXWAITMOD_S                                   5U

// Field: [4:4] phedisc
//
// Phase Error Discard Control For TX and RX
#define RFE_COMMON_RAM_SYNTHCTL_PHEDISC                                  0x0010U
#define RFE_COMMON_RAM_SYNTHCTL_PHEDISC_M                                0x0010U
#define RFE_COMMON_RAM_SYNTHCTL_PHEDISC_S                                     4U
#define RFE_COMMON_RAM_SYNTHCTL_PHEDISC_DIS                              0x0000U
#define RFE_COMMON_RAM_SYNTHCTL_PHEDISC_EN                               0x0010U

// Field: [3:3] rtrimtst
//
// DCO Amplitude Trimming Mode
#define RFE_COMMON_RAM_SYNTHCTL_RTRIMTST                                 0x0008U
#define RFE_COMMON_RAM_SYNTHCTL_RTRIMTST_M                               0x0008U
#define RFE_COMMON_RAM_SYNTHCTL_RTRIMTST_S                                    3U
#define RFE_COMMON_RAM_SYNTHCTL_RTRIMTST_DIS                             0x0000U
#define RFE_COMMON_RAM_SYNTHCTL_RTRIMTST_EN                              0x0008U

// Field: [2:2] iir
//
// Post-lock IIR Filter Control
#define RFE_COMMON_RAM_SYNTHCTL_IIR                                      0x0004U
#define RFE_COMMON_RAM_SYNTHCTL_IIR_M                                    0x0004U
#define RFE_COMMON_RAM_SYNTHCTL_IIR_S                                         2U
#define RFE_COMMON_RAM_SYNTHCTL_IIR_DIS                                  0x0000U
#define RFE_COMMON_RAM_SYNTHCTL_IIR_EN                                   0x0004U

// Field: [1:1] refdthr
//
// Post-lock Reference Dithering Control
#define RFE_COMMON_RAM_SYNTHCTL_REFDTHR                                  0x0002U
#define RFE_COMMON_RAM_SYNTHCTL_REFDTHR_M                                0x0002U
#define RFE_COMMON_RAM_SYNTHCTL_REFDTHR_S                                     1U
#define RFE_COMMON_RAM_SYNTHCTL_REFDTHR_DIS                              0x0000U
#define RFE_COMMON_RAM_SYNTHCTL_REFDTHR_EN                               0x0002U

// Field: [0:0] fcdem
//
// TX Finecode DEM Control
#define RFE_COMMON_RAM_SYNTHCTL_FCDEM                                    0x0001U
#define RFE_COMMON_RAM_SYNTHCTL_FCDEM_M                                  0x0001U
#define RFE_COMMON_RAM_SYNTHCTL_FCDEM_S                                       0U
#define RFE_COMMON_RAM_SYNTHCTL_FCDEM_DIS                                0x0000U
#define RFE_COMMON_RAM_SYNTHCTL_FCDEM_EN                                 0x0001U

//******************************************************************************
// Register: TDCCAL0
//******************************************************************************
// Field: [10:8] stop
//
// Stop configuration value. When CFG.RTRIMTST = 1, this field shall be copied into TDCPLL.STOP (RCL-L/TFW).
#define RFE_COMMON_RAM_TDCCAL0_STOP_W                                         3U
#define RFE_COMMON_RAM_TDCCAL0_STOP_M                                    0x0700U
#define RFE_COMMON_RAM_TDCCAL0_STOP_S                                         8U

//******************************************************************************
// Register: TDCCAL1
//******************************************************************************
// Field: [7:0] sub
//
// Offset to subtract from TDC data during TDC gain calibration.
#define RFE_COMMON_RAM_TDCCAL1_SUB_W                                          8U
#define RFE_COMMON_RAM_TDCCAL1_SUB_M                                     0x00FFU
#define RFE_COMMON_RAM_TDCCAL1_SUB_S                                          0U

//******************************************************************************
// Register: TDCCAL2
//******************************************************************************
// Field: [5:0] avg
//
// Averaging constant, AVG =  2^CAL0.TDCAVG * (TDCCAL0.STOP+1).
#define RFE_COMMON_RAM_TDCCAL2_AVG_W                                          6U
#define RFE_COMMON_RAM_TDCCAL2_AVG_M                                     0x003FU
#define RFE_COMMON_RAM_TDCCAL2_AVG_S                                          0U

//******************************************************************************
// Register: TDCPLL
//******************************************************************************
// Field: [10:8] stop
//
// Stop configuration value.
#define RFE_COMMON_RAM_TDCPLL_STOP_W                                          3U
#define RFE_COMMON_RAM_TDCPLL_STOP_M                                     0x0700U
#define RFE_COMMON_RAM_TDCPLL_STOP_S                                          8U

//******************************************************************************
// Register: K1LSB
//******************************************************************************
// Field: [15:0] val
//
// LSB value.
#define RFE_COMMON_RAM_K1LSB_VAL_W                                           16U
#define RFE_COMMON_RAM_K1LSB_VAL_M                                       0xFFFFU
#define RFE_COMMON_RAM_K1LSB_VAL_S                                            0U

//******************************************************************************
// Register: K1MSB
//******************************************************************************
// Field: [15:0] val
//
// MSB value.
#define RFE_COMMON_RAM_K1MSB_VAL_W                                           16U
#define RFE_COMMON_RAM_K1MSB_VAL_M                                       0xFFFFU
#define RFE_COMMON_RAM_K1MSB_VAL_S                                            0U

//******************************************************************************
// Register: K2BL
//******************************************************************************
// Field: [15:15] hpm
//
// High Precision Mode
#define RFE_COMMON_RAM_K2BL_HPM                                          0x8000U
#define RFE_COMMON_RAM_K2BL_HPM_M                                        0x8000U
#define RFE_COMMON_RAM_K2BL_HPM_S                                            15U

// Field: [14:0] val
//
// Value
#define RFE_COMMON_RAM_K2BL_VAL_W                                            15U
#define RFE_COMMON_RAM_K2BL_VAL_M                                        0x7FFFU
#define RFE_COMMON_RAM_K2BL_VAL_S                                             0U

//******************************************************************************
// Register: K2AL
//******************************************************************************
// Field: [15:15] hpm
//
// High Precision Mode
#define RFE_COMMON_RAM_K2AL_HPM                                          0x8000U
#define RFE_COMMON_RAM_K2AL_HPM_M                                        0x8000U
#define RFE_COMMON_RAM_K2AL_HPM_S                                            15U

// Field: [14:0] val
//
// Value
#define RFE_COMMON_RAM_K2AL_VAL_W                                            15U
#define RFE_COMMON_RAM_K2AL_VAL_M                                        0x7FFFU
#define RFE_COMMON_RAM_K2AL_VAL_S                                             0U

//******************************************************************************
// Register: K3BL
//******************************************************************************
// Field: [15:0] val
//
// Value
#define RFE_COMMON_RAM_K3BL_VAL_W                                            16U
#define RFE_COMMON_RAM_K3BL_VAL_M                                        0xFFFFU
#define RFE_COMMON_RAM_K3BL_VAL_S                                             0U

//******************************************************************************
// Register: K3AL
//******************************************************************************
// Field: [15:0] val
//
// Value
#define RFE_COMMON_RAM_K3AL_VAL_W                                            16U
#define RFE_COMMON_RAM_K3AL_VAL_M                                        0xFFFFU
#define RFE_COMMON_RAM_K3AL_VAL_S                                             0U

//******************************************************************************
// Register: K5
//******************************************************************************
// Field: [15:0] val
//
// Value
#define RFE_COMMON_RAM_K5_VAL_W                                              16U
#define RFE_COMMON_RAM_K5_VAL_M                                          0xFFFFU
#define RFE_COMMON_RAM_K5_VAL_S                                               0U

//******************************************************************************
// Register: RXIF
//******************************************************************************
// Field: [11:0] foff
//
// Signed frequency offset.
#define RFE_COMMON_RAM_RXIF_FOFF_W                                           12U
#define RFE_COMMON_RAM_RXIF_FOFF_M                                       0x0FFFU
#define RFE_COMMON_RAM_RXIF_FOFF_S                                            0U

//******************************************************************************
// Register: TXIF
//******************************************************************************
// Field: [11:0] foff
//
// Signed frequency offset.
#define RFE_COMMON_RAM_TXIF_FOFF_W                                           12U
#define RFE_COMMON_RAM_TXIF_FOFF_M                                       0x0FFFU
#define RFE_COMMON_RAM_TXIF_FOFF_S                                            0U

//******************************************************************************
// Register: RTRIMOFF
//******************************************************************************
// Field: [3:0] val
//
// Unsigned offset to be added to possibly temperature compensated RTRIM from FCFG.
#define RFE_COMMON_RAM_RTRIMOFF_VAL_W                                         4U
#define RFE_COMMON_RAM_RTRIMOFF_VAL_M                                    0x000FU
#define RFE_COMMON_RAM_RTRIMOFF_VAL_S                                         0U

//******************************************************************************
// Register: RTRIMMIN
//******************************************************************************
// Field: [3:0] val
//
// Possibly temperature compensated RTRIM from FCFG adjusted by RTRIMOFF.VAL must be at least this value.
#define RFE_COMMON_RAM_RTRIMMIN_VAL_W                                         4U
#define RFE_COMMON_RAM_RTRIMMIN_VAL_M                                    0x000FU
#define RFE_COMMON_RAM_RTRIMMIN_VAL_S                                         0U

//******************************************************************************
// Register: DIVI
//******************************************************************************
// Field: [15:15] pdet
//
// Peak Detector Mode
#define RFE_COMMON_RAM_DIVI_PDET                                         0x8000U
#define RFE_COMMON_RAM_DIVI_PDET_M                                       0x8000U
#define RFE_COMMON_RAM_DIVI_PDET_S                                           15U

// Field: [14:12] nmireftrim
//
// NMOS Bias Voltage Trim
#define RFE_COMMON_RAM_DIVI_NMIREFTRIM_W                                      3U
#define RFE_COMMON_RAM_DIVI_NMIREFTRIM_M                                 0x7000U
#define RFE_COMMON_RAM_DIVI_NMIREFTRIM_S                                     12U

// Field: [11:9] pmireftrim
//
// PMOS Bias Voltage Trim
#define RFE_COMMON_RAM_DIVI_PMIREFTRIM_W                                      3U
#define RFE_COMMON_RAM_DIVI_PMIREFTRIM_M                                 0x0E00U
#define RFE_COMMON_RAM_DIVI_PMIREFTRIM_S                                      9U

// Field: [8:8] txboost
//
// TX Buffer Boost
#define RFE_COMMON_RAM_DIVI_TXBOOST                                      0x0100U
#define RFE_COMMON_RAM_DIVI_TXBOOST_M                                    0x0100U
#define RFE_COMMON_RAM_DIVI_TXBOOST_S                                         8U
#define RFE_COMMON_RAM_DIVI_TXBOOST_DEFAULT                              0x0000U
#define RFE_COMMON_RAM_DIVI_TXBOOST_INCREASED                            0x0100U

// Field: [7:7] s1gfrc
//
// S1G Power Switch Force
#define RFE_COMMON_RAM_DIVI_S1GFRC                                       0x0080U
#define RFE_COMMON_RAM_DIVI_S1GFRC_M                                     0x0080U
#define RFE_COMMON_RAM_DIVI_S1GFRC_S                                          7U
#define RFE_COMMON_RAM_DIVI_S1GFRC_DIS                                   0x0000U
#define RFE_COMMON_RAM_DIVI_S1GFRC_EN                                    0x0080U

// Field: [6:5] bufgain
//
// Not connected or used in LRF
#define RFE_COMMON_RAM_DIVI_BUFGAIN_W                                         2U
#define RFE_COMMON_RAM_DIVI_BUFGAIN_M                                    0x0060U
#define RFE_COMMON_RAM_DIVI_BUFGAIN_S                                         5U

// Field: [4:4] bias
//
// Not connected or used in LRF
#define RFE_COMMON_RAM_DIVI_BIAS                                         0x0010U
#define RFE_COMMON_RAM_DIVI_BIAS_M                                       0x0010U
#define RFE_COMMON_RAM_DIVI_BIAS_S                                            4U

// Field: [3:3] out
//
// Divider Output
#define RFE_COMMON_RAM_DIVI_OUT                                          0x0008U
#define RFE_COMMON_RAM_DIVI_OUT_M                                        0x0008U
#define RFE_COMMON_RAM_DIVI_OUT_S                                             3U
#define RFE_COMMON_RAM_DIVI_OUT_FE_S1G                                   0x0000U
#define RFE_COMMON_RAM_DIVI_OUT_FE_2G4                                   0x0008U

// Field: [2:0] ratio
//
// Divider Ratio
#define RFE_COMMON_RAM_DIVI_RATIO_W                                           3U
#define RFE_COMMON_RAM_DIVI_RATIO_M                                      0x0007U
#define RFE_COMMON_RAM_DIVI_RATIO_S                                           0U

//******************************************************************************
// Register: DIVF
//******************************************************************************
// Field: [15:15] pdet
//
// Peak Detector Mode
#define RFE_COMMON_RAM_DIVF_PDET                                         0x8000U
#define RFE_COMMON_RAM_DIVF_PDET_M                                       0x8000U
#define RFE_COMMON_RAM_DIVF_PDET_S                                           15U

// Field: [14:12] nmireftrim
//
// NMOS Bias Voltage Trim
#define RFE_COMMON_RAM_DIVF_NMIREFTRIM_W                                      3U
#define RFE_COMMON_RAM_DIVF_NMIREFTRIM_M                                 0x7000U
#define RFE_COMMON_RAM_DIVF_NMIREFTRIM_S                                     12U

// Field: [11:9] pmireftrim
//
// PMOS Bias Voltage Trim
#define RFE_COMMON_RAM_DIVF_PMIREFTRIM_W                                      3U
#define RFE_COMMON_RAM_DIVF_PMIREFTRIM_M                                 0x0E00U
#define RFE_COMMON_RAM_DIVF_PMIREFTRIM_S                                      9U

// Field: [8:8] txboost
//
// TX Buffer Boost
#define RFE_COMMON_RAM_DIVF_TXBOOST                                      0x0100U
#define RFE_COMMON_RAM_DIVF_TXBOOST_M                                    0x0100U
#define RFE_COMMON_RAM_DIVF_TXBOOST_S                                         8U
#define RFE_COMMON_RAM_DIVF_TXBOOST_DEFAULT                              0x0000U
#define RFE_COMMON_RAM_DIVF_TXBOOST_INCREASED                            0x0100U

// Field: [7:7] s1gfrc
//
// S1G Power Switch Force
#define RFE_COMMON_RAM_DIVF_S1GFRC                                       0x0080U
#define RFE_COMMON_RAM_DIVF_S1GFRC_M                                     0x0080U
#define RFE_COMMON_RAM_DIVF_S1GFRC_S                                          7U
#define RFE_COMMON_RAM_DIVF_S1GFRC_DIS                                   0x0000U
#define RFE_COMMON_RAM_DIVF_S1GFRC_EN                                    0x0080U

// Field: [6:5] bufgain
//
// Not connected or used in LRF
#define RFE_COMMON_RAM_DIVF_BUFGAIN_W                                         2U
#define RFE_COMMON_RAM_DIVF_BUFGAIN_M                                    0x0060U
#define RFE_COMMON_RAM_DIVF_BUFGAIN_S                                         5U

// Field: [4:4] bias
//
// Not connected or used in LRF
#define RFE_COMMON_RAM_DIVF_BIAS                                         0x0010U
#define RFE_COMMON_RAM_DIVF_BIAS_M                                       0x0010U
#define RFE_COMMON_RAM_DIVF_BIAS_S                                            4U

// Field: [3:3] out
//
// Divider Output
#define RFE_COMMON_RAM_DIVF_OUT                                          0x0008U
#define RFE_COMMON_RAM_DIVF_OUT_M                                        0x0008U
#define RFE_COMMON_RAM_DIVF_OUT_S                                             3U
#define RFE_COMMON_RAM_DIVF_OUT_FE_S1G                                   0x0000U
#define RFE_COMMON_RAM_DIVF_OUT_FE_2G4                                   0x0008U

// Field: [2:0] ratio
//
// Divider Ratio
#define RFE_COMMON_RAM_DIVF_RATIO_W                                           3U
#define RFE_COMMON_RAM_DIVF_RATIO_M                                      0x0007U
#define RFE_COMMON_RAM_DIVF_RATIO_S                                           0U

//******************************************************************************
// Register: DIVLDOI
//******************************************************************************
// Field: [15:15] itestctl
//
// ITEST Control
#define RFE_COMMON_RAM_DIVLDOI_ITESTCTL                                  0x8000U
#define RFE_COMMON_RAM_DIVLDOI_ITESTCTL_M                                0x8000U
#define RFE_COMMON_RAM_DIVLDOI_ITESTCTL_S                                    15U

// Field: [14:8] vouttrim
//
// VOUT Trim Code
#define RFE_COMMON_RAM_DIVLDOI_VOUTTRIM_W                                     7U
#define RFE_COMMON_RAM_DIVLDOI_VOUTTRIM_M                                0x7F00U
#define RFE_COMMON_RAM_DIVLDOI_VOUTTRIM_S                                     8U

// Field: [7:7] spare7
//
// ITEST Buffer Block Enable(Not Connected)
#define RFE_COMMON_RAM_DIVLDOI_SPARE7                                    0x0080U
#define RFE_COMMON_RAM_DIVLDOI_SPARE7_M                                  0x0080U
#define RFE_COMMON_RAM_DIVLDOI_SPARE7_S                                       7U

// Field: [6:4] tmux
//
// TMUX control bits
#define RFE_COMMON_RAM_DIVLDOI_TMUX_W                                         3U
#define RFE_COMMON_RAM_DIVLDOI_TMUX_M                                    0x0070U
#define RFE_COMMON_RAM_DIVLDOI_TMUX_S                                         4U
#define RFE_COMMON_RAM_DIVLDOI_TMUX_OFF                                  0x0000U
#define RFE_COMMON_RAM_DIVLDOI_TMUX_VDDR                                 0x0010U

// Field: [3:3] pdsel
//
// Resistor or diode stack PD
#define RFE_COMMON_RAM_DIVLDOI_PDSEL                                     0x0008U
#define RFE_COMMON_RAM_DIVLDOI_PDSEL_M                                   0x0008U
#define RFE_COMMON_RAM_DIVLDOI_PDSEL_S                                        3U
#define RFE_COMMON_RAM_DIVLDOI_PDSEL_R                                   0x0000U
#define RFE_COMMON_RAM_DIVLDOI_PDSEL_DIODE                               0x0008U

// Field: [2:2] mode
//
// High BW Operation
#define RFE_COMMON_RAM_DIVLDOI_MODE                                      0x0004U
#define RFE_COMMON_RAM_DIVLDOI_MODE_M                                    0x0004U
#define RFE_COMMON_RAM_DIVLDOI_MODE_S                                         2U
#define RFE_COMMON_RAM_DIVLDOI_MODE_NORM                                 0x0000U
#define RFE_COMMON_RAM_DIVLDOI_MODE_FAST                                 0x0004U

// Field: [1:1] bypass
//
// Regulator Bypass
#define RFE_COMMON_RAM_DIVLDOI_BYPASS                                    0x0002U
#define RFE_COMMON_RAM_DIVLDOI_BYPASS_M                                  0x0002U
#define RFE_COMMON_RAM_DIVLDOI_BYPASS_S                                       1U
#define RFE_COMMON_RAM_DIVLDOI_BYPASS_DIS                                0x0000U
#define RFE_COMMON_RAM_DIVLDOI_BYPASS_EN                                 0x0002U

// Field: [0:0] ctl
//
// Regulator Control
#define RFE_COMMON_RAM_DIVLDOI_CTL                                       0x0001U
#define RFE_COMMON_RAM_DIVLDOI_CTL_M                                     0x0001U
#define RFE_COMMON_RAM_DIVLDOI_CTL_S                                          0U
#define RFE_COMMON_RAM_DIVLDOI_CTL_DIS                                   0x0000U
#define RFE_COMMON_RAM_DIVLDOI_CTL_EN                                    0x0001U

//******************************************************************************
// Register: DIVLDOF
//******************************************************************************
// Field: [15:15] itestctl
//
// ITEST Control
#define RFE_COMMON_RAM_DIVLDOF_ITESTCTL                                  0x8000U
#define RFE_COMMON_RAM_DIVLDOF_ITESTCTL_M                                0x8000U
#define RFE_COMMON_RAM_DIVLDOF_ITESTCTL_S                                    15U

// Field: [14:8] vouttrim
//
// VOUT Trim Code
#define RFE_COMMON_RAM_DIVLDOF_VOUTTRIM_W                                     7U
#define RFE_COMMON_RAM_DIVLDOF_VOUTTRIM_M                                0x7F00U
#define RFE_COMMON_RAM_DIVLDOF_VOUTTRIM_S                                     8U

// Field: [7:7] spare7
//
// ITEST Buffer Block Enable(Not Connected)
#define RFE_COMMON_RAM_DIVLDOF_SPARE7                                    0x0080U
#define RFE_COMMON_RAM_DIVLDOF_SPARE7_M                                  0x0080U
#define RFE_COMMON_RAM_DIVLDOF_SPARE7_S                                       7U

// Field: [6:4] tmux
//
// TMUX control bits
#define RFE_COMMON_RAM_DIVLDOF_TMUX_W                                         3U
#define RFE_COMMON_RAM_DIVLDOF_TMUX_M                                    0x0070U
#define RFE_COMMON_RAM_DIVLDOF_TMUX_S                                         4U
#define RFE_COMMON_RAM_DIVLDOF_TMUX_OFF                                  0x0000U
#define RFE_COMMON_RAM_DIVLDOF_TMUX_VDDR                                 0x0010U

// Field: [3:3] pdsel
//
// Resistor or diode stack PD
#define RFE_COMMON_RAM_DIVLDOF_PDSEL                                     0x0008U
#define RFE_COMMON_RAM_DIVLDOF_PDSEL_M                                   0x0008U
#define RFE_COMMON_RAM_DIVLDOF_PDSEL_S                                        3U
#define RFE_COMMON_RAM_DIVLDOF_PDSEL_R                                   0x0000U
#define RFE_COMMON_RAM_DIVLDOF_PDSEL_DIODE                               0x0008U

// Field: [2:2] mode
//
// High BW Operation
#define RFE_COMMON_RAM_DIVLDOF_MODE                                      0x0004U
#define RFE_COMMON_RAM_DIVLDOF_MODE_M                                    0x0004U
#define RFE_COMMON_RAM_DIVLDOF_MODE_S                                         2U
#define RFE_COMMON_RAM_DIVLDOF_MODE_NORM                                 0x0000U
#define RFE_COMMON_RAM_DIVLDOF_MODE_FAST                                 0x0004U

// Field: [1:1] bypass
//
// Regulator Bypass
#define RFE_COMMON_RAM_DIVLDOF_BYPASS                                    0x0002U
#define RFE_COMMON_RAM_DIVLDOF_BYPASS_M                                  0x0002U
#define RFE_COMMON_RAM_DIVLDOF_BYPASS_S                                       1U
#define RFE_COMMON_RAM_DIVLDOF_BYPASS_DIS                                0x0000U
#define RFE_COMMON_RAM_DIVLDOF_BYPASS_EN                                 0x0002U

// Field: [0:0] ctl
//
// Regulator Control
#define RFE_COMMON_RAM_DIVLDOF_CTL                                       0x0001U
#define RFE_COMMON_RAM_DIVLDOF_CTL_M                                     0x0001U
#define RFE_COMMON_RAM_DIVLDOF_CTL_S                                          0U
#define RFE_COMMON_RAM_DIVLDOF_CTL_DIS                                   0x0000U
#define RFE_COMMON_RAM_DIVLDOF_CTL_EN                                    0x0001U

//******************************************************************************
// Register: DIVLDOIOFF
//******************************************************************************
// Field: [6:0] val
//
// Offset that RCL uses to adjust DIVLDOI.VOUTTRIM during startup.
#define RFE_COMMON_RAM_DIVLDOIOFF_VAL_W                                       7U
#define RFE_COMMON_RAM_DIVLDOIOFF_VAL_M                                  0x007FU
#define RFE_COMMON_RAM_DIVLDOIOFF_VAL_S                                       0U

//******************************************************************************
// Register: LDOSETTLE
//******************************************************************************
// Field: [9:0] val
//
// Value. Delay  = (VAL+1)/24 (us).
#define RFE_COMMON_RAM_LDOSETTLE_VAL_W                                       10U
#define RFE_COMMON_RAM_LDOSETTLE_VAL_M                                   0x03FFU
#define RFE_COMMON_RAM_LDOSETTLE_VAL_S                                        0U

//******************************************************************************
// Register: CHRGSETTLE
//******************************************************************************
// Field: [9:0] val
//
// Value. Delay  = (VAL+1)/24 (us). 
#define RFE_COMMON_RAM_CHRGSETTLE_VAL_W                                      10U
#define RFE_COMMON_RAM_CHRGSETTLE_VAL_M                                  0x03FFU
#define RFE_COMMON_RAM_CHRGSETTLE_VAL_S                                       0U

//******************************************************************************
// Register: DCOSETTLE
//******************************************************************************
// Field: [9:0] val
//
// Value. Delay = 1.5 (us) + (VAL+1)/24 (us). Minimum value shall be 5.
#define RFE_COMMON_RAM_DCOSETTLE_VAL_W                                       10U
#define RFE_COMMON_RAM_DCOSETTLE_VAL_M                                   0x03FFU
#define RFE_COMMON_RAM_DCOSETTLE_VAL_S                                        0U

//******************************************************************************
// Register: IFAMPRFLDOTX
//******************************************************************************
// Field: [15:9] trim
//
// Value to use in TX except low output power
#define RFE_COMMON_RAM_IFAMPRFLDOTX_TRIM_W                                    7U
#define RFE_COMMON_RAM_IFAMPRFLDOTX_TRIM_M                               0xFE00U
#define RFE_COMMON_RAM_IFAMPRFLDOTX_TRIM_S                                    9U

//******************************************************************************
// Register: IFAMPRFLDODEFAULT
//******************************************************************************
// Field: [15:9] trim
//
// Production trim value
#define RFE_COMMON_RAM_IFAMPRFLDODEFAULT_TRIM_W                               7U
#define RFE_COMMON_RAM_IFAMPRFLDODEFAULT_TRIM_M                          0xFE00U
#define RFE_COMMON_RAM_IFAMPRFLDODEFAULT_TRIM_S                               9U

//******************************************************************************
// Register: LFKIBL
//******************************************************************************
// Field: [12:12] hpm
//
// High Precision Mode
#define RFE_COMMON_RAM_LFKIBL_HPM                                        0x1000U
#define RFE_COMMON_RAM_LFKIBL_HPM_M                                      0x1000U
#define RFE_COMMON_RAM_LFKIBL_HPM_S                                          12U

// Field: [11:0] ki
//
// KI written by RFE
#define RFE_COMMON_RAM_LFKIBL_KI_W                                           12U
#define RFE_COMMON_RAM_LFKIBL_KI_M                                       0x0FFFU
#define RFE_COMMON_RAM_LFKIBL_KI_S                                            0U

//******************************************************************************
// Register: LFKPBL
//******************************************************************************
// Field: [14:0] kp
//
// KP written by RFE
#define RFE_COMMON_RAM_LFKPBL_KP_W                                           15U
#define RFE_COMMON_RAM_LFKPBL_KP_M                                       0x7FFFU
#define RFE_COMMON_RAM_LFKPBL_KP_S                                            0U

//******************************************************************************
// Register: PHYRSSIOFFSET
//******************************************************************************
// Field: [7:0] val
//
// Unsinged number, offset between magnitude samples and dBm
#define RFE_COMMON_RAM_PHYRSSIOFFSET_VAL_W                                    8U
#define RFE_COMMON_RAM_PHYRSSIOFFSET_VAL_M                               0x00FFU
#define RFE_COMMON_RAM_PHYRSSIOFFSET_VAL_S                                    0U

//******************************************************************************
// Register: SPARE0SHADOW
//******************************************************************************
// Field: [15:0] val
//
// 
#define RFE_COMMON_RAM_SPARE0SHADOW_VAL_W                                    16U
#define RFE_COMMON_RAM_SPARE0SHADOW_VAL_M                                0xFFFFU
#define RFE_COMMON_RAM_SPARE0SHADOW_VAL_S                                     0U

//******************************************************************************
// Register: SPARE1SHADOW
//******************************************************************************
// Field: [15:0] val
//
// 
#define RFE_COMMON_RAM_SPARE1SHADOW_VAL_W                                    16U
#define RFE_COMMON_RAM_SPARE1SHADOW_VAL_M                                0xFFFFU
#define RFE_COMMON_RAM_SPARE1SHADOW_VAL_S                                     0U

//******************************************************************************
// Register: AGCINFO
//******************************************************************************
// Field: [0:0] mode
//
// Information about AGC behavior of this PHY
#define RFE_COMMON_RAM_AGCINFO_MODE                                      0x0001U
#define RFE_COMMON_RAM_AGCINFO_MODE_M                                    0x0001U
#define RFE_COMMON_RAM_AGCINFO_MODE_S                                         0U
#define RFE_COMMON_RAM_AGCINFO_MODE_FAST                                 0x0000U
#define RFE_COMMON_RAM_AGCINFO_MODE_GEN                                  0x0001U

//******************************************************************************
// Register: GRANTPIN
//******************************************************************************
// Field: [3:0] cfg
//
// Tells which pin the grant signal is mapped to
#define RFE_COMMON_RAM_GRANTPIN_CFG_W                                         4U
#define RFE_COMMON_RAM_GRANTPIN_CFG_M                                    0x000FU
#define RFE_COMMON_RAM_GRANTPIN_CFG_S                                         0U
#define RFE_COMMON_RAM_GRANTPIN_CFG_GPI0                                 0x0000U
#define RFE_COMMON_RAM_GRANTPIN_CFG_GPI1                                 0x0001U
#define RFE_COMMON_RAM_GRANTPIN_CFG_GPI2                                 0x0002U
#define RFE_COMMON_RAM_GRANTPIN_CFG_GPI3                                 0x0003U
#define RFE_COMMON_RAM_GRANTPIN_CFG_GPI4                                 0x0004U
#define RFE_COMMON_RAM_GRANTPIN_CFG_GPI5                                 0x0005U
#define RFE_COMMON_RAM_GRANTPIN_CFG_GPI6                                 0x0006U
#define RFE_COMMON_RAM_GRANTPIN_CFG_GPI7                                 0x0007U
#define RFE_COMMON_RAM_GRANTPIN_CFG_DIS                                  0x000FU

//******************************************************************************
// Register: PATRIM01
//******************************************************************************
// Field: [12:8] val1
//
// PA Trim for Mode 1 
#define RFE_COMMON_RAM_PATRIM01_VAL1_W                                        5U
#define RFE_COMMON_RAM_PATRIM01_VAL1_M                                   0x1F00U
#define RFE_COMMON_RAM_PATRIM01_VAL1_S                                        8U

// Field: [4:0] val0
//
// PA Trim for Mode 0 
#define RFE_COMMON_RAM_PATRIM01_VAL0_W                                        5U
#define RFE_COMMON_RAM_PATRIM01_VAL0_M                                   0x001FU
#define RFE_COMMON_RAM_PATRIM01_VAL0_S                                        0U

//******************************************************************************
// Register: PATRIM23
//******************************************************************************
// Field: [12:8] val3
//
// PA Trim for Mode 3 
#define RFE_COMMON_RAM_PATRIM23_VAL3_W                                        5U
#define RFE_COMMON_RAM_PATRIM23_VAL3_M                                   0x1F00U
#define RFE_COMMON_RAM_PATRIM23_VAL3_S                                        8U

// Field: [4:0] val2
//
// PA Trim for Mode 2 
#define RFE_COMMON_RAM_PATRIM23_VAL2_W                                        5U
#define RFE_COMMON_RAM_PATRIM23_VAL2_M                                   0x001FU
#define RFE_COMMON_RAM_PATRIM23_VAL2_S                                        0U

//******************************************************************************
// Register: IKT
//******************************************************************************
// Field: [15:0] val
//
// Value
#define RFE_COMMON_RAM_IKT_VAL_W                                             16U
#define RFE_COMMON_RAM_IKT_VAL_M                                         0xFFFFU
#define RFE_COMMON_RAM_IKT_VAL_S                                              0U


#endif // __RFE_COMMON_RAM_REGS_H
