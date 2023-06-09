// ===========================================================================
// This file is autogenerated, please DO NOT modify!
//
// Generated on  2023-04-18 13:27:21
// by user:      developer
// on machine:   swtools
// CWD:          /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tc/octopus/regs/inc
// Commandline:  /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tools/topsm/topsmregs.pl -target c -base 0x40096000 -dat_sz 16 -ramreg -i /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/rfe/common/doc/ram_registers.txt
// C&P friendly: /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tools/topsm/topsmregs.pl -target c -base 0x40096000 -dat_sz 16 -ramreg -i /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/rfe/common/doc/ram_registers.txt
//
// Relevant file version(s):
//
// /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tools/topsm/topsmregs.pl
//   rcs-info: (file not managed or unknown revision control system)
//   git-hash: 0d11a0ea4ba55ba3ef648be18aeec231d5314753
//
// /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/rfe/common/doc/ram_registers.txt
//   rcs-info: (file not managed or unknown revision control system)
//   git-hash: bb0745aae8a89ae740919ffe9b80ecbdea39098c
//
// ===========================================================================


#ifndef __RFE_COMMON_RAM_REGS_H
#define __RFE_COMMON_RAM_REGS_H

#define RFE_COMMON_RAM_BASE 0x40096000UL
// --------------------------------------------------------------
// SYNTHCTL
// 
#define RFE_COMMON_RAM_SYNTHCTL_ADR (RFE_COMMON_RAM_BASE + 0x0804UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_SYNTHCTL = (unsigned short*) RFE_COMMON_RAM_SYNTHCTL_ADR;
#define S_RFE_COMMON_RAM_SYNTHCTL (*SP_RFE_COMMON_RAM_SYNTHCTL)
// bitfield: SYNTHCTL_CHRGFILT
#define RFE_COMMON_RAM_SYNTHCTL_CHRGFILT                        7UL
#define RFE_COMMON_RAM_SYNTHCTL_CHRGFILT_BM                     0x0080UL
// bitfield: SYNTHCTL_VREFBP
#define RFE_COMMON_RAM_SYNTHCTL_VREFBP                          6UL
#define RFE_COMMON_RAM_SYNTHCTL_VREFBP_BM                       0x0040UL
// bitfield: SYNTHCTL_TXWAITMOD
#define RFE_COMMON_RAM_SYNTHCTL_TXWAITMOD                       5UL
#define RFE_COMMON_RAM_SYNTHCTL_TXWAITMOD_BM                    0x0020UL
// bitfield: SYNTHCTL_PHEDISC
#define RFE_COMMON_RAM_SYNTHCTL_PHEDISC                         4UL
#define RFE_COMMON_RAM_SYNTHCTL_PHEDISC_BM                      0x0010UL
// enums for bitfield SYNTHCTL_PHEDISC (width: 1)UL
#define RFE_COMMON_RAM_SYNTHCTL_PHEDISC_DIS                     0x0UL
#define RFE_COMMON_RAM_SYNTHCTL_PHEDISC_EN                      0x1UL
// bitfield: SYNTHCTL_RTRIMTST
#define RFE_COMMON_RAM_SYNTHCTL_RTRIMTST                        3UL
#define RFE_COMMON_RAM_SYNTHCTL_RTRIMTST_BM                     0x0008UL
// enums for bitfield SYNTHCTL_RTRIMTST (width: 1)UL
#define RFE_COMMON_RAM_SYNTHCTL_RTRIMTST_DIS                    0x0UL
#define RFE_COMMON_RAM_SYNTHCTL_RTRIMTST_EN                     0x1UL
// bitfield: SYNTHCTL_IIR
#define RFE_COMMON_RAM_SYNTHCTL_IIR                             2UL
#define RFE_COMMON_RAM_SYNTHCTL_IIR_BM                          0x0004UL
// enums for bitfield SYNTHCTL_IIR (width: 1)UL
#define RFE_COMMON_RAM_SYNTHCTL_IIR_DIS                         0x0UL
#define RFE_COMMON_RAM_SYNTHCTL_IIR_EN                          0x1UL
// bitfield: SYNTHCTL_REFDTHR
#define RFE_COMMON_RAM_SYNTHCTL_REFDTHR                         1UL
#define RFE_COMMON_RAM_SYNTHCTL_REFDTHR_BM                      0x0002UL
// enums for bitfield SYNTHCTL_REFDTHR (width: 1)UL
#define RFE_COMMON_RAM_SYNTHCTL_REFDTHR_DIS                     0x0UL
#define RFE_COMMON_RAM_SYNTHCTL_REFDTHR_EN                      0x1UL
// bitfield: SYNTHCTL_FCDEM
#define RFE_COMMON_RAM_SYNTHCTL_FCDEM                           0UL
#define RFE_COMMON_RAM_SYNTHCTL_FCDEM_BM                        0x0001UL
// enums for bitfield SYNTHCTL_FCDEM (width: 1)UL
#define RFE_COMMON_RAM_SYNTHCTL_FCDEM_DIS                       0x0UL
#define RFE_COMMON_RAM_SYNTHCTL_FCDEM_EN                        0x1UL
// --------------------------------------------------------------
// TDCCAL0
// 
#define RFE_COMMON_RAM_TDCCAL0_ADR (RFE_COMMON_RAM_BASE + 0x0806UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_TDCCAL0 = (unsigned short*) RFE_COMMON_RAM_TDCCAL0_ADR;
#define S_RFE_COMMON_RAM_TDCCAL0 (*SP_RFE_COMMON_RAM_TDCCAL0)
// bitfield: TDCCAL0_STOP
#define RFE_COMMON_RAM_TDCCAL0_STOP                             8UL
#define RFE_COMMON_RAM_TDCCAL0_STOP_BM                          0x0700UL
// --------------------------------------------------------------
// TDCCAL1
// 
#define RFE_COMMON_RAM_TDCCAL1_ADR (RFE_COMMON_RAM_BASE + 0x0808UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_TDCCAL1 = (unsigned short*) RFE_COMMON_RAM_TDCCAL1_ADR;
#define S_RFE_COMMON_RAM_TDCCAL1 (*SP_RFE_COMMON_RAM_TDCCAL1)
// bitfield: TDCCAL1_SUB
#define RFE_COMMON_RAM_TDCCAL1_SUB                              0UL
#define RFE_COMMON_RAM_TDCCAL1_SUB_BM                           0x00FFUL
// --------------------------------------------------------------
// TDCCAL2
// 
#define RFE_COMMON_RAM_TDCCAL2_ADR (RFE_COMMON_RAM_BASE + 0x080AUL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_TDCCAL2 = (unsigned short*) RFE_COMMON_RAM_TDCCAL2_ADR;
#define S_RFE_COMMON_RAM_TDCCAL2 (*SP_RFE_COMMON_RAM_TDCCAL2)
// bitfield: TDCCAL2_AVG
#define RFE_COMMON_RAM_TDCCAL2_AVG                              0UL
#define RFE_COMMON_RAM_TDCCAL2_AVG_BM                           0x003FUL
// --------------------------------------------------------------
// TDCPLL
// 
#define RFE_COMMON_RAM_TDCPLL_ADR (RFE_COMMON_RAM_BASE + 0x080CUL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_TDCPLL = (unsigned short*) RFE_COMMON_RAM_TDCPLL_ADR;
#define S_RFE_COMMON_RAM_TDCPLL (*SP_RFE_COMMON_RAM_TDCPLL)
// bitfield: TDCPLL_STOP
#define RFE_COMMON_RAM_TDCPLL_STOP                              8UL
#define RFE_COMMON_RAM_TDCPLL_STOP_BM                           0x0700UL
// --------------------------------------------------------------
// K1LSB
// 
#define RFE_COMMON_RAM_K1LSB_ADR (RFE_COMMON_RAM_BASE + 0x080EUL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_K1LSB = (unsigned short*) RFE_COMMON_RAM_K1LSB_ADR;
#define S_RFE_COMMON_RAM_K1LSB (*SP_RFE_COMMON_RAM_K1LSB)
// bitfield: K1LSB_VAL
#define RFE_COMMON_RAM_K1LSB_VAL                                0UL
#define RFE_COMMON_RAM_K1LSB_VAL_BM                             0xFFFFUL
// --------------------------------------------------------------
// K1MSB
// 
#define RFE_COMMON_RAM_K1MSB_ADR (RFE_COMMON_RAM_BASE + 0x0810UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_K1MSB = (unsigned short*) RFE_COMMON_RAM_K1MSB_ADR;
#define S_RFE_COMMON_RAM_K1MSB (*SP_RFE_COMMON_RAM_K1MSB)
// bitfield: K1MSB_VAL
#define RFE_COMMON_RAM_K1MSB_VAL                                0UL
#define RFE_COMMON_RAM_K1MSB_VAL_BM                             0xFFFFUL
// --------------------------------------------------------------
// K2BL
// 
#define RFE_COMMON_RAM_K2BL_ADR (RFE_COMMON_RAM_BASE + 0x0812UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_K2BL = (unsigned short*) RFE_COMMON_RAM_K2BL_ADR;
#define S_RFE_COMMON_RAM_K2BL (*SP_RFE_COMMON_RAM_K2BL)
// bitfield: K2BL_HPM
#define RFE_COMMON_RAM_K2BL_HPM                                 15UL
#define RFE_COMMON_RAM_K2BL_HPM_BM                              0x8000UL
// bitfield: K2BL_VAL
#define RFE_COMMON_RAM_K2BL_VAL                                 0UL
#define RFE_COMMON_RAM_K2BL_VAL_BM                              0x7FFFUL
// --------------------------------------------------------------
// K2AL
// 
#define RFE_COMMON_RAM_K2AL_ADR (RFE_COMMON_RAM_BASE + 0x0814UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_K2AL = (unsigned short*) RFE_COMMON_RAM_K2AL_ADR;
#define S_RFE_COMMON_RAM_K2AL (*SP_RFE_COMMON_RAM_K2AL)
// bitfield: K2AL_HPM
#define RFE_COMMON_RAM_K2AL_HPM                                 15UL
#define RFE_COMMON_RAM_K2AL_HPM_BM                              0x8000UL
// bitfield: K2AL_VAL
#define RFE_COMMON_RAM_K2AL_VAL                                 0UL
#define RFE_COMMON_RAM_K2AL_VAL_BM                              0x7FFFUL
// --------------------------------------------------------------
// K3BL
// 
#define RFE_COMMON_RAM_K3BL_ADR (RFE_COMMON_RAM_BASE + 0x0816UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_K3BL = (unsigned short*) RFE_COMMON_RAM_K3BL_ADR;
#define S_RFE_COMMON_RAM_K3BL (*SP_RFE_COMMON_RAM_K3BL)
// bitfield: K3BL_VAL
#define RFE_COMMON_RAM_K3BL_VAL                                 0UL
#define RFE_COMMON_RAM_K3BL_VAL_BM                              0xFFFFUL
// --------------------------------------------------------------
// K3AL
// 
#define RFE_COMMON_RAM_K3AL_ADR (RFE_COMMON_RAM_BASE + 0x0818UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_K3AL = (unsigned short*) RFE_COMMON_RAM_K3AL_ADR;
#define S_RFE_COMMON_RAM_K3AL (*SP_RFE_COMMON_RAM_K3AL)
// bitfield: K3AL_VAL
#define RFE_COMMON_RAM_K3AL_VAL                                 0UL
#define RFE_COMMON_RAM_K3AL_VAL_BM                              0xFFFFUL
// --------------------------------------------------------------
// K5
// 
#define RFE_COMMON_RAM_K5_ADR (RFE_COMMON_RAM_BASE + 0x081AUL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_K5 = (unsigned short*) RFE_COMMON_RAM_K5_ADR;
#define S_RFE_COMMON_RAM_K5 (*SP_RFE_COMMON_RAM_K5)
// bitfield: K5_VAL
#define RFE_COMMON_RAM_K5_VAL                                   0UL
#define RFE_COMMON_RAM_K5_VAL_BM                                0xFFFFUL
// --------------------------------------------------------------
// RXIF
// 
#define RFE_COMMON_RAM_RXIF_ADR (RFE_COMMON_RAM_BASE + 0x081CUL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_RXIF = (unsigned short*) RFE_COMMON_RAM_RXIF_ADR;
#define S_RFE_COMMON_RAM_RXIF (*SP_RFE_COMMON_RAM_RXIF)
// bitfield: RXIF_FOFF
#define RFE_COMMON_RAM_RXIF_FOFF                                0UL
#define RFE_COMMON_RAM_RXIF_FOFF_BM                             0x0FFFUL
// --------------------------------------------------------------
// TXIF
// 
#define RFE_COMMON_RAM_TXIF_ADR (RFE_COMMON_RAM_BASE + 0x081EUL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_TXIF = (unsigned short*) RFE_COMMON_RAM_TXIF_ADR;
#define S_RFE_COMMON_RAM_TXIF (*SP_RFE_COMMON_RAM_TXIF)
// bitfield: TXIF_FOFF
#define RFE_COMMON_RAM_TXIF_FOFF                                0UL
#define RFE_COMMON_RAM_TXIF_FOFF_BM                             0x0FFFUL
// --------------------------------------------------------------
// RTRIMOFF
// 
#define RFE_COMMON_RAM_RTRIMOFF_ADR (RFE_COMMON_RAM_BASE + 0x0820UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_RTRIMOFF = (unsigned short*) RFE_COMMON_RAM_RTRIMOFF_ADR;
#define S_RFE_COMMON_RAM_RTRIMOFF (*SP_RFE_COMMON_RAM_RTRIMOFF)
// bitfield: RTRIMOFF_VAL
#define RFE_COMMON_RAM_RTRIMOFF_VAL                             0UL
#define RFE_COMMON_RAM_RTRIMOFF_VAL_BM                          0x000FUL
// --------------------------------------------------------------
// RTRIMMIN
// 
#define RFE_COMMON_RAM_RTRIMMIN_ADR (RFE_COMMON_RAM_BASE + 0x0822UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_RTRIMMIN = (unsigned short*) RFE_COMMON_RAM_RTRIMMIN_ADR;
#define S_RFE_COMMON_RAM_RTRIMMIN (*SP_RFE_COMMON_RAM_RTRIMMIN)
// bitfield: RTRIMMIN_VAL
#define RFE_COMMON_RAM_RTRIMMIN_VAL                             0UL
#define RFE_COMMON_RAM_RTRIMMIN_VAL_BM                          0x000FUL
// --------------------------------------------------------------
// DIVI
// 
#define RFE_COMMON_RAM_DIVI_ADR (RFE_COMMON_RAM_BASE + 0x0824UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_DIVI = (unsigned short*) RFE_COMMON_RAM_DIVI_ADR;
#define S_RFE_COMMON_RAM_DIVI (*SP_RFE_COMMON_RAM_DIVI)
// bitfield: DIVI_PDET
#define RFE_COMMON_RAM_DIVI_PDET                                15UL
#define RFE_COMMON_RAM_DIVI_PDET_BM                             0x8000UL
// bitfield: DIVI_NMIREFTRIM
#define RFE_COMMON_RAM_DIVI_NMIREFTRIM                          12UL
#define RFE_COMMON_RAM_DIVI_NMIREFTRIM_BM                       0x7000UL
// bitfield: DIVI_PMIREFTRIM
#define RFE_COMMON_RAM_DIVI_PMIREFTRIM                          9UL
#define RFE_COMMON_RAM_DIVI_PMIREFTRIM_BM                       0x0E00UL
// bitfield: DIVI_TXBOOST
#define RFE_COMMON_RAM_DIVI_TXBOOST                             8UL
#define RFE_COMMON_RAM_DIVI_TXBOOST_BM                          0x0100UL
// enums for bitfield DIVI_TXBOOST (width: 1)UL
#define RFE_COMMON_RAM_DIVI_TXBOOST_DEFAULT                     0x0UL
#define RFE_COMMON_RAM_DIVI_TXBOOST_INCREASED                   0x1UL
// bitfield: DIVI_S1GFRC
#define RFE_COMMON_RAM_DIVI_S1GFRC                              7UL
#define RFE_COMMON_RAM_DIVI_S1GFRC_BM                           0x0080UL
// enums for bitfield DIVI_S1GFRC (width: 1)UL
#define RFE_COMMON_RAM_DIVI_S1GFRC_DIS                          0x0UL
#define RFE_COMMON_RAM_DIVI_S1GFRC_EN                           0x1UL
// bitfield: DIVI_BUFGAIN
#define RFE_COMMON_RAM_DIVI_BUFGAIN                             5UL
#define RFE_COMMON_RAM_DIVI_BUFGAIN_BM                          0x0060UL
// bitfield: DIVI_BIAS
#define RFE_COMMON_RAM_DIVI_BIAS                                4UL
#define RFE_COMMON_RAM_DIVI_BIAS_BM                             0x0010UL
// bitfield: DIVI_OUT
#define RFE_COMMON_RAM_DIVI_OUT                                 3UL
#define RFE_COMMON_RAM_DIVI_OUT_BM                              0x0008UL
// enums for bitfield DIVI_OUT (width: 1)UL
#define RFE_COMMON_RAM_DIVI_OUT_FE_S1G                          0x0UL
#define RFE_COMMON_RAM_DIVI_OUT_FE_2G4                          0x1UL
// bitfield: DIVI_RATIO
#define RFE_COMMON_RAM_DIVI_RATIO                               0UL
#define RFE_COMMON_RAM_DIVI_RATIO_BM                            0x0007UL
// --------------------------------------------------------------
// DIVF
// 
#define RFE_COMMON_RAM_DIVF_ADR (RFE_COMMON_RAM_BASE + 0x0826UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_DIVF = (unsigned short*) RFE_COMMON_RAM_DIVF_ADR;
#define S_RFE_COMMON_RAM_DIVF (*SP_RFE_COMMON_RAM_DIVF)
// bitfield: DIVF_PDET
#define RFE_COMMON_RAM_DIVF_PDET                                15UL
#define RFE_COMMON_RAM_DIVF_PDET_BM                             0x8000UL
// bitfield: DIVF_NMIREFTRIM
#define RFE_COMMON_RAM_DIVF_NMIREFTRIM                          12UL
#define RFE_COMMON_RAM_DIVF_NMIREFTRIM_BM                       0x7000UL
// bitfield: DIVF_PMIREFTRIM
#define RFE_COMMON_RAM_DIVF_PMIREFTRIM                          9UL
#define RFE_COMMON_RAM_DIVF_PMIREFTRIM_BM                       0x0E00UL
// bitfield: DIVF_TXBOOST
#define RFE_COMMON_RAM_DIVF_TXBOOST                             8UL
#define RFE_COMMON_RAM_DIVF_TXBOOST_BM                          0x0100UL
// enums for bitfield DIVF_TXBOOST (width: 1)UL
#define RFE_COMMON_RAM_DIVF_TXBOOST_DEFAULT                     0x0UL
#define RFE_COMMON_RAM_DIVF_TXBOOST_INCREASED                   0x1UL
// bitfield: DIVF_S1GFRC
#define RFE_COMMON_RAM_DIVF_S1GFRC                              7UL
#define RFE_COMMON_RAM_DIVF_S1GFRC_BM                           0x0080UL
// enums for bitfield DIVF_S1GFRC (width: 1)UL
#define RFE_COMMON_RAM_DIVF_S1GFRC_DIS                          0x0UL
#define RFE_COMMON_RAM_DIVF_S1GFRC_EN                           0x1UL
// bitfield: DIVF_BUFGAIN
#define RFE_COMMON_RAM_DIVF_BUFGAIN                             5UL
#define RFE_COMMON_RAM_DIVF_BUFGAIN_BM                          0x0060UL
// bitfield: DIVF_BIAS
#define RFE_COMMON_RAM_DIVF_BIAS                                4UL
#define RFE_COMMON_RAM_DIVF_BIAS_BM                             0x0010UL
// bitfield: DIVF_OUT
#define RFE_COMMON_RAM_DIVF_OUT                                 3UL
#define RFE_COMMON_RAM_DIVF_OUT_BM                              0x0008UL
// enums for bitfield DIVF_OUT (width: 1)UL
#define RFE_COMMON_RAM_DIVF_OUT_FE_S1G                          0x0UL
#define RFE_COMMON_RAM_DIVF_OUT_FE_2G4                          0x1UL
// bitfield: DIVF_RATIO
#define RFE_COMMON_RAM_DIVF_RATIO                               0UL
#define RFE_COMMON_RAM_DIVF_RATIO_BM                            0x0007UL
// --------------------------------------------------------------
// DIVLDOI
// 
#define RFE_COMMON_RAM_DIVLDOI_ADR (RFE_COMMON_RAM_BASE + 0x0828UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_DIVLDOI = (unsigned short*) RFE_COMMON_RAM_DIVLDOI_ADR;
#define S_RFE_COMMON_RAM_DIVLDOI (*SP_RFE_COMMON_RAM_DIVLDOI)
// bitfield: DIVLDOI_ITEST
#define RFE_COMMON_RAM_DIVLDOI_ITEST                            15UL
#define RFE_COMMON_RAM_DIVLDOI_ITEST_BM                         0x8000UL
// bitfield: DIVLDOI_VOUTTRIM
#define RFE_COMMON_RAM_DIVLDOI_VOUTTRIM                         8UL
#define RFE_COMMON_RAM_DIVLDOI_VOUTTRIM_BM                      0x7F00UL
// bitfield: DIVLDOI_ITST
#define RFE_COMMON_RAM_DIVLDOI_ITST                             7UL
#define RFE_COMMON_RAM_DIVLDOI_ITST_BM                          0x0080UL
// bitfield: DIVLDOI_TMUX
#define RFE_COMMON_RAM_DIVLDOI_TMUX                             4UL
#define RFE_COMMON_RAM_DIVLDOI_TMUX_BM                          0x0070UL
// enums for bitfield DIVLDOI_TMUX (width: 3)UL
#define RFE_COMMON_RAM_DIVLDOI_TMUX_OFF                         0x0UL
#define RFE_COMMON_RAM_DIVLDOI_TMUX_GND                         0x1UL
// bitfield: DIVLDOI_MODE
#define RFE_COMMON_RAM_DIVLDOI_MODE                             2UL
#define RFE_COMMON_RAM_DIVLDOI_MODE_BM                          0x0004UL
// enums for bitfield DIVLDOI_MODE (width: 1)UL
#define RFE_COMMON_RAM_DIVLDOI_MODE_NORM                        0x0UL
#define RFE_COMMON_RAM_DIVLDOI_MODE_FAST                        0x1UL
// bitfield: DIVLDOI_BYPASS
#define RFE_COMMON_RAM_DIVLDOI_BYPASS                           1UL
#define RFE_COMMON_RAM_DIVLDOI_BYPASS_BM                        0x0002UL
// enums for bitfield DIVLDOI_BYPASS (width: 1)UL
#define RFE_COMMON_RAM_DIVLDOI_BYPASS_DIS                       0x0UL
#define RFE_COMMON_RAM_DIVLDOI_BYPASS_EN                        0x1UL
// bitfield: DIVLDOI_CTL
#define RFE_COMMON_RAM_DIVLDOI_CTL                              0UL
#define RFE_COMMON_RAM_DIVLDOI_CTL_BM                           0x0001UL
// enums for bitfield DIVLDOI_CTL (width: 1)UL
#define RFE_COMMON_RAM_DIVLDOI_CTL_DIS                          0x0UL
#define RFE_COMMON_RAM_DIVLDOI_CTL_EN                           0x1UL
// --------------------------------------------------------------
// DIVLDOF
// 
#define RFE_COMMON_RAM_DIVLDOF_ADR (RFE_COMMON_RAM_BASE + 0x082AUL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_DIVLDOF = (unsigned short*) RFE_COMMON_RAM_DIVLDOF_ADR;
#define S_RFE_COMMON_RAM_DIVLDOF (*SP_RFE_COMMON_RAM_DIVLDOF)
// bitfield: DIVLDOF_ITEST
#define RFE_COMMON_RAM_DIVLDOF_ITEST                            15UL
#define RFE_COMMON_RAM_DIVLDOF_ITEST_BM                         0x8000UL
// bitfield: DIVLDOF_VOUTTRIM
#define RFE_COMMON_RAM_DIVLDOF_VOUTTRIM                         8UL
#define RFE_COMMON_RAM_DIVLDOF_VOUTTRIM_BM                      0x7F00UL
// bitfield: DIVLDOF_ITST
#define RFE_COMMON_RAM_DIVLDOF_ITST                             7UL
#define RFE_COMMON_RAM_DIVLDOF_ITST_BM                          0x0080UL
// bitfield: DIVLDOF_TMUX
#define RFE_COMMON_RAM_DIVLDOF_TMUX                             4UL
#define RFE_COMMON_RAM_DIVLDOF_TMUX_BM                          0x0070UL
// enums for bitfield DIVLDOF_TMUX (width: 3)UL
#define RFE_COMMON_RAM_DIVLDOF_TMUX_OFF                         0x0UL
#define RFE_COMMON_RAM_DIVLDOF_TMUX_GND                         0x1UL
// bitfield: DIVLDOF_MODE
#define RFE_COMMON_RAM_DIVLDOF_MODE                             2UL
#define RFE_COMMON_RAM_DIVLDOF_MODE_BM                          0x0004UL
// enums for bitfield DIVLDOF_MODE (width: 1)UL
#define RFE_COMMON_RAM_DIVLDOF_MODE_NORM                        0x0UL
#define RFE_COMMON_RAM_DIVLDOF_MODE_FAST                        0x1UL
// bitfield: DIVLDOF_BYPASS
#define RFE_COMMON_RAM_DIVLDOF_BYPASS                           1UL
#define RFE_COMMON_RAM_DIVLDOF_BYPASS_BM                        0x0002UL
// enums for bitfield DIVLDOF_BYPASS (width: 1)UL
#define RFE_COMMON_RAM_DIVLDOF_BYPASS_DIS                       0x0UL
#define RFE_COMMON_RAM_DIVLDOF_BYPASS_EN                        0x1UL
// bitfield: DIVLDOF_CTL
#define RFE_COMMON_RAM_DIVLDOF_CTL                              0UL
#define RFE_COMMON_RAM_DIVLDOF_CTL_BM                           0x0001UL
// enums for bitfield DIVLDOF_CTL (width: 1)UL
#define RFE_COMMON_RAM_DIVLDOF_CTL_DIS                          0x0UL
#define RFE_COMMON_RAM_DIVLDOF_CTL_EN                           0x1UL
// --------------------------------------------------------------
// DIVLDOIOFF
// 
#define RFE_COMMON_RAM_DIVLDOIOFF_ADR (RFE_COMMON_RAM_BASE + 0x082CUL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_DIVLDOIOFF = (unsigned short*) RFE_COMMON_RAM_DIVLDOIOFF_ADR;
#define S_RFE_COMMON_RAM_DIVLDOIOFF (*SP_RFE_COMMON_RAM_DIVLDOIOFF)
// bitfield: DIVLDOIOFF_VAL
#define RFE_COMMON_RAM_DIVLDOIOFF_VAL                           0UL
#define RFE_COMMON_RAM_DIVLDOIOFF_VAL_BM                        0x007FUL
// --------------------------------------------------------------
// LDOSETTLE
// 
#define RFE_COMMON_RAM_LDOSETTLE_ADR (RFE_COMMON_RAM_BASE + 0x082EUL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_LDOSETTLE = (unsigned short*) RFE_COMMON_RAM_LDOSETTLE_ADR;
#define S_RFE_COMMON_RAM_LDOSETTLE (*SP_RFE_COMMON_RAM_LDOSETTLE)
// bitfield: LDOSETTLE_VAL
#define RFE_COMMON_RAM_LDOSETTLE_VAL                            0UL
#define RFE_COMMON_RAM_LDOSETTLE_VAL_BM                         0x03FFUL
// --------------------------------------------------------------
// CHRGSETTLE
// 
#define RFE_COMMON_RAM_CHRGSETTLE_ADR (RFE_COMMON_RAM_BASE + 0x0830UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_CHRGSETTLE = (unsigned short*) RFE_COMMON_RAM_CHRGSETTLE_ADR;
#define S_RFE_COMMON_RAM_CHRGSETTLE (*SP_RFE_COMMON_RAM_CHRGSETTLE)
// bitfield: CHRGSETTLE_VAL
#define RFE_COMMON_RAM_CHRGSETTLE_VAL                           0UL
#define RFE_COMMON_RAM_CHRGSETTLE_VAL_BM                        0x03FFUL
// --------------------------------------------------------------
// DCOSETTLE
// 
#define RFE_COMMON_RAM_DCOSETTLE_ADR (RFE_COMMON_RAM_BASE + 0x0832UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_DCOSETTLE = (unsigned short*) RFE_COMMON_RAM_DCOSETTLE_ADR;
#define S_RFE_COMMON_RAM_DCOSETTLE (*SP_RFE_COMMON_RAM_DCOSETTLE)
// bitfield: DCOSETTLE_VAL
#define RFE_COMMON_RAM_DCOSETTLE_VAL                            0UL
#define RFE_COMMON_RAM_DCOSETTLE_VAL_BM                         0x03FFUL
// --------------------------------------------------------------
// IFAMPRFLDOTX
// 
#define RFE_COMMON_RAM_IFAMPRFLDOTX_ADR (RFE_COMMON_RAM_BASE + 0x0834UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_IFAMPRFLDOTX = (unsigned short*) RFE_COMMON_RAM_IFAMPRFLDOTX_ADR;
#define S_RFE_COMMON_RAM_IFAMPRFLDOTX (*SP_RFE_COMMON_RAM_IFAMPRFLDOTX)
// bitfield: IFAMPRFLDOTX_TRIM
#define RFE_COMMON_RAM_IFAMPRFLDOTX_TRIM                        9UL
#define RFE_COMMON_RAM_IFAMPRFLDOTX_TRIM_BM                     0xFE00UL
// --------------------------------------------------------------
// IFAMPRFLDODEFAULT
// 
#define RFE_COMMON_RAM_IFAMPRFLDODEFAULT_ADR (RFE_COMMON_RAM_BASE + 0x0836UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_IFAMPRFLDODEFAULT = (unsigned short*) RFE_COMMON_RAM_IFAMPRFLDODEFAULT_ADR;
#define S_RFE_COMMON_RAM_IFAMPRFLDODEFAULT (*SP_RFE_COMMON_RAM_IFAMPRFLDODEFAULT)
// bitfield: IFAMPRFLDODEFAULT_TRIM
#define RFE_COMMON_RAM_IFAMPRFLDODEFAULT_TRIM                   9UL
#define RFE_COMMON_RAM_IFAMPRFLDODEFAULT_TRIM_BM                0xFE00UL
// --------------------------------------------------------------
// LFKIBL
// 
#define RFE_COMMON_RAM_LFKIBL_ADR (RFE_COMMON_RAM_BASE + 0x0838UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_LFKIBL = (unsigned short*) RFE_COMMON_RAM_LFKIBL_ADR;
#define S_RFE_COMMON_RAM_LFKIBL (*SP_RFE_COMMON_RAM_LFKIBL)
// bitfield: LFKIBL_HPM
#define RFE_COMMON_RAM_LFKIBL_HPM                               12UL
#define RFE_COMMON_RAM_LFKIBL_HPM_BM                            0x1000UL
// bitfield: LFKIBL_KI
#define RFE_COMMON_RAM_LFKIBL_KI                                0UL
#define RFE_COMMON_RAM_LFKIBL_KI_BM                             0x0FFFUL
// --------------------------------------------------------------
// LFKPBL
// 
#define RFE_COMMON_RAM_LFKPBL_ADR (RFE_COMMON_RAM_BASE + 0x083AUL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_LFKPBL = (unsigned short*) RFE_COMMON_RAM_LFKPBL_ADR;
#define S_RFE_COMMON_RAM_LFKPBL (*SP_RFE_COMMON_RAM_LFKPBL)
// bitfield: LFKPBL_KP
#define RFE_COMMON_RAM_LFKPBL_KP                                0UL
#define RFE_COMMON_RAM_LFKPBL_KP_BM                             0x7FFFUL
// --------------------------------------------------------------
// IKT
// 
#define RFE_COMMON_RAM_IKT_ADR (RFE_COMMON_RAM_BASE + 0x083CUL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_IKT = (unsigned short*) RFE_COMMON_RAM_IKT_ADR;
#define S_RFE_COMMON_RAM_IKT (*SP_RFE_COMMON_RAM_IKT)
// bitfield: IKT_VAL
#define RFE_COMMON_RAM_IKT_VAL                                  0UL
#define RFE_COMMON_RAM_IKT_VAL_BM                               0xFFFFUL
// --------------------------------------------------------------
// PHYRSSIOFFSET
// 
#define RFE_COMMON_RAM_PHYRSSIOFFSET_ADR (RFE_COMMON_RAM_BASE + 0x083EUL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_PHYRSSIOFFSET = (unsigned short*) RFE_COMMON_RAM_PHYRSSIOFFSET_ADR;
#define S_RFE_COMMON_RAM_PHYRSSIOFFSET (*SP_RFE_COMMON_RAM_PHYRSSIOFFSET)
// bitfield: PHYRSSIOFFSET_VAL
#define RFE_COMMON_RAM_PHYRSSIOFFSET_VAL                        0UL
#define RFE_COMMON_RAM_PHYRSSIOFFSET_VAL_BM                     0x00FFUL
// --------------------------------------------------------------
// SPARE0SHADOW
// 
#define RFE_COMMON_RAM_SPARE0SHADOW_ADR (RFE_COMMON_RAM_BASE + 0x0840UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_SPARE0SHADOW = (unsigned short*) RFE_COMMON_RAM_SPARE0SHADOW_ADR;
#define S_RFE_COMMON_RAM_SPARE0SHADOW (*SP_RFE_COMMON_RAM_SPARE0SHADOW)
// bitfield: SPARE0SHADOW_VAL
#define RFE_COMMON_RAM_SPARE0SHADOW_VAL                         0UL
#define RFE_COMMON_RAM_SPARE0SHADOW_VAL_BM                      0xFFFFUL
// --------------------------------------------------------------
// SPARE1SHADOW
// 
#define RFE_COMMON_RAM_SPARE1SHADOW_ADR (RFE_COMMON_RAM_BASE + 0x0842UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_SPARE1SHADOW = (unsigned short*) RFE_COMMON_RAM_SPARE1SHADOW_ADR;
#define S_RFE_COMMON_RAM_SPARE1SHADOW (*SP_RFE_COMMON_RAM_SPARE1SHADOW)
// bitfield: SPARE1SHADOW_VAL
#define RFE_COMMON_RAM_SPARE1SHADOW_VAL                         0UL
#define RFE_COMMON_RAM_SPARE1SHADOW_VAL_BM                      0xFFFFUL
// --------------------------------------------------------------
// AGCINFO
// 
#define RFE_COMMON_RAM_AGCINFO_ADR (RFE_COMMON_RAM_BASE + 0x0844UL)
static volatile unsigned short* const SP_RFE_COMMON_RAM_AGCINFO = (unsigned short*) RFE_COMMON_RAM_AGCINFO_ADR;
#define S_RFE_COMMON_RAM_AGCINFO (*SP_RFE_COMMON_RAM_AGCINFO)
// bitfield: AGCINFO_MODE
#define RFE_COMMON_RAM_AGCINFO_MODE                             0UL
#define RFE_COMMON_RAM_AGCINFO_MODE_BM                          0x0001UL
// enums for bitfield AGCINFO_MODE (width: 1)UL
#define RFE_COMMON_RAM_AGCINFO_MODE_FAST                        0x0UL
#define RFE_COMMON_RAM_AGCINFO_MODE_GEN                         0x1UL

#endif
