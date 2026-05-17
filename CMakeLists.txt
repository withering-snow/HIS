cmake_minimum_required(VERSION 4.0)

project(HIS
        LANGUAGES C
        VERSION 1.0.0)

set(CMAKE_C_STANDARD 11)

# Debug/Release 配置
# Debug 模式：定义 NDEBUG，ASSERT 生效
# Release 模式：不定义 NDEBUG，ASSERT 跳过
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")

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
