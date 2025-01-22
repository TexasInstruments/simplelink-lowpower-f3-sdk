# TI SimpleLink CRC Tool

The TI SimpleLink CRC Tool is created by Texas Instruments to support building FW 
images for the CC23xx and CC27xx device families.

This tool can be used to modify an ELF file by calculating a 4-byte CRC value
for a given memory region and inserting it into the next 4 bytes after the
end of the region.
Which flash regions to insert CRC values for is configurable.

The tool does also support generation of a binary image with data for the user record.
The user record is a specific region in CCFG for Texas Instruments CC23xx and CC27xx devices.

## Dependencies

The Python script relies on the external library [LIEF](https://pypi.org/project/lief/0.8.3/)

This libary is included in the packaged executable.

## Use case 1 - Adding CRC to ELF file

This use case is accessed through using the subcommand ``patch-image`` when invoking ``crc_tool``.

This use case will:

 1. Parse symbols with a given prefix from the stated ELF file.
 2. Use these symbols and their values to find sections to calculate CRC for.
 3. Insert CRCs in the next four bytes after the given end of the region.
 4. Write this new ELF file to disk.

You then need to specify the location of an input file on disk.
Currently the tool has only been tested with ELF files.
Other file types may be added in the future.

The following command line options are available in the ``patch-image`` use-case.

| Name                                 | Usage                                             | Required? | Default              |
| ------------------------------------ | ------------------------------------------------- | --------- | -------------------- |
| ``--elf {file location}``                | Specify location of input file                    | Yes       | N/A                  |
| ``-p {prefix} / --symbol-prefix {prefix}``                 | Specify prefix of begin / end symbols in ELF file | no        | ``__crc_section`` |
| ``-o {file_name} / --ouput {file_name}`` | Specify name of output file                       | No        | ``patched.out``         |
| ``--verbose``                          | Output all available logging information          | No         | N/A
| ``--quiet``                          | Output no logging information, only fatal errors    | No         | N/A


### Required modifications:

For ``crc_tool`` to know where to insert crc32 values
it needs to be able to find symbols in the ELF file which matches the stated
prefix. All symbols without a matching prefix are ignored.

Symbols need to come in pairs with matching names, except for ``_end``, and ``_begin`` suffixes.
Symbols without a suffix, or without a matching counterpart are ignored..

If you have the _end values of two sections which within the same 4 byte range,
meaning that they would overwrite eachother's CRCs, then an error is thrown.

The value of the _end symbol must be greater or equal to the _begin value, otherwise
an error is thrown.

In a ticlang linker file symbols can be added like so
(Note the prefix differing from above default value):

```c
__my_prefix_boot_cfg_begin = 0x4E020000;
__my_prefix_boot_cfg_end = 0x4E02000B;

__my_prefix_main_begin = 0x4E020010;
__my_prefix_main_end = 0x4E02074B;

__my_prefix_user_record_crc32_begin = 0x4E020750;
__my_prefix_user_record_crc32_end = 0x4E0207CB;

__my_prefix_debug_cfg_crc32_begin = 0x4E0207D0;
__my_prefix_debug_cfg_crc32_end = 0x4E0207FB;
```

The following example lists **invalid** symbols, all of these will be ignored:

```c
// Ignored because no corresponding _end value
__my_prefix_boot_cfg_begin = 0x4E020000;

// Ignored because of invalid suffix
__my_prefix_main_be = 0x4E020010;
__my_prefix_main_en = 0x4E02074B;

// Ignored because no corresponding _begin value
__my_prefix_user_record_crc32_end = 0x4E020750;

// Ignored because of no valid prefix (assuming __my_prefix is used)
debug_cfg_crc32_begin = 0x4E0207D0;
debug_cfg_crc32_end = 0x4E0207FB;
```

The following example lists **invalid** symbols, all of the following will cause
the program to throw an error.

```c
// Throw error because end is before beginning
__my_prefix_main_begin = 0x4E020010;
__my_prefix_main_end = 0x4E020000;

// Throws error because overlap1 and overlap2 would overwrite eachother's CRCs
__my_prefix_overlap1_begin = 0x4E020000;
__my_prefix_overlap1_end = 0x4E020101;

__my_prefix_overlap2_begin = 0x4E020000;
__my_prefix_overlap2_end = 0x4E02100;
```

### Example 1:

In order to overwrite an ELF file named ``empty.out`` with a file with CRCs inserted,
using the above symbols, the following cli invocation can be used:

```
crc_tool patch-image --elf empty.out --symbol-prefix __my_prefix -o empty.out
```

## Use case 2 - generate binary with crc:

This use case is accessed through using the subcommand "generate-bin" when invoking crc_tool.

Currently only text files are supported, using the ``--user-record-file`` flag.
Other file types or input formats may be defined in the future.

The following command line options are available in the generate-bin use-case.

| Name                                   | Usage                                             | Required? | Default              |
| -------------------------------------- | ------------------------------------------------- | --------- | -------------------- |
| ``--user-record-file {file_location}``                  | Specify location of input file                    | yes       | N/A                  |
| ``-o {file_name} / --output {file_name}``   | Specify name of output file                       | no        | ``user_section.bin``         |
| ``--verbose``                          | Output all available logging information          | No         | N/A
| ``--quiet``                          | Output no logging information, only fatal errors    | No         | N/A


### Input file format:

The input text file must contain one or more valid hex values (0x prefix is optional).
Separate integers are separated by whitespace or newlines.

All hex values must consist of an even number of hex characters.

1 is invalid, 01 is valid.

The total length of all values can be at most 124 bytes.

Comments can be added with #, and blank lines are ignored.

An example of a valid input file can be seen in user_record_example.txt.

This file is found in ``docs/example/generate_user_record/`` during development,
but is placed alongside the source files during packaging.

### Output file format:

The output file will be a 128 byte binary file, containing integers in the same order as the input data,
With the first integer at the lowest address.

The output data will be right-padded with zeros to a total of 124 bytes. After the 124 bytes of content
there will be four CRC bytes.

Each integer will be written to file using little endian.

### Example 1:

In order to write to a binary file named ``output.bin`` with input data and CRC,
the following cli invocation can be used:

```
crc_tool generate-bin --user-record-file input.txt -o output.bin
```

## Generic options

A ``--version`` flag is available to check what version of crc_tool you are using.

Using ``--version`` will override any other options stated.

This means that:
```
crc_tool --version generate-user-record --user-record-file user_record_example.txt
```

Will NOT generate a user record binary, it will only print the version number of crc_tool

NB. the ``--version`` flag is only available before stating a use-case.

This means that:

```
crc_tool --version
```

Is a valid invocation, while:
```
crc_tool generate-user-record --user-record-file user_record_example.txt --version
```

Will throw an error.
