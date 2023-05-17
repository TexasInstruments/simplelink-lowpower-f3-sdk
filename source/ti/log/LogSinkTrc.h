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
 *  ======== LogSinkTrc.h ========
 */

#ifndef ti_log_LogSinkTrc__include
#define ti_log_LogSinkTrc__include

#include <stdint.h>
#include "Log.h"

#if defined (__cplusplus)
extern "C" {
#if 0
} // Fix indentation in emacs...
#endif
#endif

/*
 *  ======== LogSinkTrc_printf ========
 */
extern void  ti_log_LogSinkTrcShort_printf(const Log_Module *handle,
                                           uint32_t header,
                                           uint32_t numArgs,
                                           ...);
extern void  ti_log_LogSinkTrcLong_printf(const Log_Module *handle,
                                          uint32_t header,
                                          uint32_t numArgs,
                                          ...);

/*
 *  ======== LogSinkTrc_buf ========
 */
extern void ti_log_LogSinkTrc_buf(const Log_Module *handle,
                                  uint32_t header,
                                  uint8_t *data,
                                  size_t size);


/*
 *  ======== LogSinkTrc_endsim ========
 */
extern void LogSinkTrc_endsim(uint8_t channel, 
                              uint16_t code);


/*
 *  ======== LogSinkTrc_timetick ========
 */
extern void LogSinkTrc_timetick(uint8_t channel);


/*
 * Helpers to define/use instance. Trc is a singleton so no arguments are taken.
 */
#define Log_SINK_TRC_SHORT_DEFINE()
#define Log_SINK_TRC_SHORT_USE()
#define Log_SINK_TRC_LONG_DEFINE()
#define Log_SINK_TRC_LONG_USE()
#define Log_MODULE_INIT_SINK_TRC_SHORT(_levels)                       \
    {                                                                 \
        .sinkConfig = NULL,                                           \
        .printf = ti_log_LogSinkTrcShort_printf,                      \
        .buf = ti_log_LogSinkTrc_buf,                                 \
        .levels = _levels,                                            \
    }
#define Log_MODULE_INIT_SINK_TRC_LONG(_levels)          \
    {                                                               \
        .sinkConfig = NULL,                                         \
        .printf = ti_log_LogSinkTrcLong_printf,                     \
        .buf = ti_log_LogSinkTrc_buf,                               \
        .levels = _levels,                                          \
    }




#if defined (__cplusplus)
#if 0
{ // Fix indentation in emacs...
#endif
}
#endif

#endif /* ti_log_LogSinkTrc__include */
