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
//Get General Script
const generalScript = system.getScript("/ti/ble/general/"
                            + "ble_general");
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
const advSetScript = system.getScript("/ti/ble/adv_config/"
                            + "ble_adv_config");
// Get common Script
const Common = system.getScript("/ti/ble/ble_common.js");

// Get profiles Script
const profilesScript = system.getScript("/ti/ble/profiles/ble_profiles_config");

// Get profiles Script
const csScript = system.getScript("/ti/ble/channel_sounding/ble_cs");

// Get Channel Sounding supported indication
const isNotChannelSoundingSupported = !Common.isChannelSoundingSupported();

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
            description: "Configures genLibs usage for local libraries. Always hidden"
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
            name: "oadProject",
            displayName: "OAD Project",
            default: false,
            hidden: true,
            onChange: onOadProjectChange,
            description: "Used to indicate that this is an OAD project. Always hidden"
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
        {
            name: "bleFeatures",
            displayName: "BLE Features",
            description: "BLE Stack Features",
            config:[
                {
                    name: "bondManager",
                    displayName: "Bond Manager",
                    description: "The Gap Bond Manager is always enabled",
                    longDescription: Docs.bondManagerLongDescription,
                    default: true,
                    onChange: onBondManagerChange,
                },
                {
                    name: "extAdv",
                    displayName: "Extended Advertising",
                    description: "BLE5 extended advertising feature",
                    longDescription: Docs.extAdvLongDescription,
                    onChange: onExtAdvChange,
                    default: false
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
                    name: "periodicAdv",
                    displayName: "Periodic Advertising",
                    longDescription: Docs.periodicAdvLongDescription,
                    hidden: false,
                    default: false
                },
                {
                    name: "periodicAdvSync",
                    displayName: "Periodic Advertising Sync",
                    longDescription: Docs.periodicAdvSyncLongDescription,
                    hidden: true,
                    default: false
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
                    name: "L2CAPCOC",
                    displayName: "L2CAP Connection Oriented Channels",
                    default: false,
                    longDescription: Docs.L2CAPCOCLongDescription,
                    onChange: onL2CAPCOCChange,
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
                    name: "trensLayer",
                    displayName: "Transport Layer",
                    default:"HCI_TL_NONE",
                    description: "When using PTM configuration please choose HCI_TL_NONE",
                    longDescription: Docs.trensLayerLongDescription,
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
                    name: "nwpMode",
                    displayName: "Network Processor Mode",
                    default: false,
                    hidden: true
                },
                {
                    name: "appExternalControlMode",
                    displayName: "App External Control Mode",
                    default: false,
                    hidden: true,
                },
                {
                    name:"healthToolkit",
                    displayName: "Health Toolkit",
                    longDescription: Docs.bleHealthLongDescription,
                    default: false,
                    hidden: true,
                },
                {
                    name: "hideHealthToolkit",
                    displayName: "Hide Health Toolkit",
                    default: true,
                    hidden: true,
                    description: "Used to hide the Health Toolkit configurable. Always hidden",
                    onChange: onHideHealthToolkitChange
                },
                {
                    name: "connectionHandover",
                    displayName: "Connection Handover",
                    default: false,
                    hidden: false,
                },
                {
                    name:"powerControl",
                    displayName: "Power Control",
                    default: false,
                    hidden: false,
                },
                {
                    name:"rssiMonitor",
                    displayName: "Rssi Monitor",
                    default: false,
                    hidden: false,
                },
                {
                    name: "channelSounding",
                    displayName: "Channel Sounding",
                    default: false,
                    hidden: isNotChannelSoundingSupported,
                    onChange: onChannelSoundingChange,
                },
                {
                    name: "connectionMonitorRole",
                    displayName: "Connection Monitor",
                    default: false,
                    hidden: false,
                }
            ]
        },
        {
            name: "enableGattBuilder",
            displayName: "Enable GATT Builder",
            default: false,
            hidden: true,
            onChange: onEnableGattBuildeChange
        },
        {
            name: "gattBuilder",
            displayName: "Custom GATT",
            description: "Adding services and characteristic ",
            default: false,
            hidden: true
        },
        {
            name: "keyNode",
            displayName: "Key Node",
            default: false,
            hidden: true,
        },
        csScript.config,
        profilesScript.config,
        radioScript.config,
        generalScript.config,
        bondMgrScript.config,
        advSetScript.config,
        centralScript.config,
        observerScript.config,
        peripheralScript.config,
        broadcasterScript.config,
        l2capScript.config,
    ],

    dependencies: {
        modules: { "/ti/devices/CCFG": ["srcClkLF"] },
		// This function will be run when the dependent module is added/removed or if any configurables listed in the map are changed
		onModuleChanged: function (inst, depInst, moduleName, configurable) {
            let ui = inst.$uiState;
			if (depInst) {
				if (depInst.srcClkLF == "LF RCOSC") {
                    inst.useRcosc = true;
                    ui.peripheralExtraLFOSCPPM.hidden = false;
                }
                else {
                    inst.useRcosc = false;
                    ui.peripheralExtraLFOSCPPM.hidden = true;

                }
			}
		}
	},

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
    radioScript.validate(inst, validation);
    generalScript.validate(inst, validation);
    bondMgrScript.validate(inst, validation);
    advSetScript.validate(inst, validation);
    centralScript.validate(inst, validation);
    observerScript.validate(inst, validation);
    peripheralScript.validate(inst, validation);
    broadcasterScript.validate(inst, validation);
    profilesScript.validate(inst, validation);
    csScript.validate(inst, validation);
    l2capScript.validate(inst, validation);

    // Throw a warning on the useRcosc configurable when oadProject and useRcosc are set to true,
    // to indicate the user that the CCFG LF clock configuration should be done at the BIM project
    if(inst.oadProject && inst.useRcosc)
    {
        validation.logWarning("For OAD projects, the LF Clock configuration is set in the CCFG file of the BIM project", inst, "useRcosc");
    }

    // Throw a "Preview" warning to Health Toolkit and Extended Advertisement when enabled.
    if (inst.healthToolkit)
    {
        validation.logWarning("Health Toolkit preview", inst, "healthToolkit");
    }

    // Throw a "Preview" warning to Power Control when enabled
    if (inst.powerControl)
    {
        validation.logWarning("Power Control preview (Not yet certified)", inst, "powerControl");
    }

    // Throw a "Preview" warning to RSSI Monitor when enabled
    if (inst.rssiMonitor)
    {
        validation.logWarning("RSSI Monitor preview", inst, "rssiMonitor");
    }
}

