#-------------------------------------------------------------------------------
# Copyright (c) 2020-2023, Arm Limited. All rights reserved.
# Copyright (c) 2023, Texas Instruments Incorporated. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(CMAKE_SYSTEM_NAME Generic)

find_program(CMAKE_C_COMPILER ${CROSS_COMPILE}-gcc)
find_program(CMAKE_CXX_COMPILER ${CROSS_COMPILE}-g++)

if(CMAKE_C_COMPILER STREQUAL "CMAKE_C_COMPILER-NOTFOUND")
    message(FATAL_ERROR "Could not find compiler: '${CROSS_COMPILE}-gcc'")
endif()

if(CMAKE_CXX_COMPILER STREQUAL "CMAKE_CXX_COMPILER-NOTFOUND")
    message(FATAL_ERROR "Could not find compiler: '${CROSS_COMPILE}-g++'")
endif()

set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})

set(LINKER_VENEER_OUTPUT_FLAG -Wl,--cmse-implib,--out-implib=)
set(COMPILER_CMSE_FLAG -mcmse)

# This variable name is a bit of a misnomer. The file it is set to is included
# at a particular step in the compiler initialisation. It is used here to
# configure the extensions for object files. Despite the name, it also works
# with the Ninja generator.
set(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_LIST_DIR}/cmake/set_extensions.cmake)

# CMAKE_C_COMPILER_VERSION is not guaranteed to be defined.
EXECUTE_PROCESS( COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION )

# ===================== SEt toolchain CPU and Arch =============================

if (DEFINED TFM_SYSTEM_PROCESSOR)
    if(TFM_SYSTEM_PROCESSOR MATCHES "cortex-m85" AND GCC_VERSION VERSION_LESS "13.0.0")
        # GNUARM until version 13 does not support the -mcpu=cortex-m85 flag
        message(WARNING "Cortex-m85 is only supported from GCC13. "
                        "Falling back to -march usage for earlier versions.")
    else()
        set(CMAKE_SYSTEM_PROCESSOR ${TFM_SYSTEM_PROCESSOR})

        if (DEFINED TFM_SYSTEM_DSP)
            if (NOT TFM_SYSTEM_DSP)
                string(APPEND CMAKE_SYSTEM_PROCESSOR "+nodsp")
            endif()
        endif()
        # GCC specifies that '+nofp' is available on following M-profile cpus: 'cortex-m4',
        # 'cortex-m7', 'cortex-m33', 'cortex-m35p' and 'cortex-m55'.
        # Build fails if other M-profile cpu, such as 'cortex-m23', is added with '+nofp'.
        # Explicitly list those cpu to align with GCC description.
        if(GCC_VERSION VERSION_GREATER_EQUAL "8.0.0")
            if(NOT CONFIG_TFM_ENABLE_FP AND
                (TFM_SYSTEM_PROCESSOR STREQUAL "cortex-m4"
                OR TFM_SYSTEM_PROCESSOR STREQUAL "cortex-m7"
                OR TFM_SYSTEM_PROCESSOR STREQUAL "cortex-m33"
                OR TFM_SYSTEM_PROCESSOR STREQUAL "cortex-m35p"
                OR TFM_SYSTEM_PROCESSOR STREQUAL "cortex-m55"))
                    string(APPEND CMAKE_SYSTEM_PROCESSOR "+nofp")
            endif()
        endif()

        if(TFM_SYSTEM_ARCHITECTURE STREQUAL "armv8.1-m.main")
            if(NOT CONFIG_TFM_ENABLE_MVE)
                string(APPEND CMAKE_SYSTEM_PROCESSOR "+nomve")
            endif()
            if(NOT CONFIG_TFM_ENABLE_MVE_FP)
                string(APPEND CMAKE_SYSTEM_PROCESSOR "+nomve.fp")
            endif()
        endif()
    endif()

endif()

# CMAKE_SYSTEM_ARCH variable is not a built-in CMAKE variable. It is used to
# set the compile and link flags when TFM_SYSTEM_PROCESSOR is not specified.
# The variable name is choosen to align with the ARMCLANG toolchain file.
set(CMAKE_SYSTEM_ARCH         ${TFM_SYSTEM_ARCHITECTURE})

