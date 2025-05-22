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
get_install_dir(MBEDTLS_INSTALL_DIR)
get_install_dir(TFM_INSTALL_DIR)
get_install_dir(HSMDDK_INSTALL_DIR)

include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_common.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_cc13xx_cc26xx.cmake OPTIONAL)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_cc23x0.cmake OPTIONAL)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_cc27xx.cmake OPTIONAL)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_cc35xx.cmake OPTIONAL)

project(drivers LANGUAGES C ASM)

# Regular expressions used to split SUPPORTED_PLATFORMS into a per-family list.
# Lists are stringified on the form "cc13x1;cc35xx;...", so the resulting list
# should include everything until (but not including) the next semicolon (;),
# or when end of string ($) is reached.
set(LPF2_REGEX "cc(13|26).[1-4][^;$]*")
set(LPF3_REGEX "cc(23|27)[^;$]*")
set(WFF3_REGEX "cc35[^;$]*")

# For each item in SUPPORTED_PLATFORMS, add it to a family-specific list
string(REGEX MATCHALL "${LPF2_REGEX}" LPF2_PLATFORMS "${SUPPORTED_PLATFORMS}")
string(REGEX MATCHALL "${LPF3_REGEX}" LPF3_PLATFORMS "${SUPPORTED_PLATFORMS}")
string(REGEX MATCHALL "${WFF3_REGEX}" WFF3_PLATFORMS "${SUPPORTED_PLATFORMS}")

#
# Create LPF2 package
#
if(LPF2_PLATFORMS)
    ti_init_package(NAMESPACE Drivers PACKAGE_NAME LPF2 DEPENDS_ON DriversUtils ThirdPartyEccLib TiUtils TiUtilsJson)
    foreach(TI_PLATFORM ${LPF2_PLATFORMS})
        if(${TI_PLATFORM} MATCHES "cc(13|26).1")
            ti_add_package_dependency(DriverlibCC13X1CC26X1)
            ti_add_package_dependency(Tirtos7Generic)
            ti_add_library(
                drivers_${TI_PLATFORM}
                SOURCES ${SOURCES_CC13X1_CC26X1}
                ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
                # This is only needed for PDMCC26XX to find xdc/std.h
                LINK_PRIVATE Tirtos7Generic::tirtos7
            )

            ti_add_library(
                drivers_${TI_PLATFORM}_log
                SOURCES ${SOURCES_CC13X1_CC26X1}
                ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
                # This is only needed for PDMCC26XX to find xdc/std.h
                LINK_PRIVATE Tirtos7Generic::tirtos7
            )

            # Set global Log include switch for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE)
            # Set module-level Log include switches for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_Power=1)
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_UART2=1)

        elseif(${TI_PLATFORM} MATCHES "cc(13|26).2.7")
            ti_add_package_dependency(DriverlibCC13X2X7CC26X2X7)
            ti_add_package_dependency(Tirtos7Generic)
            ti_add_library(
                drivers_${TI_PLATFORM}
                SOURCES ${SOURCES_CC13X2_CC26X2}
                ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
                # This is only needed for PDMCC26XX to find xdc/std.h
                LINK_PRIVATE Tirtos7Generic::tirtos7
            )

            ti_add_library(
                drivers_${TI_PLATFORM}_log
                SOURCES ${SOURCES_CC13X2_CC26X2}
                ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
                # This is only needed for PDMCC26XX to find xdc/std.h
                LINK_PRIVATE Tirtos7Generic::tirtos7
            )

            # Set global Log include switch for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE)
            # Set module-level Log include switches for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_Power=1)
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_UART2=1)

        elseif(${TI_PLATFORM} MATCHES "cc(13|26).2")
            ti_add_package_dependency(DriverlibCC13X2CC26X2)
            ti_add_package_dependency(Tirtos7Generic)
            ti_add_library(
                drivers_${TI_PLATFORM}
                SOURCES ${SOURCES_CC13X2_CC26X2}
                ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
                # This is only needed for PDMCC26XX to find xdc/std.h
                LINK_PRIVATE Tirtos7Generic::tirtos7
            )

            ti_add_library(
                drivers_${TI_PLATFORM}_log
                SOURCES ${SOURCES_CC13X2_CC26X2}
                ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
                # This is only needed for PDMCC26XX to find xdc/std.h
                LINK_PRIVATE Tirtos7Generic::tirtos7
            )

            # Set global Log include switch for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE)
            # Set module-level Log include switches for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_Power=1)
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_UART2=1)

            # C26X3 is deliberately excluded from this regex, as the 26x4 libraries are good enough
        elseif(${TI_PLATFORM} MATCHES "cc(13|26).4")
            ti_add_package_dependency(DriverlibCC13X4CC26X3CC26X4)
            ti_add_package_dependency(Tirtos7Generic)
            ti_add_library(
                drivers_${TI_PLATFORM}
                SOURCES ${SOURCES_SEC_ONLY_CC13X4_CC26X3_CC26X4}
                ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
                # This is only needed for PDMCC26XX to find xdc/std.h
                LINK_PRIVATE Tirtos7Generic::tirtos7
            )

            ti_add_library(
                drivers_${TI_PLATFORM}_log
                SOURCES ${SOURCES_SEC_ONLY_CC13X4_CC26X3_CC26X4}
                ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
                # This is only needed for PDMCC26XX to find xdc/std.h
                LINK_PRIVATE Tirtos7Generic::tirtos7
            )

            # Set global Log include switch for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE)
            # Set module-level Log include switches for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_Power=1)
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_UART2=1)

            ti_add_library(
                drivers_${TI_PLATFORM}_ns
                SOURCES ${SOURCES_NONSEC_CC13X4_CC26X3_CC26X4}
                ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC
                    Driverlib::${TI_PLATFORM} DriversUtils::tfm_enabled
                    CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}} TiUtils::json_${ARCH_${TI_PLATFORM}}
                LINK_PRIVATE Tirtos7Generic::tirtos7
            )

            ti_add_library(
                drivers_${TI_PLATFORM}_ns_log
                SOURCES ${SOURCES_NONSEC_CC13X4_CC26X3_CC26X4}
                ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC
                    Driverlib::${TI_PLATFORM} DriversUtils::tfm_enabled
                    CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}} TiUtils::json_${ARCH_${TI_PLATFORM}}
                LINK_PRIVATE Tirtos7Generic::tirtos7
            )

            # Set global Log include switch for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_ns_log PRIVATE ti_log_Log_ENABLE)
            # Set module-level Log include switches for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_ns_log PRIVATE ti_log_Log_ENABLE_LogModule_Power=1)
            target_compile_definitions(drivers_${TI_PLATFORM}_ns_log PRIVATE ti_log_Log_ENABLE_LogModule_UART2=1)
        endif()
    endforeach()

    ti_export_package()
