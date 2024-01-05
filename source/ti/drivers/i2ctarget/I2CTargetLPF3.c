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

/*
 *  ======== I2CTargetLPF3.c ========
 */

#include <stdint.h>

#include <ti/drivers/i2ctarget/I2CTargetLPF3.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

/* Driverlib header files */
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(driverlib/i2c.h)

/* Prototypes */
static void I2CTargetLPF3_hwiFxn(uintptr_t arg);
static void I2CTargetLPF3_configHw(I2CTarget_Handle handle);
static int_fast16_t I2CTargetLPF3_initIO(I2CTarget_Handle handle);

/* Default I2C parameters structure */
extern const I2CTarget_Params I2CTarget_defaultParams;

/*!
 *  ======== I2CTarget_open ========
 */
I2CTarget_Handle I2CTarget_open(uint_least8_t index, I2CTarget_Params *params)
{
    I2CTarget_Handle handle = NULL;
    HwiP_Params hwiParams;
    uintptr_t key;
    I2CTargetLPF3_Object *object;
    I2CTargetLPF3_HWAttrs const *hwAttrs;

    if (index < I2CTarget_count)
    {
        if (params == NULL)
        {
            params = (I2CTarget_Params *)&I2CTarget_defaultParams;
        }
        handle  = (I2CTarget_Handle) & (I2CTarget_config[index]);
        hwAttrs = handle->hwAttrs;
        object  = handle->object;
    }
    else
    {
        return NULL;
    }

    /* Callback function must be defined */
    if (params->eventCallbackFxn == NULL)
    {
        return NULL;
    }

    /* Only 7-bit address supported by this driver implementation */
    if (params->targetAddress >> 7)
    {
        return NULL;
    }

    /* Determine if the device index was already opened */
    key = HwiP_disable();
    if (object->isOpen == true)
    {
        HwiP_restore(key);
        return NULL;
    }

    /* Mark the handle as being used */
    object->isOpen = true;
    HwiP_restore(key);

    /* Power on the I2C module */
    Power_setDependency(hwAttrs->powerMngrId);

    /* Configure the IOs.*/
    if (I2CTargetLPF3_initIO(handle) != I2CTarget_STATUS_SUCCESS)
    {
        /* Mark the module as available */
        object->isOpen = false;
        /* Release dependency if open fails */
        Power_releaseDependency(hwAttrs->powerMngrId);
        /* Signal back to application that I2C driver was not successfully opened */
        return NULL;
    }

    /* Save parameters */
    object->eventCallbackFxn = params->eventCallbackFxn;
    object->targetAddress    = params->targetAddress;

    /* Set driver state */
    object->currentState = I2CTarget_State_STOPPED;

    /* Configure HW based on driver state */
    I2CTargetLPF3_configHw(handle);

    /* Create Hwi object for this I2C peripheral */
    HwiP_Params_init(&hwiParams);
    hwiParams.arg      = (uintptr_t)handle;
    hwiParams.priority = hwAttrs->intPriority;
    HwiP_construct(&(object->hwi), hwAttrs->intNum, I2CTargetLPF3_hwiFxn, &hwiParams);

    /* Create thread safe handles for this I2C peripheral
     * Semaphore to provide exclusive access to the I2C peripheral
     */
    SemaphoreP_constructBinary(&(object->mutex), 1);

    /* Return the address of the handle */
    return handle;
}

/*!
 *  @brief  Start I2C Target driver listening on I2C bus.
 *
 *  @param  handle  A I2CTarget_Handle returned from I2CTarget_open
 */
