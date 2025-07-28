#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ThirdPartySpiffs::spiffs_m3" for configuration "Release"
set_property(TARGET ThirdPartySpiffs::spiffs_m3 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ThirdPartySpiffs::spiffs_m3 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/third_party/spiffs/lib/iar/m3/spiffs.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ThirdPartySpiffs::spiffs_m3 )
list(APPEND _IMPORT_CHECK_FILES_FOR_ThirdPartySpiffs::spiffs_m3 "${_IMPORT_PREFIX}/source/third_party/spiffs/lib/iar/m3/spiffs.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
