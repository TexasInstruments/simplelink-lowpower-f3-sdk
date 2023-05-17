cmake_minimum_required(VERSION 3.21.3)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_common.cmake)

set(SOURCES_CC27XX
    ${SOURCES_COMMON}
    # AESGCM.c
    GPIO.c
    gpio/GPIOLPF3.c
    # cryptoutils/sharedresources/PKAResourceCC26XX.c
    # ECJPAKE.c
    # EDDSA.c
    # spi/SPICC26X4DMA.c
    # watchdog/WatchdogCC26X4.c
)

set(SOURCES_TFM_NS_API ${TFM_INSTALL_DIR}/source/third_party/tfm/interface/src/tfm_psa_ns_api.c
                       ${TFM_INSTALL_DIR}/source/third_party/tfm/platform/ext/target/ti/common/tfm_ns_interface.c
)

set(SOURCES_SEC_ONLY_CC27XX
    ${SOURCES_CC27XX}
    power/PowerCC27XX.c
    # aescbc/AESCBCCC26XX.c
    # aesccm/AESCCMCC26X4.c
    # aescmac/AESCMACCC26XX.c
    # aesctr/AESCTRCC26XX.c
    # aesctrdrbg/AESCTRDRBGXX.c
    # aesecb/AESECBCC26XX.c
    # aesgcm/AESGCMCC26X4.c
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
