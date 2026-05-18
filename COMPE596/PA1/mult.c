#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TOL 1.1102230246251568E-16 
#define ORDER 1000
#define AVAL 3.0
#define BVAL 5.0


int main(int argc, char *argv[])
{
    double start_time = omp_get_wtime();

	int Ndim, Pdim, Mdim;
	int i, j, k;
    double tmp;

	double *A, *B, *C;
	Ndim = ORDER;
	Pdim = ORDER;
	Mdim = ORDER;

	A = (double*)malloc(Ndim*Pdim*sizeof(double));
	B = (double*)malloc(Pdim*Mdim*sizeof(double));
	C = (double*)malloc(Ndim*Mdim*sizeof(double));



    /* initialize matrices */

    for (i = 0; i < Ndim; i++)
	    for (j = 0; j < Pdim; j++)
		    *(A+(i*Ndim+j)) = AVAL;

    for (i = 0; i < Pdim; i++)
	    for (j = 0; j < Mdim; j++)
    		*(B+(i*Ndim+j)) = BVAL;

    for (i = 0; i < Ndim; i++)
	    for (j = 0; j < Mdim; j++)
		    *(C+(i*Ndim+j)) = 0.0;

    for (i = 0; i < Ndim; i++) {
	    for (j = 0; j < Mdim; j++) {
    	tmp = 0.0;
            for (k = 0; k < Pdim; k++) {
        		tmp += *(A+(i*Ndim + k)) *  *(B+(k*Pdim+j));
            }
        }
        *(C+(i*Ndim+j)) = tmp;
    }



    double run_time = omp_get_wtime() - start_time;
    double dN = (double)ORDER;
    double mflops = 2.0 * dN * dN * dN/(1000000.0 * run_time);
    printf("%f mflops\n", mflops);


    double Cval = Pdim * AVAL * BVAL;
    double errsq = 0.0;
    for (i = 0; i < Ndim; i++) {
        for (j = 0; j < Mdim; j++) {
            double err = *(C+i*Ndim+j) - Cval;
            errsq += err * err;
        }
    }

    if (errsq >= TOL)
        printf("\n Errors in multiplications: %f", errsq);
    else 
        printf("\n No errors (errsq = %.16e).", errsq);

}