/*
 *  ======== isConnectableDeviceRole ========
 * Returns true if the device role is of a connectable role
 * and false if not
 * @param inst  - Module instance containing the config that changed
 */
function isConnectableDeviceRole(inst)
{

    return inst.deviceRole == "BROADCASTER_CFG" ||
           inst.deviceRole == "OBSERVER_CFG" ||
           inst.deviceRole == "BROADCASTER_CFG+OBSERVER_CFG" ?
           false : true
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
    if(!isConnectableDeviceRole(inst))
    {
        inst.maxConnNum = 0;
        inst.maxPDUNum = 0;
        inst.bondManager = false;
        ui.peerConnParamUpdateRejectInd.hidden = true;
    }
    else
    {
        inst.maxConnNum = generalScript.maxConnNumDefaultValue();
        inst.maxPDUNum = 5;
        inst.bondManager = true;
        ui.peerConnParamUpdateRejectInd.hidden = false;
    }

    if ( inst.deviceRole.includes("PERIPHERAL_CFG") )
    {
        // Connection Handover is for peripheral role only
        ui.connectionHandover.hidden = false;
    }
    else
    {
        // Connection Handover is for peripheral role only
        inst.connectionHandover = false;
        ui.connectionHandover.hidden = true;
    }

    inst.calledFromDeviceRole = true;
    // Hide/Unhide groups since the device role was changed
    changeGroupsState(inst,ui);
    inst.calledFromDeviceRole = false;

    // Change the Periodic configurable state
    onExtAdvChange(inst,ui);

    // Enable bondFailAction only when using Central role
    if(inst.deviceRole.includes("CENTRAL_CFG"))
    {
        ui.bondFailAction.hidden = false;
    }
    else
    {
        ui.bondFailAction.hidden = true;
    }

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
 * Show/hide the Profiles group and parameters depanding on the
 * values of basicBLE and basicBLEProfiles configurables.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onBasicBLEChange(inst,ui)
{
    if(inst.basicBLE)
    {
        inst.deviceInfo = true;
        inst.profiles = [];
        inst.hideBasicBLEGroup = false;
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "profiles_module"), inst.hideBasicBLEGroup, ui);

        // Display the profiles selection only when basicBLEProfiles is set
        if(!inst.basicBLEProfiles)
        {
            inst.deviceInfo = false;
            ui.deviceInfo.hidden = true;
            ui.profiles.hidden = true;
        }
    }
    else
    {
        inst.deviceInfo = false;
        inst.profiles = [];
        inst.hideBasicBLEGroup = true;
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "profiles_module"), inst.hideBasicBLEGroup, ui);
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
    changeGroupsState(inst,ui);
}

