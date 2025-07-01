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
/* PURPOSE: SE startup attribute sets definitions
*/
#ifndef ZB_SE_SAS_H
#define ZB_SE_SAS_H 1

#ifdef ZB_ENABLE_SE
#include "zb_se_config.h"
#endif

/*
If this key establishment fails, it is likely that one side of the
exchange is configured with an invalid certificate or with no
certificate at all. It is permissible to retry this step multiple
times in succession, but it is expressly not allowed that a
device repeat this step more than ten times in succession without
pausing for a minimum of least fifteen minutes.
*/
#define ZB_SE_MAX_SUCCESSION_CBKE_RETRIES 10
#define ZB_SE_PAUSE_IN_CBKE_RETRIES ZB_MILLISECONDS_TO_BEACON_INTERVAL(15 * 60 * 1000)

#ifndef ZSE_TEST_HACKS
#define ZB_SE_JOIN_DEV_CBKE_TIMEOUT ZB_MILLISECONDS_TO_BEACON_INTERVAL(20 * 60 * 1000)
#else
#define ZB_SE_JOIN_DEV_CBKE_TIMEOUT ZB_MILLISECONDS_TO_BEACON_INTERVAL(10 * 1000)
#endif

/*
When forming a new network, or scanning to join a network, Smart
Energy devices should do channel scans using the following
preferred channels before scanning the rest of the channels
*/
#define ZB_SE_PREFERRED_24_CHANNELS  ((1<<11)|(1<<14)|(1<<15)|(1<<19)|(1<<20)|(1<<24)|(1<<25))

#define ZB_SE_MAX_FORMATION_RETRIES 3

#if defined ZB_ENABLE_SE_SAS

/** @cond DOXYGEN_SE_SECTION */

/* Zigbee-16-05033-011 ZSE 1.4 spec:
   5.3 Startup Attribute Set */

/* 5.3.1 Startup Parameters (Table 5-1) */

#define ZB_SE_SAS_SHORT_ADDRESS ZB_NWK_BROADCAST_ALL_DEVICES
/* EPAN ID - 0x0000000000000000 or installer specified. */
/* PAN ID - 0xFFFF or installer specified. */
#define ZB_SE_SAS_PAN_ID 0xFFFF
#define ZB_SE_SAS_CHANNEL_MASK ZB_DEFAULT_APS_CHANNEL_MASK/* ZB_TRANSCEIVER_ALL_CHANNELS_MASK */

/* 2 (Zigbee and later) */
#define ZB_SE_SAS_PROTO_VER	STACK_PRO

/* 1 (Zigbee) or 2 (Zigbee PRO) */
#define ZB_SE_SAS_STACK_PROFILE ZB_PROTOCOL_VERSION

/* 2 (two) if uncommissioned, so it will join network by
   association when a join command is indicated.
   0 (zero) if commissioned. Indicates that the device
   should consider itself a part of the network indicated by
   the ExtendedPANId attribute. In this case it will not perform
   any explicit join or rejoin operation. */
/* Startup Control - optional and not implemented. */


/* Please note - in Smart Energy Profile 1.1 and above, only the
   Coordinator (0x0000) can be the SE Trust Center.*/
#define ZB_SE_SAS_TRUST_CENTER_ADDR 0x0000

/* Master Key - Not used, high security is not used in this profile. */

/* If the Key Establishment Cluster is being used to
   install a link key Installer provided if using preconfigured link keys. */
/* FIXME: It is unclear how to use it! */
#define ZB_SE_SAS_LINK_LEY	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01"

/* If no pre-installed key present. */
/* FIXME: It is unclear how to use it! */
#define ZB_SE_SAS_NETWORK_KEY "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01"

/* Not Used */
/* FIXME: Remove? */
#define ZB_SE_SAS_DTC_LINK_KEY  \
  "\x5a\x69\x67\x42\x65\x65\x41\x6C\x6C\x69\x61\x6E\x63\x65\x30\x39"

/* Flag that disables the use of insecure join as a fallback case at startup time*/
#define ZB_SE_SAS_INSECURE_JOIN ZB_FALSE


/* 5.3.2 Join Parameters (Table 5-2) */

