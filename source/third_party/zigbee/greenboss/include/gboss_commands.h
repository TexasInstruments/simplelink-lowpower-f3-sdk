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
/* PURPOSE: Defines for GPD commands
*/

#ifndef GBOSS_COMMANDS_H
#define GBOSS_COMMANDS_H 1

/*! \addtogroup grpCmdOnOff */
/*! @{ */

/**
 * @brief GPD command On from cluster ID On/Off. Payloadless.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * 
 */
#define GBOSS_CMD_ON(device_config, tx_cfg) gboss_payloadless_cmd(GBOSS_APP_CMD_ID_ON, (device_config), (tx_cfg))

/**
 * @brief GPD command Off from cluster ID On/Off. Payloadless.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions.
 * 
 */
#define GBOSS_CMD_OFF(device_config, tx_cfg) gboss_payloadless_cmd(GBOSS_APP_CMD_ID_OFF, (device_config), (tx_cfg))

/**
 * @brief GPD command Toggle from cluster ID On/Off. Payloadless.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions.
 *  
 */
#define GBOSS_CMD_TOGGLE(device_config, tx_cfg) gboss_payloadless_cmd(GBOSS_APP_CMD_ID_TOGGLE, (device_config), (tx_cfg))
/*! @} */

/**
 * @brief GPD command Release. Payloadless.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions.
 *  
 */
#define GBOSS_CMD_RELEASE(device_config, tx_cfg) gboss_payloadless_cmd(GBOSS_APP_CMD_ID_RELEASE, (device_config), (tx_cfg))

/*! \addtogroup grpCmdScenes */
/*! @{ */

/**
 * @brief GPD command Recall Scene N, where N is the selected scene in the range from
 * 0 to 7, specified by Scenes cluster. Payloadless.
 *
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions.
 * @param scene_n Selection of scene, from 0 to 7
 * 
 */
#define GBOSS_CMD_RECALL_SCENE(device_config, tx_cfg, scene_n) gboss_payloadless_cmd(GBOSS_APP_CMD_ID_RECALL_SCENE0 + (scene_n), (device_config), (tx_cfg))

/**
 * @brief GPD command Store Scene N, where N is the selected scene in the range from
 * 0 to 7, specified by Scenes cluster. Payloadless.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions.
 * @param scene_n Selection of scene, from 0 to 7
 * 
 */
#define GBOSS_CMD_STORE_SCENE(device_config, tx_cfg, scene_n) gboss_payloadless_cmd(GBOSS_APP_CMD_ID_STORE_SCENE0 + (scene_n), (device_config), (tx_cfg))

/*! @} */

/*! \addtogroup grpCmdLevelControl */
/*! @{ */

/**
 * @brief GPD command Move Up from cluster ID Level Control. This is the
 * variant of the command without payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * 
 */
#define GBOSS_CMD_MOVE_UP(device_config, tx_cfg) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_UP, 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Down from cluster ID Level Control. This is the
 * variant of the command without payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * 
 */
#define GBOSS_CMD_MOVE_DOWN(device_config, tx_cfg) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_DOWN, 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Up from cluster ID Level Control. This is the
 * variant of the command with payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param rate Specifies the rate of movement in units per second
 * 
 */
#define GBOSS_CMD_MOVE_UP_W_RATE(device_config, tx_cfg, rate) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_UP, (rate), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Down from cluster ID Level Control. This is the
 * variant of the command with payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param rate Specifies the rate of movement in units per second
 * 
 */
#define GBOSS_CMD_MOVE_DOWN_W_RATE(device_config, tx_cfg, rate) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_DOWN, (rate), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Up from cluster ID Level Control. It has payload.
 * This is the variant of the command without transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the current level of "Step size" units
 * 
 */
#define GBOSS_CMD_STEP_UP(device_config, tx_cfg, step_size) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_UP, (step_size), 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Down from cluster ID Level Control. It has payload.
 * This is the variant of the command without transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the current level of "Step size" units
 * 
 */
