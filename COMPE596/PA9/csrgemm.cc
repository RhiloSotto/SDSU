#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include <hip/hip_runtime.h>
#include <rocblas/rocblas.h>
#include <rocsparse/rocsparse.h>

#define CHECK_HIP(cmd)                                                                  \
    do                                                                                  \
    {                                                                                   \
        hipError_t _err = (cmd);                                                        \
        if(_err != hipSuccess)                                                          \
        {                                                                               \
            std::cerr << "HIP error: " << hipGetErrorString(_err)                     \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl;         \
            std::exit(EXIT_FAILURE);                                                    \
        }                                                                               \
    } while(false)

#define CHECK_ROCBLAS(cmd)                                                              \
    do                                                                                  \
    {                                                                                   \
        rocblas_status _err = (cmd);                                                    \
        if(_err != rocblas_status_success)                                              \
        {                                                                               \
            std::cerr << "rocBLAS error: " << static_cast<int>(_err)                  \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl;         \
            std::exit(EXIT_FAILURE);                                                    \
        }                                                                               \
    } while(false)

#define CHECK_ROCSPARSE(cmd)                                                            \
    do                                                                                  \
    {                                                                                   \
        rocsparse_status _err = (cmd);                                                  \
        if(_err != rocsparse_status_success)                                            \
        {                                                                               \
            std::cerr << "rocSPARSE error: " << static_cast<int>(_err)                \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl;         \
            std::exit(EXIT_FAILURE);                                                    \
        }                                                                               \
    } while(false)

namespace
{
    const int N = 1 << 13; // 8192
    const float ALPHA = 1.0f;
    const float BETA  = 0.0f;

    struct CsrMatrix
    {
        int rows = 0;
        int cols = 0;
        std::vector<int> row_ptr;
        std::vector<int> col_ind;
        std::vector<float> val;
    };

    struct DeviceCsrMatrix
    {
        int rows   = 0;
        int cols   = 0;
        int nnz    = 0;
        int* row_ptr = nullptr;
        int* col_ind = nullptr;
        float* val   = nullptr;
    };

    struct SampleErrorSummary
    {
        double max_abs = 0.0;
        double max_rel = 0.0;
        bool passed = true;
    };

    void free_device_csr(DeviceCsrMatrix& M)
    {
        if(M.row_ptr) CHECK_HIP(hipFree(M.row_ptr));
        if(M.col_ind) CHECK_HIP(hipFree(M.col_ind));
        if(M.val)     CHECK_HIP(hipFree(M.val));
        M.row_ptr = nullptr;
        M.col_ind = nullptr;
        M.val     = nullptr;
        M.nnz     = 0;
        M.rows    = 0;
        M.cols    = 0;
    }

    CsrMatrix generate_sparse_matrix_csr(int rows, int cols, double density, std::uint32_t seed)
    {
        CsrMatrix A;
        A.rows = rows;
        A.cols = cols;
        A.row_ptr.resize(rows + 1, 0);

        std::mt19937 rng(seed);
        std::uniform_real_distribution<float> prob(0.0f, 1.0f);
        std::uniform_real_distribution<float> val_dist(0.1f, 1.0f);

        for(int i = 0; i < rows; ++i)
        {
            A.row_ptr[i] = static_cast<int>(A.col_ind.size());
            for(int j = 0; j < cols; ++j)
            {
                if(prob(rng) < density)
                {
                    A.col_ind.push_back(j);
                    A.val.push_back(val_dist(rng));
                }
            }
        }
        A.row_ptr[rows] = static_cast<int>(A.col_ind.size());
        return A;
    }

    std::vector<float> csr_to_dense_col_major(const CsrMatrix& A)
    {
        std::vector<float> dense(static_cast<std::size_t>(A.rows) * A.cols, 0.0f);
        for(int i = 0; i < A.rows; ++i)
        {
            for(int p = A.row_ptr[i]; p < A.row_ptr[i + 1]; ++p)
            {
                const int j = A.col_ind[p];
                dense[static_cast<std::size_t>(i) + static_cast<std::size_t>(j) * A.rows] = A.val[p];
            }
        }
        return dense;
    }

