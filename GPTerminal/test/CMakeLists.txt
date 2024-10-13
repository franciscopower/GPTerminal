set(BINARY ${CMAKE_PROJECT_NAME}_tst)

enable_testing()

find_package(GTest CONFIG REQUIRED)

set(TEST_SOURCES
  TextManipulation_test.cpp
)

add_executable(${BINARY} ${TEST_SOURCES})

target_link_libraries(${BINARY}
  PUBLIC ${CMAKE_PROJECT_NAME}_lib
  PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)


add_test(NAME ${BINARY} COMMAND ${BINARY})

