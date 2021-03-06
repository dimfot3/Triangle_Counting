#include <omp.h>
#include <stdlib.h>
#include "utils.h"

void triangle_counting_openmp_implementation(struct CSR_mtx *mtx, struct implementation_args *args)
{
    //this is used in case the columns in each row are out of order
    for(int i = 0; i < mtx->mat_size; i++)
    {
        if(mtx->row_idx[i+1] - mtx->row_idx[i] > 5)
            quickSortIterative(mtx->col_idx, mtx->row_idx[i], mtx->row_idx[i+1]-1);
    }
    omp_set_dynamic(0);
    omp_set_num_threads(args->num_of_threads);
    #pragma omp parallel for schedule(dynamic, 1)
    for(int i = 0; i < mtx->mat_size ; i++)
    {
        for(int j = mtx->row_idx[i]; j < mtx->row_idx[i+1]; j++)    // all non zero colums of i row. this is the actual mask
        {
            int col = mtx->col_idx[j];
            for(int k = mtx->row_idx[col]; k < mtx->row_idx[col+1]; k++)        // all non zero columns of row[col]
            {
                int col1 = mtx->col_idx[k];
                int succ;
                if(mtx->row_idx[i+1] - mtx->row_idx[i] > 5)
                    succ = binarySearch(mtx->col_idx, mtx->row_idx[i], mtx->row_idx[i+1]-1, col1);
                else
                    succ = linearSearch(mtx->col_idx, mtx->row_idx[i], mtx->row_idx[i+1]-1, col1);
                if(succ!=-1)
                {
                    mtx->val[j]++;                        
                }
            }
        }
    }
    #pragma omp barrier
}