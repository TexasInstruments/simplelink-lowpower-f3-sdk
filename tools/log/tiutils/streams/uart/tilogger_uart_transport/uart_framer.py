import logging
from dataclasses import dataclass, field
from enum import Enum

from typing import Optional

from tilogger.tracedb import ElfString, Opcode, TraceDB

logger = logging.getLogger("UART Framer")
# To enable debug output, uncomment the following line
# logging.basicConfig(level=logging.DEBUG)

UART_RESET_TOKEN = bytes([0xBB, 0xBB, 0xBB, 0xBB])


class UARTOpcode(Enum):
    """Opcodes for UART frames that are built in UARTFramer"""

    DATA = 0
    ERROR = 1
    TIMESTAMP_FORMAT = 2
    UNPARSED = None


################################################################################
################################################################################


def build_value(buf):
    """
    Helper function: Turn an iterable into a little-endian integer
    """
    value = 0
    for idx, val in enumerate(buf):
        value += val << (idx * 8)
    return value


@dataclass
class UARTFrame:
    """Base UART frame that stores common information and should be subclassed by other frames"""

    header: int
    opcode: UARTOpcode
    ts_counter: float = 0
    # value: int = 0

    size: int = 0
    string: str = "Frame has not yet been parsed"

    def __len__(self):
        return self.size

    def __str__(self):
        return self.string


@dataclass
class UARTDataFrame(UARTFrame):
    """Log data frame"""

    opcode: UARTOpcode = UARTOpcode.DATA
    data: bytearray = field(default_factory=bytearray)

    def parse(self, buf, size):
        """Build UARTDataFrame from buf"""
        self.size = size
        # Store data
        self.data = buf[: self.size]
        # build string
        self.string = " ".join((("0x{:02X}".format(i)) for i in self.data))
        return buf[self.size :]

    def __str__(self):
        return self.string


class UARTErrorFrame(UARTFrame):
    """Log error frame"""

    opcode: UARTOpcode = UARTOpcode.ERROR
    data: bytearray = field(default_factory=bytearray)

    def parse(self, buf):
        """Build UARTErrorFrame from buf"""
        self.size = 4
        # Store data
        self.data = buf[: self.size]
        # build string
        self.string = " ".join((("0x{:02X}".format(i)) for i in self.data))
        return buf[self.size :]

    def __str__(self):
        return self.string


class UARTTimestampFormatFrame(UARTFrame):
    """Log timestamp format frame"""

    opcode: UARTOpcode = UARTOpcode.TIMESTAMP_FORMAT
    data: bytearray = field(default_factory=bytearray)

    def parse(self, buf):
        """Build UARTTimestampFormatFrame from buf"""
        self.size = 4
        # Store data
        self.data = buf[: self.size]
        # build string
        self.string = " ".join((("0x{:02X}".format(i)) for i in self.data))
        return buf[self.size :]

    def __str__(self):
        return self.string


################################################################################
################################################################################


