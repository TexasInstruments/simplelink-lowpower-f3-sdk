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
 *  ======== ble.syscfg.js ========
 */

"use strict";

// Get ble long descriptions
const Docs = system.getScript("/ti/ble/ble_docs.js");

//Get Radio Script
const radioScript = system.getScript("/ti/ble/rf_config/"
                            + "ble_rf_config");
//Get Connection Resources Script
const connectionResourcesScript = system.getScript("/ti/ble/general/ble_connection_resources");
//Get Device Identity Script
const deviceIdentityScript = system.getScript("/ti/ble/general/ble_device_identity");
//Get Bond Manager Script
const bondMgrScript = system.getScript("/ti/ble/bondManager/"
                            + "ble_bondmgr");

//Get Central Script
const centralScript = system.getScript("/ti/ble/central/"
                            + "ble_central");

//Get Observer Script
const observerScript = system.getScript("/ti/ble/observer/"
                            + "ble_observer");

//Get Peripheral Script
const peripheralScript = system.getScript("/ti/ble/peripheral/"
                            + "ble_peripheral");

//Get broadcaster Script
const broadcasterScript = system.getScript("/ti/ble/broadcaster/"
                            + "ble_broadcaster");
//Get l2capScript Script
const l2capScript = system.getScript("/ti/ble/l2cap_coc/"
                            + "ble_l2cap_coc");

//Get Adv Settings Script
const advSetScript = system.getScript("/ti/ble/advanced_settings/ble_advanced_settings");
// Get common Script
const Common = system.getScript("/ti/ble/ble_common.js");

// Get profiles Script
const profilesScript = system.getScript("/ti/ble/profiles/ble_profiles_config");

// Get profiles Script
const csScript = system.getScript("/ti/ble/channel_sounding/ble_cs");

// Get PADV Time Sync Script
const padvTimeSyncScript = system.getScript("/ti/ble/padv_time_sync/ble_padv_time_sync");

// Get OAD Script
const oadScript = system.getScript("/ti/ble/oad/ble_oad");

// Get BLE Features Script
const bleFeaturesScript = system.getScript("/ti/ble/ble_features/ble_features");

// Get shared driver dependencies
const driverDepsScript = system.getScript("/ti/ble/ble_dependencies");

