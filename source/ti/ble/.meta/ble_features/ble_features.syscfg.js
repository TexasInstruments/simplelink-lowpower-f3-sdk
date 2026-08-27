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
 *  ======== ble_features.syscfg.js ========
 *  BLE feature flags, split into Controller (link-layer) and
 *  Host (GAP/GATT/ATT/SMP/L2CAP) groups.
 */

"use strict";

// Get ble features long descriptions
const Docs = system.getScript("/ti/ble/ble_features/ble_features_docs.js");

// Get common Script
const Common = system.getScript("/ti/ble/ble_common.js");

// Get Adaptivity Script
const adaptivityScript = system.getScript("/ti/ble/adaptivity/ble_adaptivity");

// Get OAD Script and docs
const oadScript = system.getScript("/ti/ble/oad/ble_oad");
const OadDocs   = system.getScript("/ti/ble/oad/ble_oad_docs.js");

// Get Channel Sounding supported indication
const isNotChannelSoundingSupported = !Common.isChannelSoundingSupported();

const controllerFeaturesConfig = {
    displayName: "Controller Features",
    config: [
        {
            name: "extAdv",
            displayName: "Extended Advertising",
            description: "BLE5 extended advertising feature",
            longDescription: Docs.extAdvLongDescription,
            onChange: onExtAdvChange,
            default: false
        },
        {
            name: "periodicAdv",
            displayName: "Periodic Advertising",
            longDescription: Docs.periodicAdvLongDescription,
            onChange: onPeriodicAdvChange,
            hidden: true,
            default: false
        },
        {
            name: "periodicAdvSync",
            displayName: "Periodic Advertising Sync",
            longDescription: Docs.periodicAdvSyncLongDescription,
            onChange: onPeriodicAdvChange,
            hidden: true,
            default: false
        },
        {
            name: "PAwRScanner",
            displayName: "Periodic Advertising with Responses - Scanner",
            longDescription: Docs.PAwRScannerLongDescription,
            hidden: true,
            default: false
        },
        {
            name: "PAwRAdvertiser",
            displayName: "Periodic Advertising with Responses - Advertiser",
            longDescription: Docs.PAwRAdvertiserLongDescription,
            hidden: true,
            default: false
        },
        {
            name: "PASTReceiver",
            displayName: "Periodic Advertising Sync Transfer Receiver",
            longDescription: Docs.PASTReceiverLongDescription,
            hidden: true,
            default: false
        },
        {
            name: "PASTSender",
            displayName: "Periodic Advertising Sync Transfer Sender",
            longDescription: Docs.PASTSenderLongDescription,
            hidden: true,
            default: false
        },
        {
            name: "channelSounding",
            displayName: "Channel Sounding",
            default: false,
            hidden: isNotChannelSoundingSupported,
            onChange: onChannelSoundingChange
        },
        {
            name: "bleCoexEnable",
            displayName: "Coex",
            default: false,
            hidden: false
        },
        {
            name: "powerControl",
            displayName: "Power Control",
            default: false,
            hidden: false
        },
        {
            name: "connectionMonitorRole",
            displayName: "Connection Monitor",
            default: false,
            hidden: false
        },
        {
            name: "connectionHandover",
            displayName: "Connection Handover",
            default: false,
            hidden: false
        },
        {
            name: "rssiMonitor",
            displayName: "Rssi Monitor",
            default: false,
            hidden: false
        },
        {
            name: "legacyCmd",
            displayName: "Legacy CMD",
            longDescription: Docs.legacyCmdLongDescription,
            default: false,
            hidden: false
        },
        {
            name: "vendorSpecificCmd",
            displayName: "Vendor Specific CMD",
            longDescription: Docs.vendorSpecificCmdLongDescription,
            default: false,
            hidden: false
        },
        {
            name: "extVendorSpecificCmd",
            displayName: "Extended Vendor Specific CMD",
            longDescription: Docs.extVendorSpecificCmdLongDescription,
            default: false,
            hidden: false
        },
        {
            name: "adaptivity",
            displayName: "Adaptivity using DAA",
            longDescription: Docs.adaptivityLongDescription,
            hidden: false,
            default: false,
            onChange: onAdaptivityChange
        },
        {
            name: "hideAdaptivityGroup",
            default: true,
            hidden: true
        },
    ]
};

