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
 *  ======== advertisement_params.syscfg.js ========
 */

"use strict";
// Get advertisement_params long descriptions
const Docs = system.getScript("/ti/ble/broadcaster/advertisement_params_docs.js");

// Get common utility functions
const Common = system.getScript("/ti/ble/ble_common.js");

const config = [
    {
        name: "name",
        displayName: "Name",
        default: ""
    },
    {
        name: "advType",
        displayName: "Advertisement Type",
        default: "legacy",
        longDescription: Docs.advTypeLongDescription,
        getDisabledOptions: generateDisabledOptions("advType"),
        onChange: onAdvTypeChange,
        options: [
            { displayName: "Legacy",    name: "legacy"   },
            { displayName: "Extended",  name: "extended" }
        ]
    },
    {
        name: "legacyEvnPropOptions",
        displayName: "Legacy Event Properties Options",
        default: "CONN_SCAN",
        getDisabledOptions: generateDisabledOptions("legacyEvnPropOptions"),
        longDescription: Docs.legacyEvnPropOptionsLongDescription,
        onChange: onLegacyEvnPropOptionsChange,
        hidden: false,
        options: [
            { displayName: "Connectable and scannable undirected",         name: "CONN_SCAN" },
            { displayName: "Connectable directed",                         name: "CONN_DIR" },
            { displayName: "High Duty Cycle Connectable directed",         name: "HDC_CONN_DIR"},
            { displayName: "Scannable undirected",                         name: "SCAN" },
            { displayName: "Non-connectable and Non-scannable undirected", name: "NC_NS"}
        ]
    },
    {
        name: "eventProps",
        displayName: "Event Properties",
        description: "Advertising Event Param Properties",
        getDisabledOptions: generateDisabledOptions("eventProps"),
        longDescription: Docs.eventPropsLongDescription,
        default: ["GAP_ADV_PROP_CONNECTABLE", "GAP_ADV_PROP_SCANNABLE", "GAP_ADV_PROP_LEGACY"],
		minSelections: 0,
        options: [
            {
                displayName: "Connectable advertising",
                name: "GAP_ADV_PROP_CONNECTABLE"
            },
            {
                displayName: "Scannable advertising",
                name: "GAP_ADV_PROP_SCANNABLE"
            },
            {
                displayName: "Directed advertising",
                name: "GAP_ADV_PROP_DIRECTED"
            },
            {
                displayName: "High Duty Cycle Directed Advertising",
                name: "GAP_ADV_PROP_HDC"
            },
            {
                displayName: "Legacy advertising PDU's",
                name: "GAP_ADV_PROP_LEGACY"
            },
            {
                displayName: "Omit advertiser's address from PDU's (anonymous advertising)",
                name: "GAP_ADV_PROP_ANONYMOUS"
            },
            {
                displayName: "Include TxPower in the extended header of the advertising PDU",
                name: "GAP_ADV_PROP_TX_POWER"
            }
        ]
    },
    {
        name: "primIntMin",
        displayName: "Primary PHY Interval Minimum (ms)",
        longDescription: Docs.primIntMinLongDescription,
        default: 100
    },
    {
        name: "primIntMax",
        displayName: "Primary PHY Interval Maximum (ms)",
        longDescription: Docs.primIntMaxLongDescription,
        default: 100
    },
    {
        name: "primChanMap",
        displayName: "Primary Channel Map",
        longDescription: Docs.primChanMapLongDescription,
        default: ["GAP_ADV_CHAN_37", "GAP_ADV_CHAN_38", "GAP_ADV_CHAN_39"],
        options: [
            { displayName: "Channel 37",    name: "GAP_ADV_CHAN_37"  },
            { displayName: "Channel 38",    name: "GAP_ADV_CHAN_38"  },
            { displayName: "Channel 39",    name: "GAP_ADV_CHAN_39"  }
        ]
    },
    {
        name: "peerAddrType",
        displayName: "Peer Address Type",
        longDescription: Docs.peerAddrTypeLongDescription,
        default: "PEER_ADDRTYPE_PUBLIC_OR_PUBLIC_ID",
        options: [
            {
                displayName: "Public Or Public ID",
                name: "PEER_ADDRTYPE_PUBLIC_OR_PUBLIC_ID",
                description: "Public Device Address or Public Identity Address"
            },
            {
                displayName: "Random or Random ID",
                name: "PEER_ADDRTYPE_RANDOM_OR_RANDOM_ID",
                description: "Random Device Address or Random (static) Identity Address"
            }
        ]
    },
    {
        name: "peerAddr",
        displayName: "Peer Address",
        longDescription: Docs.peerAddrLongDescription,
        description: "Public Device Address, Random Device Address, Public Identity Address, "
                      + "or Random (static) Identity Address of the device to be connected",
        default: "aa:aa:aa:aa:aa:aa",
        placeholder: "00:00:00:00:00:00",
        textType: "mac_address_48"
    },
    {
        name: "filterPolicy",
        displayName: "Filter Policy",
        longDescription: Docs.filterPolicyLongDescription,
        default: "GAP_ADV_AL_POLICY_ANY_REQ",
        options: [
            {
                displayName: "Process requests from all devices",
                name: "GAP_ADV_AL_POLICY_ANY_REQ",
                legacyNames: ["GAP_ADV_WL_POLICY_ANY_REQ"],
                description: "Process scan and connection requests from all devices "
                              + "(i.e., the AcceptList is not in use)"
            },
            {
                displayName: "Process conn req from all devices and only scan req from devices in AL",
                name: "GAP_ADV_AL_POLICY_AL_SCAN_REQ",
                legacyNames: ["GAP_ADV_WL_POLICY_WL_SCAN_REQ"],
                description:  "Process connection requests from all devices and only scan requests "
                            +"from devices that are in the Accept List"
            },
            {
                displayName: "Process scan req from all devices and only conn req from devices in AL",
                name: "GAP_ADV_AL_POLICY_AL_CONNECT_IND",
                legacyNames: ["GAP_ADV_AL_POLICY_WL_CONNECT_IND"],
                description: "Process scan requests from all devices and only connection requests"
                              + "from devices that are in the Accept List"
            },
            {
                displayName: "Process requests only from devices in AL",
                name: "GAP_ADV_AL_POLICY_AL_ALL_REQ",
                legacyNames: ["GAP_ADV_AL_POLICY_WL_ALL_REQ"],
                description: "Process scan and connection requests only from devices in the Accept List"
            }
        ]
    },
    {
        name: "txPower",
        displayName: "TX Power",
        longDescription: Docs.txPowerLongDescription,
        default: "GAP_ADV_TX_POWER_NO_PREFERENCE",
        onChange: onTxPowerChange,
        options: [
            {
                displayName: "The Controller will choose the Tx power",
                name: "GAP_ADV_TX_POWER_NO_PREFERENCE"
            },
            {
                displayName: "TX Power Value",
                name: "otherTxPower"
            }
        ]
    },
    {
        name: "txPowerValue",
        displayName: "TX Power Value",
        longDescription: Docs.txPowerValueLongDescription,
        description: "The TX Power indicates the maximum level at which the advertising "
                        + "packets are to be transmitted on the advertising channels",
        default: 0,
        hidden: true
    },
    {
        name: "primPhy",
        displayName: "Primary PHY",
        getDisabledOptions: generateDisabledOptions("primPhy"),
        longDescription: Docs.primPhyLongDescription,
        default: "GAP_ADV_PRIM_PHY_1_MBPS",
        options: [
            { displayName: "1M",        name: "GAP_ADV_PRIM_PHY_1_MBPS"   },
            { displayName: "Coded S8",  name: "GAP_ADV_PRIM_PHY_CODED_S8" },
            { displayName: "Coded S2",  name: "GAP_ADV_PRIM_PHY_CODED_S2" }
        ]
    },
    {
        name: "secPhy",
        displayName: "Secondary PHY",
        longDescription: Docs.secPhyLongDescription,
        getDisabledOptions: generateDisabledOptions("secPhy"),
        default: "GAP_ADV_SEC_PHY_1_MBPS",
        options: [
            { displayName: "1M",        name: "GAP_ADV_SEC_PHY_1_MBPS"   },
            { displayName: "2M",        name: "GAP_ADV_SEC_PHY_2_MBPS"   },
            { displayName: "Coded S8",  name: "GAP_ADV_SEC_PHY_CODED_S8" },
            { displayName: "Coded S2",  name: "GAP_ADV_SEC_PHY_CODED_S2" }
        ]
    },
    {
        name: "sid",
        displayName: "Set ID",
        longDescription: Docs.sidLongDescription,
        displayFormat: "dec",
        default: 0
    },
    {
        name: "deviceRole",
        default: "",
        onChange: onDeviceRoleChange,
        hidden: true
    },
    {
        name: "extAdv",
        default: false,
        hidden: true
    }
];

