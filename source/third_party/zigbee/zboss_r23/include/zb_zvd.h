/* ZBOSS Zigbee software protocol stack
 *
 * Copyright (c) 2012-2021 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
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

/**
 * PURPOSE: Zigbee Diret ZVD API
 */

#ifndef __ZV_ZVD_H__
#define __ZV_ZVD_H__ 1

#include "zb_common.h"
#include "zb_zvd_ble.h"
#include "zb_direct_common.h"

#define ZIGBEE_DIRECT_ZVD_MAX_CONNECTIONS       2

#define ZIGBEE_DIRECT_DEFAULT_ZVD_NAME      "DSR ZVD"               //!< Name of device. Will be included in the advertising data

typedef struct zbd_zvd_tunnel_service_npdu_value_s {
    zb_uint8_t                          flags;                      //!< The flags for transfer of the NPDU
    zb_uint8_t                          npdu_length;                //!< The number of octets comprising the NPDU to be transferred
    zb_uint8_t                          *npdu;                      //!< The set of octets comprising the NPDU to be transferred
} zbd_zvd_tunnel_service_npdu_value_s;


/* --- ZIGBEE DIRECT TEST SPECIFICATION: SPECIFIC FUNCTIONS TO EXECUTE TEST CASES -------------- */

zb_ret_t zbd_zvd_test_set_outgoing_packet_counter(zb_ble_conn_id_t conn_id, zb_uint32_t counter);
zb_ret_t zbd_zvd_test_write_characteristic_raw(zb_ble_conn_id_t conn_id, zbd_characteristic_tag_t charact, size_t length, const zb_uint8_t *raw);

zb_ret_t zbd_zvd_initiate_secure_session_reestablishment(
  zb_ble_conn_id_t conn_id,
  zb_uint8_t param,
  zb_tlv_psk_secrets_t psk_type,
  const zb_uint8_t *psk,
  zb_uint8_t psk_length);

#ifdef ZB_CERTIFICATION_HACKS
const zb_uint8_t *zbd_zvd_get_session_key(zb_ble_conn_id_t conn_id);
#endif /* ZB_CERTIFICATION_HACKS */

/* Moved from zboss_api_direct.h to exclude dependency from zb_tlv.h */
extern const char * str_tlv_tag(zb_tlv_tag_t tag);

#endif /* __ZV_ZVD_H__ */
