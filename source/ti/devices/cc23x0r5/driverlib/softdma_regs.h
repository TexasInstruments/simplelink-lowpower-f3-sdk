// ===========================================================================
// This file is autogenerated, please DO NOT modify!
//
// Generated on  2023-04-18 13:27:23
// by user:      developer
// on machine:   swtools
// CWD:          /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tc/octopus/regs/inc
// Commandline:  /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tools/topsm/topsmregs.pl -target c -base 0x80000000 -dat_sz 32 -i /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/source/octopusble/doc/softdma_regs.txt
// C&P friendly: /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tools/topsm/topsmregs.pl -target c -base 0x80000000 -dat_sz 32 -i /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/source/octopusble/doc/softdma_regs.txt
//
// Relevant file version(s):
//
// /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/tools/topsm/topsmregs.pl
//   rcs-info: (file not managed or unknown revision control system)
//   git-hash: 0d11a0ea4ba55ba3ef648be18aeec231d5314753
//
// /home/developer/.conan/data/loki-lrf/7.11.00.18/library-lprf/eng/build/7c92806c54e4cfdd9df8e8c53e37468130adda90/source/octopusble/doc/softdma_regs.txt
//   rcs-info: (file not managed or unknown revision control system)
//   git-hash: 7fa3b063718172f082fdd4ff173ebce0cd168dc9
//
// ===========================================================================


#ifndef __SOFTDMA_REGS_H
#define __SOFTDMA_REGS_H

#define SOFTDMA_BASE 0x80000000UL
// --------------------------------------------------------------
// TRIGCNTRX
// 
#define SOFTDMA_TRIGCNTRX_ADR (SOFTDMA_BASE + 0x0000UL)
static volatile unsigned long* const SP_SOFTDMA_TRIGCNTRX = (unsigned long*) SOFTDMA_TRIGCNTRX_ADR;
#define S_SOFTDMA_TRIGCNTRX (*SP_SOFTDMA_TRIGCNTRX)
// bitfield: TRIGCNTRX_VAL
#define SOFTDMA_TRIGCNTRX_VAL                            0UL
#define SOFTDMA_TRIGCNTRX_VAL_BM                         0x0000003FUL
// --------------------------------------------------------------
// TRIGCNTTX
// 
#define SOFTDMA_TRIGCNTTX_ADR (SOFTDMA_BASE + 0x0004UL)
static volatile unsigned long* const SP_SOFTDMA_TRIGCNTTX = (unsigned long*) SOFTDMA_TRIGCNTTX_ADR;
#define S_SOFTDMA_TRIGCNTTX (*SP_SOFTDMA_TRIGCNTTX)
// bitfield: TRIGCNTTX_VAL
#define SOFTDMA_TRIGCNTTX_VAL                            0UL
#define SOFTDMA_TRIGCNTTX_VAL_BM                         0x0000003FUL
// --------------------------------------------------------------
// DONERX
// 
#define SOFTDMA_DONERX_ADR (SOFTDMA_BASE + 0x0008UL)
static volatile unsigned long* const SP_SOFTDMA_DONERX = (unsigned long*) SOFTDMA_DONERX_ADR;
#define S_SOFTDMA_DONERX (*SP_SOFTDMA_DONERX)
// bitfield: DONERX_TRIGGER
#define SOFTDMA_DONERX_TRIGGER                           2UL
#define SOFTDMA_DONERX_TRIGGER_BM                        0x00000004UL
// bitfield: DONERX_STATUS
#define SOFTDMA_DONERX_STATUS                            0UL
#define SOFTDMA_DONERX_STATUS_BM                         0x00000003UL
// --------------------------------------------------------------
// DONETX
// 
#define SOFTDMA_DONETX_ADR (SOFTDMA_BASE + 0x000CUL)
static volatile unsigned long* const SP_SOFTDMA_DONETX = (unsigned long*) SOFTDMA_DONETX_ADR;
#define S_SOFTDMA_DONETX (*SP_SOFTDMA_DONETX)
// bitfield: DONETX_TRIGGER
#define SOFTDMA_DONETX_TRIGGER                           2UL
#define SOFTDMA_DONETX_TRIGGER_BM                        0x00000004UL
// bitfield: DONETX_STATUS
#define SOFTDMA_DONETX_STATUS                            0UL
#define SOFTDMA_DONETX_STATUS_BM                         0x00000003UL

#endif