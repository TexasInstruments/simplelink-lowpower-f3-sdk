cmake_minimum_required(VERSION ${TI_MIN_CMAKE_VERSION})

file(GLOB_RECURSE SOURCES_INTEGRATION_ADAPTER_DRIVERINIT "include/Integration/Adapter_DriverInit/Driver/src/*.c")

file(GLOB_RECURSE SOURCES_INTEGRATION_ADAPTER_GENERIC "include/Integration/Adapter_Generic/*.c")

file(GLOB_RECURSE SOURCES_INTERGRATION_ADAPTER_PSA "include/Integration/Adapter_PSA/src/*.c")

file(GLOB_RECURSE SOURCES_INTEGRATION_ADAPTER_VEX "include/Integration/Adapter_VEX/src/*.c")

file(GLOB_RECURSE SOURCES_INTEGRATION_DRIVER_FRAMEWORK "include/Integration/DriverFramework/src/*.c")

set(SOURCES_INTEGRATION_HSMSAL
    include/Integration/HSMSAL/HSMSAL.c
)

set(SOURCES_COMMON_INTEGRATION
    ${SOURCES_INTEGRATION_ADAPTER_DRIVERINIT}
    ${SOURCES_INTEGRATION_ADAPTER_GENERIC}
    ${SOURCES_INTERGRATION_ADAPTER_PSA}
    ${SOURCES_INTEGRATION_ADAPTER_VEX}
    ${SOURCES_INTEGRATION_DRIVER_FRAMEWORK}
    ${SOURCES_INTEGRATION_HSMSAL}
)

set(SOURCES_KIT_EIP130
    include/Kit/EIP130/DomainHelper/src/eip130_domain_ecc_getcurve.c
    include/Kit/EIP130/MailboxControl/src/eip130.c
    include/Kit/EIP130/TokenHelper/src/eip130_token_common.c
)

set(SOURCES_KIT_EIP201
    include/Kit/EIP201/src/eip201.c
)

set(SOURCES_KIT_LOG
    include/Kit/Log/src/log.c
)

set(SOURCES_COMMON_KIT
    ${SOURCES_KIT_EIP130}
    ${SOURCES_KIT_EIP201}
    ${SOURCES_KIT_LOG}
)

set(SOURCES_COMMON
    ${SOURCES_COMMON_INTEGRATION}
    ${SOURCES_COMMON_KIT}
)