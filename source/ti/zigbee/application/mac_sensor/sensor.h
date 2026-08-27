/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2026, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/
#ifndef SENSOR_H
#define SENSOR_H 1

/******************************************************************************
 Includes
 *****************************************************************************/

#include "zb_common.h"
#include "smsgs.h"
#include "nvocmp.h"
#include "nvintf.h"

/******************************************************************************
 Constants and definitions
 *****************************************************************************/
/*! Event ID - Start the device in the network */
#define SENSOR_START_EVT 0x0001
/*! Event ID - Reading Timeout Event */
#define SENSOR_READING_TIMEOUT_EVT 0x0002

#ifdef FEATURE_NATIVE_OAD
/*! Event ID - OAD Timeout Event */
#define SENSOR_OAD_TIMEOUT_EVT 0x0004
#ifdef OAD_IMG_A
#define SENSOR_OAD_SEND_RESET_RSP_EVT 0x0008
#endif

#endif /* FEATURE_NATIVE_OAD */

#if (USE_DMM) && !(DMM_CENTRAL)
/*! Event ID - start provisioning Event */
#define SENSOR_PROV_EVT 0x0020
#endif /* USE_DMM && !DMM_CENTRAL */
/*! Event ID - Disassociate Event */
#define SENSOR_DISASSOC_EVT 0x0040

#ifdef DMM_OAD
/*! Event ID - Pause 154 Sensor */
#define SENSOR_PAUSE_EVT 0x0080
/*! Event ID - Resume 154 Sensor */
#define SENSOR_RESUME_EVT 0x0100
#endif /* DMM_OAD */

#ifdef FEATURE_TOAD
/*! Event ID - Turbo OAD Decoding Event*/
#define SENSOR_TOAD_DECODE_EVT 0x0200
#endif

/* Beacon order for non beacon network */
#define NON_BEACON_ORDER      15

/******************************************************************************
 MAC Structs and other defines from the 15.4 stack needed for NV access
 *****************************************************************************/
/* NV Item ID - the device's network information */
#define NV_NETWORK_INFO_ID 0x0001
/* NV Item ID - Config information */
#define NV_CONFIG_INFO_ID  0x0002
/* NV Item ID - this devices frame counter */
#define NV_FRAMECOUNTER_ID 0x0004
/* NV Item ID - Assert reset reason */
#define NV_RESET_REASON_ID 0x0005
/* NV Item ID - Number of resets */
#define NV_RESET_COUNT_ID 0x0006
/* NV Item ID - OAD information */
#define NV_OAD_ID 0x0007
/* NV Item ID - Device Key information */
#define NV_DEVICE_KEY_ID  0x0008

/* Alternate HAL Assert function pointer */
typedef void (*alternateHalAssertFp_t)(void);

/* Range Extender function pointer */
typedef void (*setRangeExtenderFp_t)(uint32_t);
typedef void (*rfSelectFp_t)(uint16_t);

typedef struct
{
    uint32_t               getHwRevision;     /* API to get HW revision */
    uint32_t               *pRfDrvTblPtr;     /* RF Driver API table */
    uint32_t               *pCryptoDrvTblPtr; /* Crypto Driver API table */
    alternateHalAssertFp_t pAssertFP;         /* Assert Function Pointer */
    rfSelectFp_t           pRfSelectFP;       /* RF select Function Pointer */
} macUserCfg_t;

/**
 * @brief       MAC Thread configuration structure used to pass
 *              configuration information to the stack thread.
 */

typedef struct
{
    //! The device's extended address. MACNP only
    zb_uint8_t extendedAddress[8];

    //! Non-volatile function pointers
    NVINTF_nvFuncts_t nvFps;

    //!Initialization failure, non-zero means failure. MACNP only
    zb_uint8_t initFail;

    //! MAC initialization structures
    macUserCfg_t macConfig;
    
} mac_Config_t;

/******************************************************************************
 Global Variables
 *****************************************************************************/

/*! Sensor Task ID */
extern zb_uint8_t Sensor_TaskId;

/*! Sensor events flags */
extern zb_uint16_t Sensor_events;

/*! Sensor statistics */
extern Smsgs_msgStatsField_t Sensor_msgStats;

/******************************************************************************
 Function Prototypes
 *****************************************************************************/

#ifdef USE_DMM
/**
 * @brief Synchronize provisioning attributes with GATT.
 *
 * This function ensures that provisioning-related attributes are updated
 * and synchronized between the Zigbee stack and the GATT layer.
 */
extern void PROVGatt_syncProvisioningAttributes_util(void);

/**
 * @brief Synchronize sensor attributes with GATT.
 *
 * This function ensures that sensor-related attributes are updated
 * and synchronized between the Zigbee stack and the GATT layer.
 */
extern void MACGatt_syncSensorAttributes_util(void);
#endif /* USE_DMM */

/*!
 * @brief Application task processing.
 */
extern void Sensor_process(void);

/*!
 * @brief   Send MAC data request
 *
 * @param   type - message type
 * @param   pDstAddr - destination address
 * @param   rxOnIdle - true if not a sleepy device
 * @param   len - length of payload
 * @param   pData - pointer to the buffer
 *
 * @return  true if sent, false if not
 */
extern zb_bool_t Sensor_sendMsg(Smsgs_cmdIds_t type, zb_addr_u *pDstAddr, zb_bool_t rxOnIdle, zb_uint16_t len, zb_uint8_t *pData);

/*!
 * @brief Send identify LED request to collector
 */
extern void Sensor_sendIdentifyLedRequest(void);

/*!
 * @brief       Initialize the reading clock.
 */
extern void initializeReadingClock(void);

/*!
 * @brief       set the reading clock.
 *
 * @param       readingTime - timer duration to read from sensor (in msec)
 */
extern void setReadingClock(zb_uint32_t readingTime);

/*!
 * @brief       Start the sensor 
 *
 */
extern void triggerSensorStartEvt(void);

/**
 * @brief Get the current reporting interval for the sensor.
 *
 * @return Reporting interval in milliseconds.
 */
uint32_t sensorGetReportingInterval(void);

/**
 * @brief Get the current temperature sensor data.
 *
 * @return Temperature value in Celsius.
 */
uint16_t getTempSensorData(void);

/*!
Build a zb_uint32_t from a zb_uint8_t array

Taken from mac_util.c in 15.4 stack
*/
zb_uint32_t Util_parseUint32(zb_uint8_t *pArray);
 
/*!
Break and buffer a zb_uint16_t value - LSB first

Taken from mac_util.c in 15.4 stack
*/
zb_uint8_t *Util_bufferUint16(zb_uint8_t *pBuf, zb_uint16_t val);
#endif // SENSOR