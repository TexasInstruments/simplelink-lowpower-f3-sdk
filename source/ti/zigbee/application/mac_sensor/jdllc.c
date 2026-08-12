/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2025, Texas Instruments Incorporated
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
#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#include "sensor_mac_config_settings.h"
#include "jdllc.h"
#include "sensor.h"
#include "zb_scheduler.h"
#include "mac_disassociate.h"

/******************************************************************************
 Constants and definitions
 *****************************************************************************/
/*
 sensor reporting interval in milliseconds used to restart sensor reading
 after coordinator realignment
*/
#define JDLLC_RESTART_REPORTING_INTERVAL 1000

#define JDLLC_BEACON_ORDER_NON_BEACON 15
#define JDLLC_INVALID_PAN             0xFFFF
#define JDLLC_INVALID_CHANNEL         0xFF
#define JDLLC_RETRY_POLL              500  /* In ms, and used for retrying polling with a shorter interval */
#define UtilTimer_MS_ADJUSTMENT       1000 /* Used for converting milliseconds into ClockP ticks, which is 1 microsecond (1000 ms) */

// In the association request, there has to be a bit set to indicate that the collector should allocate an address for the sensor. Not only that,
// but there also needs to be a bit set to indicate that the sensor is a sleepy or non-sleepy device. So this capability information bit field
// will change depending on CONFIG_RX_ON_WHEN_IDLE. If the RX on when idle bit in the capability information is not modified, there will be unexpected
// results seen over the air (ex: if the device is non-sleepy but the corresponding bit is not set, the collector will still think the sensor is sleepy
// and not send anything over the air until there was a poll).
                                
/* Allocate address (collector should set the sensor's 16-bit address during association) and set RX on when idle value */
#define CAP_INFO                (0x80 | ((CONFIG_RX_ON_IDLE) << (3)))

/******************************************************************************
 Structures
 *****************************************************************************/
/* Device information, used to store default parameters */
typedef struct
{
    zb_uint16_t panID;
    zb_uint8_t channel;
    zb_uint16_t coordShortAddr;
    zb_ieee_addr_t coordExtAddr;
    zb_uint16_t devShortAddr;
    zb_ieee_addr_t devExtAddr;
    zb_uint8_t beaconOrder;
    zb_uint8_t superframeOrder;
    Jdllc_states_t currentJdllcState;
    Jdllc_states_t prevJdllcState;
    Jdllc_device_states_t currentDevState;
    Jdllc_device_states_t prevDevState;
    zb_uint8_t dataFailures;
    zb_uint32_t pollInterval;
} devInformation_t;

/******************************************************************************
 Global variables
 *****************************************************************************/
/* Task pending events */
zb_uint16_t Jdllc_events = 0;

extern SemaphoreP_Handle wakeSem;

/* Interim Delay Ticks (used for rejoin delay calculation) */
static uint_fast32_t interimDelayTicks = 0;

zb_ieee_addr_t defaultExtAddr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/******************************************************************************
 Local variables
 *****************************************************************************/
/* structure containing device and its parents information*/
static volatile devInformation_t devInfoBlock =
                {
                  CONFIG_PAN_ID,
                  JDLLC_INVALID_CHANNEL,
                  0xFFFF,
                  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
                  0xFFFF,
                  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
                  CONFIG_MAC_BEACON_ORDER,
                  CONFIG_MAC_SUPERFRAME_ORDER,
                  Jdllc_states_initWaiting,
                  Jdllc_states_initWaiting,
                  Jdllc_deviceStates_scanActive,
                  Jdllc_deviceStates_scanActive,
                  0,
                  CONFIG_POLLING_INTERVAL
                };

/* default channel mask */
static zb_uint32_t defaultChannelMask = CONFIG_CHANNEL_MASK;

/* copy of MAC API callbacks */
static zb_mac_callbacks_t macCallbacksCopy =  { 0 };
/* copy of CLLC callbacks */
static Jdllc_callbacks_t *pJdllcCallbacksCopy = (Jdllc_callbacks_t *)NULL;

/* Clock/timer resources */
/* poll timer */
static ClockP_Struct pollClkStruct;
static ClockP_Handle pollClkHandle;
/* scan backoff timer */
static ClockP_Struct scanBackoffClkStruct;
static ClockP_Handle scanBackoffClkHandle;
/* reading timer (defined in sensor.c but still used here, part of effort to not add a SSF layer) */
extern ClockP_Struct readingClkStruct;
extern ClockP_Handle readingClkHandle;

/* Flag to pick parent from incoming beacons */
bool parentFound = false;

/* Flag to indicate whether a valid matching PAN ID has been identified from
 * an incoming beacon
 */
static bool panIdMatch = false;

static zb_uint8_t numSyncLoss = 0;

#ifdef ZB_MAC_SECURITY
/******************************************************************************
 Local security variables (taken from jdllc.c in 15.4 stack)
 *****************************************************************************/

/* This is just a random number used for key ID lookup entry in 15.4 stack when identifying the key. When the packet gets sent, that's what put into octets.
   Then they will know that it's the only key used. */
static const zb_uint8_t secKeySource[8] = {0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33};

/* AES encryption and MIC-32 authentication are used */
static const zb_uint8_t secLevel = ZB_MAC_SECURITY_LEVEL_ENCMIC32;

/* Key is determined explicitly from the 8-octet key source subfield and the 1-octet key index subfield of the key identifier field of the auxiliary security header */
static const zb_uint8_t secKeyIdMode = ZB_MAC_KEY_ID_MODE_8;

/* The security key index allows unique identification of different keys from the same originator. The key originator must make sure that actively used keys
   that it issues have distinct key indices and that the key indices are all different from 0. Here it's set to 3. */
static const zb_uint8_t secKeyIndex = 3;

#endif /* ZB_MAC_SECURITY */

/******************************************************************************
 Local Function Prototypes
 *****************************************************************************/
static void setEvent(zb_uint16_t event);
static void clearEvent(zb_uint16_t event);
static zb_bufid_t allocateOutBuffer();

/* ClockP callbacks */
static void processPollTimeoutCallback(uintptr_t a0);
static void processScanBackoffTimeoutCallback(uintptr_t a0);

/* CLLC callbacks */
static void assocCnfCb(zb_mlme_associate_confirm_t *pData);
static void beaconNotifyIndCb(zb_mac_beacon_notify_indication_t *pData);
static void scanCnfCb(zb_mac_scan_confirm_t *pData);
static void disassoCnfCb(zb_mac_disassociate_confirm_t *pData);
static void disassocIndCb(zb_mac_disassociate_indication_t *pData);
static void dataCnfCb(zb_mcps_data_confirm_params_t *pData);
static void pollCnfCb(zb_ret_t status);

static void switchState(Jdllc_device_states_t newState);
static void processState(Jdllc_device_states_t state);
static bool checkBeaconOrder(zb_uint16_t* superframeSpec);
static void sendAssocReq(void);
static void updateState(Jdllc_states_t state);
static void sendPollReq(void);
static void processPollEvt(zb_uint8_t param);
static void processCoordRealign(zb_uint8_t param);
static void sendScanReq(zb_uint8_t scan_type);
static void populateInfo(zb_device_descriptor_t *pDevInfo,
                         Llc_netInfo_t *pParentNetInfo);
static void handleMaxDataFail(void);
static void handleSuccessfulJoin(void);

/* Helper function for MLME-GET.confirm primitive */
static void getPibAttrCb(zb_uint8_t param);

/******************************************************************************
 ClockP Setup Functions
 *****************************************************************************/
void setScanBackoffClock(zb_uint32_t scanBackoffTime);
void setPollClock(zb_uint32_t pollTime);

/******************************************************************************
 Public Functions
 *****************************************************************************/

/*!
 Initialize this module.

 Public function defined in jdllc.h
 */
void Jdllc_init(zb_mac_callbacks_t *pMacCbs, Jdllc_callbacks_t *pJdllcCbs)
{
    // Joining device will only connect to coordinators that send a beacon when requested to
    if (CONFIG_MAC_BEACON_ORDER == JDLLC_BEACON_ORDER_NON_BEACON)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Setting state to active scan");
        // Active scan --> send beacon requests and listen for beacons
        devInfoBlock.currentDevState = Jdllc_deviceStates_scanActive;
        devInfoBlock.prevDevState = Jdllc_deviceStates_scanActive;
    }
    // Joining device will only connect to coordinators that periodically send beacons
    else if ((CONFIG_MAC_BEACON_ORDER > 0) && (CONFIG_MAC_BEACON_ORDER < JDLLC_BEACON_ORDER_NON_BEACON))
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Setting state to passive scan");
        // Passive scan --> only listen for beacons, no sending requests
        devInfoBlock.currentDevState = Jdllc_deviceStates_scanPassive;
        devInfoBlock.prevDevState = Jdllc_deviceStates_scanPassive;
    }

    // Copy the application's MAC callbacks into a structure only used in the JDLLC. These application-level callbacks will be called after
    // the JDLLC-specific versions finish executing.
    ZB_MEMCPY(&macCallbacksCopy, pMacCbs, sizeof(zb_mac_callbacks_t));

    // Save the JDLLC callbacks that are used in the application layer.
    pJdllcCallbacksCopy = pJdllcCbs;

    // Overwrite the MAC callbacks callbacks with LLC callbacks. Uncomment after each define.
    pMacCbs->pAssocCnfCb = assocCnfCb;
    pMacCbs->pDisassociateCnfCb = disassoCnfCb;
    pMacCbs->pDataCnfCb = dataCnfCb;
    pMacCbs->pPollCnfCb = pollCnfCb;
    // pMacCbs->pDisassociateIndCb = disassocIndCb;
    pMacCbs->pBeaconNotifyIndCb = beaconNotifyIndCb;
    pMacCbs->pScanCnfCb = scanCnfCb;
    
    // Create and initialize poll clock if device is a sleepy ZED.
    if (!CONFIG_RX_ON_IDLE)
    {
        pollClkHandle = UtilTimer_construct(&pollClkStruct,
                                     processPollTimeoutCallback,
                                     CONFIG_POLLING_INTERVAL,
                                     0,
                                     ZB_FALSE,
                                     ZB_UNUSED_PARAM);
    }

    // Turn off RX until joining (this should only happen during initial joining and rejoining)
    {
        // Taken from frame_validation_03_ffd1.c
        zb_bufid_t buf = allocateOutBuffer();
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_init: turning off RX with buffer ID %d", buf);
        zb_mlme_set_request_t *set_req;
        set_req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_set_request_t) + sizeof(zb_uint8_t));
        ZB_BZERO(set_req, sizeof(zb_mlme_set_request_t));
        set_req -> pib_attr   = ZB_PIB_ATTRIBUTE_RX_ON_WHEN_IDLE;
        set_req -> pib_length = sizeof(zb_uint8_t);
        *((zb_uint8_t *)(set_req + 1)) = CONFIG_RX_ON_IDLE; // Why does 15.4 stack do it like this instead of setting it to false?
        ZB_SCHEDULE_CALLBACK(zb_mlme_set_request, buf);
    }

    // Initialize scan backoff clock. Make it a ClockP module 
    // Duration is equal to CONFIG_SCAN_BACKOFF_INTERVAL. So processScanBackoffTimeoutCallback will be called CONFIG_SCAN_BACKOFF_INTERVAL ms after the clock is started.
    // Period is 0, meaning this is a one-shot function (not periodic).
    // Do not start immediately after construction is true. Wait for call to start.
    scanBackoffClkHandle = UtilTimer_construct(&scanBackoffClkStruct, processScanBackoffTimeoutCallback, 
                                               CONFIG_SCAN_BACKOFF_INTERVAL, 0, ZB_FALSE, ZB_UNUSED_PARAM);
}

