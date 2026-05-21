#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <cstddef>
#include <vector>

class MemoryPool {
public:
    // Constructor to initialize the memory pool with a default chunk size
    MemoryPool(size_t chunk_size = 1024);

    // Destructor to free all allocated memory chunks
    ~MemoryPool();

    // Allocate memory block of requested size
    int* allocate(size_t size);

    // Deallocate previously allocated memory
    void deallocate(int* ptr);

    // Tracking allocation counts
    size_t get_alloc_count() const { return alloc_count; }
    size_t get_dealloc_count() const { return dealloc_count; }

private:
    size_t chunk_size;            // Chunk size for allocation
    std::vector<int*> pool;       // All allocated chunks
    std::vector<int*> free_list;  // Free blocks stack
    size_t alloc_count = 0;
    size_t dealloc_count = 0;
};

#endif // MEMORY_POOL_H