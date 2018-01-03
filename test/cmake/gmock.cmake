set(GOOGLEMOCK_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/external/gtest/googlemock CACHE STRING "Google Mock source root")

include_directories(SYSTEM
    ${GOOGLEMOCK_ROOT}
    ${GOOGLEMOCK_ROOT}/include
    )

set(GOOGLEMOCK_SOURCES
    ${GOOGLEMOCK_ROOT}/src/gmock-all.cc
    ${GOOGLEMOCK_ROOT}/src/gmock_main.cc
    )

foreach(_source ${GOOGLEMOCK_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_library(gmock ${GOOGLEMOCK_SOURCES})