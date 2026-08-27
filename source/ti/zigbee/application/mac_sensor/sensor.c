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

#include <ti/log/Log.h>

// Do not include zb_common.h in the application file (zb_mac.h if MAC only application, else zboss_api.h)
// If you need something, include it directly (don't solely rely on dependencies to pull them in)
#include "sensor_mac_config_settings.h"
#include "sensor.h"
#include "zb_scheduler.h"
#include "zb_bufpool.h"
#include "jdllc.h"
#include "smsgs.h"
#include "zb_mac.h"
#include "zb_led_button.h"

#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_fcfg.h)
#include <ti/drivers/Temperature.h>

#ifdef USE_DMM
#include <dmm_policy.h>
/* Button Support via BLE */
#undef ZB_USE_BUTTONS
#endif

#ifdef ZB_MAC_SECURITY
#include "mac_internal.h"
#endif /* ZB_MAC_SECURITY */

/******************************************************************************
 Constants and definitions
 *****************************************************************************/
/* Initial timeout value for the reading clock (in ms) */
#define READING_INIT_TIMEOUT_VALUE 100

/* default MSDU Handle rollover */
#define MSDU_HANDLE_MAX 0x1F

/* App marker in MSDU handle */
#define APP_MARKER_MSDU_HANDLE 0x80

/* App Message Tracking Mask */
#define APP_MASK_MSDU_HANDLE 0x60

/* App Sensor Data marker for the MSDU handle */
#define APP_SENSOR_MSDU_HANDLE 0x40

/* App tracking response marker for the MSDU handle */
#define APP_TRACKRSP_MSDU_HANDLE 0x20

/* App config response marker for the MSDU handle */
#define APP_CONFIGRSP_MSDU_HANDLE 0x60

/* Reporting Interval Min and Max (in milliseconds) */
#define MIN_REPORTING_INTERVAL 1000
#define MAX_REPORTING_INTERVAL 360000

/* Polling Interval Min and Max (in milliseconds) */
#define MIN_POLLING_INTERVAL 1000
#define MAX_POLLING_INTERVAL 10000

/* Blink Time for Identify LED Request (in seconds) */
#define IDENTIFY_LED_TIME 1

/* Used for converting milliseconds into ClockP ticks, which is 1 microsecond (1000 ms) */
#define UtilTimer_MS_ADJUSTMENT       1000

/* Calculated from sendSensorMessage in sensor.c in 15.4 satck when the length value gets incremented */
#define MAX_SENSOR_DATA_MESSAGE_LENGTH 117

/* Signifies that there we will be retrieving the temperature value from the temperature sensor */
#define TEMP_SENSOR

/*
 The increment value needed to save a frame counter. Example, setting this
 constant to 100, means that the frame counter will be saved when the new
 frame counter is 100 more than the last saved frame counter.  Also, when
 the get frame counter function reads the value from NV it will add this value
 to the read value.
 */
#define FRAME_COUNTER_SAVE_WINDOW     25


/******************************************************************************
 Local Function Prototypes
 *****************************************************************************/
static void processReadingTimeoutCallback(uintptr_t a0);
static zb_bufid_t allocateOutBuffer();
static void setEvent(zb_uint16_t event);
static void clearEvent(zb_uint16_t event);

static void commStatusIndCb(zb_mlme_comm_status_indication_t *pCommStatusInd);
static void dataCnfCB(zb_mcps_data_confirm_params_t *pDataCnf);
static void dataIndCB(zb_mcps_data_indication_t *pDataInd);
static void resetCnfCb(zb_mlme_reset_confirm_t *pResetCnf);
static zb_uint8_t getMsduHandle(Smsgs_cmdIds_t msgType);

static void processSensorMsgEvt(zb_uint8_t param);
static zb_bool_t sendSensorMessage(zb_addr_u *pDstAddr, Smsgs_sensorMsg_t *pMsg);
static void readSensors();

static void mlme_reset_request(zb_uint8_t param);
static void set_ieee_addr(zb_uint8_t unused);
static void Sensor_finish_init(zb_uint8_t param);

static void processConfigRequest(zb_mcps_data_indication_t *pDataInd);
static zb_bool_t sendConfigRsp(zb_addr_u *pDstAddr, Smsgs_configRspMsg_t *pMsg);
static zb_uint16_t validateFrameControl(zb_uint16_t frameControl);

static void jdllcJoinedCb(zb_device_descriptor_t *pDevInfo,
                          Llc_netInfo_t  *pStartedInfo);
static void jdllcDisassocIndCb(zb_ieee_addr_t *extAddress,
                               zb_disassociate_reason_t reason);
static void jdllcDisassocCnfCb(zb_ieee_addr_t *extAddress,
                               zb_mac_status_t status);
static void jdllcStateChangeCb(Jdllc_states_t state);
void setReadingClock(zb_uint32_t readingTime);

/******************************************************************************
 Application NVRAM callback prototypes
 *****************************************************************************/

/* Structure to store the device information in NV (size = 28 bytes) */
typedef struct
{
  zb_device_descriptor_t device;
  Llc_netInfo_t parent;
  // Size of anything to be written to flash must be divisible by 4
  zb_uint8_t reserved[4 - ((sizeof(zb_device_descriptor_t) + sizeof(Llc_netInfo_t)) % 4)];
} nvDeviceInfo_t;

// Device information to be stored in NV read/write
zb_device_descriptor_t joiningSensorDevice = {0};

static NVINTF_nvFuncts_t* pNV = NULL;
// Will try to define all the NV structs here and not in a separate ssf file in order to reduce dependencies.
#ifdef NV_RESTORE
mac_Config_t Main_user1Cfg;
#endif

/* Structure of validated config information */
typedef struct
{
  /* Frame Control field of the Config Response message */
  zb_uint16_t frameControl;
  /* Reporting Interval field of the Config Response message */
  zb_uint32_t reportingInterval;
  /* Polling Interval field of the Config Response message */
  zb_uint32_t pollingInterval;
} zb_config_settings_t;

zb_bool_t getNetworkInfo(zb_device_descriptor_t *pDevInfo, Llc_netInfo_t *pParentNetInfo);
void networkUpdate(zb_bool_t rejoined, zb_device_descriptor_t *pDevInfo, Llc_netInfo_t *pParentInfo);
zb_bool_t getConfigInfo(zb_config_settings_t *pInfo);
void configurationUpdate(Smsgs_configRspMsg_t *pRsp);
void clearNetworkInfo();

void updateFrameCounter(zb_uint32_t frameCounter);
zb_bool_t getFrameCounter(zb_uint32_t *pFrameCounter);

/******************************************************************************
 Button handler callback prototypes
 *****************************************************************************/
void button1_handler(zb_uint8_t param);
void button2_handler(zb_uint8_t param);

/******************************************************************************
 Global variables
 *****************************************************************************/
/* Task pending events */
zb_uint16_t Sensor_events = 0;

zb_bool_t started = ZB_FALSE;

/* accumulated total E2E delay */
zb_uint32_t totalE2EDelaySum = 0;

/* saved end to end delay */
zb_uint32_t endToEndDelay = 0;

/*! Sensor statistics */
Smsgs_msgStatsField_t Sensor_msgStats = { 0 };
extern bool parentFound;

extern SemaphoreP_Handle wakeSem;

/*!
 Assert reason for the last reset -  0 - no reason, 2 - HAL/ICALL,
 3 - MAC, 4 - TIRTOS (originally from SSF layer but trying to move up to here)
 */
zb_uint8_t resetReseason = 0;

/*! Number of times the device has reset (originally from SSF layer but trying to move up to here) */
zb_uint16_t resetCount = 0;

// Is the red LED on or off?
zb_bool_t redLEDState = ZB_FALSE;

/******************************************************************************
 Local variables
 *****************************************************************************/

// MAC security key (configured to default key value from 15.4 collector and sensor syscfg settings)
static zb_uint8_t default_key[16] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static zb_uint8_t default_key_source[8] __attribute__((unused)) = {0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33};

/* Clock/timer resources */
ClockP_Struct readingClkStruct;
ClockP_Handle readingClkHandle;

// Rejoined flag
static zb_bool_t rejoining = ZB_FALSE;

// Collector's address and address mode
static zb_addr_u collectorAddr = {0};
static zb_uint8_t collectorAddrMode = 0;

// Join time ticks (used for average join time calculations)
static zb_uint32_t joinTimeTicks = 0;

/* End to end delay statistics timestamp */
static zb_uint32_t startSensorMsgTimeStamp = 0;

/*! Device's Outgoing MSDU Handle values */
static zb_uint8_t deviceTxMsduHandle = 0;

static Smsgs_configReqMsg_t configSettings;

/*!
 Temp Sensor field - valid only if Smsgs_dataFields_tempSensor
 is set in frameControl.
 */
static Smsgs_tempSensorField_t tempSensor = { 0 };

/*!
 Light Sensor field - valid only if Smsgs_dataFields_lightSensor
 is set in frameControl.
 */
static Smsgs_lightSensorField_t lightSensor = { 0 };

/*!
 Humidity Sensor field - valid only if Smsgs_dataFields_humiditySensor
 is set in frameControl.
 */
static Smsgs_humiditySensorField_t humiditySensor = { 0 };
static Llc_netInfo_t parentInfo = {0};

/* The last saved frame counter (the last frame counter value that got written to flash) */
static zb_uint32_t lastSavedFrameCounter = 0;

/******************************************************************************
 Callback tables
 *****************************************************************************/
/*! API MAC Callback table */
static zb_mac_callbacks_t Sensor_macCallbacks =
    {
      /*! Associate Indication callback */
      NULL,
      /*! Associate Confirmation callback */
      NULL,
      /*! Disassociate Indication callback */
      NULL,
      /*! Disassociate Confirmation callback */
      NULL,
      /*! Beacon Notify Indication callback */
      NULL,
      /*! Orphan Indication callback */
      NULL,
      /*! Scan Confirmation callback */
      NULL,
      /*! Start Confirmation callback */
      NULL,
      /*! Poll Confirm callback */
      NULL,
      /*! Comm Status Indication callback */
      commStatusIndCb,
      /*! Data Confirmation callback */
      dataCnfCB,
      /*! Data Indication callback */
      dataIndCB,
      /*! Purge Confirm callback */
      NULL,
      /*! Reset Confirm callback */
      resetCnfCb
    };

static Jdllc_callbacks_t jdllcCallbacks =
    {
      /*! Network Joined Indication callback */
      jdllcJoinedCb,
      /* Disassociation Indication callback */
      jdllcDisassocIndCb,
      /* Disassociation Confirm callback */
      jdllcDisassocCnfCb,
      /*! State Changed indication callback */
      jdllcStateChangeCb
    };

/******************************************************************************
 Helper functions for zb_uint16_t and zb_uint32_t manipulation
 *****************************************************************************/
