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
/*  PURPOSE: OTA protocol API definitions
*/

#ifndef ZB_OTA_TRANSPORT_H
#define ZB_OTA_TRANSPORT_H 1

#include "zb_common.h"
#include "zb_types.h"
#include "zb_ringbuffer.h"
#include "zb_config.h"

typedef enum zb_ota_protocol_msg_type_e
{
  OTA_START                       = 0x01,
  OTA_START_RESPONSE              = 0x02,
  OTA_PORTION                     = 0x03,
  OTA_PORTION_RESPONSE            = 0x04,
  OTA_CHECK_FW_INTEGRITY          = 0x05,
  OTA_CHECK_FW_INTEGRITY_RESPONSE = 0x06
} zb_ota_protocol_msg_type_t;

typedef enum zb_ota_protocol_error_type_e
{
  OTA_SUCCESS                         = 0x00,
  OTA_UNABLE_TO_ALLOCATE_ZBOSS_BUFFER = 0x01,
  OTA_ALREADY_IN_PROGRESS             = 0x02,
  OTA_NOT_STARTED                     = 0x03,
  OTA_INVALID_OFFSET                  = 0x04,
  OTA_INVALID_IMAGE_SIZE              = 0x05,
  OTA_INTEGRITY_CHECK_FAILED          = 0x06,
  OTA_GAIN_PORTION_FAILED             = 0x07,
  OTA_UNSUPPORTED_MESSAGE_RECEIVED    = 0x08
} zb_ota_protocol_error_type_t;

/* Struct used for error indication */
typedef struct zb_ota_protocol_error_s
{
  zb_bool_t is_host; /* whether it is host or MAC-Split device error */
  zb_ota_protocol_error_type_t error_type;
} zb_ota_protocol_error_t;

typedef ZB_PACKED_PRE struct zb_ota_protocol_hdr_s
{
  zb_uint8_t msg_type; /* value from zb_ota_protocol_msg_type_t */
  zb_uint8_t msg_len; /* payload size, zb_ota_protocol_hdr_t size excluded */
} ZB_PACKED_STRUCT
zb_ota_protocol_hdr_t;


typedef ZB_PACKED_PRE struct zb_ota_protocol_msg_start_s
{
  zb_uint32_t image_length;
} ZB_PACKED_STRUCT
zb_ota_protocol_msg_start_t;

typedef ZB_PACKED_PRE struct zb_ota_protocol_msg_start_response_s
{
  zb_uint8_t status; /* value from zb_ota_protocol_error_type_t */
} ZB_PACKED_STRUCT
zb_ota_protocol_msg_start_response_t;

typedef ZB_PACKED_PRE struct zb_ota_protocol_msg_portion_s
{
  zb_uint32_t offset;
  /* portion size is calculated based on OTA message size from OTA header,
     so there is no need to include it here */
} ZB_PACKED_STRUCT
zb_ota_protocol_msg_portion_t;

typedef ZB_PACKED_PRE struct zb_ota_protocol_msg_portion_response_s
{
  zb_uint8_t  status; /* value from zb_ota_protocol_error_type_t */
  zb_uint32_t offset; /* value offset at least helps to generate different body CRC */

} ZB_PACKED_STRUCT
zb_ota_protocol_msg_portion_response_t;

typedef ZB_PACKED_PRE struct zb_ota_protocol_msg_check_fw_integrity_s
{
  zb_uint8_t status;  /* not used yet */
} ZB_PACKED_STRUCT
zb_ota_protocol_msg_check_fw_integrity_t;

typedef ZB_PACKED_PRE struct zb_ota_protocol_msg_check_fw_integrity_response_s
{
  zb_uint8_t status; /* value from zb_ota_protocol_error_type_t */

} ZB_PACKED_STRUCT
zb_ota_protocol_msg_check_fw_integrity_response_t;


/* replace it with configurable value in the OTA context if needed */
#define OTA_FW_PORTION_SIZE 64U

/**
 *  @brief Gain cb for fetching OTA data
 *
 * @param buf [IN/OUT] - buffer for data
 * @param offset [IN]  - offset
 * @param portion [IN/OUT] - preferred portion as IN, actual portion as OUT related as data len
 *        for data that copied into buf
 *
 * @ret   RET_OK in success
 */
typedef zb_ret_t (*zb_ota_protocol_gain_cb_t)(zb_uint8_t *buf, zb_uint32_t offset, zb_uint16_t *portion);

typedef ZB_PACKED_PRE struct zb_ota_protocol_context_s
{
  zb_bool_t   ota_in_progress;
  zb_uint32_t image_size;
#ifdef ZB_MACSPLIT_HOST
  zb_uint32_t fw_offset;
  zb_ota_protocol_gain_cb_t gain_cb;
#else /* ZB_MACSPLIT_HOST */
  void       *dev;
#endif/* ZB_MACSPLIT_HOST */
} ZB_PACKED_STRUCT
zb_ota_protocol_context_t;

void zb_ota_protocol_init();
zb_ota_protocol_error_type_t zb_ota_start(zb_ota_protocol_gain_cb_t gain_cb, zb_uint32_t len);
void zb_ota_handle_packet(zb_uint8_t* buf, zb_uint_t length);

#endif /* ZB_OTA_TRANSPORT_H */
