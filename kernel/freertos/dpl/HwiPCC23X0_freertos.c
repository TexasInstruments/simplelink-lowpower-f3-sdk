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
/*
 *  ======== HwiPCC23X0_freertos ========
 */

#include <stdbool.h>
#include <stdlib.h>

#include <ti/drivers/dpl/HwiP.h>
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

/* Driverlib includes */
#include <ti/devices/DeviceFamily.h>
#ifdef DeviceFamily_CC23X0R2
    #include DeviceFamily_constructPath(cmsis/cc23x0r2.h)
#elif (defined(DeviceFamily_CC23X0R5) || defined(DeviceFamily_CC23X0R22))
    #include DeviceFamily_constructPath(cmsis/cc23x0r5.h)
#else
    #error "Unsupported DeviceFamily specified!"
#endif
#include DeviceFamily_constructPath(cmsis/core/core_cm0plus.h)
#include DeviceFamily_constructPath(driverlib/interrupt.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)

/* interrupt reserved for SwiP */
int HwiP_swiPIntNum = INT_CPUIRQ1;

/* The name of this struct and the names of its members are used by ROV */
typedef struct _HwiP_Obj
{
    uint32_t intNum;
    HwiP_Fxn fxn;
    uintptr_t arg;
} HwiP_Obj;

/* The name of this table is used by ROV */
static HwiP_Obj *HwiP_dispatchTable[NUM_INTERRUPTS] = {0};

void HwiP_dispatch(void);

/*
 *  ======== HwiP_clearInterrupt ========
 */
void HwiP_clearInterrupt(int interruptNum)
{
    IntClearPend((uint32_t)interruptNum);
}

/*
 *  ======== HwiP_construct ========
 */
HwiP_Handle HwiP_construct(HwiP_Struct *handle, int interruptNum, HwiP_Fxn hwiFxn, HwiP_Params *params)
{
    HwiP_Params defaultParams;
    HwiP_Obj *obj = (HwiP_Obj *)handle;

    if (handle != NULL)
    {
        if (params == NULL)
        {
            params = &defaultParams;
            HwiP_Params_init(&defaultParams);
        }

        if ((params->priority & 0xFF) == 0xFF)
        {
            /* SwiP_freertos.c uses INT_PRI_LEVEL3 as its scheduler. No one else
             * may use that priority level. CC23X0 only has two priority bits.
             * So level 3 is the lowest.
             */
            params->priority = INT_PRI_LEVEL2;
        }

        if (interruptNum != HwiP_swiPIntNum && params->priority == INT_PRI_LEVEL3)
        {
            handle = NULL;
        }
        else
        {
            HwiP_dispatchTable[interruptNum] = obj;
            obj->fxn                         = hwiFxn;
            obj->arg                         = params->arg;
            obj->intNum                      = (uint32_t)interruptNum;

            IntRegister((uint32_t)interruptNum, HwiP_dispatch);
            IntSetPriority((uint32_t)interruptNum, (uint8_t)params->priority);

            if (params->enableInt)
            {
                IntEnable((uint32_t)interruptNum);
            }
        }
    }

    return ((HwiP_Handle)handle);
}

/*
 *  ======== HwiP_create ========
 */
HwiP_Handle HwiP_create(int interruptNum, HwiP_Fxn hwiFxn, HwiP_Params *params)
{
    HwiP_Handle handle;
    HwiP_Handle retHandle;

    handle = (HwiP_Handle)malloc(sizeof(HwiP_Obj));

    /*
     * Even though HwiP_construct will check handle for NULL and not do
     * anything, we should check it here so that we can know afterwards
     * that construct failed with non-NULL pointer and that we need to
     * free the handle.
     */
    if (handle != NULL)
    {
        retHandle = HwiP_construct((HwiP_Struct *)handle, interruptNum, hwiFxn, params);
        if (retHandle == NULL)
        {
            free(handle);
            handle = NULL;
        }
    }

    return (handle);
}

/*
 *  ======== HwiP_delete ========
 */
void HwiP_delete(HwiP_Handle handle)
{
    HwiP_destruct((HwiP_Struct *)handle);

    free(handle);
}

/*
 *  ======== HwiP_destruct ========
 */
void HwiP_destruct(HwiP_Struct *handle)
{
    HwiP_Obj *obj = (HwiP_Obj *)handle;

    IntDisable(obj->intNum);
    IntUnregister(obj->intNum);
}

/*
 *  ======== HwiP_enable ========
 */
