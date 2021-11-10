cmake_minimum_required(VERSION 3.1)
#these for opencilk
set(CMAKE_C_COMPILER /home/fdimitri/openCilk/bin/clang )
set(CMAKE_CXX_COMPILER /home/fdimitri/openCilk/bin/clang++)
project(triagle_counting LANGUAGES C CXX)
include_directories(include ../include/)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

add_library(cilk_implementation src/triangle_counting_cilk_implementation.c)

add_executable(triangle_counting_cilk src/triangle_counting_cilk.c)
target_link_libraries(triangle_counting_cilk PUBLIC cilk_implementation utils benchmarks matrix_loader)
