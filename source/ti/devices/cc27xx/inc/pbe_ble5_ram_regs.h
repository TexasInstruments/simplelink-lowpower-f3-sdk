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
 *  ======== pbe_ble5_ram_regs.h ========
*/

#ifndef __PBE_BLE5_RAM_REGS_H
#define __PBE_BLE5_RAM_REGS_H

//******************************************************************************
// REGISTER OFFSETS
//******************************************************************************
// 
#define PBE_BLE5_RAM_O_PHY                                           0x00000020U

// 
#define PBE_BLE5_RAM_O_PRETXIFS500K                                  0x00000022U

// 
#define PBE_BLE5_RAM_O_TXIFS500KADJ                                  0x00000024U

// 
#define PBE_BLE5_RAM_O_PRETXIFS                                      0x00000026U

// 
#define PBE_BLE5_RAM_O_PRERXIFS                                      0x00000028U

// 
#define PBE_BLE5_RAM_O_RXTIMEOUT                                     0x0000002AU

// 
#define PBE_BLE5_RAM_O_SYNTHCALTIMEOUT                               0x0000002CU

// 
#define PBE_BLE5_RAM_O_RECALTIMEOUT                                  0x0000002EU

// Scanner and Intor use both. Advertiser uses only ADRMODE_PEERADR, it shall be unaffected by  ADRMODE_OWNADR.
#define PBE_BLE5_RAM_O_ADRMODE                                       0x00000030U

// Configure acceptance of unknown addresses for connectable advertising packets in initiators and advertisers 
#define PBE_BLE5_RAM_O_RPACONNECT                                    0x00000032U

// Configure FIFO usage
#define PBE_BLE5_RAM_O_FIFOCFG                                       0x00000034U

// Status for the received packet, appended in the RX FIFO depending on FIFOCFG.
#define PBE_BLE5_RAM_O_STATUSBYTE                                    0x00000036U

// 
#define PBE_BLE5_RAM_O_NAKHUB                                        0x00000038U

// Initialization for the whitener, if 0 the whitener is not used
#define PBE_BLE5_RAM_O_WHITEINIT                                     0x0000003AU

// Indicates how many bytes that are required after the PDU to hold the statuses configured in FIFOCFG. It has to be equal or greater.
#define PBE_BLE5_RAM_O_EXTRABYTES                                    0x0000003CU

// CRC initialization value Random by Link Layer for Data PDU, 0x555555 for Adv PDU.
#define PBE_BLE5_RAM_O_CRCINITL                                      0x00000040U

// CRC initialization value Random by Link Layer for Data PDU, 0x555555 for Adv PDU.
#define PBE_BLE5_RAM_O_CRCINITH                                      0x00000042U

// Sequencing status
#define PBE_BLE5_RAM_O_SEQSTAT                                       0x00000044U

// Back off count
#define PBE_BLE5_RAM_O_BACKOFFCNT                                    0x00000046U

// Configure scanner
#define PBE_BLE5_RAM_O_SCANCFG                                       0x00000048U

// Extended advertising configuration
#define PBE_BLE5_RAM_O_AECFG                                         0x0000004AU

// Temporary storage for AE flags
#define PBE_BLE5_RAM_O_AETMP                                         0x0000004CU

// Temporary storage for RT7_EXT_LENGTH
#define PBE_BLE5_RAM_O_EXTLENTMP                                     0x0000004EU

// WinOffset parameter to divide by 4 and write in CONNECT_IND message. This value will be decremented at each systimer event x until transmission of a CONNECT_REQ has started. If the value is decremented from 4, it is set to WINMOD.
#define PBE_BLE5_RAM_O_WINOFFSET                                     0x00000050U

// 
#define PBE_BLE5_RAM_O_WINMOD                                        0x00000052U

// Maximum number of payload bytes in a packet. Checked in OP_BLE_TX_RAW and OP_BLE_RX_RAW. PDU TYPE specific lengths are confirmed in addition.
#define PBE_BLE5_RAM_O_MAXLEN                                        0x00000054U

// Filter policies
#define PBE_BLE5_RAM_O_FILTPOLICY                                    0x00000056U

// Address modes
#define PBE_BLE5_RAM_O_OWNADRTYPE                                    0x00000058U

// Address mode for the ScanA in AUX_CONNECT_REQ
#define PBE_BLE5_RAM_O_SCANATYPE                                     0x0000005AU

// Address modes
#define PBE_BLE5_RAM_O_PEERADRTYPE                                   0x0000005CU

// Low part of own address, 16 bits
#define PBE_BLE5_RAM_O_OWNADRL                                       0x0000005EU

// Middle part of own address, 16 bits
#define PBE_BLE5_RAM_O_OWNADRM                                       0x00000060U

// High part of own address, 16 bits
#define PBE_BLE5_RAM_O_OWNADRH                                       0x00000062U

// Low part of scan address
#define PBE_BLE5_RAM_O_SCANADRL                                      0x00000064U

// Middle part of scan address
#define PBE_BLE5_RAM_O_SCANADRM                                      0x00000066U

// High part of scan address
#define PBE_BLE5_RAM_O_SCANADRH                                      0x00000068U

// 
#define PBE_BLE5_RAM_O_TMPADRL                                       0x0000006AU

// 
#define PBE_BLE5_RAM_O_TMPADRM                                       0x0000006CU

// 
#define PBE_BLE5_RAM_O_TMPADRH                                       0x0000006EU

// Low part of peer address
#define PBE_BLE5_RAM_O_PEERADRL                                      0x00000070U

// Middle part of peer address
#define PBE_BLE5_RAM_O_PEERADRM                                      0x00000072U

// High part of peer address
#define PBE_BLE5_RAM_O_PEERADRH                                      0x00000074U

// 
#define PBE_BLE5_RAM_O_NTXDONE                                       0x00000076U

// 
#define PBE_BLE5_RAM_O_NTXACK                                        0x00000078U

// 
#define PBE_BLE5_RAM_O_NTXCTLACK                                     0x0000007AU

// 
#define PBE_BLE5_RAM_O_NTXCTL                                        0x0000007CU

// 
#define PBE_BLE5_RAM_O_NTXRETRANS                                    0x0000007EU

// 
#define PBE_BLE5_RAM_O_NRXNOK                                        0x00000080U

// 
#define PBE_BLE5_RAM_O_NRXIGNORED                                    0x00000082U

// 
#define PBE_BLE5_RAM_O_NRXEMPTY                                      0x00000084U

// 
#define PBE_BLE5_RAM_O_NRXFIFOFULL                                   0x00000086U

// 
#define PBE_BLE5_RAM_O_NRXOK                                         0x00000088U

// 
#define PBE_BLE5_RAM_O_NTX                                           0x0000008AU

// 
#define PBE_BLE5_RAM_O_NRXCTL                                        0x0000008CU

// 
#define PBE_BLE5_RAM_O_NRXCTLACK                                     0x0000008EU

// 
#define PBE_BLE5_RAM_O_LASTRSSI                                      0x00000090U

// 
#define PBE_BLE5_RAM_O_FIRSTRXTIMEOUT                                0x00000092U

// 
#define PBE_BLE5_RAM_O_LASTTIMESTAMPL                                0x00000094U

// 
#define PBE_BLE5_RAM_O_LASTTIMESTAMPH                                0x00000096U

// 
#define PBE_BLE5_RAM_O_MDCFG                                         0x00000098U

// Configure Advertiser Task Behavior. This configuration determines the control flow of the advertiser command(OP_BLE_ADV) on the PBE_API. More than one bit may be high at the same time.
#define PBE_BLE5_RAM_O_ADVCFG                                        0x0000009AU

// Temporary register for storing RSSI
#define PBE_BLE5_RAM_O_TMPRSSI                                       0x0000009CU

// Temporary register
#define PBE_BLE5_RAM_O_TMPREG                                        0x0000009EU

// 
#define PBE_BLE5_RAM_O_FIRSTTIMESTAMPL                               0x000000A0U

// 
#define PBE_BLE5_RAM_O_FIRSTTIMESTAMPH                               0x000000A2U

// 
#define PBE_BLE5_RAM_O_FL1RESULT                                     0x000000A4U

// 
#define PBE_BLE5_RAM_O_FL1MASK                                       0x000000A6U

// 
#define PBE_BLE5_RAM_O_FL2RESULT                                     0x000000A8U

// 
#define PBE_BLE5_RAM_O_FL2MASK                                       0x000000AAU

// 
#define PBE_BLE5_RAM_O_FLSTAT                                        0x000000ACU

// 
#define PBE_BLE5_RAM_O_TMPATYPE                                      0x000000AEU

// 
#define PBE_BLE5_RAM_O_PATTERN                                       0x000000B0U

// 
#define PBE_BLE5_RAM_O_RFINTERVAL                                    0x000000B2U

// 
#define PBE_BLE5_RAM_O_NTXTARGET                                     0x000000B4U

// combined tx/rx/fs configuration
#define PBE_BLE5_RAM_O_OPCFG                                         0x000000B6U

// 
#define PBE_BLE5_RAM_O_FL1INFO0                                      0x000000B8U

// 
#define PBE_BLE5_RAM_O_FL1ADRL0                                      0x000000BAU

// 
#define PBE_BLE5_RAM_O_FL1ADRM0                                      0x000000BCU

// 
#define PBE_BLE5_RAM_O_FL1ADRH0                                      0x000000BEU

// 
#define PBE_BLE5_RAM_O_FL1INFO1                                      0x000000C0U

// 
#define PBE_BLE5_RAM_O_FL1ADRL1                                      0x000000C2U

// 
#define PBE_BLE5_RAM_O_FL1ADRM1                                      0x000000C4U

// 
#define PBE_BLE5_RAM_O_FL1ADRH1                                      0x000000C6U

// 
#define PBE_BLE5_RAM_O_FL1INFO2                                      0x000000C8U

// 
#define PBE_BLE5_RAM_O_FL1ADRL2                                      0x000000CAU

// 
#define PBE_BLE5_RAM_O_FL1ADRM2                                      0x000000CCU

// 
#define PBE_BLE5_RAM_O_FL1ADRH2                                      0x000000CEU

// 
#define PBE_BLE5_RAM_O_FL1INFO3                                      0x000000D0U

// 
#define PBE_BLE5_RAM_O_FL1ADRL3                                      0x000000D2U

// 
#define PBE_BLE5_RAM_O_FL1ADRM3                                      0x000000D4U

// 
#define PBE_BLE5_RAM_O_FL1ADRH3                                      0x000000D6U

// 
#define PBE_BLE5_RAM_O_FL1INFO4                                      0x000000D8U

// 
#define PBE_BLE5_RAM_O_FL1ADRL4                                      0x000000DAU

// 
#define PBE_BLE5_RAM_O_FL1ADRM4                                      0x000000DCU

// 
#define PBE_BLE5_RAM_O_FL1ADRH4                                      0x000000DEU

// 
#define PBE_BLE5_RAM_O_FL1INFO5                                      0x000000E0U

// 
#define PBE_BLE5_RAM_O_FL1ADRL5                                      0x000000E2U

// 
#define PBE_BLE5_RAM_O_FL1ADRM5                                      0x000000E4U

// 
#define PBE_BLE5_RAM_O_FL1ADRH5                                      0x000000E6U

// 
#define PBE_BLE5_RAM_O_FL1INFO6                                      0x000000E8U

// 
#define PBE_BLE5_RAM_O_FL1ADRL6                                      0x000000EAU

// 
#define PBE_BLE5_RAM_O_FL1ADRM6                                      0x000000ECU

// 
#define PBE_BLE5_RAM_O_FL1ADRH6                                      0x000000EEU

// 
#define PBE_BLE5_RAM_O_FL1INFO7                                      0x000000F0U

// 
#define PBE_BLE5_RAM_O_FL1ADRL7                                      0x000000F2U

// 
#define PBE_BLE5_RAM_O_FL1ADRM7                                      0x000000F4U

// 
#define PBE_BLE5_RAM_O_FL1ADRH7                                      0x000000F6U

// 
#define PBE_BLE5_RAM_O_FL1INFO8                                      0x000000F8U

// 
#define PBE_BLE5_RAM_O_FL1ADRL8                                      0x000000FAU

// 
#define PBE_BLE5_RAM_O_FL1ADRM8                                      0x000000FCU

// 
#define PBE_BLE5_RAM_O_FL1ADRH8                                      0x000000FEU

// 
#define PBE_BLE5_RAM_O_FL1INFO9                                      0x00000100U

// 
#define PBE_BLE5_RAM_O_FL1ADRL9                                      0x00000102U

// 
#define PBE_BLE5_RAM_O_FL1ADRM9                                      0x00000104U

// 
#define PBE_BLE5_RAM_O_FL1ADRH9                                      0x00000106U

// 
#define PBE_BLE5_RAM_O_FL1INFO10                                     0x00000108U

// 
#define PBE_BLE5_RAM_O_FL1ADRL10                                     0x0000010AU

// 
#define PBE_BLE5_RAM_O_FL1ADRM10                                     0x0000010CU

// 
#define PBE_BLE5_RAM_O_FL1ADRH10                                     0x0000010EU

// 
#define PBE_BLE5_RAM_O_FL1INFO11                                     0x00000110U

// 
#define PBE_BLE5_RAM_O_FL1ADRL11                                     0x00000112U

// 
#define PBE_BLE5_RAM_O_FL1ADRM11                                     0x00000114U

// 
#define PBE_BLE5_RAM_O_FL1ADRH11                                     0x00000116U

// 
#define PBE_BLE5_RAM_O_FL1INFO12                                     0x00000118U

// 
#define PBE_BLE5_RAM_O_FL1ADRL12                                     0x0000011AU

// 
#define PBE_BLE5_RAM_O_FL1ADRM12                                     0x0000011CU

// 
#define PBE_BLE5_RAM_O_FL1ADRH12                                     0x0000011EU

// 
#define PBE_BLE5_RAM_O_FL1INFO13                                     0x00000120U

// 
#define PBE_BLE5_RAM_O_FL1ADRL13                                     0x00000122U

// 
#define PBE_BLE5_RAM_O_FL1ADRM13                                     0x00000124U

// 
#define PBE_BLE5_RAM_O_FL1ADRH13                                     0x00000126U

// 
#define PBE_BLE5_RAM_O_FL1INFO14                                     0x00000128U

// 
#define PBE_BLE5_RAM_O_FL1ADRL14                                     0x0000012AU

// 
#define PBE_BLE5_RAM_O_FL1ADRM14                                     0x0000012CU

// 
#define PBE_BLE5_RAM_O_FL1ADRH14                                     0x0000012EU

// 
#define PBE_BLE5_RAM_O_FL1INFO15                                     0x00000130U

// 
#define PBE_BLE5_RAM_O_FL1ADRL15                                     0x00000132U

// 
#define PBE_BLE5_RAM_O_FL1ADRM15                                     0x00000134U

// 
#define PBE_BLE5_RAM_O_FL1ADRH15                                     0x00000136U

// 
#define PBE_BLE5_RAM_O_FL2INFO0                                      0x00000138U

// 
#define PBE_BLE5_RAM_O_FL2ADRL0                                      0x0000013AU

// 
#define PBE_BLE5_RAM_O_FL2ADRM0                                      0x0000013CU

// 
#define PBE_BLE5_RAM_O_FL2ADRH0                                      0x0000013EU

// 
#define PBE_BLE5_RAM_O_FL2INFO1                                      0x00000140U

// 
#define PBE_BLE5_RAM_O_FL2ADRL1                                      0x00000142U

// 
#define PBE_BLE5_RAM_O_FL2ADRM1                                      0x00000144U

// 
#define PBE_BLE5_RAM_O_FL2ADRH1                                      0x00000146U

// 
#define PBE_BLE5_RAM_O_FL2INFO2                                      0x00000148U

// 
#define PBE_BLE5_RAM_O_FL2ADRL2                                      0x0000014AU

// 
#define PBE_BLE5_RAM_O_FL2ADRM2                                      0x0000014CU

// 
#define PBE_BLE5_RAM_O_FL2ADRH2                                      0x0000014EU

// 
#define PBE_BLE5_RAM_O_FL2INFO3                                      0x00000150U

// 
#define PBE_BLE5_RAM_O_FL2ADRL3                                      0x00000152U

// 
#define PBE_BLE5_RAM_O_FL2ADRM3                                      0x00000154U

// 
#define PBE_BLE5_RAM_O_FL2ADRH3                                      0x00000156U

