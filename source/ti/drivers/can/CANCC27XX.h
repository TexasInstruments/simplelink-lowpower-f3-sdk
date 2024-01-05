/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
/*!*****************************************************************************
 *  @file       CANCC27XX.h
 *
 *  @brief      CAN driver implementation for the CC27XX family
 *
 *  # Overview
 *  The CC27XX family of devices has an integrated CAN-FD controller.
 *
 *  ## Message RAM Size
 *  The CC27XX CAN-FD controller has 4KB of message RAM.
 *******************************************************************************
 */
#ifndef ti_drivers_can_cancc27xx__include
#define ti_drivers_can_cancc27xx__include

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** CAN message RAM size */
#define CANCC27XX_MRAM_SIZE 4096U

/** CAN functional clock frequency in MHz */
#define CANCC27XX_CLK_FREQ_MHZ 80U

/*!
 *  @brief      CANCC27XX global configuration
 */
typedef struct
{
    uint32_t clkFreqMHz; /*!< MCAN input clock frequency in MHz */
} CANCC27XX_Config;

/* Externs from ti_drivers_config.c */
extern const CANCC27XX_Config CANCC27XX_config;

/*!
 *  @brief  Reads the CANCC27XX device status flags
 *
 *  @return Device status flags.
 *
 *  @sa     #CANCC27XX_clearStatus
 */
uint32_t CANCC27XX_getStatus(void);

/*!
 *  @brief  Clears all CANCC27XX device status flags
 *
 *  @sa     #CANCC27XX_getStatus
 */
void CANCC27XX_clearStatus(void);

/*!
 *  @brief  Sets the CANCC27XX operational mode
 *
 *  @param  mode  Operational mode to set:
 *                  CANCC27XX_MODE_OPMODE_SLEEP,
 *                  CANCC27XX_MODE_OPMODE_STANDBY,
 *                  CANCC27XX_MODE_OPMODE_NORMAL
 *
 *  @sa     #CANCC27XX_getMode
 */
void CANCC27XX_setMode(uint32_t mode);

/*!
 *  @brief  Reads the CANCC27XX operational mode
 *
 *  @return Operational mode:
 *            CANCC27XX_MODE_OPMODE_SLEEP,
 *            CANCC27XX_MODE_OPMODE_STANDBY,
 *            CANCC27XX_MODE_OPMODE_NORMAL
 *
 *  @sa     #CANCC27XX_setMode
 */
uint32_t CANCC27XX_getMode(void);

/*!
 *  @brief  Disables the sleep wake error timeout
 *
 *  The sleep wake error timeout is enabled by default and powers down the
 *  CANCC27XX device within four minutes after power-on, reset, or coming out of
 *  sleep if the device is not configured by the host. This function can be used
 *  to disable the sleep wake error timeout and prevent the device from powering
 *  down.
 *
 */
void CANCC27XX_disableSleepWakeErrorTimeout(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_can_cancc27xx__include */
