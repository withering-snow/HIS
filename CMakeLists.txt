cmake_minimum_required(VERSION 4.0)

project(HIS
        LANGUAGES C
        VERSION 1.0.0)

set(CMAKE_C_STANDARD 11)

include_directories(
    include/core
    include/entity
    include/ctrl
    include/service
    include/ui
    include/api
)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(HIS ${SOURCES})
