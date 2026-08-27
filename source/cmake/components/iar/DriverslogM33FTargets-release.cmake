#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Drivers::log_cc27xxx10" for configuration "Release"
set_property(TARGET Drivers::log_cc27xxx10 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Drivers::log_cc27xxx10 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/ti/log/lib/iar/m33f/log_cc27xxx10.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Drivers::log_cc27xxx10 )
list(APPEND _IMPORT_CHECK_FILES_FOR_Drivers::log_cc27xxx10 "${_IMPORT_PREFIX}/source/ti/log/lib/iar/m33f/log_cc27xxx10.a" )

# Import target "Drivers::log_cc27xxx20" for configuration "Release"
set_property(TARGET Drivers::log_cc27xxx20 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Drivers::log_cc27xxx20 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/ti/log/lib/iar/m33f/log_cc27xxx20.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Drivers::log_cc27xxx20 )
list(APPEND _IMPORT_CHECK_FILES_FOR_Drivers::log_cc27xxx20 "${_IMPORT_PREFIX}/source/ti/log/lib/iar/m33f/log_cc27xxx20.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
