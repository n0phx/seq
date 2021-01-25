cmake_minimum_required(VERSION 3.10)

set(SEQ seq)
set(SEQ_VERSION 0.2.0)
project(${SEQ} VERSION ${SEQ_VERSION} LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
set(SEQ_INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${SEQ}")

# Target configuration
add_library(${SEQ} INTERFACE)
add_library(seq::seq ALIAS ${SEQ})
target_compile_features(${SEQ} INTERFACE cxx_std_17)
target_include_directories(${SEQ} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Tests
enable_testing()
option(SEQ_BUILD_TESTS "Build tests" ON)
if(SEQ_BUILD_TESTS)
    add_subdirectory(tests)
endif()

# Export and install targets
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${SEQ}-config-version.cmake"
    VERSION ${SEQ_VERSION}
    COMPATIBILITY SameMajorVersion)

install(TARGETS ${SEQ} EXPORT seq-targets)
install(EXPORT seq-targets
    FILE seq-config.cmake
    NAMESPACE seq::
    DESTINATION ${SEQ_INSTALL_CONFIGDIR}
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${SEQ}-config-version.cmake"
    DESTINATION ${SEQ_INSTALL_CONFIGDIR})
install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(DIRECTORY seq DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
