/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc27xx

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
/* Common Includes */
/*cstat -MISRAC2012-* */
#include <pthread.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/pmctl.h)

#include "ti_drivers_config.h"
#include <ti/log/Log.h>

/* Zigbee Includes */
#include "zb_common.h"

#ifdef ZB_ZGPD_ROLE
#include "zb_osif.h"
#else // R23_MAC
#include "zb_osif_platform.h"
// Required for sleepy zed check in zb_osif_sleep()
#include "zb_mac_globals.h"
#endif /* ZB_ZGPD_ROLE */

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
#include <ti/zigbee/osif/include/zb_hal_crypto.h>
#endif
/*cstat +MISRAC2012-* */

/* Stack size in bytes */
#if defined SNCP_MODE || defined ZB_ZGPD_ROLE || defined ZBOSS_REV22
/* increased because of large aps binding table */
#define THREADSTACKSIZE    2048U
#else //defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
#define THREADSTACKSIZE    6000U
#endif // defined SNCP_MODE || defined ZB_ZGPD_ROLE
#define MAX_INT_DEPTH 8

/* BLE Includes */
#include <stdint.h>
#ifdef __ICCARM__
    #include <DLib_Threads.h>
#endif

#include "ti/ble/stack_util/icall/app/icall.h"
#include "ti/ble/stack_util/health_toolkit/assert.h"
#include "ti/ble/stack_util/bcomdef.h"
#ifndef USE_DEFAULT_USER_CFG
#include "ti/ble/app_util/config/ble_user_config.h"
#endif // USE_DEFAULT_USER_CFG

/* DMM Includes */
#include <dmm_scheduler.h>
#include <dmm_policy.h>
#include <ti_dmm_application_policy.h>
#include <dmm_priority_ble_zigbee_zed.h>

// Buttons controlled via BLE application
#undef ZB_USE_BUTTONS
/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

/* BLE */
#ifndef USE_DEFAULT_USER_CFG
// BLE user defined configuration
icall_userCfg_t user0Cfg = BLE_USER_CFG;
#endif // USE_DEFAULT_USER_CFG

/* Zigbee */
static zb_bool_t gs_platform_init_done = ZB_FALSE;
static volatile zb_uint8_t count = 0;
static uintptr_t hwiKey;
extern SemaphoreP_Handle wakeSem;
extern RCL_Client ZRCL_client;

/*******************************************************************************
 * EXTERNS
 */
extern void appMain(void);
extern void AssertHandler(uint8 assertCause, uint8 assertSubcause);

void *main_task_function(void *arg0)
{
  ZVUNUSED(arg0);

  Log_printf(LogModule_Zigbee, Log_DEBUG, "initialized");

  zb_ti_f3_main();

  return NULL;
}

int main(void)
{
  pthread_t           thread;
  pthread_attr_t      attrs;
  struct sched_param  priParam;
  int                 retc;
  int                 detachState;

  /* Call driver init functions */
  Board_init();

  /* Initialize DMM Scheduler */
  DMMPolicy_Params dmmPolicyParams;
  DMMPolicy_Status policyRet;

  /* Initialize RCL */
  DMMSch_RCL_init();

  /* Initial Policy Manager for tests that require priority handling */
  DMMPolicy_init();
  DMMPolicy_Params_init(&dmmPolicyParams);
  dmmPolicyParams.numPolicyTableEntries = DMMPolicy_ApplicationPolicySize;
  dmmPolicyParams.policyTable = DMMPolicy_ApplicationPolicyTable;
  dmmPolicyParams.globalPriorityTable = globalPriorityTable_bleZigbeeZed;
  policyRet = DMMPolicy_open(&dmmPolicyParams);

  /* Initialize DMM scheduler */
  DMMSch_init();

  /* Zigbee Initialization */
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

  /* BLE Initialization */

  /* Register Application callback to trap asserts raised in the Stack */
  halAssertCback = AssertHandler;
  RegisterAssertCback(AssertHandler);

  /* Update User Configuration of the stack */
  user0Cfg.appServiceInfo->timerTickPeriod = ICall_getTickPeriod();
  user0Cfg.appServiceInfo->timerMaxMillisecond  = ICall_getMaxMSecs();

  DMMSch_registerClient(&ZRCL_client, DMMPolicy_StackRole_ZigbeeEndDevice, DMMPolicy_Id_Zigbee);

  /* Initialize BLE application task */
  appMain();

  /* Start the FreeRTOS scheduler */
  vTaskStartScheduler(); /* does not return */

  return (0);
}

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

  // When device is a sleepy ZED, ensure that RX is off when going to sleep.
  if (MAC_PIB().mac_rx_on_when_idle == 0U)
  {
    ZB_TRANSCEIVER_SET_RX_ON_OFF(ZB_FALSE);
  }

  /* zzzzZZZZZzzzzzz */
  SemaphoreP_Status semStatus = SemaphoreP_TIMEOUT;
  if(NULL != wakeSem)
  {
    semStatus = SemaphoreP_pend(wakeSem, sleep_ticks);
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


//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    AssertHandler(HAL_ASSERT_CAUSE_STACK_OVERFLOW_ERROR, 0);
}

