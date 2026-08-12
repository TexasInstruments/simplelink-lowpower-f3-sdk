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
 *  ======== CC1352R1_LAUNCHXL_rf_defaults.js ========
 */

"use strict";

// Get common rf settings
const rfCommon = system.getScript("/ti/prop_rf/rf_config/prop_rf_config_common.js");

/*
 *  ======== Device Specific Proprietary PHY Settings ========
 *
 * These Objects will be used to extend the common proprietary phy settings
 * defined in prop_rf_config_common.js.
 */

// Object containing MSK 250Kbps settings
const devSpecificMSK250KbpsSettings = {
    args: {
        freqBand: "2400",
        phyType2400: "msk_250_kbps", // phyType suffix must match freqBand
    }
};

// Object containing MSK 250Kbps FEC settings
const devSpecificMSK250KbpsFecSettings = {
    args: {
        freqBand: "2400",
        phyType2400: "msk_250_kbps_fec", // phyType suffix must match freqBand
    }
};

/*
 *  ======== Device Specific BLE PHY Settings ========
 *
 * These Objects will be used to extend the common BLE phy settings defined in
 * prop_rf_config_common.js.
 */

// Object containing BLE settings
const devSpecificBleSettings = {
    args: {
        codeExportConfig: {}
    }
};

/*
 *  ======== Arrays Containing all PHY Settings ========
 *
 * These Arrays will pull the common phy settings defined in
 * prop_rf_config_common.js and merge them with the device specific phy
 * settings defined in this file.
 *
 * Note: The first element in the defaultPropPhyList with easyLinkOption defined
 *       will be the phy that EasyLink_Phy_Custom defaults to when added to a
 *       configuration
 */

// Array containing all the proprietary phy settings for the CC1352R1_LAUNCHXL
const defaultPropPhyList = [
    rfCommon.mergeRfSettings(devSpecificMSK250KbpsSettings, rfCommon.commonMSK250KbpsSettings),
    rfCommon.mergeRfSettings(devSpecificMSK250KbpsFecSettings, rfCommon.commonMSK250KbpsFecSettings),
];

// Array containing all the BLE phy settings for the CC1352R1_LAUNCHXL
const defaultBlePhyList = [
    rfCommon.mergeRfSettings(devSpecificBleSettings, rfCommon.commonBleSettings)
];


exports = {
    defaultPropPhyList: defaultPropPhyList,
    defaultBlePhyList: defaultBlePhyList,
};
