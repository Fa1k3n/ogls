set(ogls_EXTERNAL_PROJECTS_DIR "${CMAKE_BINARY_DIR}/test/external")

# Download and unpack googletest at configure time
configure_file(cmake/googletest-CMakeLists.txt.in external/googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${ogls_EXTERNAL_PROJECTS_DIR}/googletest-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${ogls_EXTERNAL_PROJECTS_DIR}/googletest-download" )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory("${ogls_EXTERNAL_PROJECTS_DIR}/googletest-src"
                 "${ogls_EXTERNAL_PROJECTS_DIR}/googletest-build")

# glmock build requires gtest and gmock include dirs
include_directories("${gtest_SOURCE_DIR}/include"
                    "${gmock_SOURCE_DIR}/include")


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