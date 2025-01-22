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

import logging
import typer

from typing import List

from tilogger.interface import LogOutputABC, LogPacket, LoggerCliCtx, LogLevel

logger = logging.getLogger("Standard Output")

DEFAULT_LOGGING_SCHEME = "{I} | {T} | {M} | {L} | {F} | {D}"


class StdoutOutput(LogOutputABC):
    """
    This class outputs a standard output stream for
    notified log packets.
    """

    def __init__(self, logging_scheme, column_padding):
        self.logging_scheme = logging_scheme
        self.column_padding = column_padding
        logger.info("Standard output initilazed")
        self._alias_column_width = 0
        self._time_column_width = 0
        self._module_column_width = 0
        self._level_column_width = len(max(LogLevel, key=lambda level: len(level.name)).name)
        self._fileLineNumber_column_width = 0

    def start(self):
        logger.info("Waiting to receive logs..")

    def notify_packet(self, packet: LogPacket) -> None:
        self._send(packet)

    def _send(self, packet: LogPacket):

        if self.column_padding:

            self._update_max_column_width(packet)

            # Configuration for logging scheme - with column padding.
            column_padding_config = {
                "I": f"{packet.alias: <{self._alias_column_width}}",
                "T": "{:>{w}}".format(f"{packet.timestamp_local : .9f}", w=self._time_column_width),
                "M": f"{packet.module:<{self._module_column_width}}",
                "L": f"{packet.level.name:<{self._level_column_width}}",
                "F": "{:<{w}}".format(f"{packet.filename}:{packet.lineno}", w=self._fileLineNumber_column_width),
                "D": packet._str_data,
            }

            print(self.logging_scheme.format(**column_padding_config))

        else:

            # Confiurations for logging scheme - no column padding.
            no_padding_config = {
                "I": packet.alias,
                "T": f"{packet.timestamp_local:.9f}",
                "M": packet.module,
                "L": str(packet.level.name),
                "F": f"{packet.filename}:{packet.lineno}",
                "D": packet._str_data,
            }

            print(self.logging_scheme.format(**no_padding_config))

    def _update_max_column_width(self, packet: LogPacket):
        # Update column width values if packet requires bigger columns
        if self._alias_column_width < len(packet.alias):
            self._alias_column_width = len(packet.alias)
        if self._time_column_width < len(f"{packet.timestamp_local : .9f}"):
            self._time_column_width = len(f"{packet.timestamp_local : .9f}")
        if self._module_column_width < len(packet.module):
            self._module_column_width = len(packet.module)
        if self._level_column_width < len(packet.level.name):
            self._level_column_width = len(packet.level.name)
        # File and lineno. column need +1 to account for colon - "{filename}:{lineno}"
        if self._fileLineNumber_column_width < len(str(packet.filename)) + len(str(packet.lineno)) + 1:
            self._fileLineNumber_column_width = len(str(packet.filename)) + len(str(packet.lineno)) + 1


# Function that adds a command to a typer instance via decorator
def output_factory_cli(app: typer.Typer):
    @app.command(name="stdout")
    def output_factory_cb(
        ctx: typer.Context,
        logging_scheme: str = typer.Option(
            DEFAULT_LOGGING_SCHEME, "--format", "-f", show_default=False, help=f'[default: "{DEFAULT_LOGGING_SCHEME}"]'
        ),
        disable_column_padding: bool = typer.Option(
            False, "--disable-column-alignment", show_default=False, help="Disable dynamic column alignment"
        ),
    ):
        """Add log output as standard output stream.

        If custom format is wanted, use --format "<FORMAT_SPEC>" with desired format specifiers.

        \b
        Supported Specifiers
        {I}: Identifier (for example COM7, or the provided alias)
        {T}: Timestamp
        {M}: Module  (for example LogMod_App1)
        {L}: Log level (for example Log_INFO)
        {F}: File and line number (for example path/log.c:134)
        {D}: Log data

        """

        state = ctx.ensure_object(LoggerCliCtx)
        return StdoutOutput(logging_scheme=logging_scheme, column_padding=False if disable_column_padding else True)
