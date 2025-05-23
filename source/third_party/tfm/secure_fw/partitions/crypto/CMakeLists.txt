#-------------------------------------------------------------------------------
# Copyright (c) 2020-2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_PARTITION_CRYPTO)
    return()
endif()

find_package(Python3)

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_crypto STATIC)

add_dependencies(tfm_psa_rot_partition_crypto manifest_tool)

target_sources(tfm_psa_rot_partition_crypto
    PRIVATE
        crypto_init.c
        crypto_alloc.c
        crypto_cipher.c
        crypto_hash.c
        crypto_mac.c
        crypto_aead.c
        crypto_asymmetric.c
        crypto_key_derivation.c
        crypto_key_management.c
        crypto_rng.c
        crypto_library.c
        $<$<BOOL:${CRYPTO_TFM_BUILTIN_KEYS_DRIVER}>:psa_driver_api/tfm_builtin_key_loader.c>
)

# The generated sources
target_sources(tfm_psa_rot_partition_crypto
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/crypto/auto_generated/intermedia_tfm_crypto.c
)
target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/crypto/auto_generated/load_info_tfm_crypto.c
)

# Set include directory
target_include_directories(tfm_psa_rot_partition_crypto
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/crypto
)
target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/crypto
)

# Linking to external interfaces
target_link_libraries(tfm_psa_rot_partition_crypto
    PRIVATE
        platform_s
        crypto_service_mbedcrypto
        tfm_config
        tfm_sprt
)
target_compile_definitions(tfm_psa_rot_partition_crypto
    PUBLIC
        MBEDTLS_PSA_CRYPTO_DRIVERS
        $<$<BOOL:${CRYPTO_TFM_BUILTIN_KEYS_DRIVER}>:MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS PSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY>
    PRIVATE
        $<$<STREQUAL:${CRYPTO_HW_ACCELERATOR_TYPE},cc312>:CRYPTO_HW_ACCELERATOR_CC312>
        MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_crypto
)

target_compile_definitions(tfm_config
    INTERFACE
        TFM_PARTITION_CRYPTO
)

target_link_libraries(tfm_config
    INTERFACE
        psa_crypto_config
)

############################### PSA CRYPTO CONFIG ##############################
add_library(psa_crypto_config INTERFACE)
target_compile_definitions(psa_crypto_config
    INTERFACE
        MBEDTLS_PSA_CRYPTO_CONFIG_FILE="${TFM_MBEDCRYPTO_PSA_CRYPTO_CONFIG_PATH}"
)

############################### MBEDCRYPTO #####################################

add_library(crypto_service_mbedcrypto_config INTERFACE)

target_compile_definitions(crypto_service_mbedcrypto_config
    INTERFACE
        MBEDTLS_CONFIG_FILE="${TFM_MBEDCRYPTO_CONFIG_PATH}"
        $<$<BOOL:${TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH}>:MBEDTLS_USER_CONFIG_FILE="${TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH}">
        PSA_CRYPTO_SECURE
        # Workaround for https://github.com/ARMmbed/mbedtls/issues/1077
        $<$<OR:$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv8-m.base>,$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv6-m>>:MULADDC_CANNOT_USE_R7>
        $<$<BOOL:${PLATFORM_DEFAULT_NV_SEED}>:PLATFORM_DEFAULT_NV_SEED>
        $<$<BOOL:${PLATFORM_DEFAULT_CRYPTO_KEYS}>:PLATFORM_DEFAULT_CRYPTO_KEYS>
        MBEDTLS_PSA_CRYPTO_DRIVERS
        $<$<BOOL:${CRYPTO_TFM_BUILTIN_KEYS_DRIVER}>:MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS PSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER>
)

target_link_libraries(crypto_service_mbedcrypto_config
    INTERFACE
        tfm_config
        psa_crypto_config
)

cmake_policy(SET CMP0079 NEW)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)
set(ENABLE_TESTING OFF)
set(ENABLE_PROGRAMS OFF)
set(MBEDTLS_FATAL_WARNINGS OFF)
set(ENABLE_DOCS OFF)
set(INSTALL_MBEDTLS_HEADERS OFF)
set(LIB_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/mbedcrypto/install)
set(GEN_FILES OFF)

# Set the prefix to be used by mbedTLS targets
set(MBEDTLS_TARGET_PREFIX crypto_service_)
# CMake should be aware of the config files being used
# FixMe: comment these until the config files are cleaned up to be self-contained
#set(MBEDTLS_PSA_CRYPTO_CONFIG_FILE "${TFM_MBEDCRYPTO_PSA_CRYPTO_CONFIG_PATH}")
#set(MBEDTLS_CONFIG_FILE "${TFM_MBEDCRYPTO_CONFIG_PATH}")

