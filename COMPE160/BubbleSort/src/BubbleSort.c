/*
** Assignment: Laboratory Assignment 11 "Bubble Sort"
**
** Filename: BubbleSort.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: 11/7/2023, 9PM
**
** Description: Creates an array with random elements given a size, sorts array,
** counting the number of comparisons and exchanges made while sorting
**
** Input: None
**
** Output: Size of array N, number of comparisons, and number of swaps,
** number of comparisons and exchanges increase with respect to N, exponentially,
** taking an increasing amount of time to finish running the program.
**
** Algorithm: creates an array given a random seed, sorts array with bubble sort
** prints array size, number of comparisons and swaps made by sorting algorithm
**
** Caveats: array size is hard coded, runtime is long for larger values of N
**
** Version: 1.0
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bubble.h"


int main(void) {
	int a[SIZE]; 	// initialize array a
	srand(time(NULL));

	for (size_t i = 0; i < SIZE; ++i) // initialize array with random numbers [1,N]
		a[i] = (rand() % SIZE) + 1;

	//PrintArray(a); 	// print unsorted array
	BubbleSort(a); 	// sort array
	//PrintArray(a); 	// print sorted array

	printf("N = %d\nComparisons: %u\nSwaps: %u\n", SIZE, comparisons, swaps);

	return 0;
}
