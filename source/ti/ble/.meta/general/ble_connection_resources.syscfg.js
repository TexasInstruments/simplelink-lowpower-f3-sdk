/*
 * Copyright (c) 2018-2025 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ble_connection_resources.syscfg.js ========
 *  BLE connection resource limits: max connections, PDU buffers,
 *  resolving list and accept list sizes.
 */

"use strict";

// Get long descriptions
const Docs = system.getScript("/ti/ble/general/ble_connection_resources_docs.js");

// Get common Script
const Common = system.getScript("/ti/ble/ble_common.js");

const configItems = [
    {
        name: "maxConnNum",
        displayName: "Max Number of Connections",
        default: maxConnNumDefaultValue(),
        longDescription: Docs.maxConnNumLongDescription
    },
    {
        name: "maxRLSize",
        displayName: "Max Number of Resolving List Entries",
        default: 5,
        longDescription: Docs.maxRLSizeLongDescription
    },
    {
        name: "maxALSize",
        displayName: "Max Number of Accept List Entries",
        default: 5,
        longDescription: Docs.maxALSizeLongDescription
    },
    {
        name: "maxPDUNum",
        displayName: "Max Number of PDUs",
        default: 5,
        longDescription: Docs.maxPDUNumLongDescription
    },
    {
        name: "maxPDUSize",
        displayName: "Max Size of PDU (bytes)",
        default: 69,
        longDescription: Docs.maxPDUSizeLongDescription
    },
];

const config = {
    displayName: "Connection Resources",
    description: "Configure BLE connection and buffer resource limits",
    config: configItems
};

/*
 *  ======== maxConnNumDefaultValue ========
 *  Return the required default value of the maxConnNum according
 *  to the current device.
 *  @returns - the default max num of conns value
 */
function maxConnNumDefaultValue()
{
    return 1;
}

/*
 * ======== validate ========
 * Validate this inst's configuration
 *
 * @param inst       - instance to be validated
 * @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    const modes = Common.getInstModes(inst);

    // Limit Maximum Number of Connections to be 16 when device role != Central
    if(!modes.central && (inst.maxConnNum < 0 || inst.maxConnNum > 16))
    {
        validation.logError("Maximum Number of Connections range is 0 to 16", inst, "maxConnNum");
    }
    // Limit Maximum Number of Connections to be 32 when device role == Central
    if(modes.central && (inst.maxConnNum < 0 || inst.maxConnNum > 32))
    {
        validation.logError("Maximum Number of Connections range is 0 to 32", inst, "maxConnNum");
    }

    if(inst.maxPDUSize < 27 || inst.maxPDUSize > 255)
    {
        validation.logError("Max PDU size Valid range is 27 to 255", inst, "maxPDUSize");
    }
    if(inst.maxPDUNum < 0 || inst.maxPDUNum > (Math.pow(2,8) - 1))
    {
        validation.logError("Maximum Number of PDUs range is 0 to sizeof(uint8)", inst, "maxPDUNum");
    }

    if ((inst.maxRLSize < 1) || (inst.maxRLSize > 50))
    {
        validation.logError("The max number of resolving list size is 1 to 50", inst, "maxRLSize");
    }

    if ((inst.maxALSize < 1) || (inst.maxALSize > 50))
    {
        validation.logError("The max number of resolving list size is 1 to 50", inst, "maxALSize");
    }
}

/*
 *  ======== getOpts ========
 */
function getOpts(mod)
{
    const inst = mod.$static;
    let result = [];

    result.push("-DSYSCFG");
    result.push("-DMAX_NUM_BLE_CONNS=" + inst.maxConnNum);
    result.push("-DCFG_MAX_NUM_RL_ENTRIES=" + inst.maxRLSize);
    result.push("-DCFG_MAX_NUM_AL_ENTRIES=" + inst.maxALSize);
    result.push("-DMAX_NUM_PDU=" + inst.maxPDUNum);
    result.push("-DMAX_PDU_SIZE=" + inst.maxPDUSize);

    return result;
}

/*
 *  ======== exports ========
 *  Export the BLE Connection Resources module
 */
exports = {
    config: config,
    configItems: configItems,
    validate: validate,
    maxConnNumDefaultValue: maxConnNumDefaultValue,
    getOpts: getOpts
};
