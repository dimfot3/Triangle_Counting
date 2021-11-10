#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include "utils.h"
#include "benchmarks.h"
#include "triangle_counting_cilk_implementation.h"

struct session_args
{
    char *name;
    int bechmark_option;    //0: time benchmark, 1:scalability  (only for parallel implementation)
    int num_of_loops;
    int full_mat;
    int num_of_threads;
};

struct tm buf;
void set_args(int argc, char** argv, struct session_args* ses_args)
{
    time_t t = time(NULL);
    struct tm *timeptr;
    timeptr = localtime(&t);
    ses_args->name = (char*)malloc(sizeof(char)*40);
    assert(strftime(ses_args->name,50,"results/ses_%d_%m_%y_%H_%M_%S.txt", timeptr));
    ses_args->num_of_loops = 1;
    ses_args->bechmark_option = 0;
    ses_args->full_mat = 0;
    ses_args->num_of_threads = 0;
    if(argc > 2)
    {
        ses_args->bechmark_option = atoi(argv[1]);
        ses_args->num_of_loops = atoi(argv[2]);
        ses_args->num_of_threads = atoi(argv[3]);
        ses_args->full_mat = atoi(argv[4]);
    }
    return;
}

void run_session(struct session_args *ses_args)
{

    FILE *f = fopen(ses_args->name, "w");
    fprintf(f, "ses_option cilk \ benchmark_option %d \ full_mat %d \ num_of_loops %d\n", ses_args->bechmark_option, 
            ses_args->full_mat, ses_args->num_of_loops);
    struct datasets *dt = (struct datasets*)malloc(sizeof(struct datasets));
    dt->path = (char*)malloc(sizeof(char)*6);
    dt->path = "../data/\0";
    list_dataset(dt);
    for(int i = 0; i < dt->size; i++)
    {   
        printf("%s\n", dt->list[i]);
        struct COO_mtx *mtx_coo_fmt =  malloc(sizeof(struct COO_mtx));
        struct CSR_mtx *mtx_csr_fmt =  malloc(sizeof(struct CSR_mtx));
        read_matrix(dt->list[i], mtx_coo_fmt, ses_args->full_mat);
        coo_to_csr(mtx_coo_fmt, mtx_csr_fmt);
        float num_of_triangles = 0;
        struct results res;
        time_bechmark(triangle_counting_cilk_implementation, mtx_csr_fmt, ses_args->num_of_loops, ses_args->num_of_threads, &res);
        fprintf(f, "%s mean time: %f var time: %f triangle_num: %d\n", dt->list[i], res.mean_time, res.var_time, res.triangles);
        free(mtx_coo_fmt);
        free(mtx_csr_fmt);
    }
    fclose(f);
}

int main(int argc, char** argv)
{
    struct session_args *ses_args = (struct session_args*)malloc(sizeof(struct session_args));
    set_args(argc, argv, ses_args);
    run_session(ses_args);
    
    return 0;
}