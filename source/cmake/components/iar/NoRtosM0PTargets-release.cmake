#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "NoRtos::nortos_cc23x0r2" for configuration "Release"
set_property(TARGET NoRtos::nortos_cc23x0r2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(NoRtos::nortos_cc23x0r2 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/kernel/nortos/lib/iar/m0p/nortos_cc23x0r2.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS NoRtos::nortos_cc23x0r2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_NoRtos::nortos_cc23x0r2 "${_IMPORT_PREFIX}/kernel/nortos/lib/iar/m0p/nortos_cc23x0r2.a" )

# Import target "NoRtos::nortos_cc23x0r5" for configuration "Release"
set_property(TARGET NoRtos::nortos_cc23x0r5 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(NoRtos::nortos_cc23x0r5 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/kernel/nortos/lib/iar/m0p/nortos_cc23x0r5.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS NoRtos::nortos_cc23x0r5 )
list(APPEND _IMPORT_CHECK_FILES_FOR_NoRtos::nortos_cc23x0r5 "${_IMPORT_PREFIX}/kernel/nortos/lib/iar/m0p/nortos_cc23x0r5.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
