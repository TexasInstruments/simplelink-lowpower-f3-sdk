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
 *  ======== ble.component.js ========
 */

const deviceId = system.deviceData.deviceId;
const displayName = "BLE";
const categoryName = "RF Stacks";
let topModules;

if(deviceId.match(/CC2340|CC27../))
{
    topModules = [
        {
            displayName: categoryName,
            description: categoryName,
            modules: ["/ti/ble/ble"]
        }
    ];
}

const templates = [
    {
        name: "/ti/ble/templates/ble_config.h.xdt",
        outputPath: "ti_ble_config.h"
    },
    {
        name: "/ti/ble/templates/ble_config.c.xdt",
        outputPath: "ti_ble_config.c"
    },
    {
        name: "/ti/ble/templates/ble_gatt_service.h.xdt",
        outputPath: "ti_ble_gatt_service.h"
    },
    {
        name: "/ti/ble/templates/ble_gatt_service.c.xdt",
        outputPath: "ti_ble_gatt_service.c"
    },
];

const views = [
    {
      "name": "ti/ble/templates/dynamicMarkdown.xdt",
      "displayName": "Gatt Builder Dynamic Markdown",
      "viewType": "markdown"
    },
];

exports = {
    displayName: displayName,
    topModules: topModules,
    templates: templates,
    views: views
};
