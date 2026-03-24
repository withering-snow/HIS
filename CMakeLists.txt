cmake_minimum_required(VERSION 4.0)

project(HIS
        LANGUAGES C
        VERSION 1.0.0)

set(CMAKE_C_STANDARD 11)

include_directories(
    include
    include/core
    include/entity
    include/repository
    include/service
    include/io
    include/ui
)

file(GLOB_RECURSE SOURCES "src/*.c")

# 3. 生成可执行文件
add_executable(HIS ${SOURCES})