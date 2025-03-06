#ifndef ZB_EMILL_SUPPORT_CHECK_H
#define ZB_EMILL_SUPPORT_CHECK_H

/**
 * This macro is used here to notify zb_vendor.h that only macro information is needed,
 * as some platforms may declare types, functions, and variables there,
 * which makes it impossible to utilize vendor for Assembly preprocessing
 */
#define PREPROCESSOR_ONLY
#include "../../include/zb_vendor.h"
#undef  PREPROCESSOR_ONLY

#define ZB_EMILL_MIN_CORTEX_M 4U

/**
 * To enable usage of Emill security backend, you need to provide following definitions in zb_vendor.h file:
 * ZB_USE_CORTEX_SW_SECURITY  indicates that Cortex-optimized SW security shall be enabled if possible
 * ZB_CORTEX_M                shall be equal to target Cortex-M generation
 *
 * ZB_IS_EMILL_SUPPORTED will contain logical value, indicating wether Emill security implementation is enabled
 */
#if !defined (ZB_USE_CORTEX_SW_SECURITY) || !defined (ZB_CORTEX_M)
#define ZB_IS_EMILL_SUPPORTED ZB_FALSE
#else
#define ZB_IS_EMILL_SUPPORTED (ZB_CORTEX_M >= ZB_EMILL_MIN_CORTEX_M)
#endif

#endif /* ZB_EMILL_SUPPORT_CHECK_H */