// 
#define PBE_BLE5_RAM_O_FL2INFO4                                      0x00000158U

// 
#define PBE_BLE5_RAM_O_FL2ADRL4                                      0x0000015AU

// 
#define PBE_BLE5_RAM_O_FL2ADRM4                                      0x0000015CU

// 
#define PBE_BLE5_RAM_O_FL2ADRH4                                      0x0000015EU

// 
#define PBE_BLE5_RAM_O_FL2INFO5                                      0x00000160U

// 
#define PBE_BLE5_RAM_O_FL2ADRL5                                      0x00000162U

// 
#define PBE_BLE5_RAM_O_FL2ADRM5                                      0x00000164U

// 
#define PBE_BLE5_RAM_O_FL2ADRH5                                      0x00000166U

// 
#define PBE_BLE5_RAM_O_FL2INFO6                                      0x00000168U

// 
#define PBE_BLE5_RAM_O_FL2ADRL6                                      0x0000016AU

// 
#define PBE_BLE5_RAM_O_FL2ADRM6                                      0x0000016CU

// 
#define PBE_BLE5_RAM_O_FL2ADRH6                                      0x0000016EU

// 
#define PBE_BLE5_RAM_O_FL2INFO7                                      0x00000170U

// 
#define PBE_BLE5_RAM_O_FL2ADRL7                                      0x00000172U

// 
#define PBE_BLE5_RAM_O_FL2ADRM7                                      0x00000174U

// 
#define PBE_BLE5_RAM_O_FL2ADRH7                                      0x00000176U

// 
#define PBE_BLE5_RAM_O_FL2INFO8                                      0x00000178U

// 
#define PBE_BLE5_RAM_O_FL2ADRL8                                      0x0000017AU

// 
#define PBE_BLE5_RAM_O_FL2ADRM8                                      0x0000017CU

// 
#define PBE_BLE5_RAM_O_FL2ADRH8                                      0x0000017EU

// 
#define PBE_BLE5_RAM_O_FL2INFO9                                      0x00000180U

// 
#define PBE_BLE5_RAM_O_FL2ADRL9                                      0x00000182U

// 
#define PBE_BLE5_RAM_O_FL2ADRM9                                      0x00000184U

// 
#define PBE_BLE5_RAM_O_FL2ADRH9                                      0x00000186U

// 
#define PBE_BLE5_RAM_O_FL2INFO10                                     0x00000188U

// 
#define PBE_BLE5_RAM_O_FL2ADRL10                                     0x0000018AU

// 
#define PBE_BLE5_RAM_O_FL2ADRM10                                     0x0000018CU

// 
#define PBE_BLE5_RAM_O_FL2ADRH10                                     0x0000018EU

// 
#define PBE_BLE5_RAM_O_FL2INFO11                                     0x00000190U

// 
#define PBE_BLE5_RAM_O_FL2ADRL11                                     0x00000192U

// 
#define PBE_BLE5_RAM_O_FL2ADRM11                                     0x00000194U

// 
#define PBE_BLE5_RAM_O_FL2ADRH11                                     0x00000196U

// 
#define PBE_BLE5_RAM_O_FL2INFO12                                     0x00000198U

// 
#define PBE_BLE5_RAM_O_FL2ADRL12                                     0x0000019AU

// 
#define PBE_BLE5_RAM_O_FL2ADRM12                                     0x0000019CU

// 
#define PBE_BLE5_RAM_O_FL2ADRH12                                     0x0000019EU

// 
#define PBE_BLE5_RAM_O_FL2INFO13                                     0x000001A0U

// 
#define PBE_BLE5_RAM_O_FL2ADRL13                                     0x000001A2U

// 
#define PBE_BLE5_RAM_O_FL2ADRM13                                     0x000001A4U

// 
#define PBE_BLE5_RAM_O_FL2ADRH13                                     0x000001A6U

// 
#define PBE_BLE5_RAM_O_FL2INFO14                                     0x000001A8U

// 
#define PBE_BLE5_RAM_O_FL2ADRL14                                     0x000001AAU

// 
#define PBE_BLE5_RAM_O_FL2ADRM14                                     0x000001ACU

// 
#define PBE_BLE5_RAM_O_FL2ADRH14                                     0x000001AEU

// 
#define PBE_BLE5_RAM_O_FL2INFO15                                     0x000001B0U

// 
#define PBE_BLE5_RAM_O_FL2ADRL15                                     0x000001B2U

// 
#define PBE_BLE5_RAM_O_FL2ADRM15                                     0x000001B4U

// 
#define PBE_BLE5_RAM_O_FL2ADRH15                                     0x000001B6U

//******************************************************************************
// Register: PHY
//******************************************************************************
// Field: [1:0] sel
//
// Used to pass on the value of the Coding Indicator,
#define PBE_BLE5_RAM_PHY_SEL_W                                                2U
#define PBE_BLE5_RAM_PHY_SEL_M                                           0x0003U
#define PBE_BLE5_RAM_PHY_SEL_S                                                0U
#define PBE_BLE5_RAM_PHY_SEL_1M                                          0x0000U
#define PBE_BLE5_RAM_PHY_SEL_2M                                          0x0001U
#define PBE_BLE5_RAM_PHY_SEL_CODED_125K                                  0x0002U
#define PBE_BLE5_RAM_PHY_SEL_CODED_500K                                  0x0003U

//******************************************************************************
// Register: PRETXIFS500K
//******************************************************************************
// Field: [15:0] val
//
// For bluetooth long range (BLR) and the 500k data rate only. Base value used after RX to set timer0 for when to start Transmission.
#define PBE_BLE5_RAM_PRETXIFS500K_VAL_W                                      16U
#define PBE_BLE5_RAM_PRETXIFS500K_VAL_M                                  0xFFFFU
#define PBE_BLE5_RAM_PRETXIFS500K_VAL_S                                       0U

//******************************************************************************
// Register: TXIFS500KADJ
//******************************************************************************
// Field: [15:0] val
//
// Short packets needs to adjust PRETXIFS500K to get consistent timing
#define PBE_BLE5_RAM_TXIFS500KADJ_VAL_W                                      16U
#define PBE_BLE5_RAM_TXIFS500KADJ_VAL_M                                  0xFFFFU
#define PBE_BLE5_RAM_TXIFS500KADJ_VAL_S                                       0U

//******************************************************************************
// Register: PRETXIFS
//******************************************************************************
// Field: [15:0] val
//
// Base value used after RX to set timer0 for when to start Transmission.
#define PBE_BLE5_RAM_PRETXIFS_VAL_W                                          16U
#define PBE_BLE5_RAM_PRETXIFS_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_PRETXIFS_VAL_S                                           0U

//******************************************************************************
// Register: PRERXIFS
//******************************************************************************
// Field: [15:0] val
//
// Base value used after TX to set timer0 for when to start Receiving.
#define PBE_BLE5_RAM_PRERXIFS_VAL_W                                          16U
#define PBE_BLE5_RAM_PRERXIFS_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_PRERXIFS_VAL_S                                           0U

//******************************************************************************
// Register: RXTIMEOUT
//******************************************************************************
// Field: [15:0] val
//
// Time to stay in RX before giving up sync search. Only used in T_IFS scenarios. If VAL = 0 then the PBE waits forever or until the CM0 manually interrupts.
#define PBE_BLE5_RAM_RXTIMEOUT_VAL_W                                         16U
#define PBE_BLE5_RAM_RXTIMEOUT_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_RXTIMEOUT_VAL_S                                          0U

//******************************************************************************
// Register: SYNTHCALTIMEOUT
//******************************************************************************
// Field: [15:0] val
//
// How many µs the PBE shall set its local timer to wait for the RFE synth calibration.
#define PBE_BLE5_RAM_SYNTHCALTIMEOUT_VAL_W                                   16U
#define PBE_BLE5_RAM_SYNTHCALTIMEOUT_VAL_M                               0xFFFFU
#define PBE_BLE5_RAM_SYNTHCALTIMEOUT_VAL_S                                    0U

//******************************************************************************
// Register: RECALTIMEOUT
//******************************************************************************
// Field: [15:0] val
//
// How many µs the PBE shall set its local timer to wait for the RFE synth re-calibration.
#define PBE_BLE5_RAM_RECALTIMEOUT_VAL_W                                      16U
#define PBE_BLE5_RAM_RECALTIMEOUT_VAL_M                                  0xFFFFU
#define PBE_BLE5_RAM_RECALTIMEOUT_VAL_S                                       0U

//******************************************************************************
// Register: ADRMODE
//******************************************************************************
// Field: [2:2] peeradr
//
// Filtering of peer (transmitter) address, may use the Filter Accept List
#define PBE_BLE5_RAM_ADRMODE_PEERADR                                     0x0004U
#define PBE_BLE5_RAM_ADRMODE_PEERADR_M                                   0x0004U
#define PBE_BLE5_RAM_ADRMODE_PEERADR_S                                        2U

//******************************************************************************
// Register: RPACONNECT
//******************************************************************************
// Field: [2:2] noreplace
//
// Replacement mode for TargetA in AUX_CONNECT_RSP
#define PBE_BLE5_RAM_RPACONNECT_NOREPLACE                                0x0004U
#define PBE_BLE5_RAM_RPACONNECT_NOREPLACE_M                              0x0004U
#define PBE_BLE5_RAM_RPACONNECT_NOREPLACE_S                                   2U

// Field: [1:1] endintor
//
// Configure Initiator ENDCAUSE status
#define PBE_BLE5_RAM_RPACONNECT_ENDINTOR                                 0x0002U
#define PBE_BLE5_RAM_RPACONNECT_ENDINTOR_M                               0x0002U
#define PBE_BLE5_RAM_RPACONNECT_ENDINTOR_S                                    1U

// Field: [0:0] endadv
//
// Configure Advertiser ENDCAUSE status.
#define PBE_BLE5_RAM_RPACONNECT_ENDADV                                   0x0001U
#define PBE_BLE5_RAM_RPACONNECT_ENDADV_M                                 0x0001U
#define PBE_BLE5_RAM_RPACONNECT_ENDADV_S                                      0U

//******************************************************************************
// Register: FIFOCFG
//******************************************************************************
// Field: [8:8] appendtimestamp
//
// 
#define PBE_BLE5_RAM_FIFOCFG_APPENDTIMESTAMP                             0x0100U
#define PBE_BLE5_RAM_FIFOCFG_APPENDTIMESTAMP_M                           0x0100U
#define PBE_BLE5_RAM_FIFOCFG_APPENDTIMESTAMP_S                                8U
#define PBE_BLE5_RAM_FIFOCFG_APPENDTIMESTAMP_DIS                         0x0000U
#define PBE_BLE5_RAM_FIFOCFG_APPENDTIMESTAMP_ENA                         0x0100U

// Field: [7:7] appendrssi
//
// 
#define PBE_BLE5_RAM_FIFOCFG_APPENDRSSI                                  0x0080U
#define PBE_BLE5_RAM_FIFOCFG_APPENDRSSI_M                                0x0080U
#define PBE_BLE5_RAM_FIFOCFG_APPENDRSSI_S                                     7U

// Field: [6:6] appendfreqest
//
// 
#define PBE_BLE5_RAM_FIFOCFG_APPENDFREQEST                               0x0040U
#define PBE_BLE5_RAM_FIFOCFG_APPENDFREQEST_M                             0x0040U
#define PBE_BLE5_RAM_FIFOCFG_APPENDFREQEST_S                                  6U

// Field: [5:5] appendlqi
//
// 
#define PBE_BLE5_RAM_FIFOCFG_APPENDLQI                                   0x0020U
#define PBE_BLE5_RAM_FIFOCFG_APPENDLQI_M                                 0x0020U
#define PBE_BLE5_RAM_FIFOCFG_APPENDLQI_S                                      5U

// Field: [4:4] appendstatus
//
// CRCOK, Ignore, Code rate
#define PBE_BLE5_RAM_FIFOCFG_APPENDSTATUS                                0x0010U
#define PBE_BLE5_RAM_FIFOCFG_APPENDSTATUS_M                              0x0010U
#define PBE_BLE5_RAM_FIFOCFG_APPENDSTATUS_S                                   4U

// Field: [3:3] appendcrc
//
// Append all received crc bits
#define PBE_BLE5_RAM_FIFOCFG_APPENDCRC                                   0x0008U
#define PBE_BLE5_RAM_FIFOCFG_APPENDCRC_M                                 0x0008U
#define PBE_BLE5_RAM_FIFOCFG_APPENDCRC_S                                      3U

// Field: [2:2] autoflushempty
//
// 
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHEMPTY                              0x0004U
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHEMPTY_M                            0x0004U
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHEMPTY_S                                 2U
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHEMPTY_KEEP                         0x0000U
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHEMPTY_FLUSH                        0x0004U

// Field: [1:1] autoflushign
//
// 
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHIGN                                0x0002U
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHIGN_M                              0x0002U
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHIGN_S                                   1U
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHIGN_KEEP                           0x0000U
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHIGN_FLUSH                          0x0002U

// Field: [0:0] autoflushcrc
//
// 
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHCRC                                0x0001U
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHCRC_M                              0x0001U
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHCRC_S                                   0U
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHCRC_KEEP                           0x0000U
#define PBE_BLE5_RAM_FIFOCFG_AUTOFLUSHCRC_FLUSH                          0x0001U

//******************************************************************************
// Register: STATUSBYTE
//******************************************************************************
// Field: [15:6] reserved
//
// Flags
#define PBE_BLE5_RAM_STATUSBYTE_RESERVED_W                                   10U
#define PBE_BLE5_RAM_STATUSBYTE_RESERVED_M                               0xFFC0U
#define PBE_BLE5_RAM_STATUSBYTE_RESERVED_S                                    6U

// Field: [5:5] syncinfoonly
//
// Ignored due to addrerss filtering result, but contains SyncInfo
#define PBE_BLE5_RAM_STATUSBYTE_SYNCINFOONLY                             0x0020U
#define PBE_BLE5_RAM_STATUSBYTE_SYNCINFOONLY_M                           0x0020U
#define PBE_BLE5_RAM_STATUSBYTE_SYNCINFOONLY_S                                5U

// Field: [4:4] adrignored
//
// Ignored due to an unknown address.
#define PBE_BLE5_RAM_STATUSBYTE_ADRIGNORED                               0x0010U
#define PBE_BLE5_RAM_STATUSBYTE_ADRIGNORED_M                             0x0010U
#define PBE_BLE5_RAM_STATUSBYTE_ADRIGNORED_S                                  4U

// Field: [3:3] ignored
//
// Ignored due to PID or address.
#define PBE_BLE5_RAM_STATUSBYTE_IGNORED                                  0x0008U
#define PBE_BLE5_RAM_STATUSBYTE_IGNORED_M                                0x0008U
#define PBE_BLE5_RAM_STATUSBYTE_IGNORED_S                                     3U

// Field: [2:2] crcerror
//
// CRC check with PHA failed.
#define PBE_BLE5_RAM_STATUSBYTE_CRCERROR                                 0x0004U
#define PBE_BLE5_RAM_STATUSBYTE_CRCERROR_M                               0x0004U
#define PBE_BLE5_RAM_STATUSBYTE_CRCERROR_S                                    2U

// Field: [1:0] phy
//
// aka Rate Indicator.
#define PBE_BLE5_RAM_STATUSBYTE_PHY_W                                         2U
#define PBE_BLE5_RAM_STATUSBYTE_PHY_M                                    0x0003U
#define PBE_BLE5_RAM_STATUSBYTE_PHY_S                                         0U
#define PBE_BLE5_RAM_STATUSBYTE_PHY_1M                                   0x0000U
#define PBE_BLE5_RAM_STATUSBYTE_PHY_2M                                   0x0001U
#define PBE_BLE5_RAM_STATUSBYTE_PHY_CODED_125K                           0x0002U
#define PBE_BLE5_RAM_STATUSBYTE_PHY_CODED_500K                           0x0003U

//******************************************************************************
// Register: NAKHUB
//******************************************************************************
// Field: [5:3] nrnaks
//
// How many consecutive NAKS has been received
#define PBE_BLE5_RAM_NAKHUB_NRNAKS_W                                          3U
#define PBE_BLE5_RAM_NAKHUB_NRNAKS_M                                     0x0038U
#define PBE_BLE5_RAM_NAKHUB_NRNAKS_S                                          3U

