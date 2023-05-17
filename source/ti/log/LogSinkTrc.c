/*
 * Copyright (c) 2021 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== LogSinkTrc.c ========
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#include "Log.h"
#include "LogSinkTrc.h"

#ifdef NO_DPL
#include <cm0ikmcu.h>
#include <trc_regs.h>
#else
#include <ti/drivers/dpl/HwiP.h>
#ifdef _TCBUILD
#include <trc_regs.h>
#else
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/trc_regs.h)
#endif
#endif


#define DBGID_SYSTICK           1   ///< Debug ID reserved for system tick trace packets
#define DBGID_ENDSIM            2   ///< Debug ID reserved for end of simulation directive. Takes one parameter.

static void writeToTrc0(uint32_t narg,
                        uint32_t header)
{
    uint32_t trcCmd;
    int32_t chInd;
#ifndef NO_DPL
    uint32_t key;
#endif
    /* Find channel index (channel number minus 1) from header */
    chInd = ((header >> 24) & 0x03) - 1;
    /* Possible values are now -1, 0, 1, 2. -1 is illegal. */
    if (chInd < 0)
    {
        chInd = 0;
    }    
    trcCmd = ((header & 0x03FC) << 6);
    
    while (1)
    {
        /* disable interrupts */
#ifdef NO_DPL
        __disable_irq();
#else
        key = HwiP_disable();
#endif        
        if ((SP_TRC_CH1CMD[chInd] & TRC_CH1CMD_PKTHDR_BM) == 0)
        {
            /* Channel ready, transmit packet */
            SP_TRC_CH1CMD[chInd] = trcCmd;
            /* enable interrupts */
#ifdef NO_DPL
            __enable_irq();
#else            
            HwiP_restore(key);
#endif
            break;
        }
        else
        {
            /* enable interrupts */
#ifdef NO_DPL
            __enable_irq();
#else            
            HwiP_restore(key);
#endif
        }
    }
}

static void writeToTrc12(uint32_t narg,
                         uint32_t header,
                         uint32_t arg)
{
    uint32_t trcCmd;
    int32_t chInd;
#ifndef NO_DPL
    uint32_t key;
#endif
    /* Find channel index (channel number minus 1) from header */
    chInd = ((header >> 24) & 0x03) - 1;
    /* Possible values are now -1, 0, 1, 2. -1 is illegal. */
    if (chInd < 0)
    {
        chInd = 0;
    }    
    trcCmd = ((header & 0x03FC) << 6) | narg;
    
    while (1)
    {
        /* disable interrupts */
#ifdef NO_DPL
        __disable_irq();
#else
        key = HwiP_disable();
#endif        
        if ((SP_TRC_CH1CMD[chInd] & TRC_CH1CMD_PKTHDR_BM) == 0)
        {
            SP_TRC_CH1PAR01[chInd] = arg;
            /* Channel ready, transmit packet */
            SP_TRC_CH1CMD[chInd] = trcCmd;
            /* enable interrupts */
#ifdef NO_DPL
            __enable_irq();
#else            
            HwiP_restore(key);
#endif
            break;
        }
        else
        {
            /* enable interrupts */
#ifdef NO_DPL
            __enable_irq();
#else            
            HwiP_restore(key);
#endif
        }
    }
}