#define GBOSS_CMD_STEP_DOWN(device_config, tx_cfg, step_size) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_DOWN, (step_size), 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Up from cluster ID Level Control. It has payload.
 * This is the variant of the command with transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the current level of "Step size" units
 * @param tr_time Specifies the time that SHALL be taken to perform the step,
 * in tenths of a second
 * 
 */
#define GBOSS_CMD_STEP_UP_W_TR_TIME(device_config, tx_cfg, step_size, tr_time) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_UP, (step_size), (tr_time), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Down from cluster ID Level Control. It has payload.
 * This is the variant of the command with transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the current level of "Step size" units
 * @param tr_time Specifies the time that SHALL be taken to perform the step,
 * in tenths of a second
 * 
 */
#define GBOSS_CMD_STEP_DOWN_W_TR_TIME(device_config, tx_cfg, step_size, tr_time) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_DOWN, (step_size), (tr_time), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief Upon receipt of this command, any Move to Level, Move or Step
 * command (and their "with On/Off" variants) currently in process SHALL be
 * terminated. Payloadless.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * 
 */
#define GBOSS_CMD_LEVEL_CONTROL_STOP(device_config, tx_cfg) gboss_payloadless_cmd(GBOSS_APP_CMD_ID_LEVEL_CONTROL_STOP, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Up with On/Off from cluster ID Level Control. 
 * This is the variant of the command without payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * 
 */
#define GBOSS_CMD_MOVE_UP_W_ON_OFF(device_config, tx_cfg) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_UP_W_ON_OFF, 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Down with On/Off from cluster ID Level Control. 
 * This is the variant of the command without payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * 
 */
#define GBOSS_CMD_MOVE_DOWN_W_ON_OFF(device_config, tx_cfg) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_DOWN_W_ON_OFF, 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Up with On/Off from cluster ID Level Control.
 * This is the variant of the command with payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param rate Specifies the rate of movement in units per second
 * 
 */
#define GBOSS_CMD_MOVE_UP_W_ON_OFF_W_RATE(device_config, tx_cfg, rate) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_UP_W_ON_OFF, (rate), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Down with On/Off from cluster ID Level Control.
 * This is the variant of the command with payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param rate Specifies the rate of movement in units per second
 * 
 */
#define GBOSS_CMD_MOVE_DOWN_W_ON_OFF_W_RATE(device_config, tx_cfg, rate) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_DOWN_W_ON_OFF, (rate), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Up with On/Off from cluster ID Level Control.
 * It has payload. This is the variant of the command without transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the current level of "Step size" units
 * 
 */
#define GBOSS_CMD_STEP_UP_W_ON_OFF(device_config, tx_cfg, step_size) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_UP_W_ON_OFF, (step_size), 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Down with On/Off from cluster ID Level Control.
 * It has payload. This is the variant of the command without transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the current level of "Step size" units
 * 
 */
#define GBOSS_CMD_STEP_DOWN_W_ON_OFF(device_config, tx_cfg, step_size) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_DOWN_W_ON_OFF, (step_size), 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Up with On/Off from cluster ID Level Control.
 * It has payload. This is the variant of the command with transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the current level of "Step size" units
 * @param tr_time Specifies the time that SHALL be taken to perform the step,
 * in tenths of a second
 * 
 */
#define GBOSS_CMD_STEP_UP_W_ON_OFF_W_TR_TIME(device_config, tx_cfg, step_size, tr_time) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_UP_W_ON_OFF, (step_size), (tr_time), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Down with On/Off from cluster ID Level Control.
 * It has payload. This is the variant of the command with transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the current level of "Step size" units
 * @param tr_time Specifies the time that SHALL be taken to perform the step,
 * in tenths of a second
 * 
 */
#define GBOSS_CMD_STEP_DOWN_W_ON_OFF_W_TR_TIME(device_config, tx_cfg, step_size, tr_time) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_DOWN_W_ON_OFF, (step_size), (tr_time), ZB_TRUE, (device_config), (tx_cfg))

