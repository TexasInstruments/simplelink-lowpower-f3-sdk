"""
TI SimpleLink CRC tool

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
"""Generic utility functions."""

import zlib

from typing import List, Tuple, Dict, Set
from crc_tool.util.consts import CRC_BYTE_COUNT, BYTE_ORDER


def int_list_to_hex_string_list(ints: List[int]) -> str:
    """Get string representation of list of integers, as list of hex numbers.

    Used to make numbers in logging more human-readable.

    For example: [255, 254, 1] -> "[0xff, 0xfe, 0x1]"
    """
    # pylint: disable-next=consider-using-f-string
    return "[{}]".format(", ".join(hex(num) for num in ints))


def do_crc_write_regions_overlap(range1: Tuple[int, int], range2: Tuple[int, int]):
    """Check whether there is overlap where CRCs will be written."""
    # So here we check if the start of the second write region
    # (second in the order of address being written to)
    # is smaller or equal to than the end of the first one
    return max(range1[1] + 1, range2[1] + 1) <= min(
        range1[1] + CRC_BYTE_COUNT, range2[1] + CRC_BYTE_COUNT
    )


# Function name is too long
# pylint: disable-next=invalid-name
def get_keys_with_overlapping_write_regions(
    symbol_pair_map: Dict[str, Tuple[int, int]]
) -> Dict[str, Set[str]]:
    """Get a mapping of names which would write CRCs to overlapping memory locations.

    Parameters:
        symbol_pair_map:
            Dictionary with regions, to check if any would write to same region.
    Returns:
        Dict of symbol name without suffix->{set of symbol names without suffix, with the same CRC write region}
    """
    res: Dict[str, Set[str]] = {}
    for outer_key, outer_value in symbol_pair_map.items():
        for inner_key, inner_value in symbol_pair_map.items():
            if inner_key != outer_key:
                if do_crc_write_regions_overlap(outer_value, inner_value):
                    # if overlap A -> B exists, check that B -> A isn't mapped already
                    if inner_key in res and outer_key in res[inner_key]:
                        # We have already mapped inverse of the connection we just found
                        continue
                    if outer_key in res:
                        res[outer_key].add(inner_key)
                    else:
                        res[outer_key] = {inner_key}
    return res


def get_crc_bytes(data: List[int]) -> List[int]:
    """Calculate crc32 bytes over a list of bytes."""
    crc = zlib.crc32(bytearray(data))
    return list(crc.to_bytes((crc.bit_length() + 7) // 8, byteorder=BYTE_ORDER))  # type: ignore


def get_int_from_byte_list(data: List[int]) -> int:
    """Get integer from list of bytes."""
    return int.from_bytes(bytearray(data), byteorder=BYTE_ORDER, signed=False)  # type: ignore

def get_symbol_pair_map_string(
    symbol_pair_map: Dict[str, Tuple[int, int]]
) -> str:
    """
    Get string representation of { string -> (int, int) } dictionary using hexadecimal.

    Parameters:
        symbol_pair_map:
            mapping of identifier -> (crc region begin address, crc region end address)

    The hexadecimal numbers are padded to be minimum 8 characters wide.
    """

    return "{" + ", ".join(
        [
            # Pad with leading zeroes, minimum length 8
            f"'{symbol}': (0x{value[0]:08x}, 0x{value[1]:08x})"
            for symbol, value in symbol_pair_map.items()
        ]
    ) + "}"


def get_symbol_value_map_string(symbol_value_map: Dict[str, int]) -> str:
    """
    Get string representation of { string -> int } dictionary using hexadecimal.

    Parameters:
        symbol_value_map: Dict of { symbol name -> value }

    The hexadecimal numbers are padded to be minimum 8 characters wide.
    """
    return "{" + ", ".join(
        [
            # Pad with leading zeroes minimum length 8
            f"'{symbol}': 0x{value:08x}"
            for symbol, value in symbol_value_map.items()
        ]
    ) + "}"
