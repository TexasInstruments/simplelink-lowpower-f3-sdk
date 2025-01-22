/*
 * Copyright (c) 2022-2024, Texas Instruments Incorporated
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

/* !****************************************************************************
 *  @file       ComparatorLPF3LP.h
 *  @brief      Comparator driver implementation for the Low-Power Comparator on
 *              the Low Power F3 family devices
 *
 *  This Comparator driver is meant to operate the Low-Power Comparator
 *  peripheral for Low Power F3 devices.
 *
 *  Refer to @ref Comparator.h for a complete description of APIs & example use.
 *
 *******************************************************************************
 */

#ifndef ti_drivers_comparator_ComparatorLPF3LP__include
#define ti_drivers_comparator_ComparatorLPF3LP__include

#include <stdint.h>
#include <stdbool.h>

#include <ti/drivers/Comparator.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

/* Driverlib header files */
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/lpcmp.h)

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief  ComparatorLPF3LP Negative input channel These fields are intended to
 *  serve as the options for the @ref Comparator.h API negative inputs.
 *
 *  The Low-Power comparators on the Low Power F3 devices support four negative
 *  input sources:
 *  - internal voltage VDDD
 *  - internal voltage VDDS
 *  - External pin PAD_A2
 *  - External pin PAD_A3
 *
 *  \note A fixed device-specific mapping exists between PAD_An and DIOn. Such
 *  mapping is automatically handled when using SysConfig. In case SysConfig is
 *  not used by the application, the mapping needs to be manually verified with
 *  the device Technical Reference Manual.
 */
typedef enum
{
    ComparatorLPF3LP_NEG_INPUT_VDDD = LPCMP_NEG_INPUT_VDDD,
    ComparatorLPF3LP_NEG_INPUT_VDDS = LPCMP_NEG_INPUT_VDDS,
    ComparatorLPF3LP_NEG_INPUT_A2   = LPCMP_NEG_INPUT_A2,
    ComparatorLPF3LP_NEG_INPUT_A3   = LPCMP_NEG_INPUT_A3
} ComparatorLPF3LP_NegativeInputChannel;

/*!
 *  @brief  ComparatorLPF3LP Positive input channel
 *
 *  These fields are intended to serve as the options for the @ref Comparator.h
 *  API Positive inputs.
 *
 *  The Low-Power comparator on the Low Power F3 devices supports four positive
 *  input sources:
 *  - External pin PAD_A1
 *  - External pin PAD_A2
 *  - External pin PAD_A3
 *  - Internal voltage VDDS
 *
 *  \note A fixed device-specific mapping exists between PAD_An and DIOn. Such
 *  mapping is automatically handled when using SysConfig. In case SysConfig is
 *  not used by the application, the mapping needs to be manually verified with
 *  the device Technical Reference Manual.
 */
typedef enum
{
    ComparatorLPF3LP_POS_INPUT_A1   = LPCMP_POS_INPUT_A1,
    ComparatorLPF3LP_POS_INPUT_A2   = LPCMP_POS_INPUT_A2,
    ComparatorLPF3LP_POS_INPUT_A3   = LPCMP_POS_INPUT_A3,
    ComparatorLPF3LP_POS_INPUT_VDDS = LPCMP_POS_INPUT_VDDS
} ComparatorLPF3LP_PositiveInputChannel;

/*!
 *  @brief  ComparatorLPF3LP Voltage divider path
 *
 *  These fields are intended to serve as options for the
 *  @ref ComparatorLPF3LP.h API voltage divider path
 *
 *  The Low-Power comparators on the Low Power F3 devices support internal
 *  voltage division on either the negative or positive input.
 */
typedef enum
{
    ComparatorLPF3LP_V_DIV_PATH_NEG = LPCMP_DIVISION_PATH_N,
    ComparatorLPF3LP_V_DIV_PATH_POS = LPCMP_DIVISION_PATH_P
} ComparatorLPF3LP_VoltageDividerPath;