//static implementation of the BLE module
const moduleStatic = {

    //configurables for the static BLE module
    config: [
        {
            name: "lockProject",
            displayName: "Lock Project",
            default: false,
            hidden: true,
            onChange: onLockProjectChange
        },
        {
            name: "genLibs",
            displayName: "Generate BLE Libraries",
            default: true,
            hidden: true,
            description: "Configures genLibs usage for local libraries. Always hidden",
            onChange: advSetScript.onGenLibsChange
        },
        {
            name: "calledFromDeviceRole",
            default: false,
            hidden: true
        },
        {
            name: "basicBLE",
            default: false,
            hidden: true,
            onChange: onBasicBLEChange
        },
        {
            name: "basicBLEProfiles",
            default: false,
            hidden: true,
            onChange: onBasicBLEChange
        },
        {
            name: "hidePtm",
            displayName: "Hide PTM",
            default: false,
            hidden: true,
            description: "Used to hide the PTM configurable. Always hidden"
        },
        {
            name: "host",
            default: false,
            hidden: true,
            description: "When true, only Stack Settings are exposed (host test mode). Always hidden",
            onChange: onHostChange
        },
        {
            name: "trensLayer",
            displayName: "Transport Layer",
            default: "HCI_TL_NONE",
            description: "When using PTM configuration please choose HCI_TL_NONE",
            hidden: true,
            options: [
                {
                    displayName: "None",
                    name: "HCI_TL_NONE"
                },
                {
                    displayName: "Full",
                    name: "HCI_TL_FULL"
                }
            ]
        },
        {
            name: "oadProject",
            displayName: "OAD Project",
            default: false,
            hidden: true,
            onChange: onOadProjectChange,
            description: "Used to indicate that this is an OAD project. Always hidden"
        },
        {
            name: "enablePersistentOad",
            default: false,
            hidden: true,
            onChange: onEnablePersistentOadChange,
            description: "Enable Persistent OAD. Set only by .syscfg scripts. Always hidden"
        },
        {
            name: "deviceRole",
            displayName: "Device Role",
            description: "The BLE device role",
            default: "PERIPHERAL_CFG",
            readOnly: false,
            onChange: ondeviceRoleChange,
            longDescription: Docs.deviceRoleLongDescription,
            options: [
                {
                    displayName: "Observer",
                    name: "OBSERVER_CFG"
                },
                {
                    displayName: "Broadcaster",
                    name: "BROADCASTER_CFG"
                },
                {
                    displayName: "Peripheral",
                    name: "PERIPHERAL_CFG"
                },
                {
                    displayName: "Central",
                    name: "CENTRAL_CFG"
                },
                {
                    displayName: "Broadcaster + Observer",
                    name: "BROADCASTER_CFG+OBSERVER_CFG"
                },
                {
                    displayName: "Central + Broadcaster",
                    name: "CENTRAL_CFG+BROADCASTER_CFG"
                },
                {
                    displayName: "Peripheral + Observer",
                    name: "PERIPHERAL_CFG+OBSERVER_CFG"
                },
                {
                    displayName: "Peripheral + Central",
                    name: "PERIPHERAL_CFG+CENTRAL_CFG"
                }
            ]
        },
        // Hidden items from build config
        ...advSetScript.bleHiddenConfigItems,
        {
            name: "keyNode",
            displayName: "Key Node",
            default: false,
            hidden: true,
        },
        // BLE Features — top-level, not inside any wrapper group
        bleFeaturesScript.config,
        // Stack Settings — parameters consumed by the BLE stack libraries
        {
            name: "stackSettings",
            displayName: "Stack Settings",
            config: [
                connectionResourcesScript.config,
                radioScript.config,
                {
                    name: "bondMgrStackGroup",
                    displayName: "Bond Manager",
                    config: bondMgrScript.bondMgrStackConfigItems
                },
                {
                    name: "csStackGroup",
                    displayName: "Channel Sounding",
                    config: csScript.csStackConfigItems
                },
                {
                    name: "profilesStackConfig",
                    displayName: "Profiles",
                    config: profilesScript.profilesStackConfigItems
                },
                {
                    name: "adaptivityConfig",
                    displayName: "Adaptivity Configuration",
                    config: bleFeaturesScript.adaptivityConfigItems
                },
                {
                    name: "advancedStackSettings",
                    displayName: "Advanced Settings",
                    config: [
                        advSetScript.bleDebugGroup,
                        advSetScript.bleStackSettingsGroup,
                        advSetScript.bleICallSettingsGroup,
                    ]
                },
            ]
        },
        // Application Settings — parameters used by the application layer
        {
            name: "appSettings",
            displayName: "Application Settings",
            config: [
                {
                    name: "channelSoundingConfig",
                    displayName: "Channel Sounding",
                    config: csScript.csAppConfigItems
                },
                padvTimeSyncScript.config,
                {
                    name: "profiles_module",
                    displayName: "Profiles Configurations",
                    config: profilesScript.profilesAppConfigItems
                },
                deviceIdentityScript.config,
                {
                    name: "bondMgrConfig",
                    displayName: "Bond Manager",
                    config: bondMgrScript.bondMgrAppConfig.config
                },
                centralScript.config,
                observerScript.config,
                peripheralScript.config,
                broadcasterScript.config,
                l2capScript.config,
                oadScript.config,
                {
                    name: "advancedAppSettings",
                    displayName: "Advanced Settings",
                    config: [
                        advSetScript.blePtmGroup,
                        {
                            name: "appModules",
                            displayName: "Application Modules",
                            config: [advSetScript.disableDisplayModuleConfig]
                        },
                    ]
                },
            ]
        },
    ],

    dependencies: driverDepsScript.ccfgDependencies,

    validate: validate,
    migrateLegacyConfiguration: migrateLegacyConfiguration,
    moduleInstances: moduleInstances,
    modules: modules
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
    bleFeaturesScript.validate(inst, validation);
    radioScript.validate(inst, validation);
    connectionResourcesScript.validate(inst, validation);
    bondMgrScript.validate(inst, validation);
    advSetScript.validate(inst, validation);
    profilesScript.validate(inst, validation);
    csScript.validate(inst, validation);

    if(!inst.host)
    {
        deviceIdentityScript.validate(inst, validation);
        centralScript.validate(inst, validation);
        observerScript.validate(inst, validation);
        peripheralScript.validate(inst, validation);
        broadcasterScript.validate(inst, validation);
        l2capScript.validate(inst, validation);
        padvTimeSyncScript.validate(inst, validation);
        oadScript.validate(inst, validation);

        // Throw a warning on the useRcosc configurable when oadProject and useRcosc are set to true,
        // to indicate the user that the CCFG LF clock configuration should be done at the BIM project
        if(inst.oadProject && inst.useRcosc)
        {
            validation.logWarning("For OAD projects, the LF Clock configuration is set in the CCFG file of the BIM project", inst, "useRcosc");
        }
    }

    // BLE requires the RNG driver to operate in Blocking mode
    const rngMod = system.modules["/ti/drivers/RNG"];
    if(rngMod && rngMod.$static.rngReturnBehavior !== "RNG_RETURN_BEHAVIOR_BLOCKING")
    {
        validation.logError("BLE requires RNG Return Behavior to be set to Blocking", inst);
    }
}

