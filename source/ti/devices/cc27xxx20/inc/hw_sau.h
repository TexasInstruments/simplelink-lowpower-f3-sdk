/******************************************************************************
*  Filename:       hw_sau_h
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

#ifndef __HW_SAU_H__
#define __HW_SAU_H__

//*****************************************************************************
//
// This section defines the register offsets of
// SAU component
//
//*****************************************************************************
// Allows enabling of the Security Attribution Unit
#define SAU_O_SAU_CTRL                                              0x00000000U

// Indicates the number of regions implemented by the Security Attribution Unit
#define SAU_O_SAU_TYPE                                              0x00000004U

// Selects the region currently accessed by SAU_RBAR and SAU_RLAR
#define SAU_O_SAU_RNR                                               0x00000008U

// Provides indirect read and write access to the base address of the currently
// selected SAU region
#define SAU_O_SAU_RBAR                                              0x0000000CU

// Provides indirect read and write access to the limit address of the
// currently selected SAU region
#define SAU_O_SAU_RLAR                                              0x00000010U

// Provides information about any security related faults
#define SAU_O_SFSR                                                  0x00000014U

// Shows the address of the memory location that caused a Security violation
#define SAU_O_SFAR                                                  0x00000018U

//*****************************************************************************
//
// Register: SAU_O_SAU_CTRL
//
//*****************************************************************************
// Field:  [31:2] RES0
//
// Reserved, RES0
#define SAU_SAU_CTRL_RES0_W                                                 30U
#define SAU_SAU_CTRL_RES0_M                                         0xFFFFFFFCU
#define SAU_SAU_CTRL_RES0_S                                                  2U

// Field:     [1] ALLNS
//
// When SAU_CTRL.ENABLE is 0 this bit controls if the memory is marked as
// Non-secure or Secure
#define SAU_SAU_CTRL_ALLNS                                          0x00000002U
#define SAU_SAU_CTRL_ALLNS_M                                        0x00000002U
#define SAU_SAU_CTRL_ALLNS_S                                                 1U

// Field:     [0] ENABLE
//
// Enables the SAU
#define SAU_SAU_CTRL_ENABLE                                         0x00000001U
#define SAU_SAU_CTRL_ENABLE_M                                       0x00000001U
#define SAU_SAU_CTRL_ENABLE_S                                                0U

//*****************************************************************************
//
// Register: SAU_O_SAU_TYPE
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define SAU_SAU_TYPE_RES0_W                                                 24U
#define SAU_SAU_TYPE_RES0_M                                         0xFFFFFF00U
#define SAU_SAU_TYPE_RES0_S                                                  8U

// Field:   [7:0] SREGION
//
// The number of implemented SAU regions
#define SAU_SAU_TYPE_SREGION_W                                               8U
#define SAU_SAU_TYPE_SREGION_M                                      0x000000FFU
#define SAU_SAU_TYPE_SREGION_S                                               0U

//*****************************************************************************
//
// Register: SAU_O_SAU_RNR
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define SAU_SAU_RNR_RES0_W                                                  24U
#define SAU_SAU_RNR_RES0_M                                          0xFFFFFF00U
#define SAU_SAU_RNR_RES0_S                                                   8U

// Field:   [7:0] REGION
//
// Indicates the SAU region accessed by SAU_RBAR and SAU_RLAR
#define SAU_SAU_RNR_REGION_W                                                 8U
#define SAU_SAU_RNR_REGION_M                                        0x000000FFU
#define SAU_SAU_RNR_REGION_S                                                 0U

//*****************************************************************************
//
// Register: SAU_O_SAU_RBAR
//
//*****************************************************************************
// Field:  [31:5] BADDR
//
// Holds bits [31:5] of the base address for the selected SAU region
#define SAU_SAU_RBAR_BADDR_W                                                27U
#define SAU_SAU_RBAR_BADDR_M                                        0xFFFFFFE0U
#define SAU_SAU_RBAR_BADDR_S                                                 5U

// Field:   [4:0] RES0
//
// Reserved, RES0
#define SAU_SAU_RBAR_RES0_W                                                  5U
#define SAU_SAU_RBAR_RES0_M                                         0x0000001FU
#define SAU_SAU_RBAR_RES0_S                                                  0U

//*****************************************************************************
//
// Register: SAU_O_SAU_RLAR
//
//*****************************************************************************
// Field:  [31:5] LADDR
//
// Holds bits [31:5] of the limit address for the selected SAU region
#define SAU_SAU_RLAR_LADDR_W                                                27U
#define SAU_SAU_RLAR_LADDR_M                                        0xFFFFFFE0U
#define SAU_SAU_RLAR_LADDR_S                                                 5U

// Field:   [4:2] RES0
//
// Reserved, RES0
#define SAU_SAU_RLAR_RES0_W                                                  3U
#define SAU_SAU_RLAR_RES0_M                                         0x0000001CU
#define SAU_SAU_RLAR_RES0_S                                                  2U

// Field:     [1] NSC
//
// Controls whether Non-secure state is permitted to execute an SG instruction
// from this region
#define SAU_SAU_RLAR_NSC                                            0x00000002U
#define SAU_SAU_RLAR_NSC_M                                          0x00000002U
#define SAU_SAU_RLAR_NSC_S                                                   1U

// Field:     [0] ENABLE
//
// SAU region enable
#define SAU_SAU_RLAR_ENABLE                                         0x00000001U
#define SAU_SAU_RLAR_ENABLE_M                                       0x00000001U
#define SAU_SAU_RLAR_ENABLE_S                                                0U

//*****************************************************************************
//
// Register: SAU_O_SFSR
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define SAU_SFSR_RES0_W                                                     24U
#define SAU_SFSR_RES0_M                                             0xFFFFFF00U
#define SAU_SFSR_RES0_S                                                      8U

// Field:     [7] LSERR
//
// Sticky flag indicating that an error occurred during lazy state activation
// or deactivation
#define SAU_SFSR_LSERR                                              0x00000080U
#define SAU_SFSR_LSERR_M                                            0x00000080U
#define SAU_SFSR_LSERR_S                                                     7U

// Field:     [6] SFARVALID
//
// This bit is set when the SFAR register contains a valid value. As with
// similar fields, such as BFSR.BFARVALID and MMFSR.MMARVALID, this bit can be
// cleared by other exceptions, such as BusFault
#define SAU_SFSR_SFARVALID                                          0x00000040U
#define SAU_SFSR_SFARVALID_M                                        0x00000040U
#define SAU_SFSR_SFARVALID_S                                                 6U

// Field:     [5] LSPERR
//
// Stick flag indicating that an SAU or IDAU violation occurred during the lazy
// preservation of floating-point state
#define SAU_SFSR_LSPERR                                             0x00000020U
#define SAU_SFSR_LSPERR_M                                           0x00000020U
#define SAU_SFSR_LSPERR_S                                                    5U

// Field:     [4] INVTRAN
//
// Sticky flag indicating that an exception was raised due to a branch that was
// not flagged as being domain crossing causing a transition from Secure to
// Non-secure memory
#define SAU_SFSR_INVTRAN                                            0x00000010U
#define SAU_SFSR_INVTRAN_M                                          0x00000010U
#define SAU_SFSR_INVTRAN_S                                                   4U

// Field:     [3] AUVIOL
//
// Sticky flag indicating that an attempt was made to access parts of the
// address space that are marked as Secure with NS-Req for the transaction set
// to Non-secure. This bit is not set if the violation occurred during lazy
// state preservation. See LSPERR
#define SAU_SFSR_AUVIOL                                             0x00000008U
#define SAU_SFSR_AUVIOL_M                                           0x00000008U
#define SAU_SFSR_AUVIOL_S                                                    3U

// Field:     [2] INVER
//
// This can be caused by EXC_RETURN.DCRS being set to 0 when returning from an
// exception in the Non-secure state, or by EXC_RETURN.ES being set to 1 when
// returning from an exception in the Non-secure state
#define SAU_SFSR_INVER                                              0x00000004U
#define SAU_SFSR_INVER_M                                            0x00000004U
#define SAU_SFSR_INVER_S                                                     2U

// Field:     [1] INVIS
//
// This bit is set if the integrity signature in an exception stack frame is
// found to be invalid during the unstacking operation
#define SAU_SFSR_INVIS                                              0x00000002U
#define SAU_SFSR_INVIS_M                                            0x00000002U
#define SAU_SFSR_INVIS_S                                                     1U

// Field:     [0] INVEP
//
// This bit is set if a function call from the Non-secure state or exception
// targets a non-SG instruction in the Secure state. This bit is also set if
// the target address is a SG instruction, but there is no matching SAU/IDAU
// region with the NSC flag set
#define SAU_SFSR_INVEP                                              0x00000001U
#define SAU_SFSR_INVEP_M                                            0x00000001U
#define SAU_SFSR_INVEP_S                                                     0U

//*****************************************************************************
//
// Register: SAU_O_SFAR
//
//*****************************************************************************
// Field:  [31:0] ADDRESS
//
// The address of an access that caused a attribution unit violation. This
// field is only valid when SFSR.SFARVALID is set. This allows the actual flip
// flops associated with this register to be shared with other fault address
// registers. If an implementation chooses to share the storage in this way,
// care must be taken to not leak Secure address information to the Non-secure
// state. One way of achieving this is to share the SFAR register with the
// MMFAR_S register, which is not accessible to the Non-secure state
#define SAU_SFAR_ADDRESS_W                                                  32U
#define SAU_SFAR_ADDRESS_M                                          0xFFFFFFFFU
#define SAU_SFAR_ADDRESS_S                                                   0U


#endif // __SAU__
