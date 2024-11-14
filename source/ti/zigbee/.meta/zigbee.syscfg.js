/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

 /*
 *  ======== zigbee.syscfg.js ========
 */

"use strict";

const driverlib = system.getScript("/ti/devices/.meta/DriverLib.syscfg.js");

const modulelongDescription = `TI implementation of the ZigBee network stack, \
building the ZigBee 3.0 Specification on top of the IEEE 802.15.4 Specification.`

const deviceTypeLongDescription = `The logical device type for the ZigBee node.`

/* Static module definition for zigbee module */
const moduleStatic = {
    config: [
        /* Device Type Configurable */
        {
            name: "deviceType",
            displayName: "Device Type",
            description: "The logical device type for the ZigBee node.",
            longDescription: deviceTypeLongDescription,
            default: "zc",
            options: [
                {name: "zc", displayName: "ZigBee Coordinator"},
                {name: "zr", displayName: "ZigBee Router"},
                {name: "zed", displayName: "ZigBee End Device"},
                {name: "gpd", displayName: "Green Power Device"},
                //{name: "znp", displayName: "ZigBee Network Processor"}
            ]
        },
        {
            name: "deviceTypeReadOnly",
            default: false,
            hidden: true,
            onChange: onDeviceTypeReadOnlyChange
        },
        {
            name: "genLibs",
            default: true,
            hidden: true,
        },
        {
            name: "loggingEnabled",
            displayName: "Enable Logging",
            hidden : false,
            description: `This setting will enable logging for the Zigbee Stack`,
            default: false
        },
    ],
    moduleInstances: moduleInstances,
    modules: modules
};


/* Submodule instance definitions */
function moduleInstances(inst)
{
    const submodules = [];

    if(inst.deviceType === "gpd")
    {
        submodules.push({
            name: "gpdRf",
            displayName: "Radio",
            description: "Configure default radio channel",
            moduleName: "/ti/zigbee/gpd/zigbee_gpd_rf",
            collapsed: true
        });
        submodules.push({
            name: "gpdNetwork",
            displayName: "Network",
            description: "Configure Green Power network settings",
            moduleName: "/ti/zigbee/gpd/zigbee_gpd_network",
            collapsed: true
        });
        submodules.push({
            name: "gpdSecurity",
            displayName: "Security",
            description: "Configure Green Power security settings",
            moduleName: "/ti/zigbee/gpd/zigbee_gpd_security",
            collapsed: true
        });
        submodules.push({
            name: "gpdAdvanced",
            displayName: "Advanced",
            description: "Configure Green Power advanced settings",
            moduleName: "/ti/zigbee/gpd/zigbee_gpd_advanced",
            collapsed: true
        });
    }
    else
    {
        submodules.push({
            name: "pm",
            displayName: "Power Management",
            description: "Configure radio power settings",
            moduleName: "/ti/zigbee/pm/zigbee_pm",
            collapsed: true,
            args: {deviceType: inst.deviceType}
        });

        submodules.push({
            name: "rf",
            displayName: "Radio",
            description: "Configure default radio channel",
            moduleName: "/ti/zigbee/rf/zigbee_rf",
            collapsed: true
        });

        submodules.push({
            name: "network",
            displayName: "Network",
            description: "Configure network identification and security "
                         + "settings",
            moduleName: "/ti/zigbee/network/zigbee_network",
            collapsed: true,
            args: {deviceType: inst.deviceType}
        });
    }

    if (inst.loggingEnabled) {
        submodules.push(
            {
                name: "LogModuleZigbeeLLMAC",
                displayName: "Zigbee Stack Low Level MAC Log Configuration",
                moduleName: "/ti/log/LogModule",
                collapsed: true,
                args: {
                    $name: "LogModule_Zigbee_Low_Level_MAC",
                    enable_DEBUG: false,
                    enable_INFO: false,
                    enable_VERBOSE: false,
                    // Only enable WARNING and ERROR enabled by default
                    enable_WARNING: true,
                    enable_ERROR: true
                }
            }
        );

        submodules.push(
            {
                name: "LogModuleZigbeeApp",
                displayName: "Zigbee Application Log Configuration",
                moduleName: "/ti/log/LogModule",
                collapsed: true,
                args: {
                    $name: "LogModule_Zigbee_App",
                    enable_DEBUG: false,
                    enable_INFO: false,
                    enable_VERBOSE: false,
                    // Only enable WARNING and ERROR enabled by default
                    enable_WARNING: true,
                    enable_ERROR: true
                }
            }
        );
    }

    submodules.push({
        name: "phy",
        displayName: "PHY",
        moduleName: "/ti/devices/radioconfig/phy_groups/ieee_15_4",
        collapsed: true,
        hidden: true,
        args: {
            $name: "zb_ieee_15_4_phy",
            codeExportConfig: {
                $name: "zb_ieee_15_4_phy_code_export"
            }
        }
    });

    submodules.push({
        name: "adcNoise",
        displayName: "ADC_NOISE",
        moduleName: "/ti/devices/radioconfig/phy_groups/ble",
        collapsed: true,
        hidden: true,
        args: {
            $name: "zb_ble_adc_noise",
            codeExportConfig: {
                $name: "zb_ble_adc_noise_code_export",
                symGenMethod: "Custom",
                phyAbbr: true
            }
        }
    });

    submodules.push({
        name: "aesecb",
        displayName: "AESECB",
        moduleName: "/ti/drivers/AESECB",
        collapsed: true,
        args: {
            $name: "CONFIG_AESECB_ZB"
        }
    });

    submodules.push({
        name: "rng",
        displayName: "RNG",
        moduleName: "/ti/drivers/RNG",
        collapsed: true,
        args: {
            $name: "CONFIG_RNG_ZB",
        }
    });

    const linkerDefs = driverlib.templates["/ti/utils/build/GenMap.cmd.xdt"].getLinkerDefs();
    const flashSize = linkerDefs.find(function (element) {
        return element.name == "FLASH0_SIZE";
    }).value;
    const flashBase = linkerDefs.find(function (element) {
        return element.name == "FLASH0_BASE";
    }).value;

    if (inst.deviceType === "gpd")
    {
        submodules.push({
            name: "nvs",
            displayName: "NVS",
            moduleName: "/ti/drivers/NVS",
            collapsed: true,
            args: {
                $name: "CONFIG_NVSINTERNAL_ZB",
                internalFlash: {
                    $name: "zb_nvs",
                    regionBase: flashBase + flashSize - 0x16000,
                    regionSize: 0x16000
                }
            }
        });
    }
    else
    {
        submodules.push({
            name: "nvs",
            displayName: "NVS",
            moduleName: "/ti/drivers/NVS",
            collapsed: true,
            args: {
                $name: "CONFIG_NVSINTERNAL_ZB",
                internalFlash: {
                    $name: "zb_nvs",
                    regionBase: flashBase + flashSize - 0x8000,
                    regionSize: 0x8000
                }
            }
        });
    }

    return submodules;
}

