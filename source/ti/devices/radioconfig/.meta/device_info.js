/*
 * Copyright (c) 2022-2023 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== device_info.js ========
 *  Device information database
 */

"use strict";

// Common utility functions
const Common = system.getScript("/ti/devices/radioconfig/radioconfig_common.js");

// RF Driver patch information file
const PatchInfo = Common.isPlatformRFD()
    ? system.getScript("/ti/devices/radioconfig/config/rfd/patch_info.json") : null;

// Global path to Radio configuration root
const ConfigPath = Common.ConfigPath;
const ConfigPathRclCommon = Common.ConfigPathRclCommon;

// Mapping SysCfg device name notation to SmartRF Studio format
const DevNameMap = system.getScript("/ti/devices/radioconfig/data/device_name_map.json");

// SmartRF Studio compatible device name
const DeviceName = DevNameMap[Common.Device] || "none";
const DeviceSupported = DeviceName !== "none";

// True if High PA device
const HighPaDevice = DeviceName.includes("cc1352p")
    || DeviceName.includes("cc1354p")
    || DeviceName.includes("cc2674p")
    || DeviceName.includes("cc2653p")
    || DeviceName.includes("cc1312psip")
    || DeviceName.includes("cc2652p")
    || DeviceName.includes("cc2672p")
    || DeviceName.includes("cc1311p")
    || DeviceName.includes("cc2651p");

// True if wBMS support
const WbmsSupport = DeviceName === "cc2642r" || DeviceName === "cc2652r";

/*
 * Global device information
 */
const DevInfo = {
    // Path to the configuration data for the device
    devicePath: ConfigPath + DeviceName + "/",
    phyGroup: {
        prop: {},
        ble: {},
        ieee_15_4: {}
    },
    // PHY name: "ble", "prop" or "ieee_15_4"
    phy: "",
    // Board name on SmartRF Studio format
    target: "",
    // True if device supports High PA
    highPaSupport: HighPaDevice,
    // Code export config
    ceConfig: {}
};

// Load the device configuration database
if (DeviceSupported) {
    const fileName = ConfigPathRclCommon + `param_syscfg_codeexport_${DeviceName}.json`;
    DevInfo.ceConfig = system.getScript(fileName);
}

/**
 *  ======== loadConfiguration ========
 *  Load configuration data of a PHY group
 *
 * @param phy - ble, prop, ieee_154
 * @returns the device configuration options for a given device
 */
function loadConfiguration(phy) {
    let phyGroup = phy;

    // TBD: fix naming inconsistency
    if (phyGroup.includes("ieee")) {
        phyGroup = "ieee";
    }
    const fileName = ConfigPath + `param_syscfg_${phyGroup}_${DeviceName}.json`;
    const devCfg = system.getScript(fileName);

    return devCfg;
}

/**
 * ======== createSettingMap ========
 * Create list of PHY settings for a PHY group
 *
 * @param phyGroup - ble, prop or ieee_154
 * @returns New list of settings
 */
function createSettingMap(phyGroup) {
    const data = DevInfo.phyGroup[phyGroup].config;

    if (phyGroup === Common.PHY_IEEE_15_4) {
        return data.phys.ieee;
    }
    else if (phyGroup === Common.PHY_BLE) {
        return data.phys.ble;
    }
    else if (phyGroup === Common.PHY_PROP) {
        let settingMap = [];
        if (Common.isSub1gDevice()) {
            settingMap = settingMap.concat(data.phys.prop868);
            if ("prop433" in data.phys) {
                settingMap = settingMap.concat(data.phys.prop433);
            }
            if ("prop169" in data.phys) {
                settingMap = settingMap.concat(data.phys.prop169);
            }
        }
        if (Common.HAS_24G_PROP) {
            settingMap = settingMap.concat(data.phys.prop2400);
        }
        return settingMap;
    }
    throw Error("Unknown PHY group: ", phyGroup);
}

/**
 *  ======== getVersionInfo ========
 *  Get version information for the RadioConfig tool and the RF settings.
 *  @returns Radioconfig8 version info
 */
function getVersionInfo() {
    const version = system.getScript(ConfigPath + "version_info.json");

    // Currently 1-to-1. Some processing may be introduced before the final release,
    // (removing patch and build number).
    return {
        toolVersion: version.radioconfig,
        settingsVersion: version.rfconfig,
        sdkVersion: version.sdk
    };
}