const hostFeaturesConfig = {
    displayName: "Host Features",
    config: [
        {
            name: "bondManager",
            displayName: "Bond Manager",
            description: "The Gap Bond Manager is always enabled",
            longDescription: Docs.bondManagerLongDescription,
            default: true,
            onChange: onBondManagerChange
        },
        {
            name: "disableConfig",
            displayName: "Disable Config",
            description: "Disable Configuration",
            onChange: ondisableConfigChange,
            default: false,
            hidden: true
        },
        {
            name: "L2CAPCOC",
            displayName: "L2CAP Connection Oriented Channels",
            default: false,
            longDescription: Docs.L2CAPCOCLongDescription,
            onChange: onL2CAPCOCChange,
            hidden: false
        },
        {
            name: "gattDB",
            displayName: "GATT Database Off Chip",
            description: "Indicates that the GATT database is maintained off the chip on the"
                            + "Application Processor (AP)",
            longDescription: Docs.gattDBLongDescription,
            default: false
        },
        {
            name: "gattNoClient",
            displayName: "GATT No Client",
            description: "The app must have GATT client functionality "
                       + "to read the Resolvable Private Address Only "
                       + "characteristic and the Central Address Resolution "
                       + "characteristic. To enable it, Uncheck GATT "
                       + "No Client.",
            longDescription: Docs.gattNoClientLongDescription,
            default: false,
            hidden: false
        },
        {
            name: "delayingAttReadReq",
            displayName: "Delaying An ATT Read Request",
            longDescription: Docs.delayingAttReadReqLongDescription,
            default: false,
            hidden: false
        },
        {
            name: "healthToolkit",
            displayName: "Health Toolkit",
            longDescription: Docs.bleHealthLongDescription,
            default: false,
            hidden: true
        },
        {
            name: "hideHealthToolkit",
            displayName: "Hide Health Toolkit",
            default: true,
            hidden: true,
            description: "Used to hide the Health Toolkit configurable. Always hidden",
            onChange: onHideHealthToolkitChange
        },
    ]
};

const appFeaturesConfig = {
    name: "appFeaturesGroup",
    displayName: "Application Features",
    config: [
        {
            name: "enableOad",
            displayName: "Enable OAD",
            description: "Enable Over-the-Air Download functionality",
            longDescription: OadDocs.enableOadLongDescription,
            default: false,
            hidden: !oadScript.isOadSupported,
            onChange: onEnableOadChange
        },
        {
            name: "appExternalControlMode",
            displayName: "App External Control Mode",
            default: false,
            hidden: true
        },
        {
            name: "nwpMode",
            displayName: "Network Processor Mode",
            default: false,
            hidden: true
        },
        {
            name: "enableGattBuilder",
            displayName: "Enable GATT Builder",
            default: false,
            hidden: true,
            onChange: onEnableGattBuilderChange
        },
        {
            name: "gattBuilder",
            displayName: "Custom GATT",
            description: "Adding services and characteristic",
            default: false,
            hidden: true
        }
    ]
};

const config = {
    name: "bleFeatures",
    displayName: "BLE Features",
    description: "BLE Stack Features",
    config: [controllerFeaturesConfig, hostFeaturesConfig, appFeaturesConfig]
};


