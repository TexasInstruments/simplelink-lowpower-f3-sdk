#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ThirdPartySpiffs::spiffs_m0p" for configuration "Release"
set_property(TARGET ThirdPartySpiffs::spiffs_m0p APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ThirdPartySpiffs::spiffs_m0p PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/third_party/spiffs/lib/ticlang/m0p/spiffs.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ThirdPartySpiffs::spiffs_m0p )
list(APPEND _IMPORT_CHECK_FILES_FOR_ThirdPartySpiffs::spiffs_m0p "${_IMPORT_PREFIX}/source/third_party/spiffs/lib/ticlang/m0p/spiffs.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