/* At boot time or when instructed to join a network, the device should complete up to
   three (3) scan attempts to find a Zigbee Coordinator or Router with which to associate. If it has
   not been commissioned, this means that when the user presses a button or uses another methodology to
   join a network, it will scan all of the channels up to three times to find a network that allows
   joining. If it has already been commissioned, it should scan up to three times to find its original
   PAN to join. (Zigbee Pro devices should scan for their original extended PAN ID and Zigbee (2007)
   devices can only scan for their original PAN ID). */
#define ZB_SE_SAS_SCAN_ATTEMPTS 3

/* Determines the number of seconds between each unsuccessful scan attempt. */
#define ZB_SE_SAS_TIME_BTWN_SCANS 1000

/* 60 seconds or shorter
   How quickly a device will attempt to rejoin the network if it finds itself disconnected. */
/* Rejoin Interval - optional, not implemented */

/* 15 minutes
   Imposes an upper bound on the RejoinInterval parameter - this must be restarted if device is
   touched by human user, i.e. by a button press. This parameter is intended to throttle how
   often a device will scan to find its network in case the network is no longer present and
   therefore a scan attempt by the device would always fail (i.e., if a device finds it has lost
   network connectivity, it will try to rejoin the network, scanning all channels if necessary).
   If the scan fails to find the network, or fails to successfully rejoin, the device will wait for
   15 minutes before attempting to rejoin again. To be network friendly, it would be recommended to
   adaptively extend this time period if successive rejoins fail. It would also be recommended the
   device should try a rejoin when triggered (via a control, button, etc.) and fall back to this
   interval if rejoins fail again. */
/* Max Rejoin Interval - optional, not implemented */

/* 5.3.3 Security Parameters (Table 5-3) */

/* SecurityTimeoutPeriod - Set by stack profile. */
/* TrustCenterNetworkKey - The Trust Center will pick the network key. */

/* 5.3.4 End Device Parameters (Table 5-4) */
/* IndirectPollRate - Set by stack profile. */

/* 5.3.5 Link Status Parameters (Table 5-5) */
/* LinkStatusPeriod, LinkPowerDeltaTransmitRate, RouterAgeLimit, RepairThreshold - Set by stack
 * profile. */

/* 5.3.6 Concentrator Parameters (Table 5-6) */
/* ConcentratorFlag - Set by stack profile */

/* Device manufacturers that produce a concentrator product will set the max  concentrator radius to
 * this value.  */
#define ZB_SE_SAS_CONCENTRATOR_RADIUS 11

/* ConcentratorDiscoveryTime - Set by stack profile */

/* 5.3.7 APS Transport Parameters (Table 5-7) */
/* MaxFrameRetries, AckWaitDuration - Set by stack profile. */

/* 5.3.8 APS Fragmentation Parameters (Table 5-8) */

/* FIXME: Connect with fragmentation branch. Now parameter is not used and not configurable -
 * fragmentation works only with Max Window Size == 1, no interframe delay in that case (each frame
 * is sent in its own frame window as usual APS packet with acknowledge). */
/* Standard delay in milliseconds between sending two blocks of a fragmented transmission (see [B3]
 * sub-clause 2.2.8.4.5) */
#define ZB_SE_SAS_APS_INTERFRAME_DELAY 50

/* FIXME: Connect with fragmentation branch. Now parameter is not configurable - fragmentation
 * works only with Max Window Size == 1. */
/* Fragmentation parameter - the maximum number of unacknowledged frames that can be active at once
 * (see [B3] sub-clause 2.2.8.4.5) */
#define ZB_SE_SAS_APS_MAX_WINDOW_SIZE	1

/* FIXME: Connect with fragmentation branch. */
/* In addition the Maximum Incoming Transfer Size Field in the Node descriptor defines the
   largest ASDU that can be transferred using fragmentation. For the Smart Energy Profile the
   default value shall be set to 128 bytes. Maximum ASDU size allowed is specified in [B3] and
   dictated by solution needs and RAM capacities of the communicating devices.
*/
#define ZB_SE_SAS_MAX_INCOMING_TRANSFER_SIZE 128

/* 5.3.9 Binding Parameters (Table 5-9) */

/* EndDeviceBindTimeout - 60 seconds */
#define ZB_SE_SAS_END_DEVICE_BIND_TIMEOUT 60

void zb_se_init_sas();

/** @endcond */ /* DOXYGEN_SE_SECTION */

#endif /* defined ZB_ENABLE_SE_SAS */

#endif /* ZB_SE_SAS_H */
