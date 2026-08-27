#-------------------------------------------------------------------------------
# Copyright (c) 2024-2025, Texas Instruments Incorporated. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

# Allow the target_link_libraries() command to be called from any directory to
# add link dependencies and link interface libraries to targets created in other
# directories
cmake_policy(SET CMP0079 NEW)

# Set to "ON" to enable PSA crypto APIs to be accessed by this partition and by
# other secure partitions via PSA calls (IPC model). Required to support
# attestation.
set (ENABLE_PSA_CRYPTO_API ON)

# Key storage is required - setting this variable to "OFF" is for debug purposes only.
set (ENABLE_KEY_STORAGE ON)

if (ENABLE_KEY_STORAGE)
    # Internal Trusted Storage (ITS) local integration is required to support
    # Trusted Direct Call (TDC) crypto functions which utilize secure key
    # storage since the SPM is bypassed and partition boundaries cannot be
    # crossed.
    set (TI_CRYPTO_ITS_INTEGRATION    ON    CACHE BOOL  "Enable ITS integration into TI Crypto partition")
endif()

if (TI_CRYPTO_ITS_INTEGRATION)
    # Set to "ON" to enable ITS functions to be accessed by NS code or other secure
    # partitions via PSA calls (IPC model). Requires TI_CRYPTO_ITS_INTEGRATION.
    set (ENABLE_ITS_IPC_INTEGRATION OFF)
endif()

add_library(ti_tfm_psa_rot_partition_crypto STATIC)

add_dependencies(ti_tfm_psa_rot_partition_crypto manifest_tool)

if (TI_CRYPTO_ITS_INTEGRATION)
    add_library(ti_tfm_crypto_its STATIC)

    # Add defines to tfm_config allow integration of ITS into TI Crypto partition
    target_compile_definitions(tfm_config
        INTERFACE
            TFM_PARTITION_INTERNAL_TRUSTED_STORAGE
            TI_CRYPTO_ITS_INTEGRATION
    )

    # Add ITS source files to TI Crypto partition target
    set(ITS_PARTITION_SOURCE_DIR ${CMAKE_SOURCE_DIR}/secure_fw/partitions/internal_trusted_storage)
    target_sources(ti_tfm_crypto_its
        PRIVATE
            tfm_its_local_api.c
            ${ITS_PARTITION_SOURCE_DIR}/tfm_internal_trusted_storage.c
            ${ITS_PARTITION_SOURCE_DIR}/its_utils.c
            $<$<BOOL:${ITS_ENCRYPTION}>:${ITS_PARTITION_SOURCE_DIR}/its_crypto_interface.c>
            ${ITS_PARTITION_SOURCE_DIR}/flash/its_flash.c
            ${ITS_PARTITION_SOURCE_DIR}/flash/its_flash_nand.c
            ${ITS_PARTITION_SOURCE_DIR}/flash/its_flash_nor.c
            ${ITS_PARTITION_SOURCE_DIR}/flash/its_flash_ram.c
            ${ITS_PARTITION_SOURCE_DIR}/flash_fs/its_flash_fs.c
            ${ITS_PARTITION_SOURCE_DIR}/flash_fs/its_flash_fs_dblock.c
            ${ITS_PARTITION_SOURCE_DIR}/flash_fs/its_flash_fs_mblock.c
    )

    target_link_libraries(ti_tfm_crypto_its
        PRIVATE
            platform_s
            tfm_config
            tfm_sprt
    )

    # Reference source/third_party/tfm/cmake/install.cmake
    set(INTERFACE_INC_DIR ${CMAKE_SOURCE_DIR}/interface/include)
    set(INSTALL_INTERFACE_INC_DIR interface/include)
    install(FILES       ${INTERFACE_INC_DIR}/psa/internal_trusted_storage.h
                        ${INTERFACE_INC_DIR}/psa/storage_common.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa)
    # tfm_its_defs.h is not installed because SFN model not being used
endif()

