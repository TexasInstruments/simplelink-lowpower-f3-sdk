/*
 * Copyright (c) 2025, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Texas Instruments Incorporated nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file  mux_common.h
 * @brief Common types, constants, and error codes shared between the
 *        embedded MUX layer and the Linux host MUX application for the
 *        TI Combined Serial Interface.
 *
 * The Combined Serial Interface multiplexes BLE (HCI) and Zigbee MAC
 * traffic over a single UART/SPI link using HDLC framing and Spinel
 * headers. Each protocol stack is identified by a Network Layer
 * Identifier (NLI) encoded in the Spinel header byte.
 *
 * Packet format (wire):
 *   [0x7E] [HDLC payload ...] [CRC16 lo] [CRC16 hi] [0x7E]
 *
 * HDLC payload for BLE/ZB:
 *   [Spinel header: FLG(2b)=01 | NLI(2b) | TID(4b)=0] [CMD(packed uint)] [data ...]
 *
 * NLI assignments:
 *   0 = OpenThread (reserved, not used in BLE+ZB scope)
 *   1 = BLE HCI
 *   2 = Zigbee
 *   3 = Keepalive
 */

#ifndef MUX_COMMON_H
#define MUX_COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------
 * NLI (Network Layer Identifier) assignments
 *--------------------------------------------------------------------------*/

/** OpenThread — reserved for Thread+BLE scope (Stage 1 future work) */
#define MUX_NLI_OT          (0U)

/** BLE HCI traffic */
#define MUX_NLI_BLE         (1U)

/** Zigbee MAC traffic */
#define MUX_NLI_ZB          (2U)

/** Keepalive messages */
#define MUX_NLI_KEEPALIVE   (3U)

/** Number of supported NLI channels */
#define MUX_NLI_COUNT       (4U)

/** Invalid / unrecognised NLI */
#define MUX_NLI_INVALID     (0xFFU)

/*---------------------------------------------------------------------------
 * Keepalive command IDs (vendor-specific Spinel CMD range: 15360–16383)
 *--------------------------------------------------------------------------*/

/** Keepalive ping sent by the embedded device */
#define CMD_KEEPALIVE       (15555U)

/** Keepalive acknowledgement sent by the host */
#define CMD_KEEPALIVE_ACK   (15556U)

/** Logging command */
#define CMD_LOGGING         (15557U)

/*---------------------------------------------------------------------------
 * Spinel header bit definitions
 *--------------------------------------------------------------------------*/

/** FLG field value — always 0x01 (bits [7:6] of header byte = 0b10 → 0x80 mask)
 *  Per Spinel spec: header byte = 0x80 | (NLI << 4) | TID */
#define SPINEL_HEADER_FLAG      (0x80U)

/** Mask to extract NLI from Spinel header byte */
#define SPINEL_NLI_MASK         (0x30U)

/** Shift to extract NLI from Spinel header byte */
#define SPINEL_NLI_SHIFT        (4U)

/** Mask to extract TID from Spinel header byte */
#define SPINEL_TID_MASK         (0x0FU)

/** TID value used for keepalive / non-transactional messages */
#define SPINEL_TID_NONE         (0x00U)

/** Spinel PROP_VALUE_IS command (used for BLE and ZB data) */
#define SPINEL_CMD_PROP_VALUE_IS    (0x06U)

/** Spinel PROP_VALUE_SET command (used by host for BLE and ZB data) */
#define SPINEL_CMD_PROP_VALUE_SET   (0x03U)

/** Helper macro: build Spinel header byte from NLI */
#define SPINEL_HEADER(nli)  ((uint8_t)(SPINEL_HEADER_FLAG | (((nli) & 0x03U) << SPINEL_NLI_SHIFT) | SPINEL_TID_NONE))

/** Helper macro: extract NLI from Spinel header byte (matches ZB_SPINEL_NLI_GET) */
#define SPINEL_HEADER_GET_NLI(hdr)  (((hdr) >> SPINEL_NLI_SHIFT) & 0x03U)

/** Helper macro: extract TID from Spinel header byte */
#define SPINEL_HEADER_GET_TID(hdr)  ((hdr) & SPINEL_TID_MASK)

/*---------------------------------------------------------------------------
 * HDLC framing constants
 *--------------------------------------------------------------------------*/

/** HDLC flag byte — marks start and end of every frame */
#define HDLC_FLAG           (0x7EU)

/** HDLC escape byte */
#define HDLC_ESCAPE         (0x7DU)

/** XOR mask applied to escaped bytes */
#define HDLC_ESCAPE_MASK    (0x20U)

/*---------------------------------------------------------------------------
 * Buffer and queue size constants
 *--------------------------------------------------------------------------*/

/**
 * Maximum size of a single encoded HDLC frame (bytes).
 * Worst case: every byte in a max-payload frame requires escaping,
 * plus 2 flag bytes + 2 CRC bytes (also potentially escaped).
 * Set conservatively to accommodate BLE HCI max ACL packet (1024 B payload).
 */
#define MAX_FRAME_SIZE      (2048U)

/**
 * Maximum payload length stored in a single MUX queue message.
 * Covers the largest BLE HCI or Zigbee MAC packet expected.
 */
#define MUX_MSG_BUF_LEN     (512U)

/**
 * Size of the intermediary ring buffer used to accumulate serial RX bytes
 * before a full HDLC packet is identified.
 * Must be at least 2 × MAX_FRAME_SIZE to handle back-to-back frames.
 */
#define MAX_RING_BUF_SIZE   (4096U)

/**
 * Maximum number of messages in the MUX TX or RX FreeRTOS queue.
 */
#define MUX_QUEUE_DEPTH     (6U)

/*---------------------------------------------------------------------------
 * Error / return codes
 *--------------------------------------------------------------------------*/

typedef int32_t MuxErr_t;

/** Operation completed successfully */
#define MUX_SUCCESS             ( 0)

/** Invalid argument (NULL pointer, zero length, out-of-range NLI, etc.) */
#define MUX_ERR_INVALID         (-1)

/** Output buffer too small to hold the encoded/decoded result */
#define MUX_ERR_OVERFLOW        (-2)

/** HDLC CRC mismatch on decode */
#define MUX_ERR_CRC             (-3)

/** Ring buffer is full — write rejected */
#define MUX_ERR_FULL            (-4)

/** Not enough data in buffer to satisfy read request */
#define MUX_ERR_NO_SPACE        (-5)

/** No complete HDLC packet found in ring buffer yet */
#define MUX_ERR_NO_PACKET       (-6)

/** Ring buffer is empty */
#define MUX_ERR_EMPTY           (-7)

/** UART write failed */
#define MUX_ERR_UART            (-9)

/*---------------------------------------------------------------------------
 * Stack receive callback type
 * Registered per NLI. On the embedded side, called directly from UART ISR
 * context when a decoded packet arrives for that stack — the callback (and
 * everything it calls) must be ISR-safe. See mux_task_app.h for the full
 * ISR-safety contract.
 *--------------------------------------------------------------------------*/

/**
 * @brief Callback invoked to deliver a decoded inbound packet to the
 *        appropriate protocol stack.
 *
 * @param buf   Pointer to decoded payload (valid for duration of call).
 * @param len   Length of the decoded payload in bytes.
 */
typedef void (*MuxStackRxCb_t)(const uint8_t *buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* MUX_COMMON_H */
