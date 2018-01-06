set(ogls_EXTERNAL_PROJECTS_DIR "${CMAKE_BINARY_DIR}/test/external")

include(cmake/googletest.cmake)
include(cmake/glmock.cmake)

set(sources
	src/unittest_main.cpp
	src/unittest_shader.cpp
	src/unittest_program.cpp
)

add_executable(unittest EXCLUDE_FROM_ALL ${sources})

target_include_directories(
	unittest
	PUBLIC
	include
)

target_link_libraries(
	unittest
	gtest
	gmock
	glmock
	pthread
	ogls
	GL
)