/*
 * Compile:
 * nvcc -c -I/usr/local/cuda/include lu.c
 * gcc -o gpulu lu.o -L/usr/local/cuda/lib64 -lcusolver -lcudart
 */

#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <cusolverDn.h>

#define MAX_POW 10   // up to N = 2^10 = 1024

// Generate Hilbert matrix (column-major)
void generateHilbert(double* A, int N) {
    int i, j;
    for (j = 0; j < N; j++) {
        for (i = 0; i < N; i++) {
            A[j*N + i] = 1.0 / (i + j + 1);
        }
    }
}

// Fill RHS with 1s
void generateB(double* B, int N) {
    int i;
    for (i = 0; i < N; i++) B[i] = 1.0;
}

// Add random perturbation to RHS
void perturbB(double* B, int N) {
    int i;
    for (i = 0; i < N; i++) {
        double eps = (double)rand() / RAND_MAX; // 0 < eps < 1
        B[i] += eps;
    }
}

int main() {
    cusolverDnHandle_t cusolverH = NULL;
    cudaStream_t stream = NULL;

    cusolverDnCreate(&cusolverH);
    cudaStreamCreateWithFlags(&stream, cudaStreamNonBlocking);
    cusolverDnSetStream(cusolverH, stream);

    // random seed
    srand(1);

    int p;
    for (p = 1; p <= MAX_POW; p++) {

        int N = 1 << p;
        int lda = N, ldb = N;

        printf("\n===== N = %d =====\n", N);

        // Host allocations
        double* A = (double*)malloc(sizeof(double) * N * N);
        double* B = (double*)malloc(sizeof(double) * N);
        double* B2 = (double*)malloc(sizeof(double) * N);

        generateHilbert(A, N);
        generateB(B, N);

        int i;
        for (i = 0; i < N; i++) B2[i] = B[i];
        perturbB(B2, N);

        // Device allocations
        double *d_A, *d_B;
        int* d_info;
        cudaMalloc((void**)&d_A, sizeof(double) * N * N);
        cudaMalloc((void**)&d_B, sizeof(double) * N);
        cudaMalloc((void**)&d_info, sizeof(int));

        cudaMemcpy(d_A, A, sizeof(double) * N * N, cudaMemcpyHostToDevice);
        cudaMemcpy(d_B, B, sizeof(double) * N, cudaMemcpyHostToDevice);

        // Workspace
        int lwork = 0;
        cusolverDnDgetrf_bufferSize(cusolverH, N, N, d_A, lda, &lwork);

        double* d_work;
        cudaMalloc((void**)&d_work, sizeof(double) * lwork);

        cudaEvent_t start1, stop1, start2, stop2;
        cudaEventCreate(&start1);
        cudaEventCreate(&stop1);
        cudaEventCreate(&start2);
        cudaEventCreate(&stop2);

        float time1, time2;

        // ---- FIRST SOLVE (factorization + solve) ----
        cudaEventRecord(start1, 0);

        cusolverDnDgetrf(cusolverH, N, N, d_A, lda, d_work, NULL, d_info);
        cusolverDnDgetrs(cusolverH, CUBLAS_OP_N, N, 1, d_A, lda, NULL, d_B, ldb, d_info);

        cudaEventRecord(stop1, 0);
        cudaEventSynchronize(stop1);
        cudaEventElapsedTime(&time1, start1, stop1);

        // ---- SECOND SOLVE (reuse LU) ----
        cudaMemcpy(d_B, B2, sizeof(double) * N, cudaMemcpyHostToDevice);

        cudaEventRecord(start2, 0);
        cusolverDnDgetrs(cusolverH, CUBLAS_OP_N, N, 1, d_A, lda, NULL, d_B, ldb, d_info);
        cudaEventRecord(stop2, 0);
        cudaEventSynchronize(stop2);
        cudaEventElapsedTime(&time2, start2, stop2);

        printf("Time (LU + solve): %f ms\n", time1);
        printf("Time (solve only): %f ms\n", time2);

        // Cleanup
        free(A); free(B); free(B2);
        cudaFree(d_A); cudaFree(d_B); cudaFree(d_info); cudaFree(d_work);

        cudaEventDestroy(start1); cudaEventDestroy(stop1);
        cudaEventDestroy(start2); cudaEventDestroy(stop2);
    }

    cusolverDnDestroy(cusolverH);
    cudaStreamDestroy(stream);
    cudaDeviceReset();

    return 0;
}