/*!
 Jdllc task processing.

 Public function defined in jdllc.h
 */
void Jdllc_process(void)
{
    /* Process poll event */
    if (Jdllc_events & JDLLC_POLL_EVT)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_process: Processing poll event");

        // Originally, the processPollEvt function was here, but the logic requires that you get
        // the auto request PIB attribute, which dictates what the function does. Because we need to schedule the get request,
        // let's form the get request here, and then use processPollEvt as the callback.
        {
            zb_bufid_t buf = allocateOutBuffer();
            Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_process: getting PIB attribute for auto request with buffer ID %d", buf);
            zb_mlme_get_request_t *req;

            req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_get_request_t));
            req->pib_attr   = ZB_PIB_ATTRIBUTE_AUTO_REQUEST;
            req->iface_id  = ZB_PIBCACHE_PRIMARY_IFACE();
            req->confirm_cb_u.cb = processPollEvt;

            zb_mlme_get_request(buf);
        }

        /* Clear the event */
        clearEvent(JDLLC_POLL_EVT);
    }

    /* Send association request */
    if (Jdllc_events & JDLLC_ASSOCIATE_REQ_EVT)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_process: Processing association request event");
        /* Send Associate request */
        sendAssocReq();

        /* Clear the event */
        clearEvent(JDLLC_ASSOCIATE_REQ_EVT);
    }

    /* Handle successful join */
    if (Jdllc_events & JDLLC_JOIN_EVT)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_process: Handle successful join");
        /* Call corresponding pJoinedCb in application layer and then set poll event afterwards */
        handleSuccessfulJoin();

        /* Clear the event */
        clearEvent(JDLLC_JOIN_EVT);
    }

    /* Process coordinator realignment (happens when there's a coordinator realignment packet picked up from an orphan scan) */
    if (Jdllc_events & JDLLC_COORD_REALIGN)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_process: Processing coordinator realignment event");
        // Originally, the processCoordRealign function was called here, but the function requires that you get
        // the PAN ID, which dictates what the function does. Because we need to schedule the get request, let's
        // form the get request here, and then use processCoordRealign as the callback.
        {
            zb_bufid_t buf = allocateOutBuffer();
            Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_process: getting PIB attribute for PAN ID with buffer ID %d", buf);
            zb_mlme_get_request_t *req;

            req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_get_request_t));
            req->pib_attr   = ZB_PIB_ATTRIBUTE_PANID;
            req->iface_id  = ZB_PIBCACHE_PRIMARY_IFACE();
            req->confirm_cb_u.cb = processCoordRealign;

            zb_mlme_get_request(buf);
        }

        /* Clear the event */
        clearEvent(JDLLC_COORD_REALIGN);
   }

    /* Process scan backoff in case of orphan scan */
    if (Jdllc_events & JDLLC_SCAN_BACKOFF)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_process: Processing scan backoff event");
        switchState(devInfoBlock.prevDevState);
        /* Clear the event */
        clearEvent(JDLLC_SCAN_BACKOFF);
    }

    /* Process state change event */
    if (Jdllc_events & JDLLC_STATE_CHANGE_EVT)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_process: Processing state change event");
        /* Process LLC Event */
        processState(devInfoBlock.currentDevState);

        /* Clear the event */
        clearEvent(JDLLC_STATE_CHANGE_EVT);
    }
}

/*!
 Set PANID

 Public function defined in jdllc.h
 */
void Jdllc_setJoiningPanId(zb_uint16_t panId)
{
    if(devInfoBlock.currentJdllcState == Jdllc_states_initWaiting)
    {
        devInfoBlock.panID = panId;
    }
}

/*!
 Get PANID

 Public function defined in jdllc.h
 */
void Jdllc_getJoiningPanId(zb_uint16_t *pPanId)
{
    // Modifying this function so that the only time this function gets called is when it's the first time the collector is setting the an ID.
    // Use the value that was either compiled or selected through the collector's CUI. The rest of this logic is inlined in getProvisioningCb so
    // that I don't have to move the getNetworkInfo function into this file.
    *pPanId = devInfoBlock.panID;
}

/*!
 Set Channel Mask

 Public function defined in jdllc.h
 */
void Jdllc_setChanMask(zb_uint32_t chanMask)
{
    if (devInfoBlock.currentJdllcState == Jdllc_states_initWaiting)
    {
        defaultChannelMask = chanMask;
    }
}

/*!
 Get Channel Mask

 Public function defined in jdllc.h
 */
void Jdllc_getChanMask(zb_uint8_t *_chanMask)
{
    ZB_MEMCPY(_chanMask, &defaultChannelMask, sizeof(zb_uint32_t)); // Because the channel mask is being held in a uint32_t
}

/*!
 Get the collector (Full Function Device - FFD) address

 Public function defined in jdllc.h
 */
void Jdllc_getFfdAddr(zb_ieee_addr_t *addr)
{
    // Put in an explicit cast to ensure the compiler that the volatile qualifier is being removed.
    ZB_IEEE_ADDR_COPY(addr, (zb_ieee_addr_t*) &devInfoBlock.coordExtAddr);
}

/*!
 Get the current PHY ID of the sensor device

 Public function defined in jdllc.h
 */
zb_uint8_t Jdllc_getFreq()
{
    // There's no PHY ID attribute in ZBOSS, but there is such an attribute in the 15.4 stack.
    // In the 15.4 stack, there's only one option for 2.4 GHz, which is 0 (corresponds to APIMAC_250KBPS_IEEE_PHY_0).
    // Will always send at 250 kb/s.
    return (0);
}

/*!
 Get the current channel of the sensor device

 Public function defined in jdllc.h
 */
zb_uint8_t Jdllc_getChan()
{
    //return non-zero indexed value
    return (devInfoBlock.channel - 1);
}

/*!
 Get the current state of the sensor device

 Public function defined in jdllc.h
 */
zb_uint8_t Jdllc_getProvState()
{
    return (devInfoBlock.currentJdllcState);
}

/*!
 Get the previous state of the sensor device

 Public function defined in jdllc.h
 */
zb_uint8_t Jdllc_getPrevProvState()
{
    return (devInfoBlock.prevJdllcState);
}

/*!
 Join the network

 Public function defined in jdllc.h
 */
void Jdllc_join()
{
    // Set state
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_join: update state to joining state");
    updateState(Jdllc_states_joining);

    // If non-beacon network (beacon doesn't get periodically sent, only on request), perform an active scan. If beacon enabled network, perform a passive scan.
    if (CONFIG_MAC_BEACON_ORDER == JDLLC_BEACON_ORDER_NON_BEACON)
    {
        // Non-beacon network
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_join: non-beacon network, perform an active scan");
        switchState(Jdllc_deviceStates_scanActive);
    }
    else if((CONFIG_MAC_BEACON_ORDER > 0) && (CONFIG_MAC_BEACON_ORDER < JDLLC_BEACON_ORDER_NON_BEACON))
    {
        // Beacon network
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_join: beacon network, perform a passive scan");
        switchState(Jdllc_deviceStates_scanPassive);
    }
}

/*!
 Restore the device in the network. The application will call this function to restore the device
 in the network by passing all the network information needed to restore the device.

 This module will configure the MAC with all the network information, then start the device
 without scanning.

 Public function defined in jdllc.h
 */
