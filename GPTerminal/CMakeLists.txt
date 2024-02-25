cmake_minimum_required(VERSION 3.22)

project(GPTerminal VERSION 0.1.0)

find_package(CURL REQUIRED)
find_package(Curses REQUIRED)

include_directories(include)
include_directories(dependencies)

file(GLOB SOURCES "src/*.cpp")

add_executable(GPTerminal ${SOURCES})

target_link_libraries(GPTerminal PRIVATE ${CURL_LIBRARIES})
target_include_directories(GPTerminal PUBLIC ${CURL_INCLUDE_DIRS})