# Check if the p256m driver is enabled in the config file, as that will require a
# dedicated target to be linked in. Note that 0 means SUCCESS here, 1 means FAILURE
set(MBEDTLS_P256M_NOT_FOUND 1)
execute_process(COMMAND
    ${Python3_EXECUTABLE}
    ${MBEDCRYPTO_PATH}/scripts/config.py -f "${TFM_MBEDCRYPTO_CONFIG_PATH}" get MBEDTLS_PSA_P256M_DRIVER_ENABLED
    RESULT_VARIABLE MBEDTLS_P256M_NOT_FOUND)

if (${MBEDTLS_P256M_NOT_FOUND} EQUAL 0)
    message(STATUS "[Crypto service] Using P256M software driver in PSA Crypto backend")
    set(MBEDTLS_P256M_ENABLED true)
else()
    set(MBEDTLS_P256M_ENABLED false)
endif()

# Mbedcrypto is quite a large lib, and it uses too much memory for it to be
# reasonable to build it in debug info. As a compromise, if `debug` build type
# is selected mbedcrypto will build under `relwithdebinfo` which preserved debug
# symbols whild optimizing space.
set(SAVED_BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE ${MBEDCRYPTO_BUILD_TYPE})
add_subdirectory(${MBEDCRYPTO_PATH} ${CMAKE_CURRENT_BINARY_DIR}/mbedcrypto EXCLUDE_FROM_ALL)
set(CMAKE_BUILD_TYPE ${SAVED_BUILD_TYPE} CACHE STRING "Build type: [Debug, Release, RelWithDebInfo, MinSizeRel]" FORCE)

if(NOT TARGET ${MBEDTLS_TARGET_PREFIX}mbedcrypto)
    message(FATAL_ERROR "[Crypto service] Target ${MBEDTLS_TARGET_PREFIX}mbedcrypto does not exist. Have the patches in ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto been applied to the mbedcrypto repo at ${MBEDCRYPTO_PATH} ?
    Hint: The command might be `cd ${MBEDCRYPTO_PATH} && git apply ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/*.patch`")
endif()

target_include_directories(${MBEDTLS_TARGET_PREFIX}mbedcrypto
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/psa_driver_api
)

# Fix platform_s and crypto_service_mbedcrypto libraries cyclic linking
set_target_properties(${MBEDTLS_TARGET_PREFIX}mbedcrypto PROPERTIES LINK_INTERFACE_MULTIPLICITY 3)

target_sources(${MBEDTLS_TARGET_PREFIX}mbedcrypto
    PRIVATE
        $<$<NOT:$<BOOL:${CRYPTO_HW_ACCELERATOR}>>:${CMAKE_CURRENT_SOURCE_DIR}/tfm_mbedcrypto_alt.c>
)

target_compile_options(${MBEDTLS_TARGET_PREFIX}mbedcrypto
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wno-unused-const-variable>
        $<$<C_COMPILER_ID:GNU>:-Wno-unused-parameter>
        $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-const-variable>
        $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-parameter>
)

target_compile_definitions(${MBEDTLS_TARGET_PREFIX}mbedcrypto
    PRIVATE
        MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER
)

# FixMe: The p256m CmakeLists.txt in version 3.5.0 has an issue with target
# names and for this reason we need to force those defines at this stage
target_compile_definitions(${MBEDTLS_TARGET_PREFIX}p256m
    PRIVATE
        MBEDTLS_PSA_P256M_DRIVER_ENABLED
        MBEDTLS_PSA_CRYPTO_SPM
)

# The crypto_spe.h to be passed to p256m is here
target_include_directories(${MBEDTLS_TARGET_PREFIX}p256m
    PRIVATE
        .
)

# TI-TFM: Added compiler flags for p256m target to ensure correct FPU support.
target_compile_options(${MBEDTLS_TARGET_PREFIX}p256m
    PRIVATE
        ${COMPILER_CP_FLAG}
)

target_link_libraries(${MBEDTLS_TARGET_PREFIX}mbedcrypto
    PRIVATE
        psa_interface
        platform_s
        $<$<BOOL:${MBEDTLS_P256M_ENABLED}>:${MBEDTLS_TARGET_PREFIX}p256m>
    PUBLIC
        crypto_service_mbedcrypto_config
    INTERFACE
        platform_common_interface
)