// Legacy valid eventProp options
const legacyEventPropValidOpt = {
    CONN_SCAN_LEG:      ["GAP_ADV_PROP_CONNECTABLE", "GAP_ADV_PROP_SCANNABLE",
                         "GAP_ADV_PROP_LEGACY"],
    CONN_DIR_LEG:       ["GAP_ADV_PROP_CONNECTABLE", "GAP_ADV_PROP_DIRECTED",
                         "GAP_ADV_PROP_LEGACY"],
    HDC_CONN_DIR_LEG:   ["GAP_ADV_PROP_CONNECTABLE", "GAP_ADV_PROP_DIRECTED",
                         "GAP_ADV_PROP_HDC", "GAP_ADV_PROP_LEGACY"],
    SCAN_LEG:           ["GAP_ADV_PROP_SCANNABLE", "GAP_ADV_PROP_LEGACY"],
    NC_NS_LEG:          ["GAP_ADV_PROP_LEGACY"]
  };

// Extended invalid eventProp options
const extEventPropInvalidOpt = {
    CONN_SCAN: ["GAP_ADV_PROP_CONNECTABLE", "GAP_ADV_PROP_SCANNABLE"],
    CONN_ANON: ["GAP_ADV_PROP_CONNECTABLE", "GAP_ADV_PROP_ANONYMOUS"],
    SCAN_ANON: ["GAP_ADV_PROP_SCANNABLE", "GAP_ADV_PROP_ANONYMOUS"],
    HDC:       ["GAP_ADV_PROP_HDC"],
    LEG:       ["GAP_ADV_PROP_LEGACY"]
  };

