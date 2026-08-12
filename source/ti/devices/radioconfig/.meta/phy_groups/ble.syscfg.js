/*
 * Copyright (c) 2019-2023 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ble.syscfg.js ========
 *  Radio configuration support for BLE settings.
 */

"use strict";

/* Common utility functions */
const Common = system.getScript("/ti/devices/radioconfig/radioconfig_common.js");

/* Device information */
const DevInfo = Common.getScript("device_info.js");

/* PHY group */
const PHY_GROUP = Common.PHY_BLE;

/* Base module for RF Settings */
const RFBase = Common.getScript("radioconfig");

/* PHY handler */
const PhyHandler = Common.getPhyHandler();

/* Documentation */
const BleDocs = Common.getScript("phy_groups/ble_docs.js");
const SharedDocs = Common.getScript("phy_groups/shared_docs.js");

/* Setting specific configurable */
const tmp = DevInfo.getConfiguration(PHY_GROUP);
const config = _.cloneDeep(tmp.configs);

let hasWBMS = false;

// Special handling for PHY with RF commands
const VisibleWithRfCmd = [
    "txPower", "frequency"
];

const bleSpecific = {
    displayName: "RF Settings BLE",
    description: "RF Settings BLE",
    longDescription: "RF Settings module for Bluetooth Low Energy protocol",
    validate: validate,
    hasWBMS: () => hasWBMS,
    phyGroup: PHY_GROUP,
    config: config
};

/**
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  @param inst       - RF Settings instance to be validated
 *  @param validation - Issue reporting object
 */
function validate(inst, validation) {
    /* Validation common to all PHY groups */
    Common.validateBasic(inst, validation);
    const phyType = inst.phyType;
    hasWBMS = phyType.includes("wbms");

    if (inst.paramVisibility && !hasWBMS) {
        RFBase.validatePhyProperty(inst, validation, phyType, PHY_GROUP);
    }
    RFBase.validateFrontendSettings(inst, validation, PHY_GROUP);

    // Check if a wBMS setting is included
    const instances = inst.$module.$instances;
    for (let i = 0; i < instances.length && !hasWBMS; i++) {
        if (instances[i].phyType.includes("wbms")) {
            hasWBMS = true;
        }
    }
    // Make sure that RF commands are updated
    const phyGroup = Common.getPhyGroup(inst);
    PhyHandler.getUpdatedRfCommands(inst, phyGroup);
}

/*
 *  ======== phyTypeOnChange ========
 *  Change handler for PHY type configurable
 *
 */
function phyTypeOnChange(inst, ui) {
    const phyType = inst.phyType;
    const phyHandler = PhyHandler.get(PHY_GROUP, inst.phyType);

    updatePDUConfig(inst, ui);

    // Refresh the instance
    RFBase.reloadInstanceFromPhy(inst, ui, phyType, PHY_GROUP, ["txPower", "txPowerHi"]);

    // Hide whitening if wBMS setting
    hasWBMS = phyType.includes("wbms");
    if ("whitening" in ui) {
        ui.whitening.hidden = hasWBMS;
    }

    if ("settingGroup" in ui) {
        // The sensitivity v selectivity choice only applies to the BLE 2M PHY
        ui.settingGroup.hidden = !phyType.includes("bt5le2m");
    }

    // Update PHY property visibility
    for (const cfg of VisibleWithRfCmd) {
        if (cfg in ui) {
            const visible = phyHandler.isPhyPropSupported(inst, cfg);
            ui[cfg].hidden = !visible;
        }
    }

    if ("codedTxRate" in ui) {
        ui.codedTxRate.hidden = !phyHandler.hasFeatureGroup("coded_tx_rate");
    }
    if ("subPhy" in ui) {
        ui.subPhy.hidden = !phyHandler.hasFeatureGroup("sub_phy");
    }
}

/**
 *  ======== onPermissionChange ========
 *  Change permission according to permission configurable
 */
function onPermissionChange(inst, ui) {
    // PHY type:
    // - always ReadOnly with a Custom stack
    // - otherwise controlled by the 'permission' configurable
    const freqReadOnly = inst.permission === "ReadOnly" || inst.parent === "Custom";
    ui.phyType.readOnly = freqReadOnly;
}

/**
 *  ======== onVisibilityChange ========
 *  Change visibility of RF parameters
 */
function onVisibilityChange(inst, ui) {
    const phyHandler = PhyHandler.get(PHY_GROUP, inst.phyType);
    const rfData = phyHandler.getRfData();
    const hidden = !inst.paramVisibility;

    _.each(rfData, (value, key) => {
        if (!("highPA" in inst && key.includes("txPower"))) {
            ui[key].hidden = hidden;
        }
    });
}

/**
 *  ======== updatePDUConfig ========
 *  Either shows or hides the PDU config if the phyType supports it
 */
function updatePDUConfig(inst, ui) {
    if ("packetLengthBle" in ui) {
        // Only for BT4
        ui.packetLengthBle.hidden = !inst.phyType.includes("bt5le1madvnc");
    }
}

/**
 *  ======== initConfigurables ========
 *  Initialize configurables that are not completed in pre-processing script.
 *
 *  @param configurables - configurables to act on
 */
function initConfigurables(configurables) {
    // eslint-disable-next-line guard-for-in
    for (const idx in configurables) {
        const item = configurables[idx];

        switch (item.name) {
        case "highPA":
            item.onChange = RFBase.highPaOnChange;
            break;
        case "phyType":
        case "settingGroup":
            item.onChange = phyTypeOnChange;
            break;
        case "permission":
            item.onChange = onPermissionChange;
            break;
        case "paramVisibility":
            item.onChange = onVisibilityChange;
            break;
        default:
            break;
        }
    }
}

/**
 *  ======== extend ========
 *  Extends BLE object to include RF Basic module
 *
 */
function extend(base) {
    /* Initialize state of UI elements (readOnly when appropriate) */
    Common.initLongDescription(bleSpecific.config, BleDocs.bleDocs);
    Common.initLongDescription(bleSpecific.config, SharedDocs.sharedDocs);

    /* Initialize configurables */
    initConfigurables(bleSpecific.config);

    /* Make sure our copy of configurables is updated */
    const cmdHandler = PhyHandler.get(PHY_GROUP, config[0].default);
    cmdHandler.initConfigurables(bleSpecific.config);

    /* Remove invalid elements from configurables */
    RFBase.pruneConfig(bleSpecific.config);

    return {...base, ...bleSpecific};
}

exports = extend(RFBase);
