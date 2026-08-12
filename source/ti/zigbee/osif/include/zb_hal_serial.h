/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2025, Texas Instruments Incorporated
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

#ifndef ZB_HAL_SERIAL_H
#define ZB_HAL_SERIAL_H 1

#if defined ZB_HAVE_SERIAL || defined USE_ASSERT

#if defined ZB_TRACE_OVER_USART && defined ZB_TRACE_LEVEL
#define ZB_SERIAL_FOR_TRACE
#endif

#include "zb_ringbuffer.h"

ZB_RING_BUFFER_DECLARE(serial_iobuf, zb_uint8_t, 256u);

/**
   Type of callback called by serial interface when it receives a single byte.

   @param byte - received byte
 */
typedef void (*zb_osif_uart_byte_received_cb_t)(zb_uint8_t byte);

typedef struct zb_serial_ctx_s
{
  zb_uint8_t inited;
  volatile zb_uint8_t tx_in_progress; /* if set, we are waiting for tx complete int */
  serial_iobuf_t tx_buf;
  zb_byte_array_t *p_tx_buf;
  zb_ushort_t tx_buf_cap;
  zb_osif_uart_byte_received_cb_t byte_received_cb;
}
zb_serial_ctx_t;

#if defined ZB_ZGPD_ROLE
extern zb_serial_ctx_t zb_serial_ctx;
#define SER_CTX() zb_serial_ctx
#else //defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
typedef struct zb_io_ctx_s
{
  zb_serial_ctx_t serial_ctx;
}
zb_io_ctx_t;

#define ZB_HAVE_IOCTX
#endif // ZB_ZGPD_ROLE

void zb_osif_serial_init(void);

extern void zb_osif_serial_put_bytes(const zb_uint8_t *buf, zb_short_t len);
#endif // ZB_HAVE_SERIAL

#endif /* ZB_HAL_SERIAL_H */
