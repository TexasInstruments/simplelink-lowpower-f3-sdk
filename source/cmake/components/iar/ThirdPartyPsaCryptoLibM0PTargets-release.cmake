#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ThirdPartyPsaCryptoLib::psa_crypto_cc23x1" for configuration "Release"
set_property(TARGET ThirdPartyPsaCryptoLib::psa_crypto_cc23x1 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ThirdPartyPsaCryptoLib::psa_crypto_cc23x1 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "ASM;C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/third_party/psa_crypto/lib/iar/m0p/psa_crypto_cc23x1.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ThirdPartyPsaCryptoLib::psa_crypto_cc23x1 )
list(APPEND _IMPORT_CHECK_FILES_FOR_ThirdPartyPsaCryptoLib::psa_crypto_cc23x1 "${_IMPORT_PREFIX}/source/third_party/psa_crypto/lib/iar/m0p/psa_crypto_cc23x1.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
