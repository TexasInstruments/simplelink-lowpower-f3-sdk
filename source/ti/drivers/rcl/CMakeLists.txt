cmake_minimum_required(VERSION 3.21.3)

find_program(SRF_CLI_EXE cli_hw_mock PATHS $ENV{SRFSTUDIO_INSTALL_DIR}/bin REQUIRED)

set(DEVICE_LIST "cc23x0r5;cc23x0r2")

foreach (DEVICE ${DEVICE_LIST})
    if (${DEVICE} STREQUAL "cc23x0r5")
        set(DEVICE_FAMILY_DEFINE "DeviceFamily_CC23X0R5=1")
    elseif (${DEVICE} STREQUAL "cc23x0r2")
        set(DEVICE_FAMILY_DEFINE "DeviceFamily_CC23X0R2=1")
    endif()

    file(GLOB sources
        *.c
        handlers/ble5.c
        handlers/generic.c
        handlers/adc_noise.c
        hal/cc23x0rx/hal_cc23x0rx.c
        wrappers/RCL_AdcNoise.c
    )

    list(
        APPEND
        RFCONFIG_PHYS
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_adc_noise.c
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_adc_noise.h
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_ble_cs_hp.c
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_ble_cs_hp.h
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_ble_generic.c
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_ble_generic.h
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_ble.c
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_ble.h
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_fsk_20_kbps.c
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_fsk_20_kbps.h
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_gfsk_1_mbps.c
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_gfsk_1_mbps.h
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_gfsk_2_mbps.c
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_gfsk_2_mbps.h
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_gfsk_100_kbps.c
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_gfsk_100_kbps.h
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_gfsk_250_kbps_hidev.c
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_gfsk_250_kbps_hidev.h
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_gfsk_250_kbps.c
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_gfsk_250_kbps.h
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_gfsk_500_kbps.c
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_gfsk_500_kbps.h
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_msk_250_kbps.c
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_msk_250_kbps.h
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_msk_500_kbps.c
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_msk_500_kbps.h
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_nesb.c
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_nesb.h
    )

    file(GLOB existing_settings
        ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings*
    )
    if(existing_settings)
        file(REMOVE ${existing_settings})
    endif()

    # TODO: Currently RFCONFIG only supports CC2340R5. So we are currently using that for CC2340R2 too. This is fine for now,
    # but this needs to be handled on a per device basis.
    add_custom_command(
        OUTPUT ${RFCONFIG_PHYS}
        COMMAND
        ${CMAKE_COMMAND} -E env
        "LD_LIBRARY_PATH=$ENV{SRFSTUDIO_INSTALL_DIR}/lib"
        # Smart rf studio CLI
        ${SRF_CLI_EXE}
          --set-defs-dir "type=sdk_support_defs,dir=$ENV{RFCONFIG_DB_DIR}"
          # If $ENV{RFCONFIG_VERSION} == EDITABLE - then we are using a local package of rfconfig
          # This means we have no version number, and use EDITABLE instead
          # If not, then we can use the version number from our rfconfig version
          --create-workspace "device=CC2340R5,sdk=SimpleLink CC23x0 SDK:$ENV{RFCONFIG_VERSION},board=LP-EM-CC2340R5"
          ${CMAKE_CURRENT_SOURCE_DIR}/config_generation.srfcli
          # The file below selects phys, naming policy and whether to export all code or just registers
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
          DEPENDS config_generation.srfcli
        VERBATIM
      )

    # Include LRF binaries and local sources in library
    add_library(rcl_${DEVICE}
    ${sources}
    ${PROJECT_SOURCE_DIR}/source/ti/boards/cc23x0rx/rcl_settings_adc_noise.c
    $<TARGET_OBJECTS:lrf_${DEVICE}>
    )

    # No librcl, just rclXX
    set_target_properties(rcl_${DEVICE} PROPERTIES PREFIX "")

    # Get board definitions (mostly (only)) device family define
    target_link_libraries(rcl_${DEVICE} boards_${DEVICE})

    # In-source library delivery
    install(TARGETS rcl_${DEVICE} ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/${TC_NAME}/m0p)

endforeach()
