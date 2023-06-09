// ===========================================================================
// This file is autogenerated, please DO NOT modify!
//
// Generated on  2023-04-18 13:27:20
// by user:      developer
// on machine:   swtools
// CWD:          /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tc/octopus/regs/inc
// Commandline:  /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tools/topsm/topsmregs.pl -target c -base 0x40092000 -dat_sz 16 -sub 128 -ramreg -i /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/pbe/common/doc/ram_registers.txt
// C&P friendly: /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tools/topsm/topsmregs.pl -target c -base 0x40092000 -dat_sz 16 -sub 128 -ramreg -i /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/pbe/common/doc/ram_registers.txt
//
// Relevant file version(s):
//
// /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tools/topsm/topsmregs.pl
//   rcs-info: (file not managed or unknown revision control system)
//   git-hash: 0d11a0ea4ba55ba3ef648be18aeec231d5314753
//
// /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/pbe/common/doc/ram_registers.txt
//   rcs-info: (file not managed or unknown revision control system)
//   git-hash: 4cd64dd059a51bb9e63980f8d142d21323ebdf8b
//
// ===========================================================================


#ifndef __PBE_COMMON_RAM_REGS_H
#define __PBE_COMMON_RAM_REGS_H

#define PBE_COMMON_RAM_BASE 0x40092000UL
// --------------------------------------------------------------
// CMDPAR0
// 
#define PBE_COMMON_RAM_CMDPAR0_ADR (PBE_COMMON_RAM_BASE + 0x0000UL)
static volatile unsigned short* const SP_PBE_COMMON_RAM_CMDPAR0 = (unsigned short*) PBE_COMMON_RAM_CMDPAR0_ADR;
#define S_PBE_COMMON_RAM_CMDPAR0 (*SP_PBE_COMMON_RAM_CMDPAR0)
// bitfield: CMDPAR0_VAL
#define PBE_COMMON_RAM_CMDPAR0_VAL                              0UL
#define PBE_COMMON_RAM_CMDPAR0_VAL_BM                           0xFFFFUL
// --------------------------------------------------------------
// CMDPAR1
// 
#define PBE_COMMON_RAM_CMDPAR1_ADR (PBE_COMMON_RAM_BASE + 0x0002UL)
static volatile unsigned short* const SP_PBE_COMMON_RAM_CMDPAR1 = (unsigned short*) PBE_COMMON_RAM_CMDPAR1_ADR;
#define S_PBE_COMMON_RAM_CMDPAR1 (*SP_PBE_COMMON_RAM_CMDPAR1)
// bitfield: CMDPAR1_VAL
#define PBE_COMMON_RAM_CMDPAR1_VAL                              0UL
#define PBE_COMMON_RAM_CMDPAR1_VAL_BM                           0xFFFFUL
// --------------------------------------------------------------
// MSGBOX
// 
#define PBE_COMMON_RAM_MSGBOX_ADR (PBE_COMMON_RAM_BASE + 0x0004UL)
static volatile unsigned short* const SP_PBE_COMMON_RAM_MSGBOX = (unsigned short*) PBE_COMMON_RAM_MSGBOX_ADR;
#define S_PBE_COMMON_RAM_MSGBOX (*SP_PBE_COMMON_RAM_MSGBOX)
// bitfield: MSGBOX_VAL
#define PBE_COMMON_RAM_MSGBOX_VAL                               0UL
#define PBE_COMMON_RAM_MSGBOX_VAL_BM                            0xFFFFUL
// --------------------------------------------------------------
// ENDCAUSE
// 
#define PBE_COMMON_RAM_ENDCAUSE_ADR (PBE_COMMON_RAM_BASE + 0x0006UL)
static volatile unsigned short* const SP_PBE_COMMON_RAM_ENDCAUSE = (unsigned short*) PBE_COMMON_RAM_ENDCAUSE_ADR;
#define S_PBE_COMMON_RAM_ENDCAUSE (*SP_PBE_COMMON_RAM_ENDCAUSE)
// bitfield: ENDCAUSE_STAT
#define PBE_COMMON_RAM_ENDCAUSE_STAT                            0UL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_BM                         0x00FFUL
// enums for bitfield ENDCAUSE_STAT (width: 8)UL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_ENDOK                      0x00UL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_RXTIMEOUT                  0x01UL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_NOSYNC                     0x02UL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_RXERR                      0x03UL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_CONNECT                    0x04UL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_SCANRSP                    0x06UL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_MAXNAK                     0x06UL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_EOPSTOP                    0x07UL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_ERR_RXF                    0xF9UL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_ERR_TXF                    0xFAUL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_ERR_SYNTH                  0xFBUL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_ERR_STOP                   0xFCUL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_ERR_PAR                    0xFDUL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_ERR_BADOP                  0xFEUL
#define PBE_COMMON_RAM_ENDCAUSE_STAT_ERR_INTERNAL               0xFFUL

#endif
