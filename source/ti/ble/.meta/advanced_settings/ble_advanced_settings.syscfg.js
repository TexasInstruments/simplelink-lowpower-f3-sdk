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
 *  ======== ble_advanced_settings.syscfg.js ========
 */

"use strict";

// Get long descriptions
const Docs = system.getScript("/ti/ble/advanced_settings/ble_advanced_settings_docs.js");

// Get common Script
const Common = system.getScript("/ti/ble/ble_common.js");

// App-only configurable — included only by the full BLE module (ble.syscfg.js)
const peerConnParamUpdateRejectIndConfig = {
    name: "peerConnParamUpdateRejectInd",
    displayName: "Peer Conn Param Update Reject Ind",
    longDescription: Docs.peerConnParamUpdateRejectIndLongDescription,
    hidden: false,
    default: false
};

// Hidden items — not in any display group
const noOsalSnvConfig = {
    name: "noOsalSnv",
    displayName: "No Osal SNV",
    description: "Deprecated. Use Persistent Storage instead.",
    default: false,
    hidden: true,
    deprecated: true
};

const useRcoscConfig = {
    name: "useRcosc",
    displayName: "Use RCOSC",
    default: false,
    hidden: true
};

// Debug sub-group items
const debugConfigItems = [
    {
        name: "halAssert",
        displayName: "Hal Assert",
        longDescription: Docs.halAssertLongDescription,
        default: false
    },
    {
        name: "enableLogging",
        displayName: "Enable Logging",
        longDescription: `This setting will enable logging for the BLE module.`,
        default: false,
        hidden: true,
        onChange: onEnableLoggingChange
    },
    {
        name: "logModules",
        displayName: "Log Modules",
        longDescription: `Select which BLE log modules to enable.`,
        default: [],
        hidden: true,
        minSelections: 0,
        options: [
            { name: "BLE_GENERAL",   displayName: "BLE General" },
            { name: "BLE_CONN",      displayName: "BLE Connection" },
            { name: "BLE_ADV",       displayName: "BLE Advertising" },
            { name: "BLE_SCAN",      displayName: "BLE Scanning" },
            { name: "BLE_INIT",      displayName: "BLE Initiating" },
            { name: "BLE_PADV",      displayName: "BLE Periodic Advertising" },
            { name: "BLE_CS",        displayName: "BLE Channel Sounding" },
            { name: "BLE_SCHEDULER", displayName: "BLE Scheduler" }
        ]
    },
    {
        name: "bleLogConfig",
        displayName: "BLE Log Modules",
        config: []
    },
];

// ICall Settings sub-group items
const icallSettingsConfigItems = [
    {
        name: "icallEvents",
        displayName: "ICALL Events",
        readOnly: true,
        hidden: true,
        default: true
    },
    {
        name: "maxNumEntIcall",
        displayName: "Max Num Of Entities That Use ICall",
        longDescription: Docs.maxNumEntIcallLongDescription,
        default: 6
    },
    {
        name: "maxNumIcallEnabledTasks",
        displayName: "Max Number of ICall Enabled Tasks",
        longDescription: Docs.maxNumIcallEnabledTasksLongDescription,
        default: 3
    }
];

// PTM sub-group items
const ptmConfigItems = [
    {
        name: "ptm",
        displayName: "PTM",
        longDescription: Docs.ptmLongDescription,
        hidden: false,
        default: false,
        onChange: onPtmChange
    },
    {
        name: "flowControl",
        displayName: "Flow Control",
        longDescription: Docs.flowControlLongDescription,
        default: "Disabled",
        hidden: true,
        options: [
            {
                name: "Disabled",
                legacyNames: ["0"]
            },
            {
                name: "Enabled",
                legacyNames: ["1"]
            }
        ]
    }
];

// Stack Settings items shared across all modules
const stackSettingsConfigItems = [
    {
        name: "extendedStackSettings",
        displayName: "Extended Stack Settings",
        description: "Deprecated. Use Central Guard Time instead.",
        default: "EXTENDED_STACK_SETTINGS_DEFAULT",
        hidden: true,
        deprecated: true,
        options: [
            {
                displayName: "Default",
                name: "EXTENDED_STACK_SETTINGS_DEFAULT"
            },
            {
                displayName: "Guard Time",
                name: "CENTRAL_GUARD_TIME_ENABLE",
                legacyNames: ["MASTER_GUARD_TIME_ENABLE"]
            }
        ]
    },
    {
        name: "centralGuardTime",
        displayName: "Central Guard Time",
        longDescription: Docs.extendedStackSettingsLongDescription,
        hidden: true,
        default: false
    },
    {
        name: "persistentStorage",
        displayName: "Persistent Storage",
        description: "Use NV pages for persistent storage (e.g. bond manager). "
                    + "Disable to exclude NV storage from the build.",
        default: true
    },
    {
        name: "peripheralExtraLFOSCPPM",
        displayName: "Peripheral Extra LFOSC PPM",
        longDescription: Docs.peripheralExtraLFOSCPPM,
        default: "1500",
        hidden: true,
    },
];

