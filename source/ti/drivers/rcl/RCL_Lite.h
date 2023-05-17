/*
 * Copyright (c) 2021-2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ti_drivers_RCL_Lite_h__include
#define ti_drivers_RCL_Lite_h__include

#include <stdint.h>
#include <stddef.h>

#include <ti/drivers/rcl/RCL_Event.h>
#include <ti/drivers/rcl/RCL_Command.h>
#include <ti/drivers/rcl/RCL_Buffer.h>
#include <ti/drivers/rcl/RCL_Scheduler.h>
#include <ti/drivers/rcl/LRF.h>
#include <ti/drivers/rcl/hal/hal.h>

/**
 * @brief Routine to be called in the RFD_IRQ0 interrupt handler
 */
void RCL_Lite_CommandHwi(void);

/**
 * @brief Sets up the radio for configuration with non-default PHY features
 *
 * Sets up the radio with a given configuration and PHY features. Note that the phyFeatures field in the
 * radio commands is ignored when using RCL Lite.
 *
 * @param lrfConfig [in]   - Radio configuration to be used
 * @param phyFeatures [in] - PHY feature selector
 *
 * @return Result of setup
 */
static inline LRF_SetupResult RCL_Lite_configSetPhyFeatures(const LRF_Config *lrfConfig, uint16_t phyFeatures)
{
    /* Temporary solution: Enable all needed clocks here and leave them enabled */
    LRF_rclEnableRadioClocks();
    return LRF_setupRadio(lrfConfig, phyFeatures, RadioState_Down);
}

/**
 * @brief Sets up the radio for configuration with default PHY features
 *
 * Sets up the radio with a given configuration, but with PHY features at default.
 *
 * @param lrfConfig [in] - Radio configuration to be used
 *
 * @return Result of setup
 */
static inline LRF_SetupResult RCL_Lite_config(const LRF_Config *lrfConfig)
{
    return RCL_Lite_configSetPhyFeatures(lrfConfig, LRF_PhyFeatures_Default);
}

/**
 * @brief Change PHY features after radio has been configured
 *
 * Sets up a given set of PHY features. It is required that the radio has
 * already been set up using %RCL_Lite_configSetPhyFeatures or %RCL_Lite_config.
 * Note that the phyFeatures field in the radio commands is ignored when using
 * RCL Lite.
 *
 * @param lrfConfig [in]   - Radio configuration to be used
 * @param phyFeatures [in] - PHY feature selector
 *
 * @return Result of setup
 */
static inline LRF_SetupResult RCL_Lite_setPhyFeatures(const LRF_Config *lrfConfig, uint16_t phyFeatures)
{
    return LRF_setupRadio(lrfConfig, phyFeatures, RadioState_Configured);
}


/**
 * @brief Submit RCL command object to be executed
 *
 * This API returns immediately with either %RCL_CommandStatus_Error or the asynchronous
 * current state of the command.
 *
 * @param c [in] - Command structure
 */
void RCL_Lite_Command_start(RCL_Command_Handle c);

/**
 * @brief Check if a command is finished. The function may be polled to wait for the running
 *        command to end.
 *
 * @return true if the last running command has finished or no command is started,
 *         false if a command is running
 */
bool RCL_Lite_Command_isFinished(void);

/**
 * @brief Stop the running command if any
 *
 * Sends the message to try to stop the running command if any. When the function returns, the
 * command may still be running. Depending on the stop type, the command may stop after some time.
 * %RCL_Lite_Command_isFinished may be used to determine when the command is finished.
 *
 * @param stopType [in] - Stop type; telling which situations the command will stop
 *
 * @return Pointer to the command object that is being stopped or remains running; NULL if no command was running
 */
RCL_Command_Handle *RCL_Lite_Command_stop(RCL_StopType stopType);

/**
 * @brief Abort the running command if any
 *
 * Sends the message to abort the running command if any. When the function returns, the command
 * may still be running, but should be expected to end after a short time.
 * %RCL_Lite_Command_isFinished may be used to determine when the command is finished.
 *
 * @return Pointer to the command object that is being stopped; NULL if no command was running
 */
static inline RCL_Command_Handle *RCL_Lite_Command_abort(void)
{
    return RCL_Lite_Command_stop(RCL_StopType_Hard);
}

/**
 * @brief Get the last valid RSSI value.
 *
 * This API returns the last valid RSSI value or a specific error status if the last obtained RSSI
 * value is no longer valid.
 *
 * @return Returns RSSI value, or LRF_RSSI_INVALID if the last obtained RSSI value is no longer valid
 * or the radio is unavailable (e.g. in the middle of a Tx operation).
 */
static inline int8_t RCL_Lite_readRssi(void)
{
    return LRF_readRssi();
}

#endif
