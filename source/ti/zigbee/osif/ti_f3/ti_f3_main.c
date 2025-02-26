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
/*
 PURPOSE: Platform specific for TI F3 SoC.
 */
#define ZB_TRACE_FILE_ID 30011

#include "zb_common.h"

#ifdef ZB_ZGPD_ROLE
#include "zb_osif.h"
#else // R23_MAC
#include "zb_osif_platform.h"
#endif /* ZB_ZGPD_ROLE */

/*cstat -MISRAC2012-* */
#if !defined(NORTOS)
#include <pthread.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#else
#include <NoRTOS.h>
#endif //NORTOS

#include <ti/drivers/Power.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/pmctl.h)

#include "ti_drivers_config.h"

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
#include <ti/zigbee/osif/include/zb_hal_crypto.h>
#endif
/*cstat +MISRAC2012-* */

/* Stack size in bytes */
/* Stack size in bytes */
#if defined SNCP_MODE || defined ZB_ZGPD_ROLE || defined ZBOSS_REV22
/* increased because of large aps binding table */
#define THREADSTACKSIZE    2048U
#else //defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
#define THREADSTACKSIZE    6000U
#endif // defined SNCP_MODE || defined ZB_ZGPD_ROLE
#include <ti/log/Log.h>

#define MAX_INT_DEPTH 8

static zb_bool_t gs_platform_init_done = ZB_FALSE;
static volatile zb_uint8_t count = 0;
static uintptr_t hwiKey;
extern SemaphoreP_Handle buttonSem;

void *main_task_function(void *arg0)
{
  ZVUNUSED(arg0);

  Log_printf(LogModule_Zigbee, Log_DEBUG, "initialized");

  zb_ti_f3_main();

  return NULL;
}

#if !defined(NORTOS)
int main(void)
{
  pthread_t           thread;
  pthread_attr_t      attrs;
  struct sched_param  priParam;
  int                 retc;
  int                 detachState;

  /* Call driver init functions */
  Board_init();

  /* Set priority and stack size attributes */
  retc = pthread_attr_init(&attrs);
  if (retc != 0)
  {
    /* pthread_attr_init() failed */
    FATAL_ERR();
  }

  priParam.sched_priority = 1;

  detachState = PTHREAD_CREATE_DETACHED;
  retc = pthread_attr_setdetachstate(&attrs, detachState);
  if (retc != 0)
  {
      /* pthread_attr_setdetachstate() failed */
      FATAL_ERR();
  }

  retc = pthread_attr_setschedparam(&attrs, &priParam);
  if (retc != 0)
  {
    /* pthread_attr_setschedparam() failed */
    FATAL_ERR();
  }

  retc = pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
  if (retc != 0)
  {
      /* pthread_attr_setstacksize() failed */
      FATAL_ERR();
  }

  retc = pthread_create(&thread, &attrs, main_task_function, NULL);
  if (retc != 0)
  {
      /* pthread_create() failed */
      FATAL_ERR();
  }

  /* Start the FreeRTOS scheduler */
  vTaskStartScheduler(); /* does not return */

  return (0);
}
#else
int main(void)
{
  /* Call driver init functions */
  Board_init();

  /* Start NoRTOS */
  NoRTOS_start();

  /* Call mainThread function */
  main_task_function(NULL);

  return (0);
}
#endif //NORTOS

void zb_ti_f3_platform_init(bool gpd)
{
  if (gs_platform_init_done)
  {
    return;
  }

  gs_platform_init_done = ZB_TRUE;

  if(gpd)
  {
#ifdef ZB_USE_BUTTONS
    ti_f3_buttons_init();
#endif //ZB_USE_BUTTONS
  }
  else
  {
#ifdef ZB_HAVE_SERIAL
    zb_osif_serial_init();
#endif // ZB_HAVE_SERIAL

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
    zb_osif_timer_init();

    if (zb_osif_random_init() != RET_OK)
    {
      FATAL_ERR();
    }

    ZB_TIMER_CTX().canstop = ZB_TRUE;
#endif //defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
  }
}

void zb_ti_f3_abort(void)
{
  FATAL_ERR();
}

void zb_ti_f3_enable_all_inter(void)
{
  const unsigned int prev_val = count;
  if(count != 0U)
  {
    count -= 1;
  }

  if (prev_val == 1) {
      HwiP_restore(hwiKey);
      hwiKey = 0;
  }
}

