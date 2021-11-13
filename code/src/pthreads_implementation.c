#include <pthread.h>
#include <stdlib.h>
#include <utils.h>
#include "pthreads_implementation.h"
#define MAX(a,b) (((a)>(b))?(a):(b))

void* obj_function(void* args)
{
    struct objective_args *arguments = args;
    int b_of_work = 4;
    while(1)
    {
        pthread_mutex_lock(arguments->lock); 
        int k = (*(arguments->com));
        *(arguments->com) -= b_of_work;
        pthread_mutex_unlock(arguments->lock);
        if(k < 0)
            break;
        for(int i = k; i > k - b_of_work && i >= 0; i--)
        {
            //printf("HELLO %dstart %dstart\n", arguments->start, arguments->end);
            for(int j = arguments->mtx->row_idx[i]; j < arguments->mtx->row_idx[i+1]; j++)    // all non zero colums of i row. this is the actual mask
            {
                int col = arguments->mtx->col_idx[j];
                for(int k = arguments->mtx->row_idx[col]; k < arguments->mtx->row_idx[col+1]; k++)        // all non zero columns of row[col]
                {
                    int col1 = arguments->mtx->col_idx[k];
                    int succ;
                    if(arguments->mtx->row_idx[i+1] - arguments->mtx->row_idx[i] > 5)
                        succ = binarySearch(arguments->mtx->col_idx, arguments->mtx->row_idx[i], arguments->mtx->row_idx[i+1]-1, col1);
                    else
                        succ = linearSearch(arguments->mtx->col_idx, arguments->mtx->row_idx[i], arguments->mtx->row_idx[i+1]-1, col1);
                    if(succ!=-1)
                    {
                        arguments->mtx->val[j]++;
                    }
                }
            }
        }
    }
}

void triangle_counting_pthread_implementation(struct CSR_mtx *mtx, struct implementation_args *args)
{
    for(int i = 0; i < mtx->mat_size; i++)
    {
       if(mtx->row_idx[i+1] - mtx->row_idx[i] > 5)
           quickSortIterative(mtx->col_idx, mtx->row_idx[i], mtx->row_idx[i+1]-1);
    }
    int remaining_rows = mtx->mat_size % args->num_of_threads;
    struct objective_args *obj_args = (struct objective_args*) malloc(args->num_of_threads *sizeof(struct objective_args));
    int last_row = 0;
    pthread_t thread_id[args->num_of_threads];
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);
    int common_work = mtx->mat_size;
    for(int i = 0; i < args->num_of_threads; i++)
    {
        obj_args[i].mtx = mtx;
        obj_args[i].start = last_row;
        obj_args[i].end = last_row + ((int) mtx->mat_size / args->num_of_threads) + 1 * (remaining_rows-- > 0);
        obj_args[i].id = i;
        obj_args[i].com = &common_work;
        obj_args[i].lock = &lock;
        pthread_create(&thread_id[i], NULL, obj_function, &(obj_args[i]));
        last_row = obj_args[i].end;
    }

    for(int i = 0; i < args->num_of_threads; i++)
    {
        pthread_join(thread_id[i], NULL); 
    }
    //free(obj_args);
}



