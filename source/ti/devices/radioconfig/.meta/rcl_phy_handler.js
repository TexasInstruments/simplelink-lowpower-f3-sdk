/* eslint-disable no-eval */
/* eslint-disable no-bitwise */
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
 *  ======== rcl_phy_handler.js ========
 *  Module to process RCL PHY definitions.
 */

"use strict";

// Common utility functions
const Common = system.getScript("/ti/devices/radioconfig/radioconfig_common.js");
const ConfigPath = Common.ConfigPath;
const ConfigPathRclCommon = Common.ConfigPathRclCommon;

// Other dependencies
const DeviceInfo = Common.getScript("device_info.js");
const RfDesign = Common.getScript("rfdesign");
const CmdHandler = Common.getScript("cmd_handler.js");

// Utility functions shared with SmartRF Studio 8
let Srfs8Util;
if (Common.getDeviceFamily() === "cc27xx") {
    Srfs8Util = system.getScript(ConfigPathRclCommon + "srfs8_util_cc27xx.js");
}
else {
    Srfs8Util = system.getScript(ConfigPathRclCommon + "srfs8_util_cc23xx.js");
}

// Address map for each module
const AddressMap = {};

// Flag used to avoid creating the address map more than once
let AddressMapCreated = false;

// Ensure that PHY Handlers are not created twice
const PhyHandlerCache = {};

// Utility functions
const int2hex = Common.int2hex;

// Register region information
const RegisterRegion = {
    // Range of 16-bit HW registers in normal space, values are zero
    T_HW_ZERO: {id: 0x0, comment: "HW zero region"},
    // Range of 16-bit HW registers in normal space, one register written per access
    T_HW_16BIT: {id: 0x1, comment: "HW 16-bit region"},
    // Range of 16-bit HW registers in alias space, two registers written per access
    T_HW_32BIT: {id: 0x2, comment: "HW 32-bit region"},
    // Address + value pairs of 16-bit HW registers in normal space
    T_HW_SPARSE: {id: 0x3, comment: "HW sparse region"},
    // Range of 16-bit SW registers in RAM, values are zero
    T_RAM_ZERO: {id: 0x4, comment: "RAM zero region"},
    // Range of 16-bit SW registers in RAM, one register written per access
    T_RAM_16BIT: {id: 0x5, comment: "RAM 16-bit region"},
    // Range of 16-bit SW registers in RAM, two registers written per access
    T_RAM_32BIT: {id: 0x6, comment: "RAM 32-bit region"},
    // Address + value pairs of 16 - bit SW registers in RAM
    T_RAM_SPARSE: {id: 0x7, comment: "RAM sparse region"},
    // Bit-mask + value pairs of 16-bit HW registers in normal space
    T_HW_MASKED: {id: 0xB, comment: "HW masked region"}
};

// Map from PHY property to RCL command field
const PhyPropCmdField = {
    subPhy: "common.phyFeatures",
    codedTxRate: "common.phyFeatures",
    frequency: "rfFrequency",
    txPower: "txPower.dBm"
};

// Code formatting
const COMMENT_INDENT = 36;

// Register field defines:
const REGISTER_FIELD_DEFINES_PREFIX = "RCL_REGISTER_FIELD_";
const REGISTER_FIELD_DEFINE_NAME_COL = 65;

// Name for register gaps
const GAP = "< GAP >";

// Exported functions

exports = {
    get: get,
    getUpdatedRfCommands: getUpdatedRfCommands
};

/**
 *  ======== get ========
 *  Get the Command Handler for a PHY layer. Create the
 *  Command Handler if it does not already exist.
 *
 *  @param phyGroup - ble, prop, ieee_154
 *  @param phyID    - short name for the PHY definition
 *  @returns Singleton phy handler for the given phy
 */
function get(phyGroup, phyID) {
    if (!(phyID in PhyHandlerCache)) {
        PhyHandlerCache[phyID] = create(phyGroup, phyID, true);
    }

    return PhyHandlerCache[phyID];
}

/**
 *  ======== getUpdatedRfCommands ========
 *  Create a new PHY setting and update with data from the instance.
 *  This leaves the original PHY setting unchanged.
 *
 *  @param inst - PHY instance from which update data are fetched
 *  @param phyGroup - BLE, IEEE_15_4 or PROP
 *  @returns Clone of current phy with updated rf commands
 */
function getUpdatedRfCommands(inst, phyGroup) {
    const phyID = Common.getPhyType(inst, phyGroup);

    // Update performed on the duplicate instance
    const phyClone = create(phyGroup, phyID, false);
    phyClone.updateRfCommands(inst);

    return phyClone;
}

/**
 *  ======== create ========
 *  Create a PHY specific instance of the PHY handler, using SmartRF Studio 8 data.
 *
 *  @param phyGroup - ble, prop, ieee_154
 *  @param phyID - short name for the PHY definition
 *  @param first - first time execution
 *  @returns the module's exported functions
 */