class UARTFramer:
    """
    Manages parsing serial data into UARTFrames and outputs UARTFrames onto output queue q

    Args:
        q: Output queue

    """

    def __init__(self, output_queue, trace_db: TraceDB):
        # Create the PDU stream thread.
        self._output_queue = output_queue
        self.last_ts_counter = 0
        self._first_read = True
        self._trace_db = trace_db

    def parse(self, buf: bytearray):
        """
        Parse as many bytes as possible from the input buffer. If the buffer contains less than 4 bytes,
        nothing is extracted

        There is special funcionality to handle the reset frame (indentified by UART_RESET_TOKEN). Initially,
        parsing of other frames will not start until the reset frame is found. After this, the buffer received for
        parsing will be searched for the reset frame. If the reset frame is found, all preceding data will be discarded
        and parsing will continue at the  software source frame containing the rest

        Args:
          buf: input buffer to parse

        Returns:
            Unparsed portion of the input buffer

        """
        if not buf:
            return buf

        frame: Optional[UARTFrame] = None

        # If the reset token is found
        if UART_RESET_TOKEN in buf and self._first_read is True:

            if len(buf) < 8:
                logger.debug("Reset token found, but no timestamp format available")
                return buf
            else:
                logger.debug("Reset token and timestamp format found.")
                # Discard anything before the reset token
                buf = buf[buf.index(UART_RESET_TOKEN) + 4 :]

                frame = UARTTimestampFormatFrame(0, opcode=UARTOpcode.TIMESTAMP_FORMAT)
                buf = frame.parse(buf)

                logger.debug("Timestamp format frame: %s", frame)

                # queue packet for output
                self._output_queue.put(frame)

        # Return full buffer in case any part of the reset token was at the end, in
        # which case only part of the token may have been received. Then reparse
        # the next time data is added to the buffer
        elif self._first_read and buf[-1] == 0xBB:
            logger.debug("Only parts of reset token found. ")
            return buf

        # If first run and no reset found yet, don't parse anything
        elif self._first_read is True:
            logger.debug("Waiting for a reset frame to begin parsing.")
            return bytearray()

        # While there is a full packet to parse...
        while True:
            self._first_read = False

            # Nothing meaningful to parse if there are less than 4 bytes
            if len(buf) < 4:
                logger.debug("Not enough data to parse.")
                return buf

            # Make the assumption that the first 4 bytes is a header or overflow header
            header = build_value(buf[0:4])
            packet_length = 0

            # Default to creating a dataframe
            frame = UARTDataFrame(0)

            if header in self._trace_db.traceDB:

                # Check what type of log statement this header is and find the expected number of arguments
                elf_string = self._trace_db.traceDB[header]

                # If it is a Log_printf statement then read the number of arguments from the elf string
                if elf_string.opcode == Opcode.FORMATTED_TEXT:
                    packet_length = 8 + elf_string.nargs * 4

                if elf_string.opcode == Opcode.EVENT:
                    packet_length = 8 + elf_string.nargs * 4

                # If it is a Log_buf statement then read the size from the third argument.
                # First make sure there is enough data to do so.
                elif elf_string.opcode == Opcode.BUFFER:
                    if len(buf) < 12:
                        return buf
                    else:
                        packet_length = 12 + build_value(buf[8:12])

                # There is not enough data to construct a packet.
                if len(buf) < packet_length:
                    logger.warning("FRAMING: Warning, this packet needs %d bytes", packet_length)
                    return buf

            elif (header & 0xFFF80000 == 0x80000000) and (0x90000000 | (header & 0x3FFFF)) in self._trace_db.traceDB:
                # This address does not exist in the trace database
                logger.warning("Overflow detected at 0x%x", header)
                frame = UARTErrorFrame(0)

            else:
                logger.warning("FRAMING: Error, unexpected header. No trace database information at 0x%x", header)
                # Try to process buffer until a valid frame is found
                buf.pop(0)
                return buf

            if frame is not None:
                # Parse packet based on packet type
                try:
                    if frame.opcode == UARTOpcode.DATA:

                        # If it's a buffer frame, trim the "nargs" field from the data,
                        # otherwise it gets parsed as part of the data
                        if self._trace_db.traceDB[header].opcode == Opcode.BUFFER:
                            del buf[8:12]
                            # Update the packet length
                            packet_length -= 4

                        # Parse buffer
                        buf = frame.parse(buf, packet_length)

                        # Log packet that was just parsed
                        logger.debug("Parsed data frame (size %d, %d left in buf): %s", packet_length, len(buf), frame)

                        # queue packet for output
                        self._output_queue.put(frame)

                    if frame.opcode == UARTOpcode.ERROR:
                        # Parse buffer
                        buf = frame.parse(buf)

                        self._output_queue.put(frame)

                except Exception as exc:  # pylint: disable=broad-except
                    logger.error("Invalid UART Packet")
                    logger.debug(exc)

        # Return unparsed data
        return buf