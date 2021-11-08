#include <stdlib.h>
#include <stdio.h>
#include "mmio.h"
#include "utils.h"
#include <dirent.h>
#include <time.h>

void read_matrix(char** path, struct COO_mtx* mtx, int full_mat)
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
    int half_nz = nz;
    if(full_mat)
    {
        nz *= 2;
    }
    // reseve memory for matrices 
    I = (int *) malloc(nz * sizeof(int));
    J = (int *) malloc(nz * sizeof(int));
    for (i=0; i<half_nz; i++)
    {
        fscanf(f, "%d %d\n", &I[i], &J[i]);
        I[i]--;  /* adjust from 1-based to 0-based */
        J[i]--;
        if(full_mat)
        {
            I[i + half_nz] = J[i];
            J[i + half_nz] = I[i];
        }
    }
    if (f !=stdin) fclose(f);

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    printf("\nSuccessfully loaded matrix in %0.2f seconds.\nMatrix info:\nRow,Cols:%d, %d\nnon zero values: %d \
    \nfirst nz position: %d %d\nlast nz position: %d %d\n", 
    time_taken, M, N, nz,I[0]+1, J[0]+1, I[nz-1]+1, J[nz-1]+1);
    
    mtx->nz_size = nz;
    mtx->mat_size = M;
    mtx->row_idx = I;
    mtx->col_idx = J;
}

//sparce matrix from coordinate list to compressed row index ***indexes doesNOT need to be sorted***! COMPLEXITY: (O(nz(A)+ N))
void coo_to_csr(struct COO_mtx* mtx, struct CSR_mtx* new_mtx)
{
    clock_t t = clock();
    new_mtx->nz_size = mtx->nz_size;
    new_mtx->mat_size = mtx->mat_size;
    new_mtx->col_idx = (int*) calloc(mtx->nz_size, sizeof(int));
    new_mtx->row_idx = (int*) calloc((mtx->mat_size + 1), sizeof(int));
    //calculation of the compressed row indexes of len n+1
    for(int i = 0; i < new_mtx->nz_size; i++){            
        new_mtx->row_idx[mtx->row_idx[i]]++;
    }
    for(int i = 0, cumsum = 0; i < mtx->mat_size; i++){     
        int temp = new_mtx->row_idx[i];
        new_mtx->row_idx[i] = cumsum;
        cumsum += temp;
    }
    new_mtx->row_idx[mtx->mat_size] = mtx->nz_size; 
    //calculation of the column indexes
    for(int i = 0; i < new_mtx->nz_size; i++){
        int row  = mtx->row_idx[i];
        int dest = new_mtx->row_idx[row];
        new_mtx->col_idx[dest] = mtx->col_idx[i];
        new_mtx->row_idx[row]++;
    }
    //restoring the row indexes
    for(int i = 0, last = 0; i <= new_mtx->mat_size; i++){
        int temp = new_mtx->row_idx[i];
        new_mtx->row_idx[i]  = last;
        last = temp;
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\nSuccessfully transformed matrix from COO to CSR in %0.3f seconds\n", time_taken);
}

//sparce matrix from coordinate list to compressed col indexes ***indexes doesNOT need to be sorted***! COMPLEXITY: (O(nz(A)+ N))
void coo_to_csc(struct COO_mtx* mtx, struct CSC_mtx* new_mtx)
{
    clock_t t = clock();
    new_mtx->nz_size = mtx->nz_size;
    new_mtx->mat_size = mtx->mat_size;
    new_mtx->row_idx = (int*) calloc(mtx->nz_size, sizeof(int));
    new_mtx->col_idx = (int*) calloc((mtx->mat_size + 1), sizeof(int));

    //calculation of the compressed col indexes of len n+1
    for(int i = 0; i < new_mtx->nz_size; i++){            
        new_mtx->col_idx[mtx->col_idx[i]]++;
    }
    for(int i = 0, cumsum = 0; i < mtx->mat_size; i++){     
        int temp = new_mtx->col_idx[i];
        new_mtx->col_idx[i] = cumsum;
        cumsum += temp;
    }
    new_mtx->col_idx[mtx->mat_size] = mtx->nz_size;
   
    //calculation of the row indexes of len nzlen
    for(int i = 0; i < new_mtx->nz_size; i++){
        int col  = mtx->col_idx[i];
        int dest = new_mtx->col_idx[col];
        new_mtx->row_idx[dest] = mtx->row_idx[i];
        new_mtx->col_idx[col]++;
    }
    //restoring the col indexes
    for(int i = 0, last = 0; i <= new_mtx->mat_size; i++){
        int temp = new_mtx->col_idx[i];
        new_mtx->col_idx[i]  = last;
        last = temp;
    }

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\nSuccessfully transformed matrix from COO to CSC in %0.3f seconds\n", time_taken);
}


void list_dataset(struct datasets* dt)
{
    DIR *d;
    struct dirent *dir;
    int size_of_path = strlen(dt->path);
    dt->size = 0;
    d = opendir(dt->path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            const char *ext = strrchr(dir->d_name,'.');
            if((!ext) || (ext == dir->d_name))
                continue;
            else if(strcmp(ext, ".mtx") == 0)
            {
                dt->size++;
            }
        }
    }
    closedir(d);
    d = opendir(dt->path);
    dt->list = (char**) malloc(sizeof(char*)*dt->size);
    int counter = 0;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            const char *ext = strrchr(dir->d_name,'.');
            if((!ext) || (ext == dir->d_name))
                continue;
            else if(strcmp(ext, ".mtx") == 0)
            {
                int size_of_dataset = strlen(dir->d_name);
                dt->list[counter] = (char*)malloc((size_of_path + size_of_dataset + 1) * sizeof(char));
                for(int i = 0; i < size_of_path; i++)
                {
                    dt->list[counter][i] = dt->path[i];
                }
                for(int i = size_of_path; i < size_of_path + size_of_dataset; i++)
                {
                    dt->list[counter][i] = dir->d_name[i-size_of_path];
                }
                dt->list[counter][size_of_dataset+size_of_path] = '\0';
                counter++;
            }
        }    
        

    }
    closedir(d);
}

int binarySearch(int arr[], int l, int r, int x)
{
    if (r >= l) {
        int mid = l + (r - l) / 2;
 
        if (arr[mid] == x)
            return mid;
 
        if (arr[mid] > x)
            return binarySearch(arr, l, mid - 1, x);
 
        return binarySearch(arr, mid + 1, r, x);
    }

    return -1;
}