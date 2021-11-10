#include <omp.h>
#include <stdlib.h>
#include "utils.h"

void triangle_counting_openmp_implementation(struct CSR_mtx *mtx, struct implementation_args *args)
{
    //this is used in case the columns in each row are out of order
    for(int i = 0; i < mtx->mat_size; i++)
    {
        quickSortIterative(mtx->col_idx, mtx->row_idx[i], mtx->row_idx[i+1]-1);
    }
    int i = 0;
    #pragma omp parallel shared(mtx) private(i)
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
                        mtx->val[j]++;                        
                    }
                }
            }
        }
    }
    #pragma omp barrier
}