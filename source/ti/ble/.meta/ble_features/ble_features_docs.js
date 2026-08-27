/*
 * Copyright (c) 2018 Texas Instruments Incorporated - http://www.ti.com
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
 *
 */

/*
 *  ======== ble_features_docs.js ========
 *  Long descriptions for BLE feature flag configurables.
 */

"use strict";

// Long description for the extAdv configuration parameter
const extAdvLongDescription = `BLE5 Extended advertising feature.\n
For more information, refer to the [BLE Stack User's Guide]\
(/ble5stack/ble_user_guide/html/ble-stack-5.x/gap-cc23xx.html).\n
__Default__: True (enabled)\n
__Note__: When the feature is disabled, remove all extended advertising sets`

// Long description for the periodicAdv configuration parameter
const periodicAdvLongDescription = `Periodic advertising feature.\n
For more information, refer to the [BLE Stack User's Guide]\
(/ble5stack/ble_user_guide/html/ble-stack-5.x/gap-cc23xx.html#periodic-\
advertising).\n
__Default__: False (disabled)\n
__Note__: When the feature is enabled, add non-connectable non-scannable \
extended advertising set`

// Long description for the periodicAdvSync configuration parameter
const periodicAdvSyncLongDescription = `Periodic adv sync feature.\n
Synchronization can only occur when scanning is enabled. While scanning is \
disabled, no attempt to synchronize will take place.\n
For more information, refer to the [BLE Stack User's Guide]\
(/ble5stack/ble_user_guide/html/ble-stack-5.x/gap-cc23xx.html#periodic-\
advertising)\n
__Default__: False (disabled)\n`

// Long description for the PAwRAdvertiser configuration parameter
const PAwRAdvertiserLongDescription = `Periodic advertising with responses feature at the advertiser side.\n
__Default__: False (disabled)\n`

// Long description for the PAwRScanner configuration parameter
const PAwRScannerLongDescription = `Periodic advertising with responses feature at the scanner side.\n
__Default__: False (disabled)\n`

// Long description for the PASTReceiver configuration parameter
const PASTReceiverLongDescription = `Periodic Advertising Sync Transfer feature \
in the receiver role.\n
When enabled, the device can receive Periodic Advertising Sync Transfer control\
packets from a peer devices advertising.\n
__Default__: False (disabled)\n`

// Long description for the PASTSender configuration parameter
const PASTSenderLongDescription = `Periodic Advertising Sync Transfer feature \
in the sender role.\n
When enabled, the device can send Periodic Advertising Sync Transfer control\
packets to a peer devices connected to.\n
__Default__: False (disabled)\n`

// Long description for the trensLayer configuration parameter
const trensLayerLongDescription = `HCI Transport Layer Mode.\n
For more information, refer to the [BLE Stack User's Guide](ble5stack/ble_user_guide/html/\
ble-stack-5.x/hci-cc23xx.html).\n
__Default__: None \n
__Note__: When using PTM configuration please choose \`None\` \n`

// Long description for the bondManager configuration parameter
const bondManagerLongDescription = `The GAP Bond Manager (GAPBondMgr) is a \
configurable module that offloads most of the Pairing & Bonding security \
mechanisms associated with the Security Manager (SM) protocol from the application. \
The GAPBondMgr executes in the protocol stack task's context. For more information, \
refer to the [BLE Stack User's Guide](/ble5stack/ble_user_guide/html/ble-stack-5.x/\
gapbondmngr-cc23xx.html#gap-bond-manager-and-le-secure-connections).\n
__Default__: True (checked)\n`

// Long description for the gattDB configuration parameter
const gattDBLongDescription = `Indicates that the GATT database is maintained off \
the chip on the Application Processor (AP)\n
__Default__: False (unchecked)\n`

// Long description for the gattNoClient configuration parameter
const gattNoClientLongDescription = `In order to use Privacy, the GAP Bond \
Manager requires the stack's GATT client to read the peer device's GAP \
characteristics. To accomplish this, GATT_NO_CLIENT should be \
unchecked so that the GATT client is included.\n
For more information, refer to the [BLE Stack User's Guide](/ble5stack/\
ble_user_guide/html/ble-stack-5.x/privacy-cc23xx.html#using-privacy-in-stack).\n
__Default__: False (unchecked)\n`

