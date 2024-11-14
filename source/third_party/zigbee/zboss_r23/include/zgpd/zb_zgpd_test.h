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
/* PURPOSE: ZGPD macros for test mode support
*/

#ifndef ZB_ZGPD_TEST_H
#define ZB_ZGPD_TEST_H 1

#define ZGPD_FN(func_name) func_name ## _test

typedef struct zb_zgpd_test_callbacks_s
{
  zb_callback_t  zgpd_channel_config_ind_test_cb;
  zb_callback_t  zgpd_commiss_reply_ind_test_cb;
}
zb_zgpd_test_callbacks_t;

void zgpd_channel_config_ind(zb_uint8_t param);
void zgpd_channel_config_ind_test(zb_uint8_t param);
void zgpd_commiss_reply_ind(zb_uint8_t param);
void zgpd_commiss_reply_ind_test(zb_uint8_t param);

void zgpd_register_test_callbacks(zb_zgpd_test_callbacks_t *cbs);

#endif /* ZB_ZGPD_TEST_H */