target_sources(ti_tfm_psa_rot_partition_crypto
    PRIVATE
        # Power and Secure Callback drivers
        ${TIDRIVERS_INSTALL_DIR}/source/ti/drivers/power/PowerCC27XX_s.c
        ${TIDRIVERS_INSTALL_DIR}/source/ti/drivers/system/SystemCC27XX_s.c
        ${TIDRIVERS_INSTALL_DIR}/source/ti/drivers/tfm/SecureCallback_s.c
        ${TIDRIVERS_INSTALL_DIR}/source/ti/drivers/utils/List.c

        # Main function for partition
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/crypto/PSACrypto_s.c

        # Driver configuration
        ti_drivers_config.c

        # Driver Porting Layer
        ${SECURE_DRIVERS_INSTALL_DIR}/kernel/tfm/dpl/HwiP_tfm.c
        ${SECURE_DRIVERS_INSTALL_DIR}/kernel/tfm/dpl/SemaphoreP_tfm.c
        ${SECURE_DRIVERS_INSTALL_DIR}/kernel/tfm/dpl/SwiP_tfm.c

        # Secure callback
        ${TIDRIVERS_INSTALL_DIR}/source/ti/drivers/tfm/SecureCallback_s.c

        # Crypto resources
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/cryptoutils/sharedresources/CryptoResourceXXF3.c
        ${TIDRIVERS_INSTALL_DIR}/source/ti/drivers/cryptoutils/sharedresources/HSMResourceXXF3.c
        ${TIDRIVERS_INSTALL_DIR}/source/ti/drivers/cryptoutils/sharedresources/CommonResourceXXF3.c

        # Key Store
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA_helpers.c

        # HSM
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/cryptoutils/hsm/HSMXXF3.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/cryptoutils/hsm/HSMXXF3Utility.c

        # AES drivers (Crypto HW accelerator)
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/cryptoutils/aes/AESCommonXXF3.c

        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/AESCBC.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/aescbc/AESCBCXXF3.c

        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/AESCCM.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/aesccm/AESCCMXXF3.c

        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/AESCMAC.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/aescmac/AESCMACXXF3.c

        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/AESCTR.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/aesctr/AESCTRXXF3.c

        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/AESECB.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/aesecb/AESECBXXF3.c

        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/AESGCM.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/aesgcm/AESGCMXXF3HSM.c

        # ECC drivers (Crypto HW accelerator)
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/cryptoutils/ecc/ECCParamsXXF3HSM.c

        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/ECDH.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/ecdh/ECDHXXF3HSM.c

        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/ECDSA.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/ecdsa/ECDSAXXF3HSM.c

        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/EDDSA.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/eddsa/EDDSAXXF3HSM.c

        # SHA2 driver (Crypto HW accelerator)
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/SHA2.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/sha2/SHA2XXF3HSM.c

        # Random number generation drivers (Crypto HW accelerator)
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/RNG.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/rng/RNGXXF3HSM.c

        # Common crypto utils
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/cryptoutils/cryptokey/CryptoKey.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/cryptoutils/cryptokey/CryptoKeyPlaintext.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/cryptoutils/utils/CryptoUtils.c

    PUBLIC
        # Source files containing only veneers
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/cryptoutils/hsm/HSMXXF3_s.c
        ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/cryptoutils/ecc/ECCParamsXXF3HSM_s.c
        ${TIDRIVERS_INSTALL_DIR}/source/ti/drivers/power/PowerCC27XX_veneers.c

)

if (ENABLE_PSA_CRYPTO_API)
    target_compile_definitions(ti_tfm_psa_rot_partition_crypto
        PUBLIC
            MBEDTLS_CONFIG_FILE="${MBEDTLS_CONFIG_PATH}"
        PRIVATE
            TI_PSA_CRYPTO_API_WRAPPER
    )

    target_sources(ti_tfm_psa_rot_partition_crypto
        PRIVATE
            ${CMAKE_SOURCE_DIR}/secure_fw/partitions/crypto/crypto_aead.c
            ${CMAKE_SOURCE_DIR}/secure_fw/partitions/crypto/crypto_alloc.c
            ${CMAKE_SOURCE_DIR}/secure_fw/partitions/crypto/crypto_asymmetric.c
            ${CMAKE_SOURCE_DIR}/secure_fw/partitions/crypto/crypto_cipher.c
            ${CMAKE_SOURCE_DIR}/secure_fw/partitions/crypto/crypto_hash.c
            ${CMAKE_SOURCE_DIR}/secure_fw/partitions/crypto/crypto_init.c
            ${CMAKE_SOURCE_DIR}/secure_fw/partitions/crypto/crypto_key_derivation.c
            ${CMAKE_SOURCE_DIR}/secure_fw/partitions/crypto/crypto_key_management.c
            ${CMAKE_SOURCE_DIR}/secure_fw/partitions/crypto/crypto_library.c
            ${CMAKE_SOURCE_DIR}/secure_fw/partitions/crypto/crypto_mac.c
            ${CMAKE_SOURCE_DIR}/secure_fw/partitions/crypto/crypto_rng.c

            ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/psa/ti_psa_crypto.c
            ${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/psa/ti_psa_crypto_helper.c
    )
endif ()

# The generated sources
target_sources(ti_tfm_psa_rot_partition_crypto
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/ti_crypto/auto_generated/intermedia_crypto_sp.c
)
target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/ti_crypto/auto_generated/load_info_crypto_sp.c
)

# Set include directory
target_include_directories(ti_tfm_psa_rot_partition_crypto
    PUBLIC
        ${HSMDDK_INSTALL_DIR}/source
        $<$<BOOL:${ENABLE_PSA_CRYPTO_API}>:${MBEDTLS_INSTALL_DIR}/source>
        $<$<BOOL:${ENABLE_PSA_CRYPTO_API}>:${MBEDTLS_INSTALL_DIR}/source/third_party/mbedtls/include>
        $<$<BOOL:${ENABLE_PSA_CRYPTO_API}>:${SECURE_DRIVERS_INSTALL_DIR}/source/ti/drivers/psa>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_BINARY_DIR}/generated/syscfg/
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/ti_crypto
)

