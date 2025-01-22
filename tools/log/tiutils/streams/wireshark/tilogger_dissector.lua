--[[
Copyright (C) 2021-2024, Texas Instruments Incorporated

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

  Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the
  distribution.

  Neither the name of Texas Instruments Incorporated nor the names of
  its contributors may be used to endorse or promote products derived
  from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--]]

local logger_protocol = Proto("TILogger",  "TILogger Protocol")

pf_alias  =     ProtoField.string("tilogger.alias", "Alias", base.STRING)
pf_ts_local  =  ProtoField.string("tilogger.ts_local", "Device time", base.STRING)
pf_opcode =     ProtoField.string("tilogger.opcode", "Opcode", base.STRING)
pf_module =     ProtoField.string("tilogger.module", "Module", base.STRING)
pf_loglevel =   ProtoField.string("tilogger.level", "Log Level", base.STRING)
pf_file =       ProtoField.string("tilogger.file", "File", base.STRING)
pf_line =       ProtoField.string("tilogger.line", "Line", base.STRING)
pf_string =     ProtoField.string("tilogger.string", "String", base.STRING)

logger_protocol.fields = {
  pf_alias,
  pf_ts_local,
  pf_opcode,
  pf_module,
  pf_loglevel,
  pf_file,
  pf_line,
  pf_string
}


function split (inputstr)
	local t={}
	for str in string.gmatch(inputstr, "([^||]+)") do
		table.insert(t, str)
	end
	return t
end


function logger_protocol.dissector(buffer, pinfo, tree)
  length = buffer:len()
  if length == 0 then return end

  pinfo.cols.protocol = logger_protocol.name

  local subtree = tree:add(logger_protocol, buffer(), "TILogger")
  local stringbuf = buffer(0, length):string(ENC_UTF_8)

  parts = split(stringbuf)
  subtree:add(pf_alias, parts[1])
  subtree:add(pf_ts_local, parts[2])
  subtree:add(pf_opcode, parts[3])
  subtree:add(pf_module, parts[4])
  subtree:add(pf_loglevel, parts[5])
  subtree:add(pf_file, parts[6])
  subtree:add(pf_line, parts[7])
  subtree:add(pf_string, parts[8])
end

local raw_ip = DissectorTable.get("wtap_encap")
raw_ip:add(7, logger_protocol)
