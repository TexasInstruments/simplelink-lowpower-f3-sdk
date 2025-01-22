cmake_minimum_required(VERSION ${TI_MIN_CMAKE_VERSION})
include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_common.cmake)

file(GLOB_RECURSE SOURCES_INTEGRATION_ADAPTER_ITS "include/Integration/Adapter_ITS/src/*.c")

set(SOURCES_CC27XX
    ${SOURCES_COMMON}
)

set(SOURCES_ITS_CC27XX
    ${SOURCES_COMMON}
    ${SOURCES_INTEGRATION_ADAPTER_ITS}
)