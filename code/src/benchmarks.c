#include "utils.h"
#include <benchmarks.h>

void time_bechmark( float (*f)(struct CSR_mtx*, struct implementation_args*), struct CSR_mtx *mtx_csr_fmt, int num_of_loops, 
                    float *num_of_triangles, int full_mat, struct results *res)
{
    float time_taken = 0;
    float square_time = 0;
    struct implementation_args func_args;
    func_args.num_of_threads = 0;
    func_args.full_mat = full_mat;
    func_args.verbose = 0;
    for(int i = 0; i < num_of_loops; i++)
    {
        func_args.time_taken = 0;
        *num_of_triangles = f(mtx_csr_fmt, &func_args);
        time_taken+=func_args.time_taken;
        square_time+=func_args.time_taken*func_args.time_taken;
    }
    res->mean_time = time_taken/num_of_loops;
    res->var_time = square_time/num_of_loops - res->mean_time*res->mean_time;
}