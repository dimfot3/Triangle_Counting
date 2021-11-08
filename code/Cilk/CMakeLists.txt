cmake_minimum_required(VERSION 3.1)
#these for opencilk
set(CMAKE_C_COMPILER /home/fdimitri/openCilk/bin/clang )
set(CMAKE_CXX_COMPILER /home/fdimitri/openCilk/bin/clang++)

project(triagle_counting LANGUAGES C CXX)
include_directories(include)

add_executable(triangle_counting_cilk src/triangle_counting_cilk.c)
target_link_libraries(triangle_counting_cilk PUBLIC utils seq_masked_implementation benchmarks pthrd_implementation openmp_implementation)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests) 
endif(BUILD_TESTING)