void Jdllc_rejoin(zb_device_descriptor_t *pDevInfo,
                  Llc_netInfo_t *pParentInfo)
{
    // Set state
    updateState(Jdllc_states_initRestoring);

    // Update device information variables (handle the volatile keyword issues)
    devInfoBlock.panID = pDevInfo->panID;
    devInfoBlock.channel = pParentInfo->channel;
    // Put in an explicit cast to ensure the compiler that the volatile qualifier is being removed.
    ZB_IEEE_ADDR_COPY((zb_ieee_addr_t*) &devInfoBlock.devExtAddr, &pDevInfo->extAddress);

    devInfoBlock.devShortAddr = pDevInfo->shortAddress;
    // Put in an explicit cast to ensure the compiler that the volatile qualifier is being removed.
    ZB_IEEE_ADDR_COPY((zb_ieee_addr_t*) &devInfoBlock.coordExtAddr, &pParentInfo->devInfo.extAddress);
    devInfoBlock.coordShortAddr = pParentInfo->devInfo.shortAddress;

    // Update MAC PIBs
    {
        zb_bufid_t buf = allocateOutBuffer();
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_rejoin: setting PAN ID PIB attribute to %x with buffer ID %d", devInfoBlock.panID, buf);
        zb_mlme_set_request_t *set_req;
        set_req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_set_request_t) + sizeof(zb_uint16_t));
        ZB_BZERO(set_req, sizeof(zb_mlme_set_request_t));
        set_req -> pib_attr   = ZB_PIB_ATTRIBUTE_PANID;
        set_req -> pib_length = sizeof(zb_uint16_t);
        zb_uint8_t *aligned_address = (zb_uint8_t *)set_req + sizeof(zb_mlme_set_request_t);
        ZB_MEMCPY(aligned_address, (zb_uint16_t*) &devInfoBlock.panID, sizeof(zb_uint16_t));
        ZB_SCHEDULE_CALLBACK(zb_mlme_set_request, buf);
    }
    {
        zb_bufid_t buf = allocateOutBuffer();
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_rejoin: setting device short address attribute to %x with buffer ID %d", devInfoBlock.devShortAddr, buf);
        zb_mlme_set_request_t *set_req;
        set_req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_set_request_t) + sizeof(zb_uint16_t));
        ZB_BZERO(set_req, sizeof(zb_mlme_set_request_t));
        set_req -> pib_attr   = ZB_PIB_ATTRIBUTE_SHORT_ADDRESS;
        set_req -> pib_length = sizeof(zb_uint16_t);
        zb_uint8_t *aligned_address = (zb_uint8_t *)set_req + sizeof(zb_mlme_set_request_t);
        ZB_MEMCPY(aligned_address, (zb_uint16_t*) &devInfoBlock.devShortAddr, sizeof(zb_uint16_t));
        ZB_SCHEDULE_CALLBACK(zb_mlme_set_request, buf);
    }
    {
        zb_bufid_t buf = allocateOutBuffer();
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_rejoin: setting coordinator's IEEE address with buffer ID %d.", buf);
        zb_mlme_set_request_t *req;
        req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_set_request_t) + sizeof(zb_ieee_addr_t));
        ZB_BZERO(req, sizeof(zb_mlme_set_request_t) + sizeof(zb_ieee_addr_t));
        req->pib_attr = ZB_PIB_ATTRIBUTE_COORD_EXTEND_ADDRESS;
        req->pib_length = sizeof(zb_ieee_addr_t);
        ZB_MEMCPY((zb_uint8_t *)(req + 1), (zb_ieee_addr_t*) &devInfoBlock.coordExtAddr, sizeof(zb_ieee_addr_t));
        ZB_SCHEDULE_CALLBACK(zb_mlme_set_request, buf);
    }
    {
        zb_bufid_t buf = allocateOutBuffer();
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_rejoin: setting channel to %d with buffer ID %d", devInfoBlock.channel, buf);
        zb_mlme_set_request_t *set_req;
        set_req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_set_request_t) + sizeof(zb_uint8_t));
        ZB_BZERO(set_req, sizeof(zb_mlme_set_request_t));
        set_req -> pib_attr   = ZB_PHY_PIB_CURRENT_CHANNEL;
        set_req -> pib_length = sizeof(zb_uint8_t);
        zb_uint8_t *aligned_address = (zb_uint8_t *)set_req + sizeof(zb_mlme_set_request_t);
        ZB_MEMCPY(aligned_address, (zb_uint8_t*) &devInfoBlock.channel, sizeof(zb_uint8_t));
        ZB_SCHEDULE_CALLBACK(zb_mlme_set_request, buf);
    }
    {
        zb_bufid_t buf = allocateOutBuffer();
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_rejoin: setting coordinator's short address attribute to %x with buffer ID %d", devInfoBlock.coordShortAddr, buf);
        zb_mlme_set_request_t *set_req;
        set_req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_set_request_t) + sizeof(zb_uint16_t));
        ZB_BZERO(set_req, sizeof(zb_mlme_set_request_t));
        set_req -> pib_attr   = ZB_PIB_ATTRIBUTE_COORD_SHORT_ADDRESS;
        set_req -> pib_length = sizeof(zb_uint16_t);
        zb_uint8_t *aligned_address = (zb_uint8_t *)set_req + sizeof(zb_mlme_set_request_t);
        ZB_MEMCPY(aligned_address, (zb_uint16_t*) &devInfoBlock.coordShortAddr, sizeof(zb_uint16_t));
        ZB_SCHEDULE_CALLBACK(zb_mlme_set_request, buf);
    }

    if ((CONFIG_MAC_BEACON_ORDER > 0) && (CONFIG_MAC_BEACON_ORDER < JDLLC_BEACON_ORDER_NON_BEACON))
    {
        // Send sync request for beacon enabled device (eventually handle this, but this is not a high priority because
        // this we have a non-beacon order device)
        switchState(Jdllc_deviceStates_syncReq);
        Log_printf(LogModule_Zigbee_App, Log_ERROR, "Jdllc_rejoin: Sync requests are not implemented yet!");
        while (1){}

        // Device joined
        if(pJdllcCallbacksCopy && pJdllcCallbacksCopy->pJoinedCb)
        {
            Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_rejoin: Calling the original device joined callback function.");
            pJdllcCallbacksCopy->pJoinedCb(pDevInfo, pParentInfo);
        }
        updateState(Jdllc_states_rejoined);
    }

    if (!CONFIG_RX_ON_IDLE)
    {
        // Set event for polling if sleepy device
        setEvent(JDLLC_POLL_EVT);
    }
    else
    {
        zb_uint32_t randomNum;
        // 15.4 stack calls the RNG three times. Their RNG returns a uint8_t. One gets shifted left 16 bits, the second gets shifted left 8 bits,
        // and the third is added to the first two. Total is a 24 bit number. We can do this by calling ZB_RANDOM_U8()
        randomNum = ((ZB_RANDOM_U8() << 16) +
                (ZB_RANDOM_U8() << 8) + ZB_RANDOM_U8());
        randomNum = (randomNum % JDLLC_RESTART_REPORTING_INTERVAL) +
                SENSOR_MIN_POLL_TIME;
#if !defined(IEEE_COEX_TEST) || !defined(COEX_MENU)
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_rejoin: setting reporting interval to %d ms", randomNum);
        setReadingClock(randomNum);
#endif
    }
}

/*!
 Set the poll interval.

 Public function defined in jdllc.h
 */
void Jdllc_setPollRate(zb_uint32_t pollInterval)
{
    devInfoBlock.pollInterval = pollInterval;
}

/*!
 Send disassociation request.

 Public function defined in jdllc.h
 */
void Jdllc_sendDisassociationRequest()
{
    zb_uint16_t coordShortAddr = devInfoBlock.coordShortAddr;

    zb_bufid_t buf = allocateOutBuffer();
    
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_sendDisassociationRequest: Sending disassociation request with buffer ID %d", buf);

    // Zero-init the data structure. Do not add security yet.
    zb_mlme_disassociate_request_t *disassocReq = ZB_BUF_GET_PARAM(buf, zb_mlme_disassociate_request_t);
    ZB_BZERO(disassocReq, sizeof(zb_mlme_disassociate_request_t));

    disassocReq->deviceAddress.addr_short = devInfoBlock.coordShortAddr;
    disassocReq->addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;
    disassocReq->devicePanId = devInfoBlock.panID;
    disassocReq->disassociateReason = DISASSOCIATE_DEVICE;
    disassocReq->txIndirect = ZB_FALSE;

    ZB_SCHEDULE_CALLBACK(zb_mlme_disassociate_request, buf);
}

#ifdef ZB_MAC_SECURITY

/*!
 Fill in the security structure

 Public function defined in jdllc.h
 */
void Jdllc_securityFill(zb_uint8_t *pSecurityLevel, zb_uint8_t *pKeyIdMode, zb_uint8_t *pKeySource, zb_uint8_t *pKeyIndex)
{
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_securityFill: filling in device's security parameters with security level %d, key ID mode of %d, key index of %d", secLevel, secKeyIdMode, secKeyIndex);
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_securityFill: key source: %x %x %x %x", secKeySource[0], secKeySource[1], secKeySource[2], secKeySource[3]);
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_securityFill: key source (continued): %x %x %x %x", secKeySource[4], secKeySource[5], secKeySource[6], secKeySource[7]);
    ZB_MEMCPY(pKeySource, &secKeySource, sizeof(secKeySource));
    *pSecurityLevel = secLevel;
    *pKeyIdMode = secKeyIdMode;
    *pKeyIndex = secKeyIndex;
}

/*!
 Check the security level against expected level

 Public function defined in jdllc.h
 */
zb_bool_t Jdllc_securityCheck(zb_uint8_t securityLevel)
{
    if (securityLevel == secLevel)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_securityCheck: Security level of incoming message matches expected level %d", securityLevel, secLevel);
        return ZB_TRUE;
    }
    Log_printf(LogModule_Zigbee_App, Log_ERROR, "Jdllc_securityCheck: Security level of incoming message (%d) does not match expected level %d", securityLevel, secLevel);
    return ZB_FALSE;
}

/*!
 Add a device to the MAC security device table.

 Public function defined in sensor.h
 */
void Jdllc_addSecDevice(zb_uint16_t panID, zb_uint16_t shortAddr, zb_ieee_addr_t *pExtAddr, zb_uint32_t frameCounter)
{
    // You can't use a set request for any of the security PIB attributes because there is no corresponding PIB ID for any of the attributes,
    // so we will have to set the security attributes directly.

    // Given: there is one and only one security device table entry. There shouldn't be a need for more than one entry because there should only be one network.
    MAC_PIB().mac_device_table[0].pan_id = panID;
    MAC_PIB().mac_device_table[0].short_address = shortAddr;
    ZB_IEEE_ADDR_COPY(&MAC_PIB().mac_device_table[0].long_address, pExtAddr);
    MAC_PIB().mac_device_table[0].frame_counter = frameCounter;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Jdllc_addSecDevice: Adding security device to MAC device table with PAN ID %x, short address %x, and frame counter value %d",
               panID, shortAddr, frameCounter);
}

#endif /* ZB_MAC_SECURITY */

/******************************************************************************
 Local Functions
 *****************************************************************************/
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
    Jdllc_events |= event;

    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "Jdllc::setEvent: Setting event %x", event);

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
    Jdllc_events &= ~(event);

    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "Jdllc::clearEvent: Clearing event %x", event);

    // Exit critical section
    ZB_ENABLE_ALL_INTER();
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
    Log_printf(LogModule_Zigbee_App, Log_ERROR, "jdllc::allocateOutBuffer: Failed to allocate out buffer");
    zb_reset(ZB_UNUSED_PARAM);
    // This should never be reached
    return ZB_BUF_INVALID;
  }
  return buf;
}


/*!
 * @brief       Poll timeout handler function  .
 *
 * @param       a0 - ignored
 */
static void processPollTimeoutCallback(uintptr_t a0)
{
    (void)a0; /* Parameter is not used */

    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "Entered processPollTimeoutCallback");

    setEvent(JDLLC_POLL_EVT);

    // See if this wakes up the device so there's not 10 seconds between polls
    SemaphoreP_post(wakeSem);
}

