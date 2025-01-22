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
"""Used to interact with ELF file and insert CRC values."""
from io import TextIOWrapper
from typing import Dict, Tuple
import logging

import lief
from lief import Binary

from crc_tool.util.consts import (
    CRC_BYTE_COUNT,
    SECTION_BEGIN_SUFFIX,
    SECTION_END_SUFFIX,
)
from crc_tool.util import util


class ElfHandler:
    """Type used to interact with ELF file."""

    elf_file: Binary

    def __init__(self, elf_file: TextIOWrapper):
        """Parse LIEF binary from input file."""
        self.elf_file = lief.parse(elf_file)

    def __insert_crc_after(self, begin: int, end: int):
        """Calculate CRC over section and insert in ELF file."""
        crc_write_begin = end + 1
        length = crc_write_begin - begin

        prev_bytes = util.get_int_from_byte_list(
            self.elf_file.get_content_from_virtual_address(
                crc_write_begin, CRC_BYTE_COUNT
            )
        )
        crc_bytes = util.get_crc_bytes(
            self.elf_file.get_content_from_virtual_address(begin, length)
        )

        # Logging to mirror logging from uniflash
        logging.info(
            (
                "Updating CRC32 field @ address 0x%08x, "
                "based on data in the range [0x%08x, 0x%08x]. "
                "Value changes from 0x%08x to 0x%08x"
            ),
            crc_write_begin,
            begin,
            end,
            prev_bytes,
            util.get_int_from_byte_list(crc_bytes),
        )

        self.elf_file.patch_address(address=crc_write_begin, patch_value=crc_bytes)

    def get_crc_symbol_pair_map(self, symbol_prefix: str) -> Dict[str, Tuple[int, int]]:
        """Get a map of crc sections parsed from ELF file.

        Parses symbols with given prefix from ELF file,
        removes "_end" and "_begin" in symbol names, and returns map of
        this stem -> begin value, end value

        Any symbols not matching the format {symbol_prefix}*[_begin|_end] are
        ignored.

        If a symbol matching {symbol_prefix}*_begin is found, but no
        corresponding symbol matching  {symbol_prefix}*_end is found,
        the symbol is ignored.

        The same goes for symbols with the suffix _end, but no matching
        symbol with the suffix _begin.

        Parameters:
            symbol_prefix: prefix for symbols in ELF file

        Returns:
            symbol name without suffix -> [_begin address, _end address]

        Raises:
            ValueError: If two or more regions would overwrite each other's CRC.
                For example, an error would be raised given two CRC regions
                from 0 to 4 and from 0 to 6. These would write their 4 byte CRC
                values to 5-8 and 7-10 respectively. The regions written to
                overlap, and an error would therefore be raised.
        """
        symbol_pair_map = self.__join_begin_end_symbol_pairs(
            self.__get_symbols_with_prefix(symbol_prefix)
        )
        # Check that end is not before beginning
        for key, section in symbol_pair_map.items():
            if not section[0] <= section[1]:
                raise ValueError(
                    f"Beginning comes before end for symbol: {key} = (0x{section[0]:x}, 0x{section[1]:x})",
                )
        # check that they do not want to write CRCs to the same memory address
        overlapping_crc_keys = util.get_keys_with_overlapping_write_regions(
            symbol_pair_map
        )
        if len(overlapping_crc_keys) != 0:
            raise ValueError(
                "Cannot add the following CRC sections, "
                + f"because of overlapping CRC write locations: {overlapping_crc_keys}"
            )
        return symbol_pair_map

    def insert_crc_by_symbol_pair_map(
        self, symbol_pair_map: Dict[str, Tuple[int, int]]
    ) -> bool:
        """Insert CRC after each section denoted by identifier -> (begin, end) mapping.

        Inserts values in ascending order based on end value.

        Parameters:
            symbol_pair_map:
                mapping of identifier -> (crc region begin address, crc region end address)
        Returns:
            True if any CRCs were added, false if not
        """
        logging.debug(
            "Inserting CRCs from map: %s",
            util.get_symbol_pair_map_string(symbol_pair_map),
        )
        crc_was_inserted = False
        # Get list of tuples in format [(key1, (section_begin, section_end)), (key2, (section2_begin, section2_end))]
        # Need to insert in ascending order in order to ensure we do not write
        # into an area for which the CRC value has already has been calculated
        for key, section in sorted(
            symbol_pair_map.items(), key=lambda key_value: key_value[1][1]
        ):
            logging.info(
                "Inserting CRC section for variable pair with stem: %s = (0x%08x, 0x%08x)",
                key,
                section[0],
                section[1],
            )
            self.__insert_crc_after(begin=section[0], end=section[1])
            crc_was_inserted = True
        return crc_was_inserted

    def __join_begin_end_symbol_pairs(
        self, symbol_value_map: Dict[str, int]
    ) -> Dict[str, Tuple[int, int]]:
        """Remove _end and _begin suffix from dict, and join values to single tuple.

        Ignores any symbols not matching the format "*[_begin|_end]"

        Parameters:
            symbol_value_map: Dict of { symbol name -> value }, parsed from ELF file.

        Returns:
            Dict of {
                Symbol without suffix ->
                    (value of symbol with _begin suffix, value of symbol with _end suffix)
            }
        """
        res = {}
        logging.debug(
            "Joining value pairs of dictionary: %s",
            util.get_symbol_value_map_string(symbol_value_map),
        )
        for key in symbol_value_map.keys():
            if key.endswith(SECTION_BEGIN_SUFFIX):
                begin_key = key
                # Cut off suffix
                stem = begin_key[: -len(SECTION_BEGIN_SUFFIX)]
                end_key = stem + SECTION_END_SUFFIX
                if end_key in symbol_value_map:
                    res[stem] = (symbol_value_map[begin_key], symbol_value_map[end_key])
                else:
                    logging.warning(
                        "No matching end value found for %s, ignoring begin value", key
                    )
            elif not key.endswith(SECTION_END_SUFFIX):
                logging.warning(
                    "Symbol: %s, does not have a valid suffix, ignoring", key
                )
        return res

    def __get_symbols_with_prefix(self, prefix: str) -> Dict[str, int]:
        """Get dict of name->value of symbols with given prefix.

        Args:
            prefix: The desired prefix
        Returns:
            Map of symbol name -> symbol value
        """
        logging.info("Iterating symbols, looking for prefix: %s", prefix)
        res = {}
        for stat_sym in self.elf_file.symbols:
            logging.debug("Checking symbol %s", stat_sym)
            if stat_sym.name.startswith(prefix):
                logging.debug("Found symbol %s", stat_sym)
                res[stat_sym.name] = stat_sym.value
        if len(res) == 0:
            logging.warning("No symbols with prefix: %s found", prefix)
        return res

    def write(self, dst: str):
        """Write current state of ELF file to disk."""
        logging.info("Writing patched ELF file to %s", dst)
        self.elf_file.write(dst)
