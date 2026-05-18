#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// function
double f(double x) {
    double c = cos(x);
    return acos(c / (1.0 + 2.0 * c));
}

int main(int argc, char* argv[]) {
    // exact answer to compare approximation to
    double exact = 5 * (M_PI * M_PI) / 24;
    // read arguments
    int Nthrds = atoi(argv[1]);
    // integral bounds
    double a = 0.0, b = M_PI / 2;
    double n = (double)Nthrds;
    const double h = (b - a) / n;
    
    // omp settings
    omp_set_dynamic(0);
    omp_set_num_threads(Nthrds);

    // store partial sums
    double* s = (double*)calloc(Nthrds, sizeof(double));

    #pragma omp parallel {
        unsigned int istart, iend;
        int id, N = (int)n / 2;
        id = omp_get_thread_num();
        istart = id * N / Nthrds;
        iend = (id+1) * N / Nthrds;
        if (id == Nthrds - 1) iend = N; // last thread ends at N
        for (unsigned long long j = istart+1; j <= iend; ++j) {
        //xj = a + j * h;
            s[id] += f(a + (2* j-2) * h) + 4 * f(a + (2*j-1)*h) + f(a +(2*j) *h);
        }
        s[id] *= h/3;
    }
    
    double approx = 0.0;
    #pragma omp parallel for reduction(+:approx)
    for (double i = 0; i < Nthrds; ++i) {
        // sum all s for final answer
        approx += s[i];
    }
    // error calculation
    printf("%d %lf %lf %lf\n", Nthrds, approx, exact, fabs(exact - approx));

    return 0;
}