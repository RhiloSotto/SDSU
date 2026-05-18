#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <chrono>

#include <fftw3.h>
#include <cufft.h>
#include <cuda_runtime.h>

#include "WavFile.h"

using namespace std;

#define BUFF_SIZE 16384

#define CUDA_CHECK(ans) { gpuAssert((ans), __FILE__, __LINE__); }

inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
    if (code != cudaSuccess)
    {
        fprintf(stderr, "GPUassert: %s %s %d\n",
                cudaGetErrorString(code), file, line);
        if (abort) exit(code);
    }
}

// ---------------- FILTER FUNCTION ----------------
void remove10k(double *re, double *im, int N, double Fs)
{
    int k = (int)(10000.0 * N / Fs);
    int bw = 30;

    for (int i = k - bw; i <= k + bw; i++)
    {
        if (i >= 0 && i < N)
        {
            re[i] = 0.0;
            im[i] = 0.0;
        }

        int mirror = N - i;
        if (mirror >= 0 && mirror < N)
        {
            re[mirror] = 0.0;
            im[mirror] = 0.0;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <input.wav>\n", argv[0]);
        return 1;
    }

    const char *wavfile = argv[1];

    char wavfileout[256];
    sprintf(wavfileout, "%s_out.wav", argv[0]);

    WavInFile inFile(wavfile);
    WavOutFile outFile(wavfileout,
                       inFile.getSampleRate(),
                       inFile.getNumBits(),
                       inFile.getNumChannels());

    printf("SampleRate: %d Hz\n", inFile.getSampleRate());
    printf("Channels: %d\n", inFile.getNumChannels());

    int N = BUFF_SIZE;
    double Fs = inFile.getSampleRate();

    SAMPLETYPE sampleBuffer[BUFF_SIZE];
    short outputBuffer[BUFF_SIZE];

    
    // FFTW (CPU)
    
    fftw_complex *cin = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*N);
    fftw_complex *cout = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*N);
    fftw_complex *cout2 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*N);

    fftw_plan cpu_fwd = fftw_plan_dft_1d(N, cin, cout, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan cpu_inv = fftw_plan_dft_1d(N, cout, cout2, FFTW_BACKWARD, FFTW_ESTIMATE);

    
    // cuFFT (GPU)
    
    cufftHandle plan;
    cufftDoubleComplex *d_data;
    cufftDoubleComplex *h_data;

    CUDA_CHECK(cudaMalloc((void**)&d_data, sizeof(cufftDoubleComplex)*N));
    h_data = (cufftDoubleComplex*)malloc(sizeof(cufftDoubleComplex)*N);

    cufftPlan1d(&plan, N, CUFFT_Z2Z, 1);

    
    // TIMING
    
    double cpu_time = 0.0;
    double gpu_time = 0.0;
    int blocks = 0;

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    while (!inFile.eof())
    {
        size_t samplesRead = inFile.read(sampleBuffer, N);

        
        // CPU INPUT
        
        for (int i = 0; i < N; i++)
        {
            cin[i][0] = (i < samplesRead) ? sampleBuffer[i] : 0.0;
            cin[i][1] = 0.0;
        }

        
        // CPU PIPELINE
        
        auto cpu_start = chrono::high_resolution_clock::now();

        fftw_execute(cpu_fwd);

        double cre[N], cim[N];
        for (int i = 0; i < N; i++)
        {
            cre[i] = cout[i][0];
            cim[i] = cout[i][1];
        }

        remove10k(cre, cim, N, Fs);

        for (int i = 0; i < N; i++)
        {
            cout[i][0] = cre[i];
            cout[i][1] = cim[i];
        }

        fftw_execute(cpu_inv);

        auto cpu_end = chrono::high_resolution_clock::now();
        cpu_time += chrono::duration<double, milli>(cpu_end - cpu_start).count();

        
        // GPU PIPELINE
        
        for (int i = 0; i < N; i++)
        {
            h_data[i].x = cout[i][0];
            h_data[i].y = cout[i][1];
        }

        CUDA_CHECK(cudaMemcpy(d_data, h_data,
            sizeof(cufftDoubleComplex)*N,
            cudaMemcpyHostToDevice));

        cudaEventRecord(start);

        cufftExecZ2Z(plan, d_data, d_data, CUFFT_FORWARD);
        cudaDeviceSynchronize();

        CUDA_CHECK(cudaMemcpy(h_data, d_data,
            sizeof(cufftDoubleComplex)*N,
            cudaMemcpyDeviceToHost));

        // FILTER ON GPU DATA
        double gre[N], gim[N];
        for (int i = 0; i < N; i++)
        {
            gre[i] = h_data[i].x;
            gim[i] = h_data[i].y;
        }

        remove10k(gre, gim, N, Fs);

        for (int i = 0; i < N; i++)
        {
            h_data[i].x = gre[i];
            h_data[i].y = gim[i];
        }

        CUDA_CHECK(cudaMemcpy(d_data, h_data,
            sizeof(cufftDoubleComplex)*N,
            cudaMemcpyHostToDevice));

        cufftExecZ2Z(plan, d_data, d_data, CUFFT_INVERSE);
        cudaDeviceSynchronize();

        cudaEventRecord(stop);
        cudaEventSynchronize(stop);

        float ms = 0;
        cudaEventElapsedTime(&ms, start, stop);
        gpu_time += ms;
        blocks++;

        CUDA_CHECK(cudaMemcpy(h_data, d_data,
            sizeof(cufftDoubleComplex)*N,
            cudaMemcpyDeviceToHost));

        
        // OUTPUT
        
        for (int i = 0; i < N; i++)
        {
            double val = h_data[i].x / N;

            if (val > 32767) val = 32767;
            if (val < -32768) val = -32768;

            outputBuffer[i] = (short)val;
        }

        outFile.write(outputBuffer, N);
    }

    
    // RESULTS
    
    printf("\n===== FINAL RESULTS =====\n");
    printf("CPU FFTW time: %f ms\n", cpu_time);
    printf("GPU cuFFT avg time: %f ms\n", gpu_time / blocks);
    printf("Speedup (CPU/GPU): %fx\n", cpu_time / gpu_time);

    
    // CLEANUP
    fftw_destroy_plan(cpu_fwd);
    fftw_destroy_plan(cpu_inv);

    fftw_free(cin);
    fftw_free(cout);
    fftw_free(cout2);

    cufftDestroy(plan);
    cudaFree(d_data);
    free(h_data);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    return 0;
}