/*
 * Copyright (c) 2024-2025, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== rclgpio.syscfg.js ========
 */

"use strict";

/* Common /ti/drivers utility functions */
const Common = system.getScript("/ti/drivers/Common.js");

/* Documentation for module config options */
const Docs = system.getScript("/ti/drivers/rcl/RCL_GPIO.docs.js");

let signalList = [
    {
        name: "RFEGPO0",
        displayName: Docs.config.rclPinOptions.paEnable.displayName,
        description: Docs.config.rclPinOptions.paEnable.description
    },
    {
        name: "RFEGPO1",
        displayName: Docs.config.rclPinOptions.lnaEnable.displayName,
        description: Docs.config.rclPinOptions.lnaEnable.description
    },
    {
        name: "RFEGPO2",
        displayName: Docs.config.rclPinOptions.rfeGpo2.displayName,
        description: Docs.config.rclPinOptions.rfeGpo2.description
    },
    {
        name: "RFEGPO3",
        displayName: Docs.config.rclPinOptions.rfeGpo3.displayName,
        description: Docs.config.rclPinOptions.rfeGpo3.description,
    },
    {
        name: "RFEGPO4",
        displayName: Docs.config.rclPinOptions.rfeGpo4.displayName,
        description: Docs.config.rclPinOptions.rfeGpo4.description
    },
    {
        name: "RFEGPO5",
        displayName: Docs.config.rclPinOptions.rfeGpo5.displayName,
        description: Docs.config.rclPinOptions.rfeGpo5.description
    },
    {
        name: "RFEGPO6",
        displayName: Docs.config.rclPinOptions.rfeGpo6.displayName,
        description: Docs.config.rclPinOptions.rfeGpo6.description
    },
    {
        name: "RFEGPO7",
        displayName: Docs.config.rclPinOptions.rfeGpo7.displayName,
        description: Docs.config.rclPinOptions.rfeGpo7.description
    },
    {
        name: "MCEGPO0",
        displayName: Docs.config.rclPinOptions.mceGpo0.displayName,
        description: Docs.config.rclPinOptions.mceGpo0.description
    },
    {
        name: "MCEGPO1",
        displayName: Docs.config.rclPinOptions.mceGpo1.displayName,
        description: Docs.config.rclPinOptions.mceGpo1.description
    },
    {
        name: "MCEGPO2",
        displayName: Docs.config.rclPinOptions.mceGpo2.displayName,
        description: Docs.config.rclPinOptions.mceGpo2.description
    },
    {
        name: "MCEGPO3",
        displayName: Docs.config.rclPinOptions.mceGpo3.displayName,
        description: Docs.config.rclPinOptions.mceGpo3.description
    },
    {
        name: "MCEGPO4",
        displayName: Docs.config.rclPinOptions.mceGpo4.displayName,
        description: Docs.config.rclPinOptions.mceGpo4.description
    },
    {
        name: "MCEGPO5",
        displayName: Docs.config.rclPinOptions.mceGpo5.displayName,
        description: Docs.config.rclPinOptions.mceGpo5.description
    },
    {
        name: "MCEGPO6",
        displayName: Docs.config.rclPinOptions.mceGpo6.displayName,
        description: Docs.config.rclPinOptions.mceGpo6.description
    },
    {
        name: "MCEGPO7",
        displayName: Docs.config.rclPinOptions.mceGpo7.displayName,
        description: Docs.config.rclPinOptions.mceGpo7.description
    },
    {
        name: "PBEGPO0",
        displayName: Docs.config.rclPinOptions.pbeGpo0.displayName,
        description: Docs.config.rclPinOptions.pbeGpo0.description
    },
    {
        name: "PBEGPO1",
        displayName: Docs.config.rclPinOptions.pbeGpo1.displayName,
        description: Docs.config.rclPinOptions.pbeGpo1.description
    },
    {
        name: "PBEGPO2",
        displayName: Docs.config.rclPinOptions.pbeGpo2.displayName,
        description: Docs.config.rclPinOptions.pbeGpo2.description
    },
    {
        name: "PBEGPO3",
        displayName: Docs.config.rclPinOptions.pbeGpo3.displayName,
        description: Docs.config.rclPinOptions.pbeGpo3.description
    },
    {
        name: "PBEGPO4",
        displayName: Docs.config.rclPinOptions.pbeGpo4.displayName,
        description: Docs.config.rclPinOptions.pbeGpo4.description
    },
    {
        name: "PBEGPO5",
        displayName: Docs.config.rclPinOptions.pbeGpo5.displayName,
        description: Docs.config.rclPinOptions.pbeGpo5.description
    },
    {
        name: "PBEGPO6",
        displayName: Docs.config.rclPinOptions.pbeGpo6.displayName,
        description: Docs.config.rclPinOptions.pbeGpo6.description
    },
    {
        name: "PBEGPO7",
        displayName: Docs.config.rclPinOptions.pbeGpo7.displayName,
        description: Docs.config.rclPinOptions.pbeGpo7.description
    },
    {
        name: "COEX_GRANT",
        displayName: Docs.config.rclPinOptions.coexGrant.displayName,
        description: Docs.config.rclPinOptions.coexGrant.description
    }
]