// Field: [2:0] maxnak
//
// Maximum number of packets received without a new value of NESN in the same task before the task ends. If both devices find themselves with full RX FIFO or no RX buffer available, they will request the peer to retransmit by transmitting a NAK. This is a sort of livelock loop. If it's 0, do not end the task based on this.
#define PBE_BLE5_RAM_NAKHUB_MAXNAK_W                                          3U
#define PBE_BLE5_RAM_NAKHUB_MAXNAK_M                                     0x0007U
#define PBE_BLE5_RAM_NAKHUB_MAXNAK_S                                          0U

//******************************************************************************
// Register: WHITEINIT
//******************************************************************************
// Field: [15:0] val
//
// Whitener initialization value
#define PBE_BLE5_RAM_WHITEINIT_VAL_W                                         16U
#define PBE_BLE5_RAM_WHITEINIT_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_WHITEINIT_VAL_S                                          0U

//******************************************************************************
// Register: EXTRABYTES
//******************************************************************************
// Field: [15:0] val
//
// The number of bytes required.
#define PBE_BLE5_RAM_EXTRABYTES_VAL_W                                        16U
#define PBE_BLE5_RAM_EXTRABYTES_VAL_M                                    0xFFFFU
#define PBE_BLE5_RAM_EXTRABYTES_VAL_S                                         0U

//******************************************************************************
// Register: CRCINITL
//******************************************************************************
// Field: [15:0] vallsb
//
// 
#define PBE_BLE5_RAM_CRCINITL_VALLSB_W                                       16U
#define PBE_BLE5_RAM_CRCINITL_VALLSB_M                                   0xFFFFU
#define PBE_BLE5_RAM_CRCINITL_VALLSB_S                                        0U

//******************************************************************************
// Register: CRCINITH
//******************************************************************************
// Field: [15:0] valmsb
//
// 
#define PBE_BLE5_RAM_CRCINITH_VALMSB_W                                       16U
#define PBE_BLE5_RAM_CRCINITH_VALMSB_M                                   0xFFFFU
#define PBE_BLE5_RAM_CRCINITH_VALMSB_S                                        0U

//******************************************************************************
// Register: SEQSTAT
//******************************************************************************
// Field: [7:7] ctlackpend
//
// The last successfully received packet was an LL control packet which has not yet been ACK'ed
#define PBE_BLE5_RAM_SEQSTAT_CTLACKPEND                                  0x0080U
#define PBE_BLE5_RAM_SEQSTAT_CTLACKPEND_M                                0x0080U
#define PBE_BLE5_RAM_SEQSTAT_CTLACKPEND_S                                     7U
#define PBE_BLE5_RAM_SEQSTAT_CTLACKPEND_NO                               0x0000U
#define PBE_BLE5_RAM_SEQSTAT_CTLACKPEND_YES                              0x0080U

// Field: [6:6] ctlack
//
// The last received packet was the ACK of an LL control packet
#define PBE_BLE5_RAM_SEQSTAT_CTLACK                                      0x0040U
#define PBE_BLE5_RAM_SEQSTAT_CTLACK_M                                    0x0040U
#define PBE_BLE5_RAM_SEQSTAT_CTLACK_S                                         6U
#define PBE_BLE5_RAM_SEQSTAT_CTLACK_NO                                   0x0000U
#define PBE_BLE5_RAM_SEQSTAT_CTLACK_YES                                  0x0040U

// Field: [5:5] ctltx
//
// The last transmitted packet was an LL control packet (LLID = 11)
#define PBE_BLE5_RAM_SEQSTAT_CTLTX                                       0x0020U
#define PBE_BLE5_RAM_SEQSTAT_CTLTX_M                                     0x0020U
#define PBE_BLE5_RAM_SEQSTAT_CTLTX_S                                          5U
#define PBE_BLE5_RAM_SEQSTAT_CTLTX_NO                                    0x0000U
#define PBE_BLE5_RAM_SEQSTAT_CTLTX_YES                                   0x0020U

// Field: [4:4] empty
//
// The last transmitted packet was an auto-empty packet.
#define PBE_BLE5_RAM_SEQSTAT_EMPTY                                       0x0010U
#define PBE_BLE5_RAM_SEQSTAT_EMPTY_M                                     0x0010U
#define PBE_BLE5_RAM_SEQSTAT_EMPTY_S                                          4U

// Field: [3:3] firstpkt
//
// Has there been any packets transmitted on this connection
#define PBE_BLE5_RAM_SEQSTAT_FIRSTPKT                                    0x0008U
#define PBE_BLE5_RAM_SEQSTAT_FIRSTPKT_M                                  0x0008U
#define PBE_BLE5_RAM_SEQSTAT_FIRSTPKT_S                                       3U
#define PBE_BLE5_RAM_SEQSTAT_FIRSTPKT_NOT_FIRST                          0x0000U
#define PBE_BLE5_RAM_SEQSTAT_FIRSTPKT_FIRST                              0x0008U

// Field: [2:2] nexttxsn
//
// SN in the next message to transmit.
#define PBE_BLE5_RAM_SEQSTAT_NEXTTXSN                                    0x0004U
#define PBE_BLE5_RAM_SEQSTAT_NEXTTXSN_M                                  0x0004U
#define PBE_BLE5_RAM_SEQSTAT_NEXTTXSN_S                                       2U

// Field: [1:1] lasttxsn
//
// SN in the last message transmitted.
#define PBE_BLE5_RAM_SEQSTAT_LASTTXSN                                    0x0002U
#define PBE_BLE5_RAM_SEQSTAT_LASTTXSN_M                                  0x0002U
#define PBE_BLE5_RAM_SEQSTAT_LASTTXSN_S                                       1U

// Field: [0:0] lastrxsn
//
// SN in the last message received with CRC OK.
#define PBE_BLE5_RAM_SEQSTAT_LASTRXSN                                    0x0001U
#define PBE_BLE5_RAM_SEQSTAT_LASTRXSN_M                                  0x0001U
#define PBE_BLE5_RAM_SEQSTAT_LASTRXSN_S                                       0U

//******************************************************************************
// Register: BACKOFFCNT
//******************************************************************************
// Field: [15:0] val
//
// MCU writes new value of back-off count for first scanner task after transmission of SCAN_REQ and subsequent reception or attempted reception of SCAN_RSP, or returned value from last scanner task. PBE decrements the back-off count on every correctly received advertising packet that will generate a SCAN_RSP and writes it back at the end of the task. Allowed values: write from MCU: 1–256. Read from MCU: 0–256.
#define PBE_BLE5_RAM_BACKOFFCNT_VAL_W                                        16U
#define PBE_BLE5_RAM_BACKOFFCNT_VAL_M                                    0xFFFFU
#define PBE_BLE5_RAM_BACKOFFCNT_VAL_S                                         0U

//******************************************************************************
// Register: SCANCFG
//******************************************************************************
// Field: [1:1] pawr
//
// PAwR features
#define PBE_BLE5_RAM_SCANCFG_PAWR                                        0x0002U
#define PBE_BLE5_RAM_SCANCFG_PAWR_M                                      0x0002U
#define PBE_BLE5_RAM_SCANCFG_PAWR_S                                           1U
#define PBE_BLE5_RAM_SCANCFG_PAWR_DISABLED                               0x0000U
#define PBE_BLE5_RAM_SCANCFG_PAWR_ENABLED                                0x0002U

// Field: [0:0] actpass
//
// Configure scan type
#define PBE_BLE5_RAM_SCANCFG_ACTPASS                                     0x0001U
#define PBE_BLE5_RAM_SCANCFG_ACTPASS_M                                   0x0001U
#define PBE_BLE5_RAM_SCANCFG_ACTPASS_S                                        0U
#define PBE_BLE5_RAM_SCANCFG_ACTPASS_PASS                                0x0000U
#define PBE_BLE5_RAM_SCANCFG_ACTPASS_ACT                                 0x0001U

//******************************************************************************
// Register: AECFG
//******************************************************************************
// Field: [5:5] advmode2
//
// Accept extended advertising packets with ADV_MODE=2
#define PBE_BLE5_RAM_AECFG_ADVMODE2                                      0x0020U
#define PBE_BLE5_RAM_AECFG_ADVMODE2_M                                    0x0020U
#define PBE_BLE5_RAM_AECFG_ADVMODE2_S                                         5U
#define PBE_BLE5_RAM_AECFG_ADVMODE2_REJECT                               0x0000U
#define PBE_BLE5_RAM_AECFG_ADVMODE2_ACCEPT                               0x0020U

// Field: [4:4] advmode1
//
// Accept extended advertising packets with ADV_MODE=1
#define PBE_BLE5_RAM_AECFG_ADVMODE1                                      0x0010U
#define PBE_BLE5_RAM_AECFG_ADVMODE1_M                                    0x0010U
#define PBE_BLE5_RAM_AECFG_ADVMODE1_S                                         4U
#define PBE_BLE5_RAM_AECFG_ADVMODE1_REJECT                               0x0000U
#define PBE_BLE5_RAM_AECFG_ADVMODE1_ACCEPT                               0x0010U

// Field: [3:3] advmode0
//
// Accept extended advertising packets with ADV_MODE=0
#define PBE_BLE5_RAM_AECFG_ADVMODE0                                      0x0008U
#define PBE_BLE5_RAM_AECFG_ADVMODE0_M                                    0x0008U
#define PBE_BLE5_RAM_AECFG_ADVMODE0_S                                         3U
#define PBE_BLE5_RAM_AECFG_ADVMODE0_REJECT                               0x0000U
#define PBE_BLE5_RAM_AECFG_ADVMODE0_ACCEPT                               0x0008U

// Field: [2:2] chnl
//
// Current channel type
#define PBE_BLE5_RAM_AECFG_CHNL                                          0x0004U
#define PBE_BLE5_RAM_AECFG_CHNL_M                                        0x0004U
#define PBE_BLE5_RAM_AECFG_CHNL_S                                             2U
#define PBE_BLE5_RAM_AECFG_CHNL_PRIMARY                                  0x0000U
#define PBE_BLE5_RAM_AECFG_CHNL_SECONDARY                                0x0004U

// Field: [1:1] extended
//
// Extended advertisement enabled
#define PBE_BLE5_RAM_AECFG_EXTENDED                                      0x0002U
#define PBE_BLE5_RAM_AECFG_EXTENDED_M                                    0x0002U
#define PBE_BLE5_RAM_AECFG_EXTENDED_S                                         1U
#define PBE_BLE5_RAM_AECFG_EXTENDED_DIS                                  0x0000U
#define PBE_BLE5_RAM_AECFG_EXTENDED_EN                                   0x0002U

// Field: [0:0] legacy
//
// Legacy advertisement enabled
#define PBE_BLE5_RAM_AECFG_LEGACY                                        0x0001U
#define PBE_BLE5_RAM_AECFG_LEGACY_M                                      0x0001U
#define PBE_BLE5_RAM_AECFG_LEGACY_S                                           0U
#define PBE_BLE5_RAM_AECFG_LEGACY_DIS                                    0x0000U
#define PBE_BLE5_RAM_AECFG_LEGACY_EN                                     0x0001U

//******************************************************************************
// Register: AETMP
//******************************************************************************
// Field: [9:9] scannable
//
// 
#define PBE_BLE5_RAM_AETMP_SCANNABLE                                     0x0200U
#define PBE_BLE5_RAM_AETMP_SCANNABLE_M                                   0x0200U
#define PBE_BLE5_RAM_AETMP_SCANNABLE_S                                        9U

// Field: [8:8] connectable
//
// 
#define PBE_BLE5_RAM_AETMP_CONNECTABLE                                   0x0100U
#define PBE_BLE5_RAM_AETMP_CONNECTABLE_M                                 0x0100U
#define PBE_BLE5_RAM_AETMP_CONNECTABLE_S                                      8U

// Field: [7:7] res
//
// 
#define PBE_BLE5_RAM_AETMP_RES                                           0x0080U
#define PBE_BLE5_RAM_AETMP_RES_M                                         0x0080U
#define PBE_BLE5_RAM_AETMP_RES_S                                              7U

// Field: [6:6] txpower
//
// 
#define PBE_BLE5_RAM_AETMP_TXPOWER                                       0x0040U
#define PBE_BLE5_RAM_AETMP_TXPOWER_M                                     0x0040U
#define PBE_BLE5_RAM_AETMP_TXPOWER_S                                          6U

// Field: [5:5] syncinfo
//
// 
#define PBE_BLE5_RAM_AETMP_SYNCINFO                                      0x0020U
#define PBE_BLE5_RAM_AETMP_SYNCINFO_M                                    0x0020U
#define PBE_BLE5_RAM_AETMP_SYNCINFO_S                                         5U

// Field: [4:4] auxptr
//
// 
#define PBE_BLE5_RAM_AETMP_AUXPTR                                        0x0010U
#define PBE_BLE5_RAM_AETMP_AUXPTR_M                                      0x0010U
#define PBE_BLE5_RAM_AETMP_AUXPTR_S                                           4U

// Field: [3:3] adi
//
// 
#define PBE_BLE5_RAM_AETMP_ADI                                           0x0008U
#define PBE_BLE5_RAM_AETMP_ADI_M                                         0x0008U
#define PBE_BLE5_RAM_AETMP_ADI_S                                              3U

// Field: [2:2] cteinfo
//
// 
#define PBE_BLE5_RAM_AETMP_CTEINFO                                       0x0004U
#define PBE_BLE5_RAM_AETMP_CTEINFO_M                                     0x0004U
#define PBE_BLE5_RAM_AETMP_CTEINFO_S                                          2U

// Field: [1:1] targeta
//
// 
#define PBE_BLE5_RAM_AETMP_TARGETA                                       0x0002U
#define PBE_BLE5_RAM_AETMP_TARGETA_M                                     0x0002U
#define PBE_BLE5_RAM_AETMP_TARGETA_S                                          1U

// Field: [0:0] adva
//
// 
#define PBE_BLE5_RAM_AETMP_ADVA                                          0x0001U
#define PBE_BLE5_RAM_AETMP_ADVA_M                                        0x0001U
#define PBE_BLE5_RAM_AETMP_ADVA_S                                             0U

//******************************************************************************
// Register: EXTLENTMP
//******************************************************************************
// Field: [15:0] val
//
// 
#define PBE_BLE5_RAM_EXTLENTMP_VAL_W                                         16U
#define PBE_BLE5_RAM_EXTLENTMP_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_EXTLENTMP_VAL_S                                          0U

//******************************************************************************
// Register: WINOFFSET
//******************************************************************************
// Field: [15:0] val
//
// The RCL intializes this to the range 4-WINMOD at the start of the intiator operation.
#define PBE_BLE5_RAM_WINOFFSET_VAL_W                                         16U
#define PBE_BLE5_RAM_WINOFFSET_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_WINOFFSET_VAL_S                                          0U

//******************************************************************************
// Register: WINMOD
//******************************************************************************
// Field: [15:0] val
//
// The amount of 312.5 us periods in the connection interval (four times the number given by the BLE LL). A value of 0 means that dynamic window offset is disabled.
#define PBE_BLE5_RAM_WINMOD_VAL_W                                            16U
#define PBE_BLE5_RAM_WINMOD_VAL_M                                        0xFFFFU
#define PBE_BLE5_RAM_WINMOD_VAL_S                                             0U

//******************************************************************************
// Register: MAXLEN
//******************************************************************************
// Field: [15:0] val
//
// 
#define PBE_BLE5_RAM_MAXLEN_VAL_W                                            16U
#define PBE_BLE5_RAM_MAXLEN_VAL_M                                        0xFFFFU
#define PBE_BLE5_RAM_MAXLEN_VAL_S                                             0U

//******************************************************************************
// Register: FILTPOLICY
//******************************************************************************
// Field: [5:5] syncinfo
//
// Keep packets with SyncInfo even if address check fails
#define PBE_BLE5_RAM_FILTPOLICY_SYNCINFO                                 0x0020U
#define PBE_BLE5_RAM_FILTPOLICY_SYNCINFO_M                               0x0020U
#define PBE_BLE5_RAM_FILTPOLICY_SYNCINFO_S                                    5U
#define PBE_BLE5_RAM_FILTPOLICY_SYNCINFO_FLUSH                           0x0000U
#define PBE_BLE5_RAM_FILTPOLICY_SYNCINFO_MARK                            0x0020U

