/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 *
 *  The LGPTimer driver also handles the general purpose timer resource allocation.
 *  For each driver that requires use of a general purpose timer, it calls
 *  LGPTimerLPF3_open() to occupy the specified timer, and calls
 *  LGPTimerLPF3_close() to release the occupied timer resource.
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
 * | CC27X5        | 16 bits | 16 bits | 16 bits | 24 bits |
 *
 *  @anchor ti_drivers_LGPTimer_CounterModes
 *  # Counter Modes #
 *  The LGPTimer driver supports the following timer counter modes:
 *    - Oneshot mode counting upwards. When the counter reaches the target value, the
 *      timer is stopped automatically and the TGT interrupt status flag
 *      (@ref LGPTimerLPF3_INT_TGT) is set.
 *    - Periodic mode counting upwards. When the counter reaches target value it wraps
 *      and restarts counting from 0 again. The TGT interrupt status flag is set each
 *      time the target value is reached. The ZERO interrupt status flag
 *      (@ref LGPTimerLPF3_INT_ZERO) is set each time the counter restarts from 0.
 *    - Periodic mode counting upwards/downwads. When the counter reaches target value
 *      while counting upwards the TGT interrupt flag is set and it starts counting
 *      downwards to 0. When 0 is reached the ZERO insterrupt status flag is set
 *      and the sequence is repeated.
 *
 *  @anchor ti_drivers_LGPTimer_UnsupportedFeatures
 *  # Unsupported Features #
 *  - Use of the three configurable capture/compare channels is not supported
 *  - PWM signal generation is not supported
 *  - DMA usage is not supported
 *  - ADC trigger is not supported
 *  - QDEC mode where the timer functions as a quadrature decoder is not supported
 *  - Timer synchronization is not supported
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
 *    - After LGPTimerLPF3_open():
 *        The device is still allowed to enter Standby. When the device is
 *        active, the corresponding LGPT peripheral will be enabled and clocked.
 *        Note that a configured counter target will not be retained when going
 *        in and out of Standby. The counter target is set by the functions
 *        LGPTimerLPF3_setInitialCounterTarget() and LGPTimerLPF3_setNextCounterTarget().
 *    - After LGPTimerLPF3_start():
 *        The device will only go to Idle power mode since the high frequency
 *        clock (CLKSVT) is needed for timer operation.
 *    - After LGPTimerLPF3_stop():
 *        The device is still allowed to enter Standby. When the device is
 *        active, the corresponding LGPT peripheral will be enabled and clocked.
 *    - After LGPTimerLPF3_close():
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
 *  This documentation provides a basic @ref ti_drivers_LGPTimer_Example "example"
 *  in the form of a commented code fragment.
 *
 *  @anchor ti_drivers_LGPTimer_Example
 *  ## Periodic timer example##
 *  The code example below will generate an interrupt using the LGPTimer every 1 ms.
 *
 *  @anchor ti_drivers_LGPTimer_Code
 *  @code
 *  LGPTimerLPF3_Handle hTimer;
 *  void timerCallback(LGPTimerLPF3_Handle lgptHandle, LGPTimerLPF3_IntMask interruptMask) {
 *      // interrupt callback code goes here. Minimize processing in interrupt.
 *  }
 *
 *  void taskFxn(uintptr_t a0, uintptr_t a1) {
 *    uint32_t counterTarget;
 *
 *    lgptHandle = NULL;
 *    LGPTimerLPF3_Params params;
 *    LGPTimerLPF3_Params_init(&params);
 *    params.hwiCallbackFxn = timerCallback;
 *    lgptHandle = LGPTimerLPF3_open(0, &params);
 *    if(lgptHandle == NULL) {
 *      Log_error0("Failed to open LGPTimer");
 *      Task_exit();
 *    }
 *
 *    counterTarget = 48000;  // 1 ms with a system clock of 48 MHz
 *    LGPTimerLPF3_setInitialCounterTarget(lgptHandle, counterTarget, true);
 *    LGPTimerLPF3_enableInterrupt(lgptHandle, LGPTimerLPF3_INT_TGT);
 *
 *    LGPTimerLPF3_start(lgptHandle, LGPTimerLPF3_CTL_MODE_UP_PER);
 *
 *    while(1) {
 *        vTaskSuspend(lgptHandle);
 *    }
 *  }
 *  @endcode
 *
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

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_lgpt.h)

