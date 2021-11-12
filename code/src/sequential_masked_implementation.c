#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "sequential_masked_implementation.h"


void triagle_counting_sequential_masked_implementation(struct CSR_mtx *mtx, struct implementation_args *args)        //O(N)
{
    for(int i = 0; i < mtx->mat_size; i++)
    {
       if(mtx->row_idx[i+1] - mtx->row_idx[i] > 5)
           quickSortIterative(mtx->col_idx, mtx->row_idx[i], mtx->row_idx[i+1]-1);
    }
    int counter = 0;
    for(int i = 0; i < mtx->mat_size; i++)          //all rows      
    {
        for(int j = mtx->row_idx[i]; j < mtx->row_idx[i+1]; j++)    // all non zero colums of i row. this is the actual mask
        {
            int col = mtx->col_idx[j];
            for(int k = mtx->row_idx[col]; k < mtx->row_idx[col+1]; k++)        // all non zero columns of row[col]
            {
                int succ;
                int col1 = mtx->col_idx[k];
                if(mtx->row_idx[i+1] - mtx->row_idx[i] > 5)
                    succ = binarySearch(mtx->col_idx, mtx->row_idx[i], mtx->row_idx[i+1]-1, col1);
                else
                    succ = linearSearch(mtx->col_idx, mtx->row_idx[i], mtx->row_idx[i+1]-1, col1);
                if(succ!=-1)
                {
                    mtx->val[j]++;
                }
                counter++;
            }
        }
    }
    printf("counter: %d\n", get_triangles(mtx));
    if(args->verbose)
    {
        printf("\ncounter: %d\n",counter);
    }
}
