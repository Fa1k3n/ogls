set(GLMOCK_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/external/glmock CACHE STRING "GLMock source root")

include_directories(SYSTEM
    ${GLMOCK_ROOT}
    )

set(GOOGLEMOCK_SOURCES
    ${GLMOCK_ROOT}/glmock.cpp
    )

add_library(glmock ${GOOGLEMOCK_SOURCES})