/*
 *  ======== ondeviceRoleChange ========
 * Change the bond manager value when changing the role combination
 * Broadcaster and observer are not using Bond Manager
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function ondeviceRoleChange(inst,ui)
{
    const modes = Common.getModes(inst.deviceRole);

    if(!modes.isConnectable)
    {
        inst.maxConnNum = 0;
        inst.maxPDUNum = 0;
        inst.bondManager = false;
        ui.peerConnParamUpdateRejectInd.hidden = true;
    }
    else
    {
        // Restore defaults only when coming from a non-connectable role (values were forced to 0)
        if(inst.maxConnNum === 0) inst.maxConnNum = connectionResourcesScript.maxConnNumDefaultValue();
        if(inst.maxPDUNum === 0) inst.maxPDUNum = 5;
        ui.peerConnParamUpdateRejectInd.hidden = false;
    }


    // Connection Handover is for peripheral role only
    ui.connectionHandover.hidden = !modes.peripheral;
    if(!modes.peripheral)
    {
        inst.connectionHandover = false;
    }

    inst.calledFromDeviceRole = true;
    // Hide/Unhide groups since the device role was changed
    bleFeaturesScript.changeGroupsState(inst,ui);
    inst.calledFromDeviceRole = false;

    // Change the Periodic configurable state
    bleFeaturesScript.onExtAdvChange(inst,ui);
    bleFeaturesScript.onPeriodicAdvChange(inst,ui);

    // bondFailAction and centralGuardTime only apply to Central role
    ui.bondFailAction.hidden = !modes.central;
    ui.centralGuardTime.hidden = !modes.central;

    // Profiles only apply to connectable roles; app group also requires basicBLEProfiles
    Common.hideGroup(Common.getGroupByName(inst.$module.config, "profilesStackConfig"), !modes.isConnectable, ui);
    Common.hideGroup(Common.getGroupByName(inst.$module.config, "profiles_module"),
                     !(modes.isConnectable && inst.basicBLEProfiles), ui);

    if ( inst.keyNode == true )
    {
        setKeyNodeDefualtParams(inst);
    }
}

function setKeyNodeDefualtParams(inst)
{
    if ( inst.deviceRole.includes("CENTRAL_CFG") )
    {
        // Connection interval
        inst.connIntMin = 30;
        inst.connIntMax = 30;

        // Scan parameters
        inst.scanInt = 1000;
        inst.scanWin = 500;
        inst.scanDuration = 5000;
        inst.dupFilter = "SCAN_FLT_DUP_DISABLE";

        // Don't filter by service UUID
        inst.disDevBySerUuid = false;
    }
}

/*
 *  ======== onBasicBLEChange ========
 * Show/hide the Profiles app group based on basicBLEProfiles and device role.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onBasicBLEChange(inst,ui)
{
    inst.deviceInfo = inst.basicBLEProfiles;
    inst.profiles = [];

    const connectable = Common.getModes(inst.deviceRole).isConnectable;
    Common.hideGroup(Common.getGroupByName(inst.$module.config, "profiles_module"),
                     !(connectable && inst.basicBLEProfiles), ui);
}

/*
 *  ======== onLockProjectChange ========
 * Lock or unlock the deviceRole configurable,
 * disable/enable the option to change the deviceRole.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onLockProjectChange(inst,ui)
{
    inst.lockProject ? ui.deviceRole.readOnly = "Only this role is supported" :
                       ui.deviceRole.readOnly = false;
}

/*
 *  ======== onOadProjectChange ========
 * Hide/show parameters
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onOadProjectChange(inst,ui)
{
    inst.oadProject ? ui.useRcosc.hidden = false : ui.useRcosc.hidden = true;
}

/*
 *  ======== onEnablePersistentOadChange ========
 * Delegate Persistent OAD visibility/defaults to the OAD module
 *
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onEnablePersistentOadChange(inst, ui)
{
    oadScript.updateOadVisibility(inst, ui);
}

/*
 *  ======== onHostChange ========
 * When host=true, hide the Application Settings group and Application Features entirely.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onHostChange(inst, ui)
{
    bleFeaturesScript.changeGroupsState(inst, ui);
}

/*
 * ======== getLibs ========
 * Contribute libraries to linker command file
 *
 * @param inst  - Module instance containing the config that changed
 * @returns     - Object containing the name of component, array of dependent
 *                components, and array of library names
 */