#ifdef __cplusplus
extern "C" {
#endif

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
    LGPTimerLPF3_INT_TGT  = 1 << LGPT_RIS_TGT_S,
    /*! Timer counter zero interrupt */
    LGPTimerLPF3_INT_ZERO = 1 << LGPT_RIS_ZERO_S,
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
 *  @brief LGPTimerLPF3 Parameters
 *
 *  LGPTimer parameters are used by the LGPTimerLPF3_open() call.
 *  Default values for these parameters are set using LGPTimerLPF3_Params_init().
 *
 *  @sa     LGPTimerLPF3_Params_init()
 */
typedef struct LGPTimerLPF3_Params
{
    /*! Hardware interrupt callback function.
     *  This pointer is optional and is only required if LGPTimerLPF3_enableInterrupt() is called. */
    LGPTimerLPF3_HwiFxn hwiCallbackFxn;
    /*! Interrupt phase early or late for TGT and ZERO interrupts */
    bool intPhaseLate;
    /*! Prescaler division factor */
    uint8_t prescalerDiv;
    /*! Timer debug stall mode */
    LGPTimerLPF3_DebugMode debugStallMode;
} LGPTimerLPF3_Params;

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
 *      },
 *  };
 *  @endcode
 */
typedef struct LGPTimerLPF3_HWAttrs
{
    /*! LGPTimer peripheral base address */
    uint32_t baseAddr;
    /*! LGPTimer peripheral interrupt vector */
    uint8_t intNum;
    /*! LGPTimer peripheral's interrupt priority */
    uint8_t intPriority;
    /*! LGPTimer peripheral's power resource ID */
    uint8_t powerID;
} LGPTimerLPF3_HWAttrs;

/*!
 *  @brief  LGPTimerLPF3 Object
 *
 * These fields are used by the driver to store and modify LGPTimer configuration
 * during run-time.
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
    HwiP_Struct hwi;                       /*!< Hardware interrupt struct */
    LGPTimerLPF3_HwiFxn hwiCallbackFxn;    /*!< Hardware interrupt callback function */
    Power_NotifyObj postNotify;            /*!< For Standby reconfiguration */
    uint32_t arg;                          /*!< Arbritrary Argument */
    bool isOpen;                           /*!< Object is opened flag  */
    bool intPhaseLate;                     /*!< Interrupt phase early or late for TGT and ZERO interrupts */
    uint8_t prescalerDiv;                  /*!< Prescaler division factor */
    LGPTimerLPF3_DebugMode debugStallMode; /*!< Timer debug stall mode */
} LGPTimerLPF3_Object;

/*!
 *  @brief  LGPTimer Global configuration
 *
 *  The LGPTimerLPF3_Config structure contains a set of pointers
 *  used to characterize the LGPTimer driver implementation.
 *  An example is shown below:
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
 *  Default values are:
 *  Interrupt callback fxn: NULL,
 *  Interrupt phase late  : false,
 *  Prescaler division    : 0,
 *  Timer debug stall mode: LGPTimerLPF3_DEBUG_STALL_OFF
 */
extern void LGPTimerLPF3_Params_init(LGPTimerLPF3_Params *params);

/*!
 *  @brief  Function that opens a driver for the given LGPT peripheral. Will set
 *          power dependency on timer and configure it into specified configuration.
 *
 *  @param[in]  index     Index in the LGPTimerLPF3_config table.
 *
 *  @param[in]  params    Pointer to a parameter block. If NULL, it will use
 *                        default values.
 *
 *  @return A LGPTimerLPF3_Handle on success or NULL on an error or if it has been
 *          opened already. If NULL is returned, further LGPTimerLPF3 API calls will
 *          result in undefined behaviour.
 *
 *  @sa     LGPTimerLPF3_close()
 */
