// ===========================================================================
// This file is autogenerated, please DO NOT modify!
//
// Generated on  2023-04-18 13:27:20
// by user:      developer
// on machine:   swtools
// CWD:          /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tc/octopus/regs/inc
// Commandline:  /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tools/topsm/topsmregs.pl -target c -base 0x40084000 -dat_sz 32 -i /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tc/octopus/regs/doc/TRCregs.txt
// C&P friendly: /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tools/topsm/topsmregs.pl -target c -base 0x40084000 -dat_sz 32 -i /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tc/octopus/regs/doc/TRCregs.txt
//
// Relevant file version(s):
//
// /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tools/topsm/topsmregs.pl
//   rcs-info: (file not managed or unknown revision control system)
//   git-hash: 0d11a0ea4ba55ba3ef648be18aeec231d5314753
//
// /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tc/octopus/regs/doc/TRCregs.txt
//   rcs-info: (file not managed or unknown revision control system)
//   git-hash: 587009cf7a0e7d6b41d11dc415afe1da4ba5d1bc
//
// ===========================================================================


#ifndef __TRC_REGS_H
#define __TRC_REGS_H

#ifndef __HW_TYPES_H__ 
  #ifndef HWREG
    #define HWREG(x) (*((volatile unsigned long *)(x)))
  #endif
