#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "triangle_counting_cilk_implementation.h"


float triangle_counting_cilk_implementation(struct CSR_mtx *mtx, struct implementation_args *args)        //O(N)
{
    //int *temp_mtx = (int*) calloc(mtx->nz_size,sizeof(int));
    uint triangles = 0.0;
    int counter = 0;
    for(int i = 0; i < mtx->mat_size; i++)          //all rows      
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
                    triangles+=1;
                }
                counter++;
            }
        }
    }
    
    if(args->verbose)
    {
        printf("\ncounter: %d\n",counter);
        printf("\ntriangles: %lf\n",triangles);
    }
   
    if(args->full_mat)
    {
        triangles/=6;
    }
    return triangles;
}
