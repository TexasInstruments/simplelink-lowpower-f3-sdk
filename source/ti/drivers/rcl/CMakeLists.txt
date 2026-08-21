# Copyright (c) 2022-2026 Texas Instruments Incorporated
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

project(rcl LANGUAGES C ASM)
ti_init_package(NAMESPACE Rcl PACKAGE_TYPE COMPONENT)

foreach(TI_PLATFORM ${SUPPORTED_PLATFORMS})
    if(${TI_PLATFORM} STREQUAL "cc23x0r2")
        include(${CMAKE_SOURCE_DIR}/source/cmake/components/${TI_TOOLCHAIN_NAME}/LrfCC23X0R2Config.cmake)
        ti_add_package_dependency(DriverlibCC23X0R2)
        ti_add_package_dependency(LrfCC23X0R2)
        set(SOURCES ${SOURCES_CC23X0R2})
    elseif(${TI_PLATFORM} STREQUAL "cc23x0r5")
        include(${CMAKE_SOURCE_DIR}/source/cmake/components/${TI_TOOLCHAIN_NAME}/LrfCC23X0R5Config.cmake)
        ti_add_package_dependency(DriverlibCC23X0R5)
        ti_add_package_dependency(LrfCC23X0R5)
        set(SOURCES ${SOURCES_CC23X0R5})
    elseif(${TI_PLATFORM} STREQUAL "cc27xxx10")
        include(${CMAKE_SOURCE_DIR}/source/cmake/components/${TI_TOOLCHAIN_NAME}/LrfCC27XXX10Config.cmake)
        ti_add_package_dependency(DriverlibCC27XXX10)
        ti_add_package_dependency(LrfCC27XXX10)
        set(SOURCES ${SOURCES_CC27XXX10})
    elseif(${TI_PLATFORM} STREQUAL "cc27xxx20")
        include(${CMAKE_SOURCE_DIR}/source/cmake/components/${TI_TOOLCHAIN_NAME}/LrfCC27XXX20Config.cmake)
        ti_add_package_dependency(DriverlibCC27XXX20)
        ti_add_package_dependency(LrfCC27XXX20)
        set(SOURCES ${SOURCES_CC27XXX20})
    else()
        continue()
    endif()

    ti_add_library(
        rcl_${TI_PLATFORM}
        # Contents of the library. Include LRF binaries in library
        SOURCES ${SOURCES}
        # Adds source directory to install target, helps with SDK building
        ADD_SDK_INCLUDE_PATH source
        # This library has public link dependencies on Driverlib and the device
        LINK_PUBLIC Driverlib::${TI_PLATFORM}
                    Lrf::${TI_PLATFORM}
                    CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
    )

    # Adds second library containing logs
    # Todo add helper for doing this after LPRFMW-760 has been implemented
    ti_add_library(
        rcl_${TI_PLATFORM}_log
        # Contents of the library. Include LRF binaries in library
        SOURCES ${SOURCES}
        # Adds source directory to install target, helps with SDK building
        ADD_SDK_INCLUDE_PATH source
        # This library has public link dependencies on Driverlib and the device
        LINK_PUBLIC Driverlib::${TI_PLATFORM}
                    Lrf::${TI_PLATFORM}
                    CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
    )

    # Set global Log include switch for this library
    target_compile_definitions(rcl_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE)
    # Set module-level Log include switch for this library
    target_compile_definitions(rcl_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_RCL=1)
endforeach()

ti_export_package()
