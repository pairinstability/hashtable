# building unit testing with googletest

# google test 
include(../cmake/GoogleTest.cmake)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_executable(runtests test.cpp)
target_link_libraries(runtests PRIVATE libhashtable GTest::GTest absl::hash)

add_test(runtests hash)