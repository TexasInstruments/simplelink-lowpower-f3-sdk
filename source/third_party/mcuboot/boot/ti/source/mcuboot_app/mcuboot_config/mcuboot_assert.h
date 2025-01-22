/*
 * mcuboot_assert.h
 *
 * assert() macro redefinition
 *
 */

#ifndef MCUBOOT_ASSERT_H
#define MCUBOOT_ASSERT_H

#include "bootutil/bootutil_log.h"

#if defined(__GNUC__)
#if defined(assert)
#undef assert
#endif
#endif

#define assert(x) if(!(x)) {BOOT_LOG_ERR("assert: %s line %d", __FILE__, __LINE__);while(1);}

#endif /* MCUBOOT_ASSERT_H */
