/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2025, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

#ifndef ZB_HAL_UTILS_H
#define ZB_HAL_UTILS_H 1

#ifdef ZB_ZGPD_ROLE
extern void osif_deep_sleep(void);
extern void zgpd_wait_for_event(void);
#endif // ZB_ZGPD_ROLE
extern void zb_ti_f3_abort(void);
extern void zb_ti_f3_enable_all_inter(void);
extern void zb_ti_f3_disable_all_inter(void);
#if defined(ZB_MULTI_TEST) || defined(ZB_MAC_TESTING_MODE)
extern zb_bool_t zb_ti_f3_main(void);
#else
extern void zb_ti_f3_main(void);
#endif
extern void zb_ti_f3_platform_init(bool gpd);

extern void zb_memcpy( void *dst, const void *src, unsigned int len );
extern zb_int8_t zb_memcmp(const void *src1, const void *src2, unsigned int len);
extern void zb_memset(void *str, unsigned int c, unsigned int len);
extern void zb_memmove(void *dst, const void *src, size_t len);


#if defined(ZB_MULTI_TEST) || defined(ZB_MAC_TESTING_MODE)
#define MAIN() zb_bool_t zb_ti_f3_main(void)
#else
#define MAIN() void zb_ti_f3_main(void)
#endif
#define MAIN_RETURN(v) return
#define FAKE_ARGV
#define ZVUNUSED(v) (void)v
#define ARGV_UNUSED char **argv = NULL; ZVUNUSED(argv)
#define ARGC_ARGV
#define ARGC_ARGV_PROTO void

#define ZB_VOLATILE
#define ZB_SDCC_XDATA
#define ZB_CALLBACK
#define ZB_SDCC_BANKED
#define ZB_KEIL_REENTRANT

/* use macros to be able to redefine */
#define ZB_ABORT zb_ti_f3_abort
#define ZB_ENABLE_ALL_INTER()  zb_ti_f3_enable_all_inter()
#define ZB_DISABLE_ALL_INTER() zb_ti_f3_disable_all_inter()
#define ZB_OSIF_GLOBAL_LOCK()         ZB_DISABLE_ALL_INTER()
#define ZB_OSIF_GLOBAL_UNLOCK()       ZB_ENABLE_ALL_INTER()
#define ZB_PLATFORM_INIT()     zb_ti_f3_platform_init(false)
#define FATAL_ERR() do {} while(ZB_TRUE)


#define ZB_MEMCPY   (void)zb_memcpy
#define ZB_MEMMOVE  (void)zb_memmove
#define ZB_MEMSET   (void)zb_memset
#define ZB_MEMCMP   zb_memcmp
#define ZB_BZERO(s,l) ZB_MEMSET((char*)(s), 0, (l))
#define ZB_BZERO2(s) ZB_BZERO(s, 2)
#define ZB_CONST_CAT(x, y) x##y
#define ZB_CONST_U(x) ZB_CONST_CAT(x, U)

#define ZB_TRANSPORT_NONBLOCK_ITERATION() 0

/* At ARM all types from 1 to 4 bytes are passed to vararg with casting to 4 bytes */
typedef zb_uint32_t zb_minimal_vararg_t;

#if defined SNCP_MODE

#error "SNCP mode needs to be implemented"

/* defined in linker script file */
extern zb_uint32_t zbs_ncp_lnk_fw_base;
extern zb_uint32_t zbs_ncp_lnk_nvram_base;
extern zb_uint32_t zbs_ncp_lnk_reserv_base;
extern zb_uint32_t zbs_ncp_lnk_prodcfg_base;
extern zb_uint32_t zbs_ncp_lnk_ccfg_page_base;
extern zb_uint32_t zbs_ncp_lnk_ccfg_high_base;

#define ZBS_NCP_IMAGE_ADDR ((zb_uint32_t)&zbs_ncp_lnk_fw_base)
#define ZBS_NCP_NVRAM_BASE ((zb_uint32_t)&zbs_ncp_lnk_nvram_base)
#define ZBS_NCP_RESERV_BASE ((zb_uint32_t)&zbs_ncp_lnk_reserv_base)
#define ZBS_NCP_PROD_CFG_BASE ((zb_uint32_t)&zbs_ncp_lnk_prodcfg_base)
#define ZBS_NCP_CCFG_PAGE_BASE ((zb_uint32_t)&zbs_ncp_lnk_ccfg_page_base) /* page where ccfg is located */
#define ZBS_NCP_CCFG_HIGH_BASE ((zb_uint32_t)&zbs_ncp_lnk_ccfg_high_base) /* address of ccfg reserved area, actual address of beginning of ccfg is end of the page */

#define ZB_OSIF_PRODUCTION_CONFIG_BLOCK_ADDRESS   ZBS_NCP_PROD_CFG_BASE

#else
#define ZB_OSIF_PRODUCTION_CONFIG_BLOCK_ADDRESS   "TODO: Implement"
#endif

#define ZBOSS_DEVICE (defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE)
#define GBOSS_DEVICE (defined ZB_ZGPD_ROLE)

typedef struct ti_zigbee_config_t
{
    zb_uint8_t aesecbIndex;
    zb_uint8_t aesccmIndex;
    zb_uint8_t rngIndex;
    zb_uint8_t nvsIndex;
    zb_uint8_t sha2Index;
    zb_uint8_t ecdhIndex;
} ti_zigbee_config;

extern const ti_zigbee_config zigbeeConfig;

#endif /* ZB_HAL_UTILS_H */