// Field: [4:4] intor
//
// Initiator's filter policy
#define PBE_BLE5_RAM_FILTPOLICY_INTOR                                    0x0010U
#define PBE_BLE5_RAM_FILTPOLICY_INTOR_M                                  0x0010U
#define PBE_BLE5_RAM_FILTPOLICY_INTOR_S                                       4U
#define PBE_BLE5_RAM_FILTPOLICY_INTOR_PEERADR                            0x0000U
#define PBE_BLE5_RAM_FILTPOLICY_INTOR_FL2                                0x0010U

// Field: [3:3] extscanner
//
// Extended scanner filter policy.
#define PBE_BLE5_RAM_FILTPOLICY_EXTSCANNER                               0x0008U
#define PBE_BLE5_RAM_FILTPOLICY_EXTSCANNER_M                             0x0008U
#define PBE_BLE5_RAM_FILTPOLICY_EXTSCANNER_S                                  3U
#define PBE_BLE5_RAM_FILTPOLICY_EXTSCANNER_NORPA                         0x0000U
#define PBE_BLE5_RAM_FILTPOLICY_EXTSCANNER_ANYRPA                        0x0008U

// Field: [2:2] scanner
//
// Scanner's filter policy
#define PBE_BLE5_RAM_FILTPOLICY_SCANNER                                  0x0004U
#define PBE_BLE5_RAM_FILTPOLICY_SCANNER_M                                0x0004U
#define PBE_BLE5_RAM_FILTPOLICY_SCANNER_S                                     2U
#define PBE_BLE5_RAM_FILTPOLICY_SCANNER_REJFL1                           0x0000U
#define PBE_BLE5_RAM_FILTPOLICY_SCANNER_FL1                              0x0004U

// Field: [1:0] adv
//
// Advertiser's filter policy. Note: The Advertiser's filter policy is ignored during all directed advertiser events, all combinations.
#define PBE_BLE5_RAM_FILTPOLICY_ADV_W                                         2U
#define PBE_BLE5_RAM_FILTPOLICY_ADV_M                                    0x0003U
#define PBE_BLE5_RAM_FILTPOLICY_ADV_S                                         0U
#define PBE_BLE5_RAM_FILTPOLICY_ADV_REJFL1_REJFL2                        0x0000U
#define PBE_BLE5_RAM_FILTPOLICY_ADV_FL1_REJFL2                           0x0001U
#define PBE_BLE5_RAM_FILTPOLICY_ADV_REJFL1_FL2                           0x0002U
#define PBE_BLE5_RAM_FILTPOLICY_ADV_FL1_FL2                              0x0003U

//******************************************************************************
// Register: OWNADRTYPE
//******************************************************************************
// Field: [0:0] type
//
// Address in OWNADR
#define PBE_BLE5_RAM_OWNADRTYPE_TYPE                                     0x0001U
#define PBE_BLE5_RAM_OWNADRTYPE_TYPE_M                                   0x0001U
#define PBE_BLE5_RAM_OWNADRTYPE_TYPE_S                                        0U
#define PBE_BLE5_RAM_OWNADRTYPE_TYPE_PUBLIC                              0x0000U
#define PBE_BLE5_RAM_OWNADRTYPE_TYPE_RANDOM                              0x0001U

//******************************************************************************
// Register: SCANATYPE
//******************************************************************************
// Field: [0:0] type
//
// Address in SCANADR
#define PBE_BLE5_RAM_SCANATYPE_TYPE                                      0x0001U
#define PBE_BLE5_RAM_SCANATYPE_TYPE_M                                    0x0001U
#define PBE_BLE5_RAM_SCANATYPE_TYPE_S                                         0U
#define PBE_BLE5_RAM_SCANATYPE_TYPE_PUBLIC                               0x0000U
#define PBE_BLE5_RAM_SCANATYPE_TYPE_RANDOM                               0x0001U

//******************************************************************************
// Register: PEERADRTYPE
//******************************************************************************
// Field: [0:0] type
//
// Address in PEERADR
#define PBE_BLE5_RAM_PEERADRTYPE_TYPE                                    0x0001U
#define PBE_BLE5_RAM_PEERADRTYPE_TYPE_M                                  0x0001U
#define PBE_BLE5_RAM_PEERADRTYPE_TYPE_S                                       0U
#define PBE_BLE5_RAM_PEERADRTYPE_TYPE_PUBLIC                             0x0000U
#define PBE_BLE5_RAM_PEERADRTYPE_TYPE_RANDOM                             0x0001U

//******************************************************************************
// Register: OWNADRL
//******************************************************************************
// Field: [15:0] vallsb
//
// 
#define PBE_BLE5_RAM_OWNADRL_VALLSB_W                                        16U
#define PBE_BLE5_RAM_OWNADRL_VALLSB_M                                    0xFFFFU
#define PBE_BLE5_RAM_OWNADRL_VALLSB_S                                         0U

//******************************************************************************
// Register: OWNADRM
//******************************************************************************
// Field: [15:0] vallsb
//
// 
#define PBE_BLE5_RAM_OWNADRM_VALLSB_W                                        16U
#define PBE_BLE5_RAM_OWNADRM_VALLSB_M                                    0xFFFFU
#define PBE_BLE5_RAM_OWNADRM_VALLSB_S                                         0U

//******************************************************************************
// Register: OWNADRH
//******************************************************************************
// Field: [15:0] valmsb
//
// 
#define PBE_BLE5_RAM_OWNADRH_VALMSB_W                                        16U
#define PBE_BLE5_RAM_OWNADRH_VALMSB_M                                    0xFFFFU
#define PBE_BLE5_RAM_OWNADRH_VALMSB_S                                         0U

//******************************************************************************
// Register: SCANADRL
//******************************************************************************
// Field: [15:0] vallsb
//
// 
#define PBE_BLE5_RAM_SCANADRL_VALLSB_W                                       16U
#define PBE_BLE5_RAM_SCANADRL_VALLSB_M                                   0xFFFFU
#define PBE_BLE5_RAM_SCANADRL_VALLSB_S                                        0U

//******************************************************************************
// Register: SCANADRM
//******************************************************************************
// Field: [15:0] vallsb
//
// 
#define PBE_BLE5_RAM_SCANADRM_VALLSB_W                                       16U
#define PBE_BLE5_RAM_SCANADRM_VALLSB_M                                   0xFFFFU
#define PBE_BLE5_RAM_SCANADRM_VALLSB_S                                        0U

//******************************************************************************
// Register: SCANADRH
//******************************************************************************
// Field: [15:0] valmsb
//
// 
#define PBE_BLE5_RAM_SCANADRH_VALMSB_W                                       16U
#define PBE_BLE5_RAM_SCANADRH_VALMSB_M                                   0xFFFFU
#define PBE_BLE5_RAM_SCANADRH_VALMSB_S                                        0U

//******************************************************************************
// Register: TMPADRL
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of temporary address
#define PBE_BLE5_RAM_TMPADRL_VAL_W                                           16U
#define PBE_BLE5_RAM_TMPADRL_VAL_M                                       0xFFFFU
#define PBE_BLE5_RAM_TMPADRL_VAL_S                                            0U

//******************************************************************************
// Register: TMPADRM
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of temporary address
#define PBE_BLE5_RAM_TMPADRM_VAL_W                                           16U
#define PBE_BLE5_RAM_TMPADRM_VAL_M                                       0xFFFFU
#define PBE_BLE5_RAM_TMPADRM_VAL_S                                            0U

//******************************************************************************
// Register: TMPADRH
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of temporary address
#define PBE_BLE5_RAM_TMPADRH_VAL_W                                           16U
#define PBE_BLE5_RAM_TMPADRH_VAL_M                                       0xFFFFU
#define PBE_BLE5_RAM_TMPADRH_VAL_S                                            0U

//******************************************************************************
// Register: PEERADRL
//******************************************************************************
// Field: [15:0] vallsb
//
// 
#define PBE_BLE5_RAM_PEERADRL_VALLSB_W                                       16U
#define PBE_BLE5_RAM_PEERADRL_VALLSB_M                                   0xFFFFU
#define PBE_BLE5_RAM_PEERADRL_VALLSB_S                                        0U

//******************************************************************************
// Register: PEERADRM
//******************************************************************************
// Field: [15:0] vallsb
//
// 
#define PBE_BLE5_RAM_PEERADRM_VALLSB_W                                       16U
#define PBE_BLE5_RAM_PEERADRM_VALLSB_M                                   0xFFFFU
#define PBE_BLE5_RAM_PEERADRM_VALLSB_S                                        0U

//******************************************************************************
// Register: PEERADRH
//******************************************************************************
// Field: [15:0] valmsb
//
// 
#define PBE_BLE5_RAM_PEERADRH_VALMSB_W                                       16U
#define PBE_BLE5_RAM_PEERADRH_VALMSB_M                                   0xFFFFU
#define PBE_BLE5_RAM_PEERADRH_VALMSB_S                                        0U

//******************************************************************************
// Register: NTXDONE
//******************************************************************************
// Field: [15:0] val
//
// Number of acknowledgements received on packets from the Tx FIFO
#define PBE_BLE5_RAM_NTXDONE_VAL_W                                           16U
#define PBE_BLE5_RAM_NTXDONE_VAL_M                                       0xFFFFU
#define PBE_BLE5_RAM_NTXDONE_VAL_S                                            0U

//******************************************************************************
// Register: NTXACK
//******************************************************************************
// Field: [15:0] val
//
// Total number of acknowledgement received on transmitted packets
#define PBE_BLE5_RAM_NTXACK_VAL_W                                            16U
#define PBE_BLE5_RAM_NTXACK_VAL_M                                        0xFFFFU
#define PBE_BLE5_RAM_NTXACK_VAL_S                                             0U

//******************************************************************************
// Register: NTXCTLACK
//******************************************************************************
// Field: [15:0] val
//
// Number of acknowledgements received on transmitted LL control packets
#define PBE_BLE5_RAM_NTXCTLACK_VAL_W                                         16U
#define PBE_BLE5_RAM_NTXCTLACK_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_NTXCTLACK_VAL_S                                          0U

//******************************************************************************
// Register: NTXCTL
//******************************************************************************
// Field: [15:0] val
//
// Number of unique LL control packets transmitted 
#define PBE_BLE5_RAM_NTXCTL_VAL_W                                            16U
#define PBE_BLE5_RAM_NTXCTL_VAL_M                                        0xFFFFU
#define PBE_BLE5_RAM_NTXCTL_VAL_S                                             0U

//******************************************************************************
// Register: NTXRETRANS
//******************************************************************************
// Field: [15:0] val
//
// Number of packets transmitted being the same as the previously transmitted packet.
#define PBE_BLE5_RAM_NTXRETRANS_VAL_W                                        16U
#define PBE_BLE5_RAM_NTXRETRANS_VAL_M                                    0xFFFFU
#define PBE_BLE5_RAM_NTXRETRANS_VAL_S                                         0U

//******************************************************************************
// Register: NRXNOK
//******************************************************************************
// Field: [15:0] val
//
// Number of packets received with CRC error
#define PBE_BLE5_RAM_NRXNOK_VAL_W                                            16U
#define PBE_BLE5_RAM_NRXNOK_VAL_M                                        0xFFFFU
#define PBE_BLE5_RAM_NRXNOK_VAL_S                                             0U

//******************************************************************************
// Register: NRXIGNORED
//******************************************************************************
// Field: [15:0] val
//
// Number of packets received with CRC OK, but to be ignored by the MCU
#define PBE_BLE5_RAM_NRXIGNORED_VAL_W                                        16U
#define PBE_BLE5_RAM_NRXIGNORED_VAL_M                                    0xFFFFU
#define PBE_BLE5_RAM_NRXIGNORED_VAL_S                                         0U

//******************************************************************************
// Register: NRXEMPTY
//******************************************************************************
// Field: [15:0] val
//
// Number of packets received with CRC OK and length zero
#define PBE_BLE5_RAM_NRXEMPTY_VAL_W                                          16U
#define PBE_BLE5_RAM_NRXEMPTY_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_NRXEMPTY_VAL_S                                           0U

//******************************************************************************
// Register: NRXFIFOFULL
//******************************************************************************
// Field: [15:0] val
//
// Number of received packets discarded because the Rx FIFO was full
#define PBE_BLE5_RAM_NRXFIFOFULL_VAL_W                                       16U
#define PBE_BLE5_RAM_NRXFIFOFULL_VAL_M                                   0xFFFFU
#define PBE_BLE5_RAM_NRXFIFOFULL_VAL_S                                        0U

//******************************************************************************
// Register: NRXOK
//******************************************************************************
// Field: [15:0] val
//
// Number of non-empty packets received with CRC OK and not  to be ignored by the MCU
#define PBE_BLE5_RAM_NRXOK_VAL_W                                             16U
#define PBE_BLE5_RAM_NRXOK_VAL_M                                         0xFFFFU
#define PBE_BLE5_RAM_NRXOK_VAL_S                                              0U

//******************************************************************************
// Register: NTX
//******************************************************************************
// Field: [15:0] val
//
// Total number of packets transmitted in the task
#define PBE_BLE5_RAM_NTX_VAL_W                                               16U
#define PBE_BLE5_RAM_NTX_VAL_M                                           0xFFFFU
#define PBE_BLE5_RAM_NTX_VAL_S                                                0U

//******************************************************************************
// Register: NRXCTL
//******************************************************************************
// Field: [15:0] val
//
// Number of LL control packets correctly received (also included in NRXOK)
#define PBE_BLE5_RAM_NRXCTL_VAL_W                                            16U
#define PBE_BLE5_RAM_NRXCTL_VAL_M                                        0xFFFFU
#define PBE_BLE5_RAM_NRXCTL_VAL_S                                             0U

//******************************************************************************
// Register: NRXCTLACK
//******************************************************************************
// Field: [15:0] val
//
// Total number of acknowledgement transmitted on received LL control packets.
#define PBE_BLE5_RAM_NRXCTLACK_VAL_W                                         16U
#define PBE_BLE5_RAM_NRXCTLACK_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_NRXCTLACK_VAL_S                                          0U

//******************************************************************************
// Register: LASTRSSI
//******************************************************************************
// Field: [15:0] val
//
// RSSI of last packet received in task
#define PBE_BLE5_RAM_LASTRSSI_VAL_W                                          16U
#define PBE_BLE5_RAM_LASTRSSI_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_LASTRSSI_VAL_S                                           0U

//******************************************************************************
// Register: FIRSTRXTIMEOUT
//******************************************************************************
// Field: [15:0] val
//
// For peripheral tasks, the timeout in µs from SYSTCMP0 event to giving up listening for the first packet. Must be at least 128 µs if non-zero. Can be up to 32000 µs based on supervision timeout, window widening.
#define PBE_BLE5_RAM_FIRSTRXTIMEOUT_VAL_W                                    16U
#define PBE_BLE5_RAM_FIRSTRXTIMEOUT_VAL_M                                0xFFFFU
#define PBE_BLE5_RAM_FIRSTRXTIMEOUT_VAL_S                                     0U

//******************************************************************************
// Register: LASTTIMESTAMPL
//******************************************************************************
// Field: [15:0] val
//
// Lower part of timestamp.
#define PBE_BLE5_RAM_LASTTIMESTAMPL_VAL_W                                    16U
#define PBE_BLE5_RAM_LASTTIMESTAMPL_VAL_M                                0xFFFFU
#define PBE_BLE5_RAM_LASTTIMESTAMPL_VAL_S                                     0U

//******************************************************************************
// Register: LASTTIMESTAMPH
//******************************************************************************
// Field: [15:0] val
//
// Upper part of timestamp.
#define PBE_BLE5_RAM_LASTTIMESTAMPH_VAL_W                                    16U
#define PBE_BLE5_RAM_LASTTIMESTAMPH_VAL_M                                0xFFFFU
#define PBE_BLE5_RAM_LASTTIMESTAMPH_VAL_S                                     0U

//******************************************************************************
// Register: MDCFG
//******************************************************************************
// Field: [1:0] force
//
// 
#define PBE_BLE5_RAM_MDCFG_FORCE_W                                            2U
#define PBE_BLE5_RAM_MDCFG_FORCE_M                                       0x0003U
#define PBE_BLE5_RAM_MDCFG_FORCE_S                                            0U
#define PBE_BLE5_RAM_MDCFG_FORCE_MD0                                     0x0001U
#define PBE_BLE5_RAM_MDCFG_FORCE_MD1                                     0x0002U

//******************************************************************************
// Register: ADVCFG
//******************************************************************************
// Field: [3:3] extended
//
// 
#define PBE_BLE5_RAM_ADVCFG_EXTENDED                                     0x0008U
#define PBE_BLE5_RAM_ADVCFG_EXTENDED_M                                   0x0008U
#define PBE_BLE5_RAM_ADVCFG_EXTENDED_S                                        3U

