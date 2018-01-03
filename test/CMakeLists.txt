include(cmake/gtest.cmake)
include(cmake/gmock.cmake)

set(sources
	unittest_main.cpp
	unittest_shader.cpp
	unittest_program.cpp
	mocks/glmock.cpp
)

add_executable(unittest EXCLUDE_FROM_ALL ${sources})

target_include_directories(
	unittest
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/mocks
	include
)

target_link_libraries(
	unittest
	gtest
	gmock
	pthread
	${CMAKE_CURRENT_SOURCE_DIR}/../libogls.a
)