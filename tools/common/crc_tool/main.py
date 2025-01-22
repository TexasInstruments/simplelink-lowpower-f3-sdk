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
"""Tool with two use cases:
    Update CRC32 for regions in an ELF file:
        Parse variables defined in an ELF file and output ELF file which
        contains CRC values inserted after these regions.
    Append CRC32 to CCFG user record in text format, and convert to binary:
        Generates a binary which can be used to program the CCFG 128-byte user record.
        Input needs to be from a text file containing one or more
        whitespace separated hex values adding up to of max 124 bytes,
        not counting any optional 0x prefixes.

        The given integers will be inserted in the order they are stated,
        starting at the lowest address.
        Each integer will be inserted in little endian.


        Output will be padded to a total of 128 bytes, with a 4 byte CRC.
"""
import argparse
import logging
import sys
from importlib.metadata import version


# Not used in this file, but needed to make pyinstaller import this
# There likely exists some better way of telling pyinstaller to do this,
# but I could not get the most obvious candidate of --hidden-import to work
# pylint: disable-next=unused-import
import lief

from crc_tool.input_file_handlers.elf_handler import ElfHandler
from crc_tool.input_file_handlers.txt_handler import TxtHandler


arg_parser = argparse.ArgumentParser(
    prog="CRC Tool",
    description="Insert CRC into ELF file, or generate binary file for programming user record",
)

arg_parser.add_argument("--version", action="version", version=version("ti-simplelink-crc-tool"))

subparsers = arg_parser.add_subparsers(
    help="One of two use cases: patch-image for modifying an ELF file, "
        + "generate-user-record to generate user record binary file",
    dest="use_case",
    required=True,
)

#########################################
### Common options for both use cases ###
#########################################

common_parser = argparse.ArgumentParser(add_help=False)

logging_level = common_parser.add_mutually_exclusive_group()

logging_level.add_argument(
    "--quiet", action="store_true", help="Suppress all output except for fatal errors"
)

logging_level.add_argument(
    "--verbose", action="store_true", help="Output all available logging"
)


#######################
##### USE CASE 1 ######
#######################

input_image_parser = subparsers.add_parser(
    "patch-image", help="Parse ELF file and insert CRC values", parents=[common_parser]
)

image_type_group = input_image_parser.add_argument_group("Required named arguments")

image_type_group.add_argument(
    "--elf",
    dest="input_image",
    type=argparse.FileType("rb"),
    required=True,
)

input_image_parser.add_argument(
    "-o",
    "--output",
    dest="output_file",
    help="Specify output file to write ELF with CRC to",
    default="patched.out",
    type=str,
)

input_image_parser.add_argument(
    "-p",
    "--symbol-prefix",
    dest="symbol_prefix",
    help="Specify prefix of symbols used to define CRC sections in the ELF file",
    default="__crc_section",
    type=str,
)

#######################
##### USE CASE 2 ######
#######################

input_text_parser = subparsers.add_parser(
    "generate-user-record",
    help="""Generate binary file from hex values, padded to 124 bytes,
    plus 4 bytes of CRC added.
    Output is used for programming CCFG user record""",
    parents=[common_parser],
)

text_type_group = input_text_parser.add_argument_group("Required named arguments")

text_type_group.add_argument(
    "--user-record-file",
    dest="user_record_file",
    help="""Text file containing hex data you want inserted binary for generating user record.

    Needs to contain a one or more hex values separated by whitespace,
    with or without 0x prefix, with a total length of 124 bytes.
    """,
    required=True,
    type=argparse.FileType("r"),
)

input_text_parser.add_argument(
    "-o",
    "--output",
    dest="output_file",
    help="Specify output file to write binary with CRC to",
    default="user_record.bin",
    type=str,
)


def app(args=None):
    """Entrypoint to application."""
    args = arg_parser.parse_args(args)

    if args.quiet:
        logging.getLogger().setLevel(logging.ERROR)
    elif args.verbose:
        logging.getLogger().setLevel(logging.DEBUG)
    else:
        logging.getLogger().setLevel(logging.INFO)
    if args.use_case == "patch-image":
        elf_handler = ElfHandler(args.input_image)
        symbol_pair_map = elf_handler.get_crc_symbol_pair_map(args.symbol_prefix)
        crc_was_inserted = elf_handler.insert_crc_by_symbol_pair_map(symbol_pair_map)
        if not crc_was_inserted:
            logging.warning("No CRC was inserted into file")

        elf_handler.write(args.output_file)

    elif args.use_case == "generate-user-record":
        integers = []
        for line in args.user_record_file:
            stripped = line.strip()
            # Not blank and not comment
            if stripped and not stripped.startswith("#"):
                # Split on whitespace
                integers += stripped.split()
        txt_handler = TxtHandler(integers)
        txt_handler.write(args.output_file)
    else:
        logging.error(
            "Invalid subcommand stated, please use one of {patch-image, generate-user-record}"
        )


if __name__ == "__main__":
    # Print help text when no arguments
    # Need it here instead of in app() to make it not occur
    # for some of our integration tests
    if len(sys.argv) == 1:
        arg_parser.print_help(sys.stderr)
        sys.exit(0)
    app()
