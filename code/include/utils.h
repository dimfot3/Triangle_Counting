#ifndef _UTILS_H_
#define _UTILS_H_

struct COO_mtx
{
    int *row_idx;
    int *col_idx;
    int nz_size;
    int mat_size;
};

struct CSR_mtx
{
    int *row_idx;
    int *col_idx;
    int nz_size;
    int mat_size;
};

struct CSC_mtx
{
    int *row_idx;
    int *col_idx;
    int nz_size;
    int mat_size;
};

void read_matrix(char** path, struct COO_mtx* mtx);
void coo_to_csr(struct COO_mtx* mtx, struct CSR_mtx* new_mtx);
void coo_to_csc(struct COO_mtx* mtx, struct CSC_mtx* new_mtx);
#endif