/* This maps each use case to their signals */
let useCaseMap = {
    debugSignals: ["RFEGPO0","RFEGPO1"]
}

/* Supported coex modes */
const coexModeSupport = [
    "coexMode3Wire",
    "coexMode2WireRequestGrant",
    "coexMode2WireRequestPriority",
    "coexMode1WireRequest",
    "coexMode1WireGrant"
];

/* Array of level options */
const coexLevelOptions = [
    {
        name: 0,
        displayName: "Low"
    },
    {
        name: 1,
        displayName: "High"
    }
];

/*Only enable FPGA use case when using FPGA board*/
let board = system.deviceData.board;
let isFPGA = false;
let useCaseOptions = [
        {
            name: "rfDebugSignals",
            displayName: Docs.config.useCase.standard.displayName,
            description: Docs.config.useCase.standard.description
        }
]
/* Users may be using SysConfig with no provided board */
if (board) {
    isFPGA = board.name.match(/FPGA/);
}

if (isFPGA) {
    useCaseOptions.push({
        name: "fpga",
        displayName: Docs.config.useCase.fpga.displayName,
        description: Docs.config.useCase.fpga.description
    })
}

/*Helper: Checks if all the elements of target is included in arr */
let arrayContains = (arr, target) => target.every(v => arr.includes(v));

