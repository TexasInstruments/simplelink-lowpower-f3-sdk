#!/usr/bin/env python3
import os
import argparse
import binascii
import hashlib
import typing
import struct
import io
import time
from typing import Dict

from cryptography.hazmat.primitives import serialization

from cryptography.hazmat.primitives import hashes, _serialization
from cryptography.hazmat.primitives.asymmetric import padding, rsa
from cryptography.hazmat.primitives.asymmetric.types import PRIVATE_KEY_TYPES, PUBLIC_KEY_TYPES


class bcolors:
    """helper enum class providing color codes"""

    HEADER = "\033[95m"
    OKBLUE = "\033[94m"
    OKCYAN = "\033[96m"
    OKGREEN = "\033[92m"
    WARNING = "\033[93m"
    FAIL = "\033[91m"
    ENDC = "\033[0m"
    BOLD = "\033[1m"
    UNDERLINE = "\033[4m"


IMAGE_ROLLBACK_REV_OFFSET = 0x33C


def get_keys(key_file: argparse.FileType, key_format: str) -> typing.Tuple[PRIVATE_KEY_TYPES, PUBLIC_KEY_TYPES]:

    # Read the key_file bytes
    key_bytes = key_file.read()

    try:
        # Parse the key_file based on the key_format
        if key_format == "DER":
            private_key = serialization.load_der_private_key(key_bytes, password=None)
        else:
            private_key = serialization.load_pem_private_key(key_bytes, password=None)
        public_key = private_key.public_key()
    except:
        print(
            f"{bcolors.FAIL}{bcolors.BOLD}Failure:{bcolors.ENDC} Could not parse the input key information. Maybe the --key_format is wrong?\n"
        )
        exit(-1)

    return (private_key, public_key)


def dump_key_hash(key_file: argparse.FileType, key_format: str) -> None:

    # Get the public key(ignore the private key, we don't need it)
    _, public_key = get_keys(key_file, key_format)

    # Get the modulus and exponent integers from the public key
    param_numbers = public_key.public_numbers()
    key_mod_bytes = int(param_numbers.n).to_bytes(length=384, byteorder="little")  # Note: little endianess
    key_exp_bytes = int(param_numbers.e).to_bytes(length=4, byteorder="little")  # Note: little endianess

    # Hash the integers and display to the user
    key_digest = hashlib.sha256(key_mod_bytes + key_exp_bytes).digest()
    print(f"{bcolors.OKGREEN}{bcolors.BOLD}Done:{bcolors.ENDC} public_key_hash={binascii.hexlify(key_digest)}\n")
    print("Copy the value between the quotes(``) into your application's SysConfig HSM Update public Key hash field.\n")


def gen_keys() -> None:

    private_key = rsa.generate_private_key(public_exponent=65537, key_size=3072)

    time_str = time.strftime("%Y%m%d_%H%M%S")
    private_file_name = f"./private_{time_str}.PEM"
    public_file_name = f"./public_{time_str}.PEM"

    with open(private_file_name, "wb") as f:
        f.write(
            private_key.private_bytes(
                _serialization.Encoding.PEM,
                _serialization.PrivateFormat.TraditionalOpenSSL,
                _serialization.NoEncryption(),
            )
        )

    public_key = private_key.public_key()

    with open(public_file_name, "wb") as f:
        f.write(
            public_key.public_bytes(
                _serialization.Encoding.PEM,
                _serialization.PublicFormat.SubjectPublicKeyInfo,
            )
        )

    print(f"{bcolors.OKGREEN}{bcolors.BOLD}Done:{bcolors.ENDC} Created {private_file_name} and {public_file_name}")

    with open(private_file_name, "rb") as f:
        dump_key_hash(f, "PEM")


def pad_into_24_bit_format(data_bytes: list) -> list:
    n = 4
    data_bytes_padded = []
    for i in range(0, len(data_bytes), n - 1):
        data_bytes_padded.extend(data_bytes[i : i + n - 1])
        data_bytes_padded.append(0)
    data_bytes_padded.pop()
    data_bytes_padded.extend([0] * (n - len(data_bytes_padded) % n))

    return data_bytes_padded


