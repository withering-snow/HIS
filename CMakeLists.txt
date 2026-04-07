cmake_minimum_required(VERSION 4.0)

project(HIS
        LANGUAGES C
        VERSION 1.0.0)

set(CMAKE_C_STANDARD 11)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
set(CUSTOM_RGBA_AS_DEFAULT_FORMAT ON CACHE BOOL "" FORCE)

add_subdirectory(lib/raylib-5.5)

include_directories(
    include
    include/core
    include/entity
    include/repository
    include/service
    include/io
    include/ui
    lib/raylib-5.5/src
)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(HIS ${SOURCES})

target_link_libraries(HIS raylib)

if(UNIX AND NOT APPLE)
    target_link_libraries(HIS m pthread dl rt X11)
endif()

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/data/pingfang.otf" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
