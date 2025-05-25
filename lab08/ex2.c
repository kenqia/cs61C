#include "ex2.h"

double dotp_naive(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    for (int i = 0; i < arr_size; i++)
        global_sum += x[i] * y[i];
    return global_sum;
}

double dotp_critical(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    #pragma omp parallel for
    for(int i = 0 ; i < arr_size ; i++){
        double z = x[i] * y[i];
        #pragma omp critical
        {
            global_sum += z;
        }
    }
    return global_sum;
}


double dotp_reduction(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    #pragma omp parallel for reduction(+:global_sum)
    for(int i = 0 ; i < arr_size ; i++){
        global_sum += x[i] * y[i];
    }
    return global_sum;
}

double dotp_manual_reduction(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    #pragma omp parallel
    {
        double tmp = 0;
        #pragma omp for
        for(int i = 0 ; i < arr_size ; i++){
            tmp += x[i] * y[i];
        }
        #pragma omp critical
        {
            global_sum += tmp;
        }
    }
    return global_sum;
}
