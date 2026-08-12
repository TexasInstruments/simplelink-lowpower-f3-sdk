/******************************************************************************
*  Filename:       hw_memmap_h
******************************************************************************
*  Copyright (c) 2023-2025 Texas Instruments Incorporated. All rights reserved.
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
#define FLASH_MAIN_BASE         0x00000000U // FLASH_MAIN
#define FLASH_MAIN_S_BASE       0x00000000U // FLASH_MAIN_S
#define FLASH_MAIN_NS_BASE      0x10000000U // FLASH_MAIN_NS
   #define FLASH_MAIN_SIZE                 0x100000U
#define ROM_BASE                0x0F000000U // ROM
   #define ROM_SIZE                        0x8000U
#define SRAM_BASE               0x20000000U // SRAM
#define SRAM_S_BASE             0x20000000U // SRAM_S
#define SRAM_NS_BASE            0x30000000U // SRAM_NS
   #define SRAM_SIZE                       0x28800U
#define PMCTL_BASE              0x40000000U // PMCTL
#define PMCTL_S_BASE            0x40000000U // PMCTL_S
#define PMCTL_NS_BASE           0x50000000U // PMCTL_NS
#define CKMD_BASE               0x40001000U // CKMD
#define CKMD_S_BASE             0x40001000U // CKMD_S
#define CKMD_NS_BASE            0x50001000U // CKMD_NS
#define RTC_BASE                0x40002000U // RTC
#define RTC_S_BASE              0x40002000U // RTC_S
#define RTC_NS_BASE             0x50002000U // RTC_NS
#define IOC_BASE                0x40003000U // IOC
#define IOC_S_BASE              0x40003000U // IOC_S
#define IOC_NS_BASE             0x50003000U // IOC_NS
#define SYS0_BASE               0x40004000U // SYS0
#define SYS0_S_BASE             0x40004000U // SYS0_S
#define SYS0_NS_BASE            0x50004000U // SYS0_NS
#define EVTULL_BASE             0x40005000U // EVTULL
#define EVTULL_S_BASE           0x40005000U // EVTULL_S
#define EVTULL_NS_BASE          0x50005000U // EVTULL_NS
#define PMUD_BASE               0x40006000U // PMUD
#define PMUD_S_BASE             0x40006000U // PMUD_S
#define PMUD_NS_BASE            0x50006000U // PMUD_NS
#define TCM_BASE                0x40007000U // TCM
#define DBGSS_BASE              0x4000F000U // DBGSS
#define DBGSS_S_BASE            0x4000F000U // DBGSS_S
#define DBGSS_NS_BASE           0x5000F000U // DBGSS_NS
#define CLKCTL_BASE             0x40020000U // CLKCTL
#define CLKCTL_S_BASE           0x40020000U // CLKCTL_S
#define CLKCTL_NS_BASE          0x50020000U // CLKCTL_NS
#define FLASH_BASE              0x40021000U // FLASH
#define SYSTIM_BASE             0x40022000U // SYSTIM
#define SYSTIM_S_BASE           0x40022000U // SYSTIM_S
#define SYSTIM_NS_BASE          0x50022000U // SYSTIM_NS
#define GPIO_BASE               0x40023000U // GPIO
#define GPIO_S_BASE             0x40023000U // GPIO_S
#define GPIO_NS_BASE            0x50023000U // GPIO_NS
#define VIMS_BASE               0x40024000U // VIMS
#define VIMS_S_BASE             0x40024000U // VIMS_S
#define VIMS_NS_BASE            0x50024000U // VIMS_NS
#define EVTSVT_BASE             0x40025000U // EVTSVT
#define EVTSVT_S_BASE           0x40025000U // EVTSVT_S
#define EVTSVT_NS_BASE          0x50025000U // EVTSVT_NS
#define SPI0_BASE               0x40030000U // SPI
#define SPI0_S_BASE             0x40030000U // SPI_S
#define SPI0_NS_BASE            0x50030000U // SPI_NS
#define SPI1_BASE               0x40031000U // SPI
#define SPI1_S_BASE             0x40031000U // SPI_S
#define SPI1_NS_BASE            0x50031000U // SPI_NS
#define UART0_BASE              0x40034000U // UART
#define UART0_S_BASE            0x40034000U // UART_S
#define UART0_NS_BASE           0x50034000U // UART_NS
#define UART1_BASE              0x40035000U // UART
#define UART1_S_BASE            0x40035000U // UART_S
#define UART1_NS_BASE           0x50035000U // UART_NS
#define I2C0_BASE               0x40038000U // I2C
#define I2C0_S_BASE             0x40038000U // I2C_S
#define I2C0_NS_BASE            0x50038000U // I2C_NS
#define ADC_BASE                0x40050000U // ADC
#define ADC_S_BASE              0x40050000U // ADC_S
#define ADC_NS_BASE             0x50050000U // ADC_NS
#define HSM_BASE                0x40053000U // HSM
#define HSM_S_BASE              0x40053000U // HSM_S
#define HSM_NS_BASE             0x50053000U // HSM_NS
#define LGPT0_BASE              0x40060000U // LGPT0
#define LGPT0_S_BASE            0x40060000U // LGPT0_S
#define LGPT0_NS_BASE           0x50060000U // LGPT0_NS
#define LGPT1_BASE              0x40061000U // LGPT1
#define LGPT1_S_BASE            0x40061000U // LGPT1_S
#define LGPT1_NS_BASE           0x50061000U // LGPT1_NS
#define LGPT2_BASE              0x40062000U // LGPT2
#define LGPT2_S_BASE            0x40062000U // LGPT2_S
#define LGPT2_NS_BASE           0x50062000U // LGPT2_NS
#define LGPT3_BASE              0x40063000U // LGPT3
#define LGPT3_S_BASE            0x40063000U // LGPT3_S
#define LGPT3_NS_BASE           0x50063000U // LGPT3_NS
#define LRFDDBELL_BASE          0x40080000U // LRFDDBELL
#define LRFDDBELL_S_BASE        0x40080000U // LRFDDBELL_S
#define LRFDDBELL_NS_BASE       0x50080000U // LRFDDBELL_NS
#define LRFDPBE_BASE            0x40081000U // LRFDPBE
#define LRFDPBE_S_BASE          0x40081000U // LRFDPBE_S
#define LRFDPBE_NS_BASE         0x50081000U // LRFDPBE_NS
#define LRFDPBE32_BASE          0x40081400U // LRFDPBE32
#define LRFDPBE32_S_BASE        0x40081400U // LRFDPBE32_S
#define LRFDPBE32_NS_BASE       0x50081400U // LRFDPBE32_NS
#define LRFDTXF_BASE            0x40081800U // LRFDTXF
#define LRFDTXF_S_BASE          0x40081800U // LRFDTXF_S
#define LRFDTXF_NS_BASE         0x50081800U // LRFDTXF_NS
#define LRFDRXF_BASE            0x40081C00U // LRFDRXF
#define LRFDRXF_S_BASE          0x40081C00U // LRFDRXF_S
#define LRFDRXF_NS_BASE         0x50081C00U // LRFDRXF_NS
#define LRFDMDM_BASE            0x40082000U // LRFDMDM
#define LRFDMDM_S_BASE          0x40082000U // LRFDMDM_S
#define LRFDMDM_NS_BASE         0x50082000U // LRFDMDM_NS
#define LRFDMDM32_BASE          0x40082400U // LRFDMDM32
#define LRFDMDM32_S_BASE        0x40082400U // LRFDMDM32_S
#define LRFDMDM32_NS_BASE       0x50082400U // LRFDMDM32_NS
#define LRFDRFE_BASE            0x40083000U // LRFDRFE
#define LRFDRFE_S_BASE          0x40083000U // LRFDRFE_S
#define LRFDRFE_NS_BASE         0x50083000U // LRFDRFE_NS
#define LRFDRFE32_BASE          0x40083400U // LRFDRFE32
#define LRFDRFE32_S_BASE        0x40083400U // LRFDRFE32_S
#define LRFDRFE32_NS_BASE       0x50083400U // LRFDRFE32_NS
#define LRFDTRC_BASE            0x40084000U // LRFDTRC
#define LRFDTRC_S_BASE          0x40084000U // LRFDTRC_S
#define LRFDTRC_NS_BASE         0x50084000U // LRFDTRC_NS
#define LRFDS2R_BASE            0x40085000U // LRFDS2R
#define LRFDS2R_S_BASE          0x40085000U // LRFDS2R_S
#define LRFDS2R_NS_BASE         0x50085000U // LRFDS2R_NS
#define LRFD_PBERAM_BASE        0x40090000U // LRFD_PBERAM
#define LRFD_PBERAM_S_BASE      0x40090000U // LRFD_PBERAM_S
#define LRFD_PBERAM_NS_BASE     0x50090000U // LRFD_PBERAM_NS
   #define LRFD_PBERAM_SIZE                0x1000U
#define LRFD_BUFRAM_BASE        0x40092000U // LRFD_BUFRAM
#define LRFD_BUFRAM_S_BASE      0x40092000U // LRFD_BUFRAM_S
#define LRFD_BUFRAM_NS_BASE     0x50092000U // LRFD_BUFRAM_NS
   #define LRFD_BUFRAM_SIZE                0x600U
#define LRFD_MCERAM_BASE        0x40094000U // LRFD_MCERAM
#define LRFD_MCERAM_S_BASE      0x40094000U // LRFD_MCERAM_S
#define LRFD_MCERAM_NS_BASE     0x50094000U // LRFD_MCERAM_NS
   #define LRFD_MCERAM_SIZE                0x800U
#define LRFD_RFERAM_BASE        0x40096000U // LRFD_RFERAM
#define LRFD_RFERAM_S_BASE      0x40096000U // LRFD_RFERAM_S
#define LRFD_RFERAM_NS_BASE     0x50096000U // LRFD_RFERAM_NS
   #define LRFD_RFERAM_SIZE                0x1000U
#define LRFD_S2RRAM_BASE        0x40098000U // LRFD_S2RRAM
#define LRFD_S2RRAM_S_BASE      0x40098000U // LRFD_S2RRAM_S
#define LRFD_S2RRAM_NS_BASE     0x50098000U // LRFD_S2RRAM_NS
   #define LRFD_S2RRAM_SIZE                0x1000U
#define AES_BASE                0x400C0000U // AES
#define AES_S_BASE              0x400C0000U // AES_S
#define AES_NS_BASE             0x500C0000U // AES_NS
#define I2S_BASE                0x400C1000U // I2S
#define I2S_S_BASE              0x400C1000U // I2S_S
#define I2S_NS_BASE             0x500C1000U // I2S_NS
#define DMA_BASE                0x400C4000U // DMA
#define DMA_S_BASE              0x400C4000U // DMA_S
#define DMA_NS_BASE             0x500C4000U // DMA_NS
#define SRAMCTRL_BASE           0x400C5000U // SRAMCTRL
#define CANFD_BASE              0x400D0000U // CANFD
#define CANFD_S_BASE            0x400D0000U // CANFD_S
#define CANFD_NS_BASE           0x500D0000U // CANFD_NS
#define CANFD_SRAM_BASE         0x400D1000U // CANFD_SRAM
#define CANFD_SRAM_S_BASE       0x400D1000U // CANFD_SRAM_S
#define CANFD_SRAM_NS_BASE      0x500D1000U // CANFD_SRAM_NS
   #define CANFD_SRAM_SIZE                 0x1000U
#define APU_BASE                0x400D2000U // APU
#define APU_S_BASE              0x400D2000U // APU_S
#define APU_NS_BASE             0x500D2000U // APU_NS
#define APU_TOPSMRAM_BASE       0x400D3000U // APU_TOPSMRAM
#define APU_TOPSMRAM_S_BASE     0x400D3000U // APU_TOPSMRAM_S
#define APU_TOPSMRAM_NS_BASE    0x500D3000U // APU_TOPSMRAM_NS
   #define APU_TOPSMRAM_SIZE               0x1000U
#define APURAM_DATA0_BASE       0x400E0000U // APURAM_DATA
#define APURAM_DATA0_S_BASE     0x400E0000U // APURAM_DATA_S
#define APURAM_DATA0_NS_BASE    0x500E0000U // APURAM_DATA_NS
   #define APURAM_DATA0_SIZE               0x2000U
#define APURAM_DATA1_BASE       0x400E2000U // APURAM_DATA
#define APURAM_DATA1_S_BASE     0x400E2000U // APURAM_DATA_S
#define APURAM_DATA1_NS_BASE    0x500E2000U // APURAM_DATA_NS
   #define APURAM_DATA1_SIZE               0x2000U
#define HSMCRYPTO_BASE          0x400F0000U // HSMCRYPTO
#define HSMCRYPTO_S_BASE        0x400F0000U // HSMCRYPTO_S
#define HSMCRYPTO_NS_BASE       0x500F0000U // HSMCRYPTO_NS
#define FCFG_BASE               0x4E000000U // FCFG
   #define FCFG_SIZE                       0x400U
#define CCFG_BASE               0x4E020000U // CCFG
   #define CCFG_SIZE                       0x800U
#define OTP_BASE                0x4E020800U // OTP
   #define OTP_SIZE                        0x800U
#define SCFG_BASE               0x4E040000U // SCFG
   #define SCFG_SIZE                       0x400U
#define VLOG_BASE               0x4E060000U // VLOG
   #define VLOG_SIZE                       0x800U
#define OTP_BKP_BASE            0x4E060800U // OTP_BKP
   #define OTP_BKP_SIZE                    0x800U
#define FPB_BASE                0xE0002000U // FPB
#define ICB_BASE                0xE000E000U // ICB
#define SYSTICK_BASE            0xE000E010U // SYSTICK
#define NVIC_BASE               0xE000E100U // NVIC
#define SCB_BASE                0xE000ECFCU // SCB
#define MPU_BASE                0xE000ED90U // MPU
#define SAU_BASE                0xE000EDD0U // SAU
#define DCB_BASE                0xE000EDE0U // DCB
#define FPE_BASE                0xE000EF30U // FPE
#define DIB_BASE                0xE000EFB0U // DIB
#define TPIU_BASE               0xE0040000U // TPIU
#define CPU_ROM_TABLE_BASE      0xE00FF000U // CPU_ROM_TABLE

#ifdef DRIVERLIB_NS
//*****************************************************************************
//
// Redefine the base addresses for IPs configured as non-secure by the
// TrustZone Control Module (TCM).
//
//*****************************************************************************
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

#undef APU_BASE
#define APU_BASE APU_NS_BASE

#undef APU_TOPSMRAM_BASE
#define APU_TOPSMRAM_BASE APU_TOPSMRAM_NS_BASE

#undef APURAM_DATA0_BASE
#define APURAM_DATA0_BASE APURAM_DATA0_NS_BASE

#undef APURAM_DATA1_BASE
#define APURAM_DATA1_BASE APURAM_DATA1_NS_BASE

#undef HSMCRYPTO_BASE
#define HSMCRYPTO_BASE HSMCRYPTO_NS_BASE

//*****************************************************************************
//
// Undefine the base addresses for the secure peripherals that cannot be
// accessed from non-secure side.
//
//*****************************************************************************
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

// These periepherals cannot be accesed from non-secure side due to the gasket
// configuration in TFM.
#undef CKMD_BASE
#undef EVTSVT_BASE
#undef PMCTL_BASE

#endif // DRIVERLIB_NS

#endif // __HW_MEMMAP__