let config = [
    {
        name: "rclConfig",
            displayName: Docs.config.rclConfig.displayName,
            longDescription: Docs.config.rclConfig.longDescription,
            collapsed: false,
            config: [
                {
                    name: "loggingEnabled",
                    displayName: Docs.config.loggingEnabled.displayName,
                    hidden : false,
                    description: Docs.config.loggingEnabled.longDescription,
                    default: false
                },
                {
                    name: "coexEnabled",
                    displayName: Docs.coex.enable.displayName,
                    description: Docs.coex.enable.description,
                    longDescription: Docs.coex.enable.longDescription,
                    hidden: false,
                    default: false,
                    onChange: (inst, ui) => onCoexEnableChanged(inst, ui, "coexEnabled")
                },
                {
                    name: "pbeGpoMask",
                    displayName: Docs.config.rclPbeGpoMask.displayName,
                    displayFormat: { radix: "hex", bitSize: 8 },
                    description: Docs.config.rclPbeGpoMask.description,
                    longDescription: Docs.config.rclPbeGpoMask.longDescription,
                    hidden: true,
                    default: 0x00,
                },
            ]
    },
    {
        name: "rclObservables",
        displayName: Docs.config.rclObservables.displayName,
        longDescription: Docs.config.rclObservables.longDescription,
        collapsed: false,
        config: [
            {
                name: "useCase",
                displayName: Docs.config.useCase.displayName,
                description: Docs.config.useCase.description,
                longDescription: Docs.config.useCase.longDescription,
                hidden: false,
                default: [],
                minSelections: 0,
                onChange: onUseCaseChanged,
                options: useCaseOptions
            },
            {
                name: "signals",
                displayName: Docs.config.signals.displayName,
                description: Docs.config.signals.description,
                longDescription: Docs.config.signals.longDescription,
                hidden: false,
                default: [],
                disableSelectAll: true,
                minSelections: 0,
                onChange: onSignalsChanged,
                getDisabledOptions: getDisabledOptions,
                options: getAvailableSignals,
            }
        ]
    },
    {
        name: "coexConfigGroup",
        displayName: Docs.coex.configGroup.displayName,
        collapsed: false,
        config: [
            {
                name: "coexMode",
                displayName: Docs.coex.mode.displayName,
                description: Docs.coex.mode.description,
                longDescription: Docs.coex.mode.longDescription,
                hidden: true,
                default: "coexMode3Wire",
                options: [
                    {
                        name: "coexMode3Wire",
                        displayName: Docs.coex.mode.threeWire.displayName,
                        description: Docs.coex.mode.threeWire.description
                    },
                    {
                        name: "coexMode2WireRequestGrant",
                        displayName: Docs.coex.mode.twoWireRequestGrant.displayName,
                        description: Docs.coex.mode.twoWireRequestGrant.description
                    },
                    {
                        name: "coexMode2WireRequestPriority",
                        displayName: Docs.coex.mode.twoWireRequestPriority.displayName,
                        description: Docs.coex.mode.twoWireRequestPriority.description
                    },
                    {
                        name: "coexMode1WireRequest",
                        displayName: Docs.coex.mode.oneWireRequest.displayName,
                        description: Docs.coex.mode.oneWireRequest.description
                    },
                    {
                        name: "coexMode1WireGrant",
                        displayName: Docs.coex.mode.oneWireGrant.displayName,
                        description: Docs.coex.mode.oneWireGrant.description
                    }
                ],
                onChange: onCoexModeChanged
            },
            {
                name: "coexPinRequestIdleLevel",
                displayName: Docs.coex.requestPinIdleLevel.displayName,
                description: Docs.coex.requestPinIdleLevel.description,
                longDescription: Docs.coex.requestPinIdleLevel.longDescription,
                hidden: true,
                default: 0,
                options: coexLevelOptions
            },
            {
                name: "coexPinPriorityIdleLevel",
                displayName: Docs.coex.priorityPinIdleLevel.displayName,
                description: Docs.coex.priorityPinIdleLevel.description,
                longDescription: Docs.coex.priorityPinIdleLevel.longDescription,
                hidden: true,
                default: 0,
                options: coexLevelOptions
            },
            {
                name: "coexPinPriorityIndicationEnabled",
                displayName: Docs.coex.priorityIndicationEnable.displayName,
                description: Docs.coex.priorityIndicationEnable.description,
                longDescription: Docs.coex.priorityIndicationEnable.longDescription,
                hidden: true,
                default: true,
                onChange: onCoexPriorityIndicationEnabledChanged
            },
            {
                name: "coexPinPriorityIndicationLowLevel",
                displayName: Docs.coex.priorityIndicationLowLevel.displayName,
                description: Docs.coex.priorityIndicationLowLevel.description,
                longDescription: Docs.coex.priorityIndicationLowLevel.longDescription,
                hidden: true,
                default: 0,
                options: coexLevelOptions
            },
            {
                name: "coexPinPriorityIndicationTime",
                displayName: Docs.coex.priorityIndicationTime.displayName,
                description: Docs.coex.priorityIndicationTime.description,
                longDescription: Docs.coex.priorityIndicationTime.longDescription,
                hidden: true,
                default: 20,
            },
            {
                name: "coexPinGrantIdleLevel",
                displayName: Docs.coex.grantPinIdleLevel.displayName,
                description: Docs.coex.grantPinIdleLevel.description,
                longDescription: Docs.coex.grantPinIdleLevel.longDescription,
                hidden: true,
                default: 1,
                options: coexLevelOptions
            },
            {
                name: "coexPinRequestRfActivityLatencyTime",
                displayName: Docs.coex.requestRfActivityLatencyTime.displayName,
                description: Docs.coex.requestRfActivityLatencyTime.description,
                longDescription: Docs.coex.requestRfActivityLatencyTime.longDescription,
                hidden: true,
                default: 120,
            },
            {
                name: "coexRuntimeChangeable",
                displayName: Docs.coex.runtimeChangeable.displayName,
                description: Docs.coex.runtimeChangeable.description,
                longDescription: Docs.coex.runtimeChangeable.longDescription,
                hidden: true,
                default: false,
            },
            {
                name: "coexPinRequestIeeeNumCorr",
                displayName: Docs.coex.pinRequestIeeeNumCorr.displayName,
                description: Docs.coex.pinRequestIeeeNumCorr.description,
                longDescription: Docs.coex.pinRequestIeeeNumCorr.longDescription,
                hidden: true,
                options: [1, 2, 3, 4, 5, 6, 7, 8].map(it => ({name: it})),
                default: 4
            },
            {
                /* This parameter is always hidden. It controls the timeout of a request for
                   IEEE 802.15.4 before sync must be found, when using the request on sync option.
                   The parameter may be changed using the .syscfg file */
                name: "ieeeTSync",
                hidden: true,
                default: 140
            }
        ]
    }
];

