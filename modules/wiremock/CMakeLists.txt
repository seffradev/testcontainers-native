add_library(wiremock SHARED
    wiremock.c
)

target_include_directories(wiremock PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(wiremock PRIVATE c)
