/*
** Assignment: Laboratory Assignment 12 "Numerical Integation"
**
** Filename: NumericalIntegration.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: 11/14/2023, 9PM
**
** Description: Numerically calculates an approximation for the integral of a given function
** using the rectangle method and trapezoid method with powers of 10 divisions
**
** Input: A given interval from start value to end value, and an exact value to compare
** approximation against
**
** Output: the approximation for rectangle and trapezoid method given a number of divisions
** followed by the error between approximation and given exact value
**
** Algorithm: checks to see if console command is given in correct format, returns error if
** not the case; iteratively gives approximations calculated by rectangle and trapezoid
** methods for finding the value
**
** Caveats: Runtime increases substantially for larger number of divisions (i.e N = 10^9)
**
** Version: 1.0
**
*/

#include <stdio.h>
#include <stdlib.h>

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

double rectangle_method_omp(double a, double b, int N) {
	double interval = (b - a) / N;
	double midpoint = a + (interval / 2);
	double sum = 0.0;

	for (size_t i = 0; i < N; ++i) {
		sum += (f(midpoint) * interval);
		midpoint = a + interval*(i+0.5);
	}

	return sum;
}



double trapezoid_method(double a, double b, int N) {
	double interval = (b - a) / N;
	double sum = 0.0;

	for (size_t i = 0; i <= N; ++i) {
		sum += ((f(a + i*interval) + f(a + (i + 1)*interval) )/2) * interval;
	}

	return sum;
}


int main(int argc, char *argv[]) {

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

	// calls rectangle and trapezoid integration, prints result and error for powers of 10
	for (int n = 1e1; n <= 1e9; n *= 10) {
		printf("rect: n = %11d, area = %0.13lf, err = %0.13e\n", n, rectangle_method(a, b, n), rectangle_method(a, b, n) - exact);
		printf("trap: n = %11d, area = %0.13lf. err = %0.13e\n", n, trapezoid_method(a, b, n), trapezoid_method(a, b, n) - exact);
	}

	return 0;
}
