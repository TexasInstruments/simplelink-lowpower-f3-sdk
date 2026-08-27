/*
 * Copyright (c) 2025-2026, Texas Instruments Incorporated
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
/*! ============================================================================
 *  @file       GPIOLPF4.h
 *
 *  @brief      GPIO driver implementation for Low Power F4 devices or Low
 *              Power F3 devices with the shuffle network from the F4 devices.
 *
 *  The GPIO header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/GPIO.h>
 *  #include <ti/drivers/gpio/GPIOLPF4.h>
 *  @endcode
 *
 *  Refer to @ref GPIO.h for a complete description of the GPIO
 *  driver APIs provided and examples of their use.
 *
 *  The @c GPIO_CFG_*_INTERNAL definitions in this file should not be used
 *  directly. All @c GPIO_CFG_* macros should be used as-is from @ref GPIO.h.
 *
 *  The LPF4 devices (and some LPF3 devices) have a shuffle network that allows
 *  for flexible routing of SVT peripheral functions. The generic GPIO driver
 *  APIs from @ref GPIO.h are not sufficient to support the shuffle network.
 *
 *  The LPF4 devices (and the few LPF3 devices with a shuffle network) have
 *  two layers of muxing. The shuffle network, and individual muxing for each
 *  DIO. The shuffle network has #GPIOLPF4_SHUFFLE_NETWORK_SIZE inputs and
 *  #GPIOLPF4_SHUFFLE_NETWORK_SIZE outputs. Peripheral functions/signals are
 *  connected to the inputs of the shuffle network, and DIOs are connected to
 *  outputs of the shuffle network. Note, the terms input and output when
 *  referring to the shuffle network in the GPIO driver documentation are not
 *  related to the direction of the signals themselves. For example, UART0RX is
 *  still considered an input to the shuffle network, even though it is actually
 *  an input to the UART0 peripheral. The shuffle network allows to
 *  simultaneously mux any input to any output, but only a one to one mapping
 *  is supported. The individual muxing for each DIO allows muxing of up to 8
 *  different signals to the DIO, where up to 3 of those signals are outputs of
 *  the shuffle network.
 *
 *  To mux an SVT peripheral signal to a given DIO, first the shuffle network
 *  must be correctly configured to mux the signal to the desired shuffle
 *  network output, and then the muxing for the DIO must be configured to select
 *  the the shuffle network output that the signal is muxed to.
 *
 *  The generic GPIO driver APIs from @ref GPIO.h do not take into account the
 *  shuffle network. The muxing related functions, #GPIO_getMux() and
 *  #GPIO_setConfigAndMux(), only take into account the individual muxing for
 *  each DIO. A signal from the shuffle network can be selected by using the
 *  #GPIO_MUX_SNPF0, #GPIO_MUX_SNPF1 and #GPIO_MUX_SNPF2 mux options.
 *  All valid mux options can be found at @ref gpiolpf4_mux_options.
 *
 *  It is not trivial to dynamically recompute the shuffle network configuration
 *  if the muxing requirements change. Meaning, if the application allows it,
 *  the shuffle network configuration should be fixed and pre-computed such that
 *  only the individual muxing for the DIO needs to be changed dynamically.
 *  However, the driver does support dynamic configuration of the shuffle
 *  network. See #GPIOLPF4_computeShuffleNetworkConfig() and
 *  #GPIOLPF4_applyShuffleNetworkConfig() for more details.
 *
 *  The application must define a @c GPIOLPF4_shuffleNetworkConfig symbol of the
 *  type #GPIOLPF4_ShuffleNetworkConfig with the initial shuffle network
 *  configuration. This is used by #GPIO_init() to configure the shuffle network
 *  at startup. If SysConfig is used, the @c GPIOLPF4_shuffleNetworkConfig symbol
 *  will be defined in SysConfig generated code.
 *
 */

#ifndef ti_drivers_GPIOLPF4__include
#define ti_drivers_GPIOLPF4__include

#include <ti/drivers/GPIO.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/ioc.h)

