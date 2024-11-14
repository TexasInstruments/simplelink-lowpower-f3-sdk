#ifndef ZB_SHA_256_H
#define ZB_SHA_256_H 1
#include "zb_common.h"

#define ZB_MAX_INFO_LEN 160U

void crypto_hash(zb_uint8_t *out, const zb_uint8_t *in, zb_uint64_t inlen);

#endif  /* ZB_SHA_256_H */
