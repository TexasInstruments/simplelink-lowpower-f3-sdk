#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ThirdPartyHSMDDKLib::hsmddk_cc27xx" for configuration "Release"
set_property(TARGET ThirdPartyHSMDDKLib::hsmddk_cc27xx APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ThirdPartyHSMDDKLib::hsmddk_cc27xx PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/third_party/hsmddk/lib/gcc/m33f/hsmddk.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ThirdPartyHSMDDKLib::hsmddk_cc27xx )
list(APPEND _IMPORT_CHECK_FILES_FOR_ThirdPartyHSMDDKLib::hsmddk_cc27xx "${_IMPORT_PREFIX}/source/third_party/hsmddk/lib/gcc/m33f/hsmddk.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
