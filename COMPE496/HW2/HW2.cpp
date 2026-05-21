#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "memory_pool.h"
#include "sorting_algorithms.h"

void print_array(const std::vector<int>& arr) {
    for(int v : arr) std::cout << v << " ";
    std::cout << "\n";
}

bool is_sorted(const std::vector<int>& arr) {
    for(size_t i = 1; i < arr.size(); i++)
        if(arr[i] < arr[i-1]) return false;
    return true;
}

void run_test(const char* name, std::vector<int> arr, MemoryPool* pool) {
    std::cout << "Test: " << name << "\nBefore: ";
    print_array(arr);

    introsort(arr, pool);

    std::cout << "After:  ";
    print_array(arr);
    std::cout << "Result: " << (is_sorted(arr) ? "PASS" : "FAIL") << "\n";
    if(pool)
        std::cout << "Allocations: " << pool->get_alloc_count()
                  << ", Deallocations: " << pool->get_dealloc_count() << "\n";
    std::cout << "-------------------------\n";
}

// Helper functions to generate arrays
std::vector<int> generate_random(int size, int max_val = 1000) {
    std::vector<int> arr(size);
    for(int i = 0; i < size; i++)
        arr[i] = rand() % max_val;
    return arr;
}

std::vector<int> generate_reversed(int size) {
    std::vector<int> arr(size);
    for(int i = 0; i < size; i++)
        arr[i] = size - i;
    return arr;
}

std::vector<int> generate_equal(int size, int value) {
    return std::vector<int>(size, value);
}

std::vector<int> generate_mixed(int size) {
    std::vector<int> arr(size);
    for(int i = 0; i < size; i++)
        arr[i] = (rand() % 2000) - 1000; // range -1000 to 999
    return arr;
}

int main() {
    srand((unsigned)time(0));
    MemoryPool pool(5000);

    std::cout << "=== Tests WITH MemoryPool ===\n";

    run_test("Small array", {5,2,9,1,3}, &pool);
    run_test("Large reversed array 100", generate_reversed(100), &pool);
    run_test("Large random array 1000", generate_random(1000), &pool);
    run_test("Duplicates", {3,1,2,3,3,1,2}, &pool);
    run_test("All equal elements", generate_equal(50, 7), &pool);
    run_test("Already sorted", {1,2,3,4,5,6,7}, &pool);
    run_test("Reversed", {7,6,5,4,3,2,1}, &pool);
    run_test("Empty array", {}, &pool);
    run_test("Single element", {42}, &pool);
    run_test("Random array", {5,3,8,1,9,2,4,7,6,0}, &pool);
    run_test("Mixed positives and negatives", generate_mixed(50), &pool);

    std::cout << "\n=== Tests WITHOUT MemoryPool ===\n";
    run_test("Small array", {5,2,9,1,3}, nullptr);
    run_test("Large reversed array 100", generate_reversed(100), nullptr);
    run_test("Large random array 1000", generate_random(1000), nullptr);
    run_test("Duplicates", {3,1,2,3,3,1,2}, nullptr);
    run_test("All equal elements", generate_equal(50, 7), nullptr);
    run_test("Already sorted", {1,2,3,4,5,6,7}, nullptr);
    run_test("Reversed", {7,6,5,4,3,2,1}, nullptr);
    run_test("Empty array", {}, nullptr);
    run_test("Single element", {42}, nullptr);
    run_test("Random array", {5,3,8,1,9,2,4,7,6,0}, nullptr);
    run_test("Mixed positives and negatives", generate_mixed(50), nullptr);

    return 0;
}