/**
 * ======== addPhyGroup ========
 * Initialize the database for the specified PHY group
 *
 * @param phyGroup - PHY group short name (prop, ble, or ieee_15_4)
 */
function addPhyGroup(phyGroup) {
    const phyInfo = {
        phy: phyGroup,
        phyPath: ConfigPath,
        phyDir: ConfigPath,
        paramPath: ConfigPath
    };
    DevInfo.phyGroup[phyGroup] = phyInfo;
    phyInfo.config = loadConfiguration(phyGroup);
    phyInfo.settings = createSettingMap(phyGroup);
}

/**
 * ======== getDeviceFamily ========
 * Return the device family of the current device.
 *
 * @returns Device family of current device
 */
function getDeviceFamily() {
    let devFamily = null;

    if (Common.isDeviceClass3()) {
        devFamily = "cc13x1_cc26x1";
    }
    else if (Common.isDeviceClass7()) {
        devFamily = "cc13x2x7_cc26x2x7";
    }
    else if (Common.isDeviceClass10()) {
        devFamily = "cc13x4_cc26x4";
    }
    else if (Common.isDeviceStandard()) {
        devFamily = "cc13x2_cc26x2";
    }
    return devFamily;
}

/**
 * ======== loadCommands ========
 * Load the RFD/RCL commands from the command description files.
 *
 * @returns RFD or RCL commands depending on platform
 */
function loadCommands() {
    if (Common.isPlatformRFD()) {
        return system.getScript(ConfigPath + "all.rfdcmds.json");
    }

    return system.getScript(ConfigPathRclCommon + "all.rclcmds.json");
}

/**
 * ======== getPatchInfo ========
 * Return the patch list for the current device.
 * @returns Patch list for current device
 */
function getPatchInfo() {
    if (PatchInfo == null) {
        // Not applicable for RCL
        return [];
    }

    const devFamily = getDeviceFamily();

    if (devFamily) {
        return PatchInfo[devFamily];
    }
    return [];
}

/**
 * ======== setPatchInfo ========
 * Update the current patch information
 *
 *  @param patches - List of current patches in the SDK
 */
function setPatchInfo(patches) {
    if (PatchInfo == null) {
        // Not applicable for RCL
        return;
    }
    const devFamily = getDeviceFamily();

    if (devFamily) {
        PatchInfo[devFamily] = patches;
    }
}

/**
 *  ======== getFrontendSettings ========
 *  Get the current frontend settings.
 *
 *  @param phyGroup - BLE, IEEE_15_4, PROP
 *  @param id       - front-end identifier (ID, XD, XS_RFP ...)
 *  @returns Current frontend settings
 */
function getFrontendSettings(phyGroup, id) {
    // Get frontend settings
    const devCfg = DevInfo.phyGroup[phyGroup].config;

    for (const feEntry of devCfg.frontends) {
        if (id === feEntry.name) {
            return feEntry;
        }
    }
    return null;
}

/**
 *  ======== getDeviceName ========
 *  Get the Smart RF Studio name of the currently selected device
 *
 * @returns The name of the current device as mapped by DevNameMap
 */
function getDeviceName() {
    if (DeviceName === "none") {
        throw Error(`Device: ${Common.Device} is not supported right now, unable to generate code`);
    }
    return DeviceName;
}

// Exported from this module
exports = {
    addPhyGroup: addPhyGroup,
    getVersionInfo: getVersionInfo,
    isDeviceSupported: () => DeviceSupported,
    getConfiguration: (phyGroup) => DevInfo.phyGroup[phyGroup].config,
    getCodeExportConfig: () => DevInfo.ceConfig,
    getSettingMap: (phyGroup) => DevInfo.phyGroup[phyGroup].settings,
    getDeviceName: getDeviceName,
    hasHighPaSupport: () => DevInfo.highPaSupport,
    hasWbmsSupport: () => WbmsSupport,
    getDeviceFamily: getDeviceFamily,
    setPatchInfo: setPatchInfo,
    getPatchInfo: getPatchInfo,
    loadCommands: loadCommands,
    getFrontendSettings: getFrontendSettings
};
