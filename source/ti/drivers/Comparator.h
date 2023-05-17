/*
 * Copyright (c) 2022, Texas Instruments Incorporated
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
/*!****************************************************************************
 *  @file       Comparator.h
 *  @brief      Comparator Driver
 *
 *  @anchor ti_drivers_comparator_Overview
 *  # Overview
 *
 *  The comparator driver allows you to manage the voltage comparator peripheral
 *  via simple and portable APIs. This driver supports monitoring of analog
 *  voltages on external pins to allow event generation when the voltage crosses
 *  a defined threshold.
 *
 *  The Comparator driver serves as the main interface for a typical RTOS
 *  application. Its purpose is to redirect the Comparator APIs to
 *  device-specific implementations which are specified using a pointer to a
 *  #Comparator_FxnTable.
 *  The device-specific implementations are responsible for creating all the
 *  RTOS specific primitives to allow for thread-safe operation.
 *
 *  The Comparator driver is an analog driver that accepts a configuration of
 *  two analog input signals, compares those values, and then outputs high on
 *  the output signal if the positive input is more positive than that of the
 *  negative input terminal.
 *
 *  Device-specific capabilities such as input voltage division or output analog
 *  filtering are configured via the implementation specific hwAttrs
 *  configuration structure. This top level driver provides all APIs
 *  needed to provide a uniform and generic Comparator driver experience
 *  regardless of the underlying peripheral implementation.
 *
 *  <hr>
 *  @anchor ti_drivers_Comparator_Usage
 *  # Usage
 *
 *  This documentation provides a basic @ref ti_drivers_Comparator_Synopsis
 *  "usage summary" and a set of @ref ti_drivers_Comparator_Examples "examples"
 *  in the form of commented code fragments. Detailed descriptions of the
 *  APIs are provided in subsequent sections.
 *
 *  @anchor ti_drivers_Comparator_Synopsis
 *  ## Synopsis
 *
 *  The Comparator driver is used to monitor two input analog signals (internal
 *  or external) and generate an output if the potential of the positive input
 *  terminal is greater than that of the negative input channel. This is
 *  commonly used for power supply supervision as well as precision slope
 *  analog-to-digital conversions. The code below sets up two arbitrary
 *  input signals to the Comparator driver and handles triggers of the voltage
 *   crossing in the appropriate callback function.
 *
 *  The Comparator driver supports two distinct methods of accessing/utilizing
 *  the underlying comparator peripheral's output signal:
 *  - Returning the output level on request.
 *  - Providing a user callback function invoked upon a Comparator trigger
 *    event.
 *
 *  Accessing the output level directly can be done by calling the
 *  Comparator_getLevel() API after the driver has been successfully opened
 *  and started. This function will return a #Comparator_OUTPUT_HIGH if the
 *  positive input terminal is more positive than the negative terminal and
 *  #Comparator_OUTPUT_LOW if the opposite is true. If the output level of the
 *  comparator peripheral cannot be determined or the device is in an error
 *  state a value of #Comparator_OUTPUT_NOT_AVAILABLE is returned.
 *
 *  The user callback functionality provides a way for the Comparator driver
 *  and its underlying implementation to communicate events to the calling user
 *  application. The user callback is selected through the
 *  #Comparator_Params.callbackFxn field.
 *  If a non-null function pointer is provided to the callbackFxn configuration
 *  parameter, the callback will be invoked whenever a relevant event occurs in
 *  the driver. Primarily this callback will be invoked when the output of the
 *  comparator is triggered, however error events can also be passed through
 *  this callback. Since the user's callback may be called in the context of a
 *  hardware interrupt, the callback function must not make any RTOS blocking
 *  calls.
 *
 *  Note that accessing the output level is compatible with callback mode and
 *  can be mixed accordingly. For example a user callback can be provided and
 *  the Comparator_getLevel() API can still be called when needed.
 *
 *  @anchor ti_drivers_Comparator_Synopsis_Code
 *
 *  @code
 *
 *  #include <ti/drivers/Comparator.h>
 *
 *  void comparatorCallback(Comparator_Handle handle,
 *                          int_fast16_t returnValue,
 *                          Comparator_Trigger trigger)
 *  {
 *      // React to Comparator event
 *      sem_post(&someSemaphore);
 *  }
 *
 *  void someComparatorFunction()
 *  {
 *      Comparator_Handle    handle;
 *      Comparator_Params    params;
 *
 *      Comparator_Params_init(&params);
 *      params.callbackFxn = &comparatorCallback;
 *      params.trigger     = Comparator_TRIGGER_RISING;
 *
 *      handle = Comparator_open(CONFIG_COMPARATOR_0, &params);
 *
 *      if (handle == NULL)
 *      {
 *          //Comparator_open() failed
 *          while (1) {}
 *      }
 *
 *      status = Comparator_start(handle);
 *
 *      if (status == Comparator_STATUS_ERROR)
 *      {
 *          //Comparator_start() failed
 *          while (1) {}
 *      }
 *
 *      // Waiting for some output event to signal from the callback
 *      sem_wait(&someSemaphore);
 *
 *      Comparator_stop(handle);
 *  }
 *
 *  @endcode
 *
 *  Note that while the code above operates with a callback function provided,
 *  if a NULL value is given as the callbackFxn parameter the
 *  Comparator_getLevel() function can be invoked to dynamically get the output
 *  level of an initialized/started Comparator driver instance.
 *
 *  <hr>
 *  @anchor ti_drivers_Comparator_Examples
 *  # Examples
 *
 *  @li @ref ti_drivers_Comparator_Examples_open "Opening a Comparator instance"
 *
 *  @anchor ti_drivers_Comparator_Examples_open
 *  ## Opening a Comparator instance
 *
 *  @code
 *  Comparator_Handle comparator;
 *  Comparator_Params params;
 *
 *  Comparator_Params_init(&params);
 *  comparator = Comparator_open(CONFIG_COMPARATOR_0, &params);
 *  if (comparator == NULL)
 *  {
 *      // Comparator_open() failed
 *      while (1) {}
 *  }
 *  @endcode
 *
 *  <hr>
 *  @anchor ti_drivers_Comparator_Configuration
 *  # Configuration
 *
 *  Refer to the @ref driver_configuration "Driver's Configuration" section
 *  for driver configuration information.
 *  <hr>
 *
 ******************************************************************************
 */