/*! @} */

/*! \addtogroup grpCmdColorControl */
/*! @{ */

/**
 * @brief Upon receipt of this command, any movement currently in process
 * SHALL be terminated. Payloadless.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * 
 */
#define GBOSS_CMD_MOVE_HUE_STOP(device_config, tx_cfg) gboss_payloadless_cmd(GBOSS_APP_CMD_ID_MOVE_HUE_STOP, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Hue Up from cluster ID Color Control. This is the
 * variant of the command without payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * 
 */
#define GBOSS_CMD_MOVE_HUE_UP(device_config, tx_cfg) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_HUE_UP, 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Hue Down from cluster ID Color Control. This is the
 * variant of the command without payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * 
 */
#define GBOSS_CMD_MOVE_HUE_DOWN(device_config, tx_cfg) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_HUE_DOWN, 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Hue Up from cluster ID Color Control. This is the
 * variant of the command with payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param rate Specifies the rate of movement in steps per second. A step is
 * a change in the device’s hue of one unit
 * 
 */
#define GBOSS_CMD_MOVE_HUE_UP_W_RATE(device_config, tx_cfg, rate) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_HUE_UP, (rate), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Hue Down from cluster ID Color Control. This is the
 * variant of the command with payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param rate Specifies the rate of movement in steps per second. A step is
 * a change in the device’s hue of one unit
 * 
 */
#define GBOSS_CMD_MOVE_HUE_DOWN_W_RATE(device_config, tx_cfg, rate) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_HUE_DOWN, (rate), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Hue Up from cluster ID Color Control. It has
 * payload. This is the variant of the command without transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the device’s hue of "Step size" units.
 * 
 */
#define GBOSS_CMD_STEP_HUE_UP(device_config, tx_cfg, step_size) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_HUE_UP, (step_size),0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Hue Down from cluster ID Color Control. It has
 * payload. This is the variant of the command without transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the device’s hue of "Step size" units.
 * 
 */
#define GBOSS_CMD_STEP_HUE_DOWN(device_config, tx_cfg, step_size) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_HUE_DOWN, (step_size), 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Hue Up from cluster ID Color Control. It has
 * payload. This is the variant of the command with transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the device’s hue of "Step size" units.
 * @param tr_time Specifies the time that SHALL be taken 
 * to perform a single step, in tenths of a second
 * 
 */
#define GBOSS_CMD_STEP_HUE_UP_W_TR_TIME(device_config, tx_cfg, step_size, tr_time) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_HUE_UP, (step_size), (tr_time), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Hue Down from cluster ID Color Control. It has
 * payload. This is the variant of the command with transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the device’s hue of "Step size" units.
 * @param tr_time Specifies the time that SHALL be taken 
 * to perform a single step, in tenths of a second
 * 
 */
#define GBOSS_CMD_STEP_HUE_DOWN_W_TR_TIME(device_config, tx_cfg, step_size, tr_time) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_HUE_DOWN, (step_size), (tr_time), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief Upon receipt of this command, any movement currently in process
 * SHALL be terminated. Payloadless.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * 
 */
#define GBOSS_CMD_MOVE_SATURATION_STOP(device_config, tx_cfg) gboss_payloadless_cmd(GBOSS_APP_CMD_ID_MOVE_SATURATION_STOP, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Saturation Up from cluster ID Color Control. This
 * is the variant of the command without payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * 
 */
#define GBOSS_CMD_MOVE_SATURATION_UP(device_config, tx_cfg) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_SATURATION_UP, 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Saturation Down from cluster ID Color Control.
 * This is the variant of the command without payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * 
 */
#define GBOSS_CMD_MOVE_SATURATION_DOWN(device_config, tx_cfg) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_SATURATION_DOWN, 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Saturation Up from cluster ID Color Control.
 * This is the variant of the command with payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param rate Specifies the rate of movement in steps per second. A step is
 * a change in the device’s saturation of one unit
 * 
 */
