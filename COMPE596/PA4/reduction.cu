#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define cudaCheckError() { \
    cudaError_t e=cudaGetLastError(); \
    if(e!=cudaSuccess) { \
        printf("Cuda failure %s:%d: '%s'\n",__FILE__,__LINE__,cudaGetErrorString(e)); \
        exit(EXIT_FAILURE); \
    } \
} \


#define BLOCK_SIZE 512 // You can change this

// device 
__global__ void reduction(float *input, float *output, int len) {
    // Load a segment of the input vector into shared memory
    __shared__ float partialSum[2 * BLOCK_SIZE];
    unsigned int t = threadIdx.x, start = 2 * blockIdx.x * BLOCK_SIZE;
    if (start + t < len)
        partialSum[t] = input[start + t];
    else
        partialSum[t] = 0;
    if (start + BLOCK_SIZE + t < len)
        partialSum[BLOCK_SIZE + t] = input[start + BLOCK_SIZE + t];
    else
        partialSum[BLOCK_SIZE + t] = 0;
    // Traverse the reduction tree

    /*
    strides will assume values:
    512
    256
    128
    64
    32
    16
    8
    4
    2
    1
    */
    for (unsigned int stride = BLOCK_SIZE; stride >= 1; stride >>= 1) {
        __syncthreads();
        if (t < stride)
            partialSum[t] += partialSum[t + stride];
    }
    // Write the computed sum of the block to the output vector at the
    // correct index
    if (t == 0)
        output[blockIdx.x] = partialSum[0];
}


// host
int main(int argc, char* argv[]) {

    // READ INPUTS
    unsigned int N = atoi(argv[1]);
    printf("N = %d\n", N);
    // 2^N elements
    unsigned int numInputElements = (1 << N);

    // allocate memory on host
    float *hostInput = (float*)calloc(numInputElements, sizeof(float));
    // initialize input array
    for (unsigned int i = 1; i < numInputElements; ++i) {
        hostInput[i] = (float)i;
    }

    // PERFORM SEQUENTIAL SUM ON HOST
    clock_t t;
    t = clock();
    // TODO: host computation
    float hostOutput = 0.0f;
    for (unsigned int i = 0; i < numInputElements; ++i) {
        hostOutput += hostInput[i];
    }

    t = clock() - t;
    float cpuElapsedTime = ((float)t/CLOCKS_PER_SEC * 1000.0);
    printf("host result: %f\n", hostOutput);
    printf("host elapsed time: %f ms\n" , cpuElapsedTime);


    // PERFORM SEQUENTIAL SUM ON DEVICE
    float *deviceInput, *deviceOutput;

    // allocate memory on device
    unsigned int numBlocks = (numInputElements + (2*BLOCK_SIZE - 1)) / (2*BLOCK_SIZE);
    cudaMalloc((void**)&deviceInput, numInputElements * sizeof(float));
    cudaMalloc((void**)&deviceOutput, numBlocks * sizeof(float));
    // copy host inputs to device
    cudaMemcpy(deviceInput, hostInput, numInputElements * sizeof(float), cudaMemcpyHostToDevice);

    cudaEvent_t start, stop;
    float gpuElapsedTime;

    cudaEventCreate(&start);
    cudaEventRecord(start,0);

    // invoke reduction kernel
    dim3 dimGrid(numBlocks, 1, 1);
    dim3 dimBlock(BLOCK_SIZE, 1, 1); 
    reduction<<<dimGrid, dimBlock>>>(deviceInput, deviceOutput, numInputElements);

    cudaCheckError();

    cudaEventCreate(&stop);
    cudaEventRecord(stop,0);
    cudaEventSynchronize(stop);

    // copy from device back to host
    float *hostPartial = (float*)malloc(numBlocks * sizeof(float));
    cudaMemcpy(hostPartial, deviceOutput, numBlocks * sizeof(float), cudaMemcpyDeviceToHost);

    float gpuOutput = 0.0f;
    for (unsigned int i = 0; i < numBlocks; i++) {
        gpuOutput += hostPartial[i];
    }

    cudaEventElapsedTime(&gpuElapsedTime, start, stop);
    
    printf("device result: %f\n", gpuOutput);
    printf("device elapsed time: %f ms\n", gpuElapsedTime);

    // report result as speedup (sequential/parallel)
    float speedup = cpuElapsedTime/gpuElapsedTime;
    printf("Speedup = %f\n", speedup);

    // free allocated memory
    cudaFree(deviceInput);
    cudaFree(deviceOutput);
    free(hostInput);
    free(hostPartial);

    return 0;
}