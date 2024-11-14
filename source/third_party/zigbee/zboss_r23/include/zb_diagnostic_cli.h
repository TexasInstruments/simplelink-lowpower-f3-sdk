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
/* PURPOSE:
*/

#ifndef ZB_DIAGNOSTIC_CLI_H
#define ZB_DIAGNOSTIC_CLI_H 1

#include "zb_types.h"

#define ZB_CLI_DEVICE_NAME "zb_cli"
#define ZB_CLI_MAX_DEVICE_NAME_LEN 64U

/**
 * @brief Initializes diagnostic CLI.
 * The function can be called at any time after ZB_INIT()
 * (even before zboss_start())
 */
void zb_diagnostic_cli_init(void);

/**
 * @brief De-initializes diagnostic CLI.
 * The function can be called at any time after ZB_INIT()
 * (even before or after zboss_start())
 */
void zb_diagnostic_cli_deinit(void);

/**
 * @brief Initializes selected transport type assuming it is not already used by the application
 * If application requires transport sharing between CLI and application, the function can not be used.
 *
 * Application can interact with CLI without automatic transport initialization by the following way:
 *  - Input string can be sent to CLI by zb_diagnostic_cli_handle_input() call.
 *  - To handle output from CLI, application can override zb_diagnostic_cli_print_output() function.
 *
 * @param transport_type selected transport type for CLI
 */
ZB_WEAK_PRE void zb_diagnostic_cli_init_transport(zb_cli_transport_type_t transport_type) ZB_WEAK;

/**
 * @brief Re-initializes current transport.
 * Currently, re-initialization is required in case of MAC-split host configurations.
 */
ZB_WEAK_PRE void zb_diagnostic_cli_reinit_transport(void) ZB_WEAK;

/**
 * @brief Prints a string as a CLI output
 * (by default, CLI uses transport initialized by zb_diagnostic_cli_init_transport()),
 * but this function can we overridden to allow application to print or save output by a custom way
 *
 * @param message output string
 * @param message_length output string length
 */
ZB_WEAK_PRE void zb_diagnostic_cli_print_output(zb_char_t *message, zb_size_t message_length) ZB_WEAK;

/**
 * @brief Handles CLI input string (usually input string is some command)
 *
 * @param input_data input string
 * @param input_len input string length
 */
void zb_diagnostic_cli_handle_input(const zb_char_t *input_data, zb_size_t input_len);

/**
 * @brief Starts CLI and prints welcome string to output
 */
void zb_diagnostic_cli_start(void);

/**
 * @brief Restarts CLI after switching power on and off
 */
void zb_diagnostic_cli_restart_after_power_on(void);

/**
 * @brief Handles stack signal
 *
 * @param param buffer with signal data
 * @param preserve_buffer flag indicating whether the function should preserve or free the buffer
 */
void zb_diagnostic_cli_stack_signal_handler(zb_uint8_t param, zb_bool_t preserve_buffer);

/**
 * @brief Checks whether the CLI received command to start Zigbee stack or not
 *
 * @return zb_bool_t receiving state of command to start Zigbee stack
 */
zb_bool_t zb_diagnostic_cli_is_zigbee_powered_on(void);

/**
 * @brief Returns default transport type for the current platform
 *
 * @return zb_cli_transport_type_t default transport type
 */
zb_cli_transport_type_t zb_diagnostic_cli_get_default_transport_type(void);

void zb_diagnostic_cli_wait_for_power_on(void);

#endif /* ZB_DIAGNOSTIC_CLI_H */