/*
 *******************************************************************************
 Status Functions
 *******************************************************************************
 */
/*
 *  ======== filterHardware ========
 *  Check component signals for compatibility with RCL.
 *
 *  @param component    - A hardware component
 *  @return             - Bool
 */
function filterHardware(component) {
    return (Common.typeMatches(component.type, ["RCL"]));
}

/*
 *  ======== resetDefaultValue ========
 *  Set specified configuration option to its default value
 *
 *  @param inst     - Module instance object containing the config
 *  @param config   - Config option to reset
 */
function resetDefaultValue(inst, config){
    if(config in inst)
    {
        if (config in inst.$module.$configByName)
        {
            inst[config] = inst.$module.$configByName[config].default;
        }
    }
}

/*
 *******************************************************************************
 Get Functions
 *******************************************************************************
 */
 function getDisabledOptions(inst)
{
    let maxSignalsLimit = 8;
    let disabledOptions = [];
    if (inst.signals.length >= maxSignalsLimit){
        for (let i = 0; i < signalList.length; i++) {
            let signal = signalList[i].name
            if (!inst.signals.includes(signal)){
                disabledOptions.push({
                    name: signal,
                    reason: "Too many added signals"
                })
            }
        }
    }

    return (disabledOptions);
}

function getAvailableSignals(inst)
{
    return signalList.slice(0);
}

/*  ======== getCoexPinInfo ========
 *  Get list with info on enabled coex signals.
 *
 *  @param inst     - Module instance object containing the config
 *  @return         - Array of coex pin information
 */
function getCoexPinInfo(inst)
{
    const {coexEnabled: enable, coexMode: mode} = inst;

    const request = {
        name: "PBEGPO0",
        pinInstance: {
            name:           "pbegpo0PinInstance",
            displayName:    "PBEGPO0 (COEX_REQUEST) Pin Configuration", /* GUI name */
            moduleName: "/ti/drivers/GPIO",
            readOnly: false,
            collapsed: true,
            args: {
                $name: "CONFIG_COEX_REQUEST",
            },
            requiredArgs: {
                $hardware: null,
                parentInterfaceName: "lrfGpio",
                parentSignalName: "pbegpo0Pin",
                parentSignalDisplayName: "COEX_REQUEST Pin",
                mode: "Output",
                pull: "None",
                invert: inst["coexPinRequestIdleLevel"] ? true : false,
                doNotConfig: false
            }
        }
    };

    const priority = {
        name: "PBEGPO1",
        pinInstance: {
            name:           "pbegpo1PinInstance",
            displayName:    "PBEGPO1 (COEX_PRIORITY) Pin Configuration", /* GUI name */
            moduleName: "/ti/drivers/GPIO",
            readOnly: false,
            collapsed: true,
            args: {
                $name: "CONFIG_COEX_PRIORITY",
            },
            requiredArgs: {
                $hardware: null,
                parentInterfaceName: "lrfGpio",
                parentSignalName: "pbegpo1Pin",
                parentSignalDisplayName: "COEX_PRIORITY Pin",
                mode: "Output",
                pull: "None",
                invert: inst["coexPinPriorityIdleLevel"] ? true : false,
                doNotConfig: false
            }
        }
    };

    const grant = {
        name: "COEX_GRANT",
        pinInstance: {
            name:           'coex_grantPinInstance',
            displayName:    "COEX_GRANT Pin Configuration", /* GUI name */
            moduleName: "/ti/drivers/GPIO",
            readOnly: false,
            collapsed: true,
            args: {
                $name: `CONFIG_COEX_GRANT`,
            },
            requiredArgs: {
                $hardware: null,
                parentInterfaceName: "lrfGpio",
                parentSignalName: "coex_grantPin",
                parentSignalDisplayName: "COEX_GRANT Pin",
                mode: "Input",
                invert: inst["coexPinGrantIdleLevel"] ? false : true,
                doNotConfig: false
            }
        }
    };

    const coexPinInfo = [];
    if(enable)
    {
        switch(mode) {
            case "coexMode3Wire":
                coexPinInfo.push(request, priority, grant);
                break;
            case "coexMode2WireRequestGrant":
                coexPinInfo.push(request, grant);
                break;
            case "coexMode2WireRequestPriority":
                coexPinInfo.push(request, priority);
                break;
            case "coexMode1WireRequest":
                coexPinInfo.push(request);
                break;
            case "coexMode1WireGrant":
                coexPinInfo.push(grant);
                break;
        }
    }
    return coexPinInfo;
}