/*!
 * @brief       Scan backoff timeout handler function  .
 *
 * @param       a0 - ignored
 */
static void processScanBackoffTimeoutCallback(uintptr_t a0)
{
    (void)a0; /* Parameter is not used */
    
    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "Entered processScanBackoffTimeoutCallback");

    setEvent(JDLLC_SCAN_BACKOFF);

    // See if this wakes up the device so there's not 10 seconds between scans
    SemaphoreP_post(wakeSem);
}


/*!
 * @brief       Update Jdllc state
 *
 * @param       state - new state
 */
static void updateState(Jdllc_states_t state)
{
    if (state != devInfoBlock.currentJdllcState)
    {
        devInfoBlock.prevJdllcState = devInfoBlock.currentJdllcState;
        devInfoBlock.currentJdllcState = state;

        if (state == Jdllc_states_orphan)
        {
            /* Stop the reading timer */
            if (UtilTimer_isActive(&readingClkStruct) == true)
            {
                UtilTimer_stop(&readingClkStruct);
            }
        }

        if(pJdllcCallbacksCopy && pJdllcCallbacksCopy->pStateChangeCb)
        {
            /* state change callback (all this does in 15.4 stack is call a function that prints the new status) */
            pJdllcCallbacksCopy->pStateChangeCb(devInfoBlock.currentJdllcState);
        }
    }

    if ((state == Jdllc_states_rejoined) || (state == Jdllc_states_joined))
    {
        uint_fast32_t joinTime = ClockP_getSystemTicks();
        Log_printf(LogModule_Zigbee_App, Log_INFO, "updateState: join time occurred at %d", joinTime);
        // Calculating rejoin delay in ms. Changing from how 15.4 stack does it in seconds (since their tick period is 10 microseconds).
        uint_fast32_t rejoinDelay = (joinTime - interimDelayTicks) / (ClockP_getSystemTickPeriod()) / 1000;
        if ((rejoinDelay > Sensor_msgStats.interimDelay) && (interimDelayTicks > 0))
        {
            Sensor_msgStats.interimDelay = (zb_uint16_t)rejoinDelay;
            interimDelayTicks = 0;
        }
    }
}

/*!
 * @brief       Process data confirm callback
 *
 * @param       pData - pointer to data confirm structure
 */
static void dataCnfCb(zb_mcps_data_confirm_params_t *pData)
{
    if (CONFIG_MAC_BEACON_ORDER == JDLLC_BEACON_ORDER_NON_BEACON)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "jdllc::dataCnfCb: Entering JDLLC callback function for data confirmation.");
        if (pData->status == MAC_NO_ACK)
        {
            /* track the number of failures  */
            devInfoBlock.dataFailures++;
            Log_printf(LogModule_Zigbee_App, Log_INFO, "jdllc::dataCnfCb: No ACK was received for the data request. Data failures: %d", devInfoBlock.dataFailures);
            if (devInfoBlock.dataFailures == CONFIG_MAX_DATA_FAILURES)
            {
                handleMaxDataFail();
            }
        }
        else if (pData->status == MAC_SUCCESS)
        {
            devInfoBlock.dataFailures = 0;
            if (devInfoBlock.currentJdllcState == Jdllc_states_initRestoring)
            {
                Log_printf(LogModule_Zigbee_App, Log_INFO, "dataCnfCb: First data confirmation after a rejoin.");
                zb_device_descriptor_t devInfo;
                Llc_netInfo_t parentNetInfo;
                populateInfo(&devInfo, &parentNetInfo);

                /* device joined */
                if (pJdllcCallbacksCopy && pJdllcCallbacksCopy->pJoinedCb)
                {
                    Log_printf(LogModule_Zigbee_App, Log_INFO, "dataCnfCb: Calling the original device joined callback function.");
                    pJdllcCallbacksCopy->pJoinedCb(&devInfo, &parentNetInfo);
                }

                updateState(Jdllc_states_rejoined);
            }
        }
    }

    if (macCallbacksCopy.pDataCnfCb != NULL)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "dataCnfCb: Calling the original MAC callback function for data confirmation.");
        macCallbacksCopy.pDataCnfCb(pData);
    }
}

/*!
 * @brief       Switch to the new  coordinator state and set the event bit.
 *
 * @param       newState - next state of coordinator
 */
static void switchState(Jdllc_device_states_t newState)
{
    devInfoBlock.currentDevState = newState;
    setEvent(JDLLC_STATE_CHANGE_EVT);
}

/*!
 * @brief       Function to transition various states involved with scan request
 *               and start request before the coordinator is started.
 *
 * @param       state - current startup state of coordinator
 */
static void processState(Jdllc_device_states_t state)
{
    switch(state)
    {
        case Jdllc_deviceStates_scanActive:
            /* Active scan */
            Log_printf(LogModule_Zigbee_App, Log_INFO, "processState: call sendScanReq with ACTIVE_SCAN");
            sendScanReq(ACTIVE_SCAN);
            break;

        case Jdllc_deviceStates_scanPassive:
            /* Passive scan */
            Log_printf(LogModule_Zigbee_App, Log_INFO, "processState: call sendScanReq with PASSIVE_SCAN");
            sendScanReq(PASSIVE_SCAN);
            break;

        // This case only seems to be for beacon enabled devices (BEACON_ORDER < 0xF).
        case Jdllc_deviceStates_syncReq:
            Log_printf(LogModule_Zigbee_App, Log_ERROR, "processState: Sync requests are not implemented yet!");
            while (1){}

            // ApiMac_mlmeSyncReq_t syncReq;
            // ApiMac_mlmeSetReqUint8(ApiMac_attribute_beaconOrder,
            //                        devInfoBlock.beaconOrder);
            // ApiMac_mlmeSetReqUint8(ApiMac_attribute_superframeOrder,
            //                        devInfoBlock.superframeOrder);
            // ApiMac_mlmeSetReqUint16(ApiMac_attribute_coordShortAddress,
            //                         devInfoBlock.coordShortAddr);
            // ApiMac_mlmeSetReqUint16(ApiMac_attribute_panId, devInfoBlock.panID);
            // /* Sync request for beacon enabled devices */
            // syncReq.logicalChannel = devInfoBlock.channel;
            // syncReq.channelPage = CONFIG_CHANNEL_PAGE;
            // syncReq.trackBeacon = true;
            // ApiMac_mlmeSyncReq(&syncReq);

            // /* Wait for Beacon Sync and verify that the joining device only
            //  * connects to a coordinator of the same PAN Id. */
            // if((devInfoBlock.currentJdllcState == Jdllc_states_joining)
            //     && panIdMatch)
            // {
            //     setEvent(JDLLC_ASSOCIATE_REQ_EVT);
            // }
            break;

        case Jdllc_deviceStates_scanOrphan:
            /* Orphan scan */
            Log_printf(LogModule_Zigbee_App, Log_INFO, "processState: call sendScanReq with ORPHAN_SCAN");
            sendScanReq(ORPHAN_SCAN);
            break;

        default:
            break;
    }
}

/*!
* @brief       Check if the incoming frame's beacon order matches the network
*              type of device
*
* @param       superframeSpec - super frame spec of incoming beacon
*
* @return      true if matches, else false
*/
static bool checkBeaconOrder(zb_uint16_t* superframeSpec)
{
    // If the coordinator won't transmit beacon frames except when requested to do so
    if(CONFIG_MAC_BEACON_ORDER == ZB_TURN_OFF_ORDER)
    {
        Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "checkBeaconOrder: beacon order from superframe: %d", ZB_MAC_GET_BEACON_ORDER(superframeSpec));
        if(ZB_MAC_GET_BEACON_ORDER(superframeSpec) == CONFIG_MAC_BEACON_ORDER)
        {
            return (true);
        }
        Log_printf(LogModule_Zigbee_App, Log_INFO, "checkBeaconOrder: no matching beacon");
    }
    else if((ZB_MAC_GET_BEACON_ORDER(superframeSpec) <= CONFIG_MAC_BEACON_ORDER))
    {
        return (true);
    }
    return (false);
}

/*!
 * @brief       Process  Beacon Notification callback.
 *
 * @param       pData - pointer MAC Beacon indication info
 */
