cmake_minimum_required(VERSION 3.10)

set(SEQ seq)
set(SEQ_VERSION 0.2.1)
project(${SEQ} VERSION ${SEQ_VERSION} LANGUAGES CXX)

include(GNUInstallDirs)

# Target configuration
add_library(${SEQ} INTERFACE)
add_library(seq::seq ALIAS ${SEQ})
target_compile_features(${SEQ} INTERFACE cxx_std_17)
target_include_directories(${SEQ} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Tests
enable_testing()
option(SEQ_BUILD_TESTS "Build tests" ON)
if(SEQ_BUILD_TESTS)
    add_subdirectory(tests)
endif()

# Export and install targets
set(SEQ_INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${SEQ}")

# Set install destinations and associate installed target files with an export
install(TARGETS ${SEQ}
    EXPORT ${SEQ}-targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Write build-tree targets
export(TARGETS ${SEQ}
    FILE ${CMAKE_CURRENT_BINARY_DIR}/${SEQ}-targets.cmake
    NAMESPACE ${SEQ}::)
# Allow find_package to locate package without installing it (find it's build-tree)
export(PACKAGE ${SEQ})

install(EXPORT ${SEQ}-targets
    FILE ${SEQ}-targets.cmake
    NAMESPACE ${SEQ}::
    DESTINATION ${SEQ_INSTALL_CONFIGDIR})

include(CMakePackageConfigHelpers)

# Generate build-tree configuration
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/templates/config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${SEQ}-config.cmake"
    INSTALL_DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    PATH_VARS INCLUDE_DIR
    INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})

# Generate install-tree configuration
set(INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR})
configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/templates/config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${SEQ}-config.install.cmake"
    INSTALL_DESTINATION ${SEQ_INSTALL_CONFIGDIR}
    PATH_VARS INCLUDE_DIR)

# Generate package version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${SEQ}-config-version.cmake"
    VERSION ${SEQ_VERSION}
    COMPATIBILITY SameMajorVersion)

# Install the install-tree configuration
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${SEQ}-config.install.cmake"
    DESTINATION ${SEQ_INSTALL_CONFIGDIR}
    RENAME "${SEQ}-config.cmake")

# Install package version file
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${SEQ}-config-version.cmake"
    DESTINATION ${SEQ_INSTALL_CONFIGDIR})

# Install include files
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
