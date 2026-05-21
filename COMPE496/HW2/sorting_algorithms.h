#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

#include <vector>
#include "memory_pool.h"

// Insertion Sort for small arrays
void insertion_sort(std::vector<int>& arr, int low, int high);

// Partition function for QuickSort
int partition(std::vector<int>& arr, int low, int high);

// QuickSort function
void quicksort(std::vector<int>& arr, int low, int high, int max_depth, MemoryPool* pool = nullptr);

// Heapify function for HeapSort
void heapify(std::vector<int>& arr, int n, int i, int offset);

// HeapSort function
void heap_sort(std::vector<int>& arr, int low, int high, MemoryPool* pool = nullptr);

// Introsort function
void introsort(std::vector<int>& arr, int low, int high, MemoryPool* pool = nullptr);

// Introsort utility to handle depth switching
void introsort_util(std::vector<int>& arr, int low, int high, int max_depth, MemoryPool* pool = nullptr);

// Overload to start Introsort with MemoryPool or not
void introsort(std::vector<int>& arr, MemoryPool* pool = nullptr);

#endif // SORTING_ALGORITHMS_H