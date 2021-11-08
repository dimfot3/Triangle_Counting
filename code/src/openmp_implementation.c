#include "utils.h"
#include <omp.h>

float triangle_counting_openmp_implementation(struct CSR_mtx *mtx, struct implementation_args *args)
{
    //float *triangle_threads = (float*) calloc(omp_get_max_threads() ,sizeof(float));
    float triangles = 0.0;
    int i = 0;
    omp_lock_t writelock;
    omp_init_lock(&writelock);
    #pragma omp parallel shared(mtx, triangles) private(i)
    {
        #pragma omp for   
        for(i = 0; i < mtx->mat_size; i++)          //all rows      
        {
            for(int j = mtx->row_idx[i]; j < mtx->row_idx[i+1]; j++)    // all non zero colums of i row. this is the actual mask
            {
                int col = mtx->col_idx[j];
                for(int k = mtx->row_idx[col]; k < mtx->row_idx[col+1]; k++)        // all non zero columns of row[col]
                {
                    int col1 = mtx->col_idx[k];
                    int succ = binarySearch(mtx->col_idx, mtx->row_idx[i], mtx->row_idx[i+1]-1, col1);
                    if(succ!=-1)
                    {
                        omp_set_lock(&writelock);
                        triangles++;
                        omp_unset_lock(&writelock);
                        
                    }
                }
            }
        }
    }
    #pragma omp barrier
    if(args->full_mat)
    {
        triangles/=6.0;
    }
    return triangles;
}