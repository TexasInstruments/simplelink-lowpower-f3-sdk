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

#ifndef ZB_HAL_TIMER_H
#define ZB_HAL_TIMER_H 1

#include <ti/drivers/dpl/TimestampP.h>

#ifdef ZB_ZGPD_ROLE
extern void zgpd_timer_set(zb_uint32_t timeout_ms);
extern void zgpd_timer_clear();
extern zb_bool_t zgpd_timer_is_expired();
extern void zgpd_sleep_tmo(zb_uint32_t timeout_ms);

#define TIMER_INTERVAL_US 15360u
#define ZB_TIMER_GET() ((TimestampP_getNative64() >> 12U) / (TIMER_INTERVAL_US))
#endif // ZB_ZGPD_ROLE

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
extern void zb_osif_timer_init(void);
extern void zb_osif_timer_stop(void);
extern void zb_osif_timer_start(void);
extern zb_bool_t zb_osif_timer_is_on(void);
extern zb_time_t osif_sub_trans_timer(zb_time_t t2, zb_time_t t1);
extern zb_time_t osif_transceiver_time_get();
extern zb_uint32_t zb_get_utc_time(void);

#define ZB_START_HW_TIMER()    zb_osif_timer_start()
#define ZB_STOP_HW_TIMER()     zb_osif_timer_stop()
#define ZB_CHECK_TIMER_IS_ON() zb_osif_timer_is_on()
#endif // defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE

#endif /* ZB_HAL_TIMER_H */
