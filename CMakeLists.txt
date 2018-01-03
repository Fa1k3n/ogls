cmake_minimum_required(VERSION 3.0)
project(ogls)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(COVERAGE OFF CACHE BOOL "Coverage")

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
	include
)

add_definitions(-DGL_GLEXT_PROTOTYPES)

set(headers
	include/shader.hpp
	include/program.hpp
	include/colors.hpp
)

set(sources
   src/shader.cpp
   src/program.cpp
   src/colors_priv.hpp
 )

add_library(ogls SHARED ${sources} ${headers})
set_property(TARGET ogls PROPERTY PUBLIC_HEADER ${headers})

target_link_libraries(ogls /usr/lib64/libGLEW.a)

install(
	TARGETS ogls
	LIBRARY DESTINATION lib
	PUBLIC_HEADER DESTINATION include/ogls
	)
### Basic test build
add_subdirectory(test)
