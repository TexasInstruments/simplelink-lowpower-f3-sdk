# Copyright (c) 2022-2025, Texas Instruments Incorporated
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# *  Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# *  Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# *  Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
cmake_minimum_required(VERSION ${TI_MIN_CMAKE_VERSION})

#
# Secure driverlib
#
# Fetch all .c files in driverlib/
file(GLOB driverlib_c_sources driverlib/*.c)
# Filter out veneers (these are not compiled in any driverlib build)
list(FILTER driverlib_c_sources EXCLUDE REGEX driverlib/.*_veneers.c)

set(driverlib_sources
    ${driverlib_c_sources}
    # ASM file depends on toolchain
    driverlib/aes_${TI_TOOLCHAIN_NAME}.s
)

set(startup_sources
    startup_files/ccfg.c startup_files/scfg.c startup_files/startup_${TI_TOOLCHAIN_NAME}.c
    linker_files/cc27xxx10.${TI_TOOLCHAIN_LINKER_FILE_EXTENSION}
    linker_files/cc27xxx74.${TI_TOOLCHAIN_LINKER_FILE_EXTENSION}
)

#
# Non-secure driverlib
#
# Default to the secure driverlib sources.
# *_veneers.c files are already excluded from the S sources.
# Secure only content, for example functions accessing secure peripherals, must
# be wrapped in #ifndef DRIVERLIB_NS.
set(driverlib_sources_ns ${driverlib_sources})

set(startup_sources_ns
    startup_files/ccfg.c # Owned by non-secure side, but only accessible by secure side
    startup_files/scfg.c # Owned by non-secure side, but only accessible by secure side
    startup_files/startup_${TI_TOOLCHAIN_NAME}.c linker_files/cc27xxx10_ns.${TI_TOOLCHAIN_LINKER_FILE_EXTENSION}
)
