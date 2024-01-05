/******************************************************************************
*  Filename:       hw_memmap_h
******************************************************************************
*  Copyright (c) 2021 Texas Instruments Incorporated. All rights reserved.
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

#ifndef __HW_MEMMAP_H__
#define __HW_MEMMAP_H__

//*****************************************************************************
//
// The following are defines for the base address of the memories and
// peripherals on the CPU_MMAP interface
//
//*****************************************************************************
#define FLASH_MAIN_BASE         0x00000000 // FLASH_MAIN
   #define FLASH_MAIN_SIZE                 0x100000
#define ROM_BASE                0x0F000000 // ROM
   #define ROM_SIZE                        0x8000
#define SRAM_BASE               0x20000000 // SRAM
   #define SRAM_SIZE                       0x28800
#define PMCTL_BASE              0x40000000 // PMCTL
#define CKMD_BASE               0x40001000 // CKMD
#define RTC_BASE                0x40002000 // RTC
#define IOC_BASE                0x40003000 // IOC
#define SYS0_BASE               0x40004000 // SYS0
#define EVTULL_BASE             0x40005000 // EVTULL
#define PMUD_BASE               0x40006000 // PMUD
#define TCM_BASE                0x40007000 // TCM
#define DBGSS_BASE              0x4000F000 // DBGSS
#define CLKCTL_BASE             0x40020000 // CLKCTL
#define FLASH_BASE              0x40021000 // FLASH
#define SYSTIM_BASE             0x40022000 // SYSTIM
#define GPIO_BASE               0x40023000 // GPIO
#define VIMS_BASE               0x40024000 // VIMS
#define EVTSVT_BASE             0x40025000 // EVTSVT
#define SPI0_BASE               0x40030000 // SPI
#define SPI1_BASE               0x40031000 // SPI
#define UART0_BASE              0x40034000 // UART
#define UART1_BASE              0x40035000 // UART
#define I2C0_BASE               0x40038000 // I2C
#define ADC_BASE                0x40050000 // ADC
#define HSM_BASE                0x40053000 // HSM
#define LGPT0_BASE              0x40060000 // LGPT0
#define LGPT1_BASE              0x40061000 // LGPT1
#define LGPT2_BASE              0x40062000 // LGPT2
#define LGPT3_BASE              0x40063000 // LGPT3
#define LRFDDBELL_BASE          0x40080000 // LRFDDBELL
#define LRFDPBE_BASE            0x40081000 // LRFDPBE
#define LRFDTXF_BASE            0x40081800 // LRFDTXF
#define LRFDRXF_BASE            0x40081C00 // LRFDRXF
#define LRFDMDM_BASE            0x40082000 // LRFDMDM
#define LRFDRFE_BASE            0x40083000 // LRFDRFE
#define LRFDTRC_BASE            0x40084000 // LRFDTRC
#define LRFDS2R_BASE            0x40085000 // LRFDS2R
#define LRFD_PBERAM_BASE        0x40090000 // LRFD_PBERAM
   #define LRFD_PBERAM_SIZE                0x1000
#define LRFD_BUFRAM_BASE        0x40092000 // LRFD_BUFRAM
   #define LRFD_BUFRAM_SIZE                0x600
#define LRFD_MCERAM_BASE        0x40094000 // LRFD_MCERAM
   #define LRFD_MCERAM_SIZE                0x800
#define LRFD_RFERAM_BASE        0x40096000 // LRFD_RFERAM
   #define LRFD_RFERAM_SIZE                0x1000
#define LRFD_S2RRAM_BASE        0x40098000 // LRFD_S2RRAM
   #define LRFD_S2RRAM_SIZE                0x1000
#define AES_BASE                0x400C0000 // AES
#define I2S_BASE                0x400C1000 // I2S
#define DMA_BASE                0x400C4000 // DMA
#define SRAMCTRL_BASE           0x400C5000 // SRAMCTRL
#define CANFD_BASE              0x400D0000 // CANFD
#define CANFD_SRAM_BASE         0x400D1000 // CANFD_SRAM
#define VCE_BASE                0x400D2000 // VCE
#define VCE_TOPSMRAM_BASE       0x400D3000 // VCE_TOPSMRAM
   #define VCE_TOPSMRAM_SIZE               0x1000
#define VCERAM_DATA0_BASE       0x400E0000 // VCERAM_DATA0
   #define VCERAM_DATA0_SIZE               0x2000
#define VCERAM_DATA1_BASE       0x400E2000 // VCERAM_DATA1
   #define VCERAM_DATA1_SIZE               0x2000
#define HSMCRYPTO_BASE          0x400F0000 // HSMCRYPTO
#define FCFG_BASE               0x4E000000 // FCFG
   #define FCFG_SIZE                       0x400
#define CCFG_BASE               0x4E020000 // CCFG
   #define CCFG_SIZE                       0x800
#define OTP_BASE                0x4E020800 // OTP
   #define OTP_SIZE                        0x800
#define SCFG_BASE               0x4E040000 // SCFG
   #define SCFG_SIZE                       0x400
#define VLOG_BASE               0x4E060000 // VLOG
   #define VLOG_SIZE                       0x800
#define OTP_BKP_BASE            0x4E060800 // OTP_BKP
   #define OTP_BKP_SIZE                    0x800
#define FPB_BASE                0xE0002000 // FPB
#define ICB_BASE                0xE000E000 // ICB
#define SYSTICK_BASE            0xE000E010 // SYSTICK
#define NVIC_BASE               0xE000E100 // NVIC
#define SCB_BASE                0xE000ECFC // SCB
#define MPU_BASE                0xE000ED90 // MPU
#define SAU_BASE                0xE000EDD0 // SAU
#define DCB_BASE                0xE000EDF0 // DCB
#define FPE_BASE                0xE000EF34 // FPE
#define DIB_BASE                0xE000EFB0 // DIB
#define TPIU_BASE               0xE0040000 // TPIU
#define CPU_ROM_TABLE_BASE      0xE00FF000 // CPU_ROM_TABLE

#endif // __HW_MEMMAP__
