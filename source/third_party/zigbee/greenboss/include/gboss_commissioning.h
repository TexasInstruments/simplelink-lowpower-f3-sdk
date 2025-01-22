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
/* PURPOSE: GP Commissioning API
*/

#ifndef GBOSS_COMMISSIONING_H
#define GBOSS_COMMISSIONING_H 1

/**
@addtogroup commis
@{
*/


/**
 * @brief Does one step of unidirectional commissioning, or decommissions
 * if a device was marked as "commissioned".
 *
 * GreenBOSS supposes that unidirectional commissioning is used at
 * battery-less devices, so it's likely that the GPD doesn't have enough power to perform
 * anything after that function return.
 *
 * With unidirectional commissioning GreenBOSS marks device as "commissioned"
 * at transmitting first application packet (GPD command), like GBOSS_CMD_ON(),
 * after gboss_unidir_commissioning_step() was called 1 or more times.
 *
 * @param device_config GPD configuration data structure
 * @param tx_cfg Configuration of transmission - channels and number of repetitions to be used
 * @return zb_ret_t RET_OK if the commissioning was successfully requested, RET_ERROR otherwise
 *
 * @snippet zgpd_unidir_onoff.c unidir_step
 */
zb_ret_t gboss_unidir_commissioning_step(const gboss_device_config_t* device_config,
                                         const gboss_tx_cfg_t *tx_cfg);


/**
 * @brief Decommissions the device - unidirectional.
 * 
 * @param device_config GPD configuration data structure
 * @param tx_cfg Configuration of transmission - channels and number of repetitions to be used
 * @return zb_ret_t zb_ret_t RET_OK if the decommissioning was successful,
 * RET_ERROR otherwise
 */
zb_ret_t gboss_unidir_decommission(const gboss_device_config_t* device_config,
                                   const gboss_tx_cfg_t *tx_cfg);

/**
   Initializes bidirectional operational mode with an application parameters

 * @param device_config GPD configuration data structure
 * @param tx_cfg Configuration of transmission - channels and number of repetitions to be used
 *
 * @snippet zgpd_bidir_battery_attr_read_write.c bidir_init
 */
void gboss_bidir_operational_init(const gboss_device_config_t* device_config,
                                  const gboss_tx_cfg_t *tx_cfg);

/**
   Does one step of bidirectional commissioning.

   Repeats commissioning steps until gboss_bidir_is_commissionned() returns true.

 * @param device_config GPD configuration data structure
 * @param tx_cfg Configuration of transmission - channels and number of repetitions to be used
 * @return zb_ret_t RET_OK if the commissioning was successfully requested,
 * RET_ERROR otherwise
 *
 * @snippet zgpd_bidir_battery_onoff.c bidir_step
 * @snippet zgpd_bidir_batteryless_onoff.c bidir_step
 */
zb_ret_t gboss_bidir_commissioning_step(const gboss_device_config_t* device_config,
                                        const gboss_tx_cfg_t *tx_cfg);

/**
   Checks if this bidirectional GPP is commissioned.

   @return True if commissioned, else false

   @snippet zgpd_bidir_battery_onoff.c bidir_step
   @snippet zgpd_bidir_batteryless_onoff.c bidir_step
 */
zb_bool_t gboss_bidir_is_commissionned(void);

/**
 * @brief Decommissions the device - bidirectional.
 * 
 * @param device_config GPD configuration data structure
 * @param tx_cfg Configuration of transmission - channels and number of repetitions to be used
 * @return zb_ret_t zb_ret_t RET_OK if the decommissioning was successful,
 * RET_ERROR otherwise
 */
zb_ret_t gboss_bidir_decommission(const gboss_device_config_t* device_config,
                                  const gboss_tx_cfg_t *tx_cfg);

/**
   Configures the device as a battery-powered bidirectional commissioning GPD.

   gboss_bidir_battery_mode() SHALL be called at the
   application initialization before gboss_bidir_is_commissionned().

   @snippet zgpd_bidir_battery_onoff.c bidir_step
 */
void gboss_bidir_battery_mode(void);

/**
   Configures the device as a battery-less bidirectional commissioning GPD.

   gboss_bidir_batteryless_mode()SHALL be called at the
   application initialization before gboss_bidir_is_commissionned().

   @snippet zgpd_bidir_battery_onoff.c bidir_step
 */
void gboss_bidir_batteryless_mode(void);

/*! @} */

/**
@addtogroup bidirRxTx
@{
*/

/**
 Receive a packet from GPS in Bidirectional in Operational mode

 @param timeout_ms - timeout to wait for rx
 @param cmd_id - received command id
 @param payload - received command payload
 @param length - received payload length

 @return RET_EMPTY if nothing received, else RET_OK
 */
zb_ret_t gboss_wait_rx(zb_uint32_t timeout_ms,
                       zb_uint8_t *cmd_id,
                       zb_uint8_t **payload,
                       zb_size_t *length);
/*! @} */

#endif /* GBOSS_COMMISSIONING_H */
