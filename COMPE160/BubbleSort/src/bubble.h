#ifndef BUBBLE_H_
#define BUBBLE_H_

#include <stdio.h>
#include <stdlib.h>

#define SIZE 100000 	// this is N
extern unsigned int comparisons;
extern unsigned int swaps;

// print array function
void PrintArray(int *array);

// swap function
void swap(int *element1Ptr, int *element2Ptr);

// BubbleSort function
void BubbleSort(int *array);

#endif /* BUBBLE_H_ */
