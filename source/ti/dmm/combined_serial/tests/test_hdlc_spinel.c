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
 * INTERRUPTION) IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file  test_hdlc_spinel.c
 * @brief Unit tests for hdlc_spinel.c:
 *        CRC-16/Kermit, HDLC encode/decode, Spinel packed-uint,
 *        Spinel frame parsing, and end-to-end MuxSpinelHdlc_encode.
 */

#include "test_harness.h"
#include "../hdlc_spinel.h"
#include "../mux_common.h"

#include <string.h>
#include <stdint.h>

/*---------------------------------------------------------------------------
 * Scratch buffers
 *--------------------------------------------------------------------------*/

static uint8_t gFrame[MAX_FRAME_SIZE];
static uint8_t gDecoded[MUX_SPINEL_BUF_MAX];
static uint8_t gEncScratch[MUX_SPINEL_BUF_MAX]; /* MuxSpinelHdlc_encode() scratch */

/*---------------------------------------------------------------------------
 * CRC-16 / Kermit
 *--------------------------------------------------------------------------*/

static void test_crc16_known_vector(void)
{
    /* CRC-16/Kermit ("123456789") = 0x2189 — from the CRC catalogue */
    const uint8_t data[] = {'1','2','3','4','5','6','7','8','9'};
    EXPECT_EQ(MuxCrc16_compute(data, 9U), 0x2189U);
}

static void test_crc16_empty_is_zero(void)
{
    /* Initial CRC is 0x0000; computing over 0 bytes must return it unchanged */
    const uint8_t dummy[1] = {0U};
    EXPECT_EQ(MuxCrc16_compute(dummy, 0U), 0x0000U);
}

static void test_crc16_update_matches_compute(void)
{
    const uint8_t data[] = {0x01U, 0x02U, 0x03U, 0x04U};
    uint16_t running = 0x0000U;
    uint16_t i;
    for (i = 0U; i < 4U; i++)
    {
        running = MuxCrc16_update(running, data[i]);
    }
    EXPECT_EQ(running, MuxCrc16_compute(data, 4U));
}

static void test_crc16_different_data_differs(void)
{
    const uint8_t a[] = {0xAAU, 0xBBU};
    const uint8_t b[] = {0xAAU, 0xBCU};
    EXPECT_NE(MuxCrc16_compute(a, 2U), MuxCrc16_compute(b, 2U));
}

/*---------------------------------------------------------------------------
 * HDLC encode / decode — basic
 *--------------------------------------------------------------------------*/

static void test_hdlc_encode_has_flags(void)
{
    const uint8_t payload[] = {0x01U, 0x02U, 0x03U};
    uint16_t frameLen = 0U;

    EXPECT_EQ(MuxHdlc_encode(payload, 3U, gFrame, (uint16_t)sizeof(gFrame),
                              &frameLen),
              MUX_SUCCESS);
    EXPECT(frameLen >= 7U);  /* [0x7E] + 3 payload + 2 CRC + [0x7E] min */
    EXPECT_EQ(gFrame[0], HDLC_FLAG);
    EXPECT_EQ(gFrame[frameLen - 1U], HDLC_FLAG);
}

static void test_hdlc_roundtrip_simple(void)
{
    const uint8_t payload[] = {0xAAU, 0xBBU, 0xCCU, 0xDDU};
    uint16_t frameLen = 0U, decodedLen = 0U;

    EXPECT_EQ(MuxHdlc_encode(payload, 4U, gFrame, (uint16_t)sizeof(gFrame),
                              &frameLen),
              MUX_SUCCESS);
    EXPECT_EQ(MuxHdlc_decode(gFrame, frameLen, gDecoded,
                              (uint16_t)sizeof(gDecoded), &decodedLen),
              MUX_SUCCESS);
    EXPECT_EQ(decodedLen, 4U);
    EXPECT_EQ(memcmp(payload, gDecoded, 4U), 0);
}