/*!
 Get the high byte of a zb_uint16_t variable

 Taken from mac_util.c in 15.4 stack
 */
zb_uint8_t Util_hiUint16(zb_uint16_t a)
{
  return((a >> 8) & 0xFF);
}

/*!
 Get the low byte of a zb_uint16_t variable

 Taken from mac_util.c in 15.4 stack
 */
zb_uint8_t Util_loUint16(zb_uint16_t a)
{
  return((a) & 0xFF);
}

/*!
 Build a zb_uint16_t out of 2 zb_uint8_t variables

 Taken from mac_util.c in 15.4 stack
 */
zb_uint16_t Util_buildUint16(zb_uint8_t loByte, zb_uint8_t hiByte)
{
  return((zb_uint16_t)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)));
}

/*!
 Build a zb_uint32_t out of 4 zb_uint8_t variables

 Taken from mac_util.c in 15.4 stack
 */
zb_uint32_t Util_buildUint32(zb_uint8_t byte0, zb_uint8_t byte1, zb_uint8_t byte2,
                            zb_uint8_t byte3)
{
  return((zb_uint32_t)((zb_uint32_t)((byte0) & 0x00FF) +
                    ((zb_uint32_t)((byte1) & 0x00FF) << 8) +
                    ((zb_uint32_t)((byte2) & 0x00FF) << 16) +
                    ((zb_uint32_t)((byte3) & 0x00FF) << 24)));
}

/*!
 Pull 1 zb_uint8_t out of a zb_uint32_t

 Taken from mac_util.c in 15.4 stack
 */
zb_uint8_t Util_breakUint32(zb_uint32_t var, int byteNum)
{
  return(zb_uint8_t)((zb_uint32_t)(((var) >> ((byteNum) * 8)) & 0x00FF));
}

/*!
 Build a zb_uint16_t from a zb_uint8_t array

 Taken from mac_util.c in 15.4 stack
 */
zb_uint16_t Util_parseUint16(zb_uint8_t *pArray)
{
  return(Util_buildUint16(pArray[0], pArray[1]));
}

/*!
 Build a zb_uint32_t from a zb_uint8_t array

 Taken from mac_util.c in 15.4 stack
 */
zb_uint32_t Util_parseUint32(zb_uint8_t *pArray)
{
  return(Util_buildUint32(pArray[0], pArray[1], pArray[2], pArray[3]));
}

/*!
 Break and buffer a zb_uint16_t value - LSB first

 Taken from mac_util.c in 15.4 stack
 */
zb_uint8_t *Util_bufferUint16(zb_uint8_t *pBuf, zb_uint16_t val)
{
  *pBuf++ = Util_loUint16(val);
  *pBuf++ = Util_hiUint16(val);

  return(pBuf);
}

/*!
 Break and buffer a zb_uint32_t value - LSB first

 Taken from mac_util.c in 15.4 stack
 */
zb_uint8_t *Util_bufferUint32(zb_uint8_t *pBuf, zb_uint32_t val)
{
  *pBuf++ = Util_breakUint32(val, 0);
  *pBuf++ = Util_breakUint32(val, 1);
  *pBuf++ = Util_breakUint32(val, 2);
  *pBuf++ = Util_breakUint32(val, 3);

  return(pBuf);
}

MAIN()
{
  ARGV_UNUSED;

#ifdef NV_RESTORE
  Log_printf(LogModule_Zigbee_App, Log_INFO, "Initializing NV");
  // From nvocmp.c, which is part of the 15.4 nv directory that we've included in this build. Need to explicitly add nvocmp.c
  // to lists of files for this project.
  NVOCMP_loadApiPtrs(&Main_user1Cfg.nvFps);
  if (Main_user1Cfg.nvFps.initNV)
  {
    zb_uint8_t init_nv_status = Main_user1Cfg.nvFps.initNV(NULL); // Has been returning NVINTF_SUCCESS
    Log_printf(LogModule_Zigbee_App, Log_INFO, "NV initialization status: %d", init_nv_status);
    ZVUNUSED(init_nv_status);
  }
#endif // NV_RESTORE

  ZB_INIT("sensor");
  Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor initialization");

  // Register the button press handlers.
#ifdef ZB_USE_BUTTONS
  zb_button_register_handler(0, 0, button1_handler);
  zb_button_register_handler(1, 0, button2_handler);
#endif /* ZB_USE_BUTTONS */

#ifdef ZB_MAC_SECURITY
  // Set up the security PIB attributes for the MAC key, frame counter, and number of MAC device table entries.
  // You can't use a set request for any of the security PIB attributes because there is no corresponding PIB ID for any of the attributes,
  // so we will have to set the security attributes directly.
  ZB_MEMCPY(MAC_PIB().mac_key, default_key, 16);
  MAC_PIB().mac_frame_counter = 1;
  MAC_PIB().mac_device_table_entries = 1; // there should only be one entry to hold the collector's device information
#endif /* ZB_MAC_SECURITY */

  // Registering the callbacks here in order to get the translation layer to call the appropriate application layer callback,
  // specifically for when the reset confirmation callback is called in order to synchronously reset the ZBOSS MAC.
  register_callbacks(&Sensor_macCallbacks);

  // 15.4 stack calls ApiMac_init(), which calls an API to reset the MAC and then calls an API to set the device IEEE address.
  // Do the same here. The rest of the sensor's initializations only happen after the MAC is reset and the IEEE address is set.
  zb_ret_t delayedBufferStatus = zb_buf_get_out_delayed(mlme_reset_request);
  if (delayedBufferStatus != RET_OK)
  {
    Log_printf(LogModule_Zigbee_App, Log_ERROR, "Sensor initialization failed: Could not get an output buffer or delayed buffer for MLME reset request");
    zb_reset(ZB_UNUSED_PARAM);
  }

  while(1)
  {
    zb_sched_loop_iteration();

    // Kick off application, also in a forever loop. But only after the sensor's initialization process is complete.
    if (started)
    {
      Sensor_process();
    }
  }

  MAIN_RETURN(0);
}

/******************************************************************************
 Public Functions
 *****************************************************************************/

/*!
 Application task processing. Should be called somewhere in an infinite loop.

 Public function defined in sensor.h
 */
void Sensor_process(void)
{
  // Start the sensor device in the network
  if (Sensor_events & SENSOR_START_EVT)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_process: Processing start event");

    zb_device_descriptor_t devInfo;
    Llc_netInfo_t parentDevInfo;

    // Sensor is rejoining (this is being called twice, once during initialization and once here. here, it's needed to update collector info
    // and then call Jdllc_rejoin with the necessarily information. maybe try to only make it so there's just one pass?)
    if (getNetworkInfo(&devInfo, &parentDevInfo) == ZB_TRUE)
    {
      Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_process: sensor is rejoining");
      rejoining = true;

      zb_config_settings_t configInfo;

      // Do we have config settings?
      if (getConfigInfo(&configInfo) == ZB_TRUE)
      {
        /* Save the config information */
        configSettings.frameControl = configInfo.frameControl;
        configSettings.reportingInterval = configInfo.reportingInterval;
        configSettings.pollingInterval = configInfo.pollingInterval;

        /* Update the polling interval in the LLC */
        Jdllc_setPollRate(configSettings.pollingInterval);
      }

      collectorAddrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;
      collectorAddr.addr_short = parentDevInfo.devInfo.shortAddress;

      joiningSensorDevice.shortAddress = devInfo.shortAddress;
      joiningSensorDevice.panID = devInfo.panID;

      // If MAC security is enabled, then put the parent in the MAC device table
#ifdef ZB_MAC_SECURITY
      Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_process: Adding parent's device information to MAC device table.");
      Jdllc_addSecDevice(parentDevInfo.devInfo.panID, parentDevInfo.devInfo.shortAddress, &parentDevInfo.devInfo.extAddress, 0);
#endif /* ZB_MAC_SECURITY */

      Jdllc_rejoin(&devInfo, &parentDevInfo);
    }
    else
    {
      Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_process: sensor is initially joining");
      rejoining = ZB_FALSE;
      joinTimeTicks = ClockP_getSystemTicks();
      Jdllc_join();
    }

    // Clear the event
    clearEvent(SENSOR_START_EVT);
  }

  // Is it time to send the next sensor data message?
  if (Sensor_events & SENSOR_READING_TIMEOUT_EVT)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_process: Processing reading timeout event");

    // If CERTIFICATION_TEST_MODE is not defined, then set the reading clock to configSettings.reportingInterval (should be provided by collector)
    // Just going to assume that it will never be defined for this context
    setReadingClock(configSettings.reportingInterval);
  
#ifdef FEATURE_SECURE_COMMISSIONING
    /* if secure Commissioning feature is enabled, read
      * sensor data and send it only after the secure
      * commissioning process is done successfully.
      * else, do not read and send sensor data.
      */
    if (SM_Current_State != SM_CM_InProgress)
    {
#endif /* FEATURE_SECURE_COMMISSIONING */

#if SENSOR_TEST_RAMP_DATA_SIZE && (CERTIFICATION_TEST_MODE || defined(POWER_MEAS))
      processSensorRampMsgEvt();
#endif /* SENSOR_TEST_RAMP_DATA_SIZE */

      // Read sensors
      readSensors();

      // Originally, a call to processSensorMsgEvt() was here, but the logic requires that you get
      // the extended address PIB attribute, which dictates what the function does. Because we need to schedule the get request,
      // let's form the get request here, and then use processSensorMsgEvt as the callback.
      {
        zb_bufid_t buf = allocateOutBuffer();
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_process: getting PIB attribute for extended address with buffer ID %d", buf);
        zb_mlme_get_request_t *req;

        req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_get_request_t));
        req->pib_attr   = ZB_PIB_ATTRIBUTE_EXTEND_ADDRESS;
        req->iface_id  = ZB_PIBCACHE_PRIMARY_IFACE();
        // Process sensor reading message event
        req->confirm_cb_u.cb = processSensorMsgEvt;

        zb_mlme_get_request(buf);
      }

#ifdef FEATURE_SECURE_COMMISSIONING
    }
#endif /* FEATURE_SECURE_COMMISSIONING */

    // Clear the event
    clearEvent(SENSOR_READING_TIMEOUT_EVT);
  }

  // Is it disassociate event?
  if (Sensor_events & SENSOR_DISASSOC_EVT)
  {
    // TODO
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_process: Processing disassociation event");

    // Clear the event
    clearEvent(SENSOR_DISASSOC_EVT);
  }

  // Process the LLC events
  Jdllc_process();
}


/*!
 Send LED Identify Request to collector

 Public function defined in sensor.h
 */
