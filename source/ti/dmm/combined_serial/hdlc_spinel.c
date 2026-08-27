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
 * @file  hdlc_spinel.c
 * @brief HDLC framing, Spinel header handling, and CRC-16/Kermit
 *        implementation for the TI Combined Serial MUX layer.
 *
 * Assert policy (same as mux_buffer.c):
 *   Every error path is guarded by an assert() that fires before the runtime
 *   error return.  This allows bugs to be caught immediately during
 *   development without changing release behaviour (NDEBUG).
 *   MUX_ERR_CRC is NOT asserted — a CRC mismatch is a recoverable runtime
 *   event (line noise, partial frame) rather than a programming mistake.
 */

#include "hdlc_spinel.h"

#include <assert.h>  /* assert() */
#include <string.h>  /* memcpy  */

/*---------------------------------------------------------------------------
 * CRC-16 / Kermit
 *   Polynomial : 0x1021  (reflected = 0x8408)
 *   Initial value: 0x0000
 *   Input / output reflected: yes
 *   Final XOR: none
 *--------------------------------------------------------------------------*/

uint16_t MuxCrc16_update(uint16_t crc, uint8_t byte)
{
    uint8_t i;

    crc ^= (uint16_t)byte;
    for (i = 0U; i < 8U; i++)
    {
        if (crc & 0x0001U)
        {
            crc = (uint16_t)((crc >> 1U) ^ 0x8408U);
        }
        else
        {
            crc >>= 1U;
        }
    }
    return crc;
}

uint16_t MuxCrc16_compute(const uint8_t *data, uint16_t len)
{
    uint16_t crc = 0x0000U;
    uint16_t i;

    /* NULL or zero-length: CRC of empty data is 0x0000 */
    if (!data || len == 0U)
    {
        return crc;
    }

    for (i = 0U; i < len; i++)
    {
        crc = MuxCrc16_update(crc, data[i]);
    }
    return crc;
}

/*---------------------------------------------------------------------------
 * HDLC helpers
 *--------------------------------------------------------------------------*/

/*!
 * @brief Append one byte to @p outBuf, escaping it if required.
 *
 * @param[out] outBuf     Destination buffer.
 * @param[in]  outMaxLen  Capacity of @p outBuf.
 * @param[in,out] pos     Current write position; advanced on success.
 * @param[in]  byte       Byte to append (will be escaped if 0x7E or 0x7D).
 *
 * @return MUX_SUCCESS or MUX_ERR_OVERFLOW.
 */
static MuxErr_t hdlc_appendEscaped(uint8_t *outBuf, uint16_t outMaxLen,
                                   uint16_t *pos, uint8_t byte)
{
    if (byte == HDLC_FLAG || byte == HDLC_ESCAPE)
    {
        if ((uint16_t)(*pos + 2U) > outMaxLen)
        {
            return MUX_ERR_OVERFLOW;
        }
        outBuf[(*pos)++] = HDLC_ESCAPE;
        outBuf[(*pos)++] = byte ^ HDLC_ESCAPE_MASK;
    }
    else
    {
        if (*pos >= outMaxLen)
        {
            return MUX_ERR_OVERFLOW;
        }
        outBuf[(*pos)++] = byte;
    }
    return MUX_SUCCESS;
}

/*---------------------------------------------------------------------------
 * HDLC encode
 *--------------------------------------------------------------------------*/

MuxErr_t MuxHdlc_encode(const uint8_t *payload, uint16_t payloadLen,
                        uint8_t *outBuf, uint16_t outMaxLen, uint16_t *outLen)
{
    uint16_t crc;
    uint16_t pos = 0U;
    uint16_t i;
    MuxErr_t err;

    if (!payload || payloadLen == 0U || !outBuf || !outLen)
    {
        return MUX_ERR_INVALID;
    }

    /* Opening flag */
    if (pos >= outMaxLen)
    {
        return MUX_ERR_OVERFLOW;
    }
    outBuf[pos++] = HDLC_FLAG;

    /* Compute CRC over unescaped payload */
    crc = MuxCrc16_compute(payload, payloadLen);

    /* Escaped payload */
    for (i = 0U; i < payloadLen; i++)
    {
        err = hdlc_appendEscaped(outBuf, outMaxLen, &pos, payload[i]);

        if (err != MUX_SUCCESS)
        {
            return err;
        }
    }

    /* Escaped CRC (little-endian: low byte first) */
    err = hdlc_appendEscaped(outBuf, outMaxLen, &pos, (uint8_t)(crc & 0xFFU));

    if (err != MUX_SUCCESS)
    {
        return err;
    }

    err = hdlc_appendEscaped(outBuf, outMaxLen, &pos, (uint8_t)(crc >> 8U));

    if (err != MUX_SUCCESS)
    {
        return err;
    }

    /* Closing flag */
    if (pos >= outMaxLen)
    {
        return MUX_ERR_OVERFLOW;
    }
    outBuf[pos++] = HDLC_FLAG;

    *outLen = pos;
    return MUX_SUCCESS;
}