if(TFM_SYSTEM_ARCHITECTURE STREQUAL "armv8.1-m.main")
    if(CONFIG_TFM_ENABLE_MVE)
        string(APPEND CMAKE_SYSTEM_ARCH "+mve")
    endif()
    if(CONFIG_TFM_ENABLE_MVE_FP)
        string(APPEND CMAKE_SYSTEM_ARCH "+mve.fp")
    endif()
endif()

if (DEFINED TFM_SYSTEM_DSP)
    # +nodsp modifier is only supported from GCC version 8.
    if(GCC_VERSION VERSION_GREATER_EQUAL "8.0.0")
        # armv8.1-m.main arch does not have +nodsp option
        if ((NOT TFM_SYSTEM_ARCHITECTURE STREQUAL "armv8.1-m.main") AND
            NOT TFM_SYSTEM_DSP)
            string(APPEND CMAKE_SYSTEM_ARCH "+nodsp")
        endif()
    endif()
endif()

if(GCC_VERSION VERSION_GREATER_EQUAL "8.0.0")
    if(CONFIG_TFM_ENABLE_FP)
        string(APPEND CMAKE_SYSTEM_ARCH "+fp")
    endif()
endif()

add_compile_options(
    -specs=nano.specs
    -Wall
    -Wno-format
    -Wno-return-type
    -Wno-unused-but-set-variable
    -c
    -fdata-sections
    -ffunction-sections
    -fno-builtin
    -fshort-enums
    -funsigned-char
    -mthumb
    -nostdlib
    $<$<COMPILE_LANGUAGE:C>:-std=c99>
    $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
    $<$<OR:$<BOOL:${TFM_DEBUG_SYMBOLS}>,$<BOOL:${TFM_CODE_COVERAGE}>>:-g>
)

# TI-TFM: Added "--nmagic" to the linker flags to disable page alignment of sections.
# TI-TFM: Added "-fno-lto" to the linker flags to disable LTO.
add_link_options(
    --entry=Reset_Handler
    -specs=nano.specs
    -fno-lto
    LINKER:--nmagic
    LINKER:-check-sections
    LINKER:-fatal-warnings
    LINKER:--gc-sections
    LINKER:--no-wchar-size-warning
)

if(NOT CONFIG_TFM_MEMORY_USAGE_QUIET)
    add_link_options(LINKER:--print-memory-usage)
endif()

if (GCC_VERSION VERSION_LESS 7.3.1)
    message(FATAL_ERROR "Please use newer GNU Arm compiler version starting from 7.3.1.")
endif()

if (GCC_VERSION VERSION_EQUAL 10.2.1)
    message(FATAL_ERROR "GNU Arm compiler version 10-2020-q4-major has an issue in CMSE support."
                        " Select other GNU Arm compiler versions instead."
                        " See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=99157 for the issue detail.")
endif()

# GNU Arm compiler version greater equal than *11.3.Rel1*
# has a linker issue that required system calls are missing,
# such as _read and _write. Add stub functions of required
# system calls to solve this issue.
if (GCC_VERSION VERSION_GREATER_EQUAL 11.3.1)
    set(CONFIG_GNU_SYSCALL_STUB_ENABLED TRUE)
endif()

if (CMAKE_SYSTEM_PROCESSOR)
    set(CMAKE_C_FLAGS_INIT "-mcpu=${CMAKE_SYSTEM_PROCESSOR}")
    set(CMAKE_CXX_FLAGS_INIT "-mcpu=${CMAKE_SYSTEM_PROCESSOR}")
    set(CMAKE_ASM_FLAGS_INIT "-mcpu=${CMAKE_SYSTEM_PROCESSOR}")
    set(CMAKE_C_LINK_FLAGS "-mcpu=${CMAKE_SYSTEM_PROCESSOR}")
    set(CMAKE_ASM_LINK_FLAGS "-mcpu=${CMAKE_SYSTEM_PROCESSOR}")