    DeviceCsrMatrix copy_csr_to_device(const CsrMatrix& A)
    {
        DeviceCsrMatrix dA;
        dA.rows = A.rows;
        dA.cols = A.cols;
        dA.nnz  = static_cast<int>(A.val.size());

        CHECK_HIP(hipMalloc(reinterpret_cast<void**>(&dA.row_ptr), sizeof(int) * A.row_ptr.size()));
        CHECK_HIP(hipMemcpy(dA.row_ptr,
                            A.row_ptr.data(),
                            sizeof(int) * A.row_ptr.size(),
                            hipMemcpyHostToDevice));

        const std::size_t nz_alloc = std::max<std::size_t>(1, A.val.size());
        CHECK_HIP(hipMalloc(reinterpret_cast<void**>(&dA.col_ind), sizeof(int) * nz_alloc));
        CHECK_HIP(hipMalloc(reinterpret_cast<void**>(&dA.val), sizeof(float) * nz_alloc));

        if(!A.val.empty())
        {
            CHECK_HIP(hipMemcpy(dA.col_ind,
                                A.col_ind.data(),
                                sizeof(int) * A.col_ind.size(),
                                hipMemcpyHostToDevice));
            CHECK_HIP(hipMemcpy(dA.val,
                                A.val.data(),
                                sizeof(float) * A.val.size(),
                                hipMemcpyHostToDevice));
        }

        return dA;
    }

    float host_csr_value(const CsrMatrix& A, int row, int col)
    {
        const int begin = A.row_ptr[row];
        const int end   = A.row_ptr[row + 1];
        const int* first = A.col_ind.data() + begin;
        const int* last  = A.col_ind.data() + end;
        const int* it    = std::lower_bound(first, last, col);
        if(it == last || *it != col)
        {
            return 0.0f;
        }
        return A.val[static_cast<std::size_t>(it - A.col_ind.data())];
    }

    float sample_reference_entry_from_Acsr_Bdense(const CsrMatrix& A,
                                                  const std::vector<float>& B_dense_col_major,
                                                  int row,
                                                  int col)
    {
        double sum = 0.0;
        for(int p = A.row_ptr[row]; p < A.row_ptr[row + 1]; ++p)
        {
            const int k = A.col_ind[p];
            sum += static_cast<double>(A.val[p]) *
                   static_cast<double>(B_dense_col_major[static_cast<std::size_t>(k) + static_cast<std::size_t>(col) * A.cols]);
        }
        return static_cast<float>(sum);
    }

    SampleErrorSummary validate_dense_samples(const std::vector<float>& C_dense_col_major,
                                              const CsrMatrix& A,
                                              const std::vector<float>& B_dense_col_major,
                                              int num_samples,
                                              std::uint32_t seed,
                                              double rel_tol,
                                              double abs_tol)
    {
        SampleErrorSummary summary;
        std::mt19937 rng(seed);
        std::uniform_int_distribution<int> row_dist(0, A.rows - 1);
        std::uniform_int_distribution<int> col_dist(0, A.cols - 1);

        for(int s = 0; s < num_samples; ++s)
        {
            const int row = row_dist(rng);
            const int col = col_dist(rng);
            const float ref = sample_reference_entry_from_Acsr_Bdense(A, B_dense_col_major, row, col);
            const float got = C_dense_col_major[static_cast<std::size_t>(row) + static_cast<std::size_t>(col) * A.rows];
            const double abs_err = std::fabs(static_cast<double>(got) - static_cast<double>(ref));
            const double rel_err = abs_err / std::max(1.0, std::fabs(static_cast<double>(ref)));

            summary.max_abs = std::max(summary.max_abs, abs_err);
            summary.max_rel = std::max(summary.max_rel, rel_err);
            if(!(abs_err <= abs_tol || rel_err <= rel_tol))
            {
                summary.passed = false;
            }
        }

        return summary;
    }

    SampleErrorSummary validate_csr_samples(const CsrMatrix& C,
                                            const CsrMatrix& A,
                                            const std::vector<float>& B_dense_col_major,
                                            int num_samples,
                                            std::uint32_t seed,
                                            double rel_tol,
                                            double abs_tol)
    {
        SampleErrorSummary summary;
        std::mt19937 rng(seed);
        std::uniform_int_distribution<int> row_dist(0, A.rows - 1);
        std::uniform_int_distribution<int> col_dist(0, A.cols - 1);

        for(int s = 0; s < num_samples; ++s)
        {
            const int row = row_dist(rng);
            const int col = col_dist(rng);
            const float ref = sample_reference_entry_from_Acsr_Bdense(A, B_dense_col_major, row, col);
            const float got = host_csr_value(C, row, col);
            const double abs_err = std::fabs(static_cast<double>(got) - static_cast<double>(ref));
            const double rel_err = abs_err / std::max(1.0, std::fabs(static_cast<double>(ref)));

            summary.max_abs = std::max(summary.max_abs, abs_err);
            summary.max_rel = std::max(summary.max_rel, rel_err);
            if(!(abs_err <= abs_tol || rel_err <= rel_tol))
            {
                summary.passed = false;
            }
        }

        return summary;
    }

