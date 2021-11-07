cmake_minimum_required(VERSION 3.1)
#these for opencilk
#set(CMAKE_C_COMPILER /home/fdimitri/openCilk/bin/clang )
#set(CMAKE_CXX_COMPILER /home/fdimitri/openCilk/bin/clang++)
set(CMAKE_CXX_FLAGS "-O3")
option(GET_DATASET OFF)
option(FIND_TRIANGLES OFF)

project(triagle_counting LANGUAGES C CXX)
include_directories(include)

#library for loading matrix for matrix market
add_library(matrix_loader src/mmio.c)

#sequential masked implementation
add_library(seq_masked_implementation src/sequential_masked_implementation.c)

#sequential masked implementation
add_library(benchmarks src/benchmarks.c)

#library for loading matrix for matrix market
add_library(utils src/utils.c)
target_link_libraries(utils matrix_loader)

add_executable(tri_counter src/triangle_counting.c)
target_link_libraries(tri_counter PUBLIC -lpthread utils seq_masked_implementation benchmarks)

if(GET_DATASET)
execute_process (
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/useful_scripts/download_data.sh
    OUTPUT_VARIABLE outVar
)
endif(GET_DATASET)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY results DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests) 
endif(BUILD_TESTING)
