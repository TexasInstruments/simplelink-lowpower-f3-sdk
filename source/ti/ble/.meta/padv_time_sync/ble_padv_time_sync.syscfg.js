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
 *  ======== ble_padv_time_sync.syscfg.js ========
 */

"use strict";

// Get documentation
const Docs = system.getScript("/ti/ble/padv_time_sync/ble_padv_time_sync_docs.js");

// Get common Script
const Common = system.getScript("/ti/ble/ble_common.js");

const config = {
    name: "padvTimeSyncConfig",
    displayName: "Periodic Advertising Time Sync Configuration",
    description: "Configure Periodic Advertising Time Synchronization Settings",
    config: [
        {
            name: "hidePadvTimeSyncGroup",
            default: true,
            hidden: true,
            onChange: onHidePadvTimeSyncGroupChange
        },
        {
            name: "padvTimeSync",
            displayName: "Enable PADV Time Sync",
            default: false,
            hidden: true,
            description: "Enable Periodic Advertising Time Synchronization feature",
            longDescription: Docs.padvTimeSyncLongDescription,
            onChange: onPadvTimeSyncChange
        },
        {
            name: "padvTimeSyncRole",
            displayName: "Time Sync Role",
            default: "APP_PADV_TIME_SYNC_ROLE_TSA",
            hidden: true,
            description: "Select the time synchronization role for this device",
            longDescription: Docs.padvTimeSyncRoleLongDescription,
            options: [
                {
                    name: "APP_PADV_TIME_SYNC_ROLE_TSA",
                    displayName: "Time Sync Advertiser (TSA)"
                },
                {
                    name: "APP_PADV_TIME_SYNC_ROLE_TSO",
                    displayName: "Time Sync Observer (TSO)"
                }
            ],
            onChange: onPadvTimeSyncRoleChange
        },
        {
            name: "padvTimeSyncPeriodicAdvIntervalMin",
            displayName: "Periodic Adv Interval Min",
            default: 1600,
            hidden: true,
            description: "Minimum periodic advertising interval (units of 1.25ms)",
            longDescription: Docs.padvTimeSyncPeriodicAdvIntervalLongDescription
        },
        {
            name: "padvTimeSyncPeriodicAdvIntervalMax",
            displayName: "Periodic Adv Interval Max",
            default: 1600,
            hidden: true,
            description: "Maximum periodic advertising interval (units of 1.25ms)",
            longDescription: Docs.padvTimeSyncPeriodicAdvIntervalLongDescription
        },
        {
            name: "padvTimeSyncTimeout",
            displayName: "Sync Timeout (10ms units)",
            default: 600,
            hidden: true,
            description: "Sync timeout for TSO role (units of 10ms)",
            longDescription: Docs.padvTimeSyncTimeoutLongDescription
        },
        {
            name: "padvTimeSyncSkip",
            displayName: "Skip Count",
            default: 0,
            hidden: true,
            description: "Number of periodic advertising events to skip (TSO only)",
            longDescription: Docs.padvTimeSyncSkipLongDescription
        },
        {
            name: "padvTimeSyncAdvSID",
            displayName: "Advertising SID",
            default: 1,
            hidden: true,
            description: "Advertising Set ID for periodic advertising (0-15)",
            longDescription: Docs.padvTimeSyncAdvSIDLongDescription
        },
        {
            name: "padvTimeSyncTsaName",
            displayName: "TSA Device Name",
            default: "TSA_Device",
            hidden: true,
            description: "Device name for time sync (TSA advertises this, TSO searches for this)",
            longDescription: Docs.padvTimeSyncTsaNameLongDescription
        }
    ]
};

