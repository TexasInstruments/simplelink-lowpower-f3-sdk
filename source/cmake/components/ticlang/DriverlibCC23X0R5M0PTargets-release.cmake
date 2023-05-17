#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Driverlib::cc23x0r5" for configuration "Release"
set_property(TARGET Driverlib::cc23x0r5 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Driverlib::cc23x0r5 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "ASM;C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/ti/devices/cc23x0r5/driverlib/lib/ticlang/driverlib.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Driverlib::cc23x0r5 )
list(APPEND _IMPORT_CHECK_FILES_FOR_Driverlib::cc23x0r5 "${_IMPORT_PREFIX}/source/ti/devices/cc23x0r5/driverlib/lib/ticlang/driverlib.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
