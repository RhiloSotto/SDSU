/*
** Assignment: Laboratory Assignment 06 "My Sqrt"
**
** Filename: MySqrt.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: 10/3/2023, 9PM
**
** Description: calculates an approximation for the square root of a given double after n iterations
**
** Input: a double for the number that will be "square rooted" and an unsigned integer for the number of iterations
**
** Output: The math.h definition for the squareroot of 2 to compare against, the c sqrt function for 20 fractional digits, and
** the mySqrt function's output for 20 fractional digits
**
** Algorithm: gets the input of both the number to be square rooted and the number of iterations, running
** the function mySqrt with those inputs and using the given formula for approximating the square root iteratively, outputting
** the c sqrt function, mySqrt function
**
** Caveats: Requires multiple iterations to get a more accurate representation of the square root of a given number
**
** Version: 1.0
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double mySqrt(double a, unsigned int n);

int main(void) {

	double userInput;
	unsigned int numIterations;
	scanf("%lf", &userInput);
	scanf("%u", &numIterations);

	printf("This is   M_SQRT2: %0.20lf\n", M_SQRT2);
	printf("This is   sqrt(2): %0.20lf\n", sqrt(userInput));
	printf("This is mySqrt(2): %0.20lf\n", mySqrt(userInput, numIterations));

	return 0;
}

double mySqrt(double a, unsigned int n) {
	double x = a / 2;

	for (size_t i = 0; i < n; ++i) {
		x = x - ((pow(x,2) - a) * (3 * pow(x,2) + a) * (3 * pow(x,6) + 27 * a * pow(x,4) + 33 * pow(a,2) * pow(x,2) + pow(a,3))) / ((2 * x) * (pow(x,4) + 10 * a *pow(x,2) + 5 * pow(a,2)) * (5 * pow(x,4) + 10 * a * pow(x,2) + pow(a,2)));
	}

	return x;
}