/*
 *  ======== changeGroupsState ========
 * Hide/Unhide groups, according to the selected features/deviceRole
 *
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function changeGroupsState(inst, ui)
{
    const modes = Common.getInstModes(inst);

    // Bond Manager group — present in full BLE module (bondMgrConfig) and ble_host
    if("hideBondMgrGroup" in inst)
    {
        inst.hideBondMgrGroup = !modes.isConnectable || !inst.bondManager;
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "bondMgrConfig"), inst.hideBondMgrGroup, ui);
        // Also hide the Bond Manager stack group when it exists (full BLE two-group layout)
        const bondMgrStackGroup = Common.getGroupByName(inst.$module.config, "bondMgrStackGroup");
        if(bondMgrStackGroup) Common.hideGroup(bondMgrStackGroup, inst.hideBondMgrGroup, ui);
    }

    // Role-specific groups — full BLE module only
    if("hidePeripheralGroup" in inst)
    {
        inst.hidePeripheralGroup = !modes.peripheral;
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "peripheralConfig"), inst.hidePeripheralGroup, ui);
    }
    if("hideBroadcasterGroup" in inst)
    {
        inst.hideBroadcasterGroup = !modes.isAdvertising;
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "broadcasterConfig"), inst.hideBroadcasterGroup, ui);
    }
    if("hideCentralGroup" in inst)
    {
        inst.hideCentralGroup = !modes.central;
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "centralConfig"), inst.hideCentralGroup, ui);
    }
    if("hideObserverGroup" in inst)
    {
        inst.hideObserverGroup = !modes.isScanning;
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "observerConfig"), inst.hideObserverGroup, ui);
    }

    // Host features — only relevant for connectable roles
    ui.bondManager.hidden = !modes.isConnectable;
    ui.gattDB.hidden = !modes.isConnectable;
    ui.gattNoClient.hidden = !modes.isConnectable;
    ui.delayingAttReadReq.hidden = !modes.isConnectable;
    ui.L2CAPCOC.hidden = !modes.isConnectable;

    // Reset host feature values when hidden to prevent stale defines from being emitted
    if(!modes.isConnectable)
    {
        inst.bondManager = false;
        inst.gattDB = false;
        inst.gattNoClient = false;
        inst.delayingAttReadReq = false;
        inst.L2CAPCOC = false;
    }

    // L2CAP config group — full BLE module only, visible when connectable and L2CAPCOC is selected
    if("hideL2CAPGroup" in inst)
    {
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "l2capConfig"), !modes.isConnectable || !inst.L2CAPCOC, ui);
    }

    // In host mode the entire Application Settings group must stay hidden,
    // and all role-specific hide-flags must be true so templates and
    // moduleInstances guards that check these flags also behave correctly.
    if(inst.host)
    {
        if("hidePeripheralGroup"  in inst) inst.hidePeripheralGroup  = true;
        if("hideBroadcasterGroup" in inst) inst.hideBroadcasterGroup = true;
        if("hideCentralGroup"     in inst) inst.hideCentralGroup     = true;
        if("hideObserverGroup"    in inst) inst.hideObserverGroup    = true;
        if("hideL2CAPGroup"       in inst) inst.hideL2CAPGroup       = true;
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "appSettings"), true, ui);
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "appFeaturesGroup"), true, ui);
    }
}

/*
 *  ======== onBondManagerChange ========
 * Show or hide the bond manager group
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onBondManagerChange(inst, ui)
{
    changeGroupsState(inst, ui);
}

/*
 *  ======== ondisableConfigChange ========
 * When using exclude SM then GapBondMgr should be removed.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function ondisableConfigChange(inst, ui)
{
    if(inst.disableConfig == true)
    {
        inst.bondManager = false;
    }
    else
    {
        inst.bondManager = true;
    }

    changeGroupsState(inst, ui);
}

/*
 *  ======== onExtAdvChange ========
 * Lock or unlock the deviceRole configurable,
 * disable/enable the option to change the deviceRole.
 * Defensive: works in both full BLE module (with deviceRole) and
 * standalone controller module (without deviceRole).
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onExtAdvChange(inst, ui)
{
    const modes = Common.getInstModes(inst);

    // Hide/UnHide periodicAdv if extended advertising is enabled and the instance has advertising capability
    ui.periodicAdv.hidden = !(inst.extAdv && modes.isAdvertising);
    if(ui.periodicAdv.hidden) inst.periodicAdv = false;

    // Hide/UnHide periodicAdvSync if extended advertising is enabled and the instance has scanning capability
    ui.periodicAdvSync.hidden = !(inst.extAdv && modes.isScanning);
    if(ui.periodicAdvSync.hidden) inst.periodicAdvSync = false;

    // Call onPeriodicAdvChange to ensure that any UI elements dependent on both
    // extAdv and periodicAdvSync are properly updated when extAdv is toggled.
    onPeriodicAdvChange(inst, ui);
}

/*
 *  ======== onPeriodicAdvChange ========
 * Updates UI options for Periodic Advertising features based on
 * current device role and settings.
 * Defensive: works in both full BLE module (with deviceRole/padvTimeSync) and
 * standalone controller module (without those).
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onPeriodicAdvChange(inst, ui)
{
    const modes = Common.getInstModes(inst);

    // Hide/UnHide PAwR Scanner if extended advertising and periodic sync advertising
    // are enabled and the instance has scanning capability
    ui.PAwRScanner.hidden = !(inst.extAdv && inst.periodicAdvSync && modes.isScanning);

    // Hide/UnHide PAwR Advertiser if extended advertising and periodic advertising
    // are enabled and the Broadcaster/Peripheral roles is used
    ui.PAwRAdvertiser.hidden = !(inst.extAdv && inst.periodicAdv && modes.isAdvertising);

    // Hide/UnHide PASTReceiver if extended advertising and
    // periodic advertising are enabled and the instance has peripheral/advertising capability
    ui.PASTReceiver.hidden = !(inst.extAdv && inst.periodicAdv && modes.peripheral);

    // Hide/UnHide PASTSender if extended advertising and periodic sync advertising
    // are enabled with central capability, or periodic advertising with peripheral capability
    ui.PASTSender.hidden = !(inst.extAdv && ((inst.periodicAdvSync && modes.central)
                                           || (inst.periodicAdv   && modes.peripheral)));

    if(ui.PAwRScanner.hidden)    inst.PAwRScanner    = false;
    if(ui.PAwRAdvertiser.hidden) inst.PAwRAdvertiser = false;
    if(ui.PASTReceiver.hidden)   inst.PASTReceiver   = false;
    if(ui.PASTSender.hidden)     inst.PASTSender     = false;

    // Hide/UnHide PADV Time Sync (only present in full BLE module, never in host mode)
    if("padvTimeSync" in inst)
    {
        const showPadvTimeSync = inst.extAdv && (inst.periodicAdv || inst.periodicAdvSync) && !inst.host;
        ui.padvTimeSync.hidden = !showPadvTimeSync;

        // Reset and hide PADV Time Sync settings when disabled
        if(!showPadvTimeSync)
        {
            inst.padvTimeSync = false;
            inst.hidePadvTimeSyncGroup = true;
            Common.hideGroup(Common.getGroupByName(inst.$module.config, "padvTimeSyncConfig"), true, ui);
        }
        else
        {
            // Trigger padv_time_sync's hidePadvTimeSyncGroup onChange, which
            // calls onPadvTimeSyncChange to re-sync sub-element visibility.
            inst.hidePadvTimeSyncGroup = false;
        }
    }
}

/*
 *  ======== onChannelSoundingChange ========
 * Handles the change of the channelSounding configuration.
 * Defensive: CS group and UI elements are only present in the full BLE module.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onChannelSoundingChange(inst, ui)
{
    // Channel Sounding group (only present in full BLE module via csScript.config)
    if("hideChannelSoundingGroup" in inst)
    {
        inst.hideChannelSoundingGroup = !inst.channelSounding;
        // App CS group stays hidden in host mode
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "channelSoundingConfig"),
                         inst.hideChannelSoundingGroup || inst.host, ui);
        // Stack CS group shows/hides based on channelSounding regardless of host mode
        const csStackGroup = Common.getGroupByName(inst.$module.config, "csStackGroup");
        if(csStackGroup) Common.hideGroup(csStackGroup, inst.hideChannelSoundingGroup, ui);

        // App-level CS UI elements — only present in the full BLE module
        if("rangingServerExtCtrlMode" in inst)   ui.rangingServerExtCtrlMode.hidden = true;
        if("rangingServerRealTimeFeature" in inst) ui.rangingServerRealTimeFeature.hidden = true;
        if("rangingClientExtCtrlMode" in inst)   ui.rangingClientExtCtrlMode.hidden = true;
        if("rangingClientMode" in inst)          ui.rangingClientMode.hidden = true;
        if("csMeasureResultsMode" in inst)       ui.csMeasureResultsMode.hidden = inst.csMeasureDistance === false;

        // The antenna max values should always be hidden
        ui.antennasMuxValues.hidden = true;

        // Reset antennas values
        inst.numAntennas = 1;
        inst.antennasMuxValues = 0x0;
    }
}

/*
 *  ======== onHideHealthToolkitChange ========
 * disable/enable the Health Toolkit configurable.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onHideHealthToolkitChange(inst, ui)
{
    inst.hideHealthToolkit ? ui.healthToolkit.hidden = true :
                             ui.healthToolkit.hidden = false;
}

/*
 *  ======== onL2CAPCOCChange ========
 * Add/remove the L2CAP module
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onL2CAPCOCChange(inst, ui)
{
    if("hideL2CAPGroup" in inst)
    {
        inst.hideL2CAPGroup = !inst.L2CAPCOC || inst.host;
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "l2capConfig"), inst.hideL2CAPGroup, ui);
    }
}

/*
 *  ======== onEnableOadChange ========
 * Delegate OAD visibility updates to the OAD module.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onEnableOadChange(inst, ui)
{
    oadScript.updateOadVisibility(inst, ui);
}

/*
 *  ======== onEnableGattBuilderChange ========
 * Lock or unlock the enableGattBuilder configurable,
 * disable/enable the option to change the enableGattBuilder.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onEnableGattBuilderChange(inst, ui)
{
    inst.enableGattBuilder ? ui.gattBuilder.hidden = false :
                             ui.gattBuilder.hidden = true;
}

/*
 *  ======== onAdaptivityChange ========
 * Show or hide adaptivity sub-options when the master adaptivity toggle changes.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onAdaptivityChange(inst, ui)
{
    if("hideAdaptivityGroup" in inst)
    {
        inst.hideAdaptivityGroup = !inst.adaptivity;
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "adaptivityConfig"), inst.hideAdaptivityGroup, ui);
    }
}

/*
 * ======== validate ========
 * Validate this inst's configuration
 *
 * @param inst       - BLE instance to be validated
 * @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    if(inst.healthToolkit)
    {
        validation.logWarning("Health Toolkit preview", inst, "healthToolkit");
    }

    if(inst.connectionHandover)
    {
        validation.logWarning("Connection Handover preview", inst, "connectionHandover");
    }

    adaptivityScript.validate(inst, validation);
}

/*
 *  ======== getControllerOpts ========
 * Returns compiler defines for BLE Controller (link-layer) feature flags.
 * Does not include HOST_CONFIG (host-only define).
 */