def format_hui_bytes_from_private_key(
    key_file: argparse.FileType, key_format: str, image_file: argparse.FileType
) -> bytearray:

    # Get the private and public key pair
    private_key, public_key = get_keys(key_file, key_format)

    # Get the modulus and exponent integers from the public key
    param_numbers = public_key.public_numbers()
    key_mod_bytes = int(param_numbers.n).to_bytes(length=384, byteorder="little")  # Note: little endianess
    key_exp_bytes = int(param_numbers.e).to_bytes(length=4, byteorder="little")  # Note: little endianess

    image_bytes = image_file.read()

    msg_digest_byte_list = list()
    msg_digest_byte_list.extend(pad_into_24_bit_format(list(key_mod_bytes)))
    msg_digest_byte_list.extend(key_exp_bytes)
    msg_digest_byte_list.extend(b"FWr\0")
    msg_digest_byte_list.extend(struct.pack("<I", len(image_bytes)))
    msg_digest_byte_list.extend(image_bytes[IMAGE_ROLLBACK_REV_OFFSET : IMAGE_ROLLBACK_REV_OFFSET + 3] + b"\0")
    msg_digest_byte_list.extend(list(image_bytes))

    try:
        signature = private_key.sign(bytearray(msg_digest_byte_list), padding.PKCS1v15(), hashes.SHA256())
        public_key.verify(signature, bytearray(msg_digest_byte_list), padding.PKCS1v15(), hashes.SHA256())
    except:
        print(f"{bcolors.FAIL}{bcolors.BOLD}Failure:{bcolors.ENDC} Failed to generate a valid signature :(\n")
        exit(-1)

    hui_byte_list = list()
    hui_byte_list.extend(pad_into_24_bit_format(list(signature[::-1])))  # Note: little endianess (reversed)
    hui_byte_list.extend(msg_digest_byte_list)

    return bytearray(hui_byte_list)


def main(raw_args=None):
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    mode_group = parser.add_mutually_exclusive_group(required=True)
    mode_group.add_argument(
        "--sign",
        help="""
            Use this mode when you wish to wrap the input image with a signature
            calculated based on a provided private key.
        """,
        action="store_true",
    )
    mode_group.add_argument(
        "--gen_keys",
        help="""
            Use this mode when you wish to generate a new set of private/public
            RSA 3K keys. No signing operation is performed. Generates keys with
            the following format ./{private/public}_YYYYMMDD_HHMMSS.der.
        """,
        action="store_true",
    )
    mode_group.add_argument(
        "--dump_key_hash",
        help="""
            Use this mode to dump out the key hash which must be configured in
            the `SCFG.hsmCfg.publicKeyHash` field. No signing operation is
            performed.
        """,
        action="store_true",
    )
    parser.add_argument(
        "--key",
        help="""
            Input private key file used for signing. The default key format is
            PEM. Required when using the `sign` or `dump_key_hash` mode.
        """,
        metavar="KEY",
        type=argparse.FileType("rb"),
    )
    parser.add_argument(
        "--key_format",
        help="""
            Specify the format of the input private key. By default 'PEM' is
            used, but 'DER' can optionally be specified.
        """,
        choices=["PEM", "DER"],
        default="PEM",
    )
    parser.add_argument(
        "--input",
        help="""
            The input HSM FW image you wish to wrap with your signature and key
            information. Required when using the --sign mode.
        """,
        metavar="INPUT",
        type=argparse.FileType("rb"),
    )
    parser.add_argument(
        "--output",
        help="""
            Specify the output file to write the signed HUI image to. Defaults
            to using appending '_signed' onto the end of the input file name.
        """,
        metavar="OUTPUT",
        type=argparse.FileType("wb"),
    )

    args = parser.parse_args(raw_args)

    def input_failure(arg):
        print(
            f"{bcolors.FAIL}{bcolors.BOLD}Input Failure:{bcolors.ENDC} {bcolors.BOLD}--{arg}{bcolors.ENDC} is required.\n"
        )
        exit(-1)

    if args.dump_key_hash:
        if not args.key:
            input_failure("key")
        dump_key_hash(args.key, args.key_format)
    elif args.gen_keys:
        if args.key:
            print(
                f"{bcolors.WARNING}{bcolors.BOLD}Input Warning:{bcolors.ENDC} {bcolors.BOLD}--key{bcolors.ENDC} is being ignored.\n"
            )
        gen_keys()
    elif args.sign:
        if not args.input:
            input_failure("input")

        signed_image_bytes: bytearray = format_hui_bytes_from_private_key(args.key, args.key_format, args.input)

        # If no output was provided use the default
        if not args.output:
            name, ext = os.path.splitext(args.input.name)
            output_file_name = name + "_signed" + ext

        if isinstance(args.output, io.BufferedWriter):
            # Write binary output to already opened file handle
            args.output.write(signed_image_bytes)
        elif isinstance(args.output, io.TextIOWrapper):
            # Write hex string output to stdin
            args.output.write(signed_image_bytes.hex())
        else:
            # Write binary output to new file handle
            with open(output_file_name, "wb") as f:
                f.write(signed_image_bytes)


if __name__ == "__main__":
    main()
