#include "ex1.h"

void v_add_naive(double* x, double* y, double* z) {
    #pragma omp parallel
    {
        for(int i=0; i<ARRAY_SIZE; i++)
            z[i] = x[i] + y[i];
    }
}

// Adjacent Method
void v_add_optimized_adjacent(double* x, double* y, double* z) {
    #pragma omp parallel
    {
        int threads_num = omp_get_num_threads();
        int threads_id = omp_get_thread_num();
        for (int i = threads_id; i < ARRAY_SIZE; i += threads_num) {
            z[i] = x[i] + y[i];
        }
    }
}

// Chunks Method
void v_add_optimized_chunks(double* x, double* y, double* z) {
    #pragma omp parallel
    {
        int threads_num = omp_get_num_threads();
        int threads_id = omp_get_thread_num();
        int chunk_size = ARRAY_SIZE / threads_num;
        int start = threads_id * chunk_size;
        int end = (threads_id == threads_num - 1) ? ARRAY_SIZE : start + chunk_size;
        for (int i = start; i < end; i++) {
            z[i] = x[i] + y[i];
        }
    }
}
