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
/* PURPOSE: Zigbee nvram management
*/

#ifndef ZB_NVRAM_GLOBAL_H
#define ZB_NVRAM_GLOBAL_H 1

#if defined ZB_USE_NVRAM || defined doxygen

#include "zb_nvram.h"

typedef struct zb_nvram_pos_payload_checker_s
{
  zb_lbitfield_t pos:30;
  zb_lbitfield_t page:2;
  zb_uint16_t payload_length;
  zb_uint16_t crc; /* < CRC field is needed to accumulate crc while writing */
} zb_nvram_pos_payload_checker_t;

typedef ZB_PACKED_PRE struct zb_nvram_page_state_s
{
  zb_nvram_ver_t nvram_ver;
  zb_bitfield_t erased:1;
  zb_bitfield_t erase_in_progress:1;
  zb_bitfield_t reserved:6;
  zb_bitfield_t alignment:8;
} ZB_PACKED_STRUCT zb_nvram_page_state_t;

#define ZB_NVRAM_MAX_N_PAGES 2U

#ifdef ZB_NVRAM_ENABLE_DIRECT_API

typedef struct zb_nvram_ds_pos_s
{
  zb_uint32_t pos;
  zb_uint8_t page;
} zb_nvram_ds_pos_t;

#endif /* ZB_NVRAM_ENABLE_DIRECT_API */

typedef struct zb_nvram_custom_ds_entry_s
{
  zb_nvram_ds_filter_cb_t filter;
  zb_nvram_ds_get_length_cb_t get_length;
  zb_nvram_ds_get_version_cb_t get_version;
  zb_nvram_ds_read_cb_t read;
  zb_nvram_ds_write_cb_t write;
} zb_nvram_custom_ds_entry_t;

#define ZB_NVRAM_CUSTOM_DS_MAX_NUMBER 2U

typedef struct zb_nvram_custom_ds_ctx_s
{
  zb_nvram_custom_ds_entry_t handlers[ZB_NVRAM_CUSTOM_DS_MAX_NUMBER];
  zb_size_t ds_cnt;
} zb_nvram_custom_ds_ctx_t;


/**
   NVRAM global context
*/
typedef struct zb_nvram_globals_s
{
  zb_nvram_read_app_data_t      read_app_data_cb[ZB_NVRAM_APP_DATASET_NUMBER];
  zb_nvram_write_app_data_t     write_app_data_cb[ZB_NVRAM_APP_DATASET_NUMBER];
  zb_nvram_get_app_data_size_t  get_app_data_size_cb[ZB_NVRAM_APP_DATASET_NUMBER];
  zb_nvram_custom_ds_ctx_t      custom_ds_ctx;

  zb_nvram_ds_filter_cb_t ds_filter_cb; /*!< If assigned function to be called to filter out some datasets at load  */
  zb_nvram_tl_t current_time_label;     /*!< Current time label. Max from wrote NVRAM dataset header */
  zb_nvram_page_state_t pages[ZB_NVRAM_MAX_N_PAGES];
  zb_uint32_t current_pos;              /*!< Index of first free byte in Current Page NVRAM */
  zb_uint8_t current_page;              /*!< Current page NVRAM */

  zb_bitfield_t transaction_ongoing:1; /*!< @see @ref nvram_transaction_state  */
  zb_bitfield_t empty:1;             /*!< if true, NVRAM is empty (maybe, except NWK
                                 * counter), so device is factory new  */
  zb_bitfield_t inited:1;            /*!< if true, NVRAM is inited (read/write operations are
                                      * allowed) */
  zb_bitfield_t alignment:5;

#ifdef ZB_NVRAM_ENABLE_DIRECT_API
  /* positions of datasets; updated on each write operation; used for read operations */
  zb_nvram_ds_pos_t ds_pos[ZB_NVRAM_DATASET_NUMBER];
#endif /* ZB_NVRAM_ENABLE_DIRECT_API */

  /* Checker for nvram read/write calls */
  zb_nvram_pos_payload_checker_t gs_nvram_write_checker;
  zb_nvram_pos_payload_checker_t gs_nvram_read_checker;
  zb_nvram_pos_payload_checker_t gs_nvram_read_checker_cache;
} zb_nvram_globals_t;

#define ZB_NVRAM() ZG->nvram

#endif


#endif /* ZB_NVRAM_GLOBAL_H */