function getControllerOpts(mod)
{
    const inst = mod.$static;
    let result = [];

    inst.extAdv && result.push("-DUSE_AE");

    if(inst.extAdv)
    {
        inst.periodicAdv && result.push("-DUSE_PERIODIC_ADV");
        inst.periodicAdvSync && result.push("-DUSE_PERIODIC_SCAN");

        if(inst.periodicAdvSync)
        {
            inst.PAwRScanner && result.push("-DUSE_PAWR_SCAN");
        }

        if(inst.periodicAdv)
        {
            inst.PAwRAdvertiser && result.push("-DUSE_PAWR_ADV");
            inst.PASTReceiver && result.push("-DUSE_PAST_RECEIVER");
        }

        if(inst.periodicAdvSync || inst.periodicAdv)
        {
            inst.PASTSender && result.push("-DUSE_PAST_SENDER");
        }
    }

    inst.channelSounding && result.push("-DCHANNEL_SOUNDING");
    inst.bleCoexEnable && result.push("-DUSE_COEX");
    inst.powerControl && result.push("-DPOWER_CONTROL");
    inst.connectionMonitorRole && result.push("-DCONNECTION_MONITOR");
    inst.connectionHandover && result.push("-DCONNECTION_HANDOVER");
    inst.rssiMonitor && result.push("-DRSSI_MONITOR");
    inst.legacyCmd && result.push("-DLEGACY_CMD");
    inst.vendorSpecificCmd && result.push("-DVENDOR_SPECIFIC_CMD");
    inst.extVendorSpecificCmd && result.push("-DEXT_VENDOR_SPECIFIC_CMD");

    result.push(...adaptivityScript.getOpts(mod));

    return result;
}

