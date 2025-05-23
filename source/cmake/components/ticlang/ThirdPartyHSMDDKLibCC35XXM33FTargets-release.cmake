#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ThirdPartyHSMDDKLib::hsmddk_cc35xx" for configuration "Release"
set_property(TARGET ThirdPartyHSMDDKLib::hsmddk_cc35xx APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ThirdPartyHSMDDKLib::hsmddk_cc35xx PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/third_party/hsmddk/lib/ticlang/m33f/hsmddk_cc35xx.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ThirdPartyHSMDDKLib::hsmddk_cc35xx )
list(APPEND _IMPORT_CHECK_FILES_FOR_ThirdPartyHSMDDKLib::hsmddk_cc35xx "${_IMPORT_PREFIX}/source/third_party/hsmddk/lib/ticlang/m33f/hsmddk_cc35xx.a" )

# Import target "ThirdPartyHSMDDKLib::hsmddk_cc35xx_its" for configuration "Release"
set_property(TARGET ThirdPartyHSMDDKLib::hsmddk_cc35xx_its APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ThirdPartyHSMDDKLib::hsmddk_cc35xx_its PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/third_party/hsmddk/lib/ticlang/m33f/hsmddk_cc35xx_its.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ThirdPartyHSMDDKLib::hsmddk_cc35xx_its )
list(APPEND _IMPORT_CHECK_FILES_FOR_ThirdPartyHSMDDKLib::hsmddk_cc35xx_its "${_IMPORT_PREFIX}/source/third_party/hsmddk/lib/ticlang/m33f/hsmddk_cc35xx_its.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
