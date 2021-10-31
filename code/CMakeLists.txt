cmake_minimum_required(VERSION 3.1)
project(triagle_counting LANGUAGES C CXX)
include_directories(include)

#library for loading matrix for matrix market
add_library(matrix_loader src/mmio.c)

#library for loading matrix for matrix market
add_library(utils src/utils.c)
target_link_libraries(utils matrix_loader)

add_executable(tri_counter src/triangle_counting.c)
target_link_libraries(tri_counter PUBLIC -lpthread utils)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests) 
endif(BUILD_TESTING)