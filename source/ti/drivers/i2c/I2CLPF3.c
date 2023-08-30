/*
 * Copyright (c) 2022-2023, Texas Instruments Incorporated
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
 *  ======== I2CLPF3.c ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <ti/drivers/i2c/I2CLPF3.h>
#include <ti/drivers/i2c/I2CSupport.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>

#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/SwiP.h>

/* Driverlib header files */
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(driverlib/i2c.h)

/*
 *  =============================== Macros =====================================
 *
 * Specific I2C CMD MACROs that are not defined in I2C.h are defined here. Their
 * equivalent values are taken from the existing MACROs in I2C.h
 *
 */
#ifndef I2C_CONTROLLER_CMD_BURST_RECEIVE_START_NACK
    #define I2C_CONTROLLER_CMD_BURST_RECEIVE_START_NACK I2C_CONTROLLER_CMD_BURST_SEND_START
#endif

#ifndef I2C_CONTROLLER_CMD_BURST_RECEIVE_STOP
    #define I2C_CONTROLLER_CMD_BURST_RECEIVE_STOP I2C_CONTROLLER_CMD_BURST_RECEIVE_ERROR_STOP
#endif

#ifndef I2C_CONTROLLER_CMD_BURST_RECEIVE_CONT_NACK
    #define I2C_CONTROLLER_CMD_BURST_RECEIVE_CONT_NACK I2C_CONTROLLER_CMD_BURST_SEND_CONT
#endif

/* Prototypes */
static void I2CLPF3_blockingCallback(I2C_Handle handle, I2C_Transaction *msg, bool transferStatus);
static inline void I2CLPF3_completeTransfer(I2C_Handle handle);
static void I2CLPF3_hwiFxn(uintptr_t arg);
void I2CLPF3_init(I2C_Handle handle);
static void I2CLPF3_initHw(I2C_Handle handle);
static int I2CLPF3_initIO(I2C_Handle handle, void *pinCfg);
static int I2CLPF3_postNotify(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg);
static void I2CLPF3_swiFxn(uintptr_t arg0, uintptr_t arg1);

/* Default I2C parameters structure */
extern const I2C_Params I2C_defaultParams;

/*!
 *  ======== I2C_close ========
 *  @brief Function to close a given I2C peripheral specified by the
 *         I2C handle.
 *
 *  After calling the close function, the I2C is disabled.
 *
 *  @pre    I2C_open() has to be called first.
 *          Calling context: Task
 *
 *  @param handle An I2C_Handle returned by I2C_open()
 *
 *  @note  The generic I2C API should be used when accessing the I2CLPF3.
 *
 *  @sa     I2C_open(), I2C_close(), I2C_open()
 */
void I2C_close(I2C_Handle handle)
{
    I2CLPF3_Object *object;
    I2CLPF3_HWAttrs const *hwAttrs;

    /* Get the pointer to the object and hwAttrs */
    hwAttrs = handle->hwAttrs;
    object  = handle->object;

    /* Check to see if a I2C transaction is in progress */
    DebugP_assert(object->headPtr == NULL);

    /* Mask I2C interrupts */
    I2CControllerDisableInt(hwAttrs->baseAddr);

    /* Disable the I2C Controller */
    I2CControllerDisable(hwAttrs->baseAddr);

    /* Deallocate pins */
    GPIO_resetConfig(object->sclPin);
    GPIO_resetConfig(object->sdaPin);

    /* Power off the I2C module */
    Power_releaseDependency(hwAttrs->powerMngrId);

    /* Destruct modules used in driver */
    HwiP_destruct(&(object->hwi));
    SwiP_destruct(&(object->swi));
    SemaphoreP_destruct(&(object->mutex));
    if (object->transferMode == I2C_MODE_BLOCKING)
    {
        SemaphoreP_destruct(&(object->transferComplete));
    }

    /* Unregister power post notification object */
    Power_unregisterNotify(&object->i2cPostObj);

    /* Mark the module as available */
    object->isOpen = false;

    return;
}

