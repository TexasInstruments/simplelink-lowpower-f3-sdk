#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
# Copyright (c) 2024-2025, Texas Instruments Incorporated. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# config.cmake is the default build configuration for this platform.

# Platform-specific compile defines
add_compile_definitions(
    # Define for TI custom memory layout for region_defs.h
    TI_CUSTOM_MEMORY_LAYOUT

    # Defines to build TI drivers
    DeviceFamily_CC27XXX10
    TFM_ENABLED=1
    TFM_BUILD
)

# Platform-specific fixed configuration
set(CONFIG_TFM_USE_TRUSTZONE          ON )
set(TFM_MULTI_CORE_TOPOLOGY           OFF)
set(CONFIG_TFM_ENABLE_FP              ON    CACHE BOOL    "Enable/disable FP usage")

# TF-M's bundled secondary bootloader is not used.
set(BL2                               OFF   CACHE BOOL    "Whether to build BL2")

# Disable FPU lazy stacking until TIRTOS7 can support it.
set(CONFIG_TFM_LAZY_STACKING          OFF   CACHE BOOL    "Enable/disable lazy stacking")

# Enable MM-IOVEC to avoid overhead of copying IO buffers.
set(PSA_FRAMEWORK_HAS_MM_IOVEC        ON    CACHE BOOL    "Enable MM-IOVEC")

if(NOT TEST_S AND NOT TEST_NS)
    # Use platform specific manifest list.
    set(TFM_MANIFEST_LIST                 "${CMAKE_CURRENT_LIST_DIR}/manifest/tfm_manifest_list.yaml" CACHE PATH "Platform specific Secure Partition manifests list file")

    # Dummy provisioning cannot be used for production.
    set(TFM_DUMMY_PROVISIONING            OFF   CACHE BOOL    "Provision with dummy values. NOT to be used in production")

    # Disable TF-M Crypto partition as this platform implements a proprietary crypto partition.
    set(TFM_PARTITION_CRYPTO              OFF   CACHE BOOL    "Enable Crypto partition")

    # Disable TF-M built-in keys driver. TI's key storage driver is used instead
    # to support pre-provisioned key for both Secure-only and TF-M
    # implementations.
    set(CRYPTO_TFM_BUILTIN_KEYS_DRIVER    OFF   CACHE BOOL    "Whether to allow crypto service to store builtin keys. Without this, ALL builtin keys must be stored in a platform-specific location")

    # Disable default RoT PK. This platform does not implement functionality to read PK hash.
    set(PLATFORM_DEFAULT_ROTPK            OFF   CACHE BOOL    "Use default root of trust public key.")

    # Disable default NV seed. This platform uses a proprietary method to seed the RNG.
    set(PLATFORM_DEFAULT_NV_SEED          OFF   CACHE BOOL    "Use default NV seed implementation.")

    # Disable default OTP. This platform uses write/erase protected flash memory
    # to store OEM attestation configurable info and optional pre-provisioned
    # IAK. The HUK is stored within the HSM itself. If a derived IAK is used, it
    # is stored in the ITS. Protected Storage and associated NV counters are not
    # currently supported.
    set(PLATFORM_DEFAULT_OTP              OFF   CACHE BOOL    "Use trusted on-chip flash to implement OTP memory")
    set(PLATFORM_DEFAULT_OTP_WRITEABLE    OFF   CACHE BOOL    "Use OTP memory with write support")

    # Disable default crypto keys. This platform uses HSM DDK for all crypto keys.
    set(PLATFORM_DEFAULT_CRYPTO_KEYS      OFF   CACHE BOOL    "Use default crypto keys implementation.")

    # Disable default NV counters. NV counters are not currently supported.
    set(PLATFORM_DEFAULT_NV_COUNTERS      OFF   CACHE BOOL    "Use default nv counter implementation.")

    # Disable default provisioning. Use platform-specific provisioning within
    # the TI Crypto partition.
    set(PLATFORM_DEFAULT_PROVISIONING     OFF   CACHE BOOL    "Use default provisioning implementation")

    # Disable default attestation HAL. Use TI platform-specific attestation HAL.
    set(PLATFORM_DEFAULT_ATTEST_HAL       OFF   CACHE BOOL    "Use default attest hal implementation.")
endif()

if(BL2)
    message(FATAL_ERROR "Only TI's standalone secondary bootloader is supported.")
else()
    # TI's secondary bootloader specifies a header size of 256-bytes and trailer of 4KB.
    set(BL2_HEADER_SIZE         0x100       CACHE STRING    "BL2 Header size")
    set(BL2_TRAILER_SIZE        0x1000      CACHE STRING    "BL2 Trailer size")
endif()
