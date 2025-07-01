/* ZBOSS Zigbee software protocol stack
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 * This is unpublished proprietary source code of DSR Corporation
 * The copyright notice does not evidence any actual or intended
 * publication of such source code.
 *
 * ZBOSS is a registered trademark of Data Storage Research LLC d/b/a DSR
 * Corporation
 *
 * Commercial Usage
 * Licensees holding valid DSR Commercial licenses may use
 * this file in accordance with the DSR Commercial License
 * Agreement provided with the Software or, alternatively, in accordance
 * with the terms contained in a written agreement between you and
 * DSR.
 */
/*  PURPOSE: Sniffer header file. All sniffer-related stuff.
 */
#ifndef ZB_SNIFFER_H
#define ZB_SNIFFER_H 1

#ifdef ZB_PROMISCUOUS_MODE

/*
 * Used constant for cases when page is not set. It includes:
 * - Startup,
 * - Stopped,
 * - Paused.
 */
#define ZB_SNIFFER_PAGE_NOT_SET 0xFFU

/*
 * Page and channel may be received as 2 bytes or packed in a single byte.
 * For single byte cases, the following pattern is found:
 * -> 0bPPPC CCCC (P - Page, C - Channel).
 * -> 3 MS bits for pages 0, [28;31].
 * -> remaining 5 bits for channel.
 *
 * Out of the 8 combinations for the 'single byte' interval, 3 are free and we use 1 to flag that it is a 2 byte scheme.
 * 0b000c cccc -> page 0
 * 0b001c cccc -> page 28
 * 0b010c cccc -> page 29
 * 0b011c cccc -> page 30
 * 0b100c cccc -> page 31
 * 0b101c cccc -> free
 * 0b110c cccc -> free
 * 0b111c cccc -> used by sniffer GUI/console to indicate 2 byte scenario
 */

/*
 * In single byte schemes, used in legacy devices, the following definitions
 * allow to apply the offset from page 0 up to page 31 within the
 * {0, [28;31]} interval.
 */
#define ZB_PAGE_AND_CHANNEL_PACKED_OFFSET  5U
#define ZB_CHANNEL_LIST_PAGE_0_PACKED_IDX  ZB_CHANNEL_LIST_PAGE0_IDX
#define ZB_CHANNEL_LIST_PAGE_28_PACKED_IDX  ZB_CHANNEL_LIST_PAGE28_IDX - ZB_PAGE_AND_CHANNEL_PACKED_OFFSET // 1
#define ZB_CHANNEL_LIST_PAGE_29_PACKED_IDX  ZB_CHANNEL_LIST_PAGE29_IDX - ZB_PAGE_AND_CHANNEL_PACKED_OFFSET // 2
#define ZB_CHANNEL_LIST_PAGE_30_PACKED_IDX  ZB_CHANNEL_LIST_PAGE30_IDX - ZB_PAGE_AND_CHANNEL_PACKED_OFFSET // 3
#define ZB_CHANNEL_LIST_PAGE_31_PACKED_IDX  ZB_CHANNEL_LIST_PAGE31_IDX - ZB_PAGE_AND_CHANNEL_PACKED_OFFSET // 4

/* Right-shift amount for getting page in packed scheme */
#define ZB_PAGE_AND_CHANNEL_PACKED_PAGE_SHIFT 5
/* Mask to retrieve page (3 most-significant bits) from packed 'page and channel' byte */
#define ZB_PAGE_AND_CHANNEL_PACKED_PAGE_BITS_MASK 0xE0U
/* Mask to retrieve channel (5 least-significant bits) from packed 'page and channel' byte */
#define ZB_PAGE_AND_CHANNEL_PACKED_CHANNEL_BITS_MASK 0x1FU

/* Bitmask to apply when we want to remove '2 byte signal' from received byte */
#define ZB_PAGE_AND_CHANNEL_2_BYTE_SIGNAL_REMOVE 0x1FU

/**
   Set sniffer channel and page.
   In single byte schemes, where page and channel are packed, a single call sets page and channel.
   In 2 byte schemes, a first call of this function sets the page, whereas a second sets the channel.

   @param channel_page_cmd - channel/page value
*/
zb_ret_t zboss_sniffer_set_channel_page(zb_uint8_t channel_page_cmd);

#endif /* ZB_PROMISCUOUS_MODE */

#endif /* ZB_SNIFFER_H */
