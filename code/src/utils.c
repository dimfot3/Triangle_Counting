#include <stdlib.h>
#include <stdio.h>
#include "mmio.h"
#include <time.h>
#include "utils.h"

void read_matrix(char** path, struct COO_mtx* mtx)
{
    
    int ret_code;
    MM_typecode matcode;
    FILE *f;
    int M, N, nz;   
    int i, *I, *J;
    double *val;

    clock_t t = clock();
    if ((f = fopen(path, "r")) == NULL) 
    {
        printf("No matrix found!\n");
        exit(0);
    }
    if (mm_read_banner(f, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }
    //check if the matrix is valid for this application
    if (mm_is_complex(matcode) || !mm_is_matrix(matcode) || mm_is_dense(matcode) || !mm_is_symmetric(matcode))
    {
        printf("Sorry, matrix is invalid. We need symmetrix sparse matrix");
        exit(1);
    }
    //finds some basic of the matrix
    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) !=0)
        exit(1);
    // reseve memory for matrices 
    I = (int *) malloc(nz * sizeof(int));
    J = (int *) malloc(nz * sizeof(int));
    for (i=0; i<nz; i++)
    {
        fscanf(f, "%d %d\n", &I[i], &J[i]);
        I[i]--;  /* adjust from 1-based to 0-based */
        J[i]--;
    }
    if (f !=stdin) fclose(f);

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    printf("\nSuccessfully loaded matrix in %0.2f seconds.\nMatrix info:\nRow,Cols:%d, %d\nnon zero values: %d \
    \nfirst nz position: %d %d\nlast nz position: %d %d\n", 
    time_taken, M, N, nz,I[0]+1, J[0]+1, I[nz-1]+1, J[nz-1]+1);
    
    mtx->nz_size = nz;
    mtx->mat_size = M;
    mtx->row_idx = J;
    mtx->col_idx = I;
}

//sparce matrix from coordinate list to compressed row index
void coo_to_csr(struct COO_mtx* mtx, struct CSR_mtx* new_mtx)
{
    new_mtx->nz_size = mtx->nz_size;
    new_mtx->mat_size = mtx->mat_size;
    new_mtx->col_idx = (int*) malloc(mtx->nz_size * sizeof(int));
    new_mtx->row_idx = (int*) malloc((mtx->mat_size + 1) * sizeof(int));
    
    for (int i = 0; i < mtx->nz_size; i++)
    {
        new_mtx->col_idx[i] = mtx->col_idx[i];
        new_mtx->row_idx[mtx->row_idx[i] + 1]++;
    }
    for (int i = 0; i < mtx->mat_size; i++)
    {
        new_mtx->row_idx[i+1] += new_mtx->row_idx[i];
    }
}
