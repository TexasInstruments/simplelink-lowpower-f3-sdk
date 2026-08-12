/*
 * Copyright (c) 2023-2024, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!*****************************************************************************
 *  @file       LGPTimerLPF3.h
 *  @brief      LGPTimer driver implementation for Low Power F3 devices
 *
 *  @anchor ti_drivers_LGPTimer_Overview
 *  # Overview #
 *  The LGPTimer driver allows you to measure elapsed time with simple and
 *  portable APIs. It also allows for asynchronous callbacks after a
 *  certain amount of time has elapsed.
 *  In addition the driver supports APIs for both capture and compare of
 *  of IO signals muxable to the three channels of each LGPT peripheral
 *  instance. The channel capture functionality can be used to measure period
 *  and duty cycle of an input signal. The channel compare functionality can
 *  be used for generating PWM signals.
 *
 *  The LGPTimer driver also handles the general purpose timer resource allocation.
 *  For each driver that requires use of a general purpose timer, it calls
 *  #LGPTimerLPF3_open() to occupy the specified timer, and calls
 *  #LGPTimerLPF3_close() to release the occupied timer resource.
 *
 *  @anchor ti_drivers_LGPTimer_PeripheralProperties
 *  # LGPT peripheral properties #
 *  The LGPT peripherals support different timer counter widths depending
 *  on the peripheral instance number (LGPT0, LGPT1, ..). The table below
 *  lists supported counter widths for each peripheral instance number on
 *  available device types.
 *  The timer counter clock is sourced from the internal prescaler stage which
 *  has the high frequency clock (CLKSVT) as input.
 *  The prescaler can be configured to divide the input system clock, effectively
 *  extending the maximal time interval for the timer counter while reducing the
 *  timer resolution.
 *
 * | Device type   | LGPT0   | LGPT1   | LGPT2   | LGPT3   |
 * |---------------|---------|---------|---------|---------|
 * | CC2340R2      | 16 bits | 16 bits |   N.A   |   N.A   |
 * | CC2340R5      | 16 bits | 16 bits | 16 bits | 24 bits |
 * | CC27XX        | 16 bits | 16 bits | 16 bits | 32 bits |
 *
 *  @anchor ti_drivers_LGPTimer_CounterModes
 *  # Counter Modes #
 *  The LGPTimer driver supports the following timer counter modes:
 *    - Oneshot mode counting upwards. When the counter reaches the target value, the
 *      timer is stopped automatically and the TGT interrupt status flag
 *      (#LGPTimerLPF3_INT_TGT) is set.
 *    - Periodic mode counting upwards. When the counter reaches target value it wraps
 *      and restarts counting from 0 again. The TGT interrupt status flag is set each
 *      time the target value is reached. The ZERO interrupt status flag
 *      (#LGPTimerLPF3_INT_ZERO) is set each time the counter restarts from 0.
 *    - Periodic mode counting upwards/downwards. When the counter reaches target value
 *      while counting upwards the TGT interrupt flag is set and it starts counting
 *      downwards to 0. When 0 is reached the ZERO interrupt status flag is set
 *      and the sequence is repeated.
 *
 *  @anchor ti_drivers_LGPTimer_UnsupportedFeatures
 *  # LGPT peripheral features not supported by the LGPTimer driver #
 *  - Channel input from the event fabric is not supported
 *  - Channel output dead band control present in LGPT1 only, is not supported
 *  - Use of channel input filter is not supported
 *  - DMA usage is not supported
 *  - ADC trigger is not supported
 *  - QDEC mode where the timer functions as a quadrature decoder is not supported
 *  - Timer synchronization is not supported
 *  - IR generation is not supported
 *  - Brushless DC motor control is not supported
 *  - Fault and Park is not supported
 *  - Prescaler counter clock sourced from events is not supported
 *
 *  @anchor ti_drivers_LGPTimer_PowerManagement
 *  # Power Management #
 *  The power management framework will try to put the device into the most
 *  power efficient mode whenever possible. Please see the technical reference
 *  manual for further details on each power mode.
 *
 *  The LGPTimer driver will set constraints on disallowed power modes when
 *  needed, removing the need for the application to handle this.
 *  The following statements are valid:
 *    - After #LGPTimerLPF3_open():
 *        The device is still allowed to enter Standby. When the device is
 *        active, the corresponding LGPT peripheral will be enabled and clocked.
 *        Note that a configured counter target and configured channels, will not
 *        be retained when going in and out of Standby.
 *        The counter target is set by the functions
 *        #LGPTimerLPF3_setInitialCounterTarget() and #LGPTimerLPF3_setNextCounterTarget().
 *    - After #LGPTimerLPF3_start():
 *        The device will only go to Idle power mode since the high frequency
 *        clock (CLKSVT) is needed for timer operation.
 *    - After #LGPTimerLPF3_stop():
 *        The device is still allowed to enter Standby. When the device is
 *        active, the corresponding LGPT peripheral will be enabled and clocked.
 *    - After #LGPTimerLPF3_close():
 *        The underlying LGPT peripheral is turned off. The device is allowed to
 *        enter Standby.
 *
 *  @anchor ti_drivers_LGPTimer_Accuracy
 *  # Accuracy #
 *  The timer clock of the LGPT peripheral is dependent on the high frequency clock (CLKSVT).
 *
 *  @anchor ti_drivers_LGPTimer_Usage
 *  # Usage #
 *
 *  This documentation provides some basic @ref ti_drivers_LGPTimer_Examples "examples"
 *  in the form of commented code fragments.
 *
 *
 *  <hr>
 *  @anchor ti_drivers_LGPTimer_Examples
 *  # Examples
 *
 *  @note
 *  <b>The following examples are intended for reference only and are not
 *  intended for application use.</b>
 *
 *  @li @ref ti_drivers_LGPTimer_Example_periodic "Periodic timer"
 *  @li @ref ti_drivers_LGPTimer_Example_output "Output signal generation"
 *
 *
 *  @anchor ti_drivers_LGPTimer_Example_periodic
 *  ## Periodic timer example##
 *  The code example below will generate an interrupt using the LGPTimer every 1 ms.
 *  Note that when a count-up counter mode is used, the number of counter ticks to
 *  reach the target value equals target value + 1.
 *
 *  @anchor ti_drivers_LGPTimer_Code
 *  @code
 *  #include <ti/drivers/timer/LGPTimerLPF3.h>
 *
 *  void timerCallback(LGPTimerLPF3_Handle lgptHandle, LGPTimerLPF3_IntMask interruptMask) {
 *      // interrupt callback code goes here. Minimize processing in interrupt.
 *  }
 *
 *  void* taskFxn(void* arg) {
 *
 *    LGPTimerLPF3_Handle lgptHandle;
 *    LGPTimerLPF3_Params params;
 *    uint32_t counterTarget;
 *
 *    // Initialize parameters and assign callback function to be used
 *    LGPTimerLPF3_Params_init(&params);
 *    params.hwiCallbackFxn = timerCallback;
 *
 *    // Open driver
 *    lgptHandle = LGPTimerLPF3_open(0, &params);
 *
 *    // Set counter target
 *    counterTarget = 48000 - 1;  // 1 ms with a system clock of 48 MHz
 *    LGPTimerLPF3_setInitialCounterTarget(lgptHandle, counterTarget, true);
 *
 *    // Enable counter target interrupt
 *    LGPTimerLPF3_enableInterrupt(lgptHandle, LGPTimerLPF3_INT_TGT);
 *
 *    // Start counter in count-up-periodic mode
 *    LGPTimerLPF3_start(lgptHandle, LGPTimerLPF3_CTL_MODE_UP_PER);
 *
 *    // Generate counter target interrupt every 1 ms forever
 *    while(1);
 *  }
 *  @endcode
 *
 *
 *  @anchor ti_drivers_LGPTimer_Example_output
 *  ## Output signal generation example##
 *  The code example below will generate an output signal of 32 kHz with a 50 % duty cycle
 *  on channel 2.
 *  With an up/down counter mode, the counter target value determines the signal period and
 *  the value must be set to half the number of the total counter ticks per signal period.
 *  With a channel action of toggle-on-compare, the channel compare value must be set to
 *  (counter target value)/2 in order to obtain a 50 % duty cycle of the output signal.
 *  The period of a 32 kHz signal equals 1500 counter ticks when the counter has a 48 MHz clock.
 *
 *
 *  @code
 *  #include <ti/drivers/timer/LGPTimerLPF3.h>
 *
 *  void* taskFxn(void* arg) {
 *
 *    LGPTimerLPF3_Handle lgptHandle;
 *    LGPTimerLPF3_Params params;
 *    uint32_t cntTargetVal = 1500/2;
 *    uint32_t chCompVal    = cntTargetVal/2;
 *
 *    // Configure channel 2 action
 *    LGPTimerLPF3_Params_init(&params);
 *    params.channelProperty[2].action = LGPTimerLPF3_CH_TOGGLE_ON_COMPARE_PERIODIC;
 *
 *    // Open driver
 *    lgptHandle = LGPTimerLPF3_open(0, &params);
 *
 *    // Set channel output signal period
 *    LGPTimerLPF3_setInitialCounterTarget(lgptHandle, cntTargetVal, false);
 *
 *    // Set channel output signal duty cycle
 *    LGPTimerLPF3_setInitialChannelCompVal(lgptHandle, LGPTimerLPF3_CH_NO_2, chCompVal, false);
 *
 *    // Start the LGPTimer in up-down-periodic mode
 *    LGPTimerLPF3_start(lgptHandle, LGPTimerLPF3_CTL_MODE_UPDWN_PER);
 *
 *    // Output signal forever
 *    while(1);
 *  }
 *  @endcode
 *
 *
 *  ## Opening the LGPTimerLPF3 Driver #
 *
 *  Opening a LGPTimerLPF3 requires four steps:
 *  1.  Create and initialize a #LGPTimerLPF3_Params structure.
 *  2.  Set non-default parameter values.
 *  3.  Call #LGPTimerLPF3_open(), passing the index of the timer in the
 *      #LGPTimerLPF3_Config structure, and the address of the #LGPTimerLPF3_Params
 *      structure. The timer instance is specified by the index in the
 *      #LGPTimerLPF3_Config structure.
 *  4.  Verify that the timer handle returned by #LGPTimerLPF3_open() is non-NULL.
 *      The handle will be used to operate the timer driver instance you just opened.
 *
 *  <hr>
 *  @anchor ti_drivers_LGPT_Configuration
 *  # Configuration
 *
 *  Refer to the @ref driver_configuration "Driver's Configuration" section
 *  for driver configuration information.
 *  <hr>
 *
 *  ============================================================================
 */

