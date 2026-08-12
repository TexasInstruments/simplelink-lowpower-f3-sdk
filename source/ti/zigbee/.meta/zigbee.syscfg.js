/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2025, Texas Instruments Incorporated
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

const hsm_fw_size = 0x00018000;

// Get PM Settings script
const common = system.getScript("/ti/zigbee/zigbee_common.js");
const pmScript = system.getScript("/ti/zigbee/pm/zigbee_pm.syscfg.js");
const rfScript = system.getScript("/ti/zigbee/rf/zigbee_rf.syscfg.js");
const networkScript = system.getScript("/ti/zigbee/network/zigbee_network.syscfg.js");
const subsections = [pmScript, rfScript, networkScript];

const modulelongDescription = `TI implementation of the ZigBee network stack, \
building the ZigBee 3.0 Specification on top of the IEEE 802.15.4 Specification.`

const deviceTypeLongDescription = `The logical device type for the ZigBee node.`

/* Static module definition for zigbee module */
const moduleStatic = {
    config: [
        {
            name: "loggingEnabled",
            displayName: "Enable Logging",
            hidden : false,
            description: `This setting will enable logging for the Zigbee Stack`,
            default: false
        },
        {
            name: "zigbeeRevision",
            displayName: "Zigbee Pro 3.0 Revision",
            hidden : false,
            description: `The revision of the Zigbee Pro stack to use`,
            default: "r23",
            onChange:onRevisionChange,
            options: [
                { name: "r22", displayName: "Revision 22" },
                { name: "r23", displayName: "Revision 23" }
            ]
        },
        /* Device Type Configurable */
        {
            name: "deviceType",
            displayName: "Enabled Device Types",
            description: "The logical device type for the ZigBee node.",
            longDescription: deviceTypeLongDescription,
            default: [common.defaultDeviceType],
            minSelections: 1,
            onChange: onDeviceTypeChange,
            options: [
                {name: "zc", displayName: "ZigBee Coordinator"},
                {name: "zr", displayName: "ZigBee Router"},
                {name: "zed", displayName: "ZigBee End Device"},
                {name: "mac", displayName: "Zigbee MAC Only Device"},
                {name: "gpd", displayName: "Green Power Device"},
            ]
        },
        {
            name: "macInterface",
            displayName: "MAC Interface",
            hidden: true,
            default: "api",
            options: [
                {name: "api", displayName: "API"},
                // {name: "uart", displayName: "UART"}
            ],

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
            name: "zgpDirectEnabled",
            displayName: "Enable Green Power Direct Support",
            hidden : !common.defaultDeviceTypeIsFFD,
            description: `Ability to receive any Green Power frame in both direct mode and in tunneled mode`,
            default: false
        },
        {
            name: "preInstalledNwkEnabled",
            displayName: "Enable use of APIs to Join a Pre-Installed Network",
            hidden : common.defaultDeviceTypeIsGPD,
            description: `Allow factory new devices join using pre-installed network parameters using the APIs in zb_bdb_preinst_nwk.h`,
            default: false
        },
        {
            name: "disableAPSFragmentation",
            displayName: "Disable APS fragmentation",
            hidden : true,
            description: `Disable APS Fragmentation feature in the stack (Only for R22)`,
            default: false
        },
        {
            name: "adcNoiseEnabled",
            displayName: "Enable ADC Noise Dependency",
            description: "Include the ADC Noise dependency for specific configurations.",
            default: true,
            hidden: true
        },
        {
            name: "enableMACSecurity",
            displayName: "Enable MAC security",
            hidden : common.defaultDeviceTypeIsMAC,
            description: "Allow MAC-level security for devices with MAC roles",
            default: true
        },
        pmScript.config,
        rfScript.config,
        networkScript.config,
    ],
    moduleInstances: moduleInstances,
    modules: modules,
    validate: validate
};

/* Validation function for the network submodule */
function validate(inst, validation)
{
    pmScript.validate(inst, validation);
    networkScript.validate(inst, validation);

    if(inst.deviceType.includes("zc"))
    {
        if(inst.deviceType.includes("zed"))
        {
            validation.logError(
                "ZC and ZED roles cannot be enabled simultaneously",
                inst, "deviceType"
            );
        }
    }

    if(inst.deviceType.includes("mac") && inst.deviceType.length > 1)
    {
        validation.logError(
            "MAC role cannot be enabled with any other roles",
            inst, "deviceType"
        );
    }

    if(inst.deviceType.includes("mac") && inst.zigbeeRevision === "r22")
    {
        validation.logInfo(
            "Zigbee Revision has no effect when using MAC role",
            inst, "zigbeeRevision"
        );
    }

    if(inst.deviceType.includes("gpd") && inst.deviceType.length > 1)
    {
        validation.logError(
            "GPD role cannot be enabled with any other roles",
            inst, "deviceType"
        );
    }
}