    double sampled_sparse_vs_dense_diff(const CsrMatrix& C_sparse,
                                        const std::vector<float>& C_dense_col_major,
                                        int rows,
                                        int cols,
                                        int num_samples,
                                        std::uint32_t seed)
    {
        std::mt19937 rng(seed);
        std::uniform_int_distribution<int> row_dist(0, rows - 1);
        std::uniform_int_distribution<int> col_dist(0, cols - 1);
        double max_abs = 0.0;

        for(int s = 0; s < num_samples; ++s)
        {
            const int row = row_dist(rng);
            const int col = col_dist(rng);
            const double a = static_cast<double>(host_csr_value(C_sparse, row, col));
            const double b = static_cast<double>(C_dense_col_major[static_cast<std::size_t>(row) + static_cast<std::size_t>(col) * rows]);
            max_abs = std::max(max_abs, std::fabs(a - b));
        }

        return max_abs;
    }
}

int main()
{
    CHECK_HIP(hipSetDevice(0));

    rocblas_handle handle_blas;
    rocsparse_handle handle_sparse;
    CHECK_ROCBLAS(rocblas_create_handle(&handle_blas));
    CHECK_ROCSPARSE(rocsparse_create_handle(&handle_sparse));

    const double densities[] = {1e-5, 1e-4, 1e-3, 1e-2, 1e-1, 1.0};

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "density,nnzA,nnzB,nnzC,rocSPARSE,rocBLAS\n";
    
    for(int idx = 0; idx < static_cast<int>(sizeof(densities) / sizeof(densities[0])); ++idx)
    {
        const double density = densities[idx];

        // Generate A and B in CSR. Distinct seeds keep runs reproducible.
        CsrMatrix A = generate_sparse_matrix_csr(N, N, density, 12345u + static_cast<std::uint32_t>(idx) * 17u);
        CsrMatrix B = generate_sparse_matrix_csr(N, N, density, 54321u + static_cast<std::uint32_t>(idx) * 31u);

        // Dense column-major copies for rocBLAS and sampled correctness checks.
        std::vector<float> A_dense = csr_to_dense_col_major(A);
        std::vector<float> B_dense = csr_to_dense_col_major(B);

        DeviceCsrMatrix dA = copy_csr_to_device(A);
        DeviceCsrMatrix dB = copy_csr_to_device(B);

        // Dummy D for beta = 0. rocSPARSE still expects valid descriptors/pointers when beta is a valid pointer.
        std::vector<int> hD_row_ptr(N + 1, 0);
        int* dD_row_ptr = nullptr;
        int* dD_col_ind = nullptr;
        float* dD_val = nullptr;
        CHECK_HIP(hipMalloc(reinterpret_cast<void**>(&dD_row_ptr), sizeof(int) * hD_row_ptr.size()));
        CHECK_HIP(hipMemcpy(dD_row_ptr,
                            hD_row_ptr.data(),
                            sizeof(int) * hD_row_ptr.size(),
                            hipMemcpyHostToDevice));
        CHECK_HIP(hipMalloc(reinterpret_cast<void**>(&dD_col_ind), sizeof(int)));
        CHECK_HIP(hipMalloc(reinterpret_cast<void**>(&dD_val), sizeof(float)));
        CHECK_HIP(hipMemset(dD_col_ind, 0, sizeof(int)));
        CHECK_HIP(hipMemset(dD_val, 0, sizeof(float)));

        rocsparse_mat_descr descrA, descrB, descrC, descrD;
        CHECK_ROCSPARSE(rocsparse_create_mat_descr(&descrA));
        CHECK_ROCSPARSE(rocsparse_create_mat_descr(&descrB));
        CHECK_ROCSPARSE(rocsparse_create_mat_descr(&descrC));
        CHECK_ROCSPARSE(rocsparse_create_mat_descr(&descrD));
        CHECK_ROCSPARSE(rocsparse_set_mat_index_base(descrA, rocsparse_index_base_zero));
        CHECK_ROCSPARSE(rocsparse_set_mat_index_base(descrB, rocsparse_index_base_zero));
        CHECK_ROCSPARSE(rocsparse_set_mat_index_base(descrC, rocsparse_index_base_zero));
        CHECK_ROCSPARSE(rocsparse_set_mat_index_base(descrD, rocsparse_index_base_zero));
        CHECK_ROCSPARSE(rocsparse_set_mat_type(descrA, rocsparse_matrix_type_general));
        CHECK_ROCSPARSE(rocsparse_set_mat_type(descrB, rocsparse_matrix_type_general));
        CHECK_ROCSPARSE(rocsparse_set_mat_type(descrC, rocsparse_matrix_type_general));
        CHECK_ROCSPARSE(rocsparse_set_mat_type(descrD, rocsparse_matrix_type_general));

        rocsparse_mat_info infoC;
        CHECK_ROCSPARSE(rocsparse_create_mat_info(&infoC));

        size_t buffer_size = 0;
        CHECK_ROCSPARSE(rocsparse_scsrgemm_buffer_size(handle_sparse,
                                                       rocsparse_operation_none,
                                                       rocsparse_operation_none,
                                                       N,
                                                       N,
                                                       N,
                                                       &ALPHA,
                                                       descrA,
                                                       dA.nnz,
                                                       dA.row_ptr,
                                                       dA.col_ind,
                                                       descrB,
                                                       dB.nnz,
                                                       dB.row_ptr,
                                                       dB.col_ind,
                                                       &BETA,
                                                       descrD,
                                                       0,
                                                       dD_row_ptr,
                                                       dD_col_ind,
                                                       infoC,
                                                       &buffer_size));

        void* temp_buffer = nullptr;
        CHECK_HIP(hipMalloc(&temp_buffer, std::max<std::size_t>(buffer_size, 1)));

        int* dC_row_ptr = nullptr;
        int* dC_col_ind = nullptr;
        float* dC_val = nullptr;
        CHECK_HIP(hipMalloc(reinterpret_cast<void**>(&dC_row_ptr), sizeof(int) * (N + 1)));

        // Time the sparse multiplication path. This includes the required nnz discovery step
        // plus rocsparse_scsrgemm itself, but excludes host-device transfers and matrix generation.
        int nnzC = 0;
        auto sparse_start = std::chrono::high_resolution_clock::now();

        CHECK_ROCSPARSE(rocsparse_csrgemm_nnz(handle_sparse,
                                              rocsparse_operation_none,
                                              rocsparse_operation_none,
                                              N,
                                              N,
                                              N,
                                              descrA,
                                              dA.nnz,
                                              dA.row_ptr,
                                              dA.col_ind,
                                              descrB,
                                              dB.nnz,
                                              dB.row_ptr,
                                              dB.col_ind,
                                              descrD,
                                              0,
                                              dD_row_ptr,
                                              dD_col_ind,
                                              descrC,
                                              dC_row_ptr,
                                              &nnzC,
                                              infoC,
                                              temp_buffer));

        CHECK_HIP(hipMalloc(reinterpret_cast<void**>(&dC_col_ind), sizeof(int) * std::max(1, nnzC)));
        CHECK_HIP(hipMalloc(reinterpret_cast<void**>(&dC_val), sizeof(float) * std::max(1, nnzC)));

        CHECK_ROCSPARSE(rocsparse_scsrgemm(handle_sparse,
                                           rocsparse_operation_none,
                                           rocsparse_operation_none,
                                           N,
                                           N,
                                           N,
                                           &ALPHA,
                                           descrA,
                                           dA.nnz,
                                           dA.val,
                                           dA.row_ptr,
                                           dA.col_ind,
                                           descrB,
                                           dB.nnz,
                                           dB.val,
                                           dB.row_ptr,
                                           dB.col_ind,
                                           &BETA,
                                           descrD,
                                           0,
                                           dD_val,
                                           dD_row_ptr,
                                           dD_col_ind,
                                           descrC,
                                           dC_val,
                                           dC_row_ptr,
                                           dC_col_ind,
                                           infoC,
                                           temp_buffer));

        CHECK_HIP(hipDeviceSynchronize());
        auto sparse_end = std::chrono::high_resolution_clock::now();
        const double sparse_time = std::chrono::duration<double>(sparse_end - sparse_start).count();

        // Bring sparse result back to host for verification.
        CsrMatrix C_sparse;
        C_sparse.rows = N;
        C_sparse.cols = N;
        C_sparse.row_ptr.resize(N + 1);
        C_sparse.col_ind.resize(std::max(0, nnzC));
        C_sparse.val.resize(std::max(0, nnzC));
        CHECK_HIP(hipMemcpy(C_sparse.row_ptr.data(), dC_row_ptr, sizeof(int) * (N + 1), hipMemcpyDeviceToHost));
        if(nnzC > 0)
        {
            CHECK_HIP(hipMemcpy(C_sparse.col_ind.data(), dC_col_ind, sizeof(int) * nnzC, hipMemcpyDeviceToHost));
            CHECK_HIP(hipMemcpy(C_sparse.val.data(), dC_val, sizeof(float) * nnzC, hipMemcpyDeviceToHost));
        }

        // rocBLAS path with dense matrices in column-major layout.
        float* dA_dense = nullptr;
        float* dB_dense = nullptr;
        float* dC_dense = nullptr;
        CHECK_HIP(hipMalloc(reinterpret_cast<void**>(&dA_dense), sizeof(float) * static_cast<std::size_t>(N) * N));
        CHECK_HIP(hipMalloc(reinterpret_cast<void**>(&dB_dense), sizeof(float) * static_cast<std::size_t>(N) * N));
        CHECK_HIP(hipMalloc(reinterpret_cast<void**>(&dC_dense), sizeof(float) * static_cast<std::size_t>(N) * N));
        CHECK_HIP(hipMemcpy(dA_dense,
                            A_dense.data(),
                            sizeof(float) * static_cast<std::size_t>(N) * N,
                            hipMemcpyHostToDevice));
        CHECK_HIP(hipMemcpy(dB_dense,
                            B_dense.data(),
                            sizeof(float) * static_cast<std::size_t>(N) * N,
                            hipMemcpyHostToDevice));
        CHECK_HIP(hipMemset(dC_dense, 0, sizeof(float) * static_cast<std::size_t>(N) * N));

        auto dense_start = std::chrono::high_resolution_clock::now();
        CHECK_ROCBLAS(rocblas_sgemm(handle_blas,
                                    rocblas_operation_none,
                                    rocblas_operation_none,
                                    N,
                                    N,
                                    N,
                                    &ALPHA,
                                    dA_dense,
                                    N,
                                    dB_dense,
                                    N,
                                    &BETA,
                                    dC_dense,
                                    N));
        CHECK_HIP(hipDeviceSynchronize());
        auto dense_end = std::chrono::high_resolution_clock::now();
        const double dense_time = std::chrono::duration<double>(dense_end - dense_start).count();

        std::vector<float> C_dense(static_cast<std::size_t>(N) * N);
        CHECK_HIP(hipMemcpy(C_dense.data(),
                            dC_dense,
                            sizeof(float) * static_cast<std::size_t>(N) * N,
                            hipMemcpyDeviceToHost));

        const double rel_tol = 1e-3;
        const double abs_tol = 5e-2;
        const int num_check_samples = 32;

        SampleErrorSummary sparse_check = validate_csr_samples(C_sparse,
                                                               A,
                                                               B_dense,
                                                               num_check_samples,
                                                               9001u + static_cast<std::uint32_t>(idx),
                                                               rel_tol,
                                                               abs_tol);

        SampleErrorSummary dense_check = validate_dense_samples(C_dense,
                                                                A,
                                                                B_dense,
                                                                num_check_samples,
                                                                1337u + static_cast<std::uint32_t>(idx),
                                                                rel_tol,
                                                                abs_tol);

        const double sparse_dense_sample_abs_diff = sampled_sparse_vs_dense_diff(C_sparse,
                                                                                  C_dense,
                                                                                  N,
                                                                                  N,
                                                                                  num_check_samples,
                                                                                  777u + static_cast<std::uint32_t>(idx));

        // Print results
        std::cout << density << ","
          << dA.nnz << ","
          << dB.nnz << ","
          << nnzC << ","
          << sparse_time << ","
          << dense_time << "\n";

        // Cleanup this iteration.
        CHECK_HIP(hipFree(dA_dense));
        CHECK_HIP(hipFree(dB_dense));
        CHECK_HIP(hipFree(dC_dense));

        CHECK_HIP(hipFree(dC_row_ptr));
        CHECK_HIP(hipFree(dC_col_ind));
        CHECK_HIP(hipFree(dC_val));
        CHECK_HIP(hipFree(temp_buffer));

        CHECK_HIP(hipFree(dD_row_ptr));
        CHECK_HIP(hipFree(dD_col_ind));
        CHECK_HIP(hipFree(dD_val));

        free_device_csr(dA);
        free_device_csr(dB);

        CHECK_ROCSPARSE(rocsparse_destroy_mat_info(infoC));
        CHECK_ROCSPARSE(rocsparse_destroy_mat_descr(descrA));
        CHECK_ROCSPARSE(rocsparse_destroy_mat_descr(descrB));
        CHECK_ROCSPARSE(rocsparse_destroy_mat_descr(descrC));
        CHECK_ROCSPARSE(rocsparse_destroy_mat_descr(descrD));
    }

    CHECK_ROCSPARSE(rocsparse_destroy_handle(handle_sparse));
    CHECK_ROCBLAS(rocblas_destroy_handle(handle_blas));

    return 0;
}
