#include "utils.h"
#include <benchmarks.h>
#include <time.h>
void time_bechmark( float (*f)(struct CSR_mtx*, struct implementation_args*), struct CSR_mtx *mtx_csr_fmt, int num_of_loops, 
                    float *num_of_triangles, int full_mat, struct results *res)
{
    float time_taken = 0;
    float square_time = 0;
    struct implementation_args func_args;
    func_args.num_of_threads = 5;
    func_args.full_mat = full_mat;
    func_args.verbose = 0;
    for(int i = 0; i < num_of_loops; i++)
    {      
        float temp_time = 0;
        struct timespec start, finish;
        clock_gettime(CLOCK_MONOTONIC, &start);
        *num_of_triangles = f(mtx_csr_fmt, &func_args);
        clock_gettime(CLOCK_MONOTONIC, &finish);
        temp_time = finish.tv_sec + finish.tv_nsec / 1000000000.0 - start.tv_sec - start.tv_nsec / 1000000000.0;
        time_taken+=temp_time;
        square_time+=temp_time*temp_time;
    }
    res->mean_time = time_taken/num_of_loops;
    res->var_time = square_time/num_of_loops - res->mean_time*res->mean_time;
}