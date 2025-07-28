#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Driverlib::cc283x" for configuration "Release"
set_property(TARGET Driverlib::cc283x APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Driverlib::cc283x PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/ti/devices/cc283x/driverlib/lib/gcc/driverlib.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Driverlib::cc283x )
list(APPEND _IMPORT_CHECK_FILES_FOR_Driverlib::cc283x "${_IMPORT_PREFIX}/source/ti/devices/cc283x/driverlib/lib/gcc/driverlib.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