static void test_hdlc_roundtrip_single_byte(void)
{
    const uint8_t payload[] = {0x42U};
    uint16_t frameLen = 0U, decodedLen = 0U;

    MuxHdlc_encode(payload, 1U, gFrame, (uint16_t)sizeof(gFrame), &frameLen);
    EXPECT_EQ(MuxHdlc_decode(gFrame, frameLen, gDecoded,
                              (uint16_t)sizeof(gDecoded), &decodedLen),
              MUX_SUCCESS);
    EXPECT_EQ(decodedLen, 1U);
    EXPECT_EQ(gDecoded[0], 0x42U);
}

/*---------------------------------------------------------------------------
 * HDLC escaping
 *--------------------------------------------------------------------------*/

static void test_hdlc_escape_flag_byte_in_payload(void)
{
    /* 0x7E in the payload must be escaped; the decoded result must match */
    const uint8_t payload[] = {0x7EU, 0x00U, 0x7EU};
    uint16_t frameLen = 0U, decodedLen = 0U;

    EXPECT_EQ(MuxHdlc_encode(payload, 3U, gFrame, (uint16_t)sizeof(gFrame),
                              &frameLen),
              MUX_SUCCESS);
    /* Two 0x7E bytes in payload → each becomes 2 bytes; plus 2 CRC + 2 flags */
    EXPECT(frameLen >= 3U + 2U + 2U + 2U); /* 9 minimum */

    EXPECT_EQ(MuxHdlc_decode(gFrame, frameLen, gDecoded,
                              (uint16_t)sizeof(gDecoded), &decodedLen),
              MUX_SUCCESS);
    EXPECT_EQ(decodedLen, 3U);
    EXPECT_EQ(memcmp(payload, gDecoded, 3U), 0);
}

static void test_hdlc_escape_escape_byte_in_payload(void)
{
    /* 0x7D in the payload must also be escaped */
    const uint8_t payload[] = {0x7DU, 0x01U, 0x7DU};
    uint16_t frameLen = 0U, decodedLen = 0U;

    MuxHdlc_encode(payload, 3U, gFrame, (uint16_t)sizeof(gFrame), &frameLen);
    EXPECT_EQ(MuxHdlc_decode(gFrame, frameLen, gDecoded,
                              (uint16_t)sizeof(gDecoded), &decodedLen),
              MUX_SUCCESS);
    EXPECT_EQ(decodedLen, 3U);
    EXPECT_EQ(memcmp(payload, gDecoded, 3U), 0);
}

static void test_hdlc_all_bytes_roundtrip(void)
{
    /* Encode all 256 byte values and verify they survive a roundtrip.
     * Note: MuxHdlc_decode temporarily holds payload + 2 CRC bytes in outBuf
     * before stripping the CRC, so outBuf must be at least 258 bytes. */
    uint8_t payload[256];
    uint8_t decoded[260];   /* 256 payload + 2 CRC + 2 spare */
    uint8_t frame[MAX_FRAME_SIZE];
    uint16_t frameLen = 0U, decodedLen = 0U;
    uint16_t i;

    for (i = 0U; i < 256U; i++) { payload[i] = (uint8_t)i; }

    EXPECT_EQ(MuxHdlc_encode(payload, 256U, frame, (uint16_t)sizeof(frame),
                              &frameLen),
              MUX_SUCCESS);
    EXPECT_EQ(MuxHdlc_decode(frame, frameLen, decoded, (uint16_t)sizeof(decoded),
                              &decodedLen),
              MUX_SUCCESS);
    EXPECT_EQ(decodedLen, 256U);
    EXPECT_EQ(memcmp(payload, decoded, 256U), 0);
}

/*---------------------------------------------------------------------------
 * HDLC error cases
 *--------------------------------------------------------------------------*/

static void test_hdlc_decode_bad_crc(void)
{
    const uint8_t payload[] = {0x01U, 0x02U, 0x03U};
    uint16_t frameLen = 0U, decodedLen = 0U;

    MuxHdlc_encode(payload, 3U, gFrame, (uint16_t)sizeof(gFrame), &frameLen);
    /* Corrupt the second byte (inside payload, not a flag) */
    gFrame[1] ^= 0xFFU;
    EXPECT_EQ(MuxHdlc_decode(gFrame, frameLen, gDecoded,
                              (uint16_t)sizeof(gDecoded), &decodedLen),
              MUX_ERR_CRC);
}