static void writeToTrc34(uint32_t narg,
                         uint32_t header,
                         uint32_t arg01,
                         uint32_t arg23)
{
    uint32_t trcCmd;
    int32_t chInd;
#ifndef NO_DPL
    uint32_t key;
#endif
    /* Find channel index (channel number minus 1) from header */
    chInd = ((header >> 24) & 0x03) - 1;
    /* Possible values are now -1, 0, 1, 2. -1 is illegal. */
    if (chInd < 0)
    {
        chInd = 0;
    }    
    trcCmd = ((header & 0x03FC) << 6) | narg;
    
    while (1)
    {
        /* disable interrupts */
#ifdef NO_DPL
        __disable_irq();
#else
        key = HwiP_disable();
#endif        
        if ((SP_TRC_CH1CMD[chInd] & TRC_CH1CMD_PKTHDR_BM) == 0)
        {
            SP_TRC_CH1PAR01[chInd] = arg01;
            SP_TRC_CH1PAR23[chInd] = arg23;
            /* Channel ready, transmit packet */
            SP_TRC_CH1CMD[chInd] = trcCmd;
            /* enable interrupts */
#ifdef NO_DPL
            __enable_irq();
#else            
            HwiP_restore(key);
#endif
            break;
        }
        else
        {
            /* enable interrupts */
#ifdef NO_DPL
            __enable_irq();
#else            
            HwiP_restore(key);
#endif
        }
    }
}

/*
 *  ======== LogSinkTrcShort_printf ========
 */
void  ti_log_LogSinkTrcShort_printf(const Log_Module *handle,
                                    uint32_t header,
                                    uint32_t numArgs,
                                    ...)
{
    va_list argptr;
    uint32_t arg01;
    uint32_t arg23;
    va_start(argptr, numArgs);

    switch (numArgs) {
    case 0:
        writeToTrc0(0, header);
        break;

    case 1:
        arg01 = va_arg(argptr, uintptr_t) & 0xFFFF;
        writeToTrc12(1, header, arg01);
        break;

    case 2:
        arg01 = va_arg(argptr, uintptr_t) & 0xFFFF;
        arg01 |= va_arg(argptr, uintptr_t) << 16;
        writeToTrc12(2, header, arg01);
        break;
        
    case 3:
        arg01 = va_arg(argptr, uintptr_t) & 0xFFFF;
        arg01 |= va_arg(argptr, uintptr_t) << 16;
        arg23 = va_arg(argptr, uintptr_t) & 0xFFFF;
        writeToTrc34(3, header, arg01, arg23);
        break;
        
    case 4:
    default:
        arg01 = va_arg(argptr, uintptr_t) & 0xFFFF;
        arg01 |= va_arg(argptr, uintptr_t) << 16;
        arg23 = va_arg(argptr, uintptr_t) & 0xFFFF;
        arg23 |= va_arg(argptr, uintptr_t) << 16;
        writeToTrc34(4, header, arg01, arg23);
        break;
    }
    va_end (argptr);
}
    


/*
 *  ======== LogSinkTrcLong_printf ========
 */
void  ti_log_LogSinkTrcLong_printf(const Log_Module *handle,
                                   uint32_t header,
                                   uint32_t numArgs,
                                   ...)
{
    va_list argptr;
    uint32_t arg0;
    uint32_t arg1;
    va_start(argptr, numArgs);

    switch (numArgs) {
    case 0:
        writeToTrc0(0, header);
        break;

    case 1:
        arg0 = va_arg(argptr, uintptr_t);
        writeToTrc12(2, header, arg0);
        break;

    case 2:
    default:
        arg0 = va_arg(argptr, uintptr_t);
        arg1 = va_arg(argptr, uintptr_t);
        writeToTrc34(4, header, arg0, arg1);
        break;
    }
    va_end (argptr);
}



/*
 *  ======== LogSinkTrc_buf ========
 */
void ti_log_LogSinkTrc_buf(const Log_Module *handle,
                           uint32_t header,
                           uint8_t *data,
                           size_t size)
{
    /* This function is not implemented due to lack of HW support */
}


/*
 *  ======== LogSinkTrc_endsim ========
 */
void LogSinkTrc_endsim(uint8_t channel, 
                       uint16_t code)
{
    uint32_t header;

    header = (channel << 24) | (DBGID_ENDSIM << 2);

    writeToTrc12(1, header, code);
}


/*
 *  ======== LogSinkTrc_timetick ========
 */
void LogSinkTrc_timetick(uint8_t channel)
{
    uint32_t header;

    header = (channel << 24) | (DBGID_SYSTICK << 2);

    writeToTrc0(0, header);
}