/*
 *  ======== onHidePadvTimeSyncGroupChange ========
 * Called when the group visibility flag changes.
 * When the group is shown again, re-syncs internal UI with current padvTimeSync state.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onHidePadvTimeSyncGroupChange(inst, ui)
{
    if (!inst.hidePadvTimeSyncGroup)
    {
        onPadvTimeSyncChange(inst, ui);
    }
}

/*
 *  ======== onPadvTimeSyncChange ========
 * Handles the change of PADV Time Sync enable checkbox
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onPadvTimeSyncChange(inst, ui)
{
    // Show/hide role selection and parameters based on enable state
    ui.padvTimeSyncRole.hidden = !inst.padvTimeSync;

    // Update role-specific parameters visibility
    if (inst.padvTimeSync)
    {
        onPadvTimeSyncRoleChange(inst, ui);
    }
    else
    {
        // Hide all parameters when disabled
        ui.padvTimeSyncPeriodicAdvIntervalMin.hidden = true;
        ui.padvTimeSyncPeriodicAdvIntervalMax.hidden = true;
        ui.padvTimeSyncAdvSID.hidden = true;
        ui.padvTimeSyncTimeout.hidden = true;
        ui.padvTimeSyncSkip.hidden = true;
        ui.padvTimeSyncTsaName.hidden = true;
    }
}

/*
 *  ======== onPadvTimeSyncRoleChange ========
 * Handles the change of PADV Time Sync role selection
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onPadvTimeSyncRoleChange(inst, ui)
{
    const isTSA = inst.padvTimeSyncRole === "APP_PADV_TIME_SYNC_ROLE_TSA";
    const isTSO = inst.padvTimeSyncRole === "APP_PADV_TIME_SYNC_ROLE_TSO";

    // TSA-specific parameters (periodic advertising intervals)
    ui.padvTimeSyncPeriodicAdvIntervalMin.hidden = !isTSA;
    ui.padvTimeSyncPeriodicAdvIntervalMax.hidden = !isTSA;

    // Common parameter (used by both TSA and TSO)
    ui.padvTimeSyncAdvSID.hidden = !inst.padvTimeSync;

    // TSO-specific parameters (sync timeout, skip)
    ui.padvTimeSyncTimeout.hidden = !isTSO;
    ui.padvTimeSyncSkip.hidden = !isTSO;

    // TSA name is used by both roles:
    // - TSA: name to advertise for TSO discovery
    // - TSO: name to search for when finding TSA
    ui.padvTimeSyncTsaName.hidden = !inst.padvTimeSync;
}

/*
 * ======== validate ========
 * Validate this inst's configuration
 *
 * @param inst       - Module instance to be validated
 * @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    if (inst.padvTimeSync)
    {
        // Validate Advertising SID range (0-15)
        if (inst.padvTimeSyncAdvSID < 0 || inst.padvTimeSyncAdvSID > 15)
        {
            validation.logError("Advertising SID must be between 0 and 15", inst, "padvTimeSyncAdvSID");
        }

        // Validate periodic advertising intervals for TSA
        if (inst.padvTimeSyncRole === "APP_PADV_TIME_SYNC_ROLE_TSA")
        {
            if (inst.padvTimeSyncPeriodicAdvIntervalMin < 6)
            {
                validation.logError("Minimum periodic advertising interval must be at least 6 (7.5ms)", inst, "padvTimeSyncPeriodicAdvIntervalMin");
            }

            if (inst.padvTimeSyncPeriodicAdvIntervalMax < inst.padvTimeSyncPeriodicAdvIntervalMin)
            {
                validation.logError("Maximum interval must be greater than or equal to minimum interval", inst, "padvTimeSyncPeriodicAdvIntervalMax");
            }
        }

        // Validate TSA name (required for both roles)
        if (inst.padvTimeSyncTsaName.length === 0)
        {
            validation.logError("TSA Device Name cannot be empty", inst, "padvTimeSyncTsaName");
        }

        if (inst.padvTimeSyncTsaName.length > 29)
        {
            validation.logError("TSA Device Name must be 29 characters or less", inst, "padvTimeSyncTsaName");
        }

        // Validate sync timeout for TSO
        if (inst.padvTimeSyncRole === "APP_PADV_TIME_SYNC_ROLE_TSO")
        {
            if (inst.padvTimeSyncTimeout < 10)
            {
                validation.logError("Sync timeout must be at least 10 (100ms)", inst, "padvTimeSyncTimeout");
            }
        }

        // Validate role matches device role
        const isTSA = inst.padvTimeSyncRole === "APP_PADV_TIME_SYNC_ROLE_TSA";
        const isTSO = inst.padvTimeSyncRole === "APP_PADV_TIME_SYNC_ROLE_TSO";

        if (isTSA && !inst.periodicAdv)
        {
            validation.logError("TSA role requires Periodic Advertising to be enabled", inst, "padvTimeSyncRole");
        }

        if (isTSO && !inst.periodicAdvSync)
        {
            validation.logError("TSO role requires Periodic Advertising Sync to be enabled", inst, "padvTimeSyncRole");
        }
    }
}

/*
 *  ======== getOpts ========
 * Generate compiler defines for PADV Time Sync
 */
function getOpts(mod)
{
    let inst = mod.$static;
    let result = [];

    if (inst.padvTimeSync)
    {
        // Add time sync enabled flag
        result.push("-DTIME_SYNC=1");

        // Add time sync role define
        result.push("-DAPP_TIME_SYNC_ROLE=" + inst.padvTimeSyncRole);

        // Add advertising SID define
        result.push("-DAPP_PADV_TIME_SYNC_ADV_SID=" + inst.padvTimeSyncAdvSID);

        if (inst.padvTimeSyncRole === "APP_PADV_TIME_SYNC_ROLE_TSA")
        {
            // TSA-specific defines
            result.push("-DAPP_PADV_TIME_SYNC_PERIODIC_ADV_INTERVAL_MIN=" + inst.padvTimeSyncPeriodicAdvIntervalMin);
            result.push("-DAPP_PADV_TIME_SYNC_PERIODIC_ADV_INTERVAL_MAX=" + inst.padvTimeSyncPeriodicAdvIntervalMax);
        }
        else if (inst.padvTimeSyncRole === "APP_PADV_TIME_SYNC_ROLE_TSO")
        {
            // TSO-specific defines
            result.push("-DAPP_PADV_TIME_SYNC_TIMEOUT=" + inst.padvTimeSyncTimeout);
            result.push("-DAPP_PADV_TIME_SYNC_SKIP=" + inst.padvTimeSyncSkip);
        }
    }

    return result;
}

/*
 *  ======== exports ========
 *  Export the PADV Time Sync Configuration module
 */
exports = {
    config: config,
    validate: validate,
    getOpts: getOpts,
    onPadvTimeSyncChange: onPadvTimeSyncChange
};
