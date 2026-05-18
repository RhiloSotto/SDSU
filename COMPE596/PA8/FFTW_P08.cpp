#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fftw3.h>
#include <cufft.h>
#include <cuda_runtime.h>

#include <soundtouch/SoundTouch.h>

#include "WavFile.h"

using namespace std;
using namespace soundtouch;

#define BUFF_SIZE           16384
#define MAX_FREQ            48//KHz

#define CUDA_CHECK(ans)				\
  { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line,
		      bool abort = true) {
  if (code != cudaSuccess) {
    fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code),
	    file, line);
    if (abort)
      exit(code);
  }
}

int main(int argc, char *argv[]) {

  const char *wavfile; // tuning_fork.wav
  if( argc != 2 )
    {
      fprintf(stderr,"usage: %s <input.wav>\n", argv[0]);
      exit(1);
    }
  else
    {
      wavfile = argv[1];
    }

  char *wavfileout = (char *)malloc(strlen(argv[0]) + strlen("_out.wav") + 1);
  char *logfile = (char *)malloc(strlen(argv[0]) + strlen("_out.log") + 1);
  wavfileout = strcat(strcpy(wavfileout, argv[0]),"_out.wav");
  logfile = strcat(strcpy(logfile, argv[0]),"_out.log");

  FILE *log;
  fftw_complex *in, *out, *out2; // typedef double fftw_complex[2];
  in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * BUFF_SIZE);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * BUFF_SIZE);
  out2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * BUFF_SIZE); // IFFT

  fftw_plan plan = fftw_plan_dft_1d(BUFF_SIZE, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_plan plan2 = fftw_plan_dft_1d(BUFF_SIZE, out, out2, FFTW_BACKWARD, FFTW_ESTIMATE);

  // https://docs.nvidia.com/cuda/cufft/index.html
  cufftHandle cuplan;
  // cufftReal *cuRealData;
  cufftDoubleComplex *d_DoubleComplexData;
  cufftDoubleComplex *h_DoubleComplexData;
  int nx = BUFF_SIZE;
  cudaMalloc((void**)&d_DoubleComplexData, nx * sizeof(cufftDoubleComplex));
  h_DoubleComplexData = (cufftDoubleComplex *)calloc(nx, sizeof(cufftDoubleComplex));
  int batch = 1;
  cufftType type = CUFFT_Z2Z;
  cufftResult status;
	
  status = cufftPlan1d(&cuplan, nx, type, batch);
  if( status != CUFFT_SUCCESS)
    {
      printf("error: cufftPlan1d failed.");
      exit(1);
    }
	

  SAMPLETYPE sampleBuffer[BUFF_SIZE];
  // char buffer[BUFF_SIZE]; // 8bit .wav output buffer
  short buffer[BUFF_SIZE]; // 16bit .wav output buffer
  //float buffer[BUFF_SIZE]; // 32bit IEEE FP16 .wav output buffer
  static float power[MAX_FREQ];

  WavInFile inFile(wavfile);
  printf("SampleRate: %d Hz\n",inFile.getSampleRate());
  printf("Number of bits per sample: %d\n",inFile.getNumBits());
  printf("Sample data size in bytes: %d\n",inFile.getDataSizeInBytes());
  printf("Total number of samples in file: %d\n",inFile.getNumSamples());
  printf("Number of bytes per audio sample: %d\n",inFile.getBytesPerSample());
  printf("Number of audio channels in the file (1=mono, 2=stereo): %d\n",inFile.getNumChannels());
  printf("Audio file length in milliseconds: %d\n",inFile.getLengthMS());

  WavOutFile outFile(wavfileout,inFile.getSampleRate(),inFile.getNumBits(),inFile.getNumChannels());
  if((log = fopen(logfile,"w")) == NULL)
    {
      fprintf(stderr,"can't open %s for writing\n", logfile);
      exit(1);
    }
  while (inFile.eof() == 0) {
    size_t samplesRead = inFile.read(sampleBuffer, BUFF_SIZE);
    for (int i = 0; i < BUFF_SIZE; i++) {
      in[i][0] = (double) sampleBuffer[i]; // real
      in[i][1] = 0.f; // imag

      out[i][0] = 0.f; // real
      out[i][1] = 0.f; // imag
    }
		
    CUDA_CHECK(cudaMemcpy(d_DoubleComplexData, in, nx * sizeof(cufftDoubleComplex),
			  cudaMemcpyHostToDevice));

    fftw_execute(plan);
    status = cufftExecZ2Z(cuplan, d_DoubleComplexData, d_DoubleComplexData,
			  CUFFT_FORWARD);

    switch(status)
      {
      case CUFFT_INVALID_PLAN:
 	fprintf(stderr,"The plan parameter is not a valid handle.\n");
	exit(1);
      case CUFFT_INVALID_VALUE:
	fprintf(stderr,"At least one of the parameters idata, odata, and direction is not valid.\n");
	exit(1);
      case CUFFT_INTERNAL_ERROR:
	fprintf(stderr,"An internal driver error was detected.\n");
	exit(1);
      case CUFFT_EXEC_FAILED:
	fprintf(stderr,"cuFFT failed to execute the transform on the GPU.\n");
	exit(1);
      case CUFFT_SETUP_FAILED:
	fprintf(stderr,"The cuFFT library failed to initialize.\n");
	exit(1);
      case CUFFT_SUCCESS:
      default:
	break;
      }

    CUDA_CHECK(cudaDeviceSynchronize());

    CUDA_CHECK(cudaMemcpy(h_DoubleComplexData, d_DoubleComplexData,
			  nx * sizeof(cufftDoubleComplex),
			  cudaMemcpyDeviceToHost));
		
    for (size_t i = 0; i < samplesRead; i++) {
      int re, im;
      float freq, magnitude;
      int index;
      re = out[i][0];
      im = out[i][1];
      magnitude = sqrt(re * re + im * im);
      freq = (i + 1) * inFile.getSampleRate() / samplesRead;  // (ith sample * samples/s * 1/number samples)
      index = freq / 1000; // quantized, kHz
   
      if (index <= MAX_FREQ) {
		power[index] += magnitude;
      }
    } // for

    fftw_execute(plan2); // freq. to time transform

    for (int i = 0; i < BUFF_SIZE; i++) {
        buffer[i] = out2[i][0]; // Re part
    }
    outFile.write(buffer, BUFF_SIZE);
  } // while

  for (int i = 0; i < MAX_FREQ; i++)
    printf("%2d kHz, power: %9.2f\n",i,power[i]);

  fftw_destroy_plan(plan);
  fftw_destroy_plan(plan2);
  fftw_free(in);
  fftw_free(out);
  fftw_free(out2);

  cufftDestroy(cuplan);
  cudaFree(d_DoubleComplexData);
 
  fclose(log);
  return 0;
}
