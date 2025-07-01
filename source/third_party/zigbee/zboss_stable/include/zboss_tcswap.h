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

#if defined TC_SWAPOUT && defined ZB_COORDINATOR_ROLE

/*! \addtogroup ZB_SECUR */
/*! @{ */

typedef enum
{
  ZB_TCSWAP_SECUR = 1,
  ZB_TCSWAP_BINDINGS = 2
} zb_tcswap_dirty_state_t;


/**
   Return bitmask of the TC swapout DB backup necessity

   @return bitmask of @ref zb_tcswap_dirty_state_t values. 0 if no backup required.
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
   @return ZB_TRUE if backup completed successfully
 */
zb_ret_t zb_tcsw_start_backup_db(zb_uint8_t *backup_buf, zb_uint_t buf_size);

/**
   Start TC swapout restore procedure.
   @return ZB_TRUE if restore completed successfully
 */
zb_ret_t zb_tcsw_start_restore_db(zb_uint8_t *restore_buf, zb_uint_t buf_size, zb_bool_t change_tc_addr);

/*! @} */

#endif  /* defined TC_SWAPOUT && defined ZB_COORDINATOR_ROLE */

#endif  /* ZBOSS_TCSWAP_H */