/*
 *******************************************************************************
 OnChange functions
 *******************************************************************************
 */
function onHardwareChanged(inst, ui) {
}

function onSignalsChanged(inst, ui){

}

function onUseCaseChanged(inst, ui) {

    if (inst.signals.length <= 6)
    {
        if (inst.useCase.includes("rfDebugSignals"))
        {
            updateConfigValue(inst, ui, "signals",  [ ...inst.signals, ...useCaseMap.debugSignals])
        }
    }
    if (!inst.useCase.includes("rfDebugSignals"))
    {
        if (arrayContains(inst.signals,useCaseMap.debugSignals ))
        {
            let templist = []
            let index = 0
            inst.signals.forEach(signal => {
                if (useCaseMap.debugSignals.includes(signal))
                {
                    templist = [...inst.signals]
                    index = templist.indexOf(signal);
                    templist.splice(index,1)
                    inst.signals = templist
                }
            });
        }
    }
}


/*
 *  ======== onCoexEnableChanged ========
 *  Called when config coexEnabled changes
 *
 *  @param inst   - Module instance object containing config that changed
 *  @param ui     - User Interface state object
 *  @param config - Name of calling config
 */
function onCoexEnableChanged(inst, ui, config) {
    const {coexEnabled: enabled} = inst;

    const coexInstances = Object.keys(ui).filter(key => ((key.includes("coex") && key !== "coex_grantPinInstance" && key !== config)));

    /* Show coex config instances according to coexEnabled state */
    coexInstances.forEach(instance => {
        ui[instance].hidden = !enabled;
    });

    /* Reset config instances to default value if hidden */
    coexInstances.filter(instance => ui[instance].hidden).forEach(instance => resetDefaultValue(inst, instance));

    /* Make coex pins selected in GPIO signal list */
    updatePinSymbols(inst, ui);
}

/*
 *  ======== onCoexModeChanged ========
 *  Called when config coexMode changes
 *
 *  @param inst - Module instance object containing config that changed
 *  @param ui   - User Interface state object
 */
function onCoexModeChanged(inst, ui) {
    const {coexMode: mode} = inst;
    const coexPinInstances = Object.keys(ui).filter(key => key.includes("coexPin"));

    /* Update pin config visibility according to mode */
    coexPinInstances.forEach(instance => {
        if(instance.includes("Request")) {
            ui[instance].hidden = (mode == "coexMode1WireGrant");
        }
        else if(instance.includes("Priority")) {
            ui[instance].hidden = (mode !== "coexMode3Wire" && mode != "coexMode2WireRequestPriority");
            if (!ui[instance].hidden && instance.includes("PriorityIndication") && !instance.includes("Enabled")) {
                ui[instance].hidden = !inst.coexPinPriorityIndicationEnabled;
            }
        }
        else if(instance.includes("Grant")) {
            ui[instance].hidden = (mode == "coexMode1WireRequest" || mode == "coexMode2WireRequestPriority");
        }
    });

    /* Reset config instance to default value if hidden */
    const hiddenInstances = coexPinInstances.filter(instance => ui[instance].hidden);
    hiddenInstances.forEach(instance => resetDefaultValue(inst, instance));

    /* Update which coex pins are selected in GPIO signal list */
    updatePinSymbols(inst, ui);
}

