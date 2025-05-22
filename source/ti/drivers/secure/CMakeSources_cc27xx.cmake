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
    ../cryptoutils/hsm/HSMLPF3.c
    ../cryptoutils/hsm/HSMLPF3Utility.c
    ../cryptoutils/sharedresources/CryptoResourceLPF3.c
    ../cryptoutils/utils/CryptoUtils.c
    ../ECDH.c
    ../ECDSA.c
    # ../ECJPAKE.c
    ../EDDSA.c
    ../RNG.c
    ../SHA2.c
    ../TRNG.c
)

set(SOURCES_TFM_NS_API
    ${TFM_INSTALL_DIR}/source/third_party/tfm/interface/src/tfm_tz_psa_ns_api.c
    ${TFM_INSTALL_DIR}/source/third_party/tfm/interface/src/os_wrapper/tfm_ns_interface_bare_metal.c
    # ${TFM_INSTALL_DIR}/source/third_party/tfm/interface/src/tfm_attest_api.c
)

set(SOURCES_SEC_ONLY_CC27XX
    ${SOURCES_CC27XX}
    ../aescbc/AESCBCLPF3.c
    ../aesccm/AESCCMLPF3.c
    ../aescmac/AESCMACLPF3.c
    ../aesctr/AESCTRLPF3.c
    ../aesecb/AESECBLPF3.c
    ../aesgcm/AESGCMLPF3HSM.c
    ../cryptoutils/aes/AESCommonLPF3.c
    ../cryptoutils/cryptokey/CryptoKeyKeyStore_PSA_helpers.c
    ../cryptoutils/cryptokey/CryptoKeyKeyStore_PSA.c
    ../cryptoutils/ecc/ECCParamsLPF3HSM.c
    ../ecdh/ECDHLPF3HSM.c
    ../ecdsa/ECDSALPF3HSM.c
    # ../ecjpake/ECJPAKECC26X2.c
    ../eddsa/EDDSALPF3HSM.c
    ../rng/RNGLPF3HSM.c
    ../sha2/SHA2LPF3HSM.c
    ../trng/TRNGLPF3HSM.c
    ../utils/Random.c
)

set(SOURCES_NONSEC_CC27XX
    ${SOURCES_CC27XX}
    ${SOURCES_TFM_NS_API}
    # ../aescbc/AESCBCCC26X4_ns.c
    # ../aesccm/AESCCMCC26X4_ns.c
    # ../aescmac/AESCMACCC26X4_ns.c
    # ../aesctr/AESCTRCC26X4_ns.c
    # ../aesctrdrbg/AESCTRDRBGX4_ns.c
    # ../aesecb/AESECBCC26X4_ns.c
    # ../aesgcm/AESGCMCC26X4_ns.c
    # ../attestation/Attestation_PSA_ns.c
    # ../crypto/CryptoCC26X4_ns.c
    # ../cryptoutils/cryptokey/CryptoKeyKeyStore_PSA_ns.c
    # ../cryptoutils/cryptokey/CryptoKeyKeyStore_PSA.c
    # ../cryptoutils/ecc/ECCParamsCC26X4_ns.c
    # ../ecdh/ECDHCC26X4_ns.c
    # ../sha2/SHA2CC26X4_ns.c
    # ../trng/TRNGCC26X4_ns.c
)
