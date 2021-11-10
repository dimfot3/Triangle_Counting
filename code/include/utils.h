#ifndef _UTILS_H_
#define _UTILS_H_

struct COO_mtx
{
    int *row_idx;
    int *col_idx;
    int nz_size;
    int mat_size;
    int full_mat;
};

struct CSR_mtx
{
    int *row_idx;
    int *col_idx;
    uint *val;
    int nz_size;
    int mat_size;
    int full_mat;
};

struct CSC_mtx
{
    int *row_idx;
    int *col_idx;
    uint *val;
    int nz_size;
    int mat_size;
    int full_mat;
};

struct datasets
{
    char* path;
    char **list;
    int size;
};

struct implementation_args
{
    int num_of_threads;
    int verbose;
};

struct results
{
    float mean_time;
    float var_time;
    float *scal_time;   //this is used in scalability benchmarks
    uint triangles;
};

void read_matrix(char** path, struct COO_mtx* mtx, int full_mat);
void coo_to_csr(struct COO_mtx* mtx, struct CSR_mtx* new_mtx);
void coo_to_csc(struct COO_mtx* mtx, struct CSC_mtx* new_mtx);
void coo_to_csc(struct COO_mtx* mtx, struct CSC_mtx* new_mtx);
void list_dataset(struct datasets* dt);
int binarySearch(int arr[], int l, int r, int x);
int linearSearch(int arr[], int l, int r, int x);
uint get_triangles(struct CSR_mtx* mtx);
void restore_mat(struct CSR_mtx* mtx);
void swap(int* a, int* b);
int partition(int arr[], int l, int h);
void quickSortIterative(int arr[], int l, int h);

#endif