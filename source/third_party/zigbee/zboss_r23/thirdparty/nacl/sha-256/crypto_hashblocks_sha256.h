#ifndef crypto_hashblocks_sha256_H
#define crypto_hashblocks_sha256_H

#include "zb_common.h"

#define crypto_hashblocks_sha256_STATEBYTES 32
#define crypto_hashblocks_sha256_BLOCKBYTES 64

int crypto_hashblocks(zb_uint8_t *statebytes, const zb_uint8_t *in, zb_uint64_t inlen);

#endif
