/*
** Assignment: Laboratory Assignment 15 "OpenMP"
**
** Filename: PL15.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: //2023, 9PM
**
** Description: Numerically calculates an approximation for the integral of a given function
** using the rectangle method serial and parallel and with 8 * powers of 10 divisions
**
** Input: A given interval from start value to end value, and an exact value to compare
** approximation against
**
** Output: the approximation for rectangle and trapezoid method given a number of divisions
** followed by the error between approximation and given exact value
**
** Algorithm: checks to see if console command is given in correct format, returns error if
** not the case; iteratively gives approximations calculated by rectangle method for finding the value
**
** Caveats: Runtime increases substantially for larger number of divisions (i.e N = 10^9)
**
** Version: 1.0
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <omp.h>
#include <time.h>


double f(double x) {
	return (1/x); // replace parenthesis with any desired function to integrate
}

double rectangle_method(double a, double b, int N) {
	double interval = (b - a) / N;
	double midpoint = a + (interval / 2);
	double sum = 0.0;

	for (size_t i = 0; i < N; ++i) {
		sum += (f(midpoint) * interval);
		midpoint = a + interval*(i+0.5);
	}

	return sum;
}

double rectangle_method_omp(double a, double b, int n)
{
        double sum = 0.0;
        int numThreads = 1;

#pragma omp parallel
        {
                numThreads = omp_get_num_threads();
        }

        double* s = (double*)calloc(numThreads, sizeof(double));

#pragma omp parallel
        {
                int id = omp_get_thread_num();
                int istart = id * n / numThreads;
                int iend = (id + 1) * n / numThreads;

                double interval = (b - a) / n;
                double midpoint = a + (istart / n) + (interval / 2);
                double area = 0.0;

                for (size_t i = istart; i<iend; i++)
                {
                    midpoint = a + interval * i;
                	area = f(midpoint) * interval;

                    s[id] += area;
                }
        }

        for (size_t i = 0; i < numThreads; ++i) {
        	sum += s[i];
        }

        return sum;
}

/*
double trapezoid_method(double a, double b, int N) {
	double interval = (b - a) / N;
	double sum = 0.0;

	for (size_t i = 0; i <= N; ++i) {
		sum += ((f(a + i*interval) + f(a + (i + 1)*interval) )/2) * interval;
	}

	return sum;
}
*/

int main(int argc, char *argv[]) {

	int numThreads = 1;
#pragma omp parallel
	{
		numThreads = omp_get_num_threads();
	}

	omp_set_dynamic(0); // disables dynamic adjustment of threads
	omp_set_num_threads(numThreads);


	double a = 0.0;
	double b = 0.0;
	double exact = 0.0;
	// the following checks if parameters are correct for program
	if(argc != 4) {
		printf("usage: integration a b exact\n");
		exit(0);
	}
	else {
		char *endp;
		a = strtod(argv[1], &endp);
		if (argv[1] == endp || *endp != '\0') {
			printf("error: a parameter incorrect\n");
			exit(0);
		}
		b = strtod(argv[2], &endp);
		if (argv[2] == endp || *endp != '\0') {
			printf("error: b parameter incorrect\n");
			exit(0);
		}
		exact = strtod(argv[3], &endp);

		if (argv[3] == endp || *endp != '\0') {
			printf("error: exact parameter incorrect\n");
			exit(0);
		}
	}


	double sum = 0.0;
	clock_t begin, end;

	// calls rectangle integration, prints result and error for powers of 10
	for (int n = 1e1 * numThreads; n <= 1e7 * numThreads; n *= 10) {
		begin = clock();
		sum = rectangle_method(a,b,n);
		end = clock();
		printf("rect: n = %11d, area = %.13lf, err = %.13e\n", n, sum, fabs(exact - sum));
		printf("Elapsed time = %.2e s (serial)\n", (((double)(end - begin))/CLOCKS_PER_SEC));

		begin = clock();
		sum = rectangle_method_omp(a,b,n);
		end = clock();
		printf("rect: n = %11d, area = %.13lf, err = %.13e\n", n, sum, fabs(exact - sum));
		printf("Elapsed time = %.2e s (parallel)\n\n", (((double)(end - begin))/CLOCKS_PER_SEC));
	}

	return 0;
}
