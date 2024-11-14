/* GreenBOSS Zigbee Green Power Device software protocol stack
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
 * www.dsr-iot.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 * This is unpublished proprietary source code of DSR Corporation
 * The copyright notice does not evidence any actual or intended
 * publication of such source code.
 *
 * GreenBOSS is a registered trademark of Data Storage Research LLC d/b/a DSR
 * Corporation
 *
 * Commercial Usage
 * Licensees holding valid DSR Commercial licenses may use
 * this file in accordance with the DSR Commercial License
 * Agreement provided with the Software or, alternatively, in accordance
 * with the terms contained in a written agreement between you and
 * DSR.
 */
/* PURPOSE: Defines routines for unidirectional commissioning procedure
*/

#ifndef ZB_COMMISSIONING_H
#define ZB_COMMISSIONING_H 1

/*! \addtogroup comm_int */
/*! @{ */

#define ZGPD_COMM_CHANNEL_REQ_STATE 0U
#define ZGPD_COMM_COMMISSIONING_STATE 1U
#define ZGPD_COMM_SUCCESS_STATE 2U
#define ZGPD_COMM_OPERATIONAL_STATE 3U
#define ZGPD_COMM_DECOMMISSIONING_STATE 4U

#define ZGPD_COMM_N_STAGES (ZGPD_COMM_OPERATIONAL_STATE+1)

/**
 * @brief Returns channel index, which was used on the previous commissioning stage.
 *
 * The function should be used by application to determine the channel group, which
 * should be used for communication.
 *
 * @note Calling this function automatically saves last used index for
 * commissioning as a current working index. Any subsequent calls will return the
 * same value until #zb_commission_get_next_index() will be called for another
 * commissioning.
 *
 * @return zb_int16_t Current channel index
 */
zb_int16_t zb_commissioned_index(void);

void zgpd_bidir_save_comm_state(zb_uint8_t state, 
				zb_uint8_t channel_idx,
				zb_uint8_t *key, 
				zb_uint8_t key_type,
				zb_uint16_t pan_id);

void zgpd_bidir_reset_comm_context(void);

zb_uint8_t zgpd_bidir_get_comm_state(void);

zb_int16_t zgpd_bidir_get_channel_idx(void);

zb_uint16_t zgpd_bidir_get_pan_id(void);

zb_ret_t zb_commission_prepare(zb_uint32_t seq_number,
                               const gboss_device_config_t* device_config,
                               zb_uint8_t *buffer,
                               zb_uint8_t *buffer_length);

/*! @} */


#endif /* ZB_COMMISSIONING_H */
