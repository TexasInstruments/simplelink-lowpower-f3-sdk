"""
Copyright (C) 2024, Texas Instruments Incorporated

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
"""

import json
from typing import Optional
from tilogger.interface import LogPacket, LoggerCliCtx, TransportABC, LogLevel
from tilogger.logger import Logger
import typer

from tilogger_to_replayfile.main import REPLAY_FILE_FORMAT_VERSION


SUPPORTED_FILE_FORMAT_VERSION = REPLAY_FILE_FORMAT_VERSION


class FromReplayFile_Transport(TransportABC):
    """This class reads logs from a JSON file, convert them into log packets
    and sends them to the logger core.
    """

    def __init__(self, replayFile):
        self.replayFile = replayFile
        self._alias = f"Replay from {replayFile}"

    def alias(self) -> str:
        return self._alias

    def stop(self):
        pass

    def start(self, logger: Optional[Logger]):
        with open(self.replayFile, "r", encoding="utf-8") as file:

            # Check replay file version is supported
            metadata = json.loads(file.readline())
            received_file_version = metadata["properties"]["format_version"]
            if received_file_version != SUPPORTED_FILE_FORMAT_VERSION:
                raise Exception(
                    f"WARNING: This replayfile version is not supported. This replayfile version is {received_file_version}, this tool expects version {SUPPORTED_FILE_FORMAT_VERSION}."
                )

            # Read rest of file and log each line
            for line in file:
                packet = json.loads(line, object_hook=log_decode)
                logger.log(packet)

    def reset(self):
        pass

    def timestamp_to_seconds(self, timestamp: int) -> float:
        return float(timestamp)


def transport_factory_cli(app: typer.Typer):
    @app.command(name="from-replayfile")
    def transport_factory_cb(ctx: typer.Context, file: str = typer.Argument(..., help="Replay file path (JSON file)")):
        """Replay a sequence of logs from a replay file.

        You need to specify path to a replay file (JSON file) and an
        output mode (stdout or wireshark).

        A simple invocation that loads from replay file and outputs to stdout is

            tilogger from-replayfile path/to/replayFile.json stdout
        """

        state = ctx.ensure_object(LoggerCliCtx)
        return FromReplayFile_Transport(replayFile=file)


#### JSON Helper


def log_decode(input_dict) -> LogPacket:
    packet = LogPacket(
        alias=input_dict["alias"],
        module=input_dict["module"],
        opcode=input_dict["opcode"],
        level=LogLevel(input_dict["level"]),
        filename=input_dict["filename"],
        lineno=input_dict["lineno"],
        timestamp=input_dict["timestamp"],
        timestamp_local=input_dict["timestamp_local"],
        data=None,
        trace_db=None,
    )
    packet._str_data = input_dict["_str_data"]
    packet._final_str_data = input_dict["_final_str_data"]

    return packet
