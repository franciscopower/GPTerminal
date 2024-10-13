find_package(CURL CONFIG REQUIRED)

file(GLOB SOURCES "*.cpp")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE CURL::libcurl)
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE CURL::libcurl)
