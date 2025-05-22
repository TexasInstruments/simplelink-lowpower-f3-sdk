cmake_minimum_required(VERSION 3.21.3)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_common.cmake)

set(SOURCES_CC23X0_COMMON
    ${SOURCES_COMMON}
    ADC.c
    adc/ADCLPF3.c
    ADCBuf.c
    adcbuf/ADCBufLPF3.c
    batterymonitor/BatteryMonitorLPF3.c
    batterymonitor/BatMonSupportLPF3.c
    CAN.c
    can/TCAN455X.c
    can/common/CANMCAN.c
    ../../third_party/mcan/MCAN.c
    Comparator.c
    comparator/ComparatorLPF3LP.c
    dma/UDMALPF3.c
    GPIO.c
    gpio/GPIOLPF3.c
    I2C.c
    i2c/I2CLPF3.c
    I2CTarget.c
    i2ctarget/I2CTargetLPF3.c
    NVS.c
    nvs/NVSLPF3.c
    nvs/NVSRAM.c
    nvs/NVSSPI25X.c
    power/PowerCC23X0.c
    PWM.c
    pwm/PWMTimerLPF3.c
    SD.c
    sd/SDSPI.c
    SPI.c
    spi/SPILPF3DMA.c
    SDFatFS.c
    Temperature.c
    temperature/TemperatureLPF3.c
    timer/LGPTimerLPF3.c
    UART2.c
    uart2/UART2LPF3.c
    Watchdog.c
    watchdog/WatchdogLPF3.c
)

set(SOURCES_CC23X0R5 ${SOURCES_CC23X0_COMMON})

set(SOURCES_CC23X0R2 ${SOURCES_CC23X0_COMMON})