#ifdef __cplusplus
extern "C" {
#endif

/** @name GPIO Mux options
 *  @anchor gpiolpf4_mux_options
 *  @brief Mux options for LPF4 devices
 *
 *  @details Mux options to be passed to the @c mux parameter of
 *  #GPIO_setConfigAndMux().
 *
 *  @note When #GPIO_MUX_SNPF0, #GPIO_MUX_SNPF1 or GPIO_MUX_SNPF2 is used
 *  the shuffle network must be correctly configured to ensure the desired
 *  peripheral signal will be used.
 *  @{
 */
//! \hideinitializer GPIO function
#define GPIO_MUX_GPIO_INTERNAL (IOC_MUX_GPIO)
/*! \hideinitializer AON ULL function */
#define GPIO_MUX_AON_ULL       (IOC_MUX_AON_ULL)
/*! Digital Test Bus (DTB) function */
#define GPIO_MUX_DTB           (IOC_MUX_DTB)
/*! \hideinitializer Analog function */
#define GPIO_MUX_ANALOG        (IOC_MUX_ANALOG)
/*! \hideinitializer High speed peripheral function */
#define GPIO_MUX_HIGH_SPEED    (IOC_MUX_HIGH_SPEED)
/*! \hideinitializer Shuffle Network PF0 */
#define GPIO_MUX_SNPF0         (IOC_MUX_SNPF0)
/*! \hideinitializer Shuffle Network PF1 */
#define GPIO_MUX_SNPF1         (IOC_MUX_SNPF1)
/*! \hideinitializer Shuffle Network PF2 */
#define GPIO_MUX_SNPF2         (IOC_MUX_SNPF2)
/** @} */

#define GPIOLPF4_SHUFFLE_NETWORK_SIZE             (IOC_SHUFFLE_NETWORK_SIZE)
#define GPIOLPF4_SHUFFLE_NETWORK_CONFIG_REG_COUNT (IOC_SHUFFLE_NETWORK_CONFIG_REG_COUNT)

/* We don't define this value on purpose - any unsupported values will cause a
 * compile-time error. If your compiler tells you that this macro is missing,
 * you are trying to use an unsupported option.
 *
 * See below for which options are unsupported.
 */
#undef GPIOLPF4_CFG_OPTION_NOT_SUPPORTED

/* Most configuration values are directly mapped to fields in the IOCn
 * registers, but the mux bits are reserved for configuration options that
 * cannot be directly mapped to the IOCn registers. The define below is the
 * mask used by the GPIO driver to mask off the non-IOC configuration values.
 */
#define GPIOLPF4_CFG_IOC_M (IOC_CONFIG_ALL_M)

#define GPIO_CFG_DO_NOT_CONFIG_INTERNAL (0x80000000U)

/* Low and high value interrupts are not available on Low Power F4 devices */
#define GPIO_CFG_INT_LOW_INTERNAL  (GPIOLPF4_CFG_OPTION_NOT_SUPPORTED)
#define GPIO_CFG_INT_HIGH_INTERNAL (GPIOLPF4_CFG_OPTION_NOT_SUPPORTED)

/* General options */
#define GPIO_CFG_NO_DIR_INTERNAL (IOC_CONFIG_IO_MODE_NORMAL | GPIOLPF4_CFG_PIN_IS_INPUT_INTERNAL)
/* Hysteresis is enabled by default for all input pins due the specific hardware
 * implementation on these devices. This may impact pin response by 1-2ns, but
 * creates significantly more stable environments for high-speed use cases like
 * SPI.
 */
#define GPIO_CFG_INPUT_INTERNAL                                                                        \
    (IOC_CONFIG_IO_MODE_NORMAL | IOC_CONFIG_INPUT_BUFFER_ENABLED | IOC_CONFIG_STANDBY_WAKEUP_ENABLED | \
     IOC_CONFIG_HYSTERESIS_ENABLED | GPIOLPF4_CFG_PIN_IS_INPUT_INTERNAL)
#define GPIO_CFG_OUTPUT_INTERNAL \
    (IOC_CONFIG_IO_MODE_NORMAL | IOC_CONFIG_INPUT_BUFFER_ENABLED | GPIOLPF4_CFG_PIN_IS_OUTPUT_INTERNAL)
#define GPIO_CFG_OUTPUT_OPEN_DRAIN_INTERNAL \
    (IOC_CONFIG_IO_MODE_OPEN_DRAIN_NMOS | IOC_CONFIG_INPUT_BUFFER_ENABLED | GPIOLPF4_CFG_PIN_IS_OUTPUT_INTERNAL)
#define GPIO_CFG_OUT_OPEN_SOURCE_INTERNAL \
    (IOC_CONFIG_IO_MODE_OPEN_DRAIN_PMOS | IOC_CONFIG_INPUT_BUFFER_ENABLED | GPIOLPF4_CFG_PIN_IS_OUTPUT_INTERNAL)

#define GPIO_CFG_PULL_NONE_INTERNAL (IOC_CONFIG_PULL_CONTROL_NONE)
#define GPIO_CFG_PULL_UP_INTERNAL   (IOC_CONFIG_PULL_CONTROL_UP)
#define GPIO_CFG_PULL_DOWN_INTERNAL (IOC_CONFIG_PULL_CONTROL_DOWN)

#define GPIO_CFG_INT_NONE_INTERNAL       (IOC_CONFIG_EDGE_DETECT_DISABLED)
#define GPIO_CFG_INT_FALLING_INTERNAL    (IOC_CONFIG_EDGE_DETECT_FALLING)
#define GPIO_CFG_INT_RISING_INTERNAL     (IOC_CONFIG_EDGE_DETECT_RISING)
#define GPIO_CFG_INT_BOTH_EDGES_INTERNAL (IOC_CONFIG_EDGE_DETECT_BOTH)

#define GPIO_CFG_INVERT_OFF_INTERNAL (IOC_CONFIG_INVERT_DISABLED)
#define GPIO_CFG_INVERT_ON_INTERNAL  (IOC_CONFIG_INVERT_ENABLED)

#define GPIO_CFG_HYSTERESIS_OFF_INTERNAL (IOC_CONFIG_HYSTERESIS_DISABLED)
#define GPIO_CFG_HYSTERESIS_ON_INTERNAL  (IOC_CONFIG_HYSTERESIS_ENABLED)

#define GPIO_CFG_SHUTDOWN_WAKE_OFF_INTERNAL  (IOC_CONFIG_SHUTDOWN_WAKEUP_DISABLED)
#define GPIO_CFG_SHUTDOWN_WAKE_HIGH_INTERNAL (IOC_CONFIG_SHUTDOWN_WAKEUP_HIGH)
#define GPIO_CFG_SHUTDOWN_WAKE_LOW_INTERNAL  (IOC_CONFIG_SHUTDOWN_WAKEUP_LOW)

/* Slew limits and drive strength are only supported on specific pins. */
#define GPIO_CFG_SLEW_NORMAL_INTERNAL  (IOC_CONFIG_SLEW_RATE_NORMAL)
#define GPIO_CFG_SLEW_REDUCED_INTERNAL (IOC_CONFIG_SLEW_RATE_REDUCED)

#define GPIO_CFG_DRVSTR_LOW_INTERNAL  (IOC_CONFIG_DRIVE_STRENGTH_3MA)
#define GPIO_CFG_DRVSTR_MED_INTERNAL  (IOC_CONFIG_DRIVE_STRENGTH_6MA)
#define GPIO_CFG_DRVSTR_HIGH_INTERNAL (IOC_CONFIG_DRIVE_STRENGTH_12MA)

/* Configuration values stored in mux bits. Any configuration options not
 * directly handled by IOC need to be stored inside the mux bits (lowest 3 bits
 * on Low Power F4 devices). These are masked out by GPIO_init(),
 * GPIO_setConfig(), GPIO_setConfigAndMux() and GPIO_getConfig() using
 * GPIOLPF3_CFG_IOC_M.
 */

/* Default output value */
#define GPIO_CFG_OUTPUT_DEFAULT_HIGH_INTERNAL (0x1U)
#define GPIO_CFG_OUTPUT_DEFAULT_LOW_INTERNAL  (0x0U)

/* Whether GPIO hardware should have the output enable bit set for this pin */
#define GPIOLPF4_CFG_PIN_IS_INPUT_INTERNAL  (0x2U)
#define GPIOLPF4_CFG_PIN_IS_OUTPUT_INTERNAL (0x0U)

/* Interrupt enable is in the GPIO module */
#define GPIO_CFG_INT_ENABLE_INTERNAL  (0x4U)
#define GPIO_CFG_INT_DISABLE_INTERNAL (0x0U)

#if GPIOLPF4_CFG_IOC_M & (GPIO_CFG_INT_ENABLE_INTERNAL | GPIOLPF4_CFG_PIN_IS_INPUT_INTERNAL | \
                          GPIO_CFG_OUTPUT_DEFAULT_HIGH_INTERNAL | GPIO_CFG_DO_NOT_CONFIG_INTERNAL)
    #error "Error in GPIOLPF4 implementation: Internally used config bits are overlapping with IOC config bits"