void Sensor_sendIdentifyLedRequest(void)
{
  zb_uint8_t cmdBytes[SMSGS_INDENTIFY_LED_REQUEST_MSG_LEN];

  /* send the response message directly */
  cmdBytes[0] = (zb_uint8_t) Smsgs_cmdIds_IdentifyLedReq;
  cmdBytes[1] = (zb_uint8_t) IDENTIFY_LED_TIME; // Have the collector's green LED toggle on and off for 1 second so it can identify itself to the user
  Sensor_sendMsg(Smsgs_cmdIds_IdentifyLedReq, &collectorAddr, ZB_TRUE, SMSGS_INDENTIFY_LED_REQUEST_MSG_LEN, cmdBytes);
}

void button1_handler(zb_uint8_t param)
{
  ZVUNUSED(param);

#ifdef ZB_USE_BUTTONS
  // If the device has joined to a collector's network, then send an LED toggle request to identify which collector this sensor
  // is connected to. You should see the green light toggle on the collector. Note that if the collector disconnects from the network
  // and leaves the sensor orphaned, the sensor will end up sending a bunch of unacknowledged data packets, just like in the 15.4 stack.
  if (started == ZB_TRUE)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "button1_handler: Sending identify LED request to collector");
    Sensor_sendIdentifyLedRequest(); 
  }
  else
  {
    // Tell the sensor to start (if CONFIG_AUTO_START is disabled, then pressing the left button is how the sensor is going to start)
    started = ZB_TRUE;
    setEvent(SENSOR_START_EVT);
  }
#endif
}

void button2_handler(zb_uint8_t param)
{
  ZVUNUSED(param);

#ifdef ZB_USE_BUTTONS
  // If and only if the device has started and joined a network, then pressing the right button will send a disassociation request from the sensor to the collector.
  // This will be done by calling Jdllc_sendDisassociationRequest, which will handle forming the payload to create the disassociation request.
  if (started == ZB_TRUE)
  {
    zb_uint8_t currentJdllcState = Jdllc_getProvState();
    if (currentJdllcState == Jdllc_states_joined || currentJdllcState == Jdllc_states_rejoined)
    {
      Log_printf(LogModule_Zigbee_App, Log_INFO, "button2_handler: Sending disassociation request to collector");
      Jdllc_sendDisassociationRequest();
    }
  }
#endif
}

/*!
 * @brief       The application calls this function to indicate that it has
 *              started or restored the device in a network.
 *
 *              The information will be saved and used to determine if a
 *              network was already started and should be restored instead
 *              of started. Taken from ssf.c in the 15.4 stack.
 *
 * @param       rejoined - true if rejoined to network, false if joined
 * @param       pDevInfo - This device's information
 * @param       pParentInfo - This is the parent's information
 */
void networkUpdate(zb_bool_t rejoined, zb_device_descriptor_t *pDevInfo, Llc_netInfo_t *pParentInfo)
{
  /* check for valid structure pointers, ignore if not */
  if((pDevInfo != NULL) && (pParentInfo != NULL))
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "networkUpdate: pDevInfo and pParentInfo are not null. Rejoined: %d", rejoined);
    if((pNV != NULL) && (pNV->writeItem != NULL))
    {
      Log_printf(LogModule_Zigbee_App, Log_INFO, "networkUpdate: Writing device and parent information to NV. Rejoined: %d", rejoined);
      Log_printf(LogModule_Zigbee_App, Log_INFO, "networkUpdate: pDevInfo has PAN ID %x and short address %x", pDevInfo->panID, pDevInfo->shortAddress);
      Log_printf(LogModule_Zigbee_App, Log_INFO, "networkUpdate: pParentInfo has PAN ID %x and short address %x on channel %d", pParentInfo->devInfo.panID, pParentInfo->devInfo.shortAddress, pParentInfo->channel);
      NVINTF_itemID_t id;
      nvDeviceInfo_t nvItem;

      /* Setup NV ID */
      id.systemID = NVINTF_SYSID_APP;
      id.itemID = NV_NETWORK_INFO_ID;
      id.subID = 0;

      ZB_MEMCPY(&nvItem.device, pDevInfo, sizeof(zb_device_descriptor_t));
      ZB_MEMCPY(&nvItem.parent, pParentInfo, sizeof(Llc_netInfo_t));

      /* Write the NV item */
      pNV->writeItem(id, sizeof(nvDeviceInfo_t), &nvItem);
    }

    started = ZB_TRUE;
#ifndef USE_DMM
    // Turn on the red LED to signify that the sensor has connected to the collector
    zb_osif_led_on(1);
#endif
    redLEDState = ZB_TRUE;
  }
}

/*!
 * @brief       The application calls this function to get the device
 *              information in a network. Taken from ssf.h in 15.4 stack.
 *
 * @param       pDevInfo - This device's information
 * @param       pParentInfo - This is the parent's information
 *
 * @return      true if info found, false if not
 */
zb_bool_t getNetworkInfo(zb_device_descriptor_t *pDevInfo, Llc_netInfo_t  *pParentInfo)
{
  // Check that the NV pointer exists as well as its read item callback function. Also make
  // sure that the device and parent info pointers are not null.
  if ((pNV != NULL) && (pNV->readItem != NULL) && (pDevInfo != NULL) && (pParentInfo != NULL))
  {
    NVINTF_itemID_t id;
    nvDeviceInfo_t nvItem;

    /* Setup NV ID */
    id.systemID = NVINTF_SYSID_APP;
    id.itemID = NV_NETWORK_INFO_ID;
    id.subID = 0;

    /* Read Network Information from NV */
    if(pNV->readItem(id, 0, sizeof(nvDeviceInfo_t), &nvItem) == NVINTF_SUCCESS)
    {
      Log_printf(LogModule_Zigbee_App, Log_INFO, "getNetworkInfo: Read network information from NV");
      ZB_MEMCPY(pDevInfo, &nvItem.device, sizeof(zb_device_descriptor_t));
      ZB_MEMCPY(pParentInfo, &nvItem.parent, sizeof(Llc_netInfo_t));

      Log_printf(LogModule_Zigbee_App, Log_INFO, "getNetworkInfo: pDevInfo has PAN ID %x and short address %x", pDevInfo->panID, pDevInfo->shortAddress);
      Log_printf(LogModule_Zigbee_App, Log_INFO, "getNetworkInfo: pParentInfo has PAN ID %x and short address %x on channel %d", pParentInfo->devInfo.panID, pParentInfo->devInfo.shortAddress, pParentInfo->channel);

      return ZB_TRUE;
    }
  }
  return ZB_FALSE;
}

/*!
 The application calls this function to get the saved device configuration.

 Taken from ssf.c in the 15.4 stack.
 */
zb_bool_t getConfigInfo(zb_config_settings_t *pInfo)
{
  // Check that the NV pointer exists as well as its read item callback function. Also make
  // sure that the configuration settings pointer is not null.
  if ((pNV != NULL) && (pNV->readItem != NULL) && (pInfo != NULL))
  {
      NVINTF_itemID_t id;

      /* Setup NV ID */
      id.systemID = NVINTF_SYSID_APP;
      id.itemID = NV_CONFIG_INFO_ID;
      id.subID = 0;

      /* Read Network Information from NV */
      if(pNV->readItem(id, 0, sizeof(zb_config_settings_t), pInfo) == NVINTF_SUCCESS)
      {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "getConfigInfo: Read config information from NV");
        Log_printf(LogModule_Zigbee_App, Log_INFO, "getConfigInfo: pInfo has frame control of %x, reporting interval of %d, and polling interval of %d",
                   pInfo->frameControl, pInfo->reportingInterval, pInfo->pollingInterval);
        return (ZB_TRUE);
      }
  }
  return (ZB_FALSE);
}

/*!
 The application calls this function to store the configuration response data (that it also sends out to the collector) to flash.

 Taken from ssf.c in the 15.4 stack.
 */
void configurationUpdate(Smsgs_configRspMsg_t *pRsp)
{
  if((pNV != NULL) && (pNV->writeItem != NULL) && (pRsp != NULL))
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "configurationUpdate: Writing configuration settings (also sent to collector) to NV");
    Log_printf(LogModule_Zigbee_App, Log_INFO, "configurationUpdate: pRsp has frame control of %x, reporting interval of %d, and polling interval of %d",
               pRsp->frameControl, pRsp->reportingInterval, pRsp->pollingInterval);
    NVINTF_itemID_t id;
    zb_config_settings_t configInfo;

    /* Setup NV ID */
    id.systemID = NVINTF_SYSID_APP;
    id.itemID = NV_CONFIG_INFO_ID;
    id.subID = 0;

    configInfo.frameControl = pRsp->frameControl;
    configInfo.reportingInterval = pRsp->reportingInterval;
    configInfo.pollingInterval = pRsp->pollingInterval;

    /* Write the NV item */
    pNV->writeItem(id, sizeof(zb_config_settings_t), &configInfo);
  }
}

/*!

 Clear network information in NV

 Taken from ssf.c in the 15.4 stack.
*/
void clearNetworkInfo()
{
  if ((pNV != NULL) && (pNV->deleteItem != NULL))
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "clearNetworkInfo: Clearing network information in NV");
    NVINTF_itemID_t id;

    /* Setup NV ID */
    id.systemID = NVINTF_SYSID_APP;
    id.itemID = NV_NETWORK_INFO_ID;
    id.subID = 0;
    pNV->deleteItem(id);

    /* sensor ready to associate again */
    started = ZB_FALSE;
  }
}

/*!

 Update the frame counter in NV

 Taken from ssf.c in the 15.4 stack, but with the assumption that only the sensor device will be using the frame counter.
*/
void updateFrameCounter(zb_uint32_t frameCounter)
{
  // If the value of frameCounter is greater than or equal to the value of the last saved frame counter plus the save window, then write the frame counter to flash.
  // This is being done so that we don't write to flash every single time there is a data confirmation (which is every time there is a call to send a data packet).
  if ((pNV != NULL) && (pNV->writeItem != NULL) && (frameCounter >= (lastSavedFrameCounter + FRAME_COUNTER_SAVE_WINDOW)))
  {
    NVINTF_itemID_t id;

    /* Setup NV ID */
    id.systemID = NVINTF_SYSID_APP;
    id.itemID = NV_FRAMECOUNTER_ID;
    id.subID = 0;

    /* Write the NV item */
    if (pNV->writeItem(id, sizeof(uint32_t), &frameCounter) == NVINTF_SUCCESS)
    {
      Log_printf(LogModule_Zigbee_App, Log_INFO, "updateFrameCounter: wrote frame counter value of %d to flash", frameCounter);
      lastSavedFrameCounter = frameCounter;
    }
  }
}

/*!
 Get the Frame Counter

 Taken from ssf.c in the 15.4 stack, but with the assumption that only the sensor device will be using the frame counter.
 */