// Field: [2:2] directed
//
// 
#define PBE_BLE5_RAM_ADVCFG_DIRECTED                                     0x0004U
#define PBE_BLE5_RAM_ADVCFG_DIRECTED_M                                   0x0004U
#define PBE_BLE5_RAM_ADVCFG_DIRECTED_S                                        2U

// Field: [1:1] scannable
//
// 
#define PBE_BLE5_RAM_ADVCFG_SCANNABLE                                    0x0002U
#define PBE_BLE5_RAM_ADVCFG_SCANNABLE_M                                  0x0002U
#define PBE_BLE5_RAM_ADVCFG_SCANNABLE_S                                       1U

// Field: [0:0] connectable
//
// 
#define PBE_BLE5_RAM_ADVCFG_CONNECTABLE                                  0x0001U
#define PBE_BLE5_RAM_ADVCFG_CONNECTABLE_M                                0x0001U
#define PBE_BLE5_RAM_ADVCFG_CONNECTABLE_S                                     0U

//******************************************************************************
// Register: TMPRSSI
//******************************************************************************
// Field: [15:0] val
//
// RSSI
#define PBE_BLE5_RAM_TMPRSSI_VAL_W                                           16U
#define PBE_BLE5_RAM_TMPRSSI_VAL_M                                       0xFFFFU
#define PBE_BLE5_RAM_TMPRSSI_VAL_S                                            0U

//******************************************************************************
// Register: TMPREG
//******************************************************************************
// Field: [15:0] val
//
// VAL
#define PBE_BLE5_RAM_TMPREG_VAL_W                                            16U
#define PBE_BLE5_RAM_TMPREG_VAL_M                                        0xFFFFU
#define PBE_BLE5_RAM_TMPREG_VAL_S                                             0U

//******************************************************************************
// Register: FIRSTTIMESTAMPL
//******************************************************************************
// Field: [15:0] val
//
// Peripheral only.
#define PBE_BLE5_RAM_FIRSTTIMESTAMPL_VAL_W                                   16U
#define PBE_BLE5_RAM_FIRSTTIMESTAMPL_VAL_M                               0xFFFFU
#define PBE_BLE5_RAM_FIRSTTIMESTAMPL_VAL_S                                    0U

//******************************************************************************
// Register: FIRSTTIMESTAMPH
//******************************************************************************
// Field: [15:0] val
//
// Peripheral only.
#define PBE_BLE5_RAM_FIRSTTIMESTAMPH_VAL_W                                   16U
#define PBE_BLE5_RAM_FIRSTTIMESTAMPH_VAL_M                               0xFFFFU
#define PBE_BLE5_RAM_FIRSTTIMESTAMPH_VAL_S                                    0U

//******************************************************************************
// Register: FL1RESULT
//******************************************************************************
// Field: [3:0] index
//
// Which entry in the address table matched
#define PBE_BLE5_RAM_FL1RESULT_INDEX_W                                        4U
#define PBE_BLE5_RAM_FL1RESULT_INDEX_M                                   0x000FU
#define PBE_BLE5_RAM_FL1RESULT_INDEX_S                                        0U

//******************************************************************************
// Register: FL1MASK
//******************************************************************************
// Field: [15:15] match
//
// 
#define PBE_BLE5_RAM_FL1MASK_MATCH                                       0x8000U
#define PBE_BLE5_RAM_FL1MASK_MATCH_M                                     0x8000U
#define PBE_BLE5_RAM_FL1MASK_MATCH_S                                         15U
#define PBE_BLE5_RAM_FL1MASK_MATCH_DIS                                   0x0000U
#define PBE_BLE5_RAM_FL1MASK_MATCH_EN                                    0x8000U

// Field: [14:4] rfuign
//
// These bits works the same was as PRIVIGN and FALIGN.
#define PBE_BLE5_RAM_FL1MASK_RFUIGN_W                                        11U
#define PBE_BLE5_RAM_FL1MASK_RFUIGN_M                                    0x7FF0U
#define PBE_BLE5_RAM_FL1MASK_RFUIGN_S                                         4U

// Field: [3:3] privign
//
// 
#define PBE_BLE5_RAM_FL1MASK_PRIVIGN                                     0x0008U
#define PBE_BLE5_RAM_FL1MASK_PRIVIGN_M                                   0x0008U
#define PBE_BLE5_RAM_FL1MASK_PRIVIGN_S                                        3U
#define PBE_BLE5_RAM_FL1MASK_PRIVIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL1MASK_PRIVIGN_IGN                                 0x0008U

// Field: [2:2] duplicateign
//
// 
#define PBE_BLE5_RAM_FL1MASK_DUPLICATEIGN                                0x0004U
#define PBE_BLE5_RAM_FL1MASK_DUPLICATEIGN_M                              0x0004U
#define PBE_BLE5_RAM_FL1MASK_DUPLICATEIGN_S                                   2U
#define PBE_BLE5_RAM_FL1MASK_DUPLICATEIGN_NIGN                           0x0000U
#define PBE_BLE5_RAM_FL1MASK_DUPLICATEIGN_IGN                            0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1MASK_TYPE                                        0x0002U
#define PBE_BLE5_RAM_FL1MASK_TYPE_M                                      0x0002U
#define PBE_BLE5_RAM_FL1MASK_TYPE_S                                           1U
#define PBE_BLE5_RAM_FL1MASK_TYPE_DIS                                    0x0000U
#define PBE_BLE5_RAM_FL1MASK_TYPE_EN                                     0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1MASK_EN                                          0x0001U
#define PBE_BLE5_RAM_FL1MASK_EN_M                                        0x0001U
#define PBE_BLE5_RAM_FL1MASK_EN_S                                             0U
#define PBE_BLE5_RAM_FL1MASK_EN_DIS                                      0x0000U
#define PBE_BLE5_RAM_FL1MASK_EN_EN                                       0x0001U

//******************************************************************************
// Register: FL2RESULT
//******************************************************************************
// Field: [3:0] index
//
// Which entry in the address table matched
#define PBE_BLE5_RAM_FL2RESULT_INDEX_W                                        4U
#define PBE_BLE5_RAM_FL2RESULT_INDEX_M                                   0x000FU
#define PBE_BLE5_RAM_FL2RESULT_INDEX_S                                        0U

//******************************************************************************
// Register: FL2MASK
//******************************************************************************
// Field: [15:15] match
//
// 
#define PBE_BLE5_RAM_FL2MASK_MATCH                                       0x8000U
#define PBE_BLE5_RAM_FL2MASK_MATCH_M                                     0x8000U
#define PBE_BLE5_RAM_FL2MASK_MATCH_S                                         15U
#define PBE_BLE5_RAM_FL2MASK_MATCH_DIS                                   0x0000U
#define PBE_BLE5_RAM_FL2MASK_MATCH_EN                                    0x8000U

// Field: [14:4] rfuign
//
// These bits works the same was as PRIVIGN and FALIGN.
#define PBE_BLE5_RAM_FL2MASK_RFUIGN_W                                        11U
#define PBE_BLE5_RAM_FL2MASK_RFUIGN_M                                    0x7FF0U
#define PBE_BLE5_RAM_FL2MASK_RFUIGN_S                                         4U

// Field: [3:3] privign
//
// 
#define PBE_BLE5_RAM_FL2MASK_PRIVIGN                                     0x0008U
#define PBE_BLE5_RAM_FL2MASK_PRIVIGN_M                                   0x0008U
#define PBE_BLE5_RAM_FL2MASK_PRIVIGN_S                                        3U
#define PBE_BLE5_RAM_FL2MASK_PRIVIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL2MASK_PRIVIGN_IGN                                 0x0008U

// Field: [2:2] duplicateign
//
// 
#define PBE_BLE5_RAM_FL2MASK_DUPLICATEIGN                                0x0004U
#define PBE_BLE5_RAM_FL2MASK_DUPLICATEIGN_M                              0x0004U
#define PBE_BLE5_RAM_FL2MASK_DUPLICATEIGN_S                                   2U
#define PBE_BLE5_RAM_FL2MASK_DUPLICATEIGN_NIGN                           0x0000U
#define PBE_BLE5_RAM_FL2MASK_DUPLICATEIGN_IGN                            0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2MASK_TYPE                                        0x0002U
#define PBE_BLE5_RAM_FL2MASK_TYPE_M                                      0x0002U
#define PBE_BLE5_RAM_FL2MASK_TYPE_S                                           1U
#define PBE_BLE5_RAM_FL2MASK_TYPE_DIS                                    0x0000U
#define PBE_BLE5_RAM_FL2MASK_TYPE_EN                                     0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2MASK_EN                                          0x0001U
#define PBE_BLE5_RAM_FL2MASK_EN_M                                        0x0001U
#define PBE_BLE5_RAM_FL2MASK_EN_S                                             0U
#define PBE_BLE5_RAM_FL2MASK_EN_DIS                                      0x0000U
#define PBE_BLE5_RAM_FL2MASK_EN_EN                                       0x0001U

//******************************************************************************
// Register: FLSTAT
//******************************************************************************
// Field: [1:1] fl2running
//
// Status; is FL2 running address filtering
#define PBE_BLE5_RAM_FLSTAT_FL2RUNNING                                   0x0002U
#define PBE_BLE5_RAM_FLSTAT_FL2RUNNING_M                                 0x0002U
#define PBE_BLE5_RAM_FLSTAT_FL2RUNNING_S                                      1U
#define PBE_BLE5_RAM_FLSTAT_FL2RUNNING_IDLE                              0x0000U
#define PBE_BLE5_RAM_FLSTAT_FL2RUNNING_RUNNING                           0x0002U

// Field: [0:0] fl1running
//
// Status; is FL1 running address filtering
#define PBE_BLE5_RAM_FLSTAT_FL1RUNNING                                   0x0001U
#define PBE_BLE5_RAM_FLSTAT_FL1RUNNING_M                                 0x0001U
#define PBE_BLE5_RAM_FLSTAT_FL1RUNNING_S                                      0U
#define PBE_BLE5_RAM_FLSTAT_FL1RUNNING_IDLE                              0x0000U
#define PBE_BLE5_RAM_FLSTAT_FL1RUNNING_RUNNING                           0x0001U

//******************************************************************************
// Register: TMPATYPE
//******************************************************************************
// Field: [0:0] val
//
// temporary address type storage
#define PBE_BLE5_RAM_TMPATYPE_VAL                                        0x0001U
#define PBE_BLE5_RAM_TMPATYPE_VAL_M                                      0x0001U
#define PBE_BLE5_RAM_TMPATYPE_VAL_S                                           0U

//******************************************************************************
// Register: PATTERN
//******************************************************************************
// Field: [15:0] val
//
// Data to send if OPCFG.TXPATTERN is 1
#define PBE_BLE5_RAM_PATTERN_VAL_W                                           16U
#define PBE_BLE5_RAM_PATTERN_VAL_M                                       0xFFFFU
#define PBE_BLE5_RAM_PATTERN_VAL_S                                            0U

//******************************************************************************
// Register: RFINTERVAL
//******************************************************************************
// Field: [15:0] val
//
// RF interval, measured from start-to-start of TX_RAW. The actual time depends on configuration of timer1.
#define PBE_BLE5_RAM_RFINTERVAL_VAL_W                                        16U
#define PBE_BLE5_RAM_RFINTERVAL_VAL_M                                    0xFFFFU
#define PBE_BLE5_RAM_RFINTERVAL_VAL_S                                         0U

//******************************************************************************
// Register: NTXTARGET
//******************************************************************************
// Field: [15:0] val
//
// Used in TX_RAW; Total number of packets to transfer.
#define PBE_BLE5_RAM_NTXTARGET_VAL_W                                         16U
#define PBE_BLE5_RAM_NTXTARGET_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_NTXTARGET_VAL_S                                          0U

//******************************************************************************
// Register: OPCFG
//******************************************************************************
// Field: [4:4] repeat
//
// Rule for what to do after operation completes.
#define PBE_BLE5_RAM_OPCFG_REPEAT                                        0x0010U
#define PBE_BLE5_RAM_OPCFG_REPEAT_M                                      0x0010U
#define PBE_BLE5_RAM_OPCFG_REPEAT_S                                           4U
#define PBE_BLE5_RAM_OPCFG_REPEAT_NO                                     0x0000U
#define PBE_BLE5_RAM_OPCFG_REPEAT_YES                                    0x0010U

// Field: [3:3] txpattern
//
// Send fixed pattern
#define PBE_BLE5_RAM_OPCFG_TXPATTERN                                     0x0008U
#define PBE_BLE5_RAM_OPCFG_TXPATTERN_M                                   0x0008U
#define PBE_BLE5_RAM_OPCFG_TXPATTERN_S                                        3U
#define PBE_BLE5_RAM_OPCFG_TXPATTERN_NO                                  0x0000U
#define PBE_BLE5_RAM_OPCFG_TXPATTERN_YES                                 0x0008U

// Field: [2:1] txfcmd
//
// Rule for FCMD after TX_DONE
#define PBE_BLE5_RAM_OPCFG_TXFCMD_W                                           2U
#define PBE_BLE5_RAM_OPCFG_TXFCMD_M                                      0x0006U
#define PBE_BLE5_RAM_OPCFG_TXFCMD_S                                           1U
#define PBE_BLE5_RAM_OPCFG_TXFCMD_NONE                                   0x0000U
#define PBE_BLE5_RAM_OPCFG_TXFCMD_RETRY                                  0x0002U
#define PBE_BLE5_RAM_OPCFG_TXFCMD_DEALLOC                                0x0004U

// Field: [0:0] rfinterval
//
// Requires REPEAT to make sense.
#define PBE_BLE5_RAM_OPCFG_RFINTERVAL                                    0x0001U
#define PBE_BLE5_RAM_OPCFG_RFINTERVAL_M                                  0x0001U
#define PBE_BLE5_RAM_OPCFG_RFINTERVAL_S                                       0U
#define PBE_BLE5_RAM_OPCFG_RFINTERVAL_DIS                                0x0000U
#define PBE_BLE5_RAM_OPCFG_RFINTERVAL_EN                                 0x0001U

//******************************************************************************
// Register: FL1INFO0
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO0_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL1INFO0_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL1INFO0_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL1INFO0_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL1INFO0_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO0_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL1INFO0_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL1INFO0_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO0_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL1INFO0_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL1INFO0_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL1INFO0_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO0_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO0_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL1INFO0_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL1INFO0_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL1INFO0_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL1INFO0_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO0_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL1INFO0_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL1INFO0_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL1INFO0_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL1INFO0_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO0_EN                                         0x0001U
#define PBE_BLE5_RAM_FL1INFO0_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL1INFO0_EN_S                                            0U
#define PBE_BLE5_RAM_FL1INFO0_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL1INFO0_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL1ADRL0
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL0_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRL0_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL0_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRM0
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM0_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRM0_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM0_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRH0
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH0_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRH0_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH0_VAL_S                                           0U

//******************************************************************************
// Register: FL1INFO1
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO1_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL1INFO1_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL1INFO1_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL1INFO1_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL1INFO1_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO1_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL1INFO1_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL1INFO1_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO1_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL1INFO1_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL1INFO1_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL1INFO1_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO1_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO1_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL1INFO1_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL1INFO1_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL1INFO1_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL1INFO1_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO1_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL1INFO1_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL1INFO1_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL1INFO1_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL1INFO1_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO1_EN                                         0x0001U
#define PBE_BLE5_RAM_FL1INFO1_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL1INFO1_EN_S                                            0U
#define PBE_BLE5_RAM_FL1INFO1_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL1INFO1_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL1ADRL1
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL1_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRL1_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL1_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRM1
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM1_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRM1_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM1_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRH1
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH1_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRH1_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH1_VAL_S                                           0U

//******************************************************************************
// Register: FL1INFO2
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO2_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL1INFO2_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL1INFO2_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL1INFO2_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL1INFO2_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO2_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL1INFO2_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL1INFO2_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO2_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL1INFO2_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL1INFO2_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL1INFO2_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO2_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO2_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL1INFO2_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL1INFO2_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL1INFO2_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL1INFO2_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO2_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL1INFO2_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL1INFO2_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL1INFO2_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL1INFO2_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO2_EN                                         0x0001U
#define PBE_BLE5_RAM_FL1INFO2_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL1INFO2_EN_S                                            0U
#define PBE_BLE5_RAM_FL1INFO2_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL1INFO2_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL1ADRL2
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL2_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRL2_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL2_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRM2
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM2_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRM2_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM2_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRH2
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH2_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRH2_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH2_VAL_S                                           0U