/*!
 *  @brief  ComparatorLPF3LP Voltage divider factor
 *
 *  These fields are intended to serve as options for the
 *  @ref ComparatorLPF3LP.h API voltage divider factors
 *
 *  The Low-Power comparator on the Low Power F3 devices supports internal
 *  voltage division on either the negative or positive input using one of the
 *  following factors:
 *  - 1/1 voltage division factor
 *  - 3/4 voltage division factor
 *  - 1/2 voltage division factor
 *  - 1/3 voltage division factor
 *  - 1/4 voltage division factor
 */
typedef enum
{
    ComparatorLPF3LP_V_DIV_FACTOR_1_1 = LPCMP_DIVISION_FACTOR_1_1,
    ComparatorLPF3LP_V_DIV_FACTOR_3_4 = LPCMP_DIVISION_FACTOR_3_4,
    ComparatorLPF3LP_V_DIV_FACTOR_1_2 = LPCMP_DIVISION_FACTOR_1_2,
    ComparatorLPF3LP_V_DIV_FACTOR_1_3 = LPCMP_DIVISION_FACTOR_1_3,
    ComparatorLPF3LP_V_DIV_FACTOR_1_4 = LPCMP_DIVISION_FACTOR_1_4
} ComparatorLPF3LP_VoltageDividerFactor;

/*!
 *  @brief  ComparatorLPF3LP Hardware attributes
 *
 *  A sample structure is shown below:
 *  @code
 *  const ComparatorLPF3LP_HWAttrs ComparatorLPF3LP_hwAttrs[CONFIG_COMPARATOR_COUNT] = {
 *     {
 *         .intNum = INT_CPUIRQ0,
 *         .intPriority = (~0),
 *         .intMux = EVTSVT_CPUIRQ0SEL_PUBID_AON_LPMCMP_IRQ,
 *         .positiveInputPin = CONFIG_GPIO_COMPARATOR_0_PLUS,
 *         .positiveInputPinMux = GPIO_MUX_PORTCFG_PFUNC6,
 *         .positiveInputChannel = ComparatorLPF3LP_POS_INPUT_A1,
 *         .negativeInputPin = GPIO_INVALID_INDEX,
 *         .negativeInputPinMux = GPIO_MUX_GPIO_INTERNAL,
 *         .negativeInputChannel = ComparatorLPF3LP_NEG_INPUT_VDDS,
 *         .voltageDividerPath = ComparatorLPF3LP_V_DIV_PATH_NEG,
 *         .voltageDividerFactor = ComparatorLPF3LP_V_DIV_FACTOR_1_2,
 *     },
 *
 *  };
 *  @endcode
 */
typedef struct
{
    uint32_t intNum;                                            /*!< Device-specific interrupt number */
    uint32_t intPriority;                                       /*!< Device-specific interrupt priority */
    uint32_t intMux;                                            /*!< Device-specific interrupt muxing */
    ComparatorLPF3LP_PositiveInputChannel positiveInputChannel; /*!< Internal muxing on the positive channel */
    ComparatorLPF3LP_NegativeInputChannel negativeInputChannel; /*!< Internal muxing on the negative channel */
    ComparatorLPF3LP_VoltageDividerPath voltageDividerPath;     /*!< Voltage divider path */
    ComparatorLPF3LP_VoltageDividerFactor voltageDividerFactor; /*!< Voltage divider factor */
    uint8_t positiveInputPin;                                   /*!< Positive input pin index */
    uint8_t positiveInputPinMux;                                /*!< Positive input pin muxing */
    uint8_t negativeInputPin;                                   /*!< Negative input pin index */
    uint8_t negativeInputPinMux;                                /*!< Negative input pin muxing */
} ComparatorLPF3LP_HWAttrs;

/*!
 *  @brief  ComparatorLPF3LP Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct
{
    HwiP_Struct hwi;                    /*!< Hwi used for handling comparator events */
    Comparator_CallBackFxn callbackFxn; /*!< User callback function */
    Comparator_Trigger trigger;         /*!< Condition on which comparator triggers an event */
    bool isOpen;                        /*!< Determine if comparator is open */
    bool isRunning;                     /*!< Determine if comparator instance is already running */
} ComparatorLPF3LP_Object;

/*!
 *  @brief  Comparator function table for the ComparatorLPF3LP implementation
 */
extern const Comparator_FxnTable ComparatorLPF3LP_fxnTable;

#ifdef __cplusplus
}
#endif

#endif /* COMPARATORLPF3LP_H_ */
