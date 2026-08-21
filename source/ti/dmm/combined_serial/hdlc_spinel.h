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
 * @file  hdlc_spinel.h
 * @brief HDLC framing, Spinel header handling, and CRC-16 for the
 *        TI Combined Serial MUX layer.
 *
 * Layering
 * --------
 *
 *   Application payload (BLE HCI bytes / Zigbee MAC bytes / keepalive body)
 *       │
 *       ▼  MuxSpinelHdlc_encode() ──────────────── TX combined path
 *   Spinel frame  [HDR(1)] [CMD(packed-uint)] [payload...]
 *       │
 *       ▼  MuxHdlc_encode()
 *   HDLC frame  [0x7E] [escaped Spinel frame + CRC16] [0x7E]
 *       │
 *       ▼  wire / UART
 *
 *   HDLC frame  ──►  MuxHdlc_decode()  ──►  Spinel frame
 *   Spinel frame ──► MuxSpinel_parseFrame() ──► NLI, CMD, payload ptr
 *
 * CRC-16
 * ------
 * Variant: CRC-16/Kermit  (poly = 0x1021, init = 0x0000, reflected I/O,
 * no final XOR).  Reflected polynomial = 0x8408.
 * Computed over the unescaped Spinel frame bytes; appended as two
 * little-endian bytes before the closing HDLC flag.
 *
 * Spinel packed-uint
 * ------------------
 * 7-bit groups, little-endian, MSB of each byte = 1 means more bytes follow.
 * Most CMD values (0x03, 0x06) encode as a single byte.
 * Keepalive CMDs (15555, 15556, 15557) encode as two bytes.
 *
 * Scratch buffer note
 * -------------------
 * MuxSpinelHdlc_encode() stages the intermediate Spinel frame
 * (MUX_SPINEL_BUF_MAX bytes = 1 + 4 + MUX_MSG_BUF_LEN = 517 bytes) in a
 * caller-supplied scratch buffer rather than on its own stack or in
 * internal static storage. Callers own this buffer's lifetime and
 * concurrency; each caller should use its own scratch buffer (e.g. a field
 * in its own state struct) rather than sharing one across concurrent
 * callers, since the function itself does not synchronize access to it.
 */

#ifndef HDLC_SPINEL_H
#define HDLC_SPINEL_H

#include "mux_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------
 * Internal sizing constant (exposed for callers that need to allocate decode
 * scratch buffers of the right size)
 *--------------------------------------------------------------------------*/

/*!
 * Maximum size of a decoded Spinel frame (bytes).
 * = 1 (Spinel header) + 4 (max packed-uint CMD) + MUX_MSG_BUF_LEN (payload)
 */
#define MUX_SPINEL_BUF_MAX  ((uint16_t)(1U + 4U + MUX_MSG_BUF_LEN))

/*---------------------------------------------------------------------------
 * CRC-16 / Kermit
 *--------------------------------------------------------------------------*/

/*!
 * @brief Feed one byte into a running CRC-16/Kermit computation.
 *
 * Initialise @p crc to 0x0000 before the first call.
 *
 * @param crc   Running CRC value.
 * @param byte  Next data byte to process.
 * @return      Updated CRC value.
 */
uint16_t MuxCrc16_update(uint16_t crc, uint8_t byte);

/*!
 * @brief Compute CRC-16/Kermit over a contiguous byte array.
 *
 * Equivalent to calling MuxCrc16_update() for each byte starting from
 * an initial CRC of 0x0000.
 *
 * @param data  Pointer to data bytes (must not be NULL).
 * @param len   Number of bytes to process.
 * @return      Final CRC-16 value.
 */
uint16_t MuxCrc16_compute(const uint8_t *data, uint16_t len);

/*---------------------------------------------------------------------------
 * HDLC framing
 *--------------------------------------------------------------------------*/

