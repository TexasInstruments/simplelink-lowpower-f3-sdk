"""
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
"""

from dataclasses import dataclass, field
from os import name
from pathlib import Path
import sys
import atexit
import argparse
import queue
import threading
import logging

from typing import Optional, NoReturn, List
import click

from tilogger.interface import LogPacket, LoggerCliCtx, TransportABC
from tilogger.logger import Logger
from tilogger.tracedb import TraceDB

from .serial_rx import SerialRx
from .itm_framer import ITMFramer
from .itm_to_log import ITMPacketiser

import typer


class ITM_Transport(TransportABC):
    def __init__(self, port: str, baudrate: int, trace_db: TraceDB, alias: str):
        super().__init__()

        self._com_port = port
        self._baud_rate = baudrate
        self._trace_db = trace_db
        self._alias = alias
        self.serial: Optional[SerialRx] = None
        self.stop_event = threading.Event()

    @property
    def alias(self):
        return self._alias

    def stop(self):
        self.stop_event.set()

    def start(self, logger: Optional[Logger]) -> NoReturn:
        self.serial = SerialRx(self._com_port, self._baud_rate, alias=self._alias)
        atexit.register(self.serial.close)

        frame_queue: queue.Queue = queue.Queue()
        framer = ITMFramer(frame_queue)

        # Note we use "Logger == None" as our 'ITM only mode' flag
        if logger:
            packetiser = ITMPacketiser(self._trace_db, logger, self.alias)
        else:
            packetiser = None

        rx_data: bytearray = bytearray()
        while not self.stop_event.is_set():
            rx_data.extend(self.serial.receive())
            rx_data = framer.parse(rx_data)

            try:
                result = frame_queue.get(block=True, timeout=0.001)
            except queue.Empty:
                continue

            if logger:
                packet: Optional[LogPacket] = packetiser.parse(result)
                if packet:
                    logger.log(packet)
            else:
                print(result)

    def reset(self):
        pass

    def timestamp_to_seconds(self, timestamp: int) -> float:
        return float(timestamp)


def transport_factory_cli(app: typer.Typer):
    @app.command(name="itm")
    def transport_factory_cb(
        ctx: typer.Context,
        port: str = typer.Argument(..., help="Serial port (eg COM12)"),
        baudrate: int = typer.Argument(..., help="TPIU baudrate"),
        elf: List[Path] = typer.Option([], help="Symbol file path (elf/out file)"),
        alias: Optional[str] = typer.Option(None, help="Alias for this device in the log"),
    ):
        """Add ITM transport as input to log.

        You need to specify a serial port and a baudrate (ITM driver module
        configures this). The baudrate is often quite high, such as 12000000
        (12MHz).

        You also need to specify .out/.elf files that contain symbol information
        needed to parse the log, but this may also be provided globally before
        adding transports.
        """

        state = ctx.ensure_object(LoggerCliCtx)
        elves = state.symbol_files + elf

        if len(elves) == 0:
            typer.secho(
                "Need elf symbols. Specify via --elf <file> on command line.", fg=typer.colors.BRIGHT_RED, err=True
            )
            sys.exit(1)

        db = TraceDB(elves, repickle=False)
        itm_transport = ITM_Transport(port, baudrate, db, alias or port)
        return itm_transport


#### Local usage below


def itm_parser_main():
    # Local finalizer for command results, not exported.
    def local_cli_finalizer(transports: List[ITM_Transport]):
        for t in transports:  # transports returned by each subcommand
            t.start(logger=None)

    local_cli = typer.Typer(name="itm_transport", result_callback=local_cli_finalizer, chain=True)
    transport_factory_cli(local_cli)
    local_cli()


def itm_raw_viewer():
    # Open raw ITM viewer based on COM-port and baudrate.
    parser = argparse.ArgumentParser(prog="itm_transport", description="View raw ITM data from auxiliary COM port")

    # Add positional argument to get COM-port
    parser.add_argument(
        "com_port",
        metavar="COM",
        type=str,
        help="Choose the COM port for the device to be debugged. Note that this must be the Auxiliary COM port.",
    )

    # Add positional argument to get baudrate
    parser.add_argument(
        "baud_rate",
        metavar="BAUD",
        type=int,
        help="Choose a baud rate for serial communication.",
    )

    # Parse arguments from command-line
    args = parser.parse_args()

    # Create an ITM transport and start it. With logger=None, the raw output will be printed to terminal
    itm_transport = ITM_Transport(args.com_port, args.baud_rate, None, None)
    itm_transport.start(logger=None)