#endif

/*!
 *  @brief Type to store the ID for a specific peripheral function.
 *
 * The value is the input number of the shuffle network.
 *
 * Valid values are the @c IOC_SHUFFLE_NETWORK_PERIPHERAL_FUNCTION_* defines
 * from driverlib. Or if using numeric values directly, any value from @c 0 to
 * <tt>n - 1</tt> (both included), where @c n is the size of the shuffle
 * network.
 */
typedef uint8_t GPIOLPF4_PeripheralFunction;

/*!
 *  @brief Type to store the ID for a specific DIO and port combination.
 *
 * The value is the output number of the shuffle network.
 *
 * Valid values are the @c IOC_SHUFFLE_NETWORK_DIO<x>_SNPF<y> defines
 * from driverlib. Or if using numeric values directly, any value from @c 0 to
 * <tt>n - 1</tt> (both included), where @c n is the size of the shuffle
 * network.
 */
typedef uint8_t GPIOLPF4_DioNumberAndPort;

/*!
 *  @brief Type used to declare a required route in the shuffle network.
 *
 * A route is a mapping between a peripheral function and a DIO number and port.
 */
typedef struct
{
    GPIOLPF4_PeripheralFunction peripheralFunction; /*!< Peripheral Function (input) */
    GPIOLPF4_DioNumberAndPort dioNumberAndPort;     /*!< DIO number and port (output) */
} GPIOLPF4_ShuffleNetworkRoute;