zb_bool_t getFrameCounter(zb_uint32_t *pFrameCounter)
{
  /* Check for valid pointer */
  if (pFrameCounter != NULL)
  {
    if ((pNV != NULL) && (pNV->readItem != NULL))
    {
        NVINTF_itemID_t id;

        /* Setup NV ID */
        id.systemID = NVINTF_SYSID_APP;
        id.itemID = NV_FRAMECOUNTER_ID;
        id.subID = 0;

        /* Read Network Information from NV. If the read succeeds, then this is a rejoin. */
        if (pNV->readItem(id, 0, sizeof(zb_uint32_t), pFrameCounter) == NVINTF_SUCCESS)
        {
          Log_printf(LogModule_Zigbee_App, Log_INFO, "getFrameCounter: retrieved value of %d from flash. Setting value to next window.", *pFrameCounter);
          /* Set to the next window so that the first data confirmation received after the rejoin so that there aren't any packets that get dropped by the collector
             due to having a previously seen frame counter value.
            
            Example: If there was a reset that occurred when *pFrameCounter - lastSavedFrameCounter = FRAME_COUNTER_SAVE_WINDOW - 1, then advancing frame counter
            to the next window would prevent us from having FRAME_COUNTER_SAVE_WINDOW - 1 packets dropped by the collector. All future packets will be accepted. */
          *pFrameCounter += FRAME_COUNTER_SAVE_WINDOW;
          return ZB_TRUE;
        }
        /* If the read doesn't succeed, then this is an initial join. */
        else
        {
          Log_printf(LogModule_Zigbee_App, Log_INFO, "getFrameCounter: writing a frame counter of 0 to flash because this is an initial join.");
          /*
            Wasn't found, so write 0, so the next time it will be
            greater than 0
          */
          zb_uint32_t fc = 0;

          /* Setup NV ID */
          id.systemID = NVINTF_SYSID_APP;
          id.itemID = NV_FRAMECOUNTER_ID;
          id.subID = 0;

          /* Write the NV item */
          pNV->writeItem(id, sizeof(zb_uint32_t), &fc);
        }
    }

    *pFrameCounter = 0;
  }
  return ZB_FALSE;
}

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
zb_bool_t Sensor_sendMsg(Smsgs_cmdIds_t type, zb_addr_u *pDstAddr, zb_bool_t rxOnIdle, zb_uint16_t len, zb_uint8_t *pData)
{
  zb_bool_t ret = ZB_FALSE;

  // If the payload is larger than the size of a ZBOSS buffer (minus the size of a data request payload), then we cannot send the message.
  if (len > ZB_IO_BUF_SIZE - sizeof(zb_mcps_data_req_params_t))
  {
    Log_printf(LogModule_Zigbee_App, Log_ERROR, "Sensor_sendMsg: Cannot send message of type %d with payload length %d. Payload length maximum possible size. Other data request failures: %d", type, pDstAddr->addr_short, len, Sensor_msgStats.otherDataRequestFailures++);
    Sensor_msgStats.otherDataRequestFailures++;
    return ret;
  }

  zb_bufid_t buf = allocateOutBuffer();
  // Assuming that we're always using the short address of the destination device
  Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_sendMsg: Sending message of type %d to destination address %x with buffer ID %d", type, pDstAddr->addr_short, buf);
  // Information about the network
  zb_mcps_data_req_params_t *dataReq = ZB_BUF_GET_PARAM(buf, zb_mcps_data_req_params_t);

  // Record the current timestamp as part of computing end-to-end delay
  startSensorMsgTimeStamp = ClockP_getSystemTicks();

  // Construct the data request field
  ZB_BZERO(dataReq, sizeof(zb_mcps_data_req_params_t));

  // Set the correct address mode. The 15.4 stack passes in a struct that uses ApiMac_sAddr_t, which has an address mode field.
  // ZBOSS does not have such a struct. I will be getting the address mode by using collectorAddrMode, which should
  // be set in jdllcJoinedCb, which gets called after a successful join/rejoin.
  if (collectorAddrMode == ZB_ADDR_64BIT_DEV)
  {
    dataReq->src_addr_mode = ZB_ADDR_64BIT_DEV;
    ZB_IEEE_ADDR_COPY(&dataReq->dst_addr.addr_long, &pDstAddr->addr_long);
  }
  else
  {
    dataReq->src_addr_mode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;
    dataReq->dst_addr.aligned.addr_short = pDstAddr->addr_short;
  }
  // There aren't any fields for the destination address mode or the source address mode in the 15.4 stack's data request structure.
  // Unconditionally set the destination address mode to short addressing mode and the source address to the device's short
  // address since that's what happens in the 15.4 examples.
  dataReq->dst_addr_mode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;
  dataReq->src_addr.addr_short = joiningSensorDevice.shortAddress;

  // If the device is rejoining, then get the new PAN ID from the MAC via the translation layer.
  if (rejoining == ZB_TRUE)
  {
    parentInfo.devInfo.panID = getPibPanIdSynchronous();
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_sendMsg: device rejoined to PAN ID %x", parentInfo.devInfo.panID);
  }

  // Set the destination's PAN ID to the PAN ID from the parentInfo struct.
  dataReq->dst_pan_id = parentInfo.devInfo.panID;

  // Get the next MSDU handle based on the type of message.
  dataReq->msdu_handle = getMsduHandle(type);

  // Set the ACK bit in the TX options to true because we expect an ACK (but if CERTIFICATION_TEST_MODE is enabled, then we don't expect an ACK).
  dataReq->tx_options |= MAC_TX_OPTION_ACKNOWLEDGED_BIT;

  // If the device is a sleepy device, then this is an indrect transmission (MAC will queue the data and wait for the destination device to poll for it)
  if (rxOnIdle == ZB_FALSE)
  {
    dataReq->tx_options |= MAC_TX_OPTION_INDIRECT_TRANSMISSION_BIT;
  }

  // 15.4 stack fills in the MSDU length and pointer here, but ZBOSS doesn't have fields for that in the data request. It only has the MSDU handle.
  // Potentially do what's done in the warm start test cases as well as mac_ack_frame_delivery_01_dut_ffd0.c where the data request is built in the
  // tail of the buffer but the payload is built in the head of the buffer. This way, we can use the same buffer for both the data request and the payload.

  // Allocate from the head of the buffer we've already gotten from zb_buf_get_out() for the data request
  zb_uint8_t *msdu = zb_buf_initial_alloc(buf, len);
  ZB_MEMCPY(msdu, pData, len);

#ifdef ZB_MAC_SECURITY
  Jdllc_securityFill(&dataReq->security_level, &dataReq->key_id_mode, &dataReq->key_source[0], &dataReq->key_index);
#endif /* ZB_MAC_SECURITY */
  for (zb_uint16_t i = 0; i < len; i++)
  {
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "Sensor_sendMsg: msdu[%d] = %x. Expected %x", i, msdu[i], pData[i]);
  }

  // Increment the right message statistic based on the type of message being sent (sensor data? tracking response? configuration response?)
  if (type == Smsgs_cmdIds_sensorData || type == Smsgs_cmdIds_rampdata)
  {
    Sensor_msgStats.msgsAttempted++;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_sendMsg: sending either sensor data or ramp data. Messages attempted: %d", Sensor_msgStats.msgsAttempted);
  }
  else if (type == Smsgs_cmdIds_trackingRsp)
  {
    Sensor_msgStats.trackingResponseAttempts++;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_sendMsg: sending tracking response. Tracking response attempts: %d", Sensor_msgStats.trackingResponseAttempts);
  }
  else if (type == Smsgs_cmdIds_configRsp)
  {
    Sensor_msgStats.configResponseAttempts++;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_sendMsg: sending config response. Config response attempts: %d", Sensor_msgStats.configResponseAttempts);
  }

  // Send the message by scheduling a data request. Issue: the macro casts the return type to void, but we want to see if the message was sent successfully.
  ZB_SCHEDULE_CALLBACK(zb_mcps_data_request, buf);
  ret = ZB_TRUE;
  
  return (ret);
}

/***************** Local functions *****************/
static void mlme_reset_request(zb_uint8_t param)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO, "Sending reset request");
  // Need to call ZB_BUF_GET_PARAM since that gets the tail of the buffer, which is what zb_mlme_reset_request_sync uses
  // in order to read the buffer.
  zb_mlme_reset_request_t *reset_req = ZB_BUF_GET_PARAM(param, zb_mlme_reset_request_t);
  ZB_BZERO(reset_req, sizeof(zb_mlme_reset_request_t));
  reset_req->set_default_pib = 1;
  reset_req->cert_hacks.allow_empty_beacon_payload = 1;

  ZB_SCHEDULE_CALLBACK(zb_mlme_reset_request, param);
}

static void set_ieee_addr(zb_uint8_t unused)
{
  zb_bufid_t buf = allocateOutBuffer();
  Log_printf(LogModule_Zigbee_App, Log_INFO, "Setting IEEE address with buffer ID %d.", buf);
  zb_mlme_set_request_t *req;
  req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_set_request_t) + sizeof(zb_ieee_addr_t));
  ZB_BZERO(req, sizeof(zb_mlme_set_request_t) + sizeof(zb_ieee_addr_t));

  req->pib_attr = ZB_PIB_ATTRIBUTE_EXTEND_ADDRESS;
  req->pib_length = sizeof(zb_ieee_addr_t);
  // After setting the IEEE address of the device in the PIB attributes, perform the rest of the sensor's initialization process.
  req->confirm_cb_u.cb = Sensor_finish_init;
  ZB_MEMCPY((zb_uint8_t *)(req+1), fcfg->deviceInfo.macAddr, sizeof(zb_ieee_addr_t));

  ZB_SCHEDULE_CALLBACK(zb_mlme_set_request, buf);
}

static void Sensor_finish_init(zb_uint8_t param)
{
  zb_device_descriptor_t devInfo;
  Llc_netInfo_t parentDevInfo;
  zb_uint32_t frameCounter = 0;

  // The buffer is not used in this function, so free it.
  zb_mlme_set_confirm_t *conf = (zb_mlme_set_confirm_t*)zb_buf_begin(param);
  Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_finish_init: Setting IEEE address finished with status %x", conf->status);
  ZVUNUSED(conf);
  zb_buf_free(param);

  // Add sensor's structures
  ZB_BZERO(&configSettings, sizeof(Smsgs_configReqMsg_t));

  Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_finish_init: Initializing JDLLC");
  Jdllc_init(&Sensor_macCallbacks, &jdllcCallbacks);
#if defined(TEMP_SENSOR)
    configSettings.frameControl |= Smsgs_dataFields_tempSensor;
#endif
    configSettings.frameControl |= Smsgs_dataFields_msgStats;
    configSettings.frameControl |= Smsgs_dataFields_configSettings;

  // Assuming CERTIFICATION_TEST_MODE will never be defined so just set the reporting interval to CONFIG_REPORTING_INTERVAL and not 100 ms
  configSettings.reportingInterval = CONFIG_REPORTING_INTERVAL;
  configSettings.pollingInterval = CONFIG_POLLING_INTERVAL;

  // Register the MAC callbacks with the translator. The JDLLC will have overridden the application's callbacks by now and stored a copy in its own structure.
  Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_finish_init: Saving the sensor application's callback table.");
  // Registering the callbacks again in order to set the translation layer callbacks to the JDLLC layer callbacks.
  register_callbacks(&Sensor_macCallbacks);

  // Save the NV function pointers (from Ssf_init in ssf.c)
#ifdef NV_RESTORE
  pNV = &Main_user1Cfg.nvFps;
#endif

  // Set the channel page (always 0 since we're working with 2.4 GHz in this use case)
  {
    zb_bufid_t buf = allocateOutBuffer();
    zb_mlme_set_request_t *set_req;
    set_req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_set_request_t) + sizeof(zb_uint8_t));
    ZB_BZERO(set_req, sizeof(zb_mlme_set_request_t) + sizeof(zb_uint8_t));
    set_req -> pib_attr   = ZB_PHY_PIB_CURRENT_PAGE;
    set_req -> pib_length = sizeof(zb_uint8_t);
    *((zb_uint8_t *)(set_req + 1)) = CONFIG_CHANNEL_PAGE;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_finish_init: Setting channel page with buffer ID %d.", buf);
    ZB_SCHEDULE_CALLBACK(zb_mlme_set_request, buf);
  }

