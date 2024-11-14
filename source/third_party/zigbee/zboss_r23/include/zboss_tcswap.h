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
/* PURPOSE: TC swapout - public API
*/

#ifndef ZBOSS_TCSWAP_H
#define ZBOSS_TCSWAP_H 1

#if (defined TC_SWAPOUT && defined ZB_COORDINATOR_ROLE) || defined DOXYGEN

/*! @addtogroup r23_tcsw */
/*! @{ */

/**
   Bits of bitmask defining which part of TC database need to be backed up
 */
typedef enum
{
  ZB_TCSWAP_SECUR = 1,
  ZB_TCSWAP_BINDINGS = 2
} zb_tcswap_dirty_state_t;


/**
   Return bitmask of the TC swapout DB backup necessity

   @return bitmask of @ref zb_tcswap_dirty_state_t values. 0 if no backup required.

   @snippet tc_swap_out/tcso_zc.c tcsw_backup1_snippet
   @snippet tc_swap_out/tcso_zc.c tcsw_backup2_snippet
 */
zb_uint_t zb_tcsw_need_backup(void);

/**
 * Return status of TC swapout DB
   @return ZB_TRUE if save/load TC swapout DB in progress
 */
zb_bool_t zb_tcsw_is_busy(void);

/**
 * Calculate size of TC swapout DB
   @return TC swapout DB size
 */
zb_uint_t zb_tcsw_calculate_db_size(void);

/**
 * Check buffer size for TC swapout DB save
   @return ZB_FALSE if size is too small
 */
zb_bool_t zb_tcsw_check_buffer_size(zb_uint_t size);

/**
   Start TC swapout backup procedure.

    @return - RET_OK if backup procesdue is started, RET_ERROR in other cases.

   @snippet tc_swap_out/tcso_zc.c tcsw_backup1_snippet
   @snippet tc_swap_out/tcso_zc.c tcsw_backup2_snippet
 */
zb_ret_t zb_tcsw_start_backup_db(void);

/**
   Get next portion of TC Swapout DB.

   @param buf - buffer to place data into
   @param buf_size - buffer size

   @return - portion size or 0 if nothing more to backup.

   @snippet tc_swap_out/tcso_zc.c tcsw_backup1_snippet
   @snippet tc_swap_out/tcso_zc.c tcsw_backup2_snippet
 */
zb_uint_t zb_tcsw_get_portion(zb_uint8_t *buf, zb_uint_t buf_size);

/**
   Start TC swapout restore procedure.
   @return ZB_TRUE if restore completed successfully

   @snippet tc_swap_out/tcso_zc.c tcsw_restore_snippet
 */
zb_ret_t zb_tcsw_start_restore_db(zb_bool_t change_tc_addr);


/**
   TC swapout, ZC side - provide TC DB portion to ZBOSS TC.

   To be called N times after zb_tcsw_start_restore_db().

   @param buf - data buffer
   @param len - buffer length

   @snippet tc_swap_out/tcso_zc.c tcsw_restore_snippet
 */
zb_ret_t zc_tcsw_restore_portion(zb_uint8_t *buf, zb_uint_t len);

/**
   Complete TC DB restore.

   @snippet tc_swap_out/tcso_zc.c tcsw_restore_snippet
 */
zb_ret_t zc_tcsw_restore_finish(void);

/**
   Switch on/off backup of the established authentication token into TC swapout DB.

   By default such backup is enabled.

   @param enable - if ZB_TRUE, enable backup, else disable
 */
void zb_tcsw_enable_passkey_backup(zb_bool_t enable);


/*! @} r23_tcsw */

#endif  /* defined TC_SWAPOUT && defined ZB_COORDINATOR_ROLE */

#endif  /* ZBOSS_TCSWAP_H */
