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

// Global path to Radio configuration root
const ConfigPath = Common.ConfigPath;
const ConfigPathRclCommon = Common.ConfigPathRclCommon;
const DeviceName = Common.DeviceName;
const DeviceSupported = DeviceName !== "none";

// Register definitions
const FileData = system.getScript(ConfigPathRclCommon + "rcl_registers.json");
let ModDef;
let RclRegisters;
let RclRegLookupAddr;

/**
 *  Get singleton module defintion for current device read from rcl_registers.json
 *
 * @returns Device module defintion
 */
function getModuleDefinition() {
    if (typeof ModDef === "undefined") {
        ModDef = FileData[Common.getDeviceFamily()].module;
    }
    return ModDef;
}

/**
 * Get singleton rcl registers for current device read from rcl_registers.json
 *
 * @returns RCL registers
 */
function getRclRegisters() {
    if (typeof RclRegisters === "undefined") {
        RclRegisters = FileData[Common.getDeviceFamily()].registers;
    }
    return RclRegisters;
}

/**
 * Get sorted RCL register list
 *
 * @returns RCL register list sorted by address
 */
function getRclRegLookupAddr() {
    if (typeof RclRegLookupAddr === "undefined") {
        RclRegLookupAddr = Object.entries(getRclRegisters()).sort(fnComp);
    }
    return RclRegLookupAddr;
}

/**
 * ======== fnComp ========
 * @param a - first register
 * @param b - second register
 * @returns 1 if a has a larger address, else -1
 */
function fnComp(a, b) {
    if (Number(a[1].addr) > Number(b[1].addr)) {
        return 1;
    }
    return -1;
}

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
        toolVersion: version.radioconfig8_source_version,
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
    phyInfo.config = Common.loadConfiguration(phyGroup);
    phyInfo.settings = createSettingMap(phyGroup);
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
    loadCommands: loadCommands,
    getFrontendSettings: getFrontendSettings,
    getModuleDefinition: getModuleDefinition,
    getRclRegisters: getRclRegisters,
    getRclRegLookupAddr: getRclRegLookupAddr
};
