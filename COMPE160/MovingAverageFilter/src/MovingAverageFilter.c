/*
** Assignment: Laboratory Assignment 09 "Moving Average Filter"
**
** Filename: MovingAverageFilter.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: 10/24/2023, 9PM
**
** Description: given a .dat file, outputs the discrete-time moving average filter .dat file for a
** specified filter order
**
** Input: signal.dat
**
** Output: .dat file with filter order N (manually set N and file name)
**
** Algorithm: read file "signal.dat" and write into an array, iterating through every data point of y and
** for each, N+1 data points of x
**
** Caveats: must manually set/hard code filterOrder and file name, couldn't make
** an integer into a string to turn into a file name
**
** Version: 1.0
**
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 578

int main(void) {
	FILE* inFile = NULL;

	inFile = fopen("signal.dat", "r");
	if (inFile == NULL) {
		printf("Could not open file");
		return -1;
	}

	// inputs contents of signal.dat into array
	double x[MAX_SIZE] = {0};

	size_t n = 0;
	fscanf(inFile, "%lf", &x[n]);
	while (!feof(inFile)) {
			++n;
		fscanf(inFile, "%lf", &x[n]);
	}
	fclose(inFile);


	// MOVING AVERAGE FILTER
	FILE* outFile = NULL;
	double y[MAX_SIZE] = {0};
	int filterOrder = 100; // N = 10, N = 100, changes filter order
	double b = 1.0 / (filterOrder + 1.0);

	for (n = filterOrder; n < MAX_SIZE; ++n) { // n = filterOrder to get the zeros at the start
		for (size_t i = 0; i <= filterOrder; ++i) {
			y[n - filterOrder/2] += b * (x[n - i]); // y[n - filterOrder/2] to end on zeros
		}
	}

	outFile = fopen("y100.dat", "w"); // N = 10, N = 100, change file to match filter order
	if (outFile == NULL) {
		printf("Could not open file.\n");
		return -1;
	}
	for (n = 0; n < MAX_SIZE; ++n) {
		fprintf(outFile, "%.3lf\n", y[n]);
	}
	fclose(outFile);


	return 0;
}