function getLibs(inst)
{
    let GenLibs = system.getScript("/ti/utils/build/GenLibs.syscfg.js");
    let libs = [];
    let toolchain = GenLibs.getToolchainDir();

    if(inst.$static.genLibs)
    {
        let basePath = "";

        if(system.deviceData.deviceId.match(/CC23/))
        {
            // Add OneLib library
            basePath = `ti/ble/lib/CC2340R5/OneLib/lib/${toolchain}/m0p`
            libs.push(basePath + "/OneLib.a");
            // Add StackWrapper library (not needed in host-only mode)
            if(!inst.$static.host)
            {
                basePath = `ti/ble/lib/CC2340R5/StackWrapper/lib/${toolchain}/m0p`
                libs.push(basePath + "/StackWrapper.a");
            }
        }
        else if(system.deviceData.deviceId.match(/CC27/))
        {
            // Add OneLib library
            basePath = `ti/ble/lib/CC27XXX10/OneLib/lib/${toolchain}/m33f`
            libs.push(basePath + "/OneLib.a");
            // Add StackWrapper library (not needed in host-only mode)
            if(!inst.$static.host)
            {
                basePath = `ti/ble/lib/CC27XXX10/StackWrapper/lib/${toolchain}/m33f`
                libs.push(basePath + "/StackWrapper.a");
            }
        }
    }

    // Add the Channel Sounding related libs
    let csLibs = csScript.getLibs(inst)
    if(csLibs.length > 0)
    {
        libs.push(csLibs);
    }

    // Note: The OAD linker command file (ti_ble_oad_linker.cmd) is included
    // directly by the main linker file (cc2340_freertos.cmd) when MS_OAD is
    // defined, so we don't add it to genlibs here.

    // Create a GenLibs input argument
    const linkOpts = {
        name: "/ti/ble",
        deps: [],
        libs: libs
    };

    return(linkOpts);
}

/*
 *  ======== migrateLegacyConfiguration ========
 * Migration of deprecated configurables
 * @param inst  - Module instance containing the config that changed
 */
function migrateLegacyConfiguration(inst)
{
    // defaultTxPower was deprecated, if it used in users scripts
    // assign it's value to the new parameter defaultTxPowerValue
    inst.defaultTxPowerValue = Common.convertTxPowerIndexToValue(inst.defaultTxPower);

    // noOsalSnv was deprecated in favour of persistentStorage (inverted semantics)
    inst.persistentStorage = !inst.noOsalSnv;

    // extendedStackSettings was deprecated in favour of centralGuardTime (boolean)
    inst.centralGuardTime = (inst.extendedStackSettings === "CENTRAL_GUARD_TIME_ENABLE");
}

/*
 *  ======== getOpts ========
 */
function getOpts(mod)
{
    const inst = mod.$static;
    let result = [];

    // HOST_CONFIG must come first as it is the primary role define.
    result.push("-DHOST_CONFIG=" + inst.deviceRole);

    if(inst.host)
    {
        // Host-only mode: hardcode HCI_TL_FULL; use separate controller and host opts
        result.push("-DHCI_TL_FULL");
        result.push(bleFeaturesScript.getControllerOpts(mod));
        result.push(bleFeaturesScript.getHostOpts(mod));
        result.push(csScript.getStackOpts(mod));
    }
    else
    {
        result.push("-D" + inst.trensLayer);
        result.push(bleFeaturesScript.getOpts(mod));
        result.push(observerScript.getOpts(mod));
        result.push(csScript.getOpts(mod));
        result.push(padvTimeSyncScript.getOpts(mod));
        result.push(oadScript.getOpts(inst));
        inst.peerConnParamUpdateRejectInd && result.push("-DNOTIFY_PARAM_UPDATE_RJCT");
    }

    result.push(radioScript.getOpts(mod));
    result.push(connectionResourcesScript.getOpts(mod));
    result.push(bondMgrScript.getOpts(mod));
    result.push(advSetScript.getOpts(mod));
    result.push(profilesScript.getOpts(mod));

    return result.flat(Infinity);
}

