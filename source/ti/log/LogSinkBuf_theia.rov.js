/*
 * Copyright (c) 2025-2026, Texas Instruments Incorporated - http://www.ti.com
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

/*
 *  ======== LoggerBuf_theia.rov.js ========
 */

"use strict";

Object.defineProperty(exports, "__esModule", { value: true });

const LOG_EVT_FORMAT_STR_OFFSET = 5;
const LOG_EVT_MODULE_OFFSET = 4;
const LOG_EVT_OFFSET = 3;
const NUM_BYTES_PER_WORD = 4;

const LoggerBuf_EVENT = 0;
const LoggerBuf_PRINTF = 1;
const LoggerBuf_BUFFER_START = 2;
const LoggerBuf_BUFFER_CONTINUED = 3;
const LoggerBuf_WordsPerRecord = 5;

class LogSinkBufInstance {
}

class LogSinkBufRecord {
}

class LogSinkBuf {
    constructor(ctx) {
        this.ctx = ctx;
        this.viewMap = [
            { name: 'Instance', fxn: this.getInstanceView.bind(this), structName: LogSinkBufInstance },
            { name: 'Records', fxn: this.getRecordView.bind(this), structName: LogSinkBufRecord }
        ];
        this.Program = this.ctx.getProgram();
    }
    getModuleName() {
        return 'LogSinkBuf';
    }

    async getInstanceView()
    {
        var table = new Array();
        var view = new LogSinkBufInstance();

        /* Look up all symbols in SRAM region to find all elements of type
         * LogSinkBuf_Instance. We currently cannot look up all section lengths
         * to search SRAM sections exactly. So we'll read out all symbols in a
         * large range that should cover possible log module locations.
         * lookupSymbols gracefully handles asking for symbols in non-existent
         * memory ranges.
         */
        let symbolLookupRange = {address: 0x20000000, length: 0x9000000};
        /* Returns an array of SymbolLookupResult objects in the specified range
         */
        let sramSymbols = await this.Program.task.symbols.lookupSymbols(symbolLookupRange);
        /* Filter the resulting list to get all instance elements with their
         * symbol names and addresses
         */
        let instanceSymbols = sramSymbols.symbols.filter((symbol) => symbol.name.match(/LogSinkBuf_.*_config/));

        for (var i = 0; i < instanceSymbols.length; i++) {
            view = new LogSinkBufInstance();
            var inst = await this.Program.fetchFromAddr(Number(instanceSymbols[i].location), "LogSinkBuf_Instance");
            view.Name = instanceSymbols[i].name.replace("LogSinkBuf_", "").replace("_config", "");
            view.Address = instanceSymbols[i].location;
            view.Serial = inst.serial;
            view.NumEntries = inst.numEntries;
            view.Type = (inst.bufType == 1 ? "Linear" : "Circular");
            table.push(view);
        }

        /* If no instances on target, render and headings only */
        if (table.length == 0) {
            view = new LogSinkBufInstance();
            view.Address = "";
            view.Serial = "";
            view.NumEntries = "";
            view.Type = "";
            table.push(view);
        }

        return table;
    }