/* Submodule instance definitions */
function moduleInstances(inst)
{
    const submodules = [];

    if(inst.deviceType.includes("gpd"))
    {
        submodules.push({
            name: "gpdRf",
            displayName: "Radio",
            description: "Configure Green Power default radio channels",
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
                name: "LogModuleZigbeeOSIF",
                displayName: "Zigbee OSIF Log Configuration",
                moduleName: "/ti/log/LogModule",
                collapsed: true,
                args: {
                    $name: "LogModule_Zigbee_OSIF",
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
            phyType: "ieee_802_15_4",
            codeExportConfig: {
                  $name: "zb_ieee_15_4_phy_code_export",
                  symGenMethod: "Automatic",
            }
        }
    });

      if (inst.adcNoiseEnabled) {
    submodules.push({
        name: "adcNoise",
        displayName: "ADC_NOISE",
        moduleName: "/ti/devices/radioconfig/phy_groups/ble",
        collapsed: true,
        hidden: true,
        args: {
            $name: "zb_ble_adc_noise",
            phyType: "adc_noise",
            codeExportConfig: {
                $name: "zb_ble_adc_noise_code_export",
                symGenMethod: "Custom",
                phyAbbr: true
            }
        }
    });
      }
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
        name: "aesccm",
        displayName: "AESCCM",
        moduleName: "/ti/drivers/AESCCM",
        collapsed: true,
        args: {
            $name: "CONFIG_AESCCM_ZB"
        }
    });

    submodules.push({
        name: "sha2",
        displayName: "SHA2",
        moduleName: "/ti/drivers/SHA2",
        collapsed: true,
        args: {
            $name: "CONFIG_SHA2_ZB"
        }
    });

    submodules.push({
        name: "ecdh",
        displayName: "ECDH",
        moduleName: "/ti/drivers/ECDH",
        collapsed: true,
        args: {
            $name: "CONFIG_ECDH_ZB"
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

    // TODO: Need a more sustainable solution. Used to get this info from getLinkerDefs
    // in source/ti/devices/.meta/DriverLib.syscfg.js
    // But now it's dependent on a module which breaks sysconfigs restrictions
    const dev2FlashSize = {
        "CC2340R2": 0x00040000,
        "CC2340R5": 0x00080000,
        "CC2745R7": 0x000C0000 - hsm_fw_size,
        "CC2755R10": 0x00100000 - hsm_fw_size,
        "CC2745R10": 0x00100000 - hsm_fw_size,
        "CC2755P10": 0x00100000 - hsm_fw_size,
    }

    // Iterate over dev2FlashSize keys as regex patterns and match against system.deviceData.deviceId
    let matchedFlashSize = undefined;
    for (const pattern in dev2FlashSize) {
        const regex = new RegExp(pattern);
        if (regex.test(system.deviceData.deviceId)) {
            matchedFlashSize = dev2FlashSize[pattern];
            break;
        }
    }
    const flashSize = matchedFlashSize !== undefined ? matchedFlashSize : 0x00040000;
    const flashBase = 0x00000000;
    if (inst.deviceType.includes("gpd"))
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
        let flashLength = (inst.deviceType.includes("zr")) ? 0x2000 : 0x1000;

        submodules.push({
            name: "nvs",
            displayName: "NVS",
            moduleName: "/ti/drivers/NVS",
            collapsed: true,
            args: {
                $name: "CONFIG_NVSINTERNAL_ZB",
                internalFlash: {
                    $name: "zb_nvs",
                    regionBase: flashBase + flashSize - flashLength,
                    regionSize: flashLength
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

/* Makes the device type read only when deviceTypeReadOnly is true */
function onDeviceTypeChange(inst, ui)
{
    if(inst.deviceType.includes("mac"))
    {
        ui.zgpDirectEnabled.hidden = true;
        ui.preInstalledNwkEnabled.hidden = true;
        ui.macInterface.hidden = false;
        ui.enableMACSecurity.hidden = false;
    }
    else if(inst.deviceType.includes("zr") || inst.deviceType.includes("zc") || inst.deviceType.includes("zed"))
    {
        ui.zgpDirectEnabled.hidden = false;
        ui.preInstalledNwkEnabled.hidden = false;
        ui.macInterface.hidden = true;
        ui.enableMACSecurity.hidden = true;
    }
    else
    {
        inst.zgpDirectEnabled = false;
        ui.zgpDirectEnabled.hidden = true;

        inst.preInstalledNwkEnabled = false;
        ui.preInstalledNwkEnabled.hidden = true;

        ui.macInterface.hidden = true;
    }

    let subsection = null;
    for(subsection of subsections)
    {
        if(subsection["onDeviceTypeChange"] !== undefined)
        {
            subsection.onDeviceTypeChange(inst, ui);
        }
    }
}

/* Make updates on revision change */
function onRevisionChange(inst, ui)
{
    if(inst.zigbeeRevision.includes("r22") && !(inst.deviceType.includes("gpd")) && !(inst.deviceType.includes("mac")))
    {
        ui.disableAPSFragmentation.hidden = false;
    }
    else
    {
        ui.disableAPSFragmentation.hidden = true;
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
        let lib_names = [];
        let log_suffix = "";
        let lib_dir = ((inst.$static.zigbeeRevision === "r22") && !(inst.$static.deviceType.includes("mac"))) ? "/zboss_stable" : "/zboss_r23";
        let revision = (inst.$static.deviceType.includes("mac")) ? "r23" : inst.$static.zigbeeRevision;
        if(inst.$static.deviceType.includes("gpd"))
        {
            lib_dir = ""
        }

        if(inst.$static.loggingEnabled)
        {
            log_suffix = "_log";
        }

        let pre_installed_nwk_suffix = "";
        if(inst.$static.preInstalledNwkEnabled)
        {
            pre_installed_nwk_suffix = "_preinst_nwk"
        }

        let aps_frag_suffix = "";
        if(inst.$static.disableAPSFragmentation && (inst.$static.zigbeeRevision === "r22"))
        {
            aps_frag_suffix = "_no_aps_frag";
        }

        if(inst.$static.deviceType.includes("zc"))
        {
            if(inst.$static.zgpDirectEnabled)
            {
                lib_names.push("zb_core_zc_zr_gp_combo" + pre_installed_nwk_suffix + aps_frag_suffix);
            }
            else
            {
                lib_names.push("zb_core_zc_zr"  + pre_installed_nwk_suffix  + aps_frag_suffix);
            }
            lib_names.push("zb_zcl_zc_zr" + aps_frag_suffix, "zb_zdo_zc_zr" + pre_installed_nwk_suffix + aps_frag_suffix, "zb_ti_platform_zc_zr" + log_suffix);
        }
        else if(inst.$static.deviceType.includes("zr"))
        {
            if(inst.$static.zgpDirectEnabled)
            {
                lib_names.push("zb_core_zr_gp_combo"  + pre_installed_nwk_suffix  + aps_frag_suffix);
            }
            else
            {
                lib_names.push("zb_core_zr"  + pre_installed_nwk_suffix  + aps_frag_suffix);
            }
            lib_names.push("zb_zcl_zr" + aps_frag_suffix, "zb_zdo_zr" + pre_installed_nwk_suffix + aps_frag_suffix, "zb_ti_platform_zc_zr" + log_suffix);
        }
        else if(inst.$static.deviceType.includes("zed"))
        {
            if(inst.$static.zgpDirectEnabled)
            {
                lib_names.push("zb_core_zed_target_plus" + pre_installed_nwk_suffix + aps_frag_suffix);
            }
            else
            {
                lib_names.push("zb_core_zed" + pre_installed_nwk_suffix  + aps_frag_suffix);
            }
            lib_names.push("zb_zcl_zed" + aps_frag_suffix, "zb_zdo_zed"  + pre_installed_nwk_suffix + aps_frag_suffix, "zb_ti_platform_zed" + log_suffix);
        }
        else if(inst.$static.deviceType.includes("mac"))
        {
            if (inst.$static.enableMACSecurity)
            {
                lib_names.push("zb_mac_secur",  "zb_ti_platform_zc_zr_secur" + log_suffix);
            }
            else
            {
                lib_names.push("zb_mac",  "zb_ti_platform_zc_zr" + log_suffix);
            }

            if(inst.$static.macInterface !== "api")
            {
                lib_names.push("zb_macsplit_soc");
            }
        }
        else
        {
            lib_names.push("zb_core_gpd");
        }

        lib_names.forEach((lib_name) =>
        {
            let lib_with_rev = (lib_name.includes("gpd") || lib_name.includes("mac")) ? lib_name: lib_name.replace("zb_", `zb_${revision}_`)
            libs.push(`third_party/zigbee/libraries${lib_dir}/${lib_with_rev}/lib/${toolchain}/${GenLibs.getDeviceIsa()}/${lib_with_rev}.a`)
        });
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

    const sysconfig_products = ["simplelink_lowpower_f3_sdk", "TI_ZIGBEE_SDK", "SIMPLELINK_LOWPOWER_F3_TEST_SDK"];
    let sdkPath = undefined;
    for(let i = 0; i < sysconfig_products.length; i++)
    {
        sdkPath = system.getProducts().find(p => p.name === sysconfig_products[i]);
        if(sdkPath !== undefined)
        {
            // WARNING using system.utils.path.posix breaks the paths on windows
            sdkPath = system.utils.path.parse(system.utils.path.parse(sdkPath.path).dir).dir;
            sdkPath = system.utils.path.resolve(sdkPath).replaceAll('\\','/');
            break;
        }
    }

    if((inst.$static.zigbeeRevision === "r22") && !(inst.$static.deviceType.includes("gpd")) && !(inst.$static.deviceType.includes("mac")))
    {
        result.push("-DZBOSS_REV22");
        result.push(`-I${sdkPath}/source/third_party/zigbee/zboss_stable/include`);
        console.log(`Zigbee SDK Path: ${inst.$static.deviceType}`);
    }
    else if(((inst.$static.zigbeeRevision === "r23") || (inst.$static.deviceType.includes("mac"))) && !(inst.$static.deviceType.includes("gpd")))
    {

        result.push("-DZBOSS_REV23");
        result.push(`-I${sdkPath}/source/third_party/zigbee/zboss_r23/include`);
        result.push(`-I${sdkPath}/source/third_party/zigbee/zboss_r23/thirdparty/uECC`);
        result.push(`-I${sdkPath}/source/third_party/zigbee/zboss_r23/thirdparty/emill`);
        result.push(`-I${sdkPath}/source/third_party/zigbee/zboss_r23/thirdparty/nacl`);
    }

    if(inst.$static.deviceType.includes("zc") || inst.$static.deviceType.includes("zr") || inst.$static.deviceType.includes("zed"))
    {
        result.push("-DZB_DEFAULT_VENDOR");
    }

    if(inst.$static.deviceType.includes("zc"))
    {
        result.push("-DZB_COORDINATOR_ROLE", "-DZB_ROUTER_ROLE", "-DZB_CONFIG_ROLE_ZC");
        if(inst.$static.zgpDirectEnabled)
        {
            result.push("-DZB_ENABLE_ZGP_COMBO");
        }
    }
    else if(inst.$static.deviceType.includes("zr"))
    {
        result.push("-DZB_ROUTER_ROLE", "-DZB_CONFIG_ROLE_ZC");
        if(inst.$static.zgpDirectEnabled)
        {
            result.push("-DZB_ENABLE_ZGP_COMBO");
        }
    }
    else if(inst.$static.deviceType.includes("zed"))
    {
        result.push("-DZB_ED_ROLE", "-DZB_CONFIG_ROLE_ZED");
        if(inst.$static.zgpDirectEnabled)
        {
            result.push("-DZB_ENABLE_ZGP_TARGET_PLUS");
        }
    }
    else if(inst.$static.deviceType.includes("mac"))
    {
        // If security is enabled, then add the ZB_MAC_SECURITY macro to allow application to use MAC security APIs
        if (inst.$static.enableMACSecurity)
        {
            result.push("-DZB_MAC_SECURITY");
        }
    }
    else
    {
        result.push("-DZB_ZGPD_ROLE");
    }

    if(inst.$static.preInstalledNwkEnabled)
    {
        result.push("-DZB_BDB_PREINST_NWK_JOINING");
    }

    if((!inst.$static.disableAPSFragmentation && (inst.$static.zigbeeRevision === "r22") && !(inst.$static.deviceType.includes("mac"))))
    {
        // R23 always has APS fragmentation enabled. APS_FRAGMENTATION will be defined within the R23 stack.
        result.push("-DAPS_FRAGMENTATION");
    }
    return result;
}


/*
 *  ======== getLinkerDefs ========
 */
function getLinkerDefs() {
    var linkDef = []
    if ("CC2755P105RHA" ==  system.deviceData.deviceId)
    {
        linkDef.push({ name: "HSM_FW_SIZE", value: hsm_fw_size })
    }
    return linkDef;
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
        },
        "/ti/utils/build/GenMap.cmd.xdt":
        {
            modName: "/ti/zigbee/zigbee",
            getLinkerDefs: getLinkerDefs
        },
    },

};

exports = zigbeeModule;
