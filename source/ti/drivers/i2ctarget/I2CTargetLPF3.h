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
/*!****************************************************************************
 *  @file       I2CTargetLPF3.h
 *
 *  @brief      I2CTarget driver implementation for a Low Power F3 device I2C
 *              target.
 *
 *  @note       All calls should go through the generic I2CTarget API.
 *
 *  # Driver Include #
 *  The I2CTarget header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/I2CTarget.h>
 *  #include <ti/drivers/i2c/I2CTargetLPF3.h>
 *  @endcode
 *
 *  Refer to @ref I2CTarget.h for a complete description of APIs.
 *
 *  # Overview
 *  The generic I2CTarget API is normally used in application code,
 *  for example, @ref I2CTarget_open(). Do not use APIs in this header file
 *  directly. The board file will define the device specific config, and casting
 *  in the general API will ensure that the correct device specific functions
 *  are called.
 *
 *  ## General Behavior
 *  Before using the I2C in LPF3:
 *   - The I2CTarget driver is initialized by calling I2CTarget_init().
 *   - The I2C HW is configured and system dependencies are declared (e.g. IOs,
 *     power, etc.) by calling I2CTarget_open().
 *   - The I2CTarget driver starts acting on I2C bus activities after calling
 *     @ref I2CTarget_start().
 *
 *  ## Power Management
 *  The I2CTargetLPF3 driver sets a power constraint when @ref I2CTarget_start()
 *  is called to keep the device out of standby; so when all tasks are blocked, the device will
 *  enter idle mode instead of standby. When the application calls @ref I2CTarget_stop(), the
 *  the power constraint to prohibit standby is released.
 *  The following statements are valid:
 *    - After I2CTarget_open() call: I2C peripheral is enabled in target mode, but the device can
 *      enter standby. The device will need to be woken up (for example on GPIO or timer events)
 *      before I2C bus activity can be processed.
 *    - After I2CTarget_start() call: I2C events will trigger the user callback function. The device
 *      might enter idle, but not standby.
 *    - After I2C_stop() call: Same as after I2CTarget_open() call.
 *    - After I2C_close() call: I2C peripheral is disabled.
 *
 * ## Supported Functions
 *  @note All calls should go through the generic API.
 *
 *  @anchor ti_drivers_I2CTARGETLPF3_USE_CASES
 *  # Use Cases
 *  See generic API for examples.
 *
 ******************************************************************************
 */

#ifndef ti_drivers_i2c_I2CTARGETLPF3__include
#define ti_drivers_i2c_I2CTARGETLPF3__include

#include <stdint.h>
#include <stdbool.h>

#include <ti/drivers/I2CTarget.h>
#include <ti/drivers/Power.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief  I2CTargetLPF3 Hardware attributes
 *
 *  The baseAddr and intNum fields define the base address and the interrupt
 *  number of the I2C peripheral.  These values are passed to driverlib APIs
 *  and therefore must be populated by driverlib macro definitions.  These
 *  macros are found in the header files:
 *      - inc/hw_memmap.h
 *      - inc/hw_ints.h
 *
 *  The powerMngrId is the Power driver resource ID for the I2C peripheral.
 *  These macros are defined in the device specific Power include file.

 *  intPriority is the I2C peripheral's interrupt priority. This value is passed
 *  unmodified to @ref HwiP_construct().
 *
 *  sdaPin and sclPin define the SDA and SCL pin mapping, respectively. These
 *  are typically defined with a macro in a header file, which maps to an
 *  IOID. For example, when using SysConfig, ti_drivers_config.h defines
 *  CONFIG_GPIO_I2C_0_SDA to be DIO 0.
 *
 *  sdaPinMux and sclPinMux are values drawn from the GPIO driver. They indicate
 *  the function for that specific DIO pin that select I2C mode.
 *
 *  A sample structure is shown below:
 *  @code
 *  const I2CTargetLPF3_HWAttrs I2CTargetLPF3_hwAttrs[CONFIG_I2CTARGET_COUNT] = {
 *      {
 *          .baseAddr    = I2C0_BASE,
 *          .powerMngrId = PowerLPF3_PERIPH_I2C0,
 *          .intNum      = INT_I2C0_IRQ,
 *          .intPriority = (~0),
 *          .sclPin      = CONFIG_GPIO_I2C_0_SCL,
 *          .sclPinMux   = GPIO_MUX_PORTCFG_PFUNC5,
 *          .sdaPin      = CONFIG_GPIO_I2C_0_SDA,
 *          .sdaPinMux   = GPIO_MUX_PORTCFG_PFUNC2
 *      },
 *  };
 *  @endcode
 */
typedef struct
{
    I2CTARGET_BASE_HWATTRS

    /* I2C peripheral's Power driver ID */
    PowerLPF3_Resource powerMngrId;

    /* SDA pin index and mux */
    uint8_t sdaPin;
    uint8_t sdaPinMux;
    /* SCL pin index and mux */
    uint8_t sclPin;
    uint8_t sclPinMux;
} I2CTargetLPF3_HWAttrs;

/*!
 *  @brief      I2CTargetLPF3 Object.
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct
{
    I2CTARGET_BASE_OBJECT

    /* I2C power notification */
    void *i2cPostFxn;
    Power_NotifyObj i2cPostObj;
} I2CTargetLPF3_Object;

/*!
 *  @cond NODOC
 *  I2C Target Low Power F3 interrupt flags used to trigger Hwi callback.
 */
#define I2CTARGET_INT_FLAGS (I2C_TARGET_INT_DATA | I2C_TARGET_INT_STOP)
/*! @endcond */

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_i2c_I2CTARGETLPF3__include */
