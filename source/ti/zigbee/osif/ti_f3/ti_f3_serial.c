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

#define ZB_TRACE_FILE_ID 30004

#include "zb_common.h"

#include <ti/drivers/UART2.h>
#include <ti/drivers/uart2/UART2LPF3.h>

#include <ti/drivers/dpl/HwiP.h>

#include "ti_drivers_config.h"

#if defined ZB_HAVE_SERIAL || defined USE_ASSERT

zb_serial_ctx_t zb_serial_ctx;

static UART2_Handle gs_uart;
static uint8_t gs_read_buffer;
static volatile zb_bool_t gs_uart_inited = ZB_FALSE;
static void uart_write_callback(UART2_Handle handle, void *ptr, size_t size, void *userArg, int_fast16_t status);

static void uart_read_callback(UART2_Handle handle, void *ptr, size_t size, void *userArg, int_fast16_t status)
{
  ZVUNUSED(handle);
  ZVUNUSED(ptr);
  ZVUNUSED(size);

#if defined(NCP_MODE) && defined(ZB_TRACE_LEVEL)
  if (!g_trace_level)
  {
    return;
  }
#endif

  if (SER_CTX().byte_received_cb != NULL)
  {
    SER_CTX().byte_received_cb(gs_read_buffer);
  }

  UART2_read(gs_uart, &gs_read_buffer, 1, NULL);
}

static void uart_init()
{
  UART2_Params uart_params;

  if (gs_uart_inited)
  {
    return;
  }

  gs_uart_inited = ZB_TRUE;

  UART2_Params_init(&uart_params);
#ifdef SYNC_WRITE
  uart_params.writeMode = UART2_Mode_BLOCKING;
  uart_params.writeCallback = NULL;
#else
  uart_params.writeMode = UART2_Mode_CALLBACK;
  uart_params.writeCallback = &uart_write_callback;
#endif
  uart_params.readMode = UART2_Mode_CALLBACK;
  uart_params.readCallback = &uart_read_callback;
  uart_params.readReturnMode = UART2_ReadReturnMode_FULL;
  uart_params.baudRate = ZB_UART_BAUD_RATE;

  gs_uart = UART2_open(CONFIG_UART2_ZB, &uart_params);

  if (gs_uart == NULL)
  {
    ZB_ABORT();
  }

  /*
   * Asynchronous UART reading:
   * uart_read_callback will be called once a symbol has arrived,
   * then UART_read must be called again to fetch another symbol.
   *
   * The alternative approach requires another task hanging on
   * an infinite loop with UART_read, but this would cause additional
   * memory consumption.
   */
  UART2_read(gs_uart, &gs_read_buffer, 1, NULL);
}

void zb_osif_serial_init(void)
{
  zb_uint8_t enable_trace;
  zb_uint8_t enable_traf_dump;

#ifdef ZB_TRACE_LEVEL
  enable_trace = g_trace_level;
#else
  enable_trace = 0;
#endif
#ifdef ZB_TRAFFIC_DUMP_ON
  enable_traf_dump = g_traf_dump;
#else
  enable_traf_dump = 0;
#endif

#if !defined ZB_MACSPLIT_TRANSPORT_SERIAL && !defined ZB_HAVE_SERIAL
  if (enable_trace || enable_traf_dump)
#else
  ZVUNUSED(enable_trace);
  ZVUNUSED(enable_traf_dump);
#endif
  {
    SER_CTX().p_tx_buf = (zb_byte_array_t*) &SER_CTX().tx_buf;
    SER_CTX().tx_buf_cap = ZB_RING_BUFFER_CAPACITY(&SER_CTX().tx_buf);

    uart_init();
  }
}

void zb_osif_serial_deinit(void)
{
}

#ifdef ZB_MACSPLIT_TRANSPORT_SERIAL
void zb_osif_serial_transport_init(void)
{
  zb_osif_serial_init();
}
#endif /* #ifdef ZB_MACSPLIT_TRANSPORT_SERIAL */

