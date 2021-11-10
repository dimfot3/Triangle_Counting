#ifndef _PTHREADS_IMPLEMENTATION_H_
#define _PTHREADS_IMPLEMENTATION_H_

struct objective_args
{
    struct CSR_mtx *mtx;
    int start;      //start of rows
    int end;        //end of rows
    int id;         //num of thread
    int* C;         //in case you want to calculate the C and not the triangles
};

void triangle_counting_pthread_implementation(struct CSR_mtx *mtx, struct implementation_args *args);
void *obj_function(void* args);
#endif