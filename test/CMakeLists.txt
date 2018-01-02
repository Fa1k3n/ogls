#find_package(PkgConfig)
#pkg_check_modules(GTEST REQUIRED gtest>=1.7.0)
#pkg_check_modules(GMOCK REQUIRED gmock>=1.7.0)

include(gtest.cmake)

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
	#${GTEST_INCLUDE_DIRS}
	#${GMOCK_INCLUDE_DIRS}
	include
)

target_link_libraries(
	unittest
	gtest
	gmock
	pthread
	${CMAKE_CURRENT_SOURCE_DIR}/../libogls.a
)