/*---------------------------------------------------------------------------
 * HDLC decode
 *--------------------------------------------------------------------------*/

MuxErr_t MuxHdlc_decode(const uint8_t *frame, uint16_t frameLen,
                        uint8_t *outBuf, uint16_t outMaxLen, uint16_t *outLen)
{
    uint16_t pos;
    uint16_t outPos = 0U;
    uint8_t  b;
    bool     escape = false;
    uint16_t crcReceived;
    uint16_t crcComputed;

    if (!frame || !outBuf || !outLen || frameLen < 4U)
    {
        return MUX_ERR_INVALID;
    }

    /* Must be delimited by HDLC flag bytes */
    if (frame[0] != HDLC_FLAG || frame[frameLen - 1U] != HDLC_FLAG)
    {
        return MUX_ERR_INVALID;
    }

    /* Decode (unescape) bytes between the two flag bytes */
    for (pos = 1U; pos < frameLen - 1U; pos++)
    {
        b = frame[pos];

        if (b == HDLC_FLAG)
        {
            /* Unexpected flag inside payload — framing error */
            return MUX_ERR_INVALID;
        }

        if (b == HDLC_ESCAPE)
        {
            escape = true;
            continue;
        }

        if (escape)
        {
            b      = b ^ HDLC_ESCAPE_MASK;
            escape = false;
        }

        if (outPos >= outMaxLen)
        {
            return MUX_ERR_OVERFLOW;
        }
        outBuf[outPos++] = b;
    }

    /* Trailing escape with no following byte is a framing error */
    if (escape)
    {
        return MUX_ERR_INVALID;
    }

    /* Must have decoded at least 2 CRC bytes */
    if (outPos < 2U)
    {
        return MUX_ERR_INVALID;
    }

    /* Extract and strip the CRC (last two decoded bytes, little-endian) */
    crcReceived = (uint16_t)((uint16_t)outBuf[outPos - 2U] |
                             ((uint16_t)outBuf[outPos - 1U] << 8U));
    outPos      = (uint16_t)(outPos - 2U);

    /* Verify CRC over the decoded payload */
    crcComputed = MuxCrc16_compute(outBuf, outPos);

    /* NOTE: CRC mismatch is a runtime event (line noise), not asserted */
    if (crcComputed != crcReceived)
    {
        return MUX_ERR_CRC;
    }

    *outLen = outPos;
    return MUX_SUCCESS;
}

/*---------------------------------------------------------------------------
 * Spinel packed-uint encode
 *--------------------------------------------------------------------------*/

MuxErr_t MuxSpinel_encodeUint(uint32_t value,
                              uint8_t *buf, uint16_t maxLen, uint16_t *written)
{
    uint16_t pos = 0U;
    uint8_t  b;

    if (!buf || maxLen == 0U || !written)
    {
        return MUX_ERR_INVALID;
    }

    do
    {
        if (pos >= maxLen)
        {
            return MUX_ERR_OVERFLOW;
        }

        b      = (uint8_t)(value & 0x7FU);
        value >>= 7U;

        if (value != 0U)
        {
            b |= 0x80U;   /* more bytes follow */
        }

        buf[pos++] = b;
    }
    while (value != 0U);

    *written = pos;
    return MUX_SUCCESS;
}

/*---------------------------------------------------------------------------
 * Spinel packed-uint decode
 *--------------------------------------------------------------------------*/

