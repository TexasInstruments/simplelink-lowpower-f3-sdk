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
/* PURPOSE: TC swapout - internal header
*/

#ifndef ZB_TCSWAP_H
#define ZB_TCSWAP_H 1


typedef enum
{
  TCSW_STATE_DONE,
  TCSW_STATE_JUST_STARTED,
  TCSW_STATE_GLOBAL_SECTION,
  TCSW_STATE_DEVICE_INFO,
  TCSW_STATE_BINDINGS
} zb_tcsw_load_stage_t;



/*
#### Backup DB format

Data to be backed up is divided into sections.

TC info section:

- flags:
  - Installcodes backed up
  - binding backed up
- ext panid (8b)
- IEEE address of TC (8b)

Devices info. Used to backup device keys as well as its installcodes.

- # of devices
- devices info []
  - device IEEE
  - contents bitmap (key/installcode/passphrase)
  - key attributes :2 zb_secur_key_attributes_t
  - security level :3 (initial join authentication + key negotiation method)
  - hashed device key [8] (opt)
  - prev hashed device key [8] (opt)
  - Installcode [8] (opt)
  - passphrase [8] (opt)

Binding info section (optional - implement later):

- # if bindings[]
- binding table []
 - uint16 src profileid
 - uint8 src ep
 - uint8 dst_type
 - IEEE dest ieee / uint16 group id

Binding info section is not required for SE.
 */

typedef enum
{
  ZB_TCSW_DB_TCINFO_BIG_ENDIAN  = (1<<0),
  ZB_TCSW_DB_TCINFO_HAS_DEVICES = (1<<1),
  ZB_TCSW_DB_TCINFO_HAS_BINDING = (1<<2)
} zb_tcsw_db_tcinfo_flag_t;

/**
TC info section:

- uint8_t flags:
  - binding backed up
- ext panid (8b)
- IEEE address of TC (8b)
*/
typedef ZB_PACKED_PRE struct
{
  zb_uint8_t       flags;
  zb_uint8_t       db_version;  /*!< 0 now  */
  zb_uint8_t       device_count;
  zb_ieee_addr_t   tc_addr;
  zb_ext_pan_id_t  tc_panid;
  zb_uint8_t       db_mic[4];      /*!< CRC of the entire DB, AES-128 */

} ZB_PACKED_STRUCT zb_tcsw_global_t;


typedef enum
{
  TCSWAP_INSTALLCODE = 0x1,
  TCSWAP_HASHED_KEY  = 0x2,
  TCSWAP_AUTH_TOKEN  = 0x4
} zb_tcswap_dev_flags_t;

/**
Devices info. Used to backup device keys as well as its installcodes.
  - device IEEE
  - contents bitmap (key/installcode/passphrase)
  - key attributes :2 zb_secur_key_attributes_t
  - security level :3 (initial join authentication + key negotiation method)
  - hashed device key [ZB_CCM_KEY_SIZE] (opt)
  - prev hashed device key [ZB_CCM_KEY_SIZE] (opt)
  - passphrase [ZB_CCM_KEY_SIZE] (opt)
  - Installcode [16] (opt)
*/
typedef ZB_PACKED_PRE struct
{
  zb_ieee_addr_t addr;
  zb_uint8_t flags;             /*!< @see zb_tcswap_dev_flags_t */
  zb_uint8_t key_attr;
  zb_uint8_t initial_join_auth;
  zb_uint8_t key_upd_method;
  zb_uint8_t ic_type;
  zb_uint8_t hashed_key[ZB_CCM_KEY_SIZE];
  zb_uint8_t prev_hashed_key[ZB_CCM_KEY_SIZE];
  zb_uint8_t passphrase[ZB_CCM_KEY_SIZE];
  zb_uint8_t ic[ZB_IC_TYPE_MAX_SIZE+2];
} ZB_PACKED_STRUCT zb_tcsw_device_t;

typedef struct
{
  zb_uint8_t dirty_bitmask; /* zb_tcswap_dirty_state_t */
  zb_bool_t have_bindings;
  zb_bool_t passkey_backup_disabled;
} zb_tcswap_flags_t;


/* TC SWAP STRUCT FOR SAVE/LOAD DB*/
typedef struct
{
  zb_tcswap_flags_t flags;
  zb_ieee_addr_t    tc_addr;
  zb_uint8_t        device_count;

  /* save/load volatile part */
  zb_uint8_t state;        //!< @see zb_tcsw_load_stage_t
  zb_uint_t  size;
  zb_uint_t  dev_idx;
  union
  {
    zb_tcsw_global_t global;
    zb_tcsw_device_t device;
    zb_uint8_t buf[sizeof(zb_tcsw_device_t)];
  } load_u;

  zb_uint_t buf_pos;
  zb_bool_t change_tc_addr;
} zb_tcswap_t;

void zb_tcsw_key_added(void);

void zb_tcsw_binding_added(void);

void zb_tcsw_trace_global_section(zb_tcsw_global_t *global);

void zb_tcsw_trace_device_section(zb_tcsw_device_t *device);

#endif  /* ZB_TCSWAP */
