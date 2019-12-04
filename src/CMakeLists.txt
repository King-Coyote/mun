file(GLOB SRC *.cpp)

set(CMAKE_BUILD_TYPE Debug)

add_library(luavm ${SRC})

target_include_directories(luavm
    PUBLIC
        ${luavm_dir}
)

target_link_libraries(luavm
    lua
)