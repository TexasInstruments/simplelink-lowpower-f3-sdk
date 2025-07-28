#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Drivers::drivers_cc283x" for configuration "Release"
set_property(TARGET Drivers::drivers_cc283x APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Drivers::drivers_cc283x PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/ti/drivers/lib/iar/m3/drivers_cc283x.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Drivers::drivers_cc283x )
list(APPEND _IMPORT_CHECK_FILES_FOR_Drivers::drivers_cc283x "${_IMPORT_PREFIX}/source/ti/drivers/lib/iar/m3/drivers_cc283x.a" )

# Import target "Drivers::drivers_cc283x_log" for configuration "Release"
set_property(TARGET Drivers::drivers_cc283x_log APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Drivers::drivers_cc283x_log PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/ti/drivers/lib/iar/m3/drivers_cc283x_log.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Drivers::drivers_cc283x_log )
list(APPEND _IMPORT_CHECK_FILES_FOR_Drivers::drivers_cc283x_log "${_IMPORT_PREFIX}/source/ti/drivers/lib/iar/m3/drivers_cc283x_log.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