static zb_uint32_t uart_take_next_portion(zb_uint8_t **into)
{
  zb_uint_t size;
  *into = ZB_BYTE_ARRAY_GET_BATCH(SER_CTX().p_tx_buf, size, SER_CTX().tx_buf_cap);
  SER_CTX().tx_in_progress = !!size;

  return size;
}

static void uart_write_callback(UART2_Handle handle, void *ptr, size_t size, void *userArg, int_fast16_t status)
{
  zb_uint8_t *p;

  ZVUNUSED(handle);
  ZVUNUSED(ptr);

  ZB_BYTE_ARRAY_FLUSH_GET_BATCH(SER_CTX().p_tx_buf, size, SER_CTX().tx_buf_cap);
  size = uart_take_next_portion(&p);

  if (size)
  {
    UART2_write(gs_uart, p, size, NULL);
  }
}

#ifdef SYNC_WRITE
void zb_osif_serial_put_bytes(const zb_uint8_t *buf, zb_short_t len)
{
  if (gs_uart_inited)
  {
    UART2_write(gs_uart, buf, len, NULL);
  }
}
#else // !SYNC_WRITE

#ifdef ZB_MACSPLIT_TRANSPORT_SERIAL
void zb_osif_serial_transport_put_bytes(zb_uint8_t *buf, zb_short_t len)
{
  zb_osif_serial_put_bytes(buf, len);
}
#endif /* ZB_MACSPLIT_TRANSPORT_SERIAL */

void zb_osif_serial_put_bytes(const zb_uint8_t *buf, zb_short_t len)
{
  size_t size = 0;
  zb_uint8_t *p = NULL;

  if (gs_uart_inited)
  {
    while (len)
    {
      HwiP_disableInterrupt(((UART2LPF3_HWAttrs const *)(UART2_config[CONFIG_UART2_ZB].hwAttrs))->intNum);

      while( !ZB_BYTE_ARRAY_IS_FULL(SER_CTX().p_tx_buf, SER_CTX().tx_buf_cap) && len)
      {
        ZB_BYTE_ARRAY_PUT(SER_CTX().p_tx_buf, *buf, SER_CTX().tx_buf_cap);
        buf++;
        len--;
      }

      /* start write ASAP, even for the first byte. */
      if (!SER_CTX().tx_in_progress)
      {
        size = uart_take_next_portion(&p);
        /* Currently we are looping then. TODO: synchronize by semaphore with
         * write callback. */
      }

      HwiP_enableInterrupt(((UART2LPF3_HWAttrs const *)(UART2_config[CONFIG_UART2_ZB].hwAttrs))->intNum);

      if (size)
      {
        UART2_write(gs_uart, p, size, NULL);

        size = 0;
      }
    } /* while */
  }
}
#endif // SYNC_WRITE

void zb_osif_set_uart_byte_received_cb(void (*hnd)(zb_uint8_t))
{
  SER_CTX().byte_received_cb = hnd;
}

void zb_osif_set_user_io_buffer(zb_byte_array_t *buf_ptr, zb_ushort_t capacity)
{
  ZB_ASSERT(buf_ptr);
  ZB_ASSERT(capacity);

  /* Block until TX is free */
  while (SER_CTX().tx_in_progress)
  {

  }

  SER_CTX().p_tx_buf = buf_ptr;
  SER_CTX().tx_buf_cap = capacity;
}

void zb_osif_uart_sleep(void)
{
#ifndef NCP_MODE
  if (gs_uart_inited)
  {
    UART2_readCancel(gs_uart);
    UART2_writeCancel(gs_uart);

    UART2_close(gs_uart);
    gs_uart_inited = ZB_FALSE;
  }

#endif // !NCP_MODE
}

void zb_osif_uart_wake_up(void)
{
#ifndef NCP_MODE
  if (gs_uart_inited)
  {
    UART2_read(gs_uart, &gs_read_buffer, 1, NULL);
  }
  else
  {
    uart_init();
  }
#endif // !NCP_MODE
}

#endif // ZB_HAVE_SERIAL