/*
 * ======== validate ========
 * Validate this inst's configuration
 *
 * @param inst       - Advertisement Parameters instance to be validated
 * @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    //Validate the name
    if(inst.name.includes(" "))
    {
        validation.logError("The name should not contain spaces", inst, "name");
    }

    if(inst.txPowerValue < Common.advParamsRanges.advParamTXPowerMinValue
         || inst.txPowerValue > Common.advParamsRanges.advParamTXPowerMaxValue)
    {
        validation.logError("TX Power range is " + Common.advParamsRanges.advParamTXPowerMinValue + " to "
        + Common.advParamsRanges.advParamTXPowerMaxValue, inst, "txPowerValue");
    }
    if(inst.sid < Common.advParamsRanges.advSIDMinValue || inst.sid > Common.advParamsRanges.advSIDMaxValue)
    {
        validation.logError("Set ID range is " + Common.advParamsRanges.advSIDMinValue + " to "
                             + Common.advParamsRanges.advSIDMaxValue, inst, "sid");
    }
    // Validate connection interval
    Common.validateAdvInterval(inst, validation, inst.primIntMin, "primIntMin", inst.primIntMax, "primIntMax");

    // Validate eventProps selection
    if((inst.advType == "extended" &&
        // if true exist log error
        checkEvnPropSel(extEventPropInvalidOpt, inst.eventProps)) ||
        (inst.advType == "legacy" &&
        // if a valid option is selected // if true exist dont log error
        !checkEvnPropSel(legacyEventPropValidOpt, inst.eventProps)))
    {
        validation.logError("An invalid combination of event properties was selected", inst, "eventProps");
    }

    // Validate advType
    // If extended advertising is disabled, log an error if the user has selected extended advertising type
    if(inst.extAdv == false)
    {
        if(inst.advType == "extended")
        {
            validation.logError("Extended Advertising feature is disabled", inst, "advType");
        }
    }
}

/*
 *  ======== checkEvnPropSel ========
 *  Check if a valid/invalid event properties has been selected.
 *
 *  @param optionsArray      - the "legacy" valid options array or
 *                             the "extended" invalid options array
 *  @param eventPropsList    - current eventProp
 *
 *  @returns true - if "extended" invalid option was selected or
 *                  "legacy" valid option was selected
 */
function checkEvnPropSel(optionsArray, eventPropsList)
{
    return _.includes(_.map(optionsArray, function(option)
           {return _.intersection(option, eventPropsList).length == option.length}), true);
}

/*
 *  ======== generateDisabledOptions ========
 *  Generates a list of options that should be disabled in a
 *  drop-down when "advType" is Legacy
 *
 * @returns Array - array of strings that should be disabled
 */
