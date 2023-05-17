/******************************************************************************

 @file  icall_cc23x0.c

 @brief CC2650 specific ICall function implementation

 Group: WCS, LPC, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2013-2023, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/
#include "icall_platform.h"
#include <icall_cc23x0_defs.h>

#include <stdint.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC23X0.h>

#ifdef __IAR_SYSTEMS_ICC__
#include <intrinsics.h>
#endif

/* temporary until next kernel define it in the power header file*/
#ifndef CC23X0
extern bool PowerCC23X0_isStableXOSC_HF(void);
#endif
/**
 * @internal Flag offset where dependencies starts.
 * Note that the value may change if ICallCC26xxDefs.h changes.
 * When the flags change, not only this module but the client code
 * also changes resulting into requirement to update both images.
 */
#define ICALLCC2650_PWR_CFG_D_OFFSET                   5

/**
 * @internal Invalid configuration flag mask.
 * Note that the value may change if ICallCC26xxDefs.h changes.
 * When the flags change, not only this module but the client code
 * also changes resulting into requirement to update both images.
 */
#define ICALLCC2650_PWR_CFG_NMASK                      0xff000000l

/** @internal a function called from here but not exposed in PowerCC26XX.h */
//extern bool PowerCC26XX_isStableXOSC_HF(void);

/** @internal power activity counter */
static uint_least8_t ICallPlatform_pwrActivityCount = 0;

/** @internal power constraint dependency bitmap */
typedef uint_least32_t ICallPlatform_pwrBitmap_t;

/** @internal power activity counter action configuration */
static ICallPlatform_pwrBitmap_t ICallPlatform_pwrCfgACAction =
  ( ICALL_PWR_C_SD_DISALLOW |
    ICALL_PWR_C_SB_DISALLOW );

/** @internal power notify handler */
static int_least32_t
ICallPlatform_pwrNotify(unsigned int eventType, uintptr_t eventArg,
                        uintptr_t clientArg)
{
  ICall_PwrNotifyData *data = (ICall_PwrNotifyData *) clientArg;
  ICall_PwrTransition transition;
  switch (eventType)
  {
  case PowerLPF3_AWAKE_STANDBY:
    transition = ICALL_PWR_AWAKE_FROM_STANDBY;
    break;
  case PowerLPF3_ENTERING_STANDBY:
    transition = ICALL_PWR_ENTER_STANDBY;
    break;
  case PowerLPF3_ENTERING_SHUTDOWN:
    transition = ICALL_PWR_ENTER_SHUTDOWN;
    break;
  default:
    ICALL_HOOK_ABORT_FUNC();
    break;
  }
  data->_private(transition, data);
  return Power_NOTIFYDONE;
}

/**
 * @internal
 * Sets constraints and dependencies.
 *
 * @param bitmap  bitmap of constraint and dependency flags.
 */
static void ICallPlatform_pwrRequireImpl(uint_fast32_t bitmap)
{
  for (;;)
  {
#ifdef __IAR_SYSTEMS_ICC__
    char pos = __CLZ(bitmap);
#elif __GNUC__
    int pos = __builtin_clz(bitmap);
#else
    int pos = __clz(bitmap);
#endif
    if (pos == 32)
      break;
    pos = 31 - pos;
    bitmap ^= 1ul << pos;
    if (pos < ICALLCC2650_PWR_CFG_D_OFFSET)
    {
      Power_setConstraint((uint_least32_t) (pos));
    }
    else
    {
      Power_setDependency(pos - ICALLCC2650_PWR_CFG_D_OFFSET);
    }
  }
}

/**
 * @internal
 * Releases constraints and dependencies.
 *
 * @param bitmap  bitmap of constraint and dependency flags.
 */
static void ICallPlatform_pwrDispenseImpl(uint_fast32_t bitmap)
{
  for (;;)
  {
#ifdef __IAR_SYSTEMS_ICC__
    char pos = __CLZ(bitmap);
#elif __GNUC__
    int pos = __builtin_clz(bitmap);
#else
    int pos = __clz(bitmap);
#endif
    if (pos == 32)
      break;
    pos = 31 - pos;
    bitmap ^= 1ul << pos;
    if (pos < ICALLCC2650_PWR_CFG_D_OFFSET)
    {
      Power_releaseConstraint((uint_least32_t) (pos));
    }
    else
    {
      Power_releaseDependency(pos - ICALLCC2650_PWR_CFG_D_OFFSET);
    }
  }
}