static void test_hdlc_decode_missing_opening_flag(void)
{
    const uint8_t bad[] = {0x01U, 0x02U, 0x03U, 0x7EU};
    uint16_t decodedLen = 0U;
    EXPECT_EQ(MuxHdlc_decode(bad, (uint16_t)sizeof(bad), gDecoded,
                              (uint16_t)sizeof(gDecoded), &decodedLen),
              MUX_ERR_INVALID);
}

static void test_hdlc_decode_too_short(void)
{
    /* Minimum valid frame is 4 bytes: [0x7E] [CRC lo] [CRC hi] [0x7E] */
    const uint8_t bad[] = {0x7EU, 0x00U, 0x7EU};
    uint16_t decodedLen = 0U;
    EXPECT_EQ(MuxHdlc_decode(bad, 3U, gDecoded, (uint16_t)sizeof(gDecoded),
                              &decodedLen),
              MUX_ERR_INVALID);
}

static void test_hdlc_encode_overflow(void)
{
    /* Payload larger than output buffer; worst-case escaping doubles size */
    uint8_t payload[256];
    uint8_t tiny[10];
    uint16_t frameLen = 0U;
    memset(payload, 0x7EU, sizeof(payload)); /* all bytes need escaping */

    EXPECT_EQ(MuxHdlc_encode(payload, 256U, tiny, (uint16_t)sizeof(tiny),
                              &frameLen),
              MUX_ERR_OVERFLOW);
}

static void test_hdlc_invalid_args(void)
{
    uint16_t len = 0U;
    EXPECT_EQ(MuxHdlc_encode(NULL, 1U, gFrame, (uint16_t)sizeof(gFrame), &len),
              MUX_ERR_INVALID);
    EXPECT_EQ(MuxHdlc_encode(gDecoded, 0U, gFrame, (uint16_t)sizeof(gFrame), &len),
              MUX_ERR_INVALID);
    EXPECT_EQ(MuxHdlc_encode(gDecoded, 1U, NULL, (uint16_t)sizeof(gFrame), &len),
              MUX_ERR_INVALID);

    EXPECT_EQ(MuxHdlc_decode(NULL, 4U, gDecoded, (uint16_t)sizeof(gDecoded), &len),
              MUX_ERR_INVALID);
    EXPECT_EQ(MuxHdlc_decode(gFrame, 4U, NULL, (uint16_t)sizeof(gDecoded), &len),
              MUX_ERR_INVALID);
}

/*---------------------------------------------------------------------------
 * Spinel packed-uint encoding
 *--------------------------------------------------------------------------*/

static void test_spinel_uint_single_byte_values(void)
{
    uint8_t  buf[8];
    uint16_t written  = 0U;
    uint32_t decoded  = 0U;
    uint16_t consumed = 0U;

    /* Values 0–127 must encode as exactly 1 byte */
    EXPECT_EQ(MuxSpinel_encodeUint(0U, buf, 8U, &written), MUX_SUCCESS);
    EXPECT_EQ(written, 1U);
    EXPECT_EQ(buf[0], 0x00U);

    EXPECT_EQ(MuxSpinel_encodeUint(0x06U, buf, 8U, &written), MUX_SUCCESS);
    EXPECT_EQ(written, 1U);
    EXPECT_EQ(buf[0], 0x06U);

    EXPECT_EQ(MuxSpinel_encodeUint(127U, buf, 8U, &written), MUX_SUCCESS);
    EXPECT_EQ(written, 1U);
    EXPECT_EQ(buf[0], 0x7FU);

    /* Decode back */
    EXPECT_EQ(MuxSpinel_decodeUint(buf, 1U, &decoded, &consumed), MUX_SUCCESS);
    EXPECT_EQ(decoded, 127U);
    EXPECT_EQ(consumed, 1U);
}