#define GBOSS_CMD_MOVE_SATURATION_UP_W_RATE(device_config, tx_cfg, rate) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_SATURATION_UP, (rate), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Saturation Down from cluster ID Color Control.
 * This is the variant of the command with payload (rate).
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param rate Specifies the rate of movement in steps per second. A step is
 * a change in the device’s saturation of one unit
 * 
 */
#define GBOSS_CMD_MOVE_SATURATION_DOWN_W_RATE(device_config, tx_cfg, rate) gboss_cmd_move(GBOSS_APP_CMD_ID_MOVE_SATURATION_DOWN, (rate), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Saturation Up from cluster ID Color Control. 
 * It has payload. This is the variant of the command without transition
 * time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the device’s saturation of "Step size" units
 * 
 */
#define GBOSS_CMD_STEP_SATURATION_UP(device_config, tx_cfg, step_size) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_SATURATION_UP, (step_size), 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Saturation Down from cluster ID Color Control. 
 * It has payload. This is the variant of the command without transition
 * time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the device’s saturation of "Step size" units
 * 
 */
#define GBOSS_CMD_STEP_SATURATION_DOWN(device_config, tx_cfg, step_size) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_SATURATION_DOWN, (step_size), 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Saturation Up from cluster ID Color Control.
 * It has payload. This is the variant of the command with transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the device’s saturation of "Step size" units
 * @param tr_time Specifies the time that SHALL be taken 
 * to perform a single step, in tenths of a second
 * 
 */
#define GBOSS_CMD_STEP_SATURATION_UP_W_TR_TIME(device_config, tx_cfg, step_size, tr_time) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_SATURATION_UP, (step_size), (tr_time), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Saturation Down from cluster ID Color Control.
 * It has payload. This is the variant of the command with transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_size Change in the device’s saturation of "Step size" units
 * @param tr_time Specifies the time that SHALL be taken 
 * to perform a single step, in tenths of a second
 * 
 */
#define GBOSS_CMD_STEP_SATURATION_DOWN_W_TR_TIME(device_config, tx_cfg, step_size, tr_time) gboss_cmd_step(GBOSS_APP_CMD_ID_STEP_SATURATION_DOWN, (step_size), (tr_time), ZB_TRUE, (device_config), (tx_cfg))

/**
 * @brief GPD command Move Color from cluster ID Color Control.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param rate_x Specifies the rate of movement in steps per second. A step
 * is a change in the device's CurrentX attribute of one unit
 * @param rate_y Specifies the rate of movement in steps per second. A step
 * is a change in the device's CurrentY attribute of one unit
 * 
 */
#define GBOSS_CMD_MOVE_COLOR(device_config, tx_cfg, rate_x, rate_y) gboss_cmd_move_color(GBOSS_APP_CMD_ID_MOVE_COLOR, (rate_x), (rate_y), (device_config), (tx_cfg))

/**
 * @brief GPD command Step Color from cluster ID Color Control. This is the
 * variant of the command without transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_x Specifies the change to be added to the device's CurrentX
 * attribute
 * @param step_y Specifies the change to be added to the device's CurrentX
 * attribute
 * 
 */
#define GBOSS_CMD_STEP_COLOR(device_config, tx_cfg, step_x, step_y) gboss_cmd_step_color(GBOSS_APP_CMD_ID_STEP_COLOR, (step_x), (step_y), 0, ZB_FALSE, (device_config), (tx_cfg))

/**
 * @brief GPD command Step Color from cluster ID Color Control. This is the
 * variant of the command with transition time.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param step_x Specifies the change to be added to the device's CurrentX
 * attribute
 * @param step_y Specifies the change to be added to the device's CurrentX
 * attribute
 * @param tr_time Specifies the time that SHALL be taken 
 * to perform the color change, in tenths of a second
 */
