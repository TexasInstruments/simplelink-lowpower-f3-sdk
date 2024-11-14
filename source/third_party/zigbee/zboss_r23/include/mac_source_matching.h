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
/* PURPOSE: MAC pending bit source matching
*/

#ifndef MAC_SOURCE_MATCHING_INCLUDED
#define MAC_SOURCE_MATCHING_INCLUDED 1

#include "zb_common.h"
/*! \addtogroup ZB_MAC */
/*! @{ */

#ifdef ZB_MAC_PENDING_BIT_SOURCE_MATCHING

#if defined ZB_MAC_SOFTWARE_PB_MATCHING
typedef enum source_matching_search_action_e
{
  SEARCH_ACTION_NO_ACTION,
  SEARCH_ACTION_ALLOW_CREATE,
  SEARCH_ACTION_DELETE_IF_EXIST
} source_matching_search_action_t;

zb_bool_t mac_child_hash_table_search(zb_uint16_t short_addr,
                                      zb_uint32_t *ref_p,
                                      zb_bool_t   called_from_interrupt,
                                      source_matching_search_action_t action);
zb_uint8_t mac_software_src_match_add_short_addr(zb_uint16_t short_addr);
zb_uint8_t mac_software_src_match_delete_short_addr(zb_uint16_t short_addr);
void mac_software_src_match_tbl_drop(void);
void mac_software_src_match_short_set_pending_bit(zb_uint16_t short_addr, zb_uint8_t pending);
#endif /* ZB_MAC_SOFTWARE_PB_MATCHING */

#if defined ZB_MAC_SWITCHABLE_PB_MATCHING

void mac_source_matching_set_src_matching_type(zb_uint8_t type);
zb_bool_t is_mac_source_matching_software(void);

#define MAC_SOURCE_MATCHING_TYPE_SOFTWARE 0U
#define MAC_SOURCE_MATCHING_TYPE_HARDWARE 1U

#define SET_MAC_SRC_MATCHING_TYPE_SOFTWARE() \
  mac_source_matching_set_src_matching_type(MAC_SOURCE_MATCHING_TYPE_SOFTWARE)

#define SET_MAC_SRC_MATCHING_TYPE_HARDWARE() \
  mac_source_matching_set_src_matching_type(MAC_SOURCE_MATCHING_TYPE_HARDWARE)

#else

#define SET_MAC_SRC_MATCHING_TYPE_SOFTWARE()
#define SET_MAC_SRC_MATCHING_TYPE_HARDWARE()

#endif /* ZB_MAC_SWITCHABLE_PB_MATCHING */

/*
 * Call functions via macro to choose between SW and HW
 * implementations of source address & pending bit matching.
 *
 * Define one of the following macros in your vendor
 * or platform config files to enable PB matching:
 * 1) ZB_MAC_SWITCHABLE_PB_MATCHING - to enable both of SW and HW pending bit matching types
 * 2) ZB_MAC_SOFTWARE_PB_MATCHING - software PB matching only
 * 3) ZB_MAC_HARDWARE_PB_MATCHING - hardware PB matching only
 * 4) ZB_MAC_STICKY_PENDING_BIT - sticky pending bit
 *
 * ZB_MAC_SOFTWARE_PB_MATCHING and ZB_MAC_HARDWARE_PB_MATCHING will be
 * defined automatically when you define ZB_MAC_SWITCHABLE_PB_MATCHING.
 *
 * Do not define SW and HW pending bit matching at the same time!
 * Use ZB_MAC_SWITCHABLE_PB_MATCHING instead of it!
 *
 * Note: ZB_MAC_PENDING_BIT_SOURCE_MATCHING must be enabled either!
 */

/* **** SOFTWARE AND HARDWARE PENDING BIT MATCHING **** */
/*
 * ZB_MAC_SWITCHABLE_PB_MATCHING allows to switch PB matching behavior in runtime.
 *
 * Use SET_MAC_SRC_MATCHING_TYPE_SOFTWARE() and SET_MAC_SRC_MATCHING_TYPE_HARDWARE()
 * macros in your platform to do it.
 *
 * For example, you can set hardware PB matching after switching to 2.4 GHz
 * and software PB matching after switching to sub-GHz.
 *
 * Note: software PB matching is used by default in this case.
 */