target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/ti_crypto
)

target_compile_definitions(ti_tfm_psa_rot_partition_crypto
    PRIVATE
        AES_HSM_ONLY_BUILD # Building AES drivers for HSM-only reduces secure image size
        $<$<BOOL:${ENABLE_ITS_IPC_INTEGRATION}>:ENABLE_ITS_IPC_INTEGRATION>
        $<$<BOOL:${ENABLE_KEY_STORAGE}>:ENABLE_KEY_STORAGE>
)

target_compile_options(ti_tfm_psa_rot_partition_crypto
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wno-unused-parameter>
        $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-parameter>
        $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-const-variable>
        $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-parameter>
)

# Add HSM DDK library
add_library(hsm_ddk STATIC IMPORTED)

if (CMAKE_C_COMPILER_ID MATCHES "ARMClang")
    set_target_properties(hsm_ddk PROPERTIES
        IMPORTED_LOCATION
            ${HSMDDK_INSTALL_DIR}/source/third_party/hsmddk/lib/ticlang/m33f/hsmddk_cc27xxx10.a
    )
elseif (CMAKE_C_COMPILER_ID MATCHES "GNU")
    set_target_properties(hsm_ddk PROPERTIES
        IMPORTED_LOCATION
            ${HSMDDK_INSTALL_DIR}/source/third_party/hsmddk/lib/gcc/m33f/hsmddk_cc27xxx10.a
    )
elseif (CMAKE_C_COMPILER_ID MATCHES "IAR")
    set_target_properties(hsm_ddk PROPERTIES
        IMPORTED_LOCATION
            ${HSMDDK_INSTALL_DIR}/source/third_party/hsmddk/lib/iar/m33f/hsmddk_cc27xxx10.a
    )
endif()

target_link_libraries(hsm_ddk INTERFACE ti_tfm_crypto_its)

target_link_libraries(ti_tfm_psa_rot_partition_crypto
    PRIVATE
        psa_interface
        platform_s
        $<$<BOOL:${TI_CRYPTO_ITS_INTEGRATION}>:ti_tfm_crypto_its>
        tfm_config
        tfm_sprt

        # Driverlib library
        $<$<C_COMPILER_ID:ARMClang>:${DRIVERLIB_SOURCE_DIR}/ti/devices/cc27xxx10/driverlib/lib/ticlang/driverlib.a>
        $<$<C_COMPILER_ID:GNU>:${DRIVERLIB_SOURCE_DIR}/ti/devices/cc27xxx10/driverlib/lib/gcc/driverlib.a>
        $<$<C_COMPILER_ID:IAR>:${DRIVERLIB_SOURCE_DIR}/ti/devices/cc27xxx10/driverlib/lib/iar/driverlib.a>

        # HSM DDK library
        hsm_ddk
)

################################ TFM SPRT #######################################

# Add PSA API and ITS interface to Secure Partition Runtime library to allow
# other SPs to access via PSA calls.
target_sources(tfm_sprt
    PRIVATE
        $<$<BOOL:${ENABLE_PSA_CRYPTO_API}>:${CMAKE_SOURCE_DIR}/interface/src/tfm_crypto_api.c>
        $<$<BOOL:${ENABLE_ITS_IPC_INTEGRATION}>:${CMAKE_SOURCE_DIR}/interface/src/tfm_its_api.c>
        ${CMAKE_SOURCE_DIR}/interface/src/tfm_psa_call.c
)

################################ TFM SPM #######################################

target_sources(tfm_spm
    PRIVATE
        crypto_sp_interrupts.c
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        ti_tfm_psa_rot_partition_crypto
)

############################### TI SysConfig ###################################

# Custom target and command to generate ti_drivers_config.c/.h
# set(SYSCFG_DIR ${CMAKE_CURRENT_LIST_DIR}/syscfg)
# set(METADATA_PRODUCTS --product ${TIDRIVERS_INSTALL_DIR}/.metadata/product.json --product ${SECURE_DRIVERS_INSTALL_DIR}/.metadata/product.json --product ${TIUTILS_INSTALL_DIR}/.metadata/product.json)
# set(SYSCONFIG_ARGS -d CC2745R10RHAQ1 -o syscfg -c ticlang ${METADATA_PRODUCTS} ${SYSCFG_DIR}/crypto_sp.syscfg)
# add_custom_target(ti_sysconfig)
# add_custom_command(TARGET ti_sysconfig
#                    PRE_BUILD
#                    COMMAND ${SYSCONFIG_TOOL} ${SYSCONFIG_ARGS}
#                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/generated
#                    DEPENDS ${SYSCFG_DIR}/crypto_sp.syscfg
#                    BYPRODUCTS ${CMAKE_BINARY_DIR}/generated/syscfg/ti_drivers_config.c
#                               ${CMAKE_BINARY_DIR}/generated/syscfg/ti_drivers_config.h
#                    COMMENT "Running SysConfig to generate ti_drivers_config.c/.h"
#                    VERBATIM)