static void beaconNotifyIndCb(zb_mac_beacon_notify_indication_t *pData)
{
    Log_printf(LogModule_Zigbee_App, Log_INFO, "beaconNotifyIndCb: Beacon received from PAN %x, channel %d, rssi %d",
                pData->pan_descriptor.coord_pan_id,
                pData->pan_descriptor.logical_channel,
                pData->rssi);
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "beaconNotifyIndCb: Beacon type: %d", pData->beacon_type);
    // Check beacon type
    if (pData->beacon_type == ZB_MAC_BEACON_TYPE_BEACON)
    {
        if (parentFound == false)
        {
            // Need to copy the superframe information from a bitfield to an integer.
            zb_super_frame_spec_t temp = pData->pan_descriptor.super_frame_spec;
            zb_uint16_t* super_frame_spec = (zb_uint16_t*) &temp;
            Log_printf(LogModule_Zigbee_App, Log_INFO, "beaconNotifyIndCb: Superframe has been converted to integer with value %x", *super_frame_spec);
            // Check if association bit permit is set (i.e. the coordinator that sent the beacon is allowing association requests on its network).
            if (ZB_MAC_GET_ASSOCIATION_PERMIT(super_frame_spec) == 1)
            {
                // Check for beacon order match
                if (checkBeaconOrder(super_frame_spec))
                {
                    // Device can join any network, associate with first coordinator from which beacon is received.
                    if (devInfoBlock.panID == JDLLC_INVALID_PAN)
                    {
                        Log_printf(LogModule_Zigbee_App, Log_INFO, "beaconNotifyIndCb: Sensor has received beacon from coordinator with PAN ID %x", pData->pan_descriptor.coord_pan_id);
                        devInfoBlock.panID = pData->pan_descriptor.coord_pan_id;
                        panIdMatch = true;
                    }
                    // Check the incoming PAN ID to see if it's a valid coordinator.
                    else if (devInfoBlock.panID == pData->pan_descriptor.coord_pan_id)
                    {
                        panIdMatch = true;
                        numSyncLoss = 0;
                        {
                            // Set auto request, meaning that the joining device should automatically send a data request command if its address is listed in the beacon frame.
                            zb_bufid_t buf = allocateOutBuffer();
                            Log_printf(LogModule_Zigbee_App, Log_INFO, "beaconNotifyIndCb: turning on auto request with buffer ID %d", buf);
                            zb_mlme_set_request_t *set_req;
                            set_req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_set_request_t) + sizeof(zb_uint8_t));
                            ZB_BZERO(set_req, sizeof(zb_mlme_set_request_t));
                            set_req -> pib_attr   = ZB_PIB_ATTRIBUTE_AUTO_REQUEST;
                            set_req -> pib_length = sizeof(zb_uint8_t);
                            *((zb_uint8_t *)(set_req + 1)) = ZB_TRUE;
                            ZB_SCHEDULE_CALLBACK(zb_mlme_set_request, buf);
                        }
                    }
                    devInfoBlock.channel = pData->pan_descriptor.logical_channel;
                    devInfoBlock.coordShortAddr = pData->pan_descriptor.coord_address.addr_short;
                    Log_printf(LogModule_Zigbee_App, Log_INFO, "beaconNotifyIndCb: Found coordinator on channel %d with short address %x", pData->pan_descriptor.logical_channel, pData->pan_descriptor.coord_address.addr_short);
                    if (ZB_MAC_GET_BEACON_ORDER(super_frame_spec) != ZB_TURN_OFF_ORDER)
                    {
                        // Set beacon order and superframe order
                        devInfoBlock.beaconOrder = ZB_MAC_GET_BEACON_ORDER(super_frame_spec);
                        devInfoBlock.superframeOrder = ZB_MAC_GET_SUPERFRAME_ORDER(super_frame_spec);
                        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "beaconNotifyIndCb: Beacon order %d, superframe order %d",
                                devInfoBlock.beaconOrder,
                                devInfoBlock.superframeOrder);
                    }
                    if (devInfoBlock.beaconOrder == ZB_TURN_OFF_ORDER)
                    {
                        parentFound = true;

                        // Stop scanning when parent is found (but if association with this parent fails, this will start again)
                        if (UtilTimer_isActive(&scanBackoffClkStruct))
                        {
                            UtilTimer_stop(&scanBackoffClkStruct);
                        }
                    }
                }
            }
        }
    }
    // Callback to original MAC callback API
    if (macCallbacksCopy.pBeaconNotifyIndCb != NULL)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "beaconNotifyIndCb: Calling the original MAC callback function for beacon notify indication.");
        macCallbacksCopy.pBeaconNotifyIndCb(pData);
    }
}

/*!
 * @brief       Process  scan confirm callback.
 *
 * @param       pData - pointer to Scan Confirm
 */
static void scanCnfCb(zb_mac_scan_confirm_t *pData)
{
    Log_printf(LogModule_Zigbee_App, Log_INFO, "scanCnfCb: Scan confirm with status %x",
             pData->status);

    if (pData->status == MAC_SUCCESS)
    {
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "scanCnfCb: Successful scan with scan type %d", pData->scan_type);
        // Device is sending beacon requests while scanning for a beacon response
        if (pData->scan_type == ACTIVE_SCAN)
        {
            // Only send association requests for a matching PAN
            if (panIdMatch)
            {
                // Set event to send association request
                Log_printf(LogModule_Zigbee_App, Log_INFO, "scanCnfCb: Active scan - there was a beacon received, so send an association to that beacon.");
                setEvent(JDLLC_ASSOCIATE_REQ_EVT);
            }
            // The current scan didn't return a matching beacon
            else
            {
                Log_printf(LogModule_Zigbee_App, Log_INFO, "scanCnfCb: Active scan - the current scan didn't return a beacon from the scanned channel(s).");

                // This state should be the joining state since it's an active scan that didn't result
                // in finding a beacon allowing this device to join its network.
                devInfoBlock.prevDevState = devInfoBlock.currentDevState;
               
                // Schedule next scan by setting scan backoff clock (Ssf_setScanBackoffClock in ssf.c)
                setScanBackoffClock(CONFIG_SCAN_BACKOFF_INTERVAL);
            }
        }
        // Device is scanning for a beacon response without sending any requests
        else if (pData->scan_type == PASSIVE_SCAN)
        {
            if (panIdMatch)
            {
                Log_printf(LogModule_Zigbee_App, Log_ERROR, "scanCnfCb: Sync requests are not implemented yet!");
                while (1){}
            }
            else
            {
                Log_printf(LogModule_Zigbee_App, Log_INFO, "scanCnfCb: Passive scan - the current scan didn't return a beacon from the scanned channel(s).");

                // Schedule next scan
                devInfoBlock.prevDevState = devInfoBlock.currentDevState;
               
                // Set scan backoff clock (Ssf_setScanBackoffClock in ssf.c)
                setScanBackoffClock(CONFIG_SCAN_BACKOFF_INTERVAL);
            }
        }
        // Device is trying to find its coordinator
        else if (pData->scan_type == ORPHAN_SCAN)
        {
            numSyncLoss = 0;
            Log_printf(LogModule_Zigbee_App, Log_INFO, "scanCnfCb: Orphan scan - cooordinator realignment received");
            // Coordinator realignment received, set event to process it
            setEvent(JDLLC_COORD_REALIGN);
        }
    }
    // Scan failed
    else
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "scanCnfCb: Scan failed");
        // Orphan failed to find a coordinator (there was not a beacon returned). Start scanning for one every CONFIG_ORPHAN_BACKOFF_INTERVAL ms.
        if (pData->scan_type == ORPHAN_SCAN && pData->status == MAC_NO_BEACON)
        {
            Log_printf(LogModule_Zigbee_App, Log_DEBUG, "scanCnfCb: Orphan scan failed");
            // Sets the previous state to the orphan state. During an orphan scan, the device state will be set to orphan.
            devInfoBlock.prevDevState = devInfoBlock.currentDevState;

            // Set the scan backoff clock (Ssf_setScanBackoffClock in ssf.c)
            setScanBackoffClock(CONFIG_ORPHAN_BACKOFF_INTERVAL);
        }
        else if (pData->scan_type == ACTIVE_SCAN)
        {
            // Reset previously found PAN ID match state
            panIdMatch = false;

            // This state should be set to the joining state since it's an active scan, which occurs when the device
            // has not found a network to join up to by this point.
            devInfoBlock.prevDevState = devInfoBlock.currentDevState;
            // Set the scan backoff clock (Ssf_setScanBackoffClock in ssf.c)
            setScanBackoffClock(CONFIG_SCAN_BACKOFF_INTERVAL);
        }
        else
        {
            if ((CONFIG_MAC_BEACON_ORDER > 0) && (CONFIG_MAC_BEACON_ORDER < JDLLC_BEACON_ORDER_NON_BEACON))
            {
                /* Reset previously found PAN ID match state */
                panIdMatch = false;

                devInfoBlock.prevDevState = devInfoBlock.currentDevState;
                // Set the scan backoff clock (Ssf_setScanBackoffClock in ssf.c)
                setScanBackoffClock(CONFIG_SCAN_BACKOFF_INTERVAL);
            }
        }
    }

    if (macCallbacksCopy.pScanCnfCb != NULL)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "scanCnfCb: Calling the original MAC callback function for scan confirmation.");
        macCallbacksCopy.pScanCnfCb(pData);
    }
}

/*!
 * @brief       Moving logic for populating devInfo and parentInfo structures and then subsequently calling pJoinedCb function
 *              (see assocCnfCb in 15.4 stack code) into its own function because the PIB attributes for the device and coordinator's
 *              extended address aren't actually copied over to devInfoBlock until after the association confirm callback finishes
 *              running and the getPibAttrCb function is called via the scheduler.
 *
 */
static void handleSuccessfulJoin()
{
    // Given: There was a successful join from the sensor to a coordinator/collector. The PIB attributes for the device's extended address
    // and the coordinator's extended address have been copied to the devInfoBlock structure. Same with the PAN ID, channel, and coordinator's
    // short address, all of which were previously set before the association confirmation. The device's short address to be used in the coordinator's
    // network should also have been set in the association confirmation callback. This function is only called when the event is JDLLC_JOIN_EVT, which
    // happens in getPibAttrCb.

    Llc_netInfo_t parentInfo;
    zb_device_descriptor_t devInfo;

    parentInfo.devInfo.panID = devInfoBlock.panID;
    devInfo.shortAddress = devInfoBlock.devShortAddr;

    Log_printf(LogModule_Zigbee_App, Log_INFO, "handleSuccessfulJoin: Copying device's extended address from devInfoBlock to devInfo");
    // Put in an explicit cast to ensure the compiler that the volatile qualifier is being removed.
    ZB_IEEE_ADDR_COPY(&devInfo.extAddress, (zb_ieee_addr_t*) &devInfoBlock.devExtAddr);

    devInfo.panID = devInfoBlock.panID;

    Log_printf(LogModule_Zigbee_App, Log_INFO, "handleSuccessfulJoin: Copying cordinator's extended address from devInfoBlock to parentInfo");
    // Put in an explicit cast to ensure the compiler that the volatile qualifier is being removed.
    ZB_IEEE_ADDR_COPY(&parentInfo.devInfo.extAddress, (zb_ieee_addr_t*) &devInfoBlock.coordExtAddr);

    // Set the parent info's short address field based on devInfoBlock
    parentInfo.devInfo.shortAddress = devInfoBlock.coordShortAddr;

    parentInfo.channel = devInfoBlock.channel;

    // Device has joined, so call the appropriate device joined callback function in the application layer.
    if (pJdllcCallbacksCopy && pJdllcCallbacksCopy->pJoinedCb)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "handleSuccessfulJoin: Calling the original device joined callback function.");
        pJdllcCallbacksCopy->pJoinedCb(&devInfo, &parentInfo);
    }
    updateState(Jdllc_states_joined);

    // For sleepy devices
    if ((!CONFIG_RX_ON_IDLE))
    {
        if ((devInfoBlock.currentJdllcState == Jdllc_states_joined) || (devInfoBlock.currentJdllcState == Jdllc_states_rejoined))
        {
            setEvent(JDLLC_POLL_EVT);
        }
    }
}

/*!
 * @brief       Helper function for associate confirm callback. Used to get the device and coordinator's extended address from MAC PIB.
 *
 * @param       param - buffer ID containing MLME-GET.confirm structure containing the PIB attribute
 */