/*******************************************************************************
 * @fn          AssertHandler
 *
 * @brief       This is the Application's callback handler for asserts raised
 *              in the stack.  When EXT_HAL_ASSERT is defined in the Stack Wrapper
 *              project this function will be called when an assert is raised,
 *              and can be used to observe or trap a violation from expected
 *              behavior.
 *
 *              As an example, for Heap allocation failures the Stack will raise
 *              HAL_ASSERT_CAUSE_OUT_OF_MEMORY as the assertCause and
 *              HAL_ASSERT_SUBCAUSE_NONE as the assertSubcause.  An application
 *              developer could trap any malloc failure on the stack by calling
 *              HAL_ASSERT_SPINLOCK under the matching case.
 *
 *              An application developer is encouraged to extend this function
 *              for use by their own application.  To do this, add assert.c
 *              to your project workspace, the path to assert.h (this can
 *              be found on the stack side). Asserts are raised by including
 *              assert.h and using macro HAL_ASSERT(cause) to raise an
 *              assert with argument assertCause.  the assertSubcause may be
 *              optionally set by macro HAL_ASSERT_SET_SUBCAUSE(subCause) prior
 *              to asserting the cause it describes. More information is
 *              available in assert.h.
 *
 * input parameters
 *
 * @param       assertCause    - Assert cause as defined in assert.h.
 * @param       assertSubcause - Optional assert subcause (see assert.h).
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void AssertHandler(uint8_t assertCause, uint8_t assertSubcause)
{
    // check the assert cause
    switch(assertCause)
    {
        case HAL_ASSERT_CAUSE_OUT_OF_MEMORY:
        {
            // ERROR: OUT OF MEMORY
            HAL_ASSERT_SPINLOCK;
            break;
        }

        case HAL_ASSERT_CAUSE_INTERNAL_ERROR:
        {
            // check the subcause
            if(assertSubcause == HAL_ASSERT_SUBCAUSE_FW_INERNAL_ERROR)
            {
                // ERROR: INTERNAL FW ERROR
                HAL_ASSERT_SPINLOCK;
            }
            else
            {
                // ERROR: INTERNAL ERROR
                HAL_ASSERT_SPINLOCK;
            }
            break;
        }

        case HAL_ASSERT_CAUSE_ICALL_ABORT:
        {
            HAL_ASSERT_SPINLOCK;
            break;
        }

        case HAL_ASSERT_CAUSE_ICALL_TIMEOUT:
        {
            HAL_ASSERT_SPINLOCK;
            break;
        }

        case HAL_ASSERT_CAUSE_WRONG_API_CALL:
        {
            HAL_ASSERT_SPINLOCK;
            break;
        }

        case HAL_ASSERT_CAUSE_STACK_OVERFLOW_ERROR:
        {
            HAL_ASSERT_SPINLOCK;
            break;
        }

        case HAL_ASSERT_CAUSE_LL_INIT_RNG_NOISE_FAILURE:
        {
            /*
             * Device must be reset to recover from this case.
             *
             * The HAL_ASSERT_SPINLOCK with is replacable with custom handling,
             * at the end of which Power_reset(); MUST be called.
             *
             * BLE5-stack functionality will be compromised when LL_initRNGNoise
             * fails.
             */
            HAL_ASSERT_SPINLOCK;
            break;
        }

        default:
        {
            HAL_ASSERT_SPINLOCK;
            break;
        }
    }

    return;
}

/*******************************************************************************
 */