/*
 *  ======== onCoexPriorityIndicationEnabledChanged ========
 *  Called when config coexPinPriorityIndicationEnabled changes
 *
 *  @param inst - Module instance object containing config that changed
 *  @param ui   - User Interface state object
 */
function onCoexPriorityIndicationEnabledChanged(inst, ui) {
    /* Hide or unhide the other signals*/
    const priorityIndicationInstances = Object.keys(ui).filter(key => key.includes("coexPinPriorityIndication"));
    priorityIndicationInstances.forEach(instance => {
        if (!instance.includes("Enabled")) {
            ui[instance].hidden = !inst.coexPinPriorityIndicationEnabled;
        }
    });
}

/*
 *******************************************************************************
 Update functions
 *******************************************************************************
 */

 /*  ======== updatePinSymbols ========
 *  Update pin symbol visibility according to selected pins.
 *
 *  @param inst - Module instance object containing config that changed
 *  @param ui   - User Interface state object
 */
function updatePinSymbols(inst, ui) {
    const {coexEnabled: enable} = inst;
    let coexSignals = getCoexPinInfo(inst).map(signal => signal.name);

    let allCoexSignals = ["PBEGPO0", "PBEGPO1", "COEX_GRANT"];
    let templist = [];
    let index = 0;
    inst.signals.forEach(signal => {
        if (allCoexSignals.includes(signal))
        {
            templist = [...inst.signals]
            index = templist.indexOf(signal);
            templist.splice(index,1)
            inst.signals = templist
        }
    });
    if (enable)
    {
        if (inst.signals.length + coexSignals.length <= 8)
        {
            updateConfigValue(inst, ui, "signals",  [ ...inst.signals, ...coexSignals])
        }
    }
}

/*
 *  ======== updateConfigValue ========
 *  Update config value and trigger onChange member function.
 *
 *  @param inst     - Module instance object containing the config
 *  @param ui       - UI state object
 *  @param config   - Config option part of module
 *  @param value    - New value for config
 */
function updateConfigValue(inst, ui, config, value){
    if(config in inst) {
        inst[config] = value;
        inst.$module.$configByName[config].onChange(inst, ui);
    }
}

/*
 *******************************************************************************
 Module Dependencies
 *******************************************************************************
 * When change occurs within module, the default module functions (if declared)
 * will be triggered when a configurable changes state:
 *  - pinmuxRequirements(...)
 *  - moduleInstances(...)
 *  - sharedModuleInstances(...)
 *  - modules(...)
 */
 function getOpts()
 {

     let board = system.deviceData.board;
     let isFPGA = false;

     /* Users may be using SysConfig with no provided board */
     if (board) {
         isFPGA = board.name.match(/FPGA/);
     }

     if (isFPGA) {
         return ["-DSOCFPGA"];
     }
     else {
         return [];
     }
 }


 function getDeviceType()
 {
    let device = "";
    if ("device" in system.deviceData){
        device = system.deviceData.device;
    }
    if (device.match(/CC23/)){
        return "CC23"
    }
    else if (device.match(/CC27/)){
        return "CC27"
    }
    else{
        return ""
    }
 }

 /*!
 *  ======== allowedInputPins ========
 *  Filter out pins that do not support LRFD input
 */
 function allowedInputPins(devicePin, peripheralPin)
{
    if (getDeviceType() == "CC23"){
        /* On CC23xx, DIO3 and DIO4 are outputs only for LRFD */
        return (devicePin.designSignalName.match(/^(?!.*(DIO3|DIO4))/));
    }
    else{
        return (devicePin.designSignalName);
    }
}

/*!
 *  ======== pinmuxRequirements ========
 *  Return peripheral pin requirements as a function of config.
 *
 *  Called when a configuration changes in the module.
 *
 *  @param inst - Module instance containing the config that changed
 *  @return     - Array of pin requirements
 */
