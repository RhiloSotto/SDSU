#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TILE_WIDTH 16
#define MASK_WIDTH 5
#define MASK_RADIUS 2

// CUDA error checking macro
#define CUDA_CHECK(err) \
    if (err != cudaSuccess) { \
        printf("CUDA error: %s\n", cudaGetErrorString(err)); \
        exit(-1); \
    }

__global__ void convolution(unsigned int *input,
                            int *mask,
                            int *output,
                            int channels,
                            int width,
                            int height)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    // Only compute if inside image
    if (row >= height || col >= width) return;

    // Loop over channels (RGB)
    for (int ch = 0; ch < channels; ch++)
    {
        int sum = 0;

        // Apply 5x5 filter
        for (int i = -MASK_RADIUS; i <= MASK_RADIUS; i++)
        {
            for (int j = -MASK_RADIUS; j <= MASK_RADIUS; j++)
            {
                int r = row + i;
                int c = col + j;

                // Zero padding at borders
                if (r >= 0 && r < height && c >= 0 && c < width)
                {
                    int imageIndex = (r * width + c) * channels + ch;
                    int maskIndex = (i + MASK_RADIUS) * MASK_WIDTH + (j + MASK_RADIUS);

                    sum += input[imageIndex] * mask[maskIndex];
                }
            }
        }

        // Write output
        int outputIndex = (row * width + col) * channels + ch;
        output[outputIndex] = sum;
    }
}

int main(int argc, char *argv[])
{
    unsigned int *hostInputImage;
    int *hostOutputImage;

    printf("Reading image data...\n");

    // Open file
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Error opening input file\n");
        return -1;
    }

    // Read dimensions from dat file
    int imageHeight, imageWidth, imageChannels;
    fscanf(f, "%d %d %d", &imageHeight, &imageWidth, &imageChannels);

    printf("Image size: %d x %d x %d\n", imageHeight, imageWidth, imageChannels);

    // Compute total size
    unsigned int inputLength = imageWidth * imageHeight * imageChannels;

    // Allocate host memory
    hostInputImage = (unsigned int *)malloc(inputLength * sizeof(unsigned int));
    hostOutputImage = (int *)malloc(inputLength * sizeof(int));

    // Read pixel data
    unsigned int pixelValue;
    for (unsigned int i = 0; i < inputLength; i++) {
        fscanf(f, "%d", &pixelValue);
        hostInputImage[i] = pixelValue;
    }
    fclose(f);

    // Sobel 5x5 horizontal mask
    int hostMask[5][5] = {
        { 2,  2,  4,  2,  2},
        { 1,  1,  2,  1,  1},
        { 0,  0,  0,  0,  0},
        {-1, -1, -2, -1, -1},
        {-2, -2, -4, -2, -2}
    };

    unsigned int *deviceInputImage;
    int *deviceOutputImage;
    int *deviceMask;

    // Allocate GPU memory
    CUDA_CHECK(cudaMalloc((void **)&deviceInputImage,
        inputLength * sizeof(unsigned int)));

    CUDA_CHECK(cudaMalloc((void **)&deviceOutputImage,
        inputLength * sizeof(int)));

    CUDA_CHECK(cudaMalloc((void **)&deviceMask,
        MASK_WIDTH * MASK_WIDTH * sizeof(int)));

    // Copy data to GPU
    CUDA_CHECK(cudaMemcpy(deviceInputImage,
        hostInputImage,
        inputLength * sizeof(unsigned int),
        cudaMemcpyHostToDevice));

    CUDA_CHECK(cudaMemcpy(deviceMask,
        hostMask,
        MASK_WIDTH * MASK_WIDTH * sizeof(int),
        cudaMemcpyHostToDevice));

    // Configure execution
    dim3 dimBlock(TILE_WIDTH, TILE_WIDTH, 1);
    dim3 dimGrid(
        (imageWidth + TILE_WIDTH - 1) / TILE_WIDTH,
        (imageHeight + TILE_WIDTH - 1) / TILE_WIDTH
    );

    // Launch kernel
    convolution<<<dimGrid, dimBlock>>>(
        deviceInputImage,
        deviceMask,
        deviceOutputImage,
        imageChannels,
        imageWidth,
        imageHeight
    );

    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    // Copy result back
    CUDA_CHECK(cudaMemcpy(hostOutputImage,
        deviceOutputImage,
        inputLength * sizeof(int),
        cudaMemcpyDeviceToHost));

    // Write output file
    f = fopen("output.out", "w");
    for (unsigned int i = 0; i < inputLength; i++)
        fprintf(f, "%d\n", hostOutputImage[i]);
    fclose(f);

    // Cleanup
    cudaFree(deviceInputImage);
    cudaFree(deviceOutputImage);
    cudaFree(deviceMask);

    free(hostInputImage);
    free(hostOutputImage);

    printf("Done.\n");

    return 0;
}