extern LGPTimerLPF3_Handle LGPTimerLPF3_open(uint_least8_t index, const LGPTimerLPF3_Params *params);

/*!
 *  @brief  Function that closes a LGPTimer driver specified by the LGPTimer handle.
 *          Closing LGPTimer driver will release power dependency on timer and clear
 *          configuration.
 *
 *  @pre    LGPTimerLPF3_open() has to be called first.
 *  @pre    LGPTimerLPF3_stop() should to be called first if LGPTimer is started.
 *
 *  @param[in]  handle A LGPTimerLPF3 handle returned from LGPTimerLPF3_open()
 *
 *  @sa     LGPTimerLPF3_open()
 *  @sa     LGPTimerLPF3_start()
 *  @sa     LGPTimerLPF3_stop()
 */
extern void LGPTimerLPF3_close(LGPTimerLPF3_Handle handle);

/*!
 *  @brief  Function that starts the timer counter of the specified LGPTimer
 *          handle with current settings and specified timer counter mode.
 *
 *  @note  The LGPT3 timer has timer counter width of 24-bits. The others have 16-bits.
 *
 *  @pre    LGPTimerLPF3_open() has to be called first successfully.
 *
 *  @param[in]  handle    A LGPTimerLPF3 handle returned from LGPTimerLPF3_open().
 *  @param[in]  mode      The timer counter mode.
 *
 *  @sa     LGPTimerLPF3_setPrescaler()
 *  @post   LGPTimerLPF3_stop()
 */
extern void LGPTimerLPF3_start(LGPTimerLPF3_Handle handle, LGPTimerLPF3_Mode mode);

/*!
 *  @brief  Function that stops the timer counter of the specified LGPTimer driver.
 *
 *  @pre    LGPTimerLPF3_open() has to be called first successfully.
 *
 *  @param[in]  handle    A LGPTimerLPF3 handle returned from LGPTimerLPF3_open()
 *
 *  @sa     LGPTimerLPF3_open()
 *  @sa     LGPTimerLPF3_start()
 */
extern void LGPTimerLPF3_stop(LGPTimerLPF3_Handle handle);

/*!
 *  @brief  Function that sets the initial timer counter target on the specified LGPTimer.
 *          This function must be called before the timer is started.
 *          The @ref LGPTimerLPF3_INT_TGT and @ref LGPTimerLPF3_INT_ZERO interrupt status
 *          flags are cleared.
 *          Timer counter width (16 or 24 bits) depends on selected peripheral instance.
 *
 *  @pre    LGPTimerLPF3_open() has to be called first successfully
 *
 *  @param[in]  handle    A LGPTimerLPF3 handle returned from LGPTimerLPF3_open
 *  @param[in]  value     Initial target value of the timer counter.
 *                        Max value: @ref ti_drivers_LGPTimer_PeripheralProperties "LGPT peripheral properties"
 *  @param[in]  intFlagClr Controls if the @ref LGPTimerLPF3_INT_TGT and @ref LGPTimerLPF3_INT_ZERO
 *                         interrupt status flags are cleared or not when this function is executed.
 *
 *  @sa     LGPTimerLPF3_open()
 *  @sa     LGPTimerLPF3_start()
 */
extern void LGPTimerLPF3_setInitialCounterTarget(LGPTimerLPF3_Handle handle, uint32_t value, bool intFlagClr);

