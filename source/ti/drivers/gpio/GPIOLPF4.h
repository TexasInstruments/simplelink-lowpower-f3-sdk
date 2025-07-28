/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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
 *              Power F3 devices with the shuffle network from the newer F4
 *              devices.
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
 *  The definitions in this file should not be used directly. All GPIO_CFG
 *  macros should be used as-is from GPIO.h.
 *
 *  There are no additional configuration values or platform-specific
 *  functions for GPIOLPF4.
 */

#ifndef ti_drivers_GPIOLPF4__include
#define ti_drivers_GPIOLPF4__include

#include <ti/drivers/GPIO.h>

#ifdef __cplusplus
extern "C" {
#endif

/* We don't define this value on purpose - any unsupported values will cause a
 * compile-time error. If your compiler tells you that this macro is missing,
 * you are trying to use an unsupported option.
 *
 * See below for which options are unsupported.
 */
#undef GPIOLPF4_CFG_OPTION_NOT_SUPPORTED

/* TODO: Use appropriate values for GPIOLPF4 */
#define GPIO_CFG_DO_NOT_CONFIG_INTERNAL       (0U)
#define GPIO_CFG_INT_LOW_INTERNAL             (0U)
#define GPIO_CFG_INT_HIGH_INTERNAL            (0U)
#define GPIO_CFG_NO_DIR_INTERNAL              (0U)
#define GPIO_CFG_INPUT_INTERNAL               (0U)
#define GPIO_CFG_OUTPUT_INTERNAL              (0U)
#define GPIO_CFG_OUTPUT_OPEN_DRAIN_INTERNAL   (0U)
#define GPIO_CFG_OUT_OPEN_SOURCE_INTERNAL     (0U)
#define GPIO_CFG_PULL_NONE_INTERNAL           (0U)
#define GPIO_CFG_PULL_UP_INTERNAL             (0U)
#define GPIO_CFG_PULL_DOWN_INTERNAL           (0U)
#define GPIO_CFG_INT_NONE_INTERNAL            (0U)
#define GPIO_CFG_INT_FALLING_INTERNAL         (0U)
#define GPIO_CFG_INT_RISING_INTERNAL          (0U)
#define GPIO_CFG_INT_BOTH_EDGES_INTERNAL      (0U)
#define GPIO_CFG_INVERT_OFF_INTERNAL          (0U)
#define GPIO_CFG_INVERT_ON_INTERNAL           (0U)
#define GPIO_CFG_HYSTERESIS_OFF_INTERNAL      (0U)
#define GPIO_CFG_HYSTERESIS_ON_INTERNAL       (0U)
#define GPIO_CFG_SHUTDOWN_WAKE_OFF_INTERNAL   (0U)
#define GPIO_CFG_SHUTDOWN_WAKE_HIGH_INTERNAL  (0U)
#define GPIO_CFG_SHUTDOWN_WAKE_LOW_INTERNAL   (0U)
#define GPIO_CFG_SLEW_NORMAL_INTERNAL         (0U)
#define GPIO_CFG_SLEW_REDUCED_INTERNAL        (0U)
#define GPIO_CFG_DRVSTR_LOW_INTERNAL          (0U)
#define GPIO_CFG_DRVSTR_MED_INTERNAL          (0U)
#define GPIO_CFG_DRVSTR_HIGH_INTERNAL         (0U)
#define GPIO_CFG_OUTPUT_DEFAULT_HIGH_INTERNAL (0U)
#define GPIO_CFG_OUTPUT_DEFAULT_LOW_INTERNAL  (0U)
#define GPIOLPF3_CFG_PIN_IS_INPUT_INTERNAL    (0U)
#define GPIOLPF3_CFG_PIN_IS_OUTPUT_INTERNAL   (0U)
#define GPIO_CFG_INT_ENABLE_INTERNAL          (0U)
#define GPIO_CFG_INT_DISABLE_INTERNAL         (0U)

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_GPIOLPF4__include */