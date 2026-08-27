cmake_minimum_required(VERSION 3.21.3)

set(SOURCES_CC27XX
    ../AESCBC.c
    ../AESCCM.c
    ../AESCMAC.c
    ../AESCTR.c
    ../AESCTRDRBG.c
    ../AESECB.c
    ../AESGCM.c
    ../cryptoutils/cryptokey/CryptoKey.c
    ../cryptoutils/cryptokey/CryptoKeyPlaintext.c
    ../cryptoutils/hsm/HSMXXF3Utility.c
    ../cryptoutils/sharedresources/CryptoResourceXXF3.c
    ../cryptoutils/utils/CryptoUtils.c
    ../ECDH.c
    ../ECDSA.c
    # ../ECJPAKE.c
    ../EDDSA.c
    ../psa/ti_psa_crypto_helper.c
    ../psa/ti_psa_crypto_wrapper.c
    ../RNG.c
    ../SHA2.c
    ../TRNG.c
)

set(SOURCES_TFM_NS_API
    ${TFM_INSTALL_DIR}/source/third_party/tfm/interface/src/tfm_tz_psa_ns_api.c
    ${TFM_INSTALL_DIR}/source/third_party/tfm/interface/src/os_wrapper/tfm_ns_interface_bare_metal.c
    ${TFM_INSTALL_DIR}/source/third_party/tfm/interface/src/tfm_crypto_api.c
    ${TFM_INSTALL_DIR}/source/third_party/tfm/interface/src/tfm_attest_api.c
)

set(SOURCES_SEC_ONLY_CC27XX
    ${SOURCES_CC27XX}
    ../aescbc/AESCBCXXF3.c
    ../aesccm/AESCCMXXF3.c
    ../aescmac/AESCMACXXF3.c
    ../aesctr/AESCTRXXF3.c
    ../aesecb/AESECBXXF3.c
    ../aesgcm/AESGCMXXF3HSM.c
    ../cryptoutils/aes/AESCommonXXF3.c
    ../cryptoutils/cryptokey/CryptoKeyKeyStore_PSA_helpers.c
    ../cryptoutils/cryptokey/CryptoKeyKeyStore_PSA.c
    ../cryptoutils/ecc/ECCParamsXXF3HSM.c
    ../cryptoutils/hsm/HSMXXF3.c
    ../ecdh/ECDHXXF3HSM.c
    ../ecdsa/ECDSAXXF3HSM.c
    ../eddsa/EDDSAXXF3HSM.c
    ../psa/ti_psa_crypto.c
    ../rng/RNGXXF3HSM.c
    ../sha2/SHA2XXF3HSM.c
    ../trng/TRNGXXF3HSM.c
    ../utils/Random.c
)

set(SOURCES_NONSEC_CC27XX ${SOURCES_CC27XX} ${SOURCES_TFM_NS_API} ../crypto/CryptoTFM_ns.c ../crypto/PSACrypto_ns.c
                          ../cryptoutils/ecc/ECCParamsTFM_ns.c ../cryptoutils/hsm/HSMXXF3_ns.c
)
