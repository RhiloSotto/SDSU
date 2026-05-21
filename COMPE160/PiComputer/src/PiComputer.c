/*
** Assignment: Laboratory Assignment 04 "Pi Computer"
**
** Filename: PiComputer.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: 9/19/2023, 9PM
**
** Description: Three different algorithms for computing Pi or it's reciprocal 1/Pi
**
** Input: None
**
** Output: First algorithm's estimated Pi value after four iterations followed by M_PI both with fifteen fractional digits.
** Second algorithm's estimated 1/Pi value after four iterations followed by 1/M_PI both with fifteen fractional digits.
** Third algorithm's estimated 1/Pi value after four iterations followed by 1/M_PI both with fifteen fractional digits.
**
** Algorithm: First algorithm declares and initializes three variables, 'a', 'b', and 'p' which are run through a
** for loop for four iterations, updating each variable and printing both the iteration value 'n'
** as well as 'p' with fifteen fractional digits.
** After the loop, printing the math library value M_PI with fifteen fractional digits
** to compare value found by algorithm against.
** Second algorithm declares variables 's' and 'r', initializes 's', and 'a', and runs a for loop for four iterations,
** updating each variable and printing both the iteration value 'k' as well as 'a' with fifteen fractional digits.
** After the loop, printing the math library value M_PI with fifteen fractional digits
** to compare value found by algorithm against.
** Third algorithm declares variable 'y', initializes 'a' and 'y', and runs a for loop for four iterations,
** updating each variable and printing both the iteration value 'k' as well as 'a' with fifteen fractional digits.
** After the loop, printing the math library value M_PI with fifteen fractional digits
** to compare value found by algorithm against.
**
** Caveats: No user input; only 15 fractional digits of M_PI are used, so each algorithm's effectiveness in getting the correct
** fractional digits is hard to judge past four iterations.
**
** Version: 1.0
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(void) {


	printf("First Algorithm:\n"); // FIRST ALGORITHM
	double a = M_SQRT2, b = 0, p = 2 + M_SQRT2; // initializing variables

	for (int n = 1; n <= 4; ++n) {
		b = ((1+b) * sqrt(a)) / (a + b); // iterate 'b' first, uses current 'a' value
		a = (sqrt(a) + (1 / sqrt(a))) / 2; // iterate 'a'
		p = ((1 + a) * p * b) / (1 + b); // iterate 'p' last, uses new 'a' and 'b' values

		printf("%d\n", n); // printing iteration value 'n'
		printf("%.15lf\n", p); // compare to M_PI
	}
	printf("This is M_PI: %.15lf\n", M_PI); // this is M_PI
	printf("\n");


	printf("Second Algorithm:\n"); // SECOND ALGORITHM
	a = (1.0/3); // a already declared
	double s = (sqrt(3) - 1.0) / 2.0, r;

	for (int k = 0; k < 4; ++k) {
		r = (3.0) / (1 + 2*pow((1 - pow(s,3)),(1.0/3))); // iterate 'r' first, uses current value of 's'
		s = (r - 1) / 2.0; // iterate 's', uses new value of 'r'
		a = pow(r,2) * a - pow(3,k)*(pow(r,2) - 1); //iterate a, uses current 'a' and new 'r'

		printf("%d\n", k); // printing iteration value 'k'
		printf("%.15lf\n", a); // compare to 1/M_PI
	}
	printf("This is 1/M_PI: %.15lf\n", 1.0/M_PI); // this is 1/M_PI
	printf("\n");


	printf("Third Algorithm: \n"); // THIRD ALGORITHM
	a = 2*pow((M_SQRT2 - 1),2);
	double y = M_SQRT2 - 1;

	for (int k = 0; k < 4; ++k) {
		y = (1 - pow(1 - pow(y,4), (1.0/4))) / (1 + pow(1 - pow(y,4), (1.0/4))); // iterate 'y' first, use current value of 'y'
		a = a*pow(1 + y, 4) - pow(2, 2*k+3) * y * (1 + y + pow(y,2)); // iterate 'a', uses new 'y'

		printf("%d\n", k); // printing iteration value 'k'
		printf("%.15lf\n", a); // compare to 1/M_PI
	}
	printf("This is 1/M_PI: %.15lf\n", 1.0/M_PI); // this is 1/M_PI
	printf("\n");


	return 0;
}
