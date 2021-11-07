#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "sequential_masked_implementation.h"

float triagle_counting_sequential_masked_implementation(struct CSR_mtx *mtx, float *time_taken, int verbose)        //O(N)
{
    clock_t t = clock();
    //int *temp_mtx = (int*) calloc(mtx->nz_size,sizeof(int));
    float triangles = 0.0;
    int counter = 0;
    for(int i = 0; i < mtx->mat_size; i++)          //all rows      
    {
        for(int j = mtx->row_idx[i]; j < mtx->row_idx[i+1]; j++)    // all non zero colums of i row. this is the actual mask
        {
            int col = mtx->col_idx[j];
            for(int k = mtx->row_idx[col]; k < mtx->row_idx[col+1]; k++)        // all non zero columns of row[col]
            {
                for(int l = mtx->row_idx[i]; l < mtx->row_idx[i+1]; l++)           //all the non zero colums in i row
                {
                    int col1 = mtx->col_idx[l];
                    int col2 = mtx->col_idx[k];
                    if(col1 == col2)
                    {
                        triangles += 1;
                        //temp_mtx[j] += 0.5;
                        break;
                    }
                    counter++;
                }
                
            }
        }
    }
    t = clock() - t;
    *time_taken = ((float)t)/CLOCKS_PER_SEC; // in seconds
    if(verbose)
    {
        printf("\ncounter: %d\n",counter);
        printf("\nSequential Masked implementation: %f s\n",*time_taken);
        printf("\ntriangles: %f\n",triangles);
    }
    return triangles;
}