/*!
 *  ======== I2C_control ========
 *  @brief  Function for setting control parameters of the I2C driver
 *          after it has been opened.
 *
 *  @pre    Function assumes that the handle is not NULL
 *
 *  @note   Currently not in use.
 */
int_fast16_t I2C_control(I2C_Handle handle, uint_fast16_t cmd, void *arg)
{
    /* No implementation */
    return (I2C_STATUS_UNDEFINEDCMD);
}

/*!
 *  ======== I2C_open ========
 *  @brief Function to initialize a given I2C peripheral specified by the
 *         particular handle. The parameter specifies which mode the I2C
 *         will operate.
 *
 *  After calling the open function, the I2C is enabled. If there is no active
 *  I2C transactions, the device can enter standby.
 *
 *  @pre    The I2CLPF3_Config structure must exist and be persistent before this
 *          function can be called. I2CLPF3 has been initialized with I2CLPF3_init().
 *          Calling context: Task
 *
 *  @param  handle   An I2C_Handle
 *
 *  @param  params   Pointer to a parameter block, if NULL it will use default values.
 *
 *  @return A I2C_Handle on success, or a NULL on an error or if it has been
 *          already opened.
 *
 *  @note  The generic I2C API should be used when accessing the I2CLPF3.
 *
 *  @sa     I2C_close(), I2CLPF3_init(), I2C_open(), I2C_init()
 */
I2C_Handle I2C_open(uint_least8_t index, I2C_Params *params)
{
    I2C_Handle handle = NULL;
    union
    {
        HwiP_Params hwiParams;
        SwiP_Params swiParams;
    } paramsUnion;
    uintptr_t key;
    I2CLPF3_Object *object;
    I2CLPF3_HWAttrs const *hwAttrs;

    if (index < I2C_count)
    {
        if (params == NULL)
        {
            params = (I2C_Params *)&I2C_defaultParams;
        }

        handle  = (I2C_Handle) & (I2C_config[index]);
        hwAttrs = handle->hwAttrs;
        object  = handle->object;
    }
    else
    {
        return (NULL);
    }

    /* Check for valid bit rate */
    DebugP_assert(params->bitRate <= I2C_400kHz);

    /* Determine if the device index was already opened */
    key = HwiP_disable();
    if (object->isOpen == true)
    {
        HwiP_restore(key);
        return (NULL);
    }

    /* Mark the handle as being used */
    object->isOpen = true;
    HwiP_restore(key);

    /* Power on the I2C module */
    Power_setDependency(hwAttrs->powerMngrId);

    /* Configure the IOs.*/
    if (I2CLPF3_initIO(handle, params->custom))
    {
        /* Mark the module as available */
        key            = HwiP_disable();
        object->isOpen = false;
        HwiP_restore(key);
        /* Release dependency if open fails */
        Power_releaseDependency(hwAttrs->powerMngrId);
        /* Signal back to application that I2C driver was not successfully opened */
        return (NULL);
    }

    /* Save parameters */
    object->transferMode        = params->transferMode;
    object->transferCallbackFxn = params->transferCallbackFxn;
    object->bitRate             = params->bitRate;

    /* Disable and clear interrupts possible from soft resets */
    I2CControllerDisableInt(hwAttrs->baseAddr);
    I2CControllerClearInt(hwAttrs->baseAddr);

    /* Create Hwi object for this I2C peripheral */
    HwiP_Params_init(&paramsUnion.hwiParams);
    paramsUnion.hwiParams.arg      = (uintptr_t)handle;
    paramsUnion.hwiParams.priority = hwAttrs->intPriority;
    HwiP_construct(&(object->hwi), hwAttrs->intNum, I2CLPF3_hwiFxn, &paramsUnion.hwiParams);

    /* Create Swi object for this I2C peripheral */
    SwiP_Params_init(&(paramsUnion.swiParams));
    paramsUnion.swiParams.arg0     = (uintptr_t)handle;
    paramsUnion.swiParams.priority = hwAttrs->swiPriority;
    SwiP_construct(&(object->swi), I2CLPF3_swiFxn, &(paramsUnion.swiParams));

    /*
     * Create thread safe handles for this I2C peripheral
     * Semaphore to provide exclusive access to the I2C peripheral
     */
    SemaphoreP_constructBinary(&(object->mutex), 1);

    /*
     * Store a callback function that posts the transfer complete
     * semaphore for synchronous mode
     */
    if (object->transferMode == I2C_MODE_BLOCKING)
    {
        /* Semaphore to cause the waiting task to block for the I2C to finish */
        SemaphoreP_constructBinary(&(object->transferComplete), 0);

        /* Store internal callback function */
        object->transferCallbackFxn = I2CLPF3_blockingCallback;
    }
    else
    {
        /* Check to see if a callback function was defined for async mode */
        DebugP_assert(object->transferCallbackFxn != NULL);
    }

    /* Clear the head pointer */
    object->headPtr = NULL;
    object->tailPtr = NULL;

    /* Initialize the I2C hardware module */
    I2CLPF3_initHw(handle);

    /* Register notification functions */
    Power_registerNotify(&object->i2cPostObj,
                         PowerLPF3_AWAKE_STANDBY,
                         (Power_NotifyFxn)I2CLPF3_postNotify,
                         (uint32_t)handle);

    /* Return the address of the handle */
    return (handle);
}

