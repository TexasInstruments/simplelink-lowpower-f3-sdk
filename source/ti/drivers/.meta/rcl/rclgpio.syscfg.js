/*
 * Copyright (c) 2024, Texas Instruments Incorporated - http://www.ti.com
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
    }]

/* This maps each use case to their signals */
let useCaseMap = {
    debugSignals: ["RFEGPO0","RFEGPO1"]
}

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
 *******************************************************************************
 Get Functions
 *******************************************************************************
 */



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
 *******************************************************************************
 Update functions
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
    return signalList.slice(1);
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

 function getBoard()
 {
    let board = system.deviceData.board;
    if (board.name.match(/CC23/)){
        return "CC23"
    }
    else if (board.name.match(/CC27/)){
        return "CC27"
    }
    else{
        return ""
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
        resources.push({
            name:           `${signal.name.toLowerCase()}Pin`,
            displayName:    `${signal.name} Pin`, /* GUI name */
            interfaceNames: ["D0","D1","D2","D3","D4","D5","D6","D7"]
        });
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
 *  @return     - Array of PIN instances
 */
function moduleInstances(inst) {
    let dependencyModules = [];

    signalList.forEach(signal => {
        if (inst.signals.includes(signal.name))
        {
        dependencyModules.push({
            name:           `${signal.name.toLowerCase()}PinInstance`,
            displayName:    `${signal.name} PIN Configuration While Pin Is Not In Use`, /* GUI name */
            moduleName: "/ti/drivers/GPIO",
            readOnly: false,
            collapsed: true,
            requiredArgs: {
                $name: `CONFIG_${signal.name}`,
                parentInterfaceName: "lrfGpio",
                parentSignalName: `${signal.name.toLowerCase()}Pin`,
                parentSignalDisplayName:  `${signal.name.toLowerCase()} Pin`,
                mode: "Output",
                initialOutputState: "Low",
                outputStrength: "High",
                pull: "None"
            }
        });
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
//function validate(inst, validation){}
function validate(inst, validation) {

    if (inst.useCase.includes("rfDebugSignals")){
        useCaseMap.debugSignals.forEach(signal => {
            if (!inst.signals.includes(signal))
            {
                Common.logError(validation, inst,"signals",
                "All RCL debug signals are not set");
            }
        });
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
