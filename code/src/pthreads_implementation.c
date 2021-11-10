#include <pthread.h>
#include <utils.h>
#include <stdlib.h>
#include "pthreads_implementation.h"
#define MAX(a,b) (((a)>(b))?(a):(b))

void* obj_function(void* args)
{
    struct objective_args *arguments = args;
    int counter;
    //printf("HELLO %dstart %dstart\n", arguments->start, arguments->end);
    for(int i = arguments->start; i < arguments->end; i++)          //all rows      
    {
        for(int j = arguments->mtx->row_idx[i]; j < arguments->mtx->row_idx[i+1]; j++)    // all non zero colums of i row. this is the actual mask
        {
            int col = arguments->mtx->col_idx[j];
            for(int k = arguments->mtx->row_idx[col]; k < arguments->mtx->row_idx[col+1]; k++)        // all non zero columns of row[col]
            {
                int col1 = arguments->mtx->col_idx[k];
                int succ = binarySearch(arguments->mtx->col_idx, arguments->mtx->row_idx[i], arguments->mtx->row_idx[i+1]-1, col1);
                if(succ!=-1)
                {
                    pthread_mutex_lock( arguments->mutex1 );
                    *(arguments->triangles) += 1;
                    pthread_mutex_unlock( arguments->mutex1 );
                }
                counter++;
            }
        }
    }
}

float triangle_counting_pthread_implementation(struct CSR_mtx *mtx, struct implementation_args *args)
{
    uint total_triangles = 0;
    pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
    int remaining_rows = mtx->mat_size % args->num_of_threads;
    struct objective_args *obj_args = (struct objective_args*) malloc(args->num_of_threads *sizeof(struct objective_args));
    int last_row = 0;
    pthread_t thread_id[args->num_of_threads];
    for(int i = 0; i < args->num_of_threads; i++)
    {
        obj_args[i].mtx = mtx;
        obj_args[i].start = last_row;
        obj_args[i].end = last_row + ((int) mtx->mat_size / args->num_of_threads) + 1 * (remaining_rows-- > 0);
        obj_args[i].id = i;
        obj_args[i].triangles = &total_triangles;
        obj_args[i].mutex1 = &mutex1;
        pthread_create(&thread_id[i], NULL, obj_function, &(obj_args[i]));
        last_row = obj_args[i].end;
    }

    for(int i = 0; i < args->num_of_threads; i++)
    {
        pthread_join( thread_id[i], NULL); 
    }
    free(obj_args);
    if(args->full_mat)
    {
        total_triangles/=6.0;
    }
    return total_triangles;
}