#endif
#define TRC_BASE 0x40084000UL
#define RF24_TRC_BASE 0x40084000UL
// --------------------------------------------------------------
// CFG
// 
#define TRC_CFG_ADR (TRC_BASE + 0x0000UL)
static volatile unsigned long* const SP_TRC_CFG = (unsigned long*) TRC_CFG_ADR;
#define S_TRC_CFG (*SP_TRC_CFG)
#define RF24_TRC_O_CFG                           0
// bitfield: CFG_PRESCAL
#define TRC_CFG_PRESCAL                              7UL
#define RF24_TRC_CFG_PRESCAL_S                   7UL
#define TRC_CFG_PRESCAL_BM                           0x00000180UL
#define RF24_TRC_CFG_PRESCAL_M                   0x00000180UL
// enums for bitfield CFG_PRESCAL (width: 2)UL
#define TRC_CFG_PRESCAL_DIV1                         0x0UL
#define TRC_CFG_PRESCAL_DIV2                         0x1UL
#define TRC_CFG_PRESCAL_DIV3                         0x2UL
#define TRC_CFG_PRESCAL_DIV4                         0x3UL
// bitfield: CFG_TSCLR
#define TRC_CFG_TSCLR                                6UL
#define RF24_TRC_CFG_TSCLR_S                     6UL
#define TRC_CFG_TSCLR_BM                             0x00000040UL
#define RF24_TRC_CFG_TSCLR_M                     0x00000040UL
// enums for bitfield CFG_TSCLR (width: 1)UL
#define TRC_CFG_TSCLR_ZERO                           0x0UL
#define TRC_CFG_TSCLR_ONE                            0x1UL
// bitfield: CFG_TSEN
#define TRC_CFG_TSEN                                 5UL
#define RF24_TRC_CFG_TSEN_S                      5UL
#define TRC_CFG_TSEN_BM                              0x00000020UL
#define RF24_TRC_CFG_TSEN_M                      0x00000020UL
// enums for bitfield CFG_TSEN (width: 1)UL
#define TRC_CFG_TSEN_OFF                             0x0UL
#define TRC_CFG_TSEN_ON                              0x1UL
// bitfield: CFG_CH3EN
#define TRC_CFG_CH3EN                                3UL
#define RF24_TRC_CFG_CH3EN_S                     3UL
#define TRC_CFG_CH3EN_BM                             0x00000018UL
#define RF24_TRC_CFG_CH3EN_M                     0x00000018UL
// enums for bitfield CFG_CH3EN (width: 2)UL
#define TRC_CFG_CH3EN_OFF                            0x0UL
#define TRC_CFG_CH3EN_NORM                           0x1UL
#define TRC_CFG_CH3EN_TOPSM                          0x2UL
// bitfield: CFG_CH2EN
#define TRC_CFG_CH2EN                                1UL
#define RF24_TRC_CFG_CH2EN_S                     1UL
#define TRC_CFG_CH2EN_BM                             0x00000006UL
#define RF24_TRC_CFG_CH2EN_M                     0x00000006UL
// enums for bitfield CFG_CH2EN (width: 2)UL
#define TRC_CFG_CH2EN_OFF                            0x0UL
#define TRC_CFG_CH2EN_NORM                           0x1UL
#define TRC_CFG_CH2EN_TOPSM                          0x2UL
// bitfield: CFG_CH1EN
#define TRC_CFG_CH1EN                                0UL
#define RF24_TRC_CFG_CH1EN_S                     0UL
#define TRC_CFG_CH1EN_BM                             0x00000001UL
#define RF24_TRC_CFG_CH1EN_M                     0x00000001UL
// enums for bitfield CFG_CH1EN (width: 1)UL
#define TRC_CFG_CH1EN_OFF                            0x0UL
#define TRC_CFG_CH1EN_NORM                           0x1UL
// --------------------------------------------------------------
// CH1CMD
// 
#define TRC_CH1CMD_ADR (TRC_BASE + 0x0004UL)
static volatile unsigned long* const SP_TRC_CH1CMD = (unsigned long*) TRC_CH1CMD_ADR;
#define S_TRC_CH1CMD (*SP_TRC_CH1CMD)
#define RF24_TRC_O_CH1CMD                        4
// bitfield: CH1CMD_PKTHDR
#define TRC_CH1CMD_PKTHDR                            8UL
#define RF24_TRC_CH1CMD_PKTHDR_S                 8UL
#define TRC_CH1CMD_PKTHDR_BM                         0x0000FF00UL
#define RF24_TRC_CH1CMD_PKTHDR_M                 0x0000FF00UL
// enums for bitfield CH1CMD_PKTHDR (width: 8)UL
#define TRC_CH1CMD_PKTHDR_ALLZEROS                   0x00UL
#define TRC_CH1CMD_PKTHDR_ALLONES                    0xFFUL
// bitfield: CH1CMD_PARCNT
#define TRC_CH1CMD_PARCNT                            0UL
#define RF24_TRC_CH1CMD_PARCNT_S                 0UL
#define TRC_CH1CMD_PARCNT_BM                         0x00000007UL
#define RF24_TRC_CH1CMD_PARCNT_M                 0x00000007UL
// enums for bitfield CH1CMD_PARCNT (width: 3)UL
#define TRC_CH1CMD_PARCNT_ALLZEROS                   0x0UL
#define TRC_CH1CMD_PARCNT_ALLONES                    0x7UL
// --------------------------------------------------------------
// CH2CMD
// 
#define TRC_CH2CMD_ADR (TRC_BASE + 0x0008UL)
static volatile unsigned long* const SP_TRC_CH2CMD = (unsigned long*) TRC_CH2CMD_ADR;
#define S_TRC_CH2CMD (*SP_TRC_CH2CMD)
#define RF24_TRC_O_CH2CMD                        8
// bitfield: CH2CMD_PKTHDR
#define TRC_CH2CMD_PKTHDR                            8UL
#define RF24_TRC_CH2CMD_PKTHDR_S                 8UL
#define TRC_CH2CMD_PKTHDR_BM                         0x0000FF00UL
#define RF24_TRC_CH2CMD_PKTHDR_M                 0x0000FF00UL
// enums for bitfield CH2CMD_PKTHDR (width: 8)UL
#define TRC_CH2CMD_PKTHDR_ALLZEROS                   0x00UL
#define TRC_CH2CMD_PKTHDR_ALLONES                    0xFFUL
// bitfield: CH2CMD_PARCNT
#define TRC_CH2CMD_PARCNT                            0UL
#define RF24_TRC_CH2CMD_PARCNT_S                 0UL
#define TRC_CH2CMD_PARCNT_BM                         0x00000007UL
#define RF24_TRC_CH2CMD_PARCNT_M                 0x00000007UL
// enums for bitfield CH2CMD_PARCNT (width: 3)UL
#define TRC_CH2CMD_PARCNT_ALLZEROS                   0x0UL
#define TRC_CH2CMD_PARCNT_ALLONES                    0x7UL
// --------------------------------------------------------------
// CH3CMD
// 
#define TRC_CH3CMD_ADR (TRC_BASE + 0x000CUL)
static volatile unsigned long* const SP_TRC_CH3CMD = (unsigned long*) TRC_CH3CMD_ADR;
#define S_TRC_CH3CMD (*SP_TRC_CH3CMD)
#define RF24_TRC_O_CH3CMD                        12
// bitfield: CH3CMD_PKTHDR
#define TRC_CH3CMD_PKTHDR                            8UL
#define RF24_TRC_CH3CMD_PKTHDR_S                 8UL
#define TRC_CH3CMD_PKTHDR_BM                         0x0000FF00UL
#define RF24_TRC_CH3CMD_PKTHDR_M                 0x0000FF00UL
// enums for bitfield CH3CMD_PKTHDR (width: 8)UL
#define TRC_CH3CMD_PKTHDR_ALLZEROS                   0x00UL
#define TRC_CH3CMD_PKTHDR_ALLONES                    0xFFUL
// bitfield: CH3CMD_PARCNT
#define TRC_CH3CMD_PARCNT                            0UL
#define RF24_TRC_CH3CMD_PARCNT_S                 0UL
#define TRC_CH3CMD_PARCNT_BM                         0x00000007UL
#define RF24_TRC_CH3CMD_PARCNT_M                 0x00000007UL
// enums for bitfield CH3CMD_PARCNT (width: 3)UL
#define TRC_CH3CMD_PARCNT_ALLZEROS                   0x0UL
#define TRC_CH3CMD_PARCNT_ALLONES                    0x7UL
// --------------------------------------------------------------
// CH1PAR01
// 
#define TRC_CH1PAR01_ADR (TRC_BASE + 0x0014UL)
static volatile unsigned long* const SP_TRC_CH1PAR01 = (unsigned long*) TRC_CH1PAR01_ADR;
#define S_TRC_CH1PAR01 (*SP_TRC_CH1PAR01)
#define RF24_TRC_O_CH1PAR01                      20
// bitfield: CH1PAR01_PAR1
#define TRC_CH1PAR01_PAR1                            16UL
#define RF24_TRC_CH1PAR01_PAR1_S                 16UL
#define TRC_CH1PAR01_PAR1_BM                         0xFFFF0000UL
#define RF24_TRC_CH1PAR01_PAR1_M                 0xFFFF0000UL
// enums for bitfield CH1PAR01_PAR1 (width: 16)UL
#define TRC_CH1PAR01_PAR1_ALLZEROS                   0x0000UL
#define TRC_CH1PAR01_PAR1_ALLONES                    0xFFFFUL
// bitfield: CH1PAR01_PAR0
#define TRC_CH1PAR01_PAR0                            0UL
#define RF24_TRC_CH1PAR01_PAR0_S                 0UL
#define TRC_CH1PAR01_PAR0_BM                         0x0000FFFFUL
#define RF24_TRC_CH1PAR01_PAR0_M                 0x0000FFFFUL
// enums for bitfield CH1PAR01_PAR0 (width: 16)UL
#define TRC_CH1PAR01_PAR0_ALLZEROS                   0x0000UL
#define TRC_CH1PAR01_PAR0_ALLONES                    0xFFFFUL
// --------------------------------------------------------------
// CH2PAR01
// 
#define TRC_CH2PAR01_ADR (TRC_BASE + 0x0018UL)
static volatile unsigned long* const SP_TRC_CH2PAR01 = (unsigned long*) TRC_CH2PAR01_ADR;
#define S_TRC_CH2PAR01 (*SP_TRC_CH2PAR01)
#define RF24_TRC_O_CH2PAR01                      24
// bitfield: CH2PAR01_PAR1
#define TRC_CH2PAR01_PAR1                            16UL
#define RF24_TRC_CH2PAR01_PAR1_S                 16UL
#define TRC_CH2PAR01_PAR1_BM                         0xFFFF0000UL
#define RF24_TRC_CH2PAR01_PAR1_M                 0xFFFF0000UL
// enums for bitfield CH2PAR01_PAR1 (width: 16)UL
#define TRC_CH2PAR01_PAR1_ALLZEROS                   0x0000UL
#define TRC_CH2PAR01_PAR1_ALLONES                    0xFFFFUL
// bitfield: CH2PAR01_PAR0
#define TRC_CH2PAR01_PAR0                            0UL
#define RF24_TRC_CH2PAR01_PAR0_S                 0UL
#define TRC_CH2PAR01_PAR0_BM                         0x0000FFFFUL
#define RF24_TRC_CH2PAR01_PAR0_M                 0x0000FFFFUL
// enums for bitfield CH2PAR01_PAR0 (width: 16)UL
#define TRC_CH2PAR01_PAR0_ALLZEROS                   0x0000UL
#define TRC_CH2PAR01_PAR0_ALLONES                    0xFFFFUL
// --------------------------------------------------------------
// CH3PAR01
// 
#define TRC_CH3PAR01_ADR (TRC_BASE + 0x001CUL)
static volatile unsigned long* const SP_TRC_CH3PAR01 = (unsigned long*) TRC_CH3PAR01_ADR;
#define S_TRC_CH3PAR01 (*SP_TRC_CH3PAR01)
#define RF24_TRC_O_CH3PAR01                      28
// bitfield: CH3PAR01_PAR1
#define TRC_CH3PAR01_PAR1                            16UL
#define RF24_TRC_CH3PAR01_PAR1_S                 16UL
#define TRC_CH3PAR01_PAR1_BM                         0xFFFF0000UL
#define RF24_TRC_CH3PAR01_PAR1_M                 0xFFFF0000UL
// enums for bitfield CH3PAR01_PAR1 (width: 16)UL
#define TRC_CH3PAR01_PAR1_ALLZEROS                   0x0000UL
#define TRC_CH3PAR01_PAR1_ALLONES                    0xFFFFUL
// bitfield: CH3PAR01_PAR0
#define TRC_CH3PAR01_PAR0                            0UL
#define RF24_TRC_CH3PAR01_PAR0_S                 0UL
#define TRC_CH3PAR01_PAR0_BM                         0x0000FFFFUL
#define RF24_TRC_CH3PAR01_PAR0_M                 0x0000FFFFUL
// enums for bitfield CH3PAR01_PAR0 (width: 16)UL
#define TRC_CH3PAR01_PAR0_ALLZEROS                   0x0000UL
#define TRC_CH3PAR01_PAR0_ALLONES                    0xFFFFUL
// --------------------------------------------------------------
// CH1PAR23
// 
#define TRC_CH1PAR23_ADR (TRC_BASE + 0x0024UL)
static volatile unsigned long* const SP_TRC_CH1PAR23 = (unsigned long*) TRC_CH1PAR23_ADR;
#define S_TRC_CH1PAR23 (*SP_TRC_CH1PAR23)
#define RF24_TRC_O_CH1PAR23                      36
// bitfield: CH1PAR23_PAR3
#define TRC_CH1PAR23_PAR3                            16UL
#define RF24_TRC_CH1PAR23_PAR3_S                 16UL
#define TRC_CH1PAR23_PAR3_BM                         0xFFFF0000UL
#define RF24_TRC_CH1PAR23_PAR3_M                 0xFFFF0000UL
// enums for bitfield CH1PAR23_PAR3 (width: 16)UL
#define TRC_CH1PAR23_PAR3_ALLZEROS                   0x0000UL
#define TRC_CH1PAR23_PAR3_ALLONES                    0xFFFFUL
// bitfield: CH1PAR23_PAR2
#define TRC_CH1PAR23_PAR2                            0UL
#define RF24_TRC_CH1PAR23_PAR2_S                 0UL
#define TRC_CH1PAR23_PAR2_BM                         0x0000FFFFUL
#define RF24_TRC_CH1PAR23_PAR2_M                 0x0000FFFFUL
// enums for bitfield CH1PAR23_PAR2 (width: 16)UL
#define TRC_CH1PAR23_PAR2_ALLZEROS                   0x0000UL
#define TRC_CH1PAR23_PAR2_ALLONES                    0xFFFFUL
// --------------------------------------------------------------
// CH2PAR23
// 
#define TRC_CH2PAR23_ADR (TRC_BASE + 0x0028UL)
static volatile unsigned long* const SP_TRC_CH2PAR23 = (unsigned long*) TRC_CH2PAR23_ADR;
#define S_TRC_CH2PAR23 (*SP_TRC_CH2PAR23)
#define RF24_TRC_O_CH2PAR23                      40
// bitfield: CH2PAR23_PAR3
#define TRC_CH2PAR23_PAR3                            16UL
#define RF24_TRC_CH2PAR23_PAR3_S                 16UL
#define TRC_CH2PAR23_PAR3_BM                         0xFFFF0000UL
#define RF24_TRC_CH2PAR23_PAR3_M                 0xFFFF0000UL
// enums for bitfield CH2PAR23_PAR3 (width: 16)UL
#define TRC_CH2PAR23_PAR3_ALLZEROS                   0x0000UL
#define TRC_CH2PAR23_PAR3_ALLONES                    0xFFFFUL
// bitfield: CH2PAR23_PAR2
#define TRC_CH2PAR23_PAR2                            0UL
#define RF24_TRC_CH2PAR23_PAR2_S                 0UL
#define TRC_CH2PAR23_PAR2_BM                         0x0000FFFFUL
#define RF24_TRC_CH2PAR23_PAR2_M                 0x0000FFFFUL
// enums for bitfield CH2PAR23_PAR2 (width: 16)UL
#define TRC_CH2PAR23_PAR2_ALLZEROS                   0x0000UL
#define TRC_CH2PAR23_PAR2_ALLONES                    0xFFFFUL
// --------------------------------------------------------------
// CH3PAR23
// 
#define TRC_CH3PAR23_ADR (TRC_BASE + 0x002CUL)
static volatile unsigned long* const SP_TRC_CH3PAR23 = (unsigned long*) TRC_CH3PAR23_ADR;
#define S_TRC_CH3PAR23 (*SP_TRC_CH3PAR23)
#define RF24_TRC_O_CH3PAR23                      44
// bitfield: CH3PAR23_PAR3
#define TRC_CH3PAR23_PAR3                            16UL
#define RF24_TRC_CH3PAR23_PAR3_S                 16UL
#define TRC_CH3PAR23_PAR3_BM                         0xFFFF0000UL
#define RF24_TRC_CH3PAR23_PAR3_M                 0xFFFF0000UL
// enums for bitfield CH3PAR23_PAR3 (width: 16)UL
#define TRC_CH3PAR23_PAR3_ALLZEROS                   0x0000UL
#define TRC_CH3PAR23_PAR3_ALLONES                    0xFFFFUL
// bitfield: CH3PAR23_PAR2
#define TRC_CH3PAR23_PAR2                            0UL
#define RF24_TRC_CH3PAR23_PAR2_S                 0UL
#define TRC_CH3PAR23_PAR2_BM                         0x0000FFFFUL
#define RF24_TRC_CH3PAR23_PAR2_M                 0x0000FFFFUL
// enums for bitfield CH3PAR23_PAR2 (width: 16)UL
#define TRC_CH3PAR23_PAR2_ALLZEROS                   0x0000UL
#define TRC_CH3PAR23_PAR2_ALLONES                    0xFFFFUL

#endif