/*!
 *  @brief  Function that sets the next timer counter target on the specified LGPTimer.
 *          The specified target value will be valid as timer counter target on the
 *          upcoming zero crossing. When counting repeatedly upwards a zero crossing
 *          is regarded as when the timer counter restarts counting from 0.
 *          The @ref LGPTimerLPF3_INT_TGT and @ref LGPTimerLPF3_INT_ZERO interrupt status
 *          flags are cleared at the counter zero crossing.
 *          This function can be called after the timer has started.
 *          Timer counter width (16 or 24 bits) depends on selected peripheral instance.
 *
 *  @pre    LGPTimerLPF3_open() has to be called first successfully
 *
 *  @param[in]  handle     A LGPTimerLPF3 handle returned from LGPTimerLPF3_open
 *  @param[in]  value      Next target value of the timer counter.
 *                         Max value Max value: @ref ti_drivers_LGPTimer_PeripheralProperties "LGPT peripheral
 *                         properties"
 *
 *  @param[in]  intFlagClr Controls if the @ref LGPTimerLPF3_INT_TGT and @ref LGPTimerLPF3_INT_ZERO
 *                         interrupt status flags are cleared or not when this function is executed.
 *
 *  @sa     LGPTimerLPF3_open()
 *  @sa     LGPTimerLPF3_start()
 */
extern void LGPTimerLPF3_setNextCounterTarget(LGPTimerLPF3_Handle handle, uint32_t value, bool intFlagClr);

/*!
 *  @brief  Function that returns the current timer counter value.
 *
 *  @pre    LGPTimerLPF3_open() has to be called first successfully
 *
 *  @param[in]  handle    A LGPTimerLPF3 handle returned from LGPTimerLPF3_open
 *
 *  @return  The current timer counter value
 *
 *  @sa     LGPTimerLPF3_open
 */
extern uint32_t LGPTimerLPF3_getCounter(LGPTimerLPF3_Handle handle);

/*!
 *  @brief  Function to get a custom argument.
 *
 *  @pre    LGPTimerLPF3_open() has to be called first successfully
 *  @pre    LGPTimerLPF3_setArg() has to be called first
 *
 *  @param[in]  handle    A LGPTimerLPF3 handle returned from LGPTimerLPF3_open
 *
 *  @return  The custom argument
 *
 *  @sa     LGPTimerLPF3_setArg
 *  @sa     LGPTimerLPF3_open
 */
extern uint32_t LGPTimerLPF3_getArg(LGPTimerLPF3_Handle handle);

/*!
 *  @brief  Function to set a custom argument.
 *
 *  @pre    LGPTimerLPF3_open() has to be called first successfully
 *
 *  @param[in]  handle    A LGPTimerLPF3 handle returned from LGPTimerLPF3_open
 *  @param[in]  arg       Pointer to a custom argument
 *
 *  @sa     LGPTimerLPF3_getArg
 *  @sa     LGPTimerLPF3_open
 */
extern void LGPTimerLPF3_setArg(LGPTimerLPF3_Handle handle, void *arg);

/*!
 *  @brief  Enable interrupt source for the LGPTimer handle.
 *
 *  @pre    LGPTimerLPF3_open() has to be called first successfully
 *
 * @param[in]  handle    A LGPTimerLPF3 handle returned from LGPTimerLPF3_open
 * @param[in]  intMask   A bit mask of interrupt flags to enable
 *
 */
void LGPTimerLPF3_enableInterrupt(LGPTimerLPF3_Handle handle, LGPTimerLPF3_IntMask intMask);

/*!
 *  @brief  Disable interrupt source for the LGPTimer handle.
 *
 *  @pre    LGPTimerLPF3_open() has to be called first successfully
 *  @pre    LGPTimerLPF3_enableInterrupt() has to be called first
 *
 * @param[in]  handle    A LGPTimerLPF3 handle returned from LGPTimerLPF3_open
 * @param[in]  intMask   A bit mask of interrupt flags to disable
 *
 */
void LGPTimerLPF3_disableInterrupt(LGPTimerLPF3_Handle handle, LGPTimerLPF3_IntMask intMask);

#ifdef __cplusplus
}
#endif
#endif /* ti_drivers_timer_LGPTimerLPF3__include */
