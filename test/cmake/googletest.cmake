########################### GTEST
# Enable ExternalProject CMake module
INCLUDE(ExternalProject)

# Set default ExternalProject root directory
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${ogls_EXTERNAL_PROJECTS_DIR}/googletest)

# Add gtest
ExternalProject_Add(
    googletest    
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.8.0
    BINARY_DIR "${ogls_EXTERNAL_PROJECTS_DIR}/googletest/build"
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

ExternalProject_Get_Property(googletest binary_dir)

add_library(gtest UNKNOWN IMPORTED)
add_library(gtest_main UNKNOWN IMPORTED)
set_target_properties(gtest PROPERTIES
    IMPORTED_LOCATION ${binary_dir}/googlemock/gtest/libgtest.a
)
set_target_properties(gtest_main PROPERTIES
    IMPORTED_LOCATION ${binary_dir}/googlemock/gtest/libgtest_main.a
)
add_dependencies(gtest googletest)
add_dependencies(gtest_main googletest)

add_library(gmock UNKNOWN IMPORTED)
add_library(gmock_main UNKNOWN IMPORTED)
set_target_properties(gmock PROPERTIES
    IMPORTED_LOCATION ${binary_dir}/googlemock/libgmock.a
)
set_target_properties(gmock_main PROPERTIES
    IMPORTED_LOCATION ${binary_dir}/googlemock/libgmock_main.a
)
add_dependencies(gmock googletest)
add_dependencies(gmock_main googletest)