/* See ICallPlatform.h for description */
ICall_Errno
ICallPlatform_pwrUpdActivityCounter(ICall_PwrUpdActivityCounterArgs *args)
{
  args->pwrRequired = TRUE;
  if (args->incFlag)
  {
    /* Increment counter */
    ICall_CSState state = ICall_enterCSImpl();
    if (ICallPlatform_pwrActivityCount == 255)
    {
      /* Counter value overflow */
      ICall_leaveCSImpl(state);
      return ICALL_ERRNO_OVERFLOW;
    }
    ICallPlatform_pwrActivityCount++;
    if (ICallPlatform_pwrActivityCount == 1)
    {
      /* Needs to stay in power active state.
       * Set constraints and dependencies as configured. */
      ICallPlatform_pwrRequireImpl(ICallPlatform_pwrCfgACAction);
    }
    ICall_leaveCSImpl(state);
  }
  else
  {
    /* Decrement counter */
    ICall_CSState state = ICall_enterCSImpl();
    if (ICallPlatform_pwrActivityCount == 0)
    {
      /* Counter value underflow */
      ICall_leaveCSImpl(state);
      return ICALL_ERRNO_UNDERFLOW;
    }
    ICallPlatform_pwrActivityCount--;
    if (ICallPlatform_pwrActivityCount == 0)
    {
      /* No activity.
       * Lift the constraints and dependencies as configured. */
      ICall_leaveCSImpl(state);
      ICallPlatform_pwrDispenseImpl(ICallPlatform_pwrCfgACAction);
      args->pwrRequired = FALSE;
    }
    else
    {
      ICall_leaveCSImpl(state);
    }
  }
  return ICALL_ERRNO_SUCCESS;
}

/* See ICallPlatform.h for description */
ICall_Errno
ICallPlatform_pwrConfigACAction(ICall_PwrBitmapArgs *args)
{
  ICall_CSState state;

  if (args->bitmap & ICALLCC2650_PWR_CFG_NMASK)
  {
    return ICALL_ERRNO_INVALID_PARAMETER;
  }

  state = ICall_enterCSImpl();

  if (ICallPlatform_pwrActivityCount > 0)
  {
    /* Release previous constraints */
    ICallPlatform_pwrDispenseImpl(ICallPlatform_pwrCfgACAction);
    /* Set new constraints */
    ICallPlatform_pwrRequireImpl(args->bitmap);
  }
  ICallPlatform_pwrCfgACAction = args->bitmap;

  ICall_leaveCSImpl(state);
  return ICALL_ERRNO_SUCCESS;
}

/* See ICallPlatform.h for description */
ICall_Errno
ICallPlatform_pwrRequire(ICall_PwrBitmapArgs *args)
{
  if (args->bitmap & ICALLCC2650_PWR_CFG_NMASK)
  {
    return ICALL_ERRNO_INVALID_PARAMETER;
  }

  ICallPlatform_pwrRequireImpl(args->bitmap);

  return ICALL_ERRNO_SUCCESS;
}

/* See ICallPlatform.h for description */
ICall_Errno
ICallPlatform_pwrDispense(ICall_PwrBitmapArgs *args)
{
  if (args->bitmap & ICALLCC2650_PWR_CFG_NMASK)
  {
    return ICALL_ERRNO_INVALID_PARAMETER;
  }

  ICallPlatform_pwrDispenseImpl(args->bitmap);

  return ICALL_ERRNO_SUCCESS;
}

/* See ICallPlatform.h for description */
ICall_Errno
ICallPlatform_pwrRegisterNotify(ICall_PwrRegisterNotifyArgs *args)
{
  Power_NotifyObj *obj[1];
  size_t i;

  static const uint_least32_t events[1] =
  {
     ((uint_least32_t) PowerLPF3_ENTERING_STANDBY |
     (uint_least32_t) PowerLPF3_ENTERING_SHUTDOWN |
     (uint_least32_t) PowerLPF3_AWAKE_STANDBY)
  };

  args->obj->_private = args->fn;
  for (i = 0; i < sizeof(obj)/sizeof(obj[0]); i++)
  {
    obj[i] =  (Power_NotifyObj *) ICall_mallocImpl(sizeof(Power_NotifyObj));
    if (!obj[i])
    {
      size_t j;
      for (j = 0; j < i; j++)
      {
        ICall_freeImpl(obj[j]);
      }
      return ICALL_ERRNO_NO_RESOURCE;
    }
  }
  for (i = 0; i < sizeof(obj)/sizeof(obj[0]); i++)
  {
    Power_registerNotify(obj[i], events[i],
                       (Power_NotifyFxn)ICallPlatform_pwrNotify,
                         (uintptr_t) args->obj);
  }
  return ICALL_ERRNO_SUCCESS;
}

/* See ICallPlatform.h for description */
ICall_Errno
ICallPlatform_pwrIsStableXOSCHF(ICall_GetBoolArgs* args)
{
#ifndef CC23X0
  args->value = PowerCC23X0_isStableXOSC_HF();
#endif
  return ICALL_ERRNO_SUCCESS;
}

/* See ICallPlatform.h for description */
ICall_Errno
ICallPlatform_pwrSwitchXOSCHF(ICall_FuncArgsHdr* args)
{
#ifndef CC23X0
  PowerCC23X0_switchXOSC_HF();
#endif
  return ICALL_ERRNO_SUCCESS;
}

/* See ICallPlatform.h for description */
ICall_Errno
ICallPlatform_pwrGetTransitionState(ICall_PwrGetTransitionStateArgs *args)
{
  args->state = Power_getTransitionState();
  return ICALL_ERRNO_SUCCESS;
}

/* See ICallPlatform.h for description */
ICall_Errno
ICallPlatform_pwrGetXOSCStartupTime(ICall_PwrGetXOSCStartupTimeArgs * args)
{
  /*??????????????
   * #ifdef CC23X0 ??????????
   * */
#ifndef CC23X0
  args->value = PowerCC23X0_getXoscStartupTime(args->timeUntilWakeupInMs);
#endif
  return ICALL_ERRNO_SUCCESS;
}