/*!
 *  @brief Struct to hold the routes to be applied to the shuffle network.
 *
 *  This type is is the requirement/specification for the
 *  #GPIOLPF4_computeShuffleNetworkConfig() to compute a configuration of the
 *  type #GPIOLPF4_ShuffleNetworkConfig.
 *
 *  #GPIOLPF4_ShuffleNetworkRoutes.routes must be en exhaustive list of the
 *  shuffle network muxing requirements of the system.
 *
 *  @warning #GPIOLPF4_ShuffleNetworkRoutes.routes must not contain multiple
 *           elements with the same peripheral function or the same DIO number
 *           and port combination. The shuffle network only supports a one to
 *           one mapping.
 *
 */
typedef struct
{
    uint8_t size;                         /*!< Number of routes in the network. That is, the number
                                           * of used up spots in #GPIOLPF4_ShuffleNetworkRoutes.routes
                                           */
    GPIOLPF4_ShuffleNetworkRoute *routes; /*!< Pointer to an array of routes */
} GPIOLPF4_ShuffleNetworkRoutes;

/*!
 *  @brief Structure to hold the configuration of the shuffle network.
 *
 *  This is the result of computing the configuration based on the required
 *  routes of the shuffle network.
 *
 *  The application must define a @c GPIOLPF4_shuffleNetworkConfig symbol of
 *  this type with the initial shuffle network configuration. This is used by
 *  #GPIO_init() to configure the shuffle network at startup. If SysConfig is
 *  used, the @c GPIOLPF4_shuffleNetworkConfig symbol will be defined in
 *  ti_drivers_config.c.
 *
 */
typedef struct
{
    uint32_t configRegs[GPIOLPF4_SHUFFLE_NETWORK_CONFIG_REG_COUNT];
} GPIOLPF4_ShuffleNetworkConfig;

/*!
 *  @brief Computes the configuration of the shuffle network based on the
 *         supplied @c routes object.
 *
 *  @note It is suggested to use a pre-computed shuffle network configuration
 *  instead of this function, if possible. This is because computing the
 *  configuration of the shuffle network is an expensive operation. Both in
 *  terms of time and memory usage. For example, you need to reserve a
 *  considerate amount of memory for @c tmpBuffer, and the stack usage of this
 *  function is also relatively high. The execution time of the function depends
 *  on the device, mainly the shuffle network size, and the required routes. It
 *  is in the order of 0.5 ms.
 *
 *  @param[in]  routes  Pointer to a #GPIOLPF4_ShuffleNetworkRoutes object
 *                      containing routes to be applied to the shuffle network.
 *
 *  @param[in]  tmpBuffer  Pointer to a temporary buffer that will be used to
 *                         compute the configuration of the shuffle network.
 *                         The size of the buffer must be at least
 *                         @c sizeof(GPIOLPF4_ShuffleNetworkRoute) * @c routes->size
 *                         bytes.
 *
 *  @param[out] config  Pointer to a #GPIOLPF4_ShuffleNetworkConfig object that
 *                      the resulting configuration will be stored in.
 *
 *  @return     #GPIO_STATUS_SUCCESS or an error.
 */
int_fast16_t GPIOLPF4_computeShuffleNetworkConfig(const GPIOLPF4_ShuffleNetworkRoutes *routes,
                                                  void *tmpBuffer,
                                                  GPIOLPF4_ShuffleNetworkConfig *config);

/*!
 *  @brief Apply the configuration of the shuffle network to the hardware.
 *
 *  @param[in] config  Pointer to a #GPIOLPF4_ShuffleNetworkConfig object that
 *                     should be applied. This can be either pre-computed or the
 *                     result of #GPIOLPF4_computeShuffleNetworkConfig().
 *
 *  @return     #GPIO_STATUS_SUCCESS or an error.
 */
int_fast16_t GPIOLPF4_applyShuffleNetworkConfig(const GPIOLPF4_ShuffleNetworkConfig *config);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_GPIOLPF4__include */