#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Device::cc27xx_startup" for configuration "Release"
set_property(TARGET Device::cc27xx_startup APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Device::cc27xx_startup PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/ti/devices/cc27xx/startup_files/lib/iar/device_cc27xx_startup.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Device::cc27xx_startup )
list(APPEND _IMPORT_CHECK_FILES_FOR_Device::cc27xx_startup "${_IMPORT_PREFIX}/source/ti/devices/cc27xx/startup_files/lib/iar/device_cc27xx_startup.a" )

# Import target "Device::cc27xx_startup_ns" for configuration "Release"
set_property(TARGET Device::cc27xx_startup_ns APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Device::cc27xx_startup_ns PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/source/ti/devices/cc27xx/startup_files/lib/iar/device_cc27xx_startup_ns.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Device::cc27xx_startup_ns )
list(APPEND _IMPORT_CHECK_FILES_FOR_Device::cc27xx_startup_ns "${_IMPORT_PREFIX}/source/ti/devices/cc27xx/startup_files/lib/iar/device_cc27xx_startup_ns.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
