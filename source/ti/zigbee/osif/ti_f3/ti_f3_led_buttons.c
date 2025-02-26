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

#define ZB_TRACE_FILE_ID 30001

#include <stddef.h>

#include "zb_common.h"

#include <ti/drivers/GPIO.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#include "ti_drivers_config.h"

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/cpu.h)

/* Convenience macro to convert milliseconds into CPU delay cycles */
#define CPU_convertMsToDelayCycles(milliseconds) \
    (((uint32_t)(milliseconds)) * (48000 / 3))

static zb_uint8_t gs_inited = ZB_FALSE;
static zb_uint8_t s_btn_max_num = 0;
static zb_bool_t s_btn_debounce = ZB_FALSE;
SemaphoreP_Handle buttonSem = NULL;

/***** Function definitions *****/
/* GPIO interrupt Callback function for CONFIG_GPIO_BTN1 and CONFIG_GPIO_BTN2. */
void button_callback(zb_uint8_t index)
{
  /* This function is called only while waiting for button event and
     applied in battery-power device. GPD design assumes that debouncing
     is needed only for battery-powered device, because batteryless devices
     hae buttons pressed right on startup (i.e. power-enabled) time.

     Actual buttons reading is performed by application when
     gboss_test_get_button_mask() function is called. */

  /* Simple debounce logic controlled by core logic */
  if (s_btn_debounce)
  {
    /* ~15ms sync delay */
    CPUDelay(CPU_convertMsToDelayCycles(15));
  }

  // Signal a button event just occurred
  SemaphoreP_post(buttonSem);

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
  zb_bool_t state;
  zb_uint8_t button_no;

  state = (GPIO_read(index) == 0U);
  if (index == (zb_uint8_t)CONFIG_GPIO_BTN1)
  {
    button_no = 0;
  }
  else
  {
    ZB_ASSERT(index == (zb_uint8_t)CONFIG_GPIO_BTN2);
    button_no = 1;
  }

  if (state)
  {
    zb_button_on_cb(button_no);
  }
  else
  {
    zb_button_off_cb(button_no);
  }
#endif //defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
}

static void ti_f3_buttons_enable_irq(void)
{
  GPIO_enableInt(CONFIG_GPIO_BTN1);
  GPIO_enableInt(CONFIG_GPIO_BTN2);
}

static void ti_f3_buttons_disable_irq(void)
{
  GPIO_disableInt(CONFIG_GPIO_BTN1);
  GPIO_disableInt(CONFIG_GPIO_BTN2);
}

void ti_f3_buttons_init(void)
{
  if(NULL == buttonSem)
  {
    SemaphoreP_Params semParams;
    SemaphoreP_Params_init(&semParams);
    buttonSem = SemaphoreP_create(0,&semParams);

#if defined ZB_ZGPD_ROLE
    GPIO_setConfig(CONFIG_GPIO_BTN1, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setConfig(CONFIG_GPIO_BTN2, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
#else //defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
    GPIO_setConfig(CONFIG_GPIO_BTN1, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_BOTH_EDGES);
    GPIO_setConfig(CONFIG_GPIO_BTN2, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_BOTH_EDGES);
    ti_f3_buttons_enable_irq();
#endif // ZB_ZGPD_ROLE

    /* Install Button callback */
    GPIO_setCallback(CONFIG_GPIO_BTN1, button_callback);
    GPIO_setCallback(CONFIG_GPIO_BTN2, button_callback);
  } // ELSE: already initialized
}

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
void ti_f3_leds_init(void)
{
  /* Configure the LED pin and turn it off */
  GPIO_setConfig(CONFIG_GPIO_GLED, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
  GPIO_setConfig(CONFIG_GPIO_RLED, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
  GPIO_write(CONFIG_GPIO_GLED, CONFIG_GPIO_LED_OFF);
  GPIO_write(CONFIG_GPIO_RLED, CONFIG_GPIO_LED_OFF);
}

void zb_osif_led_button_init(void)
{

  if (!gs_inited)
  {
    ti_f3_buttons_init();
    ti_f3_leds_init();

    gs_inited = ZB_TRUE;
  }
}

void zb_osif_led_on(zb_uint8_t led_no)
{
  switch(led_no)
  {
    case 0U:
      GPIO_write(CONFIG_GPIO_GLED, CONFIG_GPIO_LED_ON);
      break;
    case 1U:
      GPIO_write(CONFIG_GPIO_RLED, CONFIG_GPIO_LED_ON);
      break;
    default:
      /*Nothing to do*/
      break;
  }
}

void zb_osif_led_off(zb_uint8_t led_no)
{
  switch(led_no)
  {
    case 0U:
      GPIO_write(CONFIG_GPIO_GLED, CONFIG_GPIO_LED_OFF);
      break;
    case 1U:
      GPIO_write(CONFIG_GPIO_RLED, CONFIG_GPIO_LED_OFF);
      break;
    default:
      /*Nothing to do*/
      break;
  }
}

void zb_osif_button_cb(zb_uint8_t arg)
{
  /* Implemented via interrupts */
  ZVUNUSED(arg);
}

zb_bool_t zb_osif_button_state(zb_uint8_t arg)
{
  zb_bool_t state;

  /* Pressed = 0
   * Released = 1
   */
  if (arg == 0U)
  {
    state = (GPIO_read(CONFIG_GPIO_BTN1) == 0U);
  }
  else
  {
    ZB_ASSERT(arg == 1U);
    state = (GPIO_read(CONFIG_GPIO_BTN2) == 0U);
  }

  return state;
}

zb_bool_t zb_setup_buttons_cb(zb_callback_t cb)
{
  ZVUNUSED(cb);
  return(!gs_inited);
}
#endif //defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE

#ifdef ZB_ZGPD_ROLE
#include <zb_nvm.h>
zb_uint32_t gboss_test_get_button_mask(void)
{
  zb_uint32_t ret_state = 0;
  zb_bool_t btn1 = ZB_FALSE;
  zb_bool_t btn2 = ZB_FALSE;
  zb_uint8_t offset = 0;

  /* It is possible that function returns multiple buttons press.
     Application logic will decide how to process them. */

  /* Commissioning button */
  btn1 = (zb_bool_t)!GPIO_read(CONFIG_GPIO_BTN1);
  ret_state |= btn1;

  /* Application button */
  btn2 = (zb_bool_t)!GPIO_read(CONFIG_GPIO_BTN2);
  if (s_btn_max_num == 0 || s_btn_max_num == 1 || s_btn_max_num == 2)
  {
    ret_state |= btn2 << 1u;
  }
  else
  {
    /* If user specified maximum number of buttons, each application
       button press emulates the next button using round robin algorithm.
       Button 0 is always reserved for commissioning purpose and emulation
       starts with button 1 */
    offset = zgpd_lookup_seq_number() % (s_btn_max_num - 1) + 1;
    ret_state |= btn2 << offset;
  }

  return ret_state;
}

void gboss_wait_for_button(void)
{
  ti_f3_buttons_enable_irq();
  /* zgpd_wait_for_event() is platform-specific function */
  zgpd_wait_for_event();
  ti_f3_buttons_disable_irq();
}

void gboss_set_buttons_max_num(zb_uint8_t num)
{
  s_btn_max_num = num;
}

void zgpd_button_enable_debounce(void)
{
  s_btn_debounce = ZB_TRUE;
}

void zgpd_button_disable_debounce(void)
{
  s_btn_debounce = ZB_FALSE;
}
#endif //ZB_ZGPD_ROLE

