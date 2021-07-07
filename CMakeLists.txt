cmake_minimum_required(VERSION 3.11)
project(fission)

# output everything to bin/, including the data folder (look at the bottom of this file)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# fission
add_executable(fission
    "source/main.c"
    "source/text_buf.c"
    "source/font.c"
    "source/file_io.c"
    "source/component.c"
    "source/backend.c"

    "source/component/timer.c"
    "source/component/title.c"

    "source/backend/glfw_gl4.c"

    "external/glad/gl.c"
)

# livesplit-core
include(ExternalProject)
set_directory_properties(PROPERTIES EP_PREFIX "${CMAKE_BINARY_DIR}/rust")

ExternalProject_Add(
    livesplit-core
    DOWNLOAD_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND   cargo build -p staticlib
    COMMAND         cargo build --release -p staticlib
    SOURCE_DIR      "${CMAKE_SOURCE_DIR}/external/livesplit-core"
    BUILD_IN_SOURCE ON
    INSTALL_COMMAND ""
    LOG_BUILD       ON
    BUILD_BYPRODUCTS
        "${CMAKE_SOURCE_DIR}/external/livesplit-core/target/debug/liblivesplit_core.a"
        "${CMAKE_SOURCE_DIR}/external/livesplit-core/target/release/liblivesplit_core.a"
)

set_property(TARGET livesplit-core
    PROPERTY IMPORTED_LOCATION)

# livesplit_core.h
ExternalProject_Add(
    livesplit_core-h
    DOWNLOAD_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND   cargo run
    COMMAND         cargo run --release
    SOURCE_DIR      "${CMAKE_SOURCE_DIR}/external/livesplit-core/capi/bind_gen"
    BUILD_IN_SOURCE ON
    INSTALL_COMMAND ""
    LOG_BUILD       ON
    DEPENDS         livesplit-core
    BUILD_BYPRODUCTS "${CMAKE_SOURCE_DIR}/external/livesplit-core/capi/bindings/livesplit_core.h"
)

# dependencies
add_dependencies(fission livesplit_core-h livesplit-core)

# fission config and link
target_include_directories(fission PRIVATE
    "${CMAKE_SOURCE_DIR}/external"
    "${CMAKE_SOURCE_DIR}/external/livesplit-core/capi/bindings"
)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
target_link_libraries(fission
    debug           "${CMAKE_SOURCE_DIR}/external/livesplit-core/target/debug/liblivesplit_core.a"
    optimized       "${CMAKE_SOURCE_DIR}/external/livesplit-core/target/release/liblivesplit_core.a"
    glfw OpenGL::GL Threads::Threads
)

if(UNIX)
    target_link_libraries(fission
        m dl
    )
endif()

# data files
file(COPY
    "${CMAKE_SOURCE_DIR}/data/"
    DESTINATION "${CMAKE_BINARY_DIR}/bin/data/"
)