void zb_ti_f3_disable_all_inter(void)
{
  const unsigned int prev_val = count++;
  if (prev_val == 0) {
      hwiKey = HwiP_disable();
  }

}

#ifdef ZB_ZGPD_ROLE
void gboss_platform_init(ARGC_ARGV_PROTO)
{
  zb_ti_f3_platform_init(true);
}

void osif_deep_sleep(void)
{
  /* switch to shutdown mode, the parameters are default, they are device-specific and not used */
  Power_shutdown(0, 0);
}

void zgpd_wait_for_event(void)
{
  do
  {
    if (TRANS_CTX().rx_int != 0u)
    {
      break;
    }

    /* just go to sleep, it's up to layer above to set timers */
    Power_idleFunc();
  }
  while (0);
}
#endif //ZB_ZGPD_ROLE

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE

#ifdef ZB_USE_SLEEP
extern void zb_osif_uart_wake_up(void);
extern void zb_osif_uart_sleep(void);

zb_uint32_t zb_osif_sleep(zb_uint32_t sleep_tmo)
{
  zb_uint32_t sleep_ticks;
  zb_uint32_t t1;
  zb_uint32_t t2;
  zb_uint32_t slept_time_ms;
  zb_uint32_t tickPeriod = ClockP_getSystemTickPeriod();

#ifndef NCP_MODE
#ifdef ZB_HAVE_SERIAL
  zb_osif_uart_sleep();
#endif
#endif

  //Seconds_set(0);
  sleep_ticks = (sleep_tmo * 1000U)/tickPeriod;

  ZG->sleep.sleeping_now = ZB_TRUE;

  // Get starting time
  t1 = ClockP_getSystemTicks();

  /* zzzzZZZZZzzzzzz */
  SemaphoreP_Status semStatus = SemaphoreP_TIMEOUT;
  if(NULL != buttonSem)
  {
    semStatus = SemaphoreP_pend(buttonSem, sleep_ticks);
  }
  else
  {
    ClockP_usleep(sleep_tmo * 1000U);
  }

  // Get ending time
  t2 = ClockP_getSystemTicks();

  if(SemaphoreP_TIMEOUT == semStatus)
  {
    slept_time_ms = sleep_tmo;
  }
  else
  {
    if(t2 > t1)
    {
      slept_time_ms = ((t2-t1) * tickPeriod)/1000U;
    }
    else
    {
      slept_time_ms = ((((uint64_t)t2 + 0xFFFFFFFFU - (uint64_t)t1) % 0xFFFFFFFFU) * tickPeriod)/1000U;
    }
  }

  ZG->sleep.sleeping_now = ZB_FALSE;

  return slept_time_ms;
}
#else

zb_uint32_t zb_osif_sleep(zb_uint32_t sleep_tmo)
{
  return 0;
}
#endif //ZB_USE_SLEEP

void zb_osif_wake_up(void)
{
#ifndef NCP_MODE
#ifdef ZB_HAVE_SERIAL
  zb_osif_uart_wake_up();
#endif //ZB_HAVE_SERIAL
#endif // !NCP_MODE
}

zb_uint8_t zb_get_reset_source(void)
{
  zb_uint32_t rst_src;
  zb_uint8_t zboss_rst_src;

  /*cstat !MISRAC2012-Rule-11.1 Violation cause by a third-party library */
  rst_src  =  PMCTLGetResetReason();

  switch (rst_src)
  {
    case PMCTL_RESET_POR:     /* PWR_ON Power on reset*/
      zboss_rst_src = ZB_RESET_SRC_POWER_ON;
      break;
    case PMCTL_RESET_PIN:  /* PIN_RESET Reset pin*/
      zboss_rst_src = ZB_RESET_SRC_RESET_PIN;
      break;
    case PMCTL_RESET_VDDS:  /* Brown out detect on VDDS fall through */
    case PMCTL_RESET_VDDR:  /* Brown out detect on VDDR */
      zboss_rst_src = ZB_RESET_SRC_BROWN_OUT;
      break;
    case PMCTL_RESET_LFXT:   /* Clock loss detect */
      zboss_rst_src = ZB_RESET_SRC_CLOCK_LOSS;
      break;
    case PMCTL_RESET_SYSTEM:   /* Software reset via SYSRESET register fall through */
      zboss_rst_src = ZB_RESET_SRC_SW_RESET;
      break;
    default:                /* RSTSRC_WAKEUP_FROM_SHUTDOWN or RSTSRC_WAKEUP_FROM_TCK_NOISE or RSTSRC_WARMRESET */
      zboss_rst_src = ZB_RESET_SRC_OTHER;
      break;
  }

  return zboss_rst_src;
}

