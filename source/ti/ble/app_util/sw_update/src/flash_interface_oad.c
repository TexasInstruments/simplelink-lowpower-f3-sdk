/******************************************************************************

 @file  flash_interface_oad.c

 @brief Conditional compilation wrapper for the flash interface.

        When OAD is configured via SysConfig (OAD_CFG defined): compiles the
        real NVS-based implementation from flash_interface_int_rtos_NVS.c.

        When OAD is not configured (OAD_CFG not defined): compiles stub
        implementations so the project builds cleanly without
        CONFIG_NVSINTERNAL1 being defined.

 Group: WCS, BTS
 Target Device: cc23xx

 *******************************************************************************

 Copyright (c) 2024-2026, Texas Instruments Incorporated
 All rights reserved.

 *******************************************************************************

******************************************************************************/

#ifdef OAD_CFG

/* Implementation being used depends OAD method */
#ifdef OAD_APP_OFFCHIP
#include "ti/common/cc26xx/flash_interface/external/flash_interface_ext_rtos_NVS.c"
#else
#include "ti/common/cc26xx/flash_interface/internal/flash_interface_int_rtos_NVS.c"
#endif

#else /* OAD_CFG not defined */

/* Flash functions are not called when OAD is disabled */

#endif /* OAD_CFG */
