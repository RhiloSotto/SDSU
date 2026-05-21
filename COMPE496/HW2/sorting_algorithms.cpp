#include "sorting_algorithms.h"
#include <iostream>

// Helper: integer log2
int log2_int(int n) {
    int log = 0;
    while (n > 1) { n /= 2; log++; }
    return log;
}

// Insertion Sort
void insertion_sort(std::vector<int>& arr, int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Partition
int partition(std::vector<int>& arr, int low, int high) {
    // Choose the last element as the pivot
    int pivot = arr[high];
    // i will track the position where the next smaller-than-pivot element should go
    int i = low - 1;
    // Traverse the array from low to high-1
    for (int j = low; j < high; j++) {
        // If current element is less than or equal to pivot
        if (arr[j] <= pivot) {
            i++;  // move boundary of smaller elements forward
            std::swap(arr[i], arr[j]); // Swap current element with element at index i

        }
    }
    // Place the pivot so that elements to the left are <= pivot, to the right are > pivot
    std::swap(arr[i + 1], arr[high]);
    // Return the pivot index
    return i + 1;
}

// Heapify
void heapify_array(int* arr, int n, int i) {
    // Assume the current node is the largest
    int largest = i;
    // Calculate indices of left and right children
    int l = 2*i + 1;  // left child
    int r = 2*i + 2;  // right child
    // If left child exists and is greater than current largest, update largest
    if (l < n && arr[l] > arr[largest]) 
        largest = l;
    // If right child exists and is greater than current largest, update largest
    if (r < n && arr[r] > arr[largest]) 
        largest = r;
    // If the largest value is not the root node
    if (largest != i) {
        // Swap the root with the largest child
        std::swap(arr[i], arr[largest]);

        // Recursively heapify the affected subtree
        heapify_array(arr, n, largest);
    }
}

// HeapSort with optional MemoryPool
void heap_sort(std::vector<int>& arr, int low, int high, MemoryPool* pool) {
    int n = high - low + 1;

    // Allocate a temporary array using MemoryPool (or heap if pool is nullptr)
    int* temp = nullptr;
    if(pool) temp = pool->allocate(n);
    else temp = new int[n];  // fallback allocation

    // Copy current subarray to temp
    for(int i=0; i<n; i++) temp[i] = arr[low + i];

    // Build heap in temp array
    for (int i = n / 2 - 1; i >= 0; i--) {
        int largest = i;
        int l = 2*i + 1;
        int r = 2*i + 2;
        if(l < n && temp[l] > temp[largest]) largest = l;
        if(r < n && temp[r] > temp[largest]) largest = r;
        if(largest != i) std::swap(temp[i], temp[largest]);
    }

    // Extract elements back into arr
    for(int i = n - 1; i >= 0; i--) {
        arr[low + i] = temp[0];
        temp[0] = temp[i];
        heapify_array(temp, i, 0); // works on temp array directly
    }

    // Deallocate temp array
    if(pool) pool->deallocate(temp);
    else delete[] temp;
}

// QuickSort with optional MemoryPool
void quicksort(std::vector<int>& arr, int low, int high, int max_depth, MemoryPool* pool) {
    if (low < high) {
        if (max_depth == 0) {
            heap_sort(arr, low, high, pool);
            return;
        }
        int p = partition(arr, low, high);
        quicksort(arr, low, p - 1, max_depth - 1, pool);
        quicksort(arr, p + 1, high, max_depth - 1, pool);
    }
}

// Introsort utility
void introsort_util(std::vector<int>& arr, int low, int high, int max_depth, MemoryPool* pool) {
    int size = high - low + 1;
    
    if (size <= 16) {
        insertion_sort(arr, low, high);
        return;
    }

    if (max_depth == 0) {
        heap_sort(arr, low, high, pool);
        return;
    }

    int p = partition(arr, low, high);
    introsort_util(arr, low, p - 1, max_depth - 1, pool);
    introsort_util(arr, p + 1, high, max_depth - 1, pool);
}

// Introsort main
void introsort(std::vector<int>& arr, int low, int high, MemoryPool* pool) {
    int max_depth = 2 * log2_int(high - low + 1);
    introsort_util(arr, low, high, max_depth, pool);
}

// Introsort overload
void introsort(std::vector<int>& arr, MemoryPool* pool) {
    if(arr.empty()) return;
    introsort(arr, 0, arr.size() - 1, pool);
}