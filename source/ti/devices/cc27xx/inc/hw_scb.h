/******************************************************************************
*  Filename:       hw_scb_h
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

#ifndef __HW_SCB_H__
#define __HW_SCB_H__

//*****************************************************************************
//
// This section defines the register offsets of
// SCB component
//
//*****************************************************************************
// Provides implementation-specific minor revision information
#define SCB_O_REVIDR                                                0x00000000U

// Provides identification information for the PE, including an implementer
// code for the device and a device ID number
#define SCB_O_CPUID                                                 0x00000004U

// Controls and provides status information for NMI, PendSV, SysTick and
// interrupts
#define SCB_O_ICSR                                                  0x00000008U

// Indicates the offset of the vector table base address from memory address
// 0x00000000
#define SCB_O_VTOR                                                  0x0000000CU

// This register is used to determine data endianness, clear all active state
// information for debug or to recover from a hard failure, execute a system
// reset, alter the priority grouping position (binary point).
#define SCB_O_AIRCR                                                 0x00000010U

// This register is used for power-management functions, i.e., signaling to the
// system when the processor can enter a low power state, controlling how the
// processor enters and exits low power states.
#define SCB_O_SCR                                                   0x00000014U

// Sets or returns configuration and control data
#define SCB_O_CCR                                                   0x00000018U

// Sets or returns priority for system handlers 4 - 7
#define SCB_O_SHPR1                                                 0x0000001CU

// Sets or returns priority for system handlers 8 - 11
#define SCB_O_SHPR2                                                 0x00000020U

// Sets or returns priority for system handlers 12 - 15
#define SCB_O_SHPR3                                                 0x00000024U

// Provides access to the active and pending status of system exceptions
#define SCB_O_SHCSR                                                 0x00000028U

// Contains the three Configurable Fault Status Registers
#define SCB_O_CFSR                                                  0x0000002CU

// Shows the cause of any HardFaults
#define SCB_O_HFSR                                                  0x00000030U

// Shows which debug event occurred
#define SCB_O_DFSR                                                  0x00000034U

// Shows the address of the memory location that caused an MPU fault
#define SCB_O_MMFAR                                                 0x00000038U

// Shows the address associated with a precise data access BusFault
#define SCB_O_BFAR                                                  0x0000003CU

// This register is used to determine additional system fault information to
// software. Single-cycle high level on an auxiliary faults is latched as one.
// The bit can only be cleared by writing a one to the corresponding bit.
// Auxiliary fault inputs to the CPU are tied to 0.
#define SCB_O_AFSR                                                  0x00000040U

// Gives top-level information about the instruction set supported by the PE
#define SCB_O_ID_PFR0                                               0x00000044U

// Gives information about the programmers' model and Extensions support
#define SCB_O_ID_PFR1                                               0x00000048U

// Provides top level information about the debug system
#define SCB_O_ID_DFR0                                               0x0000004CU

// Provides information about the IMPLEMENTATION DEFINED features of the PE
#define SCB_O_ID_AFR0                                               0x00000050U

// Provides information about the implemented memory model and memory
// management support
#define SCB_O_ID_MMFR0                                              0x00000054U

// Provides information about the implemented memory model and memory
// management support
#define SCB_O_ID_MMFR1                                              0x00000058U

// Provides information about the implemented memory model and memory
// management support
#define SCB_O_ID_MMFR2                                              0x0000005CU

// Provides information about the implemented memory model and memory
// management support
#define SCB_O_ID_MMFR3                                              0x00000060U

// Provides information about the instruction set implemented by the PE
#define SCB_O_ID_ISAR0                                              0x00000064U

// Provides information about the instruction set implemented by the PE
#define SCB_O_ID_ISAR1                                              0x00000068U

// Provides information about the instruction set implemented by the PE
#define SCB_O_ID_ISAR2                                              0x0000006CU

// Provides information about the instruction set implemented by the PE
#define SCB_O_ID_ISAR3                                              0x00000070U

// Provides information about the instruction set implemented by the PE
#define SCB_O_ID_ISAR4                                              0x00000074U

// Provides information about the instruction set implemented by the PE
#define SCB_O_ID_ISAR5                                              0x00000078U

// Identifies the type of caches implemented and the level of coherency and
// unification
#define SCB_O_CLIDR                                                 0x0000007CU

// The CTR provides information about the architecture of the currently
// selected cache
#define SCB_O_CTR                                                   0x00000080U

// Provides information about the architecture of the caches. CCSIDR is RES0 if
// CLIDR is zero.
#define SCB_O_CCSIDR                                                0x00000084U

// Selects the current Cache Size ID Register, CCSIDR, by specifying the
// required cache level and the cache
#define SCB_O_CSSELR                                                0x00000088U

// Specifies the access privileges for coprocessors and the FP Extension
#define SCB_O_CPACR                                                 0x0000008CU

// Defines the Non-secure access permissions for both the FP Extension and
// coprocessors CP0 to CP7
#define SCB_O_NSACR                                                 0x00000090U

//*****************************************************************************
//
// Register: SCB_O_REVIDR
//
//*****************************************************************************
// Field:  [31:0] IMPLEMENTAION_DEFINED
//
// The contents of this field are IMPLEMENTATION DEFINED
#define SCB_REVIDR_IMPLEMENTAION_DEFINED_W                                  32U
#define SCB_REVIDR_IMPLEMENTAION_DEFINED_M                          0xFFFFFFFFU
#define SCB_REVIDR_IMPLEMENTAION_DEFINED_S                                   0U

//*****************************************************************************
//
// Register: SCB_O_CPUID
//
//*****************************************************************************
// Field: [31:24] Implementer
//
// This field must hold an implementer code that has been assigned by ARM
#define SCB_CPUID_IMPLEMENTER_W                                              8U
#define SCB_CPUID_IMPLEMENTER_M                                     0xFF000000U
#define SCB_CPUID_IMPLEMENTER_S                                             24U

// Field: [23:20] Variant
//
// IMPLEMENTATION DEFINED variant number. Typically, this field is used to
// distinguish between different product variants, or major revisions of a
// product
#define SCB_CPUID_VARIANT_W                                                  4U
#define SCB_CPUID_VARIANT_M                                         0x00F00000U
#define SCB_CPUID_VARIANT_S                                                 20U

// Field: [19:16] Architecture
//
// Defines the Architecture implemented by the PE
#define SCB_CPUID_ARCHITECTURE_W                                             4U
#define SCB_CPUID_ARCHITECTURE_M                                    0x000F0000U
#define SCB_CPUID_ARCHITECTURE_S                                            16U

// Field:  [15:4] PartNo
//
// IMPLEMENTATION DEFINED primary part number for the device
#define SCB_CPUID_PARTNO_W                                                  12U
#define SCB_CPUID_PARTNO_M                                          0x0000FFF0U
#define SCB_CPUID_PARTNO_S                                                   4U

// Field:   [3:0] Revision
//
// IMPLEMENTATION DEFINED revision number for the device
#define SCB_CPUID_REVISION_W                                                 4U
#define SCB_CPUID_REVISION_M                                        0x0000000FU
#define SCB_CPUID_REVISION_S                                                 0U

//*****************************************************************************
//
// Register: SCB_O_ICSR
//
//*****************************************************************************
// Field:    [31] PENDNMISET
//
// Indicates whether the NMI exception is pending
#define SCB_ICSR_PENDNMISET                                         0x80000000U
#define SCB_ICSR_PENDNMISET_M                                       0x80000000U
#define SCB_ICSR_PENDNMISET_S                                               31U

// Field:    [30] PENDNMICLR
//
// Allows the NMI exception pend state to be cleared
#define SCB_ICSR_PENDNMICLR                                         0x40000000U
#define SCB_ICSR_PENDNMICLR_M                                       0x40000000U
#define SCB_ICSR_PENDNMICLR_S                                               30U

// Field:    [29] RES0
//
// Reserved, RES0
#define SCB_ICSR_RES0                                               0x20000000U
#define SCB_ICSR_RES0_M                                             0x20000000U
#define SCB_ICSR_RES0_S                                                     29U

// Field:    [28] PENDSVSET
//
// Indicates whether the PendSV `FTSSS exception is pending
#define SCB_ICSR_PENDSVSET                                          0x10000000U
#define SCB_ICSR_PENDSVSET_M                                        0x10000000U
#define SCB_ICSR_PENDSVSET_S                                                28U

// Field:    [27] PENDSVCLR
//
// Allows the PendSV exception pend state to be cleared `FTSSS
#define SCB_ICSR_PENDSVCLR                                          0x08000000U
#define SCB_ICSR_PENDSVCLR_M                                        0x08000000U
#define SCB_ICSR_PENDSVCLR_S                                                27U

// Field:    [26] PENDSTSET
//
// Indicates whether the SysTick `FTSSS exception is pending
#define SCB_ICSR_PENDSTSET                                          0x04000000U
#define SCB_ICSR_PENDSTSET_M                                        0x04000000U
#define SCB_ICSR_PENDSTSET_S                                                26U

// Field:    [25] PENDSTCLR
//
// Allows the SysTick exception pend state to be cleared `FTSSS
#define SCB_ICSR_PENDSTCLR                                          0x02000000U
#define SCB_ICSR_PENDSTCLR_M                                        0x02000000U
#define SCB_ICSR_PENDSTCLR_S                                                25U

// Field:    [24] STTNS
//
// Controls whether in a single SysTick implementation, the SysTick is Secure
// or Non-secure
#define SCB_ICSR_STTNS                                              0x01000000U
#define SCB_ICSR_STTNS_M                                            0x01000000U
#define SCB_ICSR_STTNS_S                                                    24U

// Field:    [23] ISRPREEMPT
//
// Indicates whether a pending exception will be serviced on exit from debug
// halt state
#define SCB_ICSR_ISRPREEMPT                                         0x00800000U
#define SCB_ICSR_ISRPREEMPT_M                                       0x00800000U
#define SCB_ICSR_ISRPREEMPT_S                                               23U

// Field:    [22] ISRPENDING
//
// Indicates whether an external interrupt, generated by the NVIC, is pending
#define SCB_ICSR_ISRPENDING                                         0x00400000U
#define SCB_ICSR_ISRPENDING_M                                       0x00400000U
#define SCB_ICSR_ISRPENDING_S                                               22U

// Field:    [21] RES0_1
//
// Reserved, RES0
#define SCB_ICSR_RES0_1                                             0x00200000U
#define SCB_ICSR_RES0_1_M                                           0x00200000U
#define SCB_ICSR_RES0_1_S                                                   21U

// Field: [20:12] VECTPENDING
//
// The exception number of the highest priority pending and enabled interrupt
#define SCB_ICSR_VECTPENDING_W                                               9U
#define SCB_ICSR_VECTPENDING_M                                      0x001FF000U
#define SCB_ICSR_VECTPENDING_S                                              12U

// Field:    [11] RETTOBASE
//
// In Handler mode, indicates whether there is more than one active exception
#define SCB_ICSR_RETTOBASE                                          0x00000800U
#define SCB_ICSR_RETTOBASE_M                                        0x00000800U
#define SCB_ICSR_RETTOBASE_S                                                11U

// Field:  [10:9] RES0_2
//
// Reserved, RES0
#define SCB_ICSR_RES0_2_W                                                    2U
#define SCB_ICSR_RES0_2_M                                           0x00000600U
#define SCB_ICSR_RES0_2_S                                                    9U

// Field:   [8:0] VECTACTIVE
//
// The exception number of the current executing exception
#define SCB_ICSR_VECTACTIVE_W                                                9U
#define SCB_ICSR_VECTACTIVE_M                                       0x000001FFU
#define SCB_ICSR_VECTACTIVE_S                                                0U

//*****************************************************************************
//
// Register: SCB_O_VTOR
//
//*****************************************************************************
// Field:  [31:7] TBLOFF
//
// Bits 31 down to 7 of the vector table base offset.
#define SCB_VTOR_TBLOFF_W                                                   25U
#define SCB_VTOR_TBLOFF_M                                           0xFFFFFF80U
#define SCB_VTOR_TBLOFF_S                                                    7U

// Field:   [6:0] RES0
//
// Software should not rely on the value of a reserved. Writing any other value
// than the reset value may result in undefined behavior.
#define SCB_VTOR_RES0_W                                                      7U
#define SCB_VTOR_RES0_M                                             0x0000007FU
#define SCB_VTOR_RES0_S                                                      0U

//*****************************************************************************
//
// Register: SCB_O_AIRCR
//
//*****************************************************************************
// Field: [31:16] VECTKEY
//
// Register key. Writing to this register (AIRCR) requires 0x05FA in VECTKEY.
// Otherwise the write value is ignored. Read always returns 0xFA05.
#define SCB_AIRCR_VECTKEY_W                                                 16U
#define SCB_AIRCR_VECTKEY_M                                         0xFFFF0000U
#define SCB_AIRCR_VECTKEY_S                                                 16U

// Field:    [15] ENDIANESS
//
// Data endianness bit
// 0 Little-endian.
// 1 Big-endian.
#define SCB_AIRCR_ENDIANESS                                         0x00008000U
#define SCB_AIRCR_ENDIANESS_M                                       0x00008000U
#define SCB_AIRCR_ENDIANESS_S                                               15U

// Field:    [14] PRIS
//
// Prioritize Secure exceptions. The value of this bit defines whether Secure
// exception priority boosting is
// enabled.
#define SCB_AIRCR_PRIS                                              0x00004000U
#define SCB_AIRCR_PRIS_M                                            0x00004000U
#define SCB_AIRCR_PRIS_S                                                    14U

// Field:    [13] BFHFNMINS
//
// BusFault, HardFault, and NMI Non-secure enable. The value of this bit
// defines whether BusFault and NMI
// exceptions are Non-secure, and whether exceptions target the Non-secure
// HardFault exception
// 0x0 BusFault, HardFault, and NMI are Secure.
// 0x1 BusFault and NMI are Non-secure and exceptions can target Non-secure
// HardFault.
#define SCB_AIRCR_BFHFNMINS                                         0x00002000U
#define SCB_AIRCR_BFHFNMINS_M                                       0x00002000U
#define SCB_AIRCR_BFHFNMINS_S                                               13U

// Field:  [10:8] PRIGROUP
//
// Interrupt priority grouping field. This field determines the split of group
// priority from
// subpriority
#define SCB_AIRCR_PRIGROUP_W                                                 3U
#define SCB_AIRCR_PRIGROUP_M                                        0x00000700U
#define SCB_AIRCR_PRIGROUP_S                                                 8U

// Field:   [7:4] RES4
//
// Software should not rely on the value of a reserved. Writing any other value
// than the reset value may result in undefined behavior.
#define SCB_AIRCR_RES4_W                                                     4U
#define SCB_AIRCR_RES4_M                                            0x000000F0U
#define SCB_AIRCR_RES4_S                                                     4U

// Field:     [3] SYSRESETREQS
//
// System reset request Secure only. The value of this bit defines whether the
// SYSRESETREQ bit is functional
// for Non-secure use
#define SCB_AIRCR_SYSRESETREQS                                      0x00000008U
#define SCB_AIRCR_SYSRESETREQS_M                                    0x00000008U
#define SCB_AIRCR_SYSRESETREQS_S                                             3U

// Field:     [2] SYSRESETREQ
//
// System reset request. This bit allows software or a debugger to request a
// system reset:
// 0 Do not request a system reset.
// 1 Request a system reset.
// This bit is not banked between Security states.
#define SCB_AIRCR_SYSRESETREQ                                       0x00000004U
#define SCB_AIRCR_SYSRESETREQ_M                                     0x00000004U
#define SCB_AIRCR_SYSRESETREQ_S                                              2U

// Field:     [1] VECTCLRACTIVE
//
// Reserved for Debug use. This bit reads as 0. When writing to the register
// you must write
// 0 to this bit, otherwise behavior is UNPREDICTABLE.
#define SCB_AIRCR_VECTCLRACTIVE                                     0x00000002U
#define SCB_AIRCR_VECTCLRACTIVE_M                                   0x00000002U
#define SCB_AIRCR_VECTCLRACTIVE_S                                            1U

// Field:     [0] RES0
//
// Software should not rely on the value of a reserved. Writing any other value
// than the reset value may result in undefined behavior.
#define SCB_AIRCR_RES0                                              0x00000001U
#define SCB_AIRCR_RES0_M                                            0x00000001U
#define SCB_AIRCR_RES0_S                                                     0U

//*****************************************************************************
//
// Register: SCB_O_SCR
//
//*****************************************************************************
// Field:     [4] SEVONPEND
//
// Send Event on Pending bit:
// 0 Only enabled interrupts or events can wakeup the processor, disabled
// interrupts are excluded.
// 1 Enabled events and all interrupts, including disabled interrupts, can
// wakeup the processor.
// When an event or interrupt enters pending state, the event signal wakes up
// the processor from WFE. If the
// processor is not waiting for an event, the event is registered and affects
// the next WFE.
// The processor also wakes up on execution of an SEV instruction or an
// external event.
// This bit is banked between Security states.
#define SCB_SCR_SEVONPEND                                           0x00000010U
#define SCB_SCR_SEVONPEND_M                                         0x00000010U
#define SCB_SCR_SEVONPEND_S                                                  4U

// Field:     [3] SLEEPDEEPS
//
// Controls whether the SLEEPDEEP bit is only accessible from the Secure state:
// 0 The SLEEPDEEP bit accessible from both Security states.
// 1 The SLEEPDEEP bit behaves as RAZ/WI when accessed from the Non-secure
// state.
// This bit in only accessible from the Secure state, and behaves as RAZ/WI
// when accessed from the Nonsecure
// state.
// This bit is not banked between Security states.
#define SCB_SCR_SLEEPDEEPS                                          0x00000008U
#define SCB_SCR_SLEEPDEEPS_M                                        0x00000008U
#define SCB_SCR_SLEEPDEEPS_S                                                 3U

// Field:     [2] SLEEPDEEP
//
// Controls whether the processor uses sleep or deep sleep as its low power
// mode.
// 0 Sleep.
// 1 Deep sleep.
// This bit is not banked between Security states.
#define SCB_SCR_SLEEPDEEP                                           0x00000004U
#define SCB_SCR_SLEEPDEEP_M                                         0x00000004U
#define SCB_SCR_SLEEPDEEP_S                                                  2U

// Field:     [1] SLEEPONEXIT
//
// Indicates sleep-on-exit when returning from Handler mode to Thread mode:
// 0 Do not sleep when returning to Thread mode.
// 1 Enter sleep, or deep sleep, on return from an ISR.
// Setting this bit to 1 enables an interrupt driven application to avoid
// returning to an empty main application.
// This bit is banked between Security states.
#define SCB_SCR_SLEEPONEXIT                                         0x00000002U
#define SCB_SCR_SLEEPONEXIT_M                                       0x00000002U
#define SCB_SCR_SLEEPONEXIT_S                                                1U

//*****************************************************************************
//
// Register: SCB_O_CCR
//
//*****************************************************************************
// Field: [31:19] RES0
//
// Reserved, RES0
#define SCB_CCR_RES0_W                                                      13U
#define SCB_CCR_RES0_M                                              0xFFF80000U
#define SCB_CCR_RES0_S                                                      19U

// Field:    [18] BP
//
// Enables program flow prediction `FTSSS
#define SCB_CCR_BP                                                  0x00040000U
#define SCB_CCR_BP_M                                                0x00040000U
#define SCB_CCR_BP_S                                                        18U

// Field:    [17] IC
//
// This is a global enable bit for instruction caches in the selected Security
// state
#define SCB_CCR_IC                                                  0x00020000U
#define SCB_CCR_IC_M                                                0x00020000U
#define SCB_CCR_IC_S                                                        17U

// Field:    [16] DC
//
// Enables data caching of all data accesses to Normal memory `FTSSS
#define SCB_CCR_DC                                                  0x00010000U
#define SCB_CCR_DC_M                                                0x00010000U
#define SCB_CCR_DC_S                                                        16U

// Field: [15:11] RES0_1
//
// Reserved, RES0
#define SCB_CCR_RES0_1_W                                                     5U
#define SCB_CCR_RES0_1_M                                            0x0000F800U
#define SCB_CCR_RES0_1_S                                                    11U

// Field:    [10] STKOFHFNMIGN
//
// Controls the effect of a stack limit violation while executing at a
// requested priority less than 0
#define SCB_CCR_STKOFHFNMIGN                                        0x00000400U
#define SCB_CCR_STKOFHFNMIGN_M                                      0x00000400U
#define SCB_CCR_STKOFHFNMIGN_S                                              10U

// Field:     [9] RES1
//
// Reserved, RES1
#define SCB_CCR_RES1                                                0x00000200U
#define SCB_CCR_RES1_M                                              0x00000200U
#define SCB_CCR_RES1_S                                                       9U

// Field:     [8] BFHFNMIGN
//
// Determines the effect of precise BusFaults on handlers running at a
// requested priority less than 0
#define SCB_CCR_BFHFNMIGN                                           0x00000100U
#define SCB_CCR_BFHFNMIGN_M                                         0x00000100U
#define SCB_CCR_BFHFNMIGN_S                                                  8U

// Field:   [7:5] RES0_2
//
// Reserved, RES0
#define SCB_CCR_RES0_2_W                                                     3U
#define SCB_CCR_RES0_2_M                                            0x000000E0U
#define SCB_CCR_RES0_2_S                                                     5U

// Field:     [4] DIV_0_TRP
//
// Controls the generation of a DIVBYZERO UsageFault when attempting to perform
// integer division by zero
#define SCB_CCR_DIV_0_TRP                                           0x00000010U
#define SCB_CCR_DIV_0_TRP_M                                         0x00000010U
#define SCB_CCR_DIV_0_TRP_S                                                  4U

// Field:     [3] UNALIGN_TRP
//
// Controls the trapping of unaligned word or halfword accesses
#define SCB_CCR_UNALIGN_TRP                                         0x00000008U
#define SCB_CCR_UNALIGN_TRP_M                                       0x00000008U
#define SCB_CCR_UNALIGN_TRP_S                                                3U

// Field:     [2] RES0_3
//
// Reserved, RES0
#define SCB_CCR_RES0_3                                              0x00000004U
#define SCB_CCR_RES0_3_M                                            0x00000004U
#define SCB_CCR_RES0_3_S                                                     2U

// Field:     [1] USERSETMPEND
//
// Determines whether unprivileged accesses are permitted to pend interrupts
// via the STIR
#define SCB_CCR_USERSETMPEND                                        0x00000002U
#define SCB_CCR_USERSETMPEND_M                                      0x00000002U
#define SCB_CCR_USERSETMPEND_S                                               1U

// Field:     [0] RES1_1
//
// Reserved, RES1
#define SCB_CCR_RES1_1                                              0x00000001U
#define SCB_CCR_RES1_1_M                                            0x00000001U
#define SCB_CCR_RES1_1_S                                                     0U

//*****************************************************************************
//
// Register: SCB_O_SHPR1
//
//*****************************************************************************
// Field: [31:24] PRI_7
//
// Priority of system handler 7, SecureFault
#define SCB_SHPR1_PRI_7_W                                                    8U
#define SCB_SHPR1_PRI_7_M                                           0xFF000000U
#define SCB_SHPR1_PRI_7_S                                                   24U

// Field: [23:16] PRI_6
//
// Priority of system handler 6, UsageFault
#define SCB_SHPR1_PRI_6_W                                                    8U
#define SCB_SHPR1_PRI_6_M                                           0x00FF0000U
#define SCB_SHPR1_PRI_6_S                                                   16U

// Field:  [15:8] PRI_5
//
// Priority of system handler 5, BusFault
#define SCB_SHPR1_PRI_5_W                                                    8U
#define SCB_SHPR1_PRI_5_M                                           0x0000FF00U
#define SCB_SHPR1_PRI_5_S                                                    8U

// Field:   [7:0] PRI_4
//
// Priority of system handler 4, MemManage
#define SCB_SHPR1_PRI_4_W                                                    8U
#define SCB_SHPR1_PRI_4_M                                           0x000000FFU
#define SCB_SHPR1_PRI_4_S                                                    0U

//*****************************************************************************
//
// Register: SCB_O_SHPR2
//
//*****************************************************************************
// Field: [31:24] PRI_11
//
// Priority of system handler 11, SVCall
#define SCB_SHPR2_PRI_11_W                                                   8U
#define SCB_SHPR2_PRI_11_M                                          0xFF000000U
#define SCB_SHPR2_PRI_11_S                                                  24U

// Field:  [23:0] RES0
//
// Reserved, RES0
#define SCB_SHPR2_RES0_W                                                    24U
#define SCB_SHPR2_RES0_M                                            0x00FFFFFFU
#define SCB_SHPR2_RES0_S                                                     0U

//*****************************************************************************
//
// Register: SCB_O_SHPR3
//
//*****************************************************************************
// Field: [31:24] PRI_15
//
// Priority of system handler 15, SysTick
#define SCB_SHPR3_PRI_15_W                                                   8U
#define SCB_SHPR3_PRI_15_M                                          0xFF000000U
#define SCB_SHPR3_PRI_15_S                                                  24U

// Field: [23:16] PRI_14
//
// Priority of system handler 14, PendSV
#define SCB_SHPR3_PRI_14_W                                                   8U
#define SCB_SHPR3_PRI_14_M                                          0x00FF0000U
#define SCB_SHPR3_PRI_14_S                                                  16U

// Field:  [15:0] RES0_0
//
// Reserved, RES0
#define SCB_SHPR3_RES0_0_W                                                  16U
#define SCB_SHPR3_RES0_0_M                                          0x0000FFFFU
#define SCB_SHPR3_RES0_0_S                                                   0U

//*****************************************************************************
//
// Register: SCB_O_SHCSR
//
//*****************************************************************************
// Field: [31:22] RES0
//
// Reserved, RES0
#define SCB_SHCSR_RES0_W                                                    10U
#define SCB_SHCSR_RES0_M                                            0xFFC00000U
#define SCB_SHCSR_RES0_S                                                    22U

// Field:    [21] HARDFAULTPENDED
//
// `IAAMO the pending state of the HardFault exception `CTTSSS
#define SCB_SHCSR_HARDFAULTPENDED                                   0x00200000U
#define SCB_SHCSR_HARDFAULTPENDED_M                                 0x00200000U
#define SCB_SHCSR_HARDFAULTPENDED_S                                         21U

// Field:    [20] SECUREFAULTPENDED
//
// `IAAMO the pending state of the SecureFault exception
#define SCB_SHCSR_SECUREFAULTPENDED                                 0x00100000U
#define SCB_SHCSR_SECUREFAULTPENDED_M                               0x00100000U
#define SCB_SHCSR_SECUREFAULTPENDED_S                                       20U

// Field:    [19] SECUREFAULTENA
//
// `DW the SecureFault exception is enabled
#define SCB_SHCSR_SECUREFAULTENA                                    0x00080000U
#define SCB_SHCSR_SECUREFAULTENA_M                                  0x00080000U
#define SCB_SHCSR_SECUREFAULTENA_S                                          19U

// Field:    [18] USGFAULTENA
//
// `DW the UsageFault exception is enabled `FTSSS
#define SCB_SHCSR_USGFAULTENA                                       0x00040000U
#define SCB_SHCSR_USGFAULTENA_M                                     0x00040000U
#define SCB_SHCSR_USGFAULTENA_S                                             18U

// Field:    [17] BUSFAULTENA
//
// `DW the BusFault exception is enabled
#define SCB_SHCSR_BUSFAULTENA                                       0x00020000U
#define SCB_SHCSR_BUSFAULTENA_M                                     0x00020000U
#define SCB_SHCSR_BUSFAULTENA_S                                             17U

// Field:    [16] MEMFAULTENA
//
// `DW the MemManage exception is enabled `FTSSS
#define SCB_SHCSR_MEMFAULTENA                                       0x00010000U
#define SCB_SHCSR_MEMFAULTENA_M                                     0x00010000U
#define SCB_SHCSR_MEMFAULTENA_S                                             16U

// Field:    [15] SVCALLPENDED
//
// `IAAMO the pending state of the SVCall exception `FTSSS
#define SCB_SHCSR_SVCALLPENDED                                      0x00008000U
#define SCB_SHCSR_SVCALLPENDED_M                                    0x00008000U
#define SCB_SHCSR_SVCALLPENDED_S                                            15U

// Field:    [14] BUSFAULTPENDED
//
// `IAAMO the pending state of the BusFault exception
#define SCB_SHCSR_BUSFAULTPENDED                                    0x00004000U
#define SCB_SHCSR_BUSFAULTPENDED_M                                  0x00004000U
#define SCB_SHCSR_BUSFAULTPENDED_S                                          14U

// Field:    [13] MEMFAULTPENDED
//
// `IAAMO the pending state of the MemManage exception `FTSSS
#define SCB_SHCSR_MEMFAULTPENDED                                    0x00002000U
#define SCB_SHCSR_MEMFAULTPENDED_M                                  0x00002000U
#define SCB_SHCSR_MEMFAULTPENDED_S                                          13U

// Field:    [12] USGFAULTPENDED
//
// The UsageFault exception is banked between Security states, `IAAMO the
// pending state of the UsageFault exception `FTSSS
#define SCB_SHCSR_USGFAULTPENDED                                    0x00001000U
#define SCB_SHCSR_USGFAULTPENDED_M                                  0x00001000U
#define SCB_SHCSR_USGFAULTPENDED_S                                          12U

// Field:    [11] SYSTICKACT
//
// `IAAMO the active state of the SysTick exception `FTSSS
#define SCB_SHCSR_SYSTICKACT                                        0x00000800U
#define SCB_SHCSR_SYSTICKACT_M                                      0x00000800U
#define SCB_SHCSR_SYSTICKACT_S                                              11U

// Field:    [10] PENDSVACT
//
// `IAAMO the active state of the PendSV exception `FTSSS
#define SCB_SHCSR_PENDSVACT                                         0x00000400U
#define SCB_SHCSR_PENDSVACT_M                                       0x00000400U
#define SCB_SHCSR_PENDSVACT_S                                               10U

// Field:     [9] RES0_1
//
// Reserved, RES0
#define SCB_SHCSR_RES0_1                                            0x00000200U
#define SCB_SHCSR_RES0_1_M                                          0x00000200U
#define SCB_SHCSR_RES0_1_S                                                   9U

// Field:     [8] MONITORACT
//
// `IAAMO the active state of the DebugMonitor exception
#define SCB_SHCSR_MONITORACT                                        0x00000100U
#define SCB_SHCSR_MONITORACT_M                                      0x00000100U
#define SCB_SHCSR_MONITORACT_S                                               8U

// Field:     [7] SVCALLACT
//
// `IAAMO the active state of the SVCall exception `FTSSS
#define SCB_SHCSR_SVCALLACT                                         0x00000080U
#define SCB_SHCSR_SVCALLACT_M                                       0x00000080U
#define SCB_SHCSR_SVCALLACT_S                                                7U

// Field:     [6] RES0_2
//
// Reserved, RES0
#define SCB_SHCSR_RES0_2                                            0x00000040U
#define SCB_SHCSR_RES0_2_M                                          0x00000040U
#define SCB_SHCSR_RES0_2_S                                                   6U

// Field:     [5] NMIACT
//
// `IAAMO the active state of the NMI exception
#define SCB_SHCSR_NMIACT                                            0x00000020U
#define SCB_SHCSR_NMIACT_M                                          0x00000020U
#define SCB_SHCSR_NMIACT_S                                                   5U

// Field:     [4] SECUREFAULTACT
//
// `IAAMO the active state of the SecureFault exception
#define SCB_SHCSR_SECUREFAULTACT                                    0x00000010U
#define SCB_SHCSR_SECUREFAULTACT_M                                  0x00000010U
#define SCB_SHCSR_SECUREFAULTACT_S                                           4U

// Field:     [3] USGFAULTACT
//
// `IAAMO the active state of the UsageFault exception `FTSSS
#define SCB_SHCSR_USGFAULTACT                                       0x00000008U
#define SCB_SHCSR_USGFAULTACT_M                                     0x00000008U
#define SCB_SHCSR_USGFAULTACT_S                                              3U

// Field:     [2] HARDFAULTACT
//
// Indicates and allows limited modification of the active state of the
// HardFault exception `FTSSS
#define SCB_SHCSR_HARDFAULTACT                                      0x00000004U
#define SCB_SHCSR_HARDFAULTACT_M                                    0x00000004U
#define SCB_SHCSR_HARDFAULTACT_S                                             2U

// Field:     [1] BUSFAULTACT
//
// `IAAMO the active state of the BusFault exception
#define SCB_SHCSR_BUSFAULTACT                                       0x00000002U
#define SCB_SHCSR_BUSFAULTACT_M                                     0x00000002U
#define SCB_SHCSR_BUSFAULTACT_S                                              1U

// Field:     [0] MEMFAULTACT
//
// `IAAMO the active state of the MemManage exception `FTSSS
#define SCB_SHCSR_MEMFAULTACT                                       0x00000001U
#define SCB_SHCSR_MEMFAULTACT_M                                     0x00000001U
#define SCB_SHCSR_MEMFAULTACT_S                                              0U

//*****************************************************************************
//
// Register: SCB_O_CFSR
//
//*****************************************************************************
// Field: [31:26] RES0_3
//
// Reserved, RES0
#define SCB_CFSR_RES0_3_W                                                    6U
#define SCB_CFSR_RES0_3_M                                           0xFC000000U
#define SCB_CFSR_RES0_3_S                                                   26U

// Field:    [25] DIVBYZERO
//
// Sticky flag indicating whether an integer division by zero error has
// occurred
#define SCB_CFSR_DIVBYZERO                                          0x02000000U
#define SCB_CFSR_DIVBYZERO_M                                        0x02000000U
#define SCB_CFSR_DIVBYZERO_S                                                25U

// Field:    [24] UNALIGNED
//
// Sticky flag indicating whether an unaligned access error has occurred
#define SCB_CFSR_UNALIGNED                                          0x01000000U
#define SCB_CFSR_UNALIGNED_M                                        0x01000000U
#define SCB_CFSR_UNALIGNED_S                                                24U

// Field: [23:21] RES0_1_2
//
// Reserved, RES0
#define SCB_CFSR_RES0_1_2_W                                                  3U
#define SCB_CFSR_RES0_1_2_M                                         0x00E00000U
#define SCB_CFSR_RES0_1_2_S                                                 21U

// Field:    [20] STKOF
//
// Sticky flag indicating whether a stack overflow error has occurred
#define SCB_CFSR_STKOF                                              0x00100000U
#define SCB_CFSR_STKOF_M                                            0x00100000U
#define SCB_CFSR_STKOF_S                                                    20U

// Field:    [19] NOCP
//
// Sticky flag indicating whether a coprocessor disabled or not present error
// has occurred
#define SCB_CFSR_NOCP                                               0x00080000U
#define SCB_CFSR_NOCP_M                                             0x00080000U
#define SCB_CFSR_NOCP_S                                                     19U

// Field:    [18] INVPC
//
// Sticky flag indicating whether an integrity check error has occurred
#define SCB_CFSR_INVPC                                              0x00040000U
#define SCB_CFSR_INVPC_M                                            0x00040000U
#define SCB_CFSR_INVPC_S                                                    18U

// Field:    [17] INVSTATE
//
// Sticky flag indicating whether an EPSR.T or EPSR.IT validity error has
// occurred
#define SCB_CFSR_INVSTATE                                           0x00020000U
#define SCB_CFSR_INVSTATE_M                                         0x00020000U
#define SCB_CFSR_INVSTATE_S                                                 17U

// Field:    [16] UNDEFINSTR
//
// Sticky flag indicating whether an undefined instruction error has occurred
#define SCB_CFSR_UNDEFINSTR                                         0x00010000U
#define SCB_CFSR_UNDEFINSTR_M                                       0x00010000U
#define SCB_CFSR_UNDEFINSTR_S                                               16U

// Field:    [15] BFARVALID
//
// Indicates validity of the contents of the BFAR register
#define SCB_CFSR_BFARVALID                                          0x00008000U
#define SCB_CFSR_BFARVALID_M                                        0x00008000U
#define SCB_CFSR_BFARVALID_S                                                15U

// Field:    [14] RES0_2
//
// Reserved, RES0
#define SCB_CFSR_RES0_2                                             0x00004000U
#define SCB_CFSR_RES0_2_M                                           0x00004000U
#define SCB_CFSR_RES0_2_S                                                   14U

// Field:    [13] LSPERR
//
// Records whether a BusFault occurred during FP lazy state preservation
#define SCB_CFSR_LSPERR                                             0x00002000U
#define SCB_CFSR_LSPERR_M                                           0x00002000U
#define SCB_CFSR_LSPERR_S                                                   13U

// Field:    [12] STKERR
//
// Records whether a derived BusFault occurred during exception entry stacking
#define SCB_CFSR_STKERR                                             0x00001000U
#define SCB_CFSR_STKERR_M                                           0x00001000U
#define SCB_CFSR_STKERR_S                                                   12U

// Field:    [11] UNSTKERR
//
// Records whether a derived BusFault occurred during exception return
// unstacking
#define SCB_CFSR_UNSTKERR                                           0x00000800U
#define SCB_CFSR_UNSTKERR_M                                         0x00000800U
#define SCB_CFSR_UNSTKERR_S                                                 11U

// Field:    [10] IMPRECISERR
//
// Records whether an imprecise data access error has occurred
#define SCB_CFSR_IMPRECISERR                                        0x00000400U
#define SCB_CFSR_IMPRECISERR_M                                      0x00000400U
#define SCB_CFSR_IMPRECISERR_S                                              10U

// Field:     [9] PRECISERR
//
// Records whether a precise data access error has occurred
#define SCB_CFSR_PRECISERR                                          0x00000200U
#define SCB_CFSR_PRECISERR_M                                        0x00000200U
#define SCB_CFSR_PRECISERR_S                                                 9U

// Field:     [8] IBUSERR
//
// Records whether a BusFault on an instruction prefetch has occurred
#define SCB_CFSR_IBUSERR                                            0x00000100U
#define SCB_CFSR_IBUSERR_M                                          0x00000100U
#define SCB_CFSR_IBUSERR_S                                                   8U

// Field:     [7] MMARVALID
//
// Indicates validity of the MMFAR register
#define SCB_CFSR_MMARVALID                                          0x00000080U
#define SCB_CFSR_MMARVALID_M                                        0x00000080U
#define SCB_CFSR_MMARVALID_S                                                 7U

// Field:     [6] RES0
//
// Reserved, RES0
#define SCB_CFSR_RES0                                               0x00000040U
#define SCB_CFSR_RES0_M                                             0x00000040U
#define SCB_CFSR_RES0_S                                                      6U

// Field:     [5] MLSPERR
//
// Records whether a MemManage fault occurred during FP lazy state preservation
#define SCB_CFSR_MLSPERR                                            0x00000020U
#define SCB_CFSR_MLSPERR_M                                          0x00000020U
#define SCB_CFSR_MLSPERR_S                                                   5U

// Field:     [4] MSTKERR
//
// Records whether a derived MemManage fault occurred during exception entry
// stacking
#define SCB_CFSR_MSTKERR                                            0x00000010U
#define SCB_CFSR_MSTKERR_M                                          0x00000010U
#define SCB_CFSR_MSTKERR_S                                                   4U

// Field:     [3] MUNSTKERR
//
// Records whether a derived MemManage fault occurred during exception return
// unstacking
#define SCB_CFSR_MUNSTKERR                                          0x00000008U
#define SCB_CFSR_MUNSTKERR_M                                        0x00000008U
#define SCB_CFSR_MUNSTKERR_S                                                 3U

// Field:     [2] RES0_1
//
// Reserved, RES0
#define SCB_CFSR_RES0_1                                             0x00000004U
#define SCB_CFSR_RES0_1_M                                           0x00000004U
#define SCB_CFSR_RES0_1_S                                                    2U

// Field:     [1] DACCVIOL
//
// Records whether a data access violation has occurred
#define SCB_CFSR_DACCVIOL                                           0x00000002U
#define SCB_CFSR_DACCVIOL_M                                         0x00000002U
#define SCB_CFSR_DACCVIOL_S                                                  1U

// Field:     [0] IACCVIOL
//
// Records whether an instruction related memory access violation has occurred
#define SCB_CFSR_IACCVIOL                                           0x00000001U
#define SCB_CFSR_IACCVIOL_M                                         0x00000001U
#define SCB_CFSR_IACCVIOL_S                                                  0U

//*****************************************************************************
//
// Register: SCB_O_HFSR
//
//*****************************************************************************
// Field:    [31] DEBUGEVT
//
// Indicates when a Debug event has occurred
#define SCB_HFSR_DEBUGEVT                                           0x80000000U
#define SCB_HFSR_DEBUGEVT_M                                         0x80000000U
#define SCB_HFSR_DEBUGEVT_S                                                 31U

// Field:    [30] FORCED
//
// Indicates that a fault with configurable priority has been escalated to a
// HardFault exception, because it could not be made active, because of
// priority, or because it was disabled
#define SCB_HFSR_FORCED                                             0x40000000U
#define SCB_HFSR_FORCED_M                                           0x40000000U
#define SCB_HFSR_FORCED_S                                                   30U

// Field:  [29:2] RES0
//
// Reserved, RES0
#define SCB_HFSR_RES0_W                                                     28U
#define SCB_HFSR_RES0_M                                             0x3FFFFFFCU
#define SCB_HFSR_RES0_S                                                      2U

// Field:     [1] VECTTBL
//
// Indicates when a fault has occurred because of a vector table read error on
// exception processing
#define SCB_HFSR_VECTTBL                                            0x00000002U
#define SCB_HFSR_VECTTBL_M                                          0x00000002U
#define SCB_HFSR_VECTTBL_S                                                   1U

// Field:     [0] RES0_1
//
// Reserved, RES0
#define SCB_HFSR_RES0_1                                             0x00000001U
#define SCB_HFSR_RES0_1_M                                           0x00000001U
#define SCB_HFSR_RES0_1_S                                                    0U

//*****************************************************************************
//
// Register: SCB_O_DFSR
//
//*****************************************************************************
// Field:  [31:5] RES0
//
// Reserved, RES0
#define SCB_DFSR_RES0_W                                                     27U
#define SCB_DFSR_RES0_M                                             0xFFFFFFE0U
#define SCB_DFSR_RES0_S                                                      5U

// Field:     [4] EXTERNAL
//
// Sticky flag indicating whether an External debug request debug event has
// occurred
#define SCB_DFSR_EXTERNAL                                           0x00000010U
#define SCB_DFSR_EXTERNAL_M                                         0x00000010U
#define SCB_DFSR_EXTERNAL_S                                                  4U

// Field:     [3] VCATCH
//
// Sticky flag indicating whether a Vector catch debug event has occurred
#define SCB_DFSR_VCATCH                                             0x00000008U
#define SCB_DFSR_VCATCH_M                                           0x00000008U
#define SCB_DFSR_VCATCH_S                                                    3U

// Field:     [2] DWTTRAP
//
// Sticky flag indicating whether a Watchpoint debug event has occurred
#define SCB_DFSR_DWTTRAP                                            0x00000004U
#define SCB_DFSR_DWTTRAP_M                                          0x00000004U
#define SCB_DFSR_DWTTRAP_S                                                   2U

// Field:     [1] BKPT
//
// Sticky flag indicating whether a Breakpoint debug event has occurred
#define SCB_DFSR_BKPT                                               0x00000002U
#define SCB_DFSR_BKPT_M                                             0x00000002U
#define SCB_DFSR_BKPT_S                                                      1U

// Field:     [0] HALTED
//
// Sticky flag indicating that a Halt request debug event or Step debug event
// has occurred
#define SCB_DFSR_HALTED                                             0x00000001U
#define SCB_DFSR_HALTED_M                                           0x00000001U
#define SCB_DFSR_HALTED_S                                                    0U

//*****************************************************************************
//
// Register: SCB_O_MMFAR
//
//*****************************************************************************
// Field:  [31:0] ADDRESS
//
// This register is updated with the address of a location that produced a
// MemManage fault. The MMFSR shows the cause of the fault, and whether this
// field is valid. This field is valid only when MMFSR.MMARVALID is set,
// otherwise it is UNKNOWN
#define SCB_MMFAR_ADDRESS_W                                                 32U
#define SCB_MMFAR_ADDRESS_M                                         0xFFFFFFFFU
#define SCB_MMFAR_ADDRESS_S                                                  0U

//*****************************************************************************
//
// Register: SCB_O_BFAR
//
//*****************************************************************************
// Field:  [31:0] ADDRESS
//
// This register is updated with the address of a location that produced a
// BusFault. The BFSR shows the reason for the fault. This field is valid only
// when BFSR.BFARVALID is set, otherwise it is UNKNOWN
#define SCB_BFAR_ADDRESS_W                                                  32U
#define SCB_BFAR_ADDRESS_M                                          0xFFFFFFFFU
#define SCB_BFAR_ADDRESS_S                                                   0U

//*****************************************************************************
//
// Register: SCB_O_AFSR
//
//*****************************************************************************
// Field:     [0] IMPDEF
//
// Implementation defined. The bits map directly onto the signal assignment to
// the auxiliary fault inputs. Tied to 0
#define SCB_AFSR_IMPDEF                                             0x00000001U
#define SCB_AFSR_IMPDEF_M                                           0x00000001U
#define SCB_AFSR_IMPDEF_S                                                    0U

//*****************************************************************************
//
// Register: SCB_O_ID_PFR0
//
//*****************************************************************************
// Field:  [31:8] RES0
//
// Reserved, RES0
#define SCB_ID_PFR0_RES0_W                                                  24U
#define SCB_ID_PFR0_RES0_M                                          0xFFFFFF00U
#define SCB_ID_PFR0_RES0_S                                                   8U

// Field:   [7:4] State1
//
// T32 instruction set support
#define SCB_ID_PFR0_STATE1_W                                                 4U
#define SCB_ID_PFR0_STATE1_M                                        0x000000F0U
#define SCB_ID_PFR0_STATE1_S                                                 4U

// Field:   [3:0] State0
//
// A32 instruction set support
#define SCB_ID_PFR0_STATE0_W                                                 4U
#define SCB_ID_PFR0_STATE0_M                                        0x0000000FU
#define SCB_ID_PFR0_STATE0_S                                                 0U

//*****************************************************************************
//
// Register: SCB_O_ID_PFR1
//
//*****************************************************************************
// Field: [31:12] RES0
//
// Reserved, RES0
#define SCB_ID_PFR1_RES0_W                                                  20U
#define SCB_ID_PFR1_RES0_M                                          0xFFFFF000U
#define SCB_ID_PFR1_RES0_S                                                  12U

// Field:  [11:8] MProgMod
//
// Identifies support for the M-Profile programmers' model support
#define SCB_ID_PFR1_MPROGMOD_W                                               4U
#define SCB_ID_PFR1_MPROGMOD_M                                      0x00000F00U
#define SCB_ID_PFR1_MPROGMOD_S                                               8U

// Field:   [7:4] Security
//
// Identifies whether the Security Extension is implemented
#define SCB_ID_PFR1_SECURITY_W                                               4U
#define SCB_ID_PFR1_SECURITY_M                                      0x000000F0U
#define SCB_ID_PFR1_SECURITY_S                                               4U

// Field:   [3:0] RES0_1
//
// Reserved, RES0
#define SCB_ID_PFR1_RES0_1_W                                                 4U
#define SCB_ID_PFR1_RES0_1_M                                        0x0000000FU
#define SCB_ID_PFR1_RES0_1_S                                                 0U

//*****************************************************************************
//
// Register: SCB_O_ID_DFR0
//
//*****************************************************************************
// Field: [31:24] RES0
//
// Reserved, RES0
#define SCB_ID_DFR0_RES0_W                                                   8U
#define SCB_ID_DFR0_RES0_M                                          0xFF000000U
#define SCB_ID_DFR0_RES0_S                                                  24U

// Field: [23:20] MProfDbg
//
// Indicates the supported M-profile debug architecture
#define SCB_ID_DFR0_MPROFDBG_W                                               4U
#define SCB_ID_DFR0_MPROFDBG_M                                      0x00F00000U
#define SCB_ID_DFR0_MPROFDBG_S                                              20U

// Field:  [19:0] RES0_1
//
// Reserved, RES0
#define SCB_ID_DFR0_RES0_1_W                                                20U
#define SCB_ID_DFR0_RES0_1_M                                        0x000FFFFFU
#define SCB_ID_DFR0_RES0_1_S                                                 0U

//*****************************************************************************
//
// Register: SCB_O_ID_AFR0
//
//*****************************************************************************
// Field: [31:16] RES0
//
// Reserved, RES0
#define SCB_ID_AFR0_RES0_W                                                  16U
#define SCB_ID_AFR0_RES0_M                                          0xFFFF0000U
#define SCB_ID_AFR0_RES0_S                                                  16U

// Field: [15:12] IMPDEF3
//
// IMPLEMENTATION DEFINED meaning
#define SCB_ID_AFR0_IMPDEF3_W                                                4U
#define SCB_ID_AFR0_IMPDEF3_M                                       0x0000F000U
#define SCB_ID_AFR0_IMPDEF3_S                                               12U

// Field:  [11:8] IMPDEF2
//
// IMPLEMENTATION DEFINED meaning
#define SCB_ID_AFR0_IMPDEF2_W                                                4U
#define SCB_ID_AFR0_IMPDEF2_M                                       0x00000F00U
#define SCB_ID_AFR0_IMPDEF2_S                                                8U

// Field:   [7:4] IMPDEF1
//
// IMPLEMENTATION DEFINED meaning
#define SCB_ID_AFR0_IMPDEF1_W                                                4U
#define SCB_ID_AFR0_IMPDEF1_M                                       0x000000F0U
#define SCB_ID_AFR0_IMPDEF1_S                                                4U

// Field:   [3:0] IMPDEF0
//
// IMPLEMENTATION DEFINED meaning
#define SCB_ID_AFR0_IMPDEF0_W                                                4U
#define SCB_ID_AFR0_IMPDEF0_M                                       0x0000000FU
#define SCB_ID_AFR0_IMPDEF0_S                                                0U

//*****************************************************************************
//
// Register: SCB_O_ID_MMFR0
//
//*****************************************************************************
// Field: [31:24] RES0
//
// Reserved, RES0
#define SCB_ID_MMFR0_RES0_W                                                  8U
#define SCB_ID_MMFR0_RES0_M                                         0xFF000000U
#define SCB_ID_MMFR0_RES0_S                                                 24U

// Field: [23:20] AuxReg
//
// Indicates support for Auxiliary Control Registers
#define SCB_ID_MMFR0_AUXREG_W                                                4U
#define SCB_ID_MMFR0_AUXREG_M                                       0x00F00000U
#define SCB_ID_MMFR0_AUXREG_S                                               20U

// Field: [19:16] TCM
//
// Indicates support for tightly coupled memories (TCMs)
#define SCB_ID_MMFR0_TCM_W                                                   4U
#define SCB_ID_MMFR0_TCM_M                                          0x000F0000U
#define SCB_ID_MMFR0_TCM_S                                                  16U

// Field: [15:12] ShareLvl
//
// Indicates the number of shareability levels implemented
#define SCB_ID_MMFR0_SHARELVL_W                                              4U
#define SCB_ID_MMFR0_SHARELVL_M                                     0x0000F000U
#define SCB_ID_MMFR0_SHARELVL_S                                             12U

// Field:  [11:8] OuterShr
//
// Indicates the outermost shareability domain implemented
#define SCB_ID_MMFR0_OUTERSHR_W                                              4U
#define SCB_ID_MMFR0_OUTERSHR_M                                     0x00000F00U
#define SCB_ID_MMFR0_OUTERSHR_S                                              8U

// Field:   [7:4] PMSA
//
// Indicates support for the protected memory system architecture (PMSA)
#define SCB_ID_MMFR0_PMSA_W                                                  4U
#define SCB_ID_MMFR0_PMSA_M                                         0x000000F0U
#define SCB_ID_MMFR0_PMSA_S                                                  4U

// Field:   [3:0] RES0_1
//
// Reserved, RES0
#define SCB_ID_MMFR0_RES0_1_W                                                4U
#define SCB_ID_MMFR0_RES0_1_M                                       0x0000000FU
#define SCB_ID_MMFR0_RES0_1_S                                                0U

//*****************************************************************************
//
// Register: SCB_O_ID_MMFR1
//
//*****************************************************************************
// Field:  [31:0] RES0
//
// Reserved, RES0
#define SCB_ID_MMFR1_RES0_W                                                 32U
#define SCB_ID_MMFR1_RES0_M                                         0xFFFFFFFFU
#define SCB_ID_MMFR1_RES0_S                                                  0U

//*****************************************************************************
//
// Register: SCB_O_ID_MMFR2
//
//*****************************************************************************
// Field: [31:28] RES0
//
// Reserved, RES0
#define SCB_ID_MMFR2_RES0_W                                                  4U
#define SCB_ID_MMFR2_RES0_M                                         0xF0000000U
#define SCB_ID_MMFR2_RES0_S                                                 28U

// Field: [27:24] WFIStall
//
// Indicates the support for Wait For Interrupt (WFI) stalling
#define SCB_ID_MMFR2_WFISTALL_W                                              4U
#define SCB_ID_MMFR2_WFISTALL_M                                     0x0F000000U
#define SCB_ID_MMFR2_WFISTALL_S                                             24U

// Field:  [23:0] RES0_1
//
// Reserved, RES0
#define SCB_ID_MMFR2_RES0_1_W                                               24U
#define SCB_ID_MMFR2_RES0_1_M                                       0x00FFFFFFU
#define SCB_ID_MMFR2_RES0_1_S                                                0U

//*****************************************************************************
//
// Register: SCB_O_ID_MMFR3
//
//*****************************************************************************
// Field: [31:12] RES0
//
// Reserved, RES0
#define SCB_ID_MMFR3_RES0_W                                                 20U
#define SCB_ID_MMFR3_RES0_M                                         0xFFFFF000U
#define SCB_ID_MMFR3_RES0_S                                                 12U

// Field:  [11:8] BPMaint
//
// Indicates the supported branch predictor maintenance
#define SCB_ID_MMFR3_BPMAINT_W                                               4U
#define SCB_ID_MMFR3_BPMAINT_M                                      0x00000F00U
#define SCB_ID_MMFR3_BPMAINT_S                                               8U

// Field:   [7:4] CMaintSW
//
// Indicates the supported cache maintenance operations by set/way
#define SCB_ID_MMFR3_CMAINTSW_W                                              4U
#define SCB_ID_MMFR3_CMAINTSW_M                                     0x000000F0U
#define SCB_ID_MMFR3_CMAINTSW_S                                              4U

// Field:   [3:0] CMaintVA
//
// Indicates the supported cache maintenance operations by address
#define SCB_ID_MMFR3_CMAINTVA_W                                              4U
#define SCB_ID_MMFR3_CMAINTVA_M                                     0x0000000FU
#define SCB_ID_MMFR3_CMAINTVA_S                                              0U

//*****************************************************************************
//
// Register: SCB_O_ID_ISAR0
//
//*****************************************************************************
// Field: [31:28] RES0
//
// Reserved, RES0
#define SCB_ID_ISAR0_RES0_W                                                  4U
#define SCB_ID_ISAR0_RES0_M                                         0xF0000000U
#define SCB_ID_ISAR0_RES0_S                                                 28U

// Field: [27:24] Divide
//
// Indicates the supported Divide instructions
#define SCB_ID_ISAR0_DIVIDE_W                                                4U
#define SCB_ID_ISAR0_DIVIDE_M                                       0x0F000000U
#define SCB_ID_ISAR0_DIVIDE_S                                               24U

// Field: [23:20] Debug
//
// Indicates the implemented Debug instructions
#define SCB_ID_ISAR0_DEBUG_W                                                 4U
#define SCB_ID_ISAR0_DEBUG_M                                        0x00F00000U
#define SCB_ID_ISAR0_DEBUG_S                                                20U

// Field: [19:16] Coproc
//
// Indicates the supported Coprocessor instructions
#define SCB_ID_ISAR0_COPROC_W                                                4U
#define SCB_ID_ISAR0_COPROC_M                                       0x000F0000U
#define SCB_ID_ISAR0_COPROC_S                                               16U

// Field: [15:12] CmpBranch
//
// Indicates the supported combined Compare and Branch instructions
#define SCB_ID_ISAR0_CMPBRANCH_W                                             4U
#define SCB_ID_ISAR0_CMPBRANCH_M                                    0x0000F000U
#define SCB_ID_ISAR0_CMPBRANCH_S                                            12U

// Field:  [11:8] BitField
//
// Indicates the supported bit field instructions
#define SCB_ID_ISAR0_BITFIELD_W                                              4U
#define SCB_ID_ISAR0_BITFIELD_M                                     0x00000F00U
#define SCB_ID_ISAR0_BITFIELD_S                                              8U

// Field:   [7:4] BitCount
//
// Indicates the supported bit count instructions
#define SCB_ID_ISAR0_BITCOUNT_W                                              4U
#define SCB_ID_ISAR0_BITCOUNT_M                                     0x000000F0U
#define SCB_ID_ISAR0_BITCOUNT_S                                              4U

// Field:   [3:0] RES0_1
//
// Reserved, RES0
#define SCB_ID_ISAR0_RES0_1_W                                                4U
#define SCB_ID_ISAR0_RES0_1_M                                       0x0000000FU
#define SCB_ID_ISAR0_RES0_1_S                                                0U

//*****************************************************************************
//
// Register: SCB_O_ID_ISAR1
//
//*****************************************************************************
// Field: [31:28] RES0
//
// Reserved, RES0
#define SCB_ID_ISAR1_RES0_W                                                  4U
#define SCB_ID_ISAR1_RES0_M                                         0xF0000000U
#define SCB_ID_ISAR1_RES0_S                                                 28U

// Field: [27:24] Interwork
//
// Indicates the implemented Interworking instructions
#define SCB_ID_ISAR1_INTERWORK_W                                             4U
#define SCB_ID_ISAR1_INTERWORK_M                                    0x0F000000U
#define SCB_ID_ISAR1_INTERWORK_S                                            24U

// Field: [23:20] Immediate
//
// Indicates the implemented for data-processing instructions with long
// immediates
#define SCB_ID_ISAR1_IMMEDIATE_W                                             4U
#define SCB_ID_ISAR1_IMMEDIATE_M                                    0x00F00000U
#define SCB_ID_ISAR1_IMMEDIATE_S                                            20U

// Field: [19:16] IfThen
//
// Indicates the implemented If-Then instructions
#define SCB_ID_ISAR1_IFTHEN_W                                                4U
#define SCB_ID_ISAR1_IFTHEN_M                                       0x000F0000U
#define SCB_ID_ISAR1_IFTHEN_S                                               16U

// Field: [15:12] Extend
//
// Indicates the implemented Extend instructions
#define SCB_ID_ISAR1_EXTEND_W                                                4U
#define SCB_ID_ISAR1_EXTEND_M                                       0x0000F000U
#define SCB_ID_ISAR1_EXTEND_S                                               12U

// Field:  [11:0] RES0_1
//
// Reserved, RES0
#define SCB_ID_ISAR1_RES0_1_W                                               12U
#define SCB_ID_ISAR1_RES0_1_M                                       0x00000FFFU
#define SCB_ID_ISAR1_RES0_1_S                                                0U

//*****************************************************************************
//
// Register: SCB_O_ID_ISAR2
//
//*****************************************************************************
// Field: [31:28] Reversal
//
// Indicates the implemented Reversal instructions
#define SCB_ID_ISAR2_REVERSAL_W                                              4U
#define SCB_ID_ISAR2_REVERSAL_M                                     0xF0000000U
#define SCB_ID_ISAR2_REVERSAL_S                                             28U

// Field: [27:24] RES0
//
// Reserved, RES0
#define SCB_ID_ISAR2_RES0_W                                                  4U
#define SCB_ID_ISAR2_RES0_M                                         0x0F000000U
#define SCB_ID_ISAR2_RES0_S                                                 24U

// Field: [23:20] MultU
//
// Indicates the implemented advanced unsigned Multiply instructions
#define SCB_ID_ISAR2_MULTU_W                                                 4U
#define SCB_ID_ISAR2_MULTU_M                                        0x00F00000U
#define SCB_ID_ISAR2_MULTU_S                                                20U

// Field: [19:16] MultS
//
// Indicates the implemented advanced signed Multiply instructions
#define SCB_ID_ISAR2_MULTS_W                                                 4U
#define SCB_ID_ISAR2_MULTS_M                                        0x000F0000U
#define SCB_ID_ISAR2_MULTS_S                                                16U

// Field: [15:12] Mult
//
// Indicates the implemented additional Multiply instructions
#define SCB_ID_ISAR2_MULT_W                                                  4U
#define SCB_ID_ISAR2_MULT_M                                         0x0000F000U
#define SCB_ID_ISAR2_MULT_S                                                 12U

// Field:  [11:8] MultiAccessInt
//
// Indicates the support for interruptible multi-access instructions
#define SCB_ID_ISAR2_MULTIACCESSINT_W                                        4U
#define SCB_ID_ISAR2_MULTIACCESSINT_M                               0x00000F00U
#define SCB_ID_ISAR2_MULTIACCESSINT_S                                        8U

// Field:   [7:4] MemHint
//
// Indicates the implemented Memory Hint instructions
#define SCB_ID_ISAR2_MEMHINT_W                                               4U
#define SCB_ID_ISAR2_MEMHINT_M                                      0x000000F0U
#define SCB_ID_ISAR2_MEMHINT_S                                               4U

// Field:   [3:0] LoadStore
//
// Indicates the implemented additional load/store instructions
#define SCB_ID_ISAR2_LOADSTORE_W                                             4U
#define SCB_ID_ISAR2_LOADSTORE_M                                    0x0000000FU
#define SCB_ID_ISAR2_LOADSTORE_S                                             0U

//*****************************************************************************
//
// Register: SCB_O_ID_ISAR3
//
//*****************************************************************************
// Field: [31:28] RES0
//
// Reserved, RES0
#define SCB_ID_ISAR3_RES0_W                                                  4U
#define SCB_ID_ISAR3_RES0_M                                         0xF0000000U
#define SCB_ID_ISAR3_RES0_S                                                 28U

// Field: [27:24] TrueNOP
//
// Indicates the implemented true NOP instructions
#define SCB_ID_ISAR3_TRUENOP_W                                               4U
#define SCB_ID_ISAR3_TRUENOP_M                                      0x0F000000U
#define SCB_ID_ISAR3_TRUENOP_S                                              24U

// Field: [23:20] T32Copy
//
// Indicates the support for T32 non flag-setting MOV instructions
#define SCB_ID_ISAR3_T32COPY_W                                               4U
#define SCB_ID_ISAR3_T32COPY_M                                      0x00F00000U
#define SCB_ID_ISAR3_T32COPY_S                                              20U

// Field: [19:16] TabBranch
//
// Indicates the implemented Table Branch instructions
#define SCB_ID_ISAR3_TABBRANCH_W                                             4U
#define SCB_ID_ISAR3_TABBRANCH_M                                    0x000F0000U
#define SCB_ID_ISAR3_TABBRANCH_S                                            16U

// Field: [15:12] SynchPrim
//
// Used in conjunction with ID_ISAR4.SynchPrim_frac to indicate the implemented
// Synchronization Primitive instructions
#define SCB_ID_ISAR3_SYNCHPRIM_W                                             4U
#define SCB_ID_ISAR3_SYNCHPRIM_M                                    0x0000F000U
#define SCB_ID_ISAR3_SYNCHPRIM_S                                            12U

// Field:  [11:8] SVC
//
// Indicates the implemented SVC instructions
#define SCB_ID_ISAR3_SVC_W                                                   4U
#define SCB_ID_ISAR3_SVC_M                                          0x00000F00U
#define SCB_ID_ISAR3_SVC_S                                                   8U

// Field:   [7:4] SIMD
//
// Indicates the implemented SIMD instructions
#define SCB_ID_ISAR3_SIMD_W                                                  4U
#define SCB_ID_ISAR3_SIMD_M                                         0x000000F0U
#define SCB_ID_ISAR3_SIMD_S                                                  4U

// Field:   [3:0] Saturate
//
// Indicates the implemented saturating instructions
#define SCB_ID_ISAR3_SATURATE_W                                              4U
#define SCB_ID_ISAR3_SATURATE_M                                     0x0000000FU
#define SCB_ID_ISAR3_SATURATE_S                                              0U

//*****************************************************************************
//
// Register: SCB_O_ID_ISAR4
//
//*****************************************************************************
// Field: [31:28] RES0
//
// Reserved, RES0
#define SCB_ID_ISAR4_RES0_W                                                  4U
#define SCB_ID_ISAR4_RES0_M                                         0xF0000000U
#define SCB_ID_ISAR4_RES0_S                                                 28U

// Field: [27:24] PSR_M
//
// Indicates the implemented M profile instructions to modify the PSRs
#define SCB_ID_ISAR4_PSR_M_W                                                 4U
#define SCB_ID_ISAR4_PSR_M_M                                        0x0F000000U
#define SCB_ID_ISAR4_PSR_M_S                                                24U

// Field: [23:20] SyncPrim_frac
//
// Used in conjunction with ID_ISAR3.SynchPrim to indicate the implemented
// Synchronization Primitive instructions
#define SCB_ID_ISAR4_SYNCPRIM_FRAC_W                                         4U
#define SCB_ID_ISAR4_SYNCPRIM_FRAC_M                                0x00F00000U
#define SCB_ID_ISAR4_SYNCPRIM_FRAC_S                                        20U

// Field: [19:16] Barrier
//
// Indicates the implemented Barrier instructions
#define SCB_ID_ISAR4_BARRIER_W                                               4U
#define SCB_ID_ISAR4_BARRIER_M                                      0x000F0000U
#define SCB_ID_ISAR4_BARRIER_S                                              16U

// Field: [15:12] RES0_1
//
// Reserved, RES0
#define SCB_ID_ISAR4_RES0_1_W                                                4U
#define SCB_ID_ISAR4_RES0_1_M                                       0x0000F000U
#define SCB_ID_ISAR4_RES0_1_S                                               12U

// Field:  [11:8] Writeback
//
// Indicates the support for writeback addressing modes
#define SCB_ID_ISAR4_WRITEBACK_W                                             4U
#define SCB_ID_ISAR4_WRITEBACK_M                                    0x00000F00U
#define SCB_ID_ISAR4_WRITEBACK_S                                             8U

// Field:   [7:4] WithShifts
//
// Indicates the support for writeback addressing modes
#define SCB_ID_ISAR4_WITHSHIFTS_W                                            4U
#define SCB_ID_ISAR4_WITHSHIFTS_M                                   0x000000F0U
#define SCB_ID_ISAR4_WITHSHIFTS_S                                            4U

// Field:   [3:0] Unpriv
//
// Indicates the implemented unprivileged instructions
#define SCB_ID_ISAR4_UNPRIV_W                                                4U
#define SCB_ID_ISAR4_UNPRIV_M                                       0x0000000FU
#define SCB_ID_ISAR4_UNPRIV_S                                                0U

//*****************************************************************************
//
// Register: SCB_O_ID_ISAR5
//
//*****************************************************************************
// Field:  [31:0] RES0
//
// Reserved, RES0
#define SCB_ID_ISAR5_RES0_W                                                 32U
#define SCB_ID_ISAR5_RES0_M                                         0xFFFFFFFFU
#define SCB_ID_ISAR5_RES0_S                                                  0U

//*****************************************************************************
//
// Register: SCB_O_CLIDR
//
//*****************************************************************************
// Field: [31:30] ICB
//
// This field indicates the boundary between inner and outer domain
// ENUMs:
// L3                       L3 cache is the highest inner level
// L2                       L2 cache is the highest inner level
// L1                       L1 cache is the highest inner level
// NA                       Not disclosed in this mechanism
#define SCB_CLIDR_ICB_W                                                      2U
#define SCB_CLIDR_ICB_M                                             0xC0000000U
#define SCB_CLIDR_ICB_S                                                     30U
#define SCB_CLIDR_ICB_L3                                            0xC0000000U
#define SCB_CLIDR_ICB_L2                                            0x80000000U
#define SCB_CLIDR_ICB_L1                                            0x40000000U
#define SCB_CLIDR_ICB_NA                                            0x00000000U

// Field: [29:27] LoUU
//
// This field indicates the Level of Unification Uniprocessor for the cache
// hierarchy
#define SCB_CLIDR_LOUU_W                                                     3U
#define SCB_CLIDR_LOUU_M                                            0x38000000U
#define SCB_CLIDR_LOUU_S                                                    27U

// Field: [26:24] LoC
//
// This field indicates the Level of Coherence for the cache hierarchy
#define SCB_CLIDR_LOC_W                                                      3U
#define SCB_CLIDR_LOC_M                                             0x07000000U
#define SCB_CLIDR_LOC_S                                                     24U

// Field: [23:21] LoUIS
//
// Enables Non-secure access to coprocessor CP0
#define SCB_CLIDR_LOUIS_W                                                    3U
#define SCB_CLIDR_LOUIS_M                                           0x00E00000U
#define SCB_CLIDR_LOUIS_S                                                   21U

// Field: [20:18] Ctype7
//
// Cache type field 7. Indicates the type of cache implemented at level 7.
// ENUMs:
// BOTH_UNIFIED             Unified cache
// BOTH_SEP                 Separate instruction and data caches
// DATA                     Data cache only
// INSTR                    Instruction cache only
// NO_CACHE                 No cache
#define SCB_CLIDR_CTYPE7_W                                                   3U
#define SCB_CLIDR_CTYPE7_M                                          0x001C0000U
#define SCB_CLIDR_CTYPE7_S                                                  18U
#define SCB_CLIDR_CTYPE7_BOTH_UNIFIED                               0x00100000U
#define SCB_CLIDR_CTYPE7_BOTH_SEP                                   0x000C0000U
#define SCB_CLIDR_CTYPE7_DATA                                       0x00080000U
#define SCB_CLIDR_CTYPE7_INSTR                                      0x00040000U
#define SCB_CLIDR_CTYPE7_NO_CACHE                                   0x00000000U

// Field: [17:15] Ctype6
//
// Cache type field 6. Indicates the type of cache implemented at level 6.
// ENUMs:
// BOTH_UNIFIED             Unified cache
// BOTH_SEP                 Separate instruction and data caches
// DATA                     Data cache only
// INSTR                    Instruction cache only
// NO_CACHE                 No cache
#define SCB_CLIDR_CTYPE6_W                                                   3U
#define SCB_CLIDR_CTYPE6_M                                          0x00038000U
#define SCB_CLIDR_CTYPE6_S                                                  15U
#define SCB_CLIDR_CTYPE6_BOTH_UNIFIED                               0x00020000U
#define SCB_CLIDR_CTYPE6_BOTH_SEP                                   0x00018000U
#define SCB_CLIDR_CTYPE6_DATA                                       0x00010000U
#define SCB_CLIDR_CTYPE6_INSTR                                      0x00008000U
#define SCB_CLIDR_CTYPE6_NO_CACHE                                   0x00000000U

// Field: [14:12] Ctype5
//
// Cache type field 5. Indicates the type of cache implemented at level 5.
// ENUMs:
// BOTH_UNIFIED             Unified cache
// BOTH_SEP                 Separate instruction and data caches
// DATA                     Data cache only
// INSTR                    Instruction cache only
// NO_CACHE                 No cache
#define SCB_CLIDR_CTYPE5_W                                                   3U
#define SCB_CLIDR_CTYPE5_M                                          0x00007000U
#define SCB_CLIDR_CTYPE5_S                                                  12U
#define SCB_CLIDR_CTYPE5_BOTH_UNIFIED                               0x00004000U
#define SCB_CLIDR_CTYPE5_BOTH_SEP                                   0x00003000U
#define SCB_CLIDR_CTYPE5_DATA                                       0x00002000U
#define SCB_CLIDR_CTYPE5_INSTR                                      0x00001000U
#define SCB_CLIDR_CTYPE5_NO_CACHE                                   0x00000000U

// Field:  [11:9] Ctype4
//
// Cache type field 4. Indicates the type of cache implemented at level 4.
// ENUMs:
// BOTH_UNIFIED             Unified cache
// BOTH_SEP                 Separate instruction and data caches
// DATA                     Data cache only
// INSTR                    Instruction cache only
// NO_CACHE                 No cache
#define SCB_CLIDR_CTYPE4_W                                                   3U
#define SCB_CLIDR_CTYPE4_M                                          0x00000E00U
#define SCB_CLIDR_CTYPE4_S                                                   9U
#define SCB_CLIDR_CTYPE4_BOTH_UNIFIED                               0x00000800U
#define SCB_CLIDR_CTYPE4_BOTH_SEP                                   0x00000600U
#define SCB_CLIDR_CTYPE4_DATA                                       0x00000400U
#define SCB_CLIDR_CTYPE4_INSTR                                      0x00000200U
#define SCB_CLIDR_CTYPE4_NO_CACHE                                   0x00000000U

// Field:   [8:6] Ctype3
//
// Cache type field 3. Indicates the type of cache implemented at level 3.
// ENUMs:
// BOTH_UNIFIED             Unified cache
// BOTH_SEP                 Separate instruction and data caches
// DATA                     Data cache only
// INSTR                    Instruction cache only
// NO_CACHE                 No cache
#define SCB_CLIDR_CTYPE3_W                                                   3U
#define SCB_CLIDR_CTYPE3_M                                          0x000001C0U
#define SCB_CLIDR_CTYPE3_S                                                   6U
#define SCB_CLIDR_CTYPE3_BOTH_UNIFIED                               0x00000100U
#define SCB_CLIDR_CTYPE3_BOTH_SEP                                   0x000000C0U
#define SCB_CLIDR_CTYPE3_DATA                                       0x00000080U
#define SCB_CLIDR_CTYPE3_INSTR                                      0x00000040U
#define SCB_CLIDR_CTYPE3_NO_CACHE                                   0x00000000U

// Field:   [5:3] Ctype2
//
// Cache type field 2. Indicates the type of cache implemented at level 2.
// ENUMs:
// BOTH_UNIFIED             Unified cache
// BOTH_SEP                 Separate instruction and data caches
// DATA                     Data cache only
// INSTR                    Instruction cache only
// NO_CACHE                 No cache
#define SCB_CLIDR_CTYPE2_W                                                   3U
#define SCB_CLIDR_CTYPE2_M                                          0x00000038U
#define SCB_CLIDR_CTYPE2_S                                                   3U
#define SCB_CLIDR_CTYPE2_BOTH_UNIFIED                               0x00000020U
#define SCB_CLIDR_CTYPE2_BOTH_SEP                                   0x00000018U
#define SCB_CLIDR_CTYPE2_DATA                                       0x00000010U
#define SCB_CLIDR_CTYPE2_INSTR                                      0x00000008U
#define SCB_CLIDR_CTYPE2_NO_CACHE                                   0x00000000U

// Field:   [2:0] Ctype1
//
// Cache type field 1. Indicates the type of cache implemented at level 1.
// ENUMs:
// BOTH_UNIFIED             Unified cache
// BOTH_SEP                 Separate instruction and data caches
// DATA                     Data cache only
// INSTR                    Instruction cache only
// NO_CACHE                 No cache
#define SCB_CLIDR_CTYPE1_W                                                   3U
#define SCB_CLIDR_CTYPE1_M                                          0x00000007U
#define SCB_CLIDR_CTYPE1_S                                                   0U
#define SCB_CLIDR_CTYPE1_BOTH_UNIFIED                               0x00000004U
#define SCB_CLIDR_CTYPE1_BOTH_SEP                                   0x00000003U
#define SCB_CLIDR_CTYPE1_DATA                                       0x00000002U
#define SCB_CLIDR_CTYPE1_INSTR                                      0x00000001U
#define SCB_CLIDR_CTYPE1_NO_CACHE                                   0x00000000U

//*****************************************************************************
//
// Register: SCB_O_CTR
//
//*****************************************************************************
// Field:    [31] RES1
//
// Reserved, RES1
#define SCB_CTR_RES1                                                0x80000000U
#define SCB_CTR_RES1_M                                              0x80000000U
#define SCB_CTR_RES1_S                                                      31U

// Field: [30:28] RES0
//
// Reserved, RES0
#define SCB_CTR_RES0_W                                                       3U
#define SCB_CTR_RES0_M                                              0x70000000U
#define SCB_CTR_RES0_S                                                      28U

// Field: [27:24] CWG
//
// Log2 of the number of words of the maximum size of memory that can be
// overwritten as a result of the eviction of a cache entry that has had a
// memory location in it modified
#define SCB_CTR_CWG_W                                                        4U
#define SCB_CTR_CWG_M                                               0x0F000000U
#define SCB_CTR_CWG_S                                                       24U

// Field: [23:20] ERG
//
// Log2 of the number of words of the maximum size of the reservation granule
// that has been implemented for the Load-Exclusive and Store-Exclusive
// instructions
#define SCB_CTR_ERG_W                                                        4U
#define SCB_CTR_ERG_M                                               0x00F00000U
#define SCB_CTR_ERG_S                                                       20U

// Field: [19:16] DminLine
//
// Log2 of the number of words in the smallest cache line of all the data
// caches and unified caches that are controlled by the PE
#define SCB_CTR_DMINLINE_W                                                   4U
#define SCB_CTR_DMINLINE_M                                          0x000F0000U
#define SCB_CTR_DMINLINE_S                                                  16U

// Field: [15:14] RES1_1
//
// Reserved, RES1
#define SCB_CTR_RES1_1_W                                                     2U
#define SCB_CTR_RES1_1_M                                            0x0000C000U
#define SCB_CTR_RES1_1_S                                                    14U

// Field:  [13:4] RES0_1
//
// Reserved, RES0
#define SCB_CTR_RES0_1_W                                                    10U
#define SCB_CTR_RES0_1_M                                            0x00003FF0U
#define SCB_CTR_RES0_1_S                                                     4U

// Field:   [3:0] IminLine
//
// Log2 of the number of words in the smallest cache line of all the
// instruction caches that are controlled by the PE
#define SCB_CTR_IMINLINE_W                                                   4U
#define SCB_CTR_IMINLINE_M                                          0x0000000FU
#define SCB_CTR_IMINLINE_S                                                   0U

//*****************************************************************************
//
// Register: SCB_O_CCSIDR
//
//*****************************************************************************
// Field:    [31] WT
//
// Indicates whether the currently selected cache level supports Write-Through
// ENUMs:
// Supported                Supported
// NOT_SUPPORTED            Not supported
#define SCB_CCSIDR_WT                                               0x80000000U
#define SCB_CCSIDR_WT_M                                             0x80000000U
#define SCB_CCSIDR_WT_S                                                     31U
#define SCB_CCSIDR_WT_SUPPORTED                                     0x80000000U
#define SCB_CCSIDR_WT_NOT_SUPPORTED                                 0x00000000U

// Field:    [30] WB
//
// Indicates whether the currently selected cache level supports Write-Back
// ENUMs:
// Supported                Supported
// NOT_SUPPORTED            Not supported
#define SCB_CCSIDR_WB                                               0x40000000U
#define SCB_CCSIDR_WB_M                                             0x40000000U
#define SCB_CCSIDR_WB_S                                                     30U
#define SCB_CCSIDR_WB_SUPPORTED                                     0x40000000U
#define SCB_CCSIDR_WB_NOT_SUPPORTED                                 0x00000000U

// Field:    [29] RA
//
// Indicates whether the currently selected cache level supports
// read-allocation
// ENUMs:
// Supported                Supported
// NOT_SUPPORTED            Not supported
#define SCB_CCSIDR_RA                                               0x20000000U
#define SCB_CCSIDR_RA_M                                             0x20000000U
#define SCB_CCSIDR_RA_S                                                     29U
#define SCB_CCSIDR_RA_SUPPORTED                                     0x20000000U
#define SCB_CCSIDR_RA_NOT_SUPPORTED                                 0x00000000U

// Field:    [28] WA
//
// Indicates whether the currently selected cache level supports
// write-allocation
// ENUMs:
// Supported                Supported
// NOT_SUPPORTED            Not supported
#define SCB_CCSIDR_WA                                               0x10000000U
#define SCB_CCSIDR_WA_M                                             0x10000000U
#define SCB_CCSIDR_WA_S                                                     28U
#define SCB_CCSIDR_WA_SUPPORTED                                     0x10000000U
#define SCB_CCSIDR_WA_NOT_SUPPORTED                                 0x00000000U

// Field: [27:13] NumSets
//
// Indicates (Number of sets in the currently selected cache) - 1. Therefore, a
// value of 0
// indicates that 1 is set in the cache. The number of sets does not have to be
// a power of 2
#define SCB_CCSIDR_NUMSETS_W                                                15U
#define SCB_CCSIDR_NUMSETS_M                                        0x0FFFE000U
#define SCB_CCSIDR_NUMSETS_S                                                13U

// Field:  [12:3] Associativity
//
// Indicates (Associativity of cache) - 1. A value of 0 indicates an
// associativity of 1. The
// associativity does not have to be a power of 2
#define SCB_CCSIDR_ASSOCIATIVITY_W                                          10U
#define SCB_CCSIDR_ASSOCIATIVITY_M                                  0x00001FF8U
#define SCB_CCSIDR_ASSOCIATIVITY_S                                           3U

// Field:   [2:0] LineSize
//
// Indicates (Log2(Number of words per line in the currently selected cache)) -
// 2.
#define SCB_CCSIDR_LINESIZE_W                                                3U
#define SCB_CCSIDR_LINESIZE_M                                       0x00000007U
#define SCB_CCSIDR_LINESIZE_S                                                0U

//*****************************************************************************
//
// Register: SCB_O_CSSELR
//
//*****************************************************************************
// Field:  [31:4] Res0
//
// Reserved,Res0
#define SCB_CSSELR_RES0_W                                                   28U
#define SCB_CSSELR_RES0_M                                           0xFFFFFFF0U
#define SCB_CSSELR_RES0_S                                                    4U

// Field:   [3:1] Level
//
// Selects which cache level is to be identified. Permitted values are from
// 0b000, indicating Level
// 1 cache, to 0b110 indicating Level 7 cache
// ENUMs:
// L7                       Level 7 cache
// L6                       Level 6 cache
// L5                       Level 5 cache
// L4                       Level 4 cache
// L3                       Level 3 cache
// L2                       Level 2 cache
// L1                       Level 1 cache
#define SCB_CSSELR_LEVEL_W                                                   3U
#define SCB_CSSELR_LEVEL_M                                          0x0000000EU
#define SCB_CSSELR_LEVEL_S                                                   1U
#define SCB_CSSELR_LEVEL_L7                                         0x0000000CU
#define SCB_CSSELR_LEVEL_L6                                         0x0000000AU
#define SCB_CSSELR_LEVEL_L5                                         0x00000008U
#define SCB_CSSELR_LEVEL_L4                                         0x00000006U
#define SCB_CSSELR_LEVEL_L3                                         0x00000004U
#define SCB_CSSELR_LEVEL_L2                                         0x00000002U
#define SCB_CSSELR_LEVEL_L1                                         0x00000000U

// Field:     [0] InD
//
// Selects whether the instruction or the data cache is to be identified
// ENUMs:
// INSTR                    Instruction cache
// DATA                     Data or unified cache
#define SCB_CSSELR_IND                                              0x00000001U
#define SCB_CSSELR_IND_M                                            0x00000001U
#define SCB_CSSELR_IND_S                                                     0U
#define SCB_CSSELR_IND_INSTR                                        0x00000001U
#define SCB_CSSELR_IND_DATA                                         0x00000000U

//*****************************************************************************
//
// Register: SCB_O_CPACR
//
//*****************************************************************************
// Field: [31:24] RES0
//
// Reserved, RES0
#define SCB_CPACR_RES0_W                                                     8U
#define SCB_CPACR_RES0_M                                            0xFF000000U
#define SCB_CPACR_RES0_S                                                    24U

// Field: [23:22] CP11
//
// The value in this field is ignored. If the implementation does not include
// the FP Extension, this field is RAZ/WI. If the value of this bit is not
// programmed to the same value as the CP10 field, then the value is UNKNOWN
#define SCB_CPACR_CP11_W                                                     2U
#define SCB_CPACR_CP11_M                                            0x00C00000U
#define SCB_CPACR_CP11_S                                                    22U

// Field: [21:20] CP10
//
// Defines the access rights for the floating-point functionality
#define SCB_CPACR_CP10_W                                                     2U
#define SCB_CPACR_CP10_M                                            0x00300000U
#define SCB_CPACR_CP10_S                                                    20U

// Field: [19:16] RES0_1
//
// Reserved, RES0
#define SCB_CPACR_RES0_1_W                                                   4U
#define SCB_CPACR_RES0_1_M                                          0x000F0000U
#define SCB_CPACR_RES0_1_S                                                  16U

// Field: [15:14] CP7
//
//  Controls access privileges for coprocessor 7
#define SCB_CPACR_CP7_W                                                      2U
#define SCB_CPACR_CP7_M                                             0x0000C000U
#define SCB_CPACR_CP7_S                                                     14U

// Field: [13:12] CP6
//
//  Controls access privileges for coprocessor 6
#define SCB_CPACR_CP6_W                                                      2U
#define SCB_CPACR_CP6_M                                             0x00003000U
#define SCB_CPACR_CP6_S                                                     12U

// Field: [11:10] CP5
//
//  Controls access privileges for coprocessor 5
#define SCB_CPACR_CP5_W                                                      2U
#define SCB_CPACR_CP5_M                                             0x00000C00U
#define SCB_CPACR_CP5_S                                                     10U

// Field:   [9:8] CP4
//
//  Controls access privileges for coprocessor 4
#define SCB_CPACR_CP4_W                                                      2U
#define SCB_CPACR_CP4_M                                             0x00000300U
#define SCB_CPACR_CP4_S                                                      8U

// Field:   [7:6] CP3
//
//  Controls access privileges for coprocessor 3
#define SCB_CPACR_CP3_W                                                      2U
#define SCB_CPACR_CP3_M                                             0x000000C0U
#define SCB_CPACR_CP3_S                                                      6U

// Field:   [5:4] CP2
//
//  Controls access privileges for coprocessor 2
#define SCB_CPACR_CP2_W                                                      2U
#define SCB_CPACR_CP2_M                                             0x00000030U
#define SCB_CPACR_CP2_S                                                      4U

// Field:   [3:2] CP1
//
//  Controls access privileges for coprocessor 1
#define SCB_CPACR_CP1_W                                                      2U
#define SCB_CPACR_CP1_M                                             0x0000000CU
#define SCB_CPACR_CP1_S                                                      2U

// Field:   [1:0] CP0
//
//  Controls access privileges for coprocessor 0
#define SCB_CPACR_CP0_W                                                      2U
#define SCB_CPACR_CP0_M                                             0x00000003U
#define SCB_CPACR_CP0_S                                                      0U

//*****************************************************************************
//
// Register: SCB_O_NSACR
//
//*****************************************************************************
// Field: [31:12] RES0
//
// Reserved, RES0
#define SCB_NSACR_RES0_W                                                    20U
#define SCB_NSACR_RES0_M                                            0xFFFFF000U
#define SCB_NSACR_RES0_S                                                    12U

// Field:    [11] CP11
//
// Enables Non-secure access to the Floating-point Extension
#define SCB_NSACR_CP11                                              0x00000800U
#define SCB_NSACR_CP11_M                                            0x00000800U
#define SCB_NSACR_CP11_S                                                    11U

// Field:    [10] CP10
//
// Enables Non-secure access to the Floating-point Extension
#define SCB_NSACR_CP10                                              0x00000400U
#define SCB_NSACR_CP10_M                                            0x00000400U
#define SCB_NSACR_CP10_S                                                    10U

// Field:   [9:8] RES0_1
//
// Reserved, RES0
#define SCB_NSACR_RES0_1_W                                                   2U
#define SCB_NSACR_RES0_1_M                                          0x00000300U
#define SCB_NSACR_RES0_1_S                                                   8U

// Field:     [7] CP7
//
// Enables Non-secure access to coprocessor CP7
#define SCB_NSACR_CP7                                               0x00000080U
#define SCB_NSACR_CP7_M                                             0x00000080U
#define SCB_NSACR_CP7_S                                                      7U

// Field:     [6] CP6
//
// Enables Non-secure access to coprocessor CP6
#define SCB_NSACR_CP6                                               0x00000040U
#define SCB_NSACR_CP6_M                                             0x00000040U
#define SCB_NSACR_CP6_S                                                      6U

// Field:     [5] CP5
//
// Enables Non-secure access to coprocessor CP5
#define SCB_NSACR_CP5                                               0x00000020U
#define SCB_NSACR_CP5_M                                             0x00000020U
#define SCB_NSACR_CP5_S                                                      5U

// Field:     [4] CP4
//
// Enables Non-secure access to coprocessor CP4
#define SCB_NSACR_CP4                                               0x00000010U
#define SCB_NSACR_CP4_M                                             0x00000010U
#define SCB_NSACR_CP4_S                                                      4U

// Field:     [3] CP3
//
// Enables Non-secure access to coprocessor CP3
#define SCB_NSACR_CP3                                               0x00000008U
#define SCB_NSACR_CP3_M                                             0x00000008U
#define SCB_NSACR_CP3_S                                                      3U

// Field:     [2] CP2
//
// Enables Non-secure access to coprocessor CP2
#define SCB_NSACR_CP2                                               0x00000004U
#define SCB_NSACR_CP2_M                                             0x00000004U
#define SCB_NSACR_CP2_S                                                      2U

// Field:     [1] CP1
//
// Enables Non-secure access to coprocessor CP1
#define SCB_NSACR_CP1                                               0x00000002U
#define SCB_NSACR_CP1_M                                             0x00000002U
#define SCB_NSACR_CP1_S                                                      1U

// Field:     [0] CP0
//
// Enables Non-secure access to coprocessor CP0
#define SCB_NSACR_CP0                                               0x00000001U
#define SCB_NSACR_CP0_M                                             0x00000001U
#define SCB_NSACR_CP0_S                                                      0U


#endif // __SCB__
