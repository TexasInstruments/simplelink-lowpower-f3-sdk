cmake_minimum_required(VERSION 3.21.3)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_common.cmake)

set(SOURCES_CC27XX
    ${SOURCES_COMMON}
    ADC.c
    adc/ADCLPF3.c
    ADCBuf.c
    adcbuf/ADCBufLPF3.c
    AESCBC.c
    AESCCM.c
    AESCMAC.c
    AESCTR.c
    AESCTRDRBG.c
    AESECB.c
    AESGCM.c
    apu/APULPF3.c
    batterymonitor/BatteryMonitorLPF3.c
    batterymonitor/BatMonSupportLPF3.c
    CAN.c
    can/CANCC27XX.c
    can/common/CANMCAN.c
    ../../third_party/mcan/MCAN.c
    cryptoutils/cryptokey/CryptoKey.c
    cryptoutils/cryptokey/CryptoKeyPlaintext.c
    cryptoutils/sharedresources/CryptoResourceLPF3.c
    cryptoutils/utils/CryptoUtils.c
    cryptoutils/hsm/HSMLPF3.c
    cryptoutils/hsm/HSMLPF3Utility.c
    Comparator.c
    comparator/ComparatorLPF3LP.c
    dma/UDMALPF3.c
    ECDH.c
    # ECJPAKE.c
    ECDSA.c
    # EDDSA.c
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
    nvs/NVSLPF3.c
    nvs/NVSRAM.c
    nvs/NVSSPI25X.c
    power/PowerCC27XX.c
    power/PowerCC27XX_lfoscCompensation.c
    PWM.c
    pwm/PWMTimerLPF3.c
    RNG.c
    SD.c
    sd/SDSPI.c
    SDFatFS.c
    SHA2.c
    SPI.c
    spi/SPILPF3DMA.c
    Temperature.c
    temperature/TemperatureLPF3.c
    timer/LGPTimerLPF3.c
    TRNG.c
    UART2.c
    uart2/UART2LPF3.c
    Watchdog.c
    watchdog/WatchdogLPF3.c
)

set(SOURCES_TFM_NS_API
    ${TFM_INSTALL_DIR}/source/third_party/tfm/interface/src/tfm_tz_psa_ns_api.c
    ${TFM_INSTALL_DIR}/source/third_party/tfm/interface/src/os_wrapper/tfm_ns_interface_bare_metal.c
    # ${TFM_INSTALL_DIR}/source/third_party/tfm/interface/src/tfm_attest_api.c
)

set(SOURCES_SEC_ONLY_CC27XX
    ${SOURCES_CC27XX}
    aescbc/AESCBCLPF3.c
    aesccm/AESCCMLPF3.c
    aescmac/AESCMACLPF3.c
    aesctr/AESCTRLPF3.c
    aesecb/AESECBLPF3.c
    aesgcm/AESGCMLPF3HSM.c
    cryptoutils/aes/AESCommonLPF3.c
    cryptoutils/ecc/ECCParamsLPF3HSM.c
    cryptoutils/cryptokey/CryptoKeyKeyStore_PSA_helpers.c
    cryptoutils/cryptokey/CryptoKeyKeyStore_PSA.c
    ecdh/ECDHLPF3HSM.c
    ecdsa/ECDSALPF3HSM.c
    # ecjpake/ECJPAKECC26X2.c
    # eddsa/EDDSACC26X2.c
    power/PowerCC27XX.c
    # power/PowerCC26X2_calibrateRCOSC_helpers.c
    # power/PowerCC26X2_helpers.c
    rng/RNGLPF3HSM.c
    sha2/SHA2LPF3HSM.c
    trng/TRNGLPF3HSM.c
)

set(SOURCES_NONSEC_CC27XX
    ${SOURCES_CC27XX}
    ${SOURCES_TFM_NS_API}
    # aescbc/AESCBCCC26X4_ns.c
    # aesccm/AESCCMCC26X4_ns.c
    # aescmac/AESCMACCC26X4_ns.c
    # aesctr/AESCTRCC26X4_ns.c
    # aesctrdrbg/AESCTRDRBGX4_ns.c
    # aesecb/AESECBCC26X4_ns.c
    # aesgcm/AESGCMCC26X4_ns.c
    # attestation/Attestation_PSA_ns.c
    # crypto/CryptoCC26X4_ns.c
    # cryptoutils/cryptokey/CryptoKeyKeyStore_PSA_ns.c
    # cryptoutils/cryptokey/CryptoKeyKeyStore_PSA.c
    # cryptoutils/ecc/ECCParamsCC26X4_ns.c
    # ecdh/ECDHCC26X4_ns.c
    # nvs/flash/FlashCC26X4_ns.c
    # sha2/SHA2CC26X4_ns.c
    tfm/SecureCallback.c
    # trng/TRNGCC26X4_ns.c
)
