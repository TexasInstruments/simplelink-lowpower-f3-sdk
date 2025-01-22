# Copyright (c) 2022-2024, Texas Instruments Incorporated
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
include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources.cmake)

# All targets in this file are for Cortex-M33F
set(TI_ARCH m33f)

# For customer builds, this package is defined by platform imports, so the targets already exist
if(NOT TARGET Device::cc27xx)
    ti_init_package(NAMESPACE Device PACKAGE_NAME CC27XX PACKAGE_TYPE PLATFORM)

    # Add compile definition if the DRIVERLIB_DEBUG option is ON
    if(DRIVERLIB_DEBUG)
        add_compile_definitions(DRIVERLIB_DEBUG)
        message(STATUS "Option DRIVERLIB_DEBUG=ON: Debug asserts enabled (cc27xx)")
    endif(DRIVERLIB_DEBUG)

    # Meta-library for device, this causes DeviceFamily to be defined on downstream consumers
    ti_add_library(device_cc27xx INTERFACE EXPORT_NAME cc27xx LINK_INTERFACE CMakeCommon::${TI_TOOLCHAIN_NAME}_m33f)
    target_compile_definitions(device_cc27xx INTERFACE DeviceFamily_CC27XX)

    #
    # Secure
    #
    # Startup files get their own dedicated library in the same package
    ti_add_library(
        device_cc27xx_startup
        SOURCES ${startup_sources}
        # The resulting library will be named Device::cc27xx_startup
        EXPORT_NAME cc27xx_startup
        # Compile these files with the DeviceFamily from above
        LINK_PUBLIC
            Device::cc27xx
            # Emit the final files into a lib/ folder directly, with no architecture/tool component
            OUTPUT_FOLDER startup_files/lib/${TI_TOOLCHAIN_NAME}
    )
    if(${TI_TOOLCHAIN_NAME} MATCHES "ticlang|gcc")
        target_compile_options(device_cc27xx_startup PRIVATE -Wno-unused-function -Wno-unused-command-line-argument)
    endif()

    #
    # Non-Secure
    #
    # Startup files get their own dedicated library in the same package
    ti_add_library(
        device_cc27xx_startup_ns
        SOURCES ${startup_sources_ns}
        # The resulting library will be named Device::cc27xx_startup
        EXPORT_NAME cc27xx_startup_ns
        # Compile these files with the DeviceFamily from above
        LINK_PUBLIC
            Device::cc27xx
            # Emit the final files into a lib/ folder directly, with no architecture/tool component
            OUTPUT_FOLDER startup_files/lib/${TI_TOOLCHAIN_NAME}
    )
    if(${TI_TOOLCHAIN_NAME} MATCHES "ticlang|gcc")
        target_compile_options(device_cc27xx_startup_ns PRIVATE -Wno-unused-function -Wno-unused-command-line-argument)
    endif()

    # Public define (needed to build device_cc27xx_startup_ns, and by its consumers)
    target_compile_definitions(device_cc27xx_startup_ns PUBLIC DRIVERLIB_NS)

    ti_export_package()
endif()

# Driverlib package is a component
ti_init_package(NAMESPACE Driverlib PACKAGE_NAME CC27XX)

#
# Secure driverlib
#
ti_add_library(
    driverlib_cc27xx
    SOURCES ${driverlib_sources}
    # The resulting library will be named Driverlib::cc27xx
    EXPORT_NAME cc27xx
    OUTPUT_NAME driverlib
    # Pass on DeviceFamily from above
    LINK_PUBLIC
        Device::cc27xx
        # Emit the final files into a lib/ folder directly, with no architecture/tool component
        OUTPUT_FOLDER driverlib/lib/${TI_TOOLCHAIN_NAME}
    ADD_SDK_INCLUDE_PATH source
)

#
# Non-secure driverlib
#
ti_add_library(
    driverlib_cc27xx_ns
    SOURCES ${driverlib_sources_ns}
    # The resulting library will be named Driverlib::cc27xx_ns
    EXPORT_NAME cc27xx_ns
    OUTPUT_NAME driverlib_ns
    # Pass on DeviceFamily from above
    LINK_PUBLIC
        Device::cc27xx
        # Emit the final files into a lib/ folder directly, with no architecture/tool component
        OUTPUT_FOLDER driverlib/lib/${TI_TOOLCHAIN_NAME}
    ADD_SDK_INCLUDE_PATH source
)

# Public define (needed to build driverlib_ns, and by its consumers)
target_compile_definitions(driverlib_cc27xx_ns PUBLIC DRIVERLIB_NS)

ti_export_package()
