cmake_minimum_required(VERSION 3.21.3)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_common.cmake)

set(SOURCES_CC27XX
    ${SOURCES_COMMON}
    AESCBC.c
    # AESCCM.c
    # AESCMAC.c
    AESCTR.c
    AESCTRDRBG.c
    AESECB.c
    # AESGCM.c
    cryptoutils/cryptokey/CryptoKeyPlaintext.c
    cryptoutils/sharedresources/CryptoResourceLPF3.c
    cryptoutils/utils/CryptoUtils.c
    GPIO.c
    gpio/GPIOLPF3.c
    # ECJPAKE.c
    # EDDSA.c
    pwm/PWMTimerLPF3.c
    NVS.c
    nvs/NVSLPF3.c
    nvs/NVSRAM.c
    nvs/NVSSPI25X.c
    timer/LGPTimerLPF3.c
    SPI.c
    spi/SPILPF3DMA.c
    UART2.c
    uart2/UART2LPF3.c
    power/PowerCC27XX.c
    dma/UDMALPF3.c
    # watchdog/WatchdogCC26X4.c
)

set(SOURCES_TFM_NS_API ${TFM_INSTALL_DIR}/source/third_party/tfm/interface/src/tfm_psa_ns_api.c
                       ${TFM_INSTALL_DIR}/source/third_party/tfm/platform/ext/target/ti/common/tfm_ns_interface.c
)

set(SOURCES_SEC_ONLY_CC27XX
    ${SOURCES_CC27XX}
    power/PowerCC27XX.c
    aescbc/AESCBCLPF3.c
    # aesccm/AESCCMLPF3.c
    # aescmac/AESCMACLPF3.c
    aesctr/AESCTRLPF3.c
    aesctrdrbg/AESCTRDRBGXX.c
    aesecb/AESECBLPF3.c
    # aesgcm/AESGCMLPF3.c
    cryptoutils/aes/AESCommonLPF3.c
    # cryptoutils/ecc/ECCParamsCC26X2.c
    # ecdh/ECDHCC26X2.c
    # ecdsa/ECDSACC26X2.c
    # ecjpake/ECJPAKECC26X2.c
    # eddsa/EDDSACC26X2.c
    # power/PowerCC26X2_calibrateRCOSC_helpers.c
    # power/PowerCC26X2_helpers.c
    # sha2/SHA2CC26X2.c
    # trng/TRNGCC26XX.c
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
    # tfm/SecureCallback.c
    # trng/TRNGCC26X4_ns.c
    ${TFM_INSTALL_DIR}/source/third_party/tfm/interface/src/tfm_initial_attestation_ipc_api.c
)