//******************************************************************************
// Register: FL1INFO3
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO3_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL1INFO3_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL1INFO3_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL1INFO3_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL1INFO3_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO3_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL1INFO3_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL1INFO3_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO3_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL1INFO3_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL1INFO3_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL1INFO3_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO3_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO3_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL1INFO3_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL1INFO3_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL1INFO3_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL1INFO3_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO3_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL1INFO3_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL1INFO3_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL1INFO3_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL1INFO3_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO3_EN                                         0x0001U
#define PBE_BLE5_RAM_FL1INFO3_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL1INFO3_EN_S                                            0U
#define PBE_BLE5_RAM_FL1INFO3_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL1INFO3_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL1ADRL3
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL3_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRL3_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL3_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRM3
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM3_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRM3_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM3_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRH3
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH3_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRH3_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH3_VAL_S                                           0U

//******************************************************************************
// Register: FL1INFO4
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO4_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL1INFO4_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL1INFO4_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL1INFO4_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL1INFO4_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO4_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL1INFO4_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL1INFO4_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO4_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL1INFO4_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL1INFO4_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL1INFO4_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO4_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO4_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL1INFO4_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL1INFO4_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL1INFO4_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL1INFO4_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO4_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL1INFO4_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL1INFO4_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL1INFO4_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL1INFO4_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO4_EN                                         0x0001U
#define PBE_BLE5_RAM_FL1INFO4_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL1INFO4_EN_S                                            0U
#define PBE_BLE5_RAM_FL1INFO4_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL1INFO4_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL1ADRL4
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL4_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRL4_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL4_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRM4
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM4_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRM4_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM4_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRH4
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH4_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRH4_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH4_VAL_S                                           0U

//******************************************************************************
// Register: FL1INFO5
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO5_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL1INFO5_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL1INFO5_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL1INFO5_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL1INFO5_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO5_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL1INFO5_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL1INFO5_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO5_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL1INFO5_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL1INFO5_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL1INFO5_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO5_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO5_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL1INFO5_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL1INFO5_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL1INFO5_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL1INFO5_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO5_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL1INFO5_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL1INFO5_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL1INFO5_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL1INFO5_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO5_EN                                         0x0001U
#define PBE_BLE5_RAM_FL1INFO5_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL1INFO5_EN_S                                            0U
#define PBE_BLE5_RAM_FL1INFO5_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL1INFO5_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL1ADRL5
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL5_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRL5_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL5_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRM5
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM5_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRM5_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM5_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRH5
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH5_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRH5_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH5_VAL_S                                           0U

//******************************************************************************
// Register: FL1INFO6
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO6_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL1INFO6_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL1INFO6_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL1INFO6_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL1INFO6_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO6_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL1INFO6_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL1INFO6_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO6_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL1INFO6_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL1INFO6_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL1INFO6_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO6_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO6_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL1INFO6_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL1INFO6_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL1INFO6_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL1INFO6_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO6_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL1INFO6_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL1INFO6_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL1INFO6_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL1INFO6_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO6_EN                                         0x0001U
#define PBE_BLE5_RAM_FL1INFO6_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL1INFO6_EN_S                                            0U
#define PBE_BLE5_RAM_FL1INFO6_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL1INFO6_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL1ADRL6
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL6_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRL6_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL6_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRM6
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM6_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRM6_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM6_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRH6
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH6_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRH6_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH6_VAL_S                                           0U

//******************************************************************************
// Register: FL1INFO7
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO7_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL1INFO7_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL1INFO7_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL1INFO7_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL1INFO7_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO7_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL1INFO7_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL1INFO7_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO7_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL1INFO7_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL1INFO7_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL1INFO7_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO7_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO7_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL1INFO7_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL1INFO7_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL1INFO7_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL1INFO7_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO7_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL1INFO7_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL1INFO7_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL1INFO7_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL1INFO7_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO7_EN                                         0x0001U
#define PBE_BLE5_RAM_FL1INFO7_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL1INFO7_EN_S                                            0U
#define PBE_BLE5_RAM_FL1INFO7_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL1INFO7_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL1ADRL7
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL7_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRL7_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL7_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRM7
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM7_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRM7_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM7_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRH7
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH7_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRH7_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH7_VAL_S                                           0U

//******************************************************************************
// Register: FL1INFO8
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO8_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL1INFO8_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL1INFO8_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL1INFO8_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL1INFO8_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO8_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL1INFO8_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL1INFO8_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO8_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL1INFO8_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL1INFO8_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL1INFO8_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO8_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO8_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL1INFO8_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL1INFO8_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL1INFO8_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL1INFO8_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO8_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL1INFO8_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL1INFO8_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL1INFO8_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL1INFO8_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO8_EN                                         0x0001U
#define PBE_BLE5_RAM_FL1INFO8_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL1INFO8_EN_S                                            0U
#define PBE_BLE5_RAM_FL1INFO8_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL1INFO8_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL1ADRL8
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL8_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRL8_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL8_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRM8
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM8_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRM8_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM8_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRH8
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH8_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRH8_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH8_VAL_S                                           0U

//******************************************************************************
// Register: FL1INFO9
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO9_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL1INFO9_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL1INFO9_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL1INFO9_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL1INFO9_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO9_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL1INFO9_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL1INFO9_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO9_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL1INFO9_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL1INFO9_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL1INFO9_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO9_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO9_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL1INFO9_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL1INFO9_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL1INFO9_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL1INFO9_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO9_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL1INFO9_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL1INFO9_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL1INFO9_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL1INFO9_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO9_EN                                         0x0001U
#define PBE_BLE5_RAM_FL1INFO9_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL1INFO9_EN_S                                            0U
#define PBE_BLE5_RAM_FL1INFO9_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL1INFO9_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL1ADRL9
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL9_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRL9_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL9_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRM9
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM9_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRM9_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM9_VAL_S                                           0U

//******************************************************************************
// Register: FL1ADRH9
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH9_VAL_W                                          16U
#define PBE_BLE5_RAM_FL1ADRH9_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH9_VAL_S                                           0U

//******************************************************************************
// Register: FL1INFO10
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO10_MATCHTMP                                  0x8000U
#define PBE_BLE5_RAM_FL1INFO10_MATCHTMP_M                                0x8000U
#define PBE_BLE5_RAM_FL1INFO10_MATCHTMP_S                                    15U
#define PBE_BLE5_RAM_FL1INFO10_MATCHTMP_NOMATCH                          0x0000U
#define PBE_BLE5_RAM_FL1INFO10_MATCHTMP_MATCH                            0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO10_RESERVED_W                                    11U
#define PBE_BLE5_RAM_FL1INFO10_RESERVED_M                                0x7FF0U
#define PBE_BLE5_RAM_FL1INFO10_RESERVED_S                                     4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO10_PRIVIGN                                   0x0008U
#define PBE_BLE5_RAM_FL1INFO10_PRIVIGN_M                                 0x0008U
#define PBE_BLE5_RAM_FL1INFO10_PRIVIGN_S                                      3U
#define PBE_BLE5_RAM_FL1INFO10_PRIVIGN_NIGN                              0x0000U
#define PBE_BLE5_RAM_FL1INFO10_PRIVIGN_IGN                               0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO10_FALIGN                                    0x0004U
#define PBE_BLE5_RAM_FL1INFO10_FALIGN_M                                  0x0004U
#define PBE_BLE5_RAM_FL1INFO10_FALIGN_S                                       2U
#define PBE_BLE5_RAM_FL1INFO10_FALIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO10_FALIGN_IGN                                0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO10_TYPE                                      0x0002U
#define PBE_BLE5_RAM_FL1INFO10_TYPE_M                                    0x0002U
#define PBE_BLE5_RAM_FL1INFO10_TYPE_S                                         1U
#define PBE_BLE5_RAM_FL1INFO10_TYPE_PUBLIC                               0x0000U
#define PBE_BLE5_RAM_FL1INFO10_TYPE_RANDOM                               0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO10_EN                                        0x0001U
#define PBE_BLE5_RAM_FL1INFO10_EN_M                                      0x0001U
#define PBE_BLE5_RAM_FL1INFO10_EN_S                                           0U
#define PBE_BLE5_RAM_FL1INFO10_EN_DIS                                    0x0000U
#define PBE_BLE5_RAM_FL1INFO10_EN_EN                                     0x0001U

//******************************************************************************
// Register: FL1ADRL10
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL10_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRL10_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL10_VAL_S                                          0U

//******************************************************************************
// Register: FL1ADRM10
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM10_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRM10_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM10_VAL_S                                          0U

//******************************************************************************
// Register: FL1ADRH10
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH10_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRH10_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH10_VAL_S                                          0U

//******************************************************************************
// Register: FL1INFO11
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO11_MATCHTMP                                  0x8000U
#define PBE_BLE5_RAM_FL1INFO11_MATCHTMP_M                                0x8000U
#define PBE_BLE5_RAM_FL1INFO11_MATCHTMP_S                                    15U
#define PBE_BLE5_RAM_FL1INFO11_MATCHTMP_NOMATCH                          0x0000U
#define PBE_BLE5_RAM_FL1INFO11_MATCHTMP_MATCH                            0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO11_RESERVED_W                                    11U
#define PBE_BLE5_RAM_FL1INFO11_RESERVED_M                                0x7FF0U
#define PBE_BLE5_RAM_FL1INFO11_RESERVED_S                                     4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO11_PRIVIGN                                   0x0008U
#define PBE_BLE5_RAM_FL1INFO11_PRIVIGN_M                                 0x0008U
#define PBE_BLE5_RAM_FL1INFO11_PRIVIGN_S                                      3U
#define PBE_BLE5_RAM_FL1INFO11_PRIVIGN_NIGN                              0x0000U
#define PBE_BLE5_RAM_FL1INFO11_PRIVIGN_IGN                               0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO11_FALIGN                                    0x0004U
#define PBE_BLE5_RAM_FL1INFO11_FALIGN_M                                  0x0004U
#define PBE_BLE5_RAM_FL1INFO11_FALIGN_S                                       2U
#define PBE_BLE5_RAM_FL1INFO11_FALIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO11_FALIGN_IGN                                0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO11_TYPE                                      0x0002U
#define PBE_BLE5_RAM_FL1INFO11_TYPE_M                                    0x0002U
#define PBE_BLE5_RAM_FL1INFO11_TYPE_S                                         1U
#define PBE_BLE5_RAM_FL1INFO11_TYPE_PUBLIC                               0x0000U
#define PBE_BLE5_RAM_FL1INFO11_TYPE_RANDOM                               0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO11_EN                                        0x0001U
#define PBE_BLE5_RAM_FL1INFO11_EN_M                                      0x0001U
#define PBE_BLE5_RAM_FL1INFO11_EN_S                                           0U
#define PBE_BLE5_RAM_FL1INFO11_EN_DIS                                    0x0000U
#define PBE_BLE5_RAM_FL1INFO11_EN_EN                                     0x0001U

//******************************************************************************
// Register: FL1ADRL11
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL11_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRL11_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL11_VAL_S                                          0U

//******************************************************************************
// Register: FL1ADRM11
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM11_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRM11_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM11_VAL_S                                          0U

//******************************************************************************
// Register: FL1ADRH11
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH11_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRH11_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH11_VAL_S                                          0U

//******************************************************************************
// Register: FL1INFO12
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO12_MATCHTMP                                  0x8000U
#define PBE_BLE5_RAM_FL1INFO12_MATCHTMP_M                                0x8000U
#define PBE_BLE5_RAM_FL1INFO12_MATCHTMP_S                                    15U
#define PBE_BLE5_RAM_FL1INFO12_MATCHTMP_NOMATCH                          0x0000U
#define PBE_BLE5_RAM_FL1INFO12_MATCHTMP_MATCH                            0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO12_RESERVED_W                                    11U
#define PBE_BLE5_RAM_FL1INFO12_RESERVED_M                                0x7FF0U
#define PBE_BLE5_RAM_FL1INFO12_RESERVED_S                                     4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO12_PRIVIGN                                   0x0008U
#define PBE_BLE5_RAM_FL1INFO12_PRIVIGN_M                                 0x0008U
#define PBE_BLE5_RAM_FL1INFO12_PRIVIGN_S                                      3U
#define PBE_BLE5_RAM_FL1INFO12_PRIVIGN_NIGN                              0x0000U
#define PBE_BLE5_RAM_FL1INFO12_PRIVIGN_IGN                               0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO12_FALIGN                                    0x0004U
#define PBE_BLE5_RAM_FL1INFO12_FALIGN_M                                  0x0004U
#define PBE_BLE5_RAM_FL1INFO12_FALIGN_S                                       2U
#define PBE_BLE5_RAM_FL1INFO12_FALIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO12_FALIGN_IGN                                0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO12_TYPE                                      0x0002U
#define PBE_BLE5_RAM_FL1INFO12_TYPE_M                                    0x0002U
#define PBE_BLE5_RAM_FL1INFO12_TYPE_S                                         1U
#define PBE_BLE5_RAM_FL1INFO12_TYPE_PUBLIC                               0x0000U
#define PBE_BLE5_RAM_FL1INFO12_TYPE_RANDOM                               0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO12_EN                                        0x0001U
#define PBE_BLE5_RAM_FL1INFO12_EN_M                                      0x0001U
#define PBE_BLE5_RAM_FL1INFO12_EN_S                                           0U
#define PBE_BLE5_RAM_FL1INFO12_EN_DIS                                    0x0000U
#define PBE_BLE5_RAM_FL1INFO12_EN_EN                                     0x0001U

//******************************************************************************
// Register: FL1ADRL12
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL12_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRL12_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL12_VAL_S                                          0U

//******************************************************************************
// Register: FL1ADRM12
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM12_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRM12_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM12_VAL_S                                          0U

//******************************************************************************
// Register: FL1ADRH12
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH12_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRH12_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH12_VAL_S                                          0U

//******************************************************************************
// Register: FL1INFO13
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO13_MATCHTMP                                  0x8000U
#define PBE_BLE5_RAM_FL1INFO13_MATCHTMP_M                                0x8000U
#define PBE_BLE5_RAM_FL1INFO13_MATCHTMP_S                                    15U
#define PBE_BLE5_RAM_FL1INFO13_MATCHTMP_NOMATCH                          0x0000U
#define PBE_BLE5_RAM_FL1INFO13_MATCHTMP_MATCH                            0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO13_RESERVED_W                                    11U
#define PBE_BLE5_RAM_FL1INFO13_RESERVED_M                                0x7FF0U
#define PBE_BLE5_RAM_FL1INFO13_RESERVED_S                                     4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO13_PRIVIGN                                   0x0008U
#define PBE_BLE5_RAM_FL1INFO13_PRIVIGN_M                                 0x0008U
#define PBE_BLE5_RAM_FL1INFO13_PRIVIGN_S                                      3U
#define PBE_BLE5_RAM_FL1INFO13_PRIVIGN_NIGN                              0x0000U
#define PBE_BLE5_RAM_FL1INFO13_PRIVIGN_IGN                               0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO13_FALIGN                                    0x0004U
#define PBE_BLE5_RAM_FL1INFO13_FALIGN_M                                  0x0004U
#define PBE_BLE5_RAM_FL1INFO13_FALIGN_S                                       2U
#define PBE_BLE5_RAM_FL1INFO13_FALIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO13_FALIGN_IGN                                0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO13_TYPE                                      0x0002U
#define PBE_BLE5_RAM_FL1INFO13_TYPE_M                                    0x0002U
#define PBE_BLE5_RAM_FL1INFO13_TYPE_S                                         1U
#define PBE_BLE5_RAM_FL1INFO13_TYPE_PUBLIC                               0x0000U
#define PBE_BLE5_RAM_FL1INFO13_TYPE_RANDOM                               0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO13_EN                                        0x0001U
#define PBE_BLE5_RAM_FL1INFO13_EN_M                                      0x0001U
#define PBE_BLE5_RAM_FL1INFO13_EN_S                                           0U
#define PBE_BLE5_RAM_FL1INFO13_EN_DIS                                    0x0000U
#define PBE_BLE5_RAM_FL1INFO13_EN_EN                                     0x0001U

//******************************************************************************
// Register: FL1ADRL13
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL13_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRL13_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL13_VAL_S                                          0U