// Long description for the L2CAPCOC configuration parameter
const L2CAPCOCLongDescription = `Enable/Disable the use of L2CAP Connection Oriented Channels. \
The BLE5-Stack provides APIs to create L2CAP CoC channels to transfer bidirectional data between \
two Bluetooth Low Energy devices supporting this feature. \
For more information, refer to the User Guide \n
__Default__: False (unchecked)\n`

// Long description for the delayingAttReadReq configuration parameter
const delayingAttReadReqLongDescription = `Delaying an ATT_READ_REQ from a \
registered service is possible by enabling this parameter.\n
For more information, refer to the [BLE Stack User's Guide](/ble5stack/\
ble_user_guide/html/ble-stack-5.x/gatt-cc23xx.html#delaying-an-att-read-request).\n
__Default__: False (unchecked)\n`

// Long description for the legacyCmd configuration parameter
const legacyCmdLongDescription = `Enable support for legacy (standard Bluetooth) HCI commands.\n
When enabled, the controller includes the legacy HCI command parser which processes \
standard Bluetooth HCI commands as defined in the Bluetooth Core Specification.\n
Disable this feature only if the application does not issue any standard HCI commands, \
in order to reduce code size.\n
__Default__: False (unchecked)\n`

// Long description for the vendorSpecificCmd configuration parameter
const vendorSpecificCmdLongDescription = `Enable support for TI vendor-specific HCI commands.\n
When enabled, the controller includes the vendor-specific HCI command parser which provides \
extended functionality beyond the standard Bluetooth specification, including commands for \
Direct Test Mode (DTM), RSSI monitoring, and other proprietary features.\n
Disable this feature only if none of these vendor-specific commands are needed, \
in order to reduce code size.\n
__Default__: False (unchecked)\n`

// Long description for the extVendorSpecificCmd configuration parameter
const extVendorSpecificCmdLongDescription = `Enable support for TI extended vendor-specific HCI commands.\n
When enabled, the controller includes the extended vendor-specific HCI command parser which \
provides basic HCI utility commands such as HCI Reset, Set BD Address, and Read BD Address.\n
Disable this feature only if none of these extended commands are needed, \
in order to reduce code size.\n
__Default__: False (unchecked)\n`

// Long descriptions for the Adaptivity configuration parameters
const adaptivityLongDescription = `Enable Adaptivity module,\
 This module is responsible for monitoring and limiting the transmit time on each BLE channel,\
 This module can be used to help meet the adaptivity requirements of some regulatory regions.\n
__Default__: Disabled\n`

// Long description for the bleHealth configuration parameter
const bleHealthLongDescription = `Enable/Disable the use of Health Toolkit.\n
For more information, refer to the User Guide \n
__Default__: False (unchecked)\n`

// Exports the long descriptions for each configurable
exports = {
    extAdvLongDescription:            extAdvLongDescription,
    periodicAdvLongDescription:       periodicAdvLongDescription,
    periodicAdvSyncLongDescription:   periodicAdvSyncLongDescription,
    PAwRAdvertiserLongDescription:    PAwRAdvertiserLongDescription,
    PAwRScannerLongDescription:       PAwRScannerLongDescription,
    PASTReceiverLongDescription:      PASTReceiverLongDescription,
    PASTSenderLongDescription:        PASTSenderLongDescription,
    trensLayerLongDescription:        trensLayerLongDescription,
    bondManagerLongDescription:       bondManagerLongDescription,
    gattDBLongDescription:            gattDBLongDescription,
    gattNoClientLongDescription:      gattNoClientLongDescription,
    L2CAPCOCLongDescription:          L2CAPCOCLongDescription,
    delayingAttReadReqLongDescription:  delayingAttReadReqLongDescription,
    legacyCmdLongDescription:           legacyCmdLongDescription,
    vendorSpecificCmdLongDescription:   vendorSpecificCmdLongDescription,
    extVendorSpecificCmdLongDescription: extVendorSpecificCmdLongDescription,
    adaptivityLongDescription:           adaptivityLongDescription,
    bleHealthLongDescription:            bleHealthLongDescription,
};
