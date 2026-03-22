cmake_minimum_required(VERSION 4.0)
project(HIS
        LANGUAGES C
        VERSION 0.0.1)

set(CMAKE_C_STANDARD 11)

file(GLOB SOURCES "src/*.c")
add_executable(HIS ${SOURCES})

target_include_directories(HIS PRIVATE "include")