else()
    set(CMAKE_C_FLAGS_INIT "-march=${CMAKE_SYSTEM_ARCH}")
    set(CMAKE_CXX_FLAGS_INIT "-march=${CMAKE_SYSTEM_ARCH}")
    set(CMAKE_ASM_FLAGS_INIT "-march=${CMAKE_SYSTEM_ARCH}")
    set(CMAKE_C_LINK_FLAGS "-march=${CMAKE_SYSTEM_ARCH}")
    set(CMAKE_ASM_LINK_FLAGS "-march=${CMAKE_SYSTEM_ARCH}")
endif()

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS_INIT})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_INIT})
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS_INIT})

set(BL2_COMPILER_CP_FLAG -mfloat-abi=soft)

if (CONFIG_TFM_FLOAT_ABI STREQUAL "hard")
    set(COMPILER_CP_FLAG -mfloat-abi=hard)
    set(LINKER_CP_OPTION -mfloat-abi=hard)
    if (CONFIG_TFM_ENABLE_FP OR CONFIG_TFM_ENABLE_MVE_FP)
        set(COMPILER_CP_FLAG -mfloat-abi=hard -mfpu=${CONFIG_TFM_FP_ARCH})
        set(LINKER_CP_OPTION -mfloat-abi=hard -mfpu=${CONFIG_TFM_FP_ARCH})
    endif()
else()
    set(COMPILER_CP_FLAG -mfloat-abi=soft)
    set(LINKER_CP_OPTION -mfloat-abi=soft)
endif()

# For GNU Arm Embedded Toolchain doesn't emit __ARM_ARCH_8_1M_MAIN__, adding this macro manually.
add_compile_definitions($<$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv8.1-m.main>:__ARM_ARCH_8_1M_MAIN__>)

macro(target_add_scatter_file target)
    target_link_options(${target}
        PRIVATE
        -T $<TARGET_OBJECTS:${target}_scatter>
    )

    add_library(${target}_scatter OBJECT)
    foreach(scatter_file ${ARGN})
        target_sources(${target}_scatter
            PRIVATE
                ${scatter_file}
        )
        # Cmake cannot use generator expressions in the
        # set_source_file_properties command, so instead we just parse the regex
        # for the filename and set the property on all files, regardless of if
        # the generator expression would evaluate to true or not.
        string(REGEX REPLACE ".*>:(.*)>$" "\\1" SCATTER_FILE_PATH "${scatter_file}")
        set_source_files_properties(${SCATTER_FILE_PATH}
            PROPERTIES
            LANGUAGE C
            KEEP_EXTENSION True # Don't use .o extension for the preprocessed file
        )
    endforeach()

    add_dependencies(${target}
        ${target}_scatter
    )

    set_target_properties(${target} PROPERTIES LINK_DEPENDS $<TARGET_OBJECTS:${target}_scatter>)

    target_link_libraries(${target}_scatter
        platform_region_defs
        psa_interface
        tfm_config
    )

    target_compile_options(${target}_scatter
        PRIVATE
            -E
            -P
            -xc
    )
endmacro()

macro(add_convert_to_bin_target target)
    get_target_property(bin_dir ${target} RUNTIME_OUTPUT_DIRECTORY)

    add_custom_target(${target}_bin
        SOURCES ${bin_dir}/${target}.bin
    )
    add_custom_command(OUTPUT ${bin_dir}/${target}.bin
        DEPENDS ${target}
        COMMAND ${CMAKE_OBJCOPY}
            -O binary $<TARGET_FILE:${target}>
            ${bin_dir}/${target}.bin
    )

    add_custom_target(${target}_elf
        SOURCES ${bin_dir}/${target}.elf
    )
    add_custom_command(OUTPUT ${bin_dir}/${target}.elf
        DEPENDS ${target}
        COMMAND ${CMAKE_OBJCOPY}
            -O elf32-littlearm $<TARGET_FILE:${target}>
            ${bin_dir}/${target}.elf
    )

    add_custom_target(${target}_hex
        SOURCES ${bin_dir}/${target}.hex
    )
    add_custom_command(OUTPUT ${bin_dir}/${target}.hex
        DEPENDS ${target}
        COMMAND ${CMAKE_OBJCOPY}
            -O ihex $<TARGET_FILE:${target}>
            ${bin_dir}/${target}.hex
    )

    add_custom_target(${target}_binaries
        ALL
        DEPENDS ${target}_bin
        DEPENDS ${target}_elf
        DEPENDS ${target}_hex
    )