#ifdef ZB_MAC_SECURITY
  // Get frame counter
  getFrameCounter(&frameCounter);
#endif /* ZB_MAC_SECURITY */

  // 15.4 stack sets transmit power, min backoff exponent, max backoff exponent, max CSMA backoffs, max frame retries (all of these are configurable)
  // TX power is configurable via ZB_MAC_CONFIGURABLE_TX_POWER
  // Do not make MIN_BE, MAX_BE, MAX_CSMA_BACKOFFS, and MAX_FRAME_RETRIES attributes configurable here. We already have these defined in the ZBOSS stack.

  /* Initialize the app clocks */
  readingClkHandle = UtilTimer_construct(&readingClkStruct,
                                      processReadingTimeoutCallback,
                                      READING_INIT_TIMEOUT_VALUE,
                                      0,
                                      ZB_FALSE,
                                      ZB_UNUSED_PARAM);

  // If there already exists device information in the network in flash
  if (getNetworkInfo(&devInfo, &parentDevInfo) == ZB_TRUE)
  {
#ifdef ZB_MAC_SECURITY
    // Since this is a rejoin, set the MAC PIB attribute for the frame counter to the value retrieved from getFrameCounter.
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_finish_init: Setting frame counter to %d", frameCounter);
    setPibFrameCounterSynchronous(frameCounter);
#endif /* ZB_MAC_SECURITY */
    // Update channel mask to show the previous network channel (as opposed to whatever channel mask was used before) so that the sensor
    // only scans the channel that its parent is on as opposed to every single one since it's not trying to find a new parent. This is
    // necessarily for if the collector disconnects from the network after the sensor rejoins.
    zb_uint8_t channel = parentDevInfo.channel;
    zb_uint32_t channelMaskRejoin = (1L << (channel));
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_finish_init: Parent was on channel %d", channel);
    Jdllc_setChanMask(channelMaskRejoin);

    // Start the device
    started = ZB_TRUE;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_finish_init: Starting the device");
    setEvent(SENSOR_START_EVT);
  }
  else if (CONFIG_AUTO_START)
  {
    // Start the device
    started = ZB_TRUE;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Sensor_finish_init: Starting the device");
    setEvent(SENSOR_START_EVT);
  }
}

/*!
* @brief       Allocate an output buffer. If one cannot be allocated, then reset the device.
*
* @return      The buffer ID of the newly allocated output buffer (if successful).
*/
static zb_bufid_t allocateOutBuffer()
{
  zb_bufid_t buf = zb_buf_get_out();
  if (buf == ZB_BUF_INVALID)
  {
    Log_printf(LogModule_Zigbee_App, Log_ERROR, "sensor::allocateOutBuffer: Failed to allocate out buffer");
    zb_reset(ZB_UNUSED_PARAM);
    // This should never be reached
    return ZB_BUF_INVALID;
  }
  return buf;
}

/*!
 * @brief       set an event.
 *
 * @param       event - event(s) to set
 */
static void setEvent(zb_uint16_t event)
{
  // Enter critical section
  ZB_DISABLE_ALL_INTER();

  // Set the event
  Sensor_events |= event;

  // Exit critical section
  ZB_ENABLE_ALL_INTER();
}

/*!
 * @brief       clear an event
 *
 * @param       event - event(s) to clear
 */
static void clearEvent(zb_uint16_t event)
{
    // Enter critical section
    ZB_DISABLE_ALL_INTER();

    /* Clear the event */
    Sensor_events &= ~(event);

    // Exit critical section
    ZB_ENABLE_ALL_INTER();
}

static void commStatusIndCb(zb_mlme_comm_status_indication_t *pCommStatusInd)
{
  return;
}

static void dataCnfCB(zb_mcps_data_confirm_params_t *pDataCnf)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO, "sensor::dataCnfCB: Entered sensor's data confirm callback function with MSDU handle: %x", pDataCnf->msdu_handle);
  /* Record statistics */
  if (pDataCnf->status == MAC_CHANNEL_ACCESS_FAILURE)
  {
    Sensor_msgStats.channelAccessFailures++;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sensor::dataCnfCB: Data request was unsuccessful because the channel is busy. Channel access failures: %d", Sensor_msgStats.channelAccessFailures);
  }
  else if (pDataCnf->status == MAC_NO_ACK)
  {
    Sensor_msgStats.macAckFailures++;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sensor::dataCnfCB: No ACK was received for the data request. ACK failures: %d", Sensor_msgStats.macAckFailures);
  }
  else if (pDataCnf->status != MAC_SUCCESS)
  {
    Sensor_msgStats.otherDataRequestFailures++;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sensor::dataCnfCB: Data request was unsuccessful because of error status %x. Other data request failures: %d", pDataCnf->status, Sensor_msgStats.otherDataRequestFailures);
  }
  // 15.4 stack updates the frame counter, but there is no frame counter in the ZBOSS data confirmation struct.
  // Handling this by retrieving the frame counter value directly from MAC PIB. Note that this can result in race conditions where the frame counter value is updated
  // between the time the data request is sent and this corresponding data confirmation has been received. This is inconsequential because a higher frame counter value
  // would result in greater chance of being written to flash, which will help reduce the number of packets rejected by the collector if this sensor device resets.
  // Also, any repetitive calls to updateFrameCounter would not be written to flash because the frame counter value would not have changed by more than the save window.
#ifdef ZB_MAC_SECURITY
  else if (pDataCnf->status == MAC_SUCCESS)
  {
    zb_uint32_t frameCounter = getPibFrameCounterSynchronous();
    updateFrameCounter(frameCounter);
  }
#endif

  /* Make sure the message came from the app */
  if (pDataCnf->msdu_handle & APP_MARKER_MSDU_HANDLE)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sensor::dataCnfCB: MSB in MSDU handle is set, so that means that app sent the message.");
    // What message type was the original request? Note that this check means that the E2E delay and message count only increments on confirmation
    // of the collector receiving data from the sensor (ex: temperature) and not configuration or tracking responses.
    if ((pDataCnf->msdu_handle & APP_MASK_MSDU_HANDLE) == APP_SENSOR_MSDU_HANDLE)
    {
      if (pDataCnf->status == MAC_SUCCESS)
      {
        Sensor_msgStats.msgsSent++;
        Log_printf(LogModule_Zigbee_App, Log_INFO, "sensor::dataCnfCB: Can confirm that the data request sent successfully. Messages sent: %d", Sensor_msgStats.msgsSent);
        if (Sensor_msgStats.msgsSent == 0)
        {
          /* the count is wrapped around, set to 1 to avoid divided by zero */
          Sensor_msgStats.msgsSent = 1;
          totalE2EDelaySum = 0;
        }

        /* Calculate end to end delay (time between when sensor sends message and then receives a confirmation) */
        if (ClockP_getSystemTicks() < startSensorMsgTimeStamp)
        {
          // Case where the number of ticks wrapped back to zero after the message got sent
          endToEndDelay = ClockP_getSystemTicks() + (0xFFFFFFFF - startSensorMsgTimeStamp);
        }
        else
        {
          endToEndDelay = ClockP_getSystemTicks() - startSensorMsgTimeStamp;
        }
        // This essentially does nothing because the tick period is 1 microsecond per tick in LPF3
        endToEndDelay = (endToEndDelay + ClockP_getSystemTickPeriod() - 1) / ClockP_getSystemTickPeriod();
        // If adding the most recent end-to-end delay to the total end-to-end delay sum would cause an overflow, then reset the total end-to-end delay sum
        // to this most recent delay. In that case, also reset the messages sent to 1.
        if ( (totalE2EDelaySum + endToEndDelay ) < totalE2EDelaySum)
        {
          totalE2EDelaySum = endToEndDelay;
          Sensor_msgStats.msgsSent = 1;
        }
        // Otherwise, just add the most recent end-to-end delay to the total end-to-end delay sum as normal
        else
        {
          totalE2EDelaySum += endToEndDelay;
        }
        // Update the worst case delay. Is it still the same as before, or is it a new worst case?
        Sensor_msgStats.worstCaseE2EDelay = (Sensor_msgStats.worstCaseE2EDelay > endToEndDelay) ? Sensor_msgStats.worstCaseE2EDelay : endToEndDelay;

        // Update the average delay. Will be inaccurate after the sum overflows for the first time.
        Sensor_msgStats.avgE2EDelay = totalE2EDelaySum / Sensor_msgStats.msgsSent;
      }
    }
    if ((pDataCnf->msdu_handle & APP_MASK_MSDU_HANDLE) == APP_TRACKRSP_MSDU_HANDLE)
    {
      // Confirmed that tracking response was sent successfully to collector
      if (pDataCnf->status == MAC_SUCCESS)
      {
        Sensor_msgStats.trackingResponseSent++;
        Log_printf(LogModule_Zigbee_App, Log_INFO, "sensor::dataCnfCB: Successful data request packet was a tracking response. Tracking responses sent: %d", Sensor_msgStats.trackingResponseSent);
      }
    }
    if ((pDataCnf->msdu_handle & APP_MASK_MSDU_HANDLE) == APP_CONFIGRSP_MSDU_HANDLE)
    {
      // Confirmed that configuration response was sent successfully to collector
      if (pDataCnf->status == MAC_SUCCESS)
      {
        Sensor_msgStats.configResponseSent++;
        Log_printf(LogModule_Zigbee_App, Log_INFO, "sensor::dataCnfCB: Successful data request packet was a config response. Config responses sent: %d", Sensor_msgStats.configResponseSent);
      }
    }
  }
}