/*
 *  ======== onExtAdvChange ========
 * Lock or unlock the deviceRole configurable,
 * disable/enable the option to change the deviceRole.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onExtAdvChange(inst,ui)
{
    // Hide/UnHide periodicAdv if extended advertising is enabled and the Broadcaster/Peripheral roles
    // is used
    inst.extAdv && (inst.deviceRole.includes("BROADCASTER_CFG") || inst.deviceRole.includes("PERIPHERAL_CFG")) ?
    ui.periodicAdv.hidden = false : ui.periodicAdv.hidden = true;
    // Hide/UnHide periodicAdv if extended advertising is enabled and the Observer/Central roles
    // is used
    inst.extAdv && (inst.deviceRole.includes("OBSERVER_CFG") || inst.deviceRole.includes("CENTRAL_CFG")) ?
    ui.periodicAdvSync.hidden = false : ui.periodicAdvSync.hidden = true;
}

/*
 *  ======== ondisableConfigChange ========
 * When using exlude SM than GapBondMgr should be,
 * removed.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function ondisableConfigChange(inst, ui)
{
    if ( inst.disableConfig == true )
    {
        inst.bondManager = false;
    }
    else
    {
        inst.bondManager = true;
    }

    changeGroupsState(inst,ui);
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
 *  ======== onHideHealthToolkitChange ========
 * disable/enable the Health Toolkit configurable.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onHideHealthToolkitChange(inst,ui)
{
    inst.hideHealthToolkit ? ui.healthToolkit.hidden = true :
                             ui.healthToolkit.hidden = false;
}

/*
 *  ======== onEnableGattBuildeChange ========
 * Lock or unlock the enableGattBuilder configurable,
 * disable/enable the option to change the enableGattBuilder.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onEnableGattBuildeChange(inst,ui)
{
    inst.enableGattBuilder ? ui.gattBuilder.hidden = false :
                             ui.gattBuilder.hidden = true;
}

/*
 *  ======== onChannelSoundingChange ========
 * Handles the change of the channelSounding configuration.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onChannelSoundingChange(inst, ui) {

    // Hide/UnHide the Channel Sounding Group
    inst.channelSounding ? inst.hideChannelSoundingGroup = false : inst.hideChannelSoundingGroup = true;
    Common.hideGroup(Common.getGroupByName(inst.$module.config, "channelSoundingConfig"), inst.hideChannelSoundingGroup, ui);

    // The ranging client external control mode should be hidden
    ui.rangingClientExtCtrlMode.hidden = true;

    // The antenna max values should always be hidden
    ui.antennasMuxValues.hidden = true;

    // Show the CS Measure Distance Results Mode option only when the Measure Distance is enabled
    ui.csMeasureResultsMode.hidden = inst.csMeasureDistance === false;

    // Reset antennas values
    inst.numAntennas = 1;
    inst.antennasMuxValues = 0x0;
}

/*
 *  ======== onL2CAPCOCChange ========
 * Add/remove the L2CAP module
 *
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onL2CAPCOCChange(inst,ui)
{
    inst.hideL2CAPGroup = inst.L2CAPCOC ? false : true;
    Common.hideGroup(Common.getGroupByName(inst.$module.config, "l2capConfig"), inst.hideL2CAPGroup, ui);
}

/*
 * ======== changeGroupsState ========
 * Hide/Unhide groups, according to the selected features/deviceRole
 *
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function changeGroupsState(inst,ui)
{
    if(!isConnectableDeviceRole(inst))
    {
        // Hide Bond Manager
        inst.hideBondMgrGroup = true;
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "bondMgrConfig"), inst.hideBondMgrGroup, ui);
    }
    else
    {
        // Show/Hide Bond Manager
        inst.hideBondMgrGroup = inst.bondManager ? false : true;
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "bondMgrConfig"), inst.hideBondMgrGroup, ui);
    }

    // Hide/UnHide Peripheral Group
    inst.deviceRole.includes("PERIPHERAL_CFG") ? inst.hidePeripheralGroup = false : inst.hidePeripheralGroup = true;
    Common.hideGroup(Common.getGroupByName(inst.$module.config, "peripheralConfig"), inst.hidePeripheralGroup, ui);

    // Hide/UnHide Broadcaster Group
    inst.deviceRole.includes("BROADCASTER_CFG") || inst.deviceRole.includes("PERIPHERAL_CFG") ? inst.hideBroadcasterGroup = false : inst.hideBroadcasterGroup = true;
    Common.hideGroup(Common.getGroupByName(inst.$module.config, "broadcasterConfig"), inst.hideBroadcasterGroup, ui);

    // Hide/UnHide Central Group
    inst.deviceRole.includes("CENTRAL_CFG") ? inst.hideCentralGroup = false : inst.hideCentralGroup = true;
    Common.hideGroup(Common.getGroupByName(inst.$module.config, "centralConfig"), inst.hideCentralGroup, ui);

    // Hide/UnHide Observer Group
    inst.deviceRole.includes("OBSERVER_CFG") || inst.deviceRole.includes("CENTRAL_CFG") ? inst.hideObserverGroup = false : inst.hideObserverGroup = true;
    Common.hideGroup(Common.getGroupByName(inst.$module.config, "observerConfig"), inst.hideObserverGroup, ui);


    if(isConnectableDeviceRole(inst))
    {
        ui.L2CAPCOC.hidden = false;
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "l2capConfig"), inst.hideL2CAPGroup, ui);
    }
    else
    {
        ui.L2CAPCOC.hidden = true;
        Common.hideGroup(Common.getGroupByName(inst.$module.config, "l2capConfig"), true, ui);
    }
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
        // Add the BLE libs (oneLib, stackWrapper and Symbols) according to the
        // board/device that is being used.
        // There are 3 different folders (cc26x2r1, cc13x2r1 and cc1352p)
        // Each device should use it from the appropriate folder.
        const devFamily = Common.device2DeviceFamily(system.deviceData.deviceId);

        let basePath = "";

        if( devFamily == "DeviceFamily_CC23X0R5"  ||
            devFamily == "DeviceFamily_CC23X0R2"  ||
            devFamily == "DeviceFamily_CC23X0R22" ||
            devFamily == "DeviceFamily_CC23X0R53")
        {
            // Add OneLib library
            basePath = `ti/ble/lib/CC2340R5/OneLib/lib/${toolchain}/m0p`
            libs.push(basePath + "/OneLib.a");
            // Add StackWrapper library
            basePath = `ti/ble/lib/CC2340R5/StackWrapper/lib/${toolchain}/m0p`
            libs.push(basePath + "/StackWrapper.a");
        }
        else if(devFamily == "DeviceFamily_CC27XX")
        {
            // Add OneLib library
            basePath = `ti/ble/lib/CC27XXX10/OneLib/lib/${toolchain}/m33f`
            libs.push(basePath + "/OneLib.a");
            // Add StackWrapper library
            basePath = `ti/ble/lib/CC27XXX10/StackWrapper/lib/${toolchain}/m33f`
            libs.push(basePath + "/StackWrapper.a");
        }
    }

    // Add the Channel Sounding related libs
    let csLibs = csScript.getLibs(inst)
    if(csLibs.length > 0)
    {
        libs.push(csLibs);
    }

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
}

/*
 *  ======== getOpts ========
 */