endif() # if(LPF2_PLATFORMS)

#
# Create LPF3 package
#
if(LPF3_PLATFORMS)
    ti_init_package(NAMESPACE Drivers PACKAGE_NAME LPF3 DEPENDS_ON DriversUtils TiUtils TiUtilsJson)
    foreach(TI_PLATFORM ${LPF3_PLATFORMS})
        if(${TI_PLATFORM} STREQUAL "cc23x0r5")
            ti_add_package_dependency(DriverlibCC23X0R5)
            ti_add_library(
                drivers_${TI_PLATFORM} SOURCES ${SOURCES_CC23X0R5} ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
            )

            ti_add_library(
                drivers_${TI_PLATFORM}_log SOURCES ${SOURCES_CC23X0R5} ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
            )

            # Set global Log include switch for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE)
            # Set module-level Log include switches for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_Power=1)
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_UART2=1)

        elseif(${TI_PLATFORM} STREQUAL "cc23x0r2")
            ti_add_package_dependency(DriverlibCC23X0R2)
            ti_add_library(
                drivers_${TI_PLATFORM} SOURCES ${SOURCES_CC23X0R2} ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
            )

            ti_add_library(
                drivers_${TI_PLATFORM}_log SOURCES ${SOURCES_CC23X0R2} ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
            )

            # Set global Log include switch for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE)
            # Set module-level Log include switches for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_Power=1)
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_UART2=1)

        elseif(${TI_PLATFORM} STREQUAL "cc27xx")
            ti_add_package_dependency(DriverlibCC27XX)
            ti_add_library(
                drivers_${TI_PLATFORM} SOURCES ${SOURCES_SEC_ONLY_CC27XX} ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
            )

            ti_add_library(
                drivers_${TI_PLATFORM}_log SOURCES ${SOURCES_SEC_ONLY_CC27XX} ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
            )

            # Set global Log include switch for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE)
            # Set module-level Log include switches for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_Power=1)
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_UART2=1)

            ti_add_library(
                drivers_${TI_PLATFORM}_ns SOURCES ${SOURCES_NONSEC_CC27XX} ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM}_ns CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}} DriversUtils::tfm_enabled
            )

            ti_add_library(
                drivers_${TI_PLATFORM}_ns_log SOURCES ${SOURCES_NONSEC_CC27XX} ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM}_ns CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}} DriversUtils::tfm_enabled
            )

            # Set global Log include switch for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_ns_log PRIVATE ti_log_Log_ENABLE)
            # Set module-level Log include switches for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_ns_log PRIVATE ti_log_Log_ENABLE_LogModule_Power=1)
            target_compile_definitions(drivers_${TI_PLATFORM}_ns_log PRIVATE ti_log_Log_ENABLE_LogModule_UART2=1)
        endif()
    endforeach()

    ti_export_package()
endif() # if(LPF3_PLATFORMS)

#
# Create WFF3 package
#
if(WFF3_PLATFORMS)
    ti_init_package(NAMESPACE Drivers PACKAGE_NAME WFF3 DEPENDS_ON DriversUtils TiUtils TiUtilsJson)
    foreach(TI_PLATFORM ${WFF3_PLATFORMS})
        if(${TI_PLATFORM} MATCHES "cc35")
            ti_add_package_dependency(DriverlibCC35XX)

            ti_add_library(
                drivers_${TI_PLATFORM} SOURCES ${SOURCES_SEC_ONLY_CC35XX} ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
            )

            ti_add_library(
                drivers_${TI_PLATFORM}_log SOURCES ${SOURCES_SEC_ONLY_CC35XX} ADD_SDK_INCLUDE_PATH source
                LINK_PUBLIC Driverlib::${TI_PLATFORM} CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
                            TiUtils::json_${ARCH_${TI_PLATFORM}}
            )

            # Set global Log include switch for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE)
            # Set module-level Log include switches for this library
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_Power=1)
            target_compile_definitions(drivers_${TI_PLATFORM}_log PRIVATE ti_log_Log_ENABLE_LogModule_UART2=1)

        endif()
    endforeach()

    ti_export_package()
endif() # if(WFF3_PLATFORMS)

#
# A couple of simple utilities used by the drivers to portably export compile options
#
ti_init_package(NAMESPACE DriversUtils)

ti_add_library(tfm_enabled INTERFACE ARCH all)
target_compile_definitions(
    tfm_enabled INTERFACE $<$<COMPILE_LANGUAGE:C>:TFM_ENABLED=1> $<$<COMPILE_LANGUAGE:C>:ITS_CREATE_FLASH_LAYOUT=1>
                          $<$<COMPILE_LANGUAGE:C>:ENABLE_KEY_STORAGE=1>
)

ti_export_package()
