"""
TI SimpleLink CRC Tool

Tool to insert CRCs into ELF files or generate binary for programming
ccfg user region

Copyright (C) 2024 Texas Instruments Incorporated


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
"""Used to generate user record binary file with crc."""
from typing import List
import logging

from crc_tool.util import util
from crc_tool.util.consts import USER_RECORD_CONTENT_LENGTH, BYTE_ORDER


class TxtHandler:
    """Interacts with string data from txt files for programming user record.

    Class used to read a hex value, calculate the crc.
    and write the orignal data + crc to a binary file
    """

    data: List[int]
    crc: List[int]

    def __init__(self, input_hexes: List[str]):
        """Parse hex number from input and calculate crc."""
        logging.debug("Converting user record data to binary: %s", input_hexes)

        # Got some errors due to me giving strings by accident
        # So this is useful for development
        assert isinstance(input_hexes, list)
        input_hex_strings = []
        for hex_string in input_hexes:
            # Need to remove prefix for python to parse properly
            # Check startswith instead of just removing to avoid AB0x34 being valid
            if hex_string.startswith("0x"):
                without_prefix = hex_string[2:]
            else:
                without_prefix = hex_string
            input_hex_strings.append(without_prefix)

        logging.debug("Input with stripped prefixes is: %s", input_hex_strings)
        if len(input_hex_strings) == 0:
            raise ValueError("No data found in user-record-file")

        self.data = []
        for hex_string in input_hex_strings:
            # Need to check length of integer, to include zeros in desired width
            # For example 0x0002 -> should return 2, not 1
            num_hex_chars = len(hex_string)
            # Must be an even number of hex characters
            if num_hex_chars % 2 == 1:
                raise ValueError(
                    f"Input value {hex_string} is invalid, length must be a multiple of two"
                )

            # Two hex characters to every byte
            num_bytes_in_integer = num_hex_chars // 2

            desired_integer = int(hex_string, 16)
            self.data += list(
                desired_integer.to_bytes(
                    length=num_bytes_in_integer, byteorder=BYTE_ORDER  # type: ignore
                )
            )
        logging.debug(
            "Parsed input data is: %s", util.int_list_to_hex_string_list(self.data)
        )

        if len(self.data) > USER_RECORD_CONTENT_LENGTH:
            raise ValueError(
                f"Value passed to TxtHandler is too long at {len(self.data)} bytes, "
                    + f"max length is {USER_RECORD_CONTENT_LENGTH}"
            )
        pad_length = USER_RECORD_CONTENT_LENGTH - len(self.data)
        logging.debug("Padding data with %d zeroes", pad_length)
        self.data += [0] * pad_length
        logging.debug(
            "Calculating CRC for %s", util.int_list_to_hex_string_list(self.data)
        )

        self.crc = util.get_crc_bytes(self.data)
        logging.debug(
            "Calculated CRC is %s", util.int_list_to_hex_string_list(self.crc)
        )

    def write(self, dst: str):
        """Write current state of file to disk."""
        logging.info("Writing binary data to %s", dst)
        with open(dst, "wb") as binary_file:
            binary_file.write(bytearray(self.data + self.crc))

    def __str__(self):
        return f"TxtHandler(data={self.data}, crc={self.crc})"