function getOpts(mod)
{
    let result = [];

    let buildConfigDefines = getBuildConfigOpts();
    let appConfigDefines = getAppConfigOpts();
    let csConfigDefines = csScript.getOpts(mod);

    result.push(buildConfigDefines);
    result.push(appConfigDefines);

    // Add the Channel Sounding related defines
    if(csConfigDefines.length > 0)
    {
        result.push(csConfigDefines);
    }

    return result.flat(Infinity);
}

/*
 *  ======== getBuildConfigOpts ========
 */
function getBuildConfigOpts()
{
    const bleMod = system.modules["/ti/ble/ble"].$static;
    const devFamily = Common.device2DeviceFamily(system.deviceData.deviceId);
    let result = [];

    result.push("-DHOST_CONFIG="+bleMod.deviceRole);
    result.push("-D"+bleMod.trensLayer);

    bleMod.extAdv && result.push("-DUSE_AE");

    if(bleMod.extAdv)
    {
        bleMod.periodicAdv && result.push("-DUSE_PERIODIC_ADV");
        bleMod.periodicAdvSync && result.push("-DUSE_PERIODIC_SCAN");
    }

    bleMod.gattDB && result.push("-DGATT_DB_OFF_CHIP");
    bleMod.gattNoClient && result.push("-DGATT_NO_CLIENT");
    bleMod.bondManager && result.push("-DGAP_BOND_MGR");
    bleMod.L2CAPCOC && result.push("-DV41_FEATURES=L2CAP_COC_CFG", "-DBLE_V41_FEATURES=V41_FEATURES");
    bleMod.delayingAttReadReq && result.push("-DATT_DELAYED_REQ");
    bleMod.gattBuilder && result.push("-DUSE_GATT_BUILDER");

    if ( bleMod.deviceRole.includes("PERIPHERAL_CFG") )
    {
        bleMod.connectionHandover && result.push("-DCONNECTION_HANDOVER");
    }
    bleMod.connectionMonitorRole && result.push("-DCONNECTION_MONITOR");

    bleMod.channelSounding && result.push("-DCHANNEL_SOUNDING");

    return result;
}