/*
 *  ======== I2CLPF3_hwiFxn ========
 *  Hwi interrupt handler to service the I2C peripheral
 *
 *  The handler is a generic handler for a I2C object.
 */
static void I2CLPF3_hwiFxn(uintptr_t arg)
{
    I2C_Handle handle              = (I2C_Handle)arg;
    I2CLPF3_Object *object         = handle->object;
    I2CLPF3_HWAttrs const *hwAttrs = handle->hwAttrs;
    uint32_t command               = I2C_CCTL_RUN_EN;

    /* Clear the interrupt */
    I2CControllerClearInt(hwAttrs->baseAddr);

    /*
     * Check if the Controller is busy. If busy, the MSTAT is invalid as
     * the controller is still transmitting or receiving. In that case,
     * we should wait for the next interrupt.
     */
    if (I2CControllerBusy(hwAttrs->baseAddr))
    {
        return;
    }

    uint32_t status = HWREG(I2C0_BASE + I2C_O_CSTA);

    /* Current transaction is cancelled */
    if (object->currentTransaction->status == I2C_STATUS_CANCEL)
    {
        I2CControllerCommand(hwAttrs->baseAddr, I2C_CCTL_STOP_EN);
        I2CLPF3_completeTransfer(handle);
        return;
    }

    /* Handle errors. ERR bit is not set if arbitration lost.
     * The I2C peripheral has an issue where the DATACK_N bit
     * is not updated if the previous command sets the ACK bit
     * (Controller automatically ACK's received data). This condition
     * can be detected by the state of writeCount, readCount, and
     * the status register. If the condition is true, don't enter
     * the error-handling block, but carry on reading instead.
     */
    if ((status & (I2C_CSTA_ERR_M | I2C_CSTA_ARBLST_M)) &&
        !(object->writeCount == 0 && ((status & 0x1F) == (I2C_CSTA_ERR_M | I2C_CSTA_DATACKN_M))))
    {
        /* Decode interrupt status */
        if (status & I2C_CSTA_ARBLST_M)
        {
            object->currentTransaction->status = I2C_STATUS_ARB_LOST;
        }
        /*
         * The I2C peripheral has an issue where the first data byte
         * is always transmitted, regardless of the ADDR NACK. Therefore,
         * we should check this error condition first.
         */
        else if (status & I2C_CSTA_ADRACKN_M)
        {
            object->currentTransaction->status = I2C_STATUS_ADDR_NACK;
        }
        else
        {
            /* Last possible bit is the I2C_MSTAT_DATACK_N */
            object->currentTransaction->status = I2C_STATUS_DATA_NACK;
        }

        /*
         * The Low Power F3 I2C peripheral does not have an explicit STOP
         * interrupt bit. Therefore, if an error occurred, we send the STOP
         * bit and complete the transfer immediately.
         */
        I2CControllerCommand(hwAttrs->baseAddr, I2C_CCTL_STOP_EN);
        I2CLPF3_completeTransfer(handle);
    }
    else if (object->writeCount)
    {
        object->writeCount--;

        /* Is there more to transmit */
        if (object->writeCount)
        {
            I2CControllerPutData(hwAttrs->baseAddr, *(object->writeBuf++));
        }
        /* If we need to receive */
        else if (object->readCount)
        {
            /* Place controller in receive mode */
            I2CControllerSetTargetAddr(hwAttrs->baseAddr, object->currentTransaction->targetAddress, true);

            if (object->readCount > 1)
            {
                /* RUN and generate ACK to target */
                command |= I2C_CCTL_ACK_M;
            }

            /* RUN and generate a repeated START */
            command |= I2C_CCTL_START_M;
        }
        else
        {
            /* Send STOP */
            command = I2C_CCTL_STOP_EN;
        }

        I2CControllerCommand(hwAttrs->baseAddr, command);
    }
    else if (object->readCount)
    {
        object->readCount--;

        /* Read data */
        *(object->readBuf++) = I2CControllerGetData(hwAttrs->baseAddr);

        if (object->readCount > 1)
        {
            /* Send ACK and RUN */
            command |= I2C_CCTL_ACK_EN;
        }
        else if (object->readCount < 1)
        {
            /* Send STOP */
            command = I2C_CCTL_STOP_EN;
        }
        else
        {
            /* Send RUN */
        }

        I2CControllerCommand(hwAttrs->baseAddr, command);
    }
    else
    {
        I2CControllerCommand(hwAttrs->baseAddr, I2C_CCTL_STOP_EN);
        object->currentTransaction->status = I2C_STATUS_SUCCESS;
        I2CLPF3_completeTransfer(handle);
    }

    return;
}

