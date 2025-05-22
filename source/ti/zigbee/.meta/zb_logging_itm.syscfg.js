/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
 */

/* This file can be loaded in .syscfg files programmatically in order to load
 * Logging settings for the RCL/Zigbee MAC
 */

const RCL      = scripting.addModule("/ti/drivers/RCL");
const ZIGBEE   = scripting.addModule("/ti/zigbee/zigbee");
ZIGBEE.loggingEnabled = true;
RCL.loggingEnabled = true;

RCL.LogModule.enable_DEBUG = false;
RCL.LogModule.enable_INFO = false;
RCL.LogModule.enable_VERBOSE = false;

ZIGBEE.LogModuleZigbeeLLMAC.enable_DEBUG = true;
ZIGBEE.LogModuleZigbeeLLMAC.enable_INFO = true;
ZIGBEE.LogModuleZigbeeLLMAC.enable_VERBOSE = true;

ZIGBEE.LogModuleZigbeeApp.enable_DEBUG = true;
ZIGBEE.LogModuleZigbeeApp.enable_INFO = true;
ZIGBEE.LogModuleZigbeeApp.enable_VERBOSE = true;

ZIGBEE.LogModuleZigbeeOSIF.enable_DEBUG = false;
ZIGBEE.LogModuleZigbeeOSIF.enable_INFO = false;
ZIGBEE.LogModuleZigbeeOSIF.enable_VERBOSE = false;

RCL.LogModule.loggerSink               = "/ti/log/LogSinkITM";
ZIGBEE.LogModuleZigbeeLLMAC.loggerSink = "/ti/log/LogSinkITM";
ZIGBEE.LogModuleZigbeeApp.loggerSink   = "/ti/log/LogSinkITM";
ZIGBEE.LogModuleZigbeeOSIF.loggerSink   = "/ti/log/LogSinkITM";

const LogSinkITM           = scripting.addModule("/ti/log/LogSinkITM", {}, false);
const LogSinkITM1          = LogSinkITM.addInstance({}, false);
LogSinkITM1.$name          = "CONFIG_ti_log_LogSinkITM_0";

/* ======= Advanced exception-handling if FreeRTOS is enabled ====== */
if (system.getRTOS() === "freertos")
{
    const exception = scripting.addModule("/freertos/exception");
    exception.nmiHandler       = "Exception_handlerMax";
    exception.hardFaultHandler = "Exception_handlerMax";
    exception.defaultHandler   = "Exception_handlerMax";
    exception.LogModule.loggerSink = "/ti/log/LogSinkITM";
    exception.LogModule.logger     = LogSinkITM1;
}

RCL.LogModule.logger               = LogSinkITM1;
ZIGBEE.LogModuleZigbeeLLMAC.logger = LogSinkITM1;
ZIGBEE.LogModuleZigbeeApp.logger   = LogSinkITM1;
ZIGBEE.LogModuleZigbeeOSIF.logger  = LogSinkITM1;

/* ======== ITM ======== */
var board = system.deviceData.board.name;
var ITM = scripting.addModule("/ti/drivers/ITM");
/* Set baud rate (default: 12 Mbaud) */
ITM.baudRate = 12000000;

if (board.match(/CC13../) || board.match(/CC26../)) {
    if (board.match(/CC13.4/) || board.match(/CC26.[34]/)) {
        /* CC13X4, CC26X4 and CC26X3 requires an active debug session.
         * During an active debug session the TDO aux pin is used for the aux
         * COM port. DIO16 is the TDO pin.
         */
        ITM.swoPin.$assign  = "DIO16";
    } else {
        /* For other devices, the README file instructs users to end the debug
         * session. Which will cause the XDS to use the SWO signal for the aux
         * COM port. DIO18 is connected to the SWO signal.
         */
        ITM.swoPin.$assign  = "DIO18";
    }
}
else if(board.match(/CC27../)) {

    ITM.swoPin.swoResource.$assign = "DIO11";
    scripting.suppress("Connected to hardware*", ITM.swoPin, "swoResource");
}