static void dataIndCB(zb_mcps_data_indication_t *pDataInd)
{
  // Payload that's part of the buffer getting sent to the collector via data request. Why does 15.4 stack automatically make this payload 2 bytes?
  // The command IDs are one byte.
  zb_uint8_t cmdBytes[SMSGS_TOGGLE_LED_RESPONSE_MSG_LEN];

  // Check that the payload is not NULL and that the MAC service data unit (MSDU) isn't NULL or empty
  if ((pDataInd != NULL) && (pDataInd->msdu != NULL) && (pDataInd->msduLength > 0))
  {
    // Extract the command ID from the payload. According to the 15.4 stack, the sensor message command ID is right at the start of the MSDU pointer.
    // This requires making the sensor command message enum that holds all the possibilities the way 15.4 stack does (did this in sensor.h)
    Smsgs_cmdIds_t cmdId = (Smsgs_cmdIds_t)*(pDataInd->msdu);

    Log_printf(LogModule_Zigbee_App, Log_INFO, "dataIndCB: Entering dataIndCB with command ID %d and MSDU length %d", cmdId, pDataInd->msduLength);

    // If Jdllc_securityCheck is false then reject the message
#ifdef ZB_MAC_SECURITY
    // Call this in the JDLLC layer like in 15.4 stack because 15.4 stack sets the security level in that layer as a global variable and compares
    // it to that. The only difference here is that the security level would be passed to the function and not an entire security struct.
    if (Jdllc_securityCheck(pDataInd->securityLevel) == ZB_FALSE)
    {
      Log_printf(LogModule_Zigbee_App, Log_INFO, "dataIndCB: Security check failed. Rejecting the message.");
      return;
    }
#endif /* ZB_MAC_SECURITY */

    // Switch statement based on the command ID contained in the MSDU from the collector.
    switch (cmdId)
    {
      // Configuration request message: call processConfigRequest, then increase the message stats for the configuration request message.
      // Collector sends this out every 1000 ms.
      case Smsgs_cmdIds_configReq:
        Log_printf(LogModule_Zigbee_App, Log_INFO, "dataIndCB: Processing configuration request from collector");
        processConfigRequest(pDataInd);
        // Increment the number of configuration request messages received
        Sensor_msgStats.configRequests++;
        break;

      // Tracking request message: if the sensor is in the network (determined by JDLLC state being either joined or rejoined), then increase
      // the number of tracking request messages in the message stats. Indicate that a tracking message was received through Ssf_trackingUpdate.
      // Then send the tracking response back directly to the collector. Collector sends this out every 100 ms.
      case Smsgs_cmdIds_trackingReq:
        Log_printf(LogModule_Zigbee_App, Log_INFO, "dataIndCB: Processing tracking request from collector");
        // Make sure the message is the correct size
        if (pDataInd->msduLength == SMSGS_TRACKING_REQUEST_MSG_LENGTH)
        {
          // Only send data if the sensor is in the network
          if ((Jdllc_getProvState() == Jdllc_states_joined) || (Jdllc_getProvState() == Jdllc_states_rejoined))
          {
            // Increment the number of tracking request messages received
            Sensor_msgStats.trackingRequests++;

            Log_printf(LogModule_Zigbee_App, Log_INFO, "dataIndCB: Sensor is in the network, so send tracking response.");
            
            // Send the response message directly
            cmdBytes[0] = (zb_uint8_t) Smsgs_cmdIds_trackingRsp;
            Sensor_sendMsg(Smsgs_cmdIds_trackingRsp, &pDataInd->srcAddr, ZB_TRUE, SMSGS_TRACKING_RESPONSE_MSG_LENGTH, cmdBytes);
          }
        }
        break;

      // Toggle LED message: make sure the MSDU length (message length) is the right size (SMGS_TOGGLE_LED_REQUEST_MSG_LEN). If the sensor is
      // in the network, then send the response directly
      case Smsgs_cmdIds_toggleLedReq:
        Log_printf(LogModule_Zigbee_App, Log_INFO, "dataIndCB: Processing toggle LED request from collector");
        // Make sure the message is the correct size
        if (pDataInd->msduLength == SMSGS_TOGGLE_LED_REQUEST_MSG_LEN)
        {
          // Only send data if sensor is in the network */
          if ((Jdllc_getProvState() == Jdllc_states_joined) || (Jdllc_getProvState() == Jdllc_states_rejoined))
          {
            // Send the response message directly */
            cmdBytes[0] = (zb_uint8_t) Smsgs_cmdIds_toggleLedRsp;
            // Is the red LED on or off?
            redLEDState = (zb_uint8_t)!redLEDState;
            cmdBytes[1] = redLEDState;
#ifndef USE_DMM
            zb_osif_led_toggle(1);
#endif
            Log_printf(LogModule_Zigbee_App, Log_INFO, "dataIndCB: Sensor is in the network, so send LED response. Red LED state: %d", redLEDState);
            Sensor_sendMsg(Smsgs_cmdIds_toggleLedRsp, &pDataInd->srcAddr, ZB_TRUE, SMSGS_TOGGLE_LED_RESPONSE_MSG_LEN, cmdBytes);
          }
        }
        break;

      // Broadcast control message: if there's a parent found (shouldn't it always be this way?), then call processBroadcastCtrlMsg
      case Smgs_cmdIds_broadcastCtrlMsg:
        Log_printf(LogModule_Zigbee_App, Log_INFO, "dataIndCB: Processing broadcast control message from collector");
        break;

      // There's other types of messages guarded by ifdef statements (ramp data if POWER_MEAS is defined, OAD if FEATURE_NATIVE_OAD is defined,
      // start commissioning if FEATURE_SECURE_COMMISSIONING is defiend, if DEVICE_TYPE_MSG is defined).

      // Default case: break (should not receive other types of messages)
      default:
        /* Should not receive other messages */
        break;
    }
  }
}

/* Allow the rest of the sensor's initialization to continue now that the MAC has been reset */
static void resetCnfCb(zb_mlme_reset_confirm_t *pResetCnf)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO, "resetCnfCb: Reset confirm with status %x", pResetCnf->status);
  // Section 8.2.9.3 of IEEE standard suggests that the only status that can be returned is SUCCESS. Not sure how a non-successful
  // status could be returned, but if so, then something definitely went wrong.
  if (pResetCnf->status == MAC_SUCCESS)
  {
    // The next step of the initialization process is the setting of the device's IEEE address.
    ZB_SCHEDULE_CALLBACK(set_ieee_addr, ZB_UNUSED_PARAM);
  }
  else
  {
    Log_printf(LogModule_Zigbee_App, Log_ERROR, "resetCnfCb: An error has occurred. Did not return a successful reset status.");
    while (1){}
  }
}

/*!
 * @brief      Get the next MSDU Handle
 *             The MSDU handle has 3 parts:
 *             - The MSBit(7), when set means the the application sent the
 *               message
 *             - Bit 6, when set means that the app message is a config request
 *             - Bits 0-5, used as a message counter that rolls over.
 *
 * @param      msgType - message command id needed
 *
 * @return     msdu Handle
 */
static zb_uint8_t getMsduHandle(Smsgs_cmdIds_t msgType)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO, "getMsduHandle: Getting next MSDU handle for message type %d", msgType);
  zb_uint8_t msduHandle = deviceTxMsduHandle;

  /* Increment for the next msdu handle, or roll over */
  if (deviceTxMsduHandle >= MSDU_HANDLE_MAX)
  {
    deviceTxMsduHandle = 0;
  }
  else
  {
    deviceTxMsduHandle++;
  }

  /* Add the App specific bit */
  msduHandle |= APP_MARKER_MSDU_HANDLE;

  /* Add the message type bit */
  if (msgType == Smsgs_cmdIds_sensorData || msgType == Smsgs_cmdIds_rampdata)
  {
    msduHandle |= APP_SENSOR_MSDU_HANDLE;
  }
  else if (msgType == Smsgs_cmdIds_trackingRsp)
  {
    msduHandle |= APP_TRACKRSP_MSDU_HANDLE;
  }
  else if (msgType == Smsgs_cmdIds_configRsp)
  {
    msduHandle |= APP_CONFIGRSP_MSDU_HANDLE;
  }

  return (msduHandle);
}

/*!
 * @brief   Build and send sensor data message
 *
 * @param   param - buffer ID containing the extended address of the sensor 
 */
static void processSensorMsgEvt(zb_uint8_t param)
{
  Smsgs_sensorMsg_t sensor;
  ZB_BZERO(&sensor, sizeof(Smsgs_sensorMsg_t));

  // Retrieve the extended address from MAC PIB, then free the buffer
  zb_mlme_get_confirm_t* conf = (zb_mlme_get_confirm_t*)zb_buf_begin(param);
  Log_printf(LogModule_Zigbee_App, Log_INFO, "processSensorMsgEvt: Retrieved PIB attribute for extended address with status %x", conf->status);
  ZB_IEEE_ADDR_COPY((zb_ieee_addr_t*) &sensor.extAddress, (zb_uint8_t*)(conf + 1));
  zb_buf_free(param);

  // Fill in the message. Frame control will tell us which data fields are enabled.
  sensor.frameControl = configSettings.frameControl;
  if (sensor.frameControl & Smsgs_dataFields_tempSensor)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "processSensorMsgEvt: placing temperature sensor data in sensor message");
    ZB_MEMCPY(&sensor.tempSensor, &tempSensor, sizeof(Smsgs_tempSensorField_t));
  }
  if (sensor.frameControl & Smsgs_dataFields_lightSensor)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "processSensorMsgEvt: placing light sensor data in sensor message");
    ZB_MEMCPY(&sensor.lightSensor, &lightSensor, sizeof(Smsgs_lightSensorField_t));
  }
  if (sensor.frameControl & Smsgs_dataFields_humiditySensor)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "processSensorMsgEvt: placing humidity sensor data in sensor message");
    ZB_MEMCPY(&sensor.humiditySensor, &humiditySensor, sizeof(Smsgs_humiditySensorField_t));
  }
  if (sensor.frameControl & Smsgs_dataFields_msgStats)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "processSensorMsgEvt: placing message stats in sensor message");
    ZB_MEMCPY(&sensor.msgStats, &Sensor_msgStats, sizeof(Smsgs_msgStatsField_t));
  }
  if (sensor.frameControl & Smsgs_dataFields_configSettings)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "processSensorMsgEvt: placing config settings in sensor message");
    sensor.configSettings.reportingInterval = configSettings.reportingInterval;
    sensor.configSettings.pollingInterval = configSettings.pollingInterval;
  }


  sendSensorMessage(&collectorAddr, &sensor);
#ifdef USE_DMM
  MACGatt_syncSensorAttributes_util();
#endif
}

/*!
 * @brief   Manually read the sensors
 */