/*
 *  ======== I2C_setClockTimeout ========
 */
int_fast16_t I2C_setClockTimeout(I2C_Handle handle, uint32_t timeout)
{
    return (I2C_STATUS_ERROR);
}

/*
 *  ======== I2CLPF3_swiFxn ========
 *  SWI interrupt handler to service the I2C peripheral
 *
 *  Takes care of cleanup and the callback in SWI context after an I2C transfer
 */
static void I2CLPF3_swiFxn(uintptr_t arg0, uintptr_t arg1)
{
    I2C_Handle handle      = (I2C_Handle)arg0;
    I2CLPF3_Object *object = handle->object;
    int_fast16_t status;

    /*
     *  If this current transaction was canceled, we need to cancel
     *  any queued transactions
     */
    if (object->currentTransaction->status == I2C_STATUS_CANCEL)
    {
        /*
         * Allow callback to run. If in CALLBACK mode, the application
         * may initiate a transfer in the callback which will call
         * primeTransfer().
         */
        object->transferCallbackFxn(handle, object->currentTransaction, false);
        object->currentTransaction->status = I2C_STATUS_CANCEL;

        /* release the constraint to disallow standby */
        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

        /* also dequeue and call the transfer callbacks for any queued transfers */
        while (object->headPtr != object->tailPtr)
        {
            object->headPtr         = object->headPtr->nextPtr;
            object->headPtr->status = I2C_STATUS_CANCEL;
            object->transferCallbackFxn(handle, object->headPtr, false);
            object->headPtr->status = I2C_STATUS_CANCEL;
        }

        /* clean up object */
        object->currentTransaction = NULL;
        object->headPtr            = NULL;
        object->tailPtr            = NULL;
    }
    else if (object->currentTransaction->status == I2C_STATUS_TIMEOUT)
    {
        /*
         * This can only occur in BLOCKING mode. No need to call the blocking
         * callback as the Semaphore has already timed out.
         */
        object->headPtr = NULL;
        object->tailPtr = NULL;

        /* Release standby disallow constraint. */
        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
    }
    /* See if we need to process any other transactions */
    else if (object->headPtr == object->tailPtr)
    {

        /* No other transactions need to occur */
        object->headPtr = NULL;
        object->tailPtr = NULL;

        /*
         * Allow callback to run. If in CALLBACK mode, the application
         * may initiate a transfer in the callback which will call
         * primeTransfer().
         */
        object->transferCallbackFxn(handle,
                                    object->currentTransaction,
                                    (object->currentTransaction->status == I2C_STATUS_SUCCESS));

        /* Release standby disallow constraint. */
        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
    }
    else
    {
        /* Another transfer needs to take place */
        object->headPtr = object->headPtr->nextPtr;

        /*
         * Allow application callback to run. The application may
         * initiate a transfer in the callback which will add an
         * additional transfer to the queue.
         */
        object->transferCallbackFxn(handle,
                                    object->currentTransaction,
                                    (object->currentTransaction->status == I2C_STATUS_SUCCESS));

        status = I2CSupport_primeTransfer(handle, object->headPtr);

        /* Call back now if not able to start transfer */
        if (status != I2C_STATUS_SUCCESS)
        {
            /* Transaction status set in primeTransfer() */
            SwiP_post(&(object->swi));
        }
    }
}

