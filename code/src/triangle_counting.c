#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "utils.h"
#include <sequential_masked_implementation.h>

int main(int argc, char** argv)
{
    struct COO_mtx *mtx_coo_fmt =  malloc(sizeof(struct COO_mtx));
    struct CSR_mtx *mtx_csr_fmt =  malloc(sizeof(struct CSR_mtx));
    read_matrix("data/belgium_osm.mtx", mtx_coo_fmt, 1);
    coo_to_csr(mtx_coo_fmt, mtx_csr_fmt);
    float num = triagle_counting_sequential_masked_implementation(mtx_csr_fmt);
    return 0;
}