static void getPibAttrCb(zb_uint8_t param)
{
    zb_mlme_get_confirm_t* conf = (zb_mlme_get_confirm_t*)zb_buf_begin(param);
    Log_printf(LogModule_Zigbee_App, Log_INFO, "getPibAttrCb: Retrieved PIB attribute %x with status %x", conf->pib_attr, conf->status);
    if (conf->pib_attr == ZB_PIB_ATTRIBUTE_EXTEND_ADDRESS)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "getPibAttrCb: Got the device's extended address and copied it to devInfoBlock");
        // Inspired by zb_mlme_get_request() in mac_pib.c where the extended address is retrieved. Note that the attribute value
        // is stored just after the confirm structure. Put in an explicit cast to ensure the compiler that the volatile qualifier is being removed.
        ZB_IEEE_ADDR_COPY((zb_ieee_addr_t*) &devInfoBlock.devExtAddr, (zb_uint8_t*)(conf + 1));
    }
    else if (conf->pib_attr == ZB_PIB_ATTRIBUTE_COORD_EXTEND_ADDRESS)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "getPibAttrCb: Got the coordinator's extended address and copied it to devInfoBlock");
        // Inspired by zb_mlme_get_request() in mac_pib.c where the extended address is retrieved. Note that the attribute value
        // is stored just after the confirm structure. Put in an explicit cast to ensure the compiler that the volatile qualifier is being removed.
        ZB_IEEE_ADDR_COPY((zb_ieee_addr_t*) &devInfoBlock.coordExtAddr, (zb_uint8_t*)(conf + 1));
    }

    // Set the device to a join event once both the device and coordinator's extended address have been retrieved from the PIB attributes
    // and have been copied over to the devInfoBlock. This confirms that the association was successful, and now we can handle a
    // successful join.

    // Checking that the addresses have been set (i.e. it's no longer a default address of all 0s)
    if (!ZB_IEEE_ADDR_CMP((zb_ieee_addr_t*) &devInfoBlock.devExtAddr, &defaultExtAddr) && !ZB_IEEE_ADDR_CMP((zb_ieee_addr_t*) &devInfoBlock.coordExtAddr, &defaultExtAddr))
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "getPibAttrCb: Both the device and parrent's extended address have been set in devInfoBlock, so handle the successful join");
        // The association was successful, so handle the successful join
        setEvent(JDLLC_JOIN_EVT);
    }
    zb_buf_free(param);
}

/*!
 * @brief       Handle Jdllc callback for assoc Confirm
 *
 * @param       pData - pointer to Associate Confirm structure
 */
static void assocCnfCb(zb_mlme_associate_confirm_t *pData)
{   
    // Valid status values of association confirm: success, PAN at capacity, PAN access denied, hopping sequence offset duplication, fast association successful
    // Table 7-54 in IEEE specs
    if (pData->status == MAC_SUCCESS)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "assocCnfCb: Successful association with parent");

        // This is the short address that the coordinator/collector assigns to the sensor device.
        devInfoBlock.devShortAddr = pData->assoc_short_address;

        // Get the device IEEE address PIB and put it into the devInfoBlock (previously set during the sensor's initialization process)
        {
            zb_bufid_t buf = allocateOutBuffer();
            Log_printf(LogModule_Zigbee_App, Log_INFO, "assocCnfCb: getting PIB attribute for device's IEEE address with buffer ID %d", buf);
            zb_mlme_get_request_t *req;

            req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_get_request_t));
            req->pib_attr   = ZB_PIB_ATTRIBUTE_EXTEND_ADDRESS;
            req->iface_id  = ZB_PIBCACHE_PRIMARY_IFACE();
            req->confirm_cb_u.cb = getPibAttrCb;

            zb_mlme_get_request(buf);
        }

        // Set device short address PIB
        {
            zb_bufid_t buf = allocateOutBuffer();
            Log_printf(LogModule_Zigbee_App, Log_INFO, "assocCnfCb: setting short address PIB attribute with buffer ID %d", buf);
            zb_mlme_set_request_t *set_req;
            set_req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_set_request_t) + sizeof(zb_uint16_t));
            ZB_BZERO(set_req, sizeof(zb_mlme_set_request_t));
            set_req -> pib_attr   = ZB_PIB_ATTRIBUTE_SHORT_ADDRESS;
            set_req -> pib_length = sizeof(zb_uint16_t);
            // Use ZB_MEMCPY to avoid unaligned access (do not use *((zb_uint16_t *)(set_req + 1)) = pData->assoc_short_address)
            zb_uint8_t *aligned_address = (zb_uint8_t *)set_req + sizeof(zb_mlme_set_request_t);
            ZB_MEMCPY(aligned_address, &pData->assoc_short_address, sizeof(zb_uint16_t));
            ZB_SCHEDULE_CALLBACK(zb_mlme_set_request, buf);
        }

        // Get coordinator's extended address PIB and put it into the devInfoBlock, then copy that into the parent information structure.
        // Unsure where this attribute is being set, maybe externally somehow, but verified via memory browser that the PIB attribute
        // is in fact being set.
        {
            zb_bufid_t buf = allocateOutBuffer();
            Log_printf(LogModule_Zigbee_App, Log_INFO, "assocCnfCb: getting PIB attribute for coordinator's IEEE address with buffer ID %d", buf);
            zb_mlme_get_request_t *req;

            req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_get_request_t));
            req->pib_attr   = ZB_PIB_ATTRIBUTE_COORD_EXTEND_ADDRESS;
            req->iface_id  = ZB_PIBCACHE_PRIMARY_IFACE();
            req->confirm_cb_u.cb = getPibAttrCb;

            zb_mlme_get_request(buf);
        }

        // Stop scan backoff timer because we are able to associate with the parent that sent the beacon
        UtilTimer_stop(&scanBackoffClkStruct);
    }
    else if (pData->status == MAC_PAN_AT_CAPACITY || pData->status == MAC_PAN_ACCESS_DENIED)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "assocCnfCb: Association failed with status %x", pData->status);
        // Stop trying to join the parent because its network is either at full capacity or denying you access to join.
        switchState(Jdllc_deviceStates_accessDenied);
        updateState(Jdllc_states_accessDenied);
        // Update the number of join failures (in stats)
        Sensor_msgStats.joinFails++;
    }
    else
    {
        // Could not associate with parent that sent the beacon, so scan for another parent
        parentFound = false;
        Log_printf(LogModule_Zigbee_App, Log_INFO, "assocCnfCb: Could not associate with parent that sent the beacon, so scan for another parent");
        if (CONFIG_MAC_BEACON_ORDER == JDLLC_BEACON_ORDER_NON_BEACON)
        {
            /* Non-beacon network */
            switchState(Jdllc_deviceStates_scanActive);
            setScanBackoffClock(CONFIG_SCAN_BACKOFF_INTERVAL);
        }
        else if ((CONFIG_MAC_BEACON_ORDER > 0) && (CONFIG_MAC_BEACON_ORDER < JDLLC_BEACON_ORDER_NON_BEACON))
        {
            /* Beacon network */
            if (devInfoBlock.currentDevState != Jdllc_deviceStates_scanPassive)
            {
                switchState(Jdllc_deviceStates_scanPassive);
            }
        }

        // Update the number of join failures (in stats)
        Sensor_msgStats.joinFails++;
    }

    if (macCallbacksCopy.pAssocCnfCb != NULL)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "assocCnfCb: Calling the original MAC callback function for association confirmation.");
        macCallbacksCopy.pAssocCnfCb(pData);
    }
}

/*!
 * @brief       Handle Jdllc callback for assoc Confirm
 *
 * @param       pData - pointer to Associate Confirm structure
 */
static void disassoCnfCb(zb_mac_disassociate_confirm_t *pData)
{
    if (pData->status == MAC_SUCCESS)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "disassoCnfCb: Successful disassociation from parent. Resetting devInfoBlock to all of its default values.");
        // Stop polling
        if (!CONFIG_RX_ON_IDLE)
        {
            UtilTimer_stop(&pollClkStruct);
        }
        // Enable looking for a new parent
        parentFound = ZB_FALSE;

        // Set devInfoBlock back to defaults
        devInfoBlock.panID = CONFIG_PAN_ID;
        devInfoBlock.channel = JDLLC_INVALID_CHANNEL;
        devInfoBlock.coordShortAddr = 0xFFFF;
        ZB_MEMCPY((zb_ieee_addr_t*) &devInfoBlock.coordExtAddr[0], 0x00, sizeof(zb_ieee_addr_t));
        devInfoBlock.devShortAddr = 0xFFFF;
        ZB_MEMCPY((zb_ieee_addr_t*) &devInfoBlock.devExtAddr[0], 0x00, sizeof(zb_ieee_addr_t));
        devInfoBlock.beaconOrder = CONFIG_MAC_BEACON_ORDER;
        devInfoBlock.superframeOrder = CONFIG_MAC_SUPERFRAME_ORDER;
        devInfoBlock.dataFailures = 0;
        devInfoBlock.pollInterval = CONFIG_POLLING_INTERVAL;

#ifndef USE_DMM
        // Turn off the red LED to signify that the sensor is no longer connected to the collector.
        zb_osif_led_off(1);
#endif
        // Change the state back to initWaiting
        updateState(Jdllc_states_initWaiting);
    }

    if (pJdllcCallbacksCopy && pJdllcCallbacksCopy->pDisassocCnfCb)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "disassoCnfCb: Calling the original disassociation confirmation function.");
        // In our version of this implementation, these arguments serve no purpose because we handle security differently and don't need to use the
        // collector's extended address. Additionally, the 15.4 stack never used the disassociation reason argument in its version of the disassociation
        // confirmation callback in the sensor application layer, so we will just pass in ZB_UNUSED_PARAM.
        pJdllcCallbacksCopy->pDisassocCnfCb(&defaultExtAddr, ZB_UNUSED_PARAM);
    }

    if (macCallbacksCopy.pDisassociateCnfCb != NULL)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "disassoCnfCb: Calling the original MAC callback function for disassociation confirmation.");
        macCallbacksCopy.pDisassociateCnfCb(pData);
    }
}

/*!
 * @brief       Process poll confirm callback
 *
 * @param       status - the status of the corresponding data request
 */