#ifndef ti_drivers_timer_LGPTimerLPF3__include
#define ti_drivers_timer_LGPTimerLPF3__include

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/GPIO.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_lgpt.h)

#ifdef __cplusplus
extern "C" {
#endif

/* Define for number of channels on the LPGPT peripheral */
#define NO_OF_LGPT_CHANNELS 3

/*!
 *  @brief
 *  Definitions for supported LGPTimer channel numbers.
 *
 */
typedef enum
{
    LGPTimerLPF3_CH_NO_0 = 0,
    LGPTimerLPF3_CH_NO_1 = 1,
    LGPTimerLPF3_CH_NO_2 = 2,
} LGPTimerLPF3_ChannelNo;

/*!
 *  @brief
 *  Definitions for supported LGPTimer channel direction.
 *
 */
typedef enum
{
    LGPTimerLPF3_CH_DIR_NONE   = 0,
    LGPTimerLPF3_CH_DIR_INPUT  = 1,
    LGPTimerLPF3_CH_DIR_OUTPUT = 2,
} LGPTimerLPF3_ChannelDir;

/*!
 *  @brief
 *  Definitions for supported LGPTimer channel output levels.
 *
 */
typedef enum
{
    LGPTimerLPF3_CH_LEVEL_LOW  = LGPT_OUTCTL_CLROUT0,
    LGPTimerLPF3_CH_LEVEL_HIGH = LGPT_OUTCTL_SETOUT0,
} LGPTimerLPF3_ChannelLevel;

/*!
 *  @brief
 *  Definitions for supported LGPTimer channel input edge.
 *
 */
typedef enum
{
    LGPTimerLPF3_CH_EDGE_NONE = LGPT_C0CFG_EDGE_NONE,
    LGPTimerLPF3_CH_EDGE_RISE = LGPT_C0CFG_EDGE_RISE,
    LGPTimerLPF3_CH_EDGE_FALL = LGPT_C0CFG_EDGE_FALL,
    LGPTimerLPF3_CH_EDGE_BOTH = LGPT_C0CFG_EDGE_BOTH,
} LGPTimerLPF3_ChannelInputEdge;

/*!
 *  @brief
 *  Definitions for supported LGPTimer counting modes.
 *  @cond NODOC
 *  New modes must not be added to the below description without
 *  also updating the driver.
 *  @endcond
 *
 */
typedef enum
{
    /*! Disable timer. Updates to counter and events are stopped. */
    LGPTimerLPF3_CTL_MODE_DIS       = LGPT_CTL_MODE_DIS,
    /*! Count up one-shot. The timer increments from 0 to target value, then stops and
     *  sets MODE to LGPTimerLPF3_CTL_MODE_DIS.
     */
    LGPTimerLPF3_CTL_MODE_UP_ONCE   = LGPT_CTL_MODE_UP_ONCE,
    /*! Count up periodically. The timer increments from 0 to target value, repeatedly. */
    LGPTimerLPF3_CTL_MODE_UP_PER    = LGPT_CTL_MODE_UP_PER,
    /*! Count up and down periodically. The timer counts from 0 to target value and back to 0, repeatedly. */
    LGPTimerLPF3_CTL_MODE_UPDWN_PER = LGPT_CTL_MODE_UPDWN_PER,
} LGPTimerLPF3_Mode;

/*!
 *  @brief
 *  Definitions for supported LGPTimer interrupts. LGPTimerLPF3_IntMask
 *  arguments should be a bit vector containing these definitions.
 */
typedef enum
{
    /*! Timer counter target interrupt */
    LGPTimerLPF3_INT_TGT            = 1 << LGPT_RIS_TGT_S,
    /*! Timer counter zero interrupt */
    LGPTimerLPF3_INT_ZERO           = 1 << LGPT_RIS_ZERO_S,
    /*! Timer counter increment/decrement interrupt */
    LGPTimerLPF3_INT_COUNTER_CHANGE = 1 << LGPT_RIS_CNTRCHNG_S,
    /*! Timer counter direction change interrupt */
    LGPTimerLPF3_INT_DIR_CHANGE     = 1 << LGPT_RIS_DIRCHNG_S,
    /*! Channel 0 capture or compare interrupt */
    LGPTimerLPF3_INT_CH0_CC         = 1 << LGPT_RIS_C0CC_S,
    /*! Channel 1 capture or compare interrupt */
    LGPTimerLPF3_INT_CH1_CC         = 1 << LGPT_RIS_C1CC_S,
    /*! Channel 2 capture or compare interrupt */
    LGPTimerLPF3_INT_CH2_CC         = 1 << LGPT_RIS_C2CC_S,
} LGPTimerLPF3_Interrupt;

/*!
 *  @brief
 *  Definitions for controlling timer debug stall mode
 */
typedef enum
{
    /*! The LGPT is not halted when CPU is halted in debug */
    LGPTimerLPF3_DEBUG_STALL_OFF       = LGPT_EMU_HALT_DIS,
    /*! Halt LGPT immediatly when CPU is halted in debug */
    LGPTimerLPF3_DEBUG_STALL_IMMEDIATE = (LGPT_EMU_HALT_EN | LGPT_EMU_CTL_IMMEDIATE),
    /*! Halt LGPT when counter equals 0 while CPU is halted in debug */
    LGPTimerLPF3_DEBUG_STALL_ON_ZERO   = (LGPT_EMU_HALT_EN | LGPT_EMU_CTL_ZERCOND),
} LGPTimerLPF3_DebugMode;

/*!
 *  @brief
 *  Definitions for which direction the timer counter must have in order to
 *  set channel compare interrupt status flag.
 */
typedef enum
{
    /*! Channel compare interrupt status flag is set on up count and down count. */
    LGPTimerLPF3_CH_COMPARE_COUNTER_DIR_BOTH = LGPT_CTL_CMPDIR_BOTH,
    /*! Channel compare interrupt status flag is only set on up count */
    LGPTimerLPF3_CH_COMPARE_COUNTER_DIR_UP   = LGPT_CTL_CMPDIR_UP,
    /*! Channel compare interrupt status flag is only set on down count */
    LGPTimerLPF3_CH_COMPARE_COUNTER_DIR_DOWN = LGPT_CTL_CMPDIR_DOWN,
} LGPTimerLPF3_ChannelCmpDir;

/*!
 *  @brief
 *  Definitions for supported LGPTimer channel actions.
 *
 */
typedef enum
{
    /*! Channel disabled */
    LGPTimerLPF3_CH_DISABLE = LGPT_C1CFG_CCACT_DIS,

    /*************************************************************************************
     *                        Channel compare actions.                                   *
     *                                                                                   *
     * The following list of channel compare actions will force the channel to be        *
     * configured as an output.                                                          *
     ************************************************************************************/
    /*! Toggle on compare repeatedly.
     *
     * Toggle channel output when the timer counter equals the compare value set by either
     * #LGPTimerLPF3_setInitialChannelCompVal() or #LGPTimerLPF3_setNextChannelCompVal().
     */
    LGPTimerLPF3_CH_TOGGLE_ON_COMPARE_PERIODIC = LGPT_C0CFG_CCACT_TGL_ON_CMP,

    /*! Toggle on compare, and then disable channel.
     *
     * Toggle channel output when the timer counter equals the compare value set by
     * #LGPTimerLPF3_setInitialChannelCompVal().
     */
    LGPTimerLPF3_CH_TOGGLE_ON_COMPARE_ONCE = LGPT_C0CFG_CCACT_TGL_ON_CMP_DIS,

    /*! Set on compare repeatedly.
     *
     * Set channel output when the timer counter equals the compare value set by either
     * #LGPTimerLPF3_setInitialChannelCompVal() or #LGPTimerLPF3_setNextChannelCompVal().
     */
    LGPTimerLPF3_CH_SET_ON_COMPARE_PERIODIC = LGPT_C0CFG_CCACT_SET_ON_CMP,

    /*! Set on compare, and then disable channel.
     *
     * Set channel output when the timer counter equals the compare value set by
     * #LGPTimerLPF3_setInitialChannelCompVal().
     */
    LGPTimerLPF3_CH_SET_ON_COMPARE_ONCE = LGPT_C0CFG_CCACT_SET_ON_CMP_DIS,

    /*! Clear on compare repeatedly.
     *
     * Clear channel output when the timer counter equals the compare value set by either
     * #LGPTimerLPF3_setInitialChannelCompVal() or #LGPTimerLPF3_setNextChannelCompVal().
     */
    LGPTimerLPF3_CH_CLEAR_ON_COMPARE_PERIODIC = LGPT_C0CFG_CCACT_CLR_ON_CMP,

    /*! Clear on compare, and then disable channel.
     *
     * Clear channel output when the timer counter equals the compare value set by
     * #LGPTimerLPF3_setInitialChannelCompVal().
     */
    LGPTimerLPF3_CH_CLEAR_ON_COMPARE_ONCE = LGPT_C0CFG_CCACT_CLR_ON_CMP_DIS,

    /*! Set on zero, toggle on compare repeatedly.
     *
     * Set channel output when timer counter value equals zero.
     * Toggle channel output when the timer counter equals the compare value set by either
     * #LGPTimerLPF3_setInitialChannelCompVal() or #LGPTimerLPF3_setNextChannelCompVal().
     */
    LGPTimerLPF3_CH_SET_ON_0_TOGGLE_ON_CMP_PERIODIC = LGPT_C0CFG_CCACT_SET_ON_0_TGL_ON_CMP,

    /*! Set on zero, toggle on compare, and then disable channel.
     *
     * Set channel output when timer counter equals zero.
     * Toggle channel output when the timer counter equals the compare value set by
     * #LGPTimerLPF3_setInitialChannelCompVal().
     */
    LGPTimerLPF3_CH_SET_ON_0_TOGGLE_ON_COMPARE_ONCE = LGPT_C0CFG_CCACT_SET_ON_0_TGL_ON_CMP_DIS,

    /*! Clear on zero, toggle on compare repeatedly.
     *
     * Clear channel output when timer counter equals zero.
     * Toggle channel output when the timer counter equals the compare value set by either
     * #LGPTimerLPF3_setInitialChannelCompVal() or #LGPTimerLPF3_setNextChannelCompVal().
     */
    LGPTimerLPF3_CH_CLR_ON_0_TOGGLE_ON_COMPARE_PERIODIC = LGPT_C0CFG_CCACT_CLR_ON_0_TGL_ON_CMP,

    /*! Clear on zero, toggle on compare, and then disable channel.
     *
     * Clear channel output when timer counter equals zero.
     * Toggle channel output when the timer counter equals the compare value set by
     * #LGPTimerLPF3_setInitialChannelCompVal().
     */
    LGPTimerLPF3_CH_CLR_ON_0_TOGGLE_ON_COMPARE_ONCE = LGPT_C0CFG_CCACT_CLR_ON_0_TGL_ON_CMP_DIS,

    /*! Pulse on compare repeatedly.
     *
     * Pulse channel output when the timer counter equals the compare value set by either
     * #LGPTimerLPF3_setInitialChannelCompVal() or #LGPTimerLPF3_setNextChannelCompVal().
     * The channel output is high for two timer clock periods.
     */
    LGPTimerLPF3_CH_PULSE_ON_COMPARE_PERIODIC = LGPT_C0CFG_CCACT_PULSE_ON_CMP,

    /*! Pulse on compare, and then disable channel.
     *
     * Pulse channel output when the timer counter equals the compare value set by
     * #LGPTimerLPF3_setInitialChannelCompVal().
     * The channel output is high for two timer clock periods.
     */
    LGPTimerLPF3_CH_PULSE_ON_COMPARE_ONCE = LGPT_C0CFG_CCACT_PULSE_ON_CMP_DIS,

    /*************************************************************************************
     *                     Channel capture actions.                                      *
     *                                                                                   *
     * The following list of channel actions will force the channel to be configured as  *
     * an input.                                                                         *
     * The counter value will be captured and the captured value can be read by the      *
     * #LGPTimerLPF3_getChCompareVal() and #LGPTimerLPF3_getNextChCompareVal()           *
     * functions.                                                                        *
     ************************************************************************************/
    /*! Set on capture repeatedly.
     *
     * The channel number dependent interrupt status flag (#LGPTimerLPF3_INT_CH0_CC
     * for channel number #LGPTimerLPF3_CH_NO_0) will be set when the signal edge
     * selected by the ch<x>InputEdge element in the LGPTimerLPF3_Params structure,
     * is detected on the channel input signal.
     */
    LGPTimerLPF3_CH_SET_ON_CAPTURE_PERIODIC = LGPT_C0CFG_CCACT_SET_ON_CAPT,

    /*! Set on capture, and then disable channel.
     *
     * The channel number dependent interrupt status flag (#LGPTimerLPF3_INT_CH0_CC
     * for channel number #LGPTimerLPF3_CH_NO_0) will be set when the signal edge
     * selected by the ch<x>InputEdge element in the LGPTimerLPF3_Params structure,
     * is detected on the channel input signal.
     */
    LGPTimerLPF3_CH_SET_ON_CAPTURE_ONCE = LGPT_C0CFG_CCACT_SET_ON_CAPT_DIS,

    /*! Period and pulse width measurement.
     *
     * Continuously capture period and pulse width of the channel input signal relative to
     * the signal edge selected by the ch<x>InputEdge element in the LGPTimerLPF3_Params
     * structure.
     * The channel number dependent interrupt status flag (#LGPTimerLPF3_INT_CH0_CC
     * for channel number #LGPTimerLPF3_CH_NO_0) will be set when the signal
     * period and pulse width have been captured. The period and pulse width are reported
     * in numbers of counter ticks. The #LGPTimerLPF3_getChCompareVal() function returns
     * the measured period and the #LGPTimerLPF3_getNextChCompareVal() functions returns
     * the measured pulse width.
     *
     * @note
     * Note that when selecting this channel action, #LGPTimerLPF3_start() function must
     * be called with either #LGPTimerLPF3_CTL_MODE_UP_ONCE or #LGPTimerLPF3_CTL_MODE_UP_PER
     * as function argument.
     *
     * @note
     * Note that the timer counter restarts regularly when this action is used, so other
     * channel actions must be chosen with this in mind. The timer counter restarts
     * when the period of the channel input signal has been captured.
     *
     * @note
     * If multiple channels are configured with this channel action, the measurements
     * are not performed simultaneously on the channels.
     * The measurements are done in a time-interleaved manner between the channels.
     *
     * Signal property requirements for this channel action:
     * - Signal Period >= 2 * (1 + params.prescalerDiv) * high frequency clock (CLKSVT) period.
     * - Signal Period <= MAX(timer counter) * (1 + params.prescalerDiv) * high frequency clock (CLKSVT) period.
     * - Signal low and high phase >= (1 + params.prescalerDiv) * high frequency clock (CLKSVT) period.
     */
    LGPTimerLPF3_CH_PULSE_WIDTH_MEASURE = LGPT_C0CFG_CCACT_PER_PULSE_WIDTH_MEAS,
} LGPTimerLPF3_ChannelAction;

/* Forward declaration of LGPTimer configuration */
typedef struct LGPTimerLPF3_Config LGPTimerLPF3_Config;

/* LGPTimer handle is pointer to configuration structure */
typedef LGPTimerLPF3_Config *LGPTimerLPF3_Handle;

/* Interrupt bit vector. See LGPTimerLPF3_Interrupt for available interrupts */
typedef uint16_t LGPTimerLPF3_IntMask;

/*!
 *  @brief      The definition of a callback function used by the LGPTimer driver.
 *
 *  @param[in]  handle                  A LGPTimer handle
 *
 *  @param[in]  interruptMask           LGPTimer interrupt mask
 *
 */
typedef void (*LGPTimerLPF3_HwiFxn)(LGPTimerLPF3_Handle handle, LGPTimerLPF3_IntMask interruptMask);

/*!
 *  @brief LGPTimerLPF3 channel dependent properties struct.
 *
 *  LGPTimer struct used by the #LGPTimerLPF3_Params.
 *
 */
typedef struct LGPTimerLPF3_ChannelProp
{
    /*! Channel action */
    LGPTimerLPF3_ChannelAction action;
    /*! Channel input edge type required for triggering a channel action of capture type */
    LGPTimerLPF3_ChannelInputEdge inputEdge;
} LGPTimerLPF3_ChannelProp;

/*!
 *  @brief LGPTimerLPF3 Parameters
 *
 *  LGPTimer parameters are used by the #LGPTimerLPF3_open() call.
 *  Default values for these parameters are set using #LGPTimerLPF3_Params_init().
 *
 * @note
 *  The prescalerDiv parameter determines the division factor of the
 *  system clock being input to the timer counter:
 *  - 0x00: Divide by 1
 *  - 0x01: Divide by 2
 *  - ...
 *  - 0xFF: Divide by 256
 *
 * @note
 *  The intPhaseLate parameter which determines if the #LGPTimerLPF3_INT_TGT
 *  and #LGPTimerLPF3_INT_ZERO interrupt status flags will be set early or late, is
 *  specified as follows:
 *  - false: Interrupt status flags are set one system clock cycle after counter = TARGET/ZERO.
 *  - true : Interrupt status flags are set one timer clock cycle after counter = TARGET/ZERO.
 *
 *  Please note that if the value of the intPhaseLate parameter is set to false while the prescalerDiv
 *  parameter value is high and either the #LGPTimerLPF3_INT_TGT or #LGPTimerLPF3_INT_ZERO
 *  interrupts are enabled, these interrupts might occur multiple times back-to-back when the
 *  interrupts are first triggered.
 *  While the counter is active, the timer will hold the state of the counter for one clock period
 *  of the timer clock before the counter gets updated. When the timer clock frequency is
 *  configured low by a high prescalerDiv parameter value, this hold time might be longer than it
 *  takes for the interrupt service routine to clear the interrupt status. This will cause a new
 *  interrupt to be immediatly generated. In order to avoid this situation, the intPhaseLate parameter
 *  value needs to be set to true. Then the interrupt will occur one timer clock cycle after the
 *  counter has reached the TARGET/ZERO value, meaning that the described hold time is reduced to 0.
 *
 *
 *  @sa     #LGPTimerLPF3_Params_init()
 */
typedef struct LGPTimerLPF3_Params
{
    /*! Hardware interrupt callback function.
     *  This pointer is optional and is only required if #LGPTimerLPF3_enableInterrupt() is called. */
    LGPTimerLPF3_HwiFxn hwiCallbackFxn;
    /*! Interrupt phase early or late for TGT and ZERO interrupts */
    bool intPhaseLate;
    /*! Prescaler division factor */
    uint8_t prescalerDiv;
    /*! Timer debug stall mode */
    LGPTimerLPF3_DebugMode debugStallMode;
    /*! Timer counter direction for channel compare. Covers all channels */
    LGPTimerLPF3_ChannelCmpDir counterDirChCompare;
    /*! Channel action and input edge type required for triggering a channel action of capture type */
    LGPTimerLPF3_ChannelProp channelProperty[NO_OF_LGPT_CHANNELS];
} LGPTimerLPF3_Params;

/*!
 *  @brief LGPTimerLPF3 channel dependent pin configuration struct.
 *
 *  LGPTimer struct used by the #LGPTimerLPF3_HWAttrs.
 *
 */
typedef struct LGPTimerLPF3_ChannelConf
{
    /*! Channel pin */
    uint8_t pin;
    /*! Channel pin mux */
    uint8_t pinMux;
    /*! Complementary channel pin */
    uint8_t nPin;
    /*! Complementary channel pin mux */
    uint8_t nPinMux;
} LGPTimerLPF3_ChannelConf;

/*!
 *  @brief  LGPTimerLPF3 Hardware attributes
 *
 *  These fields are used by the driver to set up underlying LGPTimer
 *  driver statically. A sample structure is shown below:
 *
 *  @code
 *  // LGPTimer hardware attributes, one per supported LGPT peripheral
 *  const LGPTimerLPF3_HWAttrs LGPTimerLPF3HWAttrs[] = {
 *      {
 *           .baseAddr     = LGPT0_BASE,
 *           .intNum       = INT_LGPT0_COMB,
 *           .intPriority  = INT_PRI_LEVEL1,
 *           .powerMngrId  = PowerLPF3_PERIPH_LGPT0,
 *
 *           .channelConfig[0] = {
 *               .pin     = CONFIG_GPIO_LGPT0C0,
 *               .pinMux  = GPIO_MUX_PORTCFG_PFUNC3,
 *               .nPin    = CONFIG_GPIO_LGPT0C0N,
 *               .nPinMux = GPIO_MUX_PORTCFG_PFUNC4,
 *           },
 *           .channelConfig[1] = {
 *               .pin     = CONFIG_GPIO_LGPT0C1,
 *               .pinMux  = GPIO_MUX_PORTCFG_PFUNC3,
 *               .nPin    = CONFIG_GPIO_LGPT0C1N,
 *               .nPinMux = GPIO_MUX_PORTCFG_PFUNC2,
 *           },
 *           .channelConfig[2] = {
 *               .pin     = CONFIG_GPIO_LGPT0C2,
 *               .pinMux  = GPIO_MUX_PORTCFG_PFUNC5,
 *               .nPin    = CONFIG_GPIO_LGPT0C2N,
 *               .nPinMux = GPIO_MUX_PORTCFG_PFUNC1,
 *           },
 *      },
 *  };
 *  @endcode
 */
typedef struct LGPTimerLPF3_HWAttrs
{
    /*! LGPTimer peripheral base address */
    uint32_t baseAddr;
    /*! LGPTimer peripheral's power resource ID */
    PowerLPF3_Resource powerID;
    /*! LGPTimer peripheral channel selection for for pin and pin mux */
    LGPTimerLPF3_ChannelConf channelConfig[NO_OF_LGPT_CHANNELS];
    /*! LGPTimer peripheral interrupt vector */
    uint8_t intNum;
    /*! LGPTimer peripheral's interrupt priority */
    uint8_t intPriority;
} LGPTimerLPF3_HWAttrs;

/*!
 *  @brief  LGPTimerLPF3 Object
 *
 * These fields are used by the driver to store and modify LGPTimer
 * configuration during run-time.
 * The application must not edit any member variables of this structure.
 * Appplications should also not access member variables of this structure
 * as backwards compatibility is not guaranteed. An example structure is shown
 * below:
 * @code
 * // LGPTimer objects, one per LGPT peripheral (LGPT0, LGPT1..)
 * LGPTimerLPF3_Object LGPTimerLPF3Objects[];
 * @endcode
 */
typedef struct LGPTimerLPF3_Object
{
    HwiP_Struct hwi;                                /*!< Hardware interrupt struct */
    LGPTimerLPF3_HwiFxn hwiCallbackFxn;             /*!< Hardware interrupt callback function */
    Power_NotifyObj postNotify;                     /*!< For Standby reconfiguration */
    uint32_t arg;                                   /*!< Arbritrary Argument */
    bool isOpen;                                    /*!< Object is opened flag  */
    bool intPhaseLate;                              /*!< Interrupt phase early or late for TGT and ZERO interrupts */
    uint8_t prescalerDiv;                           /*!< Prescaler division factor */
    LGPTimerLPF3_DebugMode debugStallMode;          /*!< Timer debug stall mode */
    LGPTimerLPF3_ChannelCmpDir counterDirChCompare; /*!< Counter direction required for triggering a channel compare */
    LGPTimerLPF3_ChannelProp channelProperty[NO_OF_LGPT_CHANNELS]; /*!< Channel dependent properties */
} LGPTimerLPF3_Object;

/*!
 *  @brief  LGPTimer Global configuration
 *
 *  The #LGPTimerLPF3_Config structure contains a set of pointers used to
 *  characterize the LGPTimer driver implementation. An example is shown below:
 *  @code
 *  // LGPTimer configuration (used as LGPTimer_Handle by driver and application)
 *  const LGPTimerLPF3_Config LGPTimerLPF3_config[] = {
 *      { &LGPTimerLPF3Objects[0], &LGPTimerLPF3HWAttrs[0]},
 *      { &LGPTimerLPF3Objects[1], &LGPTimerLPF3HWAttrs[1]},
 *      { &LGPTimerLPF3Objects[2], &LGPTimerLPF3HWAttrs[2]},
 *      { &LGPTimerLPF3Objects[3], &LGPTimerLPF3HWAttrs[3]},
 *  };
 *  @endcode
 */
struct LGPTimerLPF3_Config
{
    LGPTimerLPF3_Object *object;
    const LGPTimerLPF3_HWAttrs *hwAttrs;
};

/*!
 *  @brief  Function that initializes the LGPTimerLPF3_Params struct to
 *          its default values.
 *
 *  @param[in]  params  An pointer to LGPTimerLPF3_Params structure for initialization
 *
 *  Default values:
 * | Parameter              | Default value                             |
 * |------------------------|-------------------------------------------|
 * | Interrupt callback fxn | \c NULL                                   |
 * | Interrupt phase late   | \c true                                   |
 * | Prescaler division     | \c 0                                      |
 * | Timer debug stall mode | #LGPTimerLPF3_DEBUG_STALL_OFF             |
 * | Counter dir ch cmp     | #LGPTimerLPF3_CH_COMPARE_COUNTER_DIR_BOTH |
 * | Channel 0 action       | #LGPTimerLPF3_CH_DISABLE                  |
 * | Channel 0 input edge   | #LGPTimerLPF3_CH_EDGE_NONE                |
 * | Channel 1 action       | #LGPTimerLPF3_CH_DISABLE                  |
 * | Channel 1 input edge   | #LGPTimerLPF3_CH_EDGE_NONE                |
 * | Channel 2 action       | #LGPTimerLPF3_CH_DISABLE                  |
 * | Channel 2 input edge   | #LGPTimerLPF3_CH_EDGE_NONE                |
 */
extern void LGPTimerLPF3_Params_init(LGPTimerLPF3_Params *params);

/*!
 *  @brief  Function that opens a driver for the given LGPT peripheral. Will set
 *          power dependency on timer and configure it into specified
 *          configuration.
 *
 *  @param[in]  index     Index in the LGPTimerLPF3_config table.
 *
 *  @param[in]  params    Pointer to a parameter block. If NULL, it will use
 *                        default values.
 *
 *  @return A #LGPTimerLPF3_Handle on success or NULL on an error or if it has
 *          been opened already. If NULL is returned, further LGPTimerLPF3 API
 *          calls will result in undefined behaviour.
 *
 *  @sa     #LGPTimerLPF3_close()
 */
extern LGPTimerLPF3_Handle LGPTimerLPF3_open(uint_least8_t index, const LGPTimerLPF3_Params *params);

/*!
 *  @brief  Function that closes a LGPTimer driver specified by the LGPTimer
 *          handle. Closing LGPTimer driver will release power dependency on
 *          timer and clear configuration.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first.
 *  @pre    #LGPTimerLPF3_stop() should to be called first if LGPTimer is started.
 *
 *  @param[in]  handle A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *
 *  @sa     #LGPTimerLPF3_open()
 *  @sa     #LGPTimerLPF3_start()
 *  @sa     #LGPTimerLPF3_stop()
 */
extern void LGPTimerLPF3_close(LGPTimerLPF3_Handle handle);

/*!
 *  @brief  Function that starts the timer counter of the specified LGPTimer
 *          handle with current settings and specified timer counter mode.
 *
 *  @note  The timer counter width may be 16, 24, or 32 bits depending on the
 *         LGPT instance (see @ref ti_drivers_LGPTimer_PeripheralProperties
 *         "LGPT peripheral properties").
 *
 *  @note  When signal measurement (#LGPTimerLPF3_CH_PULSE_WIDTH_MEASURE) has
 *         been selected for one or more channels, the value of mode must be
 *         either #LGPTimerLPF3_CTL_MODE_UP_ONCE or
 *         #LGPTimerLPF3_CTL_MODE_UP_PER.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully.
 *
 *  @param[in]  handle    A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open().
 *  @param[in]  mode      The timer counter mode.
 *
 *  @post   #LGPTimerLPF3_stop()
 */
extern void LGPTimerLPF3_start(LGPTimerLPF3_Handle handle, LGPTimerLPF3_Mode mode);

/*!
 *  @brief  Function that stops the timer counter of the specified LGPTimer driver.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully.
 *
 *  @param[in]  handle    A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *
 *  @sa     #LGPTimerLPF3_open()
 *  @sa     #LGPTimerLPF3_start()
 */
extern void LGPTimerLPF3_stop(LGPTimerLPF3_Handle handle);

/*!
 *  @brief  Function that sets the initial timer counter target on the specified
 *          LGPTimer. This function must be called before the timer is started.
 *          Timer counter width (16, 24, or 32 bits) depends on selected
 *          peripheral instance (see @ref ti_drivers_LGPTimer_PeripheralProperties
 *          "LGPT peripheral properties").
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *
 *  @param[in]  handle     A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *  @param[in]  value      Initial target value of the timer counter.
 *                         Max value: @ref ti_drivers_LGPTimer_PeripheralProperties "LGPT peripheral properties"
 *                         The number of counter ticks required to reach target value is value + 1.
 *                         Note that if either #LGPTimerLPF3_CTL_MODE_UP_ONCE or
 *                         #LGPTimerLPF3_CTL_MODE_UP_PER counter modes are used for generating
 *                         a PWM signal, the signal period equals value + 1.
 *                         Note that if #LGPTimerLPF3_CTL_MODE_UPDWN_PER counter mode is used for
 *                         generating a PWM signal, the signal period equals value * 2.
 *  @param[in]  intFlagClr Controls if the #LGPTimerLPF3_INT_TGT and #LGPTimerLPF3_INT_ZERO
 *                         interrupt status flags are cleared or not when this function is executed.
 *
 *  @sa     #LGPTimerLPF3_open()
 *  @sa     #LGPTimerLPF3_start()
 */
extern void LGPTimerLPF3_setInitialCounterTarget(LGPTimerLPF3_Handle handle, uint32_t value, bool intFlagClr);

/*!
 *  @brief  Function that sets the timer counter target for the next counter
 *          period on the specified LGPTimer. The specified target value will be
 *          valid as timer counter target on the upcoming zero crossing. When
 *          counting repeatedly upwards a zero crossing is regarded as when the
 *          timer counter restarts counting from 0. This function can be called
 *          after the timer has started. Timer counter width (16, 24, or 32
 *          bits) depends on selected peripheral instance
 *          (see @ref ti_drivers_LGPTimer_PeripheralProperties "LGPT peripheral properties").
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *
 *  @param[in]  handle     A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *  @param[in]  value      Next target value of the timer counter.
 *                         Max value Max value: @ref ti_drivers_LGPTimer_PeripheralProperties "LGPT peripheral
 *                         properties"
 *                         The number of counter ticks required to reach target value on the next counter
 *                         period is value + 1.
 *                         Note that if either #LGPTimerLPF3_CTL_MODE_UP_ONCE or
 *                         #LGPTimerLPF3_CTL_MODE_UP_PER counter modes are used for generating
 *                         a PWM signal, the signal period equals value + 1.
 *                         Note that if #LGPTimerLPF3_CTL_MODE_UPDWN_PER counter mode is used for
 *                         generating a PWM signal, the signal period equals value * 2.
 *
 *  @param[in]  intFlagClr Controls if the #LGPTimerLPF3_INT_TGT and #LGPTimerLPF3_INT_ZERO
 *                         interrupt status flags are cleared or not when this function is executed.
 *
 *  @sa     #LGPTimerLPF3_open()
 *  @sa     #LGPTimerLPF3_start()
 */
extern void LGPTimerLPF3_setNextCounterTarget(LGPTimerLPF3_Handle handle, uint32_t value, bool intFlagClr);

/*!
 *  @brief  Function that returns the current timer counter value.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *
 *  @param[in]  handle    A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *
 *  @return  The current timer counter value
 *
 *  @sa     #LGPTimerLPF3_open()
 */
extern uint32_t LGPTimerLPF3_getCounter(LGPTimerLPF3_Handle handle);

/*!
 *  @brief  Function to get a custom argument.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *  @pre    #LGPTimerLPF3_setArg() has to be called first
 *
 *  @param[in]  handle    A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *
 *  @return  The custom argument
 *
 *  @sa     #LGPTimerLPF3_setArg()
 *  @sa     #LGPTimerLPF3_open()
 */
extern uint32_t LGPTimerLPF3_getArg(LGPTimerLPF3_Handle handle);

/*!
 *  @brief  Function to set a custom argument.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *
 *  @param[in]  handle    A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *  @param[in]  arg       Pointer to a custom argument
 *
 *  @sa     #LGPTimerLPF3_getArg()
 *  @sa     #LGPTimerLPF3_open()
 */
extern void LGPTimerLPF3_setArg(LGPTimerLPF3_Handle handle, void *arg);

/*!
 *  @brief  Enable interrupt source for the LGPTimer handle.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *
 * @param[in]  handle    A #LGPTimerLPF3_Handle returned from LGPTimerLPF3_open()
 * @param[in]  intMask   A bit mask of interrupt flags to enable
 *
 */
void LGPTimerLPF3_enableInterrupt(LGPTimerLPF3_Handle handle, LGPTimerLPF3_IntMask intMask);

/*!
 *  @brief  Disable interrupt source for the LGPTimer handle.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *  @pre    #LGPTimerLPF3_enableInterrupt() has to be called first
 *
 * @param[in]  handle    A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 * @param[in]  intMask   A bit mask of interrupt flags to disable
 *
 */
void LGPTimerLPF3_disableInterrupt(LGPTimerLPF3_Handle handle, LGPTimerLPF3_IntMask intMask);

/*!
 *  @brief  Function that sets the initial channel compare value on the
 *          specified LGPTimer and channel. The compare value for the specified
 *          channel will be used by any compare type channel action
 *          #LGPTimerLPF3_ChannelAction specified by the LGPTimer params. The
 *          channel number dependent interrupt status flag
 *          (#LGPTimerLPF3_INT_CH0_CC for channel number #LGPTimerLPF3_CH_NO_0)
 *          will be set when the timer counter equals the channel compare value.
 *          This function must be called prior to #LGPTimerLPF3_start(). Timer
 *          compare value width (16, 24, or 32 bits) depends on selected
 *          peripheral instance
 *          (see @ref ti_drivers_LGPTimer_PeripheralProperties "LGPT peripheral properties").
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *
 *  @param[in]  handle     A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *  @param[in]  chNo       Channel number
 *  @param[in]  value      Channel compare value for specified channel number
 *
 *  @param[in]  intFlagClr Controls if the channel number dependent compare/capture interrupt
 *                         status flag is cleared or not when this function is executed.
 *
 *  @sa     #LGPTimerLPF3_open()
 */
void LGPTimerLPF3_setInitialChannelCompVal(LGPTimerLPF3_Handle handle,
                                           LGPTimerLPF3_ChannelNo chNo,
                                           uint32_t value,
                                           bool intFlagClr);

/*!
 *  @brief  Function that sets the channel compare value on the specified
 *          LGPTimer for the next cycle of the already started timer counter.
 *          The compare value for the specified channel is valid for any compare
 *          type channel action #LGPTimerLPF3_ChannelAction specified by the
 *          LGPTimer params. The channel number dependent interrupt status flag
 *          (#LGPTimerLPF3_INT_CH0_CC for channel number #LGPTimerLPF3_CH_NO_0)
 *          will be set when the timer counter equals the channel compare value
 *          in the next and following timer counter cycles. This function can be
 *          called while the timer is active.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *  @pre    #LGPTimerLPF3_setInitialChannelCompVal() has to be called first successfully
 *  @pre    #LGPTimerLPF3_start() has to be called first successfully
 *
 *  @param[in]  handle     A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *  @param[in]  chNo       Channel number
 *  @param[in]  value      Channel compare value for specified channel number.
 *                         Width (16, 24, or 32 bits) of value depends on selected peripheral instance
 *                         (see @ref ti_drivers_LGPTimer_PeripheralProperties "LGPT peripheral properties").
 *
 *  @param[in]  intFlagClr Controls if the channel number dependent compare/capture interrupt
 *                         status flag is cleared or not when this function is executed.
 *
 *  @sa     #LGPTimerLPF3_open()
 *  @sa     #LGPTimerLPF3_setInitialChannelCompVal()
 */
void LGPTimerLPF3_setNextChannelCompVal(LGPTimerLPF3_Handle handle,
                                        LGPTimerLPF3_ChannelNo chNo,
                                        uint32_t value,
                                        bool intFlagClr);

/*!
 *  @brief  Function that manually sets the current channel output level high or
 *          low. Manual update of a channel output takes priority over automatic
 *          channel updates to the same output when occurring at the same time.
 *          The complementary channel output will be set to the complementary
 *          level of the specified level.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *
 *  @param[in]  handle     A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *  @param[in]  chNo       Channel number
 *  @param[in]  level      Channel level for specified channel number
 *
 *
 *  @sa     #LGPTimerLPF3_open()
 */
void LGPTimerLPF3_setChannelOutputLevel(LGPTimerLPF3_Handle handle,
                                        LGPTimerLPF3_ChannelNo chNo,
                                        LGPTimerLPF3_ChannelLevel level);

/*!
 *  @brief  Function to get the channel compare value or channel-updated capture
 *          value. Dependent on the selected channel action for the specified
 *          channel, the function will return either the current channel compare
 *          value or the channel-updated capture value. The channel-updated
 *          capture value is returned if a successful channel capture event, as
 *          specified by the channel action, has occured on the specified
 *          channel. For a channel action of
 *          #LGPTimerLPF3_CH_PULSE_WIDTH_MEASURE, the returned value after a
 *          successful channel capture event, represents the measured period of
 *          the selected channel input signal.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *  @pre    #LGPTimerLPF3_setInitialChannelCompVal() has to be called first when
 *          channel compare action type is used.
 *
 *  @param[in]  handle    A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *  @param[in]  chNo      Channel number
 *
 *  @return  The compare value or input signal pulse width
 *
 *  @sa     #LGPTimerLPF3_open()
 *  @sa     #LGPTimerLPF3_setInitialChannelCompVal()
 */
uint32_t LGPTimerLPF3_getChCompareVal(LGPTimerLPF3_Handle handle, LGPTimerLPF3_ChannelNo chNo);

/*!
 *  @brief  Function to get the channel compare value or channel-updated capture
 *          value. Dependent on the selected channel action for the specified
 *          channel, the function will return either the current channel compare
 *          value or the channel-updated capture value. The channel-updated
 *          capture value is returned if a successful channel capture event, as
 *          specified by the channel action, has occured on the specified
 *          channel. For a channel action of
 *          #LGPTimerLPF3_CH_PULSE_WIDTH_MEASURE, the returned value after a
 *          successful channel capture event, represents the measured period of
 *          the selected channel input signal.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *  @pre    #LGPTimerLPF3_setInitialChannelCompVal() has to be called first when
 *          channel compare action type is used.
 *
 *  @param[in]  handle    A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *  @param[in]  chNo      Channel number
 *
 *  @return  The compare value or input signal pulse width
 *
 *  @sa     #LGPTimerLPF3_open()
 *  @sa     #LGPTimerLPF3_setInitialChannelCompVal()
 */
uint32_t LGPTimerLPF3_getChCaptureVal(LGPTimerLPF3_Handle handle, LGPTimerLPF3_ChannelNo chNo);

/*!
 *  @brief  Function to get the channel compare value for the next counter cycle
 *          or the channel-updated capture value. Dependent on the selected
 *          channel action for the specified channel, the function will return
 *          either the channel compare value for the next counter cycle or the
 *          channel-updated capture value. For a channel action mode of
 *          #LGPTimerLPF3_CH_PULSE_WIDTH_MEASURE, the returned value after a
 *          successful capture event will be the width of the low or high phase
 *          of the selected channel input signal. The phase is specified by
 *          #LGPTimerLPF3_ChannelInputEdge parameter for the selected channel.
 *          In order to get the channel-updated capture value for other capture
 *          channel actions than #LGPTimerLPF3_CH_PULSE_WIDTH_MEASURE, the
 *          function #LGPTimerLPF3_getChCompareVal should be used.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *  @pre    #LGPTimerLPF3_setInitialChannelCompVal() has to be called first when
 *          channel compare action type is used.
 *
 *  @param[in]  handle    A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *  @param[in]  chNo      Channel number
 *
 *  @return  The custom argument
 *
 *  @sa     #LGPTimerLPF3_open()
 *  @sa     #LGPTimerLPF3_setInitialChannelCompVal()
 */
uint32_t LGPTimerLPF3_getNextChCompareVal(LGPTimerLPF3_Handle handle, LGPTimerLPF3_ChannelNo chNo);

/*!
 *  @brief  Function to get the channel compare value for the next counter cycle
 *          or the channel-updated capture value. Dependent on the selected
 *          channel action for the specified channel, the function will return
 *          either the channel compare value for the next counter cycle or the
 *          channel-updated capture value. For a channel action mode of
 *          #LGPTimerLPF3_CH_PULSE_WIDTH_MEASURE, the returned value after a
 *          successful capture event will be the width of the low or high phase
 *          of the selected channel input signal. The phase is specified by
 *          #LGPTimerLPF3_ChannelInputEdge parameter for the selected channel.
 *          In order to get the channel-updated capture value for other capture
 *          channel actions than #LGPTimerLPF3_CH_PULSE_WIDTH_MEASURE, the
 *          function #LGPTimerLPF3_getChCompareVal() should be used.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *  @pre    #LGPTimerLPF3_setInitialChannelCompVal() has to be called first when
 *          channel compare action type is used.
 *
 *  @param[in]  handle    A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *  @param[in]  chNo      Channel number
 *
 *  @return  The custom argument
 *
 *  @sa     #LGPTimerLPF3_open()
 *  @sa     #LGPTimerLPF3_setInitialChannelCompVal()
 */
uint32_t LGPTimerLPF3_getNextChCaptureVal(LGPTimerLPF3_Handle handle, LGPTimerLPF3_ChannelNo chNo);

/*!
 *  @brief  Function to get the width of the timer counter in number of bits.
 *
 *  @pre    #LGPTimerLPF3_open() has to be called first successfully
 *
 *  @param[in]  handle     A #LGPTimerLPF3_Handle returned from #LGPTimerLPF3_open()
 *
 *  @return  The timer counter width in number of bits
 *
 *  @sa     #LGPTimerLPF3_open()
 */
uint32_t LGPTimerLPF3_getCounterWidth(LGPTimerLPF3_Handle handle);

#ifdef __cplusplus
}
#endif
#endif /* ti_drivers_timer_LGPTimerLPF3__include */
