cmake_minimum_required(VERSION 3.22)

project(GPTerminal VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL CONFIG REQUIRED)

include_directories(
    "include"
    "dependencies"
)

file(GLOB SOURCES "src/*.cpp")

add_executable(GPTerminal ${SOURCES})

target_link_libraries(GPTerminal PRIVATE CURL::libcurl)

######### Linux #########
# add_subdirectory(dependencies/clip)

# target_link_libraries(GPTerminal
#     PRIVATE ${CURL_LIBRARIES}
#     clip
# )

# target_link_libraries(GPTerminal PRIVATE ${CURL_LIBRARIES})

# target_include_directories(GPTerminal PRIVATE ${CURL_INCLUDE_DIRS})