static void pollCnfCb(zb_ret_t status)
{
    // Note that the status is zb_ret_t (zb_uint32_t) according to the ZBOSS specs, but it's supposed to be zb_mac_status_t (zb_uint8_t).
    // Need to cast to zb_uint8_t to avoid compiler warnings.
    zb_mac_status_t macStatus = (zb_mac_status_t) status;

    Log_printf(LogModule_Zigbee_App, Log_INFO, "pollCnfCb: Poll confirmation with status %x", macStatus);

    // If there the poll request did not return any data (because the frame pending field of the ACK frame is set to zero) or the poll was successful
    if (macStatus == MAC_NO_DATA || macStatus == MAC_SUCCESS)
    {
        // This is the first poll confirmation after the state was switched to initRestoring from Jdllc_rejoin
        if (devInfoBlock.currentJdllcState == Jdllc_states_initRestoring)
        {
            Log_printf(LogModule_Zigbee_App, Log_INFO, "pollCnfCb: Restoring end device and parent information");
            // Create devInfo and parentNetInfo structures, and then call populateInfo function to populate those structures
            zb_device_descriptor_t devInfo;
            Llc_netInfo_t parentNetInfo;
            populateInfo(&devInfo, &parentNetInfo);

            // Call the pJoinedCb with devInfo and parentNetInfo
            if (pJdllcCallbacksCopy && pJdllcCallbacksCopy->pJoinedCb)
            {
                Log_printf(LogModule_Zigbee_App, Log_INFO, "pollCnfCb: Calling the original device joined callback function.");
                pJdllcCallbacksCopy->pJoinedCb(&devInfo, &parentNetInfo);
            }

            // Update state to rejoined state
            updateState(Jdllc_states_rejoined);

            // If the device is a sleepy device, then set event for polling
            if (!CONFIG_RX_ON_IDLE)
            {
                setEvent(JDLLC_POLL_EVT);
            }
        }
        // Set the devInfoBlock's number of data failures to 0 (happens after any case, even if we're not restoring)
        devInfoBlock.dataFailures = 0;
    }

    // If there was no ACK received (note that MAC_CHANNEL_ACCESS_FAILURES are currently treated as MAC_NO_ACKs, so if the channel is busy, then it will
    // be treated like a MAC_NO_ACK where if there's CONFIG_MAX_DATA_FAILURES in a row, then handleMaxDataFail() will be called if the beacon order is
    // non-beacon, which will in turn make the device start sending orphan scans periodically)
    else if (macStatus == MAC_NO_ACK)
    {
        // Increment the number of data failures
        devInfoBlock.dataFailures++;

        Log_printf(LogModule_Zigbee_App, Log_INFO, "pollCnfCb: No ACK was received for the poll request. Data failures: %d", devInfoBlock.dataFailures);

        // If the current JDLLC state is joined or rejoined, then set the poll clock to the retry poll interval
        if ((devInfoBlock.currentJdllcState == Jdllc_states_joined) || (devInfoBlock.currentJdllcState == Jdllc_states_rejoined))
        {
            setPollClock(JDLLC_RETRY_POLL);
        }

        // If the number of data failures is equal to the maximum number of data failures, then if the beacon order is non-beacon,
        // then call handleMaxDataFail().
        if (devInfoBlock.dataFailures == CONFIG_MAX_DATA_FAILURES)
        {
            Log_printf(LogModule_Zigbee_App, Log_INFO, "pollCnfCb: Reached the maximum number of data failures");
            if (CONFIG_MAC_BEACON_ORDER == JDLLC_BEACON_ORDER_NON_BEACON)
            {
                handleMaxDataFail();
            }
        }
        // Otherwise, if the current device has rejoined the network but didn't get an ACK
        // from this poll, then set the event for polling if the device is sleepy
        else
        {
            if (devInfoBlock.currentJdllcState == Jdllc_states_initRestoring)
            {
                if (!CONFIG_RX_ON_IDLE)
                {
                    Log_printf(LogModule_Zigbee_App, Log_INFO, "pollCnfCb: Sleepy ZED device has been restored, set event to poll.");
                    setEvent(JDLLC_POLL_EVT);
                }
            }
        }
    }

    // If there was a channel access failure (from CSMA-CA failure)
    else if (macStatus == MAC_CHANNEL_ACCESS_FAILURE)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "pollCnfCb: Poll request was unsuccessful because the channel is busy");

        // Set the poll clock to the retry poll interval (shorter interval since network is busy)
        setPollClock(JDLLC_RETRY_POLL);
    }

    if (macCallbacksCopy.pPollCnfCb != NULL)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "pollCnfCb: Calling the original MAC callback function for poll confirmation.");
        // Will need to cast this to a zb_mac_status_t type as well in the upper layer.
        macCallbacksCopy.pPollCnfCb(status);
    }
}

/*!
 * @brief       Send Poll request
 */
static void sendPollReq()
{
    zb_bufid_t buf = allocateOutBuffer();
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sendPollReq: sending poll request with buffer ID %d", buf);
    // Important to use ZB_BUF_GET_PARAM to set the payload from the tail and not use zb_buf_initial_alloc to set from the head.
    // Otherwise, the header and payload will be messed up and display the wrong information.
    zb_mlme_poll_request_t* pollReq = ZB_BUF_GET_PARAM(buf, zb_mlme_poll_request_t);
    ZB_BZERO(pollReq, sizeof(zb_mlme_poll_request_t));
    pollReq->coord_pan_id = devInfoBlock.panID;
    pollReq->coord_addr_mode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;
    pollReq->coord_addr.addr_short = devInfoBlock.coordShortAddr;
    pollReq->poll_rate = CONFIG_POLLING_INTERVAL;
    ZB_SCHEDULE_CALLBACK(zb_mlme_poll_request, buf);
}


/*!
 * @brief       Process poll event
 *
 * @param       param - buffer ID containing the MLME-GET.confirm payload for the PAN ID 
 */
static void processPollEvt(zb_uint8_t param)
{
    zb_mlme_get_confirm_t* conf = (zb_mlme_get_confirm_t*)zb_buf_begin(param);
    zb_uint8_t autoRequest = *((zb_uint8_t*)(conf + 1));
    Log_printf(LogModule_Zigbee_App, Log_INFO, "processPollEvt: Received auto request PIB from buffer ID %d with value %d", param, autoRequest);

    if( (CONFIG_MAC_BEACON_ORDER == JDLLC_BEACON_ORDER_NON_BEACON) || (autoRequest == ZB_FALSE) )
    {
        if ((devInfoBlock.currentJdllcState == Jdllc_states_joined) || (devInfoBlock.currentJdllcState == Jdllc_states_rejoined))
        {
            /* set poll timer */
            setPollClock(CONFIG_POLLING_INTERVAL);
        }

        /* send poll request */
        sendPollReq();
    }

    zb_buf_free(param);
}

/*!
 * @brief       Process coordinator realignment
 *
 * @param       param - buffer ID containing the MLME-GET.confirm payload for the PAN ID 
 */
static void processCoordRealign(zb_uint8_t param)
{
    zb_mlme_get_confirm_t* conf = (zb_mlme_get_confirm_t*)zb_buf_begin(param);
    zb_uint16_t panID = *((zb_uint16_t*)(conf + 1));
    Log_printf(LogModule_Zigbee_App, Log_INFO, "processCoordRealign: Received PAN ID PIB from buffer ID %d with value %x", param, panID);

    // Able to rejoin the network that the device was previously associated with. Transition to the correct non-orphan state.
    if (panID == devInfoBlock.panID)
    {
        if (CONFIG_MAC_BEACON_ORDER != JDLLC_BEACON_ORDER_NON_BEACON)
        {
            Log_printf(LogModule_Zigbee_App, Log_ERROR, "processCoordRealign: Sync requests are not implemented yet!");
            while (1){}
        }

        // Transition to the correct non-orphan state

        // Device has previously joined the network, but then polling failed the maximum number of times, switching its state to orphan.
        if (devInfoBlock.prevJdllcState == Jdllc_states_joined && devInfoBlock.currentJdllcState == Jdllc_states_orphan)
        {
            Log_printf(LogModule_Zigbee_App, Log_INFO, "processCoordRealign: Device is rejoining network. Previous state is joined, current state is orphan");
            updateState(Jdllc_states_rejoined);
        }
        // Previously joined but not orphan scan
        else if (devInfoBlock.prevJdllcState == Jdllc_states_joined)
        {
            Log_printf(LogModule_Zigbee_App, Log_INFO, "processCoordRealign: Device is joining network. Previous state is joined, current state is %d", devInfoBlock.currentJdllcState);
            updateState(Jdllc_states_joined);
        }
        // Device has previously rejoined the network, but then the polling failed the maximum number of times, switching its state to orphan.
        else if (devInfoBlock.prevJdllcState == Jdllc_states_rejoined)
        {
            Log_printf(LogModule_Zigbee_App, Log_INFO, "processCoordRealign: Device is rejoining network. Previous state is rejoined, current state is %d", devInfoBlock.currentJdllcState);
            updateState(Jdllc_states_rejoined);
        }
        // Device has previously been restored in the network
        else if (devInfoBlock.prevJdllcState == Jdllc_states_initRestoring)
        {
            Log_printf(LogModule_Zigbee_App, Log_INFO, "processCoordRealign: Device is rejoining network. Previous state is initRestoring, current state is %d", devInfoBlock.currentJdllcState);
            zb_device_descriptor_t devInfo;
            Llc_netInfo_t parentNetInfo;
            populateInfo(&devInfo, &parentNetInfo);

            if (pJdllcCallbacksCopy && pJdllcCallbacksCopy->pJoinedCb)
            {
                Log_printf(LogModule_Zigbee_App, Log_INFO, "processCoordRealign: Calling the original device joined callback function.");
                pJdllcCallbacksCopy->pJoinedCb(&devInfo, &parentNetInfo);
            }
            updateState(Jdllc_states_rejoined);
        }

        /* if sleepy device before setting poll timer */
        if ((!CONFIG_RX_ON_IDLE))
        {
            /* start polling if parent matches*/
            setPollClock(devInfoBlock.pollInterval);
        }

        {
            zb_bufid_t buf = allocateOutBuffer();
            Log_printf(LogModule_Zigbee_App, Log_INFO, "processCoordRealign: turning off RX with buffer ID %d", buf);
            zb_mlme_set_request_t *set_req;
            set_req = zb_buf_initial_alloc(buf, sizeof(zb_mlme_set_request_t) + sizeof(zb_uint8_t));
            ZB_BZERO(set_req, sizeof(zb_mlme_set_request_t));
            set_req -> pib_attr   = ZB_PIB_ATTRIBUTE_RX_ON_WHEN_IDLE;
            set_req -> pib_length = sizeof(zb_uint8_t);
            *((zb_uint8_t *)(set_req + 1)) = CONFIG_RX_ON_IDLE;
            ZB_SCHEDULE_CALLBACK(zb_mlme_set_request, buf);
        }
        /* restart sensor reading timer */
        {
            zb_uint32_t randomNum;
            // 15.4 stack calls the RNG three times. Their RNG returns a uint8_t. One gets shifted left 16 bits, the second gets shifted left 8 bits,
            // and the third is added to the first two. Total is a 24 bit number. We can do this by calling ZB_RANDOM_U8()
            randomNum = ((ZB_RANDOM_U8() << 16) +
                        (ZB_RANDOM_U8() << 8) + ZB_RANDOM_U8());
            randomNum = (randomNum % JDLLC_RESTART_REPORTING_INTERVAL) + SENSOR_MIN_POLL_TIME;
            Log_printf(LogModule_Zigbee_App, Log_INFO, "processCoordRealign: setting reporting interval to %d ms", randomNum);
            setReadingClock(randomNum);
        }
        UtilTimer_stop(&scanBackoffClkStruct);
    }
    else
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "processCoordRealign: Orphan scan failed");

        // Orphan scan
        switchState(Jdllc_deviceStates_scanOrphan);
        updateState(Jdllc_states_orphan);
    }

    zb_buf_free(param);
}

