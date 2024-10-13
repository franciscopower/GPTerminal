cmake_minimum_required(VERSION 3.22)

project(GPTerminal VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    "include"
    "thirdparty"
)

add_subdirectory(src)
add_subdirectory(test)
