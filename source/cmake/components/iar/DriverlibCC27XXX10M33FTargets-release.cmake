#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Driverlib::cc27xxx10" for configuration "Release"
set_property(TARGET Driverlib::cc27xxx10 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Driverlib::cc27xxx10 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "ASM;C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/ti/devices/cc27xxx10/driverlib/lib/iar/driverlib.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Driverlib::cc27xxx10 )
list(APPEND _IMPORT_CHECK_FILES_FOR_Driverlib::cc27xxx10 "${_IMPORT_PREFIX}/source/ti/devices/cc27xxx10/driverlib/lib/iar/driverlib.a" )

# Import target "Driverlib::cc27xxx10_ns" for configuration "Release"
set_property(TARGET Driverlib::cc27xxx10_ns APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Driverlib::cc27xxx10_ns PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "ASM;C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/ti/devices/cc27xxx10/driverlib/lib/iar/driverlib_ns.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Driverlib::cc27xxx10_ns )
list(APPEND _IMPORT_CHECK_FILES_FOR_Driverlib::cc27xxx10_ns "${_IMPORT_PREFIX}/source/ti/devices/cc27xxx10/driverlib/lib/iar/driverlib_ns.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
