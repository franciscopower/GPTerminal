cmake_minimum_required(VERSION 3.22)

project(GPTerminal VERSION 0.1.0)

find_package(CURL REQUIRED)

include_directories(
    "include"
    "dependencies"
    "dependencies/clip"
)

file(GLOB SOURCES "src/*.cpp")

add_executable(GPTerminal ${SOURCES})

add_subdirectory(dependencies/clip)

target_link_libraries(GPTerminal 
    PRIVATE ${CURL_LIBRARIES}
    clip
)
target_include_directories(GPTerminal PUBLIC ${CURL_INCLUDE_DIRS})