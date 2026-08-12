/*
 * Copyright (c) 2019 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== prop_rf_common.js ========
 */

"use strict";

// Get common LPRF utility functions
const common = system.getScript("/ti/common/.meta/lprf_common.js");

// Max and min C type values
const maxUint32t = 0xFFFFFFFF;
const maxUint32tRatUsTime = Math.floor(maxUint32t / 4);
const maxUint32tRatMsTime = Math.floor(maxUint32t / 4000);
const maxInt8t = 127;
const minInt8t = -128;

// Settings for ti/devices/CCFG module
const propRfCCFGSettings = {
	LP_EM_CC2340R5_CCFG_SETTINGS: {}
};

const boardName = common.getDeviceOrLaunchPadName(true);
const ccfgSettings = propRfCCFGSettings[boardName + "_CCFG_SETTINGS"];
const supportedMigrations = {
    LP_EM_CC2340R5: {
        CC2340R5RGE: {},
        CC2340R5RKP: {},
        LP_EM_CC2340R5: {}
    },
};

/*
 * ======== isMigrationValid ========
 * Determines whether a migration from one board/device to another board/device
 * is supported by Prop RF.
 *
 * @returns One of the following Objects:
 *    - {} <--- Empty object if migration is valid
 *    - {warn: "Warning markdown text"} <--- Object with warn property
 *                                           if migration is valid but
 *                                           might require user action
 *    - {disable: "Disable markdown text"} <--- Object with disable property
 *                                              if migration is not valid
 */
function isMigrationValid(currentTarget, migrationTarget)
{
    let migSupported = {
        warn: "This migration requires manual steps and  has not been fully "
        + "tested. See additional documentation for more details"
    };

    if(supportedMigrations[currentTarget]
        && supportedMigrations[currentTarget][migrationTarget])
    {
        migSupported = supportedMigrations[currentTarget][migrationTarget];
    }

    return(migSupported);
}

/*
 * ======== migrate ========
 * Perform stack specific changes to the SysConfig env POST migration
 *
 * @param currTarget - Board/device being migrated FROM
 * @param migrationTarget - Board/device being migrated TO
 * @param env - SysConfig environment providing access to all configurables
 * @param projectName - Optional name of the project being migrated
 *
 * @returns boolean - true when migration is supported and succesful, false when
 *                    migration is not supported and/or unsuccesful
 */
function migrate(currTarget, migrationTarget, env, projectName = null)
{
    const migrationInfo = isMigrationValid(currTarget, migrationTarget);
    let migrationValid = true;
    if(migrationInfo.disable)
    {
        migrationValid = false;
    }

    if(migrationValid)
    {
        /* ======== RF Design Settings ======== */
        const rfDesign = env.system.modules[
            "/ti/devices/radioconfig/rfdesign"].$static;
        const rfDesignSettings = env.system.getScript(
            "/ti/prop_rf/lprf_rf_design_settings.js"
        ).rfDesignSettings;

        if(rfDesignSettings.rfDesign !== undefined)
        {
            let setting = null;
            for(setting in rfDesignSettings)
            {
                if(Object.prototype.hasOwnProperty.call(rfDesignSettings,
                    setting))
                {
                    rfDesign[setting] = rfDesignSettings[setting];
                }
            }
        }

        /* ======== CCFG Settings ======== */
        const device = env.system.modules["/ti/devices/CCFG"].$static;
        const ccfgSettingObj = env.system.getScript(
            "/ti/prop_rf/lprf_ccfg_settings.js"
        ).ccfgSettings;

        let setting = null;
        for(setting in ccfgSettingObj)
        {
            if(Object.prototype.hasOwnProperty.call(ccfgSettingObj, setting))
            {
                device[setting] = ccfgSettingObj[setting];
            }
        }

        const phyInstances = [];
        const customStack = env.system.modules[
            "/ti/devices/radioconfig/custom"].$static;
        _.forIn(customStack, (value, key) =>
        {
            if(key.includes("radioConfig"))
            {
                phyInstances.push(customStack[key]);
            }
        });

        // TODO: Consider removing
        /*
        * The rfPacketErrorRate projects support both Sub-1GHz and 2.4GHz PHYs.
        * By default, the Sub-1GHz PHYs transmit at 14dBm power requiring
        * CCFG_FORCE_VDDR = 1. The 2.4GHz PHYs do not support boost mode. For
        * both to operate at runtime for this project, the TX power of Sub-1GHz
        * phys must be < 14dBm and CCFG_FORCE_VDDR = 0.
        */
        const boardRepresentation = common.convertDeviceToBoard(migrationTarget);
        if((projectName === "rfPacketErrorRate")
            && ((boardRepresentation === "CC1352R1_LAUNCHXL")
            || (boardRepresentation === "CC1352P_2_LAUNCHXL")
            || (boardRepresentation === "CC1352P_4_LAUNCHXL")))
        {
            for(const phy of phyInstances)
            {
                if((phy.txPower) && (phy.txPower === "14"))
                {
                    device.forceVddr = false;
                    phy.txPower = "12.5";
                }
                else if((phy.txPower433) && (phy.txPower433 === "14.5"))
                {
                    device.forceVddr = false;
                    phy.txPower433 = "13";
                }
            }
        }
    }

    return(migrationValid);
}

