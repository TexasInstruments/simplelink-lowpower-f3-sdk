include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources.cmake)

file(GLOB boards_cc23x0rx_headers *.h)

set(DEVICE_LIST "cc23x0r5;cc23x0r2")

foreach (DEVICE ${DEVICE_LIST})
    if (${DEVICE} STREQUAL "cc23x0r5")
        set(DEVICE_FAMILY_DEFINE "DeviceFamily_CC23X0R5=1")
        set(boards_cc23x0rx_sources ${SOURCES_CC23X0R5})
    elseif (${DEVICE} STREQUAL "cc23x0r2")
        set(DEVICE_FAMILY_DEFINE "DeviceFamily_CC23X0R2=1")
        set(boards_cc23x0rx_sources ${SOURCES_CC23X0R2})
    endif()

    # Define object library, can be included by top-level executable
    add_library(boards_${DEVICE}_objects OBJECT ${boards_cc23x0rx_sources} )

    # Define an interface library to handle the properties needed to use this
    add_library(boards_${DEVICE} INTERFACE)

    # If using board, need to include this directory
    target_include_directories(boards_${DEVICE} INTERFACE
        INTERFACE
            .
    )

    # If using board, need device family define
    target_compile_definitions(boards_${DEVICE}
        INTERFACE
            ${DEVICE_FAMILY_DEFINE}
        )

    # Attach properties to object files for compilation. Some may only need the
    # headers, and can link the interface only
    target_link_libraries(boards_${DEVICE}_objects boards_${DEVICE} freertos_interface_${DEVICE})

endforeach()


