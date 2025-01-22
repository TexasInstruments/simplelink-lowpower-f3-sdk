/******************************************************************************
*  Filename:       hw_ckmd_h
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

#ifndef __HW_CKMD_H__
#define __HW_CKMD_H__

//*****************************************************************************
//
// This section defines the register offsets of
// CKMD component
//
//*****************************************************************************
// IP Description
#define CKMD_O_DESC                                                 0x00000000U

// Interrupt mask.
#define CKMD_O_IMASK                                                0x00000044U

// Raw interrupt flag register
#define CKMD_O_RIS                                                  0x00000048U

// Masked interrupt flag register
#define CKMD_O_MIS                                                  0x0000004CU

// Interrupt flag set register
#define CKMD_O_ISET                                                 0x00000050U

// Interrupt flag clear register
#define CKMD_O_ICLR                                                 0x00000054U

// Interrupt mask set register
#define CKMD_O_IMSET                                                0x00000058U

// Interrupt mask clear register.
#define CKMD_O_IMCLR                                                0x0000005CU

// Internal. Only to be used through TI provided API.
#define CKMD_O_HFOSCCTL                                             0x00000080U

// High frequency crystal control
#define CKMD_O_HFXTCTL                                              0x00000084U

// Low frequency oscillator control
#define CKMD_O_LFOSCCTL                                             0x0000008CU

// Low frequency crystal control
#define CKMD_O_LFXTCTL                                              0x00000090U

// Low frequency clock qualification control
#define CKMD_O_LFQUALCTL                                            0x00000094U

// Low frequency time increment control
#define CKMD_O_LFINCCTL                                             0x00000098U

// Low frequency time increment override control
#define CKMD_O_LFINCOVR                                             0x0000009CU

// Internal. Only to be used through TI provided API.
#define CKMD_O_AMPADCCTL                                            0x000000A0U

// High frequency tracking loop control
#define CKMD_O_HFTRACKCTL                                           0x000000A4U

// Internal. Only to be used through TI provided API.
#define CKMD_O_LDOCTL                                               0x000000A8U

// Nanoamp-bias control
#define CKMD_O_NABIASCTL                                            0x000000ACU

// Low-frequency clock-monitor control
#define CKMD_O_LFMONCTL                                             0x000000B0U

// Low frequency time increment control-2
#define CKMD_O_LFINCCTL2                                            0x000000B4U

// Low frequency clock selection
#define CKMD_O_LFCLKSEL                                             0x000000C0U

// Internal. Only to be used through TI provided API.
#define CKMD_O_TDCCLKSEL                                            0x000000C4U

// ADC clock selection
#define CKMD_O_ADCCLKSEL                                            0x000000C8U

// Low-frequency clock status
#define CKMD_O_LFCLKSTAT                                            0x000000E0U

// HFXT status information
#define CKMD_O_HFXTSTAT                                             0x000000E4U

// Internal. Only to be used through TI provided API.
#define CKMD_O_AMPADCSTAT                                           0x000000E8U

// HF tracking loop status information
#define CKMD_O_TRACKSTAT                                            0x000000ECU

// HFXT Amplitude Compensation Status
#define CKMD_O_AMPSTAT                                              0x000000F0U

// Low-frequency clock status-2
#define CKMD_O_LFCLKSTAT2                                           0x000000F4U

// Internal. Only to be used through TI provided API.
#define CKMD_O_ATBCTL0                                              0x00000100U

// Internal. Only to be used through TI provided API.
#define CKMD_O_ATBCTL1                                              0x00000104U

// Digital test bus mux control
#define CKMD_O_DTBCTL                                               0x00000108U

// Digital test bus mux control
#define CKMD_O_DTBCTL2                                              0x0000010CU

// Internal. Only to be used through TI provided API.
#define CKMD_O_TRIM0                                                0x00000110U

// Internal. Only to be used through TI provided API.
#define CKMD_O_TRIM1                                                0x00000114U

// Initial values for HFXT ramping
#define CKMD_O_HFXTINIT                                             0x00000118U

// Target values for HFXT ramping
#define CKMD_O_HFXTTARG                                             0x0000011CU

// Alternative target values for HFXT configuration
#define CKMD_O_HFXTDYN                                              0x00000120U

// Amplitude Compensation Configuration 0
#define CKMD_O_AMPCFG0                                              0x00000124U

// Amplitude Compensation Configuration 1
#define CKMD_O_AMPCFG1                                              0x00000128U

// Configuration Register for the Tracking Loop
#define CKMD_O_LOOPCFG                                              0x0000012CU

// Configuration Register for underclocking HFOSC
#define CKMD_O_LOOPCFG1                                             0x00000130U

// Audio frequency oscillator control
#define CKMD_O_AFOSCCTL                                             0x00000140U

// Audio frequency tracking loop control
#define CKMD_O_AFTRACKCTL                                           0x00000144U

// Internal. Only to be used through TI provided API.
#define CKMD_O_BANDGAPCTL                                           0x00000148U

// Audio clock selection
#define CKMD_O_AFCLKSEL                                             0x00000150U

// CAN clock selection
#define CKMD_O_CANCLKSEL                                            0x00000154U

// AF tracking loop status information
#define CKMD_O_TRACKSTATAF                                          0x00000160U

// AF tracking loop status information
#define CKMD_O_TRACKSTATAF1                                         0x00000164U

// AF tracking loop status information
#define CKMD_O_TRACKSTATAF2                                         0x00000168U

// Configuration Register for the Audio frequency Tracking Loop
#define CKMD_O_LOOPCFGAF                                            0x00000170U

// Control
#define CKMD_O_CTL                                                  0x00000200U

// Status
#define CKMD_O_STAT                                                 0x00000204U

// Result
#define CKMD_O_RESULT                                               0x00000208U

// Saturation Configuration
#define CKMD_O_SATCFG                                               0x0000020CU

// Trigger Source
#define CKMD_O_TRIGSRC                                              0x00000210U

// Trigger Counter
#define CKMD_O_TRIGCNT                                              0x00000214U

// Trigger Counter Load
#define CKMD_O_TRIGCNTLOAD                                          0x00000218U

// Trigger Counter Configuration
#define CKMD_O_TRIGCNTCFG                                           0x0000021CU

// Prescaler Control
#define CKMD_O_PRECTL                                               0x00000220U

// Prescaler Counter
#define CKMD_O_PRECNTR                                              0x00000224U

// WDT counter value register
#define CKMD_O_CNT                                                  0x00000300U

// WDT test mode register
#define CKMD_O_TEST                                                 0x00000304U

// WDT lock register
#define CKMD_O_LOCK                                                 0x00000308U

//*****************************************************************************
//
// Register: CKMD_O_DESC
//
//*****************************************************************************
// Field: [31:16] MODID
//
// Module identifier
#define CKMD_DESC_MODID_W                                                   16U
#define CKMD_DESC_MODID_M                                           0xFFFF0000U
#define CKMD_DESC_MODID_S                                                   16U

// Field: [15:12] STDIPOFF
//
// Standard IP MMR block offset
#define CKMD_DESC_STDIPOFF_W                                                 4U
#define CKMD_DESC_STDIPOFF_M                                        0x0000F000U
#define CKMD_DESC_STDIPOFF_S                                                12U

// Field:   [7:4] MAJREV
//
// Major revision
#define CKMD_DESC_MAJREV_W                                                   4U
#define CKMD_DESC_MAJREV_M                                          0x000000F0U
#define CKMD_DESC_MAJREV_S                                                   4U

// Field:   [3:0] MINREV
//
// Minor revision
#define CKMD_DESC_MINREV_W                                                   4U
#define CKMD_DESC_MINREV_M                                          0x0000000FU
#define CKMD_DESC_MINREV_S                                                   0U

//*****************************************************************************
//
// Register: CKMD_O_IMASK
//
//*****************************************************************************
// Field:    [23] HFOSCSETTLED
//
// Indicates that HFOSC has settled, based on LOOPCFG.SETTLED_TARGET and
// LOOPCFG1.SETTLEIRQ
#define CKMD_IMASK_HFOSCSETTLED                                     0x00800000U
#define CKMD_IMASK_HFOSCSETTLED_M                                   0x00800000U
#define CKMD_IMASK_HFOSCSETTLED_S                                           23U

// Field:    [22] LFGEARRSTRTLIM
//
// Indicates that the LFINC filter gearing mechanism has restarted more than
// LFINCCTL2.GEARRSTRTLIM
#define CKMD_IMASK_LFGEARRSTRTLIM                                   0x00400000U
#define CKMD_IMASK_LFGEARRSTRTLIM_M                                 0x00400000U
#define CKMD_IMASK_LFGEARRSTRTLIM_S                                         22U

// Field:    [20] SYSUNDERCLOCKED
//
// Indicates system frequency has been lowered. It will be set only if
// HFTRACKCTL.UNDERCLK is appropriately configured and HFOSC tracking loop is
// not running.
#define CKMD_IMASK_SYSUNDERCLOCKED                                  0x00100000U
#define CKMD_IMASK_SYSUNDERCLOCKED_M                                0x00100000U
#define CKMD_IMASK_SYSUNDERCLOCKED_S                                        20U

// Field:    [19] AFOSCGOOD
//
// AFOSC good indication.
#define CKMD_IMASK_AFOSCGOOD                                        0x00080000U
#define CKMD_IMASK_AFOSCGOOD_M                                      0x00080000U
#define CKMD_IMASK_AFOSCGOOD_S                                              19U

// Field:    [18] TRACKREFAFOOR
//
// Out-of-range indication from the AFOSC tracking loop.
//
// Indicates that the reference clock frequency of AFOSC tracking loop is
// out-of-range.
#define CKMD_IMASK_TRACKREFAFOOR                                    0x00040000U
#define CKMD_IMASK_TRACKREFAFOOR_M                                  0x00040000U
#define CKMD_IMASK_TRACKREFAFOOR_S                                          18U

// Field:    [17] LFTICK
//
// 32kHz TICK to RTC and WDT.
//
// Either derived from selected LFCLK or generated from CLKULL in absence of
// LFCLK.
#define CKMD_IMASK_LFTICK                                           0x00020000U
#define CKMD_IMASK_LFTICK_M                                         0x00020000U
#define CKMD_IMASK_LFTICK_S                                                 17U

// Field:    [16] LFGEARRSTRT
//
// LFINC filter gearing restart.
//
// Indicates that the LFINC filter restarted gearing. Subsequent LFINC
// estimates may have higher variation.
#define CKMD_IMASK_LFGEARRSTRT                                      0x00010000U
#define CKMD_IMASK_LFGEARRSTRT_M                                    0x00010000U
#define CKMD_IMASK_LFGEARRSTRT_S                                            16U

// Field:    [15] AMPSETTLED
//
// HFXT Amplitude compensation - settled
//
// Indicates that the amplitude compensation FSM has reached the SETTLED or
// TCXOMODE state,
// and the controls configured in HFXTTARG or HFXTDYN are reached.
#define CKMD_IMASK_AMPSETTLED                                       0x00008000U
#define CKMD_IMASK_AMPSETTLED_M                                     0x00008000U
#define CKMD_IMASK_AMPSETTLED_S                                             15U

// Field:    [14] AMPCTRLATTARG
//
// HFXT Amplitude compensation - controls at target
//
// Indicates that the control values configured in HFXTTARG or HFXTDYN are
// reached.
// Applies to Q1CAP, Q2CAP and IREF.
#define CKMD_IMASK_AMPCTRLATTARG                                    0x00004000U
#define CKMD_IMASK_AMPCTRLATTARG_M                                  0x00004000U
#define CKMD_IMASK_AMPCTRLATTARG_S                                          14U

// Field:    [13] PRELFEDGE
//
// Pre-LF clock edge detect.
//
// Indicates that a positive edge occured on the selected pre-LF clock
// CLKSEL.PRELFCLK'
// Can be used by software to confirm that a LF clock source is running and
// within the expected frequency,
// before selecting it as the main LF clock source.
#define CKMD_IMASK_PRELFEDGE                                        0x00002000U
#define CKMD_IMASK_PRELFEDGE_M                                      0x00002000U
#define CKMD_IMASK_PRELFEDGE_S                                              13U

// Field:    [12] LFCLKLOSS
//
// LF clock is lost.
//
// Indicates that no LF clock edge occured for ~49us (~1.6 times nominal
// period).
// The system will automatically fall-back to generating LFTICK based on
// CLKULL,
// to avoid timing corruption.
// Note that this signal is NOT related to the analog LF clock-loss detector
// which can reset the device during STANDBY.
#define CKMD_IMASK_LFCLKLOSS                                        0x00001000U
#define CKMD_IMASK_LFCLKLOSS_M                                      0x00001000U
#define CKMD_IMASK_LFCLKLOSS_S                                              12U

// Field:    [11] LFCLKOOR
//
// LF clock period out-of-range.
//
// Indicates that a LF clock period was measured to be out-of-range,
// according to LFQUALCTL.MAXERR.
#define CKMD_IMASK_LFCLKOOR                                         0x00000800U
#define CKMD_IMASK_LFCLKOOR_M                                       0x00000800U
#define CKMD_IMASK_LFCLKOOR_S                                               11U

// Field:    [10] LFCLKGOOD
//
// LF clock good.
//
// Indicates that the LF clock is good, according to the configuration in
// LFQUALCTL.
#define CKMD_IMASK_LFCLKGOOD                                        0x00000400U
#define CKMD_IMASK_LFCLKGOOD_M                                      0x00000400U
#define CKMD_IMASK_LFCLKGOOD_S                                              10U

// Field:     [9] LFINCUPD
//
// LFINC updated.
//
// Indicates that a new LFINC measurement value is available in
// LFCLKSTAT.LFINC.
#define CKMD_IMASK_LFINCUPD                                         0x00000200U
#define CKMD_IMASK_LFINCUPD_M                                       0x00000200U
#define CKMD_IMASK_LFINCUPD_S                                                9U

// Field:     [8] TDCDONE
//
// TDC done event.
//
// Indicates that the TDC measurement is done.
#define CKMD_IMASK_TDCDONE                                          0x00000100U
#define CKMD_IMASK_TDCDONE_M                                        0x00000100U
#define CKMD_IMASK_TDCDONE_S                                                 8U

// Field:     [7] ADCPEAKUPD
//
// HFXT-ADC PEAK measurement update event.
//
// Indicates that the HFXT-ADC PEAK measurement is done.
#define CKMD_IMASK_ADCPEAKUPD                                       0x00000080U
#define CKMD_IMASK_ADCPEAKUPD_M                                     0x00000080U
#define CKMD_IMASK_ADCPEAKUPD_S                                              7U

// Field:     [6] ADCBIASUPD
//
// HFXT-ADC BIAS measurement update event.
//
// Indicates that the HFXT-ADC BIAS measurement is done.
#define CKMD_IMASK_ADCBIASUPD                                       0x00000040U
#define CKMD_IMASK_ADCBIASUPD_M                                     0x00000040U
#define CKMD_IMASK_ADCBIASUPD_S                                              6U

// Field:     [5] ADCCOMPUPD
//
// HFXT-ADC comparison update event.
//
// Indicates that the HFXT-ADC comparison is done.
#define CKMD_IMASK_ADCCOMPUPD                                       0x00000020U
#define CKMD_IMASK_ADCCOMPUPD_M                                     0x00000020U
#define CKMD_IMASK_ADCCOMPUPD_S                                              5U

// Field:     [4] TRACKREFOOR
//
// Out-of-range indication from the tracking loop.
//
// Indicates that the selected reference clock frequency of the tracking loop
// is out-of-range.
#define CKMD_IMASK_TRACKREFOOR                                      0x00000010U
#define CKMD_IMASK_TRACKREFOOR_M                                    0x00000010U
#define CKMD_IMASK_TRACKREFOOR_S                                             4U

// Field:     [3] TRACKREFLOSS
//
// Clock loss indication from the tracking loop.
//
// Indicates that the selected reference clock of the tracking loop is lost.
#define CKMD_IMASK_TRACKREFLOSS                                     0x00000008U
#define CKMD_IMASK_TRACKREFLOSS_M                                   0x00000008U
#define CKMD_IMASK_TRACKREFLOSS_S                                            3U

// Field:     [2] HFXTAMPGOOD
//
// HFXT amplitude good indication.
#define CKMD_IMASK_HFXTAMPGOOD                                      0x00000004U
#define CKMD_IMASK_HFXTAMPGOOD_M                                    0x00000004U
#define CKMD_IMASK_HFXTAMPGOOD_S                                             2U

// Field:     [1] HFXTFAULT
//
// HFXT fault indication.
//
// Indicates that HFXT did not start correctly, or its frequency is too low.
// HFXT will not recover from this fault and has to be restarted.
// This is only a one-time check at HFXT startup.
#define CKMD_IMASK_HFXTFAULT                                        0x00000002U
#define CKMD_IMASK_HFXTFAULT_M                                      0x00000002U
#define CKMD_IMASK_HFXTFAULT_S                                               1U

// Field:     [0] HFXTGOOD
//
// HFXT good indication.
//
// Indicates that HFXT started correctly. The frequency is not necessarily good
// enough for radio operation.
// This is only a one-time check at HFXT startup.
#define CKMD_IMASK_HFXTGOOD                                         0x00000001U
#define CKMD_IMASK_HFXTGOOD_M                                       0x00000001U
#define CKMD_IMASK_HFXTGOOD_S                                                0U

//*****************************************************************************
//
// Register: CKMD_O_RIS
//
//*****************************************************************************
// Field:    [23] HFOSCSETTLED
//
// Indicates that HFOSC has settled, based on LOOPCFG.SETTLED_TARGET and
// LOOPCFG1.SETTLEIRQ
#define CKMD_RIS_HFOSCSETTLED                                       0x00800000U
#define CKMD_RIS_HFOSCSETTLED_M                                     0x00800000U
#define CKMD_RIS_HFOSCSETTLED_S                                             23U

// Field:    [22] LFGEARRSTRTLIM
//
// Indicates that the LFINC filter gearing mechanism has restarted more than
// LFINCCTL2.GEARRSTRTLIM
#define CKMD_RIS_LFGEARRSTRTLIM                                     0x00400000U
#define CKMD_RIS_LFGEARRSTRTLIM_M                                   0x00400000U
#define CKMD_RIS_LFGEARRSTRTLIM_S                                           22U

// Field:    [20] SYSUNDERCLOCKED
//
// Indicates system frequency has been lowered. It will be set only if
// HFTRACKCTL.UNDERCLK is appropriately configured and HFOSC tracking loop is
// not running.
#define CKMD_RIS_SYSUNDERCLOCKED                                    0x00100000U
#define CKMD_RIS_SYSUNDERCLOCKED_M                                  0x00100000U
#define CKMD_RIS_SYSUNDERCLOCKED_S                                          20U

// Field:    [19] AFOSCGOOD
//
// AFOSC good indication.
#define CKMD_RIS_AFOSCGOOD                                          0x00080000U
#define CKMD_RIS_AFOSCGOOD_M                                        0x00080000U
#define CKMD_RIS_AFOSCGOOD_S                                                19U

// Field:    [18] TRACKREFAFOOR
//
// Out-of-range indication from the AFOSC tracking loop.
//
// Indicates that the reference clock frequency of AFOSC tracking loop is
// out-of-range.
#define CKMD_RIS_TRACKREFAFOOR                                      0x00040000U
#define CKMD_RIS_TRACKREFAFOOR_M                                    0x00040000U
#define CKMD_RIS_TRACKREFAFOOR_S                                            18U

// Field:    [17] LFTICK
//
// 32kHz TICK to RTC and WDT.
//
// Either derived from selected LFCLK or generated from CLKULL in absence of
// LFCLK.
#define CKMD_RIS_LFTICK                                             0x00020000U
#define CKMD_RIS_LFTICK_M                                           0x00020000U
#define CKMD_RIS_LFTICK_S                                                   17U

// Field:    [16] LFGEARRSTRT
//
// LFINC filter gearing restart.
//
// Indicates that the LFINC filter restarted gearing. Subsequent LFINC
// estimates may have higher variation.
#define CKMD_RIS_LFGEARRSTRT                                        0x00010000U
#define CKMD_RIS_LFGEARRSTRT_M                                      0x00010000U
#define CKMD_RIS_LFGEARRSTRT_S                                              16U

// Field:    [15] AMPSETTLED
//
// HFXT Amplitude compensation - settled
//
// Indicates that the amplitude compensation FSM has reached the SETTLED or
// TCXOMODE state,
// and the controls configured in HFXTTARG or HFXTDYN are reached.
#define CKMD_RIS_AMPSETTLED                                         0x00008000U
#define CKMD_RIS_AMPSETTLED_M                                       0x00008000U
#define CKMD_RIS_AMPSETTLED_S                                               15U

// Field:    [14] AMPCTRLATTARG
//
// HFXT Amplitude compensation - controls at target
//
// Indicates that the control values configured in HFXTTARG or HFXTDYN are
// reached.
// Applies to Q1CAP, Q2CAP and IREF.
#define CKMD_RIS_AMPCTRLATTARG                                      0x00004000U
#define CKMD_RIS_AMPCTRLATTARG_M                                    0x00004000U
#define CKMD_RIS_AMPCTRLATTARG_S                                            14U

// Field:    [13] PRELFEDGE
//
// Pre-LF clock edge detect.
//
// Indicates that a positive edge occured on the selected pre-LF clock
// CLKSEL.PRELFCLK'
// Can be used by software to confirm that a LF clock source is running and
// within the expected frequency,
// before selecting it as the main LF clock source.
#define CKMD_RIS_PRELFEDGE                                          0x00002000U
#define CKMD_RIS_PRELFEDGE_M                                        0x00002000U
#define CKMD_RIS_PRELFEDGE_S                                                13U

// Field:    [12] LFCLKLOSS
//
// LF clock is lost.
//
// Indicates that no LF clock edge occured for ~49us (~1.6 times nominal
// period).
// The system will automatically fall-back to generating LFTICK based on
// CLKULL,
// to avoid timing corruption.
// Note that this signal is NOT related to the analog LF clock-loss detector
// which can reset the device during STANDBY.
#define CKMD_RIS_LFCLKLOSS                                          0x00001000U
#define CKMD_RIS_LFCLKLOSS_M                                        0x00001000U
#define CKMD_RIS_LFCLKLOSS_S                                                12U

// Field:    [11] LFCLKOOR
//
// LF clock period out-of-range.
//
// Indicates that a LF clock period was measured to be out-of-range,
// according to LFQUALCTL.MAXERR.
#define CKMD_RIS_LFCLKOOR                                           0x00000800U
#define CKMD_RIS_LFCLKOOR_M                                         0x00000800U
#define CKMD_RIS_LFCLKOOR_S                                                 11U

// Field:    [10] LFCLKGOOD
//
// LF clock good.
//
// Indicates that the LF clock is good, according to the configuration in
// LFQUALCTL.
#define CKMD_RIS_LFCLKGOOD                                          0x00000400U
#define CKMD_RIS_LFCLKGOOD_M                                        0x00000400U
#define CKMD_RIS_LFCLKGOOD_S                                                10U

// Field:     [9] LFINCUPD
//
// LFINC updated.
//
// Indicates that a new LFINC measurement value is available in
// LFCLKSTAT.LFINC.
#define CKMD_RIS_LFINCUPD                                           0x00000200U
#define CKMD_RIS_LFINCUPD_M                                         0x00000200U
#define CKMD_RIS_LFINCUPD_S                                                  9U

// Field:     [8] TDCDONE
//
// TDC done event.
//
// Indicates that the TDC measurement is done.
#define CKMD_RIS_TDCDONE                                            0x00000100U
#define CKMD_RIS_TDCDONE_M                                          0x00000100U
#define CKMD_RIS_TDCDONE_S                                                   8U

// Field:     [7] ADCPEAKUPD
//
// HFXT-ADC PEAK measurement update event.
//
// Indicates that the HFXT-ADC PEAK measurement is done.
#define CKMD_RIS_ADCPEAKUPD                                         0x00000080U
#define CKMD_RIS_ADCPEAKUPD_M                                       0x00000080U
#define CKMD_RIS_ADCPEAKUPD_S                                                7U

// Field:     [6] ADCBIASUPD
//
// HFXT-ADC BIAS measurement update event.
//
// Indicates that the HFXT-ADC BIAS measurement is done.
#define CKMD_RIS_ADCBIASUPD                                         0x00000040U
#define CKMD_RIS_ADCBIASUPD_M                                       0x00000040U
#define CKMD_RIS_ADCBIASUPD_S                                                6U

// Field:     [5] ADCCOMPUPD
//
// HFXT-ADC comparison update event.
//
// Indicates that the HFXT-ADC comparison is done.
#define CKMD_RIS_ADCCOMPUPD                                         0x00000020U
#define CKMD_RIS_ADCCOMPUPD_M                                       0x00000020U
#define CKMD_RIS_ADCCOMPUPD_S                                                5U

// Field:     [4] TRACKREFOOR
//
// Out-of-range indication from the tracking loop.
//
// Indicates that the selected reference clock frequency of the tracking loop
// is out-of-range.
#define CKMD_RIS_TRACKREFOOR                                        0x00000010U
#define CKMD_RIS_TRACKREFOOR_M                                      0x00000010U
#define CKMD_RIS_TRACKREFOOR_S                                               4U

// Field:     [3] TRACKREFLOSS
//
// Clock loss indication from the tracking loop.
//
// Indicates that the selected reference clock of the tracking loop is lost.
#define CKMD_RIS_TRACKREFLOSS                                       0x00000008U
#define CKMD_RIS_TRACKREFLOSS_M                                     0x00000008U
#define CKMD_RIS_TRACKREFLOSS_S                                              3U

// Field:     [2] HFXTAMPGOOD
//
// HFXT amplitude good indication.
#define CKMD_RIS_HFXTAMPGOOD                                        0x00000004U
#define CKMD_RIS_HFXTAMPGOOD_M                                      0x00000004U
#define CKMD_RIS_HFXTAMPGOOD_S                                               2U

// Field:     [1] HFXTFAULT
//
// HFXT fault indication.
//
// Indicates that HFXT did not start correctly, or its frequency is too low.
// HFXT will not recover from this fault and has to be restarted.
// This is only a one-time check at HFXT startup.
#define CKMD_RIS_HFXTFAULT                                          0x00000002U
#define CKMD_RIS_HFXTFAULT_M                                        0x00000002U
#define CKMD_RIS_HFXTFAULT_S                                                 1U

// Field:     [0] HFXTGOOD
//
// HFXT good indication.
//
// Indicates that HFXT started correctly. The frequency is not necessarily good
// enough for radio operation.
// This is only a one-time check at HFXT startup.
#define CKMD_RIS_HFXTGOOD                                           0x00000001U
#define CKMD_RIS_HFXTGOOD_M                                         0x00000001U
#define CKMD_RIS_HFXTGOOD_S                                                  0U

//*****************************************************************************
//
// Register: CKMD_O_MIS
//
//*****************************************************************************
// Field:    [23] HFOSCSETTLED
//
// Indicates that HFOSC has settled, based on LOOPCFG.SETTLED_TARGET and
// LOOPCFG1.SETTLEIRQ
#define CKMD_MIS_HFOSCSETTLED                                       0x00800000U
#define CKMD_MIS_HFOSCSETTLED_M                                     0x00800000U
#define CKMD_MIS_HFOSCSETTLED_S                                             23U

// Field:    [22] LFGEARRSTRTLIM
//
// Indicates that the LFINC filter gearing mechanism has restarted more than
// LFINCCTL2.GEARRSTRTLIM
#define CKMD_MIS_LFGEARRSTRTLIM                                     0x00400000U
#define CKMD_MIS_LFGEARRSTRTLIM_M                                   0x00400000U
#define CKMD_MIS_LFGEARRSTRTLIM_S                                           22U

// Field:    [20] SYSUNDERCLOCKED
//
// Indicates system frequency has been lowered. It will be set only if
// HFTRACKCTL.UNDERCLK is appropriately configured and HFOSC tracking loop is
// not running.
#define CKMD_MIS_SYSUNDERCLOCKED                                    0x00100000U
#define CKMD_MIS_SYSUNDERCLOCKED_M                                  0x00100000U
#define CKMD_MIS_SYSUNDERCLOCKED_S                                          20U

// Field:    [19] AFOSCGOOD
//
// AFOSC good indication.
#define CKMD_MIS_AFOSCGOOD                                          0x00080000U
#define CKMD_MIS_AFOSCGOOD_M                                        0x00080000U
#define CKMD_MIS_AFOSCGOOD_S                                                19U

// Field:    [18] TRACKREFAFOOR
//
// Out-of-range indication from the AFOSC tracking loop.
//
// Indicates that the reference clock frequency of AFOSC tracking loop is
// out-of-range.
#define CKMD_MIS_TRACKREFAFOOR                                      0x00040000U
#define CKMD_MIS_TRACKREFAFOOR_M                                    0x00040000U
#define CKMD_MIS_TRACKREFAFOOR_S                                            18U

// Field:    [17] LFTICK
//
// 32kHz TICK to RTC and WDT.
//
// Either derived from selected LFCLK or generated from CLKULL in absence of
// LFCLK.
#define CKMD_MIS_LFTICK                                             0x00020000U
#define CKMD_MIS_LFTICK_M                                           0x00020000U
#define CKMD_MIS_LFTICK_S                                                   17U

// Field:    [16] LFGEARRSTRT
//
// LFINC filter gearing restart.
//
// Indicates that the LFINC filter restarted gearing. Subsequent LFINC
// estimates may have higher variation.
#define CKMD_MIS_LFGEARRSTRT                                        0x00010000U
#define CKMD_MIS_LFGEARRSTRT_M                                      0x00010000U
#define CKMD_MIS_LFGEARRSTRT_S                                              16U

// Field:    [15] AMPSETTLED
//
// HFXT Amplitude compensation - settled
//
// Indicates that the amplitude compensation FSM has reached the SETTLED or
// TCXOMODE state,
// and the controls configured in HFXTTARG or HFXTDYN are reached.
#define CKMD_MIS_AMPSETTLED                                         0x00008000U
#define CKMD_MIS_AMPSETTLED_M                                       0x00008000U
#define CKMD_MIS_AMPSETTLED_S                                               15U

// Field:    [14] AMPCTRLATTARG
//
// HFXT Amplitude compensation - controls at target
//
// Indicates that the control values configured in HFXTTARG or HFXTDYN are
// reached.
// Applies to Q1CAP, Q2CAP and IREF.
#define CKMD_MIS_AMPCTRLATTARG                                      0x00004000U
#define CKMD_MIS_AMPCTRLATTARG_M                                    0x00004000U
#define CKMD_MIS_AMPCTRLATTARG_S                                            14U

// Field:    [13] PRELFEDGE
//
// Pre-LF clock edge detect.
//
// Indicates that a positive edge occured on the selected pre-LF clock
// CLKSEL.PRELFCLK'
// Can be used by software to confirm that a LF clock source is running and
// within the expected frequency,
// before selecting it as the main LF clock source.
#define CKMD_MIS_PRELFEDGE                                          0x00002000U
#define CKMD_MIS_PRELFEDGE_M                                        0x00002000U
#define CKMD_MIS_PRELFEDGE_S                                                13U

// Field:    [12] LFCLKLOSS
//
// LF clock is lost.
//
// Indicates that no LF clock edge occured for ~49us (~1.6 times nominal
// period).
// The system will automatically fall-back to generating LFTICK based on
// CLKULL,
// to avoid timing corruption.
// Note that this signal is NOT related to the analog LF clock-loss detector
// which can reset the device during STANDBY.
#define CKMD_MIS_LFCLKLOSS                                          0x00001000U
#define CKMD_MIS_LFCLKLOSS_M                                        0x00001000U
#define CKMD_MIS_LFCLKLOSS_S                                                12U

// Field:    [11] LFCLKOOR
//
// LF clock period out-of-range.
//
// Indicates that a LF clock period was measured to be out-of-range,
// according to LFQUALCTL.MAXERR.
#define CKMD_MIS_LFCLKOOR                                           0x00000800U
#define CKMD_MIS_LFCLKOOR_M                                         0x00000800U
#define CKMD_MIS_LFCLKOOR_S                                                 11U

// Field:    [10] LFCLKGOOD
//
// LF clock good.
//
// Indicates that the LF clock is good, according to the configuration in
// LFQUALCTL.
#define CKMD_MIS_LFCLKGOOD                                          0x00000400U
#define CKMD_MIS_LFCLKGOOD_M                                        0x00000400U
#define CKMD_MIS_LFCLKGOOD_S                                                10U

// Field:     [9] LFINCUPD
//
// LFINC updated.
//
// Indicates that a new LFINC measurement value is available in
// LFCLKSTAT.LFINC.
#define CKMD_MIS_LFINCUPD                                           0x00000200U
#define CKMD_MIS_LFINCUPD_M                                         0x00000200U
#define CKMD_MIS_LFINCUPD_S                                                  9U

// Field:     [8] TDCDONE
//
// TDC done event.
//
// Indicates that the TDC measurement is done.
#define CKMD_MIS_TDCDONE                                            0x00000100U
#define CKMD_MIS_TDCDONE_M                                          0x00000100U
#define CKMD_MIS_TDCDONE_S                                                   8U

// Field:     [7] ADCPEAKUPD
//
// HFXT-ADC PEAK measurement update event.
//
// Indicates that the HFXT-ADC PEAK measurement is done.
#define CKMD_MIS_ADCPEAKUPD                                         0x00000080U
#define CKMD_MIS_ADCPEAKUPD_M                                       0x00000080U
#define CKMD_MIS_ADCPEAKUPD_S                                                7U

// Field:     [6] ADCBIASUPD
//
// HFXT-ADC BIAS measurement update event.
//
// Indicates that the HFXT-ADC BIAS measurement is done.
#define CKMD_MIS_ADCBIASUPD                                         0x00000040U
#define CKMD_MIS_ADCBIASUPD_M                                       0x00000040U
#define CKMD_MIS_ADCBIASUPD_S                                                6U

// Field:     [5] ADCCOMPUPD
//
// HFXT-ADC comparison update event.
//
// Indicates that the HFXT-ADC comparison is done.
#define CKMD_MIS_ADCCOMPUPD                                         0x00000020U
#define CKMD_MIS_ADCCOMPUPD_M                                       0x00000020U
#define CKMD_MIS_ADCCOMPUPD_S                                                5U

// Field:     [4] TRACKREFOOR
//
// Out-of-range indication from the tracking loop.
//
// Indicates that the selected reference clock frequency of the tracking loop
// is out-of-range.
#define CKMD_MIS_TRACKREFOOR                                        0x00000010U
#define CKMD_MIS_TRACKREFOOR_M                                      0x00000010U
#define CKMD_MIS_TRACKREFOOR_S                                               4U

// Field:     [3] TRACKREFLOSS
//
// Clock loss indication from the tracking loop.
//
// Indicates that the selected reference clock of the tracking loop is lost.
#define CKMD_MIS_TRACKREFLOSS                                       0x00000008U
#define CKMD_MIS_TRACKREFLOSS_M                                     0x00000008U
#define CKMD_MIS_TRACKREFLOSS_S                                              3U

// Field:     [2] HFXTAMPGOOD
//
// HFXT amplitude good indication.
#define CKMD_MIS_HFXTAMPGOOD                                        0x00000004U
#define CKMD_MIS_HFXTAMPGOOD_M                                      0x00000004U
#define CKMD_MIS_HFXTAMPGOOD_S                                               2U

// Field:     [1] HFXTFAULT
//
// HFXT fault indication.
//
// Indicates that HFXT did not start correctly, or its frequency is too low.
// HFXT will not recover from this fault and has to be restarted.
// This is only a one-time check at HFXT startup.
#define CKMD_MIS_HFXTFAULT                                          0x00000002U
#define CKMD_MIS_HFXTFAULT_M                                        0x00000002U
#define CKMD_MIS_HFXTFAULT_S                                                 1U

// Field:     [0] HFXTGOOD
//
// HFXT good indication.
//
// Indicates that HFXT started correctly. The frequency is not necessarily good
// enough for radio operation.
// This is only a one-time check at HFXT startup.
#define CKMD_MIS_HFXTGOOD                                           0x00000001U
#define CKMD_MIS_HFXTGOOD_M                                         0x00000001U
#define CKMD_MIS_HFXTGOOD_S                                                  0U

//*****************************************************************************
//
// Register: CKMD_O_ISET
//
//*****************************************************************************
// Field:    [23] HFOSCSETTLED
//
// Indicates that HFOSC has settled, based on LOOPCFG.SETTLED_TARGET and
// LOOPCFG1.SETTLEIRQ
#define CKMD_ISET_HFOSCSETTLED                                      0x00800000U
#define CKMD_ISET_HFOSCSETTLED_M                                    0x00800000U
#define CKMD_ISET_HFOSCSETTLED_S                                            23U

// Field:    [22] LFGEARRSTRTLIM
//
// Indicates that the LFINC filter gearing mechanism has restarted more than
// LFINCCTL2.GEARRSTRTLIM
#define CKMD_ISET_LFGEARRSTRTLIM                                    0x00400000U
#define CKMD_ISET_LFGEARRSTRTLIM_M                                  0x00400000U
#define CKMD_ISET_LFGEARRSTRTLIM_S                                          22U

// Field:    [20] SYSUNDERCLOCKED
//
// Indicates system frequency has been lowered. It will be set only if
// HFTRACKCTL.UNDERCLK is appropriately configured and HFOSC tracking loop is
// not running.
#define CKMD_ISET_SYSUNDERCLOCKED                                   0x00100000U
#define CKMD_ISET_SYSUNDERCLOCKED_M                                 0x00100000U
#define CKMD_ISET_SYSUNDERCLOCKED_S                                         20U

// Field:    [19] AFOSCGOOD
//
// AFOSC good indication.
#define CKMD_ISET_AFOSCGOOD                                         0x00080000U
#define CKMD_ISET_AFOSCGOOD_M                                       0x00080000U
#define CKMD_ISET_AFOSCGOOD_S                                               19U

// Field:    [18] TRACKREFAFOOR
//
// Out-of-range indication from the AFOSC tracking loop.
//
// Indicates that the reference clock frequency of AFOSC tracking loop is
// out-of-range.
#define CKMD_ISET_TRACKREFAFOOR                                     0x00040000U
#define CKMD_ISET_TRACKREFAFOOR_M                                   0x00040000U
#define CKMD_ISET_TRACKREFAFOOR_S                                           18U

// Field:    [17] LFTICK
//
// 32kHz TICK to RTC and WDT.
//
// Either derived from selected LFCLK or generated from CLKULL in absence of
// LFCLK.
#define CKMD_ISET_LFTICK                                            0x00020000U
#define CKMD_ISET_LFTICK_M                                          0x00020000U
#define CKMD_ISET_LFTICK_S                                                  17U

// Field:    [16] LFGEARRSTRT
//
// LFINC filter gearing restart.
//
// Indicates that the LFINC filter restarted gearing. Subsequent LFINC
// estimates may have higher variation.
#define CKMD_ISET_LFGEARRSTRT                                       0x00010000U
#define CKMD_ISET_LFGEARRSTRT_M                                     0x00010000U
#define CKMD_ISET_LFGEARRSTRT_S                                             16U

// Field:    [15] AMPSETTLED
//
// HFXT Amplitude compensation - settled
//
// Indicates that the amplitude compensation FSM has reached the SETTLED or
// TCXOMODE state,
// and the controls configured in HFXTTARG or HFXTDYN are reached.
#define CKMD_ISET_AMPSETTLED                                        0x00008000U
#define CKMD_ISET_AMPSETTLED_M                                      0x00008000U
#define CKMD_ISET_AMPSETTLED_S                                              15U

// Field:    [14] AMPCTRLATTARG
//
// HFXT Amplitude compensation - controls at target
//
// Indicates that the control values configured in HFXTTARG or HFXTDYN are
// reached.
// Applies to Q1CAP, Q2CAP and IREF.
#define CKMD_ISET_AMPCTRLATTARG                                     0x00004000U
#define CKMD_ISET_AMPCTRLATTARG_M                                   0x00004000U
#define CKMD_ISET_AMPCTRLATTARG_S                                           14U

// Field:    [13] PRELFEDGE
//
// Pre-LF clock edge detect.
//
// Indicates that a positive edge occured on the selected pre-LF clock
// CLKSEL.PRELFCLK'
// Can be used by software to confirm that a LF clock source is running and
// within the expected frequency,
// before selecting it as the main LF clock source.
#define CKMD_ISET_PRELFEDGE                                         0x00002000U
#define CKMD_ISET_PRELFEDGE_M                                       0x00002000U
#define CKMD_ISET_PRELFEDGE_S                                               13U

// Field:    [12] LFCLKLOSS
//
// LF clock is lost.
//
// Indicates that no LF clock edge occured for ~49us (~1.6 times nominal
// period).
// The system will automatically fall-back to generating LFTICK based on
// CLKULL,
// to avoid timing corruption.
// Note that this signal is NOT related to the analog LF clock-loss detector
// which can reset the device during STANDBY.
#define CKMD_ISET_LFCLKLOSS                                         0x00001000U
#define CKMD_ISET_LFCLKLOSS_M                                       0x00001000U
#define CKMD_ISET_LFCLKLOSS_S                                               12U

// Field:    [11] LFCLKOOR
//
// LF clock period out-of-range.
//
// Indicates that a LF clock period was measured to be out-of-range,
// according to LFQUALCTL.MAXERR.
#define CKMD_ISET_LFCLKOOR                                          0x00000800U
#define CKMD_ISET_LFCLKOOR_M                                        0x00000800U
#define CKMD_ISET_LFCLKOOR_S                                                11U

// Field:    [10] LFCLKGOOD
//
// LF clock good.
//
// Indicates that the LF clock is good, according to the configuration in
// LFQUALCTL.
#define CKMD_ISET_LFCLKGOOD                                         0x00000400U
#define CKMD_ISET_LFCLKGOOD_M                                       0x00000400U
#define CKMD_ISET_LFCLKGOOD_S                                               10U

// Field:     [9] LFINCUPD
//
// LFINC updated.
//
// Indicates that a new LFINC measurement value is available in
// LFCLKSTAT.LFINC.
#define CKMD_ISET_LFINCUPD                                          0x00000200U
#define CKMD_ISET_LFINCUPD_M                                        0x00000200U
#define CKMD_ISET_LFINCUPD_S                                                 9U

// Field:     [8] TDCDONE
//
// TDC done event.
//
// Indicates that the TDC measurement is done.
#define CKMD_ISET_TDCDONE                                           0x00000100U
#define CKMD_ISET_TDCDONE_M                                         0x00000100U
#define CKMD_ISET_TDCDONE_S                                                  8U

// Field:     [7] ADCPEAKUPD
//
// HFXT-ADC PEAK measurement update event.
//
// Indicates that the HFXT-ADC PEAK measurement is done.
#define CKMD_ISET_ADCPEAKUPD                                        0x00000080U
#define CKMD_ISET_ADCPEAKUPD_M                                      0x00000080U
#define CKMD_ISET_ADCPEAKUPD_S                                               7U

// Field:     [6] ADCBIASUPD
//
// HFXT-ADC BIAS measurement update event.
//
// Indicates that the HFXT-ADC BIAS measurement is done.
#define CKMD_ISET_ADCBIASUPD                                        0x00000040U
#define CKMD_ISET_ADCBIASUPD_M                                      0x00000040U
#define CKMD_ISET_ADCBIASUPD_S                                               6U

// Field:     [5] ADCCOMPUPD
//
// HFXT-ADC comparison update event.
//
// Indicates that the HFXT-ADC comparison is done.
#define CKMD_ISET_ADCCOMPUPD                                        0x00000020U
#define CKMD_ISET_ADCCOMPUPD_M                                      0x00000020U
#define CKMD_ISET_ADCCOMPUPD_S                                               5U

// Field:     [4] TRACKREFOOR
//
// Out-of-range indication from the tracking loop.
//
// Indicates that the selected reference clock frequency of the tracking loop
// is out-of-range.
#define CKMD_ISET_TRACKREFOOR                                       0x00000010U
#define CKMD_ISET_TRACKREFOOR_M                                     0x00000010U
#define CKMD_ISET_TRACKREFOOR_S                                              4U

// Field:     [3] TRACKREFLOSS
//
// Clock loss indication from the tracking loop.
//
// Indicates that the selected reference clock of the tracking loop is lost.
#define CKMD_ISET_TRACKREFLOSS                                      0x00000008U
#define CKMD_ISET_TRACKREFLOSS_M                                    0x00000008U
#define CKMD_ISET_TRACKREFLOSS_S                                             3U

// Field:     [2] HFXTAMPGOOD
//
// HFXT amplitude good indication.
#define CKMD_ISET_HFXTAMPGOOD                                       0x00000004U
#define CKMD_ISET_HFXTAMPGOOD_M                                     0x00000004U
#define CKMD_ISET_HFXTAMPGOOD_S                                              2U

// Field:     [1] HFXTFAULT
//
// HFXT fault indication.
//
// Indicates that HFXT did not start correctly, or its frequency is too low.
// HFXT will not recover from this fault and has to be restarted.
// This is only a one-time check at HFXT startup.
#define CKMD_ISET_HFXTFAULT                                         0x00000002U
#define CKMD_ISET_HFXTFAULT_M                                       0x00000002U
#define CKMD_ISET_HFXTFAULT_S                                                1U

// Field:     [0] HFXTGOOD
//
// HFXT good indication.
//
// Indicates that HFXT started correctly. The frequency is not necessarily good
// enough for radio operation.
// This is only a one-time check at HFXT startup.
#define CKMD_ISET_HFXTGOOD                                          0x00000001U
#define CKMD_ISET_HFXTGOOD_M                                        0x00000001U
#define CKMD_ISET_HFXTGOOD_S                                                 0U

//*****************************************************************************
//
// Register: CKMD_O_ICLR
//
//*****************************************************************************
// Field:    [23] HFOSCSETTLED
//
// Indicates that HFOSC has settled, based on LOOPCFG.SETTLED_TARGET and
// LOOPCFG1.SETTLEIRQ
#define CKMD_ICLR_HFOSCSETTLED                                      0x00800000U
#define CKMD_ICLR_HFOSCSETTLED_M                                    0x00800000U
#define CKMD_ICLR_HFOSCSETTLED_S                                            23U

// Field:    [22] LFGEARRSTRTLIM
//
// Indicates that the LFINC filter gearing mechanism has restarted more than
// LFINCCTL2.GEARRSTRTLIM
#define CKMD_ICLR_LFGEARRSTRTLIM                                    0x00400000U
#define CKMD_ICLR_LFGEARRSTRTLIM_M                                  0x00400000U
#define CKMD_ICLR_LFGEARRSTRTLIM_S                                          22U

// Field:    [20] SYSUNDERCLOCKED
//
// Indicates system frequency has been lowered. It will be set only if
// HFTRACKCTL.UNDERCLK is appropriately configured and HFOSC tracking loop is
// not running.
#define CKMD_ICLR_SYSUNDERCLOCKED                                   0x00100000U
#define CKMD_ICLR_SYSUNDERCLOCKED_M                                 0x00100000U
#define CKMD_ICLR_SYSUNDERCLOCKED_S                                         20U

// Field:    [19] AFOSCGOOD
//
// AFOSC good indication.
#define CKMD_ICLR_AFOSCGOOD                                         0x00080000U
#define CKMD_ICLR_AFOSCGOOD_M                                       0x00080000U
#define CKMD_ICLR_AFOSCGOOD_S                                               19U

// Field:    [18] TRACKREFAFOOR
//
// Out-of-range indication from the AFOSC tracking loop.
//
// Indicates that the reference clock frequency of AFOSC tracking loop is
// out-of-range.
#define CKMD_ICLR_TRACKREFAFOOR                                     0x00040000U
#define CKMD_ICLR_TRACKREFAFOOR_M                                   0x00040000U
#define CKMD_ICLR_TRACKREFAFOOR_S                                           18U

// Field:    [17] LFTICK
//
// 32kHz TICK to RTC and WDT.
//
// Either derived from selected LFCLK or generated from CLKULL in absence of
// LFCLK.
#define CKMD_ICLR_LFTICK                                            0x00020000U
#define CKMD_ICLR_LFTICK_M                                          0x00020000U
#define CKMD_ICLR_LFTICK_S                                                  17U

// Field:    [16] LFGEARRSTRT
//
// LFINC filter gearing restart.
//
// Indicates that the LFINC filter restarted gearing. Subsequent LFINC
// estimates may have higher variation.
#define CKMD_ICLR_LFGEARRSTRT                                       0x00010000U
#define CKMD_ICLR_LFGEARRSTRT_M                                     0x00010000U
#define CKMD_ICLR_LFGEARRSTRT_S                                             16U

// Field:    [15] AMPSETTLED
//
// HFXT Amplitude compensation - settled
//
// Indicates that the amplitude compensation FSM has reached the SETTLED or
// TCXOMODE state,
// and the controls configured in HFXTTARG or HFXTDYN are reached.
#define CKMD_ICLR_AMPSETTLED                                        0x00008000U
#define CKMD_ICLR_AMPSETTLED_M                                      0x00008000U
#define CKMD_ICLR_AMPSETTLED_S                                              15U

// Field:    [14] AMPCTRLATTARG
//
// HFXT Amplitude compensation - controls at target
//
// Indicates that the control values configured in HFXTTARG or HFXTDYN are
// reached.
// Applies to Q1CAP, Q2CAP and IREF.
#define CKMD_ICLR_AMPCTRLATTARG                                     0x00004000U
#define CKMD_ICLR_AMPCTRLATTARG_M                                   0x00004000U
#define CKMD_ICLR_AMPCTRLATTARG_S                                           14U

// Field:    [13] PRELFEDGE
//
// Pre-LF clock edge detect.
//
// Indicates that a positive edge occured on the selected pre-LF clock
// CLKSEL.PRELFCLK'
// Can be used by software to confirm that a LF clock source is running and
// within the expected frequency,
// before selecting it as the main LF clock source.
#define CKMD_ICLR_PRELFEDGE                                         0x00002000U
#define CKMD_ICLR_PRELFEDGE_M                                       0x00002000U
#define CKMD_ICLR_PRELFEDGE_S                                               13U

// Field:    [12] LFCLKLOSS
//
// LF clock is lost.
//
// Indicates that no LF clock edge occured for ~49us (~1.6 times nominal
// period).
// The system will automatically fall-back to generating LFTICK based on
// CLKULL,
// to avoid timing corruption.
// Note that this signal is NOT related to the analog LF clock-loss detector
// which can reset the device during STANDBY.
#define CKMD_ICLR_LFCLKLOSS                                         0x00001000U
#define CKMD_ICLR_LFCLKLOSS_M                                       0x00001000U
#define CKMD_ICLR_LFCLKLOSS_S                                               12U

// Field:    [11] LFCLKOOR
//
// LF clock period out-of-range.
//
// Indicates that a LF clock period was measured to be out-of-range,
// according to LFQUALCTL.MAXERR.
#define CKMD_ICLR_LFCLKOOR                                          0x00000800U
#define CKMD_ICLR_LFCLKOOR_M                                        0x00000800U
#define CKMD_ICLR_LFCLKOOR_S                                                11U

// Field:    [10] LFCLKGOOD
//
// LF clock good.
//
// Indicates that the LF clock is good, according to the configuration in
// LFQUALCTL.
#define CKMD_ICLR_LFCLKGOOD                                         0x00000400U
#define CKMD_ICLR_LFCLKGOOD_M                                       0x00000400U
#define CKMD_ICLR_LFCLKGOOD_S                                               10U

// Field:     [9] LFINCUPD
//
// LFINC updated.
//
// Indicates that a new LFINC measurement value is available in
// LFCLKSTAT.LFINC.
#define CKMD_ICLR_LFINCUPD                                          0x00000200U
#define CKMD_ICLR_LFINCUPD_M                                        0x00000200U
#define CKMD_ICLR_LFINCUPD_S                                                 9U

// Field:     [8] TDCDONE
//
// TDC done event.
//
// Indicates that the TDC measurement is done.
#define CKMD_ICLR_TDCDONE                                           0x00000100U
#define CKMD_ICLR_TDCDONE_M                                         0x00000100U
#define CKMD_ICLR_TDCDONE_S                                                  8U

// Field:     [7] ADCPEAKUPD
//
// HFXT-ADC PEAK measurement update event.
//
// Indicates that the HFXT-ADC PEAK measurement is done.
#define CKMD_ICLR_ADCPEAKUPD                                        0x00000080U
#define CKMD_ICLR_ADCPEAKUPD_M                                      0x00000080U
#define CKMD_ICLR_ADCPEAKUPD_S                                               7U

// Field:     [6] ADCBIASUPD
//
// HFXT-ADC BIAS measurement update event.
//
// Indicates that the HFXT-ADC BIAS measurement is done.
#define CKMD_ICLR_ADCBIASUPD                                        0x00000040U
#define CKMD_ICLR_ADCBIASUPD_M                                      0x00000040U
#define CKMD_ICLR_ADCBIASUPD_S                                               6U

// Field:     [5] ADCCOMPUPD
//
// HFXT-ADC comparison update event.
//
// Indicates that the HFXT-ADC comparison is done.
#define CKMD_ICLR_ADCCOMPUPD                                        0x00000020U
#define CKMD_ICLR_ADCCOMPUPD_M                                      0x00000020U
#define CKMD_ICLR_ADCCOMPUPD_S                                               5U

// Field:     [4] TRACKREFOOR
//
// Out-of-range indication from the tracking loop.
//
// Indicates that the selected reference clock frequency of the tracking loop
// is out-of-range.
#define CKMD_ICLR_TRACKREFOOR                                       0x00000010U
#define CKMD_ICLR_TRACKREFOOR_M                                     0x00000010U
#define CKMD_ICLR_TRACKREFOOR_S                                              4U

// Field:     [3] TRACKREFLOSS
//
// Clock loss indication from the tracking loop.
//
// Indicates that the selected reference clock of the tracking loop is lost.
#define CKMD_ICLR_TRACKREFLOSS                                      0x00000008U
#define CKMD_ICLR_TRACKREFLOSS_M                                    0x00000008U
#define CKMD_ICLR_TRACKREFLOSS_S                                             3U

// Field:     [2] HFXTAMPGOOD
//
// HFXT amplitude good indication.
#define CKMD_ICLR_HFXTAMPGOOD                                       0x00000004U
#define CKMD_ICLR_HFXTAMPGOOD_M                                     0x00000004U
#define CKMD_ICLR_HFXTAMPGOOD_S                                              2U

// Field:     [1] HFXTFAULT
//
// HFXT fault indication.
//
// Indicates that HFXT did not start correctly, or its frequency is too low.
// HFXT will not recover from this fault and has to be restarted.
// This is only a one-time check at HFXT startup.
#define CKMD_ICLR_HFXTFAULT                                         0x00000002U
#define CKMD_ICLR_HFXTFAULT_M                                       0x00000002U
#define CKMD_ICLR_HFXTFAULT_S                                                1U

// Field:     [0] HFXTGOOD
//
// HFXT good indication.
//
// Indicates that HFXT started correctly. The frequency is not necessarily good
// enough for radio operation.
// This is only a one-time check at HFXT startup.
#define CKMD_ICLR_HFXTGOOD                                          0x00000001U
#define CKMD_ICLR_HFXTGOOD_M                                        0x00000001U
#define CKMD_ICLR_HFXTGOOD_S                                                 0U

//*****************************************************************************
//
// Register: CKMD_O_IMSET
//
//*****************************************************************************
// Field:    [23] HFOSCSETTLED
//
// Indicates that HFOSC has settled, based on LOOPCFG.SETTLED_TARGET and
// LOOPCFG1.SETTLEIRQ
#define CKMD_IMSET_HFOSCSETTLED                                     0x00800000U
#define CKMD_IMSET_HFOSCSETTLED_M                                   0x00800000U
#define CKMD_IMSET_HFOSCSETTLED_S                                           23U

// Field:    [22] LFGEARRSTRTLIM
//
// Indicates that the LFINC filter gearing mechanism has restarted more than
// LFINCCTL2.GEARRSTRTLIM
#define CKMD_IMSET_LFGEARRSTRTLIM                                   0x00400000U
#define CKMD_IMSET_LFGEARRSTRTLIM_M                                 0x00400000U
#define CKMD_IMSET_LFGEARRSTRTLIM_S                                         22U

// Field:    [20] SYSUNDERCLOCKED
//
// Indicates system frequency has been lowered. It will be set only if
// HFTRACKCTL.UNDERCLK is appropriately configured and HFOSC tracking loop is
// not running.
#define CKMD_IMSET_SYSUNDERCLOCKED                                  0x00100000U
#define CKMD_IMSET_SYSUNDERCLOCKED_M                                0x00100000U
#define CKMD_IMSET_SYSUNDERCLOCKED_S                                        20U

// Field:    [19] AFOSCGOOD
//
// AFOSC good indication.
#define CKMD_IMSET_AFOSCGOOD                                        0x00080000U
#define CKMD_IMSET_AFOSCGOOD_M                                      0x00080000U
#define CKMD_IMSET_AFOSCGOOD_S                                              19U

// Field:    [18] TRACKREFAFOOR
//
// Out-of-range indication from the AFOSC tracking loop.
//
// Indicates that the reference clock frequency of AFOSC tracking loop is
// out-of-range.
#define CKMD_IMSET_TRACKREFAFOOR                                    0x00040000U
#define CKMD_IMSET_TRACKREFAFOOR_M                                  0x00040000U
#define CKMD_IMSET_TRACKREFAFOOR_S                                          18U

// Field:    [17] LFTICK
//
// 32kHz TICK to RTC and WDT.
//
// Either derived from selected LFCLK or generated from CLKULL in absence of
// LFCLK.
#define CKMD_IMSET_LFTICK                                           0x00020000U
#define CKMD_IMSET_LFTICK_M                                         0x00020000U
#define CKMD_IMSET_LFTICK_S                                                 17U

// Field:    [16] LFGEARRSTRT
//
// LFINC filter gearing restart.
//
// Indicates that the LFINC filter restarted gearing. Subsequent LFINC
// estimates may have higher variation.
#define CKMD_IMSET_LFGEARRSTRT                                      0x00010000U
#define CKMD_IMSET_LFGEARRSTRT_M                                    0x00010000U
#define CKMD_IMSET_LFGEARRSTRT_S                                            16U

// Field:    [15] AMPSETTLED
//
// HFXT Amplitude compensation - settled
//
// Indicates that the amplitude compensation FSM has reached the SETTLED or
// TCXOMODE state,
// and the controls configured in HFXTTARG or HFXTDYN are reached.
#define CKMD_IMSET_AMPSETTLED                                       0x00008000U
#define CKMD_IMSET_AMPSETTLED_M                                     0x00008000U
#define CKMD_IMSET_AMPSETTLED_S                                             15U

// Field:    [14] AMPCTRLATTARG
//
// HFXT Amplitude compensation - controls at target
//
// Indicates that the control values configured in HFXTTARG or HFXTDYN are
// reached.
// Applies to Q1CAP, Q2CAP and IREF.
#define CKMD_IMSET_AMPCTRLATTARG                                    0x00004000U
#define CKMD_IMSET_AMPCTRLATTARG_M                                  0x00004000U
#define CKMD_IMSET_AMPCTRLATTARG_S                                          14U

// Field:    [13] PRELFEDGE
//
// Pre-LF clock edge detect.
//
// Indicates that a positive edge occured on the selected pre-LF clock
// CLKSEL.PRELFCLK'
// Can be used by software to confirm that a LF clock source is running and
// within the expected frequency,
// before selecting it as the main LF clock source.
#define CKMD_IMSET_PRELFEDGE                                        0x00002000U
#define CKMD_IMSET_PRELFEDGE_M                                      0x00002000U
#define CKMD_IMSET_PRELFEDGE_S                                              13U

// Field:    [12] LFCLKLOSS
//
// LF clock is lost.
//
// Indicates that no LF clock edge occured for ~49us (~1.6 times nominal
// period).
// The system will automatically fall-back to generating LFTICK based on
// CLKULL,
// to avoid timing corruption.
// Note that this signal is NOT related to the analog LF clock-loss detector
// which can reset the device during STANDBY.
#define CKMD_IMSET_LFCLKLOSS                                        0x00001000U
#define CKMD_IMSET_LFCLKLOSS_M                                      0x00001000U
#define CKMD_IMSET_LFCLKLOSS_S                                              12U

// Field:    [11] LFCLKOOR
//
// LF clock period out-of-range.
//
// Indicates that a LF clock period was measured to be out-of-range,
// according to LFQUALCTL.MAXERR.
#define CKMD_IMSET_LFCLKOOR                                         0x00000800U
#define CKMD_IMSET_LFCLKOOR_M                                       0x00000800U
#define CKMD_IMSET_LFCLKOOR_S                                               11U

// Field:    [10] LFCLKGOOD
//
// LF clock good.
//
// Indicates that the LF clock is good, according to the configuration in
// LFQUALCTL.
#define CKMD_IMSET_LFCLKGOOD                                        0x00000400U
#define CKMD_IMSET_LFCLKGOOD_M                                      0x00000400U
#define CKMD_IMSET_LFCLKGOOD_S                                              10U

// Field:     [9] LFINCUPD
//
// LFINC updated.
//
// Indicates that a new LFINC measurement value is available in
// LFCLKSTAT.LFINC.
#define CKMD_IMSET_LFINCUPD                                         0x00000200U
#define CKMD_IMSET_LFINCUPD_M                                       0x00000200U
#define CKMD_IMSET_LFINCUPD_S                                                9U

// Field:     [8] TDCDONE
//
// TDC done event.
//
// Indicates that the TDC measurement is done.
#define CKMD_IMSET_TDCDONE                                          0x00000100U
#define CKMD_IMSET_TDCDONE_M                                        0x00000100U
#define CKMD_IMSET_TDCDONE_S                                                 8U

// Field:     [7] ADCPEAKUPD
//
// HFXT-ADC PEAK measurement update event.
//
// Indicates that the HFXT-ADC PEAK measurement is done.
#define CKMD_IMSET_ADCPEAKUPD                                       0x00000080U
#define CKMD_IMSET_ADCPEAKUPD_M                                     0x00000080U
#define CKMD_IMSET_ADCPEAKUPD_S                                              7U

// Field:     [6] ADCBIASUPD
//
// HFXT-ADC BIAS measurement update event.
//
// Indicates that the HFXT-ADC BIAS measurement is done.
#define CKMD_IMSET_ADCBIASUPD                                       0x00000040U
#define CKMD_IMSET_ADCBIASUPD_M                                     0x00000040U
#define CKMD_IMSET_ADCBIASUPD_S                                              6U

// Field:     [5] ADCCOMPUPD
//
// HFXT-ADC comparison update event.
//
// Indicates that the HFXT-ADC comparison is done.
#define CKMD_IMSET_ADCCOMPUPD                                       0x00000020U
#define CKMD_IMSET_ADCCOMPUPD_M                                     0x00000020U
#define CKMD_IMSET_ADCCOMPUPD_S                                              5U

// Field:     [4] TRACKREFOOR
//
// Out-of-range indication from the tracking loop.
//
// Indicates that the selected reference clock frequency of the tracking loop
// is out-of-range.
#define CKMD_IMSET_TRACKREFOOR                                      0x00000010U
#define CKMD_IMSET_TRACKREFOOR_M                                    0x00000010U
#define CKMD_IMSET_TRACKREFOOR_S                                             4U

// Field:     [3] TRACKREFLOSS
//
// Clock loss indication from the tracking loop.
//
// Indicates that the selected reference clock of the tracking loop is lost.
#define CKMD_IMSET_TRACKREFLOSS                                     0x00000008U
#define CKMD_IMSET_TRACKREFLOSS_M                                   0x00000008U
#define CKMD_IMSET_TRACKREFLOSS_S                                            3U

// Field:     [2] HFXTAMPGOOD
//
// HFXT amplitude good indication.
#define CKMD_IMSET_HFXTAMPGOOD                                      0x00000004U
#define CKMD_IMSET_HFXTAMPGOOD_M                                    0x00000004U
#define CKMD_IMSET_HFXTAMPGOOD_S                                             2U

// Field:     [1] HFXTFAULT
//
// HFXT fault indication.
//
// Indicates that HFXT did not start correctly, or its frequency is too low.
// HFXT will not recover from this fault and has to be restarted.
// This is only a one-time check at HFXT startup.
#define CKMD_IMSET_HFXTFAULT                                        0x00000002U
#define CKMD_IMSET_HFXTFAULT_M                                      0x00000002U
#define CKMD_IMSET_HFXTFAULT_S                                               1U

// Field:     [0] HFXTGOOD
//
// HFXT good indication.
//
// Indicates that HFXT started correctly. The frequency is not necessarily good
// enough for radio operation.
// This is only a one-time check at HFXT startup.
#define CKMD_IMSET_HFXTGOOD                                         0x00000001U
#define CKMD_IMSET_HFXTGOOD_M                                       0x00000001U
#define CKMD_IMSET_HFXTGOOD_S                                                0U

//*****************************************************************************
//
// Register: CKMD_O_IMCLR
//
//*****************************************************************************
// Field:    [23] HFOSCSETTLED
//
// Indicates that HFOSC has settled, based on LOOPCFG.SETTLED_TARGET and
// LOOPCFG1.SETTLEIRQ
#define CKMD_IMCLR_HFOSCSETTLED                                     0x00800000U
#define CKMD_IMCLR_HFOSCSETTLED_M                                   0x00800000U
#define CKMD_IMCLR_HFOSCSETTLED_S                                           23U

// Field:    [22] LFGEARRSTRTLIM
//
// Indicates that the LFINC filter gearing mechanism has restarted more than
// LFINCCTL2.GEARRSTRTLIM
#define CKMD_IMCLR_LFGEARRSTRTLIM                                   0x00400000U
#define CKMD_IMCLR_LFGEARRSTRTLIM_M                                 0x00400000U
#define CKMD_IMCLR_LFGEARRSTRTLIM_S                                         22U

// Field:    [20] SYSUNDERCLOCKED
//
// Indicates system frequency has been lowered. It will be set only if
// HFTRACKCTL.UNDERCLK is appropriately configured and HFOSC tracking loop is
// not running.
#define CKMD_IMCLR_SYSUNDERCLOCKED                                  0x00100000U
#define CKMD_IMCLR_SYSUNDERCLOCKED_M                                0x00100000U
#define CKMD_IMCLR_SYSUNDERCLOCKED_S                                        20U

// Field:    [19] AFOSCGOOD
//
// AFOSC good indication.
#define CKMD_IMCLR_AFOSCGOOD                                        0x00080000U
#define CKMD_IMCLR_AFOSCGOOD_M                                      0x00080000U
#define CKMD_IMCLR_AFOSCGOOD_S                                              19U

// Field:    [18] TRACKREFAFOOR
//
// Out-of-range indication from the AFOSC tracking loop.
//
// Indicates that the reference clock frequency of AFOSC tracking loop is
// out-of-range.
#define CKMD_IMCLR_TRACKREFAFOOR                                    0x00040000U
#define CKMD_IMCLR_TRACKREFAFOOR_M                                  0x00040000U
#define CKMD_IMCLR_TRACKREFAFOOR_S                                          18U

// Field:    [17] LFTICK
//
// 32kHz TICK to RTC and WDT.
//
// Either derived from selected LFCLK or generated from CLKULL in absence of
// LFCLK.
#define CKMD_IMCLR_LFTICK                                           0x00020000U
#define CKMD_IMCLR_LFTICK_M                                         0x00020000U
#define CKMD_IMCLR_LFTICK_S                                                 17U

// Field:    [16] LFGEARRSTRT
//
// LFINC filter gearing restart.
//
// Indicates that the LFINC filter restarted gearing. Subsequent LFINC
// estimates may have higher variation.
#define CKMD_IMCLR_LFGEARRSTRT                                      0x00010000U
#define CKMD_IMCLR_LFGEARRSTRT_M                                    0x00010000U
#define CKMD_IMCLR_LFGEARRSTRT_S                                            16U

// Field:    [15] AMPSETTLED
//
// HFXT Amplitude compensation - settled
//
// Indicates that the amplitude compensation FSM has reached the SETTLED or
// TCXOMODE state,
// and the controls configured in HFXTTARG or HFXTDYN are reached.
#define CKMD_IMCLR_AMPSETTLED                                       0x00008000U
#define CKMD_IMCLR_AMPSETTLED_M                                     0x00008000U
#define CKMD_IMCLR_AMPSETTLED_S                                             15U

// Field:    [14] AMPCTRLATTARG
//
// HFXT Amplitude compensation - controls at target
//
// Indicates that the control values configured in HFXTTARG or HFXTDYN are
// reached.
// Applies to Q1CAP, Q2CAP and IREF.
#define CKMD_IMCLR_AMPCTRLATTARG                                    0x00004000U
#define CKMD_IMCLR_AMPCTRLATTARG_M                                  0x00004000U
#define CKMD_IMCLR_AMPCTRLATTARG_S                                          14U

// Field:    [13] PRELFEDGE
//
// Pre-LF clock edge detect.
//
// Indicates that a positive edge occured on the selected pre-LF clock
// CLKSEL.PRELFCLK'
// Can be used by software to confirm that a LF clock source is running and
// within the expected frequency,
// before selecting it as the main LF clock source.
#define CKMD_IMCLR_PRELFEDGE                                        0x00002000U
#define CKMD_IMCLR_PRELFEDGE_M                                      0x00002000U
#define CKMD_IMCLR_PRELFEDGE_S                                              13U

// Field:    [12] LFCLKLOSS
//
// LF clock is lost.
//
// Indicates that no LF clock edge occured for ~49us (~1.6 times nominal
// period).
// The system will automatically fall-back to generating LFTICK based on
// CLKULL,
// to avoid timing corruption.
// Note that this signal is NOT related to the analog LF clock-loss detector
// which can reset the device during STANDBY.
#define CKMD_IMCLR_LFCLKLOSS                                        0x00001000U
#define CKMD_IMCLR_LFCLKLOSS_M                                      0x00001000U
#define CKMD_IMCLR_LFCLKLOSS_S                                              12U

// Field:    [11] LFCLKOOR
//
// LF clock period out-of-range.
//
// Indicates that a LF clock period was measured to be out-of-range,
// according to LFQUALCTL.MAXERR.
#define CKMD_IMCLR_LFCLKOOR                                         0x00000800U
#define CKMD_IMCLR_LFCLKOOR_M                                       0x00000800U
#define CKMD_IMCLR_LFCLKOOR_S                                               11U

// Field:    [10] LFCLKGOOD
//
// LF clock good.
//
// Indicates that the LF clock is good, according to the configuration in
// LFQUALCTL.
#define CKMD_IMCLR_LFCLKGOOD                                        0x00000400U
#define CKMD_IMCLR_LFCLKGOOD_M                                      0x00000400U
#define CKMD_IMCLR_LFCLKGOOD_S                                              10U

// Field:     [9] LFINCUPD
//
// LFINC updated.
//
// Indicates that a new LFINC measurement value is available in
// LFCLKSTAT.LFINC.
#define CKMD_IMCLR_LFINCUPD                                         0x00000200U
#define CKMD_IMCLR_LFINCUPD_M                                       0x00000200U
#define CKMD_IMCLR_LFINCUPD_S                                                9U

// Field:     [8] TDCDONE
//
// TDC done event.
//
// Indicates that the TDC measurement is done.
#define CKMD_IMCLR_TDCDONE                                          0x00000100U
#define CKMD_IMCLR_TDCDONE_M                                        0x00000100U
#define CKMD_IMCLR_TDCDONE_S                                                 8U

// Field:     [7] ADCPEAKUPD
//
// HFXT-ADC PEAK measurement update event.
//
// Indicates that the HFXT-ADC PEAK measurement is done.
#define CKMD_IMCLR_ADCPEAKUPD                                       0x00000080U
#define CKMD_IMCLR_ADCPEAKUPD_M                                     0x00000080U
#define CKMD_IMCLR_ADCPEAKUPD_S                                              7U

// Field:     [6] ADCBIASUPD
//
// HFXT-ADC BIAS measurement update event.
//
// Indicates that the HFXT-ADC BIAS measurement is done.
#define CKMD_IMCLR_ADCBIASUPD                                       0x00000040U
#define CKMD_IMCLR_ADCBIASUPD_M                                     0x00000040U
#define CKMD_IMCLR_ADCBIASUPD_S                                              6U

// Field:     [5] ADCCOMPUPD
//
// HFXT-ADC comparison update event.
//
// Indicates that the HFXT-ADC comparison is done.
#define CKMD_IMCLR_ADCCOMPUPD                                       0x00000020U
#define CKMD_IMCLR_ADCCOMPUPD_M                                     0x00000020U
#define CKMD_IMCLR_ADCCOMPUPD_S                                              5U

// Field:     [4] TRACKREFOOR
//
// Out-of-range indication from the tracking loop.
//
// Indicates that the selected reference clock frequency of the tracking loop
// is out-of-range.
#define CKMD_IMCLR_TRACKREFOOR                                      0x00000010U
#define CKMD_IMCLR_TRACKREFOOR_M                                    0x00000010U
#define CKMD_IMCLR_TRACKREFOOR_S                                             4U

// Field:     [3] TRACKREFLOSS
//
// Clock loss indication from the tracking loop.
//
// Indicates that the selected reference clock of the tracking loop is lost.
#define CKMD_IMCLR_TRACKREFLOSS                                     0x00000008U
#define CKMD_IMCLR_TRACKREFLOSS_M                                   0x00000008U
#define CKMD_IMCLR_TRACKREFLOSS_S                                            3U

// Field:     [2] HFXTAMPGOOD
//
// HFXT amplitude good indication.
#define CKMD_IMCLR_HFXTAMPGOOD                                      0x00000004U
#define CKMD_IMCLR_HFXTAMPGOOD_M                                    0x00000004U
#define CKMD_IMCLR_HFXTAMPGOOD_S                                             2U

// Field:     [1] HFXTFAULT
//
// HFXT fault indication.
//
// Indicates that HFXT did not start correctly, or its frequency is too low.
// HFXT will not recover from this fault and has to be restarted.
// This is only a one-time check at HFXT startup.
#define CKMD_IMCLR_HFXTFAULT                                        0x00000002U
#define CKMD_IMCLR_HFXTFAULT_M                                      0x00000002U
#define CKMD_IMCLR_HFXTFAULT_S                                               1U

// Field:     [0] HFXTGOOD
//
// HFXT good indication.
//
// Indicates that HFXT started correctly. The frequency is not necessarily good
// enough for radio operation.
// This is only a one-time check at HFXT startup.
#define CKMD_IMCLR_HFXTGOOD                                         0x00000001U
#define CKMD_IMCLR_HFXTGOOD_M                                       0x00000001U
#define CKMD_IMCLR_HFXTGOOD_S                                                0U

//*****************************************************************************
//
// Register: CKMD_O_HFOSCCTL
//
//*****************************************************************************
// Field: [31:24] PW
//
// Internal. Only to be used through TI provided API.
#define CKMD_HFOSCCTL_PW_W                                                   8U
#define CKMD_HFOSCCTL_PW_M                                          0xFF000000U
#define CKMD_HFOSCCTL_PW_S                                                  24U

// Field:     [1] FORCEOFF
//
// Internal. Only to be used through TI provided API.
#define CKMD_HFOSCCTL_FORCEOFF                                      0x00000002U
#define CKMD_HFOSCCTL_FORCEOFF_M                                    0x00000002U
#define CKMD_HFOSCCTL_FORCEOFF_S                                             1U

// Field:     [0] QUALBYP
//
// Internal. Only to be used through TI provided API.
#define CKMD_HFOSCCTL_QUALBYP                                       0x00000001U
#define CKMD_HFOSCCTL_QUALBYP_M                                     0x00000001U
#define CKMD_HFOSCCTL_QUALBYP_S                                              0U

//*****************************************************************************
//
// Register: CKMD_O_HFXTCTL
//
//*****************************************************************************
// Field:    [31] AMPOVR
//
// Internal. Only to be used through TI provided API.
#define CKMD_HFXTCTL_AMPOVR                                         0x80000000U
#define CKMD_HFXTCTL_AMPOVR_M                                       0x80000000U
#define CKMD_HFXTCTL_AMPOVR_S                                               31U

// Field:    [26] BIASEN
//
// Internal. Only to be used through TI provided API.
#define CKMD_HFXTCTL_BIASEN                                         0x04000000U
#define CKMD_HFXTCTL_BIASEN_M                                       0x04000000U
#define CKMD_HFXTCTL_BIASEN_S                                               26U

// Field:    [25] LPBUFEN
//
// Internal. Only to be used through TI provided API.
#define CKMD_HFXTCTL_LPBUFEN                                        0x02000000U
#define CKMD_HFXTCTL_LPBUFEN_M                                      0x02000000U
#define CKMD_HFXTCTL_LPBUFEN_S                                              25U

// Field:    [24] INJECT
//
// Internal. Only to be used through TI provided API.
#define CKMD_HFXTCTL_INJECT                                         0x01000000U
#define CKMD_HFXTCTL_INJECT_M                                       0x01000000U
#define CKMD_HFXTCTL_INJECT_S                                               24U

// Field:    [23] QUALBYP
//
// Internal. Only to be used through TI provided API.
#define CKMD_HFXTCTL_QUALBYP                                        0x00800000U
#define CKMD_HFXTCTL_QUALBYP_M                                      0x00800000U
#define CKMD_HFXTCTL_QUALBYP_S                                              23U

// Field:  [19:8] QUALDLY
//
// Skip potentially unstable clock cycles after enabling HFXT.
// Number of cycles skipped is 8*QUALDLY.
#define CKMD_HFXTCTL_QUALDLY_W                                              12U
#define CKMD_HFXTCTL_QUALDLY_M                                      0x000FFF00U
#define CKMD_HFXTCTL_QUALDLY_S                                               8U

// Field:     [7] TCXOMODE
//
// Temperature compensated crystal oscillator mode.
//
// Set this bit if a TXCO is connected.
#define CKMD_HFXTCTL_TCXOMODE                                       0x00000080U
#define CKMD_HFXTCTL_TCXOMODE_M                                     0x00000080U
#define CKMD_HFXTCTL_TCXOMODE_S                                              7U

// Field:     [6] TCXOTYPE
//
// Type of temperature compensated crystal used.
//
// Only has effect if TCXOMODE is set.
// ENUMs:
// CMOS                     Use with CMOS TCXO
// CLIPPEDSINE              Use with clipped-sine TCXO
#define CKMD_HFXTCTL_TCXOTYPE                                       0x00000040U
#define CKMD_HFXTCTL_TCXOTYPE_M                                     0x00000040U
#define CKMD_HFXTCTL_TCXOTYPE_S                                              6U
#define CKMD_HFXTCTL_TCXOTYPE_CMOS                                  0x00000040U
#define CKMD_HFXTCTL_TCXOTYPE_CLIPPEDSINE                           0x00000000U

// Field:     [2] AUTOEN
//
// Internal. Only to be used through TI provided API.
#define CKMD_HFXTCTL_AUTOEN                                         0x00000004U
#define CKMD_HFXTCTL_AUTOEN_M                                       0x00000004U
#define CKMD_HFXTCTL_AUTOEN_S                                                2U

// Field:     [1] HPBUFEN
//
// High performance clock buffer enable.
//
// This bit controls the clock output for the RF PLL.
// It is required for radio operation.
#define CKMD_HFXTCTL_HPBUFEN                                        0x00000002U
#define CKMD_HFXTCTL_HPBUFEN_M                                      0x00000002U
#define CKMD_HFXTCTL_HPBUFEN_S                                               1U

// Field:     [0] EN
//
// Internal. Only to be used through TI provided API.
#define CKMD_HFXTCTL_EN                                             0x00000001U
#define CKMD_HFXTCTL_EN_M                                           0x00000001U
#define CKMD_HFXTCTL_EN_S                                                    0U

//*****************************************************************************
//
// Register: CKMD_O_LFOSCCTL
//
//*****************************************************************************
// Field:     [0] EN
//
// LFOSC enable
#define CKMD_LFOSCCTL_EN                                            0x00000001U
#define CKMD_LFOSCCTL_EN_M                                          0x00000001U
#define CKMD_LFOSCCTL_EN_S                                                   0U

//*****************************************************************************
//
// Register: CKMD_O_LFXTCTL
//
//*****************************************************************************
// Field: [14:13] LEAKCOMP
//
// Leakage compensation control
// ENUMs:
// OFF                      No leakage compensation
// HALF                     Half leakage compensation
// FULL                     Full leakage compensation
#define CKMD_LFXTCTL_LEAKCOMP_W                                              2U
#define CKMD_LFXTCTL_LEAKCOMP_M                                     0x00006000U
#define CKMD_LFXTCTL_LEAKCOMP_S                                             13U
#define CKMD_LFXTCTL_LEAKCOMP_OFF                                   0x00006000U
#define CKMD_LFXTCTL_LEAKCOMP_HALF                                  0x00002000U
#define CKMD_LFXTCTL_LEAKCOMP_FULL                                  0x00000000U

// Field:    [12] BUFBIAS
//
// Control the BIAS current of the input amp in LP buffer
// ENUMs:
// MAX                      Maximum bias current: 50nA
// MIN                      Minimum bias current: 25nA
#define CKMD_LFXTCTL_BUFBIAS                                        0x00001000U
#define CKMD_LFXTCTL_BUFBIAS_M                                      0x00001000U
#define CKMD_LFXTCTL_BUFBIAS_S                                              12U
#define CKMD_LFXTCTL_BUFBIAS_MAX                                    0x00001000U
#define CKMD_LFXTCTL_BUFBIAS_MIN                                    0x00000000U

// Field:  [11:8] AMPBIAS
//
// Adjust current mirror ratio into oscillator core. This value is depending on
// crystal and is set by FW. This field uses a 2's complement encoding.
#define CKMD_LFXTCTL_AMPBIAS_W                                               4U
#define CKMD_LFXTCTL_AMPBIAS_M                                      0x00000F00U
#define CKMD_LFXTCTL_AMPBIAS_S                                               8U

// Field:   [7:6] BIASBOOST
//
// Boost oscillator amplitude
//
// This value depends on the crystal and needs to be configured by Firmware.
#define CKMD_LFXTCTL_BIASBOOST_W                                             2U
#define CKMD_LFXTCTL_BIASBOOST_M                                    0x000000C0U
#define CKMD_LFXTCTL_BIASBOOST_S                                             6U

// Field:   [5:4] REGBIAS
//
// Regulation loop bias resistor value
//
// This value depends on the crystal and needs to be configured by Firmware.
#define CKMD_LFXTCTL_REGBIAS_W                                               2U
#define CKMD_LFXTCTL_REGBIAS_M                                      0x00000030U
#define CKMD_LFXTCTL_REGBIAS_S                                               4U

// Field:     [2] HPBUFEN
//
// Control the buffer used. In normal operation, low-power buffer is used in
// all device modes. The high-performance buffer is only used for test
// purposes.
#define CKMD_LFXTCTL_HPBUFEN                                        0x00000004U
#define CKMD_LFXTCTL_HPBUFEN_M                                      0x00000004U
#define CKMD_LFXTCTL_HPBUFEN_S                                               2U

// Field:     [1] AMPREGMODE
//
// Amplitude regulation mode
// ENUMs:
// LOOPDIS                  Amplitude control loop disabled
// LOOPEN                   Amplitude control loop enabled
#define CKMD_LFXTCTL_AMPREGMODE                                     0x00000002U
#define CKMD_LFXTCTL_AMPREGMODE_M                                   0x00000002U
#define CKMD_LFXTCTL_AMPREGMODE_S                                            1U
#define CKMD_LFXTCTL_AMPREGMODE_LOOPDIS                             0x00000002U
#define CKMD_LFXTCTL_AMPREGMODE_LOOPEN                              0x00000000U

// Field:     [0] EN
//
// LFXT enable
#define CKMD_LFXTCTL_EN                                             0x00000001U
#define CKMD_LFXTCTL_EN_M                                           0x00000001U
#define CKMD_LFXTCTL_EN_S                                                    0U

//*****************************************************************************
//
// Register: CKMD_O_LFQUALCTL
//
//*****************************************************************************
// Field:  [13:8] MAXERR
//
// Maximum LFCLK period error.
//
// Value given in microseconds, 3 integer bits + 3 fractional bits.
#define CKMD_LFQUALCTL_MAXERR_W                                              6U
#define CKMD_LFQUALCTL_MAXERR_M                                     0x00003F00U
#define CKMD_LFQUALCTL_MAXERR_S                                              8U

// Field:   [7:0] CONSEC
//
// Number of consecutive times the LFCLK period error has to be
// smaller than MAXERR to be considered "good".
// Setting this value to 0 will bypass clock qualification,
// and the "good" indicator will always be 1.
#define CKMD_LFQUALCTL_CONSEC_W                                              8U
#define CKMD_LFQUALCTL_CONSEC_M                                     0x000000FFU
#define CKMD_LFQUALCTL_CONSEC_S                                              0U

//*****************************************************************************
//
// Register: CKMD_O_LFINCCTL
//
//*****************************************************************************
// Field:    [31] PREVENTSTBY
//
// Controls if the LFINC filter prevents STANBY entry until settled.
// ENUMs:
// ON                       Enable. Prevent STANDBY entry.
// OFF                      Disable. Do not prevent STANDBY entry.
#define CKMD_LFINCCTL_PREVENTSTBY                                   0x80000000U
#define CKMD_LFINCCTL_PREVENTSTBY_M                                 0x80000000U
#define CKMD_LFINCCTL_PREVENTSTBY_S                                         31U
#define CKMD_LFINCCTL_PREVENTSTBY_ON                                0x80000000U
#define CKMD_LFINCCTL_PREVENTSTBY_OFF                               0x00000000U

// Field:    [30] KEEPHFXTEN
//
// Keeps the HFXT enabled till the LFINC filter settles
// ENUMs:
// EN                       Enable. Keep HFXT enabled.
// DIS                      Disable. Do not keep HFXT enabled.
#define CKMD_LFINCCTL_KEEPHFXTEN                                    0x40000000U
#define CKMD_LFINCCTL_KEEPHFXTEN_M                                  0x40000000U
#define CKMD_LFINCCTL_KEEPHFXTEN_S                                          30U
#define CKMD_LFINCCTL_KEEPHFXTEN_EN                                 0x40000000U
#define CKMD_LFINCCTL_KEEPHFXTEN_DIS                                0x00000000U

// Field:  [29:8] INT
//
// Integral part of the LFINC filter.
//
// This value is updated by Hardware to reflect the current state of the
// filter.
// It can also be written to change the current state.
#define CKMD_LFINCCTL_INT_W                                                 22U
#define CKMD_LFINCCTL_INT_M                                         0x3FFFFF00U
#define CKMD_LFINCCTL_INT_S                                                  8U

// Field:     [7] STOPGEAR
//
// Controls the final gear of the LFINC filter.
// ENUMs:
// HIGH                     The LF filter is considered settled when
//                          LFCLKSTAT2.MAINGEAR reads 8 and
//                          LFCLKSTAT2.SUBGEAR reads 7. That means the LF
//                          filter will take 71 LF periods to settle
//                          provided the gear does not restarts.
// LOW                      The LF filter is considered settled when
//                          LFCLKSTAT2.MAINGEAR reads 9 and
//                          LFCLKSTAT2.SUBGEAR reads 7. That means the LF
//                          filter will take 79 LF periods to settle
//                          provided the gear does not restarts.
#define CKMD_LFINCCTL_STOPGEAR                                      0x00000080U
#define CKMD_LFINCCTL_STOPGEAR_M                                    0x00000080U
#define CKMD_LFINCCTL_STOPGEAR_S                                             7U
#define CKMD_LFINCCTL_STOPGEAR_HIGH                                 0x00000080U
#define CKMD_LFINCCTL_STOPGEAR_LOW                                  0x00000000U

// Field:   [6:5] ERRTHR
//
// Controls the threshold for gearing restart of the LFINC filter.
//
// Only effective if GEARRSTRT is not ONETHR or TWOTHR.
// ENUMs:
// SMALL                    Restart gearing on small error. Potentially more
//                          false restarts, faster response on small
//                          frequency shifts.
// MIDSMALL                 Middle value towards SMALL.
// MIDLARGE                 Middle value towards LARGE.
// LARGE                    Restart gearing on large error. Fewer false
//                          restarts, slower response on small frequency
//                          shifts.
#define CKMD_LFINCCTL_ERRTHR_W                                               2U
#define CKMD_LFINCCTL_ERRTHR_M                                      0x00000060U
#define CKMD_LFINCCTL_ERRTHR_S                                               5U
#define CKMD_LFINCCTL_ERRTHR_SMALL                                  0x00000060U
#define CKMD_LFINCCTL_ERRTHR_MIDSMALL                               0x00000040U
#define CKMD_LFINCCTL_ERRTHR_MIDLARGE                               0x00000020U
#define CKMD_LFINCCTL_ERRTHR_LARGE                                  0x00000000U

// Field:   [4:3] GEARRSTRT
//
// Controls gearing restart of the LFINC filter.
// ENUMs:
// TWOTHR                   Restart gearing when the error accumulator crosses
//                          the threshold twice in a row.
// ONETHR                   Restart gearing when the error accumulator crosses
//                          the threshold once.
// NEVER                    Never restart gearing. Very stable filter value,
//                          but very slow response on frequency changes.
#define CKMD_LFINCCTL_GEARRSTRT_W                                            2U
#define CKMD_LFINCCTL_GEARRSTRT_M                                   0x00000018U
#define CKMD_LFINCCTL_GEARRSTRT_S                                            3U
#define CKMD_LFINCCTL_GEARRSTRT_TWOTHR                              0x00000010U
#define CKMD_LFINCCTL_GEARRSTRT_ONETHR                              0x00000008U
#define CKMD_LFINCCTL_GEARRSTRT_NEVER                               0x00000000U

// Field:     [2] SOFTRSTRT
//
// Use a higher gear after re-enabling / wakeup.
//
// The filter will require 16-24 LFCLK periods to settle (depending on
// STOPGEAR), but may respond faster to frequency changes during STANDBY.
// ENUMs:
// ON                       Use soft gearing restarts
// OFF                      Don't use soft gearing restarts
#define CKMD_LFINCCTL_SOFTRSTRT                                     0x00000004U
#define CKMD_LFINCCTL_SOFTRSTRT_M                                   0x00000004U
#define CKMD_LFINCCTL_SOFTRSTRT_S                                            2U
#define CKMD_LFINCCTL_SOFTRSTRT_ON                                  0x00000004U
#define CKMD_LFINCCTL_SOFTRSTRT_OFF                                 0x00000000U

// Field:   [1:0] EN
//
// Enable LFINC filter. Programming with a value of 0x3 will disable the LFINC
// filter
// ENUMs:
// AUTO                     Enable based on HFOS getting settled. HFOSC gets
//                          settled after the tracking loop has updated
//                          equal to or more than LOOPCFG.SETTLED_TARGET
//                          times.
// EN                       ENABLED
// DIS                      DISABLED
#define CKMD_LFINCCTL_EN_W                                                   2U
#define CKMD_LFINCCTL_EN_M                                          0x00000003U
#define CKMD_LFINCCTL_EN_S                                                   0U
#define CKMD_LFINCCTL_EN_AUTO                                       0x00000002U
#define CKMD_LFINCCTL_EN_EN                                         0x00000001U
#define CKMD_LFINCCTL_EN_DIS                                        0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_LFINCOVR
//
//*****************************************************************************
// Field:    [31] OVERRIDE
//
// Override LF increment
//
// Use the value provided in LFINC instead of the value calculated by Hardware.
#define CKMD_LFINCOVR_OVERRIDE                                      0x80000000U
#define CKMD_LFINCOVR_OVERRIDE_M                                    0x80000000U
#define CKMD_LFINCOVR_OVERRIDE_S                                            31U

// Field:  [21:0] LFINC
//
// LF increment value
//
// This value is used when OVERRIDE is set to 1.
// Otherwise the value is calculated automatically.
// The current LFINC value can be read from [CKM.LFCLKSTAT.LFINC].
#define CKMD_LFINCOVR_LFINC_W                                               22U
#define CKMD_LFINCOVR_LFINC_M                                       0x003FFFFFU
#define CKMD_LFINCOVR_LFINC_S                                                0U

//*****************************************************************************
//
// Register: CKMD_O_AMPADCCTL
//
//*****************************************************************************
// Field:    [31] SWOVR
//
// Internal. Only to be used through TI provided API.
#define CKMD_AMPADCCTL_SWOVR                                        0x80000000U
#define CKMD_AMPADCCTL_SWOVR_M                                      0x80000000U
#define CKMD_AMPADCCTL_SWOVR_S                                              31U

// Field:    [17] PEAKDETEN
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ENABLE                   Internal. Only to be used through TI provided API.
// DISABLE                  Internal. Only to be used through TI provided API.
#define CKMD_AMPADCCTL_PEAKDETEN                                    0x00020000U
#define CKMD_AMPADCCTL_PEAKDETEN_M                                  0x00020000U
#define CKMD_AMPADCCTL_PEAKDETEN_S                                          17U
#define CKMD_AMPADCCTL_PEAKDETEN_ENABLE                             0x00020000U
#define CKMD_AMPADCCTL_PEAKDETEN_DISABLE                            0x00000000U

// Field:    [16] ADCEN
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ENABLE                   Internal. Only to be used through TI provided API.
// DISABLE                  Internal. Only to be used through TI provided API.
#define CKMD_AMPADCCTL_ADCEN                                        0x00010000U
#define CKMD_AMPADCCTL_ADCEN_M                                      0x00010000U
#define CKMD_AMPADCCTL_ADCEN_S                                              16U
#define CKMD_AMPADCCTL_ADCEN_ENABLE                                 0x00010000U
#define CKMD_AMPADCCTL_ADCEN_DISABLE                                0x00000000U

// Field:  [14:8] COMPVAL
//
// Internal. Only to be used through TI provided API.
#define CKMD_AMPADCCTL_COMPVAL_W                                             7U
#define CKMD_AMPADCCTL_COMPVAL_M                                    0x00007F00U
#define CKMD_AMPADCCTL_COMPVAL_S                                             8U

// Field:     [4] SRCSEL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// PEAK                     Internal. Only to be used through TI provided API.
// BIAS                     Internal. Only to be used through TI provided API.
#define CKMD_AMPADCCTL_SRCSEL                                       0x00000010U
#define CKMD_AMPADCCTL_SRCSEL_M                                     0x00000010U
#define CKMD_AMPADCCTL_SRCSEL_S                                              4U
#define CKMD_AMPADCCTL_SRCSEL_PEAK                                  0x00000010U
#define CKMD_AMPADCCTL_SRCSEL_BIAS                                  0x00000000U

// Field:     [1] COMPSTRT
//
// Internal. Only to be used through TI provided API.
#define CKMD_AMPADCCTL_COMPSTRT                                     0x00000002U
#define CKMD_AMPADCCTL_COMPSTRT_M                                   0x00000002U
#define CKMD_AMPADCCTL_COMPSTRT_S                                            1U

// Field:     [0] SARSTRT
//
// Internal. Only to be used through TI provided API.
#define CKMD_AMPADCCTL_SARSTRT                                      0x00000001U
#define CKMD_AMPADCCTL_SARSTRT_M                                    0x00000001U
#define CKMD_AMPADCCTL_SARSTRT_S                                             0U

//*****************************************************************************
//
// Register: CKMD_O_HFTRACKCTL
//
//*****************************************************************************
// Field:    [31] EN
//
// Enable tracking loop.
#define CKMD_HFTRACKCTL_EN                                          0x80000000U
#define CKMD_HFTRACKCTL_EN_M                                        0x80000000U
#define CKMD_HFTRACKCTL_EN_S                                                31U

// Field:    [30] DSMBYP
//
// Bypass Delta-Sigma-Modulation of fine trim.
#define CKMD_HFTRACKCTL_DSMBYP                                      0x40000000U
#define CKMD_HFTRACKCTL_DSMBYP_M                                    0x40000000U
#define CKMD_HFTRACKCTL_DSMBYP_S                                            30U

// Field: [29:28] UNDERCLK
//
// When the HFOSC tracking loop is not running, this bitfield can be used to
// set the condition to automatically lower the HFOSC frequency. This will
// prevent frequency drift that may lead to SOC instability.
// ENUMs:
// BOTH                     Temperature event from Batmon or Timer event
// BATMON                   Temperature event from Batmon
// TIMER                    Timer event
// DIS                      Disable
#define CKMD_HFTRACKCTL_UNDERCLK_W                                           2U
#define CKMD_HFTRACKCTL_UNDERCLK_M                                  0x30000000U
#define CKMD_HFTRACKCTL_UNDERCLK_S                                          28U
#define CKMD_HFTRACKCTL_UNDERCLK_BOTH                               0x30000000U
#define CKMD_HFTRACKCTL_UNDERCLK_BATMON                             0x20000000U
#define CKMD_HFTRACKCTL_UNDERCLK_TIMER                              0x10000000U
#define CKMD_HFTRACKCTL_UNDERCLK_DIS                                0x00000000U

// Field: [27:26] REFCLK
//
// Select the reference clock for the tracking loop.
// Change only while the tracking loop is disabled.
// ENUMs:
// GPI                      Select GPI as reference clock.
// LRF                      Select LRF reference clock.
// HFXT                     Select HFXT as reference clock.
#define CKMD_HFTRACKCTL_REFCLK_W                                             2U
#define CKMD_HFTRACKCTL_REFCLK_M                                    0x0C000000U
#define CKMD_HFTRACKCTL_REFCLK_S                                            26U
#define CKMD_HFTRACKCTL_REFCLK_GPI                                  0x08000000U
#define CKMD_HFTRACKCTL_REFCLK_LRF                                  0x04000000U
#define CKMD_HFTRACKCTL_REFCLK_HFXT                                 0x00000000U

// Field:  [25:0] RATIO
//
// Reference clock ratio. Ratio format is 2b.24b
//
// RATIO = 24MHz / (2*reference-frequency) * 2^24
// Commonly used reference clock frequencies are provided as enumerations.
#define CKMD_HFTRACKCTL_RATIO_W                                             26U
#define CKMD_HFTRACKCTL_RATIO_M                                     0x03FFFFFFU
#define CKMD_HFTRACKCTL_RATIO_S                                              0U

//*****************************************************************************
//
// Register: CKMD_O_LDOCTL
//
//*****************************************************************************
// Field:    [31] SWOVR
//
// Internal. Only to be used through TI provided API.
#define CKMD_LDOCTL_SWOVR                                           0x80000000U
#define CKMD_LDOCTL_SWOVR_M                                         0x80000000U
#define CKMD_LDOCTL_SWOVR_S                                                 31U

// Field:     [4] HFXTLVLEN
//
// Internal. Only to be used through TI provided API.
#define CKMD_LDOCTL_HFXTLVLEN                                       0x00000010U
#define CKMD_LDOCTL_HFXTLVLEN_M                                     0x00000010U
#define CKMD_LDOCTL_HFXTLVLEN_S                                              4U

// Field:     [3] STARTCTL
//
// Internal. Only to be used through TI provided API.
#define CKMD_LDOCTL_STARTCTL                                        0x00000008U
#define CKMD_LDOCTL_STARTCTL_M                                      0x00000008U
#define CKMD_LDOCTL_STARTCTL_S                                               3U

// Field:     [2] START
//
// Internal. Only to be used through TI provided API.
#define CKMD_LDOCTL_START                                           0x00000004U
#define CKMD_LDOCTL_START_M                                         0x00000004U
#define CKMD_LDOCTL_START_S                                                  2U

// Field:     [1] BYPASS
//
// Internal. Only to be used through TI provided API.
#define CKMD_LDOCTL_BYPASS                                          0x00000002U
#define CKMD_LDOCTL_BYPASS_M                                        0x00000002U
#define CKMD_LDOCTL_BYPASS_S                                                 1U

// Field:     [0] EN
//
// Internal. Only to be used through TI provided API.
#define CKMD_LDOCTL_EN                                              0x00000001U
#define CKMD_LDOCTL_EN_M                                            0x00000001U
#define CKMD_LDOCTL_EN_S                                                     0U

//*****************************************************************************
//
// Register: CKMD_O_NABIASCTL
//
//*****************************************************************************
// Field:     [0] EN
//
// Enable nanoamp-bias
#define CKMD_NABIASCTL_EN                                           0x00000001U
#define CKMD_NABIASCTL_EN_M                                         0x00000001U
#define CKMD_NABIASCTL_EN_S                                                  0U

//*****************************************************************************
//
// Register: CKMD_O_LFMONCTL
//
//*****************************************************************************
// Field:     [0] EN
//
// Enable LFMONITOR.
// Enable only after a LF clock source has been selected, enabled and is
// stable.
// If LFMONITOR detects a clock loss, the system will be reset.
#define CKMD_LFMONCTL_EN                                            0x00000001U
#define CKMD_LFMONCTL_EN_M                                          0x00000001U
#define CKMD_LFMONCTL_EN_S                                                   0U

//*****************************************************************************
//
// Register: CKMD_O_LFINCCTL2
//
//*****************************************************************************
// Field:    [31] ADJUSTLFINC
//
// Adjusts LFINC while transitioning from fake to real LF clock if necessary.
// For the adjustment to happen, tracking loop must be running.
#define CKMD_LFINCCTL2_ADJUSTLFINC                                  0x80000000U
#define CKMD_LFINCCTL2_ADJUSTLFINC_M                                0x80000000U
#define CKMD_LFINCCTL2_ADJUSTLFINC_S                                        31U

// Field:   [9:4] GEARRSTRTLIM
//
// Specifies the number of times gear could be restarted before raising an
// interrupt. It has no impact on the number of times gear can be reduced.
// A value of 0 indicates that the interrupt mechanism is disabled
#define CKMD_LFINCCTL2_GEARRSTRTLIM_W                                        6U
#define CKMD_LFINCCTL2_GEARRSTRTLIM_M                               0x000003F0U
#define CKMD_LFINCCTL2_GEARRSTRTLIM_S                                        4U

// Field:   [3:0] GEARREDCNT
//
// Specifies the number by which gear should be reduced post standby exit
#define CKMD_LFINCCTL2_GEARREDCNT_W                                          4U
#define CKMD_LFINCCTL2_GEARREDCNT_M                                 0x0000000FU
#define CKMD_LFINCCTL2_GEARREDCNT_S                                          0U

//*****************************************************************************
//
// Register: CKMD_O_LFCLKSEL
//
//*****************************************************************************
// Field:   [3:2] PRE
//
// Select low frequency clock source for the PRELFCLK interrupt.
//
// Can be used by Software to confirm that the clock is running and it's
// frequency is good, before selecting it in MAIN.
// ENUMs:
// EXTLF                    External LF clock through GPI.
// LFXT                     Low frequency crystal oscillator
// LFOSC                    Low frequency on-chip oscillator
// NONE                     No clock. Output will be tied low.
#define CKMD_LFCLKSEL_PRE_W                                                  2U
#define CKMD_LFCLKSEL_PRE_M                                         0x0000000CU
#define CKMD_LFCLKSEL_PRE_S                                                  2U
#define CKMD_LFCLKSEL_PRE_EXTLF                                     0x0000000CU
#define CKMD_LFCLKSEL_PRE_LFXT                                      0x00000008U
#define CKMD_LFCLKSEL_PRE_LFOSC                                     0x00000004U
#define CKMD_LFCLKSEL_PRE_NONE                                      0x00000000U

// Field:   [1:0] MAIN
//
// Select the main low frequency clock source.
//
// If running, this clock will be used to generate LFTICK and as CLKULL during
// STANDBY.
// If not running, LFTICK will be generated from HFOSC and STANDBY entry will
// be prevented.
// ENUMs:
// EXTLF                    External LF clock through GPI.
// LFXT                     Low frequency crystal oscillator
// LFOSC                    Low frequency on-chip oscillator
// FAKE                     No LF clock selected. LFTICK will be generated
//                          from HFOSC, STANDBY entry will be prevented.
#define CKMD_LFCLKSEL_MAIN_W                                                 2U
#define CKMD_LFCLKSEL_MAIN_M                                        0x00000003U
#define CKMD_LFCLKSEL_MAIN_S                                                 0U
#define CKMD_LFCLKSEL_MAIN_EXTLF                                    0x00000003U
#define CKMD_LFCLKSEL_MAIN_LFXT                                     0x00000002U
#define CKMD_LFCLKSEL_MAIN_LFOSC                                    0x00000001U
#define CKMD_LFCLKSEL_MAIN_FAKE                                     0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_TDCCLKSEL
//
//*****************************************************************************
// Field:   [2:0] REFCLK
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// HFXT                     Internal. Only to be used through TI provided API.
// AFOSC                    Internal. Only to be used through TI provided API.
// GPI                      Internal. Only to be used through TI provided API.
// CLKULL                   Internal. Only to be used through TI provided API.
// CLKSVT                   Internal. Only to be used through TI provided API.
// NONE                     Internal. Only to be used through TI provided API.
#define CKMD_TDCCLKSEL_REFCLK_W                                              3U
#define CKMD_TDCCLKSEL_REFCLK_M                                     0x00000007U
#define CKMD_TDCCLKSEL_REFCLK_S                                              0U
#define CKMD_TDCCLKSEL_REFCLK_HFXT                                  0x00000005U
#define CKMD_TDCCLKSEL_REFCLK_AFOSC                                 0x00000004U
#define CKMD_TDCCLKSEL_REFCLK_GPI                                   0x00000003U
#define CKMD_TDCCLKSEL_REFCLK_CLKULL                                0x00000002U
#define CKMD_TDCCLKSEL_REFCLK_CLKSVT                                0x00000001U
#define CKMD_TDCCLKSEL_REFCLK_NONE                                  0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_ADCCLKSEL
//
//*****************************************************************************
// Field:   [1:0] SRC
//
// Select ADC clock source
//
// Change only while ADC is disabled!
// ENUMs:
// HFXT                     48MHz HFXT
// CLKSVT                   48MHz CLKSVT
#define CKMD_ADCCLKSEL_SRC_W                                                 2U
#define CKMD_ADCCLKSEL_SRC_M                                        0x00000003U
#define CKMD_ADCCLKSEL_SRC_S                                                 0U
#define CKMD_ADCCLKSEL_SRC_HFXT                                     0x00000001U
#define CKMD_ADCCLKSEL_SRC_CLKSVT                                   0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_LFCLKSTAT
//
//*****************************************************************************
// Field:    [31] GOOD
//
// Low frequency clock good
//
// Note: This is only a coarse frequency check based on LFQUALCTL. The clock
// may not be accurate enough for timing purposes.
#define CKMD_LFCLKSTAT_GOOD                                         0x80000000U
#define CKMD_LFCLKSTAT_GOOD_M                                       0x80000000U
#define CKMD_LFCLKSTAT_GOOD_S                                               31U

// Field:    [25] FLTSETTLED
//
// LFINC filter is running and settled.
#define CKMD_LFCLKSTAT_FLTSETTLED                                   0x02000000U
#define CKMD_LFCLKSTAT_FLTSETTLED_M                                 0x02000000U
#define CKMD_LFCLKSTAT_FLTSETTLED_S                                         25U

// Field:    [24] LFTICKSRC
//
// Source of LFTICK.
// ENUMs:
// FAKE                     LFTICK generated from CLKULL (LFCLK not available)
// LFCLK                    LFTICK generated from the selected LFCLK
#define CKMD_LFCLKSTAT_LFTICKSRC                                    0x01000000U
#define CKMD_LFCLKSTAT_LFTICKSRC_M                                  0x01000000U
#define CKMD_LFCLKSTAT_LFTICKSRC_S                                          24U
#define CKMD_LFCLKSTAT_LFTICKSRC_FAKE                               0x01000000U
#define CKMD_LFCLKSTAT_LFTICKSRC_LFCLK                              0x00000000U

// Field: [23:22] LFINCSRC
//
// Source of LFINC used by the RTC.
//
// This value depends on LFINCOVR.OVERRIDE, LF clock availability, HF tracking
// loop status and the device state (ACTIVE/STANDBY).
// ENUMs:
// FAKE                     Using FAKE LFTICKs with corresponding LFINC value.
// OVERRIDE                 Using override value from LFINCOVR.LFINC
// AVG                      Using filtered / average value.
//                          This value is updated by
//                          hardware and can be read and updated in
//                          LFINCCTL.INT.
// MEAS                     Using measured value.
//                          This value is updated by
//                          hardware and can be read from LFINC.
#define CKMD_LFCLKSTAT_LFINCSRC_W                                            2U
#define CKMD_LFCLKSTAT_LFINCSRC_M                                   0x00C00000U
#define CKMD_LFCLKSTAT_LFINCSRC_S                                           22U
#define CKMD_LFCLKSTAT_LFINCSRC_FAKE                                0x00C00000U
#define CKMD_LFCLKSTAT_LFINCSRC_OVERRIDE                            0x00800000U
#define CKMD_LFCLKSTAT_LFINCSRC_AVG                                 0x00400000U
#define CKMD_LFCLKSTAT_LFINCSRC_MEAS                                0x00000000U

// Field:  [21:0] LFINC
//
// Measured value of LFINC.
//
// Given in microseconds with 16 fractional bits.
// This value is calculated by Hardware.
// It is the LFCLK period according to CLKULL cycles.
#define CKMD_LFCLKSTAT_LFINC_W                                              22U
#define CKMD_LFCLKSTAT_LFINC_M                                      0x003FFFFFU
#define CKMD_LFCLKSTAT_LFINC_S                                               0U

//*****************************************************************************
//
// Register: CKMD_O_HFXTSTAT
//
//*****************************************************************************
// Field: [30:16] STARTUPTIME
//
// HFXT startup time
//
// Can be used by software to plan starting HFXT ahead in time.
// Measured whenever HFXT is enabled in CLKULL periods (24MHz), from HFXTCTL.EN
// until the clock is good for radio operation (amplitude compensation is
// settled).
#define CKMD_HFXTSTAT_STARTUPTIME_W                                         15U
#define CKMD_HFXTSTAT_STARTUPTIME_M                                 0x7FFF0000U
#define CKMD_HFXTSTAT_STARTUPTIME_S                                         16U

// Field:     [1] FAULT
//
// HFXT clock fault
//
// Indicates a lower than expected HFXT frequency.
// HFXT will not recover from this fault, disabling and re-enabling HFXT is
// required.
#define CKMD_HFXTSTAT_FAULT                                         0x00000002U
#define CKMD_HFXTSTAT_FAULT_M                                       0x00000002U
#define CKMD_HFXTSTAT_FAULT_S                                                1U

// Field:     [0] GOOD
//
// HFXT clock available.
//
// The frequency is not necessarily good enough for radio operation.
#define CKMD_HFXTSTAT_GOOD                                          0x00000001U
#define CKMD_HFXTSTAT_GOOD_M                                        0x00000001U
#define CKMD_HFXTSTAT_GOOD_S                                                 0U

//*****************************************************************************
//
// Register: CKMD_O_AMPADCSTAT
//
//*****************************************************************************
// Field:    [24] COMPOUT
//
// Internal. Only to be used through TI provided API.
#define CKMD_AMPADCSTAT_COMPOUT                                     0x01000000U
#define CKMD_AMPADCSTAT_COMPOUT_M                                   0x01000000U
#define CKMD_AMPADCSTAT_COMPOUT_S                                           24U

// Field: [22:16] PEAKRAW
//
// Internal. Only to be used through TI provided API.
#define CKMD_AMPADCSTAT_PEAKRAW_W                                            7U
#define CKMD_AMPADCSTAT_PEAKRAW_M                                   0x007F0000U
#define CKMD_AMPADCSTAT_PEAKRAW_S                                           16U

// Field:  [15:8] PEAK
//
// Internal. Only to be used through TI provided API.
#define CKMD_AMPADCSTAT_PEAK_W                                               8U
#define CKMD_AMPADCSTAT_PEAK_M                                      0x0000FF00U
#define CKMD_AMPADCSTAT_PEAK_S                                               8U

// Field:   [6:0] BIAS
//
// Internal. Only to be used through TI provided API.
#define CKMD_AMPADCSTAT_BIAS_W                                               7U
#define CKMD_AMPADCSTAT_BIAS_M                                      0x0000007FU
#define CKMD_AMPADCSTAT_BIAS_S                                               0U

//*****************************************************************************
//
// Register: CKMD_O_TRACKSTAT
//
//*****************************************************************************
// Field:    [31] LOOPERRVLD
//
// Current HFOSC tracking error valid
//
// This bit is one if the tracking loop is running and the error value is
// valid.
#define CKMD_TRACKSTAT_LOOPERRVLD                                   0x80000000U
#define CKMD_TRACKSTAT_LOOPERRVLD_M                                 0x80000000U
#define CKMD_TRACKSTAT_LOOPERRVLD_S                                         31U

// Field: [29:16] LOOPERR
//
// Current HFOSC tracking error
//
// This field uses the internal fractional representation (sign, 9 integer
// bits, 4 fractional bits).
#define CKMD_TRACKSTAT_LOOPERR_W                                            14U
#define CKMD_TRACKSTAT_LOOPERR_M                                    0x3FFF0000U
#define CKMD_TRACKSTAT_LOOPERR_S                                            16U

// Field:  [12:0] FINETRIM
//
// Current HFOSC Fine-trim value
//
// This field uses the internal fractional representation (sign, 5 integer
// bits, 7 fractional bits).
// The actual trim value applied to the oscillator is delta-sigma modulated 6
// bits non-signed
// (inverted sign bit + integer bits).
#define CKMD_TRACKSTAT_FINETRIM_W                                           13U
#define CKMD_TRACKSTAT_FINETRIM_M                                   0x00001FFFU
#define CKMD_TRACKSTAT_FINETRIM_S                                            0U

//*****************************************************************************
//
// Register: CKMD_O_AMPSTAT
//
//*****************************************************************************
// Field: [28:25] STATE
//
// Current AMPCOMP FSM state.
// ENUMs:
// SETTLED                  Settled state
// UPDATEUP                 Amplitude up correction
// TXCOMODE                 TCXO settled state
// SHUTDN0                  First shutdown state
// INJWAIT                  Post injection settle wait
// UPDATEDN                 Amplitude down correction
// RAMP0                    Initial amplitude ramping with HFXTINIT values
// RAMP1                    Transition to HFXTTARG values
// INJECT                   Injecting HFOSC for fast startup
// SHUTDN1                  Second shutdown state
// LDOSTART                 Starting LDO
// IDLE                     FSM in idle state
#define CKMD_AMPSTAT_STATE_W                                                 4U
#define CKMD_AMPSTAT_STATE_M                                        0x1E000000U
#define CKMD_AMPSTAT_STATE_S                                                25U
#define CKMD_AMPSTAT_STATE_SETTLED                                  0x1E000000U
#define CKMD_AMPSTAT_STATE_UPDATEUP                                 0x1C000000U
#define CKMD_AMPSTAT_STATE_TXCOMODE                                 0x18000000U
#define CKMD_AMPSTAT_STATE_SHUTDN0                                  0x14000000U
#define CKMD_AMPSTAT_STATE_INJWAIT                                  0x0E000000U
#define CKMD_AMPSTAT_STATE_UPDATEDN                                 0x0C000000U
#define CKMD_AMPSTAT_STATE_RAMP0                                    0x0A000000U
#define CKMD_AMPSTAT_STATE_RAMP1                                    0x08000000U
#define CKMD_AMPSTAT_STATE_INJECT                                   0x06000000U
#define CKMD_AMPSTAT_STATE_SHUTDN1                                  0x04000000U
#define CKMD_AMPSTAT_STATE_LDOSTART                                 0x02000000U
#define CKMD_AMPSTAT_STATE_IDLE                                     0x00000000U

// Field: [24:18] IDAC
//
// Current IDAC control value.
#define CKMD_AMPSTAT_IDAC_W                                                  7U
#define CKMD_AMPSTAT_IDAC_M                                         0x01FC0000U
#define CKMD_AMPSTAT_IDAC_S                                                 18U

// Field: [17:14] IREF
//
// Current IREF control value.
#define CKMD_AMPSTAT_IREF_W                                                  4U
#define CKMD_AMPSTAT_IREF_M                                         0x0003C000U
#define CKMD_AMPSTAT_IREF_S                                                 14U

// Field:  [13:8] Q2CAP
//
// Current Q2CAP control value.
#define CKMD_AMPSTAT_Q2CAP_W                                                 6U
#define CKMD_AMPSTAT_Q2CAP_M                                        0x00003F00U
#define CKMD_AMPSTAT_Q2CAP_S                                                 8U

// Field:   [7:2] Q1CAP
//
// Current Q1CAP control value.
#define CKMD_AMPSTAT_Q1CAP_W                                                 6U
#define CKMD_AMPSTAT_Q1CAP_M                                        0x000000FCU
#define CKMD_AMPSTAT_Q1CAP_S                                                 2U

// Field:     [1] CTRLATTARGET
//
// HFXT control values match target values.
//
// This applies to IREF, Q1CAP, Q2CAP values.
#define CKMD_AMPSTAT_CTRLATTARGET                                   0x00000002U
#define CKMD_AMPSTAT_CTRLATTARGET_M                                 0x00000002U
#define CKMD_AMPSTAT_CTRLATTARGET_S                                          1U

// Field:     [0] AMPGOOD
//
// HFXT amplitude good
#define CKMD_AMPSTAT_AMPGOOD                                        0x00000001U
#define CKMD_AMPSTAT_AMPGOOD_M                                      0x00000001U
#define CKMD_AMPSTAT_AMPGOOD_S                                               0U

//*****************************************************************************
//
// Register: CKMD_O_LFCLKSTAT2
//
//*****************************************************************************
// Field:   [6:4] SUBGEAR
//
// The value of sub gear in LF filter. It counts from 0 to 7.
#define CKMD_LFCLKSTAT2_SUBGEAR_W                                            3U
#define CKMD_LFCLKSTAT2_SUBGEAR_M                                   0x00000070U
#define CKMD_LFCLKSTAT2_SUBGEAR_S                                            4U

// Field:   [3:0] MAINGEAR
//
// The value of main gear in LF filter. The main gear increments when the sub
// gear crosses 7.
#define CKMD_LFCLKSTAT2_MAINGEAR_W                                           4U
#define CKMD_LFCLKSTAT2_MAINGEAR_M                                  0x0000000FU
#define CKMD_LFCLKSTAT2_MAINGEAR_S                                           0U

//*****************************************************************************
//
// Register: CKMD_O_ATBCTL0
//
//*****************************************************************************
// Field:  [23:0] SEL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// AFOSCTESTCLK             Internal. Only to be used through TI provided API.
// LFXTTESTCLK              Internal. Only to be used through TI provided API.
// LFOSCTESTCLK             Internal. Only to be used through TI provided API.
// HFXTTESTCLK              Internal. Only to be used through TI provided API.
// HFOSCTESTCLK             Internal. Only to be used through TI provided API.
// AFOSCOUT                 Internal. Only to be used through TI provided API.
// LFMONVTEST               Internal. Only to be used through TI provided API.
// HFOSCOUT                 Internal. Only to be used through TI provided API.
// NABIASITEST              Internal. Only to be used through TI provided API.
// ADCDACOUT                Internal. Only to be used through TI provided API.
// ADCCOMPIN                Internal. Only to be used through TI provided API.
// ADCCOMPOUT               Internal. Only to be used through TI provided API.
// LFXTANA                  Internal. Only to be used through TI provided API.
// VDDCKM                   Internal. Only to be used through TI provided API.
// HFXTANA                  Internal. Only to be used through TI provided API.
// OFF                      Internal. Only to be used through TI provided API.
#define CKMD_ATBCTL0_SEL_W                                                  24U
#define CKMD_ATBCTL0_SEL_M                                          0x00FFFFFFU
#define CKMD_ATBCTL0_SEL_S                                                   0U
#define CKMD_ATBCTL0_SEL_AFOSCTESTCLK                               0x00090000U
#define CKMD_ATBCTL0_SEL_LFXTTESTCLK                                0x00070000U
#define CKMD_ATBCTL0_SEL_LFOSCTESTCLK                               0x00050000U
#define CKMD_ATBCTL0_SEL_HFXTTESTCLK                                0x00030000U
#define CKMD_ATBCTL0_SEL_HFOSCTESTCLK                               0x00010000U
#define CKMD_ATBCTL0_SEL_AFOSCOUT                                   0x00004000U
#define CKMD_ATBCTL0_SEL_LFMONVTEST                                 0x00001000U
#define CKMD_ATBCTL0_SEL_HFOSCOUT                                   0x00000100U
#define CKMD_ATBCTL0_SEL_NABIASITEST                                0x00000080U
#define CKMD_ATBCTL0_SEL_ADCDACOUT                                  0x00000040U
#define CKMD_ATBCTL0_SEL_ADCCOMPIN                                  0x00000020U
#define CKMD_ATBCTL0_SEL_ADCCOMPOUT                                 0x00000010U
#define CKMD_ATBCTL0_SEL_LFXTANA                                    0x00000008U
#define CKMD_ATBCTL0_SEL_VDDCKM                                     0x00000002U
#define CKMD_ATBCTL0_SEL_HFXTANA                                    0x00000001U
#define CKMD_ATBCTL0_SEL_OFF                                        0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_ATBCTL1
//
//*****************************************************************************
// Field:    [18] BGAP
//
// Internal. Only to be used through TI provided API.
#define CKMD_ATBCTL1_BGAP                                           0x00040000U
#define CKMD_ATBCTL1_BGAP_M                                         0x00040000U
#define CKMD_ATBCTL1_BGAP_S                                                 18U

// Field: [17:15] AFOSC
//
// Internal. Only to be used through TI provided API.
#define CKMD_ATBCTL1_AFOSC_W                                                 3U
#define CKMD_ATBCTL1_AFOSC_M                                        0x00038000U
#define CKMD_ATBCTL1_AFOSC_S                                                15U

// Field: [14:13] LFOSC
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// BOTH                     Internal. Only to be used through TI provided API.
// VDDLOCAL                 Internal. Only to be used through TI provided API.
// TESTCLK                  Internal. Only to be used through TI provided API.
// OFF                      Internal. Only to be used through TI provided API.
#define CKMD_ATBCTL1_LFOSC_W                                                 2U
#define CKMD_ATBCTL1_LFOSC_M                                        0x00006000U
#define CKMD_ATBCTL1_LFOSC_S                                                13U
#define CKMD_ATBCTL1_LFOSC_BOTH                                     0x00006000U
#define CKMD_ATBCTL1_LFOSC_VDDLOCAL                                 0x00004000U
#define CKMD_ATBCTL1_LFOSC_TESTCLK                                  0x00002000U
#define CKMD_ATBCTL1_LFOSC_OFF                                      0x00000000U

// Field:    [12] NABIAS
//
// Internal. Only to be used through TI provided API.
#define CKMD_ATBCTL1_NABIAS                                         0x00001000U
#define CKMD_ATBCTL1_NABIAS_M                                       0x00001000U
#define CKMD_ATBCTL1_NABIAS_S                                               12U

// Field:    [10] LFXT
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// TESTCLK                  Internal. Only to be used through TI provided API.
// OFF                      Internal. Only to be used through TI provided API.
#define CKMD_ATBCTL1_LFXT                                           0x00000400U
#define CKMD_ATBCTL1_LFXT_M                                         0x00000400U
#define CKMD_ATBCTL1_LFXT_S                                                 10U
#define CKMD_ATBCTL1_LFXT_TESTCLK                                   0x00000400U
#define CKMD_ATBCTL1_LFXT_OFF                                       0x00000000U

// Field:   [9:8] LFMON
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// TEST2                    Internal. Only to be used through TI provided API.
// TEST1                    Internal. Only to be used through TI provided API.
// OFF                      Internal. Only to be used through TI provided API.
#define CKMD_ATBCTL1_LFMON_W                                                 2U
#define CKMD_ATBCTL1_LFMON_M                                        0x00000300U
#define CKMD_ATBCTL1_LFMON_S                                                 8U
#define CKMD_ATBCTL1_LFMON_TEST2                                    0x00000200U
#define CKMD_ATBCTL1_LFMON_TEST1                                    0x00000100U
#define CKMD_ATBCTL1_LFMON_OFF                                      0x00000000U

// Field:     [7] HFXT
//
// Internal. Only to be used through TI provided API.
#define CKMD_ATBCTL1_HFXT                                           0x00000080U
#define CKMD_ATBCTL1_HFXT_M                                         0x00000080U
#define CKMD_ATBCTL1_HFXT_S                                                  7U

// Field:   [2:0] HFOSC
//
// Internal. Only to be used through TI provided API.
#define CKMD_ATBCTL1_HFOSC_W                                                 3U
#define CKMD_ATBCTL1_HFOSC_M                                        0x00000007U
#define CKMD_ATBCTL1_HFOSC_S                                                 0U

//*****************************************************************************
//
// Register: CKMD_O_DTBCTL
//
//*****************************************************************************
// Field: [22:18] DSEL2
//
// Internal. Only to be used through TI provided API.
#define CKMD_DTBCTL_DSEL2_W                                                  5U
#define CKMD_DTBCTL_DSEL2_M                                         0x007C0000U
#define CKMD_DTBCTL_DSEL2_S                                                 18U

// Field: [17:13] DSEL1
//
// Internal. Only to be used through TI provided API.
#define CKMD_DTBCTL_DSEL1_W                                                  5U
#define CKMD_DTBCTL_DSEL1_M                                         0x0003E000U
#define CKMD_DTBCTL_DSEL1_S                                                 13U

// Field:  [12:8] DSEL0
//
// Internal. Only to be used through TI provided API.
#define CKMD_DTBCTL_DSEL0_W                                                  5U
#define CKMD_DTBCTL_DSEL0_M                                         0x00001F00U
#define CKMD_DTBCTL_DSEL0_S                                                  8U

// Field:   [7:3] CLKSEL
//
// Select clock to output on DTB[0]
// ENUMs:
// HFOSC_DIV2               HFOSC div by 2 clock
// AFOSC                    Select AFOSC after qualification
// LFXT                     Select LFXT
// LFOSC                    Select LFOSC
// HFXT                     Select HFXT
// HFXTBY8                  Select HFXT divided by 8
// HFOSC                    Select HFOSC after qualification
// LFCLK                    Select LFCLK (selected by LFCLKSEL.MAIN)
// TRACKREF                 Select tracking loop reference clock
// CLKADC                   Select CLKADC (48 MHz)
// CLKSVT                   Select CLKSVT (48 MHz)
// CLKULL                   Select CLKULL (24 MHz during ACTIVE, 32kHz during
//                          STANDBY)
#define CKMD_DTBCTL_CLKSEL_W                                                 5U
#define CKMD_DTBCTL_CLKSEL_M                                        0x000000F8U
#define CKMD_DTBCTL_CLKSEL_S                                                 3U
#define CKMD_DTBCTL_CLKSEL_HFOSC_DIV2                               0x00000090U
#define CKMD_DTBCTL_CLKSEL_AFOSC                                    0x00000088U
#define CKMD_DTBCTL_CLKSEL_LFXT                                     0x00000078U
#define CKMD_DTBCTL_CLKSEL_LFOSC                                    0x00000070U
#define CKMD_DTBCTL_CLKSEL_HFXT                                     0x00000068U
#define CKMD_DTBCTL_CLKSEL_HFXTBY8                                  0x00000060U
#define CKMD_DTBCTL_CLKSEL_HFOSC                                    0x00000050U
#define CKMD_DTBCTL_CLKSEL_LFCLK                                    0x00000038U
#define CKMD_DTBCTL_CLKSEL_TRACKREF                                 0x00000020U
#define CKMD_DTBCTL_CLKSEL_CLKADC                                   0x00000010U
#define CKMD_DTBCTL_CLKSEL_CLKSVT                                   0x00000008U
#define CKMD_DTBCTL_CLKSEL_CLKULL                                   0x00000000U

// Field:     [0] EN
//
// Enable DTB output
#define CKMD_DTBCTL_EN                                              0x00000001U
#define CKMD_DTBCTL_EN_M                                            0x00000001U
#define CKMD_DTBCTL_EN_S                                                     0U

//*****************************************************************************
//
// Register: CKMD_O_DTBCTL2
//
//*****************************************************************************
// Field: [13:10] CLK2DTBSEL
//
// Select a DTB other than DTB0 to route the clock. Value of 0 indicates that
// clock 2 won't be sent to DTB.
#define CKMD_DTBCTL2_CLK2DTBSEL_W                                            4U
#define CKMD_DTBCTL2_CLK2DTBSEL_M                                   0x00003C00U
#define CKMD_DTBCTL2_CLK2DTBSEL_S                                           10U

// Field:   [7:5] CLKSEL2
//
// Select the clock that needs to be routed to a DTB other than DTB0
// ENUMs:
// CLKSVT                   Select CLKSVT (48 MHz)
// CLKULL                   Select CLKULL (24 MHz during ACTIVE, 32kHz during
//                          STANDBY)
#define CKMD_DTBCTL2_CLKSEL2_W                                               3U
#define CKMD_DTBCTL2_CLKSEL2_M                                      0x000000E0U
#define CKMD_DTBCTL2_CLKSEL2_S                                               5U
#define CKMD_DTBCTL2_CLKSEL2_CLKSVT                                 0x00000020U
#define CKMD_DTBCTL2_CLKSEL2_CLKULL                                 0x00000000U

// Field:   [2:1] CLK2DIVVAL
//
// These bits are used to configure the  divider value.
// ENUMs:
// DIVBY16                  Divide by 16
// DIVBY8                   Divide by 8
// DIVBY4                   Divide by 4
// DIVBY2                   Divide by 2
#define CKMD_DTBCTL2_CLK2DIVVAL_W                                            2U
#define CKMD_DTBCTL2_CLK2DIVVAL_M                                   0x00000006U
#define CKMD_DTBCTL2_CLK2DIVVAL_S                                            1U
#define CKMD_DTBCTL2_CLK2DIVVAL_DIVBY16                             0x00000006U
#define CKMD_DTBCTL2_CLK2DIVVAL_DIVBY8                              0x00000004U
#define CKMD_DTBCTL2_CLK2DIVVAL_DIVBY4                              0x00000002U
#define CKMD_DTBCTL2_CLK2DIVVAL_DIVBY2                              0x00000000U

// Field:     [0] CLK2DIVEN
//
// Enable divider on second clock path
// ENUMs:
// EN                       Enable
// DIS                      Disable
#define CKMD_DTBCTL2_CLK2DIVEN                                      0x00000001U
#define CKMD_DTBCTL2_CLK2DIVEN_M                                    0x00000001U
#define CKMD_DTBCTL2_CLK2DIVEN_S                                             0U
#define CKMD_DTBCTL2_CLK2DIVEN_EN                                   0x00000001U
#define CKMD_DTBCTL2_CLK2DIVEN_DIS                                  0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_TRIM0
//
//*****************************************************************************
// Field:    [25] AFOSC_MODE
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM0_AFOSC_MODE                                       0x02000000U
#define CKMD_TRIM0_AFOSC_MODE_M                                     0x02000000U
#define CKMD_TRIM0_AFOSC_MODE_S                                             25U

// Field: [24:21] AFOSC_MID
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM0_AFOSC_MID_W                                               4U
#define CKMD_TRIM0_AFOSC_MID_M                                      0x01E00000U
#define CKMD_TRIM0_AFOSC_MID_S                                              21U

// Field: [20:16] AFOSC_COARSE
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM0_AFOSC_COARSE_W                                            5U
#define CKMD_TRIM0_AFOSC_COARSE_M                                   0x001F0000U
#define CKMD_TRIM0_AFOSC_COARSE_S                                           16U

// Field:     [9] HFOSC_MODE
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM0_HFOSC_MODE                                       0x00000200U
#define CKMD_TRIM0_HFOSC_MODE_M                                     0x00000200U
#define CKMD_TRIM0_HFOSC_MODE_S                                              9U

// Field:   [8:5] HFOSC_MID
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM0_HFOSC_MID_W                                               4U
#define CKMD_TRIM0_HFOSC_MID_M                                      0x000001E0U
#define CKMD_TRIM0_HFOSC_MID_S                                               5U

// Field:   [4:0] HFOSC_COARSE
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM0_HFOSC_COARSE_W                                            5U
#define CKMD_TRIM0_HFOSC_COARSE_M                                   0x0000001FU
#define CKMD_TRIM0_HFOSC_COARSE_S                                            0U

//*****************************************************************************
//
// Register: CKMD_O_TRIM1
//
//*****************************************************************************
// Field: [31:30] HFXTSLICER
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM1_HFXTSLICER_W                                              2U
#define CKMD_TRIM1_HFXTSLICER_M                                     0xC0000000U
#define CKMD_TRIM1_HFXTSLICER_S                                             30U

// Field: [29:28] PEAKIBIAS
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM1_PEAKIBIAS_W                                               2U
#define CKMD_TRIM1_PEAKIBIAS_M                                      0x30000000U
#define CKMD_TRIM1_PEAKIBIAS_S                                              28U

// Field:    [27] NABIAS_UDIGLDO
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM1_NABIAS_UDIGLDO                                   0x08000000U
#define CKMD_TRIM1_NABIAS_UDIGLDO_M                                 0x08000000U
#define CKMD_TRIM1_NABIAS_UDIGLDO_S                                         27U

// Field: [26:24] LDOBW
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM1_LDOBW_W                                                   3U
#define CKMD_TRIM1_LDOBW_M                                          0x07000000U
#define CKMD_TRIM1_LDOBW_S                                                  24U

// Field: [23:20] LDOFB
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM1_LDOFB_W                                                   4U
#define CKMD_TRIM1_LDOFB_M                                          0x00F00000U
#define CKMD_TRIM1_LDOFB_S                                                  20U

// Field: [19:16] LFDLY
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM1_LFDLY_W                                                   4U
#define CKMD_TRIM1_LFDLY_M                                          0x000F0000U
#define CKMD_TRIM1_LFDLY_S                                                  16U

// Field:    [15] NABIAS_LFOSC
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM1_NABIAS_LFOSC                                     0x00008000U
#define CKMD_TRIM1_NABIAS_LFOSC_M                                   0x00008000U
#define CKMD_TRIM1_NABIAS_LFOSC_S                                           15U

// Field:  [14:8] NABIAS_RES
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM1_NABIAS_RES_W                                              7U
#define CKMD_TRIM1_NABIAS_RES_M                                     0x00007F00U
#define CKMD_TRIM1_NABIAS_RES_S                                              8U

// Field:   [7:0] LFOSC_CAP
//
// Internal. Only to be used through TI provided API.
#define CKMD_TRIM1_LFOSC_CAP_W                                               8U
#define CKMD_TRIM1_LFOSC_CAP_M                                      0x000000FFU
#define CKMD_TRIM1_LFOSC_CAP_S                                               0U

//*****************************************************************************
//
// Register: CKMD_O_HFXTINIT
//
//*****************************************************************************
// Field: [29:23] AMPTHR
//
// Amplitude threshold during HFXT ramping
#define CKMD_HFXTINIT_AMPTHR_W                                               7U
#define CKMD_HFXTINIT_AMPTHR_M                                      0x3F800000U
#define CKMD_HFXTINIT_AMPTHR_S                                              23U

// Field: [22:16] IDAC
//
// Initial HFXT IDAC current
#define CKMD_HFXTINIT_IDAC_W                                                 7U
#define CKMD_HFXTINIT_IDAC_M                                        0x007F0000U
#define CKMD_HFXTINIT_IDAC_S                                                16U

// Field: [15:12] IREF
//
// Initial HFXT IREF current
#define CKMD_HFXTINIT_IREF_W                                                 4U
#define CKMD_HFXTINIT_IREF_M                                        0x0000F000U
#define CKMD_HFXTINIT_IREF_S                                                12U

// Field:  [11:6] Q2CAP
//
// Initial HFXT Q2 cap trim
// ENUMs:
// CAP63                    Nominal 25C	=	1.10E-11 F
//                          Stong 25C		=	1.00E-11 F
//                          Weak 25C		=	1.19E-11 F
// CAP62                    Nominal 25C	=	1.08E-11 F
//                          Stong 25C		=	9.82E-12 F
//                          Weak 25C		=	1.17E-11 F
// CAP61                    Nominal 25C	=	1.06E-11 F
//                          Stong 25C		=	9.64E-12 F
//                          Weak 25C		=	1.15E-11 F
// CAP60                    Nominal 25C	=	1.04E-11 F
//                          Stong 25C		=	9.45E-12 F
//                          Weak 25C		=	1.13E-11 F
// CAP59                    Nominal 25C	=	1.02E-11 F
//                          Stong 25C		=	9.26E-12 F
//                          Weak 25C		=	1.11E-11 F
// CAP58                    Nominal 25C	=	1.00E-11 F
//                          Stong 25C		=	9.07E-12 F
//                          Weak 25C		=	1.09E-11 F
// CAP57                    Nominal 25C	=	9.84E-12 F
//                          Stong 25C		=	8.89E-12 F
//                          Weak 25C		=	1.07E-11 F
// CAP56                    Nominal 25C	=	9.65E-12 F
//                          Stong 25C		=	8.70E-12 F
//                          Weak 25C		=	1.04E-11 F
// CAP55                    Nominal 25C	=	9.46E-12 F
//                          Stong 25C		=	8.51E-12 F
//                          Weak 25C		=	1.02E-11 F
// CAP54                    Nominal 25C	=	9.26E-12 F
//                          Stong 25C		=	8.32E-12 F
//                          Weak 25C		=	1.00E-11 F
// CAP53                    Nominal 25C	=	9.07E-12 F
//                          Stong 25C		=	8.13E-12 F
//                          Weak 25C		=	9.81E-12 F
// CAP52                    Nominal 25C	=	8.88E-12 F
//                          Stong 25C		=	7.94E-12 F
//                          Weak 25C		=	9.60E-12 F
// CAP51                    Nominal 25C	=	8.69E-12 F
//                          Stong 25C		=	7.76E-12 F
//                          Weak 25C		=	9.39E-12 F
// CAP50                    Nominal 25C	=	8.49E-12 F
//                          Stong 25C		=	7.57E-12 F
//                          Weak 25C		=	9.18E-12 F
// CAP49                    Nominal 25C	=	8.30E-12 F
//                          Stong 25C		=	7.38E-12 F
//                          Weak 25C		=	8.96E-12 F
// CAP48                    Nominal 25C	=	8.10E-12 F
//                          Stong 25C		=	7.18E-12 F
//                          Weak 25C		=	8.75E-12 F
// CAP47                    Nominal 25C	=	8.10E-12 F
//                          Stong 25C		=	7.18E-12 F
//                          Weak 25C		=	8.75E-12 F
// CAP46                    Nominal 25C	=	7.97E-12 F
//                          Stong 25C		=	7.05E-12 F
//                          Weak 25C		=	8.60E-12 F
// CAP45                    Nominal 25C	=	7.83E-12 F
//                          Stong 25C		=	6.91E-12 F
//                          Weak 25C		=	8.44E-12 F
// CAP44                    Nominal 25C	=	7.69E-12 F
//                          Stong 25C		=	6.77E-12 F
//                          Weak 25C		=	8.29E-12 F
// CAP43                    Nominal 25C	=	7.55E-12 F
//                          Stong 25C		=	6.63E-12 F
//                          Weak 25C		=	8.13E-12 F
// CAP42                    Nominal 25C	=	7.40E-12 F
//                          Stong 25C		=	6.49E-12 F
//                          Weak 25C		=	7.98E-12 F
// CAP41                    Nominal 25C	=	7.26E-12 F
//                          Stong 25C		=	6.35E-12 F
//                          Weak 25C		=	7.82E-12 F
// CAP40                    Nominal 25C	=	7.12E-12 F
//                          Stong 25C		=	6.21E-12 F
//                          Weak 25C		=	7.67E-12 F
// CAP39                    Nominal 25C	=	6.98E-12 F
//                          Stong 25C		=	6.06E-12 F
//                          Weak 25C		=	7.51E-12 F
// CAP38                    Nominal 25C	=	6.84E-12 F
//                          Stong 25C		=	5.92E-12 F
//                          Weak 25C		=	7.35E-12 F
// CAP37                    Nominal 25C	=	6.69E-12 F
//                          Stong 25C		=	5.78E-12 F
//                          Weak 25C		=	7.20E-12 F
// CAP36                    Nominal 25C	=	6.55E-12 F
//                          Stong 25C		=	5.63E-12 F
//                          Weak 25C		=	7.04E-12 F
// CAP35                    Nominal 25C	=	6.41E-12 F
//                          Stong 25C		=	5.49E-12 F
//                          Weak 25C		=	6.88E-12 F
// CAP34                    Nominal 25C	=	6.26E-12 F
//                          Stong 25C		=	5.35E-12 F
//                          Weak 25C		=	6.72E-12 F
// CAP33                    Nominal 25C	=	6.12E-12 F
//                          Stong 25C		=	5.20E-12 F
//                          Weak 25C		=	6.56E-12 F
// CAP32                    Nominal 25C	=	5.97E-12 F
//                          Stong 25C		=	5.05E-12 F
//                          Weak 25C		=	6.40E-12 F
// CAP31                    Nominal 25C	=	5.72E-12 F
//                          Stong 25C		=	4.79E-12 F
//                          Weak 25C		=	6.12E-12 F
// CAP30                    Nominal 25C	=	5.60E-12 F
//                          Stong 25C		=	4.67E-12 F
//                          Weak 25C		=	5.99E-12 F
// CAP29                    Nominal 25C	=	5.49E-12 F
//                          Stong 25C		=	4.56E-12 F
//                          Weak 25C		=	5.87E-12 F
// CAP28                    Nominal 25C	=	5.37E-12 F
//                          Stong 25C		=	4.44E-12 F
//                          Weak 25C		=	5.74E-12 F
// CAP27                    Nominal 25C	=	5.26E-12 F
//                          Stong 25C		=	4.32E-12 F
//                          Weak 25C		=	5.61E-12 F
// CAP26                    Nominal 25C	=	5.15E-12 F
//                          Stong 25C		=	4.21E-12 F
//                          Weak 25C		=	5.49E-12 F
// CAP25                    Nominal 25C	=	5.03E-12 F
//                          Stong 25C		=	4.09E-12 F
//                          Weak 25C		=	5.36E-12 F
// CAP24                    Nominal 25C	=	4.91E-12 F
//                          Stong 25C		=	3.97E-12 F
//                          Weak 25C		=	5.23E-12 F
// CAP23                    Nominal 25C	=	4.80E-12 F
//                          Stong 25C		=	3.85E-12 F
//                          Weak 25C		=	5.10E-12 F
// CAP22                    Nominal 25C	=	4.68E-12 F
//                          Stong 25C		=	3.73E-12 F
//                          Weak 25C		=	4.98E-12 F
// CAP21                    Nominal 25C	=	4.57E-12 F
//                          Stong 25C		=	3.60E-12 F
//                          Weak 25C		=	4.85E-12 F
// CAP20                    Nominal 25C	=	4.45E-12 F
//                          Stong 25C		=	3.48E-12 F
//                          Weak 25C		=	4.72E-12 F
// CAP19                    Nominal 25C	=	4.33E-12 F
//                          Stong 25C		=	3.36E-12 F
//                          Weak 25C		=	4.59E-12 F
// CAP18                    Nominal 25C	=	4.21E-12 F
//                          Stong 25C		=	3.23E-12 F
//                          Weak 25C		=	4.46E-12 F
// CAP17                    Nominal 25C	=	4.09E-12 F
//                          Stong 25C		=	3.11E-12 F
//                          Weak 25C		=	4.33E-12 F
// CAP16                    Nominal 25C	=	3.97E-12 F
//                          Stong 25C		=	2.98E-12 F
//                          Weak 25C		=	4.20E-12 F
// CAP15                    Nominal 25C	=	3.97E-12 F
//                          Stong 25C		=	2.98E-12 F
//                          Weak 25C		=	4.20E-12 F
// CAP14                    Nominal 25C	=	3.88E-12 F
//                          Stong 25C		=	2.89E-12 F
//                          Weak 25C		=	4.10E-12 F
// CAP13                    Nominal 25C	=	3.79E-12 F
//                          Stong 25C		=	2.79E-12 F
//                          Weak 25C		=	4.00E-12 F
// CAP12                    Nominal 25C	=	3.70E-12 F
//                          Stong 25C		=	2.69E-12 F
//                          Weak 25C		=	3.90E-12 F
// CAP11                    Nominal 25C	=	3.61E-12 F
//                          Stong 25C		=	2.59E-12 F
//                          Weak 25C		=	3.80E-12 F
// CAP10                    Nominal 25C	=	3.51E-12 F
//                          Stong 25C		=	2.49E-12 F
//                          Weak 25C		=	3.70E-12 F
// CAP9                     Nominal 25C	=	3.42E-12 F
//                          Stong 25C		=	2.39E-12 F
//                          Weak 25C		=	3.60E-12 F
// CAP8                     Nominal 25C	=	3.33E-12 F
//                          Stong 25C		=	2.29E-12 F
//                          Weak 25C		=	3.50E-12 F
// CAP7                     Nominal 25C	=	3.23E-12 F
//                          Stong 25C		=	2.19E-12 F
//                          Weak 25C		=	3.40E-12 F
// CAP6                     Nominal 25C	=	3.14E-12 F
//                          Stong 25C		=	2.09E-12 F
//                          Weak 25C		=	3.30E-12 F
// CAP5                     Nominal 25C	=	3.04E-12 F
//                          Stong 25C		=	1.99E-12 F
//                          Weak 25C		=	3.19E-12 F
// CAP4                     Nominal 25C	=	2.95E-12 F
//                          Stong 25C		=	1.89E-12 F
//                          Weak 25C		=	3.09E-12 F
// CAP3                     Nominal 25C	=	2.85E-12 F
//                          Stong 25C		=	1.79E-12 F
//                          Weak 25C		=	2.99E-12 F
// CAP2                     Nominal 25C	=	2.76E-12 F
//                          Stong 25C		=	1.69E-12 F
//                          Weak 25C		=	2.89E-12 F
// CAP1                     Nominal 25C	=	2.66E-12 F
//                          Stong 25C		=	1.60E-12 F
//                          Weak 25C		=	2.79E-12 F
// CAP0                     Nominal 25C	=	2.57E-12 F
//                          Stong 25C		=	1.50E-12 F
//                          Weak 25C		=	2.69E-12 F
#define CKMD_HFXTINIT_Q2CAP_W                                                6U
#define CKMD_HFXTINIT_Q2CAP_M                                       0x00000FC0U
#define CKMD_HFXTINIT_Q2CAP_S                                                6U
#define CKMD_HFXTINIT_Q2CAP_CAP63                                   0x00000FC0U
#define CKMD_HFXTINIT_Q2CAP_CAP62                                   0x00000F80U
#define CKMD_HFXTINIT_Q2CAP_CAP61                                   0x00000F40U
#define CKMD_HFXTINIT_Q2CAP_CAP60                                   0x00000F00U
#define CKMD_HFXTINIT_Q2CAP_CAP59                                   0x00000EC0U
#define CKMD_HFXTINIT_Q2CAP_CAP58                                   0x00000E80U
#define CKMD_HFXTINIT_Q2CAP_CAP57                                   0x00000E40U
#define CKMD_HFXTINIT_Q2CAP_CAP56                                   0x00000E00U
#define CKMD_HFXTINIT_Q2CAP_CAP55                                   0x00000DC0U
#define CKMD_HFXTINIT_Q2CAP_CAP54                                   0x00000D80U
#define CKMD_HFXTINIT_Q2CAP_CAP53                                   0x00000D40U
#define CKMD_HFXTINIT_Q2CAP_CAP52                                   0x00000D00U
#define CKMD_HFXTINIT_Q2CAP_CAP51                                   0x00000CC0U
#define CKMD_HFXTINIT_Q2CAP_CAP50                                   0x00000C80U
#define CKMD_HFXTINIT_Q2CAP_CAP49                                   0x00000C40U
#define CKMD_HFXTINIT_Q2CAP_CAP48                                   0x00000C00U
#define CKMD_HFXTINIT_Q2CAP_CAP47                                   0x00000BC0U
#define CKMD_HFXTINIT_Q2CAP_CAP46                                   0x00000B80U
#define CKMD_HFXTINIT_Q2CAP_CAP45                                   0x00000B40U
#define CKMD_HFXTINIT_Q2CAP_CAP44                                   0x00000B00U
#define CKMD_HFXTINIT_Q2CAP_CAP43                                   0x00000AC0U
#define CKMD_HFXTINIT_Q2CAP_CAP42                                   0x00000A80U
#define CKMD_HFXTINIT_Q2CAP_CAP41                                   0x00000A40U
#define CKMD_HFXTINIT_Q2CAP_CAP40                                   0x00000A00U
#define CKMD_HFXTINIT_Q2CAP_CAP39                                   0x000009C0U
#define CKMD_HFXTINIT_Q2CAP_CAP38                                   0x00000980U
#define CKMD_HFXTINIT_Q2CAP_CAP37                                   0x00000940U
#define CKMD_HFXTINIT_Q2CAP_CAP36                                   0x00000900U
#define CKMD_HFXTINIT_Q2CAP_CAP35                                   0x000008C0U
#define CKMD_HFXTINIT_Q2CAP_CAP34                                   0x00000880U
#define CKMD_HFXTINIT_Q2CAP_CAP33                                   0x00000840U
#define CKMD_HFXTINIT_Q2CAP_CAP32                                   0x00000800U
#define CKMD_HFXTINIT_Q2CAP_CAP31                                   0x000007C0U
#define CKMD_HFXTINIT_Q2CAP_CAP30                                   0x00000780U
#define CKMD_HFXTINIT_Q2CAP_CAP29                                   0x00000740U
#define CKMD_HFXTINIT_Q2CAP_CAP28                                   0x00000700U
#define CKMD_HFXTINIT_Q2CAP_CAP27                                   0x000006C0U
#define CKMD_HFXTINIT_Q2CAP_CAP26                                   0x00000680U
#define CKMD_HFXTINIT_Q2CAP_CAP25                                   0x00000640U
#define CKMD_HFXTINIT_Q2CAP_CAP24                                   0x00000600U
#define CKMD_HFXTINIT_Q2CAP_CAP23                                   0x000005C0U
#define CKMD_HFXTINIT_Q2CAP_CAP22                                   0x00000580U
#define CKMD_HFXTINIT_Q2CAP_CAP21                                   0x00000540U
#define CKMD_HFXTINIT_Q2CAP_CAP20                                   0x00000500U
#define CKMD_HFXTINIT_Q2CAP_CAP19                                   0x000004C0U
#define CKMD_HFXTINIT_Q2CAP_CAP18                                   0x00000480U
#define CKMD_HFXTINIT_Q2CAP_CAP17                                   0x00000440U
#define CKMD_HFXTINIT_Q2CAP_CAP16                                   0x00000400U
#define CKMD_HFXTINIT_Q2CAP_CAP15                                   0x000003C0U
#define CKMD_HFXTINIT_Q2CAP_CAP14                                   0x00000380U
#define CKMD_HFXTINIT_Q2CAP_CAP13                                   0x00000340U
#define CKMD_HFXTINIT_Q2CAP_CAP12                                   0x00000300U
#define CKMD_HFXTINIT_Q2CAP_CAP11                                   0x000002C0U
#define CKMD_HFXTINIT_Q2CAP_CAP10                                   0x00000280U
#define CKMD_HFXTINIT_Q2CAP_CAP9                                    0x00000240U
#define CKMD_HFXTINIT_Q2CAP_CAP8                                    0x00000200U
#define CKMD_HFXTINIT_Q2CAP_CAP7                                    0x000001C0U
#define CKMD_HFXTINIT_Q2CAP_CAP6                                    0x00000180U
#define CKMD_HFXTINIT_Q2CAP_CAP5                                    0x00000140U
#define CKMD_HFXTINIT_Q2CAP_CAP4                                    0x00000100U
#define CKMD_HFXTINIT_Q2CAP_CAP3                                    0x000000C0U
#define CKMD_HFXTINIT_Q2CAP_CAP2                                    0x00000080U
#define CKMD_HFXTINIT_Q2CAP_CAP1                                    0x00000040U
#define CKMD_HFXTINIT_Q2CAP_CAP0                                    0x00000000U

// Field:   [5:0] Q1CAP
//
// Initial HFXT Q1 cap trim
// ENUMs:
// CAP63                    Nominal 25C	=	1.10E-11 F
//                          Stong 25C		=	1.00E-11 F
//                          Weak 25C		=	1.19E-11 F
// CAP62                    Nominal 25C	=	1.08E-11 F
//                          Stong 25C		=	9.82E-12 F
//                          Weak 25C		=	1.17E-11 F
// CAP61                    Nominal 25C	=	1.06E-11 F
//                          Stong 25C		=	9.64E-12 F
//                          Weak 25C		=	1.15E-11 F
// CAP60                    Nominal 25C	=	1.04E-11 F
//                          Stong 25C		=	9.45E-12 F
//                          Weak 25C		=	1.13E-11 F
// CAP59                    Nominal 25C	=	1.02E-11 F
//                          Stong 25C		=	9.26E-12 F
//                          Weak 25C		=	1.11E-11 F
// CAP58                    Nominal 25C	=	1.00E-11 F
//                          Stong 25C		=	9.07E-12 F
//                          Weak 25C		=	1.09E-11 F
// CAP57                    Nominal 25C	=	9.84E-12 F
//                          Stong 25C		=	8.89E-12 F
//                          Weak 25C		=	1.07E-11 F
// CAP56                    Nominal 25C	=	9.65E-12 F
//                          Stong 25C		=	8.70E-12 F
//                          Weak 25C		=	1.04E-11 F
// CAP55                    Nominal 25C	=	9.46E-12 F
//                          Stong 25C		=	8.51E-12 F
//                          Weak 25C		=	1.02E-11 F
// CAP54                    Nominal 25C	=	9.26E-12 F
//                          Stong 25C		=	8.32E-12 F
//                          Weak 25C		=	1.00E-11 F
// CAP53                    Nominal 25C	=	9.07E-12 F
//                          Stong 25C		=	8.13E-12 F
//                          Weak 25C		=	9.81E-12 F
// CAP52                    Nominal 25C	=	8.88E-12 F
//                          Stong 25C		=	7.94E-12 F
//                          Weak 25C		=	9.60E-12 F
// CAP51                    Nominal 25C	=	8.69E-12 F
//                          Stong 25C		=	7.76E-12 F
//                          Weak 25C		=	9.39E-12 F
// CAP50                    Nominal 25C	=	8.49E-12 F
//                          Stong 25C		=	7.57E-12 F
//                          Weak 25C		=	9.18E-12 F
// CAP49                    Nominal 25C	=	8.30E-12 F
//                          Stong 25C		=	7.38E-12 F
//                          Weak 25C		=	8.96E-12 F
// CAP48                    Nominal 25C	=	8.10E-12 F
//                          Stong 25C		=	7.18E-12 F
//                          Weak 25C		=	8.75E-12 F
// CAP47                    Nominal 25C	=	8.10E-12 F
//                          Stong 25C		=	7.18E-12 F
//                          Weak 25C		=	8.75E-12 F
// CAP46                    Nominal 25C	=	7.97E-12 F
//                          Stong 25C		=	7.05E-12 F
//                          Weak 25C		=	8.60E-12 F
// CAP45                    Nominal 25C	=	7.83E-12 F
//                          Stong 25C		=	6.91E-12 F
//                          Weak 25C		=	8.44E-12 F
// CAP44                    Nominal 25C	=	7.69E-12 F
//                          Stong 25C		=	6.77E-12 F
//                          Weak 25C		=	8.29E-12 F
// CAP43                    Nominal 25C	=	7.55E-12 F
//                          Stong 25C		=	6.63E-12 F
//                          Weak 25C		=	8.13E-12 F
// CAP42                    Nominal 25C	=	7.40E-12 F
//                          Stong 25C		=	6.49E-12 F
//                          Weak 25C		=	7.98E-12 F
// CAP41                    Nominal 25C	=	7.26E-12 F
//                          Stong 25C		=	6.35E-12 F
//                          Weak 25C		=	7.82E-12 F
// CAP40                    Nominal 25C	=	7.12E-12 F
//                          Stong 25C		=	6.21E-12 F
//                          Weak 25C		=	7.67E-12 F
// CAP39                    Nominal 25C	=	6.98E-12 F
//                          Stong 25C		=	6.06E-12 F
//                          Weak 25C		=	7.51E-12 F
// CAP38                    Nominal 25C	=	6.84E-12 F
//                          Stong 25C		=	5.92E-12 F
//                          Weak 25C		=	7.35E-12 F
// CAP37                    Nominal 25C	=	6.69E-12 F
//                          Stong 25C		=	5.78E-12 F
//                          Weak 25C		=	7.20E-12 F
// CAP36                    Nominal 25C	=	6.55E-12 F
//                          Stong 25C		=	5.63E-12 F
//                          Weak 25C		=	7.04E-12 F
// CAP35                    Nominal 25C	=	6.41E-12 F
//                          Stong 25C		=	5.49E-12 F
//                          Weak 25C		=	6.88E-12 F
// CAP34                    Nominal 25C	=	6.26E-12 F
//                          Stong 25C		=	5.35E-12 F
//                          Weak 25C		=	6.72E-12 F
// CAP33                    Nominal 25C	=	6.12E-12 F
//                          Stong 25C		=	5.20E-12 F
//                          Weak 25C		=	6.56E-12 F
// CAP32                    Nominal 25C	=	5.97E-12 F
//                          Stong 25C		=	5.05E-12 F
//                          Weak 25C		=	6.40E-12 F
// CAP31                    Nominal 25C	=	5.72E-12 F
//                          Stong 25C		=	4.79E-12 F
//                          Weak 25C		=	6.12E-12 F
// CAP30                    Nominal 25C	=	5.60E-12 F
//                          Stong 25C		=	4.67E-12 F
//                          Weak 25C		=	5.99E-12 F
// CAP29                    Nominal 25C	=	5.49E-12 F
//                          Stong 25C		=	4.56E-12 F
//                          Weak 25C		=	5.87E-12 F
// CAP28                    Nominal 25C	=	5.37E-12 F
//                          Stong 25C		=	4.44E-12 F
//                          Weak 25C		=	5.74E-12 F
// CAP27                    Nominal 25C	=	5.26E-12 F
//                          Stong 25C		=	4.32E-12 F
//                          Weak 25C		=	5.61E-12 F
// CAP26                    Nominal 25C	=	5.15E-12 F
//                          Stong 25C		=	4.21E-12 F
//                          Weak 25C		=	5.49E-12 F
// CAP25                    Nominal 25C	=	5.03E-12 F
//                          Stong 25C		=	4.09E-12 F
//                          Weak 25C		=	5.36E-12 F
// CAP24                    Nominal 25C	=	4.91E-12 F
//                          Stong 25C		=	3.97E-12 F
//                          Weak 25C		=	5.23E-12 F
// CAP23                    Nominal 25C	=	4.80E-12 F
//                          Stong 25C		=	3.85E-12 F
//                          Weak 25C		=	5.10E-12 F
// CAP22                    Nominal 25C	=	4.68E-12 F
//                          Stong 25C		=	3.73E-12 F
//                          Weak 25C		=	4.98E-12 F
// CAP21                    Nominal 25C	=	4.57E-12 F
//                          Stong 25C		=	3.60E-12 F
//                          Weak 25C		=	4.85E-12 F
// CAP20                    Nominal 25C	=	4.45E-12 F
//                          Stong 25C		=	3.48E-12 F
//                          Weak 25C		=	4.72E-12 F
// CAP19                    Nominal 25C	=	4.33E-12 F
//                          Stong 25C		=	3.36E-12 F
//                          Weak 25C		=	4.59E-12 F
// CAP18                    Nominal 25C	=	4.21E-12 F
//                          Stong 25C		=	3.23E-12 F
//                          Weak 25C		=	4.46E-12 F
// CAP17                    Nominal 25C	=	4.09E-12 F
//                          Stong 25C		=	3.11E-12 F
//                          Weak 25C		=	4.33E-12 F
// CAP16                    Nominal 25C	=	3.97E-12 F
//                          Stong 25C		=	2.98E-12 F
//                          Weak 25C		=	4.20E-12 F
// CAP15                    Nominal 25C	=	3.97E-12 F
//                          Stong 25C		=	2.98E-12 F
//                          Weak 25C		=	4.20E-12 F
// CAP14                    Nominal 25C	=	3.88E-12 F
//                          Stong 25C		=	2.89E-12 F
//                          Weak 25C		=	4.10E-12 F
// CAP13                    Nominal 25C	=	3.79E-12 F
//                          Stong 25C		=	2.79E-12 F
//                          Weak 25C		=	4.00E-12 F
// CAP12                    Nominal 25C	=	3.70E-12 F
//                          Stong 25C		=	2.69E-12 F
//                          Weak 25C		=	3.90E-12 F
// CAP11                    Nominal 25C	=	3.61E-12 F
//                          Stong 25C		=	2.59E-12 F
//                          Weak 25C		=	3.80E-12 F
// CAP10                    Nominal 25C	=	3.51E-12 F
//                          Stong 25C		=	2.49E-12 F
//                          Weak 25C		=	3.70E-12 F
// CAP9                     Nominal 25C	=	3.42E-12 F
//                          Stong 25C		=	2.39E-12 F
//                          Weak 25C		=	3.60E-12 F
// CAP8                     Nominal 25C	=	3.33E-12 F
//                          Stong 25C		=	2.29E-12 F
//                          Weak 25C		=	3.50E-12 F
// CAP7                     Nominal 25C	=	3.23E-12 F
//                          Stong 25C		=	2.19E-12 F
//                          Weak 25C		=	3.40E-12 F
// CAP6                     Nominal 25C	=	3.14E-12 F
//                          Stong 25C		=	2.09E-12 F
//                          Weak 25C		=	3.30E-12 F
// CAP5                     Nominal 25C	=	3.04E-12 F
//                          Stong 25C		=	1.99E-12 F
//                          Weak 25C		=	3.19E-12 F
// CAP4                     Nominal 25C	=	2.95E-12 F
//                          Stong 25C		=	1.89E-12 F
//                          Weak 25C		=	3.09E-12 F
// CAP3                     Nominal 25C	=	2.85E-12 F
//                          Stong 25C		=	1.79E-12 F
//                          Weak 25C		=	2.99E-12 F
// CAP2                     Nominal 25C	=	2.76E-12 F
//                          Stong 25C		=	1.69E-12 F
//                          Weak 25C		=	2.89E-12 F
// CAP1                     Nominal 25C	=	2.66E-12 F
//                          Stong 25C		=	1.60E-12 F
//                          Weak 25C		=	2.79E-12 F
// CAP0                     Nominal 25C	=	2.57E-12 F
//                          Stong 25C		=	1.50E-12 F
//                          Weak 25C		=	2.69E-12 F
#define CKMD_HFXTINIT_Q1CAP_W                                                6U
#define CKMD_HFXTINIT_Q1CAP_M                                       0x0000003FU
#define CKMD_HFXTINIT_Q1CAP_S                                                0U
#define CKMD_HFXTINIT_Q1CAP_CAP63                                   0x0000003FU
#define CKMD_HFXTINIT_Q1CAP_CAP62                                   0x0000003EU
#define CKMD_HFXTINIT_Q1CAP_CAP61                                   0x0000003DU
#define CKMD_HFXTINIT_Q1CAP_CAP60                                   0x0000003CU
#define CKMD_HFXTINIT_Q1CAP_CAP59                                   0x0000003BU
#define CKMD_HFXTINIT_Q1CAP_CAP58                                   0x0000003AU
#define CKMD_HFXTINIT_Q1CAP_CAP57                                   0x00000039U
#define CKMD_HFXTINIT_Q1CAP_CAP56                                   0x00000038U
#define CKMD_HFXTINIT_Q1CAP_CAP55                                   0x00000037U
#define CKMD_HFXTINIT_Q1CAP_CAP54                                   0x00000036U
#define CKMD_HFXTINIT_Q1CAP_CAP53                                   0x00000035U
#define CKMD_HFXTINIT_Q1CAP_CAP52                                   0x00000034U
#define CKMD_HFXTINIT_Q1CAP_CAP51                                   0x00000033U
#define CKMD_HFXTINIT_Q1CAP_CAP50                                   0x00000032U
#define CKMD_HFXTINIT_Q1CAP_CAP49                                   0x00000031U
#define CKMD_HFXTINIT_Q1CAP_CAP48                                   0x00000030U
#define CKMD_HFXTINIT_Q1CAP_CAP47                                   0x0000002FU
#define CKMD_HFXTINIT_Q1CAP_CAP46                                   0x0000002EU
#define CKMD_HFXTINIT_Q1CAP_CAP45                                   0x0000002DU
#define CKMD_HFXTINIT_Q1CAP_CAP44                                   0x0000002CU
#define CKMD_HFXTINIT_Q1CAP_CAP43                                   0x0000002BU
#define CKMD_HFXTINIT_Q1CAP_CAP42                                   0x0000002AU
#define CKMD_HFXTINIT_Q1CAP_CAP41                                   0x00000029U
#define CKMD_HFXTINIT_Q1CAP_CAP40                                   0x00000028U
#define CKMD_HFXTINIT_Q1CAP_CAP39                                   0x00000027U
#define CKMD_HFXTINIT_Q1CAP_CAP38                                   0x00000026U
#define CKMD_HFXTINIT_Q1CAP_CAP37                                   0x00000025U
#define CKMD_HFXTINIT_Q1CAP_CAP36                                   0x00000024U
#define CKMD_HFXTINIT_Q1CAP_CAP35                                   0x00000023U
#define CKMD_HFXTINIT_Q1CAP_CAP34                                   0x00000022U
#define CKMD_HFXTINIT_Q1CAP_CAP33                                   0x00000021U
#define CKMD_HFXTINIT_Q1CAP_CAP32                                   0x00000020U
#define CKMD_HFXTINIT_Q1CAP_CAP31                                   0x0000001FU
#define CKMD_HFXTINIT_Q1CAP_CAP30                                   0x0000001EU
#define CKMD_HFXTINIT_Q1CAP_CAP29                                   0x0000001DU
#define CKMD_HFXTINIT_Q1CAP_CAP28                                   0x0000001CU
#define CKMD_HFXTINIT_Q1CAP_CAP27                                   0x0000001BU
#define CKMD_HFXTINIT_Q1CAP_CAP26                                   0x0000001AU
#define CKMD_HFXTINIT_Q1CAP_CAP25                                   0x00000019U
#define CKMD_HFXTINIT_Q1CAP_CAP24                                   0x00000018U
#define CKMD_HFXTINIT_Q1CAP_CAP23                                   0x00000017U
#define CKMD_HFXTINIT_Q1CAP_CAP22                                   0x00000016U
#define CKMD_HFXTINIT_Q1CAP_CAP21                                   0x00000015U
#define CKMD_HFXTINIT_Q1CAP_CAP20                                   0x00000014U
#define CKMD_HFXTINIT_Q1CAP_CAP19                                   0x00000013U
#define CKMD_HFXTINIT_Q1CAP_CAP18                                   0x00000012U
#define CKMD_HFXTINIT_Q1CAP_CAP17                                   0x00000011U
#define CKMD_HFXTINIT_Q1CAP_CAP16                                   0x00000010U
#define CKMD_HFXTINIT_Q1CAP_CAP15                                   0x0000000FU
#define CKMD_HFXTINIT_Q1CAP_CAP14                                   0x0000000EU
#define CKMD_HFXTINIT_Q1CAP_CAP13                                   0x0000000DU
#define CKMD_HFXTINIT_Q1CAP_CAP12                                   0x0000000CU
#define CKMD_HFXTINIT_Q1CAP_CAP11                                   0x0000000BU
#define CKMD_HFXTINIT_Q1CAP_CAP10                                   0x0000000AU
#define CKMD_HFXTINIT_Q1CAP_CAP9                                    0x00000009U
#define CKMD_HFXTINIT_Q1CAP_CAP8                                    0x00000008U
#define CKMD_HFXTINIT_Q1CAP_CAP7                                    0x00000007U
#define CKMD_HFXTINIT_Q1CAP_CAP6                                    0x00000006U
#define CKMD_HFXTINIT_Q1CAP_CAP5                                    0x00000005U
#define CKMD_HFXTINIT_Q1CAP_CAP4                                    0x00000004U
#define CKMD_HFXTINIT_Q1CAP_CAP3                                    0x00000003U
#define CKMD_HFXTINIT_Q1CAP_CAP2                                    0x00000002U
#define CKMD_HFXTINIT_Q1CAP_CAP1                                    0x00000001U
#define CKMD_HFXTINIT_Q1CAP_CAP0                                    0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_HFXTTARG
//
//*****************************************************************************
// Field: [31:30] AMPHYST
//
// ADC hysteresis used during IDAC updates.
//
// Every AMPCFG1.INTERVAL, IDAC will be regulated
// - up as long as ADC < AMPTHR
// - down as long as ADC > AMPTHR+AMPHYST
#define CKMD_HFXTTARG_AMPHYST_W                                              2U
#define CKMD_HFXTTARG_AMPHYST_M                                     0xC0000000U
#define CKMD_HFXTTARG_AMPHYST_S                                             30U

// Field: [29:23] AMPTHR
//
// Minimum HFXT amplitude
#define CKMD_HFXTTARG_AMPTHR_W                                               7U
#define CKMD_HFXTTARG_AMPTHR_M                                      0x3F800000U
#define CKMD_HFXTTARG_AMPTHR_S                                              23U

// Field: [22:16] IDAC
//
// Minimum IDAC current
#define CKMD_HFXTTARG_IDAC_W                                                 7U
#define CKMD_HFXTTARG_IDAC_M                                        0x007F0000U
#define CKMD_HFXTTARG_IDAC_S                                                16U

// Field: [15:12] IREF
//
// Target HFXT IREF current
#define CKMD_HFXTTARG_IREF_W                                                 4U
#define CKMD_HFXTTARG_IREF_M                                        0x0000F000U
#define CKMD_HFXTTARG_IREF_S                                                12U

// Field:  [11:6] Q2CAP
//
// Target HFXT Q2 cap trim
// ENUMs:
// CAP63                    Nominal 25C	=	1.10E-11 F
//                          Stong 25C		=	1.00E-11 F
//                          Weak 25C		=	1.19E-11 F
// CAP62                    Nominal 25C	=	1.08E-11 F
//                          Stong 25C		=	9.82E-12 F
//                          Weak 25C		=	1.17E-11 F
// CAP61                    Nominal 25C	=	1.06E-11 F
//                          Stong 25C		=	9.64E-12 F
//                          Weak 25C		=	1.15E-11 F
// CAP60                    Nominal 25C	=	1.04E-11 F
//                          Stong 25C		=	9.45E-12 F
//                          Weak 25C		=	1.13E-11 F
// CAP59                    Nominal 25C	=	1.02E-11 F
//                          Stong 25C		=	9.26E-12 F
//                          Weak 25C		=	1.11E-11 F
// CAP58                    Nominal 25C	=	1.00E-11 F
//                          Stong 25C		=	9.07E-12 F
//                          Weak 25C		=	1.09E-11 F
// CAP57                    Nominal 25C	=	9.84E-12 F
//                          Stong 25C		=	8.89E-12 F
//                          Weak 25C		=	1.07E-11 F
// CAP56                    Nominal 25C	=	9.65E-12 F
//                          Stong 25C		=	8.70E-12 F
//                          Weak 25C		=	1.04E-11 F
// CAP55                    Nominal 25C	=	9.46E-12 F
//                          Stong 25C		=	8.51E-12 F
//                          Weak 25C		=	1.02E-11 F
// CAP54                    Nominal 25C	=	9.26E-12 F
//                          Stong 25C		=	8.32E-12 F
//                          Weak 25C		=	1.00E-11 F
// CAP53                    Nominal 25C	=	9.07E-12 F
//                          Stong 25C		=	8.13E-12 F
//                          Weak 25C		=	9.81E-12 F
// CAP52                    Nominal 25C	=	8.88E-12 F
//                          Stong 25C		=	7.94E-12 F
//                          Weak 25C		=	9.60E-12 F
// CAP51                    Nominal 25C	=	8.69E-12 F
//                          Stong 25C		=	7.76E-12 F
//                          Weak 25C		=	9.39E-12 F
// CAP50                    Nominal 25C	=	8.49E-12 F
//                          Stong 25C		=	7.57E-12 F
//                          Weak 25C		=	9.18E-12 F
// CAP49                    Nominal 25C	=	8.30E-12 F
//                          Stong 25C		=	7.38E-12 F
//                          Weak 25C		=	8.96E-12 F
// CAP48                    Nominal 25C	=	8.10E-12 F
//                          Stong 25C		=	7.18E-12 F
//                          Weak 25C		=	8.75E-12 F
// CAP47                    Nominal 25C	=	8.10E-12 F
//                          Stong 25C		=	7.18E-12 F
//                          Weak 25C		=	8.75E-12 F
// CAP46                    Nominal 25C	=	7.97E-12 F
//                          Stong 25C		=	7.05E-12 F
//                          Weak 25C		=	8.60E-12 F
// CAP45                    Nominal 25C	=	7.83E-12 F
//                          Stong 25C		=	6.91E-12 F
//                          Weak 25C		=	8.44E-12 F
// CAP44                    Nominal 25C	=	7.69E-12 F
//                          Stong 25C		=	6.77E-12 F
//                          Weak 25C		=	8.29E-12 F
// CAP43                    Nominal 25C	=	7.55E-12 F
//                          Stong 25C		=	6.63E-12 F
//                          Weak 25C		=	8.13E-12 F
// CAP42                    Nominal 25C	=	7.40E-12 F
//                          Stong 25C		=	6.49E-12 F
//                          Weak 25C		=	7.98E-12 F
// CAP41                    Nominal 25C	=	7.26E-12 F
//                          Stong 25C		=	6.35E-12 F
//                          Weak 25C		=	7.82E-12 F
// CAP40                    Nominal 25C	=	7.12E-12 F
//                          Stong 25C		=	6.21E-12 F
//                          Weak 25C		=	7.67E-12 F
// CAP39                    Nominal 25C	=	6.98E-12 F
//                          Stong 25C		=	6.06E-12 F
//                          Weak 25C		=	7.51E-12 F
// CAP38                    Nominal 25C	=	6.84E-12 F
//                          Stong 25C		=	5.92E-12 F
//                          Weak 25C		=	7.35E-12 F
// CAP37                    Nominal 25C	=	6.69E-12 F
//                          Stong 25C		=	5.78E-12 F
//                          Weak 25C		=	7.20E-12 F
// CAP36                    Nominal 25C	=	6.55E-12 F
//                          Stong 25C		=	5.63E-12 F
//                          Weak 25C		=	7.04E-12 F
// CAP35                    Nominal 25C	=	6.41E-12 F
//                          Stong 25C		=	5.49E-12 F
//                          Weak 25C		=	6.88E-12 F
// CAP34                    Nominal 25C	=	6.26E-12 F
//                          Stong 25C		=	5.35E-12 F
//                          Weak 25C		=	6.72E-12 F
// CAP33                    Nominal 25C	=	6.12E-12 F
//                          Stong 25C		=	5.20E-12 F
//                          Weak 25C		=	6.56E-12 F
// CAP32                    Nominal 25C	=	5.97E-12 F
//                          Stong 25C		=	5.05E-12 F
//                          Weak 25C		=	6.40E-12 F
// CAP31                    Nominal 25C	=	5.72E-12 F
//                          Stong 25C		=	4.79E-12 F
//                          Weak 25C		=	6.12E-12 F
// CAP30                    Nominal 25C	=	5.60E-12 F
//                          Stong 25C		=	4.67E-12 F
//                          Weak 25C		=	5.99E-12 F
// CAP29                    Nominal 25C	=	5.49E-12 F
//                          Stong 25C		=	4.56E-12 F
//                          Weak 25C		=	5.87E-12 F
// CAP28                    Nominal 25C	=	5.37E-12 F
//                          Stong 25C		=	4.44E-12 F
//                          Weak 25C		=	5.74E-12 F
// CAP27                    Nominal 25C	=	5.26E-12 F
//                          Stong 25C		=	4.32E-12 F
//                          Weak 25C		=	5.61E-12 F
// CAP26                    Nominal 25C	=	5.15E-12 F
//                          Stong 25C		=	4.21E-12 F
//                          Weak 25C		=	5.49E-12 F
// CAP25                    Nominal 25C	=	5.03E-12 F
//                          Stong 25C		=	4.09E-12 F
//                          Weak 25C		=	5.36E-12 F
// CAP24                    Nominal 25C	=	4.91E-12 F
//                          Stong 25C		=	3.97E-12 F
//                          Weak 25C		=	5.23E-12 F
// CAP23                    Nominal 25C	=	4.80E-12 F
//                          Stong 25C		=	3.85E-12 F
//                          Weak 25C		=	5.10E-12 F
// CAP22                    Nominal 25C	=	4.68E-12 F
//                          Stong 25C		=	3.73E-12 F
//                          Weak 25C		=	4.98E-12 F
// CAP21                    Nominal 25C	=	4.57E-12 F
//                          Stong 25C		=	3.60E-12 F
//                          Weak 25C		=	4.85E-12 F
// CAP20                    Nominal 25C	=	4.45E-12 F
//                          Stong 25C		=	3.48E-12 F
//                          Weak 25C		=	4.72E-12 F
// CAP19                    Nominal 25C	=	4.33E-12 F
//                          Stong 25C		=	3.36E-12 F
//                          Weak 25C		=	4.59E-12 F
// CAP18                    Nominal 25C	=	4.21E-12 F
//                          Stong 25C		=	3.23E-12 F
//                          Weak 25C		=	4.46E-12 F
// CAP17                    Nominal 25C	=	4.09E-12 F
//                          Stong 25C		=	3.11E-12 F
//                          Weak 25C		=	4.33E-12 F
// CAP16                    Nominal 25C	=	3.97E-12 F
//                          Stong 25C		=	2.98E-12 F
//                          Weak 25C		=	4.20E-12 F
// CAP15                    Nominal 25C	=	3.97E-12 F
//                          Stong 25C		=	2.98E-12 F
//                          Weak 25C		=	4.20E-12 F
// CAP14                    Nominal 25C	=	3.88E-12 F
//                          Stong 25C		=	2.89E-12 F
//                          Weak 25C		=	4.10E-12 F
// CAP13                    Nominal 25C	=	3.79E-12 F
//                          Stong 25C		=	2.79E-12 F
//                          Weak 25C		=	4.00E-12 F
// CAP12                    Nominal 25C	=	3.70E-12 F
//                          Stong 25C		=	2.69E-12 F
//                          Weak 25C		=	3.90E-12 F
// CAP11                    Nominal 25C	=	3.61E-12 F
//                          Stong 25C		=	2.59E-12 F
//                          Weak 25C		=	3.80E-12 F
// CAP10                    Nominal 25C	=	3.51E-12 F
//                          Stong 25C		=	2.49E-12 F
//                          Weak 25C		=	3.70E-12 F
// CAP9                     Nominal 25C	=	3.42E-12 F
//                          Stong 25C		=	2.39E-12 F
//                          Weak 25C		=	3.60E-12 F
// CAP8                     Nominal 25C	=	3.33E-12 F
//                          Stong 25C		=	2.29E-12 F
//                          Weak 25C		=	3.50E-12 F
// CAP7                     Nominal 25C	=	3.23E-12 F
//                          Stong 25C		=	2.19E-12 F
//                          Weak 25C		=	3.40E-12 F
// CAP6                     Nominal 25C	=	3.14E-12 F
//                          Stong 25C		=	2.09E-12 F
//                          Weak 25C		=	3.30E-12 F
// CAP5                     Nominal 25C	=	3.04E-12 F
//                          Stong 25C		=	1.99E-12 F
//                          Weak 25C		=	3.19E-12 F
// CAP4                     Nominal 25C	=	2.95E-12 F
//                          Stong 25C		=	1.89E-12 F
//                          Weak 25C		=	3.09E-12 F
// CAP3                     Nominal 25C	=	2.85E-12 F
//                          Stong 25C		=	1.79E-12 F
//                          Weak 25C		=	2.99E-12 F
// CAP2                     Nominal 25C	=	2.76E-12 F
//                          Stong 25C		=	1.69E-12 F
//                          Weak 25C		=	2.89E-12 F
// CAP1                     Nominal 25C	=	2.66E-12 F
//                          Stong 25C		=	1.60E-12 F
//                          Weak 25C		=	2.79E-12 F
// CAP0                     Nominal 25C	=	2.57E-12 F
//                          Stong 25C		=	1.50E-12 F
//                          Weak 25C		=	2.69E-12 F
#define CKMD_HFXTTARG_Q2CAP_W                                                6U
#define CKMD_HFXTTARG_Q2CAP_M                                       0x00000FC0U
#define CKMD_HFXTTARG_Q2CAP_S                                                6U
#define CKMD_HFXTTARG_Q2CAP_CAP63                                   0x00000FC0U
#define CKMD_HFXTTARG_Q2CAP_CAP62                                   0x00000F80U
#define CKMD_HFXTTARG_Q2CAP_CAP61                                   0x00000F40U
#define CKMD_HFXTTARG_Q2CAP_CAP60                                   0x00000F00U
#define CKMD_HFXTTARG_Q2CAP_CAP59                                   0x00000EC0U
#define CKMD_HFXTTARG_Q2CAP_CAP58                                   0x00000E80U
#define CKMD_HFXTTARG_Q2CAP_CAP57                                   0x00000E40U
#define CKMD_HFXTTARG_Q2CAP_CAP56                                   0x00000E00U
#define CKMD_HFXTTARG_Q2CAP_CAP55                                   0x00000DC0U
#define CKMD_HFXTTARG_Q2CAP_CAP54                                   0x00000D80U
#define CKMD_HFXTTARG_Q2CAP_CAP53                                   0x00000D40U
#define CKMD_HFXTTARG_Q2CAP_CAP52                                   0x00000D00U
#define CKMD_HFXTTARG_Q2CAP_CAP51                                   0x00000CC0U
#define CKMD_HFXTTARG_Q2CAP_CAP50                                   0x00000C80U
#define CKMD_HFXTTARG_Q2CAP_CAP49                                   0x00000C40U
#define CKMD_HFXTTARG_Q2CAP_CAP48                                   0x00000C00U
#define CKMD_HFXTTARG_Q2CAP_CAP47                                   0x00000BC0U
#define CKMD_HFXTTARG_Q2CAP_CAP46                                   0x00000B80U
#define CKMD_HFXTTARG_Q2CAP_CAP45                                   0x00000B40U
#define CKMD_HFXTTARG_Q2CAP_CAP44                                   0x00000B00U
#define CKMD_HFXTTARG_Q2CAP_CAP43                                   0x00000AC0U
#define CKMD_HFXTTARG_Q2CAP_CAP42                                   0x00000A80U
#define CKMD_HFXTTARG_Q2CAP_CAP41                                   0x00000A40U
#define CKMD_HFXTTARG_Q2CAP_CAP40                                   0x00000A00U
#define CKMD_HFXTTARG_Q2CAP_CAP39                                   0x000009C0U
#define CKMD_HFXTTARG_Q2CAP_CAP38                                   0x00000980U
#define CKMD_HFXTTARG_Q2CAP_CAP37                                   0x00000940U
#define CKMD_HFXTTARG_Q2CAP_CAP36                                   0x00000900U
#define CKMD_HFXTTARG_Q2CAP_CAP35                                   0x000008C0U
#define CKMD_HFXTTARG_Q2CAP_CAP34                                   0x00000880U
#define CKMD_HFXTTARG_Q2CAP_CAP33                                   0x00000840U
#define CKMD_HFXTTARG_Q2CAP_CAP32                                   0x00000800U
#define CKMD_HFXTTARG_Q2CAP_CAP31                                   0x000007C0U
#define CKMD_HFXTTARG_Q2CAP_CAP30                                   0x00000780U
#define CKMD_HFXTTARG_Q2CAP_CAP29                                   0x00000740U
#define CKMD_HFXTTARG_Q2CAP_CAP28                                   0x00000700U
#define CKMD_HFXTTARG_Q2CAP_CAP27                                   0x000006C0U
#define CKMD_HFXTTARG_Q2CAP_CAP26                                   0x00000680U
#define CKMD_HFXTTARG_Q2CAP_CAP25                                   0x00000640U
#define CKMD_HFXTTARG_Q2CAP_CAP24                                   0x00000600U
#define CKMD_HFXTTARG_Q2CAP_CAP23                                   0x000005C0U
#define CKMD_HFXTTARG_Q2CAP_CAP22                                   0x00000580U
#define CKMD_HFXTTARG_Q2CAP_CAP21                                   0x00000540U
#define CKMD_HFXTTARG_Q2CAP_CAP20                                   0x00000500U
#define CKMD_HFXTTARG_Q2CAP_CAP19                                   0x000004C0U
#define CKMD_HFXTTARG_Q2CAP_CAP18                                   0x00000480U
#define CKMD_HFXTTARG_Q2CAP_CAP17                                   0x00000440U
#define CKMD_HFXTTARG_Q2CAP_CAP16                                   0x00000400U
#define CKMD_HFXTTARG_Q2CAP_CAP15                                   0x000003C0U
#define CKMD_HFXTTARG_Q2CAP_CAP14                                   0x00000380U
#define CKMD_HFXTTARG_Q2CAP_CAP13                                   0x00000340U
#define CKMD_HFXTTARG_Q2CAP_CAP12                                   0x00000300U
#define CKMD_HFXTTARG_Q2CAP_CAP11                                   0x000002C0U
#define CKMD_HFXTTARG_Q2CAP_CAP10                                   0x00000280U
#define CKMD_HFXTTARG_Q2CAP_CAP9                                    0x00000240U
#define CKMD_HFXTTARG_Q2CAP_CAP8                                    0x00000200U
#define CKMD_HFXTTARG_Q2CAP_CAP7                                    0x000001C0U
#define CKMD_HFXTTARG_Q2CAP_CAP6                                    0x00000180U
#define CKMD_HFXTTARG_Q2CAP_CAP5                                    0x00000140U
#define CKMD_HFXTTARG_Q2CAP_CAP4                                    0x00000100U
#define CKMD_HFXTTARG_Q2CAP_CAP3                                    0x000000C0U
#define CKMD_HFXTTARG_Q2CAP_CAP2                                    0x00000080U
#define CKMD_HFXTTARG_Q2CAP_CAP1                                    0x00000040U
#define CKMD_HFXTTARG_Q2CAP_CAP0                                    0x00000000U

// Field:   [5:0] Q1CAP
//
// Target HFXT Q1 cap trim
// ENUMs:
// CAP63                    Nominal 25C	=	1.10E-11 F
//                          Stong 25C		=	1.00E-11 F
//                          Weak 25C		=	1.19E-11 F
// CAP62                    Nominal 25C	=	1.08E-11 F
//                          Stong 25C		=	9.82E-12 F
//                          Weak 25C		=	1.17E-11 F
// CAP61                    Nominal 25C	=	1.06E-11 F
//                          Stong 25C		=	9.64E-12 F
//                          Weak 25C		=	1.15E-11 F
// CAP60                    Nominal 25C	=	1.04E-11 F
//                          Stong 25C		=	9.45E-12 F
//                          Weak 25C		=	1.13E-11 F
// CAP59                    Nominal 25C	=	1.02E-11 F
//                          Stong 25C		=	9.26E-12 F
//                          Weak 25C		=	1.11E-11 F
// CAP58                    Nominal 25C	=	1.00E-11 F
//                          Stong 25C		=	9.07E-12 F
//                          Weak 25C		=	1.09E-11 F
// CAP57                    Nominal 25C	=	9.84E-12 F
//                          Stong 25C		=	8.89E-12 F
//                          Weak 25C		=	1.07E-11 F
// CAP56                    Nominal 25C	=	9.65E-12 F
//                          Stong 25C		=	8.70E-12 F
//                          Weak 25C		=	1.04E-11 F
// CAP55                    Nominal 25C	=	9.46E-12 F
//                          Stong 25C		=	8.51E-12 F
//                          Weak 25C		=	1.02E-11 F
// CAP54                    Nominal 25C	=	9.26E-12 F
//                          Stong 25C		=	8.32E-12 F
//                          Weak 25C		=	1.00E-11 F
// CAP53                    Nominal 25C	=	9.07E-12 F
//                          Stong 25C		=	8.13E-12 F
//                          Weak 25C		=	9.81E-12 F
// CAP52                    Nominal 25C	=	8.88E-12 F
//                          Stong 25C		=	7.94E-12 F
//                          Weak 25C		=	9.60E-12 F
// CAP51                    Nominal 25C	=	8.69E-12 F
//                          Stong 25C		=	7.76E-12 F
//                          Weak 25C		=	9.39E-12 F
// CAP50                    Nominal 25C	=	8.49E-12 F
//                          Stong 25C		=	7.57E-12 F
//                          Weak 25C		=	9.18E-12 F
// CAP49                    Nominal 25C	=	8.30E-12 F
//                          Stong 25C		=	7.38E-12 F
//                          Weak 25C		=	8.96E-12 F
// CAP48                    Nominal 25C	=	8.10E-12 F
//                          Stong 25C		=	7.18E-12 F
//                          Weak 25C		=	8.75E-12 F
// CAP47                    Nominal 25C	=	8.10E-12 F
//                          Stong 25C		=	7.18E-12 F
//                          Weak 25C		=	8.75E-12 F
// CAP46                    Nominal 25C	=	7.97E-12 F
//                          Stong 25C		=	7.05E-12 F
//                          Weak 25C		=	8.60E-12 F
// CAP45                    Nominal 25C	=	7.83E-12 F
//                          Stong 25C		=	6.91E-12 F
//                          Weak 25C		=	8.44E-12 F
// CAP44                    Nominal 25C	=	7.69E-12 F
//                          Stong 25C		=	6.77E-12 F
//                          Weak 25C		=	8.29E-12 F
// CAP43                    Nominal 25C	=	7.55E-12 F
//                          Stong 25C		=	6.63E-12 F
//                          Weak 25C		=	8.13E-12 F
// CAP42                    Nominal 25C	=	7.40E-12 F
//                          Stong 25C		=	6.49E-12 F
//                          Weak 25C		=	7.98E-12 F
// CAP41                    Nominal 25C	=	7.26E-12 F
//                          Stong 25C		=	6.35E-12 F
//                          Weak 25C		=	7.82E-12 F
// CAP40                    Nominal 25C	=	7.12E-12 F
//                          Stong 25C		=	6.21E-12 F
//                          Weak 25C		=	7.67E-12 F
// CAP39                    Nominal 25C	=	6.98E-12 F
//                          Stong 25C		=	6.06E-12 F
//                          Weak 25C		=	7.51E-12 F
// CAP38                    Nominal 25C	=	6.84E-12 F
//                          Stong 25C		=	5.92E-12 F
//                          Weak 25C		=	7.35E-12 F
// CAP37                    Nominal 25C	=	6.69E-12 F
//                          Stong 25C		=	5.78E-12 F
//                          Weak 25C		=	7.20E-12 F
// CAP36                    Nominal 25C	=	6.55E-12 F
//                          Stong 25C		=	5.63E-12 F
//                          Weak 25C		=	7.04E-12 F
// CAP35                    Nominal 25C	=	6.41E-12 F
//                          Stong 25C		=	5.49E-12 F
//                          Weak 25C		=	6.88E-12 F
// CAP34                    Nominal 25C	=	6.26E-12 F
//                          Stong 25C		=	5.35E-12 F
//                          Weak 25C		=	6.72E-12 F
// CAP33                    Nominal 25C	=	6.12E-12 F
//                          Stong 25C		=	5.20E-12 F
//                          Weak 25C		=	6.56E-12 F
// CAP32                    Nominal 25C	=	5.97E-12 F
//                          Stong 25C		=	5.05E-12 F
//                          Weak 25C		=	6.40E-12 F
// CAP31                    Nominal 25C	=	5.72E-12 F
//                          Stong 25C		=	4.79E-12 F
//                          Weak 25C		=	6.12E-12 F
// CAP30                    Nominal 25C	=	5.60E-12 F
//                          Stong 25C		=	4.67E-12 F
//                          Weak 25C		=	5.99E-12 F
// CAP29                    Nominal 25C	=	5.49E-12 F
//                          Stong 25C		=	4.56E-12 F
//                          Weak 25C		=	5.87E-12 F
// CAP28                    Nominal 25C	=	5.37E-12 F
//                          Stong 25C		=	4.44E-12 F
//                          Weak 25C		=	5.74E-12 F
// CAP27                    Nominal 25C	=	5.26E-12 F
//                          Stong 25C		=	4.32E-12 F
//                          Weak 25C		=	5.61E-12 F
// CAP26                    Nominal 25C	=	5.15E-12 F
//                          Stong 25C		=	4.21E-12 F
//                          Weak 25C		=	5.49E-12 F
// CAP25                    Nominal 25C	=	5.03E-12 F
//                          Stong 25C		=	4.09E-12 F
//                          Weak 25C		=	5.36E-12 F
// CAP24                    Nominal 25C	=	4.91E-12 F
//                          Stong 25C		=	3.97E-12 F
//                          Weak 25C		=	5.23E-12 F
// CAP23                    Nominal 25C	=	4.80E-12 F
//                          Stong 25C		=	3.85E-12 F
//                          Weak 25C		=	5.10E-12 F
// CAP22                    Nominal 25C	=	4.68E-12 F
//                          Stong 25C		=	3.73E-12 F
//                          Weak 25C		=	4.98E-12 F
// CAP21                    Nominal 25C	=	4.57E-12 F
//                          Stong 25C		=	3.60E-12 F
//                          Weak 25C		=	4.85E-12 F
// CAP20                    Nominal 25C	=	4.45E-12 F
//                          Stong 25C		=	3.48E-12 F
//                          Weak 25C		=	4.72E-12 F
// CAP19                    Nominal 25C	=	4.33E-12 F
//                          Stong 25C		=	3.36E-12 F
//                          Weak 25C		=	4.59E-12 F
// CAP18                    Nominal 25C	=	4.21E-12 F
//                          Stong 25C		=	3.23E-12 F
//                          Weak 25C		=	4.46E-12 F
// CAP17                    Nominal 25C	=	4.09E-12 F
//                          Stong 25C		=	3.11E-12 F
//                          Weak 25C		=	4.33E-12 F
// CAP16                    Nominal 25C	=	3.97E-12 F
//                          Stong 25C		=	2.98E-12 F
//                          Weak 25C		=	4.20E-12 F
// CAP15                    Nominal 25C	=	3.97E-12 F
//                          Stong 25C		=	2.98E-12 F
//                          Weak 25C		=	4.20E-12 F
// CAP14                    Nominal 25C	=	3.88E-12 F
//                          Stong 25C		=	2.89E-12 F
//                          Weak 25C		=	4.10E-12 F
// CAP13                    Nominal 25C	=	3.79E-12 F
//                          Stong 25C		=	2.79E-12 F
//                          Weak 25C		=	4.00E-12 F
// CAP12                    Nominal 25C	=	3.70E-12 F
//                          Stong 25C		=	2.69E-12 F
//                          Weak 25C		=	3.90E-12 F
// CAP11                    Nominal 25C	=	3.61E-12 F
//                          Stong 25C		=	2.59E-12 F
//                          Weak 25C		=	3.80E-12 F
// CAP10                    Nominal 25C	=	3.51E-12 F
//                          Stong 25C		=	2.49E-12 F
//                          Weak 25C		=	3.70E-12 F
// CAP9                     Nominal 25C	=	3.42E-12 F
//                          Stong 25C		=	2.39E-12 F
//                          Weak 25C		=	3.60E-12 F
// CAP8                     Nominal 25C	=	3.33E-12 F
//                          Stong 25C		=	2.29E-12 F
//                          Weak 25C		=	3.50E-12 F
// CAP7                     Nominal 25C	=	3.23E-12 F
//                          Stong 25C		=	2.19E-12 F
//                          Weak 25C		=	3.40E-12 F
// CAP6                     Nominal 25C	=	3.14E-12 F
//                          Stong 25C		=	2.09E-12 F
//                          Weak 25C		=	3.30E-12 F
// CAP5                     Nominal 25C	=	3.04E-12 F
//                          Stong 25C		=	1.99E-12 F
//                          Weak 25C		=	3.19E-12 F
// CAP4                     Nominal 25C	=	2.95E-12 F
//                          Stong 25C		=	1.89E-12 F
//                          Weak 25C		=	3.09E-12 F
// CAP3                     Nominal 25C	=	2.85E-12 F
//                          Stong 25C		=	1.79E-12 F
//                          Weak 25C		=	2.99E-12 F
// CAP2                     Nominal 25C	=	2.76E-12 F
//                          Stong 25C		=	1.69E-12 F
//                          Weak 25C		=	2.89E-12 F
// CAP1                     Nominal 25C	=	2.66E-12 F
//                          Stong 25C		=	1.60E-12 F
//                          Weak 25C		=	2.79E-12 F
// CAP0                     Nominal 25C	=	2.57E-12 F
//                          Stong 25C		=	1.50E-12 F
//                          Weak 25C		=	2.69E-12 F
#define CKMD_HFXTTARG_Q1CAP_W                                                6U
#define CKMD_HFXTTARG_Q1CAP_M                                       0x0000003FU
#define CKMD_HFXTTARG_Q1CAP_S                                                0U
#define CKMD_HFXTTARG_Q1CAP_CAP63                                   0x0000003FU
#define CKMD_HFXTTARG_Q1CAP_CAP62                                   0x0000003EU
#define CKMD_HFXTTARG_Q1CAP_CAP61                                   0x0000003DU
#define CKMD_HFXTTARG_Q1CAP_CAP60                                   0x0000003CU
#define CKMD_HFXTTARG_Q1CAP_CAP59                                   0x0000003BU
#define CKMD_HFXTTARG_Q1CAP_CAP58                                   0x0000003AU
#define CKMD_HFXTTARG_Q1CAP_CAP57                                   0x00000039U
#define CKMD_HFXTTARG_Q1CAP_CAP56                                   0x00000038U
#define CKMD_HFXTTARG_Q1CAP_CAP55                                   0x00000037U
#define CKMD_HFXTTARG_Q1CAP_CAP54                                   0x00000036U
#define CKMD_HFXTTARG_Q1CAP_CAP53                                   0x00000035U
#define CKMD_HFXTTARG_Q1CAP_CAP52                                   0x00000034U
#define CKMD_HFXTTARG_Q1CAP_CAP51                                   0x00000033U
#define CKMD_HFXTTARG_Q1CAP_CAP50                                   0x00000032U
#define CKMD_HFXTTARG_Q1CAP_CAP49                                   0x00000031U
#define CKMD_HFXTTARG_Q1CAP_CAP48                                   0x00000030U
#define CKMD_HFXTTARG_Q1CAP_CAP47                                   0x0000002FU
#define CKMD_HFXTTARG_Q1CAP_CAP46                                   0x0000002EU
#define CKMD_HFXTTARG_Q1CAP_CAP45                                   0x0000002DU
#define CKMD_HFXTTARG_Q1CAP_CAP44                                   0x0000002CU
#define CKMD_HFXTTARG_Q1CAP_CAP43                                   0x0000002BU
#define CKMD_HFXTTARG_Q1CAP_CAP42                                   0x0000002AU
#define CKMD_HFXTTARG_Q1CAP_CAP41                                   0x00000029U
#define CKMD_HFXTTARG_Q1CAP_CAP40                                   0x00000028U
#define CKMD_HFXTTARG_Q1CAP_CAP39                                   0x00000027U
#define CKMD_HFXTTARG_Q1CAP_CAP38                                   0x00000026U
#define CKMD_HFXTTARG_Q1CAP_CAP37                                   0x00000025U
#define CKMD_HFXTTARG_Q1CAP_CAP36                                   0x00000024U
#define CKMD_HFXTTARG_Q1CAP_CAP35                                   0x00000023U
#define CKMD_HFXTTARG_Q1CAP_CAP34                                   0x00000022U
#define CKMD_HFXTTARG_Q1CAP_CAP33                                   0x00000021U
#define CKMD_HFXTTARG_Q1CAP_CAP32                                   0x00000020U
#define CKMD_HFXTTARG_Q1CAP_CAP31                                   0x0000001FU
#define CKMD_HFXTTARG_Q1CAP_CAP30                                   0x0000001EU
#define CKMD_HFXTTARG_Q1CAP_CAP29                                   0x0000001DU
#define CKMD_HFXTTARG_Q1CAP_CAP28                                   0x0000001CU
#define CKMD_HFXTTARG_Q1CAP_CAP27                                   0x0000001BU
#define CKMD_HFXTTARG_Q1CAP_CAP26                                   0x0000001AU
#define CKMD_HFXTTARG_Q1CAP_CAP25                                   0x00000019U
#define CKMD_HFXTTARG_Q1CAP_CAP24                                   0x00000018U
#define CKMD_HFXTTARG_Q1CAP_CAP23                                   0x00000017U
#define CKMD_HFXTTARG_Q1CAP_CAP22                                   0x00000016U
#define CKMD_HFXTTARG_Q1CAP_CAP21                                   0x00000015U
#define CKMD_HFXTTARG_Q1CAP_CAP20                                   0x00000014U
#define CKMD_HFXTTARG_Q1CAP_CAP19                                   0x00000013U
#define CKMD_HFXTTARG_Q1CAP_CAP18                                   0x00000012U
#define CKMD_HFXTTARG_Q1CAP_CAP17                                   0x00000011U
#define CKMD_HFXTTARG_Q1CAP_CAP16                                   0x00000010U
#define CKMD_HFXTTARG_Q1CAP_CAP15                                   0x0000000FU
#define CKMD_HFXTTARG_Q1CAP_CAP14                                   0x0000000EU
#define CKMD_HFXTTARG_Q1CAP_CAP13                                   0x0000000DU
#define CKMD_HFXTTARG_Q1CAP_CAP12                                   0x0000000CU
#define CKMD_HFXTTARG_Q1CAP_CAP11                                   0x0000000BU
#define CKMD_HFXTTARG_Q1CAP_CAP10                                   0x0000000AU
#define CKMD_HFXTTARG_Q1CAP_CAP9                                    0x00000009U
#define CKMD_HFXTTARG_Q1CAP_CAP8                                    0x00000008U
#define CKMD_HFXTTARG_Q1CAP_CAP7                                    0x00000007U
#define CKMD_HFXTTARG_Q1CAP_CAP6                                    0x00000006U
#define CKMD_HFXTTARG_Q1CAP_CAP5                                    0x00000005U
#define CKMD_HFXTTARG_Q1CAP_CAP4                                    0x00000004U
#define CKMD_HFXTTARG_Q1CAP_CAP3                                    0x00000003U
#define CKMD_HFXTTARG_Q1CAP_CAP2                                    0x00000002U
#define CKMD_HFXTTARG_Q1CAP_CAP1                                    0x00000001U
#define CKMD_HFXTTARG_Q1CAP_CAP0                                    0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_HFXTDYN
//
//*****************************************************************************
// Field:    [31] SEL
//
// Select the dynamic configuration.
//
// Amplitude ramping will always happen using the values in HFXTINIT, and
// HFXTTARG.
// Afterwards, this bit can be used to select between HFXTTARG and HFXTDYN.
// Hardware will ensure a smooth transition of analog control signals.
// ENUMs:
// DYNAMIC                  Select configuration in CKM.HFXTDYN0 and
//                          CKM.HFXTDYN1.
// TARGET                   Select configuration in CKM.HFXTTARG0 and
//                          CKM.HFXTTARG1.
#define CKMD_HFXTDYN_SEL                                            0x80000000U
#define CKMD_HFXTDYN_SEL_M                                          0x80000000U
#define CKMD_HFXTDYN_SEL_S                                                  31U
#define CKMD_HFXTDYN_SEL_DYNAMIC                                    0x80000000U
#define CKMD_HFXTDYN_SEL_TARGET                                     0x00000000U

// Field: [29:23] AMPTHR
//
// Minimum HFXT amplitude
#define CKMD_HFXTDYN_AMPTHR_W                                                7U
#define CKMD_HFXTDYN_AMPTHR_M                                       0x3F800000U
#define CKMD_HFXTDYN_AMPTHR_S                                               23U

// Field: [22:16] IDAC
//
// Minimum IDAC current
#define CKMD_HFXTDYN_IDAC_W                                                  7U
#define CKMD_HFXTDYN_IDAC_M                                         0x007F0000U
#define CKMD_HFXTDYN_IDAC_S                                                 16U

// Field: [15:12] IREF
//
// Target HFXT IREF current
#define CKMD_HFXTDYN_IREF_W                                                  4U
#define CKMD_HFXTDYN_IREF_M                                         0x0000F000U
#define CKMD_HFXTDYN_IREF_S                                                 12U

// Field:  [11:6] Q2CAP
//
// Target HFXT Q2 cap trim
// ENUMs:
// CAP63                    Nominal 25C	=	1.10E-11 F
//                          Stong 25C		=	1.00E-11 F
//                          Weak 25C		=	1.19E-11 F
// CAP62                    Nominal 25C	=	1.08E-11 F
//                          Stong 25C		=	9.82E-12 F
//                          Weak 25C		=	1.17E-11 F
// CAP61                    Nominal 25C	=	1.06E-11 F
//                          Stong 25C		=	9.64E-12 F
//                          Weak 25C		=	1.15E-11 F
// CAP60                    Nominal 25C	=	1.04E-11 F
//                          Stong 25C		=	9.45E-12 F
//                          Weak 25C		=	1.13E-11 F
// CAP59                    Nominal 25C	=	1.02E-11 F
//                          Stong 25C		=	9.26E-12 F
//                          Weak 25C		=	1.11E-11 F
// CAP58                    Nominal 25C	=	1.00E-11 F
//                          Stong 25C		=	9.07E-12 F
//                          Weak 25C		=	1.09E-11 F
// CAP57                    Nominal 25C	=	9.84E-12 F
//                          Stong 25C		=	8.89E-12 F
//                          Weak 25C		=	1.07E-11 F
// CAP56                    Nominal 25C	=	9.65E-12 F
//                          Stong 25C		=	8.70E-12 F
//                          Weak 25C		=	1.04E-11 F
// CAP55                    Nominal 25C	=	9.46E-12 F
//                          Stong 25C		=	8.51E-12 F
//                          Weak 25C		=	1.02E-11 F
// CAP54                    Nominal 25C	=	9.26E-12 F
//                          Stong 25C		=	8.32E-12 F
//                          Weak 25C		=	1.00E-11 F
// CAP53                    Nominal 25C	=	9.07E-12 F
//                          Stong 25C		=	8.13E-12 F
//                          Weak 25C		=	9.81E-12 F
// CAP52                    Nominal 25C	=	8.88E-12 F
//                          Stong 25C		=	7.94E-12 F
//                          Weak 25C		=	9.60E-12 F
// CAP51                    Nominal 25C	=	8.69E-12 F
//                          Stong 25C		=	7.76E-12 F
//                          Weak 25C		=	9.39E-12 F
// CAP50                    Nominal 25C	=	8.49E-12 F
//                          Stong 25C		=	7.57E-12 F
//                          Weak 25C		=	9.18E-12 F
// CAP49                    Nominal 25C	=	8.30E-12 F
//                          Stong 25C		=	7.38E-12 F
//                          Weak 25C		=	8.96E-12 F
// CAP48                    Nominal 25C	=	8.10E-12 F
//                          Stong 25C		=	7.18E-12 F
//                          Weak 25C		=	8.75E-12 F
// CAP47                    Nominal 25C	=	8.10E-12 F
//                          Stong 25C		=	7.18E-12 F
//                          Weak 25C		=	8.75E-12 F
// CAP46                    Nominal 25C	=	7.97E-12 F
//                          Stong 25C		=	7.05E-12 F
//                          Weak 25C		=	8.60E-12 F
// CAP45                    Nominal 25C	=	7.83E-12 F
//                          Stong 25C		=	6.91E-12 F
//                          Weak 25C		=	8.44E-12 F
// CAP44                    Nominal 25C	=	7.69E-12 F
//                          Stong 25C		=	6.77E-12 F
//                          Weak 25C		=	8.29E-12 F
// CAP43                    Nominal 25C	=	7.55E-12 F
//                          Stong 25C		=	6.63E-12 F
//                          Weak 25C		=	8.13E-12 F
// CAP42                    Nominal 25C	=	7.40E-12 F
//                          Stong 25C		=	6.49E-12 F
//                          Weak 25C		=	7.98E-12 F
// CAP41                    Nominal 25C	=	7.26E-12 F
//                          Stong 25C		=	6.35E-12 F
//                          Weak 25C		=	7.82E-12 F
// CAP40                    Nominal 25C	=	7.12E-12 F
//                          Stong 25C		=	6.21E-12 F
//                          Weak 25C		=	7.67E-12 F
// CAP39                    Nominal 25C	=	6.98E-12 F
//                          Stong 25C		=	6.06E-12 F
//                          Weak 25C		=	7.51E-12 F
// CAP38                    Nominal 25C	=	6.84E-12 F
//                          Stong 25C		=	5.92E-12 F
//                          Weak 25C		=	7.35E-12 F
// CAP37                    Nominal 25C	=	6.69E-12 F
//                          Stong 25C		=	5.78E-12 F
//                          Weak 25C		=	7.20E-12 F
// CAP36                    Nominal 25C	=	6.55E-12 F
//                          Stong 25C		=	5.63E-12 F
//                          Weak 25C		=	7.04E-12 F
// CAP35                    Nominal 25C	=	6.41E-12 F
//                          Stong 25C		=	5.49E-12 F
//                          Weak 25C		=	6.88E-12 F
// CAP34                    Nominal 25C	=	6.26E-12 F
//                          Stong 25C		=	5.35E-12 F
//                          Weak 25C		=	6.72E-12 F
// CAP33                    Nominal 25C	=	6.12E-12 F
//                          Stong 25C		=	5.20E-12 F
//                          Weak 25C		=	6.56E-12 F
// CAP32                    Nominal 25C	=	5.97E-12 F
//                          Stong 25C		=	5.05E-12 F
//                          Weak 25C		=	6.40E-12 F
// CAP31                    Nominal 25C	=	5.72E-12 F
//                          Stong 25C		=	4.79E-12 F
//                          Weak 25C		=	6.12E-12 F
// CAP30                    Nominal 25C	=	5.60E-12 F
//                          Stong 25C		=	4.67E-12 F
//                          Weak 25C		=	5.99E-12 F
// CAP29                    Nominal 25C	=	5.49E-12 F
//                          Stong 25C		=	4.56E-12 F
//                          Weak 25C		=	5.87E-12 F
// CAP28                    Nominal 25C	=	5.37E-12 F
//                          Stong 25C		=	4.44E-12 F
//                          Weak 25C		=	5.74E-12 F
// CAP27                    Nominal 25C	=	5.26E-12 F
//                          Stong 25C		=	4.32E-12 F
//                          Weak 25C		=	5.61E-12 F
// CAP26                    Nominal 25C	=	5.15E-12 F
//                          Stong 25C		=	4.21E-12 F
//                          Weak 25C		=	5.49E-12 F
// CAP25                    Nominal 25C	=	5.03E-12 F
//                          Stong 25C		=	4.09E-12 F
//                          Weak 25C		=	5.36E-12 F
// CAP24                    Nominal 25C	=	4.91E-12 F
//                          Stong 25C		=	3.97E-12 F
//                          Weak 25C		=	5.23E-12 F
// CAP23                    Nominal 25C	=	4.80E-12 F
//                          Stong 25C		=	3.85E-12 F
//                          Weak 25C		=	5.10E-12 F
// CAP22                    Nominal 25C	=	4.68E-12 F
//                          Stong 25C		=	3.73E-12 F
//                          Weak 25C		=	4.98E-12 F
// CAP21                    Nominal 25C	=	4.57E-12 F
//                          Stong 25C		=	3.60E-12 F
//                          Weak 25C		=	4.85E-12 F
// CAP20                    Nominal 25C	=	4.45E-12 F
//                          Stong 25C		=	3.48E-12 F
//                          Weak 25C		=	4.72E-12 F
// CAP19                    Nominal 25C	=	4.33E-12 F
//                          Stong 25C		=	3.36E-12 F
//                          Weak 25C		=	4.59E-12 F
// CAP18                    Nominal 25C	=	4.21E-12 F
//                          Stong 25C		=	3.23E-12 F
//                          Weak 25C		=	4.46E-12 F
// CAP17                    Nominal 25C	=	4.09E-12 F
//                          Stong 25C		=	3.11E-12 F
//                          Weak 25C		=	4.33E-12 F
// CAP16                    Nominal 25C	=	3.97E-12 F
//                          Stong 25C		=	2.98E-12 F
//                          Weak 25C		=	4.20E-12 F
// CAP15                    Nominal 25C	=	3.97E-12 F
//                          Stong 25C		=	2.98E-12 F
//                          Weak 25C		=	4.20E-12 F
// CAP14                    Nominal 25C	=	3.88E-12 F
//                          Stong 25C		=	2.89E-12 F
//                          Weak 25C		=	4.10E-12 F
// CAP13                    Nominal 25C	=	3.79E-12 F
//                          Stong 25C		=	2.79E-12 F
//                          Weak 25C		=	4.00E-12 F
// CAP12                    Nominal 25C	=	3.70E-12 F
//                          Stong 25C		=	2.69E-12 F
//                          Weak 25C		=	3.90E-12 F
// CAP11                    Nominal 25C	=	3.61E-12 F
//                          Stong 25C		=	2.59E-12 F
//                          Weak 25C		=	3.80E-12 F
// CAP10                    Nominal 25C	=	3.51E-12 F
//                          Stong 25C		=	2.49E-12 F
//                          Weak 25C		=	3.70E-12 F
// CAP9                     Nominal 25C	=	3.42E-12 F
//                          Stong 25C		=	2.39E-12 F
//                          Weak 25C		=	3.60E-12 F
// CAP8                     Nominal 25C	=	3.33E-12 F
//                          Stong 25C		=	2.29E-12 F
//                          Weak 25C		=	3.50E-12 F
// CAP7                     Nominal 25C	=	3.23E-12 F
//                          Stong 25C		=	2.19E-12 F
//                          Weak 25C		=	3.40E-12 F
// CAP6                     Nominal 25C	=	3.14E-12 F
//                          Stong 25C		=	2.09E-12 F
//                          Weak 25C		=	3.30E-12 F
// CAP5                     Nominal 25C	=	3.04E-12 F
//                          Stong 25C		=	1.99E-12 F
//                          Weak 25C		=	3.19E-12 F
// CAP4                     Nominal 25C	=	2.95E-12 F
//                          Stong 25C		=	1.89E-12 F
//                          Weak 25C		=	3.09E-12 F
// CAP3                     Nominal 25C	=	2.85E-12 F
//                          Stong 25C		=	1.79E-12 F
//                          Weak 25C		=	2.99E-12 F
// CAP2                     Nominal 25C	=	2.76E-12 F
//                          Stong 25C		=	1.69E-12 F
//                          Weak 25C		=	2.89E-12 F
// CAP1                     Nominal 25C	=	2.66E-12 F
//                          Stong 25C		=	1.60E-12 F
//                          Weak 25C		=	2.79E-12 F
// CAP0                     Nominal 25C	=	2.57E-12 F
//                          Stong 25C		=	1.50E-12 F
//                          Weak 25C		=	2.69E-12 F
#define CKMD_HFXTDYN_Q2CAP_W                                                 6U
#define CKMD_HFXTDYN_Q2CAP_M                                        0x00000FC0U
#define CKMD_HFXTDYN_Q2CAP_S                                                 6U
#define CKMD_HFXTDYN_Q2CAP_CAP63                                    0x00000FC0U
#define CKMD_HFXTDYN_Q2CAP_CAP62                                    0x00000F80U
#define CKMD_HFXTDYN_Q2CAP_CAP61                                    0x00000F40U
#define CKMD_HFXTDYN_Q2CAP_CAP60                                    0x00000F00U
#define CKMD_HFXTDYN_Q2CAP_CAP59                                    0x00000EC0U
#define CKMD_HFXTDYN_Q2CAP_CAP58                                    0x00000E80U
#define CKMD_HFXTDYN_Q2CAP_CAP57                                    0x00000E40U
#define CKMD_HFXTDYN_Q2CAP_CAP56                                    0x00000E00U
#define CKMD_HFXTDYN_Q2CAP_CAP55                                    0x00000DC0U
#define CKMD_HFXTDYN_Q2CAP_CAP54                                    0x00000D80U
#define CKMD_HFXTDYN_Q2CAP_CAP53                                    0x00000D40U
#define CKMD_HFXTDYN_Q2CAP_CAP52                                    0x00000D00U
#define CKMD_HFXTDYN_Q2CAP_CAP51                                    0x00000CC0U
#define CKMD_HFXTDYN_Q2CAP_CAP50                                    0x00000C80U
#define CKMD_HFXTDYN_Q2CAP_CAP49                                    0x00000C40U
#define CKMD_HFXTDYN_Q2CAP_CAP48                                    0x00000C00U
#define CKMD_HFXTDYN_Q2CAP_CAP47                                    0x00000BC0U
#define CKMD_HFXTDYN_Q2CAP_CAP46                                    0x00000B80U
#define CKMD_HFXTDYN_Q2CAP_CAP45                                    0x00000B40U
#define CKMD_HFXTDYN_Q2CAP_CAP44                                    0x00000B00U
#define CKMD_HFXTDYN_Q2CAP_CAP43                                    0x00000AC0U
#define CKMD_HFXTDYN_Q2CAP_CAP42                                    0x00000A80U
#define CKMD_HFXTDYN_Q2CAP_CAP41                                    0x00000A40U
#define CKMD_HFXTDYN_Q2CAP_CAP40                                    0x00000A00U
#define CKMD_HFXTDYN_Q2CAP_CAP39                                    0x000009C0U
#define CKMD_HFXTDYN_Q2CAP_CAP38                                    0x00000980U
#define CKMD_HFXTDYN_Q2CAP_CAP37                                    0x00000940U
#define CKMD_HFXTDYN_Q2CAP_CAP36                                    0x00000900U
#define CKMD_HFXTDYN_Q2CAP_CAP35                                    0x000008C0U
#define CKMD_HFXTDYN_Q2CAP_CAP34                                    0x00000880U
#define CKMD_HFXTDYN_Q2CAP_CAP33                                    0x00000840U
#define CKMD_HFXTDYN_Q2CAP_CAP32                                    0x00000800U
#define CKMD_HFXTDYN_Q2CAP_CAP31                                    0x000007C0U
#define CKMD_HFXTDYN_Q2CAP_CAP30                                    0x00000780U
#define CKMD_HFXTDYN_Q2CAP_CAP29                                    0x00000740U
#define CKMD_HFXTDYN_Q2CAP_CAP28                                    0x00000700U
#define CKMD_HFXTDYN_Q2CAP_CAP27                                    0x000006C0U
#define CKMD_HFXTDYN_Q2CAP_CAP26                                    0x00000680U
#define CKMD_HFXTDYN_Q2CAP_CAP25                                    0x00000640U
#define CKMD_HFXTDYN_Q2CAP_CAP24                                    0x00000600U
#define CKMD_HFXTDYN_Q2CAP_CAP23                                    0x000005C0U
#define CKMD_HFXTDYN_Q2CAP_CAP22                                    0x00000580U
#define CKMD_HFXTDYN_Q2CAP_CAP21                                    0x00000540U
#define CKMD_HFXTDYN_Q2CAP_CAP20                                    0x00000500U
#define CKMD_HFXTDYN_Q2CAP_CAP19                                    0x000004C0U
#define CKMD_HFXTDYN_Q2CAP_CAP18                                    0x00000480U
#define CKMD_HFXTDYN_Q2CAP_CAP17                                    0x00000440U
#define CKMD_HFXTDYN_Q2CAP_CAP16                                    0x00000400U
#define CKMD_HFXTDYN_Q2CAP_CAP15                                    0x000003C0U
#define CKMD_HFXTDYN_Q2CAP_CAP14                                    0x00000380U
#define CKMD_HFXTDYN_Q2CAP_CAP13                                    0x00000340U
#define CKMD_HFXTDYN_Q2CAP_CAP12                                    0x00000300U
#define CKMD_HFXTDYN_Q2CAP_CAP11                                    0x000002C0U
#define CKMD_HFXTDYN_Q2CAP_CAP10                                    0x00000280U
#define CKMD_HFXTDYN_Q2CAP_CAP9                                     0x00000240U
#define CKMD_HFXTDYN_Q2CAP_CAP8                                     0x00000200U
#define CKMD_HFXTDYN_Q2CAP_CAP7                                     0x000001C0U
#define CKMD_HFXTDYN_Q2CAP_CAP6                                     0x00000180U
#define CKMD_HFXTDYN_Q2CAP_CAP5                                     0x00000140U
#define CKMD_HFXTDYN_Q2CAP_CAP4                                     0x00000100U
#define CKMD_HFXTDYN_Q2CAP_CAP3                                     0x000000C0U
#define CKMD_HFXTDYN_Q2CAP_CAP2                                     0x00000080U
#define CKMD_HFXTDYN_Q2CAP_CAP1                                     0x00000040U
#define CKMD_HFXTDYN_Q2CAP_CAP0                                     0x00000000U

// Field:   [5:0] Q1CAP
//
// Target HFXT Q1 cap trim
// ENUMs:
// CAP63                    Nominal 25C	=	1.10E-11 F
//                          Stong 25C		=	1.00E-11 F
//                          Weak 25C		=	1.19E-11 F
// CAP62                    Nominal 25C	=	1.08E-11 F
//                          Stong 25C		=	9.82E-12 F
//                          Weak 25C		=	1.17E-11 F
// CAP61                    Nominal 25C	=	1.06E-11 F
//                          Stong 25C		=	9.64E-12 F
//                          Weak 25C		=	1.15E-11 F
// CAP60                    Nominal 25C	=	1.04E-11 F
//                          Stong 25C		=	9.45E-12 F
//                          Weak 25C		=	1.13E-11 F
// CAP59                    Nominal 25C	=	1.02E-11 F
//                          Stong 25C		=	9.26E-12 F
//                          Weak 25C		=	1.11E-11 F
// CAP58                    Nominal 25C	=	1.00E-11 F
//                          Stong 25C		=	9.07E-12 F
//                          Weak 25C		=	1.09E-11 F
// CAP57                    Nominal 25C	=	9.84E-12 F
//                          Stong 25C		=	8.89E-12 F
//                          Weak 25C		=	1.07E-11 F
// CAP56                    Nominal 25C	=	9.65E-12 F
//                          Stong 25C		=	8.70E-12 F
//                          Weak 25C		=	1.04E-11 F
// CAP55                    Nominal 25C	=	9.46E-12 F
//                          Stong 25C		=	8.51E-12 F
//                          Weak 25C		=	1.02E-11 F
// CAP54                    Nominal 25C	=	9.26E-12 F
//                          Stong 25C		=	8.32E-12 F
//                          Weak 25C		=	1.00E-11 F
// CAP53                    Nominal 25C	=	9.07E-12 F
//                          Stong 25C		=	8.13E-12 F
//                          Weak 25C		=	9.81E-12 F
// CAP52                    Nominal 25C	=	8.88E-12 F
//                          Stong 25C		=	7.94E-12 F
//                          Weak 25C		=	9.60E-12 F
// CAP51                    Nominal 25C	=	8.69E-12 F
//                          Stong 25C		=	7.76E-12 F
//                          Weak 25C		=	9.39E-12 F
// CAP50                    Nominal 25C	=	8.49E-12 F
//                          Stong 25C		=	7.57E-12 F
//                          Weak 25C		=	9.18E-12 F
// CAP49                    Nominal 25C	=	8.30E-12 F
//                          Stong 25C		=	7.38E-12 F
//                          Weak 25C		=	8.96E-12 F
// CAP48                    Nominal 25C	=	8.10E-12 F
//                          Stong 25C		=	7.18E-12 F
//                          Weak 25C		=	8.75E-12 F
// CAP47                    Nominal 25C	=	8.10E-12 F
//                          Stong 25C		=	7.18E-12 F
//                          Weak 25C		=	8.75E-12 F
// CAP46                    Nominal 25C	=	7.97E-12 F
//                          Stong 25C		=	7.05E-12 F
//                          Weak 25C		=	8.60E-12 F
// CAP45                    Nominal 25C	=	7.83E-12 F
//                          Stong 25C		=	6.91E-12 F
//                          Weak 25C		=	8.44E-12 F
// CAP44                    Nominal 25C	=	7.69E-12 F
//                          Stong 25C		=	6.77E-12 F
//                          Weak 25C		=	8.29E-12 F
// CAP43                    Nominal 25C	=	7.55E-12 F
//                          Stong 25C		=	6.63E-12 F
//                          Weak 25C		=	8.13E-12 F
// CAP42                    Nominal 25C	=	7.40E-12 F
//                          Stong 25C		=	6.49E-12 F
//                          Weak 25C		=	7.98E-12 F
// CAP41                    Nominal 25C	=	7.26E-12 F
//                          Stong 25C		=	6.35E-12 F
//                          Weak 25C		=	7.82E-12 F
// CAP40                    Nominal 25C	=	7.12E-12 F
//                          Stong 25C		=	6.21E-12 F
//                          Weak 25C		=	7.67E-12 F
// CAP39                    Nominal 25C	=	6.98E-12 F
//                          Stong 25C		=	6.06E-12 F
//                          Weak 25C		=	7.51E-12 F
// CAP38                    Nominal 25C	=	6.84E-12 F
//                          Stong 25C		=	5.92E-12 F
//                          Weak 25C		=	7.35E-12 F
// CAP37                    Nominal 25C	=	6.69E-12 F
//                          Stong 25C		=	5.78E-12 F
//                          Weak 25C		=	7.20E-12 F
// CAP36                    Nominal 25C	=	6.55E-12 F
//                          Stong 25C		=	5.63E-12 F
//                          Weak 25C		=	7.04E-12 F
// CAP35                    Nominal 25C	=	6.41E-12 F
//                          Stong 25C		=	5.49E-12 F
//                          Weak 25C		=	6.88E-12 F
// CAP34                    Nominal 25C	=	6.26E-12 F
//                          Stong 25C		=	5.35E-12 F
//                          Weak 25C		=	6.72E-12 F
// CAP33                    Nominal 25C	=	6.12E-12 F
//                          Stong 25C		=	5.20E-12 F
//                          Weak 25C		=	6.56E-12 F
// CAP32                    Nominal 25C	=	5.97E-12 F
//                          Stong 25C		=	5.05E-12 F
//                          Weak 25C		=	6.40E-12 F
// CAP31                    Nominal 25C	=	5.72E-12 F
//                          Stong 25C		=	4.79E-12 F
//                          Weak 25C		=	6.12E-12 F
// CAP30                    Nominal 25C	=	5.60E-12 F
//                          Stong 25C		=	4.67E-12 F
//                          Weak 25C		=	5.99E-12 F
// CAP29                    Nominal 25C	=	5.49E-12 F
//                          Stong 25C		=	4.56E-12 F
//                          Weak 25C		=	5.87E-12 F
// CAP28                    Nominal 25C	=	5.37E-12 F
//                          Stong 25C		=	4.44E-12 F
//                          Weak 25C		=	5.74E-12 F
// CAP27                    Nominal 25C	=	5.26E-12 F
//                          Stong 25C		=	4.32E-12 F
//                          Weak 25C		=	5.61E-12 F
// CAP26                    Nominal 25C	=	5.15E-12 F
//                          Stong 25C		=	4.21E-12 F
//                          Weak 25C		=	5.49E-12 F
// CAP25                    Nominal 25C	=	5.03E-12 F
//                          Stong 25C		=	4.09E-12 F
//                          Weak 25C		=	5.36E-12 F
// CAP24                    Nominal 25C	=	4.91E-12 F
//                          Stong 25C		=	3.97E-12 F
//                          Weak 25C		=	5.23E-12 F
// CAP23                    Nominal 25C	=	4.80E-12 F
//                          Stong 25C		=	3.85E-12 F
//                          Weak 25C		=	5.10E-12 F
// CAP22                    Nominal 25C	=	4.68E-12 F
//                          Stong 25C		=	3.73E-12 F
//                          Weak 25C		=	4.98E-12 F
// CAP21                    Nominal 25C	=	4.57E-12 F
//                          Stong 25C		=	3.60E-12 F
//                          Weak 25C		=	4.85E-12 F
// CAP20                    Nominal 25C	=	4.45E-12 F
//                          Stong 25C		=	3.48E-12 F
//                          Weak 25C		=	4.72E-12 F
// CAP19                    Nominal 25C	=	4.33E-12 F
//                          Stong 25C		=	3.36E-12 F
//                          Weak 25C		=	4.59E-12 F
// CAP18                    Nominal 25C	=	4.21E-12 F
//                          Stong 25C		=	3.23E-12 F
//                          Weak 25C		=	4.46E-12 F
// CAP17                    Nominal 25C	=	4.09E-12 F
//                          Stong 25C		=	3.11E-12 F
//                          Weak 25C		=	4.33E-12 F
// CAP16                    Nominal 25C	=	3.97E-12 F
//                          Stong 25C		=	2.98E-12 F
//                          Weak 25C		=	4.20E-12 F
// CAP15                    Nominal 25C	=	3.97E-12 F
//                          Stong 25C		=	2.98E-12 F
//                          Weak 25C		=	4.20E-12 F
// CAP14                    Nominal 25C	=	3.88E-12 F
//                          Stong 25C		=	2.89E-12 F
//                          Weak 25C		=	4.10E-12 F
// CAP13                    Nominal 25C	=	3.79E-12 F
//                          Stong 25C		=	2.79E-12 F
//                          Weak 25C		=	4.00E-12 F
// CAP12                    Nominal 25C	=	3.70E-12 F
//                          Stong 25C		=	2.69E-12 F
//                          Weak 25C		=	3.90E-12 F
// CAP11                    Nominal 25C	=	3.61E-12 F
//                          Stong 25C		=	2.59E-12 F
//                          Weak 25C		=	3.80E-12 F
// CAP10                    Nominal 25C	=	3.51E-12 F
//                          Stong 25C		=	2.49E-12 F
//                          Weak 25C		=	3.70E-12 F
// CAP9                     Nominal 25C	=	3.42E-12 F
//                          Stong 25C		=	2.39E-12 F
//                          Weak 25C		=	3.60E-12 F
// CAP8                     Nominal 25C	=	3.33E-12 F
//                          Stong 25C		=	2.29E-12 F
//                          Weak 25C		=	3.50E-12 F
// CAP7                     Nominal 25C	=	3.23E-12 F
//                          Stong 25C		=	2.19E-12 F
//                          Weak 25C		=	3.40E-12 F
// CAP6                     Nominal 25C	=	3.14E-12 F
//                          Stong 25C		=	2.09E-12 F
//                          Weak 25C		=	3.30E-12 F
// CAP5                     Nominal 25C	=	3.04E-12 F
//                          Stong 25C		=	1.99E-12 F
//                          Weak 25C		=	3.19E-12 F
// CAP4                     Nominal 25C	=	2.95E-12 F
//                          Stong 25C		=	1.89E-12 F
//                          Weak 25C		=	3.09E-12 F
// CAP3                     Nominal 25C	=	2.85E-12 F
//                          Stong 25C		=	1.79E-12 F
//                          Weak 25C		=	2.99E-12 F
// CAP2                     Nominal 25C	=	2.76E-12 F
//                          Stong 25C		=	1.69E-12 F
//                          Weak 25C		=	2.89E-12 F
// CAP1                     Nominal 25C	=	2.66E-12 F
//                          Stong 25C		=	1.60E-12 F
//                          Weak 25C		=	2.79E-12 F
// CAP0                     Nominal 25C	=	2.57E-12 F
//                          Stong 25C		=	1.50E-12 F
//                          Weak 25C		=	2.69E-12 F
#define CKMD_HFXTDYN_Q1CAP_W                                                 6U
#define CKMD_HFXTDYN_Q1CAP_M                                        0x0000003FU
#define CKMD_HFXTDYN_Q1CAP_S                                                 0U
#define CKMD_HFXTDYN_Q1CAP_CAP63                                    0x0000003FU
#define CKMD_HFXTDYN_Q1CAP_CAP62                                    0x0000003EU
#define CKMD_HFXTDYN_Q1CAP_CAP61                                    0x0000003DU
#define CKMD_HFXTDYN_Q1CAP_CAP60                                    0x0000003CU
#define CKMD_HFXTDYN_Q1CAP_CAP59                                    0x0000003BU
#define CKMD_HFXTDYN_Q1CAP_CAP58                                    0x0000003AU
#define CKMD_HFXTDYN_Q1CAP_CAP57                                    0x00000039U
#define CKMD_HFXTDYN_Q1CAP_CAP56                                    0x00000038U
#define CKMD_HFXTDYN_Q1CAP_CAP55                                    0x00000037U
#define CKMD_HFXTDYN_Q1CAP_CAP54                                    0x00000036U
#define CKMD_HFXTDYN_Q1CAP_CAP53                                    0x00000035U
#define CKMD_HFXTDYN_Q1CAP_CAP52                                    0x00000034U
#define CKMD_HFXTDYN_Q1CAP_CAP51                                    0x00000033U
#define CKMD_HFXTDYN_Q1CAP_CAP50                                    0x00000032U
#define CKMD_HFXTDYN_Q1CAP_CAP49                                    0x00000031U
#define CKMD_HFXTDYN_Q1CAP_CAP48                                    0x00000030U
#define CKMD_HFXTDYN_Q1CAP_CAP47                                    0x0000002FU
#define CKMD_HFXTDYN_Q1CAP_CAP46                                    0x0000002EU
#define CKMD_HFXTDYN_Q1CAP_CAP45                                    0x0000002DU
#define CKMD_HFXTDYN_Q1CAP_CAP44                                    0x0000002CU
#define CKMD_HFXTDYN_Q1CAP_CAP43                                    0x0000002BU
#define CKMD_HFXTDYN_Q1CAP_CAP42                                    0x0000002AU
#define CKMD_HFXTDYN_Q1CAP_CAP41                                    0x00000029U
#define CKMD_HFXTDYN_Q1CAP_CAP40                                    0x00000028U
#define CKMD_HFXTDYN_Q1CAP_CAP39                                    0x00000027U
#define CKMD_HFXTDYN_Q1CAP_CAP38                                    0x00000026U
#define CKMD_HFXTDYN_Q1CAP_CAP37                                    0x00000025U
#define CKMD_HFXTDYN_Q1CAP_CAP36                                    0x00000024U
#define CKMD_HFXTDYN_Q1CAP_CAP35                                    0x00000023U
#define CKMD_HFXTDYN_Q1CAP_CAP34                                    0x00000022U
#define CKMD_HFXTDYN_Q1CAP_CAP33                                    0x00000021U
#define CKMD_HFXTDYN_Q1CAP_CAP32                                    0x00000020U
#define CKMD_HFXTDYN_Q1CAP_CAP31                                    0x0000001FU
#define CKMD_HFXTDYN_Q1CAP_CAP30                                    0x0000001EU
#define CKMD_HFXTDYN_Q1CAP_CAP29                                    0x0000001DU
#define CKMD_HFXTDYN_Q1CAP_CAP28                                    0x0000001CU
#define CKMD_HFXTDYN_Q1CAP_CAP27                                    0x0000001BU
#define CKMD_HFXTDYN_Q1CAP_CAP26                                    0x0000001AU
#define CKMD_HFXTDYN_Q1CAP_CAP25                                    0x00000019U
#define CKMD_HFXTDYN_Q1CAP_CAP24                                    0x00000018U
#define CKMD_HFXTDYN_Q1CAP_CAP23                                    0x00000017U
#define CKMD_HFXTDYN_Q1CAP_CAP22                                    0x00000016U
#define CKMD_HFXTDYN_Q1CAP_CAP21                                    0x00000015U
#define CKMD_HFXTDYN_Q1CAP_CAP20                                    0x00000014U
#define CKMD_HFXTDYN_Q1CAP_CAP19                                    0x00000013U
#define CKMD_HFXTDYN_Q1CAP_CAP18                                    0x00000012U
#define CKMD_HFXTDYN_Q1CAP_CAP17                                    0x00000011U
#define CKMD_HFXTDYN_Q1CAP_CAP16                                    0x00000010U
#define CKMD_HFXTDYN_Q1CAP_CAP15                                    0x0000000FU
#define CKMD_HFXTDYN_Q1CAP_CAP14                                    0x0000000EU
#define CKMD_HFXTDYN_Q1CAP_CAP13                                    0x0000000DU
#define CKMD_HFXTDYN_Q1CAP_CAP12                                    0x0000000CU
#define CKMD_HFXTDYN_Q1CAP_CAP11                                    0x0000000BU
#define CKMD_HFXTDYN_Q1CAP_CAP10                                    0x0000000AU
#define CKMD_HFXTDYN_Q1CAP_CAP9                                     0x00000009U
#define CKMD_HFXTDYN_Q1CAP_CAP8                                     0x00000008U
#define CKMD_HFXTDYN_Q1CAP_CAP7                                     0x00000007U
#define CKMD_HFXTDYN_Q1CAP_CAP6                                     0x00000006U
#define CKMD_HFXTDYN_Q1CAP_CAP5                                     0x00000005U
#define CKMD_HFXTDYN_Q1CAP_CAP4                                     0x00000004U
#define CKMD_HFXTDYN_Q1CAP_CAP3                                     0x00000003U
#define CKMD_HFXTDYN_Q1CAP_CAP2                                     0x00000002U
#define CKMD_HFXTDYN_Q1CAP_CAP1                                     0x00000001U
#define CKMD_HFXTDYN_Q1CAP_CAP0                                     0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_AMPCFG0
//
//*****************************************************************************
// Field: [31:28] Q2DLY
//
// Q2CAP change delay.
//
// Number of clock cycles to wait before changing Q2CAP by one step.
// Clock frequency defined in FSMRATE.
#define CKMD_AMPCFG0_Q2DLY_W                                                 4U
#define CKMD_AMPCFG0_Q2DLY_M                                        0xF0000000U
#define CKMD_AMPCFG0_Q2DLY_S                                                28U

// Field: [27:24] Q1DLY
//
// Q1CAP change delay.
//
// Number of clock cycles to wait before changing Q1CAP by one step.
// Clock frequency defined in FSMRATE.
#define CKMD_AMPCFG0_Q1DLY_W                                                 4U
#define CKMD_AMPCFG0_Q1DLY_M                                        0x0F000000U
#define CKMD_AMPCFG0_Q1DLY_S                                                24U

// Field: [23:20] ADCDLY
//
// ADC and PEAKDET startup time.
//
// Number of clock cycles to wait after enabling the PEAKDET and ADC before the
// first measurement.
// Clock frequency defined in FSMRATE.
#define CKMD_AMPCFG0_ADCDLY_W                                                4U
#define CKMD_AMPCFG0_ADCDLY_M                                       0x00F00000U
#define CKMD_AMPCFG0_ADCDLY_S                                               20U

// Field: [19:15] LDOSTART
//
// LDO startup time.
//
// Number of clock cycles to bypass the LDO resistors for faster startup.
// Clock frequency defined in FSMRATE.
#define CKMD_AMPCFG0_LDOSTART_W                                              5U
#define CKMD_AMPCFG0_LDOSTART_M                                     0x000F8000U
#define CKMD_AMPCFG0_LDOSTART_S                                             15U

// Field: [14:10] INJWAIT
//
// Inject HFOSC for faster HFXT startup.
//
// This value specifies the number of clock cycles to wait after injection is
// done.
// The clock speed is defined in FSMRATE.
#define CKMD_AMPCFG0_INJWAIT_W                                               5U
#define CKMD_AMPCFG0_INJWAIT_M                                      0x00007C00U
#define CKMD_AMPCFG0_INJWAIT_S                                              10U

// Field:   [9:5] INJTIME
//
// Inject HFOSC for faster HFXT startup.
//
// This value specifies the number of clock cycles the injection is enabled.
// The clock speed is defined in FSMRATE.
// Set to 0 to disable injection.
#define CKMD_AMPCFG0_INJTIME_W                                               5U
#define CKMD_AMPCFG0_INJTIME_M                                      0x000003E0U
#define CKMD_AMPCFG0_INJTIME_S                                               5U

// Field:   [4:0] FSMRATE
//
// Update rate for the AMPCOMP update rate.
// Also affects the clock rate for the Amplitude ADC.
//
// The update rate is 6MHz / (FSMRATE+1).
// ENUMs:
// _250K                    250 kHz
// _500K                    500 kHz
// _1M                      1 MHz
// _2M                      2 MHz
// _3M                      3 MHz
// _6M                      6 MHz
#define CKMD_AMPCFG0_FSMRATE_W                                               5U
#define CKMD_AMPCFG0_FSMRATE_M                                      0x0000001FU
#define CKMD_AMPCFG0_FSMRATE_S                                               0U
#define CKMD_AMPCFG0_FSMRATE__250K                                  0x00000017U
#define CKMD_AMPCFG0_FSMRATE__500K                                  0x0000000BU
#define CKMD_AMPCFG0_FSMRATE__1M                                    0x00000005U
#define CKMD_AMPCFG0_FSMRATE__2M                                    0x00000002U
#define CKMD_AMPCFG0_FSMRATE__3M                                    0x00000001U
#define CKMD_AMPCFG0_FSMRATE__6M                                    0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_AMPCFG1
//
//*****************************************************************************
// Field: [31:28] IDACDLY
//
// IDAC change delay.
//
// Time to wait before changing IDAC by one step.
// This time needs to be long enough for the crystal to settle.
// The number of clock cycles to wait is IDACDLY<<4 + 15.
// Clock frequency defined in AMPCFG0.FSMRATE.
#define CKMD_AMPCFG1_IDACDLY_W                                               4U
#define CKMD_AMPCFG1_IDACDLY_M                                      0xF0000000U
#define CKMD_AMPCFG1_IDACDLY_S                                              28U

// Field: [27:24] IREFDLY
//
// IREF change delay.
//
// Number of clock cycles to wait before changing IREF by one step.
// Clock frequency defined in AMPCFG0.FSMRATE.
#define CKMD_AMPCFG1_IREFDLY_W                                               4U
#define CKMD_AMPCFG1_IREFDLY_M                                      0x0F000000U
#define CKMD_AMPCFG1_IREFDLY_S                                              24U

// Field: [23:12] BIASLT
//
// Lifetime of the amplitude ADC bias value.
// This value specifies the number of adjustment intervals,
// until the ADC bias value has to be measured again.
// Set to 0 to disable automatic bias measurements.
#define CKMD_AMPCFG1_BIASLT_W                                               12U
#define CKMD_AMPCFG1_BIASLT_M                                       0x00FFF000U
#define CKMD_AMPCFG1_BIASLT_S                                               12U

// Field:  [11:0] INTERVAL
//
// Interval for amplitude adjustments.
// Set to 0 to disable periodic adjustments.
//
// This value specifies the number of clock cycles between adjustments.
// The clock speed is defined in AMPCFG0.FSMRATE.
#define CKMD_AMPCFG1_INTERVAL_W                                             12U
#define CKMD_AMPCFG1_INTERVAL_M                                     0x00000FFFU
#define CKMD_AMPCFG1_INTERVAL_S                                              0U

//*****************************************************************************
//
// Register: CKMD_O_LOOPCFG
//
//*****************************************************************************
// Field: [31:26] FINETRIM_INIT
//
// Initial value for the resistor fine trim
#define CKMD_LOOPCFG_FINETRIM_INIT_W                                         6U
#define CKMD_LOOPCFG_FINETRIM_INIT_M                                0xFC000000U
#define CKMD_LOOPCFG_FINETRIM_INIT_S                                        26U

// Field: [25:21] BOOST_TARGET
//
// Error-updates for 4xBOOST_TARGET times using KI_BOOST/KP_BOOST, before using
// KI/KP.
// Note: If boost is used for long duration using large values of KI_BOOST &
// KP_BOOST, the oscillator frequency can reach well above the max frequence
// limit of the design, causing unexpected behaviour.
#define CKMD_LOOPCFG_BOOST_TARGET_W                                          5U
#define CKMD_LOOPCFG_BOOST_TARGET_M                                 0x03E00000U
#define CKMD_LOOPCFG_BOOST_TARGET_S                                         21U

// Field: [20:18] KP_BOOST
//
// Proportional loop coefficient during BOOST
#define CKMD_LOOPCFG_KP_BOOST_W                                              3U
#define CKMD_LOOPCFG_KP_BOOST_M                                     0x001C0000U
#define CKMD_LOOPCFG_KP_BOOST_S                                             18U

// Field: [17:15] KI_BOOST
//
// Integral loop coefficient during BOOST
#define CKMD_LOOPCFG_KI_BOOST_W                                              3U
#define CKMD_LOOPCFG_KI_BOOST_M                                     0x00038000U
#define CKMD_LOOPCFG_KI_BOOST_S                                             15U

// Field: [14:10] SETTLED_TARGET
//
// Number of HFOSC tracking loop updates before HFOSC is considered "settled".
// The tracking loop updates at a rough frequency of (2*Reference
// frequency/256).
// If the reference frequency is 48MHz, the loop update frequency comes out to
// be 375Khz.
// Internally the MMR is multiplied by 4
#define CKMD_LOOPCFG_SETTLED_TARGET_W                                        5U
#define CKMD_LOOPCFG_SETTLED_TARGET_M                               0x00007C00U
#define CKMD_LOOPCFG_SETTLED_TARGET_S                                       10U

// Field:   [9:6] OOR_LIMIT
//
// Out-of-range threshold. OOR_LIMIT is compared with absolute value of 5 MSB
// bits of loop filter error.
#define CKMD_LOOPCFG_OOR_LIMIT_W                                             4U
#define CKMD_LOOPCFG_OOR_LIMIT_M                                    0x000003C0U
#define CKMD_LOOPCFG_OOR_LIMIT_S                                             6U

// Field:   [5:3] KP
//
// Proportional loop coefficient
#define CKMD_LOOPCFG_KP_W                                                    3U
#define CKMD_LOOPCFG_KP_M                                           0x00000038U
#define CKMD_LOOPCFG_KP_S                                                    3U

// Field:   [2:0] KI
//
// Integral loop coefficient
#define CKMD_LOOPCFG_KI_W                                                    3U
#define CKMD_LOOPCFG_KI_M                                           0x00000007U
#define CKMD_LOOPCFG_KI_S                                                    0U

//*****************************************************************************
//
// Register: CKMD_O_LOOPCFG1
//
//*****************************************************************************
// Field:    [25] SETTLEIRQ
//
// Configuration to enable the interrupt when the HFOSC tracking loop has
// settled. The interrupt will be based on LOOPCFG.SETTLED_TARGET.
// ENUMs:
// ENABLE                   Enable the interrupt to indicate that HFOSC
//                          tracking loop has settled.
// DISABLE                  Disable the interrupt to indicate that HFOSC
//                          tracking loop has settled.
#define CKMD_LOOPCFG1_SETTLEIRQ                                     0x02000000U
#define CKMD_LOOPCFG1_SETTLEIRQ_M                                   0x02000000U
#define CKMD_LOOPCFG1_SETTLEIRQ_S                                           25U
#define CKMD_LOOPCFG1_SETTLEIRQ_ENABLE                              0x02000000U
#define CKMD_LOOPCFG1_SETTLEIRQ_DISABLE                             0x00000000U

// Field:  [24:6] UNDERCLKCNT
//
// Timer to trigger HFOSC underclocking. The timer will run at approximately
// 32.768 KHz.
#define CKMD_LOOPCFG1_UNDERCLKCNT_W                                         19U
#define CKMD_LOOPCFG1_UNDERCLKCNT_M                                 0x01FFFFC0U
#define CKMD_LOOPCFG1_UNDERCLKCNT_S                                          6U

// Field:   [5:0] KIOFF
//
// Based on HFTRACKCTRL.UNDERCLK configuration, after an event is triggerred,
// KI of the HFOSC tracking loop will be reduced by this amount.
#define CKMD_LOOPCFG1_KIOFF_W                                                6U
#define CKMD_LOOPCFG1_KIOFF_M                                       0x0000003FU
#define CKMD_LOOPCFG1_KIOFF_S                                                0U

//*****************************************************************************
//
// Register: CKMD_O_AFOSCCTL
//
//*****************************************************************************
// Field: [31:24] PW
//
// Password protection for QUALBYP.
//
// Write this field to 0xA5 to accept writes to QUALBYP.
#define CKMD_AFOSCCTL_PW_W                                                   8U
#define CKMD_AFOSCCTL_PW_M                                          0xFF000000U
#define CKMD_AFOSCCTL_PW_S                                                  24U

// Field:     [2] AUTODIS
//
// If set, AFOSC can be disabled by PMCTL upon standby entry. EN bit will be
// overriden with a value 0 and user has to manually re-enable AFOSC.
#define CKMD_AFOSCCTL_AUTODIS                                       0x00000004U
#define CKMD_AFOSCCTL_AUTODIS_M                                     0x00000004U
#define CKMD_AFOSCCTL_AUTODIS_S                                              2U

// Field:     [1] QUALBYP
//
// Clock qualification bypass.
//
// AFOSC qualification will skip a fixed number of clock cycles to prevent
// glitches
// or frequency overshoots from reaching the system. Setting this bit will
// bypass the qualification.
// This bit can be locked in SYS0. If unlocked, it is password protected with
// PW.
#define CKMD_AFOSCCTL_QUALBYP                                       0x00000002U
#define CKMD_AFOSCCTL_QUALBYP_M                                     0x00000002U
#define CKMD_AFOSCCTL_QUALBYP_S                                              1U

// Field:     [0] EN
//
// Enable AFOSC.
#define CKMD_AFOSCCTL_EN                                            0x00000001U
#define CKMD_AFOSCCTL_EN_M                                          0x00000001U
#define CKMD_AFOSCCTL_EN_S                                                   0U

//*****************************************************************************
//
// Register: CKMD_O_AFTRACKCTL
//
//*****************************************************************************
// Field:    [31] EN
//
// Enable tracking loop.
#define CKMD_AFTRACKCTL_EN                                          0x80000000U
#define CKMD_AFTRACKCTL_EN_M                                        0x80000000U
#define CKMD_AFTRACKCTL_EN_S                                                31U

// Field:    [30] DSMBYP
//
// Bypass Delta-Sigma-Modulation of fine trim.
#define CKMD_AFTRACKCTL_DSMBYP                                      0x40000000U
#define CKMD_AFTRACKCTL_DSMBYP_M                                    0x40000000U
#define CKMD_AFTRACKCTL_DSMBYP_S                                            30U

// Field:  [29:0] RATIO
//
// Ratio. Ratio format is 0b.30b
// ENUMs:
// _79MHZ
// _80MHZ
// _86MHZ
// _90P288MHZ
// _98MHZ
#define CKMD_AFTRACKCTL_RATIO_W                                             30U
#define CKMD_AFTRACKCTL_RATIO_M                                     0x3FFFFFFFU
#define CKMD_AFTRACKCTL_RATIO_S                                              0U
#define CKMD_AFTRACKCTL_RATIO__79MHZ                                0x09B8B578U
#define CKMD_AFTRACKCTL_RATIO__80MHZ                                0x0999999AU
#define CKMD_AFTRACKCTL_RATIO__86MHZ                                0x08EE23B9U
#define CKMD_AFTRACKCTL_RATIO__90P288MHZ                            0x088190ACU
#define CKMD_AFTRACKCTL_RATIO__98MHZ                                0x07D6343FU

//*****************************************************************************
//
// Register: CKMD_O_BANDGAPCTL
//
//*****************************************************************************
// Field:    [31] BGOVR
//
// Internal. Only to be used through TI provided API.
#define CKMD_BANDGAPCTL_BGOVR                                       0x80000000U
#define CKMD_BANDGAPCTL_BGOVR_M                                     0x80000000U
#define CKMD_BANDGAPCTL_BGOVR_S                                             31U

// Field:     [3] VBGAPBYP
//
// Internal. Only to be used through TI provided API.
#define CKMD_BANDGAPCTL_VBGAPBYP                                    0x00000008U
#define CKMD_BANDGAPCTL_VBGAPBYP_M                                  0x00000008U
#define CKMD_BANDGAPCTL_VBGAPBYP_S                                           3U

// Field:     [2] VBGAPREFEN
//
// Internal. Only to be used through TI provided API.
#define CKMD_BANDGAPCTL_VBGAPREFEN                                  0x00000004U
#define CKMD_BANDGAPCTL_VBGAPREFEN_M                                0x00000004U
#define CKMD_BANDGAPCTL_VBGAPREFEN_S                                         2U

// Field:     [1] VDDRREFEN
//
// Internal. Only to be used through TI provided API.
#define CKMD_BANDGAPCTL_VDDRREFEN                                   0x00000002U
#define CKMD_BANDGAPCTL_VDDRREFEN_M                                 0x00000002U
#define CKMD_BANDGAPCTL_VDDRREFEN_S                                          1U

// Field:     [0] REFEN
//
// Internal. Only to be used through TI provided API.
#define CKMD_BANDGAPCTL_REFEN                                       0x00000001U
#define CKMD_BANDGAPCTL_REFEN_M                                     0x00000001U
#define CKMD_BANDGAPCTL_REFEN_S                                              0U

//*****************************************************************************
//
// Register: CKMD_O_AFCLKSEL
//
//*****************************************************************************
// Field:   [2:0] SRC
//
// Select audio frequency clock source
//
// Software should make sure that proper clock is selected before enabling the
// audio IP.
// ENUMs:
// CLKEXT                   External clock
// CLKREF                   48MHz reference clock (HFXT)
// CLKHF                    96MHz CLKHF
// CLKAF                    AFOSC clock
// DIS                      Clock disabled
#define CKMD_AFCLKSEL_SRC_W                                                  3U
#define CKMD_AFCLKSEL_SRC_M                                         0x00000007U
#define CKMD_AFCLKSEL_SRC_S                                                  0U
#define CKMD_AFCLKSEL_SRC_CLKEXT                                    0x00000004U
#define CKMD_AFCLKSEL_SRC_CLKREF                                    0x00000003U
#define CKMD_AFCLKSEL_SRC_CLKHF                                     0x00000002U
#define CKMD_AFCLKSEL_SRC_CLKAF                                     0x00000001U
#define CKMD_AFCLKSEL_SRC_DIS                                       0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_CANCLKSEL
//
//*****************************************************************************
// Field:   [1:0] SRC
//
// Select audio frequency clock source
//
// Software should make sure that proper clock is selected before enabling the
// audio IP.
// ENUMs:
// CLKHF                    96MHz CLKHF
// CLKAF                    AFOSC clock
// DIS                      Clock disabled
#define CKMD_CANCLKSEL_SRC_W                                                 2U
#define CKMD_CANCLKSEL_SRC_M                                        0x00000003U
#define CKMD_CANCLKSEL_SRC_S                                                 0U
#define CKMD_CANCLKSEL_SRC_CLKHF                                    0x00000002U
#define CKMD_CANCLKSEL_SRC_CLKAF                                    0x00000001U
#define CKMD_CANCLKSEL_SRC_DIS                                      0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_TRACKSTATAF
//
//*****************************************************************************
// Field:    [31] LOOPERRVLD
//
// Current AFOSC tracking error valid
//
// This bit is one if the tracking loop is running and the error value is
// valid.
#define CKMD_TRACKSTATAF_LOOPERRVLD                                 0x80000000U
#define CKMD_TRACKSTATAF_LOOPERRVLD_M                               0x80000000U
#define CKMD_TRACKSTATAF_LOOPERRVLD_S                                       31U

// Field: [29:16] LOOPERR
//
// Current AFOSC tracking error
//
// This field uses the internal fractional representation (sign, 9 integer
// bits, 4 fractional bits).
// The actual fine trim value of format (sign, 9 integer bits, 30 fractional
// bits) is saturated to (sign, 9 integer bits, 4 fractional bits).
#define CKMD_TRACKSTATAF_LOOPERR_W                                          14U
#define CKMD_TRACKSTATAF_LOOPERR_M                                  0x3FFF0000U
#define CKMD_TRACKSTATAF_LOOPERR_S                                          16U

// Field:  [12:0] FINETRIM
//
// Current AFOSC Fine-trim value
//
// This field uses the internal fractional representation (sign, 5 integer
// bits, 7 fractional bits).
// The actual fine trim value of format (sign, 5 integer bits, 19 fractional
// bits) is saturated to (sign, 5 integer bits, 7 fractional bits).
// The actual trim value applied to the oscillator is delta-sigma modulated 6
// bits non-signed
// (inverted sign bit + integer bits).
#define CKMD_TRACKSTATAF_FINETRIM_W                                         13U
#define CKMD_TRACKSTATAF_FINETRIM_M                                 0x00001FFFU
#define CKMD_TRACKSTATAF_FINETRIM_S                                          0U

//*****************************************************************************
//
// Register: CKMD_O_TRACKSTATAF1
//
//*****************************************************************************
// Field:  [29:0] LOOPERR
//
// Current AFOSC tracking error
//
// This field uses the fractional representation of the actual error(30
// fractional bits). The actual error is of format (sign, 9 integer bits, 30
// fractional bits).
#define CKMD_TRACKSTATAF1_LOOPERR_W                                         30U
#define CKMD_TRACKSTATAF1_LOOPERR_M                                 0x3FFFFFFFU
#define CKMD_TRACKSTATAF1_LOOPERR_S                                          0U

//*****************************************************************************
//
// Register: CKMD_O_TRACKSTATAF2
//
//*****************************************************************************
// Field:  [24:0] FINETRIM
//
// Current AFOSC Fine-trim value
//
// This field uses the internal fractional representation (sign, 5 integer
// bits, 19 fractional bits).
// The actual trim value applied to the oscillator is delta-sigma modulated 6
// bits non-signed
// (inverted sign bit + integer bits).
#define CKMD_TRACKSTATAF2_FINETRIM_W                                        25U
#define CKMD_TRACKSTATAF2_FINETRIM_M                                0x01FFFFFFU
#define CKMD_TRACKSTATAF2_FINETRIM_S                                         0U

//*****************************************************************************
//
// Register: CKMD_O_LOOPCFGAF
//
//*****************************************************************************
// Field: [31:26] FINETRIM_INIT
//
// Initial value for the resistor fine trim
#define CKMD_LOOPCFGAF_FINETRIM_INIT_W                                       6U
#define CKMD_LOOPCFGAF_FINETRIM_INIT_M                              0xFC000000U
#define CKMD_LOOPCFGAF_FINETRIM_INIT_S                                      26U

// Field: [25:21] BOOST_TARGET
//
// Number of error-updates using BOOST values, before using KI/KP
#define CKMD_LOOPCFGAF_BOOST_TARGET_W                                        5U
#define CKMD_LOOPCFGAF_BOOST_TARGET_M                               0x03E00000U
#define CKMD_LOOPCFGAF_BOOST_TARGET_S                                       21U

// Field: [20:18] KP_BOOST
//
// Proportional loop coefficient during BOOST
#define CKMD_LOOPCFGAF_KP_BOOST_W                                            3U
#define CKMD_LOOPCFGAF_KP_BOOST_M                                   0x001C0000U
#define CKMD_LOOPCFGAF_KP_BOOST_S                                           18U

// Field: [17:15] KI_BOOST
//
// Integral loop coefficient during BOOST
#define CKMD_LOOPCFGAF_KI_BOOST_W                                            3U
#define CKMD_LOOPCFGAF_KI_BOOST_M                                   0x00038000U
#define CKMD_LOOPCFGAF_KI_BOOST_S                                           15U

// Field: [14:10] SETTLED_TARGET
//
// Number of updates before AFOSC is considered "settled"
#define CKMD_LOOPCFGAF_SETTLED_TARGET_W                                      5U
#define CKMD_LOOPCFGAF_SETTLED_TARGET_M                             0x00007C00U
#define CKMD_LOOPCFGAF_SETTLED_TARGET_S                                     10U

// Field:   [9:6] OOR_LIMIT
//
// Out-of-range threshold. Out-of-range threshold. OOR_LIMIT is compared with
// absolute value of 5 MSB bits of loop filter error.
#define CKMD_LOOPCFGAF_OOR_LIMIT_W                                           4U
#define CKMD_LOOPCFGAF_OOR_LIMIT_M                                  0x000003C0U
#define CKMD_LOOPCFGAF_OOR_LIMIT_S                                           6U

// Field:   [5:3] KP
//
// Proportional loop coefficient
#define CKMD_LOOPCFGAF_KP_W                                                  3U
#define CKMD_LOOPCFGAF_KP_M                                         0x00000038U
#define CKMD_LOOPCFGAF_KP_S                                                  3U

// Field:   [2:0] KI
//
// Integral loop coefficient
#define CKMD_LOOPCFGAF_KI_W                                                  3U
#define CKMD_LOOPCFGAF_KI_M                                         0x00000007U
#define CKMD_LOOPCFGAF_KI_S                                                  0U

//*****************************************************************************
//
// Register: CKMD_O_CTL
//
//*****************************************************************************
// Field:   [1:0] CMD
//
// TDC commands.
// ENUMs:
// ABORT                    Force TDC state machine back to IDLE state.
//
//                          Never write this command
//                          while [TDC.STAT.STATE] equals CLR_CNT or
//                          WAIT_CLR_CNT_DONE.
// RUN                      Asynchronous counter start.
//
//                          The counter starts to
//                          count when the start event is high. To achieve
//                          precise edge-to-edge measurements you must
//                          ensure that the start event is low for at least
//                          420 ns after you write this command.
// RUN_SYNC_START           Synchronous counter start.
//
//                          The counter looks for the
//                          opposite edge of the selected start event
//                          before it starts to count when the selected
//                          edge occurs. This guarantees an edge-triggered
//                          start and is recommended for frequency
//                          measurements.
// CLR_RESULT               Clear [TDC.STAT.SAT], [TDC.STAT.DONE], and
//                          [TDC.RESULT.VALUE].
//
//                          This is not needed as
//                          prerequisite for a measurement. Reliable clear
//                          is only guaranteed from IDLE state.
#define CKMD_CTL_CMD_W                                                       2U
#define CKMD_CTL_CMD_M                                              0x00000003U
#define CKMD_CTL_CMD_S                                                       0U
#define CKMD_CTL_CMD_ABORT                                          0x00000003U
#define CKMD_CTL_CMD_RUN                                            0x00000002U
#define CKMD_CTL_CMD_RUN_SYNC_START                                 0x00000001U
#define CKMD_CTL_CMD_CLR_RESULT                                     0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_STAT
//
//*****************************************************************************
// Field:     [7] SAT
//
// TDC measurement saturation flag.
//
// 0: Conversion has not saturated.
// 1: Conversion stopped due to saturation.
//
// This field is cleared when a new measurement is started or when CLR_RESULT
// is written to [TDC.CTL.CMD].
#define CKMD_STAT_SAT                                               0x00000080U
#define CKMD_STAT_SAT_M                                             0x00000080U
#define CKMD_STAT_SAT_S                                                      7U

// Field:     [6] DONE
//
// TDC measurement complete flag.
//
// 0: TDC measurement has not yet completed.
// 1: TDC measurement has completed.
//
// This field clears when a new TDC measurement starts or when you write
// CLR_RESULT to [TDC.CTL.CMD].
#define CKMD_STAT_DONE                                              0x00000040U
#define CKMD_STAT_DONE_M                                            0x00000040U
#define CKMD_STAT_DONE_S                                                     6U

// Field:   [5:0] STATE
//
// TDC state machine status.
// ENUMs:
// FORCE_STOP               Current state is TDC_FORCESTOP.
//                          You wrote ABORT to
//                          [TDC.CTL.CMD] to abort the TDC measurement.
// START_FALL               Current state is TDC_WAIT_STARTFALL.
//                          The fast-counter circuit
//                          waits for a falling edge on the start event.
// WAIT_CLR_CNT_DONE        Current state is TDC_STATE_WAIT_CLRCNT_DONE.
//                          The state machine waits
//                          for fast-counter circuit to finish reset.
// POR                      Current state is TDC_STATE_POR.
//                          This is the reset state.
// GET_RESULT               Current state is TDC_STATE_GETRESULTS.
//                          The state machine copies
//                          the counter value from the fast-counter
//                          circuit.
// WAIT_STOP_CNTDWN         Current state is TDC_STATE_WAIT_STOPCNTDOWN.
//                          The fast-counter circuit
//                          looks for the stop condition. It will ignore a
//                          number of stop events configured in
//                          [TDC.TRIGCNTLOAD.CNT].
// WAIT_STOP                Current state is TDC_STATE_WAIT_STOP.
//                          The state machine waits
//                          for the fast-counter circuit to stop.
// CLR_CNT                  Current state is TDC_STATE_CLRCNT. The
//                          fast-counter circuit is reset.
// IDLE                     Current state is TDC_STATE_IDLE.
//                          This is the default state
//                          after reset and abortion. State will change
//                          when you write [TDC.CTL.CMD] to either
//                          RUN_SYNC_START or RUN.
// WAIT_START_STOP_CNT_EN   Current state is TDC_STATE_WAIT_STARTSTOPCNTEN.
//                          The fast-counter circuit
//                          looks for the start condition. The state
//                          machine waits for the fast-counter to
//                          increment.
// WAIT_START               Current state is TDC_STATE_WAIT_START.
//                          The fast-counter circuit
//                          looks for the start condition. The state
//                          machine waits for the fast-counter to
//                          increment.
#define CKMD_STAT_STATE_W                                                    6U
#define CKMD_STAT_STATE_M                                           0x0000003FU
#define CKMD_STAT_STATE_S                                                    0U
#define CKMD_STAT_STATE_FORCE_STOP                                  0x0000002EU
#define CKMD_STAT_STATE_START_FALL                                  0x0000001EU
#define CKMD_STAT_STATE_WAIT_CLR_CNT_DONE                           0x00000016U
#define CKMD_STAT_STATE_POR                                         0x0000000FU
#define CKMD_STAT_STATE_GET_RESULT                                  0x0000000EU
#define CKMD_STAT_STATE_WAIT_STOP_CNTDWN                            0x0000000CU
#define CKMD_STAT_STATE_WAIT_STOP                                   0x00000008U
#define CKMD_STAT_STATE_CLR_CNT                                     0x00000007U
#define CKMD_STAT_STATE_IDLE                                        0x00000006U
#define CKMD_STAT_STATE_WAIT_START_STOP_CNT_EN                      0x00000004U
#define CKMD_STAT_STATE_WAIT_START                                  0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_RESULT
//
//*****************************************************************************
// Field:  [31:0] VALUE
//
// TDC conversion result.
//
// The result of the TDC conversion is given in number of clock edges of the
// clock source selected in [IPSPECIFIC.CKM.TDCCLKSEL.REFCLK]. Both rising and
// falling edges are counted.
//
// Note that [TDC.SATCFG.LIMIT] is given in periods, while VALUE is given in
// edges (periods*2).
// If TDC counter saturates, VALUE is slightly higher than
// [TDC.SATCFG.LIMIT]*2, as it takes a non-zero time to stop the measurement.
// Hence, the maximum value of this field becomes slightly higher than 2^31
// (2^30 periods*2) if you configure [TDC.SATCFG.LIMIT] to R30.
#define CKMD_RESULT_VALUE_W                                                 32U
#define CKMD_RESULT_VALUE_M                                         0xFFFFFFFFU
#define CKMD_RESULT_VALUE_S                                                  0U

//*****************************************************************************
//
// Register: CKMD_O_SATCFG
//
//*****************************************************************************
// Field:   [4:0] LIMIT
//
// Saturation limit.
//
// The flag [TDC.STAT.SAT] is set when the TDC counter saturates.
// Note that this value is given in periods, while [TDC.RESULT.VALUE] is given
// in edges (periods*2).
//
// Values not enumerated are not supported
// ENUMs:
// R30                      TDC conversion saturates and stops after 2^30
//                          periods.
// R29                      TDC conversion saturates and stops after 2^29
//                          periods.
// R28                      TDC conversion saturates and stops after 2^28
//                          periods.
// R27                      TDC conversion saturates and stops after 2^27
//                          periods.
// R26                      TDC conversion saturates and stops after 2^26
//                          periods.
// R25                      TDC conversion saturates and stops after 2^25
//                          periods.
// R24                      TDC conversion saturates and stops after 2^24
//                          periods.
// R23                      TDC conversion saturates and stops after 2^23
//                          periods.
// R22                      TDC conversion saturates and stops after 2^22
//                          periods.
// R21                      TDC conversion saturates and stops after 2^21
//                          periods.
// R20                      TDC conversion saturates and stops after 2^20
//                          periods.
// R19                      TDC conversion saturates and stops after 2^19
//                          periods.
// R18                      TDC conversion saturates and stops after 2^18
//                          periods.
// R17                      TDC conversion saturates and stops after 2^17
//                          periods.
// R16                      TDC conversion saturates and stops after 2^16
//                          periods.
// R15                      TDC conversion saturates and stops after 2^15
//                          periods.
// R14                      TDC conversion saturates and stops after 2^14
//                          periods.
// R13                      TDC conversion saturates and stops after 2^13
//                          periods.
// R12                      TDC conversion saturates and stops after 2^12
//                          periods.
// NONE                     No saturation. An additional timer should be used
//                          to know if [TDC.RESULT.VALUE] rolled over.
#define CKMD_SATCFG_LIMIT_W                                                  5U
#define CKMD_SATCFG_LIMIT_M                                         0x0000001FU
#define CKMD_SATCFG_LIMIT_S                                                  0U
#define CKMD_SATCFG_LIMIT_R30                                       0x00000015U
#define CKMD_SATCFG_LIMIT_R29                                       0x00000014U
#define CKMD_SATCFG_LIMIT_R28                                       0x00000013U
#define CKMD_SATCFG_LIMIT_R27                                       0x00000012U
#define CKMD_SATCFG_LIMIT_R26                                       0x00000011U
#define CKMD_SATCFG_LIMIT_R25                                       0x00000010U
#define CKMD_SATCFG_LIMIT_R24                                       0x0000000FU
#define CKMD_SATCFG_LIMIT_R23                                       0x0000000EU
#define CKMD_SATCFG_LIMIT_R22                                       0x0000000DU
#define CKMD_SATCFG_LIMIT_R21                                       0x0000000CU
#define CKMD_SATCFG_LIMIT_R20                                       0x0000000BU
#define CKMD_SATCFG_LIMIT_R19                                       0x0000000AU
#define CKMD_SATCFG_LIMIT_R18                                       0x00000009U
#define CKMD_SATCFG_LIMIT_R17                                       0x00000008U
#define CKMD_SATCFG_LIMIT_R16                                       0x00000007U
#define CKMD_SATCFG_LIMIT_R15                                       0x00000006U
#define CKMD_SATCFG_LIMIT_R14                                       0x00000005U
#define CKMD_SATCFG_LIMIT_R13                                       0x00000004U
#define CKMD_SATCFG_LIMIT_R12                                       0x00000003U
#define CKMD_SATCFG_LIMIT_NONE                                      0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_TRIGSRC
//
//*****************************************************************************
// Field:    [15] STOP_POL
//
// Polarity of stop source.
//
// Change only while [TDC.STAT.STATE] is IDLE.
// ENUMs:
// LOW                      TDC conversion stops when low level is detected.
// HIGH                     TDC conversion stops when high level is detected.
#define CKMD_TRIGSRC_STOP_POL                                       0x00008000U
#define CKMD_TRIGSRC_STOP_POL_M                                     0x00008000U
#define CKMD_TRIGSRC_STOP_POL_S                                             15U
#define CKMD_TRIGSRC_STOP_POL_LOW                                   0x00008000U
#define CKMD_TRIGSRC_STOP_POL_HIGH                                  0x00000000U

// Field:  [12:8] STOP_SRC
//
// Select stop source from the asynchronous AUX event bus.
//
// Change only while [TDC.STAT.STATE] is IDLE.
// ENUMs:
// TDC_PRE                  Select TDC Prescaler event which is generated by
//                          configuration of TDC.PRECTL.
// DTB15                    Digital testbus bit 15
// DTB14                    Digital testbus bit 14
// DTB13                    Digital testbus bit 13
// DTB12                    Digital testbus bit 12
// DTB11                    Digital testbus bit 11
// DTB10                    Digital testbus bit 10
// DTB9                     Digital testbus bit 9
// DTB8                     Digital testbus bit 8
// DTB7                     Digital testbus bit 7
// DTB6                     Digital testbus bit 6
// DTB5                     Digital testbus bit 5
// DTB4                     Digital testbus bit 4
// DTB3                     Digital testbus bit 3
// DTB2                     Digital testbus bit 2
// DTB1                     Digital testbus bit 1
// DTB0                     Digital testbus bit 0
// GPI                      General purpose input signal
// LFCLK_DLY                Delayed version of selected LFCLK
// LFXT                     Low frequency crystal oscillator
// LFOSC                    Low frequency on-chip oscillator
// LFTICK                   LFTICK signal going to the RTC
#define CKMD_TRIGSRC_STOP_SRC_W                                              5U
#define CKMD_TRIGSRC_STOP_SRC_M                                     0x00001F00U
#define CKMD_TRIGSRC_STOP_SRC_S                                              8U
#define CKMD_TRIGSRC_STOP_SRC_TDC_PRE                               0x00001F00U
#define CKMD_TRIGSRC_STOP_SRC_DTB15                                 0x00001400U
#define CKMD_TRIGSRC_STOP_SRC_DTB14                                 0x00001300U
#define CKMD_TRIGSRC_STOP_SRC_DTB13                                 0x00001200U
#define CKMD_TRIGSRC_STOP_SRC_DTB12                                 0x00001100U
#define CKMD_TRIGSRC_STOP_SRC_DTB11                                 0x00001000U
#define CKMD_TRIGSRC_STOP_SRC_DTB10                                 0x00000F00U
#define CKMD_TRIGSRC_STOP_SRC_DTB9                                  0x00000E00U
#define CKMD_TRIGSRC_STOP_SRC_DTB8                                  0x00000D00U
#define CKMD_TRIGSRC_STOP_SRC_DTB7                                  0x00000C00U
#define CKMD_TRIGSRC_STOP_SRC_DTB6                                  0x00000B00U
#define CKMD_TRIGSRC_STOP_SRC_DTB5                                  0x00000A00U
#define CKMD_TRIGSRC_STOP_SRC_DTB4                                  0x00000900U
#define CKMD_TRIGSRC_STOP_SRC_DTB3                                  0x00000800U
#define CKMD_TRIGSRC_STOP_SRC_DTB2                                  0x00000700U
#define CKMD_TRIGSRC_STOP_SRC_DTB1                                  0x00000600U
#define CKMD_TRIGSRC_STOP_SRC_DTB0                                  0x00000500U
#define CKMD_TRIGSRC_STOP_SRC_GPI                                   0x00000400U
#define CKMD_TRIGSRC_STOP_SRC_LFCLK_DLY                             0x00000300U
#define CKMD_TRIGSRC_STOP_SRC_LFXT                                  0x00000200U
#define CKMD_TRIGSRC_STOP_SRC_LFOSC                                 0x00000100U
#define CKMD_TRIGSRC_STOP_SRC_LFTICK                                0x00000000U

// Field:     [7] START_POL
//
// Polarity of start source.
//
// Change only while [TDC.STAT.STATE] is IDLE.
// ENUMs:
// LOW                      TDC conversion starts when low level is detected.
// HIGH                     TDC conversion starts when high level is detected.
#define CKMD_TRIGSRC_START_POL                                      0x00000080U
#define CKMD_TRIGSRC_START_POL_M                                    0x00000080U
#define CKMD_TRIGSRC_START_POL_S                                             7U
#define CKMD_TRIGSRC_START_POL_LOW                                  0x00000080U
#define CKMD_TRIGSRC_START_POL_HIGH                                 0x00000000U

// Field:   [4:0] START_SRC
//
// Select start source from the asynchronous AUX event bus.
//
// Change only while [TDC.STAT.STATE] is IDLE.
// ENUMs:
// TDC_PRE                  Select TDC Prescaler event which is generated by
//                          configuration of TDC.PRECTL.
// DTB15                    Digital testbus bit 15
// DTB14                    Digital testbus bit 14
// DTB13                    Digital testbus bit 13
// DTB12                    Digital testbus bit 12
// DTB11                    Digital testbus bit 11
// DTB10                    Digital testbus bit 10
// DTB9                     Digital testbus bit 9
// DTB8                     Digital testbus bit 8
// DTB7                     Digital testbus bit 7
// DTB6                     Digital testbus bit 6
// DTB5                     Digital testbus bit 5
// DTB4                     Digital testbus bit 4
// DTB3                     Digital testbus bit 3
// DTB2                     Digital testbus bit 2
// DTB1                     Digital testbus bit 1
// DTB0                     Digital testbus bit 0
// GPI                      General purpose input signal
// LFCLK_DLY                Delayed version of selected LFCLK
// LFXT                     Low frequency crystal oscillator
// LFOSC                    Low frequency on-chip oscillator
// LFTICK                   LFTICK signal going to the RTC
#define CKMD_TRIGSRC_START_SRC_W                                             5U
#define CKMD_TRIGSRC_START_SRC_M                                    0x0000001FU
#define CKMD_TRIGSRC_START_SRC_S                                             0U
#define CKMD_TRIGSRC_START_SRC_TDC_PRE                              0x0000001FU
#define CKMD_TRIGSRC_START_SRC_DTB15                                0x00000014U
#define CKMD_TRIGSRC_START_SRC_DTB14                                0x00000013U
#define CKMD_TRIGSRC_START_SRC_DTB13                                0x00000012U
#define CKMD_TRIGSRC_START_SRC_DTB12                                0x00000011U
#define CKMD_TRIGSRC_START_SRC_DTB11                                0x00000010U
#define CKMD_TRIGSRC_START_SRC_DTB10                                0x0000000FU
#define CKMD_TRIGSRC_START_SRC_DTB9                                 0x0000000EU
#define CKMD_TRIGSRC_START_SRC_DTB8                                 0x0000000DU
#define CKMD_TRIGSRC_START_SRC_DTB7                                 0x0000000CU
#define CKMD_TRIGSRC_START_SRC_DTB6                                 0x0000000BU
#define CKMD_TRIGSRC_START_SRC_DTB5                                 0x0000000AU
#define CKMD_TRIGSRC_START_SRC_DTB4                                 0x00000009U
#define CKMD_TRIGSRC_START_SRC_DTB3                                 0x00000008U
#define CKMD_TRIGSRC_START_SRC_DTB2                                 0x00000007U
#define CKMD_TRIGSRC_START_SRC_DTB1                                 0x00000006U
#define CKMD_TRIGSRC_START_SRC_DTB0                                 0x00000005U
#define CKMD_TRIGSRC_START_SRC_GPI                                  0x00000004U
#define CKMD_TRIGSRC_START_SRC_LFCLK_DLY                            0x00000003U
#define CKMD_TRIGSRC_START_SRC_LFXT                                 0x00000002U
#define CKMD_TRIGSRC_START_SRC_LFOSC                                0x00000001U
#define CKMD_TRIGSRC_START_SRC_LFTICK                               0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_TRIGCNT
//
//*****************************************************************************
// Field:  [15:0] CNT
//
// Number of stop events to ignore when [TDC.TRIGCNTCFG.EN] is 1.
//
// Read CNT to get the remaining number of stop events to ignore during a TDC
// measurement.
//
// Write CNT to update the remaining number of stop events to ignore during a
// TDC measurement. The TDC measurement ignores updates of CNT if there are no
// more stop events left to ignore.
//
// When [TDC.TRIGCNTCFG.EN] is 1, [TDC.TRIGCNTLOAD.CNT] is loaded into CNT at
// the start of the measurement.
#define CKMD_TRIGCNT_CNT_W                                                  16U
#define CKMD_TRIGCNT_CNT_M                                          0x0000FFFFU
#define CKMD_TRIGCNT_CNT_S                                                   0U

//*****************************************************************************
//
// Register: CKMD_O_TRIGCNTLOAD
//
//*****************************************************************************
// Field:  [15:0] CNT
//
// Number of stop events to ignore when [TDC.TRIGCNTCFG.EN] is 1.
//
// To measure frequency of an event source:
// - Set start event equal to stop event.
// - Set CNT to number of periods to measure. Both 0 and 1 values measures a
// single event source period.
//
// To measure pulse width of an event source:
// - Set start event source equal to stop event source.
// - Select different polarity for start and stop event.
// - Set CNT to 0.
//
// To measure time from the start event to the Nth stop event when N > 1:
// - Select different start and stop event source.
// - Set CNT to (N-1).
//
// See the Technical Reference Manual for event timing requirements.
//
// When [TDC.TRIGCNTCFG.EN] is 1, CNT is loaded into [TDC.TRIGCNT.CNT] at the
// start of the measurement.
#define CKMD_TRIGCNTLOAD_CNT_W                                              16U
#define CKMD_TRIGCNTLOAD_CNT_M                                      0x0000FFFFU
#define CKMD_TRIGCNTLOAD_CNT_S                                               0U

//*****************************************************************************
//
// Register: CKMD_O_TRIGCNTCFG
//
//*****************************************************************************
// Field:     [0] EN
//
// Enable stop-counter.
//
// 0: Disable stop-counter.
// 1: Enable stop-counter.
//
// Change only while [TDC.STAT.STATE] is IDLE.
#define CKMD_TRIGCNTCFG_EN                                          0x00000001U
#define CKMD_TRIGCNTCFG_EN_M                                        0x00000001U
#define CKMD_TRIGCNTCFG_EN_S                                                 0U

//*****************************************************************************
//
// Register: CKMD_O_PRECTL
//
//*****************************************************************************
// Field:     [7] RESET_N
//
// Prescaler reset.
//
// 0: Reset prescaler.
// 1: Release reset of prescaler.
//
// AUX_TDC_PRE event becomes 0 when you reset the prescaler.
#define CKMD_PRECTL_RESET_N                                         0x00000080U
#define CKMD_PRECTL_RESET_N_M                                       0x00000080U
#define CKMD_PRECTL_RESET_N_S                                                7U

// Field:     [6] RATIO
//
// Prescaler ratio.
//
// This controls how often the TDC_PRE event is generated by the prescaler.
// ENUMs:
// DIV64                    Prescaler divides input by 64.
//
//                          AUX_TDC_PRE event has a
//                          rising edge for every 64 rising edges of the
//                          input. AUX_TDC_PRE event toggles on every 32nd
//                          rising edge of the input.
// DIV16                    Prescaler divides input by 16.
//
//                          AUX_TDC_PRE event has a
//                          rising edge for every 16 rising edges of the
//                          input. AUX_TDC_PRE event toggles on every 8th
//                          rising edge of the input.
#define CKMD_PRECTL_RATIO                                           0x00000040U
#define CKMD_PRECTL_RATIO_M                                         0x00000040U
#define CKMD_PRECTL_RATIO_S                                                  6U
#define CKMD_PRECTL_RATIO_DIV64                                     0x00000040U
#define CKMD_PRECTL_RATIO_DIV16                                     0x00000000U

// Field:   [4:0] SRC
//
// Prescaler event source.
//
// Select an event from the asynchronous AUX event bus to connect to the
// prescaler input.
//
// Configure only while RESET_N is 0.
// ENUMs:
// AFOSC                    Audio frequency on-chip oscillator
// HFXT                     High frequency crystal oscillator
// HFOSC                    High frequency on-chip oscillator
// DTB15                    Digital testbus bit 15
// DTB14                    Digital testbus bit 14
// DTB13                    Digital testbus bit 13
// DTB12                    Digital testbus bit 12
// DTB11                    Digital testbus bit 11
// DTB10                    Digital testbus bit 10
// DTB9                     Digital testbus bit 9
// DTB8                     Digital testbus bit 8
// DTB7                     Digital testbus bit 7
// DTB6                     Digital testbus bit 6
// DTB5                     Digital testbus bit 5
// DTB4                     Digital testbus bit 4
// DTB3                     Digital testbus bit 3
// DTB2                     Digital testbus bit 2
// DTB1                     Digital testbus bit 1
// DTB0                     Digital testbus bit 0
// GPI                      General purpose input signal
// LFCLK_DLY                Delayed version of selected LFCLK
// LFXT                     Low frequency crystal oscillator
// LFOSC                    Low frequency on-chip oscillator
// LFTICK                   LFTICK signal going to the RTC
#define CKMD_PRECTL_SRC_W                                                    5U
#define CKMD_PRECTL_SRC_M                                           0x0000001FU
#define CKMD_PRECTL_SRC_S                                                    0U
#define CKMD_PRECTL_SRC_AFOSC                                       0x00000017U
#define CKMD_PRECTL_SRC_HFXT                                        0x00000016U
#define CKMD_PRECTL_SRC_HFOSC                                       0x00000015U
#define CKMD_PRECTL_SRC_DTB15                                       0x00000014U
#define CKMD_PRECTL_SRC_DTB14                                       0x00000013U
#define CKMD_PRECTL_SRC_DTB13                                       0x00000012U
#define CKMD_PRECTL_SRC_DTB12                                       0x00000011U
#define CKMD_PRECTL_SRC_DTB11                                       0x00000010U
#define CKMD_PRECTL_SRC_DTB10                                       0x0000000FU
#define CKMD_PRECTL_SRC_DTB9                                        0x0000000EU
#define CKMD_PRECTL_SRC_DTB8                                        0x0000000DU
#define CKMD_PRECTL_SRC_DTB7                                        0x0000000CU
#define CKMD_PRECTL_SRC_DTB6                                        0x0000000BU
#define CKMD_PRECTL_SRC_DTB5                                        0x0000000AU
#define CKMD_PRECTL_SRC_DTB4                                        0x00000009U
#define CKMD_PRECTL_SRC_DTB3                                        0x00000008U
#define CKMD_PRECTL_SRC_DTB2                                        0x00000007U
#define CKMD_PRECTL_SRC_DTB1                                        0x00000006U
#define CKMD_PRECTL_SRC_DTB0                                        0x00000005U
#define CKMD_PRECTL_SRC_GPI                                         0x00000004U
#define CKMD_PRECTL_SRC_LFCLK_DLY                                   0x00000003U
#define CKMD_PRECTL_SRC_LFXT                                        0x00000002U
#define CKMD_PRECTL_SRC_LFOSC                                       0x00000001U
#define CKMD_PRECTL_SRC_LFTICK                                      0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_PRECNTR
//
//*****************************************************************************
// Field:    [16] CAPT
//
// Prescaler counter capture strobe.
//
// Write a 1 to CAPT to capture the value of the 16-bit prescaler counter into
// CNT. Read CNT to get the captured value.
#define CKMD_PRECNTR_CAPT                                           0x00010000U
#define CKMD_PRECNTR_CAPT_M                                         0x00010000U
#define CKMD_PRECNTR_CAPT_S                                                 16U

// Field:  [15:0] CNT
//
// Prescaler counter value.
//
// Write a 1 to CAPT to capture the value of the 16-bit prescaler counter into
// CNT. Read CNT to get the captured value.
//
// The read value gets 1 LSB uncertainty if the event source level rises when
// you release the reset.
// The read value gets 1 LSB uncertainty if the event source level rises when
// you capture the prescaler counter.
//
// Please note the following:
// - The prescaler counter is reset to 3 by [TDC.PRECTL.RESET_N].
// - The captured value is 3 when the number of rising edges on prescaler input
// is less than 3. Otherwise, captured value equals number of event pulses.
#define CKMD_PRECNTR_CNT_W                                                  16U
#define CKMD_PRECNTR_CNT_M                                          0x0000FFFFU
#define CKMD_PRECNTR_CNT_S                                                   0U

//*****************************************************************************
//
// Register: CKMD_O_CNT
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Counter value.
//
// A write to this field immediately starts (or restarts) the counter. It will
// count down from the written value.
// If the counter reaches 0, a reset will be generated.
// A write value of 0 immediately generates a reset.
//
// This field is only writable if not locked. See LOCK register.
// Writing this field will automatically activate the lock.
//
// A read returns the current value of the counter.
#define CKMD_CNT_VAL_W                                                      32U
#define CKMD_CNT_VAL_M                                              0xFFFFFFFFU
#define CKMD_CNT_VAL_S                                                       0U

//*****************************************************************************
//
// Register: CKMD_O_TEST
//
//*****************************************************************************
// Field:     [0] STALLEN
//
// WDT stall enable
//
// This field is only writable if not locked. See LOCK register.
// ENUMs:
// EN                       ENABLE
//
//                          WDT stops counting while
//                          the CPU is stopped by a debugger.
// DIS                      DISABLE
//
//                          WDT continues counting
//                          while the CPU is stopped by a debugger.
#define CKMD_TEST_STALLEN                                           0x00000001U
#define CKMD_TEST_STALLEN_M                                         0x00000001U
#define CKMD_TEST_STALLEN_S                                                  0U
#define CKMD_TEST_STALLEN_EN                                        0x00000001U
#define CKMD_TEST_STALLEN_DIS                                       0x00000000U

//*****************************************************************************
//
// Register: CKMD_O_LOCK
//
//*****************************************************************************
// Field:  [31:0] STAT
//
// A write with value 0x1ACCE551 unlocks the watchdog registers for write
// access.
// A write with any other value locks the watchdog registers for write access.
// Writing the CNT register will also lock the watchdog registers.
//
// A read of this field returns the state of the lock (0=unlocked, 1=locked).
#define CKMD_LOCK_STAT_W                                                    32U
#define CKMD_LOCK_STAT_M                                            0xFFFFFFFFU
#define CKMD_LOCK_STAT_S                                                     0U


#endif // __CKMD__
