#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "utils.h"

int main(int argc, char** argv)
{
    int *row, *col, *row_indexes, *col_indexes;
    int nzlen, mat_size;
    struct COO_mtx *mtx_coo_fmt =  malloc(sizeof(struct COO_mtx));
    struct CSR_mtx *mtx_csr_fmt =  malloc(sizeof(struct CSR_mtx));
    read_matrix("data/belgium_osm.mtx", mtx_coo_fmt);
    coo_to_csr(mtx_coo_fmt, mtx_csr_fmt);
    free(mtx_coo_fmt);
    printf("\n%d\n",mtx_csr_fmt->col_idx[1]);
    
    return 0;
}