/*
 *  ======== getAppConfigOpts ========
 */
function getAppConfigOpts()
{
    const bleMod = system.modules["/ti/ble/ble"].$static;
    const ccfg = system.modules["/ti/devices/CCFG"];
    const devFamily = Common.device2DeviceFamily(system.deviceData.deviceId);
    let result = [];

    result.push("-DSYSCFG");
    result.push("-DMAX_NUM_BLE_CONNS="+bleMod.maxConnNum,
                "-DGATT_MAX_PREPARE_WRITES="+bleMod.maxNumPrepareWrites,
                "-DEXTENDED_STACK_SETTINGS="+bleMod.extendedStackSettings);

    result.push("-DCFG_MAX_NUM_RL_ENTRIES="+bleMod.maxRLSize);
    result.push("-DCFG_MAX_NUM_AL_ENTRIES="+bleMod.maxALSize);

    bleMod.bondManager && result.push("-DGAP_BONDINGS_MAX="+bleMod.maxBonds);

    bleMod.disableDisplayModule && result.push("-DDisplay_DISABLE_ALL");
    bleMod.halAssert && result.push("-DEXT_HAL_ASSERT");
    bleMod.dontTransmitNewRpa && result.push("-DDONT_TRANSMIT_NEW_RPA");
    bleMod.peerConnParamUpdateRejectInd && result.push("-DNOTIFY_PARAM_UPDATE_RJCT");
    bleMod.noOsalSnv && result.push("-DNO_OSAL_SNV");
    bleMod.icallEvents && result.push("-DICALL_EVENTS");
    bleMod.icallStackAddress && result.push("-DICALL_STACK0_ADDR");
    bleMod.appExternalControlMode && result.push("-DAPP_EXTERNAL_CONTROL");

    result.push("-DICALL_MAX_NUM_ENTITIES="+bleMod.maxNumEntIcall,
                "-DICALL_MAX_NUM_TASKS="+bleMod.maxNumIcallEnabledTasks,
                "-DOSAL_CBTIMER_NUM_TASKS=1");

    bleMod.ledDebug && result.push("-DLED_DEBUG");
    bleMod.oadDebug && result.push("-DOAD_DEBUG");
    bleMod.oadFeature && result.push("-DFEATURE_OAD");
    bleMod.oadBleSecurity && result.push("-DOAD_BLE_SECURITY");
    bleMod.adaptivity && result.push("-DSDAA_ENABLE");

    // Health Toolkit parameters
    if (bleMod.healthToolkit == true){
        result.push("-DBLE_HEALTH");
    }

    // Power control parameter
    if (bleMod.powerControl == true){
        result.push("-DPOWER_CONTROL");
    }

    // Rssi monitor parameter
    if (bleMod.rssiMonitor == true){
        result.push("-DRSSI_MONITOR");
    }

    if ((_.isEqual(bleMod.deviceRole, "PERIPHERAL_CFG")) || (bleMod.ptm) || bleMod.appExternalControlMode)
    {
        result.push("-DNPI_USE_UART");
    }
    if(bleMod.ptm)
    {
        let flowCtrlValue = bleMod.flowControl == "Disabled" ? 0 : 1;
        result.push("-DPTM_MODE", "-DNPI_FLOW_CTRL="+flowCtrlValue);
    }

    if( (bleMod.deviceRole.includes("OBSERVER_CFG") || bleMod.deviceRole.includes("CENTRAL_CFG")) && bleMod.advReportChanNum )
    {
        result.push("-DADV_RPT_INC_CHANNEL=1");
    }

    return result;
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
    dependencyModule = dependencyModule.concat(centralScript.moduleInstances(inst));
    dependencyModule = dependencyModule.concat(peripheralScript.moduleInstances(inst));
    dependencyModule = dependencyModule.concat(broadcasterScript.moduleInstances(inst));
    dependencyModule = dependencyModule.concat(csScript.moduleInstances(inst));

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
    displayName: "BLE",
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

        "/ti/utils/build/GenLibs.cmd.xdt":
        {
            modName: "/ti/ble/ble",
            getLibs: getLibs
        },
        "/ti/utils/build/GenOpts.opt.xdt": {
            modName: "/ti/ble/ble",
            getOpts: getOpts
        }
    }
};

/*
 *  ======== exports ========
 *  Export the BLE module
 */
exports = bleModule;
