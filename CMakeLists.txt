cmake_minimum_required(VERSION 3.13.0)

project(alpha-utils VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/source-charset:utf-8)
endif()

find_package(PNG REQUIRED)

add_executable(
    alpha-bleeding
    src/alpha-bleeding.cpp
    src/png.cpp
)

target_link_libraries(alpha-bleeding PRIVATE PNG::PNG)

add_executable(
    alpha-remove
    src/alpha-remove.cpp
    src/png.cpp
)

target_link_libraries(alpha-remove PRIVATE PNG::PNG)

add_executable(
    alpha-set
    src/alpha-set.cpp
    src/png.cpp
)

target_link_libraries(alpha-set PRIVATE PNG::PNG)
