cmake_minimum_required(VERSION 3.13.0)

project(alpha-utils VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/source-charset:utf-8)
endif()

find_package(PNG REQUIRED)
find_package(argparse REQUIRED)

add_executable(
    alpha
    src/alpha-bleed.cpp
    src/alpha-set.cpp
    src/main.cpp
    src/png.cpp
)

target_link_libraries(alpha PRIVATE PNG::PNG)
target_link_libraries(alpha PRIVATE argparse::argparse)
