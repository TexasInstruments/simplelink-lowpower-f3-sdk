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
const LogSinkTraceLPF3  = scripting.addModule("/ti/log/LogSinkTraceLPF3", {}, false);
const LogSinkTraceLPF31 = LogSinkTraceLPF3.addInstance();

ZIGBEE.loggingEnabled = true;
RCL.loggingEnabled = true;

RCL.LogModule.loggerSink               = "/ti/log/LogSinkTraceLPF3";
ZIGBEE.LogModuleZigbeeLLMAC.loggerSink = "/ti/log/LogSinkTraceLPF3";
ZIGBEE.LogModuleZigbeeApp.loggerSink   = "/ti/log/LogSinkTraceLPF3";

LogSinkTraceLPF31.$name            = "CONFIG_ti_log_LogSinkTraceLPF3_0";

LogSinkTraceLPF31.rfTraceMode      = "LogSinkTraceLPF3_CONFIG_CHANNELS_CPU_ONLY";

ZIGBEE.LogModuleZigbeeLLMAC.logger = LogSinkTraceLPF31;
ZIGBEE.LogModuleZigbeeApp.logger   = LogSinkTraceLPF31;
RCL.LogModule.logger               = LogSinkTraceLPF31;

ZIGBEE.LogModuleZigbeeLLMAC.enable_DEBUG = true;
ZIGBEE.LogModuleZigbeeLLMAC.enable_INFO = true;
ZIGBEE.LogModuleZigbeeLLMAC.enable_VERBOSE = true;

ZIGBEE.LogModuleZigbeeApp.enable_DEBUG = true;
ZIGBEE.LogModuleZigbeeApp.enable_INFO = true;
ZIGBEE.LogModuleZigbeeApp.enable_VERBOSE = true;