function create(phyGroup, phyID, first) {
    const PhyID = phyID;
    const PhyGroup = phyGroup;

    // Get configuration for PHY group
    const DeviceConfig = DeviceInfo.getConfiguration(phyGroup);

    // Load PHY definition file
    const PhyInfo = Common.getPhyInfo(DeviceConfig.phys, phyGroup, phyID);
    const PhyDefFile = PhyInfo.file;
    const PhyDef = loadPhyDef(PhyDefFile);
    const HasFeatures = "feature_group" in PhyDef;
    const IsExternalReleasePhy = PhyDef.release === "external";

    // Get the frequency band (embedded in file name)
    const FreqBand = Common.getFrequencyBand(phyGroup, PhyDefFile);

    // Create command handler
    const cmdHandler = CmdHandler.get(PhyDef, phyGroup, phyID);

    // Populate the PA table
    initPaTable();

    // RF parameter cache (PHY properties)
    const RfParamCache = {};

    // RF properties, indexed by property name
    const RfPropMap = {};

    // Register fields that need dynamic updating
    const UpdatedRegData = {};

    // Functions needed by the common JavaScript utilities
    const exp = {
        getPhyProperty: getPhyProperty
    };

    // Handle to the common JavaScript utilities
    const UtilHandle = Srfs8Util.get(PhyGroup, PhyID, exp);

    // Initialize with values from the PHY definition file
    initRfParamCache();

    // Get RF properties from SmartRF Studio 8 (PHY and Test properties)
    initRfProperties();

    if (first) {
        // Apply defaults to configurables
        initConfigurables(DeviceConfig.configs);
    }

    // Ensure that RF command fields are updated
    updateRfCommands(RfParamCache);

    // Refresh cache
    updateRfParamCache();

    /**
     *  ======== loadPhyDef ========
     *  Load the PHY definition data
     *
     *  @param file - JSON file from which to load the PHY definition
     *  @returns Phy definition loaded from given file
     */
    function loadPhyDef(file) {
        const fileData = system.getScript(ConfigPath + file);
        return _.cloneDeep(fileData.phy_def);
    }

    /**
     *  ======== updateRfParamCache ========
     *  Updates the RF parameter cache with values from RF command fields.
     */
    function updateRfParamCache() {
        const rfData = getRfData();
        for (const key of Object.keys(rfData)) {
            RfParamCache[key] = rfData[key];
        }
    }

    /**
     *  ======== updateRfParamCacheFromInst ========
     *  Updates the RF parameter cache with values from RF command fields.
     *
     *  @param inst - current instance
     */
    function updateRfParamCacheFromInst(inst) {
        for (const key of Object.keys(RfParamCache)) {
            if (key in inst) {
                RfParamCache[key] = inst[key];
            }
        }
        if (phyGroup === Common.PHY_BLE) {
            RfParamCache.bleChannel = Common.bleFreqToChan(inst.frequency);
        }
    }

    /**
     *  ======== updateRfCommands ========
     *  Updates the RF command values based on the content of the UI instance.
     *
     *  @param inst - current instance
     */
    function updateRfCommands(inst) {
        if (!("phy_properties" in PhyDef)) {
            return;
        }
        // Cache must be updated before running the scripts
        updateRfParamCacheFromInst(inst);

        // Update the commands that use scripts
        const commands = cmdHandler.getCommandBuffer();
        // eslint-disable-next-line no-unused-vars
        for (const [_key, command] of Object.entries(commands)) {
            for (const field of command) {
                if (field.script !== null) {
                    const script = field.script;
                    if (script.role === "update") {
                        let code;

                        if ("call" in script) {
                            // Call to function
                            code = "UtilHandle." + script.call;
                        }
                        else {
                            // In-line script
                            code = script["#text"];
                        }
                        // eslint-disable-next-line no-eval
                        field.value = eval(code);
                    }
                }
            }
        }

        /**
         * Get tx power from rf parameter cache
         * @returns txPower from rf parameter cache
         */
        // eslint-disable-next-line no-unused-vars
        function getTxPowerValue() {
            // Invoked from PHY definition
            return RfParamCache.txPower;
        }

        /**
         * Get test value of property with name
         * @param name - name of test property
         * @returns value of test property from rf parameter cache, unless packetcount which is always 100
         */
        // eslint-disable-next-line no-unused-vars
        function getTestProperty(name) {
            // Invoked from PHY definition
            if (name === "packetCount") {
                // Not used by RadioConfig but enforces the same value as SmartRF Studio 8.
                // (for code comparison purposes)
                return 100;
            }
            if (name in RfParamCache) {
                return RfParamCache[name];
            }
            return "";
        }

        /**
         * Get bitmask feature selector
         * @param feature - feature you want to get selector for
         * @returns bitmask, throws error if not found
         */
        // eslint-disable-next-line no-unused-vars
        function getFeatureSelector(feature) {
            // Invoked from PHY definition
            if (feature in RfParamCache) {
                // Look up type in PHY properties
                for (const category of Common.forceArray(PhyDef.phy_properties)) {
                    for (const prop of Common.forceArray(category.property)) {
                        if (prop.name === feature) {
                            const type = prop.type.replace("feature:", "");
                            // Look up feature in the feature group
                            for (const group of Common.forceArray(PhyDef.feature_group)) {
                                if (type === group.name) {
                                    // Find bitmask
                                    const featName = RfParamCache[feature];
                                    for (const feat of group.feature) {
                                        if (featName === feat.name) {
                                            return feat.bitmask;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            throw Error(feature + " not found");
        }
    }

    /**
     *  ======== getFrequency ========
     *  Calculate Frequency in MHz based on raw CMD value
     *  (CMD_FS, frequency and fraction fields)
     *
     *  @returns number as string - frequency
     */
    function getFrequency() {
        return cmdHandler.getCommandField("rfFrequency") / 1e6;
    }

    /**
     *  ======== getWhitening ========
     *  Get Whitening as a descriptive string ("No whitening", "PN9 whitening" ....)
     *
     *  @returns Return value of javascript whitening script
     */
    // eslint-disable-next-line no-unused-vars
    function getWhitening() {
        if (PhyGroup === Common.PHY_PROP) {
            return executePhyPropertyUpdate("whitening");
        }
        throw Error("getWhitening() not applicable to BLE and IEEE 802.15.4-2006");
    }

    /**
     *  ======== getSyncWord ========
     *  Get RX sync word
     *  @returns syncWord command field
     */
    function getSyncWord() {
        return cmdHandler.getCommandField("syncWord");
    }

    /**
     *  ======== createSegments ========
     *  Create feature combinations (segments) for use with code export. This database is
     *  initially empty. The structure is determined by the 'feature_group' node in the PHY definition.
     *
     *  @returns the created segments
     */
    function createSegments() {
        const segments = {};

        const groups = HasFeatures ? PhyDef.feature_group : PhyDef.group;
        for (const group of Common.forceArray(groups)) {
            const allFeatures = [];
            const groupSegments = []; // Segments in this group
            let nCombos;
            let groupBitmap = 0; // One bit per feature in the group
            let featureBitmask = 0; // Accumulated bitmask for all features

            if (HasFeatures) {
                // Collect all features in the group
                let i = 0;
                for (const feat of group.feature) {
                    allFeatures.push(feat.name);
                    groupBitmap |= 1 << i;
                    featureBitmask |= Number(feat.bitmask);
                    i += 1;
                }
                nCombos = (1 << allFeatures.length);
            }
            else {
                // One combined segment
                nCombos = 1;
            }

            // Create the feature combinations (segments) - initially empty
            for (let i = 0; i < nCombos; i++) {
                const segment = {
                    feat: [], register: {}, regions: [], rcl_struct: []
                };

                // Add features to segment
                let featureSelector = 0;
                let usedBitmap = 0;
                let invert = false;
                const label = [];

                // Create combination content
                for (let j = 0; j < i; j++) {
                    if ((1 << j) & i) {
                        const groupFeature = group.feature[j];
                        // First item in the group must be inverted
                        if (j === 0) {
                            invert = true;
                        }
                        featureSelector |= Number(groupFeature.bitmask);
                        usedBitmap |= 1 << j;
                        segment.feat.push(allFeatures[j]);
                        label.push(groupFeature.label);
                    }
                }
                // Create segment header
                if (HasFeatures) {
                    segment.header = 0;
                    if (usedBitmap === groupBitmap) {
                        // All features selected
                        invert = false;
                        featureSelector = 0;
                    }
                    if (invert) {
                        // Inverted
                        featureSelector = (~featureSelector) & featureBitmask;
                        segment.header |= 0x4000;
                    }
                    segment.header |= featureSelector << 16;
                }
                else {
                    segment.header = 0;
                }

                // Add segment name and label
                if (segment.header === 0) {
                    segment.name = "common";
                }
                else {
                    segment.name = segment.feat.join();
                }
                segment.label = label.toString();

                // Add to group segments
                groupSegments[i] = segment;
            }
            // Add to group segments to global segments
            if (HasFeatures) {
                segments[group.name] = groupSegments;
            }
            else {
                segments.Combined = groupSegments;
            }
        }
        return segments;
    }

    /**
     *  ======== populateSegments ========
     *  Create a register database for use by the code generation
     *
     *  @param segments  - Feature segments to be populated
     */
    function populateSegments(segments) {
        // Register data storage, indexed by feature
        const registerStore = {};

        // Make sure dynamic register data are up to date
        updateRegisterData();

        if (AddressMapCreated === false) {
            createAddressMap();
            AddressMapCreated = true;
        }

        // Iterate settings (feature groups), populating the feature segments
        for (const regItem of Common.forceArray(PhyDef.rcl_registers)) {
            // Get the segments of the feature group
            const group = HasFeatures ? regItem.feature_group : regItem.group;
            const fgSegments = segments[group];

            // Create register data; bit fields merged to words
            createRegisterData(registerStore, regItem);

            // Store register values; indexed by feature, register name
            const featureData = createFeatureData(registerStore, regItem);

            // Create initial regions (16-bit regions only)
            createRawRegions(fgSegments, featureData);

            // Refactor regions: zero and sparse regions created, 16-bit regions converted to 32-bit where possible
            refactorRegions(fgSegments);
        }
    }

    /**
     *  ======== createRegisterData ========
     *  Create a lookup object for registers, two-levels deep.
     *  The top level is indexed by feature name, the next level down
     *  is indexed by register name (module.register).
     *
     *  The values of the bit-fields are merged. The register name
     *  indexes an object that holds the address and the value.
     *
     *  @param featureStore  - Storage for registers that belong to the given feature.
     *                         Populated in this function.
     *  @param regItem - Register item from the PHY definition file.
     */
    function createRegisterData(featureStore, regItem) {
        /**
         * Get register from "common" part of storage
         * @param regName - The desired register
         * @returns The given register, null if not
         */
        function getRegFromCommon(regName) {
            if ("common" in featureStore) {
                if (regName in featureStore.common.regs) {
                    return featureStore.common.regs[regName];
                }
            }
            return null;
        }

        for (const feature of regItem.rcl_register_field_settings) {
            const featureName = feature.feature_filter;
            if (!(featureName in featureStore)) {
                featureStore[featureName] = {regs: {}, regions: []};
            }

            const featRegs = featureStore[featureName].regs;

            // Iterate registers in the module
            for (const mod of feature.data) {
                // Iterate register fields in the module
                for (const [fieldPath, featureData] of Object.entries(mod.reg)) {
                    if (featureData.value === "") {
                        // eslint-disable-next-line no-continue
                        continue;
                    }
                    const fp = fieldPath.toUpperCase();
                    if (fp in UpdatedRegData) {
                        featureData.value = UpdatedRegData[fp];
                    }

                    const [modName, regName, fieldName] = fp.split(".");
                    const regPath = modName + "." + regName;

                    // Look up register and bit-field info
                    const ri = DeviceInfo.getRclRegisters()[regPath];
                    const fi = ri[fieldName];
                    const pos = Number(fi.pos);
                    const mask = (~(0xFFFF << fi.width) & 0xFFFF) << pos;
                    const val = Number(featureData.value) << pos;

                    const addr = ri.addr;
                    if (!(regPath in featRegs)) {
                        featRegs[regPath] = {
                            value: 0,
                            addr: addr,
                            trim: false,
                            mask: 0
                        };
                        // Check if the bit-field exists in common segment
                        const cmReg = getRegFromCommon(regPath);
                        if (cmReg !== null) {
                            featRegs[regPath].value = cmReg.value & ~mask;
                        }
                    }
                    // Feature value overrides common value
                    const reg = featRegs[regPath];
                    reg.value &= ~mask;
                    reg.value |= val;

                    // Check if trim type, used for masked region
                    if (fi.type === "trim") {
                        reg.trim = true;
                        reg.mask = ~mask;
                    }
                }
            }
        }
    }

    /**
     *  ======== createAddressMap ========
     *  Create a Address map object for registers, indexed by module name. The indexed
     *  data contains an array of register addresses used by the module.
     */
    function createAddressMap() {
        for (const item of DeviceInfo.getRclRegLookupAddr()) {
            const [name, reg] = item;
            // eslint-disable-next-line no-unused-vars
            const [module, regName] = name.split(".");
            const addr = Number(reg.addr);

            if (!(module in AddressMap)) {
                AddressMap[module] = [];
            }
            AddressMap[module].push(addr);
        }
    }

    /**
     *  ======== createFeatureData ========
     *  Create a lookup object for features, indexed by register name. The indexed
     *  data contains the register address, and an object of values, one value for
     *  each feature.
     *
     *  @param featureStore  - Storage for the registers belonging to the given feature.
     *  @param regItem - Register item from the PHY definition file.
     *
     *  @returns - Object with collected register data
     */
    function createFeatureData(featureStore, regItem) {
        const featureData = {register: {}, struct: {}};

        // Add registers
        for (const data of regItem.rcl_register_field_settings) {
            const feature = data.feature_filter;
            const featRegs = featureStore[feature].regs;

            for (const [regPath, featData] of Object.entries(featRegs)) {
                const featureRegs = featureData.register;
                if (!(regPath in featureRegs)) {
                    featureRegs[regPath] = {
                        class: "register",
                        addr: featData.addr,
                        trim: featData.trim,
                        mask: featData.mask,
                        value: {},
                        isCommon: feature === "common"
                    };
                }
                featureRegs[regPath].value[feature] = featData.value;
            }
        }
        // Add software parameter structures if applicable
        if ("rcl_struct" in regItem) {
            const featureStructs = featureData.struct;
            for (const data of Common.forceArray(regItem.rcl_struct)) {
                let features;
                if ("feature_filter" in data) {
                    features = data.feature_filter.split(",");
                }
                else {
                    features = ["Combined"];
                }

                for (const feature of features) {
                    for (const field of data.rcl_struct_field) {
                        const name = field.name;
                        if (!(name in featureStructs)) {
                            // eslint-disable-next-line max-len
                            featureStructs[name] = {class: "struct", value: {}, isCommon: feature === "common" || feature === "Combined"};
                        }
                        featureStructs[name].value[feature] = field.value;
                    }
                }
            }
        }
        return featureData;
    }

    /**
     *  ======== createRawRegions ========
     *  Create initial regions for all feature combinations (segments). The only regions
     *  that are created at this stage are the 16-bit regions. The regions will be
     *  re-shuffled later.
     *
     *  @param segments  - Object containing feature combinations (segments).
     *  @param featureData - Object containing feature data, index by register name
     */
    function createRawRegions(segments, featureData) {
        const nFeatures = Math.log2(segments.length);
        let structPos = 0;
        const entries = {...featureData.register, ...featureData.struct};
        const commonSegment = segments[segments.length - 1];

        for (const [regPath, featData] of Object.entries(entries)) {
            const addr = featData.addr;
            const trim = featData.trim;
            const mask = featData.mask;

            // eslint-disable-next-line no-unused-vars
            const [modName, regName, _] = regPath.split(".");

            // Create array indexed by values; content is an array of features
            const valLookup = {};
            let valCommon = null;
            const dataValues = Object.entries(featData.value);
            const addedFeat = [];
            for (const [feat, val] of dataValues) {
                if (feat === "common") {
                    valCommon = val;
                }
                else {
                    if (!(val in valLookup)) {
                        valLookup[val] = [];
                    }
                    valLookup[val].push(feat);
                    addedFeat.push(feat);
                }
            }

            // Add to register lookup table
            const valList = Object.keys(valLookup);
            const nValues = valList.length;

            // Determine if the register has the same value for all features
            let singleValue;
            if (featData.isCommon) {
                // If feature values exist they take precedence
                singleValue = true;
                if (nValues > 0) {
                    // Feature values exists in addition to "common"
                    singleValue = false;
                    // Distribute common value across other features
                    const feats = commonSegment.feat;
                    for (const feat of feats) {
                        if (!(addedFeat.includes(feat))) {
                            if (!(valCommon in valLookup)) {
                                valLookup[valCommon] = [];
                            }
                            valLookup[valCommon].push(feat);
                        }
                    }
                    singleValue = nValues === 1 && dataValues.length >= nFeatures;
                    if (singleValue) {
                        valCommon = valList[0];
                    }
                }
            }
            else {
                // All features have the same value?
                singleValue = nValues === 1 && dataValues.length >= nFeatures;
                valCommon = valList[0];
            }

            if (singleValue) {
                // Only one value, so use it with all features
                // const segment = segments[segments.length - 1];
                const value = valCommon;
                if (featData.class === "register") {
                    const reg = {
                        addr: addr,
                        value: value,
                        trim: trim,
                        mask: mask
                    };
                    commonSegment.register[regPath] = reg;
                    addRegisterToRegion(commonSegment.regions, reg, modName, regName);
                }
                else {
                    // Data struct region
                    const data = {
                        name: regPath,
                        value: value,
                        bytePos: structPos
                    };
                    commonSegment.rcl_struct.push(data);
                }
            }
            else {
                // Find which features that share this value
                for (const [value, feat] of Object.entries(valLookup)) {
                    // Establish which feature combination to use
                    for (let i = segments.length - 2; i >= 0; i--) {
                        const segment = segments[i];
                        const featName = feat.join();
                        if (segment.name === featName) {
                            if (featData.class === "register") {
                                const reg = {
                                    addr: addr,
                                    value: value,
                                    trim: trim,
                                    mask: mask
                                };
                                segment.register[regPath] = reg;
                                addRegisterToRegion(segment.regions, reg, modName, regName);
                            }
                            else {
                                // Data struct region
                                const data = {
                                    name: regPath,
                                    value: value,
                                    bytePos: structPos
                                };
                                segment.rcl_struct.push(data);
                            }
                            break;
                        }
                    }
                }
            }
            if (featData.class === "struct") {
                structPos += 4;
            }
        }
    }

    /**
     *  ======== refactorRegions ========
     *  Dual purpose:
     *
     *  1. Merging all single element 16-bit regions into sparse regions.
     *  2. Create zero-regions where possible (2 or more contiguous registers which values are zero).
     *  3. Create 32-bit regions from 16-bit regions where possible.
     *
     *  @param segments  - Object containing feature combinations (segments).
     *
     */
    function refactorRegions(segments) {
        // Create zero or sparse regions
        for (const segment of segments) {
            const sparseRegionHW = {addr: 0, count: 0, regs: []};
            const maskedRegionHW = {addr: 0, count: 0, regs: []};
            const sparseRegionRAM = {addr: 0, count: 0, regs: []};

            // Calculated zero regions
            const zeroRegions = [];

            // Raw regions for deletion after refactoring
            const toDelete = [];

            let i = 0;
            for (const region of segment.regions) {
                // Process sparse/masked regions
                if (region.regNames.length === 1) {
                    // Add to sparse region
                    addSparseRegion(region, sparseRegionRAM, sparseRegionHW, maskedRegionHW, segment);

                    // Mark the item in original 16-bit region for deletion
                    toDelete.push(i);
                }
                else {
                    // Check if region is all zero (or contains 4+ adjacent zeros)
                    let sum = 0;
                    let wc = 0; // Word counter
                    const regionInfo = {
                        lo: Number.MAX_VALUE,
                        hi: Number.MAX_VALUE,
                        hasZeroRegn: false,
                        regs: []
                    };
                    const MIN_ZERO = 3;
                    let counting = true;

                    for (const regName of region.regNames) {
                        let val = Number(segment.register[regName].value);
                        const addr = segment.register[regName].addr;
                        if (regName !== GAP) {
                            sum += val;
                        }
                        else {
                            val = 0;
                        }

                        if (val === 0) {
                            if (counting) {
                                if (regionInfo.lo === Number.MAX_VALUE) {
                                    regionInfo.lo = wc;
                                }
                                if (wc - regionInfo.lo >= (MIN_ZERO - 1)) {
                                    regionInfo.hasZeroRegn = true;
                                }
                                regionInfo.hi = wc;
                            }
                        }
                        else if (regionInfo.hasZeroRegn) {
                            // Above limit: Lock
                            counting = false;
                        }
                        else if ((regionInfo.hi - regionInfo.lo) < MIN_ZERO) {
                            // Below limit: Invalidate
                            regionInfo.lo = Number.MAX_VALUE;
                            regionInfo.hi = Number.MAX_VALUE;
                        }

                        const reg = {name: regName, addr: addr, val: val};
                        regionInfo.regs.push(reg);
                        wc += 1;
                    }

                    if (sum === 0) {
                        // Convert entire region to zero region
                        if (region.type === RegisterRegion.T_HW_16BIT) {
                            region.type = RegisterRegion.T_HW_ZERO;
                        }
                        else {
                            region.type = RegisterRegion.T_RAM_ZERO;
                        }
                    }
                    else if (regionInfo.hasZeroRegn) {
                        // Create new zero region
                        const idxLo = regionInfo.lo;
                        const idxHi = regionInfo.hi;

                        const zerosFromStart = idxLo === 0;
                        const zerosFromEnd = idxLo > 0 && (idxHi + 1) === regionInfo.regs.length;

                        const addr = regionInfo.regs[idxLo].addr;
                        const zeroRegn = {
                            addr: addr,
                            count: idxHi - idxLo + 1
                        };
                        if (region.type === RegisterRegion.T_HW_16BIT) {
                            zeroRegn.type = RegisterRegion.T_HW_ZERO;
                        }
                        else {
                            zeroRegn.type = RegisterRegion.T_RAM_ZERO;
                        }
                        const step = region.type === RegisterRegion.T_HW_16BIT ? 4 : 2;

                        // First address in the region
                        const firstAddr = Number(region.addr);
                        const isAligned = is32BitAligned(firstAddr, step);

                        if (region.addr !== 0xffff && isAligned) {
                            zeroRegions.push(zeroRegn);

                            if (zerosFromStart || zerosFromEnd) {
                                // Remove zero's from original region
                                region.count -= zeroRegn.count;
                                if (zerosFromEnd) {
                                    // From the end
                                    region.regNames.splice(region.count, zeroRegn.count);
                                }
                                else if (zerosFromStart) {
                                    // From the start
                                    region.regNames.splice(0, zeroRegn.count);
                                    region.addr = regionInfo.regs[idxHi + 1].addr;
                                }
                            }
                            else {
                                let existingRegnUsed = false;
                                if (idxLo > 1) {
                                    // Keep existing region, cropped from end
                                    region.count = idxLo;
                                    region.regNames.splice(region.count, zeroRegn.count - idxLo);
                                    existingRegnUsed = true;
                                }
                                else {
                                    // Add to sparse region
                                    addSparseRegion(region, sparseRegionRAM, sparseRegionHW, maskedRegionHW, segment);

                                    // Remove the first item from the raw 16-bit region
                                    region.regNames.shift();
                                    region.addr = regionInfo.regs[idxLo].addr;
                                    region.count -= 1;
                                }
                                const nonZeroAtEnd = regionInfo.regs.length - idxHi - 1;
                                if (nonZeroAtEnd > 1) {
                                    if (existingRegnUsed) {
                                        // Create new 16-bit region
                                        const regn = {
                                            addr: regionInfo.regs[idxHi + 1].addr,
                                            type: region.type,
                                            count: idxHi - idxLo + 1,
                                            regNames: region.regNames.splice(idxLo + 1, region.regNames.length)
                                        };
                                        segment.regions.push(regn);
                                    }
                                    else {
                                        // Keep existing region, cropped from start
                                        region.regNames.splice(0, idxHi);
                                        region.addr = regionInfo.regs[idxHi + 1].addr;
                                        region.count = nonZeroAtEnd;
                                    }
                                }
                                else {
                                    // Add to sparse region
                                    addSparseRegion(region, sparseRegionRAM, sparseRegionHW, maskedRegionHW, segment);

                                    // Remove the item from the raw 16-bit region
                                    region.regNames.pop();
                                    region.addr = regionInfo.regs[0].addr;
                                    region.count -= 1;
                                }
                            }
                        }
                    }
                }
                i += 1;
            }
            // Delete regions that have been added to the sparse, masked or zero regions
            for (const index of toDelete.reverse()) {
                segment.regions.splice(index, 1);
            }
            // Add the sparse regions
            sparseRegionHW.regs.sort((a, b) => (Number(a.addr) > Number(b.addr) ? 1 : -1));
            segment.regions.push(sparseRegionHW);

            sparseRegionRAM.regs.sort((a, b) => (Number(a.addr) > Number(b.addr) ? 1 : -1));
            segment.regions.push(sparseRegionRAM);

            // Add the masked region
            maskedRegionHW.regs.sort((a, b) => (Number(a.addr) > Number(b.addr) ? 1 : -1));
            segment.regions.push(maskedRegionHW);

            // Add the new zero regions
            for (const regn of zeroRegions) {
                segment.regions.push(regn);
            }

            // Merge adjacent 16-bit regions (across 1-register gap)
            merge16BitRegions(segment);

            // Convert to 32-bit regions where possible
            for (const region of segment.regions) {
                // Don't process regions that have been marked for delete
                if (region.count === 0) {
                    // eslint-disable-next-line no-continue
                    continue;
                }
                let canConvert = canConvTo32bitRegion(region);

                // Conversion criteria satisfied?
                if (canConvert) {
                    if (region.type === RegisterRegion.T_HW_16BIT) {
                        // eslint-disable-next-line no-unused-vars
                        const [modName, _unsed] = region.regNames[0].split(".");
                        const moduleDef = DeviceInfo.getModuleDefinition()[modName];
                        if ("alias_region" in moduleDef) {
                            const aliasAddr = ((region.addr - Number(moduleDef.normal_region.address)) / 2)
                                + Number(moduleDef.alias_region.address);
                            if (is32BitAligned(aliasAddr, 2)) {
                                region.alias_addr = aliasAddr;
                                region.type = RegisterRegion.T_HW_32BIT;
                            }
                            else {
                                canConvert = false;
                            }
                        }
                        else {
                            canConvert = false;
                        }
                    }
                    else if (region.type === RegisterRegion.T_RAM_16BIT) {
                        region.type = RegisterRegion.T_RAM_32BIT;
                    }
                    if (canConvert) {
                        make32BitAligned(region);
                    }
                }
            }
        }
    }

    /**
     *  ======== addSparseRegion ========
     *  Create Sparse Region if not existing or add single register to an existing region
     *
     *  @param region          - The region to check
     *  @param sparseRegionRAM - RAM sparse region
     *  @param sparseRegionHW  - HW sparse region
     *  @param maskedRegionHW  - HW masked region
     *  @param segment         - Register segment
     */
    function addSparseRegion(region, sparseRegionRAM, sparseRegionHW, maskedRegionHW, segment) {
        let sparseRegion;
        let regionType;
        const regName = region.regNames[0];

        // Determine region type (still 16-bit only)
        if (region.type === RegisterRegion.T_RAM_16BIT) {
            sparseRegion = sparseRegionRAM;
            regionType = RegisterRegion.T_RAM_SPARSE;
        }
        else if (region.trim && segment.header !== 0) {
            // Masked region
            sparseRegion = maskedRegionHW;
            regionType = RegisterRegion.T_HW_MASKED;
        }
        else {
            sparseRegion = sparseRegionHW;
            regionType = RegisterRegion.T_HW_SPARSE;
        }

        // Create new sparse region if it doesn't exist
        if (sparseRegion.count === 0) {
            sparseRegion.type = regionType;
            sparseRegion.addr = region.addr;
        }

        // Add register to region
        const reg = {name: regName, addr: region.addr};
        sparseRegion.regs.push(reg);
        sparseRegion.count += 1;

        if (region.addr < sparseRegion.addr) {
            // For sorting during regions code export
            // Ensure that the region address is the lowest found
            sparseRegion.addr = region.addr;
        }
    }

    /**
     *  ======== canConvTo32bitRegion ========
     *  Return true if a region can be converted to 32 bit.
     *
     *  @param region - the region to check
     *  @returns boolean
     */
    function canConvTo32bitRegion(region) {
        if (region.type === RegisterRegion.T_HW_16BIT || region.type === RegisterRegion.T_RAM_16BIT) {
            // 32-bit step
            const step = region.type === RegisterRegion.T_HW_16BIT ? 4 : 2;

            // First address in the region
            const firstAddr = Number(region.addr);
            // Split the register name in <module>.<register> for the first item in the list
            const modName = region.regNames[0].split(".")[0];
            const hasGapBefore = isAddressGap(modName, firstAddr - step);
            const isFirstAligned = is32BitAligned(firstAddr, step);

            // Next address after the region
            const nextAddr = Number(region.addr) + (region.count * step);
            const hasGapAfter = isAddressGap(modName, nextAddr);
            const isNextAligned = is32BitAligned(nextAddr, step);

            // Cannot convert if the first address is unaligned and there is no gap before it
            if (!isFirstAligned && !hasGapBefore) {
                return false;
            }
            // Cannot convert if the last address is unaligned and there is no gap after it
            if (!isNextAligned && !hasGapAfter) {
                return false;
            }
            // When utilizing gaps at start and end, we do not accept the size increase for less than 6 registers
            if (!isFirstAligned && !isNextAligned && region.count < 6) {
                return false;
            }
            return true;
        }
        return false;
    }

    /**
     * Utilizes already identified gaps to make the region 32-bit aligned, if needed
     *
     * This function should only be called for regions that have been converted to 32-bit. It has no effect
     * for other region types.
     *
     *  @param region - the region to align
     */
    function make32BitAligned(region) {
        const step = region.type === RegisterRegion.T_HW_32BIT ? 4 : 2;
        const firstAddr = Number(region.addr);
        const lastAddr = Number(region.addr) + (region.count * step);

        if (!is32BitAligned(firstAddr, step)) {
            region.addr = firstAddr - step;
            region.regNames.unshift(GAP);
            region.count += 1;
        }
        if (!is32BitAligned(lastAddr, step)) {
            region.regNames.push(GAP);
            region.count += 1;
        }
        region.count /= 2;
    }

    /**
     * Checks if address is 32 bit aligned
     * @param addr  - First address
     * @param step  - Number of steps, which depends on address type
     * @returns boolean
     */
    function is32BitAligned(addr, step) {
        return (addr % (2 * step)) === 0;
    }

    /**
     *  ======== isAddressGap ========
     *  Check if given address is used or not.
     *  If not, the address can be seen as a "gap"
     *
     *  @param modName - The module name
     *  @param addr    - The address to be checked
     *  @returns True if address is a gap, false if not
     */
    function isAddressGap(modName, addr) {
        if (modName in AddressMap) {
            for (let i = 0; i < AddressMap[modName].length; i++) {
                if (AddressMap[modName][i] === addr) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Get lapel of given group
     *
     * @param groupID name of given group
     * @returns label of group matching name, otherwise stated groupID
     */
    function getFeatureGroupLabel(groupID) {
        if ("feature_group" in PhyDef) {
            const groups = HasFeatures ? PhyDef.feature_group : PhyDef.group;
            for (const group of Common.forceArray(groups)) {
                if (groupID === group.name) {
                    return group.label;
                }
            }
        }
        return groupID;
    }

    /**
     *  ======== merge16BitRegions ========
     *  Merged 16-bit regions if the gap between them is one register.
     *
     *  @param segment - The segment to operate on
     */
    function merge16BitRegions(segment) {
        let currRegion = null;
        let nextRegion = null;

        // Iterate through all regions of given segment
        for (let i = 0; i < segment.regions.length - 1; i++) {
            // Check current region and next region
            currRegion = segment.regions[i];
            nextRegion = segment.regions[i + 1];

            // Check if region is valid and if they can be merged accross gap
            if ((currRegion.count > 0) && (nextRegion.count > 0)) {
                if (canMergeAcrossGap(currRegion, nextRegion, 2)) {
                    // Prepend current region to next region
                    prependCurrRegionToNext(currRegion, nextRegion, 2);
                }
            }
        }
    }

    /**
     *  ======== canMergeAcrossGap ========
     *  Checks whether two regions of same type can be merged across a gap in the address space
     *
     *  @param currRegion    - Current Region
     *  @param nextRegion    - Next Region
     *  @param addrIncrement - Address increment per register
     *  @returns If possible to merge across gap
     */
    function canMergeAcrossGap(currRegion, nextRegion, addrIncrement) {
        // Cannot merge if the regions have different type ...
        if (currRegion.type !== nextRegion.type) {
            return false;
        }

        const nextFirstAddr = Number(nextRegion.addr);
        const currLastAddr = Number(currRegion.addr) + ((currRegion.count - 1) * addrIncrement);

        // Nothing to be gained if the spacing is more than 2 registers
        const spacing = (nextFirstAddr - (currLastAddr + addrIncrement)) / addrIncrement;
        if (spacing > 2) {
            return false;
        }
        // Nothing to be gained if there is spacing and one of the regions contain less than 2 registers
        if ((spacing > 0) && ((currRegion.count < 2) || (nextRegion.count < 2))) {
            return false;
        }

        // Cannot merge if the spacing contains any registers
        for (let n = 0; n < spacing; n++) {
            const addr = currLastAddr + ((1 + n) * addrIncrement);
            const modName = currRegion.regNames[currRegion.count - 1].split(".")[0];
            if (!isAddressGap(modName, addr)) {
                return false;
            }
        }
        // Can merge
        return true;
    }

    /**
     *  ======== prependCurrRegionToNext ========
     *  Prepend current region to the next region
     *
     *  @param currRegion    - Current Region
     *  @param nextRegion    - Next Region
     *  @param addrIncrement - Address increment per register
     */
    function prependCurrRegionToNext(currRegion, nextRegion, addrIncrement) {
        const nextFirstAddr = Number(nextRegion.addr);
        const currLastAddr = Number(currRegion.addr) + ((currRegion.count - 1) * addrIncrement);

        // Fill any spacing between the regions
        let spacing = (nextFirstAddr - (currLastAddr + addrIncrement)) / addrIncrement;
        if (spacing > 0) {
            // eslint-disable-next-line no-plusplus
            while (spacing--) {
                nextRegion.regNames.unshift(GAP);
                nextRegion.addr = int2hex(nextFirstAddr - addrIncrement);
                nextRegion.count++;
            }

            // Prepend the current region
            for (let n = (currRegion.count - 1); n >= 0; n--) {
                nextRegion.regNames.unshift(currRegion.regNames[n]);
                nextRegion.addr -= addrIncrement;
                nextRegion.count++;
            }

            // Invalidate current region
            currRegion.count = 0;
        }
    }

    /**
     *  ======== addRegisterToRegion ========
     *  Add a register to a region. Depending on the address it may be appended or prepended
     *  to the region. If the address is in a gap between two regions these will be merged.
     *  At this stage only 16-bit regions are created, refactoring takes place later.
     *
     *  @param regions - Region object
     *  @param reg - Address/value
     *  @param modName - Module name
     *  @param regName - Register name
     */
    function addRegisterToRegion(regions, reg, modName, regName) {
        // Try to add to region
        const addr = reg.addr;
        const regPath = modName + "." + regName;
        const registerAddr = Number(addr);

        const modInfo = DeviceInfo.getModuleDefinition()[modName];
        const modType = modInfo.type;

        // Determine region type for 16 bit (32-bit, zero, and sparse regions to be calculated later)
        let step;
        let regionType;
        let trim = false;

        if (modType === "rcl_hw") {
            if (reg.trim) {
                trim = true;
            }
            regionType = RegisterRegion.T_HW_16BIT;
            step = 4;
        }
        else {
            regionType = RegisterRegion.T_RAM_16BIT;
            step = 2;
        }

        let regionUp = null;
        let regionDown = null;

        // Try to add entry to a region
        for (const region of regions) {
            if ("type" in region) {
                if (regionType.id !== region.type.id) {
                    // eslint-disable-next-line no-continue
                    continue;
                }
            }
            else {
                throw Error("Invalid region");
            }
            const regionAddr = Number(region.addr);
            const addrNextUp = regionAddr + (step * region.count);
            const addrNextDown = regionAddr - step;

            // Continuous upwards ?
            if (registerAddr === addrNextUp) {
                region.count += 1;
                regionDown = region;
                region.regNames.push(regPath);
            }
            // Continuous downwards ?
            else if (registerAddr === addrNextDown) {
                region.addr = addr;
                region.count += 1;
                regionUp = region;
                region.regNames.unshift(regPath);
            }
        }

        if (regionUp === null && regionDown === null) {
            // Create new region
            regions.push({
                addr: addr,
                count: 1,
                type: regionType,
                regNames: [regPath],
                trim: trim
            });
        }
        else if (regionUp && regionDown) {
            // Ready for merge: Use upper region as new, delete lower region
            regionUp.addr = regionDown.addr;
            regionUp.count += regionDown.count - 1;

            // Remove the overlapping element
            for (let i = regionUp.count - 1; i >= 0; i--) {
                if (regPath === regionUp.regNames[i]) {
                    regionUp.regNames.splice(i, 1);
                }
            }

            // Merge register names
            regionUp.regNames = regionDown.regNames.concat(regionUp.regNames);

            // Invalidate lower region
            regionDown.addr = 0xFFFF;
            regionDown.count = 0;
        } // else: already processed
    }

    /**
     *  ======== updateRegisterData ========
     *  Update registers that are found the PHY definition. They may be dynamically
     *  changed by the application or contain a fixed value.
     */
    function updateRegisterData() {
        // Ensure that the cache is updated with values from RF commands (if applicable)
        updateRfParamCache();

        // Update register fields
        for (const regItem of Common.forceArray(PhyDef.rcl_registers)) {
            if ("rcl_register_field" in regItem) {
                const regFields = Common.forceArray(regItem.rcl_register_field);
                for (const data of regFields) {
                    if ("script" in data) {
                        const val = eval(data.script["#text"]);
                        UpdatedRegData[data.name] = val;
                    }
                    else if ("value" in data) {
                        UpdatedRegData[data.name] = data.value;
                    }
                }
            }
        }
    }

    /**
     *  ======== initRfProperties ========
     *  Initialize the RF properties array
     */
    function initRfProperties() {
        if (!("phy_properties" in PhyDef)) {
            return;
        }

        // Join all RF properties
        let data = Common.forceArray(PhyDef.phy_properties);

        if ("test_properties" in PhyDef) {
            data = data.concat(PhyDef.test_properties);
        }
        // Create RF property map
        for (const group of data) {
            const properties = Common.forceArray(group.property);
            for (const prop of properties) {
                RfPropMap[prop.name] = prop;
            }
        }
    }

    /**
     *  ======== initRfParamCache ========
     *  Initialize the RF parameters cache with values
     *  from the PHY definition file (PHY properties)
     */
    function initRfParamCache() {
        if (!("phy_properties" in PhyDef)) {
            return;
        }
        // Get properties from SmartRF Studio 8
        let allProps = Common.forceArray(PhyDef.phy_properties);

        if ("test_properties" in PhyDef) {
            allProps = allProps.concat(PhyDef.test_properties);
        }

        for (const prop of allProps) {
            const properties = Common.forceArray(prop.property);

            for (const property of properties) {
                const name = property.name;
                const type = property.type;
                const value = property.value;
                let fmtValue = value;

                if (type === "dec") {
                    const opts = Common.getOptions(DeviceConfig.configs, name);
                    fmtValue = Number(value);

                    if (opts !== null) {
                        // Use drop-down list
                        _.each(opts, (opt) => {
                            if (opt.key === fmtValue) {
                                fmtValue = opt.name;
                            }
                        });
                    }
                }
                else if (type === "float") {
                    fmtValue = parseFloat(value);
                }
                else if (type === "hex") {
                    fmtValue = Number(value);
                }

                if (!(name in RfParamCache)) {
                    RfParamCache[name] = fmtValue;
                    // Make sure that frequency is correctly set
                    if (name === "bleChannel") {
                        RfParamCache.frequency = UtilHandle.convBleChannelToFreq(fmtValue);
                    }
                }
            }
        }
    }

    /**
     *  ======== initConfigurables ========
     *  Initialize the configurables with default values and options
     *
     *  @param config  - array of configurables
     */
    function initConfigurables(config) {
        /**
         * Initialize configurable item
         * @param item - Given item
         */
        function initConfigItem(item) {
            switch (item.name) {
            case "txPower":
                item.options = (inst) => RfDesign.getTxPowerOptions(FreqBand, false);
                break;
            default:
                if (item.name in RfParamCache) {
                    item.default = RfParamCache[item.name];
                }
                break;
            }
        }

        // Initialize configurables
        for (const item of config) {
            if ("config" in item) {
                for (const subItem of item.config) {
                    initConfigItem(subItem);
                }
            }
            else {
                initConfigItem(item);
            }
        }
    }

    /**
     *  ======== getPhyProperty ========
     *  Support function for SmartRF Studio 8 JavaScript functions
     *
     *  @param name - Name of PHY property (SRFS8)
     *  @returns Property from parameter cache
     */
    function getPhyProperty(name) {
        if (name in RfParamCache) {
            const opts = Common.getOptions(DeviceConfig.configs, name);
            if (opts !== null) {
                // Use drop-down list
                for (const opt of opts) {
                    if (opt.name === RfParamCache[name]) {
                        return opt.key;
                    }
                }
            }
            return RfParamCache[name];
        }
        throw Error("Unknown PHY property: " + name);
    }

    /**
     *  ======== getRfData ========
     *  Get initial settings
     *  @returns The initial settings
     */
    function getRfData() {
        const freq = getFrequency();

        if (PhyGroup === Common.PHY_PROP) {
            return {
                frequency: parseFloat(freq),
                syncWord: parseInt(getSyncWord()),
                txPower: 0,
                subPhy: "1_mbps"
            };
        }
        else if (PhyGroup === Common.PHY_BLE) {
            RfParamCache.bleChannel = Common.bleFreqToChan(freq);
            return {
                // Placeholder values
                frequency: freq,
                txPower: 0,
                subPhy: "1_mbps",
                codedTxRate: "s8"
            };
        }
        return {};
    }

    /**
     *  ======== executePhyPropertyUpdate ========
     *  Execute JavaScript code to update an PHY property.
     *
     *  @param rfProp - Name of PHY property (SRFS8)
     *
     *  @returns Value of the executed script
     */
    function executePhyPropertyUpdate(rfProp) {
        const prop = getPhyPropertyByName(rfProp);
        if (prop !== null && "script" in prop) {
            const scripts = Common.forceArray(prop.script);
            const updScript = scripts[0];

            if (updScript.role.match(/update/)) {
                let code;

                if ("call" in updScript) {
                    // Call to function
                    code = "UtilHandle." + updScript.call;
                }
                else {
                    // In-line script
                    code = updScript["#text"];
                }
                // eslint-disable-next-line no-eval
                let val = eval(code);
                if (prop.type === "float") {
                    val = parseFloat(val).toFixed(prop.decimals);
                }
                else if ("enum" in prop) {
                    // eslint-disable-next-line no-undef
                    val = getOptByKey(rfProp, val);
                }
                return val;
            }
        }
        throw Error("PHY property update failed: " + rfProp);
    }

    /**
     *  ======== is154g ========
     *  True if the setting supports IEEE 802.15.4g (proprietary advanced RX/TX commands)
     *  @returns boolean
     */
    function is154g() {
        return false; // TBD: reintroduce when 15.4g support is added
    }

    /**
     *  ======== generateParameterSummary ========
     *  Generate a parameter summary as C++ comments
     *
     *  @param inst - The current instance
     *  @returns The generated parameter summary
     */
    function generateParameterSummary(inst) {
        // Create list of configurables
        const configs = Common.flattenConfigs(inst.$module.config);
        const ui = inst.$uiState;

        // Generate summary
        let summary = "";
        for (const cfg of configs) {
            const id = cfg.name;
            // Parameter summary skipped for hidden configurables
            if (ui[id].hidden || id.includes("phyType")) {
                // eslint-disable-next-line no-continue
                continue;
            }

            // Append generated line
            let value = inst[id];
            if ("displayFormat" in cfg && cfg.displayFormat.radix === "hex") {
                value = int2hex(value, cfg.displayFormat.bitSize / 4);
            }
            summary += "// " + cfg.displayName + ": " + value + "\n";
        }
        if (summary !== "") {
            summary = "// PARAMETER SUMMARY\n\n" + summary;
        }
        return summary;
    }

    /**
     *  ======== getRclStructFieldArraySize ========
     *  Get the size of an array in a struct field.
     *
     *  @param fieldData - field name (e.g. LRF_shapeBaseGfsk067.coeff)
     *  @returns the size of the array
     */
    // eslint-disable-next-line no-unused-vars
    function getRclStructFieldArraySize(fieldData) {
        const structData = PhyDef.rcl_structs;
        const [struct, field] = fieldData.split(".");
        const data = structData[struct][field].split(",");

        return data.length;
    }

    /**
     *  ======== formatData ========
     *  Format a 32 bit data item for code generation. The data
     *  is presented as a hex value, indented 4 spaces. Comments
     *  are added and vertically aligned.
     *
     *  @param data - The data value to be displayed.
     *  @param width - The data is padded with spaces up to the given width
     *  @param comment - A comment string
     *  @returns Aligned 32 bit data item with comment
     */
    function formatData(data, width, comment) {
        const hex32 = int2hex(data, 8);
        return `    ${hex32},`.padEnd(width) + `// ${comment}\n`;
    }

    /**
     *  ======== initPaTable ========
     *  Initialize the PA table in the PHY definition (LRF_TxPowerTable struct).
     */
    function initPaTable() {
        if ("LRF_txPowerTable" in PhyDef.rcl_structs) {
            // Get PA table (dBm values)
            const paTable = [];
            const paList = RfDesign.getPaTable(FreqBand, false);
            for (const pa of paList) {
                paTable.push(pa._text);
            }
            // Apply to PHY definition, needed for calculating length
            PhyDef.rcl_structs.LRF_txPowerTable.powerTable = paTable.toString();
        }
    }

    /**
     *  ======== generatePaTable ========
     *  Generate the PA table code
     *
     *  @returns The generated string
     */
    function generatePaTable() {
        let paList = RfDesign.getPaTable(FreqBand, false);
        if (paList.length === 0) {
            return "";
        }

        const indent8 = "".padStart(8);
        let code = "{\n";

        // Get PA table (dBm values)
        const paToString = UtilHandle.convPaTableSettingToString;
        paList = paList.reverse();
        const last = paList.pop();
        for (const pa of paList) {
            code += indent8 + paToString(pa._text, pa.Value) + ",\n";
        }
        code += indent8 + paToString(last._text, last.Value) + "\n";

        return code + "    }";
    }

    /**
     *  ======== generateRegConfigCode ========
     *  Generate code for the LPRF registers.
     *
     *  @param inst     - UI instance
     *  @param symNames - Symbol names to use for register symbols.
     *  @param ceConfig - Code export configuration
     *  @returns The generated code
     */
    function generateRegConfigCode(inst, symNames, ceConfig) {
        const symGenMethod = ceConfig.symGenMethod;
        updateRfParamCacheFromInst(inst);

        // Create the segment database
        const segments = createSegments();
        populateSegments(segments);

        let code = "";
        const structNames = [];
        const phyAbbr = ceConfig.phyAbbr && symGenMethod === "Legacy";
        const postfix = getPostfix(phyAbbr);

        // Iterate feature groups
        for (const [featGroup, featData] of Object.entries(segments)) {
            const groupLabel = getFeatureGroupLabel(featGroup);

            // Sort segments by number of features (high to low)
            const segmentsSorted = featData.sort((a, b) => (Number(a.feat.length) > Number(b.feat.length) ? -1 : 1));

            // Iterate segments (combination of features)
            for (const segment of segmentsSorted) {
                segment.code = "";
                let segmentLen = 0;

                // Add data structure 32-bit region
                const structCount = segment.rcl_struct.length;
                if (structCount > 0) {
                    // Generate region header
                    const bytePos = segment.rcl_struct[0].bytePos;
                    const hdr = (0xA000 + structCount - 1) | (bytePos << 16);
                    const hdrComment = `  Data structure 32-bit region `
                        + `(start byte position = ${bytePos}, count = ${structCount})`;

                    segment.code += formatData(hdr, COMMENT_INDENT, hdrComment);
                    segmentLen += 1;

                    // Generate content
                    for (const data of segment.rcl_struct) {
                        let sym = data.value;
                        const id = data.name;
                        const tag = sym.replace("LRF_", "");

                        if (id === "trimDef") {
                            sym = "(fcfg->appTrims)";
                        }
                        else if (id === "txPowerTable") {
                            sym += postfix;
                        }
                        else if (id === "swConfig") {
                            // Look up configurable symbol names
                            if (tag in symNames && symGenMethod !== "Legacy") {
                                sym = symNames[tag];
                            }
                            else {
                                sym += postfix;
                            }
                        }
                        else if (id !== "txPowerTable") {
                            // Look up configurable symbol names
                            if (tag in symNames) {
                                sym = symNames[tag];
                            }
                            else {
                                sym = id;
                            }
                        }
                        // Generate struct symbol
                        if (data.value === "0") {
                            const val = int2hex(data.value, 8);
                            segment.code += `    ${val},`.padEnd(36) + `//     LRF_swParam : ${id}\n`;
                        }
                        else {
                            segment.code += `    (uint32_t) &${sym},`.padEnd(36) + `//     LRF_swParam : ${id}\n`;
                        }
                        segmentLen += 1;
                    }
                }
                // Sort regions by first address (low to high)
                const regions = segment.regions.sort((a, b) => (Number(a.addr) > Number(b.addr) ? 1 : -1));

                // Iterate register regions
                for (const region of regions) {
                    if (region.count > 0) {
                        const regionHdr = (region.addr << 16) + (region.type.id << 12) + (region.count - 1);

                        switch (region.type) {
                        case RegisterRegion.T_HW_16BIT:
                        case RegisterRegion.T_RAM_16BIT:
                            // Process 16 bit region
                            segmentLen += codegen16BitRegion(regionHdr, region, segment);
                            break;
                        case RegisterRegion.T_HW_32BIT:
                        case RegisterRegion.T_RAM_32BIT:
                            // Process 32 bit region
                            segmentLen += codegen32BitRegion(region, segment);
                            break;
                        case RegisterRegion.T_HW_SPARSE:
                        case RegisterRegion.T_RAM_SPARSE:
                            // Process sparse region
                            segmentLen += codegenSparseRegion(regionHdr, region, segment);
                            break;
                        case RegisterRegion.T_HW_MASKED:
                            // Process masked region
                            segmentLen += codegenMaskedRegion(regionHdr, region, segment);
                            break;
                        default:
                            // Process zero region
                            {
                                const comment = `  ${region.type.comment} `
                                        + `(start address = ${region.addr}, count = ${region.count})`;

                                segment.code += formatData(regionHdr, COMMENT_INDENT, comment);
                                segmentLen += 1;
                            }
                            break;
                        }
                    } // else: no registers in the region
                }
                // Wrap up the segment
                if (segmentLen > 0) {
                    // Sometimes comment has comma, which we do not want to remove, so use special method
                    segment.code = Common.replaceLastCharBeforeLastSlash(segment.code, ",", " ");

                    // Create symbol name for segment array
                    const segmentName = segment.name.replace(/,/g, "_");
                    const segIsCommon = segmentName === "common";
                    let symBase;
                    if (segIsCommon) {
                        symBase = "commonRegConfig";
                    }
                    else {
                        symBase = featGroup + "_" + segmentName + "RegConfig";
                    }
                    if (ceConfig.symGenMethod === "Automatic") {
                        symBase += "_" + PhyID;
                    }
                    let symName = "LRF_" + _.camelCase(symBase);

                    // In custom mode a postfix could be added to avoid symbol conflicts
                    if (ceConfig.phyAbbr && symGenMethod === "Custom") {
                        symName += getPostfix(true);
                    }

                    // Generate segment header (directly in register code)
                    const hdr = segment.header | segmentLen;

                    code += "\n// Configuration: ";
                    if (segIsCommon) {
                        code += "Common\n";
                    }
                    else {
                        let segLabel = "";
                        if (segment.label.length > 0) {
                            segLabel = " = " + segment.label.replace(/,/g, ", ");
                        }
                        code += `${groupLabel}${segLabel}\n`;
                    }
                    code += `static const uint32_t ${symName}[] =\n{\n`;
                    code += formatData(hdr, COMMENT_INDENT, `Segment length = ${segmentLen}`);

                    // Add segment code to register code
                    code += segment.code + "};\n";

                    // Add symbol name of segment array
                    structNames.push(symName);
                }
            }
        }

        // Create symbol name code for register list
        const indent8 = "".padStart(8);
        const symCount = structNames.length;
        const cast = "(LRF_ConfigWord*)";

        // The last item is without comma and newline
        const lastStruct = structNames.pop();

        let symNameCode = "";
        for (const struct of structNames) {
            symNameCode += indent8 + cast + struct + ",\n";
        }
        symNameCode += indent8 + cast + lastStruct;

        const regConfListSym = symNames.cfg_segment_list;

        // Code for register list
        const regListCode = `

// LRF register configuration list
static const LRF_RegConfigList ${regConfListSym} = {
    .numEntries = ${symCount},
    .entries = {
${symNameCode}
    }
};
`;
        return code + regListCode;
    }

    /**
     *  ======== codegenSparseRegion ========
     *  Generate code for a sparse region.
     *
     *  @param hdr     - Region header.
     *  @param region  - Region data.
     *  @param segment - Segment information (generated code).
     *  @returns Length of the generated region + 1
     */
    function codegenSparseRegion(hdr, region, segment) {
        // Region header
        const hdrTrimmed = hdr & 0x0000FFFF; // Address masked away
        segment.code += formatData(hdrTrimmed,
            COMMENT_INDENT,
            `  ${region.type.comment} (address/value pairs, count = ${region.count})`);

        // Iterate registers
        for (const regionReg of Common.forceArray(region.regs)) {
            const regName = regionReg.name;
            const reg = segment.register[regName];
            const addr = Number(reg.addr);
            const value = Number(reg.value);

            // Create 32-bit address/value pair
            const val32 = (addr << 16) | value;

            // Generate code line
            segment.code += formatData(val32, COMMENT_INDENT, `    ${regName}`);
        }
        return region.regs.length + 1;
    }

    /**
     *  ======== codegenMaskedRegion ========
     *  Generate code for a masked region.
     *
     *  @param hdr     - Region header.
     *  @param region  - Region data.
     *  @param segment - Segment information (generated code).
     *  @returns Length of the generated region + 1
     */
    function codegenMaskedRegion(hdr, region, segment) {
        // Region header
        segment.code += formatData(hdr,
            COMMENT_INDENT,
            `  ${region.type.comment} (mask/value pairs, count = ${region.count})`);

        // Iterate registers
        for (const regionReg of region.regs) {
            const regName = regionReg.name;
            const reg = segment.register[regName];
            const mask = Number(reg.mask);
            const value = Number(reg.value);

            // Create 32-bit mask/value pair
            const val32 = (mask << 16) | value;

            // Generate code line
            segment.code += formatData(val32, COMMENT_INDENT, `    ${regName}`);
        }
        return region.regs.length + 1;
    }

    /**
     *  ======== codegen16BitRegion ========
     *  Generate code for a 16-bit region.
     *
     *  @param hdr     - Region header.
     *  @param region  - Region data.
     *  @param segment - Segment information (generated code).
     *  @returns Length of the given region + 1.
     */
    function codegen16BitRegion(hdr, region, segment) {
        // Region header
        const addr16 = int2hex(region.addr, 4);
        segment.code += formatData(hdr,
            COMMENT_INDENT,
            `  ${region.type.comment} (start address = ${addr16}, count = ${region.count})`);

        // Iterate registers
        let i = 0;
        while (i < region.regNames.length) {
            const regLsw = region.regNames[i];
            let valLsw = 0;
            if (regLsw !== GAP) {
                valLsw = Number(segment.register[regLsw].value);
            }

            // The most significant word may be out of range
            let regMsw = region.regNames[i + 1];
            let valMsw = 0;
            if (regMsw && regMsw !== GAP) {
                valMsw = Number(segment.register[regMsw].value);
            }
            else {
                regMsw = "-";
            }
            // Create 32-bit combined value
            const val32 = (valMsw << 16) | valLsw;

            // Format code line
            regMsw = regMsw.padEnd(29);
            segment.code += formatData(val32, COMMENT_INDENT, `    ${regMsw} ${regLsw}`);

            i += 2;
        }
        return (i / 2) + 1;
    }

    /**
     *  ======== codegen32BitRegion ========
     *  Generate code for a 32-bit region.
     *
     *  @param region  - Region data.
     *  @param segment - Segment information (generated code).
     *  @returns number of generated regions + 1
     */
    function codegen32BitRegion(region, segment) {
        // Determine address
        let addr;
        if ("alias_addr" in region) {
            addr = region.alias_addr;
        }
        else {
            addr = region.addr;
        }

        // Create region header
        const hdr = (addr << 16) + (region.type.id << 12) + (region.count - 1);
        const addr16 = int2hex(addr, 4);

        segment.code += formatData(hdr,
            COMMENT_INDENT,
            `  ${region.type.comment} (start address = ${addr16}, count = ${region.count})`);

        // Iterate registers
        let i = 0;
        while (i < region.regNames.length) {
            const regLsw = region.regNames[i];
            let valLsw = 0;
            if (regLsw !== GAP) {
                valLsw = Number(segment.register[regLsw].value);
            }

            // The most significant word may be out of range
            let regMsw = region.regNames[i + 1];
            let valMsw = 0;
            if (regMsw) {
                if (regMsw !== GAP) {
                    valMsw = Number(segment.register[regMsw].value);
                }
            }
            else {
                regMsw = "-";
            }

            // Create 32-bit combined value
            const val32 = (valMsw << 16) | valLsw;

            // Format code line
            regMsw = regMsw.padEnd(29);
            segment.code += formatData(val32, COMMENT_INDENT, `    ${regMsw} ${regLsw}`);

            i += 2;
        }
        return (i / 2) + 1;
    }

    /**
     *  ======== generateDataStructCode ========
     *  Generate code for the LPRF data structure.
     *
     *  @param fileType - File to generate ("H" - header, or "C" - implementation).
     *  @param symNames - Symbol names to use for data structure symbols.
     *  @param ceConfig - The current code export configuration
     *  @returns The generated code
     */
    function generateDataStructCode(fileType, symNames, ceConfig) {
        let code = "";
        const structData = PhyDef.rcl_structs;
        const phyAbbr = ceConfig.phyAbbr && ceConfig.symGenMethod === "Legacy";

        // Iterate structs in PHY definition
        for (const [symID, item] of Object.entries(structData)) {
            const typeName = item.attr.type;
            const postfix = getPostfix(phyAbbr);

            // Look up configurable symbol names
            let sym = symID.replace("LRF_", "");
            if (sym in symNames) {
                sym = symNames[sym];
            }
            else {
                sym = symID;
            }
            if (sym === "LRF_txPowerTable") {
                sym += postfix;
            }

            const isPaTable = item.attr.info === "pa_table";

            // Skip generation of PA table for all but first PHY
            if (isPaTable) {
                if (Common.paTableDone) {
                    // eslint-disable-next-line no-continue
                    continue;
                }
            }

            // Header file
            if (fileType === "H") {
                code += `extern const ${typeName} ${sym};\n`;
            }
            // Source file
            else {
                // Get struct type information
                const structInfo = DeviceConfig.rcl_structs[typeName];

                code += `// ${typeName} data structure\n`;
                code += `const ${typeName} ${sym} = {\n`;

                // Iterate members
                const last = structInfo.field.length - 1;
                let i = 0;

                for (const field of structInfo.field) {
                    const isArray = field.name.includes("[]");
                    const name = field.name.replace("[]", "");
                    const type = field.type;
                    const phydefInst = item[name];

                    let value = 0;
                    let prefix = ""; // Typecast etc.

                    if (typeof (phydefInst) !== "object") {
                        // Pointer to another struct?
                        if (Common.isCName(phydefInst) && type.includes("*")) {
                            if (type === "LRF_TOPsmImage*") {
                                const info = findFwInfo(phydefInst);
                                value = info.variable;
                                prefix = "(const LRF_TOPsmImage*) ";
                            }
                            else {
                                const id = phydefInst.replace("LRF_", "");

                                if (id in symNames) {
                                    value = symNames[id];
                                }
                                else {
                                    value = name;
                                }
                                prefix = "&";
                            }
                        }
                        else if (isArray) {
                            if (type === "uint8_t") {
                                // Assume byte array
                                const values = phydefInst.split(",");
                                const lastVal = int2hex(values.pop(), 2);
                                value = "{";
                                for (const val of values) {
                                    const hex = int2hex(val, 2);
                                    value += " " + hex + ",";
                                }
                                value += " " + lastVal + "}";
                            }
                            else if (isPaTable) {
                                value = generatePaTable();
                                Common.paTableDone = true;
                            }
                        }
                        else {
                            // Discrete value
                            if (!type.includes("int")) {
                                prefix = `(${type}) `;
                            }
                            value = phydefInst;
                        }
                    }
                    else if ("script" in phydefInst) {
                        // Value calculated by script
                        const jsCode = phydefInst.script["#text"];
                        value = eval(jsCode);
                    }

                    // Convert to HEX if applicable
                    if (field.type.includes("uint") && !isArray) {
                        const width = Number(field.size);
                        value = int2hex(value, (width + 3) / 4);
                    }

                    // Add code
                    code += `    .${name} = ${prefix}${value}`;
                    if (i < last) {
                        code += ",\n";
                    }
                    i += 1;
                }
                code += "\n};\n\n";
            }
        }
        return code;
    }

    /**
     *  ======== findFwInfo ========
     *  Get information about a firmware image from the PHY definition
     *
     *  @param role - Role of the FW image (pbe_image, mce_image, rfe_image)
     *  @returns Firmware image with given role
     */
    function findFwInfo(role) {
        for (const item of Common.forceArray(PhyDef.rcl_lrf_fw_image)) {
            if (item.role === role) {
                return item;
            }
        }
        throw Error("Firmware info not found: " + role);
    }

    /**
     *  ======== getIncludeFiles ========
     *  Get the include files from the RCL PHY definition
     *
     *  @returns list of objects containing file names and types.
     */
    function getIncludeFiles() {
        // Accumulated list of header files
        const files = [{name: "ti/devices/DeviceFamily.h", type: "common"}];

        // Common include files
        for (const file of Common.forceArray(PhyDef.header_file)) {
            let type = "common";
            if ("type" in file) {
                type = file.type;
            }
            files.push({name: file.name, type: type});
        }
        return files;
    }

    /**
     *  ======== generatePhyFeatureCode ========
     *  Get the PHY features constants from the RCL PHY definition (#defines)
     *
     *  @param symAppend - True if PHY name to be appended
     *  @returns The generated code
     */
    function generatePhyFeatureCode(symAppend) {
        if (!HasFeatures) {
            return null;
        }
        let code = "";
        const symBase = "RCL_PHY_FEATURE_";
        const postfix = symAppend ? "_" + PhyID.replace("_gen", "") : "";

        // Iterate feature groups
        for (const group of Common.forceArray(PhyDef.feature_group)) {
            const symGroup = symBase + group.name.toUpperCase();

            // Iterate features
            const featureList = group.feature;
            for (const feature of featureList) {
                const sym = feature.name + postfix;
                const symFeature = symGroup + "_" + sym.toUpperCase();
                code += `#define ${symFeature.padEnd(45)} ${int2hex(feature.bitmask, 4)}\n`;
            }
        }
        return code;
    }

    /**
     *  ======== getRclRegFieldValue ========
     *  Get the value of a RCL register field, filtered by feature group/filter
     *
     *  @param featureGroup - sub_phy, coded_tx_rate ...
     *  @param desiredFeatureFilter   - Feature filter, e.g. "1_MBS" or "coded"
     *  @param fieldName    - Full path of the register field (module.register.field)
     *
     *  @returns The value of the register or null if not found
     */
    function getRclRegFieldValue(featureGroup, desiredFeatureFilter, fieldName) {
        // Iterate feature groups
        for (const item of Common.forceArray(PhyDef.rcl_registers)) {
            if (item.feature_group === featureGroup || featureGroup === "Combined") {
                // Iterate fields
                for (const rfsData of item.rcl_register_field_settings) {
                    // Check if any of the stated feature filters match what we are looking for
                    if (rfsData.feature_filter === desiredFeatureFilter) {
                        // Iterate registers
                        for (const data of rfsData.data) {
                            // Iterate register fields
                            for (const [fn, featureData] of Object.entries(data.reg)) {
                                if (fieldName === fn.toUpperCase()) {
                                    if (fieldName in UpdatedRegData) {
                                        featureData.value = UpdatedRegData[fieldName];
                                    }
                                    return featureData.value;
                                }
                            }
                        }
                    }
                }
            }
        }
        return null;
    }

    /**
     *  ======== generateRegisterSummary ========
     *  Get the register summary from the RCL PHY definition for both BLE Loki feature groups.
     *
     *  @param inst - UI instance
     *  @returns - Register summary as code comments
     */
    function generateRegisterSummary(inst) {
        let code = "";
        const allRegisters = getAllRegisterFieldData(inst);
        let currentFieldInfo;
        for (const currentRegister of Object.values(allRegisters)) {
            // If we need to insert a new header
            if (currentFieldInfo !== currentRegister.fieldInfo) {
                currentFieldInfo = currentRegister.fieldInfo;
                code += getRegisterSummaryHeader(currentFieldInfo);
            }
            // Add single line of register info
            code += getAlignedRegisterInfo(currentRegister);
        }
        return code;
    }
    /**
     * Gets the associated header with a given fieldInfo
     * @param fieldInfo field info for register
     * @returns Register summary header as code comments
     */
    function getRegisterSummaryHeader(fieldInfo) {
        let header = "";
        for (const fi of fieldInfo) {
            header += fi.name.padEnd(fi.width);
        }
        let res = `// ${header}\n`;

        // Add separator line
        res += "// ".padEnd(header.length, "-") + "\n";
        return res;
    }

    /**
     * Gets information on all register info from the current instance
     *
     * @param inst                   - current instance
     * @param registerFieldSeparator - The desired character separator between fields in register names
     * @returns A mapping of full register name -> register values
     */
    function getAllRegisterFieldData(inst, registerFieldSeparator = ".") {
        // Ensure that register values are up to date
        updateRfParamCacheFromInst(inst);
        updateRegisterData();

        // Fields of the summary
        const fieldInfoBasic = [
            {name: "Address", width: 11},
            {name: "Module", width: 18},
            {name: "Register", width: 21},
            {name: "Bit(s)", width: 11},
            {name: "Field", width: 19}
        ];

        let res = {};
        // Iterate feature groups
        for (const item of Common.forceArray(PhyDef.rcl_registers)) {
            const fieldInfoGroup = [];
            const filters = [];
            let group;

            // Iterate register settings fields
            if (HasFeatures) {
                for (const rfsData of item.rcl_register_field_settings) {
                    const filter = rfsData.feature_filter;
                    if (filter !== "common") {
                        filters.push(filter);
                        fieldInfoGroup.push({name: `Value (${filter})`, width: 18});
                    }
                }
                group = item.feature_group;
            }
            else {
                fieldInfoGroup.push({name: "Value", width: 18});
                group = item.group;
            }

            const fieldInfo = fieldInfoBasic.concat(fieldInfoGroup);
            // join previous results and new ones
            res = {...res, ...getRegisterFeatureGroupInfo(group, filters, fieldInfo, registerFieldSeparator)};
        }
        return res;
    }

    /**
     * Get all registers with given featureGroup and featureFilters
     *
     * assign each of them the given fieldInfo
     *
     * @param featureGroup           - Feature group you want to find register value for
     * @param featureFilters         - Feature filter you want to find register value for
     * @param fieldInfo              - Field info for register
     * @param registerFieldSeparator - The desired character separator between fields in register names
     * @returns A dictionary of { full_register_names : register info } mappings
     */
    function getRegisterFeatureGroupInfo(featureGroup, featureFilters, fieldInfo, registerFieldSeparator = ".") {
        const res = {};
        // Iterate registers
        for (const item of DeviceInfo.getRclRegLookupAddr()) {
            const [name, reg] = item;
            const [module, regName] = name.split(".");
            const addr = int2hex(reg.addr & 0xffff);

            // Iterate bit fields
            for (const [fieldName, field] of Object.entries(reg)) {
                if (typeof field === "object" && field !== null) {
                    // Create bit field string
                    const fpos = Number(field.pos);
                    const width = Number(field.width);
                    let bits = "[";
                    if (width > 1) {
                        bits += (fpos + width - 1) + ":" + field.pos;
                    }
                    else {
                        bits += field.pos;
                    }
                    bits += "]";

                    const baseRegisterInfo = {
                        fieldInfo: fieldInfo,
                        addr: addr,
                        module: module,
                        regName: regName,
                        bits: bits,
                        fieldName: fieldName
                    };
                    // Extract register field values from PHY definition file
                    const searchName = name + "." + fieldName;
                    const outputName = name.replace(".", registerFieldSeparator) + registerFieldSeparator + fieldName;
                    const valCommon = getRclRegFieldValue(featureGroup, "common", searchName);
                    if (featureGroup === "Combined" && valCommon !== null && valCommon !== "") {
                        const val = int2hex(valCommon, (width + 3) / 4);
                        baseRegisterInfo.val = val;
                        res[outputName] = baseRegisterInfo;
                    }
                    else {
                        const values = getFeatGroupValues(featureGroup, featureFilters, searchName, valCommon, width);
                        if (values.isValid) {
                            baseRegisterInfo.val = values;
                            res[outputName] = baseRegisterInfo;
                        }
                    }
                }
            }
        }
        return res;
    }

    /**
     * Return string containing a comment with aligned information about the given register
     * Used in producing register summary at the bottom of the header file
     *
     * @param registerInfo - basic register info
     * @returns String starting with // containing aligned information about the given register
     */
    function getAlignedRegisterInfo(registerInfo) {
        if (registerInfo.val.data instanceof Array) {
            if (registerInfo.val.isValid) {
                return printAligned(registerInfo.fieldInfo,
                    [
                        registerInfo.addr,
                        registerInfo.module,
                        registerInfo.regName,
                        registerInfo.bits,
                        registerInfo.fieldName
                    ].concat(registerInfo.val.data));
            }
            return "";
        }
        return printAligned(registerInfo.fieldInfo,
            [
                registerInfo.addr,
                registerInfo.module,
                registerInfo.regName,
                registerInfo.bits,
                registerInfo.fieldName
            ].concat([registerInfo.val]));
    }

    /**
     * Get aligned register information
     * @param info array of register information, containing width for values
     * @param values register values
     * @returns String with aligned values
     */
    function printAligned(info, values) {
        let str = "// ";
        let i = 0;
        for (let val of values) {
            const fi = info[i];
            if (val == null) {
                val = "-";
            }
            str += val.padEnd(fi.width, " ");
            i += 1;
        }
        return str + "\n";
    }

    /**
     *  Get a set of values from 'n' feature group items
     *
     * @param featGroup   - The given feature group
     * @param featureFilters - List of feature filters
     * @param fieldName   - Name of field
     * @param valCommon   - Value of "common" filter used as fallback
     * @param width       - Width of a given field
     * @returns Object with validity and list of feature group values
     */
    function getFeatGroupValues(featGroup, featureFilters, fieldName, valCommon, width) {
        const values = {isValid: false, data: []};
        for (const featureFilter of featureFilters) {
            let val = getRclRegFieldValue(featGroup, featureFilter, fieldName);
            if (val === null || val === "") {
                val = valCommon;
            }
            if (val !== null && val !== "") {
                val = int2hex(val, (width + 3) / 4);
                values.isValid = true;
            }
            values.data.push(val);
        }
        return values;
    }

    /**
     * Split comma separated string of register names into an array
     *
     * @param registerString string of register names
     * @returns Array of trimmed full register name strings
     */
    function splitRegisterString(registerString) {
        const res = [];
        // split(",") will create an array even with no comma, so can iterate safely
        if (typeof registerString.trim === "function" && registerString.trim().length > 0) {
            for (const registerName of registerString.split(",")) {
                res.push(registerName.trim());
            }
        }
        return res;
    }

    /**
     * Generate pound defines for the registers selected through the API
     * and their values
     *
     * @param ceConfig - The current code export configuration
     * @param inst     - current instance
     * @returns A code string with desired defines
     */
    function generateRegisterFieldDefines(ceConfig, inst) {
        let code = "";
        let invalidRegisterFeedbackCode = "";
        const allRegisters = getAllRegisterFieldData(inst, "_");
        const doubleRegisterValueNames = [];
        // iterate registers stated by users
        const allDesiredRegisterFields = splitRegisterString(ceConfig.registerFieldDefines);
        for (let i = 0; i < allDesiredRegisterFields.length; i++) {
            const desiredRegisterField = allDesiredRegisterFields[i];
            // check that user has stated valid register name
            if (desiredRegisterField in allRegisters) {
                const currentRegister = allRegisters[desiredRegisterField];
                // this is fine if we have a single value (no filters)
                let val = currentRegister.val;
                // if we have multiple values
                if (currentRegister.val.data instanceof Array) {
                    val = currentRegister.val.data[0];
                    // then check that value does not vary based on feature filter
                    for (const value of currentRegister.val.data) {
                        if (value !== val) {
                            // better for error message to include all invalid names instead of just first
                            doubleRegisterValueNames.push(desiredRegisterField);
                        }
                    }
                }
                if (i === 0) {
                    code += "// Register field definitions:\n";
                }
                // one space before val so even if padding is too short, the define is still valid
                code += `#define ${REGISTER_FIELD_DEFINES_PREFIX}${desiredRegisterField}`
                    .padEnd(REGISTER_FIELD_DEFINE_NAME_COL) + ` ${val}\n`;
            }
            else {
                // Store to later add at the end of all the valid registers
                invalidRegisterFeedbackCode += `// ERROR: ${desiredRegisterField} is not a valid register name\n`;
            }
        }
        code += invalidRegisterFeedbackCode;
        if (doubleRegisterValueNames.length !== 0) {
            throw Error(`Cannot define fields where value depends on feature filter: ${doubleRegisterValueNames}`);
        }
        return code;
    }

    /**
     *  ======== generateRfCommandCode ========
     *  Generate code for RF commands.
     *
     *  @param fileType - Whether header file ('H') or implementation ('C').
     *  @param symNames - Symbol names from "code generation configW module.
     *  @param ceConfig - Code generation configuration
     *  @returns The generated code
     */
    function generateRfCommandCode(fileType, symNames, ceConfig) {
        const genHeader = fileType === "H";
        const qualifier = ceConfig.useConst ? "const " : "";

        let code = "";
        const cmds = cmdHandler.getCommands();
        for (const cmd of cmds) {
            const cmdName = cmdHandler.getCommandName(cmd);
            const cmdKey = _.camelCase(cmdName);

            if (cmdKey in symNames) {
                const cmdBase = Common.ucFirst(cmdKey);
                const cmdDescr = cmdHandler.getCommandDescription(cmdName);
                const varName = symNames[cmdKey];

                // H file
                if (genHeader) {
                    code += `extern ${qualifier}RCL_${cmdBase} ${varName};\n`;
                }
                // C file
                else {
                    let testLabel = "testLabel" in cmd ? cmd.testLabel : "";
                    // Generate command code
                    if (testLabel !== "") {
                        testLabel += " -";
                    }
                    code += `// Command: ${testLabel}${cmdName} - ${cmdDescr}\n`;
                    code += `${qualifier}RCL_${cmdBase} ${varName} = \n{\n`;
                    const fieldCode = cmdHandler.generateRfCmdCodeRCL(cmd).rfCmd;
                    code += fieldCode.join(",\n");
                    code += "\n};\n\n";
                }
            }
        }

        if (code !== "") {
            // Code for .h file
            if (genHeader) {
                return "// RCL commands\n" + code;
            }
            // Code for .c file
            return Common.replaceLastChar(code, "\n", "");
        }
        return "";
    }

    /**
     *  ======== getPhyPropertyByName ========
     *  Initialize the RF parameters cache with values
     *  from the PHY definition file (PHY properties)
     *
     *  @param name  - PHY property name
     *  @returns PhyProperty
     */
    function getPhyPropertyByName(name) {
        if (name in RfPropMap) {
            return RfPropMap[name];
        }
        return null;
    }

    /**
     *  ======== getPostfix ========
     *  Create a postfix for a symbol name based on PHY abbreviation
     *  (empty string if PHY abbreviation not used).
     *
     *  @param phyAbbr  - True if PHY abbreviation in use
     *  @returns Postfix for symbols for current phy
     */
    function getPostfix(phyAbbr) {
        let postfix = "";
        if (phyAbbr) {
            postfix = _.camelCase(PhyID.replace("_gen", ""));
            postfix = Common.ucFirst(postfix);
        }
        return postfix;
    }

    /**
     *  ======== validatePhyProperty ========
     *  Validate a PHY property using the common Javascript functions
     *
     *  @param phyProp - Name of PHY property
     *  @returns status return value of funciton is valid, otherwise null
     */
    function validatePhyProperty(phyProp) {
        const prop = getPhyPropertyByName(phyProp);
        if (prop !== null && "script" in prop) {
            const scripts = Common.forceArray(prop.script);
            // Requires validation ?
            if (scripts.length > 1) {
                const valScript = scripts[1];
                if (valScript.role.match(/validate_error/)) {
                    if ("call" in valScript) {
                        // Call to function
                        const code = "UtilHandle." + valScript.call;
                        // eslint-disable-next-line no-eval
                        const val = eval(code);
                        if (val !== "") {
                            // Validation failed
                            const status = {
                                valid: false,
                                msg: val,
                                config: phyProp
                            };
                            return status;
                        }
                        // Validation passed
                        return null;
                    }
                }
            }
        }
        return null;
    }

    /**
     *  ======== isPhyPropSupported ========
     *  Check if a PHY property is supported in the selected RCL commands
     *
     *  @param inst      - UI instances
     *  @param phyProp   - Name of PHY property (SRFS8)
     *  @param selection - "default" or "actual"
     *  @returns boolean
     */
    function isPhyPropSupported(inst, phyProp, selection = "default") {
        const usedCmds = cmdHandler.getUsedCommands(inst, selection);
        if (usedCmds.length > 0) {
            const field = PhyPropCmdField[phyProp];
            const cmds = cmdHandler.getCommandBuffer();

            // Search for relevant command field
            for (const cmdName of usedCmds) {
                if (cmdName in cmds) {
                    const cmdDef = cmds[cmdName];
                    for (const cmd of cmdDef) {
                        if (cmd.name === field) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    /**
     *  ======== hasFeatureGroup ========
     *  Check if the phy has a given feature group
     *
     *  @param featureGroup - feature group (e.g. sub_phy, coded_tx_rate)
     *  @returns boolean
     */
    function hasFeatureGroup(featureGroup) {
        if (HasFeatures) {
            for (const fg of Common.forceArray(PhyDef.feature_group)) {
                if (fg.name === featureGroup) {
                    return true;
                }
            }
        }
        return false;
    }

    // Return the module's exported functions
    return {
        getPhyGroup: () => PhyGroup,
        getPhyName: () => PhyID,
        getPhydefFile: () => PhyDefFile,
        getPhydefLongName: () => PhyInfo.label,
        is154g: is154g,
        initConfigurables: initConfigurables,
        isExternalRelease: () => IsExternalReleasePhy,
        getCmdList: cmdHandler.getCmdList,
        isPhyPropSupported: isPhyPropSupported,
        getRfData: getRfData,
        getLrfImages: () => Common.forceArray(PhyDef.rcl_lrf_fw_image),
        updateRfCommands: updateRfCommands,
        hasFeatureGroup: hasFeatureGroup,
        // Code export for header file
        getIncludeFiles: getIncludeFiles,
        generatePhyFeatureCode: generatePhyFeatureCode,
        generateRegisterFieldDefines: generateRegisterFieldDefines,
        generateRegisterSummary: generateRegisterSummary,
        // Code export for C file
        generateParameterSummary: generateParameterSummary,
        generateRfCommandCode: generateRfCommandCode,
        generateRegConfigCode: generateRegConfigCode,
        generateDataStructCode: generateDataStructCode,
        // Validation
        validatePhyProperty: validatePhyProperty
    };
}
