cmake_minimum_required(VERSION 3.11)
project(fission)

# project structure
set(FIS_BINS "bin")
set(FIS_DATA "data")
set(FIS_LIBS "external")
set(FIS_SRCS "source")
set(FIS_RUST "rust")

# gl libs
# glfw
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
add_subdirectory(
    "${CMAKE_SOURCE_DIR}/${FIS_LIBS}/glfw"
)

# glad
add_library(glad STATIC
    "${FIS_LIBS}/glad/gl.c"
)
target_include_directories(glad PRIVATE
    "${CMAKE_SOURCE_DIR}/${FIS_LIBS}"
)

# output everything to bin/, including the data folder (look at the bottom of this file)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${FIS_BINS}")

# fission
add_executable(fission
    "${FIS_SRCS}/main.c"
    "${FIS_SRCS}/text_buf.c"
    "${FIS_SRCS}/font.c"
    "${FIS_SRCS}/file_io.c"
    "${FIS_SRCS}/component.c"
    "${FIS_SRCS}/backend.c"

    "${FIS_SRCS}/component/timer.c"
    "${FIS_SRCS}/component/title.c"

    "${FIS_SRCS}/backend/glfw_gl4.c"
)

# livesplit-core
include(ExternalProject)
set_directory_properties(PROPERTIES EP_PREFIX "${CMAKE_BINARY_DIR}/${FIS_RUST}")

if(CMAKE_CROSSCOMPILING)
    set(CARGO_TARGET_PATH "${CARGO_TARGET}")
    set(CARGO_TARGET_PARAM --target "${CARGO_TARGET}")
else()
    set(CARGO_TARGET_PATH)
    set(CARGO_TARGET_PARAM)
endif()

if(MSVC)
    set(CARGO_LIB_PFX "")
    set(CARGO_LIB_EXT ".lib")
else()
    set(CARGO_LIB_PFX "lib")
    set(CARGO_LIB_EXT ".a")
endif()

set(RELEASE "$<NOT:$<CONFIG:Debug>>")
set(RUST_BUILD_TYPE_DIR "$<IF:${RELEASE},release,debug>")
ExternalProject_Add(
    livesplit-core
    DOWNLOAD_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND   ${CMAKE_COMMAND} -E env "CARGO_TARGET_DIR=${CMAKE_BINARY_DIR}/${FIS_RUST}/target" cargo build $<${RELEASE}:--release> -p staticlib ${CARGO_TARGET_PARAM}
    SOURCE_DIR      "${CMAKE_SOURCE_DIR}/${FIS_LIBS}/livesplit-core"
    BUILD_IN_SOURCE ON
    INSTALL_COMMAND ""
    LOG_BUILD       ON
    BUILD_BYPRODUCTS
        "${CMAKE_BINARY_DIR}/${FIS_RUST}/target/${CARGO_TARGET_PATH}/${RUST_BUILD_TYPE_DIR}/${CARGO_LIB_PFX}livesplit_core${CARGO_LIB_EXT}"
)

# livesplit_core.h
ExternalProject_Add(
    livesplit_core-h
    DOWNLOAD_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND   cargo run
    COMMAND         mkdir -p "${CMAKE_BINARY_DIR}/${FIS_RUST}/h"
    COMMAND         mv "${CMAKE_SOURCE_DIR}/${FIS_LIBS}/livesplit-core/capi/bindings/livesplit_core.h" "${CMAKE_BINARY_DIR}/${FIS_RUST}/h/livesplit_core.h"
    SOURCE_DIR      "${CMAKE_SOURCE_DIR}/${FIS_LIBS}/livesplit-core/capi/bind_gen"
    BUILD_IN_SOURCE ON
    INSTALL_COMMAND ""
    LOG_BUILD       ON
    DEPENDS         livesplit-core
    BUILD_BYPRODUCTS 
        "${CMAKE_BINARY_DIR}/${FIS_RUST}/h/livesplit_core.h"
)

# dependencies
add_dependencies(fission livesplit_core-h livesplit-core)

# fission config and link
target_include_directories(fission PRIVATE
    "${CMAKE_SOURCE_DIR}/${FIS_LIBS}"
    "${CMAKE_BINARY_DIR}/${FIS_RUST}/h"
)

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
target_link_libraries(fission
    "${CMAKE_BINARY_DIR}/${FIS_RUST}/target/${CARGO_TARGET_PATH}/${RUST_BUILD_TYPE_DIR}/${CARGO_LIB_PFX}livesplit_core${CARGO_LIB_EXT}"
    glad glfw OpenGL::GL Threads::Threads
)

if(UNIX)
    target_link_libraries(fission
        m dl
    )
elseif(WIN32)
    target_link_libraries(fission
        debug ws2_32 userenv
        bcrypt
    )
endif()

# data files
file(COPY
    "${CMAKE_SOURCE_DIR}/${FIS_DATA}/"
    DESTINATION "${CMAKE_BINARY_DIR}/${FIS_BINS}/${FIS_DATA}/"
)
