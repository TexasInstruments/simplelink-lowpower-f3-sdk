/* eslint-disable guard-for-in */
/*
 * Copyright (c) 2023 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== cmd_handler.js ========
 *  Module to access RF Command raw data (SmartRF Studio 8 format).
 *  Supports RFD and RCL format.
 */

"use strict";

// Common utility functions
const Common = system.getScript("/ti/devices/radioconfig/radioconfig_common.js");
const ConfigPath = Common.ConfigPath;
const ConfigPathRclCommon = Common.ConfigPathRclCommon;

// Distinguish between RFD and RCL
const isRFD = Common.isPlatformRFD();
const isRCL = !isRFD;

let CmdScripts = null;
if (isRCL) {
    CmdScripts = system.getScript(ConfigPathRclCommon + `rfcmd_scripts.json`);
}

// Other dependencies
const DeviceInfo = Common.getScript("device_info.js");
// eslint-disable-next-line no-unused-vars
const OverrideHandler = Common.getScript("override_handler.js"); // RFD only

// Command definitions
const AllCmds = DeviceInfo.loadCommands();
const RfCommands = isRFD ? AllCmds.rfd_commands_def.command : AllCmds.rcl_commands_def.command;

// Ensure that Command Handlers are not created twice
const CmdHandlerCache = {};

// Utility functions
const int2hex = Common.int2hex;

// Exported functions
exports = {
    get: get
};

/**
 *  ======== get ========
 *  Get the Command Handler for a PHY layer. Create the
 *  Command Handler if it does not already exist.
 *
 *  @param setting - RF setting (PHY definition)
 *  @param phyGroup - ble, prop, ieee_154
 *  @param phyName - short name for the PHY layer
 */
function get(setting, phyGroup, phyName) {
    if (!(phyName in CmdHandlerCache)) {
        CmdHandlerCache[phyName] = create(setting, phyGroup, phyName);
    }

    return CmdHandlerCache[phyName];
}

/**
 *  ======== create ========
 *  Create a setting specific instance of the command handler, using
 *  SmartRF Studio 8 data.
 *
 *  @param phyDef - PHY definition
 *  @param phyGroup - ble, prop, ieee_154
 *  @param phyName - short name for the PHY layer
 */
