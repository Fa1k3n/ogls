set(GOOGLEMOCK_ROOT gtest/googlemock CACHE STRING "Google Mock source root")

include_directories(SYSTEM
    ${CMAKE_CURRENT_SOURCE_DIR}/${GOOGLEMOCK_ROOT}
    ${CMAKE_CURRENT_SOURCE_DIR}/${GOOGLEMOCK_ROOT}/include
    )

set(GOOGLEMOCK_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/${GOOGLEMOCK_ROOT}/src/gmock-all.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/${GOOGLEMOCK_ROOT}/src/gmock_main.cc
    )

foreach(_source ${GOOGLEMOCK_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_library(gmock ${GOOGLEMOCK_SOURCES})