/*
 * ======== getMigrationMarkdown ========
 * Returns text in markdown format that customers can use to aid in migrating a
 * project between device/boards. It's recommended to use at most 3 bullets,
 * each line should have a maximum of 120 characters, and a header will be
 * automatically added via the consumer of this API.
 *
 * @param currTarget - Board/device being migrated FROM
 *
 * @returns string - Markdown formatted string
 */
function getMigrationMarkdown(currTarget)
{
    const migrationText = `\
* PHY settings are not common across all devices, verify they match your \
desired application and modify accordingly\n
* More Detailed Guidelines: [Link to Docs](/proprietary-rf/proprietary-rf-\
users-guide/sysconfig/proprietary-rf-migration.html#sysconfig-migration-tool)`;

    return(migrationText);
}

/*
 * ======== isValidAddress ========
 * Checks that the address is hex format
 *
 * @param addr    - The address to be validated in string format
 * @returns bool     - True or false depending on if the string format is valid
 */
function isValidAddress(addr)
{
    let isValid = false;

    // Check if the address is in hex format
    if(addr.substring(0, 2) === "0x")
    {
        // Parse the address
        const parsedNum = Number(addr);

        // Check if the parsed number is valid
        if(!isNaN(parsedNum))
        {
            isValid = true;
        }
    }

    return(isValid);
}


/*!
 *  ======== underscoreToCamelCase ========
 *  Convert a string separated by underscores to a camelCase representation
 *
 *  @param underscoreCase  - a string separated by underscores with no spaces
 *
 *  @returns String - the corresponding camelCase representation
 */
function underscoreToCamelCase(underscoreCase)
{
    let camelCase = null;

    // Check the string is not empty and contains no spaces before parsing
    if(underscoreCase.length > 0 && !underscoreCase.includes(" "))
    {
        const splitString = underscoreCase.split("_"); // Split on underscore
        let i = 0;
        for(i = 0; i < splitString.length; i++)
        {
            // Convert first letter of each entry to upper case
            splitString[i] = splitString[i].charAt(0).toUpperCase()
                + splitString[i].slice(1).toLowerCase();
        }

        camelCase = splitString.join("");
    }

    return(camelCase);
}

/*!
 *  ======== isCName ========
 *  Determine if specified id is either empty or a valid C identifier
 *
 *  @param id  - String that may/may not be a valid C identifier
 *
 *  @returns true if id is a valid C identifier OR is the empty
 *           string; otherwise false.
 */
function isCName(id)
{
    if((id !== null && id.match(/^[a-zA-Z_][0-9a-zA-Z_]*$/) !== null)
            || id === "") /* '' is a special value that means "default" */
    {
        return true;
    }
    return false;
}

/*
 * ======== stringToArray ========
 * Convert a comma separated string to an array
 *
 * @param string       - The string to be converted
 * @returns Array      - An array containing the values separated from string
 *                       without whitespace characters
 */
function stringToArray(string)
{
    let splitString = [];

    // Ignore spaces in user input
    const stringNoSpaces = string.replace(/\s/g, "");

    // Check if the string is empty before parsing
    if(stringNoSpaces.length > 0)
    {
        splitString = stringNoSpaces.split(","); // Split based on comma
    }

    return(splitString);
}

