#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include "utils.h"
#include <sequential_masked_implementation.h>
#include <pthreads_implementation.h>
#include "benchmarks.h"
#include <openmp_implementation.h>

struct session_args
{
    char *name;
    int ses_option;     //0:sequential, 1:pthreads, 2:openmp, 3:opencilk
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
    ses_args->ses_option = 0;
    ses_args->num_of_loops = 1;
    ses_args->bechmark_option = 0;
    ses_args->full_mat = 1;
    ses_args->num_of_threads = 1;
    if(argc > 2)
    {
        ses_args->ses_option = atoi(argv[1]);
        ses_args->bechmark_option = atoi(argv[2]);
        ses_args->num_of_loops = atoi(argv[3]);
        ses_args->num_of_threads = atoi(argv[4]);
        ses_args->full_mat = atoi(argv[5]);
    }
    return;
}

void run_session(struct session_args *ses_args)
{

    FILE *f = fopen(ses_args->name, "w");
    fprintf(f, "ses_option %d \ benchmark_option %d \ full_mat %d \ num_of_loops %d\ num_of_threads %d\n", ses_args->ses_option, ses_args->bechmark_option, 
            ses_args->full_mat, ses_args->num_of_loops, ses_args->num_of_threads);
    struct datasets *dt = (struct datasets*)malloc(sizeof(struct datasets));
    dt->path = (char*)malloc(sizeof(char)*6);
    dt->path = "data/\0";
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
        if(ses_args->ses_option==0){
            time_bechmark(triagle_counting_sequential_masked_implementation, mtx_csr_fmt, ses_args->num_of_loops, ses_args->num_of_threads, &res);
            fprintf(f, "%s mean time: %f var time: %f triangle_num: %d\n", dt->list[i], res.mean_time, res.var_time, res.triangles);
        }
        else if(ses_args->ses_option==1 && ses_args->bechmark_option==0){
            time_bechmark(triangle_counting_pthread_implementation, mtx_csr_fmt, ses_args->num_of_loops, ses_args->num_of_threads, &res);
            fprintf(f, "%s mean time: %f var time: %f triangle_num: %d\n", dt->list[i], res.mean_time, res.var_time, res.triangles);
        }
        else if(ses_args->ses_option==2 && ses_args->bechmark_option==0){
            time_bechmark(triangle_counting_openmp_implementation, mtx_csr_fmt, ses_args->num_of_loops, ses_args->num_of_threads, &res);
            fprintf(f, "%s mean time: %f var time: %f triangle_num: %d\n", dt->list[i], res.mean_time, res.var_time, res.triangles);
        }
        else if(ses_args->ses_option==1 && ses_args->bechmark_option==1){
            scalability_bechmark(triangle_counting_pthread_implementation, mtx_csr_fmt, ses_args->num_of_loops, ses_args->num_of_threads, &res);
            fprintf(f, "%s triangle_num: %d times [", dt->list[i], res.triangles);
            for(int j = 0; j < ses_args->num_of_threads; j++)
            {
                fprintf(f, " %f ", res.scal_time[j]);
            }
            fprintf(f, "]\n");
        }
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