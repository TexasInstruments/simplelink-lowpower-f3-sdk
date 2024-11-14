/* ZBOSS Zigbee software protocol stack
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 * This is unpublished proprietary source code of DSR Corporation
 * The copyright notice does not evidence any actual or intended
 * publication of such source code.
 *
 * ZBOSS is a registered trademark of Data Storage Research LLC d/b/a DSR
 * Corporation
 *
 * Commercial Usage
 * Licensees holding valid DSR Commercial licenses may use
 * this file in accordance with the DSR Commercial License
 * Agreement provided with the Software or, alternatively, in accordance
 * with the terms contained in a written agreement between you and
 * DSR.
 */
/* PURPOSE: Common definitions for leds and buttons functionality
*/

#ifndef ZB_LED_H
#define ZB_LED_H 1

#include "zb_vendor.h"
#include "zboss_api_core.h"

/** @cond DOXYGEN_LEDS_BUTTONS_SECTION */

/*! @addtogroup leds_buttons */
/*! @{ */

/**
 *  @name LED
 *  @{
 */

#ifdef ZB_DEPRECATED_API
/* This function is not used anywhere and seems its objective is not different from
 * 'zb_osif_led_button_init()' which is currently used. It will be removed in a future
 * release. */
/**
 * Prepare LEDs for usage
 */
void zb_led_init(void) ZB_DEPRECATED;
#endif /* ZB_DEPRECATED_API */

/**
 * Stop blinking on given LED
 *
 * See ZB_LED_ARG_CREATE() for constructing led_arg
 */
void zb_led_blink_off(zb_uint8_t led_arg);

/**
 * Start blinking on given LED
 *
 * See ZB_LED_ARG_CREATE() for constructing led_arg
 */
void zb_led_blink_on(zb_uint8_t led_arg);

/**
 * Quant size for intervals passed to ZBOSS LED functions.
 *
 * See ZB_LED_ARG_CREATE()
 */
#define ZB_LED_PERIOD_MULTIPLIER ZB_MILLISECONDS_TO_BEACON_INTERVAL(250U)

/**
 * Conveniency constants to be used with ZB_LED_ARG_CREATE() macro
 */
typedef enum zb_led_blink_frequency_e
{
  ZB_LED_BLINK_QUATER_SEC = 1, /*!< Blink four times per second */
  ZB_LED_BLINK_HALF_SEC = 2,   /*!< Blink twice per second */
  ZB_LED_BLINK_PER_SEC = 4,    /*!< Blink every second */
  ZB_LED_BLINK_PER_2SEC = 8,   /*!< Blink every two seconds */
} zb_led_blink_frequency_t;

/**
 * @brief get period argument value (in beacon intervals) from led_arg.
 * To be used in ZBOSS led functions
 */
#define ZB_LED_ARG_PERIOD(led_arg) (((led_arg) >> 4U) * ZB_LED_PERIOD_MULTIPLIER)

/**
 * @brief get LED number from led_arg
 * To be used in ZBOSS led functions
 */
#define ZB_LED_ARG_NUMBER(led_arg) ((led_arg) & 0xfU)

/**
 * @brief Construct argument to pass to LEd functions line zb_led_blink_on()
 * @param num - number of LED for which command is meant
 * @param period - interval between LED state changes.
 *                 see @ref ZB_LED_PERIOD_MULTIPLIER for quant size.
 * @note period should not exceed value of 16
 */
#define ZB_LED_ARG_CREATE(num, period) ((zb_uint8_t)((num) | (((zb_uint8_t)(period)) << 4U)))

/** @} */

/**
 *  @name Buttons
 *  @{
 */

/**
 * @brief Set state of given button to on
 *
 * @param butt_no - number of button being pressed
 */
void zb_button_on_cb(zb_uint8_t butt_no);

/**
 * @brief Set state of given button to off and invoke button handler.
 *
 * Handler to invoke is chosen by button and duration of button press.
 * See @ref zb_button_register_handler() for how to setup button handlers
 *
 * @param butt_no - number of button being released
 */
void zb_button_off_cb(zb_uint8_t butt_no);

/**
 * @brief Register handler to be called on button release
 *
 * Several handlers can be set for one button (see ZB_BUTT_N_CBS for maximum number of callbacks)
 * Each handler is associated with handler press duration, and only one will be called,
 * it depends on how long button was pressed which one.
 *
 * @param butt_no - number of button with which handler is associated
 * @param pressed_sec_pow2 - duration with which handler is associated.
 *                           calculated as given power of 2 (in sec): 1-2-4-8... sec
 * @param cb - pointer to handler functions
 */
void zb_button_register_handler(zb_uint8_t butt_no, zb_uint8_t pressed_sec_pow2, zb_callback_t cb);


/* Init leds and buttons
 *
 */
void zb_osif_led_button_init(void);

/* Set a LED on
 * It must be initialized first.
 *
 * @param led_no - id of the LED in board (check platform specific headers to identify LED)
 */
void zb_osif_led_on(zb_uint8_t led_no);

/* Set a LED off
 * It must be initialized first.
 *
 * @param led_no - id of the LED in board (check platform specific headers to identify LED)
 */
void zb_osif_led_off(zb_uint8_t led_no);

/* Invert a LED off
 * It must be initialized first.
 *
 * @param led_no - id of the LED in board (check platform specific headers to identify LED)
 */
void zb_osif_led_toggle(zb_uint8_t led_no);


/* Get the current button tate
 * It must be initialized first.
 *
 * @param arg - id of the LED in board (check platform specific headers to identify LED)
 *
 * @return ZB_FALSE is button is pressed, ZB_TRUE if button is not being pressed
 */
zb_bool_t zb_osif_button_state(zb_uint8_t arg);

/* Inform osif layer that button callback is being set
 * @return ZB_TRUE if leds and buttons are not initialized yet
 *  with zb_osif_led_button_init(). In this case stack logic
 *  should call it explicitly.
 */
zb_bool_t zb_setup_buttons_cb(zb_callback_t cb);

void zb_osif_button_cb(zb_uint8_t arg);


/* Button indexes utilized by applications
 * note: more can be used/defined depending on target HW
 */
#define ZB_BOARD_BUTTON_0 0U
#define ZB_BOARD_BUTTON_1 1U
#define ZB_BOARD_BUTTON_2 2U
#define ZB_BOARD_BUTTON_3 3U

/**
 * Number of handler that can be associated with a button.
 *
 * Each of those handlers corresponds to its own press duration
 */
#define ZB_BUTT_N_CBS 5U

#ifndef ZB_N_BUTTONS

/**
 * Number of button utilized by application
 */
#define ZB_N_BUTTONS 1U

#endif

#ifndef ZB_N_LEDS

/**
 * Number of leds utilized by application
 */
#define ZB_N_LEDS 1U

#endif

typedef struct zb_button_ctl_s
{
  zb_bool_t is_on;
  zb_time_t on_time;
  zb_callback_t handlers[ZB_BUTT_N_CBS];
} zb_button_ctl_t;


typedef struct zb_buttons_global_s
{
  zb_button_ctl_t buttons[ZB_N_BUTTONS];
} zb_buttons_global_t;

/*! @} */

/*! @} */

/** @endcond */ /*DOXYGEN_LEDS_BUTTONS_SECTION */

#endif /* ZB_LED_H */
