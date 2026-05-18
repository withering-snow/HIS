cmake_minimum_required(VERSION 4.0)

project(HIS
        LANGUAGES C
        VERSION 1.0.3)

set(CMAKE_C_STANDARD 11)

include_directories(
    include/core
    include/entity
    include/ctrl
    include/service
        include/service/serv_base
    include/ui
    include/api
)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(HIS ${SOURCES})


target_compile_options(HIS PRIVATE $<$<CONFIG:Release>:-O2>)
target_compile_definitions(HIS PRIVATE $<$<CONFIG:Release>:NDEBUG>)

target_compile_options(HIS PRIVATE $<$<CONFIG:Debug>:-g>)