function generateDisabledOptions(name)
{
	return (inst) => {

        // Find the configurable we're going to generate a disabled list from
        const configurable = _.find(inst.$module.config,(conf) => conf.name == name);

        if(name == "advType")
        {
            // List of invalid options
            let disabledOptions = [];

            // If extended advertising is disabled, add the extended option to the disabled list
            if(inst.extAdv == false)
            {
                disabledOptions = disabledOptions.concat({name: "extended", reason: "Not supported when the Extended Advertising feature is disabled"});
            }

            return disabledOptions;
        }

        // Hide the option to use PHY != 1M for Legacy adv params
        if(name == "primPhy" || name == "secPhy")
        {
            // Don't mark anything disabled
            if (inst.advType == "extended") {
                return [];
            }

            // List of invalid options
            const disabledOptions = configurable.options.slice(configurable.options.includes("1_MBPS") == false);

            // Add the "reason" why it's disabled, and return that information
            return disabledOptions.map((option) => ({ name: option.name, reason: "This is not a valid option for Legacy Advertisement Type" }));
        }

        if(name == "eventProps")
        {
            let disabledOptions = configurable.options;
            const devFamily = Common.device2DeviceFamily(system.deviceData.deviceId);

            if (inst.advType == "extended")
            {
                // List of invalid options
                disabledOptions = disabledOptions.filter(function(index){ return index.name.includes("HDC") == true || index.name.includes("LEGACY") == true});

                // Add the "reason" why it's disabled, and return that information
                disabledOptions = disabledOptions.map((option) => ({ name: option.name, reason: "This is not a valid option for Extended Advertisement Type" }));

                // If the device role is broadcaster, disable the connectable option
                if(inst.$ownedBy.$ownedBy.deviceRole.includes("BROADCASTER_CFG"))
                {
                    disabledOptions = disabledOptions.concat({name: "GAP_ADV_PROP_CONNECTABLE", reason: "Connectable can not be used for the Broadcaster role"});
                    if(inst.eventProps.includes("GAP_ADV_PROP_SCANNABLE"))
                    {
                        disabledOptions = disabledOptions.concat({name: "GAP_ADV_PROP_ANONYMOUS", reason: "Anonymous advertising is not supported when Scannable advertising is selected"});
                    }
                    if(inst.eventProps.includes("GAP_ADV_PROP_ANONYMOUS"))
                    {
                        disabledOptions = disabledOptions.concat({name: "GAP_ADV_PROP_SCANNABLE", reason: "Scannable advertising is not supported when Anonymous advertising is selected"});
                    }
                }
                else
                {
                    // Disable the option to choose both CONNECTABLE and SCANNABLE advertise properties
                    if(inst.eventProps.includes("GAP_ADV_PROP_CONNECTABLE"))
                    {
                        disabledOptions = disabledOptions.concat({name: "GAP_ADV_PROP_SCANNABLE", reason: "Connectable and Scannable can not be used in the same time"});
                        disabledOptions = disabledOptions.concat({name: "GAP_ADV_PROP_ANONYMOUS", reason: "Anonymous advertising is not supported when Connectable advertising is selected"});
                    }
                    else if(inst.eventProps.includes("GAP_ADV_PROP_SCANNABLE"))
                    {
                        disabledOptions = disabledOptions.concat({name: "GAP_ADV_PROP_CONNECTABLE", reason: "Connectable and Scannable can not be used in the same time"});
                        disabledOptions = disabledOptions.concat({name: "GAP_ADV_PROP_ANONYMOUS", reason: "Anonymous advertising is not supported when Scannable advertising is selected"});
                    }
                    else if(inst.eventProps.includes("GAP_ADV_PROP_ANONYMOUS"))
                    {
                        disabledOptions = disabledOptions.concat({name: "GAP_ADV_PROP_CONNECTABLE", reason: "Connectable advertising is not supported when Anonymous advertising is selected"});
                        disabledOptions = disabledOptions.concat({name: "GAP_ADV_PROP_SCANNABLE", reason: "Scannable advertising is not supported when Anonymous advertising is selected"});
                    }
                }
            }
            if (inst.advType == "legacy")
            {
                // Add the "reason" why it's disabled, and return that information
                disabledOptions = disabledOptions.map((option) => ({ name: option.name,
                    reason: "The Event Properties selection for Legacy advertisement is done by choosing an option from Legacy Event Properties Options" }));
            }
            return disabledOptions;
        }
        if(name == "legacyEvnPropOptions")
        {
            if(inst.$ownedBy.$ownedBy.deviceRole.includes("BROADCASTER_CFG"))
            {
                let disabledOptions = configurable.options;
                disabledOptions = disabledOptions.filter(function(index){ return index.name.includes("CONN") == true});
                // Add the "reason" why it's disabled, and return that information
                disabledOptions = disabledOptions.map((option) => ({ name: option.name,
                    reason: "Connectable can not be used in the Broadcaster role" }));
                return disabledOptions;
            }
            else
            {
                return [];
            }
        }
	}
}