    async getRecordView()
    {
        /* Look up all symbols in SRAM region to find all elements of type
         * LogSinkBuf_Rec.
         */
        var symbolLookupRange = {address: 0x20000000, length: 0x9000000};
        /* Returns an array of SymbolLookupResult objects in the specified range
         */
        var sramSymbols = await this.Program.task.symbols.lookupSymbols(symbolLookupRange);
        /* Filter the resulting list to get all instance elements with their
         * symbol names and addresses. The Log macros prepend and append the
         * module name and config to ensure uniqueness.
         */
        var instanceSymbols = sramSymbols.symbols.filter((symbol) => symbol.name.match(/LogSinkBuf_.*_config/));

        var recSize = (await this.Program.lookupType("LogSinkBuf_Rec")).size;

        /* Table of records across all instances */
        var table = new Array();

        /* Make this.Program accessible to functions defined within this one */
        var Program = this.Program;

        /* Helper function to extract unsigned bytes from a uint32 */
        function toBytesFromUInt32 (num, numBytes) {
            var arr = new Array();

            if (numBytes >= 1 ) {
                arr.push(num & 0x000000ff);
            }
            if(numBytes >= 2 ) {
                arr.push((num & 0x0000ff00) >>> 8);
            }
            if(numBytes >= 3) {
                arr.push((num & 0x00ff0000) >>> 16);
            }
            if(numBytes >= 4) {
                arr.push((num & 0xff000000) >>> 24);
            }
            return arr;
        }

        /* Helper function to generate a uint32 from unsigned bytes */
        function toUInt32FromBytes (bytes, numBytes) {
            /* Always perform an unsigned right shift by 0 at the end of each
             * shift operation to guarantee that our result is unsigned at the
             * end
             */
            let num = 0 >>> 0;

            if (numBytes >= 1 ) {
                num += (bytes[0] << 0) >>> 0;
            }
            if(numBytes >= 2 ) {
                num += (bytes[1] << 8) >>> 0;
            }
            if(numBytes >= 3) {
                num += (bytes[2] << 16) >>> 0;
            }
            if(numBytes >= 4) {
                num += (bytes[3] << 24) >>> 0;
            }

            return num;
        }

        /* ======== readStringFromAddr ========
         * Reads a string from memory at address ptr at most maxLen
         * characters or until the first null character
         */
        async function readStringFromAddr(ptr, maxLen) {
            let logString = "";

            /* Assume that we are only loading one symbol file for now */
            let symbolFile = (await Program.task.symbols.getSymbolFiles()).symbolFiles[0];

            /* maxLen is here just in case there is no NULL terminator to find
             * so we do not spin forever or run off the end of the section
             */
            for (let i = 0; i < maxLen; i++) {
                let stringAddress = ptr + i - 0x90000000;
                /* Read one byte at a time since we will throw an error if we
                 * read past the end of a section.
                 */
                let outFileData = await Program.task.symbols.getSectionData(symbolFile,
                                                                            ".log_data",
                                                                            stringAddress,
                                                                            1);

                /* If we found the NULL terminator, break out of the loop */
                if (outFileData.bytes[0] == 0) {
                    break;
                }

                logString += String.fromCharCode(outFileData.bytes[0]);
            }
            return logString;
        }

        /*
         *  ======== rsprintf ========
         *  Restricted string print format
         *
         *  Scan format string and replace control characters with supplied
         *  argument. Only a limited set of control characters are supported:
         *
         *      %c - character
         *      %d - decimal
         *      %f - float
         *      %x - hexadecimal
         *
         *  Parameters
         *      fmt - format string
         *      args - string array of arguments
         */
        function rsprintf(fmt, args)
        {
            var regex = /%([cdfx%])/g;
            var i = -1;

            function reparg(match, p)
            {
                if (match == '%%') {
                    return ('%');
                }
                if (args[++i] === undefined) {
                    return undefined;
                }
                var val;
                switch (p) {
                    case 'c': val = String.fromCharCode(args[i]); break;
                    case 'd': val = parseInt(args[i]); break;
                    case 'f': val = parseFloat(args[i]); break;
                    case 'x': val = '0x' + parseInt(args[i]).toString(16); break;
                }
            return val;
            }

            return (fmt.replace(regex, reparg));
        }

        function addEventToTable(rec, fmt, mdata, recView)
        {
            var args = new Array();
            args.push(rec.data[1]);
            args.push(rec.data[2]);
            args.push(rec.data[3]);
            args.push(rec.data[4]);

            recView.Module = mdata[LOG_EVT_MODULE_OFFSET];
            recView.Type = "EVENT";
            recView.SubType = mdata[LOG_EVT_OFFSET];
            recView.Message = rsprintf(fmt, args);
            recView.Data = ("").concat("Args: ", args.toString());
            table.push(recView);
        }

        function addPrintfToTable(rec, fmt, mdata, recView)
        {
            var args = new Array();
            args.push(rec.data[1]);
            args.push(rec.data[2]);
            args.push(rec.data[3]);
            args.push(rec.data[4]);

            recView.Module = mdata[LOG_EVT_MODULE_OFFSET];
            recView.Type = "FORMATTED_TEXT";
            recView.Serial = rec.serial;
            recView.Message = rsprintf(fmt, args);

            recView.Data = ("").concat("Args: ", args.toString());
            table.push(recView);
        }

        function addBufToTable(rec, fmt, mdata, recView)
        {
            /* This function is stateful and assumes that buf records are
             * placed in the buffer contiguously and are not interleaved.
             * This is an area that has been identified for improvement.
             */
            if( typeof addBufToTable.numWords == 'undefined' ||
                typeof addBufToTable.remainder == 'undefined') {
                addBufToTable.numWords = 0;
                addBufToTable.remainder = 0;
            }

            if(rec.type == LoggerBuf_BUFFER_START) {
                /* The first record is purely informational, use it to set state */
                recView.Message = fmt;
                recView.Module = mdata[LOG_EVT_MODULE_OFFSET];
                recView.Type = "BUFFER_START";

                var size = rec.data[1];
                recView.Data = ("").concat("size: ", size);
                addBufToTable.numWords = Math.floor(size/NUM_BYTES_PER_WORD);
                addBufToTable.remainder = size % NUM_BYTES_PER_WORD;
            }
            else if (rec.type == LoggerBuf_BUFFER_CONTINUED) {
                recView.Type = "BUFFER_CONTINUED";

                /* Protect against data loss from wraparound in circular buf */
                if ((addBufToTable.numWords == -1) && (addBufToTable.remainder == -1)) {
                    recView.Message = "Found continue without start the rest " +
                                        "of the buffer will not be parsed";
                    return;
                }

                /* The next record stores the user data */
                var outputArray = new Array();

                /* Target memory is arranged as uint32_t array, reformat this
                 * to an array of bytes, ignoring any padding entries
                 */
                for (var i = 0;
                        (i < LoggerBuf_WordsPerRecord)
                        && (addBufToTable.numWords >= 0);
                        i++, addBufToTable.numWords--) {

                    if(addBufToTable.numWords == 0) {
                        outputArray.push(toBytesFromUInt32(rec.data[i], addBufToTable.remainder));
                        /* Reset the control variables */
                        addBufToTable.numWords = -1;
                        addBufToTable.remainder = -1;
                    }
                    else {
                        outputArray.push(toBytesFromUInt32(rec.data[i], NUM_BYTES_PER_WORD));
                    }
                }
                recView.Data = outputArray.toString();
            }
            table.push(recView);
        }

        /* Add a new entry to table for given log record */
        async function makeRecord(addr, instanceName)
        {

            /* Read a record from the buffer */
            var rec = await Program.fetchFromAddr(addr, "LogSinkBuf_Rec");
            /* Determine what type of log statement the record originated from */
            var recType = Number(rec.type);

            var dataAddr;
            var ldata;
            var mdata;

            /* Assume that we are only loading one symbol file for now */
            var symbolFile = (await Program.task.symbols.getSymbolFiles()).symbolFiles[0];

            /* Events are also transported with printf - if the type is PRINTF,
             * check if the magic string indicates an event
             */
            if(recType == LoggerBuf_PRINTF)
            {
                /* Get the pointer to the out file string from the out file
                 * pointer table in the out file
                 */
                let ptrOffset = Number(rec.data[0]) - 0x94000008;

                let dataAddrBytes = await Program.task.symbols.getSectionData(symbolFile, ".log_ptr", ptrOffset, 4);

                dataAddr = toUInt32FromBytes(dataAddrBytes.bytes, 4) ;

                ldata = await readStringFromAddr(dataAddr, 1000);
                mdata = ldata.split("\x1e");
                if(mdata[0] == "LOG_OPCODE_EVENT")
                {
                    recType = LoggerBuf_EVENT;
                }
            }

            /* The continued data structure doesn't have a format string as this
             * place is used for storage of user data
             */
            if(recType != LoggerBuf_BUFFER_CONTINUED) {
                /* Get the pointer to the out file string from the out file
                 * pointer table in the out file
                 */
                let ptrOffset = Number(rec.data[0]) - 0x94000008;

                let dataAddrBytes = await Program.task.symbols.getSectionData(symbolFile, ".log_ptr", ptrOffset, 4);

                dataAddr = toUInt32FromBytes(dataAddrBytes.bytes, 4) ;

                ldata = await readStringFromAddr(dataAddr, 1000);
                mdata = ldata.split("\x1e");

                /* Remove leading and trailing double-quote (") in format string */
                var fmt = mdata[LOG_EVT_FORMAT_STR_OFFSET].replace(/^"/, "").replace(/"$/, "");

                /* If the record is an event, get the formatted string from the event symbol */
                if (recType == LoggerBuf_EVENT) {
                    var evtAddr = await Program.lookupSymbolValue("LogSymbol_" + fmt);
                    ldata = String(await readStringFromAddr(evtAddr, 1000));
                    mdata = ldata.split("\x1e");
                    fmt = mdata[LOG_EVT_FORMAT_STR_OFFSET].replace(/^"/, "").replace(/"$/, "");
                }
            }

            /* Create a new record for the log statement */
            var recView = new LogSinkBufRecord();

            /* Lead with instance name */
            recView.Instance = instanceName;

            /* Capture sequence number */
            recView.Serial = rec.serial;

            /* Construct timestamp */
            var iaddr = await Program.lookupSymbolValue("TimestampP_nativeFormat32");
            var timeStampFormat = await Program.fetchFromAddr(iaddr, "TimestampP_Format");

            var fracBytes = timeStampFormat.format.fracBytes;
            var intBytes = timeStampFormat.format.intBytes;
            var exponent = timeStampFormat.format.exponent;
            var multiplier = timeStampFormat.format.multiplier;

            /* Recreate timestamp as per the TimestampP_Format format */
            var timeStampFractional = 0;
            var timestampInteger = 0;

            if (fracBytes == 4) {
                timeStampFractional = rec.timestampLow;
            }
            else {
                timeStampFractional = (rec.timestampLow & ((1 << (fracBytes*8)) - 1)) / (1 << (fracBytes*8));
            }

            if (intBytes == 4) {
                timestampInteger = rec.timestampLow;
            }
            else {
                timestampInteger = (rec.timestampLow >> (fracBytes*8)) & ((1 << (intBytes*8)) - 1);
            }

            recView.Timestamp = multiplier * (timeStampFractional + timestampInteger) * Math.pow(10, -exponent);

            /* Set fixed number of decimals on timestamp */
            recView.Timestamp = recView.Timestamp.toFixed(8);

            /* Parse the record based on log type */
            if (recType == LoggerBuf_EVENT) {
                addEventToTable(rec, fmt, mdata, recView);
            }
            else if (recType == LoggerBuf_PRINTF) {
                addPrintfToTable(rec, fmt, mdata, recView);
            }
            else if (recType == LoggerBuf_BUFFER_START ||
                        recType == LoggerBuf_BUFFER_CONTINUED) {
                addBufToTable(rec, fmt, mdata, recView);
            }
        }

        for (let idx = 0; idx < instanceSymbols.length; idx++) {
            /* Read requested instance from target memory */

            /* Trim off the generated parts of the symbol name to make it a bit
             * shorter.
             */
            var instanceName = instanceSymbols[idx].name.replace("LogSinkBuf_", "").replace("_config", "");
            var inst = await this.Program.fetchFromAddr(Number(instanceSymbols[idx].location), "LogSinkBuf_Instance");

            /* check for empty log buffer */
            if (inst.serial == 0) {
                continue;
            }

            var addr;

            if (inst.bufType == 1) { /* linear buffer  */
                for (var i = 0; i < inst.serial; i++) {
                    addr = inst.buffer + (i * recSize);
                    await makeRecord(addr, instanceName);
                }
            }
            else { /* circular buffer */
                var count;

                if (inst.serial < inst.numEntries) {
                    addr = inst.buffer;
                    count = inst.serial;
                }
                else {
                    addr = (inst.curEntry == inst.endEntry ? inst.buffer :
                        inst.curEntry + recSize);
                    count = inst.numEntries - 1;
                }

                for (var r = 0; r < count; r++) {
                    await makeRecord(addr, instanceName);
                    addr = (addr == inst.endEntry ? inst.buffer : addr + recSize);
                }
            }
        }

        return table;
    }
}
exports.classCtor = LogSinkBuf;