//******************************************************************************
// Register: FL1ADRM13
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM13_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRM13_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM13_VAL_S                                          0U

//******************************************************************************
// Register: FL1ADRH13
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH13_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRH13_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH13_VAL_S                                          0U

//******************************************************************************
// Register: FL1INFO14
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO14_MATCHTMP                                  0x8000U
#define PBE_BLE5_RAM_FL1INFO14_MATCHTMP_M                                0x8000U
#define PBE_BLE5_RAM_FL1INFO14_MATCHTMP_S                                    15U
#define PBE_BLE5_RAM_FL1INFO14_MATCHTMP_NOMATCH                          0x0000U
#define PBE_BLE5_RAM_FL1INFO14_MATCHTMP_MATCH                            0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO14_RESERVED_W                                    11U
#define PBE_BLE5_RAM_FL1INFO14_RESERVED_M                                0x7FF0U
#define PBE_BLE5_RAM_FL1INFO14_RESERVED_S                                     4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO14_PRIVIGN                                   0x0008U
#define PBE_BLE5_RAM_FL1INFO14_PRIVIGN_M                                 0x0008U
#define PBE_BLE5_RAM_FL1INFO14_PRIVIGN_S                                      3U
#define PBE_BLE5_RAM_FL1INFO14_PRIVIGN_NIGN                              0x0000U
#define PBE_BLE5_RAM_FL1INFO14_PRIVIGN_IGN                               0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO14_FALIGN                                    0x0004U
#define PBE_BLE5_RAM_FL1INFO14_FALIGN_M                                  0x0004U
#define PBE_BLE5_RAM_FL1INFO14_FALIGN_S                                       2U
#define PBE_BLE5_RAM_FL1INFO14_FALIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO14_FALIGN_IGN                                0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO14_TYPE                                      0x0002U
#define PBE_BLE5_RAM_FL1INFO14_TYPE_M                                    0x0002U
#define PBE_BLE5_RAM_FL1INFO14_TYPE_S                                         1U
#define PBE_BLE5_RAM_FL1INFO14_TYPE_PUBLIC                               0x0000U
#define PBE_BLE5_RAM_FL1INFO14_TYPE_RANDOM                               0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO14_EN                                        0x0001U
#define PBE_BLE5_RAM_FL1INFO14_EN_M                                      0x0001U
#define PBE_BLE5_RAM_FL1INFO14_EN_S                                           0U
#define PBE_BLE5_RAM_FL1INFO14_EN_DIS                                    0x0000U
#define PBE_BLE5_RAM_FL1INFO14_EN_EN                                     0x0001U

//******************************************************************************
// Register: FL1ADRL14
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL14_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRL14_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL14_VAL_S                                          0U

//******************************************************************************
// Register: FL1ADRM14
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM14_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRM14_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM14_VAL_S                                          0U

//******************************************************************************
// Register: FL1ADRH14
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH14_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRH14_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH14_VAL_S                                          0U

//******************************************************************************
// Register: FL1INFO15
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL1INFO15_MATCHTMP                                  0x8000U
#define PBE_BLE5_RAM_FL1INFO15_MATCHTMP_M                                0x8000U
#define PBE_BLE5_RAM_FL1INFO15_MATCHTMP_S                                    15U
#define PBE_BLE5_RAM_FL1INFO15_MATCHTMP_NOMATCH                          0x0000U
#define PBE_BLE5_RAM_FL1INFO15_MATCHTMP_MATCH                            0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL1INFO15_RESERVED_W                                    11U
#define PBE_BLE5_RAM_FL1INFO15_RESERVED_M                                0x7FF0U
#define PBE_BLE5_RAM_FL1INFO15_RESERVED_S                                     4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL1INFO15_PRIVIGN                                   0x0008U
#define PBE_BLE5_RAM_FL1INFO15_PRIVIGN_M                                 0x0008U
#define PBE_BLE5_RAM_FL1INFO15_PRIVIGN_S                                      3U
#define PBE_BLE5_RAM_FL1INFO15_PRIVIGN_NIGN                              0x0000U
#define PBE_BLE5_RAM_FL1INFO15_PRIVIGN_IGN                               0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL1INFO15_FALIGN                                    0x0004U
#define PBE_BLE5_RAM_FL1INFO15_FALIGN_M                                  0x0004U
#define PBE_BLE5_RAM_FL1INFO15_FALIGN_S                                       2U
#define PBE_BLE5_RAM_FL1INFO15_FALIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL1INFO15_FALIGN_IGN                                0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL1INFO15_TYPE                                      0x0002U
#define PBE_BLE5_RAM_FL1INFO15_TYPE_M                                    0x0002U
#define PBE_BLE5_RAM_FL1INFO15_TYPE_S                                         1U
#define PBE_BLE5_RAM_FL1INFO15_TYPE_PUBLIC                               0x0000U
#define PBE_BLE5_RAM_FL1INFO15_TYPE_RANDOM                               0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL1INFO15_EN                                        0x0001U
#define PBE_BLE5_RAM_FL1INFO15_EN_M                                      0x0001U
#define PBE_BLE5_RAM_FL1INFO15_EN_S                                           0U
#define PBE_BLE5_RAM_FL1INFO15_EN_DIS                                    0x0000U
#define PBE_BLE5_RAM_FL1INFO15_EN_EN                                     0x0001U

//******************************************************************************
// Register: FL1ADRL15
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL1ADRL15_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRL15_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRL15_VAL_S                                          0U

//******************************************************************************
// Register: FL1ADRM15
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL1ADRM15_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRM15_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRM15_VAL_S                                          0U

//******************************************************************************
// Register: FL1ADRH15
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL1ADRH15_VAL_W                                         16U
#define PBE_BLE5_RAM_FL1ADRH15_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL1ADRH15_VAL_S                                          0U

//******************************************************************************
// Register: FL2INFO0
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO0_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL2INFO0_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL2INFO0_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL2INFO0_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL2INFO0_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO0_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL2INFO0_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL2INFO0_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO0_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL2INFO0_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL2INFO0_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL2INFO0_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO0_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO0_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL2INFO0_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL2INFO0_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL2INFO0_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL2INFO0_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO0_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL2INFO0_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL2INFO0_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL2INFO0_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL2INFO0_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO0_EN                                         0x0001U
#define PBE_BLE5_RAM_FL2INFO0_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL2INFO0_EN_S                                            0U
#define PBE_BLE5_RAM_FL2INFO0_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL2INFO0_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL2ADRL0
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL0_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRL0_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL0_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRM0
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM0_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRM0_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM0_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRH0
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH0_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRH0_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH0_VAL_S                                           0U

//******************************************************************************
// Register: FL2INFO1
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO1_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL2INFO1_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL2INFO1_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL2INFO1_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL2INFO1_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO1_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL2INFO1_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL2INFO1_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO1_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL2INFO1_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL2INFO1_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL2INFO1_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO1_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO1_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL2INFO1_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL2INFO1_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL2INFO1_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL2INFO1_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO1_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL2INFO1_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL2INFO1_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL2INFO1_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL2INFO1_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO1_EN                                         0x0001U
#define PBE_BLE5_RAM_FL2INFO1_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL2INFO1_EN_S                                            0U
#define PBE_BLE5_RAM_FL2INFO1_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL2INFO1_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL2ADRL1
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL1_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRL1_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL1_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRM1
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM1_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRM1_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM1_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRH1
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH1_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRH1_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH1_VAL_S                                           0U

//******************************************************************************
// Register: FL2INFO2
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO2_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL2INFO2_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL2INFO2_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL2INFO2_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL2INFO2_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO2_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL2INFO2_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL2INFO2_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO2_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL2INFO2_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL2INFO2_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL2INFO2_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO2_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO2_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL2INFO2_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL2INFO2_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL2INFO2_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL2INFO2_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO2_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL2INFO2_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL2INFO2_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL2INFO2_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL2INFO2_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO2_EN                                         0x0001U
#define PBE_BLE5_RAM_FL2INFO2_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL2INFO2_EN_S                                            0U
#define PBE_BLE5_RAM_FL2INFO2_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL2INFO2_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL2ADRL2
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL2_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRL2_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL2_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRM2
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM2_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRM2_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM2_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRH2
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH2_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRH2_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH2_VAL_S                                           0U

//******************************************************************************
// Register: FL2INFO3
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO3_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL2INFO3_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL2INFO3_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL2INFO3_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL2INFO3_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO3_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL2INFO3_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL2INFO3_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO3_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL2INFO3_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL2INFO3_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL2INFO3_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO3_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO3_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL2INFO3_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL2INFO3_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL2INFO3_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL2INFO3_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO3_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL2INFO3_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL2INFO3_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL2INFO3_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL2INFO3_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO3_EN                                         0x0001U
#define PBE_BLE5_RAM_FL2INFO3_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL2INFO3_EN_S                                            0U
#define PBE_BLE5_RAM_FL2INFO3_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL2INFO3_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL2ADRL3
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL3_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRL3_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL3_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRM3
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM3_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRM3_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM3_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRH3
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH3_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRH3_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH3_VAL_S                                           0U

//******************************************************************************
// Register: FL2INFO4
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO4_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL2INFO4_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL2INFO4_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL2INFO4_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL2INFO4_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO4_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL2INFO4_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL2INFO4_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO4_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL2INFO4_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL2INFO4_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL2INFO4_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO4_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO4_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL2INFO4_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL2INFO4_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL2INFO4_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL2INFO4_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO4_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL2INFO4_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL2INFO4_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL2INFO4_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL2INFO4_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO4_EN                                         0x0001U
#define PBE_BLE5_RAM_FL2INFO4_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL2INFO4_EN_S                                            0U
#define PBE_BLE5_RAM_FL2INFO4_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL2INFO4_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL2ADRL4
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL4_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRL4_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL4_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRM4
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM4_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRM4_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM4_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRH4
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH4_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRH4_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH4_VAL_S                                           0U

//******************************************************************************
// Register: FL2INFO5
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO5_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL2INFO5_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL2INFO5_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL2INFO5_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL2INFO5_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO5_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL2INFO5_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL2INFO5_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO5_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL2INFO5_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL2INFO5_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL2INFO5_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO5_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO5_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL2INFO5_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL2INFO5_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL2INFO5_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL2INFO5_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO5_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL2INFO5_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL2INFO5_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL2INFO5_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL2INFO5_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO5_EN                                         0x0001U
#define PBE_BLE5_RAM_FL2INFO5_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL2INFO5_EN_S                                            0U
#define PBE_BLE5_RAM_FL2INFO5_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL2INFO5_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL2ADRL5
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL5_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRL5_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL5_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRM5
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM5_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRM5_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM5_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRH5
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH5_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRH5_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH5_VAL_S                                           0U

//******************************************************************************
// Register: FL2INFO6
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO6_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL2INFO6_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL2INFO6_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL2INFO6_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL2INFO6_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO6_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL2INFO6_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL2INFO6_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO6_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL2INFO6_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL2INFO6_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL2INFO6_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO6_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO6_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL2INFO6_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL2INFO6_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL2INFO6_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL2INFO6_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO6_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL2INFO6_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL2INFO6_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL2INFO6_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL2INFO6_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO6_EN                                         0x0001U
#define PBE_BLE5_RAM_FL2INFO6_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL2INFO6_EN_S                                            0U
#define PBE_BLE5_RAM_FL2INFO6_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL2INFO6_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL2ADRL6
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL6_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRL6_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL6_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRM6
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM6_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRM6_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM6_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRH6
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH6_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRH6_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH6_VAL_S                                           0U

//******************************************************************************
// Register: FL2INFO7
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO7_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL2INFO7_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL2INFO7_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL2INFO7_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL2INFO7_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO7_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL2INFO7_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL2INFO7_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO7_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL2INFO7_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL2INFO7_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL2INFO7_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO7_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO7_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL2INFO7_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL2INFO7_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL2INFO7_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL2INFO7_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO7_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL2INFO7_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL2INFO7_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL2INFO7_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL2INFO7_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO7_EN                                         0x0001U
#define PBE_BLE5_RAM_FL2INFO7_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL2INFO7_EN_S                                            0U
#define PBE_BLE5_RAM_FL2INFO7_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL2INFO7_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL2ADRL7
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL7_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRL7_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL7_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRM7
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM7_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRM7_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM7_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRH7
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH7_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRH7_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH7_VAL_S                                           0U

//******************************************************************************
// Register: FL2INFO8
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO8_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL2INFO8_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL2INFO8_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL2INFO8_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL2INFO8_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO8_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL2INFO8_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL2INFO8_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO8_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL2INFO8_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL2INFO8_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL2INFO8_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO8_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO8_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL2INFO8_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL2INFO8_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL2INFO8_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL2INFO8_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO8_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL2INFO8_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL2INFO8_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL2INFO8_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL2INFO8_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO8_EN                                         0x0001U
#define PBE_BLE5_RAM_FL2INFO8_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL2INFO8_EN_S                                            0U
#define PBE_BLE5_RAM_FL2INFO8_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL2INFO8_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL2ADRL8
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL8_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRL8_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL8_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRM8
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM8_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRM8_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM8_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRH8
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH8_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRH8_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH8_VAL_S                                           0U

//******************************************************************************
// Register: FL2INFO9
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO9_MATCHTMP                                   0x8000U
#define PBE_BLE5_RAM_FL2INFO9_MATCHTMP_M                                 0x8000U
#define PBE_BLE5_RAM_FL2INFO9_MATCHTMP_S                                     15U
#define PBE_BLE5_RAM_FL2INFO9_MATCHTMP_NOMATCH                           0x0000U
#define PBE_BLE5_RAM_FL2INFO9_MATCHTMP_MATCH                             0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO9_RESERVED_W                                     11U
#define PBE_BLE5_RAM_FL2INFO9_RESERVED_M                                 0x7FF0U
#define PBE_BLE5_RAM_FL2INFO9_RESERVED_S                                      4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO9_PRIVIGN                                    0x0008U
#define PBE_BLE5_RAM_FL2INFO9_PRIVIGN_M                                  0x0008U
#define PBE_BLE5_RAM_FL2INFO9_PRIVIGN_S                                       3U
#define PBE_BLE5_RAM_FL2INFO9_PRIVIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO9_PRIVIGN_IGN                                0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO9_FALIGN                                     0x0004U
#define PBE_BLE5_RAM_FL2INFO9_FALIGN_M                                   0x0004U
#define PBE_BLE5_RAM_FL2INFO9_FALIGN_S                                        2U
#define PBE_BLE5_RAM_FL2INFO9_FALIGN_NIGN                                0x0000U
#define PBE_BLE5_RAM_FL2INFO9_FALIGN_IGN                                 0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO9_TYPE                                       0x0002U
#define PBE_BLE5_RAM_FL2INFO9_TYPE_M                                     0x0002U
#define PBE_BLE5_RAM_FL2INFO9_TYPE_S                                          1U
#define PBE_BLE5_RAM_FL2INFO9_TYPE_PUBLIC                                0x0000U
#define PBE_BLE5_RAM_FL2INFO9_TYPE_RANDOM                                0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO9_EN                                         0x0001U
#define PBE_BLE5_RAM_FL2INFO9_EN_M                                       0x0001U
#define PBE_BLE5_RAM_FL2INFO9_EN_S                                            0U
#define PBE_BLE5_RAM_FL2INFO9_EN_DIS                                     0x0000U
#define PBE_BLE5_RAM_FL2INFO9_EN_EN                                      0x0001U

//******************************************************************************
// Register: FL2ADRL9
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL9_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRL9_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL9_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRM9
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM9_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRM9_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM9_VAL_S                                           0U

//******************************************************************************
// Register: FL2ADRH9
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH9_VAL_W                                          16U
#define PBE_BLE5_RAM_FL2ADRH9_VAL_M                                      0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH9_VAL_S                                           0U

