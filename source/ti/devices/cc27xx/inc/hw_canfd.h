/******************************************************************************
*  Filename:       hw_canfd_h
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

#ifndef __HW_CANFD_H__
#define __HW_CANFD_H__

//*****************************************************************************
//
// This section defines the register offsets of
// CANFD component
//
//*****************************************************************************
// MCAN Core Release Register
#define CANFD_O_MCAN_CREL                                           0x00000000U

// MCAN Endian Register
#define CANFD_O_MCAN_ENDN                                           0x00000004U

// MCAN Data Bit Timing and Prescaler Register
#define CANFD_O_MCAN_DBTP                                           0x0000000CU

// MCAN Test Register
#define CANFD_O_MCAN_TEST                                           0x00000010U

// MCAN RAM Watchdog
#define CANFD_O_MCAN_RWD                                            0x00000014U

// MCAN CC Control Register
#define CANFD_O_MCAN_CCCR                                           0x00000018U

// MCAN Nominal Bit Timing and Prescaler Register
#define CANFD_O_MCAN_NBTP                                           0x0000001CU

// MCAN Timestamp Counter Configuration
#define CANFD_O_MCAN_TSCC                                           0x00000020U

// MCAN Timestamp Counter Value
#define CANFD_O_MCAN_TSCV                                           0x00000024U

// MCAN Timeout Counter Configuration
#define CANFD_O_MCAN_TOCC                                           0x00000028U

// MCAN Timeout Counter Value
#define CANFD_O_MCAN_TOCV                                           0x0000002CU

// MCAN Error Counter Register
#define CANFD_O_MCAN_ECR                                            0x00000040U

// MCAN Protocol Status Register
#define CANFD_O_MCAN_PSR                                            0x00000044U

// MCAN Transmitter Delay Compensation Register
#define CANFD_O_MCAN_TDCR                                           0x00000048U

// MCAN Interrupt Register
#define CANFD_O_MCAN_IR                                             0x00000050U

// MCAN Interrupt Enable
#define CANFD_O_MCAN_IE                                             0x00000054U

// MCAN Interrupt Line Select
#define CANFD_O_MCAN_ILS                                            0x00000058U

// MCAN Interrupt Line Enable
#define CANFD_O_MCAN_ILE                                            0x0000005CU

// MCAN Global Filter Configuration
#define CANFD_O_MCAN_GFC                                            0x00000080U

// MCAN Standard ID Filter Configuration
#define CANFD_O_MCAN_SIDFC                                          0x00000084U

// MCAN Extended ID Filter Configuration
#define CANFD_O_MCAN_XIDFC                                          0x00000088U

// MCAN Extended ID and Mask
#define CANFD_O_MCAN_XIDAM                                          0x00000090U

// MCAN High Priority Message Status
#define CANFD_O_MCAN_HPMS                                           0x00000094U

// MCAN New Data 1
#define CANFD_O_MCAN_NDAT1                                          0x00000098U

// MCAN New Data 2
#define CANFD_O_MCAN_NDAT2                                          0x0000009CU

// MCAN Rx FIFO 0 Configuration
#define CANFD_O_MCAN_RXF0C                                          0x000000A0U

// MCAN Rx FIFO 0 Status
#define CANFD_O_MCAN_RXF0S                                          0x000000A4U

// MCAN Rx FIFO 0 Acknowledge
#define CANFD_O_MCAN_RXF0A                                          0x000000A8U

// MCAN Rx Buffer Configuration
#define CANFD_O_MCAN_RXBC                                           0x000000ACU

// MCAN Rx FIFO 1 Configuration
#define CANFD_O_MCAN_RXF1C                                          0x000000B0U

// MCAN Rx FIFO 1 Status
#define CANFD_O_MCAN_RXF1S                                          0x000000B4U

// MCAN Rx FIFO 1 Acknowledge
#define CANFD_O_MCAN_RXF1A                                          0x000000B8U

// MCAN Rx Buffer / FIFO Element Size Configuration
#define CANFD_O_MCAN_RXESC                                          0x000000BCU

// MCAN Tx Buffer Configuration
#define CANFD_O_MCAN_TXBC                                           0x000000C0U

// MCAN Tx FIFO / Queue Status
#define CANFD_O_MCAN_TXFQS                                          0x000000C4U

// MCAN Tx Buffer Element Size Configuration
#define CANFD_O_MCAN_TXESC                                          0x000000C8U

// MCAN Tx Buffer Request Pending
#define CANFD_O_MCAN_TXBRP                                          0x000000CCU

// MCAN Tx Buffer Add Request
#define CANFD_O_MCAN_TXBAR                                          0x000000D0U

// MCAN Tx Buffer Cancellation Request
#define CANFD_O_MCAN_TXBCR                                          0x000000D4U

// MCAN Tx Buffer Transmission Occurred
#define CANFD_O_MCAN_TXBTO                                          0x000000D8U

// MCAN Tx Buffer Cancellation Finished
#define CANFD_O_MCAN_TXBCF                                          0x000000DCU

// MCAN Tx Buffer Transmission Interrupt Enable
#define CANFD_O_MCAN_TXBTIE                                         0x000000E0U

// MCAN Tx Buffer Cancellation Finished Interrupt Enable
#define CANFD_O_MCAN_TXBCIE                                         0x000000E4U

// MCAN Tx Event FIFO Configuration
#define CANFD_O_MCAN_TXEFC                                          0x000000F0U

// MCAN Tx Event FIFO Status
#define CANFD_O_MCAN_TXEFS                                          0x000000F4U

// MCAN Tx Event FIFO Acknowledge
#define CANFD_O_MCAN_TXEFA                                          0x000000F8U

// MCAN Subsystem Revision Register
#define CANFD_O_MCANSS_PID                                          0x00000200U

// MCAN Subsystem Control Register
#define CANFD_O_MCANSS_CTRL                                         0x00000204U

// MCAN Subsystem Status Register
#define CANFD_O_MCANSS_STAT                                         0x00000208U

// MCAN Subsystem Interrupt Clear Shadow Register
#define CANFD_O_MCANSS_ICS                                          0x0000020CU

// MCAN Subsystem Interrupt Raw Satus Register
#define CANFD_O_MCANSS_IRS                                          0x00000210U

// MCAN Subsystem Interrupt Enable Clear Shadow Register
#define CANFD_O_MCANSS_IECS                                         0x00000214U

// MCAN Subsystem Interrupt Enable Register
#define CANFD_O_MCANSS_IE                                           0x00000218U

// MCAN Subsystem Masked Interrupt Status
#define CANFD_O_MCANSS_IES                                          0x0000021CU

// MCAN Subsystem End of Interrupt
#define CANFD_O_MCANSS_EOI                                          0x00000220U

// MCAN Subsystem External Timestamp Prescaler 0
#define CANFD_O_MCANSS_EXT_TS_PRESCALER                             0x00000224U

// MCAN Subsystem External Timestamp Unserviced Interrupts Counter
#define CANFD_O_MCANSS_EXT_TS_UNSERVICED_INTR_CNTR                  0x00000228U

// MCAN Error Aggregator Revision Register
#define CANFD_O_MCANERR_REV                                         0x00000400U

// MCAN ECC Vector Register
#define CANFD_O_MCANERR_VECTOR                                      0x00000408U

// MCAN Error Misc Status
#define CANFD_O_MCANERR_STAT                                        0x0000040CU

// MCAN ECC Wrapper Revision Register
#define CANFD_O_MCANERR_WRAP_REV                                    0x00000410U

// MCAN ECC Control
#define CANFD_O_MCANERR_CTRL                                        0x00000414U

// MCAN ECC Error Control 1 Register
#define CANFD_O_MCANERR_ERR_CTRL1                                   0x00000418U

// MCAN ECC Error Control 2 Register
#define CANFD_O_MCANERR_ERR_CTRL2                                   0x0000041CU

// MCAN ECC Error Status 1 Register
#define CANFD_O_MCANERR_ERR_STAT1                                   0x00000420U

// MCAN ECC Error Status 2 Register
#define CANFD_O_MCANERR_ERR_STAT2                                   0x00000424U

// MCAN ECC Error Status 3 Register
#define CANFD_O_MCANERR_ERR_STAT3                                   0x00000428U

// MCAN Single Error Corrected End of Interrupt Register
#define CANFD_O_MCANERR_SEC_EOI                                     0x0000043CU

// MCAN Single Error Corrected Interrupt Status Register
#define CANFD_O_MCANERR_SEC_STATUS                                  0x00000440U

// MCAN Single Error Corrected Interrupt Enable Set Register
#define CANFD_O_MCANERR_SEC_ENABLE_SET                              0x00000480U

// MCAN Single Error Corrected Interrupt Enable Clear Register
#define CANFD_O_MCANERR_SEC_ENABLE_CLR                              0x000004C0U

// MCAN Double Error Detected End of Interrupt Register
#define CANFD_O_MCANERR_DED_EOI                                     0x0000053CU

// MCAN Double Error Detected Interrupt Status Register
#define CANFD_O_MCANERR_DED_STATUS                                  0x00000540U

// MCAN Double Error Detected Interrupt Enable Set Register
#define CANFD_O_MCANERR_DED_ENABLE_SET                              0x00000580U

// MCAN Double Error Detected Interrupt Enable Clear Register
#define CANFD_O_MCANERR_DED_ENABLE_CLR                              0x000005C0U

// MCAN Error Aggregator Enable Set Register
#define CANFD_O_MCANERR_AGGR_ENABLE_SET                             0x00000600U

// MCAN Error Aggregator Enable Clear Register
#define CANFD_O_MCANERR_AGGR_ENABLE_CLR                             0x00000604U

// MCAN Error Aggregator Status Set Register
#define CANFD_O_MCANERR_AGGR_STATUS_SET                             0x00000608U

// MCAN Error Aggregator Status Clear Register
#define CANFD_O_MCANERR_AGGR_STATUS_CLR                             0x0000060CU

// Description
#define CANFD_O_DESC                                                0x00000800U

// Interrupt mask
#define CANFD_O_IMASK0                                              0x00000844U

// Raw interrupt status
#define CANFD_O_RIS0                                                0x00000848U

// Masked interrupt status
#define CANFD_O_MIS0                                                0x0000084CU

// Interrupt set
#define CANFD_O_ISET0                                               0x00000850U

// Interrupt clear
#define CANFD_O_ICLR0                                               0x00000854U

// Digital Test Bus
#define CANFD_O_DTB                                                 0x00000864U

// Interrupt mask
#define CANFD_O_IMASK1                                              0x00000868U

// Raw interrupt status
#define CANFD_O_RIS1                                                0x0000086CU

// Masked interrupt status
#define CANFD_O_MIS1                                                0x00000870U

// Interrupt set
#define CANFD_O_ISET1                                               0x00000874U

// Interrupt clear
#define CANFD_O_ICLR1                                               0x00000878U

// Clock divider
#define CANFD_O_MCANSS_CLKDIV                                       0x00000904U

// MCAN-SS clock stop control register
#define CANFD_O_MCANSS_CLKCTL                                       0x00000908U

// MCANSS clock stop status register
#define CANFD_O_MCANSS_CLKSTS                                       0x0000090CU

// MCANSS Fixed DMA0 Control Register
#define CANFD_O_MCANSS_DMA0_CTL                                     0x00000924U

// MCANSS Fixed DMA1 Control Register
#define CANFD_O_MCANSS_DMA1_CTL                                     0x0000092CU

// Rx buffer [x] base address - most significant word
#define CANFD_O_RXDMA_TTO_FE0_BA                                    0x00000938U

// Rx buffer [x+1] base address - most significant word
#define CANFD_O_RXDMA_TTO_FE1_BA                                    0x00000948U

// Rx Buffer two-to-one DMA mode, hardware NDAT1 value register.
#define CANFD_O_RXDMA_TTO_NDAT1                                     0x00000950U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_CREL
//
//*****************************************************************************
// Field: [31:28] REL
//
// Core Release. One digit, BCD-coded.
#define CANFD_MCAN_CREL_REL_W                                                4U
#define CANFD_MCAN_CREL_REL_M                                       0xF0000000U
#define CANFD_MCAN_CREL_REL_S                                               28U

// Field: [27:24] STEP
//
// Step of Core Release. One digit, BCD-coded.
#define CANFD_MCAN_CREL_STEP_W                                               4U
#define CANFD_MCAN_CREL_STEP_M                                      0x0F000000U
#define CANFD_MCAN_CREL_STEP_S                                              24U

// Field: [23:20] SUBSTEP
//
// Sub-Step of Core Release. One digit, BCD-coded.
#define CANFD_MCAN_CREL_SUBSTEP_W                                            4U
#define CANFD_MCAN_CREL_SUBSTEP_M                                   0x00F00000U
#define CANFD_MCAN_CREL_SUBSTEP_S                                           20U

// Field: [19:16] YEAR
//
// Time Stamp Year. One digit, BCD-coded.
#define CANFD_MCAN_CREL_YEAR_W                                               4U
#define CANFD_MCAN_CREL_YEAR_M                                      0x000F0000U
#define CANFD_MCAN_CREL_YEAR_S                                              16U

// Field:  [15:8] MON
//
// Time Stamp Month. Two digits, BCD-coded.
#define CANFD_MCAN_CREL_MON_W                                                8U
#define CANFD_MCAN_CREL_MON_M                                       0x0000FF00U
#define CANFD_MCAN_CREL_MON_S                                                8U

// Field:   [7:0] DAY
//
// Time Stamp Day. Two digits, BCD-coded.
#define CANFD_MCAN_CREL_DAY_W                                                8U
#define CANFD_MCAN_CREL_DAY_M                                       0x000000FFU
#define CANFD_MCAN_CREL_DAY_S                                                0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_ENDN
//
//*****************************************************************************
// Field:  [31:0] ETV
//
// Endianess Test Value. Reading the constant value maintained in this register
// allows software to determine the endianess of the host CPU.
#define CANFD_MCAN_ENDN_ETV_W                                               32U
#define CANFD_MCAN_ENDN_ETV_M                                       0xFFFFFFFFU
#define CANFD_MCAN_ENDN_ETV_S                                                0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_DBTP
//
//*****************************************************************************
// Field:    [23] TDC
//
// Transmitter Delay Compensation
//   0  Transmitter Delay Compensation disabled
//   1  Transmitter Delay Compensation enabled
#define CANFD_MCAN_DBTP_TDC                                         0x00800000U
#define CANFD_MCAN_DBTP_TDC_M                                       0x00800000U
#define CANFD_MCAN_DBTP_TDC_S                                               23U

// Field: [20:16] DBRP
//
// Data Bit Rate Prescaler. The value by which the oscillator frequency is
// divided for generating the bit time quanta. The bit time is built up from a
// multiple of this quanta. Valid values for the Bit Rate Prescaler are 0 to
// 31. The actual interpretation by the hardware of this value is such that one
// more than the value programmed here is used.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_DBTP_DBRP_W                                               5U
#define CANFD_MCAN_DBTP_DBRP_M                                      0x001F0000U
#define CANFD_MCAN_DBTP_DBRP_S                                              16U

// Field:  [12:8] DTSEG1
//
// Data Time Segment Before Sample Point. Valid values are 0 to 31. The actual
// interpretation by the hardware of this value is such that one more than the
// programmed value is used.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_DBTP_DTSEG1_W                                             5U
#define CANFD_MCAN_DBTP_DTSEG1_M                                    0x00001F00U
#define CANFD_MCAN_DBTP_DTSEG1_S                                             8U

// Field:   [7:4] DTSEG2
//
// Data Time Segment After Sample Point. Valid values are 0 to 15. The actual
// interpretation by the hardware of this value is such that one more than the
// programmed value is used.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_DBTP_DTSEG2_W                                             4U
#define CANFD_MCAN_DBTP_DTSEG2_M                                    0x000000F0U
#define CANFD_MCAN_DBTP_DTSEG2_S                                             4U

// Field:   [3:0] DSJW
//
// Data Resynchronization Jump Width. Valid values are 0 to 15. The actual
// interpretation by the hardware of this value is such that one more than the
// value programmed here is used.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_DBTP_DSJW_W                                               4U
#define CANFD_MCAN_DBTP_DSJW_M                                      0x0000000FU
#define CANFD_MCAN_DBTP_DSJW_S                                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TEST
//
//*****************************************************************************
// Field:     [7] RX
//
// Receive Pin. Monitors the actual value of the CAN receive pin.
//   0  The CAN bus is dominant (CAN RX pin = '0')
//   1  The CAN bus is recessive (CAN RX pin = '1')
#define CANFD_MCAN_TEST_RX                                          0x00000080U
#define CANFD_MCAN_TEST_RX_M                                        0x00000080U
#define CANFD_MCAN_TEST_RX_S                                                 7U

// Field:   [6:5] TX
//
// Control of Transmit Pin
//   00  CAN TX pin controlled by the CAN Core, updated at the end of the CAN
// bit time
//   01  Sample Point can be monitored at CAN TX pin
//   10  Dominant ('0') level at CAN TX pin
//   11  Recessive ('1') at CAN TX pin
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_TEST_TX_W                                                 2U
#define CANFD_MCAN_TEST_TX_M                                        0x00000060U
#define CANFD_MCAN_TEST_TX_S                                                 5U

// Field:     [4] LBCK
//
// Loop Back Mode
//   0  Reset value, Loop Back Mode is disabled
//   1  Loop Back Mode is enabled
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_TEST_LBCK                                        0x00000010U
#define CANFD_MCAN_TEST_LBCK_M                                      0x00000010U
#define CANFD_MCAN_TEST_LBCK_S                                               4U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_RWD
//
//*****************************************************************************
// Field:  [15:8] WDV
//
// Watchdog Value. Acutal Message RAM Watchdog Counter Value.
//
// The RAM Watchdog monitors the READY output of the Message RAM. A Message RAM
// access via the MCAN's Generic Master Interface starts the Message RAM
// Watchdog Counter with the value configured by the WDC field. The counter is
// reloaded with WDC when the Message RAM signals successful completion by
// activating its READY output. In case there is no response from the Message
// RAM until the counter has counted down to zero, the counter stops and
// interrupt flag MCAN_IR.WDI is set. The RAM Watchdog Counter is clocked by
// the host (system) clock.
#define CANFD_MCAN_RWD_WDV_W                                                 8U
#define CANFD_MCAN_RWD_WDV_M                                        0x0000FF00U
#define CANFD_MCAN_RWD_WDV_S                                                 8U

// Field:   [7:0] WDC
//
// Watchdog Configuration. Start value of the Message RAM Watchdog Counter.
// With the reset value of "00" the counter is disabled.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_RWD_WDC_W                                                 8U
#define CANFD_MCAN_RWD_WDC_M                                        0x000000FFU
#define CANFD_MCAN_RWD_WDC_S                                                 0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_CCCR
//
//*****************************************************************************
// Field:    [15] NISO
//
// Non ISO Operation. If this bit is set, the MCAN uses the CAN FD frame format
// as specified by the Bosch CAN FD Specification V1.0.
//   0  CAN FD frame format according to ISO 11898-1:2015
//   1  CAN FD frame format according to Bosch CAN FD Specification V1.0
#define CANFD_MCAN_CCCR_NISO                                        0x00008000U
#define CANFD_MCAN_CCCR_NISO_M                                      0x00008000U
#define CANFD_MCAN_CCCR_NISO_S                                              15U

// Field:    [14] TXP
//
// Transmit Pause. If this bit is set, the MCAN pauses for two CAN bit times
// before starting the next transmission after itself has successfully
// transmitted a frame.
//   0  Transmit pause disabled
//   1  Transmit pause enabled
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_CCCR_TXP                                         0x00004000U
#define CANFD_MCAN_CCCR_TXP_M                                       0x00004000U
#define CANFD_MCAN_CCCR_TXP_S                                               14U

// Field:    [13] EFBI
//
// Edge Filtering during Bus Integration
//   0  Edge filtering disabled
//   1  Two consecutive dominant tq required to detect an edge for hard
// synchronization
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_CCCR_EFBI                                        0x00002000U
#define CANFD_MCAN_CCCR_EFBI_M                                      0x00002000U
#define CANFD_MCAN_CCCR_EFBI_S                                              13U

// Field:    [12] PXHD
//
// Protocol Exception Handling Disable
//   0  Protocol exception handling enabled
//   1  Protocol exception handling disabled
// Note: When protocol exception handling is disabled, the MCAN will transmit
// an error frame when it detects a protocol exception condition.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_CCCR_PXHD                                        0x00001000U
#define CANFD_MCAN_CCCR_PXHD_M                                      0x00001000U
#define CANFD_MCAN_CCCR_PXHD_S                                              12U

// Field:     [9] BRSE
//
// Bit Rate Switch Enable
//   0  Bit rate switching for transmissions disabled
//   1  Bit rate switching for transmissions enabled
// Note: When CAN FD operation is disabled FDOE = '0', BRSE is not evaluated.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_CCCR_BRSE                                        0x00000200U
#define CANFD_MCAN_CCCR_BRSE_M                                      0x00000200U
#define CANFD_MCAN_CCCR_BRSE_S                                               9U

// Field:     [8] FDOE
//
// Flexible Datarate Operation Enable
//   0  FD operation disabled
//   1  FD operation enabled
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_CCCR_FDOE                                        0x00000100U
#define CANFD_MCAN_CCCR_FDOE_M                                      0x00000100U
#define CANFD_MCAN_CCCR_FDOE_S                                               8U

// Field:     [7] TEST
//
// Test Mode Enable
//   0  Normal operation, register TEST holds reset values
//   1  Test Mode, write access to register TEST enabled
//
// Qualified Write 1 to Set is possible only with CCCR.CCE='1' and
// CCCR.INIT='1'.
#define CANFD_MCAN_CCCR_TEST                                        0x00000080U
#define CANFD_MCAN_CCCR_TEST_M                                      0x00000080U
#define CANFD_MCAN_CCCR_TEST_S                                               7U

// Field:     [6] DAR
//
// Disable Automatic Retransmission
//   0  Automatic retransmission of messages not transmitted successfully
// enabled
//   1  Automatic retransmission disabled
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_CCCR_DAR                                         0x00000040U
#define CANFD_MCAN_CCCR_DAR_M                                       0x00000040U
#define CANFD_MCAN_CCCR_DAR_S                                                6U

// Field:     [5] MON
//
// Bus Monitoring Mode. Bit MON can only be set by SW when both CCE and INIT
// are set to '1'. The bit can be reset by SW at any time.
//   0  Bus Monitoring Mode is disabled
//   1  Bus Monitoring Mode is enabled
//
// Qualified Write 1 to Set is possible only with CCCR.CCE='1' and
// CCCR.INIT='1'.
#define CANFD_MCAN_CCCR_MON                                         0x00000020U
#define CANFD_MCAN_CCCR_MON_M                                       0x00000020U
#define CANFD_MCAN_CCCR_MON_S                                                5U

// Field:     [4] CSR
//
// Clock Stop Request
//   0  No clock stop is requested
//   1  Clock stop requested. When clock stop is requested, first INIT and then
// CSA will be set after all pending transfer requests have been completed and
// the CAN bus reached idle.
#define CANFD_MCAN_CCCR_CSR                                         0x00000010U
#define CANFD_MCAN_CCCR_CSR_M                                       0x00000010U
#define CANFD_MCAN_CCCR_CSR_S                                                4U

// Field:     [3] CSA
//
// Clock Stop Acknowledge
//   0  No clock stop acknowledged
//   1  MCAN may be set in power down by stopping the Host and CAN clocks
#define CANFD_MCAN_CCCR_CSA                                         0x00000008U
#define CANFD_MCAN_CCCR_CSA_M                                       0x00000008U
#define CANFD_MCAN_CCCR_CSA_S                                                3U

// Field:     [2] ASM
//
// Restricted Operation Mode. Bit ASM can only be set by SW when both CCE and
// INIT are set to '1'. The bit can be reset by SW at any time.
//   0  Normal CAN operation
//   1  Restricted Operation Mode active
//
// Qualified Write 1 to Set is possible only with CCCR.CCE='1' and
// CCCR.INIT='1'.
#define CANFD_MCAN_CCCR_ASM                                         0x00000004U
#define CANFD_MCAN_CCCR_ASM_M                                       0x00000004U
#define CANFD_MCAN_CCCR_ASM_S                                                2U

// Field:     [1] CCE
//
// Configuration Change Enable
//   0  The CPU has no write access to the protected configuration registers
//   1  The CPU has write access to the protected configuration registers
// (while CCCR.INIT = '1')
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_CCCR_CCE                                         0x00000002U
#define CANFD_MCAN_CCCR_CCE_M                                       0x00000002U
#define CANFD_MCAN_CCCR_CCE_S                                                1U

// Field:     [0] INIT
//
// Initialization
//   0  Normal Operation
//   1  Initialization is started
// Note: Due to the synchronization mechanism between the two clock domains,
// there may be a delay until the value written to INIT can be read back.
// Therefore the programmer has to assure that the previous value written to
// INIT has been accepted by reading INIT before setting INIT to a new value.
#define CANFD_MCAN_CCCR_INIT                                        0x00000001U
#define CANFD_MCAN_CCCR_INIT_M                                      0x00000001U
#define CANFD_MCAN_CCCR_INIT_S                                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_NBTP
//
//*****************************************************************************
// Field: [31:25] NSJW
//
// Nominal (Re)Synchronization Jump Width. Valid values are 0 to 127. The
// actual interpretation by the hardware of this value is such that one more
// than the value programmed here is used.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_NBTP_NSJW_W                                               7U
#define CANFD_MCAN_NBTP_NSJW_M                                      0xFE000000U
#define CANFD_MCAN_NBTP_NSJW_S                                              25U

// Field: [24:16] NBRP
//
// Nominal Bit Rate Prescaler. The value by which the oscillator frequency is
// divided for generating the bit time quanta. The bit time is built up from a
// multiple of this quanta. Valid values for the Bit Rate Prescaler are 0 to
// 511. The actual interpretation by the hardware of this value is such that
// one more than the value programmed here is used.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_NBTP_NBRP_W                                               9U
#define CANFD_MCAN_NBTP_NBRP_M                                      0x01FF0000U
#define CANFD_MCAN_NBTP_NBRP_S                                              16U

// Field:  [15:8] NTSEG1
//
// Nominal Time Segment Before Sample Point. Valid values are 1 to 255. The
// actual interpretation by the hardware of this value is such that one more
// than the programmed value is used.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_NBTP_NTSEG1_W                                             8U
#define CANFD_MCAN_NBTP_NTSEG1_M                                    0x0000FF00U
#define CANFD_MCAN_NBTP_NTSEG1_S                                             8U

// Field:   [6:0] NTSEG2
//
// Nominal Time Segment After Sample Point. Valid values are 1 to 127. The
// actual interpretation by the hardware of this value is such that one more
// than the programmed value is used.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_NBTP_NTSEG2_W                                             7U
#define CANFD_MCAN_NBTP_NTSEG2_M                                    0x0000007FU
#define CANFD_MCAN_NBTP_NTSEG2_S                                             0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TSCC
//
//*****************************************************************************
// Field: [19:16] TCP
//
// Timestamp Counter Prescaler. Configures the timestamp and timeout counters
// time unit in multiples of CAN bit times. Valid values are 0 to 15. The
// actual interpretation by the hardware of this value is such that one more
// than the value programmed here is used.
//
// Note: With CAN FD an external counter is required for timestamp generation
// (TSS = "10").
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_TSCC_TCP_W                                                4U
#define CANFD_MCAN_TSCC_TCP_M                                       0x000F0000U
#define CANFD_MCAN_TSCC_TCP_S                                               16U

// Field:   [1:0] TSS
//
// Timestamp Select
//   00  Timestamp counter value always 0x0000
//   01  Timestamp counter value incremented according to TCP
//   10  External timestamp counter value used
//   11  Same as "00"
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_TSCC_TSS_W                                                2U
#define CANFD_MCAN_TSCC_TSS_M                                       0x00000003U
#define CANFD_MCAN_TSCC_TSS_S                                                0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TSCV
//
//*****************************************************************************
// Field:  [15:0] TSC
//
// Timestamp Counter. The internal/external Timestamp Counter value is captured
// on start of frame (both Rx and Tx). When TSCC.TSS = "01", the Timestamp
// Counter is incremented in multiples of CAN bit times, (1...16), depending on
// the configuration of TSCC.TCP. A wrap around sets interrupt flag IR.TSW.
// Write access resets the counter to zero. When TSCC.TSS = "10", TSC reflects
// the External Timestamp Counter value, and a write access has no impact.
//
// Note: A "wrap around" is a change of the Timestamp Counter value from
// non-zero to zero not
// caused by write access to MCAN_TSCV.
#define CANFD_MCAN_TSCV_TSC_W                                               16U
#define CANFD_MCAN_TSCV_TSC_M                                       0x0000FFFFU
#define CANFD_MCAN_TSCV_TSC_S                                                0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TOCC
//
//*****************************************************************************
// Field: [31:16] TOP
//
// Timeout Period. Start value of the Timeout Counter (down-counter).
// Configures the Timeout Period.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_TOCC_TOP_W                                               16U
#define CANFD_MCAN_TOCC_TOP_M                                       0xFFFF0000U
#define CANFD_MCAN_TOCC_TOP_S                                               16U

// Field:   [2:1] TOS
//
// Timeout Select. When operating in Continuous mode, a write to TOCV presets
// the counter to the value configured by TOCC.TOP and continues down-counting.
// When the Timeout Counter is controlled by one of the FIFOs, an empty FIFO
// presets the counter to the value configured by TOCC.TOP. Down-counting is
// started when the first FIFO element is stored.
//   00  Continuous operation
//   01  Timeout controlled by Tx Event FIFO
//   10  Timeout controlled by Rx FIFO 0
//   11  Timeout controlled by Rx FIFO 1
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_TOCC_TOS_W                                                2U
#define CANFD_MCAN_TOCC_TOS_M                                       0x00000006U
#define CANFD_MCAN_TOCC_TOS_S                                                1U

// Field:     [0] ETOC
//
// Enable Timeout Counter
//   0  Timeout Counter disabled
//   1  Timeout Counter enabled
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_TOCC_ETOC                                        0x00000001U
#define CANFD_MCAN_TOCC_ETOC_M                                      0x00000001U
#define CANFD_MCAN_TOCC_ETOC_S                                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TOCV
//
//*****************************************************************************
// Field:  [15:0] TOC
//
// Timeout Counter. The Timeout Counter is decremented in multiples of CAN bit
// times, (1...16), depending on the configuration of TSCC.TCP. When
// decremented to zero, interrupt flag IR.TOO is set and the Timeout Counter is
// stopped. Start and reset/restart conditions are configured via TOCC.TOS.
#define CANFD_MCAN_TOCV_TOC_W                                               16U
#define CANFD_MCAN_TOCV_TOC_M                                       0x0000FFFFU
#define CANFD_MCAN_TOCV_TOC_S                                                0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_ECR
//
//*****************************************************************************
// Field: [23:16] CEL
//
// CAN Error Logging. The counter is incremented each time when a CAN protocol
// error causes the Transmit Error Counter or the Receive Error Counter to be
// incremented. It is reset by read access to CEL. The counter stops at 0xFF;
// the next increment of TEC or REC sets interrupt flag IR.ELO.
//
// Note: When CCCR.ASM is set, the CAN protocol controller does not increment
// TEC and REC when a CAN protocol error is detected, but CEL is still
// incremented.
#define CANFD_MCAN_ECR_CEL_W                                                 8U
#define CANFD_MCAN_ECR_CEL_M                                        0x00FF0000U
#define CANFD_MCAN_ECR_CEL_S                                                16U

// Field:    [15] RP
//
// Receive Error Passive
//   0  The Receive Error Counter is below the error passive level of 128
//   1  The Receive Error Counter has reached the error passive level of 128
#define CANFD_MCAN_ECR_RP                                           0x00008000U
#define CANFD_MCAN_ECR_RP_M                                         0x00008000U
#define CANFD_MCAN_ECR_RP_S                                                 15U

// Field:  [14:8] REC
//
// Receive Error Counter. Actual state of the Receive Error Counter, values
// between 0 and 127.
//
// Note: When CCCR.ASM is set, the CAN protocol controller does not increment
// TEC and REC when a CAN protocol error is detected, but CEL is still
// incremented.
#define CANFD_MCAN_ECR_REC_W                                                 7U
#define CANFD_MCAN_ECR_REC_M                                        0x00007F00U
#define CANFD_MCAN_ECR_REC_S                                                 8U

// Field:   [7:0] TEC
//
// Transmit Error Counter. Actual state of the Transmit Error Counter, values
// between 0 and 255.
//
// Note: When CCCR.ASM is set, the CAN protocol controller does not increment
// TEC and REC when a CAN protocol error is detected, but CEL is still
// incremented.
#define CANFD_MCAN_ECR_TEC_W                                                 8U
#define CANFD_MCAN_ECR_TEC_M                                        0x000000FFU
#define CANFD_MCAN_ECR_TEC_S                                                 0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_PSR
//
//*****************************************************************************
// Field: [22:16] TDCV
//
// Transmitter Delay Compensation Value. Position of the secondary sample
// point, defined by the sum of the measured delay from the internal CAN TX
// signal to the internal CAN RX signal and TDCR.TDCO. The SSP position is, in
// the data phase, the number of mtq between the start of the transmitted bit
// and the secondary sample point. Valid values are 0 to 127 mtq.
#define CANFD_MCAN_PSR_TDCV_W                                                7U
#define CANFD_MCAN_PSR_TDCV_M                                       0x007F0000U
#define CANFD_MCAN_PSR_TDCV_S                                               16U

// Field:    [14] PXE
//
// Protocol Exception Event
//   0  No protocol exception event occurred since last read access
//   1  Protocol exception event occurred
#define CANFD_MCAN_PSR_PXE                                          0x00004000U
#define CANFD_MCAN_PSR_PXE_M                                        0x00004000U
#define CANFD_MCAN_PSR_PXE_S                                                14U

// Field:    [13] RFDF
//
// Received a CAN FD Message.  This bit is set independent of acceptance
// filtering.
//   0  Since this bit was reset by the CPU, no CAN FD message has been
// received
//   1  Message in CAN FD format with FDF flag set has been received
#define CANFD_MCAN_PSR_RFDF                                         0x00002000U
#define CANFD_MCAN_PSR_RFDF_M                                       0x00002000U
#define CANFD_MCAN_PSR_RFDF_S                                               13U

// Field:    [12] RBRS
//
// BRS Flag of Last Received CAN FD Message. This bit is set together with
// RFDF, independent of acceptance filtering.
//   0  Last received CAN FD message did not have its BRS flag set
//   1  Last received CAN FD message had its BRS flag set
#define CANFD_MCAN_PSR_RBRS                                         0x00001000U
#define CANFD_MCAN_PSR_RBRS_M                                       0x00001000U
#define CANFD_MCAN_PSR_RBRS_S                                               12U

// Field:    [11] RESI
//
// ESI Flag of Last Received CAN FD Message. This bit is set together with
// RFDF, independent of acceptance filtering.
//   0  Last received CAN FD message did not have its ESI flag set
//   1  Last received CAN FD message had its ESI flag set
#define CANFD_MCAN_PSR_RESI                                         0x00000800U
#define CANFD_MCAN_PSR_RESI_M                                       0x00000800U
#define CANFD_MCAN_PSR_RESI_S                                               11U

// Field:  [10:8] DLEC
//
// Data Phase Last Error Code. Type of last error that occurred in the data
// phase of a CAN FD format frame with its BRS flag set. Coding is the same as
// for LEC. This field will be cleared to zero when a CAN FD format frame with
// its BRS flag set has been transferred (reception or transmission) without
// error.
#define CANFD_MCAN_PSR_DLEC_W                                                3U
#define CANFD_MCAN_PSR_DLEC_M                                       0x00000700U
#define CANFD_MCAN_PSR_DLEC_S                                                8U

// Field:     [7] BO
//
// Bus_Off Status
//   0  The M_CAN is not Bus_Off
//   1  The M_CAN is in Bus_Off state
#define CANFD_MCAN_PSR_BO                                           0x00000080U
#define CANFD_MCAN_PSR_BO_M                                         0x00000080U
#define CANFD_MCAN_PSR_BO_S                                                  7U

// Field:     [6] EW
//
// Warning Status
//   0  Both error counters are below the Error_Warning limit of 96
//   1  At least one of error counter has reached the Error_Warning limit of 96
#define CANFD_MCAN_PSR_EW                                           0x00000040U
#define CANFD_MCAN_PSR_EW_M                                         0x00000040U
#define CANFD_MCAN_PSR_EW_S                                                  6U

// Field:     [5] EP
//
// Error Passive
//   0  The M_CAN is in the Error_Active state. It normally takes part in bus
// communication and sends an active error flag when an error has been detected
//   1  The M_CAN is in the Error_Passive state
#define CANFD_MCAN_PSR_EP                                           0x00000020U
#define CANFD_MCAN_PSR_EP_M                                         0x00000020U
#define CANFD_MCAN_PSR_EP_S                                                  5U

// Field:   [4:3] ACT
//
// Node Activity.  Monitors the module's CAN communication state.
//   00  Synchronizing - node is synchronizing on CAN communication
//   01  Idle - node is neither receiver nor transmitter
//   10  Receiver - node is operating as receiver
//   11  Transmitter - node is operating as transmitter
//
// Note: ACT is set to "00" by a Protocol Exception Event.
#define CANFD_MCAN_PSR_ACT_W                                                 2U
#define CANFD_MCAN_PSR_ACT_M                                        0x00000018U
#define CANFD_MCAN_PSR_ACT_S                                                 3U

// Field:   [2:0] LEC
//
// Last Error Code. The LEC indicates the type of the last error to occur on
// the CAN bus. This field will be cleared to '0' when a message has been
// transferred (reception or transmission) without error.
//   0  No Error: No error occurred since LEC has been reset by successful
// reception or transmission.
//   1  Stuff Error: More than 5 equal bits in a sequence have occurred in a
// part of a received message where this is not allowed.
//   2  Form Error: A fixed format part of a received frame has the wrong
// format.
//   3  AckError: The message transmitted by the MCAN was not acknowledged by
// another node.
//   4  Bit1Error: During the transmission of a message (with the exception of
// the arbitration field), the device wanted to send a recessive level (bit of
// logical value '1'), but the monitored bus value was dominant.
//   5  Bit0Error: During the transmission of a message (or acknowledge bit, or
// active error flag, or overload flag), the device wanted to send a dominant
// level (data or identifier bit logical value '0'), but the monitored bus
// value was recessive. During Bus_Off recovery this status is set each time a
// sequence of 11 recessive bits has been monitored. This enables the CPU to
// monitor the proceeding of the Bus_Off recovery sequence (indicating the bus
// is not stuck at dominant or continuously disturbed).
//   6  CRCError: The CRC check sum of a received message was incorrect. The
// CRC of an incoming message does not match with the CRC calculated from the
// received data.
//   7  NoChange: Any read access to the Protocol Status Register
// re-initializes the LEC to '7'. When the LEC shows the value '7', no CAN bus
// event was detected since the last CPU read access to the Protocol Status
// Register.
//
// Note: When a frame in CAN FD format has reached the data phase with BRS flag
// set, the next CAN event (error or valid frame) will be shown in DLEC instead
// of LEC. An error in a fixed stuff bit of a CAN FD CRC sequence will be shown
// as a Form Error, not Stuff Error. Note: The Bus_Off recovery sequence (see
// ISO 11898-1:2015) cannot be shortened by setting or resetting CCCR.INIT. If
// the device goes Bus_Off, it will set CCCR.INIT of its own accord, stopping
// all bus activities. Once CCCR.INIT has been cleared by the CPU, the device
// will then wait for 129 occurrences of Bus Idle (129 * 11 consecutive
// recessive bits) before resuming normal operation. At the end of the Bus_Off
// recovery sequence, the Error Management Counters will be reset. During the
// waiting time after the resetting of CCCR.INIT, each time a sequence of 11
// recessive bits has been monitored, a Bit0Error code is written to PSR.LEC,
// enabling the CPU to readily check up whether the CAN bus is stuck at
// dominant or continuously disturbed and to monitor the Bus_Off recovery
// sequence. ECR.REC is used to count these sequences.
#define CANFD_MCAN_PSR_LEC_W                                                 3U
#define CANFD_MCAN_PSR_LEC_M                                        0x00000007U
#define CANFD_MCAN_PSR_LEC_S                                                 0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TDCR
//
//*****************************************************************************
// Field:  [14:8] TDCO
//
// Transmitter Delay Compensation Offset. Offset value defining the distance
// between the measured delay from the internal CAN TX signal to the internal
// CAN RX signal and the secondary sample point. Valid values are 0 to 127 mtq.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_TDCR_TDCO_W                                               7U
#define CANFD_MCAN_TDCR_TDCO_M                                      0x00007F00U
#define CANFD_MCAN_TDCR_TDCO_S                                               8U

// Field:   [6:0] TDCF
//
// Transmitter Delay Compensation Filter Window Length. Defines the minimum
// value for the SSP position, dominant edges on the internal CAN RX signal
// that would result in an earlier SSP position are ignored for transmitter
// delay measurement. The feature is enabled when TDCF is configured to a value
// greater than TDCO. Valid values are 0 to 127 mtq.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_TDCR_TDCF_W                                               7U
#define CANFD_MCAN_TDCR_TDCF_M                                      0x0000007FU
#define CANFD_MCAN_TDCR_TDCF_S                                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_IR
//
//*****************************************************************************
// Field:    [29] ARA
//
// Access to Reserved Address
//   0  No access to reserved address occurred
//   1  Access to reserved address occurred
#define CANFD_MCAN_IR_ARA                                           0x20000000U
#define CANFD_MCAN_IR_ARA_M                                         0x20000000U
#define CANFD_MCAN_IR_ARA_S                                                 29U

// Field:    [28] PED
//
// Protocol Error in Data Phase (Data Bit Time is used)
//   0  No protocol error in data phase
//   1  Protocol error in data phase detected (PSR.DLEC ? 0,7)
#define CANFD_MCAN_IR_PED                                           0x10000000U
#define CANFD_MCAN_IR_PED_M                                         0x10000000U
#define CANFD_MCAN_IR_PED_S                                                 28U

// Field:    [27] PEA
//
// Protocol Error in Arbitration Phase (Nominal Bit Time is used)
//   0  No protocol error in arbitration phase
//   1  Protocol error in arbitration phase detected (PSR.LEC ? 0,7)
#define CANFD_MCAN_IR_PEA                                           0x08000000U
#define CANFD_MCAN_IR_PEA_M                                         0x08000000U
#define CANFD_MCAN_IR_PEA_S                                                 27U

// Field:    [26] WDI
//
// Watchdog Interrupt
//   0  No Message RAM Watchdog event occurred
//   1  Message RAM Watchdog event due to missing READY
#define CANFD_MCAN_IR_WDI                                           0x04000000U
#define CANFD_MCAN_IR_WDI_M                                         0x04000000U
#define CANFD_MCAN_IR_WDI_S                                                 26U

// Field:    [25] BO
//
// Bus_Off Status
//   0  Bus_Off status unchanged
//   1  Bus_Off status changed
#define CANFD_MCAN_IR_BO                                            0x02000000U
#define CANFD_MCAN_IR_BO_M                                          0x02000000U
#define CANFD_MCAN_IR_BO_S                                                  25U

// Field:    [24] EW
//
// Warning Status
//   0  Error_Warning status unchanged
//   1  Error_Warning status changed
#define CANFD_MCAN_IR_EW                                            0x01000000U
#define CANFD_MCAN_IR_EW_M                                          0x01000000U
#define CANFD_MCAN_IR_EW_S                                                  24U

// Field:    [23] EP
//
// Error Passive
//   0  Error_Passive status unchanged
//   1  Error_Passive status changed
#define CANFD_MCAN_IR_EP                                            0x00800000U
#define CANFD_MCAN_IR_EP_M                                          0x00800000U
#define CANFD_MCAN_IR_EP_S                                                  23U

// Field:    [22] ELO
//
// Error Logging Overflow
//   0  CAN Error Logging Counter did not overflow
//   1  Overflow of CAN Error Logging Counter occurred
#define CANFD_MCAN_IR_ELO                                           0x00400000U
#define CANFD_MCAN_IR_ELO_M                                         0x00400000U
#define CANFD_MCAN_IR_ELO_S                                                 22U

// Field:    [21] BEU
//
// Bit Error Uncorrected. Message RAM bit error detected, uncorrected. This bit
// is set when a double bit error is detected by the ECC aggregator attached to
// the Message RAM. An uncorrected Message RAM bit error sets CCCR.INIT to '1'.
// This is done to avoid transmission of corrupted data.
//   0  No bit error detected when reading from Message RAM
//   1  Bit error detected, uncorrected (e.g. parity logic)
#define CANFD_MCAN_IR_BEU                                           0x00200000U
#define CANFD_MCAN_IR_BEU_M                                         0x00200000U
#define CANFD_MCAN_IR_BEU_S                                                 21U

// Field:    [19] DRX
//
// Message Stored to Dedicated Rx Buffer. The flag is set whenever a received
// message has been stored into a dedicated Rx Buffer.
//   0  No Rx Buffer updated
//   1  At least one received message stored into an Rx Buffer
#define CANFD_MCAN_IR_DRX                                           0x00080000U
#define CANFD_MCAN_IR_DRX_M                                         0x00080000U
#define CANFD_MCAN_IR_DRX_S                                                 19U

// Field:    [18] TOO
//
// Timeout Occurred
//   0  No timeout
//   1  Timeout reached
#define CANFD_MCAN_IR_TOO                                           0x00040000U
#define CANFD_MCAN_IR_TOO_M                                         0x00040000U
#define CANFD_MCAN_IR_TOO_S                                                 18U

// Field:    [17] MRAF
//
// Message RAM Access Failure.  The flag is set, when the Rx Handler:
//   - has not completed acceptance filtering or storage of an accepted message
// until the arbitration field of the following message has been received. In
// this case acceptance filtering or message storage is aborted and the Rx
// Handler starts processing of the following message.
//   - was not able to write a message to the Message RAM. In this case message
// storage is aborted.
//
// In both cases the FIFO put index is not updated resp. the New Data flag for
// a dedicated Rx Buffer is not set, a partly stored message is overwritten
// when the next message is stored to this location.
//
// The flag is also set when the Tx Handler was not able to read a message from
// the Message RAM in time. In this case message transmission is aborted. In
// case of a Tx Handler access failure the MCAN is switched into Restricted
// Operation Mode. To leave Restricted Operation Mode, the Host CPU has to
// reset CCCR.ASM.
//   0  No Message RAM access failure occurred
//   1  Message RAM access failure occurred
#define CANFD_MCAN_IR_MRAF                                          0x00020000U
#define CANFD_MCAN_IR_MRAF_M                                        0x00020000U
#define CANFD_MCAN_IR_MRAF_S                                                17U

// Field:    [16] TSW
//
// Timestamp Wraparound
//   0  No timestamp counter wrap-around
//   1  Timestamp counter wrapped around
#define CANFD_MCAN_IR_TSW                                           0x00010000U
#define CANFD_MCAN_IR_TSW_M                                         0x00010000U
#define CANFD_MCAN_IR_TSW_S                                                 16U

// Field:    [15] TEFL
//
// Tx Event FIFO Element Lost
//   0  No Tx Event FIFO element lost
//   1  Tx Event FIFO element lost, also set after write attempt to Tx Event
// FIFO of size zero
#define CANFD_MCAN_IR_TEFL                                          0x00008000U
#define CANFD_MCAN_IR_TEFL_M                                        0x00008000U
#define CANFD_MCAN_IR_TEFL_S                                                15U

// Field:    [14] TEFF
//
// Tx Event FIFO Full
//   0  Tx Event FIFO not full
//   1  Tx Event FIFO full
#define CANFD_MCAN_IR_TEFF                                          0x00004000U
#define CANFD_MCAN_IR_TEFF_M                                        0x00004000U
#define CANFD_MCAN_IR_TEFF_S                                                14U

// Field:    [13] TEFW
//
// Tx Event FIFO Watermark Reached
//   0  Tx Event FIFO fill level below watermark
//   1  Tx Event FIFO fill level reached watermark
#define CANFD_MCAN_IR_TEFW                                          0x00002000U
#define CANFD_MCAN_IR_TEFW_M                                        0x00002000U
#define CANFD_MCAN_IR_TEFW_S                                                13U

// Field:    [12] TEFN
//
// Tx Event FIFO New Entry
//   0  Tx Event FIFO unchanged
//   1  Tx Handler wrote Tx Event FIFO element
#define CANFD_MCAN_IR_TEFN                                          0x00001000U
#define CANFD_MCAN_IR_TEFN_M                                        0x00001000U
#define CANFD_MCAN_IR_TEFN_S                                                12U

// Field:    [11] TFE
//
// Tx FIFO Empty
//   0  Tx FIFO non-empty
//   1  Tx FIFO empty
#define CANFD_MCAN_IR_TFE                                           0x00000800U
#define CANFD_MCAN_IR_TFE_M                                         0x00000800U
#define CANFD_MCAN_IR_TFE_S                                                 11U

// Field:    [10] TCF
//
// Transmission Cancellation Finished
//   0  No transmission cancellation finished
//   1  Transmission cancellation finished
#define CANFD_MCAN_IR_TCF                                           0x00000400U
#define CANFD_MCAN_IR_TCF_M                                         0x00000400U
#define CANFD_MCAN_IR_TCF_S                                                 10U

// Field:     [9] TC
//
// Transmission Completed
//   0  No transmission completed
//   1  Transmission completed
#define CANFD_MCAN_IR_TC                                            0x00000200U
#define CANFD_MCAN_IR_TC_M                                          0x00000200U
#define CANFD_MCAN_IR_TC_S                                                   9U

// Field:     [8] HPM
//
// High Priority Message
//   0  No high priority message received
//   1  High priority message received
#define CANFD_MCAN_IR_HPM                                           0x00000100U
#define CANFD_MCAN_IR_HPM_M                                         0x00000100U
#define CANFD_MCAN_IR_HPM_S                                                  8U

// Field:     [7] RF1L
//
// Rx FIFO 1 Message Lost
//   0  No Rx FIFO 1 message lost
//   1  Rx FIFO 1 message lost, also set after write attempt to Rx FIFO 1 of
// size zero
#define CANFD_MCAN_IR_RF1L                                          0x00000080U
#define CANFD_MCAN_IR_RF1L_M                                        0x00000080U
#define CANFD_MCAN_IR_RF1L_S                                                 7U

// Field:     [6] RF1F
//
// Rx FIFO 1 Full
//   0  Rx FIFO 1 not full
//   1  Rx FIFO 1 full
#define CANFD_MCAN_IR_RF1F                                          0x00000040U
#define CANFD_MCAN_IR_RF1F_M                                        0x00000040U
#define CANFD_MCAN_IR_RF1F_S                                                 6U

// Field:     [5] RF1W
//
// Rx FIFO 1 Watermark Reached
//   0  Rx FIFO 1 fill level below watermark
//   1  Rx FIFO 1 fill level reached watermark
#define CANFD_MCAN_IR_RF1W                                          0x00000020U
#define CANFD_MCAN_IR_RF1W_M                                        0x00000020U
#define CANFD_MCAN_IR_RF1W_S                                                 5U

// Field:     [4] RF1N
//
// Rx FIFO 1 New Message
//   0  No new message written to Rx FIFO 1
//   1  New message written to Rx FIFO 1
#define CANFD_MCAN_IR_RF1N                                          0x00000010U
#define CANFD_MCAN_IR_RF1N_M                                        0x00000010U
#define CANFD_MCAN_IR_RF1N_S                                                 4U

// Field:     [3] RF0L
//
// Rx FIFO 0 Message Lost
//   0  No Rx FIFO 0 message lost
//   1  Rx FIFO 0 message lost, also set after write attempt to Rx FIFO 0 of
// size zero
#define CANFD_MCAN_IR_RF0L                                          0x00000008U
#define CANFD_MCAN_IR_RF0L_M                                        0x00000008U
#define CANFD_MCAN_IR_RF0L_S                                                 3U

// Field:     [2] RF0F
//
// Rx FIFO 0 Full
//   0  Rx FIFO 0 not full
//   1  Rx FIFO 0 full
#define CANFD_MCAN_IR_RF0F                                          0x00000004U
#define CANFD_MCAN_IR_RF0F_M                                        0x00000004U
#define CANFD_MCAN_IR_RF0F_S                                                 2U

// Field:     [1] RF0W
//
// Rx FIFO 0 Watermark Reached
//   0  Rx FIFO 0 fill level below watermark
//   1  Rx FIFO 0 fill level reached watermark
#define CANFD_MCAN_IR_RF0W                                          0x00000002U
#define CANFD_MCAN_IR_RF0W_M                                        0x00000002U
#define CANFD_MCAN_IR_RF0W_S                                                 1U

// Field:     [0] RF0N
//
// Rx FIFO 0 New Message
//   0  No new message written to Rx FIFO 0
//   1  New message written to Rx FIFO 0
#define CANFD_MCAN_IR_RF0N                                          0x00000001U
#define CANFD_MCAN_IR_RF0N_M                                        0x00000001U
#define CANFD_MCAN_IR_RF0N_S                                                 0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_IE
//
//*****************************************************************************
// Field:    [29] ARAE
//
// Access to Reserved Address Enable
#define CANFD_MCAN_IE_ARAE                                          0x20000000U
#define CANFD_MCAN_IE_ARAE_M                                        0x20000000U
#define CANFD_MCAN_IE_ARAE_S                                                29U

// Field:    [28] PEDE
//
// Protocol Error in Data Phase Enable
#define CANFD_MCAN_IE_PEDE                                          0x10000000U
#define CANFD_MCAN_IE_PEDE_M                                        0x10000000U
#define CANFD_MCAN_IE_PEDE_S                                                28U

// Field:    [27] PEAE
//
// Protocol Error in Arbitration Phase Enable
#define CANFD_MCAN_IE_PEAE                                          0x08000000U
#define CANFD_MCAN_IE_PEAE_M                                        0x08000000U
#define CANFD_MCAN_IE_PEAE_S                                                27U

// Field:    [26] WDIE
//
// Watchdog Interrupt Enable
#define CANFD_MCAN_IE_WDIE                                          0x04000000U
#define CANFD_MCAN_IE_WDIE_M                                        0x04000000U
#define CANFD_MCAN_IE_WDIE_S                                                26U

// Field:    [25] BOE
//
// Bus_Off Status Enable
#define CANFD_MCAN_IE_BOE                                           0x02000000U
#define CANFD_MCAN_IE_BOE_M                                         0x02000000U
#define CANFD_MCAN_IE_BOE_S                                                 25U

// Field:    [24] EWE
//
// Warning Status Enable
#define CANFD_MCAN_IE_EWE                                           0x01000000U
#define CANFD_MCAN_IE_EWE_M                                         0x01000000U
#define CANFD_MCAN_IE_EWE_S                                                 24U

// Field:    [23] EPE
//
// Error Passive Enable
#define CANFD_MCAN_IE_EPE                                           0x00800000U
#define CANFD_MCAN_IE_EPE_M                                         0x00800000U
#define CANFD_MCAN_IE_EPE_S                                                 23U

// Field:    [22] ELOE
//
// Error Logging Overflow Enable
#define CANFD_MCAN_IE_ELOE                                          0x00400000U
#define CANFD_MCAN_IE_ELOE_M                                        0x00400000U
#define CANFD_MCAN_IE_ELOE_S                                                22U

// Field:    [21] BEUE
//
// Bit Error Uncorrected Enable
#define CANFD_MCAN_IE_BEUE                                          0x00200000U
#define CANFD_MCAN_IE_BEUE_M                                        0x00200000U
#define CANFD_MCAN_IE_BEUE_S                                                21U

// Field:    [19] DRXE
//
// Message Stored to Dedicated Rx Buffer Enable
#define CANFD_MCAN_IE_DRXE                                          0x00080000U
#define CANFD_MCAN_IE_DRXE_M                                        0x00080000U
#define CANFD_MCAN_IE_DRXE_S                                                19U

// Field:    [18] TOOE
//
// Timeout Occurred Enable
#define CANFD_MCAN_IE_TOOE                                          0x00040000U
#define CANFD_MCAN_IE_TOOE_M                                        0x00040000U
#define CANFD_MCAN_IE_TOOE_S                                                18U

// Field:    [17] MRAFE
//
// Message RAM Access Failure Enable
#define CANFD_MCAN_IE_MRAFE                                         0x00020000U
#define CANFD_MCAN_IE_MRAFE_M                                       0x00020000U
#define CANFD_MCAN_IE_MRAFE_S                                               17U

// Field:    [16] TSWE
//
// Timestamp Wraparound Enable
#define CANFD_MCAN_IE_TSWE                                          0x00010000U
#define CANFD_MCAN_IE_TSWE_M                                        0x00010000U
#define CANFD_MCAN_IE_TSWE_S                                                16U

// Field:    [15] TEFLE
//
// Tx Event FIFO Element Lost Enable
#define CANFD_MCAN_IE_TEFLE                                         0x00008000U
#define CANFD_MCAN_IE_TEFLE_M                                       0x00008000U
#define CANFD_MCAN_IE_TEFLE_S                                               15U

// Field:    [14] TEFFE
//
// Tx Event FIFO Full Enable
#define CANFD_MCAN_IE_TEFFE                                         0x00004000U
#define CANFD_MCAN_IE_TEFFE_M                                       0x00004000U
#define CANFD_MCAN_IE_TEFFE_S                                               14U

// Field:    [13] TEFWE
//
// Tx Event FIFO Watermark Reached Enable
#define CANFD_MCAN_IE_TEFWE                                         0x00002000U
#define CANFD_MCAN_IE_TEFWE_M                                       0x00002000U
#define CANFD_MCAN_IE_TEFWE_S                                               13U

// Field:    [12] TEFNE
//
// Tx Event FIFO New Entry Enable
#define CANFD_MCAN_IE_TEFNE                                         0x00001000U
#define CANFD_MCAN_IE_TEFNE_M                                       0x00001000U
#define CANFD_MCAN_IE_TEFNE_S                                               12U

// Field:    [11] TFEE
//
// Tx FIFO Empty Enable
#define CANFD_MCAN_IE_TFEE                                          0x00000800U
#define CANFD_MCAN_IE_TFEE_M                                        0x00000800U
#define CANFD_MCAN_IE_TFEE_S                                                11U

// Field:    [10] TCFE
//
// Transmission Cancellation Finished Enable
#define CANFD_MCAN_IE_TCFE                                          0x00000400U
#define CANFD_MCAN_IE_TCFE_M                                        0x00000400U
#define CANFD_MCAN_IE_TCFE_S                                                10U

// Field:     [9] TCE
//
// Transmission Completed Enable
#define CANFD_MCAN_IE_TCE                                           0x00000200U
#define CANFD_MCAN_IE_TCE_M                                         0x00000200U
#define CANFD_MCAN_IE_TCE_S                                                  9U

// Field:     [8] HPME
//
// High Priority Message Enable
#define CANFD_MCAN_IE_HPME                                          0x00000100U
#define CANFD_MCAN_IE_HPME_M                                        0x00000100U
#define CANFD_MCAN_IE_HPME_S                                                 8U

// Field:     [7] RF1LE
//
// Rx FIFO 1 Message Lost Enable
#define CANFD_MCAN_IE_RF1LE                                         0x00000080U
#define CANFD_MCAN_IE_RF1LE_M                                       0x00000080U
#define CANFD_MCAN_IE_RF1LE_S                                                7U

// Field:     [6] RF1FE
//
// Rx FIFO 1 Full Enable
#define CANFD_MCAN_IE_RF1FE                                         0x00000040U
#define CANFD_MCAN_IE_RF1FE_M                                       0x00000040U
#define CANFD_MCAN_IE_RF1FE_S                                                6U

// Field:     [5] RF1WE
//
// Rx FIFO 1 Watermark Reached Enable
#define CANFD_MCAN_IE_RF1WE                                         0x00000020U
#define CANFD_MCAN_IE_RF1WE_M                                       0x00000020U
#define CANFD_MCAN_IE_RF1WE_S                                                5U

// Field:     [4] RF1NE
//
// Rx FIFO 1 New Message Enable
#define CANFD_MCAN_IE_RF1NE                                         0x00000010U
#define CANFD_MCAN_IE_RF1NE_M                                       0x00000010U
#define CANFD_MCAN_IE_RF1NE_S                                                4U

// Field:     [3] RF0LE
//
// Rx FIFO 0 Message Lost Enable
#define CANFD_MCAN_IE_RF0LE                                         0x00000008U
#define CANFD_MCAN_IE_RF0LE_M                                       0x00000008U
#define CANFD_MCAN_IE_RF0LE_S                                                3U

// Field:     [2] RF0FE
//
// Rx FIFO 0 Full Enable
#define CANFD_MCAN_IE_RF0FE                                         0x00000004U
#define CANFD_MCAN_IE_RF0FE_M                                       0x00000004U
#define CANFD_MCAN_IE_RF0FE_S                                                2U

// Field:     [1] RF0WE
//
// Rx FIFO 0 Watermark Reached Enable
#define CANFD_MCAN_IE_RF0WE                                         0x00000002U
#define CANFD_MCAN_IE_RF0WE_M                                       0x00000002U
#define CANFD_MCAN_IE_RF0WE_S                                                1U

// Field:     [0] RF0NE
//
// Rx FIFO 0 New Message Enable
#define CANFD_MCAN_IE_RF0NE                                         0x00000001U
#define CANFD_MCAN_IE_RF0NE_M                                       0x00000001U
#define CANFD_MCAN_IE_RF0NE_S                                                0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_ILS
//
//*****************************************************************************
// Field:    [29] ARAL
//
// Access to Reserved Address Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_ARAL                                         0x20000000U
#define CANFD_MCAN_ILS_ARAL_M                                       0x20000000U
#define CANFD_MCAN_ILS_ARAL_S                                               29U

// Field:    [28] PEDL
//
// Protocol Error in Data Phase Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_PEDL                                         0x10000000U
#define CANFD_MCAN_ILS_PEDL_M                                       0x10000000U
#define CANFD_MCAN_ILS_PEDL_S                                               28U

// Field:    [27] PEAL
//
// Protocol Error in Arbitration Phase Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_PEAL                                         0x08000000U
#define CANFD_MCAN_ILS_PEAL_M                                       0x08000000U
#define CANFD_MCAN_ILS_PEAL_S                                               27U

// Field:    [26] WDIL
//
// Watchdog Interrupt Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_WDIL                                         0x04000000U
#define CANFD_MCAN_ILS_WDIL_M                                       0x04000000U
#define CANFD_MCAN_ILS_WDIL_S                                               26U

// Field:    [25] BOL
//
// Bus_Off Status Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_BOL                                          0x02000000U
#define CANFD_MCAN_ILS_BOL_M                                        0x02000000U
#define CANFD_MCAN_ILS_BOL_S                                                25U

// Field:    [24] EWL
//
// Warning Status Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_EWL                                          0x01000000U
#define CANFD_MCAN_ILS_EWL_M                                        0x01000000U
#define CANFD_MCAN_ILS_EWL_S                                                24U

// Field:    [23] EPL
//
// Error Passive Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_EPL                                          0x00800000U
#define CANFD_MCAN_ILS_EPL_M                                        0x00800000U
#define CANFD_MCAN_ILS_EPL_S                                                23U

// Field:    [22] ELOL
//
// Error Logging Overflow Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_ELOL                                         0x00400000U
#define CANFD_MCAN_ILS_ELOL_M                                       0x00400000U
#define CANFD_MCAN_ILS_ELOL_S                                               22U

// Field:    [21] BEUL
//
// Bit Error Uncorrected Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_BEUL                                         0x00200000U
#define CANFD_MCAN_ILS_BEUL_M                                       0x00200000U
#define CANFD_MCAN_ILS_BEUL_S                                               21U

// Field:    [19] DRXL
//
// Message Stored to Dedicated Rx Buffer Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_DRXL                                         0x00080000U
#define CANFD_MCAN_ILS_DRXL_M                                       0x00080000U
#define CANFD_MCAN_ILS_DRXL_S                                               19U

// Field:    [18] TOOL
//
// Timeout Occurred Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_TOOL                                         0x00040000U
#define CANFD_MCAN_ILS_TOOL_M                                       0x00040000U
#define CANFD_MCAN_ILS_TOOL_S                                               18U

// Field:    [17] MRAFL
//
// Message RAM Access Failure Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_MRAFL                                        0x00020000U
#define CANFD_MCAN_ILS_MRAFL_M                                      0x00020000U
#define CANFD_MCAN_ILS_MRAFL_S                                              17U

// Field:    [16] TSWL
//
// Timestamp Wraparound Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_TSWL                                         0x00010000U
#define CANFD_MCAN_ILS_TSWL_M                                       0x00010000U
#define CANFD_MCAN_ILS_TSWL_S                                               16U

// Field:    [15] TEFLL
//
// Tx Event FIFO Element Lost Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_TEFLL                                        0x00008000U
#define CANFD_MCAN_ILS_TEFLL_M                                      0x00008000U
#define CANFD_MCAN_ILS_TEFLL_S                                              15U

// Field:    [14] TEFFL
//
// Tx Event FIFO Full Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_TEFFL                                        0x00004000U
#define CANFD_MCAN_ILS_TEFFL_M                                      0x00004000U
#define CANFD_MCAN_ILS_TEFFL_S                                              14U

// Field:    [13] TEFWL
//
// Tx Event FIFO Watermark Reached Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_TEFWL                                        0x00002000U
#define CANFD_MCAN_ILS_TEFWL_M                                      0x00002000U
#define CANFD_MCAN_ILS_TEFWL_S                                              13U

// Field:    [12] TEFNL
//
// Tx Event FIFO New Entry Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_TEFNL                                        0x00001000U
#define CANFD_MCAN_ILS_TEFNL_M                                      0x00001000U
#define CANFD_MCAN_ILS_TEFNL_S                                              12U

// Field:    [11] TFEL
//
// Tx FIFO Empty Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_TFEL                                         0x00000800U
#define CANFD_MCAN_ILS_TFEL_M                                       0x00000800U
#define CANFD_MCAN_ILS_TFEL_S                                               11U

// Field:    [10] TCFL
//
// Transmission Cancellation Finished Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_TCFL                                         0x00000400U
#define CANFD_MCAN_ILS_TCFL_M                                       0x00000400U
#define CANFD_MCAN_ILS_TCFL_S                                               10U

// Field:     [9] TCL
//
// Transmission Completed Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_TCL                                          0x00000200U
#define CANFD_MCAN_ILS_TCL_M                                        0x00000200U
#define CANFD_MCAN_ILS_TCL_S                                                 9U

// Field:     [8] HPML
//
// High Priority Message Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_HPML                                         0x00000100U
#define CANFD_MCAN_ILS_HPML_M                                       0x00000100U
#define CANFD_MCAN_ILS_HPML_S                                                8U

// Field:     [7] RF1LL
//
// Rx FIFO 1 Message Lost Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_RF1LL                                        0x00000080U
#define CANFD_MCAN_ILS_RF1LL_M                                      0x00000080U
#define CANFD_MCAN_ILS_RF1LL_S                                               7U

// Field:     [6] RF1FL
//
// Rx FIFO 1 Full Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_RF1FL                                        0x00000040U
#define CANFD_MCAN_ILS_RF1FL_M                                      0x00000040U
#define CANFD_MCAN_ILS_RF1FL_S                                               6U

// Field:     [5] RF1WL
//
// Rx FIFO 1 Watermark Reached Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_RF1WL                                        0x00000020U
#define CANFD_MCAN_ILS_RF1WL_M                                      0x00000020U
#define CANFD_MCAN_ILS_RF1WL_S                                               5U

// Field:     [4] RF1NL
//
// Rx FIFO 1 New Message Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_RF1NL                                        0x00000010U
#define CANFD_MCAN_ILS_RF1NL_M                                      0x00000010U
#define CANFD_MCAN_ILS_RF1NL_S                                               4U

// Field:     [3] RF0LL
//
// Rx FIFO 0 Message Lost Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_RF0LL                                        0x00000008U
#define CANFD_MCAN_ILS_RF0LL_M                                      0x00000008U
#define CANFD_MCAN_ILS_RF0LL_S                                               3U

// Field:     [2] RF0FL
//
// Rx FIFO 0 Full Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_RF0FL                                        0x00000004U
#define CANFD_MCAN_ILS_RF0FL_M                                      0x00000004U
#define CANFD_MCAN_ILS_RF0FL_S                                               2U

// Field:     [1] RF0WL
//
// Rx FIFO 0 Watermark Reached Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_RF0WL                                        0x00000002U
#define CANFD_MCAN_ILS_RF0WL_M                                      0x00000002U
#define CANFD_MCAN_ILS_RF0WL_S                                               1U

// Field:     [0] RF0NL
//
// Rx FIFO 0 New Message Line
//   0  Interrupt source is assigned to Interrupt Line 0
//   1  Interrupt source is assigned to Interrupt Line 1
#define CANFD_MCAN_ILS_RF0NL                                        0x00000001U
#define CANFD_MCAN_ILS_RF0NL_M                                      0x00000001U
#define CANFD_MCAN_ILS_RF0NL_S                                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_ILE
//
//*****************************************************************************
// Field:     [1] EINT1
//
// Enable Interrupt Line 1
//   0  Interrupt Line 1 is disabled
//   1  Interrupt Line 1 is enabled
#define CANFD_MCAN_ILE_EINT1                                        0x00000002U
#define CANFD_MCAN_ILE_EINT1_M                                      0x00000002U
#define CANFD_MCAN_ILE_EINT1_S                                               1U

// Field:     [0] EINT0
//
// Enable Interrupt Line 0
//   0  Interrupt Line 0 is disabled
//   1  Interrupt Line 0 is enabled
#define CANFD_MCAN_ILE_EINT0                                        0x00000001U
#define CANFD_MCAN_ILE_EINT0_M                                      0x00000001U
#define CANFD_MCAN_ILE_EINT0_S                                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_GFC
//
//*****************************************************************************
// Field:   [5:4] ANFS
//
// Accept Non-matching Frames Standard. Defines how received messages with
// 11-bit IDs that do not match any element of the filter list are treated.
//   00  Accept in Rx FIFO 0
//   01  Accept in Rx FIFO 1
//   10  Reject
//   11  Reject
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_GFC_ANFS_W                                                2U
#define CANFD_MCAN_GFC_ANFS_M                                       0x00000030U
#define CANFD_MCAN_GFC_ANFS_S                                                4U

// Field:   [3:2] ANFE
//
// Accept Non-matching Frames Extended. Defines how received messages with
// 29-bit IDs that do not match any element of the filter list are treated.
//   00  Accept in Rx FIFO 0
//   01  Accept in Rx FIFO 1
//   10  Reject
//   11  Reject
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_GFC_ANFE_W                                                2U
#define CANFD_MCAN_GFC_ANFE_M                                       0x0000000CU
#define CANFD_MCAN_GFC_ANFE_S                                                2U

// Field:     [1] RRFS
//
// Reject Remote Frames Standard
//   0  Filter remote frames with 11-bit standard IDs
//   1  Reject all remote frames with 11-bit standard IDs
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_GFC_RRFS                                         0x00000002U
#define CANFD_MCAN_GFC_RRFS_M                                       0x00000002U
#define CANFD_MCAN_GFC_RRFS_S                                                1U

// Field:     [0] RRFE
//
// Reject Remote Frames Extended
//   0  Filter remote frames with 29-bit extended IDs
//   1  Reject all remote frames with 29-bit extended IDs
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_GFC_RRFE                                         0x00000001U
#define CANFD_MCAN_GFC_RRFE_M                                       0x00000001U
#define CANFD_MCAN_GFC_RRFE_S                                                0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_SIDFC
//
//*****************************************************************************
// Field: [23:16] LSS
//
// List Size Standard
//   0        No standard Message ID filter
//   1-128  Number of standard Message ID filter elements
//   >128   Values greater than 128 are interpreted as 128
#define CANFD_MCAN_SIDFC_LSS_W                                               8U
#define CANFD_MCAN_SIDFC_LSS_M                                      0x00FF0000U
#define CANFD_MCAN_SIDFC_LSS_S                                              16U

// Field:  [15:2] FLSSA
//
// Filter List Standard Start Address. Start address of standard Message ID
// filter list (32-bit word address).
#define CANFD_MCAN_SIDFC_FLSSA_W                                            14U
#define CANFD_MCAN_SIDFC_FLSSA_M                                    0x0000FFFCU
#define CANFD_MCAN_SIDFC_FLSSA_S                                             2U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_XIDFC
//
//*****************************************************************************
// Field: [22:16] LSE
//
// List Size Extended
//   0     No extended Message ID filter
//   1-64 Number of extended Message ID filter elements
//   >64  Values greater than 64 are interpreted as 64
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_XIDFC_LSE_W                                               7U
#define CANFD_MCAN_XIDFC_LSE_M                                      0x007F0000U
#define CANFD_MCAN_XIDFC_LSE_S                                              16U

// Field:  [15:2] FLESA
//
// Filter List Extended Start Address. Start address of extended Message ID
// filter list (32-bit word address).
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_XIDFC_FLESA_W                                            14U
#define CANFD_MCAN_XIDFC_FLESA_M                                    0x0000FFFCU
#define CANFD_MCAN_XIDFC_FLESA_S                                             2U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_XIDAM
//
//*****************************************************************************
// Field:  [28:0] EIDM
//
// Extended ID Mask. For acceptance filtering of extended frames the Extended
// ID AND Mask is ANDed with the Message ID of a received frame. Intended for
// masking of 29-bit IDs in SAE J1939. With the reset value of all bits set to
// one the mask is not active.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_XIDAM_EIDM_W                                             29U
#define CANFD_MCAN_XIDAM_EIDM_M                                     0x1FFFFFFFU
#define CANFD_MCAN_XIDAM_EIDM_S                                              0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_HPMS
//
//*****************************************************************************
// Field:    [15] FLST
//
// Filter List. Indicates the filter list of the matching filter element.
//   0  Standard Filter List
//   1  Extended Filter List
#define CANFD_MCAN_HPMS_FLST                                        0x00008000U
#define CANFD_MCAN_HPMS_FLST_M                                      0x00008000U
#define CANFD_MCAN_HPMS_FLST_S                                              15U

// Field:  [14:8] FIDX
//
// Filter Index. Index of matching filter element. Range is 0 to SIDFC.LSS - 1
// resp. XIDFC.LSE - 1.
#define CANFD_MCAN_HPMS_FIDX_W                                               7U
#define CANFD_MCAN_HPMS_FIDX_M                                      0x00007F00U
#define CANFD_MCAN_HPMS_FIDX_S                                               8U

// Field:   [7:6] MSI
//
// Message Storage Indicator
//   00  No FIFO selected
//   01  FIFO message lost
//   10  Message stored in FIFO 0
//   11  Message stored in FIFO 1
#define CANFD_MCAN_HPMS_MSI_W                                                2U
#define CANFD_MCAN_HPMS_MSI_M                                       0x000000C0U
#define CANFD_MCAN_HPMS_MSI_S                                                6U

// Field:   [5:0] BIDX
//
// Buffer Index. Index of Rx FIFO element to which the message was stored. Only
// valid when MSI(1) = '1'.
#define CANFD_MCAN_HPMS_BIDX_W                                               6U
#define CANFD_MCAN_HPMS_BIDX_M                                      0x0000003FU
#define CANFD_MCAN_HPMS_BIDX_S                                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_NDAT1
//
//*****************************************************************************
// Field:    [31] ND31
//
// New Data RX Buffer 31
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND31                                       0x80000000U
#define CANFD_MCAN_NDAT1_ND31_M                                     0x80000000U
#define CANFD_MCAN_NDAT1_ND31_S                                             31U

// Field:    [30] ND30
//
// New Data RX Buffer 30
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND30                                       0x40000000U
#define CANFD_MCAN_NDAT1_ND30_M                                     0x40000000U
#define CANFD_MCAN_NDAT1_ND30_S                                             30U

// Field:    [29] ND29
//
// New Data RX Buffer 29
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND29                                       0x20000000U
#define CANFD_MCAN_NDAT1_ND29_M                                     0x20000000U
#define CANFD_MCAN_NDAT1_ND29_S                                             29U

// Field:    [28] ND28
//
// New Data RX Buffer 28
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND28                                       0x10000000U
#define CANFD_MCAN_NDAT1_ND28_M                                     0x10000000U
#define CANFD_MCAN_NDAT1_ND28_S                                             28U

// Field:    [27] ND27
//
// New Data RX Buffer 27
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND27                                       0x08000000U
#define CANFD_MCAN_NDAT1_ND27_M                                     0x08000000U
#define CANFD_MCAN_NDAT1_ND27_S                                             27U

// Field:    [26] ND26
//
// New Data RX Buffer 26
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND26                                       0x04000000U
#define CANFD_MCAN_NDAT1_ND26_M                                     0x04000000U
#define CANFD_MCAN_NDAT1_ND26_S                                             26U

// Field:    [25] ND25
//
// New Data RX Buffer 25
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND25                                       0x02000000U
#define CANFD_MCAN_NDAT1_ND25_M                                     0x02000000U
#define CANFD_MCAN_NDAT1_ND25_S                                             25U

// Field:    [24] ND24
//
// New Data RX Buffer 24
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND24                                       0x01000000U
#define CANFD_MCAN_NDAT1_ND24_M                                     0x01000000U
#define CANFD_MCAN_NDAT1_ND24_S                                             24U

// Field:    [23] ND23
//
// New Data RX Buffer 23
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND23                                       0x00800000U
#define CANFD_MCAN_NDAT1_ND23_M                                     0x00800000U
#define CANFD_MCAN_NDAT1_ND23_S                                             23U

// Field:    [22] ND22
//
// New Data RX Buffer 22
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND22                                       0x00400000U
#define CANFD_MCAN_NDAT1_ND22_M                                     0x00400000U
#define CANFD_MCAN_NDAT1_ND22_S                                             22U

// Field:    [21] ND21
//
// New Data RX Buffer 21
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND21                                       0x00200000U
#define CANFD_MCAN_NDAT1_ND21_M                                     0x00200000U
#define CANFD_MCAN_NDAT1_ND21_S                                             21U

// Field:    [20] ND20
//
// New Data RX Buffer 20
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND20                                       0x00100000U
#define CANFD_MCAN_NDAT1_ND20_M                                     0x00100000U
#define CANFD_MCAN_NDAT1_ND20_S                                             20U

// Field:    [19] ND19
//
// New Data RX Buffer 19
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND19                                       0x00080000U
#define CANFD_MCAN_NDAT1_ND19_M                                     0x00080000U
#define CANFD_MCAN_NDAT1_ND19_S                                             19U

// Field:    [18] ND18
//
// New Data RX Buffer 18
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND18                                       0x00040000U
#define CANFD_MCAN_NDAT1_ND18_M                                     0x00040000U
#define CANFD_MCAN_NDAT1_ND18_S                                             18U

// Field:    [17] ND17
//
// New Data RX Buffer 17
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND17                                       0x00020000U
#define CANFD_MCAN_NDAT1_ND17_M                                     0x00020000U
#define CANFD_MCAN_NDAT1_ND17_S                                             17U

// Field:    [16] ND16
//
// New Data RX Buffer 16
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND16                                       0x00010000U
#define CANFD_MCAN_NDAT1_ND16_M                                     0x00010000U
#define CANFD_MCAN_NDAT1_ND16_S                                             16U

// Field:    [15] ND15
//
// New Data RX Buffer 15
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND15                                       0x00008000U
#define CANFD_MCAN_NDAT1_ND15_M                                     0x00008000U
#define CANFD_MCAN_NDAT1_ND15_S                                             15U

// Field:    [14] ND14
//
// New Data RX Buffer 14
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND14                                       0x00004000U
#define CANFD_MCAN_NDAT1_ND14_M                                     0x00004000U
#define CANFD_MCAN_NDAT1_ND14_S                                             14U

// Field:    [13] ND13
//
// New Data RX Buffer 13
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND13                                       0x00002000U
#define CANFD_MCAN_NDAT1_ND13_M                                     0x00002000U
#define CANFD_MCAN_NDAT1_ND13_S                                             13U

// Field:    [12] ND12
//
// New Data RX Buffer 12
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND12                                       0x00001000U
#define CANFD_MCAN_NDAT1_ND12_M                                     0x00001000U
#define CANFD_MCAN_NDAT1_ND12_S                                             12U

// Field:    [11] ND11
//
// New Data RX Buffer 11
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND11                                       0x00000800U
#define CANFD_MCAN_NDAT1_ND11_M                                     0x00000800U
#define CANFD_MCAN_NDAT1_ND11_S                                             11U

// Field:    [10] ND10
//
// New Data RX Buffer 10
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND10                                       0x00000400U
#define CANFD_MCAN_NDAT1_ND10_M                                     0x00000400U
#define CANFD_MCAN_NDAT1_ND10_S                                             10U

// Field:     [9] ND9
//
// New Data RX Buffer 9
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND9                                        0x00000200U
#define CANFD_MCAN_NDAT1_ND9_M                                      0x00000200U
#define CANFD_MCAN_NDAT1_ND9_S                                               9U

// Field:     [8] ND8
//
// New Data RX Buffer 8
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND8                                        0x00000100U
#define CANFD_MCAN_NDAT1_ND8_M                                      0x00000100U
#define CANFD_MCAN_NDAT1_ND8_S                                               8U

// Field:     [7] ND7
//
// New Data RX Buffer 7
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND7                                        0x00000080U
#define CANFD_MCAN_NDAT1_ND7_M                                      0x00000080U
#define CANFD_MCAN_NDAT1_ND7_S                                               7U

// Field:     [6] ND6
//
// New Data RX Buffer 6
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND6                                        0x00000040U
#define CANFD_MCAN_NDAT1_ND6_M                                      0x00000040U
#define CANFD_MCAN_NDAT1_ND6_S                                               6U

// Field:     [5] ND5
//
// New Data RX Buffer 5
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND5                                        0x00000020U
#define CANFD_MCAN_NDAT1_ND5_M                                      0x00000020U
#define CANFD_MCAN_NDAT1_ND5_S                                               5U

// Field:     [4] ND4
//
// New Data RX Buffer 4
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND4                                        0x00000010U
#define CANFD_MCAN_NDAT1_ND4_M                                      0x00000010U
#define CANFD_MCAN_NDAT1_ND4_S                                               4U

// Field:     [3] ND3
//
// New Data RX Buffer 3
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND3                                        0x00000008U
#define CANFD_MCAN_NDAT1_ND3_M                                      0x00000008U
#define CANFD_MCAN_NDAT1_ND3_S                                               3U

// Field:     [2] ND2
//
// New Data RX Buffer 2
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND2                                        0x00000004U
#define CANFD_MCAN_NDAT1_ND2_M                                      0x00000004U
#define CANFD_MCAN_NDAT1_ND2_S                                               2U

// Field:     [1] ND1
//
// New Data RX Buffer 1
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND1                                        0x00000002U
#define CANFD_MCAN_NDAT1_ND1_M                                      0x00000002U
#define CANFD_MCAN_NDAT1_ND1_S                                               1U

// Field:     [0] ND0
//
// New Data RX Buffer 0
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT1_ND0                                        0x00000001U
#define CANFD_MCAN_NDAT1_ND0_M                                      0x00000001U
#define CANFD_MCAN_NDAT1_ND0_S                                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_NDAT2
//
//*****************************************************************************
// Field:    [31] ND63
//
// New Data RX Buffer 63
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND63                                       0x80000000U
#define CANFD_MCAN_NDAT2_ND63_M                                     0x80000000U
#define CANFD_MCAN_NDAT2_ND63_S                                             31U

// Field:    [30] ND62
//
// New Data RX Buffer 62
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND62                                       0x40000000U
#define CANFD_MCAN_NDAT2_ND62_M                                     0x40000000U
#define CANFD_MCAN_NDAT2_ND62_S                                             30U

// Field:    [29] ND61
//
// New Data RX Buffer 61
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND61                                       0x20000000U
#define CANFD_MCAN_NDAT2_ND61_M                                     0x20000000U
#define CANFD_MCAN_NDAT2_ND61_S                                             29U

// Field:    [28] ND60
//
// New Data RX Buffer 60
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND60                                       0x10000000U
#define CANFD_MCAN_NDAT2_ND60_M                                     0x10000000U
#define CANFD_MCAN_NDAT2_ND60_S                                             28U

// Field:    [27] ND59
//
// New Data RX Buffer 59
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND59                                       0x08000000U
#define CANFD_MCAN_NDAT2_ND59_M                                     0x08000000U
#define CANFD_MCAN_NDAT2_ND59_S                                             27U

// Field:    [26] ND58
//
// New Data RX Buffer 58
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND58                                       0x04000000U
#define CANFD_MCAN_NDAT2_ND58_M                                     0x04000000U
#define CANFD_MCAN_NDAT2_ND58_S                                             26U

// Field:    [25] ND57
//
// New Data RX Buffer 57
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND57                                       0x02000000U
#define CANFD_MCAN_NDAT2_ND57_M                                     0x02000000U
#define CANFD_MCAN_NDAT2_ND57_S                                             25U

// Field:    [24] ND56
//
// New Data RX Buffer 56
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND56                                       0x01000000U
#define CANFD_MCAN_NDAT2_ND56_M                                     0x01000000U
#define CANFD_MCAN_NDAT2_ND56_S                                             24U

// Field:    [23] ND55
//
// New Data RX Buffer 55
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND55                                       0x00800000U
#define CANFD_MCAN_NDAT2_ND55_M                                     0x00800000U
#define CANFD_MCAN_NDAT2_ND55_S                                             23U

// Field:    [22] ND54
//
// New Data RX Buffer 54
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND54                                       0x00400000U
#define CANFD_MCAN_NDAT2_ND54_M                                     0x00400000U
#define CANFD_MCAN_NDAT2_ND54_S                                             22U

// Field:    [21] ND53
//
// New Data RX Buffer 53
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND53                                       0x00200000U
#define CANFD_MCAN_NDAT2_ND53_M                                     0x00200000U
#define CANFD_MCAN_NDAT2_ND53_S                                             21U

// Field:    [20] ND52
//
// New Data RX Buffer 52
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND52                                       0x00100000U
#define CANFD_MCAN_NDAT2_ND52_M                                     0x00100000U
#define CANFD_MCAN_NDAT2_ND52_S                                             20U

// Field:    [19] ND51
//
// New Data RX Buffer 51
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND51                                       0x00080000U
#define CANFD_MCAN_NDAT2_ND51_M                                     0x00080000U
#define CANFD_MCAN_NDAT2_ND51_S                                             19U

// Field:    [18] ND50
//
// New Data RX Buffer 50
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND50                                       0x00040000U
#define CANFD_MCAN_NDAT2_ND50_M                                     0x00040000U
#define CANFD_MCAN_NDAT2_ND50_S                                             18U

// Field:    [17] ND49
//
// New Data RX Buffer 49
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND49                                       0x00020000U
#define CANFD_MCAN_NDAT2_ND49_M                                     0x00020000U
#define CANFD_MCAN_NDAT2_ND49_S                                             17U

// Field:    [16] ND48
//
// New Data RX Buffer 48
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND48                                       0x00010000U
#define CANFD_MCAN_NDAT2_ND48_M                                     0x00010000U
#define CANFD_MCAN_NDAT2_ND48_S                                             16U

// Field:    [15] ND47
//
// New Data RX Buffer 47
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND47                                       0x00008000U
#define CANFD_MCAN_NDAT2_ND47_M                                     0x00008000U
#define CANFD_MCAN_NDAT2_ND47_S                                             15U

// Field:    [14] ND46
//
// New Data RX Buffer 46
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND46                                       0x00004000U
#define CANFD_MCAN_NDAT2_ND46_M                                     0x00004000U
#define CANFD_MCAN_NDAT2_ND46_S                                             14U

// Field:    [13] ND45
//
// New Data RX Buffer 45
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND45                                       0x00002000U
#define CANFD_MCAN_NDAT2_ND45_M                                     0x00002000U
#define CANFD_MCAN_NDAT2_ND45_S                                             13U

// Field:    [12] ND44
//
// New Data RX Buffer 44
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND44                                       0x00001000U
#define CANFD_MCAN_NDAT2_ND44_M                                     0x00001000U
#define CANFD_MCAN_NDAT2_ND44_S                                             12U

// Field:    [11] ND43
//
// New Data RX Buffer 43
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND43                                       0x00000800U
#define CANFD_MCAN_NDAT2_ND43_M                                     0x00000800U
#define CANFD_MCAN_NDAT2_ND43_S                                             11U

// Field:    [10] ND42
//
// New Data RX Buffer 42
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND42                                       0x00000400U
#define CANFD_MCAN_NDAT2_ND42_M                                     0x00000400U
#define CANFD_MCAN_NDAT2_ND42_S                                             10U

// Field:     [9] ND41
//
// New Data RX Buffer 41
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND41                                       0x00000200U
#define CANFD_MCAN_NDAT2_ND41_M                                     0x00000200U
#define CANFD_MCAN_NDAT2_ND41_S                                              9U

// Field:     [8] ND40
//
// New Data RX Buffer 40
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND40                                       0x00000100U
#define CANFD_MCAN_NDAT2_ND40_M                                     0x00000100U
#define CANFD_MCAN_NDAT2_ND40_S                                              8U

// Field:     [7] ND39
//
// New Data RX Buffer 39
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND39                                       0x00000080U
#define CANFD_MCAN_NDAT2_ND39_M                                     0x00000080U
#define CANFD_MCAN_NDAT2_ND39_S                                              7U

// Field:     [6] ND38
//
// New Data RX Buffer 38
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND38                                       0x00000040U
#define CANFD_MCAN_NDAT2_ND38_M                                     0x00000040U
#define CANFD_MCAN_NDAT2_ND38_S                                              6U

// Field:     [5] ND37
//
// New Data RX Buffer 37
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND37                                       0x00000020U
#define CANFD_MCAN_NDAT2_ND37_M                                     0x00000020U
#define CANFD_MCAN_NDAT2_ND37_S                                              5U

// Field:     [4] ND36
//
// New Data RX Buffer 36
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND36                                       0x00000010U
#define CANFD_MCAN_NDAT2_ND36_M                                     0x00000010U
#define CANFD_MCAN_NDAT2_ND36_S                                              4U

// Field:     [3] ND35
//
// New Data RX Buffer 35
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND35                                       0x00000008U
#define CANFD_MCAN_NDAT2_ND35_M                                     0x00000008U
#define CANFD_MCAN_NDAT2_ND35_S                                              3U

// Field:     [2] ND34
//
// New Data RX Buffer 34
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND34                                       0x00000004U
#define CANFD_MCAN_NDAT2_ND34_M                                     0x00000004U
#define CANFD_MCAN_NDAT2_ND34_S                                              2U

// Field:     [1] ND33
//
// New Data RX Buffer 33
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND33                                       0x00000002U
#define CANFD_MCAN_NDAT2_ND33_M                                     0x00000002U
#define CANFD_MCAN_NDAT2_ND33_S                                              1U

// Field:     [0] ND32
//
// New Data RX Buffer 32
//   0  Rx Buffer not updated
//   1  Rx Buffer updated from new message
#define CANFD_MCAN_NDAT2_ND32                                       0x00000001U
#define CANFD_MCAN_NDAT2_ND32_M                                     0x00000001U
#define CANFD_MCAN_NDAT2_ND32_S                                              0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_RXF0C
//
//*****************************************************************************
// Field:    [31] F0OM
//
// FIFO 0 Operation Mode. FIFO 0 can be operated in blocking or in overwrite
// mode.
//   0  FIFO 0 blocking mode
//   1  FIFO 0 overwrite mode
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_RXF0C_F0OM                                       0x80000000U
#define CANFD_MCAN_RXF0C_F0OM_M                                     0x80000000U
#define CANFD_MCAN_RXF0C_F0OM_S                                             31U

// Field: [30:24] F0WM
//
// Rx FIFO 0 Watermark
//   0      Watermark interrupt disabled
//   1-64  Level for Rx FIFO 0 watermark interrupt (IR.RF0W)
//   >64   Watermark interrupt disabled
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_RXF0C_F0WM_W                                              7U
#define CANFD_MCAN_RXF0C_F0WM_M                                     0x7F000000U
#define CANFD_MCAN_RXF0C_F0WM_S                                             24U

// Field: [22:16] F0S
//
// Rx FIFO 0 Size. The Rx FIFO 0 elements are indexed from 0 to F0S-1.
//   0      No Rx FIFO 0
//   1-64  Number of Rx FIFO 0 elements
//   >64   Values greater than 64 are interpreted as 64
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_RXF0C_F0S_W                                               7U
#define CANFD_MCAN_RXF0C_F0S_M                                      0x007F0000U
#define CANFD_MCAN_RXF0C_F0S_S                                              16U

// Field:  [15:2] F0SA
//
// Rx FIFO 0 Start Address. Start address of Rx FIFO 0 in Message RAM (32-bit
// word address).
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_RXF0C_F0SA_W                                             14U
#define CANFD_MCAN_RXF0C_F0SA_M                                     0x0000FFFCU
#define CANFD_MCAN_RXF0C_F0SA_S                                              2U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_RXF0S
//
//*****************************************************************************
// Field:    [25] RF0L
//
// Rx FIFO 0 Message Lost. This bit is a copy of interrupt flag IR.RF0L. When
// IR.RF0L is reset, this bit is also reset.
//   0  No Rx FIFO 0 message lost
//   1  Rx FIFO 0 message lost, also set after write attempt to Rx FIFO 0 of
// size zero
//
// Note: Overwriting the oldest message when RXF0C.F0OM = '1' will not set this
// flag.
#define CANFD_MCAN_RXF0S_RF0L                                       0x02000000U
#define CANFD_MCAN_RXF0S_RF0L_M                                     0x02000000U
#define CANFD_MCAN_RXF0S_RF0L_S                                             25U

// Field:    [24] F0F
//
// Rx FIFO 0 Full
//   0  Rx FIFO 0 not full
//   1  Rx FIFO 0 full
#define CANFD_MCAN_RXF0S_F0F                                        0x01000000U
#define CANFD_MCAN_RXF0S_F0F_M                                      0x01000000U
#define CANFD_MCAN_RXF0S_F0F_S                                              24U

// Field: [21:16] F0PI
//
// Rx FIFO 0 Put Index. Rx FIFO 0 write index pointer, range 0 to 63.
#define CANFD_MCAN_RXF0S_F0PI_W                                              6U
#define CANFD_MCAN_RXF0S_F0PI_M                                     0x003F0000U
#define CANFD_MCAN_RXF0S_F0PI_S                                             16U

// Field:  [13:8] F0GI
//
// Rx FIFO 0 Get Index. Rx FIFO 0 read index pointer, range 0 to 63.
#define CANFD_MCAN_RXF0S_F0GI_W                                              6U
#define CANFD_MCAN_RXF0S_F0GI_M                                     0x00003F00U
#define CANFD_MCAN_RXF0S_F0GI_S                                              8U

// Field:   [6:0] F0FL
//
// Rx FIFO 0 Fill Level. Number of elements stored in Rx FIFO 0, range 0 to 64.
#define CANFD_MCAN_RXF0S_F0FL_W                                              7U
#define CANFD_MCAN_RXF0S_F0FL_M                                     0x0000007FU
#define CANFD_MCAN_RXF0S_F0FL_S                                              0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_RXF0A
//
//*****************************************************************************
// Field:   [5:0] F0AI
//
// Rx FIFO 0 Acknowledge Index. After the Host has read a message or a sequence
// of messages from Rx FIFO 0 it has to write the buffer index of the last
// element read from Rx FIFO 0 to F0AI. This will set the Rx FIFO 0 Get Index
// RXF0S.F0GI to F0AI + 1 and update the FIFO 0 Fill Level RXF0S.F0FL.
#define CANFD_MCAN_RXF0A_F0AI_W                                              6U
#define CANFD_MCAN_RXF0A_F0AI_M                                     0x0000003FU
#define CANFD_MCAN_RXF0A_F0AI_S                                              0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_RXBC
//
//*****************************************************************************
// Field:  [15:2] RBSA
//
// Rx Buffer Start Address. Configures the start address of the Rx Buffers
// section in the Message RAM (32-bit word address).
//
// +I466
#define CANFD_MCAN_RXBC_RBSA_W                                              14U
#define CANFD_MCAN_RXBC_RBSA_M                                      0x0000FFFCU
#define CANFD_MCAN_RXBC_RBSA_S                                               2U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_RXF1C
//
//*****************************************************************************
// Field:    [31] F1OM
//
// FIFO 1 Operation Mode. FIFO 1 can be operated in blocking or in overwrite
// mode.
//   0  FIFO 1 blocking mode
//   1  FIFO 1 overwrite mode
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_RXF1C_F1OM                                       0x80000000U
#define CANFD_MCAN_RXF1C_F1OM_M                                     0x80000000U
#define CANFD_MCAN_RXF1C_F1OM_S                                             31U

// Field: [30:24] F1WM
//
// Rx FIFO 1 Watermark
//   0      Watermark interrupt disabled
//   1-64  Level for Rx FIFO 1 watermark interrupt (IR.RF1W)
//   >64   Watermark interrupt disabled
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_RXF1C_F1WM_W                                              7U
#define CANFD_MCAN_RXF1C_F1WM_M                                     0x7F000000U
#define CANFD_MCAN_RXF1C_F1WM_S                                             24U

// Field: [22:16] F1S
//
// Rx FIFO 1 Size. The Rx FIFO 1 elements are indexed from 0 to F1S - 1.
//   0      No Rx FIFO 1
//   1-64  Number of Rx FIFO 1 elements
//   >64   Values greater than 64 are interpreted as 64
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_RXF1C_F1S_W                                               7U
#define CANFD_MCAN_RXF1C_F1S_M                                      0x007F0000U
#define CANFD_MCAN_RXF1C_F1S_S                                              16U

// Field:  [15:2] F1SA
//
// Rx FIFO 1 Start Address Start address of Rx FIFO 1 in Message RAM (32-bit
// word address).
#define CANFD_MCAN_RXF1C_F1SA_W                                             14U
#define CANFD_MCAN_RXF1C_F1SA_M                                     0x0000FFFCU
#define CANFD_MCAN_RXF1C_F1SA_S                                              2U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_RXF1S
//
//*****************************************************************************
// Field: [31:30] DMS
//
// Debug Message Status
//   00  Idle state, wait for reception of debug messages, DMA request is
// cleared
//   01  Debug message A received
//   10  Debug messages A, B received
//   11  Debug messages A, B, C received, DMA request is set
#define CANFD_MCAN_RXF1S_DMS_W                                               2U
#define CANFD_MCAN_RXF1S_DMS_M                                      0xC0000000U
#define CANFD_MCAN_RXF1S_DMS_S                                              30U

// Field:    [25] RF1L
//
// Rx FIFO 1 Message Lost. This bit is a copy of interrupt flag IR.RF1L. When
// IR.RF1L is reset, this bit is also reset.
//   0  No Rx FIFO 1 message lost
//   1  Rx FIFO 1 message lost, also set after write attempt to Rx FIFO 1 of
// size zero
//
// Note: Overwriting the oldest message when RXF1C.F1OM = '1' will not set this
// flag.
#define CANFD_MCAN_RXF1S_RF1L                                       0x02000000U
#define CANFD_MCAN_RXF1S_RF1L_M                                     0x02000000U
#define CANFD_MCAN_RXF1S_RF1L_S                                             25U

// Field:    [24] F1F
//
// Rx FIFO 1 Full
//   0  Rx FIFO 1 not full
//   1  Rx FIFO 1 full
#define CANFD_MCAN_RXF1S_F1F                                        0x01000000U
#define CANFD_MCAN_RXF1S_F1F_M                                      0x01000000U
#define CANFD_MCAN_RXF1S_F1F_S                                              24U

// Field: [21:16] F1PI
//
// Rx FIFO 1 Put Index. Rx FIFO 1 write index pointer, range 0 to 63.
#define CANFD_MCAN_RXF1S_F1PI_W                                              6U
#define CANFD_MCAN_RXF1S_F1PI_M                                     0x003F0000U
#define CANFD_MCAN_RXF1S_F1PI_S                                             16U

// Field:  [13:8] F1GI
//
// Rx FIFO 1 Get Index. Rx FIFO 1 read index pointer, range 0 to 63.
#define CANFD_MCAN_RXF1S_F1GI_W                                              6U
#define CANFD_MCAN_RXF1S_F1GI_M                                     0x00003F00U
#define CANFD_MCAN_RXF1S_F1GI_S                                              8U

// Field:   [6:0] F1FL
//
// Rx FIFO 1 Fill Level. Number of elements stored in Rx FIFO 1, range 0 to 64.
#define CANFD_MCAN_RXF1S_F1FL_W                                              7U
#define CANFD_MCAN_RXF1S_F1FL_M                                     0x0000007FU
#define CANFD_MCAN_RXF1S_F1FL_S                                              0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_RXF1A
//
//*****************************************************************************
// Field:   [5:0] F1AI
//
// Rx FIFO 1 Acknowledge Index. After the Host has read a message or a sequence
// of messages from Rx FIFO 1 it has to write the buffer index of the last
// element read from Rx FIFO 1 to F1AI. This will set the Rx FIFO 1 Get Index
// RXF1S.F1GI to F1AI + 1 and update the FIFO 1 Fill Level RXF1S.F1FL.
#define CANFD_MCAN_RXF1A_F1AI_W                                              6U
#define CANFD_MCAN_RXF1A_F1AI_M                                     0x0000003FU
#define CANFD_MCAN_RXF1A_F1AI_S                                              0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_RXESC
//
//*****************************************************************************
// Field:  [10:8] RBDS
//
// Rx Buffer Data Field Size
//   000  8 byte data field
//   001  12 byte data field
//   010  16 byte data field
//   011  20 byte data field
//   100  24 byte data field
//   101  32 byte data field
//   110  48 byte data field
//   111  64 byte data field
//
// Note: In case the data field size of an accepted CAN frame exceeds the data
// field size configured for the matching Rx Buffer or Rx FIFO, only the number
// of bytes as configured by RXESC are stored to the Rx Buffer resp. Rx FIFO
// element. The rest of the frame's data field is ignored.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_RXESC_RBDS_W                                              3U
#define CANFD_MCAN_RXESC_RBDS_M                                     0x00000700U
#define CANFD_MCAN_RXESC_RBDS_S                                              8U

// Field:   [6:4] F1DS
//
// Rx FIFO 1 Data Field Size
//   000  8 byte data field
//   001  12 byte data field
//   010  16 byte data field
//   011  20 byte data field
//   100  24 byte data field
//   101  32 byte data field
//   110  48 byte data field
//   111  64 byte data field
//
// Note: In case the data field size of an accepted CAN frame exceeds the data
// field size configured for the matching Rx Buffer or Rx FIFO, only the number
// of bytes as configured by RXESC are stored to the Rx Buffer resp. Rx FIFO
// element. The rest of the frame's data field is ignored.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_RXESC_F1DS_W                                              3U
#define CANFD_MCAN_RXESC_F1DS_M                                     0x00000070U
#define CANFD_MCAN_RXESC_F1DS_S                                              4U

// Field:   [2:0] F0DS
//
// Rx FIFO 0 Data Field Size
//   000  8 byte data field
//   001  12 byte data field
//   010  16 byte data field
//   011  20 byte data field
//   100  24 byte data field
//   101  32 byte data field
//   110  48 byte data field
//   111  64 byte data field
//
// Note: In case the data field size of an accepted CAN frame exceeds the data
// field size configured for the matching Rx Buffer or Rx FIFO, only the number
// of bytes as configured by RXESC are stored to the Rx Buffer resp. Rx FIFO
// element. The rest of the frame's data field is ignored.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_RXESC_F0DS_W                                              3U
#define CANFD_MCAN_RXESC_F0DS_M                                     0x00000007U
#define CANFD_MCAN_RXESC_F0DS_S                                              0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TXBC
//
//*****************************************************************************
// Field:    [30] TFQM
//
// Tx FIFO/Queue Mode
//   0  Tx FIFO operation
//   1  Tx Queue operation
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_TXBC_TFQM                                        0x40000000U
#define CANFD_MCAN_TXBC_TFQM_M                                      0x40000000U
#define CANFD_MCAN_TXBC_TFQM_S                                              30U

// Field: [29:24] TFQS
//
// Transmit FIFO/Queue Size
//   0      No Tx FIFO/Queue
//   1-32  Number of Tx Buffers used for Tx FIFO/Queue
//   >32   Values greater than 32 are interpreted as 32
//
// Note: Be aware that the sum of TFQS and NDTB may be not greater than 32.
// There is no check for erroneous configurations. The Tx Buffers section in
// the Message RAM starts with the dedicated Tx Buffers.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_TXBC_TFQS_W                                               6U
#define CANFD_MCAN_TXBC_TFQS_M                                      0x3F000000U
#define CANFD_MCAN_TXBC_TFQS_S                                              24U

// Field: [21:16] NDTB
//
// Number of Dedicated Transmit Buffers
//   0      No Dedicated Tx Buffers
//   1-32  Number of Dedicated Tx Buffers
//   >32   Values greater than 32 are interpreted as 32
//
// Note: Be aware that the sum of TFQS and NDTB may be not greater than 32.
// There is no check for erroneous configurations. The Tx Buffers section in
// the Message RAM starts with the dedicated Tx Buffers.
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_TXBC_NDTB_W                                               6U
#define CANFD_MCAN_TXBC_NDTB_M                                      0x003F0000U
#define CANFD_MCAN_TXBC_NDTB_S                                              16U

// Field:  [15:2] TBSA
//
// Tx Buffers Start Address. Start address of Tx Buffers section in Message RAM
// (32-bit word address).
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_TXBC_TBSA_W                                              14U
#define CANFD_MCAN_TXBC_TBSA_M                                      0x0000FFFCU
#define CANFD_MCAN_TXBC_TBSA_S                                               2U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TXFQS
//
//*****************************************************************************
// Field:    [21] TFQF
//
// Tx FIFO/Queue Full
//   0  Tx FIFO/Queue not full
//   1  Tx FIFO/Queue full
#define CANFD_MCAN_TXFQS_TFQF                                       0x00200000U
#define CANFD_MCAN_TXFQS_TFQF_M                                     0x00200000U
#define CANFD_MCAN_TXFQS_TFQF_S                                             21U

// Field: [20:16] TFQP
//
// Tx FIFO/Queue Put Index. Tx FIFO/Queue write index pointer, range 0 to 31.
//
// Note: In case of mixed configurations where dedicated Tx Buffers are
// combined with a Tx FIFO or a Tx Queue, the Put and Get Indices indicate the
// number of the Tx Buffer starting with the first dedicated Tx Buffers.
// Example: For a configuration of 12 dedicated Tx Buffers and a Tx FIFO of 20
// Buffers a Put Index of 15 points to the fourth buffer of the Tx FIFO.
#define CANFD_MCAN_TXFQS_TFQP_W                                              5U
#define CANFD_MCAN_TXFQS_TFQP_M                                     0x001F0000U
#define CANFD_MCAN_TXFQS_TFQP_S                                             16U

// Field:  [12:8] TFGI
//
// Tx FIFO Get Index. Tx FIFO read index pointer, range 0 to 31. Read as zero
// when Tx Queue operation is configured (TXBC.TFQM = '1').
//
// Note: In case of mixed configurations where dedicated Tx Buffers are
// combined with a Tx FIFO or a Tx Queue, the Put and Get Indices indicate the
// number of the Tx Buffer starting with the first dedicated Tx Buffers.
// Example: For a configuration of 12 dedicated Tx Buffers and a Tx FIFO of 20
// Buffers a Put Index of 15 points to the fourth buffer of the Tx FIFO.
#define CANFD_MCAN_TXFQS_TFGI_W                                              5U
#define CANFD_MCAN_TXFQS_TFGI_M                                     0x00001F00U
#define CANFD_MCAN_TXFQS_TFGI_S                                              8U

// Field:   [5:0] TFFL
//
// Tx FIFO Free Level.  Number of consecutive free Tx FIFO elements starting
// from TFGI, range 0 to 32. Read as zero when Tx Queue operation is configured
// (TXBC.TFQM = '1').
#define CANFD_MCAN_TXFQS_TFFL_W                                              6U
#define CANFD_MCAN_TXFQS_TFFL_M                                     0x0000003FU
#define CANFD_MCAN_TXFQS_TFFL_S                                              0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TXESC
//
//*****************************************************************************
// Field:   [2:0] TBDS
//
// Tx Buffer Data Field Size
//   000  8 byte data field
//   001  12 byte data field
//   010  16 byte data field
//   011  20 byte data field
//   100  24 byte data field
//   101  32 byte data field
//   110  48 byte data field
//   111  64 byte data field
//
// Note: In case the data length code DLC of a Tx Buffer element is configured
// to a value higher than the Tx Buffer data field size TXESC.TBDS, the bytes
// not defined by the Tx Buffer are transmitted as "0xCC" (padding bytes).
//
// Qualified Write is possible only with CCCR.CCE='1' and CCCR.INIT='1'.
#define CANFD_MCAN_TXESC_TBDS_W                                              3U
#define CANFD_MCAN_TXESC_TBDS_M                                     0x00000007U
#define CANFD_MCAN_TXESC_TBDS_S                                              0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TXBRP
//
//*****************************************************************************
// Field:    [31] TRP31
//
// Transmission Request Pending 31. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP31                                      0x80000000U
#define CANFD_MCAN_TXBRP_TRP31_M                                    0x80000000U
#define CANFD_MCAN_TXBRP_TRP31_S                                            31U

// Field:    [30] TRP30
//
// Transmission Request Pending 30. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP30                                      0x40000000U
#define CANFD_MCAN_TXBRP_TRP30_M                                    0x40000000U
#define CANFD_MCAN_TXBRP_TRP30_S                                            30U

// Field:    [29] TRP29
//
// Transmission Request Pending 29. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP29                                      0x20000000U
#define CANFD_MCAN_TXBRP_TRP29_M                                    0x20000000U
#define CANFD_MCAN_TXBRP_TRP29_S                                            29U

// Field:    [28] TRP28
//
// Transmission Request Pending 28. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP28                                      0x10000000U
#define CANFD_MCAN_TXBRP_TRP28_M                                    0x10000000U
#define CANFD_MCAN_TXBRP_TRP28_S                                            28U

// Field:    [27] TRP27
//
// Transmission Request Pending 27. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP27                                      0x08000000U
#define CANFD_MCAN_TXBRP_TRP27_M                                    0x08000000U
#define CANFD_MCAN_TXBRP_TRP27_S                                            27U

// Field:    [26] TRP26
//
// Transmission Request Pending 26. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP26                                      0x04000000U
#define CANFD_MCAN_TXBRP_TRP26_M                                    0x04000000U
#define CANFD_MCAN_TXBRP_TRP26_S                                            26U

// Field:    [25] TRP25
//
// Transmission Request Pending 25. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP25                                      0x02000000U
#define CANFD_MCAN_TXBRP_TRP25_M                                    0x02000000U
#define CANFD_MCAN_TXBRP_TRP25_S                                            25U

// Field:    [24] TRP24
//
// Transmission Request Pending 24. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP24                                      0x01000000U
#define CANFD_MCAN_TXBRP_TRP24_M                                    0x01000000U
#define CANFD_MCAN_TXBRP_TRP24_S                                            24U

// Field:    [23] TRP23
//
// Transmission Request Pending 23. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP23                                      0x00800000U
#define CANFD_MCAN_TXBRP_TRP23_M                                    0x00800000U
#define CANFD_MCAN_TXBRP_TRP23_S                                            23U

// Field:    [22] TRP22
//
// Transmission Request Pending 22. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP22                                      0x00400000U
#define CANFD_MCAN_TXBRP_TRP22_M                                    0x00400000U
#define CANFD_MCAN_TXBRP_TRP22_S                                            22U

// Field:    [21] TRP21
//
// Transmission Request Pending 21. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP21                                      0x00200000U
#define CANFD_MCAN_TXBRP_TRP21_M                                    0x00200000U
#define CANFD_MCAN_TXBRP_TRP21_S                                            21U

// Field:    [20] TRP20
//
// Transmission Request Pending 20. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP20                                      0x00100000U
#define CANFD_MCAN_TXBRP_TRP20_M                                    0x00100000U
#define CANFD_MCAN_TXBRP_TRP20_S                                            20U

// Field:    [19] TRP19
//
// Transmission Request Pending 19. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP19                                      0x00080000U
#define CANFD_MCAN_TXBRP_TRP19_M                                    0x00080000U
#define CANFD_MCAN_TXBRP_TRP19_S                                            19U

// Field:    [18] TRP18
//
// Transmission Request Pending 18. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP18                                      0x00040000U
#define CANFD_MCAN_TXBRP_TRP18_M                                    0x00040000U
#define CANFD_MCAN_TXBRP_TRP18_S                                            18U

// Field:    [17] TRP17
//
// Transmission Request Pending 17. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP17                                      0x00020000U
#define CANFD_MCAN_TXBRP_TRP17_M                                    0x00020000U
#define CANFD_MCAN_TXBRP_TRP17_S                                            17U

// Field:    [16] TRP16
//
// Transmission Request Pending 16. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP16                                      0x00010000U
#define CANFD_MCAN_TXBRP_TRP16_M                                    0x00010000U
#define CANFD_MCAN_TXBRP_TRP16_S                                            16U

// Field:    [15] TRP15
//
// Transmission Request Pending 15. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP15                                      0x00008000U
#define CANFD_MCAN_TXBRP_TRP15_M                                    0x00008000U
#define CANFD_MCAN_TXBRP_TRP15_S                                            15U

// Field:    [14] TRP14
//
// Transmission Request Pending 14. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP14                                      0x00004000U
#define CANFD_MCAN_TXBRP_TRP14_M                                    0x00004000U
#define CANFD_MCAN_TXBRP_TRP14_S                                            14U

// Field:    [13] TRP13
//
// Transmission Request Pending 13. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP13                                      0x00002000U
#define CANFD_MCAN_TXBRP_TRP13_M                                    0x00002000U
#define CANFD_MCAN_TXBRP_TRP13_S                                            13U

// Field:    [12] TRP12
//
// Transmission Request Pending 12. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP12                                      0x00001000U
#define CANFD_MCAN_TXBRP_TRP12_M                                    0x00001000U
#define CANFD_MCAN_TXBRP_TRP12_S                                            12U

// Field:    [11] TRP11
//
// Transmission Request Pending 11. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP11                                      0x00000800U
#define CANFD_MCAN_TXBRP_TRP11_M                                    0x00000800U
#define CANFD_MCAN_TXBRP_TRP11_S                                            11U

// Field:    [10] TRP10
//
// Transmission Request Pending 10. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP10                                      0x00000400U
#define CANFD_MCAN_TXBRP_TRP10_M                                    0x00000400U
#define CANFD_MCAN_TXBRP_TRP10_S                                            10U

// Field:     [9] TRP9
//
// Transmission Request Pending 9. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP9                                       0x00000200U
#define CANFD_MCAN_TXBRP_TRP9_M                                     0x00000200U
#define CANFD_MCAN_TXBRP_TRP9_S                                              9U

// Field:     [8] TRP8
//
// Transmission Request Pending 8. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP8                                       0x00000100U
#define CANFD_MCAN_TXBRP_TRP8_M                                     0x00000100U
#define CANFD_MCAN_TXBRP_TRP8_S                                              8U

// Field:     [7] TRP7
//
// Transmission Request Pending 7. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP7                                       0x00000080U
#define CANFD_MCAN_TXBRP_TRP7_M                                     0x00000080U
#define CANFD_MCAN_TXBRP_TRP7_S                                              7U

// Field:     [6] TRP6
//
// Transmission Request Pending 6. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP6                                       0x00000040U
#define CANFD_MCAN_TXBRP_TRP6_M                                     0x00000040U
#define CANFD_MCAN_TXBRP_TRP6_S                                              6U

// Field:     [5] TRP5
//
// Transmission Request Pending 5. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP5                                       0x00000020U
#define CANFD_MCAN_TXBRP_TRP5_M                                     0x00000020U
#define CANFD_MCAN_TXBRP_TRP5_S                                              5U

// Field:     [4] TRP4
//
// Transmission Request Pending 4. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP4                                       0x00000010U
#define CANFD_MCAN_TXBRP_TRP4_M                                     0x00000010U
#define CANFD_MCAN_TXBRP_TRP4_S                                              4U

// Field:     [3] TRP3
//
// Transmission Request Pending 3. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP3                                       0x00000008U
#define CANFD_MCAN_TXBRP_TRP3_M                                     0x00000008U
#define CANFD_MCAN_TXBRP_TRP3_S                                              3U

// Field:     [2] TRP2
//
// Transmission Request Pending 2. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP2                                       0x00000004U
#define CANFD_MCAN_TXBRP_TRP2_M                                     0x00000004U
#define CANFD_MCAN_TXBRP_TRP2_S                                              2U

// Field:     [1] TRP1
//
// Transmission Request Pending 1. See description for bit 0.
#define CANFD_MCAN_TXBRP_TRP1                                       0x00000002U
#define CANFD_MCAN_TXBRP_TRP1_M                                     0x00000002U
#define CANFD_MCAN_TXBRP_TRP1_S                                              1U

// Field:     [0] TRP0
//
// Transmission Request Pending 0.
//
// Each Tx Buffer has its own Transmission Request Pending bit. The bits are
// set via register TXBAR. The bits are reset after a requested transmission
// has completed or has been cancelled via register TXBCR.
//
// TXBRP bits are set only for those Tx Buffers configured via TXBC. After a
// TXBRP bit has been set, a Tx scan is started to check for the pending Tx
// request with the highest priority (Tx Buffer with lowest Message ID).
//
// A cancellation request resets the corresponding transmission request pending
// bit of register TXBRP. In case a transmission has already been started when
// a cancellation is requested, this is done at the end of the transmission,
// regardless whether the transmission was successful or not. The cancellation
// request bits are reset directly after the corresponding TXBRP bit has been
// reset.
//
// After a cancellation has been requested, a finished cancellation is
// signalled via TXBCF
// - after successful transmission together with the corresponding TXBTO bit
// - when the transmission has not yet been started at the point of
// cancellation
// - when the transmission has been aborted due to lost arbitration
// - when an error occurred during frame transmission
//
// In DAR mode all transmissions are automatically cancelled if they are not
// successful. The corresponding TXBCF bit is set for all unsuccessful
// transmissions.
//   0  No transmission request pending
//   1  Transmission request pending
//
// Note: TXBRP bits which are set while a Tx scan is in progress are not
// considered during this particular Tx scan. In case a cancellation is
// requested for such a Tx Buffer, this Add Request is cancelled immediately,
// the corresponding TXBRP bit is reset.
#define CANFD_MCAN_TXBRP_TRP0                                       0x00000001U
#define CANFD_MCAN_TXBRP_TRP0_M                                     0x00000001U
#define CANFD_MCAN_TXBRP_TRP0_S                                              0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TXBAR
//
//*****************************************************************************
// Field:    [31] AR31
//
// Add Request 31. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR31                                       0x80000000U
#define CANFD_MCAN_TXBAR_AR31_M                                     0x80000000U
#define CANFD_MCAN_TXBAR_AR31_S                                             31U

// Field:    [30] AR30
//
// Add Request 30. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR30                                       0x40000000U
#define CANFD_MCAN_TXBAR_AR30_M                                     0x40000000U
#define CANFD_MCAN_TXBAR_AR30_S                                             30U

// Field:    [29] AR29
//
// Add Request 29. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR29                                       0x20000000U
#define CANFD_MCAN_TXBAR_AR29_M                                     0x20000000U
#define CANFD_MCAN_TXBAR_AR29_S                                             29U

// Field:    [28] AR28
//
// Add Request 28. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR28                                       0x10000000U
#define CANFD_MCAN_TXBAR_AR28_M                                     0x10000000U
#define CANFD_MCAN_TXBAR_AR28_S                                             28U

// Field:    [27] AR27
//
// Add Request 27. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR27                                       0x08000000U
#define CANFD_MCAN_TXBAR_AR27_M                                     0x08000000U
#define CANFD_MCAN_TXBAR_AR27_S                                             27U

// Field:    [26] AR26
//
// Add Request 26. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR26                                       0x04000000U
#define CANFD_MCAN_TXBAR_AR26_M                                     0x04000000U
#define CANFD_MCAN_TXBAR_AR26_S                                             26U

// Field:    [25] AR25
//
// Add Request 25. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR25                                       0x02000000U
#define CANFD_MCAN_TXBAR_AR25_M                                     0x02000000U
#define CANFD_MCAN_TXBAR_AR25_S                                             25U

// Field:    [24] AR24
//
// Add Request 24. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR24                                       0x01000000U
#define CANFD_MCAN_TXBAR_AR24_M                                     0x01000000U
#define CANFD_MCAN_TXBAR_AR24_S                                             24U

// Field:    [23] AR23
//
// Add Request 23. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR23                                       0x00800000U
#define CANFD_MCAN_TXBAR_AR23_M                                     0x00800000U
#define CANFD_MCAN_TXBAR_AR23_S                                             23U

// Field:    [22] AR22
//
// Add Request 22. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR22                                       0x00400000U
#define CANFD_MCAN_TXBAR_AR22_M                                     0x00400000U
#define CANFD_MCAN_TXBAR_AR22_S                                             22U

// Field:    [21] AR21
//
// Add Request 21. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR21                                       0x00200000U
#define CANFD_MCAN_TXBAR_AR21_M                                     0x00200000U
#define CANFD_MCAN_TXBAR_AR21_S                                             21U

// Field:    [20] AR20
//
// Add Request 20. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR20                                       0x00100000U
#define CANFD_MCAN_TXBAR_AR20_M                                     0x00100000U
#define CANFD_MCAN_TXBAR_AR20_S                                             20U

// Field:    [19] AR19
//
// Add Request 19. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR19                                       0x00080000U
#define CANFD_MCAN_TXBAR_AR19_M                                     0x00080000U
#define CANFD_MCAN_TXBAR_AR19_S                                             19U

// Field:    [18] AR18
//
// Add Request 18. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR18                                       0x00040000U
#define CANFD_MCAN_TXBAR_AR18_M                                     0x00040000U
#define CANFD_MCAN_TXBAR_AR18_S                                             18U

// Field:    [17] AR17
//
// Add Request 17. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR17                                       0x00020000U
#define CANFD_MCAN_TXBAR_AR17_M                                     0x00020000U
#define CANFD_MCAN_TXBAR_AR17_S                                             17U

// Field:    [16] AR16
//
// Add Request 16. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR16                                       0x00010000U
#define CANFD_MCAN_TXBAR_AR16_M                                     0x00010000U
#define CANFD_MCAN_TXBAR_AR16_S                                             16U

// Field:    [15] AR15
//
// Add Request 15. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR15                                       0x00008000U
#define CANFD_MCAN_TXBAR_AR15_M                                     0x00008000U
#define CANFD_MCAN_TXBAR_AR15_S                                             15U

// Field:    [14] AR14
//
// Add Request 14. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR14                                       0x00004000U
#define CANFD_MCAN_TXBAR_AR14_M                                     0x00004000U
#define CANFD_MCAN_TXBAR_AR14_S                                             14U

// Field:    [13] AR13
//
// Add Request 13. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR13                                       0x00002000U
#define CANFD_MCAN_TXBAR_AR13_M                                     0x00002000U
#define CANFD_MCAN_TXBAR_AR13_S                                             13U

// Field:    [12] AR12
//
// Add Request 12. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR12                                       0x00001000U
#define CANFD_MCAN_TXBAR_AR12_M                                     0x00001000U
#define CANFD_MCAN_TXBAR_AR12_S                                             12U

// Field:    [11] AR11
//
// Add Request 11. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR11                                       0x00000800U
#define CANFD_MCAN_TXBAR_AR11_M                                     0x00000800U
#define CANFD_MCAN_TXBAR_AR11_S                                             11U

// Field:    [10] AR10
//
// Add Request 10. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR10                                       0x00000400U
#define CANFD_MCAN_TXBAR_AR10_M                                     0x00000400U
#define CANFD_MCAN_TXBAR_AR10_S                                             10U

// Field:     [9] AR9
//
// Add Request 9. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR9                                        0x00000200U
#define CANFD_MCAN_TXBAR_AR9_M                                      0x00000200U
#define CANFD_MCAN_TXBAR_AR9_S                                               9U

// Field:     [8] AR8
//
// Add Request 8. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR8                                        0x00000100U
#define CANFD_MCAN_TXBAR_AR8_M                                      0x00000100U
#define CANFD_MCAN_TXBAR_AR8_S                                               8U

// Field:     [7] AR7
//
// Add Request 7. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR7                                        0x00000080U
#define CANFD_MCAN_TXBAR_AR7_M                                      0x00000080U
#define CANFD_MCAN_TXBAR_AR7_S                                               7U

// Field:     [6] AR6
//
// Add Request 6. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR6                                        0x00000040U
#define CANFD_MCAN_TXBAR_AR6_M                                      0x00000040U
#define CANFD_MCAN_TXBAR_AR6_S                                               6U

// Field:     [5] AR5
//
// Add Request 5. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR5                                        0x00000020U
#define CANFD_MCAN_TXBAR_AR5_M                                      0x00000020U
#define CANFD_MCAN_TXBAR_AR5_S                                               5U

// Field:     [4] AR4
//
// Add Request 4. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR4                                        0x00000010U
#define CANFD_MCAN_TXBAR_AR4_M                                      0x00000010U
#define CANFD_MCAN_TXBAR_AR4_S                                               4U

// Field:     [3] AR3
//
// Add Request 3. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR3                                        0x00000008U
#define CANFD_MCAN_TXBAR_AR3_M                                      0x00000008U
#define CANFD_MCAN_TXBAR_AR3_S                                               3U

// Field:     [2] AR2
//
// Add Request 2. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR2                                        0x00000004U
#define CANFD_MCAN_TXBAR_AR2_M                                      0x00000004U
#define CANFD_MCAN_TXBAR_AR2_S                                               2U

// Field:     [1] AR1
//
// Add Request 1. See description for bit 0.
#define CANFD_MCAN_TXBAR_AR1                                        0x00000002U
#define CANFD_MCAN_TXBAR_AR1_M                                      0x00000002U
#define CANFD_MCAN_TXBAR_AR1_S                                               1U

// Field:     [0] AR0
//
// Add Request 0.
//
// Each Tx Buffer has its own Add Request bit. Writing a '1' will set the
// corresponding Add Request bit; writing a '0' has no impact. This enables the
// Host to set transmission requests for multiple Tx Buffers with one write to
// TXBAR. TXBAR bits are set only for those Tx Buffers configured via TXBC.
// When no Tx scan is running, the bits are reset immediately, else the bits
// remain set until the Tx scan process has completed.
//   0  No transmission request added
//   1  Transmission requested added
//
// Note: If an add request is applied for a Tx Buffer with pending transmission
// request (corresponding TXBRP bit already set), this add request is ignored.
//
// Qualified Write is possible only with CCCR.CCE='0'
#define CANFD_MCAN_TXBAR_AR0                                        0x00000001U
#define CANFD_MCAN_TXBAR_AR0_M                                      0x00000001U
#define CANFD_MCAN_TXBAR_AR0_S                                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TXBCR
//
//*****************************************************************************
// Field:    [31] CR31
//
// Cancellation Request 31. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR31                                       0x80000000U
#define CANFD_MCAN_TXBCR_CR31_M                                     0x80000000U
#define CANFD_MCAN_TXBCR_CR31_S                                             31U

// Field:    [30] CR30
//
// Cancellation Request 30. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR30                                       0x40000000U
#define CANFD_MCAN_TXBCR_CR30_M                                     0x40000000U
#define CANFD_MCAN_TXBCR_CR30_S                                             30U

// Field:    [29] CR29
//
// Cancellation Request 29. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR29                                       0x20000000U
#define CANFD_MCAN_TXBCR_CR29_M                                     0x20000000U
#define CANFD_MCAN_TXBCR_CR29_S                                             29U

// Field:    [28] CR28
//
// Cancellation Request 28. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR28                                       0x10000000U
#define CANFD_MCAN_TXBCR_CR28_M                                     0x10000000U
#define CANFD_MCAN_TXBCR_CR28_S                                             28U

// Field:    [27] CR27
//
// Cancellation Request 27. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR27                                       0x08000000U
#define CANFD_MCAN_TXBCR_CR27_M                                     0x08000000U
#define CANFD_MCAN_TXBCR_CR27_S                                             27U

// Field:    [26] CR26
//
// Cancellation Request 26. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR26                                       0x04000000U
#define CANFD_MCAN_TXBCR_CR26_M                                     0x04000000U
#define CANFD_MCAN_TXBCR_CR26_S                                             26U

// Field:    [25] CR25
//
// Cancellation Request 25. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR25                                       0x02000000U
#define CANFD_MCAN_TXBCR_CR25_M                                     0x02000000U
#define CANFD_MCAN_TXBCR_CR25_S                                             25U

// Field:    [24] CR24
//
// Cancellation Request 24. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR24                                       0x01000000U
#define CANFD_MCAN_TXBCR_CR24_M                                     0x01000000U
#define CANFD_MCAN_TXBCR_CR24_S                                             24U

// Field:    [23] CR23
//
// Cancellation Request 23. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR23                                       0x00800000U
#define CANFD_MCAN_TXBCR_CR23_M                                     0x00800000U
#define CANFD_MCAN_TXBCR_CR23_S                                             23U

// Field:    [22] CR22
//
// Cancellation Request 22. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR22                                       0x00400000U
#define CANFD_MCAN_TXBCR_CR22_M                                     0x00400000U
#define CANFD_MCAN_TXBCR_CR22_S                                             22U

// Field:    [21] CR21
//
// Cancellation Request 21. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR21                                       0x00200000U
#define CANFD_MCAN_TXBCR_CR21_M                                     0x00200000U
#define CANFD_MCAN_TXBCR_CR21_S                                             21U

// Field:    [20] CR20
//
// Cancellation Request 20. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR20                                       0x00100000U
#define CANFD_MCAN_TXBCR_CR20_M                                     0x00100000U
#define CANFD_MCAN_TXBCR_CR20_S                                             20U

// Field:    [19] CR19
//
// Cancellation Request 19. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR19                                       0x00080000U
#define CANFD_MCAN_TXBCR_CR19_M                                     0x00080000U
#define CANFD_MCAN_TXBCR_CR19_S                                             19U

// Field:    [18] CR18
//
// Cancellation Request 18. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR18                                       0x00040000U
#define CANFD_MCAN_TXBCR_CR18_M                                     0x00040000U
#define CANFD_MCAN_TXBCR_CR18_S                                             18U

// Field:    [17] CR17
//
// Cancellation Request 17. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR17                                       0x00020000U
#define CANFD_MCAN_TXBCR_CR17_M                                     0x00020000U
#define CANFD_MCAN_TXBCR_CR17_S                                             17U

// Field:    [16] CR16
//
// Cancellation Request 16. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR16                                       0x00010000U
#define CANFD_MCAN_TXBCR_CR16_M                                     0x00010000U
#define CANFD_MCAN_TXBCR_CR16_S                                             16U

// Field:    [15] CR15
//
// Cancellation Request 15. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR15                                       0x00008000U
#define CANFD_MCAN_TXBCR_CR15_M                                     0x00008000U
#define CANFD_MCAN_TXBCR_CR15_S                                             15U

// Field:    [14] CR14
//
// Cancellation Request 14. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR14                                       0x00004000U
#define CANFD_MCAN_TXBCR_CR14_M                                     0x00004000U
#define CANFD_MCAN_TXBCR_CR14_S                                             14U

// Field:    [13] CR13
//
// Cancellation Request 13. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR13                                       0x00002000U
#define CANFD_MCAN_TXBCR_CR13_M                                     0x00002000U
#define CANFD_MCAN_TXBCR_CR13_S                                             13U

// Field:    [12] CR12
//
// Cancellation Request 12. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR12                                       0x00001000U
#define CANFD_MCAN_TXBCR_CR12_M                                     0x00001000U
#define CANFD_MCAN_TXBCR_CR12_S                                             12U

// Field:    [11] CR11
//
// Cancellation Request 11. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR11                                       0x00000800U
#define CANFD_MCAN_TXBCR_CR11_M                                     0x00000800U
#define CANFD_MCAN_TXBCR_CR11_S                                             11U

// Field:    [10] CR10
//
// Cancellation Request 10. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR10                                       0x00000400U
#define CANFD_MCAN_TXBCR_CR10_M                                     0x00000400U
#define CANFD_MCAN_TXBCR_CR10_S                                             10U

// Field:     [9] CR9
//
// Cancellation Request 9. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR9                                        0x00000200U
#define CANFD_MCAN_TXBCR_CR9_M                                      0x00000200U
#define CANFD_MCAN_TXBCR_CR9_S                                               9U

// Field:     [8] CR8
//
// Cancellation Request 8. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR8                                        0x00000100U
#define CANFD_MCAN_TXBCR_CR8_M                                      0x00000100U
#define CANFD_MCAN_TXBCR_CR8_S                                               8U

// Field:     [7] CR7
//
// Cancellation Request 7. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR7                                        0x00000080U
#define CANFD_MCAN_TXBCR_CR7_M                                      0x00000080U
#define CANFD_MCAN_TXBCR_CR7_S                                               7U

// Field:     [6] CR6
//
// Cancellation Request 6. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR6                                        0x00000040U
#define CANFD_MCAN_TXBCR_CR6_M                                      0x00000040U
#define CANFD_MCAN_TXBCR_CR6_S                                               6U

// Field:     [5] CR5
//
// Cancellation Request 5. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR5                                        0x00000020U
#define CANFD_MCAN_TXBCR_CR5_M                                      0x00000020U
#define CANFD_MCAN_TXBCR_CR5_S                                               5U

// Field:     [4] CR4
//
// Cancellation Request 4. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR4                                        0x00000010U
#define CANFD_MCAN_TXBCR_CR4_M                                      0x00000010U
#define CANFD_MCAN_TXBCR_CR4_S                                               4U

// Field:     [3] CR3
//
// Cancellation Request 3. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR3                                        0x00000008U
#define CANFD_MCAN_TXBCR_CR3_M                                      0x00000008U
#define CANFD_MCAN_TXBCR_CR3_S                                               3U

// Field:     [2] CR2
//
// Cancellation Request 2. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR2                                        0x00000004U
#define CANFD_MCAN_TXBCR_CR2_M                                      0x00000004U
#define CANFD_MCAN_TXBCR_CR2_S                                               2U

// Field:     [1] CR1
//
// Cancellation Request 1. See description for bit 0.
#define CANFD_MCAN_TXBCR_CR1                                        0x00000002U
#define CANFD_MCAN_TXBCR_CR1_M                                      0x00000002U
#define CANFD_MCAN_TXBCR_CR1_S                                               1U

// Field:     [0] CR0
//
// Cancellation Request 0.
//
// Each Tx Buffer has its own Cancellation Request bit. Writing a '1' will set
// the corresponding Cancellation Request bit; writing a '0' has no impact.
// This enables the Host to set cancellation requests for multiple Tx Buffers
// with one write to TXBCR. TXBCR bits are set only for those Tx Buffers
// configured via TXBC. The bits remain set until the corresponding bit of
// TXBRP is reset.
//   0  No cancellation pending
//   1  Cancellation pending
//
// Qualified Write is possible only with CCCR.CCE='0'
#define CANFD_MCAN_TXBCR_CR0                                        0x00000001U
#define CANFD_MCAN_TXBCR_CR0_M                                      0x00000001U
#define CANFD_MCAN_TXBCR_CR0_S                                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TXBTO
//
//*****************************************************************************
// Field:    [31] TO31
//
// Transmission Occurred 31. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO31                                       0x80000000U
#define CANFD_MCAN_TXBTO_TO31_M                                     0x80000000U
#define CANFD_MCAN_TXBTO_TO31_S                                             31U

// Field:    [30] TO30
//
// Transmission Occurred 30. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO30                                       0x40000000U
#define CANFD_MCAN_TXBTO_TO30_M                                     0x40000000U
#define CANFD_MCAN_TXBTO_TO30_S                                             30U

// Field:    [29] TO29
//
// Transmission Occurred 29. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO29                                       0x20000000U
#define CANFD_MCAN_TXBTO_TO29_M                                     0x20000000U
#define CANFD_MCAN_TXBTO_TO29_S                                             29U

// Field:    [28] TO28
//
// Transmission Occurred 28. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO28                                       0x10000000U
#define CANFD_MCAN_TXBTO_TO28_M                                     0x10000000U
#define CANFD_MCAN_TXBTO_TO28_S                                             28U

// Field:    [27] TO27
//
// Transmission Occurred 27. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO27                                       0x08000000U
#define CANFD_MCAN_TXBTO_TO27_M                                     0x08000000U
#define CANFD_MCAN_TXBTO_TO27_S                                             27U

// Field:    [26] TO26
//
// Transmission Occurred 26. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO26                                       0x04000000U
#define CANFD_MCAN_TXBTO_TO26_M                                     0x04000000U
#define CANFD_MCAN_TXBTO_TO26_S                                             26U

// Field:    [25] TO25
//
// Transmission Occurred 25. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO25                                       0x02000000U
#define CANFD_MCAN_TXBTO_TO25_M                                     0x02000000U
#define CANFD_MCAN_TXBTO_TO25_S                                             25U

// Field:    [24] TO24
//
// Transmission Occurred 24. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO24                                       0x01000000U
#define CANFD_MCAN_TXBTO_TO24_M                                     0x01000000U
#define CANFD_MCAN_TXBTO_TO24_S                                             24U

// Field:    [23] TO23
//
// Transmission Occurred 23. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO23                                       0x00800000U
#define CANFD_MCAN_TXBTO_TO23_M                                     0x00800000U
#define CANFD_MCAN_TXBTO_TO23_S                                             23U

// Field:    [22] TO22
//
// Transmission Occurred 22. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO22                                       0x00400000U
#define CANFD_MCAN_TXBTO_TO22_M                                     0x00400000U
#define CANFD_MCAN_TXBTO_TO22_S                                             22U

// Field:    [21] TO21
//
// Transmission Occurred 21. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO21                                       0x00200000U
#define CANFD_MCAN_TXBTO_TO21_M                                     0x00200000U
#define CANFD_MCAN_TXBTO_TO21_S                                             21U

// Field:    [20] TO20
//
// Transmission Occurred 20. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO20                                       0x00100000U
#define CANFD_MCAN_TXBTO_TO20_M                                     0x00100000U
#define CANFD_MCAN_TXBTO_TO20_S                                             20U

// Field:    [19] TO19
//
// Transmission Occurred 19. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO19                                       0x00080000U
#define CANFD_MCAN_TXBTO_TO19_M                                     0x00080000U
#define CANFD_MCAN_TXBTO_TO19_S                                             19U

// Field:    [18] TO18
//
// Transmission Occurred 18. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO18                                       0x00040000U
#define CANFD_MCAN_TXBTO_TO18_M                                     0x00040000U
#define CANFD_MCAN_TXBTO_TO18_S                                             18U

// Field:    [17] TO17
//
// Transmission Occurred 17. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO17                                       0x00020000U
#define CANFD_MCAN_TXBTO_TO17_M                                     0x00020000U
#define CANFD_MCAN_TXBTO_TO17_S                                             17U

// Field:    [16] TO16
//
// Transmission Occurred 16. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO16                                       0x00010000U
#define CANFD_MCAN_TXBTO_TO16_M                                     0x00010000U
#define CANFD_MCAN_TXBTO_TO16_S                                             16U

// Field:    [15] TO15
//
// Transmission Occurred 15. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO15                                       0x00008000U
#define CANFD_MCAN_TXBTO_TO15_M                                     0x00008000U
#define CANFD_MCAN_TXBTO_TO15_S                                             15U

// Field:    [14] TO14
//
// Transmission Occurred 14. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO14                                       0x00004000U
#define CANFD_MCAN_TXBTO_TO14_M                                     0x00004000U
#define CANFD_MCAN_TXBTO_TO14_S                                             14U

// Field:    [13] TO13
//
// Transmission Occurred 13. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO13                                       0x00002000U
#define CANFD_MCAN_TXBTO_TO13_M                                     0x00002000U
#define CANFD_MCAN_TXBTO_TO13_S                                             13U

// Field:    [12] TO12
//
// Transmission Occurred 12. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO12                                       0x00001000U
#define CANFD_MCAN_TXBTO_TO12_M                                     0x00001000U
#define CANFD_MCAN_TXBTO_TO12_S                                             12U

// Field:    [11] TO11
//
// Transmission Occurred 11. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO11                                       0x00000800U
#define CANFD_MCAN_TXBTO_TO11_M                                     0x00000800U
#define CANFD_MCAN_TXBTO_TO11_S                                             11U

// Field:    [10] TO10
//
// Transmission Occurred 10. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO10                                       0x00000400U
#define CANFD_MCAN_TXBTO_TO10_M                                     0x00000400U
#define CANFD_MCAN_TXBTO_TO10_S                                             10U

// Field:     [9] TO9
//
// Transmission Occurred 9. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO9                                        0x00000200U
#define CANFD_MCAN_TXBTO_TO9_M                                      0x00000200U
#define CANFD_MCAN_TXBTO_TO9_S                                               9U

// Field:     [8] TO8
//
// Transmission Occurred 8. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO8                                        0x00000100U
#define CANFD_MCAN_TXBTO_TO8_M                                      0x00000100U
#define CANFD_MCAN_TXBTO_TO8_S                                               8U

// Field:     [7] TO7
//
// Transmission Occurred 7. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO7                                        0x00000080U
#define CANFD_MCAN_TXBTO_TO7_M                                      0x00000080U
#define CANFD_MCAN_TXBTO_TO7_S                                               7U

// Field:     [6] TO6
//
// Transmission Occurred 6. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO6                                        0x00000040U
#define CANFD_MCAN_TXBTO_TO6_M                                      0x00000040U
#define CANFD_MCAN_TXBTO_TO6_S                                               6U

// Field:     [5] TO5
//
// Transmission Occurred 5. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO5                                        0x00000020U
#define CANFD_MCAN_TXBTO_TO5_M                                      0x00000020U
#define CANFD_MCAN_TXBTO_TO5_S                                               5U

// Field:     [4] TO4
//
// Transmission Occurred 4. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO4                                        0x00000010U
#define CANFD_MCAN_TXBTO_TO4_M                                      0x00000010U
#define CANFD_MCAN_TXBTO_TO4_S                                               4U

// Field:     [3] TO3
//
// Transmission Occurred 3. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO3                                        0x00000008U
#define CANFD_MCAN_TXBTO_TO3_M                                      0x00000008U
#define CANFD_MCAN_TXBTO_TO3_S                                               3U

// Field:     [2] TO2
//
// Transmission Occurred 2. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO2                                        0x00000004U
#define CANFD_MCAN_TXBTO_TO2_M                                      0x00000004U
#define CANFD_MCAN_TXBTO_TO2_S                                               2U

// Field:     [1] TO1
//
// Transmission Occurred 1. See description for bit 0.
#define CANFD_MCAN_TXBTO_TO1                                        0x00000002U
#define CANFD_MCAN_TXBTO_TO1_M                                      0x00000002U
#define CANFD_MCAN_TXBTO_TO1_S                                               1U

// Field:     [0] TO0
//
// Transmission Occurred 0.
//
// Each Tx Buffer has its own Transmission Occurred bit. The bits are set when
// the corresponding TXBRP bit is cleared after a successful transmission. The
// bits are reset when a new transmission is requested by writing a '1' to the
// corresponding bit of register TXBAR.
//   0  No transmission occurred
//   1  Transmission occurred
#define CANFD_MCAN_TXBTO_TO0                                        0x00000001U
#define CANFD_MCAN_TXBTO_TO0_M                                      0x00000001U
#define CANFD_MCAN_TXBTO_TO0_S                                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TXBCF
//
//*****************************************************************************
// Field:    [31] CF31
//
// Cancellation Finished 31. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF31                                       0x80000000U
#define CANFD_MCAN_TXBCF_CF31_M                                     0x80000000U
#define CANFD_MCAN_TXBCF_CF31_S                                             31U

// Field:    [30] CF30
//
// Cancellation Finished 30. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF30                                       0x40000000U
#define CANFD_MCAN_TXBCF_CF30_M                                     0x40000000U
#define CANFD_MCAN_TXBCF_CF30_S                                             30U

// Field:    [29] CF29
//
// Cancellation Finished 29. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF29                                       0x20000000U
#define CANFD_MCAN_TXBCF_CF29_M                                     0x20000000U
#define CANFD_MCAN_TXBCF_CF29_S                                             29U

// Field:    [28] CF28
//
// Cancellation Finished 28. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF28                                       0x10000000U
#define CANFD_MCAN_TXBCF_CF28_M                                     0x10000000U
#define CANFD_MCAN_TXBCF_CF28_S                                             28U

// Field:    [27] CF27
//
// Cancellation Finished 27. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF27                                       0x08000000U
#define CANFD_MCAN_TXBCF_CF27_M                                     0x08000000U
#define CANFD_MCAN_TXBCF_CF27_S                                             27U

// Field:    [26] CF26
//
// Cancellation Finished 26. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF26                                       0x04000000U
#define CANFD_MCAN_TXBCF_CF26_M                                     0x04000000U
#define CANFD_MCAN_TXBCF_CF26_S                                             26U

// Field:    [25] CF25
//
// Cancellation Finished 25. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF25                                       0x02000000U
#define CANFD_MCAN_TXBCF_CF25_M                                     0x02000000U
#define CANFD_MCAN_TXBCF_CF25_S                                             25U

// Field:    [24] CF24
//
// Cancellation Finished 24. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF24                                       0x01000000U
#define CANFD_MCAN_TXBCF_CF24_M                                     0x01000000U
#define CANFD_MCAN_TXBCF_CF24_S                                             24U

// Field:    [23] CF23
//
// Cancellation Finished 23. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF23                                       0x00800000U
#define CANFD_MCAN_TXBCF_CF23_M                                     0x00800000U
#define CANFD_MCAN_TXBCF_CF23_S                                             23U

// Field:    [22] CF22
//
// Cancellation Finished 22. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF22                                       0x00400000U
#define CANFD_MCAN_TXBCF_CF22_M                                     0x00400000U
#define CANFD_MCAN_TXBCF_CF22_S                                             22U

// Field:    [21] CF21
//
// Cancellation Finished 21. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF21                                       0x00200000U
#define CANFD_MCAN_TXBCF_CF21_M                                     0x00200000U
#define CANFD_MCAN_TXBCF_CF21_S                                             21U

// Field:    [20] CF20
//
// Cancellation Finished 20. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF20                                       0x00100000U
#define CANFD_MCAN_TXBCF_CF20_M                                     0x00100000U
#define CANFD_MCAN_TXBCF_CF20_S                                             20U

// Field:    [19] CF19
//
// Cancellation Finished 19. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF19                                       0x00080000U
#define CANFD_MCAN_TXBCF_CF19_M                                     0x00080000U
#define CANFD_MCAN_TXBCF_CF19_S                                             19U

// Field:    [18] CF18
//
// Cancellation Finished 18. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF18                                       0x00040000U
#define CANFD_MCAN_TXBCF_CF18_M                                     0x00040000U
#define CANFD_MCAN_TXBCF_CF18_S                                             18U

// Field:    [17] CF17
//
// Cancellation Finished 17. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF17                                       0x00020000U
#define CANFD_MCAN_TXBCF_CF17_M                                     0x00020000U
#define CANFD_MCAN_TXBCF_CF17_S                                             17U

// Field:    [16] CF16
//
// Cancellation Finished 16. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF16                                       0x00010000U
#define CANFD_MCAN_TXBCF_CF16_M                                     0x00010000U
#define CANFD_MCAN_TXBCF_CF16_S                                             16U

// Field:    [15] CF15
//
// Cancellation Finished 15. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF15                                       0x00008000U
#define CANFD_MCAN_TXBCF_CF15_M                                     0x00008000U
#define CANFD_MCAN_TXBCF_CF15_S                                             15U

// Field:    [14] CF14
//
// Cancellation Finished 14. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF14                                       0x00004000U
#define CANFD_MCAN_TXBCF_CF14_M                                     0x00004000U
#define CANFD_MCAN_TXBCF_CF14_S                                             14U

// Field:    [13] CF13
//
// Cancellation Finished 13. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF13                                       0x00002000U
#define CANFD_MCAN_TXBCF_CF13_M                                     0x00002000U
#define CANFD_MCAN_TXBCF_CF13_S                                             13U

// Field:    [12] CF12
//
// Cancellation Finished 12. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF12                                       0x00001000U
#define CANFD_MCAN_TXBCF_CF12_M                                     0x00001000U
#define CANFD_MCAN_TXBCF_CF12_S                                             12U

// Field:    [11] CF11
//
// Cancellation Finished 11. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF11                                       0x00000800U
#define CANFD_MCAN_TXBCF_CF11_M                                     0x00000800U
#define CANFD_MCAN_TXBCF_CF11_S                                             11U

// Field:    [10] CF10
//
// Cancellation Finished 10. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF10                                       0x00000400U
#define CANFD_MCAN_TXBCF_CF10_M                                     0x00000400U
#define CANFD_MCAN_TXBCF_CF10_S                                             10U

// Field:     [9] CF9
//
// Cancellation Finished 9. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF9                                        0x00000200U
#define CANFD_MCAN_TXBCF_CF9_M                                      0x00000200U
#define CANFD_MCAN_TXBCF_CF9_S                                               9U

// Field:     [8] CF8
//
// Cancellation Finished 8. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF8                                        0x00000100U
#define CANFD_MCAN_TXBCF_CF8_M                                      0x00000100U
#define CANFD_MCAN_TXBCF_CF8_S                                               8U

// Field:     [7] CF7
//
// Cancellation Finished 7. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF7                                        0x00000080U
#define CANFD_MCAN_TXBCF_CF7_M                                      0x00000080U
#define CANFD_MCAN_TXBCF_CF7_S                                               7U

// Field:     [6] CF6
//
// Cancellation Finished 6. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF6                                        0x00000040U
#define CANFD_MCAN_TXBCF_CF6_M                                      0x00000040U
#define CANFD_MCAN_TXBCF_CF6_S                                               6U

// Field:     [5] CF5
//
// Cancellation Finished 5. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF5                                        0x00000020U
#define CANFD_MCAN_TXBCF_CF5_M                                      0x00000020U
#define CANFD_MCAN_TXBCF_CF5_S                                               5U

// Field:     [4] CF4
//
// Cancellation Finished 4. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF4                                        0x00000010U
#define CANFD_MCAN_TXBCF_CF4_M                                      0x00000010U
#define CANFD_MCAN_TXBCF_CF4_S                                               4U

// Field:     [3] CF3
//
// Cancellation Finished 3. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF3                                        0x00000008U
#define CANFD_MCAN_TXBCF_CF3_M                                      0x00000008U
#define CANFD_MCAN_TXBCF_CF3_S                                               3U

// Field:     [2] CF2
//
// Cancellation Finished 2. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF2                                        0x00000004U
#define CANFD_MCAN_TXBCF_CF2_M                                      0x00000004U
#define CANFD_MCAN_TXBCF_CF2_S                                               2U

// Field:     [1] CF1
//
// Cancellation Finished 1. See description for bit 0.
#define CANFD_MCAN_TXBCF_CF1                                        0x00000002U
#define CANFD_MCAN_TXBCF_CF1_M                                      0x00000002U
#define CANFD_MCAN_TXBCF_CF1_S                                               1U

// Field:     [0] CF0
//
// Cancellation Finished 0.
//
// Each Tx Buffer has its own Cancellation Finished bit. The bits are set when
// the corresponding TXBRP bit is cleared after a cancellation was requested
// via TXBCR. In case the corresponding TXBRP bit was not set at the point of
// cancellation, CF is set immediately. The bits are reset when a new
// transmission is requested by writing a '1' to the corresponding bit of
// register TXBAR.
//   0  No transmit buffer cancellation
//   1  Transmit buffer cancellation finished
#define CANFD_MCAN_TXBCF_CF0                                        0x00000001U
#define CANFD_MCAN_TXBCF_CF0_M                                      0x00000001U
#define CANFD_MCAN_TXBCF_CF0_S                                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TXBTIE
//
//*****************************************************************************
// Field:    [31] TIE31
//
// Transmission Interrupt Enable 31. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE31                                     0x80000000U
#define CANFD_MCAN_TXBTIE_TIE31_M                                   0x80000000U
#define CANFD_MCAN_TXBTIE_TIE31_S                                           31U

// Field:    [30] TIE30
//
// Transmission Interrupt Enable 30. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE30                                     0x40000000U
#define CANFD_MCAN_TXBTIE_TIE30_M                                   0x40000000U
#define CANFD_MCAN_TXBTIE_TIE30_S                                           30U

// Field:    [29] TIE29
//
// Transmission Interrupt Enable 29. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE29                                     0x20000000U
#define CANFD_MCAN_TXBTIE_TIE29_M                                   0x20000000U
#define CANFD_MCAN_TXBTIE_TIE29_S                                           29U

// Field:    [28] TIE28
//
// Transmission Interrupt Enable 28. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE28                                     0x10000000U
#define CANFD_MCAN_TXBTIE_TIE28_M                                   0x10000000U
#define CANFD_MCAN_TXBTIE_TIE28_S                                           28U

// Field:    [27] TIE27
//
// Transmission Interrupt Enable 27. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE27                                     0x08000000U
#define CANFD_MCAN_TXBTIE_TIE27_M                                   0x08000000U
#define CANFD_MCAN_TXBTIE_TIE27_S                                           27U

// Field:    [26] TIE26
//
// Transmission Interrupt Enable 26. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE26                                     0x04000000U
#define CANFD_MCAN_TXBTIE_TIE26_M                                   0x04000000U
#define CANFD_MCAN_TXBTIE_TIE26_S                                           26U

// Field:    [25] TIE25
//
// Transmission Interrupt Enable 25. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE25                                     0x02000000U
#define CANFD_MCAN_TXBTIE_TIE25_M                                   0x02000000U
#define CANFD_MCAN_TXBTIE_TIE25_S                                           25U

// Field:    [24] TIE24
//
// Transmission Interrupt Enable 24. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE24                                     0x01000000U
#define CANFD_MCAN_TXBTIE_TIE24_M                                   0x01000000U
#define CANFD_MCAN_TXBTIE_TIE24_S                                           24U

// Field:    [23] TIE23
//
// Transmission Interrupt Enable 23. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE23                                     0x00800000U
#define CANFD_MCAN_TXBTIE_TIE23_M                                   0x00800000U
#define CANFD_MCAN_TXBTIE_TIE23_S                                           23U

// Field:    [22] TIE22
//
// Transmission Interrupt Enable 22. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE22                                     0x00400000U
#define CANFD_MCAN_TXBTIE_TIE22_M                                   0x00400000U
#define CANFD_MCAN_TXBTIE_TIE22_S                                           22U

// Field:    [21] TIE21
//
// Transmission Interrupt Enable 21. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE21                                     0x00200000U
#define CANFD_MCAN_TXBTIE_TIE21_M                                   0x00200000U
#define CANFD_MCAN_TXBTIE_TIE21_S                                           21U

// Field:    [20] TIE20
//
// Transmission Interrupt Enable 20. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE20                                     0x00100000U
#define CANFD_MCAN_TXBTIE_TIE20_M                                   0x00100000U
#define CANFD_MCAN_TXBTIE_TIE20_S                                           20U

// Field:    [19] TIE19
//
// Transmission Interrupt Enable 19. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE19                                     0x00080000U
#define CANFD_MCAN_TXBTIE_TIE19_M                                   0x00080000U
#define CANFD_MCAN_TXBTIE_TIE19_S                                           19U

// Field:    [18] TIE18
//
// Transmission Interrupt Enable 18. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE18                                     0x00040000U
#define CANFD_MCAN_TXBTIE_TIE18_M                                   0x00040000U
#define CANFD_MCAN_TXBTIE_TIE18_S                                           18U

// Field:    [17] TIE17
//
// Transmission Interrupt Enable 17. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE17                                     0x00020000U
#define CANFD_MCAN_TXBTIE_TIE17_M                                   0x00020000U
#define CANFD_MCAN_TXBTIE_TIE17_S                                           17U

// Field:    [16] TIE16
//
// Transmission Interrupt Enable 16. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE16                                     0x00010000U
#define CANFD_MCAN_TXBTIE_TIE16_M                                   0x00010000U
#define CANFD_MCAN_TXBTIE_TIE16_S                                           16U

// Field:    [15] TIE15
//
// Transmission Interrupt Enable 15. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE15                                     0x00008000U
#define CANFD_MCAN_TXBTIE_TIE15_M                                   0x00008000U
#define CANFD_MCAN_TXBTIE_TIE15_S                                           15U

// Field:    [14] TIE14
//
// Transmission Interrupt Enable 14. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE14                                     0x00004000U
#define CANFD_MCAN_TXBTIE_TIE14_M                                   0x00004000U
#define CANFD_MCAN_TXBTIE_TIE14_S                                           14U

// Field:    [13] TIE13
//
// Transmission Interrupt Enable 13. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE13                                     0x00002000U
#define CANFD_MCAN_TXBTIE_TIE13_M                                   0x00002000U
#define CANFD_MCAN_TXBTIE_TIE13_S                                           13U

// Field:    [12] TIE12
//
// Transmission Interrupt Enable 12. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE12                                     0x00001000U
#define CANFD_MCAN_TXBTIE_TIE12_M                                   0x00001000U
#define CANFD_MCAN_TXBTIE_TIE12_S                                           12U

// Field:    [11] TIE11
//
// Transmission Interrupt Enable 11. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE11                                     0x00000800U
#define CANFD_MCAN_TXBTIE_TIE11_M                                   0x00000800U
#define CANFD_MCAN_TXBTIE_TIE11_S                                           11U

// Field:    [10] TIE10
//
// Transmission Interrupt Enable 10. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE10                                     0x00000400U
#define CANFD_MCAN_TXBTIE_TIE10_M                                   0x00000400U
#define CANFD_MCAN_TXBTIE_TIE10_S                                           10U

// Field:     [9] TIE9
//
// Transmission Interrupt Enable 9. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE9                                      0x00000200U
#define CANFD_MCAN_TXBTIE_TIE9_M                                    0x00000200U
#define CANFD_MCAN_TXBTIE_TIE9_S                                             9U

// Field:     [8] TIE8
//
// Transmission Interrupt Enable 8. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE8                                      0x00000100U
#define CANFD_MCAN_TXBTIE_TIE8_M                                    0x00000100U
#define CANFD_MCAN_TXBTIE_TIE8_S                                             8U

// Field:     [7] TIE7
//
// Transmission Interrupt Enable 7. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE7                                      0x00000080U
#define CANFD_MCAN_TXBTIE_TIE7_M                                    0x00000080U
#define CANFD_MCAN_TXBTIE_TIE7_S                                             7U

// Field:     [6] TIE6
//
// Transmission Interrupt Enable 6. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE6                                      0x00000040U
#define CANFD_MCAN_TXBTIE_TIE6_M                                    0x00000040U
#define CANFD_MCAN_TXBTIE_TIE6_S                                             6U

// Field:     [5] TIE5
//
// Transmission Interrupt Enable 5. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE5                                      0x00000020U
#define CANFD_MCAN_TXBTIE_TIE5_M                                    0x00000020U
#define CANFD_MCAN_TXBTIE_TIE5_S                                             5U

// Field:     [4] TIE4
//
// Transmission Interrupt Enable 4. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE4                                      0x00000010U
#define CANFD_MCAN_TXBTIE_TIE4_M                                    0x00000010U
#define CANFD_MCAN_TXBTIE_TIE4_S                                             4U

// Field:     [3] TIE3
//
// Transmission Interrupt Enable 3. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE3                                      0x00000008U
#define CANFD_MCAN_TXBTIE_TIE3_M                                    0x00000008U
#define CANFD_MCAN_TXBTIE_TIE3_S                                             3U

// Field:     [2] TIE2
//
// Transmission Interrupt Enable 2. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE2                                      0x00000004U
#define CANFD_MCAN_TXBTIE_TIE2_M                                    0x00000004U
#define CANFD_MCAN_TXBTIE_TIE2_S                                             2U

// Field:     [1] TIE1
//
// Transmission Interrupt Enable 1. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE1                                      0x00000002U
#define CANFD_MCAN_TXBTIE_TIE1_M                                    0x00000002U
#define CANFD_MCAN_TXBTIE_TIE1_S                                             1U

// Field:     [0] TIE0
//
// Transmission Interrupt Enable 0. Each Tx Buffer has its own Transmission
// Interrupt Enable bit.
//   0  Transmission interrupt disabled
//   1  Transmission interrupt enable
#define CANFD_MCAN_TXBTIE_TIE0                                      0x00000001U
#define CANFD_MCAN_TXBTIE_TIE0_M                                    0x00000001U
#define CANFD_MCAN_TXBTIE_TIE0_S                                             0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TXBCIE
//
//*****************************************************************************
// Field:    [31] CFIE31
//
// Cancellation Finished Interrupt Enable 31. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE31                                    0x80000000U
#define CANFD_MCAN_TXBCIE_CFIE31_M                                  0x80000000U
#define CANFD_MCAN_TXBCIE_CFIE31_S                                          31U

// Field:    [30] CFIE30
//
// Cancellation Finished Interrupt Enable 30. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE30                                    0x40000000U
#define CANFD_MCAN_TXBCIE_CFIE30_M                                  0x40000000U
#define CANFD_MCAN_TXBCIE_CFIE30_S                                          30U

// Field:    [29] CFIE29
//
// Cancellation Finished Interrupt Enable 29. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE29                                    0x20000000U
#define CANFD_MCAN_TXBCIE_CFIE29_M                                  0x20000000U
#define CANFD_MCAN_TXBCIE_CFIE29_S                                          29U

// Field:    [28] CFIE28
//
// Cancellation Finished Interrupt Enable 28. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE28                                    0x10000000U
#define CANFD_MCAN_TXBCIE_CFIE28_M                                  0x10000000U
#define CANFD_MCAN_TXBCIE_CFIE28_S                                          28U

// Field:    [27] CFIE27
//
// Cancellation Finished Interrupt Enable 27. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE27                                    0x08000000U
#define CANFD_MCAN_TXBCIE_CFIE27_M                                  0x08000000U
#define CANFD_MCAN_TXBCIE_CFIE27_S                                          27U

// Field:    [26] CFIE26
//
// Cancellation Finished Interrupt Enable 26. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE26                                    0x04000000U
#define CANFD_MCAN_TXBCIE_CFIE26_M                                  0x04000000U
#define CANFD_MCAN_TXBCIE_CFIE26_S                                          26U

// Field:    [25] CFIE25
//
// Cancellation Finished Interrupt Enable 25. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE25                                    0x02000000U
#define CANFD_MCAN_TXBCIE_CFIE25_M                                  0x02000000U
#define CANFD_MCAN_TXBCIE_CFIE25_S                                          25U

// Field:    [24] CFIE24
//
// Cancellation Finished Interrupt Enable 24. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE24                                    0x01000000U
#define CANFD_MCAN_TXBCIE_CFIE24_M                                  0x01000000U
#define CANFD_MCAN_TXBCIE_CFIE24_S                                          24U

// Field:    [23] CFIE23
//
// Cancellation Finished Interrupt Enable 23. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE23                                    0x00800000U
#define CANFD_MCAN_TXBCIE_CFIE23_M                                  0x00800000U
#define CANFD_MCAN_TXBCIE_CFIE23_S                                          23U

// Field:    [22] CFIE22
//
// Cancellation Finished Interrupt Enable 22. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE22                                    0x00400000U
#define CANFD_MCAN_TXBCIE_CFIE22_M                                  0x00400000U
#define CANFD_MCAN_TXBCIE_CFIE22_S                                          22U

// Field:    [21] CFIE21
//
// Cancellation Finished Interrupt Enable 21. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE21                                    0x00200000U
#define CANFD_MCAN_TXBCIE_CFIE21_M                                  0x00200000U
#define CANFD_MCAN_TXBCIE_CFIE21_S                                          21U

// Field:    [20] CFIE20
//
// Cancellation Finished Interrupt Enable 20. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE20                                    0x00100000U
#define CANFD_MCAN_TXBCIE_CFIE20_M                                  0x00100000U
#define CANFD_MCAN_TXBCIE_CFIE20_S                                          20U

// Field:    [19] CFIE19
//
// Cancellation Finished Interrupt Enable 19. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE19                                    0x00080000U
#define CANFD_MCAN_TXBCIE_CFIE19_M                                  0x00080000U
#define CANFD_MCAN_TXBCIE_CFIE19_S                                          19U

// Field:    [18] CFIE18
//
// Cancellation Finished Interrupt Enable 18. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE18                                    0x00040000U
#define CANFD_MCAN_TXBCIE_CFIE18_M                                  0x00040000U
#define CANFD_MCAN_TXBCIE_CFIE18_S                                          18U

// Field:    [17] CFIE17
//
// Cancellation Finished Interrupt Enable 17. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE17                                    0x00020000U
#define CANFD_MCAN_TXBCIE_CFIE17_M                                  0x00020000U
#define CANFD_MCAN_TXBCIE_CFIE17_S                                          17U

// Field:    [16] CFIE16
//
// Cancellation Finished Interrupt Enable 16. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE16                                    0x00010000U
#define CANFD_MCAN_TXBCIE_CFIE16_M                                  0x00010000U
#define CANFD_MCAN_TXBCIE_CFIE16_S                                          16U

// Field:    [15] CFIE15
//
// Cancellation Finished Interrupt Enable 15. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE15                                    0x00008000U
#define CANFD_MCAN_TXBCIE_CFIE15_M                                  0x00008000U
#define CANFD_MCAN_TXBCIE_CFIE15_S                                          15U

// Field:    [14] CFIE14
//
// Cancellation Finished Interrupt Enable 14. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE14                                    0x00004000U
#define CANFD_MCAN_TXBCIE_CFIE14_M                                  0x00004000U
#define CANFD_MCAN_TXBCIE_CFIE14_S                                          14U

// Field:    [13] CFIE13
//
// Cancellation Finished Interrupt Enable 13. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE13                                    0x00002000U
#define CANFD_MCAN_TXBCIE_CFIE13_M                                  0x00002000U
#define CANFD_MCAN_TXBCIE_CFIE13_S                                          13U

// Field:    [12] CFIE12
//
// Cancellation Finished Interrupt Enable 12. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE12                                    0x00001000U
#define CANFD_MCAN_TXBCIE_CFIE12_M                                  0x00001000U
#define CANFD_MCAN_TXBCIE_CFIE12_S                                          12U

// Field:    [11] CFIE11
//
// Cancellation Finished Interrupt Enable 11. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE11                                    0x00000800U
#define CANFD_MCAN_TXBCIE_CFIE11_M                                  0x00000800U
#define CANFD_MCAN_TXBCIE_CFIE11_S                                          11U

// Field:    [10] CFIE10
//
// Cancellation Finished Interrupt Enable 10. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE10                                    0x00000400U
#define CANFD_MCAN_TXBCIE_CFIE10_M                                  0x00000400U
#define CANFD_MCAN_TXBCIE_CFIE10_S                                          10U

// Field:     [9] CFIE9
//
// Cancellation Finished Interrupt Enable 9. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE9                                     0x00000200U
#define CANFD_MCAN_TXBCIE_CFIE9_M                                   0x00000200U
#define CANFD_MCAN_TXBCIE_CFIE9_S                                            9U

// Field:     [8] CFIE8
//
// Cancellation Finished Interrupt Enable 8. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE8                                     0x00000100U
#define CANFD_MCAN_TXBCIE_CFIE8_M                                   0x00000100U
#define CANFD_MCAN_TXBCIE_CFIE8_S                                            8U

// Field:     [7] CFIE7
//
// Cancellation Finished Interrupt Enable 7. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE7                                     0x00000080U
#define CANFD_MCAN_TXBCIE_CFIE7_M                                   0x00000080U
#define CANFD_MCAN_TXBCIE_CFIE7_S                                            7U

// Field:     [6] CFIE6
//
// Cancellation Finished Interrupt Enable 6. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE6                                     0x00000040U
#define CANFD_MCAN_TXBCIE_CFIE6_M                                   0x00000040U
#define CANFD_MCAN_TXBCIE_CFIE6_S                                            6U

// Field:     [5] CFIE5
//
// Cancellation Finished Interrupt Enable 5. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE5                                     0x00000020U
#define CANFD_MCAN_TXBCIE_CFIE5_M                                   0x00000020U
#define CANFD_MCAN_TXBCIE_CFIE5_S                                            5U

// Field:     [4] CFIE4
//
// Cancellation Finished Interrupt Enable 4. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE4                                     0x00000010U
#define CANFD_MCAN_TXBCIE_CFIE4_M                                   0x00000010U
#define CANFD_MCAN_TXBCIE_CFIE4_S                                            4U

// Field:     [3] CFIE3
//
// Cancellation Finished Interrupt Enable 3. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE3                                     0x00000008U
#define CANFD_MCAN_TXBCIE_CFIE3_M                                   0x00000008U
#define CANFD_MCAN_TXBCIE_CFIE3_S                                            3U

// Field:     [2] CFIE2
//
// Cancellation Finished Interrupt Enable 2. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE2                                     0x00000004U
#define CANFD_MCAN_TXBCIE_CFIE2_M                                   0x00000004U
#define CANFD_MCAN_TXBCIE_CFIE2_S                                            2U

// Field:     [1] CFIE1
//
// Cancellation Finished Interrupt Enable 1. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE1                                     0x00000002U
#define CANFD_MCAN_TXBCIE_CFIE1_M                                   0x00000002U
#define CANFD_MCAN_TXBCIE_CFIE1_S                                            1U

// Field:     [0] CFIE0
//
// Cancellation Finished Interrupt Enable 0. Each Tx Buffer has its own
// Cancellation Finished Interrupt Enable bit.
//   0  Cancellation finished interrupt disabled
//   1  Cancellation finished interrupt enabled
#define CANFD_MCAN_TXBCIE_CFIE0                                     0x00000001U
#define CANFD_MCAN_TXBCIE_CFIE0_M                                   0x00000001U
#define CANFD_MCAN_TXBCIE_CFIE0_S                                            0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TXEFC
//
//*****************************************************************************
// Field: [29:24] EFWM
//
// Event FIFO Watermark
//   0      Watermark interrupt disabled
//   1-32  Level for Tx Event FIFO watermark interrupt (IR.TEFW)
//   >32   Watermark interrupt disabled
#define CANFD_MCAN_TXEFC_EFWM_W                                              6U
#define CANFD_MCAN_TXEFC_EFWM_M                                     0x3F000000U
#define CANFD_MCAN_TXEFC_EFWM_S                                             24U

// Field: [21:16] EFS
//
// Event FIFO Size. The Tx Event FIFO elements are indexed from 0 to EFS - 1.
//   0      Tx Event FIFO disabled
//   1-32  Number of Tx Event FIFO elements
//   >32   Values greater than 32 are interpreted as 32
#define CANFD_MCAN_TXEFC_EFS_W                                               6U
#define CANFD_MCAN_TXEFC_EFS_M                                      0x003F0000U
#define CANFD_MCAN_TXEFC_EFS_S                                              16U

// Field:  [15:2] EFSA
//
// Event FIFO Start Address. Start address of Tx Event FIFO in Message RAM
// (32-bit word address).
#define CANFD_MCAN_TXEFC_EFSA_W                                             14U
#define CANFD_MCAN_TXEFC_EFSA_M                                     0x0000FFFCU
#define CANFD_MCAN_TXEFC_EFSA_S                                              2U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TXEFS
//
//*****************************************************************************
// Field:    [25] TEFL
//
// Tx Event FIFO Element Lost. This bit is a copy of interrupt flag IR.TEFL.
// When IR.TEFL is reset, this bit is also reset.
//   0  No Tx Event FIFO element lost
//   1  Tx Event FIFO element lost, also set after write attempt to Tx Event
// FIFO of size zero.
#define CANFD_MCAN_TXEFS_TEFL                                       0x02000000U
#define CANFD_MCAN_TXEFS_TEFL_M                                     0x02000000U
#define CANFD_MCAN_TXEFS_TEFL_S                                             25U

// Field:    [24] EFF
//
// Event FIFO Full
//   0  Tx Event FIFO not full
//   1  Tx Event FIFO full
#define CANFD_MCAN_TXEFS_EFF                                        0x01000000U
#define CANFD_MCAN_TXEFS_EFF_M                                      0x01000000U
#define CANFD_MCAN_TXEFS_EFF_S                                              24U

// Field: [20:16] EFPI
//
// Event FIFO Put Index.Tx Event FIFO write index pointer, range 0 to 31.
#define CANFD_MCAN_TXEFS_EFPI_W                                              5U
#define CANFD_MCAN_TXEFS_EFPI_M                                     0x001F0000U
#define CANFD_MCAN_TXEFS_EFPI_S                                             16U

// Field:  [12:8] EFGI
//
// Event FIFO Get Index. Tx Event FIFO read index pointer, range 0 to 31.
#define CANFD_MCAN_TXEFS_EFGI_W                                              5U
#define CANFD_MCAN_TXEFS_EFGI_M                                     0x00001F00U
#define CANFD_MCAN_TXEFS_EFGI_S                                              8U

// Field:   [5:0] EFFL
//
// Event FIFO Fill Level. Number of elements stored in Tx Event FIFO, range 0
// to 32.
#define CANFD_MCAN_TXEFS_EFFL_W                                              6U
#define CANFD_MCAN_TXEFS_EFFL_M                                     0x0000003FU
#define CANFD_MCAN_TXEFS_EFFL_S                                              0U

//*****************************************************************************
//
// Register: CANFD_O_MCAN_TXEFA
//
//*****************************************************************************
// Field:   [4:0] EFAI
//
// Event FIFO Acknowledge Index. After the Host has read an element or a
// sequence of elements from the Tx Event FIFO it has to write the index of the
// last element read from Tx Event FIFO to EFAI. This will set the Tx Event
// FIFO Get Index TXEFS.EFGI to EFAI + 1 and update the Event FIFO Fill Level
// TXEFS.EFFL.
#define CANFD_MCAN_TXEFA_EFAI_W                                              5U
#define CANFD_MCAN_TXEFA_EFAI_M                                     0x0000001FU
#define CANFD_MCAN_TXEFA_EFAI_S                                              0U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_PID
//
//*****************************************************************************
// Field: [31:30] SCHEME
//
// PID Register Scheme
#define CANFD_MCANSS_PID_SCHEME_W                                            2U
#define CANFD_MCANSS_PID_SCHEME_M                                   0xC0000000U
#define CANFD_MCANSS_PID_SCHEME_S                                           30U

// Field: [29:28] BU
//
// Business Unit: 0x2 = Processors
#define CANFD_MCANSS_PID_BU_W                                                2U
#define CANFD_MCANSS_PID_BU_M                                       0x30000000U
#define CANFD_MCANSS_PID_BU_S                                               28U

// Field: [27:16] MODULE_ID
//
// Module Identification Number
#define CANFD_MCANSS_PID_MODULE_ID_W                                        12U
#define CANFD_MCANSS_PID_MODULE_ID_M                                0x0FFF0000U
#define CANFD_MCANSS_PID_MODULE_ID_S                                        16U

// Field: [15:11] RTL
//
// RTL revision. Will vary depending on release
#define CANFD_MCANSS_PID_RTL_W                                               5U
#define CANFD_MCANSS_PID_RTL_M                                      0x0000F800U
#define CANFD_MCANSS_PID_RTL_S                                              11U

// Field:  [10:8] MAJOR
//
// Major Revision of the MCAN Subsystem
#define CANFD_MCANSS_PID_MAJOR_W                                             3U
#define CANFD_MCANSS_PID_MAJOR_M                                    0x00000700U
#define CANFD_MCANSS_PID_MAJOR_S                                             8U

// Field:   [7:6] CUSTOM
//
// Custom Value
#define CANFD_MCANSS_PID_CUSTOM_W                                            2U
#define CANFD_MCANSS_PID_CUSTOM_M                                   0x000000C0U
#define CANFD_MCANSS_PID_CUSTOM_S                                            6U

// Field:   [5:0] MINOR
//
// Minor Revision of the MCAN Subsystem
#define CANFD_MCANSS_PID_MINOR_W                                             6U
#define CANFD_MCANSS_PID_MINOR_M                                    0x0000003FU
#define CANFD_MCANSS_PID_MINOR_S                                             0U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_CTRL
//
//*****************************************************************************
// Field:     [6] EXT_TS_CNTR_EN
//
// External Timestamp Counter Enable. When disabled, the counter is reset back
// to zero.While enabled, the counter keeps incrementing.
//   0  External timestamp counter disabled
//   1  External timestamp counter enabled
#define CANFD_MCANSS_CTRL_EXT_TS_CNTR_EN                            0x00000040U
#define CANFD_MCANSS_CTRL_EXT_TS_CNTR_EN_M                          0x00000040U
#define CANFD_MCANSS_CTRL_EXT_TS_CNTR_EN_S                                   6U

// Field:     [5] AUTOWAKEUP
//
// Automatic Wakeup Enable. Enables the MCANSS to automatically clear the MCAN
// CCCR.INIT bit, fully waking the MCAN up, on an enabled wakeup request.
//   0  Disable the automatic write to CCCR.INIT
//   1  Enable the automatic write to CCCR.INIT
#define CANFD_MCANSS_CTRL_AUTOWAKEUP                                0x00000020U
#define CANFD_MCANSS_CTRL_AUTOWAKEUP_M                              0x00000020U
#define CANFD_MCANSS_CTRL_AUTOWAKEUP_S                                       5U

// Field:     [4] WAKEUPREQEN
//
// Wakeup Request Enable. Enables the MCANSS to wakeup on CAN RXD activity.
//   0  Disable wakeup request
//   1  Enables wakeup request
#define CANFD_MCANSS_CTRL_WAKEUPREQEN                               0x00000010U
#define CANFD_MCANSS_CTRL_WAKEUPREQEN_M                             0x00000010U
#define CANFD_MCANSS_CTRL_WAKEUPREQEN_S                                      4U

// Field:     [3] DBGSUSP_FREE
//
// Debug Suspend Free Bit. Enables debug suspend.
//   0  Disable debug suspend
//   1  Enable debug suspend
#define CANFD_MCANSS_CTRL_DBGSUSP_FREE                              0x00000008U
#define CANFD_MCANSS_CTRL_DBGSUSP_FREE_M                            0x00000008U
#define CANFD_MCANSS_CTRL_DBGSUSP_FREE_S                                     3U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_STAT
//
//*****************************************************************************
// Field:     [2] ENABLE_FDOE
//
// Flexible Datarate Operation Enable. Determines whether CAN FD operation can
// be enabled via the MCAN core CCCR.FDOE bit (bit 8) or if only standard CAN
// operation is possible with this instance of the MCAN.
//   0  MCAN is only capable of standard CAN communication
//   1  MCAN may be configured to perform CAN FD communication
#define CANFD_MCANSS_STAT_ENABLE_FDOE                               0x00000004U
#define CANFD_MCANSS_STAT_ENABLE_FDOE_M                             0x00000004U
#define CANFD_MCANSS_STAT_ENABLE_FDOE_S                                      2U

// Field:     [1] MEM_INIT_DONE
//
// Memory Initialization Done.
//   0  Message RAM initialization is in progress
//   1  Message RAM is initialized for use
#define CANFD_MCANSS_STAT_MEM_INIT_DONE                             0x00000002U
#define CANFD_MCANSS_STAT_MEM_INIT_DONE_M                           0x00000002U
#define CANFD_MCANSS_STAT_MEM_INIT_DONE_S                                    1U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_ICS
//
//*****************************************************************************
// Field:     [0] EXT_TS_CNTR_OVFL
//
// External Timestamp Counter Overflow Interrupt Status Clear. Reads always
// return a 0.
//   0  Write of '0' has no effect
//   1  Write of '1' clears the MCANSS_IRS.EXT_TS_CNTR_OVFL bit
#define CANFD_MCANSS_ICS_EXT_TS_CNTR_OVFL                           0x00000001U
#define CANFD_MCANSS_ICS_EXT_TS_CNTR_OVFL_M                         0x00000001U
#define CANFD_MCANSS_ICS_EXT_TS_CNTR_OVFL_S                                  0U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_IRS
//
//*****************************************************************************
// Field:     [0] EXT_TS_CNTR_OVFL
//
// External Timestamp Counter Overflow Interrupt Status. This bit is set by HW
// or by a SW write of '1'. To clear, use the MCANSS_ICS.EXT_TS_CNTR_OVFL bit.
//   0  External timestamp counter has not overflowed
//   1  External timestamp counter has overflowed
//
// When this bit is set to '1' by HW or SW, the
// MCANSS_EXT_TS_UNSERVICED_INTR_CNTR.EXT_TS_INTR_CNTR bit field will increment
// by 1.
#define CANFD_MCANSS_IRS_EXT_TS_CNTR_OVFL                           0x00000001U
#define CANFD_MCANSS_IRS_EXT_TS_CNTR_OVFL_M                         0x00000001U
#define CANFD_MCANSS_IRS_EXT_TS_CNTR_OVFL_S                                  0U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_IECS
//
//*****************************************************************************
// Field:     [0] EXT_TS_CNTR_OVFL
//
// External Timestamp Counter Overflow Interrupt Enable Clear. Reads always
// return a 0.
//   0  Write of '0' has no effect
//   1  Write of '1' clears the MCANSS_IES.EXT_TS_CNTR_OVFL bit
#define CANFD_MCANSS_IECS_EXT_TS_CNTR_OVFL                          0x00000001U
#define CANFD_MCANSS_IECS_EXT_TS_CNTR_OVFL_M                        0x00000001U
#define CANFD_MCANSS_IECS_EXT_TS_CNTR_OVFL_S                                 0U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_IE
//
//*****************************************************************************
// Field:     [0] EXT_TS_CNTR_OVFL
//
// External Timestamp Counter Overflow Interrupt Enable. A write of '0' has no
// effect. A write of '1' unmasks the MCAN_IRS.EXT_EVT_CNTR_OVFLW and reflects
// the unmasked IRS value in MCAN_IES.EXT_TS_CNTR_OVFL
#define CANFD_MCANSS_IE_EXT_TS_CNTR_OVFL                            0x00000001U
#define CANFD_MCANSS_IE_EXT_TS_CNTR_OVFL_M                          0x00000001U
#define CANFD_MCANSS_IE_EXT_TS_CNTR_OVFL_S                                   0U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_IES
//
//*****************************************************************************
// Field:     [0] EXT_TS_CNTR_OVFL
//
// External Timestamp Counter Overflow masked interrupt status.
//   0  External timestamp counter overflow interrupt is cleared
//   1  External timestamp counter overflow interrupt is set
#define CANFD_MCANSS_IES_EXT_TS_CNTR_OVFL                           0x00000001U
#define CANFD_MCANSS_IES_EXT_TS_CNTR_OVFL_M                         0x00000001U
#define CANFD_MCANSS_IES_EXT_TS_CNTR_OVFL_S                                  0U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_EOI
//
//*****************************************************************************
// Field:   [7:0] EOI
//
// End of Interrupt. A write to this register will clear the associated
// interrupt. If the unserviced interrupt counter is > 1, another interrupt is
// generated.
//   0x00  External TS Interrupt is cleared
//   0x01  MCAN(0) interrupt is cleared
//   0x02  MCAN(1) interrupt is cleared
//   Other writes are ignored.
#define CANFD_MCANSS_EOI_EOI_W                                               8U
#define CANFD_MCANSS_EOI_EOI_M                                      0x000000FFU
#define CANFD_MCANSS_EOI_EOI_S                                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_EXT_TS_PRESCALER
//
//*****************************************************************************
// Field:  [23:0] PRESCALER
//
// External Timestamp Prescaler Reload Value. The external timestamp count rate
// is the host (system) clock rate divided by this value, except in the case of
// 0. A zero value in this bit field will act identically to a value of
// 0x000001.
#define CANFD_MCANSS_EXT_TS_PRESCALER_PRESCALER_W                           24U
#define CANFD_MCANSS_EXT_TS_PRESCALER_PRESCALER_M                   0x00FFFFFFU
#define CANFD_MCANSS_EXT_TS_PRESCALER_PRESCALER_S                            0U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_EXT_TS_UNSERVICED_INTR_CNTR
//
//*****************************************************************************
// Field:   [4:0] EXT_TS_INTR_CNTR
//
// External Timestamp Counter Unserviced Rollover Interrupts. If this value is
// > 1, an MCANSS_EOI write of '1' to bit 0 will issue another interrupt.
//
// The status of this bit field is affected by the MCANSS_IRS.EXT_TS_CNTR_OVFL
// bit field.
#define CANFD_MCANSS_EXT_TS_UNSERVICED_INTR_CNTR_EXT_TS_INTR_CNTR_W  \
                                                                             5U
#define CANFD_MCANSS_EXT_TS_UNSERVICED_INTR_CNTR_EXT_TS_INTR_CNTR_M  \
                                                                    0x0000001FU
#define CANFD_MCANSS_EXT_TS_UNSERVICED_INTR_CNTR_EXT_TS_INTR_CNTR_S  \
                                                                             0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_REV
//
//*****************************************************************************
// Field: [31:30] SCHEME
//
// PID Register Scheme
#define CANFD_MCANERR_REV_SCHEME_W                                           2U
#define CANFD_MCANERR_REV_SCHEME_M                                  0xC0000000U
#define CANFD_MCANERR_REV_SCHEME_S                                          30U

// Field: [29:28] BU
//
// Business Unit: 0x2 = Processors
#define CANFD_MCANERR_REV_BU_W                                               2U
#define CANFD_MCANERR_REV_BU_M                                      0x30000000U
#define CANFD_MCANERR_REV_BU_S                                              28U

// Field: [27:16] MODULE_ID
//
// Module Identification Number
#define CANFD_MCANERR_REV_MODULE_ID_W                                       12U
#define CANFD_MCANERR_REV_MODULE_ID_M                               0x0FFF0000U
#define CANFD_MCANERR_REV_MODULE_ID_S                                       16U

// Field: [15:11] REVRTL
//
// RTL revision. Will vary depending on release
#define CANFD_MCANERR_REV_REVRTL_W                                           5U
#define CANFD_MCANERR_REV_REVRTL_M                                  0x0000F800U
#define CANFD_MCANERR_REV_REVRTL_S                                          11U

// Field:  [10:8] REVMAJ
//
// Major Revision of the Error Aggregator
#define CANFD_MCANERR_REV_REVMAJ_W                                           3U
#define CANFD_MCANERR_REV_REVMAJ_M                                  0x00000700U
#define CANFD_MCANERR_REV_REVMAJ_S                                           8U

// Field:   [7:6] REVCUSTOM
//
// Custom Revision of the Error Aggregator
#define CANFD_MCANERR_REV_REVCUSTOM_W                                        2U
#define CANFD_MCANERR_REV_REVCUSTOM_M                               0x000000C0U
#define CANFD_MCANERR_REV_REVCUSTOM_S                                        6U

// Field:   [5:0] REVMIN
//
// Minor Revision of the Error Aggregator
#define CANFD_MCANERR_REV_REVMIN_W                                           6U
#define CANFD_MCANERR_REV_REVMIN_M                                  0x0000003FU
#define CANFD_MCANERR_REV_REVMIN_S                                           0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_VECTOR
//
//*****************************************************************************
// Field:    [24] RD_SVBUS_DONE
//
// Read Completion Flag
#define CANFD_MCANERR_VECTOR_RD_SVBUS_DONE                          0x01000000U
#define CANFD_MCANERR_VECTOR_RD_SVBUS_DONE_M                        0x01000000U
#define CANFD_MCANERR_VECTOR_RD_SVBUS_DONE_S                                24U

// Field: [23:16] RD_SVBUS_ADDRESS
//
// Read Address Offset
#define CANFD_MCANERR_VECTOR_RD_SVBUS_ADDRESS_W                              8U
#define CANFD_MCANERR_VECTOR_RD_SVBUS_ADDRESS_M                     0x00FF0000U
#define CANFD_MCANERR_VECTOR_RD_SVBUS_ADDRESS_S                             16U

// Field:    [15] RD_SVBUS
//
// Read Trigger
#define CANFD_MCANERR_VECTOR_RD_SVBUS                               0x00008000U
#define CANFD_MCANERR_VECTOR_RD_SVBUS_M                             0x00008000U
#define CANFD_MCANERR_VECTOR_RD_SVBUS_S                                     15U

// Field:  [10:0] ECC_VECTOR
//
// ECC RAM ID. Each error detection and correction (EDC) controller has a bank
// of error registers (offsets 0x10 - 0x3B) associated with it. These registers
// are accessed via an internal serial bus (SVBUS). To access them through the
// ECC aggregator the controller ID desired must be written to the ECC_VECTOR
// field, together with the RD_SVBUS trigger and RD_SVBUS_ADDRESS bit field.
// This initiates the serial read which consummates by setting the
// RD_SVBUS_DONE bit. At this point the addressed register may be read by a
// normal CPU read of the appropriate offset address.
//   0x000  Message RAM ECC controller is selected
//   Others  Reserved (do not use)
//
// Subsequent writes through the SVBUS (offsets 0x10 - 0x3B) have a delayed
// completion. To avoid conflicts, perform a read back of a register within
// this range after writing.
#define CANFD_MCANERR_VECTOR_ECC_VECTOR_W                                   11U
#define CANFD_MCANERR_VECTOR_ECC_VECTOR_M                           0x000007FFU
#define CANFD_MCANERR_VECTOR_ECC_VECTOR_S                                    0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_STAT
//
//*****************************************************************************
// Field:  [10:0] NUM_RAMS
//
// Number of RAMs. Number of ECC RAMs serviced by the aggregator.
#define CANFD_MCANERR_STAT_NUM_RAMS_W                                       11U
#define CANFD_MCANERR_STAT_NUM_RAMS_M                               0x000007FFU
#define CANFD_MCANERR_STAT_NUM_RAMS_S                                        0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_WRAP_REV
//
//*****************************************************************************
// Field: [31:30] SCHEME
//
// PID Register Scheme
#define CANFD_MCANERR_WRAP_REV_SCHEME_W                                      2U
#define CANFD_MCANERR_WRAP_REV_SCHEME_M                             0xC0000000U
#define CANFD_MCANERR_WRAP_REV_SCHEME_S                                     30U

// Field: [29:28] BU
//
// Business Unit: 0x2 = Processors
#define CANFD_MCANERR_WRAP_REV_BU_W                                          2U
#define CANFD_MCANERR_WRAP_REV_BU_M                                 0x30000000U
#define CANFD_MCANERR_WRAP_REV_BU_S                                         28U

// Field: [27:16] MODULE_ID
//
// Module Identification Number
#define CANFD_MCANERR_WRAP_REV_MODULE_ID_W                                  12U
#define CANFD_MCANERR_WRAP_REV_MODULE_ID_M                          0x0FFF0000U
#define CANFD_MCANERR_WRAP_REV_MODULE_ID_S                                  16U

// Field: [15:11] REVRTL
//
// RTL revision. Will vary depending on release
#define CANFD_MCANERR_WRAP_REV_REVRTL_W                                      5U
#define CANFD_MCANERR_WRAP_REV_REVRTL_M                             0x0000F800U
#define CANFD_MCANERR_WRAP_REV_REVRTL_S                                     11U

// Field:  [10:8] REVMAJ
//
// Major Revision of the Error Aggregator
#define CANFD_MCANERR_WRAP_REV_REVMAJ_W                                      3U
#define CANFD_MCANERR_WRAP_REV_REVMAJ_M                             0x00000700U
#define CANFD_MCANERR_WRAP_REV_REVMAJ_S                                      8U

// Field:   [7:6] REVCUSTOM
//
// Custom Revision of the Error Aggregator
#define CANFD_MCANERR_WRAP_REV_REVCUSTOM_W                                   2U
#define CANFD_MCANERR_WRAP_REV_REVCUSTOM_M                          0x000000C0U
#define CANFD_MCANERR_WRAP_REV_REVCUSTOM_S                                   6U

// Field:   [5:0] REVMIN
//
// Minor Revision of the Error Aggregator
#define CANFD_MCANERR_WRAP_REV_REVMIN_W                                      6U
#define CANFD_MCANERR_WRAP_REV_REVMIN_M                             0x0000003FU
#define CANFD_MCANERR_WRAP_REV_REVMIN_S                                      0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_CTRL
//
//*****************************************************************************
// Field:     [8] CHECK_SVBUS_TIMEOUT
//
// Enables Serial VBUS timeout mechanism
#define CANFD_MCANERR_CTRL_CHECK_SVBUS_TIMEOUT                      0x00000100U
#define CANFD_MCANERR_CTRL_CHECK_SVBUS_TIMEOUT_M                    0x00000100U
#define CANFD_MCANERR_CTRL_CHECK_SVBUS_TIMEOUT_S                             8U

// Field:     [7] CHECK_PARITY
//
// Enables parity checking on internal data
#define CANFD_MCANERR_CTRL_CHECK_PARITY                             0x00000080U
#define CANFD_MCANERR_CTRL_CHECK_PARITY_M                           0x00000080U
#define CANFD_MCANERR_CTRL_CHECK_PARITY_S                                    7U

// Field:     [6] ERROR_ONCE
//
// If this bit is set, the FORCE_SEC/FORCE_DED will inject an error to the
// specified row only once. The FORCE_SEC bit will be cleared once a writeback
// happens. If writeback is not enabled, this error will be cleared the cycle
// following the read when the data is corrected. For double-bit errors, the
// FORCE_DED bit will be cleared the cycle following the double-bit error. Any
// subsequent reads will not force an error.
#define CANFD_MCANERR_CTRL_ERROR_ONCE                               0x00000040U
#define CANFD_MCANERR_CTRL_ERROR_ONCE_M                             0x00000040U
#define CANFD_MCANERR_CTRL_ERROR_ONCE_S                                      6U

// Field:     [5] FORCE_N_ROW
//
// Enable single/double-bit error on the next RAM read, regardless of the
// MCANERR_ERR_CTRL1.ECC_ROW setting. For write through mode, this applies to
// writes as well as reads.
#define CANFD_MCANERR_CTRL_FORCE_N_ROW                              0x00000020U
#define CANFD_MCANERR_CTRL_FORCE_N_ROW_M                            0x00000020U
#define CANFD_MCANERR_CTRL_FORCE_N_ROW_S                                     5U

// Field:     [4] FORCE_DED
//
// Force double-bit error. Cleared the cycle following the error if ERROR_ONCE
// is asserted. For write through mode, this applies to writes as well as
// reads. MCANERR_ERR_CTRL1 and MCANERR_ERR_CTRL2 should be configured prior to
// setting this bit.
#define CANFD_MCANERR_CTRL_FORCE_DED                                0x00000010U
#define CANFD_MCANERR_CTRL_FORCE_DED_M                              0x00000010U
#define CANFD_MCANERR_CTRL_FORCE_DED_S                                       4U

// Field:     [3] FORCE_SEC
//
// Force single-bit error. Cleared on a writeback or the cycle following the
// error if ERROR_ONCE is asserted. For write through mode, this applies to
// writes as well as reads. MCANERR_ERR_CTRL1 and MCANERR_ERR_CTRL2 should be
// configured prior to setting this bit.
#define CANFD_MCANERR_CTRL_FORCE_SEC                                0x00000008U
#define CANFD_MCANERR_CTRL_FORCE_SEC_M                              0x00000008U
#define CANFD_MCANERR_CTRL_FORCE_SEC_S                                       3U

// Field:     [2] ENABLE_RMW
//
// Enable read-modify-write on partial word writes
#define CANFD_MCANERR_CTRL_ENABLE_RMW                               0x00000004U
#define CANFD_MCANERR_CTRL_ENABLE_RMW_M                             0x00000004U
#define CANFD_MCANERR_CTRL_ENABLE_RMW_S                                      2U

// Field:     [1] ECC_CHECK
//
// Enable ECC Check. ECC is completely bypassed if both ECC_ENABLE and
// ECC_CHECK are '0'.
#define CANFD_MCANERR_CTRL_ECC_CHECK                                0x00000002U
#define CANFD_MCANERR_CTRL_ECC_CHECK_M                              0x00000002U
#define CANFD_MCANERR_CTRL_ECC_CHECK_S                                       1U

// Field:     [0] ECC_ENABLE
//
// Enable ECC Generation
#define CANFD_MCANERR_CTRL_ECC_ENABLE                               0x00000001U
#define CANFD_MCANERR_CTRL_ECC_ENABLE_M                             0x00000001U
#define CANFD_MCANERR_CTRL_ECC_ENABLE_S                                      0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_ERR_CTRL1
//
//*****************************************************************************
// Field:  [31:0] ECC_ROW
//
// Row address where FORCE_SEC or FORCE_DED needs to be applied. This is
// ignored if FORCE_N_ROW is set.
#define CANFD_MCANERR_ERR_CTRL1_ECC_ROW_W                                   32U
#define CANFD_MCANERR_ERR_CTRL1_ECC_ROW_M                           0xFFFFFFFFU
#define CANFD_MCANERR_ERR_CTRL1_ECC_ROW_S                                    0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_ERR_CTRL2
//
//*****************************************************************************
// Field: [31:16] ECC_BIT2
//
// Second column/data bit that needs to be flipped when FORCE_DED is set
#define CANFD_MCANERR_ERR_CTRL2_ECC_BIT2_W                                  16U
#define CANFD_MCANERR_ERR_CTRL2_ECC_BIT2_M                          0xFFFF0000U
#define CANFD_MCANERR_ERR_CTRL2_ECC_BIT2_S                                  16U

// Field:  [15:0] ECC_BIT1
//
// Column/Data bit that needs to be flipped when FORCE_SEC or FORCE_DED is set
#define CANFD_MCANERR_ERR_CTRL2_ECC_BIT1_W                                  16U
#define CANFD_MCANERR_ERR_CTRL2_ECC_BIT1_M                          0x0000FFFFU
#define CANFD_MCANERR_ERR_CTRL2_ECC_BIT1_S                                   0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_ERR_STAT1
//
//*****************************************************************************
// Field: [31:16] ECC_BIT1
//
// ECC Error Bit Position. Indicates the bit position in the RAM data that is
// in error on an SEC error. Only valid on an SEC error.
//   0  Bit 0 is in error
//   1  Bit 1 is in error
//   2  Bit 2 is in error
//   3  Bit 3 is in error
//   ...
//   31 Bit 31 is in error
//   >32 Invalid
#define CANFD_MCANERR_ERR_STAT1_ECC_BIT1_W                                  16U
#define CANFD_MCANERR_ERR_STAT1_ECC_BIT1_M                          0xFFFF0000U
#define CANFD_MCANERR_ERR_STAT1_ECC_BIT1_S                                  16U

// Field:    [15] CLR_CTRL_REG_ERROR
//
// Writing a '1' clears the CTRL_REG_ERROR bit
#define CANFD_MCANERR_ERR_STAT1_CLR_CTRL_REG_ERROR                  0x00008000U
#define CANFD_MCANERR_ERR_STAT1_CLR_CTRL_REG_ERROR_M                0x00008000U
#define CANFD_MCANERR_ERR_STAT1_CLR_CTRL_REG_ERROR_S                        15U

// Field: [14:13] CLR_PARITY_ERROR
//
// Clear Parity Error. A write of a non-zero value to this bit field decrements
// the PARITY_ERROR bit field by the value provided.
#define CANFD_MCANERR_ERR_STAT1_CLR_PARITY_ERROR_W                           2U
#define CANFD_MCANERR_ERR_STAT1_CLR_PARITY_ERROR_M                  0x00006000U
#define CANFD_MCANERR_ERR_STAT1_CLR_PARITY_ERROR_S                          13U

// Field:    [12] CLR_ECC_OTHER
//
// Writing a '1' clears the ECC_OTHER bit.
#define CANFD_MCANERR_ERR_STAT1_CLR_ECC_OTHER                       0x00001000U
#define CANFD_MCANERR_ERR_STAT1_CLR_ECC_OTHER_M                     0x00001000U
#define CANFD_MCANERR_ERR_STAT1_CLR_ECC_OTHER_S                             12U

// Field: [11:10] CLR_ECC_DED
//
// Clear ECC_DED. A write of a non-zero value to this bit field decrements the
// ECC_DED bit field by the value provided.
#define CANFD_MCANERR_ERR_STAT1_CLR_ECC_DED_W                                2U
#define CANFD_MCANERR_ERR_STAT1_CLR_ECC_DED_M                       0x00000C00U
#define CANFD_MCANERR_ERR_STAT1_CLR_ECC_DED_S                               10U

// Field:   [9:8] CLR_ECC_SEC
//
// Clear ECC_SEC. A write of a non-zero value to this bit field decrements the
// ECC_SEC bit field by the value provided.
#define CANFD_MCANERR_ERR_STAT1_CLR_ECC_SEC_W                                2U
#define CANFD_MCANERR_ERR_STAT1_CLR_ECC_SEC_M                       0x00000300U
#define CANFD_MCANERR_ERR_STAT1_CLR_ECC_SEC_S                                8U

// Field:     [7] CTRL_REG_ERROR
//
// Control Register Error. A bit field in the control register is in an
// ambiguous state. This means that the redundancy registers have detected a
// state where not all values are the same and has defaulted to the reset
// state. S/W needs to re-write these registers to a known state. A write of 1
// will set this interrupt flag.
#define CANFD_MCANERR_ERR_STAT1_CTRL_REG_ERROR                      0x00000080U
#define CANFD_MCANERR_ERR_STAT1_CTRL_REG_ERROR_M                    0x00000080U
#define CANFD_MCANERR_ERR_STAT1_CTRL_REG_ERROR_S                             7U

// Field:   [6:5] PARITY_ERROR
//
// Parity Error Status. A 2-bit saturating counter of the number of parity
// errors that have occurred since last cleared.
//
//   0  No parity error detected
//   1  One parity error was detected
//   2  Two parity errors were detected
//   3  Three parity errors were detected
//
// A write of a non-zero value to this bit field increments it by the value
// provided.
#define CANFD_MCANERR_ERR_STAT1_PARITY_ERROR_W                               2U
#define CANFD_MCANERR_ERR_STAT1_PARITY_ERROR_M                      0x00000060U
#define CANFD_MCANERR_ERR_STAT1_PARITY_ERROR_S                               5U

// Field:     [4] ECC_OTHER
//
// SEC While Writeback Error Status
//   0  No SEC error while writeback pending
//   1  Indicates that successive single-bit errors have occurred while a
// writeback is still pending
#define CANFD_MCANERR_ERR_STAT1_ECC_OTHER                           0x00000010U
#define CANFD_MCANERR_ERR_STAT1_ECC_OTHER_M                         0x00000010U
#define CANFD_MCANERR_ERR_STAT1_ECC_OTHER_S                                  4U

// Field:   [3:2] ECC_DED
//
// Double Bit Error Detected Status. A 2-bit saturating counter of the number
// of DED errors that have occurred since last cleared.
//
//   0  No double-bit error detected
//   1  One double-bit error was detected
//   2  Two double-bit errors were detected
//   3  Three double-bit errors were detected
//
// A write of a non-zero value to this bit field increments it by the value
// provided.
#define CANFD_MCANERR_ERR_STAT1_ECC_DED_W                                    2U
#define CANFD_MCANERR_ERR_STAT1_ECC_DED_M                           0x0000000CU
#define CANFD_MCANERR_ERR_STAT1_ECC_DED_S                                    2U

// Field:   [1:0] ECC_SEC
//
// Single Bit Error Corrected Status. A 2-bit saturating counter of the number
// of SEC errors that have occurred since last cleared.
//
//   0  No single-bit error detected
//   1  One single-bit error was detected and corrected
//   2  Two single-bit errors were detected and corrected
//   3  Three single-bit errors were detected and corrected
//
// A write of a non-zero value to this bit field increments it by the value
// provided.
#define CANFD_MCANERR_ERR_STAT1_ECC_SEC_W                                    2U
#define CANFD_MCANERR_ERR_STAT1_ECC_SEC_M                           0x00000003U
#define CANFD_MCANERR_ERR_STAT1_ECC_SEC_S                                    0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_ERR_STAT2
//
//*****************************************************************************
// Field:  [31:0] ECC_ROW
//
// Indicates the row address where the single or double-bit error occurred.
// This value is address offset/4.
#define CANFD_MCANERR_ERR_STAT2_ECC_ROW_W                                   32U
#define CANFD_MCANERR_ERR_STAT2_ECC_ROW_M                           0xFFFFFFFFU
#define CANFD_MCANERR_ERR_STAT2_ECC_ROW_S                                    0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_ERR_STAT3
//
//*****************************************************************************
// Field:     [9] CLR_SVBUS_TIMEOUT
//
// Write 1 to clear the Serial VBUS Timeout Flag
#define CANFD_MCANERR_ERR_STAT3_CLR_SVBUS_TIMEOUT                   0x00000200U
#define CANFD_MCANERR_ERR_STAT3_CLR_SVBUS_TIMEOUT_M                 0x00000200U
#define CANFD_MCANERR_ERR_STAT3_CLR_SVBUS_TIMEOUT_S                          9U

// Field:     [1] SVBUS_TIMEOUT
//
// Serial VBUS Timeout Flag. Write 1 to set.
#define CANFD_MCANERR_ERR_STAT3_SVBUS_TIMEOUT                       0x00000002U
#define CANFD_MCANERR_ERR_STAT3_SVBUS_TIMEOUT_M                     0x00000002U
#define CANFD_MCANERR_ERR_STAT3_SVBUS_TIMEOUT_S                              1U

// Field:     [0] WB_PEND
//
// Delayed Write Back Pending Status
//   0  No write back pending
//   1  An ECC data correction write back is pending
#define CANFD_MCANERR_ERR_STAT3_WB_PEND                             0x00000001U
#define CANFD_MCANERR_ERR_STAT3_WB_PEND_M                           0x00000001U
#define CANFD_MCANERR_ERR_STAT3_WB_PEND_S                                    0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_SEC_EOI
//
//*****************************************************************************
// Field:     [0] EOI_WR
//
// Write to this register indicates that software has acknowledged the pending
// interrupt and the next interrupt can be sent to the host.
//
// Note that a write to the  MCANERR_ERR_STAT1.CLR_ECC_SEC goes through the
// SVBUS and has a delayed completion. To avoid an additional interrupt, read
// the MCANERR_ERR_STAT1 register back prior to writing to this bit field.
#define CANFD_MCANERR_SEC_EOI_EOI_WR                                0x00000001U
#define CANFD_MCANERR_SEC_EOI_EOI_WR_M                              0x00000001U
#define CANFD_MCANERR_SEC_EOI_EOI_WR_S                                       0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_SEC_STATUS
//
//*****************************************************************************
// Field:     [0] MSGMEM_PEND
//
// Message RAM SEC Interrupt Pending
//   0  No SEC interrupt is pending
//   1  SEC interrupt is pending
#define CANFD_MCANERR_SEC_STATUS_MSGMEM_PEND                        0x00000001U
#define CANFD_MCANERR_SEC_STATUS_MSGMEM_PEND_M                      0x00000001U
#define CANFD_MCANERR_SEC_STATUS_MSGMEM_PEND_S                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_SEC_ENABLE_SET
//
//*****************************************************************************
// Field:     [0] MSGMEM_ENABLE_SET
//
// Message RAM SEC Interrupt Pending Enable Set. Writing a 1 to this bit
// enables the Message RAM SEC error interrupts. Writing a 0 has no effect.
// Reads return the corresponding enable bit's current value.
#define CANFD_MCANERR_SEC_ENABLE_SET_MSGMEM_ENABLE_SET              0x00000001U
#define CANFD_MCANERR_SEC_ENABLE_SET_MSGMEM_ENABLE_SET_M            0x00000001U
#define CANFD_MCANERR_SEC_ENABLE_SET_MSGMEM_ENABLE_SET_S                     0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_SEC_ENABLE_CLR
//
//*****************************************************************************
// Field:     [0] MSGMEM_ENABLE_CLR
//
// Message RAM SEC Interrupt Pending Enable Clear. Writing a 1 to this bit
// disables the Message RAM SEC error interrupts. Writing a 0 has no effect.
// Reads return the corresponding enable bit's current value.
#define CANFD_MCANERR_SEC_ENABLE_CLR_MSGMEM_ENABLE_CLR              0x00000001U
#define CANFD_MCANERR_SEC_ENABLE_CLR_MSGMEM_ENABLE_CLR_M            0x00000001U
#define CANFD_MCANERR_SEC_ENABLE_CLR_MSGMEM_ENABLE_CLR_S                     0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_DED_EOI
//
//*****************************************************************************
// Field:     [0] EOI_WR
//
// Write to this register indicates that software has acknowledged the pending
// interrupt and the next interrupt can be sent to the host.
//
// Note that a write to the  MCANERR_ERR_STAT1.CLR_ECC_DED goes through the
// SVBUS and has a delayed completion. To avoid an additional interrupt, read
// the MCANERR_ERR_STAT1 register back prior to writing to this bit field.
#define CANFD_MCANERR_DED_EOI_EOI_WR                                0x00000001U
#define CANFD_MCANERR_DED_EOI_EOI_WR_M                              0x00000001U
#define CANFD_MCANERR_DED_EOI_EOI_WR_S                                       0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_DED_STATUS
//
//*****************************************************************************
// Field:     [0] MSGMEM_PEND
//
// Message RAM DED Interrupt Pending
//   0  No DED interrupt is pending
//   1  DED interrupt is pending
#define CANFD_MCANERR_DED_STATUS_MSGMEM_PEND                        0x00000001U
#define CANFD_MCANERR_DED_STATUS_MSGMEM_PEND_M                      0x00000001U
#define CANFD_MCANERR_DED_STATUS_MSGMEM_PEND_S                               0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_DED_ENABLE_SET
//
//*****************************************************************************
// Field:     [0] MSGMEM_ENABLE_SET
//
// Message RAM DED Interrupt Pending Enable Set. Writing a 1 to this bit
// enables the Message RAM DED error interrupts. Writing a 0 has no effect.
// Reads return the corresponding enable bit's current value.
#define CANFD_MCANERR_DED_ENABLE_SET_MSGMEM_ENABLE_SET              0x00000001U
#define CANFD_MCANERR_DED_ENABLE_SET_MSGMEM_ENABLE_SET_M            0x00000001U
#define CANFD_MCANERR_DED_ENABLE_SET_MSGMEM_ENABLE_SET_S                     0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_DED_ENABLE_CLR
//
//*****************************************************************************
// Field:     [0] MSGMEM_ENABLE_CLR
//
// Message RAM DED Interrupt Pending Enable Clear. Writing a 1 to this bit
// disables the Message RAM DED error interrupts. Writing a 0 has no effect.
// Reads return the corresponding enable bit's current value.
#define CANFD_MCANERR_DED_ENABLE_CLR_MSGMEM_ENABLE_CLR              0x00000001U
#define CANFD_MCANERR_DED_ENABLE_CLR_MSGMEM_ENABLE_CLR_M            0x00000001U
#define CANFD_MCANERR_DED_ENABLE_CLR_MSGMEM_ENABLE_CLR_S                     0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_AGGR_ENABLE_SET
//
//*****************************************************************************
// Field:     [1] ENABLE_TIMEOUT_SET
//
// Write 1 to enable timeout errors. Reads return the corresponding enable
// bit's current value.
#define CANFD_MCANERR_AGGR_ENABLE_SET_ENABLE_TIMEOUT_SET            0x00000002U
#define CANFD_MCANERR_AGGR_ENABLE_SET_ENABLE_TIMEOUT_SET_M          0x00000002U
#define CANFD_MCANERR_AGGR_ENABLE_SET_ENABLE_TIMEOUT_SET_S                   1U

// Field:     [0] ENABLE_PARITY_SET
//
// Write 1 to enable parity errors. Reads return the corresponding enable bit's
// current value.
#define CANFD_MCANERR_AGGR_ENABLE_SET_ENABLE_PARITY_SET             0x00000001U
#define CANFD_MCANERR_AGGR_ENABLE_SET_ENABLE_PARITY_SET_M           0x00000001U
#define CANFD_MCANERR_AGGR_ENABLE_SET_ENABLE_PARITY_SET_S                    0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_AGGR_ENABLE_CLR
//
//*****************************************************************************
// Field:     [1] ENABLE_TIMEOUT_CLR
//
// Write 1 to disable timeout errors. Reads return the corresponding enable
// bit's current value.
#define CANFD_MCANERR_AGGR_ENABLE_CLR_ENABLE_TIMEOUT_CLR            0x00000002U
#define CANFD_MCANERR_AGGR_ENABLE_CLR_ENABLE_TIMEOUT_CLR_M          0x00000002U
#define CANFD_MCANERR_AGGR_ENABLE_CLR_ENABLE_TIMEOUT_CLR_S                   1U

// Field:     [0] ENABLE_PARITY_CLR
//
// Write 1 to disable parity errors. Reads return the corresponding enable
// bit's current value.
#define CANFD_MCANERR_AGGR_ENABLE_CLR_ENABLE_PARITY_CLR             0x00000001U
#define CANFD_MCANERR_AGGR_ENABLE_CLR_ENABLE_PARITY_CLR_M           0x00000001U
#define CANFD_MCANERR_AGGR_ENABLE_CLR_ENABLE_PARITY_CLR_S                    0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_AGGR_STATUS_SET
//
//*****************************************************************************
// Field:   [3:2] SVBUS_TIMEOUT
//
// Aggregator Serial VBUS Timeout Error Status
//
// 2-bit saturating counter of the number of SVBUS timeout errors that have
// occurred since last cleared.
//   0  No timeout errors have occurred
//   1  One timeout error has occurred
//   2  Two timeout errors have occurred
//   3  Three timeout errors have occurred
//
// A write of a non-zero value to this bit field increments it by the value
// provided.
#define CANFD_MCANERR_AGGR_STATUS_SET_SVBUS_TIMEOUT_W                        2U
#define CANFD_MCANERR_AGGR_STATUS_SET_SVBUS_TIMEOUT_M               0x0000000CU
#define CANFD_MCANERR_AGGR_STATUS_SET_SVBUS_TIMEOUT_S                        2U

// Field:   [1:0] AGGR_PARITY_ERR
//
// Aggregator Parity Error Status
//
// 2-bit saturating counter of the number of parity errors that have occurred
// since last cleared.
//   0  No parity errors have occurred
//   1  One parity error has occurred
//   2  Two parity errors have occurred
//   3  Three parity errors have occurred
//
// A write of a non-zero value to this bit field increments it by the value
// provided.
#define CANFD_MCANERR_AGGR_STATUS_SET_AGGR_PARITY_ERR_W                      2U
#define CANFD_MCANERR_AGGR_STATUS_SET_AGGR_PARITY_ERR_M             0x00000003U
#define CANFD_MCANERR_AGGR_STATUS_SET_AGGR_PARITY_ERR_S                      0U

//*****************************************************************************
//
// Register: CANFD_O_MCANERR_AGGR_STATUS_CLR
//
//*****************************************************************************
// Field:   [3:2] SVBUS_TIMEOUT
//
// Aggregator Serial VBUS Timeout Error Status
//
// 2-bit saturating counter of the number of SVBUS timeout errors that have
// occurred since last cleared.
//   0  No timeout errors have occurred
//   1  One timeout error has occurred
//   2  Two timeout errors have occurred
//   3  Three timeout errors have occurred
//
// A write of a non-zero value to this bit field decrements it by the value
// provided.
#define CANFD_MCANERR_AGGR_STATUS_CLR_SVBUS_TIMEOUT_W                        2U
#define CANFD_MCANERR_AGGR_STATUS_CLR_SVBUS_TIMEOUT_M               0x0000000CU
#define CANFD_MCANERR_AGGR_STATUS_CLR_SVBUS_TIMEOUT_S                        2U

// Field:   [1:0] AGGR_PARITY_ERR
//
// Aggregator Parity Error Status
//
// 2-bit saturating counter of the number of parity errors that have occurred
// since last cleared.
//   0  No parity errors have occurred
//   1  One parity error has occurred
//   2  Two parity errors have occurred
//   3  Three parity errors have occurred
//
// A write of a non-zero value to this bit field decrements it by the value
// provided.
#define CANFD_MCANERR_AGGR_STATUS_CLR_AGGR_PARITY_ERR_W                      2U
#define CANFD_MCANERR_AGGR_STATUS_CLR_AGGR_PARITY_ERR_M             0x00000003U
#define CANFD_MCANERR_AGGR_STATUS_CLR_AGGR_PARITY_ERR_S                      0U

//*****************************************************************************
//
// Register: CANFD_O_DESC
//
//*****************************************************************************
// Field: [31:16] MODID
//
// Module identifier MODID[15:0]. Used to uniquely identify this IP.
#define CANFD_DESC_MODID_W                                                  16U
#define CANFD_DESC_MODID_M                                          0xFFFF0000U
#define CANFD_DESC_MODID_S                                                  16U

// Field: [15:12] STDIPOFF
//
//  64 B standard IP MMR block (beginning with aggregated IRQ registers)
//
// 0: STDIP MMRs do not exist
// 1:15: These MMRs begin at offset 64*STDIPOFF from IP base address
#define CANFD_DESC_STDIPOFF_W                                                4U
#define CANFD_DESC_STDIPOFF_M                                       0x0000F000U
#define CANFD_DESC_STDIPOFF_S                                               12U

// Field:  [11:8] INSTIDX
//
// If multiple instances of IP exists in SOC, this field can identify the
// instance number 0-15
#define CANFD_DESC_INSTIDX_W                                                 4U
#define CANFD_DESC_INSTIDX_M                                        0x00000F00U
#define CANFD_DESC_INSTIDX_S                                                 8U

// Field:   [7:4] MAJREV
//
// Major revision of IP 0-15
#define CANFD_DESC_MAJREV_W                                                  4U
#define CANFD_DESC_MAJREV_M                                         0x000000F0U
#define CANFD_DESC_MAJREV_S                                                  4U

// Field:   [3:0] MINREV
//
// Minor revision of IP 0-15 (typically revision is 1.0 for a verified new IP)
#define CANFD_DESC_MINREV_W                                                  4U
#define CANFD_DESC_MINREV_M                                         0x0000000FU
#define CANFD_DESC_MINREV_S                                                  0U

//*****************************************************************************
//
// Register: CANFD_O_IMASK0
//
//*****************************************************************************
// Field:     [7] DMA_DONE1
//
// DMA DONE channel 1 interrupt mask for MIS0.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK0_DMA_DONE1                                      0x00000080U
#define CANFD_IMASK0_DMA_DONE1_M                                    0x00000080U
#define CANFD_IMASK0_DMA_DONE1_S                                             7U
#define CANFD_IMASK0_DMA_DONE1_SET                                  0x00000080U
#define CANFD_IMASK0_DMA_DONE1_CLR                                  0x00000000U

// Field:     [6] DMA_DONE0
//
// DMA DONE channel 0 interrupt mask for MIS0.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK0_DMA_DONE0                                      0x00000040U
#define CANFD_IMASK0_DMA_DONE0_M                                    0x00000040U
#define CANFD_IMASK0_DMA_DONE0_S                                             6U
#define CANFD_IMASK0_DMA_DONE0_SET                                  0x00000040U
#define CANFD_IMASK0_DMA_DONE0_CLR                                  0x00000000U

// Field:     [5] FE2
//
// Filter event 2 interrupt mask for MIS0.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK0_FE2                                            0x00000020U
#define CANFD_IMASK0_FE2_M                                          0x00000020U
#define CANFD_IMASK0_FE2_S                                                   5U
#define CANFD_IMASK0_FE2_SET                                        0x00000020U
#define CANFD_IMASK0_FE2_CLR                                        0x00000000U

// Field:     [4] EXT_TS_OR_WAKE
//
// External Timestamp Counter Overflow interrupt mask for MIS0.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK0_EXT_TS_OR_WAKE                                 0x00000010U
#define CANFD_IMASK0_EXT_TS_OR_WAKE_M                               0x00000010U
#define CANFD_IMASK0_EXT_TS_OR_WAKE_S                                        4U
#define CANFD_IMASK0_EXT_TS_OR_WAKE_SET                             0x00000010U
#define CANFD_IMASK0_EXT_TS_OR_WAKE_CLR                             0x00000000U

// Field:     [3] DED
//
// Message RAM DED interrupt mask for MIS0.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK0_DED                                            0x00000008U
#define CANFD_IMASK0_DED_M                                          0x00000008U
#define CANFD_IMASK0_DED_S                                                   3U
#define CANFD_IMASK0_DED_SET                                        0x00000008U
#define CANFD_IMASK0_DED_CLR                                        0x00000000U

// Field:     [2] SEC
//
// Message RAM SEC interrupt mask for MIS0.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK0_SEC                                            0x00000004U
#define CANFD_IMASK0_SEC_M                                          0x00000004U
#define CANFD_IMASK0_SEC_S                                                   2U
#define CANFD_IMASK0_SEC_SET                                        0x00000004U
#define CANFD_IMASK0_SEC_CLR                                        0x00000000U

// Field:     [1] INTL1
//
// MCAN Interrupt Line1 mask for MIS0.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK0_INTL1                                          0x00000002U
#define CANFD_IMASK0_INTL1_M                                        0x00000002U
#define CANFD_IMASK0_INTL1_S                                                 1U
#define CANFD_IMASK0_INTL1_SET                                      0x00000002U
#define CANFD_IMASK0_INTL1_CLR                                      0x00000000U

// Field:     [0] INTL0
//
// MCAN Interrupt Line 0 mask for MIS0.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK0_INTL0                                          0x00000001U
#define CANFD_IMASK0_INTL0_M                                        0x00000001U
#define CANFD_IMASK0_INTL0_S                                                 0U
#define CANFD_IMASK0_INTL0_SET                                      0x00000001U
#define CANFD_IMASK0_INTL0_CLR                                      0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_RIS0
//
//*****************************************************************************
// Field:     [7] DMA_DONE1
//
// Raw Interrupt Status for DMA Done interrupt of DMA channel1.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS0_DMA_DONE1                                        0x00000080U
#define CANFD_RIS0_DMA_DONE1_M                                      0x00000080U
#define CANFD_RIS0_DMA_DONE1_S                                               7U
#define CANFD_RIS0_DMA_DONE1_SET                                    0x00000080U
#define CANFD_RIS0_DMA_DONE1_CLR                                    0x00000000U

// Field:     [6] DMA_DONE0
//
// Raw Interrupt Status for DMA Done interrupt of DMA channel0.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS0_DMA_DONE0                                        0x00000040U
#define CANFD_RIS0_DMA_DONE0_M                                      0x00000040U
#define CANFD_RIS0_DMA_DONE0_S                                               6U
#define CANFD_RIS0_DMA_DONE0_SET                                    0x00000040U
#define CANFD_RIS0_DMA_DONE0_CLR                                    0x00000000U

// Field:     [5] FE2
//
// Raw Interrupt Status for Filter Event 2 interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS0_FE2                                              0x00000020U
#define CANFD_RIS0_FE2_M                                            0x00000020U
#define CANFD_RIS0_FE2_S                                                     5U
#define CANFD_RIS0_FE2_SET                                          0x00000020U
#define CANFD_RIS0_FE2_CLR                                          0x00000000U

// Field:     [4] EXT_TS_OR_WAKE
//
// Raw Interrupt Status for External Timestamp Counter Overflow interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS0_EXT_TS_OR_WAKE                                   0x00000010U
#define CANFD_RIS0_EXT_TS_OR_WAKE_M                                 0x00000010U
#define CANFD_RIS0_EXT_TS_OR_WAKE_S                                          4U
#define CANFD_RIS0_EXT_TS_OR_WAKE_SET                               0x00000010U
#define CANFD_RIS0_EXT_TS_OR_WAKE_CLR                               0x00000000U

// Field:     [3] DED
//
// Raw Interrupt Status for Message RAM DED interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS0_DED                                              0x00000008U
#define CANFD_RIS0_DED_M                                            0x00000008U
#define CANFD_RIS0_DED_S                                                     3U
#define CANFD_RIS0_DED_SET                                          0x00000008U
#define CANFD_RIS0_DED_CLR                                          0x00000000U

// Field:     [2] SEC
//
// Raw Interrupt status for Message RAM SEC interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS0_SEC                                              0x00000004U
#define CANFD_RIS0_SEC_M                                            0x00000004U
#define CANFD_RIS0_SEC_S                                                     2U
#define CANFD_RIS0_SEC_SET                                          0x00000004U
#define CANFD_RIS0_SEC_CLR                                          0x00000000U

// Field:     [1] INTL1
//
// Raw Interrupt status for MCAN Interrupt Line 1.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS0_INTL1                                            0x00000002U
#define CANFD_RIS0_INTL1_M                                          0x00000002U
#define CANFD_RIS0_INTL1_S                                                   1U
#define CANFD_RIS0_INTL1_SET                                        0x00000002U
#define CANFD_RIS0_INTL1_CLR                                        0x00000000U

// Field:     [0] INTL0
//
// Raw Interrupt status for MCAN Interrupt Line 0.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS0_INTL0                                            0x00000001U
#define CANFD_RIS0_INTL0_M                                          0x00000001U
#define CANFD_RIS0_INTL0_S                                                   0U
#define CANFD_RIS0_INTL0_SET                                        0x00000001U
#define CANFD_RIS0_INTL0_CLR                                        0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_MIS0
//
//*****************************************************************************
// Field:     [7] DMA_DONE1
//
// Mask interrupt status for DMA DONE interrupt of DMA channel1.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS0_DMA_DONE1                                        0x00000080U
#define CANFD_MIS0_DMA_DONE1_M                                      0x00000080U
#define CANFD_MIS0_DMA_DONE1_S                                               7U
#define CANFD_MIS0_DMA_DONE1_SET                                    0x00000080U
#define CANFD_MIS0_DMA_DONE1_CLR                                    0x00000000U

// Field:     [6] DMA_DONE0
//
// Mask interrupt status for DMA DONE interrupt of DMA channel0.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS0_DMA_DONE0                                        0x00000040U
#define CANFD_MIS0_DMA_DONE0_M                                      0x00000040U
#define CANFD_MIS0_DMA_DONE0_S                                               6U
#define CANFD_MIS0_DMA_DONE0_SET                                    0x00000040U
#define CANFD_MIS0_DMA_DONE0_CLR                                    0x00000000U

// Field:     [5] FE2
//
// Masked Interrupt status for Filter event 2 interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS0_FE2                                              0x00000020U
#define CANFD_MIS0_FE2_M                                            0x00000020U
#define CANFD_MIS0_FE2_S                                                     5U
#define CANFD_MIS0_FE2_SET                                          0x00000020U
#define CANFD_MIS0_FE2_CLR                                          0x00000000U

// Field:     [4] EXT_TS_OR_WAKE
//
// Masked Interrupt status for External Timestamp counter Overflow interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS0_EXT_TS_OR_WAKE                                   0x00000010U
#define CANFD_MIS0_EXT_TS_OR_WAKE_M                                 0x00000010U
#define CANFD_MIS0_EXT_TS_OR_WAKE_S                                          4U
#define CANFD_MIS0_EXT_TS_OR_WAKE_SET                               0x00000010U
#define CANFD_MIS0_EXT_TS_OR_WAKE_CLR                               0x00000000U

// Field:     [3] DED
//
// Masked Interrupt status for Message RAM DED interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS0_DED                                              0x00000008U
#define CANFD_MIS0_DED_M                                            0x00000008U
#define CANFD_MIS0_DED_S                                                     3U
#define CANFD_MIS0_DED_SET                                          0x00000008U
#define CANFD_MIS0_DED_CLR                                          0x00000000U

// Field:     [2] SEC
//
// Masked Interrupt status for Message RAM SEC interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS0_SEC                                              0x00000004U
#define CANFD_MIS0_SEC_M                                            0x00000004U
#define CANFD_MIS0_SEC_S                                                     2U
#define CANFD_MIS0_SEC_SET                                          0x00000004U
#define CANFD_MIS0_SEC_CLR                                          0x00000000U

// Field:     [1] INTL1
//
// Mask interrupt status for MCAN Interrupt Line 1.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS0_INTL1                                            0x00000002U
#define CANFD_MIS0_INTL1_M                                          0x00000002U
#define CANFD_MIS0_INTL1_S                                                   1U
#define CANFD_MIS0_INTL1_SET                                        0x00000002U
#define CANFD_MIS0_INTL1_CLR                                        0x00000000U

// Field:     [0] INTL0
//
// Mask interrupt status for MCAN Interrupt Line 0.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS0_INTL0                                            0x00000001U
#define CANFD_MIS0_INTL0_M                                          0x00000001U
#define CANFD_MIS0_INTL0_S                                                   0U
#define CANFD_MIS0_INTL0_SET                                        0x00000001U
#define CANFD_MIS0_INTL0_CLR                                        0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_ISET0
//
//*****************************************************************************
// Field:     [7] DMA_DONE1
//
// Sets DMA DONE for DMA channel 1 interrupt in RIS0.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET0_DMA_DONE1                                       0x00000080U
#define CANFD_ISET0_DMA_DONE1_M                                     0x00000080U
#define CANFD_ISET0_DMA_DONE1_S                                              7U
#define CANFD_ISET0_DMA_DONE1_SET                                   0x00000080U
#define CANFD_ISET0_DMA_DONE1_NO_EFFECT                             0x00000000U

// Field:     [6] DMA_DONE0
//
// Sets DMA DONE for DMA channel 0 interrupt in RIS0.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET0_DMA_DONE0                                       0x00000040U
#define CANFD_ISET0_DMA_DONE0_M                                     0x00000040U
#define CANFD_ISET0_DMA_DONE0_S                                              6U
#define CANFD_ISET0_DMA_DONE0_SET                                   0x00000040U
#define CANFD_ISET0_DMA_DONE0_NO_EFFECT                             0x00000000U

// Field:     [5] FE2
//
// Sets Filter event 2 interrupt in RIS0.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET0_FE2                                             0x00000020U
#define CANFD_ISET0_FE2_M                                           0x00000020U
#define CANFD_ISET0_FE2_S                                                    5U
#define CANFD_ISET0_FE2_SET                                         0x00000020U
#define CANFD_ISET0_FE2_NO_EFFECT                                   0x00000000U

// Field:     [4] EXT_TS_OR_WAKE
//
// Sets External Time stamp counter Overflow interrupt in RIS0.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET0_EXT_TS_OR_WAKE                                  0x00000010U
#define CANFD_ISET0_EXT_TS_OR_WAKE_M                                0x00000010U
#define CANFD_ISET0_EXT_TS_OR_WAKE_S                                         4U
#define CANFD_ISET0_EXT_TS_OR_WAKE_SET                              0x00000010U
#define CANFD_ISET0_EXT_TS_OR_WAKE_NO_EFFECT                        0x00000000U

// Field:     [3] DED
//
// Sets Message RAM DED interrupt in RIS0.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET0_DED                                             0x00000008U
#define CANFD_ISET0_DED_M                                           0x00000008U
#define CANFD_ISET0_DED_S                                                    3U
#define CANFD_ISET0_DED_SET                                         0x00000008U
#define CANFD_ISET0_DED_NO_EFFECT                                   0x00000000U

// Field:     [2] SEC
//
// Sets Message RAM SEC interrupt in RIS0.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET0_SEC                                             0x00000004U
#define CANFD_ISET0_SEC_M                                           0x00000004U
#define CANFD_ISET0_SEC_S                                                    2U
#define CANFD_ISET0_SEC_SET                                         0x00000004U
#define CANFD_ISET0_SEC_NO_EFFECT                                   0x00000000U

// Field:     [1] INTL1
//
// Sets MCAN Interrupt Line 1 interrupt in RIS0.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET0_INTL1                                           0x00000002U
#define CANFD_ISET0_INTL1_M                                         0x00000002U
#define CANFD_ISET0_INTL1_S                                                  1U
#define CANFD_ISET0_INTL1_SET                                       0x00000002U
#define CANFD_ISET0_INTL1_NO_EFFECT                                 0x00000000U

// Field:     [0] INTL0
//
// Sets MCAN Interrupt Line 0 interrupt in RIS0.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET0_INTL0                                           0x00000001U
#define CANFD_ISET0_INTL0_M                                         0x00000001U
#define CANFD_ISET0_INTL0_S                                                  0U
#define CANFD_ISET0_INTL0_SET                                       0x00000001U
#define CANFD_ISET0_INTL0_NO_EFFECT                                 0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_ICLR0
//
//*****************************************************************************
// Field:     [7] DMA_DONE1
//
// Clears DMA DONE interrupt for DMA channel 1 in RIS0.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR0_DMA_DONE1                                       0x00000080U
#define CANFD_ICLR0_DMA_DONE1_M                                     0x00000080U
#define CANFD_ICLR0_DMA_DONE1_S                                              7U
#define CANFD_ICLR0_DMA_DONE1_CLR                                   0x00000080U
#define CANFD_ICLR0_DMA_DONE1_NO_EFFECT                             0x00000000U

// Field:     [6] DMA_DONE0
//
// Clears DMA DONE interrupt for DMA channel 0 in RIS0.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR0_DMA_DONE0                                       0x00000040U
#define CANFD_ICLR0_DMA_DONE0_M                                     0x00000040U
#define CANFD_ICLR0_DMA_DONE0_S                                              6U
#define CANFD_ICLR0_DMA_DONE0_CLR                                   0x00000040U
#define CANFD_ICLR0_DMA_DONE0_NO_EFFECT                             0x00000000U

// Field:     [5] FE2
//
// Clears Filter Event 2  interrupt in RIS0.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR0_FE2                                             0x00000020U
#define CANFD_ICLR0_FE2_M                                           0x00000020U
#define CANFD_ICLR0_FE2_S                                                    5U
#define CANFD_ICLR0_FE2_CLR                                         0x00000020U
#define CANFD_ICLR0_FE2_NO_EFFECT                                   0x00000000U

// Field:     [4] EXT_TS_OR_WAKE
//
// Clears External Time stamp counter Overflow in RIS0.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR0_EXT_TS_OR_WAKE                                  0x00000010U
#define CANFD_ICLR0_EXT_TS_OR_WAKE_M                                0x00000010U
#define CANFD_ICLR0_EXT_TS_OR_WAKE_S                                         4U
#define CANFD_ICLR0_EXT_TS_OR_WAKE_CLR                              0x00000010U
#define CANFD_ICLR0_EXT_TS_OR_WAKE_NO_EFFECT                        0x00000000U

// Field:     [3] DED
//
// Clears Message RAM DED interrupt in RIS0.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR0_DED                                             0x00000008U
#define CANFD_ICLR0_DED_M                                           0x00000008U
#define CANFD_ICLR0_DED_S                                                    3U
#define CANFD_ICLR0_DED_CLR                                         0x00000008U
#define CANFD_ICLR0_DED_NO_EFFECT                                   0x00000000U

// Field:     [2] SEC
//
// Clears Message RAM SEC interrupt in RIS0.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR0_SEC                                             0x00000004U
#define CANFD_ICLR0_SEC_M                                           0x00000004U
#define CANFD_ICLR0_SEC_S                                                    2U
#define CANFD_ICLR0_SEC_CLR                                         0x00000004U
#define CANFD_ICLR0_SEC_NO_EFFECT                                   0x00000000U

// Field:     [1] INTL1
//
// Clears MCAN Interrupt Line 1 interrupt in RIS0.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR0_INTL1                                           0x00000002U
#define CANFD_ICLR0_INTL1_M                                         0x00000002U
#define CANFD_ICLR0_INTL1_S                                                  1U
#define CANFD_ICLR0_INTL1_CLR                                       0x00000002U
#define CANFD_ICLR0_INTL1_NO_EFFECT                                 0x00000000U

// Field:     [0] INTL0
//
// Clears MCAN Interrupt Line 0 interrupt in RIS0.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR0_INTL0                                           0x00000001U
#define CANFD_ICLR0_INTL0_M                                         0x00000001U
#define CANFD_ICLR0_INTL0_S                                                  0U
#define CANFD_ICLR0_INTL0_CLR                                       0x00000001U
#define CANFD_ICLR0_INTL0_NO_EFFECT                                 0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_DTB
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
// DISABLE                  DTB output from peripheral is 0x0.
#define CANFD_DTB_SEL_W                                                      3U
#define CANFD_DTB_SEL_M                                             0x00000007U
#define CANFD_DTB_SEL_S                                                      0U
#define CANFD_DTB_SEL_GRP7                                          0x00000007U
#define CANFD_DTB_SEL_GRP6                                          0x00000006U
#define CANFD_DTB_SEL_GRP5                                          0x00000005U
#define CANFD_DTB_SEL_GRP4                                          0x00000004U
#define CANFD_DTB_SEL_GRP3                                          0x00000003U
#define CANFD_DTB_SEL_GRP2                                          0x00000002U
#define CANFD_DTB_SEL_GRP1                                          0x00000001U
#define CANFD_DTB_SEL_DISABLE                                       0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_IMASK1
//
//*****************************************************************************
// Field:     [7] DMA_DONE1
//
// DMA DONE channel 1 interrupt mask for MIS1.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK1_DMA_DONE1                                      0x00000080U
#define CANFD_IMASK1_DMA_DONE1_M                                    0x00000080U
#define CANFD_IMASK1_DMA_DONE1_S                                             7U
#define CANFD_IMASK1_DMA_DONE1_SET                                  0x00000080U
#define CANFD_IMASK1_DMA_DONE1_CLR                                  0x00000000U

// Field:     [6] DMA_DONE0
//
// DMA DONE channel 0 interrupt mask for MIS1.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK1_DMA_DONE0                                      0x00000040U
#define CANFD_IMASK1_DMA_DONE0_M                                    0x00000040U
#define CANFD_IMASK1_DMA_DONE0_S                                             6U
#define CANFD_IMASK1_DMA_DONE0_SET                                  0x00000040U
#define CANFD_IMASK1_DMA_DONE0_CLR                                  0x00000000U

// Field:     [5] FE2
//
// Filter event 2 interrupt mask for MIS1.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK1_FE2                                            0x00000020U
#define CANFD_IMASK1_FE2_M                                          0x00000020U
#define CANFD_IMASK1_FE2_S                                                   5U
#define CANFD_IMASK1_FE2_SET                                        0x00000020U
#define CANFD_IMASK1_FE2_CLR                                        0x00000000U

// Field:     [4] EXT_TS_OR_WAKE
//
// External Timestamp Counter Overflow interrupt mask for MIS1.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK1_EXT_TS_OR_WAKE                                 0x00000010U
#define CANFD_IMASK1_EXT_TS_OR_WAKE_M                               0x00000010U
#define CANFD_IMASK1_EXT_TS_OR_WAKE_S                                        4U
#define CANFD_IMASK1_EXT_TS_OR_WAKE_SET                             0x00000010U
#define CANFD_IMASK1_EXT_TS_OR_WAKE_CLR                             0x00000000U

// Field:     [3] DED
//
// Message RAM DED interrupt mask for MIS1.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK1_DED                                            0x00000008U
#define CANFD_IMASK1_DED_M                                          0x00000008U
#define CANFD_IMASK1_DED_S                                                   3U
#define CANFD_IMASK1_DED_SET                                        0x00000008U
#define CANFD_IMASK1_DED_CLR                                        0x00000000U

// Field:     [2] SEC
//
// Message RAM SEC interrupt mask for MIS1.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK1_SEC                                            0x00000004U
#define CANFD_IMASK1_SEC_M                                          0x00000004U
#define CANFD_IMASK1_SEC_S                                                   2U
#define CANFD_IMASK1_SEC_SET                                        0x00000004U
#define CANFD_IMASK1_SEC_CLR                                        0x00000000U

// Field:     [1] INTL1
//
// MCAN Interrupt Line1 mask for MIS1.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK1_INTL1                                          0x00000002U
#define CANFD_IMASK1_INTL1_M                                        0x00000002U
#define CANFD_IMASK1_INTL1_S                                                 1U
#define CANFD_IMASK1_INTL1_SET                                      0x00000002U
#define CANFD_IMASK1_INTL1_CLR                                      0x00000000U

// Field:     [0] INTL0
//
// MCAN Interrupt Line 0 mask for MIS1.
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define CANFD_IMASK1_INTL0                                          0x00000001U
#define CANFD_IMASK1_INTL0_M                                        0x00000001U
#define CANFD_IMASK1_INTL0_S                                                 0U
#define CANFD_IMASK1_INTL0_SET                                      0x00000001U
#define CANFD_IMASK1_INTL0_CLR                                      0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_RIS1
//
//*****************************************************************************
// Field:     [7] DMA_DONE1
//
// Raw Interrupt Status for DMA Done interrupt of DMA channel1.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS1_DMA_DONE1                                        0x00000080U
#define CANFD_RIS1_DMA_DONE1_M                                      0x00000080U
#define CANFD_RIS1_DMA_DONE1_S                                               7U
#define CANFD_RIS1_DMA_DONE1_SET                                    0x00000080U
#define CANFD_RIS1_DMA_DONE1_CLR                                    0x00000000U

// Field:     [6] DMA_DONE0
//
// Raw Interrupt Status for DMA Done interrupt of DMA channel0.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS1_DMA_DONE0                                        0x00000040U
#define CANFD_RIS1_DMA_DONE0_M                                      0x00000040U
#define CANFD_RIS1_DMA_DONE0_S                                               6U
#define CANFD_RIS1_DMA_DONE0_SET                                    0x00000040U
#define CANFD_RIS1_DMA_DONE0_CLR                                    0x00000000U

// Field:     [5] FE2
//
// Raw Interrupt Status for Filter Event 2 interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS1_FE2                                              0x00000020U
#define CANFD_RIS1_FE2_M                                            0x00000020U
#define CANFD_RIS1_FE2_S                                                     5U
#define CANFD_RIS1_FE2_SET                                          0x00000020U
#define CANFD_RIS1_FE2_CLR                                          0x00000000U

// Field:     [4] EXT_TS_OR_WAKE
//
// Raw Interrupt Status for External Timestamp Counter Overflow interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS1_EXT_TS_OR_WAKE                                   0x00000010U
#define CANFD_RIS1_EXT_TS_OR_WAKE_M                                 0x00000010U
#define CANFD_RIS1_EXT_TS_OR_WAKE_S                                          4U
#define CANFD_RIS1_EXT_TS_OR_WAKE_SET                               0x00000010U
#define CANFD_RIS1_EXT_TS_OR_WAKE_CLR                               0x00000000U

// Field:     [3] DED
//
// Raw Interrupt Status for Message RAM DED interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS1_DED                                              0x00000008U
#define CANFD_RIS1_DED_M                                            0x00000008U
#define CANFD_RIS1_DED_S                                                     3U
#define CANFD_RIS1_DED_SET                                          0x00000008U
#define CANFD_RIS1_DED_CLR                                          0x00000000U

// Field:     [2] SEC
//
// Raw Interrupt status for Message RAM SEC interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS1_SEC                                              0x00000004U
#define CANFD_RIS1_SEC_M                                            0x00000004U
#define CANFD_RIS1_SEC_S                                                     2U
#define CANFD_RIS1_SEC_SET                                          0x00000004U
#define CANFD_RIS1_SEC_CLR                                          0x00000000U

// Field:     [1] INTL1
//
// Raw Interrupt status for MCAN Interrupt Line 1.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS1_INTL1                                            0x00000002U
#define CANFD_RIS1_INTL1_M                                          0x00000002U
#define CANFD_RIS1_INTL1_S                                                   1U
#define CANFD_RIS1_INTL1_SET                                        0x00000002U
#define CANFD_RIS1_INTL1_CLR                                        0x00000000U

// Field:     [0] INTL0
//
// Raw Interrupt status for MCAN Interrupt Line 0.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_RIS1_INTL0                                            0x00000001U
#define CANFD_RIS1_INTL0_M                                          0x00000001U
#define CANFD_RIS1_INTL0_S                                                   0U
#define CANFD_RIS1_INTL0_SET                                        0x00000001U
#define CANFD_RIS1_INTL0_CLR                                        0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_MIS1
//
//*****************************************************************************
// Field:     [7] DMA_DONE1
//
// Mask interrupt status for DMA DONE interrupt of DMA channel1.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS1_DMA_DONE1                                        0x00000080U
#define CANFD_MIS1_DMA_DONE1_M                                      0x00000080U
#define CANFD_MIS1_DMA_DONE1_S                                               7U
#define CANFD_MIS1_DMA_DONE1_SET                                    0x00000080U
#define CANFD_MIS1_DMA_DONE1_CLR                                    0x00000000U

// Field:     [6] DMA_DONE0
//
// Mask interrupt status for DMA DONE interrupt of DMA channel0.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS1_DMA_DONE0                                        0x00000040U
#define CANFD_MIS1_DMA_DONE0_M                                      0x00000040U
#define CANFD_MIS1_DMA_DONE0_S                                               6U
#define CANFD_MIS1_DMA_DONE0_SET                                    0x00000040U
#define CANFD_MIS1_DMA_DONE0_CLR                                    0x00000000U

// Field:     [5] FE2
//
// Masked Interrupt status for Filter event 2 interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS1_FE2                                              0x00000020U
#define CANFD_MIS1_FE2_M                                            0x00000020U
#define CANFD_MIS1_FE2_S                                                     5U
#define CANFD_MIS1_FE2_SET                                          0x00000020U
#define CANFD_MIS1_FE2_CLR                                          0x00000000U

// Field:     [4] EXT_TS_OR_WAKE
//
// Masked Interrupt status for External Timestamp counter Overflow interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS1_EXT_TS_OR_WAKE                                   0x00000010U
#define CANFD_MIS1_EXT_TS_OR_WAKE_M                                 0x00000010U
#define CANFD_MIS1_EXT_TS_OR_WAKE_S                                          4U
#define CANFD_MIS1_EXT_TS_OR_WAKE_SET                               0x00000010U
#define CANFD_MIS1_EXT_TS_OR_WAKE_CLR                               0x00000000U

// Field:     [3] DED
//
// Masked Interrupt status for Message RAM DED interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS1_DED                                              0x00000008U
#define CANFD_MIS1_DED_M                                            0x00000008U
#define CANFD_MIS1_DED_S                                                     3U
#define CANFD_MIS1_DED_SET                                          0x00000008U
#define CANFD_MIS1_DED_CLR                                          0x00000000U

// Field:     [2] SEC
//
// Masked Interrupt status for Message RAM SEC interrupt.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS1_SEC                                              0x00000004U
#define CANFD_MIS1_SEC_M                                            0x00000004U
#define CANFD_MIS1_SEC_S                                                     2U
#define CANFD_MIS1_SEC_SET                                          0x00000004U
#define CANFD_MIS1_SEC_CLR                                          0x00000000U

// Field:     [1] INTL1
//
// Mask interrupt status for MCAN Interrupt Line 1.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS1_INTL1                                            0x00000002U
#define CANFD_MIS1_INTL1_M                                          0x00000002U
#define CANFD_MIS1_INTL1_S                                                   1U
#define CANFD_MIS1_INTL1_SET                                        0x00000002U
#define CANFD_MIS1_INTL1_CLR                                        0x00000000U

// Field:     [0] INTL0
//
// Mask interrupt status for MCAN Interrupt Line 0.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define CANFD_MIS1_INTL0                                            0x00000001U
#define CANFD_MIS1_INTL0_M                                          0x00000001U
#define CANFD_MIS1_INTL0_S                                                   0U
#define CANFD_MIS1_INTL0_SET                                        0x00000001U
#define CANFD_MIS1_INTL0_CLR                                        0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_ISET1
//
//*****************************************************************************
// Field:     [7] DMA_DONE1
//
// Sets DMA DONE for DMA channel 1 interrupt in RIS1.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET1_DMA_DONE1                                       0x00000080U
#define CANFD_ISET1_DMA_DONE1_M                                     0x00000080U
#define CANFD_ISET1_DMA_DONE1_S                                              7U
#define CANFD_ISET1_DMA_DONE1_SET                                   0x00000080U
#define CANFD_ISET1_DMA_DONE1_NO_EFFECT                             0x00000000U

// Field:     [6] DMA_DONE0
//
// Sets DMA DONE for DMA channel 0 interrupt in RIS1.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET1_DMA_DONE0                                       0x00000040U
#define CANFD_ISET1_DMA_DONE0_M                                     0x00000040U
#define CANFD_ISET1_DMA_DONE0_S                                              6U
#define CANFD_ISET1_DMA_DONE0_SET                                   0x00000040U
#define CANFD_ISET1_DMA_DONE0_NO_EFFECT                             0x00000000U

// Field:     [5] FE2
//
// Sets Filter event 2 interrupt in RIS1.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET1_FE2                                             0x00000020U
#define CANFD_ISET1_FE2_M                                           0x00000020U
#define CANFD_ISET1_FE2_S                                                    5U
#define CANFD_ISET1_FE2_SET                                         0x00000020U
#define CANFD_ISET1_FE2_NO_EFFECT                                   0x00000000U

// Field:     [4] EXT_TS_OR_WAKE
//
// Sets External Time stamp counter Overflow interrupt in RIS1.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET1_EXT_TS_OR_WAKE                                  0x00000010U
#define CANFD_ISET1_EXT_TS_OR_WAKE_M                                0x00000010U
#define CANFD_ISET1_EXT_TS_OR_WAKE_S                                         4U
#define CANFD_ISET1_EXT_TS_OR_WAKE_SET                              0x00000010U
#define CANFD_ISET1_EXT_TS_OR_WAKE_NO_EFFECT                        0x00000000U

// Field:     [3] DED
//
// Sets Message RAM DED interrupt in RIS1.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET1_DED                                             0x00000008U
#define CANFD_ISET1_DED_M                                           0x00000008U
#define CANFD_ISET1_DED_S                                                    3U
#define CANFD_ISET1_DED_SET                                         0x00000008U
#define CANFD_ISET1_DED_NO_EFFECT                                   0x00000000U

// Field:     [2] SEC
//
// Sets Message RAM SEC interrupt in RIS1.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET1_SEC                                             0x00000004U
#define CANFD_ISET1_SEC_M                                           0x00000004U
#define CANFD_ISET1_SEC_S                                                    2U
#define CANFD_ISET1_SEC_SET                                         0x00000004U
#define CANFD_ISET1_SEC_NO_EFFECT                                   0x00000000U

// Field:     [1] INTL1
//
// Sets MCAN Interrupt Line 1 interrupt in RIS1.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET1_INTL1                                           0x00000002U
#define CANFD_ISET1_INTL1_M                                         0x00000002U
#define CANFD_ISET1_INTL1_S                                                  1U
#define CANFD_ISET1_INTL1_SET                                       0x00000002U
#define CANFD_ISET1_INTL1_NO_EFFECT                                 0x00000000U

// Field:     [0] INTL0
//
// Sets MCAN Interrupt Line 0 interrupt in RIS1.
// ENUMs:
// SET                      Sets interrupt
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ISET1_INTL0                                           0x00000001U
#define CANFD_ISET1_INTL0_M                                         0x00000001U
#define CANFD_ISET1_INTL0_S                                                  0U
#define CANFD_ISET1_INTL0_SET                                       0x00000001U
#define CANFD_ISET1_INTL0_NO_EFFECT                                 0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_ICLR1
//
//*****************************************************************************
// Field:     [7] DMA_DONE1
//
// Clears DMA DONE interrupt for DMA channel 1 in RIS1.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR1_DMA_DONE1                                       0x00000080U
#define CANFD_ICLR1_DMA_DONE1_M                                     0x00000080U
#define CANFD_ICLR1_DMA_DONE1_S                                              7U
#define CANFD_ICLR1_DMA_DONE1_CLR                                   0x00000080U
#define CANFD_ICLR1_DMA_DONE1_NO_EFFECT                             0x00000000U

// Field:     [6] DMA_DONE0
//
// Clears DMA DONE interrupt for DMA channel 0 in RIS1.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR1_DMA_DONE0                                       0x00000040U
#define CANFD_ICLR1_DMA_DONE0_M                                     0x00000040U
#define CANFD_ICLR1_DMA_DONE0_S                                              6U
#define CANFD_ICLR1_DMA_DONE0_CLR                                   0x00000040U
#define CANFD_ICLR1_DMA_DONE0_NO_EFFECT                             0x00000000U

// Field:     [5] FE2
//
// Clears Filter Event 2  interrupt in RIS1.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR1_FE2                                             0x00000020U
#define CANFD_ICLR1_FE2_M                                           0x00000020U
#define CANFD_ICLR1_FE2_S                                                    5U
#define CANFD_ICLR1_FE2_CLR                                         0x00000020U
#define CANFD_ICLR1_FE2_NO_EFFECT                                   0x00000000U

// Field:     [4] EXT_TS_OR_WAKE
//
// Clears External Time stamp counter Overflow in RIS1.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR1_EXT_TS_OR_WAKE                                  0x00000010U
#define CANFD_ICLR1_EXT_TS_OR_WAKE_M                                0x00000010U
#define CANFD_ICLR1_EXT_TS_OR_WAKE_S                                         4U
#define CANFD_ICLR1_EXT_TS_OR_WAKE_CLR                              0x00000010U
#define CANFD_ICLR1_EXT_TS_OR_WAKE_NO_EFFECT                        0x00000000U

// Field:     [3] DED
//
// Clears Message RAM DED interrupt in RIS1.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR1_DED                                             0x00000008U
#define CANFD_ICLR1_DED_M                                           0x00000008U
#define CANFD_ICLR1_DED_S                                                    3U
#define CANFD_ICLR1_DED_CLR                                         0x00000008U
#define CANFD_ICLR1_DED_NO_EFFECT                                   0x00000000U

// Field:     [2] SEC
//
// Clears Message RAM SEC interrupt in RIS1.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR1_SEC                                             0x00000004U
#define CANFD_ICLR1_SEC_M                                           0x00000004U
#define CANFD_ICLR1_SEC_S                                                    2U
#define CANFD_ICLR1_SEC_CLR                                         0x00000004U
#define CANFD_ICLR1_SEC_NO_EFFECT                                   0x00000000U

// Field:     [1] INTL1
//
// Clears MCAN Interrupt Line 0 interrupt in RIS1.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR1_INTL1                                           0x00000002U
#define CANFD_ICLR1_INTL1_M                                         0x00000002U
#define CANFD_ICLR1_INTL1_S                                                  1U
#define CANFD_ICLR1_INTL1_CLR                                       0x00000002U
#define CANFD_ICLR1_INTL1_NO_EFFECT                                 0x00000000U

// Field:     [0] INTL0
//
// Clears MCAN Interrupt Line 0 interrupt in RIS1.
// ENUMs:
// CLR                      Clears the Event
// NO_EFFECT                Writing 0 has no effect
#define CANFD_ICLR1_INTL0                                           0x00000001U
#define CANFD_ICLR1_INTL0_M                                         0x00000001U
#define CANFD_ICLR1_INTL0_S                                                  0U
#define CANFD_ICLR1_INTL0_CLR                                       0x00000001U
#define CANFD_ICLR1_INTL0_NO_EFFECT                                 0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_CLKDIV
//
//*****************************************************************************
// Field:   [1:0] RATIO
//
// Clock divide ratio specification. Enables configuring clock divide settings
// for the MCAN functional clock input to the MCAN-SS.
// ENUMs:
// DIV_BY_4_                Divides input clock by 4
// DIV_BY_2_                Divides input clock by 2
// DIV_BY_1_                Divides input clock by 1
#define CANFD_MCANSS_CLKDIV_RATIO_W                                          2U
#define CANFD_MCANSS_CLKDIV_RATIO_M                                 0x00000003U
#define CANFD_MCANSS_CLKDIV_RATIO_S                                          0U
#define CANFD_MCANSS_CLKDIV_RATIO_DIV_BY_4_                         0x00000002U
#define CANFD_MCANSS_CLKDIV_RATIO_DIV_BY_2_                         0x00000001U
#define CANFD_MCANSS_CLKDIV_RATIO_DIV_BY_1_                         0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_CLKCTL
//
//*****************************************************************************
// Field:     [8] WKUP_GLTFLT_EN
//
// Setting this bit enables the glitch filter on MCAN RXD input, which wakes up
// the MCAN controller to exit clock gating.
// ENUMs:
// ENABLE                   Enable glitch filter enable on RXD input when MCAN
//                          is in clock stop mode (waiting for event on RXD
//                          input for clock stop wakeup).
// DISABLE                  Disable glitch filter enable on RXD input when
//                          MCAN is in clock stop mode (waiting for event
//                          on RXD input for clock stop wakeup).
#define CANFD_MCANSS_CLKCTL_WKUP_GLTFLT_EN                          0x00000100U
#define CANFD_MCANSS_CLKCTL_WKUP_GLTFLT_EN_M                        0x00000100U
#define CANFD_MCANSS_CLKCTL_WKUP_GLTFLT_EN_S                                 8U
#define CANFD_MCANSS_CLKCTL_WKUP_GLTFLT_EN_ENABLE                   0x00000100U
#define CANFD_MCANSS_CLKCTL_WKUP_GLTFLT_EN_DISABLE                  0x00000000U

// Field:     [4] WAKEUP_INT_EN
//
// This bit contols enabling or disabling the MCAN IP clock stop wakeup
// interrupt (when MCANSS_CTRL.WAKEUPREQEN wakeup request is enabled to wakeup
// MCAN IP upon CAN RXD activity)
// ENUMs:
// ENABLE                   Enable MCAN IP clock stop wakeup interrupt
// DISABLE                  Disable MCAN IP clock stop wakeup interrupt
#define CANFD_MCANSS_CLKCTL_WAKEUP_INT_EN                           0x00000010U
#define CANFD_MCANSS_CLKCTL_WAKEUP_INT_EN_M                         0x00000010U
#define CANFD_MCANSS_CLKCTL_WAKEUP_INT_EN_S                                  4U
#define CANFD_MCANSS_CLKCTL_WAKEUP_INT_EN_ENABLE                    0x00000010U
#define CANFD_MCANSS_CLKCTL_WAKEUP_INT_EN_DISABLE                   0x00000000U

// Field:     [0] STOPREQ
//
// This bit is used to enable/disable MCAN clock (both host clock and
// functional clock) gating request.
//
// Note: This bit can be reset by HW by Clock-Stop Wake-up via CAN RX Activity.
// See spec for more details.
// ENUMs:
// ENABLE                   Enable MCAN-SS clock stop request
// DISABLE                  Disable MCAN-SS clock stop request
#define CANFD_MCANSS_CLKCTL_STOPREQ                                 0x00000001U
#define CANFD_MCANSS_CLKCTL_STOPREQ_M                               0x00000001U
#define CANFD_MCANSS_CLKCTL_STOPREQ_S                                        0U
#define CANFD_MCANSS_CLKCTL_STOPREQ_ENABLE                          0x00000001U
#define CANFD_MCANSS_CLKCTL_STOPREQ_DISABLE                         0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_CLKSTS
//
//*****************************************************************************
// Field:     [4] STOPREQ_HW_OVR
//
// MCANSS clock stop HW override status bit.
//
// This bit indicates when the MCANSS_CLKCTL.STOPREQ bit has been cleared by HW
// when a clock-stop wake-up event via CAN RX activity is trigged.
// ENUMs:
// SET                      MCANSS_CLKCTL.STOPREQ bit has been cleared by HW.
// RESET                    MCANSS_CLKCTL.STOPREQ bit has not been cleared by
//                          HW.
#define CANFD_MCANSS_CLKSTS_STOPREQ_HW_OVR                          0x00000010U
#define CANFD_MCANSS_CLKSTS_STOPREQ_HW_OVR_M                        0x00000010U
#define CANFD_MCANSS_CLKSTS_STOPREQ_HW_OVR_S                                 4U
#define CANFD_MCANSS_CLKSTS_STOPREQ_HW_OVR_SET                      0x00000010U
#define CANFD_MCANSS_CLKSTS_STOPREQ_HW_OVR_RESET                    0x00000000U

// Field:     [0] CLKSTOP_ACKSTS
//
// Clock stop acknowledge status from MCAN IP
// ENUMs:
// SET                      MCAN-SS may be clock gated by stopping both the
//                          CAN host and functional clocks.
// RESET                    No clock stop acknowledged.
#define CANFD_MCANSS_CLKSTS_CLKSTOP_ACKSTS                          0x00000001U
#define CANFD_MCANSS_CLKSTS_CLKSTOP_ACKSTS_M                        0x00000001U
#define CANFD_MCANSS_CLKSTS_CLKSTOP_ACKSTS_S                                 0U
#define CANFD_MCANSS_CLKSTS_CLKSTOP_ACKSTS_SET                      0x00000001U
#define CANFD_MCANSS_CLKSTS_CLKSTOP_ACKSTS_RESET                    0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_DMA0_CTL
//
//*****************************************************************************
// Field: [31:27] RX_BUF_TTO_OFFST
//
// Indicates the Rx-buffer (index x) to be mapped to FE_0 (FE001) and
// automatically maps and Rx buffer (index x+1) to FE_1 (FE010)
// Valid range: Rxbuffer (0) to Rxbuffer (30)
// ENUMs:
// MAX                      Maximum index value: 30
//
//                          Note: RX_FE_TTO_SEL Rx
//                          buffer index selection for FE01 cannot be 31,
//                          as Rxbuffer (index +1) maps to 32, which
//                          requires updating NDAT1 and NDAT2 value, which
//                          is not supported.
// MIN                      Minimum index value: 0
#define CANFD_MCANSS_DMA0_CTL_RX_BUF_TTO_OFFST_W                             5U
#define CANFD_MCANSS_DMA0_CTL_RX_BUF_TTO_OFFST_M                    0xF8000000U
#define CANFD_MCANSS_DMA0_CTL_RX_BUF_TTO_OFFST_S                            27U
#define CANFD_MCANSS_DMA0_CTL_RX_BUF_TTO_OFFST_MAX                  0xF0000000U
#define CANFD_MCANSS_DMA0_CTL_RX_BUF_TTO_OFFST_MIN                  0x00000000U

// Field:    [24] RX_FE_OTO_SEL
//
// RX_FE_OTO_SEL is used to select the MCAN RX buffer filter event signal
// mapped to trigger fixed MCANSS DMA channel trigger
// ENUMs:
// FE_1                     Filter Event 1
// FE_0                     Filter Event 0
#define CANFD_MCANSS_DMA0_CTL_RX_FE_OTO_SEL                         0x01000000U
#define CANFD_MCANSS_DMA0_CTL_RX_FE_OTO_SEL_M                       0x01000000U
#define CANFD_MCANSS_DMA0_CTL_RX_FE_OTO_SEL_S                               24U
#define CANFD_MCANSS_DMA0_CTL_RX_FE_OTO_SEL_FE_1                    0x01000000U
#define CANFD_MCANSS_DMA0_CTL_RX_FE_OTO_SEL_FE_0                    0x00000000U

// Field: [21:16] TX_BRP_MTO_NUM
//
// Number of TX buffer request pending (BRP) signals for multi-to-one DMA
// trigger mapping sequence, starting from the buffer offset number selected by
// TX_BRP_MTO_OFFST bits
// ENUMs:
// MAX                      Max number for TX BRP multi-to-one DMA trigger
//                          mapping sequence is 32
// MIN                      Min number for TX BRP multi-to-one DMA trigger
//                          mapping sequence is 2
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_NUM_W                               6U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_NUM_M                      0x003F0000U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_NUM_S                              16U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_NUM_MAX                    0x00200000U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_NUM_MIN                    0x00020000U

// Field: [14:10] TX_BRP_MTO_OFFST
//
// TX_BRP_MTO_OFFST selects the Tx buffer offset number for the multi-to-one
// round robin DMA trigger mode.
// ENUMs:
// TX_BRP_31                TX Buffer Request Pending 31
// TX_BRP_30                TX Buffer Request Pending 30
// TX_BRP_29                TX Buffer Request Pending 29
// TX_BRP_28                TX Buffer Request Pending 28
// TX_BRP_27                TX Buffer Request Pending 27
// TX_BRP_26                TX Buffer Request Pending 26
// TX_BRP_25                TX Buffer Request Pending 25
// TX_BRP_24                TX Buffer Request Pending 24
// TX_BRP_23                TX Buffer Request Pending 23
// TX_BRP_22                TX Buffer Request Pending 22
// TX_BRP_21                TX Buffer Request Pending 21
// TX_BRP_20                TX Buffer Request Pending 20
// TX_BRP_19                TX Buffer Request Pending 19
// TX_BRP_18                TX Buffer Request Pending 18
// TX_BRP_17                TX Buffer Request Pending 17
// TX_BRP_16                TX Buffer Request Pending 16
// TX_BRP_15                TX Buffer Request Pending 15
// TX_BRP_14                TX Buffer Request Pending 14
// TX_BRP_13                TX Buffer Request Pending 13
// TX_BRP_12                TX Buffer Request Pending 12
// TX_BRP_11                TX Buffer Request Pending 11
// TX_BRP_10                TX Buffer Request Pending 10
// TX_BRP_9                 TX Buffer Request Pending 9
// TX_BRP_8                 TX Buffer Request Pending 8
// TX_BRP_7                 TX Buffer Request Pending 7
// TX_BRP_6                 TX Buffer Request Pending 6
// TX_BRP_5                 TX Buffer Request Pending 5
// TX_BRP_4                 TX Buffer Request Pending 4
// TX_BRP_3                 TX Buffer Request Pending 3
// TX_BRP_2                 TX Buffer Request Pending 2
// TX_BRP_1                 TX Buffer Request Pending 1
// TX_BRP_0                 TX Buffer Request Pending 0
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_W                             5U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_M                    0x00007C00U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_S                            10U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_31            0x00007C00U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_30            0x00007800U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_29            0x00007400U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_28            0x00007000U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_27            0x00006C00U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_26            0x00006800U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_25            0x00006400U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_24            0x00006000U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_23            0x00005C00U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_22            0x00005800U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_21            0x00005400U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_20            0x00005000U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_19            0x00004C00U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_18            0x00004800U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_17            0x00004400U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_16            0x00004000U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_15            0x00003C00U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_14            0x00003800U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_13            0x00003400U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_12            0x00003000U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_11            0x00002C00U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_10            0x00002800U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_9             0x00002400U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_8             0x00002000U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_7             0x00001C00U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_6             0x00001800U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_5             0x00001400U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_4             0x00001000U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_3             0x00000C00U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_2             0x00000800U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_1             0x00000400U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_MTO_OFFST_TX_BRP_0             0x00000000U

// Field:   [8:4] TX_BRP_OTO_SEL
//
// TX_BRP_OTO_SEL is used to select the MCAN TX buffer request pending (BRP)
// signal mapped to trigger fixed MCANSS DMA channel trigger
// ENUMs:
// TX_BRP_31                TX Buffer Request Pending 31
// TX_BRP_30                TX Buffer Request Pending 30
// TX_BRP_29                TX Buffer Request Pending 29
// TX_BRP_28                TX Buffer Request Pending 28
// TX_BRP_27                TX Buffer Request Pending 27
// TX_BRP_26                TX Buffer Request Pending 26
// TX_BRP_25                TX Buffer Request Pending 25
// TX_BRP_24                TX Buffer Request Pending 24
// TX_BRP_23                TX Buffer Request Pending 23
// TX_BRP_22                TX Buffer Request Pending 22
// TX_BRP_21                TX Buffer Request Pending 21
// TX_BRP_20                TX Buffer Request Pending 20
// TX_BRP_19                TX Buffer Request Pending 19
// TX_BRP_18                TX Buffer Request Pending 18
// TX_BRP_17                TX Buffer Request Pending 17
// TX_BRP_16                TX Buffer Request Pending 16
// TX_BRP_15                TX Buffer Request Pending 15
// TX_BRP_14                TX Buffer Request Pending 14
// TX_BRP_13                TX Buffer Request Pending 13
// TX_BRP_12                TX Buffer Request Pending 12
// TX_BRP_11                TX Buffer Request Pending 11
// TX_BRP_10                TX Buffer Request Pending 10
// TX_BRP_9                 TX Buffer Request Pending 9
// TX_BRP_8                 TX Buffer Request Pending 8
// TX_BRP_7                 TX Buffer Request Pending 7
// TX_BRP_6                 TX Buffer Request Pending 6
// TX_BRP_5                 TX Buffer Request Pending 5
// TX_BRP_4                 TX Buffer Request Pending 4
// TX_BRP_3                 TX Buffer Request Pending 3
// TX_BRP_2                 TX Buffer Request Pending 2
// TX_BRP_1                 TX Buffer Request Pending 1
// TX_BRP_0                 TX Buffer Request Pending 0
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_W                               5U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_M                      0x000001F0U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_S                               4U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_31              0x000001F0U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_30              0x000001E0U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_29              0x000001D0U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_28              0x000001C0U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_27              0x000001B0U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_26              0x000001A0U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_25              0x00000190U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_24              0x00000180U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_23              0x00000170U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_22              0x00000160U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_21              0x00000150U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_20              0x00000140U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_19              0x00000130U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_18              0x00000120U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_17              0x00000110U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_16              0x00000100U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_15              0x000000F0U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_14              0x000000E0U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_13              0x000000D0U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_12              0x000000C0U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_11              0x000000B0U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_10              0x000000A0U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_9               0x00000090U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_8               0x00000080U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_7               0x00000070U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_6               0x00000060U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_5               0x00000050U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_4               0x00000040U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_3               0x00000030U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_2               0x00000020U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_1               0x00000010U
#define CANFD_MCANSS_DMA0_CTL_TX_BRP_OTO_SEL_TX_BRP_0               0x00000000U

// Field:   [3:2] DMA_TRIG_SEL
//
// DMA trigger select bits used to select between MCAN TX one-to-one mapping,
// MCAN TX multi-to-one round robin mapping and MCAN Rx one-to-one mapping
// options
// ENUMs:
// RX_TTO_TRIG              Rx buffer two-to-one DMA trigger
// RX_OTO_TRIG              MCAN RX Buffer one-to-one Tx BRP (buffer request
//                          pending) trigger to DMA channel select
// TX_MTO_TRIG              MCAN TX Buffer multi-to-one round robin, Tx BRP
//                          (buffer request pending) triggers to DMA
//                          channel select
// TX_OTO_TRIG              MCAN TX Buffer one-to-one Tx BRP (buffer request
//                          pending) trigger to DMA channel select
#define CANFD_MCANSS_DMA0_CTL_DMA_TRIG_SEL_W                                 2U
#define CANFD_MCANSS_DMA0_CTL_DMA_TRIG_SEL_M                        0x0000000CU
#define CANFD_MCANSS_DMA0_CTL_DMA_TRIG_SEL_S                                 2U
#define CANFD_MCANSS_DMA0_CTL_DMA_TRIG_SEL_RX_TTO_TRIG              0x0000000CU
#define CANFD_MCANSS_DMA0_CTL_DMA_TRIG_SEL_RX_OTO_TRIG              0x00000008U
#define CANFD_MCANSS_DMA0_CTL_DMA_TRIG_SEL_TX_MTO_TRIG              0x00000004U
#define CANFD_MCANSS_DMA0_CTL_DMA_TRIG_SEL_TX_OTO_TRIG              0x00000000U

// Field:     [0] DMA_TRIG_EN
//
// DMA_TRIG_EN is used to enable/disable MCAN RX, TX triggers to MCANSS fixed
// DMA channel.
// <Note to design> check if this bit is needed depending on if similar
// functionality is enabled in the EXT_DMA aperture.
// ENUMs:
// ENABLE                   MCANSS fixed DMA channel trigger is enabled.
// DISABLE                  MCANSS fixed DMA channel trigger is disabled.
#define CANFD_MCANSS_DMA0_CTL_DMA_TRIG_EN                           0x00000001U
#define CANFD_MCANSS_DMA0_CTL_DMA_TRIG_EN_M                         0x00000001U
#define CANFD_MCANSS_DMA0_CTL_DMA_TRIG_EN_S                                  0U
#define CANFD_MCANSS_DMA0_CTL_DMA_TRIG_EN_ENABLE                    0x00000001U
#define CANFD_MCANSS_DMA0_CTL_DMA_TRIG_EN_DISABLE                   0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_MCANSS_DMA1_CTL
//
//*****************************************************************************
// Field: [31:27] RX_BUF_TTO_OFFST
//
// Indicates the Rx-buffer (index x) to be mapped to FE_0 (FE001) and
// automatically maps and Rx buffer (index x+1) to FE_1 (FE010)
// Valid range: Rxbuffer (0) to Rxbuffer (30)
// ENUMs:
// MAX                      Maximum index value: 30
//
//                          Note: RX_FE_TTO_SEL Rx
//                          buffer index selection for FE01 cannot be 31,
//                          as Rxbuffer (index +1) maps to 32, which
//                          requires updating NDAT1 and NDAT2 value, which
//                          is not supported.
// MIN                      Minimum index value: 0
#define CANFD_MCANSS_DMA1_CTL_RX_BUF_TTO_OFFST_W                             5U
#define CANFD_MCANSS_DMA1_CTL_RX_BUF_TTO_OFFST_M                    0xF8000000U
#define CANFD_MCANSS_DMA1_CTL_RX_BUF_TTO_OFFST_S                            27U
#define CANFD_MCANSS_DMA1_CTL_RX_BUF_TTO_OFFST_MAX                  0xF0000000U
#define CANFD_MCANSS_DMA1_CTL_RX_BUF_TTO_OFFST_MIN                  0x00000000U

// Field:    [24] RX_FE_OTO_SEL
//
// RX_FE_OTO_SEL is used to select the MCAN RX buffer filter event signal
// mapped to trigger fixed MCANSS DMA channel trigger
// ENUMs:
// FE_1                     Filter Event 1
// FE_0                     Filter Event 0
#define CANFD_MCANSS_DMA1_CTL_RX_FE_OTO_SEL                         0x01000000U
#define CANFD_MCANSS_DMA1_CTL_RX_FE_OTO_SEL_M                       0x01000000U
#define CANFD_MCANSS_DMA1_CTL_RX_FE_OTO_SEL_S                               24U
#define CANFD_MCANSS_DMA1_CTL_RX_FE_OTO_SEL_FE_1                    0x01000000U
#define CANFD_MCANSS_DMA1_CTL_RX_FE_OTO_SEL_FE_0                    0x00000000U

// Field: [21:16] TX_BRP_MTO_NUM
//
// Number of TX buffer request pending (BRP) signals for multi-to-one DMA
// trigger mapping sequence, starting from the buffer offset number selected by
// TX_BRP_MTO_OFFST bits
// ENUMs:
// MAX                      Max number for TX BRP multi-to-one DMA trigger
//                          mapping sequence is 32
// MIN                      Min number for TX BRP multi-to-one DMA trigger
//                          mapping sequence is 2
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_NUM_W                               6U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_NUM_M                      0x003F0000U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_NUM_S                              16U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_NUM_MAX                    0x00200000U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_NUM_MIN                    0x00020000U

// Field: [14:10] TX_BRP_MTO_OFFST
//
// TX_BRP_MTO_OFFST selects the Tx buffer offset number for the multi-to-one
// round robin DMA trigger mode.
// ENUMs:
// TX_BRP_31                TX Buffer Request Pending 31
// TX_BRP_30                TX Buffer Request Pending 30
// TX_BRP_29                TX Buffer Request Pending 29
// TX_BRP_28                TX Buffer Request Pending 28
// TX_BRP_27                TX Buffer Request Pending 27
// TX_BRP_26                TX Buffer Request Pending 26
// TX_BRP_25                TX Buffer Request Pending 25
// TX_BRP_24                TX Buffer Request Pending 24
// TX_BRP_23                TX Buffer Request Pending 23
// TX_BRP_22                TX Buffer Request Pending 22
// TX_BRP_21                TX Buffer Request Pending 21
// TX_BRP_20                TX Buffer Request Pending 20
// TX_BRP_19                TX Buffer Request Pending 19
// TX_BRP_18                TX Buffer Request Pending 18
// TX_BRP_17                TX Buffer Request Pending 17
// TX_BRP_16                TX Buffer Request Pending 16
// TX_BRP_15                TX Buffer Request Pending 15
// TX_BRP_14                TX Buffer Request Pending 14
// TX_BRP_13                TX Buffer Request Pending 13
// TX_BRP_12                TX Buffer Request Pending 12
// TX_BRP_11                TX Buffer Request Pending 11
// TX_BRP_10                TX Buffer Request Pending 10
// TX_BRP_9                 TX Buffer Request Pending 9
// TX_BRP_8                 TX Buffer Request Pending 8
// TX_BRP_7                 TX Buffer Request Pending 7
// TX_BRP_6                 TX Buffer Request Pending 6
// TX_BRP_5                 TX Buffer Request Pending 5
// TX_BRP_4                 TX Buffer Request Pending 4
// TX_BRP_3                 TX Buffer Request Pending 3
// TX_BRP_2                 TX Buffer Request Pending 2
// TX_BRP_1                 TX Buffer Request Pending 1
// TX_BRP_0                 TX Buffer Request Pending 0
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_W                             5U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_M                    0x00007C00U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_S                            10U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_31            0x00007C00U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_30            0x00007800U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_29            0x00007400U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_28            0x00007000U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_27            0x00006C00U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_26            0x00006800U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_25            0x00006400U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_24            0x00006000U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_23            0x00005C00U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_22            0x00005800U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_21            0x00005400U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_20            0x00005000U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_19            0x00004C00U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_18            0x00004800U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_17            0x00004400U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_16            0x00004000U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_15            0x00003C00U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_14            0x00003800U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_13            0x00003400U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_12            0x00003000U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_11            0x00002C00U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_10            0x00002800U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_9             0x00002400U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_8             0x00002000U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_7             0x00001C00U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_6             0x00001800U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_5             0x00001400U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_4             0x00001000U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_3             0x00000C00U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_2             0x00000800U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_1             0x00000400U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_MTO_OFFST_TX_BRP_0             0x00000000U

// Field:   [8:4] TX_BRP_OTO_SEL
//
// TX_BRP_OTO_SEL is used to select the MCAN TX buffer request pending (BRP)
// signal mapped to trigger fixed MCANSS DMA channel trigger
// ENUMs:
// TX_BRP_31                TX Buffer Request Pending 31
// TX_BRP_30                TX Buffer Request Pending 30
// TX_BRP_29                TX Buffer Request Pending 29
// TX_BRP_28                TX Buffer Request Pending 28
// TX_BRP_27                TX Buffer Request Pending 27
// TX_BRP_26                TX Buffer Request Pending 26
// TX_BRP_25                TX Buffer Request Pending 25
// TX_BRP_24                TX Buffer Request Pending 24
// TX_BRP_23                TX Buffer Request Pending 23
// TX_BRP_22                TX Buffer Request Pending 22
// TX_BRP_21                TX Buffer Request Pending 21
// TX_BRP_20                TX Buffer Request Pending 20
// TX_BRP_19                TX Buffer Request Pending 19
// TX_BRP_18                TX Buffer Request Pending 18
// TX_BRP_17                TX Buffer Request Pending 17
// TX_BRP_16                TX Buffer Request Pending 16
// TX_BRP_15                TX Buffer Request Pending 15
// TX_BRP_14                TX Buffer Request Pending 14
// TX_BRP_13                TX Buffer Request Pending 13
// TX_BRP_12                TX Buffer Request Pending 12
// TX_BRP_11                TX Buffer Request Pending 11
// TX_BRP_10                TX Buffer Request Pending 10
// TX_BRP_9                 TX Buffer Request Pending 9
// TX_BRP_8                 TX Buffer Request Pending 8
// TX_BRP_7                 TX Buffer Request Pending 7
// TX_BRP_6                 TX Buffer Request Pending 6
// TX_BRP_5                 TX Buffer Request Pending 5
// TX_BRP_4                 TX Buffer Request Pending 4
// TX_BRP_3                 TX Buffer Request Pending 3
// TX_BRP_2                 TX Buffer Request Pending 2
// TX_BRP_1                 TX Buffer Request Pending 1
// TX_BRP_0                 TX Buffer Request Pending 0
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_W                               5U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_M                      0x000001F0U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_S                               4U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_31              0x000001F0U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_30              0x000001E0U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_29              0x000001D0U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_28              0x000001C0U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_27              0x000001B0U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_26              0x000001A0U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_25              0x00000190U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_24              0x00000180U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_23              0x00000170U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_22              0x00000160U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_21              0x00000150U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_20              0x00000140U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_19              0x00000130U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_18              0x00000120U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_17              0x00000110U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_16              0x00000100U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_15              0x000000F0U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_14              0x000000E0U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_13              0x000000D0U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_12              0x000000C0U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_11              0x000000B0U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_10              0x000000A0U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_9               0x00000090U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_8               0x00000080U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_7               0x00000070U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_6               0x00000060U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_5               0x00000050U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_4               0x00000040U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_3               0x00000030U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_2               0x00000020U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_1               0x00000010U
#define CANFD_MCANSS_DMA1_CTL_TX_BRP_OTO_SEL_TX_BRP_0               0x00000000U

// Field:   [3:2] DMA_TRIG_SEL
//
// DMA trigger select bits used to select between MCAN TX one-to-one mapping,
// MCAN TX multi-to-one round robin mapping and MCAN Rx one-to-one mapping
// options
// ENUMs:
// RX_TTO_TRIG              Rx buffer two-to-one DMA trigger
// RX_OTO_TRIG              MCAN RX Buffer one-to-one Tx BRP (buffer request
//                          pending) trigger to DMA channel select
// TX_MTO_TRIG              MCAN TX Buffer multi-to-one round robin Tx BRP
//                          (buffer request pending) triggers to DMA
//                          channel select
// TX_OTO_TRIG              MCAN TX Buffer one-to-one Tx BRP (buffer request
//                          pending) trigger to DMA channel select
#define CANFD_MCANSS_DMA1_CTL_DMA_TRIG_SEL_W                                 2U
#define CANFD_MCANSS_DMA1_CTL_DMA_TRIG_SEL_M                        0x0000000CU
#define CANFD_MCANSS_DMA1_CTL_DMA_TRIG_SEL_S                                 2U
#define CANFD_MCANSS_DMA1_CTL_DMA_TRIG_SEL_RX_TTO_TRIG              0x0000000CU
#define CANFD_MCANSS_DMA1_CTL_DMA_TRIG_SEL_RX_OTO_TRIG              0x00000008U
#define CANFD_MCANSS_DMA1_CTL_DMA_TRIG_SEL_TX_MTO_TRIG              0x00000004U
#define CANFD_MCANSS_DMA1_CTL_DMA_TRIG_SEL_TX_OTO_TRIG              0x00000000U

// Field:     [0] DMA_TRIG_EN
//
// DMA_TRIG_EN is used to enable/disable MCAN RX, TX triggers to MCANSS fixed
// DMA channel.
// <Note to design> check if this bit is needed depending on if similar
// functionality is enabled in the EXT_DMA aperture.
// ENUMs:
// ENABLE                   MCANSS fixed DMA channel trigger is enabled.
// DISABLE                  MCANSS fixed DMA channel trigger is disabled.
#define CANFD_MCANSS_DMA1_CTL_DMA_TRIG_EN                           0x00000001U
#define CANFD_MCANSS_DMA1_CTL_DMA_TRIG_EN_M                         0x00000001U
#define CANFD_MCANSS_DMA1_CTL_DMA_TRIG_EN_S                                  0U
#define CANFD_MCANSS_DMA1_CTL_DMA_TRIG_EN_ENABLE                    0x00000001U
#define CANFD_MCANSS_DMA1_CTL_DMA_TRIG_EN_DISABLE                   0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_RXDMA_TTO_FE0_BA
//
//*****************************************************************************
// Field:  [14:2] BASE_ADDR
//
// FE0 Rx Buf x Base adddress (14:2).
// Address should be compited based on the 14-bit RBSA (Rx buffer start
// address) + offset (depending on Rx buffer element index value and data
// length code (DLC) for all the buffer elements before the Rx buffer element
// (x))
// ENUMs:
// MAX                      Max address offset within MCANSS SRAM: 0x1fff
// MIN                      Min address offset within MCANSS SRAM: 0x0
#define CANFD_RXDMA_TTO_FE0_BA_BASE_ADDR_W                                  13U
#define CANFD_RXDMA_TTO_FE0_BA_BASE_ADDR_M                          0x00007FFCU
#define CANFD_RXDMA_TTO_FE0_BA_BASE_ADDR_S                                   2U
#define CANFD_RXDMA_TTO_FE0_BA_BASE_ADDR_MAX                        0x00007FFCU
#define CANFD_RXDMA_TTO_FE0_BA_BASE_ADDR_MIN                        0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_RXDMA_TTO_FE1_BA
//
//*****************************************************************************
// Field:  [14:2] BASE_ADDR
//
// FE010 Rx Buf x Base adddress (14:2).
// Address should be compited based on the 14-bit RBSA (Rx buffer start
// address) + offset (depending on Rx buffer element index value and data
// length code (DLC) for all the buffer elements before the Rx buffer element
// (x+1))
// ENUMs:
// MAX                      Max address offset within MCANSS SRAM: 0x1fff
// MIN                      Min address offset within MCANSS SRAM: 0x0
#define CANFD_RXDMA_TTO_FE1_BA_BASE_ADDR_W                                  13U
#define CANFD_RXDMA_TTO_FE1_BA_BASE_ADDR_M                          0x00007FFCU
#define CANFD_RXDMA_TTO_FE1_BA_BASE_ADDR_S                                   2U
#define CANFD_RXDMA_TTO_FE1_BA_BASE_ADDR_MAX                        0x00007FFCU
#define CANFD_RXDMA_TTO_FE1_BA_BASE_ADDR_MIN                        0x00000000U

//*****************************************************************************
//
// Register: CANFD_O_RXDMA_TTO_NDAT1
//
//*****************************************************************************
// Field:  [31:0] NDAT1_VAL
//
// NDAT1 value to be programmed onto MCAN.NDAT1 MMR.
// Automatically updated by HW.
// ENUMs:
// MAX                      max value = (bit 31 set) = 0x80000000
// MIN                      Min value = 0x0 (not bits set)
#define CANFD_RXDMA_TTO_NDAT1_NDAT1_VAL_W                                   32U
#define CANFD_RXDMA_TTO_NDAT1_NDAT1_VAL_M                           0xFFFFFFFFU
#define CANFD_RXDMA_TTO_NDAT1_NDAT1_VAL_S                                    0U
#define CANFD_RXDMA_TTO_NDAT1_NDAT1_VAL_MAX                         0x80000000U
#define CANFD_RXDMA_TTO_NDAT1_NDAT1_VAL_MIN                         0x00000000U


#endif // __CANFD__
