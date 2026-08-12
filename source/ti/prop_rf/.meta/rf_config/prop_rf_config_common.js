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
 *  ======== prop_rf_config_common.js ========
 */

"use strict";

/*
 *  ======== Common Proprietary PHY Settings ========
 *
 * These Objects contain common proprietary phy settings for all boards/devices
 */

const commonMSK250KbpsSettings = {
    args: {
        $name: "RF_MSK_250_KBPS_Setting",
        codeExportConfig: {
            symGenMethod: "Custom",
            cmdList_prop: ["cmdGenericRx","cmdGenericTx"],
            cmdGenericTx: "txCmd",
            cmdGenericRx: "rxCmd",
        }
    }
};

const commonMSK250KbpsFecSettings = {
    args: {
        $name: "RF_MSK_250_KBPS_FEC_Setting",
        codeExportConfig: {
            symGenMethod: "Custom",
            cmdList_prop: ["cmdGenericRx","cmdGenericTx"],
            cmdGenericTx: "txCmd",
            cmdGenericRx: "rxCmd",
        }
    }
};

/*
 *  ======== Common BLE PHY Settings ========
 *
 * Array containing all the ble phy settings for a given board/device.
 */

// Object containing BLE settings for the all devices
const commonBleSettings = {
    args: {
        $name: "RF_BLE_Setting",
        phyType: "ble_gen",
        codeExportConfig: {
            symGenMethod: "Custom",
            cmdList_ble: ["cmdGenericRx","cmdGenericTx"],
            cmdGenericTx: "txCmd",
            cmdGenericRx: "rxCmd",
        }
    }
};

/*!
 *  ======== arrayMerge ========
 *  Helper function for merging two Arrays when using _.mergeWith
 *
 *  @param objValue - new property to be merged
 *  @param srcValue - property of the original object
 *  @returns Array - If objValue is an array, concat of objValue and srcValue
 *  @returns undefined - If objValue is not an array
 */
function arrayMerge(objValue, srcValue)
{
    let concatArray; // undefined array

    if(_.isArray(objValue))
    {
        // concatArray = _.union(objValue, srcValue);
        concatArray = objValue.concat(srcValue);
    }

    return(concatArray);
}

/*!
 *  ======== mergeRfSettings ========
 * Helper function for merging two rf settings objects into a single object
 * with correct ordering. Ordering of the arguments matters. Source objects are
 * applied from left to right. Subsequent sources overwrite property assignments
 * of previous sources.
 *
 * @param object1 - first rf setting object to be merged. Must contain an args
 * and codeExportConfig property
 * @param object2 - second rf setting object to be merged. Must contain an args
 * and codeExportConfig property
 *
 * @returns Object - An object containing all the properties from object1 and
 * object2 with the correct ordering required for the Radio Configuration module
 */
function mergeRfSettings(obj1, obj2)
{
    // Possible cmdList_ and phyTypeXXX properties in the input objects
    const cmdLists = ["cmdList_prop", "cmdList_ble"];
    const phyTypes = ["phyType2400"];

    // Object to contain the newly merged properties
    const mergedObj = {args: {}};

    // If there is a freqBand property set it as the first property of args
    if(_.has(obj1.args, "freqBand") || _.has(obj2.args, "freqBand"))
    {
        // Set to dummy value to specify ordering, overwritten by _.mergeWith
        mergedObj.args.freqBand = "dummy";
    }

    // If there is a phyType property set it as the second property of args
    let phyType = null;
    for(phyType of phyTypes)
    {
        if(_.has(obj1.args, phyType) || _.has(obj2.args, phyType))
        {
            // Set to dummy value to specify order, overwritten by _.mergeWith
            mergedObj.args[phyType] = "dummy";
            break;
        }
    }

    // If a symGenMethod property set as first property of codeExportConfig
    mergedObj.args.codeExportConfig = {};
    if(_.has(obj1.args.codeExportConfig, "symGenMethod")
        || _.has(obj2.args.codeExportConfig, "symGenMethod"))
    {
        // Set to dummy value to specify ordering, overwritten by _.mergeWith
        mergedObj.args.codeExportConfig.symGenMethod = "dummy";
    }

    // If a cmdList_ property set as second property of codeExportConfig
    let list = null;
    for(list of cmdLists)
    {
        if(_.has(obj1.args.codeExportConfig, list)
            || _.has(obj2.args.codeExportConfig, list))
        {
            // Set to dummy value to specify order, overwritten by _.mergeWith
            mergedObj.args.codeExportConfig[list] = [];
            break;
        }
    }

    // Merge args.codeExportConfig property of new object, obj1, and obj2 using
    _.mergeWith(mergedObj, obj1, obj2, arrayMerge);

    return(mergedObj);
}

exports = {
    commonMSK250KbpsSettings: commonMSK250KbpsSettings,
    commonMSK250KbpsFecSettings: commonMSK250KbpsFecSettings,
    commonBleSettings: commonBleSettings,
    mergeRfSettings: mergeRfSettings
};