static void test_spinel_uint_two_byte_values(void)
{
    uint8_t  buf[8];
    uint16_t written  = 0U;
    uint32_t decoded  = 0U;
    uint16_t consumed = 0U;

    /* 128 encodes as [0x80, 0x01] */
    EXPECT_EQ(MuxSpinel_encodeUint(128U, buf, 8U, &written), MUX_SUCCESS);
    EXPECT_EQ(written, 2U);
    EXPECT(buf[0] & 0x80U); /* continuation bit set */

    EXPECT_EQ(MuxSpinel_decodeUint(buf, written, &decoded, &consumed),
              MUX_SUCCESS);
    EXPECT_EQ(decoded, 128U);
    EXPECT_EQ(consumed, 2U);
}

static void test_spinel_uint_keepalive_cmds(void)
{
    /* CMD_KEEPALIVE (15555) and CMD_KEEPALIVE_ACK (15556) encode as 2 bytes */
    uint8_t  buf[8];
    uint16_t written  = 0U;
    uint32_t decoded  = 0U;
    uint16_t consumed = 0U;

    EXPECT_EQ(MuxSpinel_encodeUint(CMD_KEEPALIVE, buf, 8U, &written),
              MUX_SUCCESS);
    EXPECT_EQ(written, 2U);
    EXPECT_EQ(MuxSpinel_decodeUint(buf, written, &decoded, &consumed),
              MUX_SUCCESS);
    EXPECT_EQ(decoded, (uint32_t)CMD_KEEPALIVE);

    EXPECT_EQ(MuxSpinel_encodeUint(CMD_KEEPALIVE_ACK, buf, 8U, &written),
              MUX_SUCCESS);
    EXPECT_EQ(written, 2U);
    EXPECT_EQ(MuxSpinel_decodeUint(buf, written, &decoded, &consumed),
              MUX_SUCCESS);
    EXPECT_EQ(decoded, (uint32_t)CMD_KEEPALIVE_ACK);
}

static void test_spinel_uint_roundtrip_boundary_values(void)
{
    const uint32_t values[] = {0U, 1U, 127U, 128U, 255U,
                                16383U, 16384U, 0x7FFFFFFFU};
    uint8_t  buf[8];
    uint16_t written  = 0U;
    uint32_t decoded  = 0U;
    uint16_t consumed = 0U;
    uint16_t i;

    for (i = 0U; i < 8U; i++)
    {
        EXPECT_EQ(MuxSpinel_encodeUint(values[i], buf, 8U, &written),
                  MUX_SUCCESS);
        EXPECT_EQ(MuxSpinel_decodeUint(buf, written, &decoded, &consumed),
                  MUX_SUCCESS);
        EXPECT_EQ(decoded, values[i]);
        EXPECT_EQ(consumed, written);
    }
}

static void test_spinel_uint_decode_truncated(void)
{
    /* A packed-uint where the continuation bit is set but no next byte exists */
    const uint8_t bad[] = {0x80U}; /* continuation set, no second byte */
    uint32_t decoded  = 0U;
    uint16_t consumed = 0U;
    EXPECT_EQ(MuxSpinel_decodeUint(bad, 1U, &decoded, &consumed),
              MUX_ERR_NO_SPACE);
}

/*---------------------------------------------------------------------------
 * Spinel frame parsing
 *--------------------------------------------------------------------------*/

static void test_spinel_parse_ble_frame(void)
{
    /* [header=0x90 (NLI=1,BLE)] [CMD=0x06] [0xDE 0xAD] */
    uint8_t frame[4];
    const uint8_t *payPtr = NULL;
    uint16_t       payLen = 0U;
    uint8_t        nli    = 0U;
    uint32_t       cmd    = 0U;

    frame[0] = SPINEL_HEADER((uint8_t)MUX_NLI_BLE);
    frame[1] = (uint8_t)SPINEL_CMD_PROP_VALUE_IS;
    frame[2] = 0xDEU;
    frame[3] = 0xADU;

    EXPECT_EQ(MuxSpinel_parseFrame(frame, 4U, &nli, &cmd, &payPtr, &payLen),
              MUX_SUCCESS);
    EXPECT_EQ(nli, (uint8_t)MUX_NLI_BLE);
    EXPECT_EQ(cmd, (uint32_t)SPINEL_CMD_PROP_VALUE_IS);
    EXPECT_EQ(payLen, 2U);
    EXPECT_NONNULL(payPtr);
    EXPECT_EQ(payPtr[0], 0xDEU);
    EXPECT_EQ(payPtr[1], 0xADU);
}

