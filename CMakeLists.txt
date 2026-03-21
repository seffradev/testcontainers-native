cmake_minimum_required (VERSION 3.26)
project (testcontainers-native
    VERSION 0.1.0
    DESCRIPTION "Testcontainers for C and C++"
    LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CPPCHECK "cppcheck;--enable=style,warning;--check-level=exhaustive")

include(GNUInstallDirs)
include(CTest)
include(CMakePackageConfigHelpers)

# Testcontainers Bridge
add_custom_command(
    OUTPUT
        bridge.so
        bridge.h
    DEPENDS
        bridge/bridge.go
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bridge
    COMMAND env GOPATH=${GOPATH} go build -buildmode=c-archive -linkshared
    -o "${CMAKE_CURRENT_BINARY_DIR}/bridge.so"
    ${CMAKE_GO_FLAGS} bridge.go
    COMMENT "Building Testcontainers Go to C bridge library"
)

add_custom_target(
    bridge-shim
    DEPENDS
        bridge.so
        bridge.h
)

add_library(
    bridge
    STATIC
    IMPORTED
    GLOBAL
)

add_dependencies(
    bridge
    bridge-shim
)

set_target_properties(
    bridge PROPERTIES
    LINKER_LANGUAGE C
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER
        bridge.h
    IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/bridge.so
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR}
)

# Testcontainers C
add_library(
    c
    SHARED
    c/src/container.c
)

add_library(
    testcontainers-native::c ALIAS c
)

target_include_directories(
    c PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/c/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    c
    PRIVATE bridge
)

install(
    TARGETS c
    EXPORT cTargets
    LIBRARY DESTINATION lib
    INCLUDES DESTINATION include
)

install(
    EXPORT cTargets
    FILE cTargets.cmake
    NAMESPACE testcontainers-native::
    DESTINATION lib/cmake/c
)

configure_package_config_file(
    c/cConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cConfig.cmake
    INSTALL_DESTINATION lib/cmake/c
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/cConfig.cmake
    DESTINATION lib/cmake/c
)

# Testcontainers C++
add_library(
    cpp
    INTERFACE
)

add_library(
    testcontainers-native::cpp ALIAS cpp
)

target_include_directories(cpp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpp>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(cpp INTERFACE c)

install(
    TARGETS cpp
    EXPORT cppTargets
    LIBRARY DESTINATION lib
    INCLUDES DESTINATION include
)

install(
    EXPORT cppTargets
    FILE cppTargets.cmake
    NAMESPACE testcontainers-native::
    DESTINATION lib/cmake/cpp
)

configure_package_config_file(
    cpp/cppConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cppConfig.cmake
    INSTALL_DESTINATION lib/cmake/cpp
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/cppConfig.cmake
    DESTINATION lib/cmake/cpp
)

# Modules
add_subdirectory(modules)
