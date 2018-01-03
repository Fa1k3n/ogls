cmake_minimum_required(VERSION 3.0)
project(ogls)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
SET(COVERAGE OFF CACHE BOOL "Coverage")

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})  
else()
	message(STATUS "glew not found")
endif()

include_directories(
	include/ogls
	src	
)

add_definitions(-DGL_GLEXT_PROTOTYPES)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(headers
	include/ogls/shader.hpp
	include/ogls/program.hpp
	include/ogls/colors.hpp
)

set(sources
   src/shader.cpp
   src/program.cpp
)

add_library(ogls STATIC ${sources} ${headers})
target_link_libraries(ogls  /usr/lib64/libGLEW.a)
### Basic test build
add_subdirectory(test)
