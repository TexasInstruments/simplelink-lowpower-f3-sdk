cmake_minimum_required(VERSION 3.21.3)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_common.cmake)

set(SOURCES_CC283X_COMMON
    ${SOURCES_COMMON}
    dma/UDMALPF3.c
    GPIO.c
    gpio/GPIOLPF4.c
    power/PowerCC283X.c
    UART2.c
    uart2/UART2LPF3.c
)

set(SOURCES_CC283X ${SOURCES_CC283X_COMMON})