function pinmuxRequirements(inst) {

    let resources = [];
    let rclArray = [];

    signalList.forEach(signal => {

        if (inst.signals.includes(signal.name))
        {
            let resourceItem = {
                name:           `${signal.name.toLowerCase()}Pin`,
                displayName:    `${signal.name} Pin`, /* GUI name */
                interfaceNames: ["D0","D1","D2","D3","D4","D5","D6","D7"]
            };

            if (signal.name == "COEX_GRANT")
            {
                resourceItem.filter = allowedInputPins;
            }
            resources.push(resourceItem);
        }
    });


    if (inst.signals.length > 0){
        let lrfGpio = {
            name          : "lrfGpio",
            readOnly      : true,
            displayName   : "LRF GPIO",
            interfaceName : "LRF",
            canShareWith  : "LRF",
            resources     : resources,
            signalTypes   : {
            }
        };
        rclArray.push(lrfGpio);
    }

    return rclArray;
}



/*
 *  ======== moduleInstances ========
 *  Determines what modules are added as non-static submodules.
 *
 *  Called when a configuration changes in the module.
 *
 *  @param inst - Module instance containing the config that changed
 *  @return     - Array of pin instances
 */
function moduleInstances(inst) {
    let dependencyModules = [];

    let coexSignalList = getCoexPinInfo(inst);

    coexSignalList.forEach(signal => {
        if (inst.signals.includes(signal.name))
        {
            dependencyModules.push(signal.pinInstance);
        }
    });
    let coexNameList = coexSignalList.map(signal => signal.name);

    signalList.forEach(signal => {
        if (inst.signals.includes(signal.name) && !coexNameList.includes(signal.name))
        {
            if (signal.name == "COEX_GRANT")
            {
                dependencyModules.push({
                    name:           `${signal.name.toLowerCase()}PinInstance`,
                    displayName:    `${signal.name} Pin Configuration`, /* GUI name */
                    moduleName: "/ti/drivers/GPIO",
                    readOnly: false,
                    collapsed: true,
                    args: {
                        $name: `CONFIG_${signal.name}`,
                    },
                    requiredArgs: {
                        $hardware: null,
                        parentInterfaceName: "lrfGpio",
                        parentSignalName: `${signal.name.toLowerCase()}Pin`,
                        parentSignalDisplayName: `${signal.name} Pin`,
                        mode: "Input",
                        doNotConfig: false
                    }
                });
            }
            else {
                dependencyModules.push({
                    name:           `${signal.name.toLowerCase()}PinInstance`,
                    displayName:    `${signal.name} Pin Configuration`, /* GUI name */
                    moduleName: "/ti/drivers/GPIO",
                    readOnly: false,
                    collapsed: true,
                    args: {
                        $name: `CONFIG_${signal.name}`,
                    },
                    requiredArgs: {
                        $hardware: null,
                        parentInterfaceName: "lrfGpio",
                        parentSignalName: `${signal.name.toLowerCase()}Pin`,
                        parentSignalDisplayName: `${signal.name} Pin`,
                        mode: "Output",
                        pull: "None",
                        doNotConfig: false
                    }
                });
            }
        }
    });

    /* If logging is enabled, push a dependency on a log module */
    if (inst.loggingEnabled) {
        dependencyModules.push(
            {
                name: "LogModule",
                displayName: "RCL Log Configuration",
                moduleName: "/ti/log/LogModule",
                collapsed: true,
                args: {
                    $name: "LogModule_RCL",
                    enable_DEBUG: false,
                    enable_INFO: false,
                    enable_VERBOSE: false,
                    // Only enable WARNING and ERROR by default
                    enable_WARNING: true,
                    enable_ERROR: true
                }
            }
        );
    }
    return dependencyModules;
}

/*
 *  ======== sharedModuleInstances ========
 *  Determines what modules are added as shared static sub-modules.
 *
 *  Called when a configuration changes in the module.
 *
 *  @param inst - Module instance containing the config that changed
 *  @return     - Array containing dependency modules
 */
function sharedModuleInstances(inst){
    return [];
}

/*
 *  ======== modules ========
 *  Determines what modules are added as static sub-modules.
 *
 *  Called when a configuration changes in the module.
 *
 *  @param inst - Module instance containing the config that changed
 *  @return     - Array containing static dependency modules
 */
