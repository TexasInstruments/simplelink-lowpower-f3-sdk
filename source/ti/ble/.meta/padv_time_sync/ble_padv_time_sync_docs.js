/*
 * Copyright (c) 2025 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ble_padv_time_sync_docs.js ========
 */

"use strict";

// Long descriptions for PADV Time Sync Configuration

const padvTimeSyncLongDescription = `
Periodic Advertising Time Synchronization enables time synchronization between
BLE devices using periodic advertising.

**Architecture:**
- **Time Sync Advertiser (TSA):** Broadcasts local time in periodic advertising packets
- **Time Sync Observer (TSO):** Receives TSA periodic advertising and extracts
  timing information to synchronize to TSA local time

**Use Cases:**
- Synchronized sensor networks
- Multi-device time-critical applications (Connection Handover, Connection Monitor)
- Asset tracking with precise timing

When enabled, the appropriate role-specific parameters will be shown based on
the selected Time Sync Role.

**This task priority is high to maintain time synchronization consistency;
Low execution frequency keeps the impact on other tasks minimal.**
`;

const padvTimeSyncRoleLongDescription = `
Select the time synchronization role for this device:

**Time Sync Advertiser (TSA):**
- Acts as the time reference (master)
- Broadcasts local time in periodic advertising data
- Requires Periodic Advertising to be enabled

**Time Sync Observer (TSO):**
- Acts as time receiver (slave)
- Listens for periodic advertising from a TSA
- Synchronizes time operations based on received TSA local time
- Requires Periodic Advertising Sync to be enabled
`;

const padvTimeSyncPeriodicAdvIntervalLongDescription = `
Configure the periodic advertising interval for the TSA role.

The interval is specified in units of 1.25ms.

**Range:** 6 to 65535 (7.5ms to ~82 seconds)

**Common values:**
- 1600 = 2s (default, good balance of power and responsiveness)

**Note:** Both minimum and maximum should typically be set to the same value
for consistent timing behavior.
`;

const padvTimeSyncAdvSIDLongDescription = `
The Advertising Set ID (SID) identifies the periodic advertising train.

**Range:** 0 to 15

**Usage:**
- TSA uses this SID when creating the periodic advertising set
- TSO uses this SID to filter which periodic advertising to sync to

**Note:** Both TSA and TSO devices must use the same SID to establish
time synchronization.
`;

const padvTimeSyncTimeoutLongDescription = `
The synchronization timeout specifies how long the TSO will wait for
periodic advertising packets before considering sync lost.

**Unit:** 10ms

**Range:** 10 to 16384 (100ms to ~163 seconds)

**Default:** 600 (6 seconds)

**Recommendation:**
- Set to at least 3x the periodic advertising interval
- Account for potential packet loss and interference
`;

const padvTimeSyncSkipLongDescription = `
The skip count specifies the maximum number of periodic advertising
events that can be skipped after successfully receiving a packet.

**Range:** 0 to 499

**Default:** 0 (no skipping)

**Usage:**
- Higher values reduce power consumption but increase sync latency
- Set to 0 for fastest time synchronization updates
- Increase if power consumption is a concern and occasional missed
  updates are acceptable
`;

const padvTimeSyncTsaNameLongDescription = `
The TSA Device Name is used for time sync device discovery:

**For TSA Role:**
- This name is advertised in the extended advertisement data
- TSO devices use this name to discover and identify the TSA

**For TSO Role:**
- This name is used to filter extended advertisements
- TSO discovers the TSA by matching this name

**Discovery Process (TSO):**
1. TSO starts scanning for extended advertisements
2. Filters advertisements by this configured device name
3. When found, extracts the TSA's address from the advertisement
4. Creates periodic advertising sync using the discovered address

**Range:** 1 to 29 characters

**Default:** "TSA_Device"

**Important:** Both TSA and TSO devices must use the same TSA Device Name
for successful discovery and time synchronization.
`;

exports = {
    padvTimeSyncLongDescription: padvTimeSyncLongDescription,
    padvTimeSyncRoleLongDescription: padvTimeSyncRoleLongDescription,
    padvTimeSyncPeriodicAdvIntervalLongDescription: padvTimeSyncPeriodicAdvIntervalLongDescription,
    padvTimeSyncAdvSIDLongDescription: padvTimeSyncAdvSIDLongDescription,
    padvTimeSyncTimeoutLongDescription: padvTimeSyncTimeoutLongDescription,
    padvTimeSyncSkipLongDescription: padvTimeSyncSkipLongDescription,
    padvTimeSyncTsaNameLongDescription: padvTimeSyncTsaNameLongDescription
};