static void test_spinel_parse_zigbee_frame(void)
{
    uint8_t frame[2];
    const uint8_t *payPtr = NULL;
    uint16_t       payLen = 0U;
    uint8_t        nli    = 0U;
    uint32_t       cmd    = 0U;

    frame[0] = SPINEL_HEADER((uint8_t)MUX_NLI_ZB);
    frame[1] = (uint8_t)SPINEL_CMD_PROP_VALUE_SET;

    EXPECT_EQ(MuxSpinel_parseFrame(frame, 2U, &nli, &cmd, &payPtr, &payLen),
              MUX_SUCCESS);
    EXPECT_EQ(nli, (uint8_t)MUX_NLI_ZB);
    EXPECT_EQ(cmd, (uint32_t)SPINEL_CMD_PROP_VALUE_SET);
    EXPECT_EQ(payLen, 0U);
}

static void test_spinel_parse_keepalive_frame(void)
{
    /* Keepalive: 2-byte packed CMD (15555) */
    uint8_t  frame[4];
    uint16_t written = 0U;
    const uint8_t *payPtr = NULL;
    uint16_t       payLen = 0U;
    uint8_t        nli    = 0U;
    uint32_t       cmd    = 0U;

    frame[0] = SPINEL_HEADER((uint8_t)MUX_NLI_KEEPALIVE);
    MuxSpinel_encodeUint(CMD_KEEPALIVE, frame + 1U, 3U, &written);

    EXPECT_EQ(MuxSpinel_parseFrame(frame, (uint16_t)(1U + written),
                                   &nli, &cmd, &payPtr, &payLen),
              MUX_SUCCESS);
    EXPECT_EQ(nli, (uint8_t)MUX_NLI_KEEPALIVE);
    EXPECT_EQ(cmd, (uint32_t)CMD_KEEPALIVE);
    EXPECT_EQ(payLen, 0U);
}

static void test_spinel_parse_bad_flag(void)
{
    /* Header byte with FLG != 0b10 is invalid per Spinel spec */
    const uint8_t bad[] = {0x00U, 0x06U}; /* FLG = 00 */
    const uint8_t *payPtr;
    uint16_t payLen;
    uint8_t  nli;
    uint32_t cmd;
    EXPECT_EQ(MuxSpinel_parseFrame(bad, 2U, &nli, &cmd, &payPtr, &payLen),
              MUX_ERR_INVALID);
}

/*---------------------------------------------------------------------------
 * Combined MuxSpinelHdlc_encode + decode + parse (end-to-end)
 *--------------------------------------------------------------------------*/

static void test_mux_encode_decode_ble(void)
{
    const uint8_t payload[] = {0x01U, 0x02U, 0x03U, 0x04U, 0x05U};
    uint16_t outLen = 0U, hdlcLen = 0U;
    uint8_t  nli    = 0U;
    uint32_t cmd    = 0U;
    const uint8_t *payPtr = NULL;
    uint16_t       payLen = 0U;

    EXPECT_EQ(MuxSpinelHdlc_encode((uint8_t)MUX_NLI_BLE,
                                    (uint32_t)SPINEL_CMD_PROP_VALUE_IS,
                                    payload, 5U,
                                    gEncScratch, (uint16_t)sizeof(gEncScratch),
                                    gFrame, (uint16_t)sizeof(gFrame), &outLen),
              MUX_SUCCESS);
    EXPECT(outLen > 0U);

    EXPECT_EQ(MuxHdlc_decode(gFrame, outLen, gDecoded,
                              (uint16_t)sizeof(gDecoded), &hdlcLen),
              MUX_SUCCESS);
    EXPECT_EQ(MuxSpinel_parseFrame(gDecoded, hdlcLen, &nli, &cmd,
                                   &payPtr, &payLen),
              MUX_SUCCESS);
    EXPECT_EQ(nli, (uint8_t)MUX_NLI_BLE);
    EXPECT_EQ(cmd, (uint32_t)SPINEL_CMD_PROP_VALUE_IS);
    EXPECT_EQ(payLen, 5U);
    EXPECT_EQ(memcmp(payPtr, payload, 5U), 0);
}

