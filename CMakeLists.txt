cmake_minimum_required(VERSION 3.0)
project(ogls)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(COVERAGE OFF CACHE BOOL "Coverage")
 
set(ogls_MAJOR_VERSION 0)
set(ogls_MINOR_VERSION 1)
set(ogls_PATCH_VERSION 0)
set(ogls_VERSION
  ${ogls_MAJOR_VERSION}.${ogls_MINOR_VERSION}.${ogls_PATCH_VERSION})

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR cmake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/cmake/ogls)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

if (COVERAGE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})  
else()
	message(STATUS "glew not found")
endif()

include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_BINARY_DIR}/include
)

add_definitions(-DGL_GLEXT_PROTOTYPES)

set(headers
	include/shader.hpp
	include/program.hpp
	include/colors.hpp
  include/exception.hpp
)

set(sources
   src/shader.cpp
   src/program.cpp
   src/exception.cpp
   src/uniforms.cpp
   src/colors_priv.hpp
 )

add_library(ogls SHARED ${sources} ${headers})
set_property(TARGET ogls PROPERTY PUBLIC_HEADER ${headers})

target_link_libraries(ogls /usr/lib64/libGLEW.a)

find_package(Doxygen)

if(DOXYGEN_FOUND)

set(DOXYGEN_INPUT ${PROJECT_SOURCE_DIR}/docs/doxy.conf)
set(DOXYGEN_OUTPUT ${PROJECT_BINARY_DIR}/docs/html)

message(STATUS "Docs output ${DOXYGEN_OUTPUT}")

add_custom_command(
  OUTPUT ${DOXYGEN_OUTPUT}
  COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
  COMMAND ${CMAKE_COMMAND} -E echo "Done."
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${DOXYGEN_INPUT}
  )

add_custom_target(apidoc ALL DEPENDS ${DOXYGEN_OUTPUT})

add_custom_target(apidoc_forced
  COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
  COMMAND ${CMAKE_COMMAND} -E echo "Done."
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

endif(DOXYGEN_FOUND)

export(TARGETS ogls FILE 
	"${PROJECT_BINARY_DIR}/oglsTargets.cmake")
export(PACKAGE ogls)

# Create the oglsConfig.cmake and oglsConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}/include")
configure_file(cmake/oglsConfig.cmake.in
  "${PROJECT_BINARY_DIR}/oglsConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${ogls_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(cmake/oglsConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/oglsConfig.cmake" @ONLY)
# ... for both
configure_file(cmake/oglsConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/oglsConfigVersion.cmake" @ONLY)

# Install the oglsConfig.cmake and oglsConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/oglsConfig.cmake"
  "${PROJECT_BINARY_DIR}/oglsConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
 
# Install the export set for use with the install-tree
install(EXPORT oglsTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)

install(
	TARGETS ogls
	EXPORT oglsTargets
	LIBRARY DESTINATION lib
	PUBLIC_HEADER DESTINATION include/ogls
	)

### Basic test build
add_subdirectory(test)
