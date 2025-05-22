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

const LogSinkBuf        = scripting.addModule("/ti/log/LogSinkBuf", {}, false);
const LogSinkBuf1       = LogSinkBuf.addInstance({}, false);
LogSinkBuf1.$name       = "CONFIG_ti_log_LogSinkBuf_0";

RCL.LogModule.logger                = LogSinkBuf1;
ZIGBEE.LogModuleZigbeeLLMAC.logger  = LogSinkBuf1;
ZIGBEE.LogModuleZigbeeApp.logger    = LogSinkBuf1;
ZIGBEE.LogModuleZigbeeOSIF.logger    = LogSinkBuf1;

ZIGBEE.LogModuleZigbeeApp.enable_DEBUG = true;
ZIGBEE.LogModuleZigbeeApp.enable_INFO = true;
ZIGBEE.LogModuleZigbeeApp.enable_VERBOSE = true;

ZIGBEE.LogModuleZigbeeLLMAC.enable_DEBUG = true;
ZIGBEE.LogModuleZigbeeLLMAC.enable_INFO = true;
ZIGBEE.LogModuleZigbeeLLMAC.enable_VERBOSE = true;

// Many calls in the OSIF so we don't enable all by default
ZIGBEE.LogModuleZigbeeOSIF.enable_DEBUG = false;
ZIGBEE.LogModuleZigbeeOSIF.enable_INFO = false;
ZIGBEE.LogModuleZigbeeOSIF.enable_VERBOSE = false;

/* ======= Advanced exception-handling if FreeRTOS is enabled ====== */
if (system.getRTOS() === "freertos")
{
    const exception = scripting.addModule("/freertos/exception");
    exception.nmiHandler       = "Exception_handlerMax";
    exception.hardFaultHandler = "Exception_handlerMax";
    exception.defaultHandler   = "Exception_handlerMax";
    exception.LogModule.logger     = LogSinkBuf1;
}