/* Static submodule instance definitions */
function modules(inst)
{
    const submodules = [];

    submodules.push({
        name: "rfDesign",
        displayName: "RF Design",
        moduleName: "/ti/devices/radioconfig/rfdesign"
    });

    submodules.push({
        name: "rcl",
        displayName: "RCL",
        moduleName: "/ti/drivers/RCL"
    });

    return submodules;
}

/* Makes the device type read only when deviceTypeReadOnly is true */
function onDeviceTypeReadOnlyChange(inst, ui)
{
    const readOnlyReason = "Device type cannot be configured via SysConfig "
        + "due to differing dependencies. Change device type by importing "
        + "the appropriate project.";

    if(inst.deviceTypeReadOnly === true)
    {
        ui.deviceType.readOnly = readOnlyReason;
    }
    else
    {
        ui.deviceType.readOnly = false;
    }
}

/*
 * ======== getLibs ========
 * Contribute libraries to linker command file
 *
 * @param inst  - zigbee module instance
 * @returns     - Object containing the name of component, array of dependent
 *                components, and array of library names
 */
function getLibs(inst)
{
    // Get device ID and toolchain to select appropriate libs
    const GenLibs = system.getScript("/ti/utils/build/GenLibs.syscfg.js");
    const toolchain = GenLibs.getToolchainDir();
    const libs = [];

    if(inst.$static.genLibs)
    {
        let lib_name = ""
        switch(inst.$static.deviceType)
        {
            case "zc":
            case "zr":
                lib_name = "zb_coordinator_router_roles_gp_combo";
                break;
            case "zed":
                lib_name = "zb_ed_role";
                break;
            case "gpd":
                lib_name = "zb_gpd_role";
                break;
            default:
                lib_name = inst.$static.deviceType;
                break;
        }

        libs.push(`third_party/zigbee/libraries/${lib_name}/lib/${toolchain}/m0p/${lib_name}.a`);
    }


    // Create a GenLibs input argument
    const linkOpts = {
        name: "/ti/zigbee/zigbee",
        deps: ["/ti/drivers", "/ti/drivers/rcl"],
        libs: libs
    };

    return(linkOpts);
}

/*
 *  ======== getOpts ========
 */
function getOpts(inst) {
    let result = []

    switch(inst.$static.deviceType)
    {
        case "zc":
            result.push("-DZB_COORDINATOR_ROLE", "-DZB_ROUTER_ROLE");
            break;
        case "zr":
            result.push("-DZB_ROUTER_ROLE");
            break;
        case "zed":
            result.push("-DZB_ED_ROLE");
            break;
        case "gpd":
            result.push("-DZB_ZGPD_ROLE");
            break;
        default:
            result.push("-DZB_COORDINATOR_ROLE");
            break;
    }

    return result;
}


/* Zigbee module definition */
const zigbeeModule = {
    displayName: "Zigbee",
    description: "Zigbee configuration module",
    longDescription: modulelongDescription,
    moduleStatic: moduleStatic,
    templates: {
        "/ti/zigbee/templates/ti_zigbee_config.h.xdt":
        "/ti/zigbee/templates/ti_zigbee_config.h.xdt",

        "/ti/zigbee/templates/ti_zigbee_config.c.xdt":
        "/ti/zigbee/templates/ti_zigbee_config.c.xdt",

        "/ti/utils/build/GenLibs.cmd.xdt":
        {
            modName: "/ti/zigbee/zigbee",
            getLibs: getLibs
        },
        "/ti/utils/build/GenOpts.opt.xdt": {
            modName: "/ti/zigbee/zigbee",
            getOpts: getOpts
        }
    },

};

exports = zigbeeModule;