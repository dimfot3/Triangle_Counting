#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "utils.h"

float triagle_counting_sequential_masked_implementation(struct CSR_mtx *mtx)        //O(N)
{
    clock_t t = clock();
    int *temp_mtx = (int*) calloc(mtx->nz_size,sizeof(int));
    float triangles = 0.0;
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
                        triangles+=0.5;
                        temp_mtx[j]+=1;
                    }   
                }
                
            }
        }
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\nSequential Masked implementation: %f\n",time_taken);
    printf("\nones: %f\n",triangles);
    return triangles;
}



int main(int argc, char** argv)
{
    int *row, *col, *row_indexes, *col_indexes;
    int nzlen, mat_size;
    int **mat;
    struct COO_mtx *mtx_coo_fmt =  malloc(sizeof(struct COO_mtx));
    struct CSR_mtx *mtx_csr_fmt =  malloc(sizeof(struct CSR_mtx));
    read_matrix("data/belgium_osm.mtx", mtx_coo_fmt);
    coo_to_csr(mtx_coo_fmt, mtx_csr_fmt);
    float num = triagle_counting_sequential_masked_implementation(mtx_csr_fmt);
    return 0;
}