void HwiP_enable(void)
{
    __set_PRIMASK(0);
}

/*
 *  ======== HwiP_disable ========
 */
uintptr_t HwiP_disable(void)
{
    uintptr_t key;

    /*
     *  If we're not in ISR context, use the FreeRTOS macro, since
     *  it handles nesting.
     */
    if ((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) == 0)
    {
        /* Cannot be called from an ISR! */
        portENTER_CRITICAL();
        key = 0;
    }
    else
    {
        key = __get_PRIMASK();
        __set_PRIMASK(1);
    }

    return (key);
}

/*
 *  ======== HwiP_disableInterrupt ========
 */
void HwiP_disableInterrupt(int interruptNum)
{
    IntDisable((uint32_t)interruptNum);
}

/*
 *  ======== HwiP_dispatch ========
 */
void HwiP_dispatch(void)
{
    uint32_t intNum = (SCB->ICSR & 0x000000ff);
    HwiP_Obj *obj   = HwiP_dispatchTable[intNum];
    if (obj)
    {
        (obj->fxn)(obj->arg);
        taskYIELD();
    }
}

/*
 *  ======== HwiP_enableInterrupt ========
 */
void HwiP_enableInterrupt(int interruptNum)
{
    IntEnable(interruptNum);
}

/*
 *  ======== HwiP_interruptsEnabled ========
 */
bool HwiP_interruptsEnabled(void)
{
    return (__get_PRIMASK() == 0L);
}

/*

 *  ======== HwiP_inISR ========
 */
bool HwiP_inISR(void)
{
    bool stat;

    if ((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) == 0)
    {
        /* Not currently in an ISR */
        stat = false;
    }
    else
    {
        stat = true;
    }

    return (stat);
}

/*
 *  ======== HwiP_inSwi ========
 */
bool HwiP_inSwi(void)
{
    uint32_t intNum = SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk;
    if (intNum == HwiP_swiPIntNum)
    {
        /* Currently in a Swi */
        return (true);
    }

    return (false);
}

/*
 *  ======== HwiP_Params_init ========
 */
void HwiP_Params_init(HwiP_Params *params)
{
    if (params != NULL)
    {
        params->arg       = 0;
        params->priority  = (~0);
        params->enableInt = true;
    }
}

/*
 *  ======== HwiP_plug ========
 */
void HwiP_plug(int interruptNum, void *fxn)
{
    IntRegister((uint32_t)interruptNum, (void (*)(void))fxn);
}

/*
 *  ======== HwiP_post ========
 */
void HwiP_post(int interruptNum)
{
    IntSetPend(interruptNum);
}

/*
 *  ======== HwiP_restore ========
 */
void HwiP_restore(uintptr_t key)
{
    if ((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) == 0)
    {
        /* Cannot be called from an ISR! */
        portEXIT_CRITICAL();
    }
    else
    {
        __set_PRIMASK(key);
    }
}

/*
 *  ======== HwiP_setFunc ========
 */
void HwiP_setFunc(HwiP_Handle hwiP, HwiP_Fxn fxn, uintptr_t arg)
{
    HwiP_Obj *obj = (HwiP_Obj *)hwiP;

    uintptr_t key = HwiP_disable();

    obj->fxn = fxn;
    obj->arg = arg;

    HwiP_restore(key);
}

/*
 * ======== HwiP_setPri ========
 */
void HwiP_setPriority(int interruptNum, uint32_t priority)
{
    /*
     * On CM0+, dynamically changing priorities is not allowed. In order to
     * change priority of an interrupt, after it already has been enabled, the
     * following must be done:
     * - Disable the interrupt whose priority needs to be updated
     * - Set the priority to be at the desired priority level
     * - Re-enable the interrupt.
     *
     * These steps are all handled by IntSetPriority().
     *
     * HwiP_disable/enable here serves the purpose of deferring the handling of
     * user interrupts until the priority of interruptNum has been changed. This
     * prevents another interrupt from changing the global state potentially
     * altering the outcome of this function.
     */

    uintptr_t key = HwiP_disable();
    IntSetPriority((uint32_t)interruptNum, (uint8_t)priority);
    HwiP_restore(key);
}

#if (configSUPPORT_STATIC_ALLOCATION == 1)
/*
 *  ======== HwiP_staticObjectSize ========
 */
size_t HwiP_staticObjectSize(void)
{
    return (sizeof(HwiP_Obj));
}
#endif