/*
 * ======== addPropPhy ========
 * Adds an instance of a proprietary phy in the "Custom" stack module. Used in
 * RF Driver .syscfg files
 *
 * @param stackInst - Object. Instance of the "Custom" stack
 * @param phy       - Object. The phy containing args and codeExportConfig
 *
 * @returns - Object. The instance of the newly added phy
 */
function addPropPhy(stackInst, phy)
{
    // Get the frequency band and phy type of the phy
    const freqBand = phy.args.freqBand;
    const phyType = phy.args["phyType" + freqBand];

    const blackList = ["codeExportConfig", "freqBand", "phyType" + freqBand];

    // Add the instance of the new PHY
    const selectedPhyOptions = stackInst["prop" + freqBand[0]].slice(0);
    selectedPhyOptions.push(phyType);
    stackInst["prop" + freqBand[0]] = selectedPhyOptions;

    // Get the instance of the newly added PHY
    const phyInst = stackInst["radioConfig" + phyType];

    // Set RF settings of the phy instance to match RF Driver project defaults
    let key = null;
    for(key in phy.args)
    {
        if(!blackList.includes(key))
        {
            phyInst[key] = phy.args[key];
        }
    }

    // Set code export settings of phy inst to match RF Driver project defaults
    for(key in phy.args.codeExportConfig)
    {
        if(Object.prototype.hasOwnProperty.call(phy.args.codeExportConfig, key))
        {
            phyInst.codeExportConfig[key] = phy.args.codeExportConfig[key];
        }
    }

    return(phyInst);
}

/*
 * ======== addBlePhy ========
 * Adds an instance of a ble phy in the "Custom" stack module. Used in
 * RF Driver .syscfg files
 *
 * @param stackInst - Object. Instance of the "Custom" stack
 * @param phy       - Object. The phy containing args and codeExportConfig
 *
 * @returns - Object. The instance of the newly added phy
 */
function addBlePhy(stackInst, phy)
{
    // Add the instance of the new PHY
    const selectedPhyOptions = stackInst.ble.slice(0);
    selectedPhyOptions.push(phy.args.phyType);
    stackInst.ble = selectedPhyOptions;

    const blackList = ["codeExportConfig", "phyType"];

    // Get the instance of the newly added PHY
    const phyInst = stackInst["radioConfig" + phy.args.phyType];

    // Set RF settings of the phy instance to match RF Driver project defaults
    let key = null;
    for(key in phy.args)
    {
        if(!blackList.includes(key))
        {
            phyInst[key] = phy.args[key];
        }
    }

    // Set code export settings of phy inst to match RF Driver project defaults
    for(key in phy.args.codeExportConfig)
    {
        if(Object.prototype.hasOwnProperty.call(phy.args.codeExportConfig, key))
        {
            phyInst.codeExportConfig[key] = phy.args.codeExportConfig[key];
        }
    }

    return(phyInst);
}

/*
 * ======== getDropDownOptions ========
 * Finds and returns an array of the "name" property of each option available in
 * the drop-down
 *
 * @param inst       - The module instance to search for the configurable in
 * @param configName - The script name of the configurable to search for
 *
 * @returns - empty array if the configurable is not found, otherwise an array
 * of the "name" property of each option available in the drop-down
 */
function getDropDownOptions(inst, configName)
{
    const dropDownOptions = [];

    const enumDef = common.findConfig(inst.$module.config, configName);

    // Verify the enum was found and it is a drop-down before extracting names
    if(enumDef !== undefined && ("options" in enumDef))
    {
        // Create an array with only the "name" elements
        let option = null;
        for(option of enumDef.options)
        {
            dropDownOptions.push(option.name);
        }
    }

    return(dropDownOptions);
}

exports = {
    isValidAddress: isValidAddress,
    isCName: isCName,
    getDeviceOrLaunchPadName: common.getDeviceOrLaunchPadName,
    underscoreToCamelCase: underscoreToCamelCase,
    stringToArray: stringToArray,
    addPropPhy: addPropPhy,
    addBlePhy: addBlePhy,
    getDropDownOptions: getDropDownOptions,
    maxUint32t: maxUint32t,
    maxUint32tRatUsTime: maxUint32tRatUsTime,
    maxUint32tRatMsTime: maxUint32tRatMsTime,
    maxInt8t: maxInt8t,
    minInt8t: minInt8t,
    ccfgSettings: ccfgSettings,
    isMigrationValid: isMigrationValid,
    migrate: migrate,
    getMigrationMarkdown: getMigrationMarkdown
};