/*
 *  ======== I2CSupport_controllerFinish ========
 */
void I2CSupport_controllerFinish(I2C_HWAttrs const *hwAttrs)
{
    /* Asynchronously generate a STOP condition. */
    I2CControllerCommand(hwAttrs->baseAddr, I2C_CCTL_STOP_EN);
}

/*
 *  ======== I2CSupport_powerRelConstraint =======
 */
void I2CSupport_powerRelConstraint(void)
{
    /* Release standby disallow constraint. */
    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*
 *  ======== I2CSupport_powerSetConstraint =======
 */
void I2CSupport_powerSetConstraint(void)
{
    /* Set standby disallow constraint. */
    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*
 *  ======== I2CSupport_primeTransfer =======
 */
int_fast16_t I2CSupport_primeTransfer(I2C_Handle handle, I2C_Transaction *transaction)
{
    I2CLPF3_Object *object         = handle->object;
    I2CLPF3_HWAttrs const *hwAttrs = handle->hwAttrs;
    int_fast16_t status            = I2C_STATUS_SUCCESS;

    /* Store the new internal counters and pointers */
    object->currentTransaction = transaction;

    object->writeBuf   = transaction->writeBuf;
    object->writeCount = transaction->writeCount;
    object->readBuf    = transaction->readBuf;
    object->readCount  = transaction->readCount;

    /*
     * Transaction is incomplete unless the stop condition occurs AND
     * all bytes have been sent and received. This condition is checked
     * in the hardware interrupt.
     */
    object->currentTransaction->status = I2C_STATUS_INCOMPLETE;

    /* Determine if the bus is in use by another I2C Controller */
    if (I2CControllerBusBusy(hwAttrs->baseAddr))
    {
        transaction->status = I2C_STATUS_BUS_BUSY;
        status              = I2C_STATUS_BUS_BUSY;
    }
    /* Start transfer in Transmit */
    else if (object->writeCount)
    {
        I2CControllerEnableInt(hwAttrs->baseAddr);

        /* Specify target address and transmit mode */
        I2CControllerSetTargetAddr(hwAttrs->baseAddr, object->currentTransaction->targetAddress, false);

        I2CControllerPutData(hwAttrs->baseAddr, *((object->writeBuf)++));
        I2CControllerCommand(hwAttrs->baseAddr, I2C_CONTROLLER_CMD_BURST_SEND_START);
    }
    else
    {
        I2CControllerEnableInt(hwAttrs->baseAddr);

        /* Specify target address and receive mode */
        I2CControllerSetTargetAddr(hwAttrs->baseAddr, object->currentTransaction->targetAddress, true);

        if (object->readCount == 1)
        {
            /* Send START, read 1 data byte, and NACK */
            I2CControllerCommand(hwAttrs->baseAddr, I2C_CONTROLLER_CMD_BURST_RECEIVE_START_NACK);
        }
        else
        {
            /* Start the I2C transfer in controller receive mode */
            I2CControllerCommand(hwAttrs->baseAddr, I2C_CONTROLLER_CMD_BURST_RECEIVE_START);
        }
    }

    return (status);
}

/*
 *  ======== I2CLPF3_completeTransfer ========
 *  This function may be invoked in the context of the HWI.
 */
static inline void I2CLPF3_completeTransfer(I2C_Handle handle)
{
    I2CLPF3_Object *object         = handle->object;
    I2CLPF3_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Disable and clear any interrupts */
    I2CControllerDisableInt(hwAttrs->baseAddr);
    I2CControllerClearInt(hwAttrs->baseAddr);

    SwiP_post(&(object->swi));
}

/*
 *  ======== I2CLPF3_blockingCallback ========
 */
static void I2CLPF3_blockingCallback(I2C_Handle handle, I2C_Transaction *msg, bool transferStatus)
{
    I2CLPF3_Object *object = handle->object;

    SemaphoreP_post(&(object->transferComplete));
}

/*
 *  ======== I2CLPF3_initHw ========
 *  This functions initializes the I2C hardware module.
 *
 *  @pre    Function assumes that the I2C handle is pointing to a hardware
 *          module which has already been opened.
 */
static void I2CLPF3_initHw(I2C_Handle handle)
{
    I2CLPF3_Object *object         = handle->object;
    I2CLPF3_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Disable and clear interrupts */
    I2CControllerDisableInt(hwAttrs->baseAddr);
    I2CControllerClearInt(hwAttrs->baseAddr);

    /* Set the I2C configuration */
    I2CControllerInitExpClk(hwAttrs->baseAddr, object->bitRate > I2C_100kHz);

    /* Enable the I2C Controller for operation */
    I2CControllerEnable(hwAttrs->baseAddr);
}

/*
 *  ======== I2CLPF3_initIO ========
 *  This functions initializes the I2C IOs.
 *
 *  @pre    Function assumes that the I2C handle is pointing to a hardware
 *          module which has already been opened.
 */
static int I2CLPF3_initIO(I2C_Handle handle, void *pinCfg)
{
    I2CLPF3_Object *object;
    I2CLPF3_HWAttrs const *hwAttrs;

    /* Get the pointer to the object and hwAttrs */
    object  = handle->object;
    hwAttrs = handle->hwAttrs;

    /* If the pinCfg pointer is NULL, use hwAttrs pins */
    if (pinCfg == NULL)
    {
        object->sdaPin = hwAttrs->sdaPin;
        object->sclPin = hwAttrs->sclPin;
    }
    else
    {
        object->sdaPin = ((I2CLPF3_I2CPinCfg *)pinCfg)->pinSDA;
        object->sclPin = ((I2CLPF3_I2CPinCfg *)pinCfg)->pinSCL;
    }

    /* Configure I2C pins SDA and SCL and set their muxes */
    GPIO_setConfigAndMux(object->sdaPin, GPIO_CFG_OUT_OD_PU | GPIO_CFG_OUT_HIGH, hwAttrs->sdaPinMux);
    GPIO_setConfigAndMux(object->sclPin, GPIO_CFG_OUT_OD_PU | GPIO_CFG_OUT_HIGH, hwAttrs->sclPinMux);

    return I2C_STATUS_SUCCESS;
}

/*
 *  ======== I2CLPF3_postNotify ========
 *  This functions is called to notify the I2C driver of an ongoing transition
 *  out of sleep mode.
 *
 *  @pre    Function assumes that the I2C handle (clientArg) is pointing to a
 *          hardware module which has already been opened.
 */
static int I2CLPF3_postNotify(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg)
{
    /* reconfigure the hardware if returning from sleep*/
    if (eventType == PowerLPF3_AWAKE_STANDBY)
    {
        I2CLPF3_initHw((I2C_Handle)clientArg);
    }

    return (Power_NOTIFYDONE);
}
