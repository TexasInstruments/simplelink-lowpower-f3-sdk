cmake_minimum_required(VERSION 3.21.3)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_common.cmake)

set(SOURCES_CC27XX
    ${SOURCES_COMMON}
    ADC.c
    adc/ADCLPF3.c
    ADCBuf.c
    adcbuf/ADCBufLPF3.c
    apu/APULPF3.c
    apu/APULPF3_FW.c
    CAN.c
    can/CANCC27XX.c
    can/common/CANMCAN.c
    ../../third_party/mcan/MCAN.c
    Comparator.c
    comparator/ComparatorLPF3LP.c
    cryptoutils/sharedresources/HSMResourceXXF3.c
    cryptoutils/sharedresources/CommonResourceXXF3.c
    dma/UDMALPF3.c
    GPIO.c
    gpio/GPIOLPF3.c
    I2C.c
    i2c/I2CLPF3.c
    I2CTarget.c
    i2ctarget/I2CTargetLPF3.c
    I2S.c
    i2s/I2SLPF3.c
    ITM.c
    itm/ITMLPF3.c
    NVS.c
    nvs/NVSRAM.c
    nvs/NVSSPI25X.c
    nvs/NVSLPF3.c
    PWM.c
    pwm/PWMTimerLPF3.c
    SD.c
    sd/SDSPI.c
    SDFatFS.c
    SPI.c
    spi/SPILPF3DMA.c
    Temperature.c
    timer/LGPTimerLPF3.c
    UART2.c
    uart2/UART2LPF3.c
    Watchdog.c
    watchdog/WatchdogLPF3.c
)

set(SOURCES_CC27XXX10 ${SOURCES_CC27XX} apu/APUCC27XXX10.c can/CANCC27XXX10.c)

set(SOURCES_CC27XXX20 ${SOURCES_CC27XX} apu/APUCC27XXX20.c can/CANCC27XXX20.c)

set(SOURCES_SEC_ONLY_CC27XX batterymonitor/BatteryMonitorLPF3.c batterymonitor/BatMonSupportLPF3.c nvs/NVSLPF3.c
                            power/PowerCC27XX.c power/PowerCC27XX_lfoscCompensation.c temperature/TemperatureLPF3.c
)

set(SOURCES_SEC_ONLY_CC27XXX10 ${SOURCES_CC27XXX10} ${SOURCES_SEC_ONLY_CC27XX})

set(SOURCES_SEC_ONLY_CC27XXX20 ${SOURCES_CC27XXX20} ${SOURCES_SEC_ONLY_CC27XX})

set(SOURCES_NONSEC_CC27XXX10 ${SOURCES_CC27XXX10} batterymonitor/BatteryMonitorLPF3_ns.c power/PowerCC27XX_ns.c
                             temperature/TemperatureLPF3_ns.c tfm/SecureCallback.c
)

set(SOURCES_NONSEC_CC27XXX20 ${SOURCES_CC27XXX20} batterymonitor/BatteryMonitorLPF3_ns.c power/PowerCC27XX_ns.c
                             temperature/TemperatureLPF3_ns.c tfm/SecureCallback.c
)