#ifndef ti_drivers_Comparator__include
#define ti_drivers_Comparator__include

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @defgroup Comparator_STATUS Comparator status codes to be used with
 *  Comparator.h APIs.
 *  @{
 */

/*!
 * Common Comparator_control status code reservation offset.
 * Comparator driver implementations should offset status codes with
 * Comparator_STATUS_RESERVED growing negatively.
 *
 * Example implementation specific status codes:
 * @code
 * #define ComparatorXYZ_STATUS_ERROR0     Comparator_STATUS_RESERVED - 0
 * #define ComparatorXYZ_STATUS_ERROR1     Comparator_STATUS_RESERVED - 1
 * @endcode
 */
#define Comparator_STATUS_RESERVED (-32)

/*!
 * @brief   Successful status code.
 *
 */
#define Comparator_STATUS_SUCCESS (0)

/*!
 * @brief   Generic error status code.
 *
 */
#define Comparator_STATUS_ERROR (-1)
/** @}*/

/*!
 *  @brief  A handle that is returned from a Comparator_open() call.
 */
typedef struct Comparator_Config *Comparator_Handle;

/*!
 *  @brief Comparator output level
 *
 *  This enum defines the level of the output.
 *
 */
typedef enum
{
    Comparator_OUTPUT_HIGH,         /*!< Positive input terminal is more
                                     *   positive than the negative input
                                     *   terminal
                                     */
    Comparator_OUTPUT_LOW,          /*!< Negative input terminal is more
                                     *   positive than the positive input
                                     *   terminal
                                     */
    Comparator_OUTPUT_NOT_AVAILABLE /*!< Output level cannot be determined.
                                     *   Possibly due to error or it not being
                                     *   ready.
                                     */
} Comparator_OutputLevel;

/*!
 *  @brief  Comparator interrupt trigger
 *
 *  This enum defines the comparator output condition that will trigger an
 *  interrupt. Not all output conditions are supported on all devices, refer
 *  to the device documentation to see which interrupt levels are supported.
 */
typedef enum
{
    Comparator_TRIGGER_NONE,   /*!< The comparator module should not trigger
                                *   an interrupt
                                */
    Comparator_TRIGGER_RISING, /*!< A rising edge on the comparator output will
                                *   trigger an interrupt
                                */
    Comparator_TRIGGER_FALLING /*!< A falling edge on the comparator output will
                                *   trigger an interrupt
                                */
} Comparator_Trigger;

/*!
 *  @brief  Comparator callback function
 *
 *  Callback from the Comparator driver. This will be called when the driver is
 *  setup with a callback function pointer provided to the
 *  #Comparator_Params.callbackFxn parameter and is used to communicate events
 *  to the calling application.
 *
 *  @param[in]  handle       The handle of the Comparator instance that
 *                           generated the callback.
 *  @param[in]  returnValue  The result of the Comparator operation. Depending
 *                           on the device-specific implementation it may
 *                           contain an error code. Informs the application of
 *                           why the callback function was called.
 *                           The generic return values, common to all
 *                           implementations, are listed below:
 *                           @li Comparator_STATUS_SUCCESS
 *                           @li Comparator_STATUS_ERROR
 *
 *  @note                    Refer to the device-specific implementation
 *                           header file for additional status codes.

 *  @param[in]  trigger      The trigger currently selected for the Comparator
 *                           instance that generated the callback. The supported
 *                           trigger values that can be received by the callback
 *                           function are:
 *                           @li Comparator_TRIGGER_NONE
 *                           @li Comparator_TRIGGER_FALLING
 *                           @li Comparator_TRIGGER_RISING
 *
 *  @note                    When #Comparator_TRIGGER_NONE is selected, a
 *                           callback can only be invoked due to a comparator
 *                           error, on devices supporting this type of return
 *                           value.
 */
typedef void (*Comparator_CallBackFxn)(Comparator_Handle handle, int_fast16_t returnValue, Comparator_Trigger trigger);

/*!
 *  @brief Comparator parameters
 *
 *  Comparator parameters are used by the Comparator_open() call. Default values
 *  for these parameters are set using Comparator_Params_init().
 *
 */
typedef struct
{
    /*! Callback function called when events happen in the Comparator driver */
    Comparator_CallBackFxn callbackFxn;

    /*! Interrupt trigger of the Comparator module */
    Comparator_Trigger trigger;
} Comparator_Params;

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              Comparator_close().
 */
typedef void (*Comparator_CloseFxn)(Comparator_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              Comparator_getLevel().
 */
typedef Comparator_OutputLevel (*Comparator_GetLevelFxn)(Comparator_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              Comparator_getLevel().
 */
typedef Comparator_Trigger (*Comparator_getTriggerFxn)(Comparator_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              Comparator_init().
 */
typedef void (*Comparator_InitFxn)(Comparator_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              Comparator_open().
 */
typedef Comparator_Handle (*Comparator_OpenFxn)(Comparator_Handle handle, Comparator_Params *params);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              Comparator_open().
 */
typedef int_fast16_t (*Comparator_setTriggerFxn)(Comparator_Handle handle, Comparator_Trigger trigger);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              Comparator_start().
 */
typedef int_fast16_t (*Comparator_StartFxn)(Comparator_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              Comparator_stop().
 */
typedef void (*Comparator_StopFxn)(Comparator_Handle handle);

/*!
 *  @brief      The definition of a comparator function table that contains the
 *              required set of functions to control a specific comparator
 *              driver implementation.
 */
typedef struct
{
    /*! Function to close the specified peripheral. */
    Comparator_CloseFxn closeFxn;

    /*! Function to get comparator output level. */
    Comparator_GetLevelFxn getLevelFxn;

    /*! Function to get the comparator output trigger condition */
    Comparator_getTriggerFxn getTriggerFxn;

    /*! Function to initialize the given data object. */
    Comparator_InitFxn initFxn;

    /*! Function to open the specified peripheral. */
    Comparator_OpenFxn openFxn;

    /*! Function to set the comparator output trigger condition */
    Comparator_setTriggerFxn setTriggerFxn;

    /*! Function to start the specified peripheral. */
    Comparator_StartFxn startFxn;

    /*! Function to stop the specified peripheral. */
    Comparator_StopFxn stopFxn;
} Comparator_FxnTable;

/*!
 *  @brief Comparator driver's custom @ref driver_configuration "configuration"
 *  structure.
 *
 *  @sa     Comparator_init()
 */
typedef struct Comparator_Config
{
    /*! Pointer to a @ref driver_function_table "function pointer table"
     *  with driver-specific implementations of Comparator APIs */
    Comparator_FxnTable const *fxnTablePtr;

    /*! Pointer to a driver specific @ref driver_objects "data object". */
    void *object;

    /*! Pointer to a driver specific @ref driver_hardware_attributes
     *  "hardware attributes structure". */
    void const *hwAttrs;
} Comparator_Config;

extern const Comparator_Config Comparator_config[];

/*!
 *  @brief  Function to close a Comparator driver instance
 *
 *  @pre  Comparator_open() has been called.
 *
 *  @param[in]  handle  A #Comparator_Handle returned from Comparator_open()
 */
extern void Comparator_close(Comparator_Handle handle);

/*!
 *  @brief  Function which returns the level of the comparator output.
 *
 *  @pre    Comparator_open() and Comparator_start() have been called.
 *
 *  @param[in]  handle  A Comparator handle returned from Comparator_open().
 *
 *  @return A value of type #Comparator_OutputLevel describing the output state
 *          of the comparator peripheral.
 *
 *  @sa     Comparator_start()
 */
extern Comparator_OutputLevel Comparator_getLevel(Comparator_Handle handle);

/*!
 *  @brief  Function to get the interrupt trigger of the comparator instance
 *
 *  @pre    Comparator_open() has been called.
 *
 *  @param[in]  handle  A #Comparator_Handle returned from Comparator_open().
 *
 *  @return A value of type #Comparator_Trigger describing the output condition
 *          of the comparator that will generate an interrupt.
 *
 *  @sa Comparator_getTrigger()
 */
extern Comparator_Trigger Comparator_getTrigger(Comparator_Handle handle);

/*!
 *  @brief  Function to initialize the Comparator driver
 *
 *  This function must also be called before any other Comparator driver APIs.
 */
extern void Comparator_init(void);

/*!
 *  @brief  Function to initialize the Comparator peripheral
 *
 *  Function to initialize the Comparator peripheral specified by the
 *  particular index value. Comparator_open() will only setup the Comparator
 *  instance but it will not enable it. It is required to subsequently call
 *  Comparator_start() to make the instance operational.
 *
 *  @pre    Comparator_init() has been called
 *
 *  @param[in]  index   Index in the @p Comparator_Config[] array.
 *  @param[in]  params  Pointer to an initialized #Comparator_Params structure.
 *                      If NULL, the default #Comparator_Params values are used.
 *
 *  @return A #Comparator_Handle on success or NULL on error.
 *
 *  @sa     Comparator_init()
 *  @sa     Comparator_close()
 */
extern Comparator_Handle Comparator_open(uint32_t index, Comparator_Params *params);

/*!
 *  @brief  Initialize an #Comparator_Params structure to its default values.
 *
 *  @param[in]  params  A pointer to an #Comparator_Params structure.
 *
 *  Default values are:
 *  @arg #Comparator_Params.callbackFxn = NULL
 *  @arg #Comparator_Params.trigger = Comparator_TRIGGER_RISING
 */
extern void Comparator_Params_init(Comparator_Params *params);

/*!
 *  @brief  Function to set at run-time the interrupt trigger.
 *
 *  This functions sets the Comparator trigger at run-time. In case the
 *  comparator is running, following a call to Comparator_start(), this function
 *  will temporarily stop the Comparator instance, modify the trigger value and
 *  then restart it once again.
 *
 *  @pre    Comparator_open() has been called.
 *
 *  @param[in]  handle   A #Comparator_Handle returned from Comparator_open().
 *  @param[in]  trigger  A value of type #Comparator_Trigger describing the
 *                       output condition of the comparator that will generate
 *                       an interrupt.
 *
 *  @retval #Comparator_STATUS_SUCCESS The comparator trigger have been
 *          succesfully modified.
 *  @retval #Comparator_STATUS_ERROR An error occured.
 *
 *  @note The return codes listed above are generic and common to all
 *        implementations.
 *        Refer to the device-specific implementation header file for additional
 *        status codes.
 *
 *  @sa Comparator_getTrigger()
 */
extern int_fast16_t Comparator_setTrigger(Comparator_Handle handle, Comparator_Trigger trigger);

/*!
 *  @brief  Function to start the comparator instance.
 *
 *  This function enables the Comparator instance. After this function has been
 *  called the Comparator will start generating a valid output and events.
 *  After Comparator_start has been called, the Comparator's output can be read
 *  using Comparator_getLevel().
 *
 *  @pre    Comparator_open() has been called.
 *
 *  @param[in]  handle  A #Comparator_Handle returned from Comparator_open().
 *
 *  @retval #Comparator_STATUS_SUCCESS The comparator successfully started.
 *  @retval #Comparator_STATUS_ERROR The comparator failed to start.
 *
 *  @note The return codes listed above are generic and common to all
 *        implementations.
 *        Refer to the device-specific implementation header file for additional
 *        status codes.
 *
 *  @sa     Comparator_stop()
 */
extern int_fast16_t Comparator_start(Comparator_Handle handle);

/*!
 *  @brief  Function to stop a comparator instance. If the comparator instance
 *          is already stopped this function has no effect.
 *
 *  @pre    Comparator_open() has been called.
 *
 *  @param[in]  handle  A #Comparator_Handle returned from Comparator_open().
 *
 *  @sa     Comparator_start()
 *
 */
extern void Comparator_stop(Comparator_Handle handle);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_Comparator__include */