/*!
 * @brief HDLC-encode a raw payload into a framed, CRC-protected byte stream.
 *
 * Output format:
 *   [0x7E] [escaped(payload)] [escaped(CRC_lo)] [escaped(CRC_hi)] [0x7E]
 *
 * Bytes 0x7E and 0x7D in the payload and CRC are replaced by the two-byte
 * escape sequence [0x7D] [byte XOR 0x20].
 *
 * @param[in]  payload     Raw bytes to frame (must not be NULL).
 * @param[in]  payloadLen  Number of bytes in @p payload.  Must be > 0.
 * @param[out] outBuf      Destination buffer for the encoded frame.
 * @param[in]  outMaxLen   Capacity of @p outBuf.
 * @param[out] outLen      Number of bytes written to @p outBuf on success.
 *
 * @return MUX_SUCCESS      Frame written to @p outBuf.
 * @return MUX_ERR_OVERFLOW Encoded frame would exceed @p outMaxLen.
 * @return MUX_ERR_INVALID  NULL pointer or @p payloadLen is 0.
 */
MuxErr_t MuxHdlc_encode(const uint8_t *payload, uint16_t payloadLen,
                        uint8_t *outBuf, uint16_t outMaxLen, uint16_t *outLen);

/*!
 * @brief HDLC-decode a raw frame: strip flags, unescape, verify CRC.
 *
 * On success @p outBuf contains the decoded payload (Spinel frame) without
 * the CRC bytes and without the flag bytes.
 *
 * @param[in]  frame      Raw frame including both 0x7E flag bytes.
 * @param[in]  frameLen   Total length of @p frame in bytes.  Must be >= 4.
 * @param[out] outBuf     Destination for the decoded payload.
 * @param[in]  outMaxLen  Capacity of @p outBuf.
 * @param[out] outLen     Number of decoded payload bytes written on success.
 *
 * @return MUX_SUCCESS      Payload decoded and CRC verified.
 * @return MUX_ERR_CRC      CRC mismatch — frame is corrupt.
 * @return MUX_ERR_OVERFLOW Decoded payload exceeds @p outMaxLen.
 * @return MUX_ERR_INVALID  NULL pointer, @p frameLen < 4, missing flag bytes,
 *                          unexpected flag byte inside payload, or incomplete
 *                          escape sequence.
 */
MuxErr_t MuxHdlc_decode(const uint8_t *frame, uint16_t frameLen,
                        uint8_t *outBuf, uint16_t outMaxLen, uint16_t *outLen);

/*---------------------------------------------------------------------------
 * Spinel packed-uint codec
 *--------------------------------------------------------------------------*/

/*!
 * @brief Encode a 32-bit unsigned integer as a Spinel packed-uint.
 *
 * Encoding: 7 bits per byte, little-endian; MSB of each byte is 1 if
 * more bytes follow, 0 for the last byte.
 *
 * @param[in]  value    Value to encode.
 * @param[out] buf      Destination buffer.
 * @param[in]  maxLen   Capacity of @p buf in bytes.
 * @param[out] written  Number of bytes written on success (1–5).
 *
 * @return MUX_SUCCESS      Value encoded.
 * @return MUX_ERR_OVERFLOW @p maxLen too small (need up to 5 bytes).
 * @return MUX_ERR_INVALID  NULL pointer or @p maxLen is 0.
 */
MuxErr_t MuxSpinel_encodeUint(uint32_t value,
                              uint8_t *buf, uint16_t maxLen, uint16_t *written);

/*!
 * @brief Decode a Spinel packed-uint from a byte buffer.
 *
 * @param[in]  buf       Source buffer.
 * @param[in]  len       Number of bytes available in @p buf.
 * @param[out] value     Decoded value on success.
 * @param[out] consumed  Number of bytes consumed from @p buf.
 *
 * @return MUX_SUCCESS     Value decoded.
 * @return MUX_ERR_NO_SPACE Buffer too short (truncated packed-uint).
 * @return MUX_ERR_OVERFLOW Encoded value exceeds 32 bits.
 * @return MUX_ERR_INVALID  NULL pointer or @p len is 0.
 */