static void test_mux_encode_decode_zigbee(void)
{
    const uint8_t payload[] = {0xA0U, 0xB0U, 0xC0U};
    uint16_t outLen = 0U, hdlcLen = 0U;
    uint8_t  nli    = 0U;
    uint32_t cmd    = 0U;
    const uint8_t *payPtr = NULL;
    uint16_t       payLen = 0U;

    EXPECT_EQ(MuxSpinelHdlc_encode((uint8_t)MUX_NLI_ZB,
                                    (uint32_t)SPINEL_CMD_PROP_VALUE_SET,
                                    payload, 3U,
                                    gEncScratch, (uint16_t)sizeof(gEncScratch),
                                    gFrame, (uint16_t)sizeof(gFrame), &outLen),
              MUX_SUCCESS);
    MuxHdlc_decode(gFrame, outLen, gDecoded, (uint16_t)sizeof(gDecoded),
                   &hdlcLen);
    MuxSpinel_parseFrame(gDecoded, hdlcLen, &nli, &cmd, &payPtr, &payLen);

    EXPECT_EQ(nli, (uint8_t)MUX_NLI_ZB);
    EXPECT_EQ(cmd, (uint32_t)SPINEL_CMD_PROP_VALUE_SET);
    EXPECT_EQ(payLen, 3U);
    EXPECT_EQ(memcmp(payPtr, payload, 3U), 0);
}

static void test_mux_encode_decode_keepalive(void)
{
    uint16_t outLen = 0U, hdlcLen = 0U;
    uint8_t  nli    = 0U;
    uint32_t cmd    = 0U;
    const uint8_t *payPtr = NULL;
    uint16_t       payLen = 0U;

    EXPECT_EQ(MuxSpinelHdlc_encode((uint8_t)MUX_NLI_KEEPALIVE,
                                    (uint32_t)CMD_KEEPALIVE,
                                    NULL, 0U,
                                    gEncScratch, (uint16_t)sizeof(gEncScratch),
                                    gFrame, (uint16_t)sizeof(gFrame), &outLen),
              MUX_SUCCESS);
    MuxHdlc_decode(gFrame, outLen, gDecoded, (uint16_t)sizeof(gDecoded),
                   &hdlcLen);
    EXPECT_EQ(MuxSpinel_parseFrame(gDecoded, hdlcLen, &nli, &cmd,
                                   &payPtr, &payLen),
              MUX_SUCCESS);
    EXPECT_EQ(nli, (uint8_t)MUX_NLI_KEEPALIVE);
    EXPECT_EQ(cmd, (uint32_t)CMD_KEEPALIVE);
    EXPECT_EQ(payLen, 0U);
}

static void test_mux_encode_decode_keepalive_ack(void)
{
    uint16_t outLen = 0U, hdlcLen = 0U;
    uint8_t  nli    = 0U;
    uint32_t cmd    = 0U;
    const uint8_t *payPtr = NULL;
    uint16_t       payLen = 0U;

    MuxSpinelHdlc_encode((uint8_t)MUX_NLI_KEEPALIVE, (uint32_t)CMD_KEEPALIVE_ACK,
                         NULL, 0U,
                         gEncScratch, (uint16_t)sizeof(gEncScratch),
                         gFrame, (uint16_t)sizeof(gFrame), &outLen);
    MuxHdlc_decode(gFrame, outLen, gDecoded, (uint16_t)sizeof(gDecoded),
                   &hdlcLen);
    MuxSpinel_parseFrame(gDecoded, hdlcLen, &nli, &cmd, &payPtr, &payLen);

    EXPECT_EQ(nli, (uint8_t)MUX_NLI_KEEPALIVE);
    EXPECT_EQ(cmd, (uint32_t)CMD_KEEPALIVE_ACK);
    EXPECT_EQ(payLen, 0U);
}