const config = {
    displayName: "Advanced Settings",
    description: "Configure BLE Build Settings",
    config: [
        // Hidden items — no group needed
        noOsalSnvConfig,
        useRcoscConfig,
        // Sub-groups
        { displayName: "Debug",           config: debugConfigItems          },
        { displayName: "Stack Settings",  config: stackSettingsConfigItems  },
        { displayName: "ICall Settings",  config: icallSettingsConfigItems  },
    ]
};

function onGenLibsChange(inst, ui)
{
    ui.enableLogging.hidden = inst.genLibs;
}


function onEnableLoggingChange(inst, ui)
{
    ui.logModules.hidden = !inst.enableLogging;
    if(!inst.enableLogging)
    {
        inst.logModules = [];
    }
}

function onPtmChange(inst, ui)
{
    ui.flowControl.hidden = !inst.ptm;

    // When PTM_MODE is selected change the number of ICALL tasks to 4
    if(inst.ptm)
    {
        inst.maxNumIcallEnabledTasks = 4;
        inst.vendorSpecificCmd = true;
        inst.extVendorSpecificCmd = true;
    }
    else
    {
        inst.maxNumIcallEnabledTasks = 3;
        inst.vendorSpecificCmd = false;
        inst.extVendorSpecificCmd = false;
    }
}


/*
 *  ======== moduleInstances ========
 *  Returns log module instances for each selected BLE log module.
 *  Called by all three BLE modules (ble, ble_host, ble_controller).
 *
 *  @param inst  - Module instance containing the config that changed
 *  @returns     - Array containing log module instance descriptors
 */
function moduleInstances(inst)
{
    let dependencyModule = [];

    // If logging is enabled, push a dependency on each selected log module.
    // args.$name must match the log module name used in the compilation units.
    if (inst.enableLogging === true) {
        let defaultLogSink = "ti/log/LogSinkBuf";
        if (system.deviceData.deviceId.match(/CC27/)) {
            defaultLogSink = "ti/log/LogSinkITM";
        }

        // Look up display names from the logModules configurable options
        const logModuleOptions = inst.$module.$configByName.logModules.options;

        const selectedModules = inst.logModules || [];
        for (const modName of selectedModules) {
            const option = logModuleOptions.find(opt => opt.name === modName);
            const displayName = option ? option.displayName : modName;
            dependencyModule.push(
                {
                    name: modName + "_LogModule",
                    group: "bleLogConfig",
                    displayName: displayName + " Log Configuration",
                    moduleName: "/ti/log/LogModule",
                    collapsed: true,
                    args: {
                        enable_DEBUG: false,
                        enable_INFO: true,
                        enable_VERBOSE: false,
                        enable_WARNING: true,
                        enable_ERROR: true,
                        loggerSink: defaultLogSink
                    },
                    requiredArgs: {
                        $name: modName
                    }
                }
            );
        }
    }

    return dependencyModule;
}

/*
 *  ======== modules ========
 *  Determines what modules are added as static submodules
 *
 *  @param inst  - Module instance containing the config that changed
 *  @returns     - Array containing a static dependency modules
 */
function modules(inst)
{
    let dependencyModule = [];

    if(inst.ptm && system.getRTOS() === "tirtos7")
    {
        dependencyModule.push({
            name: "gateMutex",
            displayName: "GateMutex",
            moduleName: "/ti/sysbios/gates/GateMutex",
            hidden: true
        });
    }

    return(dependencyModule);
}

/*
 * ======== validate ========
 * Validate this inst's configuration
 *
 * @param inst       - Build Config instance to be validated
 * @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    if(inst.maxNumEntIcall < 0)
    {
        validation.logError("The value must be a positive number", inst, "maxNumEntIcall");
    }

    if(inst.maxNumIcallEnabledTasks < 0)
    {
        validation.logError("The value must be a positive number", inst, "maxNumIcallEnabledTasks");
    }

    if(inst.ptm && inst.maxNumIcallEnabledTasks < 4)
    {
        validation.logError("The value must be >= 4", inst, "maxNumIcallEnabledTasks");
    }

    if((system.modules["/ti/devices/CCFG"] && (system.modules["/ti/devices/CCFG"].$static.srcClkLF == "LF RCOSC")) && ( inst.peripheralExtraLFOSCPPM < 0 || inst.peripheralExtraLFOSCPPM > 65535) )
    {
        validation.logError("The value must be greater than 0 or smaller than 65535 ", inst, "peripheralExtraLFOSCPPM");
    }
}

/*
 *  ======== getOpts ========
 */
