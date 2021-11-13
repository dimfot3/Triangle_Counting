cmake_minimum_required(VERSION 3.1)
#these for opencilk
option(GET_DATASET OFF)
option(CILK OFF)

project(triagle_counting LANGUAGES C CXX)
include_directories(include)


find_package(OpenMP)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -O3")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -O3")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

#library for loading matrix for matrix market
add_library(matrix_loader src/mmio.c)

#sequential masked implementation
add_library(seq_masked_implementation src/sequential_masked_implementation.c)

#bechmarks 
add_library(benchmarks src/benchmarks.c)

#pthread implementation
add_library(pthrd_implementation src/pthreads_implementation.c)
target_link_libraries(pthrd_implementation PUBLIC -lpthread)

#pthread implementation
add_library(openmp_implementation src/openmp_implementation.c)

#library for loading matrix for matrix market
add_library(utils src/utils.c)
target_link_libraries(utils matrix_loader)

add_executable(tri_counter src/triangle_counting.c)
target_link_libraries(tri_counter PUBLIC utils seq_masked_implementation benchmarks pthrd_implementation openmp_implementation)

if(GET_DATASET)
execute_process (
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/useful_scripts/download_data.sh
    OUTPUT_VARIABLE outVar
)
endif(GET_DATASET)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY results DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#build cilk
if(CILK)
ADD_CUSTOM_TARGET(cilk_implemnt ALL COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/build_cilk.sh)
endif(CILK)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests) 
endif(BUILD_TESTING)

