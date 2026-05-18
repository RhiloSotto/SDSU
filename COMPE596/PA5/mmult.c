#include <stdio.h>
#include <stdlib.h>

#define cudaCheckError() { \
    cudaError_t e=cudaGetLastError(); \
    if(e!=cudaSuccess) { \
        printf("Cuda failure %s:%d: '%s'\n",__FILE__,__LINE__,cudaGetErrorString(e)); \
        exit(EXIT_FAILURE); \
    } \
}

#define TILE_WIDTH 16

// basic matrix multiply kernel
__global__ void matrixMultiply(float* M, float* N, float* P, int Width) {
    int Row = blockIdx.y * blockDim.y + threadIdx.y;
    int Col = blockIdx.x * blockDim.x + threadIdx.x;
    float Pvalue = 0;

    // Participating threads
    if (Row < Width && Col < Width) {
        for (int i = 0; i < Width; i++) {
            Pvalue += M[Row*Width + i] * N[i*Width + Col];
        }
        P[Row*Width+Col] = Pvalue;
    }
}

// tiled matrix multiplacation kernel
__global__ void matrixTiledMultiply(float* M, float* N, float* P, int Width) {
    __shared__ float ds_M[TILE_WIDTH][TILE_WIDTH];
    __shared__ float ds_N[TILE_WIDTH][TILE_WIDTH];

    int bx = blockIdx.x; int by = blockIdx.y;
    int tx = threadIdx.x; int ty = threadIdx.y;

    int Row = by * blockDim.y + ty;
    int Col = bx * blockDim.x + tx;
    float Pvalue = 0;

    // Loop over the M and N tiles required to compute the P element
    for (int p = 0; p < (Width + TILE_WIDTH - 1) / TILE_WIDTH; p++) {
        // Collaborative loading of M and N tiles into shared memory
        if (Row < Width && p*TILE_WIDTH + tx < Width) {
            ds_M[ty][tx] = M[Row*Width + p*TILE_WIDTH+tx];
        }
        else {
            ds_M[ty][tx] = 0;
        } 
        if (Col < Width && p*TILE_WIDTH + ty < Width) {
            ds_N[ty][tx] = N[(p*TILE_WIDTH+ty)*Width + Col];
        }
        else {
            ds_N[ty][tx] = 0;

        }
        __syncthreads();
        for (int i = 0; i < TILE_WIDTH; ++i) {
            Pvalue += ds_M[ty][i] * ds_N[i][tx];
        }
        __syncthreads();
    }
    if (Row < Width && Col < Width) {
        P[Row*Width+Col] = Pvalue;
    }
}


int main(int argc, char* argv[]) {
    // read inputs
    unsigned int pow2 = atoi(argv[1]);
    printf("N = %d\n", pow2);
    unsigned int dim = (1 << pow2);

    // matrix dimensions
    unsigned int m, p, n;
    m = dim;
    p = dim;
    n = dim;

    // allocate host memory for A, B, C arrays
    float* hA = (float*)calloc(m*p, sizeof(float));
    float* hB = (float*)calloc(p*n, sizeof(float));
    float* hC = (float*)calloc(m*n, sizeof(float));

    // fill A and B with random values
    srand(0);
    for (unsigned int i = 0; i < m*p; i++) {
        hA[i] = rand() / (float)RAND_MAX;
    }
    for (unsigned int i = 0; i < p*n; i++) {
        hB[i] = rand() / (float)RAND_MAX;
    }


    // DEVICE START //

    // initalize and allocate device memory
    float* dA, *dB, *dC;
    cudaMalloc(&dA, sizeof(float) * m * p);
    cudaMalloc(&dB, sizeof(float) * p * n);
    cudaMalloc(&dC, sizeof(float) * m * n);
    // copy host memory to device
    cudaMemcpy(dA, hA, sizeof(float) * m * p, cudaMemcpyHostToDevice);
    cudaMemcpy(dB, hB, sizeof(float) * p * n, cudaMemcpyHostToDevice);
    // initalize thread block and kernel grid dimensions
    dim3 dimBlock_basic(16,16);
    dim3 dimGrid_basic((dim + 15)/16, (dim + 15)/16);
    dim3 dimBlock_tiled(TILE_WIDTH, TILE_WIDTH);
    dim3 dimGrid_tiled((dim + TILE_WIDTH - 1)/TILE_WIDTH, (dim + TILE_WIDTH - 1)/TILE_WIDTH);
    // time measurements (measure only the kernel invocations)
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    // warm up drivers
    matrixMultiply<<<1,1>>>(dA,dB,dC,1);
    cudaDeviceSynchronize();
    // invoke CUDA kernel
    cudaFree(0);
    cudaEventRecord(start);
    matrixMultiply<<<dimGrid_basic, dimBlock_basic>>>(dA, dB, dC, dim);
    cudaCheckError();
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms_basic = 0;
    cudaEventElapsedTime(&ms_basic, start, stop);

    cudaMemset(dC, 0, sizeof(float) * m * n);
    // invoke CUDA kernel
    cudaEventRecord(start);
    matrixTiledMultiply<<<dimGrid_tiled, dimBlock_tiled>>>(dA, dB, dC, dim);
    cudaCheckError();
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms_tiled = 0;
    cudaEventElapsedTime(&ms_tiled, start, stop);
    // copy results from device to host
    cudaDeviceSynchronize();
    cudaMemcpy(hC, dC, sizeof(float) * m * n, cudaMemcpyDeviceToHost);
    // deallocate device memory
    cudaFree(dA);
    cudaFree(dB);
    cudaFree(dC);
    // DEVICE END //


    // RESULTS //
    printf("basic: %f, tiled: %f\n", ms_basic, ms_tiled);

    // deallocate host memory
    free(hA);
    free(hB);
    free(hC);
}