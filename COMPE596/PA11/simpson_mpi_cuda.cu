#include <mpi.h>
#include <cuda_runtime.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define CUDA_CHECK(call) do {                                      \
    cudaError_t err = call;                                        \
    if (err != cudaSuccess) {                                      \
        fprintf(stderr, "CUDA error %s:%d: %s\n",                  \
                __FILE__, __LINE__, cudaGetErrorString(err));      \
        MPI_Abort(MPI_COMM_WORLD, 1);                              \
    }                                                             \
} while (0)

__device__ double f_gpu(double x) {
    double c = cos(x);
    return acos(c / (1.0 + 2.0 * c));
}

__global__ void simpson_kernel(
    unsigned long long j_start,
    unsigned long long j_end,
    double a,
    double h,
    double *block_sums
) {
    extern __shared__ double shmem[];

    unsigned int tid = threadIdx.x;
    unsigned long long global_tid = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned long long stride = gridDim.x * blockDim.x;

    double sum = 0.0;

    for (unsigned long long j = j_start + global_tid; j <= j_end; j += stride) {
        sum += f_gpu(a + (2 * j - 2) * h)
             + 4.0 * f_gpu(a + (2 * j - 1) * h)
             + f_gpu(a + (2 * j) * h);
    }

    shmem[tid] = sum;
    __syncthreads();

    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) {
            shmem[tid] += shmem[tid + s];
        }
        __syncthreads();
    }

    if (tid == 0) {
        block_sums[blockIdx.x] = shmem[0];
    }
}

unsigned long long read_ull_arg(int argc, char **argv, const char *flag, unsigned long long def) {
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], flag) == 0) {
            return strtoull(argv[i + 1], NULL, 10);
        }
    }
    return def;
}

int read_int_arg(int argc, char **argv, const char *flag, int def) {
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], flag) == 0) {
            return atoi(argv[i + 1]);
        }
    }
    return def;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, M;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &M);

    unsigned long long N = read_ull_arg(argc, argv, "-n", 1000000ULL);
    int blockDim = read_int_arg(argc, argv, "-t", 256);

    if (N % 2 != 0) {
        if (rank == 0) {
            fprintf(stderr, "Error: N must be even for composite Simpson's 1/3 rule.\n");
        }
        MPI_Finalize();
        return 1;
    }

    if ((blockDim & (blockDim - 1)) != 0) {
        if (rank == 0) {
            fprintf(stderr, "Error: CUDA thread count must be a power of 2.\n");
        }
        MPI_Finalize();
        return 1;
    }

    int device_count = 0;
    CUDA_CHECK(cudaGetDeviceCount(&device_count));
    CUDA_CHECK(cudaSetDevice(rank % device_count));

    double a = 0.0;
    double b = M_PI / 2.0;
    double exact = 5.0 * M_PI * M_PI / 24.0;
    double h = (b - a) / (double)N;

    unsigned long long num_pairs = N / 2;

    unsigned long long j_start = rank * num_pairs / M + 1;
    unsigned long long j_end   = (rank + 1) * num_pairs / M;

    unsigned long long local_pairs = 0;
    if (j_end >= j_start) {
        local_pairs = j_end - j_start + 1;
    }

    int gridDim = 1024;
    if (local_pairs < (unsigned long long)gridDim * blockDim) {
        gridDim = (int)((local_pairs + blockDim - 1) / blockDim);
        if (gridDim < 1) gridDim = 1;
    }

    double *d_block_sums = NULL;
    double *h_block_sums = (double *)calloc(gridDim, sizeof(double));

    CUDA_CHECK(cudaMalloc((void **)&d_block_sums, gridDim * sizeof(double)));

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    simpson_kernel<<<gridDim, blockDim, blockDim * sizeof(double)>>>(
        j_start,
        j_end,
        a,
        h,
        d_block_sums
    );

    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    CUDA_CHECK(cudaMemcpy(
        h_block_sums,
        d_block_sums,
        gridDim * sizeof(double),
        cudaMemcpyDeviceToHost
    ));

    double local_sum = 0.0;
    for (int i = 0; i < gridDim; i++) {
        local_sum += h_block_sums[i];
    }

    double local_approx = local_sum * h / 3.0;
    double approx = 0.0;

    MPI_Reduce(
        &local_approx,
        &approx,
        1,
        MPI_DOUBLE,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );

    double t1 = MPI_Wtime();
    double local_time = t1 - t0;
    double wall_time = 0.0;

    MPI_Reduce(
        &local_time,
        &wall_time,
        1,
        MPI_DOUBLE,
        MPI_MAX,
        0,
        MPI_COMM_WORLD
    );

    if (rank == 0) {
        double error = fabs(exact - approx);
        printf("M=%d threads=%d N=%llu approx=%.15f exact=%.15f error=%.15e time=%.8f\n",
               M, blockDim, N, approx, exact, error, wall_time);
    }

    CUDA_CHECK(cudaFree(d_block_sums));
    free(h_block_sums);

    MPI_Finalize();
    return 0;
}