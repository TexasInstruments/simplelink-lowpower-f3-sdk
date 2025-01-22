/******************************************************************************
*  Filename:       hw_dcb_h
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

#ifndef __HW_DCB_H__
#define __HW_DCB_H__

//*****************************************************************************
//
// This section defines the register offsets of
// DCB component
//
//*****************************************************************************
// Controls halting debug
#define DCB_O_DHCSR                                                 0x00000010U

// With the DCRDR, provides debug access to the general-purpose registers,
// special-purpose registers, and the FP extension registers. A write to the
// DCRSR specifies the register to transfer, whether the transfer is a read or
// write, and starts the transfer
#define DCB_O_DCRSR                                                 0x00000014U

// With the DCRSR, provides debug access to the general-purpose registers,
// special-purpose registers, and the FP Extension registers. If the Main
// Extension is implemented, it can also be used for message passing between an
// external debugger and a debug agent running on the PE
#define DCB_O_DCRDR                                                 0x00000018U

// Manages vector catch behavior and DebugMonitor handling when debugging
#define DCB_O_DEMCR                                                 0x0000001CU

// This register allows the external authentication interface to be overridden
// from software.
#define DCB_O_DAUTHCTRL                                             0x00000024U

// Provides control and status information for Secure debug
#define DCB_O_DSCSR                                                 0x00000028U

//*****************************************************************************
//
// Register: DCB_O_DHCSR
//
//*****************************************************************************
// Field: [31:27] RES0
//
// Reserved, RES0
#define DCB_DHCSR_RES0_W                                                     5U
#define DCB_DHCSR_RES0_M                                            0xF8000000U
#define DCB_DHCSR_RES0_S                                                    27U

// Field:    [26] S_RESTART_ST
//
// Indicates the PE has processed a request to clear DHCSR.C_HALT to 0. That
// is, either a write to DHCSR that clears DHCSR.C_HALT from 1 to 0, or an
// External Restart Request
#define DCB_DHCSR_S_RESTART_ST                                      0x04000000U
#define DCB_DHCSR_S_RESTART_ST_M                                    0x04000000U
#define DCB_DHCSR_S_RESTART_ST_S                                            26U

// Field:    [25] S_RESET_ST
//
// Indicates whether the PE has been reset since the last read of the DHCSR
#define DCB_DHCSR_S_RESET_ST                                        0x02000000U
#define DCB_DHCSR_S_RESET_ST_M                                      0x02000000U
#define DCB_DHCSR_S_RESET_ST_S                                              25U

// Field:    [24] S_RETIRE_ST
//
// Set to 1 every time the PE retires one of more instructions
#define DCB_DHCSR_S_RETIRE_ST                                       0x01000000U
#define DCB_DHCSR_S_RETIRE_ST_M                                     0x01000000U
#define DCB_DHCSR_S_RETIRE_ST_S                                             24U

// Field: [23:21] RES0_1
//
// Reserved, RES0
#define DCB_DHCSR_RES0_1_W                                                   3U
#define DCB_DHCSR_RES0_1_M                                          0x00E00000U
#define DCB_DHCSR_RES0_1_S                                                  21U

// Field:    [20] S_SDE
//
// Indicates whether Secure invasive debug is allowed
#define DCB_DHCSR_S_SDE                                             0x00100000U
#define DCB_DHCSR_S_SDE_M                                           0x00100000U
#define DCB_DHCSR_S_SDE_S                                                   20U

// Field:    [19] S_LOCKUP
//
// Indicates whether the PE is in Lockup state
#define DCB_DHCSR_S_LOCKUP                                          0x00080000U
#define DCB_DHCSR_S_LOCKUP_M                                        0x00080000U
#define DCB_DHCSR_S_LOCKUP_S                                                19U

// Field:    [18] S_SLEEP
//
// Indicates whether the PE is sleeping
#define DCB_DHCSR_S_SLEEP                                           0x00040000U
#define DCB_DHCSR_S_SLEEP_M                                         0x00040000U
#define DCB_DHCSR_S_SLEEP_S                                                 18U

// Field:    [17] S_HALT
//
// Indicates whether the PE is in Debug state
#define DCB_DHCSR_S_HALT                                            0x00020000U
#define DCB_DHCSR_S_HALT_M                                          0x00020000U
#define DCB_DHCSR_S_HALT_S                                                  17U

// Field: [31:16] DBGKEY
//
// A debugger must write 0xA05F to this field to enable write access to the
// remaining bits, otherwise the PE ignores the write access
#define DCB_DHCSR_DBGKEY_W                                                  16U
#define DCB_DHCSR_DBGKEY_M                                          0xFFFF0000U
#define DCB_DHCSR_DBGKEY_S                                                  16U

// Field:    [16] S_REGRDY
//
// Handshake flag to transfers through the DCRDR
#define DCB_DHCSR_S_REGRDY                                          0x00010000U
#define DCB_DHCSR_S_REGRDY_M                                        0x00010000U
#define DCB_DHCSR_S_REGRDY_S                                                16U

// Field:  [15:6] RES0_2
//
// Reserved, RES0
#define DCB_DHCSR_RES0_2_W                                                  10U
#define DCB_DHCSR_RES0_2_M                                          0x0000FFC0U
#define DCB_DHCSR_RES0_2_S                                                   6U

// Field:     [5] C_SNAPSTALL
//
// Allow imprecise entry to Debug state
#define DCB_DHCSR_C_SNAPSTALL                                       0x00000020U
#define DCB_DHCSR_C_SNAPSTALL_M                                     0x00000020U
#define DCB_DHCSR_C_SNAPSTALL_S                                              5U

// Field:     [4] RES0_3
//
// Reserved, RES0
#define DCB_DHCSR_RES0_3                                            0x00000010U
#define DCB_DHCSR_RES0_3_M                                          0x00000010U
#define DCB_DHCSR_RES0_3_S                                                   4U

// Field:     [3] C_MASKINTS
//
// When debug is enabled, the debugger can write to this bit to mask PendSV,
// SysTick and external configurable interrupts
#define DCB_DHCSR_C_MASKINTS                                        0x00000008U
#define DCB_DHCSR_C_MASKINTS_M                                      0x00000008U
#define DCB_DHCSR_C_MASKINTS_S                                               3U

// Field:     [2] C_STEP
//
// Enable single instruction step
#define DCB_DHCSR_C_STEP                                            0x00000004U
#define DCB_DHCSR_C_STEP_M                                          0x00000004U
#define DCB_DHCSR_C_STEP_S                                                   2U

// Field:     [1] C_HALT
//
// PE enter Debug state halt request
#define DCB_DHCSR_C_HALT                                            0x00000002U
#define DCB_DHCSR_C_HALT_M                                          0x00000002U
#define DCB_DHCSR_C_HALT_S                                                   1U

// Field:     [0] C_DEBUGEN
//
// Enable Halting debug
#define DCB_DHCSR_C_DEBUGEN                                         0x00000001U
#define DCB_DHCSR_C_DEBUGEN_M                                       0x00000001U
#define DCB_DHCSR_C_DEBUGEN_S                                                0U

//*****************************************************************************
//
// Register: DCB_O_DCRSR
//
//*****************************************************************************
// Field: [31:17] RES0
//
// Reserved, RES0
#define DCB_DCRSR_RES0_W                                                    15U
#define DCB_DCRSR_RES0_M                                            0xFFFE0000U
#define DCB_DCRSR_RES0_S                                                    17U

// Field:    [16] REGWnR
//
// Specifies the access type for the transfer
#define DCB_DCRSR_REGWNR                                            0x00010000U
#define DCB_DCRSR_REGWNR_M                                          0x00010000U
#define DCB_DCRSR_REGWNR_S                                                  16U

// Field:  [15:7] RES0_1
//
// Reserved, RES0
#define DCB_DCRSR_RES0_1_W                                                   9U
#define DCB_DCRSR_RES0_1_M                                          0x0000FF80U
#define DCB_DCRSR_RES0_1_S                                                   7U

// Field:   [6:0] REGSEL
//
// Specifies the general-purpose register, special-purpose register, or FP
// register to transfer
#define DCB_DCRSR_REGSEL_W                                                   7U
#define DCB_DCRSR_REGSEL_M                                          0x0000007FU
#define DCB_DCRSR_REGSEL_S                                                   0U

//*****************************************************************************
//
// Register: DCB_O_DCRDR
//
//*****************************************************************************
// Field:  [31:0] DBGTMP
//
// Provides debug access for reading and writing the general-purpose registers,
// special-purpose registers, and Floating-point Extension registers
#define DCB_DCRDR_DBGTMP_W                                                  32U
#define DCB_DCRDR_DBGTMP_M                                          0xFFFFFFFFU
#define DCB_DCRDR_DBGTMP_S                                                   0U

//*****************************************************************************
//
// Register: DCB_O_DEMCR
//
//*****************************************************************************
// Field: [31:25] RES0
//
// Reserved, RES0
#define DCB_DEMCR_RES0_W                                                     7U
#define DCB_DEMCR_RES0_M                                            0xFE000000U
#define DCB_DEMCR_RES0_S                                                    25U

// Field:    [24] TRCENA
//
// Global enable for all DWT and ITM features
#define DCB_DEMCR_TRCENA                                            0x01000000U
#define DCB_DEMCR_TRCENA_M                                          0x01000000U
#define DCB_DEMCR_TRCENA_S                                                  24U

// Field: [23:21] RES0_1
//
// Reserved, RES0
#define DCB_DEMCR_RES0_1_W                                                   3U
#define DCB_DEMCR_RES0_1_M                                          0x00E00000U
#define DCB_DEMCR_RES0_1_S                                                  21U

// Field:    [20] SDME
//
// Indicates whether the DebugMonitor targets the Secure or the Non-secure
// state and whether debug events are allowed in Secure state
#define DCB_DEMCR_SDME                                              0x00100000U
#define DCB_DEMCR_SDME_M                                            0x00100000U
#define DCB_DEMCR_SDME_S                                                    20U

// Field:    [19] MON_REQ
//
// DebugMonitor semaphore bit
#define DCB_DEMCR_MON_REQ                                           0x00080000U
#define DCB_DEMCR_MON_REQ_M                                         0x00080000U
#define DCB_DEMCR_MON_REQ_S                                                 19U

// Field:    [18] MON_STEP
//
// Enable DebugMonitor stepping
#define DCB_DEMCR_MON_STEP                                          0x00040000U
#define DCB_DEMCR_MON_STEP_M                                        0x00040000U
#define DCB_DEMCR_MON_STEP_S                                                18U

// Field:    [17] MON_PEND
//
// Sets or clears the pending state of the DebugMonitor exception
#define DCB_DEMCR_MON_PEND                                          0x00020000U
#define DCB_DEMCR_MON_PEND_M                                        0x00020000U
#define DCB_DEMCR_MON_PEND_S                                                17U

// Field:    [16] MON_EN
//
// Enable the DebugMonitor exception
#define DCB_DEMCR_MON_EN                                            0x00010000U
#define DCB_DEMCR_MON_EN_M                                          0x00010000U
#define DCB_DEMCR_MON_EN_S                                                  16U

// Field: [15:12] RES0_2
//
// Reserved, RES0
#define DCB_DEMCR_RES0_2_W                                                   4U
#define DCB_DEMCR_RES0_2_M                                          0x0000F000U
#define DCB_DEMCR_RES0_2_S                                                  12U

// Field:    [11] VC_SFERR
//
// SecureFault exception halting debug vector catch enable
#define DCB_DEMCR_VC_SFERR                                          0x00000800U
#define DCB_DEMCR_VC_SFERR_M                                        0x00000800U
#define DCB_DEMCR_VC_SFERR_S                                                11U

// Field:    [10] VC_HARDERR
//
// HardFault exception halting debug vector catch enable
#define DCB_DEMCR_VC_HARDERR                                        0x00000400U
#define DCB_DEMCR_VC_HARDERR_M                                      0x00000400U
#define DCB_DEMCR_VC_HARDERR_S                                              10U

// Field:     [9] VC_INTERR
//
// Enable halting debug vector catch for faults during exception entry and
// return
#define DCB_DEMCR_VC_INTERR                                         0x00000200U
#define DCB_DEMCR_VC_INTERR_M                                       0x00000200U
#define DCB_DEMCR_VC_INTERR_S                                                9U

// Field:     [8] VC_BUSERR
//
// BusFault exception halting debug vector catch enable
#define DCB_DEMCR_VC_BUSERR                                         0x00000100U
#define DCB_DEMCR_VC_BUSERR_M                                       0x00000100U
#define DCB_DEMCR_VC_BUSERR_S                                                8U

// Field:     [7] VC_STATERR
//
// Enable halting debug trap on a UsageFault exception caused by a state
// information error, for example an Undefined Instruction exception
#define DCB_DEMCR_VC_STATERR                                        0x00000080U
#define DCB_DEMCR_VC_STATERR_M                                      0x00000080U
#define DCB_DEMCR_VC_STATERR_S                                               7U

// Field:     [6] VC_CHKERR
//
// Enable halting debug trap on a UsageFault exception caused by a checking
// error, for example an alignment check error
#define DCB_DEMCR_VC_CHKERR                                         0x00000040U
#define DCB_DEMCR_VC_CHKERR_M                                       0x00000040U
#define DCB_DEMCR_VC_CHKERR_S                                                6U

// Field:     [5] VC_NOCPERR
//
// Enable halting debug trap on a UsageFault caused by an access to a
// coprocessor
#define DCB_DEMCR_VC_NOCPERR                                        0x00000020U
#define DCB_DEMCR_VC_NOCPERR_M                                      0x00000020U
#define DCB_DEMCR_VC_NOCPERR_S                                               5U

// Field:     [4] VC_MMERR
//
// Enable halting debug trap on a MemManage exception
#define DCB_DEMCR_VC_MMERR                                          0x00000010U
#define DCB_DEMCR_VC_MMERR_M                                        0x00000010U
#define DCB_DEMCR_VC_MMERR_S                                                 4U

// Field:   [3:1] RES0_3
//
// Reserved, RES0
#define DCB_DEMCR_RES0_3_W                                                   3U
#define DCB_DEMCR_RES0_3_M                                          0x0000000EU
#define DCB_DEMCR_RES0_3_S                                                   1U

// Field:     [0] VC_CORERESET
//
// Enable Reset Vector Catch. This causes a warm reset to halt a running system
#define DCB_DEMCR_VC_CORERESET                                      0x00000001U
#define DCB_DEMCR_VC_CORERESET_M                                    0x00000001U
#define DCB_DEMCR_VC_CORERESET_S                                             0U

//*****************************************************************************
//
// Register: DCB_O_DAUTHCTRL
//
//*****************************************************************************
// Field:     [3] INTSPNIDEN
//
// Internal Secure non-invasive debug enable. Overrides the external Secure
// non-invasive debug authentication interface
#define DCB_DAUTHCTRL_INTSPNIDEN                                    0x00000008U
#define DCB_DAUTHCTRL_INTSPNIDEN_M                                  0x00000008U
#define DCB_DAUTHCTRL_INTSPNIDEN_S                                           3U

// Field:     [2] SPNIDENSEL
//
// Secure non-invasive debug enable select. Selects between DAUTHCTRL and the
// external authentication interface for control of Secure non-invasive debug
#define DCB_DAUTHCTRL_SPNIDENSEL                                    0x00000004U
#define DCB_DAUTHCTRL_SPNIDENSEL_M                                  0x00000004U
#define DCB_DAUTHCTRL_SPNIDENSEL_S                                           2U

// Field:     [1] INTSPIDEN
//
// Internal Secure invasive debug enable. Overrides the external Secure
// invasive debug authentication Interfaces.
#define DCB_DAUTHCTRL_INTSPIDEN                                     0x00000002U
#define DCB_DAUTHCTRL_INTSPIDEN_M                                   0x00000002U
#define DCB_DAUTHCTRL_INTSPIDEN_S                                            1U

// Field:     [0] SPIDENSEL
//
// Secure invasive debug enable select. Selects between DAUTHCTRL and the
// external authentication interface for control of Secure invasive debug.
#define DCB_DAUTHCTRL_SPIDENSEL                                     0x00000001U
#define DCB_DAUTHCTRL_SPIDENSEL_M                                   0x00000001U
#define DCB_DAUTHCTRL_SPIDENSEL_S                                            0U

//*****************************************************************************
//
// Register: DCB_O_DSCSR
//
//*****************************************************************************
// Field: [31:18] RES0
//
// Reserved, RES0
#define DCB_DSCSR_RES0_W                                                    14U
#define DCB_DSCSR_RES0_M                                            0xFFFC0000U
#define DCB_DSCSR_RES0_S                                                    18U

// Field:    [17] CDSKEY
//
// Writes to the CDS bit are ignored unless CDSKEY is concurrently written to
// zero
#define DCB_DSCSR_CDSKEY                                            0x00020000U
#define DCB_DSCSR_CDSKEY_M                                          0x00020000U
#define DCB_DSCSR_CDSKEY_S                                                  17U

// Field:    [16] CDS
//
// This field indicates the current Security state of the processor
#define DCB_DSCSR_CDS                                               0x00010000U
#define DCB_DSCSR_CDS_M                                             0x00010000U
#define DCB_DSCSR_CDS_S                                                     16U

// Field:  [15:2] RES0_1
//
// Reserved, RES0
#define DCB_DSCSR_RES0_1_W                                                  14U
#define DCB_DSCSR_RES0_1_M                                          0x0000FFFCU
#define DCB_DSCSR_RES0_1_S                                                   2U

// Field:     [1] SBRSEL
//
// If SBRSELEN is 1 this bit selects whether the Non-secure or the Secure
// version of the memory-mapped Banked registers are accessible to the debugger
#define DCB_DSCSR_SBRSEL                                            0x00000002U
#define DCB_DSCSR_SBRSEL_M                                          0x00000002U
#define DCB_DSCSR_SBRSEL_S                                                   1U

// Field:     [0] SBRSELEN
//
// Controls whether the SBRSEL field or the current Security state of the
// processor selects which version of the memory-mapped Banked registers are
// accessed to the debugger
#define DCB_DSCSR_SBRSELEN                                          0x00000001U
#define DCB_DSCSR_SBRSELEN_M                                        0x00000001U
#define DCB_DSCSR_SBRSELEN_S                                                 0U


#endif // __DCB__