function modules(inst) {

    const dependencies = ["Board", "Power", "Temperature"];

    return Common.autoForceModules(dependencies)();
}
/*
 *******************************************************************************
 GenLibs
 *******************************************************************************
 */

/*!
 *  ======== getLibs ========
 *  Return link option argument for GenLibs.
 *
 *  @param mod  - Module object
 *  @return     - Link option object
 */
function getLibs(mod) {
    const GenLibs = system.getScript("/ti/utils/build/GenLibs.syscfg.js");
    const DriverLib = system.getScript("/ti/devices/DriverLib");

    /* get device information from DriverLib */
    const deviceId = system.deviceData.deviceId;
    var log_suffix = mod.$static.loggingEnabled ? "_log" : "";
    var libName = DriverLib.getAttrs(deviceId).libName;

    var link_info = {
        name: "/ti/drivers/rcl",
        deps: [
            "/ti/drivers"
        ],
        libs: [
            GenLibs.libPath('ti/drivers/rcl', `rcl_${libName}${log_suffix}.a`),
            GenLibs.libPath(`ti/devices/${libName}/rf_patches`, `lrf_${libName}.a`)
        ]
    };

    return link_info;
}



/*
 *******************************************************************************
 Module Validation
 *******************************************************************************
 */

/*!
 *  ======== validate ========
 *  Validate rfdriver module configuration.
 *
 *  @param inst         - Module instance containing the config that changed
 *  @param validation   - Object to hold detected validation issues
 */
function validate(inst, validation)
{
    if (inst.useCase.includes("rfDebugSignals")){
        useCaseMap.debugSignals.forEach(signal => {
            if (!inst.signals.includes(signal))
            {
                Common.logError(validation, inst,"signals",
                `All RCL debug signals are not set according to ${system.getReference(inst, "useCase")}.`);
            }
        });
    }
    let coexSignalList = getCoexPinInfo(inst);

    let coexSignalError = false;
    coexSignalList.forEach(signal => {
        if (!inst.signals.includes(signal.name))
        {
            coexSignalError = true;
        }
    });
    if (coexSignalError)
    {
        Common.logError(validation, inst, "signals",
            `All configured RCL coex signals are not set according to ${system.getReference(inst, "coexMode")}.`);
    }

    /* Check that coex times are within bounds */
    if (inst.coexPinPriorityIndicationTime > 30 || inst.coexPinPriorityIndicationTime < 3)
    {
        Common.logError(validation, inst, "coexPinPriorityIndicationTime",
            "Priority Indication Time must be between 3 and 30 µs");
    }
    if (inst.coexPinRequestRfActivityLatencyTime > 150 || inst.coexPinRequestRfActivityLatencyTime < 90)
    {
        Common.logError(validation, inst, "coexPinRequestRfActivityLatencyTime",
            "RF Activity Latency Time must be between 90 and 150 µs");
    }
}

/*
 *******************************************************************************
 Module Export
 *******************************************************************************
 */

 /*
 *  ======== rclgpioModule ========
 *  Define the RCL Driver module properties and methods.
 */
let rclgpioModule = {
    moduleStatic: {
        config,
        pinmuxRequirements,
        moduleInstances,
        sharedModuleInstances,
        modules,
        validate,
        filterHardware,
        onHardwareChanged
    },

    /* override device-specific templates */
    templates: {
        "/ti/utils/build/GenLibs.cmd.xdt":
            { modName: "/ti/drivers/RCL", getLibs },
        "/ti/utils/build/GenOpts.opt.xdt":
            { modName: "/ti/drivers/RCL", getOpts : getOpts },
        boardc: "/ti/drivers/rcl/templates/RCL.Board.c.xdt",
        board_initc: "/ti/drivers/rcl/templates/RCL.Board_init.c.xdt",
        boardh: "/ti/drivers/rcl/templates/RCL.Board.h.xdt"
    }
};


/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic RCL module to
 *  allow us to augment/override as needed for the CC23XX
 */
function extend(base) {
    /* merge and overwrite base module attributes */
    return({...base, ...rclgpioModule});
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by generic RCL module */
    extend: extend,
    signalList: signalList
};