function getOpts(mod)
{
    const inst = mod.$static;
    let result = [];

    result.push("-DEXTENDED_STACK_SETTINGS=" + (inst.centralGuardTime ? "CENTRAL_GUARD_TIME_ENABLE" : "EXTENDED_STACK_SETTINGS_DEFAULT"));
    result.push("-DICALL_MAX_NUM_ENTITIES=" + inst.maxNumEntIcall);
    result.push("-DICALL_MAX_NUM_TASKS=" + inst.maxNumIcallEnabledTasks);
    result.push("-DOSAL_CBTIMER_NUM_TASKS=1");

    inst.disableDisplayModule && result.push("-DDisplay_DISABLE_ALL");
    inst.halAssert && result.push("-DEXT_HAL_ASSERT");


    !inst.persistentStorage && result.push("-DNO_OSAL_SNV");
    inst.icallEvents && result.push("-DICALL_EVENTS");
    result.push("-DSRC_CLK_IS_LFOSC=" + (inst.useRcosc ? 1 : 0));
    result.push("-DUSER_CFG_LFOSC_EXTRA_PPM=" + inst.peripheralExtraLFOSCPPM);

    if((_.isEqual(inst.deviceRole, "PERIPHERAL_CFG")) || inst.ptm || inst.appExternalControlMode)
    {
        result.push("-DNPI_USE_UART");
    }
    if(inst.ptm)
    {
        let flowCtrlValue = inst.flowControl == "Disabled" ? 0 : 1;
        result.push("-DPTM_MODE", "-DNPI_FLOW_CTRL=" + flowCtrlValue);
    }

    return result;
}

// App-only configurable — included only by the full BLE module (ble.syscfg.js)
const disableDisplayModuleConfig = {
    name: "disableDisplayModule",
    displayName: "Disable The Display Module",
    longDescription: Docs.disableDisplayModuleLongDescription,
    default: false
};

// BLE-specific sub-group objects — include app-only items not present in
// the shared config. Used by the full BLE module (ble.syscfg.js) to build
// the Stack Settings / Application Settings two-group layout.
const bleDebugGroup          = { displayName: "Debug",          config: debugConfigItems         };
const bleStackSettingsGroup  = { displayName: "Stack Settings", config: [...stackSettingsConfigItems, peerConnParamUpdateRejectIndConfig] };
const bleICallSettingsGroup  = { displayName: "ICall Settings", config: icallSettingsConfigItems };
const blePtmGroup            = { displayName: "PTM",            config: ptmConfigItems           };
const bleHiddenConfigItems   = [noOsalSnvConfig, useRcoscConfig];

// Controller-specific config variant: centralGuardTime is visible by default
// because ctrlConfig defaults include INIT_CFG (Initiating/Central capability).
const ctrlStackSettingsConfigItems = stackSettingsConfigItems.map(item => {
    if(item.name === "centralGuardTime")  return Object.assign({}, item, { hidden: false });
    return item;
});
const ctrlConfig = {
    displayName: "Advanced Settings",
    description: "Configure BLE Build Settings",
    config: [
        noOsalSnvConfig,
        useRcoscConfig,
        { displayName: "Debug",           config: debugConfigItems              },
        { displayName: "Stack Settings",  config: ctrlStackSettingsConfigItems  },
        { displayName: "ICall Settings",  config: icallSettingsConfigItems      },
    ]
};

// BLE-only config variant — same as config but includes app-only configurables
// in their respective groups. Used only by the full BLE module (ble.syscfg.js).
const bleConfig = {
    displayName: "Advanced Settings",
    description: "Configure BLE Build Settings",
    config: [
        ...bleHiddenConfigItems,
        bleDebugGroup,
        bleStackSettingsGroup,
        bleICallSettingsGroup,
        blePtmGroup,
    ]
};

// Exports to the top level BLE module
exports = {
    config: config,
    ctrlConfig: ctrlConfig,
    bleConfig: bleConfig,
    // Individual sub-group exports for the full BLE module two-group layout
    bleDebugGroup: bleDebugGroup,
    disableDisplayModuleConfig: disableDisplayModuleConfig,
    bleStackSettingsGroup: bleStackSettingsGroup,
    bleICallSettingsGroup: bleICallSettingsGroup,
    blePtmGroup: blePtmGroup,
    bleHiddenConfigItems: bleHiddenConfigItems,
    onGenLibsChange: onGenLibsChange,
    moduleInstances: moduleInstances,
    modules: modules,
    validate: validate,
    getOpts: getOpts
};
