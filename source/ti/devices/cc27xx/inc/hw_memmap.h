/******************************************************************************
*  Filename:       hw_memmap_h
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
#define FLASH_MAIN_NS_BASE      0x10000000 // FLASH_MAIN_NS
#define ROM_BASE                0x0F000000 // ROM
   #define ROM_SIZE                        0x8000
#define SRAM_BASE               0x20000000 // SRAM
   #define SRAM_SIZE                       0x28800
#define SRAM_NS_BASE            0x30000000 // SRAM_NS
#define PMCTL_BASE              0x40000000 // PMCTL
#define PMCTL_NS_BASE           0x50000000 // PMCTL_NS
#define CKMD_BASE               0x40001000 // CKMD
#define CKMD_NS_BASE            0x50001000 // CKMD_NS
#define RTC_BASE                0x40002000 // RTC
#define RTC_NS_BASE             0x50002000 // RTC_NS
#define IOC_BASE                0x40003000 // IOC
#define IOC_NS_BASE             0x50003000 // IOC_NS
#define SYS0_BASE               0x40004000 // SYS0
#define SYS0_NS_BASE            0x50004000 // SYS0_NS
#define EVTULL_BASE             0x40005000 // EVTULL
#define EVTULL_NS_BASE          0x50005000 // EVTULL_NS
#define PMUD_BASE               0x40006000 // PMUD
#define PMUD_NS_BASE            0x50006000 // PMUD_NS
#define TCM_BASE                0x40007000 // TCM
#define DBGSS_BASE              0x4000F000 // DBGSS
#define DBGSS_NS_BASE           0x5000F000 // DBGSS_NS
#define CLKCTL_BASE             0x40020000 // CLKCTL
#define CLKCTL_NS_BASE          0x50020000 // CLKCTL_NS
#define FLASH_BASE              0x40021000 // FLASH
#define SYSTIM_BASE             0x40022000 // SYSTIM
#define SYSTIM_NS_BASE          0x50022000 // SYSTIM_NS
#define GPIO_BASE               0x40023000 // GPIO
#define GPIO_NS_BASE            0x50023000 // GPIO_NS
#define VIMS_BASE               0x40024000 // VIMS
#define VIMS_NS_BASE            0x50024000 // VIMS_NS
#define EVTSVT_BASE             0x40025000 // EVTSVT
#define EVTSVT_NS_BASE          0x50025000 // EVTSVT_NS
#define SPI0_BASE               0x40030000 // SPI
#define SPI0_NS_BASE            0x50030000 // SPI_NS
#define SPI1_BASE               0x40031000 // SPI
#define SPI1_NS_BASE            0x50031000 // SPI_NS
#define UART0_BASE              0x40034000 // UART0
#define UART0_NS_BASE           0x50034000 // UART0_NS
#define UART1_BASE              0x40035000 // UART1
#define UART1_NS_BASE           0x50035000 // UART1_NS
#define I2C0_BASE               0x40038000 // I2C0
#define I2C0_NS_BASE            0x50038000 // I2C0_NS
#define ADC_BASE                0x40050000 // ADC
#define ADC_NS_BASE             0x50050000 // ADC_NS
#define HSM_BASE                0x40053000 // HSM
#define HSM_NS_BASE             0x50053000 // HSM_NS
#define LGPT0_BASE              0x40060000 // LGPT0
#define LGPT0_NS_BASE           0x50060000 // LGPT0_NS
#define LGPT1_BASE              0x40061000 // LGPT1
#define LGPT1_NS_BASE           0x50061000 // LGPT1_NS
#define LGPT2_BASE              0x40062000 // LGPT2
#define LGPT2_NS_BASE           0x50062000 // LGPT2_NS
#define LGPT3_BASE              0x40063000 // LGPT3
#define LGPT3_NS_BASE           0x50063000 // LGPT3_NS
#define LRFDDBELL_BASE          0x40080000 // LRFDDBELL
#define LRFDDBELL_NS_BASE       0x50080000 // LRFDDBELL_NS
#define LRFDPBE_BASE            0x40081000 // LRFDPBE
#define LRFDPBE_NS_BASE         0x50081000 // LRFDPBE_NS
#define LRFDPBE32_BASE          0x40081400 // LRFDPBE32
#define LRFDPBE32_NS_BASE       0x50081400 // LRFDPBE32_NS
#define LRFDTXF_BASE            0x40081800 // LRFDTXF
#define LRFDTXF_NS_BASE         0x50081800 // LRFDTXF_NS
#define LRFDRXF_BASE            0x40081C00 // LRFDRXF
#define LRFDRXF_NS_BASE         0x50081C00 // LRFDRXF_NS
#define LRFDMDM_BASE            0x40082000 // LRFDMDM
#define LRFDMDM_NS_BASE         0x50082000 // LRFDMDM_NS
#define LRFDMDM32_BASE          0x40082400 // LRFDMDM32
#define LRFDMDM32_NS_BASE       0x50082400 // LRFDMDM32_NS
#define LRFDRFE_BASE            0x40083000 // LRFDRFE
#define LRFDRFE_NS_BASE         0x50083000 // LRFDRFE_NS
#define LRFDRFE32_BASE          0x40083400 // LRFDRFE32
#define LRFDRFE32_NS_BASE       0x50083400 // LRFDRFE32_NS
#define LRFDTRC_BASE            0x40084000 // LRFDTRC
#define LRFDTRC_NS_BASE         0x50084000 // LRFDTRC_NS
#define LRFDS2R_BASE            0x40085000 // LRFDS2R
#define LRFDS2R_NS_BASE         0x50085000 // LRFDS2R_NS
#define LRFD_PBERAM_BASE        0x40090000 // LRFD_PBERAM
   #define LRFD_PBERAM_SIZE                0x1000
#define LRFD_PBERAM_NS_BASE     0x50090000 // LRFD_PBERAM_NS
#define LRFD_BUFRAM_BASE        0x40092000 // LRFD_BUFRAM
   #define LRFD_BUFRAM_SIZE                0x600
#define LRFD_BUFRAM_NS_BASE     0x50092000 // LRFD_BUFRAM_NS
#define LRFD_MCERAM_BASE        0x40094000 // LRFD_MCERAM
   #define LRFD_MCERAM_SIZE                0x800
#define LRFD_MCERAM_NS_BASE     0x50094000 // LRFD_MCERAM_NS
#define LRFD_RFERAM_BASE        0x40096000 // LRFD_RFERAM
   #define LRFD_RFERAM_SIZE                0x1000
#define LRFD_RFERAM_NS_BASE     0x50096000 // LRFD_RFERAM_NS
#define LRFD_S2RRAM_BASE        0x40098000 // LRFD_S2RRAM
   #define LRFD_S2RRAM_SIZE                0x1000
#define LRFD_S2RRAM_NS_BASE     0x50098000 // LRFD_S2RRAM_NS
#define AES_BASE                0x400C0000 // AES
#define AES_NS_BASE             0x500C0000 // AES_NS
#define I2S_BASE                0x400C1000 // I2S
#define I2S_NS_BASE             0x500C1000 // I2S_NS
#define DMA_BASE                0x400C4000 // DMA
#define DMA_NS_BASE             0x500C4000 // DMA_NS
#define SRAMCTRL_BASE           0x400C5000 // SRAMCTRL
#define CANFD_BASE              0x400D0000 // CANFD
#define CANFD_NS_BASE           0x500D0000 // CANFD_NS
#define CANFD_SRAM_BASE         0x400D1000 // CANFD_SRAM
   #define CANFD_SRAM_SIZE                 0x1000
#define CANFD_SRAM_NS_BASE      0x500D1000 // CANFD_SRAM_NS
#define VCE_BASE                0x400D2000 // VCE
#define VCE_NS_BASE             0x500D2000 // VCE_NS
#define VCE_TOPSMRAM_BASE       0x400D3000 // VCE_TOPSMRAM
   #define VCE_TOPSMRAM_SIZE               0x1000
#define VCE_TOPSMRAM_NS_BASE    0x500D3000 // VCE_TOPSMRAM_NS
#define VCERAM_DATA0_BASE       0x400E0000 // VCERAM_DATA0
   #define VCERAM_DATA0_SIZE               0x2000
#define VCERAM_DATA0_NS_BASE    0x500E0000 // VCERAM_DATA0_NS
#define VCERAM_DATA1_BASE       0x400E2000 // VCERAM_DATA1
   #define VCERAM_DATA1_SIZE               0x2000
#define VCERAM_DATA1_NS_BASE    0x500E2000 // VCERAM_DATA1_NS
#define HSMCRYPTO_BASE          0x400F0000 // HSMCRYPTO
#define HSMCRYPTO_NS_BASE       0x500F0000 // HSMCRYPTO_NS
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
#define DCB_BASE                0xE000EDE0 // DCB
#define FPE_BASE                0xE000EF30 // FPE
#define DIB_BASE                0xE000EFB0 // DIB
#define TPIU_BASE               0xE0040000 // TPIU
#define CPU_ROM_TABLE_BASE      0xE00FF000 // CPU_ROM_TABLE

#ifdef DRIVERLIB_NS
//*****************************************************************************
//
// Redefine the base addresses for IPs configured as non-secure by the
// TrustZone Control Module (TCM).
//
//*****************************************************************************
#undef PMCTL_BASE
#define PMCTL_BASE PMCTL_NS_BASE

#undef CKMD_BASE
#define CKMD_BASE CKMD_NS_BASE

#undef RTC_BASE
#define RTC_BASE RTC_NS_BASE

#undef IOC_BASE
#define IOC_BASE IOC_NS_BASE

#undef SYS0_BASE
#define SYS0_BASE SYS0_NS_BASE

#undef EVTULL_BASE
#define EVTULL_BASE EVTULL_NS_BASE

#undef PMUD_BASE
#define PMUD_BASE PMUD_NS_BASE

#undef DBGSS_BASE
#define DBGSS_BASE DBGSS_NS_BASE

#undef CLKCTL_BASE
#define CLKCTL_BASE CLKCTL_NS_BASE

#undef SYSTIM_BASE
#define SYSTIM_BASE SYSTIM_NS_BASE

#undef GPIO_BASE
#define GPIO_BASE GPIO_NS_BASE

#undef VIMS_BASE
#define VIMS_BASE VIMS_NS_BASE

#undef EVTSVT_BASE
#define EVTSVT_BASE EVTSVT_NS_BASE

#undef SPI0_BASE
#define SPI0_BASE SPI0_NS_BASE

#undef SPI1_BASE
#define SPI1_BASE SPI1_NS_BASE

#undef UART0_BASE
#define UART0_BASE UART0_NS_BASE

#undef UART1_BASE
#define UART1_BASE UART1_NS_BASE

#undef I2C0_BASE
#define I2C0_BASE I2C0_NS_BASE

#undef ADC_BASE
#define ADC_BASE ADC_NS_BASE

#undef HSM_BASE
#define HSM_BASE HSM_NS_BASE

#undef LGPT0_BASE
#define LGPT0_BASE LGPT0_NS_BASE

#undef LGPT1_BASE
#define LGPT1_BASE LGPT1_NS_BASE

#undef LGPT2_BASE
#define LGPT2_BASE LGPT2_NS_BASE

#undef LGPT3_BASE
#define LGPT3_BASE LGPT3_NS_BASE

#undef LRFDDBELL_BASE
#define LRFDDBELL_BASE LRFDDBELL_NS_BASE

#undef LRFDPBE_BASE
#define LRFDPBE_BASE LRFDPBE_NS_BASE

#undef LRFDPBE32_BASE
#define LRFDPBE32_BASE LRFDPBE32_NS_BASE

#undef LRFDTXF_BASE
#define LRFDTXF_BASE LRFDTXF_NS_BASE

#undef LRFDRXF_BASE
#define LRFDRXF_BASE LRFDRXF_NS_BASE

#undef LRFDMDM_BASE
#define LRFDMDM_BASE LRFDMDM_NS_BASE

#undef LRFDMDM32_BASE
#define LRFDMDM32_BASE LRFDMDM32_NS_BASE

#undef LRFDRFE_BASE
#define LRFDRFE_BASE LRFDRFE_NS_BASE

#undef LRFDRFE32_BASE
#define LRFDRFE32_BASE LRFDRFE32_NS_BASE

#undef LRFDTRC_BASE
#define LRFDTRC_BASE LRFDTRC_NS_BASE

#undef LRFDS2R_BASE
#define LRFDS2R_BASE LRFDS2R_NS_BASE

#undef LRFD_PBERAM_BASE
#define LRFD_PBERAM_BASE LRFD_PBERAM_NS_BASE

#undef LRFD_BUFRAM_BASE
#define LRFD_BUFRAM_BASE LRFD_BUFRAM_NS_BASE

#undef LRFD_MCERAM_BASE
#define LRFD_MCERAM_BASE LRFD_MCERAM_NS_BASE

#undef LRFD_RFERAM_BASE
#define LRFD_RFERAM_BASE LRFD_RFERAM_NS_BASE

#undef LRFD_S2RRAM_BASE
#define LRFD_S2RRAM_BASE LRFD_S2RRAM_NS_BASE

#undef AES_BASE
#define AES_BASE AES_NS_BASE

#undef I2S_BASE
#define I2S_BASE I2S_NS_BASE

#undef DMA_BASE
#define DMA_BASE DMA_NS_BASE

#undef CANFD_BASE
#define CANFD_BASE CANFD_NS_BASE

#undef CANFD_SRAM_BASE
#define CANFD_SRAM_BASE CANFD_SRAM_NS_BASE

#undef VCE_BASE
#define VCE_BASE VCE_NS_BASE

#undef VCE_TOPSMRAM_BASE
#define VCE_TOPSMRAM_BASE VCE_TOPSMRAM_NS_BASE

#undef VCERAM_DATA0_BASE
#define VCERAM_DATA0_BASE VCERAM_DATA0_NS_BASE

#undef VCERAM_DATA1_BASE
#define VCERAM_DATA1_BASE VCERAM_DATA1_NS_BASE

#undef HSMCRYPTO_BASE
#define HSMCRYPTO_BASE HSMCRYPTO_NS_BASE

//*****************************************************************************
//
// Undefine the base addresses for the secure peripherals that cannot be
// accessed from non-secure side.
//
//*****************************************************************************
// TODO: Revise this list (LPRFXXWARE-975)
#undef FLASH_BASE
#undef CCFG_BASE
#undef CPU_ROM_TABLE_BASE
#undef DCB_BASE
#undef DIB_BASE
#undef FCFG_BASE
#undef FPE_BASE
#undef ICB_BASE
#undef OTP_BASE
#undef OTP_BKP_BASE
#undef ROM_BASE
#undef SCFG_BASE
#undef SRAMCTRL_BASE
#undef TPIU_BASE
#undef VLOG_BASE

#endif // DRIVERLIB_NS

#endif // __HW_MEMMAP__
