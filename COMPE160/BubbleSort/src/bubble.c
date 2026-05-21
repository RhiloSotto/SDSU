#include "bubble.h"
unsigned int comparisons = 0;
unsigned int swaps = 0;

// print array function
void PrintArray(int *array) {
	for (size_t i = 0; i < SIZE; ++i)
		printf("%zu: %d\n", i + 1, array[i]);
	printf("\n");
}

// swap function
void swap(int *element1Ptr, int *element2Ptr) {
	int hold = *element1Ptr;
	*element1Ptr = *element2Ptr;
	*element2Ptr = hold;
	++swaps;
}

// BubbleSort function
void BubbleSort(int *array) {
	for (size_t i = 0; i < SIZE; ++i) {
		for (size_t j = 0; j < SIZE - i - 1; ++j) {
			++comparisons;
			if (array[j] > array[j + 1])
				swap(&array[j], &array[j + 1]);
		}
	}
}
