cmake_minimum_required(VERSION 3.21.3)

# Common RCL sources for all device families
set(SOURCES_COMMON
    # RCL Core
    LRF.c
    RCL.c
    RCL_Buffer.c
    RCL_Debug.c
    RCL_Gpio.c
    RCL_Profiling.c
    RCL_Scheduler.c
    # RCL feature control configuration
    RCL_Feature.c
)

# LPF3 specific sources
set(SOURCES_LPF3
    # HAL
    hal/RCLLPF3_Hal.c
    # LRF
    lrf/LRFLPF3.c
    lrf/LRFLPF3_FIFO.c
    lrf/LRFLPF3_Interface_Generic.c
    # Command Handlers
    handlers/adc_noise.c
    handlers/ble5.c
    handlers/generic.c
    handlers/ieee.c
    # Adc Noise Wrapper
    wrappers/RCL_AdcNoise.c
)

# Channel Sounding is not supported on CC23X0R2
# ble_cs.c is therefore added separately to device families with CS support
set(SOURCES_CC23X0R2
    ${SOURCES_COMMON}
    ${SOURCES_LPF3}
)

set(SOURCES_CC23X0R5
    ${SOURCES_COMMON}
    ${SOURCES_LPF3}
    handlers/ble_cs.c
)

set(SOURCES_CC23X1R10
    ${SOURCES_COMMON}
    ${SOURCES_LPF3}
    handlers/ble_cs.c
)

set(SOURCES_CC27XXX10
    ${SOURCES_COMMON}
    ${SOURCES_LPF3}
    handlers/ble_cs.c
)

set(SOURCES_CC27XXX20
    ${SOURCES_COMMON}
    ${SOURCES_LPF3}
    handlers/ble_cs.c
)