/*
 *  ======== getLinkerDefs ========
 */
function getLinkerDefs(mod)
{
    return oadScript.getLinkerDefs(mod.$static);
}

/*
 *  ======== moduleInstances ========
 *  Determines what modules are added as non-static submodules
 *
 *  @param inst  - Module instance containing the config that changed
 *  @returns     - Array containing dependency modules
 */
function moduleInstances(inst)
{
    let dependencyModule = [];

    dependencyModule = radioScript.moduleInstances(inst);

    if(!inst.host)
    {
        dependencyModule = dependencyModule.concat(centralScript.moduleInstances(inst));
        dependencyModule = dependencyModule.concat(peripheralScript.moduleInstances(inst));
        dependencyModule = dependencyModule.concat(broadcasterScript.moduleInstances(inst));
        dependencyModule = dependencyModule.concat(oadScript.moduleInstances(inst));

        if(inst.gattBuilder)
        {
            dependencyModule.push(
            {
                name            : 'services',
                displayName     : 'Service',
                useArray        : true,
                moduleName      : '/ti/ble/gatt_services/Service',
                collapsed       : true,
                args            : {
                    hiddenServName : "1",
                    numOfServices  : 1
                }
            });
        }
    }

    dependencyModule = dependencyModule.concat(csScript.moduleInstances(inst));
    dependencyModule = dependencyModule.concat(advSetScript.moduleInstances(inst));

    dependencyModule = dependencyModule.concat(driverDepsScript.moduleInstances);

    return(dependencyModule);
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

    dependencyModule = advSetScript.modules(inst);
    dependencyModule = dependencyModule.concat(driverDepsScript.modules);
    // Pull in Multi-Stack validation module
    dependencyModule.push({
        name: "multiStack",
        displayName: "Multi-Stack Validation",
        moduleName: "/ti/common/multi_stack_validate",
        hidden: true
    });

    return(dependencyModule);
}

/*
 *  ======== bleModule ========
 *  Define the BLE module properties and methods
 */
const bleModule = {
    displayName: "BLE Stack",
    longDescription: "The BLE stack module is intended to simplify the stack "
                    + "configuration for the user. This module can be used "
                    + "only with the following applications:\n"
                    + "multi_role, simple_broadcaster, simple_central and "
                    + "simple_peripheral. For more information, refer to "
                    + "the [BLE User's Guide](ble/ble_user_guide/html/"
                    + "ble-stack-5.x/overview.html).",
    moduleStatic: moduleStatic,
    templates: {
        "/ti/ble/templates/ble_config.h.xdt":
        "/ti/ble/templates/ble_config.h.xdt",

        "/ti/ble/templates/ble_config.c.xdt":
        "/ti/ble/templates/ble_config.c.xdt",

        "/ti/ble/templates/ble_oad_linker.cmd.xdt":
        "/ti/ble/templates/ble_oad_linker.cmd.xdt",

        "/ti/ble/templates/ble_oad_postbuild.cfg.xdt":
        "/ti/ble/templates/ble_oad_postbuild.cfg.xdt",

        "/ti/ble/templates/ble_oad_prebuild.cfg.xdt":
        "/ti/ble/templates/ble_oad_prebuild.cfg.xdt",

        "/ti/utils/build/GenLibs.cmd.xdt":
        {
            modName: "/ti/ble/ble",
            getLibs: getLibs
        },
        "/ti/utils/build/GenOpts.opt.xdt": {
            modName: "/ti/ble/ble",
            getOpts: getOpts
        },
        "/ti/utils/build/GenMap.cmd.xdt": {
            modName: "/ti/ble/ble",
            getLinkerDefs: getLinkerDefs
        }
    }
};

/*
 *  ======== exports ========
 *  Export the BLE module
 */
exports = bleModule;
