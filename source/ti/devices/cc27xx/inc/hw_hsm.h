/******************************************************************************
*  Filename:       hw_hsm_h
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

#ifndef __HW_HSM_H__
#define __HW_HSM_H__

//*****************************************************************************
//
// This section defines the register offsets of
// HSM component
//
//*****************************************************************************
// Module Description
#define HSM_O_DESC                                                  0x00000000U

// Control Register
#define HSM_O_CTL                                                   0x00000004U

// Control Register
#define HSM_O_CHARCTL                                               0x00000008U

// BIST for CRNG
#define HSM_O_BIST                                                  0x0000000CU

//*****************************************************************************
//
// Register: HSM_O_DESC
//
//*****************************************************************************
// Field: [31:16] MODID
//
// Module identification contains a unique peripheral identification number.
#define HSM_DESC_MODID_W                                                    16U
#define HSM_DESC_MODID_M                                            0xFFFF0000U
#define HSM_DESC_MODID_S                                                    16U

// Field: [15:12] STDIPOFF
//
// Standard IP MMR block offset. Standard IP MMRs are the set of from
// aggregated IRQ registers till DTB.
// 0: Standard IP MMRs do not exist
// 0x1-0xF: Standard IP MMRs begin at offset of (64*STDIPOFF from the base IP
// address)
#define HSM_DESC_STDIPOFF_W                                                  4U
#define HSM_DESC_STDIPOFF_M                                         0x0000F000U
#define HSM_DESC_STDIPOFF_S                                                 12U

// Field:  [11:8] INSTIDX
//
// IP Instance ID number. If multiple instances of IP exist in the device, this
// field can identify the instance number (0-15).
#define HSM_DESC_INSTIDX_W                                                   4U
#define HSM_DESC_INSTIDX_M                                          0x00000F00U
#define HSM_DESC_INSTIDX_S                                                   8U

// Field:   [7:4] MAJREV
//
// Major revision of IP
#define HSM_DESC_MAJREV_W                                                    4U
#define HSM_DESC_MAJREV_M                                           0x000000F0U
#define HSM_DESC_MAJREV_S                                                    4U

// Field:   [3:0] MINREV
//
// Minor revision of IP
#define HSM_DESC_MINREV_W                                                    4U
#define HSM_DESC_MINREV_M                                           0x0000000FU
#define HSM_DESC_MINREV_S                                                    0U

//*****************************************************************************
//
// Register: HSM_O_CTL
//
//*****************************************************************************
// Field:    [31] CPUIDUNLK
//
// CPUID Lock. Sets sticky '0' lock for CTL.CPUIDSEL
// ENUMs:
// UNLOCK                   Unlock
// LOCK                     Lock
#define HSM_CTL_CPUIDUNLK                                           0x80000000U
#define HSM_CTL_CPUIDUNLK_M                                         0x80000000U
#define HSM_CTL_CPUIDUNLK_S                                                 31U
#define HSM_CTL_CPUIDUNLK_UNLOCK                                    0x80000000U
#define HSM_CTL_CPUIDUNLK_LOCK                                      0x00000000U

// Field:    [30] CPUIDSEL
//
// CPUID Select. Selects between ROMFW CPUID and Application CPUID
// ENUMs:
// ROMID                    Assert
// APPID                    Dassert
#define HSM_CTL_CPUIDSEL                                            0x40000000U
#define HSM_CTL_CPUIDSEL_M                                          0x40000000U
#define HSM_CTL_CPUIDSEL_S                                                  30U
#define HSM_CTL_CPUIDSEL_ROMID                                      0x40000000U
#define HSM_CTL_CPUIDSEL_APPID                                      0x00000000U

// Field:     [7] REFMARK
//
// Refresher Marker. Trigger writting refresh marker. This bit is auto cleared
// when programming is done
// ENUMs:
// TRG                      Trigger/Busy
// DONE                     Done
#define HSM_CTL_REFMARK                                             0x00000080U
#define HSM_CTL_REFMARK_M                                           0x00000080U
#define HSM_CTL_REFMARK_S                                                    7U
#define HSM_CTL_REFMARK_TRG                                         0x00000080U
#define HSM_CTL_REFMARK_DONE                                        0x00000000U

// Field:     [6] DMAFWDIS
//
// DMA Firewall Disable
// ENUMs:
// DIS                      DMA Firewall Disabled
// EN                       DMA Firewall Enabled
#define HSM_CTL_DMAFWDIS                                            0x00000040U
#define HSM_CTL_DMAFWDIS_M                                          0x00000040U
#define HSM_CTL_DMAFWDIS_S                                                   6U
#define HSM_CTL_DMAFWDIS_DIS                                        0x00000040U
#define HSM_CTL_DMAFWDIS_EN                                         0x00000000U

// Field:     [5] OTPBUSY
//
// OTP Busy. OTP contoller is busy
#define HSM_CTL_OTPBUSY                                             0x00000020U
#define HSM_CTL_OTPBUSY_M                                           0x00000020U
#define HSM_CTL_OTPBUSY_S                                                    5U

// Field:     [4] OTPEVTST
//
// OTP Event Status.
// ENUMs:
// START                    OTP Start
// DONE                     OTP Done
#define HSM_CTL_OTPEVTST                                            0x00000010U
#define HSM_CTL_OTPEVTST_M                                          0x00000010U
#define HSM_CTL_OTPEVTST_S                                                   4U
#define HSM_CTL_OTPEVTST_START                                      0x00000010U
#define HSM_CTL_OTPEVTST_DONE                                       0x00000000U

// Field:     [3] OTPEVTCLR
//
// OTP Event Clear
// ENUMs:
// CLR                      Request PKA Abort
#define HSM_CTL_OTPEVTCLR                                           0x00000008U
#define HSM_CTL_OTPEVTCLR_M                                         0x00000008U
#define HSM_CTL_OTPEVTCLR_S                                                  3U
#define HSM_CTL_OTPEVTCLR_CLR                                       0x00000008U

// Field:     [2] OTPEVTEN
//
// OTP Event Enable
// ENUMs:
// EN                       OTP Event Enable
// DIS                      OTP Event Disable
#define HSM_CTL_OTPEVTEN                                            0x00000004U
#define HSM_CTL_OTPEVTEN_M                                          0x00000004U
#define HSM_CTL_OTPEVTEN_S                                                   2U
#define HSM_CTL_OTPEVTEN_EN                                         0x00000004U
#define HSM_CTL_OTPEVTEN_DIS                                        0x00000000U

// Field:     [1] PKABORT
//
// PKA Abort. Writing 1 to this bit requests PKA Abort, writing 0 has no effect
// ENUMs:
// REQ                      Request PKA Abort
#define HSM_CTL_PKABORT                                             0x00000002U
#define HSM_CTL_PKABORT_M                                           0x00000002U
#define HSM_CTL_PKABORT_S                                                    1U
#define HSM_CTL_PKABORT_REQ                                         0x00000002U

//*****************************************************************************
//
// Register: HSM_O_CHARCTL
//
//*****************************************************************************
// Field:  [12:8] FROSEL
//
// Selects FRO to characterize
#define HSM_CHARCTL_FROSEL_W                                                 5U
#define HSM_CHARCTL_FROSEL_M                                        0x00001F00U
#define HSM_CHARCTL_FROSEL_S                                                 8U

// Field:     [2] DLYSEL
//
// Selects delay in target FRO.This input must only be changed when
// CHARCTL.FROEN = 0
// ENUMs:
// TAP95                    Selects a tap on the approx. 95% of full length
// FULL                     Selects the full chain length
#define HSM_CHARCTL_DLYSEL                                          0x00000004U
#define HSM_CHARCTL_DLYSEL_M                                        0x00000004U
#define HSM_CHARCTL_DLYSEL_S                                                 2U
#define HSM_CHARCTL_DLYSEL_TAP95                                    0x00000004U
#define HSM_CHARCTL_DLYSEL_FULL                                     0x00000000U

// Field:     [1] FROEN
//
// Enables selected FRO
// ENUMs:
// EN                       Enabled
// DIS                      Disabled
#define HSM_CHARCTL_FROEN                                           0x00000002U
#define HSM_CHARCTL_FROEN_M                                         0x00000002U
#define HSM_CHARCTL_FROEN_S                                                  1U
#define HSM_CHARCTL_FROEN_EN                                        0x00000002U
#define HSM_CHARCTL_FROEN_DIS                                       0x00000000U

// Field:     [0] EN
//
// Enables` characterization
// ENUMs:
// EN                       Enabled
// DIS                      Disabled
#define HSM_CHARCTL_EN                                              0x00000001U
#define HSM_CHARCTL_EN_M                                            0x00000001U
#define HSM_CHARCTL_EN_S                                                     0U
#define HSM_CHARCTL_EN_EN                                           0x00000001U
#define HSM_CHARCTL_EN_DIS                                          0x00000000U

//*****************************************************************************
//
// Register: HSM_O_BIST
//
//*****************************************************************************
// Field:    [18] RDY
//
// CRNG BIST ready. When 1, BIST and health checks are complete
#define HSM_BIST_RDY                                                0x00040000U
#define HSM_BIST_RDY_M                                              0x00040000U
#define HSM_BIST_RDY_S                                                      18U

// Field:    [17] CMPLT
//
// CRNG BIST Complete. When 1, BIST checks are done
#define HSM_BIST_CMPLT                                              0x00020000U
#define HSM_BIST_CMPLT_M                                            0x00020000U
#define HSM_BIST_CMPLT_S                                                    17U

// Field:    [16] ERR
//
// CRNG BIST Error. When 1, BIST failed
#define HSM_BIST_ERR                                                0x00010000U
#define HSM_BIST_ERR_M                                              0x00010000U
#define HSM_BIST_ERR_S                                                      16U

// Field:     [0] RSTRT
//
// CRNG BIST Restart. When 1, starts BIST sequence. Ignored when BIST.RDY = 0
#define HSM_BIST_RSTRT                                              0x00000001U
#define HSM_BIST_RSTRT_M                                            0x00000001U
#define HSM_BIST_RSTRT_S                                                     0U


#endif // __HSM__