void zb_reset(zb_uint8_t param)
{
  ZVUNUSED(param);

  PMCTLResetSystem();
}

/* The following stubs were added to satisfy MISRA Rule 8.6.
 * The prototypes are declared unconditionally in zb_osif.h but no
 * implementation is needed for SNCP. Linker will optimize it anyway. */
/*********** BEGIN Stubs to satisfy MISRA Rule 8.6 ***********/
zb_uint32_t osif_get_time_ms(void)
{
  return ZB_UINT32_MAX;
}

zb_ret_t osif_set_transmit_power(zb_uint8_t channel, zb_int8_t power)
{
  ZVUNUSED(channel);
  ZVUNUSED(power);
  return RET_OK;
}

void osif_set_default_trasnmit_powers(zb_int8_t *tx_powers)
{
  ZVUNUSED(tx_powers);
  return;
}

zb_bool_t zb_osif_led_level_init(zb_uint8_t led_no)
{
  ZVUNUSED(led_no);
  return ZB_FALSE;
}

void zb_osif_led_on_set_level(zb_uint8_t level)
{
  ZVUNUSED(level);
  return;
}
/*********** END Stubs to satisfy MISRA Rule 8.6 ***********/
#endif //defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE

#if defined ZB_INTERRUPT_SAFE_CALLBACKS && (defined ZB_TRACE_LEVEL || defined DOXYGEN)
zb_bool_t zb_osif_is_inside_isr(void)
{
  return HwiP_inISR();
}
#endif /* defined ZB_INTERRUPT_SAFE_CALLBACKS && (defined ZB_TRACE_LEVEL || defined DOXYGEN) */

// Required due to M0+ not allowing unaligned memory access.
void zb_memcpy( void *dst, const void *src, unsigned int len )
{
  uint8_t *pDst;
  const uint8_t *pSrc;

  pSrc = src;
  pDst = dst;

  while ( len-- )
    *pDst++ = *pSrc++;

}

zb_int8_t zb_memcmp(const void *src1, const void *src2, unsigned int len)
{
  const uint8_t *pSrc1;
  const uint8_t *pSrc2;

  pSrc1 = src1;
  pSrc2 = src2;

  while (len > 0)
  {
      if (*pSrc1 != *pSrc2)
          return (*pSrc1 - *pSrc2);
      len--;
      pSrc1++;
      pSrc2++;
  }
  return 0;
}

void zb_memset(void *str, unsigned int c, unsigned int len)
{
  uint8_t *pStr = str;

  while (len > 0)
  {
      *pStr = c;
      len--;
      pStr++;
  }
}

void zb_memmove(void *dst, const void *src, size_t len)
{
    size_t i;

    /*
      * If the buffers don't overlap, it doesn't matter what direction
      * we copy in. If they do, it does, so just assume they always do.
      * We don't concern ourselves with the possibility that the region
      * to copy might roll over across the top of memory, because it's
      * not going to happen.
      *
      * If the destination is above the source, we have to copy
      * back to front to avoid overwriting the data we want to
      * copy.
      *
      *      dest:       dddddddd
      *      src:    ssssssss   ^
      *              |   ^  |___|
      *              |___|
      *
      * If the destination is below the source, we have to copy
      * front to back.
      *
      *      dest:   dddddddd
      *      src:    ^   ssssssss
      *              |___|  ^   |
      *                     |___|
      */

    if ((uintptr_t)dst < (uintptr_t)src)
    {
        zb_memcpy(dst, src, len);
        return;
    }

    /*
    * Copy by words in the common case.
    */
    if ((((uintptr_t)dst & 0x3) == 0) &&
        (((uintptr_t)src & 0x3) == 0) &&
        ((len % sizeof(zb_uint32_t)) == 0))
    {

        zb_uint32_t *d = dst;
        const zb_uint32_t *s = src;

        /*
          * The reason we copy index i-1 and test i>0 is that
          * i is unsigned -- so testing i>=0 doesn't work.
          */
        for (i=len/sizeof(zb_uint32_t); i>0; i--)
        {
                d[i-1] = s[i-1];
        }
    }
    else
    {
        zb_uint8_t *d = dst;
        const zb_uint8_t *s = src;

        for (i=len; i>0; i--)
        {
                d[i-1] = s[i-1];
        }
    }
}
