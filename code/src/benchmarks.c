#include "stdlib.h"
#include <time.h>
#include "utils.h"
#include <benchmarks.h>

void time_bechmark( void (*f)(struct CSR_mtx*, struct implementation_args*), struct CSR_mtx *mtx_csr_fmt, int num_of_loops, struct results *res)
{
    float time_taken = 0;
    float square_time = 0;
    struct implementation_args func_args;
    func_args.num_of_threads = 5;
    func_args.verbose = 0;
    for(int i = 0; i < num_of_loops; i++)
    {      
        float temp_time = 0;
        struct timespec start, finish;
        clock_gettime(CLOCK_MONOTONIC, &start);
        f(mtx_csr_fmt, &func_args);
        clock_gettime(CLOCK_MONOTONIC, &finish);
        temp_time = finish.tv_sec + finish.tv_nsec / 1000000000.0 - start.tv_sec - start.tv_nsec / 1000000000.0;
        time_taken+=temp_time;
        square_time+=temp_time*temp_time;
        res->triangles = get_triangles(mtx_csr_fmt);
        restore_mat(mtx_csr_fmt);
    }
    res->mean_time = time_taken/num_of_loops;
    res->var_time = square_time/num_of_loops - res->mean_time*res->mean_time;
}