/*
 *  ======== getHostOpts ========
 * Returns compiler defines for BLE Host (GAP/GATT/ATT/SMP/L2CAP) feature flags.
 * Does not include HOST_CONFIG — that is emitted by the calling module (ble or ble_host).
 */
function getHostOpts(mod)
{
    const inst = mod.$static;
    let result = [];

    inst.bondManager && result.push("-DGAP_BOND_MGR");
    inst.gattDB && result.push("-DGATT_DB_OFF_CHIP");
    inst.gattNoClient && result.push("-DGATT_NO_CLIENT");
    inst.L2CAPCOC && result.push("-DV41_FEATURES=L2CAP_COC_CFG", "-DBLE_V41_FEATURES=V41_FEATURES");
    inst.delayingAttReadReq && result.push("-DATT_DELAYED_REQ");
    inst.gattBuilder && result.push("-DUSE_GATT_BUILDER");

    inst.healthToolkit && result.push("-DBLE_HEALTH");
    inst.appExternalControlMode && result.push("-DAPP_EXTERNAL_CONTROL");

    return result;
}

/*
 *  ======== getOpts ========
 * Returns compiler defines for all BLE feature flags (controller + host).
 * HOST_CONFIG is not included here — the calling module (ble) emits it.
 */
function getOpts(mod)
{
    return [
        ...getControllerOpts(mod),
        ...getHostOpts(mod)
    ];
}

// Exports to the top level BLE module
exports = {
    config: config,
    controllerFeaturesConfig: controllerFeaturesConfig,
    hostFeaturesConfig: hostFeaturesConfig,
    appFeaturesConfig: appFeaturesConfig,
    adaptivityConfigItems: adaptivityScript.adaptivityConfigItems,
    validate: validate,
    getOpts: getOpts,
    getControllerOpts: getControllerOpts,
    getHostOpts: getHostOpts,
    changeGroupsState: changeGroupsState,
    onExtAdvChange: onExtAdvChange,
    onPeriodicAdvChange: onPeriodicAdvChange
};