#define GBOSS_CMD_STEP_COLOR_W_TR_TIME(device_config, tx_cfg, step_x, step_y, tr_time) gboss_cmd_step_color(GBOSS_APP_CMD_ID_STEP_COLOR, (step_x), (step_y), (tr_time), ZB_TRUE, (device_config), (tx_cfg))

/*! @} */

/*! \addtogroup grpCmdDoorLock */
/*! @{ */

/**
 * @brief GPD command Lock Door from cluster ID Door Lock. Payloadless.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * 
 */
#define GBOSS_CMD_LOCK_DOOR(device_config, tx_cfg) gboss_payloadless_cmd(GBOSS_APP_CMD_ID_LOCK_DOOR, (device_config), (tx_cfg))

/**
 * @brief GPD command Unlock Door from cluster ID Door Lock. Payloadless.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * 
 */
#define GBOSS_CMD_UNLOCK_DOOR(device_config, tx_cfg) gboss_payloadless_cmd(GBOSS_APP_CMD_ID_UNLOCK_DOOR, (device_config), (tx_cfg))

/*! @} */

/*! \addtogroup grpCmdAttributeReporting */
/*! @{ */

/**
 * @brief GPD command ZCL Attribute Reporting.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param zb_cluster_id Cluster ID used for this report
 * @param attr_count Number of attributes being reported
 * @param attr_reports Array of reports, which contain Attribute ID, 
 * Attribute Type and Attribute Data for each one
 * 
 */
#define GBOSS_CMD_ID_ATTR_REPORT(device_config, tx_cfg, zb_cluster_id, attr_count, attr_reports) gboss_cmd_attr_report((zb_cluster_id), (attr_count), (attr_reports), (device_config), (tx_cfg))


/**
 * @brief GPD command ZCL Attribute Reporting.
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param zb_cluster_id Cluster ID used for this report
 * @param attr_count Number of attributes being reported
 * @param attr_reports Array of reports, which contain Attribute ID, 
 * Attribute Type and Attribute Data for each one
 * 
 */
#define GBOSS_CMD_ID_ATTR_REPORT(device_config, tx_cfg, zb_cluster_id, attr_count, attr_reports) gboss_cmd_attr_report((zb_cluster_id), (attr_count), (attr_reports), (device_config), (tx_cfg))

/*! @} */

/*! \addtogroup grpBidirCmd */
/*! @{ */

/**
 * @brief GPD command ZCL Attribute Reporting for Bidirectional on Operational mode
 * 
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param zb_cluster_id Cluster ID used for this report
 * @param attr_count Number of attributes being reported
 * @param attr_reports Array of reports, which contain Attribute ID, 
 * Attribute Type and Attribute Data for each one
 * @patram rx_after_tx The value of RxAfterTx flag in GPDF header
 * 
 */
#define GBOSS_CMD_ID_ATTR_REPORT_RX(device_config, tx_cfg, zb_cluster_id, attr_count, attr_reports, rx_after_tx) gboss_cmd_attr_report_rx((zb_cluster_id), (attr_count), (attr_reports), (device_config), (tx_cfg), (rx_after_tx))

/**
 * @brief GPD command Attributes Request
 * 
 * To be transmitted by GPD which supports Bidirectional in Operational mode. Requests GPS to send Write Attributes to GPD.
 *
 * @param device_config Describes main configuration constants of a GP device
 * @param tx_cfg TX configuration outlining how a transmission should proceed concerning
 * the channels to be used and the number of repetitions
 * @param zb_cluster_id Cluster ID used for this report
 * @param attr_count Number of attributes being reported
 * @param attr_reports Array of reports, which contain Attribute ID, 
 * Attribute Type and Attribute Data for each one
 * 
 * @snippet zgpd_bidir_battery_attr_read_write.c attr_req
 */
#define GBOSS_CMD_ATTR_REQUEST(device_config, tx_cfg, zb_cluster_id, attr_count, attr_reports) gboss_cmd_attr_request((zb_cluster_id), (attr_count), (attr_reports), (device_config), (tx_cfg))


/*! @} */

/*! @} */


#endif /* GBOSS_COMMANDS_H */