#if defined ZB_MAC_SWITCHABLE_PB_MATCHING

#define ZB_SRC_MATCH_ADD_SHORT_ADDR(index, short_addr)          \
  (is_mac_source_matching_software() ?                          \
  mac_software_src_match_add_short_addr(short_addr) :           \
  ZB_TRANSCEIVER_SRC_MATCH_ADD_SHORT_ADDR(index, short_addr))

#define ZB_SRC_MATCH_DELETE_SHORT_ADDR(index, short_addr)       \
  (is_mac_source_matching_software() ?                          \
  mac_software_src_match_delete_short_addr(short_addr) :        \
  ZB_TRANSCEIVER_SRC_MATCH_DELETE_SHORT_ADDR(index, short_addr))

#define ZB_SRC_MATCH_TBL_DROP()                \
  (is_mac_source_matching_software() ?         \
  mac_software_src_match_tbl_drop() :          \
  ZB_TRANSCEIVER_SRC_MATCH_TBL_DROP())

#define ZB_SRC_MATCH_SHORT_SET_PENDING_BIT(short_addr, pending)         \
  (is_mac_source_matching_software() ?                                  \
  mac_software_src_match_short_set_pending_bit(short_addr, pending) :   \
  ZB_TRANSCEIVER_SRC_MATCH_SHORT_SET_PENDING_BIT(short_addr, pending))

#define ZB_SRC_MATCH_ADD_IEEE_ADDR(index, ieee_addr)            \
  (is_mac_source_matching_software() ?                          \
  (MAC_SUCCESS) :                                               \
  ZB_TRANSCEIVER_SRC_MATCH_ADD_IEEE_ADDR(index, ieee_addr))     \

#define ZB_SRC_MATCH_DELETE_IEEE_ADDR(index, ieee_addr)         \
  (is_mac_source_matching_software() ?                          \
  (MAC_SUCCESS) :                                               \
  ZB_TRANSCEIVER_SRC_MATCH_DELETE_IEEE_ADDR(index, ieee_addr))

#ifdef ZB_MAC_POLL_INDICATION_CALLS_REDUCED
void mac_software_src_match_update_poll_ind_call_timeout(zb_uint16_t short_addr, zb_uint16_t poll_timeout);

#define ZB_MAC_UPDATE_POLL_IND_CALL_TIMEOUT(index, short_addr, tmo)         \
  do {                                                                      \
    if (is_mac_source_matching_software())                                  \
    {                                                                       \
      mac_software_src_match_update_poll_ind_call_timeout(short_addr, tmo); \
    }                                                                       \
  } while (0)

#endif /* ZB_MAC_POLL_INDICATION_CALLS_REDUCED */

/* **** SOFTWARE PENDING BIT MATCHING ONLY **** */
#elif defined ZB_MAC_SOFTWARE_PB_MATCHING

#define ZB_SRC_MATCH_ADD_SHORT_ADDR(index, short_addr)  \
  mac_software_src_match_add_short_addr(short_addr)
#define ZB_SRC_MATCH_DELETE_SHORT_ADDR(index, short_addr) \
  mac_software_src_match_delete_short_addr(short_addr)
#define ZB_SRC_MATCH_TBL_DROP() \
  mac_software_src_match_tbl_drop()
#define ZB_SRC_MATCH_SHORT_SET_PENDING_BIT(short_addr, pending) \
  mac_software_src_match_short_set_pending_bit(short_addr, pending)
#define ZB_SRC_MATCH_ADD_IEEE_ADDR(index, ieee_addr) (MAC_SUCCESS)
#define ZB_SRC_MATCH_DELETE_IEEE_ADDR(index, ieee_addr) (MAC_SUCCESS)

#ifdef ZB_MAC_POLL_INDICATION_CALLS_REDUCED
void mac_software_src_match_update_poll_ind_call_timeout(zb_uint16_t short_addr, zb_uint16_t poll_timeout);