void I2CTarget_start(I2CTarget_Handle handle)
{
    I2CTargetLPF3_Object *object;

    /* Get the pointer to the object */
    object = handle->object;

    object->currentState = I2CTarget_State_IDLE;

    /* Configure HW based on driver state */
    I2CTargetLPF3_configHw(handle);

    /* Set standby disallow constraint. */
    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*!
 *  @brief  Stop I2C Target driver from listening on I2C bus.
 *
 *  @param  handle  A I2CTarget_Handle returned from I2CTarget_open
 */
void I2CTarget_stop(I2CTarget_Handle handle)
{
    I2CTargetLPF3_Object *object;

    /* Get the pointer to the object */
    object = handle->object;

    object->currentState = I2CTarget_State_STOPPED;

    /* Configure HW based on driver state */
    I2CTargetLPF3_configHw(handle);

    /* Release power constraint */
    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*!
 *  ======== I2CTarget_close ========
 */
void I2CTarget_close(I2CTarget_Handle handle)
{
    I2CTargetLPF3_Object *object;
    I2CTargetLPF3_HWAttrs const *hwAttrs;

    /* Get the pointer to the object and hwAttrs */
    hwAttrs = handle->hwAttrs;
    object  = handle->object;

    /* Stop I2C Target operation */
    if (object->currentState != I2CTarget_State_STOPPED)
    {
        I2CTarget_stop(handle);
    }

    /* Disable the I2C Target */
    I2CTargetDisable(hwAttrs->baseAddr);

    /* Power off the I2C module */
    Power_releaseDependency(hwAttrs->powerMngrId);

    /* Destruct modules used in driver */
    HwiP_destruct(&(object->hwi));
    SemaphoreP_destruct(&(object->mutex));

    /* Unregister power post notification object */
    Power_unregisterNotify(&object->i2cPostObj);

    /* Mark the module as available */
    object->currentState = I2CTarget_State_STOPPED;
    object->isOpen       = false;

    return;
}

/*
 *  ======== I2CTargetLPF3_hwiFxn ========
 *  Hwi interrupt handler to service the I2C peripheral
 *
 *  The handler is a generic handler for a I2C object.
 */
static void I2CTargetLPF3_hwiFxn(uintptr_t arg)
{
    I2CTarget_Handle handle              = (I2CTarget_Handle)arg;
    I2CTargetLPF3_Object *object         = handle->object;
    I2CTargetLPF3_HWAttrs const *hwAttrs = handle->hwAttrs;
    uint8_t dataByte;
    I2CTarget_Event event;

    /* Get interrupt status and clear */
    uint32_t intFlags = I2CTargetIntStatus(hwAttrs->baseAddr, true);
    I2CTargetClearInt(hwAttrs->baseAddr, I2CTARGET_INT_FLAGS);

    /* Check if controller has written or requested data */
    uint32_t status = I2CTargetStatus(hwAttrs->baseAddr);

    /* Controller will write to target (target-receiver) */
    if (status & I2C_TARGET_ACT_RREQ)
    {
        object->currentState = I2CTarget_State_RECEIVE;
        object->eventCallbackFxn(handle, I2CTarget_Event_WRITE_REQUESTED, NULL);
    }

    /* Controller has written first byte to target (target-receiver) */
    if (status & I2C_TARGET_ACT_RREQ_FBR)
    {
        /* Read data byte from I2C peripheral */
        dataByte = I2CTargetGetData(hwAttrs->baseAddr);

        object->currentState = I2CTarget_State_RECEIVE;
        object->eventCallbackFxn(handle, I2CTarget_Event_WRITE_RECEIVED, &dataByte);
    }

    /* Controller wants to read from target (target-transmitter) */
    if (status & I2C_TARGET_ACT_TREQ)
    {
        /* Use internal state machine to differentiate between
         * - the first byte (I2CTarget_Event_READ_REQUESTED), and
         * - subsequent bytes (I2CTarget_Event_READ_PROCESSED).
         */
        event = I2CTarget_Event_READ_REQUESTED;
        if (object->currentState == I2CTarget_State_TRANSMIT)
        {
            event = I2CTarget_Event_READ_PROCESSED;
        }

        object->currentState = I2CTarget_State_TRANSMIT;

        /* Call callback function */
        object->eventCallbackFxn(handle, event, &dataByte);

        /* Write byte to I2C peripheral */
        I2CTargetPutData(hwAttrs->baseAddr, dataByte);
    }

    if (intFlags & I2C_TARGET_INT_STOP)
    {
        /* Stop condition received. */
        object->currentState = I2CTarget_State_IDLE;
        object->eventCallbackFxn(handle, I2CTarget_Event_STOP, NULL);
    }
}

/*
 *  ======== I2CTargetLPF3_configHw ========
 *  This functions configures the I2C hardware module.
 *
 *  @pre    Function assumes that the I2C handle is pointing to a hardware
 *          module which has already been opened.
 */
static void I2CTargetLPF3_configHw(I2CTarget_Handle handle)
{
    I2CTargetLPF3_Object *object         = handle->object;
    I2CTargetLPF3_HWAttrs const *hwAttrs = handle->hwAttrs;

    if (object->currentState == I2CTarget_State_STOPPED)
    {
        /* Driver is stopped/inactive. Disable interrupts */
        I2CTargetDisableInt(hwAttrs->baseAddr, I2CTARGET_INT_FLAGS);

        /* Disable I2C module target mode */
        I2CTargetDisable(hwAttrs->baseAddr);

        /* Clear interrupt flags */
        I2CTargetClearInt(hwAttrs->baseAddr, I2CTARGET_INT_FLAGS);
    }
    else
    {
        /* Driver is started/active. Clear flags and enable interrupts. */
        I2CTargetClearInt(hwAttrs->baseAddr, I2CTARGET_INT_FLAGS);
        I2CTargetEnableInt(hwAttrs->baseAddr, I2CTARGET_INT_FLAGS);

        /* Initialize target mode */
        I2CTargetInit(hwAttrs->baseAddr, (uint8_t)object->targetAddress);
    }
}

/*
 *  ======== I2CTargetLPF3_initIO ========
 *  This functions initializes the I2C IOs.
 *
 *  @pre    Function assumes that the I2C handle is pointing to a hardware
 *          module which has already been opened.
 */
static int_fast16_t I2CTargetLPF3_initIO(I2CTarget_Handle handle)
{
    I2CTargetLPF3_HWAttrs const *hwAttrs;

    /* Get the pointer to hwAttrs */
    hwAttrs = handle->hwAttrs;

    /* Configure I2C pins SDA and SCL and set their muxes */
    GPIO_setConfigAndMux(hwAttrs->sdaPin, GPIO_CFG_OUT_OD_PU | GPIO_CFG_OUT_HIGH, hwAttrs->sdaPinMux);
    GPIO_setConfigAndMux(hwAttrs->sclPin, GPIO_CFG_OUT_OD_PU | GPIO_CFG_OUT_HIGH, hwAttrs->sclPinMux);

    return I2CTarget_STATUS_SUCCESS;
}
