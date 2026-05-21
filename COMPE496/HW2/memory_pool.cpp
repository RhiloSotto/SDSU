#include "memory_pool.h"
#include <cstdlib>
#include <iostream>

MemoryPool::MemoryPool(size_t chunk_size) : chunk_size(chunk_size) {}

MemoryPool::~MemoryPool() {
    for (int* block : pool) {
        free(block);
    }
    pool.clear();
    free_list.clear();
}

// Allocate memory from pool or system
int* MemoryPool::allocate(size_t size) {
    alloc_count++;
    // take a free block from the pool
    if (!free_list.empty()) {
        int* block = free_list.back();
        free_list.pop_back();
        return block;
    } else {
        int* block = (int*)malloc(sizeof(int) * size);
        pool.push_back(block);
        return block;
    }
}

// Return memory to pool
void MemoryPool::deallocate(int* ptr) {
    dealloc_count++;
    // return block back to pool
    free_list.push_back(ptr);
}