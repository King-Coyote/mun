file(GLOB SRC *.cpp)
file(GLOB TESTS tests/*.cpp)

set(CMAKE_BUILD_TYPE Debug)

add_library(mun ${SRC})

target_include_directories(mun
    PUBLIC
        ${mun_dir}
)

target_link_libraries(mun
    lua
)

add_executable(tests ${TESTS} StackOps.cpp)

target_include_directories(tests
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${catch_dir}
        ${mun_dir}
)

target_link_libraries(tests
    mun
)