static void readSensors(void)
{
  // Read the temperature sensor value. Note that with F3, there is only one temperature value, but in F2, there's two. Just set both fields
  // to the value returned from Temperature_getTemperature() because the collector will expect there to be two temperature values.
#if defined(TEMP_SENSOR)
  tempSensor.ambienceTemp = Temperature_getTemperature();
  tempSensor.objectTemp = tempSensor.ambienceTemp;
  Log_printf(LogModule_Zigbee_App, Log_INFO, "readSensors: Temperature sensor reading: %d", tempSensor.ambienceTemp);
#endif

}

/*!
 * @brief   Build and send sensor data message
 *
 * @param   pDstAddr - Where to send the message
 * @param   pMsg - pointer to the sensor data
 *
 * @return  true if message was sent, false if not
 */
static zb_bool_t sendSensorMessage(zb_addr_u *pDstAddr, Smsgs_sensorMsg_t *pMsg)
{
  zb_bool_t ret = ZB_FALSE;
  // Changing to using an array of a maximum size for now because I want to try avoiding dynamic memory allocation.
  // Calculated the maximum possible length by seeing everything that can possibly get added to len in sendSensorMessage in 15.4 stack.
  zb_uint8_t pMsgBuf[MAX_SENSOR_DATA_MESSAGE_LENGTH];
  zb_uint16_t len = SMSGS_BASIC_SENSOR_LEN;

  Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: calculating length of the sensor data message. Starting length: %d", len);

  /* Figure out the length */
  if (pMsg->frameControl & Smsgs_dataFields_tempSensor)
  {
    len += SMSGS_SENSOR_TEMP_LEN;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: temperature sensor is part of message. Current length: %d", len);
  }
  if (pMsg->frameControl & Smsgs_dataFields_lightSensor)
  {
    len += SMSGS_SENSOR_LIGHT_LEN;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: light sensor is part of message. Current length: %d", len);
  }
  if (pMsg->frameControl & Smsgs_dataFields_humiditySensor)
  {
    len += SMSGS_SENSOR_HUMIDITY_LEN;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: humidity sensor is part of message. Current length: %d", len);
  }
  if (pMsg->frameControl & Smsgs_dataFields_msgStats)
  {
    //len += SMSGS_SENSOR_MSG_STATS_LEN; (15.4 stack comments this out for some reason)
    len += sizeof(Smsgs_msgStatsField_t);
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: message stats field is part of message. Current length: %d", len);
  }
  if (pMsg->frameControl & Smsgs_dataFields_configSettings)
  {
    len += SMSGS_SENSOR_CONFIG_SETTINGS_LEN;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: config settings is part of message. Current length: %d", len);
  }

  // Removing if (pMsgBuf) check because we're not dynamically allocating memory in this version of sendSensorMessage
  zb_uint8_t *pBuf = pMsgBuf;

  Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: Placing command ID for sensor data inside message buffer.");
  *pBuf++ = (zb_uint8_t)Smsgs_cmdIds_sensorData;

  ZB_MEMCPY(pBuf, pMsg->extAddress, SMGS_SENSOR_EXTADDR_LEN);
  Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: Placing sensor's extended address inside message buffer.");
  pBuf += SMGS_SENSOR_EXTADDR_LEN;

  Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: Placing sensor's frame control field inside message buffer.");
  Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: Frame control field: %x", pMsg->frameControl);
  pBuf = Util_bufferUint16(pBuf, pMsg->frameControl);

  /* Buffer data in order of frameControl mask, starting with LSB */
  if (pMsg->frameControl & Smsgs_dataFields_tempSensor)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: Placing temperature sensor data inside message buffer.");
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Ambience temperature (in Celsius): %d", pMsg->tempSensor.ambienceTemp);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Object temperature (in Celsius): %d", pMsg->tempSensor.objectTemp);
    pBuf = Util_bufferUint16(pBuf, pMsg->tempSensor.ambienceTemp);
    pBuf = Util_bufferUint16(pBuf, pMsg->tempSensor.objectTemp);
  }
  if (pMsg->frameControl & Smsgs_dataFields_lightSensor)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: Placing light sensor data inside message buffer.");
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Light sensor raw data: %d", pMsg->lightSensor.rawData);
    pBuf = Util_bufferUint16(pBuf, pMsg->lightSensor.rawData);
  }
  if (pMsg->frameControl & Smsgs_dataFields_humiditySensor)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: Placing humidity sensor data inside message buffer.");
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Humidity sensor temperature: %d", pMsg->humiditySensor.temp);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Humidity sensor humidity: %d", pMsg->humiditySensor.humidity);
    pBuf = Util_bufferUint16(pBuf, pMsg->humiditySensor.temp);
    pBuf = Util_bufferUint16(pBuf, pMsg->humiditySensor.humidity);
  }
  if (pMsg->frameControl & Smsgs_dataFields_msgStats)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: Placing sensor's message stats inside message buffer.");
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Join attempts: %d", pMsg->msgStats.joinAttempts);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Join fails: %d", pMsg->msgStats.joinFails);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Messages attempted: %d", pMsg->msgStats.msgsAttempted);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Messages sent: %d", pMsg->msgStats.msgsSent);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Tracking requests: %d", pMsg->msgStats.trackingRequests);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Tracking response attempts: %d", pMsg->msgStats.trackingResponseAttempts);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Tracking responses sent: %d", pMsg->msgStats.trackingResponseSent);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Config requests: %d", pMsg->msgStats.configRequests);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Config response attempts: %d", pMsg->msgStats.configResponseAttempts);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Config responses sent: %d", pMsg->msgStats.configResponseSent);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Channel access failures: %d", pMsg->msgStats.channelAccessFailures);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: MAC ACK failures: %d", pMsg->msgStats.macAckFailures);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Other data request failures: %d", pMsg->msgStats.otherDataRequestFailures);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Sync loss indications: %d", pMsg->msgStats.syncLossIndications);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: RX decrypt failures: %d", pMsg->msgStats.rxDecryptFailures);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: TX encrypt failures: %d", pMsg->msgStats.txEncryptFailures);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Reset count: %d", pMsg->msgStats.resetCount);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Reset reason: %d", pMsg->msgStats.lastResetReason);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Join Time (ms): %d", pMsg->msgStats.joinTime);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Interim delay (ms): %d", pMsg->msgStats.interimDelay);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Number of broadcast messages received: %d", pMsg->msgStats.numBroadcastMsgRcvd);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Number of broadcast messages lost: %d", pMsg->msgStats.numBroadcastMsglost);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Average end-to-end delay (us): %d", pMsg->msgStats.avgE2EDelay);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Worst case end-to-end delay (us): %d", pMsg->msgStats.worstCaseE2EDelay);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.joinAttempts);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.joinFails);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.msgsAttempted);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.msgsSent);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.trackingRequests);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.trackingResponseAttempts);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.trackingResponseSent);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.configRequests);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.configResponseAttempts);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.configResponseSent);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.channelAccessFailures);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.macAckFailures);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.otherDataRequestFailures);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.syncLossIndications);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.rxDecryptFailures);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.txEncryptFailures);
    pBuf = Util_bufferUint16(pBuf, resetCount); // Not implemented yet
    pBuf = Util_bufferUint16(pBuf, resetReseason); // Not implemented yet
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.joinTime);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.interimDelay);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.numBroadcastMsgRcvd);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.numBroadcastMsglost);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.avgE2EDelay);
    pBuf = Util_bufferUint16(pBuf, pMsg->msgStats.worstCaseE2EDelay);
  }
  if (pMsg->frameControl & Smsgs_dataFields_configSettings)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sendSensorMessage: Placing sensor's reporting and polling intervals inside message buffer.");
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Reporting interval (ms): %d", pMsg->configSettings.reportingInterval);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "sendSensorMessage: Polling interval (ms): %d", pMsg->configSettings.pollingInterval);
    pBuf = Util_bufferUint32(pBuf, pMsg->configSettings.reportingInterval);
    pBuf = Util_bufferUint32(pBuf, pMsg->configSettings.pollingInterval);
  } 
  ret = Sensor_sendMsg(Smsgs_cmdIds_sensorData, pDstAddr, ZB_TRUE, len, pMsgBuf);
  return (ret);
}

/*!
 * @brief      Process the Config Request message.
 *
 * @param      pDataInd - pointer to the data indication information
 */
