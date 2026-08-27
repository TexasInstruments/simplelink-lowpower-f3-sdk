/*
 * Copyright (c) 2018-2022 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ble_connection_resources_docs.js ========
 */

"use strict";

// Long description for the maxConnNum configuration parameter
const maxConnNumLongDescription = `This is the maximum number of simultaneous \
BLE connections allowed. Adding more connections uses more RAM and may \
require increasing HEAPMGR_SIZE.\n
For more information, refer to the [BLE User's Guide](ble5stack/ble_user_guide/\
html/ble-stack-5.x/optimization-memory-cc23xx.html#ram-optimization).\n
__Default__: 1\n
__Range__: A combination that includes Central or Peripheral Roles: 1 to 32, Other Role \
combinations: 0`

// Long description for the maxRLSize configuration parameter
const maxRLSizeLongDescription = `This parameter defines the maximum number of \
entries in the Resolving List. The Resolving List is used to store the Identity \
Resolving Keys (IRKs) and associated device addresses for resolving private \
addresses.\n
__Default__: 5\n
__Range__: 1 to 50\n
__Note__: Increasing the size of the Resolving List may increase the time required \
to resolve device RPAs, as the list must be searched for a matching IRK.`

// Long description for the maxALSize configuration parameter
const maxALSizeLongDescription = `This parameter defines the maximum number of \
entries in the Accept List. The Accept List is used to store device addresses \
that are allowed to connect or communicate with the device. It acts as a filter \
to ensure that only devices in the Accept List can establish a connection or be \
scanned, according to the privacy policy configuration.\n
__Default__: 5\n
__Range__: 1 to 50`

// Long description for the maxPDUNum configuration parameter
const maxPDUNumLongDescription = `Maximum number of BLE HCI PDUs.\n
__Default__: 5\n
__Range__: 0 to sizeof(uint8)\n
__Note__: If the maximum number of connections is set to 0, then this number \
should also be set to 0.\n
Set \`MAX_NUM_PDU\` and \`MAX_PDU_SIZE\` to reduce the amount of packets that \
can be queued up by the stack at a time. This will reduce heap consumption.\n
For more information, refer to the [BLE User's Guide](ble5stack/ble_user_guide/\
html/ble-stack-5.x/optimization-memory-cc23xx.html#ram-optimization).\n`

// Long description for the maxPDUSize configuration parameter
const maxPDUSizeLongDescription = `Maximum size in bytes of the BLE HCI PDU.\n
__Default__: 69\n
__Range__: 27 to 255\n
__Note__: If using LE Secure Connections, MAX_PDU_SIZE must be >= 69\n
The maximum ATT_MTU is MAX_PDU_SIZE - 4\n
For more information, refer to the [BLE User's Guide](ble5stack/ble_user_guide/\
html/ble-stack-5.x/optimization-memory-cc23xx.html#ram-optimization).\n`

exports = {
    maxConnNumLongDescription: maxConnNumLongDescription,
    maxRLSizeLongDescription: maxRLSizeLongDescription,
    maxALSizeLongDescription: maxALSizeLongDescription,
    maxPDUNumLongDescription: maxPDUNumLongDescription,
    maxPDUSizeLongDescription: maxPDUSizeLongDescription
};