/*!
 * @brief       Send Association request
 */
static void sendAssocReq(void)
{
    // Set the association request's channel and short address from the data in the device info block, which was set in the beacon indication callback.
    // Address type will be short address, see logs from F2 collector (15.4 stack sets it to this as well)

    // Issue: memcpy is incompatible with volatile objects. Create a copy of the coordinator's short address to handle this.
    zb_uint16_t coordShortAddr = devInfoBlock.coordShortAddr;

    zb_bufid_t buf = allocateOutBuffer();
    ZB_MLME_BUILD_ASSOCIATE_REQUEST(buf,
        CONFIG_CHANNEL_PAGE,
        devInfoBlock.channel,
        devInfoBlock.panID,
        ZB_ADDR_16BIT_DEV_OR_BROADCAST,
        &coordShortAddr, /* Because we are copying to the address of the short address argument in the request. */
        CAP_INFO);
    
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sendAssocReq: Sending association request with buffer ID %d", buf);
    
    ZB_SCHEDULE_CALLBACK(zb_mlme_associate_request, buf);

    // Increment the number of join attempts in the stats struct
    Sensor_msgStats.joinAttempts++;
}

/*!
 * @brief       Send scan request (beacon request)
 *
 * @param       scan_type - type of scan: active, passive or orphan (see mac_scan_type section in zb_mac.h)
 */
static void sendScanReq(zb_uint8_t scan_type)
{
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sendScanReq: Starting a scan");

    zb_uint8_t scan_duration;
    if (CONFIG_MAC_BEACON_ORDER == JDLLC_BEACON_ORDER_NON_BEACON)
    {
        // Default: 5
        // Setting scan duration to 5 since it's being done that way in some test cases, and because that's the deafult value of 15.4 stack
        scan_duration = CONFIG_SCAN_DURATION;
    }
    else
    {
        scan_duration = CONFIG_MAC_BEACON_ORDER;
    }

    zb_bufid_t buf = allocateOutBuffer();
    Log_printf(LogModule_Zigbee_App, Log_INFO, "sendScanReq: Scheduling callback to scan request with buffer ID %d and scan type %d", buf, scan_type);
    ZB_MLME_BUILD_SCAN_REQUEST(buf, CONFIG_CHANNEL_PAGE, defaultChannelMask, scan_type, scan_duration, ZB_PIBCACHE_PRIMARY_IFACE());
    ZB_SCHEDULE_CALLBACK(zb_mlme_scan_request, buf);
}

/*!
 * @brief       Populate information for end device and its parent
 *
 * @param       pDevInfo - pointer to device descriptor structure
 *
 * @param       pParentNetInfo - pointer to network information structure
 */
static void populateInfo(zb_device_descriptor_t *pDevInfo,
                         Llc_netInfo_t *pParentNetInfo)
{
    Log_printf(LogModule_Zigbee_App, Log_INFO, "populateInfo: Populating device and parent information");
    ZB_IEEE_ADDR_COPY((zb_ieee_addr_t*) &devInfoBlock.devExtAddr, &pDevInfo->extAddress);
    pDevInfo->panID = devInfoBlock.panID;
    pDevInfo->shortAddress = devInfoBlock.devShortAddr;
    ZB_IEEE_ADDR_COPY((zb_ieee_addr_t*) &devInfoBlock.coordExtAddr, &pParentNetInfo->devInfo.extAddress);
    pParentNetInfo->channel = devInfoBlock.channel;
    pParentNetInfo->devInfo.panID = devInfoBlock.panID;
    pParentNetInfo->devInfo.shortAddress = devInfoBlock.coordShortAddr;
}

/*!
 * @brief       Handle maximum ACK failures
 */
static void handleMaxDataFail(void)
{
    if (!CONFIG_RX_ON_IDLE)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "handleMaxDataFail: Stopping polling for sleepy ZED");
        // Stop polling
        UtilTimer_stop(&pollClkStruct);
    }

    // Initialize counter for re-join delay calculation
    interimDelayTicks = ClockP_getSystemTicks();
    Log_printf(LogModule_Zibgbee_App, Log_INFO, "handleMaxDataFail: interimDelayTicks = %d", interimDelayTicks);
    // Non-beacon network - update stats
    Sensor_msgStats.syncLossIndications++;

    Log_printf(LogModule_Zigbee_App, Log_INFO, "handleMaxDataFail: Switching JDLLC state to orphan and device state to orphan scan");
    switchState(Jdllc_deviceStates_scanOrphan);
    updateState(Jdllc_states_orphan);
    devInfoBlock.dataFailures = 0;
}

/******************************************************************************
 ClockP Utilities
 *****************************************************************************/

/*!
 Initialize a TIRTOS Timer/Clock instance. Note that a ClockP_Fxn is of type void with a uintprt_t argument.
 */
ClockP_Handle UtilTimer_construct(ClockP_Struct *pClock, ClockP_Fxn clockCB,
                                 zb_uint32_t clockDuration, zb_uint32_t clockPeriod,
                                 zb_uint8_t startFlag,
                                 uintptr_t arg)
{
    ClockP_Params clockParams;

    /* Convert clockDuration in milliseconds to ticks. */
    zb_uint32_t clockTicks = clockDuration * (1000 / ClockP_getSystemTickPeriod());

    /* Setup parameters. */
    ClockP_Params_init(&clockParams);

    /* Setup argument. */
    clockParams.arg = arg;

    /* If period is 0, this is a one-shot timer. */
    clockParams.period = clockPeriod * (1000 / ClockP_getSystemTickPeriod());

    /*
     Starts immediately after construction if true, otherwise wait for a
     call to start.
     */
    clockParams.startFlag = startFlag;

    /*/ Initialize clock instance. */
    ClockP_construct(pClock, clockCB, clockTicks, &clockParams);

    return ClockP_handle(pClock);
}

/*!
 Destruct a TIRTOS Timer/Clock instance.
 */
void UtilTimer_destruct(ClockP_Struct *pClock)
{
    ClockP_destruct(pClock);
}

/*!
 Start a timer/clock.

 Public function defined in jdllc.h
 */
void UtilTimer_start(ClockP_Struct *pClock)
{
    ClockP_Handle handle = ClockP_handle(pClock);

    /* Start clock instance */
    ClockP_start(handle);
}

/*!
 Determine if a timer/clock is currently active.

 Public function defined in jdllc.h
 */
bool UtilTimer_isActive(ClockP_Struct *pClock)
{
    ClockP_Handle handle = ClockP_handle(pClock);

    /* Start clock instance */
    return ClockP_isActive(handle);
}

/*!
 Stop a timer/clock.

 Public function defined in jdllc.h
 */
void UtilTimer_stop(ClockP_Struct *pClock)
{
    ClockP_Handle handle = ClockP_handle(pClock);

    /* Start clock instance */
    ClockP_stop(handle);
}

/*!
 * @brief   Set a Timer/Clock timeout.
 *
 * @param   timeOut - Timeout value in milliseconds
 */
void UtilTimer_setTimeout(ClockP_Handle handle, zb_uint32_t timeout)
{
    ClockP_setTimeout(handle, (timeout * UtilTimer_MS_ADJUSTMENT));
}

/*!
 * @brief   Get a Timer/Clock timeout.
 *
 * @param   handle - clock handle
 *
 * @return   timeout - Timeout value in milliseconds
 */
uint32_t UtilTimer_getTimeout(ClockP_Handle handle)
{
    zb_uint32_t timeout;

    timeout = ClockP_getTimeout(handle);

    return (timeout / UtilTimer_MS_ADJUSTMENT);
}

/*!
 Set the poll clock. Note that poll time is in milliseconds.
 */
void setPollClock(zb_uint32_t pollTime)
{
    // Assuming that pollClkStruct is initialized by the time this function is called. This should always be the case
    // because the construction of pollClkStruct is done in Jdllc_init().

    /* Stop the polling timer */
    if(UtilTimer_isActive(&pollClkStruct) == true)
    {
        UtilTimer_stop(&pollClkStruct);
    }

    // Removed the if-statements on stuff like FEATURE_SECURE_COMMISSIONING and POWER_MEAS

    /* Setup timer */
    if(pollTime > 0)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "setPollClock: Setting poll timer period to %d ms", pollTime);
        UtilTimer_setTimeout(pollClkHandle, pollTime);
        UtilTimer_start(&pollClkStruct);
    }
}

/*!
 Set the scan backoff clock. Note that scanBackoffTime is in milliseconds.
 */
void setScanBackoffClock(zb_uint32_t scanBackoffTime)
{
    // Assuming that scanBackoffClkStruct is initialized by the time this function is called. This should always be the case
    // because the construction of scanBackoffClkStruct is done in Jdllc_init().

    /* Stop the timer */
    if(UtilTimer_isActive(&scanBackoffClkStruct) == true)
    {
        UtilTimer_stop(&scanBackoffClkStruct);
    }

    /* Setup timer */
    if(scanBackoffTime > 0)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "setScanBackoffClock: Setting scan backoff timer period to %d ms", scanBackoffTime);
        UtilTimer_setTimeout(scanBackoffClkHandle, scanBackoffTime);
        UtilTimer_start(&scanBackoffClkStruct);
    }
}