/*
 *  ======== onAdvTypeChange ========
 * Changes the default values of PHY parameters when switching between
 * Legacy and Extended
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
 function onAdvTypeChange(inst,ui)
 {
    if(inst.advType == "legacy")
    {
		if(inst.deviceRole.includes("BROADCASTER_CFG"))
		{
			inst.legacyEvnPropOptions = "NC_NS";
		}
		else
		{
            inst.legacyEvnPropOptions = "CONN_SCAN";
		}
        inst.primPhy = "GAP_ADV_PRIM_PHY_1_MBPS";
        inst.secPhy = "GAP_ADV_SEC_PHY_1_MBPS";
        ui.legacyEvnPropOptions.hidden = false;
        onLegacyEvnPropOptionsChange(inst,ui);
    }
    else if(inst.advType == "extended")
    {
		if(inst.deviceRole.includes("BROADCASTER_CFG"))
		{
			inst.eventProps = [];
		}
		else
		{
			inst.eventProps = ["GAP_ADV_PROP_CONNECTABLE"];
		}
        inst.primPhy = "GAP_ADV_PRIM_PHY_CODED_S2";
        inst.secPhy = "GAP_ADV_SEC_PHY_CODED_S2";
        ui.legacyEvnPropOptions.hidden = true;
    }
 }

/*
 *  ======== onLegacyEvnPropOptionsChange ========
 * Changes the legacyEvnPropOptions configurable value
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onLegacyEvnPropOptionsChange(inst,ui)
{
    switch(inst.legacyEvnPropOptions) {
        case "CONN_SCAN":
            inst.eventProps = legacyEventPropValidOpt.CONN_SCAN_LEG;
            break;
        case "CONN_DIR":
            inst.eventProps = legacyEventPropValidOpt.CONN_DIR_LEG;
            break;
        case "HDC_CONN_DIR":
            inst.eventProps = legacyEventPropValidOpt.HDC_CONN_DIR_LEG;
            break;
        case "SCAN":
            inst.eventProps = legacyEventPropValidOpt.SCAN_LEG;
            break;
        case "NC_NS":
            inst.eventProps = legacyEventPropValidOpt.NC_NS_LEG;
            break;
        default:
            inst.eventProps = legacyEventPropValidOpt.CONN_SCAN_LEG;
      }
}

 /*
 *  ======== onDeviceRoleChange ========
 * Change the device role configurable
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onDeviceRoleChange(inst,ui)
{
    if(inst.deviceRole.includes("BROADCASTER_CFG"))
    {
        if(inst.advType == "legacy")
        {
            inst.legacyEvnPropOptions = "SCAN";
            inst.eventProps = legacyEventPropValidOpt.SCAN_LEG;
        }
        else if(inst.advType == "extended")
        {
            inst.eventProps = [];
        }
    }
    else if(inst.deviceRole.includes("PERIPHERAL_CFG"))
    {
        if(inst.advType == "legacy")
        {
            inst.legacyEvnPropOptions = "CONN_SCAN";
            inst.eventProps = legacyEventPropValidOpt.CONN_SCAN_LEG;
        }
    }
}

 /*
 *  ======== onTxPowerChange ========
 * Change the tx power input option
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onTxPowerChange(inst,ui)
{
    return inst.txPower == "otherTxPower" ? ui.txPowerValue.hidden = false:
                                            ui.txPowerValue.hidden = true;
}

/*
 *  ======== advParamModule ========
 *  Define the BLE Central Role Configuration module properties and methods
 */
const advParamModule = {
    displayName: "Advertisement Parameters",
    config: config,
    validate: validate,
    maxInstances: Common.maxNumAdvSets
};

/*
 *  ======== exports ========
 *  Export the Advertisement Parameters module
 */
exports = advParamModule;