static void processConfigRequest(zb_mcps_data_indication_t *pDataInd)
{
  // Initialize status value for sensor message
  Smsgs_statusValues_t stat = Smsgs_statusValues_invalid;
  // Initialize the confirmation response message structure
  Smsgs_configRspMsg_t configRsp;

  Log_printf(LogModule_Zigbee_App, Log_INFO, "processConfigRequest: received config request from collector with source address %x and length %d",
             pDataInd->srcAddr.addr_short, pDataInd->msduLength);
  // Make sure the message is the correct size
  if (pDataInd->msduLength == SMSGS_CONFIG_REQUEST_MSG_LENGTH)
  {
    zb_uint8_t *pBuf = pDataInd->msdu;
    zb_uint16_t frameControl;
    zb_uint32_t reportingInterval;
    zb_uint32_t pollingInterval;

    // Parse the MSDU (get the frame control, reporting interval, and polling interval values, then set the above variables to the retrieved values)
    configSettings.cmdId = (Smsgs_cmdIds_t)*(pBuf++);
    frameControl = Util_parseUint16(pBuf);
    pBuf += 2;
    reportingInterval = Util_parseUint32(pBuf);
    pBuf += 4;
    pollingInterval = Util_parseUint32(pBuf);

    // Change status to success
    stat = Smsgs_statusValues_success;
    // Set the collector's address mode, and then based on that, set the collector's address field
    collectorAddrMode = pDataInd->srcAddrMode;
    if (collectorAddrMode == ZB_ADDR_16BIT_DEV_OR_BROADCAST)
    {
      collectorAddr.addr_short = pDataInd->srcAddr.addr_short;
    }
    else
    {
      ZB_IEEE_ADDR_COPY(collectorAddr.addr_long, pDataInd->srcAddr.addr_long);
    }

    // Handle frame control via the validateFrameControl function.
    configSettings.frameControl = validateFrameControl(frameControl);
    // Case where the frame control returned by validateFrameControl doesn't match the original request, which could happen from the macros
    // for some of the corresponding bits not being set (ex: collector's frame control in the configuration request could have the temp sensor
    // bits set, but TEMP_SENSOR may not be defined).
    if (configSettings.frameControl != frameControl)
    {
      Log_printf(LogModule_Zigbee_App, Log_ERROR, "processConfigRequest: could not validate frame control. Check if all the expected macros are set.");
      stat = Smsgs_statusValues_partialSuccess;
    }
    configRsp.frameControl = configSettings.frameControl;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "processConfigRequest: frame control read from collector's configuration request is %x", frameControl);
    Log_printf(LogModule_Zigbee_App, Log_INFO, "processConfigRequest: validated frame control is %x", configRsp.frameControl);
    // If the reporting interval is not within the desired boundaries, then status becomes partial success
    if ((reportingInterval < MIN_REPORTING_INTERVAL) || (reportingInterval > MAX_REPORTING_INTERVAL))
    {
      stat = Smsgs_statusValues_partialSuccess;
      Log_printf(LogModule_Zigbee_App, Log_ERROR, "processConfigRequest: reporting interval read from collector's configuration request is not within boundaries. Reporting interval received: %d", reportingInterval);
    }
    // Otherwise set reading clock. This is the first time where it gets set.
    else
    {
      configSettings.reportingInterval = reportingInterval;
      {
        zb_uint32_t randomNum;
        // 15.4 stack calls the RNG three times. Their RNG returns a uint8_t. One gets shifted left 16 bits, the second gets shifted left 8 bits,
        // and the third is added to the first two. Total is a 24 bit number. We can do this by calling ZB_RANDOM_U8()
        randomNum = ((ZB_RANDOM_U8() << 16) +
                     (ZB_RANDOM_U8() << 8) + ZB_RANDOM_U8());
        randomNum = (randomNum % reportingInterval) + SENSOR_MIN_POLL_TIME;
#if !defined(IEEE_COEX_TEST) || !defined(COEX_MENU)
        Log_printf(LogModule_Zigbee_App, Log_INFO, "processConfigRequest: setting reporting interval to %d ms", randomNum);
        setReadingClock(randomNum);
#endif
      }
    }

    // Set reporting interval in configuration response
    configRsp.reportingInterval = configSettings.reportingInterval;

    // If polling interval is not within the boundaries, then status becomes partial success
    if ((pollingInterval < MIN_POLLING_INTERVAL) || (pollingInterval > MAX_POLLING_INTERVAL))
    {
      stat = Smsgs_statusValues_partialSuccess;
      Log_printf(LogModule_Zigbee_App, Log_ERROR, "processConfigRequest: polling interval read from collector's configuration request is not within boundaries. Polling interval received: %d", pollingInterval);
    }
    // Otherwise set the polling interval in the config settings to the interval from the request buffer.
    // Set this new poll rate in the JDLLC layer via Jdllc_setPollRate.
    else
    {
      configSettings.pollingInterval = pollingInterval;
      Log_printf(LogModule_Zigbee_App, Log_INFO, "processConfigRequest: setting polling interval to %d ms", pollingInterval);
      Jdllc_setPollRate(configSettings.pollingInterval);
    }
    configRsp.pollingInterval = configSettings.pollingInterval;
  }

  // Send the response message
  configRsp.cmdId = Smsgs_cmdIds_configRsp;
  configRsp.status = stat;

  // Write the configuration settings to flash just like how the network information is written to flash upon a successful join
  configurationUpdate(&configRsp);

  // Response to the source device by calling sendConfigRsp
  sendConfigRsp(&pDataInd->srcAddr, &configRsp);
}

/*!
 * @brief   Build and send Config Response message
 *
 * @param   pDstAddr - Where to send the message
 * @param   pMsg - pointer to the Config Response
 *
 * @return  true if message was sent, false if not
 */
static zb_bool_t sendConfigRsp(zb_addr_u *pDstAddr, Smsgs_configRspMsg_t *pMsg)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO, "sendConfigRsp: populating message buffer with configuration response message fields");
  // Create a message buffer with size SMGS_CONFIG_RESPONSE_MSG_LENGTH, then set a pointer to this buffer.
  zb_uint8_t messageBuffer[SMSGS_CONFIG_RESPONSE_MSG_LENGTH];
  zb_uint8_t *pBuf = messageBuffer;

  // Set the first field (command ID) to indicate that this message is a configuration response
  *pBuf++ = (zb_uint8_t) Smsgs_cmdIds_configRsp;

  // Insert pMsg's status, frame control field, reporting interval, and polling interval into the message buffer in that order
  pBuf = Util_bufferUint16(pBuf, pMsg->status);
  pBuf = Util_bufferUint16(pBuf, pMsg->frameControl);
  pBuf = Util_bufferUint32(pBuf, pMsg->reportingInterval);
  pBuf = Util_bufferUint32(pBuf, pMsg->pollingInterval);

  return Sensor_sendMsg(Smsgs_cmdIds_configRsp, pDstAddr, ZB_TRUE, SMSGS_CONFIG_RESPONSE_MSG_LENGTH, messageBuffer);
}

/*!
 * @brief   Filter the frameControl with readings supported by this device. Taken from 15.4 stack. The settings supported
 *          will be set by using the BITWISE OR operation on the frame control variable.
 *
 * @param   frameControl - suggested frameControl
 *
 * @return  new frame control settings supported
 */
static zb_uint16_t validateFrameControl(zb_uint16_t frameControl)
{
  zb_uint16_t newFrameControl = 0;

#if defined(TEMP_SENSOR)
    if(frameControl & Smsgs_dataFields_tempSensor)
    {
        newFrameControl |= Smsgs_dataFields_tempSensor;
    }
#endif
    if(frameControl & Smsgs_dataFields_msgStats)
    {
        newFrameControl |= Smsgs_dataFields_msgStats;
    }
    if(frameControl & Smsgs_dataFields_configSettings)
    {
        newFrameControl |= Smsgs_dataFields_configSettings;
    } 
    return (newFrameControl);
}

/*!
 * @brief   The device joined callback.
 *
 * @param   pDevInfo - This device's information
 * @param   pParentInfo - This is the parent's information
 */
static void jdllcJoinedCb(zb_device_descriptor_t *pDevInfo,
                          Llc_netInfo_t  *pParentInfo)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO, "jdllcJoinedCb: Device joined callback in sensor application layer.");
  zb_uint32_t randomNum = 0;

  // Copy the device and parent information
  ZB_MEMCPY(&joiningSensorDevice, pDevInfo, sizeof(zb_device_descriptor_t));
  ZB_MEMCPY(&parentInfo, pParentInfo, sizeof(Llc_netInfo_t));

  // Set the collector's address as the parent's address (like the 15.4 stack, we are assuming that this devic ONLY joins a collector's network)
  collectorAddrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;
  collectorAddr.addr_short = pParentInfo->devInfo.shortAddress;

  // Start the reporting timer
  // 15.4 stack calls the RNG three times. Their RNG returns a uint8_t. One gets shifted left 16 bits, the second gets shifted left 8 bits,
  // and the third is added to the first two. Total is a 24 bit number. We can do this by calling ZB_RANDOM_U8()
  randomNum = ((ZB_RANDOM_U8() << 16) +
                (ZB_RANDOM_U8() << 8) + ZB_RANDOM_U8());
  randomNum = (randomNum % configSettings.reportingInterval) + SENSOR_MIN_POLL_TIME;
  Log_printf(LogModule_Zigbee_App, Log_INFO, "jdllcJoinedCb: Setting reading clock to %d ms", randomNum);
  setReadingClock(randomNum);

  // Inform the user of the joined information (Ssf_networkUpdate in the 15.4 stack)
  networkUpdate(rejoining, pDevInfo, pParentInfo);

  // If security is enabled, then add the parent device to the MAC device table if this isn't a rejoin.
#ifdef ZB_MAC_SECURITY
  if (rejoining == ZB_FALSE)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "jdllcJoinedCb: Adding parent's device information to MAC device table.");
    Jdllc_addSecDevice(pParentInfo->devInfo.panID, pParentInfo->devInfo.shortAddress, &pParentInfo->devInfo.extAddress, 0);
  }
#endif /* ZB_MAC_SECURITY */

  // Calculate join time
  if (ClockP_getSystemTicks() < joinTimeTicks)
  {
    // Handle wrap around case
    joinTimeTicks = ClockP_getSystemTicks() + (0xFFFFFFFF - joinTimeTicks);
  }
  else
  {
    joinTimeTicks = ClockP_getSystemTicks() - joinTimeTicks;
  }
  // Update join time in sensor message stats (to ms)
  Sensor_msgStats.joinTime = (joinTimeTicks / ClockP_getSystemTickPeriod()) / 1000;

#ifdef DISPLAY_PER_STATS
    /* clear the stats used for PER so that we start out at a
     * zeroed state
     */
    Sensor_msgStats.macAckFailures = 0;
    Sensor_msgStats.otherDataRequestFailures = 0;
    Sensor_msgStats.msgsSent = 0;
#endif
}

static void jdllcDisassocIndCb(zb_ieee_addr_t *extAddress, zb_disassociate_reason_t reason)
{
  return;
}

static void jdllcDisassocCnfCb(zb_ieee_addr_t *extAddress, zb_mac_status_t status)
{
  // TODO: If MAC security is enabled, then delete the device. 15.4 uses ApiMac_secDeleteDevice. See what this does.

  // Stop the reporting timer and clear the network information.
  if (UtilTimer_isActive(&readingClkStruct) == true)
  {
    UtilTimer_stop(&readingClkStruct);
  }
  clearNetworkInfo();
}

static void jdllcStateChangeCb(Jdllc_states_t state)
{
#if USE_DMM
  /* Optional DMM Application policy state updates may be added here */

  PROVGatt_syncProvisioningAttributes_util();

#endif /* USE_DMM */
return;
}

/*!
 * @brief   Reading timeout handler function.
 *
 * @param   a0 - ignored
 */
static void processReadingTimeoutCallback(uintptr_t a0)
{
  (void)a0; /* Parameter is not used */

  setEvent(SENSOR_READING_TIMEOUT_EVT);

  Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "Entered processReadingTimeoutCallback");

  // See if this wakes up the device so there's not 10 seconds between reads
  SemaphoreP_post(wakeSem);
}

/*!
 Set the reading clock. Note that reading time is in milliseconds.
 */
void setReadingClock(zb_uint32_t readingTime)
{
  // Assuming that readingClkStruct is initialized by the time this function is called. This should always be the case
  // because the construction of readingClkStruct is done in Jdllc_init().

  /* Stop the Reading timer */
  if (UtilTimer_isActive(&readingClkStruct) == true)
  {
    UtilTimer_stop(&readingClkStruct);
  }

  /* Setup timer */
  if (readingTime > 0)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "setReadingClock: Setting reading timer period to %d ms", readingTime);
    UtilTimer_setTimeout(readingClkHandle, readingTime);
    UtilTimer_start(&readingClkStruct);
  }
}

/**
 * @brief Trigger the sensor start event.
 *
 * @return  None
 */
void triggerSensorStartEvt(void)
{
  started = ZB_TRUE;
  setEvent(SENSOR_START_EVT);
}

/**
 * @brief Get the current reporting interval for the sensor.
 *
 * @return Reporting interval in milliseconds.
 */
uint32_t sensorGetReportingInterval(void)
{
  return configSettings.reportingInterval;
}

/**
 * @brief Get the current temperature sensor data.
 *
 * @return Temperature value in Celsius.
 */
uint16_t getTempSensorData(void)
{
  return (tempSensor.ambienceTemp);
}