endmacro()

macro(target_share_symbols target symbol_name_file)
    get_target_property(TARGET_TYPE ${target} TYPE)
    if (NOT TARGET_TYPE STREQUAL "EXECUTABLE")
        message(FATAL_ERROR "${target} is not an executable. Symbols cannot be shared from libraries.")
    endif()

    FILE(STRINGS ${symbol_name_file} KEEP_SYMBOL_LIST
        LENGTH_MINIMUM 1
    )
    set(STRIP_SYMBOL_KEEP_LIST ${KEEP_SYMBOL_LIST})

    # Force the target to not remove the symbols if they're unused.
    list(TRANSFORM KEEP_SYMBOL_LIST PREPEND "-Wl,--undefined=")
    target_link_options(${target}
        PRIVATE
            ${KEEP_SYMBOL_LIST}
    )

    list(TRANSFORM STRIP_SYMBOL_KEEP_LIST PREPEND  --keep-symbol=)
    # strip all the symbols except those proveded as arguments
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY}
        ARGS $<TARGET_FILE:${target}> --wildcard ${STRIP_SYMBOL_KEEP_LIST} --strip-all $<TARGET_FILE_DIR:${target}>/${target}_shared_symbols.axf
    )
endmacro()

macro(target_link_shared_code target)
    foreach(symbol_provider ${ARGN})
        if (TARGET ${symbol_provider})
            get_target_property(SYMBOL_PROVIDER_TYPE ${symbol_provider} TYPE)
            if (NOT SYMBOL_PROVIDER_TYPE STREQUAL "EXECUTABLE")
                message(FATAL_ERROR "${symbol_provider} is not an executable. Symbols cannot be shared from libraries.")
            endif()
        endif()

        add_dependencies(${target} ${symbol_provider})
        target_link_options(${target} PRIVATE LINKER:-R$<TARGET_FILE_DIR:${symbol_provider}>/${symbol_provider}_shared_symbols.axf)
    endforeach()
endmacro()

macro(target_strip_symbols target)
    set(SYMBOL_LIST "${ARGN}")
    list(TRANSFORM SYMBOL_LIST PREPEND  --strip-symbol=)

    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY}
        ARGS $<TARGET_FILE:${target}> --wildcard ${SYMBOL_LIST} $<TARGET_FILE:${target}>
    )
endmacro()

macro(target_strip_symbols_from_dependency target dependency)
    set(SYMBOL_LIST "${ARGN}")
    list(TRANSFORM SYMBOL_LIST PREPEND  --strip-symbol=)

    add_custom_command(
        TARGET ${target}
        PRE_LINK
        COMMAND ${CMAKE_OBJCOPY}
        ARGS $<TARGET_FILE:${dependency}> --wildcard ${SYMBOL_LIST} $<TARGET_FILE:${dependency}>
    )
endmacro()

macro(target_weaken_symbols target)
    set(SYMBOL_LIST "${ARGN}")
    list(TRANSFORM SYMBOL_LIST PREPEND  --weaken-symbol=)

    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY}
        ARGS $<TARGET_FILE:${target}> --wildcard ${SYMBOL_LIST} $<TARGET_FILE:${target}>
    )
endmacro()

macro(target_weaken_symbols_from_dependency target dependency)
    set(SYMBOL_LIST "${ARGN}")
    list(TRANSFORM SYMBOL_LIST PREPEND  --weaken-symbol=)

    add_custom_command(
        TARGET ${target}
        PRE_LINK
        COMMAND ${CMAKE_OBJCOPY}
        ARGS $<TARGET_FILE:${dependency}> --wildcard ${SYMBOL_LIST} $<TARGET_FILE:${dependency}>
    )
endmacro()

# A dummy macro to align with Armclang workaround
macro(tfm_toolchain_reload_compiler)
endmacro()