#define ZB_MAC_UPDATE_POLL_IND_CALL_TIMEOUT(index, short_addr, tmo)     \
  mac_software_src_match_update_poll_ind_call_timeout(short_addr, tmo)
#endif /* ZB_MAC_POLL_INDICATION_CALLS_REDUCED */

/* **** STICKY PENDING BIT ONLY **** */
#elif (defined ZB_MAC_STICKY_PENDING_BIT) /* #if !ZB_MAC_SOFTWARE_PB_MATCHING */

#define ZB_SRC_MATCH_ADD_SHORT_ADDR(index, short_addr) \
  ZB_TRANSCEIVER_SRC_MATCH_ADD_SHORT_ADDR(index, short_addr)
#define ZB_SRC_MATCH_DELETE_SHORT_ADDR(index, short_addr) \
  ZB_TRANSCEIVER_SRC_MATCH_DELETE_SHORT_ADDR(index, short_addr)
#define ZB_SRC_MATCH_TBL_DROP() \
  ZB_TRANSCEIVER_SRC_MATCH_TBL_DROP()
#define ZB_SRC_MATCH_SHORT_SET_PENDING_BIT(short_addr, pending) \
  ZB_TRANSCEIVER_SRC_MATCH_SHORT_SET_PENDING_BIT(short_addr, pending)
#define ZB_SRC_MATCH_ADD_IEEE_ADDR(index, ieee_addr) \
  ZB_TRANSCEIVER_SRC_MATCH_ADD_IEEE_ADDR(index, ieee_addr)
#define ZB_SRC_MATCH_DELETE_IEEE_ADDR(index, ieee_addr) \
  ZB_TRANSCEIVER_SRC_MATCH_DELETE_IEEE_ADDR(index, ieee_addr)

/* **** HARDWARE PENDING BIT MATCHING ONLY **** */
#elif (defined ZB_MAC_HARDWARE_PB_MATCHING)

#define ZB_SRC_MATCH_ADD_SHORT_ADDR(index, short_addr) \
  ZB_TRANSCEIVER_SRC_MATCH_ADD_SHORT_ADDR(index, short_addr)
#define ZB_SRC_MATCH_DELETE_SHORT_ADDR(index, short_addr) \
  ZB_TRANSCEIVER_SRC_MATCH_DELETE_SHORT_ADDR(index, short_addr)
#define ZB_SRC_MATCH_TBL_DROP() \
  ZB_TRANSCEIVER_SRC_MATCH_TBL_DROP()
#define ZB_SRC_MATCH_SHORT_SET_PENDING_BIT(short_addr, pending) \
  ZB_TRANSCEIVER_SRC_MATCH_SHORT_SET_PENDING_BIT(short_addr, pending)
#define ZB_SRC_MATCH_ADD_IEEE_ADDR(index, ieee_addr) \
  ZB_TRANSCEIVER_SRC_MATCH_ADD_IEEE_ADDR(index, ieee_addr)
#define ZB_SRC_MATCH_DELETE_IEEE_ADDR(index, ieee_addr) \
  ZB_TRANSCEIVER_SRC_MATCH_DELETE_IEEE_ADDR(index, ieee_addr)

#endif /* ZB_MAC_SOFTWARE_PB_MATCHING */

#else

#ifdef ZB_ROUTER_ROLE
#error "Please, define ZB_MAC_PENDING_BIT_SOURCE_MATCHING and one of the following PB matching types: ZB_MAC_SWITCHABLE_PB_MATCHING, ZB_MAC_SOFTWARE_PB_MATCHING, ZB_MAC_HARDWARE_PB_MATCHING or ZB_MAC_STICKY_PENDING_BIT"
#else
#define SET_MAC_SRC_MATCHING_TYPE_SOFTWARE()
#define SET_MAC_SRC_MATCHING_TYPE_HARDWARE()
#endif /* ZB_ROUTER_ROLE*/

#endif /* ZB_MAC_PENDING_BIT_SOURCE_MATCHING */

#endif /* MAC_SOURCE_MATCHING_INCLUDED */
