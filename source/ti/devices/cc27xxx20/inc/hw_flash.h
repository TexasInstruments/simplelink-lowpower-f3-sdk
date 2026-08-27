/******************************************************************************
*  Filename:       hw_flash.h
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

#ifndef __HW_FLASH_H__
#define __HW_FLASH_H__

//*****************************************************************************
//
// This section defines the register offsets of
// FLASH component
//
//*****************************************************************************
// Interrupt Index Register
#define FLASH_O_IIDX                                                0x00000020U

// Interrupt Mask Register
#define FLASH_O_IMASK                                               0x00000028U

// Raw Interrupt Status Register
#define FLASH_O_RIS                                                 0x00000030U

// Masked Interrupt Status Register
#define FLASH_O_MIS                                                 0x00000038U

// Interrupt Set Register
#define FLASH_O_ISET                                                0x00000040U

// Interrupt Clear Register
#define FLASH_O_ICLR                                                0x00000048U

// Event Mode
#define FLASH_O_EVT_MODE                                            0x000000E0U

// Hardware Version Description Register
#define FLASH_O_DESC                                                0x000000FCU

// Command Execute Register
#define FLASH_O_CMDEXEC                                             0x00000100U

// Command Type Register
#define FLASH_O_CMDTYPE                                             0x00000104U

// Command Control Register
#define FLASH_O_CMDCTL                                              0x00000108U

// Command Address Register
#define FLASH_O_CMDADDR                                             0x00000120U

// Command Program Byte Enable Register
#define FLASH_O_CMDBYTEN                                            0x00000124U

// Command Data Register 0
#define FLASH_O_CMDDATA0                                            0x00000130U

// Command Data Register 1
#define FLASH_O_CMDDATA1                                            0x00000134U

// Command Data Register 2
#define FLASH_O_CMDDATA2                                            0x00000138U

// Command Data Register Bits 127:96
#define FLASH_O_CMDDATA3                                            0x0000013CU

// Command Write Erase Protect A Register
#define FLASH_O_CMDWEPROTA                                          0x000001D0U

// Command Write Erase Protect B Register
#define FLASH_O_CMDWEPROTB                                          0x000001D4U

// Command Write Erase Protect Non-Main Register
#define FLASH_O_CMDWEPROTNM                                         0x00000210U

// Command Write Erase Protect Trim Register
#define FLASH_O_CMDWEPROTTR                                         0x00000214U

// Command Write Erase Protect Engr Register
#define FLASH_O_CMDWEPROTEN                                         0x00000218U

// Command Configuration Register
#define FLASH_O_CFGCMD                                              0x000003B0U

// Pulse Counter Configuration Register
#define FLASH_O_CFGPCNT                                             0x000003B4U

// Command Status Register
#define FLASH_O_STATCMD                                             0x000003D0U

// Address Status Register
#define FLASH_O_STATADDR                                            0x000003D4U

// Pulse Count Status Register
#define FLASH_O_STATPCNT                                            0x000003D8U

// Mode Status Register
#define FLASH_O_STATMODE                                            0x000003DCU

// Global Information Register 0
#define FLASH_O_GBLINFO0                                            0x000003F0U

// Global Information Register 1
#define FLASH_O_GBLINFO1                                            0x000003F4U

// Global Information Register 2
#define FLASH_O_GBLINFO2                                            0x000003F8U

// Bank Information Register 0 for Bank 0
#define FLASH_O_BANK0INFO0                                          0x00000400U

// Bank Information Register 1 for Bank 0
#define FLASH_O_BANK0INFO1                                          0x00000404U

// Bank Information Register 0 for Bank 1
#define FLASH_O_BANK1INFO0                                          0x00000410U

// Bank Information Register 1 for Bank 1
#define FLASH_O_BANK1INFO1                                          0x00000414U

// DFT Enable Register
#define FLASH_O_DFTEN                                               0x00000500U

// DFT Command Control Register
#define FLASH_O_DFTCMDCTL                                           0x00000504U

// DFT Timer Control Register
#define FLASH_O_DFTTIMERCTL                                         0x00000508U

// Flash Wrapper State EXECUTEZ Control Register
#define FLASH_O_DFTEXECZCTL                                         0x0000050CU

// DFT Pump Clock Test Control Register
#define FLASH_O_DFTPCLKTESTCTL                                      0x00000510U

// DFT Pump Clock Test Status Register
#define FLASH_O_DFTPCLKTESTSTAT                                     0x00000514U

// DFT Data Register Redundant 0
#define FLASH_O_DFTDATARED0                                         0x00000540U

// DFT Pump Control Register
#define FLASH_O_DFTPUMPCTL                                          0x00000560U

// DFT Bank Control Register
#define FLASH_O_DFTBANKCTL                                          0x00000564U

//*****************************************************************************
//
// Register: FLASH_O_IIDX
//
//*****************************************************************************
// Field:     [0] STAT
//
// Indicates which interrupt has fired. 0x0 means no event pending. The
// priority order is fixed. On each read, only one interrupt is indicated. On a
// read, the current interrupt (highest priority) is automatically cleared by
// the hardware and the corresponding interrupt flags in the RIS and MIS are
// cleared as well. After a read from the CPU (not from the debug interface),
// the register must be updated with the next highest priority interrupt.
// ENUMs:
// DONE                     DONE Interrupt Pending
// NO_INTR                  No Interrupt Pending
#define FLASH_IIDX_STAT                                             0x00000001U
#define FLASH_IIDX_STAT_M                                           0x00000001U
#define FLASH_IIDX_STAT_S                                                    0U
#define FLASH_IIDX_STAT_DONE                                        0x00000001U
#define FLASH_IIDX_STAT_NO_INTR                                     0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_IMASK
//
//*****************************************************************************
// Field:     [0] DONE
//
// Interrupt mask for DONE:
// 0: Interrupt is disabled in MIS register
// 1: Interrupt is enabled in MIS register
// ENUMs:
// ENABLED                  Interrupt will request an interrupt service
//                          routine and corresponding bit in IPSTANDARD.MIS
//                          will be set
// DISABLED                 Interrupt is masked out
#define FLASH_IMASK_DONE                                            0x00000001U
#define FLASH_IMASK_DONE_M                                          0x00000001U
#define FLASH_IMASK_DONE_S                                                   0U
#define FLASH_IMASK_DONE_ENABLED                                    0x00000001U
#define FLASH_IMASK_DONE_DISABLED                                   0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_RIS
//
//*****************************************************************************
// Field:     [0] DONE
//
// Flash wrapper operation completed.
// This interrupt bit is set by firmware or the corresponding bit in the ISET
// register.
// It is cleared by the corresponding bit in in the ICLR register or reading
// the IIDX register when this interrupt is the highest priority.
// ENUMs:
// SET                      Interrupt occurred
// CLR                      Interrupt did not occur
#define FLASH_RIS_DONE                                              0x00000001U
#define FLASH_RIS_DONE_M                                            0x00000001U
#define FLASH_RIS_DONE_S                                                     0U
#define FLASH_RIS_DONE_SET                                          0x00000001U
#define FLASH_RIS_DONE_CLR                                          0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_MIS
//
//*****************************************************************************
// Field:     [0] DONE
//
// Flash wrapper operation completed.
// This masked interrupt bit reflects the bitwise AND of the corresponding RIS
// and IMASK bits.
// ENUMs:
// SET                      Masked interrupt occurred
// CLR                      Masked interrupt did not occur
#define FLASH_MIS_DONE                                              0x00000001U
#define FLASH_MIS_DONE_M                                            0x00000001U
#define FLASH_MIS_DONE_S                                                     0U
#define FLASH_MIS_DONE_SET                                          0x00000001U
#define FLASH_MIS_DONE_CLR                                          0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_ISET
//
//*****************************************************************************
// Field:     [0] DONE
//
// 0: No effect
// 1: Set the DONE interrupt in the RIS register
// ENUMs:
// SET                      Set IPSTANDARD.RIS bit
// NO_EFFECT                Writing a 0 has no effect
#define FLASH_ISET_DONE                                             0x00000001U
#define FLASH_ISET_DONE_M                                           0x00000001U
#define FLASH_ISET_DONE_S                                                    0U
#define FLASH_ISET_DONE_SET                                         0x00000001U
#define FLASH_ISET_DONE_NO_EFFECT                                   0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_ICLR
//
//*****************************************************************************
// Field:     [0] DONE
//
// 0: No effect
// 1: Clear the DONE interrupt in the RIS register
// ENUMs:
// CLR                      Clear IPSTANDARD.RIS bit
// NO_EFFECT                Writing a 0 has no effect
#define FLASH_ICLR_DONE                                             0x00000001U
#define FLASH_ICLR_DONE_M                                           0x00000001U
#define FLASH_ICLR_DONE_S                                                    0U
#define FLASH_ICLR_DONE_CLR                                         0x00000001U
#define FLASH_ICLR_DONE_NO_EFFECT                                   0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_EVT_MODE
//
//*****************************************************************************
// Field:   [1:0] INT0_CFG
//
// Event line mode select for peripheral event
// ENUMs:
// HARDWARE                 The interrupt or event line is in hardware mode.
//                          Hardware should clear the RIS.
// SOFTWARE                 The interrupt or event line is in software mode.
//                          Software must clear the RIS.
// DISABLE                  The interrupt or event line is disabled.
#define FLASH_EVT_MODE_INT0_CFG_W                                            2U
#define FLASH_EVT_MODE_INT0_CFG_M                                   0x00000003U
#define FLASH_EVT_MODE_INT0_CFG_S                                            0U
#define FLASH_EVT_MODE_INT0_CFG_HARDWARE                            0x00000002U
#define FLASH_EVT_MODE_INT0_CFG_SOFTWARE                            0x00000001U
#define FLASH_EVT_MODE_INT0_CFG_DISABLE                             0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_DESC
//
//*****************************************************************************
// Field: [31:16] MODULEID
//
// Module ID
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define FLASH_DESC_MODULEID_W                                               16U
#define FLASH_DESC_MODULEID_M                                       0xFFFF0000U
#define FLASH_DESC_MODULEID_S                                               16U
#define FLASH_DESC_MODULEID_MAXIMUM                                 0xFFFF0000U
#define FLASH_DESC_MODULEID_MINIMUM                                 0x00000000U

// Field: [15:12] FEATUREVER
//
// Feature set
// ENUMs:
// MAXIMUM                  Maximum Value
// MINIMUM                  Minimum Value
#define FLASH_DESC_FEATUREVER_W                                              4U
#define FLASH_DESC_FEATUREVER_M                                     0x0000F000U
#define FLASH_DESC_FEATUREVER_S                                             12U
#define FLASH_DESC_FEATUREVER_MAXIMUM                               0x0000F000U
#define FLASH_DESC_FEATUREVER_MINIMUM                               0x00000000U

// Field:  [11:8] INSTNUM
//
// Instance number
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define FLASH_DESC_INSTNUM_W                                                 4U
#define FLASH_DESC_INSTNUM_M                                        0x00000F00U
#define FLASH_DESC_INSTNUM_S                                                 8U
#define FLASH_DESC_INSTNUM_MAXIMUM                                  0x00000F00U
#define FLASH_DESC_INSTNUM_MINIMUM                                  0x00000000U

// Field:   [7:4] MAJREV
//
// Major Revision
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define FLASH_DESC_MAJREV_W                                                  4U
#define FLASH_DESC_MAJREV_M                                         0x000000F0U
#define FLASH_DESC_MAJREV_S                                                  4U
#define FLASH_DESC_MAJREV_MAXIMUM                                   0x000000F0U
#define FLASH_DESC_MAJREV_MINIMUM                                   0x00000000U

// Field:   [3:0] MINREV
//
// Minor Revision
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define FLASH_DESC_MINREV_W                                                  4U
#define FLASH_DESC_MINREV_M                                         0x0000000FU
#define FLASH_DESC_MINREV_S                                                  0U
#define FLASH_DESC_MINREV_MAXIMUM                                   0x0000000FU
#define FLASH_DESC_MINREV_MINIMUM                                   0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CMDEXEC
//
//*****************************************************************************
// Field:     [0] VAL
//
// Command Execute value
// Initiates execution of the command specified in the CMDTYPE register.
// ENUMs:
// EXECUTE                  Command will execute or is executing in flash
//                          wrapper
// NOEXECUTE                Command will not execute or is not executing in
//                          flash wrapper
#define FLASH_CMDEXEC_VAL                                           0x00000001U
#define FLASH_CMDEXEC_VAL_M                                         0x00000001U
#define FLASH_CMDEXEC_VAL_S                                                  0U
#define FLASH_CMDEXEC_VAL_EXECUTE                                   0x00000001U
#define FLASH_CMDEXEC_VAL_NOEXECUTE                                 0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CMDTYPE
//
//*****************************************************************************
// Field:   [6:4] SIZE
//
// Command size
// ENUMs:
// BANK                     Operate on an entire flash bank
// SECTOR                   Operate on a flash sector
// EIGHTWORD                Operate on 8 flash words
// FOURWORD                 Operate on 4 flash words
// TWOWORD                  Operate on 2 flash words
// ONEWORD                  Operate on 1 flash word
#define FLASH_CMDTYPE_SIZE_W                                                 3U
#define FLASH_CMDTYPE_SIZE_M                                        0x00000070U
#define FLASH_CMDTYPE_SIZE_S                                                 4U
#define FLASH_CMDTYPE_SIZE_BANK                                     0x00000050U
#define FLASH_CMDTYPE_SIZE_SECTOR                                   0x00000040U
#define FLASH_CMDTYPE_SIZE_EIGHTWORD                                0x00000030U
#define FLASH_CMDTYPE_SIZE_FOURWORD                                 0x00000020U
#define FLASH_CMDTYPE_SIZE_TWOWORD                                  0x00000010U
#define FLASH_CMDTYPE_SIZE_ONEWORD                                  0x00000000U

// Field:   [2:0] COMMAND
//
// Command type
// ENUMs:
// BLANKVERIFY              Blank Verify - Check whether a flash word is in
//                          the erased state.
//                          This command may only be
//                          used with CMDTYPE.SIZE = ONEWORD
// CLEARSTATUS              Clear Status - Clear status bits in FW_SMSTAT
//                          only.
// MODECHANGE               Mode Change - Perform a mode change only, no other
//                          operation.
// READVERIFY               Read Verify - Perform a standalone read verify
//                          operation.
// ERASE                    Erase
// PROGRAM                  Program
// NOOP                     No Operation
#define FLASH_CMDTYPE_COMMAND_W                                              3U
#define FLASH_CMDTYPE_COMMAND_M                                     0x00000007U
#define FLASH_CMDTYPE_COMMAND_S                                              0U
#define FLASH_CMDTYPE_COMMAND_BLANKVERIFY                           0x00000006U
#define FLASH_CMDTYPE_COMMAND_CLEARSTATUS                           0x00000005U
#define FLASH_CMDTYPE_COMMAND_MODECHANGE                            0x00000004U
#define FLASH_CMDTYPE_COMMAND_READVERIFY                            0x00000003U
#define FLASH_CMDTYPE_COMMAND_ERASE                                 0x00000002U
#define FLASH_CMDTYPE_COMMAND_PROGRAM                               0x00000001U
#define FLASH_CMDTYPE_COMMAND_NOOP                                  0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CMDCTL
//
//*****************************************************************************
// Field:    [21] DATAVEREN
//
// Enable invalid data verify.
// This checks for 0->1 transitions in the memory when
// a program operation is initiated.  If such a transition is found, the
// program will
// fail with an error without doing any programming.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define FLASH_CMDCTL_DATAVEREN                                      0x00200000U
#define FLASH_CMDCTL_DATAVEREN_M                                    0x00200000U
#define FLASH_CMDCTL_DATAVEREN_S                                            21U
#define FLASH_CMDCTL_DATAVEREN_ENABLE                               0x00200000U
#define FLASH_CMDCTL_DATAVEREN_DISABLE                              0x00000000U

// Field:    [20] SSERASEDIS
//
// Disable Stair-Step Erase.  If set, the default VHV trim voltage setting will
// be used
// for all erase pulses.
// By default, this bit is reset, meaning that the VHV voltage will be stepped
// during
// successive erase pulses.  The step count, step voltage, begin and end
// voltages
// are all hard-wired.
// ENUMs:
// DISABLE                  Disable
// ENABLE                   Enable
#define FLASH_CMDCTL_SSERASEDIS                                     0x00100000U
#define FLASH_CMDCTL_SSERASEDIS_M                                   0x00100000U
#define FLASH_CMDCTL_SSERASEDIS_S                                           20U
#define FLASH_CMDCTL_SSERASEDIS_DISABLE                             0x00100000U
#define FLASH_CMDCTL_SSERASEDIS_ENABLE                              0x00000000U

// Field:    [16] ADDRXLATEOVR
//
// Override hardware address translation of address in CMDADDR from a
// system address to a bank address and bank ID.  Use data written to
// CMDADDR directly as the bank address.  Use the value written to
// CMDCTL.BANKSEL directly as the bank ID.  Use the value written to
// CMDCTL.REGIONSEL directly as the region ID.
// ENUMs:
// OVERRIDE                 Override
// NOOVERRIDE               Do not override
#define FLASH_CMDCTL_ADDRXLATEOVR                                   0x00010000U
#define FLASH_CMDCTL_ADDRXLATEOVR_M                                 0x00010000U
#define FLASH_CMDCTL_ADDRXLATEOVR_S                                         16U
#define FLASH_CMDCTL_ADDRXLATEOVR_OVERRIDE                          0x00010000U
#define FLASH_CMDCTL_ADDRXLATEOVR_NOOVERRIDE                        0x00000000U

// Field:  [12:9] REGIONSEL
//
// Bank Region
// A specific region ID can be written to this field to indicate to which
// region an
// operation should be applied if CMDCTL.ADDRXLATEOVR is set.
// ENUMs:
// ENGR                     Engr Region
// TRIM                     Trim Region
// NONMAIN                  Non-Main Region
// MAIN                     Main Region
#define FLASH_CMDCTL_REGIONSEL_W                                             4U
#define FLASH_CMDCTL_REGIONSEL_M                                    0x00001E00U
#define FLASH_CMDCTL_REGIONSEL_S                                             9U
#define FLASH_CMDCTL_REGIONSEL_ENGR                                 0x00001000U
#define FLASH_CMDCTL_REGIONSEL_TRIM                                 0x00000800U
#define FLASH_CMDCTL_REGIONSEL_NONMAIN                              0x00000400U
#define FLASH_CMDCTL_REGIONSEL_MAIN                                 0x00000200U

// Field:   [5:4] BANKSEL
//
// Bank Select
// A specific Bank ID can be written to this field to indicate to which bank an
// operation should be applied if CMDCTL.ADDRXLATEOVR is set.
// ENUMs:
// BANK4                    Bank 4
// BANK3                    Bank 3
// BANK2                    Bank 2
// BANK1                    Bank 1
// BANK0                    Bank 0
#define FLASH_CMDCTL_BANKSEL_W                                               2U
#define FLASH_CMDCTL_BANKSEL_M                                      0x00000030U
#define FLASH_CMDCTL_BANKSEL_S                                               4U
#define FLASH_CMDCTL_BANKSEL_BANK4                                  0x00000100U
#define FLASH_CMDCTL_BANKSEL_BANK3                                  0x00000080U
#define FLASH_CMDCTL_BANKSEL_BANK2                                  0x00000040U
#define FLASH_CMDCTL_BANKSEL_BANK1                                  0x00000020U
#define FLASH_CMDCTL_BANKSEL_BANK0                                  0x00000010U

// Field:   [3:0] MODESEL
//
// Mode
// This field is only used for the Mode Change command type.  Otherwise, bank
// and pump modes are set automaticlly via the NW hardware.
// ENUMs:
// ERASEBNK                 Erase Bank
// PGMMW                    Program Multiple Word
// ERASESECT                Erase Sector
// ERASEVER                 Erase Verify Mode
// PGMSW                    Program Single Word
// PGMVER                   Program Verify Mode
// RDMARG1B                 Read Margin 1B Mode
// RDMARG0B                 Read Margin 0B Mode
// RDMARG1                  Read Margin 1 Mode
// RDMARG0                  Read Margin 0 Mode
// READ                     Read Mode
#define FLASH_CMDCTL_MODESEL_W                                               4U
#define FLASH_CMDCTL_MODESEL_M                                      0x0000000FU
#define FLASH_CMDCTL_MODESEL_S                                               0U
#define FLASH_CMDCTL_MODESEL_ERASEBNK                               0x0000000FU
#define FLASH_CMDCTL_MODESEL_PGMMW                                  0x0000000EU
#define FLASH_CMDCTL_MODESEL_ERASESECT                              0x0000000CU
#define FLASH_CMDCTL_MODESEL_ERASEVER                               0x0000000BU
#define FLASH_CMDCTL_MODESEL_PGMSW                                  0x0000000AU
#define FLASH_CMDCTL_MODESEL_PGMVER                                 0x00000009U
#define FLASH_CMDCTL_MODESEL_RDMARG1B                               0x00000007U
#define FLASH_CMDCTL_MODESEL_RDMARG0B                               0x00000006U
#define FLASH_CMDCTL_MODESEL_RDMARG1                                0x00000004U
#define FLASH_CMDCTL_MODESEL_RDMARG0                                0x00000002U
#define FLASH_CMDCTL_MODESEL_READ                                   0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CMDADDR
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Address value
// ENUMs:
// MAXIMUM                  Maximum value of VAL
// MINIMUM                  Minimum value of VAL
#define FLASH_CMDADDR_VAL_W                                                 32U
#define FLASH_CMDADDR_VAL_M                                         0xFFFFFFFFU
#define FLASH_CMDADDR_VAL_S                                                  0U
#define FLASH_CMDADDR_VAL_MAXIMUM                                   0xFFFFFFFFU
#define FLASH_CMDADDR_VAL_MINIMUM                                   0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CMDBYTEN
//
//*****************************************************************************
// Field:  [15:0] VAL
//
// Command Byte Enable value.
// A 1-bit per flash word byte value is placed in this register.
// ENUMs:
// MAXIMUM                  Maximum value of VAL
// MINIMUM                  Minimum value of VAL
#define FLASH_CMDBYTEN_VAL_W                                                16U
#define FLASH_CMDBYTEN_VAL_M                                        0x0000FFFFU
#define FLASH_CMDBYTEN_VAL_S                                                 0U
#define FLASH_CMDBYTEN_VAL_MAXIMUM                                  0x0003FFFFU
#define FLASH_CMDBYTEN_VAL_MINIMUM                                  0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CMDDATA0
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// A 32-bit data value is placed in this field.
// ENUMs:
// MAXIMUM                  Maximum value of VAL
// MINIMUM                  Minimum value of VAL
#define FLASH_CMDDATA0_VAL_W                                                32U
#define FLASH_CMDDATA0_VAL_M                                        0xFFFFFFFFU
#define FLASH_CMDDATA0_VAL_S                                                 0U
#define FLASH_CMDDATA0_VAL_MAXIMUM                                  0xFFFFFFFFU
#define FLASH_CMDDATA0_VAL_MINIMUM                                  0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CMDDATA1
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// A 32-bit data value is placed in this field.
// ENUMs:
// MAXIMUM                  Maximum value of VAL
// MINIMUM                  Minimum value of VAL
#define FLASH_CMDDATA1_VAL_W                                                32U
#define FLASH_CMDDATA1_VAL_M                                        0xFFFFFFFFU
#define FLASH_CMDDATA1_VAL_S                                                 0U
#define FLASH_CMDDATA1_VAL_MAXIMUM                                  0xFFFFFFFFU
#define FLASH_CMDDATA1_VAL_MINIMUM                                  0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CMDDATA2
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// A 32-bit data value is placed in this field.
// ENUMs:
// MAXIMUM                  Maximum value of VAL
// MINIMUM                  Minimum value of VAL
#define FLASH_CMDDATA2_VAL_W                                                32U
#define FLASH_CMDDATA2_VAL_M                                        0xFFFFFFFFU
#define FLASH_CMDDATA2_VAL_S                                                 0U
#define FLASH_CMDDATA2_VAL_MAXIMUM                                  0xFFFFFFFFU
#define FLASH_CMDDATA2_VAL_MINIMUM                                  0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CMDDATA3
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// A 32-bit data value is placed in this field.
// ENUMs:
// MAXIMUM                  Maximum value of VAL
// MINIMUM                  Minimum value of VAL
#define FLASH_CMDDATA3_VAL_W                                                32U
#define FLASH_CMDDATA3_VAL_M                                        0xFFFFFFFFU
#define FLASH_CMDDATA3_VAL_S                                                 0U
#define FLASH_CMDDATA3_VAL_MAXIMUM                                  0xFFFFFFFFU
#define FLASH_CMDDATA3_VAL_MINIMUM                                  0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CMDWEPROTA
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Each bit protects 1 sector.
//
// bit [0]:	When 1, sector 0 of the flash memory will be protected from program
// 		and erase.
// bit [1]:	When 1, sector 1 of the flash memory will be protected from program
// 		and erase.
// 	:
// 	:
// bit [31]:	When 1, sector 31 of the flash memory will be protected from
// program
// 		and erase.
// ENUMs:
// MAXIMUM                  Maximum value of VAL
// MINIMUM                  Minimum value of VAL
#define FLASH_CMDWEPROTA_VAL_W                                              32U
#define FLASH_CMDWEPROTA_VAL_M                                      0xFFFFFFFFU
#define FLASH_CMDWEPROTA_VAL_S                                               0U
#define FLASH_CMDWEPROTA_VAL_MAXIMUM                                0xFFFFFFFFU
#define FLASH_CMDWEPROTA_VAL_MINIMUM                                0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CMDWEPROTB
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Each bit protects a group of 8 sectors.  When a bit is 1, the associated 8
// sectors
// in the flash will be protected from program and erase.  A maximum of 256
// sectors can be protected with this register.
// ENUMs:
// MAXIMUM                  Maximum value of VAL
// MINIMUM                  Minimum value of VAL
#define FLASH_CMDWEPROTB_VAL_W                                              32U
#define FLASH_CMDWEPROTB_VAL_M                                      0xFFFFFFFFU
#define FLASH_CMDWEPROTB_VAL_S                                               0U
#define FLASH_CMDWEPROTB_VAL_MAXIMUM                                0xFFFFFFFFU
#define FLASH_CMDWEPROTB_VAL_MINIMUM                                0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CMDWEPROTNM
//
//*****************************************************************************
// Field:   [1:0] VAL
//
// Each bit protects 1 sector.
//
// bit [0]:	When 1, sector 0 of the non-main region will be protected from
// program
// 		and erase.
// bit [1]:	When 1, sector 1 of the non-main region will be protected from
// program
// 		and erase.
// 	:
// 	:
// bit [31]:	When 1, sector 31 of the non-main will be protected from program
// 		and erase.
// ENUMs:
// MAXIMUM                  Maximum value of VAL
// MINIMUM                  Minimum value of VAL
#define FLASH_CMDWEPROTNM_VAL_W                                              2U
#define FLASH_CMDWEPROTNM_VAL_M                                     0x00000003U
#define FLASH_CMDWEPROTNM_VAL_S                                              0U
#define FLASH_CMDWEPROTNM_VAL_MAXIMUM                               0xFFFFFFFFU
#define FLASH_CMDWEPROTNM_VAL_MINIMUM                               0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CMDWEPROTTR
//
//*****************************************************************************
// Field:     [0] VAL
//
// Each bit protects 1 sector.
//
// bit [0]:	When 1, sector 0 of the engr region will be protected from program
// 		and erase.
// bit [1]:	When 1, sector 1 of the engr region will be protected from program
// 		and erase.
// 	:
// 	:
// bit [31]:	When 1, sector 31 of the engr region will be protected from
// program
// 		and erase.
// ENUMs:
// MAXIMUM                  Maximum value of VAL
// MINIMUM                  Minimum value of VAL
#define FLASH_CMDWEPROTTR_VAL                                       0x00000001U
#define FLASH_CMDWEPROTTR_VAL_M                                     0x00000001U
#define FLASH_CMDWEPROTTR_VAL_S                                              0U
#define FLASH_CMDWEPROTTR_VAL_MAXIMUM                               0xFFFFFFFFU
#define FLASH_CMDWEPROTTR_VAL_MINIMUM                               0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CMDWEPROTEN
//
//*****************************************************************************
// Field:     [0] VAL
//
// Each bit protects 1 sector.
//
// bit [0]:	When 1, sector 0 of the engr region will be protected from program
// 		and erase.
// bit [1]:	When 1, sector 1 of the engr region will be protected from program
// 		and erase.
// 	:
// 	:
// bit [31]:	When 1, sector 31 of the engr region will be protected from
// program
// 		and erase.
// ENUMs:
// MAXIMUM                  Maximum value of VAL
// MINIMUM                  Minimum value of VAL
#define FLASH_CMDWEPROTEN_VAL                                       0x00000001U
#define FLASH_CMDWEPROTEN_VAL_M                                     0x00000001U
#define FLASH_CMDWEPROTEN_VAL_S                                              0U
#define FLASH_CMDWEPROTEN_VAL_MAXIMUM                               0xFFFFFFFFU
#define FLASH_CMDWEPROTEN_VAL_MINIMUM                               0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CFGCMD
//
//*****************************************************************************
// Field:   [3:0] WAITSTATE
//
// Wait State setting for program verify, erase verify and read verify
// ENUMs:
// MAXIMUM                  Maximum value
// MINIMUM                  Minimum value
#define FLASH_CFGCMD_WAITSTATE_W                                             4U
#define FLASH_CFGCMD_WAITSTATE_M                                    0x0000000FU
#define FLASH_CFGCMD_WAITSTATE_S                                             0U
#define FLASH_CFGCMD_WAITSTATE_MAXIMUM                              0x0000000FU
#define FLASH_CFGCMD_WAITSTATE_MINIMUM                              0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_CFGPCNT
//
//*****************************************************************************
// Field:  [11:4] MAXPCNTVAL
//
// Override maximum pulse counter with this value.
// If MAXPCNTOVR = 0, then this field is ignored.
// If MAXPCNTOVR = 1 and MAXERSPCNTOVR = 0, then this value will be used
// to override the max pulse count for both program and erase.  Full max value
// will be {4'h0, MAXPCNTVAL} .
// If MAXPCNTOVR = 1 and MAXERSPCNTOVR = 1, then this value will be used
// to override the max pulse count for program only.  Full max value will be
// {4'h0, MAXPCNTVAL}.
// ENUMs:
// MAXIMUM                  Maximum value
// MINIMUM                  Minimum value
#define FLASH_CFGPCNT_MAXPCNTVAL_W                                           8U
#define FLASH_CFGPCNT_MAXPCNTVAL_M                                  0x00000FF0U
#define FLASH_CFGPCNT_MAXPCNTVAL_S                                           4U
#define FLASH_CFGPCNT_MAXPCNTVAL_MAXIMUM                            0x00000FF0U
#define FLASH_CFGPCNT_MAXPCNTVAL_MINIMUM                            0x00000000U

// Field:     [0] MAXPCNTOVR
//
// Override hard-wired maximum pulse count.  If MAXERSPCNTOVR
// is not set, then setting this value alone will override the max pulse count
// for
// both program and erase.  If MAXERSPCNTOVR is set, then this bit will only
// control the max pulse count setting for program.
// By default, this bit is 0, and a hard-wired max pulse count is used.
// ENUMs:
// OVERRIDE                 Use value from MAXPCNTVAL field as maximum puse
//                          count
// DEFAULT                  Use hard-wired (default) value for maximum pulse
//                          count
#define FLASH_CFGPCNT_MAXPCNTOVR                                    0x00000001U
#define FLASH_CFGPCNT_MAXPCNTOVR_M                                  0x00000001U
#define FLASH_CFGPCNT_MAXPCNTOVR_S                                           0U
#define FLASH_CFGPCNT_MAXPCNTOVR_OVERRIDE                           0x00000001U
#define FLASH_CFGPCNT_MAXPCNTOVR_DEFAULT                            0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_STATCMD
//
//*****************************************************************************
// Field:    [12] FAILMISC
//
// Command failed due to error other than write/erase protect violation or
// verify
// error.  This is an extra bit in case a new failure mechanism is added which
// requires a status bit.
// ENUMs:
// STATFAIL                 Fail
// STATNOFAIL               No Fail
#define FLASH_STATCMD_FAILMISC                                      0x00001000U
#define FLASH_STATCMD_FAILMISC_M                                    0x00001000U
#define FLASH_STATCMD_FAILMISC_S                                            12U
#define FLASH_STATCMD_FAILMISC_STATFAIL                             0x00001000U
#define FLASH_STATCMD_FAILMISC_STATNOFAIL                           0x00000000U

// Field:     [8] FAILINVDATA
//
// Program command failed because an attempt was made to program a stored
// 0 value to a 1.
// ENUMs:
// STATFAIL                 Fail
// STATNOFAIL               No Fail
#define FLASH_STATCMD_FAILINVDATA                                   0x00000100U
#define FLASH_STATCMD_FAILINVDATA_M                                 0x00000100U
#define FLASH_STATCMD_FAILINVDATA_S                                          8U
#define FLASH_STATCMD_FAILINVDATA_STATFAIL                          0x00000100U
#define FLASH_STATCMD_FAILINVDATA_STATNOFAIL                        0x00000000U

// Field:     [7] FAILMODE
//
// Command failed because a bank has been set to a mode other than READ.
// Program and Erase commands cannot be initiated unless all banks are in READ
// mode.
// ENUMs:
// STATFAIL                 Fail
// STATNOFAIL               No Fail
#define FLASH_STATCMD_FAILMODE                                      0x00000080U
#define FLASH_STATCMD_FAILMODE_M                                    0x00000080U
#define FLASH_STATCMD_FAILMODE_S                                             7U
#define FLASH_STATCMD_FAILMODE_STATFAIL                             0x00000080U
#define FLASH_STATCMD_FAILMODE_STATNOFAIL                           0x00000000U

// Field:     [6] FAILILLADDR
//
// Command failed due to the use of an illegal address
// ENUMs:
// STATFAIL                 Fail
// STATNOFAIL               No Fail
#define FLASH_STATCMD_FAILILLADDR                                   0x00000040U
#define FLASH_STATCMD_FAILILLADDR_M                                 0x00000040U
#define FLASH_STATCMD_FAILILLADDR_S                                          6U
#define FLASH_STATCMD_FAILILLADDR_STATFAIL                          0x00000040U
#define FLASH_STATCMD_FAILILLADDR_STATNOFAIL                        0x00000000U

// Field:     [5] FAILVERIFY
//
// Command failed due to verify error
// ENUMs:
// STATFAIL                 Fail
// STATNOFAIL               No Fail
#define FLASH_STATCMD_FAILVERIFY                                    0x00000020U
#define FLASH_STATCMD_FAILVERIFY_M                                  0x00000020U
#define FLASH_STATCMD_FAILVERIFY_S                                           5U
#define FLASH_STATCMD_FAILVERIFY_STATFAIL                           0x00000020U
#define FLASH_STATCMD_FAILVERIFY_STATNOFAIL                         0x00000000U

// Field:     [4] FAILWEPROT
//
// Command failed due to Write/Erase Protect Sector Violation
// ENUMs:
// STATFAIL                 Fail
// STATNOFAIL               No Fail
#define FLASH_STATCMD_FAILWEPROT                                    0x00000010U
#define FLASH_STATCMD_FAILWEPROT_M                                  0x00000010U
#define FLASH_STATCMD_FAILWEPROT_S                                           4U
#define FLASH_STATCMD_FAILWEPROT_STATFAIL                           0x00000010U
#define FLASH_STATCMD_FAILWEPROT_STATNOFAIL                         0x00000000U

// Field:     [2] CMDINPROGRESS
//
// Command In Progress
// ENUMs:
// STATINPROGRESS           In Progress
// STATCOMPLETE             Complete
#define FLASH_STATCMD_CMDINPROGRESS                                 0x00000004U
#define FLASH_STATCMD_CMDINPROGRESS_M                               0x00000004U
#define FLASH_STATCMD_CMDINPROGRESS_S                                        2U
#define FLASH_STATCMD_CMDINPROGRESS_STATINPROGRESS                  0x00000004U
#define FLASH_STATCMD_CMDINPROGRESS_STATCOMPLETE                    0x00000000U

// Field:     [1] CMDPASS
//
// Command Pass - valid when CMD_DONE field is 1
// ENUMs:
// STATPASS                 Pass
// STATFAIL                 Fail
#define FLASH_STATCMD_CMDPASS                                       0x00000002U
#define FLASH_STATCMD_CMDPASS_M                                     0x00000002U
#define FLASH_STATCMD_CMDPASS_S                                              1U
#define FLASH_STATCMD_CMDPASS_STATPASS                              0x00000002U
#define FLASH_STATCMD_CMDPASS_STATFAIL                              0x00000000U

// Field:     [0] CMDDONE
//
// Command Done
// ENUMs:
// STATDONE                 Done
// STATNOTDONE              Not Done
#define FLASH_STATCMD_CMDDONE                                       0x00000001U
#define FLASH_STATCMD_CMDDONE_M                                     0x00000001U
#define FLASH_STATCMD_CMDDONE_S                                              0U
#define FLASH_STATCMD_CMDDONE_STATDONE                              0x00000001U
#define FLASH_STATCMD_CMDDONE_STATNOTDONE                           0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_STATADDR
//
//*****************************************************************************
// Field: [25:21] BANKID
//
// Current Bank ID
// A bank indicator is stored in this register which represents the current
// bank on
// which the state  machine is operating.  There is 1 bit per bank.
// ENUMs:
// BANK4                    Bank 4
// BANK3                    Bank 3
// BANK2                    Bank 2
// BANK1                    Bank 1
// BANK0                    Bank 0
#define FLASH_STATADDR_BANKID_W                                              5U
#define FLASH_STATADDR_BANKID_M                                     0x03E00000U
#define FLASH_STATADDR_BANKID_S                                             21U
#define FLASH_STATADDR_BANKID_BANK4                                 0x02000000U
#define FLASH_STATADDR_BANKID_BANK3                                 0x01000000U
#define FLASH_STATADDR_BANKID_BANK2                                 0x00800000U
#define FLASH_STATADDR_BANKID_BANK1                                 0x00400000U
#define FLASH_STATADDR_BANKID_BANK0                                 0x00200000U

// Field: [20:16] REGIONID
//
// Current Region ID
// A region indicator is stored in this register which represents the current
// flash
// region on which the state  machine is operating.
// ENUMs:
// ENGR                     Engr Region
// TRIM                     Trim Region
// NONMAIN                  Non-Main Region
// MAIN                     Main Region
#define FLASH_STATADDR_REGIONID_W                                            5U
#define FLASH_STATADDR_REGIONID_M                                   0x001F0000U
#define FLASH_STATADDR_REGIONID_S                                           16U
#define FLASH_STATADDR_REGIONID_ENGR                                0x00080000U
#define FLASH_STATADDR_REGIONID_TRIM                                0x00040000U
#define FLASH_STATADDR_REGIONID_NONMAIN                             0x00020000U
#define FLASH_STATADDR_REGIONID_MAIN                                0x00010000U

// Field:  [15:0] BANKADDR
//
// Current Bank Address
// A bank offset address is stored in this register.
// ENUMs:
// MAXIMUM                  Maximum value
// MINIMUM                  Minimum value
#define FLASH_STATADDR_BANKADDR_W                                           16U
#define FLASH_STATADDR_BANKADDR_M                                   0x0000FFFFU
#define FLASH_STATADDR_BANKADDR_S                                            0U
#define FLASH_STATADDR_BANKADDR_MAXIMUM                             0x0000FFFFU
#define FLASH_STATADDR_BANKADDR_MINIMUM                             0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_STATPCNT
//
//*****************************************************************************
// Field:  [11:0] PULSECNT
//
// Current Pulse Counter Value
// ENUMs:
// MAXIMUM                  Maximum value
// MINIMUM                  Minimum value
#define FLASH_STATPCNT_PULSECNT_W                                           12U
#define FLASH_STATPCNT_PULSECNT_M                                   0x00000FFFU
#define FLASH_STATPCNT_PULSECNT_S                                            0U
#define FLASH_STATPCNT_PULSECNT_MAXIMUM                             0x00000FFFU
#define FLASH_STATPCNT_PULSECNT_MINIMUM                             0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_STATMODE
//
//*****************************************************************************
// Field:    [17] BANK1TRDY
//
// Bank 1T Ready.
// Bank(s) are ready for 1T access.  This is accomplished when the bank and
// pump
// have been trimmed.
// ENUMs:
// TRUE                     Ready
// FALSE                    Not ready
#define FLASH_STATMODE_BANK1TRDY                                    0x00020000U
#define FLASH_STATMODE_BANK1TRDY_M                                  0x00020000U
#define FLASH_STATMODE_BANK1TRDY_S                                          17U
#define FLASH_STATMODE_BANK1TRDY_TRUE                               0x00020000U
#define FLASH_STATMODE_BANK1TRDY_FALSE                              0x00000000U

// Field:    [16] BANK2TRDY
//
// Bank 2T Ready.
// Bank(s) are ready for 2T access.  This is accomplished when the pump has
// fully driven power rails to the bank(s).
// ENUMs:
// TRUE                     Ready
// FALSE                    Not ready
#define FLASH_STATMODE_BANK2TRDY                                    0x00010000U
#define FLASH_STATMODE_BANK2TRDY_M                                  0x00010000U
#define FLASH_STATMODE_BANK2TRDY_S                                          16U
#define FLASH_STATMODE_BANK2TRDY_TRUE                               0x00010000U
#define FLASH_STATMODE_BANK2TRDY_FALSE                              0x00000000U

// Field:  [11:8] BANKMODE
//
// Indicates mode of bank(s) that are not in READ mode
// ENUMs:
// ERASEBNK                 Erase Bank
// PGMMW                    Program Multiple Word
// ERASESECT                Erase Sector
// ERASEVER                 Erase Verify Mode
// PGMSW                    Program Single Word
// PGMVER                   Program Verify Mode
// RDMARG1B                 Read Margin 1B Mode
// RDMARG0B                 Read Margin 0B Mode
// RDMARG1                  Read Margin 1 Mode
// RDMARG0                  Read Margin 0 Mode
// READ                     Read Mode
#define FLASH_STATMODE_BANKMODE_W                                            4U
#define FLASH_STATMODE_BANKMODE_M                                   0x00000F00U
#define FLASH_STATMODE_BANKMODE_S                                            8U
#define FLASH_STATMODE_BANKMODE_ERASEBNK                            0x00000F00U
#define FLASH_STATMODE_BANKMODE_PGMMW                               0x00000E00U
#define FLASH_STATMODE_BANKMODE_ERASESECT                           0x00000C00U
#define FLASH_STATMODE_BANKMODE_ERASEVER                            0x00000B00U
#define FLASH_STATMODE_BANKMODE_PGMSW                               0x00000A00U
#define FLASH_STATMODE_BANKMODE_PGMVER                              0x00000900U
#define FLASH_STATMODE_BANKMODE_RDMARG1B                            0x00000700U
#define FLASH_STATMODE_BANKMODE_RDMARG0B                            0x00000600U
#define FLASH_STATMODE_BANKMODE_RDMARG1                             0x00000400U
#define FLASH_STATMODE_BANKMODE_RDMARG0                             0x00000200U
#define FLASH_STATMODE_BANKMODE_READ                                0x00000000U

// Field:   [1:0] BANKNOTINRD
//
// Bank not in read mode.
// Indicates which banks are not in READ mode.  There is 1 bit per bank.
// ENUMs:
// BANK4                    Bank 4
// BANK3                    Bank 3
// BANK2                    Bank 2
// BANK1                    Bank 1
// BANK0                    Bank 0
#define FLASH_STATMODE_BANKNOTINRD_W                                         2U
#define FLASH_STATMODE_BANKNOTINRD_M                                0x00000003U
#define FLASH_STATMODE_BANKNOTINRD_S                                         0U
#define FLASH_STATMODE_BANKNOTINRD_BANK4                            0x00000010U
#define FLASH_STATMODE_BANKNOTINRD_BANK3                            0x00000008U
#define FLASH_STATMODE_BANKNOTINRD_BANK2                            0x00000004U
#define FLASH_STATMODE_BANKNOTINRD_BANK1                            0x00000002U
#define FLASH_STATMODE_BANKNOTINRD_BANK0                            0x00000001U

//*****************************************************************************
//
// Register: FLASH_O_GBLINFO0
//
//*****************************************************************************
// Field: [18:16] NUMBANKS
//
// Number of banks instantiated
// Minimum:	1
// Maximum:	5
// ENUMs:
// MAXIMUM                  Maximum value
// MINIMUM                  Minimum value
#define FLASH_GBLINFO0_NUMBANKS_W                                            3U
#define FLASH_GBLINFO0_NUMBANKS_M                                   0x00070000U
#define FLASH_GBLINFO0_NUMBANKS_S                                           16U
#define FLASH_GBLINFO0_NUMBANKS_MAXIMUM                             0x00050000U
#define FLASH_GBLINFO0_NUMBANKS_MINIMUM                             0x00010000U

// Field:  [15:0] SECTORSIZE
//
// Sector size in bytes
// ENUMs:
// TWOKB                    Sector size is TWOKB
// ONEKB                    Sector size is ONEKB
#define FLASH_GBLINFO0_SECTORSIZE_W                                         16U
#define FLASH_GBLINFO0_SECTORSIZE_M                                 0x0000FFFFU
#define FLASH_GBLINFO0_SECTORSIZE_S                                          0U
#define FLASH_GBLINFO0_SECTORSIZE_TWOKB                             0x00000800U
#define FLASH_GBLINFO0_SECTORSIZE_ONEKB                             0x00000400U

//*****************************************************************************
//
// Register: FLASH_O_GBLINFO1
//
//*****************************************************************************
// Field: [18:16] REDWIDTH
//
// Redundant data width in bits
// ENUMs:
// W4BIT                    Redundant data width is 4 bits
// W2BIT                    Redundant data width is 2 bits
// W0BIT                    Redundant data width is 0.  Redundancy/Repair not
//                          present.
#define FLASH_GBLINFO1_REDWIDTH_W                                            3U
#define FLASH_GBLINFO1_REDWIDTH_M                                   0x00070000U
#define FLASH_GBLINFO1_REDWIDTH_S                                           16U
#define FLASH_GBLINFO1_REDWIDTH_W4BIT                               0x00040000U
#define FLASH_GBLINFO1_REDWIDTH_W2BIT                               0x00020000U
#define FLASH_GBLINFO1_REDWIDTH_W0BIT                               0x00000000U

// Field:  [12:8] ECCWIDTH
//
// ECC data width in bits
// ENUMs:
// W16BIT                   ECC data width is 16 bits
// W8BIT                    ECC data width is 8 bits
// W0BIT                    ECC data width is 0.  ECC not used.
#define FLASH_GBLINFO1_ECCWIDTH_W                                            5U
#define FLASH_GBLINFO1_ECCWIDTH_M                                   0x00001F00U
#define FLASH_GBLINFO1_ECCWIDTH_S                                            8U
#define FLASH_GBLINFO1_ECCWIDTH_W16BIT                              0x00001000U
#define FLASH_GBLINFO1_ECCWIDTH_W8BIT                               0x00000800U
#define FLASH_GBLINFO1_ECCWIDTH_W0BIT                               0x00000000U

// Field:   [7:0] DATAWIDTH
//
// Data width in bits
// ENUMs:
// W128BIT                  Data width is 128 bits
// W64BIT                   Data width is 64 bits
#define FLASH_GBLINFO1_DATAWIDTH_W                                           8U
#define FLASH_GBLINFO1_DATAWIDTH_M                                  0x000000FFU
#define FLASH_GBLINFO1_DATAWIDTH_S                                           0U
#define FLASH_GBLINFO1_DATAWIDTH_W128BIT                            0x00000080U
#define FLASH_GBLINFO1_DATAWIDTH_W64BIT                             0x00000040U

//*****************************************************************************
//
// Register: FLASH_O_GBLINFO2
//
//*****************************************************************************
// Field:   [3:0] DATAREGISTERS
//
// Number of data registers present.
// ENUMs:
// MAXIMUM                  Maximum value of DATAREGISTERS
// MINIMUM                  Minimum value of DATAREGISTERS
#define FLASH_GBLINFO2_DATAREGISTERS_W                                       4U
#define FLASH_GBLINFO2_DATAREGISTERS_M                              0x0000000FU
#define FLASH_GBLINFO2_DATAREGISTERS_S                                       0U
#define FLASH_GBLINFO2_DATAREGISTERS_MAXIMUM                        0x00000008U
#define FLASH_GBLINFO2_DATAREGISTERS_MINIMUM                        0x00000001U

//*****************************************************************************
//
// Register: FLASH_O_BANK0INFO0
//
//*****************************************************************************
// Field:  [11:0] MAINSIZE
//
// Main region size in sectors
// Minimum:	0x8 (8)
// Maximum:	0x200 (512)
// ENUMs:
// MAXSECTORS               Maximum value of MAINSIZE
// MINSECTORS               Minimum value of MAINSIZE
#define FLASH_BANK0INFO0_MAINSIZE_W                                         12U
#define FLASH_BANK0INFO0_MAINSIZE_M                                 0x00000FFFU
#define FLASH_BANK0INFO0_MAINSIZE_S                                          0U
#define FLASH_BANK0INFO0_MAINSIZE_MAXSECTORS                        0x00000200U
#define FLASH_BANK0INFO0_MAINSIZE_MINSECTORS                        0x00000008U

//*****************************************************************************
//
// Register: FLASH_O_BANK0INFO1
//
//*****************************************************************************
// Field: [23:16] ENGRSIZE
//
// Engr region size in sectors
// Minimum:	0x0 (0)
// Maximum:	0x10 (16)
// ENUMs:
// MAXSECTORS               Maximum value of ENGRSIZE
// MINSECTORS               Minimum value of ENGRSIZE
#define FLASH_BANK0INFO1_ENGRSIZE_W                                          8U
#define FLASH_BANK0INFO1_ENGRSIZE_M                                 0x00FF0000U
#define FLASH_BANK0INFO1_ENGRSIZE_S                                         16U
#define FLASH_BANK0INFO1_ENGRSIZE_MAXSECTORS                        0x00200000U
#define FLASH_BANK0INFO1_ENGRSIZE_MINSECTORS                        0x00000000U

// Field:  [15:8] TRIMSIZE
//
// Trim region size in sectors
// Minimum:	0x0 (0)
// Maximum:	0x10 (16)
// ENUMs:
// MAXSECTORS               Maximum value of TRIMSIZE
// MINSECTORS               Minimum value of TRIMSIZE
#define FLASH_BANK0INFO1_TRIMSIZE_W                                          8U
#define FLASH_BANK0INFO1_TRIMSIZE_M                                 0x0000FF00U
#define FLASH_BANK0INFO1_TRIMSIZE_S                                          8U
#define FLASH_BANK0INFO1_TRIMSIZE_MAXSECTORS                        0x00002000U
#define FLASH_BANK0INFO1_TRIMSIZE_MINSECTORS                        0x00000000U

// Field:   [7:0] NONMAINSIZE
//
// Non-main region size in sectors
// Minimum:	0x0 (0)
// Maximum:	0x10 (16)
// ENUMs:
// MAXSECTORS               Maximum value of NONMAINSIZE
// MINSECTORS               Minimum value of NONMAINSIZE
#define FLASH_BANK0INFO1_NONMAINSIZE_W                                       8U
#define FLASH_BANK0INFO1_NONMAINSIZE_M                              0x000000FFU
#define FLASH_BANK0INFO1_NONMAINSIZE_S                                       0U
#define FLASH_BANK0INFO1_NONMAINSIZE_MAXSECTORS                     0x00000020U
#define FLASH_BANK0INFO1_NONMAINSIZE_MINSECTORS                     0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_BANK1INFO0
//
//*****************************************************************************
// Field:  [11:0] MAINSIZE
//
// Main region size in sectors
// Minimum:	0x8 (8)
// Maximum:	0x200 (512)
// ENUMs:
// MAXSECTORS               Maximum value of MAINSIZE
// MINSECTORS               Minimum value of MAINSIZE
#define FLASH_BANK1INFO0_MAINSIZE_W                                         12U
#define FLASH_BANK1INFO0_MAINSIZE_M                                 0x00000FFFU
#define FLASH_BANK1INFO0_MAINSIZE_S                                          0U
#define FLASH_BANK1INFO0_MAINSIZE_MAXSECTORS                        0x00000200U
#define FLASH_BANK1INFO0_MAINSIZE_MINSECTORS                        0x00000008U

//*****************************************************************************
//
// Register: FLASH_O_BANK1INFO1
//
//*****************************************************************************
// Field: [23:16] ENGRSIZE
//
// Engr region size in sectors
// Minimum:	0x0 (0)
// Maximum:	0x10 (16)
// ENUMs:
// MAXSECTORS               Maximum value of ENGRSIZE
// MINSECTORS               Minimum value of ENGRSIZE
#define FLASH_BANK1INFO1_ENGRSIZE_W                                          8U
#define FLASH_BANK1INFO1_ENGRSIZE_M                                 0x00FF0000U
#define FLASH_BANK1INFO1_ENGRSIZE_S                                         16U
#define FLASH_BANK1INFO1_ENGRSIZE_MAXSECTORS                        0x00200000U
#define FLASH_BANK1INFO1_ENGRSIZE_MINSECTORS                        0x00000000U

// Field:  [15:8] TRIMSIZE
//
// Trim region size in sectors
// ENUMs:
// MAXSECTORS               Maximum value of TRIMSIZE
// MINSECTORS               Minimum value of TRIMSIZE
#define FLASH_BANK1INFO1_TRIMSIZE_W                                          8U
#define FLASH_BANK1INFO1_TRIMSIZE_M                                 0x0000FF00U
#define FLASH_BANK1INFO1_TRIMSIZE_S                                          8U
#define FLASH_BANK1INFO1_TRIMSIZE_MAXSECTORS                        0x00002000U
#define FLASH_BANK1INFO1_TRIMSIZE_MINSECTORS                        0x00000000U

// Field:   [7:0] NONMAINSIZE
//
// Non-main region size in sectors
// ENUMs:
// MAXSECTORS               Maximum value of NONMAINSIZE
// MINSECTORS               Minimum value of NONMAINSIZE
#define FLASH_BANK1INFO1_NONMAINSIZE_W                                       8U
#define FLASH_BANK1INFO1_NONMAINSIZE_M                              0x000000FFU
#define FLASH_BANK1INFO1_NONMAINSIZE_S                                       0U
#define FLASH_BANK1INFO1_NONMAINSIZE_MAXSECTORS                     0x00000020U
#define FLASH_BANK1INFO1_NONMAINSIZE_MINSECTORS                     0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_DFTEN
//
//*****************************************************************************
// Field:     [0] ENABLE
//
// Enable Test Features
// ENUMs:
// ENABLED                  Command
// DISABLED                 Command
#define FLASH_DFTEN_ENABLE                                          0x00000001U
#define FLASH_DFTEN_ENABLE_M                                        0x00000001U
#define FLASH_DFTEN_ENABLE_S                                                 0U
#define FLASH_DFTEN_ENABLE_ENABLED                                  0x00000001U
#define FLASH_DFTEN_ENABLE_DISABLED                                 0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_DFTCMDCTL
//
//*****************************************************************************
// Field: [31:28] DTBMUXSEL
//
// DTB Mux Select
// This field will form the select for the primary DTB mux.  This mux selects
// up to
// 16 sets of 32-bit fields of internal signals to be present to the 32-bit DTB
// output.
// ENUMs:
// MAXIMUM                  Maximum value
// MINIMUM                  Minimum value
#define FLASH_DFTCMDCTL_DTBMUXSEL_W                                          4U
#define FLASH_DFTCMDCTL_DTBMUXSEL_M                                 0xF0000000U
#define FLASH_DFTCMDCTL_DTBMUXSEL_S                                         28U
#define FLASH_DFTCMDCTL_DTBMUXSEL_MAXIMUM                           0xF0000000U
#define FLASH_DFTCMDCTL_DTBMUXSEL_MINIMUM                           0x00000000U

// Field:    [27] DTB16MUXSEL
//
// DTB16 Mux Select
// This bit is valid when the flash wrapper DTB output bus width is less than
// 32 bits.
// Specifically, this bit is valid when the DTB width is 16 bits or less.  When
// this is the case,
// then DTB16MUXSEL is used to select either the upper or lower 16-bit portion
// of
// the native 32-bit DTB bus.
// 8-bit portion of the 16-bit DTB value.
// ENUMs:
// UPPER16BITS              Select upper 16 bits of 32-bit DTB
// LOWER16BITS              Select lower 16 bits of 32-bit DTB
#define FLASH_DFTCMDCTL_DTB16MUXSEL                                 0x08000000U
#define FLASH_DFTCMDCTL_DTB16MUXSEL_M                               0x08000000U
#define FLASH_DFTCMDCTL_DTB16MUXSEL_S                                       27U
#define FLASH_DFTCMDCTL_DTB16MUXSEL_UPPER16BITS                     0x08000000U
#define FLASH_DFTCMDCTL_DTB16MUXSEL_LOWER16BITS                     0x00000000U

// Field:    [24] ERASEMASKDIS
//
// Disable use of erase mask for erase
// Bit masking will not be used during erase verify.  If any sectors fail the
// verify either before (prever) or after (postver) the operation, then all
// specified
// flash sectors will receive subsequent erase pulse.
// ENUMs:
// DISABLE                  Disable
// ENABLE                   Enable
#define FLASH_DFTCMDCTL_ERASEMASKDIS                                0x01000000U
#define FLASH_DFTCMDCTL_ERASEMASKDIS_M                              0x01000000U
#define FLASH_DFTCMDCTL_ERASEMASKDIS_S                                      24U
#define FLASH_DFTCMDCTL_ERASEMASKDIS_DISABLE                        0x01000000U
#define FLASH_DFTCMDCTL_ERASEMASKDIS_ENABLE                         0x00000000U

// Field:    [23] PROGMASKDIS
//
// Disable use of program mask for programming.
// Bit masking will not be used during program verify.  If any bits fail the
// verify either before (prever) or after (postver) the operation, then all
// specified
// flash entries will receive subsequent program pulse.
// ENUMs:
// DISABLE                  Disable
// ENABLE                   Enable
#define FLASH_DFTCMDCTL_PROGMASKDIS                                 0x00800000U
#define FLASH_DFTCMDCTL_PROGMASKDIS_M                               0x00800000U
#define FLASH_DFTCMDCTL_PROGMASKDIS_S                                       23U
#define FLASH_DFTCMDCTL_PROGMASKDIS_DISABLE                         0x00800000U
#define FLASH_DFTCMDCTL_PROGMASKDIS_ENABLE                          0x00000000U

// Field:    [22] POSTVEREN
//
// Enable verify after program or erase
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define FLASH_DFTCMDCTL_POSTVEREN                                   0x00400000U
#define FLASH_DFTCMDCTL_POSTVEREN_M                                 0x00400000U
#define FLASH_DFTCMDCTL_POSTVEREN_S                                         22U
#define FLASH_DFTCMDCTL_POSTVEREN_ENABLE                            0x00400000U
#define FLASH_DFTCMDCTL_POSTVEREN_DISABLE                           0x00000000U

// Field:    [21] PREVEREN
//
// Enable verify before program or erase.  For program, bits already programmed
// to the requested value will be masked.  For erase, sectors already erased
// will be
// masked.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define FLASH_DFTCMDCTL_PREVEREN                                    0x00200000U
#define FLASH_DFTCMDCTL_PREVEREN_M                                  0x00200000U
#define FLASH_DFTCMDCTL_PREVEREN_S                                          21U
#define FLASH_DFTCMDCTL_PREVEREN_ENABLE                             0x00200000U
#define FLASH_DFTCMDCTL_PREVEREN_DISABLE                            0x00000000U

// Field:    [20] STOPVERONFAIL
//
// Stop read verify on fail.  If this bit is set, read verify will halt when
// the first verify
// fail is detected.  If command is program or erase, another program or erase
// pulse will be executed.  If command is read verify, comand will terminate.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define FLASH_DFTCMDCTL_STOPVERONFAIL                               0x00100000U
#define FLASH_DFTCMDCTL_STOPVERONFAIL_M                             0x00100000U
#define FLASH_DFTCMDCTL_STOPVERONFAIL_S                                     20U
#define FLASH_DFTCMDCTL_STOPVERONFAIL_ENABLE                        0x00100000U
#define FLASH_DFTCMDCTL_STOPVERONFAIL_DISABLE                       0x00000000U

// Field:    [18] ODDROWINVDATA
//
// Invert data at odd row addresses for program or verify.  The LSB of the row
// address is bit [4] of the bank address.  This bit only applies when pattern
// data
// is used; i.e. the DATAPATEN bit is set.  It will have no effect if CMDDATA
// is used.
// ENUMs:
// INVERT                   Use inverted data
// TRUE                     Use true data
#define FLASH_DFTCMDCTL_ODDROWINVDATA                               0x00040000U
#define FLASH_DFTCMDCTL_ODDROWINVDATA_M                             0x00040000U
#define FLASH_DFTCMDCTL_ODDROWINVDATA_S                                     18U
#define FLASH_DFTCMDCTL_ODDROWINVDATA_INVERT                        0x00040000U
#define FLASH_DFTCMDCTL_ODDROWINVDATA_TRUE                          0x00000000U

// Field:    [17] ODDWORDINVDATA
//
// Invert data at odd bank addresses for program or verify. This bit only
// applies
// when pattern data is used; i.e. the DATAPATEN bit is set.  It will have no
// effect
// if CMDDATA is used.
// ENUMs:
// INVERT                   Use inverted data
// TRUE                     Use true data
#define FLASH_DFTCMDCTL_ODDWORDINVDATA                              0x00020000U
#define FLASH_DFTCMDCTL_ODDWORDINVDATA_M                            0x00020000U
#define FLASH_DFTCMDCTL_ODDWORDINVDATA_S                                    17U
#define FLASH_DFTCMDCTL_ODDWORDINVDATA_INVERT                       0x00020000U
#define FLASH_DFTCMDCTL_ODDWORDINVDATA_TRUE                         0x00000000U

// Field:    [16] ALWAYSINVDATA
//
// Invert data always for program or verify.  This bit only applies when
// pattern data
// is used; i.e. the DATAPATEN bit is set.  It will have no effect if CMDDATA
// is used.
// ENUMs:
// INVERT                   Use inverted data
// TRUE                     Use true data
#define FLASH_DFTCMDCTL_ALWAYSINVDATA                               0x00010000U
#define FLASH_DFTCMDCTL_ALWAYSINVDATA_M                             0x00010000U
#define FLASH_DFTCMDCTL_ALWAYSINVDATA_S                                     16U
#define FLASH_DFTCMDCTL_ALWAYSINVDATA_INVERT                        0x00010000U
#define FLASH_DFTCMDCTL_ALWAYSINVDATA_TRUE                          0x00000000U

// Field: [15:13] DATAPATSEL
//
// Select data pattern.  Valid when DATAPATEN bit is set to 1.  Overrides
// CMDDATA
// registers for program or verify.
// ENUMs:
// LOGCHKBRD                Set to logical checkerboard (0x01010101...)
// ALL1                     Set to all 1
// ALL0                     Set to all 0
#define FLASH_DFTCMDCTL_DATAPATSEL_W                                         3U
#define FLASH_DFTCMDCTL_DATAPATSEL_M                                0x0000E000U
#define FLASH_DFTCMDCTL_DATAPATSEL_S                                        13U
#define FLASH_DFTCMDCTL_DATAPATSEL_LOGCHKBRD                        0x00004000U
#define FLASH_DFTCMDCTL_DATAPATSEL_ALL1                             0x00002000U
#define FLASH_DFTCMDCTL_DATAPATSEL_ALL0                             0x00000000U

// Field:    [12] DATAPATEN
//
// Enable data pattern.  Data pattern select in DATAPATSEL field will override
// data
// from CMDDATA registers for use as program or verify data.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define FLASH_DFTCMDCTL_DATAPATEN                                   0x00001000U
#define FLASH_DFTCMDCTL_DATAPATEN_M                                 0x00001000U
#define FLASH_DFTCMDCTL_DATAPATEN_S                                         12U
#define FLASH_DFTCMDCTL_DATAPATEN_ENABLE                            0x00001000U
#define FLASH_DFTCMDCTL_DATAPATEN_DISABLE                           0x00000000U

// Field:     [9] PULSECNTLDDIS
//
// Override pulse counter enable.
// When set, the state machine pulse counter will not be loaded when a
// command is initiated.
// ENUMs:
// DISABLE                  Disable
// ENABLE                   Enable
#define FLASH_DFTCMDCTL_PULSECNTLDDIS                               0x00000200U
#define FLASH_DFTCMDCTL_PULSECNTLDDIS_M                             0x00000200U
#define FLASH_DFTCMDCTL_PULSECNTLDDIS_S                                      9U
#define FLASH_DFTCMDCTL_PULSECNTLDDIS_DISABLE                       0x00000200U
#define FLASH_DFTCMDCTL_PULSECNTLDDIS_ENABLE                        0x00000000U

// Field:     [8] ADDRCNTLDDIS
//
// Override address counter enable.
// When set, the state machine address counter will not be loaded when a
// command is initiated.
// ENUMs:
// DISABLE                  Disable
// ENABLE                   Enable
#define FLASH_DFTCMDCTL_ADDRCNTLDDIS                                0x00000100U
#define FLASH_DFTCMDCTL_ADDRCNTLDDIS_M                              0x00000100U
#define FLASH_DFTCMDCTL_ADDRCNTLDDIS_S                                       8U
#define FLASH_DFTCMDCTL_ADDRCNTLDDIS_DISABLE                        0x00000100U
#define FLASH_DFTCMDCTL_ADDRCNTLDDIS_ENABLE                         0x00000000U

// Field:     [5] REDMATCHFORCE
//
// Force redundancy match.  If set to 1, repair configuration encoded in the
// flash
// bank trim will be forced for every access.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define FLASH_DFTCMDCTL_REDMATCHFORCE                               0x00000020U
#define FLASH_DFTCMDCTL_REDMATCHFORCE_M                             0x00000020U
#define FLASH_DFTCMDCTL_REDMATCHFORCE_S                                      5U
#define FLASH_DFTCMDCTL_REDMATCHFORCE_ENABLE                        0x00000020U
#define FLASH_DFTCMDCTL_REDMATCHFORCE_DISABLE                       0x00000000U

// Field:     [4] REDMATCHDIS
//
// Disable redundancy matching.  Any repair configuration encoded into the
// bank trim bits is disabled.
// ENUMs:
// DISABLE                  Disable
// ENABLE                   Enable
#define FLASH_DFTCMDCTL_REDMATCHDIS                                 0x00000010U
#define FLASH_DFTCMDCTL_REDMATCHDIS_M                               0x00000010U
#define FLASH_DFTCMDCTL_REDMATCHDIS_S                                        4U
#define FLASH_DFTCMDCTL_REDMATCHDIS_DISABLE                         0x00000010U
#define FLASH_DFTCMDCTL_REDMATCHDIS_ENABLE                          0x00000000U

// Field:     [2] AMX2TDIS
//
// 2T address mux disable control. When set to 1 2T address shifting is
// disabled.  This bit should only be enabled for reads.  Indeterminate
// behavior
// will result if this bit is set during program/erase.  Furthermore, only
// reads done
// via a READVERIFY command will be guaranteed to work properly.  Reads via
// the FBAP port are not guaranteed to operate.
// ENUMs:
// DISABLE                  Disable
// ENABLE                   Enable
#define FLASH_DFTCMDCTL_AMX2TDIS                                    0x00000004U
#define FLASH_DFTCMDCTL_AMX2TDIS_M                                  0x00000004U
#define FLASH_DFTCMDCTL_AMX2TDIS_S                                           2U
#define FLASH_DFTCMDCTL_AMX2TDIS_DISABLE                            0x00000004U
#define FLASH_DFTCMDCTL_AMX2TDIS_ENABLE                             0x00000000U

// Field:     [1] FORCE2TEN
//
// Force 2T Enable - Force 2T access to regions that are designated as 1T.
// Regions
// designated as 2T will still be accessed as 2T.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define FLASH_DFTCMDCTL_FORCE2TEN                                   0x00000002U
#define FLASH_DFTCMDCTL_FORCE2TEN_M                                 0x00000002U
#define FLASH_DFTCMDCTL_FORCE2TEN_S                                          1U
#define FLASH_DFTCMDCTL_FORCE2TEN_ENABLE                            0x00000002U
#define FLASH_DFTCMDCTL_FORCE2TEN_DISABLE                           0x00000000U

// Field:     [0] FORCE1TEN
//
// Force 1T Enable - Force 1T access to regions that are designated as 2T.
// Regions
// designated as 1T will still be accessed as 1T.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define FLASH_DFTCMDCTL_FORCE1TEN                                   0x00000001U
#define FLASH_DFTCMDCTL_FORCE1TEN_M                                 0x00000001U
#define FLASH_DFTCMDCTL_FORCE1TEN_S                                          0U
#define FLASH_DFTCMDCTL_FORCE1TEN_ENABLE                            0x00000001U
#define FLASH_DFTCMDCTL_FORCE1TEN_DISABLE                           0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_DFTTIMERCTL
//
//*****************************************************************************
// Field: [30:28] TIMERCLOCKOVR
//
// Override Timer clock frequency using an ICG-based clock divide mechanism.
// To divide the timer clock, pulses can be skipped based on settings in this
// field.
// By default, this field is 0, which corresponds to no division on the timer
// clock.
// ENUMs:
// DIVIDEBY8                Divide timer clock by 8
// DIVIDEBY7                Divide timer clock by 7
// DIVIDEBY6                Divide timer clock by 6
// DIVIDEBY5                Divide timer clock by 5
// DIVIDEBY4                Divide timer clock by 4
// DIVIDEBY3                Divide timer clock by 3
// DIVIDEBY2                Divide timer clock by 2
// NODIVIDE                 No divide on timer clock
#define FLASH_DFTTIMERCTL_TIMERCLOCKOVR_W                                    3U
#define FLASH_DFTTIMERCTL_TIMERCLOCKOVR_M                           0x70000000U
#define FLASH_DFTTIMERCTL_TIMERCLOCKOVR_S                                   28U
#define FLASH_DFTTIMERCTL_TIMERCLOCKOVR_DIVIDEBY8                   0x70000000U
#define FLASH_DFTTIMERCTL_TIMERCLOCKOVR_DIVIDEBY7                   0x60000000U
#define FLASH_DFTTIMERCTL_TIMERCLOCKOVR_DIVIDEBY6                   0x50000000U
#define FLASH_DFTTIMERCTL_TIMERCLOCKOVR_DIVIDEBY5                   0x40000000U
#define FLASH_DFTTIMERCTL_TIMERCLOCKOVR_DIVIDEBY4                   0x30000000U
#define FLASH_DFTTIMERCTL_TIMERCLOCKOVR_DIVIDEBY3                   0x20000000U
#define FLASH_DFTTIMERCTL_TIMERCLOCKOVR_DIVIDEBY2                   0x10000000U
#define FLASH_DFTTIMERCTL_TIMERCLOCKOVR_NODIVIDE                    0x00000000U

// Field: [27:12] PEPULSETIMEVAL
//
// Program/Erase Pulse Time Value
// If operation is a program, this value gets loaded into bits [15:0] of the
// timer
// when the PEPULSETIMEVALOVR field is set to 1.
// If operation is an erase, this value gets loaded into bits [19:4] of the
// timer
// when the PEPULSETIMEVALOVR field is set to 1.
// ENUMs:
// MAXIMUM                  Maximum value
// MINIMUM                  Minimum value
#define FLASH_DFTTIMERCTL_PEPULSETIMEVAL_W                                  16U
#define FLASH_DFTTIMERCTL_PEPULSETIMEVAL_M                          0x0FFFF000U
#define FLASH_DFTTIMERCTL_PEPULSETIMEVAL_S                                  12U
#define FLASH_DFTTIMERCTL_PEPULSETIMEVAL_MAXIMUM                    0x0FFFF000U
#define FLASH_DFTTIMERCTL_PEPULSETIMEVAL_MINIMUM                    0x00001000U

// Field:     [8] PEPULSETIMEOVR
//
// Override Program/Erase Pulse Time
// If set, this will force the program or erase pulse time to be overridden
// with the
// value in the PEPULSETIMEVAL field.  If not set, then a hard-coded value will
// be
// used for this pulse time.
// ENUMs:
// OVERRIDE                 Use value from the PE_PULSE_TIME field for time
//                          value
// FUNCTIONAL               Use hard-wired (Functional) timer value
#define FLASH_DFTTIMERCTL_PEPULSETIMEOVR                            0x00000100U
#define FLASH_DFTTIMERCTL_PEPULSETIMEOVR_M                          0x00000100U
#define FLASH_DFTTIMERCTL_PEPULSETIMEOVR_S                                   8U
#define FLASH_DFTTIMERCTL_PEPULSETIMEOVR_OVERRIDE                   0x00000100U
#define FLASH_DFTTIMERCTL_PEPULSETIMEOVR_FUNCTIONAL                 0x00000000U

// Field:     [7] READMODETIME
//
// Read Mode Change Time
// ENUMs:
// TWOXFUNCTIONAL           Use 2x the hard-wired (functional) time value
// FUNCTIONAL               Use hard-wired (Functional) timer value
#define FLASH_DFTTIMERCTL_READMODETIME                              0x00000080U
#define FLASH_DFTTIMERCTL_READMODETIME_M                            0x00000080U
#define FLASH_DFTTIMERCTL_READMODETIME_S                                     7U
#define FLASH_DFTTIMERCTL_READMODETIME_TWOXFUNCTIONAL               0x00000080U
#define FLASH_DFTTIMERCTL_READMODETIME_FUNCTIONAL                   0x00000000U

// Field:     [6] PEVHOLDTIME
//
// Program/Erase Verify Hold Time
// ENUMs:
// TWOXFUNCTIONAL           Use 2x the hard-wired (functional) time value
// FUNCTIONAL               Use hard-wired (Functional) timer value
#define FLASH_DFTTIMERCTL_PEVHOLDTIME                               0x00000040U
#define FLASH_DFTTIMERCTL_PEVHOLDTIME_M                             0x00000040U
#define FLASH_DFTTIMERCTL_PEVHOLDTIME_S                                      6U
#define FLASH_DFTTIMERCTL_PEVHOLDTIME_TWOXFUNCTIONAL                0x00000040U
#define FLASH_DFTTIMERCTL_PEVHOLDTIME_FUNCTIONAL                    0x00000000U

// Field:     [5] PEVSETUPTIME
//
// Program/Erase Verify Setup Time
// ENUMs:
// TWOXFUNCTIONAL           Use 2x the hard-wired (functional) time value
// FUNCTIONAL               Use hard-wired (Functional) timer value
#define FLASH_DFTTIMERCTL_PEVSETUPTIME                              0x00000020U
#define FLASH_DFTTIMERCTL_PEVSETUPTIME_M                            0x00000020U
#define FLASH_DFTTIMERCTL_PEVSETUPTIME_S                                     5U
#define FLASH_DFTTIMERCTL_PEVSETUPTIME_TWOXFUNCTIONAL               0x00000020U
#define FLASH_DFTTIMERCTL_PEVSETUPTIME_FUNCTIONAL                   0x00000000U

// Field:     [4] PEVMODETIME
//
// Program/Erase Verify Mode Change Time
// ENUMs:
// TWOXFUNCTIONAL           Use 2x the hard-wired (functional) time value
// FUNCTIONAL               Use hard-wired (Functional) timer value
#define FLASH_DFTTIMERCTL_PEVMODETIME                               0x00000010U
#define FLASH_DFTTIMERCTL_PEVMODETIME_M                             0x00000010U
#define FLASH_DFTTIMERCTL_PEVMODETIME_S                                      4U
#define FLASH_DFTTIMERCTL_PEVMODETIME_TWOXFUNCTIONAL                0x00000010U
#define FLASH_DFTTIMERCTL_PEVMODETIME_FUNCTIONAL                    0x00000000U

// Field:     [3] PEHOLDTIME
//
// Program/Erase Hold Time
// ENUMs:
// TWOXFUNCTIONAL           Use 2x the hard-wired (functional) time value
// FUNCTIONAL               Use hard-wired (Functional) timer value
#define FLASH_DFTTIMERCTL_PEHOLDTIME                                0x00000008U
#define FLASH_DFTTIMERCTL_PEHOLDTIME_M                              0x00000008U
#define FLASH_DFTTIMERCTL_PEHOLDTIME_S                                       3U
#define FLASH_DFTTIMERCTL_PEHOLDTIME_TWOXFUNCTIONAL                 0x00000008U
#define FLASH_DFTTIMERCTL_PEHOLDTIME_FUNCTIONAL                     0x00000000U

// Field:     [2] PPVWORDLINETIME
//
// Program and Program Verify Wordline Switching Time
// ENUMs:
// TWOXFUNCTIONAL           Use 2x the hard-wired (functional) time value
// FUNCTIONAL               Use hard-wired (Functional) timer value
#define FLASH_DFTTIMERCTL_PPVWORDLINETIME                           0x00000004U
#define FLASH_DFTTIMERCTL_PPVWORDLINETIME_M                         0x00000004U
#define FLASH_DFTTIMERCTL_PPVWORDLINETIME_S                                  2U
#define FLASH_DFTTIMERCTL_PPVWORDLINETIME_TWOXFUNCTIONAL            0x00000004U
#define FLASH_DFTTIMERCTL_PPVWORDLINETIME_FUNCTIONAL                0x00000000U

// Field:     [1] PVHVSETUPTIME
//
// Program VHV Setup Time
// ENUMs:
// TWOXFUNCTIONAL           Use 2x the hard-wired (functional) time value
// FUNCTIONAL               Use hard-wired (Functional) timer value
#define FLASH_DFTTIMERCTL_PVHVSETUPTIME                             0x00000002U
#define FLASH_DFTTIMERCTL_PVHVSETUPTIME_M                           0x00000002U
#define FLASH_DFTTIMERCTL_PVHVSETUPTIME_S                                    1U
#define FLASH_DFTTIMERCTL_PVHVSETUPTIME_TWOXFUNCTIONAL              0x00000002U
#define FLASH_DFTTIMERCTL_PVHVSETUPTIME_FUNCTIONAL                  0x00000000U

// Field:     [0] PESETUPTIME
//
// Program/Erase Setup Time
// ENUMs:
// TWOXFUNCTIONAL           Use 2x the hard-wired (functional) time value
// FUNCTIONAL               Use hard-wired (Functional) timer value
#define FLASH_DFTTIMERCTL_PESETUPTIME                               0x00000001U
#define FLASH_DFTTIMERCTL_PESETUPTIME_M                             0x00000001U
#define FLASH_DFTTIMERCTL_PESETUPTIME_S                                      0U
#define FLASH_DFTTIMERCTL_PESETUPTIME_TWOXFUNCTIONAL                0x00000001U
#define FLASH_DFTTIMERCTL_PESETUPTIME_FUNCTIONAL                    0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_DFTEXECZCTL
//
//*****************************************************************************
// Field:     [1] EXEZ_OVR
//
// Override value to be applied to EXECUTEZ
// ENUMs:
// ONE                      Set EXECUTEZ to 1
// ZERO                     Set EXECUTEZ to 0
#define FLASH_DFTEXECZCTL_EXEZ_OVR                                  0x00000002U
#define FLASH_DFTEXECZCTL_EXEZ_OVR_M                                0x00000002U
#define FLASH_DFTEXECZCTL_EXEZ_OVR_S                                         1U
#define FLASH_DFTEXECZCTL_EXEZ_OVR_ONE                              0x00000002U
#define FLASH_DFTEXECZCTL_EXEZ_OVR_ZERO                             0x00000000U

// Field:     [0] EXEZOVREN
//
// Enable override of EXECUTEZ
// Note that when this bit is set, the flash wrapper has control of the bank
// pins.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define FLASH_DFTEXECZCTL_EXEZOVREN                                 0x00000001U
#define FLASH_DFTEXECZCTL_EXEZOVREN_M                               0x00000001U
#define FLASH_DFTEXECZCTL_EXEZOVREN_S                                        0U
#define FLASH_DFTEXECZCTL_EXEZOVREN_ENABLE                          0x00000001U
#define FLASH_DFTEXECZCTL_EXEZOVREN_DISABLE                         0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_DFTPCLKTESTCTL
//
//*****************************************************************************
// Field:     [0] ENABLE
//
// Enable the state machine which sequences measurement of pump clock
// frequency.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define FLASH_DFTPCLKTESTCTL_ENABLE                                 0x00000001U
#define FLASH_DFTPCLKTESTCTL_ENABLE_M                               0x00000001U
#define FLASH_DFTPCLKTESTCTL_ENABLE_S                                        0U
#define FLASH_DFTPCLKTESTCTL_ENABLE_ENABLE                          0x00000001U
#define FLASH_DFTPCLKTESTCTL_ENABLE_DISABLE                         0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_DFTPCLKTESTSTAT
//
//*****************************************************************************
// Field:  [15:4] CLOCKCNT
//
// Indicates the core clock count captured during the pump clock measurement.
// ENUMs:
// MAXIMUM                  Maximum count value
// MINIMUM                  Minimum count value
#define FLASH_DFTPCLKTESTSTAT_CLOCKCNT_W                                    12U
#define FLASH_DFTPCLKTESTSTAT_CLOCKCNT_M                            0x0000FFF0U
#define FLASH_DFTPCLKTESTSTAT_CLOCKCNT_S                                     4U
#define FLASH_DFTPCLKTESTSTAT_CLOCKCNT_MAXIMUM                      0x0000FFF0U
#define FLASH_DFTPCLKTESTSTAT_CLOCKCNT_MINIMUM                      0x00000000U

// Field:     [0] BUSY
//
// Indicates that a pump clock measurement is in progress.
// ENUMs:
// INPROGRESS               Indicates test in progress
// COMPLETE                 Indicates test complete
#define FLASH_DFTPCLKTESTSTAT_BUSY                                  0x00000001U
#define FLASH_DFTPCLKTESTSTAT_BUSY_M                                0x00000001U
#define FLASH_DFTPCLKTESTSTAT_BUSY_S                                         0U
#define FLASH_DFTPCLKTESTSTAT_BUSY_INPROGRESS                       0x00000001U
#define FLASH_DFTPCLKTESTSTAT_BUSY_COMPLETE                         0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_DFTDATARED0
//
//*****************************************************************************
// Field:   [3:0] VAL
//
// Data for redundant bits
#define FLASH_DFTDATARED0_VAL_W                                              4U
#define FLASH_DFTDATARED0_VAL_M                                     0x0000000FU
#define FLASH_DFTDATARED0_VAL_S                                              0U

//*****************************************************************************
//
// Register: FLASH_O_DFTPUMPCTL
//
//*****************************************************************************
// Field: [18:16] IREFEVCTL
//
// IREFEV control IREFVRD, REFTC, IREFCONST, IREFCCOR blocks in IREFEV
#define FLASH_DFTPUMPCTL_IREFEVCTL_W                                         3U
#define FLASH_DFTPUMPCTL_IREFEVCTL_M                                0x00070000U
#define FLASH_DFTPUMPCTL_IREFEVCTL_S                                        16U

// Field: [15:12] CONFIGPMP
//
// Pump configuration control. LP, HP operation
#define FLASH_DFTPUMPCTL_CONFIGPMP_W                                         4U
#define FLASH_DFTPUMPCTL_CONFIGPMP_M                                0x0000F000U
#define FLASH_DFTPUMPCTL_CONFIGPMP_S                                        12U

// Field:     [9] SSEN
//
// Dither control for oscillator
// Enumeration:
// 0: Disable Dither
// 1: Enable Dither
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define FLASH_DFTPUMPCTL_SSEN                                       0x00000200U
#define FLASH_DFTPUMPCTL_SSEN_M                                     0x00000200U
#define FLASH_DFTPUMPCTL_SSEN_S                                              9U
#define FLASH_DFTPUMPCTL_SSEN_ENABLE                                0x00000200U
#define FLASH_DFTPUMPCTL_SSEN_DISABLE                               0x00000000U

// Field:     [8] PUMPCLKEN
//
// Allows direct control of the pump oscillator which is used to generate
// pumpclk.
// Normally, enable/disable of pumpclk is under flash wrapper state machine
// control.  This bit allows system to enable the clock independently.
// ENUMs:
// ENABLE                   Force pump clock oscillator to be enabled.
// HWCTL                    Allow pump clock oscillator to be controlled by
//                          hardware.
#define FLASH_DFTPUMPCTL_PUMPCLKEN                                  0x00000100U
#define FLASH_DFTPUMPCTL_PUMPCLKEN_M                                0x00000100U
#define FLASH_DFTPUMPCTL_PUMPCLKEN_S                                         8U
#define FLASH_DFTPUMPCTL_PUMPCLKEN_ENABLE                           0x00000100U
#define FLASH_DFTPUMPCTL_PUMPCLKEN_HWCTL                            0x00000000U

// Field:   [6:0] TCR
//
// TCR test mode to be applied to the pump
// ENUMs:
// MAXIMUM                  Maximum value
// MINIMUM                  Minimum value
#define FLASH_DFTPUMPCTL_TCR_W                                               7U
#define FLASH_DFTPUMPCTL_TCR_M                                      0x0000007FU
#define FLASH_DFTPUMPCTL_TCR_S                                               0U
#define FLASH_DFTPUMPCTL_TCR_MAXIMUM                                0x0000007FU
#define FLASH_DFTPUMPCTL_TCR_MINIMUM                                0x00000000U

//*****************************************************************************
//
// Register: FLASH_O_DFTBANKCTL
//
//*****************************************************************************
// Field:     [8] TEZ
//
// When set, TEZ is asserted to the flash banks.  Which banks get the asserted
// signal is determined by the BANKSELECT field in CMDCTL.
// 0x0 Do no assert TEZ
// 0x1 Assert TEZ
// ENUMs:
// NEGATE                   Do not assert TEZ
// ASSERT                   Assert TEZ
#define FLASH_DFTBANKCTL_TEZ                                        0x00000100U
#define FLASH_DFTBANKCTL_TEZ_M                                      0x00000100U
#define FLASH_DFTBANKCTL_TEZ_S                                               8U
#define FLASH_DFTBANKCTL_TEZ_NEGATE                                 0x00000100U
#define FLASH_DFTBANKCTL_TEZ_ASSERT                                 0x00000000U

// Field:   [6:0] TCR
//
// TCR test mode to be applied to the bank
// ENUMs:
// MAXIMUM                  Maximum value
// MINIMUM                  Minimum value
#define FLASH_DFTBANKCTL_TCR_W                                               7U
#define FLASH_DFTBANKCTL_TCR_M                                      0x0000007FU
#define FLASH_DFTBANKCTL_TCR_S                                               0U
#define FLASH_DFTBANKCTL_TCR_MAXIMUM                                0x0000007FU
#define FLASH_DFTBANKCTL_TCR_MINIMUM                                0x00000000U


#endif // __FLASH__