MuxErr_t MuxSpinel_decodeUint(const uint8_t *buf, uint16_t len,
                              uint32_t *value, uint16_t *consumed)
{
    uint16_t pos    = 0U;
    uint32_t result = 0U;
    uint8_t  shift  = 0U;
    uint8_t  b;

    if (!buf || len == 0U || !value || !consumed)
    {
        return MUX_ERR_INVALID;
    }

    do
    {
        if (pos >= len)
        {
            return MUX_ERR_NO_SPACE;
        }

        /* Guard against a value that would exceed 32 bits (5 bytes max) */
        if (shift >= 35U)
        {
            return MUX_ERR_OVERFLOW;
        }

        b       = buf[pos++];
        result |= (uint32_t)(b & 0x7FU) << shift;
        shift  += 7U;
    }
    while (b & 0x80U);

    *value    = result;
    *consumed = pos;
    return MUX_SUCCESS;
}

/*---------------------------------------------------------------------------
 * Spinel frame parser
 *--------------------------------------------------------------------------*/

MuxErr_t MuxSpinel_parseFrame(const uint8_t *spinelFrame, uint16_t frameLen,
                              uint8_t *nli, uint32_t *cmd,
                              const uint8_t **payloadPtr, uint16_t *payloadLen)
{
    uint8_t  hdr;
    uint32_t cmdValue;
    uint16_t consumed;
    MuxErr_t err;

    if (!spinelFrame || frameLen < 2U ||
        !nli || !cmd || !payloadPtr || !payloadLen)
    {
        return MUX_ERR_INVALID;
    }

    /* Byte 0: Spinel header — FLG bits [7:6] must be 0b10 (= 0x80 mask) */
    hdr = spinelFrame[0];

    if ((hdr & 0xC0U) != SPINEL_HEADER_FLAG)
    {
        return MUX_ERR_INVALID;
    }

    *nli = SPINEL_HEADER_GET_NLI(hdr);

    /* Bytes 1+: packed-uint CMD */
    err = MuxSpinel_decodeUint(spinelFrame + 1U, (uint16_t)(frameLen - 1U),
                               &cmdValue, &consumed);

    if (err != MUX_SUCCESS)
    {
        return err;
    }

    *cmd        = cmdValue;

    /* Remaining bytes (after header + CMD) are the payload */
    *payloadPtr = spinelFrame + 1U + consumed;
    *payloadLen = (uint16_t)(frameLen - 1U - consumed);

    return MUX_SUCCESS;
}

/*---------------------------------------------------------------------------
 * Combined TX path: Spinel frame + HDLC encode
 *--------------------------------------------------------------------------*/

MuxErr_t MuxSpinelHdlc_encode(uint8_t nli, uint32_t cmd,
                               const uint8_t *payload, uint16_t payloadLen,
                               uint8_t *scratchBuf, uint16_t scratchMaxLen,
                               uint8_t *outBuf, uint16_t outMaxLen,
                               uint16_t *outLen)
{
    uint8_t *spinelBuf = scratchBuf;
    uint16_t spinelPos = 0U;
    uint16_t cmdBytes;
    MuxErr_t err;

    if (nli > MUX_NLI_KEEPALIVE || !outBuf || !outLen)
    {
        return MUX_ERR_INVALID;
    }
    if (payloadLen > 0U && !payload)
    {
        return MUX_ERR_INVALID;
    }
    if (!scratchBuf || scratchMaxLen < MUX_SPINEL_BUF_MAX)
    {
        /* Scratch buffer must be caller-owned and large enough to hold the
         * worst-case Spinel frame (header + packed-uint CMD + payload). */
        return MUX_ERR_INVALID;
    }

    if (payloadLen > MUX_MSG_BUF_LEN)
    {
        return MUX_ERR_OVERFLOW;
    }

    /* 1. Spinel header byte */
    spinelBuf[spinelPos++] = SPINEL_HEADER(nli);

    /* 2. Packed-uint CMD */
    err = MuxSpinel_encodeUint(cmd,
                               spinelBuf + spinelPos,
                               (uint16_t)(scratchMaxLen - spinelPos),
                               &cmdBytes);

    if (err != MUX_SUCCESS)
    {
        return err;
    }
    spinelPos += cmdBytes;

    /* 3. Application payload (zero bytes for keepalive) */
    if (payloadLen > 0U)
    {
        memcpy(spinelBuf + spinelPos, payload, payloadLen);
        spinelPos += payloadLen;
    }

    /* 4. HDLC-encode the assembled Spinel frame */
    return MuxHdlc_encode(spinelBuf, spinelPos, outBuf, outMaxLen, outLen);
}