static void test_mux_encode_payload_with_special_bytes(void)
{
    /* Payload containing HDLC_FLAG (0x7E) and HDLC_ESCAPE (0x7D) values */
    const uint8_t payload[] = {0x7EU, 0x7DU, 0xFFU, 0x7EU};
    uint16_t outLen = 0U, hdlcLen = 0U;
    uint8_t  nli    = 0U;
    uint32_t cmd    = 0U;
    const uint8_t *payPtr = NULL;
    uint16_t       payLen = 0U;

    EXPECT_EQ(MuxSpinelHdlc_encode((uint8_t)MUX_NLI_BLE,
                                    (uint32_t)SPINEL_CMD_PROP_VALUE_IS,
                                    payload, 4U,
                                    gEncScratch, (uint16_t)sizeof(gEncScratch),
                                    gFrame, (uint16_t)sizeof(gFrame), &outLen),
              MUX_SUCCESS);
    EXPECT_EQ(MuxHdlc_decode(gFrame, outLen, gDecoded,
                              (uint16_t)sizeof(gDecoded), &hdlcLen),
              MUX_SUCCESS);
    EXPECT_EQ(MuxSpinel_parseFrame(gDecoded, hdlcLen, &nli, &cmd,
                                   &payPtr, &payLen),
              MUX_SUCCESS);
    EXPECT_EQ(payLen, 4U);
    EXPECT_EQ(memcmp(payPtr, payload, 4U), 0);
}

static void test_mux_encode_invalid_nli(void)
{
    uint16_t outLen = 0U;
    EXPECT_EQ(MuxSpinelHdlc_encode(0xFFU,
                                    (uint32_t)SPINEL_CMD_PROP_VALUE_IS,
                                    NULL, 0U,
                                    gEncScratch, (uint16_t)sizeof(gEncScratch),
                                    gFrame, (uint16_t)sizeof(gFrame), &outLen),
              MUX_ERR_INVALID);
}

/*---------------------------------------------------------------------------
 * main
 *--------------------------------------------------------------------------*/

int main(void)
{
    printf("=== test_hdlc_spinel ===\n\n");

    RUN(test_crc16_known_vector);
    RUN(test_crc16_empty_is_zero);
    RUN(test_crc16_update_matches_compute);
    RUN(test_crc16_different_data_differs);

    RUN(test_hdlc_encode_has_flags);
    RUN(test_hdlc_roundtrip_simple);
    RUN(test_hdlc_roundtrip_single_byte);

    RUN(test_hdlc_escape_flag_byte_in_payload);
    RUN(test_hdlc_escape_escape_byte_in_payload);
    RUN(test_hdlc_all_bytes_roundtrip);

    RUN(test_hdlc_decode_bad_crc);
    RUN(test_hdlc_decode_missing_opening_flag);
    RUN(test_hdlc_decode_too_short);
    RUN(test_hdlc_encode_overflow);
    RUN(test_hdlc_invalid_args);

    RUN(test_spinel_uint_single_byte_values);
    RUN(test_spinel_uint_two_byte_values);
    RUN(test_spinel_uint_keepalive_cmds);
    RUN(test_spinel_uint_roundtrip_boundary_values);
    RUN(test_spinel_uint_decode_truncated);

    RUN(test_spinel_parse_ble_frame);
    RUN(test_spinel_parse_zigbee_frame);
    RUN(test_spinel_parse_keepalive_frame);
    RUN(test_spinel_parse_bad_flag);

    RUN(test_mux_encode_decode_ble);
    RUN(test_mux_encode_decode_zigbee);
    RUN(test_mux_encode_decode_keepalive);
    RUN(test_mux_encode_decode_keepalive_ack);
    RUN(test_mux_encode_payload_with_special_bytes);
    RUN(test_mux_encode_invalid_nli);

    TEST_SUMMARY();
}
