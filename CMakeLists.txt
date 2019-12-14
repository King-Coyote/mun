cmake_minimum_required (VERSION 3.14.0)
project(mun)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)

set(mun_dir ${CMAKE_CURRENT_SOURCE_DIR}/mun)
set(catch_dir /usr/include/catch2)

file(GLOB SRC src/*.cpp)

add_subdirectory(src)

add_executable(munTest main.cpp)

target_include_directories(munTest
    PUBLIC
        mun
)

target_link_libraries(munTest
    mun
    lua
)