//******************************************************************************
// Register: FL2INFO10
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO10_MATCHTMP                                  0x8000U
#define PBE_BLE5_RAM_FL2INFO10_MATCHTMP_M                                0x8000U
#define PBE_BLE5_RAM_FL2INFO10_MATCHTMP_S                                    15U
#define PBE_BLE5_RAM_FL2INFO10_MATCHTMP_NOMATCH                          0x0000U
#define PBE_BLE5_RAM_FL2INFO10_MATCHTMP_MATCH                            0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO10_RESERVED_W                                    11U
#define PBE_BLE5_RAM_FL2INFO10_RESERVED_M                                0x7FF0U
#define PBE_BLE5_RAM_FL2INFO10_RESERVED_S                                     4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO10_PRIVIGN                                   0x0008U
#define PBE_BLE5_RAM_FL2INFO10_PRIVIGN_M                                 0x0008U
#define PBE_BLE5_RAM_FL2INFO10_PRIVIGN_S                                      3U
#define PBE_BLE5_RAM_FL2INFO10_PRIVIGN_NIGN                              0x0000U
#define PBE_BLE5_RAM_FL2INFO10_PRIVIGN_IGN                               0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO10_FALIGN                                    0x0004U
#define PBE_BLE5_RAM_FL2INFO10_FALIGN_M                                  0x0004U
#define PBE_BLE5_RAM_FL2INFO10_FALIGN_S                                       2U
#define PBE_BLE5_RAM_FL2INFO10_FALIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO10_FALIGN_IGN                                0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO10_TYPE                                      0x0002U
#define PBE_BLE5_RAM_FL2INFO10_TYPE_M                                    0x0002U
#define PBE_BLE5_RAM_FL2INFO10_TYPE_S                                         1U
#define PBE_BLE5_RAM_FL2INFO10_TYPE_PUBLIC                               0x0000U
#define PBE_BLE5_RAM_FL2INFO10_TYPE_RANDOM                               0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO10_EN                                        0x0001U
#define PBE_BLE5_RAM_FL2INFO10_EN_M                                      0x0001U
#define PBE_BLE5_RAM_FL2INFO10_EN_S                                           0U
#define PBE_BLE5_RAM_FL2INFO10_EN_DIS                                    0x0000U
#define PBE_BLE5_RAM_FL2INFO10_EN_EN                                     0x0001U

//******************************************************************************
// Register: FL2ADRL10
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL10_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRL10_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL10_VAL_S                                          0U

//******************************************************************************
// Register: FL2ADRM10
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM10_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRM10_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM10_VAL_S                                          0U

//******************************************************************************
// Register: FL2ADRH10
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH10_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRH10_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH10_VAL_S                                          0U

//******************************************************************************
// Register: FL2INFO11
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO11_MATCHTMP                                  0x8000U
#define PBE_BLE5_RAM_FL2INFO11_MATCHTMP_M                                0x8000U
#define PBE_BLE5_RAM_FL2INFO11_MATCHTMP_S                                    15U
#define PBE_BLE5_RAM_FL2INFO11_MATCHTMP_NOMATCH                          0x0000U
#define PBE_BLE5_RAM_FL2INFO11_MATCHTMP_MATCH                            0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO11_RESERVED_W                                    11U
#define PBE_BLE5_RAM_FL2INFO11_RESERVED_M                                0x7FF0U
#define PBE_BLE5_RAM_FL2INFO11_RESERVED_S                                     4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO11_PRIVIGN                                   0x0008U
#define PBE_BLE5_RAM_FL2INFO11_PRIVIGN_M                                 0x0008U
#define PBE_BLE5_RAM_FL2INFO11_PRIVIGN_S                                      3U
#define PBE_BLE5_RAM_FL2INFO11_PRIVIGN_NIGN                              0x0000U
#define PBE_BLE5_RAM_FL2INFO11_PRIVIGN_IGN                               0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO11_FALIGN                                    0x0004U
#define PBE_BLE5_RAM_FL2INFO11_FALIGN_M                                  0x0004U
#define PBE_BLE5_RAM_FL2INFO11_FALIGN_S                                       2U
#define PBE_BLE5_RAM_FL2INFO11_FALIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO11_FALIGN_IGN                                0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO11_TYPE                                      0x0002U
#define PBE_BLE5_RAM_FL2INFO11_TYPE_M                                    0x0002U
#define PBE_BLE5_RAM_FL2INFO11_TYPE_S                                         1U
#define PBE_BLE5_RAM_FL2INFO11_TYPE_PUBLIC                               0x0000U
#define PBE_BLE5_RAM_FL2INFO11_TYPE_RANDOM                               0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO11_EN                                        0x0001U
#define PBE_BLE5_RAM_FL2INFO11_EN_M                                      0x0001U
#define PBE_BLE5_RAM_FL2INFO11_EN_S                                           0U
#define PBE_BLE5_RAM_FL2INFO11_EN_DIS                                    0x0000U
#define PBE_BLE5_RAM_FL2INFO11_EN_EN                                     0x0001U

//******************************************************************************
// Register: FL2ADRL11
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL11_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRL11_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL11_VAL_S                                          0U

//******************************************************************************
// Register: FL2ADRM11
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM11_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRM11_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM11_VAL_S                                          0U

//******************************************************************************
// Register: FL2ADRH11
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH11_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRH11_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH11_VAL_S                                          0U

//******************************************************************************
// Register: FL2INFO12
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO12_MATCHTMP                                  0x8000U
#define PBE_BLE5_RAM_FL2INFO12_MATCHTMP_M                                0x8000U
#define PBE_BLE5_RAM_FL2INFO12_MATCHTMP_S                                    15U
#define PBE_BLE5_RAM_FL2INFO12_MATCHTMP_NOMATCH                          0x0000U
#define PBE_BLE5_RAM_FL2INFO12_MATCHTMP_MATCH                            0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO12_RESERVED_W                                    11U
#define PBE_BLE5_RAM_FL2INFO12_RESERVED_M                                0x7FF0U
#define PBE_BLE5_RAM_FL2INFO12_RESERVED_S                                     4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO12_PRIVIGN                                   0x0008U
#define PBE_BLE5_RAM_FL2INFO12_PRIVIGN_M                                 0x0008U
#define PBE_BLE5_RAM_FL2INFO12_PRIVIGN_S                                      3U
#define PBE_BLE5_RAM_FL2INFO12_PRIVIGN_NIGN                              0x0000U
#define PBE_BLE5_RAM_FL2INFO12_PRIVIGN_IGN                               0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO12_FALIGN                                    0x0004U
#define PBE_BLE5_RAM_FL2INFO12_FALIGN_M                                  0x0004U
#define PBE_BLE5_RAM_FL2INFO12_FALIGN_S                                       2U
#define PBE_BLE5_RAM_FL2INFO12_FALIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO12_FALIGN_IGN                                0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO12_TYPE                                      0x0002U
#define PBE_BLE5_RAM_FL2INFO12_TYPE_M                                    0x0002U
#define PBE_BLE5_RAM_FL2INFO12_TYPE_S                                         1U
#define PBE_BLE5_RAM_FL2INFO12_TYPE_PUBLIC                               0x0000U
#define PBE_BLE5_RAM_FL2INFO12_TYPE_RANDOM                               0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO12_EN                                        0x0001U
#define PBE_BLE5_RAM_FL2INFO12_EN_M                                      0x0001U
#define PBE_BLE5_RAM_FL2INFO12_EN_S                                           0U
#define PBE_BLE5_RAM_FL2INFO12_EN_DIS                                    0x0000U
#define PBE_BLE5_RAM_FL2INFO12_EN_EN                                     0x0001U

//******************************************************************************
// Register: FL2ADRL12
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL12_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRL12_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL12_VAL_S                                          0U

//******************************************************************************
// Register: FL2ADRM12
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM12_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRM12_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM12_VAL_S                                          0U

//******************************************************************************
// Register: FL2ADRH12
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH12_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRH12_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH12_VAL_S                                          0U

//******************************************************************************
// Register: FL2INFO13
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO13_MATCHTMP                                  0x8000U
#define PBE_BLE5_RAM_FL2INFO13_MATCHTMP_M                                0x8000U
#define PBE_BLE5_RAM_FL2INFO13_MATCHTMP_S                                    15U
#define PBE_BLE5_RAM_FL2INFO13_MATCHTMP_NOMATCH                          0x0000U
#define PBE_BLE5_RAM_FL2INFO13_MATCHTMP_MATCH                            0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO13_RESERVED_W                                    11U
#define PBE_BLE5_RAM_FL2INFO13_RESERVED_M                                0x7FF0U
#define PBE_BLE5_RAM_FL2INFO13_RESERVED_S                                     4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO13_PRIVIGN                                   0x0008U
#define PBE_BLE5_RAM_FL2INFO13_PRIVIGN_M                                 0x0008U
#define PBE_BLE5_RAM_FL2INFO13_PRIVIGN_S                                      3U
#define PBE_BLE5_RAM_FL2INFO13_PRIVIGN_NIGN                              0x0000U
#define PBE_BLE5_RAM_FL2INFO13_PRIVIGN_IGN                               0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO13_FALIGN                                    0x0004U
#define PBE_BLE5_RAM_FL2INFO13_FALIGN_M                                  0x0004U
#define PBE_BLE5_RAM_FL2INFO13_FALIGN_S                                       2U
#define PBE_BLE5_RAM_FL2INFO13_FALIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO13_FALIGN_IGN                                0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO13_TYPE                                      0x0002U
#define PBE_BLE5_RAM_FL2INFO13_TYPE_M                                    0x0002U
#define PBE_BLE5_RAM_FL2INFO13_TYPE_S                                         1U
#define PBE_BLE5_RAM_FL2INFO13_TYPE_PUBLIC                               0x0000U
#define PBE_BLE5_RAM_FL2INFO13_TYPE_RANDOM                               0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO13_EN                                        0x0001U
#define PBE_BLE5_RAM_FL2INFO13_EN_M                                      0x0001U
#define PBE_BLE5_RAM_FL2INFO13_EN_S                                           0U
#define PBE_BLE5_RAM_FL2INFO13_EN_DIS                                    0x0000U
#define PBE_BLE5_RAM_FL2INFO13_EN_EN                                     0x0001U

//******************************************************************************
// Register: FL2ADRL13
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL13_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRL13_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL13_VAL_S                                          0U

//******************************************************************************
// Register: FL2ADRM13
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM13_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRM13_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM13_VAL_S                                          0U

//******************************************************************************
// Register: FL2ADRH13
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH13_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRH13_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH13_VAL_S                                          0U

//******************************************************************************
// Register: FL2INFO14
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO14_MATCHTMP                                  0x8000U
#define PBE_BLE5_RAM_FL2INFO14_MATCHTMP_M                                0x8000U
#define PBE_BLE5_RAM_FL2INFO14_MATCHTMP_S                                    15U
#define PBE_BLE5_RAM_FL2INFO14_MATCHTMP_NOMATCH                          0x0000U
#define PBE_BLE5_RAM_FL2INFO14_MATCHTMP_MATCH                            0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO14_RESERVED_W                                    11U
#define PBE_BLE5_RAM_FL2INFO14_RESERVED_M                                0x7FF0U
#define PBE_BLE5_RAM_FL2INFO14_RESERVED_S                                     4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO14_PRIVIGN                                   0x0008U
#define PBE_BLE5_RAM_FL2INFO14_PRIVIGN_M                                 0x0008U
#define PBE_BLE5_RAM_FL2INFO14_PRIVIGN_S                                      3U
#define PBE_BLE5_RAM_FL2INFO14_PRIVIGN_NIGN                              0x0000U
#define PBE_BLE5_RAM_FL2INFO14_PRIVIGN_IGN                               0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO14_FALIGN                                    0x0004U
#define PBE_BLE5_RAM_FL2INFO14_FALIGN_M                                  0x0004U
#define PBE_BLE5_RAM_FL2INFO14_FALIGN_S                                       2U
#define PBE_BLE5_RAM_FL2INFO14_FALIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO14_FALIGN_IGN                                0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO14_TYPE                                      0x0002U
#define PBE_BLE5_RAM_FL2INFO14_TYPE_M                                    0x0002U
#define PBE_BLE5_RAM_FL2INFO14_TYPE_S                                         1U
#define PBE_BLE5_RAM_FL2INFO14_TYPE_PUBLIC                               0x0000U
#define PBE_BLE5_RAM_FL2INFO14_TYPE_RANDOM                               0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO14_EN                                        0x0001U
#define PBE_BLE5_RAM_FL2INFO14_EN_M                                      0x0001U
#define PBE_BLE5_RAM_FL2INFO14_EN_S                                           0U
#define PBE_BLE5_RAM_FL2INFO14_EN_DIS                                    0x0000U
#define PBE_BLE5_RAM_FL2INFO14_EN_EN                                     0x0001U

//******************************************************************************
// Register: FL2ADRL14
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL14_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRL14_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL14_VAL_S                                          0U

//******************************************************************************
// Register: FL2ADRM14
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM14_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRM14_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM14_VAL_S                                          0U

//******************************************************************************
// Register: FL2ADRH14
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH14_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRH14_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH14_VAL_S                                          0U

//******************************************************************************
// Register: FL2INFO15
//******************************************************************************
// Field: [15:15] matchtmp
//
// Temporary variable used in matching, ignore from CM0
#define PBE_BLE5_RAM_FL2INFO15_MATCHTMP                                  0x8000U
#define PBE_BLE5_RAM_FL2INFO15_MATCHTMP_M                                0x8000U
#define PBE_BLE5_RAM_FL2INFO15_MATCHTMP_S                                    15U
#define PBE_BLE5_RAM_FL2INFO15_MATCHTMP_NOMATCH                          0x0000U
#define PBE_BLE5_RAM_FL2INFO15_MATCHTMP_MATCH                            0x8000U

// Field: [14:4] reserved
//
// Reserved for future use					 
#define PBE_BLE5_RAM_FL2INFO15_RESERVED_W                                    11U
#define PBE_BLE5_RAM_FL2INFO15_RESERVED_M                                0x7FF0U
#define PBE_BLE5_RAM_FL2INFO15_RESERVED_S                                     4U

// Field: [3:3] privign
//
// Ignore control for privacy
#define PBE_BLE5_RAM_FL2INFO15_PRIVIGN                                   0x0008U
#define PBE_BLE5_RAM_FL2INFO15_PRIVIGN_M                                 0x0008U
#define PBE_BLE5_RAM_FL2INFO15_PRIVIGN_S                                      3U
#define PBE_BLE5_RAM_FL2INFO15_PRIVIGN_NIGN                              0x0000U
#define PBE_BLE5_RAM_FL2INFO15_PRIVIGN_IGN                               0x0008U

// Field: [2:2] falign
//
// Ignore in accept list context
#define PBE_BLE5_RAM_FL2INFO15_FALIGN                                    0x0004U
#define PBE_BLE5_RAM_FL2INFO15_FALIGN_M                                  0x0004U
#define PBE_BLE5_RAM_FL2INFO15_FALIGN_S                                       2U
#define PBE_BLE5_RAM_FL2INFO15_FALIGN_NIGN                               0x0000U
#define PBE_BLE5_RAM_FL2INFO15_FALIGN_IGN                                0x0004U

// Field: [1:1] type
//
// Address type
#define PBE_BLE5_RAM_FL2INFO15_TYPE                                      0x0002U
#define PBE_BLE5_RAM_FL2INFO15_TYPE_M                                    0x0002U
#define PBE_BLE5_RAM_FL2INFO15_TYPE_S                                         1U
#define PBE_BLE5_RAM_FL2INFO15_TYPE_PUBLIC                               0x0000U
#define PBE_BLE5_RAM_FL2INFO15_TYPE_RANDOM                               0x0002U

// Field: [0:0] en
//
// Enable
#define PBE_BLE5_RAM_FL2INFO15_EN                                        0x0001U
#define PBE_BLE5_RAM_FL2INFO15_EN_M                                      0x0001U
#define PBE_BLE5_RAM_FL2INFO15_EN_S                                           0U
#define PBE_BLE5_RAM_FL2INFO15_EN_DIS                                    0x0000U
#define PBE_BLE5_RAM_FL2INFO15_EN_EN                                     0x0001U

//******************************************************************************
// Register: FL2ADRL15
//******************************************************************************
// Field: [15:0] val
//
// bits 15:0 of address
#define PBE_BLE5_RAM_FL2ADRL15_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRL15_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRL15_VAL_S                                          0U

//******************************************************************************
// Register: FL2ADRM15
//******************************************************************************
// Field: [15:0] val
//
// bits 31:16 of address
#define PBE_BLE5_RAM_FL2ADRM15_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRM15_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRM15_VAL_S                                          0U

//******************************************************************************
// Register: FL2ADRH15
//******************************************************************************
// Field: [15:0] val
//
// bits 47:32 of address
#define PBE_BLE5_RAM_FL2ADRH15_VAL_W                                         16U
#define PBE_BLE5_RAM_FL2ADRH15_VAL_M                                     0xFFFFU
#define PBE_BLE5_RAM_FL2ADRH15_VAL_S                                          0U


#endif // __PBE_BLE5_RAM_REGS_H