MuxErr_t MuxSpinel_decodeUint(const uint8_t *buf, uint16_t len,
                              uint32_t *value, uint16_t *consumed);

/*---------------------------------------------------------------------------
 * Spinel frame parser
 *--------------------------------------------------------------------------*/

/*!
 * @brief Parse a decoded Spinel frame into its constituent fields.
 *
 * The function does NOT copy the payload — @p *payloadPtr is set to point
 * directly into @p spinelFrame.  The pointer is valid only as long as the
 * caller keeps @p spinelFrame alive.
 *
 * @param[in]  spinelFrame  Decoded Spinel frame (header + CMD + payload).
 * @param[in]  frameLen     Length of @p spinelFrame in bytes.  Must be >= 2.
 * @param[out] nli          NLI extracted from the Spinel header byte.
 * @param[out] cmd          CMD field (packed-uint decoded to uint32).
 * @param[out] payloadPtr   Set to the first payload byte inside @p spinelFrame.
 * @param[out] payloadLen   Number of payload bytes (may be 0 for keepalive).
 *
 * @return MUX_SUCCESS     Frame parsed successfully.
 * @return MUX_ERR_INVALID NULL pointer, @p frameLen < 2, or invalid Spinel
 *                         header FLG bits.
 * @return MUX_ERR_NO_SPACE Packed-uint CMD is truncated.
 */
MuxErr_t MuxSpinel_parseFrame(const uint8_t *spinelFrame, uint16_t frameLen,
                              uint8_t *nli, uint32_t *cmd,
                              const uint8_t **payloadPtr, uint16_t *payloadLen);

/*---------------------------------------------------------------------------
 * Combined TX path: Spinel frame + HDLC encode
 *--------------------------------------------------------------------------*/

/*!
 * @brief Build a Spinel frame and HDLC-encode it in a single call.
 *
 * This is the primary function used by the embedded MUX task (TX path) and
 * the host application to prepare packets for the wire.
 *
 * Wire output:
 *   [0x7E] [escaped([HDR] [CMD] [payload])] [escaped(CRC16)] [0x7E]
 *
 * For keepalive messages pass @p payload = NULL and @p payloadLen = 0.
 *
 * @param[in]  nli          NLI channel (MUX_NLI_BLE, MUX_NLI_ZB, …).
 * @param[in]  cmd          Spinel CMD value (e.g. SPINEL_CMD_PROP_VALUE_IS).
 * @param[in]  payload      Application payload bytes (may be NULL if len = 0).
 * @param[in]  payloadLen   Length of @p payload in bytes (0 for keepalive).
 * @param[out] scratchBuf   Caller-owned scratch buffer for the intermediate
 *                          Spinel frame. Must be at least MUX_SPINEL_BUF_MAX
 *                          bytes. Not touched after this call returns.
 * @param[in]  scratchMaxLen Capacity of @p scratchBuf in bytes.
 * @param[out] outBuf       Destination for the encoded HDLC frame.
 * @param[in]  outMaxLen    Capacity of @p outBuf.  MAX_FRAME_SIZE is sufficient.
 * @param[out] outLen       Number of bytes written to @p outBuf on success.
 *
 * @return MUX_SUCCESS      Frame encoded and ready to transmit.
 * @return MUX_ERR_OVERFLOW Encoded frame exceeds @p outMaxLen or payload
 *                          exceeds MUX_MSG_BUF_LEN.
 * @return MUX_ERR_INVALID  NULL pointer, @p nli out of range, or
 *                          @p scratchMaxLen < MUX_SPINEL_BUF_MAX.
 */
MuxErr_t MuxSpinelHdlc_encode(uint8_t nli, uint32_t cmd,
                               const uint8_t *payload, uint16_t payloadLen,
                               uint8_t *scratchBuf, uint16_t scratchMaxLen,
                               uint8_t *outBuf, uint16_t outMaxLen,
                               uint16_t *outLen);

#ifdef __cplusplus
}
#endif

#endif /* HDLC_SPINEL_H */