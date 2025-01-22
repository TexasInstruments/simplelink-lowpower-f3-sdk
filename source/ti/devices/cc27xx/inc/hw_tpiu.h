/******************************************************************************
*  Filename:       hw_tpiu_h
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

#ifndef __HW_TPIU_H__
#define __HW_TPIU_H__

//*****************************************************************************
//
// This section defines the register offsets of
// TPIU component
//
//*****************************************************************************
// Supported Sync Port Sizes
#define TPIU_O_SSPSR                                                0x00000000U

// Current Sync Port Size
#define TPIU_O_CSPSR                                                0x00000004U

// Async Clock Prescaler
#define TPIU_O_ACPR                                                 0x00000010U

// Selected Pin Protocol
#define TPIU_O_SPPR                                                 0x000000F0U

// Formatter and Flush Status
#define TPIU_O_FFSR                                                 0x00000300U

// Formatter and Flush Control
#define TPIU_O_FFCR                                                 0x00000304U

// Formatter Synchronization Counter
#define TPIU_O_PSCR                                                 0x00000308U

// Claim Tag Mask
#define TPIU_O_CLAIMMASK                                            0x00000FA0U

// Claim Tag Set
#define TPIU_O_CLAIMSET                                             0x00000FA0U

// Current Claim Tag
#define TPIU_O_CLAIMTAG                                             0x00000FA4U

// Claim Tag Clear
#define TPIU_O_CLAIMCLR                                             0x00000FA4U

// Device ID
#define TPIU_O_DEVID                                                0x00000FC8U

//*****************************************************************************
//
// Register: TPIU_O_SSPSR
//
//*****************************************************************************
// Field:     [3] FOUR
//
// 4-bit port size support
//
// 0x0: Not supported
// 0x1: Supported
#define TPIU_SSPSR_FOUR                                             0x00000008U
#define TPIU_SSPSR_FOUR_M                                           0x00000008U
#define TPIU_SSPSR_FOUR_S                                                    3U

// Field:     [2] THREE
//
// 3-bit port size support
//
// 0x0: Not supported
// 0x1: Supported
#define TPIU_SSPSR_THREE                                            0x00000004U
#define TPIU_SSPSR_THREE_M                                          0x00000004U
#define TPIU_SSPSR_THREE_S                                                   2U

// Field:     [1] TWO
//
// 2-bit port size support
//
// 0x0: Not supported
// 0x1: Supported
#define TPIU_SSPSR_TWO                                              0x00000002U
#define TPIU_SSPSR_TWO_M                                            0x00000002U
#define TPIU_SSPSR_TWO_S                                                     1U

// Field:     [0] ONE
//
// 1-bit port size support
//
// 0x0: Not supported
// 0x1: Supported
#define TPIU_SSPSR_ONE                                              0x00000001U
#define TPIU_SSPSR_ONE_M                                            0x00000001U
#define TPIU_SSPSR_ONE_S                                                     0U

//*****************************************************************************
//
// Register: TPIU_O_CSPSR
//
//*****************************************************************************
// Field:     [3] FOUR
//
// 4-bit port enable
// Writing values with more than one bit set in CSPSR, or setting a bit that is
// not indicated as supported in SSPSR can cause Unpredictable behavior.
#define TPIU_CSPSR_FOUR                                             0x00000008U
#define TPIU_CSPSR_FOUR_M                                           0x00000008U
#define TPIU_CSPSR_FOUR_S                                                    3U

// Field:     [2] THREE
//
// 3-bit port enable
// Writing values with more than one bit set in CSPSR, or setting a bit that is
// not indicated as supported in SSPSR can cause Unpredictable behavior.
#define TPIU_CSPSR_THREE                                            0x00000004U
#define TPIU_CSPSR_THREE_M                                          0x00000004U
#define TPIU_CSPSR_THREE_S                                                   2U

// Field:     [1] TWO
//
// 2-bit port enable
// Writing values with more than one bit set in CSPSR, or setting a bit that is
// not indicated as supported in SSPSR can cause Unpredictable behavior.
#define TPIU_CSPSR_TWO                                              0x00000002U
#define TPIU_CSPSR_TWO_M                                            0x00000002U
#define TPIU_CSPSR_TWO_S                                                     1U

// Field:     [0] ONE
//
// 1-bit port enable
// Writing values with more than one bit set in CSPSR, or setting a bit that is
// not indicated as supported in SSPSR can cause Unpredictable behavior.
#define TPIU_CSPSR_ONE                                              0x00000001U
#define TPIU_CSPSR_ONE_M                                            0x00000001U
#define TPIU_CSPSR_ONE_S                                                     0U

//*****************************************************************************
//
// Register: TPIU_O_ACPR
//
//*****************************************************************************
// Field:  [12:0] PRESCALER
//
// Divisor for input trace clock is (PRESCALER + 1).
#define TPIU_ACPR_PRESCALER_W                                               13U
#define TPIU_ACPR_PRESCALER_M                                       0x00001FFFU
#define TPIU_ACPR_PRESCALER_S                                                0U

//*****************************************************************************
//
// Register: TPIU_O_SPPR
//
//*****************************************************************************
// Field:   [1:0] PROTOCOL
//
// Trace output protocol
// ENUMs:
// SWO_NRZ                  SerialWire Output (NRZ)
// SWO_MANCHESTER           SerialWire Output (Manchester). This is the reset
//                          value.
// TRACEPORT                TracePort mode
#define TPIU_SPPR_PROTOCOL_W                                                 2U
#define TPIU_SPPR_PROTOCOL_M                                        0x00000003U
#define TPIU_SPPR_PROTOCOL_S                                                 0U
#define TPIU_SPPR_PROTOCOL_SWO_NRZ                                  0x00000002U
#define TPIU_SPPR_PROTOCOL_SWO_MANCHESTER                           0x00000001U
#define TPIU_SPPR_PROTOCOL_TRACEPORT                                0x00000000U

//*****************************************************************************
//
// Register: TPIU_O_FFSR
//
//*****************************************************************************
// Field:     [3] FTNONSTOP
//
// 0: Formatter can be stopped
// 1: Formatter cannot be stopped
#define TPIU_FFSR_FTNONSTOP                                         0x00000008U
#define TPIU_FFSR_FTNONSTOP_M                                       0x00000008U
#define TPIU_FFSR_FTNONSTOP_S                                                3U

//*****************************************************************************
//
// Register: TPIU_O_FFCR
//
//*****************************************************************************
// Field:     [8] TRIGIN
//
// Indicates that triggers are inserted when a trigger pin is asserted.
#define TPIU_FFCR_TRIGIN                                            0x00000100U
#define TPIU_FFCR_TRIGIN_M                                          0x00000100U
#define TPIU_FFCR_TRIGIN_S                                                   8U

// Field:     [1] ENFCONT
//
// Enable continuous formatting:
//
// 0: Continuous formatting disabled
// 1: Continuous formatting enabled
#define TPIU_FFCR_ENFCONT                                           0x00000002U
#define TPIU_FFCR_ENFCONT_M                                         0x00000002U
#define TPIU_FFCR_ENFCONT_S                                                  1U

//*****************************************************************************
//
// Register: TPIU_O_PSCR
//
//*****************************************************************************
// Field:   [4:0] PSCOUNT
//
// Periodic Synchronization Count. Determines the reload value of the Periodic
// Synchronization Counter. The reload value takes effect the next time the
// counter reaches zero. Reads from this register return the reload value
// programmed into this register
//
//
//
// 0b00000 Synchronization disabled.
// 0b00111 128 bytes
//
// 0b01000 256 bytes
//
// 0b11111 2^31 bytes
#define TPIU_PSCR_PSCOUNT_W                                                  5U
#define TPIU_PSCR_PSCOUNT_M                                         0x0000001FU
#define TPIU_PSCR_PSCOUNT_S                                                  0U

//*****************************************************************************
//
// Register: TPIU_O_CLAIMMASK
//
//*****************************************************************************
// Field:  [31:0] CLAIMMASK
//
// This register forms one half of the Claim Tag value. When reading this
// register returns the number of bits that can be set (each bit is considered
// separately):
//
// 0: This claim tag bit is not implemented
// 1: This claim tag bit is not implemented
//
// The behavior when writing to this register is described in CLAIMSET.
#define TPIU_CLAIMMASK_CLAIMMASK_W                                          32U
#define TPIU_CLAIMMASK_CLAIMMASK_M                                  0xFFFFFFFFU
#define TPIU_CLAIMMASK_CLAIMMASK_S                                           0U

//*****************************************************************************
//
// Register: TPIU_O_CLAIMSET
//
//*****************************************************************************
// Field:  [31:0] CLAIMSET
//
// This register forms one half of the Claim Tag value. Writing to this
// location allows individual bits to be set (each bit is considered
// separately):
//
// 0: No effect
// 1: Set this bit in the claim tag
//
// The behavior when reading from this location is described in CLAIMMASK.
#define TPIU_CLAIMSET_CLAIMSET_W                                            32U
#define TPIU_CLAIMSET_CLAIMSET_M                                    0xFFFFFFFFU
#define TPIU_CLAIMSET_CLAIMSET_S                                             0U

//*****************************************************************************
//
// Register: TPIU_O_CLAIMTAG
//
//*****************************************************************************
// Field:  [31:0] CLAIMTAG
//
// This register forms one half of the Claim Tag value. Reading this register
// returns the current Claim Tag value.
// Reading CLAIMMASK determines how many bits from this register must be used.
//
// The behavior when writing to this register is described in CLAIMCLR.
#define TPIU_CLAIMTAG_CLAIMTAG_W                                            32U
#define TPIU_CLAIMTAG_CLAIMTAG_M                                    0xFFFFFFFFU
#define TPIU_CLAIMTAG_CLAIMTAG_S                                             0U

//*****************************************************************************
//
// Register: TPIU_O_CLAIMCLR
//
//*****************************************************************************
// Field:  [31:0] CLAIMCLR
//
// This register forms one half of the Claim Tag value. Writing to this
// location enables individual bits to be cleared (each bit is considered
// separately):
//
// 0: No effect
// 1: Clear this bit in the claim tag.
//
// The behavior when reading from this location is described in CLAIMTAG.
#define TPIU_CLAIMCLR_CLAIMCLR_W                                            32U
#define TPIU_CLAIMCLR_CLAIMCLR_M                                    0xFFFFFFFFU
#define TPIU_CLAIMCLR_CLAIMCLR_S                                             0U

//*****************************************************************************
//
// Register: TPIU_O_DEVID
//
//*****************************************************************************
// Field:  [31:0] DEVID
//
// This field returns: 0xCA1 if there is an ETM present. 0xCA0 if there is no
// ETM present.
#define TPIU_DEVID_DEVID_W                                                  32U
#define TPIU_DEVID_DEVID_M                                          0xFFFFFFFFU
#define TPIU_DEVID_DEVID_S                                                   0U


#endif // __TPIU__
