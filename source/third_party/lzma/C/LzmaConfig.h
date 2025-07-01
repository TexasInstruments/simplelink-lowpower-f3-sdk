/* LzmaConfig.h : LZMA Configurations
2025-05-21 : Texas Instruments : Public domain */

#include "mcuboot_config.h"

#ifndef LZMA2_DECOMP_CHUNK_SIZE
#define LZMA2_DECOMP_CHUNK_SIZE 256
#endif

#ifndef LZMA_LIT_SIZE
#define LZMA_LIT_SIZE 0x300
#endif

#ifndef LZMA2_LC
#define LZMA2_LC 0
#endif
#ifndef LZMA2_LP
#define LZMA2_LP 0
#endif
#ifndef LZMA2_PB
#define LZMA2_PB 0
#endif

#ifndef LZMA2_LCLP_MAX
#define LZMA2_LCLP_MAX (LZMA2_LC + LZMA2_LP)
#endif

#ifndef LZMA2_PROBS_SIZE
#define LZMA2_PROBS_SIZE (((1984) + (LZMA_LIT_SIZE << LZMA2_LCLP_MAX)))
#endif

#ifndef LZMA2_DIC_SIZE
#define LZMA2_DIC_SIZE 12288
#endif
