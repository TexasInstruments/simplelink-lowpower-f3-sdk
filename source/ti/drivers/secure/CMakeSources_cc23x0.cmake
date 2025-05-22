cmake_minimum_required(VERSION 3.21.3)

set(SOURCES_CC23X0_COMMON
    ../../../third_party/bearssl/src/i32_add.c
    ../../../third_party/bearssl/src/i32_div32.c
    ../../../third_party/bearssl/src/i32_muladd.c
    ../../../third_party/bearssl/src/i32_reduce.c
    ../../../third_party/bearssl/src/i32_sub.c
    ../AESCBC.c
    ../aescbc/AESCBCLPF3.c
    ../AESCCM.c
    ../aesccm/AESCCMLPF3.c
    ../AESCMAC.c
    ../aescmac/AESCMACLPF3.c
    ../AESCTR.c
    ../aesctr/AESCTRLPF3.c
    ../AESCTRDRBG.c
    ../aesctrdrbg/AESCTRDRBGXX.c
    ../AESECB.c
    ../aesecb/AESECBLPF3.c
    ../AESGCM.c
    ../aesgcm/AESGCMLPF3.c
    ../ANSIX936KDF.c
    ../ansix936kdf/ANSIX936KDFXX.c
    ../cryptoutils/aes/AESCommonLPF3.c
    ../cryptoutils/cryptokey/CryptoKey.c
    ../cryptoutils/cryptokey/CryptoKeyPlaintext.c
    ../cryptoutils/ecc/ECCInitLPF3SW.c
    ../cryptoutils/ecc/ECCParamsLPF3SW.c
    ../cryptoutils/sharedresources/CryptoResourceLPF3.c
    ../cryptoutils/utils/CryptoUtils.c
    ../ECDH.c
    ../ecdh/ECDHLPF3SW.c
    ../ECDSA.c
    ../ecdsa/ECDSALPF3SW.c
    ../ECIES.c
    ../ecies/ECIESXX.c
    ../RNG.c
    ../rng/RNGLPF3RF.c
    ../SHA2.c
    ../sha2/SHA2LPF3SW.c
    ../utils/BigNum.c
    ../utils/Random.c
)

set(SOURCES_CC23X0R5 ${SOURCES_CC23X0_COMMON})

set(SOURCES_CC23X0R2 ${SOURCES_CC23X0_COMMON})