function create(phyDef, phyGroup, phyName) {
    const PhyGroup = phyGroup;
    const PhyDef = _.cloneDeep(phyDef);
    const PhyName = phyName;

    // Get configuration for PHY group
    const DeviceConfig = DeviceInfo.getConfiguration(phyGroup);

    // Command definitions for this device and PHY
    const CmdDef = [];

    // Command buffers
    const CmdBuf = {};

    // Names of commands used by setting
    const CmdUsed = [];

    // PHY property to RF command mapping
    let ParamMap = null;
    if (isRFD) {
        const fileData = system.getScript(ConfigPath + `param_cmd_mapping_${phyGroup}.json`);
        ParamMap = addParameterMapping(fileData);
    }

    // Commands that are part of the PHY definition
    const PhyDefCommands = isRFD ? PhyDef.rfd_command : getRclRfCommands();

    // Command definitions for this device and PHY
    loadRfCommandDefs();

    // Commands contained in the setting
    initUsedCommands();

    // Add commands that are not used by the setting
    addUnusedCommands();

    // Create buffer for RF commands
    createCommandBuffer();
    initDefaultValues();

    // Add Javascript snippets to RF commands
    addRfCmdScripts();

    /**
     *  ======== getRclRfCommands  ========
     *  Create list of RF commands in the current PHY.
     */
    function getRclRfCommands() {
        let rfCommands = [];
        // Common RF commands (e.g. CMD_GENERIC_FS)
        if ("rcl_command" in phyDef) {
            rfCommands = phyDef.rcl_command;
        }
        // Add commands from the test modes (e.g. CMD_GENERIC_TX)
        if ("rcl_test_mode" in phyDef) {
            for (const tmode of Common.forceArray(phyDef.rcl_test_mode)) {
                if ("rcl_command" in tmode) {
                    for (const cmd of Common.forceArray(tmode.rcl_command)) {
                        cmd.testLabel = tmode.label;
                        rfCommands.push(cmd);
                    }
                }
            }
        }
        return rfCommands;
    }

    /**
     *  ======== loadRfCommandDef  ========
     *  Get the command definitions that apply to the current PHY group.
     */
    function loadRfCommandDefs() {
        const mask = 0xF800;

        for (const cmd of RfCommands) {
            const id = parseInt(cmd.id);
            // eslint-disable-next-line no-bitwise
            const idGroup = id & mask;

            let toInclude = idGroup === 0x0800
                || (PhyGroup === "ble" && (idGroup === 0x1800))
                || (PhyGroup === "ieee_15_4" && (idGroup === 0x2800))
                || (PhyGroup === "prop" && (idGroup === 0x3800))
                || isRCL; // RCL only: So far all commands are used (generic)

            // wBMS requires proprietary advanced commands
            if (DeviceInfo.hasWbmsSupport()) {
                // CMD_PROP_TX_ADV || CMD_PROP_RX_ADV
                if (id === 0x3803 || id === 0x3804) {
                    toInclude = true;
                }
            }

            // Include the command if applicable
            if (toInclude) {
                CmdDef.push(cmd);
            }
        }
    }

    /**
     *  ======== addParameterMapping  ========
     *  Map RF parameters to RF commands. An RF parameter
     *  may be mapped to multiple RF commands (e.g. txPower).
     *
     *  RFD only.
     *
     *  @param - map of parameter versus RF commands
     */
    function addParameterMapping(map) {
        const paramMap = [];

        for (const parItem of map.mapping.settings.parameters.parameter) {
            const params = parItem._names.split(",");
            for (const par of params) {
                const cmds = parItem.rfCommand;
                const pmap = Common.forceArray(cmds);
                let lpar = par;
                if (par === "CARRIER_FREQUENCY") {
                    lpar = "CARRIERFREQUENCY";
                }
                paramMap[lpar.toUpperCase()] = pmap;
            }
        }
        return paramMap;
    }

    /**
     *  ======== addRfCmdScripts  ========
     *  Add RF command scripts to commands that are not part of a PHY definition.
     *
     *  RCL only
     */
    function addRfCmdScripts() {
        if (CmdScripts === null) {
            // Only RCL so far
            return;
        }
        const key = PhyName === "ble_gen" ? "ble" : PhyName;
        const scripts = CmdScripts[key];

        // Iterate command buffer
        for (const [name, cmd] of Object.entries(CmdBuf)) {
            // Add scripts if applicable
            if (scripts !== undefined && name in scripts) {
                // Iterate fields in rF command
                const cmdScript = scripts[name];
                for (const field of cmd) {
                    if (field.name !== undefined && field.name in cmdScript) {
                        field.script = cmdScript[field.name];
                    }
                }
            }
        }
    }

    /**
     *  ======== createCommandBuffer  ========
     *  Create a cache for RF commands. It contains the
     *  data from which RF Command code is generated.
     */
    function createCommandBuffer() {
        // Process basic commands
        for (const cmd of PhyDefCommands) {
            processCommand(cmd);
        }

        function processCommand(cmd) {
            const cmdBuf = [];
            const cmdName = cmd.name;
            const cmdDef = getCommandDefByName(cmdName);
            let paramByteField = null;
            const fieldValue = "field_value" in cmdDef ? cmdDef.field_value : null;

            // Process RF command
            const fields = isRFD ? cmdDef.byte_field : cmdDef.field;
            Common.forceArray(fields).forEach((byteField) => {
                processByteField(byteField);
            });

            // Process RF parameter (if applicable)
            if (paramByteField !== null) {
                paramByteField.ext_byte_field.forEach((byteField) => {
                    processByteField(byteField);
                });
            }

            // Store command fields in command buffer
            CmdBuf[cmdName] = cmdBuf;

            function processByteField(byteField) {
                const byteIndex = byteField.pos;
                const bytefieldName = byteField.name;

                if ("bit_field" in byteField) {
                    // Word contains bit fields
                    const bitFields = Common.forceArray(byteField.bit_field);

                    for (const bitfield of bitFields) {
                        const name = bytefieldName + "." + bitfield.name;
                        const fullName = cmdName + "." + name;
                        const item = {
                            name: name,
                            isPointer: false,
                            byteOffset: byteIndex,
                            width: bitfield.size > 4 ? Math.floor((Number(bitfield.size) + 3) / 4) : 1,
                            value: bitfield.value,
                            default: getSettingFieldValue(fullName, bitfield.value)
                        };
                        cmdBuf.push(item);
                    }
                }
                else {
                    // Special handling for PHY features (TBD: check if hard-coding can be avoided)
                    let fieldName = bytefieldName;
                    if ("name" in byteField && fieldName.includes("common")) {
                        fieldName = "common.phyFeatures";
                    }

                    const fullName = cmdName + "." + fieldName;
                    const isPointer = "type" in byteField && byteField.type.includes("_t*");
                    const isGap = "gap" in byteField;
                    const width = isRFD ? Number(byteField.size) * 2 : (Number(byteField.size) + 3) / 4;
                    const fieldDefault = "default" in byteField ? byteField.default : byteField.value;

                    const item = {
                        name: fieldName,
                        isPointer: isPointer,
                        byteOffset: byteIndex,
                        width: width,
                        default: getSettingFieldValue(fullName, fieldDefault),
                        script: getScript(fullName)
                    };

                    if (fieldValue !== null) {
                        item.fieldValue = Common.forceArray(fieldValue);
                    }
                    if ("type" in byteField) {
                        item.type = byteField.type;
                    }
                    if (isGap) {
                        item.gap = byteField.gap;
                    }
                    if (bytefieldName === "pParams") {
                        paramByteField = byteField;
                    }
                    cmdBuf.push(item);
                }
            }
        }
    }

    /**
     *  ======== initDefaultValues ========
     *  Initialize the RF commands with default values and options
     *  (for fields that rely on scripts)
     */
    function initDefaultValues() {
        for (const cmdName in CmdBuf) {
            const cmd = CmdBuf[cmdName];

            for (const i in cmd) {
                const field = cmd[i];
                const fullName = cmdName + "." + field.name;

                const val = getSettingFieldDefault(fullName);
                if (val !== null) {
                    field.default = val;
                    field.value = val;
                }
                else {
                    field.value = field.default;
                }
            }
        }
    }

    /**
     *  ======== getCmdList ========
     *  Get list of commands available for this setting
     *
     *  @param cmdSet - "all": all commands defined for this setting's PHY group
     *                  "basic": commands needed for packet RX/TX
     *                  "advanced": commands needed for packet RX/TX + test (cont. RX/TX)
     */
    function getCmdList(cmdSet) {
        let ret = [];

        if (cmdSet === "all") {
            ret = Object.keys(CmdBuf);
        }
        else if (cmdSet === "advanced") {
            ret = CmdUsed;
        }
        else if (cmdSet === "basic") {
            ret = _.without(CmdUsed, "CMD_TX_TEST", "CMD_RX_TEST");
        }
        else {
            throw Error("Invalid command selection: " + cmdSet);
        }
        return ret;
    }

    function getFieldInfo(cmdName, fieldName) {
        // Find command
        const cmdDef = getCommandDefByName(cmdName);
        for (const field of cmdDef.field) {
            if (field.name === fieldName) {
                return field;
            }
        }
        return null;
    }

    /**
     *  ======== generateRfCmdCodeRCL ========
     *  Generate code for the given RF command, returns the content of a C struct.
     *  The enclosing variable declaration resides in the template.
     *
     *  @param cmd - Object describing the RF command
     */
    function generateRfCmdCodeRCL(cmd) {
        const cmdName = cmd.name;
        const fields = Common.forceArray(CmdBuf[cmdName]);
        const rclStructs = DeviceConfig.rcl_structs;
        const rclCmdStruct = rclStructs.RCL_Command;
        const code = {
            parStructName: "",
            rfCmd: [],
            rfPar: []
        };

        let processingParam = false;
        let str;

        // Special processing for Command Number
        if (isRFD) {
            const field = fields[0];
            str = "    ." + field.name + " = " + getCommandNumber(cmdName);
            code.rfCmd.push(str);
        }

        // Iterate over the other fields
        let j = isRFD ? 1 : 0;
        let prevOffset = Number.MIN_VALUE;

        while (j < fields.length) {
            const field = fields[j];
            let val = field.value;

            if ("gap" in field) {
                j += 1;
                // eslint-disable-next-line no-continue
                continue;
            }

            if ("type" in field) {
                if (field.type.includes("uint")) {
                    // Simple field
                    const fi = getFieldInfo(cmdName, field.name);
                    str = processRclCommandField(fi, null, field);
                }
                else if (field.type === "List_List") {
                    // Special processing
                    str = "    ." + field.name + ".head = (List_Elem*) 0";
                    code.rfCmd.push(str);
                    str = "    ." + field.name + ".tail = (List_Elem*) 0";
                }
                else if (field.type === "RCL_Command") {
                    // Special processing
                    for (const item of rclCmdStruct.field) {
                        const type = item.type;
                        if (type.includes("RCL_Command") && type in rclStructs) {
                            // Contains another RCL command
                            const rclCommand = rclStructs[type].field;
                            for (const cmdField of rclCommand) {
                                str = processRclCommandField(cmdField, "common." + item.name, field);
                                code.rfCmd.push(str);
                            }
                        }
                        else {
                            // Simple field
                            str = processRclCommandField(item, "common", field);
                            code.rfCmd.push(str);
                        }
                    }
                    str = null;
                }
                else if (!field.type.includes("int")) {
                    // Needs type cast
                    str = `    .${field.name} = (${field.type}) ${val}`;
                }
                else {
                    // Use directly
                    str = "    ." + field.name + " = " + val;
                }
            }
            else {
                // Bit field
                val = Number(val);
                if (field.width > 1) {
                    val = int2hex(val, field.width);
                }
                str = "    ." + field.name + " = " + val;
            }

            // Reached the start of the parameter data
            const currentOffset = parseInt(field.byteOffset);
            if (currentOffset < prevOffset) {
                processingParam = true;
            }
            prevOffset = currentOffset;

            // Aggregate command and parameter struct (RFD only)
            if (processingParam) {
                code.rfPar.push(str);
            }
            else if (str !== null) {
                code.rfCmd.push(str);
            }
            j += 1;
        }
        return code;
    }

    /**
     *  ======== generateRfCmdCodeRFD ========
     *  Generate code for the given RF command, returns the content of a C struct.
     *  The enclosing variable declaration resides in the template.
     *
     *  @param cmd - Object describing the RF command
     *  @param symNames - symbol names from Code Generation config
     *  @legacy - generate code with legacy symbol names
     */
    function generateRfCmdCodeRFD(cmd, symNames, legacy) {
        const code = {
            parStructName: "",
            rfCmd: [],
            rfPar: []
        };
        return code;
    }

    /**
     *  ======== processRclCommandField ========
     *  Generate code for an RCL command field.
     *
     *  @param cmdField - Command field info from command definition file.
     *  @base - Root of command field (e.g. common.timing, txBuffers, config)
     *  @field - Field information from PHY definition
     */
    function processRclCommandField(cmdField, base, field) {
        let sym = base !== null ? base + "." + cmdField.name : cmdField.name;
        let fv = 0;

        if ("fieldValue" in field) {
            fv = getFieldValue(field.fieldValue, sym);
            if (fv === null) {
                // Value set by user?
                if (sym === field.name && "value" in field && (typeof field.value !== "undefined")) {
                    fv = field.value;
                }
                else if ("default" in cmdField) {
                    // Use default value
                    fv = cmdField.default;
                }
                // Special case
                else if (cmdField.name.includes("CallbackMask")) {
                    fv = int2hex(0, 8);
                    sym += ".value";
                }
                else {
                    fv = `(${cmdField.type}) 0`;
                }
            }
        }
        // Use enums if available
        let isSymbol = false;
        if ("enum" in cmdField) {
            const enu = getEnum(cmdField.enum, fv);
            if (enu !== null) {
                if ("symbol" in enu) {
                    fv = enu.symbol;
                    isSymbol = true;
                }
                else if ("value" in enu) {
                    fv = enu.value;
                }
            }
        }
        if (cmdField.type.includes("uint") && !isSymbol) {
            const width = (Number(cmdField.size) + 3) / 4;
            fv = int2hex(fv, width);
        }
        return `    .${sym} = ${fv}`;

        function getFieldValue(fieldValues, name) {
            for (const fd of fieldValues) {
                if (name === fd.name) {
                    return fd.value;
                }
            }
            return null;
        }

        function getEnum(enums, value) {
            for (const enu of Common.forceArray(enums)) {
                if (value === enu.value) {
                    return enu;
                }
            }
            return null;
        }
    }

    /**
     *  ======== getCommandMap ========
     *  Get a list of commands that map to the given RF parameter.
     *
     *  @param rfParam - RF parameter name
     */
    function getCommandMap(rfParam) {
        const uc = rfParam.toUpperCase();
        if (uc in ParamMap) {
            return ParamMap[uc];
        }
        return null;
    }

    /**
     *  ======== getUsedCommands ========
     *  Get a list of commands that are used by the design.
     *
     *  @param inst - PHY setting instance
     *  @param selection - "default" or "actual"
     */
    function getUsedCommands(inst, selection = "default") {
        if (selection === "default") {
            return CmdUsed;
        }

        // Commands as selected in code export configuration
        let ret = [];
        const ce = inst.codeExportConfig;

        switch (PhyGroup) {
        case Common.PHY_BLE:
            ret = ce.cmdList_ble;
            break;
        case Common.PHY_IEEE_15_4:
            ret = inst.codeExportConfig.cmdList_ieee_15_4;
            break;
        default:
            ret = inst.codeExportConfig.cmdList_prop;
            break;
        }
        ret = _.map(ret, (cmd) => _.snakeCase(cmd).toUpperCase().replace("BLE_5", "BLE5"));

        return ret;
    }

    /**
     * ======== getSettingFieldDefault ========
     * Get the default value of a field (from settings file).
     *
     * @param name - setting field name
     */
    function getSettingFieldDefault(name) {
        const field = getSettingFieldByName(name);
        if (field !== null) {
            if ("value" in field) {
                return field.value;
            }
        }
        return null;
    }

    /**
     * ======== getSettingFieldValue ========
     * Get value of a field (from settings file)
     *
     * @param name - setting field name
     * @param initValue - the value to be used if the field is unused
     */
    function getSettingFieldValue(name, initValue) {
        // RF command field default value is missing: e.g. CMD_GENERIC_FS.fsType
        const ret = (typeof initValue === "undefined") ? "0" : initValue;

        const field = getSettingFieldByName(name);
        if (field !== null) {
            if ("value" in field) {
                return field.value;
            }
        }
        return ret;
    }

    /**
     * ======== getScript ========
     * Get script information if applicable
     *
     * @param name - setting field name
     */
    function getScript(name) {
        const field = getSettingFieldByName(name);
        if (field !== null) {
            if ("script" in field) {
                return field.script;
            }
        }
        return null;
    }

    /**
     * ======== getSettingFieldByName ========
     * Get an RF command buffer field by name (CMD_FS.frequency ....)
     *
     * The name argument may optionally be prepended with the command name
     * in order to solve ambiguities (e.g. ".syncWord" is part of both CMD_PROP_TX
     * and CMD_PROP_TX.
     *
     * If the command name is not part of the argument, the function will
     * return the first matching field.
     *
     * @param name - field name
     */
    function getSettingFieldByName(name) {
        const commands = PhyDefCommands;
        let cmdName = null;
        let fieldName = name;
        let ret = null;

        if (name.includes("CMD")) {
            // Extract command name (all characters up to '.')
            cmdName = name.substr(0, name.indexOf("."));
            fieldName = name.substr(name.indexOf(".") + 1);
        }

        const fieldType = isRFD ? "rfd_command_field" : "rcl_command_field";

        // Iterate RF commands
        _.each(commands, (cmd) => {
            if (fieldType in cmd) {
                // Iterate command fields
                const commandFields = Common.forceArray(cmd[fieldType]);
                for (const field of commandFields) {
                    if (fieldName === field.name && cmdName === null) {
                        ret = field;
                        return false;
                    }
                    else if (fieldName === field.name && cmdName === cmd.name) {
                        ret = field;
                        return false;
                    }
                }
            }
            // Continue _.each iteration
            return true;
        });
        return ret;
    }

    /**
     * ======== getCmdFieldValueByOpt ========
     * Convert an RF Command value to a corresponding RF parameter
     * option value. See setCmdFieldValueByOpt.
     *
     * @param paramName - PHY property name
     * @param fieldName - RF command field name
     */
    function getCmdFieldValueByOpt(paramName, fieldName) {
        const opts = Common.getOptions(DeviceConfig.configs, paramName);
        const key = getCommandFieldValue(fieldName);

        // Look for an options with the given key
        let val = null;
        _.each(opts, (opt) => {
            // eslint-disable-next-line
            if (opt.key == key) {
                val = opt.name;
                return false;
            }
            // Continue _.each iteration
            return true;
        });

        if (val === null) {
            // Workaround for problem with default TX Power values in some settings.
            // The default is not in the PA table, so we use the first in the list.
            val = opts[0].name;
        }
        return val;
    }

    /**
     *  ======== getCommandFieldValue ========
     *  Get the value of a field in an RF command
     *
     *  Field name formats:
     *  - a (e.g. "txPower")
     *  - a.b (e.g."preamConf.nPreamBytes")
     *  - CMD.a (e.g. "CMD_PROP_TX.syncWord")
     *
     *  @param fieldName - field name
     */
    function getCommandFieldValue(fieldName) {
        const field = getCmdFieldByName(fieldName);

        if (field != null) {
            return field.value;
        }
        throw Error("RF command field not found: " + fieldName);
    }

    /**
    *  ======== setCmdFieldValue ========
    *  Set the value of a field in an RF command
    *  (the same field may be present in several commands)
    *
    *  NB! This function only writes to commands that are present in the parameter mapping.
    *
    *  Field name formats:
    *  - a  (e.g. "txPower")
    *  - a.b (e.g."preamConf.nPreamBytes")
    *
    *  @param paramName - RF parameter name (filter for RF parameter mapping)
    *  @param fieldName - field name
    *  @param value - value to populate the command fields with
    *
    */
    function setCmdFieldValue(paramName, fieldName, value) {
        if (value === undefined) {
            // TBD: temporary workaround for initialization
            return;
        }
        const fields = getCmdFieldsByName(fieldName);

        _.each(fields, (item) => {
            const field = item.field;
            const cmd = item.cmd;

            // Check if the field is present in the Parameter Map
            const parUc = paramName.toUpperCase();
            if (parUc in ParamMap) {
                // Check if command contains this field
                const parArray = ParamMap[parUc];

                if (parArray.includes(cmd)) {
                    field.value = value;
                }
            }
        });
    }

    /**
     * ======== getCmdFieldByName ========
     * Get an RF command field by name (CMD_FS.frequency ....)
     *
     * The name argument may optionally be prepended with the command name
     * in order to solve ambiguities (e.g. ".syncWord" is part of both CMD_PROP_TX
     * and CMD_PROP_TX.
     *
     * If the command name is not part of the argument, the function will
     * return the first matching field.
     *
     * @param name - field name
     */
    function getCmdFieldByName(name) {
        const commands = CmdBuf;
        let cmdName = null;
        let fieldName = name;
        let ret = null;

        if (name.includes("CMD")) {
            // Extract command name (all characters up to '.')
            cmdName = name.substr(0, name.indexOf("."));
            fieldName = name.substr(name.indexOf(".") + 1);
        }

        // Iterate RF commands
        _.each(commands, (cmd, thisCmdName) => {
            // Iterate command fields (byte(s) or bit fields
            for (let j = 0; j < cmd.length; j++) {
                const field = cmd[j];
                if (fieldName === field.name && cmdName === null) {
                    ret = field;
                    return false;
                }
                else if (fieldName === field.name && cmdName === thisCmdName) {
                    ret = field;
                    return false;
                }
            }
            // Continue _.each iteration
            return true;
        });
        return ret;
    }

    /**
     * ======== getCmdFieldsByName ========
     * Get RF command fields by name (e.g. frequency ....)
     * The same field may be present in several commands so the
     * corresponding list is returned.
     *
     * @param name - field name
     */
    function getCmdFieldsByName(name) {
        const commands = CmdBuf;
        const ret = [];

        // Iterate RF commands
        _.each(commands, (cmd, thisCmdName) => {
            // Iterate command fields (byte(s) or bit fields
            for (let j = 0; j < cmd.length; j++) {
                const field = cmd[j];
                if (name === field.name) {
                    ret.push({
                        cmd: thisCmdName, field: field
                    });
                }
            }
        });
        return ret;
    }

    /**
     *  ======== setCmdFieldValueDirect ========
     *  Set the value of a field in an RF command without referring to RF Parameter
     *  (the same field may be present in several commands).
     *
     *  NB! This function ignores the parameter mapping.
     *
     *  Field name formats:
     *  - a  (e.g. "txPower")
     *  - a.b (e.g."preamConf.nPreamBytes")
     *
     *  @param fieldName - field name
     *  @param value - value to populate the command field
     *  @param force - apply also to settings controlled by RF parameters
     */
    function setCmdFieldValueDirect(fieldName, value, force) {
        const fields = getCmdFieldsByName(fieldName);
        _.each(fields, (item) => {
            const field = item.field;
            const cmd = item.cmd;
            // Don't overwrite commands that are included in a setting
            if (!_.includes(CmdUsed, cmd) || force) {
                // eslint-disable-next-line eqeqeq
                if (field.name == fieldName) {
                    field.value = value;
                }
            }
        });
    }

    /**
     *  ======== getCommandNumber ========
     *  Get the RF Command number (ID) by name.
     *  This is 16-bit HEX value containing in the field ".commandNo" in the command struct
     *
     *  @param name - RF Command name (CMD_FS, CMD_PROP_RADIO_DIV_SETUP ...)
     */
    function getCommandNumber(name) {
        const cmd = getCommandDefByName(name);

        return cmd.id;
    }

    /**
    *  ======== initUsedCommands ========
    *  Initial list of used commands.
    */
    function initUsedCommands() {
        if (PhyDefCommands.length === 0) {
            return;
        }

        PhyDefCommands.forEach((cmd) => {
            CmdUsed.push(cmd.name);
        });

        const testModes = Common.forceArray(isRFD ? PhyDef.rfd_test_mode : PhyDef.rcl_test_mode);
        testModes.forEach((testMode) => {
            const testLabel = testMode.label; // Used in code generation
            const commands = Common.forceArray(isRFD ? testMode.rfd_command : testMode.rcl_command);
            commands.forEach((cmd) => {
                const name = cmd.name;
                if (!(CmdUsed.includes(name))) {
                    CmdUsed.push(name);
                    cmd.testLabel = testLabel;
                    // Append commands to main command list
                    PhyDefCommands.push(cmd);
                }
            });
        });
        // Remove test commands
        if (isRFD) {
            delete (PhyDef.rfd_test_mode.rfd_command);
        }
    }

    /**
     *  ======== addUnusedCommands ========
     *  Add unused commands to the list as these may optionally be used
     *  by the code export. Unused commands are those not referred to
     *  in PHY definition files but may be added later.
     */
    function addUnusedCommands() {
        const currentCmds = PhyDefCommands;

        _.each(CmdDef, (cmd) => {
            const name = cmd.name;

            // Check if duplicate
            let present = false;
            _.each(currentCmds, (currentCmd) => {
                if (name === currentCmd.name || CmdUsed.includes(name)) {
                    present = true;
                    return false;
                }
                // Continue _.each iteration
                return true;
            });

            // Add if not already in place
            if (!present) {
                currentCmds.push(cmd);
            }
        });
    }

    /**
    *  ======== getCommandDefByName ========
     * Get a command definition by name.
     *
     *  Extracts RF Command definition from "all.rfdcmds.json"
     *
     *  @param name - RF Command (CMD_FS, CMD_PROP_RADIO_DIV_SETUP ...)
     */
    function getCommandDefByName(name) {
        let ret = null;

        CmdDef.forEach((cmd) => {
            if (name === cmd.name) {
                ret = cmd;
                return false;
            }
            // Continue _.each iteration
            return true;
        });
        return ret;
    }

    /**
     * ======== setCmdFieldValueByOpt ========
     * Set an RF command value by parameter option name. This function is
     * used for applying values that are mapped via a option list,
     * e.g txPower, whitening, syncWordLength.
     *
     * @param paramName - parameter name (e.g. whitening)
     * @param fieldName - RF command field name (e.g. formatConf.whitenMode)
     * @param optValue - option value (e.g. "none")
     */
    function setCmdFieldValueByOpt(paramName, fieldName, optValue) {
        const opts = Common.getOptions(DeviceConfig.configs, paramName);
        let parameterName = paramName;

        if (paramName.includes("txPower")) {
            parameterName = "txPower";
        }
        _.each(opts, (opt) => {
            if (opt.name === optValue) {
                setCmdFieldValue(parameterName, fieldName, opt.key);
            }
        });
    }

    /**
     *  ======== getCommandDescription ========
     *  Get the RF Command description.
     *  This is a longer description of the RF Command.
     *  Example: CMD_FS has the description "Frequency Synthesizer Programming Command"
     *
     *  @param name - RF Command (CMD_FS, CMD_PROP_RADIO_DIV_SETUP ...)
     */
    function getCommandDescription(name) {
        const cmd = getCommandDefByName(name);

        return cmd.desc;
    }

    // Return the module's exported functions
    return {
        getCommands: () => PhyDefCommands,
        getCommandBuffer: () => CmdBuf,
        getCommandName: (cmd) => cmd.name,
        getCmdList: getCmdList,
        getCommandDescription: getCommandDescription,
        getCommandMap: getCommandMap,
        generateRfCmdCodeRCL: generateRfCmdCodeRCL,
        generateRfCmdCodeRFD: generateRfCmdCodeRFD,
        getUsedCommands: getUsedCommands,

        // Support functions
        setCmdFieldValueByOpt: setCmdFieldValueByOpt,
        getCmdFieldValueByOpt: getCmdFieldValueByOpt,
        getCommandField: getCommandFieldValue,
        setCmdFieldValue: setCmdFieldValue,
        setCmdFieldValueDirect: setCmdFieldValueDirect,
        getCmdFieldByName: getCmdFieldByName,
        getCmdFieldsByName: getCmdFieldsByName,
        cmdIsUsed: (cmd) => CmdUsed.includes(cmd)
    };
}
