cmake_minimum_required(VERSION 3.1)

#download and populate the gtest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
)

FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(utils_test src/utils_test.cpp)
target_include_directories(utils_test PUBLIC ../src/)
target_link_libraries(utils_test PUBLIC utils gtest_main)


gtest_discover_tests(utils_test)