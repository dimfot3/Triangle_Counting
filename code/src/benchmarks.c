#include "utils.h"
#include <benchmarks.h>

float time_bechmark( float (*f)(struct CSR_mtx*, double*, int),struct CSR_mtx *mtx_csr_fmt, int num_of_loops)
{
    float time_taken = 0;
    for(int i = 0; i < num_of_loops; i++)
    {
        float temp_time;
        float num_of_triangles = f(mtx_csr_fmt, &temp_time, 0);
        time_taken+=temp_time;
    }
    return time_taken/num_of_loops;
}